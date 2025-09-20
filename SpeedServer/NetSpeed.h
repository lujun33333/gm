#include <windows.h>
#include <netfw.h>
#include <iostream>
#include <string>
#include <comutil.h>
#include <unordered_set>
#include <codecvt>
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "comsuppw.lib")  // For _bstr_t

#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;


struct RequestRecord {
	vector<steady_clock::time_point> timestamps;
};
struct IPData {
	vector<steady_clock::time_point> timestamps;
	mutex ip_mutex;
};

void addFirewallRuleAdd(const wchar_t* ipAddress);
void addFirewallRule(const wchar_t* ipPrefix);
bool isValidIPv4(const std::string& ip);
std::wstring generateCIDRPrefix(const std::wstring& commonPrefix);
extern int tcpon[3];

HRESULT hr = S_OK;
INetFwPolicy2* pFwPolicy2 = NULL;
INetFwRules* pFwRules = NULL;
std::unordered_set<std::wstring> firewallRuleNames;
std::unordered_set<std::wstring> firewallPrefixNames;
std::vector<std::string> ipAddresses;
unordered_map<string, RequestRecord> ipRequestMap;
const int maxRequestsPerSecond = 3;
const int monitoringWindow = 1; // seconds
std::mutex ipRequestMapMutex;


unordered_map<string, IPData> ip_map;
mutex global_mtx;

void process_ip(const string& ip) {
	unique_lock<mutex> lock(global_mtx);
	auto now = steady_clock::now();

	// 获取当前IP的数据
	IPData& ip_data = ip_map[ip];
	unique_lock<mutex> ip_lock(ip_data.ip_mutex);

	// 先记录请求时间戳
	ip_data.timestamps.push_back(now);

	//确保时间戳按时间顺序排列
	std::sort(ip_data.timestamps.begin(), ip_data.timestamps.end());

	// 清理过期数据
	ip_data.timestamps.erase(remove_if(ip_data.timestamps.begin(), ip_data.timestamps.end(),
		[now](const steady_clock::time_point& tp) {
			return duration_cast<milliseconds>(now - tp).count() > tcpon[1];
		}), ip_data.timestamps.end());


	// 统计请求次数
	if (ip_data.timestamps.size() > tcpon[2]) {
		// 计算时间差
		auto first_time = ip_data.timestamps.front();
		auto last_time = ip_data.timestamps.back();
		auto time_diff = duration_cast<milliseconds>(last_time - first_time).count();

		std::wstring ipAddress = Utf8ToWide(ip);

		// 使用 find 函数查找元素
		std::wstring speed = L"Speed";
		speed += ipAddress;

		auto it = firewallRuleNames.find(speed);
		// 检查是否找到
		if (it != firewallRuleNames.end()) {

		}
		else {
			addFirewallRuleAdd(ipAddress.c_str());
			std::cout << "IP " << ip << " 指定时间" << tcpon[1] << "毫秒内超过限制次数： " << tcpon[2] << " 次，时间是 " << time_diff << " 毫秒.\n";
		}

		// 删除IP
		ip_map.erase(ip);
		return;
	}
}



