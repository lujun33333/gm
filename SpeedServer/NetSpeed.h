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

	// ��ȡ��ǰIP������
	IPData& ip_data = ip_map[ip];
	unique_lock<mutex> ip_lock(ip_data.ip_mutex);

	// �ȼ�¼����ʱ���
	ip_data.timestamps.push_back(now);

	//ȷ��ʱ�����ʱ��˳������
	std::sort(ip_data.timestamps.begin(), ip_data.timestamps.end());

	// �����������
	ip_data.timestamps.erase(remove_if(ip_data.timestamps.begin(), ip_data.timestamps.end(),
		[now](const steady_clock::time_point& tp) {
			return duration_cast<milliseconds>(now - tp).count() > tcpon[1];
		}), ip_data.timestamps.end());


	// ͳ���������
	if (ip_data.timestamps.size() > tcpon[2]) {
		// ����ʱ���
		auto first_time = ip_data.timestamps.front();
		auto last_time = ip_data.timestamps.back();
		auto time_diff = duration_cast<milliseconds>(last_time - first_time).count();

		std::wstring ipAddress = Utf8ToWide(ip);

		// ʹ�� find ��������Ԫ��
		std::wstring speed = L"Speed";
		speed += ipAddress;

		auto it = firewallRuleNames.find(speed);
		// ����Ƿ��ҵ�
		if (it != firewallRuleNames.end()) {

		}
		else {
			addFirewallRuleAdd(ipAddress.c_str());
			std::cout << "IP " << ip << " ָ��ʱ��" << tcpon[1] << "�����ڳ������ƴ����� " << tcpon[2] << " �Σ�ʱ���� " << time_diff << " ����.\n";
		}

		// ɾ��IP
		ip_map.erase(ip);
		return;
	}
}



