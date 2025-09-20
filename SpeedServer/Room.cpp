#include "Room.h"
#include "Map.h"
#include <time.h>
#include <map>
#include "Database.h"
#include <regex>
#include "DBConnection.h"
#include "Item.h"
#include <set>

//std::deque<std::shared_ptr<ClientNode>> RankedMatchQueue;
//std::deque<std::shared_ptr<ClientNode>> BorderMatchQueue;
std::deque<std::pair<std::shared_ptr<ClientNode>, std::chrono::steady_clock::time_point>> RankedMatchQueue;
std::deque<std::pair<std::shared_ptr<ClientNode>, std::chrono::steady_clock::time_point>> BorderMatchQueue;
std::recursive_mutex RankedQueueMutex;
std::recursive_mutex BorderQueueMutex;
void RequestEnterMatchRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
std::map<long, std::shared_ptr<RoomNode>> Rooms;
std::map<long, std::shared_ptr<RoomNode>> BorderRooms;
HANDLE TimerQueue = CreateTimerQueue();
//BYTE chewangkaoyan[100] = { 0 };
BYTE shumiaoyunshu[100] = { 0 };
BYTE baofengtiaozhan[2048] = { 0 };
int baofengmap[5] = { 137,118,120,108,135 };
extern int obs;
void WINAPI OnDanceTimer(std::shared_ptr<RoomNode> Room);
extern UINT GetRandMap(short MapID);
std::recursive_mutex Mutex;
extern std::recursive_mutex ClientMutex;
long SeqRoomID = 100;
//extern DWORD WINAPI OnSingleOver(PVOID Client);
void NotifyChangeReadyState(std::shared_ptr<ClientNode> Client, UINT Uin, bool State);
void NotifyChangeSeatState(std::shared_ptr<ClientNode> Client, char SeatID, char SeatState);
void ResponseStartQuickMatchGame2(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID);
extern void WINAPI OnBegin(std::shared_ptr<RoomNode> Room);
extern int mode28346;
extern int GMTime[4];
extern std::list<UINT> modecar;

