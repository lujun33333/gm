#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef _DEBUG
#define DEBUG
#endif

#include "HPSocket/HPSocket.h"


#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"
#include "ConnectionPool.h"
#pragma comment (lib,"spdlog.lib")
#include "Utils.h"
#include <io.h>
#include <shared_mutex>
#ifdef ZingSpeed
constexpr int MaxNickName = 51;
constexpr int MaxQQNickName = 60;
constexpr int MaxSignature = 128;

constexpr int MAX_MUCH_SEATNUMINROOM = 6;
constexpr int MAXNPCNUMINROOM = 6;
#else
constexpr int MaxNickName = 17;//飞车名字长度
constexpr int MaxQQNickName = 20;//qq名字长度
constexpr int MaxSignature = 64;//签名长度
constexpr int MaxSecPwd = 11;

constexpr int MAX_MUCH_SEATNUMINROOM = 20;
constexpr int MAXNPCNUMINROOM = 20;
constexpr int MaxQQHeadUrl = 105;
#endif

#define NOINLINE __declspec(noinline)

#define WORDSWAP(w)		(((w) >> 8) | \
						(((w) << 8) & 0xFF00))

#define DWORDSWAP(dw)	(((dw) >> 24) | \
						(((dw) >> 8) & 0x0000FF00) | \
						(((dw) << 8) & 0x00FF0000) | \
						(((dw) << 24) & 0xFF000000))


#pragma pack(push, 1)

typedef struct OpenBoxDef
{
	DWORD Uin;
	DWORD Time;
	DWORD BoxID;
	WORD BoxType;//BoxType
	DWORD ItemNum;
	WORD KeyType;//10
	DWORD KeyID;//KeyID
	DWORD KeyNum;//Num
}*LPOpenBoxDef;

typedef struct OpenBox2Def
{
	DWORD Uin;
	DWORD Time;
	DWORD BoxID;
	WORD BoxType;//BoxType
	DWORD KeyNum;
	WORD KeyType;//10
	BYTE tm1;
	DWORD ItemNum;//Num
}*LPOpenBox2Def;

#pragma pack(pop)

struct ShoppingCar
{
	int CommodityID;
	int PriceIdx;
	int DiscountCardID;
};

struct PlayerRaceInfo
{
	UINT Uin;
	//UINT Round;
	UINT FinishTime;
	UINT Teamid;
	UINT kartid;
	UINT DriftCount = 0;
	UINT HighestSpeed = 0;
	UINT CollisionCount = 0;
	UINT N2OGatherCount = 0;
	UINT RankGradeLevel = 0;
};

struct AppellationInfo
{
	UINT Type;
	UINT Level;
	UINT Difficulty;
	UINT MapId;
	UINT Value;
};

typedef struct DstInfo
{
	USHORT PlayerID;
	UINT Uin;
} DstInfo;

struct AuctionItemInfo
{
	UINT ItemID = 0;
	UINT BidPrice = 0;
	int ItemNum = 0;
	int AvailPeriod = 0;
	int StartingPrice = 0;
	int IncreasePrice = 0;
	UCHAR CommodityId = 0;
	std::string Time = "";
};
struct AuctionCfg
{
	char Open = 0;
	int AuctionItemNum = 0;
	std::vector<AuctionItemInfo> AuctionItems;
	std::string Content = "";
};
struct PetInfo
{
	UINT ID;
	char Name[23];
	UINT Exp;
	UINT MaxExp;
	UINT Level;
	UINT Status;
	UINT PL;
	UINT StrengLevel;
};

struct MechTreasureInfo {
	UINT ItemId = 0;
	UINT ItemNum = 0;
	INT AvailPeriod = 0;
	UINT exchangePieceNum = 0;
	UINT exchangeNeedNum = 0;
	UINT maxGetNum = 0;
	UINT award_idx = 0;
	UINT item_type = 0;
	UINT status = 0;
	UINT getnum = 0;
	UINT errid = 0;
	UINT coupons = 0;
	UINT supermoney = 0;
	UINT merge = 0;
	int alreadyGot = 0;
	int DecomposeNum = 0;
	int seed = 0;
};

struct MapInfo
{
	UINT MapID;
	UINT Record;
};

struct ChallengeMap
{
	UINT Uin;
	UINT MapID;
	UINT RankID;
	UINT GuildID;
	int UseTime;
	UINT KartID;
	std::string Name;
	std::string GuildName;
	std::string FileMd5;
	std::string UUid;
	std::string LastUUid;
	UINT KeepCnt;
	UINT ChallengeCnt;
	char Status;
};

struct ItemStatus
{
	UINT ItemID;
	BYTE NewStatus;
};



struct ItemInfo
{
	UINT ItemID = 0;
	int ItemNum = 0;
	int AvailPeriod = 0;
	bool Status = 0;
	int ObtainTime = 0;
	int OtherAttribute = 0;
	int ItemType = 0;
	UINT ComsumeItemId = 0;
	UINT ComsumeItemNum = 0;
};
struct KartRefitItemInfo
{
	UINT KartId;
	UINT RefitCout;
	UINT MaxFlags;
	UINT WWeight;
	UINT SpeedWeight;
	UINT JetWeight;
	UINT SJetWeight;
	UINT AccuWeight;
	UINT Speed2Weight;
	UINT DriftVecWeight;
	UINT AdditionalZSpeedWeight;
	UINT AntiCollisionWeight;
	UINT ShapeRefitCount;
	UINT KartHeadRefitItemID;
	UINT KartTailRefitItemID;
	UINT KartFlankRefitItemID;
	UINT KartTireRefitItemID;
	UINT ShapeSuitID;
	UINT SecondRefitCount;

	UINT LegendSuitLevel;
	UINT LegendSuitLevelChoice;
	UINT ShapeLegendSuitID;
	UINT SkinSuitID;
};
struct KartStoneItemInfo
{
	UINT KartId;
	UINT StoneID;
	UINT StoneUseOccaType;
	UINT SkillStoneID;
};
struct NpcRacerFinishInfo
{
	UINT NpcID;
	int FinishTime;
	UINT Experience;
	UINT SuperMoney;
	UINT TotalHonor;
	UINT Money;
	char ranknumber;
	BOOL IsFinish = 0;
};