//std::vector<std::wstring> allIPs;
//std::mutex mtx; // 互斥锁
std::wstring generateRandomName(int length) {
	const wchar_t charset[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	const int charsetSize = sizeof(charset) / sizeof(charset[0]) - 1; // 减去末尾的空字符
	std::wstring randomString;

	srand(static_cast<unsigned int>(time(0))); // 使用当前时间作为种子

	for (int i = 0; i < length; ++i) {
		randomString += charset[rand() % charsetSize];
	}

	return L"Speed" + randomString;
}


// 函数用于统计特定 IP 地址出现的次数
int getCountIp(const std::vector<std::string>& ips, const std::string& targetIP) {
	int count = 0;

	// 遍历 vector 中的每一个 IP 地址
	for (const auto& ip : ips) {
		// 如果当前 IP 地址与目标 IP 地址相同，则计数加一
		if (ip == targetIP) {
			count++;
		}
	}

	return count;
}


void setFirewallRuleName(INetFwRule* pFwRule, const wchar_t* name) {
	_bstr_t bstrName(name); // 将 wchar_t* 转换为 BSTR
	HRESULT hr = pFwRule->put_Name(bstrName); // 设置防火墙规则名称
	if (FAILED(hr)) {
		std::cerr << "Failed to set firewall rule name." << std::endl;
	}
}


std::wstring GetIPPrefix(const std::wstring& ruleName) {
	// 找到 IP 地址的起始位置（即 Speed 后的位置）
	size_t startPos = ruleName.find(L"Speed") + 5; // 找到 Speed 后的位置

	// 找到 IP 地址的结束位置（点号之后的第二个点号或者字符串结束）
	size_t endPos = ruleName.find(L".", startPos);
	endPos = ruleName.find(L".", endPos + 1);

	// 提取 IP 地址的前两段
	if (startPos != std::string::npos && endPos != std::string::npos) {
		return ruleName.substr(startPos, endPos - startPos);
	}
	else {
		return L""; // 如果无法提取有效的 IP 前缀，返回空字符串
	}
}

void countIp() {
	std::unordered_map<std::wstring, int> prefixCount;

	// 统计前两段IP相同的规则数量
	for (const auto& ruleName : firewallRuleNames) {
		// 使用 ruleName 进行操作
		std::wstring prefix = GetIPPrefix(ruleName);

		if (prefixCount.find(prefix) == prefixCount.end()) {
			prefixCount[prefix] = 1;
		}
		else {
			prefixCount[prefix]++;
		}
	}

	// 打印出现超过5次的前两段IP相同的规则名称
	for (const auto& pair : prefixCount) {
		if (pair.second > 5) {
			std::wstring prefix = pair.first;
			if (prefix.length() > 0) {
				prefix = prefix.append(L".");
				prefix = generateCIDRPrefix(prefix);
				std::wstring speedfind = L"Speed";
				speedfind += prefix;
				auto it = firewallPrefixNames.find(speedfind);

				// 检查是否找到
				if (it != firewallPrefixNames.end()) {
					continue;
				}

				std::wcout << "规则前缀 " << prefix << " 出现 " << pair.second << " 次,封禁ip" << prefix << std::endl;
				addFirewallRule(prefix.c_str());
			}
		}
	}

}


void addFirewallRule(const wchar_t* ipPrefix) {

	hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		std::cerr << "Failed to initialize COM." << std::endl;
		return;
	}

	hr = CoCreateInstance(
		__uuidof(NetFwPolicy2),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwPolicy2),
		(void**)&pFwPolicy2
	);
	if (FAILED(hr)) {
		std::cerr << "Failed to create INetFwPolicy2 instance." << std::endl;
		CoUninitialize();
		return;
	}

	hr = pFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr)) {
		std::cerr << "Failed to get INetFwRules instance." << std::endl;
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	INetFwRule* pFwRule = NULL;
	hr = CoCreateInstance(
		__uuidof(NetFwRule),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwRule),
		(void**)&pFwRule
	);
	if (FAILED(hr)) {
		std::cerr << "Failed to create INetFwRule instance." << std::endl;
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}
	BSTR bstrInterfaceTypes1 = SysAllocString(L"Block IP Prefix");
	BSTR bstrInterfaceTypes2 = SysAllocString(L"All");

	pFwRule->put_Action(NET_FW_ACTION_BLOCK);
	pFwRule->put_Description(bstrInterfaceTypes1);
	pFwRule->put_Direction(NET_FW_RULE_DIR_IN);
	pFwRule->put_Enabled(VARIANT_TRUE);
	pFwRule->put_InterfaceTypes(bstrInterfaceTypes2);

	SysFreeString(bstrInterfaceTypes1);  // 释放 BSTR 字符串
	SysFreeString(bstrInterfaceTypes2);  // 释放 BSTR 字符串

	std::wstring speedIpAddressName = L"Speed";
	speedIpAddressName += ipPrefix;
	firewallPrefixNames.insert(speedIpAddressName);
	setFirewallRuleName(pFwRule, speedIpAddressName.c_str());
	//pFwRule->put_Name(randomName.c_str());

	// 转换ipPrefix为BSTR类型
	BSTR bstrRemotePrefix = SysAllocString(ipPrefix);
	if (!bstrRemotePrefix) {
		std::cerr << "Failed to allocate BSTR." << std::endl;
		pFwRule->Release();
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// 设置远程地址为指定的IP前缀
	pFwRule->put_RemoteAddresses(bstrRemotePrefix);

	SysFreeString(bstrRemotePrefix);

	hr = pFwRules->Add(pFwRule);
	if (FAILED(hr)) {
		std::cerr << "Failed to add firewall rule." << std::endl;
	}

	pFwRule->Release();
	pFwRules->Release();
	pFwPolicy2->Release();
	CoUninitialize();
}

