//#define VLD_FORCE_ENABLE
//#include <vld.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <unordered_map>
#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include "Message.h"
#include "Map.h"
#include "Item.h"
#include "Database.h"
#include <wincrypt.h>
#include <map>
#include <mutex>
#include <DbgHelp.h>
#include <time.h>
#include <iostream>
#include <filesystem> // C++17 文件系统支持
#include <conio.h>
#include "ConnectionPool.h"
#include <fstream>

#pragma comment(lib, "DbgHelp.lib")
#include <string>
#include <vector>
#include <windows.h>

#include "log.h"

#pragma comment(lib,"advapi32.lib")
#pragma comment (lib, "ws2_32.lib")

#pragma comment (lib,"HPSocket.lib")

#include "yaml-cpp/yaml.h"
#ifdef NDEBUG
#pragma comment (lib,"yaml-cpp.lib")
#else
#pragma comment (lib,"yaml-cppd.lib")
#endif

std::string IP;
USHORT TcpPort;
USHORT UdpPort;
std::string BindIP;
USHORT BindTcpPort;
USHORT BindUdpPort;
DWORD MyMainThreadId;
std::list<UINT> modecar;

extern ConnectionPool usr;
extern void initGameBegin();
extern bool is60fps;

//udp cs cmds
#define UDP_CMD_LOGIN_SERVER			0x01
#define UDP_CMD_P2P_GAME_DATA			0x02
#define UDP_CMD_SHOW_MY_IP_PORT			0x03
#define UDP_CMD_RECV_OTHER_IP_PORT	0x04
#define UDP_CMD_HEART_BEAT				0x05

//udp cc cmds
#define UDP_CMD_TEST_CONNECTION_NO_RECEIVE			0x20
#define UDP_CMD_TEST_CONNECTION_RECEIVED			0x21
#define UDP_CMD_UDP_BIDIRECTIONAL_OK				0x22
#define UDP_CMD_CLIENT_HELLO						0x25
#define UDP_CMD_CLIENT_P2P_DATA						0x26

#define UDP_CMD_TEST_NETSPEED						0x27
#define UDP_CMD_TEST_NETSPEED_REPLY					0x28


std::atomic<bool> stopFlag(false);
std::condition_variable cv;
std::mutex cvMutex;

std::string UITitleDispInfo_ADText;
const char* mytext;
extern char registerflag;
extern char propflag;
extern std::map<long, std::shared_ptr<ClientNode>> Clients;
extern std::recursive_mutex Mutex;
std::recursive_mutex ClientMutex;
std::recursive_mutex ConfigMutex;
std::string TransferModelStr;
char TransferModel = 0;//udp 0 tcp 1
int raceAdd = 0;
extern int Prob;
extern int ProbTime[2];
extern int GradeAddSet[6];
extern int LoseRate;
extern int Addexp;
extern UINT  AnonymousMode;
UINT SyncTime = 5000;
UINT DanceTime = 0;
UINT GameOverTime = 0;
UINT UnlockFPS = 0;
UINT isinvite = 0;
int StoneLvSeed[3] = { 80,60,30 };//默认概率
int BorderTimes[4] = { 0,4,20,23 };//边境倍数
extern int LoadKartPhys();
extern void PrintfKart127616();
extern void LoadKartAltasList();
extern void LoadSkillStoneCfg();
extern void LoadNPCKartAIPhysParaInfo();
extern void OnlineTest();
extern void OnlineUin();
extern void LoadTopList();
extern void UpdateTopList();
extern void LoadTopListDesc();
extern void LoadNPCListInfo();
extern void LoadKartSkill();
extern void LoadCommodityInfo();
extern void RankedMatchBeginTest();
extern void BorderMatchBeginTest();
extern void LoadSign3Cfg();
extern void LoadRegisterBox();
//extern void LoadRandomMapPool();
extern void LoadDayRaffleBox();
extern void LoadMagicFormationBoxs();
extern void LoadMagicExchangeShopBoxs();
extern void NotifyClientUpdateExp(std::shared_ptr<ClientNode> Client);
extern void LoadAuctionCfg();
extern void CheckAuctionTime();
extern void LoadAppellationConfig();
extern void LoadUserPermissionsConfig();
extern bool UpdateChallengeMapStatus(UINT mapId, USHORT rank, int newStatus);
extern void LoadMechTreasureInfoConfig();
//extern void LoadCarTreasure();
extern void LoadCommonBoxAwards();
extern void ReadCountryInfo();
int Rankset[5] = { 1,1,0,24,2 };
int RankTimes[4] = { 0,24,0,24 };
int ChangeTimes[4] = { 0,24,0,24 };

int Borderset[6] = { 1,1,10000,10000,10000,2 };//边境设置
int RankAdd[2] = { 0,10000 };//
char rankstone = 0;
int mode28346 = 1;
int BorderTeam = 0;
int notice = 0;
int isMinimum = 0;
int isRealTime[2] = { 0,6 };
int GMTime[4] = { 0,20,22,0 };
std::string GMMess = "";
int ranknoticeon = 0;
int SkipDancetime = 1000;
int mapLimit = 0;
int ismd5 = 0;
int isLog = 0;
int deleteflag = 0;
int bodyflag = 0;
int ipban = 0;
int GuildAdd = 6;
int AutoCls = 0;
int beiginmode = 0;

double version = 1.4; //版本号
std::string versionStr;
std::mutex Ipmtx; // 互斥锁
UINT raceid = 10020;
std::string ranknoticetext = "";
std::string week = "";
std::string bignotice = "";
std::string smallnotice = "";
SUPERPROP SuperProp[3] = { {18555,10} ,{19752,20},{19753,10} };
std::recursive_mutex RelaxClientMutex;
extern std::map<long, std::shared_ptr<ClientNode>> RelaxClients;
int LuckyLimit = 30000;
std::shared_ptr<ClientNode> SystemClient;
extern void LoadMagicLampCfg();
int MagicLamp[4] = { 1,30000,1000,127748 };//小橘子点灯概率
int MagicCup[2] = { 127748 ,50 }; //魔力圣杯
int DayMoeny = 100000;
int MagicExchangeShopItemId = 127748;
int MagicShop[2] = { 127748,200 }; //魔法阵消耗
int AllMapLimitTime = 50000;
int beginMode = 0;
int obs = 0;
int SwitchFlag1 = 0x0003BC27;
int StoneLimit = 0;
int NewKart = 0;
int Keymode = 1; //0是动态，1是写死
int Stake[2] = { 0,127748 };
int PreventSDP[2] = { 0,0 };
int BorderManTime = 0;
int RankerManTime = 0;
int loginMode = 0;
int LimitTalk = 0;
int Uinew = 0;
int AutoReplyGuild = 0;
int DisplayBitMapConfig[5] = { 1,0,1,1,1 };
int Mantissa[4] = { 0,200000,500000,133671 };
std::vector<UINT> ChallengeForbidKart;
std::vector<UINT> ForbidMaps;
std::vector<NewMapsInfo> NewMaps;
std::chrono::high_resolution_clock::time_point start;
YAML::Node ServerConfig;
extern void LoadMiracleCfg();
std::shared_ptr<ADVANCEDCFG> BorderAdvancedCfg = NULL;
std::shared_ptr<ADVANCEDCFG> RankAdvancedCfg = NULL;
extern void RunningLog();
extern void LoadOutDoorEggs();
extern void Reloadracingdata();
YAML::Node BaseGameConfig;
extern void initGameBegin();
extern UINT GameBeginSwitchFlag;
extern UINT FeedBackEnabled;
extern std::vector<UINT> GLSpeclEffectNum;
extern std::map<UINT, std::shared_ptr<LimitMsgInfo>> LimitMsgs;
extern std::map<UINT, std::shared_ptr<LimitMsgInfo>> Msgs;
extern std::vector<UINT> Uins;
extern std::vector<MapChallengeOpenMain> mapChallengeOpenMain;
extern std::vector<MapChallengeOpenMainMap> mapChallengeCreate;
int tcpon[3] = { 0,1000,3 };
int cdn[4] = { 0,0,60,2233 };
std::string Name;
std::string cdndomain = "";
std::vector<std::string> BroadCast;
std::vector<ChallengeMapsInfo> ChallengeMaps;
int topnew[2] = { 0,10 };
std::string topnews = "全服人数";
int BroadCaston = 0;
int IsBottomGame[4] = { 0,127748,1,1 };
int IsBottomGame4or6[2] = { 10,10 };
std::string qqurl;
std::string AdvertisingUrl;

std::vector<std::string> ChallengeCfgMsgs;
std::string ChallengeUrl;
int ChallengeFileOn;
std::string ChallengeFilePath;
std::mutex ChallengeMapsMutex;
std::vector<ChallengeMap> ChallengeAllMaps;
//BYTE LoginKey[16] = { '\x66','\x88','\x99','\x33','\x44','\x55','\x66','\x77','\x88','\x99','\x00','\xaa','\xbb','\xcc','\xdd','\xee' }; //我的KEY
BYTE LoginKey[16] = { '\x66','\x5F','\xAE','\x33','\x44','\xDF','\x99','\x11','\x88','\x99','\x00','\xaa','\x6D','\x78','\xdd','\xee' }; //

std::string LoginKeys = "HKYend$@*3591wbg"; //新版登录器key
std::mutex KeysMutex;
std::map<long, std::string> Keys;//密钥集合
UINT IsCS = 0;
FILE* LogFile;
std::mutex LogFileMutex;
std::chrono::steady_clock::time_point UpdateTime;
void addip2FireWall();
void addip2FireWall(UINT Uin);
void addip2FireWall(std::string IP);
void RemoveIp(std::string IP);
void addip2FireWall1();
void ClearTcp();
void TcpNoIPFireWall();
void TcpNoConnidFireWall();
void SelectTcptoUIN();
void AuToClearTcp();
void lookdns();
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif  // _WIND32
#include "pet.h"
#include "DBConnection.h"
#include "netSpeed.h"
#include <WS2tcpip.h>


// 定义64位整形
#if defined(_WIN32) && !defined(CYGWIN)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32

static void spdlog_init1() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm bt{};
#ifdef _WIN32
	localtime_s(&bt, &in_time_t);
#else
	localtime_r(&in_time_t, &bt);
#endif
	std::ostringstream oss;
	oss << std::put_time(&bt, "Logs/%Y%m%d%H%M%S_log.txt");
	std::string sFileName = oss.str();

	std::filesystem::create_directory("Logs");

	// 添加日志文件轮转功能，每达到5MB切换到新文件，最多保留3个旧文件
	auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(sFileName, 1024 * 1024 * 50, 10);

	spdlog::drop_all();
	spdlog::init_thread_pool(8192, 4);

	auto myLogger = std::make_shared<spdlog::logger>("SpeedServer_logger", rotating_sink);

	myLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l]: %v");
	myLogger->set_level(spdlog::level::info);

	spdlog::set_default_logger(myLogger);

	myLogger->info("[初始化日志库]");
	myLogger->flush();
}


void formatTimeStamp(long long microTimestamp, char* temp)
{
	std::time_t seconds_since_epoch = static_cast<time_t>(microTimestamp / 1000000);//将时长转换为秒
#if defined _MSC_VER && _MSC_VER >= 1400
	tm exact_time;
	localtime_s(&exact_time, &seconds_since_epoch);
	int tm_microsec = static_cast<int>(microTimestamp % 1000);
	int tm_millisec = static_cast<int>(microTimestamp / 1000 % 1000);
#elif defined __GNUC__
	tm exact_time;
	localtime_r(&seconds_since_epoch, &exact_time);
	int tm_microsec = static_cast<int>(microseconds_since_epoch % 1000);
	int tm_millisec = static_cast<int>(microseconds_since_epoch / 1000 % 1000);
#else
#error "Unknown compiler"
#endif


	snprintf(temp,
		27,
		"%04d%02d%02d",
		exact_time.tm_year + 1900,
		exact_time.tm_mon + 1,
		exact_time.tm_mday);

}


int GenerateMiniDump1(PEXCEPTION_POINTERS pExceptionPointers)
{
	// 定义函数指针
	typedef BOOL(WINAPI* MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);
	// 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary(TEXT("DbgHelp.dll"));
	if (NULL == hDbgHelp)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (NULL == pfnMiniDumpWriteDump)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 创建 dmp 文件件
	WCHAR szFileName[MAX_PATH] = { 0 };
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	wsprintfW(szFileName, L"%s-%04d%02d%02d-%02d%02d%02d.dmp",
		L"DumpDemo_v1.0", stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	HANDLE hDumpFile = CreateFileW(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 写入 dmp 文件
	MINIDUMP_EXCEPTION_INFORMATION expParam;
	expParam.ThreadId = GetCurrentThreadId();
	expParam.ExceptionPointers = pExceptionPointers;
	expParam.ClientPointers = FALSE;
	pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
	// 释放文件
	CloseHandle(hDumpFile);
	FreeLibrary(hDbgHelp);
	return EXCEPTION_EXECUTE_HANDLER;
}


int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers) {
	typedef BOOL(WINAPI* MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION);

	// 加载 DbgHelp.dll 动态库
	HMODULE hDbgHelp = LoadLibrary(TEXT("DbgHelp.dll"));
	if (!hDbgHelp) {
		std::cerr << "Failed to load DbgHelp.dll" << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// 获取 MiniDumpWriteDump 函数地址
	MiniDumpWriteDumpT pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (!pfnMiniDumpWriteDump) {
		std::cerr << "Failed to find MiniDumpWriteDump function" << std::endl;
		FreeLibrary(hDbgHelp);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// 生成 dump 文件名称
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	std::wstring dumpFileName = L"DumpDemo_v1.0-" +
		std::to_wstring(stLocalTime.wYear) +
		std::to_wstring(stLocalTime.wMonth) +
		std::to_wstring(stLocalTime.wDay) + L"-" +
		std::to_wstring(stLocalTime.wHour) +
		std::to_wstring(stLocalTime.wMinute) +
		std::to_wstring(stLocalTime.wSecond) + L".dmp";

	// 创建 dump 文件
	HANDLE hDumpFile = CreateFileW(dumpFileName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create dump file" << std::endl;
		FreeLibrary(hDbgHelp);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// 设置异常信息结构
	MINIDUMP_EXCEPTION_INFORMATION expParam;
	expParam.ThreadId = GetCurrentThreadId();
	expParam.ExceptionPointers = pExceptionPointers;
	expParam.ClientPointers = FALSE;

	// 写入 dump 文件
	BOOL result = pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
		MiniDumpWithDataSegs, &expParam, NULL, NULL);
	if (!result) {
		std::cerr << "MiniDumpWriteDump failed" << std::endl;
	}
	else {
		std::wcout << L"Dump file created: " << dumpFileName << std::endl;
	}

	// 清理资源
	CloseHandle(hDumpFile);
	FreeLibrary(hDbgHelp);

	return EXCEPTION_EXECUTE_HANDLER;
}


LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	// 这里做一些异常的过滤或提示
	if (IsDebuggerPresent())
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return GenerateMiniDump(lpExceptionInfo);
}

std::string GetKey(long Uin)
{
	std::string Key = "";
	std::lock_guard<std::mutex> lg(KeysMutex);
	auto iter = Keys.find(Uin);
	if (iter != Keys.end())
	{
		Key = iter->second;
	}
	return Key;
}


struct AccessInfo {
	int count; // 访问次数
	std::string first_access_time; // 首次访问时间
};

std::map<std::string, AccessInfo> access_count; // 存储IP和访问信息
std::mutex mutex_; // 互斥量
std::ofstream log_file("access_log.txt", std::ios::app); // 日志文件流

std::string getCurrentDate() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	struct tm tm_now;
	localtime_s(&tm_now, &now_time);

	std::ostringstream oss;
	oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S"); // 格式化为 YYYY-MM-DD HH:mm:ss
	return oss.str();
}

void logAccess(const std::string& ip) {
	std::lock_guard<std::mutex> guard(mutex_); // 加锁以保护共享数据
	std::string current_time = getCurrentDate();

	// 如果是第一次访问，记录时间
	if (access_count.find(ip) == access_count.end()) {
		access_count[ip] = { 1, current_time }; // 初始化访问次数和时间
	}
	else {
		access_count[ip].count++; // 更新访问次数
	}
}

void writeLogsToFile1() {
	try
	{
		std::lock_guard<std::mutex> guard(mutex_); // Lock for safe reading
		// 创建一个向量以便排序
		std::vector<std::pair<std::string, AccessInfo>> entries(access_count.begin(), access_count.end());

		// 根据访问次数降序排序
		std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
			return a.second.count > b.second.count;
		});

		log_file << "Access Log:\n"; // Log header
		for (const auto& entry : entries) {
			log_file << "IP: " << entry.first
				<< ", Access Count: " << entry.second.count
				<< ", First Access Time: " << entry.second.first_access_time << std::endl;
		}
		log_file.flush(); // Flush to file

		// Clear access_count
		access_count.clear();
	}
	catch (...)
	{
		std::cerr << "writeLogsToFile Error" << std::endl;
	}

}
// 增强的异常处理代码，包含日志记录和堆栈追踪功能