extern void NotifyGameNPCInfo(std::shared_ptr<ClientNode> Client);
extern void RequestEnter3DRoom(std::shared_ptr<ClientNode> Client);
extern void NoticeClient(std::shared_ptr<ClientNode> Client, std::string news);
//void NotifyChangeRoomOwner(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);
extern UINT DanceTime;
extern UINT SyncTime;
extern std::map<long, std::shared_ptr<ClientNode>> Clients;
extern std::shared_ptr<MAPPOOL> RankMap;
extern UINT NpcNum;
extern int SkipDancetime;
extern std::string week;
extern int Stake[2];
extern void virtualsend(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo> Items, short count, UINT itemid);
extern void NotifyUpdateKartSkillStoneInfoConfig(std::shared_ptr<ClientNode> Client, int KartID);
extern bool isLock(UINT Itemid);
extern std::shared_ptr<ClientNode> GetClient(long Uin);
extern void ResponseChangeOBState(std::shared_ptr<ClientNode> Client, BOOL OBState);
extern void NotifyChangeOBState(std::shared_ptr<ClientNode> Client, BOOL OBState, UINT Uin);
extern int SendAnyItem(std::shared_ptr<ClientNode> Client, char cdk[]);
extern std::string GetItemName(UINT Itemid);
extern int Rankset[5];
extern int Borderset[6];
extern std::shared_ptr<RANDOMMAPPOOL> GetRandomMapPool(long PoolID);
extern int BorderManTime;
extern int RankerManTime;
extern std::set<UINT> ClothesGMUins;  //只能刷衣服
extern std::set<UINT> CarGMUins;  //只能刷车
extern std::set<UINT> AllGMUins;  //全部
extern std::set<UINT> AdminUins;//赌注模式管理 
extern std::string ClothesGMMess;
extern std::string CarGMMess;
extern std::string GetItemMess(UINT ID);
extern int IsBottomGame[4];
extern int IsBottomGame4or6[2];
extern int LimitTalk;
extern std::string GMMess;
extern int Mantissa[4];
extern int Uinew;
extern double version;
int GVoice = 0;
void NotifyBugletTalk(std::shared_ptr<ClientNode> Client, const char* Content, short clength, char BugletType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Client->BugletTalk += 1;

	if (Client->Uin > 10000) {
		PlayerDB_UpdateBugletTalk(Client);
	}


	Write8(p, 1); //BugletNum
	for (size_t i = 0; i < 1; i++)
	{ //BugletInfo
		BYTE* pBugletInfo = p;
		Write16(pBugletInfo, 0); //len

		Write32(pBugletInfo, Client->Uin); //SrcUin
		memset(pBugletInfo, 0, 17);

		memcpy(pBugletInfo, Client->NickName, 17);

		pBugletInfo += 17;
		//WriteString(pBugletInfo, 0); //SrcNickName[]

		memset(pBugletInfo, 0, 17);//SrcGuildName[]
		pBugletInfo += 17;
		Write8(pBugletInfo, 0); //Gender
		Write16(pBugletInfo, clength); //ContentLength
		memcpy(pBugletInfo, Content, clength);
		pBugletInfo += clength;


		Write8(pBugletInfo, BugletType); //BugletType
		Write32(pBugletInfo, 0); //Identify
		Write16(pBugletInfo, 0x0107); //VipFlag
		memset(pBugletInfo, 0, 18); //Contentid[]
		pBugletInfo += 18;
		{ //NobleInfo
			BYTE* pNobleInfo = pBugletInfo;
			Write16(pNobleInfo, 0); //len

			Write32(pNobleInfo, Client->NobleID); //NobleID
			Write8(pNobleInfo, Client->NobleLevel); //NobleLevel
			Write32(pNobleInfo, 10); //NoblePoint
			Write32(pNobleInfo, -1); //NobleLeftDays

			len = pNobleInfo - pBugletInfo;
			Set16(pBugletInfo, (WORD)len);
			pBugletInfo += len;
		}
		{ //GuildVipBaseInfo
			BYTE* pGuildVipBaseInfo = pBugletInfo;
			Write16(pGuildVipBaseInfo, 0); //len

			Write8(pGuildVipBaseInfo, Client->GuildVipLevel); //GuildVipLevel
			Write32(pGuildVipBaseInfo, Client->GuildVipPoint); //GuildVipPoint

			len = pGuildVipBaseInfo - pBugletInfo;
			Set16(pBugletInfo, (WORD)len);
			pBugletInfo += len;
		}
		Write8(pBugletInfo, 1); //HasLoverVip
		for (size_t i = 0; i < 1; i++)
		{ //LoverVipInfo
			BYTE* pLoverVipInfo = pBugletInfo;
			Write16(pLoverVipInfo, 0); //len

			Write8(pLoverVipInfo, Client->LoverVipLevel); //LoverVipLevel
			Write32(pLoverVipInfo, Client->LoverVipPoint); //LoverVipPoint
			Write8(pLoverVipInfo, 10); //GrowRate

			len = pLoverVipInfo - pBugletInfo;
			Set16(pBugletInfo, (WORD)len);
			pBugletInfo += len;
		}
		Write8(pBugletInfo, Client->GardenLevel); //GardenLevel
		Write32(pBugletInfo, 9); //ConsumeVipLevel
		{ //EmperorInfo
			BYTE* pEmperorInfo = pBugletInfo;
			Write16(pEmperorInfo, 0); //len

			Write8(pEmperorInfo, Client->EmperorLevel); //EmperorLevel
			Write32(pEmperorInfo, Client->EmperorPoint); //EmperorPoint
			Write32(pEmperorInfo, -1); //EmperorLeftDays
			Write8(pEmperorInfo, 10); //EmperorGrowRate

			len = pEmperorInfo - pBugletInfo;
			Set16(pBugletInfo, (WORD)len);
			pBugletInfo += len;
		}
		Write8(pBugletInfo, 0); //HasCarLottoryRecord

		len = pBugletInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	std::map<long, std::shared_ptr<ClientNode>>::iterator iter = Clients.begin();
	while (iter != Clients.end())
	{
		std::shared_ptr<ClientNode> DstClient = iter->second;
		if (DstClient)
		{
			SendToClient(DstClient, 555, buf.data(), len, DstClient->ConnID, FE_PLAYER, DstClient->ConnID, Notify);
		}
		iter++;
	}
}

void QuickbugLeaveGame(std::shared_ptr<ClientNode> Client)
{
	SendToClient(Client, 514, (const BYTE*)"\x00\x00\x00\x00", 4, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
	std::shared_ptr<RoomNode> Room = NULL;
	if (Client->RoomID != 0)
	{
		Room = GetRoom(Client->RoomID);
	}
	if (Room)
	{
		if (Room->CurrentPlayerNum == 1)
		{//单人退出直接到房间
			if (Room->ChatRoomType == 6)
			{
				//进入3d房间
				ResponseCreateRoom(Client, Room, 6);
				RequestEnter3DRoom(Client);
			}
			if (Room->Timer)
			{
				if (DeleteTimerQueueTimer(TimerQueue, Room->Timer, INVALID_HANDLE_VALUE))
				{
					Room->Timer = NULL;
				}
			}
			Room->startStatus = 1;
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
	LeaveBorderRoom(Client, 0);
	Client->TaskID = 0;
	Client->IsFinish = FALSE;
	Client->IsReady == FALSE;
}


void NotifyUpdateKartSkillStoneInfo3(std::shared_ptr<ClientNode> Client, int KartID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	UINT skillstoneid[5] = { 21988,21982,21994,22018,21988 };
	//UINT skillstoneid[5] = { 21980,21986,22016,21992,22004 };
	Write32(p, Client->Uin); //Uin

	//KartStoneInfo
	BYTE* pKartStoneInfo = p;
	Write16(pKartStoneInfo, 0); //len
	Write32(pKartStoneInfo, KartID); //KartID
	Write32(pKartStoneInfo, 5); //StoneGrooveNum
	for (int i = 0; i < 5; i++)
	{
		BYTE* pStoneGrooveInfo = pKartStoneInfo;
		Write16(pStoneGrooveInfo, 0); //len

		Write32(pStoneGrooveInfo, 0); //StoneUseOccaType
		Write32(pStoneGrooveInfo, skillstoneid[i]); //SkillStoneID

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


void NotifyUpdateKartSkillStoneInfo2(std::shared_ptr<ClientNode> Client, int KartID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	UINT skillstoneid[2] = { 21980,21986 };
	//UINT skillstoneid[5] = { 21980,21986,22016,21992,22004 };
	Write32(p, Client->Uin); //Uin

	//KartStoneInfo
	BYTE* pKartStoneInfo = p;
	Write16(pKartStoneInfo, 0); //len
	Write32(pKartStoneInfo, KartID); //KartID
	Write32(pKartStoneInfo, 2); //StoneGrooveNum
	for (int i = 0; i < 2; i++)
	{
		BYTE* pStoneGrooveInfo = pKartStoneInfo;
		Write16(pStoneGrooveInfo, 0); //len

		Write32(pStoneGrooveInfo, 0); //StoneUseOccaType
		Write32(pStoneGrooveInfo, skillstoneid[i]); //SkillStoneID

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

void NotifyClientUpdatePlayerID(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	Write16(p, Room->CurrentPlayerNum); //PlayerNum
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];

		if (RoomClient)
		{
			{ //PlayerIDInfo
				BYTE* pPlayerIDInfo = p;
				Write16(pPlayerIDInfo, 0); //len

				Write32(pPlayerIDInfo, RoomClient->Uin); //Uin
				Write16(pPlayerIDInfo, RoomClient->ConnID); //PlayerID

				len = pPlayerIDInfo - p;
				Set16(p, (WORD)len);
				p += len;
			}
		}
	}

	Write32(p, 0);
	Write32(p, 0);
	Write32(p, 0);
	Write16(p, 0);
	len = p - buf;
	SendToClient(Client, 24390, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void WINAPI OnPrepareTest(std::shared_ptr<RoomNode> Room)
{
	UINT time = 0;
	char PlayerNum = Room->CurrentPlayerNum;
	std::shared_ptr<ClientNode> RoomClient;
	BOOL AllReady = TRUE;

	while (time < SyncTime)
	{
		for (char i = 0; i < 6; i++)
		{
			RoomClient = Room->Player[i];
			if (RoomClient)
			{
				if (RoomClient->IsReady == FALSE)
				{
					AllReady = FALSE;
				}
			}
		}
		if (AllReady)
		{
			break;//所有人准备好就马上跳出循环否则继续等待
		}
		AllReady = TRUE;
		time += 1000;
		Sleep(1000);//每休眠一秒进行一次验证 超时自动跳出
	}


	for (char i = 0; i < 6; i++)
	{
		if (PlayerNum == 0)
		{
			return;//防止房间删除后没人了还继续执行后续代码
		}
		RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient->IsReady == TRUE)
			{
				//NotifyClientUpdatePlayerID(RoomClient, Room);

				//等待所有人准备 同步跳舞动作 跳舞动作必须一样 否则卡起步 再发送性能数据
				if (mode28346 == 2) {
					NotifyKartPhysParam(RoomClient, Room->Player);
				}
				else {
					NotifyEncryptedMsg(RoomClient);
					//NotifyKartPhysParam(RoomClient, Room->Player);
				}
				//NotifyKartPhysParam(RoomClient, Room->Player);

			}
			else
			{
				if (mode28346 == 2) {
					NotifyKartPhysParam(RoomClient, Room->Player);
				}
				else {
					NotifyEncryptedMsg(RoomClient);
				}
				//NotifyKartPhysParam(RoomClient, Room->Player);

				SendToClient(RoomClient, 514, (const BYTE*)"\x00\x00\x00\x00", 4, RoomClient->GameID, FE_GAMELOGIC, RoomClient->ConnID, Notify);
				if (Room->ChatRoomType == 6)
				{
					//异常退出3d房间
					ResponseCreateRoom(RoomClient, Room, 0);
				}
				//LeaveOutdoor(RoomClient);
				LeaveRoom(RoomClient, 0);//异常退出房间
				PlayerNum--;
			}
		}
	}

	if (Room->isSkip == 1) {
		CreateRoomTimer(Room, SkipDancetime, OnBegin);//游戏开始跳舞的时间 0直接跳过
	}
	else {
		CreateRoomTimer(Room, DanceTime, OnBegin);//游戏开始跳舞的时间 0直接跳过
	}


}
std::shared_ptr<RoomNode> GetBorderRoom(long RoomID)
{
	std::shared_ptr<RoomNode> Room = NULL;
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	auto iter = BorderRooms.find(RoomID);
	if (iter != BorderRooms.end())
	{
		Room = iter->second;
	}
	else
	{

	}
	return Room;


}
std::shared_ptr<RoomNode> NewBorderRoom()
{
	std::shared_ptr<RoomNode> Room = std::make_shared<RoomNode>();
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	SeqRoomID++;
	Room->ID = SeqRoomID;
	BorderRooms[Room->ID] = Room;//多线程单读map可以不锁 同时读写map一定要锁 
	return Room;
}


std::shared_ptr<RoomNode> NewRoom()
{
	std::shared_ptr<RoomNode> Room = std::make_shared<RoomNode>();
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	SeqRoomID++;
	Room->ID = SeqRoomID;
	Room->startStatus = 1;
	Room->BaseGameMode = 0;
	Room->SubGameMode = 0;
	Rooms[Room->ID] = Room;//多线程单读map可以不锁 同时读写map一定要锁 

	return Room;
}


std::shared_ptr<RoomNode> GetRoom(long RoomID)
{
	std::shared_ptr<RoomNode> Room = NULL;
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	//DWORD dwStart = GetTickCount64(); //取windows启动到现在的流逝时间(毫秒)
	auto iter = Rooms.find(RoomID);
	if (iter != Rooms.end())
	{
		Room = iter->second;
	}
	else
	{
		//Room = iter->second;
		//printf("无效的房间ID:%d\n", RoomID);//产生房间错误如边境 3d休闲 竞速等会产生空指针
		//最好都提前执行LeaveRoom
	}
	//DWORD dwUsed = GetTickCount64() - dwStart; //计算该函数所消耗的时间
	//printf("Room Search lost:%d ms\n", dwUsed);
	return Room;


}
void PrintRoom()
{
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	std::map<long, std::shared_ptr<RoomNode>>::iterator iter;
	iter = Rooms.begin();
	while (iter != Rooms.end())
	{
		std::shared_ptr<RoomNode> Room = iter->second;
		printf("RoomID:%d, ", Room->ID);
		printf("BaseGameMode:%d, ", Room->BaseGameMode);
		printf("SubGameMode:%d, ", Room->SubGameMode);
		printf("ServerID:%d, ", Room->ServerID);
		printf("RoomOwnerID:%d, ", Room->RoomOwnerID);
		printf("Name:%s\n", Room->Name);
		printf("Flag:%d, ", Room->Flag);
		printf("Password:%s\n", Room->Password);
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				printf("\tUIN:%d, SeatID:%d\n", RoomClient->Uin, i);
			}
		}
		iter++;
	}
}

VOID WINAPI OnTimer(PVOID p, BOOLEAN b)
{
	try {
		std::shared_ptr<RoomNode>* RoomPtr = reinterpret_cast<std::shared_ptr<RoomNode>*>(p);
		if (RoomPtr)
		{
			auto& SharedRoom = *RoomPtr;
			if (SharedRoom)
			{
				std::lock_guard<std::shared_mutex> lock(SharedRoom->mutex);
				if (SharedRoom->Timer)
				{
					DeleteTimerQueueTimer(TimerQueue, SharedRoom->Timer, NULL);
					SharedRoom->Timer = NULL;
					SharedRoom->Callback(SharedRoom);
				}
			}
			delete RoomPtr;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception in OnTimer: " << e.what() << std::endl;
	}
	catch (...)
	{
		printf("OnTimer Exception!!!\n");
	}
}

void CreateRoomTimer(std::shared_ptr<RoomNode> Room, int Timeout, RoomOnTimer Callback) {
	if (Room == nullptr || TimerQueue == nullptr) {
		std::wcout << L"错误：Room 或 TimerQueue 为 null\n";
		return;
	}

	Room->Callback = Callback;
	auto RoomPtr = new std::shared_ptr<RoomNode>(Room);
	BOOL result = CreateTimerQueueTimer(
		&Room->Timer,           // Timer handle
		TimerQueue,             // Timer queue
		OnTimer,                // Callback function
		RoomPtr,                   // Parameter passed to callback
		Timeout,                // Initial wait time (ms)
		0,                      // No periodicity (one-time timer)
		WT_EXECUTEINTIMERTHREAD // Execute in timer thread
	);

	if (!result) {
		std::wcout << L"错误：创建定时器失败 (错误代码: " << GetLastError() << L")\n";
		Room->Timer = NULL; // 确保失败时将计时器设为 NULL
	}
}
//房间排序
bool CompareRoomNode(const std::shared_ptr<RoomNode> room1, const std::shared_ptr<RoomNode> room2)
{
	// 若room1的Callback为空，room2的Callback不为空，则room1排在room2前面
	if (room1->Callback == nullptr && room2->Callback != nullptr)
		return true;
	// 若room1的Callback不为空，room2的Callback为空，则room1排在room2后面
	if (room1->Callback != nullptr && room2->Callback == nullptr)
		return false;
	// 若room1的BaseGameMode为19，而room2的BaseGameMode不为19，则room1排在room2后面(紫钻练习场房间排最后)
	if (room1->BaseGameMode == 19 && room2->BaseGameMode != 19)
		return false;
	// 若room1的BaseGameMode不为19，而room2的BaseGameMode为19，则room1排在room2前面(紫钻练习场房间排最后)
	if (room1->BaseGameMode != 19 && room2->BaseGameMode == 19)
		return true;
	// 当两个RoomNode对象的Callback都为空时，按照ID从大到小排序
	return room1->ID > room2->ID;
}

void RequestGetRoomList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	USHORT StartRoomIdx = Read16(Body);//索引0 6 8
	USHORT GetRoomNum = Read16(Body);//0x18 一次性读取24个房间
	USHORT BaseGameMode = Read16(Body);
	USHORT SubGameMode = Read16(Body);
	BYTE FBLev = Read8(Body);
	BYTE FBSeason = Read8(Body);
	BYTE RoomFlag = Read8(Body);
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	std::map<long, std::shared_ptr<RoomNode>>::iterator iter = Rooms.begin();;
	std::vector<std::shared_ptr<RoomNode>> AllRooms(60);
	int RoomNum = 0;
	int index = 0;
	while (iter != Rooms.end())
	{
		if (RoomNum == GetRoomNum)
		{
			//hasMore = TRUE;
			break;
		}
		if (index < StartRoomIdx)
		{
			index++;
			iter++;
			continue;
		}
		std::shared_ptr<RoomNode> Room = iter->second;
		if (Room->BaseGameMode == 4)
		{
			index++;
			iter++;
			continue;
		}

		//滑板
		if (Client->ServerID == 900 && Room->ServerID != 900)
		{
			index++;
			iter++;
			continue;
		}
		if (Client->ServerID != 900 && Room->ServerID == 900)
		{
			index++;
			iter++;
			continue;
		}



		//轮滑
		if (Client->ServerID == 1000 && Room->ServerID != 1000)
		{
			index++;
			iter++;
			continue;
		}
		if (Client->ServerID != 1000 && Room->ServerID == 1000)
		{
			index++;
			iter++;
			continue;
		}

		//if (Room->ServerID != Client->ServerID)
		//{ // 只查询当前频道房间
		//	index++;
		//	iter++;
		//	continue;
		//}

		/*else if (Client->ServerID != 256 && Room->BaseGameMode == 18)
		{
			index++;
			iter++;
			continue;
		}
		else if (Client->ServerID == 256 && Room->BaseGameMode != 18)
		{
			index++;
			iter++;
			continue;
		}*/
		AllRooms[RoomNum] = Room;
		RoomNum++;
		index++;
		iter++;
	}

	// 对AllRooms进行排序
	std::sort(AllRooms.begin(), AllRooms.begin() + RoomNum, CompareRoomNode);
	ResponseGetRoomList(Client, BaseGameMode, SubGameMode, AllRooms, RoomNum, StartRoomIdx, RoomFlag);
}
void ResponseGetRoomList(std::shared_ptr<ClientNode> Client, USHORT BaseGameMode, USHORT SubGameMode, std::vector<std::shared_ptr<RoomNode>>& AllRooms, int RoomNum, int StartRoomIdx, BYTE RoomFlag)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	if (Uinew) {
		std::srand(static_cast<unsigned int>(std::time(0)));
		int randomChoice = std::rand() % 2; // 0 or 1
		if (randomChoice == 0) {
			WriteHex(p, "0000063500000018006000290001000101060100000167008C31302E32362036B1B6BFAAC6F4A3A1000000000000000000006E043902D1FFFF00040001000000010002261E0000000000000000000000000000000000000000000000000000000000000367151C76006000CB000100010106010000010C018C31302E32362036B1B6BFAAC6F4A3A1000000000000000000006E04DB02D1FFFF00040000000000020007CFB300000000000000000000000000000000000000000000000000000000000000000000000060013E0001000101060100000001010031302E323620CBCD41B3B5CCFDB7E7D5DF000000000000000064B67E0294FFFF000400010000000200000000000000000000000000000000000000000000000000000000000000000000116715272B0060013F0001000101060100000000000031302E323620CBCD41B3B5CCFDB7E7D5DF00000000000000006E054F02D1FFFF0004000100000002000000000000000000000000000000000000000000000000000000000000000000000D671517180060008C0001000101060100000000000031302E323620CBCD36383838B5E3C8AF0000000000000000007111DC02E5FFFF00040001000000080006CAC10000000000000000000000000000000000000000000000000000000000000A6714D4DE006000B80001000101060100000000000031302E323620CBCD36383838B5E3C8AF0000000000000000006EEF2802D7FFFF00040001000000020000000000000000000000000000000000000000000000000000000000000000000007671511950061006B00010001010601000000C2000031302E32362036B1B6BFAAC6F4A3A1000000000000000000007111BB02E5FFFF00040000000000020000000000011B000000000000000000000000000000000000000000000000000000000567152652006000BC0001000101060100000000000031302E32362036B1B6BFAAC6F4A3A10000000000000000000071120C02E5FFFF00040001000000020007CA9F0000000000000000000000000000000000000000000000000000000000000467152A210061018F0001000101060100000069000031302E32362036B1B6BFAAC6F4A3A1000000000000000000006E059F02D1FFFF00040000000000020005811D00011B00000000000000000000000000000000000000000000000000000000046715249D006001310001000101060100000008010031302E32362036B1B6BFAAC6F4A3A1000000000000000000006EC89102D6FFFF00040000000000010007F9850000000000000000000000000000000000000000000000000000000000000367152920006001390001000101060100000100000031302E32362036B1B6BFAAC6F4A3A10000000000000000000064B6790294FFFF00040000000000020007F8BC000000000000000000000000000000000000000000000000000000000000016715294B0061005A0001000101060100000113040031302E323620CBCD41B3B5CCFDB7E7D5DF00000000000000006E046A02D1FFFF00040001000000020000000000011B000000006E046A0000000000000000000000000000000000000000000167152834006000A50001000101060100000000000031302E32362036B1B6BFAAC6F4A3A10000000000000000000071AE3502E9FFFF00040000000000010001D5260000000000000000000000000000000000000000000000000000000000000167152520006001A400010001010601000001C5000031302E323620CBCD41B3B5CCFDB7E7D5DF000000000000000064B6E40294FFFF00040000000000010004E2660000000000000000000000000000000000000000000000000000000000000167151315006001110001000101060100000000000031302E323020CBCD34303030B5E3C8AF00000000000000000064B6510294FFFF0004000000000001000224EA0000000000000000000000000000000000000000000000000000000000000167150A81006000D5000100010106010000015A040031302E323620CBCD34383838CFFBB7D1C8AF000000000000006E04E502D1FFFF00040001000000020002254D0000000000006E04E500000000000000000000000000000000000000000001671509E10061009100010001010601000001C2000031302E323620CBCD36383838B5E3C8AF0000000000000000006EEF0102D7FFFF000400010000000800094F4A00011B0000000000000000000000000000000000000000000000000000000001671502500061015A00010001010601000001C5000031302E323020CBCD41B3B5B7E3C1D60000000000000000000064B69A0294FFFF00040000000000020001D52600011B00000000000000000000000000000000000000000000000000000000016714E269006100BF00010001010401000000000000CFE0C7D70000000000000000000000000000000000000000006467DF0292FFFF000400000000000100057F5900011B000000000000000000000000000000000000000000000000000000010000000000006000E10001000101060100000068040031302E323620CBCD34383838CFFBB7D1C8AF000000000000006E04F102D1FFFF0004000100000001000000000000000000006E04F10000000000000000000000000000000000000000000000000000006100160001000101060100000100010031302E323620CBCD41B3B5CCFDB7E7D5DF00000000000000006EC77602D6FFFF000400000000000200050A2A00011B0000000000000000000000000000000000000000000000000000000000000000000060000A00010001010601000001C7000031302E323620CBCD36383838B5E3C8AF0000000000000000006EEE7A02D7FFFF00040000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000060017F000100010106010000010A008031302E323620CBCD36383838B5E3C8AF00000000000000000064418F0291FFFF00040000000000020004E26F0000000000000000000000000000000000000000000000000000000000000000000000006000A800010001010601000001C7010031302E323620CBCD34383838CFFBB7D1C8AF000000000000006467C80292FFFF00040000000000020000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002");

		}
		else {
			WriteHex(p, "0000062D00000018006100C800010001040601000001C7040031302E323620CBCD36383838B5E3C8AF0000000000000000006E04D802D1FFFF0004000100000002FFFFFFFF00011B000000006E04D800000000000000000000000000000000000000000000000000000061006B00010001020601000000C2000031302E32362036B1B6BFAAC6F4A3A1000000000000000000007111BB02E5FFFF000400000000000A0000000000011B000000000000000000000000000000000000000000000000000000000567152652006001A400010001020601000001C5000031302E323620CBCD41B3B5CCFDB7E7D5DF000000000000000064B6E40294FFFF00040000000000090004E2660000000000000000000000000000000000000000000000000000000000000167151315006000290001000101060100000167008C31302E32362036B1B6BFAAC6F4A3A1000000000000000000006E043902D1FFFF00040001000000010002261E0000000000000000000000000000000000000000000000000000000000000367151C76006000CB000100010106010000010C018C31302E32362036B1B6BFAAC6F4A3A1000000000000000000006E04DB02D1FFFF00040000000000020007CFB300000000000000000000000000000000000000000000000000000000000000000000000060013E0001000101060100000001010031302E323620CBCD41B3B5CCFDB7E7D5DF000000000000000064B67E0294FFFF000400010000000200000000000000000000000000000000000000000000000000000000000000000000116715272B0060013F0001000101060100000000000031302E323620CBCD41B3B5CCFDB7E7D5DF00000000000000006E054F02D1FFFF0004000100000002000000000000000000000000000000000000000000000000000000000000000000000D671517180060008C0001000101060100000000000031302E323620CBCD36383838B5E3C8AF0000000000000000007111DC02E5FFFF00040001000000080006CAC10000000000000000000000000000000000000000000000000000000000000A6714D4DE006000B80001000101060100000000000031302E323620CBCD36383838B5E3C8AF0000000000000000006EEF2802D7FFFF0004000100000002000000000000000000000000000000000000000000000000000000000000000000000767151195006000BC0001000101060100000000000031302E32362036B1B6BFAAC6F4A3A10000000000000000000071120C02E5FFFF00040001000000020007CA9F0000000000000000000000000000000000000000000000000000000000000467152A210061018F0001000101060100000069000031302E32362036B1B6BFAAC6F4A3A1000000000000000000006E059F02D1FFFF00040000000000020005811D00011B00000000000000000000000000000000000000000000000000000000046715249D006001990001000101060100000000000031302E323620CBCD36383838B5E3C8AF0000000000000000006468B90292FFFF0004000000000002000582A4000000000000000000000000000000000000000000000000000000000000036715292A006001310001000101060100000008010031302E32362036B1B6BFAAC6F4A3A1000000000000000000006EC89102D6FFFF00040000000000010007F9850000000000000000000000000000000000000000000000000000000000000367152920006000A50001000101060100000000000031302E32362036B1B6BFAAC6F4A3A10000000000000000000071AE3502E9FFFF00040000000000010001D5260000000000000000000000000000000000000000000000000000000000000167152520006001110001000101060100000000000031302E323020CBCD34303030B5E3C8AF00000000000000000064B6510294FFFF0004000000000001000224EA0000000000000000000000000000000000000000000000000000000000000167150A81006000D5000100010106010000015A040031302E323620CBCD34383838CFFBB7D1C8AF000000000000006E04E502D1FFFF00040001000000020002254D0000000000006E04E500000000000000000000000000000000000000000001671509E1006100AB00010001010601000000DB010031302E32362036B1B6BFAAC6F4A3A1000000000000000000006467CB0292019000040000000000040006BCB100011B0000000000000000000000000000000000000000000000000000000001671503D00061009100010001010601000001C2000031302E323620CBCD36383838B5E3C8AF0000000000000000006EEF0102D7FFFF000400010000000800094F4A00011B0000000000000000000000000000000000000000000000000000000001671502500061015A00010001010601000001C5000031302E323020CBCD41B3B5B7E3C1D60000000000000000000064B69A0294FFFF00040000000000020001D52600011B00000000000000000000000000000000000000000000000000000000016714E269006100BF00010001010401000000000000CFE0C7D70000000000000000000000000000000000000000006467DF0292FFFF000400000000000100057F5900011B000000000000000000000000000000000000000000000000000000010000000000006000E10001000101060100000068040031302E323620CBCD34383838CFFBB7D1C8AF000000000000006E04F102D1FFFF0004000100000001000000000000000000006E04F10000000000000000000000000000000000000000000000000000006100160001000101060100000100010031302E323620CBCD41B3B5CCFDB7E7D5DF00000000000000006EC77602D6FFFF000400000000000200050A2A00011B0000000000000000000000000000000000000000000000000000000000000000000060000A00010001010601000001C7000031302E323620CBCD36383838B5E3C8AF0000000000000000006EEE7A02D7FFFF00040000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000060017F000100010106010000010A008031302E323620CBCD36383838B5E3C8AF00000000000000000064418F0291FFFF00040000000000020004E26F00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002");
		}
		len = p - buf.data();
		SendToClient(Client, 102, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Response);
		return;
	}
	//BYTE* p = buf;
	
	Write16(p, 0); //ResultID

	Write16(p, 0); //CurTotalRoomNum
	Write16(p, StartRoomIdx); //StartIdx

	Write16(p, RoomNum); //RoomNum
	{
		for (int i = 0; i < RoomNum; i++)
		{
			std::shared_ptr<RoomNode> Room = AllRooms[i];
			BYTE* pRooms = p;
			Write16(pRooms, 0); //len

			Write16(pRooms, Room->ID); //RoomID
			Write16(pRooms, Room->BaseGameMode); //BaseGameMode
			Write16(pRooms, Room->SubGameMode); //SubGameMode
			Write8(pRooms, Room->CurrentPlayerNum); //CurrentPlayerNum
			Write8(pRooms, Room->SeatNum); //TotalSeatNum
			if (Room->Callback == NULL && Room->SeatNum > 1)
			{
				Write8(pRooms, 1); //Status 房间状态 1为可进?0为不可进? 可以通过callback判断
			}
			else
				Write8(pRooms, 0);
			Write32(pRooms, Room->MapID); //MapID
#ifdef ZingSpeed
			Write8(pRooms, (BYTE)Room->Flag); //Flag
#else
			Write16(pRooms, Room->Flag); //Flag
#endif

			memcpy(pRooms, Room->Name, MaxRoomName); //RoomName
			pRooms += MaxRoomName;

			Write8(pRooms, 0); //HasFBInfo


#ifndef ZingSpeed
			Write8(pRooms, 0); //SpeFlag
			Write32(pRooms, 0); //RoomNo
			Write16(pRooms, 0); //SvrId
			//if(Room->ChatRoomType==0x6)
				//Write16(pRooms, 0x190); //SceneID

			Write16(pRooms, Room->SceneID); //SceneID
			Write8(pRooms, 0); //CurrentOBNum
			Write8(pRooms, 0); //TotalOBNum
			Write8(pRooms, 0); //SpecialActivityStatus
			Write8(pRooms, Room->AllowAutoStart); //AllowAutoStart
			Write32(pRooms, 0); //Gender
			Write32(pRooms, 0); //LocaleCode
			Write8(pRooms, 0); //TalkRoomBGID
			Write8(pRooms, 0); //SpecialActivityIDNum
			Write8(pRooms, 0); //SpecialActivityInfoNum

#endif

			len = pRooms - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}


	Write8(p, 0); //ReasonLen

	Write16(p, BaseGameMode); //BaseGameMode
	Write16(p, SubGameMode); //SubGameMode
	Write8(p, 0); //FBAvailableLev
	Write8(p, 0); //FBLev
	Write8(p, 0); //FBSeason

	Write16(p, 0); //ChannelStatus
	Write8(p, RoomFlag); //RoomFlag

	len = p - buf.data();
	SendToClient(Client, 102, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Response);
}


std::string generateRandomNumber() {
	std::string randomNumber;
	for (int i = 0; i < 10; ++i) {
		randomNumber += std::to_string(std::rand() % 10);
	}
	return randomNumber;
}


void RequestCreateRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	USHORT BaseGameMode = Read16(Body);
	//if (BaseGameMode == 1 || BaseGameMode == 2 || BaseGameMode == 19)
	//{
	//}
	//else
	//{
	//	ResponseStartGame(Client, "暂未开放");
	//	return;
	//}

	USHORT SubGameMode = Read16(Body);
	if (BaseGameMode == 4)
	{
		SubGameMode = 1;//舞蹈固定1
	}
#ifdef ZingSpeed
	char* RoomName = (char*)Body;
	Body += MaxRoomName;

	BYTE SeatNum = Read8(Body);
	BYTE Flag = Read8(Body);
#else
	USHORT Flag = Read16(Body);

	char* RoomName = (char*)Body;
	/*for (int i = 0; i < 22; i++)
	{
		if (Body[i] != 0)
		{
			break;
		}
		else if (Body[21] == 0&&i==21)
		{
			//房间创建失败 不能空名字
			return;
		}
	}*/
	Body += MaxRoomName;

	BYTE SeatNum = Read8(Body);
#endif

	char Password[16] = { 0 };
	if (Flag)
	{
		memcpy(Password, Body, 16);
	}
	Body += 16;

	BYTE IsNewerOpen = Read8(Body);
	BYTE FBSeason = Read8(Body);
	BYTE Contex = Read8(Body);
	BYTE FBLev = Read8(Body);

#ifndef ZingSpeed
	BYTE SpeFlag = Read8(Body);

	BYTE MoreInfoNum = Read8(Body);
	for (BYTE i = 0; i < MoreInfoNum; i++)
	{
		//m_astMoreInfos[].m_iTaskID
		Body += Get16(Body);
	}

	USHORT CreateRoomType = Read16(Body);
	USHORT ServerMatchType = Read16(Body);
	USHORT TeamMatchType = Read16(Body);
	BYTE ChatRoomType = Read8(Body);
	USHORT SceneID = Read16(Body);
	BYTE PresetOBNum = Read8(Body);
	BYTE AllowAutoStart = Read8(Body);
	BYTE TalkRoomBGID = Read8(Body);

	BYTE ValueNum = Read8(Body);
	for (BYTE i = 0; i < ValueNum; i++)
	{
		//m_aiEnterRoomValue[]
		Read32(Body);
	}
#endif

	//auto ServerLogger = spdlog::get("SpeedServer_logger");
	////ServerLogger->info("[玩家创建房间]");
	//ServerLogger->info("[玩家创建房间][Uin:{:d}]]", Client->Uin);
	//ServerLogger->flush();

	std::shared_ptr<RoomNode> Room = NewRoom();
	if (!Room)
	{
		return;
	}
	Room->BaseGameMode = BaseGameMode;
	Room->SubGameMode = SubGameMode;
	Room->GVoiceRoomName = 1 + "-" + std::to_string(Client->Uin) + "-" + generateRandomNumber();
	if (RoomName && Room->Name) {
		// 使用 strnncpy_s 进行安全的字符串复制
		strncpy_s(Room->Name, MaxRoomName, RoomName, _TRUNCATE);
		Room->Name[MaxRoomName - 1] = '\0';  // 手动添加字符串结尾的 null 字符
	}


	Room->ServerID = Client->ServerID;
	Room->RoomOwnerID = Client->ConnID;
	Client->RoomID = Room->ID;
	Room->SeatNum = SeatNum;
	Room->CurrentPlayerNum = 1;
	Room->Flag = Flag;
	memcpy(Room->Password, Password, 16);
	Room->ChatRoomType = ChatRoomType;
	Room->AllowAutoStart = AllowAutoStart;
	for (char i = 0; i < 6; i++)
	{
		Room->Player[i] = nullptr;
		Room->SeatState[i] = 1;
	}

	Room->Player[0] = Client;
	Client->ReadyState = 0;//0 是准备 1是取消准备
	Client->ChatRoomID = -1;

	if (BaseGameMode == 0x2e)//龙晶大闯关
	{
		Client->MapID = Room->MapID = 432;
		const char* buf1 = "\x00\x00\x00\x01\x00\x2e\x00\x01\x06\x00\xb4\xf3\xbc\xd2\xd2\xbb\xc6\xf0\xcc\xf4\xd5\xbd\xbc\xab\xcf\xde\xa3\xa1\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x64\x70\x64\x1c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x08\x47\x7e\x1d\x00\x00\x00\x00\x01\x00\x11\x00\x00\x00\x00\x00\x04\x01\x01\x00\x08\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x0f\x24\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x0f\x24\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x17\x31\x2d\x31\x38\x37\x32\x34\x37\x33\x37\x36\x37\x2d\x31\x36\x38\x35\x30\x38\x37\x32\x36\x30\x00\x00\x00\x00\x00\x00\x00\x00\x00\x72\x69";
		SendToClient(Client, 103, (BYTE*)buf1, 207, Client->ServerID, FE_GAMESVRD, Client->ConnID, Response);
		ResponseStartGame(Client, "当前模式为幻域大闯关\n发送/C01我要玩龙晶/C00即可玩龙晶大闯关\n发送/C01我要玩幻域/C00即可玩幻域大闯关\n发送/C01我要玩冲刺/C00即可玩冲刺大闯关");
		return;
	}
	else
	{
		ResponseCreateRoom(Client, Room, ChatRoomType);
		//设置观战
		SendToClient(Client, 134, (BYTE*)"\x00\x00\x00\x00", 4, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		if (ChatRoomType == 0x6)
		{
			RequestEnter3DRoom(Client);
		}
	}
}
void ResponseCreateRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, BYTE ChatRoomType)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	
	/*WriteHex(p, "000000E600010001000031312E3220CBCD41B3B5CFF7B7E7000000000000000006000000000000000000671F63CE00000000000000000000000000000000000A00008F489600000000000000000000000004010000000000000000000000000017312D313732383531303534362D3137333031313034313400000000000000000001000A000000000000000A00");
	len = p - buf.data();
	SendToClient(Client, 103, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Response);
	return;*/

	Write16(p, 0); //ResultID

	Write16(p, Room->ID); //RoomID
	Write16(p, Room->BaseGameMode); //BaseGameMode
	Write16(p, Room->SubGameMode); //SubGameMode
#ifdef ZingSpeed
	memcpy(p, Room->Name, MaxRoomName); //RoomName
	p += MaxRoomName;

	Write8(p, Room->SeatNum); //SeatNum
	Write8(p, (BYTE)Room->Flag); //Flag
#else
	Write16(p, 0x18c); //Flag 皇族 情侣贵族房间标志

	memcpy(p, Room->Name, MaxRoomName); //RoomName
	p += MaxRoomName;

	Write8(p, Room->SeatNum); //SeatNum
#endif // ZingSpeed


	Write8(p, 0); //SeatID
	Write8(p, 0); //TeamID

	Write8(p, 0); //ReasonLen


	Write8(p, 0); //LoverRaceOrPro

	Write32(p, Room->MapID); //MapID
	Write8(p, 0); //DebutOrX5
	Write32(p, (DWORD)time(nullptr)); //ServerTime
	Write32(p, 0); //PreKnowMapID
	Write32(p, 0); //PreKnownMusicID

	Write8(p, 0); //HasFBInfo
	/*
m_astFBInfo[%d].m_bAvailableLev
m_astFBInfo[%d].m_bLev
m_astFBInfo[%d].m_bSubLev
m_astFBInfo[%d].m_iBossID
m_astFBInfo[%d].m_bFBSeason
	*/

	Write32(p, 0); //HasFBInfoGuildScoreInNextGame
	Write8(p, 0); //HasFBInfoContex
	Write16(p, 0); //GuildScoreAlreadyAward
	Write16(p, 0); //GuildScoreTotalAward
#ifndef ZingSpeed
	Write8(p, 0xa); //HasFBInfoSpeFlag
	Write32(p, Room->ID); //RoomNo

	Write8(p, 0); //MoreInfoNum
	/*
m_astMoreInfos[%d].m_iTaskID
	*/


	Write16(p, 0); //RaceCardNums
	/*
m_astRaceCardAddValueInfo[%d].m_uiRaceCardID
m_astRaceCardAddValueInfo[%d].m_uiAddValue
	*/

	Write16(p, 0); //CreateRoomType
	Write16(p, 0); //ServerMatchType
	Write16(p, 0); //TeamMatchType
	Write8(p, ChatRoomType); //ChatRoomType 3d房间是6
	if (ChatRoomType == 6)
	{
		Write16(p, 0x190);
	}
	else
	{
		Write16(p, 0); //SceneID 0x190
	}

	Write8(p, 0); //PresetOBNum
	Write8(p, 0); //AllowAutoStart
	Write8(p, 0); //TalkRoomBGID
	Write16(p, 0); //Role

	Write8(p, 0); //ParaNum
	//ParaList[]

	Write8(p, 0); //ValueNum
	//EnterRoomValue[]

	Write32(p, 0); //PlayerEquipKartID
	Write32(p, 0); //RankedMatchTestDriveKartID
	if (GVoice|| version > 1.3) {
		WriteString_MC(p, Room->GVoiceRoomName.c_str()); //GVoiceRoomNameLen
	}
	else {
		Write8(p, 0); 
	}
	
	Write8(p, 0); //GVoiceReasonLen
	Write8(p, 0); //TDCExcluItemNum
	//TDCExclusiveItemID[]

	Write8(p, 0); //TestKartGameType
	Write32(p, 0); //GameTestKartID

	Write8(p, 0); //HasStarsFightingInfo
	/*
m_stStarsFightingInfo[%d].m_iStar
m_stStarsFightingInfo[%d].m_iStarVal
m_stStarsFightingInfo[%d].m_iStarsFightingStarLevel
	*/

	Write8(p, 0); //HasYuLeJiaNianHuaInfo
	/*
m_stYuLeJiaNianHuaInfo[%d].m_uiMode
m_stYuLeJiaNianHuaInfo[%d].m_uiScore
m_stYuLeJiaNianHuaInfo[%d].m_uchRankRangNum
m_stYuLeJiaNianHuaInfo[%d].m_auiRankRange[%d]
m_stYuLeJiaNianHuaInfo[%d].m_uchRankScoreNum
m_stYuLeJiaNianHuaInfo[%d].m_auiRankScore[%d]
m_stYuLeJiaNianHuaInfo[%d].m_uchInRecommend
m_stYuLeJiaNianHuaInfo[%d].m_uchInDoubleScore
	*/
#endif
	//WriteHex(p, "01000A000000000000000A00");
	len = p - buf.data();
	SendToClient(Client, 103, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Response);
}


void RequestEnterInvitedRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	if (Room->Callback != NULL || Room->CurrentPlayerNum == 6 || Room->SeatNum == Room->CurrentPlayerNum)
	{
		return;
	}
	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient == 0 && Room->SeatState[i] == 1)
		{
			Client->SeatID = i;
			Client->RoomID = Room->ID;
			Client->ReadyState = 1;
			Room->Player[i] = Client;
			Room->CurrentPlayerNum++;
			Client->ChatRoomID = -1;

			if (Room->ChatRoomType == 6)
			{
				//进入3d房间
				ResponseCreateRoom(Client, Room, 6);
				RequestEnter3DRoom(Client);
				NotifyChangeRoomOwner(Client, Room);
			}
			else
			{
				ResponseEnterRoom(Client, Room, i, 0, 0);//回复进入房间让自己看到其他人的信息
			}

			//通知其它玩家
			for (char i2 = 0; i2 < 6; i2++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
				if (RoomClient && RoomClient != Client)
				{

					NotifyEnterRoom(RoomClient, Room, Client, i);
					NotifyEnterRoom(Client, Room, RoomClient, i2);
				}
			}
			for (char i3 = 0; i3 < 6; i3++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i3];
				if (RoomClient && RoomClient != Client)
				{
					NotifyChangeReadyState(RoomClient, Client->Uin, Client->ReadyState);
					NotifyChangeReadyState(Client, RoomClient->Uin, RoomClient->ReadyState);
				}
			}
			for (char i4 = 0; i4 < 6; i4++)
			{
				if (Room->SeatState[i4] == 0)
				{
					NotifyChangeSeatState(Client, i4, 0);//座位状态变化
				}
			}

			if (obs == 1) {
				if (Room->ChatRoomType != 6) {
					//观战
					for (char i5 = 0; i5 < 6; i5++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[i5];
						if (RoomClient && RoomClient != Client)
						{
							if (Client->OBState == 1) {
								NotifyChangeOBState(RoomClient, Client->OBState, Client->Uin);
							}
							if (RoomClient->OBState == 1) {
								NotifyChangeOBState(Client, RoomClient->OBState, RoomClient->Uin);
							}
						}
					}
				}
			}
			//通知其它玩家

			break;
		}
	}
}

void RequestEnterRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	// 取消当前匹配和离开任何已加入的房间
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);
	//printf("RequestEnterRoom\n");
	//请求进入房间 
	//自行添加验证密码和游戏过程能否进入

	// 解析请求数据
	UINT Uin = Read32(Body);            // 用户ID
	UINT Time = Read32(Body);           // 时间戳
	USHORT RoomID = Read16(Body);       // 目标房间ID

	char Password[16] = { 0 };
	memcpy(Password, Body, 16);         // 获取输入的房间密码
	Body += 16;


	BYTE InviteType = Read8(Body);      // 邀请类型
	UINT InviterUin = Read32(Body);     // 邀请者ID
	BOOL HasMoreInfo = FALSE;

	// 获取指定房间的节点信息
	std::shared_ptr<RoomNode> Room = GetRoom(RoomID);
	if (!Room)
	{
		// 如果房间不存在
		ResponseStartGame(Client, "对不起,您要加入的房间已经不存在,请选择加入其他房间!");
		return;
	}
	// 检查房间人数是否已满
	if (Room->CurrentPlayerNum == 6 || Room->SeatNum == Room->CurrentPlayerNum) {
		ResponseStartGame(Client, "你要进入的房间已经满人了");
		return;
	}
	// 检查房间是否正在游戏中
	if (Room->Callback != NULL)
	{
		ResponseStartGame(Client, "对不起,您要加入的房间正在游戏中!");
		return;
	}
	// 如果用户不是管理员，则验证房间密码
	if (Client->admin != 1) {
		for (int i = 0; i < 15; i++)
		{
			if (Password[i + 1] != Room->Password[i + 1])
			{    //默认传0x30
				//从第1位开始比较 0位不管
				ResponseStartGame(Client, "房间密码错误!");
				return;
			}
		}
	}

	// 查找房间内的空座位
	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient == 0 && Room->SeatState[i] == 1)
		{
			Client->SeatID = i;
			Client->RoomID = RoomID;
			Client->ReadyState = 1;
			Room->Player[i] = Client;
			Room->CurrentPlayerNum++;
			Client->ChatRoomID = -1;
			// 根据房间类型处理用户加入房间的响应
			if (Room->ChatRoomType == 6)
			{
				//进入3d房间
				ResponseCreateRoom(Client, Room, 6);
				RequestEnter3DRoom(Client);
				NotifyChangeRoomOwner(Client, Room);
			}
			else
			{
				ResponseEnterRoom(Client, Room, i, 0, 0);//回复进入房间让自己看到其他人的信息

				// 根据房间的不同模式发送相应的提示信息
				if (Room->Israce == 1) {
					Client->isRace = 1;
					ResponseStartGame(Client, "该房间已开启比赛模式！");
				}
				if (Room->IsAnonymous == 1) {
					Client->IsAnonymous = 1;
					ResponseStartGame(Client, "该房间已开启匿名模式！");
				}
				if (Room->IsStake == 1) {
					Client->IsStake = 1;
					ResponseStartGame(Client, "该房间已开启下注单挑模式！");
				}
				// 根据 IsBottomGame 的模式生成不同的提示信息
				if (Room->IsBottomGame == 1) {
					Client->IsBottomGame = 1;
					std::string msg = GetItemName(IsBottomGame[1]);
					msg = "该房间已开启垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00，扣除最后/C01【" + std::to_string(IsBottomGame[3]) + "】/C00名的道具，" + "扣除数量是：/C01【" + std::to_string(IsBottomGame[2]) + "】";
					ResponseStartGame(Client, msg.c_str());
				}
				if (Room->IsBottomGame == 2) {
					Client->IsBottomGame = 1;
					std::string msg = GetItemName(IsBottomGame[1]);
					msg = "该房间已开启垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00，扣除最后/C01【" + std::to_string(IsBottomGame[3]) + "】/C00名的道具，" + "扣除数量是：/C01【" + std::to_string(Room->BottomGameNum) + "】";
					ResponseStartGame(Client, msg.c_str());
				}
				if (Room->IsBottomGame == 4) {
					Client->IsBottomGame = 1;
					int Frirst = Room->BottomGameNum / 10 * 4.5;
					int Second = Room->BottomGameNum / 10 * 3.5;
					int Third = Room->BottomGameNum / 10 * 2;
					std::string msg = "房主已开启四人垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00\n第一名/C01赢【" + std::to_string(Frirst) + "】/C00，第二名/C01赢【"
						+ std::to_string(Second) + "】/C00，第三名/C01赢【" + std::to_string(Third) + "】/C00，第四名/C01输【" + std::to_string(Room->BottomGameNum) + "】/C00";
					ResponseStartGame(Client, msg.c_str());
				}
				if (Room->IsBottomGame == 6) {
					Client->IsBottomGame = 1;
					int  Num = Room->BottomGameNum;
					int Frirst = Num / 10 * 4;
					int Second = Num / 10 * 3;
					int Third = Num / 10 * 2;
					int Four = Num / 10 * 1;
					int five = Num / 10 * 4;
					int six = Num / 10 * 6;
					std::string msg = "房主已开启六人垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00\n第一名/C01赢【" + std::to_string(Frirst) + "】/C00，第二名/C01赢【"
						+ std::to_string(Second) + "】/C00，第三名/C01赢【" + std::to_string(Third) + "】/C00\n第四名/C01赢【" + std::to_string(Four) + "】/C00"
						+ "第五名/C01输【" + std::to_string(five) + "】/C00第六名/C01输【" + std::to_string(six) + "】/C00";
					ResponseStartGame(Client, msg.c_str());
				}
				if (Room->IsBottomGame == 7) {
					Client->IsBottomGame = 1;
					int Frirst = 5;
					int Second = 4;
					int Third = 3;
					int Four = 2;
					int Five = 1;
					std::string msg = "房主已开启竞技模式！你会获得道具/C01【" + GetItemName(106614) + "】/C00\n第一名/C01赢【" + std::to_string(Frirst) + "】/C00，第二名/C01赢【"
						+ std::to_string(Second) + "】/C00，第三名/C01赢【" + std::to_string(Third) + "】/C00第四名/C01赢【" + std::to_string(Four) + "】/C00第五名/C01赢【" + std::to_string(Five) + "】/C00";

					ResponseStartGame(Client, msg.c_str());
				}

				if (Room->IsBottomGame == 9) {
					std::string msg = GetItemName(Mantissa[3]);
					msg = "房主已开启尾数！消耗道具是/C01【" + GetItemName(Mantissa[3]) + "】/C00，最低下注/C01【" + std::to_string(Mantissa[1]) + "】/C00最高下注/C01【" + std::to_string(Mantissa[2]) + "】输入obs观战后，尾数有尾大，尾小，尾单 ，尾双，四种选择。例如输入：/C01尾数单100";
					ResponseStartGame(Client, msg.c_str());
				}

			}
			//设置观战
			SendToClient(Client, 134, (BYTE*)"\x00\x00\x00\x00", 4, Client->ConnID, FE_PLAYER, Client->ConnID, Response);


			//通知其它玩家   通知房间内的其他玩家用户进入房间的信息
			for (char i2 = 0; i2 < 6; i2++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i2];
				if (RoomClient && RoomClient != Client)
				{
					NotifyEnterRoom(RoomClient, Room, Client, i);
					NotifyEnterRoom(Client, Room, RoomClient, i2);
				}
			}
			// 通知其他玩家用户的准备状态
			for (char i3 = 0; i3 < 6; i3++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i3];
				if (RoomClient && RoomClient != Client)
				{
					NotifyChangeReadyState(RoomClient, Client->Uin, Client->ReadyState);
					NotifyChangeReadyState(Client, RoomClient->Uin, RoomClient->ReadyState);
				}
			}
			// 更新所有空座位的状态
			for (char i4 = 0; i4 < 6; i4++)
			{
				if (Room->SeatState[i4] == 0)
				{
					NotifyChangeSeatState(Client, i4, 0);//座位状态变化
				}
			}
			// 如果是观战模式，发送观战状态通知
			if (obs == 1) {
				if (Room->ChatRoomType != 6) {
					//观战
					for (char i5 = 0; i5 < 6; i5++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[i5];
						if (RoomClient && RoomClient != Client)
						{
							if (Client->OBState == 1) {
								NotifyChangeOBState(RoomClient, Client->OBState, Client->Uin);
							}
							if (RoomClient->OBState == 1) {
								NotifyChangeOBState(Client, RoomClient->OBState, RoomClient->Uin);
							}
						}
					}
				}
			}


			//通知其它玩家

			break;
		}
	}

}

void ResponseEnterRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, char SeatID, BOOL HasMoreInfo, char index)
{
	//printf("ResponseEnterRoom\n");
	//回复进入房间
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write16(p, Room->ID); //RoomID
	Write16(p, Room->BaseGameMode); //BaseGameMode
	Write16(p, Room->SubGameMode); //SubGameMode


	memcpy(p, Room->Name, MaxRoomName); //RoomName
	p += MaxRoomName;

	Write8(p, 6); //SeatNum 固定写当前6个座位可用
#ifdef ZingSpeed
	Write8(p, (BYTE)Room->Flag); //Flag
#else
	Write16(p, 0x18c); //Flag
#endif

	Write8(p, SeatID); //SeatID
	Write8(p, 0); //TeamID
	Write32(p, Room->MapID); //MapID
	Write16(p, Room->RoomOwnerID); //RoomOwnerID

	//SeatStatus[6]
	for (size_t i = 0; i < 6; i++)
	{
		Write8(p, 2);//写6个座位的座位状态 0显示x 
		//Write8(p, 1);
	}

	//BYTE* pPlayerCount = p;
	Write8(p, 0); //PlayerCount //写其他玩家信息 多了看不到
	/*int PlayerCount = 0;
	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient != Client)
			{
			   PlayerCount++;
			   WriteVisbleInfo(RoomClient, p, RoomClient->Uin, RoomClient->SeatID, RoomClient->ConnID); //NewPlayerInfo
			   break;
			}
		}
	}
	Set8(pPlayerCount, PlayerCount);*/

	Write8(p, 0); //ReasonLen
	Write8(p, 0); //PlayerCount2

	Write16(p, 0); //KartNum


	Write8(p, 0); //LoverRaceOrPro

	{ //STDynamicInfo
		BYTE* pSTDynamicInfo = p;
		Write16(pSTDynamicInfo, 0); //len

		Write8(pSTDynamicInfo, Room->STDynamicInfo.Season);
		Write8(pSTDynamicInfo, Room->STDynamicInfo.Timeslot);
		Write8(pSTDynamicInfo, Room->STDynamicInfo.Weather);

		len = pSTDynamicInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //PreKnowMapID
	Write8(p, 0); //HasMoreInfo
	Write32(p, 0); //MapSubID

	Write8(p, 0); //HasFBInfo

	Write32(p, 0); //GuildScoreInNextGame
	Write16(p, 0); //GuildScoreAlreadyAward
	Write16(p, 0); //GuildScoreTotalAward

	Write8(p, 0); //SpeFlag
	Write8(p, 0); //MapNum

	Write32(p, Room->ID); //RoomNo
	Write16(p, 0); //SvrId
	Write16(p, 0); //RaceCardNums

	Write16(p, 0); //CreateRoomType
	Write16(p, 0); //ServerMatchType
	Write16(p, 0); //TeamMatchType
	Write16(p, 0); //MatchedRoomType
	Write8(p, 0); //InviteType
	Write8(p, 0); //HasWeRelayRoomInfo

	Write8(p, 0); //RoomFlag
	Write8(p, 0); //MaxOBNum
	Write8(p, 0); //AllowAutoStart
	Write8(p, 0); //HasLMInfo

	Write8(p, 0); //TalkRoomBGID
	Write16(p, 0); //Role
	Write8(p, 0); //ParaNum
	//Write32(p, 0); //ParaList[]
	{ //RoomInfoChange
		BYTE* pRoomInfoChange = p;
		Write16(pRoomInfoChange, 0); //len

		Write32(pRoomInfoChange, 0); //TeamJumpLevel
		Write32(pRoomInfoChange, 0); //RoomBGID

		len = pRoomInfoChange - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //PlayerEquipKartID
	Write32(p, 0); //RankedMatchTestDriveKartID
	Write8(p, 0); //FirstlyTipMap
	if (GVoice || version > 1.3) {
		WriteString_MC(p, Room->GVoiceRoomName.c_str()); //GVoiceRoomNameLen
	}
	else {
		Write8(p, 0);
	}
	Write8(p, 0); //GVoiceReasonLen
	Write8(p, 0); //TDCExcluItemNum
	//Write32(p, 0); //TDCExclusiveItemID[]
	Write8(p, 0); //TestKartGameType
	Write32(p, 0); //GameTestKartID
	Write8(p, 0); //HasStarsFightingInfo

	Write8(p, 0); //HasYuLeJiaNianHuaInfo

	len = p - buf.data();
	SendToClient(Client, 104, buf.data(), len, Room->ID, FE_ROOM, Client->ConnID, Response);
}

void NotifyEnterRoom1(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, std::shared_ptr<ClientNode> RoomClient, char SeatID)
{

	//printf("NotifyEnterRoom\n");
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;



	Write16(p, Room->ID); //RoomID

	WriteVisbleInfo1(RoomClient, p, RoomClient->Uin, SeatID, RoomClient->ConnID); //NewPlayerInfo

	{ //PlayerRelationInfo
		BYTE* pPlayerRelationInfo = p;
		Write16(pPlayerRelationInfo, 0); //len


		Write32(pPlayerRelationInfo, RoomClient->Uin); //SrcUin

		Write32(pPlayerRelationInfo, 0); //RelationFlag

		Write32(pPlayerRelationInfo, 0); //RelationUin

		memset(pPlayerRelationInfo, 0, MaxNickName);
		pPlayerRelationInfo += MaxNickName;

		Write32(pPlayerRelationInfo, 0); //EngageTime
		Write32(pPlayerRelationInfo, 0); //NextCanBookingTimeLimit
		Write32(pPlayerRelationInfo, 0); //BeginHoneyMoonTime
		Write32(pPlayerRelationInfo, 0); //EndHoneyMoonTime
		Write8(pPlayerRelationInfo, 0x4); //EngageFlag


		len = pPlayerRelationInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 1); //KartNum
	{ //KartRefitInfo
		BYTE* pKartRefitInfo = p;
		Write16(pKartRefitInfo, 0); //len

		Write32(pKartRefitInfo, RoomClient->Uin); //Uin
		Write32(pKartRefitInfo, RoomClient->KartID); //KartId
		Write32(pKartRefitInfo, 0); //RefitCout
		Write16(pKartRefitInfo, 0); //MaxFlags
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.WWeight); //WWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.SpeedWeight); //SpeedWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.JetWeight); //JetWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.SJetWeight); //SJetWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.AccuWeight); //AccuWeight
		Write32(pKartRefitInfo, 0); //ShapeRefitCount
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartTailRefitItemID); //KartTailRefitItemID
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartTireRefitItemID); //KartTireRefitItemID
		Write32(pKartRefitInfo, 0); //SecondRefitCount
		//Write16(pKartRefitInfo, 0); //Speed2Weight
		//Write16(pKartRefitInfo, 0); //DriftVecWeight
		//Write16(pKartRefitInfo, 0); //AdditionalZSpeedWeight
		//Write16(pKartRefitInfo, 0); //AntiCollisionWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.Speed2Weight); //Speed2Weight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.DriftVecWeight); //DriftVecWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.AntiCollisionWeight); //AntiCollisionWeight
		Write16(pKartRefitInfo, 0); //LuckyValue
		Write16(pKartRefitInfo, 0); //RefitLuckyValueMaxWeight
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.ShapeSuitID); //ShapeSuitID
		//Write8(pKartRefitInfo, 10); //LegendSuitLevel
		//Write32(pKartRefitInfo, 1111); //LegendSuitLevelChoice
		Write8(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.LegendSuitLevel); //LegendSuitLevel
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.ShapeSuitID); //ShapeLegendSuitID

		len = pKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //RoomInfoChange
		BYTE* pRoomInfoChange = p;
		Write16(pRoomInfoChange, 0); //len

		Write32(pRoomInfoChange, 0); //TeamJumpLevel
		Write32(pRoomInfoChange, 0); //RoomBGID

		len = pRoomInfoChange - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 500, buf.data(), len, Room->ID, FE_ROOM, Client->ConnID, Notify);
}


void NotifyEnterRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, std::shared_ptr<ClientNode> RoomClient, char SeatID)
{

	//printf("NotifyEnterRoom\n");
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;



	Write16(p, Room->ID); //RoomID


	WriteVisbleInfo(RoomClient, p, RoomClient->Uin, SeatID, RoomClient->ConnID); //NewPlayerInfo


	{ //PlayerRelationInfo
		BYTE* pPlayerRelationInfo = p;
		Write16(pPlayerRelationInfo, 0); //len


		Write32(pPlayerRelationInfo, RoomClient->Uin); //SrcUin

		Write32(pPlayerRelationInfo, 0); //RelationFlag

		Write32(pPlayerRelationInfo, 0); //RelationUin

		memset(pPlayerRelationInfo, 0, MaxNickName);
		pPlayerRelationInfo += MaxNickName;

		Write32(pPlayerRelationInfo, 0); //EngageTime
		Write32(pPlayerRelationInfo, 0); //NextCanBookingTimeLimit
		Write32(pPlayerRelationInfo, 0); //BeginHoneyMoonTime
		Write32(pPlayerRelationInfo, 0); //EndHoneyMoonTime
		Write8(pPlayerRelationInfo, 0x4); //EngageFlag


		len = pPlayerRelationInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 1); //KartNum
	{ //KartRefitInfo
		BYTE* pKartRefitInfo = p;
		Write16(pKartRefitInfo, 0); //len
		Write32(pKartRefitInfo, RoomClient->Uin); //Uin
		Write32(pKartRefitInfo, RoomClient->KartID); //KartId
		Write32(pKartRefitInfo, 0); //RefitCout
		Write16(pKartRefitInfo, 0); //MaxFlags
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.WWeight); //WWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.SpeedWeight); //SpeedWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.JetWeight); //JetWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.SJetWeight); //SJetWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.AccuWeight); //AccuWeight
		Write32(pKartRefitInfo, 0); //ShapeRefitCount
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartTailRefitItemID); //KartTailRefitItemID
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.KartTireRefitItemID); //KartTireRefitItemID
		Write32(pKartRefitInfo, 0); //SecondRefitCount
		//Write16(pKartRefitInfo, 0); //Speed2Weight
		//Write16(pKartRefitInfo, 0); //DriftVecWeight
		//Write16(pKartRefitInfo, 0); //AdditionalZSpeedWeight
		//Write16(pKartRefitInfo, 0); //AntiCollisionWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.Speed2Weight); //Speed2Weight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.DriftVecWeight); //DriftVecWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.AntiCollisionWeight); //AntiCollisionWeight
		Write16(pKartRefitInfo, 0); //LuckyValue
		Write16(pKartRefitInfo, 0); //RefitLuckyValueMaxWeight
		Write32(pKartRefitInfo, RoomClient->KartSkinID); //ShapeSuitID
		//Write8(pKartRefitInfo, 10); //LegendSuitLevel
		//Write32(pKartRefitInfo, 1111); //LegendSuitLevelChoice
		Write8(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.LegendSuitLevel); //LegendSuitLevel
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pKartRefitInfo, RoomClient->ClientKartRefitItemInfo.ShapeLegendSuitID); //ShapeLegendSuitID

		len = pKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //RoomInfoChange
		BYTE* pRoomInfoChange = p;
		Write16(pRoomInfoChange, 0); //len

		Write32(pRoomInfoChange, 0); //TeamJumpLevel
		Write32(pRoomInfoChange, 0); //RoomBGID

		len = pRoomInfoChange - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 500, buf.data(), len, Room->ID, FE_ROOM, Client->ConnID, Notify);
}

void RequestLeaveRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	short LeaveType = 0;
#ifndef ZingSpeed
	LeaveType = Read16(Body);//正常是0 客户端某些时候会自动请求该消息导致退房错误 
	//BYTE LeaveRoomReason = Read8(Body);