void addFirewallRuleAdd(const wchar_t* ipAddress) {

	// 使用 find 函数查找元素
	std::wstring speed = L"Speed";
	speed += ipAddress;
	auto it = firewallRuleNames.find(speed);

	// 检查是否找到
	if (it != firewallRuleNames.end()) {
		return;
	}

	countIp();


	HRESULT hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		std::cerr << "Failed to initialize COM." << std::endl;
		return;
	}

	INetFwPolicy2* pFwPolicy2 = NULL;
	hr = CoCreateInstance(
		__uuidof(NetFwPolicy2),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwPolicy2),
		(void**)&pFwPolicy2
	);
	if (FAILED(hr)) {
		std::cerr << "Failed to create INetFwPolicy2 instance." << std::endl;
		CoUninitialize();
		return;
	}

	INetFwRules* pFwRules = NULL;
	hr = pFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr)) {
		std::cerr << "Failed to get INetFwRules instance." << std::endl;
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	INetFwRule* pFwRule = NULL;
	hr = CoCreateInstance(
		__uuidof(NetFwRule),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwRule),
		(void**)&pFwRule
	);
	if (FAILED(hr)) {
		std::cerr << "Failed to create INetFwRule instance." << std::endl;
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	BSTR bstrInterfaceTypes1 = SysAllocString(L"Block IP Prefix");
	BSTR bstrInterfaceTypes2 = SysAllocString(L"All");

	pFwRule->put_Action(NET_FW_ACTION_BLOCK);
	pFwRule->put_Description(bstrInterfaceTypes1);
	pFwRule->put_Direction(NET_FW_RULE_DIR_IN);
	pFwRule->put_Enabled(VARIANT_TRUE);
	pFwRule->put_InterfaceTypes(bstrInterfaceTypes2);

	SysFreeString(bstrInterfaceTypes1);  // 释放 BSTR 字符串
	SysFreeString(bstrInterfaceTypes2);  // 释放 BSTR 字符串

	std::wstring speedIpAddressName = L"Speed";
	speedIpAddressName += ipAddress;
	setFirewallRuleName(pFwRule, speedIpAddressName.c_str());

	// 转换ipAddress为BSTR类型
	BSTR bstrRemoteAddress = SysAllocString(ipAddress);
	if (!bstrRemoteAddress) {
		std::cerr << "Failed to allocate BSTR." << std::endl;
		pFwRule->Release();
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// 设置远程地址为指定的IP地址
	pFwRule->put_RemoteAddresses(bstrRemoteAddress);

	SysFreeString(bstrRemoteAddress);

	hr = pFwRules->Add(pFwRule);
	if (FAILED(hr)) {
		std::cerr << "Failed to add firewall rule." << std::endl;
	}
	/*else {
		std::wcout << L"Firewall rule Add: " << speedIpAddressName << std::endl;
	}*/

	firewallRuleNames.insert(speedIpAddressName);
	pFwRule->Release();
	pFwRules->Release();
	pFwPolicy2->Release();
	CoUninitialize();
}


void removeFirewallRule(const wchar_t* ruleName) {
	HRESULT hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		std::cerr << "Failed to initialize COM." << std::endl;
		return;
	}

	INetFwPolicy2* pFwPolicy2 = NULL;
	hr = CoCreateInstance(
		__uuidof(NetFwPolicy2),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwPolicy2),
		(void**)&pFwPolicy2
	);
	if (FAILED(hr)) {
		std::cerr << "Failed to create INetFwPolicy2 instance." << std::endl;
		CoUninitialize();
		return;
	}

	INetFwRules* pFwRules = NULL;
	hr = pFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr)) {
		std::cerr << "Failed to get INetFwRules instance." << std::endl;
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// 将 wchar_t* 转换为 BSTR
	BSTR bstrRuleName = SysAllocString(ruleName);
	if (!bstrRuleName) {
		std::cerr << "Failed to allocate BSTR." << std::endl;
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	INetFwRule* pFwRule = NULL;
	hr = pFwRules->Item(bstrRuleName, &pFwRule);
	SysFreeString(bstrRuleName); // 释放 BSTR
	if (FAILED(hr)) {
		std::cerr << "Failed to find firewall rule with name: " << ruleName << std::endl;
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// 删除规则
	hr = pFwRules->Remove(bstrRuleName);
	if (FAILED(hr)) {
		std::cerr << "删除IP规则: " << ruleName << "失败" << std::endl;
	}
	else {
		std::cout << "删除IP规则成功" << std::endl; 
	}


	pFwRule->Release();
	pFwRules->Release();
	pFwPolicy2->Release();
	CoUninitialize();
}

void removeFirewallRuleName() {
	for (const auto& ruleName : firewallRuleNames) {
		std::wcout << L"Deleting firewall rule with name: " << ruleName << std::endl;
		removeFirewallRule(ruleName.c_str());
	}
}


void addRule(const wchar_t* ipPrefix, std::unordered_set<std::wstring>& prefixSet) {
	std::wstring prefix(ipPrefix);
	prefixSet.insert(prefix);
	std::wcout << L"Added firewall rule for IP prefix: " << prefix << std::endl;
}

void removeRule(const wchar_t* ipPrefix, std::unordered_set<std::wstring>& prefixSet) {
	std::wstring prefix(ipPrefix);
	auto it = prefixSet.find(prefix);
	if (it != prefixSet.end()) {
		prefixSet.erase(it);
		std::wcout << L"Removed firewall rule for IP prefix: " << prefix << std::endl;
	}
	else {
		std::wcout << L"No firewall rule found for IP prefix: " << prefix << std::endl;
	}
}

// 函数用于生成 CIDR 格式的前缀
std::wstring generateCIDRPrefix(const std::wstring& commonPrefix) {
	// 假设这里只处理 IPv4 地址
	// 寻找最长公共前缀的最后一个点的位置
	size_t lastDot = commonPrefix.find_last_of(L'.');
	if (lastDot != std::wstring::npos) {
		// 提取前缀部分（没有最后一个点后面的部分）
		std::wstring prefix = commonPrefix.substr(0, lastDot);
		// 添加 CIDR 后缀
		prefix += L".0.0/16"; // 这里假设使用 /24 的子网掩码长度
		return prefix;
	}
	// 如果找不到最后一个点，返回空字符串或默认的 CIDR 形式
	return L"";
}

// 提取IP地址的前两部分作为前缀
std::wstring extractPrefix(const std::wstring& ip) {
	size_t pos = ip.find('.', ip.find('.') + 1); // 找到第二个点的位置
	if (pos != std::wstring::npos) {
		return ip.substr(0, pos);
	}
	return ip; // 如果IP格式不正确，返回整个IP
}

// 判断两个wstring是否有相同前缀的函数
bool hasSamePrefix(const std::wstring& ip1, const std::wstring& ip2) {
	if (ip1 == ip2) {
		return false; // 如果两个IP地址相等，直接返回false
	}
	std::wstring prefix1 = extractPrefix(ip1);
	std::wstring prefix2 = extractPrefix(ip2);
	return prefix1 == prefix2;
}

// 删除具有相同前缀的IP地址，并返回相同前缀的前缀值
std::wstring removeIPsWithSamePrefix(std::vector<std::wstring>& ips, const std::wstring& newIP) {
	std::wstring matchedPrefix;

	auto it = std::remove_if(ips.begin(), ips.end(), [&](const std::wstring& ip) {
		if (ip != newIP && hasSamePrefix(ip, newIP)) {
			matchedPrefix = extractPrefix(ip);
			return true;
		}
		return false;
		});

	ips.erase(it, ips.end());

	return matchedPrefix;
}


bool isValidIPv4(const std::string& ip) {
	std::istringstream stream(ip);
	std::string segment;
	std::vector<int> segments;

	while (std::getline(stream, segment, '.')) {
		try {
			int value = std::stoi(segment);
			if (value < 0 || value > 255) return false;
			segments.push_back(value);
		}
		catch (...) {
			return false;
		}
	}

	return segments.size() == 4;
}