// 打印堆栈信息的函数
void printStackTrace() {
#ifdef _WIN32
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE); // 初始化符号解析

	void* stack[100];
	unsigned short frames = CaptureStackBackTrace(0, 100, stack, NULL); // 捕获堆栈信息

	for (unsigned short i = 0; i < frames; ++i) {
		DWORD64 address = (DWORD64)(stack[i]);
		DWORD64 displacementSym = 0;

		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
		SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
		symbol->MaxNameLen = MAX_SYM_NAME;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		if (SymFromAddr(process, address, &displacementSym, symbol)) {
			std::cout << symbol->Name << " 地址: 0x" << std::hex << symbol->Address << std::endl;
		}
	}

	SymCleanup(process); // 清理符号解析
#else
	std::cerr << "仅支持 Windows 打印堆栈信息" << std::endl;
#endif
}

// 将错误信息写入日志文件的函数
void logError(const std::string& message) {
	std::ofstream errorLogFile("error.log", std::ios::app);
	if (errorLogFile.is_open()) {
		errorLogFile << message << std::endl;
	}
}

// 示例函数 writeLogsToFile 增强异常处理
void writeLogsToFile() {
	try {
		std::lock_guard<std::mutex> guard(mutex_); // 锁定以保证线程安全

		// 创建一个向量以便排序
		std::vector<std::pair<std::string, AccessInfo>> entries(access_count.begin(), access_count.end());

		// 根据访问次数降序排序
		std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
			return a.second.count > b.second.count;
			});

		log_file << "Access Log:\n"; // 日志头部
		for (const auto& entry : entries) {
			log_file << "IP: " << entry.first
				<< ", Access Count: " << entry.second.count
				<< ", First Access Time: " << entry.second.first_access_time << std::endl;
		}
		log_file.flush(); // 刷新文件缓冲区

		// 清空访问计数
		access_count.clear();
	}
	catch (const std::exception& e) {
		std::string errorMessage = "捕获异常: ";
		errorMessage += e.what();
		errorMessage += " 在函数: writeLogsToFile, 文件: ";
		errorMessage += __FILE__;
		errorMessage += ", 行: ";
		errorMessage += std::to_string(__LINE__);
		std::cerr << errorMessage << std::endl;
		printStackTrace(); // 打印堆栈信息
		logError(errorMessage);
		throw; // 重新抛出异常以便上层处理
	}
	catch (...) {
		std::string errorMessage = "捕获未知异常 在函数: writeLogsToFile, 文件: ";
		errorMessage += __FILE__;
		errorMessage += ", 行: ";
		errorMessage += std::to_string(__LINE__);
		std::cerr << errorMessage << std::endl;
		printStackTrace(); // 打印堆栈信息
		logError(errorMessage);
		throw; // 重新抛出异常以便上层处理
	}
}




char* generateRandomKey() {
	// 动态分配内存
	char* key = new char[16];
	if (key == nullptr) {
		throw std::runtime_error("Memory allocation failed");
	}

	// 使用 std::random_device 作为种子，std::mt19937 作为随机数生成器
	std::random_device rd; // 获取一个随机种子
	std::mt19937 gen(rd()); // Mersenne Twister 随机数生成器
	std::uniform_int_distribution<> dis(0, 255); // 生成 0 到 255 之间的均匀分布

	// 生成随机密钥
	for (int i = 0; i < 16; ++i) {
		key[i] = static_cast<char>(dis(gen)); // 生成 0-255 之间的随机数
	}

	return key;
}

void freeKey(char* key) {
	delete[] key; // 释放动态分配的内存
}


void gen_randomcdk(char* s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	int n = strlen(alphanum);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, n - 1);

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[dis(gen)];
	}

	s[len] = '\0';  // Null-terminate the string
}

//void gen_randomcdk(char* s, const int len) {
//	static const char alphanum[] =
//		"0123456789"
//		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//		"abcdefghijklmnopqrstuvwxyz";
//
//	for (int i = 0; i < len; ++i) {
//		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
//	}
//
//	s[len] = 0;
//}

void Gen_CDK3()
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		
		srand((unsigned)time(0) + GetSysTimeMicros());  // 产生随机种子
		char cdk2[41] = { 0 };
		DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
		conn->setAutoCommit(false);
		for (int i = 0; i < 300; i++) {
			memset(cdk2, 0, 40);
			gen_randomcdk(cdk2, 40);
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO CdkInfo (KeyValue,KeyType) VALUES (?,1);"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, cdk2);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		conn->commit();
		conn->setAutoCommit(true);

	}
	catch (const std::exception& e)
	{
		printf(e.what());

	}

}
void Gen_CDK2()
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		
		srand((unsigned)time(0) + GetSysTimeMicros());  // 产生随机种子
		char cdk2[41] = { 0 };
		DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
		conn->setAutoCommit(false);
		for (int i = 0; i < 300; i++) {
			memset(cdk2, 0, 40);
			gen_randomcdk(cdk2, 40);
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO CdkInfo (KeyValue,KeyType) VALUES (?,1);"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, cdk2);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		conn->commit();
		conn->setAutoCommit(true);

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("口令CDK生成失败!\n");

	}
}


void Gen_CDK()
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		srand((unsigned)time(0) + GetSysTimeMicros());  // 产生随机种子
		char cdk[36] = { 0 };
		DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
		conn->setAutoCommit(false);
		for (int i = 0; i < 300; i++) {
			memset(cdk, 0, 35);
			gen_randomcdk(cdk, 35);
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO CdKey (KeyValue) VALUES (?);"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, cdk);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		conn->commit();
		conn->setAutoCommit(true);

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("注册CDK生成失败!\n");

	}

}

void LoadAMapConfig() {
	try
	{
		YAML::Node SpeedConfig = YAML::LoadFile(".\\Config\\AMapConfig.yml");


		//地图
		YAML::Node ForbidMapYml = SpeedConfig["ForbidMaps"];
		ForbidMaps.clear();
		for (size_t i = 0; i < ForbidMapYml.size(); i++)
		{
			YAML::Node Node = ForbidMapYml[i];
			ForbidMaps.push_back(Node["Mapid"].as<UINT>());
		}
		//NewMaps
		YAML::Node NewMapsyml = SpeedConfig["NewMaps"];
		NewMaps.clear();
		for (size_t i = 0; i < NewMapsyml.size(); i++)
		{
			YAML::Node Node = NewMapsyml[i];
			NewMapsInfo MI;
			MI.MapID = Node["MapID"].as<UINT>();
			MI.WordTxt = Node["WordTxt"].as<std::string>();
			NewMaps.push_back(MI);
			Node = NULL;
		}
		//ChallengeMaps
		if (version > 1.2) {
			YAML::Node ChallengeMapsyml = SpeedConfig["ChallengeMaps"];
			ChallengeMaps.clear();
			for (size_t i = 0; i < ChallengeMapsyml.size(); i++)
			{
				YAML::Node Node = ChallengeMapsyml[i];
				ChallengeMapsInfo CM;
				CM.MapID = Node["Mapid"].as<UINT>();
				CM.IsOpen = Node["Open"].as<UINT>();
				CM.IsNew = Node["IsNew"].as<UINT>();
				ChallengeMaps.push_back(CM);
				Node = NULL;
			}
		}
		if (version > 1.3) {
			//赛道挑战
			YAML::Node mapChallengeOpenMainYml = SpeedConfig["MapChallengeOpenMain"];
			mapChallengeOpenMain.clear();
			mapChallengeCreate.clear();
			if (mapChallengeOpenMainYml.size() > 0) {
				MapChallengeOpenMain MO;
				MO.msg = mapChallengeOpenMainYml["Msg"].as<std::string>();

				YAML::Node MapNode = mapChallengeOpenMainYml["Maps"];
				for (size_t i = 0; i < MapNode.size(); i++)
				{
					MapChallengeOpenMainMap MOP;
					MOP.Mapid = MapNode[i]["Mapid"].as<UINT>();
					MOP.Num = MapNode[i]["Num"].as<UINT>();
					MOP.firsttime = MapNode[i]["firsttime"].as<UINT>();
					MOP.secondtime = MapNode[i]["secondtime"].as<UINT>();
					MOP.thirdtime = MapNode[i]["thirdtime"].as<UINT>();
					MO.Maps.push_back(MOP);
				}
				mapChallengeOpenMain.push_back(MO);

				YAML::Node MapNodeCreate = mapChallengeOpenMainYml["MapsCreate"];
				for (size_t i = 0; i < MapNodeCreate.size(); i++)
				{
					MapChallengeOpenMainMap MOP;
					MOP.Mapid = MapNodeCreate[i]["Mapid"].as<UINT>();
					MOP.Num = MapNodeCreate[i]["Num"].as<UINT>();
					MOP.firsttime = MapNodeCreate[i]["firsttime"].as<UINT>();
					MOP.secondtime = MapNodeCreate[i]["secondtime"].as<UINT>();
					MOP.thirdtime = MapNodeCreate[i]["thirdtime"].as<UINT>();
					MOP.msg = MapNodeCreate[i]["Msg"].as<std::string>();
					mapChallengeCreate.push_back(MOP);
				}
			}

		}

		printf("Load MapConfig Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load MapConfig Error!\n");
	}

}


void parseStringToArray(const std::string& input, int array[]) {
	std::istringstream stream(input);
	std::string segment;
	int index = 0;

	// Parse the input string by splitting based on '-' and extracting numbers
	while (std::getline(stream, segment, '-') && index < 4) {
		array[index++] = std::stoi(segment);
	}
}

