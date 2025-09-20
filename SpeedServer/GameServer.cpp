#include "GameServer.h"
#include "ConnectionPool.h"
#include <filesystem>
#include "Map.h"
#include <shared_mutex>
#include <bitset>


//#include "ConmData.h"
extern std::recursive_mutex Mutex;

void NotifyRaceOverNew(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, bool First);
std::map<long, std::shared_ptr<ClientNode>> Clients;
extern std::string IP;
extern USHORT TcpPort;
extern USHORT UdpPort;
extern int BorderTimes[4];
extern bool is60fps;
extern int raceAdd;
extern int mapLimit;
extern UINT raceid;
extern std::shared_ptr<MIRACLECFG> MiracleCfg;
extern int GuildAdd;
extern int beiginmode;

BYTE taskaward[128] = { 0 };
BYTE BorderOver[187];
BYTE RankedOver[402];


void ResponseGetSuperN2O(std::shared_ptr<ClientNode> Client);
void ResponseGetSuperN2O2(std::shared_ptr<ClientNode> Client);
extern std::string UITitleDispInfo_ADText;
extern std::shared_ptr<ClientNode> GetClient(long Uin);
extern std::string GetItemName(UINT Itemid);
extern UINT GameOverTime;
extern UINT IsCS;
extern int Uinew;
int Prob = 290;
int ProbTime[2] = { 15,3 };
int LoseRate = 6;
int GradeAddSet[6] = { 10,8,6,5,5,5 };
int Addexp = 25000;
BYTE jump24255[91];
std::map<UINT, ItemInfo> EndGame;

extern void NotifyChangeReadyState(std::shared_ptr<ClientNode> Client, UINT Uin, bool State);
extern void NotifyUseItemResult3(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum, bool AddorReduce);

void NotifyGenGameEndBoxAward(std::shared_ptr<ClientNode> Client);
void NotifyRankedMatchSeasonInfo(std::shared_ptr<ClientNode> Client, UINT SrcUin);
UINT RankGradeLevel[8] = { 0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b };
extern void RequestEnter3DRoom(std::shared_ptr<ClientNode> Client);
extern void NotifyChangeRoomOwner(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);
extern UINT UnlockFPS;
extern PETITEM* GetPetItem(long PetID);
//extern TREASUREBOX* GameEndBox;
extern std::vector<ItemInfo> GameEndBox;
extern int GameEndBoxTime[4];

void NotifyGameOver(std::shared_ptr<ClientNode> Client, USHORT LeaveGameType, BYTE ParaNum, int* ParaList);
extern bool UpdateChallengeMapStatus(UINT mapId, USHORT rank, int newStatus);
extern void QuickbugLeaveGame(std::shared_ptr<ClientNode> Client);
extern void ResponseChangeOBState(std::shared_ptr<ClientNode> Client, BOOL OBState);
extern int Rankset[5];
extern int Borderset[6];
extern SUPERPROP SuperProp[3];
extern char rankstone;
extern std::shared_ptr<ADVANCEDCFG> BorderAdvancedCfg;
extern std::shared_ptr<ADVANCEDCFG> RankAdvancedCfg;
extern int AllMapLimitTime;
extern UINT NpcNum;
extern UINT GetItemGameType(UINT ID);
extern void ResponseGetSuperN2O1(std::shared_ptr<ClientNode> Client, UINT PropID);
extern ConnectionPool player;
extern YAML::Node GetPetConfig(UINT PetID);
extern std::map<UINT, UINT> GetPetStrengLevel(UINT PetID, UINT StrengLevel);
extern std::string GetItemMess(UINT ID);
extern std::map<UINT, std::vector<CarTreasureInfo>> CommonBoxAwards;
EncryptKartItem* GetEncryptKartItem(UINT KartID);
EncryptKartItem* GetEncryptKartItemNew(UINT KartID, bool isPet);
extern int mode28346;
extern std::list<UINT> modecar;
extern int RankAdd[2];
extern int isRealTime[2];
extern int ranknoticeon;
extern std::string ranknoticetext;
extern int beginMode;
extern int SwitchFlag1;
extern std::vector<UINT> ForbidMaps;
extern std::vector<NewMapsInfo> NewMaps;
extern int Stake[2];
extern int IsBottomGame[4];
extern std::map<UINT, std::shared_ptr<OpenBoxDef>> GameEndBox2Uin;
extern int DisplayBitMapConfig[5];
extern int Mantissa[4];
//================================================================================================
struct MySkinRefitEffectsInfo {
	//UINT KartSkinID;
	UINT PID;
	UINT AffectID;
	UINT Param;
	UINT LifeTime;
	UINT RefitItem[10];
};


int getMaxScorce(UINT level) {
	switch (level) {
	case 20:
		return 20;
	case 21:
		return 30;
	case 22:
		return 40;
	case 23:
		return 50;
	case 24:
		return 60;
	case 25:
		return 75;
	case 26:
		return 300;
	case 27:
		return 9999;
	default:
		return 0;
	}
}
//赛车皮肤超级喷配置
const static std::unordered_map<UINT, MySkinRefitEffectsInfo> MySkinRefitEffectInfoMap = {
	// 初始化内容，示例：
	// {kartID, MySkinRefitEffectsInfo{kartID, PID, AffectID, Param, LifeTime, {RefitItem数组}}}
	{135165, { 777, 167, 6750, 3300, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 780 } }},//至尊影龙·青龙
	{135166, { 777, 143, 6700, 3100, { 14300, 0, 0, 0, 0, 0, 0, 0, 0, 778 } }},//至尊影龙·应龙
	{135167, { 777, 167, 6800, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 781 } }},//至尊影龙·烛龙
	{135168, { 777, 143, 6700, 3100, { 14300, 0, 0, 0, 0, 0, 0, 0, 0, 779 } }},//至尊影龙·冰夷
	{138124, { 777, 167, 6750, 3300, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 780 } }},//至尊盘龙·青龙
	{138125, { 777, 143, 6700, 3100, { 14300, 0, 0, 0, 0, 0, 0, 0, 0, 778 } }},//至尊盘龙·应龙
	{138126, { 777, 167, 6800, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 781 } }},//至尊盘龙·烛龙
	{138127, { 777, 143, 6700, 3100, { 14300, 0, 0, 0, 0, 0, 0, 0, 0, 779 } }},//至尊盘龙·冰夷 
	{144521, { 777, 143, 6700, 3100, { 14300, 0, 0, 0, 0, 0, 0, 0, 0, 779 } }},//至尊-神·奥丁
	{138413, { 777, 167, 6850, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 783 } }},//X次元-青龙
	{146556, { 777, 167, 6850, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 784 } }},//X次元-白虎
	{146558, { 777, 167, 6850, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 785 } }},//X次元-朱雀
	{146559, { 777, 167, 6850, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 786 } }},//X次元-玄武
	{150527, { 777, 167, 6850, 3350, { 16700, 0, 0, 0, 0, 0, 0, 0, 0, 787 } }},//至尊-真武
};

UINT ReportDataFlag = 0;
UINT  CheckDataNum = 0;
UINT  P2PMode = 0;
UINT  TcpFrequence = 0;
UINT  MultiInfoLen = 0;
UINT  FeedBackEnabled = 0;
UINT  SpeedSectionNum = 0;
UINT  NormalSpeedSectionNum = 0;
UINT  EnableAntiDriftCheat = 0;
UINT  GameSeq = 0;
UINT  GameSeqIDHigh = 0;
UINT  GameSeqIDLow = 0;
UINT  MonitorCheckPointBegin = 0;
UINT  MonitorCheckPointEnd = 0;
UINT  MonitorLapCnt = 0;
UINT  ReportAntiCollisionDataTime = 0;
UINT  MoveFreq = 0;
UINT  RecordCheckCondNum = 0;
UINT  IsCrossNoCollision = 0;
UINT  GameBeginSwitchFlag = 0;
UINT  AnonymousMode = 0;
UINT  HasJiJia = 0;
std::vector<UINT> GLSpeclEffectNum;
extern YAML::Node BaseGameConfig;
YAML::Node getGameConfig()
{
	return BaseGameConfig;
}
void initGameBegin() {
	ReportDataFlag = getGameConfig()["GameBegin"]["ReportDataFlag"].as<UINT>();
	CheckDataNum = getGameConfig()["GameBegin"]["CheckDataNum"].as<UINT>();
	P2PMode = getGameConfig()["GameBegin"]["P2PMode"].as<UINT>();
	TcpFrequence = getGameConfig()["GameBegin"]["TcpFrequence"].as<UINT>();
	MultiInfoLen = getGameConfig()["GameBegin"]["MultiInfo"].as<UINT>();
	FeedBackEnabled = getGameConfig()["GameBegin"]["FeedBackEnabled"].as<UINT>();
	SpeedSectionNum = getGameConfig()["GameBegin"]["SpeedSectionNum"].as<UINT>();
	NormalSpeedSectionNum = getGameConfig()["GameBegin"]["NormalSpeedSectionNum"].as<UINT>();
	EnableAntiDriftCheat = getGameConfig()["GameBegin"]["EnableAntiDriftCheat"].as<UINT>();
	GameSeq = getGameConfig()["GameBegin"]["GameSeq"].as<UINT>();
	GameSeqIDHigh = getGameConfig()["GameBegin"]["GameSeqIDHigh"].as<UINT>();
	GameSeqIDLow = getGameConfig()["GameBegin"]["GameSeqIDLow"].as<UINT>();
	MonitorCheckPointBegin = getGameConfig()["GameBegin"]["MonitorCheckPointBegin"].as<UINT>();
	MonitorCheckPointEnd = getGameConfig()["GameBegin"]["MonitorCheckPointEnd"].as<UINT>();
	MonitorLapCnt = getGameConfig()["GameBegin"]["MonitorLapCnt"].as<UINT>();
	ReportAntiCollisionDataTime = getGameConfig()["GameBegin"]["ReportAntiCollisionDataTime"].as<UINT>();
	MoveFreq = getGameConfig()["GameBegin"]["MoveFreq"].as<UINT>();
	RecordCheckCondNum = getGameConfig()["GameBegin"]["RecordCheckCondNum"].as<UINT>();
	IsCrossNoCollision = getGameConfig()["GameBegin"]["IsCrossNoCollision"].as<UINT>();
	GameBeginSwitchFlag = getGameConfig()["GameBegin"]["GameBeginSwitchFlag"].as<UINT>();
	AnonymousMode = getGameConfig()["GameBegin"]["AnonymousMode"].as<UINT>();
	HasJiJia = getGameConfig()["GameBegin"]["HasJiJia"].as<UINT>();
}


BYTE jump24238[91];
DWORD WINAPI OnSingleJump(PVOID Client)
{
	auto ClientPtr = reinterpret_cast<std::shared_ptr<ClientNode>*>(Client);
	if (ClientPtr)
	{
		auto& SharedClient = *ClientPtr;
		while (1)
		{
			Sleep(30000);//模拟30秒跳一次
			if (SharedClient->TaskID == 0xFFFE && SharedClient->IsLogin)
			{
				BYTE* p = jump24238;
				Write32(p, SharedClient->Uin);
				SendToClient(SharedClient, 24238, jump24238, 91, SharedClient->ConnID, FE_PLAYER, SharedClient->ConnID, Notify);

			}
			if (SharedClient->TaskID == 0xFFFF && SharedClient->IsLogin)
			{
				//跳关
				SendToClient(SharedClient, 24255, jump24255, 91, SharedClient->ConnID, FE_PLAYER, SharedClient->ConnID, Notify);
			}
			if (SharedClient->TaskID == 0)
			{
				SharedClient->JumpThread = 0;
				break;
			}
		}
		delete ClientPtr;
	}
	return 0;
}

DWORD WINAPI OnSingleOver(PVOID Client)
{
	auto ClientPtr = reinterpret_cast<std::shared_ptr<ClientNode>*>(Client);
	auto& SharedClient = *ClientPtr;
	if (SharedClient->TaskID == 0xFFFE)
	{
		SharedClient->TaskID = 0;
		SharedClient->IsFinish = FALSE;
		SendToClient(SharedClient, 24238, (const BYTE*)"\x00\x00\x27\x11\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x1e\x00\x00\x00\x00\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x21\x00\x00\x00\x00\x00\x00\x00\x21\x00\x00\x00\x00\x00\x00", 72, SharedClient->ConnID, FE_PLAYER, SharedClient->ConnID, Notify);
		Sleep(5000);
		SendToClient(SharedClient, 514, (const BYTE*)"\x00\x00\x00\x00", 4, SharedClient->GameID, FE_GAMELOGIC, SharedClient->ConnID, Notify);
	}
	else if (SharedClient->TaskID == 0xFFFF)
	{
		SharedClient->TaskID = 0;
		SharedClient->IsFinish = FALSE;
		BYTE buf[8192] = { 0 };
		BYTE* p = buf;
		size_t len = 0;
		memcpy(p, "\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x52\x00\x20", 76);
		p += 76;
		Write32(p, 0); //Uin
		Write8(p, 0); //NameLen
		Write32(p, 0); //RecoverNum
		Write32(p, SharedClient->JumpLevel); //CrossLevelNum
		Write32(p, SharedClient->LapTime * 10); //RaceTime
		Write32(p, 120); //HistoryLevel
		Write32(p, 0); //JumpLevel
		Write32(p, 0); //AwardActiveValue
		Write8(p, 0); //IsHang
		memcpy(p, "\x00\x00\x00\x01\x00\x2c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x50\x75", 55);
		p += 55;
		len = p - buf;
		SendToClient(SharedClient, 24255, (const BYTE*)buf, len, SharedClient->ConnID, FE_PLAYER, SharedClient->ConnID, Notify);
		Sleep(5000);
		SendToClient(SharedClient, 514, (const BYTE*)"\x00\x00\x00\x00\x00\x00\x00\x00", 8, SharedClient->GameID, FE_GAMELOGIC, SharedClient->ConnID, Notify);

		const char* buf7 = "\x00\x00\x00\x01\x00\x2e\x00\x01\x06\x00\xb4\xf3\xbc\xd2\xd2\xbb\xc6\xf0\xcc\xf4\xd5\xbd\xbc\xab\xcf\xde\xa3\xa1\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x64\x70\x64\x1c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x08\x47\x7e\x1d\x00\x00\x00\x00\x01\x00\x11\x00\x00\x00\x00\x00\x04\x01\x01\x00\x08\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x0f\x24\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x0f\x24\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x17\x31\x2d\x31\x38\x37\x32\x34\x37\x33\x37\x36\x37\x2d\x31\x36\x38\x35\x30\x38\x37\x32\x36\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x72\x69";
		SendToClient(SharedClient, 103, (BYTE*)buf7, 207, SharedClient->ServerID, FE_GAMESVRD, SharedClient->ConnID, Response);

	}

	delete ClientPtr;
	return 0;
}

//================================================================================================
std::map<UINT, std::shared_ptr<KartItem28346>> KartItems28346;
std::mutex kartMutex; // Mutex for KartItems28346
std::mutex g_heap_mutex;
std::map<UINT, std::shared_ptr<EncryptKartItem>> EncryptKartItems;//加密的赛车性能
std::shared_mutex mtxEncryptKartItems; // 读写锁，允许多个线程同时读取数据
void Reloadracingdata() {
	std::unique_lock<std::shared_mutex> lock(mtxEncryptKartItems); // 使用 unique_lock 进行写锁定
	KartItems28346.clear();
	EncryptKartItems.clear();
	printf("重新加载赛车数据成功\n");
}


EncryptKartItem* GetEncryptKartItem(UINT KartID) {
	{
		std::shared_lock<std::shared_mutex> lock(mtxEncryptKartItems); // 使用 shared_lock 进行读锁定
		auto it = EncryptKartItems.find(KartID);
		if (it != EncryptKartItems.end()) {
			return it->second.get();
		}
	}

	// 如果没有找到，尝试加载数据并插入到 EncryptKartItems 中
	char FilePath[MAX_PATH];
	sprintf_s(FilePath, ".\\EncryptKart\\%d.yml", KartID);
	std::filesystem::path file_path(FilePath);
	namespace fs = std::filesystem;
	YAML::Node config;

	if (!fs::exists(FilePath)) {
		sprintf_s(FilePath, ".\\EncryptKart\\%d.yml", 0);
	}
	config = YAML::LoadFile(FilePath);

	std::vector<unsigned char> extractedData = config["Data"].as<std::vector<unsigned char>>();

	std::shared_ptr<EncryptKartItem> pKartItem = std::make_shared<EncryptKartItem>();
	pKartItem->EncryptData.resize(extractedData.size());
	std::memcpy(pKartItem->EncryptData.data(), extractedData.data(), extractedData.size());

	{
		std::unique_lock<std::shared_mutex> lock(mtxEncryptKartItems); // 使用 unique_lock 进行写锁定
		EncryptKartItems[KartID] = pKartItem;
	}

	return EncryptKartItems[KartID].get();
}

KartItem28346* GetKartItem28346(UINT KartID)
{
	// Lock the mutex before accessing shared data
	std::lock_guard<std::mutex> lock(kartMutex);

	std::shared_ptr<KartItem28346> pKartItem28346 = KartItems28346[KartID];
	if (!pKartItem28346)
	{
		char FilePath[MAX_PATH];
		sprintf_s(FilePath, ".\\Kart28346\\%d.yml", KartID);
		namespace fs = std::filesystem;
		YAML::Node config;
		if (fs::exists(FilePath))
		{
			config = YAML::LoadFile(FilePath);
			std::shared_ptr<KartItem28346> pKartItem28346 = std::make_shared<KartItem28346>();
			pKartItem28346->ByteY = config["ByteY"].as<std::string>();
			pKartItem28346->ByteP = config["ByteP"].as<std::string>();
			KartItems28346[KartID] = pKartItem28346;
		}
		else
		{
			std::string mess = GetItemMess(KartID);
			std::string path = ".\\Kart28346\\0.yml";
			if (mess == "D级赛车")
			{
				path = ".\\Kart28346\\0D.yml";
			}
			else if (mess == "C级赛车")
			{
				path = ".\\Kart28346\\0C.yml";
			}
			else if (mess == "B级赛车")
			{
				path = ".\\Kart28346\\0B.yml";
			}
			else if (mess == "A级赛车")
			{
				path = ".\\Kart28346\\0A.yml";
			}
			else if (mess == "S级赛车")
			{
				path = ".\\Kart28346\\0S.yml";
			}
			else if (mess == "T1级赛车")
			{
				path = ".\\Kart28346\\0T1.yml";
			}
			else if (mess == "T2级赛车")
			{
				path = ".\\Kart28346\\0T2.yml";
			}
			else if (mess == "T3级赛车")
			{
				path = ".\\Kart28346\\0T3.yml";
			}
			else if (mess == "赛车皮肤")
			{
				path = ".\\Kart28346\\0S.yml";
			}
			else {
				path = ".\\Kart28346\\0A.yml";
			}
			config = YAML::LoadFile(path);
			std::shared_ptr<KartItem28346> pKartItem28346 = std::make_shared<KartItem28346>();
			pKartItem28346->ByteY = config["ByteY"].as<std::string>();
			pKartItem28346->ByteP = config["ByteP"].as<std::string>();
			KartItems28346[KartID] = pKartItem28346;
		}
	}
	return KartItems28346[KartID].get();
}

void Write21032(std::shared_ptr<ClientNode> Client) {
	char FilePath[MAX_PATH];
	sprintf_s(FilePath, ".\\21032\\%d.yml", 0);
	namespace fs = std::filesystem;
	YAML::Node config;
	if (fs::exists(FilePath))
	{
		try
		{
			config = YAML::LoadFile(FilePath);
			YAML::Node config21032 = config["Bytes"];
			for (size_t i = 0; i < config21032.size(); i++)
			{
				YAML::Node Temp = config21032[i];
				const std::string& Byte = Temp["Byte"].as<std::string>();
				const unsigned char* Msg21032 = EHexToByte(Byte.data());
				SendToClient(Client, 21032, (const BYTE*)Msg21032, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
				delete[] Msg21032;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}

	}
}

void WriteKartPhysParam28346(std::shared_ptr<ClientNode> Client) {
	try
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data();

		UINT LoadID = Client->KartID;

		//Client->lastUpdateTime = std::chrono::steady_clock::time_point{};
		Client->SuperN2O = 0;
		Client->SuperJet = 0;
		Client->getPropNum = 0;
		Client->SuperJetKartFuelInc = 0;
		Client->CurrentAccelerateFuel = 0;
		Client->PropID = 0;

		//UINT PhysInfosize;
		bool IsMySkinRefitEffect = false;
		Client->IsRefitItem = false;
		auto it = MySkinRefitEffectInfoMap.find(Client->KartSkinID);
		if (it != MySkinRefitEffectInfoMap.end()) {
			//判断是否为特殊皮肤
			IsMySkinRefitEffect = true;
		}
		if (Client->Uin > 10000) {
			if (Client->KartSkinID == 127616) {
				//PhysInfosize += 1;
				Client->IsRefitItem = true;
			}
			if (IsMySkinRefitEffect) {
				//++PhysInfosize;
				Client->IsRefitItem = true;
			}
			if (Client->KartSkinID > 0) {
				LoadID = Client->KartSkinID;
			}
		}

		std::string Byte;
		KartItem28346* pKartItem28346;
		pKartItem28346 = GetKartItem28346(LoadID);

		if (Client->Pet && Client->Pet->ID > 0)
		{
			Byte = pKartItem28346->ByteP;
		}
		else
		{
			Byte = pKartItem28346->ByteY;
		}
		const unsigned char* Msg28346 = EHexToByte(Byte.data());
		SendToClient(Client, 28346, (const BYTE*)Msg28346, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		if (Msg28346)
			delete[] Msg28346;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("WriteKartPhysParam28346 Error\n");
	}

}


void WriteKartPhysParam(BYTE*& p, UINT Uin, UINT KartID, std::shared_ptr<ClientNode> Client)
{
	//写赛车物理性能 客户端根据这个决定赛车的性能
	WORD len;
	BYTE* pKartPhysParam = p;

	Write16(pKartPhysParam, 0); //len


	INT curskill1, curskill2, curskill3;
	INT curskillv1 = 0;
	INT	curskillv2 = 0;
	INT	curskillv3 = 0;
	INT strengv1 = 0;
	INT strengv2 = 0;
	INT strengv3 = 0;
	INT skilln2o, skillhide, skillview, convertspeed;
	INT PetID = 0;
	INT status = 0;
	INT level = 0;
	INT strenglevel = 0;
	skilln2o = skillhide = skillview = convertspeed = 0;
	INT SuperN2ORate = 0, SuperN2OTime = 0;

	int RefitCout = 0;
	short MaxFlags = 0;
	short WWeight = 0;
	short SpeedWeight = 0;
	short JetWeight = 0;
	short SJetWeight = 0;
	short AccuWeight = 0;
	short AntiCollisionWeight = 0;
	short Speed2Weight = 0;
	short DriftVecWeight = 0;
	short AdditionalZSpeedWeight = 0;
	int ShapeRefitCount = 0;
	int KartHeadRefitItemID = 0;
	int KartTailRefitItemID = 0;
	int KartFlankRefitItemID = 0;
	int KartTireRefitItemID = 0;
	char peton = 0;
	int CompensateParamPet = 1;
	int CrashCompensatePowerPet = 0;
	int JetWeightPet = 0;
	int SJetWeightPet = 0;
	int AccuWeightPet = 0;
	int SpeedWeightPet = 0;
	if (Uin > 10000) //Not NPC
	{
		Client->SuperN2O = 0;
		Client->SuperJet = 0;
		Client->getPropNum = 0;
		Client->SuperJetKartFuelInc = 0;
		Client->CurrentAccelerateFuel = 0;
		Client->PropID = 0;
		Client->IsRefitItem = false;
		WWeight = Client->ClientKartRefitItemInfo.WWeight;
		SpeedWeight = Client->ClientKartRefitItemInfo.SpeedWeight;
		JetWeight = Client->ClientKartRefitItemInfo.JetWeight;
		SJetWeight = Client->ClientKartRefitItemInfo.SJetWeight;
		AccuWeight = Client->ClientKartRefitItemInfo.AccuWeight;
		AntiCollisionWeight = Client->ClientKartRefitItemInfo.AntiCollisionWeight;
		Speed2Weight = Client->ClientKartRefitItemInfo.Speed2Weight;
		DriftVecWeight = Client->ClientKartRefitItemInfo.DriftVecWeight;
		AdditionalZSpeedWeight = Client->ClientKartRefitItemInfo.AdditionalZSpeedWeight;

		//查询是否装备有改装加成卡120258
		if (Client->GlobalAddcard == 1)
		{
			//如果有改装加成卡 全车29
			//进阶全40
			AntiCollisionWeight = SpeedWeight = JetWeight = SJetWeight = AccuWeight = 29;
			Speed2Weight = DriftVecWeight = AdditionalZSpeedWeight = 40;
		}

		//查询PetItem所有宠物id 并查看Item是否有装备的宠物
		if (Client->Pet && Client->Pet->ID > 0)
		{
			//基础属性
			peton = 1;
			PETITEM* PetItem = NULL;
			PetItem = GetPetItem(Client->PetID);
			if (PetItem)
			{
				curskill1 = PetItem->SkillID1;
				curskillv1 = PetItem->SkillValue1;
				curskill2 = PetItem->SkillID2;
				curskillv2 = PetItem->SkillValue2;
				curskill3 = PetItem->SkillID3;
				curskillv3 = PetItem->SkillValue3;
			}
			{
				switch (curskill1)
				{
				case 1:
					//CompensateParamPet = (float)curskillv1 / 100; //落后补偿技能生效
					CompensateParamPet = curskillv1 - 100; //落后补偿技能生效
					break;
				case 2://撞墙保护
					CrashCompensatePowerPet = curskillv1;
					break;
				case 4://引擎宝宝
					SpeedWeightPet = curskillv1 / 10;
					break;
				case 6://燃料宝宝
					JetWeightPet = curskillv1 / 10;
					break;
				case 7://点火宝宝
					SJetWeightPet = curskillv1 / 10;
					break;
				case 8:///进气宝宝
					AccuWeightPet = curskillv1 / 10;
					break;
				}
				switch (curskill2)
				{
				case 1:
					CompensateParamPet = curskillv2 - 100; //落后补偿技能生效
					//CompensateParamPet = (float)curskillv2 / 100; //落后补偿技能生效
					break;
				case 2://撞墙保护
					CrashCompensatePowerPet = curskillv2;
					break;
				case 4://引擎宝宝
					SpeedWeightPet = curskillv2 / 10;
					break;
				case 6://燃料宝宝
					JetWeightPet = curskillv2 / 10;
					break;
				case 7://点火宝宝
					SJetWeightPet = curskillv2 / 10;
					break;
				case 8:///进气宝宝
					AccuWeightPet = curskillv2 / 10;
					break;
				}

				switch (curskill3)
				{
				case 1:
					//CompensateParamPet = (float)curskillv3 / 100; //落后补偿技能生效
					CompensateParamPet = curskillv3 - 100; //落后补偿技能生效
					break;
				case 2://撞墙保护
					CrashCompensatePowerPet = curskillv3;
					break;
				case 4://引擎宝宝
					SpeedWeightPet = curskillv3 / 10;
					break;
				case 6://燃料宝宝
					JetWeightPet = curskillv3 / 10;
					break;
				case 7://点火宝宝
					SJetWeightPet = curskillv3 / 10;
					break;
				case 8:///进气宝宝
					AccuWeightPet = curskillv3 / 10;
					break;
				}
			}

			//强化属性
			std::map<UINT, UINT> PetStrengLevel = GetPetStrengLevel(Client->Pet->ID, Client->Pet->StrengLevel);
			// 使用迭代器遍历StrengLevelMap
			std::map<UINT, UINT>::iterator iter;
			for (iter = PetStrengLevel.begin(); iter != PetStrengLevel.end(); ++iter) {
				UINT key = iter->first;
				UINT value = iter->second;
				switch (key)
				{
				case 1://落后补偿
					//CompensateParamPet += (float)value / 100;
					CompensateParamPet += value;
					break;
				case 2://碰撞保护
					CrashCompensatePowerPet += value;
					break;
				case 4://引擎宝宝
					SpeedWeightPet += value / 10;
					break;
				case 6://燃料宝宝
					JetWeightPet += value / 10;
					break;
				case 7://点火宝宝
					SJetWeightPet += value / 10;
					break;
				case 8://进气宝宝
					AccuWeightPet += value / 10;
					break;
				}
			}

		}

		//查询赛车技能
		KARTSKILL* KartSkill = NULL;
		KartSkill = GetKartSkill(KartID);


		if (KartSkill)
		{
			Client->SuperN2O = KartSkill->SuperN2O1;
			skilln2o = KartSkill->SuperN2O2;
			skillhide = KartSkill->SkillHide;
			skillview = KartSkill->SkillView;
			convertspeed = KartSkill->ConvertSpeed;
			SuperN2ORate = KartSkill->SuperN2ORate;
			SuperN2OTime = KartSkill->SuperN2OTime;
		}
		if (skilln2o != 0)
			skilln2o = 1;
		if (skillhide != 0)
			skillhide = 4;
		if (skillview != 0)
			skillview = 2;
	}
	char FilePath[MAX_PATH];
	UINT PhysInfosize;
	UINT loadID = KartID;
	if (Uin > 10000)
	{
		if (Client->KartSkinID != 0)
		{
			sprintf_s(FilePath, ".\\Kart\\%d.yml", Client->KartSkinID);

			//loadID = Client->KartSkinID;
		}
		else
		{
			sprintf_s(FilePath, ".\\Kart\\%d.yml", KartID);
		}
	}
	else
	{
		sprintf_s(FilePath, ".\\Kart\\%d.yml", KartID);
	}
	//sprintf_s(FilePath, ".\\Kart\\%d.yml", KartID);
	if (!std::filesystem::exists(FilePath)) {
		sprintf_s(FilePath, ".\\Kart\\%d.yml", 10020);
	}
	YAML::Node config = YAML::LoadFile(FilePath);

	Write32(pKartPhysParam, KartID);

	YAML::Node PhysInfo = config["PhysInfo"];

	bool IsMySkinRefitEffect = false;
	auto it = MySkinRefitEffectInfoMap.find(Client->KartSkinID);
	if (it != MySkinRefitEffectInfoMap.end()) {
		//判断是否为特殊皮肤
		IsMySkinRefitEffect = true;
	}

	//PhysInfosize = PhysInfo.size() + 1;
	PhysInfosize = PhysInfo.size();
	if (Uin > 10000) {
		if (Client->KartSkinID == 127616) {
			PhysInfosize += 1;
			Client->IsRefitItem = true;
		}
		if (IsMySkinRefitEffect) {
			++PhysInfosize;
			Client->IsRefitItem = true;
		}

		if (Client->SuperN2O == 1) {
			PhysInfosize += 1;
		}
	}
	Write8(pKartPhysParam, (BYTE)PhysInfosize); //PhysInfoCount
	YAML::Node PhysInfoParam;
	for (size_t i = 0; i < PhysInfo.size(); i++)
	{
		PhysInfoParam = PhysInfo[i];
		{ //PhysInfo
			BYTE* pPhysInfo = pKartPhysParam;
			Write16(pPhysInfo, 0); //len

			int PID = PhysInfoParam["PID"].as<int>();
			int AffectID = PhysInfoParam["AffectID"].as<int>();
			int Param = PhysInfoParam["Param"].as<int>();
			int LifeTime = PhysInfoParam["LifeTime"].as<int>();



			//TODO: 应该限制最大值, 但懒得做限制, 因为不同赛车等级是不同的, 留给后人去折腾吧!
			switch (PID)//增强kart车辆部分性能
			{
			case 1: //燃料
				Param += (10 * JetWeight + JetWeightPet);
				LifeTime += (8 * JetWeight + JetWeightPet);
				break;
			case 2001: //点火
				Param += (10 * SJetWeight + SJetWeightPet);
				LifeTime += (4 * SJetWeight + SJetWeightPet);//点火时长
				break;
			case 99999: //进气
				Param += (300 * AccuWeight + AccuWeightPet);
				break;
			default:
				break;
			}



			Write32(pPhysInfo, PID);
			Write32(pPhysInfo, AffectID);
			Write32(pPhysInfo, Param);
			Write32(pPhysInfo, LifeTime);

			len = pPhysInfo - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
		PhysInfoParam.reset();
	}
	PhysInfo.reset();
	if (Uin > 10000) {
		if (Client->KartSkinID == 127616)
		{ //紫色超级氮气
			BYTE* pPhysInfo = pKartPhysParam;
			Write16(pPhysInfo, 0); //len
			Write32(pPhysInfo, 777);//玉麒麟专属超级喷 必须写入这个数据才能显示专属氮气条
			Write32(pPhysInfo, 100);
			Write32(pPhysInfo, SuperN2ORate + JetWeightPet);
			Write32(pPhysInfo, SuperN2OTime + JetWeightPet);
			len = pPhysInfo - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}

		if (IsMySkinRefitEffect) {
			//8龙皮肤专属超级喷
			const MySkinRefitEffectsInfo& MySkinRefit = it->second;

			BYTE* pPhysInfo = pKartPhysParam;
			Write16(pPhysInfo, 0); //len

			Write32(pPhysInfo, MySkinRefit.PID);
			Write32(pPhysInfo, MySkinRefit.AffectID);
			Write32(pPhysInfo, MySkinRefit.Param);
			Write32(pPhysInfo, MySkinRefit.LifeTime);

			Write8(pPhysInfo, 10);//RefitItemCount
			for (size_t i = 0; i < 10; i++) {
				Write32(pPhysInfo, MySkinRefit.RefitItem[i]);
			}

			len = pPhysInfo - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;

		}
	}

	if (Client->SuperN2O == 1) {
		BYTE* pPhysInfo = pKartPhysParam;
		Write16(pPhysInfo, 0); //len
		Write32(pPhysInfo, 34);
		Write32(pPhysInfo, 100);
		Write32(pPhysInfo, SuperN2ORate + JetWeightPet);
		Write32(pPhysInfo, SuperN2OTime + JetWeightPet);
		len = pPhysInfo - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}

	//{ //紫色超级氮气
	//	BYTE* pPhysInfo = pKartPhysParam;
	//	Write16(pPhysInfo, 0); //len
	//	Write32(pPhysInfo, 34);
	//	Write32(pPhysInfo, 100);
	//	Write32(pPhysInfo, SuperN2ORate + JetWeightPet);
	//	Write32(pPhysInfo, SuperN2OTime + JetWeightPet);
	//	len = pPhysInfo - pKartPhysParam;
	//	Set16(pKartPhysParam, (WORD)len);
	//	pKartPhysParam += len;
	//}

	//printf("落后补偿:%f", CompensateParamPet);
	YAML::Node CompensateParam = config["CompensateParam"];//落后补偿 //技能id：1
	Write8(pKartPhysParam, (BYTE)CompensateParam.size()); //CompensateParamCount
	YAML::Node CompensateParamParam;
	for (size_t i = 0; i < CompensateParam.size(); i++)
	{
		CompensateParamParam = CompensateParam[i];
		{ //CompensateParam
			BYTE* pCompensateParam = pKartPhysParam;
			Write16(pCompensateParam, 0); //len

			Write32(pCompensateParam, CompensateParamParam["Percent"].as<int>());
			Write32(pCompensateParam, CompensateParamParam["Value"].as<int>() + CompensateParamPet);

			len = pCompensateParam - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
		CompensateParamParam.reset();
	}
	CompensateParam.reset();
	{ //CollisionBoxParam
		YAML::Node CollisionBoxParam = config["CollisionBoxParam"];
		BYTE* pCollisionBoxParam = pKartPhysParam;
		Write16(pCollisionBoxParam, 0); //len

		Write32(pCollisionBoxParam, CollisionBoxParam["BoxLength"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["BoxWidth"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["BoxHeight"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["AdjustOffset"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["AdjustPickGap"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["AdjustRemoteOffset"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["AdjustBevelOffset"].as<int>());
		Write32(pCollisionBoxParam, CollisionBoxParam["ControllerOffset"].as<int>());

		len = pCollisionBoxParam - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
		CollisionBoxParam.reset();
	}

	{ //CarToCarCollisionParam
		YAML::Node CarToCarCollisionParam = config["CarToCarCollisionParam"];
		BYTE* pCarToCarCollisionParam = pKartPhysParam;
		Write16(pCarToCarCollisionParam, 0); //len

		Write32(pCarToCarCollisionParam, CarToCarCollisionParam["CollisionRadius"].as<int>());
		Write32(pCarToCarCollisionParam, CarToCarCollisionParam["CollisionLength"].as<int>());
		Write32(pCarToCarCollisionParam, CarToCarCollisionParam["CollisionWidth"].as<int>());
		Write32(pCarToCarCollisionParam, CarToCarCollisionParam["CollisionHeight"].as<int>());
		Write32(pCarToCarCollisionParam, CarToCarCollisionParam["CollisionLostCtrlTime"].as<int>());

		len = pCarToCarCollisionParam - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
		CarToCarCollisionParam.reset();
	}

	Write32(pKartPhysParam, config["BaseTurnRate"].as<int>());
	Write32(pKartPhysParam, config["VolatileTurnRate"].as<int>());
	Write32(pKartPhysParam, config["MaxTurnSpeed"].as<int>());
	Write32(pKartPhysParam, config["MinTurnSpeed"].as<int>());
	Write32(pKartPhysParam, config["MaxAccuTime"].as<int>());
	Write32(pKartPhysParam, config["BaseAccuRate"].as<int>());
	Write32(pKartPhysParam, config["MaxAffectSpeed"].as<int>());
	Write32(pKartPhysParam, config["Gravity"].as<int>() + (10 * WWeight)); //TODO: 悬挂; 未限制最大值!
	Write32(pKartPhysParam, config["AdditionalLocalZSpeed"].as<int>() - (10 * AdditionalZSpeedWeight));// 平衡
	Write32(pKartPhysParam, config["StartVec"].as<int>() + DriftVecWeight);//离合
	Write32(pKartPhysParam, config["EndVecFist"].as<int>() + DriftVecWeight);
	Write32(pKartPhysParam, config["EndVecSecon"].as<int>() + DriftVecWeight);
	Write32(pKartPhysParam, config["DirKeyForce"].as<int>());
	Write32(pKartPhysParam, config["DirKeyTwist"].as<int>());
	Write32(pKartPhysParam, config["BannerTwist"].as<int>());
	Write32(pKartPhysParam, config["BannerKeyTwist"].as<int>());
	Write32(pKartPhysParam, config["BannerVecForce"].as<int>());
	Write32(pKartPhysParam, config["BannerHeadForce"].as<int>());
	Write32(pKartPhysParam, config["SlidFricForce"].as<int>());
	Write32(pKartPhysParam, config["RollFricForce"].as<int>());
	Write32(pKartPhysParam, config["StartWec"].as<int>());
	Write32(pKartPhysParam, config["MaxWec"].as<int>());
	Write32(pKartPhysParam, config["SuaiJianTwist"].as<int>());
	Write32(pKartPhysParam, config["DirUpKeyForce"].as<int>());

	YAML::Node AccelStatus = config["AccelStatus"];
	Write8(pKartPhysParam, (BYTE)AccelStatus.size()); //AccelStatusCount
	YAML::Node AccelStatusParam;
	for (size_t i = 0; i < AccelStatus.size(); i++)
	{
		AccelStatusParam = AccelStatus[i];
		{ //m_astKartPhysParam[].m_aiAccelStatus[]
			Write32(pKartPhysParam, AccelStatusParam.as<int>());
		}
		AccelStatusParam.reset();
	}
	AccelStatus.reset();

	YAML::Node ForwardAccel = config["ForwardAccel"];
	Write8(pKartPhysParam, (BYTE)ForwardAccel.size()); //ForwardAccelNum
	YAML::Node ForwardAccelParam;
	for (size_t i = 0; i < ForwardAccel.size(); i++)
	{
		ForwardAccelParam = ForwardAccel[i];
		{ //ForwardAccel
			BYTE* pForwardAccel = pKartPhysParam;
			Write16(pForwardAccel, 0); //len

			int Key = ForwardAccelParam["Key"].as<int>();
			int Value = (ForwardAccelParam["Value"].as<int>() + SpeedWeightPet);
			if (i >= 7 && i <= 9)
			{
				//TODO: 引擎; 未限制最大值!
				Value += (35 * SpeedWeight + SpeedWeightPet);
				Value += 8 * Speed2Weight; // 推进
			}
			Write32(pForwardAccel, Key);
			Write32(pForwardAccel, Value);

			len = pForwardAccel - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
		ForwardAccelParam.reset();
	}
	ForwardAccel.reset();


	YAML::Node ForwardDecel = config["ForwardDecel"];
	Write8(pKartPhysParam, (BYTE)ForwardDecel.size()); //ForwardDecelNum
	YAML::Node ForwardDecelParam;
	for (size_t i = 0; i < ForwardDecel.size(); i++)
	{
		ForwardDecelParam = ForwardDecel[i];
		{ //ForwardDecel
			BYTE* pForwardDecel = pKartPhysParam;
			Write16(pForwardDecel, 0); //len

			Write32(pForwardDecel, ForwardDecelParam["Key"].as<int>());
			Write32(pForwardDecel, ForwardDecelParam["Value"].as<int>());

			len = pForwardDecel - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
		ForwardDecelParam.reset();
	}
	ForwardDecel.reset();

	YAML::Node BackwardAccel = config["BackwardAccel"];
	Write8(pKartPhysParam, (BYTE)BackwardAccel.size()); //BackwardAccelNum
	YAML::Node BackwardAccelParam;
	for (size_t i = 0; i < BackwardAccel.size(); i++)
	{
		BackwardAccelParam = BackwardAccel[i];
		{ //BackwardAccel
			BYTE* pBackwardAccel = pKartPhysParam;
			Write16(pBackwardAccel, 0); //len

			Write32(pBackwardAccel, BackwardAccelParam["Key"].as<int>());
			Write32(pBackwardAccel, BackwardAccelParam["Value"].as<int>());

			len = pBackwardAccel - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
		BackwardAccelParam.reset();
	}
	BackwardAccel.reset();

	YAML::Node BackwardDecel = config["BackwardDecel"];
	Write8(pKartPhysParam, (BYTE)BackwardDecel.size()); //BackwardDecelNum
	YAML::Node BackwardDecelParam;
	for (size_t i = 0; i < BackwardDecel.size(); i++)
	{
		BackwardDecelParam = BackwardDecel[i];
		{ //BackwardDecel
			BYTE* pBackwardDecel = pKartPhysParam;
			Write16(pBackwardDecel, 0); //len

			Write32(pBackwardDecel, BackwardDecelParam["Key"].as<int>());
			Write32(pBackwardDecel, BackwardDecelParam["Value"].as<int>());

			len = pBackwardDecel - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
		BackwardDecelParam.reset();
	}
	BackwardDecel.reset();


	Write32(pKartPhysParam, CrashCompensatePowerPet); //CrashCompensatePower碰撞氮气保留 保留100=100*10 加个条件有宠物才加

	Write32(pKartPhysParam, 0); //DefenseRate
	Write32(pKartPhysParam, Uin); //Uin
	Write32(pKartPhysParam, 0xFFFFFFFF); //SpeedupCardGenRate组队模式下加速卡生产速率?
	Write8(pKartPhysParam, 0xFF);
	Write8(pKartPhysParam, 0xFF);
	Write8(pKartPhysParam, 0xFF);
	//Write8(pKartPhysParam, 0b00000111);//ExtraFuncFlag额外技能标志
	Write8(pKartPhysParam, 0xF8 | skillhide | skilln2o | skillview);// skillhide | skilln2o | skillview
	//最后一个二进制位生效蓝色氮气 倒数第3位是隐身 倒数第2位其他人氮气等道具数量可视

	if (GetItemGameType(loadID) == 2)
	{
		Write8(pKartPhysParam, 1); // HasSkatePara
		for (size_t i = 0; i < 1; i++)
		{ // hasSkatePara
			BYTE* phasSkatePara = pKartPhysParam;
			Write16(phasSkatePara, 0); // len
			if (config["KartSkatePara"])
			{
				YAML::Node KartSkatePara = config["KartSkatePara"];
				Write32(phasSkatePara, KartSkatePara["TrackSpeed"].as<int>());				   // TrackSpeed //轨道墙壁速度
				Write32(phasSkatePara, KartSkatePara["SkateSpeedCoef_AirBigJet"].as<int>());   // SkateSpeedCoef_AirBigJet
				Write32(phasSkatePara, KartSkatePara["SkateSpeedCoef_Jump"].as<int>());		   // SkateSpeedCoef_Jump
				Write32(phasSkatePara, KartSkatePara["SkateSpeedCoef_AirJetPoint"].as<int>()); // SkateSpeedCoef_AirJetPoint
				Write32(phasSkatePara, KartSkatePara["SkateSpeedCoef_MaxForce"].as<int>());	   // SkateSpeedCoef_MaxForce
				Write32(phasSkatePara, KartSkatePara["OntrackAccRef"].as<int>());			   // OntrackAccRef //轨道集气
				Write32(phasSkatePara, KartSkatePara["TrackAccRef"].as<int>());				   // TrackAccRef // 墙壁
				Write32(phasSkatePara, KartSkatePara["DashSkill"].as<int>());				   // DashSkill
			}
			else
			{
				Write32(phasSkatePara, 250000); // TrackSpeed //轨道墙壁速度
				Write32(phasSkatePara, 0);		// SkateSpeedCoef_AirBigJet
				Write32(phasSkatePara, 0);		// SkateSpeedCoef_Jump
				Write32(phasSkatePara, 0);		// SkateSpeedCoef_AirJetPoint
				Write32(phasSkatePara, 0);		// SkateSpeedCoef_MaxForce
				Write32(phasSkatePara, 350000); // OntrackAccRef //轨道集气
				Write32(phasSkatePara, 350000); // TrackAccRef // 墙壁
				Write32(phasSkatePara, 100000); // DashSkill
			}

			len = phasSkatePara - pKartPhysParam;
			Set16(pKartPhysParam, (WORD)len);
			pKartPhysParam += len;
		}
	}
	else
	{
		Write8(pKartPhysParam, 0); // HasSkatePara
	}
	/*
m_astKartPhysParam[].m_hasSkatePara[].m_iTrackSpeed
m_astKartPhysParam[].m_hasSkatePara[].m_iSkateSpeedCoef_AirBigJet
m_astKartPhysParam[].m_hasSkatePara[].m_iSkateSpeedCoef_Jump
m_astKartPhysParam[].m_hasSkatePara[].m_iSkateSpeedCoef_AirJetPoint
m_astKartPhysParam[].m_hasSkatePara[].m_iSkateSpeedCoef_MaxForce
m_astKartPhysParam[].m_hasSkatePara[].m_iOntrackAccRef
m_astKartPhysParam[].m_hasSkatePara[].m_iTrackAccRef
m_astKartPhysParam[].m_hasSkatePara[].m_iDashSkill
		*/

		//m_b->bool 单字节?
		//m_sh 有符号short 2字节
		//m_i 有符号 int 4字节
		//m_uch 无符号char
	Write8(pKartPhysParam, 0); //HasBattleModeSkillInfoList
	for (size_t i = 0; i < 0; i++)
	{ //BattleModeSkillInfoList
		BYTE* pBattleModeSkillInfoList = pKartPhysParam;
		Write16(pBattleModeSkillInfoList, 0); //len

		Write8(pBattleModeSkillInfoList, 0); //SkillNum
		for (size_t i = 0; i < 0; i++)
		{ //SkillInfo
			BYTE* pSkillInfo = pBattleModeSkillInfoList;
			Write16(pSkillInfo, 0); //len

			Write16(pSkillInfo, 0); //SkillId
			Write8(pSkillInfo, 0); //SkillLevel

			len = pSkillInfo - pBattleModeSkillInfoList;
			Set16(pBattleModeSkillInfoList, (WORD)len);
			pBattleModeSkillInfoList += len;
		}

		len = pBattleModeSkillInfoList - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}

#ifndef ZingSpeed
	//Write32(pKartPhysParam, 0x000000FF); //EnergyConvert 能量转换?
	// 漂移时为隐身技能提供能量
	//0x0000FFFF漂移直接集满?
	Write8(pKartPhysParam, 0x00);
	Write8(pKartPhysParam, 0x00);
	Write8(pKartPhysParam, 0x00);
	//Write8(pKartPhysParam, 0b11111111);
	Write8(pKartPhysParam, convertspeed);//convertspeed
	Write16(pKartPhysParam, 0xFFFF); //SuperN2ORate 超级氮气生效标志? 
	Write16(pKartPhysParam, AntiCollisionWeight * LoseRate); //CollisionLoseRate 碰撞损失速率
	Write8(pKartPhysParam, 0); //NotInterruptDrift 不断漂移

	Write8(pKartPhysParam, 0); //AddWeightNum

	Write8(pKartPhysParam, 0); //WeightNum

	Write8(pKartPhysParam, 0); //HasAntiCollisionCfg 碰撞保护配置
	/*
m_astKartPhysParam[].m_astAntiCollistionCfg[].m_uchNum
m_astKartPhysParam[].m_astAntiCollistionCfg[].m_aiPosition[]
		*/
		//YAML::Node BoatParam = config["BoatParam"];
	Write8(pKartPhysParam, 1); //HasBoatParam 开船图必须写开船物理性能 否则卡死
	for (size_t i = 0; i < 1; i++)
	{ //BoatParam
		BYTE* pBoatParam = pKartPhysParam;
		Write16(pBoatParam, 0); //len

		Write32(pBoatParam, 0); //DirKeyForceParamC
		Write32(pBoatParam, 0); //DirKeyForceParamD
		Write32(pBoatParam, 0); //DTFLimitAngle
		Write32(pBoatParam, 0); //DTFRatio
		Write32(pBoatParam, 10000); //UpKeyVecForce
		Write32(pBoatParam, 2000); //UpKeyVecForceParamA
		Write32(pBoatParam, -800); //UpKeyVecForceParamB
		Write32(pBoatParam, 0); //LRKeyVecForce
		Write32(pBoatParam, 2000); //LRKeyVecForceParamA
		Write32(pBoatParam, 70); //LRKeyVecForceParamB
		Write32(pBoatParam, 1000); //VecForce
		Write32(pBoatParam, 0); //VecForceParamA
		Write32(pBoatParam, 300); //VecForceParamB
		Write32(pBoatParam, 250); //BoatShape[]
		Write32(pBoatParam, 250); //BoatShape[]
		Write32(pBoatParam, 1000); //BoatShape[]
		Write32(pBoatParam, 3100); //WecEffect
		Write32(pBoatParam, 4500); //BannerKeyTwist
		Write32(pBoatParam, 0); //BannerKeyTwistParamA
		Write32(pBoatParam, 1000); //BannerKeyTwistParamB
		Write32(pBoatParam, 5000); //SuaiJianTwist
		Write32(pBoatParam, 600); //BoatHeighRatio
		Write32(pBoatParam, 5200); //BannerTwist
		Write32(pBoatParam, 1300); //BannerTwistParamA
		Write32(pBoatParam, 4000); //DirUpKeyForce
		Write32(pBoatParam, 3500); //DirUpKeyForceParamA
		Write32(pBoatParam, 0); //DirUpKeyForceParamB
		Write32(pBoatParam, 0); //DirKeyForce
		Write32(pBoatParam, 0); //DirKeyForceParamA
		Write32(pBoatParam, 1000000); //DirKeyForceParamB
		Write32(pBoatParam, 350000); //AddNO2PerSec
		Write32(pBoatParam, 375); //BoatAirValue
		Write32(pBoatParam, 3000); //ifSlidFricForce

		len = pBoatParam - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	//YAML::Node BoatForwardAccel = config["BoatForwardAccel"];
	Write8(pKartPhysParam, 13); //BoatForwardAccelNum
	//for (size_t i = 0; i < 13; i++)
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 0);
		Write32(pBoatForwardAccel, 5100);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 5000);
		Write32(pBoatForwardAccel, 5100);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 5700);
		Write32(pBoatForwardAccel, 5600);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 9000);
		Write32(pBoatForwardAccel, 6100);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 9300);
		Write32(pBoatForwardAccel, 6600);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 11000);
		Write32(pBoatForwardAccel, 7000);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 11800);
		Write32(pBoatForwardAccel, 7100);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 15000);
		Write32(pBoatForwardAccel, 9300);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 16000);
		Write32(pBoatForwardAccel, 9300);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 20000);
		Write32(pBoatForwardAccel, 9350);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 25000);
		Write32(pBoatForwardAccel, 9300);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 30000);
		Write32(pBoatForwardAccel, 9300);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	{ //BoatForwardAccel
		BYTE* pBoatForwardAccel = pKartPhysParam;
		Write16(pBoatForwardAccel, 0); //len
		Write32(pBoatForwardAccel, 35000);
		Write32(pBoatForwardAccel, 9300);

		len = pBoatForwardAccel - pKartPhysParam;
		Set16(pKartPhysParam, (WORD)len);
		pKartPhysParam += len;
	}
	Write32(pKartPhysParam, 0); //DirUpKeyForceParamA
	Write32(pKartPhysParam, 0); //DirUpKeyForceParamB
#endif

	len = pKartPhysParam - p;
	Set16(p, len);
	p += len;
}



void WINAPI OnBegin(std::shared_ptr<RoomNode> Room)
{
	Room->dwStart = GetTickCount();
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyRaceBegin(RoomClient);
		}
	}
}

void NoticeClient(std::shared_ptr<ClientNode> Client, std::string news) {

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
		return; // 超出缓冲区，跳过这个广播
	len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();
	SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

// 计算奖励数量的函数
int CalculateReward(int baseNum, double multiplier) {
	return static_cast<int>(baseNum / 10 * multiplier);
}

std::string GetMantissa(std::shared_ptr<ClientNode> Client) {
	std::string msg = "未知";
	if (Client->Mantissa == 1) {
		msg = "尾数单";
	}
	else if (Client->Mantissa == 2) {
		msg = "尾数双";
	}
	else if (Client->Mantissa == 3) {
		msg = "尾数大";
	}
	else if (Client->Mantissa == 4) {
		msg = "尾数小";
	}
	return msg;
}

void EndRoomBottomGame(std::shared_ptr<RoomNode> Room) {

	if (!Room) return;

	if (Room->IsBottomGame == 0) return;

	//if (IsBottomGame[0] == 0) return;
	int playernum = 0;
	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++)
	{
		PlayerRaceInfo tmp;
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		ArrPlayerRaceInfo[i] = tmp;

		if (Room->Player[i])
		{
			if (Room->Player[i]->OBState && Room->IsBottomGame == 7) continue;
			//完成时间如果是0还是会作为结算成绩进行排名
			//改为-1就是未完成了
			if (Room->Player[i]->FinishTime == 0)Room->Player[i]->FinishTime = -1;
			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;

			playernum++;
		}
	}

	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool
	{
		if (a.FinishTime == b.FinishTime) {
			return  a.Uin < b.Uin;
		}
		else {
			return  a.FinishTime < b.FinishTime;
		}

	};

	//名次排序
	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);
	if (Room->IsBottomGame == 4) {
		if (playernum != 4) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
			std::string news = "上局人数不对应，输赢作废！";
			if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
				return; // 超出缓冲区，跳过这个广播
			len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();

			for (size_t i = 0; i < 4; i++)
			{
				std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
				if (Client) {
					SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
				}
			}
		}
		else {
			// 定义奖励的倍数
			const double multipliers[] = { 4.5, 3.5, 2.0 };

			for (size_t i = 0; i < 4; i++) {
				std::string news;
				int num = 0;
				bool isWin;

				// 确定是否赢和计算数量
				if (i < 3) {
					isWin = true;
					num = CalculateReward(Room->BottomGameNum, multipliers[i]);
					news = "/C00您在上局赢得道具/C01【" + GetItemName(IsBottomGame[1]) + "】/C00数量：/C01" + std::to_string(num);
				}
				else {
					isWin = false;
					num = Room->BottomGameNum;
					news = "/C00您在上局输掉道具/C01【" + GetItemName(IsBottomGame[1]) + "】/C00数量：/C01" + std::to_string(num);
				}

				// 获取客户端并发送通知
				std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
				if (Client) {
					NotifyUseItemResult3(Client, IsBottomGame[1], num, isWin);
					NoticeClient(Client, news);
				}
			}
		}

	}
	else if (Room->IsBottomGame == 6) {
		if (playernum != 6) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
			std::string news = "上局人数不对应，输赢作废！";
			if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
				return; // 超出缓冲区，跳过这个广播
			len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();

			for (size_t i = 0; i < 6; i++)
			{
				std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
				if (Client) {
					SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
				}
			}
		}
		else {
			//定义奖励的倍数
			const double multipliers[] = { 4.0, 3.0, 2.0, 1.0, 4.0, 6.0 };

			for (size_t i = 0; i < 6; i++) {
				std::string news;
				bool isWin = (i < 4); // 0-3 赢，4-5 输
				int num = CalculateReward(Room->BottomGameNum, multipliers[i]);

				// 构造消息
				if (isWin) {
					news = "/C00您在上局赢得道具/C01【" + GetItemName(IsBottomGame[1]) + "】/C00数量：/C01" + std::to_string(num);
				}
				else {
					news = "/C00您在上局输掉道具/C01【" + GetItemName(IsBottomGame[1]) + "】/C00数量：/C01" + std::to_string(num);
				}

				// 获取客户端并发送通知
				std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
				if (Client) {
					NotifyUseItemResult3(Client, IsBottomGame[1], num, isWin);
					NoticeClient(Client, news);
				}
			}
		}
	}
	else if (Room->IsBottomGame == 7) {
		if (playernum < 5) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
			std::string news = "上局人数不对应，对局作废！";
			if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
				return; // 超出缓冲区，跳过这个广播
			len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();

			for (size_t i = 0; i < 6; i++)
			{
				std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
				if (Client) {
					SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
				}
			}
		}
		else {
			//定义奖励的倍数
			const double multipliers[] = { 5,4, 3, 2, 1 };

			for (size_t i = 0; i < 5; i++) {
				std::string news;
				int num = multipliers[i];

				// 构造消息
				news = "/C00您在上局赢得道具/C01【" + GetItemName(106614) + "】/C00数量：/C01" + std::to_string(num);

				// 获取客户端并发送通知
				std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
				if (Client) {

					NotifyUseItemResult3(Client, 106614, num, true);
					NoticeClient(Client, news);
				}
			}
		}
	}
	else if (Room->IsBottomGame == 9) {
		int secondLastDigit = 0;
		std::string oddOrEven;
		std::string size;
		for (size_t i = 0; i < 6; i++) {
			std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
			if (Client && Client->ConnID == Room->RoomOwnerID)
			{
				int FinishTime = std::abs(Client->FinishTime); // 转为正值
				std::string FinishTimeStr = std::to_string(FinishTime); // 转换为字符串

				if (FinishTimeStr.length() >= 2) {
					char secondLastChar = FinishTimeStr[FinishTimeStr.length() - 2];
					secondLastDigit = secondLastChar - '0';
				}

				// 判断单双和大小
				oddOrEven = (secondLastDigit % 2 == 0) ? "偶数" : "奇数";
				size = (secondLastDigit >= 1 && secondLastDigit <= 5) ? "小" : "大";
				break;
			}
		}
		int RoomOwnerItemnum = 0;
		for (size_t i = 0; i < 6; i++) {
			std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
			if (Client && Client->ConnID != Room->RoomOwnerID)
			{
				// 检查玩家的 Mantissa 属性并判断输赢
				bool isWin = false; // 默认输
				if ((oddOrEven == "偶数" && Client->Mantissa == 2) ||
					(oddOrEven == "奇数" && Client->Mantissa == 1)) {
					isWin = true; // 单或双匹配
				}

				if ((size == "大" && Client->Mantissa == 3) ||
					(size == "小" && Client->Mantissa == 4)) {
					isWin = true; // 大或小匹配
				}
				std::string news;
				if (isWin) {
					RoomOwnerItemnum -= Client->MantissaNum;
					news = "/C00您在上局下注/C01" + GetMantissa(Client) + "/C00赢得道具 /C01【" + GetItemName(Mantissa[3]) + "】 /C00数量： /C01" + std::to_string(Client->MantissaNum);
				}
				else {
					RoomOwnerItemnum += Client->MantissaNum;
					news = "/C00您在上局下注/C01" + GetMantissa(Client) + "/C00输掉道具/C01【" + GetItemName(Mantissa[3]) + "】/C00数量：/C01" + std::to_string(Client->MantissaNum);
				}
				NotifyUseItemResult3(Client, Mantissa[3], Client->MantissaNum, isWin);
				Client->news = news;
				Client->MantissaNum = 0;
			}

		}
		for (size_t i = 0; i < 6; i++) {
			std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
			if (Client && Client->ConnID == Room->RoomOwnerID)
			{
				bool isWin = false; // 默认输
				std::string news;

				if (RoomOwnerItemnum > 0) {
					isWin = true;
					news = "/C00您在上局赢得道具/C01【" + GetItemName(Mantissa[3]) + "】/C00数量：/C01" + std::to_string(RoomOwnerItemnum);
				}
				else {
					RoomOwnerItemnum = RoomOwnerItemnum * -1;
					news = "/C00您在上局输掉道具/C01【" + GetItemName(Mantissa[3]) + "】/C00数量：/C01" + std::to_string(RoomOwnerItemnum);
				}
				NotifyUseItemResult3(Client, Mantissa[3], RoomOwnerItemnum, isWin);
				NoticeClient(Client, news);
				break;
			}
		}

	}
	else
	{
		//确保设置人数小于当前人数
		if (IsBottomGame[3] < playernum) {
			UINT BottomItemNum = Room->IsBottomGame == 1 ? IsBottomGame[2] : Room->BottomGameNum;
			int winners = playernum - IsBottomGame[3];
			int winnersNum = BottomItemNum * IsBottomGame[3] / winners; //每个人分配的

			{
				std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
				std::string news = "/C00您在上局赢得道具/C01【" + GetItemName(IsBottomGame[1]) + "】/C00" + "数量：/C01" + std::to_string(winnersNum);
				if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
					return; // 超出缓冲区，跳过这个广播
				len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();

				for (size_t i = 0; i < winners; i++)
				{
					std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
					if (Client) {
						NotifyUseItemResult3(Client, IsBottomGame[1], winnersNum, true);
						SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
					}

				}
			}

			{
				std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
				std::string news = "/C00您在上局输掉道具/C01【" + GetItemName(IsBottomGame[1]) + "】/C00" + "数量：/C01" + std::to_string(BottomItemNum);
				if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
					return; // 超出缓冲区，跳过这个广播
				len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();
				for (size_t i = winners; i < playernum; i++)
				{
					std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[i].Uin);
					if (Client) {
						NotifyUseItemResult3(Client, IsBottomGame[1], BottomItemNum, false);
						SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
					}
				}
			}
		}
	}
}

void EndRoomSkate(std::shared_ptr<RoomNode> Room) {
	if (!Room) return;

	if (Room->IsStake == 0) return;

	if (Stake[0] == 0) return;
	int playernum = 0;
	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++)
	{
		PlayerRaceInfo tmp;
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		ArrPlayerRaceInfo[i] = tmp;

		if (Room->Player[i])
		{
			//完成时间如果是0还是会作为结算成绩进行排名
			//改为-1就是未完成了
			if (Room->Player[i]->FinishTime == 0)Room->Player[i]->FinishTime = -1;
			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;
			playernum++;
		}
	}

	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool
	{
		if (a.FinishTime == b.FinishTime) {
			return  a.Uin < b.Uin;
		}
		else {
			return  a.FinishTime < b.FinishTime;
		}

	};

	//名次排序
	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);

	if (ArrPlayerRaceInfo[0].Uin > 10000 && ArrPlayerRaceInfo[1].Uin > 10000) {
		//winner

		std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[0].Uin);
		if (Client) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
			std::string news = "/C00您在上局赢得道具/C01【" + GetItemName(Stake[1]) + "】/C00" + "数量：/C01" + std::to_string(Client->StakeNum);
			if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
				return; // 超出缓冲区，跳过这个广播
			len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();
			NotifyUseItemResult3(Client, Stake[1], Client->StakeNum, true);
			SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		}
		//loser
		{
			std::shared_ptr<ClientNode> Client = GetClient(ArrPlayerRaceInfo[1].Uin);
			if (Client) {
				std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
				std::string news = "/C00您在上局输掉道具/C01【" + GetItemName(Stake[1]) + "】/C00" + "数量：/C01" + std::to_string(Client->StakeNum);
				if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
					return; // 超出缓冲区，跳过这个广播
				len = NotifyBroadcast(p, 65, 10, 7, news) - buf.data();
				NotifyUseItemResult3(Client, Stake[1], Client->StakeNum, false);
				SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
			}
		}
	}
}

void WINAPI OnCountDown3(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{

			RoomClient->ReadyState = 0;//准备状态设置 防止卡准备
			RoomClient->IsReady = FALSE;
			RoomClient->IsFinish = FALSE;
			NotifyGameOver(RoomClient, 0, 0, NULL);

			if (Room->ChatRoomType == 6)
			{
				//进入3d房间
				ResponseCreateRoom(RoomClient, Room, 6);
				RequestEnter3DRoom(RoomClient);
				NotifyChangeRoomOwner(RoomClient, Room);
				for (char j = 0; j < 6; j++)
				{
					std::shared_ptr<ClientNode> RoomClient2 = Room->Player[j];
					if (RoomClient2 && RoomClient2 != RoomClient)
					{
						NotifyEnterRoom(RoomClient, Room, RoomClient2, RoomClient2->SeatID);
					}
				}
			}
		}
	}
	if (Room->IsBottomGame) {
		EndRoomBottomGame(Room);
	}

	if (Room->IsStake) {
		EndRoomSkate(Room);
	}

	Room->Callback = NULL;//设置回调
}
void WINAPI OnCountDown2(std::shared_ptr<RoomNode> Room)
{
	bool first = true;
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			//NotifyRaceOver(RoomClient, Room);
			NotifyRaceOverNew(RoomClient, Room, first);
			NotifyGenGameEndBoxAward(RoomClient);
			first = false;
		}
	}
	if (Room->isSkip == 1) {
		CreateRoomTimer(Room, 0, OnCountDown3);//退出结算界面
	}
	else {
		CreateRoomTimer(Room, GameOverTime, OnCountDown3);//退出结算界面
	}

}
void WINAPI OnCountDown(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyRaceShow(RoomClient);
		}
	}


	CreateRoomTimer(Room, 3000, OnCountDown2);//进入结算界面
	//CreateRoomTimer(Room, 1000, OnCountDown3);//直接退出 不结算
}

void WINAPI OnBorderCountDown2(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			SendToClient(RoomClient, 514, (const BYTE*)"\x00\x00\x00\x00", 4, RoomClient->GameID, FE_GAMELOGIC, RoomClient->ConnID, Notify);
			LeaveBorderRoom(RoomClient, 0);
			if (RoomClient->RoomID != 0)
			{
				RoomClient->ReadyState = 0;
				std::shared_ptr<RoomNode> MatchRoom = GetRoom(RoomClient->RoomID);
				if (!MatchRoom)
				{
					continue;
				}
				if (MatchRoom->Callback)
				{
					MatchRoom->Callback = NULL;
				}
				ResponseEnterRoom(RoomClient, MatchRoom, RoomClient->SeatID, 0, 0);
				for (char j = 0; j < 6; j++)
				{
					std::shared_ptr<ClientNode> RoomClient2 = MatchRoom->Player[j];
					if (RoomClient2 && RoomClient2 != RoomClient)
					{
						NotifyEnterRoom(RoomClient, MatchRoom, RoomClient2, RoomClient2->SeatID);
					}
				}
			}
		}
	}
}

int GetRankAddScore(USHORT Rank)
{
	switch (Rank) {
	case 1:
		return GradeAddSet[0];
	case 2:
		return GradeAddSet[1];
	case 3:
		return GradeAddSet[2];
	case 4:
		return GradeAddSet[3];
	case 5:
		return GradeAddSet[4];
	case 6:
		return GradeAddSet[5];
	default:
		// 如果Rank不在1到6的范围内，可以返回一个默认值或者错误码
		return 0; // 或者其他你认为合适的值
	}
}
void RankLevel(std::shared_ptr<ClientNode> Client)
{
	//if (RankGradeLevel[Client->GradeLevel] <= 25) {//车神以下
	//	Client->GradeScore = Client->GradeScore - getMaxScorce(RankGradeLevel[Client->GradeLevel] - 1);
	//}
	if (Client->GradeLevel == 0) {//青铜
		if (Client->ChildLevel == 1) {//大段位升级
			Client->GradeLevel += 1;
			Client->ChildLevel = 3;
		}
		else {//小段位升级
			Client->ChildLevel -= 1;
		}
	}
	else if (Client->GradeLevel == 1) {//白银
		if (Client->ChildLevel == 1) {//大段位升级
			Client->GradeLevel += 1;
			Client->ChildLevel = 4;
		}
		else {//小段位升级
			Client->ChildLevel -= 1;
		}
	}
	else if (Client->GradeLevel == 2) {//黄金
		if (Client->ChildLevel == 1) {//大段位升级
			Client->GradeLevel += 1;
			Client->ChildLevel = 4;
		}
		else {//小段位升级
			Client->ChildLevel -= 1;
		}
	}
	else if (Client->GradeLevel == 3) {//铂金
		if (Client->ChildLevel == 1) {//大段位升级
			Client->GradeLevel += 1;
			Client->ChildLevel = 5;
		}
		else {//小段位升级
			Client->ChildLevel -= 1;
		}
	}
	else if (Client->GradeLevel == 4) {//钻石
		if (Client->ChildLevel == 1) {//大段位升级
			Client->GradeLevel += 1;
			Client->ChildLevel = 5;
		}
		else {//小段位升级
			Client->ChildLevel -= 1;
		}
	}
	else if (Client->GradeLevel == 5) {//星耀
		if (Client->ChildLevel == 1) {//大段位升级
			Client->GradeLevel += 1;
			Client->ChildLevel = 1;
		}
		else {//小段位升级
			Client->ChildLevel -= 1;
		}
	}
	else if (Client->GradeLevel == 6) {//车神
		Client->GradeLevel += 1;
		Client->ChildLevel = 1;
	}
}

void RankLevel2(std::shared_ptr<ClientNode> Client)
{
	if (Client->GradeLevel == 0) {//青铜
		if (Client->ChildLevel == 3) {//大段位掉落
			Client->GradeLevel = 0;
			Client->ChildLevel = 3;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) + Client->GradeScore;
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 1) {//白银
		if (Client->ChildLevel == 3) {//大段位掉落
			//Client->GradeLevel -= 1;
			Client->ChildLevel = 3;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;

		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 2) {//黄金
		if (Client->ChildLevel == 4) {//大段位掉落
			//Client->GradeLevel -= 1;
			Client->ChildLevel = 4;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;

	}
	else if (Client->GradeLevel == 3) {//铂金
		if (Client->ChildLevel == 5) {//大段位掉落
			//Client->GradeLevel -= 1;
			Client->ChildLevel = 5;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 4) {//钻石
		if (Client->ChildLevel == 5) {//大段位掉落
			//Client->GradeLevel -= 1;
			Client->ChildLevel = 5;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 5) {//星耀
		if (Client->ChildLevel == 5) {//大段位掉落
			//Client->GradeLevel -= 1;
			Client->ChildLevel = 5;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 6) {//车神
		Client->GradeLevel -= 1;
		Client->ChildLevel = 1;
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;

	}
	else if (Client->GradeLevel == 7) {//车神
		Client->GradeLevel -= 1;
		Client->ChildLevel = 1;
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;

	}
}

void RankLevel3(std::shared_ptr<ClientNode> Client)
{
	if (Client->GradeLevel == 0) {//青铜
		if (Client->ChildLevel == 3) {//大段位掉落
			Client->GradeLevel = 0;
			Client->ChildLevel = 3;
			Client->GradeScore = 0;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
			Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
		}
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) + Client->GradeScore;
		//Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 1) {//白银
		if (Client->ChildLevel == 3) {//大段位掉落
			Client->GradeLevel -= 1;
			Client->ChildLevel = 1;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;

		}
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 2) {//黄金
		if (Client->ChildLevel == 4) {//大段位掉落
			Client->GradeLevel -= 1;
			Client->ChildLevel = 1;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;

	}
	else if (Client->GradeLevel == 3) {//铂金
		if (Client->ChildLevel == 5) {//大段位掉落
			Client->GradeLevel -= 1;
			Client->ChildLevel = 1;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 4) {//钻石
		if (Client->ChildLevel == 5) {//大段位掉落
			Client->GradeLevel -= 1;
			Client->ChildLevel = 1;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 5) {//星耀
		if (Client->ChildLevel == 5) {//大段位掉落
			Client->GradeLevel -= 1;
			Client->ChildLevel = 1;
		}
		else {//小段位掉落
			Client->ChildLevel += 1;
		}
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;
	}
	else if (Client->GradeLevel == 6) {//车神
		Client->GradeLevel -= 1;
		Client->ChildLevel = 1;
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;

	}
	else if (Client->GradeLevel == 7) {//车神
		Client->GradeLevel -= 1;
		Client->ChildLevel = 1;
		Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]) - 4;

	}
}

int RankRaceFunc(std::shared_ptr<ClientNode> Client, SHORT RankAddScore, UINT Rank)
{
	if (Client->PromotionMatch == 1) {//进入晋级赛阶段

		Client->CurrentPromotionRoundNums += 1;
		if (Rank <= 3) {
			Client->CurrentPromotionWinRoundNums += 1;
			if (Client->PromotionRecordTag == 2) {
				Client->PromotionRecordTag = 1;
			}
			else {
				Client->PromotionRecordTag = Client->PromotionRecordTag * 10 + 1;
			}

		}
		else {
			Client->CurrentPromotionLoseRoundNums += 1;
			if (Client->PromotionRecordTag == 2) {
				Client->PromotionRecordTag = 0;
			}
			else {
				Client->PromotionRecordTag = Client->PromotionRecordTag * 10;
			}
		}

		if (Client->CurrentPromotionWinRoundNums == 3) {//晋级成功
			Client->GradeScore = 4;
			Client->TotalScore += 4;
			RankLevel(Client);
			//return 1;

		}
		else if (Client->CurrentPromotionLoseRoundNums == 3) {//晋级失败
			Client->GradeScore = Client->GradeScore - 4;
			/*Client->GradeScore = (Client->GradeScore >= 4) ? (Client->GradeScore - 4) : 0;
			Client->TotalScore = (Client->TotalScore >= 4) ? (Client->TotalScore - 4) : 0;*/
			Client->TotalScore = Client->GradeScore - 4;
			RankLevel2(Client);
			//return 1;

		}
		PlayerDB_SetRankedInfo(Client);
		return 1;

	}
	int RankType = 0;//0 普通 1 晋级赛
	//if (Client->GradeScore == 9999 && RankAddScore >= 0) {//判定超过分数上限
	//	return 0;
	//}
	//else if (Client->GradeLevel == 20 && Client->ChildLevel == 3 && Client->GradeScore + RankAddScore < 0) {//判断查过分数下限
	//	RankAddScore = Client->GradeScore * -1;//判断查过分数下限
	//}
	Client->GradeScore += RankAddScore;
	Client->TotalScore += RankAddScore;

	Client->TotalScore < 0 ? Client->TotalScore = 0 : Client->TotalScore = Client->TotalScore;
	Client->GradeScore < 0 ? Client->GradeScore = 0 : Client->GradeScore = Client->GradeScore;

	/*if (Client->GradeScore > 9999) {
		Client->TotalScore += Client->GradeScore - 9999;
		Client->GradeScore = 9999;
	}*/
	if (Client->GradeScore >= getMaxScorce(RankGradeLevel[Client->GradeLevel]) && RankAddScore > 0) {//升段 晋级赛
		if (Client->GradeLevel >= 6) {//车神直接升段传奇
			RankLevel(Client);
		}
		else {
			RankType = 1;
			Client->PromotionMatch = 1;
			unsigned int maxScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]);
			//Client->TotalScore = (Client->GradeScore > maxScore) ? (Client->TotalScore - (Client->GradeScore - maxScore)) : Client->TotalScore;
			Client->GradeScore = getMaxScorce(RankGradeLevel[Client->GradeLevel]);
		}

		//RankLevel(Client->RankInfos);
	}
	else if (Client->GradeScore <= 0) {//掉段
		RankLevel3(Client);
	}
	PlayerDB_SetRankedInfo(Client);
	//_beginthread(PlayerDB_SetRankedInfo,0,Client);
	return RankType;
}


std::string format_uint(unsigned int value, int width) {
	std::ostringstream oss;
	oss << std::setw(width) << std::setfill('0') << value;
	return oss.str();
}
UINT GetPromotionRecordTag(UINT PromotionRecordTag1, UINT CurrentPromotionRoundNums) {

	std::string PromotionRecordTag = format_uint(PromotionRecordTag1, CurrentPromotionRoundNums);

	switch (CurrentPromotionRoundNums)
	{
	case 1:
		if (PromotionRecordTag == "0") {
			return 0;
		}
		else if (PromotionRecordTag == "1") {
			return 1;
		}
		return 0;

	case 2:
		if (PromotionRecordTag == "00") {
			return 0;
		}
		else if (PromotionRecordTag == "10") {
			return 1;
		}
		else if (PromotionRecordTag == "01") {
			return 2;
		}
		else if (PromotionRecordTag == "11") {
			return 3;
		}
		return 0;

	case 3:
		if (PromotionRecordTag == "000") {
			return 0;
		}
		else if (PromotionRecordTag == "100") {
			return 1;
		}
		else if (PromotionRecordTag == "010") {
			return 2;
		}
		else if (PromotionRecordTag == "110") {
			return 3;
		}
		else if (PromotionRecordTag == "001") {
			return 4;
		}
		else if (PromotionRecordTag == "101") {
			return 5;
		}
		else if (PromotionRecordTag == "011") {
			return 6;
		}
		else if (PromotionRecordTag == "111") {
			return 7;
		}
		return 0;

	case 4:
		if (PromotionRecordTag == "0000") {
			return 0;
		}
		else if (PromotionRecordTag == "1000") {
			return 1;
		}
		else if (PromotionRecordTag == "0100") {
			return 2;
		}
		else if (PromotionRecordTag == "1100") {
			return 3;
		}
		else if (PromotionRecordTag == "0010") {
			return 4;
		}
		else if (PromotionRecordTag == "1010") {
			return 5;
		}
		else if (PromotionRecordTag == "0110") {
			return 6;
		}
		else if (PromotionRecordTag == "1110") {
			return 7;
		}
		else if (PromotionRecordTag == "0001") {
			return 8;
		}
		else if (PromotionRecordTag == "1001") {
			return 9;
		}
		else if (PromotionRecordTag == "0101") {
			return 10;
		}
		else if (PromotionRecordTag == "1101") {
			return 11;
		}
		else if (PromotionRecordTag == "0011") {
			return 12;
		}
		else if (PromotionRecordTag == "1011") {
			return 13;
		}
		else if (PromotionRecordTag == "0111") {
			return 14;
		}
		else if (PromotionRecordTag == "1111") {
			return 15;
		}
		return 0;

	case 5:
		if (PromotionRecordTag == "00000") {
			return 0;
		}
		else if (PromotionRecordTag == "10000") {
			return 1;
		}
		else if (PromotionRecordTag == "01000") {
			return 2;
		}
		else if (PromotionRecordTag == "11000") {
			return 3;
		}
		else if (PromotionRecordTag == "00100") {
			return 4;
		}
		else if (PromotionRecordTag == "10100") {
			return 5;
		}
		else if (PromotionRecordTag == "01100") {
			return 6;
		}
		else if (PromotionRecordTag == "11100") {
			return 7;
		}
		else if (PromotionRecordTag == "00010") {
			return 8;
		}
		else if (PromotionRecordTag == "10010") {
			return 9;
		}
		else if (PromotionRecordTag == "01010") {
			return 10;
		}
		else if (PromotionRecordTag == "11010") {
			return 11;
		}
		else if (PromotionRecordTag == "00110") {
			return 12;
		}
		else if (PromotionRecordTag == "10110") {
			return 13;
		}
		else if (PromotionRecordTag == "01110") {
			return 14;
		}
		else if (PromotionRecordTag == "11110") {
			return 15;
		}
		else if (PromotionRecordTag == "00001") {
			return 16;
		}
		else if (PromotionRecordTag == "10001") {
			return 17;
		}
		else if (PromotionRecordTag == "01001") {
			return 18;
		}
		else if (PromotionRecordTag == "11001") {
			return 19;
		}
		else if (PromotionRecordTag == "00101") {
			return 20;
		}
		else if (PromotionRecordTag == "10101") {
			return 21;
		}
		else if (PromotionRecordTag == "01101") {
			return 22;
		}
		else if (PromotionRecordTag == "11101") {
			return 23;
		}
		else if (PromotionRecordTag == "00011") {
			return 24;
		}
		else if (PromotionRecordTag == "10011") {
			return 25;
		}
		else if (PromotionRecordTag == "01011") {
			return 26;
		}
		else if (PromotionRecordTag == "11011") {
			return 27;
		}
		else if (PromotionRecordTag == "00111") {
			return 28;
		}
		else if (PromotionRecordTag == "10111") {
			return 29;
		}
		else if (PromotionRecordTag == "01111") {
			return 30;
		}
		else if (PromotionRecordTag == "11111") {
			return 31;
		}
		return 0;

	default:
		return 0;
	}


}




//UINT GetPromotionRecordTag(UINT PromotionRecordTag, UINT CurrentPromotionRoundNums)
//{
//
//	switch (CurrentPromotionRoundNums)
//	{
//	case 1:
//		if (PromotionRecordTag == 0) {
//			return 0;
//		}
//		else if (PromotionRecordTag == 1) {
//			return 1;
//		}
//		return 0;
//	case 2:
//		if (PromotionRecordTag == 00) {
//			return 0;
//		}
//		else if (PromotionRecordTag == 10) {
//			return 1;
//		}
//		else if (PromotionRecordTag == 01) {
//			return 2;
//		}
//		else if (PromotionRecordTag == 11) {
//			return 3;
//		}
//		return 0;
//	case 3:
//		if (PromotionRecordTag == 000) {
//			return 0;
//		}
//		else if (PromotionRecordTag == 100) {
//			return 1;
//		}
//		else if (PromotionRecordTag == 010) {
//			return 2;
//		}
//		else if (PromotionRecordTag == 110) {
//			return 3;
//		}
//		else if (PromotionRecordTag == 001) {
//			return 4;
//		}
//		else if (PromotionRecordTag == 101) {
//			return 5;
//		}
//		else if (PromotionRecordTag == 011) {
//			return 6;
//		}
//		else if (PromotionRecordTag == 111) {
//			return 7;
//		}
//		return 0;
//	case 4:
//		if (PromotionRecordTag == 0000) {
//			return 0;
//		}
//		else if (PromotionRecordTag == 1000) {
//			return 1;
//		}
//		else if (PromotionRecordTag == 0100) {
//			return 2;
//		}
//		else if (PromotionRecordTag == 1100) {
//			return 3;
//		}
//		else if (PromotionRecordTag == 0010) {
//			return 4;
//		}
//		else if (PromotionRecordTag == 1010) {
//			return 5;
//		}
//		else if (PromotionRecordTag == 0110) {
//			return 6;
//		}
//		else if (PromotionRecordTag == 1110) {
//			return 7;
//		}
//		else if (PromotionRecordTag == 0001) {
//			return 8;
//		}
//		else if (PromotionRecordTag == 1001) {
//			return 9;
//		}
//		else if (PromotionRecordTag == 0101) {
//			return 10;
//		}
//		else if (PromotionRecordTag == 1101) {
//			return 11;
//		}
//		else if (PromotionRecordTag == 0011) {
//			return 12;
//		}
//		else if (PromotionRecordTag == 1011) {
//			return 13;
//		}
//		else if (PromotionRecordTag == 0111) {
//			return 14;
//		}
//		else if (PromotionRecordTag == 1111) {
//			return 15;
//		}
//		return 0;
//	case 5:
//		if (PromotionRecordTag == 00000) {
//			return 0;
//		}
//		else if (PromotionRecordTag == 10000) {
//			return 1;
//		}
//		else if (PromotionRecordTag == 01000) {
//			return 2;
//		}
//		else if (PromotionRecordTag == 11000) {
//			return 3;
//		}
//		else if (PromotionRecordTag == 00100) {
//			return 4;
//		}
//		else if (PromotionRecordTag == 10100) {
//			return 5;
//		}
//		else if (PromotionRecordTag == 01100) {
//			return 6;
//		}
//		else if (PromotionRecordTag == 11100) {
//			return 7;
//		}
//		else if (PromotionRecordTag == 00010) {
//			return 8;
//		}
//		else if (PromotionRecordTag == 10010) {
//			return 9;
//		}
//		else if (PromotionRecordTag == 01010) {
//			return 10;
//		}
//		else if (PromotionRecordTag == 11010) {
//			return 11;
//		}
//		else if (PromotionRecordTag == 00110) {
//			return 12;
//		}
//		else if (PromotionRecordTag == 10110) {
//			return 13;
//		}
//		else if (PromotionRecordTag == 01110) {
//			return 14;
//		}
//		else if (PromotionRecordTag == 11110) {
//			return 15;
//		}
//		else if (PromotionRecordTag == 00001) {
//			return 16;
//		}
//		else if (PromotionRecordTag == 10001) {
//			return 17;
//		}
//		else if (PromotionRecordTag == 01001) {
//			return 18;
//		}
//		else if (PromotionRecordTag == 11001) {
//			return 19;
//		}
//		else if (PromotionRecordTag == 00101) {
//			return 20;
//		}
//		else if (PromotionRecordTag == 10101) {
//			return 21;
//		}
//		else if (PromotionRecordTag == 01101) {
//			return 22;
//		}
//		else if (PromotionRecordTag == 11101) {
//			return 23;
//		}
//		else if (PromotionRecordTag == 00011) {
//			return 24;
//		}
//		else if (PromotionRecordTag == 10011) {
//			return 25;
//		}
//		else if (PromotionRecordTag == 01011) {
//			return 26;
//		}
//		else if (PromotionRecordTag == 11011) {
//			return 27;
//		}
//		else if (PromotionRecordTag == 00111) {
//			return 28;
//		}
//		else if (PromotionRecordTag == 10111) {
//			return 29;
//		}
//		else if (PromotionRecordTag == 01111) {
//			return 30;
//		}
//		else if (PromotionRecordTag == 11111) {
//			return 31;
//		}
//		return 0;
//	default:
//		return 0;
//	}
//	return 0;
//}

void AddSuperMoney(std::shared_ptr<ClientNode> Client) {
	if (Client->FinishTime > 0 && RankAdd[0] == 1) {
		SelectClientExp(Client);
		Client->SuperMoney += RankAdd[1];
		UpdateClientBase(Client);
	}
}


void NotifyRankedRaceOver(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, bool first)
{
	AddSuperMoney(Client);

	UINT PlayerNum = 0;
	int Honor = 0;
	int SpeedCoin = 0;
	//mMySQL msql;

	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	//PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++)
	{
		PlayerRaceInfo tmp{};
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		ArrPlayerRaceInfo[i] = tmp;

		if (Room->Player[i])
		{
			//完成时间如果是0还是会作为结算成绩进行排名
			//改为-1就是未完成了
			if (Room->Player[i]->FinishTime == 0)Room->Player[i]->FinishTime = -1;

			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			//ArrPlayerRaceInfo[i].Round = room->Player[i]->Round;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;

			ArrPlayerRaceInfo[i].kartid = Room->Player[i]->KartSkinID > 0 ? Room->Player[i]->KartSkinID : Room->Player[i]->KartID;

			ArrPlayerRaceInfo[i].HighestSpeed = Room->Player[i]->HighestSpeed;
			ArrPlayerRaceInfo[i].CollisionCount = Room->Player[i]->CollisionCount;
			ArrPlayerRaceInfo[i].N2OGatherCount = Room->Player[i]->N2OGatherCount;
			ArrPlayerRaceInfo[i].DriftCount = Room->Player[i]->DriftCount;

			ArrPlayerRaceInfo[i].RankGradeLevel = RankGradeLevel[Room->Player[i]->GradeLevel];
			PlayerNum++;
		}
	}
	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool
	{
		if (a.FinishTime == b.FinishTime) {
			return  a.Uin < b.Uin;
		}
		else {
			return  a.FinishTime < b.FinishTime;
		}

	};

	//名次排序
	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);
	//std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);

	if (first) {
		InsertBattleRecord(ArrPlayerRaceInfo, Room->MapID, 4);
	}


	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	
	int result = 0;
	int TotalExp = 0;
	Write8(p, PlayerNum); //CurrentPlayerNum
	for (int i = 0; i < PlayerNum; i++)
	{ //RaceScore

		BYTE* pRaceScore = p;
		Write16(pRaceScore, 0); //len

		Write32(pRaceScore, ArrPlayerRaceInfo[i].Uin); //Uin
		Write32(pRaceScore, ArrPlayerRaceInfo[i].FinishTime); //FinTime
		Write32(pRaceScore, 0); //TP
		Write32(pRaceScore, 0); //MoneyInc		增加酷币
		Write32(pRaceScore, 0); //TotalMoney
		Write32(pRaceScore, 60 + 60 / (i + 1)); //ExpInc			增加经验

		Write32(pRaceScore, Client->Experience); //TotalExp

		Write8(pRaceScore, 10); // AwardNum
		// m_iSpecialAward[]
		//Write32(pRaceScore, 1); // m_iSpecialAward QQ网吧
		//Write32(pRaceScore, 2); // m_iSpecialAward 双倍经验
		Write32(pRaceScore, 3); // m_iSpecialAward 紫钻55
		//Write32(pRaceScore, 4); // m_iSpecialAward 好友加成
		//Write32(pRaceScore, 5); // m_iSpecialAward 装备加成
		Write32(pRaceScore, 6); // m_iSpecialAward 宠物加成
		Write32(pRaceScore, 7); // m_iSpecialAward 刷新纪录
		Write32(pRaceScore, 8); // m_iSpecialAward 金牌网吧
		//Write32(pRaceScore, 9); // m_iSpecialAward 蓝钻20
		//Write32(pRaceScore, 10); // m_iSpecialAward 雪球
		//Write32(pRaceScore, 11); // m_iSpecialAward 经验减半
		//Write32(pRaceScore, 12); // m_iSpecialAward 双倍酷比
		//Write32(pRaceScore, 13); // m_iSpecialAward 车队加成
		//Write32(pRaceScore, 14); // m_iSpecialAward 师徒加成
		//Write32(pRaceScore, 15); // m_iSpecialAward 挑战加成
		Write32(pRaceScore, 16); // m_iSpecialAward 活跃加成
		//Write32(pRaceScore, 17); // m_iSpecialAward 舞蹈加成
		//Write32(pRaceScore, 18); // m_iSpecialAward 网吧5
		//Write32(pRaceScore, 19); // m_iSpecialAward 网吧10
		//Write32(pRaceScore, 20); // m_iSpecialAward 网吧15
		//Write32(pRaceScore, 21); // m_iSpecialAward 网吧20
		//Write32(pRaceScore, 22); // m_iSpecialAward 会员20
		//Write32(pRaceScore, 23); // m_iSpecialAward 车队福利
		//Write32(pRaceScore, 24); // m_iSpecialAward 连续登录
		//Write32(pRaceScore, 26); // m_iSpecialAward 宠物经验
		Write32(pRaceScore, 26); // m_iSpecialAward 伴侣加成
		//Write32(pRaceScore, 28); // m_iSpecialAward 管家20
		//Write32(pRaceScore, 29); // m_iSpecialAward 黄钻加成
		Write32(pRaceScore, 30); // m_iSpecialAward 车神加成
		Write32(pRaceScore, 31); // m_iSpecialAward 皇族加成
		//Write32(pRaceScore, 32); // m_iSpecialAward 四倍经验
		//Write32(pRaceScore, 33); // m_iSpecialAward 精灵技能
		Write32(pRaceScore, 34); // m_iSpecialAward 车队贵族
		Write32(pRaceScore, 35); // m_iSpecialAward 情侣贵族
		//Write32(pRaceScore, 36); // m_iSpecialAward 金币加成
		//Write32(pRaceScore, 37); // m_iSpecialAward 天天升级
		//Write32(pRaceScore, 38); // m_iSpecialAward 勋章加成
		//Write32(pRaceScore, 39); // m_iSpecialAward 经验三倍
		//Write32(pRaceScore, 40); // m_iSpecialAward 经验四倍
		//Write32(pRaceScore, 41); // m_iSpecialAward 经验6倍
		//Write32(pRaceScore, 42); // m_iSpecialAward 经验8倍
		//Write32(pRaceScore, 48); // m_iSpecialAward 经验10倍
		//Write32(pRaceScore, 49); // m_iSpecialAward 经验12倍
		//Write32(pRaceScore, 50); // m_iSpecialAward 经验15倍

		Write32(pRaceScore, 0); //TeamWorkExpInc
		Write32(pRaceScore, 0); //PropPoint
		Write32(pRaceScore, 0); //PropPointAddExp
		Write32(pRaceScore, 0); //LuckyMatchPointInc
		Write32(pRaceScore, 0); //LuckyMatchPointTotal
		Write32(pRaceScore, 0); //LuckyMatchScoreInc
		Write32(pRaceScore, 0); //LuckyMatchScoreTotal
		Write32(pRaceScore, 0); //LuckMoneyInc
		Write32(pRaceScore, 0); //LuckMoneyTotal
		Write32(pRaceScore, 0); //GuildScoreInc

		Write8(pRaceScore, 0); //CrazyPropAchieveNum
		//m_aiCrazyPropAchieve[]

		Write32(pRaceScore, 0); //IncWlMatchScore
		Write32(pRaceScore, 0); //IncWlDegree
		Write8(pRaceScore, 0); //IncItemNumByWl


		Write32(pRaceScore, 0); //WlMutiplyCard
		Write32(pRaceScore, 0); //SkateCoinInc
		Write32(pRaceScore, 0); //SkateCoinTotal
		Write32(pRaceScore, 0); //SkateCoinHistoryTotal
		Write32(pRaceScore, 0); //TotalCoupons


		Write8(pRaceScore, 0); //ChallengeCheer


		Write32(pRaceScore, 0); //LoveValue
		Write32(pRaceScore, 0); //SkateCoinEmperorBonus
		Write32(pRaceScore, 0); //DetailRecordID

		Write8(pRaceScore, 0); //HasGangsterResult

		{ // EquippedActiveKartInfo
			BYTE* pEquippedActiveKartInfo = pRaceScore;
			Write16(pEquippedActiveKartInfo, 0); // len

			Write8(pEquippedActiveKartInfo, 1);	 // HaveActiveInfo
			Write32(pEquippedActiveKartInfo, Client->KartID); // KartID
			Write32(pEquippedActiveKartInfo, 10); // ActiveLevel

			len = pEquippedActiveKartInfo - pRaceScore;
			Set16(pRaceScore, (WORD)len);
			pRaceScore += len;
		}


		Write8(pRaceScore, 0); //HasWeRelayRaceOverInfo


		Write8(pRaceScore, 0); //HaveSkatePropRaceInfo


		Write8(pRaceScore, 0); //IsNewBox


		Write8(pRaceScore, 0); //HaveArrestScoreInfo

		if (Client->TaskID == 0xFFFC && ArrPlayerRaceInfo[i].Uin == Client->Uin) {
			Write8(pRaceScore, 1); //HasRankedMatchInfo
			//for (size_t i = 0; i < 1; i++)
			{ //RankedMatchScore
				BYTE* pRankedMatchScore = pRaceScore;
				Write16(pRankedMatchScore, 0); //len

				{ //RankedMatchLastGradeInfo
					BYTE* pRankedMatchLastGradeInfo = pRankedMatchScore;
					Write16(pRankedMatchLastGradeInfo, 0); //len

					Write32(pRankedMatchLastGradeInfo, RankGradeLevel[Client->GradeLevel]); //GradeLevel
					Write32(pRankedMatchLastGradeInfo, Client->ChildLevel); //ChildLevel
					Write32(pRankedMatchLastGradeInfo, Client->GradeScore); //GradeScore
					Write32(pRankedMatchLastGradeInfo, getMaxScorce(RankGradeLevel[Client->GradeLevel])); //MaxGradeScore
					Write32(pRankedMatchLastGradeInfo, 0); //TotalScore

					len = pRankedMatchLastGradeInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				//处理排位增加分

				int RankAddScore = 0;
				if (Client->FinishTime > 0) {
					if (raceAdd == 1) {
						NotifyGenGameEndBoxAward(Client);
					}

					RankAddScore = GetRankAddScore(i + 1);
					if (Client->GuildID > 0) {
						PlayerDB_UpdateGuildListScore(Client->GuildID, GuildAdd, GuildAdd);
						PlayerDB_UpdateGuildInfoScore(Client->Uin, Client->GuildID, GuildAdd);
					}
				}
				int FinishAddScore = 0;
				int GradeProtectAddScore = 0;
				int KillAllAddScore = 0;
				/*if (Client->FinishTime > 0) {
					FinishAddScore = 1;
				}
				if (Client->GradeLevel == 20 || Client->GradeLevel == 21 || Client->GradeLevel == 22) {
					if (RankAddScore + FinishAddScore < 0) {
						GradeProtectAddScore -= RankAddScore + FinishAddScore;
					}
				}*/
				int RankType = RankRaceFunc(Client, RankAddScore + FinishAddScore + GradeProtectAddScore + KillAllAddScore, i + 1);
				{ //RankedMatchGradeInfo
					BYTE* pRankedMatchGradeInfo = pRankedMatchScore;
					Write16(pRankedMatchGradeInfo, 0); //len

					Write32(pRankedMatchGradeInfo, RankGradeLevel[Client->GradeLevel]); //GradeLevel
					Write32(pRankedMatchGradeInfo, Client->ChildLevel); //ChildLevel
					Write32(pRankedMatchGradeInfo, Client->GradeScore); //GradeScore
					Write32(pRankedMatchGradeInfo, getMaxScorce(RankGradeLevel[Client->GradeLevel])); //MaxGradeScore
					Write32(pRankedMatchGradeInfo, 0); //TotalScore

					len = pRankedMatchGradeInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				{ //RankedMatchPromoInfo
					BYTE* pRankedMatchPromoInfo = pRankedMatchScore;
					Write16(pRankedMatchPromoInfo, 0); //len
					if (Client->CurrentPromotionRoundNums == 0) {
						Write8(pRankedMatchPromoInfo, 0); //PromotionMatch
					}
					else {
						Write8(pRankedMatchPromoInfo, RankType); //PromotionMatch
					}

					Write32(pRankedMatchPromoInfo, 5); //PromotionRoundNums
					Write32(pRankedMatchPromoInfo, 3); //PromotionSuccessRoundNums
					if (Client->CurrentPromotionWinRoundNums == 3) {
						Write32(pRankedMatchPromoInfo, 3); //PromotionAlreadySuccessRoundNums
					}
					else {
						Write32(pRankedMatchPromoInfo, 0); //PromotionAlreadySuccessRoundNums
					}


					Write32(pRankedMatchPromoInfo, GetPromotionRecordTag(Client->PromotionRecordTag, Client->CurrentPromotionRoundNums)); //PromotionRecordTag
					Write32(pRankedMatchPromoInfo, Client->CurrentPromotionRoundNums); //CurrentPromotionRoundNums
					if (Client->CurrentPromotionWinRoundNums == 3) {
						Write32(pRankedMatchPromoInfo, 0); //PromoFailedMinusScore
						Write32(pRankedMatchPromoInfo, 4); //PromoSuccAddScore
					}
					else {
						Write32(pRankedMatchPromoInfo, -4); //PromoFailedMinusScore
						//Write32(pRankedMatchPromoInfo, 12); //PromoSuccAddScore
					}

					Write8(pRankedMatchPromoInfo, 0); //PromoDirectly
					Write8(pRankedMatchPromoInfo, 0); //GradeNoNeedPromo
					Write8(pRankedMatchPromoInfo, 0); //RankedMemoryNoNeedPromo
					Write8(pRankedMatchPromoInfo, 0); //PromotionExtraScore
					Write8(pRankedMatchPromoInfo, 0); //PromtRaceFailedDefendFlag

					len = pRankedMatchPromoInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				if (Client->CurrentPromotionWinRoundNums == 3) {//晋级成功


					Client->PromotionRecordTag = 2;
					Client->PromotionMatch = 0;
					Client->CurrentPromotionRoundNums = 0;
					Client->CurrentPromotionWinRoundNums = 0;
					Client->CurrentPromotionLoseRoundNums = 0;
					RankAddScore = 0;
					FinishAddScore = 0;
					KillAllAddScore = 0;
					GradeProtectAddScore = 0;
					PlayerDB_SetRankedInfo(Client);
				}
				else if (Client->CurrentPromotionLoseRoundNums == 3) {//晋级失败

					Client->PromotionRecordTag = 2;
					Client->PromotionMatch = 0;
					Client->CurrentPromotionRoundNums = 0;
					Client->CurrentPromotionWinRoundNums = 0;
					Client->CurrentPromotionLoseRoundNums = 0;
					RankAddScore = 0;
					FinishAddScore = 0;
					KillAllAddScore = 0;
					GradeProtectAddScore = 0;
					PlayerDB_SetRankedInfo(Client);
				}
				{ //RankedMatchComputeInfo
					BYTE* pRankedMatchComputeInfo = pRankedMatchScore;
					Write16(pRankedMatchComputeInfo, 0); //len

					Write32(pRankedMatchComputeInfo, RankAddScore); //RankAddScore
					Write32(pRankedMatchComputeInfo, FinishAddScore); //FinishAddScore
					Write32(pRankedMatchComputeInfo, KillAllAddScore); //KillAllAddScore
					Write32(pRankedMatchComputeInfo, GradeProtectAddScore); //GradeProtectAddScore
					Write32(pRankedMatchComputeInfo, RankAddScore + FinishAddScore + GradeProtectAddScore + KillAllAddScore); //GradeAddScore
					Write32(pRankedMatchComputeInfo, 0); //ProtectAddScoreGradeLimit
					Write32(pRankedMatchComputeInfo, 0); //DailyExtraAddScore
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNums
					Write32(pRankedMatchComputeInfo, 0); //WeeklyAwardRankCoinNums
					Write32(pRankedMatchComputeInfo, 0); //WeeklyAwardRankCoinLimit
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByRank
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByFinish
					Write32(pRankedMatchComputeInfo, 0); //BanMapExtraAddScore
					Write32(pRankedMatchComputeInfo, 0); //TeamRankAddScore
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByTeamRank
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByTask
					Write8(pRankedMatchComputeInfo, 0); //UseDoubleScoreItem
					Write8(pRankedMatchComputeInfo, 0); //UserNotDecreaseItem
					Write8(pRankedMatchComputeInfo, 0); //FinishSeasonChallengeTaskOrNot
					Write32(pRankedMatchComputeInfo, 0); //FinishedSeasonChallengeTaskIndex
					Write32(pRankedMatchComputeInfo, 0); //SeasonChallengeTaskProtectAddScore
					Write8(pRankedMatchComputeInfo, 0); //NotDecreaseReason
					Write8(pRankedMatchComputeInfo, 0); //HaveSpeedKingInfo
					for (size_t i = 0; i < 0; i++)
					{ //RankedMatchSpeedKingInfos
						BYTE* pRankedMatchSpeedKingInfos = pRankedMatchComputeInfo;
						Write16(pRankedMatchSpeedKingInfos, 0); //len

						Write8(pRankedMatchSpeedKingInfos, 0); //WinOrNot
						Write32(pRankedMatchSpeedKingInfos, 0); //UiAwardSpeedKingEnergyItemNums
						Write32(pRankedMatchSpeedKingInfos, 0); //WeeklyAwardSpeedKingEnergyItemNums
						Write32(pRankedMatchSpeedKingInfos, 0); //WeeklyAwardSpeedKingEnergyItemLimitNums

						len = pRankedMatchSpeedKingInfos - pRankedMatchComputeInfo;
						Set16(pRankedMatchComputeInfo, (WORD)len);
						pRankedMatchComputeInfo += len;
					}
					Write8(pRankedMatchComputeInfo, 0); //HaveExtraComputeInfo
					for (size_t i = 0; i < 0; i++)
					{ //RankedMatchExtracComputeInfos
						BYTE* pRankedMatchExtracComputeInfos = pRankedMatchComputeInfo;
						Write16(pRankedMatchExtracComputeInfos, 0); //len

						Write32(pRankedMatchExtracComputeInfos, 0); //ExtraComputeType
						Write32(pRankedMatchExtracComputeInfos, 0); //AwardItemNums
						Write32(pRankedMatchExtracComputeInfos, 0); //WeekylyAwardItemNums
						Write32(pRankedMatchExtracComputeInfos, 0); //WeeklyAwardItemLimitNums
						Write8(pRankedMatchExtracComputeInfos, 0); //HaveGameLogicTask
						for (size_t i = 0; i < 0; i++)
						{ //RakedMatchGamelogicTaskInfo
							BYTE* pRakedMatchGamelogicTaskInfo = pRankedMatchExtracComputeInfos;
							Write16(pRakedMatchGamelogicTaskInfo, 0); //len

							Write32(pRakedMatchGamelogicTaskInfo, 0); //GameLogicTaskType
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskFinishCondValue
							Write8(pRakedMatchGamelogicTaskInfo, 0); //JudgeRule
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskAddGradeScore
							Write8(pRakedMatchGamelogicTaskInfo, 0); //GradeScoreRangeNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //GradeScoreRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //TaskFinishCondValueNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskFinishCondValueRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //TaskAddGradeScoreNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskAddGradeScoreRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //FailedAddGradeScoreNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //FailedAddGradeScoreRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //FinishTask
							Write32(pRakedMatchGamelogicTaskInfo, 0); //ExtraFinishTaskAddScore

							len = pRakedMatchGamelogicTaskInfo - pRankedMatchExtracComputeInfos;
							Set16(pRankedMatchExtracComputeInfos, (WORD)len);
							pRankedMatchExtracComputeInfos += len;
						}
						Write8(pRankedMatchExtracComputeInfos, 0); //Win
						Write8(pRankedMatchExtracComputeInfos, 0); //NormalPlayerOrNot
						Write8(pRankedMatchExtracComputeInfos, 0); //LeadRolePlayerNums

						len = pRankedMatchExtracComputeInfos - pRankedMatchComputeInfo;
						Set16(pRankedMatchComputeInfo, (WORD)len);
						pRankedMatchComputeInfo += len;
					}
					Write32(pRankedMatchComputeInfo, 0); //SeasonNewMapAddScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeBaseScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeContiWinExtrScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeRegretDefeatScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeRespectableOpponentScore
					Write8(pRankedMatchComputeInfo, 0); //SpeedGodChallengeAddScore
					Write8(pRankedMatchComputeInfo, 0); //SpeedGodChallengeAddCoin
					Write8(pRankedMatchComputeInfo, 0); //SpeedFlashAddFactoryCoin
					Write8(pRankedMatchComputeInfo, 0); //PinnacleBattleAddScore
					Write8(pRankedMatchComputeInfo, 0); //PromotionExtraScore
					Write32(pRankedMatchComputeInfo, 0); //SavedExtraScore
					Write32(pRankedMatchComputeInfo, 0); //DailyExtraTaskID
					Write8(pRankedMatchComputeInfo, 0); //SuitActiveAddScore
					Write8(pRankedMatchComputeInfo, 0); //SpeedGodChallengeType
					Write8(pRankedMatchComputeInfo, 0); //SuitActiveEffectID

					len = pRankedMatchComputeInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				Write8(pRankedMatchScore, 0); //HavePromotionExtraScoreInfo
				for (size_t i = 0; i < 0; i++)
				{ //PromotionExtraScoreInfo
					BYTE* pPromotionExtraScoreInfo = pRankedMatchScore;
					Write16(pPromotionExtraScoreInfo, 0); //len

					Write8(pPromotionExtraScoreInfo, 0); //PromotionMatch
					Write32(pPromotionExtraScoreInfo, 0); //PromotionRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromotionSuccessRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromotionAlreadySuccessRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromotionRecordTag
					Write32(pPromotionExtraScoreInfo, 0); //CurrentPromotionRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromoFailedMinusScore
					Write32(pPromotionExtraScoreInfo, 0); //PromoSuccAddScore
					Write8(pPromotionExtraScoreInfo, 0); //PromoDirectly
					Write8(pPromotionExtraScoreInfo, 0); //GradeNoNeedPromo
					Write8(pPromotionExtraScoreInfo, 0); //RankedMemoryNoNeedPromo
					Write8(pPromotionExtraScoreInfo, 0); //PromotionExtraScore
					Write8(pPromotionExtraScoreInfo, 0); //PromtRaceFailedDefendFlag

					len = pPromotionExtraScoreInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}

				len = pRankedMatchScore - pRaceScore;
				Set16(pRaceScore, (WORD)len);
				pRaceScore += len;
			}
		}
		else {
			Write8(pRaceScore, 0); //HasRankedMatchInfo
			for (size_t i = 0; i < 0; i++)
			{ //RankedMatchScore
				BYTE* pRankedMatchScore = pRaceScore;
				Write16(pRankedMatchScore, 0); //len

				{ //RankedMatchLastGradeInfo
					BYTE* pRankedMatchLastGradeInfo = pRankedMatchScore;
					Write16(pRankedMatchLastGradeInfo, 0); //len

					Write32(pRankedMatchLastGradeInfo, 0); //GradeLevel
					Write32(pRankedMatchLastGradeInfo, 0); //ChildLevel
					Write32(pRankedMatchLastGradeInfo, 0); //GradeScore
					Write32(pRankedMatchLastGradeInfo, 0); //MaxGradeScore
					Write32(pRankedMatchLastGradeInfo, 0); //TotalScore

					len = pRankedMatchLastGradeInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				{ //RankedMatchGradeInfo
					BYTE* pRankedMatchGradeInfo = pRankedMatchScore;
					Write16(pRankedMatchGradeInfo, 0); //len

					Write32(pRankedMatchGradeInfo, 0); //GradeLevel
					Write32(pRankedMatchGradeInfo, 0); //ChildLevel
					Write32(pRankedMatchGradeInfo, 0); //GradeScore
					Write32(pRankedMatchGradeInfo, 0); //MaxGradeScore
					Write32(pRankedMatchGradeInfo, 0); //TotalScore

					len = pRankedMatchGradeInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				{ //RankedMatchPromoInfo
					BYTE* pRankedMatchPromoInfo = pRankedMatchScore;
					Write16(pRankedMatchPromoInfo, 0); //len

					Write8(pRankedMatchPromoInfo, 0); //PromotionMatch
					Write32(pRankedMatchPromoInfo, 0); //PromotionRoundNums
					Write32(pRankedMatchPromoInfo, 0); //PromotionSuccessRoundNums
					Write32(pRankedMatchPromoInfo, 0); //PromotionAlreadySuccessRoundNums
					Write32(pRankedMatchPromoInfo, 0); //PromotionRecordTag
					Write32(pRankedMatchPromoInfo, 0); //CurrentPromotionRoundNums
					Write32(pRankedMatchPromoInfo, 0); //PromoFailedMinusScore
					Write32(pRankedMatchPromoInfo, 0); //PromoSuccAddScore
					Write8(pRankedMatchPromoInfo, 0); //PromoDirectly
					Write8(pRankedMatchPromoInfo, 0); //GradeNoNeedPromo
					Write8(pRankedMatchPromoInfo, 0); //RankedMemoryNoNeedPromo
					Write8(pRankedMatchPromoInfo, 0); //PromotionExtraScore
					Write8(pRankedMatchPromoInfo, 0); //PromtRaceFailedDefendFlag

					len = pRankedMatchPromoInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				{ //RankedMatchComputeInfo
					BYTE* pRankedMatchComputeInfo = pRankedMatchScore;
					Write16(pRankedMatchComputeInfo, 0); //len

					Write32(pRankedMatchComputeInfo, 0); //RankAddScore
					Write32(pRankedMatchComputeInfo, 0); //FinishAddScore
					Write32(pRankedMatchComputeInfo, 0); //KillAllAddScore
					Write32(pRankedMatchComputeInfo, 0); //GradeProtectAddScore
					Write32(pRankedMatchComputeInfo, 0); //GradeAddScore
					Write32(pRankedMatchComputeInfo, 0); //ProtectAddScoreGradeLimit
					Write32(pRankedMatchComputeInfo, 0); //DailyExtraAddScore
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNums
					Write32(pRankedMatchComputeInfo, 0); //WeeklyAwardRankCoinNums
					Write32(pRankedMatchComputeInfo, 0); //WeeklyAwardRankCoinLimit
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByRank
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByFinish
					Write32(pRankedMatchComputeInfo, 0); //BanMapExtraAddScore
					Write32(pRankedMatchComputeInfo, 0); //TeamRankAddScore
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByTeamRank
					Write32(pRankedMatchComputeInfo, 0); //AwardRankCoinNumsByTask
					Write8(pRankedMatchComputeInfo, 0); //UseDoubleScoreItem
					Write8(pRankedMatchComputeInfo, 0); //UserNotDecreaseItem
					Write8(pRankedMatchComputeInfo, 0); //FinishSeasonChallengeTaskOrNot
					Write32(pRankedMatchComputeInfo, 0); //FinishedSeasonChallengeTaskIndex
					Write32(pRankedMatchComputeInfo, 0); //SeasonChallengeTaskProtectAddScore
					Write8(pRankedMatchComputeInfo, 0); //NotDecreaseReason
					Write8(pRankedMatchComputeInfo, 0); //HaveSpeedKingInfo
					for (size_t i = 0; i < 0; i++)
					{ //RankedMatchSpeedKingInfos
						BYTE* pRankedMatchSpeedKingInfos = pRankedMatchComputeInfo;
						Write16(pRankedMatchSpeedKingInfos, 0); //len

						Write8(pRankedMatchSpeedKingInfos, 0); //WinOrNot
						Write32(pRankedMatchSpeedKingInfos, 0); //UiAwardSpeedKingEnergyItemNums
						Write32(pRankedMatchSpeedKingInfos, 0); //WeeklyAwardSpeedKingEnergyItemNums
						Write32(pRankedMatchSpeedKingInfos, 0); //WeeklyAwardSpeedKingEnergyItemLimitNums

						len = pRankedMatchSpeedKingInfos - pRankedMatchComputeInfo;
						Set16(pRankedMatchComputeInfo, (WORD)len);
						pRankedMatchComputeInfo += len;
					}
					Write8(pRankedMatchComputeInfo, 0); //HaveExtraComputeInfo
					for (size_t i = 0; i < 0; i++)
					{ //RankedMatchExtracComputeInfos
						BYTE* pRankedMatchExtracComputeInfos = pRankedMatchComputeInfo;
						Write16(pRankedMatchExtracComputeInfos, 0); //len

						Write32(pRankedMatchExtracComputeInfos, 0); //ExtraComputeType
						Write32(pRankedMatchExtracComputeInfos, 0); //AwardItemNums
						Write32(pRankedMatchExtracComputeInfos, 0); //WeekylyAwardItemNums
						Write32(pRankedMatchExtracComputeInfos, 0); //WeeklyAwardItemLimitNums
						Write8(pRankedMatchExtracComputeInfos, 0); //HaveGameLogicTask
						for (size_t i = 0; i < 0; i++)
						{ //RakedMatchGamelogicTaskInfo
							BYTE* pRakedMatchGamelogicTaskInfo = pRankedMatchExtracComputeInfos;
							Write16(pRakedMatchGamelogicTaskInfo, 0); //len

							Write32(pRakedMatchGamelogicTaskInfo, 0); //GameLogicTaskType
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskFinishCondValue
							Write8(pRakedMatchGamelogicTaskInfo, 0); //JudgeRule
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskAddGradeScore
							Write8(pRakedMatchGamelogicTaskInfo, 0); //GradeScoreRangeNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //GradeScoreRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //TaskFinishCondValueNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskFinishCondValueRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //TaskAddGradeScoreNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //TaskAddGradeScoreRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //FailedAddGradeScoreNum
							Write32(pRakedMatchGamelogicTaskInfo, 0); //FailedAddGradeScoreRange[]
							Write8(pRakedMatchGamelogicTaskInfo, 0); //FinishTask
							Write32(pRakedMatchGamelogicTaskInfo, 0); //ExtraFinishTaskAddScore

							len = pRakedMatchGamelogicTaskInfo - pRankedMatchExtracComputeInfos;
							Set16(pRankedMatchExtracComputeInfos, (WORD)len);
							pRankedMatchExtracComputeInfos += len;
						}
						Write8(pRankedMatchExtracComputeInfos, 0); //Win
						Write8(pRankedMatchExtracComputeInfos, 0); //NormalPlayerOrNot
						Write8(pRankedMatchExtracComputeInfos, 0); //LeadRolePlayerNums

						len = pRankedMatchExtracComputeInfos - pRankedMatchComputeInfo;
						Set16(pRankedMatchComputeInfo, (WORD)len);
						pRankedMatchComputeInfo += len;
					}
					Write32(pRankedMatchComputeInfo, 0); //SeasonNewMapAddScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeBaseScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeContiWinExtrScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeRegretDefeatScore
					Write8(pRankedMatchComputeInfo, 0); //SeasonChallengeRespectableOpponentScore
					Write8(pRankedMatchComputeInfo, 0); //SpeedGodChallengeAddScore
					Write8(pRankedMatchComputeInfo, 0); //SpeedGodChallengeAddCoin
					Write8(pRankedMatchComputeInfo, 0); //SpeedFlashAddFactoryCoin
					Write8(pRankedMatchComputeInfo, 0); //PinnacleBattleAddScore
					Write8(pRankedMatchComputeInfo, 0); //PromotionExtraScore
					Write32(pRankedMatchComputeInfo, 0); //SavedExtraScore
					Write32(pRankedMatchComputeInfo, 0); //DailyExtraTaskID
					Write8(pRankedMatchComputeInfo, 0); //SuitActiveAddScore
					Write8(pRankedMatchComputeInfo, 0); //SpeedGodChallengeType
					Write8(pRankedMatchComputeInfo, 0); //SuitActiveEffectID

					len = pRankedMatchComputeInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}
				Write8(pRankedMatchScore, 0); //HavePromotionExtraScoreInfo
				for (size_t i = 0; i < 0; i++)
				{ //PromotionExtraScoreInfo
					BYTE* pPromotionExtraScoreInfo = pRankedMatchScore;
					Write16(pPromotionExtraScoreInfo, 0); //len

					Write8(pPromotionExtraScoreInfo, 0); //PromotionMatch
					Write32(pPromotionExtraScoreInfo, 0); //PromotionRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromotionSuccessRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromotionAlreadySuccessRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromotionRecordTag
					Write32(pPromotionExtraScoreInfo, 0); //CurrentPromotionRoundNums
					Write32(pPromotionExtraScoreInfo, 0); //PromoFailedMinusScore
					Write32(pPromotionExtraScoreInfo, 0); //PromoSuccAddScore
					Write8(pPromotionExtraScoreInfo, 0); //PromoDirectly
					Write8(pPromotionExtraScoreInfo, 0); //GradeNoNeedPromo
					Write8(pPromotionExtraScoreInfo, 0); //RankedMemoryNoNeedPromo
					Write8(pPromotionExtraScoreInfo, 0); //PromotionExtraScore
					Write8(pPromotionExtraScoreInfo, 0); //PromtRaceFailedDefendFlag

					len = pPromotionExtraScoreInfo - pRankedMatchScore;
					Set16(pRankedMatchScore, (WORD)len);
					pRankedMatchScore += len;
				}

				len = pRankedMatchScore - pRaceScore;
				Set16(pRaceScore, (WORD)len);
				pRaceScore += len;
			}
		}


		Write8(pRaceScore, 0); //HaveCrazyChaseScoreInfo

		Write8(pRaceScore, 0); //TeamID

		Write8(pRaceScore, 0); //HasRankedMatchArenaInfo


		Write32(pRaceScore, 0); //DistanceToEnd
		Write8(pRaceScore, 0); //ShortDistancWinPoint
		Write8(pRaceScore, 0); //Status


		Write8(pRaceScore, 0); //HaveRankedMatchSpeedKingInfo



		Write8(pRaceScore, 0); //ProfessionLicenseAwardNum


		{ //RaceResult
			BYTE* pRaceResult = pRaceScore;
			Write16(pRaceResult, 0); //len

			Write8(pRaceResult, 0); //PLExpPowerValid
			Write8(pRaceResult, 0); //PLHonorPowerValid
			Write8(pRaceResult, 0); //ExpMultiNum
			Write8(pRaceResult, 0); //HonorMultiNum

			len = pRaceResult - pRaceScore;
			Set16(pRaceScore, (WORD)len);
			pRaceScore += len;
		}

		Write8(pRaceScore, 0); //HaveRankedMatchExtraInfo


		Write8(pRaceScore, 0); //HasYuLeJiaNianHuaComputeData


		len = pRaceScore - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //WinTeamID
	Write32(p, 0); //WinType
	Write32(p, 0); //AwardTime
	Write8(p, 0); //LuckyMatchResult
	Write8(p, 0); //LuckyMatchType


	Write8(p, 1); //PlayerNumber
	{ //HideTaskResult
		BYTE* pHideTaskResult = p;
		Write16(pHideTaskResult, 0); //len

		Write32(pHideTaskResult, Client->Uin); //Uin
		Write32(pHideTaskResult, 1000); //TaskId
		Write8(pHideTaskResult, 1); //HideTaskFinishResult

		len = pHideTaskResult - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasLadderMatchResult
	/*
m_stLDMRaceResult[].m_u8PlayerNum
m_stLDMRaceResult[].m_stResult[].m_u32Uin
m_stLDMRaceResult[].m_stResult[].m_u32Score
m_stLDMRaceResult[].m_stResult[].m_i32ScoreInc
m_stLDMRaceResult[].m_stResult[].m_u32NextGradeScore
m_stLDMRaceResult[].m_stResult[].m_u8Grade
m_stLDMRaceResult[].m_stResult[].m_u8BeforeGrade
m_stLDMRaceResult[].m_stResult[].m_stRank.m_u8RankType
m_stLDMRaceResult[].m_stResult[].m_stRank.m_u8RankValue
m_stLDMRaceResult[].m_stResult[].m_stEstimateRank.m_u8RankType
m_stLDMRaceResult[].m_stResult[].m_stEstimateRank.m_u8RankValue
	*/

	Write8(p, 0); //NeedFreshTutor
	Write8(p, 0); //hasMedalGameInfo
	//m_medalGameInfo[].m_curMedal

#ifndef ZingSpeed
	Write8(p, 0); //KickOffPlayerNumber
	/*
m_stKickOffPlayer[].m_u32Uin
m_stKickOffPlayer[].m_szNickName[17]
m_stKickOffPlayer[].m_bGender
m_stKickOffPlayer[].m_bTeamID
m_stKickOffPlayer[].m_bCountry
	*/

	Write8(p, 0); //EliminatedRacerNum
	/*
m_astEliminatedRacer[].m_unUin
m_astEliminatedRacer[].m_szRacerName[17]
	*/

	Write8(p, 0); //TeamNum
	/*
m_astTeamScore[].m_ucTeamID
m_astTeamScore[].m_iTeamScore
m_astTeamScore[].m_ucTeamMinRank
m_astTeamScore[].m_ucCooperNum
m_astTeamScore[].m_ucIncCooper
m_astTeamScore[].m_ucCurCooper
	*/

	Write8(p, 0); //NPCRacerNum
	/*
m_astArrestNPCResultInfo[].m_iNpcID
m_astArrestNPCResultInfo[].m_szNPCName[17]
m_astArrestNPCResultInfo[].m_uchResultType
	*/

	Write8(p, 0); //hasGameFrameInfo
	/*
m_astGameFrameInfo[].m_uiUin
m_astGameFrameInfo[].m_iMapID
m_astGameFrameInfo[].m_ushFrameNum
m_astGameFrameInfo[].m_astFrameInfo[].m_ushCheckPointID
m_astGameFrameInfo[].m_astFrameInfo[].m_uiNum
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_unTimeElaps
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fInPosX[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fInPosY[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fInPosZ[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fStepX[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fStepY[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fStepZ[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fCarMat[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fOutPosX[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fOutPosY[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fOutPosZ[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fAccTime[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_fFrameTime[]
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_ucSetPos
m_astGameFrameInfo[].m_astFrameInfo[].m_astFrameList[].m_ucCollision
	*/

	Write8(p, 0); //hasDesperateEscapeTrophyInfo
	/*
m_astDesperateEscapeTrophyInfo[].m_uchNum
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uiUin
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uiSrcUin
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uchTrophyType
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uiID
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uiRefitCount
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uchStatus
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_uchHasRefit
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_uiUin
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iKartId
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iRefitCout
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shMaxFlags
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shWWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shSpeedWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shJetWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shSJetWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shAccuWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iShapeRefitCount
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iKartHeadRefitItemID
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iKartTailRefitItemID
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iKartFlankRefitItemID
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iKartTireRefitItemID
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iSecondRefitCount
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shSpeed2Weight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shDriftVecWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shAdditionalZSpeedWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shAntiCollisionWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shLuckyValue
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_shRefitLuckyValueMaxWeight
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iShapeSuitID
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_ucLegendSuitLevel
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iLegendSuitLevelChoice
m_astDesperateEscapeTrophyInfo[].m_astInfo[].m_astKartRefitInfo[].m_iShapeLegendSuitID
	*/

	Write8(p, 0); //HasGameStageInfo
	/*
m_stGameStageInfo[].m_u8StageIndex
m_stGameStageInfo[].m_u8TotalStageNum
	*/
#endif

	len = p - buf.data();
	SendToClient(Client, 513, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyBorderRaceOver2(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, bool first)
{

	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++) {
		PlayerRaceInfo tmp{};
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		ArrPlayerRaceInfo[i] = tmp;

		if (Room->Player[i]) {
			if (Room->Player[i]->FinishTime == 0) Room->Player[i]->FinishTime = -1;

			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;
			ArrPlayerRaceInfo[i].kartid = Room->Player[i]->KartSkinID > 0 ? Room->Player[i]->KartSkinID : Room->Player[i]->KartID;
			ArrPlayerRaceInfo[i].HighestSpeed = Room->Player[i]->HighestSpeed;
			ArrPlayerRaceInfo[i].CollisionCount = Room->Player[i]->CollisionCount;
			ArrPlayerRaceInfo[i].N2OGatherCount = Room->Player[i]->N2OGatherCount;
			ArrPlayerRaceInfo[i].DriftCount = Room->Player[i]->DriftCount;
			ArrPlayerRaceInfo[i].RankGradeLevel = RankGradeLevel[Room->Player[i]->GradeLevel];
		}
	}

	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool {
		if (a.FinishTime == b.FinishTime) {
			return a.Uin < b.Uin;
		}
		else {
			return a.FinishTime < b.FinishTime;
		}
		};

	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);

	if (first) {
		InsertBattleRecord(ArrPlayerRaceInfo, Room->MapID, 2);
	}

	std::vector<BYTE> buf(819200);
	BYTE* p = buf.data();
	size_t len;

	// 各种加成初始化
	int BabyAddValue = (Client->LoverBaby1EquipStat || Client->LoverBaby2EquipStat) ? 500 : 0;
	int EngageAddSuperMoney = 0;
	int NobleAdd = 0;
	int EmperorAdd = 0;
	int LoverVipAdd = 0;
	int GardenAddSuperMoney = 0;
	int GardenAddHonor = 0;
	int GuildVipAdd = 0;

	// 婚姻加成
	if (Client->EngageUin != 0) {
		EngageAddSuperMoney = 500;
	}

	// 皇族贵族加成
	if (Client->NoblePoint >= 30000) NobleAdd += 500;
	if (Client->NoblePoint >= 60000) NobleAdd += 500;

	// 钻皇贵族加成
	if (Client->EmperorPoint >= 30000) EmperorAdd += 1000;
	if (Client->EmperorPoint >= 60000) EmperorAdd += 1000;

	// 情侣贵族加成
	if (Client->LoverVipPoint >= 60000) LoverVipAdd = 1000;

	// 后花园系统加成
	if (Client->GardenPoint >= 60000) {
		GardenAddSuperMoney = 1000;
		GardenAddHonor = 10000;
	}

	// 车队贵族加成
	if (Client->GuildVipPoint >= 60000) GuildVipAdd = 500;

	// 计算最终的点券和荣誉加成
	int SuperMoney = Borderset[4] + BabyAddValue + EngageAddSuperMoney + NobleAdd + EmperorAdd + LoverVipAdd + GardenAddSuperMoney + GuildVipAdd;
	int Honor = Borderset[2] + GardenAddHonor;
	int HonorAdd = 0;
	int Experience = Borderset[3];

	// 玩家完成比赛的奖励加成
	if (Client->IsFinish == TRUE) {
		SelectClientExp(Client);

		// 高级道具增加荣誉加成
		for (int i = 0; i < BorderAdvancedCfg->AdvancedProp.size(); i++) {
			if (SelectSuperItem(Client, BorderAdvancedCfg->AdvancedProp[i].PropID)) {
				HonorAdd += BorderAdvancedCfg->AdvancedProp[i].Add;
			}
		}

		// 获取当前时间，确定是否应用倍数加成
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		struct tm* tm_now;
		localtime_s(tm_now, &now_time);
		int hour = tm_now->tm_hour;

		// 如果在倍数加成时间段内，应用边境倍数
		if (hour >= BorderTimes[2] && hour < BorderTimes[3] && BorderTimes[0] == 1) {
			SuperMoney *= BorderTimes[1];
			Experience *= BorderTimes[1];
			Honor *= BorderTimes[1];
		}
	/*	Honor += HonorAdd;*/

		//// 如果玩家有车队，加成的荣誉三倍
		//if (Client->GuildID > 0) {
		//	PlayerDB_UpdateGuildListScore(Client->GuildID, GuildAdd, GuildAdd);
		//	PlayerDB_UpdateGuildInfoScore(Client->Uin, Client->GuildID, GuildAdd);
		//	Honor *= 3;
		//}

		// 防止经验溢出
		Client->Experience = (Client->Experience > UINT_MAX - Experience) ? UINT_MAX : (Client->Experience + Experience);

		// 更新客户端的点券和荣誉
		Client->SuperMoney += SuperMoney;
		Client->TotalHonor += Honor;
		UpdateClientBase(Client);

		// 检查是否需要发放盲盒奖励
		if (raceAdd == 1) {
			NotifyGenGameEndBoxAward(Client);
		}
	}

	for (char i = 0; i < Room->CurrentPlayerNum; i++)
	{
		for (char i2 = 0; i2 < 6; i2++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
			if (RoomClient)
			{
				if (RoomClient->ranknumber == i && RoomClient->IsFinish == TRUE)
				{

					BYTE* pp = BorderOver;
					pp += 2;
					Write32(pp, RoomClient->Uin);
					Write32(pp, RoomClient->FinishTime); //FinTime
					pp += 4;
					Write32(pp, Experience); //ExpInc
					Write32(pp, RoomClient->Experience); //TotalExp
					Write32(pp, SuperMoney); //SuperMoneyInc
					Write32(pp, RoomClient->SuperMoney); //TotalSuperMoney
					Write32(pp, Honor); //PersonalHonor
					//Write32(pp, 5000); //ExpInc
					//Write32(pp, RoomClient->Experience); //TotalExp
					//Write32(pp, 1000 + BabyAddValue); //SuperMoneyInc
					//Write32(pp, RoomClient->SuperMoney); //TotalSuperMoney
					//Write32(pp, 10000 + BabyAddValue + HonorAdd); //PersonalHonor
					Write32(pp, RoomClient->TotalHonor); //TotalPersonalHonor
					Write32(pp, 0); //NationalHonor
					Write32(pp, 0); //TotalNationalHonor
					memcpy(p, BorderOver, 177);
					p += 177;
					break;
				}
			}
		}
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient->IsFinish == FALSE)
			{
				BYTE* pp = BorderOver;
				pp += 2;
				Write32(pp, RoomClient->Uin);
				Write32(pp, -1); //FinTime
				pp += 4;
				Write32(pp, 0); //ExpInc
				Write32(pp, RoomClient->Experience); //TotalExp
				Write32(pp, 0); //SuperMoneyInc
				Write32(pp, RoomClient->SuperMoney); //TotalSuperMoney
				Write32(pp, 0); //PersonalHonor
				Write32(pp, RoomClient->TotalHonor); //TotalPersonalHonor
				Write32(pp, 0); //NationalHonor
				Write32(pp, 0); //TotalNationalHonor
				memcpy(p, BorderOver, 177);
				p += 177;
			}
		}
	}
	memcpy(p, "\x00\x00\x42\x68\x00\x04\x00\x00\x00\x00", 10);
	p += 10;
	len = p - buf.data();
	SendToClient(Client, 542, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyBorderRaceOver1(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, bool first)
{

	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	//PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++)
	{
		PlayerRaceInfo tmp{};
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		ArrPlayerRaceInfo[i] = tmp;

		if (Room->Player[i])
		{
			//完成时间如果是0还是会作为结算成绩进行排名
			//改为-1就是未完成了
			if (Room->Player[i]->FinishTime == 0)Room->Player[i]->FinishTime = -1;

			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			//ArrPlayerRaceInfo[i].Round = room->Player[i]->Round;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;

			ArrPlayerRaceInfo[i].kartid = Room->Player[i]->KartSkinID > 0 ? Room->Player[i]->KartSkinID : Room->Player[i]->KartID;

			ArrPlayerRaceInfo[i].HighestSpeed = Room->Player[i]->HighestSpeed;
			ArrPlayerRaceInfo[i].CollisionCount = Room->Player[i]->CollisionCount;
			ArrPlayerRaceInfo[i].N2OGatherCount = Room->Player[i]->N2OGatherCount;
			ArrPlayerRaceInfo[i].DriftCount = Room->Player[i]->DriftCount;

			ArrPlayerRaceInfo[i].RankGradeLevel = RankGradeLevel[Room->Player[i]->GradeLevel];
		}
	}

	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool
		{
			if (a.FinishTime == b.FinishTime) {
				return  a.Uin < b.Uin;
			}
			else {
				return  a.FinishTime < b.FinishTime;
			}

		};

	//名次排序
	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);

	if (first) {
		InsertBattleRecord(ArrPlayerRaceInfo, Room->MapID, 2);
	}

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	//边境结算
	int BabyAddValue = 0;
	int HonorAdd = 0;
	int SuperMoney = 0;
	int Experience = 0;
	int Honor = 0;


	if (Client->LoverBaby1EquipStat || Client->LoverBaby2EquipStat)
	{
		BabyAddValue = 500;
	}


	if (Client->IsFinish == TRUE)
	{
		SelectClientExp(Client);
		//查看是否有超级道具效果
		for (int i = 0; i < BorderAdvancedCfg->AdvancedProp.size(); i++)
		{
			if (SelectSuperItem(Client, BorderAdvancedCfg->AdvancedProp[i].PropID)) {
				HonorAdd += BorderAdvancedCfg->AdvancedProp[i].Add;
			}
		}

		// 获取当前系统时间
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		struct tm* tm_now;
		localtime_s(tm_now, &now_time);

		// 获取小时
		int hour = tm_now->tm_hour;

		// 判断是否在时间内
		if (hour >= BorderTimes[2] && hour < BorderTimes[3] && BorderTimes[0] == 1) {
			SuperMoney = (Borderset[4] + BabyAddValue) * BorderTimes[1];
			Experience = Borderset[3] * BorderTimes[1];
			Honor = Borderset[2] * BorderTimes[1];
		}
		else
		{
			SuperMoney = (Borderset[4] + BabyAddValue);
			Experience = Borderset[3];
			Honor = Borderset[2];
		}
		Honor = Honor + HonorAdd;


		if (Client->GuildID > 0) {
			PlayerDB_UpdateGuildListScore(Client->GuildID, GuildAdd, GuildAdd);
			PlayerDB_UpdateGuildInfoScore(Client->Uin, Client->GuildID, GuildAdd);
			Honor = Honor * 3;
		}


		Client->Experience = (Client->Experience > UINT_MAX - Experience) ? UINT_MAX : Client->Experience + Experience;
		//Client->Experience += Experience;
		Client->SuperMoney += SuperMoney;
		Client->TotalHonor += Honor;//+10000荣誉
		UpdateClientBase(Client);

		if (raceAdd == 1) {
			NotifyGenGameEndBoxAward(Client);
		}
	}

	for (char i = 0; i < Room->CurrentPlayerNum; i++)
	{
		for (char i2 = 0; i2 < 6; i2++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
			if (RoomClient)
			{
				if (RoomClient->ranknumber == i && RoomClient->IsFinish == TRUE)
				{

					BYTE* pp = BorderOver;
					pp += 2;
					Write32(pp, RoomClient->Uin);
					Write32(pp, RoomClient->FinishTime); //FinTime
					pp += 4;
					Write32(pp, Experience); //ExpInc
					Write32(pp, RoomClient->Experience); //TotalExp
					Write32(pp, SuperMoney); //SuperMoneyInc
					Write32(pp, RoomClient->SuperMoney); //TotalSuperMoney
					Write32(pp, Honor); //PersonalHonor
					//Write32(pp, 5000); //ExpInc
					//Write32(pp, RoomClient->Experience); //TotalExp
					//Write32(pp, 1000 + BabyAddValue); //SuperMoneyInc
					//Write32(pp, RoomClient->SuperMoney); //TotalSuperMoney
					//Write32(pp, 10000 + BabyAddValue + HonorAdd); //PersonalHonor
					Write32(pp, RoomClient->TotalHonor); //TotalPersonalHonor
					Write32(pp, 0); //NationalHonor
					Write32(pp, 0); //TotalNationalHonor
					memcpy(p, BorderOver, 177);
					p += 177;
					break;
				}
			}
		}
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient->IsFinish == FALSE)
			{
				BYTE* pp = BorderOver;
				pp += 2;
				Write32(pp, RoomClient->Uin);
				Write32(pp, -1); //FinTime
				pp += 4;
				Write32(pp, 0); //ExpInc
				Write32(pp, RoomClient->Experience); //TotalExp
				Write32(pp, 0); //SuperMoneyInc
				Write32(pp, RoomClient->SuperMoney); //TotalSuperMoney
				Write32(pp, 0); //PersonalHonor
				Write32(pp, RoomClient->TotalHonor); //TotalPersonalHonor
				Write32(pp, 0); //NationalHonor
				Write32(pp, 0); //TotalNationalHonor
				memcpy(p, BorderOver, 177);
				p += 177;
			}
		}
	}
	memcpy(p, "\x00\x00\x42\x68\x00\x04\x00\x00\x00\x00", 10);
	p += 10;
	len = p - buf.data();
	SendToClient(Client, 542, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyBorderRaceOver(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, bool first)
{
	// 初始化玩家比赛信息数组，最多容纳 6 位玩家
	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++) {
		// 初始化每位玩家的比赛信息，默认为 -1（未完成）
		PlayerRaceInfo tmp{};
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		ArrPlayerRaceInfo[i] = tmp;

		// 若房间中存在该玩家，记录其比赛信息
		if (Room->Player[i]) {
			// 若完成时间为 0，则改为 -1，表示未完成
			if (Room->Player[i]->FinishTime == 0) Room->Player[i]->FinishTime = -1;

			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;
			ArrPlayerRaceInfo[i].kartid = Room->Player[i]->KartSkinID > 0 ? Room->Player[i]->KartSkinID : Room->Player[i]->KartID;
			ArrPlayerRaceInfo[i].HighestSpeed = Room->Player[i]->HighestSpeed;
			ArrPlayerRaceInfo[i].CollisionCount = Room->Player[i]->CollisionCount;
			ArrPlayerRaceInfo[i].N2OGatherCount = Room->Player[i]->N2OGatherCount;
			ArrPlayerRaceInfo[i].DriftCount = Room->Player[i]->DriftCount;
			ArrPlayerRaceInfo[i].RankGradeLevel = RankGradeLevel[Room->Player[i]->GradeLevel];
		}
	}

	// 排序函数，根据 FinishTime 排序，如果相同则按 Uin 排序
	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool {
		if (a.FinishTime == b.FinishTime) {
			return a.Uin < b.Uin;
		}
		else {
			return a.FinishTime < b.FinishTime;
		}
		};

	// 对玩家比赛信息进行排序
	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);

	// 如果是第一次结算，插入战斗记录
	if (first) {
		InsertBattleRecord(ArrPlayerRaceInfo, Room->MapID, 2);
	}

	std::vector<BYTE> buf(819200);
	BYTE* p = buf.data();
	size_t len;

	// 初始化各种加成
	int BabyAddValue = (Client->LoverBaby1EquipStat || Client->LoverBaby2EquipStat) ? 500 : 0; // 宝宝加成
	int EngageAddSuperMoney = (Client->EngageUin != 0) ? 500 : 0; // 婚姻加成
	int NobleAdd = 0;     // 皇族贵族加成
	int EmperorAdd = 0;   // 钻皇贵族加成
	int LoverVipAdd = 0;  // 情侣贵族加成
	int GardenAddSuperMoney = 0; // 后花园点券加成
	int GardenAddHonor = 0; // 后花园荣誉加成
	int GuildVipAdd = 0;   // 车队贵族加成
	int GuildAdd = 1;
	int GuildVipAdd1 = 0;//车队贵族加车队积分
	int EngageAdd = (Client->EngageUin != 0) ? 6 : 0;
	int GardenAdd = 0;

	// 皇族贵族加成
	if (Client->NoblePoint >= 30000) NobleAdd += 500;//电卷+500
	if (Client->NoblePoint >= 60000) NobleAdd += 500;//电卷+500

	// 钻皇贵族加成
	if (Client->EmperorPoint >= 30000) EmperorAdd += 1000;//电卷+1000
	if (Client->EmperorPoint >= 60000) EmperorAdd += 1000;//电卷+1000

	// 情侣贵族加成
	if (Client->LoverVipPoint >= 60000) LoverVipAdd = 1000;//电卷+1000

	if (Client->GardenPoint >= 30000)
	{
		GardenAdd += 6;
		// 后花园系统加成
		if (Client->GardenPoint >= 60000) {
			GardenAddSuperMoney = 1000;//电卷+1000
			GardenAddHonor = 10000;
		}
	}

	// 车队贵族加成
	if (Client->GuildVipPoint >= 30000)
	{
		GuildVipAdd1 += 6;
		GuildVipAdd = 500;
	}
	if (Client->GuildVipPoint >= 60000) GuildAdd = 2;

	// 计算基础奖励，之后再加上翻倍和贵族加成
	int BaseSuperMoney = Borderset[4];         // 基础点券奖励
	int BaseHonor = Borderset[2];              // 基础荣誉奖励
	int Experience = Borderset[3];             // 基础经验奖励
	int HonorAdd = 0;                          // 高级道具荣誉加成

	// 高级道具荣誉加成
	for (int i = 0; i < BorderAdvancedCfg->AdvancedProp.size(); i++) {
		if (SelectSuperItem(Client, BorderAdvancedCfg->AdvancedProp[i].PropID)) {
			HonorAdd += BorderAdvancedCfg->AdvancedProp[i].Add;
		}
	}

	// 检查是否在倍数加成时间段内，并应用倍数加成到基础奖励
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	struct tm tm_now;
	localtime_s(&tm_now, &now_time);
	int hour = tm_now.tm_hour;

	// 判断是否在倍数加成时间段内
	if (hour >= BorderTimes[2] && hour < BorderTimes[3] && BorderTimes[0] == 1) {
		BaseSuperMoney *= BorderTimes[1];  // 基础点券翻倍
		Experience *= BorderTimes[1];      // 基础经验翻倍
		BaseHonor *= BorderTimes[1];       // 基础荣誉翻倍
	}

	// 最终奖励计算：基础奖励（可能已翻倍） + 各种贵族和其他加成
	int SuperMoney = BaseSuperMoney + BabyAddValue + EngageAddSuperMoney + NobleAdd + EmperorAdd + LoverVipAdd + GardenAddSuperMoney + GuildVipAdd;
	int Honor = BaseHonor + GardenAddHonor + HonorAdd;  // 包括花园和道具的荣誉加成

	// 更新客户端的经验、点券和荣誉信息
	if (Client->IsFinish == TRUE) {
		SelectClientExp(Client); // 更新客户端经验
		Client->Experience = (Client->Experience > UINT_MAX - Experience) ? UINT_MAX : (Client->Experience + Experience); // 防止经验溢出
		Client->SuperMoney += SuperMoney;
		Client->TotalHonor += Honor;
		UpdateClientBase(Client); // 更新客户端的基础数据
		if (Client->GuildID)
		{
			UpdateGuildInfo(Client, EngageAdd + GuildVipAdd1 + GardenAdd + 6);
		}
		// 检查是否需要发放盲盒奖励
		if (raceAdd == 1) {
			NotifyGenGameEndBoxAward(Client);
		}
	}

	// 遍历房间中的每个玩家，分别更新结算信息
	for (char i = 0; i < Room->CurrentPlayerNum; i++) {
		for (char i2 = 0; i2 < 6; i2++) {
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
			if (RoomClient && RoomClient->ranknumber == i && RoomClient->IsFinish == TRUE) {
				// 生成结算数据
				BYTE* pp = BorderOver;
				pp += 2;
				Write32(pp, RoomClient->Uin);
				Write32(pp, RoomClient->FinishTime); // 完成时间
				pp += 4;
				Write32(pp, Experience);             // 增加的经验
				Write32(pp, RoomClient->Experience); // 总经验
				Write32(pp, SuperMoney);             // 增加的点券
				Write32(pp, RoomClient->SuperMoney); // 总点券
				Write32(pp, Honor);                  // 个人荣誉
				Write32(pp, RoomClient->TotalHonor); // 总荣誉
				Write32(pp, 0); // NationalHonor
				Write32(pp, 0); // TotalNationalHonor
				memcpy(p, BorderOver, 177);
				p += 177;
				break;
			}
		}
	}

	// 为未完成比赛的玩家生成结算信息
	for (char i = 0; i < 6; i++) {
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient && RoomClient->IsFinish == FALSE) {
			BYTE* pp = BorderOver;
			pp += 2;
			Write32(pp, RoomClient->Uin);
			Write32(pp, -1); // 未完成的标记
			pp += 4;
			Write32(pp, 0); // 增加的经验
			Write32(pp, RoomClient->Experience); // 总经验
			Write32(pp, 0); // 增加的点券
			Write32(pp, RoomClient->SuperMoney); // 总点券
			Write32(pp, 0); // 个人荣誉
			Write32(pp, RoomClient->TotalHonor); // 总荣誉
			Write32(pp, 0); // NationalHonor
			Write32(pp, 0); // TotalNationalHonor
			memcpy(p, BorderOver, 177);
			p += 177;
		}
	}

	// 结束标记
	memcpy(p, "\x00\x00\x42\x68\x00\x04\x00\x00\x00\x00", 10);
	p += 10;
	len = p - buf.data();
	SendToClient(Client, 542, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyRaceOverAndNpc(std::shared_ptr<ClientNode> Client)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	
	SelectClientExp(Client);
	Client->Experience = (Client->Experience > UINT_MAX - 25000) ? UINT_MAX : Client->Experience + 25000;
	//Client->Experience += 25000;//+10000经验
	Client->Money += 1000;//+1000酷比
	UpdateClientBase(Client);

	//人机没有计时，不能用FinishTime排名
	Client->NpcFinishInfo[0].FinishTime = Client->FinishTime;
	Client->NpcFinishInfo[0].IsFinish = Client->IsFinish;
	Client->NpcFinishInfo[0].ranknumber = Client->ranknumber;
	Client->NpcFinishInfo[0].NpcID = Client->Uin;
	Client->NpcFinishInfo[0].Experience = Client->Experience;
	Client->NpcFinishInfo[0].Money = Client->Money;
	char playernum = 0;
	BYTE* PlayerNum = p;
	Write8(p, 0); //CurrentPlayerNum
	for (char i = 0; i < 6; i++)
	{
		for (char i2 = 0; i2 < 6; i2++)
		{
			NpcRacerFinishInfo NpcRacer = Client->NpcFinishInfo[i2];
			if (NpcRacer.ranknumber == i && NpcRacer.IsFinish == TRUE)//完成 
			{ //RaceScore
				playernum++;
				BYTE* pRaceScore = p;
				Write16(pRaceScore, 0); //len

				Write32(pRaceScore, NpcRacer.NpcID); //Uin
				Write32(pRaceScore, NpcRacer.FinishTime); //FinTime
				Write32(pRaceScore, 0); //TP
				Write32(pRaceScore, 1000); //MoneyInc
				Write32(pRaceScore, NpcRacer.Money); //TotalMoney
				Write32(pRaceScore, 10000); //ExpInc
				Write32(pRaceScore, NpcRacer.Experience); //TotalExp

				//Write8(pRaceScore, 0); //AwardNum
				//m_iSpecialAward[]
				memcpy(pRaceScore, "\x0a\x00\x00\x00\x03\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00\x10\x00\x00\x00\x1a\x00\x00\x00\x1e\x00\x00\x00\x1f\x00\x00\x00\x22\x00\x00\x00\x23", 41);
				pRaceScore += 41;
				Write32(pRaceScore, 0); //TeamWorkExpInc
				Write32(pRaceScore, 0); //PropPoint
				Write32(pRaceScore, 0); //PropPointAddExp
				Write32(pRaceScore, 0); //LuckyMatchPointInc
				Write32(pRaceScore, 0); //LuckyMatchPointTotal
				Write32(pRaceScore, 0); //LuckyMatchScoreInc
				Write32(pRaceScore, 0); //LuckyMatchScoreTotal
				Write32(pRaceScore, 0); //LuckMoneyInc
				Write32(pRaceScore, 0); //LuckMoneyTotal
				Write32(pRaceScore, 0); //GuildScoreInc

				Write8(pRaceScore, 0); //CrazyPropAchieveNum
				//m_aiCrazyPropAchieve[]

				Write32(pRaceScore, 0); //IncWlMatchScore
				Write32(pRaceScore, 0); //IncWlDegree
				Write8(pRaceScore, 0); //IncItemNumByWl


				Write32(pRaceScore, 0); //WlMutiplyCard
				Write32(pRaceScore, 0); //SkateCoinInc
				Write32(pRaceScore, 0); //SkateCoinTotal
				Write32(pRaceScore, 0); //SkateCoinHistoryTotal
				Write32(pRaceScore, 0); //TotalCoupons


				Write8(pRaceScore, 0); //ChallengeCheer


				Write32(pRaceScore, 0); //LoveValue
				Write32(pRaceScore, 0); //SkateCoinEmperorBonus
				Write32(pRaceScore, 0); //DetailRecordID

				Write8(pRaceScore, 0); //HasGangsterResult


				{ //EquippedActiveKartInfo
					BYTE* pEquippedActiveKartInfo = pRaceScore;
					Write16(pEquippedActiveKartInfo, 0); //len

					Write8(pEquippedActiveKartInfo, 0); //HaveActiveInfo
					Write32(pEquippedActiveKartInfo, 0); //KartID
					Write32(pEquippedActiveKartInfo, 0); //ActiveLevel

					len = pEquippedActiveKartInfo - pRaceScore;
					Set16(pRaceScore, (WORD)len);
					pRaceScore += len;
				}


				Write8(pRaceScore, 0); //HasWeRelayRaceOverInfo

				Write8(pRaceScore, 0); //HaveSkatePropRaceInfo


				Write8(pRaceScore, 0); //IsNewBox


				Write8(pRaceScore, 0); //HaveArrestScoreInfo

				Write8(pRaceScore, 0); //HasRankedMatchInfo


				Write8(pRaceScore, 0); //HaveCrazyChaseScoreInfo

				Write8(pRaceScore, 0); //TeamID

				Write8(pRaceScore, 0); //HasRankedMatchArenaInfo



				Write32(pRaceScore, 0); //DistanceToEnd
				Write8(pRaceScore, 0); //ShortDistancWinPoint
				Write8(pRaceScore, 0); //Status


				Write8(pRaceScore, 0); //HaveRankedMatchSpeedKingInfo



				Write8(pRaceScore, 0); //ProfessionLicenseAwardNum 职业生涯增长


				{ //RaceResult
					BYTE* pRaceResult = pRaceScore;
					Write16(pRaceResult, 0); //len

					Write8(pRaceResult, 0); //PLExpPowerValid
					Write8(pRaceResult, 0); //PLHonorPowerValid
					Write8(pRaceResult, 0); //ExpMultiNum
					Write8(pRaceResult, 0); //HonorMultiNum

					len = pRaceResult - pRaceScore;
					Set16(pRaceScore, (WORD)len);
					pRaceScore += len;
				}

				Write8(pRaceScore, 0); //HaveRankedMatchExtraInfo


				Write8(pRaceScore, 0); //HasYuLeJiaNianHuaComputeData


				len = pRaceScore - p;
				Set16(p, (WORD)len);
				p += len;
				break;
			}
		}
	}
	for (char i = 0; i < 6; i++)
	{
		NpcRacerFinishInfo NpcRacer = Client->NpcFinishInfo[i];
		if (NpcRacer.IsFinish == FALSE)//未完成
		{ //RaceScore
			playernum++;
			BYTE* pRaceScore = p;
			Write16(pRaceScore, 0); //len

			Write32(pRaceScore, NpcRacer.NpcID); //Uin
			Write32(pRaceScore, -1); //FinTime
			Write32(pRaceScore, 0); //TP
			Write32(pRaceScore, 1000); //MoneyInc
			Write32(pRaceScore, NpcRacer.Money); //TotalMoney
			Write32(pRaceScore, 10000); //ExpInc
			Write32(pRaceScore, NpcRacer.Experience); //TotalExp

			//Write8(pRaceScore, 0); //AwardNum
			//m_iSpecialAward[]
			memcpy(pRaceScore, "\x0a\x00\x00\x00\x03\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00\x10\x00\x00\x00\x1a\x00\x00\x00\x1e\x00\x00\x00\x1f\x00\x00\x00\x22\x00\x00\x00\x23", 41);
			pRaceScore += 41;
			Write32(pRaceScore, 0); //TeamWorkExpInc
			Write32(pRaceScore, 0); //PropPoint
			Write32(pRaceScore, 0); //PropPointAddExp
			Write32(pRaceScore, 0); //LuckyMatchPointInc
			Write32(pRaceScore, 0); //LuckyMatchPointTotal
			Write32(pRaceScore, 0); //LuckyMatchScoreInc
			Write32(pRaceScore, 0); //LuckyMatchScoreTotal
			Write32(pRaceScore, 0); //LuckMoneyInc
			Write32(pRaceScore, 0); //LuckMoneyTotal
			Write32(pRaceScore, 0); //GuildScoreInc

			Write8(pRaceScore, 0); //CrazyPropAchieveNum
			//m_aiCrazyPropAchieve[]

			Write32(pRaceScore, 0); //IncWlMatchScore
			Write32(pRaceScore, 0); //IncWlDegree
			Write8(pRaceScore, 0); //IncItemNumByWl


			Write32(pRaceScore, 0); //WlMutiplyCard
			Write32(pRaceScore, 0); //SkateCoinInc
			Write32(pRaceScore, 0); //SkateCoinTotal
			Write32(pRaceScore, 0); //SkateCoinHistoryTotal
			Write32(pRaceScore, 0); //TotalCoupons


			Write8(pRaceScore, 0); //ChallengeCheer


			Write32(pRaceScore, 0); //LoveValue
			Write32(pRaceScore, 0); //SkateCoinEmperorBonus
			Write32(pRaceScore, 0); //DetailRecordID

			Write8(pRaceScore, 0); //HasGangsterResult


			{ //EquippedActiveKartInfo
				BYTE* pEquippedActiveKartInfo = pRaceScore;
				Write16(pEquippedActiveKartInfo, 0); //len

				Write8(pEquippedActiveKartInfo, 0); //HaveActiveInfo
				Write32(pEquippedActiveKartInfo, 0); //KartID
				Write32(pEquippedActiveKartInfo, 0); //ActiveLevel

				len = pEquippedActiveKartInfo - pRaceScore;
				Set16(pRaceScore, (WORD)len);
				pRaceScore += len;
			}


			Write8(pRaceScore, 0); //HasWeRelayRaceOverInfo

			Write8(pRaceScore, 0); //HaveSkatePropRaceInfo


			Write8(pRaceScore, 0); //IsNewBox


			Write8(pRaceScore, 0); //HaveArrestScoreInfo

			Write8(pRaceScore, 0); //HasRankedMatchInfo


			Write8(pRaceScore, 0); //HaveCrazyChaseScoreInfo

			Write8(pRaceScore, 0); //TeamID

			Write8(pRaceScore, 0); //HasRankedMatchArenaInfo



			Write32(pRaceScore, 0); //DistanceToEnd
			Write8(pRaceScore, 0); //ShortDistancWinPoint
			Write8(pRaceScore, 0); //Status


			Write8(pRaceScore, 0); //HaveRankedMatchSpeedKingInfo



			Write8(pRaceScore, 0); //ProfessionLicenseAwardNum 职业生涯增长


			{ //RaceResult
				BYTE* pRaceResult = pRaceScore;
				Write16(pRaceResult, 0); //len

				Write8(pRaceResult, 0); //PLExpPowerValid
				Write8(pRaceResult, 0); //PLHonorPowerValid
				Write8(pRaceResult, 0); //ExpMultiNum
				Write8(pRaceResult, 0); //HonorMultiNum

				len = pRaceResult - pRaceScore;
				Set16(pRaceScore, (WORD)len);
				pRaceScore += len;
			}

			Write8(pRaceScore, 0); //HaveRankedMatchExtraInfo


			Write8(pRaceScore, 0); //HasYuLeJiaNianHuaComputeData


			len = pRaceScore - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	Set8(PlayerNum, playernum);
	Write8(p, 0); //WinTeamID
	Write32(p, 0); //WinType
	Write32(p, 0); //AwardTime
	Write8(p, 0); //LuckyMatchResult
	Write8(p, 0); //LuckyMatchType


	Write8(p, 1); //PlayerNumber
	{ //HideTaskResult
		BYTE* pHideTaskResult = p;
		Write16(pHideTaskResult, 0); //len

		Write32(pHideTaskResult, Client->Uin); //Uin
		Write32(pHideTaskResult, 0); //TaskId
		Write8(pHideTaskResult, 0); //HideTaskFinishResult

		len = pHideTaskResult - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasLadderMatchResult
	Write8(p, 0); //NeedFreshTutor
	Write8(p, 0); //hasMedalGameInfo


#ifndef ZingSpeed
	Write8(p, 0); //KickOffPlayerNumber
	Write8(p, 0); //EliminatedRacerNum
	Write8(p, 0); //TeamNum
	Write8(p, 0); //NPCRacerNum

	Write8(p, 0); //hasGameFrameInfo
	Write8(p, 0); //hasDesperateEscapeTrophyInfo
	Write8(p, 0); //HasGameStageInfo
#endif

	len = p - buf.data();
	SendToClient(Client, 513, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);


}
void WINAPI OnBorderCountDown(std::shared_ptr<RoomNode> Room)
{
	bool first = true;
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{

			if (RoomClient->TaskID == 0xFFFD)
			{
				NotifyBorderRaceOver(RoomClient, Room, first);
				//NotifyBorderRaceOverNew(RoomClient, Room);
			}
			else if (RoomClient->TaskID == 0xFFFC)
			{
				NotifyRankedRaceOver(RoomClient, Room, first);
				NotifyRankedMatchSeasonInfo(RoomClient, RoomClient->Uin);//通知排位等级
			}
			else if (RoomClient->TaskID == 0xFFFB)
			{
				NotifyRaceOverAndNpc(RoomClient);//人机匹配
			}
			first = false;
			//NotifyGenGameEndBoxAward(RoomClient);
		}
	}
	CreateRoomTimer(Room, 16000, OnBorderCountDown2);
}

void shuffle(int* array, size_t n) {
	if (n > 1) {
		for (size_t i = n - 1; i > 0; i--) {
			size_t j = (size_t)(rand() % (i + 1));
			int temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}
}

//原始
void NotifyGameBegin(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player)
{
	if (beiginmode == 1) {
		NotifyGameBegin1(Client, Player);
		return;
	}
	else if (beiginmode == 2) {
		NotifyGameBegin2(Client, Player);
		return;
	}
	else if (beiginmode == 3) {
		NotifyGameBegin3(Client, Player);
		return;
	}
	else if (beiginmode == 4) {
		NotifyGameBegin4(Client, Player);
		return;
	}

	bool AnonymousMode509 = false;
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	if (!Player.empty())
	{
		for (char i = 0; i < 6; i++)
		{
			if (Player[i])
			{
				Players[CurrentPlayers] = Player[i];
				CurrentPlayers++;
			}
		}
	}
	else
	{
		Players[0] = Client;
		CurrentPlayers = 1;
	}
	//printf("NotifyGameBegin CurrentPlayers:%d\n", CurrentPlayers);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	//Write32(p, 8000); //NormalLoadTime
	Write32(p, 20000); //NormalLoadTime 设置大一点防止有些模型加载不出来
	Write32(p, Client->MapID); //MapID
	//MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID); //GameID
	Write32(p, (DWORD)time(nullptr)); //Seed

	Write8(p, CurrentPlayers);
	//m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	//起跑线并排的玩家
	int index[] = { 2,3,1,4,0,5 };
	srand(time(NULL));
	// 打乱数组  
	shuffle(index, CurrentPlayers);
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Set32((BYTE*)&aRaceTrackOrders[index[i]], Players[i]->Uin);
	}


	Write16(p, Client->TotalRound); //TotalMapRound
	Write32(p, 0); //PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); //Record


	Write8(p, 0); //NianShouTypeNum
	/*
m_astNianShouAwardInfo[].m_cAwardInGameType
m_astNianShouAwardInfo[].m_iActIDForClient
m_astNianShouAwardInfo[].m_cPointArrayIdx
m_astNianShouAwardInfo[].m_cCurrAwardNum
m_astNianShouAwardInfo[].m_acAwardPos[]
	*/

	Write8(p, CurrentPlayers); //SyncCarNum
	for (char i = 0; i < CurrentPlayers; i++)
	{ //SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); //len

		Write32(pSyncCar, Players[i]->Uin); //PlayerUin
		if (Players[i] == Client)
		{
			Write32(pSyncCar, Players[i]->KartID);
		}
		else
		{
			if (Players[i]->IsAnonymous == 1)
			{
				AnonymousMode509 = true;
			}
			if ((Players[i]->isRace == 1 || AnonymousMode509) && Client->OBState != 1)
			{
				Write32(pSyncCar, raceid);
			}
			else {
				if (Players[i]->KartSkinID != 0)//CurCarID 这里也要查询是否有皮肤 不然对方看不见
				{
					Write32(pSyncCar, Players[i]->KartSkinID);
				}
				else
				{
					Write32(pSyncCar, Players[i]->KartID);
				}
			}

		}

		Write8(pSyncCar, 0); //HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 1); //ReportDataFlag

	typedef struct MapCheckDivInfo // MapCheckDivInfo
	{
		USHORT BeginIdx;
		USHORT EndIdx;
	}*LPREFITITEMWEIGHTTYPE;
	const static MapCheckDivInfo MapCheckDivInfoCfg[] =
	{
		//MapCheckDivInfo 地图检查分区信息
		{1,10},
		{11,18},
		{31,35},
		{46,49},
		{61,68},
	};

	constexpr unsigned char CheckDataNum = sizeof(MapCheckDivInfoCfg) / sizeof(MapCheckDivInfoCfg[0]); //
	Write8(p, CheckDataNum); //CheckDataNum
	/*
m_astMapCheckDiv[].m_shBeginIdx
m_astMapCheckDiv[].m_shEndIdx
	*/
	for (UCHAR i = 0; i < CheckDataNum; i++)
	{ //MapCheckDiv
		BYTE* pMapCheckDiv = p;
		Write16(pMapCheckDiv, 0); //len

		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].BeginIdx); //BeginIdx
		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].EndIdx); //EndIdx

		len = pMapCheckDiv - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //P2PMode
	Write8(p, 0); //TcpFrequence



	Write8(p, 50); //MultiInfoLen
	memset(p, 0, 50);
	memcpy(p, "\x00\x00\x00\x00\x66\xFB\x62\xCE\x65\x02\x37\x02\x01\x01\x01\x66\xFB\x62\xCE\x12\x8D\xD0\xAD\x06\x83\xDF\x96\x80\xEA\x22\x67\x48\x8C\x62\xFF\xC1\x09\xBB\x78\x0F\x94\xC0\x92\x03\x6F\xF2\xF5\xD3\xF1\x16", 50);
	p += 50;

	Write8(p, FeedBackEnabled); //FeedBackEnabled 启用反馈  这里是玩家双喷 什么技巧的提示 1是开启 0是关闭


	const static USHORT SpeedSectionLowerBoundCfg[] =
	{
		// SpeedSectionLowerBoundInfo  速度区段下限
		295,306,317,328,339,350,360,375,
	};
	constexpr unsigned char SpeedSectionNum = sizeof(SpeedSectionLowerBoundCfg) / sizeof(SpeedSectionLowerBoundCfg[0]); //

	Write8(p, SpeedSectionNum); //SpeedSectionNum
	/*
m_au16SpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < SpeedSectionNum; i++) {
		Write16(p, SpeedSectionLowerBoundCfg[i]); //SpeedSectionLowerBound[]
	}

	const static USHORT NormalSpeedSectionLowerBoundCfg[] =
	{
		//NormalSpeedSectionLowerBoundInfo  正常速度区段下限
		200,220,240,260,280,300,320,340,
	};
	constexpr unsigned char NormalSpeedSectionNum = sizeof(NormalSpeedSectionLowerBoundCfg) / sizeof(NormalSpeedSectionLowerBoundCfg[0]); //

	Write8(p, NormalSpeedSectionNum); //NormalSpeedSectionNum
	/*
m_au16NormalSpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < NormalSpeedSectionNum; i++) {
		Write16(p, NormalSpeedSectionLowerBoundCfg[i]); //NormalSpeedSectionLowerBound[]
	}

	Write8(p, 0); //MemCheckInfoNum
	/*
m_astMemCheckInfo[].m_u8MemIdxNum
m_astMemCheckInfo[].m_au32MemOffset[]
m_astMemCheckInfo[].m_u8DataType
m_astMemCheckInfo[].m_uPara
	*/

	Write8(p, 4); //ExtraInfoInterval
	Write16(p, -1); //OffsetThreshold
	Write32(p, 200); //SpeedRatioThreshold1
	Write32(p, 200); //SpeedRatioThreshold2
	Write32(p, 0); //HideTaskId
	Write8(p, 0); //HideTaskType
	Write32(p, 0); //HideTaskParam1

	Write8(p, 0); //ForceReportCPNum
	//m_au16ForceReportCP[]

	Write8(p, 0); //CliReserveFlag


	Write8(p, 0); //EnableAntiDriftCheat

	//{ //AntiDriftCheatPara
	//	BYTE* pAntiDriftCheatPara = p;
	//	Write16(pAntiDriftCheatPara, 0); //len

	//	Write32(pAntiDriftCheatPara, 0); //MaxDriftHistoryTime
	//	Write32(pAntiDriftCheatPara, 0); //MinTimeInterval
	//	Write32(pAntiDriftCheatPara, 0); //MaxTimeInterval
	//	Write32(pAntiDriftCheatPara, 220); //NormalThreshold
	//	Write32(pAntiDriftCheatPara, 15); //JetThreshold
	//	Write32(pAntiDriftCheatPara, 0); //JetInterval
	//	Write32(pAntiDriftCheatPara, 0); //OneSideSlidingInterval

	//	len = pAntiDriftCheatPara - p;
	//	Set16(p, (WORD)len);
	//	p += len;
	//}

	Write8(p, 0); //HasCrashModePara
	/*
m_astCrashModePara[].m_u8SponsorSkillNum
m_astCrashModePara[].m_astSponsorSkill[].m_u8SkillID
m_astCrashModePara[].m_astSponsorSkill[].m_iPara1
m_astCrashModePara[].m_u8InitEggNum
m_astCrashModePara[].m_u8EggRefreshInterval
m_astCrashModePara[].m_u8EggRefreshNum
m_astCrashModePara[].m_u8InitN2ONum
m_astCrashModePara[].m_u8EggSourceIncNum
m_astCrashModePara[].m_u32RandSeed
m_astCrashModePara[].m_u8PropPointRatio
m_astCrashModePara[].m_u8EggPropRatio
m_astCrashModePara[].m_u8AddN2ONumOnEggLost
	*/

	Write32(p, 0); //FizzStarTaskId
	Write8(p, 0); //FizzStarTaskType
	Write32(p, 0); //FizzStarTaskParam1
	Write32(p, 0); //LDMRecordID
	//Write32(p, 1508042); //GameSeq
	Write32(p, 3925099); //GameSeq

	Write8(p, CurrentPlayers); //PlayerNums
	for (UCHAR i = 0; i < CurrentPlayers; i++)
	{ //GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); //len

		Write32(pGameBeginPlayerInfo, Players[i]->Uin); //Uin
		//Write32(pGameBeginPlayerInfo, 0x0004DAE1); //ChumCircleID
		Write32(pGameBeginPlayerInfo, 0); //ChumCircleID

		Write8(pGameBeginPlayerInfo, 0); //SkillNums
		for (size_t i = 0; i < 0; i++)
		{
			BYTE* astChumCircleSkillInfo = pGameBeginPlayerInfo;
			Write16(astChumCircleSkillInfo, 0); //len

			if (i == 0) {
				Write32(astChumCircleSkillInfo, 1);
				Write32(astChumCircleSkillInfo, 3);
			}
			else
			{
				Write32(astChumCircleSkillInfo, 3);
				Write32(astChumCircleSkillInfo, 1);
			}

			len = astChumCircleSkillInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		/*
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillID
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillLv
		*/

		Write32(pGameBeginPlayerInfo, 0); //WorldEscapeTaskID

		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); //len


			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel


			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}

		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_ushBuffID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchFeatureID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchBuffLevel
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchIsIcon
		*/

		Write32(pGameBeginPlayerInfo, 0); //StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_iParaList[]
		*/

		Write32(pGameBeginPlayerInfo, 0);//EquipSkillId

		if (Players[i]->SpeclEffect == 1) {
			Write8(pGameBeginPlayerInfo, 3);//GLSpeclEffectNum

			for (size_t i = 0; i < 3; i++)
			{//GLSpeclEffectInfo
				BYTE* pGLSpeclEffectInfo = pGameBeginPlayerInfo;
				Write16(pGLSpeclEffectInfo, 0);//len

				Write32(pGLSpeclEffectInfo, i + 1);//SpeclEffectType
				Write32(pGLSpeclEffectInfo, 0);//Para0

				len = (WORD)(pGLSpeclEffectInfo - pGameBeginPlayerInfo);
				Set16(pGameBeginPlayerInfo, len);
				pGameBeginPlayerInfo += len;
			}
		}
		else {
			Write8(pGameBeginPlayerInfo, 0);//GLSpeclEffectNum
		}



		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //PlayeCheerNums
	Write8(p, 0); //MapNum
	Write32(p, 0); //SpecialMapId
	char NPCFlag = 0;
	char border = 0;
	if (Client->TaskID == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else
	{
		NPCFlag = 1;
	}

	if ((Client->isManMachine == 0 && border == 1) || Client->real == 1) {
		NPCFlag = 0;
	}

	if (!NPCFlag)
	{
		Write8(p, 0);//NPCNum
	}
	else
	{
		Write8(p, NpcNum); //NPCNum
		for (size_t i = 0; i < NpcNum; i++)
		{
			//NPCInfo
			BYTE* pNPCInfo = p;
			Write16(pNPCInfo, 0); //len

			Write32(pNPCInfo, NpcListInfos[i].NpcID); //NpcID
			memset(pNPCInfo, 0, 17);
			memcpy(pNPCInfo, NpcListInfos[i].RoomName, 17);
			pNPCInfo += 17;
			memset(pNPCInfo, 0, 17);//GuildName[]
			pNPCInfo += 17;
			{ //NpcItemInfo
				BYTE* pNpcItemInfo = pNPCInfo;
				Write16(pNpcItemInfo, 0); //len

				Write32(pNpcItemInfo, NpcListInfos[i].RoleID); //RoleID
				Write32(pNpcItemInfo, NpcListInfos[i].HairID); //HairID
				Write32(pNpcItemInfo, NpcListInfos[i].FaceID); //FaceID
				Write32(pNpcItemInfo, NpcListInfos[i].GlassID); //GlassID
				Write32(pNpcItemInfo, NpcListInfos[i].CoatID); //CoatID
				Write32(pNpcItemInfo, NpcListInfos[i].GloveID); //GloveID
				Write32(pNpcItemInfo, NpcListInfos[i].TrousersID); //TrousersID
				Write32(pNpcItemInfo, NpcListInfos[i].PersonarID); //PersonarID
				Write32(pNpcItemInfo, NpcListInfos[i].TattooID); //TattooID
				Write32(pNpcItemInfo, NpcListInfos[i].WingID); //WingID
				Write32(pNpcItemInfo, NpcListInfos[i].DecoratorID); //DecoratorID
				Write32(pNpcItemInfo, NpcListInfos[i].KartID); //KartID
				Write32(pNpcItemInfo, NpcListInfos[i].ColorID); //ColorID
				Write32(pNpcItemInfo, NpcListInfos[i].TrackID); //TrackID
				Write32(pNpcItemInfo, NpcListInfos[i].EffectID); //EffectID
				Write32(pNpcItemInfo, NpcListInfos[i].BrandID); //BrandID
				Write32(pNpcItemInfo, NpcListInfos[i].TailID); //TailID

				len = pNpcItemInfo - pNPCInfo;
				Set16(pNPCInfo, (WORD)len);
				pNPCInfo += len;
			}

			memcpy(pNPCInfo, "\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c", 252);
			pNPCInfo += 252;
			Write16(pNPCInfo, NpcListInfos[i].MaxLevel); //NPCAILevel
			Write16(pNPCInfo, 0); //TeamID

			len = pNPCInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//int NPCKartID[5] = {127616,127365,123501 ,118729,110710 };
		//const char* NpcName[5] = {"陈伯\x0\x0","关佳鑫","胡旭\x0\x0","江章闯","张云彬"};
	//NPC配置 //NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
	//memcpy(p, "\x01\x6e\x00\x00\x00\x01\xc9\xe7\xbb\xe1\xc3\xc0\xd1\xf2\xd1\xf2\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x59\x02\x00\x00\x00\x00\x00\x01\x88\x84\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x07\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00", 366);
	//p += 366;

	}
	//npc数据
	//"\x05\x01\x6e\x00\x00\x00\x01\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x02\xd0\xa1\xe9\xd9\xd7\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe6\x6b\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x23\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x03\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x08\x00\x00\x01\x6e\x00\x00\x00\x04\xc2\xed\xc5\xa3\xb1\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\x00\x00\x00\x01\xd2\x03\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf1\xcd\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x05\x57\x6f\x6c\x76\x65\x73\x2e\x4e\x69\x6e\x67\x51\x69\x6e\x67\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xcb\xdf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf7\xc5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00";
	Write8(p, 0); //MapNum
	int* aNPCRaceTrackOrders = (int*)p;
	for (size_t i = 0; i < MAXNPCNUMINROOM; i++)
	{
		//m_aunNPCTrackOrders[]
		Write32(p, 0);
	}
	//起跑线并排的机器人
	//int npcindex[] = { 2,3,1,4,0 };
	if (Client->TaskID != 0)
	{
		for (char i = 0; i < 5; i++)
		{
			Set32((BYTE*)&aNPCRaceTrackOrders[i], i + 1);
		}
	}

	Write8(p, 0); //PlayerNum
	Write8(p, 0); //HasTowerInfo
	Write8(p, 0); //HasWeRelayGameBeginInfo

	Write8(p, 0); //ChangeCar
	//Write32(p, 0x008DB33C); //GameSeqIDHigh	12345678
	Write32(p, 0x0124b17e); //GameSeqIDHigh	12345678
	Write32(p, 2938796198); //GameSeqIDLow	87654321
	//Write32(p, 0x0390F027E); //GameSeqIDLow	87654321
	Write32(p, 0); //KubiBigCoinReplaceItem
	Write32(p, 0); //TimerChallengeJumpLevel
	Write8(p, 0); //ShadowRunDelay
	Write16(p, 0); //ShadowCatchUpContinuesTime

	Write8(p, 0); //ArrestPlayerNums
	/*
m_astArrestRoleInfo[].m_uiUin
m_astArrestRoleInfo[].m_bTeamID
	*/

	Write16(p, 0x000C); //MonitorCheckPointBegin
	Write16(p, 0x000D); //MonitorCheckPointEnd

	Write8(p, 1); //MonitorLapCnt
	Write8(p, 0); //GameType
	Write16(p, 0); //PointID  16？8
	Write16(p, 1); //BaseMode RankedRoom->BaseGameMode = 0x28;RankedRoom->SubGameMode = 1;
	Write16(p, 1); //SubMode
	Write8(p, 0); //GameType
	Write16(p, 0); //ReportPosSpan

	Write16(p, 0); //PropID
	Write32(p, 0); //PropIndex
	Write16(p, 0); //PropNum


	Write8(p, 0x65); //BaseGameModeEx 0x65
	Write8(p, 0); //ParaNum  ??
	//Write16(p, 0); //ParaNum[]

	Write8(p, 0); //MapCheckpointFileIndex

	Write8(p, 0); //HasTimerChallenge2ndGameBeginInfo
	/*
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iPlayerNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_uiUin
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iWeekHighestTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iBonusJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iHistoryMaxTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iJumpLevel
m_astTimerChallenge2ndGameBeginInfo[].m_uiBeginUin
m_astTimerChallenge2ndGameBeginInfo[].m_bReverse
	*/

	Write8(p, 0); //HasGameStageInfo
	/*
m_stGameStageInfo[].m_u8StageIndex
m_stGameStageInfo[].m_u8TotalStageNum
	*/

	Write8(p, 0); //CarCollectInfoNum
	/*
m_stCarCollectInfo[].m_ushPlayerNum
m_stCarCollectInfo[].m_stCarCollectInfo[].m_uiUin
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write16(p, 2560); //ReportAntiCollisionDataTime 报告防冲突数据时间
	Write32(p, 0); //Duration
	Write32(p, 0); //BeginCDTime
	Write32(p, 0); //PropInteval
	Write32(p, 0x00000f00); //MoveFreq

	Write8(p, 0); //HaveGameLogicTask
	/*
m_astRakedMatchGamelogicTaskInfo[].m_uiGameLogicTaskType
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskFinishCondValue
m_astRakedMatchGamelogicTaskInfo[].m_ucJudgeRule
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskAddGradeScore
m_astRakedMatchGamelogicTaskInfo[].m_uchGradeScoreRangeNum
m_astRakedMatchGamelogicTaskInfo[].m_auiGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskFinishCondValueNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskFinishCondValueRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchFailedAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiFailedAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_ucFinishTask
m_astRakedMatchGamelogicTaskInfo[].m_uiExtraFinishTaskAddScore
	*/

	Write16(p, 0); //RankedMatchBegainTipInfoType

	Write16(p, 0); //BegainTipLen  16？8

	Write8(p, 0); //BuffBum
	/*
m_stEffectBuffInfo[].m_ushBuffID
m_stEffectBuffInfo[].m_uchFeatureID
m_stEffectBuffInfo[].m_uchBuffLevel
m_stEffectBuffInfo[].m_uchIsIcon
	*/

	Write8(p, 0); //MapChallengeInfoNum
	/*
m_stMapChallengeInfo[].m_uchStarInfoNum
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uiTime
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_bType
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_ushCount
m_stMapChallengeInfo[].m_stAssessInfo.m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_ushMsgLen
m_stMapChallengeInfo[].m_iMapID
*/

	Write8(p, 0); //IsChangeCar
	if (AnonymousMode509 || (AnonymousMode == 1 && Client->TaskID == 0xFFFC)) {
		Write8(p, CurrentPlayers - 1); // AnonymousMode
		for (size_t i = 0; i < CurrentPlayers; i++)
		{ //AnonymousModeAvatarChangeInfo
			if (Player[i] == Client)
			{
				continue;
			}
			BYTE* pAnonymousModeAvatarChangeInfo = p;
			Write16(pAnonymousModeAvatarChangeInfo, 0); //len

			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Uin); //PlayerUin
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10025 : 10042); //HairID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
			Write32(pAnonymousModeAvatarChangeInfo, 0); //SuitID
			Write32(pAnonymousModeAvatarChangeInfo, raceid); //KartID
			Write8(pAnonymousModeAvatarChangeInfo, 1); //HasItemInfo
			for (size_t j = 0; j < 1; j++)
			{ //ItemInfo
				BYTE* pItemInfo = pAnonymousModeAvatarChangeInfo;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Player[i]->Gender ? 10022 : 10018); //RoleID
				Write32(pItemInfo, Player[i]->Gender ? 10025 : 10042); //HairID
				Write32(pItemInfo, 0); //FaceID
				Write32(pItemInfo, 0); //GlassID
				Write32(pItemInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
				Write32(pItemInfo, Player[i]->Gender ? 10045 : 10032); //GloveID
				Write32(pItemInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
				Write32(pItemInfo, 0); //PersonarID
				Write32(pItemInfo, 0); //TattooID
				Write32(pItemInfo, 0); //WingID
				Write32(pItemInfo, 0); //DecoratorID
				Write32(pItemInfo, raceid); //KartID
				Write32(pItemInfo, 0); //ColorID
				Write32(pItemInfo, 0); //TrackID
				Write32(pItemInfo, 0); //EffectID
				Write32(pItemInfo, 0); //BrandID
				Write32(pItemInfo, 0); //TailID

				len = pItemInfo - pAnonymousModeAvatarChangeInfo;
				Set16(pAnonymousModeAvatarChangeInfo, (WORD)len);
				pAnonymousModeAvatarChangeInfo += len;
			}

			len = pAnonymousModeAvatarChangeInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	else {
		Write8(p, 0); //AnonymousMode  匿名模式
	}

	/*
	m_astAnonymousModeAvatarChangeInfo[].m_dwPlayerUin
	m_astAnonymousModeAvatarChangeInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_iSuitID
	m_astAnonymousModeAvatarChangeInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_uchHasItemInfo
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iRoleID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iFaceID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGlassID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGloveID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iPersonarID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTattooID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iWingID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iDecoratorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iColorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrackID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iEffectID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iBrandID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTailID
	*/

	Write8(p, 0); //IsTimeShuttleGen

	Write8(p, 0); //HalloweenDdventureInfoNum
	/*
m_stHalloweenDdventureInfo[].m_ushPlayerNum
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_uiUin
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write8(p, 0); //MaxPropEffectNum
	Write8(p, 1); //IsCrossNoCollision	交叉没有碰撞

	typedef struct CheckConditionInfo // CheckConditionInfo
	{
		USHORT CheckType;
		UCHAR CheckParaNum;
		UINT IntParas[11];
	}*LPCheckConditionInfo;
	typedef struct RecordCheckCondInfo // RecordCheckCondInfo
	{
		USHORT CheckRelation;
		UCHAR ConditionNum;
		CheckConditionInfo CheckCondition[11];
	}*LPRecordCheckCondInfo;
	const static RecordCheckCondInfo RecordCheckCondCfg[] =
	{
		//RecordCheckCondInfo
		{0,2,{{0,5,{600,-1,-1,-1,3000}},{0,5,{300,-1,0,1000,1000}}}},
		{1,2,{{0,5,{0,50,-1,-1,0}},{1,6,{0,40000,-1,-1,10000,0}}}},
		{0,2,{{2,0},{3,1,{5}}}},
		{0,2,{{4,1,{5}},{5,1,{10}}}},
	};
	constexpr unsigned char RecordCheckCondNum = sizeof(RecordCheckCondCfg) / sizeof(RecordCheckCondCfg[0]); //


	Write8(p, RecordCheckCondNum); //RecordCheckCondNum
	/*
m_astRecordCheckConds[].m_usCheckRelation
m_astRecordCheckConds[].m_ucConditionNum
m_astRecordCheckConds[].m_astCheckConditions[].m_usCheckType
m_astRecordCheckConds[].m_astCheckConditions[].m_ucCheckParaNum
m_astRecordCheckConds[].m_astCheckConditions[].m_aIntParas[]
	*/
	for (UCHAR j = 0; j < RecordCheckCondNum; j++) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, RecordCheckCondCfg[j].CheckRelation); //CheckRelation
		Write8(pRecordCheckConds, RecordCheckCondCfg[j].ConditionNum); //ConditionNum
		for (UCHAR i = 0; i < RecordCheckCondCfg[j].ConditionNum; i++) { //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckType); //CheckType
			Write8(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum); //CheckParaNum
			for (size_t x = 0; x < RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum; x++) {
				Write32(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].IntParas[x]); //IntParas[]
			}

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}

		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, GameBeginSwitchFlag); //GameBeginSwitchFlag 1971		947
	Write8(p, 0); //TriggerVeggieDogTask

	Write8(p, 0); //HasQSpeedCrystalInfo
	/*
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalID
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalLevel
m_stQSpeedCrystalInfo[].m_uchQSpeedCrystalBuffBum
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_ushBuffID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchFeatureID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchBuffLevel
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchIsIcon
	*/


	Write16(p, 0);//BorderRaceType

	constexpr static unsigned int RaceSyncParaList[] = { 200 ,1000 ,3000 ,200 };
	constexpr unsigned char RaceSyncParaNum = sizeof(RaceSyncParaList) / sizeof(RaceSyncParaList[0]); //

	Write8(p, RaceSyncParaNum);//RaceSyncParaNum
	for (const auto& RaceSyncPara : RaceSyncParaList) {
		Write32(p, RaceSyncPara);//RaceSyncParaList[]
	}

	Write8(p, 0);//QingHuaMysteryGroundDataNum
	Write32(p, 14);//SpeclEffectSwitchFlag  特效开启标志
	Write32(p, 15);
	Write32(p, 0);


	len = p - buf.data();
	SendToClient(Client, 509, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


//原始定制
void NotifyGameBegin1(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player)
{
	bool AnonymousMode509 = false;
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	if (!Player.empty())
	{
		for (char i = 0; i < 6; i++)
		{
			if (Player[i])
			{
				Players[CurrentPlayers] = Player[i];
				CurrentPlayers++;
			}
		}
	}
	else
	{
		Players[0] = Client;
		CurrentPlayers = 1;
	}
	//printf("NotifyGameBegin CurrentPlayers:%d\n", CurrentPlayers);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	//Write32(p, 8000); //NormalLoadTime
	Write32(p, 20000); //NormalLoadTime 设置大一点防止有些模型加载不出来
	Write32(p, Client->MapID); //MapID
	//MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID); //GameID
	Write32(p, (DWORD)time(nullptr)); //Seed

	Write8(p, CurrentPlayers);
	//m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	//起跑线并排的玩家
	int index[] = { 2,3,1,4,0,5 };
	srand(time(NULL));
	// 打乱数组  
	shuffle(index, CurrentPlayers);
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Set32((BYTE*)&aRaceTrackOrders[index[i]], Players[i]->Uin);
	}


	Write16(p, Client->TotalRound); //TotalMapRound
	Write32(p, 0); //PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); //Record


	Write8(p, 0); //NianShouTypeNum
	/*
m_astNianShouAwardInfo[].m_cAwardInGameType
m_astNianShouAwardInfo[].m_iActIDForClient
m_astNianShouAwardInfo[].m_cPointArrayIdx
m_astNianShouAwardInfo[].m_cCurrAwardNum
m_astNianShouAwardInfo[].m_acAwardPos[]
	*/

	Write8(p, CurrentPlayers); //SyncCarNum
	for (char i = 0; i < CurrentPlayers; i++)
	{ //SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); //len

		Write32(pSyncCar, Players[i]->Uin); //PlayerUin
		if (Players[i] == Client)
		{
			Write32(pSyncCar, Players[i]->KartID);
		}
		else
		{
			if (Players[i]->IsAnonymous == 1)
			{
				AnonymousMode509 = true;
			}
			if ((Players[i]->isRace == 1 || AnonymousMode509) && Client->OBState != 1)
			{
				Write32(pSyncCar, raceid);
			}
			else {
				if (Players[i]->KartSkinID != 0)//CurCarID 这里也要查询是否有皮肤 不然对方看不见
				{
					Write32(pSyncCar, Players[i]->KartSkinID);
				}
				else
				{
					Write32(pSyncCar, Players[i]->KartID);
				}
			}

		}

		Write8(pSyncCar, 0); //HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 10); //ReportDataFlag

	typedef struct MapCheckDivInfo // MapCheckDivInfo
	{
		USHORT BeginIdx;
		USHORT EndIdx;
	}*LPREFITITEMWEIGHTTYPE;
	const static MapCheckDivInfo MapCheckDivInfoCfg[] =
	{
		//MapCheckDivInfo 地图检查分区信息
		{1,10},
		{11,18},
		{31,35},
		{46,49},
		{61,68},
	};

	constexpr unsigned char CheckDataNum = sizeof(MapCheckDivInfoCfg) / sizeof(MapCheckDivInfoCfg[0]); //
	Write8(p, CheckDataNum); //CheckDataNum
	/*
m_astMapCheckDiv[].m_shBeginIdx
m_astMapCheckDiv[].m_shEndIdx
	*/
	for (UCHAR i = 0; i < CheckDataNum; i++)
	{ //MapCheckDiv
		BYTE* pMapCheckDiv = p;
		Write16(pMapCheckDiv, 0); //len

		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].BeginIdx); //BeginIdx
		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].EndIdx); //EndIdx

		len = pMapCheckDiv - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //P2PMode
	Write8(p, 0); //TcpFrequence

	const static unsigned char acMultiInfo[] = {
		0,0,0,0,102,11,206,123,101,1,244,2,1,1,1,102,11,206,123,18,253,16,172,6,0,97,
		244,130,96,16,1,79,4,252,184,73,221,128,57,18,14,224,70,49,42,70,7,33,200,169
	};
	constexpr unsigned char MultiInfoLen = sizeof(acMultiInfo) / sizeof(acMultiInfo[0]); //

	Write8(p, MultiInfoLen); //MultiInfoLen
	memset(p, 0, MultiInfoLen);
	memcpy(p, acMultiInfo, MultiInfoLen);
	p += MultiInfoLen;

	Write8(p, 0); //FeedBackEnabled 启用反馈  这里是玩家双喷 什么技巧的提示 1是开启 0是关闭


	const static USHORT SpeedSectionLowerBoundCfg[] =
	{
		// SpeedSectionLowerBoundInfo  速度区段下限
		295,306,317,328,339,350,360,375,
	};
	constexpr unsigned char SpeedSectionNum = sizeof(SpeedSectionLowerBoundCfg) / sizeof(SpeedSectionLowerBoundCfg[0]); //

	Write8(p, SpeedSectionNum); //SpeedSectionNum
	/*
m_au16SpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < SpeedSectionNum; i++) {
		Write16(p, SpeedSectionLowerBoundCfg[i]); //SpeedSectionLowerBound[]
	}

	const static USHORT NormalSpeedSectionLowerBoundCfg[] =
	{
		//NormalSpeedSectionLowerBoundInfo  正常速度区段下限
		200,220,240,260,280,300,320,340,
	};
	constexpr unsigned char NormalSpeedSectionNum = sizeof(NormalSpeedSectionLowerBoundCfg) / sizeof(NormalSpeedSectionLowerBoundCfg[0]); //

	Write8(p, NormalSpeedSectionNum); //NormalSpeedSectionNum
	/*
m_au16NormalSpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < NormalSpeedSectionNum; i++) {
		Write16(p, NormalSpeedSectionLowerBoundCfg[i]); //NormalSpeedSectionLowerBound[]
	}

	Write8(p, 0); //MemCheckInfoNum
	/*
m_astMemCheckInfo[].m_u8MemIdxNum
m_astMemCheckInfo[].m_au32MemOffset[]
m_astMemCheckInfo[].m_u8DataType
m_astMemCheckInfo[].m_uPara
	*/

	Write8(p, 4); //ExtraInfoInterval
	Write16(p, -1); //OffsetThreshold
	Write32(p, 200); //SpeedRatioThreshold1
	Write32(p, 200); //SpeedRatioThreshold2
	Write32(p, 0); //HideTaskId
	Write8(p, 0); //HideTaskType
	Write32(p, 0); //HideTaskParam1

	Write8(p, 0); //ForceReportCPNum
	//m_au16ForceReportCP[]

	Write8(p, 0); //CliReserveFlag


	Write8(p, 1); //EnableAntiDriftCheat

	{ //AntiDriftCheatPara
		BYTE* pAntiDriftCheatPara = p;
		Write16(pAntiDriftCheatPara, 0); //len

		Write32(pAntiDriftCheatPara, 0); //MaxDriftHistoryTime
		Write32(pAntiDriftCheatPara, 0); //MinTimeInterval
		Write32(pAntiDriftCheatPara, 0); //MaxTimeInterval
		Write32(pAntiDriftCheatPara, 220); //NormalThreshold
		Write32(pAntiDriftCheatPara, 15); //JetThreshold
		Write32(pAntiDriftCheatPara, 0); //JetInterval
		Write32(pAntiDriftCheatPara, 0); //OneSideSlidingInterval

		len = pAntiDriftCheatPara - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasCrashModePara
	/*
m_astCrashModePara[].m_u8SponsorSkillNum
m_astCrashModePara[].m_astSponsorSkill[].m_u8SkillID
m_astCrashModePara[].m_astSponsorSkill[].m_iPara1
m_astCrashModePara[].m_u8InitEggNum
m_astCrashModePara[].m_u8EggRefreshInterval
m_astCrashModePara[].m_u8EggRefreshNum
m_astCrashModePara[].m_u8InitN2ONum
m_astCrashModePara[].m_u8EggSourceIncNum
m_astCrashModePara[].m_u32RandSeed
m_astCrashModePara[].m_u8PropPointRatio
m_astCrashModePara[].m_u8EggPropRatio
m_astCrashModePara[].m_u8AddN2ONumOnEggLost
	*/

	Write32(p, 0); //FizzStarTaskId
	Write8(p, 0); //FizzStarTaskType
	Write32(p, 0); //FizzStarTaskParam1
	Write32(p, 0); //LDMRecordID
	//Write32(p, 1508042); //GameSeq
	Write32(p, 3925099); //GameSeq

	Write8(p, CurrentPlayers); //PlayerNums
	for (UCHAR i = 0; i < CurrentPlayers; i++)
	{ //GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); //len

		Write32(pGameBeginPlayerInfo, Players[i]->Uin); //Uin
		//Write32(pGameBeginPlayerInfo, 0x0004DAE1); //ChumCircleID
		Write32(pGameBeginPlayerInfo, 0); //ChumCircleID

		Write8(pGameBeginPlayerInfo, 0); //SkillNums
		for (size_t i = 0; i < 0; i++)
		{
			BYTE* astChumCircleSkillInfo = pGameBeginPlayerInfo;
			Write16(astChumCircleSkillInfo, 0); //len

			if (i == 0) {
				Write32(astChumCircleSkillInfo, 1);
				Write32(astChumCircleSkillInfo, 3);
			}
			else
			{
				Write32(astChumCircleSkillInfo, 3);
				Write32(astChumCircleSkillInfo, 1);
			}

			len = astChumCircleSkillInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		/*
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillID
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillLv
		*/

		Write32(pGameBeginPlayerInfo, 0); //WorldEscapeTaskID

		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); //len


			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel


			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}

		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_ushBuffID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchFeatureID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchBuffLevel
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchIsIcon
		*/

		Write32(pGameBeginPlayerInfo, 0); //StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_iParaList[]
		*/

		Write32(pGameBeginPlayerInfo, 0);//EquipSkillId

		if (Players[i]->SpeclEffect == 1) {
			Write8(pGameBeginPlayerInfo, 3);//GLSpeclEffectNum

			for (size_t i = 0; i < 3; i++)
			{//GLSpeclEffectInfo
				BYTE* pGLSpeclEffectInfo = pGameBeginPlayerInfo;
				Write16(pGLSpeclEffectInfo, 0);//len

				Write32(pGLSpeclEffectInfo, i + 1);//SpeclEffectType
				Write32(pGLSpeclEffectInfo, 0);//Para0

				len = (WORD)(pGLSpeclEffectInfo - pGameBeginPlayerInfo);
				Set16(pGameBeginPlayerInfo, len);
				pGameBeginPlayerInfo += len;
			}
		}
		else {
			Write8(pGameBeginPlayerInfo, 0);//GLSpeclEffectNum
		}



		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //PlayeCheerNums
	Write8(p, 0); //MapNum
	Write32(p, 0); //SpecialMapId
	char NPCFlag = 0;
	char border = 0;
	if (Client->TaskID == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else
	{
		NPCFlag = 1;
	}

	if ((Client->isManMachine == 0 && border == 1) || Client->real == 1) {
		NPCFlag = 0;
	}


	if (!NPCFlag)
	{
		Write8(p, 0);//NPCNum
	}
	else
	{
		Write8(p, NpcNum); //NPCNum
		for (size_t i = 0; i < NpcNum; i++)
		{
			//NPCInfo
			BYTE* pNPCInfo = p;
			Write16(pNPCInfo, 0); //len

			Write32(pNPCInfo, NpcListInfos[i].NpcID); //NpcID
			memset(pNPCInfo, 0, 17);
			memcpy(pNPCInfo, NpcListInfos[i].RoomName, 17);
			pNPCInfo += 17;
			memset(pNPCInfo, 0, 17);//GuildName[]
			pNPCInfo += 17;
			{ //NpcItemInfo
				BYTE* pNpcItemInfo = pNPCInfo;
				Write16(pNpcItemInfo, 0); //len

				Write32(pNpcItemInfo, NpcListInfos[i].RoleID); //RoleID
				Write32(pNpcItemInfo, NpcListInfos[i].HairID); //HairID
				Write32(pNpcItemInfo, NpcListInfos[i].FaceID); //FaceID
				Write32(pNpcItemInfo, NpcListInfos[i].GlassID); //GlassID
				Write32(pNpcItemInfo, NpcListInfos[i].CoatID); //CoatID
				Write32(pNpcItemInfo, NpcListInfos[i].GloveID); //GloveID
				Write32(pNpcItemInfo, NpcListInfos[i].TrousersID); //TrousersID
				Write32(pNpcItemInfo, NpcListInfos[i].PersonarID); //PersonarID
				Write32(pNpcItemInfo, NpcListInfos[i].TattooID); //TattooID
				Write32(pNpcItemInfo, NpcListInfos[i].WingID); //WingID
				Write32(pNpcItemInfo, NpcListInfos[i].DecoratorID); //DecoratorID
				Write32(pNpcItemInfo, NpcListInfos[i].KartID); //KartID
				Write32(pNpcItemInfo, NpcListInfos[i].ColorID); //ColorID
				Write32(pNpcItemInfo, NpcListInfos[i].TrackID); //TrackID
				Write32(pNpcItemInfo, NpcListInfos[i].EffectID); //EffectID
				Write32(pNpcItemInfo, NpcListInfos[i].BrandID); //BrandID
				Write32(pNpcItemInfo, NpcListInfos[i].TailID); //TailID

				len = pNpcItemInfo - pNPCInfo;
				Set16(pNPCInfo, (WORD)len);
				pNPCInfo += len;
			}

			memcpy(pNPCInfo, "\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c", 252);
			pNPCInfo += 252;
			Write16(pNPCInfo, NpcListInfos[i].MaxLevel); //NPCAILevel
			Write16(pNPCInfo, 0); //TeamID

			len = pNPCInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//int NPCKartID[5] = {127616,127365,123501 ,118729,110710 };
		//const char* NpcName[5] = {"陈伯\x0\x0","关佳鑫","胡旭\x0\x0","江章闯","张云彬"};
	//NPC配置 //NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
	//memcpy(p, "\x01\x6e\x00\x00\x00\x01\xc9\xe7\xbb\xe1\xc3\xc0\xd1\xf2\xd1\xf2\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x59\x02\x00\x00\x00\x00\x00\x01\x88\x84\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x07\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00", 366);
	//p += 366;

	}
	//npc数据
	//"\x05\x01\x6e\x00\x00\x00\x01\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x02\xd0\xa1\xe9\xd9\xd7\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe6\x6b\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x23\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x03\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x08\x00\x00\x01\x6e\x00\x00\x00\x04\xc2\xed\xc5\xa3\xb1\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\x00\x00\x00\x01\xd2\x03\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf1\xcd\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x05\x57\x6f\x6c\x76\x65\x73\x2e\x4e\x69\x6e\x67\x51\x69\x6e\x67\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xcb\xdf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf7\xc5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00";
	Write8(p, 0); //MapNum
	int* aNPCRaceTrackOrders = (int*)p;
	for (size_t i = 0; i < MAXNPCNUMINROOM; i++)
	{
		//m_aunNPCTrackOrders[]
		Write32(p, 0);
	}
	//起跑线并排的机器人
	//int npcindex[] = { 2,3,1,4,0 };
	if (Client->TaskID != 0)
	{
		for (char i = 0; i < 5; i++)
		{
			Set32((BYTE*)&aNPCRaceTrackOrders[i], i + 1);
		}
	}

	Write8(p, 0); //PlayerNum
	Write8(p, 0); //HasTowerInfo
	Write8(p, 0); //HasWeRelayGameBeginInfo

	Write8(p, 1); //ChangeCar
	Write32(p, 0x01F0D892); //GameSeqIDHigh	12345678
	Write32(p, 0x3325F51C); //GameSeqIDLow	87654321
	Write32(p, 0); //KubiBigCoinReplaceItem
	Write32(p, 0); //TimerChallengeJumpLevel
	Write8(p, 0); //ShadowRunDelay
	Write16(p, 0); //ShadowCatchUpContinuesTime

	Write8(p, 0); //ArrestPlayerNums
	/*
m_astArrestRoleInfo[].m_uiUin
m_astArrestRoleInfo[].m_bTeamID
	*/

	Write16(p, 0x000a); //MonitorCheckPointBegin
	Write16(p, 0x000b); //MonitorCheckPointEnd

	Write8(p, 1); //MonitorLapCnt
	Write8(p, 0); //GameType
	Write16(p, 0); //PointID
	Write16(p, 1); //BaseMode
	Write16(p, 1); //SubMode
	Write8(p, 0); //GameType
	Write16(p, 0); //ReportPosSpan

	Write16(p, 0); //PropID
	Write32(p, 0); //PropIndex
	Write16(p, 0); //PropNum


	Write8(p, 1); //BaseGameModeEx
	Write8(p, 0); //ParaNum

	Write8(p, 0); //MapCheckpointFileIndex

	Write8(p, 0); //HasTimerChallenge2ndGameBeginInfo
	/*
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iPlayerNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_uiUin
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iWeekHighestTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iBonusJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iHistoryMaxTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iJumpLevel
m_astTimerChallenge2ndGameBeginInfo[].m_uiBeginUin
m_astTimerChallenge2ndGameBeginInfo[].m_bReverse
	*/

	Write8(p, 0); //HasGameStageInfo
	/*
m_stGameStageInfo[].m_u8StageIndex
m_stGameStageInfo[].m_u8TotalStageNum
	*/

	Write8(p, 0); //CarCollectInfoNum
	/*
m_stCarCollectInfo[].m_ushPlayerNum
m_stCarCollectInfo[].m_stCarCollectInfo[].m_uiUin
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write16(p, 2560); //ReportAntiCollisionDataTime 报告防冲突数据时间
	Write32(p, 0); //Duration
	Write32(p, 0); //BeginCDTime
	Write32(p, 0); //PropInteval
	Write32(p, 0x00000f00); //MoveFreq

	Write8(p, 0); //HaveGameLogicTask
	/*
m_astRakedMatchGamelogicTaskInfo[].m_uiGameLogicTaskType
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskFinishCondValue
m_astRakedMatchGamelogicTaskInfo[].m_ucJudgeRule
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskAddGradeScore
m_astRakedMatchGamelogicTaskInfo[].m_uchGradeScoreRangeNum
m_astRakedMatchGamelogicTaskInfo[].m_auiGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskFinishCondValueNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskFinishCondValueRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchFailedAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiFailedAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_ucFinishTask
m_astRakedMatchGamelogicTaskInfo[].m_uiExtraFinishTaskAddScore
	*/

	Write16(p, 0); //RankedMatchBegainTipInfoType

	Write16(p, 0); //BegainTipLen

	Write8(p, 0); //BuffBum
	/*
m_stEffectBuffInfo[].m_ushBuffID
m_stEffectBuffInfo[].m_uchFeatureID
m_stEffectBuffInfo[].m_uchBuffLevel
m_stEffectBuffInfo[].m_uchIsIcon
	*/

	Write8(p, 0); //MapChallengeInfoNum
	/*
m_stMapChallengeInfo[].m_uchStarInfoNum
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uiTime
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_bType
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_ushCount
m_stMapChallengeInfo[].m_stAssessInfo.m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_ushMsgLen
m_stMapChallengeInfo[].m_iMapID
*/

	Write8(p, 0); //IsChangeCar

	if (AnonymousMode509 || (AnonymousMode == 1 && Client->TaskID == 0xFFFC)) {
		Write8(p, CurrentPlayers - 1); // AnonymousMode
		for (size_t i = 0; i < CurrentPlayers; i++)
		{ //AnonymousModeAvatarChangeInfo
			if (Player[i] == Client)
			{
				continue;
			}
			BYTE* pAnonymousModeAvatarChangeInfo = p;
			Write16(pAnonymousModeAvatarChangeInfo, 0); //len

			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Uin); //PlayerUin
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10025 : 10042); //HairID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
			Write32(pAnonymousModeAvatarChangeInfo, 0); //SuitID
			Write32(pAnonymousModeAvatarChangeInfo, raceid); //KartID
			Write8(pAnonymousModeAvatarChangeInfo, 1); //HasItemInfo
			for (size_t j = 0; j < 1; j++)
			{ //ItemInfo
				BYTE* pItemInfo = pAnonymousModeAvatarChangeInfo;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Player[i]->Gender ? 10022 : 10018); //RoleID
				Write32(pItemInfo, Player[i]->Gender ? 10025 : 10042); //HairID
				Write32(pItemInfo, 0); //FaceID
				Write32(pItemInfo, 0); //GlassID
				Write32(pItemInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
				Write32(pItemInfo, Player[i]->Gender ? 10045 : 10032); //GloveID
				Write32(pItemInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
				Write32(pItemInfo, 0); //PersonarID
				Write32(pItemInfo, 0); //TattooID
				Write32(pItemInfo, 0); //WingID
				Write32(pItemInfo, 0); //DecoratorID
				Write32(pItemInfo, raceid); //KartID
				Write32(pItemInfo, 0); //ColorID
				Write32(pItemInfo, 0); //TrackID
				Write32(pItemInfo, 0); //EffectID
				Write32(pItemInfo, 0); //BrandID
				Write32(pItemInfo, 0); //TailID

				len = pItemInfo - pAnonymousModeAvatarChangeInfo;
				Set16(pAnonymousModeAvatarChangeInfo, (WORD)len);
				pAnonymousModeAvatarChangeInfo += len;
			}

			len = pAnonymousModeAvatarChangeInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	else {
		Write8(p, 0); //AnonymousMode  匿名模式
	}
	/*
	m_astAnonymousModeAvatarChangeInfo[].m_dwPlayerUin
	m_astAnonymousModeAvatarChangeInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_iSuitID
	m_astAnonymousModeAvatarChangeInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_uchHasItemInfo
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iRoleID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iFaceID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGlassID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGloveID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iPersonarID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTattooID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iWingID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iDecoratorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iColorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrackID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iEffectID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iBrandID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTailID
	*/

	Write8(p, 0); //IsTimeShuttleGen

	Write8(p, 0); //HalloweenDdventureInfoNum
	/*
m_stHalloweenDdventureInfo[].m_ushPlayerNum
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_uiUin
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write8(p, 0); //MaxPropEffectNum
	Write8(p, 0); //IsCrossNoCollision	交叉没有碰撞

	typedef struct CheckConditionInfo // CheckConditionInfo
	{
		USHORT CheckType;
		UCHAR CheckParaNum;
		UINT IntParas[11];
	}*LPCheckConditionInfo;
	typedef struct RecordCheckCondInfo // RecordCheckCondInfo
	{
		USHORT CheckRelation;
		UCHAR ConditionNum;
		CheckConditionInfo CheckCondition[11];
	}*LPRecordCheckCondInfo;
	const static RecordCheckCondInfo RecordCheckCondCfg[] =
	{
		//RecordCheckCondInfo
		{0,2,{{0,5,{600,-1,-1,-1,3000}},{0,5,{300,-1,0,1000,1000}}}},
		{1,2,{{0,5,{0,50,-1,-1,0}},{1,6,{0,40000,-1,-1,10000,0}}}},
		{0,1,{{2,0}}},
	};
	constexpr unsigned char RecordCheckCondNum = sizeof(RecordCheckCondCfg) / sizeof(RecordCheckCondCfg[0]); //

	Write8(p, RecordCheckCondNum); //RecordCheckCondNum
	/*
m_astRecordCheckConds[].m_usCheckRelation
m_astRecordCheckConds[].m_ucConditionNum
m_astRecordCheckConds[].m_astCheckConditions[].m_usCheckType
m_astRecordCheckConds[].m_astCheckConditions[].m_ucCheckParaNum
m_astRecordCheckConds[].m_astCheckConditions[].m_aIntParas[]
	*/
	for (UCHAR j = 0; j < RecordCheckCondNum; j++) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, RecordCheckCondCfg[j].CheckRelation); //CheckRelation
		Write8(pRecordCheckConds, RecordCheckCondCfg[j].ConditionNum); //ConditionNum
		for (UCHAR i = 0; i < RecordCheckCondCfg[j].ConditionNum; i++) { //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckType); //CheckType
			Write8(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum); //CheckParaNum
			for (size_t x = 0; x < RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum; x++) {
				Write32(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].IntParas[x]); //IntParas[]
			}

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}

		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, GameBeginSwitchFlag); //GameBeginSwitchFlag 1971		947
	Write8(p, 0); //TriggerVeggieDogTask

	Write8(p, 0); //HasQSpeedCrystalInfo
	/*
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalID
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalLevel
m_stQSpeedCrystalInfo[].m_uchQSpeedCrystalBuffBum
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_ushBuffID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchFeatureID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchBuffLevel
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchIsIcon
	*/


	Write16(p, 0);//BorderRaceType

	constexpr static unsigned int RaceSyncParaList[] = { 200 ,1000 ,3000 ,200 };
	constexpr unsigned char RaceSyncParaNum = sizeof(RaceSyncParaList) / sizeof(RaceSyncParaList[0]); //

	Write8(p, RaceSyncParaNum);//RaceSyncParaNum
	for (const auto& RaceSyncPara : RaceSyncParaList) {
		Write32(p, RaceSyncPara);//RaceSyncParaList[]
	}

	Write8(p, 0);//QingHuaMysteryGroundDataNum
	Write32(p, 14);//SpeclEffectSwitchFlag  特效开启标志
	Write32(p, 15);
	Write32(p, 0);


	len = p - buf.data();
	SendToClient(Client, 509, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

//mc
void NotifyGameBegin2(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player)
{
	bool AnonymousMode509 = false;
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	if (!Player.empty())
	{
		for (char i = 0; i < 6; i++)
		{
			if (Player[i])
			{
				Players[CurrentPlayers] = Player[i];
				CurrentPlayers++;
			}
		}
	}
	else
	{
		Players[0] = Client;
		CurrentPlayers = 1;
	}
	//printf("NotifyGameBegin CurrentPlayers:%d\n", CurrentPlayers);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	//Write32(p, 8000); //NormalLoadTime
	Write32(p, 20000); //NormalLoadTime 设置大一点防止有些模型加载不出来
	Write32(p, Client->MapID); //MapID
	//MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID); //GameID
	Write32(p, (DWORD)time(nullptr)); //Seed

	Write8(p, CurrentPlayers);
	//m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	//起跑线并排的玩家
	int index[] = { 2,3,1,4,0,5 };
	srand(time(NULL));
	// 打乱数组  
	shuffle(index, CurrentPlayers);
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Set32((BYTE*)&aRaceTrackOrders[index[i]], Players[i]->Uin);
	}


	Write16(p, Client->TotalRound); //TotalMapRound
	Write32(p, 0); //PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); //Record


	Write8(p, 0); //NianShouTypeNum
	/*
m_astNianShouAwardInfo[].m_cAwardInGameType
m_astNianShouAwardInfo[].m_iActIDForClient
m_astNianShouAwardInfo[].m_cPointArrayIdx
m_astNianShouAwardInfo[].m_cCurrAwardNum
m_astNianShouAwardInfo[].m_acAwardPos[]
	*/

	Write8(p, CurrentPlayers); //SyncCarNum
	for (char i = 0; i < CurrentPlayers; i++)
	{ //SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); //len

		Write32(pSyncCar, Players[i]->Uin); //PlayerUin
		if (Players[i] == Client)
		{
			Write32(pSyncCar, Players[i]->KartID);
		}
		else
		{
			if (Players[i]->IsAnonymous == 1)
			{
				AnonymousMode509 = true;
			}
			if ((Players[i]->isRace == 1 || AnonymousMode509) && Client->OBState != 1)
			{
				Write32(pSyncCar, raceid);
			}
			else {
				if (Players[i]->KartSkinID != 0)//CurCarID 这里也要查询是否有皮肤 不然对方看不见
				{
					Write32(pSyncCar, Players[i]->KartSkinID);
				}
				else
				{
					Write32(pSyncCar, Players[i]->KartID);
				}
			}

		}

		Write8(pSyncCar, 0); //HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 10); //ReportDataFlag

	typedef struct MapCheckDivInfo // MapCheckDivInfo
	{
		USHORT BeginIdx;
		USHORT EndIdx;
	}*LPREFITITEMWEIGHTTYPE;
	const static MapCheckDivInfo MapCheckDivInfoCfg[] =
	{
		//MapCheckDivInfo 地图检查分区信息
		{1,10},
		{11,18},
		{31,35},
		{46,49},
		{61,68},
	};

	constexpr unsigned char CheckDataNum = sizeof(MapCheckDivInfoCfg) / sizeof(MapCheckDivInfoCfg[0]); //
	Write8(p, CheckDataNum); //CheckDataNum
	/*
m_astMapCheckDiv[].m_shBeginIdx
m_astMapCheckDiv[].m_shEndIdx
	*/
	for (UCHAR i = 0; i < CheckDataNum; i++)
	{ //MapCheckDiv
		BYTE* pMapCheckDiv = p;
		Write16(pMapCheckDiv, 0); //len

		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].BeginIdx); //BeginIdx
		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].EndIdx); //EndIdx

		len = pMapCheckDiv - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //P2PMode
	Write8(p, 0); //TcpFrequence

	const static unsigned char acMultiInfo[] = {
		0,0,0,0,102,11,206,123,101,1,244,2,1,1,1,102,11,206,123,18,253,16,172,6,0,97,
		244,130,96,16,1,79,4,252,184,73,221,128,57,18,14,224,70,49,42,70,7,33,200,169
	};
	constexpr unsigned char MultiInfoLen = sizeof(acMultiInfo) / sizeof(acMultiInfo[0]); //

	Write8(p, MultiInfoLen); //MultiInfoLen
	memset(p, 0, MultiInfoLen);
	memcpy(p, acMultiInfo, MultiInfoLen);
	p += MultiInfoLen;

	Write8(p, 0); //FeedBackEnabled 启用反馈  这里是玩家双喷 什么技巧的提示 1是开启 0是关闭


	const static USHORT SpeedSectionLowerBoundCfg[] =
	{
		// SpeedSectionLowerBoundInfo  速度区段下限
		295,306,317,328,339,350,360,375,
	};
	constexpr unsigned char SpeedSectionNum = sizeof(SpeedSectionLowerBoundCfg) / sizeof(SpeedSectionLowerBoundCfg[0]); //

	Write8(p, SpeedSectionNum); //SpeedSectionNum
	/*
m_au16SpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < SpeedSectionNum; i++) {
		Write16(p, SpeedSectionLowerBoundCfg[i]); //SpeedSectionLowerBound[]
	}

	const static USHORT NormalSpeedSectionLowerBoundCfg[] =
	{
		//NormalSpeedSectionLowerBoundInfo  正常速度区段下限
		200,220,240,260,280,300,320,340,
	};
	constexpr unsigned char NormalSpeedSectionNum = sizeof(NormalSpeedSectionLowerBoundCfg) / sizeof(NormalSpeedSectionLowerBoundCfg[0]); //

	Write8(p, NormalSpeedSectionNum); //NormalSpeedSectionNum
	/*
m_au16NormalSpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < NormalSpeedSectionNum; i++) {
		Write16(p, NormalSpeedSectionLowerBoundCfg[i]); //NormalSpeedSectionLowerBound[]
	}

	Write8(p, 0); //MemCheckInfoNum
	/*
m_astMemCheckInfo[].m_u8MemIdxNum
m_astMemCheckInfo[].m_au32MemOffset[]
m_astMemCheckInfo[].m_u8DataType
m_astMemCheckInfo[].m_uPara
	*/

	Write8(p, 4); //ExtraInfoInterval
	Write16(p, -1); //OffsetThreshold
	Write32(p, 200); //SpeedRatioThreshold1
	Write32(p, 200); //SpeedRatioThreshold2
	Write32(p, 0); //HideTaskId
	Write8(p, 0); //HideTaskType
	Write32(p, 0); //HideTaskParam1

	Write8(p, 0); //ForceReportCPNum
	//m_au16ForceReportCP[]

	Write8(p, 0); //CliReserveFlag


	Write8(p, 1); //EnableAntiDriftCheat

	{ //AntiDriftCheatPara
		BYTE* pAntiDriftCheatPara = p;
		Write16(pAntiDriftCheatPara, 0); //len

		Write32(pAntiDriftCheatPara, 0); //MaxDriftHistoryTime
		Write32(pAntiDriftCheatPara, 0); //MinTimeInterval
		Write32(pAntiDriftCheatPara, 0); //MaxTimeInterval
		Write32(pAntiDriftCheatPara, 220); //NormalThreshold
		Write32(pAntiDriftCheatPara, 15); //JetThreshold
		Write32(pAntiDriftCheatPara, 0); //JetInterval
		Write32(pAntiDriftCheatPara, 0); //OneSideSlidingInterval

		len = pAntiDriftCheatPara - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasCrashModePara
	/*
m_astCrashModePara[].m_u8SponsorSkillNum
m_astCrashModePara[].m_astSponsorSkill[].m_u8SkillID
m_astCrashModePara[].m_astSponsorSkill[].m_iPara1
m_astCrashModePara[].m_u8InitEggNum
m_astCrashModePara[].m_u8EggRefreshInterval
m_astCrashModePara[].m_u8EggRefreshNum
m_astCrashModePara[].m_u8InitN2ONum
m_astCrashModePara[].m_u8EggSourceIncNum
m_astCrashModePara[].m_u32RandSeed
m_astCrashModePara[].m_u8PropPointRatio
m_astCrashModePara[].m_u8EggPropRatio
m_astCrashModePara[].m_u8AddN2ONumOnEggLost
	*/

	Write32(p, 0); //FizzStarTaskId
	Write8(p, 0); //FizzStarTaskType
	Write32(p, 0); //FizzStarTaskParam1
	Write32(p, 0); //LDMRecordID
	//Write32(p, 1508042); //GameSeq
	Write32(p, 0x00884301); //GameSeq

	Write8(p, CurrentPlayers); //PlayerNums
	for (UCHAR i = 0; i < CurrentPlayers; i++)
	{ //GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); //len

		Write32(pGameBeginPlayerInfo, Players[i]->Uin); //Uin
		//Write32(pGameBeginPlayerInfo, 0x0004DAE1); //ChumCircleID
		Write32(pGameBeginPlayerInfo, 0); //ChumCircleID

		Write8(pGameBeginPlayerInfo, 0); //SkillNums
		for (size_t i = 0; i < 0; i++)
		{
			BYTE* astChumCircleSkillInfo = pGameBeginPlayerInfo;
			Write16(astChumCircleSkillInfo, 0); //len

			if (i == 0) {
				Write32(astChumCircleSkillInfo, 1);
				Write32(astChumCircleSkillInfo, 3);
			}
			else
			{
				Write32(astChumCircleSkillInfo, 3);
				Write32(astChumCircleSkillInfo, 1);
			}

			len = astChumCircleSkillInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		/*
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillID
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillLv
		*/

		Write32(pGameBeginPlayerInfo, 0); //WorldEscapeTaskID

		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); //len


			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel


			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}

		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_ushBuffID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchFeatureID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchBuffLevel
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchIsIcon
		*/

		Write32(pGameBeginPlayerInfo, 0); //StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_iParaList[]
		*/

		Write32(pGameBeginPlayerInfo, 0);//EquipSkillId

		if (Players[i]->SpeclEffect == 1) {
			Write8(pGameBeginPlayerInfo, 3);//GLSpeclEffectNum

			for (size_t i = 0; i < 3; i++)
			{//GLSpeclEffectInfo
				BYTE* pGLSpeclEffectInfo = pGameBeginPlayerInfo;
				Write16(pGLSpeclEffectInfo, 0);//len

				Write32(pGLSpeclEffectInfo, i + 1);//SpeclEffectType
				Write32(pGLSpeclEffectInfo, 0);//Para0

				len = (WORD)(pGLSpeclEffectInfo - pGameBeginPlayerInfo);
				Set16(pGameBeginPlayerInfo, len);
				pGameBeginPlayerInfo += len;
			}
		}
		else {
			Write8(pGameBeginPlayerInfo, 0);//GLSpeclEffectNum
		}



		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //PlayeCheerNums
	Write8(p, 0); //MapNum
	Write32(p, 0); //SpecialMapId
	char NPCFlag = 0;
	char border = 0;
	if (Client->TaskID == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else
	{
		NPCFlag = 1;
	}

	if ((Client->isManMachine == 0 && border == 1) || Client->real == 1) {
		NPCFlag = 0;
	}


	if (!NPCFlag)
	{
		Write8(p, 0);//NPCNum
	}
	else
	{
		Write8(p, NpcNum); //NPCNum
		for (size_t i = 0; i < NpcNum; i++)
		{
			//NPCInfo
			BYTE* pNPCInfo = p;
			Write16(pNPCInfo, 0); //len

			Write32(pNPCInfo, NpcListInfos[i].NpcID); //NpcID
			memset(pNPCInfo, 0, 17);
			memcpy(pNPCInfo, NpcListInfos[i].RoomName, 17);
			pNPCInfo += 17;
			memset(pNPCInfo, 0, 17);//GuildName[]
			pNPCInfo += 17;
			{ //NpcItemInfo
				BYTE* pNpcItemInfo = pNPCInfo;
				Write16(pNpcItemInfo, 0); //len

				Write32(pNpcItemInfo, NpcListInfos[i].RoleID); //RoleID
				Write32(pNpcItemInfo, NpcListInfos[i].HairID); //HairID
				Write32(pNpcItemInfo, NpcListInfos[i].FaceID); //FaceID
				Write32(pNpcItemInfo, NpcListInfos[i].GlassID); //GlassID
				Write32(pNpcItemInfo, NpcListInfos[i].CoatID); //CoatID
				Write32(pNpcItemInfo, NpcListInfos[i].GloveID); //GloveID
				Write32(pNpcItemInfo, NpcListInfos[i].TrousersID); //TrousersID
				Write32(pNpcItemInfo, NpcListInfos[i].PersonarID); //PersonarID
				Write32(pNpcItemInfo, NpcListInfos[i].TattooID); //TattooID
				Write32(pNpcItemInfo, NpcListInfos[i].WingID); //WingID
				Write32(pNpcItemInfo, NpcListInfos[i].DecoratorID); //DecoratorID
				Write32(pNpcItemInfo, NpcListInfos[i].KartID); //KartID
				Write32(pNpcItemInfo, NpcListInfos[i].ColorID); //ColorID
				Write32(pNpcItemInfo, NpcListInfos[i].TrackID); //TrackID
				Write32(pNpcItemInfo, NpcListInfos[i].EffectID); //EffectID
				Write32(pNpcItemInfo, NpcListInfos[i].BrandID); //BrandID
				Write32(pNpcItemInfo, NpcListInfos[i].TailID); //TailID

				len = pNpcItemInfo - pNPCInfo;
				Set16(pNPCInfo, (WORD)len);
				pNPCInfo += len;
			}

			memcpy(pNPCInfo, "\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c", 252);
			pNPCInfo += 252;
			Write16(pNPCInfo, NpcListInfos[i].MaxLevel); //NPCAILevel
			Write16(pNPCInfo, 0); //TeamID

			len = pNPCInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//int NPCKartID[5] = {127616,127365,123501 ,118729,110710 };
		//const char* NpcName[5] = {"陈伯\x0\x0","关佳鑫","胡旭\x0\x0","江章闯","张云彬"};
	//NPC配置 //NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
	//memcpy(p, "\x01\x6e\x00\x00\x00\x01\xc9\xe7\xbb\xe1\xc3\xc0\xd1\xf2\xd1\xf2\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x59\x02\x00\x00\x00\x00\x00\x01\x88\x84\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x07\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00", 366);
	//p += 366;

	}
	//npc数据
	//"\x05\x01\x6e\x00\x00\x00\x01\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x02\xd0\xa1\xe9\xd9\xd7\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe6\x6b\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x23\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x03\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x08\x00\x00\x01\x6e\x00\x00\x00\x04\xc2\xed\xc5\xa3\xb1\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\x00\x00\x00\x01\xd2\x03\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf1\xcd\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x05\x57\x6f\x6c\x76\x65\x73\x2e\x4e\x69\x6e\x67\x51\x69\x6e\x67\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xcb\xdf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf7\xc5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00";
	Write8(p, 0); //MapNum
	int* aNPCRaceTrackOrders = (int*)p;
	for (size_t i = 0; i < MAXNPCNUMINROOM; i++)
	{
		//m_aunNPCTrackOrders[]
		Write32(p, 0);
	}
	//起跑线并排的机器人
	//int npcindex[] = { 2,3,1,4,0 };
	if (Client->TaskID != 0)
	{
		for (char i = 0; i < 5; i++)
		{
			Set32((BYTE*)&aNPCRaceTrackOrders[i], i + 1);
		}
	}

	Write8(p, 0); //PlayerNum
	Write8(p, 0); // HasTowerInfo


	Write8(p, 0); // HasWeRelayGameBeginInfo

	Write8(p, 0);		  // ChangeCar
	Write32(p, 0x0124b17e); // GameSeqIDHigh
	Write32(p, 2938796198); // GameSeqIDLow
	Write32(p, 0);		  // KubiBigCoinReplaceItem
	Write32(p, 0);		  // TimerChallengeJumpLevel
	Write8(p, 0);		  // ShadowRunDelay
	Write16(p, 0);		  // ShadowCatchUpContinuesTime

	Write8(p, 0); // ArrestPlayerNums


	Write16(p, 0x000a); // MonitorCheckPointBegin
	Write16(p, 0x000b); // MonitorCheckPointEnd

	Write8(p, 1);  // MonitorLapCnt
	Write8(p, 0);  // GameType
	Write8(p, 0);  // PointID
	Write16(p, 1); // BaseMode
	Write16(p, 1); // SubMode
	Write8(p, 0);  // GameType
	Write16(p, 0); // ReportPosSpan

	Write16(p, 0); // PropID
	Write32(p, 0); // PropIndex
	Write16(p, 0); // PropNum

	Write8(p, 0); // BaseGameModeEx
	Write8(p, 1); // ParaNum
	if (1 == 1) {
		Write16(p, 0); // ParaNum[]
	}


	Write8(p, 0); // MapCheckpointFileIndex

	Write8(p, 0); // HasTimerChallenge2ndGameBeginInfo
	for (size_t i = 0; i < 0; i++)
	{ //TimerChallenge2ndGameBeginInfo
		BYTE* pTimerChallenge2ndGameBeginInfo = p;
		Write16(pTimerChallenge2ndGameBeginInfo, 0); //len

		{ //JumpInfo
			BYTE* pJumpInfo = pTimerChallenge2ndGameBeginInfo;
			Write16(pJumpInfo, 0); //len

			Write32(pJumpInfo, 1); //PlayerNum
			for (size_t i = 0; i < 1; i++)
			{ //PlayerJumpInfo
				BYTE* pPlayerJumpInfo = pJumpInfo;
				Write16(pPlayerJumpInfo, 0); //len

				Write32(pPlayerJumpInfo, Client->Uin); //Uin
				Write32(pPlayerJumpInfo, 3); //Jumplevel
				Write32(pPlayerJumpInfo, 0xd); //WeekHighestTongGuanNum 最高记录
				Write32(pPlayerJumpInfo, 0); //BonusJumplevel
				Write32(pPlayerJumpInfo, 10); //HistoryMaxTongGuanNum 

				len = pPlayerJumpInfo - pJumpInfo;
				Set16(pJumpInfo, (WORD)len);
				pJumpInfo += len;
			}
			Write32(pJumpInfo, 3); //JumpLevel 跳关

			len = pJumpInfo - pTimerChallenge2ndGameBeginInfo;
			Set16(pTimerChallenge2ndGameBeginInfo, (WORD)len);
			pTimerChallenge2ndGameBeginInfo += len;
		}
		Write32(pTimerChallenge2ndGameBeginInfo, Client->Uin); //BeginUin
		Write8(pTimerChallenge2ndGameBeginInfo, 0); //Reverse
		memcpy(p, "\x01\x00\x00\x00\x01\x03\x00\x08\x00\xcc\x00\x00\x00\x00\x00\x08\x00\xcf\x00\x00\x00\x00\x00\x08\x00\xd2\x00\x00\x00\x00\x00\x02\x00\x84\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x00\x42\xc1\xf7\xbb\xf0\xc1\xfa\xbe\xa7\xb8\xb3\xd3\xe8\xc8\xfc\xb3\xb5\xb7\xc9\xd0\xd0\xb5\xc4\xc4\xdc\xc1\xa6\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x00\x01\x63\x00\x00\x00\x00\x00\x00\x00\x00\x00", 164);
		pTimerChallenge2ndGameBeginInfo += 164;
		len = pTimerChallenge2ndGameBeginInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	/*
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iPlayerNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_uiUin
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iWeekHighestTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iBonusJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iHistoryMaxTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iJumpLevel
m_astTimerChallenge2ndGameBeginInfo[].m_uiBeginUin
m_astTimerChallenge2ndGameBeginInfo[].m_bReverse
	*/

	Write8(p, 0); // HasGameStageInfo
	for (size_t i = 0; i < 0; i++)
	{ //GameStageInfo
		BYTE* pGameStageInfo = p;
		Write16(pGameStageInfo, 0); //len

		Write8(pGameStageInfo, 0); //StageIndex
		Write8(pGameStageInfo, 0); //TotalStageNum

		len = pGameStageInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	/*
m_stGameStageInfo[].m_u8StageIndex
m_stGameStageInfo[].m_u8TotalStageNum
	*/
	Write8(p, 0); // CarCollectInfoNum
	for (size_t i = 0; i < 0; i++)
	{ //CarCollectInfo
		BYTE* pCarCollectInfo = p;
		Write16(pCarCollectInfo, 0); //len

		Write16(pCarCollectInfo, 0); //PlayerNum
		for (size_t i = 0; i < 0; i++)
		{ //CarCollectInfo
			BYTE* pCarCollectInfo2 = pCarCollectInfo;
			Write16(pCarCollectInfo2, 0); //len

			Write32(pCarCollectInfo2, 0); //Uin
			Write32(pCarCollectInfo2, 0); //CarCollectCarID
			Write32(pCarCollectInfo2, 0); //CarCollectTaskID

			len = pCarCollectInfo2 - pCarCollectInfo;
			Set16(pCarCollectInfo2, (WORD)len);
			pCarCollectInfo2 += len;
		}

		len = pCarCollectInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 2560); // ReportAntiCollisionDataTime
	Write32(p, 0);	   // Duration
	Write32(p, 0);	   // BeginCDTime
	Write32(p, 0);	   // PropInteval
	Write32(p, 0x00000f00);	   // MoveFreq

	Write8(p, 0); // HaveGameLogicTask


	Write16(p, 0); // RankedMatchBegainTipInfoType

	Write8(p, 0); // BegainTipLen

	Write8(p, 0); // BuffBum


	Write8(p, 0); // MapChallengeInfoNum


	Write8(p, 0); // IsChangeCar

	if (AnonymousMode509 || (AnonymousMode == 1 && Client->TaskID == 0xFFFC)) {
		Write8(p, CurrentPlayers - 1); // AnonymousMode
		for (size_t i = 0; i < CurrentPlayers; i++)
		{ //AnonymousModeAvatarChangeInfo
			if (Player[i] == Client)
			{
				continue;
			}
			BYTE* pAnonymousModeAvatarChangeInfo = p;
			Write16(pAnonymousModeAvatarChangeInfo, 0); //len

			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Uin); //PlayerUin
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10025 : 10042); //HairID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
			Write32(pAnonymousModeAvatarChangeInfo, 0); //SuitID
			Write32(pAnonymousModeAvatarChangeInfo, raceid); //KartID
			Write8(pAnonymousModeAvatarChangeInfo, 1); //HasItemInfo
			for (size_t j = 0; j < 1; j++)
			{ //ItemInfo
				BYTE* pItemInfo = pAnonymousModeAvatarChangeInfo;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Player[i]->Gender ? 10022 : 10018); //RoleID
				Write32(pItemInfo, Player[i]->Gender ? 10025 : 10042); //HairID
				Write32(pItemInfo, 0); //FaceID
				Write32(pItemInfo, 0); //GlassID
				Write32(pItemInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
				Write32(pItemInfo, Player[i]->Gender ? 10045 : 10032); //GloveID
				Write32(pItemInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
				Write32(pItemInfo, 0); //PersonarID
				Write32(pItemInfo, 0); //TattooID
				Write32(pItemInfo, 0); //WingID
				Write32(pItemInfo, 0); //DecoratorID
				Write32(pItemInfo, raceid); //KartID
				Write32(pItemInfo, 0); //ColorID
				Write32(pItemInfo, 0); //TrackID
				Write32(pItemInfo, 0); //EffectID
				Write32(pItemInfo, 0); //BrandID
				Write32(pItemInfo, 0); //TailID

				len = pItemInfo - pAnonymousModeAvatarChangeInfo;
				Set16(pAnonymousModeAvatarChangeInfo, (WORD)len);
				pAnonymousModeAvatarChangeInfo += len;
			}

			len = pAnonymousModeAvatarChangeInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	else {
		Write8(p, 0); //AnonymousMode  匿名模式
	}

	Write8(p, 0); // IsTimeShuttleGen

	Write8(p, 0); // HalloweenDdventureInfoNum


	Write8(p, 0); // MaxPropEffectNum
	Write8(p, 0); // IsCrossNoCollision

	RecordCheckCondNum = 3;
	Write8(p, RecordCheckCondNum); // RecordCheckCondNum
	if (RecordCheckCondNum > 0) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, 0); //CheckRelation
		Write8(pRecordCheckConds, 2); //ConditionNum
		for (size_t i = 0; i < 1; i++)
		{ //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, 0); //CheckType
			Write8(pCheckConditions, 5); //CheckParaNum
			Write32(pCheckConditions, 0x00000258); //IntParas[]
			Write32(pCheckConditions, 0xffffffff); //IntParas[]
			Write32(pCheckConditions, 0xffffffff); //IntParas[]
			Write32(pCheckConditions, 0xffffffff); //IntParas[]
			Write32(pCheckConditions, 0x00000bb8); //IntParas[]

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}
		for (size_t i = 0; i < 1; i++)
		{ //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, 0); //CheckType
			Write8(pCheckConditions, 5); //CheckParaNum
			Write32(pCheckConditions, 0x0000012c); //IntParas[]
			Write32(pCheckConditions, 0xffffffff); //IntParas[]
			Write32(pCheckConditions, 0x00000000); //IntParas[]
			Write32(pCheckConditions, 0x000003e8); //IntParas[]
			Write32(pCheckConditions, 0x000003e8); //IntParas[]

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}
		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}
	if (RecordCheckCondNum > 1) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, 1); //CheckRelation
		Write8(pRecordCheckConds, 2); //ConditionNum
		for (size_t i = 0; i < 1; i++)
		{ //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, 1); //CheckType
			Write8(pCheckConditions, 5); //CheckParaNum
			Write32(pCheckConditions, 0); //IntParas[]
			Write32(pCheckConditions, 0x00000032); //IntParas[]
			Write32(pCheckConditions, 0xffffffff); //IntParas[]
			Write32(pCheckConditions, 0xffffffff); //IntParas[]
			Write32(pCheckConditions, 0); //IntParas[]

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}
		for (size_t i = 0; i < 1; i++)
		{ //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, 1); //CheckType
			Write8(pCheckConditions, 6); //CheckParaNum
			Write32(pCheckConditions, 0);
			Write32(pCheckConditions, 0x00009c40);
			Write32(pCheckConditions, 0xffffffff);
			Write32(pCheckConditions, 0xffffffff);
			Write32(pCheckConditions, 0x00002710);
			Write32(pCheckConditions, 0);
			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}
		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}
	if (RecordCheckCondNum > 2) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, 0); //CheckRelation
		Write8(pRecordCheckConds, 1); //ConditionNum
		for (size_t i = 0; i < 1; i++)
		{ //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, 2); //CheckType
			Write8(pCheckConditions, 0); //CheckParaNum

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}

		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, GameBeginSwitchFlag); // GameBeginSwitchFlag
	memcpy(p, "\x00\x00\x00\x04\x00\x00\x00\xc8\x00\x00\x03\xe8\x00\x00\x0b\xb8\x00\x00\x00\xc8\x00\x00\x00\x00\x0e\x00\x00\x00\x0f\x00\x00",
		31);
	p += 31;


	len = p - buf.data();
	SendToClient(Client, 509, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


//马服
void NotifyGameBegin3(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player)
{
	bool AnonymousMode509 = false;
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	if (!Player.empty())
	{
		for (char i = 0; i < 6; i++)
		{
			if (Player[i])
			{
				Players[CurrentPlayers] = Player[i];
				CurrentPlayers++;
			}
		}
	}
	else
	{
		Players[0] = Client;
		CurrentPlayers = 1;
	}
	//printf("NotifyGameBegin CurrentPlayers:%d\n", CurrentPlayers);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	//Write32(p, 8000); //NormalLoadTime
	Write32(p, 20000); //NormalLoadTime 设置大一点防止有些模型加载不出来
	Write32(p, Client->MapID); //MapID
	//MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID); //GameID
	Write32(p, (DWORD)time(nullptr)); //Seed

	Write8(p, CurrentPlayers);
	//m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	//起跑线并排的玩家
	int index[] = { 2,3,1,4,0,5 };
	srand(time(NULL));
	// 打乱数组  
	shuffle(index, CurrentPlayers);
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Set32((BYTE*)&aRaceTrackOrders[index[i]], Players[i]->Uin);
	}


	Write16(p, Client->TotalRound); //TotalMapRound
	Write32(p, 0); //PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); //Record


	Write8(p, 0); //NianShouTypeNum
	/*
m_astNianShouAwardInfo[].m_cAwardInGameType
m_astNianShouAwardInfo[].m_iActIDForClient
m_astNianShouAwardInfo[].m_cPointArrayIdx
m_astNianShouAwardInfo[].m_cCurrAwardNum
m_astNianShouAwardInfo[].m_acAwardPos[]
	*/

	Write8(p, CurrentPlayers); //SyncCarNum
	for (char i = 0; i < CurrentPlayers; i++)
	{ //SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); //len

		Write32(pSyncCar, Players[i]->Uin); //PlayerUin
		if (Players[i] == Client)
		{
			Write32(pSyncCar, Players[i]->KartID);
		}
		else
		{
			if (Players[i]->IsAnonymous == 1)
			{
				AnonymousMode509 = true;
			}
			if ((Players[i]->isRace == 1 || AnonymousMode509) && Client->OBState != 1)
			{
				Write32(pSyncCar, raceid);
			}
			else {
				if (Players[i]->KartSkinID != 0)//CurCarID 这里也要查询是否有皮肤 不然对方看不见
				{
					Write32(pSyncCar, Players[i]->KartSkinID);
				}
				else
				{
					Write32(pSyncCar, Players[i]->KartID);
				}
			}

		}

		Write8(pSyncCar, 0); //HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 10); //ReportDataFlag

	typedef struct MapCheckDivInfo // MapCheckDivInfo
	{
		USHORT BeginIdx;
		USHORT EndIdx;
	}*LPREFITITEMWEIGHTTYPE;
	const static MapCheckDivInfo MapCheckDivInfoCfg[] =
	{
		//MapCheckDivInfo 地图检查分区信息
		{1,10},
		{11,18},
		{31,35},
		{46,49},
		{61,68},
	};

	constexpr unsigned char CheckDataNum = sizeof(MapCheckDivInfoCfg) / sizeof(MapCheckDivInfoCfg[0]); //
	Write8(p, CheckDataNum); //CheckDataNum
	/*
m_astMapCheckDiv[].m_shBeginIdx
m_astMapCheckDiv[].m_shEndIdx
	*/
	for (UCHAR i = 0; i < CheckDataNum; i++)
	{ //MapCheckDiv
		BYTE* pMapCheckDiv = p;
		Write16(pMapCheckDiv, 0); //len

		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].BeginIdx); //BeginIdx
		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].EndIdx); //EndIdx

		len = pMapCheckDiv - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //P2PMode
	Write8(p, 0); //TcpFrequence

	const static unsigned char acMultiInfo[] = {
		0,0,0,0,102,11,206,123,101,1,244,2,1,1,1,102,11,206,123,18,253,16,172,6,0,97,
		244,130,96,16,1,79,4,252,184,73,221,128,57,18,14,224,70,49,42,70,7,33,200,169
	};
	constexpr unsigned char MultiInfoLen = sizeof(acMultiInfo) / sizeof(acMultiInfo[0]); //

	Write8(p, MultiInfoLen); //MultiInfoLen
	memset(p, 0, MultiInfoLen);
	memcpy(p, acMultiInfo, MultiInfoLen);
	p += MultiInfoLen;

	Write8(p, 0); //FeedBackEnabled 启用反馈  这里是玩家双喷 什么技巧的提示 1是开启 0是关闭


	const static USHORT SpeedSectionLowerBoundCfg[] =
	{
		// SpeedSectionLowerBoundInfo  速度区段下限
		295,306,317,328,339,350,360,375,
	};
	constexpr unsigned char SpeedSectionNum = sizeof(SpeedSectionLowerBoundCfg) / sizeof(SpeedSectionLowerBoundCfg[0]); //

	Write8(p, SpeedSectionNum); //SpeedSectionNum
	/*
m_au16SpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < SpeedSectionNum; i++) {
		Write16(p, SpeedSectionLowerBoundCfg[i]); //SpeedSectionLowerBound[]
	}

	const static USHORT NormalSpeedSectionLowerBoundCfg[] =
	{
		//NormalSpeedSectionLowerBoundInfo  正常速度区段下限
		200,220,240,260,280,300,320,340,
	};
	constexpr unsigned char NormalSpeedSectionNum = sizeof(NormalSpeedSectionLowerBoundCfg) / sizeof(NormalSpeedSectionLowerBoundCfg[0]); //

	Write8(p, NormalSpeedSectionNum); //NormalSpeedSectionNum
	/*
m_au16NormalSpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < NormalSpeedSectionNum; i++) {
		Write16(p, NormalSpeedSectionLowerBoundCfg[i]); //NormalSpeedSectionLowerBound[]
	}

	Write8(p, 0); //MemCheckInfoNum
	/*
m_astMemCheckInfo[].m_u8MemIdxNum
m_astMemCheckInfo[].m_au32MemOffset[]
m_astMemCheckInfo[].m_u8DataType
m_astMemCheckInfo[].m_uPara
	*/

	Write8(p, 4); //ExtraInfoInterval
	Write16(p, -1); //OffsetThreshold
	Write32(p, 200); //SpeedRatioThreshold1
	Write32(p, 200); //SpeedRatioThreshold2
	Write32(p, 0); //HideTaskId
	Write8(p, 0); //HideTaskType
	Write32(p, 0); //HideTaskParam1

	Write8(p, 0); //ForceReportCPNum
	//m_au16ForceReportCP[]

	Write8(p, 0); //CliReserveFlag


	Write8(p, 1); //EnableAntiDriftCheat

	{ //AntiDriftCheatPara
		BYTE* pAntiDriftCheatPara = p;
		Write16(pAntiDriftCheatPara, 0); //len

		Write32(pAntiDriftCheatPara, 0); //MaxDriftHistoryTime
		Write32(pAntiDriftCheatPara, 0); //MinTimeInterval
		Write32(pAntiDriftCheatPara, 0); //MaxTimeInterval
		Write32(pAntiDriftCheatPara, 220); //NormalThreshold
		Write32(pAntiDriftCheatPara, 15); //JetThreshold
		Write32(pAntiDriftCheatPara, 0); //JetInterval
		Write32(pAntiDriftCheatPara, 0); //OneSideSlidingInterval

		len = pAntiDriftCheatPara - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasCrashModePara
	/*
m_astCrashModePara[].m_u8SponsorSkillNum
m_astCrashModePara[].m_astSponsorSkill[].m_u8SkillID
m_astCrashModePara[].m_astSponsorSkill[].m_iPara1
m_astCrashModePara[].m_u8InitEggNum
m_astCrashModePara[].m_u8EggRefreshInterval
m_astCrashModePara[].m_u8EggRefreshNum
m_astCrashModePara[].m_u8InitN2ONum
m_astCrashModePara[].m_u8EggSourceIncNum
m_astCrashModePara[].m_u32RandSeed
m_astCrashModePara[].m_u8PropPointRatio
m_astCrashModePara[].m_u8EggPropRatio
m_astCrashModePara[].m_u8AddN2ONumOnEggLost
	*/

	Write32(p, 0); //FizzStarTaskId
	Write8(p, 0); //FizzStarTaskType
	Write32(p, 0); //FizzStarTaskParam1
	Write32(p, 0); //LDMRecordID
	//Write32(p, 1508042); //GameSeq
	Write32(p, 3925099); //GameSeq

	Write8(p, CurrentPlayers); //PlayerNums
	for (UCHAR i = 0; i < CurrentPlayers; i++)
	{ //GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); //len

		Write32(pGameBeginPlayerInfo, Players[i]->Uin); //Uin
		//Write32(pGameBeginPlayerInfo, 0x0004DAE1); //ChumCircleID
		Write32(pGameBeginPlayerInfo, 0); //ChumCircleID

		Write8(pGameBeginPlayerInfo, 0); //SkillNums
		for (size_t i = 0; i < 0; i++)
		{
			BYTE* astChumCircleSkillInfo = pGameBeginPlayerInfo;
			Write16(astChumCircleSkillInfo, 0); //len

			if (i == 0) {
				Write32(astChumCircleSkillInfo, 1);
				Write32(astChumCircleSkillInfo, 3);
			}
			else
			{
				Write32(astChumCircleSkillInfo, 3);
				Write32(astChumCircleSkillInfo, 1);
			}

			len = astChumCircleSkillInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		/*
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillID
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillLv
		*/

		Write32(pGameBeginPlayerInfo, 0); //WorldEscapeTaskID

		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); //len


			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel


			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}

		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_ushBuffID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchFeatureID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchBuffLevel
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchIsIcon
		*/

		Write32(pGameBeginPlayerInfo, 0); //StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_iParaList[]
		*/

		Write32(pGameBeginPlayerInfo, 0);//EquipSkillId

		if (Players[i]->SpeclEffect == 1) {
			Write8(pGameBeginPlayerInfo, 3);//GLSpeclEffectNum

			for (size_t i = 0; i < 3; i++)
			{//GLSpeclEffectInfo
				BYTE* pGLSpeclEffectInfo = pGameBeginPlayerInfo;
				Write16(pGLSpeclEffectInfo, 0);//len

				Write32(pGLSpeclEffectInfo, i + 1);//SpeclEffectType
				Write32(pGLSpeclEffectInfo, 0);//Para0

				len = (WORD)(pGLSpeclEffectInfo - pGameBeginPlayerInfo);
				Set16(pGameBeginPlayerInfo, len);
				pGameBeginPlayerInfo += len;
			}
		}
		else {
			Write8(pGameBeginPlayerInfo, 0);//GLSpeclEffectNum
		}



		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //PlayeCheerNums
	Write8(p, 0); //MapNum
	Write32(p, 0); //SpecialMapId
	char NPCFlag = 0;
	char border = 0;
	if (Client->TaskID == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else
	{
		NPCFlag = 1;
	}

	if ((Client->isManMachine == 0 && border == 1) || Client->real == 1) {
		NPCFlag = 0;
	}


	if (!NPCFlag)
	{
		Write8(p, 0);//NPCNum
	}
	else
	{
		Write8(p, NpcNum); //NPCNum
		for (size_t i = 0; i < NpcNum; i++)
		{
			//NPCInfo
			BYTE* pNPCInfo = p;
			Write16(pNPCInfo, 0); //len

			Write32(pNPCInfo, NpcListInfos[i].NpcID); //NpcID
			memset(pNPCInfo, 0, 17);
			memcpy(pNPCInfo, NpcListInfos[i].RoomName, 17);
			pNPCInfo += 17;
			memset(pNPCInfo, 0, 17);//GuildName[]
			pNPCInfo += 17;
			{ //NpcItemInfo
				BYTE* pNpcItemInfo = pNPCInfo;
				Write16(pNpcItemInfo, 0); //len

				Write32(pNpcItemInfo, NpcListInfos[i].RoleID); //RoleID
				Write32(pNpcItemInfo, NpcListInfos[i].HairID); //HairID
				Write32(pNpcItemInfo, NpcListInfos[i].FaceID); //FaceID
				Write32(pNpcItemInfo, NpcListInfos[i].GlassID); //GlassID
				Write32(pNpcItemInfo, NpcListInfos[i].CoatID); //CoatID
				Write32(pNpcItemInfo, NpcListInfos[i].GloveID); //GloveID
				Write32(pNpcItemInfo, NpcListInfos[i].TrousersID); //TrousersID
				Write32(pNpcItemInfo, NpcListInfos[i].PersonarID); //PersonarID
				Write32(pNpcItemInfo, NpcListInfos[i].TattooID); //TattooID
				Write32(pNpcItemInfo, NpcListInfos[i].WingID); //WingID
				Write32(pNpcItemInfo, NpcListInfos[i].DecoratorID); //DecoratorID
				Write32(pNpcItemInfo, NpcListInfos[i].KartID); //KartID
				Write32(pNpcItemInfo, NpcListInfos[i].ColorID); //ColorID
				Write32(pNpcItemInfo, NpcListInfos[i].TrackID); //TrackID
				Write32(pNpcItemInfo, NpcListInfos[i].EffectID); //EffectID
				Write32(pNpcItemInfo, NpcListInfos[i].BrandID); //BrandID
				Write32(pNpcItemInfo, NpcListInfos[i].TailID); //TailID

				len = pNpcItemInfo - pNPCInfo;
				Set16(pNPCInfo, (WORD)len);
				pNPCInfo += len;
			}

			memcpy(pNPCInfo, "\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c", 252);
			pNPCInfo += 252;
			Write16(pNPCInfo, NpcListInfos[i].MaxLevel); //NPCAILevel
			Write16(pNPCInfo, 0); //TeamID

			len = pNPCInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//int NPCKartID[5] = {127616,127365,123501 ,118729,110710 };
		//const char* NpcName[5] = {"陈伯\x0\x0","关佳鑫","胡旭\x0\x0","江章闯","张云彬"};
	//NPC配置 //NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
	//memcpy(p, "\x01\x6e\x00\x00\x00\x01\xc9\xe7\xbb\xe1\xc3\xc0\xd1\xf2\xd1\xf2\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x59\x02\x00\x00\x00\x00\x00\x01\x88\x84\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x07\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00", 366);
	//p += 366;

	}
	//npc数据
	//"\x05\x01\x6e\x00\x00\x00\x01\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x02\xd0\xa1\xe9\xd9\xd7\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe6\x6b\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x23\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x03\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x08\x00\x00\x01\x6e\x00\x00\x00\x04\xc2\xed\xc5\xa3\xb1\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\x00\x00\x00\x01\xd2\x03\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf1\xcd\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x05\x57\x6f\x6c\x76\x65\x73\x2e\x4e\x69\x6e\x67\x51\x69\x6e\x67\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xcb\xdf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf7\xc5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00";
	Write8(p, 0); //MapNum
	int* aNPCRaceTrackOrders = (int*)p;
	for (size_t i = 0; i < MAXNPCNUMINROOM; i++)
	{
		//m_aunNPCTrackOrders[]
		Write32(p, 0);
	}
	//起跑线并排的机器人
	//int npcindex[] = { 2,3,1,4,0 };
	if (Client->TaskID != 0)
	{
		for (char i = 0; i < 5; i++)
		{
			Set32((BYTE*)&aNPCRaceTrackOrders[i], i + 1);
		}
	}

	memcpy(p, "\x00\x00\x00\x00\x02\x7A\x21\x05\x87\x57\x8C\xCC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1B\x00\x1C\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0A\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x37\x00\x00\x02\x00\x19\x00\x00\x05\x00\x00\x02\x58\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x0B\xB8\x00\x19\x00\x00\x05\x00\x00\x01\x2C\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x03\xE8\x00\x00\x03\xE8\x00\x3B\x00\x01\x02\x00\x19\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x32\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x1D\x00\x01\x06\x00\x00\x00\x00\x00\x00\x9C\x40\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00\x27\x10\x00\x00\x00\x00\x00\x13\x00\x00\x02\x00\x05\x00\x02\x00\x00\x09\x00\x03\x01\x00\x00\x00\x05\x00\x17\x00\x00\x02\x00\x09\x00\x04\x01\x00\x00\x00\x05\x00\x09\x00\x05\x01\x00\x00\x00\x0A\x00\x00\xC7\xB3\x00\x00\x00\x00\x04\x00\x00\x00\xC8\x00\x00\x03\xE8\x00\x00\x0B\xB8\x00\x00\x00\xC8\x00\x00\x00\x00\x0E\x00\x00\x00\x0F\x00\x00\x00\x00", 279);
	p += 279;

	len = p - buf.data();
	SendToClient(Client, 509, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyGameBegin4(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player)
{
	bool AnonymousMode509 = false;
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	if (!Player.empty())
	{
		for (char i = 0; i < 6; i++)
		{
			if (Player[i])
			{
				Players[CurrentPlayers] = Player[i];
				CurrentPlayers++;
			}
		}
	}
	else
	{
		Players[0] = Client;
		CurrentPlayers = 1;
	}
	//printf("NotifyGameBegin CurrentPlayers:%d\n", CurrentPlayers);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	//Write32(p, 8000); //NormalLoadTime
	Write32(p, 20000); //NormalLoadTime 设置大一点防止有些模型加载不出来
	Write32(p, Client->MapID); //MapID
	//MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID); //GameID
	Write32(p, (DWORD)time(nullptr)); //Seed

	Write8(p, CurrentPlayers);
	//m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	//起跑线并排的玩家
	int index[] = { 2,3,1,4,0,5 };
	srand(time(NULL));
	// 打乱数组  
	shuffle(index, CurrentPlayers);
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Set32((BYTE*)&aRaceTrackOrders[index[i]], Players[i]->Uin);
	}


	Write16(p, Client->TotalRound); //TotalMapRound
	Write32(p, 0); //PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); //Record


	Write8(p, 0); //NianShouTypeNum
	/*
m_astNianShouAwardInfo[].m_cAwardInGameType
m_astNianShouAwardInfo[].m_iActIDForClient
m_astNianShouAwardInfo[].m_cPointArrayIdx
m_astNianShouAwardInfo[].m_cCurrAwardNum
m_astNianShouAwardInfo[].m_acAwardPos[]
	*/

	Write8(p, CurrentPlayers); //SyncCarNum
	for (char i = 0; i < CurrentPlayers; i++)
	{ //SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); //len

		Write32(pSyncCar, Players[i]->Uin); //PlayerUin
		if (Players[i] == Client)
		{
			Write32(pSyncCar, Players[i]->KartID);
		}
		else
		{
			if (Players[i]->IsAnonymous == 1)
			{
				AnonymousMode509 = true;
			}
			if ((Players[i]->isRace == 1 || AnonymousMode509) && Client->OBState != 1)
			{
				Write32(pSyncCar, raceid);
			}
			else {
				if (Players[i]->KartSkinID != 0)//CurCarID 这里也要查询是否有皮肤 不然对方看不见
				{
					Write32(pSyncCar, Players[i]->KartSkinID);
				}
				else
				{
					Write32(pSyncCar, Players[i]->KartID);
				}
			}

		}

		Write8(pSyncCar, 0); //HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 1); //ReportDataFlag

	typedef struct MapCheckDivInfo // MapCheckDivInfo
	{
		USHORT BeginIdx;
		USHORT EndIdx;
	}*LPREFITITEMWEIGHTTYPE;
	const static MapCheckDivInfo MapCheckDivInfoCfg[] =
	{
		//MapCheckDivInfo 地图检查分区信息
		{1,10},
		{11,18},
		{31,35},
		{46,49},
		{61,68},
	};

	constexpr unsigned char CheckDataNum = sizeof(MapCheckDivInfoCfg) / sizeof(MapCheckDivInfoCfg[0]); //
	Write8(p, CheckDataNum); //CheckDataNum
	/*
m_astMapCheckDiv[].m_shBeginIdx
m_astMapCheckDiv[].m_shEndIdx
	*/
	for (UCHAR i = 0; i < CheckDataNum; i++)
	{ //MapCheckDiv
		BYTE* pMapCheckDiv = p;
		Write16(pMapCheckDiv, 0); //len

		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].BeginIdx); //BeginIdx
		Write16(pMapCheckDiv, MapCheckDivInfoCfg[i].EndIdx); //EndIdx

		len = pMapCheckDiv - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 1); //P2PMode
	Write8(p, 0); //TcpFrequence

	const static unsigned char acMultiInfo[] = {
		0,0,0,0,102,11,206,123,101,1,244,2,1,1,1,102,11,206,123,18,253,16,172,6,0,97,
		244,130,96,16,1,79,4,252,184,73,221,128,57,18,14,224,70,49,42,70,7,33,200,169
	};
	constexpr unsigned char MultiInfoLen = sizeof(acMultiInfo) / sizeof(acMultiInfo[0]); //

	Write8(p, MultiInfoLen); //MultiInfoLen
	memset(p, 0, MultiInfoLen);
	memcpy(p, acMultiInfo, MultiInfoLen);
	p += MultiInfoLen;

	Write8(p, 0); //FeedBackEnabled 启用反馈  这里是玩家双喷 什么技巧的提示 1是开启 0是关闭


	const static USHORT SpeedSectionLowerBoundCfg[] =
	{
		// SpeedSectionLowerBoundInfo  速度区段下限
		331,362,394,426,458,489,499,514
	};
	constexpr unsigned char SpeedSectionNum = sizeof(SpeedSectionLowerBoundCfg) / sizeof(SpeedSectionLowerBoundCfg[0]); //

	Write8(p, SpeedSectionNum); //SpeedSectionNum
	/*
m_au16SpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < SpeedSectionNum; i++) {
		Write16(p, SpeedSectionLowerBoundCfg[i]); //SpeedSectionLowerBound[]
	}

	const static USHORT NormalSpeedSectionLowerBoundCfg[] =
	{
		//NormalSpeedSectionLowerBoundInfo  正常速度区段下限
		200,220,240,260,280,300,320,340,
	};
	constexpr unsigned char NormalSpeedSectionNum = sizeof(NormalSpeedSectionLowerBoundCfg) / sizeof(NormalSpeedSectionLowerBoundCfg[0]); //

	Write8(p, NormalSpeedSectionNum); //NormalSpeedSectionNum
	/*
m_au16NormalSpeedSectionLowerBound[]
	*/
	for (UCHAR i = 0; i < NormalSpeedSectionNum; i++) {
		Write16(p, NormalSpeedSectionLowerBoundCfg[i]); //NormalSpeedSectionLowerBound[]
	}

	Write8(p, 0); //MemCheckInfoNum
	/*
m_astMemCheckInfo[].m_u8MemIdxNum
m_astMemCheckInfo[].m_au32MemOffset[]
m_astMemCheckInfo[].m_u8DataType
m_astMemCheckInfo[].m_uPara
	*/

	Write8(p, 4); //ExtraInfoInterval
	Write16(p, -1); //OffsetThreshold
	Write32(p, 200); //SpeedRatioThreshold1
	Write32(p, 200); //SpeedRatioThreshold2
	Write32(p, 0); //HideTaskId
	Write8(p, 0); //HideTaskType
	Write32(p, 0); //HideTaskParam1

	Write8(p, 0); //ForceReportCPNum
	//m_au16ForceReportCP[]

	Write8(p, 0); //CliReserveFlag


	Write8(p, 1); //EnableAntiDriftCheat

	{ //AntiDriftCheatPara
		BYTE* pAntiDriftCheatPara = p;
		Write16(pAntiDriftCheatPara, 0); //len

		Write32(pAntiDriftCheatPara, 0); //MaxDriftHistoryTime
		Write32(pAntiDriftCheatPara, 0); //MinTimeInterval
		Write32(pAntiDriftCheatPara, 0); //MaxTimeInterval
		Write32(pAntiDriftCheatPara, 220); //NormalThreshold
		Write32(pAntiDriftCheatPara, 15); //JetThreshold
		Write32(pAntiDriftCheatPara, 0); //JetInterval
		Write32(pAntiDriftCheatPara, 0); //OneSideSlidingInterval

		len = pAntiDriftCheatPara - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasCrashModePara
	/*
m_astCrashModePara[].m_u8SponsorSkillNum
m_astCrashModePara[].m_astSponsorSkill[].m_u8SkillID
m_astCrashModePara[].m_astSponsorSkill[].m_iPara1
m_astCrashModePara[].m_u8InitEggNum
m_astCrashModePara[].m_u8EggRefreshInterval
m_astCrashModePara[].m_u8EggRefreshNum
m_astCrashModePara[].m_u8InitN2ONum
m_astCrashModePara[].m_u8EggSourceIncNum
m_astCrashModePara[].m_u32RandSeed
m_astCrashModePara[].m_u8PropPointRatio
m_astCrashModePara[].m_u8EggPropRatio
m_astCrashModePara[].m_u8AddN2ONumOnEggLost
	*/

	Write32(p, 0); //FizzStarTaskId
	Write8(p, 0); //FizzStarTaskType
	Write32(p, 0); //FizzStarTaskParam1
	Write32(p, 0); //LDMRecordID
	//Write32(p, 1508042); //GameSeq
	Write32(p, 10684781); //GameSeq

	Write8(p, CurrentPlayers); //PlayerNums
	for (UCHAR i = 0; i < CurrentPlayers; i++)
	{ //GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); //len

		Write32(pGameBeginPlayerInfo, Players[i]->Uin); //Uin
		//Write32(pGameBeginPlayerInfo, 0x0004DAE1); //ChumCircleID
		Write32(pGameBeginPlayerInfo, 0); //ChumCircleID

		Write8(pGameBeginPlayerInfo, 0); //SkillNums
		for (size_t i = 0; i < 0; i++)
		{
			BYTE* astChumCircleSkillInfo = pGameBeginPlayerInfo;
			Write16(astChumCircleSkillInfo, 0); //len

			if (i == 0) {
				Write32(astChumCircleSkillInfo, 1);
				Write32(astChumCircleSkillInfo, 3);
			}
			else
			{
				Write32(astChumCircleSkillInfo, 3);
				Write32(astChumCircleSkillInfo, 1);
			}

			len = astChumCircleSkillInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		/*
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillID
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillLv
		*/

		Write32(pGameBeginPlayerInfo, 0); //WorldEscapeTaskID

		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); //len


			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel


			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}

		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_ushBuffID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchFeatureID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchBuffLevel
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchIsIcon
		*/

		Write32(pGameBeginPlayerInfo, 0); //StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_iParaList[]
		*/

		Write32(pGameBeginPlayerInfo, 0);//EquipSkillId

		if (Players[i]->SpeclEffect == 1) {
			Write8(pGameBeginPlayerInfo, 3);//GLSpeclEffectNum

			for (size_t i = 0; i < 3; i++)
			{//GLSpeclEffectInfo
				BYTE* pGLSpeclEffectInfo = pGameBeginPlayerInfo;
				Write16(pGLSpeclEffectInfo, 0);//len

				Write32(pGLSpeclEffectInfo, i + 1);//SpeclEffectType
				Write32(pGLSpeclEffectInfo, 0);//Para0

				len = (WORD)(pGLSpeclEffectInfo - pGameBeginPlayerInfo);
				Set16(pGameBeginPlayerInfo, len);
				pGameBeginPlayerInfo += len;
			}
		}
		else {
			Write8(pGameBeginPlayerInfo, 0);//GLSpeclEffectNum
		}



		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //PlayeCheerNums
	Write8(p, 0); //MapNum
	Write32(p, 0); //SpecialMapId
	char NPCFlag = 0;
	char border = 0;
	if (Client->TaskID == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else
	{
		NPCFlag = 1;
	}

	if ((Client->isManMachine == 0 && border == 1) || Client->real == 1) {
		NPCFlag = 0;
	}


	if (!NPCFlag)
	{
		Write8(p, 0);//NPCNum
	}
	else
	{
		Write8(p, NpcNum); //NPCNum
		for (size_t i = 0; i < NpcNum; i++)
		{
			//NPCInfo
			BYTE* pNPCInfo = p;
			Write16(pNPCInfo, 0); //len

			Write32(pNPCInfo, NpcListInfos[i].NpcID); //NpcID
			memset(pNPCInfo, 0, 17);
			memcpy(pNPCInfo, NpcListInfos[i].RoomName, 17);
			pNPCInfo += 17;
			memset(pNPCInfo, 0, 17);//GuildName[]
			pNPCInfo += 17;
			{ //NpcItemInfo
				BYTE* pNpcItemInfo = pNPCInfo;
				Write16(pNpcItemInfo, 0); //len

				Write32(pNpcItemInfo, NpcListInfos[i].RoleID); //RoleID
				Write32(pNpcItemInfo, NpcListInfos[i].HairID); //HairID
				Write32(pNpcItemInfo, NpcListInfos[i].FaceID); //FaceID
				Write32(pNpcItemInfo, NpcListInfos[i].GlassID); //GlassID
				Write32(pNpcItemInfo, NpcListInfos[i].CoatID); //CoatID
				Write32(pNpcItemInfo, NpcListInfos[i].GloveID); //GloveID
				Write32(pNpcItemInfo, NpcListInfos[i].TrousersID); //TrousersID
				Write32(pNpcItemInfo, NpcListInfos[i].PersonarID); //PersonarID
				Write32(pNpcItemInfo, NpcListInfos[i].TattooID); //TattooID
				Write32(pNpcItemInfo, NpcListInfos[i].WingID); //WingID
				Write32(pNpcItemInfo, NpcListInfos[i].DecoratorID); //DecoratorID
				Write32(pNpcItemInfo, NpcListInfos[i].KartID); //KartID
				Write32(pNpcItemInfo, NpcListInfos[i].ColorID); //ColorID
				Write32(pNpcItemInfo, NpcListInfos[i].TrackID); //TrackID
				Write32(pNpcItemInfo, NpcListInfos[i].EffectID); //EffectID
				Write32(pNpcItemInfo, NpcListInfos[i].BrandID); //BrandID
				Write32(pNpcItemInfo, NpcListInfos[i].TailID); //TailID

				len = pNpcItemInfo - pNPCInfo;
				Set16(pNPCInfo, (WORD)len);
				pNPCInfo += len;
			}

			memcpy(pNPCInfo, "\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c", 252);
			pNPCInfo += 252;
			Write16(pNPCInfo, NpcListInfos[i].MaxLevel); //NPCAILevel
			Write16(pNPCInfo, 0); //TeamID

			len = pNPCInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//int NPCKartID[5] = {127616,127365,123501 ,118729,110710 };
		//const char* NpcName[5] = {"陈伯\x0\x0","关佳鑫","胡旭\x0\x0","江章闯","张云彬"};
	//NPC配置 //NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
	//memcpy(p, "\x01\x6e\x00\x00\x00\x01\xc9\xe7\xbb\xe1\xc3\xc0\xd1\xf2\xd1\xf2\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x59\x02\x00\x00\x00\x00\x00\x01\x88\x84\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x07\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00", 366);
	//p += 366;

	}
	//npc数据
	//"\x05\x01\x6e\x00\x00\x00\x01\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x02\xd0\xa1\xe9\xd9\xd7\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe6\x6b\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x23\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x03\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x08\x00\x00\x01\x6e\x00\x00\x00\x04\xc2\xed\xc5\xa3\xb1\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\x00\x00\x00\x01\xd2\x03\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf1\xcd\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x05\x57\x6f\x6c\x76\x65\x73\x2e\x4e\x69\x6e\x67\x51\x69\x6e\x67\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xcb\xdf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf7\xc5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00";
	Write8(p, 0); //MapNum
	int* aNPCRaceTrackOrders = (int*)p;
	for (size_t i = 0; i < MAXNPCNUMINROOM; i++)
	{
		//m_aunNPCTrackOrders[]
		Write32(p, 0);
	}
	//起跑线并排的机器人
	//int npcindex[] = { 2,3,1,4,0 };
	if (Client->TaskID != 0)
	{
		for (char i = 0; i < 5; i++)
		{
			Set32((BYTE*)&aNPCRaceTrackOrders[i], i + 1);
		}
	}

	Write8(p, 0); //PlayerNum
	Write8(p, 0); //HasTowerInfo
	Write8(p, 0); //HasWeRelayGameBeginInfo

	Write8(p, 1); //ChangeCar
	Write32(p, 46272068); //GameSeqIDHigh	12345678
	Write32(p, 1510247268); //GameSeqIDLow	87654321
	Write32(p, 0); //KubiBigCoinReplaceItem
	Write32(p, 0); //TimerChallengeJumpLevel
	Write8(p, 0); //ShadowRunDelay
	Write16(p, 0); //ShadowCatchUpContinuesTime

	Write8(p, 0); //ArrestPlayerNums
	/*
m_astArrestRoleInfo[].m_uiUin
m_astArrestRoleInfo[].m_bTeamID
	*/

	Write16(p, 12); //MonitorCheckPointBegin
	Write16(p, 13); //MonitorCheckPointEnd

	Write8(p, 1); //MonitorLapCnt
	Write8(p, 0); //GameType
	Write16(p, 0); //PointID
	Write16(p, 1); //BaseMode
	Write16(p, 1); //SubMode
	Write8(p, 0); //GameType
	Write16(p, 0); //ReportPosSpan

	Write16(p, 0); //PropID
	Write32(p, 0); //PropIndex
	Write16(p, 0); //PropNum


	Write8(p, 1); //BaseGameModeEx
	Write8(p, 0); //ParaNum

	Write8(p, 0); //MapCheckpointFileIndex

	Write8(p, 0); //HasTimerChallenge2ndGameBeginInfo
	/*
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iPlayerNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_uiUin
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iWeekHighestTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iBonusJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iHistoryMaxTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iJumpLevel
m_astTimerChallenge2ndGameBeginInfo[].m_uiBeginUin
m_astTimerChallenge2ndGameBeginInfo[].m_bReverse
	*/

	Write8(p, 0); //HasGameStageInfo
	/*
m_stGameStageInfo[].m_u8StageIndex
m_stGameStageInfo[].m_u8TotalStageNum
	*/

	Write8(p, 0); //CarCollectInfoNum
	/*
m_stCarCollectInfo[].m_ushPlayerNum
m_stCarCollectInfo[].m_stCarCollectInfo[].m_uiUin
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write16(p, 3000); //ReportAntiCollisionDataTime 报告防冲突数据时间
	Write32(p, 0); //Duration
	Write32(p, 0); //BeginCDTime
	Write32(p, 0); //PropInteval
	Write32(p, 0x00000f00); //MoveFreq

	Write8(p, 0); //HaveGameLogicTask
	/*
m_astRakedMatchGamelogicTaskInfo[].m_uiGameLogicTaskType
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskFinishCondValue
m_astRakedMatchGamelogicTaskInfo[].m_ucJudgeRule
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskAddGradeScore
m_astRakedMatchGamelogicTaskInfo[].m_uchGradeScoreRangeNum
m_astRakedMatchGamelogicTaskInfo[].m_auiGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskFinishCondValueNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskFinishCondValueRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchFailedAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiFailedAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_ucFinishTask
m_astRakedMatchGamelogicTaskInfo[].m_uiExtraFinishTaskAddScore
	*/

	Write16(p, 0); //RankedMatchBegainTipInfoType

	Write16(p, 0); //BegainTipLen

	Write8(p, 0); //BuffBum
	/*
m_stEffectBuffInfo[].m_ushBuffID
m_stEffectBuffInfo[].m_uchFeatureID
m_stEffectBuffInfo[].m_uchBuffLevel
m_stEffectBuffInfo[].m_uchIsIcon
	*/

	Write8(p, 0); //MapChallengeInfoNum
	/*
m_stMapChallengeInfo[].m_uchStarInfoNum
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uiTime
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_bType
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_ushCount
m_stMapChallengeInfo[].m_stAssessInfo.m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_ushMsgLen
m_stMapChallengeInfo[].m_iMapID
*/

	Write8(p, 0); //IsChangeCar

	if (AnonymousMode509 || (AnonymousMode == 1 && Client->TaskID == 0xFFFC)) {
		Write8(p, CurrentPlayers - 1); // AnonymousMode
		for (size_t i = 0; i < CurrentPlayers; i++)
		{ //AnonymousModeAvatarChangeInfo
			if (Player[i] == Client)
			{
				continue;
			}
			BYTE* pAnonymousModeAvatarChangeInfo = p;
			Write16(pAnonymousModeAvatarChangeInfo, 0); //len

			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Uin); //PlayerUin
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10025 : 10042); //HairID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
			Write32(pAnonymousModeAvatarChangeInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
			Write32(pAnonymousModeAvatarChangeInfo, 0); //SuitID
			Write32(pAnonymousModeAvatarChangeInfo, raceid); //KartID
			Write8(pAnonymousModeAvatarChangeInfo, 1); //HasItemInfo
			for (size_t j = 0; j < 1; j++)
			{ //ItemInfo
				BYTE* pItemInfo = pAnonymousModeAvatarChangeInfo;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Player[i]->Gender ? 10022 : 10018); //RoleID
				Write32(pItemInfo, Player[i]->Gender ? 10025 : 10042); //HairID
				Write32(pItemInfo, 0); //FaceID
				Write32(pItemInfo, 0); //GlassID
				Write32(pItemInfo, Player[i]->Gender ? 10043 : 10050); //CoatID
				Write32(pItemInfo, Player[i]->Gender ? 10045 : 10032); //GloveID
				Write32(pItemInfo, Player[i]->Gender ? 10044 : 10051); //TrousersID
				Write32(pItemInfo, 0); //PersonarID
				Write32(pItemInfo, 0); //TattooID
				Write32(pItemInfo, 0); //WingID
				Write32(pItemInfo, 0); //DecoratorID
				Write32(pItemInfo, raceid); //KartID
				Write32(pItemInfo, 0); //ColorID
				Write32(pItemInfo, 0); //TrackID
				Write32(pItemInfo, 0); //EffectID
				Write32(pItemInfo, 0); //BrandID
				Write32(pItemInfo, 0); //TailID

				len = pItemInfo - pAnonymousModeAvatarChangeInfo;
				Set16(pAnonymousModeAvatarChangeInfo, (WORD)len);
				pAnonymousModeAvatarChangeInfo += len;
			}

			len = pAnonymousModeAvatarChangeInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	else {
		Write8(p, 0); //AnonymousMode  匿名模式
	}
	/*
	m_astAnonymousModeAvatarChangeInfo[].m_dwPlayerUin
	m_astAnonymousModeAvatarChangeInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_iSuitID
	m_astAnonymousModeAvatarChangeInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_uchHasItemInfo
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iRoleID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iFaceID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGlassID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGloveID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iPersonarID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTattooID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iWingID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iDecoratorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iColorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrackID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iEffectID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iBrandID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTailID
	*/

	Write8(p, 0); //IsTimeShuttleGen

	Write8(p, 0); //HalloweenDdventureInfoNum
	/*
m_stHalloweenDdventureInfo[].m_ushPlayerNum
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_uiUin
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write8(p, 7); //MaxPropEffectNum
	Write8(p, 1); //IsCrossNoCollision	交叉没有碰撞

	typedef struct CheckConditionInfo // CheckConditionInfo
	{
		USHORT CheckType;
		UCHAR CheckParaNum;
		UINT IntParas[11];
	}*LPCheckConditionInfo;
	typedef struct RecordCheckCondInfo // RecordCheckCondInfo
	{
		USHORT CheckRelation;
		UCHAR ConditionNum;
		CheckConditionInfo CheckCondition[11];
	}*LPRecordCheckCondInfo;
	const static RecordCheckCondInfo RecordCheckCondCfg[] =
	{
		//RecordCheckCondInfo
		{0,2,{{0,5,{600,-1,-1,-1,3000}},{0,5,{300,-1,0,1000,1000}}}},
		{1,2,{{0,5,{0,50,-1,-1,0}},{1,6,{0,40000,-1,-1,10000,0}}}},
		{0,1,{{2,0}}},
	};
	constexpr unsigned char RecordCheckCondNum = sizeof(RecordCheckCondCfg) / sizeof(RecordCheckCondCfg[0]); //

	Write8(p, RecordCheckCondNum); //RecordCheckCondNum
	/*
m_astRecordCheckConds[].m_usCheckRelation
m_astRecordCheckConds[].m_ucConditionNum
m_astRecordCheckConds[].m_astCheckConditions[].m_usCheckType
m_astRecordCheckConds[].m_astCheckConditions[].m_ucCheckParaNum
m_astRecordCheckConds[].m_astCheckConditions[].m_aIntParas[]
	*/
	for (UCHAR j = 0; j < RecordCheckCondNum; j++) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, RecordCheckCondCfg[j].CheckRelation); //CheckRelation
		Write8(pRecordCheckConds, RecordCheckCondCfg[j].ConditionNum); //ConditionNum
		for (UCHAR i = 0; i < RecordCheckCondCfg[j].ConditionNum; i++) { //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckType); //CheckType
			Write8(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum); //CheckParaNum
			for (size_t x = 0; x < RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum; x++) {
				Write32(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].IntParas[x]); //IntParas[]
			}

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}

		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 4019); //GameBeginSwitchFlag 1971		947
	Write8(p, 0); //TriggerVeggieDogTask

	Write8(p, 0); //HasQSpeedCrystalInfo
	/*
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalID
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalLevel
m_stQSpeedCrystalInfo[].m_uchQSpeedCrystalBuffBum
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_ushBuffID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchFeatureID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchBuffLevel
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchIsIcon
	*/


	Write16(p, 0);//BorderRaceType

	constexpr static unsigned int RaceSyncParaList[] = { 200 ,1000 ,3000 ,200 };
	constexpr unsigned char RaceSyncParaNum = sizeof(RaceSyncParaList) / sizeof(RaceSyncParaList[0]); //

	Write8(p, RaceSyncParaNum);//RaceSyncParaNum
	for (const auto& RaceSyncPara : RaceSyncParaList) {
		Write32(p, RaceSyncPara);//RaceSyncParaList[]
	}

	Write8(p, 0);//QingHuaMysteryGroundDataNum
	Write32(p, 14);//SpeclEffectSwitchFlag  特效开启标志
	Write32(p, 15);
	Write32(p, 0);


	len = p - buf.data();
	SendToClient(Client, 509, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyGameBeginCS(std::shared_ptr<ClientNode> Client)
{

	bool AnonymousMode509 = false;
	char CurrentPlayers = 1;


	//printf("NotifyGameBegin CurrentPlayers:%d\n", CurrentPlayers);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	//Write32(p, 8000); //NormalLoadTime
	Write32(p, 20000); //NormalLoadTime 设置大一点防止有些模型加载不出来
	Write32(p, Client->MapID); //MapID
	//MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID); //GameID
	Write32(p, (DWORD)time(nullptr)); //Seed

	Write8(p, CurrentPlayers);
	//m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	//起跑线并排的玩家
	int index[] = { 2,3,1,4,0,5 };
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Set32((BYTE*)&aRaceTrackOrders[index[i]], Client->Uin);
	}


	Write16(p, Client->TotalRound); //TotalMapRound
	Write32(p, 0); //PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); //Record


	Write8(p, 0); //NianShouTypeNum
	/*
m_astNianShouAwardInfo[].m_cAwardInGameType
m_astNianShouAwardInfo[].m_iActIDForClient
m_astNianShouAwardInfo[].m_cPointArrayIdx
m_astNianShouAwardInfo[].m_cCurrAwardNum
m_astNianShouAwardInfo[].m_acAwardPos[]
	*/

	Write8(p, CurrentPlayers); //SyncCarNum
	for (char i = 0; i < CurrentPlayers; i++)
	{ //SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); //len

		Write32(pSyncCar, Client->Uin); //PlayerUin
		Write32(pSyncCar, Client->KartID);

		Write8(pSyncCar, 0); //HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //ReportDataFlag


	Write8(p, 0); //CheckDataNum
	/*
m_astMapCheckDiv[].m_shBeginIdx
m_astMapCheckDiv[].m_shEndIdx
	*/

	Write8(p, 0); //P2PMode
	Write8(p, 0); //TcpFrequence



	Write8(p, 50); //MultiInfoLen
	WriteHex(p, "000000016700FDEE040363040A01016700FDEE2258E52E01816B178ABC43BBDE7BD1AEEADEF57F0049A106F4B5A7AB13393F");

	Write8(p, 1); //FeedBackEnabled 启用反馈  这里是玩家双喷 什么技巧的提示 1是开启 0是关闭

	Write8(p, 0); //SpeedSectionNum
	/*
m_au16SpeedSectionLowerBound[]
	*/
	Write8(p, 0); //NormalSpeedSectionNum
	/*
m_au16NormalSpeedSectionLowerBound[]
	*/


	Write8(p, 0); //MemCheckInfoNum
	/*
m_astMemCheckInfo[].m_u8MemIdxNum
m_astMemCheckInfo[].m_au32MemOffset[]
m_astMemCheckInfo[].m_u8DataType
m_astMemCheckInfo[].m_uPara
	*/

	Write8(p, 4); //ExtraInfoInterval
	Write16(p, -1); //OffsetThreshold
	Write32(p, 200); //SpeedRatioThreshold1
	Write32(p, 200); //SpeedRatioThreshold2
	Write32(p, 0); //HideTaskId
	Write8(p, 0); //HideTaskType
	Write32(p, 0); //HideTaskParam1

	Write8(p, 0); //ForceReportCPNum
	//m_au16ForceReportCP[]

	Write8(p, 1); //CliReserveFlag


	Write8(p, 1); //EnableAntiDriftCheat

	{ //AntiDriftCheatPara
		BYTE* pAntiDriftCheatPara = p;
		Write16(pAntiDriftCheatPara, 0); //len

		Write32(pAntiDriftCheatPara, 0); //MaxDriftHistoryTime
		Write32(pAntiDriftCheatPara, 0); //MinTimeInterval
		Write32(pAntiDriftCheatPara, 0); //MaxTimeInterval
		Write32(pAntiDriftCheatPara, 220); //NormalThreshold
		Write32(pAntiDriftCheatPara, 15); //JetThreshold
		Write32(pAntiDriftCheatPara, 0); //JetInterval
		Write32(pAntiDriftCheatPara, 0); //OneSideSlidingInterval

		len = pAntiDriftCheatPara - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasCrashModePara
	/*
m_astCrashModePara[].m_u8SponsorSkillNum
m_astCrashModePara[].m_astSponsorSkill[].m_u8SkillID
m_astCrashModePara[].m_astSponsorSkill[].m_iPara1
m_astCrashModePara[].m_u8InitEggNum
m_astCrashModePara[].m_u8EggRefreshInterval
m_astCrashModePara[].m_u8EggRefreshNum
m_astCrashModePara[].m_u8InitN2ONum
m_astCrashModePara[].m_u8EggSourceIncNum
m_astCrashModePara[].m_u32RandSeed
m_astCrashModePara[].m_u8PropPointRatio
m_astCrashModePara[].m_u8EggPropRatio
m_astCrashModePara[].m_u8AddN2ONumOnEggLost
	*/

	Write32(p, 0); //FizzStarTaskId
	Write8(p, 0); //FizzStarTaskType
	Write32(p, 0); //FizzStarTaskParam1
	Write32(p, 0); //LDMRecordID
	//Write32(p, 1508042); //GameSeq
	Write32(p, 3925099); //GameSeq

	Write8(p, CurrentPlayers); //PlayerNums
	for (UCHAR i = 0; i < CurrentPlayers; i++)
	{ //GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); //len

		Write32(pGameBeginPlayerInfo, Client->Uin); //Uin
		//Write32(pGameBeginPlayerInfo, 0x0004DAE1); //ChumCircleID
		Write32(pGameBeginPlayerInfo, 0); //ChumCircleID

		Write8(pGameBeginPlayerInfo, 1); //SkillNums
		for (size_t i = 0; i < 1; i++)
		{
			BYTE* astChumCircleSkillInfo = pGameBeginPlayerInfo;
			Write16(astChumCircleSkillInfo, 0); //len

			if (i == 0) {
				Write32(astChumCircleSkillInfo, 1);
				Write32(astChumCircleSkillInfo, 2);
			}
			else
			{
				Write32(astChumCircleSkillInfo, 3);
				Write32(astChumCircleSkillInfo, 1);
			}

			len = astChumCircleSkillInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		/*
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillID
m_astGameBeginPlayerInfo[].m_astChumCircleSkillInfo[].m_uiSkillLv
		*/

		Write32(pGameBeginPlayerInfo, 0); //WorldEscapeTaskID

		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); //len


			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel


			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}

		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_ushBuffID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchFeatureID
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchBuffLevel
m_astGameBeginPlayerInfo[].m_stEffectGameBuffInfo[].m_uchIsIcon
		*/

		Write32(pGameBeginPlayerInfo, 0); //StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0); //ParaNum
		/*
m_astGameBeginPlayerInfo[].m_iParaList[]
		*/

		Write32(pGameBeginPlayerInfo, 0);//EquipSkillId

		if (Client->SpeclEffect == 1) {
			Write8(pGameBeginPlayerInfo, 3);//GLSpeclEffectNum

			for (size_t i = 0; i < 3; i++)
			{//GLSpeclEffectInfo
				BYTE* pGLSpeclEffectInfo = pGameBeginPlayerInfo;
				Write16(pGLSpeclEffectInfo, 0);//len

				Write32(pGLSpeclEffectInfo, i + 1);//SpeclEffectType
				Write32(pGLSpeclEffectInfo, 0);//Para0

				len = (WORD)(pGLSpeclEffectInfo - pGameBeginPlayerInfo);
				Set16(pGameBeginPlayerInfo, len);
				pGameBeginPlayerInfo += len;
			}
		}
		else {
			Write8(pGameBeginPlayerInfo, 0);//GLSpeclEffectNum
		}



		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //PlayeCheerNums
	Write8(p, 0); //MapNum
	Write32(p, 0); //SpecialMapId
	char NPCFlag = 0;
	char border = 0;
	if (Client->TaskID == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFC && Rankset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 0)
	{
		NPCFlag = 0;
	}
	else if (Client->TaskID == 0xFFFD && Borderset[1] == 1)
	{
		NPCFlag = 1;
		border = 1;
	}
	else
	{
		NPCFlag = 1;
	}

	if ((Client->isManMachine == 0 && border == 1) || Client->real == 1) {
		NPCFlag = 0;
	}


	if (!NPCFlag)
	{
		Write8(p, 0);//NPCNum
	}
	else
	{
		Write8(p, NpcNum); //NPCNum
		for (size_t i = 0; i < NpcNum; i++)
		{
			//NPCInfo
			BYTE* pNPCInfo = p;
			Write16(pNPCInfo, 0); //len

			Write32(pNPCInfo, NpcListInfos[i].NpcID); //NpcID
			memset(pNPCInfo, 0, 17);
			memcpy(pNPCInfo, NpcListInfos[i].RoomName, 17);
			pNPCInfo += 17;
			memset(pNPCInfo, 0, 17);//GuildName[]
			pNPCInfo += 17;
			{ //NpcItemInfo
				BYTE* pNpcItemInfo = pNPCInfo;
				Write16(pNpcItemInfo, 0); //len

				Write32(pNpcItemInfo, NpcListInfos[i].RoleID); //RoleID
				Write32(pNpcItemInfo, NpcListInfos[i].HairID); //HairID
				Write32(pNpcItemInfo, NpcListInfos[i].FaceID); //FaceID
				Write32(pNpcItemInfo, NpcListInfos[i].GlassID); //GlassID
				Write32(pNpcItemInfo, NpcListInfos[i].CoatID); //CoatID
				Write32(pNpcItemInfo, NpcListInfos[i].GloveID); //GloveID
				Write32(pNpcItemInfo, NpcListInfos[i].TrousersID); //TrousersID
				Write32(pNpcItemInfo, NpcListInfos[i].PersonarID); //PersonarID
				Write32(pNpcItemInfo, NpcListInfos[i].TattooID); //TattooID
				Write32(pNpcItemInfo, NpcListInfos[i].WingID); //WingID
				Write32(pNpcItemInfo, NpcListInfos[i].DecoratorID); //DecoratorID
				Write32(pNpcItemInfo, NpcListInfos[i].KartID); //KartID
				Write32(pNpcItemInfo, NpcListInfos[i].ColorID); //ColorID
				Write32(pNpcItemInfo, NpcListInfos[i].TrackID); //TrackID
				Write32(pNpcItemInfo, NpcListInfos[i].EffectID); //EffectID
				Write32(pNpcItemInfo, NpcListInfos[i].BrandID); //BrandID
				Write32(pNpcItemInfo, NpcListInfos[i].TailID); //TailID

				len = pNpcItemInfo - pNPCInfo;
				Set16(pNPCInfo, (WORD)len);
				pNPCInfo += len;
			}

			memcpy(pNPCInfo, "\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c", 252);
			pNPCInfo += 252;
			Write16(pNPCInfo, NpcListInfos[i].MaxLevel); //NPCAILevel
			Write16(pNPCInfo, 0); //TeamID

			len = pNPCInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//int NPCKartID[5] = {127616,127365,123501 ,118729,110710 };
		//const char* NpcName[5] = {"陈伯\x0\x0","关佳鑫","胡旭\x0\x0","江章闯","张云彬"};
	//NPC配置 //NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
	//memcpy(p, "\x01\x6e\x00\x00\x00\x01\xc9\xe7\xbb\xe1\xc3\xc0\xd1\xf2\xd1\xf2\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x59\x02\x00\x00\x00\x00\x00\x01\x88\x84\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x07\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00", 366);
	//p += 366;

	}
	//npc数据
	//"\x05\x01\x6e\x00\x00\x00\x01\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x02\xd0\xa1\xe9\xd9\xd7\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe6\x6b\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x23\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x03\xcd\xe1\xb1\xf3\xc7\xfa\xcc\xab\xe0\xcb\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x63\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xf0\xbf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf2\x80\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x08\x00\x00\x01\x6e\x00\x00\x00\x04\xc2\xed\xc5\xa3\xb1\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\x00\x00\x00\x01\xd2\x03\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xf1\xcd\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x0a\x00\x00\x01\x6e\x00\x00\x00\x05\x57\x6f\x6c\x76\x65\x73\x2e\x4e\x69\x6e\x67\x51\x69\x6e\x67\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x00\x00\x00\x01\xfe\x89\x00\x00\x00\x00\x00\x01\xcb\xdf\x00\x00\x27\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf7\xc5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x22\x00\x00\x04\xe2\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x00\x00\x03\xe8\x00\x00\x02\x58\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x09\x00\x00";
	Write8(p, 0); //MapNum
	int* aNPCRaceTrackOrders = (int*)p;
	for (size_t i = 0; i < MAXNPCNUMINROOM; i++)
	{
		//m_aunNPCTrackOrders[]
		Write32(p, 0);
	}
	//起跑线并排的机器人
	//int npcindex[] = { 2,3,1,4,0 };
	if (Client->TaskID != 0)
	{
		for (char i = 0; i < 5; i++)
		{
			Set32((BYTE*)&aNPCRaceTrackOrders[i], i + 1);
		}
	}

	Write8(p, 0); //PlayerNum
	Write8(p, 0); //HasTowerInfo
	Write8(p, 0); //HasWeRelayGameBeginInfo

	Write8(p, 1); //ChangeCar
	Write32(p, 0x01F0D892); //GameSeqIDHigh	12345678
	Write32(p, 0x3325F51C); //GameSeqIDLow	87654321
	Write32(p, 0); //KubiBigCoinReplaceItem
	Write32(p, 0); //TimerChallengeJumpLevel
	Write8(p, 0); //ShadowRunDelay
	Write16(p, 0); //ShadowCatchUpContinuesTime

	Write8(p, 0); //ArrestPlayerNums
	/*
m_astArrestRoleInfo[].m_uiUin
m_astArrestRoleInfo[].m_bTeamID
	*/

	Write16(p, 0x000a); //MonitorCheckPointBegin
	Write16(p, 0x000b); //MonitorCheckPointEnd

	Write8(p, 1); //MonitorLapCnt
	Write8(p, 0); //GameType
	Write16(p, 0); //PointID
	Write16(p, 1); //BaseMode
	Write16(p, 1); //SubMode
	Write8(p, 0); //GameType
	Write16(p, 0); //ReportPosSpan

	Write16(p, 0); //PropID
	Write32(p, 0); //PropIndex
	Write16(p, 0); //PropNum


	Write8(p, 1); //BaseGameModeEx
	Write8(p, 0); //ParaNum

	Write8(p, 0); //MapCheckpointFileIndex

	Write8(p, 0); //HasTimerChallenge2ndGameBeginInfo
	/*
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iPlayerNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_uiUin
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iWeekHighestTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iBonusJumplevel
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_astPlayerJumpInfo[].m_iHistoryMaxTongGuanNum
m_astTimerChallenge2ndGameBeginInfo[].m_stJumpInfo.m_iJumpLevel
m_astTimerChallenge2ndGameBeginInfo[].m_uiBeginUin
m_astTimerChallenge2ndGameBeginInfo[].m_bReverse
	*/

	Write8(p, 0); //HasGameStageInfo
	/*
m_stGameStageInfo[].m_u8StageIndex
m_stGameStageInfo[].m_u8TotalStageNum
	*/

	Write8(p, 0); //CarCollectInfoNum
	/*
m_stCarCollectInfo[].m_ushPlayerNum
m_stCarCollectInfo[].m_stCarCollectInfo[].m_uiUin
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stCarCollectInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write16(p, 2560); //ReportAntiCollisionDataTime 报告防冲突数据时间
	Write32(p, 0); //Duration
	Write32(p, 0); //BeginCDTime
	Write32(p, 0); //PropInteval
	Write32(p, 0x00000f00); //MoveFreq

	Write8(p, 0); //HaveGameLogicTask
	/*
m_astRakedMatchGamelogicTaskInfo[].m_uiGameLogicTaskType
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskFinishCondValue
m_astRakedMatchGamelogicTaskInfo[].m_ucJudgeRule
m_astRakedMatchGamelogicTaskInfo[].m_uiTaskAddGradeScore
m_astRakedMatchGamelogicTaskInfo[].m_uchGradeScoreRangeNum
m_astRakedMatchGamelogicTaskInfo[].m_auiGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskFinishCondValueNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskFinishCondValueRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchTaskAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiTaskAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_uchFailedAddGradeScoreNum
m_astRakedMatchGamelogicTaskInfo[].m_auiFailedAddGradeScoreRange[]
m_astRakedMatchGamelogicTaskInfo[].m_ucFinishTask
m_astRakedMatchGamelogicTaskInfo[].m_uiExtraFinishTaskAddScore
	*/

	Write16(p, 0); //RankedMatchBegainTipInfoType

	Write16(p, 0); //BegainTipLen

	Write8(p, 0); //BuffBum
	/*
m_stEffectBuffInfo[].m_ushBuffID
m_stEffectBuffInfo[].m_uchFeatureID
m_stEffectBuffInfo[].m_uchBuffLevel
m_stEffectBuffInfo[].m_uchIsIcon
	*/

	Write8(p, 0); //MapChallengeInfoNum
	/*
m_stMapChallengeInfo[].m_uchStarInfoNum
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uiTime
m_stMapChallengeInfo[].m_astMapChallengeStarInfo[].m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_bType
m_stMapChallengeInfo[].m_stAssessInfo.m_stMapChallenge_AssessInfo.m_ushCount
m_stMapChallengeInfo[].m_stAssessInfo.m_uchStatus
m_stMapChallengeInfo[].m_stAssessInfo.m_ushMsgLen
m_stMapChallengeInfo[].m_iMapID
*/

	Write8(p, 0); //IsChangeCar

	Write8(p, 0); //AnonymousMode  匿名模式
	/*
	m_astAnonymousModeAvatarChangeInfo[].m_dwPlayerUin
	m_astAnonymousModeAvatarChangeInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_iSuitID
	m_astAnonymousModeAvatarChangeInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_uchHasItemInfo
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iRoleID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iHairID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iFaceID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGlassID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iCoatID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iGloveID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrousersID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iPersonarID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTattooID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iWingID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iDecoratorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iKartID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iColorID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTrackID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iEffectID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iBrandID
	m_astAnonymousModeAvatarChangeInfo[].m_stItemInfo[].m_iTailID
	*/

	Write8(p, 0); //IsTimeShuttleGen

	Write8(p, 0); //HalloweenDdventureInfoNum
	/*
m_stHalloweenDdventureInfo[].m_ushPlayerNum
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_uiUin
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectCarID
m_stHalloweenDdventureInfo[].m_stCarCollectInfo[].m_iCarCollectTaskID
	*/

	Write8(p, 0); //MaxPropEffectNum
	Write8(p, 0); //IsCrossNoCollision	交叉没有碰撞

	typedef struct CheckConditionInfo // CheckConditionInfo
	{
		USHORT CheckType;
		UCHAR CheckParaNum;
		UINT IntParas[11];
	}*LPCheckConditionInfo;
	typedef struct RecordCheckCondInfo // RecordCheckCondInfo
	{
		USHORT CheckRelation;
		UCHAR ConditionNum;
		CheckConditionInfo CheckCondition[11];
	}*LPRecordCheckCondInfo;
	const static RecordCheckCondInfo RecordCheckCondCfg[] =
	{
		//RecordCheckCondInfo
		{0,2,{{0,5,{600,-1,-1,-1,3000}},{0,5,{300,-1,0,1000,1000}}}},
		{1,2,{{0,5,{0,50,-1,-1,0}},{1,6,{0,40000,-1,-1,10000,0}}}},
		{0,1,{{2,0}}},
	};
	constexpr unsigned char RecordCheckCondNum = sizeof(RecordCheckCondCfg) / sizeof(RecordCheckCondCfg[0]); //

	Write8(p, RecordCheckCondNum); //RecordCheckCondNum
	/*
m_astRecordCheckConds[].m_usCheckRelation
m_astRecordCheckConds[].m_ucConditionNum
m_astRecordCheckConds[].m_astCheckConditions[].m_usCheckType
m_astRecordCheckConds[].m_astCheckConditions[].m_ucCheckParaNum
m_astRecordCheckConds[].m_astCheckConditions[].m_aIntParas[]
	*/
	for (UCHAR j = 0; j < RecordCheckCondNum; j++) { //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, RecordCheckCondCfg[j].CheckRelation); //CheckRelation
		Write8(pRecordCheckConds, RecordCheckCondCfg[j].ConditionNum); //ConditionNum
		for (UCHAR i = 0; i < RecordCheckCondCfg[j].ConditionNum; i++) { //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckType); //CheckType
			Write8(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum); //CheckParaNum
			for (size_t x = 0; x < RecordCheckCondCfg[j].CheckCondition[i].CheckParaNum; x++) {
				Write32(pCheckConditions, RecordCheckCondCfg[j].CheckCondition[i].IntParas[x]); //IntParas[]
			}

			len = pCheckConditions - pRecordCheckConds;
			Set16(pRecordCheckConds, (WORD)len);
			pRecordCheckConds += len;
		}

		len = pRecordCheckConds - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 1971); //GameBeginSwitchFlag 1971		947
	Write8(p, 0); //TriggerVeggieDogTask

	Write8(p, 0); //HasQSpeedCrystalInfo
	/*
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalID
m_stQSpeedCrystalInfo[].m_iQSpeedCrystalLevel
m_stQSpeedCrystalInfo[].m_uchQSpeedCrystalBuffBum
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_ushBuffID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchFeatureID
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchBuffLevel
m_stQSpeedCrystalInfo[].m_stQSpeedCrystalEffectBuffInfo[].m_uchIsIcon
	*/


	Write16(p, 0);//BorderRaceType

	constexpr static unsigned int RaceSyncParaList[] = { 200 ,1000 ,3000 ,200 };
	constexpr unsigned char RaceSyncParaNum = sizeof(RaceSyncParaList) / sizeof(RaceSyncParaList[0]); //

	Write8(p, RaceSyncParaNum);//RaceSyncParaNum
	for (const auto& RaceSyncPara : RaceSyncParaList) {
		Write32(p, RaceSyncPara);//RaceSyncParaList[]
	}

	Write8(p, 0);//QingHuaMysteryGroundDataNum
	Write32(p, 14);//SpeclEffectSwitchFlag  特效开启标志
	Write32(p, 15);


	len = p - buf.data();
	SendToClient(Client, 509, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


void NotifyEncryptedMsg(std::shared_ptr<ClientNode> Client)
{
	/*if (KartMode == 2) {
		return;
	}*/

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	UINT LoadID = Client->KartID;

	//Client->lastUpdateTime = std::chrono::steady_clock::time_point{};
	Client->SuperN2O = 0;
	Client->SuperJet = 0;
	Client->getPropNum = 0;
	Client->SuperJetKartFuelInc = 0;
	Client->CurrentAccelerateFuel = 0;
	Client->PropID = 0;

	//UINT PhysInfosize;
	bool IsMySkinRefitEffect = false;
	Client->IsRefitItem = false;
	auto it = MySkinRefitEffectInfoMap.find(Client->KartSkinID);
	if (it != MySkinRefitEffectInfoMap.end()) {
		//判断是否为特殊皮肤
		IsMySkinRefitEffect = true;
	}
	if (Client->Uin > 10000) {
		if (Client->KartSkinID == 127616) {
			//PhysInfosize += 1;
			Client->IsRefitItem = true;
		}
		if (IsMySkinRefitEffect) {
			//++PhysInfosize;
			Client->IsRefitItem = true;
		}
		if (Client->KartSkinID > 0) {
			LoadID = Client->KartSkinID;
		}
	}

	auto it1 = std::find(modecar.begin(), modecar.end(), LoadID);
	if (it1 != modecar.end()) {
		WriteKartPhysParam28346(Client);
		//delete[]buf;
		return;
	}

	{
		Write32(p, Client->Uin);//Uin
		Write32(p, (DWORD)time(nullptr));//Time
		Write16(p, 551);//MsgID
		{
			EncryptKartItem* pEncryptKartItem = GetEncryptKartItem(LoadID);
			memcpy(p, pEncryptKartItem->EncryptData.data(), pEncryptKartItem->EncryptData.size());
			p += pEncryptKartItem->EncryptData.size();

		}
	}


	len = (WORD)(p - buf.data());
	SendToClient(Client, 28346, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);

}

void NotifyKartPhysParam(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player)
{

	if (mode28346 == 2 && Client->Uin > 10000) {
		WriteKartPhysParam28346(Client);
		return;
	}

	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	if (!Player.empty())
	{
		for (char i = 0; i < 6; i++)
		{
			if (Player[i])
			{
				Players[CurrentPlayers] = Player[i];
				CurrentPlayers++;
			}
		}
	}
	else
	{
		Players[0] = Client;
		CurrentPlayers = 1;
	}


	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	int erri{};

	try {
		Write16(p, CurrentPlayers); //KartNum
		for (char i = 0; i < CurrentPlayers; i++) { //KartPhysParam

			//printf("Uin:%u KartID:%u KartSkinID:%u\n",Players[i]->Uin, Players[i]->KartID,Client->KartSkinID);
			erri = i;
			WriteKartPhysParam(p, Players[i]->Uin, Players[i]->KartID, Players[i]);
		}
	}
	catch (...) {

		auto ServerLogger = spdlog::get("SpeedServer_logger");
		//ServerLogger->info("[赛车文件异常][Uin:{:d}][ConnID:{:d}][MapID:{:d}]", Players[erri]->Uin, Players[erri]->KartID, Players[erri]->KartSkinID);
		ServerLogger->info("[赛车文件异常][Uin:{:d}][KartID:{:d}][KartSkinID:{:d}]", Client->Uin, Client->KartID, Client->KartSkinID);
		ServerLogger->flush();

		printf("赛车文件异常1!!!\n");


	}
	Write16(p, 0xFFFF); //SuperN2ORate

	len = p - buf.data();
	SendToClient(Client, 551, buf.data(), len, Client->GameID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestPrepareReady(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
#ifndef ZingSpeed
	//int MapID = Read32(Body);
	//char MapHash[32] = ReadString(Body);
	//UCHAR SingleGameRestartFlag = Read8(Body);
#endif

	Client->IsReady = TRUE;
	Client->IsFinish = FALSE;

	//等待所有人准备 并比较最大超时时间
	//std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	//if (!Room)
	//{
	//	return;
	//}
}

void NotifyRaceBegin(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, 9000); //CountDownTime
	Write16(p, 0); //DelayTime
#ifndef ZingSpeed
	Write8(p, 0); //StageIndex
	Write32(p, 0); //RaceCountDownTime
	Write32(p, 0); //RaceCountDownDis
	Write8(p, 0); //UseNewCountDownTime
	Write32(p, 0); //NewCountDownTime
	Write32(p, (DWORD)time(nullptr)); //ServerSecond
	Write32(p, 0); //ServerMicroSecond
#endif

	len = p - buf.data();
	SendToClient(Client, 511, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyPlayerFinishRace(std::shared_ptr<ClientNode> Client, UINT Uin, bool NewRecord, int FinTime)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write8(p, NewRecord); //NewRecord
	Write32(p, FinTime); //FinTime

	len = p - buf.data();
	SendToClient(Client, 522, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


void NotifyKickPlayer(std::shared_ptr<ClientNode> Client, const char* Reason, UINT KickType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write16(p, 0);			 // ReasonID
	Write32(p, Client->Uin); // SrcUin
	len = strlen(Reason);
	Write8(p, len);			// ResourceLen
	memcpy(p, Reason, len); // Resource 
	p += len;
	Write8(p, KickType); // AntiCheatMode
	Write8(p, KickType); // KickType

	len = p - buf.data();
	SendToClient(Client, 900, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestReportCurrentInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//UINT Uin = Read32(Body);
	//UINT Time = Read32(Body);
	//printf("Uin:%d\n", Uin);
	//UINT CurrentState = Read32(Body);

	//m_aiCurrentPosition[3]
	//DWORD a=Read32(Body);
	//DWORD b = Read32(Body);
	//DWORD c = Read32(Body);
	Body += 24;
	WORD len;
	//fflush(stdout);
	BOOL IsFinish = FALSE;
	BYTE PassedCheckPointNum = Read8(Body);
	//printf("PassedCheckPointNum:%d\n", PassedCheckPointNum);
	for (size_t i = 0; i < PassedCheckPointNum; i++)
	{
		WORD PassedCheckPointID = Read16(Body);
		//printf("当前圈数:%d 通过地图检查点:%d\n", Client->Round,PassedCheckPointID);
		//fflush(stdout);
		/*if (Client->Loop)
		{
			if (Client->EndCheckPoint != 0 && PassedCheckPointID > Client->EndCheckPoint) //大于终点的则可能在走近道
			{
				//printf("continue\t");
				continue;
			}
			if (PassedCheckPointID == 0 && Client->PassedCheckPoint > 4)
			{
				Client->Round++;

				if (Client->Round >= Client->TotalRound)
				{
					IsFinish = TRUE;
				}
			}
			else if (Client->PassedCheckPoint == 0 && PassedCheckPointID > 4)
			{
				Client->Round--;
			}
		}
		else
		{
			if (PassedCheckPointID == Client->EndCheckPoint)
			{
				Client->Round++;
				if (Client->Round >= Client->TotalRound)
				{
					IsFinish = TRUE;
				}
			}
		}
		Client->PassedCheckPoint = PassedCheckPointID;
		*/
		if (Client->TaskID == 0xFFFF) {
			// printf("%d\t", PassedCheckPointID);
			if (Client->Loop)
			{
				if (Client->EndCheckPoint != 0 && PassedCheckPointID > Client->EndCheckPoint) // 大于终点的则可能在走近道
				{
					// printf("continue\t");
					continue;
				}
				if (PassedCheckPointID == 0 && Client->PassedCheckPoint > 4)
				{
					Client->Round++;
					// Client->PassedCheckPointPropS.clear();
					if (Client->Round >= Client->TotalRound)
					{
						IsFinish = TRUE;
					}
				}
				// 修复阿拉丁冲线不结算 day 20220419
				else if (Client->PassedCheckPoint == 0 && PassedCheckPointID > 4)
					// else if (Client->PassedCheckPoint == 0 && PassedCheckPointID > 4 && Client->MapID != 373)
				{
					Client->Round--;
				}
			}
			else
			{
				if (PassedCheckPointID == Client->EndCheckPoint)
				{
					Client->Round++;
					// Client->PassedCheckPointPropS.clear();
					if (Client->Round >= Client->TotalRound)
					{
						IsFinish = TRUE;
					}
				}
			}
			Client->PassedCheckPoint = PassedCheckPointID;
		}

	}

	UINT LapTime = Read32(Body);

	if (Client->TaskID == 0xFFFF)
	{
		Client->LapTime = LapTime;
		if (findLevel(Client->PassedCheckPoint, Client->MapID) > Client->JumpLevel || Client->MAXLapTime == 0) {
			BYTE buf[8192] = { 0 };
			BYTE* p = buf;
			size_t len = 0;
			Write32(p, 3);
			if (Client->MAXLapTime != 0)
			{
				Client->JumpLevel++;
			}
			Write32(p, Client->JumpLevel);
			Client->MAXLapTime == 0 ? Client->MAXLapTime += 0x4e20 : Client->MAXLapTime += 9900;
			if ((Client->JumpLevel == 120 && Client->MapID == 431) || (Client->JumpLevel == 150 && Client->MapID == 432) || (Client->JumpLevel == 150 && Client->MapID == 452))
			{
				Write32(p, 0);//TimeAdd
				Write32(p, 0);//TimeNow
				Write32(p, 0); //RoundRaceLeftTime
			}
			else
			{
				Write32(p, Client->MAXLapTime == 0x4e20 ? 0 : 9900);//TimeAdd
				Write32(p, Client->MAXLapTime - LapTime * 10);//TimeNow
				Write32(p, Client->MAXLapTime - LapTime * 10); //RoundRaceLeftTime
			}
			Write32(p, 0); //StartJumpLevel
			Write32(p, 0); //HandoverUin
			Write32(p, 0); //HandoverPlayTime
			Write32(p, 0); //HandoverJumpLevel
			Write32(p, 0); //HandoverJumpTime
			Write32(p, 0); //ZuiHouYiBoLevel
			Write32(p, 0); //ZuiHouYiBoTime
			Write32(p, 0); //PlaneTime
			Write32(p, 0); //PlaneNum
			Write32(p, 0); //PlaneEnergy
			Write32(p, 0); //PlaneEnergyAdd
			Write32(p, 0); //PlaneEnergyMax
			Write32(p, 0); //HasResult
			const char* buf2 = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
			memcpy(p, buf2, 11);
			SendToClient(Client, 24255, (BYTE*)buf, 91, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}
		if (Client->MAXLapTime < Client->LapTime * 10 || (Client->JumpLevel == 120 && Client->MapID == 431) || (Client->JumpLevel == 150 && Client->MapID == 432) || (Client->JumpLevel == 150 && Client->MapID == 452))
		{
			//Client->matchQueueType = 0;
			Client->IsFinish = TRUE;
			//结束
			auto ClientPtr = new std::shared_ptr<ClientNode>(Client);
			HANDLE over = CreateThread(NULL, 0, OnSingleOver, ClientPtr, 0, NULL);
			if (over)
			{
				CloseHandle(over);
			}
			return;
		}
	}

	BYTE AccVerifyDataNum = Read8(Body);
	for (size_t i = 0; i < AccVerifyDataNum; i++)
	{
		/*
m_astAccelVerifyData[].m_unType
m_astAccelVerifyData[].m_ushTotalCount
m_astAccelVerifyData[].m_unMaxDurTime
m_astAccelVerifyData[].m_ushOverlayCount
		*/
		Body += Get16(Body);
	}

	BYTE MsgSequence = Read8(Body);


	BYTE ExtraDataNum = Read8(Body);
	for (size_t i = 0; i < ExtraDataNum; i++)
	{
		/*
m_astExtraData[].stSkillStoneTriggerData.m_uTriggerTotalCount
m_astExtraData[].stSkillStoneTriggerData.m_uCheckData1
m_astExtraData[].stSkillStoneTriggerData.m_uCheckData2
m_astExtraData[].m_stClientReportSelfErr.m_iRemoveFailedCount
m_astExtraData[].m_stClientReportSelfErr.m_iNo2CheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iFrictionCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iGPTargetCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iPowerCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iStoneCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iHashCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iTencentCarCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iUseIlleagleNo2Count
m_astExtraData[].m_stClientReportSelfErr.m_iListAffectGPsCheckErrCount
m_astExtraData[].m_stClientReportSelfErr.m_iGPTargetInvalidErrCount
m_astExtraData[].m_stClientReportSelfErr.m_aiErrData[ENMCRET_MAX]
m_astExtraData[].m_stClientReportSeflTag.m_iPowerCheckTag
m_astExtraData[].m_stClientReportSeflTag.m_iNo2CheckTag
m_astExtraData[].m_stClientReportSeflTag.m_iTencentCarCheckTag
m_astExtraData[].m_stClientReportSeflTag.m_iGPTargetInvalidCheckTag
m_astExtraData[].m_u32JetCombinationFlag
m_astExtraData[].m_unMaxSpeed
m_astExtraData[].m_u32CollisionNumOnLastCheckPoint
m_astExtraData[].m_uiTotalDrift
m_astExtraData[].m_bySpeedSectionNum
m_astExtraData[].m_auSpeedSectionAccTime[]
m_astExtraData[].m_byNormalSpeedSectionNum
m_astExtraData[].m_auNormalSpeedSectionAccTime[]
m_astExtraData[].m_u8MemCheckResultNum
m_astExtraData[].m_au32MemData[]
m_astExtraData[].m_auClientCheckErr[ECCET_MAX]
m_astExtraData[].m_auClientCheckErrTag[ECCETT_MAX]
m_astExtraData[].m_u8UpdateCount
m_astExtraData[].m_u8OffsetOverThresholdCount
m_astExtraData[].m_u32OffsetOverThresholdSum
m_astExtraData[].m_u16MaxOffsetSpeed
m_astExtraData[].m_u16AvgOffsetSpeed
m_astExtraData[].m_u16MaxRealSpeed
m_astExtraData[].m_u16AvgRealSpeed
m_astExtraData[].m_u32MaxSpeedRatio
m_astExtraData[].m_u16SpeedRatioOverThresholdCount
m_astExtraData[].m_u32MaxSpeedRatio2
m_astExtraData[].m_u16SpeedRatioOverThresholdCount2
m_astExtraData[].m_u32FBSkillRebornTime
m_astExtraData[].m_u32FBSkillChargeTime
m_astExtraData[].m_uiTotalPerpetualNight
m_astExtraData[].m_iRPCSFlag
m_astExtraData[].m_u32CollisionPlayerNum
m_astExtraData[].m_u32CollisionNumOnPlayer
m_astExtraData[].m_iCodeItegrity
m_astExtraData[].m_iSendLaPkg
m_astExtraData[].m_iSendDataToSvr
m_astExtraData[].m_aiAbnormalData[4]
m_astExtraData[].m_stLevelInfo.m_cLowFPSRatio
m_astExtraData[].m_stLevelInfo.m_cAveFPS
m_astExtraData[].m_stLevelInfo.m_iMemUsed
m_astExtraData[].m_stLevelInfo.m_iVMemUsed
m_astExtraData[].m_stLevelInfo.m_iGameSetting
m_astExtraData[].m_uiPositionErrorCount
m_astExtraData[].m_uiDropToDeathSurfaceNums
m_astExtraData[].m_uiCollideNumsInN2OStat
m_astExtraData[].m_uiBeyondPlayerNums
		*/
		Body += Get16(Body);
	}

	BYTE N2OChangeEventNum = Read8(Body);
	for (size_t i = 0; i < N2OChangeEventNum; i++)
	{
		/*
m_astN2OChangeEvent[].m_u16EventID
m_astN2OChangeEvent[].m_u32BeginLapTime
m_astN2OChangeEvent[].m_u16LastingTime
m_astN2OChangeEvent[].m_u8EventType
m_astN2OChangeEvent[].m_i16N2OChange
		*/
		Body += Get16(Body);
	}

	UINT Flag = Read32(Body);

	BYTE HasCrashModeData = Read8(Body);
	if (HasCrashModeData)
	{
		//m_astCrashModeData[].m_u8CurEggNum
		Body += Get16(Body);
	}

	BYTE HasPointChallengeData = Read8(Body);
	if (HasPointChallengeData)
	{
		/*
m_astPointChallengeData[].m_uiEatCoin
m_astPointChallengeData[].m_uiBeforeTime
m_astPointChallengeData[].m_uiBeforeLong
m_astPointChallengeData[].m_uiCollectN2
m_astPointChallengeData[].m_uiShuangpen
m_astPointChallengeData[].m_uiKongpen
m_astPointChallengeData[].m_uiLuodipen
m_astPointChallengeData[].m_uiNPCRunAfterTime
m_astPointChallengeData[].m_uiLeiYinCrazyJet
		*/
		Body += Get16(Body);
	}

	UINT SkateCoinNum = Read32(Body);
	for (size_t i = 0; i < SkateCoinNum; i++)
	{
		/*
m_astSkateCoinData[].m_uiCoinID
m_astSkateCoinData[].m_uiPlayerPosX
m_astSkateCoinData[].m_uiPlayerPosY
m_astSkateCoinData[].m_uiPlayerPosZ
m_u32SkateComboAwardCoinNum
m_astSkateComboAwardCoinData[].m_ushComboNum
m_astSkateComboAwardCoinData[].m_ushCoinNum
		*/
		Body += Get16(Body);
	}


	UINT SkateComboAwardCoinNum = Read32(Body);
	for (size_t i = 0; i < SkateComboAwardCoinNum; i++)
	{
		/*
m_astSkateComboAwardCoinData[].m_ushComboNum
m_astSkateComboAwardCoinData[].m_ushCoinNum
		*/
		Body += Get16(Body);
	}

	UINT SkateDoubleJumpCount = Read32(Body);
	UINT CurrMapID = Read32(Body);


	BYTE HasGameDetailRecord = Read8(Body);
	if (HasGameDetailRecord)
	{
		BYTE* pGameDetailRecord = Body;
		len = Read16(pGameDetailRecord);

		UINT Uin = Read32(pGameDetailRecord);
		std::string NickName = ReadString(pGameDetailRecord, 17);
		int KartId = Read32(pGameDetailRecord);
		UINT FinTime = Read32(pGameDetailRecord);
		UINT HighestSpeed = Read32(pGameDetailRecord);
		UINT AverageSpeed = Read32(pGameDetailRecord);
		UINT CollisionCount = Read32(pGameDetailRecord);
		UINT N2OGatherCount = Read32(pGameDetailRecord);
		UINT JetTime = Read32(pGameDetailRecord);
		UINT ChaoJiQiBu = Read32(pGameDetailRecord);
		UINT ZhuangQiangDianPen = Read32(pGameDetailRecord);
		UINT DriftCount = Read32(pGameDetailRecord);
		UINT SmallJetCount = Read32(pGameDetailRecord);
		UINT DoubleJetCount = Read32(pGameDetailRecord);
		UINT FlyJetCount = Read32(pGameDetailRecord);
		UINT FallJetCount = Read32(pGameDetailRecord);
		UINT DuanWeiQiBu = Read32(pGameDetailRecord);
		UINT DuanWeiDoubleJetCount = Read32(pGameDetailRecord);
		UINT LinkJetCount = Read32(pGameDetailRecord);
		UINT WCJetCount = Read32(pGameDetailRecord);
		UINT CeShenDriftCount = Read32(pGameDetailRecord);
		UINT ShuaiWeiDriftCount = Read32(pGameDetailRecord);
		UINT CWWJetCount = Read32(pGameDetailRecord);
		UINT XiaoGuaCount = Read32(pGameDetailRecord);
		UINT teamID = Read32(pGameDetailRecord);
		UINT FastCornersNum = Read32(pGameDetailRecord);
		USHORT DiffCornerNum = Read16(pGameDetailRecord);
		UINT FastCornerDetailInfo = Read32(pGameDetailRecord);
		USHORT WSingleJetCount = Read16(pGameDetailRecord);
		USHORT WCWJetCount = Read16(pGameDetailRecord);
		USHORT CWWJetCount1 = Read16(pGameDetailRecord);
		USHORT CeShenJetCount = Read16(pGameDetailRecord);
		USHORT ShuaiWeiJetCount = Read16(pGameDetailRecord);
		short RankedMatchGrade = Read16(pGameDetailRecord);
		USHORT DuanWeiPiaoYi = Read16(pGameDetailRecord);
		USHORT DuanWeiJet = Read16(pGameDetailRecord);
		USHORT OutOfTurnQuickly = Read16(pGameDetailRecord);
		USHORT Launch = Read16(pGameDetailRecord);
		USHORT HitWaterDrift = Read16(pGameDetailRecord);
		USHORT OutWaterJet = Read16(pGameDetailRecord);
		USHORT N2DriftContinue = Read16(pGameDetailRecord);
		USHORT WaveFlyJet = Read16(pGameDetailRecord);
		USHORT WorldID = Read16(pGameDetailRecord);
		UCHAR Npc = Read8(pGameDetailRecord);
		UINT GameSeq = Read32(pGameDetailRecord);
		UINT Time = Read32(pGameDetailRecord);

		Body += len;
		Client->HighestSpeed = HighestSpeed;
		Client->CollisionCount = CollisionCount;
		Client->N2OGatherCount = N2OGatherCount;
		Client->DriftCount = DriftCount;

		//Body += Get16(Body);
	}


	BYTE CurrentInfoFlag = Read8(Body);
	UINT TotalAccelerateFuel = Read32(Body);
	UINT DstNPCID = Read32(Body);
	UINT DistanceToEnd = Read32(Body);
	//std::cout << "DistanceToEnd：" << DistanceToEnd << std::endl;

	//int GansterProcess = Read32(Body);
	//int TotalGangsterPKCount = Read32(Body);
	//UCHAR HasTowerChallengeData = Read8(Body);
	//for (size_t i = 0; i < HasTowerChallengeData; i++)
	//{ //TowerChallengeData
	//	BYTE* pTowerChallengeData = Body;
	//	len = Read16(pTowerChallengeData);

	//	UINT EatCoin = Read32(pTowerChallengeData);
	//	UINT CollisionCount = Read32(pTowerChallengeData);
	//	UINT DriftCount = Read32(pTowerChallengeData);
	//	char KartStatus = Read8(pTowerChallengeData);
	//	char TrackElementNum = Read8(pTowerChallengeData);
	//	for (size_t i = 0; i < TrackElementNum; i++)
	//	{ //TriggerInfo
	//		BYTE* pTriggerInfo = pTowerChallengeData;
	//		len = Read16(pTriggerInfo);

	//		char Type = Read8(pTriggerInfo);
	//		char InitCheckPoint = Read8(pTriggerInfo);
	//		char PlayerTriggerCheckPoint = Read8(pTriggerInfo);

	//		pTowerChallengeData += len;
	//	}

	//	Body += len;
	//}
	//UCHAR CurrentSelfRank = Read8(Body);
	//UCHAR CurrentTeammateRank = Read8(Body);
	//UCHAR HasP2PStatusData = Read8(Body);
	//for (size_t i = 0; i < HasP2PStatusData; i++)
	//{ //P2PStatusData
	//	BYTE* pP2PStatusData = Body;
	//	len = Read16(pP2PStatusData);

	//	UINT Uin = Read32(pP2PStatusData);
	//	UCHAR ServerUDPLoginStaus = Read8(pP2PStatusData);
	//	//std::cout << "ServerUDPLoginStaus：" << ServerUDPLoginStaus << std::endl;
	//	UCHAR PlayerNum = Read8(pP2PStatusData);
	//	for (size_t i = 0; i < PlayerNum; i++)
	//	{ //P2PStatus2Player
	//		BYTE* pP2PStatus2Player = pP2PStatusData;
	//		len = Read16(pP2PStatus2Player);

	//		UINT Uin = Read32(pP2PStatus2Player);
	//		UCHAR P2PConnectState = Read8(pP2PStatus2Player);
	//		UCHAR P2PCanPass = Read8(pP2PStatus2Player);
	//		UINT P2PNetSpeed = Read32(pP2PStatus2Player);
	//		UINT NetSpeed = Read32(pP2PStatus2Player);
	//		UINT NotPassStateCount = Read32(pP2PStatus2Player);
	//		UINT SendDataCount = Read32(pP2PStatus2Player);
	//		UINT RecvDataCount = Read32(pP2PStatus2Player);
	//		UINT ServerRecvDataCount = Read32(pP2PStatus2Player);
	//		USHORT RecvPkgCount = Read16(pP2PStatus2Player);
	//		for (size_t i = 0; i < RecvPkgCount; i++)
	//		{ //RecvPkgInfo
	//			BYTE* pRecvPkgInfo = pP2PStatus2Player;
	//			len = Read16(pRecvPkgInfo);

	//			UINT PkgID = Read32(pRecvPkgInfo);
	//			UINT RecvTime = Read32(pRecvPkgInfo);
	//			UINT DistanceDiff = Read32(pRecvPkgInfo);

	//			pP2PStatus2Player += len;
	//		}
	//		UCHAR DetailCount = Read8(pP2PStatus2Player);
	//		for (size_t i = 0; i < DetailCount; i++)
	//		{ //DetailInfo
	//			BYTE* pDetailInfo = pP2PStatus2Player;
	//			len = Read16(pDetailInfo);

	//			UCHAR Type = Read8(pDetailInfo);
	//			UINT MsgID = Read32(pDetailInfo);
	//			UINT Count = Read32(pDetailInfo);

	//			pP2PStatus2Player += len;
	//		}

	//		pP2PStatusData += len;
	//	}
	//	USHORT SendPkgCount = Read16(pP2PStatusData);
	//	for (size_t i = 0; i < SendPkgCount; i++)
	//	{ //SendPkgInfo
	//		BYTE* pSendPkgInfo = pP2PStatusData;
	//		len = Read16(pSendPkgInfo);

	//		UINT PkgID = Read32(pSendPkgInfo);
	//		UINT SendTime = Read32(pSendPkgInfo);
	//		UCHAR UDPStatus = Read8(pSendPkgInfo);
	//		UCHAR SendDataMethod = Read8(pSendPkgInfo);

	//		pP2PStatusData += len;
	//	}
	//	UINT RaceBeginTime = Read32(pP2PStatusData);
	//	//std::cout << "RaceBeginTime：" << RaceBeginTime << std::endl;
	//	UINT LagCnt = Read32(pP2PStatusData);
	//	UINT LagCntGlobal = Read32(pP2PStatusData);
	//	UINT TickInLevel = Read32(pP2PStatusData);

	//	Body += len;
	//}
	//UINT DistanceToFirstRacer = Read32(Body);


	//std::cout<<"DistanceToFirstRacer：" << DistanceToFirstRacer << std::endl;
	//UINT TimerChallengeRecoverNum = Read32(Body);
	//UINT CoinNum = Read32(Body);
	//UCHAR ClientPlayerNum = Read8(Body);
	//for (size_t i = 0; i < ClientPlayerNum; i++)
	//{ //ClientGameInfo
	//	BYTE* pClientGameInfo = Body;
	//	len = Read16(pClientGameInfo);

	//	UINT Uin = Read32(pClientGameInfo);
	//	UCHAR DistanceNum = Read8(pClientGameInfo);
	//	for (size_t i = 0; i < DistanceNum; i++)
	//	{ //DistanceToEndInfo
	//		BYTE* pDistanceToEndInfo = pClientGameInfo;
	//		len = Read16(pDistanceToEndInfo);

	//		UINT TimeFromRaceBegin = Read32(pDistanceToEndInfo);
	//		UINT DistanceToEnd = Read32(pDistanceToEndInfo);

	//		pClientGameInfo += len;
	//	}
	//	UCHAR CollisionNum = Read8(pClientGameInfo);
	//	for (size_t i = 0; i < CollisionNum; i++)
	//	{ //ClientCollisionInfo
	//		BYTE* pClientCollisionInfo = pClientGameInfo;
	//		len = Read16(pClientCollisionInfo);

	//		UINT TimeFromRaceBegin = Read32(pClientCollisionInfo);

	//		pClientGameInfo += len;
	//	}

	//	Body += len;
	//}
	//UCHAR RankedMatchSpeedKingBuffer = Read8(Body);
	//UINT DistanceTotal = Read32(Body);
	//UCHAR IsHitRecordContion = Read8(Body);
	//USHORT HangSpan = Read16(Body);
	//UCHAR CurRound = Read8(Body);
	//UCHAR IncRound = Read8(Body);


	if (Client->IsFinish && DstNPCID == 0)
	{
		return;
	}
	if (Client->GameID == 2)
	{
		//if (DstNPCID == 0)
		//{
		//	if (Client->TaskID == 0 || Client->TaskID == 0xFFFD || Client->TaskID == 0xFFFC || Client->TaskID == 0xFFFB)
		//	{
		//		if (CurrMapID != Client->MapID)
		//		{
		//			NotifyKickPlayer(Client, "环境异常，请重新登录！", 1);
		//			//UserDB_ban(Client->Uin);
		//			//Client->deleteflag = 1;
		//			Client->Server->Disconnect(Client->ConnID);
		//			return;
		//		}

		//		if (MsgSequence != (Client->MsgSequence + 1))
		//		{
		//			NotifyKickPlayer(Client, "环境异常，请重新登录！", 1);
		//			//UserDB_ban(Client->Uin);
		//			//Client->deleteflag = 1;
		//			Client->Server->Disconnect(Client->ConnID);
		//			return;
		//		}
		//		if (MsgSequence == 255)
		//		{
		//			Client->MsgSequence = -1;
		//		}
		//		else
		//		{
		//			Client->MsgSequence = MsgSequence;
		//		}
		//	}
		//}
	}
	else if (Client->GameID == 0)
	{
		if (DstNPCID == 0)
		{
			if (Client->RoomID != 0 || Client->BorderRoomID != 0)
			{
				if (Client->TaskID == 0 || Client->TaskID == 0xFFFD || Client->TaskID == 0xFFFC || Client->TaskID == 0xFFFB)
				{
					return;
				}
			}
		}
	}
	if (DstNPCID != 0)
	{
		return;//防止人机提前完成
	}
	if (DistanceToEnd != 0)
	{
		//全部通过距离计算 防止终点不红
		//printf("DistanceToEnd:%d\n", DistanceToEnd);
		IsFinish = FALSE;
	}
	else
	{
		IsFinish = TRUE;
	}

	char ChallengeResult = 3; //成功
	if (Client->TaskID == 0xFFF1 && Client->ChallengeMaps.size() == 1) {
		if (Client->ChallengeMaps[0].UseTime <= LapTime * 10 && Client->ChallengeMaps[0].UseTime > 0) {
			IsFinish = TRUE;
			ChallengeResult = 4;
		}
	}
#ifdef DEBUG
	printf("Round:%d\n", Client->Round);
#endif
	if (IsFinish)
	{
		//printf("IsFinish:%d\n", IsFinish);
		char kickon = 0;
		if (DstNPCID == 0)
		{
			Client->IsFinish = TRUE;
			Client->FinishTime = LapTime * 10;
		}
		Map* pMap = GetMap(Client->MapID);
		if (Client->RoomID != 0 && Client->TaskID == 0)
		{

			std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
			if (!Room)
			{
				return;
			}
			if ((GetTickCount() - Room->dwStart) < AllMapLimitTime)
			{
				NotifyKickPlayer(Client, "记录这么快，你不要命了啊", 0);
				//Client->deleteflag = 1;
				UserDB_ban(Client->Uin);
				Client->IsLogin = FALSE;
				Client->Server->Disconnect(Client->ConnID);
				Client->deleteflag = 1;
				return;
			}
			if (pMap && mapLimit == 1) {
				if (Client->FinishTime < pMap->Limit) {
					NotifyKickPlayer(Client, "记录异常，你已经被封号！", 0);
					//Client->deleteflag = 1;
					UserDB_ban(Client->Uin);
					Client->IsLogin = FALSE;
					Client->Server->Disconnect(Client->ConnID);
					Client->deleteflag = 1;
					return;
				}
			}

			bool NewRecord = PlayerDB_SetMapRecord(Client->Uin, Client->MapID, Client->FinishTime, Client);
			int PlayerNum = 0;
			if (Room->CountDownFlag == 0)
			{
				Room->CountDownFlag = 1;
				Client->ranknumber = Room->ranknumber;
				Room->ranknumber++;
				for (char i = 0; i < 6; i++)
				{
					std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
					if (RoomClient)
					{
						PlayerNum++;
						NotifyCountDown(RoomClient, Client->Uin, NewRecord, Client->FinishTime);
					}
				}
			}
			else
			{
				Client->ranknumber = Room->ranknumber;
				Room->ranknumber++;
				for (char i = 0; i < 6; i++)
				{
					std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
					if (RoomClient)
					{
						NotifyPlayerFinishRace(RoomClient, Client->Uin, NewRecord, Client->FinishTime);
					}
				}
			}

			if (!Room->Timer)
			{
				CreateRoomTimer(Room, 10000, OnCountDown);
			}
		}

		else if (Client->BorderRoomID != 0 && Client->TaskID == 0xFFFD || Client->TaskID == 0xFFFC || Client->TaskID == 0xFFFB)
		{
			std::shared_ptr<RoomNode> BorderRoom = GetBorderRoom(Client->BorderRoomID);
			if (!BorderRoom)
			{
				return;
			}
			if (DstNPCID != 0)
			{
				//人机过线
				//printf("DstNPCID:%d\n", DstNPCID);
				if (BorderRoom->CountDownFlag == 0)
				{
					BorderRoom->CountDownFlag = 1;
					NotifyCountDown(Client, DstNPCID, 0, 0);
					CreateRoomTimer(BorderRoom, 10000, OnBorderCountDown);
				}
				else
				{
					NotifyPlayerFinishRace(Client, DstNPCID, 0, 0);
				}
				//Client->NpcFinishInfo[DstNPCID].NpcID = DstNPCID;
				//Client->NpcFinishInfo[DstNPCID].FinishTime = 0;
				Client->NpcFinishInfo[DstNPCID].IsFinish = TRUE;
				Client->NpcFinishInfo[DstNPCID].ranknumber = BorderRoom->ranknumber;
				BorderRoom->ranknumber++;
				return;
			}
			if ((GetTickCount() - BorderRoom->dwStart) < AllMapLimitTime)
			{
				NotifyKickPlayer(Client, "记录这么快，你不要命了啊", 0);
				//Client->deleteflag = 1;
				UserDB_ban(Client->Uin);
				Client->IsLogin = FALSE;
				Client->Server->Disconnect(Client->ConnID);
				Client->deleteflag = 1;
				return;
			}
			if (pMap && mapLimit == 1) {
				if (Client->FinishTime < pMap->Limit) {
					NotifyKickPlayer(Client, "记录异常，你已经被封号！", 0);
					//Client->deleteflag = 1;
					UserDB_ban(Client->Uin);
					Client->IsLogin = FALSE;
					Client->Server->Disconnect(Client->ConnID);
					Client->deleteflag = 1;
					return;
				}
			}

			bool NewRecord = PlayerDB_SetMapRecord(Client->Uin, Client->MapID, Client->FinishTime, Client);
			int PlayerNum = 0;
			if (BorderRoom->CountDownFlag == 0)
			{
				BorderRoom->CountDownFlag = 1;
				Client->ranknumber = BorderRoom->ranknumber;
				BorderRoom->ranknumber++;
				for (char i = 0; i < 6; i++)
				{
					std::shared_ptr<ClientNode> RoomClient = BorderRoom->Player[i];
					if (RoomClient)
					{
						PlayerNum++;
						NotifyCountDown(RoomClient, Client->Uin, NewRecord, Client->FinishTime);
					}
				}
			}
			else
			{
				Client->ranknumber = BorderRoom->ranknumber;
				BorderRoom->ranknumber++;
				for (char i = 0; i < 6; i++)
				{
					std::shared_ptr<ClientNode> RoomClient = BorderRoom->Player[i];
					if (RoomClient)
					{
						NotifyPlayerFinishRace(RoomClient, Client->Uin, NewRecord, Client->FinishTime);
					}
				}
			}

			if (!BorderRoom->Timer)
			{
				CreateRoomTimer(BorderRoom, 11000, OnBorderCountDown);

			}
		}
		else if (Client->TaskID == 0x06a5)
		{

			Client->OptNum = 0;
			Client->TaskID = 0;
			Client->IsFinish = FALSE;
		}
		else if (Client->TaskID == 0xFFF1)
		{
			NotifyRaceShow(Client);
			Sleep(2000);
			NotifyG2CChallengeResult(Client, ChallengeResult, ChallengeResult == 3 ? LapTime * 10 : -1);
			NotifyGameOver(Client, 0, 0, 0);

			Client->OptNum = 0;
			Client->TaskID = 0;
			Client->IsFinish = TRUE;
		}
		else if (Client->TaskID != 0)//其他单人任务完成
		{

			SendToClient(Client, 532, (BYTE*)"\x00\x00", 2, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);

			BYTE* pp = taskaward;
			Write32(pp, Client->Uin);
			pp += 2;
			Write32(pp, Client->TaskID);
			Write8(pp, 0);
			pp += 18;
			Write32(pp, 0); //AwardMoney
			Write32(pp, Client->Money); //TotalMoney
			Write32(pp, 0); //AwardSuperMoney
			Write32(pp, Client->SuperMoney); //TotalSuperMoney
			Write32(pp, 0); //AwardExp
			Write32(pp, Client->Experience); //TotalExp
			SendToClient(Client, 533, taskaward, 127, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);

			SendToClient(Client, 534, (BYTE*)"\x00\x00", 2, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
			Client->TaskID = 0;
			Client->OptNum = 0;
			Client->IsFinish = FALSE;
		}

	}
}

void NotifyCountDown(std::shared_ptr<ClientNode> Client, UINT WinnerUin, bool WinnerNewRecord, UINT FinTime)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 10000); //CountDownTime
	Write32(p, WinnerUin); //WinnerUin
	Write8(p, WinnerNewRecord); //WinnerNewRecord
	Write32(p, FinTime); //FinTime

	len = p - buf.data();
	SendToClient(Client, 512, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyRaceShow(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 0); //RaceShowTime
	Write8(p, 0); //HasFBInfo
	/*
m_stFBInfo[].m_bFBResult
m_stFBInfo[].m_uiKiller
	*/

#ifndef ZingSpeed
	Write8(p, 0); //WinTeamID
	Write32(p, 0); //EventID

	Write32(p, 0); //ParaNum
	//m_auiPara[]

#endif

	len = p - buf.data();
	SendToClient(Client, 515, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}




void NotifyRaceOverNew(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, bool First)
{
	/*std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room) {
		Room = GetBorderRoom(Client->BorderRoomID);
	}*/
	if (!Room) return;

	Room->startStatus = 1;

	int Honor = 0;
	int SpeedCoin = 0;
	int RedTeam = 0;
	int BlueTeam = 0;
	int TeamScore = 10;
	int FirstTeam = 0;
	int WinTeam = 0;
	int mapid = Room->MapID;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	char playernum = 0;
	BYTE* PlayerNum = p;
	//PlayerRaceInfo ArrPlayerRaceInfo[12] = {};
	PlayerRaceInfo ArrPlayerRaceInfo[6] = {};
	for (int i = 0; i < 6; i++)
	{
		PlayerRaceInfo tmp;
		tmp.Uin = -1;
		tmp.FinishTime = -1;
		tmp.Teamid = 0;
		tmp.kartid = 0;
		tmp.HighestSpeed = 0;
		tmp.CollisionCount = 0;
		tmp.N2OGatherCount = 0;
		tmp.DriftCount = 0;
		tmp.RankGradeLevel = 0;
		ArrPlayerRaceInfo[i] = tmp;

		if (Room->Player[i])
		{
			//完成时间如果是0还是会作为结算成绩进行排名
			//改为-1就是未完成了
			if (Room->Player[i]->FinishTime == 0)Room->Player[i]->FinishTime = -1;
			ArrPlayerRaceInfo[i].Uin = Room->Player[i]->Uin;
			ArrPlayerRaceInfo[i].FinishTime = Room->Player[i]->FinishTime;
			ArrPlayerRaceInfo[i].Teamid = Room->Player[i]->TeamID;
			ArrPlayerRaceInfo[i].kartid = Room->Player[i]->KartSkinID > 0 ? Room->Player[i]->KartSkinID : Room->Player[i]->KartID;

			ArrPlayerRaceInfo[i].HighestSpeed = Room->Player[i]->HighestSpeed;
			ArrPlayerRaceInfo[i].CollisionCount = Room->Player[i]->CollisionCount;
			ArrPlayerRaceInfo[i].N2OGatherCount = Room->Player[i]->N2OGatherCount;
			ArrPlayerRaceInfo[i].DriftCount = Room->Player[i]->DriftCount;

			ArrPlayerRaceInfo[i].RankGradeLevel = RankGradeLevel[Room->Player[i]->GradeLevel];

			if (mapid < 100) {
				mapid = Room->Player[i]->MapID;
			}
			playernum++;
		}
	}

	auto cmp_PlayerRaceInfo = [](PlayerRaceInfo& a, PlayerRaceInfo& b) -> bool
	{
		if (a.FinishTime == b.FinishTime) {
			return  a.Uin < b.Uin;
		}
		else {
			return  a.FinishTime < b.FinishTime;
		}

	};

	//名次排序
	std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);
	//std::sort(ArrPlayerRaceInfo, ArrPlayerRaceInfo + 6, cmp_PlayerRaceInfo);

	if (First) {
		InsertBattleRecord(ArrPlayerRaceInfo, mapid, 0);
	}

	int TotalExp = 0;
	int Money = 0;
	Write8(p, playernum); //CurrentPlayerNum
	for (int i = 0; i < playernum; i++) {
		if (ArrPlayerRaceInfo[i].Uin == Client->Uin) {
			SelectClientExp(Client);
			//Client->Experience += Addexp;//+10000经验
			Client->Experience = (Client->Experience > UINT_MAX - Addexp) ? UINT_MAX : Client->Experience + Addexp;
			Client->Money += 1000;//+1000酷比
			UpdateClientBase(Client);
		}

		std::vector<int> conditionValues;
		conditionValues.push_back(ArrPlayerRaceInfo[i].Uin);
		std::vector<int> resultRow = executeQueryRow("SELECT Money,Experience  FROM BaseInfo  WHERE Uin=?;", "NotifyRaceOverNew", conditionValues);
		if (resultRow.size() >= 2) {
			Money = resultRow[0];
			TotalExp = resultRow[1];
		}


		if (i == 0) {
			FirstTeam = ArrPlayerRaceInfo[i].Teamid;
		}

		if (ArrPlayerRaceInfo[i].Teamid == 0 && ArrPlayerRaceInfo[i].FinishTime != -1) {
			RedTeam += TeamScore;
		}
		else if (ArrPlayerRaceInfo[i].Teamid == 1 && ArrPlayerRaceInfo[i].FinishTime != -1)
		{
			BlueTeam += TeamScore;
		}
		if (i == 5) {
			TeamScore = 1;
		}
		else {
			TeamScore -= 2;
		}


		BYTE* pRaceScore = p;
		Write16(pRaceScore, 0); //len

		Write32(pRaceScore, ArrPlayerRaceInfo[i].Uin); //Uin
		Write32(pRaceScore, ArrPlayerRaceInfo[i].FinishTime); //FinTime
		Write32(pRaceScore, 0); //TP
		Write32(pRaceScore, 1000); //MoneyInc
		Write32(pRaceScore, Money); //TotalMoney
		Write32(pRaceScore, Addexp); //ExpInc
		Write32(pRaceScore, TotalExp); //TotalExp

		//Write8(pRaceScore, 0xa); //AwardNum
		//m_iSpecialAward[]
		memcpy(pRaceScore, "\x0a\x00\x00\x00\x03\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00\x10\x00\x00\x00\x1a\x00\x00\x00\x1e\x00\x00\x00\x1f\x00\x00\x00\x22\x00\x00\x00\x23", 41);
		pRaceScore += 41;
		Write32(pRaceScore, 0); //TeamWorkExpInc
		Write32(pRaceScore, 0); //PropPoint
		Write32(pRaceScore, 0); //PropPointAddExp
		Write32(pRaceScore, 0); //LuckyMatchPointInc
		Write32(pRaceScore, 0); //LuckyMatchPointTotal
		Write32(pRaceScore, 0); //LuckyMatchScoreInc
		Write32(pRaceScore, 0); //LuckyMatchScoreTotal
		Write32(pRaceScore, 0); //LuckMoneyInc
		Write32(pRaceScore, 0); //LuckMoneyTotal
		Write32(pRaceScore, 0); //GuildScoreInc

		Write8(pRaceScore, 0); //CrazyPropAchieveNum
		//m_aiCrazyPropAchieve[]

		Write32(pRaceScore, 0); //IncWlMatchScore
		Write32(pRaceScore, 0); //IncWlDegree
		Write8(pRaceScore, 0); //IncItemNumByWl


		Write32(pRaceScore, 0); //WlMutiplyCard
		Write32(pRaceScore, 0); //SkateCoinInc
		Write32(pRaceScore, 0); //SkateCoinTotal
		Write32(pRaceScore, 0); //SkateCoinHistoryTotal
		Write32(pRaceScore, 0); //TotalCoupons


		Write8(pRaceScore, 0); //ChallengeCheer


		Write32(pRaceScore, 0); //LoveValue
		Write32(pRaceScore, 0); //SkateCoinEmperorBonus
		Write32(pRaceScore, 0); //DetailRecordID

		Write8(pRaceScore, 0); //HasGangsterResult


		{ //EquippedActiveKartInfo
			BYTE* pEquippedActiveKartInfo = pRaceScore;
			Write16(pEquippedActiveKartInfo, 0); //len

			Write8(pEquippedActiveKartInfo, 0); //HaveActiveInfo
			Write32(pEquippedActiveKartInfo, 0); //KartID
			Write32(pEquippedActiveKartInfo, 0); //ActiveLevel

			len = pEquippedActiveKartInfo - pRaceScore;
			Set16(pRaceScore, (WORD)len);
			pRaceScore += len;
		}


		Write8(pRaceScore, 0); //HasWeRelayRaceOverInfo

		Write8(pRaceScore, 0); //HaveSkatePropRaceInfo


		Write8(pRaceScore, 0); //IsNewBox


		Write8(pRaceScore, 0); //HaveArrestScoreInfo

		Write8(pRaceScore, 0); //HasRankedMatchInfo


		Write8(pRaceScore, 0); //HaveCrazyChaseScoreInfo

		Write8(pRaceScore, Client->TeamID); //TeamID

		Write8(pRaceScore, 0); //HasRankedMatchArenaInfo



		Write32(pRaceScore, 0); //DistanceToEnd
		Write8(pRaceScore, 0); //ShortDistancWinPoint
		Write8(pRaceScore, 0); //Status


		Write8(pRaceScore, 0); //HaveRankedMatchSpeedKingInfo



		Write8(pRaceScore, 0); //ProfessionLicenseAwardNum 职业生涯增长


		{ //RaceResult
			BYTE* pRaceResult = pRaceScore;
			Write16(pRaceResult, 0); //len

			Write8(pRaceResult, 0); //PLExpPowerValid
			Write8(pRaceResult, 0); //PLHonorPowerValid
			Write8(pRaceResult, 0); //ExpMultiNum
			Write8(pRaceResult, 0); //HonorMultiNum

			len = pRaceResult - pRaceScore;
			Set16(pRaceScore, (WORD)len);
			pRaceScore += len;
		}

		Write8(pRaceScore, 0); //HaveRankedMatchExtraInfo


		Write8(pRaceScore, 0); //HasYuLeJiaNianHuaComputeData


		len = pRaceScore - p;
		Set16(p, (WORD)len);
		p += len;


	}

	Set8(PlayerNum, playernum);

	if (RedTeam > BlueTeam) {
		WinTeam = 0;
	}
	else if (RedTeam < BlueTeam) {
		WinTeam = 1;
	}
	else {
		WinTeam = FirstTeam;
	}
	Write8(p, WinTeam); //WinTeamID
	Write32(p, 0); //WinType
	Write32(p, 0); //AwardTime
	Write8(p, 0); //LuckyMatchResult
	Write8(p, 0); //LuckyMatchType


	Write8(p, 1); //PlayerNumber
	{ //HideTaskResult
		BYTE* pHideTaskResult = p;
		Write16(pHideTaskResult, 0); //len

		Write32(pHideTaskResult, Client->Uin); //Uin
		Write32(pHideTaskResult, 0); //TaskId
		Write8(pHideTaskResult, 0); //HideTaskFinishResult

		len = pHideTaskResult - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasLadderMatchResult
	Write8(p, 0); //NeedFreshTutor
	Write8(p, 0); //hasMedalGameInfo


#ifndef ZingSpeed
	Write8(p, 0); //KickOffPlayerNumber
	Write8(p, 0); //EliminatedRacerNum
	Write8(p, 0); //TeamNum
	Write8(p, 0); //NPCRacerNum
	Write8(p, 0); //hasGameFrameInfo
	Write8(p, 0); //hasDesperateEscapeTrophyInfo
	Write8(p, 0); //HasGameStageInfo
#endif

	len = p - buf.data();
	SendToClient(Client, 513, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyGameOver(std::shared_ptr<ClientNode> Client, USHORT LeaveGameType, BYTE ParaNum, int* ParaList)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

#ifndef ZingSpeed
	Write8(p, 0); //StageIndex
	Write8(p, 0); //ReturnHall
	Write8(p, 0); //WaitEnterRoom
	Write16(p, LeaveGameType); //LeaveGameType
	Write8(p, ParaNum); //ParaNum
	for (size_t i = 0; i < ParaNum; i++)
	{
		//m_aiParaList[]
		Write32(p, ParaList[i]);
	}
#endif

	len = p - buf.data();
	SendToClient(Client, 514, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


void RequestTransferByTCP(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	UINT Uin = Read32(Body);

	std::shared_ptr<RoomNode> Room;
	Room = GetBorderRoom(Client->BorderRoomID);
	if (!Room) {
		Room = GetRoom(Client->RoomID);
	}

	if (!Room)
	{
		return;
	}
	UINT Time = Read32(Body);

	char Ver = Read8(Body);
	int Seq = Read32(Body);
	char DstNum = Read8(Body);

	struct DstInfo
	{
		USHORT PlayerID = 0;
		UINT Uin = 0;
	};
	std::vector<DstInfo> aDstInfo;

	DstNum = DstNum > 6 ? 6 : DstNum;
	aDstInfo.resize(DstNum);
	for (char i = 0; i < DstNum; i++)
	{
		BYTE* pDstInfo = Body;
		USHORT len = Read16(pDstInfo);

		aDstInfo[i].PlayerID = Read16(pDstInfo);
		aDstInfo[i].Uin = Read32(pDstInfo);

		Body += len;
	}
	USHORT BuffLen = Read16(Body);


	if (isRealTime[0] == 1) {
		for (char i2 = 0; i2 < 6; i2++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
			if (RoomClient && RoomClient != Client)
			{
				for (int i = 0; i < isRealTime[1]; i++)
				{
					NotifyTranferByTCP(RoomClient, Client->Uin, Client->ConnID, Seq, Body, BuffLen);
				}
			}
		}
	}
	else {
		for (char i = 0; i < DstNum; i++)
		{
			for (char i2 = 0; i2 < 6; i2++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
				if (RoomClient)
				{
					if (RoomClient->Uin == aDstInfo[i].Uin)
					{
						if (RoomClient->IsReady)
						{
							NotifyTranferByTCPMC(RoomClient, Client->Uin, Client->ConnID, Seq, Body, BuffLen);
						}
						break;
					}
				}
			}
		}
	}

}


void NotifyTranferByTCPMC(std::shared_ptr<ClientNode> Client, UINT SrcUin, USHORT SrcPlayerID, int Seq, BYTE* Buff, int Bufflen)
{
	BYTE buf[8192] = { 0 };
	BYTE* p = buf;
	size_t len = 0;

	Write32(p, SrcUin);
	Write16(p, SrcPlayerID);
	Write8(p, 0); // Ver
	Write32(p, Seq);
	Write16(p, Bufflen);
	memcpy(p, Buff, Bufflen);
	p += Bufflen;

	len = p - buf;
	SendToClient(Client, 560, buf, len, SrcPlayerID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyTranferByTCP(std::shared_ptr<ClientNode> Client, UINT SrcUin, USHORT SrcPlayerID, int Seq, BYTE* Buff, int Bufflen)
{
	BYTE buf[8192] = { 0 };
	BYTE* p = buf;
	size_t len = 0;

	Write32(p, SrcUin);
	Write16(p, SrcPlayerID);
	Write8(p, 0); //Ver
	Write32(p, Seq);
	Write16(p, Bufflen);
	memcpy(p, Buff, Bufflen);
	p += Bufflen;


	len = p - buf;
	SendToClient(Client, 560, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}



void NotifyAddPropBySkillStone(std::shared_ptr<ClientNode> Client, int StoneSkillType, short PropID, int PropIndex)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, StoneSkillType);
	Write16(p, PropID);
	Write32(p, PropIndex);

	len = p - buf.data();
	SendToClient(Client, 908, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

//显示情侣超级氮气空格大招
void NotifyLM2CCooperInc(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write8(p, 0); //Reason
	Write16(p, 0); //CooperInc
	Write16(p, 0); //CurAccuCooper
	Write32(p, 0); //TotalCooper
	Write8(p, 0); //BeTriggerSuperJet
	Write8(p, 0); //SuperJetTime
	Write8(p, 0); //TriggerLoveTimeLimit
	Write32(p, 0); //NoCooperDistance
	Write32(p, Client->Uin); //Uin

	len = p - buf.data();
	SendToClient(Client, 11338, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseC2LMTriggerSuperJet(std::shared_ptr<ClientNode> Client)
{//情侣超级氮气特效
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //SkillID
	Write8(p, 0); //SkillType
	len = p - buf.data();
	SendToClient(Client, 11339, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyLM2CSuperJet(std::shared_ptr<ClientNode> Client)
{//喷气显示情侣爱心
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //SuperJetTeamMateUin
	Write8(p, 0); //SkillID
	Write8(p, 0); //SkillType
	Write8(p, 0); //IsAck
	len = p - buf.data();
	SendToClient(Client, 11340, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestReportDrift(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int DriftFlag = Read32(p);
	int AccelerateFuelInc = Read32(p);
	bool OnlyAccelSelfFue = Read8(p);
	UINT StartLapTime = Read32(p);
	UINT CurLapTime = Read32(p);

}

//添加超级氮气
void ResponseGetSuperN2O(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	int temp = 0;
	Write16(p, 0); //ResultID

	if (Client->SuperJetKartFuelInc >= 6000)
	{
		Write16(p, 777); //PropID玉麒麟超级氮气必须writekartphy写入777信息才生效
		Client->SuperJetKartFuelInc = 0;
		Client->CurrentAccelerateFuel = 0;
		temp = 100;
	}
	else
	{
		Write16(p, 0);
		Client->CurrentAccelerateFuel = 0;
		temp = 180;

	}
	Write32(p, ++Client->PropIndex); //PropIndex
	Write32(p, 0); //CurrentAccelerateFuel
	Write8(p, 0); //ReasonLen
	//printf("temp:%d\n", temp);

	Write32(p, temp); //SuperJetKartFuelInc 玉麒麟superjet进度条增长量

	len = p - buf.data();
	SendToClient(Client, 119, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}


void NotifyUseProp(std::shared_ptr<ClientNode> RoomClient, UINT DstUin, UINT Uin, UINT PropID)
{
	//通知别人使用道具效果 如氮气特效 导弹命中

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, PropID); //PropID
	Write32(p, DstUin); //DstUin
	Write32(p, 0); //PropPosition
	Write32(p, 0); //PropPosition
	Write32(p, 0); //PropPosition
	Write32(p, 0); //PropIndex
	Write16(p, 0); //NewID
	Write32(p, 0); //ItemID
	Write32(p, 0); //StoneSkillType
	Write32(p, 0); //LapTime
	Write8(p, 0); //DstUinNum
	//Write32(p, 0); //DstUin[]
	Write8(p, 0); //PropUseMode
	Write8(p, 0); //QueryUinNum
	//Write32(p, 0); //QueryUin[]
	Write8(p, 0); //DstType
	Write8(p, 0); //Position
	Write8(p, 0); //ParaNum
	//Write32(p, 0); //ParaList[]
	Write8(p, 0); //Status
	Write8(p, 0); //PropPosIdx
	Write8(p, 0); //PropSecType
	Write8(p, 0); //PassedCheckPointNum
	//Write16(p, 0); //PassedCheckPointIDs[]
	Write16(p, 0); //SubPropID
	Write8(p, 0); //OtherPropPosNum
	len = p - buf.data();
	SendToClient(RoomClient, 517, buf.data(), len, RoomClient->ConnID, FE_PLAYER, RoomClient->ConnID, Notify);
}
void RequestUseProp2(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{//使用道具时就会触发这个请求消息 如氮气 其他道具等
	/*if (Client->TaskID == 0xFFFC && !rankstone)
	{
		return;
	}*/
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	short PropID = Read16(Body);



	Client->PropID = PropID;
	if (34 == PropID)
	{
		Client->SuperJet = 0;
	}

	if (759 == PropID)
	{
		Client->getPropNum = 0;
	}

	if (Client->TaskID == 0xFFFC && !rankstone)
	{
		return;
	}

	if (Client->TaskID == 0xFFF1)
	{
		return;
	}

	UINT DstUin = Read32(Body);

	//m_aiCurrentPosition[3]
	Body += 3 * 4;

	int PropIndex = Read32(Body);
	short NewID = Read16(Body);
	char IsClearProp = Read8(Body);
	BYTE DstType = Read8(Body);
	int MyUseItemID = Read32(Body);
	int DstUseItemID = Read32(Body);
	BYTE IsConvertMode = Read8(Body);
	int StoneSkillType = Read32(Body);
	UINT LapTime = Read32(Body);
	BYTE DstUinNum = Read8(Body);
	//m_au32DstUin[]

	BYTE PropUseMode = Read8(Body);
	BYTE QueryUinNum = Read8(Body);
	//m_au32QueryUin[]

#ifndef ZingSpeed
	UCHAR CurrentSelfRank = Read8(Body);
	UCHAR CurrentTeammateRank = Read8(Body);
	UCHAR Position = Read8(Body);
	char PropSecType = Read8(Body);
	UCHAR PropPosIdx = Read8(Body);
	UINT NPCUin = Read32(Body);
	UCHAR UsePropProtoMode = Read8(Body);
	short SubPropID = Read16(Body);
	UCHAR OtherPropPosNum = Read8(Body);
#endif

#ifdef DEBUG
	printf("PropID:%d, DstUin:%d, PropIndex:%d, StoneSkillType:%d\n", PropID, DstUin, PropIndex, StoneSkillType);
#endif
	//printf("PropID:%d, DstUin:%d, PropIndex:%d, StoneSkillType:%d\n", PropID, DstUin, PropIndex, StoneSkillType);
	//int Prob = 380;//六级重生发动率
	//int Prob = 290;

	if (Client->lastUpdateTime.time_since_epoch().count() == 0)
	{
		// 如果 lastUpdateTime 未初始化，将其初始化为当前时间
		Client->lastUpdateTime = std::chrono::steady_clock::now();
	}


	// 设置种子
	std::srand(std::time(nullptr));
	// 生成随机值
	int randomValue = ProbTime[0] + (std::rand() % (2 * ProbTime[1] + 1)) - ProbTime[1];

	// 判断是否已经过去了随机秒数
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - Client->lastUpdateTime).count();
	if (elapsed >= randomValue)
	{
		// 每隔ProbTime执行的函数逻辑
		NotifyAddPropBySkillStone(Client, PropID, 1, PropIndex);
		// 重置 lastUpdateTime
		Client->lastUpdateTime = currentTime;
	}

	if (Client->PropID == 777 && (Client->MapID == 431 || Client->MapID == 417 || Client->MapID == 383 || Client->MapID == 432)) {
		//ResponseGetSuperN2O1(Client, 759);
	}

	//if ((rand() % 1000) + Prob > 1000)
	//{
	//	//重生发动 添加氮气道具 
	//	//NotifyAddPropBySkillStone(Client, PropID, 1, PropIndex);
	//}
	Client->PropIndex = PropIndex;
	std::shared_ptr<RoomNode> Room = NULL;
	if (Client->RoomID != 0)
	{
		Room = GetRoom(Client->RoomID);//通知其他人看到道具效果
	}
	else
	{
		Room = GetBorderRoom(Client->BorderRoomID);
	}
	if (!Room)
	{
		return;
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient != Client)
			{
				NotifyUseProp(RoomClient, DstUin, Uin, PropID);
			}
		}
	}


}

void NotifyAddPropByxiaolv(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 34); //PropID
	Write32(p, 1); //PropIdx
	//Write32(p, 0); //ItemID
	//Write32(p, 0); //StoneSkillType

	len = p - buf.data();
	SendToClient(Client, 526, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void ResponseGetSuperN2O1(std::shared_ptr<ClientNode> Client, UINT PropID)
{
	if (Client->getPropNum == 0)
	{
		Client->getPropNum = 1;//超级喷未使用标志 使用后才继续生产
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write16(p, 0); //ResultID
		Write32(p, Client->Uin); //Uin
		Write16(p, PropID); //PropID
		//34 433紫色氮气 2002蓝色氮气 71火球 176 190其他氮气 555火焰氮气 
		//777玉麒麟专属超级氮气
		Write32(p, 0); //PropIndex
		Write8(p, 0); //GetPropType
		Write32(p, 0); //AwardItemID
		Write8(p, 0); //ReasonLen
		Write16(p, 1); //PropPositionNO
		Write32(p, 0); //ActIDForClient
#ifndef ZingSpeed
		Write8(p, 0); //GetPropSecType
		Write8(p, 1); //PropPosIdx
		Write32(p, 0); //NPCUin
		Write8(p, 0); //ParaNum
		//Write32(p, 0); //ParaList[]
#endif
		len = p - buf.data();
		SendToClient(Client, 124, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}
//添加超级氮气
void ResponseGetSuperN2O2(std::shared_ptr<ClientNode> Client)
{
	if (Client->SuperJet == 0)
	{
		Client->SuperJet = 1;//超级喷未使用标志 使用后才继续生产
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write16(p, 0); //ResultID
		Write32(p, Client->Uin); //Uin
		Write16(p, 34); //PropID
		//34 433紫色氮气 2002蓝色氮气 71火球 176 190其他氮气 555火焰氮气 
		//777玉麒麟专属超级氮气
		Write32(p, 1); //PropIndex
		Write8(p, 0); //GetPropType
		Write32(p, 0); //AwardItemID
		Write8(p, 0); //ReasonLen
		Write16(p, 1); //PropPositionNO
		Write32(p, 0); //ActIDForClient
#ifndef ZingSpeed
		Write8(p, 0); //GetPropSecType
		Write8(p, 1); //PropPosIdx
		Write32(p, 0); //NPCUin
		Write8(p, 0); //ParaNum
		//Write32(p, 0); //ParaList[]
#endif
		len = p - buf.data();
		SendToClient(Client, 124, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}


void NotifySkillStoneKartMoreInfo(std::shared_ptr<ClientNode> Client, size_t num);
void NotifySkillStoneKartMoreInfo(std::shared_ptr<ClientNode> Client, size_t num)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len = 0;

	//if (buf == nullptr)
	//{
	//	// 分配空间失败的处理
	//	return;
	//}

	Write32(p, Client->Uin); // Uin
	int StoneKartNum = 0;
	int StoneGrooveNums = 0;
	bool HasMoreInfo = false;
	BYTE* pStoneKartNum = p;
	Write32(p, 0); // StoneKartNum

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT *  FROM kartstonenew WHERE Uin = ? LIMIT ?,2000;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->setUInt(2, num);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			if (StoneGrooveNums > 1800)
			{
				HasMoreInfo = true;
				break;
			}
			StoneKartNum++;
			BYTE* pKartStoneGrooveInfo = p;
			Write16(pKartStoneGrooveInfo, 0); // len

			int KartID = res->getUInt("KartID");

			Write32(pKartStoneGrooveInfo, KartID);

			BYTE* pStoneGrooveNum = pKartStoneGrooveInfo;
			Write32(pKartStoneGrooveInfo, 0); // StoneGrooveNum
			{
				int StoneGrooveNum = 5;
				for (size_t i = 1; i <= StoneGrooveNum; i++)
				{
					std::string StoneUseOccaType = "StoneUseOccaType" + std::to_string(i);
					std::string SkillStoneID = "SkillStoneID" + std::to_string(i);

					BYTE* pStoneGrooveInfo = pKartStoneGrooveInfo;
					Write16(pStoneGrooveInfo, 0); // len

					Write32(pStoneGrooveInfo, res->getUInt(StoneUseOccaType)); // StoneUseOccaType
					Write32(pStoneGrooveInfo, res->getUInt(SkillStoneID)); // SkillStoneID

					len = pStoneGrooveInfo - pKartStoneGrooveInfo;
					Set16(pKartStoneGrooveInfo, (WORD)len);
					pKartStoneGrooveInfo += len;
				}

				Set32(pStoneGrooveNum, StoneGrooveNum);
				StoneGrooveNums += StoneGrooveNum;
			}
			len = pKartStoneGrooveInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("NotifySkillStoneKartMoreInfo error\n");
		//throw;关闭
	}
	Set32(pStoneKartNum, StoneKartNum);
	Write8(p, HasMoreInfo); // HasNext


	len = p - buf.data();
	SendToClient(Client, 28416, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	if (HasMoreInfo)
	{
		NotifySkillStoneKartMoreInfo(Client, StoneKartNum + num);
	}
	// 释放堆空间
	//delete[] buf;
}
void NotifySkillStoneKartInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len = 0;

	//if (buf == nullptr)
	//{
	//	// 分配空间失败的处理
	//	return;
	//}

	Write32(p, Client->Uin); // Uin
	int StoneKartNum = 0;
	int StoneGrooveNums = 0;
	bool HasMoreInfo = false;
	BYTE* pStoneKartNum = p;
	Write32(p, 0); // StoneKartNum
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT *  FROM kartstonenew WHERE Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			if (StoneGrooveNums > 1800)
			{
				HasMoreInfo = true;
				break;
			}
			StoneKartNum++;
			BYTE* pKartStoneGrooveInfo = p;
			Write16(pKartStoneGrooveInfo, 0); // len

			int KartID = res->getUInt("KartID");

			Write32(pKartStoneGrooveInfo, KartID);

			BYTE* pStoneGrooveNum = pKartStoneGrooveInfo;
			Write32(pKartStoneGrooveInfo, 0); // StoneGrooveNum
			{
				int StoneGrooveNum = 5;
				for (size_t i = 1; i <= StoneGrooveNum; i++)
				{
					std::string StoneUseOccaType = "StoneUseOccaType" + std::to_string(i);
					std::string SkillStoneID = "SkillStoneID" + std::to_string(i);

					BYTE* pStoneGrooveInfo = pKartStoneGrooveInfo;
					Write16(pStoneGrooveInfo, 0); // len

					Write32(pStoneGrooveInfo, res->getUInt(StoneUseOccaType)); // StoneUseOccaType
					Write32(pStoneGrooveInfo, res->getUInt(SkillStoneID)); // SkillStoneID

					len = pStoneGrooveInfo - pKartStoneGrooveInfo;
					Set16(pKartStoneGrooveInfo, (WORD)len);
					pKartStoneGrooveInfo += len;
				}

				Set32(pStoneGrooveNum, StoneGrooveNum);
				StoneGrooveNums += StoneGrooveNum;
			}
			len = pKartStoneGrooveInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		res.reset();
		Set32(pStoneKartNum, StoneKartNum);
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("NotifySkillStoneKartInfo error\n");
	}

	Write8(p, HasMoreInfo); // HasMoreInfo

	len = p - buf.data();
	SendToClient(Client, 228, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	if (HasMoreInfo)
	{
		NotifySkillStoneKartMoreInfo(Client, StoneKartNum);
	}

	// 释放堆空间
	//delete[] buf;
}


void RequestGetProp(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	if (Client->TaskID == 0xFFFF || Client->TaskID == 0xFFFE)
	{
		return;
	}
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short PropPositionNO = Read16(p);
	bool CurrentRank = Read8(p);
	//int CurrentPosition[3] = Read32(p);
	//int PreviousInterval = Read32(p);
	//char GetPropType = Read8(p);
	//bool TeammateRank = Read8(p);
	//char PointArrayIdx = Read8(p);
	//UINT LapTime = Read32(p);
#ifndef ZingSpeed
	//char GetPropSecType = Read8(p);
	//UCHAR PropPosIdx = Read8(p);
	//UINT NPCUin = Read32(p);
	//UCHAR ParaNum = Read8(p);
	//UINT ParaList[] = Read32(p);
#endif

	ResponseGetProp(Client);
}
void ResponseGetProp(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	static WORD PropIDs[] = { 1,2,3,4,5,6,7,8,9,10,15,16,18,21,25,27,41,43,44,45,46,47,49,50,51,53 };
	static int Index = 0;
	Index++;
	if (Index >= sizeof(PropIDs) / sizeof(WORD))
	{
		Index = 0;
	}
	//printf("%d\n", Index);

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write16(p, PropIDs[Index]); //PropID
	Write32(p, 0); //PropIndex
	Write8(p, 0); //GetPropType
	Write32(p, 0); //AwardItemID
	Write8(p, 0); //ReasonLen
	Write16(p, 0); //PropPositionNO
	Write32(p, 0); //ActIDForClient
#ifndef ZingSpeed
	Write8(p, 0); //GetPropSecType
	Write8(p, 0); //PropPosIdx
	Write32(p, 0); //NPCUin
	Write8(p, 0); //ParaNum
	Write32(p, 0); //ParaList[]
#endif


	len = p - buf.data();
	SendToClient(Client, 124, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void NotifyMsgBox(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write8(p, 0); //MsgType
	{ //MsgBox
		BYTE* pMsgBox = p;
		Write16(pMsgBox, 0); //len

		{ //OnlineTaskMsg
			BYTE* pOnlineTaskMsg = pMsgBox;
			Write16(pOnlineTaskMsg, 0); //len

			Write8(pOnlineTaskMsg, 0); //MsgType
			Write32(pOnlineTaskMsg, 0); //OpenDate
			Write32(pOnlineTaskMsg, 0); //OpenTime
			Write32(pOnlineTaskMsg, 0); //CloseDate
			Write32(pOnlineTaskMsg, 0); //CloseTime
			Write8(pOnlineTaskMsg, 0); //IsToday
			Write8(pOnlineTaskMsg, 0); //URLLen
			Write8(pOnlineTaskMsg, 0); //WeekLimit

			len = pOnlineTaskMsg - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //VipPages
			BYTE* pVipPages = pMsgBox;
			Write16(pVipPages, 0); //len

			WriteString_MC(pVipPages, "http://speed.qq.com/act/a20110609vip/power.shtml"); //URLLen1
			WriteString_MC(pVipPages, "http://pay.qq.com/ipay/index.shtml?n=3&c=fczzzz,qqfczz&aid=pay.speed"); //URLLen2
			WriteString_MC(pVipPages, "http://speed.qq.com/act/a20170630zzvip/index.shtml"); //URLLen3

			len = pVipPages - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //ActivitiesAd
			BYTE* pActivitiesAd = pMsgBox;
			Write16(pActivitiesAd, 0); //len

			WriteString_MC(pActivitiesAd, "http://speed.qq.com/home/client_newsbox.htm"); //URLLen

			len = pActivitiesAd - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //MatchSchedule
			BYTE* pMatchSchedule = pMsgBox;
			Write16(pMatchSchedule, 0); //len

			Write8(pMatchSchedule, 0); //URLLen

			len = pMatchSchedule - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //LoadingAd
			BYTE* pLoadingAd = pMsgBox;
			Write16(pLoadingAd, 0); //len

			Write8(pLoadingAd, 0); //Ver
			Write8(pLoadingAd, 0); //URLLen

			len = pLoadingAd - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //TaskAdvAd
			BYTE* pTaskAdvAd = pMsgBox;
			Write16(pTaskAdvAd, 0); //len

			WriteString_MC(pTaskAdvAd, "http://speed.qq.com/home/new_year_task20140107.htm"); //URLLen

			len = pTaskAdvAd - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //LoginAd
			BYTE* pLoginAd = pMsgBox;
			Write16(pLoginAd, 0); //len

			Write8(pLoginAd, 0); //URLLen

			len = pLoginAd - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}
		{ //T3Ad
			BYTE* pT3Ad = pMsgBox;
			Write16(pT3Ad, 0); //len

			Write8(pT3Ad, 0); //URLLen1
			Write8(pT3Ad, 0); //URLLen2
			Write8(pT3Ad, 0); //URLLen3
			Write8(pT3Ad, 0); //URLLen4

			len = pT3Ad - pMsgBox;
			Set16(pMsgBox, (WORD)len);
			pMsgBox += len;
		}

		len = pMsgBox - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 596, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Notify);
}


void NotifySvrConfig(std::shared_ptr<ClientNode> Client) //服务端配置
{
	if (Uinew) {
		std::string Byte = "0000138800CE3130A1AB3230B7D6D6D300000000000000000000000000000000000000000000000000000000000000000000000000000000BAA3C1BFB8C4D7B0B5C0BEDFC4C3B2BBCDA3A3A1000000000000000000000000000000000000000000000000000000000000C2FA3332C8CBBFAAC8FC000000000000000000000000000000000000000000000000000000000000000000000000000000000000003CD0D2D4CBB4F3BDB1C8FC000000000000000000000000000000000000000000000000000000000000000000000000000000000A5701088E00000054001A000055D9000003B600000001000001900000001400000000001A000055DA0000038400000002000001F40000001400000000001A000055DB0000035200000004000002580000001400000000001A000055DC0000034A00000004000002670000001400000000001A000055DD0000000000000000000000000000000000000000001A000055DE0000000000000000000000000000000000000000001A000055DF000003E8000000010000001E0000000A00000000001A000055E0000003E800000002000000500000000A00000000001A000055E1000003E800000004000000820000000A00000000001A000055E2000003E800000006000000BE0000000A00000000001A000055E30000000000000000000000000000000000000000001A000055E40000000000000000000000000000000000000000001A000055E5000003E800000002000000C80000000A00000000001A000055E6000003E8000000030000012C0000000A00000000001A000055E7000003E800000004000001900000000A00000000001A000055E8000003E800000006000002580000000A00000000001A000055E90000000000000000000000000000000000000000001A000055EA0000000000000000000000000000000000000000001A000055EB0000000000000000000000000000000000000000001A000055EC0000000000000000000000000000000000000000001A000055ED0000000000000000000000000000000000000000001A000055EE0000000000000000000000000000000000000000001A000055EF0000000000000000000000000000000000000000001A000055F00000000000000000000000000000000000000000001A000055F10000000000000000000000000000000000000000001A000055F20000000000000000000000000000000000000000001A000055F30000000000000000000000000000000000000000001A000055F40000000000000000000000000000000000000000001A000055F50000000000000000000000000000000000000000001A000055F60000000000000000000000000000000000000000001A000055F70000000000000000000000000000000000000000001A000055F80000000000000000000000000000000000000000001A000055F90000000000000000000000000000000000000000001A000055FA0000000000000000000000000000000000000000001A000055FB0000000000000000000000000000000000000000001A000055FC0000000000000000000000000000000000000000001A000055FD000000C800000001000003E80000000A00000000001A000055FE0000009600000002000003E80000000A00000000001A000055FF0000007800000004000003E80000000A00000000001A000056000000005000000006000003E80000000A00000000001A000056010000000000000000000000000000000000000000001A000056020000000000000000000000000000000000000000001A000056030000000000000000000000000000000000000000001A000056040000000000000000000000000000000000000000001A000056050000000000000000000000000000000000000000001A000056060000000000000000000000000000000000000000001A000056070000000000000000000000000000000000000000001A00005608000000000000000000000000000000000000000A001A00005609000003E800000001000000640000000A00000000001A0000560A000003E800000003000000960000000800000000001A0000560B000003E800000005000000C80000000800000000001A0000560C000003E800000007000000FA0000000800000000001A0000560D0000000000000000000000000000000000000000001A0000560E0000000000000000000000000000000000000000001A0000560F000003E800000001000000640000000800000000001A00005610000003E800000003000000960000000800000000001A00005611000003E800000005000000C80000000800000000001A00005612000003E800000007000000FA0000000800000000001A000056130000000000000000000000000000000000000000001A000056140000000000000000000000000000000800000000001A00005615000003E800000002000000280000000500000000001A00005616000003E800000003000000460000000500000000001A00005617000003E8000000040000006E0000000500000000001A00005618000003E800000005000000A00000000500000000001A000056190000000000000000000000000000000000000000001A0000561A0000000000000000000000000000000000000000001A0000561B000003E800000001000000320000000A00000000001A0000561C000003E800000003000000500000000A00000000001A0000561D000003E800000005000000780000000A00000000001A0000561E000003E800000007000000AA0000000A00000000001A0000561F0000000000000000000000000000000000000000001A000056200000000000000000000000000000000000000000001A000056270000000000000000000000000000000000000000001A000056280000000000000000000000000000000000000000001A000056290000000000000000000000000000000000000000001A0000562A0000000000000000000000000000000000000000001A0000562B0000000000000000000000000000000000000000001A0000562C0000000000000000000000000000000000000000001A0000562D0000000000000000000000000000000000000000001A0000562E0000000000000000000000000000000000000000001A0000562F0000000000000000000000000000000000000000001A000056300000000000000000000000000000000000000000001A000056310000000000000000000000000000000000000000001A0000563200000000000000000000000000000000000000000000000F001E0000586D000000040000000000000000000000000000000100000000001E000081F7000000040000000000000000000000000000000100000000001E000081F8000000030000000000000000000000000000000000000000001E000081F9000000030000000100000001000000010000000000000000001E000081FB000000020000000000000001000000000000000000000000001E00008659000000040000000000000000000000000000000100000000001E0000EA7B000000030000000000000000000000000000000000000000001E000111A2000000030000000000000000000000000000000000000000001E0001392C000000030000000000000000000000000000000000000000001E0001414C000000040000000000000000000000000000000100000000001E0001AAF7000000040000000000000000000000000000000100000000001E0001ABFC000000040000000000000000000000000000000100000000001E0001D124000000040000000000000000000000000000000100000000001E0001F282000000050000000000000000000000000000000100000001001E0002161700000004000000000000000000000000000000010000000000443D00413E31302E32362D31302E323720D4DACFDFCBCD41B3B52DCCFDB7E7D5DF2B41B3B52DB3ACC4DCD5BDB3B52B3130383838B5E3C8AF2B36B1B6BFAAC6F4A3A10000070000000000002C0028B9D9B7BDCCE1D0D1A3BACCE1B7C0D0E9BCD9D0C5CFA2A3ACC7EBCEF0C9CFB5B1CADCC6AD20A3A10000040201100001B695000170BC000166480000558D00015F53000147060000544A00005310000042D10000558C00004B65000051280000558E0000586C00005C7900004CA50000000728687474703A2F2F73706565642E71712E636F6D2F6163742F6132303132313230346368616E67652F00050F050A000000035A0500000000060000006800000070000000BA0000008F0000009E000000A60000000A00640128687474703A2F2F73706565642E71712E636F6D2F6163742F6132303132313230346368616E67652F000F4240000004B013F40034001100000098000000270000000000000000110000009E0000002700000000000"
			"00000110000009F0000001D000000000000000011000000A000000029000000000000000011000000A200000028000000000000000011000000A400000016000000000000000011000000A500000024000000000000000011000000A60000002C000000000000000011000000A700000021000000000000000011000000BA00000038000000000000000011000000BB00000070000000000000000011000000BE0000003D000000000000000011000000C500000031000000000000000011000000C600000023000000000000000011000000DB0000002B000000000000000011000000DE00000022000000000000000011000000E70000001F000000000000000011000000EA0000005C000000000000000011000000EB00000037000000000000000011000000ED00000030000000000000000011000000EE00000030000000000000000011000000FA0000002F000000000000000011000000FD00000033000000000000000011000000FF000000440000000000000000110000010000000038000000000000000011000001010000004D00000000000000001100000102000000380000000000000000110000010400000038000000000000000011000001050000002F0000000000000000110000010600000021000000000000000011000001080000002D000000000000000011000001090000003E0000000000000000110000010B0000001F0000000000000000110000010C000000260000000000000000110000010D00000041000000000000000011000001110000003D0000000000000000110000011500000043000000000000000011000001160000001A000000000000000011000001170000001E00000000000000001100000119000000340000000000000000110000011A000000270000000000000000110000011B00000030000000000000000011000000780000002C00000000000000001100000085000000560000000000000000110000008A000000330000000000000000110000009000000029000000000000000011000000E200000047000000000000000011000000B900000048000000000000000011000000BD0000004A000000000000000011000000BF00000047000000000000000011000001140000004D0000000000000000110000012200000028000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000034001100000098000600270000000000000000110000009E000400270000000000000000110000009F0001001D000000000000000011000000A000010029000000000000000011000000A2000F0028000000000000000011000000A4000C001E000000000000000011000000A5000C0027000000000000000011000000A60001002C000000000000000011000000A700250048000000000000000011000000AD000E0020000000000000000011000000BA00010038000000000000000011000000BB00070075000000000000000011000000BE00030040000000000000000011000000C500010031000000000000000011000000C60001002F000000000000000011000000DB001A0041000000000000000011000000DE00230043000000000000000011000000E7001F005F000000000000000011000000EA0001005C000000000000000011000000EB00010037000000000000000011000000ED00040030000000000000000011000000EE0030007C000000000000000011000000FD00090033000000000000000011000000FF0001004400000000000000001100000100004B0038000000000000000011000001010003004D00000000000000001100000102000E00380000000000000000110000010400110057000000000000000011000001050017003A00000000000000001100000106000F002800000000000000001100000108000F003600000000000000001100000109000E00250000000000000000110000010B001100300000000000000000110000010C001D004E0000000000000000110000010D00110041000000000000000011000001110001003D00000000000000001100000115002F0057000000000000000011000001160004001A000000000000000011000001170003001E00000000000000001100000119000400340000000000000000110000011A000300270000000000000000110000011B001800530000000000000000110000007800050037000000000000000011000000850024007E0000000000000000110000008A0003003B000000000000000011000000900008002F000000000000000011000000E20019004E000000000000000011000000B9003F0088000000000000000011000000BD000B004B000000000000000011000000BF001C007200000000000000001100000114002C0079000000000000000011000001220004002A000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000035001100000098000600000000000000000000110000009E000400000000000000000000110000009F00010000000000000000000011000000A0000F0000000000000000000011000000A2000F0000000000000000000011000000A400150000000000000000000011000000A5000C0000000000000000000011000000A600010000000000000000000011000000A700440067000000000000000011000000AD00210000000000000000000011000000B000030000000000000000000011000000BA00010000000000000000000011000000BB00330000000000000000000011000000BE00180000000000000000000011000000C500230000000000000000000011000000C600220000000000000000000011000000DB001A0000000000000000000011000000DE0047006E000000000000000011000000E7005D0098000000000000000011000000EA001F0000000000000000000011000000EB000F0000000000000000000011000000ED003A0000000000000000000011000000EE00300083000000000000000011000000FD00130000000000000000000011000000FF0017000000000000000000001100000100001C0000000000000000000011000001010012000000000000000000001100000102000E000000000000000000001100000104007E00A800000000000000001100000105002C00000000000000000000110000010600210000000000000000000011000001080025000000000000000000001100000109003E00000000000000000000110000010B002200000000000000000000110000010C003200000000000000000000110000010D003600000000000000000000110000011100130000000000000000000011000001150041006700000000000000001100000116000E0000000000000000000011000001170014000000000000000000001100000119001B00000000000000000000110000011A001300000000000000000000110000011B002A0070000000000000000011000000780007000000000000000000001100000085002A00A00000000000000000110000008A000E00000000000000000000110000009000080000000000000000000011000000E200180000000000000000000011000000B90051009C000000000000000011000000BD001D0000000000000000000011000000BF003F000000000000000000001100000114004A008F0000000000000000110000012200040000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000110000000000000000000000000000000011000000000000000000000000000000001100000000000000000000000000000000000000040000007800000004000000850000007D000000750000006F00000069000000640000000001000000000A000001F400000019";
		const unsigned char* Msg800 = EHexToByte(Byte.data());
		SendToClient(Client, 800, (const BYTE*)Msg800, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		delete[] Msg800;
		return;
	}


	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 5000); //GuildScoreThreshold

	{ //LuckyMatchCfg 
		BYTE* pLuckyMatchCfg = p;
		Write16(pLuckyMatchCfg, 0); //len

		const char Time[50] = "10～20分钟";
		memcpy(pLuckyMatchCfg, Time, 50);
		pLuckyMatchCfg += 50;

		const char Award[50] = "海量改装道具拿不停！";
		memcpy(pLuckyMatchCfg, Award, 50);
		pLuckyMatchCfg += 50;

		const char StartCondition[50] = "满32人开赛";
		memcpy(pLuckyMatchCfg, StartCondition, 50);
		pLuckyMatchCfg += 50;

		Write32(pLuckyMatchCfg, 3000); //LuckyMoney

		const char MatchTitle[50] = "";
		memcpy(pLuckyMatchCfg, MatchTitle, 50);
		pLuckyMatchCfg += 50;


		len = pLuckyMatchCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //MsgStoneSysCfg
		BYTE* pMsgStoneSysCfg = p;
		Write16(pMsgStoneSysCfg, 0); //len

		Write8(pMsgStoneSysCfg, 1); //StoneSkillTakeEffect

		{ //SkillStoneCfg
			BYTE* pSkillStoneCfg = pMsgStoneSysCfg;
			Write16(pSkillStoneCfg, 0); //len

			BYTE* pSkillStoneNum = pSkillStoneCfg;
			Write32(pSkillStoneCfg, 0); //SkillStoneNum
			size_t Num = MSingleSkillStoneCfg->SkillStoneCfg.size();
			for (size_t i = 0; i < Num; i++)
			{
				//SingleSkillStoneCfg
				BYTE* pSingleSkillStoneCfg = pSkillStoneCfg;
				Write16(pSingleSkillStoneCfg, 0); //len

				Write32(pSingleSkillStoneCfg, MSingleSkillStoneCfg->SkillStoneCfg[i].StoneID);
				Write32(pSingleSkillStoneCfg, MSingleSkillStoneCfg->SkillStoneCfg[i].ActiveCondVal1);
				Write32(pSingleSkillStoneCfg, MSingleSkillStoneCfg->SkillStoneCfg[i].UseCountUpperlimit);
				Write32(pSingleSkillStoneCfg, MSingleSkillStoneCfg->SkillStoneCfg[i].ActiveSuccessProb);
				Write32(pSingleSkillStoneCfg, MSingleSkillStoneCfg->SkillStoneCfg[i].CoolTime);
				Write32(pSingleSkillStoneCfg, MSingleSkillStoneCfg->SkillStoneCfg[i].GenResultVal1);

				len = pSingleSkillStoneCfg - pSkillStoneCfg;
				Set16(pSkillStoneCfg, (WORD)len);
				pSkillStoneCfg += len;
			}
			Set32(pSkillStoneNum, Num);

			len = pSkillStoneCfg - pMsgStoneSysCfg;
			Set16(pMsgStoneSysCfg, (WORD)len);
			pMsgStoneSysCfg += len;
		}

		Write32(pMsgStoneSysCfg, 0); //Kart2StoneGrooveNum
		/*
m_stMsgStoneSysCfg.m_astKart2StoneGrooveCfg[].m_iKartID
m_stMsgStoneSysCfg.m_astKart2StoneGrooveCfg[].m_iInitStoneGrooveNum
m_stMsgStoneSysCfg.m_astKart2StoneGrooveCfg[].m_aiStoneUseOccasionType[MAX_KART_STONE_GROOVE_NUM]
		*/


		len = pMsgStoneSysCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}


	{ //UITitleDispInfo 标题显示设置
		BYTE* pUITitleDispInfo = p;
		Write16(pUITitleDispInfo, 0); //len

		struct
		{
			bool GameName : 1;
			bool Version : 1;
			bool WorldName : 1;
			bool Ad : 1;
			bool NickName : 1;
			bool Unknown1 : 1;
			bool Unknown2 : 1;
			bool Unknown3 : 1;
		} DisplayBitMap{ DisplayBitMapConfig[0],DisplayBitMapConfig[1],DisplayBitMapConfig[2],DisplayBitMapConfig[3],DisplayBitMapConfig[4],0,0,0 };
		memcpy(pUITitleDispInfo, &DisplayBitMap, sizeof(DisplayBitMap));
		pUITitleDispInfo += sizeof(DisplayBitMap);

		{ //ADText
			BYTE* pADText = pUITitleDispInfo;
			Write16(pADText, 0); //len

			len = strlen(UITitleDispInfo_ADText.c_str());
			Write8(pADText, (BYTE)len); //TextLen
			memcpy(pADText, UITitleDispInfo_ADText.c_str(), len);
			pADText += len;

			len = pADText - pUITitleDispInfo;
			Set16(pUITitleDispInfo, (WORD)len);
			pUITitleDispInfo += len;
		}

		len = pUITitleDispInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //LoaderTipInfo 
		BYTE* pLoaderTipInfo = p;
		Write16(pLoaderTipInfo, 0); //len

		Write32(pLoaderTipInfo, 0); //Version
		Write8(pLoaderTipInfo, 0); //TipsNum
		//m_stLoaderTipInfo.m_astLoaderTipText[].m_u8TextLen


		len = pLoaderTipInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //MailSysAD
		BYTE* pMailSysAD = p;
		Write16(pMailSysAD, 0); //len

		const char* ADText = "Mail";
		len = strlen(ADText);
		Write16(pMailSysAD, (WORD)len); //TextLen
		memcpy(pMailSysAD, ADText, len);
		pMailSysAD += len;


		len = pMailSysAD - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //CheatReportCfg 作弊报告配置
		BYTE* pCheatReportCfg = p;
		Write16(pCheatReportCfg, 0); //len

		Write8(pCheatReportCfg, 1); //ReplayDayNum
		Write8(pCheatReportCfg, 1); //IsReportCheatEnable

		len = pCheatReportCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //MaintainableKartNum
	//m_aiMaintainableKartID[]


	Write32(p, 1); //CommonBitSwitch
	//WriteString_MC(p, "http://speed.qq.com/act/a20121204change/");//QQKart2ButtonLen
	Write8(p, 1); //QQKart2ButtonLen

	{ //GuildEnlistCfg 登记配置
		BYTE* pGuildEnlistCfg = p;
		Write16(pGuildEnlistCfg, 0); //len

		Write8(pGuildEnlistCfg, 0); //MinUnloginDay
		Write8(pGuildEnlistCfg, 0); //CdDay
		Write8(pGuildEnlistCfg, 0); //MaxAwardTimeOneDay 

		len = pGuildEnlistCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 1); //CloseQT
	Write32(p, 0); //QTCommonRoomID
	Write8(p, 0); //WlFetionDayLimit
	Write8(p, 1); //ShowShopIcon
	Write32(p, 0); //TiroRecommendMapNum
	//m_aiTiroRecommendMapIdList[]
	Write16(p, 0); //BoxMutiOpenLimit
	Write8(p, 1); //HallButtonShow
	Write8(p, 0); //HallButtonShowUrlLen
	Write32(p, 0); //EndlessModeBaseScorePerLevel
#ifndef ZingSpeed
	Write32(p, 0); //GetShopNonSaleInfoInterval
	{ //ShuttleRandMap
		BYTE* pShuttleRandMap = p;
		Write16(pShuttleRandMap, 0); //len

		Write16(pShuttleRandMap, 0); //BeginNum
		/*
		for (size_t i = 0; i < n; i++)
		{ //BeginMapList
			BYTE* pBeginMapList = pShuttleRandMap;
			Write16(pBeginMapList, 0); //len

			Write32(pBeginMapList, 0); //MapID
			Write16(pBeginMapList, 0); //BeginIdx
			Write16(pBeginMapList, 0); //EndIdx
			Write8(pBeginMapList, 0); //Reverse
			Write16(pBeginMapList, 0); //BeginCheckPointIdx

			len = pBeginMapList - pShuttleRandMap;
			Set16(pShuttleRandMap, (WORD)len);
			pShuttleRandMap += len;
		}
		*/
		Write16(pShuttleRandMap, 0); //MidNum
		/*
		for (size_t i = 0; i < n; i++)
		{ //MidMapList
			BYTE* pMidMapList = pShuttleRandMap;
			Write16(pMidMapList, 0); //len

			Write32(pMidMapList, 0); //MapID
			Write16(pMidMapList, 0); //BeginIdx
			Write16(pMidMapList, 0); //EndIdx
			Write8(pMidMapList, 0); //Reverse
			Write16(pMidMapList, 0); //BeginCheckPointIdx

			len = pMidMapList - pShuttleRandMap;
			Set16(pShuttleRandMap, (WORD)len);
			pShuttleRandMap += len;
		}
		*/
		Write16(pShuttleRandMap, 0); //EndNum
		/*
		for (size_t i = 0; i < n; i++)
		{ //EndMapList
			BYTE* pEndMapList = pShuttleRandMap;
			Write16(pEndMapList, 0); //len

			Write32(pEndMapList, 0); //MapID
			Write16(pEndMapList, 0); //BeginIdx
			Write16(pEndMapList, 0); //EndIdx
			Write8(pEndMapList, 0); //Reverse
			Write16(pEndMapList, 0); //BeginCheckPointIdx

			len = pEndMapList - pShuttleRandMap;
			Set16(pShuttleRandMap, (WORD)len);
			pShuttleRandMap += len;
		}
		*/
		len = pShuttleRandMap - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //HasWeeklyRecommendModeClientInfo
	/*
	for (size_t i = 0; i < n; i++)
	{ //WeeklyRecommendModeClientInfo
		BYTE* pWeeklyRecommendModeClientInfo = p;
		Write16(pWeeklyRecommendModeClientInfo, 0); //len

		Write32(pWeeklyRecommendModeClientInfo, 0); //OpenDate
		Write32(pWeeklyRecommendModeClientInfo, 0); //CloseDate
		Write32(pWeeklyRecommendModeClientInfo, 0); //OpenTime
		Write32(pWeeklyRecommendModeClientInfo, 0); //CloseTime
		Write8(pWeeklyRecommendModeClientInfo, 0); //WeekLimitNum
		Write8(pWeeklyRecommendModeClientInfo, 0); //WeekLimits[]
		Write32(pWeeklyRecommendModeClientInfo, 0); //DesktopSpecialShowFlag
		Write16(pWeeklyRecommendModeClientInfo, 0); //BaseMode
		Write16(pWeeklyRecommendModeClientInfo, 0); //SubMode
		Write32(pWeeklyRecommendModeClientInfo, 0); //SpecialFlag
		Write(pWeeklyRecommendModeClientInfo, 0); //AwardTriggerDesc[]
		Write8(pWeeklyRecommendModeClientInfo, 0); //AwardNum
		Write32(pWeeklyRecommendModeClientInfo, 0); //AwardItemID[]

		len = pWeeklyRecommendModeClientInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write32(p, 0); //ReportWeeklyRecommendModeTriggerInfoInterval
	Write32(p, 0); //MaxEnergy
	Write32(p, 0); //N2ORatio
	Write32(p, 0); //PlayerNumRatio[]
	Write32(p, 1); //IsOpenBuf
	Write8(p, 1); //CouponTreasureGameButtonShow
	Write8(p, 1); //NewYearFinancingShow 
	Write32(p, 0); //GuildTeamSignatureReportNumsLimit
	Write32(p, 0); //GetRelationItemMaxTimeout
	Write32(p, 0); //ClientWaitGameBeginTimeout

#endif

	len = p - buf.data();
	SendToClient(Client, 800, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifySvrConfig2(std::shared_ptr<ClientNode> Client)
{
	if (Uinew) {
		std::string Byte = "0000000B020301020300000064000000000200011C1800011C1A000200011C1800011C1A00000001014E00000052000000700000007600000085000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000BF000000C2000000C6000000CC000000D4000000DB000000DE000000E6000000E7000000EB000000EE000000F6000000FD000000FE000001000000010400000105000001080000010A0000010B0000010C000001110000011300000115000001190000011C0000012200000124000001270000012A0000012B0000012D0000013300000140000001420000014600000148000001490000014C0000015F0000016C0000016D0000017A0000017D0000017E0000018000000185000001890000018A0000018B0000018C0000018D0000018E0000018F0000019000000194000001960000019F000001A0000001A2000001A4000001A5000001A6000001A7000001A8000001B2000001B3000001C700DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A600060000000000060000000000DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A6014E00000052000000700000007600000085000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000BF000000C2000000C6000000CC000000D4000000DB000000DE000000E6000000E7000000EB000000EE000000F6000000FD000000FE000001000000010400000105000001080000010A0000010B0000010C000001110000011300000115000001190000011C0000012200000124000001270000012A0000012B0000012D0000013300000140000001420000014600000148000001490000014C0000015F0000016C0000016D0000017A0000017D0000017E0000018000000185000001890000018A0000018B0000018C0000018D0000018E0000018F0000019000000194000001960000019F000001A0000001A2000001A4000001A5000001A6000001A7000001A8000001B2000001B3000001C7000600000000014E00000052000000700000007600000085000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000BF000000C2000000C6000000CC000000D4000000DB000000DE000000E6000000E7000000EB000000EE000000F6000000FD000000FE000001000000010400000105000001080000010A0000010B0000010C000001110000011300000115000001190000011C0000012200000124000001270000012A0000012B0000012D0000013300000140000001420000014600000148000001490000014C0000015F0000016C0000016D0000017A0000017D0000017E0000018000000185000001890000018A0000018B0000018C0000018D0000018E0000018F0000019000000194000001960000019F000001A0000001A2000001A4000001A5000001A6000001A7000001A8000001B2000001B3000001C7008A00000021000000890000008F0000009E000000A6000000A7000000AB000000BB000000BE000000C6000000D4000000DB000000E6000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A6014E00000052000000700000007600000085000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000BF000000C2000000C6000000CC000000D4000000DB000000DE000000E6000000E7000000EB000000EE000000F6000000FD000000FE000001000000010400000105000001080000010A0000010B0000010C000001110000011300000115000001190000011C0000012200000124000001270000012A0000012B0000012D0000013300000140000001420000014600000148000001490000014C0000015F0000016C0000016D0000017A0000017D0000017E0000018000000185000001890000018A0000018B0000018C0000018D0000018E0000018F0000019000000194000001960000019F000001A0000001A2000001A4000001A5000001A6000001A7000001A8000001B2000001B3000001C700DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A6003A0000000D00000070000000890000008F000000970000009E000000A2000000A6000000A7000000BA000000C6000000DB000000EB000000EE00060000000000DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A600DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A600060000000000060000000000060000000000060000000000060000000000060000000000060000000000DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A6000600000000014E00000052000000700000007600000085000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000BF000000C2000000C6000000CC000000D4000000DB000000DE000000E6000000E7000000EB000000EE000000F6000000FD000000FE000001000000010400000105000001080000010A0000010B0000010C000001110000011300000115000001190000011C0000012200000124000001270000012A0000012B0000012D0000013300000140000001420000014600000148000001490000014C0000015F0000016C0000016D0000017A0000017D0000017E0000018000000185000001890000018A0000018B0000018C0000018D0000018E0000018F0000019000000194000001960000019F000001A0000001A2000001A4000001A5000001A6000001A7000001A8000001B2000001B3000001C700060000000000060000000000DE0000003600000070000000890000008F000000970000009E000000A2000000A6000000A7000000AB000000BA000000BB000000BE000000C6000000D4000000DB000000E6000000E7000000EB000000EE000000F6000000FD000001000000010400000105000001080000010B0000010C000001110000011300000115000001190000011C00000122000001270000012A0000012B0000012D00000133000001420000014600000148000001490000014C0000015F0000017A0000017E000001800000018B00000190000001960000019F000001A0000001A5000001A600060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000060000000000000000000002004A000001C50042B1BED4C233D0C7D0C2C8FCB5C0A3BA2F433033A1BEB7C9B3DBD0C2BDAEA1BF2F4330300AC8FCB5C0C8CECEF1A3BA2F433032A1BEB7C9B3DBBBD5D5C2A1BF2F433030004A000001C70042B1BED4C233D0C7D0C2C8FCB5C0A3BA2F433033A1BEC7A7C4EACBBFC2B7A1BF2F4330300AC8FCB5C0C8CECEF1A3BA2F433032A1BECBBFC2B7BBD5D5C2A1BF2F433030000000000000000000010000000000000000010000000D002F0001283300010000018E00010000006B0133A1340133A1F200038270000399B700000000000000000000000000003700014BA100020000030800000309000200000084000000850133C9DD0133ECA200000000000382700100000325000003080133EC9E003700014E9100020000032000000321000200000090000000910133ECA30133ED4D000000000003827001000003220000032000000000002B00014ED100010000032400000000000000000000000000000000000000000000000000000000000000002B00015E550001000003C800000000000000000000000000000000000000000000000000000000000000002B00015E540001000003C900000000000000000000000000000000000000000000000000000000000000002B00015E530001000003CA00000000000000000000000000000000000000000000000000000000000000002B000170BB00010000041D00000000000000000000000000000000000000000000000000000000000000002B00016CF000010000043800000000000000000000000000000000000000000000000000000000000000002B00016CF0000100000439000000000000000000000000000000000000000000000000000000000000000037000156780002000003680000036900020000009B0000009C0133EDC40133EE250000000000038270010000036A0000036800000000002F0001567700020000037C0000037D00000000000000000000000000000000000000000003810000037C00000000002F0001567900020000037E0000037F00000000000000000000000000000000000000000003800000037E000000000007180000138805A0000001F40000050A0F0101008B00000000000000000000000100000000000000";
		const unsigned char* Msg822 = EHexToByte(Byte.data());
		SendToClient(Client, 822, (const BYTE*)Msg822, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		delete[] Msg822;
		return;
	}



	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write8(p, 0); //HaveTopMapRecord

	//{ //TopMapRecordConfig
	//	BYTE* pTopMapRecordConfig = p;
	//	Write16(pTopMapRecordConfig, 0); //len

	//	Write32(pTopMapRecordConfig, 0); //Version
	//	Write32(pTopMapRecordConfig, 0); //RecordDisplayLimit
	//	Write8(pTopMapRecordConfig, 1); //RecordNumber
	//	for (size_t i = 0; i < 1; i++)
	//	{ //Record
	//		BYTE* pRecord = pTopMapRecordConfig;
	//		Write16(pRecord, 0); //len

	//		Write32(pRecord, 112); //MapId
	//		Write32(pRecord, 10000); //Record
	//		Write32(pRecord, 10001); //Uin
	//		Write8(pRecord, 1); //WorldId
	//		WriteString_MC(pRecord, "测试"); //NameLength
	//		Write8(pRecord, 0); //UrlLength

	//		len = pRecord - pTopMapRecordConfig;
	//		Set16(pTopMapRecordConfig, (WORD)len);
	//		pTopMapRecordConfig += len;
	//	}

	//	len = pTopMapRecordConfig - p;
	//	Set16(p, (WORD)len);
	//	p += len;
	//}

	Write8(p, 0); //HaveKartRoomPushInfo
	/*
	{ //KartRoomPushInfo
		BYTE* pKartRoomPushInfo = p;
		Write16(pKartRoomPushInfo, 0); //len

		Write32(pKartRoomPushInfo, 0); //Version
		Write16(pKartRoomPushInfo, 0); //PushItemNumber
		for (size_t i = 0; i < n; i++)
		{ //PushItem
			BYTE* pPushItem = pKartRoomPushInfo;
			Write16(pPushItem, 0); //len

			Write32(pPushItem, 0); //KartId
			Write8(pPushItem, 0); //TextLength

			len = pPushItem - pKartRoomPushInfo;
			Set16(pKartRoomPushInfo, (WORD)len);
			pKartRoomPushInfo += len;
		}

		len = pKartRoomPushInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	{ //ClickStreamCfg
		BYTE* pClickStreamCfg = p;
		Write16(pClickStreamCfg, 0); //len

		Write8(pClickStreamCfg, 2); //ReportMode

		Write8(pClickStreamCfg, 3); //ContextNum
		Write8(pClickStreamCfg, 1);
		Write8(pClickStreamCfg, 2);
		Write8(pClickStreamCfg, 3);

		Write32(pClickStreamCfg, 100); //MaxNum

		len = pClickStreamCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 1); //IsGuildPKOpen
	Write8(p, 1); //IsEightYearsOpen
	Write8(p, 1); //HaveSkatePropGameLogicCfg滑冰道具游戏逻辑配置

	{ //SkatePropGameLogicCfg
		BYTE* pSkatePropGameLogicCfg = p;
		Write16(pSkatePropGameLogicCfg, 0); //len

		Write32(pSkatePropGameLogicCfg, 0); //PropAddN2ONum
		Write32(pSkatePropGameLogicCfg, 0); //N2OSpeed
		Write32(pSkatePropGameLogicCfg, 0); //ZanAddScoreNum
		Write32(pSkatePropGameLogicCfg, 0); //GetSpecialPropAddCoinNum
		Write32(pSkatePropGameLogicCfg, 0); //SkatePropHitCD
		Write32(pSkatePropGameLogicCfg, 0); //SkatePropHit_CoinSubNum
		Write32(pSkatePropGameLogicCfg, 0); //MaxOneGameLogicGetScoreNum

		len = pSkatePropGameLogicCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write16(p, 0); //SkatePropShoeNum
	//Write32(p, 0); //SkatePropShoeList[]

	Write16(p, 0); //SkatePropShoeNumForMultiGame
	//Write32(p, 0); //SkatePropShoeListForMultiGame[]

	Write32(p, 1); //AddAngleTime

	const int EMBM_MAX = 47;
	for (size_t i = 0; i < EMBM_MAX; i++)
	{ //ClassicMapInfo
		BYTE* pClassicMapInfo = p;
		Write16(pClassicMapInfo, 0); //len

		Write32(pClassicMapInfo, 0); //MapNums
		//Write32(pClassicMapInfo, 0); //MapIDS[]

		len = pClassicMapInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //OpenPopAds
	Write8(p, 0); //IsCloseSaveKeyTransInfo		是否关闭保存密钥TransInfo事务处理信息
	Write8(p, 0); //IsCloseSuperMoneySecondPsw	是否关闭二级密码


	Write32(p, NewMaps.size()); //NewMapNum
	for (size_t i = 0; i < NewMaps.size(); i++)
	{ //NewMapTipsCfg
		BYTE* pNewMapTipsCfg = p;
		Write16(pNewMapTipsCfg, 0); //len

		USHORT WordLen = NewMaps[i].WordTxt.length();

		Write32(pNewMapTipsCfg, NewMaps[i].MapID); //Mapid
		Write16(pNewMapTipsCfg, WordLen); //WordLen
		memcpy(pNewMapTipsCfg, NewMaps[i].WordTxt.data(), WordLen);
		pNewMapTipsCfg += WordLen;

		len = pNewMapTipsCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}


	for (size_t i = 0; i < 16; i++)
	{
		Write8(p, 0); //ClientSwitch[]
	}

	Write8(p, 1); //OpenPwdRedEnvelop 口令红包打开标志
	Write8(p, 0); //IsUseTCLS
	Write8(p, 0); //IsPropTeamMatchShow

	Write32(p, 0); //CfgNums
	/*
	for (size_t i = 0; i < 1; i++)
	{ //CommonItemTopListInfos
		BYTE* pCommonItemTopListInfos = p;
		Write16(pCommonItemTopListInfos, 0); //len

		Write32(pCommonItemTopListInfos, 75827); //ItemID

		Write16(pCommonItemTopListInfos, 1); //ExtendInfoNums
		Write32(pCommonItemTopListInfos, 0); //ExtendInfoForItemGene[]

		Write16(pCommonItemTopListInfos, 1); //UpdateTopListNums
		Write32(pCommonItemTopListInfos, 0); //TopListID[]

		Write32(pCommonItemTopListInfos, 0); //ActiveDate
		Write32(pCommonItemTopListInfos, 0); //InActiveDate
		Write32(pCommonItemTopListInfos, 0); //ActiveTime
		Write32(pCommonItemTopListInfos, 0); //InActiveTime
		Write8(pCommonItemTopListInfos, 1); //UpdateTopList
		Write32(pCommonItemTopListInfos, 0); //ExtendInfoForLastUpdateTime
		Write32(pCommonItemTopListInfos, 0); //DailyTopListExtendInfoID
		Write32(pCommonItemTopListInfos, 0); //UpdateDate

		len = pCommonItemTopListInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 1); //IsShowBoardAllareaBubble
	Write8(p, 3); //RemindClearSuperMoneyLeftDays
	Write8(p, 24); //RemindClearSuperMoneyLeftHours
	Write32(p, 0); //RemindClearSuperMoneyThreshold
	Write16(p, 0); //RemindClearSuperMoneyInterval
	Write32(p, 0); //HPJMaxAccFuel
	Write32(p, 0); //HPJWildStatusAccelParam
	Write8(p, 0); //SwitchNum
	Write8(p, 1); //UseSign3使用标志3
	Write8(p, 1); //OpenRankedMatchSpeedKing
	Write8(p, 0); //OpenLimitMapCollect
	Write8(p, 0); //HasMoreInfo

	len = p - buf.data();
	SendToClient(Client, 822, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifySvrConfig3(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;
	if (Uinew) {
		WriteHex(p, "02930000000004001300000001000000640000177000000C80000013000007D1000000640000177000000244000013000007D20000006400001770000010040000130001869F0000006400014438000013880007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000220000012C000000AA000000F0000000F0000000C8000000BE000000C8FFFFFFE200160000012C0000012C000000C8000000FA000186A0000003E80000044C00002EE000001644000002BC0000009600004A38FFFFC568FFFFF6A0000046500000C35000003E80000005DC0000170C0000106800001770000017700000157C00000A28000003E80000032000000DAC000013880000271006000004B0000004B0000003200000012C000007D00000076C0D000A0000000000000FA0000A0000138800000FA0000A0000164400001194000A0000232800001388000A000024540000157C000A00002AF8000016A8000A00002E18000016A8000A00003A980000189C000A00003E800000189C000A00004E200000189C000A000061A80000189C000A000075300000189C000A000088B80000189C05000A00000000FFFFE0C0000A000007D0FFFFE4A8000A00000FA0FFFFE890000A00001770FFFFEC78000A00004650FFFFF06005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA8405000A0000000000003E80000A000007D000003E80000A00000FA000003E80000A0000177000003A98000A0000271000003A980000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012700004E2000000000000007D000000064000000640000000100000014000000640D000A0000000000000FA0000A0000138800000FA0000A0000164400001194000A0000232800001388000A000024540000157C000A00002AF80000157C000A00002E18000015E0000A00003A9800001770000A00003E8000001770000A00004E2000001770000A000061A800001770000A0000753000001770000A000088B80000177005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000280000003C07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0012700004650000000000000079E00000069000000640000000100000014000000640D000A0000000000001068000A0000138800001068000A000016440000125C000A0000232800001450000A0000245400001644000A00002AF800001644000A00002E1800001644000A00003A9800001838000A00003E8000001900000A00004E2000001900000A000061A800001900000A0000753000001900000A000088B80000190005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000280000004107000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0012700003E80000000000000076C0000006E000000640000000100000014000000640D000A0000000000001068000A0000138800001068000A000016440000125C000A0000232800001450000A0000245400001644000A00002AF800001644000A00002E18000016A8000A00003A98000019C8000A00003E8000001A90000A00004E2000001A90000A000061A800001A90000A0000753000001A90000A000088B800001A9005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000230000004607000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0012700002EE0000000640000073A0000007300000064000000010000001E000000640D000A0000000000000FA0000A0000138800000FA0000A0000164400001130000A0000232800001388000A0000245400001518000A00002AF8000015E0000A00002E1800001644000A00003A9800001A90000A00003E8000001B58000A00004E2000001B58000A000061A800001B58000A0000753000001B58000A000088B800001B5805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000190000005007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0012700002AF800000064000007080000007800000064000000010000001E0000001E0D000A0000000000001004000A0000138800001004000A0000164400001194000A0000232800001388000A0000245400001644000A00002AF80000170C000A00002E1800001770000A00003A9800001C20000A00003E8000001C20000A00004E2000001C20000A000061A800001C20000A0000753000001C20000A000088B800001C2005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000190000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B001270000271000000064000006D60000007D000000640000000100000064000000640D000A0000000000001004000A0000138800001004000A00001644000011F8000A00002328000013EC000A00002454000015E0000A00002AF800001770000A00002E18000017D4000A00003A9800001CE8000A00003E8000001CE8000A00004E2000001CE8000A000061A800001CE8000A0000753000001CE8000A000088B800001CE805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000190000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B001270000232800000064000006A400000087000000640000000100000064000000640D000A0000000000001130000A00001388000010CC000A00001644000012C0000A0000232800001388000A000024540000157C000A00002AF800001838000A00002E1800001900000A00003A9800001DB0000A00003E8000001DB0000A00004E2000001DB0000A000061A800001DB0000A0000753000001DB0000A000088B800001DB005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000140000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0012700001B5800000064000006A400000087000000640000000100000064000000640D000A00000000000010CC000A00001388000010CC000A00001644000011F8000A0000232800001388000A00002454000015E0000A00002AF80000189C000A00002E1800001964000A00003A9800001E78000A00003E8000001E78000A00004E2000001E78000A000061A800001E78000A0000753000001E78000A000088B800001E7805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000140000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B001270000177000000064000006A40000008C000000640000000100000064000000640D000A0000000000001130000A00001388000010CC000A0000164400001324000A0000232800001450000A000024540000170C000A00002AF800001900000A00002E1800001A2C000A00003A9800001FA4000A00003E8000001FA4000A00004E2000001FA4000A000061A800001FA4000A0000753000001FA4000A000088B800001FA405000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000140000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B001270000177000000064000006A400000091000000640000000100000064000000640D000A0000000000001E46000A0000138800001F40000A0000164400001F40000A0000232800001F40000A0000245400001F40000A00002AF800002134000A00002E1800002134000A00003A9800002134000A00003E800000251C000A00004E200000251C000A000061A80000251C000A0000753000002710000A000088B80000290405000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000000F0000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B001270000157C00000064000006A400000096000000640000000100000064000000640D000A0000000000002134000A0000138800002134000A0000164400002134000A0000232800002134000A0000245400002134000A00002AF800002134000A00002E1800002134000A00003A9800002328000A00003E8000002904000A00004E2000002904000A000061A800002904000A0000753000002904000A000088B80000290405000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000000F0000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B001270000157C00000064000006A4000000A0000000640000000100000064000000640D000A0000000000002328000A0000138800002328000A0000164400002328000A0000232800002328000A0000245400002328000A00002AF800002328000A00002E1800002328000A00003A980000251C000A00003E8000002AF8000A00004E2000002AF8000A000061A800002AF8000A0000753000002AF8000A000088B800002AF805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000000F0000006407000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B002930000000004001300000001000000640000177000000C80000013000007D1000000640000177000000244000013000007D20000006400001770000010040000130001869F0000006400014438000013880007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000220000012C000000AA000000F0000000F0000000C8000000BE000000C8FFFFFFE200160000012C0000012C000000C8000000FA000186A0000003E80000044C00002EE000001644000002BC0000009600004A38FFFFC568FFFFF6A0000046500000C35000003E80000005DC0000170C0000106800001770000017700000157C00000A28000003E80000032000000DAC000013880000271006000004B0000004B0000003200000012C000007D00000076C0D000A0000000000000FA0000A0000138800000FA0000A0000164400001194000A0000232800001388000A000024540000157C000A00002AF8000016A8000A00002E18000016A8000A00003A980000189C000A00003E800000189C000A00004E200000189C000A000061A80000189C000A000075300000189C000A000088B80000189C05000A00000000FFFFE0C0000A000007D0FFFFE4A8000A00000FA0FFFFE890000A00001770FFFFEC78000A00004650FFFFF06005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA8405000A0000000000003E80000A000007D000003E80000A00000FA000003E80000A0000177000003A98000A0000271000003A9800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C014101270000EA600000006400000BB80000006400000064000000010000000A000000640D000A0000000000000ED8000A0000138800000ED8000A0000164400000ED8000A0000232800000ED8000A00002454000010CC000A00002AF8000010CC000A00002E18000010CC000A00003A98000010CC000A00003E80000010CC000A00004E20000010CC000A000061A8000010CC000A00007530000010CC000A000088B8000010CC05000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000002D0000003C07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000077240000000400000032000013880000753000000005014101270000D6D80000006400000BB80000006900000064000000010000000A000000640D000A0000000000000ED8000A0000138800000ED8000A0000164400000ED8000A0000232800000ED8000A0000245400000ED8000A00002AF800001194000A00002E1800001194000A00003A9800001194000A00003E8000001194000A00004E2000001194000A0"
			"00061A800001194000A0000753000001194000A000088B80000119405000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000002A0000004107000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000075300000000500000037000011940000714800000003014101270000CB200000006400000AF00000006E000000640000000100000014000000640D000A0000000000000F3C000A0000138800000F3C000A0000164400000F3C000A00002328000012C0000A00002454000012C0000A00002AF8000012C0000A00002E18000012C0000A00003A98000012C0000A00003E80000012C0000A00004E20000012C0000A000061A8000012C0000A00007530000012C0000A000088B8000012C005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000280000004607000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000006D60000000060000003C00000FA000006D6000000003014101270000C3500000006400000A2800000073000000640000000100000014000000640D000A0000000000000FA0000A0000138800000FA0000A0000164400001194000A0000232800001388000A0000245400001388000A00002AF800001388000A00002E1800001388000A00003A9800001388000A00003E8000001388000A00004E2000001388000A000061A800001388000A0000753000001388000A000088B80000138805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000260000005007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000003A98000000060000004100000DAC00004A3800000003014101270000AFC800000064000009600000007800000064000000010000001E0000001E0D000A0000000000001004000A0000138800001004000A00001644000011F8000A0000232800001450000A0000245400001450000A00002AF800001450000A00002E1800001450000A00003A9800001450000A00003E8000001450000A00004E2000001450000A000061A800001450000A0000753000001450000A000088B80000145005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000240000005007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000036B0000000070000004600000C800000520800000003014101270000A41000000064000008980000007D00000064000000010000001E000000640D000A0000000000001068000A0000138800001068000A000016440000125C000A000023280000157C000A000024540000157C000A00002AF80000157C000A00002E180000157C000A00003A980000157C000A00003E800000157C000A00004E200000157C000A000061A80000157C000A000075300000157C000A000088B80000157C05000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000230000005507000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000032C8000000080000004B00000DAC000055F0000000030141012700009C4000000064000007D000000087000000640000000100000028000000640D000A00000000000010CC000A00001388000010CC000A00001644000012C0000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E18000016A8000A00003A98000016A8000A00003E80000016A8000A00004E20000016A8000A000061A8000016A8000A00007530000016A8000A000088B8000016A805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F0000000080000005000000BB8000071480000000301410127000088B800000064000007D000000087000000640000000100000028000000640D000A0000000000001130000A0000138800001130000A0000164400001388000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E180000189C000A00003A980000189C000A00003E800000189C000A00004E200000189C000A000061A80000189C000A000075300000189C000A000088B80000189C05000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F00000000A0000005000000BB80000714800000000014101270000753000000064000007D000000087000000640000000100000032000000640D000A0000000000001130000A0000138800001130000A0000164400001388000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E1800001A90000A00003A9800001A90000A00003E8000001A90000A00004E2000001A90000A000061A800001A90000A0000753000001A90000A000088B800001A9005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F00000000C0000005000000BB8000071480000000001410127000061A800000064000007D000000087000000640000000100000032000000640D000A00000000000010CC000A00001388000010CC000A00001644000012C0000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E1800001900000A00003A9800001C20000A00003E8000001C20000A00004E2000001C20000A000061A800001C20000A0000753000001C20000A000088B800001C2005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F00000000E0000005000000BB800007148000000000047002D000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000047002D0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002930000000004001300000001000000640000177000000C80000013000007D1000000640000177000000244000013000007D20000006400001770000010040000130001869F0000006400014438000013880007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000220000012C000000AA000000F0000000F0000000C8000000BE000000C8FFFFFFE200160000012C0000012C000000C8000000FA000186A0000003E80000044C00002EE000001644000002BC0000009600004A38FFFFC568FFFFF6A0000046500000C35000003E80000005DC0000170C0000106800001770000017700000157C00000A28000003E80000032000000DAC000013880000271006000004B0000004B0000003200000012C000007D00000076C0D000A0000000000000FA0000A0000138800000FA0000A0000164400001194000A0000232800001388000A000024540000157C000A00002AF8000016A8000A00002E18000016A8000A00003A980000189C000A00003E800000189C000A00004E200000189C000A000061A80000189C000A000075300000189C000A000088B80000189C05000A00000000FFFFE0C0000A000007D0FFFFE4A8000A00000FA0FFFFE890000A00001770FFFFEC78000A00004650FFFFF06005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA8405000A0000000000003E80000A000007D000003E80000A00000FA000003E80000A0000177000003A98000A0000271000003A9800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A014101270000EA600000006400000BB800000064000000640000000100000064000000640D000A0000000000000ED8000A0000138800000ED8000A0000164400000ED8000A0000232800000ED8000A00002454000010CC000A00002AF8000010CC000A00002E18000010CC000A00003A98000010CC000A00003E80000010CC000A00004E20000010CC000A000061A8000010CC000A00007530000010CC000A000088B8000010CC05000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000002D0000003C07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000077240000000400000032000013880000753000000005014101270000D6D80000006400000BB800000069000000640000000100000064000000640D000A0000000000000ED8000A0000138800000ED8000A0000164400000ED8000A0000232800000ED8000A0000245400000ED8000A00002AF800001194000A00002E1800001194000A00003A9800001194000A00003E8000001194000A00004E2000001194000A000061A800001194000A0000753000001194000A000088B80000119405000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA840000002A0000004107000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000075300000000500000037000011940000714800000003014101270000CB200000006400000AF00000006E000000640000000100000064000000640D000A0000000000000F3C000A0000138800000F3C000A0000164400000F3C000A00002328000012C0000A00002454000012C0000A00002AF8000012C0000A00002E18000012C0000A00003A98000012C0000A00003E80000012C0000A00004E20000012C0000A000061A8000012C0000A00007530000012C0000A000088B8000012C005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000280000004607000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000006D60000000060000003C00000FA000006D6000000003014101270000C3500000006400000A2800000073000000640000000100000064000000640D000A0000000000000FA0000A0000138800000FA0000A0000164400001194000A0000232800001388000A0000245400001388000A00002AF800001388000A00002E1800001388000A00003A9800001388000A00003E8000001388000A00004E2000001388000A000061A800001388000A0000753000001388000A000088B80000138805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000260000005007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B000003A98000000060000004100000DAC00004A3800000003014101270000AFC80000006400000960000000780000006400000001000000640000001E0D000A0000000000001004000A0000138800001004000A00001644000011F8000A0000232800001450000A0000245400001450000A00002AF800001450000A00002E1800001450000A00003A9800001450000A00003E8000001450000A00004E2000001450000A000061A800001450000A0000753000001450000A000088B80000145005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000240000005007000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000036B0000000070000004600000C800000520800000003014101270000A41000000064000008980000007D000000640000000100000064000000640D000A0000000000001068000A0000138800001068000A000016440000125C000A000023280000157C000A000024540000157C000A00002AF80000157C000A00002E180000157C000A00003A980000157C000A00003E800000157C000A00004E200000157C000A000061A80000157C000A000075300000157C000A000088B80000157C"
			"05000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000230000005507000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000032C8000000080000004B00000DAC000055F0000000030141012700009C4000000064000007D000000087000000640000000100000064000000640D000A00000000000010CC000A00001388000010CC000A00001644000012C0000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E18000016A8000A00003A98000016A8000A00003E80000016A8000A00004E20000016A8000A000061A8000016A8000A00007530000016A8000A000088B8000016A805000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F0000000080000005000000BB8000071480000000301410127000088B800000064000007D000000087000000640000000100000064000000640D000A0000000000001130000A0000138800001130000A0000164400001388000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E180000189C000A00003A980000189C000A00003E800000189C000A00004E200000189C000A000061A80000189C000A000075300000189C000A000088B80000189C05000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F00000000A0000005000000BB80000714800000000014101270000753000000064000007D000000087000000640000000100000064000000640D000A0000000000001130000A0000138800001130000A0000164400001388000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E1800001A90000A00003A9800001A90000A00003E8000001A90000A00004E2000001A90000A000061A800001A90000A0000753000001A90000A000088B800001A9005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F00000000C0000005000000BB8000071480000000001410127000061A800000064000007D000000087000000640000000100000064000000640D000A00000000000010CC000A00001388000010CC000A00001644000012C0000A0000232800001518000A00002454000016A8000A00002AF8000016A8000A00002E1800001900000A00003A9800001C20000A00003E8000001C20000A00004E2000001C20000A000061A800001C20000A0000753000001C20000A000088B800001C2005000A00000000FFFFEC78000A00000BB8FFFFEC78000A00001770FFFFEC78000A00002710FFFFEA84000A00002EE0FFFFEA84000000220000005A07000A00000002000003F2000A0000000A000003F2000A00000014000003FC000A0000003C00000438000A0000007800000488000A000000F0000004B0000A00000384000004B0000055F00000000E0000005000000BB8000071480000000000B1BC27003C000A000C000B0009000001000130C6000D000102000139A4000139A5001100020300013C7B00013C7C00013C7D0009000301000144810011000403000144820001448300014484000900050100015B180009000601000174E000C900083100020488000178B1000178B2000216010001C8170001F176000216050001EE4A00020FC60001E264000222D90001B63A0001B63B0002048700021B8A00021B8B0001FDEC0001C8180001F1770001F28B0001F2880001F28C0001F2890001F28A00013C74000209290001F29A0001F8A3000185100001814B000174D2000185150001E7030001E7040001B13C0001D59300018A0D00015B1900015B1A00015B1D00015B1B00015B1E00015B1C00018EB700019FF70001A0AE0001D592000174D30001B13B00090009010001854E0101000A3F00016C9F00016CA0000174DB000174DD000174DC000174DE000184C100018559000174DF00018A940001897B00016CA1000193A60001934C000198E4000198B40001A10D000198E5000198E6000198D30001A2970001A2920001A7CB0001A31A0001B1030001B11D0001A7CE0001B11C0001B89A0001BCFD0001BD750001BD140001BD060001C32B0001BD290001C3410001C35C00016C9D00016C9E0001D0E40001CC3E0001D6730001D6820001DCE10001E23F0001E2480001E21B0001EC350001EDFF0001EE350001F2C30001F2D30001F2B0000215F100020FDB000215BC000222C0000215E2000215E6000222C6000222C5000222C8000222C900D9000B350001AC940001AC950001AC960001AC540001AC6E0001AC6D0001AC5D0001AC7A0001AC7E0001AC5F0001ACA60001AC5E0001AC7C0001D0D60001AC870001AC810001ACA30001ACA40001ACA20001B8A10001BCFB0001C3800001C36B0001C3A60001CC270001D0EC0001D0DE0001D0EF0001D1060001D6F70001DCB60001E23C0001DCF10001DD010001D6E10001F2A60001F2E60001D6650001D6D30001D6D40001D6E00001DCAE0001DCB70001DCC400020A060001E2250001DD050001E24D0001EC240001EC250001EC2E0001EC450001EC4901000E0000000000000064000000000000019000000E1000000032000000000B000001A9000001B5000001B8000001BA000001BB000001BC000001C30000018300000186000001870000018800004E2001001E0002093B177265732F75697265732F73686F702F6F70656E2E6773610100023C85000000120000753000009C400000000A00009C4010000A000E002C00020A420800040114000402140004031400040414000408140004051400040614000407140100020A3D003000020FFA080004011D0004021D0004031D0004041D0004081D0004051D0004061D0004071D020002100D0002100E00300002169B080004011400040214000403140004041400040814000405140004061400040714020002100600021005002C000216760800040114000402140004031400040414000408140004050000040600000407000100002C7000340002151B0800040114000402140004031400040414000408140004051400040614000407140300010A0000011A560001673900300001D5C308000401140004021400040314000404140004081400040514000406140004071402000031B00001AC010034000221E80800040114000402140004031400040414000408140004051400040614000407140300010096000131DB00014B86002C0002227408000401140004021400040314000404140004081400040514000406140004071401000221E7002C000222760800040114000402140004031400040414000408140004051400040614000407140100021CB8002C000222750800040114000402140004031400040414000408140004051400040614000407140100021CB6002C000233C308000401140004021400040314000404140004081400040514000406140004071401000234A5002C000233C208000401140004021400040314000404140004081400040514000406140004071401000234AA002C00023BB90800040114000402140004031400040414000408140004051400040614000407140100023C84002C00023BBC0800040114000402140004031400040414000408140004051400040614000407140100023C851988");
		len = p - buf.data();
		SendToClient(Client, 25066, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		return;
	}


	try {
		WriteKartPhysParam(p, 0, 0, Client); //NPCKartPhysPara
	}
	catch (...)
	{
		printf("赛车文件异常2!!!\n");
	}
	for (size_t i = 0; i < MNPCKartAIPhysParaCfg->Level; i++)
	{ //NPCKartAIPhysPara
		BYTE* pNPCKartAIPhysPara = p;
		Write16(pNPCKartAIPhysPara, 0); //len

		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OIntervalTime); //NpcN2OIntervalTime
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTriggerN2ORate); //NpcTriggerN2ORate
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRestCheckTime); //NpcRestCheckTime
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocity); //NpcFixVelocity
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocityRate); //NpcFixVelocityRate
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRunDisHulanThreshold); //NpcRunDisHulanThreshold
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcSJetRate); //NpcSJetRate
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTurnAjustDisHulanRate); //NpcTurnAjustDisHulanRate

		char Size = 0;
		Size = MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel.size();
		Write8(pNPCKartAIPhysPara, Size); //ForwardAccelNum
		//printf("Num:%d", ForwardAccelNum);
		for (size_t j = 0; j < Size; j++)
		{ //ForwardAccel
			BYTE* pForwardAccel = pNPCKartAIPhysPara;
			Write16(pForwardAccel, 0); //len

			Write32(pForwardAccel, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel[j].Key); //Key
			Write32(pForwardAccel, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel[j].Value); //Value

			len = pForwardAccel - pNPCKartAIPhysPara;
			Set16(pNPCKartAIPhysPara, (WORD)len);
			pNPCKartAIPhysPara += len;
		}
		Size = MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel.size();
		Write8(pNPCKartAIPhysPara, Size); //BackwardAccelNum
		for (size_t j = 0; j < Size; j++)
		{ //BackwardAccel
			BYTE* pBackwardAccel = pNPCKartAIPhysPara;
			Write16(pBackwardAccel, 0); //len

			Write32(pBackwardAccel, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel[j].Key); //Key
			Write32(pBackwardAccel, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel[j].Value); //Value

			len = pBackwardAccel - pNPCKartAIPhysPara;
			Set16(pNPCKartAIPhysPara, (WORD)len);
			pNPCKartAIPhysPara += len;
		}


		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold); //StraightLenThreshold
		Write32(pNPCKartAIPhysPara, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate); //NpcDriftRate

		Size = MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam.size();
		Write8(pNPCKartAIPhysPara, Size); //CompensateParamCount

		for (size_t j = 0; j < Size; j++)
		{ //CompensateParam
			BYTE* pCompensateParam = pNPCKartAIPhysPara;
			Write16(pCompensateParam, 0); //len

			Write32(pCompensateParam, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam[j].Percent); //Percent
			Write32(pCompensateParam, MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam[j].Value); //Value

			len = pCompensateParam - pNPCKartAIPhysPara;
			Set16(pNPCKartAIPhysPara, (WORD)len);
			pNPCKartAIPhysPara += len;
		}

		len = pNPCKartAIPhysPara - p;
		Set16(p, (WORD)len);
		p += len;
	}



	try
	{
		WriteKartPhysParam(p, 0, 0, Client); //NPCKartPhysPara_NpcPlayer
	}
	catch (...)
	{
		printf("赛车文件异常3!!!\n");
	}


	Write8(p, MNPCPlayerKartAIPhysParaCfg->Level); //LevelNum
	for (size_t i = 0; i < MNPCPlayerKartAIPhysParaCfg->Level; i++)
	{ //NPCKartAIPhysPara
		BYTE* pNPCKartAIPhysPara_NpcPlayer = p;
		Write16(pNPCKartAIPhysPara_NpcPlayer, 0); //len
		{ //Param

			BYTE* pParam = pNPCKartAIPhysPara_NpcPlayer;
			Write16(pParam, 0); //len

			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OIntervalTime); //NpcN2OIntervalTime
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTriggerN2ORate); //NpcTriggerN2ORate
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRestCheckTime); //NpcRestCheckTime
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocity); //NpcFixVelocity
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocityRate); //NpcFixVelocityRate
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRunDisHulanThreshold); //NpcRunDisHulanThreshold
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcSJetRate); //NpcSJetRate
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTurnAjustDisHulanRate); //NpcTurnAjustDisHulanRate


			size_t ForwardAccelNum = MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel.size();
			Write8(pParam, ForwardAccelNum); //ForwardAccelNum

			for (size_t j = 0; j < ForwardAccelNum; j++)
			{ //ForwardAccel
				BYTE* pForwardAccel = pParam;
				Write16(pForwardAccel, 0); //len

				Write32(pForwardAccel, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel[j].Key); //Key
				Write32(pForwardAccel, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel[j].Value); //Value

				len = pForwardAccel - pParam;
				Set16(pParam, (WORD)len);
				pParam += len;
			}


			size_t BackwardAccelNum = MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel.size();
			Write8(pParam, BackwardAccelNum); //BackwardAccelNum
			for (size_t j = 0; j < BackwardAccelNum; j++)
			{ //BackwardAccel
				BYTE* pBackwardAccel = pParam;
				Write16(pBackwardAccel, 0); //len

				Write32(pBackwardAccel, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel[j].Key); //Key
				Write32(pBackwardAccel, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel[j].Value); //Value

				len = pBackwardAccel - pParam;
				Set16(pParam, (WORD)len);
				pParam += len;
			}


			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold); //StraightLenThreshold
			Write32(pParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate); //NpcDriftRate


			size_t CompensateParamNum = MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam.size();
			Write8(pParam, CompensateParamNum); //CompensateParamCount

			for (size_t j = 0; j < CompensateParamNum; j++)
			{ //CompensateParam
				BYTE* pCompensateParam = pParam;
				Write16(pCompensateParam, 0); //len

				Write32(pCompensateParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam[j].Percent); //Percent
				Write32(pCompensateParam, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam[j].Value); //Value

				len = pCompensateParam - pParam;
				Set16(pParam, (WORD)len);
				pParam += len;
			}
			len = pParam - pNPCKartAIPhysPara_NpcPlayer;
			Set16(pNPCKartAIPhysPara_NpcPlayer, (WORD)len);
			pNPCKartAIPhysPara_NpcPlayer += len;
		}
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].N2OStartTime); //N2OStartTime
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OMaxCount); //NpcN2OMaxCount
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].Dis2FirstPlay); //Dis2FirstPlay
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].LostControlTime); //LostControlTime
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].LostInterval); //LostInterval
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].LostControlCount); //LostControlCount

		len = pNPCKartAIPhysPara_NpcPlayer - p;
		Set16(p, (WORD)len);
		p += len;
	}


	try
	{
		WriteKartPhysParam(p, 0, 0, Client); //NPCKartPhysPara_RankedMatch
	}
	catch (...)
	{
		printf("赛车文件异常4!!!\n");
	}
	Write8(p, MNPCRankedMatchKartAIPhysParaCfg->Level); //NPCLevelNum_RankedMatch
	for (size_t i = 0; i < MNPCRankedMatchKartAIPhysParaCfg->Level; i++)
	{ //NPCKartAIPhysPara
		BYTE* pNPCKartAIPhysPara_NpcPlayer = p;
		Write16(pNPCKartAIPhysPara_NpcPlayer, 0); //len
		{ //Param

			BYTE* pParam = pNPCKartAIPhysPara_NpcPlayer;
			Write16(pParam, 0); //len

			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OIntervalTime); //NpcN2OIntervalTime
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTriggerN2ORate); //NpcTriggerN2ORate
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRestCheckTime); //NpcRestCheckTime
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocity); //NpcFixVelocity
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocityRate); //NpcFixVelocityRate
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRunDisHulanThreshold); //NpcRunDisHulanThreshold
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcSJetRate); //NpcSJetRate
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTurnAjustDisHulanRate); //NpcTurnAjustDisHulanRate


			size_t ForwardAccelNum = MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel.size();
			Write8(pParam, ForwardAccelNum); //ForwardAccelNum

			for (size_t j = 0; j < ForwardAccelNum; j++)
			{ //ForwardAccel
				BYTE* pForwardAccel = pParam;
				Write16(pForwardAccel, 0); //len

				Write32(pForwardAccel, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel[j].Key); //Key
				Write32(pForwardAccel, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel[j].Value); //Value

				len = pForwardAccel - pParam;
				Set16(pParam, (WORD)len);
				pParam += len;
			}


			size_t BackwardAccelNum = MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel.size();
			Write8(pParam, BackwardAccelNum); //BackwardAccelNum
			for (size_t j = 0; j < BackwardAccelNum; j++)
			{ //BackwardAccel
				BYTE* pBackwardAccel = pParam;
				Write16(pBackwardAccel, 0); //len

				Write32(pBackwardAccel, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel[j].Key); //Key
				Write32(pBackwardAccel, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel[j].Value); //Value

				len = pBackwardAccel - pParam;
				Set16(pParam, (WORD)len);
				pParam += len;
			}


			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold); //StraightLenThreshold
			Write32(pParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate); //NpcDriftRate


			size_t CompensateParamNum = MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam.size();
			Write8(pParam, CompensateParamNum); //CompensateParamCount

			for (size_t j = 0; j < CompensateParamNum; j++)
			{ //CompensateParam
				BYTE* pCompensateParam = pParam;
				Write16(pCompensateParam, 0); //len

				Write32(pCompensateParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam[j].Percent); //Percent
				Write32(pCompensateParam, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam[j].Value); //Value

				len = pCompensateParam - pParam;
				Set16(pParam, (WORD)len);
				pParam += len;
			}
			len = pParam - pNPCKartAIPhysPara_NpcPlayer;
			Set16(pNPCKartAIPhysPara_NpcPlayer, (WORD)len);
			pNPCKartAIPhysPara_NpcPlayer += len;
		}
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].N2OStartTime); //N2OStartTime
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OMaxCount); //NpcN2OMaxCount
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].Dis2FirstPlay); //Dis2FirstPlay
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].LostControlTime); //LostControlTime
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].LostInterval); //LostInterval
		Write32(pNPCKartAIPhysPara_NpcPlayer, MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].LostControlCount); //LostControlCount

		len = pNPCKartAIPhysPara_NpcPlayer - p;
		Set16(p, (WORD)len);
		p += len;
	}



	if (is60fps == 1 || Client->fps == 2)
	{
		Write32(p, 0x0073bc8b); //SwitchFlag1
	}
	else
	{
		int Flag = SwitchFlag1 > 0 ? SwitchFlag1 : Client->SwitchFlag1;
		Write32(p, Flag); //SwitchFlag1
	}

	//Write32(p, 7584807); //SwitchFlag1  //0x0003BC27  赛车标签
	//Write32(p, 0x0003BC27); //SwitchFlag1  //0x0003BC27 
	Write16(p, 60); //MaxQuickLerpThrehold
	Write16(p, 10); //QuickLerpStepCnts
	Write16(p, 12); //LerpSynccpFrequence

	Write16(p, 0); //ClientItemOpenTypeNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //ClientItemOpenCfg
		BYTE* pClientItemOpenCfg = p;
		Write16(pClientItemOpenCfg, 0); //len

		Write16(pClientItemOpenCfg, 0); //OpenType
		Write8(pClientItemOpenCfg, 0); //ItemNum
		Write32(pClientItemOpenCfg, 0); //ItemList[]

		len = pClientItemOpenCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	Write8(p, 1); //OtherNum
	for (size_t i = 0; i < 1; i++)
	{ //CollisionPowerOtherInfo
		BYTE* pCollisionPowerOtherInfo = p;
		Write16(pCollisionPowerOtherInfo, 0); //len

		Write32(pCollisionPowerOtherInfo, 0); //Power
		Write32(pCollisionPowerOtherInfo, 100); //Ratio
		Write32(pCollisionPowerOtherInfo, 0); //Add

		len = pCollisionPowerOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 400); //CollisionMinPower
	Write32(p, 3400); //ItemExtendLimit
	Write32(p, 50); //ChattingTalkLevelLimit
	Write32(p, 0); //ChattingSpecialOp


	Write8(p, ForbidMaps.size()); //ForbidMapNum
	for (size_t i = 0; i < ForbidMaps.size(); i++) {
		Write32(p, ForbidMaps[i]); //ForbidMap[] 时光静域
	}
	//Write32(p, 0); //ForbidMap[]

	Write32(p, 20000); //MaxBuySpeedNum

	Write8(p, 0); //ClientItemIconNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //ClientIconCfg
		BYTE* pClientIconCfg = p;
		Write16(pClientIconCfg, 0); //len

		Write32(pClientIconCfg, 0); //ItemID
		Write8(pClientIconCfg, 0); //IconPathLen

		len = pClientIconCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	Write8(p, 0); //TDCBlackCarNum
	//Write32(p, 0); //TDCBlackCar[]

	Write8(p, 0); //TDCBlackCarTypeNum
	//Write32(p, 0); //TDCBlackCarType[]

	Write8(p, 0); //TDCBlackCarType2ndNum
	//Write32(p, 0); //TDCBlackCarType2nd[]

	{ //ClientFrameRateCfg客户端帧率配置
		BYTE* pClientFrameRateCfg = p;
		Write16(pClientFrameRateCfg, 0); //len
		if (UnlockFPS != 0 || Client->fps == 1)
		{
			Write32(pClientFrameRateCfg, 0);	 // MinFrameRate
			Write32(pClientFrameRateCfg, 60000); // MaxFrameRate
			Write32(pClientFrameRateCfg, 10);	 // FrameStep
			Write32(pClientFrameRateCfg, 0);	 // CurrentFrameRate
		}
		else
		{
			Write32(pClientFrameRateCfg, 30000); //MinFrameRate
			Write32(pClientFrameRateCfg, 40000); //MaxFrameRate
			Write32(pClientFrameRateCfg, 10); //FrameStep
			Write32(pClientFrameRateCfg, 40000); //CurrentFrameRate
		}


		len = pClientFrameRateCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 16); //ReportFrameRateValue
	Write16(p, 10); //FrameOpSpan
	Write8(p, 0); //VideoSwitch

	/*memcpy(p, "\x0E\x00\x2C\x00\x02\x0A\x42\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x0A\x3D\x00\x30\x00\x02\x0F\xFA\x08\x00\x04\x01\x1D\x00\x04\x02\x1D\x00\x04\x03\x1D\x00\x04\x04\x1D\x00\x04\x08\x1D\x00\x04\x05\x1D\x00\x04\x06\x1D\x00\x04\x07\x1D\x02\x00\x02\x10\x0D\x00\x02\x10\x0E\x00\x30\x00\x02\x16\x9B\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x02\x00\x02\x10\x06\x00\x02\x10\x05\x00\x2C\x00\x02\x16\x76\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x00\x00\x04\x06\x00\x00\x04\x07\x00\x01\x00\x00\x2C\x70\x00\x34\x00\x02\x15\x1B\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x03\x00\x01\x0A\x00\x00\x01\x1A\x56\x00\x01\x67\x39\x00\x30\x00\x01\xD5\xC3\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x02\x00\x00\x31\xB0\x00\x01\xAC\x01\x00\x34\x00\x02\x21\xE8\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x03\x00\x01\x00\x96\x00\x01\x31\xDB\x00\x01\x4B\x86\x00\x2C\x00\x02\x22\x74\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x21\xE7\x00\x2C\x00\x02\x22\x76\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x1C\xB8\x00\x2C\x00\x02\x22\x75\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x1C\xB6\x00\x2C\x00\x02\x33\xC3\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x34\xA5\x00\x2C\x00\x02\x33\xC2\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x34\xAA\x00\x2C\x00\x02\x3B\xB9\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x3C\x84\x00\x2C\x00\x02\x3B\xBC\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x3C\x85\x19\x88", 647);
	p += 647;
	len = p - buf.data();
	SendToClient(Client, 25066, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;*/

	memcpy(p, "\x0C\x00\x2C\x00\x02\x0A\x42\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x0A\x3D\x00\x30\x00\x02\x0F\xFA\x08\x00\x04\x01\x1D\x00\x04\x02\x1D\x00\x04\x03\x1D\x00\x04\x04\x1D\x00\x04\x08\x1D\x00\x04\x05\x1D\x00\x04\x06\x1D\x00\x04\x07\x1D\x02\x00\x02\x10\x0D\x00\x02\x10\x0E\x00\x30\x00\x02\x16\x9B\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x02\x00\x02\x10\x06\x00\x02\x10\x05\x00\x2C\x00\x02\x16\x76\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x00\x00\x04\x06\x00\x00\x04\x07\x00\x01\x00\x00\x2C\x70\x00\x34\x00\x02\x15\x1B\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x03\x00\x01\x17\x20\x00\x01\x0E\x59\x00\x01\x4B\x87\x00\x30\x00\x01\xD5\xC3\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x02\x00\x00\x31\xB0\x00\x01\xAC\x01\x00\x34\x00\x02\x21\xE8\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x03\x00\x01\x00\x96\x00\x01\x31\xDB\x00\x01\x4B\x86\x00\x2C\x00\x02\x22\x74\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x21\xE7\x00\x2C\x00\x02\x22\x76\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x1C\xB8\x00\x2C\x00\x02\x22\x75\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x1C\xB6\x00\x2C\x00\x02\x33\xC3\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x34\xA5\x00\x2C\x00\x02\x33\xC2\x08\x00\x04\x01\x14\x00\x04\x02\x14\x00\x04\x03\x14\x00\x04\x04\x14\x00\x04\x08\x14\x00\x04\x05\x14\x00\x04\x06\x14\x00\x04\x07\x14\x01\x00\x02\x34\xAA\x19\x88", 559);
	p += 559;
	len = p - buf.data();
	SendToClient(Client, 25066, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write8(p, 0); //KartRefitCardTypeNum
	for (size_t i = 0; i < 0; i++)
	{ //KartRefitCardCfg
		BYTE* pKartRefitCardCfg = p;
		Write16(pKartRefitCardCfg, 0); //len

		Write32(pKartRefitCardCfg, 0); //RefitCardID
		Write8(pKartRefitCardCfg, 0); //EffectNum
		for (size_t i = 0; i < 0; i++)
		{ //RefitCardEffect
			BYTE* pRefitCardEffect = pKartRefitCardCfg;
			Write16(pRefitCardEffect, 0); //len

			Write8(pRefitCardEffect, 0); //RefitID
			Write8(pRefitCardEffect, 0); //ModifyValue

			len = pRefitCardEffect - pKartRefitCardCfg;
			Set16(pKartRefitCardCfg, (WORD)len);
			pKartRefitCardCfg += len;
		}
		Write8(pKartRefitCardCfg, 0); //KartNum
		Write32(pKartRefitCardCfg, 0); //KartList[]

		len = pKartRefitCardCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 25066, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	//delete[] buf;
}

void NotifySpeed2Cfg(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //MapEffectMaskNum
	/*
	for (size_t i = 0; i < 0; i++)
	{ //MapEffectMask
		BYTE* pMapEffectMask = p;
		Write16(pMapEffectMask, 0); //len

		Write16(pMapEffectMask, 0); //MapID
		Write32(pMapEffectMask, 0); //EffectMask

		len = pMapEffectMask - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 0); //Speed2ShopLimit
	Write32(p, 127); //OpenFlag

	len = p - buf.data();
	SendToClient(Client, 11358, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Notify);
}

void NotifyTopUIItemInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin

	UINT DisplayUIItemID[] = { 43,183 };
	UINT ShowUINum = sizeof(DisplayUIItemID) / sizeof(UINT);

	Write16(p, 2); //Num
	for (size_t i = 0; i < ShowUINum; i++)
	{ //TopUIItemInfo
		BYTE* pTopUIItemInfo = p;
		Write16(pTopUIItemInfo, 0); //len

		Write8(pTopUIItemInfo, 0); //Type
		Write16(pTopUIItemInfo, DisplayUIItemID[i]); //ID
		Write16(pTopUIItemInfo, 0); //Tag
		Write16(pTopUIItemInfo, 0); //NameLen
		Write16(pTopUIItemInfo, 0); //PrompLen

		len = pTopUIItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //RecommendModeInfo
		BYTE* pRecommendModeInfo = p;
		Write16(pRecommendModeInfo, 0); //len

		Write16(pRecommendModeInfo, 0); //ModeNum
		/*
		for (size_t i = 0; i < n; i++)
		{ //ModeInfo
			BYTE* pModeInfo = pRecommendModeInfo;
			Write16(pModeInfo, 0); //len

			Write16(pModeInfo, 0); //ModeID
			Write16(pModeInfo, 0); //Status
			Write32(pModeInfo, 0); //Privige
			Write16(pModeInfo, 0); //PrompLen

			len = pModeInfo - pRecommendModeInfo;
			Set16(pRecommendModeInfo, (WORD)len);
			pRecommendModeInfo += len;
		}
		*/
		len = pRecommendModeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //Count
	/*
	for (size_t i = 0; i < n; i++)
	{ //EntryList
		BYTE* pEntryList = p;
		Write16(pEntryList, 0); //len

		Write8(pEntryList, 0); //Type
		Write8(pEntryList, 0); //Status
		Write16(pEntryList, 0); //PromptLen
		Write8(pEntryList, 0); //Tag
		Write8(pEntryList, 0); //Icon
		Write8(pEntryList, 0); //Kind
		Write8(pEntryList, 0); //Privilige
		Write8(pEntryList, 0); //IsTodayOpen
		Write8(pEntryList, 0); //SetType
		Write8(pEntryList, 0); //EmbedType
		Write8(pEntryList, 0); //LoginWindow

		len = pEntryList - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 0); //SetNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //EntrySet
		BYTE* pEntrySet = p;
		Write16(pEntrySet, 0); //len

		Write8(pEntrySet, 0); //Count
		for (size_t i = 0; i < n; i++)
		{ //EntryList
			BYTE* pEntryList = pEntrySet;
			Write16(pEntryList, 0); //len

			Write8(pEntryList, 0); //Type
			Write8(pEntryList, 0); //Status
			Write16(pEntryList, 0); //PromptLen
			Write8(pEntryList, 0); //Tag
			Write8(pEntryList, 0); //Icon
			Write8(pEntryList, 0); //Kind
			Write8(pEntryList, 0); //Privilige
			Write8(pEntryList, 0); //IsTodayOpen
			Write8(pEntryList, 0); //SetType
			Write8(pEntryList, 0); //EmbedType
			Write8(pEntryList, 0); //LoginWindow

			len = pEntryList - pEntrySet;
			Set16(pEntrySet, (WORD)len);
			pEntrySet += len;
		}

		len = pEntrySet - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 0); //MouseOverNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //MouseOverTipsCfg
		BYTE* pMouseOverTipsCfg = p;
		Write16(pMouseOverTipsCfg, 0); //len

		Write16(pMouseOverTipsCfg, 0); //ID
		Write16(pMouseOverTipsCfg, 0); //PrompLen

		len = pMouseOverTipsCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	len = p - buf.data();
	SendToClient(Client, 25116, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyTopUIItemInfoNew(std::shared_ptr<ClientNode> Client)
{
	/*if (Uinew) {
		std::string Byte = "00000000000200290000780001000AB3E5B4CCB4F3B4B3B9D80014B7C9D4BEBDAED3F2A3ACB3E5B4CCB4B3B9D8A3A1001B01002B00000006C5C5CEBBC8FC000AC8D9D2ABC5C5CEBBC8FC006D000500150013000200000008000932333A3539BDE1CAF800150000000200000006000932333A3539BDE1CAF80015002E000200000004000932333A3539BDE1CAF800150004000000000002000931303A3030BFAACABC0015001D000000000001000932333A3539BFAACABC18001B006502000AC6E6C3EED5E4C6B7CEDD000101010000020000000019014C020008B7E2C9F1D1DDD2E50201010100000000000000190135020008C6DFB2CACFC9EBF800010101000000000000001900C7020008C8FDB9FAC3D8B1A601010101000000000000007300A3020035CAD7BFEEB6AFCEEFC4E2CCAC41B3B5A1BEC9B3C4AED6AED6DBA1BFA3ACCDEAB3C9D6FEC3CE31303025C3E2B7D1B5C3D3C0BEC3A3A10000010100000001002D5265732F75697265732F414361724275696C64447265616D2F696D6167652F456E74727949636F6E342E677361001F016F02000EB4F3B6FAB9B7B5C4BFA7B7C8CEDD02000101000000010000001900BE020008C6DFC8D5B8A3C0FB010001010000000000000019008C020008C8FCB3B5B6E1B1A600000101000000000000002B00ED02001AC8ABC3F1CDB6C6B120D6DCC4EAC7ECC1ECD3C0BEC341B3B5A3A101000101000000010000002800EB020017C6E6BBC3CCC7B9FBCEDD20C0DBB5C7B5C3D3C0BEC3A3A10100010100000001000000230172020012C5DCCDBCB5C3D3C0BEC3C1AAB6AFC5E4CACE00000101000000000000003B017102002AD1DDD7E0D0C2BDAECCD8C9ABC0D6C6F7A3ACB5C3D3C0BEC3A1BEC3DBB9CFCFC9C1E9A1BFB3E8CEEFA3A10000010100000000000000230173020012B7C9B3DBD0C2BDAEA3ACCBBFC2B7C7A7C4EA0000010100000000000000210138020010CAD7B7A2CFA1CAC0C3C0D2C2C9CFD0C20000010100000000000000230107020012BBFABCD7C6A4B7F4BBC0D0C2B3F6B7A2A3A10000010100000000000000190079020008C8D9D2ABD5BDD1AB00000101000000000000001F008002000EC9F1B3B5CDBCBCF8CAD5BCAFC1EE00000101000000000000001B002E02000AD0D2D4CBB4F3BDB1C8FC00000101000000000000001B000002000ABBB6C0D6C7C0BDF0B1D20000010100000000000000190082020008C3D8B1A6C2DEC5CC0000010100000000000000250078020014B3E5B4CCB4F3B4B3B9D8C8ABD0C2C9CFCFDFA3A100000262000000000000002A015E020019327632C8FCB3B5D7E3C7F2CDE6B7A8D5FDCABDC9CFCFDFA3A100000262000000000000002E010102001DC8ABC3F7D0C7B4F3C2D2B6B778C4A7BFA8C9D9C5AED3A3C1AAB6AFA3A100000262000000000000001D014B02000CCDBFC4A8B4F3C2D2B6B7A3A10000026200000000000001024514001F016C02000ED6D5BCABC4A7B7A8D5F3D7D4D1A1000101010000010000000019014C020008B7E2C9F1D1DDD2E50201010100000100000000280156020017D6C1C3C0B7BCBBAA2DCBC4B4F3C3C0C8CBC0B4C1D9A3A10101010100000100000000190135020008C6DFB2CACFC9CCD9010101010000010000000019013E020008D7EEC3C0D0C2BDAE0101010100000100000000210107020010BBFABCD7C6A4B7F4BBC0D0C2B3F6B7A201010101000001000000001D008C02000CC8FCB3B5D7D4D1A1B6E1B1A60001010100000100000000210138020010CFA1CAC0C9CCB5EABBC0C8BBB8FCD0C2010101010000010000000044012F020033B0CDC0B2C0B2D0A1C4A7CFC9B5BDB7C3B7C9B3B5B4F3C2BDA3ACD0C2BFEEC1AAB6AFCFDEB6A853B3B5B2BBC8DDB4EDB9FDA3A10101010100000100000000190136020008CAB1BFD5D6AEBADB01010101000001000000001700FF020006C4DEC9D1B8F301010101000001000000001900C7020008C8FDB9FAC3D8B1A60101010100000100000000190079020008C8D9D2ABD5BDD1AB0001010100000100000000190082020008C3D8B1A6C2DEC5CC00010101000001000000001900BC020008C1F9C3A2D0C7D5F300010101000001000000001900BE020008C6DFC8D5B8A3C0FB000101010000010000000019007C020008BBFABCD7BCA4BBEE0000010100000100000000170066020006D5BCB2B7CEDD0000010100000100000000190041020008C3BFC8D5D2BBB3E90000010100000100000000170044020006C6E6BCA3B8F30000010100000100000002000E00AB0008BEF8C3FCB3B5CDF5000E00A20008D0C7BBF0B3A4D2B9";
		const unsigned char* Msg25116 = EHexToByte(Byte.data());
		SendToClient(Client, 25116, (const BYTE*)Msg25116, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		delete[] Msg25116;
		return;
	}*/


	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	//CONST UINT TopUIID = ServerConfig["TopUIID"].as<UINT>();
	typedef struct CTOPUIITEMINFO //
	{
		UCHAR Type;
		USHORT ID;
		USHORT Tag;
		USHORT NameLen;
		CHAR Name[66];
		USHORT PrompLen;
		CHAR Promp[66];
	}*LPCTOPUIITEMINFO;
	typedef struct CRECOMMENDMODEINFO //
	{
		USHORT ModeID;
		USHORT Status;
		UINT Privige;
		USHORT PrompLen;
		CHAR Promp[66];
	}*LPCRECOMMENDMODEINFO;

	typedef struct CENTRYLISTINFO //
	{
		USHORT ID;
		UCHAR Status;
		USHORT PromptLen;
		CHAR Promp[66];
		UCHAR Tag;
		UCHAR Icon;
		UCHAR Kind;
		UCHAR Privilige;
		UCHAR IsTodayOpen;
		UCHAR SetType;
		UCHAR EmbedType;
		UCHAR LoginWindow;
	}*LPCENTRYLISTINFO;


	typedef struct CMOUSEOVERTIPSINFO //
	{
		USHORT ID;
		USHORT PrompLen;
		CHAR Promp[66];
	}*LPCMOUSEOVERTIPSINFO;



	const static CTOPUIITEMINFO TopUIItemInfoCfg[] =
	{
		//19	变身派对
		//43	荣耀排位赛
		//62	赛道随心造
		//63	世界挑战赛
		//67	双人冲顶赛
		//81	狂野追逐
		//90	暗夜大逃亡
		//96	超能竞速赛
		//103	疾风前传
		//109	双人冲锋战
		//120	龙晶大闯关
		//127	极速对决
		//134	车王新剧情
		//143	互助接力赛
		//153	年兽大乱斗
		//155	雷诺嘉年华
		//162	QS极速幻境
		//171	绝命车王
		//183	转世续缘
		//190	跳跳派对
		//197	时空捕手
		//208	顺子大作战
		//227	冰雪飞驰
		//252	问天之约
		//257	全明星大乱斗
		//285	极速同行
		//{0,303,1,8,"熔炉盛典",18,"命运豪赌，熔炉角斗"},
		//{0,43,1,6,"排位赛",10,"荣耀排位赛"},
		//{0,303,1,10,"幸运大奖赛",10,"幸运大奖赛"},
		{0,120,1,10,"冲刺大闯关",10,"冲刺大闯关"},
		//{0,257,1,10,"全明星大乱斗",10,"全明星大乱斗"},
		//{0,350,1,8,"绿茵车王",8,"绿茵车王"},
		//46 156
		{1,43,0,6,"排位赛",10,"荣耀排位赛"},
	};
	const static CRECOMMENDMODEINFO RecommendModeInfoCfg[] =
	{
		{19,2,8,9,"23:59结束"},
		{29,0,1,9,"09:00开始"},
	};

	//const static CENTRYLISTINFO EntryListCfg[] =
	//{
	//	{101,2,10,"奇妙珍品屋",0,1,1,1,0,0,2,0},
	//	{46,2,8,"幸运大奖赛",0,0,2,1,0,0,0,0},
	//};
	const static CENTRYLISTINFO EntryListCfg[] =
	{
		{101,2,10,"奇妙珍品屋",0,1,1,1,0,0,2,0},
		{120,2,10,"冲刺大闯关",1,1,1,1,0,0,0,0},
		//{366,2,27,"首发皮肤至尊爆天甲-神·奥丁",0,1,1,1,1,0,0,0},
		//{260,2,17,"终极猛兽-幽冥骑士",1,1,1,1,0,0,0,0},

		//{39,2,10,"终极魔法阵",0,1,1,1,0,0,0,0},
		//{78,2,11,"超值周卡",0,1,1,1,0,0,2,0},
		{283,2,11,"赛车夺宝",2,1,1,1,0,0,2,0},
		{263,2,11,"机甲皮肤",0,1,1,1,0,0,2,0},
		{350,2,11,"绿茵车王",2,1,1,1,0,0,2,0},
		{ 46, 2, 8, "幸运大奖赛", 0, 0, 2, 1, 0, 0, 0, 0 },
		{ 367, 2, 14, "大耳狗的咖啡屋", 2, 0, 2, 1, 0, 0, 0, 0 },
		{ 27, 2, 9, "魔力圣杯", 0, 0, 2, 1, 0, 0, 0, 0 },
		{ 39, 2, 9, "终极魔法阵", 0, 0, 2, 1, 0, 0, 0, 0 },
		{ 65, 2, 10, "每日一抽", 0, 0, 2, 1, 0, 0, 0, 0 },
		{50,2,10,"小橘子神灯",0,0,1,1,0,0,0,0},
		{ 68, 2, 11, "奇迹阁", 0, 0, 2, 1, 0, 0, 0, 0 },

		

	};


	const static CENTRYLISTINFO EntrySetCfg[] =
	{
		//奇妙珍品屋显示列表
		{27,2,8,"魔力圣杯",0,0,1,8,0,0,1,0},

		//{283,2,8,"赛车夺宝",0,1,1,12,0,0,1,0},
	};

	const static CMOUSEOVERTIPSINFO MouseOverTipsCfg[] =
	{
		{171,8,"绝命车王"},
		{162,8,"星火长夜"},
	};




	Write32(p, Client->Uin); //Uin
	Write16(p, sizeof(TopUIItemInfoCfg) / sizeof(TopUIItemInfoCfg[0])); //Num
	for (USHORT i = 0; i < sizeof(TopUIItemInfoCfg) / sizeof(TopUIItemInfoCfg[0]); i++)
	{ //TopUIItemInfo
		BYTE* pTopUIItemInfo = p;
		Write16(pTopUIItemInfo, 0); //len

		Write8(pTopUIItemInfo, TopUIItemInfoCfg[i].Type); //Type
		Write16(pTopUIItemInfo, TopUIItemInfoCfg[i].ID); //ID
		Write16(pTopUIItemInfo, TopUIItemInfoCfg[i].Tag); //Tag
		Write16(pTopUIItemInfo, TopUIItemInfoCfg[i].NameLen); //NameLen
		memcpy(pTopUIItemInfo, TopUIItemInfoCfg[i].Name, TopUIItemInfoCfg[i].NameLen);
		pTopUIItemInfo += TopUIItemInfoCfg[i].NameLen;

		Write16(pTopUIItemInfo, TopUIItemInfoCfg[i].PrompLen); //PrompLen
		memcpy(pTopUIItemInfo, TopUIItemInfoCfg[i].Promp, TopUIItemInfoCfg[i].PrompLen);
		pTopUIItemInfo += TopUIItemInfoCfg[i].PrompLen;

		len = pTopUIItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //RecommendModeInfo
		BYTE* pRecommendModeInfo = p;
		Write16(pRecommendModeInfo, 0); //len

		Write16(pRecommendModeInfo, sizeof(RecommendModeInfoCfg) / sizeof(RecommendModeInfoCfg[0])); //ModeNum	
		for (size_t i = 0; i < sizeof(RecommendModeInfoCfg) / sizeof(RecommendModeInfoCfg[0]); i++)
		{ //ModeInfo
			BYTE* pModeInfo = pRecommendModeInfo;
			Write16(pModeInfo, 0); //len

			Write16(pModeInfo, RecommendModeInfoCfg[i].ModeID); //ModeID
			Write16(pModeInfo, RecommendModeInfoCfg[i].Status); //Status
			Write32(pModeInfo, RecommendModeInfoCfg[i].Privige); //Privige
			Write16(pModeInfo, RecommendModeInfoCfg[i].PrompLen); //PrompLen

			memcpy(pModeInfo, RecommendModeInfoCfg[i].Promp, RecommendModeInfoCfg[i].PrompLen);
			pModeInfo += RecommendModeInfoCfg[i].PrompLen;


			len = pModeInfo - pRecommendModeInfo;
			Set16(pRecommendModeInfo, (WORD)len);
			pRecommendModeInfo += len;
		}

		len = pRecommendModeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, sizeof(EntryListCfg) / sizeof(EntryListCfg[0])); //Count	
	for (UCHAR i = 0; i < sizeof(EntryListCfg) / sizeof(EntryListCfg[0]); i++)
	{ //EntryList
		BYTE* pEntryList = p;
		Write16(pEntryList, 0); //len

		Write16(pEntryList, EntryListCfg[i].ID); //Type
		Write8(pEntryList, EntryListCfg[i].Status); //Status
		Write16(pEntryList, EntryListCfg[i].PromptLen); //PromptLen

		memcpy(pEntryList, EntryListCfg[i].Promp, EntryListCfg[i].PromptLen);
		pEntryList += EntryListCfg[i].PromptLen;

		Write8(pEntryList, EntryListCfg[i].Tag); //Tag
		Write8(pEntryList, EntryListCfg[i].Icon); //Icon
		Write8(pEntryList, EntryListCfg[i].Kind); //Kind
		Write8(pEntryList, EntryListCfg[i].Privilige); //Privilige
		Write8(pEntryList, EntryListCfg[i].IsTodayOpen); //IsTodayOpen
		Write8(pEntryList, EntryListCfg[i].SetType); //SetType
		Write8(pEntryList, EntryListCfg[i].EmbedType); //EmbedType
		Write8(pEntryList, EntryListCfg[i].LoginWindow); //LoginWindow

		len = pEntryList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 1); //SetNum	
	for (size_t i = 0; i < 1; i++)
	{ //EntrySet
		BYTE* pEntrySet = p;
		Write16(pEntrySet, 0); //len

		Write8(pEntrySet, sizeof(EntrySetCfg) / sizeof(EntrySetCfg[0])); //Count
		for (size_t j = 0; j < sizeof(EntrySetCfg) / sizeof(EntrySetCfg[0]); j++)
		{ //EntryList
			BYTE* pEntryList = pEntrySet;
			Write16(pEntryList, 0); //len

			Write16(pEntryList, EntrySetCfg[j].ID); //Type
			Write8(pEntryList, EntrySetCfg[j].Status); //Status
			Write16(pEntryList, EntrySetCfg[i].PromptLen); //PromptLen

			memcpy(pEntryList, EntrySetCfg[i].Promp, EntrySetCfg[i].PromptLen);
			pEntryList += EntrySetCfg[i].PromptLen;

			Write8(pEntryList, EntrySetCfg[j].Tag); //Tag
			Write8(pEntryList, EntrySetCfg[j].Icon); //Icon
			Write8(pEntryList, EntrySetCfg[j].Kind); //Kind
			Write8(pEntryList, EntrySetCfg[j].Privilige); //Privilige
			Write8(pEntryList, EntrySetCfg[j].IsTodayOpen); //IsTodayOpen
			Write8(pEntryList, EntrySetCfg[j].SetType); //SetType
			Write8(pEntryList, EntrySetCfg[j].EmbedType); //EmbedType
			Write8(pEntryList, EntrySetCfg[j].LoginWindow); //LoginWindow

			len = pEntryList - pEntrySet;
			Set16(pEntrySet, (WORD)len);
			pEntrySet += len;
		}

		len = pEntrySet - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, sizeof(MouseOverTipsCfg) / sizeof(MouseOverTipsCfg[0])); //MouseOverNum	
	for (USHORT i = 0; i < sizeof(MouseOverTipsCfg) / sizeof(MouseOverTipsCfg[0]); i++)
	{ //MouseOverTipsCfg
		BYTE* pMouseOverTipsCfg = p;
		Write16(pMouseOverTipsCfg, 0); //len

		Write16(pMouseOverTipsCfg, MouseOverTipsCfg[i].ID); //ID
		Write16(pMouseOverTipsCfg, MouseOverTipsCfg[i].PrompLen); //PrompLen
		memcpy(pMouseOverTipsCfg, MouseOverTipsCfg[i].Promp, MouseOverTipsCfg[i].PrompLen);
		pMouseOverTipsCfg += MouseOverTipsCfg[i].PrompLen;

		len = pMouseOverTipsCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}


	len = p - buf.data();
	SendToClient(Client, 25116, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyRedPointInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	WriteHex(p, "2F0004000000040100000402010004030000040400000405000004060000040700000408000004090000040A0000040B0000040C0000040D0000040E0000040F000004100100041100000412000004130000041400000415000004160000041700000418000004190000041A0000041B0000041C0000041D0100041E0000041F000004200000042100000422000004230000042400000425010004260000042700000428000004290000042A0000042B0000042C0000042D0000042E010100");
	/*Write8(p, 0); //RedPointNum

	for (size_t i = 0; i < n; i++)
	{ //RedPointInfo
		BYTE* pRedPointInfo = p;
		Write16(pRedPointInfo, 0); //len

		Write8(pRedPointInfo, 0); //Type
		Write8(pRedPointInfo, 0); //Status

		len = pRedPointInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 1); //All
	Write8(p, 0); //IsNeedPlayVideoGuid*/

	len = p - buf.data();
	SendToClient(Client, 25114, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}



void ResponseC2GSign3Operate(std::shared_ptr<ClientNode> Client, UCHAR OperateType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	if (Uinew) {
		WriteHex(p, "00000000000000000102D20134DA7D0134DA830001070027002100000000000000000000000000000000025800000000000000000000000000000100000027002100000000000000000000032000000000000000000000000000000000000000000200000040003A000000000000000000000000000100190000305A000000010000000000000000000000000000000000000000000000000000000000000000000100000040003A0000000000000000000003E800010019000138D2000000010000016800000000000000000000010000000000000000000000000000000000000200000027002100000000000000000000032000000000000000000000000000000000000000000100000040003A00000000000000000000000000010019000123EB000000010000000000000000000000000000010000000000000000000000000000000000000100000040003A000000000000000000000320000100190000FE1D0000000100000060000000000000000000000100000000000000000000000000000000000002000000000000000000000021000000000000000000000000000000000000000000000000000000000000000000772F43303331D4C236C8D52D31D4C23137C8D52F433030C3BFCCECB5C7C2BCA3ACBFC9BBF1B5C32F433031D6DCC4EAB5B9CAFDC5C62F43303031B8F6A3ACB4F2BFAAB1D8B5C3333030B5E3C8AFA3ACD3D0BBFABBE1BBF1B5C3CBE6BBFAB8C4D7B0C5E4BCFEA1A2D3C0BEC341B3B5B0D7D2F8B5DBCDF5A3A100000000030200009E0000000000000000000005DC00050019000175AC00000001FFFFFFFF0000000000000000000001001900010E1200000001FFFFFFFF0000000000000000000001001900013F5000000001FFFFFFFF0000000000000000000001001900005A9E00000001FFFFFFFF0000000000000000000000001900005D1200000001FFFFFFFF000000000000000000000000000000000000000000000000000000000134DA220134DA83000000");
		len = p - buf.data();
		SendToClient(Client, 1401, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		return;
	}

	int SignNum = 0;
	std::string SignTime = "";
	char CurTm[27]{ 0 };
	BOOL HasNoResult = FALSE;
	BOOL CanSign = FALSE;
	int SignWeek = 0;
	BOOL FinalAward = FALSE;


	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select SignNum,SignTime,SignWeek from Sign3Info where Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			while (res->next()) {
				SignNum = res->getUInt("SignNum");
				SignTime = UTF2GB(res->getString("SignTime").c_str());
				SignWeek = res->getUInt("SignWeek");
			}
		}
		else {
			HasNoResult = TRUE;
		}
		res.reset();


	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("ResponseC2GSign3Operate1 error\n");
	}


	if (HasNoResult)
	{
		InsertSign3Info(Client);
	}
	if (SignTime.empty())
	{
		CanSign = TRUE;
		formatTimeStamp(GetSysTimeMicros(), CurTm);
	}
	else
	{
		formatTimeStamp(GetSysTimeMicros(), CurTm);
		if (!SignTime.empty())
		{
			if (strcmp(SignTime.c_str(), CurTm) != 0)
			{
				CanSign = TRUE;
			}
		}
	}
	if (SignWeek >= 3)
	{
		FinalAward = TRUE;
	}
	if (OperateType == 2 && !FinalAward)
	{
		ResponseStartGame(Client, "印记不足,无法抽奖!");
		return;
	}
	if (OperateType == 1 && !CanSign)
	{
		ResponseStartGame(Client, "签到异常!");
		return;
	}
	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write8(p, OperateType); //OperateType
	Write8(p, 1); //HasSign3Info
	{ //Sign3Info
		BYTE* pSign3Info = p;
		Write16(pSign3Info, 0); //len

		Write32(pSign3Info, 0); //BeginDate
		Write32(pSign3Info, 0); //EndDate
		Write8(pSign3Info, SignNum); //SignNum 
		Write8(pSign3Info, CanSign); //CanSign 
		Write8(pSign3Info, 7); //AwardNum
		for (size_t i = 0; i < 7; i++)
		{ //Sign2Award
			BYTE* pSign2Award = pSign3Info;
			Write16(pSign2Award, 0); //len
			{ //Award
				BYTE* pAward = pSign2Award;
				Write16(pAward, 0); //len

				Write32(pAward, Sign3Awrad[i].Experience); //Exp
				Write32(pAward, 0); //Money
				Write32(pAward, Sign3Awrad[i].SuperMoney); //SuperMoney
				Write16(pAward, Sign3Awrad[i].Items.size()); //ItemNum
				for (size_t j = 0; j < Sign3Awrad[i].Items.size(); j++)
				{ //ItemInfo
					BYTE* pItemInfo = pAward;
					Write16(pItemInfo, 0); //len
					//printf("%d\n", Sign3Awrad[i].Items[j].ItemID);
					Write32(pItemInfo, Sign3Awrad[i].Items[j].ItemID); //ItemID
					Write32(pItemInfo, Sign3Awrad[i].Items[j].ItemNum); //ItemNum
					Write32(pItemInfo, Sign3Awrad[i].Items[j].AvailPeriod); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, Sign3Awrad[i].Items[j].ItemType); //ItemType

					len = pItemInfo - pAward;
					Set16(pAward, (WORD)len);
					pAward += len;
				}
				Write32(pAward, 0); //Coupons 消费券 
				Write32(pAward, 0); //GuildPoint
				Write32(pAward, 0); //LuckMoney
				Write8(pAward, 0); //ExtendInfoNum
				Write32(pAward, 0); //SpeedCoin

				len = pAward - pSign2Award;
				Set16(pSign2Award, (WORD)len);
				pSign2Award += len;
			}
			Write16(pSign2Award, 0); //AwardTimes
			Write8(pSign2Award, 0); //HighGrade
			Write8(pSign2Award, 0); //FuncDes

			len = pSign2Award - pSign3Info;
			Set16(pSign3Info, (WORD)len);
			pSign3Info += len;
		}
		Write32(pSign3Info, 0); //SpecailAwardBeginDate
		Write32(pSign3Info, 0); //SpecailAwardEndDate
		{ //SpecialAward
			BYTE* pSpecialAward = pSign3Info;
			Write16(pSpecialAward, 0); //len

			Write32(pSpecialAward, 0); //Exp
			Write32(pSpecialAward, 0); //Money
			Write32(pSpecialAward, 0); //SuperMoney
			Write16(pSpecialAward, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pSpecialAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pSpecialAward;
				Set16(pSpecialAward, (WORD)len);
				pSpecialAward += len;
			}
			Write32(pSpecialAward, 0); //Coupons
			Write32(pSpecialAward, 0); //GuildPoint
			Write32(pSpecialAward, 0); //LuckMoney
			Write8(pSpecialAward, 0); //ExtendInfoNum
			Write32(pSpecialAward, 0); //SpeedCoin

			len = pSpecialAward - pSign3Info;
			Set16(pSign3Info, (WORD)len);
			pSign3Info += len;
		}
		Write8(pSign3Info, 0); //SpecialAwardState
		Write8(pSign3Info, 0); //IsSpecailAwardRemind
		Write8(pSign3Info, 0); //TipsLen
		Write32(pSign3Info, 0); //SpecialAwardID
		Write8(pSign3Info, 3); //LabelNum 
		Write8(pSign3Info, SignWeek); //GotLabelNum 
		Write8(pSign3Info, FinalAward); //FinalAwardState 
		{ //FinalAward
			BYTE* pFinalAward = pSign3Info;
			Write16(pFinalAward, 0); //len

			Write32(pFinalAward, Sign3Awrad[7].Experience); //Exp
			Write32(pFinalAward, 0); //Money
			Write32(pFinalAward, Sign3Awrad[7].SuperMoney); //SuperMoney
			Write16(pFinalAward, Sign3Awrad[7].Items.size()); //ItemNum
			for (size_t i = 0; i < Sign3Awrad[7].Items.size(); i++)
			{ //ItemInfo
				BYTE* pItemInfo = pFinalAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Sign3Awrad[7].Items[i].ItemID); //ItemID
				Write32(pItemInfo, Sign3Awrad[7].Items[i].ItemNum); //ItemNum
				Write32(pItemInfo, Sign3Awrad[7].Items[i].AvailPeriod); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, Sign3Awrad[7].Items[i].ItemType); //ItemType

				len = pItemInfo - pFinalAward;
				Set16(pFinalAward, (WORD)len);
				pFinalAward += len;
			}
			Write32(pFinalAward, 0); //Coupons
			Write32(pFinalAward, 0); //GuildPoint
			Write32(pFinalAward, 0); //LuckMoney
			Write8(pFinalAward, 0); //ExtendInfoNum
			Write32(pFinalAward, 0); //SpeedCoin

			len = pFinalAward - pSign3Info;
			Set16(pSign3Info, (WORD)len);
			pSign3Info += len;
		}
		Write32(pSign3Info, 0); //SeasonBeginDate
		Write32(pSign3Info, 0); //SeasonEndDate

		len = pSign3Info - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //SignNum
	if (OperateType == 0x1)
	{
		Write8(p, 1); //HasAward
		{ //Award
			BYTE* pAward = p;
			Write16(pAward, 0); //len

			Write32(pAward, Sign3Awrad[SignNum].Experience); //Exp
			Write32(pAward, 0); //Money
			Write32(pAward, Sign3Awrad[SignNum].SuperMoney); //SuperMoney
			Write16(pAward, Sign3Awrad[SignNum].Items.size()); //ItemNum
			for (int i = 0; i < Sign3Awrad[SignNum].Items.size(); i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Sign3Awrad[SignNum].Items[i].ItemID); //ItemID
				Write32(pItemInfo, Sign3Awrad[SignNum].Items[i].ItemNum); //ItemNum
				Write32(pItemInfo, Sign3Awrad[SignNum].Items[i].AvailPeriod); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, Sign3Awrad[SignNum].Items[i].ItemType); //ItemType

				len = pItemInfo - pAward;
				Set16(pAward, (WORD)len);
				pAward += len;
			}
			Write32(pAward, 0); //Coupons
			Write32(pAward, 0); //GuildPoint
			Write32(pAward, 0); //LuckMoney
			Write8(pAward, 0); //ExtendInfoNum
			Write32(pAward, 0); //SpeedCoin

			len = pAward - p;
			Set16(p, (WORD)len);
			p += len;
		}
		//	CurTm = formatTimeStamp(GetSysTimeMicros());
		SelectClientExp(Client);
		Client->Experience = (Client->Experience > UINT_MAX - Sign3Awrad[SignNum].Experience) ? UINT_MAX : Client->Experience + Sign3Awrad[SignNum].Experience;
		//Client->Experience += Sign3Awrad[SignNum].Experience;
		Client->SuperMoney += Sign3Awrad[SignNum].SuperMoney;
		for (int i = 0; i < Sign3Awrad[SignNum].Items.size(); i++)
		{
			PlayerDB_AddItem2(Client, Client->Uin, Sign3Awrad[SignNum].Items[i].ItemID, Sign3Awrad[SignNum].Items[i].ItemNum, -1, 0);
		}

		UpdateClientBase(Client);
		char NextSignDay = SignNum + 1;
		if (NextSignDay == 7)
		{
			NextSignDay = 0;
			SignWeek += 1;
		}

		UpdateSign3Info(Client, NextSignDay, CurTm, SignWeek);
		NotifyClientUpdateMoneyExp(Client);
		NotifyClientAddItem(Client, Sign3Awrad[SignNum].Items.size(), Sign3Awrad[SignNum].Items);
	}
	else if (OperateType == 0x2)//抽奖
	{
		int ranindex = rand() % Sign3Awrad[7].Items.size();//随机
		Write8(p, 1); //HasAward
		{ //Award
			BYTE* pAward = p;
			Write16(pAward, 0); //len

			Write32(pAward, Sign3Awrad[7].Experience); //Exp
			Write32(pAward, 0); //Money
			Write32(pAward, Sign3Awrad[7].SuperMoney); //SuperMoney
			Write16(pAward, 1); //ItemNum
			{ //ItemInfo
				BYTE* pItemInfo = pAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, Sign3Awrad[7].Items[ranindex].ItemID); //ItemID
				Write32(pItemInfo, Sign3Awrad[7].Items[ranindex].ItemNum); //ItemNum
				Write32(pItemInfo, Sign3Awrad[7].Items[ranindex].AvailPeriod); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, Sign3Awrad[7].Items[ranindex].ItemType); //ItemType

				len = pItemInfo - pAward;
				Set16(pAward, (WORD)len);
				pAward += len;
			}
			Write32(pAward, 0); //Coupons
			Write32(pAward, 0); //GuildPoint
			Write32(pAward, 0); //LuckMoney
			Write8(pAward, 0); //ExtendInfoNum
			Write32(pAward, 0); //SpeedCoin

			len = pAward - p;
			Set16(p, (WORD)len);
			p += len;
		}
		SelectClientExp(Client);

		//Client->Experience += Sign3Awrad[7].Experience;
		Client->SuperMoney += Sign3Awrad[7].SuperMoney;
		Client->Experience = (Client->Experience > UINT_MAX - Sign3Awrad[7].Experience) ? UINT_MAX : Client->Experience + Sign3Awrad[7].Experience;

		PlayerDB_AddItem2(Client, Client->Uin, Sign3Awrad[7].Items[ranindex].ItemID, Sign3Awrad[7].Items[ranindex].ItemNum, -1, 0);
		UpdateClientBase(Client);
		UpdateSign3InfoWeek(Client);
		NotifyClientUpdateMoneyExp(Client);

		NotifyClientAddItem(Client, 1, { Sign3Awrad[7].Items[ranindex] });
	}
	else
	{
		Write8(p, 0); //HasAward
	}

	Write8(p, 0); //LabelNum

	len = p - buf.data();
	SendToClient(Client, 1401, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}




void RequestC2GSign3Operate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UCHAR OperateType = Read8(Body);
	ResponseC2GSign3Operate(Client, OperateType);


}

void RequestGetActivityCenterInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	ResponseGetActivityCenterInfo(Client);
}

void ResponseGetActivityCenterInfo(std::shared_ptr<ClientNode> Client)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len = 0;

	Write32(p, Client->Uin); //Uin
	WriteHex(p, "0000DC100000012C000001B30000006E00000003000000700000DCD405140134DA7F0000000500D0000000010000000000000000000000140000000100000181002100000000000000000000006400000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000000C000000640000000000000000000000014D000000020000000000000000000000280000000100000183003A0000000000000000000000000001001900005E8F000000010000004800000000000000000000000000000000000000000000000000000000003A00000000000000000000000000010019000059C8000000010000004800000000000000000000000000000000000000000000000000000000003A0000000000000000000000000001001900010AEA000000010000004800000000000000000000000000000000000000000000000000000000003A000000000000000000000000000100190000358C000000010000000000000000000000000000000000000000000000000000000000000000003A0000000000000000000000000001001900002813000000010000001800000000000000000000000000000000000000000000000000000000000C00000000000000000000000000010000D00000000300000000000000000000003C0000000100000184002100000000000000000000000000000000019000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000000C000000000000019000000000000000014D000000040000000000000000000000500000000100000185003A0000000000000000000000000001001900006B7F000000010000004800000000000000000000000000000000000000000000000000000000003A0000000000000000000000000001001900008038000000010000004800000000000000000000000000000000000000000000000000000000003A000000000000000000000000000100190001213D000000010000004800000000000000000000000000000000000000000000000000000000003A0000000000000000000000000001001900003059000000010000000000000000000000000000000000000000000000000000000000000000003A00000000000000000000000000010019000055D9000000010000000000000000000000000000000000000000000000000000000000000000000C00000000000000000000000000020000D0000000050000000000000000000000640000000100000190002100000000000000000000032000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000002100000000000000000000000000000000000000000000000000000000000000000C00000320000000000000000000000002000000000300DE0000000100000001000000010000000B0014000011C3000000140002000011C4000011C5001800000E9600000014000300000E7900000E7A00000E7B001400000E970000000A000200000E7D00000E7E000C00000E54000000140000000C000000EF0000000F0000000C0000012A0000000F0000001800000E950000000A000300000D1200000D8600000DAC000C000008E1000000140000001C00000E930000000F000400000B4600000B4700000B4900000E77001C00000E940000000F000400000B4500000B4800000B4A00000E78000C00000CE20000000F0000007200000002000000020000000100000008000C000009930000000F0000000C000000F60000000F0000000C00000E7F0000000F0000000C000011CB0000000F0000000C00000E800000000F0000000C0000067C0000000F0000000C00000E7C0000000F0000000C000011CA0000000F000000AA0000000300000003000000010000000A000C0000094B0000000F0000001400000E9800000005000200000E8200000E830014000011C70000000F0002000011C8000011C9000C00000E840000000F0000001400000E990000000A000200000E8500000E86001400000E9A0000000A000200000E8700000E88000C000005B50000000F0000000C000005B80000000A0000000C00000E8A0000000F0000000C00000E890000000F00000000000000000C000000000000000000000000012C0200004F1400004CA4");
	len = p - buf.data();
	SendToClient(Client, 24029, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;

	Write32(p, Client->Uin); //Uin
	Write32(p, 34620); //TotalActivity
	Write32(p, 6666); //CurrentWeekActivity本周活跃
	Write32(p, 6666); //LastWeekActivity
	Write32(p, 666); //CurrentDayActivity
	Write32(p, 99); //ActivityAppellcation
	Write32(p, 66); //ActivityLevel
	Write32(p, 34630); //NextLevelActivity

	{ //DailyActivityBox
		BYTE* pDailyActivityBox = p;
		Write16(pDailyActivityBox, 0); //len

		Write32(pDailyActivityBox, 0x0134d82b); //Day
		Write32(pDailyActivityBox, 1); //BoxNum

		for (size_t i = 0; i < 1; i++)
		{ //ActivityBox
			BYTE* pActivityBox = pDailyActivityBox;
			Write16(pActivityBox, 0); //len

			Write32(pActivityBox, i + 1); //BoxLevel
			Write32(pActivityBox, 0); //BoxType
			Write32(pActivityBox, 0); //ActivityType
			Write32(pActivityBox, 20 * (i + 1)); //NeedActivityNum
			Write32(pActivityBox, 0); //BoxStatus
			Write32(pActivityBox, 0x00000181); //BoxID
			for (size_t ii = 0; ii < 5; ii++)
			{ //Award
				BYTE* pAward = pActivityBox;
				Write16(pAward, 0); //len

				Write32(pAward, 0); //Exp
				Write32(pAward, 0); //Money
				Write32(pAward, 0); //SuperMoney
				Write16(pAward, 1); //ItemNum
				for (size_t iii = 0; iii < 1; iii++)
				{ //ItemInfo
					BYTE* pItemInfo = pAward;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, 127616); //ItemID
					Write32(pItemInfo, 1); //ItemNum
					Write32(pItemInfo, -1); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, 1); //ItemType

					len = pItemInfo - pAward;
					Set16(pAward, (WORD)len);
					pAward += len;
				}
				Write32(pAward, 0); //Coupons
				Write32(pAward, 0); //GuildPoint
				Write32(pAward, 0); //LuckMoney
				Write8(pAward, 0); //ExtendInfoNum
				for (size_t iiii = 0; iiii < 0; iiii++)
				{ //ExtendInfoAward
					BYTE* pExtendInfoAward = pAward;
					Write16(pExtendInfoAward, 0); //len

					Write16(pExtendInfoAward, 0); //Key
					Write32(pExtendInfoAward, 0); //AddValue
					Write32(pExtendInfoAward, 0); //TotalValue
					Write8(pExtendInfoAward, 0); //ShowPriority
					Write16(pExtendInfoAward, 0); //BuyNeedScore
					Write8(pExtendInfoAward, 0); //OneMatchMaxNum

					len = pExtendInfoAward - pAward;
					Set16(pAward, (WORD)len);
					pAward += len;
				}
				Write32(pAward, 0); //SpeedCoin

				len = pAward - pActivityBox;
				pActivityBox += len;
			}
			{ //PreviewAward
				BYTE* pPreviewAward = pActivityBox;
				Write16(pPreviewAward, 0); //len

				Write32(pPreviewAward, 0); //SuperMoney
				Write32(pPreviewAward, 0); //Coupons
				Write16(pPreviewAward, 5); //ItemNum
				for (size_t iii = 0; iii < 5; iii++)
				{ //ItemInfo
					BYTE* pItemInfo = pPreviewAward;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, 10020); //ItemID
					Write32(pItemInfo, 1); //ItemNum
					Write32(pItemInfo, 0); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					WriteUn(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, 0); //ItemType

					len = pItemInfo - pPreviewAward;
					Set16(pPreviewAward, (WORD)len);
					pPreviewAward += len;
				}
				len = pPreviewAward - pActivityBox;
				Set16(pActivityBox, (WORD)len);
				pActivityBox += len;
			}
			Write32(pActivityBox, 0); //PresetBoxID
			Write8(pActivityBox, 0); //HasActiveAward

			len = pActivityBox - pDailyActivityBox;
			Set16(pDailyActivityBox, (WORD)len);
			pDailyActivityBox += len;
		}

		len = pDailyActivityBox - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //ActivityTaskCfg
		BYTE* pActivityTaskCfg = p;
		Write16(pActivityTaskCfg, 0); //len

		Write32(pActivityTaskCfg, 3); //TabNum

		for (size_t i = 0; i < 3; i++)
		{ //ActivityTaskTab
			BYTE* pActivityTaskTab = pActivityTaskCfg;
			Write16(pActivityTaskTab, 0); //len

			Write32(pActivityTaskTab, 1); //TabOrder
			Write32(pActivityTaskTab, 1); //TabType
			Write32(pActivityTaskTab, 1); //Show
			Write32(pActivityTaskTab, 1); //TaskNum
			for (size_t ii = 0; ii < 1; ii++)
			{ //ActivityTask
				BYTE* pActivityTask = pActivityTaskTab;
				Write16(pActivityTask, 0); //len

				Write32(pActivityTask, 0x00000699); //TaskID
				Write32(pActivityTask, 0x00000005); //ActivityAward
				Write16(pActivityTask, 0); //ChildTaskNum
				for (size_t iii = 0; iii < 0; iii++)
				{
					Write32(pActivityTask, 0); //ChildTaskID[]
				}
				len = pActivityTask - pActivityTaskTab;
				Set16(pActivityTaskTab, (WORD)len);
				pActivityTaskTab += len;
			}

			len = pActivityTaskTab - pActivityTaskCfg;
			Set16(pActivityTaskCfg, (WORD)len);
			pActivityTaskCfg += len;
		}

		len = pActivityTaskCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //BCNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //BC
		BYTE* pBC = p;
		Write16(pBC, 0); //len

		Write16(pBC, 0); //ContentLen

		len = pBC - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write32(p, 0); //CurrentWeekActivityUsed
	{ //MultipleTimeCfg
		BYTE* pMultipleTimeCfg = p;
		Write16(pMultipleTimeCfg, 0); //len

		Write32(pMultipleTimeCfg, 0); //BeginTime
		Write32(pMultipleTimeCfg, 0); //EndTime
		Write16(pMultipleTimeCfg, 0); //MultiValue

		len = pMultipleTimeCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0x00000023); //CurrentWeekActivityForLotto
	Write8(p, 0x02); //LottoItemNum
	Write32(p, 0x00004f14); //LottoItemID[]
	Write32(p, 0x00004ca4); //LottoItemID[]
	Write16(p, 0x4d00);

	len = p - buf.data();
	SendToClient(Client, 24029, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestHyperSpaceJumboGetCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UCHAR CfgType = Read8(Body);

	//printf("%c", CfgType);
	ResponseHyperSpaceJumboGetCfg(Client, CfgType);
	//ResponseTimerChallenge2ndMoreHallInfo(Client);
}

void ResponseHyperSpaceJumboGetCfg(std::shared_ptr<ClientNode> Client, UCHAR CfgType)
{

	BYTE buf[8192] = { 0 };
	BYTE* p = buf;
	size_t len = 0;

	Write32(p, Client->Uin); // Uin
	Write16(p, 0);			 // ResultID

	Write32(p, 0); // BeginTime
	// day 20220419 修复结算问题
	Write32(p, 240000); // EndTime
	// Write32(p, 480000); //EndTime

	Write16(p, 0); // MapNum
	// Write32(p, 0); //MapIDList[]

	const int MAX_HYPER_SPACE_JUMBO_BEST_ITEM_MUM = 30;
	int BestItemIDList[MAX_HYPER_SPACE_JUMBO_BEST_ITEM_MUM] = { 27235, 24206, 31574, 32407 };
	for (size_t i = 0; i < MAX_HYPER_SPACE_JUMBO_BEST_ITEM_MUM; i++)
	{
		Write32(p, BestItemIDList[i]); // BestItemIDList[]
	}

	Write32(p, 0);		// LeftOpenBoxNum
	Write8(p, CfgType); // CfgType
	Write32(p, 0);		// Para1

	Write16(p, 17); // ValueNum
	int Values[17] = {};
	for (size_t i = 0; i < 17; i++)
	{
		Write32(p, Values[i]); // Values[]
	}

	Write16(p, 0); // MapListNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //MapList
		BYTE* pMapList = p;
		Write16(pMapList, 0); //len

		Write32(pMapList, 0); //MapID
		Write8(pMapList, 0); //Para1

		len = pMapList - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	Write16(p, 0); // HighBoxAwardNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //HighBoxAward
		BYTE* pHighBoxAward = p;
		Write16(pHighBoxAward, 0); //len

		Write32(pHighBoxAward, 0); //Exp
		Write32(pHighBoxAward, 0); //Money
		Write32(pHighBoxAward, 0); //SuperMoney
		Write16(pHighBoxAward, 0); //ItemNum
		for (size_t i = 0; i < n; i++)
		{ //ItemInfo
			BYTE* pItemInfo = pHighBoxAward;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, 0); //ItemID
			Write32(pItemInfo, 0); //ItemNum
			Write32(pItemInfo, 0); //AvailPeriod
			Write8(pItemInfo, 0); //Status
			WriteUn(pItemInfo, 0); //ObtainTime
			Write32(pItemInfo, 0); //OtherAttribute
			Write16(pItemInfo, 0); //ItemType

			len = pItemInfo - pHighBoxAward;
			Set16(pHighBoxAward, (WORD)len);
			pHighBoxAward += len;
		}
		Write32(pHighBoxAward, 0); //Coupons
		Write32(pHighBoxAward, 0); //GuildPoint
		Write32(pHighBoxAward, 0); //LuckMoney
		Write8(pHighBoxAward, 0); //ExtendInfoNum
		for (size_t i = 0; i < n; i++)
		{ //ExtendInfoAward
			BYTE* pExtendInfoAward = pHighBoxAward;
			Write16(pExtendInfoAward, 0); //len

			Write16(pExtendInfoAward, 0); //Key
			Write32(pExtendInfoAward, 0); //AddValue
			Write32(pExtendInfoAward, 0); //TotalValue
			Write8(pExtendInfoAward, 0); //ShowPriority
			Write16(pExtendInfoAward, 0); //BuyNeedScore
			Write8(pExtendInfoAward, 0); //OneMatchMaxNum

			len = pExtendInfoAward - pHighBoxAward;
			Set16(pHighBoxAward, (WORD)len);
			pHighBoxAward += len;
		}
		Write32(pHighBoxAward, 0); //SpeedCoin

		len = pHighBoxAward - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	Write16(p, 0); // LowBoxAwardNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //LowBoxAward
		BYTE* pLowBoxAward = p;
		Write16(pLowBoxAward, 0); //len

		Write32(pLowBoxAward, 0); //Exp
		Write32(pLowBoxAward, 0); //Money
		Write32(pLowBoxAward, 0); //SuperMoney
		Write16(pLowBoxAward, 0); //ItemNum
		for (size_t i = 0; i < n; i++)
		{ //ItemInfo
			BYTE* pItemInfo = pLowBoxAward;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, 0); //ItemID
			Write32(pItemInfo, 0); //ItemNum
			Write32(pItemInfo, 0); //AvailPeriod
			Write8(pItemInfo, 0); //Status
			WriteUn(pItemInfo, 0); //ObtainTime
			Write32(pItemInfo, 0); //OtherAttribute
			Write16(pItemInfo, 0); //ItemType

			len = pItemInfo - pLowBoxAward;
			Set16(pLowBoxAward, (WORD)len);
			pLowBoxAward += len;
		}
		Write32(pLowBoxAward, 0); //Coupons
		Write32(pLowBoxAward, 0); //GuildPoint
		Write32(pLowBoxAward, 0); //LuckMoney
		Write8(pLowBoxAward, 0); //ExtendInfoNum
		for (size_t i = 0; i < n; i++)
		{ //ExtendInfoAward
			BYTE* pExtendInfoAward = pLowBoxAward;
			Write16(pExtendInfoAward, 0); //len

			Write16(pExtendInfoAward, 0); //Key
			Write32(pExtendInfoAward, 0); //AddValue
			Write32(pExtendInfoAward, 0); //TotalValue
			Write8(pExtendInfoAward, 0); //ShowPriority
			Write16(pExtendInfoAward, 0); //BuyNeedScore
			Write8(pExtendInfoAward, 0); //OneMatchMaxNum

			len = pExtendInfoAward - pLowBoxAward;
			Set16(pLowBoxAward, (WORD)len);
			pLowBoxAward += len;
		}
		Write32(pLowBoxAward, 0); //SpeedCoin

		len = pLowBoxAward - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	Write32(p, 0); // Para2

	Write16(p, 0); // ValueNum2
	// Write32(p, 0); //Values2[]

	Write8(p, 0); // HasYuLeJiaNianHuaInfo
	/*
	for (size_t i = 0; i < n; i++)
	{ //YuLeJiaNianHuaInfo
		BYTE* pYuLeJiaNianHuaInfo = p;
		Write16(pYuLeJiaNianHuaInfo, 0); //len

		Write32(pYuLeJiaNianHuaInfo, 0); //Mode
		Write32(pYuLeJiaNianHuaInfo, 0); //Score
		Write8(pYuLeJiaNianHuaInfo, 0); //RankRangNum
		Write32(pYuLeJiaNianHuaInfo, 0); //RankRange[]
		Write8(pYuLeJiaNianHuaInfo, 0); //RankScoreNum
		Write32(pYuLeJiaNianHuaInfo, 0); //RankScore[]
		Write8(pYuLeJiaNianHuaInfo, 0); //InRecommend
		Write8(pYuLeJiaNianHuaInfo, 0); //InDoubleScore

		len = pYuLeJiaNianHuaInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	len = p - buf;
	SendToClient(Client, 24202, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestTimerChallenge2ndMoreHallInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseTimerChallenge2ndMoreHallInfo(Client);
}

void ResponseTimerChallenge2ndMoreHallInfo(std::shared_ptr<ClientNode> Client)
{
	if (Client->MapID == 431)
	{
		const char* buf = "\x00\x00\x27\x11\x00\x00\x00\x0b\x00\x00\x00\x78\x00\x00\x00\x55\x00\x00\x00\x78\x00\x00\x27\x66\x00\x00\x00\x01\x00\x00\x00\x0f\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x78\x00\x14\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x05\x01\x00\x00\x00\x24\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x02\x00\x00\x00\x0a\x01\x00\x00\x00\x47\x00\x00\x14\x00\x00\x00\x0f\x00\x00\x00\x03\x00\x00\x00\x0f\x01\x00\x00\x00\x71\x00\x00\x14\x00\x00\x00\x14\x00\x00\x00\x02\x00\x00\x00\x14\x01\x00\x00\x00\x9a\x00\x00\x14\x00\x00\x00\x19\x00\x00\x00\x07\x00\x00\x00\x19\x01\x00\x00\x00\xc6\x00\x00\x14\x00\x00\x00\x1e\x00\x00\x00\x08\x00\x00\x00\x1e\x01\x00\x00\x00\xf7\x00\x00\x14\x00\x00\x00\x23\x00\x00\x00\x05\x00\x00\x00\x23\x01\x00\x00\x01\x29\x00\x00\x14\x00\x00\x00\x28\x00\x00\x00\x05\x00\x00\x00\x28\x01\x00\x00\x01\x50\x00\x00\x14\x00\x00\x00\x2d\x00\x00\x00\x05\x00\x00\x00\x2d\x01\x00\x00\x01\x85\x00\x00\x14\x00\x00\x00\x32\x00\x00\x00\x05\x00\x00\x00\x32\x01\x00\x00\x01\xb4\x00\x00\x14\x00\x00\x00\x37\x00\x00\x00\x05\x00\x00\x00\x37\x01\x00\x00\x01\xd7\x00\x00\x14\x00\x00\x00\x3c\x00\x00\x00\x05\x00\x00\x00\x3c\x01\x00\x00\x02\x0d\x00\x00\x14\x00\x00\x00\x41\x00\x00\x00\x05\x00\x00\x00\x41\x01\x00\x00\x02\x37\x00\x00\x14\x00\x00\x00\x46\x00\x00\x00\x05\x00\x00\x00\x46\x01\x00\x00\x02\x53\x00\x00\x14\x00\x00\x00\x4b\x00\x00\x00\x05\x00\x00\x00\x4b\x01\x00\x00\x02\x8a\x00\x00\x14\x00\x00\x00\x50\x00\x00\x00\x03\x00\x00\x00\x50\x01\x00\x00\x02\xae\x00\x00\x14\x00\x00\x00\x55\x00\x00\x00\x03\x00\x00\x00\x55\x01\x00\x00\x02\xcb\x00\x00\x14\x00\x00\x00\x5a\x00\x00\x00\x04\x00\x00\x00\x5a\x01\x00\x00\x02\xf3\x00\x00\x14\x00\x00\x00\x5f\x00\x00\x00\x04\x00\x00\x00\x5f\x01\x00\x00\x03\x19\x00\x00\x14\x00\x00\x00\x64\x00\x00\x00\x03\x00\x00\x00\x64\x01\x00\x00\x03\x38\x00\x00\x14\x00\x00\x00\x69\x00\x00\x00\x00\x00\x00\x00\x69\x01\x00\x00\x03\x62\x00\x00\x14\x00\x00\x00\x6e\x00\x00\x00\x00\x00\x00\x00\x6e\x01\x00\x00\x03\x88\x00\x00\x14\x00\x00\x00\x73\x00\x00\x00\x00\x00\x00\x00\x73\x01\x00\x00\x03\xa8\x00\x00\x14\x00\x00\x00\x78\x00\x00\x00\x00\x00\x00\x00\x78\x01\x00\x00\x03\xda\x00\x00\x14\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x05\x00\x00\x14\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x10\x00\x00\x14\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x18\x00\x00\x14\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x1e\x00\x00\x14\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x2b\x00\x00\x14\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x31\x00\x00\x14\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x3a\x00\x00\x14\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x41\x00\x00\x18\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x4f\x00\x00\x03\xe2\x00\x00\x18\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x57\x00\x00\x03\xea\x00\x00\x18\x00\x00\x00\x0d\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x60\x00\x00\x03\xf3\x00\x00\x18\x00\x00\x00\x0e\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x68\x00\x00\x03\xfb\x00\x00\x14\x00\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x7a\x00\x00\x14\x00\x00\x00\x11\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x82\x00\x00\x14\x00\x00\x00\x12\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x8b\x00\x00\x14\x00\x00\x00\x13\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x91\x00\x00\x14\x00\x00\x00\x15\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xa3\x00\x00\x14\x00\x00\x00\x16\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xaa\x00\x00\x14\x00\x00\x00\x17\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xb3\x00\x00\x14\x00\x00\x00\x18\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xbf\x00\x00\x18\x00\x00\x00\x1a\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xd2\x00\x00\x04\x0a\x00\x00\x18\x00\x00\x00\x1b\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xde\x00\x00\x04\x15\x00\x00\x18\x00\x00\x00\x1c\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xe8\x00\x00\x04\x20\x00\x00\x18\x00\x00\x00\x1d\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xf3\x00\x00\x04\x2b\x00\x00\x14\x00\x00\x00\x1f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x02\x00\x00\x14\x00\x00\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x0c\x00\x00\x14\x00\x00\x00\x21\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x13\x00\x00\x14\x00\x00\x00\x22\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x20\x00\x00\x14\x00\x00\x00\x24\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x30\x00\x00\x14\x00\x00\x00\x25\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x37\x00\x00\x14\x00\x00\x00\x26\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x3f\x00\x00\x14\x00\x00\x00\x27\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x48\x00\x00\x18\x00\x00\x00\x29\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x59\x00\x00\x04\x36\x00\x00\x18\x00\x00\x00\x2a\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x6c\x00\x00\x04\x49\x00\x00\x18\x00\x00\x00\x2b\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x72\x00\x00\x04\x50\x00\x00\x18\x00\x00\x00\x2c\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x7d\x00\x00\x04\x5b\x00\x00\x14\x00\x00\x00\x2e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x8c\x00\x00\x14\x00\x00\x00\x2f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x96\x00\x00\x14\x00\x00\x00\x30\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x9d\x00\x00\x14\x00\x00\x00\x31\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xa7\x00\x00\x14\x00\x00\x00\x33\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xbd\x00\x00\x14\x00\x00\x00\x34\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xc8\x00\x00\x14\x00\x00\x00\x35\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xcd\x00\x00\x14\x00\x00\x00\x36\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xd3\x00\x00\x18\x00\x00\x00\x38\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xe1\x00\x00\x04\x69\x00\x00\x18\x00\x00\x00\x39\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xed\x00\x00\x04\x74\x00\x00\x18\x00\x00\x00\x3a\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xfa\x00\x00\x04\x81\x00\x00\x18\x00\x00\x00\x3b\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x01\x00\x00\x04\x88\x00\x00\x14\x00\x00\x00\x3d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x13\x00\x00\x14\x00\x00\x00\x3e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x1b\x00\x00\x14\x00\x00\x00\x3f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x25\x00\x00\x14\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x2f\x00\x00\x14\x00\x00\x00\x42\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x3c\x00\x00\x14\x00\x00\x00\x43\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x40\x00\x00\x14\x00\x00\x00\x44\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x46\x00\x00\x14\x00\x00\x00\x45\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x4f\x00\x00\x18\x00\x00\x00\x47\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x61\x00\x00\x04\xa0\x00\x00\x18\x00\x00\x00\x48\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x68\x00\x00\x04\xa5\x00\x00\x18\x00\x00\x00\x49\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x77\x00\x00\x04\xb4\x00\x00\x18\x00\x00\x00\x4a\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x80\x00\x00\x04\xbd\x00\x00\x14\x00\x00\x00\x4c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x92\x00\x00\x14\x00\x00\x00\x4d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x9b\x00\x00\x14\x00\x00\x00\x4e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xa2\x00\x00\x14\x00\x00\x00\x4f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xa9\x00\x00\x14\x00\x00\x00\x51\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xb6\x00\x00\x14\x00\x00\x00\x52\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xba\x00\x00\x14\x00\x00\x00\x53\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xbf\x00\x00\x14\x00\x00\x00\x54\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xc7\x00\x00\x18\x00\x00\x00\x56\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xd6\x00\x00\x04\xca\x00\x00\x18\x00\x00\x00\x57\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xdb\x00\x00\x04\xd0\x00\x00\x18\x00\x00\x00\x58\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xe4\x00\x00\x04\xd9\x00\x00\x18\x00\x00\x00\x59\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xe9\x00\x00\x04\xde\x00\x00\x14\x00\x00\x00\x5b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xfa\x00\x00\x14\x00\x00\x00\x5c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x04\x00\x00\x14\x00\x00\x00\x5d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x09\x00\x00\x14\x00\x00\x00\x5e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x13\x00\x00\x14\x00\x00\x00\x60\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x1d\x00\x00\x14\x00\x00\x00\x61\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x21\x00\x00\x14\x00\x00\x00\x62\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x2a\x00\x00\x14\x00\x00\x00\x63\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x33\x00\x00\x18\x00\x00\x00\x65\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x43\x00\x00\x04\xef\x00\x00\x18\x00\x00\x00\x66\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x4b\x00\x00\x04\xf7\x00\x00\x18\x00\x00\x00\x67\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x53\x00\x00\x04\xff\x00\x00\x18\x00\x00\x00\x68\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x5a\x00\x00\x05\x08\x00\x00\x14\x00\x00\x00\x6a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x69\x00\x00\x14\x00\x00\x00\x6b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x70\x00\x00\x14\x00\x00\x00\x6c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x7b\x00\x00\x14\x00\x00\x00\x6d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x7f\x00\x00\x14\x00\x00\x00\x6f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x8c\x00\x00\x14\x00\x00\x00\x70\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x93\x00\x00\x14\x00\x00\x00\x71\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x97\x00\x00\x14\x00\x00\x00\x72\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xa0\x00\x00\x18\x00\x00\x00\x74\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xb5\x00\x00\x05\x15\x00\x00\x18\x00\x00\x00\x75\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xc4\x00\x00\x05\x26\x00\x00\x18\x00\x00\x00\x76\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xcb\x00\x00\x05\x2d\x00\x00\x18\x00\x00\x00\x77\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xd1\x00\x00\x05\x34\x00\x01\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\xc2\x20\x45\x39\x35\x30\x33\x42\x46\x39\x44\x31\x37\x30\x43\x31\x32\x42\x33\x45\x42\x44\x44\x33\x38\x37\x34\x30\x42\x41\x41\x45\x33\x45\x00\x42\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x70\x75\x74\x6f\x6e\x67\x5f\x6c\x6f\x77\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3a\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xc6\xd5\xcd\xa8\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\xc3\x20\x36\x43\x37\x32\x33\x39\x31\x43\x35\x39\x30\x34\x30\x46\x45\x31\x46\x41\x41\x32\x45\x37\x42\x31\x30\x36\x31\x41\x31\x32\x39\x32\x00\x43\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x70\x75\x74\x6f\x6e\x67\x5f\x68\x69\x67\x68\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3a\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xc6\xd5\xcd\xa8\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\xc3\x20\x37\x37\x34\x31\x39\x46\x38\x41\x44\x38\x42\x46\x44\x41\x44\x34\x42\x44\x37\x35\x30\x31\x42\x34\x34\x41\x46\x44\x37\x45\x31\x39\x00\x43\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x63\x68\x65\x73\x68\x65\x6e\x5f\x6c\x6f\x77\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3a\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xb3\xb5\xc9\xf1\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\xc4\x20\x35\x46\x42\x39\x45\x30\x33\x46\x32\x33\x34\x37\x46\x34\x33\x30\x34\x32\x37\x44\x36\x46\x41\x30\x33\x36\x45\x45\x38\x31\x38\x35\x00\x44\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x63\x68\x65\x73\x68\x65\x6e\x5f\x68\x69\x67\x68\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x27\x26\x00\x00\x27\x29\x00\x00\x27\x7a\x00\x00\x00\x00\x00\x00\x27\x3b\x00\x00\x27\x3d\x00\x00\x27\x3c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xb3\xb5\xc9\xf1\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\x00\x00\x00\x00\x00\x50\x0f\x01\x00\x06\x00\x00\x00\x00\x03\x03\x59\x04\x00\x84\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x00\x42\xc1\xf7\xbb\xf0\xc1\xfa\xbe\xa7\xb8\xb3\xd3\xe8\xc8\xfc\xb3\xb5\xb7\xc9\xd0\xd0\xb5\xc4\xc4\xdc\xc1\xa6\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x09\x01\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x3a\x98\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x4c\x76\x32\x00\x16\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\xb5\xc4\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x2d\x35\xc3\xeb\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x4c\x76\x33\x00\x16\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\xb5\xc4\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x2b\x32\xc3\xeb\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x1b\x58\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x4c\x76\x34\x00\x16\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\xb5\xc4\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x2b\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x1b\x58\x00\x00\x27\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x69\x00\x00\x00\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x69\x32\x00\x00\x00\x0a\x00\x08\xb8\xd6\xcc\xfa\xc0\xd7\xc5\xb5\x00\x27\xbf\xc9\xd7\xb0\xb1\xb8\xa1\xbe\xc2\xfa\xb8\xc4\xa1\xbf\x41\xbc\xb6\xc8\xfc\xb3\xb5\xb8\xd6\xcc\xfa\xc0\xd7\xc5\xb5\xbd\xf8\xd0\xd0\xb4\xf3\xb4\xb3\xb9\xd8\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6c\x00\x00\x00\xc2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc5\xf6\xd7\xb2\xbc\xaf\xc6\xf8\x00\x2a\xb7\xc7\xc6\xaf\xd2\xc6\xd7\xb4\xcc\xac\xcf\xc2\xa3\xac\xc3\xbf\xb4\xce\xd7\xb2\xc7\xbd\xa3\xac\x36\x30\x25\xb8\xc5\xc2\xca\xbb\xf1\xb5\xc3\x37\x30\x25\xbc\xaf\xc6\xf8\x01\x00\x00\x00\x00\x00\xc1\x00\x03\x00\x00\x00\x00\xc3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x84\x00\x00\x00\xc5\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0c\xc6\xf0\xb2\xbd\xb6\xee\xcd\xe2\xb5\xaa\xc6\xf8\x00\x3e\xc6\xf0\xb2\xbd\xc1\xa2\xbf\xcc\xbb\xf1\xb5\xc3\xd2\xbb\xb8\xf6\xb5\xaa\xc6\xf8\xa3\xac\xc7\xd2\x31\x30\xc3\xeb\xc4\xda\xb5\xaa\xc6\xf8\xd7\xee\xb8\xdf\xcb\xd9\xb6\xc8\x2b\x32\x2e\x35\x6b\x6d\x2f\x68\xa3\xac\xb5\xaa\xc6\xf8\xb6\xaf\xc1\xa6\x2b\x35\x01\x00\x00\x00\x00\x00\xc4\x00\x03\x00\x00\x00\x00\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x67\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xb6\xaf\xc1\xa6\xcc\xe1\xc9\xfd\x00\x25\xc3\xbf\xb4\xce\xbd\xf8\xc8\xeb\xc6\xaf\xd2\xc6\xa3\xac\x35\x30\x25\xb8\xc5\xc2\xca\x37\xc3\xeb\xc4\xda\xb5\xaa\xc6\xf8\xb6\xaf\xc1\xa6\x2b\x31\x30\x01\x00\x00\x00\x00\x00\xc7\x00\x03\x00\x00\x00\x00\xc9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x58\x04\x00\xa3\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\x00\x61\xc1\xf7\xbb\xf0\xc1\xfa\xbe\xa7\xca\xb9\xc8\xfc\xb5\xc0\xc9\xcf\xc3\xc5\xb1\xe4\xb3\xc9\xcc\xd8\xca\xe2\xca\xb1\xd6\xd3\xc3\xc5\xa3\xac\xcd\xa8\xb9\xfd\xba\xf3\xb4\xb3\xb9\xd8\xca\xb1\xbc\xe4\xb6\xee\xcd\xe2\x2b\x31\x2e\x35\x73\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x36\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x17\x70\x00\x00\x3a\x98\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\x4c\x76\x32\x00\x16\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\xb5\xc4\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x2d\x35\xc3\xeb\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x17\x70\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\x4c\x76\x33\x00\x16\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\xb5\xc4\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x2b\x33\xc3\xeb\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x23\x28\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\x4c\x76\x34\x00\x16\xc4\xdc\xc1\xbf\xca\xb1\xd6\xd3\xb5\xc4\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x2b\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x23\x28\x00\x00\x27\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x69\x00\x00\x00\xca\x00\x00\x00\x00\x00\x00\x00\x00\x00\xec\x0d\xff\xff\xff\xff\x00\x08\xca\xc9\xbb\xea\xbd\xf0\xbc\xd7\x00\x27\xbf\xc9\xd7\xb0\xb1\xb8\xa1\xbe\xc2\xfa\xb8\xc4\xa1\xbf\x54\xbc\xb6\xc8\xfc\xb3\xb5\xca\xc9\xbb\xea\xbd\xf0\xbc\xd7\xbd\xf8\xd0\xd0\xb4\xf3\xb4\xb3\xb9\xd8\x01\x00\x00\x00\x00\x00\x00\x00\x09\x01\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5a\x00\x00\x00\xcc\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xd6\xfa\xc1\xa6\xc6\xf0\xc5\xdc\x00\x18\xc3\xbf\xb0\xf4\xc6\xf0\xc5\xdc\xca\xb1\xa3\xac\xb4\xb3\xb9\xd8\xca\xb1\xbc\xe4\x2b\x31\xc3\xeb\x01\x00\x00\x00\x00\x00\xcb\x00\x03\x01\x00\x00\x00\xcd\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x79\x00\x00\x00\xcf\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc5\xe7\xc9\xe4\xbc\xd3\xb3\xc9\x00\x37\xca\xb9\xd3\xc3\xcb\xab\xc5\xe7\xa3\xa8\xba\xac\x63\x77\x77\xa1\xa2\x77\x63\x77\xa3\xa9\xba\xf3\xa3\xac\xb4\xb3\xb9\xd8\xca\xb1\xbc\xe4\x2b\x31\xc3\xeb\xa3\xac\xa3\xa8\xc3\xbf\xb0\xf4\xc9\xcf\xcf\xde\x32\xb4\xce\xa3\xa9\x01\x00\x00\x00\x00\x00\xce\x00\x03\x01\x00\x00\x00\xd0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x64\x00\x00\x00\xd2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbc\xab\xcb\xd9\xcc\xe1\xc9\xfd\x00\x22\xc3\xbf\x33\xb4\xce\xca\xb9\xd3\xc3\xb5\xaa\xc6\xf8\xa3\xac\x38\xc3\xeb\xc4\xda\xc6\xbd\xc5\xdc\xbc\xab\xcb\xd9\xc4\xdc\xc1\xa6\x2b\x38\x01\x00\x00\x00\x00\x00\xd1\x00\x03\x01\x00\x00\x00\xd3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x33\x04\x00\x97\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\x00\x55\xc0\xa9\xb4\xf3\xb5\xc0\xbe\xdf\xca\xb0\xc8\xa1\xb7\xb6\xce\xa7\xb2\xa2\xc7\xd2\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\xc4\xda\xc3\xbf\xc3\xeb\xd4\xf6\xbc\xd3\x32\x30\x25\xb5\xaa\xc6\xf8\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x36\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x17\x70\x00\x00\x3a\x98\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\x4c\x76\x32\x00\x16\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\xb5\xc4\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x2d\x35\xc3\xeb\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x17\x70\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\x4c\x76\x33\x00\x16\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\xb5\xc4\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x2b\x33\xc3\xeb\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x23\x28\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\x4c\x76\x34\x00\x16\xc1\xfa\xbe\xa7\xb4\xc5\xce\xfc\xb5\xc4\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x2b\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x23\x28\x00\x00\x27\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x5d\x00\x00\x00\xd4\x00\x00\x00\x00\x00\x00\x00\x00\x00\x43\xca\x00\x00\x00\x0a\x00\x06\xcc\xec\xbb\xfa\xcf\xdf\x00\x1d\xbf\xc9\xd7\xb0\xb1\xb8\x53\xbc\xb6\xc8\xfc\xb3\xb5\xcc\xec\xbb\xfa\xcf\xdf\xbd\xf8\xd0\xd0\xb4\xf3\xb4\xb3\xb9\xd8\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5e\x00\x00\x00\xd6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbc\xaf\xc6\xf8\xd4\xf6\xc7\xbf\x00\x1c\xc3\xbf\xb4\xce\xbd\xf8\xc8\xeb\xc6\xaf\xd2\xc6\xa3\xac\xc1\xa2\xbf\xcc\xbb\xf1\xb5\xc3\x38\x25\xbc\xaf\xc6\xf8\x01\x00\x00\x00\x00\x00\xd5\x00\x03\x00\x00\x00\x00\xd7\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x58\x00\x00\x00\xd9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\x77\x63\x77\xbc\xab\xcb\xd9\x00\x17\xca\xb9\xd3\xc3\xb5\xaa\xc6\xf8\xbc\xb4\xbf\xc9\xb4\xef\xb5\xbd\x77\x63\x77\xcb\xd9\xb6\xc8\x01\x00\x00\x00\x00\x00\xd8\x00\x03\x00\x00\x00\x00\xda\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x74\x00\x00\x00\xdc\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xb5\xaa\xc6\xf8\xc7\xbf\xbb\xaf\x00\x32\xbb\xf1\xb5\xc3\xb5\xaa\xc6\xf8\xba\xf3\xa3\xac\x32\x35\x25\xb5\xc4\xb8\xc5\xc2\xca\xb5\xaa\xc6\xf8\xd7\xee\xb8\xdf\xcb\xd9\xb6\xc8\x2b\x32\x2e\x35\x6b\x6d\x2f\x68\xa3\xac\xb3\xd6\xd0\xf8\x36\xc3\xeb\x01\x00\x00\x00\x00\x00\xdb\x00\x03\x00\x00\x00\x00\xdd\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x02\x01\x00\x12\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x05\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x09\x00\x00\x00\x0a\x00\x00\x00\x0b\x00\x00\x00\x0d\x00\x00\x00\x0e\x00\x00\x00\x0f\x00\x00\x00\x12\x00\x00\x00\x13\x00\x00\x00\x11\x00\x00\x00\x16\x00\x00\x00\x17\x00\x00\x00\x18\x02\xa8\x00\x28\x00\x13\x00\x00\x00\x0a\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x13\x00\x00\x00\x0b\x03\x00\x00\x00\x05\x00\x00\x00\x06\x00\x00\x00\x07\x00\x13\x00\x00\x00\x0c\x03\x00\x00\x00\x09\x00\x00\x00\x0a\x00\x00\x00\x0b\x00\x1f\x00\x00\x00\x0d\x06\x00\x00\x00\x0d\x00\x00\x00\x0e\x00\x00\x00\x0f\x00\x00\x00\x11\x00\x00\x00\x12\x00\x00\x00\x13\x00\x13\x00\x00\x00\x0e\x03\x00\x00\x00\x16\x00\x00\x00\x17\x00\x00\x00\x18\x00\x13\x00\x00\x00\x19\x03\x00\x00\x00\x19\x00\x00\x00\x1a\x00\x00\x00\x1b\x00\x1f\x00\x00\x00\x1a\x06\x00\x00\x00\x1d\x00\x00\x00\x1e\x00\x00\x00\x1f\x00\x00\x00\x21\x00\x00\x00\x22\x00\x00\x00\x23\x00\x13\x00\x00\x00\x1b\x03\x00\x00\x00\x25\x00\x00\x00\x26\x00\x00\x00\x27\x00\x13\x00\x00\x00\x1c\x03\x00\x00\x00\x2a\x00\x00\x00\x2b\x00\x00\x00\x2c\x00\x13\x00\x00\x00\x1d\x03\x00\x00\x00\x2e\x00\x00\x00\x2f\x00\x00\x00\x30\x00\x07\x00\x00\x00\x28\x00\x00\x1f\x00\x00\x00\x29\x06\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x34\x00\x00\x00\x37\x00\x00\x00\x38\x00\x00\x00\x39\x00\x13\x00\x00\x00\x2a\x03\x00\x00\x00\x3a\x00\x00\x00\x3b\x00\x00\x00\x3c\x00\x13\x00\x00\x00\x2b\x03\x00\x00\x00\x3e\x00\x00\x00\x3f\x00\x00\x00\x40\x00\x13\x00\x00\x00\x2c\x03\x00\x00\x00\x43\x00\x00\x00\x44\x00\x00\x00\x45\x00\x07\x00\x00\x00\x37\x00\x00\x13\x00\x00\x00\x38\x03\x00\x00\x00\x46\x00\x00\x00\x47\x00\x00\x00\x48\x00\x13\x00\x00\x00\x39\x03\x00\x00\x00\x49\x00\x00\x00\x4a\x00\x00\x00\x4b\x00\x13\x00\x00\x00\x3a\x03\x00\x00\x00\x4c\x00\x00\x00\x4d\x00\x00\x00\x4e\x00\x13\x00\x00\x00\x3b\x03\x00\x00\x00\x4f\x00\x00\x00\x50\x00\x00\x00\x51\x00\x07\x00\x00\x00\x46\x00\x00\x13\x00\x00\x00\x47\x03\x00\x00\x00\x5e\x00\x00\x00\x5f\x00\x00\x00\x60\x00\x13\x00\x00\x00\x48\x03\x00\x00\x00\x61\x00\x00\x00\x62\x00\x00\x00\x63\x00\x13\x00\x00\x00\x49\x03\x00\x00\x00\x64\x00\x00\x00\x65\x00\x00\x00\x66\x00\x13\x00\x00\x00\x4a\x03\x00\x00\x00\x67\x00\x00\x00\x68\x00\x00\x00\x69\x00\x13\x00\x00\x00\x55\x03\x00\x00\x00\x6a\x00\x00\x00\x6b\x00\x00\x00\x6c\x00\x07\x00\x00\x00\x56\x00\x00\x13\x00\x00\x00\x57\x03\x00\x00\x00\x6d\x00\x00\x00\x6e\x00\x00\x00\x6f\x00\x13\x00\x00\x00\x58\x03\x00\x00\x00\x70\x00\x00\x00\x71\x00\x00\x00\x72\x00\x13\x00\x00\x00\x59\x03\x00\x00\x00\x73\x00\x00\x00\x74\x00\x00\x00\x75\x00\x07\x00\x00\x00\x64\x00\x00\x13\x00\x00\x00\x65\x03\x00\x00\x00\x82\x00\x00\x00\x83\x00\x00\x00\x84\x00\x13\x00\x00\x00\x66\x03\x00\x00\x00\x85\x00\x00\x00\x86\x00\x00\x00\x87\x00\x07\x00\x00\x00\x67\x00\x00\x07\x00\x00\x00\x68\x00\x00\x0f\x00\x00\x00\x73\x02\x00\x00\x00\x88\x00\x00\x00\x91\x00\x0f\x00\x00\x00\x74\x02\x00\x00\x00\x92\x00\x00\x00\x93\x00\x07\x00\x00\x00\x75\x00\x00\x0f\x00\x00\x00\x76\x02\x00\x00\x00\x8f\x00\x00\x00\x90\x00\x07\x00\x00\x00\x77\x00\x09\x00\x00";
		SendToClient(Client, 24258, (BYTE*)buf, 6682, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Client->MapID == 432)
	{
		const char* buf = "\x00\x00\x00\x00\x00\x00\x00\x0b\x00\x00\x00\x96\x00\x00\x00\x78\x00\x00\x00\x78\x00\x00\x27\x66\x00\x00\x00\x01\x00\x00\x00\x0f\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x96\x00\x14\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x05\x01\x00\x00\x00\x36\x00\x00\x18\x00\x00\x00\x0a\x00\x00\x00\x02\x00\x00\x00\x0a\x02\x00\x00\x00\x64\x00\x00\x05\x4f\x00\x00\x14\x00\x00\x00\x0f\x00\x00\x00\x02\x00\x00\x00\x0f\x01\x00\x00\x00\x81\x00\x00\x14\x00\x00\x00\x14\x00\x00\x00\x02\x00\x00\x00\x14\x01\x00\x00\x00\xbd\x00\x00\x18\x00\x00\x00\x19\x00\x00\x00\x07\x00\x00\x00\x19\x02\x00\x00\x00\xf8\x00\x00\x05\x94\x00\x00\x14\x00\x00\x00\x1e\x00\x00\x00\x08\x00\x00\x00\x1e\x01\x00\x00\x01\x39\x00\x00\x18\x00\x00\x00\x23\x00\x00\x00\x05\x00\x00\x00\x23\x02\x00\x00\x01\x58\x00\x00\x05\xa3\x00\x00\x14\x00\x00\x00\x28\x00\x00\x00\x05\x00\x00\x00\x28\x01\x00\x00\x01\x82\x00\x00\x14\x00\x00\x00\x2d\x00\x00\x00\x05\x00\x00\x00\x2d\x01\x00\x00\x01\xac\x00\x00\x18\x00\x00\x00\x32\x00\x00\x00\x05\x00\x00\x00\x32\x02\x00\x00\x01\xe0\x00\x00\x05\xe3\x00\x00\x14\x00\x00\x00\x37\x00\x00\x00\x05\x00\x00\x00\x37\x01\x00\x00\x02\x1c\x00\x00\x14\x00\x00\x00\x3c\x00\x00\x00\x05\x00\x00\x00\x3c\x01\x00\x00\x02\x4c\x00\x00\x18\x00\x00\x00\x41\x00\x00\x00\x05\x00\x00\x00\x41\x02\x00\x00\x02\x7b\x00\x00\x06\x1c\x00\x00\x14\x00\x00\x00\x46\x00\x00\x00\x05\x00\x00\x00\x46\x01\x00\x00\x02\xb7\x00\x00\x14\x00\x00\x00\x4b\x00\x00\x00\x05\x00\x00\x00\x4b\x01\x00\x00\x02\xd8\x00\x00\x14\x00\x00\x00\x50\x00\x00\x00\x03\x00\x00\x00\x50\x01\x00\x00\x03\x05\x00\x00\x14\x00\x00\x00\x55\x00\x00\x00\x03\x00\x00\x00\x55\x01\x00\x00\x03\x2e\x00\x00\x18\x00\x00\x00\x5a\x00\x00\x00\x04\x00\x00\x00\x5a\x02\x00\x00\x03\x5f\x00\x00\x06\x6e\x00\x00\x14\x00\x00\x00\x5f\x00\x00\x00\x04\x00\x00\x00\x5f\x01\x00\x00\x03\x97\x00\x00\x18\x00\x00\x00\x64\x00\x00\x00\x04\x00\x00\x00\x64\x02\x00\x00\x03\xbc\x00\x00\x06\x83\x00\x00\x14\x00\x00\x00\x69\x00\x00\x00\x04\x00\x00\x00\x69\x01\x00\x00\x03\xd5\x00\x00\x14\x00\x00\x00\x6e\x00\x00\x00\x04\x00\x00\x00\x6e\x01\x00\x00\x03\xf4\x00\x00\x18\x00\x00\x00\x73\x00\x00\x00\x04\x00\x00\x00\x73\x02\x00\x00\x04\x1f\x00\x00\x06\xa1\x00\x00\x14\x00\x00\x00\x78\x00\x00\x00\x04\x00\x00\x00\x78\x01\x00\x00\x04\x47\x00\x00\x14\x00\x00\x00\x7d\x00\x00\x00\x04\x00\x00\x00\x7d\x01\x00\x00\x04\x78\x00\x00\x18\x00\x00\x00\x82\x00\x00\x00\x04\x00\x00\x00\x82\x02\x00\x00\x04\x9f\x00\x00\x06\xf3\x00\x00\x14\x00\x00\x00\x87\x00\x00\x00\x05\x00\x00\x00\x87\x01\x00\x00\x04\xc5\x00\x00\x14\x00\x00\x00\x8c\x00\x00\x00\x05\x00\x00\x00\x8c\x01\x00\x00\x04\xf2\x00\x00\x14\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x05\x00\x00\x14\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x0c\x00\x00\x14\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x1b\x00\x00\x14\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x2d\x00\x00\x14\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x3f\x00\x00\x18\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x47\x00\x00\x05\x32\x00\x00\x18\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x50\x00\x00\x05\x3c\x00\x00\x18\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x5c\x00\x00\x05\x48\x00\x00\x18\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x6f\x00\x00\x05\x57\x00\x00\x18\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x72\x00\x00\x05\x5b\x00\x00\x14\x00\x00\x00\x0d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x76\x00\x00\x14\x00\x00\x00\x0e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x7a\x00\x00\x14\x00\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x8e\x00\x00\x14\x00\x00\x00\x11\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x98\x00\x00\x14\x00\x00\x00\x12\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xa2\x00\x00\x14\x00\x00\x00\x13\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xb2\x00\x00\x14\x00\x00\x00\x15\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xc1\x00\x00\x18\x00\x00\x00\x16\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xcb\x00\x00\x05\x67\x00\x00\x18\x00\x00\x00\x17\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xd6\x00\x00\x05\x72\x00\x00\x18\x00\x00\x00\x18\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\xe6\x00\x00\x05\x83\x00\x00\x14\x00\x00\x00\x1a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x04\x00\x00\x14\x00\x00\x00\x1b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x0b\x00\x00\x14\x00\x00\x00\x1c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x1c\x00\x00\x14\x00\x00\x00\x1d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x25\x00\x00\x14\x00\x00\x00\x1f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x3e\x00\x00\x14\x00\x00\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x47\x00\x00\x14\x00\x00\x00\x21\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x4e\x00\x00\x18\x00\x00\x00\x22\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x55\x00\x00\x05\xa0\x00\x00\x18\x00\x00\x00\x24\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x63\x00\x00\x05\xb0\x00\x00\x18\x00\x00\x00\x25\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x6a\x00\x00\x05\xb8\x00\x00\x18\x00\x00\x00\x26\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x77\x00\x00\x05\xc4\x00\x00\x18\x00\x00\x00\x27\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x7d\x00\x00\x05\xcc\x00\x00\x14\x00\x00\x00\x29\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x89\x00\x00\x14\x00\x00\x00\x2a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x93\x00\x00\x14\x00\x00\x00\x2b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xa1\x00\x00\x14\x00\x00\x00\x2c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xa8\x00\x00\x14\x00\x00\x00\x2e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xb7\x00\x00\x14\x00\x00\x00\x2f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xc5\x00\x00\x18\x00\x00\x00\x30\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xcc\x00\x00\x05\xd0\x00\x00\x18\x00\x00\x00\x31\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xd2\x00\x00\x05\xd6\x00\x00\x18\x00\x00\x00\x33\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xea\x00\x00\x05\xec\x00\x00\x18\x00\x00\x00\x34\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\xfd\x00\x00\x06\x00\x00\x00\x14\x00\x00\x00\x35\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x06\x00\x00\x14\x00\x00\x00\x36\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x0f\x00\x00\x14\x00\x00\x00\x38\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x2c\x00\x00\x14\x00\x00\x00\x39\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x33\x00\x00\x14\x00\x00\x00\x3a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x41\x00\x00\x14\x00\x00\x00\x3b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x46\x00\x00\x14\x00\x00\x00\x3d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x53\x00\x00\x14\x00\x00\x00\x3e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x5c\x00\x00\x18\x00\x00\x00\x3f\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x68\x00\x00\x06\x0b\x00\x00\x18\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x71\x00\x00\x06\x12\x00\x00\x18\x00\x00\x00\x42\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\x80\x00\x00\x06\x21\x00\x00\x14\x00\x00\x00\x43\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x89\x00\x00\x14\x00\x00\x00\x44\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xa0\x00\x00\x14\x00\x00\x00\x45\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xb2\x00\x00\x14\x00\x00\x00\x47\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xbc\x00\x00\x14\x00\x00\x00\x48\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xc3\x00\x00\x14\x00\x00\x00\x49\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xcb\x00\x00\x14\x00\x00\x00\x4a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xd5\x00\x00\x14\x00\x00\x00\x4c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xdb\x00\x00\x18\x00\x00\x00\x4d\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xe0\x00\x00\x06\x2d\x00\x00\x18\x00\x00\x00\x4e\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xee\x00\x00\x06\x3c\x00\x00\x18\x00\x00\x00\x4f\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x02\xfd\x00\x00\x06\x4c\x00\x00\x14\x00\x00\x00\x51\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x0b\x00\x00\x14\x00\x00\x00\x52\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x15\x00\x00\x14\x00\x00\x00\x53\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x1b\x00\x00\x14\x00\x00\x00\x54\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x23\x00\x00\x14\x00\x00\x00\x56\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x3f\x00\x00\x14\x00\x00\x00\x57\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x43\x00\x00\x18\x00\x00\x00\x58\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x49\x00\x00\x06\x57\x00\x00\x18\x00\x00\x00\x59\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x52\x00\x00\x06\x5f\x00\x00\x18\x00\x00\x00\x5b\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x68\x00\x00\x06\x78\x00\x00\x18\x00\x00\x00\x5c\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\x6b\x00\x00\x06\x7a\x00\x00\x14\x00\x00\x00\x5d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x71\x00\x00\x14\x00\x00\x00\x5e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x82\x00\x00\x14\x00\x00\x00\x60\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x9d\x00\x00\x14\x00\x00\x00\x61\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xac\x00\x00\x14\x00\x00\x00\x62\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xb0\x00\x00\x18\x00\x00\x00\x63\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xb6\x00\x00\x06\x80\x00\x00\x18\x00\x00\x00\x65\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xbd\x00\x00\x06\x87\x00\x00\x18\x00\x00\x00\x66\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xc8\x00\x00\x06\x92\x00\x00\x18\x00\x00\x00\x67\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xcd\x00\x00\x06\x97\x00\x00\x18\x00\x00\x00\x68\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x03\xd1\x00\x00\x06\x9d\x00\x00\x14\x00\x00\x00\x6a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xd9\x00\x00\x14\x00\x00\x00\x6b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xdc\x00\x00\x14\x00\x00\x00\x6c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xe5\x00\x00\x14\x00\x00\x00\x6d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xec\x00\x00\x14\x00\x00\x00\x6f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xf9\x00\x00\x14\x00\x00\x00\x70\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x04\x00\x00\x14\x00\x00\x00\x71\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x10\x00\x00\x14\x00\x00\x00\x72\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x19\x00\x00\x18\x00\x00\x00\x74\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\x29\x00\x00\x06\xaa\x00\x00\x18\x00\x00\x00\x75\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\x30\x00\x00\x06\xaf\x00\x00\x18\x00\x00\x00\x76\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\x3a\x00\x00\x06\xbd\x00\x00\x18\x00\x00\x00\x77\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\x43\x00\x00\x06\xc9\x00\x00\x14\x00\x00\x00\x79\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x52\x00\x00\x14\x00\x00\x00\x7a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x5a\x00\x00\x14\x00\x00\x00\x7b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x67\x00\x00\x14\x00\x00\x00\x7c\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x70\x00\x00\x14\x00\x00\x00\x7e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x80\x00\x00\x14\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x82\x00\x00\x18\x00\x00\x00\x80\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\x86\x00\x00\x06\xdc\x00\x00\x18\x00\x00\x00\x81\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\x97\x00\x00\x06\xeb\x00\x00\x18\x00\x00\x00\x83\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\xa5\x00\x00\x06\xf7\x00\x00\x18\x00\x00\x00\x84\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x04\xae\x00\x00\x07\x02\x00\x00\x14\x00\x00\x00\x85\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xb5\x00\x00\x14\x00\x00\x00\x86\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xb9\x00\x00\x14\x00\x00\x00\x88\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xce\x00\x00\x14\x00\x00\x00\x89\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xd9\x00\x00\x14\x00\x00\x00\x8a\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xe0\x00\x00\x14\x00\x00\x00\x8b\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xe7\x00\x00\x14\x00\x00\x00\x8d\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xf7\x00\x00\x14\x00\x00\x00\x8e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xfb\x00\x00\x14\x00\x00\x00\x8f\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xfc\x00\x00\x14\x00\x00\x00\x90\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\xfd\x00\x00\x18\x00\x00\x00\x91\x00\x00\x00\x00\x00\x00\x00\x91\x02\x00\x00\x05\x01\x00\x00\x07\x0b\x00\x00\x18\x00\x00\x00\x92\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x05\x0a\x00\x00\x07\x15\x00\x00\x18\x00\x00\x00\x93\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x05\x12\x00\x00\x07\x1e\x00\x00\x18\x00\x00\x00\x94\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x05\x20\x00\x00\x07\x2c\x00\x00\x18\x00\x00\x00\x95\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x05\x29\x00\x00\x07\x35\x00\x00\x14\x00\x00\x00\x96\x00\x00\x00\x00\x00\x00\x00\x96\x01\x00\x00\x05\x2e\x00\x01\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\xc2\x20\x43\x32\x34\x45\x38\x42\x34\x46\x37\x34\x34\x31\x42\x43\x43\x39\x43\x44\x34\x42\x38\x36\x38\x33\x32\x46\x45\x31\x30\x41\x42\x41\x00\x42\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x70\x75\x74\x6f\x6e\x67\x5f\x6c\x6f\x77\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3a\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xc6\xd5\xcd\xa8\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\xc3\x20\x35\x37\x33\x39\x31\x39\x41\x41\x42\x38\x41\x36\x32\x45\x43\x36\x37\x39\x36\x37\x44\x35\x38\x37\x41\x42\x31\x34\x39\x34\x36\x39\x00\x43\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x70\x75\x74\x6f\x6e\x67\x5f\x68\x69\x67\x68\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3a\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xc6\xd5\xcd\xa8\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\xc3\x20\x32\x33\x30\x36\x39\x35\x42\x34\x32\x44\x38\x34\x32\x45\x43\x33\x36\x42\x38\x36\x35\x37\x43\x32\x32\x34\x43\x34\x42\x43\x31\x46\x00\x43\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x63\x68\x65\x73\x68\x65\x6e\x5f\x6c\x6f\x77\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3a\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xb3\xb5\xc9\xf1\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\xc4\x20\x43\x33\x41\x32\x38\x44\x45\x38\x37\x30\x36\x38\x30\x30\x45\x46\x38\x31\x45\x36\x38\x33\x46\x39\x30\x34\x31\x34\x31\x42\x34\x46\x00\x44\x68\x74\x74\x70\x3a\x2f\x2f\x64\x6f\x77\x6e\x2e\x71\x71\x2e\x63\x6f\x6d\x2f\x71\x71\x6b\x61\x72\x74\x2f\x66\x75\x6c\x6c\x2f\x54\x69\x6d\x65\x72\x43\x68\x61\x6c\x6c\x65\x6e\x67\x65\x52\x65\x63\x6f\x72\x64\x5f\x63\x68\x65\x73\x68\x65\x6e\x5f\x68\x69\x67\x68\x2e\x73\x70\x64\x01\x00\x46\x00\x00\x27\x26\x00\x00\x27\x29\x00\x00\x27\x7a\x00\x00\x00\x00\x00\x00\x27\x3b\x00\x00\x27\x3d\x00\x00\x27\x3c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xb3\xb5\xc9\xf1\xa1\xa4\xbc\xab\xcf\xde\xcc\xf4\xd5\xbd\xd5\xdf\x00\x00\x00\x00\x00\x00\x0b\x00\x50\x0f\x01\x00\x06\x00\x00\x00\x00\x03\x03\x53\x04\x00\x89\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\x00\x47\xbc\xab\xd0\xc7\x51\x73\xc4\xdc\xc1\xbf\xb8\xb3\xd3\xe8\xc8\xfc\xb3\xb5\xb7\xc9\xd0\xd0\xb5\xc4\xc4\xdc\xc1\xa6\xa1\xa3\x0a\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x3a\x98\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5d\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\x4c\x76\x32\x00\x18\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\xb5\xc4\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x2d\x35\xc3\xeb\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5d\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\x4c\x76\x33\x00\x18\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\xb5\xc4\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x2b\x32\xc3\xeb\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x1b\x58\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\x4c\x76\x34\x00\x16\xd0\xc7\xba\xa3\xb7\xc9\xd0\xd0\xb5\xc4\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x2b\x31\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x1b\x58\x00\x00\x27\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x5e\x00\x00\x01\x41\x00\x00\x00\x00\x00\x00\x00\x00\x01\xe7\x0d\xff\xff\xff\xff\x00\x04\xd0\xc7\xbb\xcd\x00\x20\xbf\xc9\xd7\xb0\xb1\xb8\xc2\xfa\xb8\xc4\x41\xbc\xb6\xc8\xfc\xb3\xb5\x2d\xd0\xc7\xbb\xcd\xbd\xf8\xd0\xd0\xb4\xf3\xb4\xb3\xb9\xd8\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6c\x00\x00\x00\xc1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc5\xf6\xd7\xb2\xbc\xaf\xc6\xf8\x00\x2a\xb7\xc7\xc6\xaf\xd2\xc6\xd7\xb4\xcc\xac\xcf\xc2\xa3\xac\xc3\xbf\xb4\xce\xd7\xb2\xc7\xbd\xa3\xac\x36\x30\x25\xb8\xc5\xc2\xca\xbb\xf1\xb5\xc3\x39\x30\x25\xbc\xaf\xc6\xf8\x01\x00\x00\x00\x00\x00\xc1\x00\x03\x00\x00\x00\x00\xc1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\xc5\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc6\xf0\xb2\xbd\xb5\xaa\xc6\xf8\x00\x3e\xc6\xf0\xb2\xbd\xc1\xa2\xbf\xcc\xbb\xf1\xb5\xc3\xd2\xbb\xb8\xf6\xb5\xaa\xc6\xf8\xa3\xac\xc7\xd2\x31\x30\xc3\xeb\xc4\xda\xb5\xaa\xc6\xf8\xd7\xee\xb8\xdf\xcb\xd9\xb6\xc8\x2b\x32\x2e\x35\x6b\x6d\x2f\x68\xa3\xac\xb5\xaa\xc6\xf8\xb6\xaf\xc1\xa6\x2b\x35\x01\x00\x00\x00\x00\x00\xc4\x00\x03\x00\x00\x00\x00\xc6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x67\x00\x00\x00\xc7\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xb6\xaf\xc1\xa6\xcc\xe1\xc9\xfd\x00\x25\xc3\xbf\xb4\xce\xbd\xf8\xc8\xeb\xc6\xaf\xd2\xc6\xa3\xac\x36\x30\x25\xb8\xc5\xc2\xca\x38\xc3\xeb\xc4\xda\xb5\xaa\xc6\xf8\xb6\xaf\xc1\xa6\x2b\x31\x35\x01\x00\x00\x00\x00\x00\xc7\x00\x03\x00\x00\x00\x00\xc7\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x7f\x04\x00\xa1\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbb\xc3\xd3\xf2\xc9\xb3\xc2\xa9\x00\x5f\xbc\xab\xd0\xc7\x51\x73\xc4\xdc\xc1\xbf\xca\xb9\xc8\xfc\xb5\xc0\xc9\xcf\xb5\xc4\xc3\xc5\xb1\xe4\xce\xaa\xc7\xbf\xbb\xaf\xc3\xc5\xa1\xa3\xcd\xa8\xb9\xfd\xba\xf3\xb4\xb3\xb9\xd8\xca\xb1\xbc\xe4\x2b\x32\xc3\xeb\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x32\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x3a\x98\x00\x00\x61\xa8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5d\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xbb\xc3\xd3\xf2\xc9\xb3\xc2\xa9\x4c\x76\x32\x00\x18\xbb\xc3\xd3\xf2\xc9\xb3\xc2\xa9\xb5\xc4\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x2b\x35\xc3\xeb\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x4e\x20\x00\x00\x61\xa8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x79\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xbb\xc3\xd3\xf2\xc9\xb3\xc2\xa9\x4c\x76\x33\x00\x34\xb4\xa9\xb9\xfd\xcc\xd8\xca\xe2\xb5\xc4\xc7\xbf\xbb\xaf\xc3\xc5\xba\xf3\xa3\xac\xbd\xab\xbb\xe1\xc1\xa2\xbf\xcc\xbb\xf1\xb5\xc3\xd2\xbb\xb8\xf6\xb6\xcc\xd4\xdd\xb5\xc4\xb5\xaf\xc9\xe4\xbc\xd3\xcb\xd9\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x4e\x20\x00\x00\x61\xa8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xbb\xc3\xd3\xf2\xc9\xb3\xc2\xa9\x4c\x76\x34\x00\x16\xbb\xc3\xd3\xf2\xc9\xb3\xc2\xa9\xb5\xc4\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x2b\x31\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x4e\x20\x00\x00\x61\xa8\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x6d\x00\x00\x01\x45\x00\x00\x00\x00\x00\x00\x00\x00\x01\xed\x89\x00\x00\x00\x05\x00\x0b\xd4\xb4\xbc\xab\xd6\xae\xd0\xc7\x45\x58\x41\x00\x28\xbf\xc9\xd7\xb0\xb1\xb8\xc2\xfa\xb8\xc4\x54\x32\xbc\xb6\xc8\xfc\xb3\xb5\x2d\xd4\xb4\xbc\xab\xd6\xae\xd0\xc7\x45\x58\x41\xbd\xf8\xd0\xd0\xb4\xf3\xb4\xb3\xb9\xd8\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5c\x00\x00\x00\xcb\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xd6\xfa\xc1\xa6\xc6\xf0\xc5\xdc\x00\x1a\xc3\xbf\xb0\xf4\xc6\xf0\xc5\xdc\xca\xb1\xa3\xac\xb4\xb3\xb9\xd8\xca\xb1\xbc\xe4\x2b\x31\x2e\x35\xc3\xeb\x01\x00\x00\x00\x00\x00\xcb\x00\x03\x00\x00\x00\x00\xcb\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7b\x00\x00\x00\xce\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc5\xe7\xc9\xe4\xbc\xd3\xb3\xc9\x00\x39\xca\xb9\xd3\xc3\xcb\xab\xc5\xe7\xa3\xa8\xba\xac\x63\x77\x77\xa1\xa2\x77\x63\x77\xa3\xa9\xba\xf3\xa3\xac\xb4\xb3\xb9\xd8\xca\xb1\xbc\xe4\x2b\x31\x2e\x35\xc3\xeb\xa3\xac\xa3\xa8\xc3\xbf\xb0\xf4\xc9\xcf\xcf\xde\x32\xb4\xce\xa3\xa9\x01\x00\x00\x00\x00\x00\xce\x00\x03\x00\x00\x00\x00\xce\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x65\x00\x00\x00\xd1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbc\xab\xcb\xd9\xcc\xe1\xc9\xfd\x00\x23\xc3\xbf\x33\xb4\xce\xca\xb9\xd3\xc3\xb5\xaa\xc6\xf8\xa3\xac\x38\xc3\xeb\xc4\xda\xc6\xbd\xc5\xdc\xbc\xab\xcb\xd9\xc4\xdc\xc1\xa6\x2b\x31\x32\x01\x00\x00\x00\x00\x00\xd1\x00\x03\x00\x00\x00\x00\xd1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x6c\x04\x00\xa0\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\x00\x5e\xc0\xa9\xb4\xf3\xb5\xc0\xbe\xdf\xca\xb0\xc8\xa1\xb7\xb6\xce\xa7\xa3\xac\xb2\xa2\xd4\xda\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\xc4\xda\xc3\xbf\xc3\xeb\xd6\xf0\xbd\xa5\xd4\xf6\xbc\xd3\x34\x30\x25\xb5\xaa\xc6\xf8\xa1\xa3\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x32\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x3a\x98\x00\x00\x61\xa8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5d\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\x4c\x76\x32\x00\x18\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\xb5\xc4\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x2b\x35\xc3\xeb\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x4e\x20\x00\x00\x61\xa8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\x4c\x76\x33\x00\x3b\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\xb5\xc4\xbc\xbc\xc4\xdc\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\xc4\xda\xa3\xac\xb5\xaa\xc6\xf8\xbd\xab\xbf\xc9\xd2\xd4\xb5\xfe\xbc\xd3\xca\xcd\xb7\xc5\xa3\xac\xd7\xee\xb6\xe0\xb5\xfe\xbc\xd3\x33\xb2\xe3\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x4e\x20\x00\x00\x61\xa8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5b\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\x4c\x76\x34\x00\x16\xbc\xab\xd0\xc7\xb4\xc5\xce\xfc\xb5\xc4\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x2b\x31\xa1\xa3\x01\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x4e\x20\x00\x00\x61\xa8\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x66\x00\x00\x01\x49\x00\x00\x00\x00\x00\x00\x00\x00\x01\xed\x8b\x00\x00\x00\x05\x00\x08\xbc\xab\xd0\xc7\xd6\xae\xd2\xed\x00\x24\xbf\xc9\xd7\xb0\xb1\xb8\xc2\xfa\xb8\xc4\x53\xbc\xb6\xc8\xfc\xb3\xb5\x2d\xbc\xab\xd0\xc7\xd6\xae\xd2\xed\xbd\xf8\xd0\xd0\xb4\xf3\xb4\xb3\xb9\xd8\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5f\x00\x00\x00\xd5\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbc\xaf\xc6\xf8\xd4\xf6\xc7\xbf\x00\x1d\xc3\xbf\xb4\xce\xbd\xf8\xc8\xeb\xc6\xaf\xd2\xc6\xa3\xac\xc1\xa2\xbf\xcc\xbb\xf1\xb5\xc3\x31\x32\x25\xbc\xaf\xc6\xf8\x01\x00\x00\x00\x00\x00\xd5\x00\x03\x00\x00\x00\x00\xd5\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x59\x00\x00\x00\xd9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xbc\xab\xcb\xd9\xc7\xbf\xbb\xaf\x00\x17\xca\xb9\xd3\xc3\xb5\xaa\xc6\xf8\xbc\xb4\xbf\xc9\xb4\xef\xb5\xbd\x77\x63\x77\xcb\xd9\xb6\xc8\x01\x00\x00\x00\x00\x00\xd8\x00\x03\x00\x00\x00\x00\xda\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x72\x00\x00\x00\xdb\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xb5\xaa\xc6\xf8\xc7\xbf\xbb\xaf\x00\x30\xbb\xf1\xb5\xc3\xb5\xaa\xc6\xf8\xba\xf3\xa3\xac\x34\x30\x25\xb5\xc4\xb8\xc5\xc2\xca\xb5\xaa\xc6\xf8\xd7\xee\xb8\xdf\xcb\xd9\xb6\xc8\x2b\x33\x6b\x6d\x2f\x68\xa3\xac\xb3\xd6\xd0\xf8\x36\xc3\xeb\x01\x00\x00\x00\x00\x00\xdb\x00\x03\x00\x00\x00\x00\xdb\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x03\x03\x0b\x00\x31\x00\x13\x00\x00\x00\x07\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x13\x00\x00\x00\x08\x03\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00\x06\x00\x13\x00\x00\x00\x09\x03\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00\x09\x00\x13\x00\x00\x00\x0a\x03\x00\x00\x00\x0b\x00\x00\x00\x0c\x00\x00\x00\x0d\x00\x13\x00\x00\x00\x0b\x03\x00\x00\x00\x0f\x00\x00\x00\x10\x00\x00\x00\x11\x00\x13\x00\x00\x00\x0c\x03\x00\x00\x00\x13\x00\x00\x00\x14\x00\x00\x00\x15\x00\x13\x00\x00\x00\x16\x03\x00\x00\x00\x16\x00\x00\x00\x17\x00\x00\x00\x18\x00\x13\x00\x00\x00\x17\x03\x00\x00\x00\x19\x00\x00\x00\x1a\x00\x00\x00\x1b\x00\x13\x00\x00\x00\x18\x03\x00\x00\x00\x1d\x00\x00\x00\x1e\x00\x00\x00\x1f\x00\x13\x00\x00\x00\x19\x03\x00\x00\x00\x21\x00\x00\x00\x22\x00\x00\x00\x23\x00\x07\x00\x00\x00\x22\x00\x00\x13\x00\x00\x00\x23\x03\x00\x00\x00\x24\x00\x00\x00\x25\x00\x00\x00\x26\x00\x13\x00\x00\x00\x24\x03\x00\x00\x00\x27\x00\x00\x00\x28\x00\x00\x00\x29\x00\x13\x00\x00\x00\x25\x03\x00\x00\x00\x2b\x00\x00\x00\x2c\x00\x00\x00\x2d\x00\x13\x00\x00\x00\x26\x03\x00\x00\x00\x2f\x00\x00\x00\x30\x00\x00\x00\x31\x00\x07\x00\x00\x00\x27\x00\x00\x07\x00\x00\x00\x30\x00\x00\x13\x00\x00\x00\x31\x03\x00\x00\x00\x33\x00\x00\x00\x34\x00\x00\x00\x35\x00\x13\x00\x00\x00\x32\x03\x00\x00\x00\x36\x00\x00\x00\x37\x00\x00\x00\x38\x00\x13\x00\x00\x00\x33\x03\x00\x00\x00\x3a\x00\x00\x00\x3b\x00\x00\x00\x3c\x00\x13\x00\x00\x00\x34\x03\x00\x00\x00\x3e\x00\x00\x00\x3f\x00\x00\x00\x40\x00\x07\x00\x00\x00\x3f\x00\x00\x13\x00\x00\x00\x40\x03\x00\x00\x00\x42\x00\x00\x00\x43\x00\x00\x00\x44\x00\x13\x00\x00\x00\x41\x03\x00\x00\x00\x46\x00\x00\x00\x47\x00\x00\x00\x48\x00\x13\x00\x00\x00\x42\x03\x00\x00\x00\x4a\x00\x00\x00\x4b\x00\x00\x00\x4c\x00\x13\x00\x00\x00\x4d\x03\x00\x00\x00\x4e\x00\x00\x00\x4f\x00\x00\x00\x50\x00\x13\x00\x00\x00\x4e\x03\x00\x00\x00\x52\x00\x00\x00\x53\x00\x00\x00\x54\x00\x13\x00\x00\x00\x4f\x03\x00\x00\x00\x56\x00\x00\x00\x57\x00\x00\x00\x58\x00\x07\x00\x00\x00\x58\x00\x00\x13\x00\x00\x00\x59\x03\x00\x00\x00\x5a\x00\x00\x00\x5b\x00\x00\x00\x5c\x00\x13\x00\x00\x00\x5a\x03\x00\x00\x00\x5e\x00\x00\x00\x5f\x00\x00\x00\x60\x00\x13\x00\x00\x00\x5b\x03\x00\x00\x00\x62\x00\x00\x00\x63\x00\x00\x00\x64\x00\x07\x00\x00\x00\x5c\x00\x00\x07\x00\x00\x00\x63\x00\x00\x13\x00\x00\x00\x64\x03\x00\x00\x00\x66\x00\x00\x00\x67\x00\x00\x00\x68\x00\x13\x00\x00\x00\x65\x03\x00\x00\x00\x6a\x00\x00\x00\x6b\x00\x00\x00\x6c\x00\x13\x00\x00\x00\x66\x03\x00\x00\x00\x6e\x00\x00\x00\x6f\x00\x00\x00\x70\x00\x07\x00\x00\x00\x67\x00\x00\x07\x00\x00\x00\x68\x00\x00\x07\x00\x00\x00\x73\x00\x00\x13\x00\x00\x00\x74\x03\x00\x00\x00\x72\x00\x00\x00\x73\x00\x00\x00\x74\x00\x13\x00\x00\x00\x75\x03\x00\x00\x00\x76\x00\x00\x00\x77\x00\x00\x00\x78\x00\x13\x00\x00\x00\x76\x03\x00\x00\x00\x7a\x00\x00\x00\x7b\x00\x00\x00\x7c\x00\x07\x00\x00\x00\x77\x00\x00\x07\x00\x00\x00\x80\x00\x00\x13\x00\x00\x00\x81\x03\x00\x00\x00\x7e\x00\x00\x00\x7f\x00\x00\x00\x80\x00\x13\x00\x00\x00\x82\x03\x00\x00\x00\x82\x00\x00\x00\x83\x00\x00\x00\x84\x00\x13\x00\x00\x00\x83\x03\x00\x00\x00\x86\x00\x00\x00\x87\x00\x00\x00\x88\x00\x07\x00\x00\x00\x84\x00\x09\x00\x00";
		SendToClient(Client, 24258, (BYTE*)buf, 7495, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Client->MapID == 452)
	{
		const char* buf = "\x00\x00\x00\x00\x00\x00\x00\x0B\x00\x00\x00\x78\x00\x00\x00\x5A\x00\x00\x00\x78\x00\x00\x27\x66\x00\x00\x00\x01\x00\x00\x00\x0F\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x64\x00\x00\x01\x2C\x00\x00\x00\x1A\x00\x78\x00\x14\x00\x00\x00\x05\x00\x00\x00\x02\x00\x00\x00\x05\x01\x00\x00\x00\x2D\x00\x00\x14\x00\x00\x00\x0A\x00\x00\x00\x02\x00\x00\x00\x0A\x01\x00\x00\x00\x67\x00\x00\x14\x00\x00\x00\x0F\x00\x00\x00\x02\x00\x00\x00\x0F\x01\x00\x00\x00\x93\x00\x00\x14\x00\x00\x00\x14\x00\x00\x00\x02\x00\x00\x00\x14\x01\x00\x00\x00\xC5\x00\x00\x14\x00\x00\x00\x19\x00\x00\x00\x07\x00\x00\x00\x19\x01\x00\x00\x00\xF8\x00\x00\x14\x00\x00\x00\x1E\x00\x00\x00\x08\x00\x00\x00\x1E\x01\x00\x00\x01\x3E\x00\x00\x14\x00\x00\x00\x23\x00\x00\x00\x05\x00\x00\x00\x23\x01\x00\x00\x01\x75\x00\x00\x14\x00\x00\x00\x28\x00\x00\x00\x05\x00\x00\x00\x28\x01\x00\x00\x01\xA9\x00\x00\x14\x00\x00\x00\x2D\x00\x00\x00\x05\x00\x00\x00\x2D\x01\x00\x00\x01\xDF\x00\x00\x14\x00\x00\x00\x32\x00\x00\x00\x05\x00\x00\x00\x32\x01\x00\x00\x02\x0D\x00\x00\x14\x00\x00\x00\x37\x00\x00\x00\x05\x00\x00\x00\x37\x01\x00\x00\x02\x3B\x00\x00\x14\x00\x00\x00\x3C\x00\x00\x00\x05\x00\x00\x00\x3C\x01\x00\x00\x02\x68\x00\x00\x14\x00\x00\x00\x41\x00\x00\x00\x05\x00\x00\x00\x41\x01\x00\x00\x02\x92\x00\x00\x14\x00\x00\x00\x46\x00\x00\x00\x05\x00\x00\x00\x46\x01\x00\x00\x02\xBF\x00\x00\x14\x00\x00\x00\x4B\x00\x00\x00\x05\x00\x00\x00\x4B\x01\x00\x00\x02\xE9\x00\x00\x14\x00\x00\x00\x50\x00\x00\x00\x03\x00\x00\x00\x50\x01\x00\x00\x03\x0B\x00\x00\x14\x00\x00\x00\x55\x00\x00\x00\x03\x00\x00\x00\x55\x01\x00\x00\x03\x2E\x00\x00\x14\x00\x00\x00\x5A\x00\x00\x00\x04\x00\x00\x00\x5A\x01\x00\x00\x03\x55\x00\x00\x14\x00\x00\x00\x5F\x00\x00\x00\x04\x00\x00\x00\x5F\x01\x00\x00\x03\x7E\x00\x00\x14\x00\x00\x00\x64\x00\x00\x00\x04\x00\x00\x00\x64\x01\x00\x00\x03\xA0\x00\x00\x14\x00\x00\x00\x69\x00\x00\x00\x04\x00\x00\x00\x69\x01\x00\x00\x03\xCC\x00\x00\x14\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x04\x00\x00\x14\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x0B\x00\x00\x14\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x1C\x00\x00\x14\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x21\x00\x00\x14\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x34\x00\x00\x14\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x40\x00\x00\x14\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x50\x00\x00\x14\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x5E\x00\x00\x14\x00\x00\x00\x0B\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x70\x00\x00\x14\x00\x00\x00\x0C\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x76\x00\x00\x14\x00\x00\x00\x0D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x79\x00\x00\x14\x00\x00\x00\x0E\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x83\x00\x00\x14\x00\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xA0\x00\x00\x14\x00\x00\x00\x11\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xAF\x00\x00\x14\x00\x00\x00\x12\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xB9\x00\x00\x14\x00\x00\x00\x13\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xBF\x00\x00\x14\x00\x00\x00\x15\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xD7\x00\x00\x14\x00\x00\x00\x16\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xE1\x00\x00\x14\x00\x00\x00\x17\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xE5\x00\x00\x14\x00\x00\x00\x18\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xEF\x00\x00\x14\x00\x00\x00\x1A\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x02\x00\x00\x14\x00\x00\x00\x1B\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x0C\x00\x00\x14\x00\x00\x00\x1C\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x19\x00\x00\x14\x00\x00\x00\x1D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x33\x00\x00\x14\x00\x00\x00\x1F\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x46\x00\x00\x14\x00\x00\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x4F\x00\x00\x14\x00\x00\x00\x21\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x59\x00\x00\x14\x00\x00\x00\x22\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x67\x00\x00\x14\x00\x00\x00\x24\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x7F\x00\x00\x14\x00\x00\x00\x25\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x85\x00\x00\x18\x00\x00\x00\x26\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x01\x8E\x00\x00\x04\x68\x00\x00\x14\x00\x00\x00\x27\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\x99\x00\x00\x14\x00\x00\x00\x29\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xAE\x00\x00\x14\x00\x00\x00\x2A\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xBC\x00\x00\x14\x00\x00\x00\x2B\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xCA\x00\x00\x14\x00\x00\x00\x2C\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xD7\x00\x00\x14\x00\x00\x00\x2E\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xE9\x00\x00\x14\x00\x00\x00\x2F\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x01\xF3\x00\x00\x14\x00\x00\x00\x30\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x01\x00\x00\x14\x00\x00\x00\x31\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x05\x00\x00\x14\x00\x00\x00\x33\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x16\x00\x00\x14\x00\x00\x00\x34\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x1F\x00\x00\x14\x00\x00\x00\x35\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x2A\x00\x00\x14\x00\x00\x00\x36\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x34\x00\x00\x14\x00\x00\x00\x38\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x47\x00\x00\x14\x00\x00\x00\x39\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x4F\x00\x00\x14\x00\x00\x00\x3A\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x5A\x00\x00\x14\x00\x00\x00\x3B\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x60\x00\x00\x14\x00\x00\x00\x3D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x70\x00\x00\x14\x00\x00\x00\x3E\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x7B\x00\x00\x14\x00\x00\x00\x3F\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x81\x00\x00\x14\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x85\x00\x00\x14\x00\x00\x00\x42\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\x95\x00\x00\x14\x00\x00\x00\x43\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xA1\x00\x00\x14\x00\x00\x00\x44\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xA7\x00\x00\x14\x00\x00\x00\x45\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xB2\x00\x00\x14\x00\x00\x00\x47\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xC8\x00\x00\x14\x00\x00\x00\x48\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xD1\x00\x00\x14\x00\x00\x00\x49\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xD8\x00\x00\x14\x00\x00\x00\x4A\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xDE\x00\x00\x14\x00\x00\x00\x4C\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xEF\x00\x00\x14\x00\x00\x00\x4D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xF6\x00\x00\x14\x00\x00\x00\x4E\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x02\xFE\x00\x00\x14\x00\x00\x00\x4F\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x05\x00\x00\x14\x00\x00\x00\x51\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x10\x00\x00\x14\x00\x00\x00\x52\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x14\x00\x00\x14\x00\x00\x00\x53\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x1B\x00\x00\x14\x00\x00\x00\x54\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x22\x00\x00\x14\x00\x00\x00\x56\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x36\x00\x00\x14\x00\x00\x00\x57\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x3B\x00\x00\x14\x00\x00\x00\x58\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x46\x00\x00\x14\x00\x00\x00\x59\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x4C\x00\x00\x14\x00\x00\x00\x5B\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x5E\x00\x00\x14\x00\x00\x00\x5C\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x68\x00\x00\x14\x00\x00\x00\x5D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x70\x00\x00\x14\x00\x00\x00\x5E\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x76\x00\x00\x14\x00\x00\x00\x60\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x84\x00\x00\x14\x00\x00\x00\x61\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x8C\x00\x00\x14\x00\x00\x00\x62\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x93\x00\x00\x14\x00\x00\x00\x63\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\x98\x00\x00\x14\x00\x00\x00\x65\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xA8\x00\x00\x14\x00\x00\x00\x66\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xB0\x00\x00\x14\x00\x00\x00\x67\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xBC\x00\x00\x14\x00\x00\x00\x68\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xC6\x00\x00\x14\x00\x00\x00\x6A\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xD4\x00\x00\x14\x00\x00\x00\x6B\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xD9\x00\x00\x14\x00\x00\x00\x6C\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xDF\x00\x00\x14\x00\x00\x00\x6D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xE4\x00\x00\x14\x00\x00\x00\x6E\x00\x00\x00\x00\x00\x00\x00\x6E\x01\x00\x00\x03\xE8\x00\x00\x14\x00\x00\x00\x6F\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xF1\x00\x00\x14\x00\x00\x00\x70\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xF6\x00\x00\x14\x00\x00\x00\x71\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x03\xFD\x00\x00\x14\x00\x00\x00\x72\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x05\x00\x00\x14\x00\x00\x00\x73\x00\x00\x00\x00\x00\x00\x00\x73\x01\x00\x00\x04\x11\x00\x00\x14\x00\x00\x00\x74\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x1D\x00\x00\x14\x00\x00\x00\x75\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x27\x00\x00\x14\x00\x00\x00\x76\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x2C\x00\x00\x14\x00\x00\x00\x77\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x04\x32\x00\x00\x14\x00\x00\x00\x78\x00\x00\x00\x00\x00\x00\x00\x78\x01\x00\x00\x04\x37\x00\x01\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\xC2\x20\x32\x31\x44\x44\x45\x36\x42\x30\x34\x44\x37\x39\x41\x38\x46\x45\x34\x45\x45\x41\x32\x36\x46\x38\x34\x36\x43\x38\x45\x30\x32\x39\x00\x42\x68\x74\x74\x70\x3A\x2F\x2F\x64\x6F\x77\x6E\x2E\x71\x71\x2E\x63\x6F\x6D\x2F\x71\x71\x6B\x61\x72\x74\x2F\x66\x75\x6C\x6C\x2F\x54\x69\x6D\x65\x72\x43\x68\x61\x6C\x6C\x65\x6E\x67\x65\x52\x65\x63\x6F\x72\x64\x5F\x70\x75\x74\x6F\x6E\x67\x5F\x6C\x6F\x77\x2E\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3A\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xC6\xD5\xCD\xA8\xA1\xA4\xBC\xAB\xCF\xDE\xCC\xF4\xD5\xBD\xD5\xDF\x00\x00\x00\xC3\x20\x38\x31\x43\x35\x35\x36\x38\x46\x41\x43\x35\x44\x45\x34\x36\x33\x43\x46\x31\x37\x36\x34\x34\x31\x44\x35\x33\x30\x45\x44\x32\x46\x00\x43\x68\x74\x74\x70\x3A\x2F\x2F\x64\x6F\x77\x6E\x2E\x71\x71\x2E\x63\x6F\x6D\x2F\x71\x71\x6B\x61\x72\x74\x2F\x66\x75\x6C\x6C\x2F\x54\x69\x6D\x65\x72\x43\x68\x61\x6C\x6C\x65\x6E\x67\x65\x52\x65\x63\x6F\x72\x64\x5F\x70\x75\x74\x6F\x6E\x67\x5F\x68\x69\x67\x68\x2E\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3A\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xC6\xD5\xCD\xA8\xA1\xA4\xBC\xAB\xCF\xDE\xCC\xF4\xD5\xBD\xD5\xDF\x00\x00\x00\xC3\x20\x35\x44\x43\x44\x38\x43\x39\x31\x43\x38\x37\x30\x30\x46\x33\x42\x31\x42\x44\x44\x43\x45\x36\x37\x43\x31\x35\x42\x32\x34\x38\x33\x00\x43\x68\x74\x74\x70\x3A\x2F\x2F\x64\x6F\x77\x6E\x2E\x71\x71\x2E\x63\x6F\x6D\x2F\x71\x71\x6B\x61\x72\x74\x2F\x66\x75\x6C\x6C\x2F\x54\x69\x6D\x65\x72\x43\x68\x61\x6C\x6C\x65\x6E\x67\x65\x52\x65\x63\x6F\x72\x64\x5F\x63\x68\x65\x73\x68\x65\x6E\x5F\x6C\x6F\x77\x2E\x73\x70\x64\x01\x00\x46\x00\x00\x00\x00\x00\x00\x27\x3A\x00\x00\x27\x77\x00\x00\x00\x00\x00\x00\x27\x42\x00\x00\x27\x30\x00\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xB3\xB5\xC9\xF1\xA1\xA4\xBC\xAB\xCF\xDE\xCC\xF4\xD5\xBD\xD5\xDF\x00\x00\x00\xC4\x20\x34\x35\x41\x41\x33\x43\x41\x41\x45\x35\x37\x42\x45\x41\x45\x36\x43\x46\x34\x39\x41\x36\x35\x39\x45\x42\x45\x41\x44\x30\x41\x39\x00\x44\x68\x74\x74\x70\x3A\x2F\x2F\x64\x6F\x77\x6E\x2E\x71\x71\x2E\x63\x6F\x6D\x2F\x71\x71\x6B\x61\x72\x74\x2F\x66\x75\x6C\x6C\x2F\x54\x69\x6D\x65\x72\x43\x68\x61\x6C\x6C\x65\x6E\x67\x65\x52\x65\x63\x6F\x72\x64\x5F\x63\x68\x65\x73\x68\x65\x6E\x5F\x68\x69\x67\x68\x2E\x73\x70\x64\x01\x00\x46\x00\x00\x27\x26\x00\x00\x27\x29\x00\x00\x27\x7A\x00\x00\x00\x00\x00\x00\x27\x3B\x00\x00\x27\x3D\x00\x00\x27\x3C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xB3\xB5\xC9\xF1\xA1\xA4\xBC\xAB\xCF\xDE\xCC\xF4\xD5\xBD\xD5\xDF\x00\x00\x00\x00\x00\x00\x0A\x00\x50\x0F\x01\x00\x06\x00\x00\x00\x00\x03\x03\x7B\x04\x00\xC4\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\x00\x80\xC8\xFC\xB3\xB5\xBF\xC9\xD4\xDA\xB5\xD8\xC3\xE6\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\xA3\xA8\xCC\xDA\xBF\xD5\xD7\xB4\xCC\xAC\xCE\xDE\xB7\xA8\xCA\xB9\xD3\xC3\xA3\xA9\xA1\xA3\x0A\xB3\xD6\xD0\xF8\xCA\xB1\xBC\xE4\x35\xC3\xEB\xA3\xAC\xC0\xE4\xC8\xB4\xCA\xB1\xBC\xE4\x31\x35\xC3\xEB\xA3\xAC\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x31\xB4\xCE\xA1\xA3\x0A\xCD\xA8\xB9\xFD\xC8\xFC\xB5\xC0\xD6\xD0\xB5\xC4\xBA\xEC\xC9\xAB\xB4\xB3\xB9\xD8\xC3\xC5\xA3\xAC\xBB\xE1\xBB\xD6\xB8\xB4\xD2\xBB\xB4\xCE\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x19\x01\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x3A\x98\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5F\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\x4C\x76\x32\x00\x18\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\xB5\xC4\xC0\xE4\xC8\xB4\xCA\xB1\xBC\xE4\x2D\x35\xC3\xEB\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x01\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5F\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\x4C\x76\x33\x00\x18\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\xB5\xC4\xB3\xD6\xD0\xF8\xCA\xB1\xBC\xE4\x2B\x32\xC3\xEB\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x01\x00\x00\x00\x00\x00\x00\x00\x1B\x58\x00\x00\x27\x10\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5D\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\x4C\x76\x34\x00\x16\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\xB5\xC4\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x2B\x31\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x01\x00\x00\x00\x00\x00\x00\x00\x1B\x58\x00\x00\x27\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x04\x00\x68\x00\x00\x01\x18\x00\x00\x00\x00\x00\x00\x00\x00\x01\xAB\xFF\xFF\xFF\xFF\xFF\x00\x08\xC9\xF1\xB7\xE7\xD3\xF1\xCC\xC3\x00\x24\xBF\xC9\xD7\xB0\xB1\xB8\xC2\xFA\xB8\xC4\x41\xBC\xB6\xC8\xFC\xB3\xB5\x2D\xC9\xF1\xB7\xE7\xD3\xF1\xCC\xC3\xBD\xF8\xD0\xD0\xB4\xF3\xB4\xB3\xB9\xD8\x01\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x5D\x00\x00\x01\x1B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xC5\xF6\xD7\xB2\xBC\xAF\xC6\xF8\x00\x19\xC3\xBF\xB4\xCE\xD7\xB2\xC7\xBD\xA3\xAC\xC1\xA2\xBF\xCC\xBB\xF1\xB5\xC3\x35\x30\x25\xBC\xAF\xC6\xF8\x01\x00\x00\x00\x00\x01\x1B\x00\x05\x00\x00\x00\x01\x1B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x00\x00\x01\x1C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xBC\xAF\xC6\xF8\xD4\xF6\xC7\xBF\x00\x0F\xBC\xAF\xC6\xF8\xD0\xA7\xC2\xCA\xD4\xF6\xBC\xD3\x33\x30\x25\x01\x00\x00\x00\x00\x01\x1C\x00\x05\x00\x00\x00\x01\x1C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x01\x1D\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xC6\xF0\xB2\xBD\xC7\xBF\xBB\xAF\x00\x3C\xC6\xF0\xB2\xBD\xC1\xA2\xBF\xCC\xBB\xF1\xB5\xC3\xD2\xBB\xB8\xF6\xB5\xAA\xC6\xF8\xA3\xAC\xC7\xD2\x32\x30\xC3\xEB\xC4\xDA\xB5\xAA\xC6\xF8\xD7\xEE\xB8\xDF\xCB\xD9\xB6\xC8\x2B\x33\x6B\x6D\x2F\x68\xA3\xAC\xB5\xAA\xC6\xF8\xB6\xAF\xC1\xA6\x2B\x35\x01\x00\x00\x00\x00\x01\x1D\x00\x05\x00\x00\x00\x01\x1D\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\xAE\x04\x00\xC5\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xB3\xE5\xB4\xCC\xC9\xB3\xC2\xA9\x00\x81\xCA\xB9\xC8\xFC\xB5\xC0\xC9\xCF\xB5\xC4\xC3\xC5\xB1\xE4\xCE\xAA\xC7\xBF\xBB\xAF\xC3\xC5\xA1\xA3\xCD\xA8\xB9\xFD\xBA\xF3\xB4\xB3\xB9\xD8\xCA\xB1\xBC\xE4\x2B\x32\xC3\xEB\xA3\xAC\xB3\xD6\xD0\xF8\xCA\xB1\xBC\xE4\x31\x35\xC3\xEB\xA3\xAC\xC0\xE4\xC8\xB4\xCA\xB1\xBC\xE4\x32\x35\xC3\xEB\xA3\xAC\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x31\xB4\xCE\xA1\xA3\xCD\xA8\xB9\xFD\xC8\xFC\xB5\xC0\xD6\xD0\xB5\xC4\xBA\xEC\xC9\xAB\xB4\xB3\xB9\xD8\xC3\xC5\xA3\xAC\xBB\xE1\xBB\xD6\xB8\xB4\xD2\xBB\xB4\xCE\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x00\x00\x00\x3A\x98\x00\x00\x61\xA8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5F\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xB3\xE5\xB4\xCC\xC9\xB3\xC2\xA9\x4C\x76\x32\x00\x18\xB3\xE5\xB4\xCC\xC9\xB3\xC2\xA9\xB5\xC4\xB3\xD6\xD0\xF8\xCA\xB1\xBC\xE4\x2B\x35\xC3\xEB\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x4E\x20\x00\x00\x61\xA8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x7B\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xB3\xE5\xB4\xCC\xC9\xB3\xC2\xA9\x4C\x76\x33\x00\x34\xB4\xA9\xB9\xFD\xCC\xD8\xCA\xE2\xB5\xC4\xC7\xBF\xBB\xAF\xC3\xC5\xBA\xF3\xA3\xAC\xBD\xAB\xBB\xE1\xC1\xA2\xBF\xCC\xBB\xF1\xB5\xC3\xD2\xBB\xB8\xF6\xB6\xCC\xD4\xDD\xB5\xC4\xB5\xAF\xC9\xE4\xBC\xD3\xCB\xD9\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x4E\x20\x00\x00\x61\xA8\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5D\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xB3\xE5\xB4\xCC\xC9\xB3\xC2\xA9\x4C\x76\x34\x00\x16\xB3\xE5\xB4\xCC\xC9\xB3\xC2\xA9\xB5\xC4\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x2B\x31\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x4E\x20\x00\x00\x61\xA8\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x04\x00\x65\x00\x00\x01\x19\x00\x00\x00\x00\x00\x00\x00\x00\x01\x2D\xB3\x00\x00\x00\x05\x00\x06\xCA\xD8\xBB\xA4\xD5\xDF\x00\x23\xBF\xC9\xD7\xB0\xB1\xB8\xC2\xFA\xB8\xC4\x54\x32\xBC\xB6\xC8\xFC\xB3\xB5\x2D\xCA\xD8\xBB\xA4\xD5\xDF\xBD\xF8\xD0\xD0\xB4\xF3\xB4\xB3\xB9\xD8\x01\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x60\x00\x00\x01\x1E\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xD6\xFA\xC1\xA6\xC6\xF0\xC5\xDC\x00\x1C\xC3\xBF\xB0\xF4\xC6\xF0\xC5\xDC\xCA\xB1\xA3\xAC\x31\x30\x30\x25\xB4\xB3\xB9\xD8\xCA\xB1\xBC\xE4\x2B\x31\xC3\xEB\x01\x00\x00\x00\x00\x01\x1E\x00\x05\x00\x00\x00\x01\x1E\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7B\x00\x00\x01\x1F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xC5\xE7\xC9\xE4\xBC\xD3\xB3\xC9\x00\x37\xCA\xB9\xD3\xC3\xCB\xAB\xC5\xE7\xA3\xA8\xBA\xAC\x63\x77\x77\xA1\xA2\x77\x63\x77\xA3\xA9\xBA\xF3\xA3\xAC\xB4\xB3\xB9\xD8\xCA\xB1\xBC\xE4\x2B\x31\xC3\xEB\xA3\xAC\xA3\xA8\xC3\xBF\xB0\xF4\xC9\xCF\xCF\xDE\x32\xB4\xCE\xA3\xA9\x01\x00\x00\x00\x00\x01\x1F\x00\x05\x00\x00\x00\x01\x1F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6E\x00\x00\x01\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xB5\xAA\xC6\xF8\xBC\xD3\xB3\xC9\x00\x2A\xC3\xBF\x34\xB4\xCE\xCA\xB9\xD3\xC3\xB5\xAA\xC6\xF8\xA3\xAC\xB4\xB3\xB9\xD8\xCA\xB1\xBC\xE4\x2B\x32\xC3\xEB\xA3\xA8\xC3\xBF\xB0\xF4\xC9\xCF\xCF\xDE\x31\xB4\xCE\xA3\xA9\x01\x00\x00\x00\x00\x01\x20\x00\x05\x00\x00\x00\x01\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x39\x04\x00\xB1\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\x00\x6D\xCA\xB9\xD3\xC3\xBA\xF3\xB3\xD6\xD0\xF8\xBB\xF1\xB5\xC3\xB6\xEE\xCD\xE2\xD0\xA1\xC5\xE7\xA1\xA3\xB3\xD6\xD0\xF8\xCA\xB1\xBC\xE4\x38\xC3\xEB\xA3\xAC\xC0\xE4\xC8\xB4\xCA\xB1\xBC\xE4\x32\x30\xC3\xEB\xA3\xAC\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x31\xB4\xCE\xA1\xA3\x0A\xCD\xA8\xB9\xFD\xC8\xFC\xB5\xC0\xD6\xD0\xB5\xC4\xBA\xEC\xC9\xAB\xB4\xB3\xB9\xD8\xC3\xC5\xA3\xAC\xBB\xE1\xBB\xD6\xB8\xB4\xD2\xBB\xB4\xCE\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x19\x01\x00\x00\x00\x00\x00\x00\x00\x1F\x40\x00\x00\x4E\x20\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5F\x00\x00\x00\x0A\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\x4C\x76\x32\x00\x18\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\xB5\xC4\xB3\xD6\xD0\xF8\xCA\xB1\xBC\xE4\x2B\x34\xC3\xEB\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x2E\xE0\x00\x00\x4E\x20\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5F\x00\x00\x00\x0B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\x4C\x76\x33\x00\x18\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\xB5\xC4\xC0\xE4\xC8\xB4\xCA\xB1\xBC\xE4\x2D\x35\xC3\xEB\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x2E\xE0\x00\x00\x3A\x98\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x00\x5D\x00\x00\x00\x0C\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\x4C\x76\x34\x00\x16\xBF\xF1\xB1\xA9\xD0\xA1\xC5\xE7\xB5\xC4\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x2B\x31\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x2E\xE0\x00\x00\x3A\x98\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07\x04\x00\x70\x00\x00\x01\x1A\x00\x00\x00\x00\x00\x00\x00\x00\x01\xA3\x6B\x00\x00\x00\x19\x00\x0C\x46\x32\x32\x2D\xC0\xD7\xF6\xAA\xCD\xF2\xBE\xFB\x00\x28\xBF\xC9\xD7\xB0\xB1\xB8\xC2\xFA\xB8\xC4\x53\xBC\xB6\xC8\xFC\xB3\xB5\x2D\x46\x32\x32\x2D\xC0\xD7\xF6\xAA\xCD\xF2\xBE\xFB\xBD\xF8\xD0\xD0\xB4\xF3\xB4\xB3\xB9\xD8\x01\x00\x00\x00\x00\x00\x00\x00\x19\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x00\x00\x01\x21\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xC6\xBD\xC5\xDC\xC7\xBF\xBB\xAF\x00\x0F\xC6\xBD\xC5\xDC\xBC\xAB\xCB\xD9\xC4\xDC\xC1\xA6\x2B\x31\x32\x01\x00\x00\x00\x00\x01\x21\x00\x05\x00\x00\x00\x01\x21\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x00\x00\x01\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xD0\xA1\xC5\xE7\xC7\xBF\xBB\xAF\x00\x0F\xD0\xA1\xC5\xE7\xBC\xAB\xCB\xD9\xC4\xDC\xC1\xA6\x2B\x31\x32\x01\x00\x00\x00\x00\x01\x22\x00\x05\x00\x00\x00\x01\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x00\x00\x01\x23\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xB5\xAA\xC6\xF8\xC7\xBF\xBB\xAF\x00\x0F\xB5\xAA\xC6\xF8\xBC\xAB\xCB\xD9\xC4\xDC\xC1\xA6\x2B\x31\x32\x01\x00\x00\x00\x00\x01\x23\x00\x05\x00\x00\x00\x01\x23\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x03\x0B\x00\x31\x00\x13\x00\x00\x00\x07\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x13\x00\x00\x00\x08\x03\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00\x06\x00\x13\x00\x00\x00\x09\x03\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00\x09\x00\x13\x00\x00\x00\x0A\x03\x00\x00\x00\x0B\x00\x00\x00\x0C\x00\x00\x00\x0D\x00\x13\x00\x00\x00\x0B\x03\x00\x00\x00\x0F\x00\x00\x00\x10\x00\x00\x00\x11\x00\x13\x00\x00\x00\x0C\x03\x00\x00\x00\x13\x00\x00\x00\x14\x00\x00\x00\x15\x00\x13\x00\x00\x00\x16\x03\x00\x00\x00\x16\x00\x00\x00\x17\x00\x00\x00\x18\x00\x13\x00\x00\x00\x17\x03\x00\x00\x00\x19\x00\x00\x00\x1A\x00\x00\x00\x1B\x00\x13\x00\x00\x00\x18\x03\x00\x00\x00\x1D\x00\x00\x00\x1E\x00\x00\x00\x1F\x00\x13\x00\x00\x00\x19\x03\x00\x00\x00\x21\x00\x00\x00\x22\x00\x00\x00\x23\x00\x07\x00\x00\x00\x22\x00\x00\x13\x00\x00\x00\x23\x03\x00\x00\x00\x24\x00\x00\x00\x25\x00\x00\x00\x26\x00\x13\x00\x00\x00\x24\x03\x00\x00\x00\x27\x00\x00\x00\x28\x00\x00\x00\x29\x00\x13\x00\x00\x00\x25\x03\x00\x00\x00\x2B\x00\x00\x00\x2C\x00\x00\x00\x2D\x00\x13\x00\x00\x00\x26\x03\x00\x00\x00\x2F\x00\x00\x00\x30\x00\x00\x00\x31\x00\x07\x00\x00\x00\x27\x00\x00\x07\x00\x00\x00\x30\x00\x00\x13\x00\x00\x00\x31\x03\x00\x00\x00\x33\x00\x00\x00\x34\x00\x00\x00\x35\x00\x13\x00\x00\x00\x32\x03\x00\x00\x00\x36\x00\x00\x00\x37\x00\x00\x00\x38\x00\x13\x00\x00\x00\x33\x03\x00\x00\x00\x3A\x00\x00\x00\x3B\x00\x00\x00\x3C\x00\x13\x00\x00\x00\x34\x03\x00\x00\x00\x3E\x00\x00\x00\x3F\x00\x00\x00\x40\x00\x07\x00\x00\x00\x3F\x00\x00\x13\x00\x00\x00\x40\x03\x00\x00\x00\x42\x00\x00\x00\x43\x00\x00\x00\x44\x00\x13\x00\x00\x00\x41\x03\x00\x00\x00\x46\x00\x00\x00\x47\x00\x00\x00\x48\x00\x13\x00\x00\x00\x42\x03\x00\x00\x00\x4A\x00\x00\x00\x4B\x00\x00\x00\x4C\x00\x13\x00\x00\x00\x4D\x03\x00\x00\x00\x4E\x00\x00\x00\x4F\x00\x00\x00\x50\x00\x13\x00\x00\x00\x4E\x03\x00\x00\x00\x52\x00\x00\x00\x53\x00\x00\x00\x54\x00\x13\x00\x00\x00\x4F\x03\x00\x00\x00\x56\x00\x00\x00\x57\x00\x00\x00\x58\x00\x07\x00\x00\x00\x58\x00\x00\x13\x00\x00\x00\x59\x03\x00\x00\x00\x5A\x00\x00\x00\x5B\x00\x00\x00\x5C\x00\x13\x00\x00\x00\x5A\x03\x00\x00\x00\x5E\x00\x00\x00\x5F\x00\x00\x00\x60\x00\x13\x00\x00\x00\x5B\x03\x00\x00\x00\x62\x00\x00\x00\x63\x00\x00\x00\x64\x00\x07\x00\x00\x00\x5C\x00\x00\x07\x00\x00\x00\x63\x00\x00\x13\x00\x00\x00\x64\x03\x00\x00\x00\x66\x00\x00\x00\x67\x00\x00\x00\x68\x00\x13\x00\x00\x00\x65\x03\x00\x00\x00\x6A\x00\x00\x00\x6B\x00\x00\x00\x6C\x00\x13\x00\x00\x00\x66\x03\x00\x00\x00\x6E\x00\x00\x00\x6F\x00\x00\x00\x70\x00\x07\x00\x00\x00\x67\x00\x00\x07\x00\x00\x00\x68\x00\x00\x07\x00\x00\x00\x73\x00\x00\x13\x00\x00\x00\x74\x03\x00\x00\x00\x72\x00\x00\x00\x73\x00\x00\x00\x74\x00\x13\x00\x00\x00\x75\x03\x00\x00\x00\x76\x00\x00\x00\x77\x00\x00\x00\x78\x00\x13\x00\x00\x00\x76\x03\x00\x00\x00\x7A\x00\x00\x00\x7B\x00\x00\x00\x7C\x00\x07\x00\x00\x00\x77\x00\x00\x07\x00\x00\x00\x80\x00\x00\x13\x00\x00\x00\x81\x03\x00\x00\x00\x7E\x00\x00\x00\x7F\x00\x00\x00\x80\x00\x13\x00\x00\x00\x82\x03\x00\x00\x00\x82\x00\x00\x00\x83\x00\x00\x00\x84\x00\x13\x00\x00\x00\x83\x03\x00\x00\x00\x86\x00\x00\x00\x87\x00\x00\x00\x88\x00\x07\x00\x00\x00\x84\x00\x19";
		SendToClient(Client, 24258, (BYTE*)buf, 6709, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}



void NotifyPreGetCommonBoxAwardMore(std::shared_ptr<ClientNode> Client, BOOL HasMore, std::vector<ItemInfo>& MiracleItems, size_t offset, char MiracleItemNum)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	{ //AwardCfgArray
		BYTE* pAwardCfgArray = p;
		Write16(pAwardCfgArray, 0); //len

		Write8(pAwardCfgArray, MiracleItemNum); //ScoreAwardNum 
		for (size_t i = offset; i < offset + MiracleItemNum; i++)
		{ //ScoreAwardCfg
			BYTE* pScoreAwardCfg = pAwardCfgArray;
			Write16(pScoreAwardCfg, 0); //len

			Write8(pScoreAwardCfg, 0); //Type
			Write32(pScoreAwardCfg, 0); //StartScore
			{ //AwardInfo
				BYTE* pAwardInfo = pScoreAwardCfg;
				Write16(pAwardInfo, 0); //len

				Write32(pAwardInfo, 0); //Exp
				Write32(pAwardInfo, 0); //Money
				Write32(pAwardInfo, 0); //SuperMoney
				Write16(pAwardInfo, 1); //ItemNum
				//for (size_t i = 0; i < 1; i++)
				{ //ItemInfo
					BYTE* pItemInfo = pAwardInfo;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, MiracleItems[i].ItemID); //ItemID
					Write32(pItemInfo, MiracleItems[i].ItemNum); //ItemNum
					Write32(pItemInfo, MiracleItems[i].AvailPeriod); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, MiracleItems[i].ItemType); //ItemType

					len = pItemInfo - pAwardInfo;
					Set16(pAwardInfo, (WORD)len);
					pAwardInfo += len;
				}

				Write32(pAwardInfo, 0); //Coupons
				Write32(pAwardInfo, 0); //GuildPoint
				Write32(pAwardInfo, 0); //LuckMoney

				Write8(pAwardInfo, 0); //ExtendInfoNum


				Write32(pAwardInfo, 0); //SpeedCoin

				len = pAwardInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}
			Write8(pScoreAwardCfg, 1);
			Write16(pScoreAwardCfg, 0); //ResultID
			Write8(pScoreAwardCfg, 1); //HasMoreInfo 
			//for (size_t i = 0; i < 1; i++)
			{ //MoreInfo
				BYTE* pMoreInfo = pScoreAwardCfg;
				Write16(pMoreInfo, 0); //len

				Write16(pMoreInfo, 0); //AlreayGetNum
				Write16(pMoreInfo, 0); //MaxGetNum
				Write32(pMoreInfo, 0); //ItemID 
				Write32(pMoreInfo, 0); //TimeStart
				Write32(pMoreInfo, 0); //TimeEnd
				Write32(pMoreInfo, 0); //NextAddNumLeftTime
				Write8(pMoreInfo, 1); //Num
				//for (size_t i = 0; i < 1; i++)
				{ //ComsumeItemArray 
					BYTE* pComsumeItemArray = pMoreInfo;
					Write16(pComsumeItemArray, 0); //len

					Write32(pComsumeItemArray, MiracleItems[i].ComsumeItemId); //ItemID
					Write32(pComsumeItemArray, MiracleItems[i].ComsumeItemNum); //StartScore 
					Write32(pComsumeItemArray, 0); //DiscountStartScore

					len = pComsumeItemArray - pMoreInfo;
					Set16(pMoreInfo, (WORD)len);
					pMoreInfo += len;
				}
				Write8(pMoreInfo, 0); //ClientParaNum
				//Write32(pMoreInfo, 0); //ClientParas[]
				Write32(pMoreInfo, 0); //DiscountTimeStart
				Write32(pMoreInfo, 0); //DiscountTimeEnd
				Write8(pMoreInfo, 0); //DiscountStatus
				Write32(pMoreInfo, 0); //LimitPara
				Write8(pMoreInfo, 0); //ItemExchangeType
				Write8(pMoreInfo, 0); //ItemLabelType
				Write16(pMoreInfo, 0); //ShowPriority
				Write8(pMoreInfo, 0); //Para0
				Write32(pMoreInfo, 0); //LastUpdateTime

				len = pMoreInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}

			Write32(pScoreAwardCfg, i); //PosKey
			Write32(pScoreAwardCfg, i); //AwardIdx
			Write8(pScoreAwardCfg, 0); //LabelStatus
			Write8(pScoreAwardCfg, 0); //ExtraAwardNum
			//Write32(pScoreAwardCfg, 0); 

			len = pScoreAwardCfg - pAwardCfgArray;
			Set16(pAwardCfgArray, (WORD)len);
			pAwardCfgArray += len;
		}
		len = pAwardCfgArray - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, HasMore); //HasNext

	len = p - buf.data();
	SendToClient(Client, 25206, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestPreGetCommonBoxAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	USHORT Type = Read16(Body);
	int AwardID = Read32(Body);
	UCHAR IsShowAllAward = Read8(Body);
	int EchoClient = Read32(Body);
	//printf("Type:%d\n", Type);
	if (Type == 45)
	{
		BOOL HasMore = FALSE;
		int offset = 0;
		int AllItemNum = MiracleCfg->MiracleItems.size();
		std::vector<ItemInfo> Items = MiracleCfg->MiracleItems;
		AllItemNum -= 20;
		if (AllItemNum > 0)
		{
			HasMore = TRUE;
			ResponsePreGetCommonBoxAward(Client, HasMore, Type, Items, 20);
			offset += 20;
		}
		else
		{
			ResponsePreGetCommonBoxAward(Client, HasMore, Type, Items, AllItemNum + 20);
		}
		while (HasMore)
		{
			HasMore = FALSE;
			AllItemNum -= 20;
			if (AllItemNum > 0)
			{
				HasMore = TRUE;
				NotifyPreGetCommonBoxAwardMore(Client, HasMore, Items, offset, 20);
				offset += 20;
			}
			else
			{
				NotifyPreGetCommonBoxAwardMore(Client, HasMore, Items, offset, AllItemNum + 20);
			}
		}
	}
	else if (Type == 312)
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

		Write32(p, Client->Uin); //Uin
		Write16(p, 0); //ResultID
		Write16(p, Type); //Type
		{ //AwardCfgArray
			BYTE* pAwardCfgArray = p;
			Write16(pAwardCfgArray, 0); //len
			Write8(pAwardCfgArray, CommonBoxAwards[Type].size()); //ScoreAwardNum
			for (size_t i = 0; i < CommonBoxAwards[Type].size(); i++)
			{ //ScoreAwardCfg
				CarTreasureInfo CarTs = CommonBoxAwards[Type][i];
				BYTE* pScoreAwardCfg = pAwardCfgArray;
				Write16(pScoreAwardCfg, 0); //len

				Write8(pScoreAwardCfg, 0); //Type
				Write32(pScoreAwardCfg, CarTs.ExchangePieceNum); //StartScore
				{ //AwardInfo
					BYTE* pAwardInfo = pScoreAwardCfg;
					Write16(pAwardInfo, 0); //len

					Write32(pAwardInfo, 0); //Exp
					Write32(pAwardInfo, 0); //Money
					Write32(pAwardInfo, 0); //SuperMoney
					Write16(pAwardInfo, 1); //ItemNum
					for (size_t i = 0; i < 1; i++)
					{ //ItemInfo
						BYTE* pItemInfo = pAwardInfo;
						Write16(pItemInfo, 0); //len

						Write32(pItemInfo, CarTs.ItemId); //ItemID
						Write32(pItemInfo, CarTs.ItemNum); //ItemNum
						Write32(pItemInfo, CarTs.AvailPeriod); //AvailPeriod
						Write8(pItemInfo, 0); //Status
						Write32(pItemInfo, 0); //ObtainTime
						Write32(pItemInfo, 0); //OtherAttribute
						Write16(pItemInfo, 0); //ItemType

						len = pItemInfo - pAwardInfo;
						Set16(pAwardInfo, (WORD)len);
						pAwardInfo += len;
					}

					Write32(pAwardInfo, 0); //Coupons
					Write32(pAwardInfo, 0); //GuildPoint
					Write32(pAwardInfo, 0); //LuckMoney

					Write8(pAwardInfo, 0); //ExtendInfoNum
					for (size_t i = 0; i < 0; i++)
					{ //ExtendInfoAward
						BYTE* pExtendInfoAward = pAwardInfo;
						Write16(pExtendInfoAward, 0); //len

						Write16(pExtendInfoAward, 0); //Key
						Write32(pExtendInfoAward, 0); //AddValue
						Write32(pExtendInfoAward, 0); //TotalValue
						Write8(pExtendInfoAward, 0); //ShowPriority
						Write16(pExtendInfoAward, 0); //BuyNeedScore
						Write8(pExtendInfoAward, 0); //OneMatchMaxNum

						len = pExtendInfoAward - pAwardInfo;
						Set16(pAwardInfo, (WORD)len);
						pAwardInfo += len;
					}

					Write32(pAwardInfo, 0); //SpeedCoin

					len = pAwardInfo - pScoreAwardCfg;
					Set16(pScoreAwardCfg, (WORD)len);
					pScoreAwardCfg += len;
				}
				Write8(pScoreAwardCfg, 0); //Status
				Write16(pScoreAwardCfg, 11498); //ResultID
				Write8(pScoreAwardCfg, 1); //HasMoreInfo
				for (size_t i = 0; i < 1; i++)
				{ //MoreInfo
					BYTE* pMoreInfo = pScoreAwardCfg;
					Write16(pMoreInfo, 0); //len

					Write16(pMoreInfo, 0); //AlreayGetNum
					Write16(pMoreInfo, 0); //MaxGetNum
					Write32(pMoreInfo, 94480); //ItemID
					Write32(pMoreInfo, 1711900800); //TimeStart
					Write32(pMoreInfo, 1714492799); //TimeEnd
					Write32(pMoreInfo, 0); //NextAddNumLeftTime
					Write8(pMoreInfo, 0); //Num
					for (size_t i = 0; i < 0; i++)
					{ //ComsumeItemArray
						BYTE* pComsumeItemArray = pMoreInfo;
						Write16(pComsumeItemArray, 0); //len

						Write32(pComsumeItemArray, 0); //ItemID
						Write32(pComsumeItemArray, 0); //StartScore
						Write32(pComsumeItemArray, 0); //DiscountStartScore

						len = pComsumeItemArray - pMoreInfo;
						Set16(pMoreInfo, (WORD)len);
						pMoreInfo += len;
					}
					Write8(pMoreInfo, 0); //ClientParaNum
					//Write32(pMoreInfo, 0); //ClientParas[]
					Write32(pMoreInfo, 0); //DiscountTimeStart
					Write32(pMoreInfo, 0); //DiscountTimeEnd
					Write8(pMoreInfo, 0); //DiscountStatus
					Write32(pMoreInfo, 0); //LimitPara
					Write8(pMoreInfo, 0); //ItemExchangeType
					Write8(pMoreInfo, 0); //ItemLabelType
					Write16(pMoreInfo, 0); //ShowPriority
					Write8(pMoreInfo, 0); //Para0
					Write32(pMoreInfo, 0); //LastUpdateTime

					len = pMoreInfo - pScoreAwardCfg;
					Set16(pScoreAwardCfg, (WORD)len);
					pScoreAwardCfg += len;
				}


				Write32(pScoreAwardCfg, i); //PosKey
				Write8(pScoreAwardCfg, i); //AwardIdx
				Write8(pScoreAwardCfg, 0); //LabelStatus

				Write8(pScoreAwardCfg, 0); //ExtraAwardNum
				for (size_t i = 0; i < 0; i++)
				{ //ExtraAwardInfo
					BYTE* pExtraAwardInfo = pScoreAwardCfg;
					Write16(pExtraAwardInfo, 0); //len

					Write32(pExtraAwardInfo, 0); //Exp
					Write32(pExtraAwardInfo, 0); //Money
					Write32(pExtraAwardInfo, 0); //SuperMoney
					Write16(pExtraAwardInfo, 0); //ItemNum
					for (size_t i = 0; i < 0; i++)
					{ //ItemInfo
						BYTE* pItemInfo = pExtraAwardInfo;
						Write16(pItemInfo, 0); //len

						Write32(pItemInfo, 0); //ItemID
						Write32(pItemInfo, 0); //ItemNum
						Write32(pItemInfo, 0); //AvailPeriod
						Write8(pItemInfo, 0); //Status
						Write32(pItemInfo, 0); //ObtainTime
						Write32(pItemInfo, 0); //OtherAttribute
						Write16(pItemInfo, 0); //ItemType

						len = pItemInfo - pExtraAwardInfo;
						Set16(pExtraAwardInfo, (WORD)len);
						pExtraAwardInfo += len;
					}
					Write32(pExtraAwardInfo, 0); //Coupons
					Write32(pExtraAwardInfo, 0); //GuildPoint
					Write32(pExtraAwardInfo, 0); //LuckMoney

					Write8(pExtraAwardInfo, 0); //ExtendInfoNum
					for (size_t i = 0; i < 0; i++)
					{ //ExtendInfoAward
						BYTE* pExtendInfoAward = pExtraAwardInfo;
						Write16(pExtendInfoAward, 0); //len

						Write16(pExtendInfoAward, 0); //Key
						Write32(pExtendInfoAward, 0); //AddValue
						Write32(pExtendInfoAward, 0); //TotalValue
						Write8(pExtendInfoAward, 0); //ShowPriority
						Write16(pExtendInfoAward, 0); //BuyNeedScore
						Write8(pExtendInfoAward, 0); //OneMatchMaxNum

						len = pExtendInfoAward - pExtraAwardInfo;
						Set16(pExtraAwardInfo, (WORD)len);
						pExtraAwardInfo += len;
					}

					Write32(pExtraAwardInfo, 0); //SpeedCoin

					len = pExtraAwardInfo - pScoreAwardCfg;
					Set16(pScoreAwardCfg, (WORD)len);
					pScoreAwardCfg += len;
				}
				Write8(pScoreAwardCfg, 0);  //zhanwei1
				Write32(pScoreAwardCfg, 0);   //zhanwei2
				Write16(pScoreAwardCfg, 0);  //zhanwei3
				len = pScoreAwardCfg - pAwardCfgArray;
				Set16(pAwardCfgArray, (WORD)len);
				pAwardCfgArray += len;
			}
			len = pAwardCfgArray - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Write32(p, 0); //AwardID
		Write32(p, 0); //NextClearLeftTime
		Write8(p, 0); //HasMoreInfo
		switch (Type)
		{
		case 312:
		{
			int ItemSpNum = PlayerDB_GetItemNum(Client->Uin, 94880);
			Write32(p, ItemSpNum); //Score 碎片数量
			break;
		}
		default:
			Write32(p, 0); //Score 碎片数量
			break;
		}

		Write16(p, 0); //LeftGetAwardNum
		Write8(p, 0); //Status
		Write8(p, 0); //EnableMergeSameItem
		Write32(p, 1711900800); //TimeStart
		Write32(p, 1714492799); //TimeEnd
		Write32(p, 0); //SCPara
		Write8(p, IsShowAllAward); //IsShowAllAward
		Write16(p, 0); //ValueNum
		//Write32(p, 0); //Values[]
		Write32(p, EchoClient); //EchoClient
		Write16(p, 0); //new
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		//delete[] buf;
	}
	else if (Type == 194) {
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "000000C207920F00810000000064003A000000000000000000000000000100190001A730000000010000000000000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000100000000000000810000000064003A0000000000000000000000000001001900003E7600000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020000000200000000000000810000000064003A000000000000000000000000000100190000544B00000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000030000000300000000000000810000000064003A00000000000000000000000000010019000031B000000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000040000000400000000000000810000000064003A0000000000000000000000000001001900019ED300000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000050000000500000000000000810000000064003A000000000000000000000000000100190001BFFB00000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000600000000000000810000000064003A000000000000000000000000000100190001C5B300000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000070000000700000000000000810000000064003A000000000000000000000000000100190001D7B100000001FFFFFFFF00000000000000000000010000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000080000000800000000000000810000000064003A000000000000000000000000000100190001001700000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000090000000900000000000000810000000064003A000000000000000000000000000100190001022600000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A0000000A00000000000000810000000064003A000000000000000000000000000100190001024E00000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000B0000000B00000000000000810000000064003A00000000000000000000000000010019000055D9000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C0000000C00000000000000810000000064003A0000000000000000000000000001001900003058000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000D0000000D00000000000000810000000064003A0000000000000000000000000001001900003059000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000E0000000E00000000000000810000000064003A000000000000000000000000000100190001A731000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000F0000000F000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 195) {
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "000000C3060F0C00810000000006003A000000000000000000000000000100190001414800000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A73000000000000000000000000000000000000000000000000000000000000000000000000000000001000000010000000000000081000000000C003A000000000000000000000000000100190001A36900000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000020000000200000000000000810000000006003A0000000000000000000000000001001900005B5200000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000030000000300000000000000810000000006003A000000000000000000000000000100190000662000000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000040000000400000000000000810000000001003A0000000000000000000000000001001900002C5800000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000050000000500000000000000810000000001003A000000000000000000000000000100190000785C00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000060000000600000000000000810000000001003A00000000000000000000000000010019000078AC00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000070000000700000000000000810000000001003A00000000000000000000000000010019000075A100000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000080000000800000000000000810000000001003A000000000000000000000000000100190000F8D700000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A730000000000000000000000000000000000000000000000000000000000000000000000000000000090000000900000000000000810000000001003A0000000000000000000000000001001900007CA900000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A7300000000000000000000000000000000000000000000000000000000000000000000000000000000A0000000A00000000000000810000000001003A0000000000000000000000000001001900007CB000000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A7300000000000000000000000000000000000000000000000000000000000000000000000000000000B0000000B00000000000000810000000001003A00000000000000000000000000010019000042D100000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E000000000001A7300000000000000000000000000000000000000000000000000000000000000000000000000000000C0000000C000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 186) { //筑梦
		//if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "000000BA0207040081000000003C003A0000000000000000000000000001001900003058000000050000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000010000000000000081000000023A003A0000000000000000000000000001001900023B6400000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E00000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000200000002000000000000008100000006A4003A0000000000000000000000000001001900023B4B00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E0000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000030000000300000000000000810000000BEA003A0000000000000000000000000001001900023C8500000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E00000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000400000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 294) { //筑梦
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "0000012604130800810000000005003A0000000000000000000000000001001900023337000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000066FACB006755C27F00000000000000000000000000000000000000000000500000000000000000010000000100000000000000810000000005003A000000000000000000000000000100190002333A000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000066FACB006755C27F0000000000000000000000000000000000000000000046000000000000000002000000020000000000000089000000000500210000000000000000000001F400000000000000000000000000000000000000002CEA01002E000000030000000066FACB006755C27F000000000000000000000000000000000000000200003C00000000000000000300000003000100210000000000000000000001F4000000000000000000000000000000000000000000000000810000000003003A0000000000000000000000000001001900004D2C000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000030000000066FACB006755C27F00000000000000000000000000000000000000020000320000000000000000040000000400000000000000810000000003003A0000000000000000000000000001001900003059000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000066FACB006755C27F00000000000000000000000000000000000000000000280000000000000000050000000500000000000000810000000003003A0000000000000000000000000001001900017955000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000066FACB006755C27F000000000000000000000000000000000000000000001E0000000000000000060000000600000000000000810000000003003A0000000000000000000000000001001900003058000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000066FACB006755C27F00000000000000000000000000000000000000000000140000000000000000070000000700000000000000810000000003003A000000000000000000000000000100190000305C000000010000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000066FACB006755C27F000000000000000000000000000000000000000000000A00000000000000000800000008000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 401) { //天山
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "000001910D001900850000000001003A00000000000000000000000000010019000039870000000500000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000190000000000000000190000001900000000000000850000000001003A000000000000000000000000000100190000487A0000000100000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000180000000000000000180000001800000000000000850000000001003A0000000000000000000000000001001900004D2C0000000100000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000170000000000000000170000001700000000000000850000000001003A000000000000000000000000000100190000358C0000000100000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000160000000000000000160000001600000000000000850000000001003A000000000000000000000000000100190000305C0000000100000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000150000000000000000150000001500000000000000850000000001003A00000000000000000000000000010019000179550000000100000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000140000000000000000140000001400000000000000850000000001003A00000000000000000000000000010019000030580000000100000000000000000000000000000000000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000000000000000000000000000000000000130000000000000000130000001300000000000000850000000001003A000000000000000000000000000100190001D5C600000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000300000000000000000000000000000000120000000000000000120000001200000000000000850000000001003A000000000000000000000000000100190001D5C600000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC200000000000000000000000000010000000300000000000000000000000000000000110000000000000000110000001100000000000000850000000001003A000000000000000000000000000100190001CFCE00000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000300000000000000000000000000000000100000000000000000100000001000000000000000850000000001003A000000000000000000000000000100190001CFCE00000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC2000000000000000000000000000100000003000000000000000000000000000000000F00000000000000000F0000000F00000000000000850000000001003A000000000000000000000000000100190001CFD200000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC2000000000000000000000000000100000003000000000000000000000000000000000E00000000000000000E0000000E00000000000000850000000001003A000000000000000000000000000100190001CFD200000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC2000000000000000000000000000100000003000000000000000000000000000000000D00000000000000000D0000000D00000000000000850000000001003A000000000000000000000000000100190001D16C00000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC2000000000000000000000000000100000002000000000000000000000000000000000C00000000000000000C0000000C00000000000000850000000001003A000000000000000000000000000100190001D16C00000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC2000000000000000000000000000100000002000000000000000000000000000000000B00000000000000000B0000000B00000000000000850000000001003A000000000000000000000000000100190001D16400000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC2000000000000000000000000000100000002000000000000000000000000000000000A00000000000000000A0000000A00000000000000850000000001003A000000000000000000000000000100190001D16400000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC200000000000000000000000000010000000200000000000000000000000000000000090000000000000000090000000900000000000000850000000001003A000000000000000000000000000100190001D91600000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000200000000000000000000000000000000080000000000000000080000000800000000000000850000000001003A000000000000000000000000000100190001D91600000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC200000000000000000000000000010000000200000000000000000000000000000000070000000000000000070000000700000000000000850000000001003A000000000000000000000000000100190000F88C00000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000100000000000000000000000000000000060000000000000000060000000600000000000000850000000001003A000000000000000000000000000100190000F88C00000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC200000000000000000000000000010000000100000000000000000000000000000000050000000000000000050000000500000000000000850000000001003A000000000000000000000000000100190001282100000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000000023BC200000000000000000000000000010000000100000000000000000000000000000000040000000000000000040000000400000000000000850000000001003A000000000000000000000000000100190001282100000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC200000000000000000000000000010000000100000000000000000000000000000000030000000000000000030000000300000000000000850000000001003A0000000000000000000000000001001900007E9700000001000000A8000000000000000000000100000000000000000000000000000000000100000100320000000100023BC200000000000000000000000000010000000100000000000000000000000000000000020000000000000000020000000200000000000000850000000001003A0000000000000000000000000001001900007E9700000001FFFFFFFF000000000000000000000100000000000000000000000000000000000100000100320000000100023BC2000000000000000000000000000100000001000000000000000000000000000000000100000000000000000100000001000000000000000000000000000000000000330000010000000000000000000000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 387) { //四季之神
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "00000183040B08008100000005DC003A000000000000000000000000000100190001A0B100000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E00000001000172A00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008100000005DC003A00000000000000000000000000010019000153D600000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E00000001000172A00000000000000000000000000000000000000000000000000000000000000000000000000000000100000001000000000000008100000005DC003A000000000000000000000000000100190001673500000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E00000001000172A00000000000000000000000000000000000000000000000000000000000000000000000000000000200000002000000000000008100000005DC003A0000000000000000000000000001001900013D8D00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01002E00000001000172A0000000000000000000000000000000000000000000000000000000000000000000000000000000030000000300000000000000810000000064003A000000000000000000000000000100190001A0B100000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000172A0000000000000000000000000000000000000000000000000000000000000000000000000000000040000000400000000000000810000000064003A00000000000000000000000000010019000153D600000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000172A0000000000000000000000000000000000000000000000000000000000000000000000000000000050000000500000000000000810000000064003A000000000000000000000000000100190001673500000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000172A0000000000000000000000000000000000000000000000000000000000000000000000000000000060000000600000000000000810000000064003A0000000000000000000000000001001900013D8D00000001000002D000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000172A000000000000000000000000000000000000000000000000000000000000000000000000000000007000000070000000000000000000000000000000000000000000000671286006723A97F0000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 388) { //四季之神
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "00000184047B08008F0000000000003A000000000000000000000000000100190001D75100000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E00023B38000001F40000000000000000000000000000000000000000000000000000000000000000000000000000000000008F0000000000003A000000000000000000000000000100190001D75100000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E000233D7000000050000000000000000000000000000000000000000000000000000000000000100000001000000000000008F0000000000003A000000000000000000000000000100190001E26E00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E00023B38000001F40000000000000000000000000000000000000000000000000000000000000200000002000000000000008F0000000000003A000000000000000000000000000100190001E26E00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E000233D7000000050000000000000000000000000000000000000000000000000000000000000300000003000000000000008F0000000000003A000000000000000000000000000100190001E70800000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E00023B38000001F40000000000000000000000000000000000000000000000000000000000000400000004000000000000008F0000000000003A000000000000000000000000000100190001E70800000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E000233D7000000050000000000000000000000000000000000000000000000000000000000000500000005000000000000008F0000000000003A000000000000000000000000000100190001ED9500000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E00023B38000001F40000000000000000000000000000000000000000000000000000000000000600000006000000000000008F0000000000003A000000000000000000000000000100190001ED9500000001FFFFFFFF00000000000000000000000000000000000000000000000000000000002CEA01003C000000010000000000000000000000000000000001000E000233D70000000500000000000000000000000000000000000000000000000000000000000007000000070000000000000000000000000000000000000000000000671286006723A97F0000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 392) { //四季之神
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "0000018803090600810000000032003A0000000000000000000000000001001900023B38000000050000000000000000000000000000000000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000810000000064003A0000000000000000000000000001001900023B38000000050000000000000000000000000000000000000000000000000000000000000000002CEA01002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000100000000000000810000000096003A0000000000000000000000000001001900023B38000000050000000000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000200000002000000000000008100000000C8003A0000000000000000000000000001001900023B380000000A0000000000000000000000000000000000000000000000000000000000000000002CEA01002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000300000003000000000000008100000000FA003A0000000000000000000000000001001900023B380000000A0000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000040000000000000081000000012C003A0000000000000000000000000001001900023B380000000A0000000000000000000000000000000000000000000000000000000000000000002CEA01002E000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005000000050000000000000000000000000000000000000000000000671286006723A97F0000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (Type == 194) { //四季之神
		if (!Uinew) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		Write32(p, Client->Uin); //Uin
		WriteHex(p, "000000C207920F00810000000064003A000000000000000000000000000100190001A73000000001000000000000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000100000000000000810000000064003A0000000000000000000000000001001900003E7600000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020000000200000000000000810000000064003A000000000000000000000000000100190000544B00000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000030000000300000000000000810000000064003A00000000000000000000000000010019000031B000000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000040000000400000000000000810000000064003A0000000000000000000000000001001900019EDD00000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000050000000500000000000000810000000064003A000000000000000000000000000100190001C00500000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000600000000000000810000000064003A000000000000000000000000000100190001C5BD00000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000070000000700000000000000810000000064003A000000000000000000000000000100190001D7BB00000001FFFFFFFF0000000000000000000001000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000080000000800000000000000810000000064003A000000000000000000000000000100190001002100000001000002D00000000000000000000000000000000000000000000000000000000001000001002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000090000000900000000000000810000000064003A000000000000000000000000000100190001023000000001000002D00000000000000000000000000000000000000000000000000000000001000001002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A0000000A00000000000000810000000064003A000000000000000000000000000100190001025800000001000002D00000000000000000000000000000000000000000000000000000000001000001002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000B0000000B00000000000000810000000064003A00000000000000000000000000010019000055D900000001000000000000000000000000000000000000000000000000000000000000000001000001002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C0000000C00000000000000810000000064003A000000000000000000000000000100190000305800000001000000000000000000000000000000000000000000000000000000000000000001000001002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000D0000000D00000000000000810000000064003A000000000000000000000000000100190000305900000001000000000000000000000000000000000000000000000000000000000000000001000001002E00010000000000000000000000000000000000000000000000000000000000000000000000000000673768B50000000E0000000E00000000000000810000000064003A000000000000000000000000000100190001A73100000001000000000000000000000000000000000000000000000000000000000000000001000001002E00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000F0000000F000000000000000000000000000000000005EB0000010000000000000000000000000000000000000000");
		len = p - buf.data();
		SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else {
		ResponsePreGetCommonBoxAwardElse(Client, Type, AwardID, IsShowAllAward, EchoClient);
	}
}

void ResponsePreGetCommonBoxAward(std::shared_ptr<ClientNode> Client, BOOL HasMore, USHORT Type, std::vector<ItemInfo>& MiracleItems, char MiracleItemNum)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	//BYTE* p = buf; size_t len;
	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write16(p, Type); //Type
	{ //AwardCfgArray
		BYTE* pAwardCfgArray = p;
		Write16(pAwardCfgArray, 0); //len

		Write8(pAwardCfgArray, MiracleItemNum); //ScoreAwardNum 
		for (size_t i = 0; i < MiracleItemNum; i++)
		{ //ScoreAwardCfg
			BYTE* pScoreAwardCfg = pAwardCfgArray;
			Write16(pScoreAwardCfg, 0); //len

			Write8(pScoreAwardCfg, 0); //Type
			Write32(pScoreAwardCfg, 0); //StartScore
			{ //AwardInfo
				BYTE* pAwardInfo = pScoreAwardCfg;
				Write16(pAwardInfo, 0); //len

				Write32(pAwardInfo, 0); //Exp
				Write32(pAwardInfo, 0); //Money
				Write32(pAwardInfo, 0); //SuperMoney
				Write16(pAwardInfo, 1); //ItemNum
				//for (size_t i = 0; i < 1; i++)
				{ //ItemInfo
					BYTE* pItemInfo = pAwardInfo;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, MiracleItems[i].ItemID); //ItemID
					Write32(pItemInfo, MiracleItems[i].ItemNum); //ItemNum
					Write32(pItemInfo, MiracleItems[i].AvailPeriod); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, MiracleItems[i].ItemType); //ItemType

					len = pItemInfo - pAwardInfo;
					Set16(pAwardInfo, (WORD)len);
					pAwardInfo += len;
				}

				Write32(pAwardInfo, 0); //Coupons
				Write32(pAwardInfo, 0); //GuildPoint
				Write32(pAwardInfo, 0); //LuckMoney

				Write8(pAwardInfo, 0); //ExtendInfoNum


				Write32(pAwardInfo, 0); //SpeedCoin

				len = pAwardInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}
			Write8(pScoreAwardCfg, 1);
			Write16(pScoreAwardCfg, 0); //ResultID
			Write8(pScoreAwardCfg, 1); //HasMoreInfo
			//for (size_t i = 0; i < 1; i++)
			{ //MoreInfo
				BYTE* pMoreInfo = pScoreAwardCfg;
				Write16(pMoreInfo, 0); //len

				Write16(pMoreInfo, 0); //AlreayGetNum
				Write16(pMoreInfo, 0); //MaxGetNum
				Write32(pMoreInfo, 0); //ItemID 
				Write32(pMoreInfo, 0); //TimeStart
				Write32(pMoreInfo, 0); //TimeEnd
				Write32(pMoreInfo, 0); //NextAddNumLeftTime
				Write8(pMoreInfo, 1); //Num
				//for (size_t i = 0; i < 1; i++)
				{ //ComsumeItemArray
					BYTE* pComsumeItemArray = pMoreInfo;
					Write16(pComsumeItemArray, 0); //len

					Write32(pComsumeItemArray, MiracleItems[i].ComsumeItemId); //ItemID
					Write32(pComsumeItemArray, MiracleItems[i].ComsumeItemNum); //StartScore 
					Write32(pComsumeItemArray, 0); //DiscountStartScore

					len = pComsumeItemArray - pMoreInfo;
					Set16(pMoreInfo, (WORD)len);
					pMoreInfo += len;
				}
				Write8(pMoreInfo, 0); //ClientParaNum
				//Write32(pMoreInfo, 0); //ClientParas[]
				Write32(pMoreInfo, 0); //DiscountTimeStart
				Write32(pMoreInfo, 0); //DiscountTimeEnd
				Write8(pMoreInfo, 0); //DiscountStatus
				Write32(pMoreInfo, 0); //LimitPara
				Write8(pMoreInfo, 0); //ItemExchangeType
				Write8(pMoreInfo, 0); //ItemLabelType
				Write16(pMoreInfo, 0); //ShowPriority
				Write8(pMoreInfo, 0); //Para0
				Write32(pMoreInfo, 0); //LastUpdateTime

				len = pMoreInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}

			Write32(pScoreAwardCfg, i); //PosKey
			Write32(pScoreAwardCfg, i); //AwardIdx
			Write8(pScoreAwardCfg, 0); //LabelStatus
			Write8(pScoreAwardCfg, 0); //ExtraAwardNum
			//Write32(pScoreAwardCfg, 0); 

			len = pScoreAwardCfg - pAwardCfgArray;
			Set16(pAwardCfgArray, (WORD)len);
			pAwardCfgArray += len;
		}
		len = pAwardCfgArray - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //AwardID
	Write32(p, 0); //NextClearLeftTime
	Write8(p, HasMore); //HasMoreInfo 
	Write32(p, 0); //Score
	Write16(p, 0); //LeftGetAwardNum
	Write8(p, 0); //Status
	Write8(p, 0); //EnableMergeSameItem
	Write32(p, 0); //TimeStart
	Write32(p, 0); //TimeEnd
	Write32(p, 0); //SCPara
	Write8(p, 0); //IsShowAllAward
	Write16(p, 0); //ValueNum
	//Write32(p, 0); //Values[]
	Write32(p, 0); //EchoClient

	len = p - buf.data();
	SendToClient(Client, 24465, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

void ResponsePreGetCommonBoxAwardElse(std::shared_ptr<ClientNode> Client, USHORT Type, int AwardID, UCHAR IsShowAllAward, int EchoClient)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write16(p, Type); //Type
	{ //AwardCfgArray
		BYTE* pAwardCfgArray = p;
		Write16(pAwardCfgArray, 0); //len

		Write8(pAwardCfgArray, 5); //ScoreAwardNum
		for (size_t i = 0; i < 5; i++)
		{ //ScoreAwardCfg
			BYTE* pScoreAwardCfg = pAwardCfgArray;
			Write16(pScoreAwardCfg, 0); //len

			Write8(pScoreAwardCfg, 1); //Type
			Write32(pScoreAwardCfg, (i + 1) * 4 + 4); //StartScore
			{ //AwardInfo
				BYTE* pAwardInfo = pScoreAwardCfg;
				Write16(pAwardInfo, 0); //len

				Write32(pAwardInfo, 0); //Exp
				Write32(pAwardInfo, 0); //Money
				Write32(pAwardInfo, (i + 1) * 500); //SuperMoney
				Write16(pAwardInfo, 0); //ItemNum
				for (size_t i = 0; i < 0; i++)
				{ //ItemInfo
					BYTE* pItemInfo = pAwardInfo;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, 0); //ItemID
					Write32(pItemInfo, 0); //ItemNum
					Write32(pItemInfo, 0); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, 0); //ItemType

					len = pItemInfo - pAwardInfo;
					Set16(pAwardInfo, (WORD)len);
					pAwardInfo += len;
				}

				Write32(pAwardInfo, 0); //Coupons
				Write32(pAwardInfo, 0); //GuildPoint
				Write32(pAwardInfo, 0); //LuckMoney

				Write8(pAwardInfo, 0); //ExtendInfoNum
				for (size_t i = 0; i < 0; i++)
				{ //ExtendInfoAward
					BYTE* pExtendInfoAward = pAwardInfo;
					Write16(pExtendInfoAward, 0); //len

					Write16(pExtendInfoAward, 0); //Key
					Write32(pExtendInfoAward, 0); //AddValue
					Write32(pExtendInfoAward, 0); //TotalValue
					Write8(pExtendInfoAward, 0); //ShowPriority
					Write16(pExtendInfoAward, 0); //BuyNeedScore
					Write8(pExtendInfoAward, 0); //OneMatchMaxNum

					len = pExtendInfoAward - pAwardInfo;
					Set16(pAwardInfo, (WORD)len);
					pAwardInfo += len;
				}

				Write32(pAwardInfo, 0); //SpeedCoin

				len = pAwardInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}
			Write8(pScoreAwardCfg, 0); //Status
			Write16(pScoreAwardCfg, 0); //ResultID
			Write16(pScoreAwardCfg, 0); //HasMoreInfo
			/*
			for (size_t i = 0; i < n; i++)
			{ //MoreInfo
				BYTE* pMoreInfo = pScoreAwardCfg;
				Write16(pMoreInfo, 0); //len

				Write16(pMoreInfo, 0); //AlreayGetNum
				Write16(pMoreInfo, 0); //MaxGetNum
				Write32(pMoreInfo, 0); //ItemID
				Write32(pMoreInfo, 0); //TimeStart
				Write32(pMoreInfo, 0); //TimeEnd
				Write32(pMoreInfo, 0); //NextAddNumLeftTime
				Write8(pMoreInfo, 0); //Num
				for (size_t i = 0; i < n; i++)
				{ //ComsumeItemArray
					BYTE* pComsumeItemArray = pMoreInfo;
					Write16(pComsumeItemArray, 0); //len

					Write32(pComsumeItemArray, 0); //ItemID
					Write32(pComsumeItemArray, 0); //StartScore
					Write32(pComsumeItemArray, 0); //DiscountStartScore

					len = pComsumeItemArray - pMoreInfo;
					Set16(pMoreInfo, (WORD)len);
					pMoreInfo += len;
				}
				Write8(pMoreInfo, 0); //ClientParaNum
				Write32(pMoreInfo, 0); //ClientParas[]
				Write32(pMoreInfo, 0); //DiscountTimeStart
				Write32(pMoreInfo, 0); //DiscountTimeEnd
				Write8(pMoreInfo, 0); //DiscountStatus
				Write32(pMoreInfo, 0); //LimitPara
				Write8(pMoreInfo, 0); //ItemExchangeType
				Write8(pMoreInfo, 0); //ItemLabelType
				Write16(pMoreInfo, 0); //ShowPriority
				Write8(pMoreInfo, 0); //Para0
				Write32(pMoreInfo, 0); //LastUpdateTime

				len = pMoreInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}
			*/

			Write32(pScoreAwardCfg, 0); //PosKey
			Write8(pScoreAwardCfg, 0); //AwardIdx
			Write8(pScoreAwardCfg, 0); //LabelStatus

			Write8(pScoreAwardCfg, 0); //ExtraAwardNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtraAwardInfo
				BYTE* pExtraAwardInfo = pScoreAwardCfg;
				Write16(pExtraAwardInfo, 0); //len

				Write32(pExtraAwardInfo, 0); //Exp
				Write32(pExtraAwardInfo, 0); //Money
				Write32(pExtraAwardInfo, 0); //SuperMoney
				Write16(pExtraAwardInfo, 0); //ItemNum
				for (size_t i = 0; i < 0; i++)
				{ //ItemInfo
					BYTE* pItemInfo = pExtraAwardInfo;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, 0); //ItemID
					Write32(pItemInfo, 0); //ItemNum
					Write32(pItemInfo, 0); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, 0); //ItemType

					len = pItemInfo - pExtraAwardInfo;
					Set16(pExtraAwardInfo, (WORD)len);
					pExtraAwardInfo += len;
				}
				Write32(pExtraAwardInfo, 0); //Coupons
				Write32(pExtraAwardInfo, 0); //GuildPoint
				Write32(pExtraAwardInfo, 0); //LuckMoney

				Write8(pExtraAwardInfo, 0); //ExtendInfoNum
				for (size_t i = 0; i < 0; i++)
				{ //ExtendInfoAward
					BYTE* pExtendInfoAward = pExtraAwardInfo;
					Write16(pExtendInfoAward, 0); //len

					Write16(pExtendInfoAward, 0); //Key
					Write32(pExtendInfoAward, 0); //AddValue
					Write32(pExtendInfoAward, 0); //TotalValue
					Write8(pExtendInfoAward, 0); //ShowPriority
					Write16(pExtendInfoAward, 0); //BuyNeedScore
					Write8(pExtendInfoAward, 0); //OneMatchMaxNum

					len = pExtendInfoAward - pExtraAwardInfo;
					Set16(pExtraAwardInfo, (WORD)len);
					pExtraAwardInfo += len;
				}

				Write32(pExtraAwardInfo, 0); //SpeedCoin

				len = pExtraAwardInfo - pScoreAwardCfg;
				Set16(pScoreAwardCfg, (WORD)len);
				pScoreAwardCfg += len;
			}

			len = pScoreAwardCfg - pAwardCfgArray;
			Set16(pAwardCfgArray, (WORD)len);
			pAwardCfgArray += len;
		}
		len = pAwardCfgArray - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //AwardID
	Write32(p, 0); //NextClearLeftTime
	Write8(p, 0); //HasMoreInfo
	Write32(p, 0); //Score
	Write16(p, 0); //LeftGetAwardNum
	Write8(p, 0); //Status
	Write8(p, 0); //EnableMergeSameItem
	Write32(p, 0); //TimeStart
	Write32(p, 0); //TimeEnd
	Write32(p, 0); //SCPara
	Write8(p, 0); //IsShowAllAward
	Write16(p, 0); //ValueNum
	//Write32(p, 0); //Values[]
	Write32(p, 0); //EchoClient

	len = p - buf;
	SendToClient(Client, 24465, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}


void RequestAddToMatchQueue(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{//请求加入匹配队列？
	BYTE* p = Body;;

	UINT uin = Read32(p);
	UINT time = Read32(p);

	USHORT matchQueueType = Read16(p);
	UINT para1 = Read32(p);
	UCHAR IsAllArea = Read8(p);
	UCHAR paraNum = Read8(p);
	for (size_t i = 0; i < paraNum; i++)
	{
		UINT para = Read32(p);
	}
	int MapID = Read32(p);

	//ResponseAddToMatchQueue(Client, matchQueueType);

}

void ResponseAddToMatchQueue(std::shared_ptr<ClientNode> Client, USHORT matchQueueType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //resultId
	Write8(p, 0); //reasonLen
	Write16(p, matchQueueType); //matchQueueType
	Write32(p, 0); //Para0
	Write8(p, 0); //IsAllArea
	Write32(p, 0); //MapID

	len = p - buf.data();
	SendToClient(Client, 20079, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyMatchResult(std::shared_ptr<ClientNode> Client, USHORT matchQueueType)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //resultId
	Write16(p, 0); //reasonLen
	Write8(p, matchQueueType); //matchQueueType 0xa是排位队列
	Write8(p, 0); //hasEnterInfo
	/*for (size_t i = 0; i < 1; i++)
	{ //enterInfo
		BYTE* penterInfo = p;
		Write16(penterInfo, 0); //len 0x42

		{ //addrInfo
			BYTE* paddrInfo = penterInfo;
			Write16(paddrInfo, 0); //len 0x3a

			Write16(paddrInfo, Client->ServerID); //svrId
			Write32(paddrInfo, inet_addr(IP.c_str())); //svrIP 0xCE5C6C71(4字节整数形式转换ip 注意大小头)
			Write8(paddrInfo, 1); //tcpPortNum 0x3
			Write16(paddrInfo, TcpPort); //tcpPort[] 0x274a 0x274b 0x274c
			Write8(paddrInfo, 1); //udpPortNum 0x1
			Write16(paddrInfo, UdpPort); //udpPort[] 0x274a
			Write8(paddrInfo, 0); //HasMultiIPInfo 0x1
			for (size_t i = 0; i < 0; i++)
			{ //MultiIPInfo
				BYTE* pMultiIPInfo = paddrInfo;
				Write16(pMultiIPInfo, 0); //len 0x27

				Write8(pMultiIPInfo, 0); //IPCount 0x4
				Write32(pMultiIPInfo, 0); //ServerIP[]0xce 5c 6c 71 0xe4 36 f1 3d 0x9e 03 35 70 0x77 e8 f4 6d
				Write8(pMultiIPInfo, 0); //DomainLen 0x13
				//0x64 78 32 70 76 70 2e 73 70 65 65 64 2e 71 71 2e 63 6f 6d
				len = pMultiIPInfo - paddrInfo;
				Set16(paddrInfo, (WORD)len);
				paddrInfo += len;
			}

			len = paddrInfo - penterInfo;
			Set16(penterInfo, (WORD)len);
			penterInfo += len;
		}
		Write32(penterInfo, Client->BorderRoomID); //roomId 0x00 00 02 eb
		Write16(penterInfo, 0); //SceneID 0x0

		len = penterInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}*/
	//00 00 00 ff 00 01 00 00 01 2e 53 40 ce 01 10 9d 8a 04 64 b2 56 00 00 00 01 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 6b 61 
	Write32(p, 0); //mapID 0x00 00 00 ff
	Write16(p, 1); //MatchedRoomType 0x00 01
	Write16(p, 0); //TeamMatchType 0x00 00
	Write8(p, 0); //PVPType 0x01
	Write32(p, 0); //PVPKey 0x2e 53 40 ce
	Write32(p, 12345678); //GameSeqIDHigh 0x01 10 9d 8a
	Write32(p, 87654321); //GameSeqIDLow 0x04 64 b2 56
	Write16(p, 0); //ForceTransCount0x00 00
	//Write32(p, 0); //ForceTransMsgID[]
	Write16(p, 1); //PlayerMatchingType 0x00 01
	Write8(p, 4); //paraNum
	Write32(p, 0);
	Write32(p, 0);
	Write32(p, 0);
	Write32(p, 0);
	//Write32(p, 0); //paraList[]

	len = p - buf.data();
	SendToClient(Client, 20081, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyMatchResultLogin(std::shared_ptr<ClientNode> Client)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	WriteHex(p, "0000000008010047003F0009AD4D0E790127180001003204AD4D0E79EC35F13D8F013570C1E8F46D1E6368617474696E672E73706565642E71712E636F6D2F6368617474696E6700000000000000000000000000070300000000028775F283D93C36000000070162E69B4E");

	len = p - buf.data();
	SendToClient(Client, 20081, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void RequestLeaveGame(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	//请求退出游戏
	UINT uin = Read32(Body);
	UINT time = Read32(Body);

	UINT GameSeqIDHigh = Read32(Body);
	UINT GameSeqIDLow = Read32(Body);
	USHORT LeaveGameType = Read16(Body);

	if (LeaveGameType == 3 && Client->TaskID == 0xFFF1) {
		//请求退出打榜
		ResponseLeaveGame(Client);
		NotifyRaceShow(Client);
		NotifyG2CChallengeResult(Client, 4, -1);
		/*if (Client->ChallengeMaps.size() == 1) {
			UpdateChallengeMapStatus(Client->ChallengeMaps[0].MapID, Client->ChallengeMaps[0].RankID, 0);
		}*/
	}
	//UCHAR ParaNum = Read8(Body);
	//int[] ParaList[] = Read32(Body);
	//int ParaList[1];
	//ParaList[0] = Client->MapID;
	//ResponseLeaveGame(Client);
	NotifyGameOver(Client, 0, 0, 0);
	if (Client->RoomID != 0)
	{
		std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
		if (Room)
		{
			if (Room->CurrentPlayerNum == 1 && Client->TaskID == 0)
			{//单人退出直接到房间
				if (Room->ChatRoomType == 6)
				{
					//进入3d房间
					ResponseCreateRoom(Client, Room, 6);
					RequestEnter3DRoom(Client);

					//NotifyChangeRoomOwner(Client, Room);
				}
				Room->Callback = NULL;
			}
			else
			{//多人退出到大厅
				if (Room->ChatRoomType == 6)
				{
					//退出3d房间
					ResponseCreateRoom(Client, Room, 0);
				}
				//LeaveOutdoor(Client);
				LeaveRoom(Client, 0);
			}
		}
	}
	else
	{
		LeaveBorderRoom(Client, 0);
	}
	Client->IsFinish = 0;
	Client->IsReady = 0;

}

void ResponseLeaveGame(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID

	len = p - buf.data();
	SendToClient(Client, 424, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Response);
}


void NotifyRandRoomNameList(std::shared_ptr<ClientNode> Client)
{

	/*std::string Byte = "0800260F392E323820CBCD36383838B5E3C8AFFE13000000100000000000000000000000000000002811392E323820CBCD34383838CFFBB7D1C8AFFE1300000010000000000000000000000000000000260F392E323820CBCDB7E7D6AEC0FBC8D0FE1300000010000000000000000000000000000000250E392E32382036B1B6BFAAC6F4A3A1FE13000000100000000000000000000000000000002D16BFECC0B4BCD3C8EBCED2B5C4B6D3CEE9B0C9A3A1A3A10000000000400000000000000000000000000000002912D0D6B5DCC6EBD0C4A3ACC6E4C0FBB6CFBDF00000000000400000000000000000000000000000002912B4F3BCD2D2BBC6F0CCF4D5BDBCABCFDEA3A10000000000400000000000000000000000000000002710CBABC8CBB8FCBFECB8FCC7BFC5B6A3A1000000000040000000000000000000000000000006001F08D2D4B3B5BBE1D3D1000000000000000000000000000000000000000000210ABDBBB8F6C5F3D3D1B0C9000000000000000000000000000000000000000000210AB4F3BCD2D2BBC6F0CDE6000000000000000000000000000000000000000000230CC0CFCBBEBBFAA3ACC9CFB3B50000000000000000000000000000000000000000001F08D3D1D2EAB5DAD2BB000000000000000000000000000000000000000000230CC1C4C1C4CCECC5DCC5DCB3B500000000000000000000000000000000000000000600230CCFEBD2AACCF4D5BDB8DFCAD6000000000000000000000000000000000000000000210AC7D0B4E8BEBABCBCA3A1000000000000000000000000000000000000000000230CCBD9B6C8B5C4B6D4BEF6A3A1000000000000000000000000000000000000000000230CBFB4BFB4CBADB8FCBFECA3A1000000000000000000000000000000000000000000210AC7EFC3FBC9BDB5C8C4E3000000000000000000000000000000000000000000250ECBD9B6C8CAC7CED2B5C4C8D9D2AB000000000000000000000000000000000000000004002710D2BBC6F0CDEAB3C9C8D5B3A3C8CECEF10000000000000000000000000000000000000000002710D5D2C8CECEF1B4EED7D3D2BBC6F0CDE6000000000000000000000000000000000000000000250EBACFD7F7CDEAB3C9C8CECEF1D1BD0000000000000000000000000000000000000000001F08C8CECEF1B3B544440000000000000000000000000000000000000000";
	const unsigned char* Msg760 = EHexToByte(Byte.data());
	SendToClient(Client, 760, (const BYTE*)Msg760, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	delete[] Msg760;
	return;*/

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	const char* RandRoomNameList[] = { "RandRoomName A","RandRoomName B","RandRoomName C" };
	BYTE Count = sizeof(RandRoomNameList) / sizeof(RandRoomNameList[0]);
	Write8(p, Count);
	for (BYTE i = 0; i < Count; i++)
	{ //RoomNames
		BYTE* pRoomNames = p;
		Write16(pRoomNames, 0); //len

		len = strlen(RandRoomNameList[i]);
		Write8(pRoomNames, (BYTE)len); //NameLen
		memcpy(pRoomNames, RandRoomNameList[i], len);
		pRoomNames += len;

		for (size_t i = 0; i < 20; i++)
		{
			Write8(pRoomNames, 0); //BaseModes[]
		}

		len = pRoomNames - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 760, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyTopListDesc(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	USHORT Count = TopListDescs.size();
	Write16(p, Count);
	for (USHORT i = 0; i < Count; i++)
	{ //Descs
		BYTE* pDescs = p;
		Write16(pDescs, 0); //len
		Write16(pDescs, TopListDescs[i].ID);

		memset(pDescs, 0, MaxNickName);
		memcpy(pDescs, TopListDescs[i].Name, MaxNickName);
		pDescs += MaxNickName;

		memset(pDescs, 0, MaxNickName);
		memcpy(pDescs, TopListDescs[i].ValueDesc, MaxNickName);
		pDescs += MaxNickName;

		memset(pDescs, 0, MaxNickName);
		memcpy(pDescs, TopListDescs[i].TitleName, MaxNickName);
		pDescs += MaxNickName;

		Write8(pDescs, 0); //HasOtherInfo
		/*
		{ //Other
			BYTE* pOther = pDescs;
			Write16(pOther, 0); //len

			//WriteString(pOther, 0); //IntParaDesc0[]
			//WriteString(pOther, 0); //IntParaDesc1[]
			//WriteString(pOther, 0); //StrParaDesc0[]
			//WriteString(pOther, 0); //StrParaDesc1[]

			len = pOther - pDescs;
			Set16(pDescs, (WORD)len);
			pDescs += len;
		}
		*/

		Write8(pDescs, TopListDescs[i].IsEnableSelfRank);
		len = pDescs - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 701, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseRankTopList(std::shared_ptr<ClientNode> Client, USHORT ID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	//BYTE* p = buf; size_t len;
	

	int playercount = 0;
	std::string NickName = "";

	UINT GradeScore = 0;
	UINT GradeLevel = 0;
	UINT ChildLevel = 0;
	UINT TotalScore = 0;
	Write32(p, Client->Uin); //Uin
	Write16(p, ID); //ID

	BYTE* pPlayerCount = p;
	Write16(p, 0); //Count
	//多表联合查询
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		//std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT R.Uin,R.TotalScore,B.NickName FROM RankInfo R join BaseInfo B on R.Uin=B.Uin order by R.GradeLevel DESC,R.ChildLevel ASC,R.GradeScore DESC limit 0,30;"));
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT R.Uin,R.TotalScore,B.NickName FROM RankInfo R join BaseInfo B on R.Uin=B.Uin order by R.TotalScore DESC limit 0,30;"));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			playercount++;
			NickName = UTF2GB(res->getString("NickName").c_str());
			
			BYTE* pPlayer = p;
			Write16(pPlayer, 0); //len
			Write32(pPlayer, res->getUInt("Uin")); //Uin
			memset(pPlayer, 0, MaxNickName);
			if (!NickName.empty())
			{
				memcpy(pPlayer, NickName.c_str(), NickName.length());
			}
			pPlayer += MaxNickName;
			Write32(pPlayer, res->getUInt("TotalScore")); //Value

			Write8(pPlayer, 0); //HasOtherInfo
			Write8(pPlayer, 0); //AreaId
			Write8(pPlayer, 0); //ParaNum
			Write8(pPlayer, 0); //AnchorNum
			len = pPlayer - p;
			Set16(p, (WORD)len);
			p += len;


		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("ResponseRankTopList error\n");
	}

	Set16(pPlayerCount, playercount);
	Write32(p, 0); //SelfRank
	Write32(p, 0); //BeginRank
	Write32(p, 0); //UperPercentage

	len = p - buf.data();
	SendToClient(Client, 461, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}


void RequestTopList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT ID = Read16(p);
	UCHAR Type = Read8(p);
	//printf("ID:%d\n", ID);
	if (ID == 0x82)
	{
		//排位车神榜
		ResponseRankTopList(Client, ID);
	}
	else
	{
		ResponseTopList(Client, ID, Type);
	}

}

void ResponseTopList(std::shared_ptr<ClientNode> Client, USHORT ID, UCHAR Type)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	//BYTE* p = buf; size_t len;

	const char* sql = NULL;

	int playercount = 0;
	std::string NickName = "";

	Write32(p, Client->Uin); //Uin
	Write16(p, ID); //ID
	//printf("%d\n", ID);
	BYTE* pPlayerCount = p;
	Write16(p, 0); //Count
	if (ID == 3)//荣誉红人
	{
		sql = "SELECT Uin,NickName,TotalHonor FROM BaseInfo order by TotalHonor DESC limit 0,30;";
	}
	else if (ID == 177)//赛车红人
	{
		//sql = "select I.Uin as Uin,B.NickName as NickName,count(I.ItemType) as KartNum  From Item I Join BaseInfo B on I.Uin=B.Uin where ItemType=1 group by I.Uin order by KartNum DESC limit 0,30;";
		sql = "select a.Uin ,B.NickName as NickName, count(1) as KartNum from kartrefit a Join BaseInfo b where a.uin = b.uin group by a.Uin  order by KartNum DESC limit 0,30;";
	}
	else if (ID == 1)//等级红人
	{
		sql = "SELECT Uin,NickName,Experience FROM BaseInfo order by Experience DESC limit 0,30;";
	}
	else if (ID == 31)//舞蹈
	{
		sql = "SELECT Uin,NickName,DanceExp FROM BaseInfo order by DanceExp DESC limit 0,30;";
	}
	else if (ID == 10 || ID == 11)//喇叭红人
	{
		sql = "SELECT Uin,NickName,BugletTalk FROM BaseInfo order by BugletTalk DESC limit 0,30;";
	}
	else if (ID == 45)//车队贵族红人
	{
		sql = "SELECT P.Uin,B.NickName,P.GuildVipPoint from Player P join BaseInfo B on B.Uin=P.Uin order by P.GuildVipPoint DESC limit 0,30;";
	}
	else if (ID == 43)//皇族红人
	{
		sql = "SELECT P.Uin,B.NickName,P.NoblePoint from Player P join BaseInfo B on B.Uin=P.Uin order by P.NoblePoint DESC limit 0,30;";
	}
	else if (ID == 53)//情侣贵族红人
	{
		sql = "SELECT P.Uin,B.NickName,P.LoverVipPoint from Player P join BaseInfo B on B.Uin=P.Uin order by P.LoverVipPoint DESC limit 0,30;";
	}
	else if (ID == 58)//钻皇红人
	{
		sql = "SELECT P.Uin,B.NickName,P.EmperorPoint from Player P join BaseInfo B on B.Uin=P.Uin order by P.EmperorPoint DESC limit 0,30;";
	}
	else if (ID == 57)//后花园红人
	{
		sql = "SELECT P.Uin,B.NickName,P.GardenPoint from Player P join BaseInfo B on B.Uin=P.Uin order by P.GardenPoint DESC limit 0,30;";
	}
	else if (ID == 76)//车队荣誉
	{
		sql = "SELECT G.Uin,B.NickName,G.MemProsperity from GuildInfo G join BaseInfo B on B.Uin=G.Uin order by G.MemProsperity DESC limit 0,30;";
	}
	else if (ID == 74)//赛季战勋
	{
		sql = "SELECT B.Uin,B.NickName,I.ItemNum from BaseInfo B join Item I on B.Uin=I.Uin And I.ItemID = 100969 order by I.ItemNum DESC limit 0,30;";
	}
	else if (ID == 37)//竞技红人
	{
		sql = "SELECT B.Uin,B.NickName,I.ItemNum from BaseInfo B join Item I on B.Uin=I.Uin And I.ItemID = 106614 order by I.ItemNum DESC limit 0,50;";
	}
	else//魅力红人
	{
		sql = "SELECT Uin,NickName,Charm FROM BaseInfo order by Charm DESC limit 0,30;";
	}


	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));

		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			playercount++;
			NickName = UTF2GB(res->getString("NickName").c_str());
			
			BYTE* pPlayer = p;
			Write16(pPlayer, 0); //len
			Write32(pPlayer, res->getUInt("Uin")); //Uin
			memset(pPlayer, 0, MaxNickName);
			if (!NickName.empty())
			{
				memcpy(pPlayer, NickName.c_str(), NickName.length());
			}
			pPlayer += MaxNickName;
			Write32(pPlayer, res->getUInt(3)); //Value

			Write8(pPlayer, 0); //HasOtherInfo
			Write8(pPlayer, 0); //AreaId
			Write8(pPlayer, 0); //ParaNum
			Write8(pPlayer, 0); //AnchorNum
			len = pPlayer - p;
			Set16(p, (WORD)len);
			p += len;
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("ResponseTopList error\n");
		//throw;关闭
	}

	Set16(pPlayerCount, playercount);
	Write32(p, 0); //SelfRank
	Write32(p, 0); //BeginRank
	Write32(p, 0); //UperPercentage

	len = p - buf.data();
	SendToClient(Client, 461, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);


}
void ResponsePointChallengeOpenMainBoard(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write32(p, 0); //TodayHighestTongGuanNum
	Write32(p, 0); //TodayHighestScore
	Write32(p, 0); //TotayTotalScore
	Write32(p, 5); //TiaoZhanNum
	Write32(p, 5); //TiaoZhanMaxNum
	Write32(p, 0); //GetTiaoZhanLeftSeconds
	Write32(p, 0); //HistoryHighestTongGuanNum
	Write32(p, 0); //HistoryHighestScore
	{ //AwardArray
		BYTE* pAwardArray = p;
		Write16(pAwardArray, 0); //len
		//道具key：586复活卡 588增时卡 590最后一搏
		Write8(pAwardArray, 3); //ExtendInfoNum
		{ //ExtendInfoAward
			BYTE* pExtendInfoAward = pAwardArray;
			Write16(pExtendInfoAward, 0); //len

			Write16(pExtendInfoAward, 586); //Key
			Write32(pExtendInfoAward, 90); //AddValue
			Write32(pExtendInfoAward, 90); //TotalValue
			Write8(pExtendInfoAward, 1); //ShowPriority
			Write16(pExtendInfoAward, 0); //BuyNeedScore
			Write8(pExtendInfoAward, 90); //OneMatchMaxNum

			len = pExtendInfoAward - pAwardArray;
			Set16(pAwardArray, (WORD)len);
			pAwardArray += len;
		}
		{ //ExtendInfoAward
			BYTE* pExtendInfoAward = pAwardArray;
			Write16(pExtendInfoAward, 0); //len

			Write16(pExtendInfoAward, 588); //Key
			Write32(pExtendInfoAward, 0); //AddValue
			Write32(pExtendInfoAward, 0); //TotalValue
			Write8(pExtendInfoAward, 0); //ShowPriority
			Write16(pExtendInfoAward, 0); //BuyNeedScore
			Write8(pExtendInfoAward, 0); //OneMatchMaxNum

			len = pExtendInfoAward - pAwardArray;
			Set16(pAwardArray, (WORD)len);
			pAwardArray += len;
		}
		{ //ExtendInfoAward
			BYTE* pExtendInfoAward = pAwardArray;
			Write16(pExtendInfoAward, 0); //len

			Write16(pExtendInfoAward, 590); //Key
			Write32(pExtendInfoAward, 0); //AddValue
			Write32(pExtendInfoAward, 0); //TotalValue
			Write8(pExtendInfoAward, 0); //ShowPriority
			Write16(pExtendInfoAward, 0); //BuyNeedScore
			Write8(pExtendInfoAward, 0); //OneMatchMaxNum

			len = pExtendInfoAward - pAwardArray;
			Set16(pAwardArray, (WORD)len);
			pAwardArray += len;
		}
		len = pAwardArray - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //WeekTotalScore
	Write32(p, 0); //TiaoZhanNum_TodayRecover
	Write32(p, 0); //TiaoZhanNum_MaxTodayRecoverNum
	Write32(p, 0); //GetTiaoZhanTotalSeconds

	len = p - buf.data();
	SendToClient(Client, 24754, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestReportTimerChallengeEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT EventID = Read32(p);
	//printf("ChallengeEvent:%d \n", EventID);
}
void NotifyBorderGameCondition(std::shared_ptr<ClientNode> Client)
{
	const char* buf1 = "\x00\x0d\x00\x00\x27\x11\x64\x75\xda\xcf\x00\x00\x00\x00\xcf\x00\xcb\x44\x69\x73\x70\x61\x74\x63\x68\x43\x6f\x52\x65\x73\x70\x6f\x6e\x73\x65\x00\x00\xf6\x00\x00\x00\x00\x00\x00\x2a\x40\xfd\x00\x00\x00\xa9\xfa\x0c\x70\x6f\x77\x65\x72\x5f\x62\x6c\x65\x76\x65\x6c\xf6\x00\x00\x00\x00\x00\x00\x14\x40\xfa\x04\x6e\x61\x6d\x65\xfa\x0c\xb1\xdf\xbe\xb3\xc8\xd9\xd3\xfe\xcc\xd8\xc8\xa8\xfa\x0b\x73\x69\x6d\x70\x6c\x65\x5f\x64\x65\x73\x63\xfa\x14\xb1\xdf\xbe\xb3\xb6\xd4\xbe\xd6\xbb\xf1\xb5\xc3\xb6\xee\xcd\xe2\xc8\xd9\xd3\xfe\xfa\x06\x73\x74\x61\x74\x75\x73\xf6\x00\x00\x00\x00\x00\x00\x00\x00\xfa\x04\x64\x65\x73\x63\xfa\x42\xc3\xbf\xbe\xd6\xb1\xdf\xbe\xb3\xb1\xc8\xc8\xfc\xbb\xf1\xb5\xc3\xb6\xee\xcd\xe2\xb5\xe3\xc8\xd9\xd3\xfe\xa3\xac\xcf\xed\xca\xdc\xb6\xe0\xb1\xb6\xbc\xd3\xb3\xc9\x28\xb3\xf5\xbc\xb6\x2b\x35\xa3\xac\xb8\xdf\xbc\xb6\x2b\x31\x30\xa3\xac\xd7\xa8\xd2\xb5\x2b\x31\x35\x29\x00\x00";
	const char* buf2 = "\x00\x18\x00\x00\x00\x00\x00\x0a\x00\x1e\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x00\x00\x00\x00\x00\x18\x00\x00\x00\x01\x00\x64\x00\x41\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x00\x00\x03\xe8\x00\x18\x00\x00\x00\x02\x00\x0a\x00\x1e\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x00\x00\x00\x00\x00\x18\x00\x00\x00\x03\x00\x64\x00\x41\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x00\x00\x03\xe8\x00\x18\x00\x00\x00\x04\x01\x2c\x00\x5a\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x00\x4e\x20\x00\x18\x00\x00\x00\x05\x01\x2c\x00\x5a\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x00\x4e\x20\x00\x00\x00\x00\x00\x00\x0e\x00\x00\x00\x00\x00\x0a\x00\x1e\x00\x00\x00\x00\x00\x0e\x00\x00\x00\x01\x00\x64\x00\x41\x00\x00\x03\xe8\x00\x0e\x00\x00\x00\x02\x00\x0a\x00\x1e\x00\x00\x00\x00\x00\x0e\x00\x00\x00\x03\x00\x64\x00\x41\x00\x00\x03\xe8\x00\x0e\x00\x00\x00\x04\x01\x2c\x00\x5a\x00\x00\x4e\x20\x00\x0e\x00\x00\x00\x05\x01\x2c\x00\x5a\x00\x00\x4e\x20\x00\x0a\x64\x5d\x11\x00\x64\xb6\xb6\xff\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x00\x00\x3c\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\xbb\x01\x00\x5c\x00\x0f\x00\x01\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x01\x00\x0f\x00\x01\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x01\x00\x0f\x00\x01\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x01\x00\x0f\x00\x01\x00\x0a\x00\x00\x00\x00\x00\x03\x99\xb7\x01\x00\x0f\x00\x01\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x00\x0f\x00\x01\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x00\x5c\x00\x0f\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x00\x0f\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x00\x0f\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x00\x0f\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x00\x0f\x00\x01\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x00\x0f\x00\x01\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x00\x0e\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x0e\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x0e\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x0e\x00\x01\x00\x0a\x00\x01\xe0\x78\x00\x03\x67\x18\x00\x0e\x00\x01\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x0e\x00\x01\x00\x0a\x00\x02\x22\xe0\x00\x03\x34\x50\x00\x1e\x00\x1a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x2f\xe4\x00\x00\x2e\xb6\x00\x00\x33\xff\x00\x00\x3e\x74\x00\x00\x2f\x0a\x00\x00";
	SendToClient(Client, 24251, (BYTE*)buf1, 222, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	SendToClient(Client, 544, (BYTE*)buf2, 628, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);

}


void ResponseGetRankedMatchShowInfo(std::shared_ptr<ClientNode> Client)
{
	if (ranknoticeon == 1) {
		ResponseStartGame(Client, ranknoticetext.c_str());
	}

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write32(p, 45); //SeasonID
	Write16(p, 2); //DailyOpenTimeInfoNums
	{ // OpenTimeInfos
		BYTE* pOpenTimeInfos = p;
		Write16(pOpenTimeInfos, 0); // len

		Write32(pOpenTimeInfos, 90000); // OpenTime
		Write32(pOpenTimeInfos, 240000); // CloseTime

		len = pOpenTimeInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // OpenTimeInfos
		BYTE* pOpenTimeInfos = p;
		Write16(pOpenTimeInfos, 0); // len

		Write32(pOpenTimeInfos, 0); // OpenTime
		Write32(pOpenTimeInfos, 20000); // CloseTime

		len = pOpenTimeInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 20240311); // SeasonOpenDate
	Write32(p, 20240428); // SeasonCloseDate
	{ //RankedMatchGradeInfo
		BYTE* pRankedMatchGradeInfo = p;
		Write16(pRankedMatchGradeInfo, 0); //len

		Write32(pRankedMatchGradeInfo, RankGradeLevel[Client->GradeLevel]); //GradeLevel
		Write32(pRankedMatchGradeInfo, Client->ChildLevel); //ChildLevel
		Write32(pRankedMatchGradeInfo, Client->GradeScore); //GradeScore
		Write32(pRankedMatchGradeInfo, getMaxScorce(RankGradeLevel[Client->GradeLevel])); //MaxGradeScore
		Write32(pRankedMatchGradeInfo, Client->TotalScore); //TotalScore

		len = pRankedMatchGradeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // NextLevelReceAwardInfo
		BYTE* pNextLevelReceAwardInfo = p;
		Write16(pNextLevelReceAwardInfo, 0); // len

		{ // AwardInfo
			BYTE* pAwardInfo = pNextLevelReceAwardInfo;
			Write16(pAwardInfo, 0); // len

			Write32(pAwardInfo, 0); // Exp
			Write32(pAwardInfo, 0); // Money
			Write32(pAwardInfo, 4000); // SuperMoney
			Write16(pAwardInfo, 1);	  // ItemNum
			{ // ItemInfo
				BYTE* pItemInfo = pAwardInfo;
				Write16(pItemInfo, 0); // len

				Write32(pItemInfo, 138189); // ItemID
				Write32(pItemInfo, 1);	   // ItemNum
				Write32(pItemInfo, 4294967295);	   // AvailPeriod
				Write8(pItemInfo, 0);	   // Status
				Write32(pItemInfo, 1710475394);	   // ObtainTime
				Write32(pItemInfo, 0);	   // OtherAttribute
				Write16(pItemInfo, 0);	   // ItemType

				len = pItemInfo - pAwardInfo;
				Set16(pAwardInfo, (WORD)len);
				pAwardInfo += len;
			}
			Write32(pAwardInfo, 0);	  // Coupons
			Write32(pAwardInfo, 0); // GuildPoint
			Write32(pAwardInfo, 0);  // LuckMoney
			Write8(pAwardInfo, 0);	  // ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ // ExtendInfoAward
				BYTE* pExtendInfoAward = pAwardInfo;
				Write16(pExtendInfoAward, 0); // len

				Write16(pExtendInfoAward, 0); // Key
				Write32(pExtendInfoAward, 0); // AddValue
				Write32(pExtendInfoAward, 0); // TotalValue
				Write8(pExtendInfoAward, 0);  // ShowPriority
				Write16(pExtendInfoAward, 0); // BuyNeedScore
				Write8(pExtendInfoAward, 0);  // OneMatchMaxNum

				len = pExtendInfoAward - pAwardInfo;
				Set16(pAwardInfo, (WORD)len);
				pAwardInfo += len;
			}
			Write32(pAwardInfo, 0); // SpeedCoin

			len = pAwardInfo - pNextLevelReceAwardInfo;
			Set16(pNextLevelReceAwardInfo, (WORD)len);
			pNextLevelReceAwardInfo += len;
		}
		Write32(pNextLevelReceAwardInfo, 24);   // GradeLevel
		Write32(pNextLevelReceAwardInfo, 5);   // ChildLevel
		Write32(pNextLevelReceAwardInfo, 30); // FinishMatchNums

		len = pNextLevelReceAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //SeasonAwardInfo
		BYTE* pSeasonAwardInfo = p;
		Write16(pSeasonAwardInfo, 0); //len

		Write32(pSeasonAwardInfo, 0); //Exp
		Write32(pSeasonAwardInfo, 0); //Money
		Write32(pSeasonAwardInfo, 0); //SuperMoney
		Write16(pSeasonAwardInfo, 0); //ItemNum

		Write32(pSeasonAwardInfo, 0); //Coupons
		Write32(pSeasonAwardInfo, 0); //GuildPoint
		Write32(pSeasonAwardInfo, 0); //LuckMoney
		Write8(pSeasonAwardInfo, 0); //ExtendInfoNum

		Write32(pSeasonAwardInfo, 0); //SpeedCoin

		len = pSeasonAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // AnotherSeasonAwardInfo
		BYTE* pAnotherSeasonAwardInfo = p;
		Write16(pAnotherSeasonAwardInfo, 0); // len

		Write32(pAnotherSeasonAwardInfo, 0);  // Exp
		Write32(pAnotherSeasonAwardInfo, 0); // Money
		Write32(pAnotherSeasonAwardInfo, 0);  // SuperMoney
		Write16(pAnotherSeasonAwardInfo, 3);   // ItemNum
		{ // ItemInfo
			BYTE* pItemInfo = pAnotherSeasonAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 121997); // ItemID
			Write32(pItemInfo, 1);	   // ItemNum
			Write32(pItemInfo, 0);	   // AvailPeriod
			Write8(pItemInfo, 0);	   // Status
			Write32(pItemInfo, 1710475394);	   // ObtainTime
			Write32(pItemInfo, 0);	   // OtherAttribute
			Write16(pItemInfo, 0);	   // ItemType

			len = pItemInfo - pAnotherSeasonAwardInfo;
			Set16(pAnotherSeasonAwardInfo, (WORD)len);
			pAnotherSeasonAwardInfo += len;
		}
		{ // ItemInfo
			BYTE* pItemInfo = pAnotherSeasonAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 89803); // ItemID
			Write32(pItemInfo, 1);	   // ItemNum
			Write32(pItemInfo, 1080);	   // AvailPeriod
			Write8(pItemInfo, 0);	   // Status
			Write32(pItemInfo, 0);	   // ObtainTime
			Write32(pItemInfo, 1710475394);	   // OtherAttribute
			Write16(pItemInfo, 0);	   // ItemType

			len = pItemInfo - pAnotherSeasonAwardInfo;
			Set16(pAnotherSeasonAwardInfo, (WORD)len);
			pAnotherSeasonAwardInfo += len;
		}
		{ // ItemInfo
			BYTE* pItemInfo = pAnotherSeasonAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 89809); // ItemID
			Write32(pItemInfo, 1);	   // ItemNum
			Write32(pItemInfo, 1080);	   // AvailPeriod
			Write8(pItemInfo, 0);	   // Status
			Write32(pItemInfo, 1710475394);	   // ObtainTime
			Write32(pItemInfo, 0);	   // OtherAttribute
			Write16(pItemInfo, 0);	   // ItemType

			len = pItemInfo - pAnotherSeasonAwardInfo;
			Set16(pAnotherSeasonAwardInfo, (WORD)len);
			pAnotherSeasonAwardInfo += len;
		}
		Write32(pAnotherSeasonAwardInfo, 0); // Coupons
		Write32(pAnotherSeasonAwardInfo, 0); // GuildPoint
		Write32(pAnotherSeasonAwardInfo, 0); // LuckMoney
		Write8(pAnotherSeasonAwardInfo, 0);	 // ExtendInfoNum
		for (size_t i = 0; i < 0; i++)
		{ // ExtendInfoAward
			BYTE* pExtendInfoAward = pAnotherSeasonAwardInfo;
			Write16(pExtendInfoAward, 0); // len

			Write16(pExtendInfoAward, 0); // Key
			Write32(pExtendInfoAward, 0); // AddValue
			Write32(pExtendInfoAward, 0); // TotalValue
			Write8(pExtendInfoAward, 0);  // ShowPriority
			Write16(pExtendInfoAward, 0); // BuyNeedScore
			Write8(pExtendInfoAward, 0);  // OneMatchMaxNum

			len = pExtendInfoAward - pAnotherSeasonAwardInfo;
			Set16(pAnotherSeasonAwardInfo, (WORD)len);
			pAnotherSeasonAwardInfo += len;
		}
		Write32(pAnotherSeasonAwardInfo, 200); // SpeedCoin

		len = pAnotherSeasonAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //SeasonMapNums

	Write16(p, 0); //PlayerSeasonInfoNums

	{ //RankedMatchPromoInfo
		BYTE* pRankedMatchPromoInfo = p;
		Write16(pRankedMatchPromoInfo, 0); //len

		Write8(pRankedMatchPromoInfo, Client->PromotionMatch); //PromotionMatch
		Write32(pRankedMatchPromoInfo, 5); //PromotionRoundNums
		Write32(pRankedMatchPromoInfo, 3); //PromotionSuccessRoundNums
		Write32(pRankedMatchPromoInfo, 0); //PromotionAlreadySuccessRoundNums
		Write32(pRankedMatchPromoInfo, GetPromotionRecordTag(Client->PromotionRecordTag, Client->CurrentPromotionRoundNums)); //PromotionRecordTag
		Write32(pRankedMatchPromoInfo, Client->CurrentPromotionRoundNums); //CurrentPromotionRoundNums
		Write32(pRankedMatchPromoInfo, 0); //PromoFailedMinusScore
		Write32(pRankedMatchPromoInfo, 0); //PromoSuccAddScore
		Write8(pRankedMatchPromoInfo, 0); //PromoDirectly
		Write8(pRankedMatchPromoInfo, 0); //GradeNoNeedPromo
		Write8(pRankedMatchPromoInfo, 0); //RankedMemoryNoNeedPromo
		Write8(pRankedMatchPromoInfo, 0); //PromotionExtraScore
		Write8(pRankedMatchPromoInfo, 0); //PromtRaceFailedDefendFlag

		len = pRankedMatchPromoInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //InhertGradeTag
	{ //GradeInheritInfo
		BYTE* pGradeInheritInfo = p;
		Write16(pGradeInheritInfo, 0); //len

		Write16(pGradeInheritInfo, 0); //OriginalGradeLevel
		Write16(pGradeInheritInfo, 0); //OriginalChildLevel
		Write16(pGradeInheritInfo, 0); //InheritedGradeLevel
		Write16(pGradeInheritInfo, 0); //InheritedChildLevel
		Write16(pGradeInheritInfo, 0); //InheritedLowLimitGradeLevel
		Write16(pGradeInheritInfo, 0); //InheritedLowLimitChildLevel

		len = pGradeInheritInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //RewardSeasonAward
	{ //LastSeasonAwardInfo
		BYTE* pLastSeasonAwardInfo = p;
		Write16(pLastSeasonAwardInfo, 0); //len

		Write32(pLastSeasonAwardInfo, 0); //Exp
		Write32(pLastSeasonAwardInfo, 0); //Money
		Write32(pLastSeasonAwardInfo, 0); //SuperMoney
		Write16(pLastSeasonAwardInfo, 0); //ItemNum

		Write32(pLastSeasonAwardInfo, 0); //Coupons
		Write32(pLastSeasonAwardInfo, 0); //GuildPoint
		Write32(pLastSeasonAwardInfo, 0); //LuckMoney
		Write8(pLastSeasonAwardInfo, 0); //ExtendInfoNum

		Write32(pLastSeasonAwardInfo, 0); //SpeedCoin

		len = pLastSeasonAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // AnotherLastSeasonAwardInfo
		BYTE* pAnotherLastSeasonAwardInfo = p;
		Write16(pAnotherLastSeasonAwardInfo, 0); // len

		Write32(pAnotherLastSeasonAwardInfo, 0); // Exp
		Write32(pAnotherLastSeasonAwardInfo, 0); // Money
		Write32(pAnotherLastSeasonAwardInfo, 0); // SuperMoney
		Write16(pAnotherLastSeasonAwardInfo, 3); // ItemNum
		{ // ItemInfo
			BYTE* pItemInfo = pAnotherLastSeasonAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 121997); // ItemID
			Write32(pItemInfo, 1); // ItemNum
			Write32(pItemInfo, 360); // AvailPeriod
			Write8(pItemInfo, 0);  // Status
			Write32(pItemInfo, 1710475394); // ObtainTime
			Write32(pItemInfo, 0); // OtherAttribute
			Write16(pItemInfo, 0); // ItemType

			len = pItemInfo - pAnotherLastSeasonAwardInfo;
			Set16(pAnotherLastSeasonAwardInfo, (WORD)len);
			pAnotherLastSeasonAwardInfo += len;
		}
		{ // ItemInfo
			BYTE* pItemInfo = pAnotherLastSeasonAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 89803); // ItemID
			Write32(pItemInfo, 1); // ItemNum
			Write32(pItemInfo, 1080); // AvailPeriod
			Write8(pItemInfo, 0);  // Status
			Write32(pItemInfo, 1710475394); // ObtainTime
			Write32(pItemInfo, 0); // OtherAttribute
			Write16(pItemInfo, 0); // ItemType

			len = pItemInfo - pAnotherLastSeasonAwardInfo;
			Set16(pAnotherLastSeasonAwardInfo, (WORD)len);
			pAnotherLastSeasonAwardInfo += len;
		}
		{ // ItemInfo
			BYTE* pItemInfo = pAnotherLastSeasonAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 89809); // ItemID
			Write32(pItemInfo, 1); // ItemNum
			Write32(pItemInfo, 1080); // AvailPeriod
			Write8(pItemInfo, 0);  // Status
			Write32(pItemInfo, 1710475394); // ObtainTime
			Write32(pItemInfo, 0); // OtherAttribute
			Write16(pItemInfo, 0); // ItemType

			len = pItemInfo - pAnotherLastSeasonAwardInfo;
			Set16(pAnotherLastSeasonAwardInfo, (WORD)len);
			pAnotherLastSeasonAwardInfo += len;
		}
		Write32(pAnotherLastSeasonAwardInfo, 0); // Coupons
		Write32(pAnotherLastSeasonAwardInfo, 0); // GuildPoint
		Write32(pAnotherLastSeasonAwardInfo, 0); // LuckMoney
		Write8(pAnotherLastSeasonAwardInfo, 0);	 // ExtendInfoNum
		for (size_t i = 0; i < 0; i++)
		{ // ExtendInfoAward
			BYTE* pExtendInfoAward = pAnotherLastSeasonAwardInfo;
			Write16(pExtendInfoAward, 0); // len

			Write16(pExtendInfoAward, 0); // Key
			Write32(pExtendInfoAward, 0); // AddValue
			Write32(pExtendInfoAward, 0); // TotalValue
			Write8(pExtendInfoAward, 0);  // ShowPriority
			Write16(pExtendInfoAward, 0); // BuyNeedScore
			Write8(pExtendInfoAward, 0);  // OneMatchMaxNum

			len = pExtendInfoAward - pAnotherLastSeasonAwardInfo;
			Set16(pAnotherLastSeasonAwardInfo, (WORD)len);
			pAnotherLastSeasonAwardInfo += len;
		}
		Write32(pAnotherLastSeasonAwardInfo, 0); // SpeedCoin

		len = pAnotherLastSeasonAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // DailyAwardInfo
		BYTE* pDailyAwardInfo = p;
		Write16(pDailyAwardInfo, 0); // len

		Write16(pDailyAwardInfo, 3); // ScoreInc
		Write16(pDailyAwardInfo, 0); // DailyScoreInc
		Write8(pDailyAwardInfo, 0);	 // DailyAwardGot
		Write32(pDailyAwardInfo, 50); // SuperMoney
		Write32(pDailyAwardInfo, 50); // CouponMoney

		len = pDailyAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 1); //CreateRoom
	Write8(p, 0); //RemindLengnedSpeedGodTip
	Write8(p, 0); //DisableSwitchGameSvr
	Write8(p, 0); //InheritFromOldSysTag
	Write32(p, 0); //OldSysGrade
	Write32(p, 0); //WeeklyAwardRankCoinNums
	Write32(p, 400); //WeeklyAwardRankCoinLimit
	Write32(p, 0); //CurrentRankCoinNums
	Write32(p, 0); //RankTaskNum

	Write32(p, 37); // FeiShiDescLen
	memcpy(p, "铂金 30天 钻石 永久星耀解锁专属特效", 36); //NickName[]
	p += 37;

	Write32(p, 146571); //FeiShiID
	Write32(p, 2); // ShopItemNums
	{ // ShopItemInfos
		BYTE* pShopItemInfos = p;
		Write16(pShopItemInfos, 0); // len

		Write8(pShopItemInfos, 6);	// ItemLabelType
		Write32(pShopItemInfos, 138203); // ItemID

		len = pShopItemInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // ShopItemInfos
		BYTE* pShopItemInfos = p;
		Write16(pShopItemInfos, 0); // len

		Write8(pShopItemInfos, 5);	// ItemLabelType
		Write32(pShopItemInfos, 138154); // ItemID

		len = pShopItemInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}

	p += 20;
	Write32(p, 125); // BackgroundSceneId
	{ //RankedMatchSeasonChallengeInfo
		BYTE* pRankedMatchSeasonChallengeInfo = p;
		Write16(pRankedMatchSeasonChallengeInfo, 0); //len

		Write32(pRankedMatchSeasonChallengeInfo, 0); //SeasonStarNums
		Write32(pRankedMatchSeasonChallengeInfo, 0); //SeasonTaskNums

		{ //RankedMatchSeasonChallengeHonorDefendInfo
			BYTE* pRankedMatchSeasonChallengeHonorDefendInfo = pRankedMatchSeasonChallengeInfo;
			Write16(pRankedMatchSeasonChallengeHonorDefendInfo, 0); //len

			Write32(pRankedMatchSeasonChallengeHonorDefendInfo, 0); //NeedStarNums
			Write32(pRankedMatchSeasonChallengeHonorDefendInfo, 0); //AwardHonorDefendNums
			Write32(pRankedMatchSeasonChallengeHonorDefendInfo, 0); //DefendAddSocre

			len = pRankedMatchSeasonChallengeHonorDefendInfo - pRankedMatchSeasonChallengeInfo;
			Set16(pRankedMatchSeasonChallengeInfo, (WORD)len);
			pRankedMatchSeasonChallengeInfo += len;
		}
		Write8(pRankedMatchSeasonChallengeInfo, 0); //FinishNewTask
		Write32(pRankedMatchSeasonChallengeInfo, 0); //CurrentDefendHonorNums
		Write32(pRankedMatchSeasonChallengeInfo, 0); //UiDefendHonorLimitNums

		len = pRankedMatchSeasonChallengeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //RaceTipType
	Write8(p, 1); //NewSeasonBegain
	Write8(p, 0); //HaveSeasonMapChallengeInfo

	Write32(p, 0); //NewSeasonMapAddDate
	Write8(p, 0); //HaveTippedSuperRaceGuard
	Write16(p, 0); //PromoAwardNums

	Write16(p, 0); //SeasonAwardNums

	Write32(p, 0); //CurrentHighGradeLevel
	Write32(p, 0); //CurrentHighChildLevel
	Write16(p, 28); //SpecialTipInfoLen
	p += 28;
	Write16(p, 3); //SeasonNewMapNums
	Write32(p, 145); //SeasonNewMapID[]
	Write32(p, 420); //SeasonNewMapID[]
	Write32(p, 446); //SeasonNewMapID[]
	Write8(p, 1); //IsTriggerInheritedLowLimit
	Write8(p, 49); //InheritedLowLimitContentLens
	p += 49;
	Write8(p, 0); //IsInheritedLowLimitUpdate
	Write8(p, 0); //InheritedLowLimitUpdateContentLens
	Write8(p, 1); //RankedMatchMissionTaskNums
	{ // RankedMatchMissionTask
		BYTE* pRankedMatchMissionTask = p;
		Write16(pRankedMatchMissionTask, 0); // len

		Write32(pRankedMatchMissionTask, 42); // SeasonID
		Write8(pRankedMatchMissionTask, 2);	 // SeasonTaskNums
		Write32(pRankedMatchMissionTask, 4726); // SeasonTask[]
		Write32(pRankedMatchMissionTask, 4727); // SeasonTask[]
		Write8(pRankedMatchMissionTask, 3);	 // RankTaskNums
		Write32(pRankedMatchMissionTask, 4723); // RankTask[]
		Write32(pRankedMatchMissionTask, 4724); // RankTask[]
		Write32(pRankedMatchMissionTask, 4725); // RankTask[]

		len = pRankedMatchMissionTask - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 23); //RankedMatchSuperRacePartLevelLimit
	Write16(p, 6148); //NextGradeAndChildLevelSeasonTask
	Write16(p, 0); //CurrentSeasonTotalPartiNum
	Write8(p, 1); //SeasonMileStoneCfgNum
	{ // SeasonMileStoneCfg
		BYTE* pSeasonMileStoneCfg = p;
		Write16(pSeasonMileStoneCfg, 0); // len

		Write16(pSeasonMileStoneCfg, 42); // SeasonID
		Write16(pSeasonMileStoneCfg, 80); // TotalPartiNums
		Write16(pSeasonMileStoneCfg, 4); // IntervalNum
		Write8(pSeasonMileStoneCfg, 20);	 // AwardNumCfgNums
		Write16(pSeasonMileStoneCfg, 600); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 800); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 800); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 600); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 800); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 600); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 800); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 600); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 300); // AwardNumCfg[]
		Write16(pSeasonMileStoneCfg, 600); // AwardNumCfg[]

		len = pSeasonMileStoneCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //SeasonMileStoneAwardNum
	Write8(p, 1);  // TestDriveKartCfgNum
	{ // KartfactoryTestDriveCfg
		BYTE* pKartfactoryTestDriveCfg = p;
		Write16(pKartfactoryTestDriveCfg, 0); // len

		Write16(pKartfactoryTestDriveCfg, 42); // SeasonID
		Write32(pKartfactoryTestDriveCfg, 0); // TestDriveTaskID
		Write8(pKartfactoryTestDriveCfg, 13);  // TestDriveKartNum
		Write32(pKartfactoryTestDriveCfg, 88867); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 84870); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 23007); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 64881); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 32409); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 65110); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 34865); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 33261); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 13308); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 32824); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 30894); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 18768); // TestDirivekartID[]
		Write32(pKartfactoryTestDriveCfg, 32407); // TestDirivekartID[]
		Write8(pKartfactoryTestDriveCfg, 2);  // KartShowIntervalTime
		Write32(pKartfactoryTestDriveCfg, 32407); // PlayerTestKartItemID[]
		Write32(pKartfactoryTestDriveCfg, 32407); // PlayerTestKartItemID[]
		Write32(pKartfactoryTestDriveCfg, 32407); // PlayerTestKartItemID[]
		Write8(pKartfactoryTestDriveCfg, 0);  // TestKartRefitLevel
		Write16(pKartfactoryTestDriveCfg, 0); // UnLockSuitNum[]

		len = pKartfactoryTestDriveCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 0); //TestDriveKartIdx
	Write32(p, 0); //TipContiUseTestKart
	Write8(p, 0); //MileStoneAddTip
	Write8(p, 0); //Type
	Write8(p, 0); //HasRankedMemoryData

	Write8(p, 0); //HasRankedSpeedFlashData

	Write8(p, 0); //CanPlayPinnacleBattle
	Write32(p, 0); //SavedExtraScore
	Write32(p, 0); //AnonymousModeStartScore
	Write16(p, 0); //SeasonHonerAwardNums

	Write32(p, 0); //LastSeasonTopHonerInfoIdx
	Write8(p, 0); //HasFengJianSunRenAwardData

	Write8(p, 0); //RankedCoinGradeCfgNum

	Write32(p, 0); //WeeklyAwardFactoryCoinLimit

	len = p - buf.data();
	SendToClient(Client, 25000, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



void NotifyRankedMatchSeasonInfo(std::shared_ptr<ClientNode> Client, UINT SrcUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, SrcUin); //Uin
	Write16(p, 1); //PlayerSeasonInfoNums
	for (size_t i = 0; i < 1; i++)
	{ //PlayerSeasonInfos
		BYTE* pPlayerSeasonInfos = p;
		Write16(pPlayerSeasonInfos, 0); //len

		Write16(pPlayerSeasonInfos, 1); //SeasonID
		Write16(pPlayerSeasonInfos, RankGradeLevel[Client->GradeLevel]); //GradeLevel
		Write8(pPlayerSeasonInfos, Client->ChildLevel); //ChildLevel
		Write16(pPlayerSeasonInfos, 0); //HighGradeLevel
		Write8(pPlayerSeasonInfos, 0); //HighChildLevel
		Write8(pPlayerSeasonInfos, 0); //SeasonInfoProcesedTag
		Write8(pPlayerSeasonInfos, 0); //SeasonComputeInfoTipTag
		Write16(pPlayerSeasonInfos, 0); //SeasonInheritTag
		Write8(pPlayerSeasonInfos, 0); //SeasonInheritTipTag
		Write16(pPlayerSeasonInfos, 0); //SpeedGodNums
		Write32(pPlayerSeasonInfos, 0); //SpeedRoundNums
		Write32(pPlayerSeasonInfos, 0); //SpeedFinishRoundNums
		Write32(pPlayerSeasonInfos, 0); //TotalRankIndex
		Write32(pPlayerSeasonInfos, 0); //ContiFinishRoundNums
		Write32(pPlayerSeasonInfos, 0); //SpeedKillAllRoundNums
		Write32(pPlayerSeasonInfos, 0); //PropRoundNums
		Write32(pPlayerSeasonInfos, 0); //PropFinishRoundNums
		Write32(pPlayerSeasonInfos, 0); //ContiFinishPropRoundNums
		Write32(pPlayerSeasonInfos, 0); //PropKillAllRoundNums
		Write8(pPlayerSeasonInfos, 0); //LastRoundIndex
		Write32(pPlayerSeasonInfos, 0); //OpenDate
		Write32(pPlayerSeasonInfos, 0); //CloseDate
		Write32(pPlayerSeasonInfos, 0); //HistroyContFinishRoundNums

		len = pPlayerSeasonInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //RankedMatchSpeedGodNums
	Write8(p, 0); //LegendSpeedGod
	Write32(p, 0); //CurrentSeasonID

	len = p - buf.data();
	SendToClient(Client, 25005, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponsePVPLogin(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID

	len = p - buf.data();
	SendToClient(Client, 697, buf.data(), len, Client->ServerID, FE_PLAYER, Client->ServerID, Response);


}
void RequestPVPLogin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT RandKey = Read32(p);
	ResponsePVPLogin(Client);
}



void ResponseChangeIdentity(std::shared_ptr<ClientNode> Client, char SetOrUnset, char IsExRightFlag, UINT IdentityFlag)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, SetOrUnset); //SetOrUnset 0x0重置
	Write8(p, IsExRightFlag); //IsExRightFlag
	Write32(p, IdentityFlag); //IdentityFlag 0x78关小喷灯 0x02关宝石
	if (SetOrUnset == 0) { //开启
		if (IdentityFlag == 0x78) {
			Client->XiaoPendFlag = 0;
		}
		else if (IdentityFlag == 0x02) {
			Client->StoneOpenFlag = 0;
		}
		else if (IdentityFlag == 0x83) {
			Client->CheDengFlag = 0;
		}
	}
	else { //关闭
		if (IdentityFlag == 0x78) {
			Client->XiaoPendFlag = 1;
		}
		else if (IdentityFlag == 0x02) {
			Client->StoneOpenFlag = 1;
		}
		else if (IdentityFlag == 0x83) {
			Client->CheDengFlag = 1;
		}
	}

	Write32(p, 0x40D00006);
	if (Client->StoneOpenFlag == 0) {
		Write8(p, 0x22);
	}
	else {
		Write8(p, 0x26);
	}
	memcpy(p, "\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\x5C\x10\x1C\x20", 14);
	p += 14;
	Write8(p, Client->XiaoPendFlag);
	if (Client->CheDengFlag == 0) {
		Write8(p, 0x40);
	}
	else {
		Write8(p, 0x48);
	}
	Write32(p, 0);
	Write16(p, 0);
	Write8(p, 0);


	len = p - buf.data();
	SendToClient(Client, 131, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	//return;
	//{
	//	BYTE buf[39] = { 0 };
	//	if (SetOrUnset == 0) //开启
	//	{
	//		if (IdentityFlag == 0x2) {
	//			Client->StoneOpenFlag = 0;//开启宝石
	//			memcpy(buf, "\x00\x00\x00\x01\x00\x00\x00\x02\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\x5C\x10\x1C\x20\x00\x48\x00\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//		else if (IdentityFlag == 0x8D) { //导航线
	//			memcpy(buf, "\x00\x00\x00\x01\x00\x00\x00\x8D\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\xDC\x10\x1C\x20\x00\x40\x00\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//		else if (IdentityFlag == 0x83) { //车灯
	//			memcpy(buf, "\x00\x00\x00\x01\x00\x00\x00\x83\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\xDC\x10\x1C\x20\x00\x40\x20\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//		else if (IdentityFlag == 0x78) { //小喷灯
	//			memcpy(buf, "\x00\x00\x00\x01\x00\x00\x00\x78\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\xDC\x10\x1C\x20\x00\x48\x20\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//	}
	//	else//关闭
	//	{
	//		if (IdentityFlag == 0x2)
	//		{
	//			//00 00 01 01 00 00 00 02 40 D0 00 06 26 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 00 40 00 00 00 00 00 00 00  
	//			//00 00 01 01 00 00 00 02 40 D0 00 06 26 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 00 48 00 00 00 00 00 00 00 宝石带车灯
	//			//00 00 01 01 00 00 00 02 40 D0 00 06 26 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 01 40 00 00 00 00 00 00 00 宝石带小喷
	//			//00 00 01 01 00 00 00 02 40 D0 00 06 26 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 01 48 00 00 00 00 00 00 00 宝石带车灯小喷

	//			Client->StoneOpenFlag = 1;//关闭宝石 
	//			memcpy(buf, "\x00\x00\x01\x01\x00\x00\x00\x02\x40\xD0\x00\x06\x26\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\x5C\x10\x1C\x20\x00\x48\x00\x00\x00\x00\x00\x00\x00", 36);
	//			memcpy(buf, "\x00\x00\x01\x01\x00\x00\x00\x02\x40\xD0\x00\x06\x26\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\x5C\x10\x1C\x20\x00\x40\x00\x00\x00\x00\x00\x00\x00", 36);
	//			memcpy(buf, "\x00\x00\x01\x01\x00\x00\x00\x02\x40\xD0\x00\x06\x26\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\x5C\x10\x1C\x20\x01\x40\x00\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//		else if (IdentityFlag == 0x8D) //导航线
	//		{
	//			memcpy(buf, "\x00\x00\x01\x01\x00\x00\x00\x8D\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\xDC\x10\x1C\x20\x00\x40\x20\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//		else if (IdentityFlag == 0x83) { //车灯
	//			//00 00 01 01 00 00 00 83 40 D0 00 06 22 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 00 48 00 00 00 00 00 00 00 
	//			//00 00 01 01 00 00 00 83 40 D0 00 06 26 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 00 48 00 00 00 00 00 00 00 车灯加宝石
	//			//00 00 01 01 00 00 00 83 40 D0 00 06 22 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 01 48 00 00 00 00 00 00 00 车灯加笑喷
	//			//00 00 01 01 00 00 00 83 40 D0 00 06 26 04 40 B0 01 20 C4 43 E0 31 01 5C 10 1C 20 01 48 00 00 00 00 00 00 00
	//			memcpy(buf, "\x00\x00\x01\x01\x00\x00\x00\x83\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\xDC\x10\x1C\x20\x00\x48\x20\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//		else if (IdentityFlag == 0x78) { //小喷灯
	//			memcpy(buf, "\x00\x00\x01\x01\x00\x00\x00\x78\x40\xD0\x00\x06\x22\x04\x40\xB0\x01\x20\xC4\x43\xE0\x31\x01\xDC\x10\x1C\x20\x01\x48\x20\x00\x00\x00\x00\x00\x00", 36);
	//		}
	//	}

	//	SendToClient(Client, 131, (BYTE*)buf, 36, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	//}
}

void RequestChangeIdentity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	char SetOrUnset = Read8(p);
	char IsExRightFlag = Read8(p);
	UINT IdentityFlag = Read32(p);
	//printf("SetOrUnset:%d IsExRightFlag:%d IdentityFlag:%d\n ", SetOrUnset, IsExRightFlag, IdentityFlag);
	//UINT BeginBit = Read32(p);
	//UINT EndBit = Read32(p);
	//UCHAR ExRightFlag[] = Read8(p);
	ResponseChangeIdentity(Client, SetOrUnset, IsExRightFlag, IdentityFlag);
}
void RequestReportCurrentOpt(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short OptType = Read16(p);//紫钻退出0x4 树苗运输碰撞0x2
	int OptID = Read32(p);
	int SpecialValue = Read32(p);
}
void ResponseReportCurrentOpt(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();  size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write16(p, 0x2); //OptType
	Write32(p, Client->OptNum); //OptNum

	len = p - buf.data();
	SendToClient(Client, 139, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //HostUin
	BYTE NpcNum = NpcListInfos.size();
	if (NpcNum > 5)
	{
		NpcNum = 5;
	}
	Write8(p, NpcNum); //NPCNum
	for (size_t i = 0; i < NpcNum; i++)
	{
		//NPCInfo
		BYTE* pNPCPos = p;
		Write16(pNPCPos, 0); //len
		Write32(pNPCPos, NpcListInfos[i].NpcID); //NpcID
		Write16(pNPCPos, 0); //StartPos
		Write16(pNPCPos, NpcListInfos[i].MaxLevel); //NPCAILevel
		len = pNPCPos - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //StageIndex

	len = p - buf.data();
	SendToClient(Client, 784, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

//打开宝箱
void ResponseGetNewHandCritBoxAwardCfg(UINT ItemId, std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();  size_t len;
	std::vector<ItemInfo> Items;
	int allitem = 0;
	int alllist = 0;

	TREASUREBOX* TreasureBox = GetTreasureBox(ItemId);
	if (!TreasureBox)
	{
		return;
	}
	std::vector<TreasureAward> TreasureAward = TreasureBox->TreasureAwards;
	if (TreasureAward.empty())
	{
		return;
	}
	if (Uinew) {
		int TreasureAwardSize = TreasureAward[0].AwardItems.size();
		Items.resize(TreasureAwardSize);
		for (char j = 0; j < TreasureAwardSize; j++)
		{
			Items[j].ItemID = TreasureAward[0].AwardItems[j].ItemID;
			Items[j].ItemNum = TreasureAward[0].AwardItems[j].ItemNum;
			Items[j].AvailPeriod = TreasureAward[0].AwardItems[j].AvailPeriod;
		}

		char ItemNum = 0;
		Write16(p, 0); //ResultId
		Write32(p, Client->Uin); //Uin
		BYTE* pItemNum = p;
		Write16(p, 0); //ItemNum
		for (size_t i = 0; i < TreasureAwardSize; i++)
		{ //LottoryAwardItem
			BYTE* pLottoryAwardItem = p;
			if (Items[i].ItemID == 0)
			{
				break;
			}
			ItemNum++;
			Write16(pLottoryAwardItem, 0); //len
			Write32(pLottoryAwardItem, Items[i].ItemID); //ItemID
			Write32(pLottoryAwardItem, Items[i].ItemNum); //Num
			//Write32(pLottoryAwardItem, 0xFFFFFFFF); //Period
			Write32(pLottoryAwardItem, Items[i].AvailPeriod); //Period
			Write16(pLottoryAwardItem, 2); //ItemType

			len = pLottoryAwardItem - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Set16(pItemNum, ItemNum);
	}
	else {
		int TreasureAwardSize = TreasureAward[0].AwardItems.size();
		Items.resize(8);//最多显示8个
		for (char j = 0; j < 8; j++)
		{
			if (j >= TreasureAwardSize)
			{
				break;
			}
			Items[j].ItemID = TreasureAward[0].AwardItems[j].ItemID;
			Items[j].ItemNum = TreasureAward[0].AwardItems[j].ItemNum;
			Items[j].AvailPeriod = TreasureAward[0].AwardItems[j].AvailPeriod;
		}

		char ItemNum = 0;
		Write16(p, 0); //ResultId
		Write32(p, Client->Uin); //Uin
		BYTE* pItemNum = p;
		Write16(p, 0); //ItemNum
		for (size_t i = 0; i < 8; i++)
		{ //LottoryAwardItem
			BYTE* pLottoryAwardItem = p;
			if (Items[i].ItemID == 0)
			{
				break;
			}
			ItemNum++;
			Write16(pLottoryAwardItem, 0); //len
			Write32(pLottoryAwardItem, Items[i].ItemID); //ItemID
			Write32(pLottoryAwardItem, Items[i].ItemNum); //Num
			//Write32(pLottoryAwardItem, 0xFFFFFFFF); //Period
			Write32(pLottoryAwardItem, Items[i].AvailPeriod); //Period
			Write16(pLottoryAwardItem, 2); //ItemType

			len = pLottoryAwardItem - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Set16(pItemNum, ItemNum);
	}

	
	{ //MultiExtraAwardItem
		BYTE* pMultiExtraAwardItem = p;
		Write16(pMultiExtraAwardItem, 0); //len

		Write32(pMultiExtraAwardItem, 0); //ItemID
		Write32(pMultiExtraAwardItem, 0); //Num
		Write32(pMultiExtraAwardItem, 0); //Period
		Write16(pMultiExtraAwardItem, 0); //ItemType

		len = pMultiExtraAwardItem - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0xa); //LuckValue
	Write32(p, 0x64); //MaxLuckValue
	Write32(p, ItemId); //ItemID

	len = p - buf.data();
	SendToClient(Client, 313, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

void NotifyGenGameEndBoxAward(std::shared_ptr<ClientNode> Client)
{

	UINT temp = 0;
	ItemInfo EndItems[3];

	int hour;
	if (GameEndBoxTime[0]) {
		// 获取当前系统时间
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		struct tm tm_now;
		localtime_s(&tm_now, &now_time);

		// 获取小时
		hour = tm_now.tm_hour;
	}

	if (GameEndBox2Uin[Client->Uin]) {
		for (size_t i = 0; i < 3; i++)
		{
			int Itemnum = GameEndBoxTime[0] && hour >= GameEndBoxTime[2] && hour < GameEndBoxTime[3] ? GameEndBox2Uin[Client->Uin]->ItemNum * GameEndBoxTime[1] : GameEndBox2Uin[Client->Uin]->ItemNum;
			EndItems[i].ItemID = GameEndBox2Uin[Client->Uin]->BoxID;
			EndItems[i].ItemNum = Itemnum;
			EndItems[i].AvailPeriod = 0;
			EndItems[i].ItemType = GameEndBox2Uin[Client->Uin]->BoxType;
		}
	}
	else {
		for (size_t i = 0; i < 3; i++)
		{
			int randomID;
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<int> dis(1, (int)GameEndBox.size());
			randomID = dis(eng);
			while (randomID == temp && temp > 0)
			{
				randomID = dis(eng);
			}
			int Itemnum = GameEndBoxTime[0] && hour >= GameEndBoxTime[2] && hour < GameEndBoxTime[3] ? GameEndBox[randomID - 1].ItemNum * GameEndBoxTime[1] : GameEndBox[randomID - 1].ItemNum;
			EndItems[i].ItemID = GameEndBox[randomID - 1].ItemID;
			EndItems[i].ItemNum = Itemnum;
			EndItems[i].AvailPeriod = GameEndBox[randomID - 1].AvailPeriod;
			EndItems[i].ItemType = GameEndBox[randomID - 1].ItemType;
			temp = randomID;
		}
	}




	EndGame[Client->Uin] = EndItems[0];

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 3); //ItemNum
	for (size_t i = 0; i < 3; i++)
	{ //ItemInfo
		BYTE* pItemInfo = p;
		Write16(pItemInfo, 0); //len

		Write32(pItemInfo, EndItems[i].ItemID); //ItemID
		Write32(pItemInfo, EndItems[i].ItemNum); //ItemNum
		Write32(pItemInfo, EndItems[i].AvailPeriod); //AvailPeriod
		Write8(pItemInfo, 0); //Status
		Write32(pItemInfo, 0); //ObtainTime
		Write32(pItemInfo, 0); //OtherAttribute
		Write16(pItemInfo, EndItems[i].ItemType); //ItemType

		len = pItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0x62); //DescLen
	memcpy(p, "\xb9\xa7\xcf\xb2\xc4\xe3\xbb\xf1\xb5\xc3\xd0\xd2\xd4\xcb\xbf\xa8\xc5\xc6\x2f\x43\x30\x33\x33\xd1\xa1\x31\x2f\x43\x30\x30\xb5\xc4\xbb\xfa\xbb\xe1\xa3\xac\xb5\xe3\xbb\xf7\xbf\xa8\xc5\xc6\xbc\xb4\xbf\xc9\xc1\xec\xbd\xb1\x21\x00\x53\x45\x4c\x45\x43\x54\x20\x49\x74\x65\x6d\x49\x44\x2c\x49\x74\x65\x6d\x4e\x75\x6d\x2c\x41\x76\x61\x69\x6c\x50\x65\x72\x69\x6f\x64\x2c\x53\x74\x61\x74\x75\x73\x2c\x4f", 98);
	p += 98;
	len = p - buf.data();
	SendToClient(Client, 24755, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);


	ItemInfo Items = { 0 };
	ItemInfo ItemIn = EndGame[Client->Uin];
	int ItemNum = PlayerDB_AddItem4(Client->Uin, ItemIn.ItemID, ItemIn.ItemNum, ItemIn.AvailPeriod, false);
	Items.ItemID = ItemIn.ItemID;
	Items.ItemNum = ItemNum;
	Items.AvailPeriod = ItemIn.AvailPeriod;
	Items.Status = false;
	Items.ObtainTime = (UINT)time(0);
	Items.ItemType = ItemIn.ItemType;
	NotifyClientAddItem(Client, 1, { Items });
}

void RequestGetGameEndBoxAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	//UINT Value = Read32(p);

	/*ItemInfo Items[1];
	ItemInfo ItemIn = EndGame[Client->Uin];
	int ItemNum = PlayerDB_AddItem(Client->Uin, ItemIn.ItemID, ItemIn.ItemNum, ItemIn.AvailPeriod, false);
	Items[0].ItemID = ItemIn.ItemID;
	Items[0].ItemNum = ItemNum;
	Items[0].AvailPeriod = ItemIn.AvailPeriod;
	Items[0].Status = false;
	Items[0].ObtainTime = (UINT)time(0);
	Items[0].ItemType = ItemIn.ItemType;
	NotifyClientAddItem(Client, 1, Items);*/

	ResponseGetGameEndBoxAward(Client);
}
void ResponseGetGameEndBoxAward(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192] = { 0 };
	BYTE* p = buf;
	size_t len = 0;
	Write32(p, Client->Uin); // Uin
	Write16(p, 0);			 // ResultID
	Write32(p, 2);
	len = p - buf;

	SendToClient(Client, 24756, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void ResponseC2SGetKartAltasConfig(std::shared_ptr<ClientNode> Client, UINT CfgVersion)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;

	Write32(p, Client->Uin); // Uin
	Write32(p, CfgVersion);	 // CfgVersion
	Write8(p, 0);			 // Result

	{
		BYTE* pKartAltasCfgNum = p;
		Write16(pKartAltasCfgNum, 25); // KartAltasCfgNum
		int ItemNum = 0;

		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;

		try
		{
			/*std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT KartID FROM KartRefit WHERE Uin=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);*/

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT id as KartID  FROM ItemallNew WHERE type = 'EAIT_CAR'"));
			prep_stmt->clearParameters();
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				ItemNum++;
				int KartID = res->getInt("KartID");
				BYTE* pKartAltasConfig = pKartAltasCfgNum;
				Write16(pKartAltasConfig, 0); //len
				Write32(pKartAltasConfig, KartID); //ItemID
				Write32(pKartAltasConfig, 0); //KartAltasUrlLen
				Write32(pKartAltasConfig, 1); //KartJetNum

				len = pKartAltasConfig - pKartAltasCfgNum;
				Set16(pKartAltasCfgNum, (WORD)len);
				pKartAltasCfgNum += len;
			}
			res.reset();
		}
		catch (const std::exception  e)
		{
			std::cerr << e.what() << "ResponseC2SGetKartAltasConfig Error" << std::endl;
			//throw;关闭
		}

		len = pKartAltasCfgNum - p;
		Set16(p, ItemNum);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 13710, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


//void ResponseC2SGetKartAltasConfig(std::shared_ptr<ClientNode> Client, UINT CfgVersion)
//{
//	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
//	size_t len;
//
//	Write32(p, Client->Uin); //Uin
//	Write32(p, CfgVersion); //CfgVersion
//	Write8(p, 0); //Result
//
//	BYTE* pKartAltasCfgNum = p;
//	Write16(p, 0); //KartAltasCfgNum
//
//	if (!MKartAltasList)
//	{
//		return;
//	}
//	KARTALTAS* KartAltas = MKartAltasList->KartAltas;
//	size_t Num = MKartAltasList->Size;
//	for (size_t i = 0; i < Num; i++)
//	{
//		BYTE* pKartAltasConfig = p;
//		Write16(pKartAltasConfig, 0); //len
//
//		Write32(pKartAltasConfig, KartAltas[i].ID); //ItemID
//		Write32(pKartAltasConfig, 0); //KartAltasUrlLen
//		Write32(pKartAltasConfig, KartAltas[i].KartJetNum); //KartJetNum
//
//		len = pKartAltasConfig - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//	Set16(pKartAltasCfgNum, Num);
//
//
//	len = p - buf.data();
//	SendToClient(Client, 13710, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
//	//delete[] buf;
//}
void RequestC2SGetKartAltasConfig(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UINT CfgVersion = Read32(Body);

	ResponseC2SGetKartAltasConfig(Client, CfgVersion);
}

void SelectGlobleStone(std::shared_ptr<ClientNode> Client) {
	
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		const char* sql = NULL;
		for (size_t i = 0; i < itemSkillStone.size(); i++)
		{
			sql = "SELECT 1 FROM Item Where ItemId=? and Status=1 and Uin=?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, itemSkillStone[i].ItemID);
			prep_stmt->setUInt(2, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				Client->GlobalStone = 1;
				Client->GlobalItemStoneid = itemSkillStone[i].ItemID;
				break;
			}
			res.reset();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectGlobleStone Error" << std::endl;
		////throw;关闭
	}
}

// 查找函数
ItemSkillStone* findItemSkillStoneById(int id) {
	auto it = std::find_if(itemSkillStone.begin(), itemSkillStone.end(), [id](const ItemSkillStone& item) {
		return item.ItemID == id;
	});

	if (it != itemSkillStone.end()) {
		return &(*it); // 返回指向找到元素的指针
	}
	else {
		return nullptr; // 如果没有找到，返回 nullptr
	}
}
std::string findItemSkillStoneById(std::shared_ptr<ClientNode> Client, int id) {
	auto it = std::find_if(itemSkillStone.begin(), itemSkillStone.end(), [id](const ItemSkillStone& item) {
		return item.ItemID == id;
	});

	std::string ret = "";
	if (it != itemSkillStone.end()) {
		Client->GlobalItemStoneid = it->ItemID;
		Client->GlobalStone = 1;
		ret = "道具：" + GetItemName(it->ItemID) + "佩戴成功！全车宝石将会变成\n/C01";
		for (size_t i = 0; i < it->StoneCfg.size(); i++)
		{
			ret += GetItemName(it->StoneCfg[i].StoneID) + " ";
		}
	}
	return ret;
}


void NotifyUpdateKartSkillStoneInfoConfig(std::shared_ptr<ClientNode> Client, int KartID)
{
	ItemSkillStone* result = findItemSkillStoneById(Client->GlobalItemStoneid);
	if (result) {
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

		Write32(p, Client->Uin); //Uin
		int Num = result->StoneCfg.size() > 5 ? 5 : result->StoneCfg.size();
		//int Num = result->StoneCfg.size();
		//KartStoneInfo
		BYTE* pKartStoneInfo = p;
		Write16(pKartStoneInfo, 0); //len
		Write32(pKartStoneInfo, KartID); //KartID
		Write32(pKartStoneInfo, Num); //StoneGrooveNum
		for (int i = 0; i < Num; i++)
		{
			BYTE* pStoneGrooveInfo = pKartStoneInfo;
			Write16(pStoneGrooveInfo, 0); //len

			Write32(pStoneGrooveInfo, 0); //StoneUseOccaType
			Write32(pStoneGrooveInfo, result->StoneCfg[i].StoneID); //SkillStoneID

			len = pStoneGrooveInfo - pKartStoneInfo;
			Set16(pKartStoneInfo, (WORD)len);
			pKartStoneInfo += len;
		}
		len = pKartStoneInfo - p;
		Set16(p, (WORD)len);
		p += len;
		len = p - buf.data();
		SendToClient(Client, 906, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	}
}