#endif
	//普通房间强制观战后防止自动退出
	if (obs == 1) {
		if (Client->OBState == 1 && Client->ChatRoomID == -1)
		{
			std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
			//防止观战状态强制退出 会请求2次
			if (Room)
			{
				if (Client->AutoLeave != 1)//保证每局只触发自动退出一次释放资源
				{
					ResponseLeaveRoom(Client, 0);
					Client->AutoLeave = 1;
					return;
				}
				ResponseEnterRoom(Client, Room, Client->SeatID, 0, 0);
				for (char j = 0; j < 6; j++)
				{
					std::shared_ptr<ClientNode> RoomClient2 = Room->Player[j];
					if (RoomClient2 && RoomClient2 != Client)
					{
						NotifyEnterRoom(Client, Room, RoomClient2, RoomClient2->SeatID);
					}
				}
				SendToClient(Client, 134, (BYTE*)"\x00\x00\x01\x00", 4, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
				ResponseStartGame(Client, "请关闭观战模式再退出房间");

				if (Room->ChatRoomType != 6) {
					for (char i5 = 0; i5 < 6; i5++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[i5];
						if (RoomClient && RoomClient != Client)
						{
							NotifyChangeOBState(RoomClient, Client->OBState, Client->Uin);
						}
					}
				}
				if (Room->IsBottomGame == 9 && Client->news.length() > 0) {
					NoticeClient(Client, Client->news);
					Client->news = "";
				}
				//Client->AutoLeave = 0;
				return;
			}
		}
	}


	if (LeaveType == 0 && Client && Client->BorderRoomID == 0)//防止匹配自动退出
	{
		LeaveRoom(Client, 0);
		LeaveOutdoor(Client);
	}

}
void LeaveRoom(std::shared_ptr<ClientNode> Client, short LeaveType)
{
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
			if (RoomClient == Client)
			{
				ResponseLeaveRoom(Client, LeaveType);
				RoomClient->ReadyState = 1;
				//RoomClient->OBState = 0;
				RoomClient->TaskID = 0;
				RoomClient->RoomID = 0;
				RoomClient->isRace = 0;
				RoomClient->IsAnonymous = 0;
				RoomClient->IsStake = 0;
				RoomClient->IsBottomGame = 0;
				RoomClient->StakeNum = 0;
				RoomClient->TeamID = 0;
				RoomClient->lastUpdateTime = std::chrono::steady_clock::time_point{};
				Room->Player[i] = NULL;
				Room->CurrentPlayerNum--;

				BOOL IsRoomOwner = FALSE;
				short NewRoomOwnerID = Room->RoomOwnerID;
				if (NewRoomOwnerID == Client->ConnID)
				{
					IsRoomOwner = TRUE;
					for (char i2 = 0; i2 < 6; i2++)
					{
						RoomClient = Room->Player[i2];
						if (RoomClient)
						{
							NewRoomOwnerID = RoomClient->ConnID; //转让房主
							RoomClient->ReadyState = 0;//设置准备状态
							// 在转让房主时，关闭所有特定模式
							Room->IsBottomGame = 0;             // 关闭垫底赛模式
							break;
						}
					}
				}
				Room->RoomOwnerID = NewRoomOwnerID;

				//通知其它玩家
				int n = 0;
				for (char i2 = 0; i2 < 6; i2++)
				{
					RoomClient = Room->Player[i2];
					if (RoomClient)
					{
						NotifyLeaveRoom(RoomClient, Client->Uin, i, IsRoomOwner, NewRoomOwnerID, LeaveType);
						n++;
					}
				}

				if (n == 0)
				{
					if (Room->Timer)
					{
						if (DeleteTimerQueueTimer(TimerQueue, Room->Timer, INVALID_HANDLE_VALUE))
						{
							Room->Timer = NULL;
						}
					}

					std::lock_guard<std::recursive_mutex> lg(Mutex);
					Rooms.erase(Room->ID);
					//Room = NULL;
				}
				break;

			}
		}
	}
}
void LeaveBorderRoom(std::shared_ptr<ClientNode> Client, short LeaveType)
{
	std::shared_ptr<RoomNode> Room = GetBorderRoom(Client->BorderRoomID);
	if (!Room)
	{
		return;
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient == Client)
			{
				ResponseLeaveRoom(RoomClient, LeaveType);
				RoomClient->ReadyState = 1;
				RoomClient->TaskID = 0;
				RoomClient->IsFinish = 0;
				RoomClient->BorderRoomID = 0;
				RoomClient->isRace = 0;
				RoomClient->IsStake = 0;
				RoomClient->IsBottomGame = 0;
				RoomClient->StakeNum = 0;
				RoomClient->IsAnonymous = 0;
				RoomClient->TeamID = 0;
				Room->Player[i] = NULL;
				Room->CurrentPlayerNum--;

				//通知其它玩家
				int n = 0;
				for (char i2 = 0; i2 < 6; i2++)
				{
					RoomClient = Room->Player[i2];
					if (RoomClient)
					{
						NotifyLeaveRoom(RoomClient, Client->Uin, i, 0, 0, LeaveType);
						n++;
					}
				}

				if (n == 0)
				{
					if (Room->Timer)
					{
						if (DeleteTimerQueueTimer(TimerQueue, Room->Timer, INVALID_HANDLE_VALUE))
						{
							Room->Timer = NULL;
						}
					}

					std::lock_guard<std::recursive_mutex> lg(Mutex);
					BorderRooms.erase(Room->ID);
				}
				break;
			}
		}
	}
}
void ResponseLeaveRoom(std::shared_ptr<ClientNode> Client, short LeaveType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write8(p, 0); //ReasonLen

#ifndef ZingSpeed
	Write16(p, LeaveType); //LeaveType
	Write16(p, 0); //BaseGameMode
	Write16(p, 0); //SubGameMode
	if (GVoice|| version > 1.3) {
		Write8(p, 1); //IsLeaveGVoice
	}
	else {
		Write8(p, 0);
	}
	
#endif

	len = p - buf.data();
	SendToClient(Client, 105, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}
void NotifyLeaveRoom(std::shared_ptr<ClientNode> Client, UINT Uin, char SeatID, bool IsChangeRoomOwner, short NewRoomOwnerID, short LeaveType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, Client->RoomID); //RoomID
	Write32(p, Uin); //Uin
	Write8(p, SeatID); //SeatID
	Write8(p, IsChangeRoomOwner); //IsChangeRoomOwner
	Write16(p, NewRoomOwnerID); //NewRoomOwnerID

#ifdef ZingSpeed
	Write8(p, 0); //Flag
#else
	Write16(p, 0); //Flag
#endif

	Write8(p, 0); //HasFBInfo
	/*
m_astFBInfo[].m_bAvailableLev
m_astFBInfo[].m_bLev
m_astFBInfo[].m_bSubLev
m_astFBInfo[].m_iBossID
m_astFBInfo[].m_bFBSeason
	*/

	len = p - buf.data();
	SendToClient(Client, 501, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Notify);
}


void NotifyBorderPlayerInfo(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> BorderRoom)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;
	Write32(p, BorderRoom->BaseGameMode); //BaseMode
	Write32(p, BorderRoom->SubGameMode); //SubMode
	Write8(p, BorderRoom->CurrentPlayerNum); //PlayerCount
	for (size_t i = 0; i < 6; i++)
	{ //PlayerRoomVisibleInfo
		std::shared_ptr<ClientNode> RoomClient = BorderRoom->Player[i];
		if (RoomClient)
		{
			WriteVisbleInfo1(RoomClient, p, RoomClient->Uin, 0, 0);
		}
	}
	Write16(p, 0);

	if (Client->TaskID == 0xFFFD || Client->TaskID == 0)
	{
		memcpy(p, "\x00\x00\x00\x00\x00", 5);
		p += 5;
	}
	else if (Client->TaskID == 0xFFFC)
	{
		memcpy(p, "\x00\x00\x01\x00\x1e\x00\x01\x98\x99\x00\x01\x98\xa3\x00\x01\x98\x99\x00\x01\x98\xa3\x00\x00\x00\x00\x00\x00\x27\x3d\x00\x00\x27\x30\x00\x00", 35);
		p += 35;
	}

	len = p - buf.data();

	SendToClient(Client, 548, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	//delete[] buf;
}
void NotifyRankedMatchBanMapInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	{ //BanMapCfg
		BYTE* pBanMapCfg = p;
		Write16(pBanMapCfg, 0); //len

		Write32(pBanMapCfg, 0); //GradeLevelLimit
		Write32(pBanMapCfg, 4); //ChooseTime
		Write32(pBanMapCfg, 4); //ShowTime
		Write32(pBanMapCfg, 8); //TotalMapNum
		Write32(pBanMapCfg, 1); //ChooseMapNum
		Write32(pBanMapCfg, 0); //BonusScore
		Write32(pBanMapCfg, 0); //PinnacleBattleTotalMapNum
		Write32(pBanMapCfg, 0); //PinnacleBattleChooseMapNum

		len = pBanMapCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 8); //MapNum
	for (char i = 0; i < 8; i++)
	{ //MapInfo
		BYTE* pMapInfo = p;
		Write16(pMapInfo, 0); //len

		Write32(pMapInfo, RankMap->MapID[rand() % RankMap->Size]); //MapID
		Write32(pMapInfo, 0); //Status
		Write8(pMapInfo, 0); //ThemeMapOrNot
		Write8(pMapInfo, 0); //SeasonNewMapOrNot
		Write8(pMapInfo, 0); //PinnacleBattleMapOrNot
		Write16(pMapInfo, 0); //TotalBeBannedNums
		Write16(pMapInfo, 0); //FinalRandBanMapType

		len = pMapInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //Type

	len = p - buf.data();
	SendToClient(Client, 25009, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);

}

NOINLINE void NotifyRankedMatchBanMapResult(std::shared_ptr<ClientNode> Client, CMatchBanMapInfo* MatchBanMapInfo)
{
	//通知排名赛禁地图结果
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, 8); //MapNum
	for (size_t j = 0; j < 8; j++)
	{ //MapInfo
		BYTE* pMapInfo = p;
		Write16(pMapInfo, 0); //len

		Write32(pMapInfo, MatchBanMapInfo[j].MapID); //MapID
		Write32(pMapInfo, MatchBanMapInfo[j].Status); //Status
		Write8(pMapInfo, MatchBanMapInfo[j].ThemeMapOrNot); //ThemeMapOrNot
		Write8(pMapInfo, MatchBanMapInfo[j].SeasonNewMapOrNot); //SeasonNewMapOrNot
		Write8(pMapInfo, MatchBanMapInfo[j].PinnacleBattleMapOrNot); //PinnacleBattleMapOrNot
		Write16(pMapInfo, MatchBanMapInfo[j].TotalBeBannedNums); //TotalBeBannedNums
		Write16(pMapInfo, MatchBanMapInfo[j].FinalRandBanMapType); //FinalRandBanMapType

		len = pMapInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	//Write8(p, 0); //Type

	len = p - buf;
	SendToClient(Client, 25011, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void WINAPI OnBorderRace(std::shared_ptr<RoomNode> Room)
{
	Room->dwStart = GetTickCount();
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyRaceBegin(RoomClient);
			//SendToClient(RoomClient, 28241, (BYTE*)"\x00\x00\x27\x11\x00\x00\x14\x00\x00\x00\x63\x00\x00\x00\x00\x00\x00\x00", 18, RoomClient->GameID, FE_GAMELOGIC, RoomClient->ConnID, Notify);

		}
	}
}
void WINAPI OnBorderGame(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			RoomClient->MsgSequence = -1;
			if (RoomClient->GlobalStone == 1)
			{
				NotifyUpdateKartSkillStoneInfoConfig(RoomClient, RoomClient->KartID);//全局宝石
				//NotifyUpdateKartSkillStoneInfo2(RoomClient, RoomClient->KartID);//全局宝石
			}


			NotifyGameBegin(RoomClient, Room->Player);
			NotifyGameNPCInfo(RoomClient);
			//NotifyKartPhysParam(RoomClient, NULL);
			if (mode28346 == 2) {
				NotifyKartPhysParam(RoomClient, Room->Player);
			}
			else {
				NotifyEncryptedMsg(RoomClient);
			}
		}
	}

	//跳舞并同步加载
	CreateRoomTimer(Room, 16000, OnBorderRace);

}

std::vector<int> GenerateDiffNumber(int min, int max, int num)
{
	int rnd;
	std::vector<int> diff;
	std::vector<int> tmp;//存储剩余的数
	//初始化
	for (int i = min; i < max + 1; i++)
	{
		tmp.emplace_back(i);
	}
	//srand((unsigned)time(0)); //初始化随机数种子
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < num; i++)
	{
		do {
			rnd = min + rand() % (max - min + 1);

		} while (tmp.at(rnd - min) == -1);
		diff.emplace_back(rnd);
		tmp.at(rnd - min) = -1;
	}
	return diff;
}

void WINAPI OnRankedMatchBanMapInfo(std::shared_ptr<RoomNode> Room)
{
	//CMatchBanMapInfo* MatchBanMapInfo = Room->MatchBanMapInfo;
	//for (const auto& RoomPlayer : Room->Player) {
	//	if (RoomPlayer) {
	//		const int MatchBanMapIndex = RoomPlayer->MatchBanMapIndex;
	//		if (MatchBanMapIndex != -1) {
	//			MatchBanMapInfo[MatchBanMapIndex].Status = 2;
	//		}

	//	}
	//}

	//srand(static_cast<unsigned int>(time(nullptr)));
	//UINT MatchMapIex = 0;//

	//do
	//{
	//	MatchMapIex = rand() % 8;//

	//} while (MatchBanMapInfo[MatchMapIex].Status == 2);

	//MatchBanMapInfo[MatchMapIex].Status = 3;

	//UINT MatchMapID = MatchBanMapInfo[MatchMapIex].MapID;//

	//for (const auto& RoomPlayer : Room->Player) {
	//	if (RoomPlayer) {
	//		RoomPlayer->MatchMapID = MatchMapID;
	//		NotifyRankedMatchBanMapResult888(RoomPlayer, MatchBanMapInfo);
	//	}
	//}

	//if (MatchBanMapInfo) {
	//	delete[] MatchBanMapInfo;
	//	MatchBanMapInfo = nullptr;
	//}

	//CreateRoomTimer(Room, 4500, OnMatchQueueType888);
	//
	//CreateRoomTimer(Room, 1000, OnNotifyPlayerInfo);
}

void WINAPI OnNotifyPlayerInfo(std::shared_ptr<RoomNode> Room)
{
	int time = 0;
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyBorderPlayerInfo(RoomClient, Room);
			if (RoomClient->TaskID == 0xFFFC)
			{
				time = 3000;
				NotifyRankedMatchBanMapInfo(RoomClient);//排位禁图

			}
			else
			{
				time = 1000;
			}
		}
	}

	//开始游戏
	CreateRoomTimer(Room, time, OnBorderGame);

}

void WINAPI OnMatchResult(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{

			NotifyMatchResult(RoomClient, Room->matchQueueType);
		}
	}
	//等待EnterMatchRoom 并发送NotifyBorderPlayerInfo
	CreateRoomTimer(Room, 1000, OnNotifyPlayerInfo);

}


void RankedMatchBeginTest()
{
	std::lock_guard<std::recursive_mutex> lg(RankedQueueMutex);
	int dequesize = RankedMatchQueue.size();


	auto now = std::chrono::steady_clock::now();
	if (dequesize < Rankset[4] && Rankset[1] == 1) {
		for (auto it = RankedMatchQueue.begin(); it != RankedMatchQueue.end(); /* no increment here */)
		{
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->second);
			if (duration.count() > RankerManTime && it->first->isManMachine == 1)
			{
				ResponseStartQuickMatchGame2(it->first, 0xa, 0);
				// 根据需要执行其他操作，如移除过期的客户端
				it = RankedMatchQueue.erase(it); // 删除过期的元素
			}
			else
			{
				++it; // 只有在没有删除元素时才移动迭代器
			}
		}
	}
	dequesize = RankedMatchQueue.size();


	//printf("RankedMatchQueueSize:%d\n", dequesize);
	if (dequesize >= Rankset[4])
	{
		std::shared_ptr<RoomNode> RankedRoom = NewBorderRoom();
		if (!RankedRoom)
		{
			return;
		}
		RankedRoom->startStatus = 2;
		RankedRoom->BaseGameMode = 0x28;
		RankedRoom->SubGameMode = 1;
		//随机地图
		RankedRoom->matchQueueType = 0xa;
		RankedRoom->MapID = GetRandMap(16);//排位地图池
		//排位禁图
		//RANDOMMAPPOOL* MapPool = GetRandomMapPool(16);
		//CMatchBanMapInfo* uMatchBanMapInfo = new CMatchBanMapInfo[8]{ 0 };
		//std::vector<int> RandMapList = GenerateDiffNumber(0, MapPool->size - 1, 8);

		//for (size_t j = 0; j < 8; j++) {
		//	uMatchBanMapInfo[j].MapID = MapPool->MapID[RandMapList[j]];
		//	if (j >= 7) {
		//		uMatchBanMapInfo[j].Status = 1;//随机
		//	}
		//}
		//RankedRoom->MatchBanMapInfo = uMatchBanMapInfo;
		//排位禁图

		for (int i = 0; i < dequesize; i++)
		{
			if (i == 6 || RankedMatchQueue.empty())
			{
				break;
			}
			std::shared_ptr<ClientNode> Client = RankedMatchQueue.front().first;
			RankedRoom->Player[i] = Client;//用户加入排位房间
			Client->GameID = 2;
			Map* pMap = GetMap(RankedRoom->MapID);
			if (pMap)
			{
				Client->MapCompatibilityMode = false;
				Client->EndCheckPoint = pMap->End;
				Client->TotalRound = pMap->Round;
				Client->Loop = pMap->Loop;
				//Client->TotalRound = 1;
			}
			else
			{
				Client->MapCompatibilityMode = true;
				Client->EndCheckPoint = 0;
				Client->TotalRound = 1;
				Client->Loop = false;
			}
			Client->MapID = RankedRoom->MapID;
			//Client->MapID = 112;
			Client->FinishTime = 0;
			Client->Round = 0;
			Client->PassedCheckPoint = 0;
			Client->IsFinish = FALSE;
			//Client->IsReady = FALSE;
			Client->IsReady = TRUE;
			Client->real = 1;
			Client->BorderRoomID = RankedRoom->ID;
			RankedRoom->CurrentPlayerNum++;
			RankedMatchQueue.pop_front();;//队列删除用户
		}
		CreateRoomTimer(RankedRoom, 1000, OnMatchResult);
	}
}


void BorderMatchBeginTest()
{
	std::lock_guard<std::recursive_mutex> lg(BorderQueueMutex);

	int dequesize = BorderMatchQueue.size();

	auto now = std::chrono::steady_clock::now();
	if (dequesize < Borderset[5] && Borderset[1] == 1) {
		for (auto it = BorderMatchQueue.begin(); it != BorderMatchQueue.end(); /* no increment here */)
		{
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->second);
			if (duration.count() > BorderManTime && it->first->isManMachine == 1)
			{
				ResponseStartQuickMatchGame2(it->first, 0x0, 0);
				// 根据需要执行其他操作，如移除过期的客户端
				it = BorderMatchQueue.erase(it); // 删除过期的元素
			}
			else
			{
				++it; // 只有在没有删除元素时才移动迭代器
			}
		}
	}
	dequesize = BorderMatchQueue.size();
	//printf("BorderMatchQueueSize:%d\n", dequesize);
	if (dequesize >= Borderset[5])
	{
		std::shared_ptr<RoomNode> BorderRoom = NewBorderRoom();
		if (!BorderRoom)
		{
			return;
		}
		BorderRoom->SubGameMode = BorderRoom->BaseGameMode = 0x1;

		//随机地图
		BorderRoom->matchQueueType = 0x0;
		BorderRoom->startStatus = 2;
		BorderRoom->MapID = GetRandMap(5);//边境地图池
		for (int i = 0; i < dequesize; i++)
		{
			if (i == 6 || BorderMatchQueue.empty())
			{
				break;
			}
			std::shared_ptr<ClientNode> Client = BorderMatchQueue.front().first;

			BorderRoom->Player[i] = Client;//用户加入排位房间
			Client->GameID = 2;
			Map* pMap = GetMap(BorderRoom->MapID);
			if (pMap)
			{
				Client->MapCompatibilityMode = false;
				Client->EndCheckPoint = pMap->End;
				Client->TotalRound = pMap->Round;
				Client->Loop = pMap->Loop;
			}
			else
			{
				Client->MapCompatibilityMode = true;
				Client->EndCheckPoint = 0;
				Client->TotalRound = 1;
				Client->Loop = false;
			}
			Client->MapID = BorderRoom->MapID;
			Client->FinishTime = 0;
			Client->Round = 0;
			Client->PassedCheckPoint = 0;
			Client->IsFinish = FALSE;
			//Client->IsReady = FALSE;
			Client->IsReady = TRUE;
			Client->BorderRoomID = BorderRoom->ID;
			Client->real = 1;
			BorderRoom->CurrentPlayerNum++;
			BorderMatchQueue.pop_front();//队列删除用户
		}
		CreateRoomTimer(BorderRoom, 1000, OnMatchResult);
	}
}

void ResponseEnterMatchRoom(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //resultId
	Write8(p, 0); //reasonLen

	Write8(p, 0); //HasMemoryRaceInfo
	//
	len = p - buf.data();
	SendToClient(Client, 20082, buf.data(), len, Client->ServerID, FE_PLAYER, Client->ServerID, Response);

}
void RequestEnterMatchRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//UINT uin = Read32(Body);
	//UINT time = Read32(Body);
	//Un roomId = ReadUn(Body);

	ResponseEnterMatchRoom(Client);
}


int GetBorderSize() {
	std::lock_guard<std::recursive_mutex> lg(BorderQueueMutex);
	return BorderMatchQueue.size();
}

int GetRankeSize() {
	std::lock_guard<std::recursive_mutex> lg(RankedQueueMutex);
	return RankedMatchQueue.size();
}
void ResponseStartQuickMatchGame2(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID)
{
	//单人

	std::shared_ptr<RoomNode> BorderRoom = NewBorderRoom();

	if (!BorderRoom)
	{
		return;
	}
	Client->BorderRoomID = BorderRoom->ID;
	if (matchQueueType == 0xa)
	{
		BorderRoom->BaseGameMode = 0x28;
		BorderRoom->SubGameMode = 1;
	}
	else
	{
		BorderRoom->BaseGameMode = BorderRoom->SubGameMode = 1;
	}

	//随机地图
	BorderRoom->matchQueueType = matchQueueType;
	BorderRoom->MapID = GetRandMap(16);//全随地图池
	Client->BaseGameMode = 1;
	Client->SubGameMode = 1;
	Client->real = 0;
	if (matchQueueType == 12)
	{
		Client->BaseGameMode = 17;
	}

	//BorderRoom->MapID = 112;//全随地图池
	//加入房间
	memset(Client->NpcFinishInfo, 0, sizeof(NpcRacerFinishInfo) * NpcNum);
	for (char i = 0; i < 6; i++)
	{
		if (!BorderRoom->Player[i])//空位
		{

			BorderRoom->Player[i] = Client;
			BorderRoom->CurrentPlayerNum++;
			if (MapID != 0)
			{
				BorderRoom->MapID = MapID;
			}
			if (BorderRoom->matchQueueType == 0)
				Client->TaskID = 0xFFFD;
			else if (BorderRoom->matchQueueType == 0xa)
				Client->TaskID = 0xFFFC;
			else if (BorderRoom->matchQueueType == 0x1)//普通立即竞速
				Client->TaskID = 0xFFFB;
			Client->GameID = 2;
			Map* pMap = GetMap(BorderRoom->MapID);
			if (pMap)
			{
				Client->MapCompatibilityMode = false;
				Client->EndCheckPoint = pMap->End;
				Client->TotalRound = pMap->Round;
				Client->Loop = pMap->Loop;
			}
			else
			{
				Client->MapCompatibilityMode = true;
				Client->EndCheckPoint = 0;
				Client->TotalRound = 1;
				Client->Loop = false;
			}
			Client->MapID = BorderRoom->MapID;
			Client->FinishTime = 0;
			Client->Round = 0;
			Client->PassedCheckPoint = 0;
			Client->IsFinish = FALSE;
			Client->IsReady = TRUE;
			Client->BorderRoomID = BorderRoom->ID;
			for (char i2 = 1; i2 < 6; i2++)
			{
				Client->NpcFinishInfo[i2].NpcID = i2;//初始化NPCID
			}
			break;
			break;
		}
	}
	CreateRoomTimer(BorderRoom, 1000, OnMatchResult);

}


void ResponseStartRankedGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID)
{
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);
	ResponseAddToMatchQueue(Client, matchQueueType);

	Client->TaskID = 0xFFFC;
	std::lock_guard<std::recursive_mutex> lg(RankedQueueMutex);
	//RankedMatchQueue.push_front(Client);
	auto now = std::chrono::steady_clock::now();
	RankedMatchQueue.push_front({ Client, now });
}

void ResponseStartBorderGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID)
{
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);
	ResponseAddToMatchQueue(Client, matchQueueType);

	Client->TaskID = 0xFFFD;
	std::lock_guard<std::recursive_mutex> lg(BorderQueueMutex);
	//BorderMatchQueue.push_front(Client);

	auto now = std::chrono::steady_clock::now();
	BorderMatchQueue.push_front({ Client, now });

}

void ResponseStartQuickMatchGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID)
{
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);
	ResponseAddToMatchQueue(Client, matchQueueType);
	//单人

	std::shared_ptr<RoomNode> BorderRoom = NewBorderRoom();

	if (!BorderRoom)
	{
		return;
	}
	Client->BorderRoomID = BorderRoom->ID;
	if (matchQueueType == 0xa)
	{
		BorderRoom->BaseGameMode = 0x28;
		BorderRoom->SubGameMode = 1;
	}
	else
	{
		BorderRoom->BaseGameMode = BorderRoom->SubGameMode = 1;
	}

	//随机地图
	BorderRoom->matchQueueType = matchQueueType;
	BorderRoom->MapID = GetRandMap(16);//全随地图池
	Client->BaseGameMode = 1;
	Client->SubGameMode = 1;
	if (matchQueueType == 12)
	{
		Client->BaseGameMode = 17;
	}

	//BorderRoom->MapID = 112;//全随地图池
	//加入房间
	memset(Client->NpcFinishInfo, 0, sizeof(NpcRacerFinishInfo) * NpcNum);
	for (char i = 0; i < 6; i++)
	{
		if (!BorderRoom->Player[i])//空位
		{

			BorderRoom->Player[i] = Client;
			BorderRoom->CurrentPlayerNum++;
			if (MapID != 0)
			{
				BorderRoom->MapID = MapID;
			}
			if (BorderRoom->matchQueueType == 0)
				Client->TaskID = 0xFFFD;
			else if (BorderRoom->matchQueueType == 0xa)
				Client->TaskID = 0xFFFC;
			else if (BorderRoom->matchQueueType == 0x1)//普通立即竞速
				Client->TaskID = 0xFFFB;
			Client->GameID = 2;
			Map* pMap = GetMap(BorderRoom->MapID);
			if (pMap)
			{
				Client->MapCompatibilityMode = false;
				Client->EndCheckPoint = pMap->End;
				Client->TotalRound = pMap->Round;
				Client->Loop = pMap->Loop;
			}
			else
			{
				Client->MapCompatibilityMode = true;
				Client->EndCheckPoint = 0;
				Client->TotalRound = 1;
				Client->Loop = false;
			}
			Client->MapID = BorderRoom->MapID;
			Client->FinishTime = 0;
			Client->Round = 0;
			Client->PassedCheckPoint = 0;
			Client->IsFinish = FALSE;
			Client->IsReady = TRUE;
			Client->BorderRoomID = BorderRoom->ID;
			for (char i2 = 1; i2 < 6; i2++)
			{
				Client->NpcFinishInfo[i2].NpcID = i2;//初始化NPCID
			}
			break;
			break;
		}
	}
	CreateRoomTimer(BorderRoom, 1000, OnMatchResult);

}