void LoadSpeedConfig() {
	try
	{
		YAML::Node SpeedConfig = YAML::LoadFile(".\\Config\\ASpeedConfig.yml");
		YAML::Node MagicLampCfg = SpeedConfig["MagicLampCfg"];

		MagicLamp[0] = MagicLampCfg["Seed"].as<UINT>();
		MagicLamp[1] = MagicLampCfg["LuckyValue"].as<UINT>();
		MagicLamp[2] = MagicLampCfg["ItemNum"].as<UINT>();
		MagicLamp[3] = MagicLampCfg["Itemid"].as<UINT>();

		YAML::Node MagicCupTempBox = SpeedConfig["MagicCupTempBox"];
		MagicCup[0] = MagicCupTempBox["Itemid"].as<UINT>();
		MagicCup[1] = MagicCupTempBox["ItemNum"].as<UINT>();

		YAML::Node DayRaffle = SpeedConfig["DayRaffle"];
		DayMoeny = DayRaffle["SuperMoney"].as<UINT>();

		YAML::Node MagicFormation = SpeedConfig["MagicFormation"];
		MagicShop[0] = MagicFormation["Itemid"].as<UINT>();
		MagicShop[1] = MagicFormation["ItemNum"].as<UINT>();

		YAML::Node MagicExchangeShop = SpeedConfig["MagicExchangeShop"];
		MagicExchangeShopItemId = MagicExchangeShop["Itemid"].as<UINT>();


		YAML::Node GameConfig = SpeedConfig["GameConfig"];
		AllMapLimitTime = GameConfig["allmaplimit"].as<UINT>();

		YAML::Node rankset = GameConfig["rankset"];
		Rankset[0] = rankset["on"].as<UINT>();
		Rankset[1] = rankset["man-machineon"]["on"].as<UINT>();

		RankerManTime = rankset["man-machineon"]["time"].as<UINT>();
		//Rankset[2] = rankset["start"].as<UINT>();
		//Rankset[3] = rankset["end"].as<UINT>();
		Rankset[4] = rankset["number"].as<UINT>();

		std::string interval1 = rankset["interval1"].as<std::string>();
		std::string interval2 = rankset["interval2"].as<std::string>();
		parseStringToArray(interval1, RankTimes);
		parseStringToArray(interval2, RankTimes + 2); // Start filling from the 3rd position


		GradeAddSet[0] = GameConfig["gradeset"]["1"].as<int>();
		GradeAddSet[1] = GameConfig["gradeset"]["2"].as<int>();
		GradeAddSet[2] = GameConfig["gradeset"]["3"].as<int>();
		GradeAddSet[3] = GameConfig["gradeset"]["4"].as<int>();
		GradeAddSet[4] = GameConfig["gradeset"]["5"].as<int>();
		GradeAddSet[5] = GameConfig["gradeset"]["6"].as<int>();

		rankstone = GameConfig["rankstone"].as<UINT>();

		YAML::Node borderset = GameConfig["borderset"];
		Borderset[0] = borderset["on"].as<UINT>();
		Borderset[1] = borderset["man-machineon"]["on"].as<UINT>();
		BorderManTime = borderset["man-machineon"]["time"].as<UINT>();
		Borderset[2] = borderset["honor"].as<UINT>();
		Borderset[3] = borderset["exp"].as<UINT>();
		Borderset[4] = borderset["supermoeny"].as<UINT>();
		Borderset[5] = borderset["number"].as<UINT>();

		YAML::Node bordertime = GameConfig["bordertime"];
		BorderTimes[0] = bordertime["on"].as<UINT>();
		BorderTimes[1] = bordertime["multiple"].as<UINT>();
		BorderTimes[2] = bordertime["start"].as<UINT>();
		BorderTimes[3] = bordertime["end"].as<UINT>();

		Addexp = GameConfig["addexp"].as<UINT>();
		raceAdd = GameConfig["raceAdd"].as<UINT>();
		isinvite = GameConfig["isinvite"].as<UINT>();
		mode28346 = GameConfig["mode28346"].as<UINT>();

		is60fps = GameConfig["fps60"].as<UINT>();

		YAML::Node cars = GameConfig["cars"];
		modecar.clear();
		for (size_t i = 0; i < cars.size(); i++)
		{
			YAML::Node Node = cars[i];
			modecar.push_back(Node["itemid"].as<UINT>());
		}

		BorderTeam = GameConfig["BorderTeam"].as<UINT>();

		YAML::Node rankadd = GameConfig["rankadd"];
		RankAdd[0] = rankadd["on"].as<UINT>();
		RankAdd[1] = rankadd["supermoeny"].as<UINT>();

		YAML::Node noticeyml = GameConfig["notice"];
		notice = noticeyml["on"].as<UINT>();
		bignotice = noticeyml["big"].as<std::string>();
		smallnotice = noticeyml["small"].as<std::string>();

		isMinimum = GameConfig["isMinimum"].as<UINT>();

		YAML::Node realtime = GameConfig["isRealTime"];
		isRealTime[0] = realtime["on"].as<UINT>();
		isRealTime[1] = realtime["loop"].as<UINT>();

		YAML::Node ranknotice = GameConfig["ranknotice"];
		ranknoticeon = ranknotice["on"].as<UINT>();
		ranknoticetext = ranknotice["text"].as<std::string>();

		YAML::Node Prob = GameConfig["prob"];
		ProbTime[0] = Prob["time"].as<UINT>();
		ProbTime[1] = Prob["float"].as<UINT>();

		YAML::Node gm = GameConfig["gm"];
		GMTime[0] = gm["on"].as<UINT>();
		GMTime[1] = gm["start"].as<UINT>();
		GMTime[2] = gm["end"].as<UINT>();
		GMTime[3] = gm["time"].as<UINT>();
		week = gm["week"].as<std::string>();
		GMMess = gm["Mess"].as<std::string>();

		if (GMMess == "null") {
			GMMess = "";
		}


		SkipDancetime = GameConfig["skipDanceTime"].as<UINT>();

		mapLimit = GameConfig["mapLimit"].as<UINT>();

		SwitchFlag1 = GameConfig["SwitchFlag1"].as<UINT>();
		ismd5 = GameConfig["passEncryp"].as<UINT>();
		obs = GameConfig["obs"].as<UINT>();
		isLog = GameConfig["Log"].as<UINT>();
		deleteflag = GameConfig["deleteflag"].as<UINT>();
		ipban = GameConfig["IpBan"].as<UINT>();
		raceid = GameConfig["raceid"].as<UINT>();
		GuildAdd = GameConfig["GuildAdd"].as<UINT>();
		AutoCls = GameConfig["AutoClsTCP"].as<UINT>();
		if (GetItemType(raceid) != EAIT_CAR && GetItemType(raceid) != EAIT_SKIN) {
			raceid = 10020;
		}
		tcpon[0] = GameConfig["tcpLimit"]["on"].as<UINT>();
		tcpon[1] = GameConfig["tcpLimit"]["milliseconds"].as<UINT>();
		tcpon[2] = GameConfig["tcpLimit"]["frequency"].as<UINT>();

		StoneLimit = GameConfig["StoneLimit"].as<UINT>();
		NewKart = GameConfig["NewKart"].as<UINT>();

		PreventSDP[0] = GameConfig["PreventSDP"]["on"].as<UINT>();
		PreventSDP[1] = GameConfig["PreventSDP"]["keyType"].as<UINT>();

		beiginmode = GameConfig["BeiginMode"].as<UINT>();
		loginMode = GameConfig["LoginMode"].as<UINT>();
		LimitTalk = GameConfig["LimitTalk"].as<UINT>();

		if (version > 1) {
			//下注
			Stake[0] = GameConfig["Stake"]["on"].as<UINT>();
			Stake[1] = GameConfig["Stake"]["Itemid"].as<UINT>();
			if (GetItemType(Stake[1]) != EAIT_PROP) {
				Stake[1] = 127748;
			}
		}
		if (version > 1.1) {
			//广播
			BroadCaston = GameConfig["broadcast"]["on"].as<UINT>();
			BroadCast.clear();
			YAML::Node BroadCastList = GameConfig["broadcast"]["news"];
			for (size_t i = 0; i < BroadCastList.size(); i++)
			{
				BroadCast.push_back(BroadCastList[i]["new"].as<std::string>());
			}

			topnew[0] = GameConfig["topnews"]["on"].as<UINT>();
			topnew[1] = GameConfig["topnews"]["time"].as<UINT>();
			topnews = GameConfig["topnews"]["new"].as<std::string>();
			//垫底赛
			IsBottomGame[0] = GameConfig["BottomGame"]["on"].as<UINT>();
			IsBottomGame[1] = GameConfig["BottomGame"]["itemid"].as<UINT>();
			IsBottomGame[2] = GameConfig["BottomGame"]["itemnum"].as<UINT>();
			IsBottomGame[3] = GameConfig["BottomGame"]["number"].as<UINT>();
			if (GetItemType(IsBottomGame[1]) != EAIT_PROP) {
				Stake[1] = 127748;
			}
			IsBottomGame4or6[0] = GameConfig["BottomGame"]["fourNumberNum"].as<UINT>();
			IsBottomGame4or6[1] = GameConfig["BottomGame"]["sixNumberNum"].as<UINT>();
		}

		if (version > 1.2) {
			//打榜
			std::string Challengval1 = GameConfig["ChallengeCfg"]["interval1"].as<std::string>();
			std::string Challengval2 = GameConfig["ChallengeCfg"]["interval2"].as<std::string>();
			parseStringToArray(Challengval1, ChangeTimes);
			parseStringToArray(Challengval2, ChangeTimes + 2); // Start filling from the 3rd position
			ChallengeUrl = GameConfig["ChallengeCfg"]["Url"].as<std::string>();
			ChallengeFileOn = GameConfig["ChallengeCfg"]["File"].as<UINT>();
			ChallengeFilePath = GameConfig["ChallengeCfg"]["FilePath"].as<std::string>();
			YAML::Node ChallengeMsgList = GameConfig["ChallengeCfg"]["News"];
			ChallengeCfgMsgs.clear();
			for (size_t i = 0; i < ChallengeMsgList.size(); i++)
			{
				ChallengeCfgMsgs.push_back(ChallengeMsgList[i]["new"].as<std::string>());
			}

			ChallengeForbidKart.clear();
			YAML::Node ChallengeForbidList = GameConfig["ChallengeCfg"]["ForbidKart"];
			for (size_t i = 0; i < ChallengeForbidList.size(); i++)
			{
				ChallengeForbidKart.push_back(ChallengeForbidList[i]["kartid"].as<UINT>());
			}
			//广告
			AdvertisingUrl = GameConfig["AdvertisingUrl"].as<std::string>();
			if (AdvertisingUrl == "null") {
				AdvertisingUrl = "";
			}

			//尾数
			Mantissa[0] = GameConfig["Mantissa"]["on"].as<UINT>();
			Mantissa[1] = GameConfig["Mantissa"]["minimum"].as<UINT>();
			Mantissa[2] = GameConfig["Mantissa"]["maximum"].as<UINT>();
			Mantissa[3] = GameConfig["Mantissa"]["itemid"].as<UINT>();

			AutoReplyGuild = GameConfig["AutoReplyGuild"].as<UINT>();
			qqurl = GameConfig["HeadUrl"].as<std::string>();
		}



		//事件
		YAML::Node LimitList = SpeedConfig["LimitMsgID"];
		LimitMsgs.clear();
		for (size_t i = 0; i < LimitList.size(); i++)
		{
			YAML::Node Node = LimitList[i];
			std::shared_ptr<LimitMsgInfo> MsgInfo = std::make_shared<LimitMsgInfo>();
			MsgInfo->MSGID = Node["MsgID"].as<UINT>();
			MsgInfo->isInfo = Node["INFO"].as<UINT>();
			LimitMsgs[Node["MsgID"].as<UINT>()] = MsgInfo;
			Node = NULL;
		}
		LimitList = NULL;

		YAML::Node MsgIDList = SpeedConfig["MsgID"];
		Msgs.clear();
		for (size_t i = 0; i < MsgIDList.size(); i++)
		{
			YAML::Node Node = MsgIDList[i];
			std::shared_ptr<LimitMsgInfo> MsgInfo = std::make_shared<LimitMsgInfo>();
			MsgInfo->MSGID = Node["MsgID"].as<UINT>();
			//MsgInfo->isInfo = Node["INFO"].as<UINT>();
			Msgs[Node["MsgID"].as<UINT>()] = MsgInfo;
			Node = NULL;
		}
		MsgIDList = NULL;

		//开始游戏
		GameBeginSwitchFlag = SpeedConfig["GameBegin"]["GameBeginSwitchFlag"].as<UINT>();
		FeedBackEnabled = SpeedConfig["GameBegin"]["FeedBackEnabled"].as<UINT>();
		AnonymousMode = SpeedConfig["GameBegin"]["AnonymousMode"].as<UINT>();


		//config
		ServerConfig = YAML::LoadFile(".\\Config\\config.yml");
		UITitleDispInfo_ADText = ServerConfig["Server"]["Title"].as<std::string>();
		if (version > 1.3) {
			YAML::Node Server = ServerConfig["Server"];
			cdn[0] = Server["CDN"]["on"].as<UINT>();
			cdn[1] = Server["CDN"]["mode"].as<UINT>();
			cdn[2] = Server["CDN"]["time"].as<UINT>();
			cdn[3] = Server["CDN"]["port"].as<UINT>();
			cdndomain = Server["CDN"]["domain"].as<std::string>();
		}

		DisplayBitMapConfig[0] = ServerConfig["GameName"].as<UINT>();
		DisplayBitMapConfig[1] = ServerConfig["Version"].as<UINT>();
		DisplayBitMapConfig[2] = ServerConfig["WorldName"].as<UINT>();
		DisplayBitMapConfig[3] = ServerConfig["Ad"].as<UINT>();
		DisplayBitMapConfig[4] = ServerConfig["NickName"].as<UINT>();
		if (_stricmp(ServerConfig["Server"]["Model"].as<std::string>().c_str(), "tcp") == 0)
		{
			TransferModel = 1;
		}
		else {
			TransferModel = 0;
		}
		printf("Load SpeedConfig Success!\n");

		LoadAMapConfig();
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load SpeedConfig Error!\n");
	}

}




std::shared_ptr<ClientNode> GetRelaxClient(long Uin)
{
	std::shared_ptr<ClientNode> Client = NULL;
	std::lock_guard<std::recursive_mutex> lg(RelaxClientMutex);
	auto iter = RelaxClients.find(Uin);
	if (iter != RelaxClients.end())
	{
		Client = iter->second;
	}
	else
	{
	}
	return Client;
}

void OutdoorUpdateExperience()
{
	std::lock_guard<std::recursive_mutex> lg(RelaxClientMutex);
	std::map<long, std::shared_ptr<ClientNode>>::iterator iter = RelaxClients.begin();
	while (iter != RelaxClients.end())
	{
		std::shared_ptr<ClientNode> DstClient = iter->second;
		if (DstClient)
		{
			//printf("NotifyClientUpdateExp\n");
			NotifyClientUpdateExp(DstClient);
		}
		iter++;
	}
}

std::shared_ptr<ClientNode> GetClient(long Uin)
{
	std::shared_ptr<ClientNode> Client = NULL;
	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	auto iter = Clients.find(Uin);
	if (iter != Clients.end())
	{
		Client = iter->second;
	}
	return Client;


}


void LoadAdvcancedProp()
{
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Advanced\\BorderAdvanced.yml");
		YAML::Node AdvcancedCfg;
		size_t Num = Config.size();
		BorderAdvancedCfg = std::make_shared<ADVANCEDCFG>();
		BorderAdvancedCfg->AdvancedProp.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			AdvcancedCfg = Config[i];
			BorderAdvancedCfg->AdvancedProp[i].PropID = AdvcancedCfg["ItemId"].as<int>();
			BorderAdvancedCfg->AdvancedProp[i].Add = AdvcancedCfg["Add"].as<int>();
			AdvcancedCfg.reset();
		}
	}
	catch (...)
	{
		printf("Load BorderAdvancedCfg Error!\n");
		return;
	}
	printf("Load BorderAdvancedCfg Success!\n");

	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Advanced\\RankedAdvanced.yml");
		YAML::Node AdvcancedCfg;
		size_t Num = Config.size();
		RankAdvancedCfg = std::make_shared<ADVANCEDCFG>();
		RankAdvancedCfg->AdvancedProp.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			AdvcancedCfg = Config[i];
			RankAdvancedCfg->AdvancedProp[i].PropID = AdvcancedCfg["ItemId"].as<int>();
			RankAdvancedCfg->AdvancedProp[i].Add = AdvcancedCfg["Add"].as<int>();
			AdvcancedCfg.reset();
		}
	}
	catch (...)
	{
		printf("Load RankAdvancedCfg Error!\n");
		return;
	}
	printf("Load RankAdvancedCfg Success!\n");
}

void findNotConnidUin() {
	std::vector<UINT> connids;
	UserDB_SelectConnID(connids);
	// 确保 connids 已排序
	std::sort(connids.begin(), connids.end());

	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	for (const auto& pair : Clients) {
		std::shared_ptr<ClientNode> value = pair.second;
		if (value) {
			UINT Uin = value->Uin;
			auto it = std::lower_bound(connids.begin(), connids.end(), Uin);

			if (it != connids.end() && *it == value->Uin) {

			}
			else {
				std::cout << "玩家缺失ConnID，账号：" << Uin << std::endl;
			}
		}
	}
}