struct MapChallengeOpenMainMap
{
	UINT Mapid;
	UINT Num;
	UINT firsttime;
	UINT secondtime;
	UINT thirdtime;
	std::string msg;
};

struct MapChallengeOpenMain
{
	std::string msg;
	std::vector<MapChallengeOpenMainMap> Maps;
};

struct MoveInfo
{

	UINT Uin;
	char WalkStatue;
	int Posion[3];
	int Ori[9];
	UINT AttachUin;
	int WalkExtState;
	UCHAR Nums;
	UINT PermitSitHorsePetUins[5];
	int EquipedPetID;
	int EquipedPetStatus;
	int PetPKResultStatus;
	USHORT HeroMajorState;
	USHORT HorsePetStatus;
	int NextPoint;
	short PathLen;
	int Path[50];
	int CurCellID;
	USHORT Speed;
	int CellID;
	USHORT Flag;
};

struct EngageInfo
{
	UINT UIN = 0;
	char NickName[MaxNickName];
	char BabyName1[MaxNickName];
	char BabyName2[MaxNickName];
	UINT BabyOneID = 0;
	UINT BabyOneItemID = 0;
	UINT BabyOneEquipStat = 0;
	UINT BabyOneGender = 0;
	UINT BabyTwoID = 0;
	UINT BabyTwoItemID = 0;
	UINT BabyTwoEquipStat = 0;
	UINT BabyTwoGender = 0;
	UINT BackGroundID = 0;
	UINT EngageTime = 0;

};

struct PlayerInfo
{
	std::string LastLoginTime;
	UINT Uin;
	UINT Index;
	UINT VipFlag;
	UINT NobleID;
	UINT NobleLevel;
	UINT NoblePoint;
	UINT GuildVipLevel;

	UINT GuildVipPoint;
	UINT LoverVipLevel;
	UINT LoverVipPoint;
	UINT EmperorLevel;
	UINT EmperorPoint;
	UINT GardenLevel;
	UINT GardenPoint;
	UINT UserHeads;
	UINT UserHeadsBox;
	UINT UserFloor;
	std::string qqNumber;
};


struct ClientInfo
{
	std::string NickName;
	UINT Uin;
	UINT Gender;
	UINT Country;
	UINT License;
	UINT Experience;
	UINT SuperMoney;
	UINT Money;
	UINT WinNum;
	UINT SecondNum;
	UINT ThirdNum;
	UINT TotalNum;
	UINT CurHonor;
	UINT TotalHonor;
	UINT TodayHonor;
	UINT RelaxTime;
	UINT MonthDurationBefor;
	UINT MonthDurationCur;
	UINT Charm;
	UINT DurationGame;
	UINT DanceExp;
	UINT Coupons;
	UINT Admiration;
	UINT LuckMoney;
	UINT TeamWorkExp;
	UINT AchievePoint;
	UINT RegTime;
	std::string Signature;
	UINT SpeedCoin;
	UINT BugletTalk;
	UINT nitrogen;
	UINT admin;
	UINT fps;
	UINT UserHeads;
	UINT UserHeadsBox;
	UINT UserFloor;
	UINT SecPwdStatus;
	UINT Verify;
	UINT gm;
	USHORT SkipFlag;
	UINT SpeclEffect;
	std::string SecPwd;
};

struct ClientNode
{
	std::shared_mutex mtx; // 互斥锁
	u_long HeadSeq = 0;
	USHORT BaseGameMode = 0;;
	USHORT SubGameMode = 0;;
	INT getPropNum = 0;
	bool StartGameTime = false;
	ITcpServer* Server = NULL;
	LONG_PTR BufferPtr = NULL;
	LONG_PTR BufferSize;
	LONG_PTR BufferOffset;
	UINT ConnID = 0;
	BOOL IsLogin = FALSE;
	UINT ConnIDUin = 0;
	DWORD Uin = 0;
	BYTE Key[16];
	USHORT ServerID;
	UINT KartID = 0;
	UINT TeamID = 0;
	//Room
	USHORT RoomID = 0;
	USHORT GameID;
	BOOL IsReady;
	BOOL IsFinish = 0;
	//
	UINT MapID = 0;
	int FinishTime;
	int TotalRound;
	int Round;
	int PassedCheckPoint;
	int EndCheckPoint;
	bool Loop;
	bool MapCompatibilityMode;

	//Dance
	int Scores;

	//Outdoor
	SHORT ChatRoomID = -1;

	UCHAR SuperN2O = 0;
	//SUPERJET
	UCHAR SuperJet;
	UINT SuperJetKartFuelInc = 0;
	USHORT CurrentAccelerateFuel = 0;
	USHORT PropID = 0;
	UINT PropIndex = 0;
	UINT KartSkinID = 0;
	UINT PetID = 0;
	char PetStatus;
	//Other
	UINT TaskID = 0;
	UINT OptNum = 0;
	UINT OptIndex = 4;
	//UINT JumpThread = 0;
	HANDLE Timer = NULL;
	WAITORTIMERCALLBACK Callback = NULL;
	UINT BorderRoomID = 0;
	//char UDPState = 1;
	//UINT AllItemindex =0;
	char SeatID;
	char ReadyState = 1;
	char ranknumber = 0;
	char GlobalStone = 0;
	UINT GlobalItemStoneid = 0;
	char GlobalAddcard = 0;
	//char OBState = 0;
	//char AutoLeave = 0;
	UINT GradeScore = 0;
	UINT GradeLevel = 0;
	UINT ChildLevel;
	UINT TotalScore = 0;
	//晋级赛
	UINT PromotionMatch = 0;
	UINT CurrentPromotionRoundNums = 0;
	UINT CurrentPromotionWinRoundNums = 0;
	UINT CurrentPromotionLoseRoundNums = 0;
	UINT PromotionRecordTag = 0;
	char NickName[17] = { 0 };
	int Charm = 0;
	UINT Experience;
	int SuperMoney;
	int TotalHonor;
	int Money;//酷币
	int BugletTalk;
	UINT DanceExp;
	UINT EngageUin = 0;
	UINT OnlineTime = 0;
	UINT TotalOnlineTime = 0;
	KartRefitItemInfo ClientKartRefitItemInfo;
	char StoneOpenFlag = 0;
	char CheDengFlag;
	char XiaoPendFlag;
	int LuckyValue;
	NpcRacerFinishInfo NpcFinishInfo[6];
	char LoverBaby1EquipStat;
	char LoverBaby2EquipStat;
	int MsgSequence;
	char Ip[24];
	char deleteflag;
	char GuildName[17];
	int GuildID;
	char GuildDuty;
	MoveInfo HeroMoveInfo;//休闲区动作信息
	bool IsRefitItem;