void ResponseStartCheShenGame(std::shared_ptr<ClientNode> Client, USHORT matchQueueType, UINT MapID)
{
	RequestCancelMatch(Client);
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);
	ResponseAddToMatchQueue(Client, matchQueueType);
	//单人

	std::shared_ptr<RoomNode> BorderRoom = NewBorderRoom();

	if (!BorderRoom)
	{
		return;
	}
	Client->BorderRoomID = BorderRoom->ID;
	if (matchQueueType == 0xa)
	{
		BorderRoom->BaseGameMode = 0x28;
		BorderRoom->SubGameMode = 1;
	}
	else
	{
		BorderRoom->BaseGameMode = BorderRoom->SubGameMode = 1;
	}

	//随机地图
	BorderRoom->matchQueueType = matchQueueType;
	BorderRoom->MapID = MapID;
	Client->BaseGameMode = 1;
	Client->SubGameMode = 1;
	if (matchQueueType == 12)
	{
		Client->BaseGameMode = 17;
	}

	//BorderRoom->MapID = 112;//全随地图池
	//加入房间
	memset(Client->NpcFinishInfo, 0, sizeof(NpcRacerFinishInfo) * NpcNum);
	for (char i = 0; i < 6; i++)
	{
		if (!BorderRoom->Player[i])//空位
		{

			BorderRoom->Player[i] = Client;
			BorderRoom->CurrentPlayerNum++;
			if (MapID != 0)
			{
				BorderRoom->MapID = MapID;
			}
			if (BorderRoom->matchQueueType == 0)
				Client->TaskID = 0xFFFD;
			else if (BorderRoom->matchQueueType == 0xa)
				Client->TaskID = 0xFFFC;
			else if (BorderRoom->matchQueueType == 0x1)//普通立即竞速
				Client->TaskID = 0xFFFB;
			Client->GameID = 0x000001D8;
			Map* pMap = GetMap(BorderRoom->MapID);
			if (pMap)
			{
				Client->MapCompatibilityMode = false;
				Client->EndCheckPoint = pMap->End;
				Client->TotalRound = pMap->Round;
				Client->Loop = pMap->Loop;
			}
			else
			{
				Client->MapCompatibilityMode = true;
				Client->EndCheckPoint = 0;
				Client->TotalRound = 1;
				Client->Loop = false;
			}
			Client->MapID = BorderRoom->MapID;
			Client->FinishTime = 0;
			Client->Round = 0;
			Client->PassedCheckPoint = 0;
			Client->IsFinish = FALSE;
			Client->IsReady = TRUE;
			Client->BorderRoomID = BorderRoom->ID;
			for (char i2 = 1; i2 < 6; i2++)
			{
				Client->NpcFinishInfo[i2].NpcID = i2;//初始化NPCID
			}
			break;
		}
	}
	CreateRoomTimer(BorderRoom, 1000, OnMatchResult);

}



void StartSingleGame(std::shared_ptr<ClientNode> Client, BYTE* Body)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	//const char* buf2;
	int TaskID = Read32(Body);
	UINT MapID = Read32(Body);
	Client->TaskID = TaskID;
	if (TaskID != 0x06a5)
	{
		RequestCancelMatch(Client);
		LeaveRoom(Client, 0);
		LeaveBorderRoom(Client, 0);
		LeaveOutdoor(Client);
	}
	//printf("TaskID:%d\n", Client->TaskID);
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		MapID = GetRandMap(0);
	}
	else
	{
		if (Room->MapID < 100)
		{
			MapID = GetRandMap(0);
		}
		else
		{
			MapID = Room->MapID;
		}
	}

	if (TaskID == 0x49) //暴风随机
	{
		//int i = rand() % 5 ;
		MapID = baofengmap[Client->OptIndex];
		if (Client->OptIndex <= 0)
			Client->OptIndex = 4;
		else
			Client->OptIndex -= 1;


	}
	Map* pMap = GetMap(MapID);
	Client->GameID = 0;
	Client->IsReady = TRUE;
	if (pMap)
	{
		Client->MapCompatibilityMode = false;
		Client->EndCheckPoint = pMap->End;
		Client->TotalRound = pMap->Round;
		Client->Loop = pMap->Loop;
	}
	else
	{
		Client->MapCompatibilityMode = true;
		Client->EndCheckPoint = 0;
		Client->TotalRound = 1;
		Client->Loop = false;
	}
	Client->MapID = MapID;
	Client->FinishTime = 0;
	Client->Round = 0;
	Client->PassedCheckPoint = 0;
	if (TaskID == 0x06a5)
	{
		//int MapID = Read32(Body);
		//UCHAR Level = Read8(Body);
		//printf("TaskID：%d MapID：%d\n", TaskID, MapID);
		Write32(p, 0xea60); //NormalLoadTime
		Write32(p, TaskID); //TaskID
		Write32(p, MapID); //MapID
		//MapHash[]
		memset(p, 0, 32);
		p += 32;
		Write32(p, 0); //GameID
		Write32(p, (DWORD)time(nullptr)); //Seed
		Write16(p, Client->TotalRound); //TotalMapRound pMap可能空指针
		Write32(p, 0); //TimeLimit
		Write32(p, 0); //MaxNPCDistance
		Write16(p, 0); //InitPropNum
		Write16(p, 0); //NpcNum

		Write32(p, 0); //PlayerItemNum

		Write8(p, 0); //SyncCarNum

		Write8(p, 0); //PlayerNum

		Write8(p, 0); //FeedBackEnabled
		Write16(p, 0); //PropID
		Write32(p, 0); //PropIndex
		Write16(p, 0); //PropNum
		Write16(p, 0); //LogicType
		Write8(p, 0); //MapCheckpointFileIndex
		Write32(p, 0); //Duration
		Write32(p, 0); //BeginCDTime
		Write32(p, 0); //PropInteval
		len = p - buf.data();
		SendToClient(Client, 530, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
	}
	else if (TaskID == 0x49)
	{
		BYTE* pp = baofengtiaozhan;
		pp += 8;
		//rand()%(b-a+1) +a 
		Write32(pp, MapID);
		pp += 40;
		Write16(pp, Client->TotalRound);
		SendToClient(Client, 530, baofengtiaozhan, 1928, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
	}
	else if (TaskID == 0x6e)//树苗
	{
		BYTE* pp = shumiaoyunshu;
		pp += 8;
		Write32(pp, MapID);
		pp += 40;
		Write16(pp, Client->TotalRound);
		SendToClient(Client, 530, shumiaoyunshu, 98, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
	}
	/*else if (TaskID == 0x48)//车王考验
	{
	BYTE* pp = chewangkaoyan;
	pp += 8;
	Write32(pp, MapID);
	pp += 40;
	Write16(pp, Client->TotalRound);
	Write32(pp, 0x2FFFD);//timelimit
	SendToClient(Client, 530, chewangkaoyan, 98, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);

	}*/


	//NotifyKartPhysParam(Client, NULL);
	if (mode28346 == 2) {
		NotifyKartPhysParam(Client, Room->Player);
	}
	else {
		NotifyEncryptedMsg(Client);
	}

	SendToClient(Client, 531, (BYTE*)"\x00\x00", 2, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);

}

void WINAPI OnStart(std::shared_ptr<RoomNode> Room)
{

}

void RequestStartGame(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);



	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}

	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient && RoomClient->ConnID != Room->RoomOwnerID)
		{
			if (RoomClient->ReadyState == 1)
			{
				ResponseStartGame(Client, "玩家未准备!");
				return;
			}
		}
	}
	if (Room->Callback)
	{
		ResponseStartGame(Client, "游戏进行中,无法开始!");
		return;//防止重复点击开始bug
	}

	if (Room->IsStake) {
		if (Room->BaseGameMode == 41 || Room->BaseGameMode == 4) {
			ResponseStartGame(Client, "只允许普通开房进行下注比赛！");
			return;
		}
		if (Room->CurrentPlayerNum != 2) {
			ResponseStartGame(Client, "当前房间属于下注模式，只允许双人开始游戏");
			return;
		}
		int commonStakeNum = -1; // 默认值，用于检查
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				if (commonStakeNum == -1) {
					// 初始化 commonStakeNum
					commonStakeNum = RoomClient->StakeNum;
				}
				else if (commonStakeNum != RoomClient->StakeNum) {
					ResponseStartGame(Client, "当前房间属于下注模式，双方下注数量不一致，请重新下注！");
					return; // 或者其他处理
				}
			}
		}

		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				int Itemnum = PlayerDB_GetItemNum(RoomClient->Uin, Stake[1]);
				if (Itemnum - commonStakeNum <= 0) {
					std::string nickNameStr(RoomClient->NickName, std::strlen(RoomClient->NickName));
					std::string msg = "玩家/C01【" + nickNameStr + "】/C00当前道具数量为：/C01" + std::to_string(Itemnum) + "/C00，下注数量为：/C01" + std::to_string(commonStakeNum) + "/C00，道具不足，无法开始游戏！";
					ResponseStartGame(Client, msg.c_str());
					return;
				}
			}
		}
	}

	if (Room->IsBottomGame) {

		if (Room->BaseGameMode == 41 || Room->BaseGameMode == 4) {
			ResponseStartGame(Client, "只允许普通开房进行垫底模式或者尾数模式比赛！");
			return;
		}

		//尾数
		if (Room->IsBottomGame == 9) {
			UINT Itemnum = 0;
			for (char i = 0; i < 6; i++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
				if (RoomClient && RoomClient != Client)
				{
					if (RoomClient->MantissaNum == 0) {
						std::string nickNameStr(RoomClient->NickName, std::strlen(RoomClient->NickName));
						std::string msg = "玩家/C01【" + nickNameStr + "】/C00未下注，无法开始游戏！";
						ResponseStartGame(Client, msg.c_str());
						return;
					}
					if (RoomClient->OBState == 0) {
						std::string nickNameStr(RoomClient->NickName, std::strlen(RoomClient->NickName));
						std::string msg = "玩家/C01【" + nickNameStr + "】/C00未开启观战，无法开始游戏！";
						ResponseStartGame(Client, msg.c_str());
						return;
					}
					int RoomClientNum = PlayerDB_GetItemNum(RoomClient->Uin, Mantissa[3]);
					if (RoomClientNum < RoomClient->MantissaNum) {
						std::string nickNameStr(RoomClient->NickName, std::strlen(RoomClient->NickName));
						std::string msg = "玩家/C01【" + nickNameStr + "】/C00数量不足，无法开始尾数游戏！";
						ResponseStartGame(Client, msg.c_str());
						return;
					}
					Itemnum += RoomClient->MantissaNum;
				}
			}
			int ClientNum = PlayerDB_GetItemNum(Client->Uin, Mantissa[3]);
			if (ClientNum < Itemnum) {
				std::string msg = "总共下注数量：/C01" + std::to_string(Itemnum) + " /C00你的数量为：/C01" + std::to_string(ClientNum) + " /C00数量不足，无法开始游戏！";
				ResponseStartGame(Client, msg.c_str());
				return;
			}

			if (Room->CurrentPlayerNum < 2) {
				ResponseStartGame(Client, "当前房间属于尾数模式，人数不够！");
				return;
			}
		}
		else {
			if (Room->CurrentPlayerNum != 4 && Room->IsBottomGame == 4) {
				ResponseStartGame(Client, "当前房间属于四人垫底模式，人数不准确！");
				return;
			}

			if (Room->CurrentPlayerNum != 6 && Room->IsBottomGame == 6) {
				ResponseStartGame(Client, "当前房间属于六人垫底模式，人数不准确！");
				return;
			}

			if (Room->CurrentPlayerNum < 5 && Room->IsBottomGame == 7) {
				ResponseStartGame(Client, "当前房间属于竞技模式，人数不准确！");
				return;
			}
			if (Room->CurrentPlayerNum < 2) {
				ResponseStartGame(Client, "当前房间属于垫底模式，人数不够！");
				return;
			}


			UINT BottomItemNum = 0;
			if (Room->IsBottomGame != 7) {
				if (Room->IsBottomGame == 4) {
					BottomItemNum = IsBottomGame4or6[0];
				}
				else if (Room->IsBottomGame == 6) {
					BottomItemNum = IsBottomGame4or6[1];
				}
				else
				{
					BottomItemNum = Room->IsBottomGame == 1 ? IsBottomGame[2] : Room->BottomGameNum;
				}


				for (char i = 0; i < 6; i++)
				{
					std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
					if (RoomClient)
					{
						int Itemnum = PlayerDB_GetItemNum(RoomClient->Uin, IsBottomGame[1]);
						if (Itemnum <= BottomItemNum) {
							std::string nickNameStr(RoomClient->NickName, std::strlen(RoomClient->NickName));
							std::string msg = "玩家/C01【" + nickNameStr + "】/C00当前道具数量为：/C01" + std::to_string(Itemnum) + "/C00，垫底数量为：/C01" + std::to_string(BottomItemNum) + "/C00，道具不足，无法开始游戏！";
							ResponseStartGame(Client, msg.c_str());
							return;
						}
					}
				}
			}
		}

		
	}



	Room->Callback = OnStart;//设置回调
	Room->ranknumber = 0;//排名
	Room->CountDownFlag = 0;
	Room->startStatus = 2;
	if (Room->BaseGameMode == 41)
	{
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient && RoomClient->GameBan)
			{
				std::string nickNameStr(RoomClient->NickName, std::strlen(RoomClient->NickName));
				std::string msg = "玩家/C01【" + nickNameStr + "】/C00已经被禁止跑边境排位！";
				ResponseStartGame(Client, msg.c_str());
				return;
			}
		}

		std::lock_guard<std::recursive_mutex> lg(BorderQueueMutex);
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			//InitClientGameArrt(RoomClient);
			if (RoomClient)
			{
				auto now = std::chrono::steady_clock::now();
				ResponseAddToMatchQueue(RoomClient, 0xa);
				RoomClient->TaskID = 0xFFFD;
				BorderMatchQueue.push_front({ RoomClient, now });
			}
		}
		return;
	}


	if (Room->BaseGameMode == 4) //舞蹈
	{
		if (Room->SceneID == 0)
		{
			Room->SceneID = 17;
		}

		if (Room->MusicID < 10)
		{
			Room->MusicID = 35;
		}

		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				RoomClient->GameID = 1;
				RoomClient->IsReady = FALSE;
				RoomClient->IsFinish = FALSE;

				RoomClient->Scores = 0;
				NotifyDanceGameBegin(RoomClient, Room);
			}
		}
		CreateRoomTimer(Room, DanceMusicDuration(Room->MusicID), OnDanceTimer);
	}
	else
	{
		UINT MapID = Room->MapID;
		if (MapID < 100) //随机
		{
			MapID = GetRandMap(MapID);
		}
		if (MapID == 335 || MapID == 419 || MapID == 155 || MapID == 156)
		{
			ResponseStartGame(Client, "该地图无法开始游戏!");
			return;
		}
		Map* pMap = GetMap(MapID);
		if (!pMap)
		{
			printf("请配置 MapID:%d\n", MapID);
			fflush(stdout);
		}

		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{


				//RoomClient->AutoLeave = 0;		
				RoomClient->GameID = 2;
				RoomClient->IsReady = FALSE;
				RoomClient->IsFinish = FALSE;
				RoomClient->IsRefitItem = false;
				if (pMap)
				{
					RoomClient->MapCompatibilityMode = false;
					RoomClient->EndCheckPoint = pMap->End;
					RoomClient->TotalRound = pMap->Round;
					RoomClient->Loop = pMap->Loop;
				}
				else
				{
					RoomClient->MapCompatibilityMode = true;
					RoomClient->EndCheckPoint = 0;
					RoomClient->TotalRound = 1;
					RoomClient->Loop = false;
				}
				RoomClient->MapID = MapID;
				RoomClient->FinishTime = 0;
				RoomClient->Round = 0;
				RoomClient->PassedCheckPoint = 0;
				RoomClient->MsgSequence = -1;
				if (RoomClient->GlobalStone == 1)
				{
					NotifyUpdateKartSkillStoneInfoConfig(RoomClient, RoomClient->KartID);
					//NotifyUpdateKartSkillStoneInfo2(RoomClient, RoomClient->KartID);
				}

				if (Room->ChatRoomType == 6)
				{
					LeaveOutdoor(RoomClient);//离开3d房休闲区防止卡起步
				}
				NotifyGameBegin(RoomClient, Room->Player);
				//发送kartphys之前等待所有人准备 同步跳舞动作 跳舞动作必须一样 否则卡起步

				//if (mode28346 == 2) {
				//	NotifyKartPhysParam(RoomClient, Room->Player);
				//}
				//else {
				//	NotifyEncryptedMsg(RoomClient);
				//	//NotifyKartPhysParam(RoomClient, Room->Player);
				//}


			}
		}
		CreateRoomTimer(Room, SyncTime, OnPrepareTest);//验证准备状态
	}


}
void ResponseStartGame(std::shared_ptr<ClientNode> Client, const char* Reason)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, -1); //ResultID

	len = strlen(Reason);
	Write8(p, (BYTE)len); //ReasonLen
	memcpy(p, Reason, len);
	p += len;

	len = p - buf.data();
	SendToClient(Client, 110, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}


void RequestChangeMap(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	UINT NewMapID = Read32(Body);
	//printf("mapid:%d\n", NewMapID);

	/*
m_iMapSubID
	*/
	char Season = -1;
	char Timeslot = -1;
	char Weather = -1;
	{
		BYTE* pSTDynamicInfo = Body;
		USHORT len = Read16(pSTDynamicInfo);

		Season = Read8(pSTDynamicInfo);
		Timeslot = Read8(pSTDynamicInfo);
		Weather = Read8(pSTDynamicInfo);

		Body += len;
	}

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	Room->MapID = NewMapID;
	Room->STDynamicInfo.Season = Season;
	Room->STDynamicInfo.Timeslot = Timeslot;
	Room->STDynamicInfo.Weather = Weather;

	ResponseChangeMap(Client, Room);

	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient != Client)
			{
				NotifyChangeMap(RoomClient, Client->Uin, Room);
			}
		}
	}


}
void ResponseChangeMap(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Room->MapID); //NewMapID
	Write8(p, 0); //ReasonLen
	Write8(p, 0); //UnLockMapCondDescNum
	//m_astUnLockMapCondDesc[].m_bDescLen

	{ //STDynamicInfo
		BYTE* pSTDynamicInfo = p;
		Write16(pSTDynamicInfo, 0); //len

		Write8(pSTDynamicInfo, Room->STDynamicInfo.Season);
		Write8(pSTDynamicInfo, Room->STDynamicInfo.Timeslot);
		Write8(pSTDynamicInfo, Room->STDynamicInfo.Weather);

		len = pSTDynamicInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //PreKnowMapID
	Write32(p, 0); //MapSubID
#ifndef ZingSpeed
	Write8(p, 0); //FirstlyTipMap
#endif

	len = p - buf.data();
	SendToClient(Client, 121, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Response);
}
void NotifyChangeMap(std::shared_ptr<ClientNode> Client, UINT Uin, std::shared_ptr<RoomNode> Room)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Uin); //Uin
	Write32(p, Room->MapID); //NewMapID

	{ //STDynamicInfo
		BYTE* pSTDynamicInfo = p;
		Write16(pSTDynamicInfo, 0); //len

		Write8(pSTDynamicInfo, Room->STDynamicInfo.Season);
		Write8(pSTDynamicInfo, Room->STDynamicInfo.Timeslot);
		Write8(pSTDynamicInfo, Room->STDynamicInfo.Weather);

		len = pSTDynamicInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //PreKnowMapID
	Write32(p, 0); //MapSubID
#ifndef ZingSpeed
	Write8(p, 0); //FirstlyTipMap
#endif

	len = p - buf.data();
	SendToClient(Client, 520, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Notify);
}

void NotifyChangeSeatState(std::shared_ptr<ClientNode> Client, char SeatID, char SeatState)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write8(p, SeatID); //SeatID
	Write8(p, SeatState); //SeatState

	len = p - buf.data();
	SendToClient(Client, 502, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestChangeSeatState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	short RoomID = Read16(Body);
	BYTE SeatID = Read8(Body);
	BYTE SeatState = Read8(Body);//0关闭 1开放

	std::shared_ptr<RoomNode> Room = GetRoom(RoomID);
	if (!Room)
	{
		return;
	}
	if (Client->ConnID != Room->RoomOwnerID)
	{
		return;
	}
	if (SeatState == 1)
	{
		Room->SeatState[SeatID] = 1;
		Room->SeatNum++;
	}
	else
	{
		Room->SeatState[SeatID] = 0;
		Room->SeatNum--;
	}
	ResponseChangeSeatState(Client, SeatID, SeatState);

	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient != Client)
			{
				NotifyChangeSeatState(RoomClient, SeatID, SeatState);
			}
		}
	}

}
void ResponseChangeSeatState(std::shared_ptr<ClientNode> Client, BYTE SeatID, BYTE SeatState)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write8(p, SeatID);
	Write8(p, SeatState);

	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 107, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}

void NotifyKickFromRoom(std::shared_ptr<ClientNode> Client, UINT Uin, USHORT RoomID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, RoomID); //RoomID
	Write32(p, Uin); //DstUin
	Write16(p, 0); //ReasonID
	Write8(p, 0); //ReasonLen
	Write16(p, 0); //BaseGameMode

	len = p - buf.data();
	SendToClient(Client, 503, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestKickFromRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	UINT DstUin = Read32(Body);
	BYTE KickReason = Read8(Body);

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	if (Client->ConnID != Room->RoomOwnerID)
	{
		return;
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient->Uin == DstUin)
			{
				if (RoomClient->admin == 1)
				{
					ResponseStartGame(Client, "他是尊敬的管理员，你没有权限踢他！");
					return;
				}

				//if (RoomClient->OBState == 1)
				//{
				//	ResponseStartGame(Client, "无法踢出观战者!");
				//	return;
				//}
				ResponseKickFromRoom(Client, DstUin);//踢出房间
				NotifyKickFromRoom(RoomClient, DstUin, Room->ID);
				//LeaveRoom(RoomClient, 0);
				//LeaveOutdoor(RoomClient);
				if (Room->ChatRoomType == 0)
				{
					ResponseStartGame(RoomClient, "你已被踢出房间!");
				}

			}
		}
	}
}
void ResponseKickFromRoom(std::shared_ptr<ClientNode> Client, UINT DstUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write32(p, DstUin);

	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 108, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}