void reload() {
	std::lock_guard<std::recursive_mutex> lg(ConfigMutex);
	try
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

		//加载配置到内存缓冲区 加速读写
		LoadMapConfig();
		LoadMapPool();
		//LoadItemConfig();
		LoadPetItem();//加载宠物到内存
		LoadTreasureBox();//加载宝箱
		LoadSkillStoneCfg();
		LoadKartSkill();
		LoadCommodityInfo();
		LoadMagicLampCfg();
		LoadSign3Cfg();
		LoadAdvcancedProp();
		LoadMiracleCfg();
		LoadRegisterBox();
		LoadNPCListInfo();
		LoadRandomMapPool();
		LoadDayRaffleBox();
		LoadOutDoorEggs();
		LoadMagicFormationBoxs();
		LoadMagicExchangeShopBoxs();
		LoadPetLevelConfig();//宠物升级配置
		LoadAuctionCfg();
		LoadCommonBoxAwards();
		LoadCarTreasure();
		LoadSpeedConfig();
		UserDB_GetAllUin(Uins);
		LoadAppellationConfig();
		LoadUserPermissionsConfig();
		if (version > 1.2) {
			SelectChallengeAllMap(ChallengeAllMaps, ChallengeMaps);
		}
		if (version > 1.3) {
			LoadMechTreasureInfoConfig();
		}

	}
	catch (...)
	{
		printf("重新加载异常!\n");
	}
}

void prints19() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	if (TransferModel == 0)
	{
		printf("===========================================================================\n");

		std::cout << "==                          当前版本号：" << versionStr << "                          ==\n";
		printf("===========================================================================\n");
		printf("==                     禁止倒卖，如遇法律责任，后果自负                  ==\n");
		//printf("===========================================================================\n");
		//printf("==     同步超时时间:%d 跳舞时间:%d 结算时间:%d 当前模式: udp     ==\n", SyncTime, DanceTime, GameOverTime);
		printf("===========================================================================\n");
		printf("==                          输入help获得帮助命令                         ==\n");
		printf("===========================================================================\n");
		printf("==                          输入gameplay获得游戏玩法                     ==\n");
		printf("===========================================================================\n");
		printf("==                          输入log打印版本更新日志                      ==\n");
		printf("===========================================================================\n");
	}
	else
	{
		printf("===========================================================================\n");
		std::cout << "==                              当前版本号：" << versionStr << "                          ==\n";
		printf("===========================================================================\n");
		printf("==                     禁止倒卖，如遇法律责任，后果自负                  ==\n");
		//printf("===========================================================================\n");
		//printf("==     同步超时时间:%d 跳舞时间:%d 结算时间:%d 当前模式: tcp      ==\n", SyncTime, DanceTime, GameOverTime);
		printf("===========================================================================\n");
		printf("==                          输入help获得帮助命令                         ==\n");
		printf("===========================================================================\n");
		printf("==                          输入gameplay获得游戏玩法                     ==\n");
		printf("===========================================================================\n");
		printf("==                          输入log打印版本更新日志                      ==\n");
		printf("===========================================================================\n");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}


void Run2()
{

	const char* str1;
	char ch[4096];
	char* power = NULL;
	do
	{
		memset(ch, 0, 4096);
		fputs(">>>", stdout);
		fflush(stdout);
		fgets(ch, 4096, stdin);
		fflush(stdin);
		try
		{	
			if (_stricmp(ch, "exit\n") == 0)
			{
				stopFlag = true;
				cv.notify_all();
				std::cout << "Console window is closing, perform cleanup here." << std::endl;
				const auto& myLogger = spdlog::get("SpeedServer_logger");
				myLogger->flush();// 刷新，否则只有程序结束时才会把日志写入文件
				spdlog::drop_all();//销毁日志系统
				//CloseDB();
				return;
			}
			else if (_stricmp(ch, "Room\n") == 0)
			{
				PrintRoom();
			}
			else if (_stricmp(ch, "log\n") == 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
				printf("1.0版本包含基础，增加大闯关，增加防止IP压测，防止恶意发包\n");
				printf("1.1版本更新对战记录，房间单挑下注\n");
				printf("1.2版本更新自定义广播，自定义置顶广播，垫底赛配置\n");
				printf("1.3版本更新车神打榜，房间猜尾数,自动审批车队,大厅广告\n");
				printf("1.4版本更新红人馆头像，拉取qq头像，敖丙抽奖，赛道挑战自定义地图显示，车库显示和摆放,房间内语音\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else if (_stricmp(ch, "help\n") == 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
				printf("=======================================S19版本帮助========================================\n");
				printf("=                     输入 usr 可查看在线人数                                            =\n");
				printf("=                     sign on/off开启或关闭注册                                          =\n");
				printf("=                     prop on/off开启或关闭抽奖                                          =\n");
				printf("=                     stoneset:10,1 设置重生宝石发动间隔秒数，上下浮动秒数               =\n");
				printf("=                     gencdk  生成注册专用CDK                                            =\n");
				printf("=                     gencdk2 生成可重复抽奖CDK（每账号只能使用一次）                    =\n");
				printf("=                     allmaplimit:6000 设置全图限制最短过线时间s/秒                      =\n");
				printf("=                     rankset:1,0 设置排位开启-排位是否人机                              =\n");
				printf("=                     rankstone:1 开启关闭排位宝石                                       =\n");
				printf("=                     borderset:1,1,1000,1000,1000 设置边境开启-边境人机-荣誉-经验-点券  =\n");
				printf("=                     bordertime:1,4,20,23 设置边境倍数开启-倍数-倍数开始时间-结束时间   =\n");
				printf("=                     stonelv:60,40,30 设置合成宝石概率                                  =\n");
				printf("=                     magiclamp:1,30000,100 设置点灯概率,1个灯消耗盲盒数量,点一次消耗数量=\n");
				printf("=                     magicshop:1000 终极魔法阵消耗数量                                  =\n");
				printf("=                     addexp:25000 对局增加经验                                          =\n");
				printf("=                     reload命令重新加载配置文件                                         =\n");
				printf("=                     cls命令清除命令行                                                  =\n");
				printf("=                     conn查看数据库链接数量                                             =\n");
				printf("=                     reloadcar重新加载赛车数据                                          =\n");
				printf("=                     kick:10001踢人下线                                                 =\n");
				printf("=                     alluin命令查看所有在线UIN                                          =\n");
				printf("=                     banuin:10001，将10001封号，并且拉黑其IP                            =\n");
				printf("=                     addip:127.0.0.1, 拉黑IP                                            =\n");
				printf("=                     reip:127.0.0.1, 解除IP                                             =\n");
				printf("=                     new:我是顶部提示消息, 这是发送顶部消息                             =\n");
				printf("=======================================S19版本帮助========================================\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else if (_stricmp(ch, "gameplay\n") == 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
				printf("=================================S19玩法帮助=================================\n");
				printf("=     游戏内卡图可以按 G+回车 可以强制退出房间                              =\n");
				printf("=     房间内聊天窗口输入 map 地图ID 可以切换指定地图                        =\n");
				printf("=     房间内聊天窗口房主输入:开启比赛模式,可以游戏内观察对方是剃刀          =\n");
				printf("=     房间内聊天窗口房主输入:关闭比赛模式,关闭功能                          =\n");
				printf("=     房间内聊天窗口输入:关闭跳舞结算,可跳过跳舞结算环节(skipflag=1)        =\n");
				printf("=     房间内聊天窗口输入:开启跳舞结算,关闭功能                              =\n");
				printf("=     佩戴奶糖自动四级宝石(刷新频道)                                        =\n");
				printf("=     佩戴菠萝糖边境排位匹配人机(刷新频道)                                  =\n");
				printf("=     佩戴太妃糖自动五颗六级宝石(刷新频道)                                  =\n");
				printf("=================================S19玩法帮助=================================\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else if (_stricmp(ch, "cls\n") == 0)
			{
				system("cls");  // 清除命令行
				prints19();
			}
			else if (_stricmp(ch, "sign on\n") == 0)
			{
				registerflag = 1;
				printf("已开启注册\n");
			}
			else if (_stricmp(ch, "sign off\n") == 0)
			{
				registerflag = 0;
				printf("已关闭注册\n");
			}
			else if (_stricmp(ch, "prop on\n") == 0)
			{
				propflag = 1;
				printf("已开启抽奖\n");
			}
			else if (_stricmp(ch, "prop off\n") == 0)
			{
				propflag = 0;
				printf("已关闭抽奖\n");
			}
			else if (_stricmp(ch, "reload\n") == 0)
			{
				reload();
			}
			else if ((power = strstr(ch, "allmaplimit:")) != NULL)
			{
				power += 12;
				AllMapLimitTime = atoi(power);
				printf("限制最短过线时间:%d秒\n", AllMapLimitTime / 1000);
			}
			else if ((power = strstr(ch, "rankstone:")) != NULL)
			{
				power += 10;
				rankstone = atoi(power);
				if (rankstone == 1)
				{
					str1 = "排位宝石开启";
				}
				else
				{
					str1 = "排位宝石关闭";
				}
				printf("%s\n", str1);
			}
			else if ((power = strstr(ch, "stoneset:")) != NULL)
			{
				power += 9;
				ProbTime[0] = atoi(power);

				power = strstr(power, ",");

				power += 1;

				ProbTime[1] = atoi(power);

				printf("重生宝石设置秒数:%d 上下浮动范围:%d\n", ProbTime[0], ProbTime[1]);
				/*power += 6;
				Prob = atoi(power);
				printf("重生宝石概率基数:%d\n", Prob);*/
			}
			else if ((power = strstr(ch, "stonelv:")) != NULL)
			{

				power += 8;
				StoneLvSeed[0] = atoi(power);
				power += 3;
				StoneLvSeed[1] = atoi(power);
				power += 3;
				StoneLvSeed[2] = atoi(power);
				printf("合成宝石概率:2级%d%% 3级:%d%% 4级:%d%%\n", StoneLvSeed[0], StoneLvSeed[1], StoneLvSeed[2]);
			}
			else if ((power = strstr(ch, "luckylimit:")) != NULL)
			{
				power += 11;
				LuckyLimit = atoi(power);
				printf("幸运抽奖保底值:%d\n", LuckyLimit);
			}
			else if ((power = strstr(ch, "loserate:")) != NULL)
			{
				power += 9;
				LoseRate = atoi(power);
				printf("碰撞损失速率:%d\n", LoseRate);
			}
			else if ((power = strstr(ch, "rankset:")) != NULL)
			{

				power += 8;
				Rankset[0] = atoi(power);

				power = strstr(power, ",");

				power += 1;

				Rankset[1] = atoi(power);

				printf("排位设置成功: 开关:%d 是否人机:%d\n", Rankset[0], Rankset[1]);

			}
			else if ((power = strstr(ch, "bordertime:")) != NULL)
			{

				power += 11;
				BorderTimes[0] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				BorderTimes[1] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				BorderTimes[2] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				BorderTimes[3] = atoi(power);

				printf("边境倍数设置成功: 开关:%d 倍数:%d，时间区间:%d点-%d点\n", BorderTimes[0], BorderTimes[1], BorderTimes[2], BorderTimes[3]);

			}
			else if ((power = strstr(ch, "borderset:")) != NULL)
			{
				power += 10;
				Borderset[0] = atoi(power);

				power = strstr(power, ",");

				power += 1;
				Borderset[1] = atoi(power);

				power = strstr(power, ",");

				power += 1;
				Borderset[2] = atoi(power);

				power = strstr(power, ",");

				power += 1;
				Borderset[3] = atoi(power);

				power = strstr(power, ",");

				power += 1;
				Borderset[4] = atoi(power);

				printf("边境设置成功: 开关:%d 是否人机:%d 荣誉:%d 经验:%d 点券:%d\n", Borderset[0], Borderset[1], Borderset[2], Borderset[3], Borderset[4]);
			}
			else if ((power = strstr(ch, "gradeset:")) != NULL)
			{
				power += 9;
				GradeAddSet[0] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				GradeAddSet[1] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				GradeAddSet[2] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				GradeAddSet[3] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				GradeAddSet[4] = atoi(power);

				power = strstr(power, ",");
				power += 1;
				GradeAddSet[5] = atoi(power);

				printf("排位分数设置: 第一名:%d 第二名:%d 第三名:%d 第四名:%d 第五名:%d 第六名:%d\n", GradeAddSet[0], GradeAddSet[1], GradeAddSet[2], GradeAddSet[3], GradeAddSet[4], GradeAddSet[5]);
			}
			else if (_stricmp(ch, "gencdk2\n") == 0)
			{
				Gen_CDK2();
				printf("CDK口令红包生成成功!\n");
			}
			else if (_stricmp(ch, "gencdk\n") == 0)
			{
				Gen_CDK();
				printf("CDK口令注册生成成功!\n");
			}
			else if ((power = strstr(ch, "superprop:")) != NULL)
			{
				power += 10;
				SuperProp[0].ItemID = atoi(power);
				power = strstr(power, ",");
				power += 1;
				SuperProp[0].GetNum = atoi(power);
				power = strstr(power, ",");
				power += 1;
				SuperProp[1].ItemID = atoi(power);
				power = strstr(power, ",");
				power += 1;
				SuperProp[1].GetNum = atoi(power);
				power = strstr(power, ",");
				power += 1;
				SuperProp[2].ItemID = atoi(power);
				power = strstr(power, ",");
				power += 1;
				SuperProp[2].GetNum = atoi(power);
				printf("超级道具设置成功: Item:%d GetNum:%d Item:%d GetNum:%d Item:%d GetNum:%d\n", SuperProp[0].ItemID, SuperProp[0].GetNum, SuperProp[1].ItemID, SuperProp[1].GetNum, SuperProp[2].ItemID, SuperProp[2].GetNum);
			}
			else if ((power = strstr(ch, "magiclamp:")) != NULL)
			{
				power += 10;
				MagicLamp[0] = atoi(power);
				power = strstr(power, ",");
				power += 1;
				MagicLamp[1] = atoi(power);
				power = strstr(power, ",");
				power += 1;
				MagicLamp[2] = atoi(power);
				printf("小橘子点灯概率:%d 最大幸运值:%d 消耗盲盒:%d\n", MagicLamp[0], MagicLamp[1], MagicLamp[2]);
			}
			else if ((power = strstr(ch, "magicshop:")) != NULL)
			{
				power += 10;
				MagicShop[1] = atoi(power);

				printf("终极魔法阵消耗小橘子数量:%d\n", MagicShop[1]);
			}
			else if ((power = strstr(ch, "addexp:")) != NULL)
			{
				power += 7;
				Addexp = atoi(power);

				printf("对局增加经验:%d\n", Addexp);
			}
			else if (_stricmp(ch, "usr\n") == 0)
			{

				int num = Clients.size();

				printf("当前在线人数:%d\n", num);
			}
			else if (_stricmp(ch, "conn\n") == 0)
			{
				int playernum = player.GetAvailableConnections();
				int usernum = usr.GetAvailableConnections();
				printf("当前可用数据库Player连接数量:%d，usr连接数量:%d\n", playernum, usernum);
			}
			else if (_stricmp(ch, "connid\n") == 0)
			{
				findNotConnidUin();
			}
			else if (_stricmp(ch, "contime\n") == 0)
			{
				int playernum = player.GetConnectionsTimes();
				int usernum = usr.GetConnectionsTimes();
				printf("当前Player可用数据库正在使用量:%d,usr可用数据库正在使用量:%d\n", playernum, usernum);
			}
			else if (_stricmp(ch, "back\n") == 0)
			{
				//player.ReleaseConnections();
				//usr.ReleaseConnections();
			}
			else if (_stricmp(ch, "reloadcar\n") == 0)
			{
				Reloadracingdata();
			}
			else if (_stricmp(ch, "alluin\n") == 0)
			{
				OnlineUin();
			}
			else if (_stricmp(ch, "cltcp\n") == 0)
			{
				ClearTcp();
			}
			else if (_stricmp(ch, "addfire\n") == 0)
			{
				addip2FireWall1();
			}
			else if (_stricmp(ch, "printip\n") == 0)
			{
				SelectTcptoUIN();
			}
			else if (_stricmp(ch, "noip\n") == 0)
			{
				TcpNoIPFireWall();
			}
			else if (_stricmp(ch, "noconn\n") == 0)
			{
				TcpNoConnidFireWall();
			}
			else if (_stricmp(ch, "stonenew\n") == 0)
			{
				std::cout << "正在插入..." << std::endl;
				PlayerDB_InsertStoneNew();
				std::cout << "插入成功..." << std::endl;
			}
			else if (_stricmp(ch, "logip\n") == 0)
			{
				writeLogsToFile();
				std::cout << "写出日志成功" << std::endl;
			}
			else if ((power = strstr(ch, "banuin:")) != NULL)
			{
				power += 7;
				UINT Uin = atoi(power);
				addip2FireWall(Uin);
			}
			else if ((power = strstr(ch, "addip:")) != NULL)
			{
				power += 6;
				std::string ip(power);  // 直接使用 power 指针构造 std::string

				// 去除尾部的空白字符
				ip.erase(ip.find_last_not_of(" \t\n\r\f\v") + 1);

				addip2FireWall(ip);
			}
			else if ((power = strstr(ch, "reip:")) != NULL)
			{
				power += 5;
				std::string ip(power);  // 直接使用 power 指针构造 std::string

				// 去除尾部的空白字符
				ip.erase(ip.find_last_not_of(" \t\n\r\f\v") + 1);

				RemoveIp(ip);
			}
			else if ((power = strstr(ch, "kick:")) != NULL)
			{
				power += 5;
				UINT Uin = atoi(power);
				std::shared_ptr<ClientNode> Temp = GetClient(Uin);
				if (Temp)
				{
					NotifyKickPlayer(Temp, "你已经被管理员踢下线", 3);
					//GetClient(DestUin)->deleteflag = 1;
					Temp->IsLogin = FALSE;
					Temp->Server->Disconnect(Temp->ConnID);
					Temp->deleteflag = 1;
					printf("玩家已被踢下线！\n");
					Temp = NULL;
				}
				else {
					printf("该玩家不在线！\n");
				}
			}
			else if ((power = strstr(ch, "new:")) != NULL)
			{
				power += 4;
				std::string news(power);  // 直接使用 power 指针构造 std::string

				// 去除尾部的空白字符
				news.erase(news.find_last_not_of(" \t\n\r\f\v") + 1);
				std::cout << "发送的消息是【如果包含中文显示不全每个字空一格】：" << news << std::endl;
				NotifyBroadcast17(news);
			}

			else if (_stricmp(ch, "test\n") == 0)
			{
				//RequestC2GGetMapChallengeRank();
				std::shared_ptr<ClientNode> Cl = GetClient(10001);
				ResponseLmtEnterChatRoom(Cl);
			}
		}
		catch (...)
		{
			printf("请重新输入命令!\n");
		}
	} while (!stopFlag && _stricmp(ch, "Exit\n") != 0);
	//fputs("Bye\n", stdout);
	return;
}


void Run3()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(5));
		try {
			//判断是否已经过去了10秒
		   /*auto currentTime = std::chrono::steady_clock::now();
		   auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - UpdateTime).count();
		   if (elapsed > 1800) {
			   exit(0);
		   }*/

			RankedMatchBeginTest();//每隔秒检查排位队列
		}
		catch (...)
		{
			printf("RankedMatchBeginTest Exception!\n");
		}
	}
	return;

}

void Run4()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(5));
		try
		{
			BorderMatchBeginTest();//每隔秒检查边境队列
		}
		catch (...)
		{
			printf("BorderMatchBeginTest Exception!\n");
		}
	}
	return;
}