	UINT isRace;
	UINT IsAnonymous;
	std::chrono::steady_clock::time_point lastUpdateTime;
	char isManMachine = 0;
	char Gender;
	UINT nitrogen = 0;
	UINT admin = 0;
	UINT gm = 0;
	std::shared_ptr<PetInfo> Pet = NULL;
	EngageInfo* Engage = NULL;
	UINT OBState = 0;
	UINT QQHead = 0;
	UINT fps = 0;
	sql::Connection* conn;
	char SecPwd[MaxSecPwd];
	UINT SecPwdStats = 0;
	UINT Verify = 0;
	UINT FrozenLeftSeconds = 0;
	UINT FrozenSeconds = 0;
	UINT OrgKartId = 0;;
	int ServeryVerifyKey;
	UINT KGMode = 0;
	INT SkillStoneList[5];
	UINT UserHeads = 0;
	UINT UserHeadsBox = 0;
	UINT UserFloor = 0;
	char QQHeadUrl[MaxQQHeadUrl] = {};
	//std::string QQHeadUrl;
	UINT skipFlag = 0;
	UINT AutoLeave = 0;
	UINT SpeclEffect = 0;
	UINT NobleID;
	USHORT VipFlag;
	UINT NobleLevel;
	UINT NoblePoint;
	UINT GuildVipLevel;
	UINT GuildVipPoint;
	UINT LoverVipLevel;
	UINT LoverVipPoint;
	UINT EmperorLevel;
	UINT EmperorPoint;
	UINT GardenLevel;
	UINT GardenPoint;
	char Password[MaxSignature];
	UINT DriftCount = 0;
	UINT HighestSpeed = 0;
	UINT CollisionCount = 0;
	UINT N2OGatherCount = 0;
	bool IsStake = false;
	bool IsBottomGame = false;
	UINT StakeNum = 0;
	bool NewKartTip = false;
	UINT JumpLevel = 0;
	UINT LapTime = 0;
	UINT MAXLapTime = 0;
	UINT EventTime = 0;
	UINT JumpThread = 0;
	UINT AppellationType = 0;
	bool IsBanMap;
	//std::shared_ptr<RoomNode> PlayerRoom = NULL;
	UINT MatchBanMapIndex;
	UINT real;
	std::string gmtime;
	UINT gmtips;
	UINT Speaking;
	std::vector<ChallengeMap> ChallengeMaps;
	LONG_PTR BufferChallengePtr = NULL;
	LONG_PTR BufferChallengeOffset = NULL;
	int MantissaNum;
	int Mantissa;
	std::string news;
	std::vector<UINT> MyKartLibrary;
	int SwitchFlag1;
	int GameBan;
	int LoginMode = 0;
};
enum ServerType
{
	Game,
	Dance,
	Relax
};
typedef struct MapPool
{
	std::vector<USHORT> MapID;
	UINT Size;
}MAPPOOL;

typedef struct PetItem
{
	UINT PetID;
	char PetName[17];
	USHORT Level;
	USHORT Status;
	USHORT StrengLevel;
	USHORT SkillID1;
	USHORT SkillValue1;
	USHORT SkillID2;
	USHORT SkillValue2;
	USHORT SkillID3;
	USHORT SkillValue3;
}PETITEM;

typedef struct CMatchBanMapInfo // MapInfo
{
	UINT MapID;
	//std::atomic_uint Status;
	UINT Status;
	UCHAR ThemeMapOrNot;
	UCHAR SeasonNewMapOrNot;
	UCHAR PinnacleBattleMapOrNot;
	USHORT TotalBeBannedNums;
	USHORT FinalRandBanMapType;
}*LPCMatchBanMapInfo;

typedef struct TreasureAward
{
	UINT Seed;
	UINT ItemType;
	std::vector<ItemInfo> AwardItems;
}TREASUREAWARD;
typedef struct TreasureBox {
	int LuckyValue;
	std::vector<TREASUREAWARD> TreasureAwards;
}TREASUREBOX;
TREASUREBOX* GetTreasureBox(long ID);

struct LimitMsgInfo
{
	UINT MSGID;
	UINT isInfo;
};

struct NewMapsInfo
{
	UINT MapID;
	std::string WordTxt;
};

struct ChallengeMapsInfo
{
	UINT MapID;
	char IsOpen;
	char IsNew;
};


void LoadRandomMapPool();
void LoadMapPool();
void LoadPetItem();
int LoadTreasureBox();
void LoadCarTreasure();


BOOL SendToClient(std::shared_ptr<ClientNode> Client, short MsgID, const BYTE* Data, DWORD Length, short SrcID, char SrcFE, short DstID, short MsgType, char Encrypt = true);
void OnRecvFromClient(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen);
void OnFullReceive(std::shared_ptr<ClientNode> Client, BYTE* Data, size_t Length);


void InitDirServer(const char* Name, UINT IP, USHORT TcpPort, USHORT UdpPort);
int GetServerType(int ServerID);


