#include "Message.h"
#include "Database.h"
#include<stdlib.h>
#include <string>
#include <malloc.h>
#include<memory>
#include "Item.h"
#include "oicq/tea.h"
#include "yaml-cpp/yaml.h"
#include "ConmData.h"
#include <iostream>
#include <shared_mutex>
#include "pet.h"
#include "LegendShape.h"
#include <regex>
#include "DBConnection.h"
#include "LoadConfig.h"
#include <filesystem>
#include <fstream>
#include "md5file.h"
#include <WS2tcpip.h>
extern void ResponseGetSuperN2O(std::shared_ptr<ClientNode> Client);
extern void ResponseGetSuperN2O2(std::shared_ptr<ClientNode> Client);
extern void StartSingleGame(std::shared_ptr<ClientNode> Client, BYTE* Body);
extern DWORD WINAPI myThreadFunction(LPVOID lpParam);

void RequestGetSuperN2O(BYTE* Body, std::shared_ptr<ClientNode> Client);
void MyRequestGetUin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void MyRequestRegister(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void MyRequestRegisterNew(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void PrintfBody(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen);
void PrintfBody(BYTE* Body, size_t BodyLen);
void virtualsend(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid);
void virtualsend3(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid, std::vector<ItemInfo>& ItemsAdd);
void virtualsend2(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid);
void changekartskin(BYTE* Body, USHORT MsgID, size_t BodyLen, std::shared_ptr<ClientNode> Client);
void RequestDoBagItemShowOperate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
bool GetRandItem(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& items);
bool GetRandItemNew(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& items);
bool GetRandItem2(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& items);
bool GetRandItemCJ(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& items);
UINT GetRandMap(short MapID);
void ResponseGetQuickRaceTimeRandMap(std::shared_ptr<ClientNode> Client);
extern void ResponseStartBorderGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID);
extern void ResponseStartRankedGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID);
char registerflag = 1;//支持注册
char propflag = 1;//支持抽奖开关
bool is60fps = false;//是否开启60帧

extern char TransferModel;//0 udp 1 tcp
extern void RequestChangeIdentity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
extern void ResponseReportCurrentOpt(std::shared_ptr<ClientNode> Client);

extern std::shared_ptr<ClientNode> GetClient(long Uin);
extern void NotifyBorderGameCondition(std::shared_ptr<ClientNode> Client);
extern void ResponseGetRankedMatchShowInfo(std::shared_ptr<ClientNode> Client);
extern void ResponseGetNewHandCritBoxAwardCfg(UINT ItemId, std::shared_ptr<ClientNode> Client);
extern std::shared_ptr<ClientNode> SystemClient;
extern void NotifyBugletTalk(std::shared_ptr<ClientNode> Client, const char* Content, short clength, char BugletType);
//extern std::deque<std::shared_ptr<ClientNode>> RankedMatchQueue;
//extern std::deque<std::shared_ptr<ClientNode>> BorderMatchQueue;
extern std::deque<std::pair<std::shared_ptr<ClientNode>, std::chrono::steady_clock::time_point>> RankedMatchQueue;
extern std::deque<std::pair<std::shared_ptr<ClientNode>, std::chrono::steady_clock::time_point>> BorderMatchQueue;
extern std::recursive_mutex RankedQueueMutex;
extern std::recursive_mutex BorderQueueMutex;
extern void ResponseStartQuickMatchGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID);
extern void ResponseStartCheShenGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID);
extern void NotifyClientLoveBabyInfoWhenLogIn(std::shared_ptr<ClientNode> Client);
extern void RequestEnterMatchRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
extern void ResponseGetGroupFriendBaseInfo(std::shared_ptr<ClientNode> Client, UINT Uin, char* newname);
extern void RequestGetGroupFriendBaseInfo(std::shared_ptr<ClientNode> Client);
extern void RequestC2SSearchKartAltas(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
extern void RequestC2SGetKartAltasConfig(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
extern void NotifyTaskAward(std::shared_ptr<ClientNode> Client, UINT TaskID);
extern std::map<UINT, std::vector<CarTreasureInfo>> CommonBoxAwards;
extern void RequestShapeLegendOperate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);


void RequestReportSkillStoneTakeEffect(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestAddFriend(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void AckBeAddedAsFriend(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestEngage(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void AckBeEngaged(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestCancelEngage(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestAddCharm(std::shared_ptr<ClientNode> Client, UINT ItemID, UINT count);
void RequestAddSuperMoney(std::shared_ptr<ClientNode> Client, UINT ItemID, UINT count);
void RequestInvitePlayer(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseQuickRPC(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time, UINT Temp);
void ResponseQuickRPC2(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time, UINT Temp);
void RequestQuickRPC(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestCancelMatch(std::shared_ptr<ClientNode> Client);
UINT GetChange(UINT ShapeSuitID);

std::shared_ptr<RANDOMMAPPOOL> GetRandomMapPool(long PoolID);

std::shared_ptr<MAPPOOL> AllMap = NULL, BorderMap = NULL, EventMap = NULL, RankMap = NULL;
std::unordered_map<long, std::unique_ptr<PETITEM>> PetItems;//hashmap
std::map<long, std::shared_ptr<RANDOMMAPPOOL>> RandomMapPools;

//抽奖
std::unordered_map<long, std::unique_ptr<TREASUREBOX>> TreasureBoxs;//hashmap
//TREASUREBOX* GameEndBox = NULL;
std::vector<ItemInfo> GameEndBox;
std::map<UINT, std::shared_ptr<OpenBoxDef>> GameEndBox2Uin;
int GameEndBoxTime[4] = { 0,1,20,24 };
std::vector<ItemInfo> MagicLampItems;
std::shared_ptr<AuctionCfg> AuctionConfig = NULL;
std::unordered_map<long, std::shared_ptr<ItemInfo>> RegisterBoxs;//hashmap
std::unordered_map<long, std::shared_ptr<ItemInfo>> DayRaffleBoxs;//hashmap
std::shared_ptr<CarTreasure> CarTreasures;//夺宝配置
std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicFormationBoxs;
std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicExchangeShopCarBoxs;//hashmap
std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicExchangeShopPetBoxs;//hashmap
std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicExchangeShopSeatBoxs;//hashmap
std::shared_ptr<MIRACLECFG> MiracleCfg = NULL;
int MagicCupExchange[7];
std::map<UINT, std::shared_ptr<AppellationInfo>> Appellations; // 全部
std::mutex Mutex2;


std::set<UINT> AllGMUins;//全部gm
std::string AllGMMess;
std::set<UINT> ClothesGMUins;  //只能刷衣服
std::string ClothesGMMess;
std::string CarGMMess;
std::set<UINT> CarGMUins;  //只能刷车
std::set<UINT> AdminUins;  // 在文件顶部全局声明 AdminUins


std::shared_mutex  RefitDataMutex; //

void RequestCheckName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestRemoveSkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestCompositeSkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestRandomOneNobleID(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestReplaceNobleID(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestSecondRefitKart(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetWareHouseStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetWareHouseItemList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestDoWareHouseOperation(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestDoWareHouseOperationBatch(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestRemoveSkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestGetPwdRedEnvelopeContent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestSendPwdRedEnvelope(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestCheckName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestBatchDeleteItem(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetShortCutSet(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetMagicLampCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestMagicLampLightOn(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetCfgFile(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestGetLoveBabyInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestChangeLoverBabyEquipStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestDanceGetRoomList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestDanceEnterRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetMagicCupCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetTempBoxItemInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestMagicCupGetAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestMagicCupExchangePiece(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestGetCommonBoxAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void RequestChangeOBState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetDailyExtractShowInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDailyExtractInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);


extern int MagicLamp[4];
extern std::unordered_map<UINT, std::unique_ptr<Item>> Items;//hashmap
extern void NotifyClientUpdateMoney(std::shared_ptr<ClientNode> Client);
extern int MagicShop[2];
extern void NotifyAddPropBySkillStone(std::shared_ptr<ClientNode> Client, int StoneSkillType, short PropID, int PropIndex);
extern UINT PetItemID;
extern UINT PetItemNum;
extern UINT PetSuperMoney;
extern int MagicCup[2];
extern int DayMoeny;
extern int MagicExchangeShopItemId;
extern UINT isinvite;
extern int isMinimum;
extern int isLog;
extern int deleteflag;
extern int bodyflag;
extern int StoneLvSeed[3];
extern int Rankset[5];
extern int RankTimes[4];
extern int Borderset[6];
extern int LuckyLimit;
extern int NewKart;
extern int Keymode;
extern int PreventSDP[2];
extern int mode28346;
extern int BorderManTime;
extern int RankerManTime;
extern std::vector<std::string> BroadCast;
extern int BroadCaston;
extern std::string qqurl;
extern std::vector<ChallengeMapsInfo> ChallengeMaps;
extern std::string ChallengeUrl;
extern USHORT TcpPort;
extern std::mutex ChallengeMapsMutex;
extern std::vector<ChallengeMap> ChallengeAllMaps;
extern int ChallengeFileOn;
extern std::string ChallengeFilePath;
extern std::string IP;
extern std::vector<std::string> ChallengeCfgMsgs;
extern int ChangeTimes[4];
extern int Uinew;
extern int AutoReplyGuild;
extern double version;
extern std::vector<UINT> ChallengeForbidKart;
std::map<UINT, std::shared_ptr<LimitMsgInfo>> LimitMsgs;
std::map<UINT, std::shared_ptr<LimitMsgInfo>> Msgs;
std::vector<UINT> Uins;
std::set<int> Seqs;
std::vector<std::string> IpList;
std::mutex IpListMutex;
BYTE StartTimerChallenge2[1024] = { 0 };
BYTE StartTimerChallenge[1024] = { 0 };
std::vector<MechTreasureInfo> mechTreasureInfo;
std::vector<MechTreasureInfo> mechExchangeTreasureInfo;
std::vector<MapChallengeOpenMain> mapChallengeOpenMain;
std::vector<MapChallengeOpenMainMap> mapChallengeCreate;

std::wstring Utf8ToWide(const std::string& utf8) {
	if (utf8.empty()) return std::wstring();

	// 计算所需的宽字符缓冲区大小
	int wideLength = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	if (wideLength == 0) {
		throw std::runtime_error("Failed to convert UTF-8 to wide string");
	}

	// 分配缓冲区并转换
	std::wstring wideStr(wideLength, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wideStr[0], wideLength);

	// 移除多余的 null 终止符
	wideStr.resize(wideLength - 1);
	return wideStr;
}

std::string SqlStingUtf8(std::string str)
{

	try
	{
		return UTF2GB(str.c_str());
	}
	catch (...)
	{
		std::cerr << "SqlStingUtf8 Error" << std::endl;
	}
	return "";
}


std::string UTF2GB(const char* str)
{
	std::string result;
	WCHAR* strSrc;
	TCHAR* strRes;

	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	strRes = new TCHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, strRes, i, NULL, NULL);
	result = strRes;
	delete[] strSrc;
	delete[] strRes;
	return result;
}

char* G2U(const char* gb2312)
{
	wchar_t* wstr = NULL;
	char* str = NULL;
	try
	{
		int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
		wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr)
		{
			delete[] wstr;
			wstr = NULL;
		}
		return str;
	}
	catch (...)
	{
		std::cerr << "myutf8trans Error" << std::endl;
		if (wstr)
		{
			delete[] wstr;
		}
		if (str)
		{
			delete[] str;
		}
		return NULL;
	}
}


void OnRecvFromClient(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen)
{

	UINT itemid = 0;
	UINT temp = 0;
	UINT temp2 = 0;
	UINT temp3 = 0;
#ifdef DEBUG
	if (MsgID != 538/*AckAntiBot*/ && MsgID != 562/*AckDpData*/)
	{
		printf("[GameServer][ConnID:%d]RecvMsg:%d\n", Client->ConnID, MsgID);
	}
#endif
	if (!Client) return;

	if (!Client->IsLogin)
	{
		if (MsgID > 100)
		{
			return; //未登录则不能执行太多操作
		}
	}

	if (LimitMsgs[MsgID])
	{
		if (LimitMsgs[MsgID]->isInfo == 1) {
			printf("禁止运行的事件:%d\n", MsgID);
		}
		return;
	}


	/*if (Msgs[MsgID]) {
		auto it = std::find(Uins.begin(), Uins.end(), Client->Uin - 10000);

		if (it != Uins.end()) {
		}
		else {
			std::cout << "数据库未找到该玩家UIN：" << Client->Uin << "，IP地址：" << Client->Ip << std::endl;
			return;
		}
	}*/

	//if (isLog == 1 && MsgID != 153) {
	//	auto ServerLogger = spdlog::get("SpeedServer_logger");
	//	ServerLogger->info("[GameServer][UIN:{:d}][ConnID:{:d}][RecvMsg:{:d}][IP:{:s}]", Client->Uin, Client->ConnID, MsgID, Client->Ip);
	//	ServerLogger->flush();
	//}

	//if (MsgID != 538/*AckAntiBot*/ && MsgID != 562/*AckDpData*/)
	//{
	//	printf("[GameServer][ConnID:%d]RecvMsg:%d\n", Client->ConnID, MsgID);
	//	PrintfFile(Client, MsgID, Body, BodyLen);
	//}

	//printf("[GameServer][ConnID:%d]RecvMsg:%d\n", Client->ConnID, MsgID);
	try
	{
		switch (MsgID)
		{
			//MyServer
		case 1:
			//printf("MSG:%u\n", MsgID);
			MyRequestGetUin(Client, Body, BodyLen);
			break;
		case 2:

			//PrintfBody(Client, 0, Body, BodyLen);
			if (registerflag == 1)
			{
				/*if (isinvite == 1) {
					MyRequestRegisterNew(Client, Body, BodyLen);
				}
				else {
					MyRequestRegister(Client, Body, BodyLen);
				}*/
				MyRequestRegisterNew(Client, Body, BodyLen);
			}
			break;
			//DirServer
		case 90:
			//printf("MSG:%u\n", MsgID);
			RequestGetWorldInfo(Client, Body, BodyLen);
			break;
		case 91:
			//printf("MSG:%u\n", MsgID);
			RequestGetChatRoomList(Client, Body, BodyLen);
			break;
			//GameServer
		case 100:
			//printf("MSG:%u\n", MsgID);
			RequestLogin(Client, Body, BodyLen);
			break;
		case 101:

			RequestLogout(Client, Body, BodyLen);
			break;
		case 102:

			//PrintfBody(Client, 0, Body, BodyLen);
			RequestGetRoomList(Client, Body, BodyLen);
			break;
		case 103:
			//printf("MSG:%u\n", MsgID);
			//PrintfBody(Client, 0,Body, BodyLen);
			RequestCreateRoom(Client, Body, BodyLen);
			break;
		case 104:
			//printf("MSG:%u\n", MsgID);
			//请求进入房间
			//PrintfBody(Client, 0, Body, BodyLen);
			RequestEnterRoom(Client, Body, BodyLen);
			break;
		case 105:

			//系统某些情况会自动调用
			//printf("RequestLeaveRoom:%u\n", MsgID);
			//请求离开房间
			//PrintfBody(Client, 0, Body, BodyLen);
			RequestLeaveRoom(Client, Body, BodyLen);
			break;
		case 107:
			//PrintfBody(Client, 0, Body, BodyLen);
			RequestChangeSeatState(Client, Body, BodyLen);
			break;
		case 108:

			//PrintfBody(Client, 0, Body, BodyLen);
			RequestKickFromRoom(Client, Body, BodyLen);
			break;
		case 109:

			//PrintfBody(Client, 0, Body, BodyLen);
			RequestChangeReadyState(Client, Body, BodyLen);
			break;
		case 110://玩家点击开始游戏

			RequestStartGame(Client, Body, BodyLen);
			break;
			/*case 24220:
				RequestStartGame(Client, Body, BodyLen);
				break;*/
		case 111:
			//printf("MSG:%u\n", MsgID);
			RequestTalk(Client, Body, BodyLen);
			break;
		case 112:
			//printf("MSG:%u\n", MsgID);
			RequestHello(Client, Body, BodyLen);
			//NotifyTaskAward(Client, 148);//在线任务奖励 taskid:148-150
			break;
		case 120:

			if (Client->TaskID == 0)//紫钻练习场房间也会请求这个消息 需要过滤
				RequestPrepareReady(Client, Body, BodyLen);
			break;
		case 121:

			RequestChangeMap(Client, Body, BodyLen);
			break;
		case 122:

			// 存在空指针
			//PrintfBody(Client, 0, Body, BodyLen);
			RequestFindPlayerByQQ(Client, Body, BodyLen);
			break;
		case 123:
			//printf("MSG:%u\n", MsgID);
			//对局实时报告 频繁适度减少读取优化
			if (Client->IsFinish == TRUE)
				break;
			RequestReportCurrentInfo(Client, Body, BodyLen);
			break;
		case 124:

			RequestGetProp(Client, Body, BodyLen);
			break;
		case 126:// 请求获得道具 道具模式用的多
			//RequestPropEffectResult(Client, Body, BodyLen);
			break;
		case 127:// 请求获得道具 道具模式用的多
			//RequestChangeAimState(Client, Body, BodyLen);
			break;
		case 128:// 更改组队队伍
			RequestChangeTeam(Client, Body, BodyLen);
			break;
		case 130:

			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestChangeItemStatus(Client, Body, BodyLen);
			break;
		case 135:

			RequestGetTaskList(Client, Body, BodyLen);
			break;
		case 139:
			//printf("MSG:%u\n", MsgID);
			//直接结束单人任务
			//Read32(Body);
			//Read32(Body);
			Body += 8;
			temp = Read16(Body);
			//printf("temp:%d\n", temp);

			if (temp != 0x2)//练习场完成12 练习场手动退出4
			{
				//练习场完成会请求这个消息 然后请求RequestReportCurrentInfo
				SendToClient(Client, 532, (BYTE*)"\x00\x00", 2, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
				SendToClient(Client, 534, (BYTE*)"\x00\x00", 2, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
				if (temp == 4)
				{
					Client->TaskID = 0;
				}
				Client->OptNum = 0;
			}
			else
			{
				Client->OptNum += 1;//单人 碰撞 踩雷次数
				ResponseReportCurrentOpt(Client);
			}

			break;
		case 145:

			//RequestGetLicenseInfo(Client, Body, BodyLen);
			break;
		case 151:

			//RequestSwitchInterface(Client, Body, BodyLen);
			break;
		case 152:

			RequestGetSystemTaskList(Client, Body, BodyLen);
			break;
		case 153:
			//printf("RequestTransferByTCP:%u\n", MsgID);
			//对局请求 udp取消该请求 
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestTransferByTCP(Client, Body, BodyLen);
			break;
		case 162:
			//printf("MSG:%u\n", MsgID);
			RequestChangeGender(Client, Body, BodyLen);
			break;
		case 163:

			RequestNewGetFriendList(Client, Body, BodyLen);
			break;
		case 148:
			//好友列表信息 启动客户端时获取
			//PrintfBody(Client, MsgID, Body, BodyLen);
			//printf("ResponseGetGroupFriendBaseInfo\n");
			RequestGetGroupFriendBaseInfo(Client);
			//一次只能发送一个人 多次发送即可
			break;
		case 158:
			RequestUseItem(Client, Body, BodyLen);
			break;
		case 159:
			RequestInvitePlayer(Client, Body, BodyLen);
			break;
		case 114:
			//添加好友
			RequestAddFriend(Client, Body, BodyLen);
			break;
		case 507:
			//添加好友结果
			//PrintfBody(Client, MsgID, Body, BodyLen);
			AckBeAddedAsFriend(Client, Body, BodyLen);
			break;
		case 166:
			//订婚
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestEngage(Client, Body, BodyLen);
			break;
		case 167:
			RequestCancelEngage(Client, Body, BodyLen);
			break;
		case 572:
			//订婚结果

			AckBeEngaged(Client, Body, BodyLen);
			break;
		case 165:

			////当游戏模式是影子挑战 紫钻练习不触发这个消息可以取消重生宝石功能 极限挑战需要
			if (Client->GameID != 0 && Client->StoneOpenFlag == 0)
			{
				RequestUseProp2(Client, Body, BodyLen);
			}
			break;
		case 174:
			//printf("请求改装MSG:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestRefitKart(Client, Body, BodyLen);
			break;
		case 178:
			RequestUpdatePetName(Client, Body, BodyLen);
			break;
		case 179:
			//改变UDP状态
		//	RequestChangeUdpState(Client, Body, BodyLen);
			break;
		case 183:

			RequestSkipFreshTask(Client, Body, BodyLen);
			break;
		case 187:

			//RequestGetRandomTask(Client, Body, BodyLen);
			break;
		case 253:

			RequestDanceChangeMusic(Client, Body, BodyLen);
			break;
		case 254:

			RequestChangeScene(Client, Body, BodyLen);
			break;
		case 257:

			RequestDanceChangeMode(Client, Body, BodyLen);
			break;
		case 258:

			RequestDanceChangeRandKeyFlag(Client, Body, BodyLen);
			break;
		case 259:

			RequestDancePrepareReady(Client, Body, BodyLen);
			break;
		case 260:

			RequestDanceStepStatistics(Client, Body, BodyLen);
			break;
		case 261:

			RequestDeleteOneItem(Client, Body, BodyLen);
			break;
		case 262:

			RequestLoverDanceChangeOneMode(Client, Body, BodyLen);
			break;
		case 263:

			RequestLoverDanceStepStatistics(Client, Body, BodyLen);
			break;
		case 271:
			//printf("请求赛车改装信息MSG:%u\n", MsgID);
			RequestKartRefitInfo(Client, Body, BodyLen);
			break;
		case 277:

			RequestChangeModel(Client, Body, BodyLen);
			break;
		case 351:

			//PrintfBody(Client, 0, Body, BodyLen);
			RequestHeroMove(Client, Body, BodyLen);
			break;
		case 352:
			//printf("MSG:%u\n", MsgID);
			//进入户外休闲房间？
			RequestEnterOutdoor(Client, Body, BodyLen);
			break;
		case 353:
			//离开户外休闲房间
			//PrintfBody(Client, 0, Body, BodyLen);
			RequestLeaveOutdoor(Client, Body, BodyLen);
			break;
		case 357:

			//检查重复用户名
			RequestRegister2(Client, Body, BodyLen);
			break;
		case 360:

			RequestChangeChairState(Client, Body, BodyLen);
			break;
		case 363:

			//RequestGetMaster(Client, Body, BodyLen);
			break;
		case 373:
			//成就列表
			RequestGetAchieveList(Client, Body, BodyLen);
			break;
		case 376:

			RequestModifyPlayerSignature(Client, Body, BodyLen);
			break;
		case 452:

			//RequestGetShoppingCarItemList(Client, Body, BodyLen);
			break;
		case 461:
			// 存在空指针
		 //红人
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestTopList(Client, Body, BodyLen);
			break;
		case 499:

			//RequestTransferUDPOK(Client, Body, BodyLen);
			break;
		case 901:

			RequestInlaySkillStone(Client, Body, BodyLen);
			break;
		case 944:

			RequestFizzInfo(Client, Body, BodyLen);
			break;
		case 1500:

			RequestStartShadowChallenge(Client, Body, BodyLen);
			break;
		case 1501:
			//printf("MSG:%u\n", MsgID);
			RequestEndShadowChallenge(Client, Body, BodyLen);
			break;
		case 10501:

			RequestFishLogin(Client, Body, BodyLen);
			break;
		case 11451:

			RequestGetPlayerEventCfg(Client, Body, BodyLen);
			break;
		case 11452:

			RequestGetPlayerEvent(Client, Body, BodyLen);
			break;
		case 13710:
			//printf("MSG:%u\n", MsgID);//请求车库配置
			RequestC2SGetKartAltasConfig(Client, Body, BodyLen);
			break;
		case 13705:
			RequestC2SSearchKartAltas(Client, Body, BodyLen);
			break;
		case 317://请求获取赛车皮肤数据
			//printf("请求赛车皮肤配置MSG:%u\n", MsgID);
			RequestGetShapeRefitCfg(Client, Body, BodyLen);
			break;
		case 319:
			//printf("请求传奇皮肤操作:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestShapeLegendOperate(Client, Body, BodyLen);
			break;
		case 424:
			//printf("MSG:%u\n", MsgID);
			//对局请求退出消息424
			//影子挑战请求退出是1501
			RequestLeaveGame(Client, Body, BodyLen);
			break;
		case 1401:
			//签到页面
			RequestC2GSign3Operate(Client, Body, BodyLen);
			break;
		case 24029:
			//printf("活跃中心MSG:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestGetActivityCenterInfo(Client, Body, BodyLen);
			break;
		//case 16055:
		//	//printf("MSG:%u\n", MsgID);
		//	ResponseGetPrivilegeIntroInfo(Client, Body, BodyLen);
			break;
		case 20079:
			//模拟启动
		//	ResponseStartGame(Client, "暂未开放!");
		//	break;
			//PrintfBody(Client, MsgID, Body, BodyLen);
			Body += 8;
			temp = Read16(Body);//0x1立即竞速
			Body += 6;
			temp2 = Read32(Body);
			if (temp2 != 0)
			{
				temp3 = Read32(Body);//指定赛道匹配
			}
			//printf("MSG:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			//RequestAddToMatchQueue(Client, Body, BodyLen);

			if (temp == 0x8)
			{
				ResponseStartTimerChallenge2(Client);
				break;
				//龙晶大闯关
			}
			else if (temp == 0x0)//联机边境
			{
				if (Borderset[0] == 0)
				{
					ResponseStartGame(Client, "边境关闭!");
					break;
				}

				if (Client->GameBan > 0) {
					std::string msg = "由于系统检测到您启用第三方软件作弊，处罚封禁/C01【" + std::to_string(Client->GameBan) + "天】/C00边境，排位功能";
					ResponseStartGame(Client, msg.c_str());
					break;
				}

				if (Client->isManMachine == 1 && Borderset[1] == 1 && BorderManTime == 0)
				{
					ResponseStartQuickMatchGame(Client, temp, temp3);
				}
				else
				{
					ResponseStartBorderGame(Client, temp, temp3);
				}
				break;

			}
			else if (temp == 0x1)//单人立即竞速
			{
				//printf("temp:%u\n", temp);
				ResponseStartQuickMatchGame(Client, temp, temp3);
				break;

			}

			else if (temp == 0xa)//联机排位
			{
				if (Rankset[0] == 0)
				{
					ResponseStartGame(Client, "排位关闭!");
					break;
				}

				if (Client->GameBan > 0) {
					std::string msg = "由于系统检测到您启用第三方软件作弊，处罚封禁/C01【" + std::to_string(Client->GameBan) + "天】/C00边境，排位功能";
					ResponseStartGame(Client, msg.c_str());
					break;
				}

				auto now = std::chrono::system_clock::now();
				std::time_t now_time = std::chrono::system_clock::to_time_t(now);
				struct tm tm_now;
				localtime_s(&tm_now, &now_time);

				// 获取小时
				int hour = tm_now.tm_hour;

				// 判断是否在时间内
				if (!((hour >= RankTimes[0] && hour < RankTimes[1]) || (hour >= RankTimes[2] && hour < RankTimes[3]))) {
					std::string Reason = "排位开启时间在";
					Reason.append(std::to_string(RankTimes[0]));
					Reason.append("点到");
					Reason.append(std::to_string(RankTimes[1]));
					Reason.append("点,");
					Reason.append(std::to_string(RankTimes[2]));
					Reason.append("点到");
					Reason.append(std::to_string(RankTimes[3]));
					Reason.append("点。");
					ResponseStartGame(Client, Reason.c_str());
					break;
				}

				if (Client->isManMachine == 1 && Rankset[1] == 1 && RankerManTime == 0)
				{
					ResponseStartQuickMatchGame(Client, temp, temp3);
				}
				else
				{
					ResponseStartRankedGame(Client, temp, temp3);
				}

				break;
				//排位
			}
			break;
		case 20080:
			RequestCancelMatch(Client);
			break;
			//取消匹配
		case 20210:
			//组队匹配
			break;
		case 703:
			//RequestPVPHello
			SendToClient(Client, 703, (BYTE*)"\x00\x00", 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
			break;
		case 20082:
			RequestEnterMatchRoom(Client, Body, BodyLen);
			break;
		case 697:
			SendToClient(Client, 697, (BYTE*)"\x00\x00", 2, Client->ServerID, FE_PLAYER, Client->ServerID, Response);
			break;
		case 21110:
			RequestSecondRefitKart(Client, Body, BodyLen);
			break;
		case 21111:
			RequestSecondGetKartRefitLimitCfg(Client, Body, BodyLen);
			break;
		case 24201:
			RequestGetItemTimeLimtCfg(Client, Body, BodyLen);
			break;
		case 24202:
			//printf("MSG:%u\n", MsgID);
			//龙晶大闯关挑战配置
			//Test(Client, Body, BodyLen);
			RequestHyperSpaceJumboGetCfg(Client, Body, BodyLen);
			break;
		case 24215:
			//printf("MSG:%u\n", MsgID);
			RequestGetKeyTranslateInfoNew(Client, Body, BodyLen);
			break;
		case 24258:
			//printf("MSG:%u\n", MsgID);
			RequestTimerChallenge2ndMoreHallInfo(Client, Body, BodyLen);
			break;
		case 24391:
			//printf("MSG:%u\n", MsgID);
			RequestWeRelayCommonCfg(Client, Body, BodyLen);
			break;
		case 24465:
			RequestPreGetCommonBoxAward(Client, Body, BodyLen);
			break;
		case 24466:
			RequestGetCommonBoxAward(Client, Body, BodyLen);
			break;
		case 24721:
			//printf("MSG:%u\n", MsgID);
			RequestSSCOpenStatus(Client, Body, BodyLen);
			break;
		case 28421:
			//printf("MSG:%u\n", MsgID);
			RequestReplayOperate(Client, Body, BodyLen);
			break;
			//ShopServer
		case 20000:
			RequestBuyCommodity(Client, Body, BodyLen);
			break;
		case 316: //选择皮肤
			RequestSaveShapeRefitNew(Client, Body, BodyLen);
			break;
		case 168:
			//打开宝箱
			RequestOpenBox(Client, Body, BodyLen);
			break;
		case 138:
			//紫钻练习场
			//printf("MSG:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			Read32(Body);
			Read32(Body);
			StartSingleGame(Client, Body);
			/*if (Read32(Body) == 0x000006a5)
			{
				StartZiZuanGame(Client);
			}
			else
			{
				Body -= 4;
				StartSingleGame(Client, Body);
			}*/
			break;
		case 119:
			//漂移会触发 
			//printf("MSG:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestGetSuperN2O(Body, Client);
			break;
		case 10929:
			//printf("RequestGetBattleModeCarSkillInfoListMSG:%u\n", MsgID);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestGetBattleModeCarSkillInfoList(Client, Body, BodyLen);
			break;
			//case 20097:
				//Uin+Time+2字节flag1+2字节flag2+0x44
				//flag1=0xa 平地小喷
				//0x27 使用氮气
				//PrintfBody(Client, MsgID, Body, BodyLen);
				//break;
		case 24754:
			//ResponsePointChallengeOpenMainBoard(Client);//极限挑战
			break;
		case 24236:
			ResponseStartTimerChallenge(Client);
			break;
		case 24237:
			//RequestReportTimerChallengeEvent(Client,Body,BodyLen);
			break;
		case 24753:
			//极限挑战购买道具 key：586复活卡 588增时卡 590最后一搏
			//PrintfBody(Client, MsgID, Body, BodyLen);
			break;
		case 25000:
			ResponseGetRankedMatchShowInfo(Client);//排位
			break;
		case 131:
			RequestChangeIdentity(Client, Body, BodyLen);
			break;
		case 24254:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestReportTimerChallenge2ndEvent(Client, Body, BodyLen);
			break;
		case 24251:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestQuickRPC(Client, Body, BodyLen);
			break;
		case 313:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			Body += 8;
			itemid = Read32(Body);
			ResponseGetNewHandCritBoxAwardCfg(itemid, Client);
			break;
		case 20128://查询产出途径
			break;
		case 24756://幸运3选一 结果
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestGetGameEndBoxAward(Client, Body, BodyLen);
			break;
		case 28412://置顶
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestDoBagItemShowOperate(Client, Body, BodyLen);
			break;
		case 28379:
			//PrintfBody(Client, MsgID, Body, BodyLen);//赛道匹配
			ResponseGetQuickRaceTimeRandMap(Client);
			break;
		case 909://宝石发动通知
			RequestReportSkillStoneTakeEffect(Client, Body, BodyLen);
			break;
		case 134:
			//观战模式
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestChangeOBState(Client, Body, BodyLen);
			break;
		case 1652://摇号
			RequestRandomOneNobleID(Client, Body, BodyLen);
			break;
		case 1654:
			RequestReplaceNobleID(Client, Body, BodyLen);
			break;
		case 912:
			RequestRemoveSkillStone(Client, Body, BodyLen);
			break;
		case 905:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestCompositeSkillStone(Client, Body, BodyLen);
			break;
		case 420:
			//仓库状态
			RequestGetWareHouseStatus(Client, Body, BodyLen);
			break;
		case 421:
			//仓库列表
			RequestGetWareHouseItemList(Client, Body, BodyLen);
			break;
		case 422:
			//存取仓库
			RequestDoWareHouseOperation(Client, Body, BodyLen);
			break;
		case 21036:
			//批量仓库
			RequestDoWareHouseOperationBatch(Client, Body, BodyLen);
			break;
		case 24724:
			RequestGetPwdRedEnvelopeContent(Client, Body, BodyLen);
			break;
		case 24723://口令红包 输入口令
			RequestSendPwdRedEnvelope(Client, Body, BodyLen);
			break;
		case 133:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestCheckName(Client, Body, BodyLen);
			break;
		case 21035:
			RequestBatchDeleteItem(Client, Body, BodyLen);
			break;
		case 11356:
			RequestGetShortCutSet(Client, Body, BodyLen);
			break;
		case 24507:
			RequestGetMagicLampCfg(Client, Body, BodyLen);
			break;
		case 24508:
			RequestMagicLampLightOn(Client, Body, BodyLen);
			break;
		case 20002:
			RequestGetCfgFile(Client, Body, BodyLen);
			break;
		case 24352:
			RequestGetLoveBabyInfo(Client, Body, BodyLen);
			break;
		case 24386:
			RequestChangeLoverBabyEquipStatus(Client, Body, BodyLen);
			break;
		case 251:
			RequestDanceGetRoomList(Client, Body, BodyLen);
			break;
		case 252:
			RequestDanceEnterRoom(Client, Body, BodyLen);
			break;
		case 24726:
			RequestGetMagicCupCfg(Client, Body, BodyLen);
			break;
		case 24727:
			RequestGetTempBoxItemInfo(Client, Body, BodyLen);
			break;
		case 24728:
			RequestMagicCupGetAward(Client, Body, BodyLen);
			break;
		case 24729:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestMagicCupExchangePiece(Client, Body, BodyLen);
			break;
		case 15000:
			RequestCreateGuild(Client, Body, BodyLen);
			break;
		case 15002:
			RequestRemoveMember(Client, Body, BodyLen);
			break;
		case 15010:
			RequestApplyJoinGuild(Client, Body, BodyLen);
			break;
		case 15013:
			RequestReplyJoinGuild(Client, Body, BodyLen);
			break;
		case 15004:
			RequestGetGuildList(Client, Body, BodyLen);
			break;
		case 15006:
			RequestAppointedPosition(Client, Body, BodyLen);
			break;
		case 15003:
			RequestGuildDetail(Client, Body, BodyLen);
			break;
		case 15009:
			RequestFindGuild(Client, Body, BodyLen);
			break;
		case 713:
			RequestChangeRoomOwner(Client, Body, BodyLen);
			break;
		case 25130: // 每日一抽 打开
			ResponseGetDailyExtractShowInfo(Client, Body, BodyLen);
			break;
		case 25131: // 每日一抽 试试手气 抽奖
			ResponseDailyExtractInfo(Client, Body, BodyLen);
			break;
		case 4766:
			//Test(Client, Body, BodyLen);
			break;
		case 28435:
			RequestRankedMatchTopRaceOpt(Client, Body, BodyLen);//巅峰赛
			break;
		case 169: //休闲区功能
			RequestOutDoorEggInfo(Client, Body, BodyLen);
			break;
		case 170:
			RequestOutDoorSmashEgg(Client, Body, BodyLen);
			break;
		case 358:
			RequestUseHammer(Client, Body, BodyLen);
			break;
		case 359:
			RequestChangeHammerStatus(Client, Body, BodyLen);
			break;
		case 355:
			RequestHeroDoAction(Client, Body, BodyLen);
			break;
		case 350:
			RequestHeroPos(Client, Body, BodyLen);
			break;
		case 24470:
			RequestGetMagicHouseInfo(Client, Body, BodyLen);
			break;
		case 24471: // 魔法阵抽奖
			ResponseMagicHouseCallOn(Client, Body, BodyLen);
			break;
		case 24786: // 魔法阵抽奖 商店
			ResponseGetExchangeShopInfo(Client, Body, BodyLen);
			break;
		case 24787: // 魔法阵抽奖 商店 兑换道具
			ResponseDoShopExchange(Client, Body, BodyLen);
			break;
		case 20040: // 天天爱升级 领取经验
			//ResponseC2SReceiveTimesExperience(Client, Body, BodyLen);
			break;
		case 20041: // 天天爱升级 冻结经验
			//ResponseC2SFrostThawTimesExperience(Client, Body, BodyLen);
			break;
		case 20042: // 天天爱升级
			//ResponseC2SGetTimesExperienceInfo(Client, Body, BodyLen);
			break;
		case 15398:
			RequestChangePetEquipStatus(Client, Body, BodyLen);
			break;
		case 11526:
			//RequestC2GGetWizardBaseInfo(Client, Body, BodyLen);
			break;
		case 21150:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			//RequestPetTrialInfo(Client, Body, BodyLen);
			break;
		case 20101:
			RequestPetStrengthen(Client, Body, BodyLen);
			break;
		case 440:
			//RequestGetPetSkillList(Client, Body, BodyLen);
			break;
		case 24426:
			//RequestGetPetShowInfo(Client, Body, BodyLen);
			break;
		case 24528:
			//RequestGetPetInfo(Client, Body, BodyLen);
			break;
		case 1651: //自定义
			//PrintfBody(Client, MsgID, Body, BodyLen);
			break;
		case 11411:
			RequestGetAuctionCfg(Client, Body, BodyLen);
			break;
		case 11412:
			RequestGetParticipateAuction(Client, Body, BodyLen);
			break;
		case 11413:
			RequestGetCurAuctionInfo(Client, Body, BodyLen);
			break;
		case 11414:
			RequestGetAuctionList(Client, Body, BodyLen);
			break;
		case 11415:
			RequestBidAuctionCommodity(Client, Body, BodyLen);
			break;
		case 11418:
			RequestPayAuctionCommodity(Client, Body, BodyLen);
			break;
		case 11419:
			RequestGetSingleAuctionInfo(Client, Body, BodyLen);
			break;
		case 15017:
			RequestCheckGuildName(Client, Body, BodyLen);
			break;
		case 288:
			//PrintfBody(Client, MsgID, Body, BodyLen);
			RequestUpdateGuildName(Client, Body, BodyLen);
			break;
		case 21017:
			RequestOpSecondPass(Client, Body, BodyLen);
			break;
		case 24095: // 全人民送车大狂欢
			//ResponseCarLottoryGetInfo(Client, Body, BodyLen);
			break;
		case 1702: //对战纪录
			ResponseGetGameBaseRecordList(Client, Body, BodyLen);
			break;
		case 1703: //对战纪录
			RequestGetGameDetailRecordList(Client, Body, BodyLen);
			break;
		case 24028://通用函数（暂无用处）
			RequestReportCommonEvent(Client, Body, BodyLen);
			break;
		case 26114:
			RequestReportAntiCollisionDataBuff(Client, Body, BodyLen);
			break;
		case 1618:
			RequestWearAppellation(Client, Body, BodyLen);
			break;
		case 1619:
			RequestUnwearAppellation(Client, Body, BodyLen);
			break;
		case 25010://排位 禁图
			//RequestRankedMatchDoBanMap(Client, Body, BodyLen);
			break;
			//车神
		case 11210:
			ResponseC2GGetChallengeInfo(Client, Body, BodyLen);
			break;
		case 11211:
			ResponseC2GGetGuildChallengeInfo(Client, Body, BodyLen);
			break;
		case 11212:
			ResponseC2GGetMapsChallengeInfo(Client, Body, BodyLen);
			break;
		case 11213:
			ResponseC2GGetMapChallengeRank(Client, Body, BodyLen);
			break;
		case 11214:
			ResponseC2GGetRecentChallengeMsg(Client, Body, BodyLen);
			break;
		case 11216:
			RequestC2GGetMapOwner(Client, Body, BodyLen);
			break;
		case 11217:
			RequestC2GChallenge(Client, Body, BodyLen);
			break;
		case 11220:
			//PrintfBody(Client, 11210, Body, BodyLen);
			NotifyC2GLoadChallengeFileResult(Client, 0);
			break;
		case 28414:
			//PrintfBody(Client, 11210, Body, BodyLen);
			RequestReportCosOpResult(Client, Body, BodyLen);
			break;
		case 11200:
			RequestC2FUpLoad(Client, Body, BodyLen);
			break;
		case 11222:
			//PrintfBody(Client, 11222, Body, BodyLen);
			NotifyC2GStartChallengeResult(Client, Body, BodyLen);
			break;
		case 11225:
			ResponseC2GUpdateChallengeRank(Client, Body, BodyLen);
			break;
		case 11229:
			RequestC2GLoadFile(Client, Body, BodyLen);
			break;
		case 11233: //显示打榜地图
			ResponseC2GChallengeCfg(Client, Body, BodyLen);
			break;
		case 24617: //大奖赛开黑房
			Request24617(Client, Body, BodyLen);
			break;
		case 24610: //大奖赛
			ResponseLuckMatchBetAwardInfo(Client, Body, BodyLen);
			break;
		case 812:
			RequestLmtBuglet(Client, Body, BodyLen);
			break;
		case 381:
			RequestLmtEnterChatRoom(Client, Body, BodyLen);
			break;
		case 382:
			ResponseLmtUpdateStatus(Client, Body, BodyLen);
			break;
		case 275:
			RequestChangeRecommandStatus(Client, Body, BodyLen);
			break;
		case 24609:
			ResponseLuckMatchShowInfo(Client, Body, BodyLen);
			break;
		case 400: //大奖赛开始
			ResponseParticipateLuckyMatch(Client, Body, BodyLen);
			break;
		case 401: //大奖赛取消
			ResponseCancelParticipateLuckyMatch(Client, Body, BodyLen);
			break;
		case 16060:
			ResponseGetExclusiveSpeedWayInfo(Client, Body, BodyLen);
			break;
		case 24085:
			ResponsePRGetInfo(Client, Body, BodyLen);
			break;
		case 11471:
			RequestGetConsumeScoreConfUrl(Client, Body, BodyLen);
			break;
		case 11474: //精灵世界
			ResponseGetWizardHomeBaseInfo(Client, Body, BodyLen);
			break;
		case 28419:
			RequestQsLabCommittalTaskInfo(Client, Body, BodyLen);
			break;
		case 28380:
			RequestGetTaskAwardInfo(Client, Body, BodyLen);
			break;
		default:
			//printf("未知消息:%u\n",MsgID);
			//fflush(stdout);
			//PrintfBody(Client, MsgID, Body, BodyLen);
			break;
		}
	}
	catch (const std::exception& e)
	{
		printf("发生异常 %d: %s\n", MsgID, e.what());
		auto ServerLogger = spdlog::get("SpeedServer_logger");
		if (ServerLogger) {
			ServerLogger->error("[发生异常MsgID:{:d}] 错误信息: {}", MsgID, e.what());
			ServerLogger->flush();
		}
	}
	catch (...)
	{
		printf("发生异常 %d\n", MsgID);
		//添加日志输出
		auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
		ServerLogger->info("[发生异常MsgID:{:d}]", MsgID);
		ServerLogger->flush();
	}
}
////void PrintfFile(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen)
////{
////	std::lock_guard<std::mutex> lg(LogFileMutex);
////	fprintf(LogFile, "Uin:%d ConnUin:%d MsgID:%d \n", Client->Uin, Client->ConnIDUin, MsgID);
////	fflush(LogFile);
////}

void RequestGetTaskAwardInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Type = Read32(p);
	UINT TaskNum = Read32(p);
	std::vector<UINT> TaskList;
	for (size_t i = 0; i < TaskNum; i++)
	{
		TaskList.push_back(Read32(p));
	}

	UCHAR TaskStrNum = Read8(p);
	std::string TaskStr = ReadString(p, TaskStrNum);

	ResponseGetTaskAwardInfo(Client, Type, TaskList, TaskStr);
}
void ResponseGetTaskAwardInfo(std::shared_ptr<ClientNode> Client, UINT Type, std::vector<UINT> TaskList, std::string TaskStr)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write32(p, Type); //Type
	WriteString_MC(p, TaskStr.c_str());
	Write32(p, TaskList.size()); //TaskInfoNum
	for (size_t i = 0; i < TaskList.size(); i++)
	{ //TaskAwardInfo
		BYTE* pTaskAwardInfo = p;
		Write16(pTaskAwardInfo, 0); //len

		Write32(pTaskAwardInfo, TaskList[i]); //TaskID
		Write32(pTaskAwardInfo, 0); //FirstAwardNum
		for (size_t i = 0; i < 0; i++)
		{ //TaskFirstAward
			BYTE* pTaskFirstAward = pTaskAwardInfo;
			Write16(pTaskFirstAward, 0); //len

			Write32(pTaskFirstAward, 0); //AwardMoney
			Write32(pTaskFirstAward, 0); //TotalMoney
			Write32(pTaskFirstAward, 0); //AwardSuperMoney
			Write32(pTaskFirstAward, 0); //TotalSuperMoney
			Write32(pTaskFirstAward, 0); //AwardExp
			Write32(pTaskFirstAward, 0); //TotalExp
			Write32(pTaskFirstAward, 0); //AwardItemNum
			for (size_t i = 0; i < 0; i++)
			{ //AwardItemInfo
				BYTE* pAwardItemInfo = pTaskFirstAward;
				Write16(pAwardItemInfo, 0); //len

				Write32(pAwardItemInfo, 0); //ItemID
				Write32(pAwardItemInfo, 0); //ItemNum
				Write32(pAwardItemInfo, 0); //AvailPeriod
				Write8(pAwardItemInfo, 0); //Status
				WriteUn(pAwardItemInfo, 0); //ObtainTime
				Write32(pAwardItemInfo, 0); //OtherAttribute
				Write16(pAwardItemInfo, 0); //ItemType

				len = pAwardItemInfo - pTaskFirstAward;
				Set16(pTaskFirstAward, (WORD)len);
				pTaskFirstAward += len;
			}
			Write32(pTaskFirstAward, 0); //AwardCoupons
			Write32(pTaskFirstAward, 0); //TotalCoupons
			Write32(pTaskFirstAward, 0); //AwardLuckMoney
			Write32(pTaskFirstAward, 0); //TotalLuckMoney
			Write32(pTaskFirstAward, 0); //AwardTeamWorkExp
			Write32(pTaskFirstAward, 0); //TotalTeamWorkExp
			Write32(pTaskFirstAward, 0); //AwardWlCash
			Write32(pTaskFirstAward, 0); //AwardWlCrystal
			Write32(pTaskFirstAward, 0); //AwardWlDegree
			Write32(pTaskFirstAward, 0); //ChumCircleActivity
			Write32(pTaskFirstAward, 0); //GuildScore
			Write16(pTaskFirstAward, 0); //AppellcationID
			Write16(pTaskFirstAward, 0); //ScoreJingJi
			Write8(pTaskFirstAward, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtendInfoAward
				BYTE* pExtendInfoAward = pTaskFirstAward;
				Write16(pExtendInfoAward, 0); //len

				Write16(pExtendInfoAward, 0); //Key
				Write32(pExtendInfoAward, 0); //AddValue
				Write32(pExtendInfoAward, 0); //TotalValue
				Write8(pExtendInfoAward, 0); //ShowPriority
				Write16(pExtendInfoAward, 0); //BuyNeedScore
				Write8(pExtendInfoAward, 0); //OneMatchMaxNum

				len = pExtendInfoAward - pTaskFirstAward;
				Set16(pTaskFirstAward, (WORD)len);
				pTaskFirstAward += len;
			}
			Write32(pTaskFirstAward, 0); //LiveExp
			Write32(pTaskFirstAward, 0); //AwardSpeedCoin
			Write32(pTaskFirstAward, 0); //TotalSpeedCoin

			len = pTaskFirstAward - pTaskAwardInfo;
			Set16(pTaskAwardInfo, (WORD)len);
			pTaskAwardInfo += len;
		}
		Write32(pTaskAwardInfo, 0); //OtherAwardNum
		for (size_t i = 0; i < 0; i++)
		{ //TaskOtherAward
			BYTE* pTaskOtherAward = pTaskAwardInfo;
			Write16(pTaskOtherAward, 0); //len

			Write32(pTaskOtherAward, 0); //AwardMoney
			Write32(pTaskOtherAward, 0); //TotalMoney
			Write32(pTaskOtherAward, 0); //AwardSuperMoney
			Write32(pTaskOtherAward, 0); //TotalSuperMoney
			Write32(pTaskOtherAward, 0); //AwardExp
			Write32(pTaskOtherAward, 0); //TotalExp
			Write32(pTaskOtherAward, 0); //AwardItemNum
			for (size_t i = 0; i < 0; i++)
			{ //AwardItemInfo
				BYTE* pAwardItemInfo = pTaskOtherAward;
				Write16(pAwardItemInfo, 0); //len

				Write32(pAwardItemInfo, 0); //ItemID
				Write32(pAwardItemInfo, 0); //ItemNum
				Write32(pAwardItemInfo, 0); //AvailPeriod
				Write8(pAwardItemInfo, 0); //Status
				WriteUn(pAwardItemInfo, 0); //ObtainTime
				Write32(pAwardItemInfo, 0); //OtherAttribute
				Write16(pAwardItemInfo, 0); //ItemType

				len = pAwardItemInfo - pTaskOtherAward;
				Set16(pTaskOtherAward, (WORD)len);
				pTaskOtherAward += len;
			}
			Write32(pTaskOtherAward, 0); //AwardCoupons
			Write32(pTaskOtherAward, 0); //TotalCoupons
			Write32(pTaskOtherAward, 0); //AwardLuckMoney
			Write32(pTaskOtherAward, 0); //TotalLuckMoney
			Write32(pTaskOtherAward, 0); //AwardTeamWorkExp
			Write32(pTaskOtherAward, 0); //TotalTeamWorkExp
			Write32(pTaskOtherAward, 0); //AwardWlCash
			Write32(pTaskOtherAward, 0); //AwardWlCrystal
			Write32(pTaskOtherAward, 0); //AwardWlDegree
			Write32(pTaskOtherAward, 0); //ChumCircleActivity
			Write32(pTaskOtherAward, 0); //GuildScore
			Write16(pTaskOtherAward, 0); //AppellcationID
			Write16(pTaskOtherAward, 0); //ScoreJingJi
			Write8(pTaskOtherAward, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtendInfoAward
				BYTE* pExtendInfoAward = pTaskOtherAward;
				Write16(pExtendInfoAward, 0); //len

				Write16(pExtendInfoAward, 0); //Key
				Write32(pExtendInfoAward, 0); //AddValue
				Write32(pExtendInfoAward, 0); //TotalValue
				Write8(pExtendInfoAward, 0); //ShowPriority
				Write16(pExtendInfoAward, 0); //BuyNeedScore
				Write8(pExtendInfoAward, 0); //OneMatchMaxNum

				len = pExtendInfoAward - pTaskOtherAward;
				Set16(pTaskOtherAward, (WORD)len);
				pTaskOtherAward += len;
			}
			Write32(pTaskOtherAward, 0); //LiveExp
			Write32(pTaskOtherAward, 0); //AwardSpeedCoin
			Write32(pTaskOtherAward, 0); //TotalSpeedCoin

			len = pTaskOtherAward - pTaskAwardInfo;
			Set16(pTaskAwardInfo, (WORD)len);
			pTaskAwardInfo += len;
		}
		Write32(pTaskAwardInfo, 0); //DBTaskInfoNum
		for (size_t i = 0; i < 0; i++)
		{ //DBTaskInfo
			BYTE* pDBTaskInfo = pTaskAwardInfo;
			Write16(pDBTaskInfo, 0); //len

			Write32(pDBTaskInfo, 0); //TaskID
			Write16(pDBTaskInfo, 0); //OptionID
			Write8(pDBTaskInfo, 0); //Status
			Write8(pDBTaskInfo, 0); //PrevStatus
			Write32(pDBTaskInfo, 0); //ProgressRate
			Write32(pDBTaskInfo, 0); //Other
			Write32(pDBTaskInfo, 0); //TakeTime
			Write32(pDBTaskInfo, 0); //LastUpdateTime

			len = pDBTaskInfo - pTaskAwardInfo;
			Set16(pTaskAwardInfo, (WORD)len);
			pTaskAwardInfo += len;
		}
		Write16(pTaskAwardInfo, 0); //FinTaskNeedNum
		Write8(pTaskAwardInfo, 0); //TaskParaToClientNum
		Write32(pTaskAwardInfo, 0); //TaskParaToClient[]

		len = pTaskAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 28380, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestC2GGetMapOwner(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT MapID = Read16(p);
	bool Lastest = Read8(p);
	ResponseC2GGetMapOwner(Client, MapID);
}

void ResponseC2GGetMapOwner(std::shared_ptr<ClientNode> Client, UINT MapId)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, MapId); //MapID
	{ //Brief
		BYTE* pBrief = p;
		Write16(pBrief, 0); //len

		Write16(pBrief, MapId); //MapID
		Write32(pBrief, 0); //ChallengeCnt
		Write16(pBrief, 0); //FreeRank
		Write32(pBrief, 0); //MaxFiniTime
		Write32(pBrief, 0); //MinFiniTime

		len = pBrief - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //HasOwner
	for (size_t i = 0; i < 0; i++)
	{ //MapOwner
		BYTE* pMapOwner = p;
		Write16(pMapOwner, 0); //len

		Write16(pMapOwner, 0); //Rank
		Write32(pMapOwner, 0); //Uin
		WriteString_MC(pMapOwner, "屌大"); //NameLen
		Write32(pMapOwner, 0); //GuildID
		Write8(pMapOwner, 0); //GuildNameLen
		Write32(pMapOwner, 0); //UseTime
		Write32(pMapOwner, 0); //KartID
		Write16(pMapOwner, 0); //UUidLen

		len = pMapOwner - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 11216, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseC2GGetChallengeInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	//std::cout << 11210 << std::endl;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //CostDayPoint
	Write16(p, 10); //DayPoint
	Write16(p, 0); //DayGuildPoint
	Write16(p, 0); //CostDayCheer
	Write16(p, 10); //DayCheer
	Write32(p, 0); //CheerValue


	len = p - buf.data();
	SendToClient(Client, 11210, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);


}


void ResponseC2GGetGuildChallengeInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	//std::cout << 11211 << std::endl;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	Write32(p, Client->Uin); //Uin
	Write32(p, Client->GuildID); //DstGuildID

	std::vector<ChallengeMap> ChallengeMapsInfo;
	SelectChallengeMap(Client, ChallengeMapsInfo);
	UINT MapID = 0;
	UINT Rank = 0;
	UINT UseTime = 0;
	UINT HeroUin = 0;
	UINT KartID = 0;
	std::string HeroName = "";
	if (ChallengeMapsInfo.size() > 0) {
		MapID = ChallengeMapsInfo[0].MapID;
		Rank = ChallengeMapsInfo[0].RankID;
		UseTime = ChallengeMapsInfo[0].UseTime;
		HeroUin = ChallengeMapsInfo[0].Uin;
		KartID = ChallengeMapsInfo[0].KartID;
		HeroName = ChallengeMapsInfo[0].Name;
	}
	{ //Info
		BYTE* pInfo = p;
		Write16(pInfo, 0); //len

		Write16(pInfo, MapID); //MapID
		Write16(pInfo, Rank); //Rank
		Write32(pInfo, UseTime); //UseTime
		Write32(pInfo, HeroUin); //HeroUin
		Write32(pInfo, KartID); //KartID
		if (HeroName.length() > 0) {
			WriteString_MC(pInfo, HeroName.c_str()); //HeroNameLen
		}
		else {
			Write8(pInfo, 0); //HeroNameLen
		}

		Write32(pInfo, 0); //Victory
		Write32(pInfo, 0); //Failure
		Write32(pInfo, 0); //DayChallenge
		Write16(pInfo, 0); //RecentMapID
		Write16(pInfo, 0); //RecentRankID
		Write32(pInfo, 0); //CheerValue
		Write32(pInfo, 0); //DayCheerCnt
		Write32(pInfo, 0); //DayCheerValue

		len = pInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //Addition
		BYTE* pAddition = p;
		Write16(pAddition, 0); //len

		Write16(pAddition, 0); //ExpAddition
		Write16(pAddition, 0); //WageAddition

		len = pAddition - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //HasExInfo
	for (size_t i = 0; i < 0; i++)
	{ //ExInfo
		BYTE* pExInfo = p;
		Write16(pExInfo, 0); //len

		Write16(pExInfo, 0); //LastSeasonMapID
		Write16(pExInfo, 0); //LastSeasonRank
		Write16(pExInfo, 0); //HistoryBestMapID
		Write16(pExInfo, 0); //HistoryBestRank
		Write32(pExInfo, 0); //TotalVictory
		Write32(pExInfo, 0); //TotalFailure
		Write32(pExInfo, 0); //HateGuildID

		len = pExInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //LogNum
	for (size_t i = 0; i < 0; i++)
	{ //Log
		BYTE* pLog = p;
		Write16(pLog, 0); //len

		Write16(pLog, 0); //SeasonID
		Write8(pLog, 0); //Type
		Write32(pLog, 0); //SrcGuildID
		WriteString(pLog, 0); //SrcGuildName[]
		Write32(pLog, 0); //Uin
		WriteString(pLog, 0); //NickName[]
		Write32(pLog, 0); //Kart
		Write16(pLog, 0); //MapID
		Write16(pLog, 0); //RankID
		Write32(pLog, 0); //UseTime
		Write32(pLog, 0); //Time

		len = pLog - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //Type

	len = p - buf.data();
	SendToClient(Client, 11211, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseC2GGetMapsChallengeInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, ChallengeMaps.size()); //MapNum
	for (size_t i = 0; i < ChallengeMaps.size(); i++)
	{ //MapStatus
		BYTE* pMapStatus = p;
		Write16(pMapStatus, 0); //len

		Write16(pMapStatus, ChallengeMaps[i].MapID); //MapID
		Write32(pMapStatus, 0); //ChallengeCnt
		Write16(pMapStatus, 0); //FreeRank
		Write32(pMapStatus, 0); //MaxFiniTime
		Write32(pMapStatus, 0); //MinFiniTime

		len = pMapStatus - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 11212, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

std::string ReadHexToString(BYTE* data, size_t len) {
	std::ostringstream oss; // 使用ostringstream来格式化输出
	for (size_t i = 0; i < len; ++i) {
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
	}
	return oss.str(); // 获取最终的字符串
}

ChallengeMap* GetChallengeMapByUUid(const std::string& uuid) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [&uuid](const ChallengeMap& map) {
		return map.UUid == uuid;
		});

	return (it != ChallengeAllMaps.end()) ? &(*it) : nullptr; // 返回找到的指针或nullptr
}

ChallengeMap* GetChallengeMapByMapIdAndRank(UINT mapId, USHORT rank) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [mapId, rank](const ChallengeMap& map) {
		return map.MapID == mapId && map.RankID == rank;
		});

	return (it != ChallengeAllMaps.end()) ? &(*it) : nullptr; // 返回找到的指针或nullptr
}

bool UpdateChallengeMapStatus(UINT mapId, USHORT rank, int newStatus) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [mapId, rank](const ChallengeMap& map) {
		return map.MapID == mapId && map.RankID == rank;
		});

	if (it != ChallengeAllMaps.end()) {
		it->Status = newStatus; // 更新状态
		return true; // 更新成功
	}

	return false; // 未找到对应的 ChallengeMap
}

bool UpdateChallengeMapMD5(UINT mapId, USHORT rank, std::string MD5) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [mapId, rank](const ChallengeMap& map) {
		return map.MapID == mapId && map.RankID == rank;
		});

	if (it != ChallengeAllMaps.end()) {
		it->FileMd5 = MD5; // 更新状态
		return true; // 更新成功
	}

	return false; // 未找到对应的 ChallengeMap
}

bool RemoveChallengeMap(UINT mapId, USHORT rank) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	auto it = std::remove_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [mapId, rank](const ChallengeMap& map) {
		return map.MapID == mapId && map.RankID == rank;
		});

	if (it != ChallengeAllMaps.end()) {
		ChallengeAllMaps.erase(it, ChallengeAllMaps.end());
		return true; // 删除成功
	}

	return false; // 未找到对应的 ChallengeMap
}

void FindOrAddChallengeMap(std::shared_ptr<ClientNode> Client, UINT mapId, USHORT rankId) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	// 查找是否存在对应的 ChallengeMap
	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [mapId, rankId](const ChallengeMap& map) {
		return map.MapID == mapId && map.RankID == rankId;
		});

	if (it == ChallengeAllMaps.end()) {
		// 如果不存在，创建新的 ChallengeMap 并添加
		ChallengeMap newMap;
		newMap.Uin = Client->Uin;
		newMap.MapID = mapId;
		newMap.RankID = rankId;
		newMap.GuildID = Client->GuildID;
		newMap.KartID = Client->KartID;
		newMap.Name = Client->NickName;
		newMap.GuildName = Client->GuildName;
		newMap.UseTime = 99999999;
		newMap.Status = 1;

		ChallengeAllMaps.push_back(newMap);
	}
}


bool UpdateChallengeMapUUID(std::shared_ptr<ClientNode> Client) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	// 获取 MapID 和 RankID
	UINT MapID = Client->ChallengeMaps[0].MapID;
	USHORT RankID = Client->ChallengeMaps[0].RankID;

	// 查找对应的 ChallengeMap
	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [MapID, RankID](const ChallengeMap& map) {
		return map.MapID == MapID && map.RankID == RankID;
		});

	if (it != ChallengeAllMaps.end()) {
		it->Uin = Client->Uin;
		it->MapID = Client->ChallengeMaps[0].MapID;
		it->RankID = Client->ChallengeMaps[0].RankID;
		it->GuildID = Client->GuildID;
		it->UUid = Client->ChallengeMaps[0].UUid;
		it->UseTime = Client->ChallengeMaps[0].UseTime;
		it->KartID = Client->KartID;
		it->Name = Client->NickName;
		it->GuildName = Client->GuildName;
		it->FileMd5 = Client->ChallengeMaps[0].FileMd5;
		it->LastUUid = Client->ChallengeMaps[0].LastUUid;
		it->KeepCnt = 0;
		it->ChallengeCnt = 0;


		/*it->Uin = Client->ChallengeMaps[0].Uin;
		it->MapID = Client->ChallengeMaps[0].MapID;
		it->RankID = Client->ChallengeMaps[0].RankID;
		it->GuildID = Client->ChallengeMaps[0].GuildID;
		it->UUid = Client->ChallengeMaps[0].UUid;
		it->UseTime = Client->ChallengeMaps[0].UseTime;
		it->KartID = Client->ChallengeMaps[0].KartID;
		it->Name = Client->ChallengeMaps[0].Name;
		it->GuildName = Client->ChallengeMaps[0].GuildName;
		it->FileMd5 = Client->ChallengeMaps[0].FileMd5;
		it->LastUUid = Client->ChallengeMaps[0].LastUUid;
		it->KeepCnt = Client->ChallengeMaps[0].KeepCnt;
		it->ChallengeCnt = Client->ChallengeMaps[0].ChallengeCnt;*/
		return true; // 更新成功
	}

	return false; // 未找到对应的 ChallengeMap
}

bool UpdateChallengeMap(std::vector<ChallengeMap> ChallengeMaps) {

	if (ChallengeMaps.size() != 1) return 0;
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);

	// 获取 MapID 和 RankID
	UINT MapID = ChallengeMaps[0].MapID;
	USHORT RankID = ChallengeMaps[0].RankID;

	// 查找对应的 ChallengeMap
	auto it = std::find_if(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), [MapID, RankID](const ChallengeMap& map) {
		return map.MapID == MapID && map.RankID == RankID;
		});

	if (it != ChallengeAllMaps.end()) {
		it->Uin = ChallengeMaps[0].Uin;
		it->MapID = ChallengeMaps[0].MapID;
		it->RankID = ChallengeMaps[0].RankID;
		it->GuildID = ChallengeMaps[0].GuildID;
		it->UUid = ChallengeMaps[0].UUid;
		it->UseTime = ChallengeMaps[0].UseTime;
		it->KartID = ChallengeMaps[0].KartID;
		it->Name = ChallengeMaps[0].Name;
		it->GuildName = ChallengeMaps[0].GuildName;
		it->FileMd5 = ChallengeMaps[0].FileMd5;
		it->LastUUid = ChallengeMaps[0].LastUUid;
		it->KeepCnt = ChallengeMaps[0].KeepCnt;
		it->ChallengeCnt = ChallengeMaps[0].ChallengeCnt;
		return true; // 更新成功
	}

	return false; // 未找到对应的 ChallengeMap
}


void AddChallengeMap(const ChallengeMap& newMap) {
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);
	ChallengeAllMaps.push_back(newMap); // 添加新的 ChallengeMap
}



void RequestC2GGetMapChallengeRank()
{
	std::string inPath = ".\\MapChallenge\\*.yml";
	//用于查找的句柄
	long handle;
	struct _finddata_t fileinfo;
	//第一次查找
	handle = _findfirst(inPath.c_str(), &fileinfo);
	if (handle == -1)
		return;

	YAML::Node config;
	std::string Byte;
	try
	{
		do
		{
			std::string outPath(".\\MapChallenge\\");
			outPath.append(fileinfo.name);

			config = YAML::LoadFile(outPath);
			//printf("TreasureID:%d\n", atoi(fileinfo.name));
			Byte = config["Byte"].as<std::string>();
			const unsigned char* Msg = EHexToByte(Byte.data());
			BYTE* p = (BYTE*)Msg;

			UINT Result = Read16(p);
			UINT Uin = Read32(p);

			UINT Mapid = Read16(p);
			UINT HeadId = Read16(p);
			UINT tailId = Read16(p);
			UINT Totalid = Read16(p);

			UINT shangbangs = Read16(p);
			DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("delete from mapchallengerank WHere mapid =? "));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Mapid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();

			for (size_t i = 0; i < shangbangs; i++)
			{

				BYTE* pMapRankEntry = p;
				UINT Len = Read16(pMapRankEntry);
				UINT Mapid = Read16(pMapRankEntry);
				UINT Rank = Read16(pMapRankEntry);
				UINT Uin = Read32(pMapRankEntry);
				UINT Namelen = Read8(pMapRankEntry);
				std::string NickName = ReadString(pMapRankEntry, Namelen);

				UINT GuildID = Read32(pMapRankEntry);
				UINT GuildNamelen = Read8(pMapRankEntry);
				std::string GuildName = ReadString(pMapRankEntry, GuildNamelen);

				UINT UseTime = Read32(pMapRankEntry);
				UINT KartId = Read32(pMapRankEntry);
				UINT ShadowFileSize = Read32(pMapRankEntry);

				std::string FileMd5 = ReadHexToString(pMapRankEntry, 16);
				pMapRankEntry += 16;
				UINT UUidLen = Read16(pMapRankEntry);
				std::string UUid = ReadString(pMapRankEntry, UUidLen);

				UINT LastUUidLen = Read16(pMapRankEntry);
				std::string LastUUid = ReadString(pMapRankEntry, LastUUidLen);

				UINT UpdateTime = Read32(pMapRankEntry);
				UINT Status = Read8(pMapRankEntry);
				UINT ReportedNum = Read32(pMapRankEntry);
				UINT KeepCnt = Read32(pMapRankEntry);
				UINT ChallengeCnt = Read32(pMapRankEntry);
				UINT EmpMode = Read8(pMapRankEntry);
				p += Len;



				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO mapchallengerank(Mapid,RankID,Uin,NickName,GuildID,GuildName,UseTime,KartId,FileMd5,UUid,LastUUid,KeepCnt,ChallengeCnt) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Mapid);
				prep_stmt->setUInt(2, Rank);
				prep_stmt->setUInt(3, Uin);
				prep_stmt->setString(4, G2U(NickName.c_str()));
				prep_stmt->setUInt(5, GuildID);
				prep_stmt->setString(6, G2U(GuildName.c_str()));
				prep_stmt->setUInt(7, UseTime);
				prep_stmt->setUInt(8, KartId);
				prep_stmt->setString(9, FileMd5.c_str());
				prep_stmt->setString(10, UUid);
				prep_stmt->setString(11, LastUUid);
				prep_stmt->setUInt(12, KeepCnt);
				prep_stmt->setUInt(13, ChallengeCnt);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}


		} while (!_findnext(handle, &fileinfo));
		std::cout << "写出车神表成功" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "RequestC2GGetMapChallengeRank Error" << std::endl;
	}






}

void ResponseC2GGetMapChallengeRank(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {

	BYTE* p = Body;
	WORD len;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT Mapid = Read16(p);
	UINT HeadId = Read16(p);
	UINT tailId = Read16(p);
	//std::cout << 11213 << std::endl;

	//std::string Byte = "00006706FA52015905D805DC05DC0001008A015905D875BEFF700EC4E3BDE3CBB5CED2CCABB8F8C1A60015B9FE0CC6DFD7DAD7EFD8BC7465616D000203A0000031B000000000009D9810B87654FD18CEC564FCE26F66001761317331305331323030317430663430313634363839340017613173313053313230303174306634303136343438353166F92DE600000000000000000C0000001B000001000300";
	//const unsigned char* Msg11213 = EHexToByte(Byte.data());
	//SendToClient(Client, 11213, (const BYTE*)Msg11213, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	//delete[] Msg11213;
	//return;
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data();
		int i = 0;
		Write16(p, 0); //ResultID
		Write32(p, Client->Uin); //Uin
		Write16(p, Mapid); //MapID
		Write16(p, HeadId); //StartRank
		Write16(p, tailId); //EndRank
		Write16(p, 5); //TotalCount 

		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{
			std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("select a.* from mapchallengerank a Where mapid =? order by rankid asc Limit 5;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Mapid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();

			Write16(p, res->rowsCount()); //Count
			while (res->next()) {
				//MapRankEntry
				BYTE* pMapRankEntry = p;
				Write16(pMapRankEntry, 0); //len

				Write16(pMapRankEntry, Mapid); //MapID
				Write16(pMapRankEntry, res->getUInt("RankID")); //Rank
				Write32(pMapRankEntry, res->getUInt("Uin")); //Uin
				std::string nickName = SqlStingUtf8(res->getString("NickName").c_str());
				if (!nickName.empty()) {
					WriteString_MC(pMapRankEntry, nickName.c_str());
				}

				Write32(pMapRankEntry, res->getUInt("GuildID")); //GuildID
				std::string GuildName = SqlStingUtf8(res->getString("GuildName").c_str());
				if (!GuildName.empty()) {
					WriteString_MC(pMapRankEntry, GuildName.c_str());
				}

				Write32(pMapRankEntry, res->getUInt("UseTime")); //UseTime
				Write32(pMapRankEntry, res->getUInt("KartID")); //KartID
				Write32(pMapRankEntry, 0); //ShadowFileSize
				WriteHex(pMapRankEntry, res->getString("FileMd5").c_str()); //FileMd5[]
				WriteString16(pMapRankEntry, res->getString("UUid").c_str()); //UUidLen
				WriteString16(pMapRankEntry, res->getString("LastUUid").c_str()); //LastUUidLen
				Write32(pMapRankEntry, 0); //UpdateTime
				Write8(pMapRankEntry, 0); //Status
				Write32(pMapRankEntry, 0); //ReportedNum
				Write32(pMapRankEntry, res->getUInt("KeepCnt")); //KeepCnt
				Write32(pMapRankEntry, res->getUInt("ChallengeCnt")); //ChallengeCnt
				Write8(pMapRankEntry, 0); //EmpMode

				len = pMapRankEntry - p;
				Set16(p, (WORD)len);
				p += len;
				i++;
			}
			Write16(p, res->rowsCount()); //OtherCount
			for (size_t i = 0; i < res->rowsCount(); i++)
			{ //RankState
				BYTE* pRankState = p;
				Write16(pRankState, 3); //len

				Write8(pRankState, 0); //LeapNum

				len = pRankState - p;
				Set16(p, (WORD)len);
				p += len;
			}

			res.reset();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "ResponseC2GGetMapChallengeRank Error" << std::endl;
			//throw;关闭
		}



		len = p - buf.data();
		SendToClient(Client, 11213, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}


}

void ResponseC2GGetRecentChallengeMsg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	Write32(p, Client->Uin); //Uin
	std::vector<ChallengeMap> ChallengeMapsMsg;
	SelectChallengeMap(ChallengeMapsMsg);
	Write8(p, ChallengeMapsMsg.size()); //MsgNum
	for (size_t i = 0; i < ChallengeMapsMsg.size(); i++)
	{ //Msg
		BYTE* pMsg = p;
		Write16(pMsg, 0); //len

		Write8(pMsg, 1); //Type
		Write32(pMsg, ChallengeMapsMsg[i].Uin); //Uin
		Write32(pMsg, ChallengeMapsMsg[i].GuildID); //GuildID
		WriteString_MC(pMsg, ChallengeMapsMsg[i].GuildName.c_str()); //GuildNameLen
		WriteString_MC(pMsg, ChallengeMapsMsg[i].Name.c_str()); //NameLen
		Write16(pMsg, ChallengeMapsMsg[i].MapID); //SrcMapID
		Write16(pMsg, ChallengeMapsMsg[i].RankID); //SrcRank
		Write32(pMsg, ChallengeMapsMsg[i].KartID); //KartID
		Write16(pMsg, ChallengeMapsMsg[i].MapID); //MapID
		Write16(pMsg, ChallengeMapsMsg[i].RankID); //Rank
		Write32(pMsg, ChallengeMapsMsg[i].UseTime); //UseTime
		Write32(pMsg, 0); //Leaps
		Write32(pMsg, 0); //DstGuildID
		Write8(pMsg, 0); //DstGuildNameLen

		len = pMsg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 11214, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

void NotifyC2GLoadChallengeFileResult(std::shared_ptr<ClientNode> Client, std::vector<ChallengeMap> ChallengeMaps)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	//00 00 27 2E 01 E5 6B 55 00 00 10 4D DF 57 38 CF EC 3F 99 10 0C 6A A3 DE 52 27 0D 00 00 00 00 00
	Write32(p, Client->Uin); //Uin
	Write32(p, (DWORD)time(nullptr)); //Time
	Write16(p, 0); //ResultID
	Write8Hex(p, ChallengeMaps[0].FileMd5.c_str());//FileMd5Len
	Write8(p, 0); //Cmd
	Write16(p, 0); //GameType
	Write16(p, 0); //PointID

	len = p - buf;
	SendToClient(Client, 11220, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyG2CChallengeResult(std::shared_ptr<ClientNode> Client, UINT Result, int FinTime)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write8(p, Result); //Result
	Write8(p, 0); //ReasonLen
	Write8(p, 1); //HasResult
	if (Client->ChallengeMaps.size() != 1) return;
	Client->ChallengeMaps[0].UseTime = FinTime;

	for (size_t i = 0; i < 1; i++)
	{ //Result
		BYTE* pResult = p;
		Write16(pResult, 0); //len

		Write8(pResult, 0); //PreUpdateRankLimit
		Write16(pResult, 0); //LeapCnt
		Write16(pResult, 0); //QQHelpCardCost
		Write32(pResult, FinTime); //FinTime
		Write32(pResult, 0); //ChallengeLimitTime
		{ //Award
			BYTE* pAward = pResult;
			Write16(pAward, 0); //len

			Write32(pAward, 1000); //SuperMoney
			Write32(pAward, 20); //Coupons
			Write32(pAward, 3); //GuildScore
			Write32(pAward, 0); //ItemID
			Write16(pAward, 0); //AwardStrLen
			Write32(pAward, 0); //TotalSuperMoney
			Write32(pAward, 0); //TotalCoupons

			len = pAward - pResult;
			Set16(pResult, (WORD)len);
			pResult += len;
		}
		Write16(pResult, 0); //CanNotUseqqhelpcardDaysBeforeFreezeDate

		len = pResult - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 11224, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

#include "Map.h"
void NotifyC2GLoadChallengeFileResult(std::shared_ptr<ClientNode> Client, UINT MapID)
{
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);

	Client->GameID = 2;
	Client->MapID = Client->ChallengeMaps.size() == 1 ? Client->ChallengeMaps[0].MapID : MapID;

	Map* pMap = GetMap(Client->MapID);
	if (pMap)
	{
		Client->TotalRound = pMap->Round;
	}
	Client->IsReady = TRUE;
	Client->IsFinish = FALSE;
	Client->IsRefitItem = false;
	Client->FinishTime = 0;
	Client->Round = 0;
	Client->PassedCheckPoint = 0;
	Client->MsgSequence = -1;
	Client->TaskID = 0xFFF1;
	//NotifyG2CStartChallenge(Client);
	NotifyGameBeginCS(Client);
	Sleep(3000);
	NotifyEncryptedMsg(Client);
	Sleep(1000);
	NotifyRaceBegin(Client);
}

int isEmpty(char* str) {
	// 检查指针是否为 NULL
	if (str == NULL) {
		return 1; // 空指针，返回 1 表示是空字符
	}
	// 检查字符串是否为空
	if (str[0] == '\0') {
		return 1; // 字符串为空，返回 1 表示是空字符
	}
	return 0; // 不是空字符，返回 0
}

void RequestC2GChallenge(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT MapID = Read16(p);
	USHORT Rank = Read16(p);
	UCHAR FileMd5Len = Read8(p);
	std::string FileMd5 = ReadString(p, FileMd5Len);
	
	ResponseC2GChallenge(Client, MapID, Rank, isEmpty(FileMd5.data()));//打榜
}


void ResponseC2GChallengeTemp(std::shared_ptr<ClientNode> Client, USHORT MapID, USHORT Rank, char FileMd5)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write32(p, Client->Uin); //Uin
	Write16(p, MapID); //MapID
	Write16(p, Rank); //Rank
	Write16(p, 0); //LeapCnt
	Write8(p, 1); //QQHelpCardMeet
	Write32(p, 0); //KubiCost
	Write16(p, 0); //QQHelpCardCost
	Write8(p, 1); //HasRankEntry
	for (size_t i = 0; i < 1; i++)
	{ //RankEntry
		BYTE* pRankEntry = p;
		Write16(pRankEntry, 0); //len

		Write16(pRankEntry, MapID); //MapID
		Write16(pRankEntry, Rank); //Rank
		WriteHex(pRankEntry, "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
		//Write16(pRankEntry, 0); //Uin
		//Write8(pRankEntry, 0); //NameLen
		//Write32(pRankEntry, 0); //GuildID
		//Write8(pRankEntry, 0); //GuildNameLen
		//Write32(pRankEntry, 0); //UseTime
		//Write32(pRankEntry, 0); //KartID
		//Write32(pRankEntry, 0); //ShadowFileSize
		//Write8(pRankEntry, 0); //FileMd5[]
		//Write16(pRankEntry, 0); //UUidLen
		//Write16(pRankEntry, 0); //LastUUidLen
		//Write32(pRankEntry, 0); //UpdateTime
		//Write8(pRankEntry, 0); //Status
		//Write32(pRankEntry, 0); //ReportedNum
		//Write32(pRankEntry, 0); //KeepCnt
		//Write32(pRankEntry, 0); //ChallengeCnt
		//Write8(pRankEntry, 0); //EmpMode

		len = pRankEntry - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 1); //HasStatus
	for (size_t i = 0; i < 1; i++)
	{ //QueueStatus
		BYTE* pQueueStatus = p;
		Write16(pQueueStatus, 0); //len

		Write8(pQueueStatus, 1); //FreeRank
		Write16(pQueueStatus, 0); //WaitLoadNum
		Write16(pQueueStatus, 0); //LoadNum
		Write16(pQueueStatus, 0); //WaitChallengeNum
		Write16(pQueueStatus, 0); //ChallengeNum
		Write16(pQueueStatus, 0); //WaitSendNum

		len = pQueueStatus - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //CreditStarLev

	len = p - buf;
	SendToClient(Client, 11217, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}
//打榜
void ResponseC2GChallenge(std::shared_ptr<ClientNode> Client, USHORT MapID, USHORT Rank, char FileMd5) {

	if (Client->GuildID == 0) {
		ResponseStartGame(Client, "请先加入车队！");
		return;
	}
	auto it = std::find(ChallengeForbidKart.begin(), ChallengeForbidKart.end(), Client->KartID);

	if (it != ChallengeForbidKart.end()) {
		ResponseStartGame(Client, "当前车辆不允许打榜！");
		return;
	}



	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	struct tm tm_now;
	localtime_s(&tm_now, &now_time);

	// 获取小时
	int hour = tm_now.tm_hour;

	// 判断是否在时间内
	if (!((hour >= ChangeTimes[0] && hour < ChangeTimes[1]) || (hour >= ChangeTimes[2] && hour < ChangeTimes[3]))) {
		std::string Reason = "打榜开启时间在";
		Reason.append(std::to_string(RankTimes[0]));
		Reason.append("点到");
		Reason.append(std::to_string(RankTimes[1]));
		Reason.append("点,");
		Reason.append(std::to_string(RankTimes[2]));
		Reason.append("点到");
		Reason.append(std::to_string(RankTimes[3]));
		Reason.append("点。");
		ResponseStartGame(Client, Reason.c_str());
		return;
	}

	/*ChallengeMap* mapPtr = GetChallengeMapByMapIdAndRank(MapID, Rank);
	if (mapPtr && mapPtr->Status) {
		ResponseStartGame(Client, "当前榜单正在被打，稍等一会吧！");
		return;
	}
	UpdateChallengeMapStatus(MapID, Rank, 1);*/

	SelectChallengeMap(MapID, Rank, Client->ChallengeMaps);
	UpdateChallengeMap(Client->ChallengeMaps);
	char HasRankEntry = FileMd5 == 1 ? 0 : 1;
	Client->BufferChallengePtr = 0;
	if (Client->ChallengeMaps.size() != 1) {
		ChallengeMap CM;
		CM.MapID = MapID;
		CM.RankID = Rank;
		CM.Uin = Client->Uin;
		CM.GuildID = Client->GuildID;
		CM.KartID = Client->KartID;
		CM.Name = Client->NickName;
		CM.GuildName = Client->GuildName;
		CM.UseTime = 99999999;
		CM.ChallengeCnt = 0;
		CM.KeepCnt = 0;
		CM.Status = 1;
		Client->ChallengeMaps.push_back(CM);
		//FindOrAddChallengeMap(Client, MapID, Rank);
		ResponseC2GChallengeTemp(Client, MapID, Rank, FileMd5);
		NotifyC2GLoadChallengeFileResult(Client, MapID);
		return;
	}

	UpdateChallengeMapCNT(MapID, Rank);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write32(p, Client->Uin); //Uin
	Write16(p, MapID); //MapID
	Write16(p, Rank); //Rank
	Write16(p, 0); //LeapCnt
	Write8(p, 1); //QQHelpCardMeet
	Write32(p, 0); //KubiCost
	Write16(p, 0); //QQHelpCardCost
	Write8(p, HasRankEntry); //HasRankEntry
	for (size_t i = 0; i < HasRankEntry; i++)
	{ //RankEntry
		BYTE* pRankEntry = p;
		Write16(pRankEntry, 0); //len

		Write16(pRankEntry, MapID); //MapID
		Write16(pRankEntry, Client->ChallengeMaps[0].MapID); //MapID
		Write16(pRankEntry, Client->ChallengeMaps[0].RankID); //Rank
		Write16(pRankEntry, Client->ChallengeMaps[0].Uin); //Uin

		std::string nickName = SqlStingUtf8(Client->ChallengeMaps[0].Name.c_str());
		if (!nickName.empty()) {
			WriteString_MC(pRankEntry, nickName.c_str());
		}

		Write32(pRankEntry, Client->ChallengeMaps[0].GuildID); //GuildID
		std::string GuildName = SqlStingUtf8(Client->ChallengeMaps[0].GuildName.c_str());
		if (!GuildName.empty()) {
			WriteString_MC(pRankEntry, GuildName.c_str());
		}

		Write32(pRankEntry, Client->ChallengeMaps[0].UseTime); //UseTime
		Write32(pRankEntry, Client->ChallengeMaps[0].KartID); //KartID
		Write32(pRankEntry, 0); //ShadowFileSize
		WriteHex(pRankEntry, Client->ChallengeMaps[0].FileMd5.c_str()); //FileMd5[]
		WriteString16(pRankEntry, Client->ChallengeMaps[0].UUid.c_str()); //UUidLen
		WriteString16(pRankEntry, Client->ChallengeMaps[0].LastUUid.c_str()); //LastUUidLen
		Write32(pRankEntry, 0); //UpdateTime
		Write8(pRankEntry, 0); //Status
		Write32(pRankEntry, 0); //ReportedNum
		Write32(pRankEntry, Client->ChallengeMaps[0].KeepCnt); //KeepCnt
		Write32(pRankEntry, Client->ChallengeMaps[0].ChallengeCnt); //ChallengeCnt
		Write8(pRankEntry, 0); //EmpMode

		len = pRankEntry - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 1); //HasStatus
	for (size_t i = 0; i < 1; i++)
	{ //QueueStatus
		BYTE* pQueueStatus = p;
		Write16(pQueueStatus, 0); //len

		Write8(pQueueStatus, 1); //FreeRank
		Write16(pQueueStatus, 0); //WaitLoadNum
		Write16(pQueueStatus, 0); //LoadNum
		Write16(pQueueStatus, 0); //WaitChallengeNum
		Write16(pQueueStatus, 0); //ChallengeNum
		Write16(pQueueStatus, 0); //WaitSendNum

		len = pQueueStatus - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //CreditStarLev

	len = p - buf.data();
	SendToClient(Client, 11217, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	if (FileMd5) {
		NotifyG2CLoadChallengeFile(Client, Client->ChallengeMaps);
	}
	else {
		NotifyC2GLoadChallengeFileResult(Client, MapID);
	}
	//NotifyC2GLoadChallengeFileResult(Client);
}

//未下载 打榜需要下载
void NotifyG2CLoadChallengeFile(std::shared_ptr<ClientNode> Client, std::vector<ChallengeMap> ChallengeMaps)
{
	if (ChallengeMaps.size() != 1)return;
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //LeapCnt
	Write8(p, 1); //QQHelpCardMeet
	Write32(p, 0); //KubiCost
	Write16(p, 0); //QQHelpCardCost
	{ //RankEntry
		BYTE* pRankEntry = p;
		Write16(pRankEntry, 0); //len


		Write16(pRankEntry, ChallengeMaps[0].MapID); //MapID
		Write16(pRankEntry, ChallengeMaps[0].RankID); //Rank
		Write32(pRankEntry, ChallengeMaps[0].Uin); //Uin

		std::string nickName = ChallengeMaps[0].Name;
		if (!nickName.empty()) {
			WriteString_MC(pRankEntry, nickName.c_str());
		}

		Write32(pRankEntry, ChallengeMaps[0].GuildID); //GuildID
		std::string GuildName = ChallengeMaps[0].GuildName;
		if (!GuildName.empty()) {
			WriteString_MC(pRankEntry, GuildName.c_str());
			
		}

		Write32(pRankEntry, ChallengeMaps[0].UseTime); //UseTime
		Write32(pRankEntry, ChallengeMaps[0].KartID); //KartID
		Write32(pRankEntry, 0); //ShadowFileSize
		WriteHex(pRankEntry, ChallengeMaps[0].FileMd5.c_str()); //FileMd5[]
		WriteString16(pRankEntry, ChallengeMaps[0].UUid.c_str()); //UUidLen
		WriteString16(pRankEntry, ChallengeMaps[0].LastUUid.c_str()); //LastUUidLen
		//Write8(pRankEntry, 0); //FileMd5[]
		//Write16(pRankEntry, 0); //UUidLen
		//Write16(pRankEntry, 0); //LastUUidLen
		Write32(pRankEntry, 0); //UpdateTime
		Write8(pRankEntry, 0); //Status
		Write32(pRankEntry, 0); //ReportedNum
		Write32(pRankEntry, 0); //KeepCnt
		Write32(pRankEntry, 0); //ChallengeCnt
		Write8(pRankEntry, 0); //EmpMode

		len = pRankEntry - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //QueueStatus
		BYTE* pQueueStatus = p;
		Write16(pQueueStatus, 0); //len

		Write8(pQueueStatus, 0); //FreeRank
		Write16(pQueueStatus, 0); //WaitLoadNum
		Write16(pQueueStatus, 1); //LoadNum
		Write16(pQueueStatus, 0); //WaitChallengeNum
		Write16(pQueueStatus, 0); //ChallengeNum
		Write16(pQueueStatus, 0); //WaitSendNum

		len = pQueueStatus - p;
		Set16(p, (WORD)len);
		p += len;
	}
	std::string Url = ChallengeUrl + ChallengeMaps[0].UUid;
	WriteString16(p, Url.c_str()); //UrlLen


	len = p - buf;
	SendToClient(Client, 11219, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyC2GStartChallengeResult(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, 0); //Uin
	Write32(p, 0); //Time
	Write8(p, 0); //Ok
	Write8(p, 0); //FileMd5Len

	len = p - buf;
	SendToClient(Client, 11222, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyG2CStartChallenge(std::shared_ptr<ClientNode> Client)
{
	if (Client->ChallengeMaps.size() != 1)return;
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //LeapCnt
	Write32(p, 0); //KubiCost
	Write8(p, 0); //QQHelpCardMeet
	Write16(p, 0); //QQHelpCardCost
	{ //RankEntry
		BYTE* pRankEntry = p;
		Write16(pRankEntry, 0); //len

		Write16(pRankEntry, Client->ChallengeMaps[0].MapID); //MapID
		Write16(pRankEntry, Client->ChallengeMaps[0].RankID); //Rank
		Write32(pRankEntry, Client->ChallengeMaps[0].Uin); //Uin

		std::string nickName = SqlStingUtf8(Client->ChallengeMaps[0].Name);
		if (!nickName.empty()) {
			WriteString_MC(pRankEntry, nickName.c_str());
		}

		Write32(pRankEntry, Client->ChallengeMaps[0].GuildID); //GuildID
		std::string GuildName = SqlStingUtf8(Client->ChallengeMaps[0].GuildName);
		if (!GuildName.empty()) {
			WriteString_MC(pRankEntry, GuildName.c_str());
		}

		Write32(pRankEntry, Client->ChallengeMaps[0].UseTime); //UseTime
		Write32(pRankEntry, Client->ChallengeMaps[0].KartID); //KartID
		Write32(pRankEntry, 0); //ShadowFileSize
		WriteHex(pRankEntry, Client->ChallengeMaps[0].FileMd5.c_str()); //FileMd5[]
		WriteString16(pRankEntry, Client->ChallengeMaps[0].UUid.c_str()); //UUidLen
		WriteString16(pRankEntry, Client->ChallengeMaps[0].LastUUid.c_str()); //LastUUidLen
		Write32(pRankEntry, 0); //UpdateTime
		Write8(pRankEntry, 0); //Status
		Write32(pRankEntry, 0); //ReportedNum
		Write32(pRankEntry, Client->ChallengeMaps[0].KeepCnt); //KeepCnt
		Write32(pRankEntry, Client->ChallengeMaps[0].ChallengeCnt); //ChallengeCnt
		Write8(pRankEntry, 0); //EmpMode

		len = pRankEntry - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //QueueStatus
		BYTE* pQueueStatus = p;
		Write16(pQueueStatus, 0); //len

		Write8(pQueueStatus, 0); //FreeRank
		Write16(pQueueStatus, 0); //WaitLoadNum 
		Write16(pQueueStatus, 0); //LoadNum
		Write16(pQueueStatus, 0); //WaitChallengeNum
		Write16(pQueueStatus, 1); //ChallengeNum
		Write16(pQueueStatus, 0); //WaitSendNum

		len = pQueueStatus - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 5); //PreUpdateRankLimit
	Write16(p, 0); //SelfMapID
	Write16(p, 3); //CanNotUseqqhelpcardDaysBeforeFreezeDate

	len = p - buf;
	SendToClient(Client, 11221, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

//挑战结束
void NotifyG2CGuildChallengeMsg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	if (Client->ChallengeMaps.size() != 1)return;
	Write32(p, Client->Uin); //Uin
	{ //Msg
		BYTE* pMsg = p;
		Write16(pMsg, 0); //len

		Write8(pMsg, 0); //Type
		Write32(pMsg, Client->Uin); //Uin
		Write32(pMsg, Client->GuildID); //GuildID
		WriteString_MC(pMsg, Client->GuildName);//GuildNameLen
		WriteString_MC(pMsg, Client->NickName); //NameLen
		Write16(pMsg, 0); //SrcMapID
		Write16(pMsg, 0); //SrcRank
		Write32(pMsg, 0); //KartID
		Write16(pMsg, 0); //MapID
		Write16(pMsg, 0); //Rank
		Write32(pMsg, 0); //UseTime
		Write32(pMsg, 0); //Leaps
		Write32(pMsg, Client->ChallengeMaps[0].GuildID); //DstGuildID
		WriteString_MC(pMsg, Client->ChallengeMaps[0].GuildName.c_str()); //DstGuildNameLen

		len = pMsg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 11215, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


//上传打榜
void RequestC2FUpLoad(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR Cmd = Read8(p);
	USHORT UuidLen = Read16(p);
	std::string Uid = ReadString(p, UuidLen);
	USHORT AuthInfoLen = Read16(p);
	p += AuthInfoLen;
	UINT TotalLen = Read32(p);
	UINT CurOffSet = Read32(p);
	USHORT DataLen = Read16(p);
	/*char[] FileMd5[] = Read8(p);
	USHORT EncrptedLen = Read16(p);
	USHORT EncrptedSegLen = Read16(p);*/
	ChallengeMap* CM = GetChallengeMapByUUid(Uid);
	/*std::cout << "RequestC2FUpLoad 0" << std::endl;
	std::cout << "Uid:"<< Uid << std::endl;*/
	if (CM) {
		try
		{
			//if (PreventSDP[0] > 0) {
			//	char* Key = GetKey(Client->Uin);
			//	if (Key)
			//	{
			//		BYTE KeyT[16];
			//		memcpy(KeyT, Key, 16);
			//		std::string ClientKeyHex = bytes_to_hex(Client->Key, 16);
			//		std::string KeyHex = bytes_to_hex(KeyT, 16);
			//		if (ClientKeyHex != KeyHex) {
			//			auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
			//			ServerLogger->info("[打榜发包Key不匹配嫌疑:][读取包UIN:{:d}][Client的UIN:{:d}][IP:{:s}][MsgID:{:d}][GetKey:{:s}][ClientKey:{:s}]", Uin, Client->Uin, Client->Ip, 100, KeyHex, ClientKeyHex);
			//			ServerLogger->flush();
			//			Client->Server->Disconnect(Client->ConnID);
			//			return;
			//		}
			//	}
			//	else
			//	{
			//		auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
			//		ServerLogger->info("[打榜发包未找到Key嫌疑:][读取包UIN:{:d}][Client的UIN:{:d}][IP:{:s}][MsgID:{:d}]", Uin, Client->Uin, Client->Ip, 100);
			//		ServerLogger->flush();

			//		Client->Server->Disconnect(Client->ConnID);
			//		return;
			//	}

			//}

			// 检查 BufferChallengePtr 是否已分配内存
			if (Client->BufferChallengePtr) {
				// 计算新大小
				size_t newSize = Client->BufferChallengeOffset + DataLen;

				// 重新分配内存以包含新数据
				void* newData = realloc((void*)Client->BufferChallengePtr, newSize);
				if (!newData) {
					// 内存重新分配失败，处理错误
					return;
				}
				// 更新指针
				Client->BufferChallengePtr = (ULONG_PTR)newData;
				//std::cout << "RequestC2FUpLoad 2" << std::endl;
				// 拷贝新数据到缓冲区的末尾
				memcpy((void*)((ULONG_PTR)Client->BufferChallengePtr + Client->BufferChallengeOffset), p, DataLen);
			}
			else {
				// 如果尚未分配内存，分配新的内存块
				void* pData = safe_malloc(DataLen);
				if (!pData) {
					// 内存分配失败，处理错误
					return;
				}
				// 更新指针
				Client->BufferChallengePtr = (ULONG_PTR)pData;
				// 拷贝新数据
				memcpy((void*)Client->BufferChallengePtr, p, DataLen);
				//std::cout << "RequestC2FUpLoad 1" << std::endl;
			}
			Client->BufferChallengeOffset += DataLen;


			if (Client->BufferChallengeOffset >= TotalLen) {
				//std::cout << "RequestC2FUpLoad 3" << std::endl;
				std::string FileName = ChallengeFilePath + CM->UUid;
				std::ofstream outFile(FileName, std::ios::binary); // 不加后缀
				if (outFile.is_open()) {
					outFile.write(reinterpret_cast<const char*>(Client->BufferChallengePtr), TotalLen);
					outFile.close();
				}
				CM->FileMd5 = getFileMD5(FileName);

				safe_free((void*)Client->BufferChallengePtr);
				Client->BufferChallengePtr = 0;
				Client->BufferChallengeOffset = 0;
				ResponseC2FUpLoad(Client, 1, 0, 0);
				NotifyG2CUpdateChallengeRankResult(Client, 1);
				//UpdateChallengeMapStatus(CM->MapID, CM->RankID, 0);

				UpdateChallengeMapMD5(CM->MapID, CM->RankID, getFileMD5(FileName));
				InSertChallengeMap(Client, CM, FileName);

				if (!ChallengeFileOn) {
					std::filesystem::remove(FileName);
				}
				return;
			}

			ResponseC2FUpLoad(Client, Cmd, Client->BufferChallengeOffset, TotalLen);
		}
		catch (...)
		{
			std::cerr << "RequestC2FUpLoad Error" << std::endl;
		}
	}


}

void ResponseC2FUpLoad(std::shared_ptr<ClientNode> Client, UCHAR Cmd, UINT DataLen, UINT TotalLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write8(p, Cmd); //Cmd
	Write32(p, DataLen); //AckLen
	Write32(p, TotalLen); //TotalLen

	len = p - buf;
	SendToClient(Client, 11200, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseC2GUpdateChallengeRank(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID

	len = p - buf;
	SendToClient(Client, 11225, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	NotifyG2CUpLoadChallengeFile(Client);
}

std::string generateRandomString(size_t length) {
	const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string randomString;
	std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);
	std::random_device rd;
	std::mt19937 generator(rd());

	for (size_t i = 0; i < length; ++i) {
		randomString += charset[dist(generator)];
	}
	return randomString;
}

std::string getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::ostringstream oss;
	struct tm tm_now;
	localtime_s(&tm_now, &in_time_t);
	oss << std::put_time(&tm_now, "%Y%m%d%H%M%S"); // 格式化时间
	return oss.str();
}

std::string ConvertTimeToString(long long UserTime) {
	long long totalSeconds = UserTime / 1000; // 将毫秒转换为秒
	long long milliseconds = UserTime % 1000; // 计算剩余毫秒
	int minutes = totalSeconds / 60;           // 计算分钟
	int seconds = totalSeconds % 60;           // 计算剩余秒数

	// 使用 std::string 创建结果字符串
	return std::to_string(minutes) + "分 " + std::to_string(seconds) + "秒 " + std::to_string(milliseconds) + "毫秒";
}

//通知上传
void NotifyG2CUpLoadChallengeFile(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	std::string timeStr = getCurrentTime();
	std::string randomStr = generateRandomString(23 - timeStr.size()); // 减去时间部分的长度
	std::string AuthInfoStr = generateRandomString(22); // 减去时间部分的长度
	std::string result = timeStr + randomStr;
	if (Client->ChallengeMaps.size() != 1) return;
	if (Client->ChallengeMaps[0].UseTime > 180000)
	{
		ResponseStartGame(Client, "跑个三分钟，你打鸡毛啊");
		return;
	}

	//判断是否有其他人提交
	int UserTime = SelectChallengeMapTime(Client);
	if (UserTime < Client->ChallengeMaps[0].UseTime && UserTime > 0) {
		std::string timeString = ConvertTimeToString(UserTime);
		timeString = "别人跑" + timeString + ",你被捷足先登了";
		ResponseStartGame(Client, timeString.c_str());
		return;
	}

	Client->ChallengeMaps[0].UUid = result;
	UpdateChallengeMapUUID(Client); //更新总记录上传到服务器

	WriteString16(p, result.c_str()); //UuidLen
	WriteString16(p, AuthInfoStr.c_str()); //AuthInfoLen
	{ //PhyInfo
		BYTE* pPhyInfo = p;
		Write16(pPhyInfo, 0); //len
		in_addr addr;
		InetPton(AF_INET, IP.c_str(), &addr);
		Write32(pPhyInfo, addr.S_un.S_addr); //ExtenalIP
		Write8(pPhyInfo, 1); //ExtTcpPortNum
		Write16(pPhyInfo, TcpPort); //ExternalTcpPort[]
		Write8(pPhyInfo, 0); //ExtUdpPortNum
		//Write16(pPhyInfo, 0); //ExternalUdpPort[]

		len = pPhyInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //Cmd

	len = p - buf;
	SendToClient(Client, 11227, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyG2CUpdateChallengeRankResult(std::shared_ptr<ClientNode> Client, int Result)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write8(p, Result); //Result
	WriteString_MC(p, "你已经获得了该车队车队先锋的称号，代表着车队的荣誉，/C0372/C00小时/C030/C00分钟内不能退队!"); //ReasonLen
	Write8(p, 1); //HasResult
	for (size_t i = 0; i < 1; i++)
	{ //Result
		BYTE* pResult = p;
		Write16(pResult, 0); //len

		{ //Award
			BYTE* pAward = pResult;
			Write16(pAward, 0); //len

			Write32(pAward, 0); //SuperMoney
			Write32(pAward, 0); //Coupons
			Write32(pAward, 0); //GuildScore
			Write32(pAward, 0); //ItemID
			Write16(pAward, 0); //AwardStrLen
			Write32(pAward, 0); //TotalSuperMoney
			Write32(pAward, 0); //TotalCoupons

			len = pAward - pResult;
			Set16(pResult, (WORD)len);
			pResult += len;
		}

		len = pResult - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 11228, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestReportCosOpResult(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT ResultID = Read16(p);
	UCHAR Cmd = Read8(p);
	USHORT UuidLen = Read16(p);
	USHORT AuthInfoLen = Read16(p);
	UINT u64SeqID = Read32(p);

}

void ResponseC2GChallengeCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	Write32(p, Client->Uin); //Uin
	{ //BaseCfg
		BYTE* pBaseCfg = p;
		Write16(pBaseCfg, 0); //len

		Write16(pBaseCfg, 126); //SeasonID
		Write32(pBaseCfg, 0x0134DA1E); //OpenDate
		Write32(pBaseCfg, 0x0134DA86); //CloseDate
		Write32(pBaseCfg, 0x00015F90); //OpenTime
		Write32(pBaseCfg, 0x00035B60); //CloseTime
		Write32(pBaseCfg, 0x0134DA85); //FreezeDate

		len = pBaseCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0x0B); //SvrAwardVer


	typedef struct ChallengeAwardCfg
	{
		UINT SuperMoney;
		UINT Coupons;
		UINT GuildScore;
		USHORT GetBoxPro;
		UINT ItemID;
		USHORT AwardStrLen;

	}*LPChallengeAwardCfg;


	typedef struct AwardNumCfg
	{
		USHORT HighRank;
		USHORT LowRank;
		ChallengeAwardCfg challengeAwardCfg[2];
		std::string MemTitleLen;
		USHORT ExpAdd;
		USHORT WageAdd;
		std::string GuildTitleLen;
	}*LPAwardNum;

	const static AwardNumCfg AwardNumCfgs[] =
	{
		{1,1,{{200,0,15,3000,26360,0},{0,0,0,10000,26360,0}},"车神",50,20,"统治赛场" },
		{2,10,{{100,0,12,3000,26359,0},{0,0,0,10000,26359,0}},"传奇霸主",20,20,"叱咤赛场" },
		{11,100,{{100,0,10,2000,26359,0},{0,0,0,10000,26359,0}},"黄金霸主",20,15,"叱咤赛场" },
		{101,200,{{100,0,8,1000,26359,0},{0,0,0,10000,26359,0}},"霸主",20,10,"叱咤赛场" },
		{201,1000,{{60,0,6,3000,26358,0},{0,0,0,10000,26358,0}},"传奇英豪",15,10,"纵横赛场" },
		{1001,1500,{{50,0,6,2000,26358,0},{0,0,0,10000,26358,0}},"黄金英豪",12,10,"纵横赛场" },
		{1501,2000,{{40,0,6,1000,26358,0},{0,0,0,10000,26358,0}},"英豪",10,10,"纵横赛场" },
		{2001,2500,{{40,0,5,3000,26357,0},{0,0,0,10000,26357,0}},"黄金骑士",10,8,"驰骋赛场" },
		{2501,3000,{{30,0,5,1000,26356,0},{0,0,0,10000,26356,0}},"黄金勇者",8,8,"" },
	};

	constexpr unsigned char AwardNum = sizeof(AwardNumCfg) / sizeof(AwardNumCfgs[0]); //

	Write8(p, AwardNum); //AwardNum
	for (size_t i = 0; i < AwardNum; i++)
	{ //AwardEntry
		BYTE* pAwardEntry = p;
		Write16(pAwardEntry, 0); //len

		Write16(pAwardEntry, AwardNumCfgs[i].HighRank); //HighRank
		Write16(pAwardEntry, AwardNumCfgs[i].LowRank); //LowRank
		{ //ChallengeAwardCfg
			BYTE* pChallengeAwardCfg = pAwardEntry;
			Write16(pChallengeAwardCfg, 0); //len

			Write32(pChallengeAwardCfg, AwardNumCfgs[i].challengeAwardCfg[0].SuperMoney); //SuperMoney
			Write32(pChallengeAwardCfg, AwardNumCfgs[i].challengeAwardCfg[0].Coupons); //Coupons
			Write32(pChallengeAwardCfg, AwardNumCfgs[i].challengeAwardCfg[0].GuildScore); //GuildScore
			Write16(pChallengeAwardCfg, AwardNumCfgs[i].challengeAwardCfg[0].GetBoxPro); //GetBoxPro
			Write32(pChallengeAwardCfg, AwardNumCfgs[i].challengeAwardCfg[0].ItemID); //ItemID
			Write16(pChallengeAwardCfg, AwardNumCfgs[i].challengeAwardCfg[0].AwardStrLen); //AwardStrLen

			len = pChallengeAwardCfg - pAwardEntry;
			Set16(pAwardEntry, (WORD)len);
			pAwardEntry += len;
		}
		{ //UpdateRankAwardCfg
			BYTE* pUpdateRankAwardCfg = pAwardEntry;
			Write16(pUpdateRankAwardCfg, 0); //len

			Write32(pUpdateRankAwardCfg, AwardNumCfgs[i].challengeAwardCfg[1].SuperMoney); //SuperMoney
			Write32(pUpdateRankAwardCfg, AwardNumCfgs[i].challengeAwardCfg[1].Coupons); //Coupons
			Write32(pUpdateRankAwardCfg, AwardNumCfgs[i].challengeAwardCfg[1].GuildScore); //GuildScore
			Write16(pUpdateRankAwardCfg, AwardNumCfgs[i].challengeAwardCfg[1].GetBoxPro); //GetBoxPro
			Write32(pUpdateRankAwardCfg, AwardNumCfgs[i].challengeAwardCfg[1].ItemID); //ItemID
			Write16(pUpdateRankAwardCfg, AwardNumCfgs[i].challengeAwardCfg[1].AwardStrLen); //AwardStrLen

			len = pUpdateRankAwardCfg - pAwardEntry;
			Set16(pAwardEntry, (WORD)len);
			pAwardEntry += len;
		}
		WriteString_MC(pAwardEntry, AwardNumCfgs[i].MemTitleLen.c_str());//MemTitleLen

		Write16(pAwardEntry, AwardNumCfgs[i].ExpAdd); //ExpAdd
		Write16(pAwardEntry, AwardNumCfgs[i].WageAdd); //WageAdd
		WriteString_MC(pAwardEntry, AwardNumCfgs[i].GuildTitleLen.c_str()); //GuildTitleLen

		len = pAwardEntry - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0x0B); //SvrMapVer
	Write16(p, ChallengeMaps.size()); //MapNum
	for (size_t i = 0; i < ChallengeMaps.size(); i++)
	{ //MapEntry
		BYTE* pMapEntry = p;
		Write16(pMapEntry, 0); //len

		Write16(pMapEntry, ChallengeMaps[i].MapID); //MapID
		Write8(pMapEntry, ChallengeMaps[i].IsOpen); //Open
		Write8(pMapEntry, ChallengeMaps[i].IsNew); //New

		len = pMapEntry - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0x0b); //SvrNormalMsgVer

	//typedef struct NormalNumfg
	//{
	//	std::string pNormalLen;
	//}*LPNormalNum;
	//const static NormalNumfg NormalNumfgs[] =
	//{
	//	/*"第126届车神争霸将于20240926正式开启，20241029开始结算，20241030正式结束!",
	//	"挑战待收复的榜位记录无需越级，您可以直接挑战上一次榜上的成绩!",
	//	"欢迎大家对妨碍竞赛公平的不良行为进行监督和举报，您可以在观看模式下举报您认为可疑的对象!GC当您发现有的榜位的记录成绩没有您的成绩棒，还等什么？立刻发起挑战吧!",
	//	"合理的使用推荐赛道，会让您快速找到合适的对手，祝您早日上榜，成就辉煌!",
	//	"当您发现有的榜位的记录成绩没有您的成绩棒，还等什么？立刻发起挑战吧!",
	//	"车神争霸还有练习模式和观看模式，任何人都可以查看每个排名上的影子录像，是您学习技术的好帮手哦!",
	//	"带领车队获得车神争霸排名的英雄，将获得车队先锋的头衔，在车队休闲区树立雕像来表彰哦!",*/
	//	"你觉得你很叼吗，那你就来干我，出来面对我！",
	//	"不要打榜，不然你会败的很惨！",
	//	"我的记录是你一辈子跑不到的知道吗",
	//	"我其实喜欢影子挑战，打榜只是哥的爱好！",

	//};

	//constexpr unsigned char NormalNum = sizeof(NormalNumfgs) / sizeof(NormalNumfgs[0]); //

	Write8(p, ChallengeCfgMsgs.size()); //NormalNum
	for (size_t i = 0; i < ChallengeCfgMsgs.size(); i++)
	{ //Normal
		BYTE* pNormal = p;
		Write16(pNormal, 0); //len

		//Write16(pNormal, 0); //Len
		WriteString16(pNormal, ChallengeCfgMsgs[i].c_str()); //Len

		len = pNormal - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0x0b); //SvrTemporaryMsgVer
	Write8(p, 0); //TemporaryNum
	for (size_t i = 0; i < 0; i++)
	{ //Temporary
		BYTE* pTemporary = p;
		Write16(pTemporary, 0); //len

		Write16(pTemporary, 0); //Len

		len = pTemporary - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 5); //TotalRank

	len = p - buf.data();
	SendToClient(Client, 11233, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void Request24617(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT Number = Read16(p);
	UINT lunshu = Read8(p);
	UINT TTrenshu = Read8(p);
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data();
		//BYTE* p = buf;
		size_t len;
		Write32(p, Client->Uin);
		Write8(p, 0);
		Write32(p, 0);

		Write32(p, 0x0037EC9A);//口令

		len = p - buf.data();
		SendToClient(Client, 24617, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}


void ResponseLuckMatchShowInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, 0); //Uin
	Write16(p, 0); //ResultID
	WriteString32(p, "欢迎来到全新幸运大奖赛，本赛季时间从2024年10月1日到2024年10月31日，工作日19:00-次日2:00、周末12:00-次日2:00准时开放。"); //SeasonBroadCastInfoLen
	Write32(p, 0); //CurrentAwardLuckGoldCoinNum
	Write32(p, 0x00000BB8); //DailyAwardLuckGoldCoinLimit
	Write8(p, 1); //CanReceDailyRankAward
	Write32(p, 0); //LastDayRankValue
	Write32(p, 0); //RankAwardItemNums
	Write32(p, 0); //NextReceRankAwardTime
	Write8(p, 1); //SeasonStat
	Write32(p, 0); //CurrentInLuckMatchGameNums
	Write32(p, 0); //CurrentLuckMatchWinnerNums

	len = p - buf;
	SendToClient(Client, 24609, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseLuckMatchBetAwardInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0x2D9E); //ResultID
	Write32(p, 0); //TotoalBetAwardItemNum
	Write32(p, 0x00000BB8); //CanReciveLuckyItemNum

	len = p - buf;
	SendToClient(Client, 24610, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



void ResponseParticipateLuckyMatch(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //CurPlayerNum
	Write8(p, 0); //NeedPlayerNum
	Write8(p, 0); //NewPlayerNumPerMinute
	Write8(p, 0); //NeedMapNum
	Write32(p, 0); //MapID[]
	Write32(p, 0); //MatchTimeOutTime
	Write32(p, 0); //OpenDate
	Write32(p, 0); //CloseDate
	Write16(p, 0); //OpenTimeInfoNums
	for (size_t i = 0; i < 0; i++)
	{ //LuckMatchOpentTimeInfo
		BYTE* pLuckMatchOpentTimeInfo = p;
		Write16(pLuckMatchOpentTimeInfo, 0); //len

		Write32(pLuckMatchOpentTimeInfo, 0); //OpenTime
		Write32(pLuckMatchOpentTimeInfo, 0); //CloseTime

		len = pLuckMatchOpentTimeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 400, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}



void ResponseCancelParticipateLuckyMatch(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID

	len = p - buf;
	SendToClient(Client, 401, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}



void RequestOpenBox(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	if (propflag == 0)
	{
		ResponseStartGame(Client, "抽奖暂时关闭!");
		return;
	}
	std::lock_guard<std::shared_mutex> lock(Client->mtx);
	int n = 1;
	BYTE* p = Body;
	WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int KeyID = 0;
	int Num = 0;
	int TreasureBoxID = Read32(p);
	{ // Key
		BYTE* pKey = p;
		len = Read16(pKey);

		int KeyType = Read32(pKey);
		for (size_t i = 0; i < n; i++)
		{ // KeyEntry
			BYTE* pKeyEntry = pKey;
			len = Read16(pKeyEntry);

			KeyID = Read32(pKeyEntry);
			Num = Read32(pKeyEntry);

			pKey += len;
		}
		UINT IsNotConsume = Read8(pKey);
		// Read8(pKey);

		p += len;
	}
	UINT SpecialIdx = Read8(p);
	USHORT MutiOpenNum = Read16(p);
	USHORT OpenBoxSrcType = Read16(p);
	UCHAR SelectAwardNum = Read8(p);
	int ItemIDNum = 1;
	for (size_t i = 0; i < n; i++)
	{ // AwardIndexInfo
		BYTE* pAwardIndexInfo = p;
		len = Read16(pAwardIndexInfo);

		UCHAR AwardIdx = Read8(pAwardIndexInfo);
		ItemIDNum = Read32(pAwardIndexInfo);
		if (ItemIDNum == 0)
		{
			ItemIDNum = 1;
		}
		p += len;
	}

	//Body += 8;
	//int TreasureBoxID = Read32(Body);//这里普通宝箱这样读取是可以的 但是其他宝箱不是
	//Body += 12;
	//int ItemIDNum = Read8(Body);//多开次数 打开次数 实际上是4位 int 你这里只是读取一位
	//if (ItemIDNum == 0) {
	//	ItemIDNum++;//星宝瓶等道具是0
	//}


	if (!isDBtHasItem(Client, TreasureBoxID, ItemIDNum))
	{
		ResponseStartGame(Client, "盒子必须留一个哟宝贝!");
		return;
	}
	if (TreasureBoxID == 62787)//魅力蝴蝶结加魅力
	{
		RequestAddCharm(Client, TreasureBoxID, ItemIDNum);
		return;
	}
	if (TreasureBoxID == 84900)//加点卷
	{
		RequestAddSuperMoney(Client, TreasureBoxID, ItemIDNum);
		return;
	}
	if (!Client->NewKartTip && (NewKart == 1 || NewKart == 2)) {
		if (NewKart == 1) {
			ResponseStartGame(Client, "当前抽奖获取新车子改装宝石方式是自动获取\n          无宝石车辆较多会有所卡顿\n/C01进入改装或者镶嵌宝石界面，所有新车辆即可获得改装和宝石孔!");
		}
		else if (NewKart == 2) {
			ResponseStartGame(Client, "当前抽奖获取新车子改装宝石方式是刷新频道\n          无宝石车辆较多会有所卡顿\n/C01刷新频道后，所有新车辆即可获得改装和宝石孔!");
		}
		Client->NewKartTip = true;
		//return;
	}

	std::vector<ItemInfo> ItemsAdd(ItemIDNum);
	std::vector<ItemInfo> Items;
	bool result = false;
	if (Uinew) {
		result = GetRandItemCJ(Client, TreasureBoxID, ItemIDNum, ItemsAdd, Items);
	}
	else {
		result = GetRandItem2(Client, TreasureBoxID, ItemIDNum, ItemsAdd, Items);
	}

	if (!result) {
		ResponseStartGame(Client, "宝箱里面没有物品请配置宝箱!");
	}
	else
	{
		virtualsend3(Client, { Items }, ItemIDNum, TreasureBoxID, ItemsAdd);
	}
}

void ResponseGetGameBaseRecordList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	int count;
	Write16(p, 0);
	Write32(p, Client->Uin);
	Write16(p, 1);

	BYTE* pInfo = p;
	Write16(pInfo, 0);//len

	{
		Write16(pInfo, 1);

		BYTE* pMapNum = pInfo;
		Write16(pInfo, 0);

		try
		{
			DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM (SELECT a.mapid, b.Ranking, a.recordid, a.gamemode, a.datetime FROM BattleRecord a JOIN battledetails b ON a.recordid = b.Recordid WHERE b.Uin = ? ORDER BY a.datetime DESC LIMIT 50) AS subquery ORDER BY datetime asc;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			count = res->rowsCount();
			// 处理结果
			while (res->next()) {
				BYTE* pMapInfo = pInfo;
				Write16(pMapInfo, 0); //len

				Write32(pMapInfo, res->getUInt("mapid"));
				Write16(pMapInfo, res->getInt("Ranking"));
				Write32(pMapInfo, res->getInt("datetime"));

				Write32(pMapInfo, res->getUInt("recordid"));
				Write32(pMapInfo, res->getInt("gamemode"));

				len = pMapInfo - pInfo;
				Set16(pInfo, (WORD)len);
				pInfo += len;
			}
			res.reset();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "RequestBattleRecord Error" << std::endl;
		}

		Set16(pMapNum, (WORD)count);
	}

	len = pInfo - p;
	Set16(p, (WORD)len);
	p += len;

	len = p - buf.data();
	SendToClient(Client, 1702, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetGameDetailRecordList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {

	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	Read16(p);
	UINT recordid = Read32(p);

	ResponseGetGameDetailRecordList(Client, recordid);
}


void ResponseGetGameDetailRecordList(std::shared_ptr<ClientNode> Client, UINT Recordid) {

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);
	Write32(p, Client->Uin);

	Write16(p, 1);
	Write16(p, 0);
	Write16(p, 0);

	BYTE* pInfo = p;
	Write16(pInfo, 0);//len

	try
	{
		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT b.nickname,a.* FROM battledetails a,baseinfo b WHERE a.Uin = b.uin and a.recordid = ? ORDER BY CASE WHEN ranking = -1 THEN 99 ELSE ranking END,ranking asc;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Recordid);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		Write16(pInfo, res->rowsCount());//人数
		// 处理结果
		while (res->next()) {
			BYTE* pBaseInfo = pInfo;
			Write16(pBaseInfo, 0); //len

			Write32(pBaseInfo, 0);
			//Write32(pBaseInfo, (DWORD)time(nullptr));

			std::string NickName = UTF2GB(res->getString("nickname").c_str());
			
			memset(pBaseInfo, 0, 17);
			if (!NickName.empty())
			{
				memcpy((char*)pBaseInfo, NickName.c_str(), NickName.length());
			}
			pBaseInfo += 17;

			Write32(pBaseInfo, res->getInt("Kartid"));//赛车ID
			Write32(pBaseInfo, res->getInt("Time"));//时间
			Write32(pBaseInfo, res->getInt("Speed"));//速度
			Write32(pBaseInfo, 0);//？
			Write32(pBaseInfo, res->getInt("collision"));//碰撞
			Write32(pBaseInfo, res->getInt("nitrogen"));//氮气

			Write32(pBaseInfo, 0);
			Write32(pBaseInfo, 1);
			Write32(pBaseInfo, 0);

			Write32(pBaseInfo, res->getInt("drift"));//漂移
			Write32(pBaseInfo, 0);//小喷
			Write32(pBaseInfo, 0);//双喷
			Write32(pBaseInfo, 0);//空喷
			Write32(pBaseInfo, 0);//落地喷

			memset(pBaseInfo, 0, 49);
			pBaseInfo += 49;

			Write8(pBaseInfo, res->getInt("RankGradeLevel"));//段位？

			memset(pBaseInfo, 0, 32);
			pBaseInfo += 32;

			len = pBaseInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "RequestBattleRecordOne Error" << std::endl;
	}

	len = pInfo - p;
	Set16(p, (WORD)len);
	p += len;

	len = p - buf.data();
	SendToClient(Client, 1703, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}



void RequestReportCommonEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//PrintfBody(Client, 24028, Body, BodyLen);
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR EventType = Read8(p);
	int EventID = Read32(p);
	UCHAR ParaNum = Read8(p);

	if (Uin != Client->Uin) {
		return;
	}

	// int[] Para[100] = Read32(p);
	if (EventID == 18 && NewKart == 1) {
		//std::cout << EventID << std::endl;
		PlayerDB_AddStoneCar(Client);
	}

}

void RequestGetSuperN2O(BYTE* Body, std::shared_ptr<ClientNode> Client)
{
	UINT temp = 0;
	Read32(Body);//Uin
	Read32(Body);//Time
	Read32(Body);// DriftFlag
	temp = Read32(Body);//当前漂移集气总量0x03e8==1000时获得一个氮气
	Client->SuperJetKartFuelInc += temp;//总量
	Client->CurrentAccelerateFuel += temp;//临时量
	//if (Client->CurrentAccelerateFuel >= 1000 && Client->KartSkinID == 127616)


	if (Client->nitrogen == 1) {
		NotifyAddPropBySkillStone(Client, 1, 1, 1);
	}

	if (Client->CurrentAccelerateFuel >= 1000 && Client->IsRefitItem)
	{
		ResponseGetSuperN2O(Client);
		return;
	}
	else if (temp != 1000)//完美集气获得一个超级氮气
	{
		return;
	}
	if (Client->SuperN2O != 0) {
		ResponseGetSuperN2O2(Client);
	}
	return;
}
UINT GetRandMap(short MapPoolID)//0 全随 5边境 8赛事 16排位
{

	int allmap = 0;
	int randindex = 0;
	UINT mapid = 0;
	//yml加载地图池

	std::shared_ptr<RANDOMMAPPOOL> MapPool = GetRandomMapPool(MapPoolID);
	if (!MapPool)
	{
		return 114;
	}
	allmap = MapPool->MapID.size();
	if (allmap <= 0)
	{
		return 114;
	}
	//printf("PoolID:%d\n", MapPoolID);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, allmap - 1);

	randindex = dist(gen);
	mapid = MapPool->MapID[randindex];
	return mapid;
}

UINT GetRandMapHome(short MapPoolID)//0 全随 5边境 8赛事 16排位
{

	int allmap = 0;
	int randindex = 0;
	UINT mapid = 0;
	//yml加载地图池

	std::shared_ptr<RANDOMMAPPOOL> MapPool = GetRandomMapPool(MapPoolID);
	if (!MapPool)
	{
		return 114;
	}
	allmap = MapPool->MapID.size();
	if (allmap <= 0)
	{
		return 114;
	}
	randindex = rand() % (allmap);
	mapid = MapPool->MapID[randindex];
	return mapid;
}

std::string GetItemName(UINT Itemid) {

	std::string newname = "Empty";

	auto it = Items.find(Itemid);

	if (it != Items.end()) {
		// Found the item
		std::unique_ptr<Item>& foundItem = it->second;
		if (foundItem) {
			newname = foundItem->Name;
		}
	}
	return newname;
}

bool isLock(UINT Itemid) {
	bool ret = false;
	auto it = Items.find(Itemid);

	if (it != Items.end()) {
		// Found the item
		std::unique_ptr<Item>& foundItem = it->second;
		if (foundItem) {
			if (foundItem->lock > 0) {
				ret = true;
			}
		}
	}
	return ret;
}

bool compareItemType(const ItemInfo& a, const ItemInfo& b) {
	// ItemType 为 1 的放在最前面
	if (a.ItemType == 1 && b.ItemType != 1) {
		return true;
	}
	else if (a.ItemType != 1 && b.ItemType == 1) {
		return false;
	}

	// ItemType 为 2 的放在第二位
	if (a.ItemType == 2 && b.ItemType != 2) {
		return true;
	}
	else if (a.ItemType != 2 && b.ItemType == 2) {
		return false;
	}

	// ItemType 为 0 的放在最后面
	if (a.ItemType == 0 && b.ItemType != 0) {
		return false;
	}
	else if (a.ItemType != 0 && b.ItemType == 0) {
		return true;
	}

	// 如果 ItemType 相等，则保持原顺序
	return false;
}

bool GetRandItemNew(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return false;//如果内存没有
		}
		TreasureAward TreasureAward;
		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return false;//无物品
		}
		items.resize(count);

		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();


		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return NULL;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			TreasureAward = TreasureBox->TreasureAwards[listindex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAward.Seed;
			ItemType = TreasureAward.ItemType;
			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}
			while (SeedTemp < Seed)
			{

				listindex++;
				if (listindex == alllist)
				{
					listindex = 0;
					Seed = (UINT)(rand() % 10000000) + 1;
				}
				TreasureAward = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAward.Seed;
				ItemType = TreasureAward.ItemType;
			}
			//printf("Seed:%d SeedTemp:%d\n", Seed, SeedTemp);
		GetItem:
			allitem = TreasureAward.AwardItems.size();
			if (allitem == 0)
			{
				continue;//无物品
			}
			if (listindex == 0)
			{
				LuckyValue = 0;
				for (int i = 0; i < allitem; i++)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM mechskinaward Where ItemID=? and Uin=? And Available = -1;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, TreasureAward.AwardItems[i].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					prep_stmt.reset();
					if (res->rowsCount() > 0) {
						if (GetItemType(TreasureAward.AwardItems[i].ItemID) == EAIT_STONE) {
							hasAward = 0;
							randindex = (rand() % allitem);
							break;
						}
						else {
							hasAward = 1;
						}
					}
					else {
						hasAward = 0;
						randindex = i;
						break;
					}
					res.reset();;
				}
				if (hasAward)
				{
					listindex++;
					goto GetRandAward;
				}
			}
			else
			{
				randindex = (UINT)(rand() % allitem);
			}
			AwardItem = TreasureAward.AwardItems[randindex];
			items[j].ItemID = AwardItem.ItemID;
			items[j].ItemNum = AwardItem.ItemNum;
			items[j].ItemType = ItemType;
			items[j].AvailPeriod = AwardItem.AvailPeriod;
			items[j].ObtainTime = AwardItem.ObtainTime;

			//if (ItemsTre[j].ItemType == 1) {
			//	std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(ItemsTre[j].ItemID);
			//	const char* charPtr = announcement.c_str();
			//	NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200);  //我是固定小公告
			//}

			/*if (Itemid != 24726) {
				ItemsTre[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, ItemsTre[j].ItemID, ItemsTre[j].ItemNum, ItemsTre[j].AvailPeriod, 0, conn);
			}*/
			listindex = 0;
			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);



		std::sort(items.begin(), items.begin() + count, compareItemType);

		//// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		//// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// 输出执行时间
		//std::cout << "执行时间: " << duration.count() << " 毫秒" << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItemNew Error\n");
	}
	return false;
}




bool GetRandItem(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& items)
{
	try
	{
		
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return false;//如果内存没有
		}
		TreasureAward TreasureAward;
		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return false;//无物品
		}
		items.resize(count);
		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();


		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return NULL;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			TreasureAward = TreasureBox->TreasureAwards[listindex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAward.Seed;
			ItemType = TreasureAward.ItemType;
			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}
			while (SeedTemp < Seed)
			{

				listindex++;
				if (listindex == alllist)
				{
					listindex = 0;
					Seed = (UINT)(rand() % 10000000) + 1;
				}
				TreasureAward = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAward.Seed;
				ItemType = TreasureAward.ItemType;
			}
			//printf("Seed:%d SeedTemp:%d\n", Seed, SeedTemp);
		GetItem:
			allitem = TreasureAward.AwardItems.size();
			if (allitem == 0)
			{
				continue;//无物品
			}
			if (listindex == 0)
			{
				LuckyValue = 0;
				for (int i = 0; i < allitem; i++)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM Item Where ItemID=? and Uin=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, TreasureAward.AwardItems[i].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					prep_stmt.reset();
					if (res->rowsCount() > 0) {
						if (GetItemType(TreasureAward.AwardItems[i].ItemID) == EAIT_STONE) {
							hasAward = 0;
							randindex = (rand() % allitem);
							break;
						}
						else {
							hasAward = 1;
						}
					}
					else {
						hasAward = 0;
						randindex = i;
						break;
					}
					res.reset();;
				}
				if (hasAward)
				{
					listindex++;
					goto GetRandAward;
				}
			}
			else
			{
				randindex = (UINT)(rand() % allitem);
			}
			AwardItem = TreasureAward.AwardItems[randindex];
			items[j].ItemID = AwardItem.ItemID;
			items[j].ItemNum = AwardItem.ItemNum;
			items[j].ItemType = ItemType;
			items[j].AvailPeriod = AwardItem.AvailPeriod;
			items[j].ObtainTime = AwardItem.ObtainTime;

			if (items[j].ItemType == 1) {
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(items[j].ItemID);
				const char* charPtr = announcement.c_str();
				NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200);  //我是固定小公告
			}

			if (Itemid != 24726) {
				items[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, items[j].ItemID, items[j].ItemNum, items[j].AvailPeriod, 0, conn);
			}
			listindex = 0;
			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);



		std::sort(items.begin(), items.begin() + count, compareItemType);

		//// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		//// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// 输出执行时间
		//std::cout << "执行时间: " << duration.count() << " 毫秒" << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem1 Error\n");
	}
	return false;
}



bool GetRandItem24(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		

		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return false;//如果内存没有
		}
		TreasureAward TreasureAward;
		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return false;//无物品
		}
		items.resize(count);

		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();


		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return NULL;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		/*std::unordered_set<int> existingIDs;*/
		bool chongfucar = false;
		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			chongfucar = false;
			TreasureAward = TreasureBox->TreasureAwards[listindex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAward.Seed;
			ItemType = TreasureAward.ItemType;
			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}
			else {
				if (isMinimum == 1 && SeedTemp >= Seed && ItemType == 1) {
					Seed = SeedTemp + 100;
				}
			}
			while (SeedTemp < Seed)
			{

				listindex++;
				if (listindex == alllist)
				{
					listindex = 0;
					Seed = (UINT)(rand() % 10000000) + 1;
				}
				TreasureAward = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAward.Seed;
				ItemType = TreasureAward.ItemType;
			}
			//printf("Seed:%d SeedTemp:%d\n", Seed, SeedTemp);
		GetItem:
			allitem = TreasureAward.AwardItems.size();
			if (allitem == 0)
			{
				continue;//无物品
			}
			if (listindex == 0)
			{
				LuckyValue = 0;
				for (int i = 0; i < allitem; i++)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM Item Where ItemID=? and Uin=? And AvailPeriod <= 0;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, TreasureAward.AwardItems[i].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					prep_stmt.reset();
					if (res->rowsCount() > 0) {
						if (GetItemType(TreasureAward.AwardItems[i].ItemID) == EAIT_STONE) {
							hasAward = 0;
							randindex = (rand() % allitem);
							break;
						}
						else {
							hasAward = 1;
						}
					}
					else {
						hasAward = 0;
						randindex = i;
						break;
					}
					res.reset();;
				}
				if (hasAward)
				{
					listindex++;
					goto GetRandAward;
				}
			}
			else
			{
				randindex = (UINT)(rand() % allitem);
			}
			AwardItem = TreasureAward.AwardItems[randindex];

			if (isLock(AwardItem.ItemID)) {
				AwardItem.ItemID = Itemid;
				AwardItem.ItemNum = 1;
				AwardItem.AvailPeriod = 0;
			}



			items[j].ItemID = AwardItem.ItemID;
			items[j].ItemNum = AwardItem.ItemNum;
			items[j].ItemType = ItemType;
			items[j].AvailPeriod = AwardItem.AvailPeriod;
			items[j].ObtainTime = AwardItem.ObtainTime;

			ItemsAdd[j].ItemID = AwardItem.ItemID;
			ItemsAdd[j].ItemNum = AwardItem.ItemNum;
			ItemsAdd[j].ItemType = ItemType;
			ItemsAdd[j].AvailPeriod = AwardItem.AvailPeriod;
			ItemsAdd[j].ObtainTime = AwardItem.ObtainTime;

			if (items[j].ItemType == 1 && Itemid != 24726) {
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(items[j].ItemID);
				const char* charPtr = announcement.c_str();
				NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200);  //我是固定小公告
			}

			//if (GetItemType(ItemsTre[j].ItemID) == EAIT_CAR) {

			//	if (existingIDs.count(ItemsTre[j].ItemID)) {
			//		chongfucar = true;
			//	}
			//	else {
			//		existingIDs.insert(ItemsTre[j].ItemID);
			//	}
			//}

			if (Itemid != 24726 && !chongfucar) {
				ItemsAdd[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, items[j].ItemID, items[j].ItemNum, items[j].AvailPeriod, 0, conn);
			}
			listindex = 0;
			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);

		std::sort(items.begin(), items.begin() + count, compareItemType);

		//// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		//// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// //输出执行时间
		//std::cout << "执行时间: " << duration.count() << " 毫秒" << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem2 Error\n");
	}

}


bool GetRandItem21(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& Items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return false;//如果内存没有
		}
		TreasureAward TreasureAward;
		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return false;//无物品
		}
		Items.resize(count);

		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();

		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return NULL;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		std::unordered_set<int> existingIDs;
		bool chongfucar = false;
		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			chongfucar = false;
			TreasureAward = TreasureBox->TreasureAwards[listindex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAward.Seed;
			ItemType = TreasureAward.ItemType;
			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}
			else {
				if (isMinimum == 1 && SeedTemp >= Seed && ItemType == 1) {
					Seed = SeedTemp + 100;
				}
			}
			while (SeedTemp < Seed)
			{

				listindex++;
				if (listindex == alllist)
				{
					listindex = 0;
					Seed = (UINT)(rand() % 10000000) + 1;
				}
				TreasureAward = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAward.Seed;
				ItemType = TreasureAward.ItemType;
			}
			//printf("Seed:%d SeedTemp:%d\n", Seed, SeedTemp);
		GetItem:
			allitem = TreasureAward.AwardItems.size();
			if (allitem == 0)
			{
				continue;//无物品
			}
			if (listindex == 0)
			{
				LuckyValue = 0;
				for (int i = 0; i < allitem; i++)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM Item Where ItemID=? and Uin=? And AvailPeriod <= 0;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, TreasureAward.AwardItems[i].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					prep_stmt.reset();
					if (res->rowsCount() > 0) {
						if (GetItemType(TreasureAward.AwardItems[i].ItemID) == EAIT_STONE) {
							hasAward = 0;
							randindex = (rand() % allitem);
							break;
						}
						else {
							hasAward = 1;
						}
					}
					else {
						hasAward = 0;
						randindex = i;
						break;
					}
					res.reset();;
				}
				if (hasAward)
				{
					listindex++;
					goto GetRandAward;
				}
			}
			else
			{
				randindex = (UINT)(rand() % allitem);
			}
			AwardItem = TreasureAward.AwardItems[randindex];

			if (isLock(AwardItem.ItemID)) {
				AwardItem.ItemID = Itemid;
				AwardItem.ItemNum = 1;
				AwardItem.AvailPeriod = 0;
			}



			Items[j].ItemID = AwardItem.ItemID;
			Items[j].ItemNum = AwardItem.ItemNum;
			Items[j].ItemType = ItemType;
			Items[j].AvailPeriod = AwardItem.AvailPeriod;
			Items[j].ObtainTime = AwardItem.ObtainTime;

			ItemsAdd[j].ItemID = AwardItem.ItemID;
			ItemsAdd[j].ItemNum = AwardItem.ItemNum;
			ItemsAdd[j].ItemType = ItemType;
			ItemsAdd[j].AvailPeriod = AwardItem.AvailPeriod;
			ItemsAdd[j].ObtainTime = AwardItem.ObtainTime;

			if (Items[j].ItemType == 1 && Itemid != 24726) {
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(Items[j].ItemID);
				const char* charPtr = announcement.c_str();
				NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200);  //我是固定小公告
			}

			if (GetItemType(Items[j].ItemID) == EAIT_CAR) {

				if (existingIDs.count(Items[j].ItemID)) {
					chongfucar = true;
				}
				else {
					existingIDs.insert(Items[j].ItemID);
				}
			}

			if (Itemid != 24726 && !chongfucar) {
				ItemsAdd[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, Items[j].ItemID, Items[j].ItemNum, Items[j].AvailPeriod, 0, conn);
			}
			listindex = 0;
			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);

		std::sort(Items.begin(), Items.begin() + count, compareItemType);

		//// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		//// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// //输出执行时间
		//std::cout << "执行时间: " << duration.count() << " 毫秒" << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem2 Error\n");
	}

}
//可重复
bool GetRandItem22(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& Items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return NULL; // 如果内存没有
		}
		TreasureAward TreasureAward;
		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return false; // 无物品
		}
		Items.resize(count);
		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();

		DBConnection myDBConnection(player);
		auto conn = myDBConnection.getConnection();
		if (!conn)
			return false;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next())
			{
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		std::unordered_set<int> existingIDs;
		bool chongfucar = false;

		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			chongfucar = false;

			// 原逻辑中逐步递增 listindex，改为随机选择物品组，同时保持概率分布机制
			listindex = (UINT)(rand() % alllist);
			TreasureAward = TreasureBox->TreasureAwards[listindex];

			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAward.Seed;
			ItemType = TreasureAward.ItemType;

			// 抽奖率机制保留
			if (LuckyValue >= TreasureBox->LuckyValue)
			{
				getItem = true;
				goto GetItem;
			}
			else
			{
				if (isMinimum == 1 && SeedTemp >= Seed && ItemType == 1)
				{
					Seed = SeedTemp + 100;
				}
			}
			while (SeedTemp < Seed)
			{
				// 随机重新选择其他物品组，而不是按顺序递增
				listindex = (UINT)(rand() % alllist);
				TreasureAward = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAward.Seed;
				ItemType = TreasureAward.ItemType;
			}

		GetItem:
			allitem = TreasureAward.AwardItems.size();
			if (allitem == 0)
			{
				continue; // 无物品
			}

			// 当前组内的物品根据概率进行随机选择
			randindex = (UINT)(rand() % allitem); // 依然是随机选择物品
			AwardItem = TreasureAward.AwardItems[randindex];

			if (isLock(AwardItem.ItemID))
			{
				AwardItem.ItemID = Itemid;
				AwardItem.ItemNum = 1;
				AwardItem.AvailPeriod = 0;
			}

			Items[j].ItemID = AwardItem.ItemID;
			Items[j].ItemNum = AwardItem.ItemNum;
			Items[j].ItemType = ItemType;
			Items[j].AvailPeriod = AwardItem.AvailPeriod;
			Items[j].ObtainTime = AwardItem.ObtainTime;

			ItemsAdd[j].ItemID = AwardItem.ItemID;
			ItemsAdd[j].ItemNum = AwardItem.ItemNum;
			ItemsAdd[j].ItemType = ItemType;
			ItemsAdd[j].AvailPeriod = AwardItem.AvailPeriod;
			ItemsAdd[j].ObtainTime = AwardItem.ObtainTime;

			if (Items[j].ItemType == 1 && Itemid != 24726)
			{
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(Items[j].ItemID);
				const char* charPtr = announcement.c_str();
				NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200); // 我是固定小公告
			}

			if (GetItemType(Items[j].ItemID) == EAIT_CAR)
			{
				if (existingIDs.count(Items[j].ItemID))
				{
					chongfucar = true;
				}
				else
				{
					existingIDs.insert(Items[j].ItemID);
				}
			}

			if (Itemid != 24726 && !chongfucar)
			{
				ItemsAdd[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, Items[j].ItemID, Items[j].ItemNum, Items[j].AvailPeriod, 0, conn);
			}

			LuckyValue++;
			if (getItem)
			{
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);

		std::sort(Items.begin(), Items.begin() + count, compareItemType); // 保留对 ItemType 的排序

		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem2 Error\n");
	}
} 


bool GetRandItem12(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& Items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;

		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return false; // 如果宝箱数据不存在
		}

		TreasureAward TreasureAward;
		ItemInfo AwardItem;

		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return false; // 无物品
		}
		Items.resize(count);
		DBConnection myDBConnection(player);
		auto conn = myDBConnection.getConnection();
		if (!conn) return NULL;

		conn->setAutoCommit(false);

		// 获取玩家幸运值
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(
				conn->prepareStatement("SELECT LuckyValue FROM TreasureLuckyInfo WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			while (res->next())
			{
				LuckyValue = res->getUInt("LuckyValue");
			}
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(
				conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin, ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		bool chongfucar = false;

		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			chongfucar = false;

			// 随机从奖池中选择一个奖品
			int randomIndex = rand() % alllist; // 随机奖池索引
			TreasureAward = TreasureBox->TreasureAwards[randomIndex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAward.Seed;
			ItemType = TreasureAward.ItemType;

			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}

			// 检查是否符合概率条件
			while (SeedTemp < Seed)
			{
				randomIndex = rand() % alllist; // 随机重新选择
				TreasureAward = TreasureBox->TreasureAwards[randomIndex];
				SeedTemp = TreasureAward.Seed;
				ItemType = TreasureAward.ItemType;
				Seed = (UINT)(rand() % 10000000) + 1;
			}

		GetItem:
			allitem = TreasureAward.AwardItems.size();
			if (allitem == 0)
			{
				continue; // 当前奖池无物品，跳过
			}

			// 随机选择奖池中的一个物品
			randindex = rand() % allitem;
			AwardItem = TreasureAward.AwardItems[randindex];

			// 检查是否重复获得
			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(
					conn->prepareStatement("SELECT 1 FROM Item WHERE ItemID=? AND Uin=? AND AvailPeriod <= 0;"));
				prep_stmt->setUInt(1, AwardItem.ItemID);
				prep_stmt->setUInt(2, Client->Uin);

				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				if (res->next())
				{
					// 如果已经获得该物品，重新抽取
					goto GetRandAward;
				}
			}

			// 锁定物品逻辑
			if (isLock(AwardItem.ItemID)) {
				AwardItem.ItemID = Itemid;
				AwardItem.ItemNum = 1;
				AwardItem.AvailPeriod = 0;
			}

			Items[j].ItemID = AwardItem.ItemID;
			Items[j].ItemNum = AwardItem.ItemNum;
			Items[j].ItemType = ItemType;
			Items[j].AvailPeriod = AwardItem.AvailPeriod;
			Items[j].ObtainTime = AwardItem.ObtainTime;

			ItemsAdd[j].ItemID = AwardItem.ItemID;
			ItemsAdd[j].ItemNum = AwardItem.ItemNum;
			ItemsAdd[j].ItemType = ItemType;
			ItemsAdd[j].AvailPeriod = AwardItem.AvailPeriod;
			ItemsAdd[j].ObtainTime = AwardItem.ObtainTime;

			// 发送公告
			if (Items[j].ItemType == 1 && Itemid != 24726)
			{
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) +
					" 在道具 " + GetItemName(Itemid) +
					" 中获得极品道具  " + GetItemName(Items[j].ItemID);
				NotifyBugletTalk(SystemClient, const_cast<char*>(announcement.c_str()), 200, 200);
			}

			// 添加到数据库
			ItemsAdd[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, Items[j].ItemID, Items[j].ItemNum, Items[j].AvailPeriod, 0, conn);

			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		// 更新幸运值
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=? WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
		}

		conn->commit();
		conn->setAutoCommit(true);

		std::sort(Items.begin(), Items.begin() + count, compareItemType);

		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem24 Error\n");
		return NULL;
	}
}





// 函数：根据 ItemID 对物品进行分类并打乱
void ShuffleTreasureAwards(TREASUREAWARD& treasureAwards) {
	std::vector<ItemInfo> carItems; // 存放 EAIT_CAR 类型的物品
	std::vector<ItemInfo> otherItems; // 存放其他类型的物品

	// 将物品分类
	for (UINT i = 0; i < treasureAwards.AwardItems.size(); ++i) {
		if (GetItemType(treasureAwards.AwardItems[i].ItemID) == EAIT_CAR) {
			carItems.push_back(treasureAwards.AwardItems[i]);
		}
		else {
			otherItems.push_back(treasureAwards.AwardItems[i]);
		}
	}

	// 打乱两个数组
	auto shuffle = [](std::vector<ItemInfo>& items) {
		for (size_t i = 0; i < items.size(); ++i) {
			size_t j = rand() % items.size(); // 随机选择一个索引
			std::swap(items[i], items[j]); // 交换元素
		}
		};

	// 打乱 carItems 和 otherItems
	shuffle(carItems);
	shuffle(otherItems);

	// 将打乱后的物品合并回原数组
	UINT index = 0;
	for (const auto& item : carItems) {
		treasureAwards.AwardItems[index++] = item;
	}
	for (const auto& item : otherItems) {
		treasureAwards.AwardItems[index++] = item;
	}
}

bool GetRandItem2(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return NULL;//如果内存没有
		}
		TreasureAward TreasureAwards;
		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return NULL;//无物品
		}
		items.resize(count);

		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();


		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return NULL;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
		std::unordered_set<int> existingIDs;
		bool chongfucar = false;
		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			chongfucar = false;
			TreasureAwards = TreasureBox->TreasureAwards[listindex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAwards.Seed;
			ItemType = TreasureAwards.ItemType;
			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}
			else {
				if (isMinimum == 1 && SeedTemp >= Seed && ItemType == 1) {
					Seed = SeedTemp + 100;
				}
			}
			while (SeedTemp < Seed)
			{

				listindex++;
				if (listindex == alllist)
				{
					listindex = 0;
					Seed = (UINT)(rand() % 10000000) + 1;
				}
				TreasureAwards = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAwards.Seed;
				ItemType = TreasureAwards.ItemType;
			}
			//printf("Seed:%d SeedTemp:%d\n", Seed, SeedTemp);
		GetItem:
			allitem = TreasureAwards.AwardItems.size();
			//随机物品
			TreasureAward TreasureAwardRand;
			TreasureAwardRand.Seed = TreasureAwards.Seed;
			TreasureAwardRand.ItemType = TreasureAwards.ItemType;

			// 复制 AwardItems
			TreasureAwardRand.AwardItems = TreasureAwards.AwardItems;



			if (allitem == 0)
			{
				continue;//无物品
			}
			if (listindex == 0)
			{
				LuckyValue = 0;
				ShuffleTreasureAwards(TreasureAwardRand);

				for (int i = 0; i < allitem; i++)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM Item Where ItemID=? and Uin=? And AvailPeriod <= 0;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, TreasureAwardRand.AwardItems[i].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					prep_stmt.reset();
					if (res->rowsCount() > 0) {
						if (GetItemType(TreasureAwardRand.AwardItems[i].ItemID) == EAIT_STONE) {
							hasAward = 0;
							randindex = (rand() % allitem);
							break;
						}
						else {
							hasAward = 1;
						}
					}
					else {
						hasAward = 0;
						randindex = i;
						break;
					}
					res.reset();;
				}
				if (hasAward)
				{
					listindex++;
					goto GetRandAward;
				}
			}
			else
			{
				randindex = (UINT)(rand() % allitem);
			}
			AwardItem = TreasureAwardRand.AwardItems[randindex];

			if (isLock(AwardItem.ItemID)) {
				AwardItem.ItemID = Itemid;
				AwardItem.ItemNum = 1;
				AwardItem.AvailPeriod = 0;
			}



			items[j].ItemID = AwardItem.ItemID;
			items[j].ItemNum = AwardItem.ItemNum;
			items[j].ItemType = ItemType;
			items[j].AvailPeriod = AwardItem.AvailPeriod;
			items[j].ObtainTime = AwardItem.ObtainTime;

			ItemsAdd[j].ItemID = AwardItem.ItemID;
			ItemsAdd[j].ItemNum = AwardItem.ItemNum;
			ItemsAdd[j].ItemType = ItemType;
			ItemsAdd[j].AvailPeriod = AwardItem.AvailPeriod;
			ItemsAdd[j].ObtainTime = AwardItem.ObtainTime;

			if (items[j].ItemType == 1 && Itemid != 24726) {
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(items[j].ItemID);
				const char* charPtr = announcement.c_str();
				NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200);  //我是固定小公告
			}

			if (GetItemType(items[j].ItemID) == EAIT_CAR) {

				if (existingIDs.count(items[j].ItemID)) {
					chongfucar = true;
				}
				else {
					existingIDs.insert(items[j].ItemID);
				}
			}

			if (Itemid != 24726 && !chongfucar) {
				ItemsAdd[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, items[j].ItemID, items[j].ItemNum, items[j].AvailPeriod, 0, conn);
			}
			listindex = 0;
			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);

		std::sort(items.begin(), items.begin() + count, compareItemType);

		//// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		//// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// //输出执行时间
		//std::cout << "执行时间: " << duration.count() << " 毫秒" << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem2 Error\n");
	}
	return false;
}

bool GetRandItemCJ(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& ItemsAdd, std::vector<ItemInfo>& items)
{
	try
	{
		INT LuckyValue = -1;
		int allitem = 0;
		int alllist = 0;
		int randindex = 0;
		int listindex = 0;
		BOOLEAN getItem = false;

		UINT Seed = 0;
		UINT SeedTemp = 0;
		UINT ItemType = 0;
		TREASUREBOX* TreasureBox = GetTreasureBox(Itemid);
		if (!TreasureBox)
		{
			return NULL;//如果内存没有
		}
		TreasureAward TreasureAwards;

		ItemInfo AwardItem;
		alllist = TreasureBox->TreasureAwards.size();
		if (alllist == 0)
		{
			return NULL;//无物品
		}
		items.resize(count);
		// 获取开始时间点
		auto start = std::chrono::high_resolution_clock::now();


		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return NULL;
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LuckyValue from TreasureLuckyInfo where Uin=? and ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				LuckyValue = res->getUInt("LuckyValue");
			}
			res.reset();
		}

		if (LuckyValue == -1)
		{
			LuckyValue = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO TreasureLuckyInfo(Uin,ItemID) VALUES(?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		char hasAward = 0;
		srand((unsigned)time(0) + GetSysTimeMicros());
	/*	std::unordered_set<int> existingIDs;*/
		bool chongfucar = false;
		for (int j = 0; j < count; j++)
		{
		GetRandAward:
			hasAward = 0;
			chongfucar = false;
			TreasureAwards = TreasureBox->TreasureAwards[listindex];
			Seed = (UINT)(rand() % 10000000) + 1;
			SeedTemp = TreasureAwards.Seed;
			ItemType = TreasureAwards.ItemType;
			if (LuckyValue >= TreasureBox->LuckyValue) {
				getItem = true;
				goto GetItem;
			}
			else {
				if (isMinimum == 1 && SeedTemp >= Seed && ItemType == 1) {
					Seed = SeedTemp + 100;
				}
			}
			while (SeedTemp < Seed)
			{

				listindex++;
				if (listindex == alllist)
				{
					listindex = 0;
					Seed = (UINT)(rand() % 10000000) + 1;
				}
				TreasureAwards = TreasureBox->TreasureAwards[listindex];
				SeedTemp = TreasureAwards.Seed;
				ItemType = TreasureAwards.ItemType;
			}
			//printf("Seed:%d SeedTemp:%d\n", Seed, SeedTemp);
		GetItem:
			allitem = TreasureAwards.AwardItems.size();
			TreasureAward TreasureAwardRand;
			TreasureAwardRand.Seed = TreasureAwards.Seed;
			TreasureAwardRand.ItemType = TreasureAwards.ItemType;

			// 复制 AwardItems
			TreasureAwardRand.AwardItems = TreasureAwards.AwardItems;
			
			if (allitem == 0)
			{
				continue;//无物品
			}
			if (listindex == 0)
			{
				LuckyValue = 0;

				ShuffleTreasureAwards(TreasureAwardRand);
				for (int i = allitem - 1; i >= 0; i--)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM Item Where ItemID=? and Uin=? And AvailPeriod <= 0;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, TreasureAwardRand.AwardItems[i].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					prep_stmt.reset();
					if (res->rowsCount() > 0) {
						if (GetItemType(TreasureAwardRand.AwardItems[i].ItemID) == EAIT_STONE) {
							hasAward = 0;
							randindex = (rand() % allitem);
							break;
						}
						else {
							hasAward = 1;
						}
					}
					else {
						if (TreasureAwardRand.AwardItems[i].AvailPeriod > 0) {
							continue;
						}
						if (!(GetItemType(items[randindex].ItemID) == EAIT_CAR && items[randindex].AvailPeriod == -1)) {
							TreasureBox->LuckyValue = 100;
						}
						hasAward = 0;
						randindex = i;
						break;
					}
					res.reset();;
				}
				if (hasAward)
				{
					listindex++;
					goto GetRandAward;
				}
			}
			else
			{
				randindex = (UINT)(rand() % allitem);
			}
			AwardItem = TreasureAwardRand.AwardItems[randindex];

			if (isLock(AwardItem.ItemID)) {
				AwardItem.ItemID = Itemid;
				AwardItem.ItemNum = 1;
				AwardItem.AvailPeriod = 0;
			}



			items[j].ItemID = AwardItem.ItemID;
			items[j].ItemNum = AwardItem.ItemNum;
			items[j].ItemType = ItemType;
			items[j].AvailPeriod = AwardItem.AvailPeriod;
			items[j].ObtainTime = AwardItem.ObtainTime;

			ItemsAdd[j].ItemID = AwardItem.ItemID;
			ItemsAdd[j].ItemNum = AwardItem.ItemNum;
			ItemsAdd[j].ItemType = ItemType;
			ItemsAdd[j].AvailPeriod = AwardItem.AvailPeriod;
			ItemsAdd[j].ObtainTime = AwardItem.ObtainTime;

			if (items[j].ItemType == 1) {
				std::string announcement = "恭喜玩家 " + std::string(Client->NickName) + " 在道具 " + GetItemName(Itemid) + " 中获得极品道具  " + GetItemName(items[j].ItemID);
				const char* charPtr = announcement.c_str();
				NotifyBugletTalk(SystemClient, const_cast<char*>(charPtr), 200, 200);  //我是固定小公告
			}

			if (GetItemType(items[j].ItemID) == EAIT_CAR) {

				int Ava = PlayerDB_GetItemAvailPeriod(Client->Uin, items[j].ItemID);
				if (Ava == -1) {
					items[j].ItemID = 12390;
					items[j].ItemNum = 1;
					items[j].ItemType = 0;
					items[j].AvailPeriod = 0;
					items[j].ObtainTime = 0;

					ItemsAdd[j].ItemID = 12390;
					ItemsAdd[j].ItemNum = 1;
					ItemsAdd[j].ItemType = 0;
					ItemsAdd[j].AvailPeriod = 0;
					ItemsAdd[j].ObtainTime = 0;
				}
				//if (existingIDs.count(ItemsTre[j].ItemID)) {
				//	ItemsTre[j].ItemID = 12390;
				//	ItemsTre[j].ItemNum = 1;
				//	ItemsTre[j].ItemType = 0;
				//	ItemsTre[j].AvailPeriod = 0;
				//	ItemsTre[j].ObtainTime = 0;

				//	ItemsAdd[j].ItemID = 12390;
				//	ItemsAdd[j].ItemNum = 1;
				//	ItemsAdd[j].ItemType = 0;
				//	ItemsAdd[j].AvailPeriod = 0;
				//	ItemsAdd[j].ObtainTime = 0;
				//	chongfucar = true;
				//}
				//else {
				//	existingIDs.insert(ItemsTre[j].ItemID);
				//}
			}

			if (Itemid != 24726 && !chongfucar) {
				if (GetItemType(items[j].ItemID) == EAIT_CAR && items[j].AvailPeriod == -1) {
					TreasureBox->LuckyValue = 200;
				}
				ItemsAdd[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, items[j].ItemID, items[j].ItemNum, items[j].AvailPeriod, 0, conn);
			}
			listindex = 0;
			LuckyValue++;
			if (getItem) {
				LuckyValue = 0;
				getItem = false;
			}
		}

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE TreasureLuckyInfo SET LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, LuckyValue);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, Itemid);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		conn->commit();
		conn->setAutoCommit(true);

		std::sort(items.begin(), items.begin() + count, compareItemType);

		//// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		//// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// //输出执行时间
		//std::cout << "执行时间: " << duration.count() << " 毫秒" << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetRandItem2 Error\n");
	}
	return false;
}

void virtualsend2(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	short ItemNum = 0;

	NotifyUseItemResult2(Client, itemid, count);

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	{ //TreasureBoxToClient
		BYTE* pTreasureBoxToClient = p;
		Write16(pTreasureBoxToClient, 0); //len

		Write32(pTreasureBoxToClient, itemid); //BoxID
		Write32(pTreasureBoxToClient, 0); //BoxType
		{ //Key
			BYTE* pKey = pTreasureBoxToClient;
			Write16(pKey, 0); //len

			Write32(pKey, 0); //KeyType
			for (size_t i = 0; i < count; i++)
			{ //KeyEntry
				BYTE* pKeyEntry = pKey;
				Write16(pKeyEntry, 0); //len

				Write32(pKeyEntry, 0); //KeyID
				Write32(pKeyEntry, 0); //Num

				len = pKeyEntry - pKey;
				Set16(pKey, (WORD)len);
				pKey += len;
			}
			Write8(pKey, 0); //IsNotConsume

			len = pKey - pTreasureBoxToClient;
			Set16(pTreasureBoxToClient, (WORD)len);
			pTreasureBoxToClient += len;
		}
		{ //AwardEntry
			BYTE* pAwardEntry = pTreasureBoxToClient;
			Write16(pAwardEntry, 0); //len

			Write32(pAwardEntry, 0); //Exp
			Write32(pAwardEntry, 0); //Money
			Write32(pAwardEntry, 0); //SuperMoney
			BYTE* pItemNum = pAwardEntry;
			Write16(pAwardEntry, 0); //ItemNum
			for (size_t i = 0; i < count; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardEntry;
				Write16(pItemInfo, 0); //len
				//if (Items[i].ItemID == 0)
				//{
				//	continue;
				//}
				ItemNum++;
				Write32(pItemInfo, Items[i].ItemID); //ItemID
				Write32(pItemInfo, Items[i].ItemNum); //ItemNum
				Write32(pItemInfo, -1); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, Items[i].ObtainTime); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, Items[i].ItemType); //ItemType 0普通 1极品金色 2稀有紫色

				len = pItemInfo - pAwardEntry;
				Set16(pAwardEntry, (WORD)len);
				pAwardEntry += len;

			}
			Set16(pItemNum, ItemNum);
			Write32(pAwardEntry, 0); //Coupons
			Write32(pAwardEntry, 0); //GuildPoint
			Write32(pAwardEntry, 0); //LuckMoney
			Write8(pAwardEntry, 0); //ExtendInfoNum
			Write32(pAwardEntry, 0); //SpeedCoin

			len = pAwardEntry - pTreasureBoxToClient;
			Set16(pTreasureBoxToClient, (WORD)len);
			pTreasureBoxToClient += len;
		}

		len = pTreasureBoxToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ReasonLen
	Write32(p, ItemNum); //Reserve
	Write32(p, 0); //MaxLuckVaue
	Write32(p, 0); //LuckValue
	Write32(p, 0); //TotalLuckValue
	Write32(p, 0); //OpenDate
	Write32(p, 0); //CloseDate
	Write32(p, 0); //OpenTime
	Write32(p, 0); //CloseTime

	len = p - buf.data();
	SendToClient(Client, 168, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void virtualsend(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	short ItemNum = 0;

	//NotifyClientAddItem(Client, count, Items);//通知客户端添加道具
	NotifyClientAddItemBox(Client, count, Items);//通知客户端添加道具
	NotifyUseItemResult2(Client, itemid, count);

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	{ //TreasureBoxToClient
		BYTE* pTreasureBoxToClient = p;
		Write16(pTreasureBoxToClient, 0); //len

		Write32(pTreasureBoxToClient, itemid); //BoxID
		Write32(pTreasureBoxToClient, 0); //BoxType
		{ //Key
			BYTE* pKey = pTreasureBoxToClient;
			Write16(pKey, 0); //len

			Write32(pKey, 0); //KeyType
			for (size_t i = 0; i < count; i++)
			{ //KeyEntry
				BYTE* pKeyEntry = pKey;
				Write16(pKeyEntry, 0); //len

				Write32(pKeyEntry, 0); //KeyID
				Write32(pKeyEntry, 0); //Num

				len = pKeyEntry - pKey;
				Set16(pKey, (WORD)len);
				pKey += len;
			}
			Write8(pKey, 0); //IsNotConsume

			len = pKey - pTreasureBoxToClient;
			Set16(pTreasureBoxToClient, (WORD)len);
			pTreasureBoxToClient += len;
		}
		{ //AwardEntry
			BYTE* pAwardEntry = pTreasureBoxToClient;
			Write16(pAwardEntry, 0); //len

			Write32(pAwardEntry, 0); //Exp
			Write32(pAwardEntry, 0); //Money
			Write32(pAwardEntry, 0); //SuperMoney
			BYTE* pItemNum = pAwardEntry;
			Write16(pAwardEntry, 0); //ItemNum
			for (size_t i = 0; i < count; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardEntry;
				Write16(pItemInfo, 0); //len
				//if (Items[i].ItemID == 0)
				//{
				//	continue;
				//}
				ItemNum++;
				Write32(pItemInfo, Items[i].ItemID); //ItemID
				Write32(pItemInfo, Items[i].ItemNum); //ItemNum
				//Write32(pItemInfo, -1); //AvailPeriod
				Write32(pItemInfo, Items[i].AvailPeriod); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, Items[i].ObtainTime); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, Items[i].ItemType); //ItemType 0普通 1极品金色 2稀有紫色

				len = pItemInfo - pAwardEntry;
				Set16(pAwardEntry, (WORD)len);
				pAwardEntry += len;

			}
			Set16(pItemNum, ItemNum);
			Write32(pAwardEntry, 0); //Coupons
			Write32(pAwardEntry, 0); //GuildPoint
			Write32(pAwardEntry, 0); //LuckMoney
			Write8(pAwardEntry, 0); //ExtendInfoNum
			Write32(pAwardEntry, 0); //SpeedCoin

			len = pAwardEntry - pTreasureBoxToClient;
			Set16(pTreasureBoxToClient, (WORD)len);
			pTreasureBoxToClient += len;
		}

		len = pTreasureBoxToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ReasonLen
	Write32(p, ItemNum); //Reserve
	Write32(p, 0); //MaxLuckVaue
	Write32(p, 0); //LuckValue
	Write32(p, 0); //TotalLuckValue
	Write32(p, 0); //OpenDate
	Write32(p, 0); //CloseDate
	Write32(p, 0); //OpenTime
	Write32(p, 0); //CloseTime

	len = p - buf.data();
	SendToClient(Client, 168, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void virtualsend3(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid, std::vector<ItemInfo>& ItemsAdd)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	short ItemNum = 0;

	NotifyClientAddItemBox(Client, count, ItemsAdd);//通知客户端添加道具
	NotifyUseItemResult2(Client, itemid, count);

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	{ //TreasureBoxToClient
		BYTE* pTreasureBoxToClient = p;
		Write16(pTreasureBoxToClient, 0); //len

		Write32(pTreasureBoxToClient, itemid); //BoxID
		Write32(pTreasureBoxToClient, 0); //BoxType
		{ //Key
			BYTE* pKey = pTreasureBoxToClient;
			Write16(pKey, 0); //len

			Write32(pKey, 0); //KeyType
			for (size_t i = 0; i < count; i++)
			{ //KeyEntry
				BYTE* pKeyEntry = pKey;
				Write16(pKeyEntry, 0); //len

				Write32(pKeyEntry, 0); //KeyID
				Write32(pKeyEntry, 0); //Num

				len = pKeyEntry - pKey;
				Set16(pKey, (WORD)len);
				pKey += len;
			}
			Write8(pKey, 0); //IsNotConsume

			len = pKey - pTreasureBoxToClient;
			Set16(pTreasureBoxToClient, (WORD)len);
			pTreasureBoxToClient += len;
		}
		{ //AwardEntry
			BYTE* pAwardEntry = pTreasureBoxToClient;
			Write16(pAwardEntry, 0); //len

			Write32(pAwardEntry, 0); //Exp
			Write32(pAwardEntry, 0); //Money
			Write32(pAwardEntry, 0); //SuperMoney
			BYTE* pItemNum = pAwardEntry;
			Write16(pAwardEntry, 0); //ItemNum
			for (size_t i = 0; i < count; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardEntry;
				Write16(pItemInfo, 0); //len
				//if (Items[i].ItemID == 0)
				//{
				//	continue;
				//}
				ItemNum++;
				Write32(pItemInfo, Items[i].ItemID); //ItemID
				Write32(pItemInfo, Items[i].ItemNum); //ItemNum
				//Write32(pItemInfo, -1); //AvailPeriod
				Write32(pItemInfo, Items[i].AvailPeriod); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, Items[i].ObtainTime); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, Items[i].ItemType); //ItemType 0普通 1极品金色 2稀有紫色

				len = pItemInfo - pAwardEntry;
				Set16(pAwardEntry, (WORD)len);
				pAwardEntry += len;

			}
			Set16(pItemNum, ItemNum);
			Write32(pAwardEntry, 0); //Coupons
			Write32(pAwardEntry, 0); //GuildPoint
			Write32(pAwardEntry, 0); //LuckMoney
			Write8(pAwardEntry, 0); //ExtendInfoNum
			Write32(pAwardEntry, 0); //SpeedCoin

			len = pAwardEntry - pTreasureBoxToClient;
			Set16(pTreasureBoxToClient, (WORD)len);
			pTreasureBoxToClient += len;
		}

		len = pTreasureBoxToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ReasonLen
	Write32(p, ItemNum); //Reserve
	Write32(p, 0); //MaxLuckVaue
	Write32(p, 0); //LuckValue
	Write32(p, 0); //TotalLuckValue
	Write32(p, 0); //OpenDate
	Write32(p, 0); //CloseDate
	Write32(p, 0); //OpenTime
	Write32(p, 0); //CloseTime

	len = p - buf.data();
	SendToClient(Client, 168, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void PrintfBody(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen)
{

	printf("MsgID:%d  Body 内容大小：%d\n", MsgID, (int)BodyLen);
	fflush(stdout);
	for (int i = 0; i < BodyLen; i++)
	{
		printf("%02X ", *(Body + i));
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);

}
void PrintfBody(BYTE* Body, size_t BodyLen)
{

	printf("Body 内容大小：%d\n", (int)BodyLen);
	fflush(stdout);
	for (int i = 0; i < BodyLen; i++)
	{
		printf("%02X ", *(Body + i));
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);

}


void PrintfBody(std::shared_ptr<ClientNode> Client, USHORT MsgID, BYTE* Body, size_t BodyLen, const char* filename)
{
	// 打开文件以进行写入
	FILE* file = nullptr;
	fopen_s(&file, filename, "a"); // 以追加模式打开文件
	if (file == nullptr) {
		printf("无法打开文件: %s\n", filename);
		return;
	}

	// 写入消息 ID 和内容大小
	fprintf(file, "MsgID:%d  Body 内容大小：%d\n", MsgID, (int)BodyLen);

	// 写入 Body 内容
	for (size_t i = 0; i < BodyLen; i++) {
		fprintf(file, "%02x ", *(Body + i)); // 以两位十六进制格式写入
	}
	fprintf(file, "\n");

	// 关闭文件
	fclose(file);
}

BOOL SendToClient(std::shared_ptr<ClientNode> Client, short MsgID, const BYTE* Data, DWORD Length, short SrcID, char SrcFE, short DstID, short MsgType, char Encrypt)
{

	if (!Client) { return 0; }
	if (Client->ConnID <= 0) return 0;

	CSHEAD* ProtocolHead;
	MSGHEAD* MsgHead;
	void* MsgBody;
	int TotalLength;

	int MsgLen = sizeof(MSGHEAD) + Length;
	MsgHead = (MSGHEAD*)safe_malloc(MsgLen);
	if (!MsgHead)
	{
		return 0;
	}
	MsgHead->MsgID = htons(MsgID);
	MsgHead->MsgType = htons(MsgType);
	MsgHead->MsgSeq = htonl(-1);
	MsgHead->SrcFE = SrcFE;
	MsgHead->DstFE = FE_CLIENT;
	MsgHead->SrcID = htons(SrcID);
	MsgHead->DstID = htons(DstID);
	MsgHead->BodyLen = htons(sizeof(USHORT) + USHORT(Length));
	MsgBody = (void*)(MsgHead + 1);
	memcpy(MsgBody, Data, Length);

	if (Encrypt)
	{
		int EncryptLen = oi_symmetry_encrypt2_len(MsgLen);
		TotalLength = sizeof(CSHEAD) + EncryptLen;
		if (!(ProtocolHead = (CSHEAD*)safe_malloc(TotalLength)))
		{
			return 0;
		}
		//加密数据包
		oi_symmetry_encrypt2((BYTE*)MsgHead, MsgLen, Client->Key, (BYTE*)(ProtocolHead + 1), &EncryptLen);
		TotalLength = sizeof(CSHEAD) + EncryptLen;
	}
	else
	{
		TotalLength = sizeof(CSHEAD) + MsgLen;
		if (!(ProtocolHead = (CSHEAD*)safe_malloc(TotalLength)))
		{
			return 0;
		}
		memcpy((MSGHEAD*)(ProtocolHead + 1), MsgHead, MsgLen);
	}
	safe_free(MsgHead);

	ProtocolHead->TotalLength = htonl(TotalLength);
	ProtocolHead->Ver = htons(116);
	ProtocolHead->DialogID = htons(SrcID);
	ProtocolHead->Seq = htonl(-1);
	ProtocolHead->Uin = htonl(Client->Uin);
	ProtocolHead->BodyFlag = Encrypt;
	ProtocolHead->OptLength = 0;

	Client->Server->Send(Client->ConnID, (BYTE*)ProtocolHead, TotalLength);


#ifdef DEBUG
	printf("[ConnID:%d]SendMsg:%d\n", Client->ConnID, MsgID);
#endif
	safe_free(ProtocolHead);


	return 0;
}

//bool processSeq(std::shared_ptr<ClientNode> Client,int seq) {
//	std::lock_guard<std::mutex> lock(Client->mtx); // 锁定当前Client实例
//	if (Client->receivedSeqs.find(seq) == Client->receivedSeqs.end()) {
//		Client->receivedSeqs.insert(seq);
//		return true;
//	}
//	return false;
//}


void printMsgHead(MSGHEAD* msg)
{
	std::cout << "MSGHEAD Structure:" << std::endl;
	std::cout << "MsgID: " << ntohs(msg->MsgID) << std::endl;
	//std::cout << "MsgType: " << ntohs(msg->MsgType) << std::endl;
	std::cout << "MsgSeq: " << htonl(msg->MsgSeq) << std::endl;
	/*std::cout << "SrcFE: " << msg->SrcFE << std::endl;
	std::cout << "DstFE: " << msg->DstFE << std::endl;*/
	//std::cout << "SrcID: " << ntohs(msg->SrcID) << std::endl;
	//std::cout << "DstID: " << ntohs(msg->DstID) << std::endl;
	//std::cout << "BodyLen: " << ntohs(msg->BodyLen) << std::endl;
}


bool isSeqValid(std::shared_ptr<ClientNode> Client, CSHEAD* ProtocolHead, int MsgID, size_t Length)
{
	u_long currentSeq = htonl(ProtocolHead->Seq); // Convert from network to host byte order

	if (Msgs[MsgID]) {
		Client->HeadSeq = currentSeq; // Update lastSeq to the new valid sequence number
	}
	else {
		if (currentSeq != Client->HeadSeq + 1 || currentSeq == -1) {
			auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
			ServerLogger->info("[发包嫌疑:][UIN:{:d}][IP:{:s}][MsgID:{:d}]", Client->Uin, Client->Ip, MsgID);
			ServerLogger->flush();
			//std::cout << "发包嫌疑:"<< MsgID << std::endl;
			return false;
		}
		Client->HeadSeq = currentSeq; // Update lastSeq to the new valid sequence number
	}
	//std::cout <<"currentSeq:"<< currentSeq << "   Client->HeadSeq:" << Client->HeadSeq << std::endl;
	return true;
}

void OnFullReceive(std::shared_ptr<ClientNode> Client, BYTE* Data, size_t Length)
{

	if (Length < sizeof(CSHEAD))
	{
		Client->IsLogin = FALSE;
		Client->Server->Disconnect(Client->ConnID);
		Client->deleteflag = 1;
		return;
	}
	CSHEAD* ProtocolHead = (CSHEAD*)Data;
	BYTE* Body = NULL;
	DWORD HeadLen = sizeof(CSHEAD) + ProtocolHead->OptLength;



	if (ProtocolHead->Ver == 23115)
	{



	}

	if (Length < HeadLen)
	{
		Client->IsLogin = FALSE;
		Client->Server->Disconnect(Client->ConnID);
		Client->deleteflag = 1;
		return;
	}
	UINT Uin = ntohl(ProtocolHead->Uin);
	if (Uin) {
		Client->ConnIDUin = Uin;
		if (!Client->IsLogin)
		{
			if (ProtocolHead->OptLength)
			{
				Body = (BYTE*)(ProtocolHead + 1);
				char Password[64] = { 0 };
				if (ProtocolHead->OptLength < 64)
				{

					memcpy(Password, Body, ProtocolHead->OptLength);
					if (UserDB_CmpPassword(Uin, Password))
					{
						Client->IsLogin = TRUE;
					}
					if (Keymode == 1) {
						std::string Key = GetKey(Uin);
						if (Key.empty()) {
							Key = UserDB_GetLoginKey(Uin);
							Keys[Uin] = Key;
						}
						if (!Key.empty()) {
							memcpy(Client->Key, Key.c_str(), 16);
						}
					}

				}
			}
		}
	}

	DWORD BodyLen = Length - HeadLen;
	if (BodyLen >= sizeof(MSGHEAD))
	{
		MSGHEAD* MsgHead = (MSGHEAD*)((DWORD64)Data + HeadLen);
		/*std::cout<< "ProtocolHead:" << htonl(ProtocolHead->Seq) << std::endl;
		std::cout<< "MsgHead:" << htonl(MsgHead->MsgSeq) << std::endl;*/
		if (ProtocolHead->BodyFlag)
		{
			Body = (BYTE*)safe_malloc(BodyLen);
			if (!Body)
			{
				return;
			}
			if (oi_symmetry_decrypt2((BYTE*)MsgHead, BodyLen, Client->Key, Body, (int*)&BodyLen) == false)
			{
				if (isLog == 1) {
					auto ServerLogger = spdlog::get("SpeedServer_logger");
					ServerLogger->info("[OnFullReceive][解密失败][Uin:{:d}][Content:{:d}][Ver:{:d}][BodyFlag:{:d}][IP:{:s}]",
						Uin, Client->ConnID, ProtocolHead->Ver, ProtocolHead->BodyFlag, Client->Ip);
					ServerLogger->flush();
				}
				safe_free(Body);
				return;
			}
		}
		else
		{
			Body = (BYTE*)safe_malloc(BodyLen);
			if (!Body)
			{
				return;
			}
			memcpy(Body, MsgHead, BodyLen);
		}

		BYTE* tmpBody = Body;
		MsgHead = (MSGHEAD*)tmpBody;
		tmpBody = (BYTE*)tmpBody + sizeof(MSGHEAD);
		BodyLen = BodyLen - sizeof(MSGHEAD);

		//printMsgHead(MsgHead);

		if (ProtocolHead->OptLength)
		{
			Client->ServerID = ntohs(MsgHead->DstID);
		}

		const unsigned short MsgID = ntohs(MsgHead->MsgID);
		const unsigned short BodyLen = ntohs(MsgHead->BodyLen);

		if (isLog == 1 && MsgID != 153) {
			auto ServerLogger = spdlog::get("SpeedServer_logger");
			ServerLogger->info("[OnRecvFromClient][ConnID:{:d}][Uin:{:d}][MsgID:{:d}][BodyLen:{:d}][Ver:{:d}][BodyFlag:{:d}][IP:{:s}]",
				Client->ConnID, Uin, MsgID, BodyLen, ProtocolHead->Ver, ProtocolHead->BodyFlag, Client->Ip);
			ServerLogger->flush();
		}


		if (PreventSDP[0] > 0) {
			if (!isSeqValid(Client, ProtocolHead, MsgID, Length)) {
				safe_free(Body);
				return;
			}
		}

		OnRecvFromClient(Client, MsgID, tmpBody, BodyLen);

		safe_free(Body);
	}
	else
	{
		//printf("[ConnID:%d]不支持请求检查更新\n", Client->ConnID);
		Client->IsLogin = FALSE;
		Client->Server->Disconnect(Client->ConnID);
		Client->deleteflag = 1;
	}

}

void RequestFishLogin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseFishLogin(Client);
}
void ResponseFishLogin(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //Result
	Write32(p, Client->Uin); //Uin

	{ //BaseFishInfo
		BYTE* pBaseFishInfo = p;
		Write16(pBaseFishInfo, 0); //len

		Write32(pBaseFishInfo, 0); //Experience
		Write32(pBaseFishInfo, 0); //Money
		Write8(pBaseFishInfo, 0); //FishStockNum
		Write8(pBaseFishInfo, 0); //RubbishCount

		len = pBaseFishInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //TodayInfo
		BYTE* pTodayInfo = p;
		Write16(pTodayInfo, 0); //len

		Write8(pTodayInfo, 0); //RestStealCount
		Write8(pTodayInfo, 0); //RestVisitStrangerCount
		Write8(pTodayInfo, 0); //RestClearRubbishCount

		len = pTodayInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //FishStockNum
	/*
m_astFishStockInfo[].m_uchFishStockID
m_astFishStockInfo[].m_uchMaturityCount
m_astFishStockInfo[].m_uchTotalGrowStage
m_astFishStockInfo[].m_uchCurrentGrowStage
m_astFishStockInfo[].m_uchFishNum
m_astFishStockInfo[].m_uchStealedCount
m_astFishStockInfo[].m_uchFeedInfo
m_astFishStockInfo[].m_iFishItemID
m_astFishStockInfo[].m_uiFishStartTime
m_astFishStockInfo[].m_uiMaturityTime
m_astFishStockInfo[].m_uiCurGrowStageEndTime
	*/

	Write8(p, 0); //ReasonLen


	len = p - buf.data();
	SendToClient(Client, 10501, buf.data(), len, 0, 32, Client->ConnID, Response);
}


void RequestGetPlayerEventCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseGetPlayerEventCfg(Client);
}
void ResponseGetPlayerEventCfg(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write32(p, 0); //GetFriendsEventInterval
	Write32(p, 0); //GetOtherEventInterval
	Write32(p, 0); //FilterTimeLimit

	len = p - buf.data();
	SendToClient(Client, 11451, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetPlayerEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	BYTE TargetType = Read8(Body);
	BYTE TimeType = Read8(Body);

	//m_ushTarGetNum
	//m_astTarGetInfo[].m_uiUin
	//m_astTarGetInfo[].m_uiEventID

	ResponseGetPlayerEvent(Client, TargetType, TimeType);
}
void ResponseGetPlayerEvent(std::shared_ptr<ClientNode> Client, BYTE TargetType, BYTE TimeType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write8(p, TargetType); //
	Write8(p, TimeType); //
	Write16(p, 0); //Num
	/*
m_astEvent[].m_uiUin
m_astEvent[].m_ucType
m_astEvent[].m_uiEventID
m_astEvent[].m_ucIntNum
m_astEvent[].m_auiPara[]
m_astEvent[].m_ucShortNum
m_astEvent[].m_aushPara[]
m_astEvent[].m_uStrNum
m_astEvent[].m_astPara[].m_szStr[]
m_astEvent[].m_uiTime
	*/

	len = p - buf.data();
	SendToClient(Client, 11452, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestChangeUdpState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	BYTE State = Read8(Body);
	//printf("Uin:%d ChangeUdpState:%d\n", Uin,State);
	//TransferModel = State;

	//fflush(stdout);

}

void RequestTransferUDPOK(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	short DstDlg = Read16(Body);
	UINT DstUin = Read32(Body);
	short InfoLength = Read16(Body);
	ResponseTransferUDPOK(Client);
}
void ResponseTransferUDPOK(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	len = p - buf.data();
	SendToClient(Client, 499, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
/*
999
--------------------CNotifyUDPOKPara--------------------
m_unUin
m_unTime
m_nSrcDlg
m_unSrcUin
m_nInfoLength
--------------------------------------------------------
*/





void RequestFizzInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseFizzInfo(Client);
}
void ResponseFizzInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	{ //FizzBaseInfo
		BYTE* pFizzBaseInfo = p;
		Write16(pFizzBaseInfo, 0); //len

		Write16(pFizzBaseInfo, 0); //FizzPointDaily
		Write16(pFizzBaseInfo, 0); //FizzPointWeekly
		Write32(pFizzBaseInfo, 0); //FizzPointTotal
		Write8(pFizzBaseInfo, 0); //FizzEmblem

		len = pFizzBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //FizzTaskNum
	/*
	for (size_t i = 0; i < 0; i++)
	{ //FizzTaskInfo
		BYTE* pFizzTaskInfo = p;
		Write16(pFizzTaskInfo, 0); //len

		Write8(pFizzTaskInfo, 0); //TaskID
		Write8(pFizzTaskInfo, 0); //Status
		Write32(pFizzTaskInfo, 0); //TotalProgress
		Write32(pFizzTaskInfo, 0); //CurrentProgress
		Write16(pFizzTaskInfo, 0); //FizzPoint
		Write8(pFizzTaskInfo, 0); //DisplayPri
		Write8(pFizzTaskInfo, 0); //SpecialFlag

		len = pFizzTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 0); //FizzLotteryNum
	/*
	for (size_t i = 0; i < 0; i++)
	{ //FizzLotteryInfo
		BYTE* pFizzLotteryInfo = p;
		Write16(pFizzLotteryInfo, 0); //len

		Write8(pFizzLotteryInfo, 0); //LotteryType
		Write8(pFizzLotteryInfo, 0); //Status
		Write16(pFizzLotteryInfo, 0); //FizzPoint
		Write8(pFizzLotteryInfo, 0); //ControlledItemNum
		Write32(pFizzLotteryInfo, 0); //ControlledItem[]

		len = pFizzLotteryInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/

	len = p - buf.data();
	SendToClient(Client, 944, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetShoppingCarItemList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	//ResponseGetShoppingCarItemList(Client);
}
void ResponseGetShoppingCarItemList(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	{ //ShoppingCarItemList
		BYTE* pShoppingCarItemList = p;
		Write16(pShoppingCarItemList, 0); //len

		Write8(pShoppingCarItemList, 1); //ItemNum
		Write32(pShoppingCarItemList, 10020); //Item[]

		len = pShoppingCarItemList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 452, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetKeyTranslateInfoNew(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)//获取新键盘设置
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseGetKeyTranslateInfoNew(Client);
}
void ResponseGetKeyTranslateInfoNew(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, 0); //Uin
	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	{ //KeyTranslateInfo
		BYTE* pKeyTranslateInfo = p;
		Write16(pKeyTranslateInfo, 0); //len

		Write8(pKeyTranslateInfo, 0); //KeyTranslateDefaultKeyNum1
		Write8(pKeyTranslateInfo, 0); //KeyTranslateNewKeyNum1
		Write8(pKeyTranslateInfo, 0); //KeyTranslateDefaultKeyNum2
		Write8(pKeyTranslateInfo, 0); //KeyTranslateNewKeyNum2

		len = pKeyTranslateInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 24215, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



void RequestGetMaster(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseGetMaster(Client);
}
void ResponseGetMaster(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write8(p, 0); //AccStatus
	Write32(p, 0); //MasterUin
	Write32(p, 0); //AccTime
	Write32(p, 0); //GraduateTime
	Write16(p, 0); //RecentPMAwardNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //RecentPMAwards
		BYTE* pRecentPMAwards = p;
		Write16(pRecentPMAwards, 0); //len

		Write16(pRecentPMAwards, 0); //RecentPMAwardDesLen 描述长度

		len = pRecentPMAwards - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 363, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestSSCOpenStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)//SSC比赛打开状态
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseSSCOpenStatus(Client);
}
void ResponseSSCOpenStatus(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //Satus
	WriteString_MC(p, "SSC线上赛火爆开战!"); //TipsLen

	len = p - buf.data();
	SendToClient(Client, 24721, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestSwitchInterface(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	bool InterfaceType = Read8(Body);

	ResponseSwitchInterface(Client);
}
void ResponseSwitchInterface(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write32(p, 0); //Uin

	len = p - buf.data();
	SendToClient(Client, 151, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void ResponseSecondRefitKart(std::shared_ptr<ClientNode> Client, int KartItemId, int RefitItemID, short Speed2Weight, short DriftVecWeight, short AdditionalZSpeedWeight)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, KartItemId); //KartItemID
	Write32(p, RefitItemID); //RefitItemID
	Write16(p, 0); //RefitTimes
	Write16(p, 1); //ResponseIdx
	Write8(p, 1); //LastIdx
	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen

	{ //Increasement
		BYTE* pIncreasement = p;
		Write16(pIncreasement, 0); //len

		Write32(pIncreasement, Client->Uin); //Uin
		Write32(pIncreasement, KartItemId); //KartId
		Write32(pIncreasement, Client->ClientKartRefitItemInfo.RefitCout); //RefitCout
		Write16(pIncreasement, 0); //MaxFlags
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.WWeight); //WWeight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.SpeedWeight); //SpeedWeight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.JetWeight); //JetWeight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.SJetWeight); //SJetWeight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.AccuWeight); //AccuWeight
		Write32(pIncreasement, 0); //ShapeRefitCount
		Write32(pIncreasement, 0); //KartHeadRefitItemID
		Write32(pIncreasement, 0); //KartTailRefitItemID
		Write32(pIncreasement, 0); //KartFlankRefitItemID
		Write32(pIncreasement, 0); //KartTireRefitItemID
		Write32(pIncreasement, 0); //SecondRefitCount
		Write16(pIncreasement, Speed2Weight); //Speed2Weight
		Write16(pIncreasement, DriftVecWeight); //DriftVecWeight
		Write16(pIncreasement, AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.AntiCollisionWeight); //AntiCollisionWeight
		Write16(pIncreasement, 0); //LuckyValue
		Write16(pIncreasement, 0); //RefitLuckyValueMaxWeight
		Write32(pIncreasement, 0); //ShapeSuitID
		Write8(pIncreasement, 0); //LegendSuitLevel
		Write32(pIncreasement, 0); //LegendSuitLevelChoice
		Write32(pIncreasement, 0); //ShapeLegendSuitID

		len = pIncreasement - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //CurAttr
		BYTE* pCurAttr = p;
		Write16(pCurAttr, 0); //len

		Write32(pCurAttr, Client->Uin); //Uin
		Write32(pCurAttr, KartItemId); //KartId
		Write32(pCurAttr, Client->ClientKartRefitItemInfo.RefitCout); //RefitCout
		Write16(pCurAttr, 0); //MaxFlags
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.WWeight); //WWeight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.SpeedWeight); //SpeedWeight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.JetWeight); //JetWeight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.SJetWeight); //SJetWeight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.AccuWeight); //AccuWeight
		Write32(pCurAttr, 0); //ShapeRefitCount
		Write32(pCurAttr, 0); //KartHeadRefitItemID
		Write32(pCurAttr, 0); //KartTailRefitItemID
		Write32(pCurAttr, 0); //KartFlankRefitItemID
		Write32(pCurAttr, 0); //KartTireRefitItemID
		Write32(pCurAttr, 0); //SecondRefitCount
		Write16(pCurAttr, Speed2Weight); //Speed2Weight
		Write16(pCurAttr, DriftVecWeight); //DriftVecWeight
		Write16(pCurAttr, AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.AntiCollisionWeight); //AntiCollisionWeight
		Write16(pCurAttr, 0); //LuckyValue
		Write16(pCurAttr, 0); //RefitLuckyValueMaxWeight
		Write32(pCurAttr, 0); //ShapeSuitID
		Write8(pCurAttr, 0); //LegendSuitLevel
		Write32(pCurAttr, 0); //LegendSuitLevelChoice
		Write32(pCurAttr, 0); //ShapeLegendSuitID

		len = pCurAttr - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //ExpAward
	Write32(p, 0); //RefitFragmentItemID
	Write32(p, 0); //RefitFragmentItemNum

	len = p - buf.data();
	SendToClient(Client, 21110, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestSecondRefitKart(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int KartItemID = Read32(p);
	int RefitItemID = Read32(p);
	short RefitCount = Read16(p);
	//UCHAR KartNameLen = Read8(p);
	//const char* Reason = "改装成功!";
	
	int RefitItemNum = 0;
	//查询当前车辆改装信息
	short Speed2Weight = 0;
	short DriftVecWeight = 0;
	short AdditionalZSpeedWeight = 0;
	char RefitUseItem = 0;

	RefitItemNum = PlayerDB_GetItemNum(Client->Uin, RefitItemID);

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(KartItemID);
	std::vector<int> resultRow = executeQueryRow("SELECT Speed2Weight,DriftVecWeight,AdditionalZSpeedWeight FROM KartRefit WHERE Uin=? AND KartID=?;", "RequestSecondRefitKart", conditionValues);
	if (resultRow.size() >= 3) {
		Speed2Weight = resultRow[0];
		DriftVecWeight = resultRow[1];
		AdditionalZSpeedWeight = resultRow[2];
	}



	char RefitLimit = 0;

	while (RefitCount > 0)
	{
		RefitCount--;
		if (RefitItemNum > 0) {
			//改装上限29
			switch (RefitItemID)
			{
			case 65025: //普通推进+1
				Speed2Weight += 1;
				if (Speed2Weight > 40)
				{
					Speed2Weight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65026: //精推进+2
				Speed2Weight += 2;
				if (Speed2Weight > 40)
				{
					Speed2Weight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65027: //极致推进+3
				Speed2Weight += 3;
				if (Speed2Weight > 40)
				{
					Speed2Weight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65034: //推进-1
				Speed2Weight -= 1;
				if (Speed2Weight <= 0)
				{
					Speed2Weight = 0;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65028: //离合+1
				DriftVecWeight += 1;
				if (DriftVecWeight > 40)
				{
					DriftVecWeight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65029: //离合+2
				DriftVecWeight += 2;
				if (DriftVecWeight > 40)
				{
					DriftVecWeight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65030: //离合+3
				DriftVecWeight += 3;
				if (DriftVecWeight > 40)
				{
					DriftVecWeight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65035: //离合-1
				DriftVecWeight -= 1;
				if (DriftVecWeight <= 0)
				{
					DriftVecWeight = 0;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65031: //平衡+1
				AdditionalZSpeedWeight += 1;
				if (AdditionalZSpeedWeight > 40)
				{
					AdditionalZSpeedWeight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65032: //平衡+2
				AdditionalZSpeedWeight += 2;
				if (AdditionalZSpeedWeight > 40)
				{
					AdditionalZSpeedWeight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65033: //平衡+3
				AdditionalZSpeedWeight += 3;
				if (AdditionalZSpeedWeight > 40)
				{
					AdditionalZSpeedWeight = 40;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			case 65036: //平衡-1
				AdditionalZSpeedWeight -= 1;
				if (AdditionalZSpeedWeight <= 0)
				{
					AdditionalZSpeedWeight = 0;
					RefitLimit = 1;
					ResponseStartGame(Client, "已达改装上限!");
					break;
				}
				break;
			default:
				ResponseStartGame(Client, "暂未配置改装!");
				//printf("改装部位:%u\n", RefitItemId);
				break;
			}
			if (RefitLimit == 1)
			{
				break;
			}
			RefitItemNum--;
			RefitUseItem++;
		}
	}
	if (KartItemID == Client->KartID)
	{
		//更新内存性能
		Client->ClientKartRefitItemInfo.Speed2Weight = Speed2Weight;
		Client->ClientKartRefitItemInfo.DriftVecWeight = DriftVecWeight;
		Client->ClientKartRefitItemInfo.AdditionalZSpeedWeight = AdditionalZSpeedWeight;
	}

	UpdateClientKart(Client, Speed2Weight, DriftVecWeight, AdditionalZSpeedWeight, KartItemID);
	if (RefitItemNum <= 0) {
		PlayerDB_DeleteItem(Client->Uin, RefitItemID);
	}
	else
	{
		PlayerDB_UpdateItemNum(Client->Uin, RefitItemID, RefitItemNum);
	}

	NotifyUseItemResult(Client, RefitItemID, RefitUseItem, RefitItemNum);
	ResponseSecondRefitKart(Client, KartItemID, RefitItemID, Speed2Weight, DriftVecWeight, AdditionalZSpeedWeight);
}



void RequestSecondGetKartRefitLimitCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//二次进阶改装
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseSecondGetKartRefitLimitCfg(Client);
}
void ResponseSecondGetKartRefitLimitCfg(std::shared_ptr<ClientNode> Client)
{

	//回复进阶改装信息?
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	const static UINT OpenKartTypeListCfg[] = {
		// 可以进阶改装的赛车类型列表
		EAIT_CAR_NEW_A,
		EAIT_CAR_NEW_B,
		EAIT_CAR_NEW_D,
		//EAIT_CAR_NEW_S,
		EAIT_CAR_NEW_M,
		EAIT_CAR_NEW_M0,
		EAIT_CAR_NEW_L3,
		EAIT_CAR_NEW_M3,
		EAIT_CAR_NEW_M4,
		EAIT_CAR_NEW_L2,
		EAIT_CAR_NEW_L3,
		EAIT_CAR_NEW_L4,
		EAIT_CAR_NEW_M2,
		EAIT_CAR_NEW_C,
		EAIT_CAR_NEW_L0,
		EAIT_CAR_NEW_L1,
		EAIT_CAR_NEW_M1,
		// 1565,
		//EAIT_CAR_NEW_T1,
		//EAIT_CAR_NEW_T2,

		//EAIT_CAR_NEW_T3,

	};

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	{ //KartRefitLimitCfg
		BYTE* pKartRefitLimitCfg = p;
		Write16(pKartRefitLimitCfg, 0); //len

		//Write16(pKartRefitLimitCfg, 21); //OpenKartTypeNum 
		//for (int i = 1539; i <= 1559; i++) //EAIT_CAR_NEW_A - EAIT_CAR_NEW_T4
		//{
		//	/*if (i == 1543)
		//		continue;*/
		//	Write32(pKartRefitLimitCfg, i);//OpenKartTypeList[]
		//}
		Write16(pKartRefitLimitCfg, sizeof(OpenKartTypeListCfg) / sizeof(OpenKartTypeListCfg[0])); // OpenKartTypeNum
		for (size_t i = 0; i < sizeof(OpenKartTypeListCfg) / sizeof(OpenKartTypeListCfg[0]); i++)
		{
			Write32(pKartRefitLimitCfg, OpenKartTypeListCfg[i]); // OpenKartTypeList[]
		}

		Write32(pKartRefitLimitCfg, 0); //MinFormerRefitTime
		Write16(pKartRefitLimitCfg, 0); //ExcludeKartTypeNum
		//Write32(pKartRefitLimitCfg, 0); //ExcludeKartTypeList[]
		Write16(pKartRefitLimitCfg, 0); //ForbidListNum
		//Write32(pKartRefitLimitCfg, 0); //ForbidRefitKartID[]

		len = pKartRefitLimitCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // RefitItemWeightTypeCfg
		BYTE* pRefitItemWeightTypeCfg = p;
		Write16(pRefitItemWeightTypeCfg, 0); // len

		Write8(pRefitItemWeightTypeCfg, 5); // TypeNum
		for (size_t i = 0; i < 4; i++)
		{ // ItemWeightLevel
			BYTE* pItemWeightLevel = pRefitItemWeightTypeCfg;
			Write16(pItemWeightLevel, 0); // len

			Write8(pItemWeightLevel, i); // ItemWeightLevel
			Write16(pItemWeightLevel, 0);		 // MinWeight
			Write16(pItemWeightLevel, 50);		 // MaxWeight
			Write16(pItemWeightLevel, 0);		 // ExpAward

			len = pItemWeightLevel - pRefitItemWeightTypeCfg;
			Set16(pRefitItemWeightTypeCfg, (WORD)len);
			pRefitItemWeightTypeCfg += len;
		}
		{ // ItemWeightLevel
			BYTE* pItemWeightLevel = pRefitItemWeightTypeCfg;
			Write16(pItemWeightLevel, 0); // len

			Write8(pItemWeightLevel, 0); // ItemWeightLevel
			Write16(pItemWeightLevel, 0);		 // MinWeight
			Write16(pItemWeightLevel, 0);		 // MaxWeight
			Write16(pItemWeightLevel, 0);		 // ExpAward

			len = pItemWeightLevel - pRefitItemWeightTypeCfg;
			Set16(pRefitItemWeightTypeCfg, (WORD)len);
			pRefitItemWeightTypeCfg += len;
		}
		len = pRefitItemWeightTypeCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // RefitItemCfg
		BYTE* pRefitItemCfg = p;
		Write16(pRefitItemCfg, 0); // len

		Write16(pRefitItemCfg, 12); // ItemNum
		for (size_t i = 0; i < 12; i++)
		{ // RefitItem
			BYTE* pRefitItem = pRefitItemCfg;
			Write16(pRefitItem, 0); // len

			Write32(pRefitItem, RefitItemCfg[i].ItemID);			   // ItemID
			Write8(pRefitItem, RefitItemCfg[i].ItemType);			   // ItemType
			Write8(pRefitItem, RefitItemCfg[i].ItemWeightLevel);	   // ItemWeightLevel
			Write16(pRefitItem, RefitItemCfg[i].Value);				   // Value
			Write32(pRefitItem, RefitItemCfg[i].FragmentItemID);	   // FragmentItemID
			Write32(pRefitItem, RefitItemCfg[i].FragmentNumToOneItem); // FragmentNumToOneItem
			Write16(pRefitItem, RefitItemCfg[i].IsItemOpen);		   // IsItemOpen

			len = pRefitItem - pRefitItemCfg;
			Set16(pRefitItemCfg, (WORD)len);
			pRefitItemCfg += len;
		}

		len = pRefitItemCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 21111, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}




void RequestGetItemTimeLimtCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UINT UpdateTime = Read32(Body);

	ResponsetGetItemTimeLimtCfg(Client, UpdateTime);
}
void ResponsetGetItemTimeLimtCfg(std::shared_ptr<ClientNode> Client, UINT UpdateTime)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID

	Write32(p, 0); //ItemNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //ItemTimeLimit
		BYTE* pItemTimeLimit = p;
		Write16(pItemTimeLimit, 0); //len

		Write32(pItemTimeLimit, 0); //ItemID
		Write32(pItemTimeLimit, 0); //ExpireTime

		len = pItemTimeLimit - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write32(p, UpdateTime); //UpdateTime

	len = p - buf.data();
	SendToClient(Client, 24201, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


NOINLINE void RequestGetShapeRefitCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	//printf("size:%d\n", sizeof(MolData::acResponseGetShapeRefitCfg));

	//SendToClient(Client, 317, MolData::acResponseGetShapeRefitCfg1, sizeof(MolData::acResponseGetShapeRefitCfg1), Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	ResponseGetShapeRefitCfgTest(Client);
	//ResponseGetShapeRefitCfg(Client);
}


void ResponseGetShapeRefitCfgTest(std::shared_ptr<ClientNode> Client)
{
	int n = 1;
	//BYTE buf[8192 * 4];
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;
	YAML::Node ShapeRefitCfg = YAML::LoadFile(".\\Config\\ShapeRefitCfg.yml");
	Write16(p, 0); // ResultID
	BYTE* pKartShapeRefitSuitNum = p;
	// size_t i = 1;
	Write16(p, ShapeRefitCfg["ShapeRefitLefend"].size()); // KartShapeRefitSuitNum
	for (size_t i = 0; i < ShapeRefitCfg["ShapeRefitLefend"].size(); i++)
	{ // ShapeRefitSuit

		BYTE* pShapeRefitSuit = p;
		Write16(pShapeRefitSuit, 0); // len

		Write32(pShapeRefitSuit, ShapeRefitCfg["ShapeRefitLefend"][i]["KartID"].as<int>()); // KartID
		if (ShapeRefitCfg["ShapeRefitLefend"][i]["KarNameLen"])
		{
			std::string GradeResource = ShapeRefitCfg["ShapeRefitLefend"][i]["KarNameLen"].as<std::string>();
			UINT GradeResourceLen = strlen(GradeResource.c_str());
			Write8(pShapeRefitSuit, GradeResourceLen);						  // KarNameLen
			memcpy(pShapeRefitSuit, GradeResource.c_str(), GradeResourceLen); // Resource 资源 兔年限定
			pShapeRefitSuit += GradeResourceLen;
		}
		else
		{
			Write8(pShapeRefitSuit, 0);		 // KarNameLen
		}
		Write8(pShapeRefitSuit, 1);	 // RefitItemNum
		Write32(pShapeRefitSuit, 0); // RefitItem[]
		INT EffectNum = ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"].size();
		Write8(pShapeRefitSuit, EffectNum); // EffectNum
		for (size_t j = 0; j < EffectNum; j++)
		{ // RefitEffects
			BYTE* pRefitEffects = pShapeRefitSuit;
			Write16(pRefitEffects, 0); // len

			Write16(pRefitEffects, ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["EffectID"].as<int>()); // EffectID
			Write8(pRefitEffects, ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["Level"].as<int>());  // Level
			Write32(pRefitEffects, ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["Value1"].as<int>()); // Value1
			Write32(pRefitEffects, ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["Value2"].as<int>()); // Value2
			std::string Resource = ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["Resource"].as<std::string>();
			UINT ResourceLen = strlen(Resource.c_str());
			Write8(pRefitEffects, ResourceLen);										   // ResourceLen
			memcpy(pRefitEffects, Resource.c_str(), ResourceLen); // Resource 资源
			pRefitEffects += ResourceLen;

			std::string EffectRes = ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["EffectRes"].as<std::string>();
			UINT EffectResLen = strlen(EffectRes.c_str());
			Write8(pRefitEffects, EffectResLen); // EffectResLen
			memcpy(pRefitEffects, EffectRes.c_str(), EffectResLen); // EffectRes 资源
			pRefitEffects += EffectResLen;

			Write8(pRefitEffects, ShapeRefitCfg["ShapeRefitLefend"][i]["RefitEffects"][j]["Grade"].as<int>()); // Grade

			len = pRefitEffects - pShapeRefitSuit;
			Set16(pShapeRefitSuit, (WORD)len);
			pShapeRefitSuit += len;
		}
		if (ShapeRefitCfg["ShapeRefitLefend"][i]["Grade"])
		{
			Write16(pShapeRefitSuit, ShapeRefitCfg["ShapeRefitLefend"][i]["Grade"].as<UINT>()); // Grade
		}
		else
		{
			Write16(pShapeRefitSuit, 0); // Grade
		}

		Write16(pShapeRefitSuit, 10);  // DressPoint
		Write16(pShapeRefitSuit, 200); // CarLibraryCollectValue
		INT RefitModifyNum = 0;
		Write8(pShapeRefitSuit, RefitModifyNum); // RefitModifyNum
		for (size_t i = 0; i < RefitModifyNum; i++)
		{ // RefitModify
			BYTE* pRefitModify = pShapeRefitSuit;
			Write16(pRefitModify, 0); // len

			Write16(pRefitModify, 1); // RefitID
			Write8(pRefitModify, 0);  // RefitModify
			Write32(pRefitModify, 0); // RefitLimitInc
			Write32(pRefitModify, 0); // RefitInc

			len = pRefitModify - pShapeRefitSuit;
			Set16(pShapeRefitSuit, (WORD)len);
			pShapeRefitSuit += len;
		}
		INT HasTurnModify = 0;
		Write8(pShapeRefitSuit, HasTurnModify); // HasTurnModify
		for (size_t i = 0; i < HasTurnModify; i++)
		{ // TurnModify
			BYTE* pTurnModify = pShapeRefitSuit;
			Write16(pTurnModify, 0); // len

			Write32(pTurnModify, 0); // BaseTurnRate
			Write32(pTurnModify, 0); // VolatileTurnRate
			Write32(pTurnModify, 0); // MaxTurnSpeed
			Write32(pTurnModify, 0); // MinTurnSpeed
			Write32(pTurnModify, 0); // MaxAccuTime
			Write32(pTurnModify, 0); // BaseAccuRate
			Write32(pTurnModify, 0); // MaxAffectSpeed

			len = pTurnModify - pShapeRefitSuit;
			Set16(pShapeRefitSuit, (WORD)len);
			pShapeRefitSuit += len;
		}

		// int ShapeSuitID = 127616;
		Write32(pShapeRefitSuit, ShapeRefitCfg["ShapeRefitLefend"][i]["ShapeSuitID"].as<int>()); // ShapeSuitID
		Write8(pShapeRefitSuit, 4);																 // RefitAddtionNum
		Write32(pShapeRefitSuit, 0);															 // RefitAddition[]
		Write32(pShapeRefitSuit, 0);															 // RefitAddition[]
		Write32(pShapeRefitSuit, 0);															 // RefitAddition[]
		Write32(pShapeRefitSuit, 0);															 // RefitAddition[]
		if (ShapeRefitCfg["ShapeRefitLefend"][i]["GradeResourceLen"])
		{
			std::string GradeResource = ShapeRefitCfg["ShapeRefitLefend"][i]["GradeResourceLen"].as<std::string>();
			UINT GradeResourceLen = strlen(GradeResource.c_str());
			Write8(pShapeRefitSuit, GradeResourceLen);						  // GradeResourceLen
			memcpy(pShapeRefitSuit, GradeResource.c_str(), GradeResourceLen); // Resource 资源 兔年限定
			pShapeRefitSuit += GradeResourceLen;
		}
		else
		{
			std::string GradeResource = "Rare";
			UINT GradeResourceLen = strlen(GradeResource.c_str());
			Write8(pShapeRefitSuit, GradeResourceLen);						  // GradeResourceLen
			memcpy(pShapeRefitSuit, GradeResource.c_str(), GradeResourceLen); // Resource 资源 兔年限定
			pShapeRefitSuit += GradeResourceLen;
		}

		len = pShapeRefitSuit - p;
		Set16(p, (WORD)len);
		p += len;
	}

	//Z_INFO("赛车皮肤加载完成[%d]!\n", ShapeRefitCfg["ShapeRefitLefend"].size());

	std::vector<KartShapeRefitLegendInfo> KartShapeRefitLegendInfoCfg = getLegendarySkin();

	UCHAR KartShapeRefitLegendNum = KartShapeRefitLegendInfoCfg.size();
	Write16(p, KartShapeRefitLegendNum); // KartShapeRefitLegendNum
	for (size_t Ki = 0; Ki < KartShapeRefitLegendNum; Ki++)
	{ // ShapeRefitLegend
		BYTE* pShapeRefitLegend = p;
		Write16(pShapeRefitLegend, 0); // len

		Write32(pShapeRefitLegend, KartShapeRefitLegendInfoCfg[Ki].KartID); // KartID		爆天甲
		UCHAR KarNameLen = KartShapeRefitLegendInfoCfg[Ki].KarName.length();
		Write8(pShapeRefitLegend, KarNameLen); // KarNameLen
		if (KarNameLen)
		{
			memcpy(pShapeRefitLegend, KartShapeRefitLegendInfoCfg[Ki].KarName.data(), KarNameLen); // 品级 资源 传奇
			pShapeRefitLegend += KarNameLen;
		}
		Write16(pShapeRefitLegend, KartShapeRefitLegendInfoCfg[Ki].Grade); // Grade			品级

		UCHAR GradeResourceLen = KartShapeRefitLegendInfoCfg[Ki].GradeResource.length();

		Write8(pShapeRefitLegend, GradeResourceLen); // GradeResourceLen
		if (GradeResourceLen)
		{
			memcpy(pShapeRefitLegend, KartShapeRefitLegendInfoCfg[Ki].GradeResource.data(), GradeResourceLen); // 品级 资源 传奇
			pShapeRefitLegend += GradeResourceLen;
		}

		auto& LevelInfoGroup = KartShapeRefitLegendInfoCfg[Ki].LevelInfo;
		UCHAR LevelNum = LevelInfoGroup.size();

		Write8(pShapeRefitLegend, LevelNum); // LevelNum  皮肤等级数量
		for (size_t LevelNumi = 0; LevelNumi < LevelNum; LevelNumi++)
		{ // LevelCfg
			BYTE* pLevelCfg = pShapeRefitLegend;
			Write16(pLevelCfg, 0); // len

			Write8(pLevelCfg, LevelInfoGroup[LevelNumi].Level); // Level
			Write8(pLevelCfg, 0);								// RefitItemNum
			// Write32(pLevelCfg, 0); //RefitItem[]

			auto& EffectGroup = LevelInfoGroup[LevelNumi].RefitEffectsInfo;
			UCHAR EffectNum = EffectGroup.size();

			Write8(pLevelCfg, EffectNum); // EffectNum
			for (size_t i = 0; i < EffectNum; i++)
			{ // RefitEffects
				BYTE* pRefitEffects = pLevelCfg;
				Write16(pRefitEffects, 0); // len

				Write16(pRefitEffects, EffectGroup[i].EffectID); // EffectID
				Write8(pRefitEffects, EffectGroup[i].Level);	 // Level
				Write32(pRefitEffects, EffectGroup[i].Value1);	 // Value1
				Write32(pRefitEffects, EffectGroup[i].Value2);	 // Value2

				UCHAR ResourceLen = EffectGroup[i].Resource.length();
				Write8(pRefitEffects, ResourceLen); // ResourceLen
				if (ResourceLen)
				{
					memcpy(pRefitEffects, EffectGroup[i].Resource.data(), ResourceLen); // 品级 资源 传奇
					pRefitEffects += ResourceLen;
				}
				UCHAR EffectResLen = EffectGroup[i].EffectRes.length();
				Write8(pRefitEffects, EffectResLen); // EffectResLen
				if (EffectResLen)
				{
					memcpy(pRefitEffects, EffectGroup[i].EffectRes.data(), EffectResLen); // 品级 资源 传奇
					pRefitEffects += EffectResLen;
				}

				Write8(pRefitEffects, EffectGroup[i].Grade); // Grade

				len = pRefitEffects - pLevelCfg;
				Set16(pLevelCfg, (WORD)len);
				pLevelCfg += len;
			}
			Write16(pLevelCfg, LevelInfoGroup[LevelNumi].DressPoint);			  // DressPoint
			Write16(pLevelCfg, LevelInfoGroup[LevelNumi].CarLibraryCollectValue); // CarLibraryCollectValue
			Write8(pLevelCfg, 0);												  // RefitModifyNum
			for (size_t i = 0; i < 0; i++)
			{ // RefitModify
				BYTE* pRefitModify = pLevelCfg;
				Write16(pRefitModify, 0); // len

				Write16(pRefitModify, 0); // RefitID
				Write8(pRefitModify, 0);  // RefitModify
				Write32(pRefitModify, 0); // RefitLimitInc
				Write32(pRefitModify, 0); // RefitInc

				len = pRefitModify - pLevelCfg;
				Set16(pLevelCfg, (WORD)len);
				pLevelCfg += len;
			}
			Write8(pLevelCfg, 0); // HasTurnModify
			for (size_t i = 0; i < 0; i++)
			{ // TurnModify
				BYTE* pTurnModify = pLevelCfg;
				Write16(pTurnModify, 0); // len

				Write32(pTurnModify, 0); // BaseTurnRate
				Write32(pTurnModify, 0); // VolatileTurnRate
				Write32(pTurnModify, 0); // MaxTurnSpeed
				Write32(pTurnModify, 0); // MinTurnSpeed
				Write32(pTurnModify, 0); // MaxAccuTime
				Write32(pTurnModify, 0); // BaseAccuRate
				Write32(pTurnModify, 0); // MaxAffectSpeed

				len = pTurnModify - pLevelCfg;
				Set16(pLevelCfg, (WORD)len);
				pLevelCfg += len;
			}
			Write32(pLevelCfg, LevelInfoGroup[LevelNumi].ShapeSuitID); // ShapeSuitID

			auto& ConsumInfoGroup = LevelInfoGroup[LevelNumi].ConsumInfo;
			UCHAR ConsumeNum = ConsumInfoGroup.size();

			Write8(pLevelCfg, ConsumeNum); // ConsumeNum
			for (size_t i = 0; i < ConsumeNum; i++)
			{ // ConsumInfo
				BYTE* pConsumInfo = pLevelCfg;
				Write16(pConsumInfo, 0); // len

				Write32(pConsumInfo, ConsumInfoGroup[i].ItemID);	 // ItemID
				Write32(pConsumInfo, ConsumInfoGroup[i].ItemNum);	 // ItemNum
				Write32(pConsumInfo, ConsumInfoGroup[i].ItemPeriod); // ItemPeriod

				len = pConsumInfo - pLevelCfg;
				Set16(pLevelCfg, (WORD)len);
				pLevelCfg += len;
			}

			Write16(pLevelCfg, 0); // 未知参数

			len = pLevelCfg - pShapeRefitLegend;
			Set16(pShapeRefitLegend, (WORD)len);
			pShapeRefitLegend += len;
		}
		Write32(pShapeRefitLegend, KartShapeRefitLegendInfoCfg[Ki].ShapeSuitID); // ShapeSuitID

		auto& MaxConsumInfo = KartShapeRefitLegendInfoCfg[Ki].MaxConsumInfo;

		{ // MaxConsumInfo
			BYTE* pMaxConsumInfo = pShapeRefitLegend;
			Write16(pMaxConsumInfo, 0); // len

			Write32(pMaxConsumInfo, MaxConsumInfo.ItemID);	   // ItemID
			Write32(pMaxConsumInfo, MaxConsumInfo.ItemNum);	   // ItemNum
			Write32(pMaxConsumInfo, MaxConsumInfo.ItemPeriod); // ItemPeriod

			len = pMaxConsumInfo - pShapeRefitLegend;
			Set16(pShapeRefitLegend, (WORD)len);
			pShapeRefitLegend += len;
		}
		Write8(pShapeRefitLegend, KartShapeRefitLegendInfoCfg[Ki].LegendSkinType); // LegendSkinType

		len = pShapeRefitLegend - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 0xCDCD); // 未知参数
	Write8(p, 0); // 未知参数
	//Z_INFO("赛车传奇皮肤加载完成[%d]!\n", KartShapeRefitLegendInfoCfg.size());
	len = p - buf.data();
	SendToClient(Client, 317, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	//delete[] buf;
}


void RequestWeRelayCommonCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseWeRelayCommonCfg(Client);
}
//void ResponseWeRelayCommonCfg(std::shared_ptr<ClientNode> Client)
//{
//	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
//
//	Write16(p, 0); //ResultID
//	Write32(p, Client->Uin); //Uin
//	{ //SeasonCfg
//		BYTE* pSeasonCfg = p;
//		Write16(pSeasonCfg, 0); //len
//
//		Write32(pSeasonCfg, 1); //SeasonId
//		Write32(pSeasonCfg, 0); //OpenDate
//		Write32(pSeasonCfg, 0); //CloseDate
//		Write32(pSeasonCfg, 0); //OpenDayNum
//		Write32(pSeasonCfg, 0); //BroadCastContentLen
//		Write32(pSeasonCfg, 0); //OpenTimeTipsLen
//
//		const int MAX_SPEEDRALLY_OPEN_DAY_NUM = 1;
//		for (size_t i = 0; i < MAX_SPEEDRALLY_OPEN_DAY_NUM; i++)
//		{ //tDayOpenCfg
//			BYTE* ptDayOpenCfg = pSeasonCfg;
//			Write16(ptDayOpenCfg, 0); //len
//
//			Write32(ptDayOpenCfg, 0); //DayID
//			Write32(ptDayOpenCfg, 0); //OpenTimeNum
//			/*
//			for (size_t i = 0; i < MAX_OPEN_TIME_NUM; i++)
//			{ //tOpenTimeCfg
//				BYTE* ptOpenTimeCfg = ptDayOpenCfg;
//				Write16(ptOpenTimeCfg, 0); //len
//
//				Write32(ptOpenTimeCfg, 0); //OpenTime
//				Write32(ptOpenTimeCfg, 0); //CloseTime
//				Write32(ptOpenTimeCfg, 0); //TiyanOpenTime
//				Write32(ptOpenTimeCfg, 0); //TiyanCloseTime
//
//				len = ptOpenTimeCfg - ptDayOpenCfg;
//				Set16(ptDayOpenCfg, (WORD)len);
//				ptDayOpenCfg += len;
//			}
//			*/
//			len = ptDayOpenCfg - pSeasonCfg;
//			Set16(pSeasonCfg, (WORD)len);
//			pSeasonCfg += len;
//		}
//		Write32(pSeasonCfg, 0); //LuckyBoxIndex
//		Write32(pSeasonCfg, 0); //LuckyBoxPriority
//		Write32(pSeasonCfg, 0); //LuckyBoxDayLimit
//
//		len = pSeasonCfg - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//	Write32(p, 0); //RankNum
//	/*
//	for (size_t i = 0; i < n; i++)
//	{ //RankAwardInfo
//		BYTE* pRankAwardInfo = p;
//		Write16(pRankAwardInfo, 0); //len
//
//		Write32(pRankAwardInfo, 0); //RankType
//		Write32(pRankAwardInfo, 0); //AwardNum
//		for (size_t i = 0; i < n; i++)
//		{ //BaseRankAwardToClient
//			BYTE* pBaseRankAwardToClient = pRankAwardInfo;
//			Write16(pBaseRankAwardToClient, 0); //len
//
//			Write32(pBaseRankAwardToClient, 0); //Index
//			Write32(pBaseRankAwardToClient, 0); //RankBegin
//			Write32(pBaseRankAwardToClient, 0); //RankEnd
//			{ //Award
//				BYTE* pAward = pBaseRankAwardToClient;
//				Write16(pAward, 0); //len
//
//				Write32(pAward, 0); //Exp
//				Write32(pAward, 0); //Money
//				Write32(pAward, 0); //SuperMoney
//				Write16(pAward, 0); //ItemNum
//				for (size_t i = 0; i < n; i++)
//				{ //ItemInfo
//					BYTE* pItemInfo = pAward;
//					Write16(pItemInfo, 0); //len
//
//					Write32(pItemInfo, 0); //ItemID
//					Write32(pItemInfo, 0); //ItemNum
//					Write32(pItemInfo, 0); //AvailPeriod
//					Write8(pItemInfo, 0); //Status
//					WriteUn(pItemInfo, 0); //ObtainTime
//					Write32(pItemInfo, 0); //OtherAttribute
//					Write16(pItemInfo, 0); //ItemType
//
//					len = pItemInfo - pAward;
//					Set16(pAward, (WORD)len);
//					pAward += len;
//				}
//				Write32(pAward, 0); //Coupons
//				Write32(pAward, 0); //GuildPoint
//				Write32(pAward, 0); //LuckMoney
//				Write8(pAward, 0); //ExtendInfoNum
//				for (size_t i = 0; i < n; i++)
//				{ //ExtendInfoAward
//					BYTE* pExtendInfoAward = pAward;
//					Write16(pExtendInfoAward, 0); //len
//
//					Write16(pExtendInfoAward, 0); //Key
//					Write32(pExtendInfoAward, 0); //AddValue
//					Write32(pExtendInfoAward, 0); //TotalValue
//					Write8(pExtendInfoAward, 0); //ShowPriority
//					Write16(pExtendInfoAward, 0); //BuyNeedScore
//					Write8(pExtendInfoAward, 0); //OneMatchMaxNum
//
//					len = pExtendInfoAward - pAward;
//					Set16(pAward, (WORD)len);
//					pAward += len;
//				}
//				Write32(pAward, 0); //SpeedCoin
//
//				len = pAward - pBaseRankAwardToClient;
//				Set16(pBaseRankAwardToClient, (WORD)len);
//				pBaseRankAwardToClient += len;
//			}
//			Write32(pBaseRankAwardToClient, 0); //Status
//
//			len = pBaseRankAwardToClient - pRankAwardInfo;
//			Set16(pRankAwardInfo, (WORD)len);
//			pRankAwardInfo += len;
//		}
//
//		len = pRankAwardInfo - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//	*/
//	Write32(p, 0); //ActiveAwardNum
//	/*
//	for (size_t i = 0; i < n; i++)
//	{ //ActiveAwardToClient
//		BYTE* pActiveAwardToClient = p;
//		Write16(pActiveAwardToClient, 0); //len
//
//		Write32(pActiveAwardToClient, 0); //Index
//		Write32(pActiveAwardToClient, 0); //ActiveValueLimit
//		Write32(pActiveAwardToClient, 0); //RankLevLimit
//		{ //Award
//			BYTE* pAward = pActiveAwardToClient;
//			Write16(pAward, 0); //len
//
//			Write32(pAward, 0); //Exp
//			Write32(pAward, 0); //Money
//			Write32(pAward, 0); //SuperMoney
//			Write16(pAward, 0); //ItemNum
//			for (size_t i = 0; i < n; i++)
//			{ //ItemInfo
//				BYTE* pItemInfo = pAward;
//				Write16(pItemInfo, 0); //len
//
//				Write32(pItemInfo, 0); //ItemID
//				Write32(pItemInfo, 0); //ItemNum
//				Write32(pItemInfo, 0); //AvailPeriod
//				Write8(pItemInfo, 0); //Status
//				WriteUn(pItemInfo, 0); //ObtainTime
//				Write32(pItemInfo, 0); //OtherAttribute
//				Write16(pItemInfo, 0); //ItemType
//
//				len = pItemInfo - pAward;
//				Set16(pAward, (WORD)len);
//				pAward += len;
//			}
//			Write32(pAward, 0); //Coupons
//			Write32(pAward, 0); //GuildPoint
//			Write32(pAward, 0); //LuckMoney
//			Write8(pAward, 0); //ExtendInfoNum
//			for (size_t i = 0; i < n; i++)
//			{ //ExtendInfoAward
//				BYTE* pExtendInfoAward = pAward;
//				Write16(pExtendInfoAward, 0); //len
//
//				Write16(pExtendInfoAward, 0); //Key
//				Write32(pExtendInfoAward, 0); //AddValue
//				Write32(pExtendInfoAward, 0); //TotalValue
//				Write8(pExtendInfoAward, 0); //ShowPriority
//				Write16(pExtendInfoAward, 0); //BuyNeedScore
//				Write8(pExtendInfoAward, 0); //OneMatchMaxNum
//
//				len = pExtendInfoAward - pAward;
//				Set16(pAward, (WORD)len);
//				pAward += len;
//			}
//			Write32(pAward, 0); //SpeedCoin
//
//			len = pAward - pActiveAwardToClient;
//			Set16(pActiveAwardToClient, (WORD)len);
//			pActiveAwardToClient += len;
//		}
//
//		len = pActiveAwardToClient - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//	*/
//	Write32(p, 0); //CarTypeNum
//	/*
//	for (size_t i = 0; i < n; i++)
//	{ //CarGuide
//		BYTE* pCarGuide = p;
//		Write16(pCarGuide, 0); //len
//
//		Write32(pCarGuide, 0); //CarType
//		Write32(pCarGuide, 0); //CarNum
//		for (size_t i = 0; i < n; i++)
//		{ //GuideCarInfo
//			BYTE* pGuideCarInfo = pCarGuide;
//			Write16(pGuideCarInfo, 0); //len
//
//			Write32(pGuideCarInfo, 0); //CarID
//
//			len = pGuideCarInfo - pCarGuide;
//			Set16(pCarGuide, (WORD)len);
//			pCarGuide += len;
//		}
//
//		len = pCarGuide - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//	*/
//	Write32(p, 0); //TiyanCarTypeNum
//	/*
//	for (size_t i = 0; i < n; i++)
//	{ //TiyanCarGuide
//		BYTE* pTiyanCarGuide = p;
//		Write16(pTiyanCarGuide, 0); //len
//
//		Write32(pTiyanCarGuide, 0); //CarType
//		Write32(pTiyanCarGuide, 0); //CarNum
//		for (size_t i = 0; i < n; i++)
//		{ //GuideCarInfo
//			BYTE* pGuideCarInfo = pTiyanCarGuide;
//			Write16(pGuideCarInfo, 0); //len
//
//			Write32(pGuideCarInfo, 0); //CarID
//
//			len = pGuideCarInfo - pTiyanCarGuide;
//			Set16(pTiyanCarGuide, (WORD)len);
//			pTiyanCarGuide += len;
//		}
//
//		len = pTiyanCarGuide - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//	*/
//	{ //LuckyAwardToClient
//		BYTE* pLuckyAwardToClient = p;
//		Write16(pLuckyAwardToClient, 0); //len
//
//		Write32(pLuckyAwardToClient, 0); //LuckBoxGet
//		{ //Award
//			BYTE* pAward = pLuckyAwardToClient;
//			Write16(pAward, 0); //len
//
//			Write32(pAward, 0); //Exp
//			Write32(pAward, 0); //Money
//			Write32(pAward, 0); //SuperMoney
//			Write16(pAward, 0); //ItemNum
//			/*
//			for (size_t i = 0; i < n; i++)
//			{ //ItemInfo
//				BYTE* pItemInfo = pAward;
//				Write16(pItemInfo, 0); //len
//
//				Write32(pItemInfo, 0); //ItemID
//				Write32(pItemInfo, 0); //ItemNum
//				Write32(pItemInfo, 0); //AvailPeriod
//				Write8(pItemInfo, 0); //Status
//				Write32(pItemInfo, 0); //ObtainTime
//				Write32(pItemInfo, 0); //OtherAttribute
//				Write16(pItemInfo, 0); //ItemType
//
//				len = pItemInfo - pAward;
//				Set16(pAward, (WORD)len);
//				pAward += len;
//			}
//			*/
//			Write32(pAward, 0); //Coupons
//			Write32(pAward, 0); //GuildPoint
//			Write32(pAward, 0); //LuckMoney
//			Write8(pAward, 0); //ExtendInfoNum
//			/*
//			for (size_t i = 0; i < n; i++)
//			{ //ExtendInfoAward
//				BYTE* pExtendInfoAward = pAward;
//				Write16(pExtendInfoAward, 0); //len
//
//				Write16(pExtendInfoAward, 0); //Key
//				Write32(pExtendInfoAward, 0); //AddValue
//				Write32(pExtendInfoAward, 0); //TotalValue
//				Write8(pExtendInfoAward, 0); //ShowPriority
//				Write16(pExtendInfoAward, 0); //BuyNeedScore
//				Write8(pExtendInfoAward, 0); //OneMatchMaxNum
//
//				len = pExtendInfoAward - pAward;
//				Set16(pAward, (WORD)len);
//				pAward += len;
//			}
//			*/
//			Write32(pAward, 0); //SpeedCoin
//
//			len = pAward - pLuckyAwardToClient;
//			Set16(pLuckyAwardToClient, (WORD)len);
//			pLuckyAwardToClient += len;
//		}
//
//		len = pLuckyAwardToClient - p;
//		Set16(p, (WORD)len);
//		p += len;
//	}
//
//	len = p - buf.data();
//	SendToClient(Client, 24391, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
//}

void ResponseWeRelayCommonCfg(std::shared_ptr<ClientNode> Client)
{
	//const unsigned char* pMsg24391 = EHexToByte("0361000000510134B17D0134B1E20000000300000051B1BEC8FCBCBEBFAAB7C5CAB1BCE4CEAA32303233C4EA3035D4C23235C8D5D6C132303233C4EA3036D4C23236C8D5A3ACC8FCBCBEC4DAC3BFD6DCCEE5B5BDD6DCC8D531303A30302D32323A3030BFAAB7C50000002E3035D4C23235C8D5D6C13036D4C23236C8D52CC3BFD6DCCEE5B5BDD6DCC8D531303A30302D32323A3030BFAAB7C5006400000005000000010012000186A000035B60000186A000035B60001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000006400000006000000010012000186A000035B60000186A000035B60001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000006400000007000000010012000186A000035B60000186A000035B600012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000064000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000640000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000006400000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000064000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000000086000000640000000100000001019D0000000000000003007E000000000000000100000032006C00000000000000000000000000030019000078AE00000001000002D00000000000000000000000001900011A6600000001000002D00000000000000000000000001900011A6900000001000002D00000000000000000000000000000000000000000000000000000000000000000007E000000010000003300000096006C00000000000000000000000000030019000078AE00000001000001680000000000000000000000001900011A6700000001000002D00000000000000000000000001900011A6A00000001000002D00000000000000000000000000000000000000000000000000000000000000000009700000002000000970000012C00850000000000000000000000000004001900006CF300000001000002D00000000000000000000000001900006CED00000001000002D00000000000000000000000001900011A6800000001000002D00000000000000000000000001900011A6B00000001000002D000000000000000000000000000000000000000000000000000000000000000000000000500AC0000009D000F424000000001009E00000000000000000000000000050019000042D300000001000002D0000000000000000000000000190000802000000001000002D00000000000000000000000001900006C7100000001000002D00000000000000000000000001900006F9100000001000002D0000000000000000000000000190000761400000001000002D00000000000000000000000000000000000000000000000000000000000AC0000009E001E848000000001009E0000000000000000000000000005001900006E1000000001000002D0000000000000000000000000190000761400000001000002D0000000000000000000000000190000881200000001FFFFFFFF000000000000000000000000190000FE5D00000001000002D0000000000000000000000000190000F7D000000001000002D00000000000000000000000000000000000000000000000000000000000AC0000009F002DC6C000000001009E000000000000000000000000000500190000845800000001000002D000000000000000000000000019000100A100000001000002D0000000000000000000000000190000FD1200000001000002D000000000000000000000000019000075A000000001FFFFFFFF0000000000000000000000001900007E9F00000001000002D00000000000000000000000000000000000000000000000000000000000AC000000A0003D090000000001009E0000000000000000000000000005001900005E6E00000001000002D0000000000000000000000000190001033D00000001000002D00000000000000000000000001900005DB000000001FFFFFFFF0000000000000000000000001900005E8F00000001000002D0000000000000000000000000190000F88C00000001000002D00000000000000000000000000000000000000000000000000000000000AC000000A1004C4B4000000001009E000000000000000000000000000500190001035100000001000002D0000000000000000000000000190000838600000001000002D00000000000000000000000001900004E9D00000001FFFFFFFF000000000000000000000000190000865B00000001FFFFFFFF00000000000000000000000019000081EA00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000000000070022000000000000000400060000FD6F0006000081E900060001171F000600010E5700220000000300000004000600005B52000600005E52000600005A170006000032940034000000040000000700060000511A000600005129000600005257000600002B8B00060000558F000600005258000600004CA50046000000050000000A00060000565700060000614300060000614400060000566600060000586E000600008403000600010E5E00060000512A00060000367100060000694A0046000000060000000A000600006BC4000600006A6900060000682E000600002BAA000600003C5E000600002E5D000600002E5F000600003C47000600004951000600003C4A001C0000000700000003000600002724000600007074000600010A60002800000009000000050006000059E8000600005DB7000600003719000600004CA70006000042D00000000000A400000000009E00000000000000000000000000050019000067D500000001000000A80000000000000000000000001900004BC400000001000000000000000000000000000000001900004C7400000001000000A80000000000000000000000001900004D2C0000000100000000000000000000000000000000190000FE05000000010000000000000000000000000000000000000000000000000000000000000000A200");

	BYTE buf[8192];
	BYTE* p = buf;
	size_t len;

	Write16(p, 0);			 // ResultID
	Write32(p, Client->Uin); // Uin

	WriteHex(p, "03610000005F0134DA1E0134DA840000000300000051B1BEC8FCBCBEBFAAB7C5CAB1BCE4CEAA32303234C4EA3039D4C23236C8D5D6C132303234C4EA3130D4C23238C8D5A3ACC8FCBCBEC4DAC3BFD6DCCEE5B5BDD6DCC8D531303A30302D32323A3030BFAAB7C50000002E3039D4C23236C8D5D6C13130D4C23238C8D52CC3BFD6DCCEE5B5BDD6DCC8D531303A30302D32323A3030BFAAB7C5006400000005000000010012000186A000035B60000186A000035B60001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000006400000006000000010012000186A000035B60000186A000035B60001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000006400000007000000010012000186A000035B60000186A000035B600012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000064000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000640000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000001200000000000000000000000000000000006400000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000012000000000000000000000000000000000064000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000120000000000000000000000000000000000000086000000640000000100000001019D0000000000000003007E000000000000000100000032006C00000000000000000000000000030019000078AE00000001000002D00000000000000000000000001900011A6600000001000002D00000000000000000000000001900011A6900000001000002D00000000000000000000000000000000000000000000000000000000000000000007E000000010000003300000096006C00000000000000000000000000030019000078AE00000001000001680000000000000000000000001900011A6700000001000002D00000000000000000000000001900011A6A00000001000002D00000000000000000000000000000000000000000000000000000000000000000009700000002000000970000012C00850000000000000000000000000004001900006CF300000001000002D00000000000000000000000001900006CED00000001000002D00000000000000000000000001900011A6800000001000002D00000000000000000000000001900011A6B00000001000002D000000000000000000000000000000000000000000000000000000000000000000000000500AC0000009D000F424000000001009E00000000000000000000000000050019000042D300000001000002D0000000000000000000000000190000802000000001000002D00000000000000000000000001900006C7100000001000002D00000000000000000000000001900006F9100000001000002D0000000000000000000000000190000761400000001000002D00000000000000000000000000000000000000000000000000000000000AC0000009E001E848000000001009E0000000000000000000000000005001900006E1000000001000002D0000000000000000000000000190000761400000001000002D0000000000000000000000000190000881200000001FFFFFFFF000000000000000000000000190000FE5D00000001000002D0000000000000000000000000190000F7D000000001000002D00000000000000000000000000000000000000000000000000000000000AC0000009F002DC6C000000001009E000000000000000000000000000500190000845800000001000002D000000000000000000000000019000100A100000001000002D0000000000000000000000000190000FD1200000001000002D000000000000000000000000019000075A000000001FFFFFFFF0000000000000000000000001900007E9F00000001000002D00000000000000000000000000000000000000000000000000000000000AC000000A0003D090000000001009E0000000000000000000000000005001900005E6E00000001000002D0000000000000000000000000190001033D00000001000002D00000000000000000000000001900005DB000000001FFFFFFFF0000000000000000000000001900005E8F00000001000002D0000000000000000000000000190000F88C00000001000002D00000000000000000000000000000000000000000000000000000000000AC000000A1004C4B4000000001009E000000000000000000000000000500190001035100000001000002D0000000000000000000000000190000838600000001000002D00000000000000000000000001900004E9D00000001FFFFFFFF000000000000000000000000190000865B00000001FFFFFFFF00000000000000000000000019000081EA00000001FFFFFFFF00000000000000000000000000000000000000000000000000000000000000070022000000000000000400060000FD6F0006000081E900060001171F000600010E5700220000000300000004000600005B52000600005E52000600005A170006000032940034000000040000000700060000511A000600005129000600005257000600002B8B00060000558F000600005258000600004CA50046000000050000000A00060000565700060000614300060000614400060000566600060000586E000600008403000600010E5E00060000512A00060000367100060000694A0046000000060000000A000600006BC4000600006A6900060000682E000600002BAA000600003C5E000600002E5D000600002E5F000600003C47000600004951000600003C4A001C0000000700000003000600002724000600007074000600010A60002800000009000000050006000059E8000600005DB7000600003719000600004CA70006000042D00000000000A400000000009E00000000000000000000000000050019000067D500000001000000A80000000000000000000000001900004BC400000001000000000000000000000000000000001900004C7400000001000000A80000000000000000000000001900004D2C0000000100000000000000000000000000000000190000FE05000000010000000000000000000000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 24391, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;

	// memcpy(p, pMsg24391, 2648);
	// p += 2648;
	// len = p - buf;
	// SendToClient(Client, 24391, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	// return;
	{ // SeasonCfg
		BYTE* pSeasonCfg = p;
		Write16(pSeasonCfg, 0); // len

		Write32(pSeasonCfg, 34); // SeasonId
		Write32(pSeasonCfg, 0);	 // OpenDate
		Write32(pSeasonCfg, 0);	 // CloseDate
		Write32(pSeasonCfg, 7);	 // OpenDayNum
		Write32(pSeasonCfg, 0);	 // BroadCastContentLen
		Write32(pSeasonCfg, 0);	 // OpenTimeTipsLen

		const int MAX_SPEEDRALLY_OPEN_DAY_NUM = 7;
		for (size_t i = 0; i < MAX_SPEEDRALLY_OPEN_DAY_NUM; i++)
		{ // tDayOpenCfg
			BYTE* ptDayOpenCfg = pSeasonCfg;
			Write16(ptDayOpenCfg, 0); // len

			Write32(ptDayOpenCfg, i + 1); // DayID
			Write32(ptDayOpenCfg, 0);	  // OpenTimeNum
			/*
			for (size_t i = 0; i < MAX_OPEN_TIME_NUM; i++)
			{ //tOpenTimeCfg
				BYTE* ptOpenTimeCfg = ptDayOpenCfg;
				Write16(ptOpenTimeCfg, 0); //len

				Write32(ptOpenTimeCfg, 0); //OpenTime
				Write32(ptOpenTimeCfg, 0); //CloseTime
				Write32(ptOpenTimeCfg, 0); //TiyanOpenTime
				Write32(ptOpenTimeCfg, 0); //TiyanCloseTime

				len = ptOpenTimeCfg - ptDayOpenCfg;
				Set16(ptDayOpenCfg, (WORD)len);
				ptDayOpenCfg += len;
			}
			*/
			len = ptDayOpenCfg - pSeasonCfg;
			Set16(pSeasonCfg, (WORD)len);
			pSeasonCfg += len;
		}
		Write32(pSeasonCfg, 0); // LuckyBoxIndex
		Write32(pSeasonCfg, 0); // LuckyBoxPriority
		Write32(pSeasonCfg, 0); // LuckyBoxDayLimit

		len = pSeasonCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 1); // RankNum

	for (size_t i = 0; i < 1; i++)
	{ // RankAwardInfo
		BYTE* pRankAwardInfo = p;
		Write16(pRankAwardInfo, 0); // len

		Write32(pRankAwardInfo, 0); // RankType
		Write32(pRankAwardInfo, 0); // AwardNum
		for (size_t i = 0; i < 0; i++)
		{ // BaseRankAwardToClient
			BYTE* pBaseRankAwardToClient = pRankAwardInfo;
			Write16(pBaseRankAwardToClient, 0); // len

			Write32(pBaseRankAwardToClient, 0); // Index
			Write32(pBaseRankAwardToClient, 0); // RankBegin
			Write32(pBaseRankAwardToClient, 0); // RankEnd
			{									// Award
				BYTE* pAward = pBaseRankAwardToClient;
				Write16(pAward, 0); // len

				Write32(pAward, 0); // Exp
				Write32(pAward, 0); // Money
				Write32(pAward, 0); // SuperMoney
				Write16(pAward, 0); // ItemNum
				for (size_t i = 0; i < 0; i++)
				{ // ItemInfo
					BYTE* pItemInfo = pAward;
					Write16(pItemInfo, 0); // len

					Write32(pItemInfo, 0); // ItemID
					Write32(pItemInfo, 0); // ItemNum
					Write32(pItemInfo, 0); // AvailPeriod
					Write8(pItemInfo, 0);  // Status
					WriteUn(pItemInfo, 0); // ObtainTime
					Write32(pItemInfo, 0); // OtherAttribute
					Write16(pItemInfo, 0); // ItemType

					len = pItemInfo - pAward;
					Set16(pAward, (WORD)len);
					pAward += len;
				}
				Write32(pAward, 0); // Coupons
				Write32(pAward, 0); // GuildPoint
				Write32(pAward, 0); // LuckMoney
				Write8(pAward, 0);	// ExtendInfoNum
				for (size_t i = 0; i < 0; i++)
				{ // ExtendInfoAward
					BYTE* pExtendInfoAward = pAward;
					Write16(pExtendInfoAward, 0); // len

					Write16(pExtendInfoAward, 0); // Key
					Write32(pExtendInfoAward, 0); // AddValue
					Write32(pExtendInfoAward, 0); // TotalValue
					Write8(pExtendInfoAward, 0);  // ShowPriority
					Write16(pExtendInfoAward, 0); // BuyNeedScore
					Write8(pExtendInfoAward, 0);  // OneMatchMaxNum

					len = pExtendInfoAward - pAward;
					Set16(pAward, (WORD)len);
					pAward += len;
				}
				Write32(pAward, 0); // SpeedCoin

				len = pAward - pBaseRankAwardToClient;
				Set16(pBaseRankAwardToClient, (WORD)len);
				pBaseRankAwardToClient += len;
			}
			Write32(pBaseRankAwardToClient, 0); // Status

			len = pBaseRankAwardToClient - pRankAwardInfo;
			Set16(pRankAwardInfo, (WORD)len);
			pRankAwardInfo += len;
		}

		len = pRankAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); // ActiveAwardNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //ActiveAwardToClient
		BYTE* pActiveAwardToClient = p;
		Write16(pActiveAwardToClient, 0); //len

		Write32(pActiveAwardToClient, 0); //Index
		Write32(pActiveAwardToClient, 0); //ActiveValueLimit
		Write32(pActiveAwardToClient, 0); //RankLevLimit
		{ //Award
			BYTE* pAward = pActiveAwardToClient;
			Write16(pAward, 0); //len

			Write32(pAward, 0); //Exp
			Write32(pAward, 0); //Money
			Write32(pAward, 0); //SuperMoney
			Write16(pAward, 0); //ItemNum
			for (size_t i = 0; i < n; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAward;
				Set16(pAward, (WORD)len);
				pAward += len;
			}
			Write32(pAward, 0); //Coupons
			Write32(pAward, 0); //GuildPoint
			Write32(pAward, 0); //LuckMoney
			Write8(pAward, 0); //ExtendInfoNum
			for (size_t i = 0; i < n; i++)
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

			len = pAward - pActiveAwardToClient;
			Set16(pActiveAwardToClient, (WORD)len);
			pActiveAwardToClient += len;
		}

		len = pActiveAwardToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write32(p, 0); // CarTypeNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //CarGuide
		BYTE* pCarGuide = p;
		Write16(pCarGuide, 0); //len

		Write32(pCarGuide, 0); //CarType
		Write32(pCarGuide, 0); //CarNum
		for (size_t i = 0; i < n; i++)
		{ //GuideCarInfo
			BYTE* pGuideCarInfo = pCarGuide;
			Write16(pGuideCarInfo, 0); //len

			Write32(pGuideCarInfo, 0); //CarID

			len = pGuideCarInfo - pCarGuide;
			Set16(pCarGuide, (WORD)len);
			pCarGuide += len;
		}

		len = pCarGuide - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write32(p, 0); // TiyanCarTypeNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //TiyanCarGuide
		BYTE* pTiyanCarGuide = p;
		Write16(pTiyanCarGuide, 0); //len

		Write32(pTiyanCarGuide, 0); //CarType
		Write32(pTiyanCarGuide, 0); //CarNum
		for (size_t i = 0; i < n; i++)
		{ //GuideCarInfo
			BYTE* pGuideCarInfo = pTiyanCarGuide;
			Write16(pGuideCarInfo, 0); //len

			Write32(pGuideCarInfo, 0); //CarID

			len = pGuideCarInfo - pTiyanCarGuide;
			Set16(pTiyanCarGuide, (WORD)len);
			pTiyanCarGuide += len;
		}

		len = pTiyanCarGuide - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	{ // LuckyAwardToClient
		BYTE* pLuckyAwardToClient = p;
		Write16(pLuckyAwardToClient, 0); // len

		Write32(pLuckyAwardToClient, 0); // LuckBoxGet
		{								 // Award
			BYTE* pAward = pLuckyAwardToClient;
			Write16(pAward, 0); // len

			Write32(pAward, 0); // Exp
			Write32(pAward, 0); // Money
			Write32(pAward, 0); // SuperMoney
			Write16(pAward, 0); // ItemNum
			/*
			for (size_t i = 0; i < n; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				Write32(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAward;
				Set16(pAward, (WORD)len);
				pAward += len;
			}
			*/
			Write32(pAward, 0); // Coupons
			Write32(pAward, 0); // GuildPoint
			Write32(pAward, 0); // LuckMoney
			Write8(pAward, 0);	// ExtendInfoNum
			/*
			for (size_t i = 0; i < n; i++)
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
			*/
			Write32(pAward, 0); // SpeedCoin

			len = pAward - pLuckyAwardToClient;
			Set16(pLuckyAwardToClient, (WORD)len);
			pLuckyAwardToClient += len;
		}

		len = pLuckyAwardToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24391, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseDoBagItemShowOperate(std::shared_ptr<ClientNode> Client, USHORT OptType, USHORT ShowType, USHORT TabType, UINT ItemID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	int TabItemTopNum = 0;
	int StarItemNum = 0;
	if (OptType == 0x7 || OptType == 0x1 && ItemID != 0)//置顶或星标
	{
		PlayerDB_UpdateItemOtherAttribute(Client->Uin, ItemID, 1);
	}
	else if (OptType == 0x2 && ItemID != 0)//取消置顶
	{
		PlayerDB_UpdateItemOtherAttribute(Client->Uin, ItemID, 0);
	}

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	std::vector<int> resultRow = executeQueryCol("SELECT ItemID FROM Item WHERE Uin = ? and OtherAttribute = 1;", "ResponseDoBagItemShowOperate", conditionValues);

	for (int itemId : resultRow) {

		if (TabItemTopNum < 24)//背包置顶上限
		{
			TabItemTopNum++;
		}
		StarItemNum++;//星标无上限
	}


	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write16(p, OptType); //OptType
	Write16(p, ShowType); //ShowType
	Write16(p, TabType); //TabType
	Write32(p, 1); //HasBagItemShowInfo
	//BagItemShowInfo
	BYTE* pBagItemShowInfo = p;
	Write16(pBagItemShowInfo, 0); //len
	Write32(pBagItemShowInfo, 6); //TabNum 6个页 赛车-宝石
	for (int i = 0; i < 6; i++)
	{
		BYTE* pBagTabItemTopInfo = pBagItemShowInfo;
		Write16(pBagTabItemTopInfo, 0); //len

		Write32(pBagTabItemTopInfo, TabItemTopNum); //ItemNum

		for (int j = 0; j < TabItemTopNum; j++)
		{
			Write32(pBagTabItemTopInfo, resultRow[j]);
		}

		len = pBagTabItemTopInfo - pBagItemShowInfo;
		Set16(pBagItemShowInfo, (WORD)len);
		pBagItemShowInfo += len;
	}
	Write32(pBagItemShowInfo, StarItemNum); //StarItemNum 星标数量
	for (int j = 0; j < StarItemNum; j++)
	{
		Write32(pBagItemShowInfo, resultRow[j]);
	}
	Write32(pBagItemShowInfo, 0); //TabTopItemNumMax
	Write32(pBagItemShowInfo, 0); //StarItemNumMax

	len = pBagItemShowInfo - p;
	Set16(p, (WORD)len);
	p += len;

	Write32(p, 0); //Para0
	Write32(p, 0); //ItemNum
	//Write32(p, 0); //ItemID[]

	len = p - buf.data();
	SendToClient(Client, 28412, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

void RequestDoBagItemShowOperate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT OptType = Read16(p);//0x0 打开背包 0x7置顶 0x2取消置顶 0x1 星标
	USHORT ShowType = Read16(p);
	USHORT TabType = Read16(p);
	int ItemID = Read32(p);
	//int ItemNum = Read32(p);

	ResponseDoBagItemShowOperate(Client, OptType, ShowType, TabType, ItemID);

}

void ResponseGetQuickRaceTimeRandMap(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, 0); //Time
	Write8(p, 3); //QuickRaceRandMapNum
	srand(time(0));
	for (char i = 0; i < 3; i++)
	{
		BYTE* QuickRaceRandMapNums = p;
		Write16(QuickRaceRandMapNums, 0); //len
		Write32(QuickRaceRandMapNums, GetRandMap(0)); //QuickRaceRandMap[]
		Write8(QuickRaceRandMapNums, 0); //NetTag
		len = QuickRaceRandMapNums - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 9); //QuickRaceRandMapRefreshTime

	Write16(p, 0); //SeasonMapNums

	//WriteHex(p, "0538000700940014000E000000B70000009E000000A2000000EE000000FD00000100000001040000010C000001110000014A000001A4000000FF00000091000001050011000000890000009E000000A2000000BB000000BE000000EE000000FD000000FF0000010000000104000001050000010B0000011100000113000001160000012B0000014A000300000111000000EE0000010500000094001500110000009E000000A2000000EE000000FD0000010000000104000001050000010B0000010C000001110000013F0000014A0000017A00000143000001A4000000FF00000091000E0000009E000000A2000000BB000000EE000000FD000000FF0000010000000104000001050000010B00000111000001160000012B0000014A000300000111000000EE00000105000000E800160021000000A2000000EE000000FD0000010000000104000001050000010B0000010C000001160000012B0000013F0000014400000145000001480000014A000001AE0000017A00000180000001850000018B0000019000000176000001960000019F00000143000001A4000001A5000001A8000001AB0000018E000001C2000000BB000001B30011000000890000009E000000A2000000BB000000BE000000EE000000FD0000010000000104000001050000010B0000011100000113000001160000012B0000014A0000013F000400000105000000BB00000185000001B30001000001C200FC00170028000000A2000000EE0000010000000104000001050000010B000001160000012B0000013F000001440000014500000148000001AE0000017800000180000001850000018A0000018D0000018B000001900000019400000176000001960000019F000000F600000143000001A5000001A6000001A8000001AB000001B40000018E000001B6000001B7000001B9000001BE000001BF000001C2000000BB000001B3001000000089000000A2000000BB000000EE00000100000001040000010B000001130000013F000000A70000010A0000011B000000BD0000013300000148000000FE0003000000BB00000185000001B30001000001C200BC00180028000000A2000000B9000000BD000000FE00000105000001160000012B0000014400000148000001AE000001AD0000017800000180000001850000018A0000018C0000018D0000018F0000018B0000010600000190000001940000019F000000F600000143000001A50000011B000001A6000001A8000001AB000001B40000018E000001B6000001B7000001B9000001BE000001BF000001C1000000BB000001B300000003000000BB00000185000001B30001000001C100C0001900280000007A0000007B00000085000000A7000000B5000000B9000000BD00000189000000C2000000D4000000DE000000E7000000EF000000FE0000010A00000149000001530000015A000001AD000001780000018A0000018C0000018D0000018F0000010600000114000000C7000000840000019F0000011B000001A6000001B4000001B6000001B7000001B9000001BE000001BF000001C0000001C1000001B300000004000001B300000114000000DE000000C20001000001C100AC001A00250000007A0000007B0000008500000089000000A7000000B5000000B9000000BD00000189000000C2000000D4000000DE000000E7000000EF000000FE0000010A00000124000001490000014C000001530000015A000001AD000001780000018A0000018C0000018D0000018F000001060000011400000099000000C7000000840000011B000001B9000001BE000001BF000001C00000000300000114000000DE000000C20000");

	for (size_t i = 0; i < 0; i++)
	{ //MapInfos
		BYTE* pMapInfos = p;
		Write16(pMapInfos, 0); //len

		Write16(pMapInfos, 0); //GradeMapNums
		for (size_t i = 0; i < 0; i++)
		{ //MapInfo
			BYTE* pMapInfo = pMapInfos;
			Write16(pMapInfo, 0); //len

			Write16(pMapInfo, 0); //GradeLevel
			Write16(pMapInfo, 0); //MapNums
			Write32(pMapInfo, 0); //MapArray[]
			Write16(pMapInfo, 0); //SuperRaceMapNums
			Write32(pMapInfo, 0); //SuperRaceMapArray[]
			Write16(pMapInfo, 0); //CarFactoryMapNums
			Write32(pMapInfo, 0); //CarFactoryMapArray[]
			Write16(pMapInfo, 0); //NewMapNums
			Write32(pMapInfo, 0); //NewMapArray[]

			len = pMapInfo - pMapInfos;
			Set16(pMapInfos, (WORD)len);
			pMapInfos += len;
		}

		len = pMapInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 28379, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}
void NotifyBeAddedAsFriend(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> SrcClient)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, SrcClient->Uin); //SrcUin
	Write32(p, Client->Uin); //DstUin
	{ //SrcPlayerInfo
		BYTE* pSrcPlayerInfo = p;
		Write16(pSrcPlayerInfo, 0); //len

		Write8(pSrcPlayerInfo, 0); //Gender
		Write8(pSrcPlayerInfo, 0); //Country
		memset(pSrcPlayerInfo, 0, 17);

		memcpy(pSrcPlayerInfo, SrcClient->NickName, 17);

		pSrcPlayerInfo += 17;
		len = pSrcPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //WordLength
	Write8(p, 0); //Type
	Write16(p, 0); //Src

	len = p - buf.data();
	SendToClient(Client, 507, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestAddFriend(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT DstUin = Read32(p);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyBeAddedAsFriend(DstClient, Client);
	}
	//UINT WordLength = Read32(p);
	//UCHAR Type = Read8(p);
	//USHORT Src = Read16(p);

}

void NotifyAddFriendResult(std::shared_ptr<ClientNode> Client, USHORT result, std::shared_ptr<ClientNode> SrcClient)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	if (result == 0)
	{
		BOOL haveFriend = PlayerDB_IsHasFriend(Client->Uin, SrcClient->Uin);

		if (!haveFriend)
		{
			PlayerDB_InsertFriend(Client->Uin, SrcClient->Uin);
		}
	}

	Write16(p, result); //ResultID
	Write32(p, Client->Uin); //SrcUin
	Write32(p, SrcClient->Uin); //DstUin
	Write16(p, 0); //WordLength
	if (result == 0)
	{
		Write8(p, 8); //ReasonLen
		WriteString(p, "添加成功");
	}
	else
	{
		Write8(p, 16); //ReasonLen
		WriteString(p, "对方拒绝好友申请");
	}
	Write8(p, 0); //Type
	Write16(p, 0); //Src
	len = p - buf.data();
	SendToClient(Client, 508, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);

	if (result == 0)
	{
		ResponseGetGroupFriendBaseInfo(Client, SrcClient->Uin, SrcClient->NickName);
	}


}
void AckBeAddedAsFriend(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	short ResultID = Read16(p);//0 同意 1不同意
	UINT SrcUin = Read32(p);
	UINT DstUin = Read32(p);
	//short WordLength = Read16(p);
	//UCHAR Type = Read8(p);
	//USHORT Src = Read16(p);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyAddFriendResult(DstClient, ResultID, Client);
		NotifyAddFriendResult(Client, ResultID, DstClient);
	}


}


void NotifyOtherSkillStoneTakeEffect(std::shared_ptr<ClientNode> Client, UINT SrcUin, UINT StoneID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, SrcUin); //Uin
	Write32(p, StoneID); //StoneID

	len = p - buf.data();
	SendToClient(Client, 910, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestReportSkillStoneTakeEffect(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int StoneID = Read32(p);
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
		if (RoomClient && RoomClient != Client)
		{
			NotifyOtherSkillStoneTakeEffect(RoomClient, Uin, StoneID);
		}
	}
}
void NotifyBeEngaged(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> SrcClient, UINT RingId)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, SrcClient->Uin); //SrcUin
	Write32(p, Client->Uin); //DstUin
	Write32(p, 0); //EngageType
	{ //SrcPlayerInfo
		BYTE* pSrcPlayerInfo = p;
		Write16(pSrcPlayerInfo, 0); //len

		Write8(pSrcPlayerInfo, 0); //Gender
		Write8(pSrcPlayerInfo, 0); //Country
		memset(pSrcPlayerInfo, 0, 17);

		memcpy(pSrcPlayerInfo, SrcClient->NickName, 17);

		pSrcPlayerInfo += 17;
		len = pSrcPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //WordLength
	Write32(p, RingId); //RingId

	len = p - buf.data();
	SendToClient(Client, 572, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestEngage(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT SrcUin = Read32(p);
	UINT Time = Read32(p);
	int EngageType = Read32(p);
	int RingID = Read32(p);
	UINT DstUin = Read32(p);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyBeEngaged(DstClient, Client, RingID);
	}

	//char[] NickName[] = ReadString(p);
	//short WordLength = Read16(p);

}

void NotifyEngageResult(std::shared_ptr<ClientNode> Client, USHORT resultID, std::shared_ptr<ClientNode> SrcClient)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	
	Write16(p, resultID); //ResultID
	Write32(p, Client->Uin); //Uin
	Write32(p, SrcClient->Uin); //DstUin

	short namelen = strlen(SrcClient->NickName);
	Write8(p, namelen);
	memcpy(p, SrcClient->NickName, namelen);
	p += namelen;
	Write32(p, 0); //EngageType
	Write16(p, 0); //WordLength
	Write8(p, 0); //ReasonLen
	Write32(p, 0); //RingId
	Write32(p, 0); //EngageTime
	Write8(p, 0); //EngageFlag

	len = p - buf.data();
	SendToClient(Client, 573, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);


	if (resultID == 0)
	{

		// 先查询是否有
		if (Client->EngageUin != 0 && Client->EngageUin != SrcClient->Uin)
		{//存在情侣且不同时

			PlayerDB_UpdateEngageInfo(Client->Uin, SrcClient->Uin);
		}
		else if (Client->EngageUin == 0)//不存在情侣
		{

			PlayerDB_InsertEngageInfo(Client->Uin, SrcClient->Uin);
			Client->EngageUin = SrcClient->Uin;
			NotifyClientLoveBabyInfoWhenLogIn(Client);
		}
	}
}
void AckBeEngaged(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	short ResultID = Read16(p);
	UINT SrcUin = Read32(p);
	UINT DstUin = Read32(p);
	//int EngageType = Read32(p);
	//short WordLength = Read16(p);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyEngageResult(DstClient, ResultID, Client);
		NotifyEngageResult(Client, ResultID, DstClient);
	}
}


void ResponseCancelEngage(std::shared_ptr<ClientNode> Client, UINT DstUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write32(p, DstUin); //DstUin
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 167, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyCancelEngage(std::shared_ptr<ClientNode> Client, UINT SrcUin, std::shared_ptr<ClientNode> DstClient, int EngageType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, SrcUin); //SrcUin
	memcpy(p, DstClient->NickName, 17); //NickName[]
	p += 17;
	Write32(p, DstClient->Uin); //DstUin
	Write32(p, EngageType); //EngageType
	Write16(p, 0); //WordLength

	len = p - buf.data();
	SendToClient(Client, 574, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestCancelEngage(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT DstUin = Read32(p);
	int EngageType = Read32(p);
	short WordLength = Read16(p);
	int UseItemId = Read32(p);
	ResponseCancelEngage(Client, DstUin);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyCancelEngage(DstClient, DstUin, Client, EngageType);
	}

	PlayerDB_DeleteEngageInfo(Uin, DstUin);
}

void RequestAddSuperMoney(std::shared_ptr<ClientNode> Client, UINT ItemID, UINT count)
{

	ItemInfo Items = { 0 };
	int ItemNum = 1500;//点卷数量
	Items.ItemID = 100083;
	Items.ItemNum = ItemNum;
	Items.ItemType = 0;
	Items.AvailPeriod = 0;
	Items.ObtainTime = 0;

	SelectClientExp(Client);//读取货币 

	Client->SuperMoney += Items.ItemNum;

	UpdateClientBase(Client); //更新点卷 
	NotifyClientUpdateMoneyExp(Client); // 通知客户端更新点卷
	virtualsend2(Client, { Items }, 1, ItemID);
}

void RequestAddCharm(std::shared_ptr<ClientNode> Client, UINT ItemID, UINT count)
{

	int ItemNum = 0;

	char Content[64] = { 0 };

	ItemNum = PlayerDB_GetItemNum(Client->Uin, 62787);

	if (ItemNum <= 0)
	{
		return;
	}
	int RandValue = 0;
	srand((unsigned)time(0) + GetSysTimeMicros());  // 产生随机种子
	int temp = ItemNum;
	while (temp > 0)
	{
		RandValue += (UINT)(30);
		temp--;
	}
	SelectClientExp(Client);
	Client->Charm += RandValue;


	NotifyUseItemResult2(Client, ItemID, ItemNum);

	UpdateClientBase(Client);

	sprintf_s(Content, "\r\n\n\n/C01魅力值总增加 %d 点\r\n\n\n\n\n", RandValue);
	ResponseStartGame(Client, Content);

}

void NotifyBeInvited(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> SrcClient)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, SrcClient->Uin); // SrcUin
	memset(p, 0, 17);
	memcpy(p, SrcClient->NickName, MaxNickName);
	p += MaxNickName;
	Write32(p, Client->Uin); // DstUin
	{					   // PositionInfo
		BYTE* pPositionInfo = p;
		Write16(pPositionInfo, 0); // len

		Write8(pPositionInfo, 0);				// ServerType
		Write16(pPositionInfo, 0);				// Status
		Write32(pPositionInfo, SrcClient->ServerID); // ServerID
		{										// RoomInfo
			BYTE* pRoomInfo = pPositionInfo;
			Write16(pRoomInfo, 0); // len

			Write16(pRoomInfo, SrcClient->RoomID);			   // RoomID
			Write16(pRoomInfo, 0);	   // BaseGameMode
			Write16(pRoomInfo, 0);	   // SubGameMode
			Write8(pRoomInfo, 0); // CurrentPlayerNum
			Write8(pRoomInfo, 0);		   // TotalSeatNum
			Write8(pRoomInfo, 0);	   // Status
			Write32(pRoomInfo, 0);		   // MapID
			Write16(pRoomInfo, 0);			   // Flag
			len = pRoomInfo - pPositionInfo;
			Set16(pPositionInfo, (WORD)len);
			pPositionInfo += len;
		}
		len = pPositionInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 569, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestInvitePlayer(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT DstUin = Read32(p);
	//bool Type = Read8(p);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyBeInvited(DstClient, Client);
	}

}

void ResponseC2SearchKartAltasMoreInfo(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Index, std::vector<KartRefitItemInfo> kartRefitItemInfo)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //DstUin

	BYTE* pItemNum = p;
	int ItemNum = 0;
	bool HasMoreInfo = false;

	Write16(p, 0); //ItemNum
	for (size_t i = 0; i < kartRefitItemInfo.size(); i++)
	{ //ItemInfoArray

		if (i + Index >= kartRefitItemInfo.size()) {
			break;
		}

		if (i < 200) {
			ItemNum++;
			BYTE* pItemInfoArray = p;
			Write16(pItemInfoArray, 0); //len

			Write32(pItemInfoArray, kartRefitItemInfo[i + Index].KartId); //ItemID
			Write32(pItemInfoArray, 0); //RefitCout
			{ //AltasCurKartAttr
				BYTE* pAltasCurKartAttr = pItemInfoArray;
				Write16(pAltasCurKartAttr, 0); //len

				Write32(pAltasCurKartAttr, Uin); //Uin

				Write32(pAltasCurKartAttr, kartRefitItemInfo[i + Index].KartId); //KartId
				Write32(pAltasCurKartAttr, 0); //RefitCout
				Write16(pAltasCurKartAttr, 0); //MaxFlags
				Write16(pAltasCurKartAttr, 0); //WWeight
				Write16(pAltasCurKartAttr, 0); //SpeedWeight
				Write16(pAltasCurKartAttr, 0); //JetWeight
				Write16(pAltasCurKartAttr, 0); //SJetWeight
				Write16(pAltasCurKartAttr, 0); //AccuWeight
				Write32(pAltasCurKartAttr, 0); //ShapeRefitCount
				Write32(pAltasCurKartAttr, 0); //KartHeadRefitItemID
				Write32(pAltasCurKartAttr, 0); //KartTailRefitItemID
				Write32(pAltasCurKartAttr, 0); //KartFlankRefitItemID
				Write32(pAltasCurKartAttr, 0); //KartTireRefitItemID
				{ //KartRefitExInfo
					BYTE* pKartRefitExInfo = pAltasCurKartAttr;
					Write16(pKartRefitExInfo, 0); //len

					Write8(pKartRefitExInfo, 0); //SpeedRefitStar
					Write8(pKartRefitExInfo, 0); //JetRefitStar
					Write8(pKartRefitExInfo, 0); //SJetRefitStar
					Write8(pKartRefitExInfo, 0); //AccuRefitStar
					Write8(pKartRefitExInfo, 0); //SpeedAddRatio
					Write8(pKartRefitExInfo, 0); //JetAddRatio
					Write8(pKartRefitExInfo, 0); //SJetAddRatio
					Write8(pKartRefitExInfo, 0); //AccuAddRatio

					len = pKartRefitExInfo - pAltasCurKartAttr;
					Set16(pAltasCurKartAttr, (WORD)len);
					pAltasCurKartAttr += len;
				}
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i + Index].SecondRefitCount); //SecondRefitCount
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i + Index].Speed2Weight); //Speed2Weight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i + Index].DriftVecWeight); //DriftVecWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i + Index].AdditionalZSpeedWeight); //AdditionalZSpeedWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i + Index].AntiCollisionWeight); //AntiCollisionWeight
				Write16(pAltasCurKartAttr, 0); //LuckyValue
				Write16(pAltasCurKartAttr, 0); //RefitLuckyValueMaxWeight
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i + Index].ShapeSuitID); //ShapeSuitID
				Write8(pAltasCurKartAttr, kartRefitItemInfo[i + Index].LegendSuitLevel); //LegendSuitLevel
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i + Index].LegendSuitLevelChoice); //LegendSuitLevelChoice
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i + Index].ShapeLegendSuitID); //ShapeLegendSuitID

				len = pAltasCurKartAttr - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}
			{ //AltasKartStoneGrooveInfo
				BYTE* pAltasKartStoneGrooveInfo = pItemInfoArray;
				Write16(pAltasKartStoneGrooveInfo, 0); //len

				Write32(pAltasKartStoneGrooveInfo, 0); //KartID
				Write32(pAltasKartStoneGrooveInfo, 0); //StoneGrooveNum
				for (size_t j = 0; j < 0; j++)
				{ //StoneGrooveInfo
					BYTE* pStoneGrooveInfo = pAltasKartStoneGrooveInfo;
					Write16(pStoneGrooveInfo, 0); //len

					Write32(pStoneGrooveInfo, 0); //StoneUseOccaType
					Write32(pStoneGrooveInfo, 0); //SkillStoneID

					len = pStoneGrooveInfo - pAltasKartStoneGrooveInfo;
					Set16(pAltasKartStoneGrooveInfo, (WORD)len);
					pAltasKartStoneGrooveInfo += len;
				}

				len = pAltasKartStoneGrooveInfo - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}
			{ //AltasKartActiveLevelInfo
				BYTE* pAltasKartActiveLevelInfo = pItemInfoArray;
				Write16(pAltasKartActiveLevelInfo, 0); //len

				Write32(pAltasKartActiveLevelInfo, 0); //KartType
				Write32(pAltasKartActiveLevelInfo, 0); //ItemID
				Write32(pAltasKartActiveLevelInfo, 0); //ActiveLevel

				len = pAltasKartActiveLevelInfo - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}

			len = pItemInfoArray - p;
			Set16(p, (WORD)len);
			p += len;
		}
		else
		{
			HasMoreInfo = true;
			break;
		}

	}

	Set16(pItemNum, (WORD)ItemNum);
	Write8(p, HasMoreInfo); //HasNext
	Index += ItemNum;

	len = p - buf.data();
	SendToClient(Client, 13703, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	if (HasMoreInfo)
	{
		ResponseC2SearchKartAltasMoreInfo(Client, Uin, Index, kartRefitItemInfo);
	}
}

void ResponseC2SearchKartAltas(std::shared_ptr<ClientNode> Client, UINT DstUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, 0); //Result
	Write32(p, DstUin); //DstUin

	BYTE* pItemNum = p;
	Write16(p, 0); //ItemNum

	std::vector<KartRefitItemInfo> kartRefitItemInfo;
	SelectKartRefitItemInfo(DstUin, kartRefitItemInfo);
	int ItemNum = 0;
	bool HasMoreInfo = false;

	for (size_t i = 0; i < kartRefitItemInfo.size(); i++)
	{
		if (i < 200) {
			ItemNum++;
			BYTE* pItemInfoArray = p;
			Write16(pItemInfoArray, 0); //len

			Write32(pItemInfoArray, kartRefitItemInfo[i].KartId); //ItemID
			Write32(pItemInfoArray, 0); //RefitCout
			{ //AltasCurKartAttr
				BYTE* pAltasCurKartAttr = pItemInfoArray;
				Write16(pAltasCurKartAttr, 0); //len

				Write32(pAltasCurKartAttr, DstUin); //Uin
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].KartId); //KartId
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].RefitCout); //RefitCout
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].MaxFlags); //MaxFlags
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].WWeight); //WWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].SpeedWeight); //SpeedWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].JetWeight); //JetWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].SJetWeight); //SJetWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].AccuWeight); //AccuWeight
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].ShapeRefitCount); //ShapeRefitCount
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].KartHeadRefitItemID); //KartHeadRefitItemID
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].KartTailRefitItemID); //KartTailRefitItemID
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].KartFlankRefitItemID); //KartFlankRefitItemID
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].KartTireRefitItemID); //KartTireRefitItemID
				{ //KartRefitExInfo
					BYTE* pKartRefitExInfo = pAltasCurKartAttr;
					Write16(pKartRefitExInfo, 0); //len

					Write8(pKartRefitExInfo, 0); //SpeedRefitStar
					Write8(pKartRefitExInfo, 0); //JetRefitStar
					Write8(pKartRefitExInfo, 0); //SJetRefitStar
					Write8(pKartRefitExInfo, 0); //AccuRefitStar
					Write8(pKartRefitExInfo, 0); //SpeedAddRatio
					Write8(pKartRefitExInfo, 0); //JetAddRatio
					Write8(pKartRefitExInfo, 0); //SJetAddRatio
					Write8(pKartRefitExInfo, 0); //AccuAddRatio

					len = pKartRefitExInfo - pAltasCurKartAttr;
					Set16(pAltasCurKartAttr, (WORD)len);
					pAltasCurKartAttr += len;
				}
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].SecondRefitCount); //SecondRefitCount
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].Speed2Weight); //Speed2Weight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].DriftVecWeight); //DriftVecWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].AdditionalZSpeedWeight); //AdditionalZSpeedWeight
				Write16(pAltasCurKartAttr, kartRefitItemInfo[i].AntiCollisionWeight); //AntiCollisionWeight
				Write16(pAltasCurKartAttr, 0); //LuckyValue
				Write16(pAltasCurKartAttr, 0); //RefitLuckyValueMaxWeight
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].ShapeSuitID); //ShapeSuitID
				Write8(pAltasCurKartAttr, kartRefitItemInfo[i].LegendSuitLevel); //LegendSuitLevel
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].LegendSuitLevelChoice); //LegendSuitLevelChoice
				Write32(pAltasCurKartAttr, kartRefitItemInfo[i].ShapeLegendSuitID); //ShapeLegendSuitID

				len = pAltasCurKartAttr - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}
			{ //AltasKartStoneGrooveInfo
				BYTE* pAltasKartStoneGrooveInfo = pItemInfoArray;
				Write16(pAltasKartStoneGrooveInfo, 0); //len

				Write32(pAltasKartStoneGrooveInfo, 0); //KartID
				Write32(pAltasKartStoneGrooveInfo, 0); //StoneGrooveNum

				for (size_t j = 0; j < 0; j++)
				{ //StoneGrooveInfo
					BYTE* pStoneGrooveInfo = pAltasKartStoneGrooveInfo;
					Write16(pStoneGrooveInfo, 0); //len

					Write32(pStoneGrooveInfo, 0); //StoneUseOccaType
					Write32(pStoneGrooveInfo, 0x55cd); //SkillStoneID

					len = pStoneGrooveInfo - pAltasKartStoneGrooveInfo;
					Set16(pAltasKartStoneGrooveInfo, (WORD)len);
					pAltasKartStoneGrooveInfo += len;
				}

				len = pAltasKartStoneGrooveInfo - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}
			{ //AltasKartActiveLevelInfo
				BYTE* pAltasKartActiveLevelInfo = pItemInfoArray;
				Write16(pAltasKartActiveLevelInfo, 0); //len

				Write32(pAltasKartActiveLevelInfo, 0); //KartType
				Write32(pAltasKartActiveLevelInfo, 0); //ItemID
				Write32(pAltasKartActiveLevelInfo, 0); //ActiveLevel

				len = pAltasKartActiveLevelInfo - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}

			len = pItemInfoArray - p;
			Set16(p, (WORD)len);
			p += len;

		}
		else {
			HasMoreInfo = true;
			break;
		}

	}
	Set16(pItemNum, (WORD)ItemNum);
	Write8(p, HasMoreInfo); //HasMoreInfo

	len = p - buf.data();
	SendToClient(Client, 13705, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	if (HasMoreInfo)
	{
		ResponseC2SearchKartAltasMoreInfo(Client, DstUin, ItemNum, kartRefitItemInfo);
	}
}
void RequestC2SSearchKartAltas(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT DstUin = Read32(p);
	ResponseC2SearchKartAltas(Client, DstUin);
}

void RequestClientVerify(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();  size_t len;
	{ //Head
		BYTE* pHead = p;
		Write16(pHead, 0); //len

		Write32(pHead, Client->Uin); //Uin
		Write32(pHead, (DWORD)time(nullptr)); //Time
		Write8(pHead, 0); //Version
		Write8(pHead, 0); //Compress
		Write8(pHead, 0); //OptLength

		len = pHead - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		Write16(pBody, 31); //MsgLen
		memcpy(pBody, "OnNotifyServerVerify", 21);
		pBody += 22;

		Write8(pBody, 246);

		double intValue = Client->ServeryVerifyKey;
		std::memcpy(pBody, &intValue, sizeof(double));
		pBody += sizeof(double);

		Write16(pBody, 0);


		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void MyResponseQuickRPC(std::shared_ptr<ClientNode> Client, BYTE* Msg, size_t MsgLen) {
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	{ //Head
		BYTE* pHead = p;
		Write16(pHead, 0); //len

		Write32(pHead, Client->Uin); //Uin
		Write32(pHead, (DWORD)time(nullptr)); //Time
		Write8(pHead, 0); //Version
		Write8(pHead, 0); //Compress
		Write8(pHead, 0); //OptLength

		len = pHead - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		Write16(pBody, MsgLen); //MsgLen
		memcpy(pBody, Msg, MsgLen);
		pBody += MsgLen;

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestQuickRPC(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//PrintfBody(Body, BodyLen);
	size_t len;
	// Head
	len = Read16(Body);
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	BYTE Version = Read8(Body);
	BYTE Compress = Read8(Body);
	BYTE OptLength = Read8(Body);
	// Body
	len = Read16(Body);
	USHORT MsgLen = Read16(Body);
	BYTE* msg = Body;
	std::string cmd((char*)msg);


	if (cmd.compare(0, 39, "Co.RequestGetCarTreasureNoSelfSelectCfg") == 0) {
		//夺宝
		RequestGetCarTreasureNoSelfSelectCfg(Client, Body);
		return;
	}
	else if (cmd.compare(0, 41, "Co.RequestGetCarTreasureNoSelfSelectAward") == 0) {
		//夺宝
		RequestGetCarTreasureNoSelfSelectAward(Client, Body);
		return;
	}


	double ping;
	{ // resolve msg
		int i = 0;
		for (; i < MsgLen - 1; i++) {
			if (msg[i] == 0 && msg[i + 1] == 0) {
				break;
			}
		}
		// assert find i
		i += 2; // 0x0000
		i += 1; // 0xF6
		ping = z_get<double>(msg + i);
	}
	double pong;
	pong = ping;

	BYTE* response = NULL;
	std::string resp = "";

	if (cmd == "Co.CheJian_QueryOrderInfo") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000804540F60000000000000000FD00000000";
	}
	else if (cmd == "Co.Beta83_ClientVerify")
	{
		RequestClientVerify(Client);
		return;
	}
	else if (cmd == "Co.Beta87_GetLoginCfg")
	{
		resp = "4F6E526571436865636B55736572446174610000F60000000000000000FD0000001CFA026964F600000000009EC340FA0474797065F6000000000000F03F";
		Request_Beta87_GetLoginCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.LimitRecommend_OpenMainDlg")
	{
		resp = "4469737061746368436F526573706F6E73650000F60000000000000040F60000000000000000FD000000CFFA0F546F704C6973744F7074696D697A65F6000000000000F03FFA0F456E61626C654173796E634C6F6164F60000000000000000FA105573654F72695472616E734C6F676963F6000000000000F03FFA0D497355726C576974685369676EF60000000000000000FA1349734E65775265636F7264536E617073686F74F60000000000000000FA0B5472616E73416C6C544350F6000000000000F03FFA124E657450657273656E745065724672616D65F60000000000408F40FA0C57696E37456E61626C653634F6000000000000F03F";
		Request_LimitRecommend_OpenMainDlg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_Dragon_City_MainDlg") {
		//封神演义
		Request_Beta87_Dragon_City_MainDlg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_Dragon_City_AwardOpt") {
		//封神演义奖励
		Request_Beta87_Dragon_City_AwardOpt(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta78_ReqMechSkinInfo") {
		//机甲回复包
		Request_Beta78_ReqMechSkinInfo(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_ReqSelfHeadAndHeadShokInfo") {
		//获取当前头像
		Request_Beta87_ReqSelfHeadAndHeadShokInfo(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_ReqBatchHeadAndHeadShokInfo") {
		//多个头像
		Request_Beta87_ReqBatchHeadAndHeadShokInfo(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_ReqHeadAndHeadShokAvatarCfg") {
		//头像配置
		Beta87_ReqHeadAndHeadShokAvatarCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_ReqHeadAndHeadShokActiveItemCfg") {
		//获取头像框配置
		Beta87_ReqHeadAndHeadShokActiveItemCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_ReqGetQQUrlHead") {
		//我的qq头像
		Request_Beta87_ReqGetQQUrlHead(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta78_MechSkinAwardOpt") {
		//敖丙
		Request_Beta78_MechSkinAwardOpt(Client, Body);
		return;
	}
	else if (cmd == "Co.RequestGetBeta84ColorfulGourdCfg") {
		//七彩葫芦
		RequestGetBeta84ColorfulGourdCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.beta91_Mountain_Music_MainDlg") {
		//天山音韵
		Request_beta91_Mountain_Music_MainDlg(Client, Body);
		return;
	}
	else if (cmd == "Co.DaZaoWorkshop_OpenMainDlg") {
		//筑梦合约
		Request_DaZaoWorkshop_OpenMainDlg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta72ReqGodOfWarBoxCfg") {
		//三国秘宝
		Request_Beta72ReqGodOfWarBoxCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta91_Silk_Road_MainDlg") {
		//丝绸之路
		Request_Beta91_Silk_Road_MainDlg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta91_Xinjiang_MainStation_MainDlg") {
		//飞驰新疆
		Request_Beta91_Xinjiang_MainStation_MainDlg(Client, Body);
		return;
	}
	else if (cmd == "Co.RequestBeta90_SkinTreasureCfg") {
		//四季之神
		Request_RequestBeta90_SkinTreasureCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta81_ReqMechSkinEntrys") {
		//机甲皮肤
		Request_Beta81_ReqMechSkinEntrys(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta77_ReqNiChangPavilionCfg") {
		//霓裳阁
		Request_Beta77_ReqNiChangPavilionCfg(Client, Body);
		return;
	}
	else if (cmd == "Co.ConsumeScore_OpenMainDlg") {
		//魅力达人
		Request_ConsumeScore_OpenMainDlg(Client, Body);
		return;
		}
	else if (cmd == "Co.Beta87_ReqSetOrUnSetUseQQHead") {
		//保存头像请求
		Request_Beta87_ReqSetOrUnSetUseQQHead(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta87_ReqOpHeadAndHeadShok") {
		//保存头像记录
		Request_Beta87_ReqOpHeadAndHeadShok(Client, Body);
		return;
	}
	else if (cmd == "Co.Beta91_Cinamon_Cafe_MainDlg") {
		//大耳狗的咖啡屋
		Request_Beta91_Cinamon_Cafe_MainDlg(Client, Body);
		return;
	}
	else if (cmd == "DCo.Beta87_ReqOpHeadAndHeadShok")
	{
		//保存成功记录
		resp = "4469737061746368436F526573706F6E73650000F60000000000003140F60000000000000000FD00000132FA0355696EF6000040EFF7D9E141FA104163746976654865616449644C697374FD00000000FA0D51514865616455726C496E666FFD00000000FA09557365515148656164F60000000000000000FA0B4571756970486561644964FD0000003FFA024964F60000000000000040FA0E4C61737441637469766554696D65F6000080F4FF84D941FA0E45787069726174696F6E54696D65F6000000000000F0BFFA08526573756C744944F60000000000000000FA0F45717569704865616453686F6B4964FD00000021FB05F6000000000000F03FFB06F6000000000000F0BFFB07F6000000000000F0BFFA144163746976654865616453686F6B49644C697374FD0000002FF6000000000000F03FFD00000021FB05F60000000000003440FB06F6000000A8FD82D941FB07F6000000000000F0BF00000000000000";
	}
	else if (cmd == "Co.CarLibrary_OpenMainDlg") {
		// 车库
		if (version > 1.3) {
			Request_CarLibrary_OpenMainDlg(Client, Body);
			return;
		}
		else {
			resp = "4469737061746368436F526573706F6E73650000F60000000000003340F60000000000000000FD00000364FA18436F6C6C65637456616C7565496E4C6576656C546F74616CF6000000000088C340FA0C436F6C6C65637456616C7565F6000000000075BF40FA07506F73496E666FFD000000C6F6000000000000F03FF60000000040DBF240F60000000000000040F600000000C027F240F60000000000000840F60000000000C5D340F60000000000001040F6000000000041D140F60000000000001440F60000000010E0F840F60000000000001840F600000000305CF740F60000000000001C40F600000000F06CF840F60000000000002040F60000000090FBF440F60000000000002240F60000000020C2F040F60000000000002440F600000000601DFC40F60000000000002640F600000000A027F240FA1A486973746F727948696768657374436F6C6C65637456616C7565F6000000000075BF40FA0554316E756DF60000000000001040FA0554326E756DF6000000000000F03FFA0554336E756DF60000000000000000FA09466C616756616C7565FD00000036F60000000000003D40F6000000000000F03FF60000000000003B40F6000000000000F03FF60000000000003C40F6000000000000F03FFA07537569746E756DF6000000000000F03FFA0742617365436667FD0000010CFA0E416C6C6F77506C61636554506F73FD0000005AF6000000000000F03FF60000000000001C40F60000000000000040F60000000000002040F60000000000000840F60000000000002240F60000000000001040F60000000000002440F60000000000001440F60000000000002640FA09536E756D546F74616CF60000000000805D40FA144D6178436F6C6C65637456616C75654C6576656CF60000000000002440FA0C537569746E756D546F74616CF60000000000004340FA0A54326E756D546F74616CF60000000000805140FA0A54316E756D546F74616CF60000000000004940FA09416E756D546F74616CF60000000000F88140FA0A54336E756D546F74616CF60000000000002240FA11436F6C6C65637456616C75654C6576656CF60000000000002040FA04416E756DF60000000000003040FA0A466F7262696453686F77F6000000000000F03FFA056F74686572F60000000000004240FA0753686F774E756DF60000000000002640FA13436F6C6C65637456616C7565496E4C6576656CF6000000000075BF40FA04536E756DF600000000000010403500";

		}
	}
	else if (cmd == "Co.MapChallenge_GetOpenResultID") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000003240F600000000000000000000";
	}
	else if (cmd == "Co.MapChallenge_OpenMainDlg") {
		//赛道挑战
		if (version > 1.3) {
			RequestMapChallenge_OpenMainDlg(Client, Body);
			return;
		}
		else {
			resp = "4469737061746368436F526573706F6E73650000F60000000000003340F60000000000000000FD000005B6FA09436C6F736554696D65F60000C0BF6728D941FA104861734D61705265636F726444617461F8FA076D6170696E666FFD0000051FF6000000000000F03FFD0000012EFA054D61704944F60000000000B07840FA0D4D61705265636F726454696D65F600000000F019F740FA0A417373657373496E666FFD00000072FA034E756DF6000000000000F03FFA0A41737365737354797065F60000000000003740FA034D7367FA19CDEAB3C9B1C8C8FCB2A2CAB9D3C3574357C5E77B74637DB4CEFA074D736747616D65FA11CAB9D3C3574357C5E77B74637DB4CEA3BAFA08697366696E697368F60000000000000000FA0853746172496E666FFD00000074F6000000000000F03FFD0000001EFB0CF60000000000000000FA086261736574696D65F600000000F001FB40F60000000000000040FD00000016FB0CF60000000000000000FB0EF600000000F007FA40F60000000000000840FD00000016FB0CF60000000000000000FB0EF600000000F00DF940F60000000000000040FD000000B9FB03F60000000000E06F40FB04F60000000070F3F640FB05FD00000029FB06F6000000000000F03FFB07F60000000000003740FB08FB09FB0AFB0BFB0CF60000000000000000FB0DFD0000006CF6000000000000F03FFD00000016FB0CF60000000000000000FB0EF60000000070DBFA40F60000000000000040FD00000016FB0CF60000000000000000FB0EF60000000070E1F940F60000000000000840FD00000016FB0CF60000000000000000FB0EF60000000070E7F840F60000000000000840FD000000B9FB03F60000000000006540FB04F6000000000000F0BFFB05FD00000029FB06F6000000000000F03FFB07F60000000000003740FB08FB09FB0AFB0BFB0CF60000000000000000FB0DFD0000006CF6000000000000F03FFD00000016FB0CF60000000000000000FB0EF600000000C0440941F60000000000000040FD00000016FB0CF60000000000000000FB0EF600000000C0C70841F60000000000000840FD00000016FB0CF60000000000000000FB0EF600000000C04A0841F60000000000001040FD000000B9FB03F60000000000007040FB04F600000000802BF940FB05FD00000029FB06F6000000000000F03FFB07F60000000000003740FB08FB09FB0AFB0BFB0CF60000000000000000FB0DFD0000006CF6000000000000F03FFD00000016FB0CF60000000000000000FB0EF6000000008013FD40F60000000000000040FD00000016FB0CF60000000000000000FB0EF6000000008019FC40F60000000000000840FD00000016FB0CF60000000000000000FB0EF600000000801FFB40F60000000000001440FD000000B9FB03F60000000000606D40FB04F6000000000000F0BFFB05FD00000029FB06F6000000000000F03FFB07F60000000000003740FB08FB09FB0AFB0BFB0CF60000000000000000FB0DFD0000006CF6000000000000F03FFD00000016FB0CF60000000000000000FB0EF600000000804F0241F60000000000000040FD00000016FB0CF60000000000000000FB0EF60000000080D20141F60000000000000840FD00000016FB0CF60000000000000000FB0EF60000000080550141F60000000000001840FD000000B9FB03F60000000000C07440FB04F600000000A0DCF340FB05FD00000029FB06F6000000000000F03FFB07F60000000000003740FB08FB09FB0AFB0BFB0CF60000000000000000FB0DFD0000006CF6000000000000F03FFD00000016FB0CF60000000000000000FB0EF600000000A0C4F740F60000000000000040FD00000016FB0CF60000000000000000FB0EF600000000A0CAF640F60000000000000840FD00000016FB0CF60000000000000000FB0EF600000000A0D0F540FA0F526566726573684C65667444617973F60000000000001C40FA0742617365436667FD00000014FA094368616E6E656C4944F60000000000000840FA084F70656E54696D65F6000000201926D941FA08537461724E756D73F600000000000000003531";
		}
	}
	else if (cmd == "Co.MapChallenge_GetUserMapInfo") {
		//赛道挑战获取房间地图
		RequestMapChallenge_GetUserMapInfo(Client, Body);
		return;
	}
	//else if (cmd == "Co.Beta90_Newmap_MainDlg") {
	//	//新图试炼
	//	resp = "4469737061746368436F526573706F6E73650000F60000000000C05540F60000000000000000FD000005F4FA0F6E65776D61705461736B5461626C65FD0000041FF60000000000707C40FD0000023FF6000000000000F03FFD000000DEFA0A6D5F75635461736B4944F60000000000D6B440FA116D5F756941776172644D6F76654E756D73F6000000000000F03FFA09636C6F736544617465F600000010A74D7341FA0F6D5F756950726F6365737352617465F60000000000000000FA126D5F75695461736B46696E6973684E756D73F6000000000000F03FFA086F70656E44617465F6000000E0A14D7341FA0D45786563757465506572696F64F60000000000000000FA0D72656365697665537461747573F60000000000000000FA146D5F7569416C726561647941776172644E756D73F60000000000000000F60000000000000040FD00000063FB01F60000000000D7B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000F60000000000000840FD00000063FB01F60000000000D8B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000F60000000000001040FD00000063FB01F60000000000D9B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000F60000000000507C40FD000001C4F6000000000000F03FFD00000063FB01F60000000000D2B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000F60000000000000040FD00000063FB01F60000000000D3B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000F60000000000000840FD00000063FB01F60000000000D4B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000F60000000000001040FD00000063FB01F60000000000D5B440FB02F6000000000000F03FFB03F600000010A74D7341FB04F60000000000000000FB05F6000000000000F03FFB06F6000000E0A14D7341FB07F60000000000000000FB08F60000000000000000FB09F60000000000000000FA126E65776D61704461795461736B5461626C65FD000000FEF60000000000707C40FD00000071F6000000000000F03FFD00000063FB01F60000000000A07240FB02F6000000000000F03FFB03F60000000000000000FB04F60000000000000000FB05F6000000000000F03FFB06F60000000000000000FB07F6000000000000F03FFB08F60000000000000000FB09F60000000000000000F60000000000507C40FD00000071F6000000000000F03FFD00000063FB01F60000000000E06D40FB02F6000000000000F03FFB03F60000000000000000FB04F60000000000000000FB05F6000000000000F03FFB06F60000000000000000FB07F6000000000000F03FFB08F60000000000000000FB09F60000000000000000FA0762617365436667FD00000039FA07656E6454696D65F60000C01FAAC1D941FA09626567696E54696D65F6000000E00CBDD941FA08536561736F6E4944F60000000000000840FA0B6D61705072696F72697479FD0000004FF6000000000000F03FFD0000001DFA024944F6000000000000F03FFA056D61704944F60000000000507C40F60000000000000040FD00000016FB10F60000000000000040FB11F60000000000707C40";
	//}
	else if (cmd == "Co.CarLibraryOp") {
		//移除车库
		Request_CarLibraryOp(Client, Body);
		return;
	}
	else if (cmd == "Co.ConsumeScore_OpenMainDlg") {
		//查看别人车库
		Request_CarLibrary_QueryOtherData(Client, Body);
		return;
		}
	else if (cmd == "Co.CarLibrary_GetCollect") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000804A40F6000000000000894000A2";
	}

	else if (cmd == "Co.ProcessionLicense_OpenMainDlg") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000003440F60000000000000000FD00000AE6FA0B7765656B6578705F6D6178F6000000000082B440FA09726571626C6576656CF60000000000000040FA06736C6576656CF60000000000001440FA0D736C6576656C5F657870637572F600000000007CAA40FA0742617365436667FD00000056FA134D61784C6576656C5F536D616C6C4C6576656CF60000000000001440FA115765656B4D6178546F74616C53636F7265F60000000000409F40FA114D61784C6576656C5F4269674C6576656CF60000000000001840FA08506F776572436667FD000002F7F60000000000609240FD0000006BFA0B73696D706C655F64657363FA18B8C4D7B0D0D2D4CBD6B5C9FAD0A7B5C8BCB6C9CFCFDE2B31FA046E616D65FA12B8C4D7B0D0D2D4CBD6B5C9CFCFDECCD8C8A8FA0464657363FA22B8C4D7B0D0D2D4CBD6B5C9FAD0A7B5C8BCB6C9CFCFDE2B31A3A8B1E4CEAA3136A3A9F60000000000549240FD00000023FB09FA0FBBF1B5C3B9A4D7CACCE1C9FD313025FB0BFA08B9A4D7CACCD8C8A8FB0DFB0FF60000000000649240FD00000043FB09FA14C2E4BAF3BBF1B5C3BCAFC6F8D0A7C2CACCE1C9FDFB0BFA0CBCAFC6F8D0A7C2CACCD8C8A8FB0DFA17C2E4BAF3BBF1B5C3BCAFC6F8D0A7C2CACCE1C9FD313525F60000000000589240FD0000006EFB09FA14B1DFBEB3B6D4BED6BBF1B5C3B6EECDE2C8D9D3FEFB0BFA0CB1DFBEB3C8D9D3FECCD8C8A8FB0DFA42C3BFBED6B1DFBEB3B1C8C8FCBBF1B5C3B6EECDE2B5E3C8D9D3FEA3ACCFEDCADCB6E0B1B6BCD3B3C928B3F5BCB62B35A3ACB8DFBCB62B3130A3ACD7A8D2B52B313529F600000000004C9240FD0000008CFB09FA14C3BFC8D5CFDEB6A8B4CECAFDBEADD1E9BCD3B3C9FB0BFA08BEADD1E9CCD8C8A8FB0DFA64C3BFC8D5CFDEB6A8C7B03135BED6C8CED2E2B1C8C8FCA3ACC3BFBED6B6EECDE2BBF1B5C3247B6164646578705F6669786E756D7DBEADD1E928CEB4CDEAB3C9BBF1B5C3247B6164646578705F6669786E756D5F6E6F7466696E69736865647DBEADD1E929F600000000005C9240FD0000004EFB09FA16C3BFC8D5C5C5CEBBCAD7B4CECAA7B0DCB2BBB5F4B7D6FB0BFA10C5C5CEBBC8FCB2BBB5F4B7D6CCD8C8A8FB0DFA1CC3BFC8D5C5C5CEBBC8FCB6D4BED6CAD7B4CECAA7B0DCB2BBB5F4B7D6F60000000000509240FD0000007CFB09FA14C3BFD6DCCFDEB6A8B4CECAFDBACFB3C9B1A3BBA4FB0BFA0CB1A6CAAFBACFB3C9CCD8C8A8FB0DFA50C3BFD6DCCFDEB6A8C9FAD0A733B4CEA3ACB1A6CAAFBACFB3C9CAA7B0DCCAB1A3ACBFC9B1A3C1F431B8F6D3C3C0B4BACFB3C9B5C4B5CDB5C8BCB6B1A6CAAFA3A8D3C5CFC8CAB9D3C3D7A3B8A3CAAFA3A9FA0E726571626C6576656C5F696E666FFD00000517FA075461736B436667FD000001E5F6000000000000F03FFD0000007EFA0A6D5F75635461736B4944F60000000000DAAE40FA126D5F75695461736B46696E6973684E756D73F60000000000003E40FA06537461747573F60000000000000040FA146D5F7569416C726561647941776172644E756D73F60000000000003E40FA116D5F756941776172644D6F76654E756D73F60000000000C08240F60000000000000040FD00000037FB22F600000000006FB040FB23F60000000000001440FB24F60000000000000040FB25F60000000000001440FB26F60000000000C08240F60000000000000840FD00000037FB22F60000000000DCAE40FB23F60000000000003940FB24F60000000000000040FB25F60000000000003940FB26F60000000000C08240F60000000000001040FD00000037FB22F60000000000DEAE40FB23F60000000000002E40FB24F60000000000000040FB25F60000000000002E40FB26F60000000000C08240F60000000000001440FD00000037FB22F60000000000E0AE40FB23F60000000000008940FB24F60000000000000040FB25F60000000000008940FB26F60000000000C08240F60000000000001840FD00000037FB22F60000000000E2AE40FB23F60000000000002E40FB24F60000000000000040FB25F60000000000002E40FB26F60000000000C08240FA046C636667FD00000319FB21FD0000019EF6000000000000F03FFD00000037FB22F60000000000DAAE40FB23F60000000000003E40FB24F60000000000000040FB25F60000000000003E40FB26F60000000000C08240F60000000000000040FD00000037FB22F600000000006FB040FB23F60000000000001440FB24F60000000000000040FB25F60000000000001440FB26F60000000000C08240F60000000000000840FD00000037FB22F60000000000DCAE40FB23F60000000000003940FB24F60000000000000040FB25F60000000000003940FB26F60000000000C08240F60000000000001040FD00000037FB22F60000000000DEAE40FB23F60000000000002E40FB24F60000000000000040FB25F60000000000002E40FB26F60000000000C08240F60000000000001440FD00000037FB22F60000000000E0AE40FB23F60000000000008940FB24F60000000000000040FB25F60000000000008940FB26F60000000000C08240F60000000000001840FD00000037FB22F60000000000E2AE40FB23F60000000000002E40FB24F60000000000000040FB25F60000000000002E40FB26F60000000000C08240FA05706F776572FD00000024F6000000000000F03FF60000000000609240F60000000000000040F600000000004C9240FA0D737461725F657870746F74616CF6000000000094D140FA0B737461725F657870636667FD00000087F6000000000000F03FFD00000015FA0A75705F6E656564657870F6000000000020AC40F60000000000000040FD0000000BFB2BF6000000000020AC40F60000000000000840FD0000000BFB2BF6000000000020AC40F60000000000001040FD0000000BFB2BF6000000000020AC40F60000000000001440FD0000000BFB2BF6000000000020AC40FA0F706F696E745F7765656B6C696D6974F6000000000082B440FA12436F6D6D6F6E4177617264426F7854797065F60000000000C05C40FA0D6164646578705F6669786E756DF60000000000E08540FA196164646578705F6669786E756D5F6E6F7466696E6973686564F60000000000807140FA056C696D6974FD00000014FA09757365726C6576656CF60000000000005940FA0B506F776572537461747573FD000001AFF60000000000609240FD00000055FA046E756D73F60000000000002640FA0C706F7765725F626C6576656CF60000000000000040FB0BFB0CFB09FB0AFA06737461747573F6000000000000F03FFA076D61786E756D73F6000000000000F0BFFB0DFB0EF60000000000549240FD00000022FB34F60000000000001040FB0BFB10FB09FB0FFB35F60000000000000000FB0DFB0FF60000000000649240FD00000022FB34F60000000000001840FB0BFB12FB09FB11FB35F60000000000000000FB0DFB13F60000000000589240FD00000022FB34F60000000000001440FB0BFB15FB09FB14FB35F60000000000000000FB0DFB16F600000000004C9240FD00000038FB33F60000000000000040FB34F6000000000000F03FFB0BFB18FB09FB17FB35F6000000000000F03FFB36F60000000000002E40FB0DFB19F600000000005C9240FD00000022FB34F60000000000000840FB0BFB1BFB09FB1AFB35F60000000000000000FB0DFB1CF60000000000509240FD00000038FB33F60000000000000000FB34F6000000000000F03FFB0BFB1EFB09FB1DFB35F6000000000000F03FFB36F60000000000000840FB0DFB1FFA077765656B657870F60000000000005A40FA06626C6576656CF60000000000000040FA0E736C6576656C5F6578706E656564F6000000000020AC400000";

	}
	else if (cmd == "Co.BattlePass_OnShowMainDlg") {//                                                                                                                                                                                                                                                          4940 经验值                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                战勋等级:          
		resp = "4469737061746368436F526573706F6E73650000F60000000000004A40F60000000000000000FD000005ECFA15497343616E53686F774275794469616D6F6E446C67F6000000000000F03FFA0E497343616E4275794469616D6F6EF6000000000000F03FFA0E5765656B546F74616C53636F7265F60000000000F89140FA0A4C6576656C53636F7265F60000000000004940FA144C6576656C55704E65656453636F726553686F77F60000000000006940FA1742505F5269676874436F726E65725F4974656D4C697374FD000000B4F6000000000000F03FF60000000070E50041F60000000000000040F600000000F8AC0041F60000000000000840F600000000C0D60041F60000000000001040F60000000070D60041F60000000000001440F60000000080D60041F60000000000001840F60000000030D60041F60000000000001C40F60000000050FF0041F60000000000002040F60000000000FF0041F60000000000002240F60000000010FF0041F60000000000002440F600000000C0FE0041FA054C6576656CF60000000000006440FA094C6576656C54797065F60000000000000000FA0742617365436667FD0000034EFA0D436F6E73756D654974656D4944F6000000005098F740FA13536561736F6E436C6561724974656D4C697374FD00000024F6000000000000F03FF60000000040EEFF40F60000000000000040F600000000F020F740FA0B43617368436F75706F6E73FD00000028FA0C7265647563655F6D6F6E6579F60000000000000000FA066974656D6964F600000000F034F940FA10536561736F6E456E6454696D65537472FA13323032342D30342D33302032333A35393A3539FA124275794469616D6F6E5F4D61784C6576656CF60000000000006940FA12536561736F6E426567696E54696D65537472FA13323032342D30332D31392030303A30303A3030FA115765656B4D6178546F74616C53636F7265F60000000000E0A540FA08536561736F6E4944F60000000000003C40FA144E6F726D616C506C617965724D61784C6576656CF60000000000C06240FA104C6576656C55704E65656453636F7265F60000000000006940FA0B48756F646F6E6744657363FA0CC8D9D2ABD5BDD1AB3235C6DAFA13536561736F6E4D6178546F74616C53636F7265F6000000000058CB40FA0F536561736F6E426567696E54696D65F600000040197ED941FA0750617954797065F60000000000000040FA114964656E746974795374617475734D6170FD00000096F6000000000000F03FFD00000024F6000000000000F03FF60000000000806140F60000000000000040F60000000000003F40F60000000000000040FD00000024F6000000000000F03FF60000000000004940F60000000000000040F60000000000003E40F60000000000000840FD00000024F6000000000000F03FF60000000000003E40F60000000000000040F60000000000003D40FA1D4E6F744275794469616D6F6E53686F774578747261446C674C6576656CF60000000000005440FA174275794469616D6F6E436F6D626F5F4D61784C6576656CF60000000000006940FA1D4E6F744275794469616D6F6E53686F774578747261446C674D6F6E6579F60000000000C08B40FA0D536561736F6E456E6454696D65F60000C05F458CD941FA1B53636F7265557067726164655F506C617965724D61784C6576656CF60000000000805B40FA1A4E6F744F70656E536561736F6E5461736B5F4F70656E54696D65F60000000000000000FA11436F6E73756D654974656D49445F4E756DF60000000000000000FA1C4F7665724D61784C6576656C5F47657441776172644C6566744E756DF60000000000000000FA0D497353686F774E657755736572F9FA224275794469616D6F6E436F6D6D6F6E446C675F5265616C4164644974656D4C697374FD00000052F6000000000000F03FFD00000044F6000000000000F03FFD00000012F6000000000000F03FF6000000009008F840F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFFA09736561736F6E637572F60000000000508440FA13497343616E4275794469616D6F6E436F6D626FF6000000000000F03FFA09736561736F6E6D6178F6000000000058CB40000000000000";
		//战巡  4469737061746368436F526573706F6E73650000F60000000000003840F60000000000000000FD000005ECFA15497343616E53686F774275794469616D6F6E446C67F6000000000000F03FFA0E497343616E4275794469616D6F6EF6000000000000F03FFA0E5765656B546F74616C53636F7265F60000000000F89140FA0A4C6576656C53636F7265F60000000000005940FA144C6576656C55704E65656453636F726553686F77F60000000000006940FA1742505F5269676874436F726E65725F4974656D4C697374FD000000B4F6000000000000F03FF60000000070E50041F60000000000000040F600000000F8AC0041F60000000000000840F600000000C0D60041F60000000000001040F60000000070D60041F60000000000001440F60000000080D60041F60000000000001840F60000000030D60041F60000000000001C40F60000000050FF0041F60000000000002040F60000000000FF0041F60000000000002240F60000000010FF0041F60000000000002440F600000000C0FE0041FA054C6576656CF60000000000002240FA094C6576656C54797065F60000000000000000FA0742617365436667FD0000034EFA0D436F6E73756D654974656D4944F6000000005098F740FA13536561736F6E436C6561724974656D4C697374FD00000024F6000000000000F03FF60000000040EEFF40F60000000000000040F600000000F020F740FA0B43617368436F75706F6E73FD00000028FA0C7265647563655F6D6F6E6579F60000000000000000FA066974656D6964F600000000F034F940FA10536561736F6E456E6454696D65537472FA13323032342D30342D33302032333A35393A3539FA124275794469616D6F6E5F4D61784C6576656CF60000000000006940FA12536561736F6E426567696E54696D65537472FA13323032342D30332D31392030303A30303A3030FA115765656B4D6178546F74616C53636F7265F60000000000E0A540FA08536561736F6E4944F60000000000003C40FA144E6F726D616C506C617965724D61784C6576656CF60000000000C06240FA104C6576656C55704E65656453636F7265F60000000000006940FA0B48756F646F6E6744657363FA0CC8D9D2ABD5BDD1AB3235C6DAFA13536561736F6E4D6178546F74616C53636F7265F6000000000058CB40FA0F536561736F6E426567696E54696D65F600000040197ED941FA0750617954797065F60000000000000040FA114964656E746974795374617475734D6170FD00000096F6000000000000F03FFD00000024F6000000000000F03FF60000000000806140F60000000000000040F60000000000003F40F60000000000000040FD00000024F6000000000000F03FF60000000000004940F60000000000000040F60000000000003E40F60000000000000840FD00000024F6000000000000F03FF60000000000003E40F60000000000000040F60000000000003D40FA1D4E6F744275794469616D6F6E53686F774578747261446C674C6576656CF60000000000005440FA174275794469616D6F6E436F6D626F5F4D61784C6576656CF60000000000006940FA1D4E6F744275794469616D6F6E53686F774578747261446C674D6F6E6579F60000000000C08B40FA0D536561736F6E456E6454696D65F60000C05F458CD941FA1B53636F7265557067726164655F506C617965724D61784C6576656CF60000000000805B40FA1A4E6F744F70656E536561736F6E5461736B5F4F70656E54696D65F60000000000000000FA11436F6E73756D654974656D49445F4E756DF60000000000000000FA1C4F7665724D61784C6576656C5F47657441776172644C6566744E756DF60000000000000000FA0D497353686F774E657755736572F9FA224275794469616D6F6E436F6D6D6F6E446C675F5265616C4164644974656D4C697374FD00000052F6000000000000F03FFD00000044F6000000000000F03FFD00000012F6000000000000F03FF6000000009008F840F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFFA09736561736F6E637572F60000000000009940FA13497343616E4275794469616D6F6E436F6D626FF6000000000000F03FFA09736561736F6E6D6178F6000000000058CB40
	}
	else if (cmd == "Co.BattlePass_OnShowBuyDiamondDlg") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000003E40F60000000000000000FD0000043AFA1F4275794469616D6F6E436F6D6D6F6E446C675F53686F774974656D4C697374FD00000198F6000000000000F03FFD00000036F6000000000000F03FF60000000070E50041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000000040FD00000036F6000000000000F03FF600000000F8AC0041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000000840FD00000036F6000000000000F03FF600000000C0D60041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000001040FD00000036F6000000000000F03FF60000000080D60041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000001440FD00000036F6000000000000F03FF60000000050FF0041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000001840FD00000036F6000000000000F03FF60000000010FF0041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFFA0C4275794469616D6F6E436667FD000000B1F6000000000000F03FFD00000022FA056D6F6E6579F60000000000806640FA0762757974797065F6000000000000F03FF60000000000000040FD00000039FB03F60000000000000040FB02F60000000000807640FA056C6576656CF60000000000002E40FA08646973636F756E74F60000000000001040F60000000000000840FD0000002CFB03F60000000000000040FB02F60000000000E09040FB04F60000000000004440FB05F60000000000000840FA194275794469616D6F6E446C675F53686F774974656D4C697374FD00000198F6000000000000F03FFD00000036F6000000000000F03FF60000000070E50041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000000040FD00000036F6000000000000F03FF600000000F8AC0041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000000840FD00000036F6000000000000F03FF600000000C0D60041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000001040FD00000036F6000000000000F03FF60000000080D60041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000001440FD00000036F6000000000000F03FF60000000050FF0041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BFF60000000000001840FD00000036F6000000000000F03FF60000000010FF0041F60000000000000040F6000000000000F03FF60000000000000840F6000000000000F0BF";
		//战巡激活
	}
	else if (cmd == "Co.BattlePass_OnDoBuyDiamond") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000004040F6000000008003C840";
		//战巡购买
	}
	else if (cmd == "Co.BattlePass_OnShowScoreGetDlg") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000004340F60000000000000000FD00000E9EFA0C5461736B4C6973745765656BFD0000055DF6000000000000F03FFD0000002BFA0573636F7265F60000000000005940FA026964F60000000000C2A140FA03746167F60000000000000000F60000000000000040FD00000021FB01F60000000000005940FB02F60000000000C3B140FB03F60000000000000000F60000000000000840FD00000021FB01F60000000000005940FB02F600000000002CAD40FB03F60000000000000000F60000000000001040FD00000021FB01F60000000000004940FB02F600000000002EAD40FB03F60000000000000000F60000000000001440FD00000021FB01F60000000000005940FB02F60000000000A8AC40FB03F60000000000000000F60000000000001840FD00000021FB01F60000000000C05240FB02F6000000000026AD40FB03F60000000000000000F60000000000001C40FD00000021FB01F60000000000C05240FB02F6000000000028AD40FB03F60000000000000000F60000000000002040FD00000021FB01F60000000000C05240FB02F60000000000E06D40FB03F60000000000000000F60000000000002240FD00000021FB01F60000000000C05240FB02F60000000000A07240FB03F60000000000000000F60000000000002440FD00000021FB01F60000000000004940FB02F600000000002AAD40FB03F60000000000000000F60000000000002640FD00000021FB01F60000000000C05240FB02F60000000000C4A940FB03F60000000000000000F60000000000002840FD00000021FB01F60000000000C05240FB02F60000000000F8AC40FB03F60000000000000000F60000000000002A40FD00000021FB01F60000000000C05240FB02F60000000000CAB140FB03F60000000000000000F60000000000002C40FD00000021FB01F60000000000C05240FB02F6000000000026A340FB03F60000000000000000F60000000000002E40FD00000021FB01F60000000000C05240FB02F60000000000C06E40FB03F60000000000000000F60000000000003040FD00000021FB01F60000000000C05240FB02F60000000000FEAC40FB03F60000000000000000F60000000000003140FD00000021FB01F60000000000C05240FB02F60000000000CBB140FB03F60000000000000000F60000000000003240FD00000021FB01F60000000000C05240FB02F6000000000000AD40FB03F60000000000000000F60000000000003340FD00000021FB01F60000000000C05240FB02F60000000000F09940FB03F60000000000000000F60000000000003440FD00000021FB01F60000000000C05240FB02F6000000000096A240FB03F60000000000000000F60000000000003540FD00000021FB01F60000000000003940FB02F6000000000030AD40FB03F60000000000000000F60000000000003640FD00000021FB01F60000000000C05240FB02F60000000000C7B140FB03F60000000000000000F60000000000003740FD00000021FB01F60000000000C05240FB02F6000000000008AD40FB03F60000000000000000F60000000000003840FD00000021FB01F60000000000004940FB02F6000000000032AD40FB03F60000000000000000F60000000000003940FD00000021FB01F60000000000004940FB02F6000000000034AD40FB03F60000000000000000F60000000000003A40FD00000021FB01F60000000000C05240FB02F60000000000D49640FB03F60000000000000000F60000000000003B40FD00000021FB01F60000000000004940FB02F60000000000E09640FB03F60000000000000000F60000000000003C40FD00000021FB01F60000000000C05240FB02F6000000000012AD40FB03F60000000000000000F60000000000003D40FD00000021FB01F60000000000C05240FB02F6000000000014AD40FB03F60000000000000000FA0E5461736B4C697374536561736F6EFD00000919FA084F70656E5461736BFD000008E5F6000000000000F03FFD0000017EFA0850726F6772657373F60000000000000000FA12436F6D6D6F6E4576656E7449444172726179FD00000036FA2B5441534B5F434F4D4D4F4E5F4556454E545F49445F54696D65724368616C6C656E6765326E645F4A6F696EF60000000000606840FA0D5461736B436F6E646974696F6EFD00000081F6000000000000F03FFD0000002BFA0A416464425053636F7265F60000000000C07240FA0B46696E697368436F756E74F60000000000002440F60000000000000040FD00000016FB0AF60000000000C07240FB0BF60000000000003440F60000000000000840FD00000016FB0AF60000000000C07240FB0BF60000000000003E40FA07456E6454696D65F60000C05F458CD941FA09426567696E54696D65F600000040197ED941FA0D5669727475616C5461736B4964F6000000000000F03FFA0444657363FA41332E31392D342E3330C6DABCE4A3ACB2CED3EBBBC3D3F2B4F3B4B3B9D8A3ACC0DBBCC6B2CED3EBB4EFB5BDCFE0D3A6B4CECAFDA3ACBFC9BBF1B5C3BBFDB7D6A1A3F60000000000000040FD00000121FB06F60000000000000000FB07FD0000003AFA2F5441534B5F434F4D4D4F4E5F4556454E545F49445F54696D65724368616C6C656E6765326E645F416464526F756E64F60000000000406B40FB09FD0000006CF6000000000000F03FFD00000016FB0AF60000000000C07240FB0BF60000000000005940F60000000000000040FD00000016FB0AF60000000000007940FB0BF60000000000006940F60000000000000840FD00000016FB0AF60000000000007940FB0BF60000000000C07240FB0CF60000C05F458CD941FB0DF600000040197ED941FB0EF60000000000000040FB0FFA3D332E31392D342E3330C6DABCE4A3ACB2CED3EBBBC3D3F2B4F3B4B3B9D8A3ACC0DBBCC6B4B3B9D8CFE0D3A6CAFDC1BFA3ACBFC9BBF1B5C3BBFDB7D6A1A3F60000000000000840FD00000105FB06F60000000000000840FB07FD00000012F6000000000000F03FF60000000000E07040FB09FD0000006CF6000000000000F03FFD00000016FB0AF60000000000C07240FB0BF60000000000000840F60000000000000040FD00000016FB0AF60000000000C07240FB0BF60000000000001840F60000000000000840FD00000016FB0AF60000000000C07240FB0BF60000000000002240FB0CF60000C05F458CD941FB0DF6000000801380D941FB0EF60000000000000840FB0FFA49332E32352D342E3330C6DABCE4A3ACC0DBBCC6B5C7C2BCB2A2C7D2B5B1CCECB2CED3EBD2BBBED6B5A5BED6C4A3CABDB4EFB5BDD6B8B6A8B4CECAFDA3ACBFC9BBF1B5C3BBFDB7D6A1A3F60000000000001040FD000000BCFB06F60000000000000840FB07FD00000012F6000000000000F03FF60000000000E07040FB09FD0000006CF6000000000000F03FFD00000016FB0AF60000000000C07240FB0BF60000000000002840F60000000000000040FD00000016FB0AF60000000000007940FB0BF60000000000002E40F60000000000000840FD00000016FB0AF60000000000007940FB0BF60000000000003240FB0CF60000C05F458CD941FB0DF6000000801380D941FB0EF60000000000001040FB0FFB13F60000000000001440FD0000013DFA11436F6D706C6574654D6F64654172726179FD0000003AF6000000000000F03FFD00000013FA08626173656D6F6465F6000000000000F03FF60000000000000040FD0000000BFB15F60000000000000040FB06F60000000000002040FB09FD0000006CF6000000000000F03FFD00000016FB0AF60000000000C07240FB0BF60000000000002440F60000000000000040FD00000016FB0AF60000000000C07240FB0BF60000000000003440F60000000000000840FD00000016FB0AF60000000000C07240FB0BF60000000000003E40FB0CF60000C05F458CD941FB0DF6000000206282D941FB0EF60000000000001440FB0FFA48342E312D342E3330C6DABCE4A3ACB2CED3EBB4ABCDB3BEBACBD9C4A3CABDBBF2D5DFB5C0BEDFC8FCC4A3CABDA3ACB4EFB5BDCFE0D3A6B5C4B4CECAFDA3ACBFC9BBF1B5C3BBFDB7D6F60000000000001840FD000000DCFB14FD00000032F6000000000000F03FFD0000000BFB15F6000000000000F03FF60000000000000040FD0000000BFB15F60000000000000040FB06F60000000000002040FB09FD0000006CF6000000000000F03FFD00000016FB0AF60000000000C07240FB0BF60000000000004440F60000000000000040FD00000016FB0AF60000000000007940FB0BF60000000000004940F60000000000000840FD00000016FB0AF60000000000007940FB0BF60000000000004E40FB0CF60000C05F458CD941FB0DF6000000206282D941FB0EF60000000000001840FB0FFB16F60000000000001C40FD00000129FB09FD00000080F6000000000000F03FFD00000022FB0AF60000000000C07240FB0BF60000000000001440FA06537566666978FA02B4CEF60000000000000040FD0000001AFB0AF60000000000C07240FB0BF60000000000002440FB17FB18F60000000000000840FD0000001AFB0AF60000000000C07240FB0BF60000000000002E40FB17FB18FB0CF60000C05F458CD941FB0EF60000000000001C40FB0FFA44342E382D342E3330C6DABCE4A3ACB2CED3EBC8CED2E2B1DFBEB3BBF2D5DFC8D9D2ABC5C5CEBBA3ACB4EFB5BDCFE0D3A6B5C4B4CECAFDA3ACBFC9BBF1B5C3BBFDB7D6A1A3FA0A73657276657274797065F60000000000002640FB0DF6000000C0B084D941FB06F60000000000000000FB07FD00000012F6000000000000F03FF60000000000606040F60000000000002040FD000000D3FB09FD00000078F6000000000000F03FFD0000001AFB0AF60000000000C07240FB0BF60000000000003440FB17FB18F60000000000000040FD0000001AFB0AF60000000000007940FB0BF60000000000003940FB17FB18F60000000000000840FD0000001AFB0AF60000000000007940FB0BF60000000000003E40FB17FB18FB0CF60000C05F458CD941FB0EF60000000000002040FB0FFB19FB1AF60000000000002640FB0DF6000000C0B084D941FB06F60000000000000000FB07FD00000012F6000000000000F03FF60000000000606040FA09436C6F73655461736BFD00000000FA0E436C6F73655461736B4172726179FD00000000000000";
		//战巡任务获取
	}
	else if (cmd == "Co.Beta90_RequestGetLoveBabyEquipItem") {
		resp = "4469737061746368436F526573706F6E73650000F60000000000002C40F60000000000000000FD00000025FA074974656D4E756DF60000000000000000FA08526573756C744944F60000";
	}
	else if (cmd == "OnReqCheckUserData") {
		resp = "4F6E526571436865636B55736572446174610000F60000000000000000FD0000001CFA026964F60000000070CEF340FA0474797065F60000000000001440";
	}

	else if (cmd == "Co.Power_GetStatus") {
		//边境
		Request_Power_GetStatus(Client, Body);

		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		WriteHex(p, "001800000000000A001E000A00000000000399B70000000000180000000100640041000A00000000000399B7000003E8001800000002000A001E000A00000000000399B70000000000180000000300640041000A00000000000399B7000003E8001800000004012C005A000A000222E00003345000004E20001800000005012C005A000A000222E00003345000004E200000000000000E00000000000A001E00000000000E0000000100640041000003E8000E00000002000A001E00000000000E0000000300640041000003E8000E00000004012C005A00004E20000E00000005012C005A00004E20000A66F58500674F2AFF0001000A0001E07800036718000000003C0000003C0000003C0000003C0000003C0000003C0000003C0000003C0000003C0001000A0001E0780003671800BB01005C000F0001000A00000000000399B701000F0001000A00000000000399B701000F0001000A00000000000399B701000F0001000A00000000000399B701000F0001000A000222E00003345001000F0001000A000222E00003345001005C000F0001000A0001E0780003671801000F0001000A0001E0780003671801000F0001000A0001E0780003671801000F0001000A0001E0780003671801000F0001000A000222E00003345001000F0001000A000222E00003345001000E0001000A0001E07800036718000E0001000A0001E07800036718000E0001000A0001E07800036718000E0001000A0001E07800036718000E0001000A000222E000033450000E0001000A000222E000033450001E001A00000000000000000000000000000000000000000000000000000500002FE400002EB6000033FF00003E7400002F0A");
		len = p - buf.data();
		SendToClient(Client, 544, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		return;
	}


	//Z_FATAL("debug, lineno=%d\n", __LINE__);
#ifdef DEBUG
	//Z_FATAL("resp=%s\n", resp.data());
	//Z_FATAL("msg=%s\n", (char*)msg);
	//Z_FATAL("cmd=%s\n", cmd.data());
#endif // DEBUG



	// Z_FATAL("cmd=%s, ping=pong=%d, resp=%s\n", cmd.c_str(), ping, resp.c_str());
	// printf("cmd=%s, ping=pong=%d, resp=%s\n", cmd.c_str(), ping, resp.c_str());

	if (resp != "") {
		response = EHexToByte(resp.data());
		int len_of_response = resp.size() / 2;
		// replace pong
		int i = 0;
		for (; i < len_of_response; i++) {
			if (response[i] == 0 && response[i + 1] == 0) {
				break;
			}
		}
		// assert find i
		i += 2; // 0x0000
		i += 1; // 0xF6
		z_set<double>(response + i, pong);

		MyResponseQuickRPC(Client, response, len_of_response);
		delete[] response;
	}
	return;



	// printf("ResponseQuickRPC Uin:%d Version:%d Compress:%d OptLength:%d MsgLen:%d\n", Uin, Version, Compress, OptLength, MsgLen);
#ifdef DEBUG
	// printf("ResponseQuickRPC:%s\n", pMsg.data());
#endif
}

void RequestRankedMatchTopRaceOpt(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	short OptType = Read16(p);
	UINT Para0 = Read32(p);
	ResponseRankedMatchTopRaceOpt(Client, OptType, Para0);
}
void ResponseRankedMatchTopRaceOpt(std::shared_ptr<ClientNode> Client, short OptType, UINT Para0)
{
	//BYTE buf[65500]; //BYTE* p = buf; WORD len;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	Write32(p, Client->Uin);//Uin
	Write16(p, 0);//ResultID
	Write16(p, OptType);//OptType
	Write32(p, Para0);//Para0
	Write8(p, 0);//ParaNum
	//Write32(p, 0);//ParaList[]
	Write8(p, 1);//HasTopRaceInfo
	for (size_t i = 0; i < 1; i++)
	{//TopRaceInfo
		BYTE* pTopRaceInfo = p;
		Write16(pTopRaceInfo, 0);//len

		Write32(pTopRaceInfo, 9999);//TopRaceScore
		Write8(pTopRaceInfo, 0);//CanCertifcJoin
		{//TopRaceCfgToClnt
			BYTE* pTopRaceCfgToClnt = pTopRaceInfo;
			Write16(pTopRaceCfgToClnt, 0);//len

			Write32(pTopRaceCfgToClnt, 0x0134d7c5);//PreviewDate
			Write32(pTopRaceCfgToClnt, 0x0134d82c);//OpenDate
			Write32(pTopRaceCfgToClnt, 0x00030d40);//CloseDate
			Write32(pTopRaceCfgToClnt, 0x0003a980);//OpenTime
			Write32(pTopRaceCfgToClnt, 0x00000000);//CloseTime
			Write32(pTopRaceCfgToClnt, 0x06010000);//ChallengeCostScore
			Write8(pTopRaceCfgToClnt, 0);//ChallengeRoundNums
			Write8(pTopRaceCfgToClnt, 0x63);//TopRaceScoreConvertRate
			Write16(pTopRaceCfgToClnt, 0);//WeekGodFrameRankLimit
			Write16(pTopRaceCfgToClnt, 0x01f4);//WeekGodAppellationRankLimit
			{//SeaonAwardCfg
				BYTE* pSeaonAwardCfg = pTopRaceCfgToClnt;
				Write16(pSeaonAwardCfg, 0);//len

				Write16(pSeaonAwardCfg, 3);//EffectAwardNum
				{//EffectAwardCfg
					BYTE* pEffectAwardCfg = pSeaonAwardCfg;
					Write16(pEffectAwardCfg, 0);//len

					Write32(pEffectAwardCfg, 2);//EffectID
					Write32(pEffectAwardCfg, 0x000001f4);//CondLimit
					Write8(pEffectAwardCfg, 1);//AwardMethod

					len = (WORD)(pEffectAwardCfg - pSeaonAwardCfg);
					Set16(pSeaonAwardCfg, len);
					pSeaonAwardCfg += len;
				}

				{//EffectAwardCfg
					BYTE* pEffectAwardCfg = pSeaonAwardCfg;
					Write16(pEffectAwardCfg, 0);//len

					Write32(pEffectAwardCfg, 3);//EffectID
					Write32(pEffectAwardCfg, 0x000003e8);//CondLimit
					Write8(pEffectAwardCfg, 1);//AwardMethod

					len = (WORD)(pEffectAwardCfg - pSeaonAwardCfg);
					Set16(pSeaonAwardCfg, len);
					pSeaonAwardCfg += len;
				}

				{//EffectAwardCfg
					BYTE* pEffectAwardCfg = pSeaonAwardCfg;
					Write16(pEffectAwardCfg, 0);//len

					Write32(pEffectAwardCfg, 1);//EffectID
					Write32(pEffectAwardCfg, 0x000005dc);//CondLimit
					Write8(pEffectAwardCfg, 1);//AwardMethod

					len = (WORD)(pEffectAwardCfg - pSeaonAwardCfg);
					Set16(pSeaonAwardCfg, len);
					pSeaonAwardCfg += len;
				}
				{//FrameAwardCfg
					BYTE* pFrameAwardCfg = pSeaonAwardCfg;
					Write16(pFrameAwardCfg, 0);//len

					Write32(pFrameAwardCfg, 5);//FrameID
					Write32(pFrameAwardCfg, 0x00021b4c);//ActiveItemID
					Write32(pFrameAwardCfg, 0x000007d0);//CondLimit
					Write8(pFrameAwardCfg, 1);//AwardMethod

					len = (WORD)(pFrameAwardCfg - pSeaonAwardCfg);
					Set16(pSeaonAwardCfg, len);
					pSeaonAwardCfg += len;
				}

				len = (WORD)(pSeaonAwardCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}
			Write8(pTopRaceCfgToClnt, 9);//ScoreGradeNum
			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x00000000);//MinScore
				Write32(pScoreCfg, 0x00000064);//MaxScore
				Write32(pScoreCfg, 0x00000000);//StartChallengeCostScore
				Write32(pScoreCfg, 0x0000000f);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000012);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000014);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000016);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000018);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff6);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffe7);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x00000065);//MinScore
				Write32(pScoreCfg, 0x000001f4);//MaxScore
				Write32(pScoreCfg, 0x00000005);//StartChallengeCostScore
				Write32(pScoreCfg, 0x0000000f);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000012);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000014);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000016);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000018);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff6);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffe7);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x000001f5);//MinScore
				Write32(pScoreCfg, 0x000003e8);//MaxScore
				Write32(pScoreCfg, 0x00000005);//StartChallengeCostScore
				Write32(pScoreCfg, 0x0000000d);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000f);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000011);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000013);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000015);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff7);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffec);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x000003e9);//MinScore
				Write32(pScoreCfg, 0x000005dc);//MaxScore
				Write32(pScoreCfg, 0x0000000a);//StartChallengeCostScore
				Write32(pScoreCfg, 0x0000000b);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000d);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000010);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000013);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000015);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff7);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffec);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x000005dd);//MinScore
				Write32(pScoreCfg, 0x000007d0);//MaxScore
				Write32(pScoreCfg, 0x0000000f);//StartChallengeCostScore
				Write32(pScoreCfg, 0x00000009);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000a);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000e);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000010);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000014);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff8);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffef);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x000007d1);//MinScore
				Write32(pScoreCfg, 0x000009c4);//MaxScore
				Write32(pScoreCfg, 0x0000000f);//StartChallengeCostScore
				Write32(pScoreCfg, 0x00000007);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000009);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000e);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000010);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000014);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffb);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff0);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x000009c5);//MinScore
				Write32(pScoreCfg, 0x00000bb8);//MaxScore
				Write32(pScoreCfg, 0x00000014);//StartChallengeCostScore
				Write32(pScoreCfg, 0x00000007);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000009);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000b);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000d);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000010);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffc);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffff1);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x00000bb9);//MinScore
				Write32(pScoreCfg, 0x00000dac);//MaxScore
				Write32(pScoreCfg, 0x00000019);//StartChallengeCostScore
				Write32(pScoreCfg, 0x00000007);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000009);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000b);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000d);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000010);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffc);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffec);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}

			{//ScoreCfg
				BYTE* pScoreCfg = pTopRaceCfgToClnt;
				Write16(pScoreCfg, 0);//len

				Write32(pScoreCfg, 0x00000dad);//MinScore
				Write32(pScoreCfg, 0x0000270f);//MaxScore
				Write32(pScoreCfg, 0x00000019);//StartChallengeCostScore
				Write32(pScoreCfg, 0x00000007);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000009);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000b);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x0000000d);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000010);//AddScoreByWinNum[]
				Write32(pScoreCfg, 0x00000000);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffff);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffe);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xfffffffc);//DecrScoreByWinNum[]
				Write32(pScoreCfg, 0xffffffec);//DecrScoreByWinNum[]

				len = (WORD)(pScoreCfg - pTopRaceCfgToClnt);
				Set16(pTopRaceCfgToClnt, len);
				pTopRaceCfgToClnt += len;
			}
			Write16(pTopRaceCfgToClnt, 0x0022);//SeasonMapNums
			Write16(pTopRaceCfgToClnt, 0x0000007a);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000007b);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000085);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000d0);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000a7);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000b5);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000b9);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000bd);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000189);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000c2);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000016c);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000d4);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000de);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000e7);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000ef);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000fe);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000010a);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000124);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000149);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000014c);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000153);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000001b4);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000007c);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000178);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000001b3);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000018a);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000018d);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000018f);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000106);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000114);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000099);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x000000c7);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x00000084);//SeasonMapID[]
			Write16(pTopRaceCfgToClnt, 0x0000011b);//SeasonMapID[]

			Write8(pTopRaceCfgToClnt, 0x1a);//MinJoinGradeLev
			Write8(pTopRaceCfgToClnt, 0);//MinJoinChildLev

			len = (WORD)(pTopRaceCfgToClnt - pTopRaceInfo);
			Set16(pTopRaceInfo, len);
			pTopRaceInfo += len;
		}
		Write8(pTopRaceInfo, 1);//HasCertifcInfo
		for (size_t i = 0; i < 1; i++)
		{//CertifcInfo
			BYTE* pCertifcInfo = pTopRaceInfo;
			Write16(pCertifcInfo, 0);//len

			Write8(pCertifcInfo, 0);//CertifctNeedMapNums
			Write8(pCertifcInfo, 0);//MaxRefreshCertifctMapCnt
			Write8(pCertifcInfo, 0);//CertifcMapSuccTag
			Write8(pCertifcInfo, 0);//LeftRefreshMapNum
			Write8(pCertifcInfo, 0);//NeedCeritfcMapNum
			Write8(pCertifcInfo, 1);
			Write16(pCertifcInfo, 1);
			//for (size_t i = 0; i < 1; i++)
			//{//CerificMapCfg
			//	BYTE* pCerificMapCfg = pCertifcInfo;
			//	Write16(pCerificMapCfg, 0);//len

			//	Write32(pCerificMapCfg, 0);//MapID
			//	Write32(pCerificMapCfg, 0);//FinTime

			//	len = (WORD)(pCerificMapCfg - pCertifcInfo);
			//	Set16(pCertifcInfo, len);
			//	pCertifcInfo += len;
			//}

			len = (WORD)(pCertifcInfo - pTopRaceInfo);
			Set16(pTopRaceInfo, len);
			pTopRaceInfo += len;
		}
		Write8(pTopRaceInfo, 0);//HasChallengeInfo
		//for (size_t i = 0; i < n; i++)
		//{//ChallengeInfo
		//	BYTE* pChallengeInfo = pTopRaceInfo;
		//	Write16(pChallengeInfo, 0);//len

		//	Write32(pChallengeInfo, 0);//ChallengeScore
		//	Write8(pChallengeInfo, 0);//ChallengeWinTag
		//	Write8(pChallengeInfo, 0);//ChallengeRound

		//	len = (WORD)(pChallengeInfo - pTopRaceInfo);
		//	Set16(pTopRaceInfo, len);
		//	pTopRaceInfo += len;
		//}
		Write8(pTopRaceInfo, 0);//FinishCertifc
		Write8(pTopRaceInfo, 0);//ReachMinJoinGradeCurSeason

		len = (WORD)(pTopRaceInfo - p);
		Set16(p, len);
		p += len;
	}
	Write32(p, 0);//SeasonRecordNum
	Write32(p, 0);//SeasonRecordNum
	Write32(p, 0);//SeasonRecordNum
	Write8(p, 0);//SeasonRecordNum
	//for (size_t i = 0; i < n; i++)
	//{//SeasonRecords
	//	BYTE* pSeasonRecords = p;
	//	Write16(pSeasonRecords, 0);//len

	//	Write32(pSeasonRecords, 0);//SeasonID
	//	Write32(pSeasonRecords, 0);//TopRaceScore
	//	Write32(pSeasonRecords, 0);//Rank
	//	Write32(pSeasonRecords, 0);//RaceNum
	//	Write32(pSeasonRecords, 0);//WinNum
	//	Write32(pSeasonRecords, 0);//TotalThreeWinNum
	//	Write32(pSeasonRecords, 0);//TotalFourWinNum
	//	Write32(pSeasonRecords, 0);//TotalFiveWinNum
	//	Write16(pSeasonRecords, 0);//MaxContinWinNum

	//	len = (WORD)(pSeasonRecords - p);
	//	Set16(p, len);
	//	p += len;
	//}

	len = (WORD)(p - buf.data());
	SendToClient(Client, 28435, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	//delete[] buf;
}



void ResponseChangeOBState(std::shared_ptr<ClientNode> Client, BOOL OBState)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, OBState); //OBState
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 134, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyChangeOBState(std::shared_ptr<ClientNode> Client, BOOL OBState, UINT Uin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write8(p, OBState); //OBState

	len = p - buf.data();
	SendToClient(Client, 609, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestChangeOBState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	bool OBState = Read8(p);
	ResponseChangeOBState(Client, OBState);
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyChangeOBState(RoomClient, OBState, Uin);
		}
	}
}


std::shared_ptr<RANDOMMAPPOOL> GetRandomMapPool(long PoolID)
{
	std::shared_ptr<RANDOMMAPPOOL> MapPool = NULL;
	auto iter = RandomMapPools.find(PoolID);
	if (iter != RandomMapPools.end())
	{
		MapPool = iter->second;
	}
	return MapPool;
}

PETITEM* GetPetItem(long PetID)
{
	PETITEM* PetItem = NULL;
	auto iter = PetItems.find(PetID);
	if (iter != PetItems.end())
	{
		PetItem = iter->second.get();
	}
	else
	{
	}
	return PetItem;
}




TREASUREBOX* GetTreasureBox(long ID)
{
	TREASUREBOX* TreasureBox = NULL;
	auto iter = TreasureBoxs.find(ID);
	if (iter != TreasureBoxs.end())
	{
		TreasureBox = iter->second.get();
	}
	else
	{
	}
	return TreasureBox;
}

//void CancelRankedMatch(std::shared_ptr<ClientNode> Client)
//{
//	std::lock_guard<std::recursive_mutex> lg(RankedQueueMutex);
//	if (RankedMatchQueue.empty())
//	{
//		return;
//	}
//	for (std::deque<std::shared_ptr<ClientNode>>::iterator it = RankedMatchQueue.begin(); it != RankedMatchQueue.end(); it++)
//	{
//		if ((*it) == Client)
//		{
//			Client->TaskID = 0;
//			RankedMatchQueue.erase(it);
//			break;
//		}
//	}
//}
//void CancelBorderMatch(std::shared_ptr<ClientNode> Client)
//{
//	std::lock_guard<std::recursive_mutex> lg(BorderQueueMutex);
//	if (BorderMatchQueue.empty())
//	{
//		return;
//	}
//	for (std::deque<std::shared_ptr<ClientNode>>::iterator it = BorderMatchQueue.begin(); it != BorderMatchQueue.end(); it++)
//	{
//		if ((*it) == Client)
//		{
//			Client->TaskID = 0;
//			BorderMatchQueue.erase(it);
//			break;
//		}
//	}
//}

void CancelRankedMatch(std::shared_ptr<ClientNode> Client)
{

	std::lock_guard<std::recursive_mutex> lg(RankedQueueMutex);

	if (RankedMatchQueue.empty())
	{
		return;
	}

	for (auto it = RankedMatchQueue.begin(); it != RankedMatchQueue.end(); /* no increment here */)
	{
		if (it->first == Client)
		{
			Client->TaskID = 0; // 假设这个操作是需要的

			it = RankedMatchQueue.erase(it); // 删除元素并更新迭代器
			break; // 删除后退出循环
		}
		else
		{
			++it; // 只有在没有删除元素时才移动迭代器
		}
	}
}

void CancelBorderMatch(std::shared_ptr<ClientNode> Client)
{
	std::lock_guard<std::recursive_mutex> lg(BorderQueueMutex);

	if (BorderMatchQueue.empty())
	{
		return;
	}

	for (auto it = BorderMatchQueue.begin(); it != BorderMatchQueue.end(); /* no increment here */)
	{
		if (it->first == Client)
		{
			Client->TaskID = 0; // 假设这个操作是需要的

			it = BorderMatchQueue.erase(it); // 删除元素并更新迭代器
			break; // 删除后退出循环
		}
		else
		{
			++it; // 只有在没有删除元素时才移动迭代器
		}
	}
}
void RequestCancelMatch(std::shared_ptr<ClientNode> Client)
{
	//BYTE* p = Body; WORD len;
	//UINT Uin = Read32(p);
	//UINT Time = Read32(p);
	if (Client->TaskID == 0xFFFC)
	{
		CancelRankedMatch(Client);
	}
	else if (Client->TaskID == 0xFFFD)
	{
		CancelBorderMatch(Client);
	}

}

void ResponseRandomOneNobleID(std::shared_ptr<ClientNode> Client, UCHAR RandType, UINT RandomID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, RandType); //RandType
	Write32(p, RandomID); //NobleID
	Write8(p, 1); //CanBroadcast

	len = p - buf.data();
	SendToClient(Client, 1652, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestRandomOneNobleID(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR RandType = Read8(p);
	UINT ItemID = 0;
	UINT AddNum = 0;
	std::uniform_int_distribution<unsigned> u(200000000, 800000000);//随机数分布类
	std::default_random_engine e(time(0));
	switch (RandType)
	{
	case 0://普通
		ItemID = 27517;
		AddNum = u(e);
		break;
	case 1://闪亮
		ItemID = 27518;
		AddNum = u(e) + 100000000;
		break;
	}
	NotifyUseItemResult2(Client, ItemID, 1);
	srand((unsigned)time(0));
	UINT RandomID = u(e) + AddNum;
	ResponseRandomOneNobleID(Client, RandType, RandomID);
}
void ResponseCompositeSkillStone(std::shared_ptr<ClientNode> Client, int SkillStoneID, int LuckyStoneID, int BlessStoneID)
{
	

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	UINT GenSkillStoneID = 0;
	char Lv = 0;
	char AddSeed = 0;
	char DeleteStoneNum = 4;
	if (LuckyStoneID == 22076)
	{
		AddSeed += 5;//加概率
		NotifyUseItemResult2(Client, LuckyStoneID, 1);
	}
	if (BlessStoneID == 22073)
	{
		DeleteStoneNum -= 1;//减少宝石消耗一颗
		NotifyUseItemResult2(Client, BlessStoneID, 1);
	}
	NotifyUseItemResult2(Client, SkillStoneID, DeleteStoneNum);

	switch (SkillStoneID)
	{
	case 21983: // 重生宝珠LV1
		GenSkillStoneID = 21984;
		Lv = 0;
		break;
	case 21984: // 重生宝珠LV2
		GenSkillStoneID = 21985;
		Lv = 1;
		break;
	case 21985:// 重生宝珠LV3
		GenSkillStoneID = 21986;
		Lv = 2;
		break;

	case 21977:
		GenSkillStoneID = 21978;
		Lv = 0;
		break; // 效率宝珠LV1
	case 21978:
		GenSkillStoneID = 21979;
		Lv = 1;
		break; // 效率宝珠LV2
	case 21979:
		GenSkillStoneID = 21980;
		Lv = 2;
		break; // 效率宝珠LV3

	case 22037:
		GenSkillStoneID = 22038;
		Lv = 0;
		break; // 攻击之钻LV1
	case 22038:
		GenSkillStoneID = 22039;
		Lv = 1;
		break; // 攻击之钻LV2
	case 22039:
		GenSkillStoneID = 22040;
		Lv = 2;
		break; // 攻击之钻LV3


	case 22043:
		GenSkillStoneID = 22044;
		Lv = 0;
		break; // 复制之钻LV1
	case 22044:
		GenSkillStoneID = 22045;
		Lv = 1;
		break; // 复制之钻LV2
	case 22045:
		GenSkillStoneID = 22046;
		Lv = 2;
		break; // 复制之钻LV3


	case 22031:
		GenSkillStoneID = 22032;
		Lv = 0;
		break; // 闪电之钻LV1
	case 22032:
		GenSkillStoneID = 22033;
		Lv = 1;
		break; // 闪电之钻LV2
	case 22033:
		GenSkillStoneID = 22034;
		Lv = 2;
		break; // 闪电之钻LV3


	case 22025:
		GenSkillStoneID = 22026;
		Lv = 0;
		break; // 复仇之钻LV1
	case 22026:
		GenSkillStoneID = 22027;
		Lv = 1;
		break; // 复仇之钻LV2
	case 22027:
		GenSkillStoneID = 22028;
		Lv = 2;
		break; // 复仇之钻LV3


	case 22013:
		GenSkillStoneID = 22014;
		Lv = 0;
		break; // 后发宝珠LV1
	case 22014:
		GenSkillStoneID = 22015;
		Lv = 1;
		break; // 后发宝珠LV2
	case 22015:
		GenSkillStoneID = 22016;
		Lv = 2;
		break; // 后发宝珠LV3


	case 21989:
		GenSkillStoneID = 21990;
		Lv = 0;
		break; // 坚韧宝珠LV1
	case 21990:
		GenSkillStoneID = 21991;
		Lv = 1;
		break; // 坚韧宝珠LV2
	case 21991:
		GenSkillStoneID = 21992;
		Lv = 2;
		break; // 坚韧宝珠LV3

	}
	char status = 0;
	int SeedTemp = StoneLvSeed[Lv] + AddSeed;
	int Seed = rand() % 100;
	if (Seed < SeedTemp)
	{
		status = 0;//成功
		ItemInfo Items = { 0 };
		Items.ItemID = GenSkillStoneID;
		Items.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, GenSkillStoneID, 1, 0, 0);
		NotifyClientAddItem(Client, 1, { Items });
	}
	else
	{
		status = 1;//失败
	}
	Write16(p, status);					// ResultID  status 0成功 1失败
	Write8(p, 0);						// ReasonLen
	Write32(p, SkillStoneID);			// SkillStoneID	技能石ID
	Write32(p, LuckyStoneID);			// LuckyStoneID  幸运
	Write32(p, BlessStoneID);			// BlessStoneID  祝福
	Write32(p, 4);						// DecSkillStoneNum 减去的宝石数量
	Write32(p, GenSkillStoneID);		// GenSkillStoneID 合成的宝石ID
	Write32(p, 0); // ExperienceCompensation补偿的经验
	Write8(p, 0);						// LeftSkillStoneNumOnFailed_ModifyType 祝福类型

	len = p - buf.data();
	SendToClient(Client, 905, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestCompositeSkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int SkillStoneID = Read32(p);
	int LuckyStoneID = Read32(p);//幸运宝石 22076
	int BlessStoneID = Read32(p);//祝福宝石 22073
	ResponseCompositeSkillStone(Client, SkillStoneID, LuckyStoneID, BlessStoneID);
}

void RequestDropRandomNobleID(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT NobleID = Read32(p);

}

void NotifyClientUpdateNobleInfo(std::shared_ptr<ClientNode> Client, int NobleID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	{ //NobleInfo
		BYTE* pNobleInfo = p;
		Write16(pNobleInfo, 0); //len

		Write32(pNobleInfo, NobleID); //NobleID
		Write8(pNobleInfo, Client->NobleLevel); //NobleLevel
		Write32(pNobleInfo, Client->NoblePoint); //NoblePoint
		Write32(pNobleInfo, -1); //NobleLeftDays

		len = pNobleInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 961, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void ResponseReplaceNobleID(std::shared_ptr<ClientNode> Client, UINT NobleID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write32(p, NobleID); //NobleID

	len = p - buf.data();
	SendToClient(Client, 1654, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestReplaceNobleID(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT NobleID = Read32(p);
	UCHAR NeedBroadcast = Read8(p);

	PlayerDB_UpdateNobel(Client->Uin, NobleID);
	ResponseReplaceNobleID(Client, NobleID);
	NotifyClientUpdateNobleInfo(Client, NobleID);
}


void ResponseRemoveSkillStone(std::shared_ptr<ClientNode> Client, int KartID, int StoneGrooveID, int RemovalToolID, int SkillStoneID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 0);			   // ResultID
	Write32(p, KartID);		   // KartID
	Write32(p, StoneGrooveID); // StoneGrooveID
	Write32(p, RemovalToolID); // RemovalToolID
	Write32(p, SkillStoneID);  // ObtainSkillStoneID

	len = p - buf.data();
	SendToClient(Client, 912, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestRemoveSkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	// WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int KartID = Read32(p);
	int StoneGrooveID = Read32(p);
	int RemovalToolID = Read32(p);


	int SkillStoneID = -1;
	int ID = -1;

	SkillStoneID = PlayerDB_UpdateKartStone(Client->Uin, KartID, StoneGrooveID, 0);

	if (SkillStoneID > 0) {
		int ItemNum = PlayerDB_AddItem2(Client, Uin, SkillStoneID, 1, 0, 0);
		ItemInfo Items = { 0 };
		Items.ItemID = SkillStoneID;
		Items.ItemNum = ItemNum;
		NotifyClientAddItem(Client, 1, { Items });
		NotifyUpdateKartSkillStoneInfo(Client, KartID);
		NotifyUseItemResult2(Client, 22082, 1);
		ResponseRemoveSkillStone(Client, KartID, StoneGrooveID, RemovalToolID, SkillStoneID);
	}

}




void ResponseGetPwdRedEnvelopeContent(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Client->Uin); // Uin
	Write16(p, 0);			 // ResultID
	Write16(p, 0);			 // ContentLen

	len = p - buf.data();
	SendToClient(Client, 24724, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetPwdRedEnvelopeContent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetPwdRedEnvelopeContent(Client);
}
void NotifyClientUpdateMoneyExp(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	{ //CurMoneyExp
		BYTE* pCurMoneyExp = p;
		Write16(pCurMoneyExp, 0); //len

		Write32(pCurMoneyExp, Client->Money); //Money
		Write32(pCurMoneyExp, Client->SuperMoney); //SuperMoney
		Write32(pCurMoneyExp, Client->Experience); //Exp
		Write32(pCurMoneyExp, Client->Charm); //Charm
		Write32(pCurMoneyExp, 0); //DanceExp
		Write32(pCurMoneyExp, 0); //Coupons
		Write32(pCurMoneyExp, 0); //Admiration
		Write32(pCurMoneyExp, 0); //LuckMoney
		Write32(pCurMoneyExp, 0); //TeamWorkExp
		Write32(pCurMoneyExp, 0); //AchievePoint
		Write32(pCurMoneyExp, 0); //ConsumeScore
		Write32(pCurMoneyExp, 0); //HistoryConsumeScore
		Write32(pCurMoneyExp, 0); //LadderMatchAchievePoint
		Write32(pCurMoneyExp, 0); //SpeedBean
		Write32(pCurMoneyExp, 0); //SpeedCoin

		len = pCurMoneyExp - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //Reason

	len = p - buf.data();
	SendToClient(Client, 559, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseSendPwdRedEnvelope(std::shared_ptr<ClientNode> Client, short ResultID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Client->Uin); // Uin
	Write16(p, ResultID);			 // ResultID

	len = p - buf.data();
	SendToClient(Client, 24723, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
BOOL SendPwdRedEnvelope(std::shared_ptr<ClientNode> Client, int KeyIndex)
{
	char FilePath[MAX_PATH];
	std::vector<ItemInfo> Items;
	//printf("KeyIndex:%d \n", KeyIndex);
	sprintf_s(FilePath, ".\\CdkAward\\%d.yml", KeyIndex);
	try
	{
		SelectClientExp(Client);
		YAML::Node config = YAML::LoadFile(FilePath);
		int Experience = config["Experience"].as<int>();
		int SuperMoney = config["SuperMoney"].as<int>();
		int Charm = config["Charm"].as<int>();
		int TotalHonor = config["TotalHonor"].as<int>();
		//Client->Experience += Experience;
		Client->Experience = (Client->Experience > UINT_MAX - Experience) ? UINT_MAX : Client->Experience + Experience;
		Client->SuperMoney += SuperMoney;
		Client->Charm += Charm;
		Client->TotalHonor += TotalHonor;

		UpdateClientBase(Client);
		NotifyClientUpdateMoneyExp(Client);
		//printf("Experience:%d \n", Experience);
		//printf("SuperMoney:%d \n", SuperMoney);
		YAML::Node AwardCfg = config["Items"];
		int allitem = AwardCfg.size();
		if (allitem != 0)
		{
			Items.resize(allitem);
		}
		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return false;
		conn->setAutoCommit(false);
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = AwardCfg[j];//0开头
			Items[j].ItemID = ItemCfg["ItemId"].as<int>();
			Items[j].ItemNum = ItemCfg["ItemNum"].as<int>();
			Items[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			Items[j].ObtainTime = ItemCfg["ObtainTime"].as<int>();
			Items[j].ItemNum = PlayerDB_AddItem1(Client, Client->Uin, Items[j].ItemID, Items[j].ItemNum, Items[j].AvailPeriod, 0, conn);
			ItemCfg.reset();//重置指针 防止混乱
		}
		conn->commit();
		conn->setAutoCommit(true);
		NotifyClientAddItem(Client, allitem, Items);
		return TRUE;
	}
	catch (...)
	{
		printf("Cdk奖励配置异常 请检查!");
	}
	return FALSE;
}

// 函数用于检查字符串是否全为数字
bool isNumeric(const char* str) {
	while (*str != '\0') {
		if (!std::isdigit(*str)) {
			return false;
		}
		str++;
	}
	return true;
}

bool isNumeric1(const std::string& str) {
	// 定义匹配的正则表达式
	std::regex pattern("^[0-9]{5,6}$");

	// 进行匹配
	return std::regex_match(str, pattern);
}

int SendAnyItem(std::shared_ptr<ClientNode> Client, char cdk[]) {
	// Helper function to parse item info from token
	auto parseItemInfo = [](const char* substr, UINT& Itemid, UINT& ItemNum) -> bool {
		if ((std::strlen(substr) == 5 || std::strlen(substr) == 6) && isNumeric(substr)) {
			Itemid = std::strtoul(substr, nullptr, 10);
			return true;
		}
		return false;
		};
	char* context = nullptr; // 上下文指针
	char* token = strtok_s(cdk, "a",&context);
	bool all = false;
	UINT uin = 0;
	UINT Itemid = 0;
	UINT ItemNum = 0;

	if (!token) {
		ResponseStartGame(Client, "输入格式错误!");
		return -1;
	}
	char* substr = token + 4;
	if (isNumeric(substr)) {
		uin = std::strtoul(substr, nullptr, 10);
		token = strtok_s(nullptr, "a", &context);

		if (token && parseItemInfo(token, Itemid, ItemNum)) {
			token = strtok_s(nullptr, "a", &context);
			if (token && isNumeric(token)) {
				ItemNum = std::strtoul(token, nullptr, 10);
			}
			else {
				ResponseStartGame(Client, "输入格式错误!");
				return -1;
			}
		}
		else {
			ResponseStartGame(Client, "输入格式错误!");
			return -1;
		}
	}
	else if (std::strcmp(substr, "one") == 0) {
		all = true;
		token = strtok_s(nullptr, "a", &context);

		if (token && parseItemInfo(token, Itemid, ItemNum)) {
			token = strtok_s(nullptr, "a", &context);
			if (token && isNumeric(token)) {
				ItemNum = std::strtoul(token, nullptr, 10);
			}
			else {
				ResponseStartGame(Client, "输入格式错误!");
				return -1;
			}
		}
		else {
			ResponseStartGame(Client, "输入格式错误!");
			return -1;
		}
	}
	else {
		ResponseStartGame(Client, "输入格式错误!");
		return -1;
	}

	ItemInfo TempItem = { 0 };
	TempItem.ItemID = Itemid;
	TempItem.ItemType = 0;
	TempItem.ItemNum = (ItemNum == 0) ? 1 : ItemNum;
	TempItem.AvailPeriod = (ItemNum == 0) ? -1 : 0;

	if (all) {
		try {
			DBConnection myDBConnection(player);
			auto conn = myDBConnection.getConnection();
			if (!conn) return -1;

			conn->setAutoCommit(false);

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Uin FROM Baseinfo;"));
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				UINT one = res->getUInt("uin");
				PlayerDB_AddItem5(one, TempItem.ItemID, TempItem.ItemNum, TempItem.AvailPeriod, 0, conn);
			}
			conn->commit();
			conn->setAutoCommit(true);
			res.reset();
		}
		catch (const std::exception& e) {
			printf("SendAnyItem Error: %s\n", e.what());
			return -1;
		}
	}
	else {
		auto client = GetClient(uin);
		if (client) {
			TempItem.ItemNum = PlayerDB_AddItem2(client, client->Uin, TempItem.ItemID, TempItem.ItemNum, TempItem.AvailPeriod, 0);
			if (Uinew) {
				NotifyClientAddItemNew(client, 1, { TempItem });
			}
			else {
				NotifyClientAddItem(client, 1, { TempItem });
			}

		}
		else {
			TempItem.ItemNum = PlayerDB_AddItem4(uin, TempItem.ItemID, TempItem.ItemNum, TempItem.AvailPeriod, 0);
		}
	}

	return 1;
}

void RequestSendPwdRedEnvelope(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	char cdk[64] = { 0 };
	char exchangesuccess = 0;
	int KeyIndex = 0;
	int KeyType = 0;
	int KeyPrice = 0;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT ContentLen = Read16(p);

	if (ContentLen <= sizeof(cdk)) {
		memcpy(cdk, p, ContentLen);
	}
	else {
		return;
	}
	// 获取开始时间点
	auto start = std::chrono::high_resolution_clock::now();

	try
	{
		//give10001a127748a100
		//giveonea127748a100
		if (strncmp(cdk, "give", 4) == 0 && Client->admin == 1) {
			if (SendAnyItem(Client, cdk) > 0) {
				ResponseSendPwdRedEnvelope(Client, 0);
			}
			return;
		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("SendAnyItem Error\n");
	}

	if (isLog == 1) {
		auto ServerLogger = spdlog::get("SpeedServer_logger");
		ServerLogger->info("[红包口令][Uin:{:d}][Content:{:s}]", Uin, cdk);
		ServerLogger->flush();
	}

	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select KeyValue,KeyIndex,KeyType,KeyPrice from CdkInfo where KeyValue =?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, cdk);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			res->next();
			exchangesuccess = 1;
			KeyIndex = res->getUInt("KeyIndex");
			KeyType = res->getUInt("KeyType");
			KeyPrice = res->getUInt("KeyPrice");
		}
		else {
			exchangesuccess = 0;
		}
		res.reset();


		char hasAward = 0;
		if (exchangesuccess)
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM CdkRecord Where Uin=? and KeyIndex=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, KeyIndex);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				hasAward = 1;
				break;
			}
			res.reset();

			if (hasAward)
			{
				ResponseStartGame(Client, "你已使用过该Cdk!");
				return;
			}

			BOOL SendOk = SendPwdRedEnvelope(Client, KeyIndex);
			if (SendOk)
			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO CdkRecord VALUES(?, ?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->setUInt(2, KeyIndex);
				prep_stmt->setString(3, cdk);
				prep_stmt->setUInt(4, KeyPrice);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
				if (KeyType == 1)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM CdkInfo  WHERE KeyIndex=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, KeyIndex);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}

				// 获取结束时间点
				auto end = std::chrono::high_resolution_clock::now();

				// 计算时间差
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

				std::cout << "Uin: " << Client->Uin << " CDK:" << cdk << " 兑换成功，消耗时间：" << duration.count() << " 毫秒" << std::endl;

				//printf("Uin:%d CDK:%s 兑换成功，消耗时间：%d 毫秒\n", Client->Uin, cdk, duration.count());
				ResponseSendPwdRedEnvelope(Client, 0);

			}
			else
			{
				printf("Uin:%d CDK:%s KeyIndex:%d 兑换异常\n", Client->Uin, cdk, KeyIndex);
				ResponseStartGame(Client, "兑换异常 请联系管理员!");
			}
		}
		else
		{
			ResponseSendPwdRedEnvelope(Client, 1);
			ResponseStartGame(Client, "CDK已使用或不存在!");
		}


	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "RequestSendPwdRedEnvelope Error" << std::endl;
		//throw;关闭
	}
	catch (...)
	{
		printf("RequestSendPwdRedEnvelope error\n");
		//throw;关闭
	}
}



void RequestCheckName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	char NickName[17];
	std::string newname = "";
	BOOL isNameExist = 0;
	memcpy(NickName, p, 17);
	newname = G2U(NickName);
	UINT ItemNum = 0;

	ItemNum = PlayerDB_GetItemNum(Client->Uin, 10595);
	if (ItemNum <= 0)
	{
		ResponseStartGame(Client, "请购买更名卡!");
		return;
	}

	std::vector<std::string> conditionValues;
	conditionValues.push_back(newname);
	std::vector<int> resultRow = executeQueryRow("SELECT 1 FROM BaseInfo Where NickName=?;", "RequestCheckName", conditionValues);
	if (resultRow.size() >= 1) {
		isNameExist = 1;
	}

	if (isNameExist != 0)
	{
		ResponseStartGame(Client, "用户名已存在!");
	}
	else {
		PlayerDB_UpNickName(Client->Uin, newname);
		ResponseStartGame(Client, "用户名修改成功!");
		NotifyUseItemResult2(Client, 10595, 1);
	}
}


void ResponseBatchDeleteItem(std::shared_ptr<ClientNode> Client, UINT* ItemID, int ItemNum)
{
	if (deleteflag == 1) {
		ResponseStartGame(Client, "目前不允许删除宝贝哦！");
		return;
	}

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	for (size_t i = 0; i < ItemNum; i++)
	{
		if (isLog) {
			auto ServerLogger = spdlog::get("SpeedServer_logger");
			ServerLogger->info("[删除道具：][Uin:{:d}][道具ID:{:d}]", Client->Uin, ItemID[i]);
			ServerLogger->flush();
		}
		PlayerDB_DeleteItem(Client->Uin, ItemID[i]);
	}

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write32(p, ItemNum); //SuccessDeleteItemNum
	for (int i = 0; i < ItemNum; i++)
	{
		Write32(p, ItemID[i]); //SuccessDeleteItemID[]
	}
	Write32(p, 0); //FailDeleteItemNum

	len = p - buf.data();
	SendToClient(Client, 21035, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestBatchDeleteItem(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	UINT ItemID[200] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int ItemNum = Read32(p);
	ItemNum = ItemNum > 200 ? 200 : ItemNum;
	for (int i = 0; i < ItemNum; i++)
	{
		ItemID[i] = Read32(p);
		//printf("ItemID: %d\n", ItemID[i]);
	}
	ResponseBatchDeleteItem(Client, ItemID, ItemNum);
}

const static USHORT DesktopShortCut[64] = {
	0x0074,// 小橘子神灯
	//0x007B,// 魔力圣杯
	0x0081, // 终极魔法阵
	0x0089, // 每日一抽
	0x008a, //奇迹阁
	0x0002
};
void ResponseGetShortCutSet(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	int ShortCutSet = sizeof(DesktopShortCut) / sizeof(DesktopShortCut[0]); //
	Write16(p, 0); //ResultID
	Write8(p, ShortCutSet); // ShortCutSet  快捷方式集
	for (size_t i = 0; i < ShortCutSet; i++)
	{
		Write16(p, DesktopShortCut[i]); // ShortCut[]
	}

	Write8(p, 0); //SuitToServer
	Write8(p, 0); //FixedShortCutNum

	len = p - buf.data();
	SendToClient(Client, 11356, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetShortCutSet(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetShortCutSet(Client);
}




void ResponseGetMagicLampCfg(std::shared_ptr<ClientNode> Client)
{
	if (MagicLampItems.empty())
	{
		return;
	}
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Client->Uin); // Uin
	Write16(p, 0);	 // ResultID
	Write32(p, 0);	 // MagicLampNum
	Write16(p, 0);	 // DescribeLen

	BYTE* pMagicLampItemCfg = p;
	Write16(pMagicLampItemCfg, 0); // len

	Write16(pMagicLampItemCfg, 6); // ItemNum
	for (size_t i = 0; i < 6; i++) {
		int CurrentRound = -1;
		CurrentRound = PlayerDB_GetMagicLampNum(Client->Uin, MagicLampItems[i].ItemID);
		if (CurrentRound == -1)
		{
			CurrentRound = 0;
			PlayerDB_InsertMagicLamp(Client->Uin, MagicLampItems[i].ItemID);
		}
		BYTE* pMagicLampOneItemCfg = pMagicLampItemCfg;
		Write16(pMagicLampOneItemCfg, 0); // len

		Write16(pMagicLampOneItemCfg, 0); // RoundNum
		Write16(pMagicLampOneItemCfg, CurrentRound); // CurrentRound  本轮已点灯个数
		{								  // AwardToClient
			BYTE* pAwardToClient = pMagicLampOneItemCfg;
			Write16(pAwardToClient, 0); // len

			Write32(pAwardToClient, 0); // Exp
			Write32(pAwardToClient, 0); // Money
			Write32(pAwardToClient, 0); // SuperMoney
			Write16(pAwardToClient, 1); // ItemNum
			for (size_t j = 0; j < 1; j++)
			{ // ItemInfo
				BYTE* pItemInfo = pAwardToClient;
				Write16(pItemInfo, 0); // len

				Write32(pItemInfo, MagicLampItems[i].ItemID); // ItemID
				Write32(pItemInfo, MagicLampItems[i].ItemNum);					 // ItemNum
				Write32(pItemInfo, MagicLampItems[i].AvailPeriod);					 // AvailPeriod
				Write8(pItemInfo, 0);					 // Status
				Write32(pItemInfo, 0);					 // ObtainTime
				Write32(pItemInfo, 0);					 // OtherAttribute
				Write16(pItemInfo, MagicLampItems[i].ItemType);					 // ItemType

				len = pItemInfo - pAwardToClient;
				Set16(pAwardToClient, (WORD)len);
				pAwardToClient += len;
			}
			Write32(pAwardToClient, 0); // Coupons
			Write32(pAwardToClient, 0); // GuildPoint
			Write32(pAwardToClient, 0); // LuckMoney
			Write8(pAwardToClient, 0);	// ExtendInfoNum
			Write32(pAwardToClient, 0); // SpeedCoin

			len = pAwardToClient - pMagicLampOneItemCfg;
			Set16(pMagicLampOneItemCfg, (WORD)len);
			pMagicLampOneItemCfg += len;
		}

		Write8(pMagicLampOneItemCfg, 0); // IsCanFree 

		len = pMagicLampOneItemCfg - pMagicLampItemCfg;
		Set16(pMagicLampItemCfg, (WORD)len);
		pMagicLampItemCfg += len;

	}
	len = pMagicLampItemCfg - p;
	Set16(p, (WORD)len);
	p += len;


	Write16(p, 0);			// FreeNum
	Write32(p, 0); // NextFreeTime
	Write32(p, 0);			// MaxFreeNum

	len = p - buf.data();
	SendToClient(Client, 24507, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetMagicLampCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetMagicLampCfg(Client);
}
void ResponseMagicLampLightOn(std::shared_ptr<ClientNode> Client, UCHAR LightType, UINT ItemID)
{


	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	
	int LightOn = 0;
	int LuckyValue = 0;
	int LightNum = 0;
	int SuccessNum = 0;
	char HasFinalAward = 0;
	ItemInfo Item = { 0 };
	switch (LightType)
	{
	case 1:
		LightNum = MagicLamp[2];
		break;
	case 2:
		LightNum = MagicLamp[2];
		break;
	case 3:
		LightNum = MagicLamp[2];
		break;
	}
	if (!isDBtHasItem(Client, MagicLamp[3], LightNum))
	{
		ResponseStartGame(Client, "道具不足!");
		return;
	}

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(ItemID);
	std::vector<int> resultRow = executeQueryRow("select LightOn,LuckyValue from MagicLampInfo where Uin=? and ItemID=?;", "ResponseMagicLampLightOn", conditionValues);
	if (resultRow.size() >= 2) {
		LightOn = resultRow[0];
		LuckyValue = resultRow[1];
	}

	if (LightOn >= 8)
	{
		ResponseStartGame(Client, "点灯数量已达上限!");
		return;
	}

	NotifyUseItemResult2(Client, MagicLamp[3], LightNum);//扣除神灯券

	std::uniform_int_distribution<unsigned> u(0, 100000);
	std::default_random_engine e(time(0));
	int Seed = 0;
	while (LightNum > 0)
	{
		LightNum--;
		Seed = u(e);
		if (Seed < MagicLamp[0])//命中概率
		{
			SuccessNum++;
			LightOn++;
			LuckyValue = 0;//重置幸运值
		}
		else
		{
			if (LuckyValue >= MagicLamp[1])//命中幸运值
			{
				SuccessNum++;
				LightOn++;
				LuckyValue = 0;//重置幸运值
			}
		}
		LuckyValue++;
	}

	if (LightOn >= 8)
	{
		LightOn = 8;
		HasFinalAward = 1;
	}

	PlayerDB_UpdatMagicLampInfo(Client->Uin, ItemID, LightOn, LuckyValue);

	Write32(p, Client->Uin);	  // Uin
	Write16(p, 0);		  // Result
	Write16(p, SuccessNum);		  // SuccNum 
	Write32(p, ItemID);	  // ItemID
	Write8(p, LightType); // LightType
	Write32(p, 0);		  // MagicLampNum
	Write8(p, HasFinalAward);		  // HasFinalAward 
	if (!HasFinalAward)
	{
		Write16(p, 0);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			if (MagicLampItems[i].ItemID == ItemID)
			{
				Item = MagicLampItems[i];
				break;
			}
		}
		Write16(p, 1);		  // AwardNum 奖励信息
		for (size_t i = 0; i < 1; i++)
		{ // AwardToClient
			BYTE* pAwardToClient = p;
			Write16(pAwardToClient, 0); // len

			Write32(pAwardToClient, 0); // Exp
			Write32(pAwardToClient, 0); // Money
			Write32(pAwardToClient, 0); // SuperMoney
			Write16(pAwardToClient, 1); // ItemNum
			for (size_t j = 0; j < 1; j++)
			{ // ItemInfo
				BYTE* pItemInfo = pAwardToClient;
				Write16(pItemInfo, 0); // len

				Write32(pItemInfo, Item.ItemID); // ItemID
				Write32(pItemInfo, Item.ItemNum);	   // ItemNum
				Write32(pItemInfo, Item.AvailPeriod);	   // AvailPeriod
				Write8(pItemInfo, 0);	   // Status
				Write32(pItemInfo, 0);	   // ObtainTime
				Write32(pItemInfo, 0);	   // OtherAttribute
				Write16(pItemInfo, Item.ItemType);	   // ItemType

				len = pItemInfo - pAwardToClient;
				Set16(pAwardToClient, (WORD)len);
				pAwardToClient += len;
			}
			Write32(pAwardToClient, 0); // Coupons
			Write32(pAwardToClient, 0); // GuildPoint
			Write32(pAwardToClient, 0); // LuckMoney
			Write8(pAwardToClient, 0);	// ExtendInfoNum
			Write32(pAwardToClient, 0); // SpeedCoin

			len = pAwardToClient - p;
			Set16(p, (WORD)len);
			p += len;
		}
		PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);
		NotifyClientAddItem(Client, 1, { Item });
	}
	Write8(p, 0);  // CostMagicLampNum 
	Write16(p, 0); // FreeNum       
	Write32(p, 0); // NextFreeTime   

	len = p - buf.data();
	SendToClient(Client, 24508, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestMagicLampLightOn(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR LightType = Read8(p); // 1 点灯1次 2 点灯5次  3 点灯20次
	UINT ItemID = Read32(p);
	ResponseMagicLampLightOn(Client, LightType, ItemID);
}
void ResponseGetCfgFile(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time, UINT FileID, UINT FileVersion, UINT CRC)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Uin);  // Uin
	Write32(p, Time); // Time
	Write16(p, 0);	  // ResultID
	{				  // CfgFileInfos
		BYTE* pCfgFileInfos = p;
		Write16(pCfgFileInfos, 0); // len

		Write32(pCfgFileInfos, FileID);		 // FileID
		Write32(pCfgFileInfos, FileVersion); // FileVersion
		Write32(pCfgFileInfos, CRC);		 // CRC
		Write32(pCfgFileInfos, 0);			 // FileLen
		Write32(pCfgFileInfos, 0);			 // FileZipLen
		Write32(pCfgFileInfos, 0);			 // CurFileZipLen

		len = pCfgFileInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	//Write8(p, 0);  // CommDirectionLen
	WriteString_MC(p, "http://down.qq.com/qqkart/full/commoditylist.202410241812.save.spd");  // CommDirectionLen
	Write32(p, 0x508E335B); // DiscountCardVersion 
	Write32(p, 0x6715BB85); // ReduceCouponsVersion

	len = p - buf.data();
	SendToClient(Client, 20002, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetCfgFile(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	//CfgFileInfos
	BYTE* pCfgFileInfos = p;
	len = Read16(pCfgFileInfos);

	int FileID = Read32(pCfgFileInfos);
	int FileVersion = Read32(pCfgFileInfos);
	int CRC = Read32(pCfgFileInfos);

	p += len;

	ResponseGetCfgFile(Client, Uin, Time, FileID, FileVersion, CRC);
}



void ResponseGetLoveBabyInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Client->Uin); // Uin
	Write16(p, 0);			 // ResultID
	Write32(p, 0);			 // DailyFeedLimitNums
	Write32(p, 0);			 // DailyShowerLimitNums
	Write32(p, 0);			 // DailyFunLimitNums
	Write32(p, 0);			 // DailyFeedNums
	Write32(p, 0);			 // DailyShowerNums
	Write32(p, 0);			 // DailyFunNums
	Write32(p, 0);			 // ShowerAddMentalValue
	Write32(p, 0);			 // FunAddMentalValue
	Write32(p, 0);			 // FeedAddGrowValue
	Write32(p, 0);			 // DailyLikeBabyNum
	Write32(p, 0);			 // RemainingLearningTime
	{						 // LoveBabyInfo
		BYTE* pLoveBabyInfo = p;
		Write16(pLoveBabyInfo, 0); // len

		Write32(pLoveBabyInfo, 71555);		 // LoveBabyID
		Write32(pLoveBabyInfo, Client->Uin); // MasterLoveUin
		Write32(pLoveBabyInfo, Client->EngageUin); // ViceLoveUin
		Write32(pLoveBabyInfo, 100);		 // GrowValue
		Write32(pLoveBabyInfo, 100);		 // MentalValue
		Write32(pLoveBabyInfo, 100);			 // BabyGrowLevel
		Write32(pLoveBabyInfo, 5);			 // BabyProfileLevel
		Write32(pLoveBabyInfo, 0);			 // BabyStrengthLevel
		Write32(pLoveBabyInfo, 0);			 // BabyPLValue
		Write32(pLoveBabyInfo, 0);			 // BabyOrgPLValue
		Write8(pLoveBabyInfo, 16);
		memset(pLoveBabyInfo, 0, 16);
		memcpy(pLoveBabyInfo, "宝宝1", 5); //LoveBabyNickName[]
		pLoveBabyInfo += 16;
		Write16(pLoveBabyInfo, 0);	   // BabyStat
		Write16(pLoveBabyInfo, 0);	   // RemainingActiveDay
		Write16(pLoveBabyInfo, 1);	   // BackGroundID
		Write16(pLoveBabyInfo, 0);	   // LoverBabyGender
		Write16(pLoveBabyInfo, 100);	   // MentalTotalValue
		Write32(pLoveBabyInfo, 100);	   // NextGrowLevelGrowValue
		Write32(pLoveBabyInfo, 71555); // LoverBabyItemID
		Write16(pLoveBabyInfo, Client->LoverBaby1EquipStat);	   // EquipStat
		Write32(pLoveBabyInfo, 1);	   // BabySkillNums
		for (size_t i = 0; i < 1; i++)
		{ // PetSkillList
			BYTE* pPetSkillList = pLoveBabyInfo;
			Write16(pPetSkillList, 0); // len

			Write16(pPetSkillList, 15); // SkillID
			Write8(pPetSkillList, 1);  // Status
			Write8(pPetSkillList, 1);  // Active
			Write32(pPetSkillList, 500); // Value

			len = pPetSkillList - pLoveBabyInfo;
			Set16(pLoveBabyInfo, (WORD)len);
			pLoveBabyInfo += len;
		}
		Write16(pLoveBabyInfo, 0); // TreatMentalValue
		Write32(pLoveBabyInfo, 0); // DailyLikeNum
		Write32(pLoveBabyInfo, 0); // TotalLikeNum
		Write32(pLoveBabyInfo, 0); // DailyLearningNum
		Write32(pLoveBabyInfo, 0); // DaliyLearningLimtNum
		Write32(pLoveBabyInfo, 0); // AlreadyLikeNums
		Write32(pLoveBabyInfo, 0); // RemainingLikeNums
		Write16(pLoveBabyInfo, 5); // BabyTransferdProfileLevel
		Write16(pLoveBabyInfo, 0); // HaveBirthPresent

		len = pLoveBabyInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // SecondBabyInfo
		BYTE* pSecondBabyInfo = p;
		Write16(pSecondBabyInfo, 0); // len

		Write32(pSecondBabyInfo, 0); // DailyFeedLimitNums
		Write32(pSecondBabyInfo, 0); // DailyShowerLimitNums
		Write32(pSecondBabyInfo, 0); // DailyFunLimitNums
		Write32(pSecondBabyInfo, 0); // DailyFeedNums
		Write32(pSecondBabyInfo, 0); // DailyShowerNums
		Write32(pSecondBabyInfo, 0); // DailyFunNums
		Write32(pSecondBabyInfo, 0); // ShowerAddMentalValue
		Write32(pSecondBabyInfo, 0); // FunAddMentalValue
		Write32(pSecondBabyInfo, 0); // FeedAddGrowValue
		Write32(pSecondBabyInfo, 0); // DailyLikeBabyNum
		Write32(pSecondBabyInfo, 0); // RemainingLearningTime
		{							 // LoveBabyInfo
			BYTE* pLoveBabyInfo = pSecondBabyInfo;
			Write16(pLoveBabyInfo, 0); // len

			Write32(pLoveBabyInfo, 71554); // LoveBabyID
			Write32(pLoveBabyInfo, Client->Uin); // MasterLoveUin
			Write32(pLoveBabyInfo, Client->EngageUin); // ViceLoveUin
			Write32(pLoveBabyInfo, 5);		 // GrowValue
			Write32(pLoveBabyInfo, 5);		 // MentalValue
			Write32(pLoveBabyInfo, 100);			 // BabyGrowLevel
			Write32(pLoveBabyInfo, 5);			 // BabyProfileLevel
			Write32(pLoveBabyInfo, 5);			 // BabyStrengthLevel
			Write32(pLoveBabyInfo, 0); // BabyPLValue
			Write32(pLoveBabyInfo, 0); // BabyOrgPLValue
			Write8(pLoveBabyInfo, 16);
			memset(pLoveBabyInfo, 0, 16);
			memcpy(pLoveBabyInfo, "宝宝2", 5); //LoveBabyNickName[]
			pLoveBabyInfo += 16;
			Write16(pLoveBabyInfo, 0); // BabyStat
			Write16(pLoveBabyInfo, 0); // RemainingActiveDay
			Write16(pLoveBabyInfo, 1); // BackGroundID
			Write16(pLoveBabyInfo, 1); // LoverBabyGender
			Write16(pLoveBabyInfo, 0); // MentalTotalValue
			Write32(pLoveBabyInfo, 0); // NextGrowLevelGrowValue
			Write32(pLoveBabyInfo, 71554); // LoverBabyItemID
			Write16(pLoveBabyInfo, Client->LoverBaby2EquipStat); // EquipStat
			Write32(pLoveBabyInfo, 1); // BabySkillNums
			for (size_t i = 0; i < 1; i++)
			{ // PetSkillList
				BYTE* pPetSkillList = pLoveBabyInfo;
				Write16(pPetSkillList, 0); // len

				Write16(pPetSkillList, 15); // SkillID
				Write8(pPetSkillList, 1);  // Status
				Write8(pPetSkillList, 1);  // Active
				Write32(pPetSkillList, 500); // Value

				len = pPetSkillList - pLoveBabyInfo;
				Set16(pLoveBabyInfo, (WORD)len);
				pLoveBabyInfo += len;
			}
			Write16(pLoveBabyInfo, 0); // TreatMentalValue
			Write32(pLoveBabyInfo, 0); // DailyLikeNum
			Write32(pLoveBabyInfo, 0); // TotalLikeNum
			Write32(pLoveBabyInfo, 0); // DailyLearningNum
			Write32(pLoveBabyInfo, 0); // DaliyLearningLimtNum
			Write32(pLoveBabyInfo, 0); // AlreadyLikeNums
			Write32(pLoveBabyInfo, 0); // RemainingLikeNums
			Write16(pLoveBabyInfo, 5); // BabyTransferdProfileLevel
			Write16(pLoveBabyInfo, 0); // HaveBirthPresent

			len = pLoveBabyInfo - pSecondBabyInfo;
			Set16(pSecondBabyInfo, (WORD)len);
			pSecondBabyInfo += len;
		}

		len = pSecondBabyInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, Client->LoverBaby2EquipStat + Client->LoverBaby1EquipStat);  // EquipBaby
	Write16(p, 0); // ActivedExtralSkillNums
	for (size_t i = 0; i < 0; i++)
	{ // BabyExtralSkillInfos
		BYTE* pBabyExtralSkillInfos = p;
		Write16(pBabyExtralSkillInfos, 0); // len

		Write8(pBabyExtralSkillInfos, i);  // BabySeq
		Write16(pBabyExtralSkillInfos, 1); // SkillID

		len = pBabyExtralSkillInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 24352, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetLoveBabyInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT Flag = Read32(p);
	//const char* buf = "\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x71\x00\x01\x17\x83\x00\x00\x27\x11\x00\x00\x27\x12\x00\x00\x00\x64\x00\x00\x00\x64\x00\x00\x00\x64\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xd2\xbb\xba\xc5\xb1\xa6\xb1\xa6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x64\x00\x00\x00\x64\x00\x01\x17\x83\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x9f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x71\x00\x01\x17\x82\x00\x00\x27\x11\x00\x00\x27\x12\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x64\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x10\xb6\xfe\xba\xc5\xb1\xa6\xb1\xa6\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x17\x82\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00";
	//SendToClient(Client, 24352, (BYTE*)buf, 327, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	ResponseGetLoveBabyInfo(Client);
}



void ResponseChangeLoverBabyEquipStatus(std::shared_ptr<ClientNode> Client, short EquipStatus)
{
	
	if (EquipStatus == 0) {
		if (Client->LoverBaby1EquipStat == 1) {
			Client->LoverBaby1EquipStat = 0;
		}
		else if (Client->LoverBaby2EquipStat == 1) {
			Client->LoverBaby2EquipStat = 0;
		}
	}
	else if (EquipStatus == 1) {
		Client->LoverBaby1EquipStat = 1;
		Client->LoverBaby2EquipStat = 0;
	}
	else if (EquipStatus == 2) {
		Client->LoverBaby1EquipStat = 0;
		Client->LoverBaby2EquipStat = 1;
	}


	PlayerDB_UpdateEngageInfoStat(Client);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResutID

	Write16(p, EquipStatus); //EquipStat



	len = p - buf.data();
	SendToClient(Client, 24386, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestChangeLoverBabyEquipStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short EquipStatus = Read16(p);
	ResponseChangeLoverBabyEquipStatus(Client, EquipStatus);
}


void ResponseGetMagicCupCfg(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	TREASUREBOX* TreasureBox = GetTreasureBox(24726);
	if (!TreasureBox)
	{
		return;
	}
	std::vector<TreasureAward> TreasureAward = TreasureBox->TreasureAwards;
	if (TreasureAward.empty())
	{
		return;
	}
	Write32(p, Client->Uin); //Uin
	Write16(p, 0);	 // ResultID
	{				 // MagicCupSeasonCfg   
		BYTE* pMagicCupSeasonCfg = p;
		Write16(pMagicCupSeasonCfg, 0); // len

		Write32(pMagicCupSeasonCfg, 1);			 // SeasonID
		Write32(pMagicCupSeasonCfg, 1678084039); // OpenDate
		Write32(pMagicCupSeasonCfg, 1678084039); // OpenTime
		Write32(pMagicCupSeasonCfg, 3382237639); // CloseDate
		Write32(pMagicCupSeasonCfg, 3382237639); // CloseTime

		len = pMagicCupSeasonCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	int CostItemNum[] = { 1,10,45 };
	Write32(p, 3); // LotteryDrawCfgNum  
	for (size_t i = 0; i < 3; i++)
	{ // LotteryDrawCfg
		BYTE* pLotteryDrawCfg = p;
		Write16(pLotteryDrawCfg, 0); // len

		Write32(pLotteryDrawCfg, 127748);			   // CostItemID  
		Write32(pLotteryDrawCfg, CostItemNum[i]); // CostItemNum 
		Write32(pLotteryDrawCfg, 0);		   // ProduceItemNum
		Write32(pLotteryDrawCfg, 0);				   // AlreadyGotNum

		len = pLotteryDrawCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, TreasureAward[0].AwardItems.size()); //AwardItemPoll 
	for (size_t i = 0; i < TreasureAward[0].AwardItems.size(); i++)
	{ //AwardItemPollInfo
		BYTE* pAwardItemPollInfo = p;
		Write16(pAwardItemPollInfo, 0); //len

		Write32(pAwardItemPollInfo, TreasureAward[0].AwardItems[i].ItemID); //ItemID
		Write32(pAwardItemPollInfo, TreasureAward[0].AwardItems[i].ItemNum); //ItemNum
		Write32(pAwardItemPollInfo, TreasureAward[0].AwardItems[i].AvailPeriod); //AvailPeriod
		Write8(pAwardItemPollInfo, 0); //Status
		Write32(pAwardItemPollInfo, 0); //ObtainTime
		Write32(pAwardItemPollInfo, 0); //OtherAttribute
		Write16(pAwardItemPollInfo, TreasureAward[0].ItemType); //ItemType

		len = pAwardItemPollInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //PieceItemNum
	//Write32(p, 0); //PieceItemIDList[]

	len = p - buf.data();
	SendToClient(Client, 24726, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetMagicCupCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetMagicCupCfg(Client);
}

void ResponseGetTempBoxItemInfo(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo>& Items, int Num)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write16(p, 0); //CritItemNum
	Write16(p, Num); //TempBoxItemNum 

	for (size_t i = 0; i < Num; i++)
	{ //MagicCupTempBoxItemInfo 
		BYTE* pMagicCupTempBoxItemInfo = p;
		Write16(pMagicCupTempBoxItemInfo, 0); //len

		Write32(pMagicCupTempBoxItemInfo, Items[i].ItemID); //CostItemID 
		Write32(pMagicCupTempBoxItemInfo, Items[i].ItemType); //ItemType
		Write32(pMagicCupTempBoxItemInfo, 81012); //ProducePieceID 
		Write32(pMagicCupTempBoxItemInfo, MagicCupExchange[GetItemType(Items[i].ItemID)] * Items[i].ItemNum); //ProducePieceNum 
		Write32(pMagicCupTempBoxItemInfo, 0); //CostItemRecProb

		len = pMagicCupTempBoxItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 24727, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetTempBoxItemInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int num = 0;
	std::vector<ItemInfo> Items(1000);
	num = PlayerDB_GetMagicCupTempBox(Uin, Items);
	ResponseGetTempBoxItemInfo(Client, Items, num);
}

void ResponseMagicCupGetAward(std::shared_ptr<ClientNode> Client, short Type, std::vector<ItemInfo> TempItem, int ItemNum)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write16(p, Type); //Type
	Write16(p, 0); //TempBoxItemNum
	{ //AwardToClient
		BYTE* pAwardToClient = p;
		Write16(pAwardToClient, 0); //len

		Write32(pAwardToClient, 0); //Exp
		Write32(pAwardToClient, 0); //Money
		Write32(pAwardToClient, 0); //SuperMoney
		Write16(pAwardToClient, ItemNum); //ItemNum
		for (size_t i = 0; i < ItemNum; i++)
		{ //ItemInfo
			BYTE* pItemInfo = pAwardToClient;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, TempItem[i].ItemID); //ItemID
			Write32(pItemInfo, TempItem[i].ItemNum); //ItemNum
			Write32(pItemInfo, TempItem[i].AvailPeriod); //AvailPeriod
			Write8(pItemInfo, 0); //Status
			Write32(pItemInfo, 0); //ObtainTime
			Write32(pItemInfo, 0); //OtherAttribute
			Write16(pItemInfo, TempItem[i].ItemType); //ItemType

			len = pItemInfo - pAwardToClient;
			Set16(pAwardToClient, (WORD)len);
			pAwardToClient += len;
		}
		Write32(pAwardToClient, 0); //Coupons
		Write32(pAwardToClient, 0); //GuildPoint
		Write32(pAwardToClient, 0); //LuckMoney
		Write8(pAwardToClient, 0); //ExtendInfoNum
		Write32(pAwardToClient, 0); //SpeedCoin

		len = pAwardToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 24728, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}
void RequestMagicCupGetAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short Type = Read16(p);
	int temp = 0;
	if (Type == 0)
	{
		temp = MagicCup[1];
	}
	else if (Type == 1)
	{
		temp = MagicCup[1];
	}
	else if (Type == 2)
	{
		temp = MagicCup[1];
	}


	if (!isDBtHasItem(Client, MagicCup[0], MagicCup[1]))
	{
		ResponseStartGame(Client, "缺少道具!");
		return;
	}
	NotifyUseItemResult2(Client, MagicCup[0], MagicCup[1]);
	if (temp > 50) {
		temp = 50;
	}
	/*if (Type == 2)
	{
		temp = 50;
	}*/
	std::vector<ItemInfo> TempItems;
	bool result = GetRandItem(Client, 24726, temp, TempItems);
	if (result)
	{
		ResponseMagicCupGetAward(Client, Type, TempItems, temp);
		//加入魔法盒
		for (int i = 0; i < temp; i++)
		{
			PlayerDB_AddMagicCupTempBox(Client, Client->Uin, TempItems[i].ItemID, TempItems[i].ItemNum, TempItems[i].AvailPeriod, TempItems[i].ItemType);
		}
	}
}

void ResponseMagicCupExchangePiece(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, int ItemNum, short Type)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write16(p, ItemNum); //AwardItemNum
	for (size_t i = 0; i < ItemNum; i++)
	{ //AwardItemInfo
		BYTE* pAwardItemInfo = p;
		Write16(pAwardItemInfo, 0); //len

		Write32(pAwardItemInfo, Items[i].ItemID); //ItemID
		Write32(pAwardItemInfo, Items[i].ItemNum); //ItemNum
		Write32(pAwardItemInfo, Items[i].AvailPeriod); //AvailPeriod
		Write8(pAwardItemInfo, 0); //Status
		Write32(pAwardItemInfo, 0); //ObtainTime
		Write32(pAwardItemInfo, 0); //OtherAttribute
		Write16(pAwardItemInfo, Items[i].ItemType); //ItemType

		len = pAwardItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, Type); //Type

	len = p - buf.data();
	SendToClient(Client, 24729, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void ResponseWareHouseOperation(std::shared_ptr<ClientNode> Client, int ItemID, bool OpType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	UINT ItemNum = 0;
	char AvailPeriod = 0;
	

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(ItemID);
	std::vector<int> resultRow = executeQueryRow("select ItemNum,AvailPeriod from Item WHERE Uin=? AND ItemID=?;", "ResponseWareHouseOperation", conditionValues);
	if (resultRow.size() >= 2) {
		ItemNum = resultRow[0];
		AvailPeriod = resultRow[1];
	}

	Write16(p, 0);	   // ResultID
	Write8(p, OpType); // OpType
	{				   // ItemInfo
		BYTE* pItemInfo = p;
		Write16(pItemInfo, 0); // len

		Write32(pItemInfo, ItemID); // ItemID
		Write32(pItemInfo, ItemNum);		// ItemNum
		Write32(pItemInfo, AvailPeriod);		// AvailPeriod
		Write8(pItemInfo, 0);		// Status
		Write32(pItemInfo, time(0));		// ObtainTime
		Write32(pItemInfo, 0);		// OtherAttribute
		Write16(pItemInfo, 0);		// ItemType

		len = pItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 422, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void ResponseGetWareHouseStatus(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write8(p, 1);	   // Status
	Write16(p, 200); // MaxSize

	len = p - buf.data();
	SendToClient(Client, 420, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetWareHouseStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetWareHouseStatus(Client);
}
void ResponseGetWareHouseItemList(std::shared_ptr<ClientNode> Client)
{

	std::vector<ItemInfo> Items(600);
	size_t ItemNum = 0;
	bool HasMoreInfo = false;

	ItemNum = PlayerDB_SelectWareHouseItem(Client->Uin, Items);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;

	Write16(p, ItemNum); // ItemNum
	for (size_t i = 0; i < ItemNum; i++)
	{ // WareHouseItems
		BYTE* pWareHouseItems = p;
		Write16(pWareHouseItems, 0); // len

		Write32(pWareHouseItems, Items[i].ItemID);		   // ItemID
		Write32(pWareHouseItems, Items[i].ItemNum);		   // ItemNum
		Write32(pWareHouseItems, Items[i].AvailPeriod);	   // AvailPeriod
		Write8(pWareHouseItems, Items[i].Status);		   // Status
		Write32(pWareHouseItems, Items[i].ObtainTime);	   // ObtainTime
		Write32(pWareHouseItems, Items[i].OtherAttribute); // OtherAttribute
		Write16(pWareHouseItems, Items[i].ItemType);	   // ItemType

		len = pWareHouseItems - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); // HasMoreInfo

	len = p - buf.data();
	SendToClient(Client, 421, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetWareHouseItemList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetWareHouseItemList(Client);
}
void RequestDoWareHouseOperation(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int ItemID = Read32(p);
	bool OpType = Read8(p); // 1 取出 0 放入

	PlayerDB_UpdateItemWareHouse(Client->Uin, ItemID, OpType);
	ResponseWareHouseOperation(Client, ItemID, OpType);
}
void RequestDoWareHouseOperationBatch(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int ItemNum = Read32(p);
	ItemStatus Items[200];
	for (int i = 0; i < ItemNum; i++)
	{
		Items[i].ItemID = Read32(p);
	}
	bool OpType = Read8(p);
	
	
	for (int i = 0; i < ItemNum; i++) {
		PlayerDB_UpdateItemWareHouse(Client->Uin, Items[i].ItemID, OpType);
	}

	for (int i = 0; i < ItemNum; i++)
	{
		ResponseWareHouseOperation(Client, Items[i].ItemID, OpType);
	}

}

void RequestMagicCupExchangePiece(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short ItemNum = Read16(p);
	std::vector<ItemInfo> Items(ItemNum);
	for (int i = 0; i < ItemNum; i++)
	{
		Items[i].ItemID = Read32(p);
	}
	short Type = Read16(p);

	for (int i = 0; i < ItemNum; i++) {
		Items[i].ItemNum = PlayerDB_GetMagicCupTempBoxNum(Client->Uin, Items[i].ItemID);
	}

	if (Type == 0)
	{//分解
		ItemInfo PieceItem = { 0 };
		PieceItem.ItemID = 81012;
		PieceItem.ItemType = 2;
		for (int i = 0; i < ItemNum; i++)
		{
			PieceItem.ItemNum += (Items[i].ItemNum * MagicCupExchange[GetItemType(Items[i].ItemID)]);
		}

		ResponseMagicCupExchangePiece(Client, { PieceItem }, 1, Type);

		PieceItem.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, PieceItem.ItemID, PieceItem.ItemNum, PieceItem.AvailPeriod, 0);
		NotifyClientAddItem(Client, 1, { PieceItem });
	}
	else if (Type == 1)//领取
	{
		ResponseMagicCupExchangePiece(Client, Items, ItemNum, Type);
		for (int i = 0; i < ItemNum; i++)
		{
			Items[i].ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Items[i].ItemID, Items[i].ItemNum, Items[i].AvailPeriod, 0);
		}
		NotifyClientAddItem(Client, ItemNum, Items);
	}

	for (int i = 0; i < ItemNum; i++) {
		PlayerDB_DeleteMagicCupTempBox(Uin, Items[i].ItemID);
	}
}

void ResponseGetCommonBoxAward(std::shared_ptr<ClientNode> Client, UINT Uin, int AwardIdx, USHORT Type, int PosKey)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, 0); //ResultID
	Write32(p, AwardIdx); //AwardIdx
	Write16(p, Type); //Type
	{ //Award
		BYTE* pAward = p;
		Write16(pAward, 0); //len

		Write32(pAward, 0); //Exp
		Write32(pAward, 0); //Money
		Write32(pAward, 0); //SuperMoney
		Write16(pAward, 1); //ItemNum
		for (size_t i = 0; i < 1; i++)
		{ //ItemInfo
			BYTE* pItemInfo = pAward;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, MiracleCfg->MiracleItems[AwardIdx].ItemID); //ItemID
			Write32(pItemInfo, MiracleCfg->MiracleItems[AwardIdx].ItemNum); //ItemNum
			Write32(pItemInfo, MiracleCfg->MiracleItems[AwardIdx].AvailPeriod); //AvailPeriod
			Write8(pItemInfo, 0); //Status
			Write32(pItemInfo, 0); //ObtainTime
			Write32(pItemInfo, 0); //OtherAttribute
			Write16(pItemInfo, MiracleCfg->MiracleItems[AwardIdx].ItemType); //ItemType

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
	Write32(p, PosKey); //PosKey
	Write32(p, 0); //Score
	Write8(p, 0); //ParaNum
	//Write32(p, 0); //ParaList[]

	len = p - buf.data();
	SendToClient(Client, 24466, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetCommonBoxAward(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//PrintfBody(Client, 20101, Body, BodyLen);
	
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int AwardIdx = Read32(p);
	/*int AwardIdx = Read8(p);
	Read8(p);
	Read8(p);
	Read8(p);*/
	USHORT Type = Read16(p);
	int PosKey = Read32(p);
	int ItemNum = 0;
	//printf("AwardIdx:%d\n", AwardIdx);


	if (Type == 45) {
		if (AwardIdx >= MiracleCfg->MiracleItems.size()) {
			return;
		}
		ItemNum = PlayerDB_GetItemNum(Client->Uin, MiracleCfg->MiracleItems[AwardIdx].ComsumeItemId);

		if (ItemNum >= MiracleCfg->MiracleItems[AwardIdx].ComsumeItemNum)
		{

			ResponseGetCommonBoxAward(Client, Uin, AwardIdx, Type, PosKey);
			NotifyUseItemResult2(Client, MiracleCfg->MiracleItems[AwardIdx].ComsumeItemId, MiracleCfg->MiracleItems[AwardIdx].ComsumeItemNum);
			ItemInfo TempItem = { 0 };
			TempItem.ItemID = MiracleCfg->MiracleItems[AwardIdx].ItemID;
			TempItem.ItemNum = MiracleCfg->MiracleItems[AwardIdx].ItemNum;
			TempItem.ItemType = MiracleCfg->MiracleItems[AwardIdx].ItemType;
			TempItem.AvailPeriod = MiracleCfg->MiracleItems[AwardIdx].AvailPeriod;
			TempItem.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, TempItem.ItemID, TempItem.ItemNum, TempItem.AvailPeriod, 0);
			NotifyClientAddItem(Client, 1, { TempItem });

		}
		else
		{
			ResponseStartGame(Client, "缺少道具!");
		}
	}
	else if (Type == 312)
	{
		BYTE* p = Body;

		UINT Uin = Read32(p);
		UINT Time = Read32(p);
		UINT AwardIdx = Read32(p);
		USHORT Type = Read16(p);
		int PosKey = Read32(p);
		UINT ItemId = 0;
		UINT ItemNum = 0;
		INT AvailPeriod = 0;
		if (CommonBoxAwards[Type].size() - 1 >= PosKey) {
			CarTreasureInfo CarTs = CommonBoxAwards[Type][PosKey];
			if (Type == 312) {
				int ItemSpNum = PlayerDB_GetItemNum(Client->Uin, 94880);
				if (CarTs.ExchangePieceNum > ItemSpNum) {//碎片不够
					ResponseStartGame(Client, "夺宝碎片不足");
					return;
				}
				else {//兑换扣除碎片
					ItemId = CarTs.ItemId;
					ItemNum = CarTs.ItemNum;
					AvailPeriod = CarTs.AvailPeriod;
					PlayerDB_Reduce(Client->Uin, 94880, CarTs.ExchangePieceNum);
					NotifyUseItemResult(Client, 94880, CarTs.ExchangePieceNum, ItemSpNum - CarTs.ExchangePieceNum);
				}
			}
		}
		else {
			ResponseStartGame(Client, "无效的物品");
			return;
		}

		//printf("RequestGetCommonBoxAward PosKey:%d,Type:%d\n", PosKey,Type);
		{
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

			Write32(p, Client->Uin); //Uin
			Write16(p, 0); //ResultID
			Write32(p, PosKey); //AwardIdx //PosKey
			Write16(p, Type); //Type
			{ //Award
				BYTE* pAward = p;
				Write8(pAward, 0); //len

				Write32(pAward, 0); //Exp
				Write32(pAward, 0); //Money
				Write32(pAward, 0); //SuperMoney
				Write16(pAward, 0); //ItemNum
				for (size_t i = 0; i < 0; i++)
				{ //ItemInfo
					BYTE* pItemInfo = pAward;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, 0); //ItemID
					Write32(pItemInfo, 0); //ItemNum
					Write32(pItemInfo, 0); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					Write32(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, 0); //ItemType

					len = pItemInfo - pAward;
					Set16(pAward, (WORD)len);
					pAward += len;
				}
				Write32(pAward, 0); //Coupons
				Write32(pAward, 0); //GuildPoint
				Write32(pAward, 0); //LuckMoney
				Write8(pAward, 0); //ExtendInfoNum
				for (size_t i = 0; i < 0; i++)
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

				len = pAward - p;
				Set16(p, (WORD)len);
				p += len;
			}
			Write32(p, PosKey); //PosKey
			Write32(p, 0); //Score
			Write32(p, 0); //ParaNum
			//Write32(p, 0); //ParaList[]
			Write8(p, 0);
			Write16(p, 0);
			len = p - buf.data();
			SendToClient(Client, 24466, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
			//delete[] buf;
			if (ItemId > 0) {
				PlayerDB_AddItem2(Client, Client->Uin, ItemId, ItemNum, AvailPeriod, false);
				{
					std::vector<BYTE> buf(819200); BYTE* p = buf.data();
					size_t len;

					Write32(p, Client->Uin); // Uin
					Write32(p, Client->Uin); // SrcUin
					Write16(p, 1);	 // ItemNum
					{
						for (size_t i = 0; i < 1; i++)
						{
							BYTE* pItemInfo = p;
							Write16(pItemInfo, 0); // len

							Write32(pItemInfo, ItemId);
							Write32(pItemInfo, ItemNum);
							Write32(pItemInfo, AvailPeriod);
							Write8(pItemInfo, 0);
							Write32(pItemInfo, (DWORD)time(nullptr));
							Write32(pItemInfo, 0);
							Write16(pItemInfo, 0); // ItemType
							len = pItemInfo - p;
							Set16(p, (WORD)len);
							p += len;
						}
					}
					Write16(p, 0); // AttachLen
					Write16(p, 0); // AddKartNum
					Write8(p, 0); // IncRedPacketNum

					len = p - buf.data();
					SendToClient(Client, 529, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify); //delete[] buf;
				}
			}
		}

	}


}


void ResponseGetGuildList(std::shared_ptr<ClientNode> Client)
{

	std::string GuildName = "";

	int GuildCount = 0;

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, 0); //StartIdx
	Write32(p, 0); //TotalCount

	BYTE* pGuildCount = p;
	Write32(p, 0); //GuildCount


	Guidinfo* guidinfo = nullptr;
	GuildCount = PlayerDB_SelectGuildInfoAll(guidinfo);
	for (size_t i = 0; i < GuildCount; i++)
	{
		GuildName = guidinfo[i].Name;
		
		BYTE* pGuildDescInfo = p;
		Write16(pGuildDescInfo, 0); //len

		Write32(pGuildDescInfo, guidinfo[i].ID); //GuildID
		memset(pGuildDescInfo, 0, 17);
		if (!GuildName.empty())
		{
			memcpy(pGuildDescInfo, GuildName.c_str(), GuildName.length());
		}
		pGuildDescInfo += 17;
		Write32(pGuildDescInfo, i + 1);//Level 
		Write32(pGuildDescInfo, guidinfo[i].Score); //Score
		Write32(pGuildDescInfo, 0); //CreateTime
		Write16(pGuildDescInfo, guidinfo[i].Attribute); //Attribute
		Write32(pGuildDescInfo, guidinfo[i].MaxMember);//MaxMember 
		Write32(pGuildDescInfo, guidinfo[i].CurrentMember); //CurrentMember
		Write32(pGuildDescInfo, guidinfo[i].Prosperity); //Prosperity
		Write16(pGuildDescInfo, guidinfo[i].Level); //Level 
		{ //GuildEnlistCond
			BYTE* pGuildEnlistCond = pGuildDescInfo;
			Write16(pGuildEnlistCond, 0); //len 

			Write32(pGuildEnlistCond, 0); //ProsperityCondition
			Write32(pGuildEnlistCond, 0); //JoinTotalCondition
			Write32(pGuildEnlistCond, 0); //HonorCondition
			Write32(pGuildEnlistCond, 0); //CharmCondition
			Write32(pGuildEnlistCond, 0); //DanceExpCondition
			Write32(pGuildEnlistCond, 0); //SkillPointCondition
			Write32(pGuildEnlistCond, 0); //ExpLevelCondition
			Write32(pGuildEnlistCond, 0); //ApplyExpLevelCondition

			Write8(pGuildEnlistCond, guidinfo[i].EnlistType); //EnlistType

			len = pGuildEnlistCond - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		Write8(pGuildDescInfo, 0); //SubType
		{ //CardOtherInfo
			BYTE* pCardOtherInfo = pGuildDescInfo;
			Write16(pCardOtherInfo, 0); //len

			Write32(pCardOtherInfo, 0); //MainZone1
			Write32(pCardOtherInfo, 0); //MainZone2
			Write32(pCardOtherInfo, 0); //MainMode1
			Write32(pCardOtherInfo, 0); //MainMode2
			Write32(pCardOtherInfo, 0); //OtherInterest1
			Write32(pCardOtherInfo, 0); //OtherInterest2

			len = pCardOtherInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		Write16(pGuildDescInfo, 0); //DescLen
		Write32(pGuildDescInfo, 0); //WeekProsper
		Write32(pGuildDescInfo, 0); //GuildQQGroup
		Write8(pGuildDescInfo, 0); //RecommendType
		Write16(pGuildDescInfo, 0); //FriendNums
		Write16(pGuildDescInfo, 0); //DCRecommendIndex

		len = pGuildDescInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	if (guidinfo) {
		delete[] guidinfo;
	}
	Set32(pGuildCount, GuildCount);

	Write16(p, 0); //ResultID
	Write16(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 15004, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	//delete[] buf;
}
void RequestGetGuildList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT StartIdx = Read32(p);
	UINT GuildCount = Read32(p);
	ResponseGetGuildList(Client);
}

void ResponseCreateGuild(std::shared_ptr<ClientNode> Client, char* GuildName, int GuildID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	memset(p, 0, 17);
	memcpy(p, GuildName, 17);
	p += 17;
	Write32(p, GuildID); //GuildID
	Write16(p, 0); //ResultID
	Write16(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 15000, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestCreateGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	if (!isDBtHasItem(Client, 11362, 1))
	{
		ResponseStartGame(Client, "车队徽章不足创建失败!");
		return;
	}
	BYTE* p = Body;
	char NickName[17] = { 0 };
	char GuildName[17] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	memcpy(NickName, p, 17);
	p += 17;
	memcpy(GuildName, p, 17);
	p += 17;
	//UINT Guid = Read32(p);
	int GuildID = 0;
	//char* newNickName = NULL;
	//newNickName = G2U(NickName);
	char* newGuildname = NULL;
	newGuildname = G2U(GuildName);

	
	BOOL HasGuild = 0;

	std::vector<int> conditionValues;
	conditionValues.push_back(Uin);
	std::vector<int> resultRow = executeQueryRow("SELECT 1 FROM GuildInfo where Uin=?;", "RequestCreateGuild", conditionValues);
	if (resultRow.size() >= 1) {
		HasGuild = 1;
	}

	if (HasGuild)
	{
		return;
	}
	HasGuild = 0;

	if (newGuildname) {
		resultRow.clear();
		std::vector<std::string> conditionValuestr;
		conditionValuestr.push_back(newGuildname);
		resultRow = executeQueryRow("SELECT 1 FROM GuildList where Name=?;", "RequestCreateGuild", conditionValuestr);
		if (resultRow.size() >= 1) {
			HasGuild = 1;
		}
	}


	if (!HasGuild)
	{
		NotifyUseItemResult2(Client, 11362, 1);
		PlayerDB_InsertGuildList(Uin, newGuildname);
		if (newGuildname) delete[] newGuildname;

		resultRow.clear();
		conditionValues.clear();
		conditionValues.push_back(Uin);
		std::vector<int> resultRow = executeQueryRow("SELECT ID from GuildList where LeaderUin=?;", "RequestCreateGuild", conditionValues);
		if (resultRow.size() >= 1) {
			GuildID = resultRow[0];
		}

		PlayerDB_InsertGuildInfo(Uin, GuildID, 0);
		Client->GuildDuty = 0;
		Client->GuildID = GuildID;
		memcpy(Client->GuildName, GuildName, 17);//更新内存
		ResponseCreateGuild(Client, GuildName, GuildID);
	}
	else
	{
		ResponseStartGame(Client, "车队已存在创建失败!");
	}

}

void NotifyApplyJoinGuild(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> SrcClient)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, SrcClient->Uin); //ApplyUin
	memcpy(p, SrcClient->NickName, 17);
	p += 17;
	Write32(p, 0); //RandomFlag
	Write32(p, SrcClient->Uin); //ExpLevel
	Write8(p, 0); //ApplyType
	Write8(p, 0); //RecommendType

	len = p - buf.data();
	SendToClient(Client, 15208, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyAddToGuildResult(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> SrcClient)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //ApplyUin

	memcpy(p, Client->NickName, 17);
	p += 17;
	memcpy(p, SrcClient->GuildName, 17);
	p += 17;
	Write32(p, SrcClient->GuildID); //GuildID
	Write16(p, 7); //Duty
	Write32(p, 0); //Right
	Write32(p, 0); //Medal
	Write32(p, SrcClient->Uin); //LeaderUin
	Write16(p, 0); //YesterdayMaxOnline
	Write32(p, 0); //LastSingleLoginTime
	Write16(p, 0); //GuildLevel
	Write32(p, 0); //WonderLandID
	Write16(p, 0); //WonderLandDuty
	Write32(p, 0); //ApproverUin
	Write8(p, 0); //ApplyType

	len = p - buf.data();
	SendToClient(Client, 15210, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void AutoReplyGuildClient(std::shared_ptr<ClientNode> ApplyClient, UINT Guid, UINT LeaderUin) {
	BOOL CanJoin = 0;


	std::vector<int> conditionValues;
	conditionValues.push_back(Guid);
	std::vector<int> resultRow = executeQueryRow("select 1 from GuildList where ID=? and MaxMember>CurrentMember;", "AutoReplyGuild", conditionValues);
	if (resultRow.size() >= 1) {
		CanJoin = 1;
	}
	if (!CanJoin)
	{
		ResponseStartGame(ApplyClient, "车队人数已满!");
		return;
	}
	BOOL HasGuild = 0;

	conditionValues.clear();
	resultRow.clear();
	conditionValues.push_back(ApplyClient->Uin);
	resultRow = executeQueryRow("SELECT 1 FROM GuildInfo where Uin=?;", "AutoReplyGuild", conditionValues);
	if (resultRow.size() >= 1) {
		HasGuild = 1;
	}
	if (HasGuild)
	{
		ResponseStartGame(ApplyClient, "你已加入车队！");
		return;
	}

	PlayerDB_InsertGuildInfo(ApplyClient->Uin, Guid, 7);
	PlayerDB_UpdateGuildList(Guid);
	BOOL IsMax = 0;
	conditionValues.clear();
	resultRow.clear();
	conditionValues.push_back(Guid);
	resultRow = executeQueryRow("select 1 from GuildList where ID=? and MaxMember<=CurrentMember;", "AutoReplyGuild", conditionValues);
	if (resultRow.size() >= 1) {
		IsMax = 1;
	}
	if (IsMax) {
		PlayerDB_UpdateGuildListEnlistType(Guid);
	}

	ApplyClient->GuildDuty = 7;
	ApplyClient->GuildID = Guid;

	std::shared_ptr<ClientNode> Client = std::make_shared<ClientNode>();
	Client->Uin = LeaderUin;
	Client->GuildID = Guid;
	NotifyAddToGuildResult(ApplyClient, Client);
	//delete Client;

	std::string GuildName = "";

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Name FROM guildlist where ID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Guid);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			GuildName = UTF2GB(res->getString("Name").c_str());
			break;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("AutoReplyGuild error\n");
	}

	if (!GuildName.empty())
	{
		memcpy(ApplyClient->GuildName, GuildName.c_str(), 17);//更新内存
	}
	ResponseStartGame(ApplyClient, "已自动审批，你已进入该车队。");
}

void RequestApplyJoinGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	char GuildName[17] = { 0 };
	char ApplyNickName[17] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	memcpy(GuildName, p, 17);
	p += 17;
	memcpy(ApplyNickName, p, 17);
	p += 17;
	UINT Guid = Read32(p);


	UINT LeaderUin = 0;


	std::vector<int> conditionValues;
	conditionValues.push_back(Guid);
	std::vector<int> resultRow = executeQueryRow("SELECT LeaderUin FROM GuildList where ID=?;", "RequestApplyJoinGuild", conditionValues);
	if (resultRow.size() >= 1) {
		LeaderUin = resultRow[0];
	}

	std::shared_ptr<ClientNode> Leader = GetClient(LeaderUin);
	if (!Leader)
	{
		if (AutoReplyGuild) {
			AutoReplyGuildClient(Client, Guid, LeaderUin);
		}
		else {
			ResponseStartGame(Client, "车队队长不在线!");
		}
	}
	else
	{
		ResponseStartGame(Client, "申请成功");
		NotifyApplyJoinGuild(Leader, Client);
	}
}



void RequestReplyJoinGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	BYTE* p = Body;
	char ApplyNickName[17] = { 0 };
	UINT ApplyUin = Read32(p);
	memcpy(ApplyNickName, p, 17);
	p += 17;
	UINT ReplyUin = Read32(p);
	short ResultID = Read16(p);
	USHORT ReasonLen = Read16(p);
	UINT RandomFlag = Read32(p);
	int OperationChannelType = Read32(p);
	UCHAR ApplyType = Read8(p);
	UCHAR RecommendType = Read8(p);
	BOOL CanJoin = 0;


	std::vector<int> conditionValues;
	conditionValues.push_back(Client->GuildID);
	std::vector<int> resultRow = executeQueryRow("select 1 from GuildList where ID=? and MaxMember>CurrentMember;", "RequestReplyJoinGuild", conditionValues);
	if (resultRow.size() >= 1) {
		CanJoin = 1;
	}
	if (!CanJoin)
	{
		return;
	}
	BOOL HasGuild = 0;

	conditionValues.clear();
	resultRow.clear();
	conditionValues.push_back(ApplyUin);
	resultRow = executeQueryRow("SELECT 1 FROM GuildInfo where Uin=?;", "RequestReplyJoinGuild", conditionValues);
	if (resultRow.size() >= 1) {
		HasGuild = 1;
	}
	if (HasGuild)
	{
		return;
	}
	if (ResultID)
	{

		PlayerDB_InsertGuildInfo(ApplyUin, Client->GuildID, 7);
		PlayerDB_UpdateGuildList(Client->GuildID);
		BOOL IsMax = 0;
		conditionValues.clear();
		resultRow.clear();
		conditionValues.push_back(Client->GuildID);
		resultRow = executeQueryRow("select 1 from GuildList where ID=? and MaxMember<=CurrentMember;", "RequestReplyJoinGuild", conditionValues);
		if (resultRow.size() >= 1) {
			IsMax = 1;
		}
		if (IsMax) {
			PlayerDB_UpdateGuildListEnlistType(Client->GuildID);
		}
	}
	std::shared_ptr<ClientNode> ApplyClient = GetClient(ApplyUin);
	if (ApplyClient)
	{
		if (ResultID)
		{
			NotifyAddToGuildResult(ApplyClient, Client);
			ApplyClient->GuildDuty = 7;
			ApplyClient->GuildID = Client->GuildID;
			memcpy(ApplyClient->GuildName, Client->GuildName, 17);//更新内存
		}
		else
		{
			ResponseStartGame(ApplyClient, "车队队长拒绝你的申请!");
		}
	}

}

void ResponseRemoveMember(std::shared_ptr<ClientNode> Client, UINT DstUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, DstUin); //DstUin
	Write16(p, 0); //ResultID
	Write16(p, 0); //ReasonLen
	Write32(p, 0); //AddCash
	memset(p, 0, 17);
	p += 17;
	memset(p, 0, 17);
	p += 17;
	Write32(p, 0); //OperationChannelType
	Write32(p, 0); //Contribute

	len = p - buf.data();
	SendToClient(Client, 15002, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyRemoveMember(std::shared_ptr<ClientNode> Client, UINT Uin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write32(p, Client->Uin); //DstUin
	memcpy(p, Client->GuildName, 17);
	p += 17;
	memcpy(p, Client->NickName, 17);
	p += 17;
	Write32(p, Client->GuildID); //Guid
	Write32(p, 0); //AddCash
	Client->GuildID = 0;
	Client->GuildDuty = 0;
	memset(Client->GuildName, 0, 17);
	len = p - buf.data();
	SendToClient(Client, 15202, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestRemoveMember(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT DstUin = Read32(p);

	PlayerDB_DeleteGuildInfo(DstUin);
	PlayerDB_UpdateGuildListCurrentMember(Client->GuildID);
	ResponseRemoveMember(Client, DstUin);
	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (DstClient)
	{
		NotifyRemoveMember(DstClient, Uin);
	}
}


void ResponseGuildDetail(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	

	std::string NickName = "";
	int MemNumber = 0;

	Write32(p, Client->Uin); // Uin
	{						 // GuildDescInfo
		BYTE* pGuildDescInfo = p;
		Write16(pGuildDescInfo, 0); // len

		Write32(pGuildDescInfo, Client->GuildID);					  // GuildID
		memset(pGuildDescInfo, 0, 17);//GuildName[]
		memcpy(pGuildDescInfo, Client->GuildName, 17);
		pGuildDescInfo += 17;
		{													  // LeaderInfo
			BYTE* pLeaderInfo = pGuildDescInfo;
			Write16(pLeaderInfo, 0); // len

			UINT Uin = PlayerDB_SelectGuildID(Client->GuildID, NickName);
			
			if (Uin > 10000) {
				Write32(pLeaderInfo, Uin);						 // Uin
				if (!NickName.empty())
				{
					memset(pLeaderInfo, 0, 17);//NickName[]
					memcpy(pLeaderInfo, NickName.c_str(), NickName.length());
					pLeaderInfo += 17;
				}
				Write16(pLeaderInfo, 0); // Duty
				Write8(pLeaderInfo, 1);	 // OnlineFlag
				Write32(pLeaderInfo, 0); // LastLogin
				Write32(pLeaderInfo, 0); // MemProsperity
				Write32(pLeaderInfo, 0); // LastEnlistedTime
				Write8(pLeaderInfo, 1);	 // EnlistFlag
				Write16(pLeaderInfo, 0); // WonderLandDuty
				Write32(pLeaderInfo, 0); // TotalWlDegree
				Write32(pLeaderInfo, 0); // WeeklyWlDegree
				Write32(pLeaderInfo, 0); // Donate
				Write32(pLeaderInfo, 0); // SeasonZhanxun
				Write32(pLeaderInfo, 0); // WeeklyContributeValue
				Write32(pLeaderInfo, 0); // WeeklyProsperContributeValue
				Write32(pLeaderInfo, 0); // MonthlyProsperContributeValue
				Write32(pLeaderInfo, 0); // CommonParas[]
				Write32(pLeaderInfo, 0); // TimeJoinedGuild
				Write8(pLeaderInfo, 0);	 // AppliedChampionship
			}
			len = pLeaderInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}

		{
			Guidinfo* guidinfo = new Guidinfo[1];
			memset(guidinfo, 0, sizeof(Guidinfo) * 1);
			PlayerDB_SelectGuildInfo(Client->GuildID, guidinfo);

			Write16(pGuildDescInfo, 0);		// NotesLen
			Write16(pGuildDescInfo, 0);		// DescLen 
			Write32(pGuildDescInfo, guidinfo[0].Level);		// Level
			Write32(pGuildDescInfo, guidinfo[0].Score); // Score 
			Write32(pGuildDescInfo, 0);		// CreateTime
			Write16(pGuildDescInfo, guidinfo[0].Attribute);		// Attribute 
			Write32(pGuildDescInfo, guidinfo[0].MaxMember);	// MaxMember 
			Write32(pGuildDescInfo, guidinfo[0].CurrentMember);	// CurrentMember
			Write16(pGuildDescInfo, guidinfo[0].TeamAddition);	// TeamAddition 
			Write16(pGuildDescInfo, 0);		// YesterdayMaxOnline
			Write16(pGuildDescInfo, 0);		// HistoryMaxOnline
			Write32(pGuildDescInfo, 0);		// HistoryMaxOnlineTime
			Write32(pGuildDescInfo, guidinfo[0].Prosperity); // Prosperity
			Write16(pGuildDescInfo, guidinfo[0].Level);		// Level

			delete[] guidinfo;
		}

		{								// GuildEnlistCond
			BYTE* pGuildEnlistCond = pGuildDescInfo;
			Write16(pGuildEnlistCond, 0); // len

			Write32(pGuildEnlistCond, 0); // ProsperityCondition
			Write32(pGuildEnlistCond, 0); // JoinTotalCondition
			Write32(pGuildEnlistCond, 0); // HonorCondition
			Write32(pGuildEnlistCond, 0); // CharmCondition
			Write32(pGuildEnlistCond, 0); // DanceExpCondition
			Write32(pGuildEnlistCond, 0); // SkillPointCondition
			Write32(pGuildEnlistCond, 0); // ExpLevelCondition
			Write32(pGuildEnlistCond, 0); // ApplyExpLevelCondition
			Write8(pGuildEnlistCond, 0);  // EnlistType

			len = pGuildEnlistCond - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		Write8(pGuildDescInfo, 0);		 // NewGuildType
		Write32(pGuildDescInfo, 0); // GuildQQGroup
		{								 // WorkDayActiveTimeSeg
			BYTE* pWorkDayActiveTimeSeg = pGuildDescInfo;
			Write16(pWorkDayActiveTimeSeg, 0); // len

			Write16(pWorkDayActiveTimeSeg, 0); // BeginTimeInt
			Write16(pWorkDayActiveTimeSeg, 0); // EndTimeInt

			len = pWorkDayActiveTimeSeg - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		{ // WeekendActiveTimeSeg
			BYTE* pWeekendActiveTimeSeg = pGuildDescInfo;
			Write16(pWeekendActiveTimeSeg, 0); // len

			Write16(pWeekendActiveTimeSeg, 0); // BeginTimeInt
			Write16(pWeekendActiveTimeSeg, 0); // EndTimeInt

			len = pWeekendActiveTimeSeg - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		Write32(pGuildDescInfo, 0); // LastEnlistTime
		Write8(pGuildDescInfo, 0);	// EnlistNum
		Write32(pGuildDescInfo, 0); // LastModGuildNameTime
		Write32(pGuildDescInfo, 0); // WonderLandID
		Write32(pGuildDescInfo, 0); // UiCurDonate
		Write32(pGuildDescInfo, 0); // LastDayCheerValue
		Write32(pGuildDescInfo, 0); // LastDayCheerCnt
		{							// BorderMatchInfo
			BYTE* pBorderMatchInfo = pGuildDescInfo;
			Write16(pBorderMatchInfo, 0); // len

			Write32(pBorderMatchInfo, 0); // Honor
			Write32(pBorderMatchInfo, 0); // SeasonHonor
			Write32(pBorderMatchInfo, 0); // SeasonZhanxunFirstUin
			Write32(pBorderMatchInfo, 0); // SeasonZhanxun
			Write8(pBorderMatchInfo, 0);  // SeasonZhanxunFirstNickName[]

			len = pBorderMatchInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		{ // CardOtherInfo
			BYTE* pCardOtherInfo = pGuildDescInfo;
			Write16(pCardOtherInfo, 0); // len

			Write32(pCardOtherInfo, 0); // MainZone1
			Write32(pCardOtherInfo, 0); // MainZone2
			Write32(pCardOtherInfo, 0); // MainMode1
			Write32(pCardOtherInfo, 0); // MainMode2
			Write32(pCardOtherInfo, 0); // OtherInterest1
			Write32(pCardOtherInfo, 0); // OtherInterest2

			len = pCardOtherInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}

		len = pGuildDescInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	BYTE* pMemberNum = p;
	Write16(p, 0); // MemberNum

	Guidinfo* guidinfo = nullptr;
	MemNumber = PlayerDB_SelectGuildMember(Client->GuildID, guidinfo);
	if (guidinfo) {
		for (size_t i = 0; i < MemNumber; i++)
		{
			BYTE* pMember = p;
			Write16(pMember, 0); // len

			Write32(pMember, guidinfo[i].Uin); // Uin
			NickName = guidinfo[i].NickName;
			
			if (!NickName.empty())
			{
				memset(pMember, 0, 17);//NickName[]
				memcpy(pMember, NickName.c_str(), NickName.length());
				pMember += 17;
			}
			Write16(pMember, guidinfo[i].Duty);	// Duty
			Write8(pMember, 1);	 // OnlineFlag
			Write32(pMember, 0); // LastLogin
			Write32(pMember, guidinfo[i].MemProsperity);		// MemProsperity
			Write32(pMember, 0); // LastEnlistedTime
			Write8(pMember, 0);	 // EnlistFlag
			Write16(pMember, 0); // WonderLandDuty
			Write32(pMember, 0); // TotalWlDegree
			Write32(pMember, 0); // WeeklyWlDegree
			Write32(pMember, 0); // Donate
			Write32(pMember, 0); // SeasonZhanxun
			Write32(pMember, 0); // WeeklyContributeValue
			Write32(pMember, 0); // WeeklyProsperContributeValue
			Write32(pMember, 0); // MonthlyProsperContributeValue
			Write32(pMember, 0); // CommonParas[]
			Write32(pMember, 0); // TimeJoinedGuild
			Write8(pMember, 0);	 // AppliedChampionship

			len = pMember - p;
			Set16(p, (WORD)len);
			p += len;

		}
		delete[] guidinfo;
	}


	Set16(pMemberNum, MemNumber);

	Write16(p, 0); // LogNum
	{ //Duty
		BYTE* pDuty = p;
		Write16(pDuty, 0); //len

		Write16(pDuty, 0xa3bf); //Dutys 
		{ //AutoPromLimit
			BYTE* pAutoPromLimit = pDuty;
			Write16(pAutoPromLimit, 0); //len

			Write32(pAutoPromLimit, 0); //ScoreLimit
			Write32(pAutoPromLimit, 0); //ProsperityLimit

			len = pAutoPromLimit - pDuty;
			Set16(pDuty, (WORD)len);
			pDuty += len;
		}
		{ //ApplyPromLimit
			BYTE* pApplyPromLimit = pDuty;
			Write16(pApplyPromLimit, 0); //len

			Write32(pApplyPromLimit, 0); //ScoreLimit
			Write32(pApplyPromLimit, 0); //ProsperityLimit

			len = pApplyPromLimit - pDuty;
			Set16(pDuty, (WORD)len);
			pDuty += len;
		}
		Write8(pDuty, 0); //DutyTitle[]

		len = pDuty - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); // ResultID
	Write16(p, 0); // ReasonLen
	Write32(p, 0); // Contribute[]
	Write32(p, 0); // SelfJoinGuildTime
	Write16(p, 0); // SelfApplyDuty
	Write8(p, 0);  // HasAward
	Write32(p, 0); // CurrentOnLineGuildMembers
	Write32(p, 0); // LastWeekProsper

	len = p - buf.data();
	SendToClient(Client, 15003, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGuildDetail(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT Sort = Read32(p);

	ResponseGuildDetail(Client);
}


void ResponseFindGuild(std::shared_ptr<ClientNode> Client, int GuildID, char* GuildName)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	

	std::string NickName = "";
	Write32(p, Client->Uin); //Uin
	{ //GuildDescInfo
		BYTE* pGuildDescInfo = p;
		Write16(pGuildDescInfo, 0); //len

		Write32(pGuildDescInfo, GuildID); //GuildID
		memset(pGuildDescInfo, 0, 17);//GuildName[]
		memcpy(pGuildDescInfo, GuildName, 17);
		pGuildDescInfo += 17;
		{ //LeaderInfo
			BYTE* pLeaderInfo = pGuildDescInfo;
			Write16(pLeaderInfo, 0); //len


			UINT Uin = PlayerDB_SelectGuildID(GuildID, NickName);
			
			if (Uin > 10000) {
				Write32(pLeaderInfo, Uin);						 // Uin
				if (!NickName.empty())
				{
					memset(pLeaderInfo, 0, 17);//NickName[]
					memcpy(pLeaderInfo, NickName.c_str(), NickName.length());
					pLeaderInfo += 17;
				}
				Write16(pLeaderInfo, 0); // Duty
				Write8(pLeaderInfo, 1);	 // OnlineFlag
				Write32(pLeaderInfo, 0); // LastLogin
				Write32(pLeaderInfo, 0); // MemProsperity
				Write32(pLeaderInfo, 0); // LastEnlistedTime
				Write8(pLeaderInfo, 1);	 // EnlistFlag
				Write16(pLeaderInfo, 0); // WonderLandDuty
				Write32(pLeaderInfo, 0); // TotalWlDegree
				Write32(pLeaderInfo, 0); // WeeklyWlDegree
				Write32(pLeaderInfo, 0); // Donate
				Write32(pLeaderInfo, 0); // SeasonZhanxun
				Write32(pLeaderInfo, 0); // WeeklyContributeValue
				Write32(pLeaderInfo, 0); // WeeklyProsperContributeValue
				Write32(pLeaderInfo, 0); // MonthlyProsperContributeValue
				Write32(pLeaderInfo, 0); // CommonParas[]
				Write32(pLeaderInfo, 0); // TimeJoinedGuild
				Write8(pLeaderInfo, 0);	 // AppliedChampionship
			}
			len = pLeaderInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}

		Guidinfo* guidinfo = new Guidinfo[1];
		memset(guidinfo, 0, sizeof(Guidinfo) * 1);
		PlayerDB_SelectGuildInfo(Client->GuildID, guidinfo);

		Write16(pGuildDescInfo, 0);		// NotesLen
		Write16(pGuildDescInfo, 0);		// DescLen 
		Write32(pGuildDescInfo, guidinfo[0].Level);		// Level
		Write32(pGuildDescInfo, guidinfo[0].Score); // Score 
		Write32(pGuildDescInfo, 0);		// CreateTime
		Write16(pGuildDescInfo, guidinfo[0].Attribute);		// Attribute 
		Write32(pGuildDescInfo, guidinfo[0].MaxMember);	// MaxMember 
		Write32(pGuildDescInfo, guidinfo[0].CurrentMember);	// CurrentMember
		Write16(pGuildDescInfo, guidinfo[0].TeamAddition);	// TeamAddition 
		Write16(pGuildDescInfo, 0);		// YesterdayMaxOnline
		Write16(pGuildDescInfo, 0);		// HistoryMaxOnline
		Write32(pGuildDescInfo, 0);		// HistoryMaxOnlineTime
		Write32(pGuildDescInfo, guidinfo[0].Prosperity); // Prosperity
		Write16(pGuildDescInfo, guidinfo[0].Level);		// Level

		delete[] guidinfo;


		{ //GuildEnlistCond
			BYTE* pGuildEnlistCond = pGuildDescInfo;
			Write16(pGuildEnlistCond, 0); //len

			Write32(pGuildEnlistCond, 0); //ProsperityCondition
			Write32(pGuildEnlistCond, 0); //JoinTotalCondition
			Write32(pGuildEnlistCond, 0); //HonorCondition
			Write32(pGuildEnlistCond, 0); //CharmCondition
			Write32(pGuildEnlistCond, 0); //DanceExpCondition
			Write32(pGuildEnlistCond, 0); //SkillPointCondition
			Write32(pGuildEnlistCond, 0); //ExpLevelCondition
			Write32(pGuildEnlistCond, 0); //ApplyExpLevelCondition
			Write8(pGuildEnlistCond, 0); //EnlistType

			len = pGuildEnlistCond - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		Write8(pGuildDescInfo, 0); //NewGuildType
		Write32(pGuildDescInfo, 0); //GuildQQGroup
		{ //WorkDayActiveTimeSeg
			BYTE* pWorkDayActiveTimeSeg = pGuildDescInfo;
			Write16(pWorkDayActiveTimeSeg, 0); //len

			Write16(pWorkDayActiveTimeSeg, 0); //BeginTimeInt
			Write16(pWorkDayActiveTimeSeg, 0); //EndTimeInt

			len = pWorkDayActiveTimeSeg - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		{ //WeekendActiveTimeSeg
			BYTE* pWeekendActiveTimeSeg = pGuildDescInfo;
			Write16(pWeekendActiveTimeSeg, 0); //len

			Write16(pWeekendActiveTimeSeg, 0); //BeginTimeInt
			Write16(pWeekendActiveTimeSeg, 0); //EndTimeInt

			len = pWeekendActiveTimeSeg - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		Write32(pGuildDescInfo, 0); // LastEnlistTime
		Write8(pGuildDescInfo, 0);	// EnlistNum
		Write32(pGuildDescInfo, 0); // LastModGuildNameTime
		Write32(pGuildDescInfo, 0); // WonderLandID
		Write32(pGuildDescInfo, 0); // UiCurDonate
		Write32(pGuildDescInfo, 0); // LastDayCheerValue
		Write32(pGuildDescInfo, 0); // LastDayCheerCnt
		{ //BorderMatchInfo
			BYTE* pBorderMatchInfo = pGuildDescInfo;
			Write16(pBorderMatchInfo, 0); //len

			Write32(pBorderMatchInfo, 0); //Honor
			Write32(pBorderMatchInfo, 0); //SeasonHonor
			Write32(pBorderMatchInfo, 0); //SeasonZhanxunFirstUin
			Write32(pBorderMatchInfo, 0); //SeasonZhanxun
			Write8(pBorderMatchInfo, 0); //SeasonZhanxunFirstNickName[]

			len = pBorderMatchInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}
		{ //CardOtherInfo
			BYTE* pCardOtherInfo = pGuildDescInfo;
			Write16(pCardOtherInfo, 0); //len

			Write32(pCardOtherInfo, 0); //MainZone1
			Write32(pCardOtherInfo, 0); //MainZone2
			Write32(pCardOtherInfo, 0); //MainMode1
			Write32(pCardOtherInfo, 0); //MainMode2
			Write32(pCardOtherInfo, 0); //OtherInterest1
			Write32(pCardOtherInfo, 0); //OtherInterest2

			len = pCardOtherInfo - pGuildDescInfo;
			Set16(pGuildDescInfo, (WORD)len);
			pGuildDescInfo += len;
		}

		len = pGuildDescInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //ResultID
	Write16(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 15009, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}
void RequestFindGuild(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	char GuildName[17] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	memcpy(GuildName, p, 17);
	p += 17;
	int GuildID = Read32(p);
	

	if (strcmp(GuildName, "\x00") == 0)
	{
		return;
	}
	else
	{
		char* newGuildname = NULL;
		newGuildname = G2U(GuildName);

		if (newGuildname)
		{
			std::vector<std::string> conditionValues;
			conditionValues.push_back(newGuildname);
			std::vector<int> resultRow = executeQueryRow("SELECT ID FROM GuildList where Name=?;", "RequestFindGuild", conditionValues);
			if (resultRow.size() >= 1) {
				GuildID = resultRow[0];
			}
		}
	}
	ResponseFindGuild(Client, GuildID, GuildName);
}

void RequestAppointedPosition(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT DstUin = Read32(p);
	UINT Duty = Read16(p); //职位
	std::string NickName = "";

	PlayerDB_UpdateGuildInfoDuty(DstUin, Duty);

	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

		Write32(p, Uin); //Uin
		memcpy(p, Client->NickName, MaxNickName);
		p += MaxNickName;

		Write32(p, DstUin); //Uin
		PlayerDB_SelectNickName(DstUin, NickName);

		if (!NickName.empty()) {
			memcpy(p, NickName.c_str(), MaxNickName);
			p += MaxNickName;
		}
		Write16(p, Duty);

		Write16(p, 0);

		Write16(p, 0x10);//len
		WriteString(p, "修改队员职务成功");

		Write32(p, 0);//duty

		len = p - buf.data();
		SendToClient(Client, 15006, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}




void ResponseSaveShapeRefit4(std::shared_ptr<ClientNode> Client, int KartID)
{
	int ShapeSuitID = 0;
	int suitid = 0;
	suitid = ShapeSuitID;
	UINT LegendSuitLevel = 0;
	UINT LegendSuitLevelChoice = 0;
	UINT ShapeLegendSuitID = 0;
	UINT KartHeadRefitItemID = 0;
	UINT KartTailRefitItemID = 0;
	UINT KartFlankRefitItemID = 0;
	UINT KartTireRefitItemID = 0;
	UINT SkinSuitID = 0;
	UINT WWeight = 0;
	UINT SpeedWeight = 0;
	UINT JetWeight = 0;
	UINT SJetWeight = 0;
	UINT AccuWeight = 0;

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(KartID);
	std::vector<int> resultRow = executeQueryRow("SELECT WWeight,SpeedWeight,JetWeight,SJetWeight,AccuWeight,KartHeadRefitItemID,KartTailRefitItemID,KartFlankRefitItemID,KartTireRefitItemID,ShapeSuitID,LegendSuitLevelChoice,LegendSuitLevel,ShapeLegendSuitID,SkinSuitID FROM KartRefit Where Uin=? and KartID=?;"
		, "ResponseSaveShapeRefit4", conditionValues);
	if (resultRow.size() >= 13) {
		WWeight = resultRow[0];
		SpeedWeight = resultRow[1];
		JetWeight = resultRow[2];
		SJetWeight = resultRow[3];
		AccuWeight = resultRow[4];
		KartHeadRefitItemID = resultRow[5];
		KartTailRefitItemID = resultRow[6];
		KartFlankRefitItemID = resultRow[7];
		KartTireRefitItemID = resultRow[8];
		ShapeSuitID = resultRow[9];
		LegendSuitLevelChoice = resultRow[10];
		LegendSuitLevel = resultRow[11];
		ShapeLegendSuitID = resultRow[12];
		SkinSuitID = resultRow[13];
		Client->ClientKartRefitItemInfo.WWeight = WWeight;
		Client->ClientKartRefitItemInfo.SpeedWeight = SpeedWeight;
		Client->ClientKartRefitItemInfo.JetWeight = JetWeight;
		Client->ClientKartRefitItemInfo.SJetWeight = SJetWeight;
		Client->ClientKartRefitItemInfo.AccuWeight = AccuWeight;
		Client->ClientKartRefitItemInfo.KartHeadRefitItemID = KartHeadRefitItemID;
		Client->ClientKartRefitItemInfo.KartTailRefitItemID = KartTailRefitItemID;
		Client->ClientKartRefitItemInfo.KartFlankRefitItemID = KartFlankRefitItemID;
		Client->ClientKartRefitItemInfo.KartTireRefitItemID = KartTireRefitItemID;
		Client->KartSkinID = Client->ClientKartRefitItemInfo.ShapeSuitID = SkinSuitID;
		Client->ClientKartRefitItemInfo.LegendSuitLevelChoice = LegendSuitLevelChoice;
		Client->ClientKartRefitItemInfo.LegendSuitLevel = LegendSuitLevel;
		Client->ClientKartRefitItemInfo.ShapeLegendSuitID = ShapeLegendSuitID;
	}
	KartShapeRefitLegendInfo ShapeSuit = getLegendaryShapeSuitID(ShapeLegendSuitID);
	if (ShapeSuit.KartID != 0)
	{
		switch (LegendSuitLevelChoice)
		{
		case 2:
			ShapeSuitID = ShapeSuit.LevelInfo[0].ShapeSuitID;
			break;
		case 4:
		case 6:
			ShapeSuitID = ShapeSuit.LevelInfo[1].ShapeSuitID;
			break;
		case 8:
		case 14:
			ShapeSuitID = ShapeSuit.LevelInfo[2].ShapeSuitID;
			break;
		case 16:
		case 24:
		case 30:
			ShapeSuitID = ShapeSuit.LevelInfo[3].ShapeSuitID;
			break;
		case 32:
		case 48:
			ShapeSuitID = ShapeSuit.LevelInfo[4].ShapeSuitID;
			break;
		case 112:
			ShapeSuitID = ShapeSuit.LevelInfo[5].ShapeSuitID;
			break;
		case 128:
			ShapeSuitID = ShapeSuit.LevelInfo[6].ShapeSuitID;
			break;
		case 384:
			ShapeSuitID = ShapeSuit.LevelInfo[7].ShapeSuitID;
			break;
		case 896:
			ShapeSuitID = ShapeSuit.LevelInfo[8].ShapeSuitID;
			break;
		case 1920:
			ShapeSuitID = ShapeSuit.LevelInfo[9].ShapeSuitID;
			break;
		}
		SkinSuitID = ShapeSuitID;

	}

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);		// ResultID
	Write8(p, 0);		// ReasonLen
	Write32(p, KartID); // KartItemID
	{					// KartRefitExInfo
		BYTE* pKartRefitExInfo = p;
		Write16(pKartRefitExInfo, 0); // len

		if (ShapeSuitID == 0) {
			if (KartHeadRefitItemID > 0) {
				Write8(pKartRefitExInfo, 3); // SpeedRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (KartTailRefitItemID > 0) {
				Write8(pKartRefitExInfo, 3); // JetRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (KartFlankRefitItemID > 0) {
				Write8(pKartRefitExInfo, 3); // SJetRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (KartTireRefitItemID > 0) {
				Write8(pKartRefitExInfo, 3); // AccuRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}

			if (KartHeadRefitItemID > 0) {
				Write8(pKartRefitExInfo, 10); // SpeedAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (KartTailRefitItemID > 0) {
				Write8(pKartRefitExInfo, 10); // JetAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (KartFlankRefitItemID > 0) {
				Write8(pKartRefitExInfo, 10); // SJetAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (KartTireRefitItemID > 0) {
				Write8(pKartRefitExInfo, 10); // AccuAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
		}
		else {
			Write8(pKartRefitExInfo, 0); // SpeedRefitStar
			Write8(pKartRefitExInfo, 0); // JetRefitStar
			Write8(pKartRefitExInfo, 0); // SJetRefitStar
			Write8(pKartRefitExInfo, 0); // AccuRefitStar
			Write8(pKartRefitExInfo, 0); // SpeedAddRatio
			Write8(pKartRefitExInfo, 0); // JetAddRatio
			Write8(pKartRefitExInfo, 0); // SJetAddRatio
			Write8(pKartRefitExInfo, 0); // AccuAddRatio
		}

		len = pKartRefitExInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 4);					  // RefitItemNum
	Write32(p, KartHeadRefitItemID);  // RefitItemID[]
	Write32(p, KartTailRefitItemID);  // RefitItemID[]
	Write32(p, KartFlankRefitItemID); // RefitItemID[]
	Write32(p, KartTireRefitItemID);  // RefitItemID[]

	Write32(p, ShapeSuitID); // ShapeSuitID
	{						 // RefitClientInfo
		BYTE* pRefitClientInfo = p;
		Write16(pRefitClientInfo, 0); // len

		Write32(pRefitClientInfo, Client->Uin);			 // Uin
		Write32(pRefitClientInfo, KartID);				 // KartId
		Write32(pRefitClientInfo, 0);					 // RefitCout
		Write16(pRefitClientInfo, 0);					 // MaxFlags
		Write16(pRefitClientInfo, 0);					 // WWeight
		Write16(pRefitClientInfo, 0);					 // SpeedWeight
		Write16(pRefitClientInfo, 0);					 // JetWeight
		Write16(pRefitClientInfo, 0);					 // SJetWeight
		Write16(pRefitClientInfo, 0);					 // AccuWeight
		Write32(pRefitClientInfo, 0);					 // ShapeRefitCount
		Write32(pRefitClientInfo, KartHeadRefitItemID);	 // KartHeadRefitItemID
		Write32(pRefitClientInfo, KartTailRefitItemID);	 // KartTailRefitItemID
		Write32(pRefitClientInfo, KartFlankRefitItemID); // KartFlankRefitItemID
		Write32(pRefitClientInfo, KartTireRefitItemID);	 // KartTireRefitItemID
		{												 // KartRefitExInfo
			BYTE* pKartRefitExInfo = pRefitClientInfo;
			Write16(pKartRefitExInfo, 0); // len

			if (ShapeSuitID == 0) {
				if (KartHeadRefitItemID > 0) {
					Write8(pKartRefitExInfo, 3); // SpeedRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (KartTailRefitItemID > 0) {
					Write8(pKartRefitExInfo, 3); // JetRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (KartFlankRefitItemID > 0) {
					Write8(pKartRefitExInfo, 3); // SJetRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (KartTireRefitItemID > 0) {
					Write8(pKartRefitExInfo, 3); // AccuRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}

				if (KartHeadRefitItemID > 0) {
					Write8(pKartRefitExInfo, 10); // SpeedAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (KartTailRefitItemID > 0) {
					Write8(pKartRefitExInfo, 10); // JetAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (KartFlankRefitItemID > 0) {
					Write8(pKartRefitExInfo, 10); // SJetAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (KartTireRefitItemID > 0) {
					Write8(pKartRefitExInfo, 10); // AccuAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
			}
			else {
				Write8(pKartRefitExInfo, 0); // SpeedRefitStar
				Write8(pKartRefitExInfo, 0); // JetRefitStar
				Write8(pKartRefitExInfo, 0); // SJetRefitStar
				Write8(pKartRefitExInfo, 0); // AccuRefitStar
				Write8(pKartRefitExInfo, 0); // SpeedAddRatio
				Write8(pKartRefitExInfo, 0); // JetAddRatio
				Write8(pKartRefitExInfo, 0); // SJetAddRatio
				Write8(pKartRefitExInfo, 0); // AccuAddRatio
			}

			len = pKartRefitExInfo - pRefitClientInfo;
			Set16(pRefitClientInfo, (WORD)len);
			pRefitClientInfo += len;
		}
		Write32(pRefitClientInfo, 0);					  // SecondRefitCount
		Write16(pRefitClientInfo, 0);					  // Speed2Weight
		Write16(pRefitClientInfo, 0);					  // DriftVecWeight
		Write16(pRefitClientInfo, 0);					  // AdditionalZSpeedWeight
		Write16(pRefitClientInfo, 0);					  // AntiCollisionWeight
		Write16(pRefitClientInfo, 0);					  // LuckyValue
		Write16(pRefitClientInfo, 0);					  // RefitLuckyValueMaxWeight
		Write32(pRefitClientInfo, SkinSuitID);			  // ShapeSuitID
		Write8(pRefitClientInfo, LegendSuitLevel);		  // LegendSuitLevel
		Write32(pRefitClientInfo, LegendSuitLevelChoice); // LegendSuitLevelChoice
		Write32(pRefitClientInfo, ShapeLegendSuitID);	  // ShapeLegendSuitID

		len = pRefitClientInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 316, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	ItemStatus aItemStatus;
	aItemStatus.ItemID = KartID;
	if (ShapeSuitID == 0)
	{
		Client->KartSkinID = 0;
		aItemStatus.ItemID = KartID;
	}
	else
	{
		Client->KartSkinID = ShapeSuitID;
		aItemStatus.ItemID = ShapeSuitID;
	}

	aItemStatus.NewStatus = 1;

	if (!Room)
	{
		// NotifyChangeItemStatus(Client, Client->Uin, 2, aItemStatus);
	}
	else
	{
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient && RoomClient != Client)
			{
				NotifyChangeItemStatus(RoomClient, Client, 1, { aItemStatus });
				// ResponseSaveShapeRefit4(RoomClient, KartID);
			}
		}
	}
}


void ResponseSaveShapeRefit(std::shared_ptr<ClientNode> Client, int KartID, int RefitItemNum, int RefitItemID[], int ShapeSuitID)
{
	int suitid = 0;
	suitid = ShapeSuitID;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	UINT LegendSuitLevel = 0;
	UINT LegendSuitLevelChoice = 0;
	UINT ShapeLegendSuitID = 0;

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(KartID);
	std::vector<int> resultRow = executeQueryRow("SELECT ShapeSuitID,LegendSuitLevel,LegendSuitLevelChoice,ShapeLegendSuitID FROM KartRefit WHERE Uin = ? AND KartID=?;", "ResponseSaveShapeRefit", conditionValues);
	if (resultRow.size() >= 4) {
		ShapeSuitID = resultRow[0];
		LegendSuitLevel = resultRow[1];
		LegendSuitLevelChoice = resultRow[2];
		ShapeLegendSuitID = resultRow[3];
	}

	Write16(p, 0);		// ResultID
	Write8(p, 0);		// ReasonLen
	Write32(p, KartID); // KartItemID
	{					// KartRefitExInfo
		BYTE* pKartRefitExInfo = p;
		Write16(pKartRefitExInfo, 0); // len

		if (ShapeSuitID == 0) {
			if (RefitItemID[0] > 0) {
				Write8(pKartRefitExInfo, 3); // SpeedRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (RefitItemID[1] > 0) {
				Write8(pKartRefitExInfo, 3); // JetRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (RefitItemID[2] > 0) {
				Write8(pKartRefitExInfo, 3); // SJetRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (RefitItemID[3] > 0) {
				Write8(pKartRefitExInfo, 3); // AccuRefitStar
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}

			if (RefitItemID[0] > 0) {
				Write8(pKartRefitExInfo, 10); // SpeedAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (RefitItemID[1] > 0) {
				Write8(pKartRefitExInfo, 10); // JetAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (RefitItemID[2] > 0) {
				Write8(pKartRefitExInfo, 10); // SJetAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
			if (RefitItemID[3] > 0) {
				Write8(pKartRefitExInfo, 10); // AccuAddRatio
			}
			else {
				Write8(pKartRefitExInfo, 0);
			}
		}
		else {
			Write8(pKartRefitExInfo, 0); // SpeedRefitStar
			Write8(pKartRefitExInfo, 0); // JetRefitStar
			Write8(pKartRefitExInfo, 0); // SJetRefitStar
			Write8(pKartRefitExInfo, 0); // AccuRefitStar
			Write8(pKartRefitExInfo, 0); // SpeedAddRatio
			Write8(pKartRefitExInfo, 0); // JetAddRatio
			Write8(pKartRefitExInfo, 0); // SJetAddRatio
			Write8(pKartRefitExInfo, 0); // AccuAddRatio
		}

		len = pKartRefitExInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, RefitItemNum);	// RefitItemNum
	Write32(p, RefitItemID[0]); // RefitItemID[]
	Write32(p, RefitItemID[1]); // RefitItemID[]
	Write32(p, RefitItemID[2]); // RefitItemID[]
	Write32(p, RefitItemID[3]); // RefitItemID[]

	Write32(p, ShapeSuitID); // ShapeSuitID
	{						 // RefitClientInfo
		BYTE* pRefitClientInfo = p;
		Write16(pRefitClientInfo, 0); // len

		Write32(pRefitClientInfo, Client->Uin);	   // Uin
		Write32(pRefitClientInfo, KartID);		   // KartId
		Write32(pRefitClientInfo, 0);			   // RefitCout
		Write16(pRefitClientInfo, 0);			   // MaxFlags
		Write16(pRefitClientInfo, 0);			   // WWeight
		Write16(pRefitClientInfo, 0);			   // SpeedWeight
		Write16(pRefitClientInfo, 0);			   // JetWeight
		Write16(pRefitClientInfo, 0);			   // SJetWeight
		Write16(pRefitClientInfo, 0);			   // AccuWeight
		Write32(pRefitClientInfo, 0);			   // ShapeRefitCount
		Write32(pRefitClientInfo, RefitItemID[0]); // KartHeadRefitItemID
		Write32(pRefitClientInfo, RefitItemID[1]); // KartTailRefitItemID
		Write32(pRefitClientInfo, RefitItemID[2]); // KartFlankRefitItemID
		Write32(pRefitClientInfo, RefitItemID[3]); // KartTireRefitItemID
		{										   // KartRefitExInfo
			BYTE* pKartRefitExInfo = pRefitClientInfo;
			Write16(pKartRefitExInfo, 0); // len

			if (ShapeSuitID == 0) {
				if (RefitItemID[0] > 0) {
					Write8(pKartRefitExInfo, 3); // SpeedRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (RefitItemID[1] > 0) {
					Write8(pKartRefitExInfo, 3); // JetRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (RefitItemID[2] > 0) {
					Write8(pKartRefitExInfo, 3); // SJetRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (RefitItemID[3] > 0) {
					Write8(pKartRefitExInfo, 3); // AccuRefitStar
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}

				if (RefitItemID[0] > 0) {
					Write8(pKartRefitExInfo, 10); // SpeedAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (RefitItemID[1] > 0) {
					Write8(pKartRefitExInfo, 10); // JetAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (RefitItemID[2] > 0) {
					Write8(pKartRefitExInfo, 10); // SJetAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
				if (RefitItemID[3] > 0) {
					Write8(pKartRefitExInfo, 10); // AccuAddRatio
				}
				else {
					Write8(pKartRefitExInfo, 0);
				}
			}
			else {
				Write8(pKartRefitExInfo, 0); // SpeedRefitStar
				Write8(pKartRefitExInfo, 0); // JetRefitStar
				Write8(pKartRefitExInfo, 0); // SJetRefitStar
				Write8(pKartRefitExInfo, 0); // AccuRefitStar
				Write8(pKartRefitExInfo, 0); // SpeedAddRatio
				Write8(pKartRefitExInfo, 0); // JetAddRatio
				Write8(pKartRefitExInfo, 0); // SJetAddRatio
				Write8(pKartRefitExInfo, 0); // AccuAddRatio
			}

			len = pKartRefitExInfo - pRefitClientInfo;
			Set16(pRefitClientInfo, (WORD)len);
			pRefitClientInfo += len;
		}
		Write32(pRefitClientInfo, 0);					  // SecondRefitCount
		Write16(pRefitClientInfo, 0);					  // Speed2Weight
		Write16(pRefitClientInfo, 0);					  // DriftVecWeight
		Write16(pRefitClientInfo, 0);					  // AdditionalZSpeedWeight
		Write16(pRefitClientInfo, 0);					  // AntiCollisionWeight
		Write16(pRefitClientInfo, 0);					  // LuckyValue
		Write16(pRefitClientInfo, 0);					  // RefitLuckyValueMaxWeight
		Write32(pRefitClientInfo, ShapeSuitID);			  // ShapeSuitID
		Write8(pRefitClientInfo, LegendSuitLevel);		  // LegendSuitLevel
		Write32(pRefitClientInfo, LegendSuitLevelChoice); // LegendSuitLevelChoice
		Write32(pRefitClientInfo, ShapeLegendSuitID);	  // ShapeLegendSuitID

		len = pRefitClientInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 316, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	if (Client->KartID == KartID)
	{
		ItemStatus* aItemStatus = (ItemStatus*)safe_malloc(sizeof(ItemStatus) * 2);
		if (ShapeSuitID != 0)
		{
			aItemStatus[0].ItemID = KartID;
			aItemStatus[0].NewStatus = 0;
			aItemStatus[1].ItemID = ShapeSuitID;
			aItemStatus[1].NewStatus = 1;
			Client->KartSkinID = ShapeSuitID;
			Client->OrgKartId = KartID;
			/*	std::shared_ptr<ClientNode> temp = GetClient(Client->Uin);
				temp->KartSkinID = ShapeSuitID;
				temp->OrgKartId = KartID;*/
		}
		else
		{
			aItemStatus[0].ItemID = Client->KartSkinID;
			aItemStatus[0].NewStatus = 0;
			aItemStatus[1].ItemID = KartID;
			aItemStatus[1].NewStatus = 1;
			Client->KartSkinID = 0;
			Client->OrgKartId = 0;
			/*std::shared_ptr<ClientNode> temp = GetClient(Client->Uin);
			temp->KartSkinID = 0;
			temp->OrgKartId = 0;*/
		}

		std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
		if (!Room)
		{
			// NotifyChangeItemStatus(Client, Client->Uin, 2, aItemStatus);
		}
		else
		{
			for (char i = 0; i < 6; i++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
				if (RoomClient && RoomClient != Client)
				{
					NotifyChangeItemStatus2(RoomClient, Client->Uin, 2, aItemStatus);
				}
			}
		}
	}
}

UINT GetChange(UINT ShapeSuitID) {
	int ID = 0;
	switch (ShapeSuitID)
	{
	case 138124:
		ID = 135165;
		break;
	case 138125:
		ID = 135166;
		break;
	case 138126:
		ID = 135167;
		break;
	case 138127:
		ID = 135168;
		break;
	default:
		break;
	}
	return ID;
}

NOINLINE void RequestSaveShapeRefitNew(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	/*char* pMsg = EByteToHexUA2((const unsigned char*)Body, BodyLen);
	printf("RequestSaveShapeRefitNew	Hex:%s\n", pMsg);*/
	//std::unique_lock<std::shared_mutex> lock(RefitDataMutex); //在执行写操作前获取独占锁


	UCHAR CurRefitItemCount = 0;
	UINT RefitItemID[4]{ 0 };
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UINT KartItemID = Read32(Body);//切换赛车ID
	UCHAR RefitItemNum = Read8(Body);//外改数量

	{
		if (isLog == 1) {
			auto ServerLogger = spdlog::get("SpeedServer_logger");
			ServerLogger->info("[玩家切换皮肤][Uin:{:d}][赛车ID:{:d}][外改数量:{:d}]", Client->Uin, KartItemID, RefitItemNum);
			ServerLogger->flush();
		}

	}

	if (RefitItemNum > 4) {
		return;
	}
	for (size_t i = 0; i < RefitItemNum; i++) {
		RefitItemID[i] = Read32(Body);
		if (RefitItemID[i] > 1) {
			++CurRefitItemCount;
		}
	}
	UINT ShapeSuitID = Read32(Body);//切换赛车皮肤ID

	UCHAR LegendSuitLevel = 0;//切换赛车传奇皮肤等级
	UINT LegendSuitLevelChoice = 0;//切换赛车传奇皮肤等级标志
	UINT ShapeLegendSuitID = 0;//切换赛车传奇皮肤ID
	UINT SkinSuitID = 0;
	const char* sql = nullptr;
	bool hasRefitData = false; // 标记是否有改装数据

	if (KartItemID < 10020) {
		return;
	}

	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(KartItemID);
	std::vector<int> resultRow = executeQueryRow("SELECT 1 FROM KartRefit WHERE Uin = ? AND KartID = ?;", "RequestSaveShapeRefitNew", conditionValues);
	if (resultRow.size() >= 1) {
		hasRefitData = true;// 检查是否存在改装数据
	}

	if (CurRefitItemCount >= 1) {
		//外改数据
		ShapeSuitID = 0;
	}
	else
	{
		KartShapeRefitLegendInfo ShapeSuit = getLegendaryShapeSuitID(ShapeSuitID);
		if (ShapeSuit.KartID != 0)
		{
			conditionValues.clear();
			resultRow.clear();
			conditionValues.push_back(Uin);
			conditionValues.push_back(KartItemID);
			std::vector<int> resultRow = executeQueryRow("SELECT LegendSuitLevelChoice,LegendSuitLevel FROM KartRefit WHERE Uin = ? AND KartID=?;", "RequestSaveShapeRefitNew", conditionValues);
			if (resultRow.size() >= 2) {
				LegendSuitLevelChoice = resultRow[0];
				LegendSuitLevel = resultRow[1];
				ShapeLegendSuitID = ShapeSuitID;
				switch (LegendSuitLevelChoice)
				{
				case 2:
					ShapeSuitID = ShapeSuit.LevelInfo[0].ShapeSuitID;
					break;
				case 4:
				case 6:
					ShapeSuitID = ShapeSuit.LevelInfo[1].ShapeSuitID;
					break;
				case 8:
				case 14:
					ShapeSuitID = ShapeSuit.LevelInfo[2].ShapeSuitID;
					break;
				case 16:
				case 24:
				case 30:
					ShapeSuitID = ShapeSuit.LevelInfo[3].ShapeSuitID;
					break;
				case 32:
				case 48:
					ShapeSuitID = ShapeSuit.LevelInfo[4].ShapeSuitID;
					break;
				case 112:
					ShapeSuitID = ShapeSuit.LevelInfo[5].ShapeSuitID;
					break;
				case 128:
					ShapeSuitID = ShapeSuit.LevelInfo[6].ShapeSuitID;
					break;
				case 384:
					ShapeSuitID = ShapeSuit.LevelInfo[7].ShapeSuitID;
					break;
				case 896:
					ShapeSuitID = ShapeSuit.LevelInfo[8].ShapeSuitID;
					break;
				case 1920:
					ShapeSuitID = ShapeSuit.LevelInfo[9].ShapeSuitID;
					break;
				}
				//ShapeLegendSuitID = ShapeSuitID;
				SkinSuitID = ShapeSuitID;
			}
		}
		else {
			SkinSuitID = ShapeSuitID;
		}
	}

	if (!hasRefitData)
	{
		PlayerDB_InsertKartRefit(Uin, KartItemID);
	}
	PlayerDB_UpdateKartRefit(Uin, RefitItemID, ShapeSuitID, ShapeLegendSuitID, SkinSuitID, KartItemID);

	//通知客户端赛车数据
	ResponseSaveShapeRefitNew(Client, KartItemID, ShapeSuitID);
	if (Client->KartID == KartItemID) {

		Client->KartSkinID = ShapeSuitID;
		//通知房间其他人此玩家更换赛车数据
		NotifyChangeSuitItemStatus(Client, ShapeSuitID, KartItemID);
	}

}

void ResponseSaveShapeRefitNew(std::shared_ptr<ClientNode> Client, CONST UINT KartID, CONST UINT ShapeSuitID)
{
	CONST UINT Uin = Client->Uin;
	CONST UINT uKartID = KartID;
	CONST UINT uShapeSuitID = ShapeSuitID;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	bool Result = false;

	KartRefitItemInfo* KartRefit = nullptr;
	SelectOneKartRefitItemInfo(Uin, uKartID, KartRefit);
	if (KartRefit) {
		if (KartRefit[0].SkinSuitID < 1)
			KartRefit[0].SkinSuitID = uShapeSuitID;

		Write16(p, 0); //ResultID
		Write8(p, 0); //ReasonLen

		Write32(p, uKartID); //KartItemID
		{ //KartRefitExInfo
			BYTE* pKartRefitExInfo = p;
			Write16(pKartRefitExInfo, 0); //len

			Write8(pKartRefitExInfo, 0); //SpeedRefitStar
			Write8(pKartRefitExInfo, 0); //JetRefitStar
			Write8(pKartRefitExInfo, 0); //SJetRefitStar
			Write8(pKartRefitExInfo, 0); //AccuRefitStar
			Write8(pKartRefitExInfo, 0); //SpeedAddRatio
			Write8(pKartRefitExInfo, 0); //JetAddRatio
			Write8(pKartRefitExInfo, 0); //SJetAddRatio
			Write8(pKartRefitExInfo, 0); //AccuAddRatio

			len = pKartRefitExInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Write8(p, 4); //RefitItemNum

		Write32(p, KartRefit[0].KartHeadRefitItemID); //RefitItemID[]
		Write32(p, KartRefit[0].KartTailRefitItemID); //RefitItemID[]   
		Write32(p, KartRefit[0].KartFlankRefitItemID); //RefitItemID[]  
		Write32(p, KartRefit[0].KartTireRefitItemID); //RefitItemID[]   

		Write32(p, uShapeSuitID); //ShapeSuitID
		{ //RefitClientInfo
			BYTE* pRefitClientInfo = p;
			Write16(pRefitClientInfo, 0); //len

			Write32(pRefitClientInfo, Uin); //Uin
			Write32(pRefitClientInfo, uKartID); //KartId
			Write32(pRefitClientInfo, KartRefit[0].RefitCout); //RefitCout

			Write16(pRefitClientInfo, KartRefit[0].MaxFlags); //MaxFlags
			Write16(pRefitClientInfo, KartRefit[0].WWeight); //WWeight
			Write16(pRefitClientInfo, KartRefit[0].SpeedWeight); //SpeedWeight
			Write16(pRefitClientInfo, KartRefit[0].JetWeight); //JetWeight
			Write16(pRefitClientInfo, KartRefit[0].SJetWeight); //SJetWeight
			Write16(pRefitClientInfo, KartRefit[0].AccuWeight); //AccuWeight

			Write32(pRefitClientInfo, KartRefit[0].ShapeRefitCount); //ShapeRefitCount
			Write32(pRefitClientInfo, KartRefit[0].KartHeadRefitItemID); //KartHeadRefitItemID
			Write32(pRefitClientInfo, KartRefit[0].KartTailRefitItemID); //KartTailRefitItemID
			Write32(pRefitClientInfo, KartRefit[0].KartFlankRefitItemID); //KartFlankRefitItemID
			Write32(pRefitClientInfo, KartRefit[0].KartTireRefitItemID); //KartTireRefitItemID
			{ //KartRefitExInfo
				BYTE* pKartRefitExInfo = pRefitClientInfo;
				Write16(pKartRefitExInfo, 0); //len

				Write8(pKartRefitExInfo, 0); //SpeedRefitStar
				Write8(pKartRefitExInfo, 0); //JetRefitStar
				Write8(pKartRefitExInfo, 0); //SJetRefitStar
				Write8(pKartRefitExInfo, 0); //AccuRefitStar
				Write8(pKartRefitExInfo, 0); //SpeedAddRatio
				Write8(pKartRefitExInfo, 0); //JetAddRatio
				Write8(pKartRefitExInfo, 0); //SJetAddRatio
				Write8(pKartRefitExInfo, 0); //AccuAddRatio

				len = pKartRefitExInfo - pRefitClientInfo;
				Set16(pRefitClientInfo, (WORD)len);
				pRefitClientInfo += len;
			}
			Write32(pRefitClientInfo, 0); //SecondRefitCount

			Write16(pRefitClientInfo, KartRefit[0].Speed2Weight); //Speed2Weight
			Write16(pRefitClientInfo, KartRefit[0].DriftVecWeight); //DriftVecWeight
			Write16(pRefitClientInfo, KartRefit[0].AdditionalZSpeedWeight); //AdditionalZSpeedWeight
			Write16(pRefitClientInfo, KartRefit[0].AntiCollisionWeight); //AntiCollisionWeight
			Write16(pRefitClientInfo, 0); //LuckyValue
			Write16(pRefitClientInfo, 0); //RefitLuckyValueMaxWeight

			Write32(pRefitClientInfo, KartRefit[0].ShapeSuitID); //ShapeSuitID
			Write8(pRefitClientInfo, KartRefit[0].LegendSuitLevel); //LegendSuitLevel
			Write32(pRefitClientInfo, KartRefit[0].LegendSuitLevelChoice); //LegendSuitLevelChoice
			Write32(pRefitClientInfo, KartRefit[0].ShapeLegendSuitID); //ShapeLegendSuitID

			len = pRefitClientInfo - p;
			Set16(p, (WORD)len);
			p += len;

		}
		delete[] KartRefit;
		len = p - buf.data();
		SendToClient(Client, 316, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}

}

void NotifyChangeSuitItemStatus(std::shared_ptr<ClientNode> Client, CONST UINT ShapeSuitID, CONST UINT KartID)
{
	UINT Uin = Client->Uin;
	UINT uShapeSuitID = ShapeSuitID;
	UINT uKartID = KartID;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	std::shared_ptr<RoomNode> PlayerRoom = GetRoom(Client->RoomID);
	if (PlayerRoom == nullptr) {
		return;
	}
	UINT ItemID = 0;
	UINT OtherAttribute = 0;
	UINT SkinSuitID = 0;

	bool Result = false;
	KartRefitItemInfo* KartRefit = nullptr;
	/*ItemStatus* aItemStatus = new ItemStatus[2];
	if (!aItemStatus)
	{
		return;
	}
	if (ShapeSuitID > 0) {
		aItemStatus[0].ItemID = KartID;
		aItemStatus[0].NewStatus = 1;
		aItemStatus[1].ItemID = 0;
		aItemStatus[1].NewStatus = 0;
	}*/

	SelectOneKartRefitItemInfo(Uin, uKartID, KartRefit);
	if (KartRefit) {
		if (KartRefit[0].SkinSuitID < 1)
			KartRefit[0].SkinSuitID = uShapeSuitID;
		if (ShapeSuitID > 1) {
			ItemID = ShapeSuitID;
			//ItemID = SkinSuitID;
			OtherAttribute = 0x58000000;

		}
		else {
			ItemID = uKartID;
			OtherAttribute = 0x50000000;
		}

		Write32(p, Uin);
		Write16(p, 1);
		for (size_t i = 0; i < 1; i++)
		{

			BYTE* pItemStatus = p;
			Write16(pItemStatus, 0); //len

			Write32(pItemStatus, ItemID);//ItemID
			Write32(pItemStatus, 1); //ItemNum
			Write32(pItemStatus, -1); //AvailPeriod
			Write8(pItemStatus, 1); //Status
			Write32(pItemStatus, 1); //ObtainTime
			Write32(pItemStatus, 0); //OtherAttribute
			Write16(pItemStatus, 0); //ItemType

			len = pItemStatus - p;
			Set16(p, (WORD)len);
			p += len;
		}

		Write8(p, 0); //SpecFlag

		Write16(p, 1); //KartNum
		for (size_t i = 0; i < 1; i++)
		{ //KartRefitInfo
			BYTE* pKartRefitInfo = p;
			Write16(pKartRefitInfo, 0); //len

			Write32(pKartRefitInfo, Uin); //Uin
			Write32(pKartRefitInfo, uKartID); //KartId
			Write32(pKartRefitInfo, KartRefit[0].RefitCout); //RefitCout
			Write16(pKartRefitInfo, KartRefit[0].MaxFlags); //MaxFlags
			Write16(pKartRefitInfo, KartRefit[0].WWeight); //WWeight
			Write16(pKartRefitInfo, KartRefit[0].SpeedWeight); //SpeedWeight
			Write16(pKartRefitInfo, KartRefit[0].JetWeight); //JetWeight
			Write16(pKartRefitInfo, KartRefit[0].SJetWeight); //SJetWeight
			Write16(pKartRefitInfo, KartRefit[0].AccuWeight); //AccuWeight
			Write32(pKartRefitInfo, KartRefit[0].ShapeRefitCount); //ShapeRefitCount
			Write32(pKartRefitInfo, KartRefit[0].KartHeadRefitItemID); //KartHeadRefitItemID
			Write32(pKartRefitInfo, KartRefit[0].KartTailRefitItemID); //KartTailRefitItemID
			Write32(pKartRefitInfo, KartRefit[0].KartFlankRefitItemID); //KartFlankRefitItemID
			Write32(pKartRefitInfo, KartRefit[0].KartTireRefitItemID); //KartTireRefitItemID
			Write32(pKartRefitInfo, 0); //SecondRefitCount
			Write16(pKartRefitInfo, KartRefit[0].Speed2Weight); //Speed2Weight
			Write16(pKartRefitInfo, KartRefit[0].DriftVecWeight); //DriftVecWeight
			Write16(pKartRefitInfo, KartRefit[0].AdditionalZSpeedWeight); //AdditionalZSpeedWeight
			Write16(pKartRefitInfo, KartRefit[0].AntiCollisionWeight); //AntiCollisionWeight
			Write16(pKartRefitInfo, 0); //LuckyValue
			Write16(pKartRefitInfo, 0); //RefitLuckyValueMaxWeight
			Write32(pKartRefitInfo, uShapeSuitID); //ShapeSuitID
			Write8(pKartRefitInfo, KartRefit[0].LegendSuitLevel); //LegendSuitLevel
			Write32(pKartRefitInfo, KartRefit[0].LegendSuitLevelChoice); //LegendSuitLevelChoice
			Write32(pKartRefitInfo, KartRefit[0].ShapeLegendSuitID); //ShapeLegendSuitID

			len = pKartRefitInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}


		Write8(p, 0); //ChangeType
		Write8(p, 0); //EquipNiChangItemNum

		len = p - buf.data();

		for (char i = 0; i < 6; i++) {
			auto& RoomPlayer = PlayerRoom->Player[i];
			if (RoomPlayer && RoomPlayer != Client) {

				SendToClient(RoomPlayer, 527, buf.data(), len, RoomPlayer->RoomID, FE_ROOM, RoomPlayer->ConnID, Notify);
			}
		}
		delete[] KartRefit;
	}
}

void RequestShapeLegendOperate(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {//皮肤进阶
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UCHAR OpType = Read8(Body);
	int ShapeID = Read32(Body);
	UCHAR ShapeLevel = Read8(Body);
	int NewShowChoice = Read32(Body);

	int KartID = 0;
	int SkinLevel = 0;
	int lantuid = 0;
	int nengyuanid = 0;
	YAML::Node Level;
	ItemInfo Items = { 0 };
	std::string Reason = "";
	KartShapeRefitLegendInfo ShapeSuit = getLegendaryShapeSuitID(ShapeID);
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	if (ShapeSuit.KartID != 0)
	{
		KartID = ShapeSuit.KartID;
		if (ShapeSuit.LevelInfo.size() >= ShapeLevel)
		{
			int level = ShapeLevel == 0 ? 0 : ShapeLevel - 1;
			if (OpType == 2)
			{
				if (NewShowChoice == 2) {
					SkinLevel = 1;
				}
				else if (NewShowChoice == 4 || NewShowChoice == 6) {
					SkinLevel = 2;
				}
				else if (NewShowChoice == 8 || NewShowChoice == 14) {
					SkinLevel = 3;
				}
				else if (NewShowChoice == 16 || NewShowChoice == 24 || NewShowChoice == 30) {
					SkinLevel = 4;
				}
				else if (NewShowChoice == 32 || NewShowChoice == 48) {
					SkinLevel = 5;
				}
				else if (NewShowChoice == 112) {
					SkinLevel = 6;
				}
				else if (NewShowChoice == 128) {
					SkinLevel = 7;
				}
				else if (NewShowChoice == 384) {
					SkinLevel = 8;
				}
				else if (NewShowChoice == 896) {
					SkinLevel = 9;
				}
				else {
					SkinLevel = 10;
				}
				level = SkinLevel - 1;
			}
			if (ShapeSuit.LevelInfo[level].ConsumInfo.size() == 1) {
				lantuid = ShapeSuit.LevelInfo[level].ConsumInfo[0].ItemID;
				Level["ShapeSuitLevel"][level]["lantu"] = ShapeSuit.LevelInfo[level].ConsumInfo[0].ItemNum;
				Level["ShapeSuitLevel"][level]["nengyuan"] = 0;
				Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"] = ShapeSuit.LevelInfo[level].ShapeSuitID;
			}
			else if (ShapeSuit.LevelInfo[level].ConsumInfo.size() == 2) {
				lantuid = ShapeSuit.LevelInfo[level].ConsumInfo[0].ItemID;
				nengyuanid = ShapeSuit.LevelInfo[level].ConsumInfo[1].ItemID;
				Level["ShapeSuitLevel"][level]["lantu"] = ShapeSuit.LevelInfo[level].ConsumInfo[0].ItemNum;
				Level["ShapeSuitLevel"][level]["nengyuan"] = ShapeSuit.LevelInfo[level].ConsumInfo[1].ItemNum;
				Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"] = ShapeSuit.LevelInfo[level].ShapeSuitID;
			}
		}
	}

	try
	{
		//升级
		if (OpType == 1) {
			if (PlayerDB_GetItemNum(Uin, lantuid) < Level["ShapeSuitLevel"][ShapeLevel - 1]["lantu"].as<int>() || PlayerDB_GetItemNum(Uin, nengyuanid) < Level["ShapeSuitLevel"][ShapeLevel - 1]["nengyuan"].as<int>() || !PlayerDB_GetItemNum(Uin, KartID)) {
				Reason = "/C01没有车辆或者材料不足！！!";
				ResponseStartGame(Client, Reason.c_str());
				return;
				ShapeLevel = 0;

			}
			else {
				UINT num = PlayerDB_GetItemNum(Uin, lantuid);
				if (num == Level["ShapeSuitLevel"][ShapeLevel - 1]["lantu"].as<int>()) {
					PlayerDB_DeleteItem(Client->Uin, lantuid);
				}
				else {
					PlayerDB_Reduce(Client->Uin, lantuid, Level["ShapeSuitLevel"][ShapeLevel - 1]["lantu"].as<int>());
				}

				Items.ItemID = lantuid;
				Items.ItemNum = num - Level["ShapeSuitLevel"][ShapeLevel - 1]["lantu"].as<int>();
				Items.AvailPeriod = 0;
				Items.ObtainTime = Time;
				Items.Status = 0;
				Items.OtherAttribute = 0;
				Items.ItemType = 0;
				NotifyClientAddItem(Client, 1, { Items });
				num = PlayerDB_GetItemNum(Uin, nengyuanid);
				if (num == Level["ShapeSuitLevel"][ShapeLevel - 1]["nengyuan"].as<int>()) {
					PlayerDB_DeleteItem(Client->Uin, nengyuanid);
				}
				else {
					PlayerDB_Reduce(Client->Uin, nengyuanid, Level["ShapeSuitLevel"][ShapeLevel - 1]["nengyuan"].as<int>());
				}
				Items.ItemID = nengyuanid;
				Items.ItemNum = num - Level["ShapeSuitLevel"][ShapeLevel - 1]["nengyuan"].as<int>();
				Items.AvailPeriod = 0;
				Items.ObtainTime = Time;
				Items.Status = 0;
				Items.OtherAttribute = 0;
				Items.ItemType = 0;
				NotifyClientAddItem(Client, 1, { Items });

				if (ShapeLevel == 1) {
					PlayerDB_AddItem4(Uin, ShapeID, 1, -1, false);
					Items.ItemID = ShapeID;
					Items.ItemNum = 1;
					Items.AvailPeriod = -1;
					Items.ObtainTime = Time;
					Items.Status = 0;
					Items.OtherAttribute = 0;
					Items.ItemType = 0;

					NotifyLoginMoreInfo(Client, 1, { Items }, false);
					NotifyClientAddItem(Client, 1, { Items });

				}


				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE KartRefit SET LegendSuitLevel=? WHERE Uin=? and KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, ShapeLevel);
				prep_stmt->setUInt(2, Uin);
				prep_stmt->setUInt(3, KartID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
		}
		//更换形态
		if (OpType == 2) {
			int SuitID = Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"].as<int>();
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE KartRefit SET ShapeSuitID=?,LegendSuitLevelChoice=?,ShapeLegendSuitID=?,SkinSuitID=? WHERE Uin=? and KartID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, SuitID);
			prep_stmt->setUInt(2, NewShowChoice);
			prep_stmt->setUInt(3, ShapeID);
			prep_stmt->setUInt(4, SuitID);
			prep_stmt->setUInt(5, Uin);
			prep_stmt->setUInt(6, KartID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();

			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT LegendSuitLevel  FROM KartRefit WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setUInt(2, KartID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				prep_stmt.reset();
				if (res->rowsCount() > 0) {
					res->next();
					ShapeLevel = res->getUInt("LegendSuitLevel");
				}
				res.reset();
			}


			//皮肤切换处理2.0
			if (Client->KartID == KartID) {
				ItemStatus* aItemStatus = (ItemStatus*)safe_malloc(sizeof(ItemStatus) * 2);

				if (ShapeID != 0) {
					//不为0则为切皮肤
					//切为无s标志的皮肤车
					aItemStatus[0].ItemID = KartID;
					aItemStatus[0].NewStatus = 0;
					aItemStatus[1].ItemID = Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"].as<int>();
					aItemStatus[1].NewStatus = 1;
					Client->KartSkinID = Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"].as<int>();
					Client->OrgKartId = KartID;
					std::shared_ptr<ClientNode> temp = GetClient(Client->Uin);
					temp->KartSkinID = Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"].as<int>();
					temp->OrgKartId = KartID;
				}
				else {
					//为0则为切原车
					aItemStatus[0].ItemID = KartID;
					aItemStatus[0].NewStatus = 0;
					aItemStatus[1].ItemID = Client->KartSkinID;
					aItemStatus[1].NewStatus = 1;
					//Client->SkinID = 0;
					/*Client->OrgKartId = 10020;
					std::shared_ptr<ClientNode> temp = GetClient(Client->Uin);*/
					//temp->SkinID = 0;
					//temp->OrgKartId = 0;
				}
				std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
				if (!Room)
				{
					// NotifyChangeItemStatus(Client, Client->Uin, 2, aItemStatus);
				}
				else
				{
					for (char i = 0; i < 6; i++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
						if (RoomClient && RoomClient != Client) {
							NotifyChangeItemStatus2(RoomClient, Client->Uin, 2, aItemStatus);
							if (ShapeID != 0) {
								//切回带皮肤带s标的原车
								aItemStatus[0].ItemID = Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"].as<int>();
								aItemStatus[0].NewStatus = 0;
								aItemStatus[1].ItemID = KartID;
								aItemStatus[1].NewStatus = 1;
							}
							else {
								aItemStatus[0].ItemID = Client->KartSkinID;
								aItemStatus[0].NewStatus = 0;
								aItemStatus[1].ItemID = KartID;
								aItemStatus[1].NewStatus = 1;
								Client->KartSkinID = 0;
								Client->OrgKartId = 0;
								/*std::shared_ptr<ClientNode> temp = GetClient(Client->Uin);
								temp->KartSkinID = 0;
								temp->OrgKartId = 0;*/
							}
							NotifyChangeItemStatus2(RoomClient, Client->Uin, 2, aItemStatus);

						}

					}
				}
			}
			// 到这里皮肤就处理完了
		}

		Write16(p, 0); //ResultID
		Write8(p, 0); //ReasonLen
		Write8(p, OpType); //OpType
		std::vector<KartShapeRefitLegendInfo> KartShapeRefitLegendInfo = getLegendarySkin();
		Write16(p, KartShapeRefitLegendInfo.size()); //ShapeNum 用户皮肤数量
		for (size_t i = 0; i < KartShapeRefitLegendInfo.size(); i++)
		{ //ShapeInfo
			BYTE* pShapeInfo = p;
			Write16(pShapeInfo, 0); //len
			Write32(pShapeInfo, KartShapeRefitLegendInfo[i].ShapeSuitID); //ShapeID

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT LegendSuitLevel,LegendSuitLevelChoice  FROM KartRefit WHERE Uin=? AND KartID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, KartShapeRefitLegendInfo[i].KartID);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			if (res->rowsCount() > 0) {
				res->next();
				Write8(pShapeInfo, res->getUInt("LegendSuitLevel")); //ShapeLevel
				Write32(pShapeInfo, res->getUInt("LegendSuitLevelChoice"));
			}
			else
			{
				Write8(pShapeInfo, 0); //ShapeLevel
				Write32(pShapeInfo, 0);
			}
			res.reset();


			len = pShapeInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Write32(p, ShapeID); //ShapeID外形ID
		Write8(p, ShapeLevel); //ShapeLevel

	}
	catch (const std::exception  e)
	{
		printf(e.what());
		printf("RequestShapeLegendOperate1 Error\n");
		//throw;关闭
	}
	//特殊段落(写法草懒得优化)
	if (OpType == 2) {
		Write8(p, 1); //HasRefit
		{
			BYTE* pRefitClientInfo = p;
			Write16(pRefitClientInfo, 0); //len
			Write32(pRefitClientInfo, Uin); //Uin
			Write32(pRefitClientInfo, KartID); //KartId
			Write32(pRefitClientInfo, 0); //RefitCout
			Write16(pRefitClientInfo, 0); //MaxFlags
			Write16(pRefitClientInfo, 0); //WWeight
			Write16(pRefitClientInfo, 0); //SpeedWeight
			Write16(pRefitClientInfo, 0); //JetWeight
			Write16(pRefitClientInfo, 0); //SJetWeight
			Write16(pRefitClientInfo, 0); //AccuWeight
			Write32(pRefitClientInfo, 0); //ShapeRefitCount
			Write32(pRefitClientInfo, 0); //KartHeadRefitItemID
			Write32(pRefitClientInfo, 0); //KartTailRefitItemID
			Write32(pRefitClientInfo, 0); //KartFlankRefitItemID
			Write32(pRefitClientInfo, 0); //KartTireRefitItemID
			{ //KartRefitExInfo
				BYTE* pKartRefitExInfo = pRefitClientInfo;
				Write16(pKartRefitExInfo, 0); //len
				Write8(pKartRefitExInfo, 0); //SpeedRefitStar
				Write8(pKartRefitExInfo, 0); //JetRefitStar
				Write8(pKartRefitExInfo, 0); //SJetRefitStar
				Write8(pKartRefitExInfo, 0); //AccuRefitStar
				Write8(pKartRefitExInfo, 0); //SpeedAddRatio
				Write8(pKartRefitExInfo, 0); //JetAddRatio
				Write8(pKartRefitExInfo, 0); //SJetAddRatio
				Write8(pKartRefitExInfo, 0); //AccuAddRatio
				len = pKartRefitExInfo - pRefitClientInfo;
				Set16(pRefitClientInfo, (WORD)len);
				pRefitClientInfo += len;
			}
			Write32(pRefitClientInfo, 0); //SecondRefitCount
			Write16(pRefitClientInfo, 0); //Speed2Weight
			Write16(pRefitClientInfo, 0); //DriftVecWeight
			Write16(pRefitClientInfo, 0); //AdditionalZSpeedWeight
			Write16(pRefitClientInfo, 0); //AntiCollisionWeight
			Write16(pRefitClientInfo, 0); //LuckyValue
			Write16(pRefitClientInfo, 0); //RefitLuckyValueMaxWeight
			Write32(pRefitClientInfo, Level["ShapeSuitLevel"][SkinLevel - 1]["SuitID"].as<int>()); //ShapeSuitID
			Write8(pRefitClientInfo, ShapeLevel); //LegendSuitLevel
			Write32(pRefitClientInfo, NewShowChoice); //LegendSuitLevelChoice
			Write32(pRefitClientInfo, ShapeID); //ShapeLegendSuitID.

			len = pRefitClientInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	else {
		Write8(p, 2); //HasRefit

		for (size_t i = 0; i < 2; i++)
		{//RefitClientInfo
			if (i == 1) {
				KartID = 12720;
			}
			else {
				KartID = 74362;
			}
			BYTE* pRefitClientInfo = p;
			Write16(pRefitClientInfo, 0); //len

			Write32(pRefitClientInfo, Uin); //Uin
			Write32(pRefitClientInfo, KartID); //KartId

			Write32(pRefitClientInfo, 0); //RefitCout
			Write16(pRefitClientInfo, 0); //MaxFlags
			Write16(pRefitClientInfo, 0); //WWeight
			Write16(pRefitClientInfo, 0); //SpeedWeight
			Write16(pRefitClientInfo, 0); //JetWeight
			Write16(pRefitClientInfo, 0); //SJetWeight
			Write16(pRefitClientInfo, 0); //AccuWeight

			Write32(pRefitClientInfo, 0); //ShapeRefitCount
			Write32(pRefitClientInfo, 0); //KartHeadRefitItemID
			Write32(pRefitClientInfo, 0); //KartTailRefitItemID
			Write32(pRefitClientInfo, 0); //KartFlankRefitItemID
			Write32(pRefitClientInfo, 0); //KartTireRefitItemID
			{ //KartRefitExInfo
				BYTE* pKartRefitExInfo = pRefitClientInfo;
				Write16(pKartRefitExInfo, 0); //len

				Write8(pKartRefitExInfo, 0); //SpeedRefitStar
				Write8(pKartRefitExInfo, 0); //JetRefitStar
				Write8(pKartRefitExInfo, 0); //SJetRefitStar
				Write8(pKartRefitExInfo, 0); //AccuRefitStar
				Write8(pKartRefitExInfo, 0); //SpeedAddRatio
				Write8(pKartRefitExInfo, 0); //JetAddRatio
				Write8(pKartRefitExInfo, 0); //SJetAddRatio
				Write8(pKartRefitExInfo, 0); //AccuAddRatio

				len = pKartRefitExInfo - pRefitClientInfo;
				Set16(pRefitClientInfo, (WORD)len);
				pRefitClientInfo += len;
			}

			Write32(pRefitClientInfo, 0); //SecondRefitCount
			Write16(pRefitClientInfo, 0); //Speed2Weight
			Write16(pRefitClientInfo, 0); //DriftVecWeight
			Write16(pRefitClientInfo, 0); //AdditionalZSpeedWeight
			Write16(pRefitClientInfo, 0); //AntiCollisionWeight
			Write16(pRefitClientInfo, 0); //LuckyValue
			Write16(pRefitClientInfo, 0); //RefitLuckyValueMaxWeight
			Write32(pRefitClientInfo, 0); //ShapeSuitID
			Write8(pRefitClientInfo, 0); //LegendSuitLevel
			Write32(pRefitClientInfo, 0); //LegendSuitLevelChoice
			Write32(pRefitClientInfo, 0); //ShapeLegendSuitID.

			len = pRefitClientInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	Write32(p, NewShowChoice); //NewShowChoice
	Write32(p, 0); //DirectMaxLevelShapeID 直接最大级别形状ID
	Write32(p, 0); //DirectMaxShapePreLevel 直接最大形状预级别

	len = p - buf.data();
	SendToClient(Client, 319, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



void ResponseGetDailyExtractShowInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;


	Write32(p, Uin); // Uin
	Write16(p, 0);	 // ResultID
	Write8(p, 1);	 // ChoosedAward  选择奖励
	Write16(p, 1);	 // ChooseAwardNums 选择奖励数量
	for (size_t i = 0; i < 1; i++)
	{ // ChooseAwardInfos
		BYTE* pChooseAwardInfos = p;
		Write16(pChooseAwardInfos, 0); // len

		Write32(pChooseAwardInfos, i); // ChooseIndex
		Write16(pChooseAwardInfos, 1); // AwardType
		{							   // AwardInfo
			BYTE* pAwardInfo = pChooseAwardInfos;
			Write16(pAwardInfo, 0); // len

			Write32(pAwardInfo, 0); // Exp
			Write32(pAwardInfo, 0); // Money
			Write32(pAwardInfo, 0); // SuperMoney
			Write16(pAwardInfo, 1); // ItemNum
			for (size_t i = 0; i < 1; i++)
			{ // ItemInfo
				BYTE* pItemInfo = pAwardInfo;
				Write16(pItemInfo, 0); // len

				Write32(pItemInfo, 127748); // ItemID  
				Write32(pItemInfo, 100);		// ItemNum
				Write32(pItemInfo, 0);		// AvailPeriod
				Write8(pItemInfo, 0);		// Status
				Write32(pItemInfo, 0);		// ObtainTime
				Write32(pItemInfo, 0);		// OtherAttribute
				Write16(pItemInfo, 1);		// ItemType

				len = pItemInfo - pAwardInfo;
				Set16(pAwardInfo, (WORD)len);
				pAwardInfo += len;
			}
			Write32(pAwardInfo, 0); // Coupons
			Write32(pAwardInfo, 0); // GuildPoint
			Write32(pAwardInfo, 0); // LuckMoney
			Write8(pAwardInfo, 0);	// ExtendInfoNum
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

			len = pAwardInfo - pChooseAwardInfos;
			Set16(pChooseAwardInfos, (WORD)len);
			pChooseAwardInfos += len;
		}
		Write8(pChooseAwardInfos, 0); // AlreayGotten
		Write8(pChooseAwardInfos, 0); // HaveAlreadyGottenPermanentItem

		len = pChooseAwardInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // ChoosedAwardInfo 选择的奖项信息
		BYTE* pChoosedAwardInfo = p;
		Write16(pChoosedAwardInfo, 0); // len

		Write32(pChoosedAwardInfo, 0); // ChooseIndex
		Write16(pChoosedAwardInfo, 0); // AwardType
		{							   // AwardInfo
			BYTE* pAwardInfo = pChoosedAwardInfo;
			Write16(pAwardInfo, 0); // len

			Write32(pAwardInfo, 0); // Exp
			Write32(pAwardInfo, 0); // Money
			Write32(pAwardInfo, 0); // SuperMoney
			Write16(pAwardInfo, 1); // ItemNum
			for (size_t i = 0; i < 1; i++)
			{ // ItemInfo
				BYTE* pItemInfo = pAwardInfo;
				Write16(pItemInfo, 0); // len

				Write32(pItemInfo, 127748); // ItemID  
				Write32(pItemInfo, 100);		// ItemNum
				Write32(pItemInfo, 0);		// AvailPeriod
				Write8(pItemInfo, 0);		// Status
				Write32(pItemInfo, 0);		// ObtainTime
				Write32(pItemInfo, 0);		// OtherAttribute
				Write16(pItemInfo, 1);		// ItemType

				len = pItemInfo - pAwardInfo;
				Set16(pAwardInfo, (WORD)len);
				pAwardInfo += len;
			}
			Write32(pAwardInfo, 0); // Coupons
			Write32(pAwardInfo, 0); // GuildPoint
			Write32(pAwardInfo, 0); // LuckMoney
			Write8(pAwardInfo, 0);	// ExtendInfoNum
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

			len = pAwardInfo - pChoosedAwardInfo;
			Set16(pChoosedAwardInfo, (WORD)len);
			pChoosedAwardInfo += len;
		}
		Write8(pChoosedAwardInfo, 0); // AlreayGotten
		Write8(pChoosedAwardInfo, 0); // HaveAlreadyGottenPermanentItem

		len = pChoosedAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 5); // DailyExtarctAwardNums  每日Extarct奖励数量
	for (size_t Extarcti = 0; Extarcti < 5; Extarcti++)
	{ // DailyExtractAwardInfos  奖励列表
		BYTE* pDailyExtractAwardInfos = p;
		Write16(pDailyExtractAwardInfos, 0); // len

		Write32(pDailyExtractAwardInfos, 0); // ChooseIndex
		Write16(pDailyExtractAwardInfos, 0); // AwardType
		{									 // AwardInfo
			BYTE* pAwardInfo = pDailyExtractAwardInfos;
			Write16(pAwardInfo, 0); // len

			Write32(pAwardInfo, 0); // Exp
			Write32(pAwardInfo, 0); // Money
			Write32(pAwardInfo, 0); // SuperMoney
			Write16(pAwardInfo, 1); // ItemNum
			for (size_t i = 0; i < 1; i++)
			{ // ItemInfo
				BYTE* pItemInfo = pAwardInfo;
				Write16(pItemInfo, 0); // len

				Write32(pItemInfo, DayRaffleBoxs[Extarcti]->ItemID); // ItemID
				Write32(pItemInfo, DayRaffleBoxs[Extarcti]->ItemNum);							  // ItemNum
				Write32(pItemInfo, DayRaffleBoxs[Extarcti]->AvailPeriod);							  // AvailPeriod
				Write8(pItemInfo, 0);							  // Status
				Write32(pItemInfo, 0);							  // ObtainTime
				Write32(pItemInfo, 0);							  // OtherAttribute
				Write16(pItemInfo, 1);							  // ItemType

				len = pItemInfo - pAwardInfo;
				Set16(pAwardInfo, (WORD)len);
				pAwardInfo += len;
			}
			Write32(pAwardInfo, 0); // Coupons
			Write32(pAwardInfo, 0); // GuildPoint
			Write32(pAwardInfo, 0); // LuckMoney
			Write8(pAwardInfo, 0);	// ExtendInfoNum
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

			len = pAwardInfo - pDailyExtractAwardInfos;
			Set16(pDailyExtractAwardInfos, (WORD)len);
			pDailyExtractAwardInfos += len;
		}
		Write8(pDailyExtractAwardInfos, 0); // AlreayGotten  得到
		Write8(pDailyExtractAwardInfos, 0); // HaveAlreadyGottenPermanentItem  已经得到永久物品

		len = pDailyExtractAwardInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 1678084039); // OpenDate
	Write32(p, 3382237639); // CloseDate
	Write32(p, 1678084039); // OpenTime
	Write32(p, 3382237639); // CloseTime
	Write32(p, 7);			// CostItemNums  消耗多少张手气卡
	Write32(p, 7);			// CurrentCaiDanAwardNums
	Write8(p, 0);			// CanChangeChooseAward   可以更改选择奖项
	Write8(p, 0);			// HaveExtractedAllAward

	len = p - buf.data();
	SendToClient(Client, 25130, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseDailyExtractInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	

	int RemainSuperMoney = -1;


	SelectClientExp(Client);
	std::string Reason = "抽一次需要消耗";
	Reason.append(std::to_string(DayMoeny));
	Reason.append("点卷，您的点卷不足!");

	RemainSuperMoney = ((Client->SuperMoney) - DayMoeny);
	if (RemainSuperMoney <= 0) {
		ResponseStartGame(Client, Reason.c_str());
		return;
	}
	Client->SuperMoney = RemainSuperMoney;
	NotifyClientUpdateMoney(Client);
	UpdateClientBase(Client);

	// 使用 rand() 函数结合 srand() 函数生成随机数
	srand(time(nullptr)); // 设置随机种子为当前时间

	long randomIndex = rand() % DayRaffleBoxs.size(); // 生成 0 到 size()-1 之间的随机索引
	std::shared_ptr<ItemInfo> randomItem = DayRaffleBoxs[randomIndex]; // 获取对应索引的 ItemInfo 对象
	ItemInfo Items = { 0 };

	Write32(p, Uin); // Uin
	Write16(p, 0);	 // ResultID
	{				 // AwardInfo
		BYTE* pAwardInfo = p;
		Write16(pAwardInfo, 0); // len

		Write32(pAwardInfo, 0); // Exp
		Write32(pAwardInfo, 0); // Money
		Write32(pAwardInfo, 0); // SuperMoney
		Write16(pAwardInfo, 1); // ItemNum
		for (size_t i = 0; i < 1; i++)
		{ // ItemInfo
			BYTE* pItemInfo = pAwardInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, randomItem->ItemID); // ItemID
			Write32(pItemInfo, randomItem->ItemNum);		// ItemNum
			Write32(pItemInfo, randomItem->AvailPeriod);		// AvailPeriod
			Write8(pItemInfo, 0);		// Status
			Write32(pItemInfo, 0);		// ObtainTime
			Write32(pItemInfo, 0);		// OtherAttribute
			Write16(pItemInfo, 1);		// ItemType

			len = pItemInfo - pAwardInfo;
			Set16(pAwardInfo, (WORD)len);
			pAwardInfo += len;
		}

		Items.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, randomItem->ItemID, randomItem->ItemNum, randomItem->AvailPeriod, 0);
		Items.ItemID = randomItem->ItemID;
		Items.ObtainTime = 0xFFFF;
		NotifyClientAddItem(Client, 1, { Items });


		Write32(pAwardInfo, 0); // Coupons
		Write32(pAwardInfo, 0); // GuildPoint
		Write32(pAwardInfo, 0); // LuckMoney
		Write8(pAwardInfo, 0);	// ExtendInfoNum
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

		len = pAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 3); // CaiDanAwardNums
	Write32(p, 7); // CostItemNums
	Write8(p, 0);  // CanChangeChooseAward
	Write32(p, 0); // AwardType
	Write32(p, 0); // CurrentCaiDanAwardNums
	Write8(p, 0);  // HaveExtractedAllAward
	Write8(p, 0);  // HaveAlreadyGottenItem

	len = p - buf.data();
	SendToClient(Client, 25131, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseMagicHouseCallOn(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	UINT Uin = Read32(Body); if (Client->Uin != Uin) return;
	UINT Time = Read32(Body);
	UINT MagicHouseID = Read32(Body);

	
	
	int AwardIndex = 0;
	std::unordered_map<int, int> Items;
	std::map<long, std::shared_ptr<ItemInfo>> MagicFormationTemp;//hashmap
	//int num = 1000;

	if (!isDBtHasItem(Client, MagicShop[0], MagicShop[1]))
	{
		ResponseStartGame(Client, "道具不足!");
		return;
	}

	PlayerDB_GetMagicFormationNum(Uin, Items);

	MagicFormationTemp.clear(); // 确保 AnotherMap 是空的
	MagicFormationTemp.insert(MagicFormationBoxs.begin(), MagicFormationBoxs.end());

	for (auto it = MagicFormationTemp.begin(); it != MagicFormationTemp.end();) {
		auto iter = Items.find(it->second->ItemID); // 使用 -> 操作符访问指针成员
		if (iter != Items.end()) {
			it = MagicFormationTemp.erase(it);
		}
		else {
			++it;
		}
	}
	// 重新构建从0开始的索引
	std::map<long, std::shared_ptr<ItemInfo>> newMap;
	long newIndex = 0;
	for (auto& pair : MagicFormationTemp) {
		newMap[newIndex++] = pair.second;
	}
	MagicFormationTemp = std::move(newMap);


	if (MagicFormationTemp.size() == 0) {
		ResponseStartGame(Client, "你已获得所有奖励！");
		return;
	}


	int randomIndex = std::rand() % MagicFormationTemp.size();

	NotifyUseItemResult2(Client, MagicShop[0], MagicShop[1]);

	Write32(p, Uin); // Uin
	Write16(p, 0);	 // ResultID
	Write32(p, 0);	 // CostCallOnItemNums
	// Write32(p, 93343); //CallOnItemID  83073 76965 32867  93343
	{ // AwardInfo
		BYTE* pAwardInfo = p;
		Write16(pAwardInfo, 0); // len

		Write32(pAwardInfo, MagicFormationTemp[randomIndex]->ItemID); // ItemID
		Write32(pAwardInfo, MagicFormationTemp[randomIndex]->ItemNum);		// ItemNums
		Write32(pAwardInfo, MagicFormationTemp[randomIndex]->AvailPeriod);	// AvailPeriod
		Write16(pAwardInfo, 1);		// Received

		// 遍历 MagicFormationBoxs
		for (const auto& pair : MagicFormationBoxs) {
			if (pair.second == MagicFormationTemp[randomIndex]) { // 检查值是否匹配
				AwardIndex = pair.first;
				break;
			}
		}

		Write16(pAwardInfo, AwardIndex);		// AwardIndex
		Write16(pAwardInfo, 0);		// ItemType

		len = pAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0);			  // ReceiveAllAward  获得所有奖项
	Write32(p, MagicHouseID); // MagicHouseID
	Write32(p, 5);			  // CostCallOnItemNumsBeforDiscount  本次参与需要的召唤卡数量
	Write8(p, 0);			  // IsFree  是免费的

	ItemInfo Item = { 0 };
	Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, MagicFormationTemp[randomIndex]->ItemID, MagicFormationTemp[randomIndex]->ItemNum, MagicFormationTemp[randomIndex]->AvailPeriod, 0);
	Item.AvailPeriod = MagicFormationTemp[randomIndex]->AvailPeriod;
	Item.ItemID = MagicFormationTemp[randomIndex]->ItemID;
	Item.ObtainTime = 0xFFFF;
	NotifyClientAddItem(Client, 1, { Item });
	PlayerDB_InsertMagicFormation(Uin, MagicFormationTemp[randomIndex]->ItemID);


	len = p - buf.data();
	SendToClient(Client, 24471, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseGetExchangeShopInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	/*char* pMsg = EByteToHexUA2((const unsigned char*)Body, BodyLen);
	printf("ResponseGetExchangeShopInfo	Hex:%s\n", pMsg);*/

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	
	
	int AwardIndex = 0;
	std::unordered_map<int, int> Items;

	PlayerDB_GetMagicShopInfo(Uin, Items);

	Write16(p, 0); // ResultID
	//for (size_t i = 0; i < 3; i++)
	{ // ExchangeItemList
		BYTE* pExchangeItemList = p;
		Write16(pExchangeItemList, 0);	   // len
		Write32(pExchangeItemList, MagicExchangeShopItemId); // ExchangeItemID
		Write16(pExchangeItemList, MagicExchangeShopCarBoxs.size());	   // Num
		for (size_t i = 0; i < MagicExchangeShopCarBoxs.size(); i++)
		{ // ExchangeItem
			BYTE* pExchangeItem = pExchangeItemList;
			Write16(pExchangeItem, 0); // len

			Write32(pExchangeItem, MagicExchangeShopCarBoxs[i]->ItemID); // ItemID
			Write32(pExchangeItem, MagicExchangeShopCarBoxs[i]->ItemNum);		// ItemNum
			Write32(pExchangeItem, MagicExchangeShopCarBoxs[i]->AvailPeriod);		// AvailPeriod
			Write32(pExchangeItem, MagicExchangeShopCarBoxs[i]->ComsumeItemNum);		// CostExchangeItemNum 兑换需要数量
			auto iter = Items.find(MagicExchangeShopCarBoxs[i]->ItemID);
			if (iter != Items.end()) {
				Write8(pExchangeItem, 1);		// Flag  是否已经兑换
			}
			else
			{
				Write8(pExchangeItem, 0);		// Flag  是否已经兑换
			}

			len = pExchangeItem - pExchangeItemList;
			Set16(pExchangeItemList, (WORD)len);
			pExchangeItemList += len;
		}

		len = pExchangeItemList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ // ExchangeItemList
		BYTE* pExchangeItemList = p;
		Write16(pExchangeItemList, 0);	   // len
		Write32(pExchangeItemList, MagicExchangeShopItemId); // ExchangeItemID
		Write16(pExchangeItemList, MagicExchangeShopPetBoxs.size());	   // Num
		for (size_t i = 0; i < MagicExchangeShopPetBoxs.size(); i++)
		{ // ExchangeItem
			BYTE* pExchangeItem = pExchangeItemList;
			Write16(pExchangeItem, 0); // len

			Write32(pExchangeItem, MagicExchangeShopPetBoxs[i]->ItemID); // ItemID
			Write32(pExchangeItem, MagicExchangeShopPetBoxs[i]->ItemNum);		// ItemNum
			Write32(pExchangeItem, MagicExchangeShopPetBoxs[i]->AvailPeriod);		// AvailPeriod
			Write32(pExchangeItem, MagicExchangeShopPetBoxs[i]->ComsumeItemNum);		// CostExchangeItemNum 兑换需要数量
			auto iter = Items.find(MagicExchangeShopPetBoxs[i]->ItemID);
			if (iter != Items.end()) {
				Write8(pExchangeItem, 1);		// Flag  是否已经兑换
			}
			else
			{
				Write8(pExchangeItem, 0);		// Flag  是否已经兑换
			}

			len = pExchangeItem - pExchangeItemList;
			Set16(pExchangeItemList, (WORD)len);
			pExchangeItemList += len;
		}

		len = pExchangeItemList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ // ExchangeItemList
		BYTE* pExchangeItemList = p;
		Write16(pExchangeItemList, 0);	   // len
		Write32(pExchangeItemList, MagicExchangeShopItemId); // ExchangeItemID
		Write16(pExchangeItemList, MagicExchangeShopSeatBoxs.size());	   // Num
		for (size_t i = 0; i < MagicExchangeShopSeatBoxs.size(); i++)
		{ // ExchangeItem
			BYTE* pExchangeItem = pExchangeItemList;
			Write16(pExchangeItem, 0); // len

			Write32(pExchangeItem, MagicExchangeShopSeatBoxs[i]->ItemID); // ItemID
			Write32(pExchangeItem, MagicExchangeShopSeatBoxs[i]->ItemNum);		// ItemNum
			Write32(pExchangeItem, MagicExchangeShopSeatBoxs[i]->AvailPeriod);		// AvailPeriod
			Write32(pExchangeItem, MagicExchangeShopSeatBoxs[i]->ComsumeItemNum);		// CostExchangeItemNum 兑换需要数量
			auto iter = Items.find(MagicExchangeShopSeatBoxs[i]->ItemID);
			if (iter != Items.end()) {
				Write8(pExchangeItem, 1);		// Flag  是否已经兑换
			}
			else
			{
				Write8(pExchangeItem, 0);		// Flag  是否已经兑换
			}

			len = pExchangeItem - pExchangeItemList;
			Set16(pExchangeItemList, (WORD)len);
			pExchangeItemList += len;
		}

		len = pExchangeItemList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 1678084039); // OpenStartTime
	Write32(p, 1698084039); // OpenEndTime

	len = p - buf.data();
	SendToClient(Client, 24786, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void ResponseDoShopExchange(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	
	int AwardIndex = 0;
	/*char* pMsg = EByteToHexUA2((const unsigned char*)Body, BodyLen);
	printf("ResponseDoShopExchange	Hex:%s\n", pMsg);*/

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	// ExchangeInfo
	len = Read16(Body);
	UINT ItemID = Read32(Body);
	UINT ItemNum = Read32(Body);
	UINT AvailPeriod = Read32(Body);
	UINT CostExchangeItemNum = Read32(Body);
	BYTE Flag = Read8(Body);
	BYTE ExchangeShopType = Read8(Body);

	if (!isDBtHasItem(Client, MagicExchangeShopItemId, CostExchangeItemNum))
	{
		ResponseStartGame(Client, "道具不足!");
		return;
	}

	NotifyUseItemResult2(Client, MagicExchangeShopItemId, CostExchangeItemNum);


	Write16(p, 0); // ResultID
	{			   // AwardItemInfo
		BYTE* pAwardItemInfo = p;
		Write16(pAwardItemInfo, 0); // len

		Write32(pAwardItemInfo, 0); // Exp
		Write32(pAwardItemInfo, 0); // Money
		Write32(pAwardItemInfo, 0); // SuperMoney
		Write16(pAwardItemInfo, 0); // ItemNum
		for (size_t i = 0; i < 0; i++)
		{ // ItemInfo
			BYTE* pItemInfo = pAwardItemInfo;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, 0); // ItemID
			Write32(pItemInfo, 0); // ItemNum
			Write32(pItemInfo, 0); // AvailPeriod
			Write8(pItemInfo, 0);  // Status
			Write32(pItemInfo, 0); // ObtainTime
			Write32(pItemInfo, 0); // OtherAttribute
			Write16(pItemInfo, 0); // ItemType

			len = pItemInfo - pAwardItemInfo;
			Set16(pAwardItemInfo, (WORD)len);
			pAwardItemInfo += len;
		}
		Write32(pAwardItemInfo, 0); // Coupons
		Write32(pAwardItemInfo, 0); // GuildPoint
		Write32(pAwardItemInfo, 0); // LuckMoney
		Write8(pAwardItemInfo, 0);	// ExtendInfoNum
		for (size_t i = 0; i < 0; i++)
		{ // ExtendInfoAward
			BYTE* pExtendInfoAward = pAwardItemInfo;
			Write16(pExtendInfoAward, 0); // len

			Write16(pExtendInfoAward, 0); // Key
			Write32(pExtendInfoAward, 0); // AddValue
			Write32(pExtendInfoAward, 0); // TotalValue
			Write8(pExtendInfoAward, 0);  // ShowPriority
			Write16(pExtendInfoAward, 0); // BuyNeedScore
			Write8(pExtendInfoAward, 0);  // OneMatchMaxNum

			len = pExtendInfoAward - pAwardItemInfo;
			Set16(pAwardItemInfo, (WORD)len);
			pAwardItemInfo += len;
		}
		Write32(pAwardItemInfo, 0); // SpeedCoin

		len = pAwardItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // ExchangeInfo
		BYTE* pExchangeInfo = p;
		Write16(pExchangeInfo, 0); // len

		Write32(pExchangeInfo, ItemID);				 // ItemID
		Write32(pExchangeInfo, ItemNum);			 // ItemNum
		Write32(pExchangeInfo, AvailPeriod);		 // AvailPeriod
		Write32(pExchangeInfo, CostExchangeItemNum); // CostExchangeItemNum
		Write8(pExchangeInfo, Flag);				 // Flag

		len = pExchangeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, ExchangeShopType); // ExchangeShopType
	//Write8(p, 0); // ExchangeShopType


	ItemInfo Item = { 0 };
	Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, ItemID, ItemNum, AvailPeriod, 0);
	Item.ItemID = ItemID;
	Item.AvailPeriod = AvailPeriod;
	Item.ObtainTime = 0xFFFF;
	NotifyClientAddItem(Client, 1, { Item });

	if (AvailPeriod != 0) {
		PlayerDB_InsertMagicShopInfo(Uin, ItemID);
	}

	len = p - buf.data();
	SendToClient(Client, 24787, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetMagicHouseInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	/*char* pMsg = EByteToHexUA2((const unsigned char*)Body, BodyLen);
	printf("ResponseGetMagicHouseInfo	Hex:%s\n", pMsg);*/

	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UINT MagicHouseID = Read32(Body);
	

	std::unordered_map<int, int> Items;

	PlayerDB_GetMagicFormationNum(Uin, Items);

	Write32(p, Uin);		// Uin
	Write16(p, 0);			// ResultID
	Write8(p, 1);			// OpenStat
	Write8(p, 0);			// ReceiveAllAward
	Write32(p, 1678084039); // OpenStartTime  开放开始时间
	Write32(p, 3382237639); // OpenEndTime    打开结束时间
	Write16(p, MagicFormationBoxs.size());			// AwardNums
	for (size_t i = 0; i < MagicFormationBoxs.size(); i++)
	{ // AwardItemInfo
		BYTE* pAwardItemInfo = p;
		Write16(pAwardItemInfo, 0); // len

		Write32(pAwardItemInfo, MagicFormationBoxs[i]->ItemID); // ItemID
		Write32(pAwardItemInfo, MagicFormationBoxs[i]->ItemNum);		// ItemNums
		Write32(pAwardItemInfo, MagicFormationBoxs[i]->AvailPeriod);	// AvailPeriod
		auto iter = Items.find(MagicFormationBoxs[i]->ItemID);
		if (iter != Items.end()) {
			Write16(pAwardItemInfo, 1);		// Received    被承认的
		}
		else
		{
			Write16(pAwardItemInfo, 0);		// Received    被承认的
		}
		Write16(pAwardItemInfo, i);		// AwardIndex
		Write16(pAwardItemInfo, 0);		// ItemType

		len = pAwardItemInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, MagicShop[1]);			  // CostCallOnItemNums  限时折扣张数 开启需要的道具数量
	Write32(p, MagicHouseID); // MagicHouseID
	Write32(p, 0);			  // TimeRemain 折扣多少秒后消失
	Write32(p, 0);			  // SaveItemNum //福利 总共节省召唤卡 张数
	Write8(p, 1);			  // TipsFlag 未知 不是1 会出现 福利提示
	Write32(p, MagicShop[1]);			  // CostCallOnItemNumsBeforDiscount 本次参与需要的召唤卡数量
	Write32(p, MagicShop[0]);		  // CallOnItemID

	len = p - buf.data();
	SendToClient(Client, 24470, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestPetStrengthen(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {

	//PrintfBody(Client, 20101, Body, BodyLen);
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	UINT Petid = Read32(Body);

	UINT Const = Read16(Body);//次数
	UINT Result = 10356; //失败
	UINT StrengLevel = 0;
	UINT RemainSuperMoney;
	UINT Luckvalue = 0;


	Luckvalue = PlayerDB_GetPetLuck(Uin, Client->Pet->ID);
	if (Luckvalue + Const >= 58) {
		Result = 0;
	}
	else {
		// 否则，按照一定概率决定是否成功，假设成功率为 (Luckvalue + Const) / 50.0
		float successRate = (Luckvalue + Const) / 50.0f;
		if (static_cast<float>(rand()) / RAND_MAX < successRate) {
			Result = 0;
		}
	}
	SelectClientExp(Client);
	RemainSuperMoney = Client->SuperMoney - PetSuperMoney * (Client->Pet->StrengLevel + 1) * Const;
	Client->SuperMoney = RemainSuperMoney < 0 ? 0 : RemainSuperMoney;

	UpdateClientBase(Client);
	NotifyClientUpdateMoney(Client);
	NotifyUseItemResult2(Client, PetItemID, Const * (Client->Pet->StrengLevel + 1) * PetItemNum);
	PlayerDB_UpdatePetLuck(Uin, Petid, Const);

	if (Result == 0) {
		Client->Pet->StrengLevel = (1 + Client->Pet->StrengLevel) > 10 ? 10 : 1 + Client->Pet->StrengLevel;
		PlayerDB_UpdatePetStrengLevel(Uin, Petid, Client->Pet->StrengLevel);
	}

	ResponsePetStrengthen(Client, Result);
	ResponseGetPetInfo(Client);
}



void ResponsePetStrengthen(std::shared_ptr<ClientNode> Client, UINT Result) {
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;


	Write32(p, Client->Uin);//uin
	Write32(p, Result);//ResultId
	Write32(p, Client->Pet->StrengLevel);//Level
	{//FailAward
		BYTE* pFailAward = p;
		Write16(pFailAward, 0);//len

		Write32(pFailAward, 0);//AwardMoney
		Write32(pFailAward, 0);//TotalMoney
		Write32(pFailAward, 0);//AwardSuperMoney
		Write32(pFailAward, 0);//TotalSuperMoney
		Write32(pFailAward, 0);//AwardExp
		Write32(pFailAward, 0);//TotalExp
		Write32(pFailAward, 0);//AwardItemNum
		Write32(pFailAward, 0);//AwardCoupons
		Write32(pFailAward, 0);//TotalCoupons
		Write32(pFailAward, 0);//AwardLuckMoney
		Write32(pFailAward, 0);//TotalLuckMoney
		Write32(pFailAward, 0);//AwardTeamWorkExP
		Write32(pFailAward, 0);//TotalTeamWorkExp
		Write32(pFailAward, 0);//AwardwlCash
		Write32(pFailAward, 0);//AwardwlCrystal
		Write32(pFailAward, 0);//AwardwlDegree
		Write32(pFailAward, 0);//chumcircleActivity
		Write32(pFailAward, 0);//Guildscore
		Write16(pFailAward, 0);//AppellcationID
		Write16(pFailAward, 0);//ScoreJingJi
		Write8(pFailAward, 0);//BxtendInfoNum
		Write32(pFailAward, 0);//LiveExp
		Write32(pFailAward, 0);//AwardSpeedCoin
		Write32(pFailAward, 0);//TotalSpeedCoin

		len = (WORD)(pFailAward - p);
		Set16(p, len);
		p += len;

	}
	Write8(p, 0);//Type
	WriteUn(p, Client->Pet->ID);
	Write32(p, 0);//BeginTime
	Write32(p, 0);//BeginTime
	len = (WORD)(p - buf.data());
	SendToClient(Client, 20101, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

time_t StringToTimeStamp2(const char* str) {
	struct tm tm_;
	int year, month, day, hour, minute, second;
	sscanf_s(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t timeStamp = mktime(&tm_);
	return timeStamp;
}

void NotifyClientSucessAuction(std::shared_ptr<ClientNode> Client, UCHAR CommodityId, int BidPrice, int ItemId, int ItemAvailPeriod, int ItemNumber)//拍卖成功
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	{ //UnpayCommodity
		BYTE* pUnpayCommodity = p;
		Write16(pUnpayCommodity, 0); //len

		Write32(pUnpayCommodity, 0); //AuctionId
		Write8(pUnpayCommodity, CommodityId); //CommodityId
		Write32(pUnpayCommodity, BidPrice); //BidPrice
		Write32(pUnpayCommodity, 0); //BidTime
		Write32(pUnpayCommodity, ItemId); //ItemId
		Write32(pUnpayCommodity, ItemAvailPeriod); //ItemAvailPeriod
		Write32(pUnpayCommodity, ItemNumber); //ItemNumber
		Write8(pUnpayCommodity, 0); //AuctionType

		len = pUnpayCommodity - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 11417, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void CheckAuctionTime()
{
	if (AuctionConfig->Open != 1)
	{
		return;
	}

	time_t curTm = time(0);
	time_t timeStamp;
	int BidPrice = 0;
	int BidUin = 0;

	std::vector<int> conditionValues;
	std::vector<int> resultRow;
	for (int i = 0; i < AuctionConfig->AuctionItemNum; i++) {
		timeStamp = StringToTimeStamp2(AuctionConfig->AuctionItems[i].Time.c_str());
		if (curTm >= timeStamp)
		{
			conditionValues.clear();
			resultRow.clear();
			BidPrice = 0;
			BidUin = 0;
			conditionValues.push_back(AuctionConfig->AuctionItems[i].CommodityId);
			resultRow = executeQueryRow("SELECT BidPrice,BidUin  FROM AuctionInfo  WHERE CommodityId=?;", "CheckAuctionTime", conditionValues);
			if (resultRow.size() >= 2) {
				BidPrice = resultRow[0];
				BidUin = resultRow[1];
			}

			if (BidUin > 10000)
			{
				PlayerDB_InsertAuctionUnPayInfo(BidUin, BidPrice, AuctionConfig->AuctionItems[i]);

				PlayerDB_DeleteAuctionInfo(AuctionConfig->AuctionItems[i].CommodityId);

				std::shared_ptr<ClientNode> DstClient = GetClient(BidUin);
				if (DstClient)
				{
					NotifyClientSucessAuction(DstClient, AuctionConfig->AuctionItems[i].CommodityId, BidPrice, AuctionConfig->AuctionItems[i].ItemID, AuctionConfig->AuctionItems[i].AvailPeriod, AuctionConfig->AuctionItems[i].ItemNum);
				}
				BidPrice = 0;
				BidUin = 0;
			}
		}
	}

}

void ResponseGetAuctionCfg(std::shared_ptr<ClientNode> Client, UINT Uin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, 0); //Result
	Write8(p, AuctionConfig->Open); //HasAuctionCfg
	for (size_t i = 0; i < AuctionConfig->Open; i++)
	{ //AuctionCfg
		BYTE* pAuctionCfg = p;
		Write16(pAuctionCfg, 0); //len

		Write32(pAuctionCfg, 0); //Version
		Write32(pAuctionCfg, 0); //AuctionId
		Write32(pAuctionCfg, 0); //BeginTime
		Write32(pAuctionCfg, 0); //CalcTime
		Write32(pAuctionCfg, 0); //EndTime
		Write8(pAuctionCfg, AuctionConfig->AuctionItemNum); //CommodityNumber
		for (size_t j = 0; j < AuctionConfig->AuctionItemNum; j++)
		{ //CommodityCfg
			BYTE* pCommodityCfg = pAuctionCfg;
			Write16(pCommodityCfg, 0); //len

			Write32(pCommodityCfg, AuctionConfig->AuctionItems[j].ItemID); //ItemId
			Write32(pCommodityCfg, AuctionConfig->AuctionItems[j].AvailPeriod); //ItemAvailPeriod
			Write32(pCommodityCfg, AuctionConfig->AuctionItems[j].ItemNum); //ItemNumber
			Write32(pCommodityCfg, AuctionConfig->AuctionItems[j].StartingPrice); //StartingPrice 
			Write32(pCommodityCfg, AuctionConfig->AuctionItems[j].IncreasePrice); //IncreasePrice 
			Write8(pCommodityCfg, AuctionConfig->AuctionItems[j].CommodityId); //CommodityId
			Write8(pCommodityCfg, 0); //DescLen

			len = pCommodityCfg - pAuctionCfg;
			Set16(pAuctionCfg, (WORD)len);
			pAuctionCfg += len;
		}
		Write32(pAuctionCfg, 0); //PreviewTime
		Write8(pAuctionCfg, 0); //AuctionType

		len = pAuctionCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, AuctionConfig->Open); //HasBulletin 
	for (size_t i = 0; i < AuctionConfig->Open; i++)
	{ //Bulletin
		BYTE* pBulletin = p;
		Write16(pBulletin, 0); //len

		Write32(pBulletin, 0); //Version
		Write8(pBulletin, AuctionConfig->Content.length()); //ContentLen
		memcpy(pBulletin, AuctionConfig->Content.c_str(), AuctionConfig->Content.length());
		pBulletin += AuctionConfig->Content.length();
		len = pBulletin - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //LeftTime 
	Write8(p, AuctionConfig->Open); //hasBegan 
	Write32(p, 0); //QBCouponsID
	Write8(p, 0); //LastAuctionType

	len = p - buf.data();
	SendToClient(Client, 11411, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetAuctionCfg(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT ClientAuctionCfgVersion = Read32(p);
	UINT ClientBulletinVersion = Read32(p);
	UINT ClientAuctionId = Read32(p);
	ResponseGetAuctionCfg(Client, Uin);
}

void ResponseGetParticipateAuction(std::shared_ptr<ClientNode> Client, UINT Uin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	
	int UnPayNumber = 0;
	int CurParticipateNumber = 0;
	Write32(p, Uin); //Uin
	Write16(p, 0); //Result
	BYTE* pUnPayNumber = p;
	Write8(p, 0); //UnPayNumber

	std::vector<AuctionItemInfo> auction;
	UnPayNumber = PlayerDB_SelectAuctionUnPayInfo(Uin, auction);
	for (size_t i = 0; i < auction.size(); i++)
	{
		BYTE* pUnPayCommodity = p;
		Write16(pUnPayCommodity, 0); //len

		Write32(pUnPayCommodity, 0); //AuctionId
		Write8(pUnPayCommodity, auction[i].CommodityId); //CommodityId
		Write32(pUnPayCommodity, auction[i].BidPrice); //BidPrice
		Write32(pUnPayCommodity, 0); //BidTime
		Write32(pUnPayCommodity, auction[i].ItemID); //ItemId
		Write32(pUnPayCommodity, auction[i].AvailPeriod); //ItemAvailPeriod
		Write32(pUnPayCommodity, auction[i].ItemNum); //ItemNumber
		Write8(pUnPayCommodity, 0); //AuctionType

		len = pUnPayCommodity - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Set8(pUnPayNumber, auction.size());
	BYTE* pCurParticipateNumber = p;
	Write8(p, 0); //CurParticipateNumber


	std::vector<int> conditionValues;
	conditionValues.push_back(Uin);
	std::vector<int> resultRow = executeQueryCol("select CommodityId from AuctionInfo where BidUin=?;", "ResponseGetParticipateAuction", conditionValues);
	for (int CommodityId : resultRow) {

		BYTE* pCurParticipate = p;
		Write16(pCurParticipate, 0); //len

		Write32(pCurParticipate, 0); //AuctionId
		Write8(pCurParticipate, CommodityId); //CommodityId

		len = pCurParticipate - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Set8(pCurParticipateNumber, resultRow.size());

	len = p - buf.data();
	SendToClient(Client, 11412, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetParticipateAuction(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);

	if (Client->Uin != Uin) {
		return;
	}
	ResponseGetParticipateAuction(Client, Uin);
}

void ResponsePayAuctionCommodity(std::shared_ptr<ClientNode> Client, UINT Uin, UCHAR CommodityId)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, 0); //Result
	Write32(p, 0); //AuctionId
	Write8(p, CommodityId); //CommodityId
	Write8(p, 0); //AuctionType

	len = p - buf.data();
	SendToClient(Client, 11418, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestPayAuctionCommodity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT AuctionId = Read32(p);
	UCHAR CommodityId = Read8(p);
	int BidPrice = 0;
	ItemInfo Item = { 0 };

	std::vector<int> conditionValues;
	conditionValues.push_back(CommodityId);
	conditionValues.push_back(Uin);
	std::vector<int> resultRow = executeQueryRow("SELECT BidPrice,ItemId,ItemAvailPeriod,ItemNumber FROM AuctionUnPayInfo WHERE CommodityId=? and BidUin=?;", "RequestPayAuctionCommodity", conditionValues);
	if (resultRow.size() >= 4) {
		BidPrice = resultRow[0];
		Item.ItemID = resultRow[1];
		Item.AvailPeriod = resultRow[2];
		Item.ItemNum = resultRow[3];
	}

	if (Client->SuperMoney <= BidPrice)
	{
		ResponseStartGame(Client, "点券不足!");
		return;
	}
	SelectClientExp(Client);
	Client->SuperMoney -= BidPrice;
	NotifyClientUpdateMoneyExp(Client);
	UpdateClientBase(Client);

	PlayerDB_DeleteAuctionUnPayInfo(Uin, CommodityId);
	Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);
	NotifyClientAddItem(Client, 1, { Item });
	ResponsePayAuctionCommodity(Client, Uin, CommodityId);
}


void ResponseGetCurAuctionInfo(std::shared_ptr<ClientNode> Client, UINT Uin)
{

	std::string NickName = "";
	int AuctionNumber = 0;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, 0); //Result
	Write32(p, 0); //AuctionId
	BYTE* pAuctionNumber = p;
	Write8(p, 0); //AuctionNumber
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT A.BidUin,A.BidPrice,A.BidNumber,CommodityId,B.NickName  FROM AuctionInfo A join BaseInfo B on A.BidUin=B.Uin;"));
		prep_stmt->clearParameters();

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {

			AuctionNumber++;
			NickName = UTF2GB(res->getString("NickName").c_str());

			{ //CurAuctionInfo
				BYTE* pCurAuctionInfo = p;
				Write16(pCurAuctionInfo, 0); //len

				Write32(pCurAuctionInfo, res->getUInt("BidUin")); //BidUin
				Write32(pCurAuctionInfo, res->getUInt("BidPrice")); //BidPrice
				Write32(pCurAuctionInfo, res->getUInt("BidNumber")); //BidNumber
				Write8(pCurAuctionInfo, res->getUInt("CommodityId")); //CommodityId
				Write8(pCurAuctionInfo, 17); //NameLength
				memset(pCurAuctionInfo, 0, 17);
				if (!NickName.empty())
				{
					memcpy(pCurAuctionInfo, NickName.c_str(), NickName.length());
				}
				pCurAuctionInfo += 17;
				len = pCurAuctionInfo - p;
				Set16(p, (WORD)len);
				p += len;
			}
		}
		res.reset();


		Set8(pAuctionNumber, AuctionNumber);

		len = p - buf.data();
		SendToClient(Client, 11413, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("ResponseGetCurAuctionInfo Error\n");

	}
}
void RequestGetCurAuctionInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	ResponseGetCurAuctionInfo(Client, Uin);
}

void ResponseGetAuctionList(std::shared_ptr<ClientNode> Client, UINT Uin)
{
	
	std::string NickName = "";
	int ListSize = 0;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, 0); //Result
	Write32(p, 0); //AuctionId
	BYTE* pListSize = p;
	Write16(p, 0); //ListSize
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT A.BidUin,A.BidPrice,A.CommodityId,B.NickName  FROM AuctionUnPayInfo A join BaseInfo B on A.BidUin=B.Uin;"));
		prep_stmt->clearParameters();

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			NickName = UTF2GB(res->getString("NickName").c_str());
			
			{ //HistoryList
				BYTE* pHistoryList = p;
				Write16(pHistoryList, 0); //len

				Write32(pHistoryList, res->getUInt("BidUin")); //BidUin
				Write32(pHistoryList, res->getUInt("BidPrice")); //BidPrice
				Write8(pHistoryList, res->getUInt("CommodityId")); //CommodityId
				Write8(pHistoryList, 17); //NameLength
				memset(pHistoryList, 0, 17);
				if (!NickName.empty())
				{
					memcpy(pHistoryList, NickName.c_str(), NickName.length());
				}
				pHistoryList += 17;
				Write32(pHistoryList, ListSize); //SeqId

				len = pHistoryList - p;
				Set16(p, (WORD)len);
				p += len;
			}
			ListSize++;
		}
		res.reset();

		Set16(pListSize, ListSize);
		len = p - buf.data();
		SendToClient(Client, 11414, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("ResponseGetAuctionList Error\n");


	}
}
void RequestGetAuctionList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT StartSeqId = Read32(p);
	ResponseGetAuctionList(Client, Uin);
}

void ResponseBidAuctionCommodity(std::shared_ptr<ClientNode> Client, UINT Uin, UCHAR CommodityId, UINT BidPrice, int BidNumber, short Result)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, Result); //Result
	Write32(p, 0); //AuctionId
	{ //CurAunctionInfo
		BYTE* pCurAunctionInfo = p;
		Write16(pCurAunctionInfo, 0); //len

		Write32(pCurAunctionInfo, Uin); //BidUin
		Write32(pCurAunctionInfo, BidPrice); //BidPrice
		Write32(pCurAunctionInfo, BidNumber); //BidNumber
		Write8(pCurAunctionInfo, CommodityId); //CommodityId
		Write8(pCurAunctionInfo, 17); //NameLength
		memcpy(pCurAunctionInfo, Client->NickName, 17);
		pCurAunctionInfo += 17;

		len = pCurAunctionInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 11415, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestBidAuctionCommodity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT AuctionId = Read32(p);
	UCHAR CommodityId = Read8(p);
	int BidPrice = Read32(p);

	int BidPriceSql = 0;
	int BidNumberSql = 0;
	int hasBid = 1;
	time_t curTm = time(0);
	time_t timeStamp = StringToTimeStamp2(AuctionConfig->AuctionItems[CommodityId].Time.c_str());
	if (curTm >= timeStamp)
	{
		ResponseBidAuctionCommodity(Client, Uin, CommodityId, BidPrice, 0, 1);
		ResponseStartGame(Client, "该商品拍卖时间已截止!");
		return;
	}

	char HasUnPay = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM AuctionUnPayInfo  WHERE BidUin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			HasUnPay = 1;
			break;
		}
		res.reset();

		if (HasUnPay)
		{

			ResponseBidAuctionCommodity(Client, Uin, CommodityId, BidPrice, 0, 1);
			ResponseStartGame(Client, "请先购买竞拍成功的商品后再进行出价!");
			return;
		}
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT BidPrice,BidNumber FROM AuctionInfo  WHERE CommodityId=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, CommodityId);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			if (res->rowsCount() > 0) {
				while (res->next()) {
					BidPriceSql = res->getUInt("BidPrice");
					BidNumberSql = res->getUInt("BidNumber");
					break;
				}
			}
			else
			{
				hasBid = 0;
			}
			res.reset();

			if (!hasBid)
			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO AuctionInfo(BidUin,BidPrice,BidNumber,CommodityId) VALUES(?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setUInt(2, BidPrice);
				prep_stmt->setUInt(3, 1);
				prep_stmt->setUInt(4, CommodityId);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
				ResponseBidAuctionCommodity(Client, Uin, CommodityId, BidPrice, 1, 0);

				return;
			}
		}

		if (BidPriceSql > BidPrice)
		{
			ResponseBidAuctionCommodity(Client, Uin, CommodityId, BidPrice, 0, 1);
			ResponseStartGame(Client, "竞拍失败,请刷新价格后再试!");

			return;
		}
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE AuctionInfo SET BidUin=?,BidPrice=?,BidNumber=? WHERE CommodityId=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, BidPrice);
			prep_stmt->setUInt(3, BidNumberSql + 1);
			prep_stmt->setUInt(4, CommodityId);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		ResponseBidAuctionCommodity(Client, Uin, CommodityId, BidPrice, BidNumberSql + 1, 0);

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("RequestBidAuctionCommodity Error\n");

	}
}

void ResponseGetSingleAuctionInfo(std::shared_ptr<ClientNode> Client, UINT Uin, UINT BidUin, int BidPrice, int BidNumber, UCHAR CommodityId, char* NickName)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write16(p, 0); //Result
	Write32(p, 0); //AuctionId
	Write8(p, 1); //HasAuctionInfo
	for (size_t i = 0; i < 1; i++)
	{ //CurAuctionInfo
		BYTE* pCurAuctionInfo = p;
		Write16(pCurAuctionInfo, 0); //len

		Write32(pCurAuctionInfo, BidUin); //BidUin
		Write32(pCurAuctionInfo, BidPrice); //BidPrice
		Write32(pCurAuctionInfo, BidNumber); //BidNumber
		Write8(pCurAuctionInfo, CommodityId); //CommodityId
		Write8(pCurAuctionInfo, strlen(NickName)); //NameLength
		memcpy(pCurAuctionInfo, NickName, strlen(NickName));
		pCurAuctionInfo += strlen(NickName);

		len = pCurAuctionInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 11419, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestGetSingleAuctionInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	BYTE* p = Body;
	char Temp[16] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT AuctionId = Read32(p);
	UCHAR CommodityId = Read8(p);

	UINT BidUin = 0;
	int BidPrice = 0;
	int BidNumber = 0;
	std::string NickName = "";
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT A.BidUin,A.BidPrice,A.BidNumber,B.NickName  FROM AuctionInfo A join BaseInfo B on A.BidUin=B.Uin WHERE CommodityId=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, CommodityId);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			BidUin = res->getUInt("BidUin");
			BidPrice = res->getUInt("BidPrice");
			BidNumber = res->getUInt("BidNumber");
			NickName = UTF2GB(res->getString("NickName").c_str());
			break;
		}
		res.reset();

		if (BidUin == 0)
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT A.BidUin,A.BidPrice,B.NickName  FROM AuctionUnPayInfo A join BaseInfo B on A.BidUin=B.Uin WHERE CommodityId=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, CommodityId);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				BidUin = res->getUInt("BidUin");
				BidPrice = res->getUInt("BidPrice");
				NickName = UTF2GB(res->getString("NickName").c_str());
				break;
			}
			res.reset();

			if (BidUin == 0)
			{

				memcpy(Temp, "暂无信息", 8);
				ResponseGetSingleAuctionInfo(Client, Uin, BidUin, BidPrice, BidNumber, CommodityId, Temp);
				return;
			}
			if (!NickName.empty())
			{
				ResponseGetSingleAuctionInfo(Client, Uin, BidUin, BidPrice, BidNumber, CommodityId, NickName.data());
			}

		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("RequestGetSingleAuctionInfo Error\n");

	}
}



void RequestCheckGuildName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	BYTE* p = Body;
	char NickName[17] = { 0 };
	char GuildName[17] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	memcpy(GuildName, p, 17);
	p += 17;
	if (strlen(GuildName) == 0)
	{
		ResponseStartGame(Client, "车队名称不能为空");
		return;
	}
	else if (strlen(GuildName) > 16)
	{
		ResponseStartGame(Client, "车队名称超过最大长度");
		return;
	}

	char* newGuildname = NULL;
	newGuildname = G2U(GuildName);

	UINT HasGuild = 0;

	{
		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM GuildList where Name=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, newGuildname);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				HasGuild = 1;
				break;
			}
			res.reset();

		}
		catch (const std::exception& e)
		{
			printf(e.what());
			printf("RequestCheckGuildName error\n");
		}
	}
	if (HasGuild == 0) {
		ResponseStartGame(Client, "车队名称可以使用");
	}
	else {
		ResponseStartGame(Client, "车队名称已存在");
	}

	//ResponseCheckGuildName(Client, HasGuild);

}

void ResponseCheckGuildName(std::shared_ptr<ClientNode> Client, UINT Type) {
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Client->Uin); // Uin
	Write8(p, 0);			 // GuildName[]
	Write16(p, Type);			 // ResultID
	Write16(p, 0);			 // ReasonLen

	len = p - buf.data();
	SendToClient(Client, 15017, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestUpdateGuildName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen) {
	BYTE* p = Body;
	char NickName[17] = { 0 };
	char GuildName[17] = { 0 };
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	memcpy(GuildName, p, 17);
	p += 17;
	if (strlen(GuildName) == 0)
	{
		ResponseStartGame(Client, "车队名称不能为空");
		return;
	}
	else if (strlen(GuildName) > 16)
	{
		ResponseStartGame(Client, "车队名称超过最大长度");
		return;
	}

	char* newGuildname = NULL;
	newGuildname = G2U(GuildName);

	UINT HasGuild = 0;
	{
		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ID FROM GuildList where Name=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, newGuildname);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				HasGuild = 1;
				break;
			}
			res.reset();

			if (HasGuild == 0) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("update guildlist set name = ? where leaderUin = ?; "));
				prep_stmt->clearParameters();
				prep_stmt->setString(1, newGuildname);
				prep_stmt->setUInt(2, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
				ResponseUpdateGuildName(Client, GuildName);
			}
			else {
				ResponseStartGame(Client, "车队名称已存在");
			}


		}
		catch (const std::exception& e)
		{
			printf(e.what());
			printf("RequestUpdateGuildName error\n");
		}
	}
}

void ResponseUpdateGuildName(std::shared_ptr<ClientNode> Client, char* newGuildname) {
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 0);
	Write8(p, 17);
	memcpy(p, newGuildname, 17);
	p += 17;
	Write16(p, 0);

	len = p - buf.data();
	SendToClient(Client, 288, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



void NotifySecondPassInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write8(p, Client->SecPwdStats); //Status
	Write8(p, Client->Verify); //Verify
	Write32(p, Client->FrozenLeftSeconds); //FrozenLeftSeconds
	Write32(p, Client->FrozenSeconds); //FrozenSeconds

	len = p - buf.data();
	SendToClient(Client, 21015, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestOpSecondPass(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	char Op = Read8(p);//0设置二级密码 4验证二级密码 3 关闭二级密码 6 修改密码 2 忘记密码

	std::string CurPass = ReadString(p, MaxSecPwd);
	std::string NewPass = ReadString(p, MaxSecPwd);

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		if (Op == 0) {
			ResponseOpSecondPass(Client, Op, 1);

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET SecPwd=?,SecPwdStatus=1 WHERE Uin=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, NewPass);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
			memcpy(Client->SecPwd, NewPass.c_str(), MaxSecPwd);//更新内存
		}
		if (Op == 2) {
			ResponseStartGame(Client, "忘记二级密码联系管理员");
		}
		else if (Op == 3) {
			if (strcmp(CurPass.c_str(), Client->SecPwd) == 0) {
				Client->SecPwdStats = 0;
				ResponseOpSecondPass(Client, Op, 1);
				NotifySecondPassInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET SecPwdStatus=0,Verify=0 WHERE Uin=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else {
				ResponseStartGame(Client, "二级密码错误");
			}
		}
		else if (Op == 4) {
			if (strcmp(CurPass.c_str(), Client->SecPwd) == 0) {
				Client->Verify = 1;
				ResponseOpSecondPass(Client, Op, 1);
				NotifySecondPassInfo(Client);
				/*std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET Verify=1 WHERE Uin=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();*/
			}
			else {
				ResponseStartGame(Client, "二级密码错误");
			}
		}

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("NotifySecondPassInfo Error\n");
	}
}


void ResponseOpSecondPass(std::shared_ptr<ClientNode> Client, char Op, UINT Status)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write8(p, Op); //Op
	Write8(p, Status); //Status
	Write16(p, 0); //ResultID

	len = p - buf.data();
	SendToClient(Client, 21017, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void NotifyClientUpdateNobleInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, Client->Uin); // Uin
	{						 // NobleInfo
		BYTE* pNobleInfo = p;
		Write16(pNobleInfo, 0); // len

		Write32(pNobleInfo, Client->NobleID);	// NobleID
		Write8(pNobleInfo, Client->NobleLevel);		// NobleLevel
		Write32(pNobleInfo, Client->NoblePoint);	// NoblePoint
		Write32(pNobleInfo, -1); // NobleLeftDays

		len = pNobleInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 961, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyClientUpdateEmperorInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, Client->Uin); // Uin
	Write8(p, 0);			 // OpenType
	{						 // EmperorInfo
		BYTE* pEmperorInfo = p;
		Write16(pEmperorInfo, 0); // len

		Write8(pEmperorInfo, Client->EmperorLevel);		// EmperorLevel
		Write32(pEmperorInfo, Client->EmperorPoint);	// EmperorPoint
		Write32(pEmperorInfo, -1); // EmperorLeftDays
		Write8(pEmperorInfo, 10);	// EmperorGrowRate

		len = pEmperorInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // EmperorOtherInfo
		BYTE* pEmperorOtherInfo = p;
		Write16(pEmperorOtherInfo, 0); // len

		Write32(pEmperorOtherInfo, 0); // ExpiredTime
		Write8(pEmperorOtherInfo, 0);  // ShowExpireTips

		len = pEmperorOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 978, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyClientUpdateLoverVipInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, Client->Uin); // Uin
	{						 // LoverVipBaseInfo
		BYTE* pLoverVipBaseInfo = p;
		Write16(pLoverVipBaseInfo, 0); // len

		Write8(pLoverVipBaseInfo, Client->LoverVipLevel);  // LoverVipLevel
		Write32(pLoverVipBaseInfo, Client->LoverVipPoint); // LoverVipPoint
		Write8(pLoverVipBaseInfo, 10);			   // GrowRate

		len = pLoverVipBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // LoverVipOtherInfo
		BYTE* pLoverVipOtherInfo = p;
		Write16(pLoverVipOtherInfo, 0); // len

		Write32(pLoverVipOtherInfo, -1); // LoverVipLeftDays
		Write8(pLoverVipOtherInfo, 0);				   // CanReceiveGift
		Write8(pLoverVipOtherInfo, 0);				   // ShowExpireTips

		len = pLoverVipOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 13800, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientUpdateGuildVipInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, Client->Uin); // Uin
	{						 // BaseInfo
		BYTE* pBaseInfo = p;
		Write16(pBaseInfo, 0); // len

		Write8(pBaseInfo, Client->GuildVipLevel);  // GuildVipLevel
		Write32(pBaseInfo, Client->GuildVipPoint); // GuildVipPoint

		len = pBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ // OtherInfo
		BYTE* pOtherInfo = p;
		Write16(pOtherInfo, 0); // len

		Write32(pOtherInfo, -1); // GuildVipLeftDays
		Write8(pOtherInfo, 0);				   // CanReceiveGift

		len = pOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 12031, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientUpdatePersonalGardenBaseInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, Client->Uin); // Uin
	{			   // BaseInfo
		BYTE* pBaseInfo = p;
		Write16(pBaseInfo, 0); // len

		Write8(pBaseInfo, Client->GardenLevel);		  // GardenLevel
		Write32(pBaseInfo, Client->GardenPoint);	  // GardenPoint
		Write8(pBaseInfo, 10);				  // GrowRate
		Write32(pBaseInfo, -1); // GardenExpireTime
		{									  // SimpleInfo
			BYTE* pSimpleInfo = pBaseInfo;
			Write16(pSimpleInfo, 0); // len
			Write32(pSimpleInfo, 0); // WeekPopularity
			Write32(pSimpleInfo, 0); // TotalPopularity
			Write32(pSimpleInfo, 0); // LastUpdatePopularityTime
			Write8(pSimpleInfo, 0);	 // PrivateType
			len = pSimpleInfo - pBaseInfo;
			Set16(pBaseInfo, (WORD)len);
			pBaseInfo += len;
		}

		len = pBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 20301, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void RequestUseItem(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int ItemID = Read32(p);
	//UINT DstUin = Read32(p);
	//bool AttachDataLen = Read8(p);
	//UCHAR OpType = Read8(p);
	//UINT Para = Read32(p);
	//UCHAR MultiUseFlag = Read8(p);
	//int MultiUseNum = Read32(p);

	if (!isDBtHasItem(Client, ItemID, 1))
	{
		ResponseStartGame(Client, "道具不足!");
		return;

	}
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		if (ItemID == 63555)
		{ // 钻皇
			if (Client->EmperorLevel >= 1)
			{
				//成长
				Client->EmperorPoint += 100;
				NotifyClientUpdateEmperorInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET EmperorPoint=EmperorPoint+100 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
				ResponseStartGame(Client, "/C03钻皇成长值增加100点!");
			}
			else
			{
				//开通
				Client->EmperorLevel++;
				Client->EmperorPoint = 10;
				NotifyClientUpdateEmperorInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET EmperorLevel=1,EmperorPoint=10 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03开通钻皇成功!");
			}
		}
		else if (ItemID == 30450)
		{ // 车队贵族
			if (Client->GuildVipLevel >= 1)
			{
				//成长
				Client->GuildVipPoint += 100;
				NotifyClientUpdateGuildVipInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET GuildVipPoint=GuildVipPoint+100 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03车队贵族成长值增加100点!");

			}
			else
			{
				//开通
				Client->GuildVipLevel++;
				Client->GuildVipPoint = 10;
				NotifyClientUpdateGuildVipInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET GuildVipLevel=1,GuildVipPoint=10 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03开通车队贵族成功!");
			}
		}
		else if (ItemID == 27137)
		{ // 皇权贵族
			if (Client->NobleLevel >= 1)
			{
				//成长
				Client->NoblePoint += 100;
				if (Client->NobleLevel == 1 && Client->NoblePoint >= 300)
				{
					Client->NobleLevel++;
				}
				else if (Client->NobleLevel == 2 && Client->NoblePoint >= 900)
				{
					Client->NobleLevel++;
				}
				else if (Client->NobleLevel == 3 && Client->NoblePoint >= 2000)
				{
					Client->NobleLevel++;
				}
				else if (Client->NobleLevel == 4 && Client->NoblePoint >= 4000)
				{
					Client->NobleLevel++;
				}
				else if (Client->NobleLevel == 5 && Client->NoblePoint >= 8000)
				{
					Client->NobleLevel++;
				}
				else if (Client->NobleLevel == 6 && Client->NoblePoint >= 30000)
				{
					Client->NobleLevel++;
				}
				NotifyClientUpdateNobleInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET NobleLevel=?,NoblePoint=NoblePoint+100 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->NobleLevel);
				prep_stmt->setUInt(2, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03皇族成长值增加100点!");
			}
			else
			{
				//开通
				Client->NobleLevel++;
				Client->NoblePoint = 10;
				NotifyClientUpdateNobleInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET NobleLevel=1,NoblePoint=10 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();


				ResponseStartGame(Client, "/C03开通皇族成功!");
			}
		}
		else if (ItemID == 31964)
		{ // 情侣贵族
			if (Client->LoverVipLevel >= 1)
			{
				//成长
				Client->LoverVipPoint += 100;
				NotifyClientUpdateLoverVipInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET LoverVipPoint=LoverVipPoint+100 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03情侣贵族成长值增加100点!");
			}
			else
			{
				//开通
				Client->LoverVipLevel++;
				Client->LoverVipPoint = 10;
				NotifyClientUpdateLoverVipInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET LoverVipLevel=1,LoverVipPoint=10 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03开通情侣贵族成功!");
			}
		}
		else if (ItemID == 63505)
		{
			if (Client->GardenLevel >= 1)
			{
				Client->GardenPoint += 100;
				if (Client->GardenLevel == 1 && Client->GardenPoint >= 500)
				{
					Client->GardenLevel++;
				}
				else if (Client->GardenLevel == 2 && Client->GardenPoint >= 1800)
				{
					Client->GardenLevel++;
				}
				else if (Client->GardenLevel == 3 && Client->GardenPoint >= 4500)
				{
					Client->GardenLevel++;
				}
				else if (Client->GardenLevel == 4 && Client->GardenPoint >= 8000)
				{
					Client->GardenLevel++;
				}
				else if (Client->GardenLevel == 5 && Client->GardenPoint >= 14000)
				{
					Client->GardenLevel++;
				}
				NotifyClientUpdatePersonalGardenBaseInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET GardenLevel=?,GardenPoint=GardenPoint+100 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->GardenLevel);
				prep_stmt->setUInt(2, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03后花园成长值增加100点!");
			}
			else
			{
				Client->GardenLevel++;
				Client->GardenPoint = 10;
				NotifyClientUpdatePersonalGardenBaseInfo(Client);

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET GardenLevel=1,GardenPoint=10 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

				ResponseStartGame(Client, "/C03开通后花园成功!");
			}
		}
		NotifyUseItemResult2(Client, ItemID, 1);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "RequestUseItem Error" << std::endl;
		//throw;关闭
	}

}



void NotifyGameBegin22(std::shared_ptr<ClientNode> Client)
{


	BYTE buf[8192] = { 0 };
	BYTE* p = buf;
	size_t len = 0;
	std::shared_ptr<ClientNode> Players[6] = { 0 };

	char CurrentPlayers = 0;

	Players[0] = Client;
	CurrentPlayers = 1;

	Write32(p, 0);		   // NormalLoadTime
	Write32(p, Client->MapID); // MapID
	// MapHash[]
	memset(p, 0, 32);
	p += 32;

	Write32(p, Client->GameID);		  // GameID
	Write32(p, (DWORD)time(nullptr)); // Seed

	Write8(p, CurrentPlayers);
	// m_aunRaceTrackOrders[MAX_MUCH_SEATNUMINROOM]
	int* aRaceTrackOrders = (int*)p;
	for (char i = 0; i < MAX_MUCH_SEATNUMINROOM; i++)
	{
		Write32(p, 0);
	}
	// 起跑线并排的玩家
	int index[] = { 2, 3, 1, 4, 0, 5 };
	Set32((BYTE*)&aRaceTrackOrders[2], Client->Uin);
	//Set32((BYTE*)&aRaceTrackOrders[index[CurrentPlayers]], 1);
	Write16(p, Client->TotalRound);								   // TotalMapRound
	Write32(p, 0);												   // PropUpdateInterval
	Write32(p, PlayerDB_GetMapRecord(Client->Uin, Client->MapID, Client)); // Record

	Write8(p, 0); // NianShouTypeNum


	Write8(p, CurrentPlayers); // SyncCarNum

	for (char i = 0; i < CurrentPlayers; i++)
	{ // SyncCar
		BYTE* pSyncCar = p;
		Write16(pSyncCar, 0); // len
		// 新增判断皮肤
		Write32(pSyncCar, Players[i]->Uin); // PlayerUin
		if (Players[i] == Client) {
			Write32(pSyncCar, Players[i]->KartID);
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

		Write8(pSyncCar, 0); // HasStoneInfo

		len = pSyncCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); // ReportDataFlag
	Write8(p, 0); // CheckDataNum

	Write8(p, 1); // P2PMode
	Write8(p, 0xa); // TcpFrequence

	Write8(p, 50); // MultiInfoLen
	memcpy(p, "\x00\x00\x00\x00\x64\x72\x23\xfd\x65\x02\x30\x02\x2e\x01\x01\x64\x72\x23\xfd\x12\x8c\x17\x3e\x01\x01\x70\x4e\x24\xae\x45\xd2\xb2\xfa\x36\x7b\x4b\xe7\xe1\x03\x1e\xa8\x8e\xa1\xa6\x39\xfe\x08\xb3\x92\xe3", 50);
	p += 50;


	Write8(p, 0); // FeedBackEnabled

	Write8(p, 0); // SpeedSectionNum


	Write8(p, 0); // NormalSpeedSectionNum


	Write8(p, 0); // MemCheckInfoNum

	Write8(p, 4);	 // ExtraInfoInterval
	Write16(p, -1);	 // OffsetThreshold
	Write32(p, 200); // SpeedRatioThreshold1
	Write32(p, 200); // SpeedRatioThreshold2
	Write32(p, 0);	 // HideTaskId
	Write8(p, 0);	 // HideTaskType
	Write32(p, 0);	 // HideTaskParam1

	Write8(p, 0); // ForceReportCPNum
	// m_au16ForceReportCP[]

	Write8(p, 0); // CliReserveFlag


	Write8(p, 0);  // EnableAntiDriftCheat



	Write8(p, 0); // HasCrashModePara

	Write32(p, 0); // FizzStarTaskId
	Write8(p, 0);  // FizzStarTaskType
	Write32(p, 0); // FizzStarTaskParam1
	Write32(p, 0); // LDMRecordID
	Write32(p, 0x0014af5d); // GameSeq

	Write8(p, CurrentPlayers); // PlayerNums
	for (size_t i = 0; i < CurrentPlayers; i++)
	{ // GameBeginPlayerInfo
		BYTE* pGameBeginPlayerInfo = p;
		Write16(pGameBeginPlayerInfo, 0); // len

		Write32(pGameBeginPlayerInfo, Players[i]->Uin); // Uin
		Write32(pGameBeginPlayerInfo, 0);// ChumCircleID
		Write8(pGameBeginPlayerInfo, 0); // SkillNums
		Write32(pGameBeginPlayerInfo, 0); // WorldEscapeTaskID
		{ // ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pGameBeginPlayerInfo;
			Write16(pProfessionLicenseInfo, 0); // len

			Write8(pProfessionLicenseInfo, 0); // BigLevel
			Write8(pProfessionLicenseInfo, 1); // SmallLevel

			len = pProfessionLicenseInfo - pGameBeginPlayerInfo;
			Set16(pGameBeginPlayerInfo, (WORD)len);
			pGameBeginPlayerInfo += len;
		}
		Write8(pGameBeginPlayerInfo, 0); // ParaNum

		Write32(pGameBeginPlayerInfo, 0); // StarsFightingStar
		Write8(pGameBeginPlayerInfo, 0);  // ParaNum
		Write32(pGameBeginPlayerInfo, 1); // ParaNum[]

		len = pGameBeginPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	memcpy(p, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x0a\xaa\x1d\xb2\x76\x6c\x86\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 138);
	p += 138;

	Write8(p, 1); // HasTimerChallenge2ndGameBeginInfo
	for (size_t i = 0; i < 1; i++)
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
				Write32(pPlayerJumpInfo, Client->JumpLevel); //Jumplevel
				Write32(pPlayerJumpInfo, 0); //WeekHighestTongGuanNum 最高记录
				Write32(pPlayerJumpInfo, 0); //BonusJumplevel
				Write32(pPlayerJumpInfo, 0); //HistoryMaxTongGuanNum 

				len = pPlayerJumpInfo - pJumpInfo;
				Set16(pJumpInfo, (WORD)len);
				pJumpInfo += len;
			}
			Write32(pJumpInfo, Client->JumpLevel); //JumpLevel 跳关

			len = pJumpInfo - pTimerChallenge2ndGameBeginInfo;
			Set16(pTimerChallenge2ndGameBeginInfo, (WORD)len);
			pTimerChallenge2ndGameBeginInfo += len;
		}
		Write32(pTimerChallenge2ndGameBeginInfo, Client->Uin); //BeginUin
		Write8(pTimerChallenge2ndGameBeginInfo, 0); //Reverse


		{
			Write8(pTimerChallenge2ndGameBeginInfo, 1);
			Write32(pTimerChallenge2ndGameBeginInfo, 1);
			Write8(pTimerChallenge2ndGameBeginInfo, 0); //Num
			for (size_t i = 0; i < 0; i++)
			{
				BYTE* pPlayerJumpInfo = pTimerChallenge2ndGameBeginInfo;
				Write16(pPlayerJumpInfo, 0); //len

				Write32(pPlayerJumpInfo, 0);
				Write8(pPlayerJumpInfo, 0);

				len = pPlayerJumpInfo - pTimerChallenge2ndGameBeginInfo;
				Set16(pTimerChallenge2ndGameBeginInfo, (WORD)len);
				pTimerChallenge2ndGameBeginInfo += len;
			}
			Write16(pTimerChallenge2ndGameBeginInfo, 0);
			{
				BYTE* pPlayerJumpInfo = pTimerChallenge2ndGameBeginInfo;
				Write16(pPlayerJumpInfo, 0); //len

				/*memcpy(pPlayerJumpInfo, "\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0B\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\x4C\x76\x34\x00\x16\xD7\xD4\xB6\xAF\xBC\xDD\xCA\xBB\xB5\xC4\xCA\xB9\xD3\xC3\xB4\xCE\xCA\xFD\x2B\x31\xA1\xA3\x01\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x1B\x58\x00\x00\x27\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x07", 91);
				pPlayerJumpInfo += 91;*/

				memcpy(pPlayerJumpInfo, "\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00", 43);
				pPlayerJumpInfo += 43;

				Write32(pPlayerJumpInfo, 9);//飞机数量
				Write32(pPlayerJumpInfo, 0);
				Write32(pPlayerJumpInfo, 0);
				Write32(pPlayerJumpInfo, 0);

				len = pPlayerJumpInfo - pTimerChallenge2ndGameBeginInfo;
				Set16(pTimerChallenge2ndGameBeginInfo, (WORD)len);
				pTimerChallenge2ndGameBeginInfo += len;
			}
		}
		len = pTimerChallenge2ndGameBeginInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); // HasGameStageInfo

	Write8(p, 0); // CarCollectInfoNum

	Write16(p, 0x0a00); // ReportAntiCollisionDataTime
	Write32(p, 0);	   // Duration
	Write32(p, 0);	   // BeginCDTime
	Write32(p, 0);	   // PropInteval
	Write32(p, 0);	   // MoveFreq

	Write8(p, 0); // HaveGameLogicTask
	Write16(p, 0); // RankedMatchBegainTipInfoType
	Write8(p, 0); // BegainTipLen

	Write8(p, 0); // BuffBum
	Write8(p, 0); // MapChallengeInfoNum
	Write8(p, 0); // IsChangeCar
	Write8(p, 0); // AnonymousMode


	Write8(p, 0); // IsTimeShuttleGen
	Write8(p, 0); // HalloweenDdventureInfoNum
	Write8(p, 0); // MaxPropEffectNum
	Write8(p, 0); // IsCrossNoCollision

	Write8(p, 3); // RecordCheckCondNum
	{ //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, 0); //CheckRelation
		Write8(pRecordCheckConds, 2); //ConditionNum
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
	{ //RecordCheckConds
		BYTE* pRecordCheckConds = p;
		Write16(pRecordCheckConds, 0); //len

		Write16(pRecordCheckConds, 1); //CheckRelation
		Write8(pRecordCheckConds, 2); //ConditionNum
		{ //CheckConditions
			BYTE* pCheckConditions = pRecordCheckConds;
			Write16(pCheckConditions, 0); //len

			Write16(pCheckConditions, 0); //CheckType
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
	{ //RecordCheckConds
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

	Write32(p, 0x000007e0); // GameBeginSwitchFlag
	Write8(p, 0);  // TriggerVeggieDogTask
	Write8(p, 0); // HasQSpeedCrystalInfo
	Write32(p, 0);


	len = p - buf;
	SendToClient(Client, 509, buf, len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


extern DWORD WINAPI OnSingleJump(PVOID Client);
void ResponseStartTimerChallenge2(std::shared_ptr<ClientNode> Client)
{
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		ResponseLeaveRoom(Client, 0);
		Sleep(1000);
		//ResponseStartGame(Client, "房间状态异常");
		return;
	}

	Client->TaskID = 0xFFFF;
	Client->JumpLevel = 0;
	Client->MapID = Room->MapID;
	Client->MAXLapTime = 0;
	Client->EventTime = 0;
	if (Client->KGMode != 1)
	{
		Client->KGMode = 0;
	}
	Client->GameID = 2;
	Client->TotalRound = 1;
	Client->Loop = false;
	Client->FinishTime = 0;
	Client->Round = 0;
	Client->PassedCheckPoint = 0;
	Client->MapCompatibilityMode = true;
	Client->EndCheckPoint = 0xFFFF;//结束点暂未定义 可以根据24256消息
	BYTE* p = StartTimerChallenge2;
	p += 57;
	Write32(p, Client->Uin);
	p += 82;
	Write32(p, Client->Uin);
	Write32(p, Client->KartID);
	p += 104;
	Write32(p, Client->Uin);
	p += 196;
	Write32(p, Client->Uin);
	p += 20;
	Write32(p, Client->Uin);
	ResponseTimerChallenge2ndMoreHallInfo(Client);
	//ResponseAddToMatchQueue(Client, 0x8);
	//NotifyMatchResult(Client, 0x8);
	//NotifyTimerChallenge2ndCommonInfo
	const char* buf1 = "\x01\xc1\x00\x00\x00\x02\x00\xd4\x13\x00\x63\x00\x01\x00\x02\x00\x03\x00\x05\x00\x06\x00\x07\x00\x09\x00\x0a\x00\x0b\x00\x0d\x00\x0e\x00\x0f\x00\x11\x00\x12\x00\x13\x00\x16\x00\x17\x00\x18\x00\x19\x00\x1a\x00\x1b\x00\x1d\x00\x1e\x00\x1f\x00\x21\x00\x22\x00\x23\x00\x25\x00\x26\x00\x27\x00\x2a\x00\x2b\x00\x2c\x00\x2e\x00\x2f\x00\x30\x00\x32\x00\x33\x00\x34\x00\x37\x00\x38\x00\x39\x00\x3a\x00\x3b\x00\x3c\x00\x3e\x00\x3f\x00\x40\x00\x43\x00\x44\x00\x45\x00\x46\x00\x47\x00\x48\x00\x49\x00\x4a\x00\x4b\x00\x4c\x00\x4d\x00\x4e\x00\x4f\x00\x50\x00\x51\x00\x5e\x00\x5f\x00\x60\x00\x61\x00\x62\x00\x63\x00\x64\x00\x65\x00\x66\x00\x67\x00\x68\x00\x69\x00\x6a\x00\x6b\x00\x6c\x00\x6d\x00\x6e\x00\x6f\x00\x70\x00\x71\x00\x72\x00\x73\x00\x74\x00\x75\x00\x82\x00\x83\x00\x84\x00\x85\x00\x86\x00\x87\x00\x8e\x00\x8f\x00\x90\x00\x91\x00\x92\x00\x93\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6e\x11\x00\x30\x00\x04\x00\x08\x00\x0c\x00\x10\x00\x14\x00\x15\x00\x1c\x00\x20\x00\x24\x00\x28\x00\x29\x00\x2d\x00\x31\x00\x35\x00\x36\x00\x3d\x00\x41\x00\x42\x00\x52\x00\x53\x00\x54\x00\x55\x00\x56\x00\x57\x00\x58\x00\x59\x00\x5a\x00\x5b\x00\x5c\x00\x5d\x00\x76\x00\x77\x00\x78\x00\x79\x00\x7a\x00\x7b\x00\x7c\x00\x7d\x00\x7e\x00\x7f\x00\x80\x00\x81\x00\x88\x00\x89\x00\x8a\x00\x8b\x00\x8c\x00\x8d\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6d\x01\x00\x6a\x00\x00\x00\x00\x00\x00\x51\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x10\x02\x4d\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\x02\x4d\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\x02\x4e\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1b\x58\x00\x00\x00\x01\x50\x0f\x01\x00\x00\x4e\x20\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x23\x28\x01\x00\x53\x00\x00\x27\x11\x01\x00\x4c\x00\x12\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x05\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x09\x00\x00\x00\x0a\x00\x00\x00\x0b\x00\x00\x00\x0d\x00\x00\x00\x0e\x00\x00\x00\x0f\x00\x00\x00\x12\x00\x00\x00\x13\x00\x00\x00\x11\x00\x00\x00\x16\x00\x00\x00\x17\x00\x00\x00\x18\x01\x00\x01\x00\x00";

	//技能显示在notifygamebegin
	//SendToClient(Client, 509, StartTimerChallenge2, 816, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	NotifyGameBegin22(Client);
	if (mode28346 == 2) {
		NotifyKartPhysParam(Client, Room->Player);
	}
	else {
		NotifyEncryptedMsg(Client);
		//NotifyKartPhysParam(RoomClient, Room->Player);
	}
	SendToClient(Client, 24256, (BYTE*)buf1, 565, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	//NotifyOtherKartStoneInfo(Client);

	NotifyRaceBegin(Client);
	if (Client->JumpThread == 0 && false)
	{
		auto ClientPtr = new std::shared_ptr<ClientNode>(Client);
		HANDLE jump = CreateThread(NULL, 0, OnSingleJump, ClientPtr, 0, NULL);
		if (jump)
		{
			Client->JumpThread = 1;
			CloseHandle(jump);
		}
	}

}

extern DWORD WINAPI OnSingleOver(PVOID Client);
void RequestReportTimerChallenge2ndEvent(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT EventID = Read32(p);
	//printf("Challenge2ndEvent:%d \n", EventID);
	if (EventID == 19)//退出挑战
	{
		if (Client->MAXLapTime == 0)
		{
			auto ClientPtr = new std::shared_ptr<ClientNode>(Client);
			HANDLE over = CreateThread(NULL, 0, OnSingleOver, ClientPtr, 0, NULL);
			if (over)
			{
				CloseHandle(over);
			}
		}
		Client->MAXLapTime = 1;
	}
	if (EventID == 17)//使用飞行技能
	{
		UINT date = std::clock();
		if (Client->EventTime != 0 && (Client->EventTime + 5000) > date)
		{
			const char* buf = "\x00\x00\x27\x18\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
			SendToClient(Client, 24254, (BYTE*)buf, 20, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
			Client->EventTime = 0;
			return;
		}

		const char* buf = "\x00\x00\x27\x18\x00\x00\x00\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
		SendToClient(Client, 24254, (BYTE*)buf, 20, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		Client->EventTime = date;
	}
	if (EventID == 0xF)
	{
		const char* buf = "\x00\x00\x27\x18\x00\x00\x00\x0F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
		SendToClient(Client, 24254, (BYTE*)buf, 20, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}


}

void RequestC2GLoadFile(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR Cmd = Read8(p);
	USHORT UuidLen = Read16(p);
	std::string FileName = ReadString(p, UuidLen);
	ResponseC2GLoadFile(Client, Cmd, UuidLen, FileName);

}
void ResponseC2GLoadFile(std::shared_ptr<ClientNode> Client, UCHAR Cmd, UCHAR UuidLen, std::string FileName)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write8(p, Cmd); //Cmd
	//Write16(p, UuidLen); //UrlLen
	std::string Url = ChallengeUrl + FileName;
	WriteString16(p, Url.c_str()); //太空边境


	len = p - buf;
	SendToClient(Client, 11229, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestReportAntiCollisionDataBuff(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	USHORT BuffeLen = Read16(p);
}

void ResponseStartTimerChallenge(std::shared_ptr<ClientNode> Client)
{
	Client->TaskID = 0xFFFE;
	Client->GameID = 2;
	Client->TotalRound = 1;
	Client->Loop = false;
	Client->FinishTime = 0;
	Client->Round = 0;
	Client->PassedCheckPoint = 0;
	Client->EndCheckPoint = 225;
	BYTE* p = StartTimerChallenge;
	p += 57;
	Write32(p, Client->Uin);
	p += 82;
	Write32(p, Client->Uin);
	Write32(p, Client->KartID);
	p += 104;
	Write32(p, Client->Uin);

	SendToClient(Client, 509, StartTimerChallenge, 589, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	NotifyEncryptedMsg(Client);
	//NotifyOtherKartStoneInfo(Client);

	//NotifyTimerChallengeCommonInfo
	const char* buf1 = "\x01\xe5\x00\x00\x00\x03\x00\x0a\x00\x00\x02\x4a\x00\x00\x00\x63\x00\x0a\x00\x00\x02\x4c\x00\x00\x00\x00\x00\x0a\x00\x00\x02\x4e\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x00\x03\x00\x26\x05\x00\x0c\x00\x0c\x00\x15\x00\x17\x00\x1b\x00\x31\x00\x32\x00\x33\x00\x3a\x00\x3b\x00\x4b\x00\x4c\x00\x4d\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x92\x0c\x00\x42\x00\x01\x00\x02\x00\x03\x00\x04\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0e\x00\x0f\x00\x11\x00\x12\x00\x1d\x00\x1e\x00\x1f\x00\x20\x00\x25\x00\x26\x00\x27\x00\x29\x00\x2a\x00\x2b\x00\x2c\x00\x2d\x00\x2e\x00\x30\x00\x34\x00\x35\x00\x36\x00\x37\x00\x38\x00\x39\x00\x3c\x00\x3d\x00\x3e\x00\x40\x00\x41\x00\x42\x00\x43\x00\x45\x00\x47\x00\x48\x00\x49\x00\x4a\x00\x4e\x00\x4f\x00\x50\x00\x51\x00\x52\x00\x53\x00\x54\x00\x55\x00\x56\x00\x59\x00\x5a\x00\x5b\x00\x5c\x00\x5d\x00\x5e\x00\x5f\x00\x60\x00\x61\x00\x62\x00\x63\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x88\x0d\x00\x3d\x00\x05\x00\x06\x00\x07\x00\x08\x00\x0a\x00\x0b\x00\x0c\x00\x0d\x00\x0e\x00\x0f\x00\x10\x00\x11\x00\x13\x00\x14\x00\x15\x00\x18\x00\x1a\x00\x1b\x00\x1d\x00\x1f\x00\x20\x00\x21\x00\x22\x00\x23\x00\x24\x00\x25\x00\x26\x00\x27\x00\x28\x00\x2a\x00\x2b\x00\x2c\x00\x2e\x00\x30\x00\x31\x00\x34\x00\x35\x00\x38\x00\x39\x00\x3e\x00\x40\x00\x42\x00\x49\x00\x4a\x00\x53\x00\x56\x00\x59\x00\x5d\x00\x5f\x00\x67\x00\x68\x00\x6a\x00\x73\x00\x7a\x00\x7c\x00\x83\x00\x8c\x00\x92\x00\x93\x00\x9c\x00\xaf\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x6d\x01\x00\x6a\x01\x00\x00\x00\x00\x00\x51\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x10\x02\x4e\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x10\x02\x4c\x00\x00\x00\x01\x00\x00\x00\x00\x01\x00\x00\x00\x00\x10\x02\x4a\x00\x00\x00\x63\x00\x00\x00\x63\x01\x00\x00\x63\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x03\x00\x00\x00\x03\x00\x00\x00\x00\x6c\x6f";

	SendToClient(Client, 24239, (BYTE*)buf1, 487, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	//NotifyTimerChallengeEvent 08

	const char* buf2 = "\x00\x00\x27\x11\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x10\x00\x0F\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	SendToClient(Client, 24238, (BYTE*)buf2, 42, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);

	NotifyRaceBegin(Client);

	const char* buf3 = "\x00\x00\x27\x11\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x0F\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	//NotifyTimerChallengeEvent 07
	SendToClient(Client, 24238, (BYTE*)buf3, 42, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	//通关全部
	if (Client->JumpThread == 0)
	{
		auto ClientPtr = new std::shared_ptr<ClientNode>(Client);
		HANDLE jump = CreateThread(NULL, 0, OnSingleJump, ClientPtr, 0, NULL);
		if (jump)
		{
			Client->JumpThread = 1;
			CloseHandle(jump);
		}
	}
}

void RequestGetBattleModeCarSkillInfoList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	int CarID = Read32(p);
	UINT Time = Read32(p);
	ResponseGetBattleModeCarSkillInfoList(Client);
}

void ResponseGetBattleModeCarSkillInfoList(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write32(p, Client->KartID); //CarID
	{ //BattleModeSkillInfoList
		BYTE* pBattleModeSkillInfoList = p;
		Write16(pBattleModeSkillInfoList, 0); //len

		Write8(pBattleModeSkillInfoList, 1); //SkillNum
		for (size_t i = 0; i < 1; i++)
		{ //SkillInfo
			BYTE* pSkillInfo = pBattleModeSkillInfoList;
			Write16(pSkillInfo, 0); //len

			Write16(pSkillInfo, 1); //SkillId
			Write8(pSkillInfo, 1); //SkillLevel

			len = pSkillInfo - pBattleModeSkillInfoList;
			Set16(pBattleModeSkillInfoList, (WORD)len);
			pBattleModeSkillInfoList += len;
		}

		len = pBattleModeSkillInfoList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 10929, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

UINT GetAppellationLevel(UINT ID)
{
	AppellationInfo* p = Appellations[ID].get();
	UINT Level = 0;
	if (p)
	{
		Level = p->Level;
	}
	return Level;
}

void NotifyAppellationList(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192];
	BYTE* p = buf;
	size_t len;
	Write8(p, Appellations.size()); // AppellationNum
	Mutex2.lock();
	std::map<UINT, std::shared_ptr<AppellationInfo>>::iterator iter;
	for (iter = Appellations.begin(); iter != Appellations.end(); iter++)
	{ // Achieves
		AppellationInfo* AppellationInfo = iter->second.get();
		if (!AppellationInfo)
		{
			continue;
		}
		BYTE* pRecord = p;
		Write16(pRecord, 0); // len

		Write8(pRecord, AppellationInfo->Type);		  // Type
		Write8(pRecord, AppellationInfo->Level);	  // Level
		Write8(pRecord, 0);							  // Status
		Write8(pRecord, AppellationInfo->Difficulty); // Difficulty
		Write32(pRecord, AppellationInfo->MapId);	  // MapId
		Write32(pRecord, AppellationInfo->Value);	  // Value

		len = pRecord - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Mutex2.unlock();
	len = p - buf;
	SendToClient(Client, 1616, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestWearAppellation(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR Type = Read8(p);
	UpdateAppellationType(Client->Uin, Type);
	ResponseWearAppellation(Client, Type);
}

void ResponseWearAppellation(std::shared_ptr<ClientNode> Client, UCHAR Type)
{
	BYTE buf[8192];
	BYTE* p = buf;
	size_t len;

	Write16(p, 0);	 // ResultId
	Write8(p, Type); // Type

	len = p - buf;
	SendToClient(Client, 1618, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestUnwearAppellation(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR Type = Read8(p);
	ResponseUnwearAppellation(Client, Type);
}

void ResponseUnwearAppellation(std::shared_ptr<ClientNode> Client, UCHAR Type)
{
	BYTE buf[8192];
	BYTE* p = buf;
	size_t len;

	Write16(p, 0);	 // ResultId
	Write8(p, Type); // Type

	len = p - buf;
	SendToClient(Client, 1619, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


NOINLINE void RequestRankedMatchDoBanMap(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	UINT MapID = Read32(Body);
	UCHAR Type = Read8(Body);
	UCHAR CurrentRound = Read8(Body);//当前轮数
	auto& CurPlayer = Client;
	//[RequestRankedMatchDoBanMap][MapID:0][Type:0][CurrentRound:0] 放弃选图

	if (CurPlayer->IsBanMap == false)
	{
		if (MapID == 0) {//玩家放弃选图
			CurPlayer->IsBanMap = true;
		}
		else
		{
			//玩家禁图处理
// 			if (CurPlayer->PlayerRoom) {
// 				CMatchBanMapInfo* MatchBanMapInfo = CurPlayer->PlayerRoom->MatchBanMapInfo;
// 				if (MatchBanMapInfo) {
// 					for (int j = 0; j < 8; j++) {
// 						if (MatchBanMapInfo[j].MapID == MapID) {
// 							CurPlayer->MatchBanMapIndex = j;
// 							break;
// 						}
// 
// 					}
// 				}
// 			}
			ResponseRankedMatchDoBanMap(CurPlayer, Type);
		}
	}
}

void ResponseRankedMatchDoBanMap(std::shared_ptr<ClientNode> Client, CONST UCHAR Type)
{

	UINT Uin = Client->Uin;
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Uin); //Uin
	Write8(p, Type); //Type
	Write8(p, 0); //BuffNum
	/*
	for (size_t i = 0; i < 0; i++)
	{ //BuffIDListInfo
		BYTE* pBuffIDListInfo = p;
		Write16(pBuffIDListInfo, 0); //len

		Write32(pBuffIDListInfo, 0); //BuffID
		Write8(pBuffIDListInfo, 0); //Lev

		len = pBuffIDListInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	len = p - buf;
	SendToClient(Client, 25010, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void NotifyBroadcast65(std::shared_ptr<ClientNode> Client)
{
	if (BroadCaston)
	{
		for (size_t i = 0; i < BroadCast.size(); i++)
		{
			std::vector<BYTE> buf(819200);
			BYTE* p = buf.data();
			size_t len;
			const std::string& news = BroadCast[i];
			if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
				continue; // 超出缓冲区，跳过这个广播

			len = NotifyBroadcast(p, 65, 1, 8, BroadCast[i]) - buf.data();
			SendToClient(Client, 541, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		}

	}
}



BYTE* NotifyBroadcast(BYTE*& ptr, UINT BCFlag, UINT ConTime, UINT DisplayFlag, std::string news)
{
	BYTE* pData = ptr;
	size_t len;
	{
		BYTE* pBroadcastInfo = pData;
		Write16(pBroadcastInfo, 0); // len

		Write8(pBroadcastInfo, BCFlag);	// BCFlag
		Write16(pBroadcastInfo, ConTime); // ConTime
		Write16(pBroadcastInfo, strlen(news.c_str())); // ContentLen
		memcpy(pBroadcastInfo, news.c_str(), strlen(news.c_str()));
		pBroadcastInfo += strlen(news.c_str());

		Write32(pBroadcastInfo, 0); // BroadcastType
		Write8(pBroadcastInfo, DisplayFlag);	// DisplayFlag
		Write32(pBroadcastInfo, 0); // ID1
		Write32(pBroadcastInfo, 0); // ID2
		Write16(pBroadcastInfo, 0); // AttachLen

		len = pBroadcastInfo - pData;
		Set16(pData, (WORD)len);
		pData += len;
	}
	return pData;
}


void NotifyCompetitionSystemConfig(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	typedef struct CompetitionCfg
	{
		char ID;
		char Status;
		std::string DescriptionLen;
		char Order;
	}*LPCompetition;
	const static CompetitionCfg CompetitionCfgCfgs[] =
	{
		{2,2,"/C03SSC2024超级联赛秋季S2分站/C00\n9.27-9.29、10.4-10.620:00-21:30\n首发绝版背景等你来拿！\n/<http_ex=https://speed.qq.com/cp/a20240913ssc/index.html>点击进入大赛官网>>>",1},
		{4,4,"幸运大奖赛火热开启，全民轻松竞技，海量道具轻松得！/C03每周末19点-21点更有限时竞技点翻倍活动开启/C00！",2},
		{1,1,"车队锦标赛，自由公平的热血比拼，决出真正的车队王者！",3},
		{5,2,"全新荣耀排位赛开启，公平竞技新征程！\n/C03每天09点-次日02点/C00，/C03限定飞饰与强力机甲/C00等你来战！",4},
		{3,4,"车神争霸赛正在进行中，更多榜单虚位以待！/C03极品A车GT1/C00等你来拿！",5},
		{6,1,"城邦专属赛事，每周五至周日14点-16点，18点-20点开启！期待您的挑战！",6},
	};


	constexpr unsigned char CompetitionCount = sizeof(CompetitionCfgCfgs) / sizeof(CompetitionCfgCfgs[0]); //

	Write8(p, CompetitionCount); //CompetitionCount
	for (size_t i = 0; i < CompetitionCount; i++)
	{ //Competition
		BYTE* pCompetition = p;
		Write16(pCompetition, 0); //len

		Write8(pCompetition, CompetitionCfgCfgs[i].ID); //ID
		Write8(pCompetition, CompetitionCfgCfgs[i].Status); //Status
		WriteString16(pCompetition, CompetitionCfgCfgs[i].DescriptionLen.c_str()); //DescriptionLen
		Write8(pCompetition, CompetitionCfgCfgs[i].Order); //Order

		len = pCompetition - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 20133, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyRelaxRoomAdvertisement(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write8(p, 0); //IsOpen
	WriteString16(p, "http://speed.qq.com/home/client_xyq.html"); //URL[]
	Write8(p, 0); //RelaxRoomNum

	len = p - buf;
	SendToClient(Client, 24009, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyActivityRecommendCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "000000000000000500C500980000000100000000000000020133A1310001388001404CFF00038E280133A1310001388001404CFF00038E28000EC8ABC3F1D4DACFDFC1ECB8A3C0FB000000000036687474703A2F2F73706565642E71712E636F6D2F686F6D652F636C69656E745F61637469766974792E68746D6C3F69643D31353036300008D4DACFDFB8A3C0FB0008D4DACFDFB8A3C0FB0008BBEEB6AFC3E8CAF6000A000000000000000000210000000000000000000000000000000000000000000000000000000000000000DA00AD0000000200000000000000060133A2B300016B4801404CFF00038E280133A2B300016B4801404CFF00038E280023BBEEB6AFBFAAB7C5CAB1BCE4CEAA34D4C23136C8D5CFC2CEE7C1BDB5E3D6C1CBC4B5E3000000000036687474703A2F2F73706565642E71712E636F6D2F686F6D652F636C69656E745F61637469766974792E68746D6C3F69643D31353035390008C9CCB3C7B8A3C0FB0008C9CCB3C7B8A3C0FB0008BBEEB6AFC3E8CAF6000A000000000000000000210000000000000000000000000000000000000000000000000000000000000000DA00AD00000003000000000000000B0133C83E0000000001404CFF00038E280133C83E0000000001404CFF00038E280023BBEEB6AFBFAAB7C5CAB1BCE4CEAA34D4C23136C8D5CFC2CEE7C1BDB5E3D6C1CBC4B5E3000000000036687474703A2F2F73706565642E71712E636F6D2F686F6D652F636C69656E745F61637469766974792E68746D6C3F69643D31353035360008B3ACD6B5BBEEB6AF0008B3ACD6B5BBEEB6AF0008BBEEB6AFC3E8CAF6000A000000000000000000210000000000000000000000000000000000000000000000000000000000000000DA00AD0000000400000000000000020133C7DE0000000001404CFF000397EC0133C7DE0000000001404CFF000397EC0023BBEEB6AFBFAAB7C5CAB1BCE4CEAA34D4C23136C8D5CFC2CEE7C1BDB5E3D6C1CBC4B5E3000000000036687474703A2F2F73706565642E71712E636F6D2F686F6D652F636C69656E745F61637469766974792E68746D6C3F69643D31353035370008C3E2B7D1B8A3C0FB0008C3E2B7D1B8A3C0FB0008BBEEB6AFC3E8CAF6000A000000000000000000210000000000000000000000000000000000000000000000000000000000000000CD00A00000000500000000000000010133C8950000000001404CFF000397EC0133C8950000000001404CFF000397EC0016D0C2B0E6D5C6B7C920C8ABC3F1CBCDBCABC6B741B3B5000000000036687474703A2F2F73706565642E71712E636F6D2F686F6D652F636C69656E745F61637469766974792E68746D6C3F69643D31353035380008B8FCB6E0BEABB2CA0008B8FCB6E0BEABB2CA0008BBEEB6AFC3E8CAF6000A00000000000000000021000000000000000000000000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 24448, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	typedef struct TabNumCfg
	{
		UINT TabOrder;
		UINT TabType;
		UINT TabAttr;
		UINT TabOpenDate;
		UINT TabOpenTime;
		UINT TabCloseDate;
		UINT TabCloseTime;
		UINT ActiveOpenDate;
		UINT ActiveOpenTime;
		UINT ActiveCloseDate;
		UINT ActiveCloseTime;
		std::string OpenTimeStrLen;
		UINT CheckOccasion;
		std::string TabURLLen;
		std::string TabNameLen;
		std::string ActiveNameLen;
		std::string ActiveDescLen;
	}*LPTabNum;
	const static TabNumCfg TabNumCfgCfgs[] =
	{
		{1,0,2,0x0133A131,0x00013880,0x01404CFF,0x00038E28,0x0133A131,0x00013880,0x01404CFF,0x00038E28,"全民在线领福利",0,"http://speed.qq.com/home/client_activity.html?id=15060","在线福利","在线福利","活动描述"},
		{2,0,6,0x0133A2B3,0x00016B48,0x01404CFF,0x00038E28,0x0133A2B3,0x00016B48,0x01404CFF,0x00038E28,"活动开放时间为4月16日下午两点至四点",0,"http://speed.qq.com/home/client_activity.html?id=15059","商城福利","商城福利","活动描述"},
		{3,0,11,0x0133C83E,0,0x01404CFF,0x00038E28,0x0133C83E,0,0x01404CFF,0x00038E28,"活动开放时间为4月16日下午两点至四点",0,"http://speed.qq.com/home/client_activity.html?id=15056","超值活动","超值活动","活动描述"},
		{4,0,2,0x0133C7DE,0,0x01404CFF,0x000397EC,0x0133C7DE,0,0x01404CFF,0x000397EC ,"活动开放时间为4月16日下午两点至四点",0,"http://speed.qq.com/home/client_activity.html?id=15057","免费福利","免费福利","活动描述"},
		{5,0,1,0x0133C895,0,0x01404CFF,0x000397EC,0x0133C895 ,0,0x01404CFF,0x000397EC ,"新版掌飞全民送极品A车",0,"http://speed.qq.com/home/client_activity.html?id=15058","更多精彩","更多精彩","活动描述"},
	};

	constexpr unsigned char TabNum = sizeof(TabNumCfgCfgs) / sizeof(TabNumCfgCfgs[0]); //

	Write32(p, Client->Uin); //Uin
	Write32(p, TabNum); //TabNum
	for (size_t i = 0; i < TabNum; i++)
	{ //ActivityOneTabCfg
		BYTE* pActivityOneTabCfg = p;
		Write16(pActivityOneTabCfg, 0); //len

		{ //ActivityRecommendTabBaseCfg
			BYTE* pActivityRecommendTabBaseCfg = pActivityOneTabCfg;
			Write16(pActivityRecommendTabBaseCfg, 0); //len

			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabOrder); //TabOrder
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabType); //TabType
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabAttr); //TabAttr
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabOpenDate); //TabOpenDate
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabOpenTime); //TabOpenTime
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabCloseDate); //TabCloseDate
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabCloseTime); //TabCloseTime
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].ActiveOpenDate); //ActiveOpenDate
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].ActiveOpenTime); //ActiveOpenTime
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].ActiveCloseDate); //ActiveCloseDate
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].ActiveCloseTime); //ActiveCloseTime
			WriteString16(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].OpenTimeStrLen.c_str()); //OpenTimeStrLen
			Write32(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].CheckOccasion); //CheckOccasion
			WriteString16(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabURLLen.c_str()); //TabURLLen
			WriteString16(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].TabNameLen.c_str()); //TabNameLen
			WriteString16(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].ActiveNameLen.c_str()); //ActiveNameLen
			WriteString16(pActivityRecommendTabBaseCfg, TabNumCfgCfgs[i].ActiveDescLen.c_str()); //ActiveDescLen

			len = pActivityRecommendTabBaseCfg - pActivityOneTabCfg;
			Set16(pActivityOneTabCfg, (WORD)len);
			pActivityOneTabCfg += len;
		}
		{ //ActivityRecommendGiftBagCfg
			BYTE* pActivityRecommendGiftBagCfg = pActivityOneTabCfg;
			Write16(pActivityRecommendGiftBagCfg, 0); //len

			Write32(pActivityRecommendGiftBagCfg, 0); //CurrentValue
			Write32(pActivityRecommendGiftBagCfg, 0); //GiftBagNum
			for (size_t i = 0; i < 0; i++)
			{ //ActivityRecommendGiftBagAwardToClient
				BYTE* pActivityRecommendGiftBagAwardToClient = pActivityRecommendGiftBagCfg;
				Write16(pActivityRecommendGiftBagAwardToClient, 0); //len

				Write32(pActivityRecommendGiftBagAwardToClient, 0); //Index
				Write32(pActivityRecommendGiftBagAwardToClient, 0); //LimitValue
				Write32(pActivityRecommendGiftBagAwardToClient, 0); //Status
				Write32(pActivityRecommendGiftBagAwardToClient, 0); //BoxID
				Write8(pActivityRecommendGiftBagAwardToClient, 0); //BagName[]
				{ //Award
					BYTE* pAward = pActivityRecommendGiftBagAwardToClient;
					Write16(pAward, 0); //len

					Write32(pAward, 0); //Exp
					Write32(pAward, 0); //Money
					Write32(pAward, 0); //SuperMoney
					Write16(pAward, 0); //ItemNum
					for (size_t i = 0; i < 0; i++)
					{ //ItemInfo
						BYTE* pItemInfo = pAward;
						Write16(pItemInfo, 0); //len

						Write32(pItemInfo, 0); //ItemID
						Write32(pItemInfo, 0); //ItemNum
						Write32(pItemInfo, 0); //AvailPeriod
						Write8(pItemInfo, 0); //Status
						WriteUn(pItemInfo, 0); //ObtainTime
						Write32(pItemInfo, 0); //OtherAttribute
						Write16(pItemInfo, 0); //ItemType

						len = pItemInfo - pAward;
						Set16(pAward, (WORD)len);
						pAward += len;
					}
					Write32(pAward, 0); //Coupons
					Write32(pAward, 0); //GuildPoint
					Write32(pAward, 0); //LuckMoney
					Write8(pAward, 0); //ExtendInfoNum
					for (size_t i = 0; i < 0; i++)
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

					len = pAward - pActivityRecommendGiftBagAwardToClient;
					Set16(pActivityRecommendGiftBagAwardToClient, (WORD)len);
					pActivityRecommendGiftBagAwardToClient += len;
				}

				len = pActivityRecommendGiftBagAwardToClient - pActivityRecommendGiftBagCfg;
				Set16(pActivityRecommendGiftBagCfg, (WORD)len);
				pActivityRecommendGiftBagCfg += len;
			}

			len = pActivityRecommendGiftBagCfg - pActivityOneTabCfg;
			Set16(pActivityOneTabCfg, (WORD)len);
			pActivityOneTabCfg += len;
		}
		{ //Award
			BYTE* pAward = pActivityOneTabCfg;
			Write16(pAward, 0); //len

			Write32(pAward, 0); //Exp
			Write32(pAward, 0); //Money
			Write32(pAward, 0); //SuperMoney
			Write16(pAward, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAward;
				Set16(pAward, (WORD)len);
				pAward += len;
			}
			Write32(pAward, 0); //Coupons
			Write32(pAward, 0); //GuildPoint
			Write32(pAward, 0); //LuckMoney
			Write8(pAward, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
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

			len = pAward - pActivityOneTabCfg;
			Set16(pActivityOneTabCfg, (WORD)len);
			pActivityOneTabCfg += len;
		}

		len = pActivityOneTabCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //AwardAvailable

	len = p - buf;
	SendToClient(Client, 24448, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyAdvertisement(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, 0); //ADFlag
	Write32(p, -1); //ContinuanceTime
	WriteString16(p, "月亮"); //ContentLen

	len = p - buf;
	SendToClient(Client, 549, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyFileCheckList(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	typedef struct FileNumCfg
	{
		std::string FileNameLen;
	}*LPFileNum;
	const static FileNumCfg FileNumCfgs[] =
	{
		"vfilesys.dll",
		"Network.dll",
		"P2PAdapter.dll",
		"Top-Kart.dll",
		"GameApp.exe",
		"QQTPPP2ND.dll",
		"TerSafe.dll",
		"QQSpeed_loader.exe",
		"TenSLX.dll",
		"Data\\Avatar\\ItemListClient1.luc",
		"Data\\Avatar\\ItemListClient2.luc",
		"Data\\Avatar\\ItemListClient3.luc",
		"Data\\Avatar\\ItemListClient4.luc",
		"Data\\Avatar\\ItemListClient5.luc",
		"Data\\Avatar\\show_avatar.luc",
		"Data\\Avatar\\show_avatar_outdoor.luc",
		"Data\\Config\\gpconfig.luc",
		"Data\\Config\\TaskConfig\\task.luc",
		"Data\\Config\\GPSettings.luc",
		"Data\\Config\\DescConstant.luc",
		"Data\\Config\\DriftCfg.luc",
		"Data\\Config\\collisionCfg.luc",
		"Data\\downloadfilelist.luc",
		"Res\\uires\\EventHandle.luc",
		"Res\\uires\\shop.luc",
		"Res\\uires\\simpleshop.luc",
		"Res\\uires\\uiroot.luc",
		"Res\\uires\\uires.luc",
		"Res\\uires\\uicmd.luc",
		"Res\\uires\\taskui.luc",
		"Res\\uires\\Login.luc",
		"Res\\uires\\playerinfo.luc",
		"Res\\uires\\startmenu.luc",
		"Res\\uires\\kart_stone_sys_cfg.luc",
	};
	constexpr unsigned char FileNum = sizeof(FileNumCfgs) / sizeof(FileNumCfgs[0]); //

	Write32(p, Client->Uin); //Uin
	Write16(p, FileNum); //FileNum
	for (size_t i = 0; i < FileNum; i++)
	{ //FileList
		BYTE* pFileList = p;
		Write16(pFileList, 0); //len

		WriteString_MC(pFileList, FileNumCfgs[i].FileNameLen.c_str()); //FileNameLen

		len = pFileList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 862, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyTVLiveInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	typedef struct CommonTipsNumCfg
	{
		char TipsType;
		UINT Second;
		std::string TipsLen;
	}*LPCommonTipsNum;
	const static CommonTipsNumCfg CommonTipsNumCfgs[] =
	{
		{2,30,"幸运招募拿大量点券和永久A车"},
		{3,0,"SSC超级联赛秋季S2分站赛线上赛签到9月27日-10月6日开启，比赛日完成1局比赛即可完成签到哦！"},
	};
	constexpr unsigned char CommonTipsNum = sizeof(CommonTipsNumCfgs) / sizeof(CommonTipsNumCfgs[0]); //
	Write32(p, Client->Uin); //Uin
	Write32(p, CommonTipsNum); //CommonTipsNum
	for (size_t i = 0; i < CommonTipsNum; i++)
	{ //CommonTipsInfo
		BYTE* pCommonTipsInfo = p;
		Write16(pCommonTipsInfo, 0); //len

		Write8(pCommonTipsInfo, CommonTipsNumCfgs[i].TipsType); //TipsType
		Write32(pCommonTipsInfo, CommonTipsNumCfgs[i].Second); //Second
		WriteString16(pCommonTipsInfo, CommonTipsNumCfgs[i].TipsLen.c_str()); //TipsLen

		len = pCommonTipsInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24822, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyClientUpdateItemDescInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	typedef struct ItemCountCfg
	{
		UINT ItemID;
		std::string NameLen;
	}*LPCommonTipsNum;
	const static ItemCountCfg ItemCountCfgCfgs[] =
	{
		{123492,"至曜·玄甲"},
		{139993,"至曜·焚炎"},
		{127617,"至曜-地狱火"},
		{136684,"至曜·玄匣"},
	};
	constexpr unsigned char ItemCount = sizeof(ItemCountCfgCfgs) / sizeof(ItemCountCfgCfgs[0]); //


	Write32(p, 65); //Version
	Write16(p, ItemCount); //ItemCount
	for (size_t i = 0; i < ItemCount; i++)
	{ //ItemList
		BYTE* pItemList = p;
		Write16(pItemList, 0); //len

		Write32(pItemList, ItemCountCfgCfgs[i].ItemID); //ItemID
		WriteString16(pItemList, ItemCountCfgCfgs[i].NameLen.c_str()); //NameLen

		len = pItemList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24379, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyDynamicCode(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write8(p, 1); //DynamicNum
	for (size_t i = 0; i < 0; i++)
	{ //Dynamic
		BYTE* pDynamic = p;
		Write16(pDynamic, 0); //len

		Write32(pDynamic, 0); //FileNameLen
		Write32(pDynamic, 0); //FileContentLen
		Write32(pDynamic, 0); //DescInfoLen

		len = pDynamic - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 21032, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyClientSkillTrainingTipsInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin

	WriteHex(p, "00000000080000000BB8000001005B00783C0000535265732F75697265732F4C6F62627955492F436F6E6669672F546F704D656E752F5265732F427562626C652F546F704D656E755F546970427562626C655F54696D65724368616C6C656E6765326E642E706E67");
	len = p - buf;
	SendToClient(Client, 28341, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write32(p, 0); //Time
	Write8(p, 0); //TipsType
	Write8(p, 0); //TipsID
	Write32(p, 0); //TipsCDTime
	Write8(p, 0); //LevLimit
	Write8(p, 0); //Status
	Write8(p, 0); //ModeNum
	for (size_t i = 0; i < 0; i++)
	{ //ModeRecommendInfo
		BYTE* pModeRecommendInfo = p;
		Write16(pModeRecommendInfo, 0); //len

		Write8(pModeRecommendInfo, 0); //ButtonID
		Write8(pModeRecommendInfo, 0); //SusTime
		Write8(pModeRecommendInfo, 0); //SurfaceID
		Write16(pModeRecommendInfo, 0); //Urlen

		len = pModeRecommendInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 28341, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void ResponseGetPrivilegeIntroInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin

	WriteHex(p, "000000000000000000000009005000000001000000000000000000000000000000000000000100000000000300100001044700000001000002D000000010000030580000000100000000000000100000358C00000001000000000000005000000009000000000000000000000000000000000000000200000000000300100001044800000001000002D000000010000083EC000000020000000000000010000035DB0000000100000000000000500000001D000000000000000000000000000000000000000300000000000300100001044900000001000002D000000010000086240000000100000000000000100000305C00000002000000000000005000000031000000010000000000000000000000000000000400000000000300100001044A00000001000002D000000010000106EE0000000100000000000000100000862100000001000000000000005000000063000000010000000000000000000000000000000500000000000300100001044B00000001000002D000000010000106EF00000001000000000000001000008627000000010000000000000050000000C7000000020000000000000000000000000000000600000000000300100001044C00000001000002D000000010000106EF000000020000000000000010000086250000000100000000000000500000012B000000020000000000000000000000000000000700000000000300100001044D00000001000002D000000010000106EF00000003000000000000001000008622000000010000000000000050000001F3000000030000000000000000000000000000000800000000000300100001044E00000001000002D000000010000106F100000003000000000000001000008628000000010000000000000050000003E7000000040000000000000000000000000000000900000000000300100001044F00000001000002D000000010000106F2000000030000000000000010000100A600000001000000000000D4DAD3CECFB7C4DACDA8B9FD51B5E3BBF251B1D2CFFBB7D1BFC9D2D4BBF1B5C3F7C8C1A6D6B5BACDBBD8C0A1B5E3A1A35C6E202F433031203151B1D2203D2031F7C8C1A6D6B5203D203130BBD8C0A1B5E3A1A33151B5E33D31BBD8C0A1B5E3A1A300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 16055, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;

	Write32(p, 0); //CharmValueOfMonth
	Write32(p, 0); //VipLevel
	Write32(p, 9); //PrivilegeInfoNums
	for (size_t i = 0; i < 0; i++)
	{ //PrivilegeInfos
		BYTE* pPrivilegeInfos = p;
		Write16(pPrivilegeInfos, 0); //len

		Write32(pPrivilegeInfos, 0); //NextLevelCharmVlaue
		Write32(pPrivilegeInfos, 0); //SearchTreasureNums
		Write32(pPrivilegeInfos, 0); //GetTreasureNums
		Write32(pPrivilegeInfos, 0); //CritCardNums
		Write32(pPrivilegeInfos, 0); //MechLargerCardNums
		Write32(pPrivilegeInfos, 0); //MallBuyLevel
		Write32(pPrivilegeInfos, 0); //HasAlreadyReceived
		Write32(pPrivilegeInfos, 0); //AwardItemNums
		for (size_t i = 0; i < 0; i++)
		{ //FirstReachLevelItemInfos
			BYTE* pFirstReachLevelItemInfos = pPrivilegeInfos;
			Write16(pFirstReachLevelItemInfos, 0); //len

			Write32(pFirstReachLevelItemInfos, 0); //itemID
			Write32(pFirstReachLevelItemInfos, 0); //itemNums
			Write32(pFirstReachLevelItemInfos, 0); //AvailPeriod
			Write32(pFirstReachLevelItemInfos, 0); //isBestQuality

			len = pFirstReachLevelItemInfos - pPrivilegeInfos;
			Set16(pPrivilegeInfos, (WORD)len);
			pPrivilegeInfos += len;
		}

		len = pPrivilegeInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	WriteUn(p, 0); //AnouncementInfo[]

	len = p - buf;
	SendToClient(Client, 16055, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void ResponseGetExclusiveSpeedWayInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin

	WriteHex(p, "000000000000000000000004004A0000010000000064000000640000000100000003000000030010000076FB00000001FFFFFFFF0000001000007D4F00000001FFFFFFFF000000100000EB8000000001FFFFFFFF0000004A00000089000000640000006400000001000000050000000300100000F7CA00000001FFFFFFFF000000100000F80D00000001FFFFFFFF0000001000004E9D00000001FFFFFFFF0000004A000000BB00000064000000C800000001000000070000000300100000EC0D00000001FFFFFFFF000000100000706A00000001FFFFFFFF00000010000065FF00000001FFFFFFFF0000004A000000DB00000064000000C8000000010000000900000003001000005F5A00000001FFFFFFFF0000001000007B5200000001FFFFFFFF0000001000005C5F00000001FFFFFFFF00002F433033C3E2B7D1D1B0B1A6A3BA2F433030BDF8C8EBC8FCB5C0A3ACCCBDD1B0BCABC6B7B4F3BDB1A3A15C6E202F433033D2BBBCFCD1B0B1A6A3BA2F433030BFECCBD9C9A8B5B4A3ACC7E1CBC9D3AEC8A1B4F3BDB1A3A10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 16060, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;


	WriteUn(p, 0); //SearchTreasureAwardItemNums
	WriteUn(p, 0); //SpeedWayNums
	for (size_t i = 0; i < 0; i++)
	{ //SpeedWayInfo
		BYTE* pSpeedWayInfo = p;
		Write16(pSpeedWayInfo, 0); //len

		WriteUn(pSpeedWayInfo, 0); //MapID
		WriteUn(pSpeedWayInfo, 0); //FreeSearchCostCouponNums
		WriteUn(pSpeedWayInfo, 0); //FastSearchCostCouponNums
		WriteUn(pSpeedWayInfo, 0); //FastSearchCostTreasureSearchCouponNums
		WriteUn(pSpeedWayInfo, 0); //SpeedWayOpenStat
		WriteUn(pSpeedWayInfo, 0); //SpeedWayOpenVipLevel
		WriteUn(pSpeedWayInfo, 0); //AwardItemNums
		for (size_t i = 0; i < 0; i++)
		{ //SearchTreasureAwardItems
			BYTE* pSearchTreasureAwardItems = pSpeedWayInfo;
			Write16(pSearchTreasureAwardItems, 0); //len

			WriteUn(pSearchTreasureAwardItems, 0); //itemID
			WriteUn(pSearchTreasureAwardItems, 0); //itemNums
			Write32(pSearchTreasureAwardItems, 0); //AvailPeriod
			WriteUn(pSearchTreasureAwardItems, 0); //isBestQuality

			len = pSearchTreasureAwardItems - pSpeedWayInfo;
			Set16(pSpeedWayInfo, (WORD)len);
			pSpeedWayInfo += len;
		}

		len = pSpeedWayInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	WriteUn(p, 0); //SearchTreasureRuleInfo[]

	len = p - buf;
	SendToClient(Client, 16060, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}




void ResponsePRGetInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	WriteHex(p, "0000000B00440000000100000000003A00000000000000000000000000010019000106EE0000000100000000006700C4A9000000000000000000000000000000000000000000000000440000000100000000003A0000000000000000000000000001001900017B200000000100000168006700C4A9000000000000000000000000000000000000000000000000440000000300000000003A000000000000000000000000000100190001113B0000000300000000006700C4A9000000000000000000000000000000000000000000000000440000000300000000003A0000000000000000000000000001001900015BFC0000000100000168006700C4A9000000000000000000000000000000000000000000000000440000000400000000003A000000000000000000000000000100190001434E0000000200000000006700C4A9000000000000000000000000000000000000000000000000440000000400000000003A00000000000000000000000000010019000177ED0000000100000168006700C4A9000000000000000000000000000000000000000000000000440000000600000000003A00000000000000000000000000010019000166D00000000100000168006700C4A9000000000000000000000000000000000000000000000000440000000200000000003A000000000000000000000000000100190001739600000001000001E0006700C4A9000000000000000000000000000000000000000000000000440000000200000000003A0000000000000000000000000001001900011F810000000800000000006700C4A9000000000000000000000000000000000000000000000000440000000500000000003A000000000000000000000000000100190002100A0000000100000168006700C4A9000000000000000000000000000000000000000000000000440000000500000000003A000000000000000000000000000100190001854E0000000200000000006700C4A90000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 24085, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;


	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write32(p, 0); //AwardNum
	for (size_t i = 0; i < 0; i++)
	{ //PRAward
		BYTE* pPRAward = p;
		Write16(pPRAward, 0); //len

		Write32(pPRAward, 0); //Type
		Write32(pPRAward, 0); //Status
		{ //Award
			BYTE* pAward = pPRAward;
			Write16(pAward, 0); //len

			Write32(pAward, 0); //Exp
			Write32(pAward, 0); //Money
			Write32(pAward, 0); //SuperMoney
			Write16(pAward, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAward;
				Set16(pAward, (WORD)len);
				pAward += len;
			}
			Write32(pAward, 0); //Coupons
			Write32(pAward, 0); //GuildPoint
			Write32(pAward, 0); //LuckMoney
			Write8(pAward, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
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

			len = pAward - pPRAward;
			Set16(pPRAward, (WORD)len);
			pPRAward += len;
		}

		len = pPRAward - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24085, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetConsumeScoreConfUrl(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	int Time = Read32(p);
	UCHAR Md5StrLen = Read8(p);

}

void ResponseGetConsumeScoreConfUrl(std::shared_ptr<ClientNode> Client, UCHAR Md5StrLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write8(p, Md5StrLen); //Md5StrLen

	WriteHex(p, "42363431424133333144413935343538343539343843333335333735313441310000000000");
	len = p - buf;
	SendToClient(Client, 11471, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;

	Write16(p, 0); //BroadcastLen
	Write16(p, 0); //ExchangeNumber
	for (size_t i = 0; i < 0; i++)
	{ //Exchange
		BYTE* pExchange = p;
		Write16(pExchange, 0); //len

		Write32(pExchange, 0); //PackId
		Write32(pExchange, 0); //ItemId
		Write32(pExchange, 0); //ItemAvailPeriod
		Write32(pExchange, 0); //ItemNumber
		Write32(pExchange, 0); //Score
		Write8(pExchange, 0); //HasTimeLimit
		for (size_t i = 0; i < 0; i++)
		{ //TimeLimit
			BYTE* pTimeLimit = pExchange;
			Write16(pTimeLimit, 0); //len

			Write32(pTimeLimit, 0); //StartTime
			Write32(pTimeLimit, 0); //EndTime

			len = pTimeLimit - pExchange;
			Set16(pExchange, (WORD)len);
			pExchange += len;
		}
		Write8(pExchange, 0); //HasDiscount
		for (size_t i = 0; i < 0; i++)
		{ //Discount
			BYTE* pDiscount = pExchange;
			Write16(pDiscount, 0); //len

			Write32(pDiscount, 0); //OrigScore
			Write32(pDiscount, 0); //DiscountValue

			len = pDiscount - pExchange;
			Set16(pExchange, (WORD)len);
			pExchange += len;
		}

		len = pExchange - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 11471, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestQsLabCommittalTaskInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT SeasonId = Read32(p);
	UCHAR Type = Read8(p);
	ResponseQsLabCommittalTaskInfo(Client);
}

void ResponseQsLabCommittalTaskInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "0000000000000002002D00030500080000143D000000080000143E000000080000143F000000080000144000000008000014410000002D0004050008000013270000000800001328000000080000132900000008000013330000000800001365000001");
	len = p - buf;
	SendToClient(Client, 28419, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;

	Write32(p, 0); //Uin
	Write16(p, 0); //ResultID
	Write8(p, 0); //TaskNum
	for (size_t i = 0; i < 0; i++)
	{ //QsLabCommittalTaskInfo
		BYTE* pQsLabCommittalTaskInfo = p;
		Write16(pQsLabCommittalTaskInfo, 0); //len

		Write32(pQsLabCommittalTaskInfo, 0); //TaskID
		Write32(pQsLabCommittalTaskInfo, 0); //UnLockLeftTime
		Write8(pQsLabCommittalTaskInfo, 0); //Status

		len = pQsLabCommittalTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //SeasonNum
	for (size_t i = 0; i < 0; i++)
	{ //QsLabSeasonTaskCfg
		BYTE* pQsLabSeasonTaskCfg = p;
		Write16(pQsLabSeasonTaskCfg, 0); //len

		Write16(pQsLabSeasonTaskCfg, 0); //SeasonID
		Write8(pQsLabSeasonTaskCfg, 0); //TaskNum
		for (size_t i = 0; i < 0; i++)
		{ //TaskAwardStatus
			BYTE* pTaskAwardStatus = pQsLabSeasonTaskCfg;
			Write16(pTaskAwardStatus, 0); //len

			Write32(pTaskAwardStatus, 0); //TaskId
			Write8(pTaskAwardStatus, 0); //ReceiveStatus
			Write8(pTaskAwardStatus, 0); //AwardStatus

			len = pTaskAwardStatus - pQsLabSeasonTaskCfg;
			Set16(pQsLabSeasonTaskCfg, (WORD)len);
			pQsLabSeasonTaskCfg += len;
		}

		len = pQsLabSeasonTaskCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //Type

	len = p - buf;
	SendToClient(Client, 28419, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}