void Run5()
{
	char checktime = 0;

	while (!stopFlag)
	{
		//Sleep(60000);
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(60));

		checktime += 1;
		try {
			if (checktime >= 6)
			{
				checktime = 0;
				player.ReleaseConnections();
				usr.ReleaseConnections();
			}
		}
		catch (...)
		{
			printf("player ReleaseConnections Exception!\n");
		}
	}
	return;

}

void Run6()
{
	while (!stopFlag)
	{
		//线程计算所有客户端在线时常 60秒检测一次
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(60));
		try
		{
			OnlineTest();
		}
		catch (...)
		{
			printf("OnlineTest Exception!\n");
		}

	}
	return;
}

void Run7()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(60));
		//Sleep(1000);
		try
		{
			UpdateTopList();
		}
		catch (...)
		{
			printf("UpdateTopList Exception!\n");
		}
	}
	return;
}

void Run8()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(6));
		try
		{
			RunningLog();
		}
		catch (...)
		{
			printf("RunningLog Exception!\n");
		}
	}
	return;
}

void Run9()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(6));
		try
		{
			OutdoorUpdateExperience();
		}
		catch (...)
		{
			printf("OutdoorUpdateExperience Exception!\n");
		}
	}
	return;
}

void Run10()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(10));
		try
		{
			CheckAuctionTime();
		}
		catch (...)
		{
			printf("CheckAuctionTime Exception!\n");
		}
	}
	return;
}

void Run11()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::seconds(10));
		try
		{
			addip2FireWall();
		}
		catch (...)
		{
			std::cerr << "addip2FireWall Exception!\n" << std::endl;
		}
	}
}

void Run12()
{
	std::unique_lock<std::mutex> lock(cvMutex);
	cv.wait_for(lock, std::chrono::seconds(cdn[2]));
	if (cdn[0] > 0 && cdn[1] == 1) {
		while (!stopFlag) {
			try {
				lookdns();
			}
			catch (...) {
				std::cerr << "lookdns Exception!\n";
			}
			cv.wait_for(lock, std::chrono::seconds(cdn[2]));
		}
	}

}

void Run14()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::hours(1)); // 每隔1小时
		try
		{
			SelectGmTime(GMTime[3]);
		}
		catch (...)
		{
			std::cerr << "SelectGmTime Exception!\n" << std::endl;
		}
	}
}

void Run15()
{
	while (!stopFlag)
	{
		std::unique_lock<std::mutex> lock(cvMutex);
		cv.wait_for(lock, std::chrono::minutes(topnew[1]));
		try
		{
			NotifyBroadcast17();
		}
		catch (...)
		{
			std::cerr << "NotifyBroadcast Exception!\n" << std::endl;
		}
	}
}



void DeleteClientBySystem(std::shared_ptr<ClientNode> Client)
{
	RequestCancelMatch(Client);
	LeaveOutdoor(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	UserDB_UpdateConnID(Client->Uin);
	if (Client->BufferPtr)
	{
		safe_free((PVOID)Client->BufferPtr);
	}
	if (Client->BufferChallengePtr)
	{
		//safe_free((PVOID)Client->BufferChallengePtr);
		//Client->BufferChallengePtr = 0;
	}
	/*if (Client->ChallengeMaps.size() == 1) {
		UpdateChallengeMapStatus(Client->ChallengeMaps[0].MapID, Client->ChallengeMaps[0].RankID, 0);
	}*/

	Client->IsLogin = FALSE;
	Client->BufferPtr = NULL;
	Client->deleteflag = 1;
}

std::vector<std::string> ResolveDomain() {
	std::vector<std::string> ipAddresses;
	struct addrinfo hints = {}, * res;

	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(cdndomain.c_str(), nullptr, &hints, &res) == 0) {
		for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
			char ipStr[INET6_ADDRSTRLEN];
			if (p->ai_family == AF_INET) {
				sockaddr_in* ipv4 = (sockaddr_in*)p->ai_addr;
				inet_ntop(AF_INET, &ipv4->sin_addr, ipStr, sizeof(ipStr));
				ipAddresses.push_back(ipStr);
				std::cout << "初始化地址：" << ipStr << "端口：" << cdn[3] << std::endl;
			}
			// 如果需要 IPv6，添加类似处理
		}
		freeaddrinfo(res);
	}
	return ipAddresses;
}


void lookdns()
{

	char** pptr = NULL;
	HOSTENT* pHostEntry = gethostbyname(cdndomain.c_str());
	if (pHostEntry != NULL)
	{
		int i = 0;
		//将主机地址列表输出，可含多个 
		char* szIpBuff = NULL;
		for (i = 0, pptr = pHostEntry->h_addr_list; *pptr != NULL; ++pptr, i++)
		{
			szIpBuff = new char[32];
			memset(szIpBuff, 0, 32);
			//inet_ntop的返回值为NULL，则表示失败，否则返回相应的IP地址（此时szIpRet指向的是szIpBuff） 
			const char* szIpRet = inet_ntop(pHostEntry->h_addrtype, *pptr, szIpBuff, 32);
			if (szIpRet) {
				//std::cout << "初始化地址：" << szIpRet << "端口：" << cdn[3] << std::endl;
				InitDirServer(Name.c_str(), inet_addr(szIpRet), cdn[3], cdn[3]);
				//break;
			}
			//domainip = szIpBuff;
			//MessageBoxA(NULL, domainip, NULL, NULL);
		}
	}
	return;
}


class GameServer : public CTcpServerListener, public CUdpNodeListener
{
public:
	CTcpServerPtr TCP;
	CUdpNodePtr UDP;
	unsigned short CheckSum(const BYTE* pBuffer, int nBufLen)
	{
		if (NULL == pBuffer || nBufLen < 1)
		{
			return 0;
		}

		unsigned long nSum = 0;
		unsigned short* pData = (unsigned short*)pBuffer;

		int nLen = nBufLen / 2;
		int nMod = nBufLen % 2;

		for (int i = 0; i < nLen; i++)
		{
			nSum += pData[i];
		}

		unsigned short nShort = 0;

		if (1 == nMod)
		{
			nShort = (unsigned short)pBuffer[nBufLen - 1];
			nSum += nShort;
		}
		nSum = (nSum >> 16) + (nSum & 0xffff);
		nSum += (nSum >> 16);
		nShort = ~((unsigned short)nSum);
		return nShort;
	}

	void SendUdpData(LPCTSTR lpszRemoteAddress, USHORT usRemotePort, short Cmd, const BYTE* Data, int Len, ULONG Seq, SHORT DialogID, ULONG Uin)
	{
		int BufferLength = sizeof(TransferHead) + Len;
		BYTE* Buffer = (BYTE*)safe_malloc(BufferLength);
		if (!Buffer)
		{
			return;
		}
		TransferHead* ProtocolHead = (TransferHead*)Buffer;
		ProtocolHead->TotalLength = htons(BufferLength);
		ProtocolHead->Ver = 0;
		ProtocolHead->Seq = htonl(Seq);
		ProtocolHead->DialogID = htons(DialogID);
		ProtocolHead->Uin = htonl(Uin);
		ProtocolHead->TransferCmd = htons(Cmd);
		ProtocolHead->CheckSum = 0;
		ProtocolHead->OptLength = (UCHAR)Len;
		memcpy(ProtocolHead + 1, Data, Len);
		ProtocolHead->CheckSum = htons(CheckSum(Buffer, BufferLength));

		UDP->Send(lpszRemoteAddress, usRemotePort, Buffer, BufferLength);
		safe_free(Buffer);
		//printf("[UDP:Send]Cmd:%d\n", Cmd);
		fflush(stdout);
	}


	//域名解析
	std::string resolveDomainToIP(const std::string& domain)
	{
		struct addrinfo hints = { 0 }, * res;
		hints.ai_family = AF_INET;
		int result = getaddrinfo(domain.c_str(), nullptr, &hints, &res);
		if (result != 0)
		{
			throw std::runtime_error("无法解析域名: " + domain);
		}
		struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
		std::string ip = inet_ntoa(ipv4->sin_addr);
		freeaddrinfo(res);
		return ip;
	}