BOOL WritePlayerDBBaseInfo(BYTE*& p, UINT Uin, std::shared_ptr<ClientNode> Client);
BOOL WritePlayerDBBaseInfo1(BYTE*& p, UINT Uin, std::shared_ptr<ClientNode> Client);
void WritePlayerGuildInfo(BYTE*& p, UINT Uin, std::shared_ptr<ClientNode> Client);
UINT WriteEquipedItem(BYTE*& p, std::shared_ptr<ClientNode> Client);
UINT WriteEquipedItem(BYTE*& p, std::shared_ptr<ClientNode> Client, UINT* cPetId, BOOLEAN isBorder);
void WriteVisbleInfo(std::shared_ptr<ClientNode> Client, BYTE*& p, UINT Uin, char SeatID, short PlayerID);
void WriteVisbleInfo1(std::shared_ptr<ClientNode> Client, BYTE*& p, UINT Uin, char SeatID, short PlayerID);


//封包
void RequestGetWorldInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetWorldInfo(std::shared_ptr<ClientNode> Client, UINT ClientVersion, UINT MasterVersion, UINT SlaveVersion);
void RequestGetChatRoomList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

extern std::mutex LogFileMutex;
extern FILE* LogFile;
void PrintfFile(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen);


void RequestRegister2(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);


void RequestLogin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestLogout(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLogout(std::shared_ptr<ClientNode> Client);

void NotifyLoginMoreInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items, bool HasNext);

#include "Room.h"