//std::vector<std::wstring> allIPs;
//std::mutex mtx; // ������
std::wstring generateRandomName(int length) {
	const wchar_t charset[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	const int charsetSize = sizeof(charset) / sizeof(charset[0]) - 1; // ��ȥĩβ�Ŀ��ַ�
	std::wstring randomString;

	srand(static_cast<unsigned int>(time(0))); // ʹ�õ�ǰʱ����Ϊ����

	for (int i = 0; i < length; ++i) {
		randomString += charset[rand() % charsetSize];
	}

	return L"Speed" + randomString;
}


// ��������ͳ���ض� IP ��ַ���ֵĴ���
int getCountIp(const std::vector<std::string>& ips, const std::string& targetIP) {
	int count = 0;

	// ���� vector �е�ÿһ�� IP ��ַ
	for (const auto& ip : ips) {
		// �����ǰ IP ��ַ��Ŀ�� IP ��ַ��ͬ���������һ
		if (ip == targetIP) {
			count++;
		}
	}

	return count;
}


void setFirewallRuleName(INetFwRule* pFwRule, const wchar_t* name) {
	_bstr_t bstrName(name); // �� wchar_t* ת��Ϊ BSTR
	HRESULT hr = pFwRule->put_Name(bstrName); // ���÷���ǽ��������
	if (FAILED(hr)) {
		std::cerr << "Failed to set firewall rule name." << std::endl;
	}
}


std::wstring GetIPPrefix(const std::wstring& ruleName) {
	// �ҵ� IP ��ַ����ʼλ�ã��� Speed ���λ�ã�
	size_t startPos = ruleName.find(L"Speed") + 5; // �ҵ� Speed ���λ��

	// �ҵ� IP ��ַ�Ľ���λ�ã����֮��ĵڶ�����Ż����ַ���������
	size_t endPos = ruleName.find(L".", startPos);
	endPos = ruleName.find(L".", endPos + 1);

	// ��ȡ IP ��ַ��ǰ����
	if (startPos != std::string::npos && endPos != std::string::npos) {
		return ruleName.substr(startPos, endPos - startPos);
	}
	else {
		return L""; // ����޷���ȡ��Ч�� IP ǰ׺�����ؿ��ַ���
	}
}

void countIp() {
	std::unordered_map<std::wstring, int> prefixCount;

	// ͳ��ǰ����IP��ͬ�Ĺ�������
	for (const auto& ruleName : firewallRuleNames) {
		// ʹ�� ruleName ���в���
		std::wstring prefix = GetIPPrefix(ruleName);

		if (prefixCount.find(prefix) == prefixCount.end()) {
			prefixCount[prefix] = 1;
		}
		else {
			prefixCount[prefix]++;
		}
	}

	// ��ӡ���ֳ���5�ε�ǰ����IP��ͬ�Ĺ�������
	for (const auto& pair : prefixCount) {
		if (pair.second > 5) {
			std::wstring prefix = pair.first;
			if (prefix.length() > 0) {
				prefix = prefix.append(L".");
				prefix = generateCIDRPrefix(prefix);
				std::wstring speedfind = L"Speed";
				speedfind += prefix;
				auto it = firewallPrefixNames.find(speedfind);

				// ����Ƿ��ҵ�
				if (it != firewallPrefixNames.end()) {
					continue;
				}

				std::wcout << "����ǰ׺ " << prefix << " ���� " << pair.second << " ��,���ip" << prefix << std::endl;
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

	SysFreeString(bstrInterfaceTypes1);  // �ͷ� BSTR �ַ���
	SysFreeString(bstrInterfaceTypes2);  // �ͷ� BSTR �ַ���

	std::wstring speedIpAddressName = L"Speed";
	speedIpAddressName += ipPrefix;
	firewallPrefixNames.insert(speedIpAddressName);
	setFirewallRuleName(pFwRule, speedIpAddressName.c_str());
	//pFwRule->put_Name(randomName.c_str());

	// ת��ipPrefixΪBSTR����
	BSTR bstrRemotePrefix = SysAllocString(ipPrefix);
	if (!bstrRemotePrefix) {
		std::cerr << "Failed to allocate BSTR." << std::endl;
		pFwRule->Release();
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// ����Զ�̵�ַΪָ����IPǰ׺
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

	// ʹ�� find ��������Ԫ��
	std::wstring speed = L"Speed";
	speed += ipAddress;
	auto it = firewallRuleNames.find(speed);

	// ����Ƿ��ҵ�
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

	SysFreeString(bstrInterfaceTypes1);  // �ͷ� BSTR �ַ���
	SysFreeString(bstrInterfaceTypes2);  // �ͷ� BSTR �ַ���

	std::wstring speedIpAddressName = L"Speed";
	speedIpAddressName += ipAddress;
	setFirewallRuleName(pFwRule, speedIpAddressName.c_str());

	// ת��ipAddressΪBSTR����
	BSTR bstrRemoteAddress = SysAllocString(ipAddress);
	if (!bstrRemoteAddress) {
		std::cerr << "Failed to allocate BSTR." << std::endl;
		pFwRule->Release();
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// ����Զ�̵�ַΪָ����IP��ַ
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

	// �� wchar_t* ת��Ϊ BSTR
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
	SysFreeString(bstrRuleName); // �ͷ� BSTR
	if (FAILED(hr)) {
		std::cerr << "Failed to find firewall rule with name: " << ruleName << std::endl;
		pFwRules->Release();
		pFwPolicy2->Release();
		CoUninitialize();
		return;
	}

	// ɾ������
	hr = pFwRules->Remove(bstrRuleName);
	if (FAILED(hr)) {
		std::cerr << "ɾ��IP����: " << ruleName << "ʧ��" << std::endl;
	}
	else {
		std::cout << "ɾ��IP����ɹ�" << std::endl; 
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

// ������������ CIDR ��ʽ��ǰ׺
std::wstring generateCIDRPrefix(const std::wstring& commonPrefix) {
	// ��������ֻ���� IPv4 ��ַ
	// Ѱ�������ǰ׺�����һ�����λ��
	size_t lastDot = commonPrefix.find_last_of(L'.');
	if (lastDot != std::wstring::npos) {
		// ��ȡǰ׺���֣�û�����һ�������Ĳ��֣�
		std::wstring prefix = commonPrefix.substr(0, lastDot);
		// ��� CIDR ��׺
		prefix += L".0.0/16"; // �������ʹ�� /24 ���������볤��
		return prefix;
	}
	// ����Ҳ������һ���㣬���ؿ��ַ�����Ĭ�ϵ� CIDR ��ʽ
	return L"";
}

// ��ȡIP��ַ��ǰ��������Ϊǰ׺
std::wstring extractPrefix(const std::wstring& ip) {
	size_t pos = ip.find('.', ip.find('.') + 1); // �ҵ��ڶ������λ��
	if (pos != std::wstring::npos) {
		return ip.substr(0, pos);
	}
	return ip; // ���IP��ʽ����ȷ����������IP
}

// �ж�����wstring�Ƿ�����ͬǰ׺�ĺ���
bool hasSamePrefix(const std::wstring& ip1, const std::wstring& ip2) {
	if (ip1 == ip2) {
		return false; // �������IP��ַ��ȣ�ֱ�ӷ���false
	}
	std::wstring prefix1 = extractPrefix(ip1);
	std::wstring prefix2 = extractPrefix(ip2);
	return prefix1 == prefix2;
}

// ɾ��������ͬǰ׺��IP��ַ����������ͬǰ׺��ǰ׺ֵ
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