	GameServer() : TCP(this), UDP(this)
	{
		try
		{
			// 加载配置文件
			YAML::Node config = YAML::LoadFile(".\\Config\\config.yml");
			ServerConfig = YAML::LoadFile(".\\Config\\config.yml");

			// 从配置文件获取服务器配置
			YAML::Node Server = config["Server"];
			std::string cs = Server["Run"].as<std::string>();

			// 检查是否处于CS模式
			if (1 == 0) { // 这段代码不会执行，因为条件始终为假
				IsCS = 1; // 设置为CS模式
				SyncTime = config["SyncTime"].as<UINT>(); // 获取同步时间
				DanceTime = config["DanceTime"].as<UINT>(); // 获取跳舞时间
				GameOverTime = config["GameOverTime"].as<UINT>(); // 获取游戏结束时间
				UnlockFPS = config["UnLockFPS"].as<UINT>(); // 获取解锁FPS的设置
				// 尝试使用域名解析获取 IP
				std::string domain = Server["domain"].as<std::string>();
				try
				{
					IP = resolveDomainToIP(domain); // 如果域名解析成功，将结果赋给 IP
				}
				catch (...)
				{
					IP = Server["IP"].as<std::string>(); // 解析失败时使用配置文件中的 IP
					std::cerr << "Domain resolution failed, using IP from config: " << IP << std::endl;
				}

				TcpPort = Server["TcpPortCS"].as<USHORT>(); // 获取TCP端口（CS模式）
				UdpPort = Server["UdpPortCS"].as<USHORT>(); // 获取UDP端口（CS模式）
				Name = "QQ飞车2.0"; // 设置名称
				UITitleDispInfo_ADText = "测试区"; // UI显示的广告文本
				TransferModelStr = Server["Model"].as<std::string>(); // 获取传输模型（例如TCP/UDP）

				// 检查传输模型是否为TCP
				if (_stricmp(TransferModelStr.c_str(), "tcp") == 0) {
					TransferModel = 1; // 设置传输模型为TCP
				}

				// 初始化目录服务器
				InitDirServer(Name.c_str(), inet_addr(IP.c_str()), TcpPort, UdpPort);

				// 绑定配置
				YAML::Node Bind = Server["Bind"];
				BindIP = Bind["IP"].as<std::string>(); // 获取绑定IP
				BindTcpPort = Bind["TcpPortCS"].as<USHORT>(); // 获取绑定TCP端口（CS模式）
				BindUdpPort = Bind["UdpPortCS"].as<USHORT>(); // 获取绑定UDP端口（CS模式）

			}
			else
			{
				// 非CS模式配置
				IsCS = 0;
				SyncTime = config["SyncTime"].as<UINT>();
				DanceTime = config["DanceTime"].as<UINT>();
				GameOverTime = config["GameOverTime"].as<UINT>();
				UnlockFPS = config["UnLockFPS"].as<UINT>();
#if 0
				// 同样尝试使用域名解析获取 IP
				std::string domain = Server["domain"].as<std::string>();
				try
				{
					IP = resolveDomainToIP(domain);
				}
				catch (const std::exception& e)
				{
					IP = Server["IP"].as<std::string>(); // 解析失败时使用 IP
					std::cerr << "Domain resolution failed, using IP from config: " << IP << std::endl;
				}
#else 
				IP = Server["IP"].as<std::string>();
#endif		
				TcpPort = Server["TcpPort"].as<USHORT>();
				UdpPort = Server["UdpPort"].as<USHORT>();
				Name = Server["Name"].as<std::string>();
				UITitleDispInfo_ADText = Server["Title"].as<std::string>();

				// 检查传输模型是否为TCP
				TransferModelStr = Server["Model"].as<std::string>();
				if (_stricmp(TransferModelStr.c_str(), "tcp") == 0) {
					TransferModel = 1;
				}

				// 检查CDN配置（版本大于1.3时）
				if (version > 1.3) {
					cdn[0] = Server["CDN"]["on"].as<UINT>();
					cdn[1] = Server["CDN"]["mode"].as<UINT>();
					cdn[2] = Server["CDN"]["time"].as<UINT>();
					cdn[3] = Server["CDN"]["port"].as<UINT>();
					cdndomain = Server["CDN"]["domain"].as<std::string>();
				}

				// 如果CDN启用，调用 `lookdns()` 方法
				if (cdn[0] > 0) {
					lookdns();
				}
				else
				{
					InitDirServer(Name.c_str(), inet_addr(IP.c_str()), TcpPort, UdpPort);
				}

				// 设置绑定配置
				YAML::Node Bind = Server["Bind"];
				BindIP = Bind["IP"].as<std::string>();
				BindTcpPort = Bind["TcpPort"].as<USHORT>();
				BindUdpPort = Bind["UdpPort"].as<USHORT>();
			}
		}
		catch (...)
		{
			// 捕获异常并在控制台显示错误信息
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			printf("加载配置文件出错\n");
			//exit(0); // 注释掉的退出代码
		}
	}

	void Run()
	{
		try
		{
			TCP->SetNoDelay(TRUE);
			TCP->SetWorkerThreadCount(200);//工作线程数量
			TCP->SetSendPolicy(SP_DIRECT);
			TCP->SetOnSendSyncPolicy(OSSP_NONE);
			if (!TCP->Start(BindIP.c_str(), BindTcpPort))
			{
				DWORD error = GetLastError();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("[TCP][GameServer] Start Failed with Error Code: %lu\n", error);
				return;
			}
		}
		catch (...)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			exit(0);
		}
	}

private:
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		prints19();
		fflush(stdout);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		//加载配置到内存缓冲区 加速读写
		LoadMapConfig();
		//LoadItemConfig();
		InitDB();
		InitChatRoom();
		InitTaskInfo();
		//LoadKartPhys();//加载所有赛车性能到内存
		LoadKartAltasList();//加载车库配置到内存
		LoadMapPool();//加载地图池到内存
		LoadPetItem();//加载宠物到内存
		LoadTopList();//加载红人
		LoadTreasureBox();//加载宝箱
		LoadSkillStoneCfg();
		LoadNPCKartAIPhysParaInfo();
		LoadTopListDesc();
		LoadNPCListInfo();
		LoadKartSkill();
		LoadCommodityInfo();
		LoadMagicLampCfg();
		LoadSign3Cfg();
		LoadAdvcancedProp();
		LoadMiracleCfg();
		LoadRegisterBox(); // 存在内存泄露
		LoadRandomMapPool();
		LoadDayRaffleBox();
		LoadOutDoorEggs();
		LoadMagicFormationBoxs();
		LoadMagicExchangeShopBoxs();
		LoadPetLevelConfig();//宠物升级配置
		//initGameBegin();
		LoadAuctionCfg();
		LoadCommonBoxAwards();
		LoadCarTreasure();
		LoadSpeedConfig();
		UserDB_UpdateConnID();
		UserDB_GetAllUin(Uins);
		LoadAppellationConfig();
		LoadUserPermissionsConfig();
		if (version > 1.2) {
			SelectChallengeAllMap(ChallengeAllMaps, ChallengeMaps);
		}
		if (version > 1.3) {
			LoadMechTreasureInfoConfig();
		}
		//UserDB_UpdateMD5();
		//LoadEncryptKartItems();
		return HR_OK;
	}

	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
	{
		try {


			char ip[24] = { 0 };
			int iplen = 24;
			USHORT ipport = 0;
			pSender->GetRemoteAddress(dwConnID, ip, iplen, ipport);
			if (tcpon[0]) {
				process_ip(ip);
			}
			if (cdn[0] > 0 && cdn[1] == 0) {
				lookdns();
			}
			logAccess(ip);

			//InitDirServer("测试", inet_addr(ip), 2233, UdpPort);
			//std::cout << ip << std::endl;
			//std::cout << dwConnID << std::endl;

			if (isLog) {
				//添加日志输出
				auto ServerLogger = spdlog::get("SpeedServer_logger");
				ServerLogger->info("[OnAccept][dwConnID:{:d}][IP:{:s}]", dwConnID, ip);//输出日志
				ServerLogger->flush();
			}

			//dwConnID = 1;

			std::shared_ptr<ClientNode> Client = std::make_shared<ClientNode>();
			Client->Server = pSender;
			Client->ConnID = dwConnID;
			Client->ChatRoomID = -1;
			std::string key = ServerConfig["Server"]["Key"].as<std::string>();

			memcpy(Client->Key, LoginKey, 16);
			//memcpy(Client->Key, key.data(), 16);
			memcpy(Client->Ip, ip, 24);
			auto ClientPtr = new std::shared_ptr<ClientNode>(Client);
			pSender->SetConnectionExtra(dwConnID, ClientPtr);

		}
		catch (...)
		{
			printf("OnAccept Exceptoin!!!\n");
			return HR_ERROR;
		}

		return HR_OK;//返回成功才执行 下面流程
	}

	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID)
	{
		return HR_IGNORE;
	}


	//接收客户端信息
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		try {

			if (iLength <= 0)
			{
				return HR_ERROR;
			}
			if (ntohl(*(u_long*)pData) <= 0)
			{
				return HR_ERROR;
			}
			//printf("[GameServer][ConnID:%d]OnReceive\n", dwConnID);
			long left;
			long need;
			const int MaxBuf = 8192;
			void* p;
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = pSender->GetConnectionExtra(dwConnID, (PVOID*)&ClientPtr);
			if (ClientPtr && result) {

				auto& SharedClient = *ClientPtr;
				do
				{
					if (SharedClient->BufferPtr)
					{
						if (SharedClient->BufferOffset < sizeof(int))
						{
							//printf("SharedClient->BufferOffset:%d\n", SharedClient->BufferOffset);
							left = min(iLength, int(sizeof(int) - SharedClient->BufferOffset));
							//printf("left:%d\n", left);
							memcpy((void*)(SharedClient->BufferPtr + SharedClient->BufferOffset), pData, left);
							SharedClient->BufferOffset += left;
							if (SharedClient->BufferOffset < sizeof(int))
							{
								break;
							}
							pData += left;
							iLength -= left;
						}
						need = ntohl(*(u_long*)SharedClient->BufferPtr);

						left = need - SharedClient->BufferOffset;
						if (need > SharedClient->BufferSize)
						{
							if (need > 65535) //If the data packet sent by the client is too large, it will not be processed.
							{
								//客户端每次最多只能处理65535字节数据 超过需要分包
								//客户端包过大丢弃
								SharedClient->Server->Disconnect(SharedClient->ConnID);
								SharedClient->IsLogin = FALSE;
								SharedClient->deleteflag = 1;
								break;
								//return HR_ERROR;
							}
							if (left > MaxBuf)
							{
								left = max(iLength, MaxBuf);
								SharedClient->BufferSize = SharedClient->BufferOffset + left;
							}
							else
							{
								SharedClient->BufferSize = need;
							}
							p = realloc((void*)SharedClient->BufferPtr, SharedClient->BufferSize);
							if (!p)
							{
								//printf("内存不足\n");
								//TCP->Stop();
								return HR_ERROR;
							}
							SharedClient->BufferPtr = (ULONG_PTR)p;
						}
						else if (need <= 0)
						{
							return HR_ERROR;
						}
						if (left >= 0)
						{
							left = min(iLength, left);
							memcpy((void*)(SharedClient->BufferPtr + SharedClient->BufferOffset), pData, left);
							SharedClient->BufferOffset += left;
							if (SharedClient->BufferOffset < need)
							{
								break;
							}

							OnFullReceive(SharedClient, (BYTE*)SharedClient->BufferPtr, need);
							pData += left;
							iLength -= left;
						}
						safe_free((void*)SharedClient->BufferPtr);
						SharedClient->BufferPtr = 0;
						SharedClient->BufferOffset = 0;
					}
					while (iLength > 0)
					{
						if (iLength < sizeof(int))
						{
							need = sizeof(int);
						}
						else
						{
							need = ntohl(*(u_long*)pData);
						}
						if (need > iLength)
						{
							if (need > MaxBuf)
							{
								SharedClient->BufferSize = max(iLength, MaxBuf);
							}
							else
							{
								SharedClient->BufferSize = need;
							}
							p = safe_malloc(SharedClient->BufferSize);
							if (!p)
							{
								//printf("内存不足\n");
								//TCP->Stop();
								return HR_ERROR;
							}
							SharedClient->BufferPtr = (ULONG_PTR)p;
							memcpy((void*)SharedClient->BufferPtr, pData, iLength);
							SharedClient->BufferOffset = iLength;
							break;
						}
						else if (need <= 0)
						{
							return HR_ERROR;
						}
						else
						{
							OnFullReceive(SharedClient, (BYTE*)pData, need);
							pData += need;
							iLength -= need;
						}
					}
				} while (false);
			}
	
		}
		catch (...)
		{
			//printf(e.what());
			printf("OnReceive Exceptoin!!!\n");
			return HR_ERROR;
		}
		return HR_OK;
	}	// 接收客户端信息


	//virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	//{
	//	try {
	//		BOOL result;
	//		std::shared_ptr<ClientNode> Client;
	//		result = pSender->GetConnectionExtra(dwConnID, (PVOID*)&Client);
	//		//if (Client->Uin == 0) {
	//		//	std::shared_ptr<ClientNode> Temp = UserDB_ConnIDFindClientNode(dwConnID);
	//		//	if (Temp) {
	//		//		Client = Temp;
	//		//	}
	//		//}
	//		// printf("[游戏服务][ConnID:%d]OnReceive\n", dwConnID);
	//		if (iLength <= 0 || iLength < sizeof(int) || Client == NULL)
	//		{
	//			return HR_ERROR;
	//		}
	//		if (ntohl(*(u_long*)pData) <= 0)
	//		{
	//			return HR_ERROR;
	//		}
	//		long left;
	//		long need;
	//		const int MaxBuf = MAXWORD;
	//		void* p;
	//		if (result) {
	//			do
	//			{
	//				if (Client->BufferPtr)
	//				{
	//					if (Client->BufferOffset < sizeof(int))
	//					{
	//						left = min(iLength, int(sizeof(int) - Client->BufferOffset));
	//						memcpy((void*)(Client->BufferPtr + Client->BufferOffset), pData, left);
	//						Client->BufferOffset += left;
	//						if (Client->BufferOffset < sizeof(int))
	//						{
	//							break;
	//						}
	//						pData += left;
	//						iLength -= left;
	//					}
	//					need = ntohl(*(u_long*)Client->BufferPtr);
	//					left = need - Client->BufferOffset;
	//					if (need > Client->BufferSize)
	//					{
	//						if (need > 65535) // If the data packet sent by the client is too large, it will not be processed.
	//						{
	//							Client->Server->Disconnect(Client->ConnID);
	//							Client->IsLogin = FALSE;
	//							break;
	//						}
	//						if (left > MaxBuf)
	//						{
	//							left = max(iLength, MaxBuf);
	//							Client->BufferSize = Client->BufferOffset + left;
	//						}
	//						else
	//						{
	//							Client->BufferSize = need;
	//						}
	//						p = realloc((void*)Client->BufferPtr, Client->BufferSize);
	//						if (!p)
	//						{
	//							printf("内存不足\n");
	//							//TCP->Stop();
	//							break;
	//						}
	//						Client->BufferPtr = (ULONG_PTR)p;
	//					}
	//					else if (need <= 0)
	//					{
	//						return HR_ERROR;
	//					}

	//					if (left >= 0)
	//					{
	//						left = min(iLength, left);
	//						memcpy((void*)(Client->BufferPtr + Client->BufferOffset), pData, left);
	//						Client->BufferOffset += left;
	//						if (Client->BufferOffset < need)
	//						{
	//							break;
	//						}
	//						OnFullReceive(Client, (BYTE*)Client->BufferPtr, need);
	//						pData += left;
	//						iLength -= left;
	//					}
	//					free((void*)Client->BufferPtr);
	//					Client->BufferPtr = 0;
	//					Client->BufferOffset = 0;

	//					if (Client->BufferOffset == need)
	//					{
	//						OnFullReceive(Client, (BYTE*)Client->BufferPtr, need);

	//						free((void*)Client->BufferPtr);
	//						Client->BufferPtr = 0;
	//						Client->BufferOffset = 0;
	//					}
	//				}
	//				while (iLength > 0)
	//				{
	//					const u_long* ptr = reinterpret_cast<const u_long*>(pData);

	//					if (iLength < ntohl(*ptr))
	//					{
	//						break;
	//					}
	//					if (iLength < sizeof(int))
	//					{
	//						need = sizeof(int);
	//					}
	//					else
	//					{
	//						need = ntohl(*ptr);
	//					}
	//					if (need > iLength)
	//					{
	//						if (need > MaxBuf)
	//						{
	//							Client->BufferSize = max(iLength, MaxBuf);
	//						}
	//						else
	//						{
	//							Client->BufferSize = need;
	//						}
	//						p = malloc(Client->BufferSize);
	//						if (!p)
	//						{
	//							printf("内存不足\n");
	//							//TCP->Stop();
	//							break;
	//						}
	//						Client->BufferPtr = (ULONG_PTR)p;
	//						memcpy((void*)Client->BufferPtr, pData, iLength);
	//						Client->BufferOffset = iLength;
	//						break;
	//					}
	//					else if (need <= 0)
	//					{
	//						return HR_ERROR;
	//					}
	//					else
	//					{
	//						OnFullReceive(Client, (BYTE*)pData, need);
	//						pData += need;
	//						iLength -= need;
	//					}
	//				}
	//			} while (false);
	//		}
	//	}
	//	catch (...)
	//	{
	//		printf("OnReceive Exceptoin!!!\n");
	//		return HR_ERROR;
	//	}
	//	return HR_OK;
	//}


	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		return HR_OK;
	}

	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
	{
		std::shared_ptr<ClientNode>* ClientPtr = NULL;
		BOOL result = pSender->GetConnectionExtra(dwConnID, (PVOID*)&ClientPtr);
		if (ClientPtr)
		{
			auto& SharedClient = *ClientPtr;
			try {

				if (SharedClient->Uin > 10000 && (iErrorCode == 10054 || iErrorCode == 10053)) {
					auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
					ServerLogger->info("[抓包嫌疑:][UIN:{:d}][IP:{:s}]]", SharedClient->Uin, SharedClient->Ip);
					ServerLogger->flush();
				}

				DeleteClientBySystem(SharedClient);
				if (SharedClient->Uin > 10000)
				{
					if (SharedClient->BufferChallengePtr)
					{
						safe_free((PVOID)SharedClient->BufferChallengePtr);
					}
					if (Clients.find(SharedClient->Uin) != Clients.end())
					{
						if (Clients[SharedClient->Uin] == SharedClient)
						{
							Clients.erase(SharedClient->Uin);
						}
					}
					if (RelaxClients.find(SharedClient->Uin) != RelaxClients.end())
					{
						if (RelaxClients[SharedClient->Uin] == SharedClient)
						{
							RelaxClients.erase(SharedClient->Uin);
						}
					}
				}
				delete ClientPtr;
			}
			catch (...)
			{
				printf("OnClose Exceptoin!!!\n");
				return HR_ERROR;
			}
		}
		return HR_OK;
	}



	virtual EnHandleResult OnShutdown(ITcpServer* pSender)
	{
		printf("[GameServer]Close\n");

		CloseDB();
		return HR_OK;
	}