void RequestHello(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseHello(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time);

void RequestPrepareReady(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void NotifyGameBegin(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player);
void NotifyGameBegin1(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player);
void NotifyGameBegin2(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player);
void NotifyGameBegin3(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player);
void NotifyGameBegin4(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player);
void NotifyGameBeginCS(std::shared_ptr<ClientNode> Client);


void NotifyKartPhysParam(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>> Player);
void NotifyRaceBegin(std::shared_ptr<ClientNode> Client);

void RequestReportCurrentInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void NotifyCountDown(std::shared_ptr<ClientNode> Client, UINT WinnerUin, bool WinnerNewRecord, UINT FinTime);
void NotifyRaceShow(std::shared_ptr<ClientNode> Client);



void RequestChangeItemStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeItemStatus(std::shared_ptr<ClientNode> Client, USHORT ItemNum, std::vector<ItemStatus> ItemStatus);
void RequestGetLicenseInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetLicenseInfo(std::shared_ptr<ClientNode> Client);

void RequestGetSystemTaskList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetSystemTaskList(std::shared_ptr<ClientNode> Client, UINT TaskVersion);

void RequestChangeGender(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeGender(std::shared_ptr<ClientNode> Client, BYTE NewGender, BYTE UseType);

void RequestSkipFreshTask(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseSkipFreshTask(std::shared_ptr<ClientNode> Client);

void RequestStartShadowChallenge(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseStartShadowChallenge(std::shared_ptr<ClientNode> Client);
void RequestEndShadowChallenge(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseEndShadowChallenge(std::shared_ptr<ClientNode> Client, int ResultID);

void RequestFishLogin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseFishLogin(std::shared_ptr<ClientNode> Client);

void RequestGetPlayerEventCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetPlayerEventCfg(std::shared_ptr<ClientNode> Client);
void RequestGetPlayerEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetPlayerEvent(std::shared_ptr<ClientNode> Client, BYTE TargetType, BYTE TimeType);

void NotifyMsgBox(std::shared_ptr<ClientNode> Client);


void RequestTransferByTCP(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyTranferByTCP(std::shared_ptr<ClientNode> Client, UINT SrcUin, USHORT SrcPlayerID, int Seq, BYTE* Buff, int Bufflen);
void NotifyTranferByTCPMC(std::shared_ptr<ClientNode> Client, UINT SrcUin, USHORT SrcPlayerID, int Seq, BYTE* Buff, int Bufflen);

void RequestGetTaskList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetTaskList(std::shared_ptr<ClientNode> Client);

void RequestUseProp2(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestChangeModel(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeModel(std::shared_ptr<ClientNode> Client, BYTE NewModel);
void NotifyChangeModel(std::shared_ptr<ClientNode> Client, UINT Uin, BYTE NewModel);

void NotifyClientVipFlag(std::shared_ptr<ClientNode> Client);

void NotifySvrConfig(std::shared_ptr<ClientNode> Client);
void NotifySvrConfig2(std::shared_ptr<ClientNode> Client);
void NotifySvrConfig3(std::shared_ptr<ClientNode> Client);
void NotifySvrConfig3MC(std::shared_ptr<ClientNode> Client);






void RequestChangeUdpState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestTransferUDPOK(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseTransferUDPOK(std::shared_ptr<ClientNode> Client);

void RequestInlaySkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseInlaySkillStone(std::shared_ptr<ClientNode> Client, int KartID, int StoneGrooveID, int SkillStoneID);

void RequestGetProp(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetProp(std::shared_ptr<ClientNode> Client);

void NotifySpeed2Cfg(std::shared_ptr<ClientNode> Client);

void RequestKartRefitInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseKartRefitInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, KartRefitItemInfo* Items, bool HasMoreInfo);

void RequestNewGetFriendList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseNewGetFriendList(std::shared_ptr<ClientNode> Client);

void RequestFizzInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseFizzInfo(std::shared_ptr<ClientNode> Client);

void RequestGetShoppingCarItemList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetShoppingCarItemList(std::shared_ptr<ClientNode> Client);

void RequestReplayOperate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseReplayOperate(std::shared_ptr<ClientNode> Client, UCHAR Cmd);

void RequestGetKeyTranslateInfoNew(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetKeyTranslateInfoNew(std::shared_ptr<ClientNode> Client);


void RequestGetMaster(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetMaster(std::shared_ptr<ClientNode> Client);

void RequestSSCOpenStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseSSCOpenStatus(std::shared_ptr<ClientNode> Client);

void RequestSwitchInterface(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseSwitchInterface(std::shared_ptr<ClientNode> Client);

void RequestSecondGetKartRefitLimitCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseSecondGetKartRefitLimitCfg(std::shared_ptr<ClientNode> Client);



void RequestGetActivityCenterInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetActivityCenterInfo(std::shared_ptr<ClientNode> Client);

void NotifyTopUIItemInfo(std::shared_ptr<ClientNode> Client);
void NotifyTopUIItemInfoNew(std::shared_ptr<ClientNode> Client);
void NotifyRedPointInfo(std::shared_ptr<ClientNode> Client);

void RequestGetRandomTask(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetRandomTask(std::shared_ptr<ClientNode> Client);

void RequestGetItemTimeLimtCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponsetGetItemTimeLimtCfg(std::shared_ptr<ClientNode> Client, UINT UpdateTime);

void RequestGetShapeRefitCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetShapeRefitCfg(std::shared_ptr<ClientNode> Client);
void ResponseGetShapeRefitCfgTest(std::shared_ptr<ClientNode> Client);

void RequestWeRelayCommonCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseWeRelayCommonCfg(std::shared_ptr<ClientNode> Client);

void RequestC2GSign3Operate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GSign3Operate(std::shared_ptr<ClientNode> Client, UCHAR OperateType);

void NotifyClientAddItem(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items);
void NotifyClientAddItemNew(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items);
void NotifyClientAddItemBox(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items);
void ResponseRegister(std::shared_ptr<ClientNode> Client, const char* Nickname, BYTE  Gender, BYTE Country, UINT ItemNum, std::vector<ItemInfo> Items);
void NotifyUpdateKartSkillStoneInfo(std::shared_ptr<ClientNode> Client, int KartID);
void NotifyUpdateKartSkillStoneInfoNew(std::shared_ptr<ClientNode> Client, int KartID);

void RequestDeleteOneItem(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDeleteOneItem(std::shared_ptr<ClientNode> Client, int ItemID);

void RequestRefitKart(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseRefitKart(std::shared_ptr<ClientNode> Client, int KartItemId, const char* Reason, short inc_SJetWeight, short inc_AccuWeight, short inc_JetWeight, short inc_WWeight, short inc_SpeedWeight, short inc_AntiCollisionWeight);
void RequestHyperSpaceJumboGetCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseHyperSpaceJumboGetCfg(std::shared_ptr<ClientNode> Client, UCHAR CfgType);

void RequestTimerChallenge2ndMoreHallInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseTimerChallenge2ndMoreHallInfo(std::shared_ptr<ClientNode> Client);

void RequestPreGetCommonBoxAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponsePreGetCommonBoxAward(std::shared_ptr<ClientNode> Client, BOOL HasMore, USHORT Type, std::vector<ItemInfo>& MiracleItems, char MiracleItemNum);
void ResponsePreGetCommonBoxAwardElse(std::shared_ptr<ClientNode> Client, USHORT Type, int AwardID, UCHAR IsShowAllAward, int EchoClient);
void RequestAddToMatchQueue(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseAddToMatchQueue(std::shared_ptr<ClientNode> Client, USHORT matchQueueType);

void NotifyMatchResult(std::shared_ptr<ClientNode> Client, USHORT matchQueueType);
void NotifyMatchResultLogin(std::shared_ptr<ClientNode> Client);



void NotifyPlayerAllActiveKartInfo(std::shared_ptr<ClientNode> Client); //24338
void NotifyKartActiveCfgInfo(std::shared_ptr<ClientNode> Client);
void RequestLeaveGame(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLeaveGame(std::shared_ptr<ClientNode> Client);

void RequestModifyPlayerSignature(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseModifyPlayerSignature(std::shared_ptr<ClientNode> Client, char* Signature);

void RequestFindPlayerByQQ(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseFindPlayerByQQ(std::shared_ptr<ClientNode> Client, UINT DstUin, UCHAR AreaIdDst, UCHAR SimpleInfo, char* NickName);
void ResponseFindPlayerByQQ2(std::shared_ptr<ClientNode> Client, UINT DstUin, UCHAR AreaIdDst, UCHAR SimpleInfo);
void RequestGetAchieveList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetAchieveList(std::shared_ptr<ClientNode> Client, UINT DstUin);

void NotifyRandRoomNameList(std::shared_ptr<ClientNode> Client);

void NotifyTopListDesc(std::shared_ptr<ClientNode> Client);
void RequestTopList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseTopList(std::shared_ptr<ClientNode> Client, USHORT ID, UCHAR Type);

void NotifyUseItemResult(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum, int CurrentItemNum);

void RequestHeroDoAction(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestOutDoorEggInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestOutDoorSmashEgg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestChangeHammerStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestUseHammer(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestHeroPos(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
#include "Room.h"
#include "Dance.h"
#include "Outdoor.h"

void RequestBuyCommodity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseBuyCommodity(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time, const char* Reason, int BuyCount, ShoppingCar* aShoppingCar);

std::string UTF2GB(const char* str);
std::wstring Utf8ToWide(const std::string& utf8);
std::string SqlStingUtf8(std::string str);
//GB2312到UTF-8的转换
char* G2U(const char* gb2312);
void NotifyChangeItemStatus(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> MyClient, USHORT ItemNum, std::vector<ItemStatus> ItemStatus);
void PrintfBody(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen);
void PrintfBody(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen, const char* filename);
void ResponseSkinChange(std::shared_ptr<ClientNode> Client, int KartItemId, int SkinItemId, int karthead, int kartail, int karflank, int kartire);
void ResponseMagicHouseCallOn(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetExchangeShopInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDoShopExchange(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetMagicHouseInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);			 //?
void getGameMode(std::shared_ptr<ClientNode> Client);
void RequestPropEffectResult(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //126
void NotifyPropEffectResult(std::shared_ptr<ClientNode> Client, UINT Uin, std::vector<UCHAR> RemoveOwnProp, UINT SrcUin, UINT PropID, UINT PropIndex, UINT StoneSkillType, UINT LapTime);
void NotifyPropPoint(std::shared_ptr<ClientNode> Client, UINT Uin, UINT PropID);
void RequestChangeAimState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //127
void NotifyChangeAimState(std::shared_ptr<ClientNode> Client, UINT Uin, UINT DstUin, UINT AimState, UINT DstType);


#include<deque>
#include <mutex>
#include<map>

#include<unordered_map>
#include <stdio.h>

void RequestCancelMatch(std::shared_ptr<ClientNode> Client);

typedef struct CREFITITEMCFG //进阶改装配件信息
{
	UINT ItemID;
	UCHAR ItemType;
	UCHAR ItemWeightLevel;
	WORD Value;
	UINT FragmentItemID;
	UINT FragmentNumToOneItem;
	WORD IsItemOpen;
} *LPCREFITITEMCFG;
const static CREFITITEMCFG RefitItemCfg[] =
{
	/*
		ItemID 进阶改装配件ID
		ItemType	改装道具类型 5=推进 6=离合 7=平衡
		ItemWeightLevel	道具重量值	1=普通 2=精良	3=极致
		Value			改装增加值
		FragmentItemID	碎片道具ID
		FragmentNumToOneItem	碎片数量
		IsItemOpen 是否启用
	*/
	{65025, 5, 1, 1, 65037, 100, 1},
	{65026, 5, 2, 1, 65038, 100, 1},
	{65027, 5, 3, 1, 65039, 100, 1},
	{65028, 6, 1, 1, 65040, 100, 1},
	{65029, 6, 2, 1, 65041, 100, 1},
	{65030, 6, 3, 1, 65042, 100, 1},
	{65031, 7, 1, 1, 65043, 100, 1},
	{65032, 7, 2, 1, 65044, 100, 1},
	{65033, 7, 3, 1, 65045, 100, 1},
	{65034, 5, 1, -1, 0, 0, 1},
	{65035, 6, 1, -1, 0, 0, 1},
	{65036, 7, 1, -1, 0, 0, 1},
};


#include<random>

struct SUPERPROP {
	int ItemID;
	int GetNum;
};
extern void NotifyUseItemResult2(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum);
int64_t GetSysTimeMicros();
void formatTimeStamp(long long microTimestamp, char* temp);
void NotifyClientUpdateMoneyExp(std::shared_ptr<ClientNode> Client);

extern void RequestEnter3DRoom(std::shared_ptr<ClientNode> Client);
extern void NotifyChangeRoomOwner(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);


typedef struct AdvancedProp {
	UINT PropID;
	UINT Add;
}ADVCANCEDPROP;
typedef struct AdvancedCfg
{
	std::vector<ADVCANCEDPROP> AdvancedProp;
}ADVANCEDCFG;

typedef struct MiracleCfg {
	std::vector<ItemInfo> MiracleItems;
}MIRACLECFG;

typedef struct RandomMapPool {
	std::vector<int> MapID;
}RANDOMMAPPOOL;

typedef struct Guidinfo {
	UINT RankLevel;
	UINT Score;
	UINT Attribute;
	UINT MaxMember;
	UINT CurrentMember;
	UINT TeamAddition;
	UINT Prosperity;
	UINT Level;
	std::string NickName;
	std::string Name;
	UINT Duty;
	UINT Uin;
	UINT ID;
	UINT MemProsperity;
	UINT EnlistType;
}GUIDINFO;



extern void NotifyKickPlayer(std::shared_ptr<ClientNode> Client, const char* Reason, UINT KickType);
void NotifyBugletTalk(std::shared_ptr<ClientNode> Client, const char* Content, short clength, char BugletType);
extern void gen_randomcdk(char* s, const int len);
extern char* generateRandomKey();
extern std::mutex KeysMutex;
extern std::map<long, std::string> Keys;
extern std::string GetKey(long Uin);
extern ConnectionPool player;
extern ConnectionPool usr;
void CheckDupUser(UINT Uin, std::shared_ptr<ClientNode> Client);

extern void DisConnectUser(char* dstip, UINT ConnIDUin);

void RequestGetGuildList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGuildDetail(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestCreateGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestApplyJoinGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestReplyJoinGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestRemoveMember(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestFindGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestAppointedPosition(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestSaveShapeRefitNew(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseSaveShapeRefitNew(std::shared_ptr<ClientNode> Client, CONST UINT KartID, CONST UINT ShapeSuitID);
void ResponseSaveShapeRefit(std::shared_ptr<ClientNode> Client, int KartID, int RefitItemNum, int RefitItemID[], int ShapeSuitID);
void NotifyChangeSuitItemStatus(std::shared_ptr<ClientNode> Client, CONST UINT ShapeSuitID, CONST UINT KartID);
void ResponseShapeLegendOperate_OpType_0(std::shared_ptr<ClientNode> Client);
void ResponseShapeLegendOperate_OpType_2(std::shared_ptr<ClientNode> Client, CONST UINT ShapeID, CONST UINT NewShowChoice);
void ResponseShapeLegendOperate_OpType_Level(std::shared_ptr<ClientNode> Client, CONST UINT ShapeID, CONST UINT NewShowChoice, CONST  UINT KartID, CONST  UINT ShapeSuitID);
void ResponseShapeLegendOperate_OpType_3(std::shared_ptr<ClientNode> Client, CONST UINT ShapeID);
void ResponseGetSuperN2O1(std::shared_ptr<ClientNode> Client, UINT PropID);
void ResponseC2SGetTimesExperienceInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2SReceiveTimesExperience(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2SFrostThawTimesExperience(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void ResponseC2GGetWizardBaseInfo(std::shared_ptr<ClientNode> Client);							  // 11526
void RequestChangeCarryWizard(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?
void ResponseChangeCarryWizard(std::shared_ptr<ClientNode> Client, USHORT WizardID, bool Carry); //11506
void ResponseChangeLoverBabyEquipStatus(std::shared_ptr<ClientNode> Client, short EquipStatus); //24386
void RequestChangePetEquipStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);		 //?
void ResponseChangePetEquipStatus(std::shared_ptr<ClientNode> Client, int NewStatus);									 // 15398
void RequestGetPetSkillList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); // 440
void ResponseGetPetSkillList(std::shared_ptr<ClientNode> Client);							 // 440
void RequestGetPKPetInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?
void ResponseGetPKPetInfo(std::shared_ptr<ClientNode> Client, UINT DstUin, char Flag, int PetId); //21133
void RequestPetTrialInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?
void ResponsePetTrialInfo(std::shared_ptr<ClientNode> Client, USHORT TeamID); //21150
void NotifyPetStrengCfgInfo(std::shared_ptr<ClientNode> Client); //20100
void NotifyPetHuanHuaCfgInfo(std::shared_ptr<ClientNode> Client); //24073
void RequestC2GGetWizardBaseInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?

void RequestPetStrengthen(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?
void ResponsePetStrengthen(std::shared_ptr<ClientNode> Client, UINT Result); //20101
PETITEM* GetPetItem(long PetID);

void RequestGetAuctionCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetCurAuctionInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetAuctionList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetParticipateAuction(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestBidAuctionCommodity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestPayAuctionCommodity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetSingleAuctionInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);


void RequestRankedMatchTopRaceOpt(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseRankedMatchTopRaceOpt(std::shared_ptr<ClientNode> Client, short OptType, UINT Para0);


void RequestChangeTeam(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);											// 128
void ResponseChangeTeam(std::shared_ptr<ClientNode> Client, bool NewTeamID);																	// 128
void NotifyPlayerChangeTeam(std::shared_ptr<ClientNode> Client, UINT Uin, bool NewTeamID);

void RequestCheckGuildName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseCheckGuildName(std::shared_ptr<ClientNode> Client, UINT Type);
void RequestUpdateGuildName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseUpdateGuildName(std::shared_ptr<ClientNode> Client, char* newGuildname);

void RequestGetSecondPassInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);									  //?
void NotifySecondPassInfo(std::shared_ptr<ClientNode> Client);
void NotifySecondPassReClose(std::shared_ptr<ClientNode> Client); //21016
void RequestOpSecondPass(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?
void ResponseOpSecondPass(std::shared_ptr<ClientNode> Client, char Op, UINT Status); //21017
void NotifySecondPassOpNotAllowed(std::shared_ptr<ClientNode> Client); //21018														  // 21018

void ResponseCarLottoryGetInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void NotifySkillStoneKartInfo(std::shared_ptr<ClientNode> Client);
void NotifyChangeItemStatus2(std::shared_ptr<ClientNode> Client, UINT Uin, USHORT ItemNum, ItemStatus* ItemStatus);


void NotifyEncryptedMsg(std::shared_ptr<ClientNode> Client);

void ResponseGetGameBaseRecordList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetGameDetailRecordList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetGameDetailRecordList(std::shared_ptr<ClientNode> Client, UINT Recordid);

void RequestGetCarTreasureNoSelfSelectCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
void RequestGetCarTreasureNoSelfSelectAward(std::shared_ptr<ClientNode> Client, BYTE* Body);
void RequestUseItem(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestReportCommonEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); // 24028

void RequestGetGameEndBoxAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);									  // 24756
void ResponseGetGameEndBoxAward(std::shared_ptr<ClientNode> Client);															  // 24756

void RequestTest(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseStartTimerChallenge2(std::shared_ptr<ClientNode> Client);
void ResponseStartTimerChallenge(std::shared_ptr<ClientNode> Client);
void RequestReportTimerChallenge2ndEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestC2GLoadFile(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GLoadFile(std::shared_ptr<ClientNode> Client, UCHAR Cmd, UCHAR UuidLen, std::string FileName);
void RequestReportAntiCollisionDataBuff(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); // 26114

void RequestGetBattleModeCarSkillInfoList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //?
void ResponseGetBattleModeCarSkillInfoList(std::shared_ptr<ClientNode> Client); //10929

UINT GetAppellationLevel(UINT ID);
void NotifyAppellationList(std::shared_ptr<ClientNode> Client);
void RequestWearAppellation(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);	// 1618
void ResponseWearAppellation(std::shared_ptr<ClientNode> Client, UCHAR Type);					// 1618
void RequestUnwearAppellation(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);	// 1619
void ResponseUnwearAppellation(std::shared_ptr<ClientNode> Client, UCHAR Type);					// 1619

void RequestRankedMatchDoBanMap(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseRankedMatchDoBanMap(std::shared_ptr<ClientNode> Client, CONST UCHAR Type);


void Request_Beta87_ReqSelfHeadAndHeadShokInfo(std::shared_ptr<ClientNode> Client, BYTE* Body);
void Request_Beta87_ReqBatchHeadAndHeadShokInfo(std::shared_ptr<ClientNode> Client, BYTE* Body);
void Beta87_ReqHeadAndHeadShokActiveItemCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
void Beta87_ReqHeadAndHeadShokAvatarCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta87_ReqOpHeadAndHeadShok(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta87_ReqSetOrUnSetUseQQHead(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta87_ReqGetQQUrlHead(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta78_MechSkinAwardOpt(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta81_ReqMechSkinEntrys(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta78_ReqMechSkinInfo(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void RequestGetBeta84ColorfulGourdCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void RequestMapChallenge_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void RequestMapChallenge_GetUserMapInfo(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_beta91_Mountain_Music_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_DaZaoWorkshop_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta72ReqGodOfWarBoxCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta91_Silk_Road_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta87_Dragon_City_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta87_Dragon_City_AwardOpt(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_RequestBeta90_SkinTreasureCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta77_ReqNiChangPavilionCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta91_Xinjiang_MainStation_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_CarLibrary_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_CarLibraryOp(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_CarLibrary_QueryOtherData(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_LimitRecommend_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta87_GetLoginCfg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Power_GetStatus(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_Beta91_Cinamon_Cafe_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);
NOINLINE void Request_ConsumeScore_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body);


void NotifyBroadcast65(std::shared_ptr<ClientNode> Client);
BYTE* NotifyBroadcast(BYTE*& ptr, UINT BCFlag, UINT ConTime, UINT DisplayFlag, std::string news);
void NotifyBroadcast17();
void NotifyBroadcast17(std::string news);
void NotifyGame(std::shared_ptr<ClientNode> Client);
void RequestOpenBox(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);											// 168
void ResponseOpenBox(std::shared_ptr<ClientNode> Client, int TreasureBoxID, ItemInfo* aItemStatus, int itemNum, int ItemIDNum); // 168

//车神
void ResponseC2GGetChallengeInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GGetGuildChallengeInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GGetMapsChallengeInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GGetMapChallengeRank(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestC2GGetMapChallengeRank();

void ResponseC2GGetRecentChallengeMsg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestC2GChallenge(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GChallenge(std::shared_ptr<ClientNode> Client, USHORT MapID, USHORT Rank, char FileMd5);
void NotifyG2CLoadChallengeFile(std::shared_ptr<ClientNode> Client, std::vector<ChallengeMap> ChallengeMaps);
void NotifyC2GStartChallengeResult(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyC2GLoadChallengeFileResult(std::shared_ptr<ClientNode> Client, std::vector<ChallengeMap> ChallengeMaps);
void NotifyC2GLoadChallengeFileResult(std::shared_ptr<ClientNode> Client, UINT MapID);
void NotifyG2CStartChallenge(std::shared_ptr<ClientNode> Client);
void ResponseC2GChallengeCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyG2CChallengeResult(std::shared_ptr<ClientNode> Client, UINT Result, int FinTime);
void RequestC2FUpLoad(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2FUpLoad(std::shared_ptr<ClientNode> Client, UCHAR Cmd, UINT DataLen, UINT TotalLen);
void ResponseC2GUpdateChallengeRank(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyG2CUpLoadChallengeFile(std::shared_ptr<ClientNode> Client);
void NotifyG2CGuildChallengeMsg(std::shared_ptr<ClientNode> Client);
void RequestReportCosOpResult(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyG2CUpdateChallengeRankResult(std::shared_ptr<ClientNode> Client, int Result);
void RequestC2GGetMapOwner(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseC2GGetMapOwner(std::shared_ptr<ClientNode> Client, UINT MapId);

void Request24617(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLuckMatchBetAwardInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLuckMatchShowInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseParticipateLuckyMatch(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseCancelParticipateLuckyMatch(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void NotifyCompetitionSystemConfig(std::shared_ptr<ClientNode> Client);
void NotifyRelaxRoomAdvertisement(std::shared_ptr<ClientNode> Client);
void NotifyActivityRecommendCfg(std::shared_ptr<ClientNode> Client);
void NotifyAdvertisement(std::shared_ptr<ClientNode> Client);
void NotifyFileCheckList(std::shared_ptr<ClientNode> Client);
void NotifyTVLiveInfo(std::shared_ptr<ClientNode> Client);
void NotifyClientUpdateItemDescInfo(std::shared_ptr<ClientNode> Client);


void ResponseGetPrivilegeIntroInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetExclusiveSpeedWayInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponsePRGetInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetConsumeScoreConfUrl(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetConsumeScoreConfUrl(std::shared_ptr<ClientNode> Client, UCHAR Md5StrLen);

void RequestWorldOnlineInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseWorldOnlineInfo(std::shared_ptr<ClientNode> Client);
std::string bytes_to_hex(const unsigned char* data, size_t length);
bool compare_char_and_byte(const char* str, const unsigned char byte_arr[16]);

void NotifyLuckyMatchReady(std::shared_ptr<ClientNode> Client);
void ResponseLmtUpdateStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestChangeRecommandStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyLmtMatchOver(std::shared_ptr<ClientNode> Client);
void NotifyLmtEnterRoom(std::shared_ptr<ClientNode> Client);
void ResponseLmtEnterChatRoom(std::shared_ptr<ClientNode> Client);
void RequestLmtEnterChatRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestLmtBuglet(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponsetLmtBuglet(std::shared_ptr<ClientNode> Client, UCHAR OpType);

void NotifyG2CFizzStarCfg(std::shared_ptr<ClientNode> Client);
void NotifyItemType2ndCfg(std::shared_ptr<ClientNode> Client);
void NotifyMultiUseItemListCfg(std::shared_ptr<ClientNode> Client);
void NotifyMusicBlackList(std::shared_ptr<ClientNode> Client);
void NotifyClientPersonalGardenCfg(std::shared_ptr<ClientNode> Client);
void NotifyClientDomainResolve(std::shared_ptr<ClientNode> Client);

void RequestQsLabCommittalTaskInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseQsLabCommittalTaskInfo(std::shared_ptr<ClientNode> Client);
void NotifyClientSkillTrainingTipsInfo(std::shared_ptr<ClientNode> Client);
void NotifyClientNoviceGuideCfg(std::shared_ptr<ClientNode> Client);
void ResponseGetWizardHomeBaseInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetTaskAwardInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetTaskAwardInfo(std::shared_ptr<ClientNode> Client, UINT Type, std::vector<UINT> TaskList, std::string TaskStr);
void NotifyFirstRechargeCfg(std::shared_ptr<ClientNode> Client);
void NotifyGroupBuyingrCfg(std::shared_ptr<ClientNode> Client);
void NotifyFBPlayerLevInfo(std::shared_ptr<ClientNode> Client);
void ResponseJiaNianHuaCfg(std::shared_ptr<ClientNode> Client);
void NotifyAllAreaNameCfg(std::shared_ptr<ClientNode> Client);
void NotifyAllCityCfg(std::shared_ptr<ClientNode> Client);
void NotifyScoreBoxCfg(std::shared_ptr<ClientNode> Client);
void NotifyRedEnvelopeInfo(std::shared_ptr<ClientNode> Client);
void NotifyPlayerRaceCardAddInfo(std::shared_ptr<ClientNode> Client);
void NotifyClientGolumBallAwardInfo(std::shared_ptr<ClientNode> Client);
void NotifyLmtGetScoreInfo(std::shared_ptr<ClientNode> Client);
void NotifyGangsterCfg(std::shared_ptr<ClientNode> Client);