void NotifyChangeReadyState(std::shared_ptr<ClientNode> Client, UINT Uin, bool State)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Uin);
	Write8(p, State);

	len = p - buf.data();
	SendToClient(Client, 504, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Notify);
}
void RequestChangeReadyState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	bool ReadyState = Read8(Body);//准备是0 取消准备是1

	ResponseChangeReadyState(Client, ReadyState);
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	if (ReadyState == 0)
	{
		Client->ReadyState = 0;
	}
	else if (ReadyState == 1)
	{
		Client->ReadyState = 1;
	}
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			if (RoomClient != Client)
			{
				NotifyChangeReadyState(RoomClient, Client->Uin, ReadyState);
			}
		}
	}
}
void ResponseChangeReadyState(std::shared_ptr<ClientNode> Client, bool ReadyState)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write8(p, ReadyState);

	Write8(p, 0); //ReasonLen


	len = p - buf.data();
	SendToClient(Client, 109, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}


// 自定义函数，将字符串分割成两部分，返回后面部分的UINT值
uint32_t splitAndConvert(const std::string& content) {
	// 寻找分割点
	size_t pos = content.find("kick");
	if (pos == std::string::npos) {
		// 如果找不到 "kick"，则返回默认值或者抛出异常，视情况而定
		return 0; // 这里返回0作为示例，你可以根据实际情况选择其他处理方式
	}

	// 提取后面的部分
	std::string secondPart = content.substr(pos + 4); // "+4" 是因为 "kick" 长度为4

	// 将后面的部分转换为UINT
	uint32_t value;
	std::istringstream iss(secondPart);
	iss >> value;
	return value;
}


bool isNumeric(const std::string& str) {
	// 定义匹配的正则表达式
	std::regex pattern("^[0-9]{5,6}$");

	// 进行匹配
	return std::regex_match(str, pattern);
}

std::string getDayOfWeekString(int dayOfWeek) {
	switch (dayOfWeek) {
	case 0: return "星期日";
	case 1: return "星期一";
	case 2: return "星期二";
	case 3: return "星期三";
	case 4: return "星期四";
	case 5: return "星期五";
	case 6: return "星期六";
	default: return "Unknown";
	}
}


bool isInteger(const char* str) {
	if (str == nullptr || *str == '\0') {
		return false;
	}

	// 处理可能的负号
	if (*str == '-') {
		str++;
		if (*str == '\0') return false; // 只包含负号的情况
	}

	// 检查剩余的字符是否全是数字
	while (*str) {
		if (!std::isdigit(static_cast<unsigned char>(*str))) {
			return false;
		}
		str++;
	}

	return true;
}


void TalkStake(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, char* Content) {
	try
	{
		char* Num = Content;
		Num += 4;
		if (isInteger(Num)) {
			int Mynum = PlayerDB_GetItemNum(Client->Uin, Stake[1]);
			if (Mynum < atoi(Num)) {
				std::string msg = GetItemName(Stake[1]);
				msg = "下注道具目前设置为/C01【" + msg + "】/C00您的数量只有/C01【" + std::to_string(Mynum) + "】/C00道具不足！";
				ResponseStartGame(Client, msg.c_str());
				return;
			}
			Client->IsStake = true;
			Client->StakeNum = atoi(Num);
			Room->IsStake = 1;
			ResponseStartGame(Client, "下注成功，请通知对方一起下注相同数量，否则无法开始游戏！");
		}
		else {
			ResponseStartGame(Client, "下注格式异常，只允许输入数字！");
			return;
		}

	}
	catch (...)
	{
		printf("赌注异常\n");
	}
}

void TalkMantissa(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, char* Content) {
	try
	{
		std::string str(Content);
		std::string thirdPart;
		std::string msg;

		
		// 检查 Content 的长度是否满足要求
		if (str.length() >= 6) { // 假设至少需要 6 个字符
			// 提取第三个部分（假设第三个字符就是我们需要的部分）
			thirdPart = str.substr(4, 2); // 从索引 3 开始提取 1 个字符
		}

		if (!thirdPart.empty()) {
			if (thirdPart == "单") {
				Client->Mantissa = 1;
			}
			else if (thirdPart == "双")
			{
				Client->Mantissa = 2;
			}
			else if (thirdPart == "大") {
				Client->Mantissa = 3;
			}
			else if (thirdPart == "小") {
				Client->Mantissa = 4;
			}
			else {
				msg = "尾数格式异常，可以输入/C01尾数单20，尾数双20，尾数大20，尾数小20/C00";
				ResponseStartGame(Client, msg.c_str());
				return;
			}
		}
		else {
			msg = "尾数格式异常，可以输入/C01尾数单20，尾数双20，尾数大20，尾数小20/C00";
			ResponseStartGame(Client, msg.c_str());
			return;
		}

		char* Num = Content;
		Num += 6;
		if (isInteger(Num)) {
			int itemnum = atoi(Num);
			if (itemnum < Mantissa[1] || itemnum > Mantissa[2]) {
				msg = "下注数量不在范围之内，最低/C01" + std::to_string(Mantissa[1]) + "/C00最高/C01" + std::to_string(Mantissa[2]);
				ResponseStartGame(Client, msg.c_str());
				return;
			}

			int Mynum = PlayerDB_GetItemNum(Client->Uin, Mantissa[3]);
			if (Mynum < itemnum) {
				msg = GetItemName(Mantissa[3]);
				msg = "尾数下注道具目前设置为/C01【" + msg + "】/C00您的数量只有/C01【" + std::to_string(Mynum) + "】/C00道具不足！";
				ResponseStartGame(Client, msg.c_str());
				return;
			}
			Client->MantissaNum = itemnum;
			ResponseStartGame(Client, "尾数下注成功！");
		}
		else {
			msg = "尾数格式异常，可以输入/C01尾数单20，尾数双20，尾数大20，尾数小20/C00";
			ResponseStartGame(Client, msg.c_str());
			return;
		}

	}
	catch (...)
	{
		printf("尾数异常\n");
	}
}









void TalkBottomGame(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, char* Content) {
	try
	{
		//d::string Content = "开启2人扣数量10";
		int playerCount = 0;
		int damageAmount = 0;

		// 将 char* 转换为 std::string
		std::string strContent(Content);

		// 使用正则表达式匹配特定格式
		std::regex regexPattern(R"(开启(\d+)人扣数量(\d+))");
		std::smatch matches;

		// 尝试匹配内容
		if (std::regex_match(strContent, matches, regexPattern)) {
			// 提取玩家数量和扣血数量
			playerCount = std::stoi(matches[1]);
			damageAmount = std::stoi(matches[2]);

			if (playerCount > 0 && damageAmount > 0) {
				//// 输出结果
				//std::cout << "玩家数量: " << playerCount << std::endl;
				//std::cout << "扣血数量: " << damageAmount << std::endl;
				Room->IsBottomGame = 3;
				Room->BottomGameNum = damageAmount;
				Room->BottomPlayerNum = playerCount;
			}
		}
		else {
			ResponseStartGame(Client, "垫底格式异常！");
		}


	}
	catch (...)
	{
		printf("垫底说话异常\n");
	}
}

std::string extractQQ(const char* content) {
	// 找到 "绑定qq" 的位置
	const char* prefix = "绑定qq";
	const char* start = std::strstr(content, prefix);

	// 如果没有找到返回空字符串
	if (!start) return "";

	// 移动指针到 QQ 号的起始位置
	start += std::strlen(prefix);

	// 读取数字直到遇到非数字字符或达到十位
	std::string qq;
	while (std::isdigit(*start) && qq.size() < 10) {
		qq += *start;
		start++;
	}

	return qq;
}


// 函数：将时间字符串转换为 time_t 类型
std::time_t stringToTimeT(const std::string& timeStr) {
	std::tm tm = {};
	std::istringstream ss(timeStr);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	return std::mktime(&tm);
}

// 函数：将 time_t 类型的时间转换为字符串
std::string timeTToString(std::time_t time) {
	struct tm* tm_now;
	localtime_s(tm_now, &time);
	std::ostringstream oss;
	oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

bool contains(const std::string& str, const std::string& substr) {
	return str.find(substr) != std::string::npos;
}

std::string getTimeDifferenceString(std::time_t futureTime, std::time_t currentTime) {
	double diffSeconds = std::difftime(futureTime, currentTime);

	int days = static_cast<int>(diffSeconds) / 86400;
	diffSeconds -= days * 86400;
	int hours = static_cast<int>(diffSeconds) / 3600;
	diffSeconds -= hours * 3600;
	int minutes = static_cast<int>(diffSeconds) / 60;

	std::ostringstream result;
	result << days << " 天, " << hours << " 小时, " << minutes << " 分钟";
	return result.str();
}

void RequestTalk(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//请求说话
	UINT Uin = Read32(Body);               // 用户ID
	UINT Time = Read32(Body);              // 时间戳
	BYTE Type = Read8(Body);               // 消息类型
	BYTE ClientUseFlag = Read8(Body);      // 客户端使用标识
	UINT DestPlayerUin = Read32(Body);     // 目标玩家的用户ID

	USHORT ContentLength = Read16(Body);   // 消息内容长度
	char* Content = (char*)Body;           // 消息内容
	Body += ContentLength;
	//printf("Content:%s\n", Content);
	BYTE BugletType = Read8(Body);         // 喇叭类型
	BYTE ReserveFlag = Read8(Body);        // 保留标识

	// 检查用户是否被禁言
	if (Client->Speaking) {
		ResponseStartGame(Client, "你已经被禁言！");
		return;
	}

	// 管理员发送物品指令
	if (strncmp(Content, "give", 4) == 0 && Client->admin == 1) {
		if (SendAnyItem(Client, Content) > 0) {
			ResponseStartGame(Client, "赠送成功！");
		}
		return;
	}
	// 管理员踢出指定玩家
	if (Client->admin == 1) {
		UINT DestUin = splitAndConvert(Content);
		if (DestUin > 0 && DestUin != Client->Uin)
		{
			std::shared_ptr<ClientNode> DestClient = GetClient(DestUin);
			if (DestClient) {
				if (DestUin == 10001 || DestUin == 10002 || DestUin == 10003) {
					ResponseStartGame(Client, "禁止踢管理下线!");
					return;
				}
				else
				{
					NotifyKickPlayer(DestClient, "你已经被管理员踢下线", 3);
					//GetClient(DestUin)->deleteflag = 1;
					DestClient->IsLogin = FALSE;
					DestClient->Server->Disconnect(DestClient->ConnID);
					DestClient->deleteflag = 1;
					return;
				}

			}
		}
	}

	// 处理退出游戏请求
	if (Type == 1 && ClientUseFlag == 0 && _stricmp(Content, "g") == 0)
	{
		if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
			return;
		}
		//手动发送GameOver消息
		/*if (Client->TaskID == 0xFFFE)
		{ //极限挑战强制退出
			Client->IsFinish = TRUE;
			HANDLE over = CreateThread(NULL, 0, OnSingleOver, Client, 0, NULL);
			if (over)
			{
				CloseHandle(over);
			}
		}*/
		if ((Client->TaskID == 0 || Client->TaskID == 0xFFFD || Client->TaskID == 0xFFFC || Client->TaskID == 0xFFFB) && Client->GameID != 0)
		{//排位 边境 竞速快速退出
			QuickbugLeaveGame(Client);
		}
		return;
	}
	//ResponseTalk(Client, Type, ClientUseFlag, DestPlayerUin);

	// 喇叭消息处理
	else if (Type == 3)
	{
		UINT ItemID = 0;
		switch (BugletType) {
		case 0: ItemID = 10207; break;
		case 1: ItemID = 14727; break;
		case 2: ItemID = 14728; break;
		case 3: ItemID = 18817; break;
		case 12: ItemID = 66641; break;
		}

		if (isDBtHasItem(Client, ItemID, 1))
		{
			NotifyUseItemResult2(Client, ItemID, 1);
			NotifyBugletTalk(Client, Content, ContentLength, BugletType);
		}
		else
		{
			ResponseStartGame(Client, "喇叭数量不足!");
		}
		return;
	}
	//printf("Type:%d, ClientUseFlag:%d. DestPlayerUin:%d BugletType:%d\n", Type, ClientUseFlag, DestPlayerUin, BugletType);
	//房间内说话Type:4, ClientUseFlag : 2
	//对局内说话Type:1, ClientUseFlag:0
	// 房间内聊天或对局聊天
	else if (Type == 1 || Type == 2 || Type == 4)
	{
		std::shared_ptr<RoomNode> Room = NULL;
		if (Client->TaskID == 0xFFFD || Client->TaskID == 0xFFFC) {
			Room = GetBorderRoom(Client->BorderRoomID);
		}
		else {
			Room = GetRoom(Client->RoomID);
		}
		if (!Room)
		{
			return;
		}



		// 处理房间内特定指令
		if (ClientUseFlag == 2) {
			// 切换观战模式
			if (strcmp(Content, "obs") == 0) {
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				if (Client->OBState == 1) {
					ResponseStartGame(Client, "观战模式关闭");
					Client->OBState = 0;
				}
				else {
					ResponseStartGame(Client, "观战模式开启");
					Client->OBState = 1;
				}
				ResponseChangeOBState(Client, Client->OBState); //134

				std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
				if (Room)
				{

					for (char i = 0; i < 6; i++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
						if (RoomClient && RoomClient != Client)
						{
							NotifyChangeOBState(RoomClient, Client->OBState, Client->Uin); //609
						}
					}
					return;
				}
				return;
			}

			// 切换房间地图
			if (Type == 1 && strstr(Content, "map"))
			{
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				//自定义切换地图
				char* mapid = Content;
				mapid += 4;
				Room->MapID = atoi(mapid);
				for (char i = 0; i < 6; i++)
				{
					std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
					if (RoomClient)
					{
						NotifyChangeMap(RoomClient, RoomClient->Uin, Room);
					}
				}
				return;
			}

			// 各种模式开关控制
			if (Type == 1 && strstr(Content, "关闭跳舞颁奖") && Client->skipFlag == 1)
			{
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				Room->isSkip = 1;
				for (char j = 0; j < 6; j++)
				{
					std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
					if (RoomClient)
					{
						ResponseStartGame(RoomClient, "房主已关闭跳舞颁奖流程!");
					}
				}
				return;

			}

			if (Type == 1 && strstr(Content, "开启比赛模式"))
			{
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->Israce = 1;
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->isRace = 1;
							ResponseStartGame(RoomClient, "房主已开启比赛模式!");
						}
					}
					return;
				}

			}

			if (Type == 1 && strstr(Content, "关闭比赛模式"))
			{
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->Israce = 0;
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->isRace = 0;
							ResponseStartGame(RoomClient, "房主已关闭比赛模式!");
						}
					}
					return;
				}

			}

			if (Type == 1 && strstr(Content, "开启匿名模式"))
			{
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->IsAnonymous = 1;
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsAnonymous = 1;
							ResponseStartGame(RoomClient, "房主已开启匿名模式!");
						}
					}
					return;
				}

			}

			if (Type == 1 && strstr(Content, "关闭匿名模式"))
			{
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->IsAnonymous = 0;
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsAnonymous = 0;
							ResponseStartGame(RoomClient, "房主已关闭匿名模式!");
						}
					}
					return;
				}

			}
			try
			{    // 检查GM权限和输入内容的合法性
				if (Type == 1 && (Client->gm > 0 || Client->admin == 1 || ClothesGMUins.count(Client->Uin) || CarGMUins.count(Client->Uin) || AllGMUins.count(Client->Uin))) {
					// 确保内容为数字（物品ID），并检查权限
					if (isNumeric(Content)) {
						if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
							return;
						}
						char* endPtr; // 用于存储转换结束位置的指针
						UINT itemid = std::strtoul(Content, &endPtr, 10); // 将内容转换为物品ID
						BOOLEAN isweek = false;
						// 非管理员用户的额外检查
						if (Client->admin != 1) {
							
							TREASUREBOX* TreasureBox = GetTreasureBox(itemid);
							if (TreasureBox)
							{
								return;
							}
							// 检查物品是否是宝箱，是否被锁住，以及GM时间限制
							if (itemid == 62787) {
								return;
							}
							if (isLock(itemid)) {
								ResponseStartGame(Client, "物品被锁住！");
								return;
							}
							// GM时间限制条件
							if (GMTime[0] == 1) {
								// 获取当前系统时间
								auto now = std::chrono::system_clock::now();
								std::time_t now_time = std::chrono::system_clock::to_time_t(now);
								struct tm tm_now;
								localtime_s(&tm_now, &now_time);

								// 获取小时
								int hour = tm_now.tm_hour;
								// 获取当前是星期几，tm_wday 的范围是 0（星期日）到 6（星期六）
								std::string day_of_week = std::to_string(tm_now.tm_wday);
								std::string Reason = "GM开启时间在";
								// 将逗号分隔的星期几字符串分割成单独的值
								std::istringstream ss(week);
								std::string token;
								std::vector<std::string> tokens;

								while (std::getline(ss, token, ',')) {
									tokens.push_back(token);
								}

								for (const auto& day : tokens) {
									Reason.append(getDayOfWeekString(std::stoi(day)));
									if (day == day_of_week) {
										isweek = true;
									}
								}

								if (!(hour >= GMTime[1] && hour < GMTime[2] && isweek == true)) {
									Reason.append("的");
									Reason.append(std::to_string(GMTime[1]));
									Reason.append("点到");
									Reason.append(std::to_string(GMTime[2]));
									Reason.append("点，你小子等会吧!");
									ResponseStartGame(Client, Reason.c_str());
									return;
								}
							}

							// GM使用期限检查
							if (GMTime[3] > 0 && Client->gmtips == 0 && Client->gm != 1) {
								Client->gmtips = 1;
								std::time_t initialTime = stringToTimeT(Client->gmtime);
								// 将天数转换为秒数
								const int secondsInDay = 86400; // 1 天 = 86400 秒
								std::time_t futureTime = initialTime + GMTime[3] * secondsInDay;
								// 获取当前时间
								std::time_t currentTime = std::time(nullptr);
								std::string resultMessage;
								if (futureTime > currentTime) {
									std::string timeDifference = getTimeDifferenceString(futureTime, currentTime);
									resultMessage = "您的Gm使用期限还有：/C01" + timeDifference + "/C00，请及时续费！";
									ResponseStartGame(Client, resultMessage.c_str());
									return;
								}
								else {
									UserDB_ban(Client->Uin);
									NotifyKickPlayer(Client, "Gm时间到期，你已经被封号！", 3);
									memcpy(Client->Key, "", 16);
									//GetClient(DestUin)->deleteflag = 1;
									Client->IsLogin = FALSE;
									Client->Server->Disconnect(Client->ConnID);
									Client->deleteflag = 1;
									return;
								}

							}
							// 检查物品种类是否符合用户权限
							if (Client->gm == 0 && ClothesGMUins.count(Client->Uin) && !contains(ClothesGMMess, GetItemMess(itemid))) {
								ResponseStartGame(Client, "您只能刷衣服类道具！");
								return;
							}
							else if (Client->gm == 0 && CarGMUins.count(Client->Uin) && !contains(CarGMMess, GetItemMess(itemid))) {
								ResponseStartGame(Client, "您只能刷赛车类道具！");
								return;
							}
							else if (Client->gm == 0 && AllGMUins.count(Client->Uin) && !contains(CarGMMess, GetItemMess(itemid))) {
								ResponseStartGame(Client, "您只能刷赛车类道具！");
								return;
							}

							if (GMMess.length() > 0 && !contains(GMMess, GetItemMess(itemid))) {
								std::string tips = "您只能获取以下类型道具：/C01" + GMMess;
								ResponseStartGame(Client, tips.c_str());
								return;
							}
						}
						// 创建物品信息并发送
						ItemInfo Items;

						if (GetItemType(itemid) == EAIT_PROP) {
							Items.ItemNum = 20000;
							Items.AvailPeriod = 0;
						}
						else {
							Items.ItemNum = 1;
							Items.AvailPeriod = -1;
						}
						Items.ItemID = itemid;
						Items.ItemType = 0;
						Items.ObtainTime = 0;
						PlayerDB_AddItem2(Client, Client->Uin, Items.ItemID, Items.ItemNum, Items.AvailPeriod, 0);
						virtualsend(Client, { Items }, 1, 23402);
						return;
						
					}
				}
			}
			catch (...)
			{
				printf("内置GM异常\n");
			}
			if (Type == 1 && strstr(Content, "下注") && Stake[0]) {
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				TalkStake(Client, Room, Content);
				return;
			}

			if (Type == 1 && strstr(Content, "开启尾数") != 0 && Mantissa[0]) {
				if (!AdminUins.count(Client->Uin)) {
					ResponseStartGame(Client, "只有管理员才能进行此操作！");
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->IsBottomGame = 9;
					std::string msg = GetItemName(Mantissa[3]);
					msg = "房主已开启尾数！消耗道具是/C01【" + GetItemName(Mantissa[3]) + "】/C00，最低下注/C01【" + std::to_string(Mantissa[1]) + "】/C00最高下注/C01【" + std::to_string(Mantissa[2]) + "】输入obs观战后，尾数有尾大，尾小，尾单 ，尾双，四种选择。例如输入：/C01尾数单100";
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsBottomGame = 1;
							ResponseStartGame(RoomClient, msg.c_str());
						}
					}
					return;
				}
			}
			if (Type == 1 && strstr(Content, "尾数") != 0 && Mantissa[0] && Room->IsBottomGame == 9) {
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				TalkMantissa(Client, Room, Content);
				return;
			}




			if (Type == 1 && strstr(Content, "开启垫底赛") != 0 && IsBottomGame[0]) {
	/*			if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}*/
				if (!AdminUins.count(Client->Uin)) {
					ResponseStartGame(Client, "只有管理员才能进行此操作！");
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->IsBottomGame = 1;
					std::string msg = GetItemName(IsBottomGame[1]);
					msg = "房主已开启垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00，扣除最后/C01【" + std::to_string(IsBottomGame[3]) + "】/C00名的道具，" + "扣除数量是：/C01【" + std::to_string(IsBottomGame[2]) + "】";
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsBottomGame = 1;
							ResponseStartGame(RoomClient, msg.c_str());
						}
					}
					return;
				}
			}

			if (Type == 1 && strstr(Content, "关闭垫底赛") && IsBottomGame[0]) {
				//if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
				//	return;
				//}
				if (!AdminUins.count(Client->Uin)) {
					ResponseStartGame(Client, "只有管理员才能进行此操作！");
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->IsBottomGame = 0;
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsBottomGame = 0;
							ResponseStartGame(RoomClient, "房主已关闭垫底赛模式！");
						}
					}
					return;
				}
			}

			if (Type == 1 && strstr(Content, "开启四人垫底") && IsBottomGame[0]) {
				//if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
				//	return;
				//}
				if (!AdminUins.count(Client->Uin)) {
					ResponseStartGame(Client, "只有管理员才能进行此操作！");
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->BottomGameNum = IsBottomGame4or6[0];
					Room->IsBottomGame = 4;
					int Frirst = Room->BottomGameNum / 10 * 4.5;
					int Second = Room->BottomGameNum / 10 * 3.5;
					int Third = Room->BottomGameNum / 10 * 2;
					std::string msg = "房主已开启垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00\n第一名/C01赢【" + std::to_string(Frirst) + "】/C00，第二名/C01赢【"
						+ std::to_string(Second) + "】/C00，第三名/C01赢【" + std::to_string(Third) + "】/C00，第四名/C01输【" + std::to_string(Room->BottomGameNum) + "】/C00";
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsBottomGame = 1;
							ResponseStartGame(RoomClient, msg.c_str());
						}
					}
					return;
				}
			}

			if (Type == 1 && strstr(Content, "开启六人垫底") && IsBottomGame[0]) {
				//if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
				//	return;
				//}
				if (!AdminUins.count(Client->Uin)) {
					ResponseStartGame(Client, "只有管理员才能进行此操作！");
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {

					Room->BottomGameNum = IsBottomGame4or6[1];
					Room->IsBottomGame = 6;
					int  Num = Room->BottomGameNum;
					int Frirst = Num / 10 * 4;
					int Second = Num / 10 * 3;
					int Third = Num / 10 * 2;
					int Four = Num / 10 * 1;
					int five = Num / 10 * 4;
					int six = Num / 10 * 6;
					std::string msg = "房主已开启垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00\n第一名/C01赢【" + std::to_string(Frirst) + "】/C00，第二名/C01赢【"
						+ std::to_string(Second) + "】/C00，第三名/C01赢【" + std::to_string(Third) + "】/C00\n第四名/C01赢【" + std::to_string(Four) + "】/C00"
						+ "第五名/C01输【" + std::to_string(five) + "】/C00第六名/C01输【" + std::to_string(six) + "】/C00";
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsBottomGame = 1;
							ResponseStartGame(RoomClient, msg.c_str());
						}
					}
					return;
				}
			}

			if (Type == 1 && strstr(Content, "开启竞技模式") != 0 && IsBottomGame[0]) {
				//if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
				//	return;
				//}
				if (!AdminUins.count(Client->Uin)) {
					ResponseStartGame(Client, "只有管理员才能进行此操作！");
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {
					Room->IsBottomGame = 7;
					int Frirst = 5;
					int Second = 4;
					int Third = 3;
					int Four = 2;
					int Five = 1;
					std::string msg = "房主已开启竞技模式！你会获得道具/C01【" + GetItemName(106614) + "】/C00\n第一名/C01赢【" + std::to_string(Frirst) + "】/C00，第二名/C01赢【"
						+ std::to_string(Second) + "】/C00，第三名/C01赢【" + std::to_string(Third) + "】/C00第四名/C01赢【" + std::to_string(Four) + "】/C00第五名/C01赢【" + std::to_string(Five) + "】/C00";
					for (char j = 0; j < 6; j++)
					{
						std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
						if (RoomClient)
						{
							RoomClient->IsBottomGame = 1;
							ResponseStartGame(RoomClient, msg.c_str());
						}
					}
					return;
				}
			}

			/*if (Type == 1 && strstr(Content, "垫底") && IsBottomGame[0]) {

				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				if (Client->ConnID == Room->RoomOwnerID) {

					char* Num = Content;
					Num += 4;
					if (isInteger(Num)) {
						Room->BottomGameNum = atoi(Num);
						Room->IsBottomGame = 2;
						std::string msg = GetItemName(IsBottomGame[1]);
						msg = "房主已开启垫底赛模式！消耗道具是/C01【" + GetItemName(IsBottomGame[1]) + "】/C00，扣除最后/C01【" + std::to_string(IsBottomGame[3]) + "】/C00名的道具，" + "扣除数量是：/C01【" + std::to_string(Room->BottomGameNum) + "】";
						for (char j = 0; j < 6; j++)
						{
							std::shared_ptr<ClientNode> RoomClient = Room->Player[j];
							if (RoomClient)
							{
								RoomClient->IsBottomGame = 1;
								ResponseStartGame(RoomClient, msg.c_str());
							}
						}
						return;
					}
					else {
						ResponseStartGame(Client, "垫底数量格式异常，只允许输入数字！");
						return;
					}
				}
			}*/



			if (Type == 1 && strstr(Content, "fps")) {
				//if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
				//	return;
				//}
				//if (!AdminUins.count(Client->Uin)) {
				//	ResponseStartGame(Client, "只有管理员才能进行此操作！");
				//	return;
				//}
				const char* message = (PlayerDB_UpdateBaseInfoFps(Client) > 0) ?
					"游戏内60帧数已开启，刷新频道生效！" :
					"游戏内60帧数已关闭，刷新频道生效!";
				ResponseStartGame(Client, message);
				return;
			}

			if (Type == 1 && strstr(Content, "绑定qq")) {
				if (LimitTalk && !(Client->gm > 0 || Client->admin > 0)) {
					return;
				}
				std::string qqNumber = extractQQ(Content);

				PlayerDB_UpdateBaseInfo(Client->Uin, qqNumber);
				qqNumber = "您绑定的QQ号码是/C01 " + qqNumber + " /C00刷新频道即可看到自己头像！";
				ResponseStartGame(Client, qqNumber.c_str());
				return;
			}


			if (Type == 1 && strcmp(Content, "我要玩龙晶") == 0)
			{
				std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
				if (Room && Room->BaseGameMode == 0x2e)
				{
					Client->MapID = Room->MapID = 431;
					ResponseStartGame(Client, "切换模式为龙晶大闯关");
					return;
				}
			}
			if (Type == 1 && strcmp(Content, "我要玩幻域") == 0)
			{
				std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
				if (Room && Room->BaseGameMode == 0x2e)
				{
					Client->MapID = Room->MapID = 432;
					ResponseStartGame(Client, "切换模式为幻域大闯关");
					return;
				}
			}
			if (Type == 1 && strcmp(Content, "我要玩冲刺") == 0)
			{
				std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
				if (Room && Room->BaseGameMode == 0x2e)
				{
					Client->MapID = Room->MapID = 452;
					ResponseStartGame(Client, "切换模式为冲刺大闯关");
					return;
				}
			}
		}


		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