private:
	virtual EnHandleResult OnPrepareListen(IUdpNode* pSender, SOCKET soListen)
	{
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		//printf("[UDP]Start Success\n");
		//fflush(stdout);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		return HR_OK;
	}
	virtual EnHandleResult OnSend(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength)
	{
		return HR_OK;
	}
	virtual EnHandleResult OnReceive(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength)
	{

		if (TransferModel == 1)
		{
			return HR_OK;
		}

		TransferHead* ProtocolHead = (TransferHead*)pData;
		if (iLength < sizeof(TransferHead) && ntohs(ProtocolHead->TotalLength) > iLength)
		{
			return HR_OK;
		}
		std::shared_ptr<ClientNode> Client = NULL;
		UINT Seq = ntohl(ProtocolHead->Seq);
		short DialogID = ntohs(ProtocolHead->DialogID);
		UINT Uin = ntohl(ProtocolHead->Uin);

		short TransferCmd = ntohs(ProtocolHead->TransferCmd);
		UCHAR OptLength = ProtocolHead->OptLength;

		//printf("[UDP:OnReceive]Uin:%d, TransferCmd:%d, OptLength:%d\n", Uin, TransferCmd, OptLength);

		size_t len = 0;
		BYTE* p = PBYTE(ProtocolHead + 1);
		UCHAR option[1024];

		int num = 0;
		switch (TransferCmd)
		{
		case UDP_CMD_LOGIN_SERVER:
		{

			//udp服务器登录命令
			in_addr LocalIP;
			LocalIP.S_un.S_addr = Read32(p);
			USHORT LocalPort = Read16(p);
#
			//printf("lpszRemoteAddress:%s LocalIP:%s, LocalPort:%d\n", lpszRemoteAddress, inet_ntoa(LocalIP), LocalPort);
			p = option;

			Write32(p, inet_addr(lpszRemoteAddress));

			len = p - option;
			SendUdpData(lpszRemoteAddress, usRemotePort, UDP_CMD_LOGIN_SERVER, option, len, Seq, DialogID, Uin);



			break;
		}

		case UDP_CMD_P2P_GAME_DATA:
		{
			//UDP游戏数据 tcp传输
			Client = GetClient(Uin);
			if (!Client)
			{
				return HR_OK;
			}
			std::shared_ptr<RoomNode> Room = NULL;
			Room = GetBorderRoom(Client->BorderRoomID);
			if (!Room) {
				Room = GetRoom(Client->RoomID);
			}
			if (!Room)
			{
				return HR_OK;
			}

			len = iLength - sizeof(TransferHead) - OptLength;
			BYTE* DataBuf = p + OptLength;
			for (char i2 = 0; i2 < 6; i2++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
				if (RoomClient && RoomClient != Client)
				{
					NotifyTranferByTCP(RoomClient, Uin, 0, Seq, DataBuf, len);
					//NotifyTranferByTCP(RoomClient, Uin, 0, Seq, DataBuf, len);
					//NotifyTranferByTCP(RoomClient, Uin, Client->ConnID, Seq, DataBuf, len);
				}
			}
			break;

			//len = iLength - sizeof(TransferHead) - OptLength;
			/*printf("TransferCmd:%d iLength:%d\n ", TransferCmd, iLength);
			for (int i = 0; i < iLength; i++)
			{
				printf("%02x ", *(pData + i));
			}
			printf("\n");*/

			//BYTE* pDstInfo = p;
			//SHORT PlayerID = 0;
			//UINT DstUin = 0;
			//BYTE* DataBuf = p + OptLength;
			/*while (OptLength >= 14)
			{

			// 一个人占6个字节
				PlayerID = Read16(pDstInfo);
				DstUin = Read32(pDstInfo);
				//printf("PlayerID:%d, PlayerUin:%d  ",  PlayerID, PlayerUin);
				//DWORD dwStart = GetTickCount(); //取windows启动到现在的流逝时间(毫秒)
				for (char i2 = 0; i2 < 6; i2++)
				{
					std::shared_ptr<ClientNode> RoomClient = Clients[DstUin];
					if (RoomClient)
					{
						NotifyTranferByTCP(RoomClient, Uin, 0, Seq, DataBuf, len);
					}
				}

				//DWORD dwUsed = GetTickCount() - dwStart; //计算该函数所消耗的时间
				//printf("Clients Search lost:%d ms\n", dwUsed);
				OptLength -= 6;
			}*/
			//UINT Time = Read32(pDstInfo);
			//UINT Temp = Read32(pDstInfo);
			//printf("\n");
			/*for (char i2 = 0; i2 < 6; i2++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
				if (RoomClient && RoomClient != Client)
				{
					NotifyTranferByTCP(RoomClient, Uin, 0, Seq, DataBuf, len);
				}
			}
			break;*/
		}
		case UDP_CMD_SHOW_MY_IP_PORT:
		{
			short SrcPlayerID = Read16(p); //src player id
			UINT SrcUin = Read32(p); //src player uin
			in_addr LocalIP;
			LocalIP.S_un.S_addr = Read32(p);
			USHORT LocalPort = Read16(p);

			//printf("SrcPlayerID:%d, SrcUin:%d\n", SrcPlayerID, SrcUin);

			p = option;
			Write16(p, SrcPlayerID); //SrcPlayerID
			Write32(p, SrcUin); //SrcUin
			Write32(p, LocalIP.S_un.S_addr); //SrcOuterIP
			Write16(p, LocalPort); //SrcOuterPort
			Write32(p, inet_addr(lpszRemoteAddress)); //SrcInerIP
			Write16(p, usRemotePort); //SrcInnerPort


			len = p - option;
			SendUdpData(lpszRemoteAddress, usRemotePort, UDP_CMD_RECV_OTHER_IP_PORT, option, len, Seq, DialogID, Uin);
			break;
		}
		case UDP_CMD_HEART_BEAT:
		{
			//UDP心跳
			//p = option;
			//printf("TransferCmd:%d iLength:%d\n ", TransferCmd, iLength);
			len = iLength - sizeof(TransferHead);
			SendUdpData(lpszRemoteAddress, usRemotePort, UDP_CMD_HEART_BEAT, p, len, Seq, DialogID, Uin);
			break;



			p = option;

			len = p - option;
			SendUdpData(lpszRemoteAddress, usRemotePort, UDP_CMD_HEART_BEAT, option, len, Seq, DialogID, Uin);
			break;
		}
		default:
			//printf("TransferCmd:%d iLength:%d\n ", TransferCmd , iLength);
			//len = iLength - sizeof(TransferHead);
			//SendUdpData(lpszRemoteAddress, usRemotePort, TransferCmd, p, len, Seq, DialogID, Uin);

			break;
		}




		return HR_OK;
	}
	virtual EnHandleResult OnError(IUdpNode* pSender, EnSocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength)
	{
		return HR_OK;
	}
	virtual EnHandleResult OnShutdown(IUdpNode* pSender)
	{
		return HR_OK;
	}

};
GameServer Server;


void addip2FireWall() {
	if (!ipban) return;

	std::lock_guard<std::mutex> lock(Ipmtx);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
			if (ClientPtr && result)
			{
				auto& SharedClient = *ClientPtr;
				// 将 char 数组转换为 std::wstring
				std::wstring ipAddress = Utf8ToWide(ipall);

				// 使用 find 函数查找元素
				std::wstring speed = L"Speed";
				speed += ipAddress;

				std::wstring Prefix = GetIPPrefix(speed);
				Prefix = generateCIDRPrefix(Prefix.append(L".")).c_str();
				Prefix = L"Speed" + Prefix;
				auto itpre = firewallPrefixNames.find(Prefix);
				// 检查是否找到网段
				if (itpre != firewallPrefixNames.end()) {
					continue;
				}

				auto it = firewallRuleNames.find(speed);
				// 检查是否找到
				if (it != firewallRuleNames.end()) {
					continue;
				}


				if (!SharedClient && !UserDB_SelectIP(ipall)) {
					Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
					//添加日志输出
					auto ServerLogger = spdlog::get("SpeedServer_logger");
					ServerLogger->info("[防火墙封禁][IP:{:s}]", ipall);//输出日志
					ServerLogger->flush();
					std::cout << "Client空封禁IP：" << ipall << std::endl;
					addFirewallRuleAdd(ipAddress.c_str());
					//Client = NULL;
				}
				else {
					if (!UserDB_CmpIP(ipall) && !UserDB_SelectIP(ipall)) {
						Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
						//添加日志输出
						auto ServerLogger = spdlog::get("SpeedServer_logger");
						ServerLogger->info("[防火墙封禁][IP:{:s}]", ipall);//输出日志
						ServerLogger->flush();
						std::cout << "无对应连接封禁IP：" << ipall << std::endl;
						addFirewallRuleAdd(ipAddress.c_str());
// 						if (Client) {
// 							Client = NULL;
// 						}
					}
				}
			}	
		}
		delete[] ConnIDs;
	}
}


void addip2FireWall(UINT Uin) {
	std::string IP = UserDB_GetIP(Uin);
	if (!IP.empty() && isValidIPv4(IP)) {
		// 将 char 数组转换为 std::wstring
		std::wstring ipAddress = Utf8ToWide(IP);
		addFirewallRuleAdd(ipAddress.c_str());
		UserDB_ban(Uin);
		std::cout << "账号" << Uin << "已经封号，并且拉黑其IP:" << IP << std::endl;
	}
	else
	{
		std::cout << "账号" << Uin << "未查到IP" << std::endl;
	}

}

void addip2FireWall(std::string IP) {
	if (isValidIPv4(IP)) {
		// 将 char 数组转换为 std::wstring
		std::wstring ipAddress = Utf8ToWide(IP);
		addFirewallRuleAdd(ipAddress.c_str());
		std::cout << "IP已拉黑:" << IP << std::endl;
	}
	else
	{
		std::cout << "格式错误" << std::endl;
	}
}
void RemoveIp(std::string IP) {
	if (isValidIPv4(IP)) {
		/// 将 char 数组转换为 std::wstring
		std::wstring ipAddress = Utf8ToWide(IP);

		// 使用 find 函数查找元素
		std::wstring speed = L"Speed";
		speed += ipAddress;
		size_t removed = firewallRuleNames.erase(speed);
		if (removed > 0) {
			// 元素已删除
		}
		else {
			// 元素不存在
		}
		removeFirewallRule(speed.c_str());
	}
	else
	{
		std::cout << "格式错误" << std::endl;
	}

}

void addip2FireWall1() {

	std::lock_guard<std::mutex> lock(Ipmtx);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		std::cout << "TCP连接数：" << ConnIDCount << std::endl;
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
			if (ClientPtr && result)
			{
				auto& SharedClient = *ClientPtr;
				// 将 char 数组转换为 std::wstring
				std::wstring ipAddress = Utf8ToWide(ipall);

				// 使用 find 函数查找元素
				std::wstring speed = L"Speed";
				speed += ipAddress;

				std::wstring Prefix = GetIPPrefix(speed);
				Prefix = generateCIDRPrefix(Prefix.append(L".")).c_str();
				Prefix = L"Speed" + Prefix;
				auto itpre = firewallPrefixNames.find(Prefix);
				// 检查是否找到网段
				if (itpre != firewallPrefixNames.end()) {
					continue;
				}

				auto it = firewallRuleNames.find(speed);
				// 检查是否找到
				if (it != firewallRuleNames.end()) {
					continue;
				}


				if (!SharedClient && !UserDB_SelectIP(ipall)) {
					Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
					//添加日志输出
					auto ServerLogger = spdlog::get("SpeedServer_logger");
					ServerLogger->info("[防火墙封禁][IP:{:s}]", ipall);//输出日志
					ServerLogger->flush();
					std::cout << "Client空封禁IP：" << ipall << std::endl;
					addFirewallRuleAdd(ipAddress.c_str());
					//Client = NULL;
				}
				else {
					if (!UserDB_CmpIP(ipall) && !UserDB_SelectIP(ipall)) {
						Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
						//添加日志输出
						auto ServerLogger = spdlog::get("SpeedServer_logger");
						ServerLogger->info("[防火墙封禁][IP:{:s}]", ipall);//输出日志
						ServerLogger->flush();
						std::cout << "无对应连接封禁IP：" << ipall << std::endl;
						addFirewallRuleAdd(ipAddress.c_str());
// 						if (Client) {
// 							Client = NULL;
// 						}
					}
				}
			}
		}
		delete[] ConnIDs;
	}
}

void TcpNoIPFireWall() {

	std::lock_guard<std::mutex> lock(Ipmtx);
	//UserDB_GetAllUin(Uins);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
			if (ClientPtr && result) {
				
				auto& SharedClient = *ClientPtr;
				// 将 char 数组转换为 std::wstring
				std::wstring ipAddress = Utf8ToWide(ipall);

				if (SharedClient) {
					if (!UserDB_SelectIPNoTime(ipall)) {
						std::cout << "TcpNoIPFireWall无对应IP封禁IP：" << ipall << std::endl;
						addFirewallRuleAdd(ipAddress.c_str());
					}
				}
				else {
					std::cout << "TcpNoIPFireWall无对应Client封禁IP：" << ipall << std::endl;
					addFirewallRuleAdd(ipAddress.c_str());
				}
			}		
		}
		delete[] ConnIDs;
	}
}

void TcpNoConnidFireWall() {

	std::lock_guard<std::mutex> lock(Ipmtx);
	//UserDB_GetAllUin(Uins);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
			if (ClientPtr && result)
			{
				auto& SharedClient = *ClientPtr;
				// 将 char 数组转换为 std::wstring
				std::wstring ipAddress = Utf8ToWide(ipall);

				if (SharedClient) {
					if (!UserDB_CmpIP(ipall)) {
						std::cout << "TcpNoConnidFireWall无对应Connid封禁IP：" << ipall << std::endl;
						addFirewallRuleAdd(ipAddress.c_str());
					}
				}
				else {
					std::cout << "TcpNoConnidFireWall无对应Client封禁IP：" << ipall << std::endl;
					addFirewallRuleAdd(ipAddress.c_str());
				}
			}
			
		}
		delete[] ConnIDs;
	}
}


void SelectTcptoUIN() {

	std::lock_guard<std::mutex> lock(Ipmtx);
	//UserDB_GetAllUin(Uins);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);

			if (ClientPtr && result) {
				auto& SharedClient = *ClientPtr;
				if (SharedClient->Uin < 10000 && !UserDB_SelectIP(ipall)) {
					auto ServerLogger = spdlog::get("SpeedServer_logger");
					ServerLogger->info("[Client小于10000并且无数据库IP][UIN:{:d}][IP:{:s}]", SharedClient->Uin, ipall);//输出日志
					ServerLogger->flush();
				}
				if (SharedClient->Uin < 10000 && !UserDB_CmpIP(ipall)) {
					auto ServerLogger = spdlog::get("SpeedServer_logger");
					ServerLogger->info("[Client小于10000并且无对应链接][UIN:{:d}][IP:{:s}]", SharedClient->Uin, ipall);//输出日志
					ServerLogger->flush();
				}
			}
			else {
				auto ServerLogger = spdlog::get("SpeedServer_logger");
				ServerLogger->info("[Client不存在][IP:{:s}]", ipall);//输出日志
				ServerLogger->flush();
			}
		}
		delete[] ConnIDs;
	}
}

void ClearTcp() {

	std::lock_guard<std::mutex> lock(Ipmtx);
	UserDB_GetAllUin(Uins);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
			if (ClientPtr && result)
			{
				auto& SharedClient = *ClientPtr;
				if (!UserDB_CmpIP(ipall)) {
					Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
					std::cout << "清除连接IP：" << ipall << std::endl;
// 					if (Client) {
// 						Client = NULL;
// 					}
				}
				else {
					if (SharedClient) {
						auto it = std::find(Uins.begin(), Uins.end(), SharedClient->Uin - 10000);

						if (it != Uins.end() || SharedClient->Uin == 0) {
						}
						else {
							Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
							std::cout << "数据库未找到该玩家UIN：" << SharedClient->Uin << "，IP地址：" << SharedClient->Ip << "，已经清除连接" << std::endl;
						}
						//Client = NULL;
					}
				}
			}
		}
		delete[] ConnIDs;
	}
}

void AuToClearTcp() {
	if (!AutoCls) return;

	std::lock_guard<std::mutex> lock(Ipmtx);

	UserDB_GetAllUin(Uins);
	int iplen = 24;
	USHORT ipport = 0;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0) {
		char ipall[24] = { 0 };
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ipall, iplen, ipport);//比较ip
			std::shared_ptr<ClientNode>* ClientPtr = NULL;
			BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
			if (ClientPtr && result)
			{
				auto& SharedClient = *ClientPtr;
				if (!UserDB_CmpIP(ipall)) {
					Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
					std::cout << "自动清除连接，IP：" << ipall << std::endl;
// 					if (Client) {
// 						Client = NULL;
// 					}
				}
				else {
					if (SharedClient) {
						auto it = std::find(Uins.begin(), Uins.end(), SharedClient->Uin - 10000);

						if (it != Uins.end() || SharedClient->Uin == 0) {
						}
						else {
							Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
							std::cout << "数据库未找到该玩家UIN：" << SharedClient->Uin << "，IP地址：" << SharedClient->Ip << "，已经自动清除连接" << std::endl;
							/*if (Client->IsLogin) {
								NotifyKickPlayer(Client, "\xb7\xe2\xb0\xfc\xd2\xec\xb3\xa3\xc4\xe3\xd2\xd1\xb1\xbb\xb7\xe2\xba\xc5\x21", 1);
								Client->Server->Disconnect(Client->ConnID);
							}*/
						}
						//Client = NULL;
					}
				}
			}	
		}
		delete[] ConnIDs;
	}
}


void DisConnectUser(char* dstip, UINT ConnIDUin)
{
	char ip[24] = { 0 };
	int iplen = 24;
	USHORT ipport = 0;
	std::shared_ptr<ClientNode>* ClientPtr = NULL;
	DWORD ConnIDCount = Server.TCP->GetConnectionCount();
	if (ConnIDCount != 0)
	{
		CONNID* ConnIDs = new CONNID[ConnIDCount];
		memset(ConnIDs, 0, sizeof(CONNID) * ConnIDCount);
		Server.TCP->GetAllConnectionIDs(ConnIDs, ConnIDCount);
		for (DWORD i = 0; i < ConnIDCount; i++)
		{
			Server.TCP->GetRemoteAddress(ConnIDs[i], ip, iplen, ipport);//比较ip
			if (strcmp(dstip, ip) == 0)
			{
				BOOL result = Server.TCP->GetConnectionExtra(ConnIDs[i], (PVOID*)&ClientPtr);
				if (ClientPtr && result)
				{
					auto& SharedPtr = *ClientPtr;
					if (SharedPtr->ConnIDUin == ConnIDUin)
					{
						Server.TCP->Disconnect(ConnIDs[i]);//删除对应ip对应Uin的连接
						//printf("Disconnect ConnID:%d ConnIDUin:%d\n", ConnIDs[i], ConnIDUin);
					}
					//Client = NULL;
				}
			}
		}
		delete[] ConnIDs;
	}

}

BOOL WINAPI ConsoleHandler(DWORD signal) {
	if (signal == CTRL_CLOSE_EVENT) {//关闭控制台
		stopFlag = true;
		cv.notify_all();
		std::cout << "Console window is closing, perform cleanup here." << std::endl;
		const auto& myLogger = spdlog::get("SpeedServer_logger");
		myLogger->flush();// 刷新，否则只有程序结束时才会把日志写入文件
		spdlog::drop_all();//销毁日志系统
		//CloseDB();
		return TRUE;
	}
	return FALSE;
}




// 崩溃时的回调函数
LONG WINAPI UnhandledExceptionHandler1(EXCEPTION_POINTERS* pExceptionInfo) {
	// 在这里执行崩溃前的清理工作
	// 例如：记录崩溃日志

	_EXCEPTION_RECORD* exceptionRecord = pExceptionInfo->ExceptionRecord;
	const auto& myLogger = spdlog::get("SpeedServer_logger");
	// 打印异常代码和地址
	// 记录异常代码和地址到日志
	myLogger->info("[Exception occurred]");

	myLogger->info("Exception code: 0x%x", exceptionRecord->ExceptionCode);
	myLogger->info("Exception address: 0x%p", exceptionRecord->ExceptionAddress);
	// 根据异常代码输出对应的错误信息
	// 
	// 输出异常类型
	const char* exceptionType;
	switch (exceptionRecord->ExceptionCode) {
	case EXCEPTION_ACCESS_VIOLATION:
		exceptionType = "EXCEPTION_ACCESS_VIOLATION";
		break;
	case EXCEPTION_STACK_OVERFLOW:
		exceptionType = "EXCEPTION_STACK_OVERFLOW";
		break;
		// 可以根据需要添加其他异常类型的处理
	default:
		exceptionType = "Unknown Exception";
		break;
	}
	myLogger->info("Exception type: %s", exceptionType);


	myLogger->flush();// 刷新，否则只有程序结束时才会把日志写入文件
	spdlog::drop_all();//销毁日志系统

	std::cerr << "Fatal Error: Unhandled Exception!" << std::endl;

	// 这里可以添加更多的错误处理逻辑，比如保存崩溃信息到文件

	// 返回 EXCEPTION_EXECUTE_HANDLER 让程序终止
	//return EXCEPTION_EXECUTE_HANDLER;


	// 这里做一些异常的过滤或提示
	if (IsDebuggerPresent()) {
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return GenerateMiniDump(pExceptionInfo);


}

LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo) {
	HANDLE hDumpFile = CreateFileW(
		L"crash_dump.dmp",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hDumpFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create dump file. Error: " << GetLastError() << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}

	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ExceptionPointers = pExceptionInfo;
	dumpInfo.ClientPointers = FALSE;

	if (MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hDumpFile,
		MiniDumpWithDataSegs,
		&dumpInfo,
		NULL,
		NULL)) {
		std::cerr << "Dump file created successfully." << std::endl;
	}
	else {
		std::cerr << "Failed to write dump file. Error: " << GetLastError() << std::endl;
	}

	CloseHandle(hDumpFile);
	return EXCEPTION_EXECUTE_HANDLER;
}

void setConsoleBold(bool bold) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	// Set bold attribute
	if (bold) {
		cfi.dwFontSize.Y += 2; // Increase font size for bold effect
	}
	else {
		cfi.dwFontSize.Y -= 2; // Decrease font size to original
	}
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

void setConsoleFont(const std::wstring& fontName, int fontSize) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };

	// 获取当前的控制台字体信息
	if (!GetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
		std::wcerr << L"Failed to get console font info." << std::endl;
		return;
	}

	// 设置字体名称
	wcscpy_s(cfi.FaceName, fontName.c_str());
	// 设置字体大小（X 和 Y 的值应为相同的，以保持字体比例）
	cfi.dwFontSize.X = fontSize;
	cfi.dwFontSize.Y = fontSize;

	// 应用新的字体设置
	if (!SetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
		std::wcerr << L"Failed to set console font." << std::endl;
	}
}

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* pExceptionPointers) {
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	char dumpFileName[256];
	sprintf_s(dumpFileName, "crash_%04d%02d%02d_%02d%02d%02d.dmp",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	HANDLE hFile = CreateFileA(dumpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionPointers;
		ExInfo.ClientPointers = TRUE;

		if (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL)) {
#ifdef _CONSOLE
			printf("Crash dump created successfully: %s\n", dumpFileName);
#else
			MessageBoxA(NULL, "Crash dump file created successfully!", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#endif
		}
		else {
#ifdef _CONSOLE
			printf("Failed to write dump file.\n");
#else
			MessageBoxA(NULL, "Failed to write dump file.", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#endif
		}

		CloseHandle(hFile);
	}
	else {
#ifdef _CONSOLE
		printf("Failed to create dump file.\n");
#else
		MessageBoxA(NULL, "Failed to create dump file.", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#endif
	}

	EXCEPTION_RECORD* record = pExceptionPointers->ExceptionRecord;
	printf("Exception code: 0x%08X\n", record->ExceptionCode);
	printf("Exception address: 0x%p\n", record->ExceptionAddress);

	return EXCEPTION_EXECUTE_HANDLER;
}

int main() {

	versionStr = std::to_string(version);
	if (versionStr.find('.') != std::string::npos) {
		versionStr = versionStr.substr(0, versionStr.find('.') + 2); // 保留到小数点后一位
	}

	start = std::chrono::high_resolution_clock::now();

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	srand((unsigned)time(0) + GetSysTimeMicros());


	SetConsoleTitle(UITitleDispInfo_ADText.c_str());

	if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
		std::cerr << "ERROR: Could not set control handler." << std::endl;
		return 1;
	}

	SetUnhandledExceptionFilter(ExceptionHandler);

	setConsoleBold(true);

	spdlog_init();
	auto ServerLogger = spdlog::get("SpeedServer_logger");
	if (!ServerLogger) {
		std::cerr << "Failed to initialize logger" << std::endl;
		return -1;
	}

	ServerLogger->info("[启动飞车服务端]");
	ServerLogger->info("Service version: {}", versionStr);
	ServerLogger->flush();

	Server.Run();

	SystemClient = std::make_shared<ClientNode>();
	//memset(SystemClient, 0, sizeof(ClientNode));
	memcpy(SystemClient->NickName, "系统消息", 8);

	std::vector<std::thread> threads;
	auto SafeThreadStart = [](std::function<void()> func) {
		try {
			func();
		}
		catch (const std::exception& e) {
			auto logger = spdlog::get("SpeedServer_logger");
			if (logger) {
				logger->error("Thread exception: {}", e.what());
			}
		}
		catch (...) {
			auto logger = spdlog::get("SpeedServer_logger");
			if (logger) {
				logger->error("Unknown exception in thread");
			}
		}
	};

	// 启动线程
	threads.emplace_back(SafeThreadStart, Run2);
	threads.emplace_back(SafeThreadStart, Run3);
	threads.emplace_back(SafeThreadStart, Run4);
	threads.emplace_back(SafeThreadStart, Run6);
	threads.emplace_back(SafeThreadStart, Run7);
	threads.emplace_back(SafeThreadStart, Run8);
	threads.emplace_back(SafeThreadStart, Run9);
	threads.emplace_back(SafeThreadStart, Run10);
	threads.emplace_back(SafeThreadStart, Run11);
	threads.emplace_back(SafeThreadStart, Run12);
	if (GMTime[3] > 0) {
		threads.emplace_back(SafeThreadStart, Run14);
	}
	threads.emplace_back(SafeThreadStart, Run15);
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}
	// 清理资源
// 	if (SystemClient) {
// 		delete SystemClient;
// 		SystemClient = nullptr;
// 	}
	CloseDB();
	return 0;
}