#ifdef ZingSpeed
				if (RoomClient == Client)
				{
					continue;
				}
#endif
				NotifyTalk(RoomClient, Client, Type, ClientUseFlag, DestPlayerUin, ContentLength, Content);
				/*BYTE* Body;
				Test(Client, Body, 1);*/
			}
		}
	}
}
void ResponseTalk(std::shared_ptr<ClientNode> Client, BYTE Type, BYTE ClientUseFlag, UINT DestPlayerUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write8(p, Type);
	Write8(p, ClientUseFlag);
	Write32(p, DestPlayerUin);

	Write8(p, 0); //ReasonLen
	Write8(p, 0); //BugletType
	Write8(p, 0); //ReserveFlag
	Write8(p, 0); //ReserveData
#ifndef ZingSpeed
	Write8(p, 0); //TalkSpecialType
	Write32(p, 0); //Seq
#endif

	len = p - buf.data();
	SendToClient(Client, 111, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}


void NotifyTalk(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> RoomClient, BYTE Type, BYTE ClientUseFlag, UINT DestPlayerUin, short ContentLength, const char* Content)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, RoomClient->Uin);

	memset(p, 0, 17);

	memcpy(p, RoomClient->NickName, 17);

	p += 17;

	Write8(p, 0); //Gender

	Write8(p, Type);
	Write8(p, ClientUseFlag);
	Write32(p, DestPlayerUin);

	Write16(p, ContentLength);
	memcpy(p, Content, ContentLength);
	p += ContentLength;


	Write8(p, 0); //BugletType
	Write32(p, 0); //Identify
	Write16(p, 0x0107); //VipFlag
	Write8(p, 0); //CanReport
	{ //TalkerInfo
		BYTE* pTalkerInfo = p;
		Write16(pTalkerInfo, 0); //len

		Write32(pTalkerInfo, 0); //Exp

		{ //NobleInfo
			BYTE* pNobleInfo = pTalkerInfo;
			Write16(pNobleInfo, 0); //len

			Write32(pNobleInfo, RoomClient->NobleID); //NobleID
			Write8(pNobleInfo, RoomClient->NobleLevel); //NobleLevel
			Write32(pNobleInfo, RoomClient->NoblePoint); //NoblePoint
			Write32(pNobleInfo, -1); //NobleLeftDays

			len = pNobleInfo - pTalkerInfo;
			Set16(pTalkerInfo, (WORD)len);
			pTalkerInfo += len;
		}
#ifndef ZingSpeed
		{ //GuildVipBaseInfo
			BYTE* pGuildVipBaseInfo = pTalkerInfo;
			Write16(pGuildVipBaseInfo, 0); //len

			Write8(pGuildVipBaseInfo, RoomClient->GuildVipLevel); //GuildVipLevel
			Write32(pGuildVipBaseInfo, RoomClient->GuildVipPoint); //GuildVipPoint

			len = pGuildVipBaseInfo - pTalkerInfo;
			Set16(pTalkerInfo, (WORD)len);
			pTalkerInfo += len;
		}
		Write32(pTalkerInfo, 0); //GuildId

		Write8(pTalkerInfo, 1); //HasLoverVip

		for (size_t i = 0; i < 1; i++)
		{ //LoverVipInfo
			BYTE* pLoverVipInfo = pTalkerInfo;
			Write16(pLoverVipInfo, 0); //len

			Write8(pLoverVipInfo, RoomClient->LoverVipLevel); //LoverVipLevel
			Write32(pLoverVipInfo, RoomClient->LoverVipPoint); //LoverVipPoint
			Write8(pLoverVipInfo, 10); //GrowRate

			len = pLoverVipInfo - pTalkerInfo;
			Set16(pTalkerInfo, (WORD)len);
			pTalkerInfo += len;
		}

		Write8(pTalkerInfo, RoomClient->GardenLevel); //GardenLevel
		Write8(pTalkerInfo, 9); //ConsumeVipLevel
		{ //EmperorInfo
			BYTE* pEmperorInfo = pTalkerInfo;
			Write16(pEmperorInfo, 0); //len

			Write8(pEmperorInfo, RoomClient->EmperorLevel); //EmperorLevel
			Write32(pEmperorInfo, RoomClient->EmperorPoint); //EmperorPoint
			Write32(pEmperorInfo, -1); //EmperorLeftDays
			Write8(pEmperorInfo, 10); //EmperorGrowRate

			len = pEmperorInfo - pTalkerInfo;
			Set16(pTalkerInfo, (WORD)len);
			pTalkerInfo += len;
		}
		Write32(pTalkerInfo, 0); //HelperLev
#endif

		len = pTalkerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write8(p, 0); //ReserveFlag
	Write32(p, 0); //ReserveData
#ifndef ZingSpeed
	memset(p, 0, 18); //Contentid[]
	p += 18;
#endif
	Write8(p, 0); //transBufType
	Write16(p, 0); //transBufLen
#ifndef ZingSpeed
	Write8(p, 0); //MsgSource
	Write16(p, 0); //DstNickNameLength
	Write32(p, 0); //Seq
#endif

	len = p - buf.data();
	SendToClient(Client, 505, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Notify);
}


void NotifyChangeRoomOwner(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, Room->ID); //RoomID
	Write16(p, Room->RoomOwnerID); //NewRoomOwnerID
	Write8(p, 0); //HasFBInfo
	for (size_t i = 0; i < 0; i++)
	{ //FBInfo
		BYTE* pFBInfo = p;
		Write16(pFBInfo, 0); //len

		Write8(pFBInfo, 0); //AvailableLev
		Write8(pFBInfo, 0); //Lev
		Write8(pFBInfo, 0); //SubLev
		Write32(pFBInfo, 0); //BossID
		Write8(pFBInfo, 0); //FBSeason

		len = pFBInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //OriRoomOwnerID
	Write8(p, 0); //ChangeType
	Write16(p, 0); //RoomType

	len = p - buf.data();
	SendToClient(Client, 524, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void ResponseChangeRoomOwner(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, Room->RoomOwnerID); //NewOwner
	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 713, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestChangeRoomOwner(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT NewOwner = Read32(p);
	UINT NewOwnerID = 0;
	// 获取房间信息
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room) // 如果房间不存在，直接返回
	{
		return;
	}
	// 遍历房间内的玩家
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> NewRoomOwner = Room->Player[i];
		if (NewRoomOwner)
		{// 如果找到匹配的新房主
			if (NewRoomOwner->Uin == NewOwner)
			{
				NewOwnerID = NewRoomOwner->ConnID;// 设置新房主连接ID
				NewRoomOwner->ReadyState = 0;// 重置新房主的准备状态
				break;
			}
		}
	}
	Room->RoomOwnerID = NewOwnerID; // 更新房主ID
	Room->IsBottomGame = 0;    // 关闭垫底赛模式
	ResponseChangeRoomOwner(Client, Room);   // 响应房主变更
	// 通知房间内的每个玩家房主已变更
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyChangeRoomOwner(RoomClient, Room);// 通知每个玩家
		}
	}

}

void NotifyLmtEnterRoom(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	{ //PlayerInfo
		BYTE* pPlayerInfo = p;
		Write16(pPlayerInfo, 0); //len

		Write32(pPlayerInfo, Client->Uin); //Uin
		WriteString(pPlayerInfo, Client->NickName); //NickName[]
		Write32(pPlayerInfo, 0); //Identify
		Write8(pPlayerInfo, Client->Gender); //Gender
		Write32(pPlayerInfo, 0); //EquipedIconID
		WriteUn(pPlayerInfo, 0); //Experience
		{ //PlayerRecord
			BYTE* pPlayerRecord = pPlayerInfo;
			Write16(pPlayerRecord, 0); //len

			Write32(pPlayerRecord, 0); //MatchNum
			Write8(pPlayerRecord, 0); //BestRank
			Write32(pPlayerRecord, 0); //ChamNum
			Write32(pPlayerRecord, 0); //WinNum

			len = pPlayerRecord - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		WriteUn(pPlayerInfo, 0); //VipFlag
		{ //NobleInfo
			BYTE* pNobleInfo = pPlayerInfo;
			Write16(pNobleInfo, 0); //len

			Write32(pNobleInfo, Client->NobleID); //NobleID
			Write8(pNobleInfo, Client->NobleLevel); //NobleLevel
			Write32(pNobleInfo, Client->NoblePoint); //NoblePoint
			Write32(pNobleInfo, -1); //NobleLeftDays

			len = pNobleInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //GuildVipBaseInfo
			BYTE* pGuildVipBaseInfo = pPlayerInfo;
			Write16(pGuildVipBaseInfo, 0); //len

			Write8(pGuildVipBaseInfo, Client->GuildVipLevel); //GuildVipLevel
			Write32(pGuildVipBaseInfo, Client->GuildVipPoint); //GuildVipPoint

			len = pGuildVipBaseInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //HasLoverVip
		for (size_t i = 0; i < 0; i++)
		{ //LoverVipInfo
			BYTE* pLoverVipInfo = pPlayerInfo;
			Write16(pLoverVipInfo, 0); //len

			Write8(pLoverVipInfo, 0); //LoverVipLevel
			Write32(pLoverVipInfo, 0); //LoverVipPoint
			Write8(pLoverVipInfo, 0); //GrowRate

			len = pLoverVipInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //GardenLevel
		{ //EmperorInfo
			BYTE* pEmperorInfo = pPlayerInfo;
			Write16(pEmperorInfo, 0); //len

			Write8(pEmperorInfo, 0); //EmperorLevel
			Write32(pEmperorInfo, 0); //EmperorPoint
			Write32(pEmperorInfo, 0); //EmperorLeftDays
			Write8(pEmperorInfo, 0); //EmperorGrowRate

			len = pEmperorInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //ConsumeVipInfo
			BYTE* pConsumeVipInfo = pPlayerInfo;
			Write16(pConsumeVipInfo, 0); //len

			WriteUn(pConsumeVipInfo, 0); //VipLevel
			WriteUn(pConsumeVipInfo, 0); //CharmValueOfMonth
			WriteUn(pConsumeVipInfo, 0); //SearchTreasureNums
			WriteUn(pConsumeVipInfo, 0); //GetTreasureNums

			len = pConsumeVipInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write32(pPlayerInfo, 0); //BeLikedNums
		Write8(pPlayerInfo, 0); //AreaID

		len = pPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 801, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestLmtEnterChatRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT RoomID = Read32(p);
	ResponseLmtEnterChatRoom(Client);
}

void ResponseLmtEnterChatRoom(std::shared_ptr<ClientNode> Client)
{
	//测试
	//NotifyLuckyMatchReady(Client);
	{
		std::string Byte = "000000000000000000000000000000";
		const unsigned char* Msg403 = EHexToByte(Byte.data());
		SendToClient(Client, 403, (const BYTE*)Msg403, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		delete[] Msg403;
	}

	{
		std::string Byte = "0001000A0005000000000000";
		const unsigned char* Msg413 = EHexToByte(Byte.data());
		SendToClient(Client, 413, (const BYTE*)Msg413, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		delete[] Msg413;
	}

	{
		std::string Byte = "000000000400080005000000000008000666FA6BF90008000766FA6BF90008000866FA6BF9";
		const unsigned char* Msg439 = EHexToByte(Byte.data());
		SendToClient(Client, 439, (const BYTE*)Msg439, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		delete[] Msg439;
	}

	{
		std::string Byte = "0028000003E800000001020007030000000000070300000000000F00000001000000000000000000";
		const unsigned char* Msg810 = EHexToByte(Byte.data());
		SendToClient(Client, 810, (const BYTE*)Msg810, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		delete[] Msg810;
	}
	NotifyLmtGetScoreInfo(Client);

	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write16(p, 1); //PlayerNum
	for (size_t i = 0; i < 1; i++)
	{ //PlayerInfo
		BYTE* pPlayerInfo = p;
		Write16(pPlayerInfo, 0); //len

		Write32(pPlayerInfo, 0); //Uin
		WriteString(pPlayerInfo, "测试"); //NickName[]
		Write32(pPlayerInfo, 0); //Identify
		Write8(pPlayerInfo, 0); //Gender
		Write32(pPlayerInfo, 0); //EquipedIconID
		WriteUn(pPlayerInfo, 0); //Experience
		{ //PlayerRecord
			BYTE* pPlayerRecord = pPlayerInfo;
			Write16(pPlayerRecord, 0); //len

			Write32(pPlayerRecord, 0); //MatchNum
			Write8(pPlayerRecord, 0); //BestRank
			Write32(pPlayerRecord, 0); //ChamNum
			Write32(pPlayerRecord, 0); //WinNum

			len = pPlayerRecord - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		WriteUn(pPlayerInfo, 0); //VipFlag
		{ //NobleInfo
			BYTE* pNobleInfo = pPlayerInfo;
			Write16(pNobleInfo, 0); //len

			Write32(pNobleInfo, 0); //NobleID
			Write8(pNobleInfo, 0); //NobleLevel
			Write32(pNobleInfo, 0); //NoblePoint
			Write32(pNobleInfo, 0); //NobleLeftDays

			len = pNobleInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //GuildVipBaseInfo
			BYTE* pGuildVipBaseInfo = pPlayerInfo;
			Write16(pGuildVipBaseInfo, 0); //len

			Write8(pGuildVipBaseInfo, 0); //GuildVipLevel
			Write32(pGuildVipBaseInfo, 0); //GuildVipPoint

			len = pGuildVipBaseInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //HasLoverVip
		for (size_t i = 0; i < 0; i++)
		{ //LoverVipInfo
			BYTE* pLoverVipInfo = pPlayerInfo;
			Write16(pLoverVipInfo, 0); //len

			Write8(pLoverVipInfo, 0); //LoverVipLevel
			Write32(pLoverVipInfo, 0); //LoverVipPoint
			Write8(pLoverVipInfo, 0); //GrowRate

			len = pLoverVipInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //GardenLevel
		{ //EmperorInfo
			BYTE* pEmperorInfo = pPlayerInfo;
			Write16(pEmperorInfo, 0); //len

			Write8(pEmperorInfo, 0); //EmperorLevel
			Write32(pEmperorInfo, 0); //EmperorPoint
			Write32(pEmperorInfo, 0); //EmperorLeftDays
			Write8(pEmperorInfo, 0); //EmperorGrowRate

			len = pEmperorInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //ConsumeVipInfo
			BYTE* pConsumeVipInfo = pPlayerInfo;
			Write16(pConsumeVipInfo, 0); //len

			WriteUn(pConsumeVipInfo, 0); //VipLevel
			WriteUn(pConsumeVipInfo, 0); //CharmValueOfMonth
			WriteUn(pConsumeVipInfo, 0); //SearchTreasureNums
			WriteUn(pConsumeVipInfo, 0); //GetTreasureNums

			len = pConsumeVipInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write32(pPlayerInfo, 0); //BeLikedNums
		Write8(pPlayerInfo, 0); //AreaID

		len = pPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 381, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	NotifyGangsterCfg(Client);

	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		WriteHex(p, "1B3E889D00C80077000000010000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F00000000000000010000C3500000000000001F40000A00000000000000000000000100000002000000010000000001000E0000000000004B64000001F40073000000020000000BFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100370000000100000001000000030000000000002710000A0000000000000000000000000000000001000E000036B0000088B8000000C8007B000000030000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000201800804000000000001003F0000000000000001000138800000000000001B58000A00000000000000000000000100000002000000080000000001000E0000000000004330000003E80073000000040000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A000000000000088B8000A00000000000000000000000100000002000000010000000001000E0000000000004B64000005DC0073000000050000000BFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000010000000000000100370000000000000001000138800000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000001900073000000060000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000006000000000001003F0000000000000001000138800000000000001B58000A00000000000000000000000100000002000000560000000001000E0000000000004330000003840065000000070000000B000000010000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100290000000000000001000186A00000000100000000000A00000000000000000000000800000000000073000000080000000BFFFFFFFF000000000000000000000000002E000000000000000100000002001200000002000000020180080400000000000E000000010000000100007530000100290000000000000001000186A00000000100000000000A00000000000000000000000600000000000065000000090000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000090000000100000BB80001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100001F400000650000000A0000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000500000001000000000001002D0000000100000001000000020000000000000FA0000A0000000000000000000000030000000100001D4C00007B0000000B0000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000088B80000000000000BB8000A00000000000000000000000100000002000000560000000001000E0000000000004330000003E800650000000C0000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000075300001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100001F400000A40000000D0000000BFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000010000000000000200290000000100000001000000020000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000560000000001000E00004B64000088B80000025800650000000E0000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000100310000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C0000007800007B0000000F0000000BFFFFFFFF00000000000000000000000000200000000100000001000000010012000000020000000200000000000000100001003F00000000000000010000EA600000000000001B58000A00000000000000000000000100000002000000560000000001000E0000000000004330000003E800AA000000100000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200290000000000000001000186A00000000100000000000A0000000000000000000000080000000000004D0000000000000001000186A00000000000007530000A00000000000000000000000100000002000000010000000002000E0000000000004B6400000384000E00004B64000088B80000019000AE000000110000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000090000000100001770000200370000000000000001000138800000000000001770000A0000000000000000000000000000000001000E000036B0000088B8000001F4003F0000000200000001000000010000000000001770000A00000000000000000000000100000002000000010000000001000E0000000000004B64000001F4006F000000120000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000050000000100000000000100370000000000000001000186A00000000000001B58000A0000000000000000000000000000000001000E000036B0000088B8000002580065000000130000000BFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100290000000100000001000000050000000100000000000A00000000000000000000000800000000000061000000140000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001002D0000000000000001000186A000000000FFFFFFFF000A0000000000000000000000030000000100001194000061000000150000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000003E8000100290000000000000001000186A00000000100000000000A00000000000000000000000600000000000073000000160000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C000003E801000E00004394000088B8000002BC0069000000170000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001002D0000000000000001000088B80000000100000000000A00000000000000000000000700000001000027100000B2000000180000000BFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000004D0000000200000001000000010000000000001388000A00000000000000000000000100000002000000020000000002000E0000000000004330000004B0000E00004394000088B8000002BC0065000000190000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000100000001000075300001002D0000000000000001000186A00000000000007530000A0000000000000000000000030000000100000FA00000610000001A0000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000061A8000100290000000000000001000186A00000000100000000000A000000000000000000000008000000000000690000001B0000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001002D00000000000000010000D6D80000000100000000000A00000000000000000000000700000001000059D80000770000001C0000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000090000000100001F400001003F0000000000000001000111700000000000001F40000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001F400730000001D0000000BFFFFFFFF0000"
			"000000000000000000000018000000000000000100000001000A00000005000000000001003F0000000100000001000000020000000000000FA0000A000000000000000000000001000000020000000C0000000001000E00004394000088B8000003E800690000001E0000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000201800804000000000001002D0000000100000001000000030000000000001388000A00000000000000000000000300000001000017700000920000001F0000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000002002D0000000000000001000186A00000000100000000000A000000000000000000000007000000010000C35000003100000002000000010000000100000000FFFFFFFF000A00000000000000000000000400000002000186A0000186A00000AA000000200000000BFFFFFFFF0000000000000000000000000026000000000000000000000002000E0000000B000000010000AFC8000A0000000C00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F00000002000000010000000100000000000007D0000A00000000000000000000000100000002000000560000000001000E0000000000004330000003E800AE000000210000000BFFFFFFFF0000000000000000000000000026000000000000000000000002000E0000000B000000010000AFC8000A0000000C000000000002002D0000000000000001000186A00000000100000000000A000000000000000000000007000000010000EA6000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000010000000001000E0000000000004B64000003200065000000220000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D0000000000000001000186A00000000000001388000A0000000000000000000000030000000100004E2000006D000000230000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC8000100350000000000000001000186A00000000000000BB8000A0000000000000000000000000000000300000000000037DC000003E800005D000000240000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000100290000000000000001000186A000000000FFFFFFFF000A0000000000000000000000090000000000007B000000250000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001003F0000000000000001000088B80000000000001F40000A00000000000000000000000100000002000000010000000001000E00004B64000088B800000190006F000000260000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000400000001FFFFFFFF000100370000000000000001000111700000000000000BB8000A0000000000000000000000010000000001000E000036B0000088B8000002580077000000270000000BFFFFFFFF000000000000000000000000002E000000000000000100000002000E0000000100000001000075300012000000020000000200000700000000200001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100002710000065000000280000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100001B5800007B000000290000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000088B80000000000001770000A00000000000000000000000100000002000000560000000001000E00004B64000088B80000025800770000002A0000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F0000000000000001000080E80000000000001F40000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001F400690000002B0000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100000BB80000AA0000002C0000000BFFFFFFFF0000000000000000000000000026000000000000000000000002000E0000000B000000010000AFC8000A0000000C000000000002003F0000000000000001000186A00000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001F400290000000200000001000000010000000000001770000A0000000000000000000000000000000000006B0000002D0000000BFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000500000000000100370000000100000001000000020000000000000FA0000A0000000000000000000000000000000001000E000036B0000088B8000000C800690000002E0000000BFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001002D0000000000000001000186A00000000100000000000A00000000000000000000000700000001000032C800006F0000002F0000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000400000001FFFFFFFF000100370000000100000001000000030000000000001B58000A0000000000000000000000010000000001000E000036B0000088B8000002580073000000300000000CFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100370000000100000001000000030000000000001388000A0000000000000000000000000000000001000E000036B0000088B8000002580073000000310000000CFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100370000000100000001000000020000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000003840077000000320000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F00000000000000010000EA600000000000001F40000A00000000000000000000000100000002000000010000000001000E0000000000004B64000002580077000000330000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F0000000000000001000186A00000000000001F40000A00000000000000000000000100000002000000010000000001000E0000000000004B6400000384007B000000340000000CFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000186A00000000000000BB8000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000000C8007B000000350000000CFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000186A00000000000000BB8000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001900077000000360000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000300000001FFFFFFFF0001003F0000000100000001000000030000000000001388000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000004B00077000000370000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000300000001FFFFFFFF0001003F0000000100000001000000020000000000001F40000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000007080077000000380000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F0000000000000001000138800000000000001388000A00000000000000000000000100000002000000010000000001000E0000000000004B64000003840077000000390000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F0000000000000001000186A00000000000001388000A00000000000000000000000100000002000000010000000001000E0000000000004B64000004E200770000003A0000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000017700001003F000000000000000100011170000000000000170C000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000019000770000003B0000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000017700001003F000000000000000100017318000000000000170C000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000002EE007B0000003C0000000CFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001003F0000000100000001000000050000000000001388000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000004B0007B0000003D0000000CFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001003F0000000100000001000000030000000000001388000A00000000000000000000000100000002000000560000000001000E00004B64000088B800000898007B0000003E0000000CFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F00000000000000010000EA600000000000001388000A00000000000000000000000100000002000000560000000001000E000000000000433000000708007B0000003F0000000CFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F000000000000000100015F900000000000001388000A00000000000000000000000100000002000000560000000001000E0000000000004330000009C400A4000000400000000C00000004000000000000000000000000001C000000000000000100000001000E0000001000000001000000040002003F0000000000000001000186A00000000000001770000A00000000000000000000000100000002000000560000000001000E0000000000004330000007D0002D0000000200000001000000010000000100000000000A00000000000000000000000700000001000075300000A4000000410000000C00000006000000000000000000000000001C000000000000000100000001000E0000001000000001000000040002003F0000000000000001000186A00000000000001F40000A00000000000000000000000100000002000000560000000001000E000000000000433000000BB8002D0000000200000001000000010000000100000000000A000000000000000000000007000000010000C3500000AA000000420000000CFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000D00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000001388000A00000000000000000000000100000002000000560000000001000E00000000000043300000064000AA000000430000000CFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000D00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000001F40000A00000000000000000000000100000002000000560000000001000E000000000000433000000C8000DC000000440000000CFFFFFFFF000000000000000000000000002C000000000000000100000001001E0000000700000005000007FC00000001000000010000000100000000000200410000000000000001000186A00000000200000001001E000000070000000500000002000000020000000100000001000000000000000300000001000017700000530000000200000001000000010000000200000001001E000000070000000500000002000000020000000100000001000000000000000100000002000000010000000001000E0000000000004B640000025800DC000000450000000CFFFFFFFF000000000000000000000000002C000000000000000100000001001E0000000700000005000007FC00000001000000010000000100000000000200410000000000000001000186A00000000200000001001E000000070000000500000002000000020000000100000001000000000000000300000001000027100000530000000200000001000000010000000200000001001E000000070000000500000002000000020000000100000001000000000000000100000002000000010000000001000E0000000000004B6400000384006B000000460000000CFFFFFFFF000007D000000000000000000018000000010000000100000001000A0000000E00000000000100370000000000000001000111700000000000000FA0000A0000000000000000000000000000000001000E000036B0000088B800000258006B000000470000000CFFFFFFFF000007D000000000000000000018000000010000000100000001000A0000000E00000000000100370000000000000001000186A00000000000001388000A0000000000000000000000000000000001000E000036B0000088B800000384006F000000480000000CFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000500000001000000000001003700000000000000010000EA600000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000001F4006F000000490000000CFFFFFFFF000000000000000000000000001C000000010000000100000001000E00000005000000010000000000010037000000000000000100015F900000000000001F40000A0000000000000000000000000000000001000E000036B0000088B80000032000A40000004A0000000CFFFFFFFF0000000000000000000000000020000000010000000100000001001200000002000000020000010000000000000200290000000100000001000000020000000100000000000A0000000000000000000000060000000000003F0000000100000001000000020000000000000BB8000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000004B000A40000004B0000000CFFFFFFFF0000000000000000000000000020000000010000000100000001001200000002000000020000010000000000000200290000000100000001000000020000000100000000000A0000000000000000000000060000000000003F0000000100000001000000020000000000001388000A00000000000000000000000100000002000000560000000001000E00004B64000088B80000096000A40000004C0000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200310000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C0000012C00003F00000002000000010000000100000000FFFFFFFF000A000000000000000000000001000000020000000C0000000001000E00004B64000088B8000004B000A40000004D0000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200310000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C000003E800003F00000002000000010000000100000000FFFFFFFF000A000000000000000000000001000000020000000C0000000001000E00004B64000088B80000096000730000004E0000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A00000000000007530000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000004B000730000004F0000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A0000000000000C350000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000007080065000000500000000C000000010000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100290000000000000001000186A00000000100000000000A00000000000000000000000800000000000065000000510000000C000000030000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100290000000000000001000186A00000000100000000000A00000000000000000000000800000000000098000000520000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200370000000000000001000186A00000000000007530000A0000000000000000000000000000000001000E000036B0000088B800000190002D0000000200000001000000010000000000007530000A0000000000000000000000030000000100001388000098000000530000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200370000000000000001000186A00000000000009C40000A0000000000000000000000000000000001000E000036B0000088B800000258002D0000000200000001000000010000000000009C40000A0000000000000000000000030000000100001B5800009C000000540000000C00000004000000000000000000000000001C000000000000000100000001000E000000100000000100000003000200370000000000000001000186A00000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000002EE002D0000000200000001000000010000000000001F40000A0000000000000000000000030000000100001F4000009C000000550000000C0000000600000000000000000000000"
			"0001C000000000000000100000001000E000000100000000100000003000200370000000000000001000186A00000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000003E8002D0000000200000001000000010000000000001F40000A0000000000000000000000030000000100002EE0000065000000560000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100001F40000065000000570000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001002D0000000000000001000186A00000000100000000000A000000000000000000000007000000010000465000005D000000580000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000100290000000000000001000186A000000000FFFFFFFF000A00000000000000000000000A0000000000005D000000590000000CFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000100290000000000000001000186A000000000FFFFFFFF000A0000000000000000000000090000000000009C0000005A0000000CFFFFFFFF000007D00000000000000000001C000000010000000100000001000E0000000B000000010000AFC80002002D0000000000000001000186A00000000100000000000A000000000000000000000007000000010000C3500000370000000000000001000186A00000000000002710000A0000000000000000000000000000000001000E000036B0000088B800000258009C0000005B0000000CFFFFFFFF000007D00000000000000000001C000000010000000100000001000E0000000B000000010000AFC80002002D0000000000000001000186A00000000100000000000A00000000000000000000000700000001000138800000370000000000000001000186A00000000000002710000A0000000000000000000000000000000001000E000036B0000088B8000003E800AA0000005C0000000CFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000C00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000FA0000A00000000000000000000000100000002000000560000000001000E0000000000004330000003E800AA0000005D0000000CFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000C00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000001388000A00000000000000000000000100000002000000560000000001000E0000000000004330000005DC00650000005E0000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D00000000000000010000EA600000000100000000000A000000000000000000000007000000010000EA600000650000005F0000000CFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D0000000000000001000173180000000100000000000A0000000000000000000000070000000100017318000077000000600000000BFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000400000001FFFFFFFF0001003F00000000000000010000EA600000000000001F40000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001900077000000610000000BFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000500000001000000000001003F0000000000000001000111700000000000000BB8000A00000000000000000000000100000002000000560000000001000E00004B64000088B80000044C0073000000620000000BFFFFFFFF0000000000000000000000000018000000010000000100000001000A00000006000000000001003F0000000000000001000186A00000000000000FA0000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001F40065000000630000000BFFFFFFFF0000000000000000000000000020000000010000000100000001001200000002000000020000030000000000000100290000000100000001000000020000000100000000000A00000000000000000000000600000000000061000000640000000BFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000090000000100001F40000100290000000000000001000186A00000000000000000000A00000000000000000000000600000000000065000000650000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000001100000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000002000003E8000065000000660000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000001100000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000002000000C8000069000000670000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000120000000100000028000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000001000003E8000069000000680000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000001200000001000000280001003100000000000000010000C3500000000100000000000A00000000000000000000000B0000000200000001000001F400006D000000690000000DFFFFFFFF0000000000000000000000000020000000000000000100000001001200000013000000020000020000000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000002000003E800006D0000006A0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000130000000200000200000000000001003100000000000000010000C3500000000100000000000A00000000000000000000000B0000000200000002000001F40000650000006B0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000001400000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000001000003E80000650000006C0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000001003100000000000000010000C3500000000100000000000A00000000000000000000000B0000000200000001000001F400006D0000006D0000000DFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020180080400000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000002000003E800006D0000006E0000000DFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020180080400000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000002000000C80000730000006F0000000DFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100370000000100000001000000030000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000002580073000000700000000DFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100370000000100000001000000030000000000001770000A0000000000000000000000000000000001000E000036B0000088B8000001900073000000710000000DFFFFFFFF0000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100370000000100000001000000030000000000001388000A0000000000000000000000000000000001000E000036B0000088B80000012C0077000000720000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F00000000000000010000EA600000000000001F40000A00000000000000000000000100000002000000010000000001000E0000000000004B64000002EE0077000000730000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F00000000000000010000C3500000000000001B58000A00000000000000000000000100000002000000010000000001000E0000000000004B64000001F40077000000740000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F000000000000000100009C400000000000001388000A00000000000000000000000100000002000000010000000001000E0000000000004B64000001C2007B000000750000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000186A00000000000000BB8000A00000000000000000000000100000002000000010000000001000E00004B64000088B800000190007B000000760000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000186A00000000000000BB8000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000000C80077000000770000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000017700001003F0000000000000001000111700000000000001770000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001C20077000000780000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000017700001003F00000000000000010000C3500000000000001770000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0077000000790000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000900000001000017700001003F000000000000000100009C400000000000001388000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000000C8007B0000007A0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001003F0000000100000001000000050000000000001388000A00000000000000000000000100000002000000560000000001000E00004B64000088B800000708007B0000007B0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001003F00000001000000010000000500"
			"00000000000FA0000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000004B0007B0000007C0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000704000000200001003F0000000100000001000000080000000000000FA0000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000003C0007B0000007D0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F00000000000000010000EA600000000000001388000A00000000000000000000000100000002000000560000000001000E000000000000433000000708007B0000007E0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000075300000000000001388000A00000000000000000000000100000002000000560000000001000E000000000000433000000708007B0000007F0000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001003F0000000000000001000061A80000000000001388000A00000000000000000000000100000002000000560000000001000E0000000000004330000003E800AA000000800000000DFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000D00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000001388000A00000000000000000000000100000002000000560000000001000E0000000000004330000009C400AA000000810000000DFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000D00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000560000000001000E00000000000043300000070800AA000000820000000DFFFFFFFF000007D000000000000000000026000000010000000000000002000E0000000B000000010000AFC8000A0000000D00000000000200290000000000000001000186A00000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000560000000001000E000000000000433000000320006F000000830000000DFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000500000001000000000001003700000000000000010000EA600000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000001F4006F000000840000000DFFFFFFFF000000000000000000000000001C000000010000000100000001000E000000050000000100000000000100370000000000000001000075300000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000001F4006F000000850000000DFFFFFFFF000000000000000000000000001C000000010000000100000001000E00000005000000010000000000010037000000000000000100003A980000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000001F400A4000000860000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200310000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C0000019000003F00000002000000010000000100000000FFFFFFFF000A000000000000000000000001000000020000000C0000000001000E00004B64000088B8000004B000A4000000870000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200310000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C0000009600003F00000002000000010000000100000000FFFFFFFF000A000000000000000000000001000000020000000C0000000001000E00004B64000088B80000034800A4000000880000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000200310000000000000001000186A000000000FFFFFFFF000A000000000000000000000001000000020000000C0000009600003F00000002000000010000000100000000FFFFFFFF000A000000000000000000000001000000020000000C0000000001000E00004B64000088B80000016800A4000000890000000DFFFFFFFF0000000000000000000000000020000000010000000100000001001200000002000000020000010000000000000200290000000100000001000000020000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000003C000A40000008A0000000DFFFFFFFF0000000000000000000000000020000000010000000100000001001200000002000000020000010000000000000200290000000100000001000000030000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000002D000A40000008B0000000DFFFFFFFF0000000000000000000000000020000000010000000100000001001200000002000000020000010000000000000200290000000100000001000000040000000100000000000A0000000000000000000000060000000000003F0000000200000001000000010000000000000BB8000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000001E000730000008C0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A00000000000007530000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000005A000730000008D0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A00000000000004E20000A00000000000000000000000100000002000000560000000001000E00004B64000088B80000043800730000008E0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A00000000000003A98000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000002D000650000008F0000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100001F40000065000000900000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100000FA0000065000000910000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001002D0000000000000001000186A00000000100000000000A00000000000000000000000700000001000007D0000065000000920000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D00000000000000010000EA600000000100000000000A0000000000000000000000070000000100011170000065000000930000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D00000000000000010000C3500000000100000000000A0000000000000000000000070000000100009C40000065000000940000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D0000000000000001000075300000000100000000000A00000000000000000000000700000001000080E8000077000000950000000DFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000400000001FFFFFFFF0001003F000000000000000100009C400000000000001770000A00000000000000000000000100000002000000010000000001000E00004B64000088B8000001900077000000960000000DFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000400000001FFFFFFFF0001003F0000000000000001000061A80000000000001770000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000014D0077000000970000000DFFFFFFFF000000000000000000000000001C000000010000000100000001000E0000000400000001FFFFFFFF0001003F000000000000000100004E200000000000000FA0000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000010B0065000000980000000D000000010000000000000000000000000020000000000000000100000001001200000002000000020000000000000010000100290000000000000001000186A00000000100000000000A00000000000000000000000800000000000065000000990000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000090000000100000BB80001002D0000000000000001000186A00000000100000000000A0000000000000000000000070000000100001F400000610000009A0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001002D0000000000000001000186A000000000FFFFFFFF000A00000000000000000000000300000001000005DC0000610000009B0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001002D0000000000000001000186A000000000FFFFFFFF000A00000000000000000000000300000001000003E80000AA0000009C0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000002004D0000000000000001000186A00000000000004E20000A00000000000000000000000100000002000000010000000002000E0000000000004B64000001F4000E00004B64000088B80000019000290000000200000001000000010000000100000000000A000000000000000000000008000000000000AA0000009D0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000002004D0000000000000001000186A00000000000002710000A00000000000000000000000100000002000000010000000002000E0000000000004B64000001F4000E00004B64000088B80000014D00290000000200000001000000010000000100000000000A000000000000000000000008000000000000AA0000009E0000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000002004D0000000000000001000186A00000000000001F40000A00000000000000000000000100000002000000010000000002000E0000000000004B6400000190000E00004B64000088B80000010B00290000000200000001000000010000000100000000000A000000000000000000000008000000000000650000009F0000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000010000000100004E200001002D0000000000000001000186A00000000000004E20000A0000000000000000000000030000000100000BB8000065000000A00000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000100000001000027100001002D0000000000000001000186A00000000000002710000A0000000000000000000000030000000100000BB8000065000000A10000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000010000000100001F400001002D0000000000000001000186A00000000000001F40000A00000000000000000000000300000001000007D0000069000000A20000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001002D0000000000000001000061A80000000100000000000A00000000000000000000000700000001000059D8000069000000A30000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001002D0000000000000001000032C80000000100000000000A00000000000000000000000700000001000059D8000069000000A40000000DFFFFFFFF00000000000000000000000000200000000000000001000000010012000000020000000200000000000000100001002D0000000000000001000027100000000100000000000A0000000000000000000000070000000100003A98000069000000A50000000EFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000150000000100003A98000100310000000000000001000186A00000000000003A98000A00000000000000000000000400000002000186A0000186A0000061000000A60000000EFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000150000000100003A98000100290000000000000001000186A00000000000003A98000A00000000000000000000000900000000000065000000A70000000EFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000150000000100003A980001002D0000000000000001000186A00000000000003A98000A0000000000000000000000020000000100000BB800006D000000A80000000FFFFFFFFF0000000000000000000000000024000000000000000100000001001600000016000000030000000600000001000000010001002D0000000000000001000186A00000000000003A98000A00000000000000000000000D000000010000000A00006B000000A900000010FFFFFFFF0000000000000000000000000018000000000000000100000001000A0000000000000000000100370000000000000001000186A000000000FFFFFFFF000A0000000000000000000000000000000001000E000036B0000088B80000012C0073000000AA00000010FFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A000000000FFFFFFFF000A00000000000000000000000100000002000000560000000001000E0000000000004330000003E80073000000AB00000010FFFFFFFF0000000000000000000000000018000000000000000100000001000A000000000000000000010"
			"03F0000000000000001000186A000000000FFFFFFFF000A00000000000000000000000100000002000000560000000001000E00004B64000088B8000002D00073000000AC00000010FFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A000000000FFFFFFFF000A00000000000000000000000100000002000000010000000001000E0000000000004B64000001F40073000000AD00000010FFFFFFFF0000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000186A000000000FFFFFFFF000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0073000000AE0000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0073000000AF0000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0073000000B00000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0073000000B10000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0073000000B20000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0073000000B30000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0086000000B40000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A0000001100000000000200290000000000000001000186A00000000100000000000A000000000000000000000008000000000000290000000000000001000186A00000000100000000000A00000000000000000000000800000000000071000000B50000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C0000006400000000000100310000000000000001000186A00000000100000000000A00000000000000000000000B0000000200000002000005DC000071000000B60000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C00000064000000000001003100000000000000010000C3500000000100000000000A00000000000000000000000B0000000200000002000005DC000069000000B70000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C0000006400000000000100290000000000000001000186A00000000100000000000A0000000000000000000000080000000000006F000000B80000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000120000000100000028000100370000000000000001000186A00000000000000BB8000A0000000000000000000000000000000001000E000036B0000088B8000001F4006F000000B90000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E000000120000000100000028000100370000000000000001000186A00000000000000BB8000A0000000000000000000000000000000001000E000036B0000088B8000000FA0077000000BA0000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C0000006400000000000100370000000000000001000186A00000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000005DC0077000000BB0000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C0000006400000000000100370000000000000001000186A00000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000003E80077000000BC0000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C0000006400000000000100370000000000000001000186A00000000000001F40000A0000000000000000000000000000000001000E000036B0000088B8000001F4006D000000BD0000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C00000064000000000001002D0000000000000001000186A00000000000001F40000A0000000000000000000000030000000100004E2000006D000000BE0000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C00000064000000000001002D0000000000000001000186A00000000000001F40000A0000000000000000000000030000000100003A9800006D000000BF0000000DFFFFFFFF0000000000000000000000000024000000000000000100000001001600000017000000030000003C00000064000000000001002D0000000000000001000186A00000000000001F40000A0000000000000000000000030000000100002710000073000000C00000000D000000000000000000000000000000000018000000000000000100000001000A00000000000000000001003F0000000000000001000000000000000100000000000A00000000000000000000000100000002000000010000000001000E00004B64000088B80000012C0065000000C10000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D00000000000000010000EA600000000100000000000A0000000000000000000000070000000100015F90000065000000C20000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D00000000000000010000EA600000000100000000000A0000000000000000000000070000000100011170000065000000C30000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000B000000010000AFC80001002D00000000000000010000EA600000000100000000000A00000000000000000000000700000001000080E80000AA000000C40000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000002004D0000000000000001000186A00000000000004E20000A00000000000000000000000100000002000000010000000002000E0000000000004B64000001F4000E00004B64000088B80000019000290000000200000001000000010000000100000000000A000000000000000000000008000000000000AA000000C50000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000002004D0000000000000001000186A00000000000002710000A00000000000000000000000100000002000000010000000002000E0000000000004B64000001F4000E00004B64000088B80000014D00290000000200000001000000010000000100000000000A000000000000000000000008000000000000AA000000C60000000DFFFFFFFF0000000000000000000000000018000000000000000100000001000A00000014000000000002004D0000000000000001000186A00000000000001F40000A00000000000000000000000100000002000000010000000002000E0000000000004B6400000190000E00004B64000088B80000010B00290000000200000001000000010000000100000000000A00000000000000000000000800000000000077000000C70000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F00000000000000010000EA600000000000001F40000A00000000000000000000000100000002000000010000000001000E0000000000004B64000002EE0077000000C80000000DFFFFFFFF000000000000000000000000001C000000000000000100000001000E0000000800000001000000010001003F00000000000000010000C3500000000000001B58000A00000000000000000000000100000002000000010000000001000E0000000000004B64000001F40C000F0000000700000001000003E8000035000000000000000003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710003D000000010000000200000001000007D003000E0000000000001F4000002710000E00001F4000003A9800002710000E00003A980000520800002710000F00000003000000010000C35000000F00000002000000010000753000000F000000050000000100061A800001");
		len = p - buf.data();
		SendToClient(Client, 28404, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	}
}
