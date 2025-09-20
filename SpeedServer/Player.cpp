#include "Message.h"
#include<unordered_map>
#include <stdio.h>
#include <time.h>
#include <map>
#include <string>
#include <mutex>
#include "Database.h"
#include "Item.h"
#include "yaml-cpp/yaml.h"
#include <codecvt>
#include "pet.h"
#include "DBConnection.h"
#include <algorithm>

//#include "ConmData.h"
extern std::map<long, std::unique_ptr<RoomNode>> Rooms;
extern std::map<long, std::shared_ptr<ClientNode>> Clients;
extern std::recursive_mutex Mutex;
extern std::recursive_mutex ClientMutex;
extern std::shared_ptr<ClientNode> GetClient(long Uin);
extern std::shared_ptr<ClientNode> GetRelaxClient(long Uin);
extern void NotifyRankedMatchSeasonInfo(std::shared_ptr<ClientNode> Client, UINT SrcUin);
extern void ResponseSaveShapeRefit4(std::shared_ptr<ClientNode> Client, int KartID);
extern void SelectGlobleStone(std::shared_ptr<ClientNode> Client);
extern UINT RankGradeLevel[8];
extern int deleteflag;
extern ConnectionPool player;
extern UINT GetChange(UINT ShapeSuitID);
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> RegisterBoxs;//hashmap
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> DayRaffleBoxs;//hashmap
extern std::string GetItemMess(UINT ID);
extern std::string findItemSkillStoneById(std::shared_ptr<ClientNode> Client, int id);
//extern void NotifySkillStoneKartInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, KartStoneItemInfo* Items, bool HasMoreInfo);
//extern void NotifySkillStoneKartMoreInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, KartStoneItemInfo* Items, bool HasMoreInfo);

//顶号消息
BYTE msg900[52] = { 0 };

void NotifyUseItemResult2(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum);
void NotifySpecialActivityInfo(std::shared_ptr<ClientNode> Client);
void ResponseLogin(std::shared_ptr<ClientNode> Client, UINT ClientVersion, UINT LoginType, UINT ItemNum, std::vector<ItemInfo> Items, bool HasMoreInfo);
void ResponseKartRefitInfoMoreInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, KartRefitItemInfo* Items, bool HasMoreInfo);




extern std::unordered_map<long, std::unique_ptr<PETITEM>> PetItems;
BOOL IsUserInTopList(long Uin);
std::vector<UINT> RankTopList(30);
std::vector<UINT> CharmTopList(30);
std::vector<UINT> HonorTopList(30);
std::vector<UINT> KartTopList(30);
std::vector<UINT> ExperienceTopList(30);
std::vector<UINT> BugletTopList(30);
std::vector<UINT> GuildTopList(30);
std::vector<UINT> LoverTopList(30);
std::vector<UINT> NobleTopList(30);
std::vector<UINT> EmperorTopList(30);
std::vector<UINT> GuildScoreTopList(30);
std::vector<UINT> GardenTopList(30);
std::vector<UINT> DanceExpTopList(30);
std::vector<UINT> MemProsperity(30);
extern void DeleteClientBySystem(std::shared_ptr<ClientNode> Client);
extern std::unordered_map<UINT, std::unique_ptr<Item>> Items;//hashmap
std::map<long, std::shared_ptr<ClientNode>> RelaxClients;
extern std::recursive_mutex RelaxClientMutex;
extern UINT GetItemGameType(UINT ID);
extern int Borderset[6];
extern std::shared_ptr<ClientNode> SystemClient;
extern int BorderTeam;
extern std::shared_ptr<RoomNode> NewRoom();
extern std::string bignotice;
extern std::string smallnotice;
extern int notice;
extern int isLog;
extern std::string UITitleDispInfo_ADText;
extern std::chrono::high_resolution_clock::time_point start;
extern int StoneLimit;
extern int NewKart;
extern int PreventSDP[2];
extern int loginMode;
extern int topnew[2];
extern std::string topnews;
extern std::string qqurl;
extern int Uinew;
std::set<UINT> SpecialLoginUins; // 定义特殊登录用户集合

std::string GetClientNames(std::string type) {

	std::string msg;
	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	msg = "当前在线人数：" + std::to_string(Clients.size());
	if (type == "全服人数名字") {
		msg = msg + "  名称：";
		int index = 1; // 初始化序号
		std::map<long, std::shared_ptr<ClientNode>>::iterator iter = Clients.begin();
		while (iter != Clients.end())
		{
			std::shared_ptr<ClientNode> DstClient = iter->second;
			if (DstClient)
			{
				std::string result1(DstClient->NickName); // 将 char 数组转换为 std::string
				msg += std::to_string(index++) + "." + result1 + "  "; // 添加序号和昵称
			}
			iter++;
		}
	}

	return msg;
}

void NotifyBroadcast17()
{
	if (topnew[0])
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		std::string news;
		if (topnews == "全服人数" || topnews == "全服人数名字") {
			news = GetClientNames(topnews);
		}
		else {
			news = topnews;
		}

		if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
			return; // 超出缓冲区，跳过这个广播
		len = NotifyBroadcast(p, 17, 10, 0, news) - buf.data();

		std::lock_guard<std::recursive_mutex> lg(ClientMutex);
		std::map<long, std::shared_ptr<ClientNode>>::iterator iter = Clients.begin();
		while (iter != Clients.end())
		{
			std::shared_ptr<ClientNode> DstClient = iter->second;
			if (DstClient)
			{
				SendToClient(DstClient, 541, buf.data(), len, DstClient->ConnID, FE_PLAYER, DstClient->ConnID, Notify);
			}
			iter++;
		}
	}

}



void NotifyBroadcast17(std::string news)
{
	try
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

		if (news.length() + 64 >= buf.size()) // 64 是其他字段的估计大小
			return; // 超出缓冲区，跳过这个广播
		len = NotifyBroadcast(p, 17, 10, 0, news) - buf.data();

		std::lock_guard<std::recursive_mutex> lg(ClientMutex);
		std::map<long, std::shared_ptr<ClientNode>>::iterator iter = Clients.begin();
		while (iter != Clients.end())
		{
			std::shared_ptr<ClientNode> DstClient = iter->second;
			if (DstClient)
			{
				SendToClient(DstClient, 541, buf.data(), len, DstClient->ConnID, FE_PLAYER, DstClient->ConnID, Notify);
			}
			iter++;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "发送消息失败!\n" << std::endl;
	}
	catch (...) {
		std::cerr << "发送消息失败!\n" << std::endl;
	}

}


void RunningLog()
{
	int num = Clients.size();  // Get the number of clients

	auto end = std::chrono::high_resolution_clock::now();  // Stop measuring time
	std::chrono::duration<double> elapsed = end - start;   // Calculate the elapsed time

	// Convert elapsed time to minutes, hours, and days
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
	int days = elapsed_seconds / 86400;  // Number of seconds in a day
	int hours = (elapsed_seconds % 86400) / 3600;  // Number of seconds in an hour
	int minutes = (elapsed_seconds % 3600) / 60;  // Number of seconds in a minute
	int seconds = elapsed_seconds % 60;  // Remaining seconds

	// Format the output string
	std::string onlinetext = UITitleDispInfo_ADText +
		" ---- 当前在线人数：" + std::to_string(num) +
		" ---- 运行时长：";

	if (days > 0)
	{
		onlinetext += std::to_string(days) + " 天 ";
	}
	if (hours > 0 || days > 0)  // Show hours if there are days or hours
	{
		onlinetext += std::to_string(hours) + " 小时 ";
	}
	if (minutes > 0 || hours > 0 || days > 0)  // Show minutes if there are hours or days
	{
		onlinetext += std::to_string(minutes) + " 分钟 ";
	}
	onlinetext += std::to_string(seconds) + " 秒";

	SetConsoleTitle(onlinetext.c_str());  // Update the console title
}
BOOL IsUserInTopList(long Uin)
{
	for (char i = 0; i < 30; i++)
	{
		if (CharmTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (ExperienceTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (KartTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (HonorTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (RankTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (BugletTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (NobleTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (GuildTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (LoverTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (EmperorTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (GuildScoreTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (GardenTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	for (char i = 0; i < 30; i++)
	{
		if (DanceExpTopList[i] == Uin)
		{
			return TRUE;
		}
	}
	return FALSE;

}
void UpdateTopList()
{
	//荣誉红人数组
	SelectTopList("SELECT Uin FROM BaseInfo order by TotalHonor DESC limit 0,30;", HonorTopList);
	//魅力红人数组
	SelectTopList("SELECT Uin FROM BaseInfo order by Charm DESC limit 0,30;", CharmTopList);
	//排位红人数组
	SelectTopList("SELECT R.Uin FROM RankInfo R join BaseInfo B on R.Uin=B.Uin order by R.GradeLevel DESC,R.GradeScore DESC limit 0,30;", RankTopList);
	//赛车红人数组
	//SelectTopList("select I.Uin as Uin,count(I.ItemType) as KartNum  From Item I Join BaseInfo B on I.Uin=B.Uin where ItemType=1 group by I.Uin order by KartNum DESC limit 0,30;", KartTopList);
	SelectTopList("select Uin ,count(1) as KartNum from kartrefit GROUP BY uin  order by KartNum DESC limit 0,30;", KartTopList);
	//等级红人数组
	SelectTopList("SELECT Uin FROM BaseInfo order by Experience DESC limit 0,30;", ExperienceTopList);
	//喇叭红人数组
	SelectTopList("SELECT Uin FROM BaseInfo order by BugletTalk DESC limit 0,30;", BugletTopList);

	//车队红人数组
	SelectTopList("SELECT Uin FROM Player order by GuildVipPoint DESC limit 0,30;", GuildTopList);
	//皇族红人数组
	SelectTopList("SELECT Uin FROM Player order by NoblePoint DESC limit 0,30;", NobleTopList);
	//情侣红人数组
	SelectTopList("SELECT Uin FROM Player order by LoverVipPoint DESC limit 0,30;", LoverTopList);
	//钻皇红人数组
	SelectTopList("SELECT Uin FROM Player order by EmperorPoint DESC limit 0,30;", EmperorTopList);

	SelectTopList("SELECT Uin FROM Player order by GardenPoint DESC limit 0,30;", GardenTopList);

	SelectTopList("SELECT Uin FROM BaseInfo order by DanceExp DESC limit 0,30;", DanceExpTopList);
	SelectTopList("SELECT Uin FROM GuildInfo order by MemProsperity DESC limit 0,30;", MemProsperity);
}
void LoadTopList()
{
	UpdateTopList();
}



BOOL WritePlayerDBBaseInfo(BYTE*& p, UINT Uin, std::shared_ptr<ClientNode> Client)
{ //PlayerDBBaseInfo

	WORD len;
	BYTE* pPlayerDBBaseInfo = p;
	Write16(pPlayerDBBaseInfo, 0); //len
	char Gender = 0;
	char Country = 0;
	UINT License = 0;
	UINT Experience = 0;
	UINT SuperMoney = 0;
	UINT Money = 0;
	UINT WinNum = 0;
	UINT SecondNum = 0;
	UINT ThirdNum = 0;
	UINT TotalNum = 0;
	UINT CurHonor = 0;
	UINT TotalHonor = 0;
	UINT TodayHonor = 0;
	UINT RelaxTime = 0;
	UINT MonthDurationBefor = 0;
	UINT MonthDurationCur = 0;
	UINT Charm = 0;
	UINT DurationGame = 0;
	UINT DanceExp = 0;
	UINT Coupons = 0;
	UINT Admiration = 0;
	UINT LuckMoney = 0;
	UINT TeamWorkExp = 0;
	UINT AchievePoint = 0;
	UINT RegTime = 0;
	std::string Signature = "";
	std::string NickName = "";
	ClientInfo* clienInfo = new ClientInfo[1];
	PlayerDB_SelectClientInfo(Uin, clienInfo);
	if (!clienInfo) {
		return false;
	}
	

	Gender = clienInfo[0].Gender;
	Country = clienInfo[0].Country;
	License = clienInfo[0].License;
	Experience = clienInfo[0].Experience;
	SuperMoney = clienInfo[0].SuperMoney;
	Money = clienInfo[0].Money;
	WinNum = clienInfo[0].WinNum;
	SecondNum = clienInfo[0].SecondNum;
	ThirdNum = clienInfo[0].ThirdNum;
	TotalNum = clienInfo[0].TotalNum;
	CurHonor = clienInfo[0].CurHonor;
	TotalHonor = clienInfo[0].TotalHonor;
	TodayHonor = clienInfo[0].TodayHonor;
	RelaxTime = clienInfo[0].RelaxTime;
	MonthDurationBefor = clienInfo[0].MonthDurationBefor;
	MonthDurationCur = clienInfo[0].MonthDurationCur;
	Charm = clienInfo[0].Charm;
	DurationGame = clienInfo[0].DurationGame;
	DanceExp = clienInfo[0].DanceExp;
	Coupons = clienInfo[0].Coupons;
	Admiration = clienInfo[0].Admiration;
	LuckMoney = clienInfo[0].LuckMoney;
	TeamWorkExp = clienInfo[0].TeamWorkExp;
	AchievePoint = clienInfo[0].AchievePoint;
	RegTime = clienInfo[0].RegTime;
	Signature = clienInfo[0].Signature;
	NickName = clienInfo[0].NickName;

	memset(pPlayerDBBaseInfo, 0, 17);
	if (!NickName.empty())
	{
		memcpy((char*)pPlayerDBBaseInfo, NickName.c_str(), NickName.length());
	}

	//WriteString(pPlayerDBBaseInfo, "秋意寒\0");

	pPlayerDBBaseInfo += MaxNickName;

	if (Client && Client->Uin == Uin) {
		Client->Gender = Gender;
	}

	Write8(pPlayerDBBaseInfo, Gender | 0x48);//性别
	Write8(pPlayerDBBaseInfo, Country);
	Write32(pPlayerDBBaseInfo, License);//许可证，驾照
	Write32(pPlayerDBBaseInfo, Experience);//Experience 经验数量决定等级显示
	Write32(pPlayerDBBaseInfo, SuperMoney); //点券
	Write32(pPlayerDBBaseInfo, Money); //酷币
	Write32(pPlayerDBBaseInfo, WinNum);
	Write32(pPlayerDBBaseInfo, SecondNum);
	Write32(pPlayerDBBaseInfo, ThirdNum);
	Write32(pPlayerDBBaseInfo, TotalNum);
	Write32(pPlayerDBBaseInfo, CurHonor);//荣誉
	Write32(pPlayerDBBaseInfo, TotalHonor);
	Write32(pPlayerDBBaseInfo, TodayHonor);
	Write32(pPlayerDBBaseInfo, RelaxTime);//休闲时间
	Write32(pPlayerDBBaseInfo, MonthDurationBefor);
	Write32(pPlayerDBBaseInfo, MonthDurationCur);
	Write32(pPlayerDBBaseInfo, Charm);//魅力值
	Write32(pPlayerDBBaseInfo, DurationGame);
	Write32(pPlayerDBBaseInfo, DanceExp);
	Write32(pPlayerDBBaseInfo, Coupons); //消费券
	Write32(pPlayerDBBaseInfo, Admiration);
	Write32(pPlayerDBBaseInfo, LuckMoney);
	Write32(pPlayerDBBaseInfo, TeamWorkExp);
	Write32(pPlayerDBBaseInfo, AchievePoint);//成就点
	Write32(pPlayerDBBaseInfo, RegTime);


	if (pPlayerDBBaseInfo && !Signature.empty()) {
		size_t newSignatureLength = Signature.length();
		if (newSignatureLength < MaxSignature) {
			strncpy_s((char*)pPlayerDBBaseInfo, MaxSignature, Signature.c_str(), _TRUNCATE);
			// 手动添加字符串结尾的 null 字符
			pPlayerDBBaseInfo[MaxSignature - 1] = '\0';
		}
	}


	/*if (strlen(NewSignature) < MaxSignature) {
		strcpy_s((char*)pPlayerDBBaseInfo, MaxSignature, NewSignature);
	}*/

	pPlayerDBBaseInfo += MaxSignature;

	Write32(pPlayerDBBaseInfo, 0); //LuckyMatchPoint
	Write32(pPlayerDBBaseInfo, 0); //LuckyMatchNum
	Write16(pPlayerDBBaseInfo, 0); //LuckyMatchFirstNum
	Write8(pPlayerDBBaseInfo, 0); //LuckyMatchBestRand
	Write32(pPlayerDBBaseInfo, 0); //LuckyMatchWinNum
	Write32(pPlayerDBBaseInfo, 0); //FizzPointTotal
	Write16(pPlayerDBBaseInfo, 0); //FizzPointDaily
	Write16(pPlayerDBBaseInfo, 0); //FizzPointWeekly
	Write16(pPlayerDBBaseInfo, 0); //FizzPointLastWeek 上周流行音乐
	Write8(pPlayerDBBaseInfo, 0); //FizzLotteryStatus
	Write32(pPlayerDBBaseInfo, 0); //FizzLastUpdateTime
	Write32(pPlayerDBBaseInfo, 0); //SNSLocaleCode 
	Write32(pPlayerDBBaseInfo, 0); //AuctionRightUnlockTime 
	Write8(pPlayerDBBaseInfo, 0); //ReserveFlag 保留标志
	Write32(pPlayerDBBaseInfo, 0); //CurConsumeScore当前消费分数
	Write32(pPlayerDBBaseInfo, 0); //HistoryConsumeScore
	Write8(pPlayerDBBaseInfo, 0); //CrashModeSponsorID崩溃模式发起人id
	Write32(pPlayerDBBaseInfo, 0); //Popularity人气
	Write32(pPlayerDBBaseInfo, 0); //LadderMatchAchievePoint阶梯匹配实现点
	Write32(pPlayerDBBaseInfo, 0); //Cash现金
	Write32(pPlayerDBBaseInfo, 0); //PlayerGPMIdentity玩家GPM身份
	Write32(pPlayerDBBaseInfo, 0); //PointTotalScore积分总分
	Write32(pPlayerDBBaseInfo, 0); //totalMapMedalNum
	Write32(pPlayerDBBaseInfo, 0); //lastMapMedalNum
	Write32(pPlayerDBBaseInfo, 0); //curMapMedalNum
	Write32(pPlayerDBBaseInfo, 0); //curMapMedalSeasonId
	Write32(pPlayerDBBaseInfo, 0); //lastClearMapMedalTime
#ifndef ZingSpeed
	Write32(pPlayerDBBaseInfo, 0); //max2048Score
	Write32(pPlayerDBBaseInfo, 0); //curPlay2048LeftNum
	Write32(pPlayerDBBaseInfo, 0); //CheerValue欢呼值
	Write16(pPlayerDBBaseInfo, 0x2b); //SeasonID赛季id
	Write32(pPlayerDBBaseInfo, 0); //LastCheerValue
	Write32(pPlayerDBBaseInfo, 0); //SpeedBean飞车豆
	Write32(pPlayerDBBaseInfo, 0); //SpeedCoin飞车币
#endif

	len = pPlayerDBBaseInfo - p;
	Set16(p, len);
	p += len;

	delete[] clienInfo;

	return true;
}

//void WritePlayerGuildInfo(BYTE*& p, UINT Uin, std::shared_ptr<ClientNode> Client)
//{ //PlayerGuildInfo
//	WORD len;
//	
//	
//	int result;
//	char GuildName[17]; // 假设 MAX_NAME_LENGTH 是足够大的常量
//	char* newname = NULL;
//	int GuildID = 0;
//	char Duty = 0;
//	BYTE* pPlayerGuildInfo = p;
//	Write16(pPlayerGuildInfo, 0); //len
//
//	Guidinfo* guidinfo = new Guidinfo[1];
//	PlayerDB_SelectGuildInfoByUin(Uin, guidinfo); 
//
//	if (!guidinfo) {
//		return;
//	}
//	GuildID = guidinfo[0].ID;
//	Duty = guidinfo[0].Duty;
//
//	if (guidinfo[0].Name && strlen(guidinfo[0].Name) < 17) {
//		strncpy(GuildName, guidinfo[0].Name, 16); // 保留一个额外的空间给 null 字符
//		GuildName[16] = '\0'; // 手动添加字符串结尾的 null 字符
//	}
//	// 释放动态分配的内存
//	if (guidinfo[0].Name) {
//		free(guidinfo[0].Name);
//		guidinfo[0].Name = nullptr;
//	}
//	
//	if (GuildName) {
//		newname = myutf8trans(GuildName);
//	}
//
//	memset(pPlayerGuildInfo, 0, 17);
//	if (newname)
//	{
//		memcpy(pPlayerGuildInfo, newname, strlen(newname));
//		delete[] newname;
//	}
//	pPlayerGuildInfo += 17;
//
//	Write16(pPlayerGuildInfo, Duty); //Duty 
//	Write32(pPlayerGuildInfo, GuildID); //GuildID
//	Write32(pPlayerGuildInfo, 0); //Right
//	Write32(pPlayerGuildInfo, 0); //Medal
//	Write32(pPlayerGuildInfo, 0); //Contribute
//
//	memset(pPlayerGuildInfo, 0, 9); //m_szDutyTitle[]:
//	pPlayerGuildInfo += 9;
//
//	len = pPlayerGuildInfo - p;
//	Set16(p, len);
//	p += len;
//
//	delete[] guidinfo;
//}

void WritePlayerGuildInfo(BYTE*& p, UINT Uin, std::shared_ptr<ClientNode> Client)
{ //PlayerGuildInfo
	WORD len;
	
	int result;
	std::string GuildName = "";
	int GuildID = 0;
	char Duty = 0;
	BYTE* pPlayerGuildInfo = p;
	Write16(pPlayerGuildInfo, 0); //len


	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT GI.GuildID,GI.Duty,GL.Name FROM GuildInfo GI join GuildList GL on GI.GuildID=GL.ID where GI.Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			GuildID = res->getInt("GuildID");
			Duty = res->getInt("Duty");
			GuildName = UTF2GB(res->getString("Name").c_str());
			break;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("WritePlayerGuildInfo error\n");
	}
	memset(pPlayerGuildInfo, 0, 17);
	if (!GuildName.empty())
	{
		memcpy(pPlayerGuildInfo, GuildName.c_str(), GuildName.length());
	}
	pPlayerGuildInfo += 17;

	Write16(pPlayerGuildInfo, Duty); //Duty 
	Write32(pPlayerGuildInfo, GuildID); //GuildID
	Write32(pPlayerGuildInfo, 0); //Right
	Write32(pPlayerGuildInfo, 0); //Medal
	Write32(pPlayerGuildInfo, 0); //Contribute

	memset(pPlayerGuildInfo, 0, 9); //m_szDutyTitle[]:
	pPlayerGuildInfo += 9;

	len = pPlayerGuildInfo - p;
	Set16(p, len);
	p += len;
}


UINT WriteEquipedItem(BYTE*& p, std::shared_ptr<ClientNode> Client, UINT* cPetId, BOOLEAN isBorder)//写入当前装备的项目让对方看到的
{ //EquipedItem

	WORD len;
	BYTE* pItemNum = p;
	Write16(p, 0); //EquipedItemNum

	int itemnum, presult;
	size_t j = 0;
	UINT KartItemId = 0;

	std::vector<ItemInfo> itemInfo;
	PlayerDB_SelectItemsStatus(Client->Uin, itemInfo);
	for (size_t i = 0; i < itemInfo.size(); i++)
	{
		UINT ItemID = itemInfo[i].ItemID;
		BYTE Status = itemInfo[i].Status;
		if (Status) {
			j++;
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				Client->KartID = ItemID;
				if (Client->KartSkinID != 0 && isBorder)
				{
					ItemID = Client->KartSkinID;
				}
				//KartItemId = ItemID;
			}
			else if (GetItemType(ItemID) == EAIT_PET)
			{
				*cPetId = ItemID;
			}
			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, ItemID);
			Write32(pItemInfo, itemInfo[i].ItemNum);
			Write32(pItemInfo, itemInfo[i].AvailPeriod);
			Write8(pItemInfo, Status);
			Write32(pItemInfo, itemInfo[i].ObtainTime);
			Write32(pItemInfo, itemInfo[i].OtherAttribute);
#ifndef ZingSpeed
			Write16(pItemInfo, 0); //ItemType
#endif
			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	Set16(pItemNum, (WORD)j);

	return KartItemId;

}

UINT WriteEquipedItem(BYTE*& p, std::shared_ptr<ClientNode> Client)//写入当前装备的项目让对方看到的
{ //EquipedItem
	WORD len;
	BYTE* pItemNum = p;
	Write16(p, 0); //EquipedItemNum

	int itemnum, presult;
	size_t j = 0;
	UINT KartItemId = 0;

	std::vector<ItemInfo> vecItem;
	PlayerDB_SelectItemsStatus(Client->Uin, vecItem);
	for (size_t i = 0; i < vecItem.size(); i++)
	{
		UINT ItemID = vecItem[i].ItemID;
		BOOL Status = vecItem[i].Status;
		if (Status) {
			j++;
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				Client->KartID = ItemID;
				if (Client->KartSkinID != 0)
				{
					ItemID = Client->KartSkinID;
				}
				//KartItemId = ItemID;
			}

			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, ItemID);
			Write32(pItemInfo, vecItem[i].ItemNum);
			Write32(pItemInfo, vecItem[i].AvailPeriod);
			Write8(pItemInfo, vecItem[i].Status);
			Write32(pItemInfo, vecItem[i].ObtainTime);
			Write32(pItemInfo, vecItem[i].OtherAttribute);
#ifndef ZingSpeed
			Write16(pItemInfo, 0); //ItemType
#endif
			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	Set16(pItemNum, (WORD)j);
	return KartItemId;
}

UINT WriteEquipedItem2(BYTE*& p, long Uin, std::shared_ptr<ClientNode> Client)//写入当前装备的项目让对方看到的
{ //EquipedItem
	WORD len;
	BYTE* pItemNum = p;
	Write16(p, 0); //EquipedItemNum

	int itemnum, presult;
	size_t j = 0;
	UINT KartItemId = 0;

	std::vector<ItemInfo> vecItem;
	PlayerDB_SelectItemsStatus(Uin, vecItem);
	for (size_t i = 0; i < vecItem.size(); i++)
	{
		UINT ItemID = vecItem[i].ItemID;
		BOOL Status = vecItem[i].Status;
		if (Status) {
			j++;
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				KartItemId = ItemID;
			}

			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, ItemID);
			Write32(pItemInfo, vecItem[i].ItemNum);
			Write32(pItemInfo, vecItem[i].AvailPeriod);
			Write8(pItemInfo, vecItem[i].Status);
			Write32(pItemInfo, vecItem[i].ObtainTime);
			Write32(pItemInfo, vecItem[i].OtherAttribute);
#ifndef ZingSpeed
			Write16(pItemInfo, 0); //ItemType
#endif
			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	Set16(pItemNum, (WORD)j);
	return KartItemId;
}

void WriteVisbleInfo1(std::shared_ptr<ClientNode> Client, BYTE*& p, UINT Uin, char SeatID, short PlayerID)//写其他玩家可见信息
{ //PlayerRoomVisibleInfo
	//printf("PlayerID：%d\n", PlayerID);

	WORD len;
	UINT PetId = 0;
	int Identity = ID_IDENTIFY_QQLEVEL1 | ID_IDENTIFY_QQLEVEL2 | ID_IDENTIFY_QQLEVEL3 | ID_IDENTIFY_SPEEDMEMBER | ID_IDENTIFY_QQFLAG | ID_IDENTIFY_HAVEWORD | ID_IDENTIFY_FINISHQQFLAG;

	BYTE* pPlayerRoomVisibleInfo = p;
	Write16(pPlayerRoomVisibleInfo, 0); //len

	Write32(pPlayerRoomVisibleInfo, Uin); //Uin
	if (HonorTopList[0] == Uin)
	{
		Identity |= ID_IDENTIFY_KING;
	}
	else if (HonorTopList[1] == Uin || HonorTopList[2] == Uin)
	{
		Identity |= ID_IDENTIFY_INFANTE;
	}
	Write32(pPlayerRoomVisibleInfo, Identity); //Identity
	Write8(pPlayerRoomVisibleInfo, SeatID); //SeatID
	Write8(pPlayerRoomVisibleInfo, Client->TeamID); //TeamID
	Write16(pPlayerRoomVisibleInfo, PlayerID); //PlayerID
	Write8(pPlayerRoomVisibleInfo, 0); //Status

	WritePlayerDBBaseInfo(pPlayerRoomVisibleInfo, Uin, Client);
	WritePlayerGuildInfo(pPlayerRoomVisibleInfo, Uin, Client);
	//WriteEquipedItem(pPlayerRoomVisibleInfo, Client);
	WriteEquipedItem(pPlayerRoomVisibleInfo, Client, &PetId, true);
	int PetNum = PetId > 1 ? 1 : 0;
	if (PetNum) {
		Write16(pPlayerRoomVisibleInfo, 1); //PetNum in WriteVisbleInfo
		{
			BYTE* pPetInfo = pPlayerRoomVisibleInfo;
			Write16(pPetInfo, 0);	  // len
			Write32(pPetInfo, Uin);	  // Uin
			Write32(pPetInfo, PetId); // PetId
			if (GetClient(Uin)) {
				memcpy(pPetInfo, GetClient(Uin)->Pet->Name, 23);
				pPetInfo += 23;
				Write8(pPetInfo, 1);   // IsProper
				Write16(pPetInfo, 30); // Level
				Write8(pPetInfo, GetClient(Uin)->Pet->Status);   // Status
			}
			else {
				memcpy(pPetInfo, "宠物", 23);
				pPetInfo += 23;
				Write8(pPetInfo, 1);   // IsProper
				Write16(pPetInfo, 30); // Level
				Write8(pPetInfo, GetPetStatus(30, PetId));   // Status
			}

			Write16(pPetInfo, 0);  // PetSkillNum
			for (size_t i = 0; i < 0; i++)
			{ // PetSkillList
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); // len

				Write16(pPetSkillList, 0);	  // SkillID
				Write8(pPetSkillList, i + 1); // Status
				Write8(pPetSkillList, 1);	  // Active
				Write32(pPetSkillList, 0);	  // Value

				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			if (GetClient(Uin)) {
				Write32(pPetInfo, GetClient(Uin)->Pet->StrengLevel); // StrengLevel
			}
			else {
				Write32(pPetInfo, 1); // StrengLevel
			}

			Write8(pPetInfo, 4);  // TopStatus
			{					  // CommonInfo
				BYTE* pCommonInfo = pPetInfo;
				Write16(pCommonInfo, 0); // len

				Write32(pCommonInfo, 0); // HuanHuaLevel
				Write8(pCommonInfo, 0);	 // CanHuanHua

				len = pCommonInfo - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			Write16(pPetInfo, 0); // PKStar
			Write32(pPetInfo, 0); // PetSkinID

			len = pPetInfo - pPlayerRoomVisibleInfo;
			Set16(pPlayerRoomVisibleInfo, (WORD)len);
			pPlayerRoomVisibleInfo += len;
		}
	}
	else {
		Write16(pPlayerRoomVisibleInfo, 0); //PetNum in WriteVisbleInfo
	}


	{ //PlayerGuildMatchInfo
		BYTE* pPlayerGuildMatchInfo = pPlayerRoomVisibleInfo;
		Write16(pPlayerGuildMatchInfo, 0); //len

		Write32(pPlayerGuildMatchInfo, 0); //SelfPoint
		Write32(pPlayerGuildMatchInfo, 0); //WinNum
		Write32(pPlayerGuildMatchInfo, 0); //TotalNum

		len = pPlayerGuildMatchInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	char IsInTopList = 0;
	IsInTopList = IsUserInTopList(Uin);

	Write8(pPlayerRoomVisibleInfo, IsInTopList); //IsInTopList
	Write8(pPlayerRoomVisibleInfo, 0); //LoverRaceOrPro
	Write8(pPlayerRoomVisibleInfo, 0); //TmpEffectNum
	//m_stPlayerRoomVisibleInfo[].m_aiTmpEffectItems[]


	Write8(pPlayerRoomVisibleInfo, 0); //OBState
	Write8(pPlayerRoomVisibleInfo, 0); //DebutOrX5
	Write8(pPlayerRoomVisibleInfo, 0); //RandKeyFlag


	Write16(pPlayerRoomVisibleInfo, 0x0107); //VipFlag

	Write8(pPlayerRoomVisibleInfo, 0); //HaveAppellation
	/*
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Type
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Level
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Status
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Difficulty
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_i32MapId
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u32Value
	*/

	{ //NobleInfo
		BYTE* pNobleInfo = pPlayerRoomVisibleInfo;
		Write16(pNobleInfo, 0); //len

		Write32(pNobleInfo, Uin); //NobleID
		Write8(pNobleInfo, 7); //NobleLevel
		Write32(pNobleInfo, 1); //NoblePoint
		Write32(pNobleInfo, 30); //NobleLeftDays

		len = pNobleInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}

	Write8(pPlayerRoomVisibleInfo, 0); //HasCarryWizard
	/*
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_ushWizardID
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_szNickname[]:
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_ushWizardType
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_bOrder
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_bEvolutionBranch
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_bIsHuanHua
	*/
#ifndef ZingSpeed
	{ //GuildVipBaseInfo
		BYTE* pGuildVipBaseInfo = pPlayerRoomVisibleInfo;
		Write16(pGuildVipBaseInfo, 0); //len

		Write8(pGuildVipBaseInfo, 1); //GuildVipLevel
		Write32(pGuildVipBaseInfo, 1); //GuildVipPoint

		len = pGuildVipBaseInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
#endif
	Write32(pPlayerRoomVisibleInfo, 0); //ExFlag

	Write8(pPlayerRoomVisibleInfo, 0); //HaveLDMBaseInfo
	/*
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u8Grade
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u32Score
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u32MaxScore
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u32MaxGrade
	*/

	Write8(pPlayerRoomVisibleInfo, 0); //HasWl
	/*
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_uiWonderLandID
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_szWonderLandName[]:
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_ushWonderLandDuty
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_ucSubType
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_bGuildGrade
	*/
#ifndef ZingSpeed
	Write8(pPlayerRoomVisibleInfo, 1); //HasLoverVip
	for (size_t i = 0; i < 1; i++)
	{ //LoverVipInfo
		BYTE* pLoverVipInfo = pPlayerRoomVisibleInfo;
		Write16(pLoverVipInfo, 0); //len

		Write8(pLoverVipInfo, 1); //LoverVipLevel
		Write32(pLoverVipInfo, 1); //LoverVipPoint
		Write8(pLoverVipInfo, 0); //GrowRate

		len = pLoverVipInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
#endif ZingSpeed
	Write8(pPlayerRoomVisibleInfo, 0); //HasBattleModeSkillInfoList
	/*
m_stPlayerRoomVisibleInfo[].m_astBattleModeSkillInfoList[].m_bSkillNum
m_stPlayerRoomVisibleInfo[].m_astBattleModeSkillInfoList[].m_astSkillInfo[].m_shSkillId
m_stPlayerRoomVisibleInfo[].m_astBattleModeSkillInfoList[].m_astSkillInfo[].m_bSkillLevel
	*/
	Write8(pPlayerRoomVisibleInfo, 1); //HasGarden
	for (size_t i = 0; i < 1; i++)
	{ //PersonalGardenBaseInfo
		BYTE* pPersonalGardenBaseInfo = pPlayerRoomVisibleInfo;
		Write16(pPersonalGardenBaseInfo, 0); //len

		Write8(pPersonalGardenBaseInfo, 6); //GardenLevel
		Write32(pPersonalGardenBaseInfo, 1); //GardenPoint
		Write8(pPersonalGardenBaseInfo, 0); //GrowRate
		Write32(pPersonalGardenBaseInfo, 0); //GardenExpireTime
		{ //SimpleInfo
			BYTE* pSimpleInfo = pPersonalGardenBaseInfo;
			Write16(pSimpleInfo, 0); //len

			Write32(pSimpleInfo, 0); //WeekPopularity
			Write32(pSimpleInfo, 0); //TotalPopularity
			Write32(pSimpleInfo, 0); //LastUpdatePopularityTime
			Write8(pSimpleInfo, 0); //PrivateType

			len = pSimpleInfo - pPersonalGardenBaseInfo;
			Set16(pPersonalGardenBaseInfo, (WORD)len);
			pPersonalGardenBaseInfo += len;
		}

		len = pPersonalGardenBaseInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	{ //ConsumeVipInfo
		BYTE* pConsumeVipInfo = pPlayerRoomVisibleInfo;
		Write16(pConsumeVipInfo, 0); //len

		Write32(pConsumeVipInfo, 9); //VipLevel
		Write32(pConsumeVipInfo, 0); //CharmValueOfMonth
		Write32(pConsumeVipInfo, 0); //SearchTreasureNums
		Write32(pConsumeVipInfo, 0); //GetTreasureNums

		len = pConsumeVipInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 1); //HaveBaby
	{ //LoverBabyInfo
		BYTE* pLoverBabyInfo = pPlayerRoomVisibleInfo;
		Write16(pLoverBabyInfo, 0); //len

		Write32(pLoverBabyInfo, 71555); // LoveBabyID
		Write32(pLoverBabyInfo, 71555); //ItemID
		Write32(pLoverBabyInfo, 5);		 // GrowValue
		Write32(pLoverBabyInfo, 5);		 // MentalValue
		Write32(pLoverBabyInfo, 5);			 // BabyProfileLevel
		Write32(pLoverBabyInfo, 5);			 // BabyStrengthLevel
		Write32(pLoverBabyInfo, 100);			 // BabyGrowLevel
		memset(pLoverBabyInfo, 0, 17);
		memcpy(pLoverBabyInfo, "宝宝1", 5); //LoveBabyNickName[]
		pLoverBabyInfo += 17;
		Write16(pLoverBabyInfo, 0); // BabyStat
		Write16(pLoverBabyInfo, Client->LoverBaby1EquipStat); // EquipStat
		Write16(pLoverBabyInfo, 5);//LoverBabyTransferdStatus

		len = pLoverBabyInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	{ //EmperorInfo钻皇信息
		BYTE* pEmperorInfo = pPlayerRoomVisibleInfo;
		Write16(pEmperorInfo, 0); //len

		Write8(pEmperorInfo, Client->EmperorLevel); //EmperorLevel
		Write32(pEmperorInfo, Client->EmperorPoint); //EmperorPoint
		Write32(pEmperorInfo, -1); //EmperorLeftDays
		Write8(pEmperorInfo, 10); //EmperorGrowRate

		len = pEmperorInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write16(pPlayerRoomVisibleInfo, 0); //RaceCardNums

	{ //GansterScoreInfo
		BYTE* pGansterScoreInfo = pPlayerRoomVisibleInfo;
		Write16(pGansterScoreInfo, 0); //len

		Write32(pGansterScoreInfo, 0); //GansterSeasonID
		Write32(pGansterScoreInfo, 0); //GansterScore
		Write32(pGansterScoreInfo, 0); //PoliceScore
		Write32(pGansterScoreInfo, 0); //TotalGansterScore

		len = pGansterScoreInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write32(pPlayerRoomVisibleInfo, 0); //OlympicId
	Write32(pPlayerRoomVisibleInfo, 0); //NPCEliminateWinTimes
	Write8(pPlayerRoomVisibleInfo, 0); //OriginTeamID
	Write8(pPlayerRoomVisibleInfo, 0); //NxNpc
	Write16(pPlayerRoomVisibleInfo, 0); //NxNpcAILevel
	{ //EquippedActiveKartInfo
		BYTE* pEquippedActiveKartInfo = pPlayerRoomVisibleInfo;
		Write16(pEquippedActiveKartInfo, 0); //len

		Write8(pEquippedActiveKartInfo, 0); //HaveActiveInfo
		Write32(pEquippedActiveKartInfo, 0); //KartID
		Write32(pEquippedActiveKartInfo, 0); //ActiveLevel

		len = pEquippedActiveKartInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 0); //SpecialActivityStatus
	Write8(pPlayerRoomVisibleInfo, 0); //AreaID
	Write8(pPlayerRoomVisibleInfo, 1); //HaveRaiseSecondBaby
	{ //SecondLoverBabyInfo
		BYTE* pSecondLoverBabyInfo = pPlayerRoomVisibleInfo;
		Write16(pSecondLoverBabyInfo, 0); //len
		Write32(pSecondLoverBabyInfo, 71554); // LoveBabyID
		Write32(pSecondLoverBabyInfo, 71554); //ItemID
		Write32(pSecondLoverBabyInfo, 5);		 // GrowValue
		Write32(pSecondLoverBabyInfo, 5);		 // MentalValue
		Write32(pSecondLoverBabyInfo, 5);			 // BabyProfileLevel
		Write32(pSecondLoverBabyInfo, 5);			 // BabyStrengthLevel
		Write32(pSecondLoverBabyInfo, 100);			 // BabyGrowLevel
		memset(pSecondLoverBabyInfo, 0, 17);
		memcpy(pSecondLoverBabyInfo, "宝宝2", 5); //LoveBabyNickName[]
		pSecondLoverBabyInfo += 17;
		Write16(pSecondLoverBabyInfo, 0); // BabyStat
		Write16(pSecondLoverBabyInfo, Client->LoverBaby2EquipStat); // EquipStat
		Write16(pSecondLoverBabyInfo, 5);//LoverBabyTransferdStatus
		len = pSecondLoverBabyInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}

	Write8(pPlayerRoomVisibleInfo, 1); //HasRankedMatchInfo
	for (size_t i = 0; i < 1; i++)
	{ //RankedMatchGradeInfo
		BYTE* pRankedMatchGradeInfo = pPlayerRoomVisibleInfo;
		Write16(pRankedMatchGradeInfo, 0); //len

		Write32(pRankedMatchGradeInfo, RankGradeLevel[Client->GradeLevel]); //GradeLevel
		Write32(pRankedMatchGradeInfo, Client->ChildLevel); //ChildLevel
		Write32(pRankedMatchGradeInfo, 0); //GradeScore
		Write32(pRankedMatchGradeInfo, 0); //MaxGradeScore
		Write32(pRankedMatchGradeInfo, 0); //TotalScore

		len = pRankedMatchGradeInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 0); //HasHuanLingChangeInfo

	Write8(pPlayerRoomVisibleInfo, 0); //EquipSealType
	Write16(pPlayerRoomVisibleInfo, 0); //Role
	Write8(pPlayerRoomVisibleInfo, 0); //SpecialActivityIDNum
	Write8(pPlayerRoomVisibleInfo, 0); //CrewID
	Write8(pPlayerRoomVisibleInfo, 1); //SpecialActivityInfoNum 职业生涯图标段位
	for (size_t i = 0; i < 1; i++)
	{ //SpecialActivityInfo
		BYTE* pSpecialActivityInfo = pPlayerRoomVisibleInfo;
		Write16(pSpecialActivityInfo, 0); //len

		Write8(pSpecialActivityInfo, 1); //ProfessionLicenseInfoNum
		for (size_t i = 0; i < 1; i++)
		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pSpecialActivityInfo;
			Write16(pProfessionLicenseInfo, 0); //len

			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel

			len = pProfessionLicenseInfo - pSpecialActivityInfo;
			Set16(pSpecialActivityInfo, (WORD)len);
			pSpecialActivityInfo += len;
		}

		len = pSpecialActivityInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 0); //ParaNum

	Write8(pPlayerRoomVisibleInfo, 0); //ItemNum

	Write8(pPlayerRoomVisibleInfo, 0); //TDCExcluItemNum

	Write8(pPlayerRoomVisibleInfo, 0); //EquipNiChangItemNum

	Write8(pPlayerRoomVisibleInfo, 0); //TestKartGameType
	Write32(pPlayerRoomVisibleInfo, 0); //GameTestKartID
	Write8(pPlayerRoomVisibleInfo, 0); //HasStarsFightingInfo

	Write8(pPlayerRoomVisibleInfo, 0); //RoomBGSetting


	len = pPlayerRoomVisibleInfo - p;
	Set16(p, len);
	p += len;
}

void WriteVisbleInfo(std::shared_ptr<ClientNode> Client, BYTE*& p, UINT Uin, char SeatID, short PlayerID)//写其他玩家可见信息
{ //PlayerRoomVisibleInfo
	//printf("PlayerID：%d\n", PlayerID);

	WORD len;
	UINT PetId = 0;
	int Identity = ID_IDENTIFY_QQLEVEL1 | ID_IDENTIFY_QQLEVEL2 | ID_IDENTIFY_QQLEVEL3 | ID_IDENTIFY_SPEEDMEMBER | ID_IDENTIFY_QQFLAG | ID_IDENTIFY_HAVEWORD | ID_IDENTIFY_FINISHQQFLAG;

	BYTE* pPlayerRoomVisibleInfo = p;
	Write16(pPlayerRoomVisibleInfo, 0); //len

	Write32(pPlayerRoomVisibleInfo, Uin); //Uin
	if (HonorTopList[0] == Uin)
	{
		Identity |= ID_IDENTIFY_KING;
	}
	else if (HonorTopList[1] == Uin || HonorTopList[2] == Uin)
	{
		Identity |= ID_IDENTIFY_INFANTE;
	}
	Write32(pPlayerRoomVisibleInfo, Identity); //Identity
	Write8(pPlayerRoomVisibleInfo, SeatID); //SeatID
	Write8(pPlayerRoomVisibleInfo, Client->TeamID); //TeamID
	Write16(pPlayerRoomVisibleInfo, PlayerID); //PlayerID
	Write8(pPlayerRoomVisibleInfo, 0); //Status

	WritePlayerDBBaseInfo(pPlayerRoomVisibleInfo, Uin, Client);
	WritePlayerGuildInfo(pPlayerRoomVisibleInfo, Uin, Client);
	WriteEquipedItem(pPlayerRoomVisibleInfo, Client, &PetId, false);

	//WriteEquipedItem(pPlayerRoomVisibleInfo, Client);

	int PetNum = PetId > 1 ? 1 : 0;

	if (PetNum) {
		Write16(pPlayerRoomVisibleInfo, 1); //PetNum in WriteVisbleInfo
		{
			BYTE* pPetInfo = pPlayerRoomVisibleInfo;
			Write16(pPetInfo, 0);	  // len
			Write32(pPetInfo, Uin);	  // Uin
			Write32(pPetInfo, PetId); // PetId
			if (GetClient(Uin)) {
				memcpy(pPetInfo, GetClient(Uin)->Pet->Name, 23);
				pPetInfo += 23;
				Write8(pPetInfo, 1);   // IsProper
				Write16(pPetInfo, 30); // Level
				Write8(pPetInfo, GetClient(Uin)->Pet->Status);   // Status
			}
			else {
				memcpy(pPetInfo, "宠物", 23);
				pPetInfo += 23;
				Write8(pPetInfo, 1);   // IsProper
				Write16(pPetInfo, 30); // Level
				Write8(pPetInfo, GetPetStatus(30, PetId));   // Status
			}

			Write16(pPetInfo, 0);  // PetSkillNum
			for (size_t i = 0; i < 0; i++)
			{ // PetSkillList
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); // len

				Write16(pPetSkillList, 0);	  // SkillID
				Write8(pPetSkillList, i + 1); // Status
				Write8(pPetSkillList, 1);	  // Active
				Write32(pPetSkillList, 0);	  // Value

				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			if (GetClient(Uin)) {
				Write32(pPetInfo, GetClient(Uin)->Pet->StrengLevel); // StrengLevel
			}
			else {
				Write32(pPetInfo, 1); // StrengLevel
			}

			Write8(pPetInfo, 4);  // TopStatus
			{					  // CommonInfo
				BYTE* pCommonInfo = pPetInfo;
				Write16(pCommonInfo, 0); // len

				Write32(pCommonInfo, 0); // HuanHuaLevel
				Write8(pCommonInfo, 0);	 // CanHuanHua

				len = pCommonInfo - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			Write16(pPetInfo, 0); // PKStar
			Write32(pPetInfo, 0); // PetSkinID

			len = pPetInfo - pPlayerRoomVisibleInfo;
			Set16(pPlayerRoomVisibleInfo, (WORD)len);
			pPlayerRoomVisibleInfo += len;
		}
	}
	else {
		Write16(pPlayerRoomVisibleInfo, 0); //PetNum in WriteVisbleInfo
	}


	{ //PlayerGuildMatchInfo
		BYTE* pPlayerGuildMatchInfo = pPlayerRoomVisibleInfo;
		Write16(pPlayerGuildMatchInfo, 0); //len

		Write32(pPlayerGuildMatchInfo, 0); //SelfPoint
		Write32(pPlayerGuildMatchInfo, 0); //WinNum
		Write32(pPlayerGuildMatchInfo, 0); //TotalNum

		len = pPlayerGuildMatchInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	char IsInTopList = 0;
	IsInTopList = IsUserInTopList(Uin);

	Write8(pPlayerRoomVisibleInfo, IsInTopList); //IsInTopList
	Write8(pPlayerRoomVisibleInfo, 0); //LoverRaceOrPro
	Write8(pPlayerRoomVisibleInfo, 0); //TmpEffectNum
	//m_stPlayerRoomVisibleInfo[].m_aiTmpEffectItems[]


	Write8(pPlayerRoomVisibleInfo, 0); //OBState
	Write8(pPlayerRoomVisibleInfo, 0); //DebutOrX5
	Write8(pPlayerRoomVisibleInfo, 0); //RandKeyFlag


	Write16(pPlayerRoomVisibleInfo, 0x0107); //VipFlag

	Write8(pPlayerRoomVisibleInfo, 0); //HaveAppellation
	/*
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Type
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Level
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Status
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u8Difficulty
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_i32MapId
m_stPlayerRoomVisibleInfo[].m_astCurAppellation[].m_u32Value
	*/

	{ //NobleInfo
		BYTE* pNobleInfo = pPlayerRoomVisibleInfo;
		Write16(pNobleInfo, 0); //len

		Write32(pNobleInfo, Client->NobleID); //NobleID
		Write8(pNobleInfo, Client->NobleLevel); //NobleLevel
		Write32(pNobleInfo, Client->NoblePoint); //NoblePoint
		Write32(pNobleInfo, -1); //NobleLeftDays

		len = pNobleInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}

	Write8(pPlayerRoomVisibleInfo, 0); //HasCarryWizard
	/*
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_ushWizardID
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_szNickname[]:
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_ushWizardType
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_bOrder
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_bEvolutionBranch
m_stPlayerRoomVisibleInfo[].m_astCarryWizardInfo[].m_bIsHuanHua
	*/
#ifndef ZingSpeed
	{ //GuildVipBaseInfo
		BYTE* pGuildVipBaseInfo = pPlayerRoomVisibleInfo;
		Write16(pGuildVipBaseInfo, 0); //len

		Write8(pGuildVipBaseInfo, Client->GuildVipLevel); //GuildVipLevel
		Write32(pGuildVipBaseInfo, Client->GuildVipPoint); //GuildVipPoint

		len = pGuildVipBaseInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
#endif
	Write32(pPlayerRoomVisibleInfo, 0); //ExFlag

	Write8(pPlayerRoomVisibleInfo, 0); //HaveLDMBaseInfo
	/*
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u8Grade
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u32Score
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u32MaxScore
m_stPlayerRoomVisibleInfo[].m_stPlayerLDMBaseInfo[].m_u32MaxGrade
	*/

	Write8(pPlayerRoomVisibleInfo, 0); //HasWl
	/*
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_uiWonderLandID
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_szWonderLandName[]:
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_ushWonderLandDuty
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_ucSubType
m_stPlayerRoomVisibleInfo[].m_astPlayerWlVisibleInfo[].m_bGuildGrade
	*/
#ifndef ZingSpeed
	Write8(pPlayerRoomVisibleInfo, 1); //HasLoverVip
	for (size_t i = 0; i < 1; i++)
	{ //LoverVipInfo
		BYTE* pLoverVipInfo = pPlayerRoomVisibleInfo;
		Write16(pLoverVipInfo, 0); //len

		Write8(pLoverVipInfo, Client->LoverVipLevel); //LoverVipLevel
		Write32(pLoverVipInfo, Client->LoverVipPoint); //LoverVipPoint
		Write8(pLoverVipInfo, 10); //GrowRate

		len = pLoverVipInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
#endif ZingSpeed
	Write8(pPlayerRoomVisibleInfo, 0); //HasBattleModeSkillInfoList
	/*
m_stPlayerRoomVisibleInfo[].m_astBattleModeSkillInfoList[].m_bSkillNum
m_stPlayerRoomVisibleInfo[].m_astBattleModeSkillInfoList[].m_astSkillInfo[].m_shSkillId
m_stPlayerRoomVisibleInfo[].m_astBattleModeSkillInfoList[].m_astSkillInfo[].m_bSkillLevel
	*/
	Write8(pPlayerRoomVisibleInfo, 1); //HasGarden
	for (size_t i = 0; i < 1; i++)
	{ //PersonalGardenBaseInfo
		BYTE* pPersonalGardenBaseInfo = pPlayerRoomVisibleInfo;
		Write16(pPersonalGardenBaseInfo, 0); //len

		Write8(pPersonalGardenBaseInfo, Client->GardenLevel); //GardenLevel
		Write32(pPersonalGardenBaseInfo, Client->GardenPoint); //GardenPoint
		Write8(pPersonalGardenBaseInfo, 10); //GrowRate
		Write32(pPersonalGardenBaseInfo, -1); //GardenExpireTime
		{ //SimpleInfo
			BYTE* pSimpleInfo = pPersonalGardenBaseInfo;
			Write16(pSimpleInfo, 0); //len

			Write32(pSimpleInfo, 0); //WeekPopularity
			Write32(pSimpleInfo, 0); //TotalPopularity
			Write32(pSimpleInfo, 0); //LastUpdatePopularityTime
			Write8(pSimpleInfo, 0); //PrivateType

			len = pSimpleInfo - pPersonalGardenBaseInfo;
			Set16(pPersonalGardenBaseInfo, (WORD)len);
			pPersonalGardenBaseInfo += len;
		}

		len = pPersonalGardenBaseInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	{ //ConsumeVipInfo
		BYTE* pConsumeVipInfo = pPlayerRoomVisibleInfo;
		Write16(pConsumeVipInfo, 0); //len

		Write32(pConsumeVipInfo, 9); //VipLevel
		Write32(pConsumeVipInfo, 0); //CharmValueOfMonth
		Write32(pConsumeVipInfo, 0); //SearchTreasureNums
		Write32(pConsumeVipInfo, 0); //GetTreasureNums

		len = pConsumeVipInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 1); //HaveBaby
	{ //LoverBabyInfo
		BYTE* pLoverBabyInfo = pPlayerRoomVisibleInfo;
		Write16(pLoverBabyInfo, 0); //len

		Write32(pLoverBabyInfo, 71555); // LoveBabyID
		Write32(pLoverBabyInfo, 71555); //ItemID
		Write32(pLoverBabyInfo, 5);		 // GrowValue
		Write32(pLoverBabyInfo, 5);		 // MentalValue
		Write32(pLoverBabyInfo, 5);			 // BabyProfileLevel
		Write32(pLoverBabyInfo, 5);			 // BabyStrengthLevel
		Write32(pLoverBabyInfo, 100);			 // BabyGrowLevel
		memset(pLoverBabyInfo, 0, 17);
		memcpy(pLoverBabyInfo, "宝宝1", 5); //LoveBabyNickName[]
		pLoverBabyInfo += 17;
		Write16(pLoverBabyInfo, 0); // BabyStat
		Write16(pLoverBabyInfo, Client->LoverBaby1EquipStat); // EquipStat
		Write16(pLoverBabyInfo, 5);//LoverBabyTransferdStatus

		len = pLoverBabyInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	{ //EmperorInfo钻皇信息
		BYTE* pEmperorInfo = pPlayerRoomVisibleInfo;
		Write16(pEmperorInfo, 0); //len

		Write8(pEmperorInfo, Client->EmperorLevel); //EmperorLevel
		Write32(pEmperorInfo, Client->EmperorPoint); //EmperorPoint
		Write32(pEmperorInfo, -1); //EmperorLeftDays
		Write8(pEmperorInfo, 10); //EmperorGrowRate

		len = pEmperorInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write16(pPlayerRoomVisibleInfo, 0); //RaceCardNums

	{ //GansterScoreInfo
		BYTE* pGansterScoreInfo = pPlayerRoomVisibleInfo;
		Write16(pGansterScoreInfo, 0); //len

		Write32(pGansterScoreInfo, 0); //GansterSeasonID
		Write32(pGansterScoreInfo, 0); //GansterScore
		Write32(pGansterScoreInfo, 0); //PoliceScore
		Write32(pGansterScoreInfo, 0); //TotalGansterScore

		len = pGansterScoreInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write32(pPlayerRoomVisibleInfo, 0); //OlympicId
	Write32(pPlayerRoomVisibleInfo, 0); //NPCEliminateWinTimes
	Write8(pPlayerRoomVisibleInfo, 0); //OriginTeamID
	Write8(pPlayerRoomVisibleInfo, 0); //NxNpc
	Write16(pPlayerRoomVisibleInfo, 0); //NxNpcAILevel
	{ //EquippedActiveKartInfo
		BYTE* pEquippedActiveKartInfo = pPlayerRoomVisibleInfo;
		Write16(pEquippedActiveKartInfo, 0); //len

		Write8(pEquippedActiveKartInfo, 0); //HaveActiveInfo
		Write32(pEquippedActiveKartInfo, 0); //KartID
		Write32(pEquippedActiveKartInfo, 0); //ActiveLevel

		len = pEquippedActiveKartInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 0); //SpecialActivityStatus
	Write8(pPlayerRoomVisibleInfo, 0); //AreaID
	Write8(pPlayerRoomVisibleInfo, 1); //HaveRaiseSecondBaby
	{ //SecondLoverBabyInfo
		BYTE* pSecondLoverBabyInfo = pPlayerRoomVisibleInfo;
		Write16(pSecondLoverBabyInfo, 0); //len
		Write32(pSecondLoverBabyInfo, 71554); // LoveBabyID
		Write32(pSecondLoverBabyInfo, 71554); //ItemID
		Write32(pSecondLoverBabyInfo, 5);		 // GrowValue
		Write32(pSecondLoverBabyInfo, 5);		 // MentalValue
		Write32(pSecondLoverBabyInfo, 5);			 // BabyProfileLevel
		Write32(pSecondLoverBabyInfo, 5);			 // BabyStrengthLevel
		Write32(pSecondLoverBabyInfo, 100);			 // BabyGrowLevel
		memset(pSecondLoverBabyInfo, 0, 17);
		memcpy(pSecondLoverBabyInfo, "宝宝2", 5); //LoveBabyNickName[]
		pSecondLoverBabyInfo += 17;
		Write16(pSecondLoverBabyInfo, 0); // BabyStat
		Write16(pSecondLoverBabyInfo, Client->LoverBaby2EquipStat); // EquipStat
		Write16(pSecondLoverBabyInfo, 5);//LoverBabyTransferdStatus
		len = pSecondLoverBabyInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}

	Write8(pPlayerRoomVisibleInfo, 1); //HasRankedMatchInfo
	for (size_t i = 0; i < 1; i++)
	{ //RankedMatchGradeInfo
		BYTE* pRankedMatchGradeInfo = pPlayerRoomVisibleInfo;
		Write16(pRankedMatchGradeInfo, 0); //len

		Write32(pRankedMatchGradeInfo, RankGradeLevel[Client->GradeLevel]); //GradeLevel
		Write32(pRankedMatchGradeInfo, Client->ChildLevel); //ChildLevel
		Write32(pRankedMatchGradeInfo, 0); //GradeScore
		Write32(pRankedMatchGradeInfo, 0); //MaxGradeScore
		Write32(pRankedMatchGradeInfo, 0); //TotalScore

		len = pRankedMatchGradeInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 0); //HasHuanLingChangeInfo

	Write8(pPlayerRoomVisibleInfo, 0); //EquipSealType
	Write16(pPlayerRoomVisibleInfo, 0); //Role
	Write8(pPlayerRoomVisibleInfo, 0); //SpecialActivityIDNum
	Write8(pPlayerRoomVisibleInfo, 0); //CrewID
	Write8(pPlayerRoomVisibleInfo, 1); //SpecialActivityInfoNum 职业生涯图标段位
	for (size_t i = 0; i < 1; i++)
	{ //SpecialActivityInfo
		BYTE* pSpecialActivityInfo = pPlayerRoomVisibleInfo;
		Write16(pSpecialActivityInfo, 0); //len

		Write8(pSpecialActivityInfo, 1); //ProfessionLicenseInfoNum
		for (size_t i = 0; i < 1; i++)
		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pSpecialActivityInfo;
			Write16(pProfessionLicenseInfo, 0); //len

			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel

			len = pProfessionLicenseInfo - pSpecialActivityInfo;
			Set16(pSpecialActivityInfo, (WORD)len);
			pSpecialActivityInfo += len;
		}

		len = pSpecialActivityInfo - pPlayerRoomVisibleInfo;
		Set16(pPlayerRoomVisibleInfo, (WORD)len);
		pPlayerRoomVisibleInfo += len;
	}
	Write8(pPlayerRoomVisibleInfo, 0); //ParaNum

	Write8(pPlayerRoomVisibleInfo, 0); //ItemNum

	Write8(pPlayerRoomVisibleInfo, 0); //TDCExcluItemNum

	Write8(pPlayerRoomVisibleInfo, 0); //EquipNiChangItemNum

	Write8(pPlayerRoomVisibleInfo, 0); //TestKartGameType
	Write32(pPlayerRoomVisibleInfo, 0); //GameTestKartID
	Write8(pPlayerRoomVisibleInfo, 0); //HasStarsFightingInfo

	Write8(pPlayerRoomVisibleInfo, 0); //RoomBGSetting


	len = pPlayerRoomVisibleInfo - p;
	Set16(p, len);
	p += len;
}



void RequestRegister2(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	int result;
	char* newname;

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	if (Client->Uin != Uin || Uin <= 10000) {
		return;
	}

	const char* Nickname = (char*)Body;
	newname = G2U(Nickname);

	Body += MaxNickName;

	BYTE Gender = Read8(Body);
	BYTE Country = Read8(Body);
	BOOL isNameExist = 0;


	std::vector<std::string> conditionValues;
	conditionValues.push_back(newname);
	std::vector<int> resultRow = executeQueryRow("SELECT 1 FROM BaseInfo Where NickName=?;", "RequestRegister2", conditionValues);
	if (resultRow.size() >= 1) {
		ResponseStartGame(Client, "用户名已存在!");
		if (newname) {
			delete[] newname;//释放内存 防止重复名过多内存泄露
		}
		return;
	}

	PlayerDB_InsertPlayer(Client->Uin);
	PlayerDB_InsertBaseInfo(Client->Uin, newname, Gender, Country);

	UINT RoleID = Read32(Body);
	UINT HairID = Read32(Body);
	UINT FaceID = Read32(Body);
	UINT CoatID = Read32(Body);
	UINT GloveID = Read32(Body);
	UINT TrousersID = Read32(Body);
	UINT KartID = Read32(Body);
	UINT ColorID = Read32(Body);

	PlayerDB_AddItem2(Client, Client->Uin, RoleID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, HairID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, FaceID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, CoatID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, GloveID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, TrousersID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, KartID, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, ColorID, 1, -1, true);
	//3D 男装
	PlayerDB_AddItem2(Client, Client->Uin, 27178, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27179, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27180, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27181, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27182, 1, -1, true);
	//3D 女装
	PlayerDB_AddItem2(Client, Client->Uin, 27190, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27191, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27192, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27193, 1, -1, true);
	PlayerDB_AddItem2(Client, Client->Uin, 27194, 1, -1, true);
	//轮滑滑板
	PlayerDB_AddItem2(Client, Client->Uin, 31941, 1, -1, false);
	PlayerDB_AddItem2(Client, Client->Uin, 31940, 1, -1, false);
	PlayerDB_AddItem2(Client, Client->Uin, 19294, 1, -1, false);

	for (size_t i = 0; i < RegisterBoxs.size(); i++)
	{
		PlayerDB_AddItem2(Client, Client->Uin, RegisterBoxs[i]->ItemID, RegisterBoxs[i]->ItemNum, RegisterBoxs[i]->AvailPeriod, false);
	}

	std::vector<ItemInfo> Items(200);
	int ItemNum = 0;
	bool HasMoreInfo = false;

	std::vector<ItemInfo> vecItems;
	PlayerDB_SelectItems(Client->Uin, vecItems);
	for (size_t i = 0; i < vecItems.size(); i++)
	{
		Items[ItemNum].ItemID = vecItems[i].ItemID;
		Items[ItemNum].ItemNum = vecItems[i].ItemNum;
		Items[ItemNum].AvailPeriod = vecItems[i].AvailPeriod;
		Items[ItemNum].Status = vecItems[i].Status;
		Items[ItemNum].ObtainTime = vecItems[i].ObtainTime;
		Items[ItemNum].OtherAttribute = vecItems[i].OtherAttribute;
		Items[ItemNum].ItemType = 0;
		if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_CAR)
		{
			Client->KartID = Items[ItemNum].ItemID;
			//printf("KartID:%d\n", Client->KartID);
		}
		else if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_PET)
		{
			Client->Pet->ID = Items[ItemNum].ItemID;
			GetPetInfo(Client);
		}
		ItemNum++;
		if (ItemNum >= 200)
		{
			HasMoreInfo = true;
			break;
		}
	}
	ResponseRegister(Client, Nickname, Gender, Country, ItemNum, Items);

	if (HasMoreInfo) {
		ItemNum = 0;
		HasMoreInfo = false;
		for (size_t i = 201; i < vecItems.size(); i++)
		{
			Items[ItemNum].ItemID = vecItems[i].ItemID;
			Items[ItemNum].ItemNum = vecItems[i].ItemNum;
			Items[ItemNum].AvailPeriod = vecItems[i].AvailPeriod;
			Items[ItemNum].Status = vecItems[i].Status;
			Items[ItemNum].ObtainTime = vecItems[i].ObtainTime;
			Items[ItemNum].OtherAttribute = vecItems[i].OtherAttribute;
			Items[ItemNum].ItemType = 0;
			if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_CAR)
			{
				Client->KartID = Items[ItemNum].ItemID;
				//printf("KartID:%d\n", Client->KartID);
			}
			else if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_PET)
			{
				Client->Pet->ID = Items[ItemNum].ItemID;
				GetPetInfo(Client);
			}
			ItemNum++;
			if (ItemNum >= 200)
			{
				HasMoreInfo = true;
				break;
			}
		}
		NotifyClientAddItem(Client, ItemNum, Items);
	}

	ResponseSkipFreshTask(Client); //跳过新手任务
}
void ResponseRegister(std::shared_ptr<ClientNode> Client, const char* Nickname, BYTE  Gender, BYTE Country, UINT ItemNum, std::vector<ItemInfo> Items)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin);

	if (strlen(Nickname) < MaxNickName)
	{
		strcpy_s((char*)p, MaxNickName, Nickname);
		p += MaxNickName; // 更新指针位置到复制后的末尾
	}
	else
	{
		return;
	}


	Write8(p, Gender);
	Write8(p, Country);

	Write16(p, ItemNum); //AddItemNum
	{
		for (size_t i = 0; i < ItemNum; i++)
		{
			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, Items[i].ItemID);
			Write32(pItemInfo, Items[i].ItemNum);
			Write32(pItemInfo, Items[i].AvailPeriod);
			Write8(pItemInfo, Items[i].Status);
			Write32(pItemInfo, Items[i].ObtainTime);
			Write32(pItemInfo, Items[i].OtherAttribute);
#ifndef ZingSpeed
			Write16(pItemInfo, Items[i].ItemType); //ItemType
#endif

			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}

	Write16(p, 0); //AddTaskNum

	Write8(p, 0); //ReasonLen


	len = p - buf.data();
	SendToClient(Client, 129, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void InsertItem(std::shared_ptr<ClientNode> Client)//通知客户端加全部数据
{

	const char* sql;
	int result;
	bool hasMore = true;
	std::vector<ItemInfo> Items(200);//一次发送200条数据 老方法会卡400上限 通过responselogin后调用
	size_t ItemNum = 0;


	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,AvailPeriod,Status,ObtainTime,OtherAttribute  FROM Item WHERE Uin = ? ;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (hasMore)
		{
			ItemNum = 0;
			hasMore = false;

			// 处理结果
			while (res->next()) {
				Items[ItemNum].ItemID = res->getUInt("ItemID");
				Items[ItemNum].ItemNum = res->getUInt("ItemNum");
				Items[ItemNum].AvailPeriod = res->getInt("AvailPeriod");
				Items[ItemNum].Status = res->getUInt("Status");
				Items[ItemNum].ObtainTime = res->getUInt("ObtainTime");
				Items[ItemNum].OtherAttribute = res->getUInt("OtherAttribute");
				Items[ItemNum].ItemType = 0;
				if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_CAR)
				{
					Client->KartID = Items[ItemNum].ItemID;
					//printf("KartID:%d\n", Client->KartID);
				}
				else if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_PET)
				{
					Client->Pet->ID = Items[ItemNum].ItemID;
					GetPetInfo(Client);
				}

				ItemNum++;
				if (ItemNum >= 200)
				{
					hasMore = true;
					break;
				}
			}

			NotifyClientAddItem(Client, ItemNum, Items);
			ItemStatus NewStatus;
			for (size_t i = 0; i < ItemNum; i++)
			{
				if (Items[i].Status)
				{
					NewStatus.ItemID = Items[i].ItemID;
					NewStatus.NewStatus = true;
					ResponseChangeItemStatus(Client, 1, { NewStatus });
				}
			}

		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("InsertItem error\n");
	}

}

void NotifyLoginMoreInfoNew_1(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);
	BYTE* p = buf.data();
	size_t len;
	USHORT ItemCount = 0;
	bool HasNext = false;

	BYTE* pItemNum = p;
	Write16(p, 0); //ItemNum

	Set16(pItemNum, ItemCount);

	Write16(p, 0); //TaskNum
	Write16(p, 0); //PetNum
	Write16(p, 0); //MapNum
	Write16(p, 0); //OverdueItemNum
	Write16(p, 0); //StoneKartNum

	Write16(p, HasNext);

	len = p - buf.data();
	SendToClient(Client, 223, buf.data(), len, Client->ServerID, FE_PLAYER, Client->ServerID, Notify);
}


void NotifyLoginMoreInfoNew_1(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo>& vecItems, bool HasMoreItemInfo)
{
	std::vector<BYTE> buf(819200);
	BYTE* p = buf.data();
	size_t len;
	USHORT ItemCount = 0;
	bool HasNext = false;

	BYTE* pItemNum = p;
	Write16(p, 0); //ItemNum

	if (!vecItems.empty()) {
		ItemCount = 0;

		for (size_t i = 0; i < vecItems.size(); ++i) {
			const ItemInfo& item = vecItems[i];

			if (item.Status && GetItemType(item.ItemID) == EAIT_CAR) {
				Client->KartID = item.ItemID;
				//printf("KartID:%d\n", Client->KartID);
			}
			else if (item.Status && GetItemType(item.ItemID) == EAIT_PET) {
				Client->Pet->ID = item.ItemID;
				GetPetInfo(Client);
			}

			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, item.ItemID);
			Write32(pItemInfo, item.ItemNum);
			Write32(pItemInfo, item.AvailPeriod);
			Write8(pItemInfo, item.Status);
			Write32(pItemInfo, item.ObtainTime);
			Write32(pItemInfo, item.OtherAttribute);
			Write16(pItemInfo, item.ItemType);

			len = pItemInfo - p;
			Set16(p, static_cast<WORD>(len));
			p += len;

			ItemCount++;
		}
	}

	Set16(pItemNum, ItemCount);

	Write16(p, 0); //TaskNum
	Write16(p, 0); //PetNum
	Write16(p, 0); //MapNum
	Write16(p, 0); //OverdueItemNum
	Write16(p, 0); //StoneKartNum

	Write16(p, HasMoreItemInfo);

	len = p - buf.data();
	SendToClient(Client, 223, buf.data(), len, Client->ServerID, FE_PLAYER, Client->ServerID, Notify);
}


void NotifyLoginMoreInfo(std::shared_ptr<ClientNode> Client, std::vector<ItemInfo>& vecItems) {
	bool HasMoreItems = false; // 初始设为 false

	if (vecItems.size() > 200) {
		HasMoreItems = true;
	}

	std::vector<ItemInfo> remainingItems;
	if (vecItems.empty()) {
		NotifyLoginMoreInfoNew_1(Client);
	}

	// Process items in batches of up to 200
	while (!vecItems.empty()) {
		std::vector<ItemInfo> itemsToSend;
		size_t itemsToSendCount = min(vecItems.size(), static_cast<size_t>(200));
		if (itemsToSendCount < 200 || vecItems.size() == 200) {
			HasMoreItems = false;
		}

		for (size_t i = 0; i < itemsToSendCount; ++i) {
			itemsToSend.push_back(vecItems[i]);
		}

		// Call NotifyLoginMoreInfoNew_1 to send items to client
		NotifyLoginMoreInfoNew_1(Client, itemsToSend, HasMoreItems);

		// Check if there are remaining items
		if (vecItems.size() > itemsToSendCount) {
			remainingItems.clear();
			for (size_t i = itemsToSendCount; i < vecItems.size(); ++i) {
				remainingItems.push_back(vecItems[i]);
			}
			vecItems = remainingItems;
		}
		else {
			vecItems.clear();
		}
	}
}

void NotifyLoginMoreInfoNew_1(std::shared_ptr<ClientNode> Client, std::unique_ptr<sql::ResultSet>& res, bool* HasMoreItemInfo)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();  size_t len;
	USHORT ItemCount = 0;
	bool HasNext = false;

	BYTE* pItemNum = p;
	Write16(p, 0); //ItemNum
	{
		if (res && *HasMoreItemInfo) {
			ItemCount = 0;
			*HasMoreItemInfo = false;
			while (res->next())
			{
				if (res->getUInt("Status") && GetItemType(res->getUInt("ItemID")) == EAIT_CAR)
				{
					Client->KartID = res->getUInt("ItemID");
					//printf("KartID:%d\n", Client->KartID);
				}
				else if (res->getUInt("Status") && GetItemType(res->getUInt("ItemID")) == EAIT_PET)
				{
					Client->Pet->ID = res->getUInt("ItemID");
					GetPetInfo(Client);
				}

				BYTE* pItemInfo = p;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, res->getUInt("ItemID"));//ItemID
				Write32(pItemInfo, res->getUInt("ItemNum"));//ItemNum
				Write32(pItemInfo, res->getInt("AvailPeriod"));//AvailPeriod
				Write8(pItemInfo, res->getUInt("Status"));//Status
				Write32(pItemInfo, res->getUInt("ObtainTime"));//ObtainTime
				Write32(pItemInfo, res->getUInt("OtherAttribute"));//OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - p;
				Set16(p, (WORD)len);
				p += len;

				ItemCount++;
				if (ItemCount >= 200) {
					HasNext = true;
					*HasMoreItemInfo = true;
					break;
				}
			}
		}
	}
	Set16(pItemNum, ItemCount);
	//printf("ResponseLoginNEW ItemCount: %d HasNext: %d\n", ItemCount, HasNext);


	Write16(p, 0); //TaskNum
	/*
m_astPlayerDBTaskInfo[].m_iTaskID
m_astPlayerDBTaskInfo[].m_shOptionID
m_astPlayerDBTaskInfo[].m_cStatus
m_astPlayerDBTaskInfo[].m_bPrevStatus
m_astPlayerDBTaskInfo[].m_iProgressRate
m_astPlayerDBTaskInfo[].m_iOther
m_astPlayerDBTaskInfo[].m_uiTakeTime
m_astPlayerDBTaskInfo[].m_uiLastUpdateTime
	*/

	Write16(p, 0); //PetNum


	Write16(p, 0); //MapNum
	/*
m_astRecord[].m_iMapID
m_astRecord[].m_iRecord
m_astRecord[].m_iLastUpdateTime
	*/


	Write16(p, 0); //OverdueItemNum
	/*
m_astOverdueItem[].m_iItemID
m_astOverdueItem[].m_iOverdueTime
m_astOverdueItem[].m_iOtherAttr
	*/

	Write16(p, 0); //StoneKartNum
	/*
m_astKartStoneGrooveInfo[].m_iKartID
m_astKartStoneGrooveInfo[].m_iStoneGrooveNum
m_astKartStoneGrooveInfo[].m_astStoneGrooveInfo[].m_iStoneUseOccaType
m_astKartStoneGrooveInfo[].m_astStoneGrooveInfo[].m_iSkillStoneID
	*/

	//Write8(p, HasNext);
	Write16(p, HasNext);


	len = p - buf.data();
	SendToClient(Client, 223, buf.data(), len, Client->ServerID, FE_PLAYER, Client->ServerID, Notify);
}

void initializeLibrary(std::vector<UINT>& library, const std::string& input) {
	library.assign(11, 0); // 初始化为0，大小为11
	std::stringstream ss(input);
	std::string token;
	size_t index = 0;

	while (std::getline(ss, token, '^') && index < library.size()) {
		library[index] = static_cast<UINT>(std::stoi(token)); // 转换并赋值
		++index;
	}
}


void initiOtherInfo(std::shared_ptr<ClientNode> Client)
{
	std::string NickName = "";

	int result;

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		const char* sql = NULL;
		sql = "SELECT WWeight,SpeedWeight,JetWeight,SJetWeight,AccuWeight,AntiCollisionWeight,KartHeadRefitItemID,KartTailRefitItemID,KartFlankRefitItemID,KartTireRefitItemID,ShapeSuitID,Speed2Weight,DriftVecWeight,AdditionalZSpeedWeight FROM KartRefit Where Uin=? and KartID=?;";

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->setUInt(2, Client->KartID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			Client->ClientKartRefitItemInfo.WWeight = res->getUInt("WWeight");
			Client->ClientKartRefitItemInfo.SpeedWeight = res->getUInt("SpeedWeight");
			Client->ClientKartRefitItemInfo.JetWeight = res->getUInt("JetWeight");
			Client->ClientKartRefitItemInfo.SJetWeight = res->getUInt("SJetWeight");
			Client->ClientKartRefitItemInfo.AccuWeight = res->getUInt("AccuWeight");
			Client->ClientKartRefitItemInfo.AntiCollisionWeight = res->getUInt("AntiCollisionWeight");
			Client->ClientKartRefitItemInfo.KartHeadRefitItemID = res->getUInt("KartHeadRefitItemID");
			Client->ClientKartRefitItemInfo.KartTailRefitItemID = res->getUInt("KartTailRefitItemID");
			Client->ClientKartRefitItemInfo.KartFlankRefitItemID = res->getUInt("KartFlankRefitItemID");
			Client->ClientKartRefitItemInfo.KartTireRefitItemID = res->getUInt("KartTireRefitItemID");
			UINT ShapeSuitID = res->getUInt("ShapeSuitID");

			Client->KartSkinID = Client->ClientKartRefitItemInfo.ShapeSuitID = ShapeSuitID;
			Client->ClientKartRefitItemInfo.Speed2Weight = res->getUInt("Speed2Weight");
			Client->ClientKartRefitItemInfo.DriftVecWeight = res->getUInt("DriftVecWeight");
			Client->ClientKartRefitItemInfo.AdditionalZSpeedWeight = res->getUInt("AdditionalZSpeedWeight");
			break;
		}
		res.reset();

		{
			sql = "SELECT P.PetID,P.Status FROM Petitem P join Item I on P.PetID=I.ItemID where I.Status=1 and I.Uin=?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				Client->PetID = res->getUInt("PetID");
				Client->PetStatus = res->getUInt("Status");
				break;
			}
			res.reset();
		}

		{
			sql = "SELECT 1 FROM Item Where ItemId=120258 and Status=1 and Uin=?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				Client->GlobalAddcard = 1;
				break;
			}
			res.reset();
		}


		{
			SelectGlobleStone(Client);
			//sql = "SELECT 1 FROM Item Where ItemId=18554 and Status=1 and Uin=?;";
			//std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			//prep_stmt->clearParameters();
			//prep_stmt->setUInt(1, Client->Uin);
			//// 执行查询
			//std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			//prep_stmt.reset();
			//while (res->next()) {
			//	Client->GlobalStone = 1;
			//	break;
			//}
			//res.reset();
		}


		{
			sql = "SELECT 1 FROM Item Where ItemId=19756 and Status=1 and Uin=?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				Client->isManMachine = 1;
				break;
			}
			res.reset();
		}

		{
			sql = "SELECT NickName,Experience,Money,SuperMoney,TotalHonor,Charm,BugletTalk,DanceExp,nitrogen,admin,fps,SecPwd,SecPwdStatus,gm,SkipFlag,UserHeads,UserHeadsBox,UserFloor,SpeclEffect,AppellationType,gmtime,QQ,Speaking,KartLibaray,GameBan FROM BaseInfo  WHERE Uin=?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				NickName = UTF2GB(res->getString("NickName").c_str());
				
				if (!NickName.empty())
				{
					memset(Client->NickName, 0, 17);
					memcpy(Client->NickName, NickName.c_str(), NickName.length());
				}
				Client->Experience = res->getUInt("Experience");
				Client->Money = res->getUInt("Money");
				Client->SuperMoney = res->getUInt("SuperMoney");
				Client->TotalHonor = res->getUInt("TotalHonor");
				Client->Charm = res->getUInt("Charm");
				Client->BugletTalk = res->getUInt("BugletTalk");
				Client->DanceExp = res->getUInt("DanceExp");
				Client->nitrogen = res->getUInt("nitrogen");
				Client->admin = res->getUInt("admin");
				Client->gm = res->getUInt("gm");
				Client->skipFlag = res->getUInt("SkipFlag");
				Client->fps = res->getUInt("fps");
				//Client->QQHead = res->getUInt("qqhead");
				char SecPwd[MaxSecPwd] = "";
				memcpy(SecPwd, res->getString("SecPwd").c_str(), MaxSecPwd);
				sprintf_s(Client->SecPwd, SecPwd);
				Client->SecPwdStats = res->getUInt("SecPwdStatus");
				//Client->Verify = res->getUInt("Verify");
				//Client->FrozenLeftSeconds = res->getUInt("FrozenLeftSeconds");
				//Client->FrozenSeconds = res->getUInt("FrozenSeconds");

				Client->UserHeads = res->getUInt("UserHeads");
				Client->UserHeadsBox = res->getUInt("UserHeadsBox");
				Client->UserFloor = res->getUInt("UserFloor");
				//Client->QQHeadUrl = std::to_string(res->getUInt("QQ"));
				//Client->QQHeadUrl = std::to_string(res->getUInt("QQ"));
				memcpy(Client->QQHeadUrl, res->getString("QQ").c_str(), MaxQQHeadUrl);
				// 替换 QQ 号
				std::string url = qqurl;
				size_t pos = url.find("QQ号");
				if (pos != std::string::npos) {
					url.replace(pos, 3, Client->QQHeadUrl); // 3 是 "QQ号" 的长度
				}
				memcpy(Client->QQHeadUrl, url.c_str(), MaxQQHeadUrl);
				//Client->QQHeadUrl = url;


				Client->SpeclEffect = res->getUInt("SpeclEffect");
				Client->AppellationType = res->getInt("AppellationType");
				Client->gmtime = res->getString("gmtime");
				Client->Speaking = res->getUInt("Speaking");
				std::string KartLibaray = res->getString("KartLibaray");
				initializeLibrary(Client->MyKartLibrary, KartLibaray); // 调用外部方法初始化
				Client->GameBan = res->getInt("GameBan");
				break;
			}
			res.reset();
		}

		{
			sql = "SELECT GI.GuildID,GI.Duty,GL.Name FROM GuildInfo GI join GuildList GL on GI.GuildID=GL.ID where GI.Uin=?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				Client->GuildID = res->getUInt("GuildID");
				Client->GuildDuty = res->getUInt("Duty");
				NickName = UTF2GB(res->getString("Name").c_str());
				if (!NickName.empty())
				{
					memset(Client->GuildName, 0, 17);
					memcpy(Client->GuildName, NickName.c_str(), NickName.length());
				}
				break;
			}
			res.reset();
		}

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("initiOtherInfo error\n");
		//throw;关闭
	}

}
void NotifyS2CMyKartAltasMoreInfo(std::shared_ptr<ClientNode> Client, UINT Index, std::vector<UINT> HasKartAltasList)
{

	std::vector<BYTE> buf(819200);
	BYTE* p = buf.data();
	size_t len;
	Write32(p, Client->Uin); //Uin
	BYTE* pItemNum = p;
	int ItemNum = 0;
	bool HasMoreInfo = false;
	Write16(p, 0); //ItemNum
	for (size_t i = 0; i < HasKartAltasList.size(); i++)
	{
		if (i + Index >= HasKartAltasList.size()) {
			break;
		}
		//ItemInfoArray
		if (i < 120) {
			ItemNum++;
			BYTE* pItemInfoArray = p;
			Write16(pItemInfoArray, 0); //len

			Write32(pItemInfoArray, HasKartAltasList[i + Index]); //ItemID
			Write32(pItemInfoArray, 0); //RefitCout
			{ //AltasCurKartAttr
				BYTE* pAltasCurKartAttr = pItemInfoArray;
				Write16(pAltasCurKartAttr, 0); //len

				Write32(pAltasCurKartAttr, Client->Uin); //Uin
				Write32(pAltasCurKartAttr, HasKartAltasList[i + Index]); //KartId
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
				Write32(pAltasCurKartAttr, 0); //SecondRefitCount
				Write16(pAltasCurKartAttr, 0); //Speed2Weight
				Write16(pAltasCurKartAttr, 0); //DriftVecWeight
				Write16(pAltasCurKartAttr, 0); //AdditionalZSpeedWeight
				Write16(pAltasCurKartAttr, 0); //AntiCollisionWeight
				Write16(pAltasCurKartAttr, 0); //LuckyValue
				Write16(pAltasCurKartAttr, 0); //RefitLuckyValueMaxWeight
				Write32(pAltasCurKartAttr, 0); //ShapeSuitID
				Write8(pAltasCurKartAttr, 0); //LegendSuitLevel
				Write32(pAltasCurKartAttr, 0); //LegendSuitLevelChoice
				Write32(pAltasCurKartAttr, 0); //ShapeLegendSuitID

				len = pAltasCurKartAttr - pItemInfoArray;
				Set16(pItemInfoArray, (WORD)len);
				pItemInfoArray += len;
			}
			{ //AltasKartStoneGrooveInfo
				BYTE* pAltasKartStoneGrooveInfo = pItemInfoArray;
				Write16(pAltasKartStoneGrooveInfo, 0); //len

				Write32(pAltasKartStoneGrooveInfo, HasKartAltasList[i + Index]); //KartID
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
	SendToClient(Client, 13704, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	if (HasMoreInfo)
	{
		NotifyS2CMyKartAltasMoreInfo(Client, Index, HasKartAltasList);
	}
}
void NotifyS2CMyKartAltas(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::vector<BYTE> buf(819200);
		BYTE* p = buf.data();
		size_t len;
		std::vector<UINT> HasKartAltasList;//拥有车列表

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemID FROM Item a,ItemallNew b WHERE a.itemid = b.id and b.type = 'EAIT_CAR'and a.Uin = ?"));

		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			int KartID = res->getUInt("ItemID");
			//if (KartID == 129295 || KartID == 129296 || KartID == 130784 || KartID == 130785) {
			//	continue;//新版b
			//}
			//if (KartID == 130774 || KartID == 130779 || KartID == 130780 || KartID == 130782 || KartID == 130783) {
			//	continue;//新版a
			//}
			//if (KartID == 130776) {
			//	continue;//新版s
			//}
			//if (KartID == 130775) {
			//	continue;//新版m
			//}
			//if (KartID == 130538 || KartID == 129285) {
			//	continue;//新版t
			//}
			if (GetItemType(KartID) == EAIT_CAR) {
				HasKartAltasList.insert(HasKartAltasList.end(), KartID);
			}
		}
		res.reset();

		Write32(p, Client->Uin); //Uin
		BYTE* pItemNum = p;
		int ItemNum = 0;
		bool HasMoreInfo = false;

		Write16(p, 0); //ItemNum
		for (size_t i = 0; i < HasKartAltasList.size(); i++)
		{ //ItemInfoArray
			if (i < 120) {
				ItemNum++;
				BYTE* pItemInfoArray = p;
				Write16(pItemInfoArray, 0); //len

				Write32(pItemInfoArray, HasKartAltasList[i]); //ItemID
				Write32(pItemInfoArray, 0); //RefitCout
				{ //AltasCurKartAttr
					BYTE* pAltasCurKartAttr = pItemInfoArray;
					Write16(pAltasCurKartAttr, 0); //len

					Write32(pAltasCurKartAttr, Client->Uin); //Uin
					Write32(pAltasCurKartAttr, HasKartAltasList[i]); //KartId
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
					Write32(pAltasCurKartAttr, 0); //SecondRefitCount
					Write16(pAltasCurKartAttr, 0); //Speed2Weight
					Write16(pAltasCurKartAttr, 0); //DriftVecWeight
					Write16(pAltasCurKartAttr, 0); //AdditionalZSpeedWeight
					Write16(pAltasCurKartAttr, 0); //AntiCollisionWeight
					Write16(pAltasCurKartAttr, 0); //LuckyValue
					Write16(pAltasCurKartAttr, 0); //RefitLuckyValueMaxWeight
					Write32(pAltasCurKartAttr, 0); //ShapeSuitID
					Write8(pAltasCurKartAttr, 0); //LegendSuitLevel
					Write32(pAltasCurKartAttr, 0); //LegendSuitLevelChoice
					Write32(pAltasCurKartAttr, 0); //ShapeLegendSuitID

					len = pAltasCurKartAttr - pItemInfoArray;
					Set16(pItemInfoArray, (WORD)len);
					pItemInfoArray += len;
				}
				{ //AltasKartStoneGrooveInfo
					BYTE* pAltasKartStoneGrooveInfo = pItemInfoArray;
					Write16(pAltasKartStoneGrooveInfo, 0); //len

					Write32(pAltasKartStoneGrooveInfo, HasKartAltasList[i]); //KartID
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
		Write8(p, HasMoreInfo); //HasMoreInfo

		len = p - buf.data();
		SendToClient(Client, 13706, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		if (HasMoreInfo)
		{
			NotifyS2CMyKartAltasMoreInfo(Client, ItemNum, HasKartAltasList);
		}
		// 释放堆空间
		//delete[] buf;
	}
	catch (std::exception& e)
	{
		printf("NotifyS2CMyKartAltas Error: %s\n", e.what());
		//throw;关闭
	}
}

void NotifyTaskProgressRate(std::shared_ptr<ClientNode> Client, UINT TaskID, UINT ProgressRate, UINT Status)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	{ //TaskStatusInfo
		BYTE* pTaskStatusInfo = p;
		Write16(pTaskStatusInfo, 0); //len

		Write32(pTaskStatusInfo, TaskID); //TaskID
		Write8(pTaskStatusInfo, Status); //Status
		Write32(pTaskStatusInfo, ProgressRate); //ProgressRate
		Write32(pTaskStatusInfo, 0); //Other
		Write32(pTaskStatusInfo, 0); //LastUpdateTime

		len = pTaskStatusInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //RandTaskUpdateFlag

	len = p - buf.data();
	SendToClient(Client, 536, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}




void NotifyTaskAward(std::shared_ptr<ClientNode> Client, UINT TaskID)
{
	
	
	int result;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	std::vector<ItemInfo> Items;
	size_t allitem;
	UINT SuperMoney, Money, Experience;
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\OnlineAward.yml");
		SuperMoney = Config["SuperMoney"].as<UINT>();
		Money = Config["Money"].as<UINT>();
		Experience = Config["Experience"].as<UINT>();

		YAML::Node ItemList = Config["Items"];
		allitem = ItemList.size();
		Items.resize(allitem);
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = ItemList[j];//0开头
			Items[j].ItemID = ItemCfg["ItemId"].as<int>();
			Items[j].ItemNum = ItemCfg["ItemNum"].as<int>();
			Items[j].ObtainTime = ItemCfg["ObtainTime"].as<int>();
			Items[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			ItemCfg.reset();
		}
	}
	catch (const std::exception& e)
	{
		printf("在线半小时奖励配置异常!\n");
		return;
	}

	SelectClientExp(Client);

	Client->SuperMoney += SuperMoney;//赠送点券
	Client->Money += Money;//赠送酷比
	//Client->Experience += Experience;//赠送经验
	Client->Experience = (Client->Experience > UINT_MAX - Experience) ? UINT_MAX : Client->Experience + Experience;
	Write32(p, Client->Uin); // Uin
	Write32(p, TaskID);		 // TaskID
	//148-150在线奖励任务

	//NotifyAddTaskInfo不添加148-150任务 可以触发奖励弹窗
	//NotifyAddTaskInfo添加后不触发奖励弹窗可以在任务进度里面查看奖励物品
	{						 // TaskAward
		BYTE* pTaskAward = p;
		Write16(pTaskAward, 0); // len

		Write32(pTaskAward, Money); // AwardMoney
		Write32(pTaskAward, Client->Money); // TotalMoney
		Write32(pTaskAward, SuperMoney); // AwardSuperMoney
		Write32(pTaskAward, Client->SuperMoney); // TotalSuperMoney
		Write32(pTaskAward, Experience); // AwardExp
		Write32(pTaskAward, Client->Experience); // TotalExp
		Write32(pTaskAward, allitem);	   // AwardItemNum


		UpdateClientBase(Client);

		for (size_t i = 0; i < allitem; i++)
		{ // AwardItemInfo
			BYTE* pAwardItemInfo = pTaskAward;
			Write16(pAwardItemInfo, 0); // len

			Write32(pAwardItemInfo, Items[i].ItemID); // ItemID
			Write32(pAwardItemInfo, Items[i].ItemNum);		// ItemNum
			Write32(pAwardItemInfo, Items[i].AvailPeriod);	// AvailPeriod
			Write8(pAwardItemInfo, 0);		// Status
			Write32(pAwardItemInfo, Items[i].ObtainTime);		// ObtainTime
			Write32(pAwardItemInfo, 0);		// OtherAttribute
			Write16(pAwardItemInfo, 0);		// ItemType

			len = pAwardItemInfo - pTaskAward;
			Set16(pTaskAward, (WORD)len);
			pTaskAward += len;
			PlayerDB_AddItem2(Client, Client->Uin, Items[i].ItemID, Items[i].ItemNum, Items[i].AvailPeriod, 0);

		}

		Write32(pTaskAward, 0); // AwardCoupons
		Write32(pTaskAward, 0); // TotalCoupons
		Write32(pTaskAward, 0); // AwardLuckMoney
		Write32(pTaskAward, 0); // TotalLuckMoney
		Write32(pTaskAward, 0); // AwardTeamWorkExp
		Write32(pTaskAward, 0); // TotalTeamWorkExp
		Write32(pTaskAward, 0); // AwardWlCash
		Write32(pTaskAward, 0); // AwardWlCrystal
		Write32(pTaskAward, 0); // AwardWlDegree
		Write32(pTaskAward, 0); // ChumCircleActivity
		Write32(pTaskAward, 0); // GuildScore
		Write16(pTaskAward, 0); // AppellcationID
		Write16(pTaskAward, 0); // ScoreJingJi
		Write8(pTaskAward, 0);	// ExtendInfoNum
		Write32(pTaskAward, 0); // LiveExp
		Write32(pTaskAward, 0); // AwardSpeedCoin
		Write32(pTaskAward, 0); // TotalSpeedCoin

		len = pTaskAward - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0);				// NextTaskID
	Write16(p, 0);				// Type
	Write32(p, 0); // TaskWindowType
	Write8(p, 0);				// TaskTipsLen
	Write8(p, 0);				// ParaNum
	len = p - buf.data();
	SendToClient(Client, 537, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void OnlineTest()
{
	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	std::map<long, std::shared_ptr<ClientNode>>::iterator iter;
	for (iter = Clients.begin(); iter != Clients.end(); iter++)
	{
		std::shared_ptr<ClientNode> Client = iter->second;
		if (Client && Client->Uin > 10000)
		{
			Client->OnlineTime += 60;//每次加10秒
			Client->TotalOnlineTime += 60;//每次加10秒
			if (Client->OnlineTime >= 900)//如果在线半个小时
			{
				//发送进度
				NotifyTaskProgressRate(Client, 148, Client->OnlineTime / 60, 1);
				NotifyTaskProgressRate(Client, 149, Client->TotalOnlineTime / 60, 0);
				NotifyTaskAward(Client, 148);//发送在线奖励
				Client->OnlineTime = 0;//重置在线时间并继续检测

			}
			else
			{
				NotifyTaskProgressRate(Client, 148, Client->OnlineTime / 60, 0);
				NotifyTaskProgressRate(Client, 149, Client->TotalOnlineTime / 60, 0);
			}
		}
	}
	/*if (Clients.size() > 4) {
		exit(0);
	}*/
}

void OnlineUin()
{
	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	std::map<long, std::shared_ptr<ClientNode>>::iterator iter;
	for (iter = Clients.begin(); iter != Clients.end(); iter++)
	{
		std::shared_ptr<ClientNode> Client = iter->second;
		if (Client)
		{
			std::cout << " UIN：" << Client->Uin << std::endl;
		}
	}
}

void NotifyAddTaskInfo(std::shared_ptr<ClientNode> Client)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 5); //TaskNum
	//for (size_t i = 0; i < 1; i++)
	{ //PlayerDBTaskInfo
		BYTE* pPlayerDBTaskInfo = p;
		Write16(pPlayerDBTaskInfo, 0); //len

		Write32(pPlayerDBTaskInfo, 0x06a5); //TaskID 紫钻练习场任务 
		Write16(pPlayerDBTaskInfo, 0); //OptionID
		Write8(pPlayerDBTaskInfo, 0); //Status
		Write8(pPlayerDBTaskInfo, 0); //PrevStatus
		Write32(pPlayerDBTaskInfo, 0); //ProgressRate
		Write32(pPlayerDBTaskInfo, 0); //Other
		Write32(pPlayerDBTaskInfo, 0); //TakeTime
		Write32(pPlayerDBTaskInfo, 0); //LastUpdateTime

		len = pPlayerDBTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //PlayerDBTaskInfo
		BYTE* pPlayerDBTaskInfo = p;
		Write16(pPlayerDBTaskInfo, 0); //len

		Write32(pPlayerDBTaskInfo, 0x49);
		Write16(pPlayerDBTaskInfo, 0); //OptionID
		Write8(pPlayerDBTaskInfo, 0); //Status
		Write8(pPlayerDBTaskInfo, 0); //PrevStatus
		Write32(pPlayerDBTaskInfo, 0); //ProgressRate
		Write32(pPlayerDBTaskInfo, 0); //Other
		Write32(pPlayerDBTaskInfo, 0); //TakeTime
		Write32(pPlayerDBTaskInfo, 0); //LastUpdateTime

		len = pPlayerDBTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //PlayerDBTaskInfo
		BYTE* pPlayerDBTaskInfo = p;
		Write16(pPlayerDBTaskInfo, 0); //len

		Write32(pPlayerDBTaskInfo, 0x6e);
		Write16(pPlayerDBTaskInfo, 0); //OptionID
		Write8(pPlayerDBTaskInfo, 0); //Status
		Write8(pPlayerDBTaskInfo, 0); //PrevStatus
		Write32(pPlayerDBTaskInfo, 0); //ProgressRate
		Write32(pPlayerDBTaskInfo, 0); //Other
		Write32(pPlayerDBTaskInfo, 0); //TakeTime
		Write32(pPlayerDBTaskInfo, 0); //LastUpdateTime

		len = pPlayerDBTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //PlayerDBTaskInfo
		BYTE* pPlayerDBTaskInfo = p;
		Write16(pPlayerDBTaskInfo, 0); //len

		Write32(pPlayerDBTaskInfo, 148);
		Write16(pPlayerDBTaskInfo, 0); //OptionID
		Write8(pPlayerDBTaskInfo, 0); //Status
		Write8(pPlayerDBTaskInfo, 0); //PrevStatus
		Write32(pPlayerDBTaskInfo, 0); //ProgressRate
		Write32(pPlayerDBTaskInfo, 0); //Other
		Write32(pPlayerDBTaskInfo, 0); //TakeTime
		Write32(pPlayerDBTaskInfo, 0); //LastUpdateTime

		len = pPlayerDBTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //PlayerDBTaskInfo
		BYTE* pPlayerDBTaskInfo = p;
		Write16(pPlayerDBTaskInfo, 0); //len

		Write32(pPlayerDBTaskInfo, 149);
		Write16(pPlayerDBTaskInfo, 0); //OptionID
		Write8(pPlayerDBTaskInfo, 0); //Status
		Write8(pPlayerDBTaskInfo, 0); //PrevStatus
		Write32(pPlayerDBTaskInfo, 0); //ProgressRate
		Write32(pPlayerDBTaskInfo, 0); //Other
		Write32(pPlayerDBTaskInfo, 0); //TakeTime
		Write32(pPlayerDBTaskInfo, 0); //LastUpdateTime

		len = pPlayerDBTaskInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 557, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
//其他添加宠物方法
void NotifyAddPet(std::shared_ptr<ClientNode> Client, PETITEM* PetItem)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	{ //PetInfo
		BYTE* pPetInfo = p;
		Write16(pPetInfo, 0); //len

		Write32(pPetInfo, Client->Uin); //Uin
		Write32(pPetInfo, PetItem->PetID); //PetId
		memset(pPetInfo, 0, 17);
		memcpy(pPetInfo, PetItem->PetName, 17);
		pPetInfo += 17;
		Write32(pPetInfo, 0x00000000);
		Write32(pPetInfo, 0x00000000);
		Write8(pPetInfo, 0x01);//IsProper
		//后16字节
				//2字节等级
		Write16(pPetInfo, PetItem->Level);
		//8字节前后经验值
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);

		Write8(pPetInfo, PetItem->Status);

		//4字节前后体力值
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);
		Write8(pPetInfo, 0x11);

		Write16(pPetInfo, 3); //PetSkillNum
		{ //PetSkillList
			BYTE* pPetSkillList = pPetInfo;
			Write16(pPetSkillList, 0); //len

			Write16(pPetSkillList, PetItem->SkillID1); //SkillID
			Write8(pPetSkillList, 1); //Status
			Write8(pPetSkillList, 1); //Active
			Write32(pPetSkillList, PetItem->SkillValue1); //Value

			len = pPetSkillList - pPetInfo;
			Set16(pPetInfo, (WORD)len);
			pPetInfo += len;
		}
		{ //PetSkillList
			BYTE* pPetSkillList = pPetInfo;
			Write16(pPetSkillList, 0); //len

			Write16(pPetSkillList, PetItem->SkillID2); //SkillID
			Write8(pPetSkillList, 2); //Status
			Write8(pPetSkillList, 1); //Active
			Write32(pPetSkillList, PetItem->SkillValue2); //Value

			len = pPetSkillList - pPetInfo;
			Set16(pPetInfo, (WORD)len);
			pPetInfo += len;
		}
		{ //PetSkillList
			BYTE* pPetSkillList = pPetInfo;
			Write16(pPetSkillList, 0); //len

			Write16(pPetSkillList, PetItem->SkillID3); //SkillID
			Write8(pPetSkillList, 3); //Status
			Write8(pPetSkillList, 1); //Active
			Write32(pPetSkillList, PetItem->SkillValue3); //Value

			len = pPetSkillList - pPetInfo;
			Set16(pPetInfo, (WORD)len);
			pPetInfo += len;
		}
		Write32(pPetInfo, PetItem->StrengLevel); //StrengLevel
		Write8(pPetInfo, 0x4); //TopStatus
		{ //CommonInfo
			BYTE* pCommonInfo = pPetInfo;
			Write16(pCommonInfo, 0); //len

			Write32(pCommonInfo, 0x5); //HuanHuaLevel
			Write8(pCommonInfo, 1); //CanHuanHua

			len = pCommonInfo - pPetInfo;
			Set16(pPetInfo, (WORD)len);
			pPetInfo += len;
		}

		len = pPetInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 586, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyLoginMsg(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 1); //MsgNum
	for (size_t i = 0; i < 1; i++)
	{ //LoginMsgList
		BYTE* pLoginMsgList = p;
		Write16(pLoginMsgList, 0); //len

		Write8(pLoginMsgList, 0); //MsgType
		Write16(pLoginMsgList, 0); //UrlLen
		Write16(pLoginMsgList, 16); //MsgLen
		memcpy(pLoginMsgList, "欢迎来到QQ飞车！", 16);
		pLoginMsgList += 16;

		len = pLoginMsgList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 575, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyClientLoveBabyInfoWhenLogIn(std::shared_ptr<ClientNode> Client)
{
	if (Client->EngageUin == 0) {
		return;
	}
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 1); //HaveLoveBaby
	Write16(p, 1); //BeNoticed
	{ //LoveBabyInfo
		BYTE* pLoveBabyInfo = p;
		Write16(pLoveBabyInfo, 0); //len

		Write32(pLoveBabyInfo, 71555); //LoveBabyID
		Write32(pLoveBabyInfo, Client->Uin); //MasterLoveUin
		Write32(pLoveBabyInfo, Client->EngageUin); //ViceLoveUin
		Write32(pLoveBabyInfo, 100); //GrowValue
		Write32(pLoveBabyInfo, 100); //MentalValue
		Write32(pLoveBabyInfo, 100); //BabyGrowLevel
		Write32(pLoveBabyInfo, 5); //BabyProfileLevel
		Write32(pLoveBabyInfo, 0); //BabyStrengthLevel
		Write32(pLoveBabyInfo, 0); //BabyPLValue
		Write32(pLoveBabyInfo, 0); //BabyOrgPLValue
		memset(pLoveBabyInfo, 0, 17);
		memcpy(pLoveBabyInfo, "宝宝1", 5); //LoveBabyNickName[]
		pLoveBabyInfo += 17;
		Write16(pLoveBabyInfo, 0); //BabyStat
		Write16(pLoveBabyInfo, 0); //RemainingActiveDay
		Write16(pLoveBabyInfo, 1); //BackGroundID
		Write16(pLoveBabyInfo, 0); //LoverBabyGender
		Write16(pLoveBabyInfo, 0); //MentalTotalValue
		Write32(pLoveBabyInfo, 0); //NextGrowLevelGrowValue
		Write32(pLoveBabyInfo, 71555); //LoverBabyItemID
		Write16(pLoveBabyInfo, Client->LoverBaby1EquipStat); //EquipStat

		Write32(pLoveBabyInfo, 1); //BabySkillNums
		for (size_t i = 0; i < 1; i++)
		{ //SkillInfos
			BYTE* pSkillInfos = pLoveBabyInfo;
			Write16(pSkillInfos, 0); //len

			Write16(pSkillInfos, 15); //SkillID 
			Write8(pSkillInfos, 1); //Status
			Write8(pSkillInfos, 1); //Active
			Write32(pSkillInfos, 500); //Value 

			len = pSkillInfos - pLoveBabyInfo;
			Set16(pLoveBabyInfo, (WORD)len);
			pLoveBabyInfo += len;
		}


		Write16(pLoveBabyInfo, 0); //TreatMentalValue
		Write32(pLoveBabyInfo, 0); //DailyLikeNum
		Write32(pLoveBabyInfo, 0); //TotalLikeNum
		Write32(pLoveBabyInfo, 0); //DailyLearningNum
		Write32(pLoveBabyInfo, 0); //DaliyLearningLimtNum
		Write32(pLoveBabyInfo, 0); //AlreadyLikeNums
		Write32(pLoveBabyInfo, 0); //RemainingLikeNums
		Write16(pLoveBabyInfo, 5); //BabyTransferdProfileLevel
		Write16(pLoveBabyInfo, 0); //HaveBirthPresent

		len = pLoveBabyInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //SecondLoveBabyInfo
		BYTE* pSecondLoveBabyInfo = p;
		Write16(pSecondLoveBabyInfo, 0); //len

		Write32(pSecondLoveBabyInfo, 71554); //LoveBabyID
		Write32(pSecondLoveBabyInfo, Client->Uin); //MasterLoveUin
		Write32(pSecondLoveBabyInfo, Client->EngageUin); //ViceLoveUin
		Write32(pSecondLoveBabyInfo, 100); //GrowValue
		Write32(pSecondLoveBabyInfo, 100); //MentalValue
		Write32(pSecondLoveBabyInfo, 100); //BabyGrowLevel
		Write32(pSecondLoveBabyInfo, 5); //BabyProfileLevel
		Write32(pSecondLoveBabyInfo, 0); //BabyStrengthLevel
		Write32(pSecondLoveBabyInfo, 0); //BabyPLValue
		Write32(pSecondLoveBabyInfo, 0); //BabyOrgPLValue
		memset(pSecondLoveBabyInfo, 0, 17);
		memcpy(pSecondLoveBabyInfo, "宝宝2", 5); //LoveBabyNickName[]
		pSecondLoveBabyInfo += 17;
		Write16(pSecondLoveBabyInfo, 0); //BabyStat
		Write16(pSecondLoveBabyInfo, 0); //RemainingActiveDay
		Write16(pSecondLoveBabyInfo, 1); //BackGroundID
		Write16(pSecondLoveBabyInfo, 1); //LoverBabyGender
		Write16(pSecondLoveBabyInfo, 0); //MentalTotalValue
		Write32(pSecondLoveBabyInfo, 0); //NextGrowLevelGrowValue
		Write32(pSecondLoveBabyInfo, 71554); //LoverBabyItemID
		Write16(pSecondLoveBabyInfo, Client->LoverBaby2EquipStat); //EquipStat
		Write32(pSecondLoveBabyInfo, 1); //BabySkillNums
		for (size_t i = 0; i < 1; i++)
		{ //PetSkillList
			BYTE* pPetSkillList = pSecondLoveBabyInfo;
			Write16(pPetSkillList, 0); //len

			Write16(pPetSkillList, 15); //SkillID
			Write8(pPetSkillList, 1); //Status
			Write8(pPetSkillList, 1); //Active
			Write32(pPetSkillList, 500); //Value

			len = pPetSkillList - pSecondLoveBabyInfo;
			Set16(pSecondLoveBabyInfo, (WORD)len);
			pSecondLoveBabyInfo += len;
		}
		Write16(pSecondLoveBabyInfo, 0); //TreatMentalValue
		Write32(pSecondLoveBabyInfo, 0); //DailyLikeNum
		Write32(pSecondLoveBabyInfo, 0); //TotalLikeNum
		Write32(pSecondLoveBabyInfo, 0); //DailyLearningNum
		Write32(pSecondLoveBabyInfo, 0); //DaliyLearningLimtNum
		Write32(pSecondLoveBabyInfo, 0); //AlreadyLikeNums
		Write32(pSecondLoveBabyInfo, 0); //RemainingLikeNums
		Write16(pSecondLoveBabyInfo, 5); //BabyTransferdProfileLevel
		Write16(pSecondLoveBabyInfo, 0); //HaveBirthPresent

		len = pSecondLoveBabyInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //LoverBabyCfgInfo
		BYTE* pLoverBabyCfgInfo = p;
		Write16(pLoverBabyCfgInfo, 0); //len

		Write32(pLoverBabyCfgInfo, 0); //LikeAddMentalValue
		Write32(pLoverBabyCfgInfo, 0); //DailyLikeAddMentalValue
		Write32(pLoverBabyCfgInfo, 0); //DailyBeLikedNumsToGetCoupon
		Write32(pLoverBabyCfgInfo, 0); //AwardCouponNums
		Write32(pLoverBabyCfgInfo, 0); //LearningAddGrowValue
		Write32(pLoverBabyCfgInfo, 0); //LearningTime

		len = pLoverBabyCfgInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //SecondLoverBabyCfgInfo
		BYTE* pSecondLoverBabyCfgInfo = p;
		Write16(pSecondLoverBabyCfgInfo, 0); //len

		Write32(pSecondLoverBabyCfgInfo, 0); //LikeAddMentalValue
		Write32(pSecondLoverBabyCfgInfo, 0); //DailyLikeAddMentalValue
		Write32(pSecondLoverBabyCfgInfo, 0); //DailyBeLikedNumsToGetCoupon
		Write32(pSecondLoverBabyCfgInfo, 0); //AwardCouponNums
		Write32(pSecondLoverBabyCfgInfo, 0); //LearningAddGrowValue
		Write32(pSecondLoverBabyCfgInfo, 0); //LearningTime

		len = pSecondLoverBabyCfgInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 1); //HaveRaisedSecondBaby
	Write8(p, Client->LoverBaby1EquipStat + Client->LoverBaby2EquipStat); //EquipBabyStatus
	Write16(p, 0); //RemainingDanceNums
	Write16(p, 0); //RemainingMaimengNums
	Write16(p, 0); //MaxDanceNums
	Write16(p, 0); //MaxMaimengNums

	len = p - buf.data();
	SendToClient(Client, 24370, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyBriefGuildInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	memset(p, 0, 17);//GuildName[]
	memcpy(p, Client->GuildName, 17);//必写 不然个人信息无法触发15009
	p += 17;
	Write32(p, Client->GuildID); //GuildID
	Write16(p, Client->GuildDuty); //Duty
	Write32(p, 0); //Right
	Write32(p, 0); //Medal
	Write32(p, 0); //Contribute
	Write16(p, 0); //TeamAddition
	Write32(p, 0); //GuildScore
	Write32(p, 0); //LeaderUin
	Write16(p, 0); //YesterdayMaxOnline
	Write32(p, 0); //LastSingleLoginTime
	Write16(p, 0); //GuildLevel
	Write8(p, 0); //TitleLen
	Write32(p, 0); //DayChallenge
	Write32(p, 0); //CurSeasonId
	Write16(p, 0); //LastSeasonMapID
	Write16(p, 0); //LastSeasonRank
	Write32(p, 0); //LastHero
	Write32(p, 0); //CurHero
	Write32(p, 0); //LastEnlistedTime
	Write32(p, 0); //LastModGuildNameTime
	Write32(p, 0); //GuildCreateTime
	Write32(p, 0); //JoinGuildTime
	Write32(p, 0); //ScoreRank
	Write32(p, 0); //WonderLandID
	Write16(p, 0); //MemberNum
	Write32(p, 0); //Prosperity
	Write16(p, 0); //WonderLandDuty
	Write32(p, 0); //TotalWlDegree
	Write32(p, 0); //WeeklyWlDegree
	Write32(p, 0); //LastCheckWlDegreeTime
	Write32(p, 0); //WeeklyWlDegreeForMatch
	Write32(p, 0); //WeeklyMatchScore
	Write32(p, 0); //WeeklyUsedMatchScore
	Write32(p, 0); //TotalMatchScore
	Write32(p, 0); //WeeklyGettedWlWelfareIdBitSet
	Write32(p, 0); //LastGetWlBonusTime
	Write32(p, 0); //BanWlTalkEndTime
	Write32(p, 0); //GetWlBonusEndTime
	Write32(p, 0); //LastDayCheerValue
	Write32(p, 0); //LastDayCheerCnt
	Write32(p, 0); //LastRegisterTime
	Write32(p, 0); //CurrentOnLineGuildMembers
	Write8(p, 0); //LeaderFirstOpened
	Write8(p, 0); //AppliedChampionship

	len = p - buf.data();
	SendToClient(Client, 15205, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void CheckRankData(std::shared_ptr<ClientNode> Client)
{
	if (Client && Client->GradeScore < 0) {
		Client->GradeScore = 0;
		PlayerDB_SetRankedInfo(Client);
	}
}

void RequestCreateBorderTeamRoom(std::shared_ptr<ClientNode> Client)
{
	std::shared_ptr<RoomNode> Room = NewRoom();
	if (!Room)
	{
		return;
	}
	Room->BaseGameMode = 41;
	Room->SubGameMode = 1;
	strcpy_s(Room->Name, "组队边境");
	Room->ServerID = Client->ServerID;
	Room->RoomOwnerID = Client->ConnID;
	Client->RoomID = Room->ID;
	Room->SeatNum = 6;
	Room->CurrentPlayerNum = 1;
	Room->Flag = 0x18c;
	for (char i = 0; i < 6; i++)
	{
		Room->Player[i] = nullptr;
		Room->SeatState[i] = 1;
	}
	Room->Player[0] = Client;
	Client->ReadyState = 0;
	Client->ChatRoomID = -1;
	ResponseCreateRoom(Client, Room, 0);
}


bool compare_char_and_byte(const char* str, const unsigned char byte_arr[16]) {
	size_t str_len = strlen(str);

	// 如果字符串长度大于 16，则不相等
	if (str_len > 16) {
		return false;
	}

	// 比较字节
	for (size_t i = 0; i < 16; ++i) {
		unsigned char str_byte = (i < str_len) ? static_cast<unsigned char>(str[i]) : 0;
		if (str_byte != byte_arr[i]) {
			return false;
		}
	}
	return true;
}

// 将字节数组转换为十六进制字符串
std::string bytes_to_hex(const unsigned char* data, size_t length) {
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	for (size_t i = 0; i < length; ++i) {
		oss << std::setw(2) << static_cast<int>(data[i]);
	}
	return oss.str();
}

extern void Write21032(std::shared_ptr<ClientNode> Client);

void RequestLogin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)//请求登录
{

	UINT Uin = Read32(Body);
	if (SpecialLoginUins.find(Uin) != SpecialLoginUins.end())
	{
		Client->LoginMode = 1;
	}
	//std::cout << Uin << std::endl;
	if (PreventSDP[0] > 0) {
		std::string Key = GetKey(Uin);
		if (!Key.empty())
		{
			BYTE KeyT[16];
			memcpy(KeyT, Key.c_str(), 16);
			std::string ClientKeyHex = bytes_to_hex(Client->Key, 16);
			std::string KeyHex = bytes_to_hex(KeyT, 16);
			if (ClientKeyHex != KeyHex) {
				auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
				ServerLogger->info("[登录发包Key不匹配嫌疑:][读取包UIN:{:d}][Client的UIN:{:d}][IP:{:s}][MsgID:{:d}][GetKey:{:s}][ClientKey:{:s}]", Uin, Client->Uin, Client->Ip, 100, KeyHex, ClientKeyHex);
				ServerLogger->flush();
				Client->IsLogin = FALSE;
				Client->Server->Disconnect(Client->ConnID);
				Client->deleteflag = 1;
				return;
			}
		}
		else
		{
			auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
			ServerLogger->info("[登录发包未找到Key嫌疑:][读取包UIN:{:d}][Client的UIN:{:d}][IP:{:s}][MsgID:{:d}]", Uin, Client->Uin, Client->Ip, 100);
			ServerLogger->flush();
			Client->IsLogin = FALSE;
			Client->Server->Disconnect(Client->ConnID);
			Client->deleteflag = 1;
			return;
		}

	}

	UINT Time = Read32(Body);

	UINT ClientVersion = Read32(Body);

	//QQNickName[20]
	Body += MaxQQNickName;
	//Gender
	Body += 1;
	//IconID
	Body += 1;

	UINT AttachIdentify = Read32(Body);
	UINT LoginType = Read32(Body);
	UINT LoaderTipVersion = Read32(Body);
	UINT ClientStatusContex = Read32(Body);
	BYTE IsSimplifyVersion = Read8(Body);
	UINT CurTopMapRecordVersion = Read32(Body);
	UINT CurKartRoomPushInfoVersion = Read32(Body);
	BYTE* p = Body; WORD len;
	short PreSvrID = Read16(p);
	UCHAR TGPSignatureLen = Read8(p);
	UINT RandomKey = Read32(p);
	{ //ClientFingerprint
		BYTE* pClientFingerprint = p;
		len = Read16(pClientFingerprint);

		UINT PID = Read32(pClientFingerprint);
		int ServeryVerifyKey = 1414812756/* ^ PID*/;
		if (Client->LoginMode == 1)
		{
			ServeryVerifyKey = 1414812756;
		}
		Client->ServeryVerifyKey = ServeryVerifyKey;
		UINT MacAddrHigh = Read32(pClientFingerprint);
		UINT MacAddrLow = Read32(pClientFingerprint);
		UINT CPUID = Read32(pClientFingerprint);
		UINT BiosID = Read32(pClientFingerprint);

		p += len;
	}
	UINT ConnID = Read32(p);
	bool UseTGP = Read8(p);
	USHORT BarPriviSignatureLen = Read16(p);
	int NetBarLev = Read32(p);
	bool Reconnect = Read8(p);
	short LoginBuffLen = Read16(p);
	USHORT ClientKeyLen = Read16(p);
	UINT PlayerFlag = Read32(p);
	UCHAR LoginSrcType = Read8(p);
	UCHAR RequestLoginType = Read8(p);
	USHORT NetBarTokenLen = Read16(p);
	/*
m_sPreSvrID
m_u8TGPSignatureLen
m_uiRandomKey
m_stClientFingerprint.m_uiPID
m_stClientFingerprint.m_uiMacAddrHigh 物理地址高位
m_stClientFingerprint.m_uiMacAddrLow
m_stClientFingerprint.m_uiCPUID cpuid
m_stClientFingerprint.m_uiBiosID biosid
m_uiConnID
m_bUseTGP
m_usBarPriviSignatureLen
m_iNetBarLev
m_bReconnect
m_shLoginBuffLen
m_usClientKeyLen
m_uiPlayerFlag
m_ucLoginSrcType
m_ucRequestLoginType
m_ushNetBarTokenLen
	*/

	//初始化一些数据
	Client->Uin = Uin;
	UserDB_ConnID(Uin, Client->ConnID);
	//printf("ClientVersion:%d, LoginType:%d\n", ClientVersion, LoginType);

	Client->Pet = std::shared_ptr<PetInfo>(new PetInfo{ 0, "", 0, 0, 0, 0, 0 });


	try
	{
		std::vector<ItemInfo> vecItems;
		PlayerDB_LoginSelectItems(Client->Uin, vecItems);
		if (loginMode == 0) {
			ResponseLogin(Client, ClientVersion, LoginType, 0, {}, true);
			CheckRankData(Client);
			NotifyLoginMoreInfo(Client, vecItems);
		}
		else {
			std::vector<ItemInfo> Items(200);
			size_t ItemNum = 0;
			bool HasMoreInfo = false;

			for (size_t i = 0; i < vecItems.size(); i++)
			{
				Items[ItemNum].ItemID = vecItems[i].ItemID;
				Items[ItemNum].ItemNum = vecItems[i].ItemNum;
				Items[ItemNum].AvailPeriod = vecItems[i].AvailPeriod;
				Items[ItemNum].Status = vecItems[i].Status;
				Items[ItemNum].ObtainTime = vecItems[i].ObtainTime;
				Items[ItemNum].OtherAttribute = vecItems[i].OtherAttribute;
				Items[ItemNum].ItemType = 0;
				if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_CAR)
				{
					Client->KartID = Items[ItemNum].ItemID;
					// printf("KartID:%d\n", Client->KartID);
				}
				else if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_PET)
				{
					Client->Pet->ID = Items[ItemNum].ItemID;
					GetPetInfo(Client);
				}

				ItemNum++;
				if (ItemNum >= 100)
				{
					HasMoreInfo = true;
					break;
				}
			}
			ResponseLogin(Client, ClientVersion, LoginType, ItemNum, Items, HasMoreInfo);

			size_t offset = 101; // Start after the first 100 items
			while (HasMoreInfo)
			{
				ItemNum = 0; // Reset ItemNum for the next batch
				HasMoreInfo = false;

				for (size_t i = offset; i < vecItems.size(); i++)
				{
					if (ItemNum >= 100) // Check if we need to break to handle more data
					{
						HasMoreInfo = true;
						offset = i; // Update offset to start from the current position in the next loop
						break;
					}

					Items[ItemNum].ItemID = vecItems[i].ItemID;
					Items[ItemNum].ItemNum = vecItems[i].ItemNum;
					Items[ItemNum].AvailPeriod = vecItems[i].AvailPeriod;
					Items[ItemNum].Status = vecItems[i].Status;
					Items[ItemNum].ObtainTime = vecItems[i].ObtainTime;
					Items[ItemNum].OtherAttribute = vecItems[i].OtherAttribute;
					Items[ItemNum].ItemType = 0;

					if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_CAR)
					{
						Client->KartID = Items[ItemNum].ItemID;
					}
					else if (Items[ItemNum].Status && GetItemType(Items[ItemNum].ItemID) == EAIT_PET)
					{
						Client->Pet->ID = Items[ItemNum].ItemID;
						GetPetInfo(Client);
					}

					ItemNum++;
				}

				NotifyLoginMoreInfo(Client, ItemNum, Items, HasMoreInfo);
				NotifyClientAddItem(Client, ItemNum, Items);

				ItemStatus NewStatus;
				for (size_t i = 0; i < ItemNum; i++)
				{
					if (Items[i].Status)
					{
						NewStatus.ItemID = Items[i].ItemID;
						NewStatus.NewStatus = true;
						ResponseChangeItemStatus(Client, 1, { NewStatus });
					}
				}
			}
		}
		//InsertItem(Client);//添加背包道具

		getGameMode(Client);

		NotifyClientVipFlag(Client);//通知客户端vip标志
		PlayerDB_SelectClientInfo(Client);

		NotifyMsgBox(Client);
		NotifyRandRoomNameList(Client);
		NotifyTopListDesc(Client);
		NotifySpeed2Cfg(Client);
		NotifySvrConfig(Client);//添加服务端配置
		NotifySvrConfig2(Client);
		NotifySvrConfig3(Client);//NPC配置
		NotifyCompetitionSystemConfig(Client);

		//未知
		if (1) {
			NotifyFirstRechargeCfg(Client);//13722
			NotifyG2CFizzStarCfg(Client);//11595
			NotifyItemType2ndCfg(Client);//314
			NotifyMultiUseItemListCfg(Client);//347
			NotifyGroupBuyingrCfg(Client);//680   10928
			NotifyMusicBlackList(Client);//952
			NotifyFBPlayerLevInfo(Client);//20069
			NotifyClientPersonalGardenCfg(Client);//20145  21001
			ResponseJiaNianHuaCfg(Client);//24076
			NotifyAllAreaNameCfg(Client);//24639
			//Write21032(Client);
			NotifyAllCityCfg(Client);//24811  24071
			NotifyScoreBoxCfg(Client);//24423 24820
			NotifyTVLiveInfo(Client);//24822
			NotifyRelaxRoomAdvertisement(Client);//24009 24033

			NotifyClientGolumBallAwardInfo(Client);//24456
			NotifyActivityRecommendCfg(Client);//24448
			NotifyClientNoviceGuideCfg(Client);//24506
		}


		NotifyTopUIItemInfoNew(Client);//25114
		NotifyRedEnvelopeInfo(Client);//24061
		NotifyAdvertisement(Client);

		//NotifyMatchResultLogin(Client);
		NotifyFileCheckList(Client);

		NotifySpecialActivityInfo(Client);//通知职业生涯段位信息
		//NotifyClientDomainResolve(Client);//2100

		NotifyClientSkillTrainingTipsInfo(Client);//28341
		NotifyClientUpdateItemDescInfo(Client);//24379

		NotifyTopUIItemInfoNew(Client);
		NotifySkillStoneKartInfo(Client);

		initiOtherInfo(Client);
		//可以再次使用该连接
		NotifyAddTaskInfo(Client);//添加任务配置
		NotifyS2CMyKartAltas(Client);//车库图集 图册
		//登录时通知 点击别人的个人信息车库会触发13705 RequestC2SSearchKartAltas消息
		NotifyRankedMatchSeasonInfo(Client, Client->Uin);//通知排位等级
		NotifyLoginMsg(Client);

		//NotifyKartActiveCfgInfo(Client);
		//NotifyPlayerAllActiveKartInfo(Client);

		NotifyPetStrengCfgInfo(Client);
		//NotifyPetHuanHuaCfgInfo(Client);
		NotifyClientLoveBabyInfoWhenLogIn(Client);
		NotifySecondPassInfo(Client);
		if (Client->GuildID)
		{
			NotifyBriefGuildInfo(Client);
		}

		if (Client->ServerID == 200 && BorderTeam >= 1)
		{
			RequestCreateBorderTeamRoom(Client);
		}
		else if (Client->ServerID == 200 && BorderTeam < 1)
		{
			ResponseStartGame(Client, "组队边境已关闭!");
		}
		if (NewKart == 2) {
			PlayerDB_AddStoneCar(Client);
		}
		if (Client->AppellationType > 0)
		{
			ResponseWearAppellation(Client, Client->AppellationType);
		}
		NotifyBroadcast65(Client);


	}
	catch (...)
	{
		//throw;关闭
	}



}
void CheckDupUser(UINT Uin, std::shared_ptr<ClientNode> Client)
{
	auto disconnect = [&Client](UINT Uin, std::shared_ptr<ClientNode>& OldClient) {
		if (OldClient)
		{
			BYTE* buf2 = msg900;
			buf2 += 2;
			Write32(buf2, Uin);
			SendToClient(OldClient, 900, msg900, 52, OldClient->ConnID, FE_PLAYER, OldClient->ConnID, Notify);
			char ip[24] = { 0 };
			memcpy(ip, OldClient->Ip, 24);
			OldClient->IsLogin = FALSE;
			OldClient->Server->Disconnect(OldClient->ConnID);
			OldClient->deleteflag = 1;
			if (!Client)
			{
				DisConnectUser(ip, Uin);
			}
		}
	};
	std::lock_guard<std::recursive_mutex> lg(ClientMutex);
	std::shared_ptr<ClientNode> OldClient = GetClient(Uin);
	std::shared_ptr<ClientNode> OldRelaxClient = GetRelaxClient(Uin);
	disconnect(Uin, OldClient);
	disconnect(Uin, OldRelaxClient);
}

void convertTimeStamp2TimeStr(time_t timeStamp, char* temp) {
	struct tm timeinfo;
	localtime_s(&timeinfo ,&timeStamp);
	snprintf(temp,
		27,
		"%04d%02d%02d",
		timeinfo.tm_year + 1900,
		timeinfo.tm_mon + 1,
		timeinfo.tm_mday);
}

void ResponseLogin(std::shared_ptr<ClientNode> Client, UINT ClientVersion, UINT LoginType, UINT ItemNum, std::vector<ItemInfo> Items, bool HasMoreInfo)
{
	//std::string Byte ="0000000066ED18FB00641B9200F1A401A342C422E4A14041C4C74024BD9F78660233F9A7A292A7A987A32DC44708089B7494B4DC131313131313131313B1FC554ADE06AA1A997EC596EEB365DB2F5251B76ECB710261232AD56DD399A65E0628E741A4D94847108B8A719D34BC9353D0D379F074BBE7D59A05D5787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878E6A6FFCE471CAF4A21FFB886CB14D4655522668CB886B59A7E16C2926D289B01256A87555D7AA62FCD2623554655247E7D66EEA494737373737DA43868DFD4FC1BC236A494737373737DA4ACB18AC9FC1BC21AA494737373737DA4ACB18AC9FC1BC230A494B416B4C0F2E4BADB1BC2CBA494737373737DA4ACB18AC9FC1BC22738BEF1B3A4651C8AC9FC1BC2CEDC0BB3A44527F101FC1BC2EADC94737373737DA422A83856FC1BC20CDC94737373737DA457A3D45DFC1BC214DC94737373737DB46D6A019EFC1BC223DC94737373737DB48862E086FC1BC2913494737373737DA44B01BA7CDB1BC2603494B3A450161FBADB1BC20C9094737373737DA4DD278638FC1BC2C8E8BE94B3A42E19D238DB1BC2D5E8D5B3A4C05750A7FC1BC2F1E87CB3A4E2E60B65FC1BC2B7D3C4B3A4E697CF38FC1BC283D3B7B3A48A97CF38FC1BC2EAA85BB3A463623038FC1BC2E90E94737373737DA4802780A9FC1BC2630E94737373737DB438CFF486FC1BC2EAEB94737373737DA4F4668AAEFC1BC298EB7CB3A410E650A7FC1BC20AEBD5B3A4C3BA50A7FC1BC27A2294B3A41AB3C386FC1BC274227CB3A4B95F269EFC1BC2B4B5B2B3A49ED84B86FC1BC202B594737373737DA4A0550314FC1BC2176660B3A4B00B8AC9FC1BC2CD66F294B3A48BBA2738FC1BC26F6669B3A43BD68A86FC1BC20D66EAB3A41A8E9000FC1BC2046683B3A4E67C5728DB1BC20166E194B3A40E71C67CFC1BC2556657B3A46068D2E9FC1BC29E6632B3A4B58871BADB1BC2BA66A994B3A45EF29A7CDB1BC28A668AB3A40202E37CDB1BC2196668B3A4451ECF7CDB1BC21B6694B3A4E3161FBADB1BC2C16694B3A44EFEDD1FFC1BC2F96694B3A40939E957DB1BC242795BB3A4F8BA50A7FC1BC228D094B818D3B47025DF57DBFAFAFABA07C211D0D0B3A46D0A45A9FC1BC251FF9EB3A4628BAD38FC1BC22FFF14B3A4975EACE2FC1BC28808B2B3A40903E300FC1BC251B01BB3A4EC6D0086FC1BC22FB07CB3A4B1E0FE50DB1BC25CB0C7B3A49AE6F2A9FC1BC28AB06DB3A4DDE60EA9FC1BC2898B7CB3A44E0B1F06FC1BC28D8B79B3A497E55DE9FC1BC29E8BCCB3A43D68E3A9FC1BC28A8B4BB3A460F22214FC1BC2E68B70B3A49BA0757CDB1BC2198B24B3A48B65CF38FC1BC2E22694B70EA4E0D203BADB1BC29A9494B3A4D49625E1FC1BC21F7694737373737DA4567548A9FCFAFAFAFA13C296761FB3A44856FEC9FC1BC261767CB3A40F16757CDB1BC21176B2B3A470AE0145DB1BC20B7694B3A49028E250DB1BC2A17610B3A48ED645A9FC1BC2307679B3A430873038FC1BC2CE8E5BB3A4F837D686FC1BC2988E94737373737DA48E028796FC1BC23A8521B3A491680EA9FC1BC288859D94B3A43414E186FC1BC2B7BB6E94B3A47D7319A9FC1BC283BBFD7CB3A4ACE3B138FC1BC22F5707B3A4390E877CDB1BC26C57EF94B3A491D28AC9FC1BC2D7576594B3A4DDE60EA9FC1BC24D57E5B3A4B0A5DDB9FC1BC21020B2B3A42891B100FC1BC24B2002B3A4B57C00C9FC1BC2638F94737373737DA4CEA7D686FCFAFAFAFA07C2D84F94737373737DB49B22903BFCFAFAFAFA13C25F8094737373737DA410CF0FAEFC1BC2AF8094737373737DA4E0DDCEE0FC1BC2148094737373737DA4CEE46360FC1BC229F7BD2EB3A46A14027CFC1BC22FF794B3A427B91586DB1BC205F7B7B3A4B31A8C09FC1BC272A694737373737DA4B00047D4FCFAFAFABA07C24B5D94737373737DA434D1A300FC1BC2F05D94737373737DA4B0EE9186FC1BC2C75DD694B3A40BBE1FBADB1BC283B194737373737DA4C99B9086FC1BC21DB194737373737DA40F39E957DBFAFAFAFA13C217B114B3A4390F50C9FC1BC2CDB127B3A40614027CFC1BC2FCB114B3A4662E22A9FC1BC26FB1B2B3A415E5D65DFC1BC204B1AEB3A485E9FEC9FC1BC2D2B1BFB3A44D0B1F7CFC1BC277B10BB3A4D29AB938FC1BC2BAB194B3A4A53D8C38FC1BC26C4B2594737373737DA40BF4919EFC1BC29C062524B3A46B9AFEC9FC1BC278062508B3A4D2979B7CFC1BC20F06251BB3A4F4C68AC9FC1BC255062594737373737DA4751FFECBFC1BC28A062594737373737DA49AE92E1DFC1BC23E062594737373737DA4EE8B805EFC1BC227062594737373737DA406DDB18BFC1BC24BCA2594737373737DA45610D1AEFC1BC2D8CA2594737373737DA40FC07DD4FC1BC24B872594737373737DA49BC9919EFC1BC23F87252EB3A41C025E00FC1BC29C872594737373737DA44DC31C37FCFAFAFAFA13C20F872594B036A4BAD203BADB1BC21487254BB3A427CEB3E1FC1BC2D9872594737373737DA44D14A300FC1BC274B72594BF2833A48CD203BADB1BC217B72594BF2833A48CD203BADB1BC26FB72594BF2833A48CD203BADB1BC2B7832594B71F33A4F1D203BADB1BC2648325F6B2B3A46200F486FC1BC22883259EB3A44DAC16A9FC1BC2A1512537B3A44BADE17CFC1BC2E5512514B3A486F450C9FC1BC25051252EB3A4E18E4786FC1BC2052F2594B3A4ACB18AC9FC1BC26C2F25EEB3A44E1D7CA7FC1BC20B5C2594737373737DA40045D45DFC1BC2B7052594737373737DA43413344DFC1BC29C052594B3A46D684500FC1BC23E052594B3A43C285728DB1BC260052524B3A4261545B9FC1BC2B9D725A7B3A4CFAC3038FC1BC2C6D72532B3A43DF83086FC1BC2CBD7254BB3A4E33756AEFC1BC277422594B3A450D2A9E3FC1BC24BE42583B3A41DAD01E0FC1BC217E4257CB3A455914E00FC1BC255E42594B3A4FE9EACE9FC1BC274CE254AB3A4B91CBAC9FC1BC21DCE254694B3A48C39BAC9FC1BC2B2B4252EB3A41626F87CDB1BC240B42523B3A4D8FE03B5FC1BC2D9B4257CB3A45E85A07CDB1BC23CB42594737373737DA419B58863FC1BC2C6F025B2B3A4AED15728DB1BC2C7F0251FB3A4D666DD00FC1BC263F025B2B3A43073E037FC1BC2F189252EB3A43D88FEC9FC1BC21A89251FB3A45ED4D600FC1BC264892570B3A414B55E39FC1BC20C0A2594737373737DA48563B9E9FC1BC24BC82594B3A43D254716FC1BC2D81A257CB3A4108CD6D4FC1BC2061A256DB3A4B5A3D1E9FC1BC2AC1A25B2B3A4B5E0CE28DB1BC25A1A252EB3A466270B65FC1BC2361A2594B3A40FE937A8FC1BC20A1A25BAB3A427161FBADB1BC2A31A2594B3A49B160662FC1BC2E71A2594B3A41C90A762FC1BC2AF1A2594B3A4CE700662FC1BC2BA1A251BB3A41E3DE957DB1BC2271A2532B3A4A96AE516FC1BC2451A254BB3A480739BD4FC1BC2A01A252EB3A42837ADE0FC1BC2AD4E25E2B3A41F020BE9FC1BC26C4E25B2B3A4A000F1AEFC1BC2EA4E257CB3A4A8975DE9FC1BC28D4E252EB3A477AE1AE9FC1BC2E24E25EEB3A45FAE3086FC1BC2394E250BB3A4EF75225EFC1BC2043F2594B3A46500035DFC1BC2CA4C250FB2B3A4B30FE4C9FC1BC275022594737373737DA455E4D139FC1BC277022594B3A48E0B5706FC1BC2CB022564B3A4974D7CC9FC1BC24BE32594B3A47D5FE18BFC1BC2AAC225B2B3A43BB0155EFC1BC2E5392594B3A4BE34A886FC1BC287E725DDB3A403908AC9FC1BC205E725B2B3A4A3B01E00FC1BC2E6142574B3A4E0DDCEE0FC1BC223562594737373737DA410CF0FAEFC1BC24D562594737373737DA4E0DDCEE0FC1BC2102A2594737373737DA4CEE46360FC1BC297402594737373737DA4A7020145DB1BC2D8752594B3A4E21E0086DB1BC287752594737373737DA4718B30FEDBFAFAFAFA13C23C742556B3A40A2ED886FC1BC2CDD92594737373737DA491E4D139FC1BC263D925B7B3A44EAE371DFC1BC2CBD92507B3A48ABA2738FC1BC2AD172594737373737DA4964EA3BADB1BC251172594737373737DA4E0C0E3BADB1BC25C172594737373737DA41D90B510FC1BC288CD2594B3A40FB150EFDB1BC2A0CD254BB3A4660B5FEFDB1BC2EC1D3DEA8A4BCD2102D78267D3136BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1BAA7005778341B264023F0664D6E319402076197F79D7EB2AEA5B5A030479D7C302885B3F136876D7CF34B1FC2A760479D77379375B722B5506D77B79D142196D0A00D67774A4FC745F7B55D65740C742116E7579D75F462BFC44106F7FD66325552011101C2ED66B619EBA261B7D76D76756D35E69165376D76F7EF75B261B7D76D7432FE85B18430176D7BBBABF1CFC32083D07D6B7BA9626420A165601D6BFBAD7795B2C0F0C76D793BA5430FC580E4D75D79BBA490FBA4E116276D797BA73AD4208167627D69FBAA40F207A347301D6ABBA792BFC201C6776D7A7BA01F55B16387E79D7AFBA97EB5B41497479D783BAA20CFC201C6776D78BBA41745B66317D79D787BA9FBDBA1B595E76D78FBA1CE34243637400D633BAD02AFC08740B26D63BBAEB76FC264D777BD737BA412BFC660D7F79D73FBAE30F2018556527D61BBACF6A201A057855D617BAE818F333066900D61FBA3A7C5B74767879D723BA778B5B1B743B0ED62BBAC7AAFC7C1C0C76D727BAA2355B4A641B05D607BAD3A15B1D276A79D70FBAAD675B2A760479D7F1BADF112020106810D6F9BA7C5220505E7902D6F5BAC7ADBA2A355776D7FDBAF6E84266317D79D7D1BA1F9EFC3E02780CD6D9BA02C5FC5613740AD6D5BA6F7CFC79413557D7DDBAB188BA38624E75D7E1BA65E3422A760479D7E9BA8BDC2031391520D6E5BAFF88FC4E737D05D6EDBA884420427C3D07D6C9BAAE97FC59034200D6C5BA63AF42125B7F00D671BA566DFC330B5300D679BA9C76FC5A4F3078D775BA4561204B194C05D67DBA37C742301C6F7ED659BA5A675E76287D76D755BA638B427A073E00D65DBA5272205A4F3078D761BA368AFC16447E79D769BAFFA4BA386E4E75D765BA58FD2071567579D76DBAEC765B2A780876D749BAAE9EBA7B4D5D76D745BACD01BA0F286776D74DBA3FD4BA5A225876D7B1BA36C75B59387E79D7B9BA88B12031391520D6B5BA90B8BA053D7776D7BDBAFEB12055266504D691BA7E91FC26187D79D799BA181FFC5D591511D695BA6A605B18430176D79DBA7030FC7B03063DD6A1BA409042346B171FD6A9BA7C1A423A012E00D6A5BAD0725B162F7C76D7ADBA77D64232164407D681BAD5BF5B063B7C76D789BA6002F4346B171FD68DBAF8A1206B0C3B0ED631BA26B1FC7F206876D73DBADB63FC62034307D611BA416FBA2C0F0C76D719BA3A5F4201283202D615BA11442042105208D61DBA7261FC41201520D621BA4CE0206E1D0505D629BA119FF423315800D625BAD35FFC76197F79D72DBA9FC75B660D7F79D701BA6F314252704807D609BAD06DFC2E167B0DD605BA2BA4BA1B595E76D7FABA58B1FC0B7B240ED6F6BA0E822001283202D6FEBAFEE35B74767879D7D2BAC1B1F35B313D78D6DABA8A97201416161FD6D6BA736FFC2A760479D7DEBAEC26BA386E4E75D7E2BA2B49425A244D00D6EABA6E7C20540F5620D6E6BAD1AE201A1F2D00D6EEBA11095B264D777BD7CABA0F1C5B52522D7ED7C6BA1C535E6C280C76D7CEBAA141FC6C280C76D772BA11405B3E143F07D77EBA1A82F4645F171FD652BA561A2018556527D65ABAA4BD425C290979D756BA1AAD5B0A6A0479D766BA26ADFC6C776223D66EBA1849FC31391520D64ABAD68B5B3C2F6777D746BA1FE3BA597E5376D74EBAEB36FC2E03490CD6B3B68B7EBA1D2D0D76D7BBB6C13CFC76197F79D7B7B67235FC3C2F6777D7BFB645D6204A227D79D793B6EA3FBA580E4D75D79BB66DB1FC6F745F76D797B656AE5B2B545D76D79FB6A523FC060E0930D6A3B60FC45B55360D0BD6ABB676ECFC63265300D6A7B640BDBA0E666376D7AFB6D3745B201C6776D783B6440CFC497F5476D78BB6D1972052204507D63BB636D45B16007F79D737B60102FC20406810D63FB690F5FC372C3F39D613B63F9EBA69165376D71BB6A5C4471A7F0776D717B63FD35B475A3178D71FB6AF55FC5C1D0979D723B600055B16387E79D72BB6B8C75B770D680BD627B628882012325708D62FB62B91BA1A2E5876D703B649FF424B475A0ED6F5B63A8B5B71567579D7FDB64C9EF3402A211ED6D1B60570BA1B730776D7D5B6B1B8FC7E077B0CD6DDB6EA8BFC0A6A0479D7E1B6180F2018594925D6E9B68A00F43E61710DD6E5B6BFAC422B015B0ED6EDB6655FFC7A0F0479D7C1B6A1C7204A7A082CD6C9B67E2642235A5C20D6C5B6619FFC784A015DD7CDB68ABFFC353A520CD671B6ACD5202A1E3900D679B6D9195B6A505601D67DB6115A5B503E7D34D651B6BFEB20506F0D02D659B65F555B5A030479D75DB64C4142196F5E07D661B6769EF440346810D665B6EBB1FC66317D79D76DB6D0CF425E5A5907D641B6963C5B46687F79D749B6D9AE274773150AD64DB6F5D7FC647C4B7FD7B1B62A6A5E2F076876D7B9B66F4942247D431ED6B5B60574FC747C5D1FD6BDB63AD6205A4F3078D791B6263C20482F6078D799B661D6206619583CD69DB6A128FC5E206277D7A1B6655AFC784A015DD7A9B66DC7BA2C450D76D7A5B626FFFC7A1B0479D781B6B8B8BA7A6C0876D789B60C9AFC126D2F2FD685B69E2AFC68657A25D63DB6AF525B0E666376D711B65A3C2018594925D619B619822076197F79D715B6FF6E202A540876D729B6CE284216617E79D725B6FF525B1A3A5876D701B605C3BA261B7D76D709B64EAFFC3F423B7FD705B6DC6A5B417C5F20D60DB6F5825B1A600876D7FEB67346FC67617F79D7D2B6DB2642665D7F79D7D6B6D655FC006B407FD7DEB65F1AFC77497479D7E2B64486FC5C3C6777D7EEB6C511FC41497479D7C2B62A315B785A2E76D7CAB6D696FC72777C35D6CEB6797220743E3242D672B6AE8ABA3C7E6D77D77AB63ABFFC72280135D676B631E0201D276A79D77EB6AAAEFC0635777BD752B6FF49202A760479D75AB69C5620411F6777D756B6C3B120145D7879D762B670B8FC52522D7ED766B6A18B426F745F76D76EB635A2FC666F327ED742B6F52320665D7F79D74AB67E46FC0A6A0479D746B6186F206856025DD7B3BE1C4AF4284B0376D7BBBE864CFC13147F79D7B7BEEB72FC3C2A3B7FD79BBE52965B264D777BD7ABBE9A265B6A7F5F79D7A7BE4144FC466D7E79D7AFBEA21F5B3C2A3B7FD783BEAF795B2C400C76D78BBE11ACBA291B5276D787BEDA2FFC4607327ED78FBE1065BA66317D79D733BE5474FC74767879D713BECEFE2040423B7FD717BE4A005B7C110979D70FBEBD4420092C6C05D7F1BED397FC7C110979D7F5BE9A86FC3A1B0479D7D1BE00735B0E53407FD7D9BE19C32074767879D7D5BEDCDC20116E7579D7DDBEFED1FC76107F79D7E1BE72605B7C110979D7EDBE3CFDFC3F4E3B7FD7C1BEA2F2BA0A6A0479D7C9BE3AF65B35517779D7CDBE8A5B6E354278D779BE36EA2066317D79D775BE2F02427C711876D77DBE900E5B635E7678D751BE96DA42367B327ED75DBEE8375B76626706D769BEE3405B367B327ED745BE7676206C17367FD7B5BE749A203D201F07D791BEF2A7D121926797426B79147DD6E292CD1A2027223D78D6E6924CD1F4707E147DD6C692C367FC36203D78D66A92FECDF40D160800D63B8449DF424A290800D6378411CF424A290800D6138405D72003056D00D61B845328427F070400D617849F1FF439230800D61F84C4C52002007D00D6238405FDFC73467700D6745E912C8273778D701230E730E7B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2F81E70A7FA42F9E04D81A02581332041812971607E8146D1E99C54F28C065E912C8273D80E314A115148914891B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2F81E6F8D683CD9E4F9E04D81202581AB71804181D6607E81ADD1EE9C54F28C065E912C8273B90DF81951A111C01DFCCC2DB2B2B2B2B2B2B2B2B2B2B2B2B2F81E53A765FD6B4D81A02581D120419B51807E9B51E99454F28C065E912C827314D2E430FFB08A01AAF87AB2B2B2B2B2B2B2B2B2B2B2B2B2B2B2F81E53A765FD6B4D81202581BCC0419B51607E9B51E99454F28C065E912C8273D2B7E441A9F80139BF889AB80A4920B2B2B2B2B2B2B2B2B2B2B2F81E67C8825CC5F9184D812025811A71C04181D6607E81B9E99C54F28C065E912C827331DEEA5081701230E7B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2F81E6318411B6EFB6B4D81F0258130984181B0207E9B51E9C054F28C065E912C827332B3EA399930A83999B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2B2F81E53A765FD6B4D81F025817190419B51E07E9B51E99454F28C063330ECBFC516FF2BFEC86F56225E49541F7C22A4C4055E22635EAEAEAEAEAEAEFC926FA81768606787D5ADFD0C65756787D5701E450C65756787D5211275136F6787D5D9ED4575136F6787D5999578F0F36787D5F08678F0F36787D5A05E4578F0F36787D567DC8B6C09116787D57E428BE9168C6787D5CA8D8BE9168C6787D571D64577E8996787D5F105B57303BB7787D54609170C86687787D50D8363616C7787D5251E456AB3667787D5E361456A746D7787D59684BBF4F07787D5BF718BBBF4F07787D5CF8145BBF4F07787D52143451008FC7787D553258BBD77F67787D55A128A6E0B6F87D508658A6E0B6F87D5F1928B8A6E0B6F87D589548B8A6E0B6F87D55C708B8A6E0B6F87D5D1518B8A6E0B6F87D5EDC48B8A6E0B6F87D5938F8B8A6E0B6F87D594DD8B8A6E0B6F87D5B0AC8B8A6E0B6F87D559D9B58A6E0B6F87D59996B58A6E0B6F87D56FFEB58A6E0B6F87D594CB638A6E0B6F87D56743458A6E0B6F87D53D5E458A6E0B6F87D53837458A6E0B6F87D59753178A6E0B6F87D5FC08178A6E0B6F87D5AD08178A6E0B6F87D55A56178A6E0B6F87D5A3EFA88A6E0B6F87D504EFA88A6E0B6F87D56159A88A6E0B6F87D59D02188A6E0B6F87D5EE8F8B17E5916F87D582926D17E5916F87D555926D17E5916F87D5466C8BE97C617F87D56C856B6009E087D5AD656B6009E087D5EDCA8B6B6009E087D55681636B6009E087D56C59A86B6009E087D504587C7B61E887D568738B7C7B61E887D5AE25177C7B61E887D50625177C7B61E887D5C733668A01E887D582628B668A01E887D5BD1E8B668A01E887D5484F6619E4F887D5320D8BE4640AF887D556C545E4640AF887D593A24574EB1FF887D552E5FE0D6BF487D599258BFE0D6BF487D582CB63FE0D6BF487D5DD5D45FE0D6BF487D592BF45FE0D6BF487D578C445FE0D6BF487D5561145FE0D6BF487D55CD1A8FE0D6BF487D5AD9A18FE0D6BF487D5B1C718FE0D6BF487D5C9B915EB0AF487D59D92180FBB9CF487D5CA4FF1070AEC87D52FD3B5BF9E7EF287D592C545850110F287D55F4217850110F287D5B15A18850110F287D543CEBDF698FA87D5E633BDF698FA87D5B033BDF698FA87D5670A6D1D6C99FA87D53D0A6D1D6C99FA87D57C3FF08E69E687D582EC4576FC88E687D5E2BC81F685E687D5A6CA8B81F685E687D58EA78B9975BFF687D56E518B9975BFF687D546688B859C7AF687D55F43451BF66DF687D5CC81FAAF855D906DEB1EF8A28CA3DA863D5D7D73F028E7E957DB8C4F0CF36DF21654D47BB1EF4867AA441423D75BCBA4236DFF40AA280335CC12D5B213FC5AD622D529F05B035498989898A12231972A18E596E22E78AA4DB879B271FFAC0EEC863A1464EDC40F1B8B156A5500C047A783BE9F94D9DFBB779F1901";
	//const unsigned char* Msg28346 = EHexToByte(Byte.data());
	//SendToClient(Client, 28346, (const BYTE*)Msg28346, Byte.length() / 2, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	//if (Msg28346)
	//	delete[] Msg28346;
	//return;


	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	/*if (!buf)
	{
		return;
	}
	//BYTE* p = buf;*/
	size_t len;
	
	
	int result;
	USHORT ResultID = 0;
	UINT NobleID = 0;
	const char* Reason = "";
	int Identity = ID_IDENTIFY_QQLEVEL1 | ID_IDENTIFY_QQLEVEL2 | ID_IDENTIFY_QQLEVEL3 | ID_IDENTIFY_QQFLAG | ID_IDENTIFY_HAVEWORD | ID_IDENTIFY_FINISHQQFLAG;
	int IsInTopList = 0;
	std::string LoginTime = "";
	char CurLoginTm[27] = { 0 };
	char NewDay = 0;
	if (!Client->IsLogin)
	{
		ResultID = 1;
		Reason = "the password is incorrect";
	}
	else
	{
		std::vector<PlayerInfo> playerInfo;
		PlayerDB_SelectPlayerInfo(Client, playerInfo);
		if (playerInfo.size() > 0) {
			ResultID = 0;
			if (playerInfo[0].VipFlag > 0) {
				Identity |= ID_IDENTIFY_SPEEDMEMBER;
			}
			LoginTime = playerInfo[0].LastLoginTime;
			
			Client->NobleID = playerInfo[0].NobleID;
			Client->NobleLevel = playerInfo[0].NobleLevel;
			Client->NoblePoint = playerInfo[0].NoblePoint;
			Client->GuildVipLevel = playerInfo[0].GuildVipLevel;
			Client->GuildVipPoint = playerInfo[0].GuildVipPoint;
			Client->LoverVipLevel = playerInfo[0].LoverVipLevel;
			Client->LoverVipPoint = playerInfo[0].LoverVipPoint;
			Client->EmperorLevel = playerInfo[0].EmperorLevel;
			Client->EmperorPoint = playerInfo[0].EmperorPoint;
			Client->GardenLevel = playerInfo[0].GardenLevel;
			Client->GardenPoint = playerInfo[0].GardenPoint;
			convertTimeStamp2TimeStr(time(0), CurLoginTm);
			if (!LoginTime.empty())
			{
				if (strcmp(LoginTime.c_str(), CurLoginTm) != 0)
				{
					NewDay = 1;
				}
			}
			if (NewDay)
			{
				if (Client->NobleLevel >= 1)//皇族成长
				{
					Client->NoblePoint += 10;
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
				}
				if (Client->GardenLevel >= 1)//后花园成长
				{
					Client->GardenPoint += 10;
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
				}
				if (Client->GuildVipLevel >= 1)//车队贵族成长
				{
					Client->GuildVipPoint += 10;
				}
				if (Client->LoverVipLevel >= 1)//情侣成长
				{
					Client->LoverVipPoint += 10;
				}
				if (Client->EmperorLevel >= 1)//钻皇成长
				{
					Client->EmperorPoint += 10;
				}

				PlayerDB_UpdatePlayerInfo(Client, CurLoginTm);
			}
		}
		else {
			ResultID = 1;
		}

		int TopListUin = Client->Uin;
		IsInTopList = IsUserInTopList(TopListUin);

		if (HonorTopList[0] == TopListUin)
		{
			Identity |= ID_IDENTIFY_KING;
		}
		else if (HonorTopList[1] == TopListUin || HonorTopList[2] == TopListUin)
		{
			Identity |= ID_IDENTIFY_INFANTE;
		}

	}

	Write16(p, ResultID); //ResultID 0为成功标志

	Write8(p, sizeof(Client->Key)); //KeyGameDataLength 密钥长度
	memcpy(p, Client->Key, sizeof(Client->Key));//对称加密密钥
	p += sizeof(Client->Key);

	Write32(p, Client->Uin);//用户登录id
	Write32(p, Identity);
	Write16(p, Client->ConnID); //用户连接id
	Write32(p, LoginType); //LoginType
	Write32(p, 4); //GameSvrdIdentity游戏服务标识

	WritePlayerDBBaseInfo(p, Client->Uin, Client);//发送基础信息

	//Write16(p, 0); //ItemNum 发送道具信息
	Write16(p, ItemNum); // ItemNum
	{
		for (size_t i = 0; i < ItemNum; i++)
		{
			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); // len

			Write32(pItemInfo, Items[i].ItemID);

			Write32(pItemInfo, Items[i].ItemNum);
			Write32(pItemInfo, Items[i].AvailPeriod);
			Write8(pItemInfo, Items[i].Status);
			Write32(pItemInfo, Items[i].ObtainTime);
			Write32(pItemInfo, Items[i].OtherAttribute);
#ifndef ZingSpeed
			Write16(pItemInfo, Items[i].ItemType); // ItemType
#endif

			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}

	Write16(p, 0); //TaskNum 任务信息
	/*
m_astPlayerDBTaskInfo[].m_iTaskID
m_astPlayerDBTaskInfo[].m_shOptionID
m_astPlayerDBTaskInfo[].m_cStatus
m_astPlayerDBTaskInfo[].m_bPrevStatus
m_astPlayerDBTaskInfo[].m_iProgressRate
m_astPlayerDBTaskInfo[].m_iOther
m_astPlayerDBTaskInfo[].m_uiTakeTime
m_astPlayerDBTaskInfo[].m_uiLastUpdateTime
	*/

	Write8(p, 0); //RecomCountry
	Write32(p, (UINT)time(nullptr)); //ServerTime 服务器时间

	len = strlen(Reason);//原因
	Write8(p, (BYTE)len); //ReasonLen
	memcpy(p, Reason, len);//原因数据
	p += len;

	{//PlayerRelationInfo玩家关系信息
		std::string newname = "";
		PlayerDB_SelectEngageInfo(Client);
		if (Client->EngageUin != 0) {
			PlayerDB_SelectNickName(Client->EngageUin, newname);
		}

		BYTE* pPlayerRelationInfo = p;
		Write16(pPlayerRelationInfo, 0); //len
		Write32(pPlayerRelationInfo, Client->Uin); //SrcUin
		if (Client->EngageUin != 0)
		{
			Write32(pPlayerRelationInfo, 0x3); //RelationFlag
		}
		else
		{
			Write32(pPlayerRelationInfo, 0); //RelationFlag
		}

		Write32(pPlayerRelationInfo, Client->EngageUin); //RelationUin

		Write8(pPlayerRelationInfo, 17);//NickNameLen
		memset(pPlayerRelationInfo, 0, MaxNickName);
		//RelationNickName[]
		if (!newname.empty())
		{
			memcpy(pPlayerRelationInfo, newname.c_str(), newname.length());
		}

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


	BYTE* pMapNum = p;
	Write16(p, 0); //MapNum 地图数据
	{
		std::vector<MapInfo> mapInfo;
		PlayerDB_SelectMapInfo(Client->Uin, mapInfo);
		for (size_t i = 0; i < mapInfo.size(); i++)
		{
			BYTE* pRecord = p;
			Write16(pRecord, 0); // len

			Write32(pRecord, mapInfo[i].MapID); // MapID
			Write32(pRecord, mapInfo[i].Record); // Record

			len = pRecord - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Set16(pMapNum, (WORD)mapInfo.size());
	}

	{//宠物信息
		int totalpet = 0;
		BYTE* pTotalPet = p;
		Write16(p, 0); //PetNum ResponseLogin

		std::vector<PetInfo> petInfo;
		PlayerDB_SelectPetInfo(Client->Uin, petInfo);

		for (size_t i = 0; i < petInfo.size(); i++)
		{
			UINT Petid = petInfo[i].ID;
			PETITEM* PetItem = GetPetItem(Petid);
			if (!PetItem)
			{
				continue;//总表没记录就不写
			}
			totalpet++;
			if (totalpet == 300) {
				totalpet--;
				break;
			}

			BYTE* pPetInfo = p;
			Write16(pPetInfo, 0); // len

			Write32(pPetInfo, Client->Uin);					// Uin
			Write32(pPetInfo, Petid); // PetId
			//memcpy(pPetInfo, myutf8trans(strdup(res->getString("PetName").c_str())), 23);
			memcpy(pPetInfo, PetItem->PetName, 23);
			pPetInfo += 23;
			Write8(pPetInfo, 1); // IsProper
			Write16(pPetInfo, 30);// LevelLimit
			Write16(pPetInfo, 30);// Level
			Write32(pPetInfo, 238966); // CurLevelExp
			Write32(pPetInfo, GetPetMaxExp(30));// CurLevelMaxExp
			Write8(pPetInfo, petInfo[i].Status);// Status
			Write16(pPetInfo, 100); // PL
			Write16(pPetInfo, 100);// TotalPL

			Write16(pPetInfo, 3); // PetSkillNum.
			{
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); // len
				Write16(pPetSkillList, PetItem->SkillID1);		   // SkillID
				Write8(pPetSkillList, 1);// Status
				Write8(pPetSkillList, 1);// Active
				Write32(pPetSkillList, PetItem->SkillValue1); // Value

				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			{
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); // len
				Write16(pPetSkillList, PetItem->SkillID2);		   // SkillID
				Write8(pPetSkillList, 2);// Status
				Write8(pPetSkillList, 1);// Active
				Write32(pPetSkillList, PetItem->SkillValue2); // Value

				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			{
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); // len
				Write16(pPetSkillList, PetItem->SkillID3);		   // SkillID
				Write8(pPetSkillList, 3);// Status
				Write8(pPetSkillList, 1);// Active
				Write32(pPetSkillList, PetItem->SkillValue3); // Value

				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			Write32(pPetInfo, petInfo[i].StrengLevel); // StrengLevel
			Write8(pPetInfo, 4);   // TopStatus
			{					   // CommonInfo
				BYTE* pCommonInfo = pPetInfo;
				Write16(pCommonInfo, 0); // len

				Write32(pCommonInfo, 0); // HuanHuaLevel
				Write8(pCommonInfo, 0);	 // CanHuanHua

				len = pCommonInfo - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}

			len = pPetInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}

		//最多传输300个宠物
		Set16(pTotalPet, totalpet);
	}

	Write8(p, IsInTopList); //IsInTopList
	Write32(p, 0); //LastLoginTime
	Write32(p, 0); //MasterPoint
	Write8(p, 0); //LoginRealNameFlag

	//ExRightFlag[]
#ifndef ZingSpeed
	if (ClientVersion > 18457)
	{
		memset(p, 0, 24);
		p += 24;
	}
	else
#endif
	{
		memset(p, 0, 16);
		p += 16;
	}

	std::vector<ItemInfo> vecItems;
	PlayerDB_LoginSelectOverItems(Client->Uin, vecItems);
	Write8(p, vecItems.size()); //OverdueItemNum过期道具

	for (size_t i = 0; i < vecItems.size(); i++)
	{ //OverdueItem
		BYTE* pOverdueItem = p;
		Write16(pOverdueItem, 0); //len

		Write32(pOverdueItem, vecItems[i].ItemID); //ItemID
		Write32(pOverdueItem, vecItems[i].AvailPeriod * 60 * 60 + vecItems[i].ObtainTime); //OverdueTime
		Write32(pOverdueItem, 0); //OtherAttr

		len = pOverdueItem - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write32(p, 0); //StoneKartNum赛车宝石数据


	Write32(p, 0); //LockedMapID
	Write32(p, 0); //TotalGuildProsperity
	Write32(p, 0); //ClientStatusContex
	Write8(p, 0); //IsHoneyMoon 
	Write8(p, HasMoreInfo); //HasMoreInfo 更多信息 通知客户端我还有道具信息
	Write16(p, 0); //CurSvrID
	//个人信息
	{ //NobleInfo 贵族信息
		BYTE* pNobleInfo = p;
		Write16(pNobleInfo, 0); //len

		Write32(pNobleInfo, Client->NobleID); //NobleID
		Write8(pNobleInfo, Client->NobleLevel); //NobleLevel
		Write32(pNobleInfo, Client->NoblePoint); //NoblePoint
		Write32(pNobleInfo, -1); //NobleLeftDays

		len = pNobleInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 20000); //ExtendItemNum扩展道具数量
	Write8(p, 60); //UpdateOnlineInfoInterval更新在线信息间隔

#ifndef ZingSpeed
	{ //GuildVipBaseInfo GuildVip信息车队贵族
		BYTE* pGuildVipBaseInfo = p;
		Write16(pGuildVipBaseInfo, 0); //len

		Write8(pGuildVipBaseInfo, Client->GuildVipLevel); //GuildVipLevel
		Write32(pGuildVipBaseInfo, Client->GuildVipPoint); //GuildVipPoint


		len = pGuildVipBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //GuildVipOtherInfo
		BYTE* pGuildVipOtherInfo = p;
		Write16(pGuildVipOtherInfo, 0); //len

		Write32(pGuildVipOtherInfo, -1); //GuildVipLeftDays
		Write8(pGuildVipOtherInfo, 0); //CanReceiveGift

		len = pGuildVipOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
#endif
	Write8(p, 0); //HasLDMInfo
	/*
m_astPlayerLDMBaseInfo[%d].m_u8Grade
m_astPlayerLDMBaseInfo[%d].m_u32Score
m_astPlayerLDMBaseInfo[%d].m_u32MaxScore
m_astPlayerLDMBaseInfo[%d].m_u32MaxGrade
	*/

	Write32(p, 0); //ForbiddenModeFreeTime禁止模式空闲时间
	Write32(p, 0); //ForbiddenModeBitSet

#ifndef ZingSpeed
	{ //LoverVipBaseInfo 情侣贵族信息
		BYTE* pLoverVipBaseInfo = p;
		Write16(pLoverVipBaseInfo, 0); //len

		Write8(pLoverVipBaseInfo, Client->LoverVipLevel); //LoverVipLevel
		Write32(pLoverVipBaseInfo, Client->LoverVipPoint); //LoverVipPoint
		Write8(pLoverVipBaseInfo, 10); //GrowRate

		len = pLoverVipBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //LoverVipOtherInfo
		BYTE* pLoverVipOtherInfo = p;
		Write16(pLoverVipOtherInfo, 0); //len

		Write32(pLoverVipOtherInfo, -1); //LoverVipLeftDays
		Write8(pLoverVipOtherInfo, 0); //CanReceiveGift
		Write8(pLoverVipOtherInfo, 0); //ShowExpireTips

		len = pLoverVipOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
#endif
	Write8(p, 0); //SkateTaskLevel 轮滑任务级别
	Write32(p, 0); //SkateCoin
	Write8(p, 0); //SkateExpSkillLevel
	Write16(p, 0); //SkateCoinCountDaily
	Write32(p, 0); //SkateCoinHistoryTotal
	Write32(p, 0); //MaxDayPveScore
	Write32(p, 0); //MaxHistoryPveScore

#ifndef ZingSpeed
	Write32(p, 0); //LoveValue

	Write8(p, 0); //HasCheerAddition 欢呼值添加
	/*
m_astCheerAdditon[%d].m_ushExpAddition
m_astCheerAdditon[%d].m_ushWageAddition
	*/

	{ //PersonalGardenBaseInfo个人花园基本信息
		BYTE* pPersonalGardenBaseInfo = p;
		Write16(pPersonalGardenBaseInfo, 0); //len

		Write8(pPersonalGardenBaseInfo, Client->GardenLevel); //GardenLevel
		Write32(pPersonalGardenBaseInfo, Client->GardenPoint); //GardenPoint
		Write8(pPersonalGardenBaseInfo, 10); //GrowRate
		Write32(pPersonalGardenBaseInfo, -1); //GardenExpireTime
		{ //SimpleInfo
			BYTE* pSimpleInfo = pPersonalGardenBaseInfo;
			Write16(pSimpleInfo, 0); //len

			Write32(pSimpleInfo, 0); //WeekPopularity
			Write32(pSimpleInfo, 0); //TotalPopularity
			Write32(pSimpleInfo, 0); //LastUpdatePopularityTime
			Write8(pSimpleInfo, 0); //PrivateType

			len = pSimpleInfo - pPersonalGardenBaseInfo;
			Set16(pPersonalGardenBaseInfo, (WORD)len);
			pPersonalGardenBaseInfo += len;
		}

		len = pPersonalGardenBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //ConsumeVipInfo消费Vip
		BYTE* pConsumeVipInfo = p;
		Write16(pConsumeVipInfo, 0); //len

		Write32(pConsumeVipInfo, Uinew > 0 ? 0 : 9); //VipLevel
		Write32(pConsumeVipInfo, 0); //CharmValueOfMonth
		Write32(pConsumeVipInfo, 0); //SearchTreasureNums
		Write32(pConsumeVipInfo, 0); //GetTreasureNums

		len = pConsumeVipInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //EmperorInfo钻皇信息
		BYTE* pEmperorInfo = p;
		Write16(pEmperorInfo, 0); //len

		Write8(pEmperorInfo, Client->EmperorLevel); //EmperorLevel
		Write32(pEmperorInfo, Client->EmperorPoint); //EmperorPoint
		Write32(pEmperorInfo, -1); //EmperorLeftDays
		Write8(pEmperorInfo, 10); //EmperorGrowRate

		len = pEmperorInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //EmperorOtherInfo
		BYTE* pEmperorOtherInfo = p;
		Write16(pEmperorOtherInfo, 0); //len

		Write32(pEmperorOtherInfo, 0); //ExpiredTime
		Write8(pEmperorOtherInfo, 0); //ShowExpireTips

		len = pEmperorOtherInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //ShuttleScoreWeek


	{ //ActivityInfo活动信息
		BYTE* pActivityInfo = p;
		Write16(pActivityInfo, 0); //len

		Write32(pActivityInfo, 0); //TotalActivity
		Write32(pActivityInfo, 0); //ActivityLevel

		len = pActivityInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	{ //GansterScoreInfo
		BYTE* pGansterScoreInfo = p;
		Write16(pGansterScoreInfo, 0); //len

		Write32(pGansterScoreInfo, 0); //GansterSeasonID
		Write32(pGansterScoreInfo, 0); //GansterScore
		Write32(pGansterScoreInfo, 0); //PoliceScore
		Write32(pGansterScoreInfo, 0); //TotalGansterScore

		len = pGansterScoreInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //OlympicId
	Write32(p, 0); //NPCEliminateWinTimes

	{ //BorderInfo
		BYTE* pBorderInfo = p;
		Write16(pBorderInfo, 0); //len

		Write32(pBorderInfo, 0x2b); //SeasonID
		Write32(pBorderInfo, 0x3); //Zhanxun
		Write32(pBorderInfo, 0x3); //SeasonZhanxun

		len = pBorderInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //ReduceReturnRate
	Write32(p, 0); //ReduceReturnAvailPeriod
	Write8(p, 0); //SpecialActivityStatus
	Write32(p, 0); //ThemeHouseDressDegreeValue

	Write32(p, 1); //3DRoomEnabled
	Write8(p, 1); //3DRoomEnabled_OB
	Write8(p, 0); //BuyZizuanPopupOpen

	Write32(p, 0); //BuyZizuanPopupLimit
	Write8(p, 1); //EnableReconnectOpt

	//查询排位信息
	PlayerDB_SelectRankInfo(Client);

	Write8(p, 1); //HasRankedMatchInfo
	{ //RankedMatchGradeInfo
		BYTE* pRankedMatchGradeInfo = p;
		Write16(pRankedMatchGradeInfo, 0); //len

		Write32(pRankedMatchGradeInfo, RankGradeLevel[Client->GradeLevel]); //GradeLevel 排位等级图标
		Write32(pRankedMatchGradeInfo, Client->ChildLevel); //ChildLevel
		Write32(pRankedMatchGradeInfo, 0); //GradeScore
		Write32(pRankedMatchGradeInfo, 0); //MaxGradeScore
		Write32(pRankedMatchGradeInfo, 0); //TotalScore

		len = pRankedMatchGradeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //HasHuanLingChangeInfo
	/*
m_astHuanLingSuitInfo[%d].m_iSuitID
m_astHuanLingSuitInfo[%d].m_ucChangeLevel
	*/

	Write8(p, 0); //EquipSealType
	Write8(p, 0); //hCreditStarFlag
	Write8(p, 0); //PersonalPanelSelectRankedMatchFrameTag
	Write8(p, 0); //VersionURLLen
	Write8(p, 0); //PrivacyURLLen
	Write8(p, 0); //PersonalRankedMatchLevelShowTag
	Write16(p, 0); //LeftDeletingRoleDays
	Write8(p, 0); //OnlyRspEquippedItem
	Write32(p, 0); //LoginSwitchFlag1
	Write16(p, 0); //LeftUploadPLogNum

#endif

	len = p - buf.data();
	int MsgID = GetServerType(Client->ServerID);
	if (MsgID == Relax)
	{
		MsgID = 98;
		CheckDupUser(Client->Uin, Client);
		RelaxClients[Client->Uin] = Client;
	}
	else if (MsgID == Dance)
	{
		MsgID = 95;
		CheckDupUser(Client->Uin, Client);
		Clients[Client->Uin] = Client;
	}
	else
	{
		//顶号
		MsgID = 100;//回复RequestLogin消息id
		CheckDupUser(Client->Uin, Client);
		Clients[Client->Uin] = Client;
	}

	//printf("ServerID:%d\n", Client->ServerID);
	SendToClient(Client, MsgID, buf.data(), len, Client->ConnID, FE_GAMESVRD, Client->ConnID, Response);

	//delete[] buf;
}
void NotifyLoginMoreInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items, bool HasNext)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;


	Write16(p, ItemNum); //ItemNum
	{
		for (size_t i = 0; i < ItemNum; i++)
		{
			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, Items[i].ItemID);
			Write32(pItemInfo, Items[i].ItemNum);
			Write32(pItemInfo, Items[i].AvailPeriod);
			Write8(pItemInfo, Items[i].Status);
			Write32(pItemInfo, Items[i].ObtainTime);
			Write32(pItemInfo, Items[i].OtherAttribute);
#ifndef ZingSpeed
			Write16(pItemInfo, Items[i].ItemType); //ItemType
#endif

			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}

	Write16(p, 0); //TaskNum
	/*
m_astPlayerDBTaskInfo[].m_iTaskID
m_astPlayerDBTaskInfo[].m_shOptionID
m_astPlayerDBTaskInfo[].m_cStatus
m_astPlayerDBTaskInfo[].m_bPrevStatus
m_astPlayerDBTaskInfo[].m_iProgressRate
m_astPlayerDBTaskInfo[].m_iOther
m_astPlayerDBTaskInfo[].m_uiTakeTime
m_astPlayerDBTaskInfo[].m_uiLastUpdateTime
	*/

	Write16(p, 0);//PetNum





	Write16(p, 0); //MapNum
	/*
m_astRecord[].m_iMapID
m_astRecord[].m_iRecord
m_astRecord[].m_iLastUpdateTime
	*/


	Write16(p, 0); //OverdueItemNum
	/*
m_astOverdueItem[].m_iItemID
m_astOverdueItem[].m_iOverdueTime
m_astOverdueItem[].m_iOtherAttr
	*/

	Write16(p, 0); //StoneKartNum 赛车宝石数量
	/*
m_astKartStoneGrooveInfo[].m_iKartID
m_astKartStoneGrooveInfo[].m_iStoneGrooveNum
m_astKartStoneGrooveInfo[].m_astStoneGrooveInfo[].m_iStoneUseOccaType
m_astKartStoneGrooveInfo[].m_astStoneGrooveInfo[].m_iSkillStoneID
	*/

	Write8(p, HasNext);


	len = p - buf.data();
	SendToClient(Client, 223, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void RequestLogout(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	UINT LogOutFlag = Read32(Body);
	int ServerKickReason = Read32(Body);

	UserDB_UpdateConnID(Client->Uin);
	ResponseLogout(Client);
	Client->IsLogin = FALSE;
	Client->Server->Disconnect(Client->ConnID);
	Client->deleteflag = 1;
}
void ResponseLogout(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 101, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestHello(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	if (notice == 1) {
		if (bignotice.length() != 0) {
			NotifyBugletTalk(SystemClient, bignotice.c_str(), 200, 12);  //我是全服大喇叭固定公告
		}
		if (smallnotice.length() != 0) {
			NotifyBugletTalk(SystemClient, smallnotice.c_str(), 200, 200);  //我是固定小公告			
		}
	}
	ResponseHello(Client, Uin, Time);
}
void ResponseHello(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write32(p, Time); //Time
	Write32(p, Uin); //Uin
	Write32(p, (DWORD)time(nullptr)); //ServerTime
	Write32(p, static_cast<DWORD>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() % 1000000)); //ServerMicroSecond
	//Write32(p, 0); //ServerMicroSecond
	Write16(p, 0);

	len = p - buf.data();

	SendToClient(Client, 112, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}


void RequestChangeGender(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	BYTE NewGender = Read8(Body);
	std::vector<ItemStatus> GenderItem(2);
	
	
	int result;


	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET Gender=?  WHERE Uin=? ;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, NewGender);
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

		char HasResult = 1;
		if (NewGender == 0)//男切女
		{
			GenderItem[0].ItemID = 10022;
			GenderItem[0].NewStatus = 0;
			GenderItem[1].ItemID = 10018;
			GenderItem[1].NewStatus = 1;

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Select Status from Item Where Uin=? and ItemID=10022;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			if (res->rowsCount() == 0) {
				HasResult = 0;
			}
			res.reset();

			if (!HasResult)
			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setUInt(2, 10022);
				prep_stmt->setUInt(3, 1);
				prep_stmt->setInt(4, -1);
				prep_stmt->setUInt(5, 0);
				prep_stmt->setUInt(6, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else
			{

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET Status=0  WHERE Uin=? and ItemID = 10022;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}

			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET Status=1  WHERE Uin=? and ItemID = 10018;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}

		}
		else {
			GenderItem[0].ItemID = 10018;
			GenderItem[0].NewStatus = 0;
			GenderItem[1].ItemID = 10022;
			GenderItem[1].NewStatus = 1;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Select Status from Item Where Uin=? and ItemID=10018;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			if (res->rowsCount() == 0) {
				HasResult = 0;
			}
			res.reset();


			if (!HasResult)
			{

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setUInt(2, 10018);
				prep_stmt->setUInt(3, 1);
				prep_stmt->setInt(4, -1);
				prep_stmt->setUInt(5, 0);
				prep_stmt->setUInt(6, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else
			{

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET Status=0  WHERE Uin=? and ItemID = 10018;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}


			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET Status=1  WHERE Uin=? and ItemID = 10022;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Client->Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
		}

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("RequestChangeGender error\n");
	}

	ResponseChangeGender(Client, NewGender, 0);
	ResponseChangeItemStatus(Client, 2, GenderItem );
}
void ResponseChangeGender(std::shared_ptr<ClientNode> Client, BYTE NewGender, BYTE UseType)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID

	Write8(p, NewGender);
	Write8(p, 0); //ReasonLen
#ifndef ZingSpeed
	Write8(p, UseType);
#endif
	Client->Gender = NewGender;
	len = p - buf.data();
	SendToClient(Client, 162, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	getGameMode(Client);
}

int GettheMechSkinId(CONST UINT ShapeSuitID)
{
	switch (ShapeSuitID)
	{
	case 123513://悟空-齐天大圣	赛车皮肤
		return 123238;//二级形态
	case 126355://源极-大河神卫	赛车皮肤
		return 127437;//源极2级形态
	case 130539://奥莉娜·天穹战音	赛车皮肤
		return 130800;//奥莉娜3级形态 赛车皮肤
	case 133683://极-烟雨天青	赛车皮肤
		return 133686;//源极·青花3级形态
	case 133723://哈雷特-炽焰龙骑	赛车皮肤
		return 133726;//哈雷特3级皮肤
	case 138408://墨影青龙		墨影青龙-敖丙
		return 138411;//哈雷特3级皮肤
	default:
		break;
	}

	return 0;

}


void NotifyChangeItemStatus(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> MyClient, USHORT ItemNum, std::vector<ItemStatus> ItemStatus)
{
	//通知所有人变化装备 有皮肤直接更新皮肤
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, MyClient->Uin);

	Write16(p, ItemNum);
	for (size_t i = 0; i < ItemNum; i++)
	{
		BYTE* pItemStatus = p;
		Write16(pItemStatus, 0); //len

		Write32(pItemStatus, ItemStatus[i].ItemID);
		//Write32(pItemStatus, GettheMechSkinId(ItemStatus[i].ItemID) > 0 ? GettheMechSkinId(ItemStatus[i].ItemID) : ItemStatus[i].ItemID);
		Write32(pItemStatus, 1); //ItemNum
		Write32(pItemStatus, -1); //AvailPeriod
		Write8(pItemStatus, ItemStatus[i].NewStatus); //Status
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

		Write32(pKartRefitInfo, MyClient->Uin); //Uin
		Write32(pKartRefitInfo, MyClient->KartID); //KartId
		Write32(pKartRefitInfo, 0); //RefitCout
		Write16(pKartRefitInfo, 0); //MaxFlags
		Write16(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.WWeight); //WWeight
		Write16(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.SpeedWeight); //SpeedWeight
		Write16(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.JetWeight); //JetWeight
		Write16(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.SJetWeight); //SJetWeight
		Write16(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.AccuWeight); //AccuWeight
		Write32(pKartRefitInfo, 0); //ShapeRefitCount
		Write32(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.KartTailRefitItemID); //KartTailRefitItemID
		Write32(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.KartTireRefitItemID); //KartTireRefitItemID
		Write32(pKartRefitInfo, 0); //SecondRefitCount
		Write16(pKartRefitInfo, 0); //Speed2Weight
		Write16(pKartRefitInfo, 0); //DriftVecWeight
		Write16(pKartRefitInfo, 0); //AdditionalZSpeedWeight
		Write16(pKartRefitInfo, 0); //AntiCollisionWeight
		Write16(pKartRefitInfo, 0); //LuckyValue
		Write16(pKartRefitInfo, 0); //RefitLuckyValueMaxWeight
		Write32(pKartRefitInfo, MyClient->KartSkinID); //ShapeSuitID
		Write8(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.LegendSuitLevel); //LegendSuitLevel
		Write32(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pKartRefitInfo, MyClient->ClientKartRefitItemInfo.ShapeLegendSuitID); //ShapeLegendSuitID

		len = pKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ChangeType

#ifndef ZingSpeed
	Write8(p, 0); //EquipNiChangItemNum

#endif



	len = p - buf.data();
	SendToClient(Client, 527, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Notify);
}


void RequestChangeItemStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//修改当前装备状态 赛车数据库status 1 为装备上 0为卸下
	//0 0 27 11 0 2b 37 b3 0 2 0 7 0 1 8d fd 1 0 7 0 1 f2 80 0 0 0 0 0 0 0 0 0 d0 25
	// 回复
	// // 0000+item数量如0002+长度0007+变更装备id如0001d117+状态01+长度0007+当前装备id如0002724+状态00+000000


	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);


	USHORT ItemNum = Read16(Body);

	if (ItemNum == 0)
	{
		return;
	}
	std::vector<ItemStatus> aItemStatus(ItemNum);
	for (size_t i = 0; i < ItemNum; i++)
	{
		BYTE* pItemStatus = Body;
		USHORT len = Read16(pItemStatus);

		aItemStatus[i].ItemID = Read32(pItemStatus);
		aItemStatus[i].NewStatus = Read8(pItemStatus);

		Body += len;
	}
	/*
m_ucSpecFlag
m_ucChangeType
m_usItemNum
m_stEquipedItem[]
m_iNewKartID
	*/

	

	// 特殊物品ID转换
	UINT actualKartID = 0; // 用于记录实际要装备的车辆ID
	UINT originalItemID = aItemStatus[0].ItemID; // 保存原始ItemID用于检查

	// 现有特殊处理
	if (95405 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 12720;
	}
	if (96462 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 34393;
	}

	// X机甲幻形映射
	const UINT relatedKartIDs[] = { 135182 ,152203 ,152201, 152199, 152200, 152202 }; // 关联赛车ID列表
	bool isSpecialProp = false; // 是否为特殊道具

	if (148400 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 152201;
		if (aItemStatus[0].NewStatus == 1)
		{
			actualKartID = 152201;
			isSpecialProp = true;
		}
	}
	if (148397 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 152199;
		if (aItemStatus[0].NewStatus == 1)
		{
			actualKartID = 152199;
			isSpecialProp = true;
		}
	}
	if (148398 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 152200;
		if (aItemStatus[0].NewStatus == 1)
		{
			actualKartID = 152200;
			isSpecialProp = true;
		}
	}
	if (148399 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 152202;
		if (aItemStatus[0].NewStatus == 1)
		{
			actualKartID = 152202;
			isSpecialProp = true;
		}
	}
	if (135163 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 135182;
		if (aItemStatus[0].NewStatus == 1)
		{
			actualKartID = 135182;
			isSpecialProp = true;
		}
	}
	if (148410 == aItemStatus[0].ItemID)
	{
		aItemStatus[0].ItemID = 152203;
		if (aItemStatus[0].NewStatus == 1)
		{
			actualKartID = 152203;
			isSpecialProp = true;
		}
	}

	int result;
	BOOLEAN First = true;

	//if (95405 == aItemStatus[0].ItemID)//机甲特别处理
	//{
	//	aItemStatus[0].ItemID = 12720;
	//}
	//if (96462 == aItemStatus[0].ItemID)
	//{
	//	aItemStatus[0].ItemID = 34393;
	//}

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	conn->setAutoCommit(false);

	for (size_t i2 = 0; i2 < ItemNum; i2++)
	{

		if (aItemStatus[i2].NewStatus && GetItemType(aItemStatus[i2].ItemID) == EAIT_CAR) {

			if (Client->ServerID == 900 && GetItemGameType(aItemStatus[i2].ItemID) != 1)
			{ // 滑板
				ResponseStartGame(Client, "请装备滑板");
				return;
			}
			else if (Client->ServerID == 1000 && GetItemGameType(aItemStatus[i2].ItemID) != 2)
			{ // 轮滑
				ResponseStartGame(Client, "请装备轮滑");
				return;
			}
			else
			{ // 其他
				// if (Client->ServerID != 116) {
				if (GetItemGameType(aItemStatus[i2].ItemID) == 1 && Client->ServerID != 900)
				{
					ResponseStartGame(Client, "普通模式无法装备滑板");
					return;
				}
				else if (GetItemGameType(aItemStatus[i2].ItemID) == 2 && Client->ServerID != 1000)
				{
					ResponseStartGame(Client, "普通模式无法装备轮滑");
					return;
				}
				//}

				Client->KartID = aItemStatus[i2].ItemID;

				if (First) {
					First = false;
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE item SET Status= 0 WHERE Uin = ? AND Status = 1 AND itemid IN(SELECT id FROM itemallnew WHERE type = 'EAIT_CAR');"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Client->Uin);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}

			}

		}
		else if (aItemStatus[i2].NewStatus && GetItemType(aItemStatus[i2].ItemID) == EAIT_PET)
		{
			Client->Pet->ID = aItemStatus[i2].ItemID;
			GetPetInfo(Client);
		}


		if (aItemStatus[i2].ItemID == 120258)//改装加成卡
		{
			if (aItemStatus[i2].NewStatus == 1)
			{
				Client->GlobalAddcard = 1;
			}
			else
			{
				Client->GlobalAddcard = 0;
			}
		}
		//else if (aItemStatus[i2].ItemID == 18554)//奶糖宝石加成卡
		//{
		//	if (aItemStatus[i2].NewStatus == 1)
		//	{
		//		Client->GlobalStone = 1;
		//		ResponseStartGame(Client, "奶糖佩戴成功，开启全局宝石，会覆盖原有宝石！");
		//	}
		//	else
		//	{
		//		Client->GlobalStone = 0;
		//	}
		//}
		else if (aItemStatus[i2].ItemID == 19756)//菠萝唐人机
		{
			if (aItemStatus[i2].NewStatus == 1)
			{
				Client->isManMachine = 1;
				if (Borderset[1] == 1) {
					ResponseStartGame(Client, "菠萝糖佩戴成功，可以边境排位匹配人机！");
				}

			}
			else
			{
				Client->isManMachine = 0;
			}
		}
		if (aItemStatus[i2].NewStatus == 1) {
			std::string str = findItemSkillStoneById(Client, aItemStatus[i2].ItemID);
			if (!str.empty())
			{
				ResponseStartGame(Client, str.c_str());
			}
		}


		try
		{
			size_t i = 0;
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET Status=? WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, aItemStatus[i2].NewStatus);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, aItemStatus[i2].ItemID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();

			if (aItemStatus[i2].NewStatus)
			{
				if (GetItemType(aItemStatus[i2].ItemID) == EAIT_CAR && GetItemGameType(aItemStatus[i2].ItemID) == 0)
				{

					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseGameTypeStatus SET KartID=? WHERE Uin=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, aItemStatus[i2].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (GetItemGameType(aItemStatus[i2].ItemID) == 1)
				{


					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseGameTypeStatus SET SlideID=? WHERE Uin=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, aItemStatus[i2].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (GetItemGameType(aItemStatus[i2].ItemID) == 2 && Client->Gender == 0)
				{

					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseGameTypeStatus SET SkateID0=? WHERE Uin=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, aItemStatus[i2].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (GetItemGameType(aItemStatus[i2].ItemID) == 2 && Client->Gender == 1)
				{

					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseGameTypeStatus SET SkateID1=? WHERE Uin=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, aItemStatus[i2].ItemID);
					prep_stmt->setUInt(2, Client->Uin);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
			}

		}
		catch (const std::exception& e)
		{
			printf(e.what());

			printf("RequestChangeItemStatus error\n");
		}


	}



	//查询是否是装备的宠物
	if (aItemStatus[0].NewStatus == 0)
	{
		PETITEM* PetItem = GetPetItem(aItemStatus[0].ItemID);
		//查到是装备的宠物
		if (PetItem != NULL)
		{
			Client->Pet->ID = 0;
		}
	}

	if (GetItemType(aItemStatus[0].ItemID) == EAIT_CAR)//如果是更换车辆
	{
		try
		{
			//PlayerDB_AddStoneCar(Client, Client->Uin, aItemStatus[0].ItemID, conn);
			const char* sql = NULL;
			sql = "SELECT WWeight,SpeedWeight,JetWeight,SJetWeight,AccuWeight,KartHeadRefitItemID,KartTailRefitItemID,KartFlankRefitItemID,KartTireRefitItemID,ShapeSuitID,LegendSuitLevelChoice,LegendSuitLevel,ShapeLegendSuitID,SkinSuitID FROM KartRefit Where Uin=? and KartID=?;";

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, aItemStatus[0].ItemID);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				Client->ClientKartRefitItemInfo.WWeight = res->getUInt("WWeight");
				Client->ClientKartRefitItemInfo.SpeedWeight = res->getUInt("SpeedWeight");
				Client->ClientKartRefitItemInfo.JetWeight = res->getUInt("JetWeight");
				Client->ClientKartRefitItemInfo.SJetWeight = res->getUInt("SJetWeight");
				Client->ClientKartRefitItemInfo.AccuWeight = res->getUInt("AccuWeight");
				Client->ClientKartRefitItemInfo.KartHeadRefitItemID = res->getUInt("KartHeadRefitItemID");
				Client->ClientKartRefitItemInfo.KartTailRefitItemID = res->getUInt("KartTailRefitItemID");
				Client->ClientKartRefitItemInfo.KartFlankRefitItemID = res->getUInt("KartFlankRefitItemID");
				Client->ClientKartRefitItemInfo.KartTireRefitItemID = res->getUInt("KartTireRefitItemID");
				Client->KartSkinID = Client->ClientKartRefitItemInfo.ShapeSuitID = res->getUInt("SkinSuitID") > 0 ? res->getUInt("SkinSuitID") : res->getUInt("ShapeSuitID");

				Client->ClientKartRefitItemInfo.LegendSuitLevelChoice = res->getUInt("LegendSuitLevelChoice");
				Client->ClientKartRefitItemInfo.LegendSuitLevel = res->getUInt("LegendSuitLevel");
				Client->ClientKartRefitItemInfo.ShapeLegendSuitID = res->getUInt("ShapeLegendSuitID");
				break;
			}
			res.reset();

		}
		catch (const std::exception& e)
		{
			printf(e.what());

			printf("RequestChangeItemStatus2 error\n");
		}
	}
	conn->commit();
	conn->setAutoCommit(true);
	ResponseChangeItemStatus(Client, ItemNum, aItemStatus);
	do
	{
		std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
		if (!Room)
		{
			break;
		}
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				if (RoomClient != Client)
				{
					NotifyChangeItemStatus(RoomClient, Client, ItemNum, aItemStatus);
				}
			}
		}
	} while (false);
}


void ResponseChangeItemStatus(std::shared_ptr<ClientNode> Client, USHORT ItemNum, std::vector<ItemStatus> ItemStatus)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;



	Write16(p, 0); //ResultID

	Write16(p, ItemNum);

	for (size_t i = 0; i < ItemNum; i++)
	{
		BYTE* pItemStatus = p;
		Write16(pItemStatus, 0); //len

		Write32(pItemStatus, ItemStatus[i].ItemID);
		Write8(pItemStatus, ItemStatus[i].NewStatus);

		len = pItemStatus - p;//长度 指针相减得到 2+4+1=7
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //ReasonLen

	Write8(p, 0); //SpecFlag
	Write8(p, 0); //ChangeType


	len = p - buf.data();
	//数据包Body分析 
	// 0000+item数量如0002+长度0007+变更装备id如0001d117+状态01
	//+长度0007+当前装备id如0002724+状态00+000000
	//printf("变更装备发送内容： \n");
	// printf("内容大小：%d\n", len);
	//for (int i = 0;i < len;i++)
	//{
	//	printf("%x ", *(buf+i));
	//}
	//printf("\n");
	SendToClient(Client, 130, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestChangeModel(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	BYTE NewModel = Read8(Body);
	//UCHAR UseType = Read8(Body);

	ResponseChangeModel(Client, NewModel);
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
			if (RoomClient != Client)
			{
				NotifyChangeModel(RoomClient, Client->Uin, NewModel);
			}
		}
	}
}
void ResponseChangeModel(std::shared_ptr<ClientNode> Client, BYTE NewModel)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write8(p, NewModel);
#ifndef ZingSpeed
	Write8(p, 0); //UseType
	Write8(p, 0); //TDCExcluItemNum
	//Write32(p, 0); //TDCExclusiveItemID[]
#endif

	len = p - buf.data();
	SendToClient(Client, 277, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void NotifyChangeModel(std::shared_ptr<ClientNode> Client, UINT Uin, BYTE NewModel)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Uin);
	Write8(p, NewModel);
#ifndef ZingSpeed
	Write8(p, 0); //TDCExcluItemNum
	//Write32(p, 0); //TDCExclusiveItemID[]
#endif

	len = p - buf.data();
	SendToClient(Client, 278, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientVipFlag(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0x0107); //VipFlag
	Write16(p, 0); //MsgLen
	Write16(p, 10); //VipGrowRate

	len = p - buf.data();
	SendToClient(Client, 948, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void RequestInlaySkillStone(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	int KartID = Read32(Body);
	int StoneGrooveID = Read32(Body);
	int SkillStoneID = Read32(Body);

	if (StoneGrooveID > 3 && StoneLimit) {
		if (GetItemMess(SkillStoneID) != "三角道具之钻") {
			ResponseStartGame(Client, "圆形竞速不允许覆盖三角宝石！");
			return;
		}
	}


	PlayerDB_UpdateKartStone(Client->Uin, KartID, StoneGrooveID, SkillStoneID);

	NotifyUpdateKartSkillStoneInfo(Client, KartID);

	NotifyUseItemResult2(Client, SkillStoneID, 1);

	ResponseInlaySkillStone(Client, KartID, StoneGrooveID, SkillStoneID);
}
void ResponseInlaySkillStone(std::shared_ptr<ClientNode> Client, int KartID, int StoneGrooveID, int SkillStoneID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write16(p, 0); //ReasonLen
	Write32(p, KartID);
	Write32(p, StoneGrooveID);
	Write32(p, SkillStoneID);

	len = p - buf.data();
	SendToClient(Client, 901, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyUpdateKartSkillStoneInfo(std::shared_ptr<ClientNode> Client, int KartID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	int ID = -1;
	std::vector<int> conditionValues;
	conditionValues.push_back(Client->Uin);
	conditionValues.push_back(KartID);
	std::vector<int> resultRow = executeQueryRow("SELECT 1  FROM kartstonenew  WHERE Uin=? AND KartID=?;", "NotifyUpdateKartSkillStoneInfo", conditionValues);
	if (resultRow.size() >= 1) {
		ID = resultRow[0];
	}
	if (ID == -1)
	{
		return;
	}

	Write32(p, Client->Uin); //Uin
	{ //KartStoneInfo
		BYTE* pKartStoneInfo = p;
		Write16(pKartStoneInfo, 0); //len

		Write32(pKartStoneInfo, KartID); //KartID

		BYTE* pStoneGrooveNum = pKartStoneInfo;
		Write32(pKartStoneInfo, 0); //StoneGrooveNum
		{
			int StoneGrooveNum = 0;
			int result;
			std::vector<KartStoneItemInfo> kartStoneItemInfo;
			PlayerDB_SelectKartStone(Client->Uin, KartID, kartStoneItemInfo);
			for (size_t i = 0; i < kartStoneItemInfo.size(); i++)
			{
				BYTE* pStoneGrooveInfo = pKartStoneInfo;
				Write16(pStoneGrooveInfo, 0); //len

				Write32(pStoneGrooveInfo, kartStoneItemInfo[i].StoneUseOccaType); //StoneUseOccaType
				Write32(pStoneGrooveInfo, kartStoneItemInfo[i].SkillStoneID); //SkillStoneID

				len = pStoneGrooveInfo - pKartStoneInfo;
				Set16(pKartStoneInfo, (WORD)len);
				pKartStoneInfo += len;
			}
			Set32(pStoneGrooveNum, kartStoneItemInfo.size());
		}
		len = pKartStoneInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 906, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyUpdateKartSkillStoneInfoNew(std::shared_ptr<ClientNode> Client, int KartID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	
	
	int result;
	int ID = -1;


	Write32(p, Client->Uin); //Uin
	{ //KartStoneInfo
		BYTE* pKartStoneInfo = p;
		Write16(pKartStoneInfo, 0); //len

		Write32(pKartStoneInfo, KartID); //KartID

		BYTE* pStoneGrooveNum = pKartStoneInfo;
		Write32(pKartStoneInfo, 0); //StoneGrooveNum
		{
			int StoneGrooveNum = 0;
			int StoneUseOccaType = 0;

			for (size_t i = 0; i < 5; i++)
			{
				StoneGrooveNum++;
				if (i > 2) {
					StoneUseOccaType = 1;
				}

				BYTE* pStoneGrooveInfo = pKartStoneInfo;
				Write16(pStoneGrooveInfo, 0); //len

				Write32(pStoneGrooveInfo, StoneUseOccaType); //StoneUseOccaType
				Write32(pStoneGrooveInfo, 0); //SkillStoneID

				len = pStoneGrooveInfo - pKartStoneInfo;
				Set16(pKartStoneInfo, (WORD)len);
				pKartStoneInfo += len;
			}
			Set32(pStoneGrooveNum, StoneGrooveNum);
		}


		len = pKartStoneInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 906, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyKickFromServer(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ReasonID
	Write32(p, Client->Uin); //SrcUin
	Write8(p, 0); //ReasonLen
	Write8(p, 0); //AntiCheatMode
	Write8(p, 0); //KickType

	len = p - buf.data();
	SendToClient(Client, 900, buf.data(), len, Client->ServerID, FE_GAMESVRD, Client->ConnID, Notify);
}

void RequestKartRefitInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);



	KartRefitItemInfo Items[70];
	size_t ItemNum = 0;
	const int step = 70;
	//bool HasMoreInfo = false;

	std::vector<KartRefitItemInfo> kartRefitItemInfo;
	SelectKartRefitItemInfo(Client->Uin, kartRefitItemInfo);

	size_t totalItems = kartRefitItemInfo.size();
	bool HasMoreInfo = (totalItems > step);

	// 处理第一批数据
	size_t firstBatchSize = min(totalItems, static_cast<size_t>(step));
	for (size_t j = 0; j < firstBatchSize; ++j) {
		Items[ItemNum].KartId = kartRefitItemInfo[j].KartId;
		Items[ItemNum].RefitCout = kartRefitItemInfo[j].RefitCout;
		Items[ItemNum].MaxFlags = kartRefitItemInfo[j].MaxFlags;
		Items[ItemNum].WWeight = kartRefitItemInfo[j].WWeight;
		Items[ItemNum].SpeedWeight = kartRefitItemInfo[j].SpeedWeight;
		Items[ItemNum].JetWeight = kartRefitItemInfo[j].JetWeight;
		Items[ItemNum].SJetWeight = kartRefitItemInfo[j].SJetWeight;
		Items[ItemNum].AccuWeight = kartRefitItemInfo[j].AccuWeight;
		Items[ItemNum].AntiCollisionWeight = kartRefitItemInfo[j].AntiCollisionWeight;
		Items[ItemNum].ShapeRefitCount = kartRefitItemInfo[j].ShapeRefitCount;
		Items[ItemNum].KartHeadRefitItemID = kartRefitItemInfo[j].KartHeadRefitItemID;
		Items[ItemNum].KartTailRefitItemID = kartRefitItemInfo[j].KartTailRefitItemID;
		Items[ItemNum].KartFlankRefitItemID = kartRefitItemInfo[j].KartFlankRefitItemID;
		Items[ItemNum].KartTireRefitItemID = kartRefitItemInfo[j].KartTireRefitItemID;
		Items[ItemNum].ShapeSuitID = kartRefitItemInfo[j].ShapeSuitID;
		Items[ItemNum].Speed2Weight = kartRefitItemInfo[j].Speed2Weight;
		Items[ItemNum].DriftVecWeight = kartRefitItemInfo[j].DriftVecWeight;
		Items[ItemNum].AdditionalZSpeedWeight = kartRefitItemInfo[j].AdditionalZSpeedWeight;
		Items[ItemNum].LegendSuitLevel = kartRefitItemInfo[j].LegendSuitLevel;
		Items[ItemNum].LegendSuitLevelChoice = kartRefitItemInfo[j].LegendSuitLevelChoice;
		Items[ItemNum].ShapeLegendSuitID = kartRefitItemInfo[j].ShapeLegendSuitID;
		Items[ItemNum].SkinSuitID = kartRefitItemInfo[j].SkinSuitID;
		ItemNum++;
	}
	ResponseKartRefitInfo(Client, ItemNum, Items, HasMoreInfo);

	// 处理剩余数据
	for (size_t i = step; i < totalItems; i += step) {
		ItemNum = 0;
		size_t batchSize = min(step, totalItems - i);
		for (size_t j = i; j < i + batchSize; ++j) {
			Items[ItemNum].KartId = kartRefitItemInfo[j].KartId;
			Items[ItemNum].RefitCout = kartRefitItemInfo[j].RefitCout;
			Items[ItemNum].MaxFlags = kartRefitItemInfo[j].MaxFlags;
			Items[ItemNum].WWeight = kartRefitItemInfo[j].WWeight;
			Items[ItemNum].SpeedWeight = kartRefitItemInfo[j].SpeedWeight;
			Items[ItemNum].JetWeight = kartRefitItemInfo[j].JetWeight;
			Items[ItemNum].SJetWeight = kartRefitItemInfo[j].SJetWeight;
			Items[ItemNum].AccuWeight = kartRefitItemInfo[j].AccuWeight;
			Items[ItemNum].AntiCollisionWeight = kartRefitItemInfo[j].AntiCollisionWeight;
			Items[ItemNum].ShapeRefitCount = kartRefitItemInfo[j].ShapeRefitCount;
			Items[ItemNum].KartHeadRefitItemID = kartRefitItemInfo[j].KartHeadRefitItemID;
			Items[ItemNum].KartTailRefitItemID = kartRefitItemInfo[j].KartTailRefitItemID;
			Items[ItemNum].KartFlankRefitItemID = kartRefitItemInfo[j].KartFlankRefitItemID;
			Items[ItemNum].KartTireRefitItemID = kartRefitItemInfo[j].KartTireRefitItemID;
			Items[ItemNum].ShapeSuitID = kartRefitItemInfo[j].ShapeSuitID;
			Items[ItemNum].Speed2Weight = kartRefitItemInfo[j].Speed2Weight;
			Items[ItemNum].DriftVecWeight = kartRefitItemInfo[j].DriftVecWeight;
			Items[ItemNum].AdditionalZSpeedWeight = kartRefitItemInfo[j].AdditionalZSpeedWeight;
			Items[ItemNum].LegendSuitLevel = kartRefitItemInfo[j].LegendSuitLevel;
			Items[ItemNum].LegendSuitLevelChoice = kartRefitItemInfo[j].LegendSuitLevelChoice;
			Items[ItemNum].ShapeLegendSuitID = kartRefitItemInfo[j].ShapeLegendSuitID;
			Items[ItemNum].SkinSuitID = kartRefitItemInfo[j].SkinSuitID;
			ItemNum++;
		}
		HasMoreInfo = (i + batchSize < totalItems);
		ResponseKartRefitInfoMoreInfo(Client, ItemNum, Items, HasMoreInfo);
	}

}

void ResponseKartRefitInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, KartRefitItemInfo* Items, bool HasMoreInfo)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin

	BYTE* pKartNum = p;
	Write16(p, ItemNum); //KartNum
	for (size_t i = 0; i < ItemNum; i++)
	{
		UINT uSkinSuitID = Items[i].SkinSuitID;
		if (uSkinSuitID < 1)
			uSkinSuitID = Items[i].ShapeSuitID;

		//KartRefitInfo
		BYTE* pKartRefitInfo = p;
		Write16(pKartRefitInfo, 0); //len

		Write32(pKartRefitInfo, Client->Uin); //Uin
		Write32(pKartRefitInfo, Items[i].KartId); //KartId
		Write32(pKartRefitInfo, Items[i].RefitCout); //RefitCout
		Write16(pKartRefitInfo, Items[i].MaxFlags); //MaxFlags
		Write16(pKartRefitInfo, Items[i].WWeight); //WWeight
		Write16(pKartRefitInfo, Items[i].SpeedWeight); //SpeedWeight
		Write16(pKartRefitInfo, Items[i].JetWeight); //JetWeight
		Write16(pKartRefitInfo, Items[i].SJetWeight); //SJetWeight
		Write16(pKartRefitInfo, Items[i].AccuWeight); //AccuWeight
		Write32(pKartRefitInfo, Items[i].ShapeRefitCount); //ShapeRefitCount
		Write32(pKartRefitInfo, Items[i].KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pKartRefitInfo, Items[i].KartTailRefitItemID); //KartTailRefitItemID
		Write32(pKartRefitInfo, Items[i].KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pKartRefitInfo, Items[i].KartTireRefitItemID); //KartTireRefitItemID
		{ //KartRefitExInfo
			BYTE* pKartRefitExInfo = pKartRefitInfo;
			Write16(pKartRefitExInfo, 0); //len

			Write8(pKartRefitExInfo, 0); //SpeedRefitStar
			Write8(pKartRefitExInfo, 0); //JetRefitStar
			Write8(pKartRefitExInfo, 0); //SJetRefitStar
			Write8(pKartRefitExInfo, 0); //AccuRefitStar
			Write8(pKartRefitExInfo, 0); //SpeedAddRatio
			Write8(pKartRefitExInfo, 0); //JetAddRatio
			Write8(pKartRefitExInfo, 0); //SJetAddRatio
			Write8(pKartRefitExInfo, 0); //AccuAddRatio

			len = pKartRefitExInfo - pKartRefitInfo;
			Set16(pKartRefitInfo, (WORD)len);
			pKartRefitInfo += len;
		}
		Write32(pKartRefitInfo, 0); //SecondRefitCount
		Write16(pKartRefitInfo, Items[i].Speed2Weight); //Speed2Weight
		Write16(pKartRefitInfo, Items[i].DriftVecWeight); //DriftVecWeight
		Write16(pKartRefitInfo, Items[i].AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pKartRefitInfo, Items[i].AntiCollisionWeight); //AntiCollisionWeight
		Write16(pKartRefitInfo, 0); //LuckyValue
		Write16(pKartRefitInfo, 0); //RefitLuckyValueMaxWeight
		Write32(pKartRefitInfo, uSkinSuitID); //ShapeSuitID
		Write8(pKartRefitInfo, Items[i].LegendSuitLevel); //LegendSuitLevel
		Write32(pKartRefitInfo, Items[i].LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pKartRefitInfo, Items[i].ShapeLegendSuitID); //ShapeLegendSuitID

		len = pKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

#ifndef ZingSpeed
	Write32(p, 0); //MaxLuckyWeight
	Write32(p, 0); //LuckyValue
	Write32(p, 0); //MaxLuckyValue
	Write8(p, 0); //LevelNum

	Write8(p, HasMoreInfo); //HasMoreInfo
#endif
	len = p - buf.data();
	SendToClient(Client, 271, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void ResponseKartRefitInfoMoreInfo(std::shared_ptr<ClientNode> Client, UINT ItemNum, KartRefitItemInfo* Items, bool HasMoreInfo)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, ItemNum); //KartNum
	for (size_t i = 0; i < ItemNum; i++)
	{
		UINT uSkinSuitID = Items[i].SkinSuitID;
		if (uSkinSuitID < 1)
			uSkinSuitID = Items[i].ShapeSuitID;

		//KartRefitInfo
		BYTE* pKartRefitInfo = p;
		Write16(pKartRefitInfo, 0); //len

		Write32(pKartRefitInfo, Client->Uin); //Uin
		Write32(pKartRefitInfo, Items[i].KartId); //KartId
		Write32(pKartRefitInfo, Items[i].RefitCout); //RefitCout
		Write16(pKartRefitInfo, Items[i].MaxFlags); //MaxFlags
		Write16(pKartRefitInfo, Items[i].WWeight); //WWeight
		Write16(pKartRefitInfo, Items[i].SpeedWeight); //SpeedWeight
		Write16(pKartRefitInfo, Items[i].JetWeight); //JetWeight
		Write16(pKartRefitInfo, Items[i].SJetWeight); //SJetWeight
		Write16(pKartRefitInfo, Items[i].AccuWeight); //AccuWeight
		Write32(pKartRefitInfo, Items[i].ShapeRefitCount); //ShapeRefitCount
		Write32(pKartRefitInfo, Items[i].KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pKartRefitInfo, Items[i].KartTailRefitItemID); //KartTailRefitItemID
		Write32(pKartRefitInfo, Items[i].KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pKartRefitInfo, Items[i].KartTireRefitItemID); //KartTireRefitItemID
		{ //KartRefitExInfo
			BYTE* pKartRefitExInfo = pKartRefitInfo;
			Write16(pKartRefitExInfo, 0); //len

			Write8(pKartRefitExInfo, 0); //SpeedRefitStar
			Write8(pKartRefitExInfo, 0); //JetRefitStar
			Write8(pKartRefitExInfo, 0); //SJetRefitStar
			Write8(pKartRefitExInfo, 0); //AccuRefitStar
			Write8(pKartRefitExInfo, 0); //SpeedAddRatio
			Write8(pKartRefitExInfo, 0); //JetAddRatio
			Write8(pKartRefitExInfo, 0); //SJetAddRatio
			Write8(pKartRefitExInfo, 0); //AccuAddRatio

			len = pKartRefitExInfo - pKartRefitInfo;
			Set16(pKartRefitInfo, (WORD)len);
			pKartRefitInfo += len;
		}
		Write32(pKartRefitInfo, 0); //SecondRefitCount
		Write16(pKartRefitInfo, Items[i].Speed2Weight); //Speed2Weight
		Write16(pKartRefitInfo, Items[i].DriftVecWeight); //DriftVecWeight
		Write16(pKartRefitInfo, Items[i].AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pKartRefitInfo, Items[i].AntiCollisionWeight); //AntiCollisionWeight
		Write16(pKartRefitInfo, 0); //LuckyValue
		Write16(pKartRefitInfo, 0); //RefitLuckyValueMaxWeight
		Write32(pKartRefitInfo, uSkinSuitID); //ShapeSuitID
		Write8(pKartRefitInfo, Items[i].LegendSuitLevel); //LegendSuitLevel
		Write32(pKartRefitInfo, Items[i].LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pKartRefitInfo, Items[i].ShapeLegendSuitID); //ShapeLegendSuitID

		len = pKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, HasMoreInfo); //HasNext

	len = p - buf.data();
	SendToClient(Client, 318, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestNewGetFriendList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	ResponseNewGetFriendList(Client);
}
void ResponseNewGetFriendList(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	
	
	int result;

	NotifyAppellationList(Client);

	UINT FriendNum = 0;
	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write16(p, 0xFF); //MaxFriendNum
	Write16(p, 1); //TotalFriendNum
	Write16(p, 1); //FriendGroupNum

	for (size_t i = 0; i < 1; i++)
	{ //FriendGroupInfo
		BYTE* pFriendGroupInfo = p;
		Write16(pFriendGroupInfo, 0); //len

		Write16(pFriendGroupInfo, 0); //GroupID
		//GroupName[]
		memset(pFriendGroupInfo, 0, 17);
		memcpy(pFriendGroupInfo, "我的好友", 9);
		pFriendGroupInfo += MaxNickName;
		BYTE* pFriendNum = pFriendGroupInfo;
		Write16(pFriendGroupInfo, 0); //FriendNum


		std::vector<int> conditionValues;
		conditionValues.push_back(Client->Uin);
		std::vector<int> resultRow = executeQueryCol("SELECT FriendUin FROM FriendInfo WHERE Uin = ?;", "ResponseNewGetFriendList", conditionValues);
		for (size_t i = 0; i < resultRow.size(); i++)
		{
			BYTE* pFriend = pFriendGroupInfo;
			Write16(pFriend, 0); //len

			Write32(pFriend, resultRow[i]); //FriendUin
			Write32(pFriend, 0); //RelationFlag
			Write32(pFriend, 0); //Degree
			Write8(pFriend, 0); //Flag
			Write32(pFriend, 0); //ReadedEventID

			len = pFriend - pFriendGroupInfo;
			Set16(pFriendGroupInfo, (WORD)len);
			pFriendGroupInfo += len;
		}

		Set16(pFriendNum, resultRow.size());
		len = pFriendGroupInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 163, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

void NotifyClientAddItemBox(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, Client->Uin); //SrcUin
	Write16(p, ItemNum); //ItemNum
	{
		for (size_t i = 0; i < ItemNum; i++)
		{

			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, Items[i].ItemID);
			Write32(pItemInfo, Items[i].ItemNum);
			Write32(pItemInfo, Items[i].AvailPeriod);//为0是物品栏显示个数 -1显示永久 抽奖列表会显示已拥有
			//Write32(pItemInfo, Items[i].AvailPeriod);//为0是物品栏显示个数 -1显示永久 抽奖列表会显示已拥有
			Write8(pItemInfo, Items[i].Status);
			Write32(pItemInfo, (UINT)time(0));//Items[i].ObtainTime 该属性放在道具添加在背包的最前面还是最后面
			Write32(pItemInfo, Items[i].OtherAttribute);

			if (GetItemType(Items[i].ItemID) == EAIT_CAR && Uinew) { //弹动画界面
				Write16(pItemInfo, 11); //ItemType
			}
			else {
				Write16(pItemInfo, Items[i].ItemType); //ItemType
			}



			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}

	Write16(p, 0); //AttachLen
	Write16(p, 0); //AddKartNum
	/*
	for (size_t i = 0; i < n; i++)
	{ //AddKartRefitInfo
		BYTE* pAddKartRefitInfo = p;
		Write16(pAddKartRefitInfo, 0); //len

		Write32(pAddKartRefitInfo, 0); //Uin
		Write32(pAddKartRefitInfo, 0); //KartId
		Write32(pAddKartRefitInfo, 0); //RefitCout
		Write16(pAddKartRefitInfo, 0); //MaxFlags
		Write16(pAddKartRefitInfo, 0); //WWeight
		Write16(pAddKartRefitInfo, 0); //SpeedWeight
		Write16(pAddKartRefitInfo, 0); //JetWeight
		Write16(pAddKartRefitInfo, 0); //SJetWeight
		Write16(pAddKartRefitInfo, 0); //AccuWeight
		Write32(pAddKartRefitInfo, 0); //ShapeRefitCount
		Write32(pAddKartRefitInfo, 0); //KartHeadRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartTailRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartFlankRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartTireRefitItemID

		len = pAddKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	*/
	Write8(p, 0); //IncRedPacketNum

	len = p - buf.data();
	SendToClient(Client, 529, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientAddItem(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	int AddKartNum = 0;
	Write32(p, Client->Uin); //Uin
	Write32(p, Client->Uin); //SrcUin
	Write16(p, ItemNum); //ItemNum
	{
		for (size_t i = 0; i < ItemNum; i++)
		{
			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, Items[i].ItemID);
			Write32(pItemInfo, Items[i].ItemNum);
			Write32(pItemInfo, Items[i].AvailPeriod);//为0是物品栏显示个数 -1显示永久 抽奖列表会显示已拥有
			Write8(pItemInfo, Items[i].Status);
			Write32(pItemInfo, (UINT)time(0));//Items[i].ObtainTime 该属性放在道具添加在背包的最前面还是最后面
			Write32(pItemInfo, Items[i].OtherAttribute);
			Write16(pItemInfo, Items[i].ItemType); //ItemType

			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}

	Write16(p, 0); //AttachLen

	Write16(p, 0); //AddKartNum

	for (size_t i = 0; i < 0; i++)
	{ //AddKartRefitInfo
		if (GetItemType(Items[i].ItemID) != EAIT_CAR) continue;
		BYTE* pAddKartRefitInfo = p;
		Write16(pAddKartRefitInfo, 0); //len

		Write32(pAddKartRefitInfo, Client->Uin); //Uin
		Write32(pAddKartRefitInfo, Items[i].ItemID); //KartId
		Write32(pAddKartRefitInfo, 0); //RefitCout
		Write16(pAddKartRefitInfo, 0); //MaxFlags
		Write16(pAddKartRefitInfo, 0); //WWeight
		Write16(pAddKartRefitInfo, 0); //SpeedWeight
		Write16(pAddKartRefitInfo, 0); //JetWeight
		Write16(pAddKartRefitInfo, 0); //SJetWeight
		Write16(pAddKartRefitInfo, 0); //AccuWeight
		Write32(pAddKartRefitInfo, 0); //ShapeRefitCount
		Write32(pAddKartRefitInfo, 0); //KartHeadRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartTailRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartFlankRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartTireRefitItemID

		len = pAddKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //IncRedPacketNum

	len = p - buf.data();
	SendToClient(Client, 529, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientAddItemNew(std::shared_ptr<ClientNode> Client, UINT ItemNum, std::vector<ItemInfo> Items)
{

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	int AddKartNum = 0;
	Write32(p, Client->Uin); //Uin
	Write32(p, 10001); //SrcUin
	Write16(p, ItemNum); //ItemNum
	{
		for (size_t i = 0; i < ItemNum; i++)
		{
			BYTE* pItemInfo = p;
			Write16(pItemInfo, 0); //len

			Write32(pItemInfo, Items[i].ItemID);
			Write32(pItemInfo, Items[i].ItemNum);
			Write32(pItemInfo, Items[i].AvailPeriod);//为0是物品栏显示个数 -1显示永久 抽奖列表会显示已拥有
			Write8(pItemInfo, Items[i].Status);
			Write32(pItemInfo, (UINT)time(0));//Items[i].ObtainTime 该属性放在道具添加在背包的最前面还是最后面
			Write32(pItemInfo, Items[i].OtherAttribute);
			Write16(pItemInfo, Items[i].ItemType); //ItemType

			len = pItemInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	//std::string msg = "您的好友/C01抖音超超(584804938)/C00向您赠送了[/C01皮肤券/C00]"
	WriteString16(p, "您的好友/C01抖音超超(584804938)/C00向您赠送了[/C01皮肤券/C00]"); //AttachLen

	Write16(p, 0); //AddKartNum

	for (size_t i = 0; i < 0; i++)
	{ //AddKartRefitInfo
		if (GetItemType(Items[i].ItemID) != EAIT_CAR) continue;
		BYTE* pAddKartRefitInfo = p;
		Write16(pAddKartRefitInfo, 0); //len

		Write32(pAddKartRefitInfo, Client->Uin); //Uin
		Write32(pAddKartRefitInfo, Items[i].ItemID); //KartId
		Write32(pAddKartRefitInfo, 0); //RefitCout
		Write16(pAddKartRefitInfo, 0); //MaxFlags
		Write16(pAddKartRefitInfo, 0); //WWeight
		Write16(pAddKartRefitInfo, 0); //SpeedWeight
		Write16(pAddKartRefitInfo, 0); //JetWeight
		Write16(pAddKartRefitInfo, 0); //SJetWeight
		Write16(pAddKartRefitInfo, 0); //AccuWeight
		Write32(pAddKartRefitInfo, 0); //ShapeRefitCount
		Write32(pAddKartRefitInfo, 0); //KartHeadRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartTailRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartFlankRefitItemID
		Write32(pAddKartRefitInfo, 0); //KartTireRefitItemID

		len = pAddKartRefitInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //IncRedPacketNum

	len = p - buf.data();
	SendToClient(Client, 529, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestDeleteOneItem(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	if (deleteflag == 1) {
		ResponseStartGame(Client, "目前不允许删除宝贝哦！");
		return;
	}

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);
	int ItemID = Read32(Body);

	PlayerDB_DeleteItem(Client->Uin, ItemID);
	ResponseDeleteOneItem(Client, ItemID);
}
void ResponseDeleteOneItem(std::shared_ptr<ClientNode> Client, int ItemID)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write32(p, ItemID); //ItemID
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 261, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyUseItemResult3(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum, bool AddorReduce)
{
	if (UseItemNum == 0) return;
	int result;

	int CurrentItemNum = 0;
	//GetItemNum
	int realnum;
	//增加

	CurrentItemNum = PlayerDB_GetItemNum(Client->Uin, ItemID);

	if (AddorReduce) {
		realnum = CurrentItemNum + UseItemNum;
		PlayerDB_UpdateItemNum(Client->Uin, ItemID, realnum);
	}
	else {
		realnum = CurrentItemNum - UseItemNum;
		if (realnum <= 0)
		{
			PlayerDB_DeleteItem(Client->Uin, ItemID);
		}
		else
		{
			PlayerDB_UpdateItemNum(Client->Uin, ItemID, realnum);
		}
	}

	NotifyUseItemResult(Client, ItemID, UseItemNum, realnum);

}

void NotifyUseItemResult2(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum)
{
	
	
	int result;

	int CurrentItemNum = 0;
	//GetItemNum

	CurrentItemNum = PlayerDB_GetItemNum(Client->Uin, ItemID);

	int realnum = CurrentItemNum - UseItemNum;
	if (realnum <= 0)
	{
		PlayerDB_DeleteItem(Client->Uin, ItemID);
	}
	else
	{
		PlayerDB_UpdateItemNum(Client->Uin, ItemID, realnum);
	}
	NotifyUseItemResult(Client, ItemID, UseItemNum, realnum);

}

void NotifyUseItemResult(std::shared_ptr<ClientNode> Client, int ItemID, int UseItemNum, int CurrentItemNum)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, ItemID); //ItemID
	Write32(p, UseItemNum); //UseItemNum
	Write32(p, CurrentItemNum); //CurrentItemNum

	len = p - buf.data();
	SendToClient(Client, 556, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestRefitKart(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int KartItemId = Read32(Body);

	UCHAR KartNameLen = Read8(Body);
	Body += KartNameLen;

	int RefitItemId = Read32(Body);

	int Reserved = Read32(Body);
#ifndef ZingSpeed
	//Read16 2字节 是批量改装的数量
	short RefitCount = Read16(Body);
	short RefitTimes = Read16(Body);
#endif
	//printf("[RequestRefitKart] KartItemId: %d, KartNameLen: %d, RefitItemId: %d, Reserved: %d\n", KartItemId, KartNameLen, RefitItemId, Reserved);

	const char* Reason = "改装成功!";

	
	
	int result;

	int RefitItemNum = 0;
	//查询当前车辆改装信息
	//short MaxFlags = 0;
	short WWeight = 0;
	short SpeedWeight = 0;
	short JetWeight = 0;
	short SJetWeight = 0;
	short AccuWeight = 0;
	short AntiCollisionWeight = 0;
	char RefitUseItem = 0;


	RefitItemNum = PlayerDB_GetItemNum(Client->Uin, RefitItemId);

	KartRefitItemInfo* KartRefit;
	SelectOneKartRefitItemInfo(Client->Uin, KartItemId, KartRefit);
	if (KartRefit) {
		WWeight = KartRefit->WWeight;
		SpeedWeight = KartRefit->SpeedWeight;
		JetWeight = KartRefit->JetWeight;
		SJetWeight = KartRefit->SJetWeight;
		AccuWeight = KartRefit->AccuWeight;
		AntiCollisionWeight = KartRefit->AntiCollisionWeight;
		delete[] KartRefit;
	}

	char RefitLimit = 0;
	while (RefitCount > 0)
	{
		RefitCount--;
		if (RefitItemNum > 0) {
			//改装上限29
			switch (RefitItemId)
			{
			case 12376: //Đánh lửa+1 //点火装置+1
				SJetWeight += 1;
				if (SJetWeight > 29)
				{
					SJetWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12377: //Bơm khí+1 //进气系统+1
				AccuWeight += 1;
				if (AccuWeight > 29)
				{
					AccuWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12378: //Nhiên liệu+1 //燃料系统+1
				JetWeight += 1;
				if (JetWeight > 29)
				{
					JetWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12379: //Giảm Sóc+1 //悬挂系统+1
				WWeight += 1;
				if (WWeight > 29)
				{
					WWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12380: //Động cơ+1 //引擎装置+1
				SpeedWeight += 1;
				if (SpeedWeight > 29)
				{
					SpeedWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12386: //Đánh lửa+2
				SJetWeight += 2;
				if (SJetWeight > 29)
				{
					SJetWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12387: //Bơm khí+2
				AccuWeight += 2;
				if (AccuWeight > 29)
				{
					AccuWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12388: //Nhiên liệu+2
				JetWeight += 2;
				if (JetWeight > 29)
				{
					JetWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12389: //Giảm Sóc+2
				WWeight += 2;
				if (WWeight > 29)
				{
					WWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12390: //Động cơ+2
				SpeedWeight += 2;
				if (SpeedWeight > 29)
				{
					SpeedWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 96597: ///防护+1
				AntiCollisionWeight += 1;
				if (AntiCollisionWeight > 29)
				{
					AntiCollisionWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 96599: //防护+2
				AntiCollisionWeight += 2;
				if (AntiCollisionWeight > 29)
				{
					AntiCollisionWeight = 29;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 12384: //悬挂-1
				WWeight -= 1;
				if (WWeight <= 0)
				{
					WWeight = 0;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			case 96598: //防护-1
				AntiCollisionWeight -= 1;
				if (AntiCollisionWeight <= 0)
				{
					AntiCollisionWeight = 0;
					RefitLimit = 1;
					ResponseStartGame(Client, "宝贝已经不能改了噢!");
					break;
				}
				break;
			default:
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
	if (KartItemId == Client->KartID)
	{
		//更新内存性能
		Client->ClientKartRefitItemInfo.WWeight = WWeight;
		Client->ClientKartRefitItemInfo.SpeedWeight = SpeedWeight;
		Client->ClientKartRefitItemInfo.JetWeight = JetWeight;
		Client->ClientKartRefitItemInfo.SJetWeight = SJetWeight;
		Client->ClientKartRefitItemInfo.AccuWeight = AccuWeight;
		Client->ClientKartRefitItemInfo.AntiCollisionWeight = AntiCollisionWeight;
	}

	KartRefitItemInfo* KartRefitUpdate = new KartRefitItemInfo[1];
	KartRefitUpdate->AntiCollisionWeight = AntiCollisionWeight;
	KartRefitUpdate->SJetWeight = SJetWeight;
	KartRefitUpdate->AccuWeight = AccuWeight;
	KartRefitUpdate->JetWeight = JetWeight;
	KartRefitUpdate->WWeight = WWeight;
	KartRefitUpdate->SpeedWeight = SpeedWeight;
	UpdateOneKartRefitItemInfo(Client->Uin, KartItemId, KartRefitUpdate);
	if (RefitItemNum < 0) {
		PlayerDB_DeleteItem(Client->Uin, RefitItemId);
	}
	else {
		PlayerDB_UpdateItemNum(Client->Uin, RefitItemId, RefitItemNum);
	}

	ResponseRefitKart(Client, KartItemId, Reason, SJetWeight, AccuWeight, JetWeight, WWeight, SpeedWeight, AntiCollisionWeight);
	NotifyUseItemResult(Client, RefitItemId, RefitUseItem, RefitItemNum);


}
void ResponseRefitKart(std::shared_ptr<ClientNode> Client, int KartItemId, const char* Reason, short inc_SJetWeight, short inc_AccuWeight, short inc_JetWeight, short inc_WWeight, short inc_SpeedWeight, short inc_AntiCollisionWeight)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, 0); //Result
	Write32(p, Client->Uin); //Uin
	Write32(p, KartItemId); //KartItemId
	{ //Increasement增长属性

		BYTE* pIncreasement = p;
		Write16(pIncreasement, 0); //len

		Write32(pIncreasement, Client->Uin); //Uin
		Write32(pIncreasement, KartItemId); //KartId
		Write32(pIncreasement, 1); //RefitCout
		Write16(pIncreasement, 0); //MaxFlags
		Write16(pIncreasement, inc_WWeight); //WWeight
		Write16(pIncreasement, inc_SpeedWeight); //SpeedWeight
		Write16(pIncreasement, inc_JetWeight); //JetWeight
		Write16(pIncreasement, inc_SJetWeight); //SJetWeight
		Write16(pIncreasement, inc_AccuWeight); //AccuWeight
		Write32(pIncreasement, 0); //ShapeRefitCount
		Write32(pIncreasement, 0); //KartHeadRefitItemID
		Write32(pIncreasement, 0); //KartTailRefitItemID
		Write32(pIncreasement, 0); //KartFlankRefitItemID
		Write32(pIncreasement, 0); //KartTireRefitItemID
#ifndef ZingSpeed
		Write32(pIncreasement, 0); //SecondRefitCount进阶修改
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.Speed2Weight); //Speed2Weight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.DriftVecWeight); //DriftVecWeight
		Write16(pIncreasement, Client->ClientKartRefitItemInfo.AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pIncreasement, inc_AntiCollisionWeight); //AntiCollisionWeight
		Write16(pIncreasement, 0); //LuckyValue
		Write16(pIncreasement, 0); //RefitLuckyValueMaxWeight
		Write32(pIncreasement, 0); //ShapeSuitID 皮肤适配id
		Write8(pIncreasement, 0); //LegendSuitLevel
		Write32(pIncreasement, 0); //LegendSuitLevelChoice
		Write32(pIncreasement, 0); //ShapeLegendSuitID 传奇皮肤适配id
#endif

		len = pIncreasement - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //CurAttr
		BYTE* pCurAttr = p;
		Write16(pCurAttr, 0); //len

		Write32(pCurAttr, Client->Uin); //Uin
		Write32(pCurAttr, KartItemId); //KartId
		Write32(pCurAttr, 0); //RefitCout
		Write16(pCurAttr, 0); //MaxFlags
		Write16(pCurAttr, inc_WWeight); //WWeight
		Write16(pCurAttr, inc_SpeedWeight); //SpeedWeight
		Write16(pCurAttr, inc_JetWeight); //JetWeight
		Write16(pCurAttr, inc_SJetWeight); //SJetWeight
		Write16(pCurAttr, inc_AccuWeight); //AccuWeight
		Write32(pCurAttr, 0); //ShapeRefitCount
		Write32(pCurAttr, 0); //KartHeadRefitItemID
		Write32(pCurAttr, 0); //KartTailRefitItemID
		Write32(pCurAttr, 0); //KartFlankRefitItemID
		Write32(pCurAttr, 0); //KartTireRefitItemID
#ifndef ZingSpeed
		Write32(pCurAttr, 0); //SecondRefitCount
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.Speed2Weight); //Speed2Weight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.DriftVecWeight); //DriftVecWeight
		Write16(pCurAttr, Client->ClientKartRefitItemInfo.AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pCurAttr, inc_AntiCollisionWeight); //AntiCollisionWeight
		Write16(pCurAttr, 0); //LuckyValue
		Write16(pCurAttr, 0); //RefitLuckyValueMaxWeight
		Write32(pCurAttr, 0); //ShapeSuitID
		Write8(pCurAttr, 0); //LegendSuitLevel
		Write32(pCurAttr, 0); //LegendSuitLevelChoice
		Write32(pCurAttr, 0); //ShapeLegendSuitID
#endif
		len = pCurAttr - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 500); //ExpAward
	Write32(p, 250); //MoneyAward

	len = strlen(Reason);
	Write16(p, (WORD)len); //ReasonLen
	memcpy(p, Reason, len);
	p += len;

#ifndef ZingSpeed
	Write16(p, 0); //RefitTimes
	Write16(p, 1); //ResponseIdx
	Write8(p, 1); //LastIdx
	Write32(p, 0); //LuckyValue
#endif

	len = p - buf.data();
	//PrintfBody(0, 0, buf, len);
	SendToClient(Client, 174, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseSkinChange(std::shared_ptr<ClientNode> Client, int KartItemId, int SkinItemId, int karthead, int kartail, int karflank, int kartire)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	UINT ShapeSuitID = SkinItemId;
	/*switch (SkinItemId)//传奇皮肤
	{
	case 120246:
		ShapeSuitID = 118729;
		break;
	case 127359:
		ShapeSuitID = 126537;
		break;
	case 127360:
		ShapeSuitID = 127365;
		break;
	case 123513:
		ShapeSuitID = 123238;
		break;
	case 126355:
		ShapeSuitID = 127437;
		break;
	}*/

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write32(p, KartItemId); //KartItemID
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
	Write32(p, karthead); //RefitItemID[]
	Write32(p, kartail); //RefitItemID[]
	Write32(p, karflank); //RefitItemID[]
	Write32(p, kartire); //RefitItemID[]
	Write32(p, SkinItemId); //ShapeSuitID
	{ //RefitClientInfo
		BYTE* pRefitClientInfo = p;
		Write16(pRefitClientInfo, 0); //len

		Write32(pRefitClientInfo, Client->Uin); //Uin
		Write32(pRefitClientInfo, KartItemId); //KartId
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
		Write32(pRefitClientInfo, ShapeSuitID); //ShapeSuitID
		Write8(pRefitClientInfo, 10); //LegendSuitLevel
		Write32(pRefitClientInfo, 1111); //LegendSuitLevelChoice
		Write32(pRefitClientInfo, SkinItemId); //ShapeLegendSuitID

		len = pRefitClientInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 316, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestModifyPlayerSignature(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);

	char Signature[MaxSignature] = {};
	memcpy(Signature, p, MaxSignature);
	p += MaxSignature;

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET Signature=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, G2U(Signature));
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("RequestModifyPlayerSignature error\n");
		//throw;关闭
	}


	ResponseModifyPlayerSignature(Client, Signature);
}

void ResponseModifyPlayerSignature(std::shared_ptr<ClientNode> Client, char* Signature)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen

	//Signature[]
	memcpy(p, Signature, MaxSignature);
	p += MaxSignature;


	len = p - buf.data();
	SendToClient(Client, 376, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseEngageRoleInfo(std::shared_ptr<ClientNode> Client, UINT DstUin)
{
	BYTE buf[8192];
	BYTE* p = buf;
	size_t len;
	UINT KartItemId = 0;
	BOOL findresult;

	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);

	Write16(p, 0); //ResultID
	{ //PlayerInfo
		BYTE* pPlayerInfo = p;
		Write16(pPlayerInfo, 0); //len

		Write32(pPlayerInfo, DstUin); //Uin
		Write32(pPlayerInfo, 0); //Identity
		Write16(pPlayerInfo, 0); //PlayerID
		findresult = WritePlayerDBBaseInfo(pPlayerInfo, DstUin, Client);
		if (findresult == FALSE)
		{

			return;
		}
		WritePlayerGuildInfo(pPlayerInfo, DstUin, Client);
		KartItemId = WriteEquipedItem2(pPlayerInfo, DstUin, Client);

		Write32(pPlayerInfo, 0); //MasterPoint
		Write32(pPlayerInfo, 0); //TotalGuildProsperity

		Write16(pPlayerInfo, 0x0107); //VipFlag
		Write16(pPlayerInfo, 10); //VipGrowRate

		Write8(pPlayerInfo, 0); //AppellationNum

		{ //NobleInfo
			BYTE* pNobleInfo = pPlayerInfo;
			Write16(pNobleInfo, 0); //len

			Write32(pNobleInfo, DstUin); //NobleID
			Write8(pNobleInfo, 0); //NobleLevel
			Write32(pNobleInfo, 0); //NoblePoint
			Write32(pNobleInfo, 0); //NobleLeftDays

			len = pNobleInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write8(pPlayerInfo, 0); //HasCarryWizard


#ifndef ZingSpeed
		{ //GuildVipBaseInfo
			BYTE* pGuildVipBaseInfo = pPlayerInfo;
			Write16(pGuildVipBaseInfo, 0); //len

			Write8(pGuildVipBaseInfo, 0); //GuildVipLevel
			Write32(pGuildVipBaseInfo, 0); //GuildVipPoint

			len = pGuildVipBaseInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
#endif

		Write8(pPlayerInfo, 0); //HasLDMInfo


#ifndef ZingSpeed
		Write8(pPlayerInfo, 1); //HasLoverVip

		for (size_t i = 0; i < 1; i++)
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

#endif

		Write8(pPlayerInfo, 0); //IsShowMounts

#ifndef ZingSpeed
		Write8(pPlayerInfo, 0); //HasGarden

		{ //PersonalGardenBaseInfo
			BYTE* pPersonalGardenBaseInfo = pPlayerInfo;
			Write16(pPersonalGardenBaseInfo, 0); //len

			Write8(pPersonalGardenBaseInfo, 0); //GardenLevel
			Write32(pPersonalGardenBaseInfo, 0); //GardenPoint
			Write8(pPersonalGardenBaseInfo, 0); //GrowRate
			Write32(pPersonalGardenBaseInfo, 0); //GardenExpireTime
			{ //SimpleInfo
				BYTE* pSimpleInfo = pPersonalGardenBaseInfo;
				Write16(pSimpleInfo, 0); //len

				Write32(pSimpleInfo, 0); //WeekPopularity
				Write32(pSimpleInfo, 0); //TotalPopularity
				Write32(pSimpleInfo, 0); //LastUpdatePopularityTime
				Write8(pSimpleInfo, 0); //PrivateType

				len = pSimpleInfo - pPersonalGardenBaseInfo;
				Set16(pPersonalGardenBaseInfo, (WORD)len);
				pPersonalGardenBaseInfo += len;
			}

			len = pPersonalGardenBaseInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}


		Write8(pPlayerInfo, 0); //isConsumeVip
		Write32(pPlayerInfo, 0); //ConsumeVipLevel
		Write32(pPlayerInfo, 0); //SearchTreasureNums 
		Write32(pPlayerInfo, 0); //GetTreasureNums 
		Write32(pPlayerInfo, 0); //ConsumeVipCharmVlaueOfMonth 
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
		{ //EmperorOtherInfo
			BYTE* pEmperorOtherInfo = pPlayerInfo;
			Write16(pEmperorOtherInfo, 0); //len

			Write32(pEmperorOtherInfo, 0); //ExpiredTime
			Write8(pEmperorOtherInfo, 0); //ShowExpireTips

			len = pEmperorOtherInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //ActivityInfo
			BYTE* pActivityInfo = pPlayerInfo;
			Write16(pActivityInfo, 0); //len

			Write32(pActivityInfo, 0); //TotalActivity
			Write32(pActivityInfo, 0); //ActivityLevel

			len = pActivityInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write8(pPlayerInfo, 0); //HaveWakedKartAttributeAddInfo  此处必须加，否则赛车进气改装会显示觉醒+4068%
		{ // WakeKartAttributeAddInfo
			BYTE* pWakeKartAttributeAddInfo = pPlayerInfo;
			Write16(pWakeKartAttributeAddInfo, 0); // len
			Write16(pWakeKartAttributeAddInfo, 0); // AccAddValue
			Write16(pWakeKartAttributeAddInfo, 0); // SpeedAddValue
			Write16(pWakeKartAttributeAddInfo, 0); // JetAddValue
			Write16(pWakeKartAttributeAddInfo, 0); // SJetAddValue

			len = pWakeKartAttributeAddInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}


		Write8(pPlayerInfo, 0); //HaveLoverBabyInfo
		{ //FindLoverBabyInfo
			BYTE* pFindLoverBabyInfo = pPlayerInfo;
			Write16(pFindLoverBabyInfo, 0); //len

			Write32(pFindLoverBabyInfo, 0); //BabyStatus
			Write32(pFindLoverBabyInfo, 0); //LoverBabyItemID
			Write32(pFindLoverBabyInfo, 0); //LoverBabyID
			Write32(pFindLoverBabyInfo, 0); //BabyGrowLevel
			Write32(pFindLoverBabyInfo, 0); //BabyStrengthLevel
			Write16(pFindLoverBabyInfo, 0); //LoverBabyStat
			Write16(pFindLoverBabyInfo, 0); //LoverBabyEquipStat
			Write16(pFindLoverBabyInfo, 0); //LoverBabyTransferdStatus

			len = pFindLoverBabyInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}


		{ //GansterScoreInfo
			BYTE* pGansterScoreInfo = pPlayerInfo;
			Write16(pGansterScoreInfo, 0); //len

			Write32(pGansterScoreInfo, 0); //GansterSeasonID
			Write32(pGansterScoreInfo, 0); //GansterScore
			Write32(pGansterScoreInfo, 0); //PoliceScore
			Write32(pGansterScoreInfo, 0); //TotalGansterScore

			len = pGansterScoreInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write32(pPlayerInfo, 0); //OlympicId
		Write32(pPlayerInfo, 0); //LastOlympicUpdateTime
		Write32(pPlayerInfo, 0); //NPCEliminateWinTimes
		{ //BorderInfo
			BYTE* pBorderInfo = pPlayerInfo;
			Write16(pBorderInfo, 0); //len

			Write32(pBorderInfo, 0); //SeasonID
			Write32(pBorderInfo, 0); //Zhanxun
			Write32(pBorderInfo, 0); //SeasonZhanxun

			len = pBorderInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //SpecialActivityID
		Write32(pPlayerInfo, 0); //ThemeHouseCollectValue 0x64
		Write8(pPlayerInfo, 0); //HaveSecondLoverBabyInfo
		{ //SecondLoverBabyInfo
			BYTE* pSecondLoverBabyInfo = pPlayerInfo;
			Write16(pSecondLoverBabyInfo, 0); //len
			Write32(pSecondLoverBabyInfo, 0); //BabyStatus 形态
			Write32(pSecondLoverBabyInfo, 0); //LoverBabyItemID
			Write32(pSecondLoverBabyInfo, 0); //LoverBabyID
			Write32(pSecondLoverBabyInfo, 0); //BabyGrowLevel
			Write32(pSecondLoverBabyInfo, 0); //BabyStrengthLevel
			Write16(pSecondLoverBabyInfo, 0); //LoverBabyStat
			Write16(pSecondLoverBabyInfo, 0); //LoverBabyEquipStat
			Write16(pSecondLoverBabyInfo, 0); //LoverBabyTransferdStatus

			len = pSecondLoverBabyInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //EquipBaby
		Write8(pPlayerInfo, 0); //ActiveStatus
		Write32(pPlayerInfo, 0); //HelperLev
		Write8(pPlayerInfo, 0); //HasRankedMatchInfo
		Write16(pPlayerInfo, 0); //PlayerSeasonInfoNums	
		Write8(pPlayerInfo, 0); //IsHelperAppOnline
		Write32(pPlayerInfo, 0); //RankedMatchSpeedGodNums
		Write8(pPlayerInfo, 0); //EquipSealType

		Write8(pPlayerInfo, 0); //HasHuanLingChangeInfo


		Write8(pPlayerInfo, 0); //SpecialActivityIDNum
		Write8(pPlayerInfo, 0); //PersonalPanelSelectRankedMatchFrameTag

		Write8(pPlayerInfo, 0); //SpecialActivityInfoNum


		Write8(pPlayerInfo, 0); //PersonalRankedMatchLevelShowTag

		Write8(pPlayerInfo, 0); //ItemNum


		Write8(pPlayerInfo, 0); //ExRightFlagLen

		Write8(pPlayerInfo, 0); //EquipNiChangItemNum

#endif
		len = pPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //IsOnLine   0不在线  1多人游戏大厅
	{ //PositionInfo
		BYTE* pPositionInfo = p;
		Write16(pPositionInfo, 0); //len

		Write8(pPositionInfo, 0); //ServerType
		Write16(pPositionInfo, 0); //Status
		Write32(pPositionInfo, 0); //ServerID
		{ //RoomInfo
			BYTE* pRoomInfo = pPositionInfo;
			Write16(pRoomInfo, 0); //len

			Write16(pRoomInfo, 0); //RoomID
			Write16(pRoomInfo, 0); //BaseGameMode
			Write16(pRoomInfo, 0); //SubGameMode
			Write8(pRoomInfo, 0); //CurrentPlayerNum
			Write8(pRoomInfo, 0); //TotalSeatNum
			Write8(pRoomInfo, 0); //Status
			Write32(pRoomInfo, 0); //MapID
			Write16(pRoomInfo, 0); //Flag

			//RoomName[]
			memset(pRoomInfo, 0, MaxRoomName);
			pRoomInfo += MaxRoomName;

			Write8(pRoomInfo, 0); //HasFBInfo

#ifndef ZingSpeed
			Write8(pRoomInfo, 0); //SpeFlag
			Write32(pRoomInfo, 0); //RoomNo
			Write16(pRoomInfo, 0); //SvrId
			Write16(pRoomInfo, 0); //SceneID
			Write8(pRoomInfo, 0); //CurrentOBNum
			Write8(pRoomInfo, 0); //TotalOBNum
			Write8(pRoomInfo, 0); //SpecialActivityStatus
			Write8(pRoomInfo, 0); //AllowAutoStart
			Write32(pRoomInfo, 0); //Gender
			Write32(pRoomInfo, 0); //LocaleCode
			Write8(pRoomInfo, 0); //TalkRoomBGID
			Write8(pRoomInfo, 0); //SpecialActivityIDNum

			Write8(pRoomInfo, 0); //SpecialActivityInfoNum


			Write8(pRoomInfo, 0); //ValueNum
			//Write32(pRoomInfo, 0); //EnterRoomValue[]
			Write8(pRoomInfo, 0); //ParaNum
			//Write32(pRoomInfo, 0); //ParaList[]
			Write32(pRoomInfo, 0); //GSvrRoomNo

			//OwnerName[]
			memset(pRoomInfo, 0, MaxNickName);
			pRoomInfo += MaxNickName;
#endif
			len = pRoomInfo - pPositionInfo;
			Set16(pPositionInfo, (WORD)len);
			pPositionInfo += len;
		}
		Write8(pPositionInfo, 0); //StatusLen
		Write8(pPositionInfo, 0); //CanJoin

		len = pPositionInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ReasonLen
	{ //PlayerRelationInfo
		BYTE* pPlayerRelationInfo = p;
		Write16(pPlayerRelationInfo, 0); //len

		Write32(pPlayerRelationInfo, 0); //SrcUin

		Write32(pPlayerRelationInfo, 0); //RelationFlag
		Write32(pPlayerRelationInfo, 0); //RelationUin

		memset(pPlayerRelationInfo, 0, MaxNickName);
		pPlayerRelationInfo += MaxNickName;

		Write32(pPlayerRelationInfo, 0); //EngageTime
		Write32(pPlayerRelationInfo, 0); //NextCanBookingTimeLimit
		Write32(pPlayerRelationInfo, 0); //BeginHoneyMoonTime
		Write32(pPlayerRelationInfo, 0); //EndHoneyMoonTime
		Write8(pPlayerRelationInfo, 0); //EngageFlag 0x4

		len = pPlayerRelationInfo - p;
		Set16(p, (WORD)len);
		p += len;

	}




	{ //CurKartAttr
		BYTE* pCurKartAttr = p;
		Write16(pCurKartAttr, 0); //len
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
		UINT ShapeSuitID = 0;
		UINT LegendSuitLevelChoice = 0;
		UINT LegendSuitLevel = 0;
		UINT ShapeLegendSuitID = 0;
		KartRefitItemInfo* kartRefitItemInfo;
		SelectOneKartRefitItemInfo(DstUin, KartItemId, kartRefitItemInfo);
		if (kartRefitItemInfo) {
			RefitCout = kartRefitItemInfo->RefitCout;
			MaxFlags = kartRefitItemInfo->MaxFlags;
			WWeight = kartRefitItemInfo->WWeight;
			SpeedWeight = kartRefitItemInfo->SpeedWeight;
			JetWeight = kartRefitItemInfo->JetWeight;
			SJetWeight = kartRefitItemInfo->SJetWeight;
			AccuWeight = kartRefitItemInfo->AccuWeight;
			AntiCollisionWeight = kartRefitItemInfo->AntiCollisionWeight;
			ShapeRefitCount = kartRefitItemInfo->ShapeRefitCount;
			KartHeadRefitItemID = kartRefitItemInfo->KartHeadRefitItemID;
			KartTailRefitItemID = kartRefitItemInfo->KartTailRefitItemID;
			KartFlankRefitItemID = kartRefitItemInfo->KartFlankRefitItemID;
			KartTireRefitItemID = kartRefitItemInfo->KartTireRefitItemID;
			ShapeSuitID = kartRefitItemInfo->ShapeSuitID;
			Speed2Weight = kartRefitItemInfo->Speed2Weight;
			DriftVecWeight = kartRefitItemInfo->DriftVecWeight;
			AdditionalZSpeedWeight = kartRefitItemInfo->AdditionalZSpeedWeight;
			ShapeLegendSuitID = kartRefitItemInfo->ShapeLegendSuitID;
			LegendSuitLevel = kartRefitItemInfo->LegendSuitLevel;
			LegendSuitLevelChoice = kartRefitItemInfo->LegendSuitLevelChoice;
			delete[] kartRefitItemInfo;
		}

		Write32(pCurKartAttr, DstUin); //Uin
		Write32(pCurKartAttr, KartItemId); //KartId
		Write32(pCurKartAttr, RefitCout); //RefitCout
		Write16(pCurKartAttr, MaxFlags); //MaxFlags
		Write16(pCurKartAttr, WWeight); //WWeight
		Write16(pCurKartAttr, SpeedWeight); //SpeedWeight
		Write16(pCurKartAttr, JetWeight); //JetWeight
		Write16(pCurKartAttr, SJetWeight); //SJetWeight
		Write16(pCurKartAttr, AccuWeight); //AccuWeight
		Write32(pCurKartAttr, ShapeRefitCount); //ShapeRefitCount
		Write32(pCurKartAttr, KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pCurKartAttr, KartTailRefitItemID); //KartTailRefitItemID
		Write32(pCurKartAttr, KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pCurKartAttr, KartTireRefitItemID); //KartTireRefitItemID
		{ //KartRefitExInfo
			BYTE* pKartRefitExInfo = pCurKartAttr;
			Write16(pKartRefitExInfo, 0); //len

			Write8(pKartRefitExInfo, 0); //SpeedRefitStar
			Write8(pKartRefitExInfo, 0); //JetRefitStar
			Write8(pKartRefitExInfo, 0); //SJetRefitStar
			Write8(pKartRefitExInfo, 0); //AccuRefitStar
			Write8(pKartRefitExInfo, 0); //SpeedAddRatio
			Write8(pKartRefitExInfo, 0); //JetAddRatio
			Write8(pKartRefitExInfo, 0); //SJetAddRatio
			Write8(pKartRefitExInfo, 0); //AccuAddRatio

			len = pKartRefitExInfo - pCurKartAttr;
			Set16(pCurKartAttr, (WORD)len);
			pCurKartAttr += len;
		}
#ifndef ZingSpeed
		Write32(pCurKartAttr, 0); //SecondRefitCount
		Write16(pCurKartAttr, Speed2Weight); //Speed2Weight
		Write16(pCurKartAttr, DriftVecWeight); //DriftVecWeight
		Write16(pCurKartAttr, AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pCurKartAttr, AntiCollisionWeight); //AntiCollisionWeight
		Write16(pCurKartAttr, 0); //LuckyValue
		Write16(pCurKartAttr, 0); //RefitLuckyValueMaxWeight
		Write32(pCurKartAttr, ShapeSuitID); //ShapeSuitID
		Write8(pCurKartAttr, LegendSuitLevel); //LegendSuitLevel
		Write32(pCurKartAttr, LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pCurKartAttr, ShapeLegendSuitID); //ShapeLegendSuitID
#endif
		len = pCurKartAttr - p;
		Set16(p, (WORD)len);
		p += len;
	}


	std::vector<int> conditionValues;
	conditionValues.push_back(DstUin);
	std::vector<int> resultRow = executeQueryRow("SELECT P.PetID,P.Status,P.StrengLevel FROM Pet P join Item I on P.PetID=I.ItemID and P.Uin=I.Uin where I.Status=1 and I.Uin=?;", "ResponseEngageRoleInfo", conditionValues);
	if (resultRow.size() >= 3) {
		Write16(p, 1); //PetNum responsefindplayerbyqq
		{ //PetInfo
				//国服宠物数据结构
			BYTE* pPetInfo = p;
			Write16(pPetInfo, 0); //len
			Write32(pPetInfo, DstUin); //Uin

			Write32(pPetInfo, resultRow[0]); //PetId黄金神兽  
			memset(pPetInfo, 0, 17);
			memcpy(pPetInfo, "宠物", 5);
			pPetInfo += 17;
			Write32(pPetInfo, 0);
			Write32(pPetInfo, 0);
			Write8(pPetInfo, 0);

			Write8(pPetInfo, resultRow[1]);//当前宠物形态 黄金神兽 最高4级 

			Write16(pPetInfo, 0x00);//SkillNum

			Write32(pPetInfo, resultRow[2]); //StrengLevel强化等级 个人信息宠物图标有+5
			Write8(pPetInfo, 0x4); //TopStatus 变形形态
			//变形功能大于1开启
			{ //CommonInfo
				BYTE* pCommonInfo = pPetInfo;
				Write16(pCommonInfo, 0); //len

				Write32(pCommonInfo, 0); //HuanHuaLevel幻化等级
				Write8(pCommonInfo, 1); //CanHuanHua

				len = pCommonInfo - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			len = pPetInfo - p;
			Set16(p, (WORD)len);

			p += len;
		}
	}
	else {
		Write16(p, 0);
	}

	Write8(p, 0); //IsInTopList
	{// KartStoneGrooveInfo
		BYTE* pKartStoneGrooveInfo = p;
		Write16(pKartStoneGrooveInfo, 0); // len
		Write32(pKartStoneGrooveInfo, 0); // KartID
		BYTE* pStoneGrooveNum = pKartStoneGrooveInfo;
		Write32(pKartStoneGrooveInfo, 0); // StoneGrooveNum

		for (size_t i = 0; i < 0; i++)
		{ //StoneGrooveInfo
			BYTE* pStoneGrooveInfo = pKartStoneGrooveInfo;
			Write16(pStoneGrooveInfo, 0); //len

			Write32(pStoneGrooveInfo, 0); //StoneUseOccaType
			Write32(pStoneGrooveInfo, 0); //SkillStoneID

			len = pStoneGrooveInfo - pKartStoneGrooveInfo;
			Set16(pKartStoneGrooveInfo, (WORD)len);
			pKartStoneGrooveInfo += len;
		}
		Set32(pStoneGrooveNum, 0);
		len = pKartStoneGrooveInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write8(p, 0); //Contex

	{ //PlayerWlVisibleInfo
		BYTE* pPlayerWlVisibleInfo = p;
		Write16(pPlayerWlVisibleInfo, 0); //len

		Write32(pPlayerWlVisibleInfo, 0); //WonderLandID

		//WonderLandName[]
		memset(pPlayerWlVisibleInfo, 0, MaxNickName);
		pPlayerWlVisibleInfo += MaxNickName;

		Write16(pPlayerWlVisibleInfo, 0); //WonderLandDuty
		Write8(pPlayerWlVisibleInfo, 0); //SubType
		Write8(pPlayerWlVisibleInfo, 0); //GuildGrade

		len = pPlayerWlVisibleInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write32(p, 0); //SkateCoin
	Write32(p, 0); //MaxDayPveScore
	Write32(p, 0); //MaxHistoryPveScore

	{ //EquipedCarBattleModeSkillInfoList
		BYTE* pEquipedCarBattleModeSkillInfoList = p;
		Write16(pEquipedCarBattleModeSkillInfoList, 0); //len

		Write8(pEquipedCarBattleModeSkillInfoList, 0); //SkillNum


		len = pEquipedCarBattleModeSkillInfoList - p;
		Set16(p, (WORD)len);
		p += len;
	}
#ifndef ZingSpeed
	Write8(p, 0); //HasCCVisibleInfo


	Write8(p, 0); //HasCheerAddition


	Write32(p, 0); //LoveValue
	Write16(p, 0); //CanEnterChannelFalg[]
	Write8(p, 0); //SpecialActivityID
	Write32(p, 0); //ThemeHouseDressDegreeValue
	Write8(p, 0); //AreaIdDst
	Write8(p, 0); //SpecialActivityIDNum
	Write8(p, 0); //SimpleInfo
#endif

	len = p - buf;
	SendToClient(Client, 122, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestFindPlayerByQQ(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	//PrintfBody(Client, 1, Body, BodyLen);
	BYTE* p = Body;;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);

	char NickName[MaxNickName] = {};
	memcpy(NickName, p, MaxNickName);
	p += MaxNickName;

	UINT DstUin = Read32(p);
	UINT DstNobleID = Read32(p);
	UCHAR Contex = Read8(p);
#ifndef ZingSpeed
	UCHAR AreaIdDst = Read8(p);
	UCHAR IgnoreByQQ2Msg = Read8(p);
	UCHAR SimpleInfo = Read8(p);
#endif

	if (DstUin < 10000)
	{
		DstUin += 10000;
	}

	ResponseFindPlayerByQQ(Client, DstUin, AreaIdDst, SimpleInfo, NickName);

}

void ResponseFindPlayerByQQ(std::shared_ptr<ClientNode> Client, UINT DstUin, UCHAR AreaIdDst, UCHAR SimpleInfo, char* NickName)
{

	if (strcmp(NickName, "") != 0) //根据昵称查找玩家
	{
		//printf("根据昵称查找玩家=====");
		char* utf8NickName = G2U(NickName);

		std::vector<std::string> conditionValues;
		conditionValues.push_back(utf8NickName);
		std::vector<int> resultRow = executeQueryRow("SELECT Uin FROM BaseInfo WHERE NickName=?;", "ResponseFindPlayerByQQ", conditionValues);

		if (utf8NickName) {
			delete[] utf8NickName;
			utf8NickName = NULL;
		}

		if (resultRow.size() >= 1) {
			DstUin = resultRow[0];
		}
		else  //未查到
		{
			ResponseStartGame(Client, "输入的昵称不存在！");
			return;
		}
	}
	else {

		if (!UserDB_GetUin(DstUin)) {
			ResponseStartGame(Client, "查找玩家不存在！");
			return;
		}
	}
	if (DstUin == Client->Uin) {
		ResponseStartGame(Client, "对不起，查询对象不能是自己");
		return;
	}

	std::shared_ptr<ClientNode> Dst = GetClient(DstUin);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	UINT KartItemId = 0;

	BOOL findresult;
	
	
	int sqlresult;
	int Identity = ID_IDENTIFY_QQLEVEL1 | ID_IDENTIFY_QQLEVEL2 | ID_IDENTIFY_QQLEVEL3 | ID_IDENTIFY_SPEEDMEMBER | ID_IDENTIFY_QQFLAG | ID_IDENTIFY_HAVEWORD | ID_IDENTIFY_FINISHQQFLAG;

	UINT EngageUin = 0;
	UINT Baby1EquipStat = 0;
	UINT Baby2EquipStat = 0;

	UINT NobleID = 0;
	UINT NobleLevel = 0;
	UINT NoblePoint = 0;
	UINT GuildVipLevel = 0;
	UINT GuildVipPoint = 0;
	UINT LoverVipLevel = 0;
	UINT LoverVipPoint = 0;
	UINT EmperorLevel = 0;
	UINT EmperorPoint = 0;
	UINT GardenLevel = 0;
	UINT GardenPoint = 0;
	int AppellationType = SelectAppellationType(DstUin);
	std::vector<int> conditionValues;
	conditionValues.push_back(DstUin);
	std::vector<int> resultRow = executeQueryRow("SELECT EngageUin,Baby1EquipStat,Baby2EquipStat FROM EngageInfo WHERE Uin = ?;", "ResponseFindPlayerByQQ", conditionValues);
	if (resultRow.size() >= 3) {
		EngageUin = resultRow[0];
		Baby1EquipStat = resultRow[1];
		Baby2EquipStat = resultRow[2];
	}

	std::vector<PlayerInfo> playerInfo;
	PlayerDB_SelectPlayerInfo(DstUin, playerInfo);
	if (playerInfo.size() > 0) {
		NobleID = playerInfo[0].NobleID;
		NobleLevel = playerInfo[0].NobleLevel;
		NoblePoint = playerInfo[0].NoblePoint;
		GuildVipLevel = playerInfo[0].GuildVipLevel;
		GuildVipPoint = playerInfo[0].GuildVipPoint;
		LoverVipLevel = playerInfo[0].LoverVipLevel;
		LoverVipPoint = playerInfo[0].LoverVipPoint;
		EmperorLevel = playerInfo[0].EmperorLevel;
		EmperorPoint = playerInfo[0].EmperorPoint;
		GardenLevel = playerInfo[0].GardenLevel;
		GardenPoint = playerInfo[0].GardenPoint;
	}
	else {
		return;
	}

	{//贵族等级优化
		//if (VipFlag > 7) {
		//	VipFlag = 7;
		//}
		//if (VipLevel > 9) {
		//	VipLevel = 9;
		//}
		if (NobleLevel > 7) {
			NobleLevel = 7;
		}
	}
	Write16(p, 0); //ResultID
	{ //PlayerInfo
		BYTE* pPlayerInfo = p;
		Write16(pPlayerInfo, 0); //len

		Write32(pPlayerInfo, DstUin); //Uin
		if (HonorTopList[0] == DstUin)
		{
			Identity |= ID_IDENTIFY_KING;
		}
		else if (HonorTopList[1] == DstUin || HonorTopList[2] == DstUin)
		{
			Identity |= ID_IDENTIFY_INFANTE;
		}
		Write32(pPlayerInfo, Identity); //Identity
		Write16(pPlayerInfo, 0); //PlayerID

		findresult = WritePlayerDBBaseInfo(pPlayerInfo, DstUin, Client);
		if (findresult == FALSE)
		{
			return;
		}
		WritePlayerGuildInfo(pPlayerInfo, DstUin, Client);
		KartItemId = WriteEquipedItem2(pPlayerInfo, DstUin, Client);

		Write32(pPlayerInfo, 0); //MasterPoint
		Write32(pPlayerInfo, 0); //TotalGuildProsperity

		Write16(pPlayerInfo, 0x0107); //VipFlag
		Write16(pPlayerInfo, 10); //VipGrowRate

		Write8(pPlayerInfo, 1); //AppellationNum
		for (size_t i = 0; i < 1; i++)
		{ // Record
			BYTE* pRecord = pPlayerInfo;
			Write16(pRecord, 0); // len

			Write8(pRecord, AppellationType);					   // Type
			Write8(pRecord, GetAppellationLevel(AppellationType)); // Level
			Write8(pRecord, 1);									   // Status
			Write8(pRecord, 0);									   // Difficulty
			Write32(pRecord, 0);								   // MapId
			Write32(pRecord, 0);								   // Value

			len = pRecord - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		{ //NobleInfo
			BYTE* pNobleInfo = pPlayerInfo;
			Write16(pNobleInfo, 0); //len

			Write32(pNobleInfo, NobleID); //NobleID
			Write8(pNobleInfo, NobleLevel); //NobleLevel
			Write32(pNobleInfo, NoblePoint); //NoblePoint
			Write32(pNobleInfo, -1); //NobleLeftDays

			len = pNobleInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write8(pPlayerInfo, 0); //HasCarryWizard


#ifndef ZingSpeed
		{ //GuildVipBaseInfo
			BYTE* pGuildVipBaseInfo = pPlayerInfo;
			Write16(pGuildVipBaseInfo, 0); //len

			Write8(pGuildVipBaseInfo, GuildVipLevel); //GuildVipLevel
			Write32(pGuildVipBaseInfo, GuildVipPoint); //GuildVipPoint

			len = pGuildVipBaseInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
#endif

		Write8(pPlayerInfo, 0); //HasLDMInfo


#ifndef ZingSpeed
		Write8(pPlayerInfo, 1); //HasLoverVip

		for (size_t i = 0; i < 1; i++)
		{ //LoverVipInfo
			BYTE* pLoverVipInfo = pPlayerInfo;
			Write16(pLoverVipInfo, 0); //len

			Write8(pLoverVipInfo, LoverVipLevel); //LoverVipLevel
			Write32(pLoverVipInfo, LoverVipPoint); //LoverVipPoint
			Write8(pLoverVipInfo, 10); //GrowRate

			len = pLoverVipInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

#endif

		Write8(pPlayerInfo, 0); //IsShowMounts

#ifndef ZingSpeed
		Write8(pPlayerInfo, 1); //HasGarden

		{ //PersonalGardenBaseInfo
			BYTE* pPersonalGardenBaseInfo = pPlayerInfo;
			Write16(pPersonalGardenBaseInfo, 0); //len

			Write8(pPersonalGardenBaseInfo, GardenLevel); //GardenLevel
			Write32(pPersonalGardenBaseInfo, GardenPoint); //GardenPoint
			Write8(pPersonalGardenBaseInfo, 10); //GrowRate
			Write32(pPersonalGardenBaseInfo, -1); //GardenExpireTime
			{ //SimpleInfo
				BYTE* pSimpleInfo = pPersonalGardenBaseInfo;
				Write16(pSimpleInfo, 0); //len

				Write32(pSimpleInfo, 0); //WeekPopularity
				Write32(pSimpleInfo, 0); //TotalPopularity
				Write32(pSimpleInfo, 0); //LastUpdatePopularityTime
				Write8(pSimpleInfo, 0); //PrivateType

				len = pSimpleInfo - pPersonalGardenBaseInfo;
				Set16(pPersonalGardenBaseInfo, (WORD)len);
				pPersonalGardenBaseInfo += len;
			}

			len = pPersonalGardenBaseInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}


		Write8(pPlayerInfo, 1); //isConsumeVip
		Write32(pPlayerInfo, 9); //ConsumeVipLevel
		Write32(pPlayerInfo, 0); //SearchTreasureNums 
		Write32(pPlayerInfo, 0); //GetTreasureNums 
		Write32(pPlayerInfo, 0); //ConsumeVipCharmVlaueOfMonth 
		{ //EmperorInfo
			BYTE* pEmperorInfo = pPlayerInfo;
			Write16(pEmperorInfo, 0); //len

			Write8(pEmperorInfo, EmperorLevel); //EmperorLevel
			Write32(pEmperorInfo, EmperorPoint); //EmperorPoint
			Write32(pEmperorInfo, -1); //EmperorLeftDays
			Write8(pEmperorInfo, 10); //EmperorGrowRate

			len = pEmperorInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //EmperorOtherInfo
			BYTE* pEmperorOtherInfo = pPlayerInfo;
			Write16(pEmperorOtherInfo, 0); //len

			Write32(pEmperorOtherInfo, 0); //ExpiredTime
			Write8(pEmperorOtherInfo, 0); //ShowExpireTips

			len = pEmperorOtherInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		{ //ActivityInfo
			BYTE* pActivityInfo = pPlayerInfo;
			Write16(pActivityInfo, 0); //len

			Write32(pActivityInfo, 0); //TotalActivity
			Write32(pActivityInfo, 0); //ActivityLevel

			len = pActivityInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write8(pPlayerInfo, 0); //HaveWakedKartAttributeAddInfo
		{ //WakeKartAttributeAddInfo
			BYTE* pWakeKartAttributeAddInfo = pPlayerInfo;
			Write16(pWakeKartAttributeAddInfo, 0); //len

			Write16(pWakeKartAttributeAddInfo, 0); //AccAddValue
			Write16(pWakeKartAttributeAddInfo, 0); //SpeedAddValue
			Write16(pWakeKartAttributeAddInfo, 0); //JetAddValue
			Write16(pWakeKartAttributeAddInfo, 0); //SJetAddValue

			len = pWakeKartAttributeAddInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 1); //HaveLoverBabyInfo
		{ //FindLoverBabyInfo
			BYTE* pFindLoverBabyInfo = pPlayerInfo;
			Write16(pFindLoverBabyInfo, 0); //len

			Write32(pFindLoverBabyInfo, 5); //BabyStatus
			Write32(pFindLoverBabyInfo, 71555); //LoverBabyItemID
			Write32(pFindLoverBabyInfo, 71555); //LoverBabyID
			Write32(pFindLoverBabyInfo, 100); //BabyGrowLevel
			Write32(pFindLoverBabyInfo, 5); //BabyStrengthLevel
			Write16(pFindLoverBabyInfo, 0); //LoverBabyStat
			Write16(pFindLoverBabyInfo, Baby1EquipStat); //LoverBabyEquipStat
			Write16(pFindLoverBabyInfo, 0); //LoverBabyTransferdStatus

			len = pFindLoverBabyInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}


		{ //GansterScoreInfo
			BYTE* pGansterScoreInfo = pPlayerInfo;
			Write16(pGansterScoreInfo, 0); //len

			Write32(pGansterScoreInfo, 0); //GansterSeasonID
			Write32(pGansterScoreInfo, 0); //GansterScore
			Write32(pGansterScoreInfo, 0); //PoliceScore
			Write32(pGansterScoreInfo, 0); //TotalGansterScore

			len = pGansterScoreInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write32(pPlayerInfo, 0); //OlympicId
		Write32(pPlayerInfo, 0); //LastOlympicUpdateTime
		Write32(pPlayerInfo, 0); //NPCEliminateWinTimes
		{ //BorderInfo
			BYTE* pBorderInfo = pPlayerInfo;
			Write16(pBorderInfo, 0); //len

			Write32(pBorderInfo, 1); //SeasonID
			Write32(pBorderInfo, 0); //Zhanxun
			Write32(pBorderInfo, 0); //SeasonZhanxun

			len = pBorderInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}
		Write8(pPlayerInfo, 0); //SpecialActivityID
		Write32(pPlayerInfo, 0); //ThemeHouseCollectValue 0x64
		Write8(pPlayerInfo, 1); //HaveSecondLoverBabyInfo
		{ //SecondLoverBabyInfo
			BYTE* pSecondLoverBabyInfo = pPlayerInfo;
			Write16(pSecondLoverBabyInfo, 0); //len
			Write32(pSecondLoverBabyInfo, 5); //BabyStatus 形态
			Write32(pSecondLoverBabyInfo, 71554); //LoverBabyItemID
			Write32(pSecondLoverBabyInfo, 71554); //LoverBabyID
			Write32(pSecondLoverBabyInfo, 100); //BabyGrowLevel
			Write32(pSecondLoverBabyInfo, 5); //BabyStrengthLevel
			Write16(pSecondLoverBabyInfo, 0); //LoverBabyStat
			Write16(pSecondLoverBabyInfo, Baby2EquipStat); //LoverBabyEquipStat
			Write16(pSecondLoverBabyInfo, 0); //LoverBabyTransferdStatus

			len = pSecondLoverBabyInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}

		Write8(pPlayerInfo, Baby1EquipStat + Baby2EquipStat); //EquipBaby
		Write8(pPlayerInfo, 0); //ActiveStatus
		Write32(pPlayerInfo, 0); //HelperLev


		conditionValues.clear();
		resultRow.clear();
		conditionValues.push_back(DstUin);
		resultRow = executeQueryRow("SELECT GradeLevel,ChildLevel FROM RankInfo Where Uin=?;", "ResponseFindPlayerByQQ", conditionValues);
		if (resultRow.size() >= 2) {
			int leveltemp = resultRow[0];
			int childlevel = resultRow[1];
			if (leveltemp > 7)
			{
				leveltemp = 7;
			}
			else if (leveltemp >= 6)
			{
				childlevel = 1;
			}
			Write8(pPlayerInfo, 1); //HasRankedMatchInfo
			for (size_t i = 0; i < 1; i++)
			{ //RankedMatchGradeInfo
				BYTE* pRankedMatchGradeInfo = pPlayerInfo;
				Write16(pRankedMatchGradeInfo, 0); //len

				Write32(pRankedMatchGradeInfo, RankGradeLevel[leveltemp]); //GradeLevel
				Write32(pRankedMatchGradeInfo, childlevel); //ChildLevel
				Write32(pRankedMatchGradeInfo, 0); //GradeScore
				Write32(pRankedMatchGradeInfo, 0); //MaxGradeScore
				Write32(pRankedMatchGradeInfo, 0); //TotalScore

				len = pRankedMatchGradeInfo - pPlayerInfo;
				Set16(pPlayerInfo, (WORD)len);
				pPlayerInfo += len;
			}
			Write16(pPlayerInfo, 1); //PlayerSeasonInfoNums
			for (size_t i = 0; i < 1; i++)
			{ //PlayerSeasonInfos
				BYTE* pPlayerSeasonInfos = pPlayerInfo;
				Write16(pPlayerSeasonInfos, 0); //len

				Write16(pPlayerSeasonInfos, 0); //SeasonID
				Write16(pPlayerSeasonInfos, RankGradeLevel[leveltemp]); //GradeLevel
				Write8(pPlayerSeasonInfos, childlevel); //ChildLevel
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

				len = pPlayerSeasonInfos - pPlayerInfo;
				Set16(pPlayerInfo, (WORD)len);
				pPlayerInfo += len;
			}
		}
		else {
			Write8(pPlayerInfo, 0); //HasRankedMatchInfo
			Write16(pPlayerInfo, 0); //PlayerSeasonInfoNums
		}

		Write8(pPlayerInfo, 0); //IsHelperAppOnline
		Write32(pPlayerInfo, 0); //RankedMatchSpeedGodNums
		Write8(pPlayerInfo, 0); //EquipSealType

		Write8(pPlayerInfo, 0); //HasHuanLingChangeInfo


		Write8(pPlayerInfo, 0); //SpecialActivityIDNum
		Write8(pPlayerInfo, 0); //PersonalPanelSelectRankedMatchFrameTag

		Write8(pPlayerInfo, 1); //SpecialActivityInfoNum
		for (size_t i = 0; i < 1; i++)
		{ //SpecialActivityInfo
			BYTE* pSpecialActivityInfo = pPlayerInfo;
			Write16(pSpecialActivityInfo, 0); //len

			Write8(pSpecialActivityInfo, 1); //ProfessionLicenseInfoNum
			for (size_t i = 0; i < 1; i++)
			{ //ProfessionLicenseInfo
				BYTE* pProfessionLicenseInfo = pSpecialActivityInfo;
				Write16(pProfessionLicenseInfo, 0); //len

				Write8(pProfessionLicenseInfo, 6); //BigLevel
				Write8(pProfessionLicenseInfo, 5); //SmallLevel

				len = pProfessionLicenseInfo - pSpecialActivityInfo;
				Set16(pSpecialActivityInfo, (WORD)len);
				pSpecialActivityInfo += len;
			}

			len = pSpecialActivityInfo - pPlayerInfo;
			Set16(pPlayerInfo, (WORD)len);
			pPlayerInfo += len;
		}


		Write8(pPlayerInfo, 0); //PersonalRankedMatchLevelShowTag

		Write8(pPlayerInfo, 0); //ItemNum


		Write8(pPlayerInfo, 0); //ExRightFlagLen

		Write8(pPlayerInfo, 0); //EquipNiChangItemNum

#endif
		len = pPlayerInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	if (GetClient(DstUin))
	{
		Write8(p, 1); //IsOnLine
		{			  // PositionInfo
			BYTE* pPositionInfo = p;
			Write16(pPositionInfo, 0); // len

			Write8(pPositionInfo, 0);  // ServerType
			Write16(pPositionInfo, 1); // Status
			Write32(pPositionInfo, GetClient(DstUin)->ServerID); // ServerID
			std::shared_ptr<RoomNode> Room = GetRoom(Dst->RoomID);
			if (Room)
			{
				{
					// RoomInfo
					BYTE* pRoomInfo = pPositionInfo;
					Write16(pRoomInfo, 0); // len


					Write16(pRoomInfo, Room->ID); // RoomID
					Write16(pRoomInfo, Room->BaseGameMode); // BaseGameMode
					Write16(pRoomInfo, Room->SubGameMode); // SubGameMode
					Write8(pRoomInfo, Room->CurrentPlayerNum);  // CurrentPlayerNum
					Write8(pRoomInfo, Room->SeatNum);  // TotalSeatNum
					Write8(pRoomInfo, Room->startStatus);  // Status
					Write32(pRoomInfo, Room->MapID); // MapID

					Write8(pRoomInfo, Room->Flag);  // Flag

					// RoomName[]
					WriteString(pRoomInfo, "多人游戏");

					Write8(pRoomInfo, 0); // HasFBInfo

					Write8(pRoomInfo, 10);  // SpeFlag
					Write32(pRoomInfo, 0); // RoomNo
					Write16(pRoomInfo, 0); // SvrId
					Write16(pRoomInfo, Room->SceneID); // SceneID
					Write8(pRoomInfo, Room->CurrentPlayerNum);  // CurrentOBNum
					Write8(pRoomInfo, Room->CurrentPlayerNum);  // TotalOBNum
					Write8(pRoomInfo, 0);  // SpecialActivityStatus
					Write8(pRoomInfo, Room->AllowAutoStart);  // AllowAutoStart
					Write32(pRoomInfo, 0); // Gender
					Write32(pRoomInfo, 0); // LocaleCode
					Write8(pRoomInfo, 0);  // TalkRoomBGID
					Write8(pRoomInfo, 0);  // SpecialActivityIDNum

					Write8(pRoomInfo, 0); // SpecialActivityInfoNum

					for (size_t i = 0; i < 0; i++)
					{ //SpecialActivityInfo
						BYTE* pSpecialActivityInfo = pRoomInfo;
						Write16(pSpecialActivityInfo, 0); //len

						Write8(pSpecialActivityInfo, 1); //ProfessionLicenseInfoNum
						for (size_t i = 0; i < 1; i++)
						{ //ProfessionLicenseInfo
							BYTE* pProfessionLicenseInfo = pSpecialActivityInfo;
							Write16(pProfessionLicenseInfo, 0); //len

							Write8(pProfessionLicenseInfo, 6); //BigLevel
							Write8(pProfessionLicenseInfo, 5); //SmallLevel

							len = pProfessionLicenseInfo - pSpecialActivityInfo;
							Set16(pSpecialActivityInfo, (WORD)len);
							pSpecialActivityInfo += len;
						}

						len = pSpecialActivityInfo - pRoomInfo;
						Set16(pRoomInfo, (WORD)len);
						pRoomInfo += len;
					}


					Write8(pRoomInfo, 0); // ValueNum
					Write32(pRoomInfo, 0); //EnterRoomValue[]
					Write8(pRoomInfo, 0); // ParaNum
					Write32(pRoomInfo, 0); //ParaList[]
					Write32(pRoomInfo, 0); // GSvrRoomNo
					Write8(pRoomInfo, 0);	// OwnerName[]

					len = pRoomInfo - pPositionInfo;
					Set16(pPositionInfo, (WORD)len);
					pPositionInfo += len;
				}
				if (GetClient(DstUin)->TaskID == 0xFFFC)
				{
					WriteString_MC(pPositionInfo, "荣耀排位赛中");
					Write8(pPositionInfo, 0);
				}
				else if (Room->startStatus == 1)
				{
					WriteString_MC(pPositionInfo, "多人游戏房间");
					Write8(pPositionInfo, 1);
				}
				else if (Room->startStatus == 2)
				{
					WriteString_MC(pPositionInfo, "多人游戏中");
					Write8(pPositionInfo, 0);
				}
				else
				{
					Write8(pPositionInfo, 0);
					Write8(pPositionInfo, 0);
				}
			}
			else
			{
				// RoomInfo
				BYTE* pRoomInfo = pPositionInfo;
				Write16(pRoomInfo, 0); // len


				Write16(pRoomInfo, Dst->RoomID); // RoomID
				Write16(pRoomInfo, 0); // BaseGameMode
				Write16(pRoomInfo, 0); // SubGameMode
				Write8(pRoomInfo, 0);  // CurrentPlayerNum
				Write8(pRoomInfo, 0);  // TotalSeatNum
				Write8(pRoomInfo, 0);  // Status
				Write32(pRoomInfo, 0); // MapID
				Write8(pRoomInfo, 0);  // Flag

				// RoomName[]
				WriteString(pRoomInfo, "多人游戏");

				Write8(pRoomInfo, 0); // HasFBInfo
#ifndef ZingSpeed
				Write8(pRoomInfo, 0);  // SpeFlag
				Write32(pRoomInfo, 0); // RoomNo
				Write16(pRoomInfo, 0); // SvrId
				Write16(pRoomInfo, 0); // SceneID
				Write8(pRoomInfo, 0);  // CurrentOBNum
				Write8(pRoomInfo, 0);  // TotalOBNum
				Write8(pRoomInfo, 0);  // SpecialActivityStatus
				Write8(pRoomInfo, 0);  // AllowAutoStart
				Write32(pRoomInfo, 0); // Gender
				Write32(pRoomInfo, 0); // LocaleCode
				Write8(pRoomInfo, 0);  // TalkRoomBGID
				Write8(pRoomInfo, 0);  // SpecialActivityIDNum

				Write8(pRoomInfo, 0); // SpecialActivityInfoNum

				for (size_t i = 0; i < 0; i++)
				{ //SpecialActivityInfo
					BYTE* pSpecialActivityInfo = pRoomInfo;
					Write16(pSpecialActivityInfo, 0); //len

					Write8(pSpecialActivityInfo, 1); //ProfessionLicenseInfoNum
					for (size_t i = 0; i < 1; i++)
					{ //ProfessionLicenseInfo
						BYTE* pProfessionLicenseInfo = pSpecialActivityInfo;
						Write16(pProfessionLicenseInfo, 0); //len

						Write8(pProfessionLicenseInfo, 6); //BigLevel
						Write8(pProfessionLicenseInfo, 5); //SmallLevel

						len = pProfessionLicenseInfo - pSpecialActivityInfo;
						Set16(pSpecialActivityInfo, (WORD)len);
						pSpecialActivityInfo += len;
					}

					len = pSpecialActivityInfo - pRoomInfo;
					Set16(pRoomInfo, (WORD)len);
					pRoomInfo += len;
				}


				Write8(pRoomInfo, 0); // ValueNum
				Write32(pRoomInfo, 0); //EnterRoomValue[]
				Write8(pRoomInfo, 0); // ParaNum
				Write32(pRoomInfo, 0); //ParaList[]
				Write32(pRoomInfo, Dst->RoomID); // GSvrRoomNo

				// OwnerName[]
				memset(pRoomInfo, 0, MaxNickName);
				pRoomInfo += MaxNickName;
#endif
				len = pRoomInfo - pPositionInfo;
				Set16(pPositionInfo, (WORD)len);
				pPositionInfo += len;
				Write8(pPositionInfo, 0);
				Write8(pPositionInfo, 0);
			}
			len = pPositionInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	else
	{
		Write8(p, 0); //IsOnLine
		{ //PositionInfo
			BYTE* pPositionInfo = p;
			Write16(pPositionInfo, 0); //len

			Write8(pPositionInfo, 0); //ServerType
			Write16(pPositionInfo, 0); //Status
			Write32(pPositionInfo, 0); //ServerID
			{ //RoomInfo
				BYTE* pRoomInfo = pPositionInfo;
				Write16(pRoomInfo, 0); //len

				Write16(pRoomInfo, 0); //RoomID
				Write16(pRoomInfo, 0); //BaseGameMode
				Write16(pRoomInfo, 0); //SubGameMode
				Write8(pRoomInfo, 0); //CurrentPlayerNum
				Write8(pRoomInfo, 0); //TotalSeatNum
				Write8(pRoomInfo, 0); //Status
				Write32(pRoomInfo, 0); //MapID
				Write16(pRoomInfo, 0); //Flag

				//RoomName[]
				memset(pRoomInfo, 0, MaxRoomName);
				pRoomInfo += MaxRoomName;

				Write8(pRoomInfo, 0); //HasFBInfo
				/*
				{ //FBInfo
					BYTE* pFBInfo = pRoomInfo;
					Write16(pFBInfo, 0); //len

					Write8(pFBInfo, 0); //AvailableLev
					Write8(pFBInfo, 0); //Lev
					Write8(pFBInfo, 0); //SubLev
					Write32(pFBInfo, 0); //BossID
					Write8(pFBInfo, 0); //FBSeason

					len = pFBInfo - pRoomInfo;
					Set16(pRoomInfo, (WORD)len);
					pRoomInfo += len;
				}
				*/
#ifndef ZingSpeed
				Write8(pRoomInfo, 0); //SpeFlag
				Write32(pRoomInfo, 0); //RoomNo
				Write16(pRoomInfo, 0); //SvrId
				Write16(pRoomInfo, 0); //SceneID
				Write8(pRoomInfo, 0); //CurrentOBNum
				Write8(pRoomInfo, 0); //TotalOBNum
				Write8(pRoomInfo, 0); //SpecialActivityStatus
				Write8(pRoomInfo, 0); //AllowAutoStart
				Write32(pRoomInfo, 0); //Gender
				Write32(pRoomInfo, 0); //LocaleCode
				Write8(pRoomInfo, 0); //TalkRoomBGID
				Write8(pRoomInfo, 0); //SpecialActivityIDNum

				Write8(pRoomInfo, 0); //SpecialActivityInfoNum


				Write8(pRoomInfo, 0); //ValueNum
				//Write32(pRoomInfo, 0); //EnterRoomValue[]
				Write8(pRoomInfo, 0); //ParaNum
				//Write32(pRoomInfo, 0); //ParaList[]
				Write32(pRoomInfo, 0); //GSvrRoomNo

				//OwnerName[]
				memset(pRoomInfo, 0, MaxNickName);
				pRoomInfo += MaxNickName;
#endif
				len = pRoomInfo - pPositionInfo;
				Set16(pPositionInfo, (WORD)len);
				pPositionInfo += len;
			}
			Write8(pPositionInfo, 0); //StatusLen
			Write8(pPositionInfo, 1); //CanJoin

			len = pPositionInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}

	Write8(p, 0); //ReasonLen

	{ //PlayerRelationInfo

		UINT EngageUin = 0;

		std::string newname = "";
		conditionValues.clear();
		resultRow.clear();
		conditionValues.push_back(DstUin);
		std::vector<int> resultRow = executeQueryRow("SELECT EngageUin FROM EngageInfo WHERE Uin = ?;", "ResponseFindPlayerByQQ", conditionValues);
		if (resultRow.size() >= 1) {
			EngageUin = resultRow[0];
		}
		if (EngageUin != 0) {
			PlayerDB_SelectNickName(EngageUin, newname);
		}

		BYTE* pPlayerRelationInfo = p;
		Write16(pPlayerRelationInfo, 0); //len

		Write32(pPlayerRelationInfo, DstUin); //SrcUin
		if (EngageUin != 0)
		{
			Write32(pPlayerRelationInfo, 0x3); //RelationFlag
		}
		else
		{
			Write32(pPlayerRelationInfo, 0); //RelationFlag
		}

		//Write32(pPlayerRelationInfo, EngageUin); //RelationUin 0x0
		Write32(pPlayerRelationInfo, 0); //RelationUin 0x0


		Write8(pPlayerRelationInfo, 17);//NickNameLen
		memset(pPlayerRelationInfo, 0, MaxNickName);
		//RelationNickName[]
		if (!newname.empty())
		{
			memcpy(pPlayerRelationInfo, newname.c_str(), newname.length());
		}

		pPlayerRelationInfo += MaxNickName;

		Write32(pPlayerRelationInfo, 0); //EngageTime
		Write32(pPlayerRelationInfo, 0); //NextCanBookingTimeLimit
		Write32(pPlayerRelationInfo, 0); //BeginHoneyMoonTime
		Write32(pPlayerRelationInfo, 0); //EndHoneyMoonTime
		Write8(pPlayerRelationInfo, 0x4); //EngageFlag 0x4

		len = pPlayerRelationInfo - p;
		Set16(p, (WORD)len);
		p += len;

		//if (Client->Uin != EngageUin)  //当查询者和被查询者不是情侣关系时（若是情侣关系，则不需要响应自己的角色数据，因为本机已存在自己的角色数据）
		//{
		//	//响应伴侣角色数据
		//	ResponseEngageRoleInfo(Client, EngageUin);
		//}
		//else
		//{
		//	ResponseEngageRoleInfo(Client, Client->Uin);
		//}

	}
	{ //CurKartAttr
		BYTE* pCurKartAttr = p;
		Write16(pCurKartAttr, 0); //len



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
		UINT ShapeSuitID = 0;
		UINT ShapeLegendSuitID = 0;
		UINT LegendSuitLevel = 0;
		UINT LegendSuitLevelChoice = 0;

		KartRefitItemInfo* kartRefitItemInfo;
		SelectOneKartRefitItemInfo(DstUin, KartItemId, kartRefitItemInfo);
		if (kartRefitItemInfo) {
			RefitCout = kartRefitItemInfo->RefitCout;
			MaxFlags = kartRefitItemInfo->MaxFlags;
			WWeight = kartRefitItemInfo->WWeight;
			SpeedWeight = kartRefitItemInfo->SpeedWeight;
			JetWeight = kartRefitItemInfo->JetWeight;
			SJetWeight = kartRefitItemInfo->SJetWeight;
			AccuWeight = kartRefitItemInfo->AccuWeight;
			AntiCollisionWeight = kartRefitItemInfo->AntiCollisionWeight;
			ShapeRefitCount = kartRefitItemInfo->ShapeRefitCount;
			KartHeadRefitItemID = kartRefitItemInfo->KartHeadRefitItemID;
			KartTailRefitItemID = kartRefitItemInfo->KartTailRefitItemID;
			KartFlankRefitItemID = kartRefitItemInfo->KartFlankRefitItemID;
			KartTireRefitItemID = kartRefitItemInfo->KartTireRefitItemID;
			ShapeSuitID = kartRefitItemInfo->ShapeSuitID;
			Speed2Weight = kartRefitItemInfo->Speed2Weight;
			DriftVecWeight = kartRefitItemInfo->DriftVecWeight;
			AdditionalZSpeedWeight = kartRefitItemInfo->AdditionalZSpeedWeight;
			ShapeLegendSuitID = kartRefitItemInfo->ShapeLegendSuitID;
			LegendSuitLevel = kartRefitItemInfo->LegendSuitLevel;
			LegendSuitLevelChoice = kartRefitItemInfo->LegendSuitLevelChoice;
			delete[] kartRefitItemInfo;
		}

		Write32(pCurKartAttr, DstUin); //Uin
		Write32(pCurKartAttr, KartItemId); //KartId
		Write32(pCurKartAttr, RefitCout); //RefitCout
		Write16(pCurKartAttr, MaxFlags); //MaxFlags
		//Write16(pCurKartAttr, 0xf880); //MaxFlags
		//Write16(pCurKartAttr, 0x4800); //MaxFlags
		Write16(pCurKartAttr, WWeight); //WWeight
		Write16(pCurKartAttr, SpeedWeight); //SpeedWeight
		Write16(pCurKartAttr, JetWeight); //JetWeight
		Write16(pCurKartAttr, SJetWeight); //SJetWeight
		Write16(pCurKartAttr, AccuWeight); //AccuWeight
		Write32(pCurKartAttr, ShapeRefitCount); //ShapeRefitCount
		Write32(pCurKartAttr, KartHeadRefitItemID); //KartHeadRefitItemID
		Write32(pCurKartAttr, KartTailRefitItemID); //KartTailRefitItemID
		Write32(pCurKartAttr, KartFlankRefitItemID); //KartFlankRefitItemID
		Write32(pCurKartAttr, KartTireRefitItemID); //KartTireRefitItemID
		{ //KartRefitExInfo
			BYTE* pKartRefitExInfo = pCurKartAttr;
			Write16(pKartRefitExInfo, 0); //len

			Write8(pKartRefitExInfo, 0); //SpeedRefitStar
			Write8(pKartRefitExInfo, 0); //JetRefitStar
			Write8(pKartRefitExInfo, 0); //SJetRefitStar
			Write8(pKartRefitExInfo, 0); //AccuRefitStar
			Write8(pKartRefitExInfo, 0); //SpeedAddRatio
			Write8(pKartRefitExInfo, 0); //JetAddRatio
			Write8(pKartRefitExInfo, 0); //SJetAddRatio
			Write8(pKartRefitExInfo, 0); //AccuAddRatio

			len = pKartRefitExInfo - pCurKartAttr;
			Set16(pCurKartAttr, (WORD)len);
			pCurKartAttr += len;
		}
#ifndef ZingSpeed
		Write32(pCurKartAttr, 0); //SecondRefitCount
		Write16(pCurKartAttr, Speed2Weight); //Speed2Weight
		Write16(pCurKartAttr, DriftVecWeight); //DriftVecWeight
		Write16(pCurKartAttr, AdditionalZSpeedWeight); //AdditionalZSpeedWeight
		Write16(pCurKartAttr, AntiCollisionWeight); //AntiCollisionWeight
		Write16(pCurKartAttr, 0); //LuckyValue
		Write16(pCurKartAttr, 0); //RefitLuckyValueMaxWeight
		Write32(pCurKartAttr, ShapeSuitID); //ShapeSuitID
		Write8(pCurKartAttr, LegendSuitLevel); //LegendSuitLevel
		Write32(pCurKartAttr, LegendSuitLevelChoice); //LegendSuitLevelChoice
		Write32(pCurKartAttr, ShapeLegendSuitID); //ShapeLegendSuitID
#endif
		len = pCurKartAttr - p;
		Set16(p, (WORD)len);
		p += len;
	}

	std::shared_ptr<ClientNode> DstClient = GetClient(DstUin);
	if (!DstClient)
	{
		Write16(p, 0);
	}
	else
	{
		Write16(p, 1); //PetNum responsefindplayerbyqq
		{ //PetInfo
			//国服宠物数据结构
			BYTE* pPetInfo = p;
			Write16(pPetInfo, 0); //len
			Write32(pPetInfo, DstUin); //Uin

			Write32(pPetInfo, DstClient->PetID); //PetId黄金神兽  
			memset(pPetInfo, 0, 17);
			memcpy(pPetInfo, "宠物", 5);
			pPetInfo += 17;
			Write32(pPetInfo, 0);
			Write32(pPetInfo, 0);
			Write8(pPetInfo, 0);

			Write8(pPetInfo, DstClient->PetStatus);//当前宠物形态 黄金神兽 最高4级 

			Write16(pPetInfo, 0x00);//SkillNum

			Write32(pPetInfo, 0x0a); //StrengLevel强化等级 个人信息宠物图标有+5
			Write8(pPetInfo, 0x4); //TopStatus 变形形态
			//变形功能大于1开启
			{ //CommonInfo
				BYTE* pCommonInfo = pPetInfo;
				Write16(pCommonInfo, 0); //len

				Write32(pCommonInfo, 0x5); //HuanHuaLevel幻化等级
				Write8(pCommonInfo, 0x1); //CanHuanHua

				len = pCommonInfo - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			len = pPetInfo - p;
			Set16(p, (WORD)len);

			p += len;
		}
	}
	char IsInTopList = 0;
	IsInTopList = IsUserInTopList(DstUin);

	Write8(p, IsInTopList); //IsInTopList

	{ //KartStoneInfo


		BYTE* pKartStoneInfo = p;
		Write16(pKartStoneInfo, 0); //len
		Write32(pKartStoneInfo, KartItemId); //KartID
		BYTE* pStoneGrooveNum = pKartStoneInfo;
		Write32(pKartStoneInfo, 0); //StoneGrooveNum

		{
			std::vector<KartStoneItemInfo> kartStoneItemInfo;
			PlayerDB_SelectKartStone(DstUin, KartItemId, kartStoneItemInfo);
			//PlayerDB_SelectKartStone(Client->Uin, KartItemId, kartStoneItemInfo);
			for (size_t i = 0; i < kartStoneItemInfo.size(); i++)
			{
				BYTE* pStoneGrooveInfo = pKartStoneInfo;
				Write16(pStoneGrooveInfo, 0); //len

				Write32(pStoneGrooveInfo, kartStoneItemInfo[i].StoneUseOccaType); //StoneUseOccaType
				Write32(pStoneGrooveInfo, kartStoneItemInfo[i].SkillStoneID); //SkillStoneID

				len = pStoneGrooveInfo - pKartStoneInfo;
				Set16(pKartStoneInfo, (WORD)len);
				pKartStoneInfo += len;
			}
			Set32(pStoneGrooveNum, kartStoneItemInfo.size());
		}

		len = pKartStoneInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	/*memcpy(p, "\x00\x00\x1b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\xb5\xa5\xbb\xfa\xb0\xe6\xbd\xf6\xb8\xf6\xc8\xcb\xd1\xd0\xbe\xbf\x00\x00\x00", 85);
	p += 85;
	len = p - buf.data();
	SendToClient(Client, 122, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;*/

	Write8(p, 0); //Contex

	{ //PlayerWlVisibleInfo
		BYTE* pPlayerWlVisibleInfo = p;
		Write16(pPlayerWlVisibleInfo, 0); //len

		Write32(pPlayerWlVisibleInfo, 0); //WonderLandID

		//WonderLandName[]
		memset(pPlayerWlVisibleInfo, 0, MaxNickName);
		pPlayerWlVisibleInfo += MaxNickName;

		Write16(pPlayerWlVisibleInfo, 0); //WonderLandDuty
		Write8(pPlayerWlVisibleInfo, 0); //SubType
		Write8(pPlayerWlVisibleInfo, 0); //GuildGrade

		len = pPlayerWlVisibleInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}


	Write32(p, 0); //SkateCoin
	Write32(p, 0); //MaxDayPveScore
	Write32(p, 0); //MaxHistoryPveScore

	{ //EquipedCarBattleModeSkillInfoList
		BYTE* pEquipedCarBattleModeSkillInfoList = p;
		Write16(pEquipedCarBattleModeSkillInfoList, 0); //len

		Write8(pEquipedCarBattleModeSkillInfoList, 0); //SkillNum
		for (size_t i = 0; i < 0; i++)
		{ //SkillInfo
			BYTE* pSkillInfo = pEquipedCarBattleModeSkillInfoList;
			Write16(pSkillInfo, 0); //len

			Write16(pSkillInfo, 0); //SkillId
			Write8(pSkillInfo, 0); //SkillLevel

			len = pSkillInfo - pEquipedCarBattleModeSkillInfoList;
			Set16(pEquipedCarBattleModeSkillInfoList, (WORD)len);
			pEquipedCarBattleModeSkillInfoList += len;
		}

		len = pEquipedCarBattleModeSkillInfoList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	if (SelectBaseInfoEffect(DstUin)) {
		WriteHex(p, "000000000000000000010001000100000000280000000003");
		//WriteHex(p, "0000000000000000000000000000000000000000000010b5a5bbfab0e6bdf6b8f6c8cbd1d0bebf000000");
	}
	else
	{
		Write8(p, 0); //HasCCVisibleInfo
		for (size_t i = 0; i < 0; i++)
		{ //ChumCircleInfo
			BYTE* pChumCircleInfo = p;
			Write16(pChumCircleInfo, 0); //len

			Write32(pChumCircleInfo, 0); //ChumCircleID

			len = pChumCircleInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Write8(p, 0); //HasCheerAddition
		for (size_t i = 0; i < 0; i++)
		{ //CheerAdditon
			BYTE* pCheerAdditon = p;
			Write16(pCheerAdditon, 0); //len

			Write16(pCheerAdditon, 0); //ExpAddition
			Write16(pCheerAdditon, 0); //WageAddition

			len = pCheerAdditon - p;
			Set16(p, (WORD)len);
			p += len;
		}


		Write32(p, 0); //LoveValue
		Write16(p, 0); //CanEnterChannelFalg[]
		Write8(p, 0); //SpecialActivityID
		Write32(p, 0); //ThemeHouseDressDegreeValue
		Write8(p, AreaIdDst); //AreaIdDst
		Write8(p, 0); //SpecialActivityIDNum
		Write8(p, SimpleInfo); //SimpleInfo
	}

	len = p - buf.data();
	SendToClient(Client, 122, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	//ResponseFindPlayerByQQ2(Client, DstUin, AreaIdDst, SimpleInfo);
}


void RequestGetAchieveList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UINT DstUin = Read32(p);

	ResponseGetAchieveList(Client, DstUin);
}

void ResponseGetAchieveList(std::shared_ptr<ClientNode> Client, UINT DstUin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write32(p, DstUin); //DstUin

	Write16(p, 0); //Result

	WriteHex(p, "02BC001900010000030000000A0000000A50696790000A0000000000190002000003000000140000001454D71862000A00000000001900030000030000001E0000001E55705A94000A00000000001900040000030000002800000028557D24D2000A000000000019000500000300000032000000325587B282000A00000000001900060000030000003C0000003C559788B4000A0000000000190007000002000000460000004655A35DF3000F0000000000190008000002000000500000005055B7234A001400000000001900090000020000005A0000005A55BC2C65001E000000000019000A000002000000640000006455BF8DD40028000000000019000B0000020000006E0000006E560CCF5E0032000000000019000C000002000000780000007859DCC4010041000000000019000D00000200000082000000825D6A92860050000000000019000E0000010000008B0000008C000000000064000000000019000F0000000000000000000096000000000078000000000019001000000000000000000000A0000000000096000000000019001100000000000000000000AA0000000000C8000000000019001200000000000000000000B400000000012C000000000019001300000000000000000000BE00000000015E000000000019001400000000000000000000C80000000001F40000000000190101000103000000320000003254D5DC10000A0000000000190102000103000000C8000000C85587670800140000000000190103000102000003E8000003E855DAA3760032000000000019010400010200001388000013885A1510F6009600000000001902010002030000000A0000000A569FF20100140000000000190202000202000000320000003259D074AE004B0000000000190203000201000000A1000000FA00000000012C00000000001903010003030000000A0000000A5144297C00140000000000190302000302000000320000003255ABB3C800500000000000190303000303000000C8000000C856EB840700960000000000190304000302000001F4000001F45AA5CD97012C0000000000190401000402000001F4000001F455B9DE1500320000000000190402000402000007D0000007D05619FA82006400000000001904030004020000138800001388578F89D401F400000000001905010105030000000A0000000A50699C88000A0000000000190502010502000000320000003255C4C05D001E0000000000190503010501000000BC000000C800000000003200000000001906010106030000000A0000000A557C23FD000A0000000000190602010602000000320000003255C064F2001E0000000000190603010602000000C8000000C85710D888003200000000001907010107020000000A0000000A55E1207C000F00000000001907020107020000003200000032571C380D002D0000000000190703010702000000C8000000C859EF4BD5005A00000000001908010108020000000A0000000A56182E3E000F0000000000190802010802000000320000003257987222002D0000000000190803010802000000C8000000C85AD81521005A00000000001909010109020000000A0000000A57B7F7F10014000000000019090201090200000032000000325BC01C80003C00000000001909030109010000009C000000C80000000000780000000000190A01010A02000000010000000155BA442E000A0000000000190A02010A02000000010000000155BC3D8E00140000000000190A03010A030000000100000001566B382000320000000000190A04010A02000000010000000155C32FC0000A0000000000190A05010A0200000001000000015619FA8200140000000000190A06010A0200000001000000015752D89200320000000000190A07010A02000000010000000155BB7AB8000A0000000000190A08010A02000000010000000155BC9F4100140000000000190A09010A03000000010000000156BC9BAD00320000000000190A0A010A02000000010000000155C5E612000A0000000000190A0B010A02000000010000000155F49F3400140000000000190A0C010A03000000010000000156BB044C00320000000000190A0D010A02000000010000000155A7D8EB000A0000000000190A0E010A02000000010000000155BAF70800140000000000190A0F010A02000000010000000157C5E50300320000000000190A10010A03000000010000000155842152000A0000000000190A11010A020000000100000001566AD62A00140000000000190A12010A03000000010000000156A92A7100320000000000190A13010A02000000010000000155DD96E1000A0000000000190A14010A0200000001000000015AA91D1400140000000000190A15010A0100000000000000010000000000320000000000190A16010A02000000010000000155BC4178000A0000000000190A17010A02000000010000000155CB115500140000000000190A18010A0200000001000000015643E45C00320000000000190A19010A02000000010000000155AC1781000A0000000000190A1A010A020000000100000001560BAE8600140000000000190A1B010A03000000010000000156ECA82C00320000000000190A1C010A02000000010000000155D59D8B000A0000000000190A1D010A02000000010000000155DAA37600140000000000190A1E010A0200000001000000015786055400320000000000190A1F010A02000000010000000155A7E116000A0000000000190A20010A02000000010000000155BF879600140000000000190A21010A020000000100000001566CE27000320000000000190A22010A02000000010000000155D6D690000A0000000000190A23010A02000000010000000155E1207C00140000000000190A24010A0100000000000000010000000000320000000000190A25010A02000000010000000155A7DF7D000A0000000000190A26010A02000000010000000155DC17F600140000000000190A27010A020000000100000001567ECCC800320000000000190A28010A02000000010000000155CDECE7000A0000000000190A29010A02000000010000000155DADB1100140000000000190A2A010A0200000001000000015711992600320000000000190A2B010A03000000010000000155600DF9000A0000000000190A2C010A02000000010000000155B9FF6800140000000000190A2D010A02000000010000000155D7FDD500320000000000190A2E010A02000000010000000155DA8E6E000A0000000000190A2F010A02000000010000000156FF8E2A00140000000000190A30010A02000000010000000156FF8E2A00320000000000190A31010A02000000010000000155DADA3B000A0000000000190A32010A0300000001000000015691BF1600140000000000190A33010A02000000010000000157A97F4100320000000000190A34010A020000000100000001567E9B51000A0000000000190A35010A0300000001000000015691BAD700140000000000190A36010A02000000010000000159A472FA00320000000000190A37010A02000000010000000155B9E748000A0000000000190A38010A02000000010000000155FE53B700140000000000190A39010A0200000001000000015767C0DE00320000000000190A3A010A02000000010000000155D64979000A0000000000190A3B010A0300000001000000015699D3F500140000000000190A3C010A02000000010000000159CE005700320000000000190A3D010A02000000010000000155F46A76000A0000000000190A3E010A03000000010000000156887B2400140000000000190A3F010A0100000000000000010000000000320000000000190A40010A02000000010000000155D810F8000A0000000000190A41010A020000000100000001563D786700140000000000190A42010A02000000010000000159A577B100320000000000190A43010A02000000010000000155E15475000A0000000000190A44010A02000000010000000156FF540B00140000000000190A45010A020000000100000001581E7E8D00320000000000190B01010B01000000000000000100000000001E0000000000190B02010B0100000000000000010000000000960000000000190B03010B0100000000000000010000000000140000000000190B04010B0100000000000000010000000000640000000000190B05010B01000000000000000100000000001E0000000000190B06010B0100000000000000010000000000960000000000190B07010B0100000000000000010000000000140000000000190B08010B0100000000000000010000000000640000000000190B09010B0100000000000000010000000000140000000000190B0A010B0100000000000000010000000000640000000000190B0B010B0100000000000000010000000000140000000000190B0C010B0100000000000000010000000000640000000000190B0D010B0100000000000000010000000000320000000000190B0E010B0100000000000000010000000000640000000000190B0F010B0100000000000000010000000000960000000000190B10010B02000000010000000155D86BD000320000000000190B11010B0100000000000000010000000000500000000000190B12010B0100000000000000010000000000780000000000190C01020C020000000A0000000A55BF363B00140000000000190C02020C03000000320000003256ED236A00320000000000190C03020C010000004B000000C80000000000C80000000000190D01020D02000000010000000155BF367200C80000000000190E01020E03000000010000000150CC288500640000000000190F01020F020000000A0000000A55C5D98000140000000000190F02020F03000000C8000000C856972E0600500000000000190F03020F02000007D0000007D05778E8DA012C0000000000191001021003000000320000003256C40FD6000A0000000000191002021001000000780000012C00000000001E000000000019100302100000000000000007D00000000000C800000000001911010011020000000A0000000A59AD60A8000A0000000000191201001202000000050000000564E980C800140000000000191301001300000000000000000500000000001E0000000000191401001400000000000000000100000000003200000000001915010015020000000A0000000A60A67EF1001400000000001916010016020000000A0000000A5A436F06001400000000001919010119000000000000000064000000000014000000000019190201190000000000000000C8000000000028000000000019190301190000000000000001F40000000000460000000000191A01011A00000000000000006400000000001E0000000000191A02011A0000000000000000C80000000000320000000000191A03011A0000000000000001F40000000000640000000000191B01011B02000000640000006455C9629F001E0000000000191B02011B02000000C8000000C8562B1B2C00320000000000191B03011B02000001F4000001F4579A530D00640000000000191C01011C01000000010000006400000000001E0000000000191C02011C0000000000000000C80000000000320000000000191C03011C0000000000000001F40000000000640000000000191D01001D0200000001000000015C42D46A001E0000000000191D02001D0200000002000000025C66956300320000000000191D03001D0200000003000000035CAC109A00500000000000191D04001D0000000000000000040000000000640000000000191D05001D0000000000000000050000000000960000000000191D06001D00000000000000000600000000012C0000000000191D07001D0000000000000000070000000001900000000000191E01001E0100000000000000010000000000C80000000000191F01021F00000000000000001E00000000001400000000001920010120020000000A0000000A5753B673000A00000000001920020120010000001100000064000000000014000000000019200301200000000000000001F4000000000064000000000019220100220100803A57FFFFFFFF0000000001F4000000000019230100230100000000000000010000000000C80000000000192401002400000000000000003C000000000096000000000019330102330100000000000000010000000000C8000000000019340102340100000000000000010000000001F40000000000193501013503000000140000001455842152001E0000000000193502013502000000280000002855A808B5003200000000001935030135020000003C0000003C55BA3B9B00500000000000193504013502000000500000005055C32EBE006400000000001935050135020000006E0000006E5617ECB0008200000000001935060135020000008700000087575DAA4A00960000000000193507013502000000960000009657C5997000C80000000000193508013501000000A9000000B40000000000FA0000000000193509013500000000000000010400000000012C000000000019350A013500000000000000012C00000000015E000000000019350B0135000000000000000190000000000190000000000019350C01350000000000000001F40000000001C20000000000193601013601000000000000000100000000012C0000000000193701023701000000000000000100000000000A0000000000193801023802000000030000000355D5C6800014000000000019390100390100000019000000640000000000640000000000193A01003A01000000090000001E0000000000140000000000193B01003B010000000B0000001E0000000000140000000000193C01003C01000000090000001E0000000000140000000000193D01003D00000000000000000100000000001E0000000000193D02003D0000000000000000020000000000320000000000193D03003D0000000000000000030000000000500000000000193D04003D0000000000000000040000000000640000000000193D05003D0000000000000000050000000000960000000000193D06003D00000000000000000600000000012C0000000000193E01003E0000000000000000960000000000320000000000193E02003E0000000000000001F40000000000960000000000193E03003E000000000000000E100000000001F40000000000193F01003F03000000140000001455929BD8000A0000000000193F02003F02000000320000003255CAAD2000190000000000193F03003F0200000064000000645641445600320000000000193F04003F02000000C8000000C857402E9600640000000000193F05003F02000001F4000001F45BC2CB9500FA0000000000193F06003F010000035F000003E80000000001F40000000000194001004000000000000000005A0000000000640000000000194002004000000000000000016800000000012C0000000000194101004100000000000000000C000000000032000000000019420100420000000000000003E8000000000032000000000019420200420000000000000027100000000000320000000000194301004302000000010000000155B9DE15001E00000000001943020043030000000900000009569FF201003200000000001943030043020000001D0000001D59D074AE0050000000000019430400430200000031000000315A90F5B2006400000000001943050043010000004B00000063000000000096000000000019430600430000000000000000C70000000000C80000000000194307004300000000000000012B00000000012C000000000019430800430000000000000001F30000000001F4000000000019430900430000000000000003E7000000000320000000000019440101440000000000000000050000000000320000000000194402014400000000000000000A000000000050000000000019440301440000000000000000140000000000780000000000194404014400000000000000003200000000012C000000000019440501440000000000000000640000000001F40000000000194501014501000000000000000100000000012C00000000001946010146000000000000000064000000000064000000000019460201460000000000000001F40000000000C8000000000019460301460000000000000003E800000000012C00000000001947010147000000000000000064000000000064000000000019470201470000000000000001F40000000000C8000000000019470301470000000000000003E800000000012C00000000001948010148000000000000000001000000000064000000000019480201480000000000000000640000000000C8000000000019490101490000000000000000010000000000640000000000194902014900000000000000000A0000000000C80000000000194A01014A0300000001000000015690D31A00640000000000194B01014B00000000000000003200000000012C0000000000194C01014C0000000000000000140000000000C80000000000194D01014D02000002EE000002EE571AAB6300640000000000194D02014D02000003E8000003E85725A6CA00960000000000194D03014D02000007D0000007D0578F89D400C80000000000194D04014D0200000BB800000BB857B0478F00FA0000000000194D05014D0200000FA000000FA057F0A77E012C0000000000194E01014E030000009600000096567F005B00320000000000194E02014E02000000FA000000FA5707825F00500000000000194E03014E020000019000000190571B015700640000000000194E04014E02000001F4000001F45720864100780000000000194E05014E0200000258000002585725A0F300960000000000194F01014F000000000000000032000000000096000000000019500101500100000005000001F40000000000C8000000000019510101510000000000000000C800000000006400000000001952010152010000003C0000006400000000006400000000001953010253000000000000000002000000000032000000000019530202530000000000000000030000000000500000000000195303025300000000000000000400000000006400000000001953040253000000000000000005000000000096000000000019540102540000000000000003E800000000001E0000000000195501015501000000030000000A0000000000320000000000195502015500000000000000003200000000004B00000000001955030155000000000000000064000000000064000000000019550401550000000000000000C800000000007D000000000019550501550000000000000001F4000000000096000000000019560101560000000000000000C800000000001E0000000000195701015700000000000000000A0000000000320000000000195702015700000000000000003200000000004B00000000001957030157000000000000000064000000000064000000000019570401570000000000000000C800000000007D000000000019570501570000000000000001F40000000000960000000000195801015800000000000000000100000000012C000000000019590101590000000000000000050000000000640000000000195A01015A02000000060000000657A92FA500320000000000195A02015A010000000C0000000D0000000000500000000000195A03015A0000000000000000140000000000640000000000195B01005B020000119400001194581E71FC000A0000000000195B02005B0100000CD9000013880000000000140000000000195B03005B00000000000000232800000000001E0000000000195B04005B000000000000003A980000000000320000000000195B05005B0000000000000052080000000000640000000000195B06005B000000000000007D000000000000960000000000195D01005D00000000000000000A0000000000140000000000195D02005D00000000000000001E00000000001E0000000000195D03005D0000000000000000320000000000320000000000195D04005D0000000000000000640000000000500000000000195D05005D0000000000000000C80000000000640000000000195E01005E00000000000000000200000000001E0000000000195E02005E00000000000000000A0000000000320000000000195E03005E0000000000000000140000000000500000000000195E04005E0000000000000000320000000000640000000000195E05005E0000000000000000640000000000960000000000195E06005E0000000000000000C80000000000B4000000000019600100600000000000000000010000000000320000000000196002006000000000000000000200000000006400000000001960030060000000000000000003000000000096000000000019610100610000000000000000030000000000140000000000196102006100000000000000000A000000000028000000000019610300610000000000000000320000000000500000000000196201006200000000000000000A00000000000A0000000000196202006200000000000000003200000000001E0000000000196203006200000000000000009600000000003200000000001963010063020000000A0000000A65BA7226001E00000000001963020063010000001300000064000000000032000000000019630300630000000000000000C80000000000780000000000196401026402000001F4000001F45DB46B5200320000000000196402026401000003DE000005DC000"
		"0000000960000000000196403026400000000000000138800000000012C000000000019650100650000000000000000140000000000140000000000196502006500000000000000006400000000001E000000000019650300650000000000000001F4000000000032000000000019650400650000000000000003E8000000000050000000000019650500650000000000000007D0000000000064000000000019660100660100000002000000140000000000140000000000196602006600000000000000003200000000001E0000000000196603006600000000000000006400000000003200000000001966040066000000000000000096000000000050000000000019660500660000000000000000C80000000000640000000000196606006600000000000000012C0000000000960000000000196701026700000000000000000500000000001400000000001967020267000000000000000032000000000032000000000019670302670000000000000000C8000000000064000000000019680102680200000001000000015996A408001E0000000000196802026801000000030000000A00000000003C000000000019680302680000000000000000320000000000960000000000196901006900000000000000000100000000000A0000000000196902006900000000000000000A000000000028000000000019690300690000000000000000640000000000640000000000196A01006A00000000000000000100000000001E0000000000196A02006A00000000000000000A0000000000320000000000196A03006A0000000000000000640000000000780000000000196B01006B0000000000000000010000000000140000000000196B02006B00000000000000000A0000000000320000000000196B03006B0000000000000000640000000000640000000000196C01006C0000000000000000010000000000140000000000196C02006C00000000000000000A0000000000320000000000196C03006C00000000000000001E0000000000640000000000196D01006D0000000000000000010000000000140000000000196D02006D0000000000000000140000000000320000000000196D03006D0000000000000000320000000000640000000000196E01006E0000000000000000010000000000140000000000196E02006E0000000000000000050000000000320000000000196E03006E00000000000000000A0000000000640000000000196F01006F0000000000000000010000000000140000000000196F02006F0000000000000000050000000000320000000000196F03006F00000000000000000A0000000000640000000000197001007000000000000000001400000000003200000000001970020070000000000000000064000000000050000000000019700300700000000000000000C80000000000640000000000197101007100000000000000001400000000001E00000000001971020071000000000000000064000000000032000000000019710300710000000000000000C80000000000500000000000197201007200000000000000001400000000001E00000000001972020072000000000000000064000000000032000000000019720300720000000000000000960000000000500000000000197301007300000000000000000A00000000003200000000001973020073000000000000000014000000000050000000000019730300730000000000000000320000000000640000000000197401007400000000000000000A00000000001E00000000001974020074000000000000000014000000000032000000000019740300740000000000000000320000000000500000000000197501037500000000000000000A0000000000030000000000197502037500000000000000003200000000000F0000000000197503037500000000000000006400000000001E0000000000197504037500000000000000012C00000000005A000000000019750503750000000000000001F4000000000096000000000019760103760000000000000000320000000000050000000000197602037600000000000000006400000000000A000000000019760303760000000000000000C8000000000014000000000019760403760000000000000001F4000000000032000000000019760503760000000000000003E8000000000064000000000019770103770000000000000000640000000000050000000000197702037700000000000000012C00000000000F000000000019770303770000000000000001F4000000000019000000000019770403770000000000000003E8000000000032000000000019770503770000000000000007D0000000000064000000000019780103780000000000000000320000000000050000000000197802037800000000000000006400000000000A000000000019780303780000000000000000C800000000001E000000000019780403780000000000000001F400000000004B000000000019780503780000000000000003E8000000000096000000000019790100790000000000000000140000000000320000000000197902007900000000000000003200000000005000000000001979030079000000000000000050000000000064000000000019790400790000000000000000640000000000960000000000197A01007A0000000000000000140000000000320000000000197A02007A0000000000000000320000000000500000000000197A03007A0000000000000000640000000000640000000000197A04007A0000000000000000C80000000000C80000000000197B01027B0000000000000000020000000000320000000000197B02027B0000000000000000030000000000500000000000197B03027B0000000000000000040000000000640000000000197B04027B0000000000000000050000000000960000000000197C01027C0000000000000003E800000000001E0000000000197D01017D0200000028000000285996F16C00320000000000197D02017D010000003A000000500000000000500000000000197D03017D0000000000000000780000000000640000000000197E01037E0100000001000000070000000000140000000000197E02037E00000000000000001E0000000000280000000000197E03037E00000000000000004200000000003C0000000000197E04037E0000000000000000640000000000640000000000197E05037E0000000000000000C80000000000A00000000000197E06037E00000000000000016D0000000000C80000000000197F01007F01000000040000000A0000000000140000000000197F02007F00000000000000000F0000000000280000000000197F03007F0000000000000000140000000000640000000000197F04007F0000000000000000280000000000C80000000000198001008002000000140000001459C4A08A0014000000000019800200800100000000000000320000000000320000000000198003008000000000000000005000000000005000000000001980040080000000000000000064000000000064000000000019810100810200000015000000155A05C71A0014000000000019810200810200000016000000165A070FC30032000000000019810300810200000017000000175A0FCFC60064000000000019810400810200000018000000185BB7554900C8000000000019810500810200000019000000195C4C65E0019000000000001981060081020000001A0000001A649816F802580000000000198107008101000000180000001B00000000025800000000001982010082020000000A0000000A5A06D7DA0014000000000019820200820200000032000000325A0ABFAD0064000000000019820300820200000064000000645A142CC100C80000000000198204008202000001F4000001F45B545C8D01F400000000001983010083020000000A0000000A5A07059D0014000000000019830200830200000014000000145A0967C20028000000000019830300830200000032000000325A0D8F5C0064000000000019830400830200000064000000645A38889800C80000000000198305008302000001F4000001F45BCF397F01F4000000000019840100840000000000000000030000000000320000000000198402008400000000000000001E000000000050000000000019840300840000000000000000640000000000640000000000198501008501000000040000000A00000000003200000000001985020085000000000000000064000000000050000000000019850300850000000000000003E80000000000640000000000198601008601000000820000012C00000000003200000000001986020086000000000000000BB8000000000050000000000019860300860000000000000075300000000000640000000000198701038701000000050000000A0000000000280000000000198702038700000000000000000F00000000003C00000000001987030387000000000000000014000000000064000000000019870403870000000000000000190000000000A00000000000198705038700000000000000001E0000000000C80000000000198801008801000000010000000A00000000001400000000001988020088000000000000000019000000000064000000000019880300880000000000000000320000000000C80000000000198901018900000000000000000A0000000000140000000000198A01018A0000000000000000050000000000190000000000198A02018A00000000000000000A0000000000320000000000198B01018B00000000000000000A0000000000320000000000198B02018B0000000000000000140000000000780000000000198B03018B0000000000000000280000000000F00000000000198C01018C0000000000000000640000000000640000000000198D01018D00000000000000000A0000000000280000000000198D02018D00000000000000001E0000000000960000000000198D03018D00000000000000003200000000012C0000000000198E01018E0000000000000000050000000000320000000000198E02018E00000000000000000A0000000000500000000000198E03018E0000000000000000140000000000640000000000198E04018E0000000000000000320000000000960000000000198E05018E00000000000000006400000000012C0000000000198F01018F0000000000000000960000000000B4000000000019900101900000000000000000C80000000000B40000000000199101019100000000000000001E00000000003200000000001991020191000000000000000032000000000064000000000019910301910000000000000000460000000000960000000000199201019200000000000000000A000000000032000000000019920201920000000000000000140000000000780000000000199203019200000000000000001E0000000000C80000000000199301019300000000000000003C000000000064000000000019930201930000000000000000780000000000C8000000000019930301930000000000000000B400000000012C00000000001994010194000000000000000014000000000032000000000019940201940000000000000000320000000000640000000000199"
		"501019500000000000000003200000000003200000000001995020195000000000000000064000000000064000000000019960101960000000000000000320000000000320000000000199602019600000000000000006400000000006400000000001998010198020000000A0000000A5D41A1F4001400000000001998020198010000000B000000140000000000280000000000199803019800000000000000001E00000000003C00000000001999010199020000000A0000000A60C22117001400000000001999020199010000000B00000014000000000028000000000019990301990000000000000000320000000000640000000000199A01009A0000000000000000010000000000140000000000199B01009B00000000000000000100000000001E0000000000199C01009C0000000000000000010000000000320000000000199D01019D0000000000000000030000000000140000000000199D02019D0000000000000000050000000000280000000000199D03019D00000000000000000800000000003C0000000000199E01019E0000000000000000030000000000140000000000199E02019E0000000000000000050000000000280000000000199E03019E00000000000000000800000000003C0000000000199F01019F000000000000000005000000000032000000000019A00101A000000000000000000F000000000050000000000019A10101A100000000000000001E000000000064000000000019A20101A2000000000000000028000000000096000000000019A30101A300000000000000003C0000000000C8000000000019A40101A400000000000000001E000000000032000000000019A40201A4000000000000000032000000000064000000000019A40301A4000000000000000046000000000096000000000019A70101A700000000000000000A000000000032000000000019A70201A7000000000000000014000000000064000000000019A70301A700000000000000001E000000000096000000000019A90101A9010000000C0000001E000000000019000000000019A90201A900000000000000003C000000000028000000000019A90301A9000000000000000050000000000032000000000019AB0101AB000000000000000064000000000019000000000019AB0201AB00000000000000012C000000000028000000000019AB0301AB0000000000000001F400000000003C000000000019AB0401AB0000000000000003E8000000000050000000000019AB0501AB0000000000000005DC000000000096000000000019AC0101AC00000000000000000F000000000019000000000019AC0201AC000000000000000019000000000028000000000019AC0301AC00000000000000002800000000003C000000000019AC0401AC000000000000000032000000000050000000000019AD0101AD000000000000000014000000000028000000000019AE0101AE00000000000000001E00000000003C000000000019AF0101AF000000000000000028000000000032000000000019B00101B00000000000000001F400000000001E000000000019B30101B30000000000000003E8000000000019000000000019B30201B3000000000000000BB8000000000028000000000019B30301B300000000000000177000000000003C000000000019B30401B3000000000000001F40000000000050000000000019B40103B40200000001000000016687EA250001000000000019B40203B40200000002000000026687EA250005000000000019B40303B40200000003000000036687EA25000A000000000019B40403B40200000004000000046687EA250014000000000019B40503B40200000005000000056687EA250014000000000019B40603B4010000000500000006000000000032000000000019B40703B4000000000000000007000000000064000000000019B40803B40000000000000000080000000000C8000000000019B40903B400000000000000000900000000012C000000000019B40A03B400000000000000000A0000000001F4000000000019B50101B5000000000000000005000000000032000000000019B50201B500000000000000000A000000000050000000000019B50301B5000000000000000014000000000064000000000019B50401B5000000000000000032000000000096000000000019B50501B500000000000000006400000000012C000000000019B60101B60000000000000000960000000000B4000000000019B70101B70000000000000000C80000000000B4000000000019B80101B800000000000000001E000000000096000000000019B90101B9000000000000000019000000000064000000000019BA0101BA000000000000000019000000000032000000000019BB0101BB00000000000000000200000000001E000000000019BB0201BB00000000000000000A000000000032000000000019BB0301BB000000000000000014000000000078000000000019BC0101BC000000000000000005000000000032000000000019BC0201BC000000000000000032000000000064000000000019BC0301BC0000000000000000780000000000C8000000000019BC0401BC00000000000000012C00000000012C000000000019BD0101BD00000000000000000A000000000032000000000019BD0201BD000000000000000032000000000064000000000019BD0301BD00000000000000012C000000000096000000000019BF0101BF000000000000000064000000000019000000000019BF0201BF00000000000000012C000000000028000000000019BF0301BF0000000000000001F400000000003C000000000019BF0401BF0000000000000003E8000000000050000000000019BF0501BF0000000000000005DC000000000096000000000019C00101C000000000000000000A000000000032000000000019C00201C000000000000000001E000000000050000000000019C00301C0000000000000000050000000000078000000000019C00401C00000000000000000960000000000B4000000000019C10101C100000000000000000F000000000032000000000019C10201C1000000000000000032000000000050000000000019C10301C100000000000000006E000000000078000000000019C10401C10000000000000000C80000000000A0000000000019C20101C200000000000000000A000000000032000000000019C20201C200000000000000001E000000000050000000000019C20301C2000000000000000050000000000078000000000019C20401C20000000000000000960000000000B4000000000019C40101C40000000000000000FE000000000064000000000019C50101C500000000000000003C000000000032000000000019C60101C600000000000000003C000000000032000000000019C70101C700000000000000003C000000000032000000000019C80101C800000000000000003C000000000032000000000019C90101C900000000000000003C000000000032000000000019CA0101CA00000000000000003C000000000032000000000019CB0101CB00000000000000003C000000000032000000000019CC0101CC000000000000000064000000000032000000000019CC0201CC00000000000000012C000000000032000000000019CC0301CC000000000000000258000000000064000000000019CC0401CC0000000000000005DC000000000064000000000019CC0501CC0000000000000009C4000000000064000000000019CE0101CE000000000000000064000000000019000000000019CE0201CE0000000000000000C8000000000028000000000019CE0301CE00000000000000012C00000000003C000000000019CE0401CE0000000000000001F4000000000050000000000019CF0101CF000000000000000096000000000014000000000019CF0201CF00000000000000012C00000000001E000000000019D00101D000000000000000000100000000001E000000000019D20101D200000000000000000100000000001E000000000019D20201D200000000000000000200000000003C000000000019D20301D2000000000000000003000000000078000000000019D30101D300000000000000000100000000001E000000000019D30201D300000000000000000200000000003C000000000019D30301D3000000000000000003000000000078000000000019D40101D400000000000000000100000000001E000000000019D40201D400000000000000000200000000003C000000000019D40301D4000000000000000003000000000078000000000019D50101D500000000000000000100000000001E000000000019D50201D500000000000000000200000000003C000000000019D50301D5000000000000000003000000000078000000000019D60101D600000000000000000100000000001E000000000019D60201D600000000000000000200000000003C000000000019D60301D6000000000000000003000000000078000000000019D70101D700000000000000000100000000001E000000000019D70201D700000000000000000500000000003C000000000019D70301D700000000000000000A000000000078000000000019D80101D800000000000000000100000000001E000000000019D80201D800000000000000000500000000003C000000000019D80301D800000000000000000A000000000078000000000019D90101D900000000000000000100000000001E000000000019D90201D900000000000000000500000000003C000000000019D90301D900000000000000000A000000000078000000000019DA0101DA00000000000000000100000000001E000000000019DA0201DA00000000000000000500000000003C000000000019DA0301DA00000000000000000A000000000078000000000019DB0101DB00000000000000000A000000000032000000000019DB0201DB00000000000000001E000000000050000000000019DB0301DB000000000000000032000000000064000000000019DB0401DB000000000000000050000000000096000000000019DB0501DB0000000000000000780000000000B4000000000019DC0101DC00000000000000001E000000000032000000000019DD0101DD00000000000000001E000000000032000000000019DE0101DE00000000000000001E000000000032000000000019DF0101DF00000000000000001E000000000032000000000019E00101E000000000000000000A000000000032000000000019E00201E0000000000000000014000000000050000000000019E00301E000000000000000001E000000000064000000000019E00401E0000000000000000032000000000096000000000019E00501E00000000000000000500000000000B4000000000019E10101E1000000000000000009000000000014000000000019E10201E100000000000000001D000000000028000000000019E10301E100000000000000003B00000000003C000000000019E10401E1000000000000000063000000000050000000000019E20101E200000000000000000100000000001E000000000019E30101E3000000000000000064000000000019000000000019E30201E300000000000000012C000000000028000000000019E30301E30000000000000001F400000000003C000000000019E30401E30000000000000003E8000000000050000000000019E30501E30000000000000005DC000000000096000000000019E40101E400000000000000000A00000000001E000000000019E40201E400000000000000001700000000003C000000000019E40301E400000000000000002200000000005A000000000019E40401E400000000000000002F000000000078000000000019E40501E400000000000000003F000000000096000000000019E50101E5000000000000000023000000000032000000000019E60101E6000000000000000019000000000050000000000019E70101E700000000000000000F000000000064000000000019E80101E80000000000000000C8000000000064000000000019EA0103EA01000003DE0000177000000000012C000000000019EA0203EA00000000000000271000000000012C000000000019EA0303EA000000000000002EE000000000012C000000000019EA0403EA000000000000003A9800000000012C000000000019EA0503EA000000000000004650000000000190000000000019EB0101EB000000000000000006000000000014000000000019EB0201EB00000000000000000C00000000001E000000000019EB0301EB000000000000000012000000000028000000000019EB0401EB000000000000000018000000000032000000000019EB0501EB00000000000000001E000000000056000000000019EE0101EE000000000000000005000000000032000000000019EE0201EE00000000000000000A000000000050000000000019EE0301EE000000000000000014000000000078000000000019EF0101EF00000000000000000A000000000032000000000019EF0201EF00000000000000000F000000000050000000000019EF0301EF000000000000000014000000000078000000000019F00101F000000000000000000C000000000032000000000019F00201F000000000000000001E000000000050000000000019F00301F000000000000000002D000000000078000000000019F20101F2000000000000000096000000000014000000000019F20201F200000000000000012C00000000001E000000000019F20301F20000000000000001F4000000000032000000000019F30101F300000000000000006400000000001E000000000019F30201F300000000000000012C000000000032000000000019F30301F30000000000000001F4000000000050000000000019F30401F30000000000000003E8000000000064000000000019F30501F30000000000000005DC000000000096000000000019F40101F400000000000000000A000000000032000000000019F40201F4000000000000000014000000000050000000000019F40301F4000000000000000032000000000078000000000019F40401F4000000000000000064000000000096000000000019F50101F5000000000000000014000000000032000000000019F50201F5000000000000000032000000000050000000000019F50301F5000000000000000064000000000078000000000019F50401F50000000000000000C8000000000096000000000019F80101F8000000000000000096000000000014000000000019F80201F800000000000000012C00000000001E000000000019F80301F80000000000000001F4000000000032000000000019F90101F901000000060000006400000000001E000000000019F90201F900000000000000012C000000000032000000000019F90301F90000000000000001F4000000000050000000000019F90401F90000000000000003E8000000000064000000000019F90501F90000000000000005DC00000000009600000000");
	len = p - buf.data();
	SendToClient(Client, 373, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;

	Write16(p, 1); //AchieveNum
	for (size_t i = 0; i < 1; i++)
	{ //Achieves
		BYTE* pAchieves = p;
		Write16(pAchieves, 0); //len

		Write16(pAchieves, 7937); //ID
		Write8(pAchieves, 0); //ShowType
		Write8(pAchieves, 0); //LogicType
		Write8(pAchieves, 0); //Status
		Write16(pAchieves, 0); //Progress
		Write16(pAchieves, 0); //TotalProgress
		Write32(pAchieves, 0); //FinTime
		Write16(pAchieves, 0); //AchieveValue
		Write32(pAchieves, 0); //AchieveFlagType

		len = pAchieves - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 373, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseGetGroupFriendBaseInfo(std::shared_ptr<ClientNode> Client, UINT Uin, char* newname)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write16(p, 0); //GroupID
	{ //GroupFriendBaseInfo
		BYTE* pGroupFriendBaseInfo = p;
		Write16(pGroupFriendBaseInfo, 0); //len

		Write32(pGroupFriendBaseInfo, Uin); //Uin
		memset(pGroupFriendBaseInfo, 0, 17);
		if (newname)
		{
			memcpy(pGroupFriendBaseInfo, newname, strlen(newname));
		}
		pGroupFriendBaseInfo += 17;
		len = pGroupFriendBaseInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ReasonLen

	len = p - buf.data();
	SendToClient(Client, 148, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestGetGroupFriendBaseInfo(std::shared_ptr<ClientNode> Client)
{
	//BYTE* p = Body; WORD len;

	//UINT Uin = Read32(p);
	//UINT Time = Read32(p);
	//short GroupID = Read16(p);

	
	
	int result;


	std::string NickName = "";
	//联合查询
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT F.Uin,B.NickName FROM FriendInfo F join BaseInfo b  on F.Uin=B.Uin WHERE F.FriendUin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();

		// 处理结果
		while (res->next()) {
			NickName = UTF2GB(res->getString("NickName").c_str());
			ResponseGetGroupFriendBaseInfo(Client, res->getUInt("Uin"), NickName.data());
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		printf(e.what());

		printf("RequestGetGroupFriendBaseInfo error\n");
	}


}
void NotifySpecialActivityInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ActivityID
	Write8(p, 0); //Flag
	Write32(p, 0); //OpenDate
	Write32(p, 0); //CloseDate
	Write32(p, 0); //OpenTime
	Write32(p, 0); //CloseTime
	Write8(p, 0); //SpecialActivityIDNum
	Write8(p, 1); //SpecialActivityInfoNum
	for (size_t i = 0; i < 1; i++)
	{ //SpecialActivityInfo
		BYTE* pSpecialActivityInfo = p;
		Write16(pSpecialActivityInfo, 0); //len

		Write8(pSpecialActivityInfo, 1); //ProfessionLicenseInfoNum
		for (size_t i = 0; i < 1; i++)
		{ //ProfessionLicenseInfo
			BYTE* pProfessionLicenseInfo = pSpecialActivityInfo;
			Write16(pProfessionLicenseInfo, 0); //len

			Write8(pProfessionLicenseInfo, 6); //BigLevel
			Write8(pProfessionLicenseInfo, 5); //SmallLevel

			len = pProfessionLicenseInfo - pSpecialActivityInfo;
			Set16(pSpecialActivityInfo, (WORD)len);
			pSpecialActivityInfo += len;
		}

		len = pSpecialActivityInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 24767, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseShapeLegendOperate(std::shared_ptr<ClientNode> Client, UCHAR OpType, int ShapeID, UCHAR ShapeLevel, int NewShowChoice)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write8(p, OpType); //OpType
	Write16(p, 0); //ShapeNum
	Write32(p, ShapeID); //ShapeID
	Write8(p, ShapeLevel); //ShapeLevel
	Write8(p, 0); //HasRefit
	Write32(p, NewShowChoice); //NewShowChoice
	Write32(p, 0); //DirectMaxLevelShapeID
	Write32(p, 0); //DirectMaxShapePreLevel

	len = p - buf.data();
	SendToClient(Client, 319, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}




void getKartSkin(std::shared_ptr<ClientNode> Client)
{
	ResponseSaveShapeRefit4(Client, Client->KartID);
}

void ChangKartMode(std::shared_ptr<ClientNode> Client, UINT kartID)
{
	if (Client->KartID == kartID)
	{
		getKartSkin(Client);
		return;
	}
	std::vector<ItemStatus> aItemStatus(2);
	aItemStatus[0].ItemID = Client->KartID;
	aItemStatus[0].NewStatus = 0;
	aItemStatus[1].ItemID = kartID;
	aItemStatus[1].NewStatus = 1;
	
	
	int result;

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		for (int i = 0; i < 2; i++)
		{

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET Status=? WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, aItemStatus[i].NewStatus);
			prep_stmt->setUInt(2, Client->Uin);
			prep_stmt->setUInt(3, aItemStatus[i].ItemID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}


	}
	catch (const std::exception& e)
	{
		printf(e.what());

		printf("ChangKartMode error\n");
	}


	ResponseChangeItemStatus(Client, 2, aItemStatus);
	Client->KartID = kartID;

	getKartSkin(Client);
}

void getGameMode(std::shared_ptr<ClientNode> Client)
{
	const unsigned char* pMsg1311171 = EHexToByte("000001010000007540D0000E220443B00160C502E47BE55FF81C310E0000000000000000F59D");

	SendToClient(Client, 131, pMsg1311171, 38, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	delete[] pMsg1311171;
	
	
	int result;
	BOOL isExist = TRUE;
	if (Client->ServerID == 900) // 滑板
	{
		UINT SlideID = 19294;

		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT SlideID FROM BaseGameTypeStatus WHERE Uin = ?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				if (res->getUInt("SlideID") > 0) {
					SlideID = res->getUInt("SlideID");
				}
			}
			res.reset();

		}
		catch (const std::exception& e)
		{
			printf(e.what());

			printf("getGameMode1 error\n");
		}
		ChangKartMode(Client, SlideID);
	}
	else if (Client->ServerID == 1000) // 轮滑
	{

		UINT SkateID = 0;

		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{
			const char* sql = NULL;
			if (Client->Gender == 0)
			{
				SkateID = 31941;
				sql = "SELECT SkateID0 FROM BaseGameTypeStatus WHERE Uin = ?;";
			}
			else
			{
				SkateID = 31940;
				sql = "SELECT SkateID1 FROM BaseGameTypeStatus WHERE Uin = ?;";
			}

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				if (res->getUInt(1) > 0) {
					SkateID = res->getUInt(1);
				}
				break;
			}
			res.reset();

		}
		catch (const std::exception& e)
		{
			printf(e.what());

			printf("getGameMode2 error\n");
		}


		ChangKartMode(Client, SkateID);
	}
	else
	{
		UINT KartID = 10020;

		const char* sql = NULL;
		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{
			sql = "SELECT KartID FROM BaseGameTypeStatus WHERE Uin = ?;";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				if (res->getUInt("KartID") > 0) {
					KartID = res->getUInt("KartID");
				}
				break;
			}
			res.reset();

		}
		catch (const std::exception& e)
		{
			printf(e.what());

			printf("getGameMode3 error\n");
		}

		ChangKartMode(Client, KartID);
	}


}


void NotifyChangeItemStatus2(std::shared_ptr<ClientNode> Client, UINT Uin, USHORT ItemNum, ItemStatus* ItemStatus)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	size_t len;
	UINT KartId = 0;
	Write32(p, Uin);

	Write16(p, ItemNum);
	for (size_t i = 0; i < ItemNum; i++)
	{
		BYTE* pItemStatus = p;
		Write16(pItemStatus, 0); // len
		BYTE NewStatus = ItemStatus[i].NewStatus;
		UINT ItemID = ItemStatus[i].ItemID;
		UINT ItemType = GetItemType(ItemID);
		if (NewStatus)
		{
			if (ItemType == EAIT_CAR || ItemType == EAIT_SKIN)
			{
				KartId = ItemID;
			}
		}
		/*if (ItemStatus[i].NewStatus==1) {
			Client->KartID = ItemStatus[i].ItemID;
		}*/
		Write32(pItemStatus, ItemStatus[i].ItemID);
		Write32(pItemStatus, 1);					  // ItemNum
		Write32(pItemStatus, -1);					  // AvailPeriod
		Write8(pItemStatus, ItemStatus[i].NewStatus); // Status
		Write32(pItemStatus, 1);					  // ObtainTime
		Write32(pItemStatus, 0);					  // OtherAttribute
		Write16(pItemStatus, 0);					  // ItemType

		len = pItemStatus - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); // SpecFlag

	// Write16(p, 0); //KartNum
	// KartId
	//if (KartId >= 1)
	{
		// UINT ShapeSuitID = GetKartItemShapeSuitID(KartId, Uin);
		UINT ShapeSuitID = 0;
		//UINT KartId = 0;
		UINT LegendSuitLevel = 0;
		UINT LegendSuitLevelChoice = 0;
		UINT ShapeLegendSuitID = 0;

		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		try
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ShapeSuitID,LegendSuitLevel,LegendSuitLevelChoice,ShapeLegendSuitID,KartID FROM KartRefit WHERE Uin = ? AND (KartID=? OR ShapeSuitID =?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, KartId);
			prep_stmt->setUInt(3, KartId);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			if (res->rowsCount() > 0) {
				res->next();
				ShapeSuitID = res->getUInt("ShapeSuitID");
				LegendSuitLevel = res->getUInt("LegendSuitLevel");
				LegendSuitLevelChoice = res->getUInt("LegendSuitLevelChoice");
				ShapeLegendSuitID = res->getUInt("ShapeLegendSuitID");
				KartId = res->getUInt("KartId");
			}
			res.reset();
		}
		catch (const std::exception& e)
		{
			printf(e.what());
			printf("NotifyChangeItemStatus2 Error\n");

		}

		Write16(p, 1); // KartNum
		{			   // KartRefitInfo
			BYTE* pKartRefitInfo = p;
			Write16(pKartRefitInfo, 0);						// len
			Write32(pKartRefitInfo, Uin);					// Uin
			Write32(pKartRefitInfo, KartId);				// KartId
			Write32(pKartRefitInfo, 99);					// RefitCout
			Write16(pKartRefitInfo, 0);						// MaxFlags
			Write16(pKartRefitInfo, 0);						// WWeight
			Write16(pKartRefitInfo, 0);						// SpeedWeight
			Write16(pKartRefitInfo, 0);						// JetWeight
			Write16(pKartRefitInfo, 0);						// SJetWeight
			Write16(pKartRefitInfo, 0);						// AccuWeight
			Write32(pKartRefitInfo, 4);						// ShapeRefitCount
			Write32(pKartRefitInfo, 0);						// KartHeadRefitItemID
			Write32(pKartRefitInfo, 0);						// KartTailRefitItemID
			Write32(pKartRefitInfo, 0);						// KartFlankRefitItemID
			Write32(pKartRefitInfo, 0);						// KartTireRefitItemID
			Write32(pKartRefitInfo, 0);						// SecondRefitCount
			Write16(pKartRefitInfo, 0);						// Speed2Weight
			Write16(pKartRefitInfo, 0);						// DriftVecWeight
			Write16(pKartRefitInfo, 0);						// AdditionalZSpeedWeight
			Write16(pKartRefitInfo, 0);						// AntiCollisionWeight
			Write16(pKartRefitInfo, 0);						// LuckyValue
			Write16(pKartRefitInfo, 0);						// RefitLuckyValueMaxWeight
			Write32(pKartRefitInfo, ShapeSuitID);			// ShapeSuitID
			Write8(pKartRefitInfo, LegendSuitLevel);		// LegendSuitLevel
			Write32(pKartRefitInfo, LegendSuitLevelChoice); // LegendSuitLevelChoice
			Write32(pKartRefitInfo, ShapeLegendSuitID);		// ShapeLegendSuitID

			len = pKartRefitInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	//else
	//{
	//	Write16(p, 0); // KartNum
	//}

	Write8(p, 0); // ChangeType

#ifndef ZingSpeed
	Write8(p, 0); // EquipNiChangItemNum

#endif

	len = p - buf.data();
	SendToClient(Client, 527, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Notify);// delete[] buf;
}

void NotifyG2CFizzStarCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "000A00000000000000000000000007080000001E04000A00000001000027DF001A01001E05000062CA000027DF00003987000047310000304E001A02003C0500002F17000040700000358C00003055000045190016030064040000563C0000281300004BCD000058AC04001C00001E00000400003C5C000037EE000058E200003F040000000A001C010064000004000044FB00005642000035DB00006AAD00000014001C0200B40005040000398F0000563900004879000069BE0000001E001803012C002803000037170000487600002CFF00000032");
	len = p - buf;
	SendToClient(Client, 11595, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	Write16(p, 0); //MaxFizzSourceTotalCount
	Write16(p, 0); //MaxFizzSourceByTimeOneDay
	Write16(p, 0); //MaxFizzSourceByRequestandBeDonatedOneDay
	Write16(p, 0); //MaxDonateFizzSourceCountOneDay
	Write32(p, 0); //FizzSourceeDonatedCoolTimeGap
	Write32(p, 0); //FizzSourceadditionTimeGap
	Write32(p, 0); //FizzStarNumUpRank
	Write8(p, 0); //DailyFizzLotteryNum
	for (size_t i = 0; i < 0; i++)
	{ //DailyFizzLotteryInfo
		BYTE* pDailyFizzLotteryInfo = p;
		Write16(pDailyFizzLotteryInfo, 0); //len

		Write8(pDailyFizzLotteryInfo, 0); //LotteryType
		Write16(pDailyFizzLotteryInfo, 0); //FizzStarNum
		Write8(pDailyFizzLotteryInfo, 0); //ControlledItemNum
		Write32(pDailyFizzLotteryInfo, 0); //ControlledItem[]

		len = pDailyFizzLotteryInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //PeriodFizzLotteryNum
	for (size_t i = 0; i < 0; i++)
	{ //PeriodFizzLotteryInfo
		BYTE* pPeriodFizzLotteryInfo = p;
		Write16(pPeriodFizzLotteryInfo, 0); //len

		Write8(pPeriodFizzLotteryInfo, 0); //LotteryType
		Write16(pPeriodFizzLotteryInfo, 0); //FizzStarNum
		Write16(pPeriodFizzLotteryInfo, 0); //FizzStarRank
		Write8(pPeriodFizzLotteryInfo, 0); //ControlledItemNum
		Write32(pPeriodFizzLotteryInfo, 0); //ControlledItem[]
		Write32(pPeriodFizzLotteryInfo, 0); //ExpIncRate

		len = pPeriodFizzLotteryInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 11595, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyItemType2ndCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "086C000000D7000A00023BB300000943000A00023B4100000943000A0001EC230000094D000A000215CA0000094D000A00020FD70000094D000A000215BF0000094D000A000215EA00000000000A00020FD600000000000A000215C900000000000A000215C500000000000A0001EC1B00000000000A000215C000000000000A000215D200000000000A0001D6CA0000094C000A000222C10000094C000A000215E90000094C000A0001EC1E0000094C000A000215CB0000094C000A000215C60000094C000A000215B20000094C000A0001EC3A0000094C000A0001EC300000094C000A0001EC420000094C000A0001EC460000094C000A0001EC5000000000000A0001EC3B0000094D000A0001FA080000094D000A00020FCB0000094C000A0001EC630000094C000A00020FD00000094C000A00020FD50000094D000A00020FD90000094C000A000215B100000000000A0001B10900000943000A0002154000000943000A0001B10700000943000A0001ED7D00000943000A00020F1800000943000A0002094900000943000A0001DC8200000943000A0001A82000000943000A0001935000000000000A0001935100000000000A0001E25100000000000A0001E21000000000000A0001E20C00000000000A0001DCC200000000000A0001D6C000000000000A0001D6A700000000000A0001D65900000000000A0002154200000000000A0001D0C500000000000A0002154500000000000A0001D65B00000000000A0001D66400000000000A0001D0CB00000000000A00020FE100000000000A00020FBF00000000000A00020FC500000000000A00020A1700000000000A00020A0A00000000000A000209F900000000000A000203EB00000000000A000203E900000000000A000203C400000000000A0001D6D600000000000A0001D6D700000000000A0001DCAF00000000000A00020A0100000000000A0001E2320000094C000A0001E2600000094C000A0001E2620000094C000A0001E2670000094C000A0001E23B0000094C000A0001E20F0000094C000A0001E20B0000094C000A0001DCE80000094C000A0001DCF00000094C000A0001DCE20000094C000A000209FF0000094C000A00020A000000094C000A00020FC70000094C000A0001DCAD0000094C000A0001DCC30000094C000A0001D6E90000094C000A0001D6EA0000094C000A0001D6D50000094C000A0001D6C70000094C000A0001D6D10000094C000A0001D6B60000094C000A0001D69B0000094C000A0001D69C0000094C000A0001D6A20000094C000A0001DCFB0000094C000A0001E2580000094D000A0001E23A0000094D000A0001E2370000094D000A0001E2350000094D000A0001DD090000094D000A0001DCEC0000094D000A00020A050000094D000A00020FCA0000094D000A0001D6EF0000094D000A0001DCC90000094D000A0001DCB20000094D000A0001DCC50000094D000A0001D6C50000094D000A0001D6D20000094D000A0001D6AA0000094D000A0001D6950000094D000A0001D65F0000094D000A0001D67F0000094D000A0001D6780000094D000A0001E2420000094D000A0001E2590000094D000A0001D0FD0000094D000A0001D0E60000094D000A0001D0D00000094D000A0001EDF10000094D000A0001D0C90000094D000A00020FB90000094D000A00020FBE0000094D000A00020A240000094D000A00020A200000094D000A00020A1C0000094D000A00020A160000094D000A00020A0F0000094D000A00020A0E0000094D000A000203F50000094D000A000203E60000094D000A000203E70000094D000A000203D80000094D000A000203E10000094D000A000203DB0000094D000A000203E50000094D000A000203D20000094D000A000203D70000094D000A000203CF0000094D000A000203C70000094D000A000203BE0000094D000A000203B90000094D000A0001FEBA0000094D000A0001FEB30000094D000A0001FEA90000094D000A0001FEA10000094D000A0001FEB60000094D000A0001FA010000094D000A0001FA0D0000094D000A0001FA0C0000094D000A0001FA280000094D000A0001FA2D0000094D000A0001F9FD0000094D000A0001FE8F0000094D000A0001FA340000094D000A0001F9FB0000094D000A0001F2F60000094D000A0001F2E70000094D000A0001F2A10000094D000A0001EE380000094D000A0001EE2C0000094D000A0001F2AC0000094D000A0001F2B50000094D000A0001F2C50000094D000A0001F2DB0000094D000A0001F2ED0000094D000A0001F2A30000094D000A0001F2DF0000094D000A0001EE2F0000094D000A0001D6AF0000094D000A0001EE2B0000094D000A0001EC3C0000094D000A0001EC5C0000094D000A0001EE0C0000094D000A0001EE2E0000094D000A0001EE200000094D000A0001EE260000094D000A0001EC580000094D000A0001E2150000094D000A0001D6540000094D000A0001D0C60000094D000A0001D6810000094D000A0001D1110000094D000A0001BD600000094D000A0001CC270000094D000A0001D0E50000094D000A0001D0F20000094D000A0001C33F0000094D000A0001D0DE0000094D000A0001CC4A0000094D000A0001C36A0000094D000A0001D0D60000094D000A0001BD120000094D000A0001C33D0000094D000A0001C32E0000094D000A0001C3310000094D000A0001BD190000094D000A0001BD1D0000094D000A0001BD210000094D000A0001C34B0000094D000A0001CC380000094D000A0001CC530000094D000A0001C34D0000094D000A0001C36B0000094D000A0001C3A60000094D000A0001CC2E0000094D000A0001CC8C0000094D000A0001BD050000094D000A0001BCF80000094D000A0001ED7800000943000A0001ED7600000943000A0001D61300000943000A0001CFEF00000943000A0001AF1E00000943000A0001AF1F00000943000A0001813C00000943");
	len = p - buf;
	SendToClient(Client, 314, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	{ //ItemType2ndCfg
		BYTE* pItemType2ndCfg = p;
		Write16(pItemType2ndCfg, 0); //len

		Write32(pItemType2ndCfg, 0); //Num
		for (size_t i = 0; i < 0; i++)
		{ //Info
			BYTE* pInfo = pItemType2ndCfg;
			Write16(pInfo, 0); //len

			Write32(pInfo, 0); //ItemID
			Write32(pInfo, 0); //Itemtype2nd

			len = pInfo - pItemType2ndCfg;
			Set16(pItemType2ndCfg, (WORD)len);
			pItemType2ndCfg += len;
		}

		len = pItemType2ndCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 314, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyMultiUseItemListCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "003A0000000D00006A0200007CDD0000F84F0000F8500000F851000100A2000100A3000100A4000100A5000100A6000100A7000076F30000F844");
	len = p - buf;
	SendToClient(Client, 347, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	{ //MultiUseItemList
		BYTE* pMultiUseItemList = p;
		Write16(pMultiUseItemList, 0); //len

		Write32(pMultiUseItemList, 0); //ItemNum
		Write32(pMultiUseItemList, 0); //ItemId[]

		len = pMultiUseItemList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 347, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyMusicBlackList(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "000B002B0027536F756E64735C44616E63654D6F64655C7765696E6966656E676B75616E672D3133352E6F676700250021536F756E64735C44616E63654D6F64655C534558594C4F56452D3133322E6F6767002A0026536F756E64735C44616E63654D6F64655C68756166616E6767756E69616E672D39322E6F67670023001F536F756E64735C44616E63654D6F64655C6261646769726C2D36342E6F676700270023536F756E64735C44616E63654D6F64655C636F6E6763697969686F752D39362E6F67670020001C536F756E64735C44616E63654D6F64655C6A756C792D38302E6F6767002A0026536F756E64735C44616E63654D6F64655C64616F6A69616E72756D656E672D3134352E6F67670023001F536F756E64735C44616E63654D6F64655C7869616E676E692D38392E6F6767002A0026536F756E64735C44616E63654D6F64655C646177616E6B75616E6D69616E2D3130302E6F676700290025736F756E64735C44616E63654D6F64655C77616E676A69616E77616E672D3132362E6F6767002A0026736F756E64735C44616E63654D6F64655C35303030372D6C69616E6169646172656E2E6F676700260000018E000002A10000C3A50000C3710000C46A0000C46B0000C46C0000C46D0000C46E0000C46F0000C4700000C47100000300000003010000030200000303000003040000030500000306000003070000009100000148000002660000026700000270000003740000C35A0000C4AE000003930000039B000003C9000003DB0000041D0000043B000004EC000002730000C3570000008D");
	len = p - buf;
	SendToClient(Client, 952, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write16(p, 0); //AddressNum
	for (size_t i = 0; i < 0; i++)
	{ //Address
		BYTE* pAddress = p;
		Write16(pAddress, 0); //len

		Write16(pAddress, 0); //Len

		len = pAddress - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write16(p, 0); //IDNum
	Write32(p, 0); //ID[]

	len = p - buf;
	SendToClient(Client, 952, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientPersonalGardenCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "0D002E0000000000000000000000010000000100000002000000020000000200000003000000000000000000000000002E0000000100000000000000010000000200000002000000020000000200000002000000000000000000000000002E0000000200000000000000030000000400000005000000060000000700000008000000000000000000000000002E0000000300000000000000010000000200000004000000040000000500000006000000000000000000000000002E0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002E00000005000000000000000F0000000F0000000F0000000F0000000F0000000F000000000000000000000000002E0000000600000000000000020000000200000003000000030000000400000006000000000000000000000000002E0000000700000000000000000000000000000000000000000000000000000001000000000000000000000000002E000000080000000000000005000000060000000700000008000000090000000A000000000000000000000000002E000000090000000000000004000000050000000600000007000000080000000A000000000000000000000000002E0000000A00000000000000020000000200000003000000030000000400000006000000000000000000000000002E0000000B0000000000000000000001F4000007080000119400001F40000036B0000000000000000000000000002E0000000C0000000100000004000000050000000600000007000000080000000A000000000000000000000000001400040000272400004B5E000081FB00007074000000020000000500003840000000010000000100000002");
	len = p - buf;
	SendToClient(Client, 20145, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write8(p, 0); //PrivilegeNum
	for (size_t i = 0; i < 0; i++)
	{ //Privilege
		BYTE* pPrivilege = p;
		Write16(pPrivilege, 0); //len

		Write32(pPrivilege, 0); //Type
		Write32(pPrivilege, 0); //Value[]

		len = pPrivilege - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //ParkingBlacklist
		BYTE* pParkingBlacklist = p;
		Write16(pParkingBlacklist, 0); //len

		Write16(pParkingBlacklist, 0); //BlackNum
		Write32(pParkingBlacklist, 0); //BlaskList[]

		len = pParkingBlacklist - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //UnlockCost
	Write32(p, 0); //UpgradeBuildingLevelCost
	Write32(p, 0); //ParkingTimeout
	Write32(p, 0); //DefaultOutParkNum
	Write32(p, 0); //DefaultDayParkOtherNum
	Write32(p, 0); //ShowRubberDuckLevel

	len = p - buf;
	SendToClient(Client, 20145, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyClientDomainResolve(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "04003C001003AD6C24B79A6AFB3A228D33790027446706FA52000000013138322E3235342E3131362E313137000000000000000000000000000000000000400014044DED6071152CF13D3BBAF178C5BF337900271B6706FA52000000013138322E3235342E3131362E31313700000000000000000000000000000000000040001404BFEB03B7A951FB3A4B60E8B72483337900272D6706FA52000000013138322E3235342E3131362E3131370000000000000000000000000000000000003C0010031B06813D4ED6F73AA22433790027136706FA52000000013138322E3235342E3131362E3131370000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 2100, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write8(p, 0); //DomainCnt
	for (size_t i = 0; i < 0; i++)
	{ //DomainList
		BYTE* pDomainList = p;
		Write16(pDomainList, 0); //len

		{ //MultiIPInfo
			BYTE* pMultiIPInfo = pDomainList;
			Write16(pMultiIPInfo, 0); //len

			Write8(pMultiIPInfo, 0); //IPCount
			Write32(pMultiIPInfo, 0); //ServerIP[]
			Write8(pMultiIPInfo, 0); //DomainLen

			len = pMultiIPInfo - pDomainList;
			Set16(pDomainList, (WORD)len);
			pDomainList += len;
		}
		Write16(pDomainList, 0); //Port
		Write32(pDomainList, 0); //Uin
		Write32(pDomainList, 0); //AreaID
		WriteString(pDomainList, 0); //HttpDNSAddr[]

		len = pDomainList - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 2100, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestWorldOnlineInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);


	ResponseWorldOnlineInfo(Client);
}

void ResponseWorldOnlineInfo(std::shared_ptr<ClientNode> Client)
{

	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data();
		size_t len;

		WriteHex(p, "3C3424012EA80105022E0101022A000B09003D000C2710177A00374B0E7927F627F642769CE30001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D000927100C8800554B0E7927972796B3EAEAFB0001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D0004271009A600374B0E792780277E6310FCC70001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D00022710099800554B0E7927362734EED343790001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D000A2710099200554B0E79279A279974ADB01F0001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D000D271007EA00374B0E7927FA27F9F3C2184C0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D000E2710075300374B0E7927FC27FC770738870001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D000B271006A000554B0E79279E279CCC1B64240001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D000327100334003F4B0E7927872787CD2D9EBF00010026043F4B0E797635F13D1A01357085E8F46D1264786773332E73706565642E71712E636F6D035F0201035B000B0E003D00321770096500554B0E7927102710D00DF4760001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D00432710090F003F4B0E79278F278D9B6DCDE000010026043F4B0E797635F13D1A01357085E8F46D1264786773332E73706565642E71712E636F6D003D0040271008F300224B0E79278327817E588B8E0001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D00442710069600374B0E792782278100B50F1E0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D00452710068600374B0E7927842784ADD473C80001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D003D2710059B00554B0E79278827871D037DDE0001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D00412710059A00224B0E79277427721530F05A0001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D004227100592003F4B0E79278C278A72987E3100010026043F4B0E797635F13D1A01357085E8F46D1264786773332E73706565642E71712E636F6D003D003F2710057B00224B0E79278427840913B4C60001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D00472710055E00554B0E7927A527A5F5DB0C080001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D003C2710055100554B0E792786278481CF65A00001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D00492710054A00374B0E7928042802C827CC0A0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D003E2710054000554B0E7927772775B8D8429D0001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D00482710034D00374B0E79280127FF584738FD0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D095403010950000B27003D036827100D4900374B0E7927DA27D862D5F2F10001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D008F27100B7800224B0E7927752775C49896250001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D009027100ACF00224B0E79277A2778F252EF030001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D036627100ACC00374B0E7927D227D2BAD9F0E40001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D009327100ABA003F4B0E7927922790F7B00DF500010026043F4B0E797635F13D1A01357085E8F46D1264786773332E73706565642E71712E636F6D003D009727100AB800374B0E7927872787EF39D24D0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D009927100AB700374B0E79278E278DB80B31570001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D036727100AB500374B0E7927D627D54968655F0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D009527100AA4003F4B0E79279627961B2D762500010026043F4B0E797635F13D1A01357085E8F46D1264786773332E73706565642E71712E636F6D003D009827100A7200374B0E79278B278A15106C870001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D009127100A6600224B0E79277C277BBC264C230001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D008A27100A4100224B0E79271627162D9446280001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D008227100A3400224B0E7927A827A87436E0810001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D036A27100A0500374B0E7927E027DE7B259D490001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D007F271009DE00554B0E7927B727B65B42AA310001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D00882710098500224B0E79273727350C97612F0001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D00802710097400554B0E7927BB27B95B5B63AD0001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D008527100950003F4B0E7927B827B66C4738D000010026043F4B0E797635F13D1A01357085E8F46D1264786773332E73706565642E71712E636F6D003D008B2710094F00554B0E7927792778B36D72D60001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D009A2710094300374B0E79279227902A5234BF0001002604374B0E791635F13D22013570E0E8F46D1264786773342E73706565642E71712E636F6D003D008E2710092F00554B0E7927812781CB16538A0001002604554B0E795C35F13D13013570D3E8F46D1264786773312E73706565642E71712E636F6D003D0089271008F400224B0E79271B27198CC259A10001002604224B0E796435F13D2F01357088E8F46D1264786773322E73706565642E71712E636F6D003D0092271008F100224B0E792780277E1F8116CE0001002604");
		len = p - buf.data();
		SendToClient(Client, 1400, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		return;
	}


	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write8(p, 0); //UpdateOnlineInfoInterval
	{ //Info
		BYTE* pInfo = p;
		Write16(pInfo, 0); //len

		Write8(pInfo, 0); //CountryCount
		for (size_t i = 0; i < 0; i++)
		{ //CountryInfo
			BYTE* pCountryInfo = pInfo;
			Write16(pCountryInfo, 0); //len

			Write8(pCountryInfo, 0); //CountryID
			Write8(pCountryInfo, 0); //ChannelNum
			for (size_t i = 0; i < 0; i++)
			{ //ChannelInfo
				BYTE* pChannelInfo = pCountryInfo;
				Write16(pChannelInfo, 0); //len

				Write8(pChannelInfo, 0); //ChannelID
				Write8(pChannelInfo, 0); //IDCNum
				for (size_t i = 0; i < 0; i++)
				{ //IDCServerInfo
					BYTE* pIDCServerInfo = pChannelInfo;
					Write16(pIDCServerInfo, 0); //len

					Write16(pIDCServerInfo, 0); //LocationID
					Write8(pIDCServerInfo, 0); //GameServerNum
					for (size_t i = 0; i < 0; i++)
					{ //GameServerInfo
						BYTE* pGameServerInfo = pIDCServerInfo;
						Write16(pGameServerInfo, 0); //len

						Write32(pGameServerInfo, 0); //ServerID
						Write16(pGameServerInfo, 0); //MaxPlayerNum
						Write16(pGameServerInfo, 0); //CurrentPlayerNum
						Write8(pGameServerInfo, 0); //PlayerIncPerSec
						Write32(pGameServerInfo, 0); //ServerIP
						Write16(pGameServerInfo, 0); //TcpPort
						Write16(pGameServerInfo, 0); //UdpPort
						Write32(pGameServerInfo, 0); //RandomKey
						Write8(pGameServerInfo, 0); //IsDisabledLogin
						Write8(pGameServerInfo, 0); //HasMultiIPInfo
						for (size_t i = 0; i < 0; i++)
						{ //MultiIPInfo
							BYTE* pMultiIPInfo = pGameServerInfo;
							Write16(pMultiIPInfo, 0); //len

							Write8(pMultiIPInfo, 0); //IPCount
							Write32(pMultiIPInfo, 0); //ServerIP[]
							Write8(pMultiIPInfo, 0); //DomainLen

							len = pMultiIPInfo - pGameServerInfo;
							Set16(pGameServerInfo, (WORD)len);
							pGameServerInfo += len;
						}

						len = pGameServerInfo - pIDCServerInfo;
						Set16(pIDCServerInfo, (WORD)len);
						pIDCServerInfo += len;
					}

					len = pIDCServerInfo - pChannelInfo;
					Set16(pChannelInfo, (WORD)len);
					pChannelInfo += len;
				}

				len = pChannelInfo - pCountryInfo;
				Set16(pCountryInfo, (WORD)len);
				pCountryInfo += len;
			}

			len = pCountryInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //BorderServerCount
		for (size_t i = 0; i < 0; i++)
		{ //BorderServerInfo
			BYTE* pBorderServerInfo = pInfo;
			Write16(pBorderServerInfo, 0); //len

			Write16(pBorderServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pBorderServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pBorderServerInfo;
				Set16(pBorderServerInfo, (WORD)len);
				pBorderServerInfo += len;
			}

			len = pBorderServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //MatchServerCount
		for (size_t i = 0; i < 0; i++)
		{ //MatchServerInfo
			BYTE* pMatchServerInfo = pInfo;
			Write16(pMatchServerInfo, 0); //len

			Write16(pMatchServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pMatchServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pMatchServerInfo;
				Set16(pMatchServerInfo, (WORD)len);
				pMatchServerInfo += len;
			}

			len = pMatchServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //RelaxServerCount
		for (size_t i = 0; i < 0; i++)
		{ //RelaxServerInfo
			BYTE* pRelaxServerInfo = pInfo;
			Write16(pRelaxServerInfo, 0); //len

			Write16(pRelaxServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pRelaxServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pRelaxServerInfo;
				Set16(pRelaxServerInfo, (WORD)len);
				pRelaxServerInfo += len;
			}

			len = pRelaxServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //TeamServerCount
		for (size_t i = 0; i < 0; i++)
		{ //TeamServerInfo
			BYTE* pTeamServerInfo = pInfo;
			Write16(pTeamServerInfo, 0); //len

			Write16(pTeamServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pTeamServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pTeamServerInfo;
				Set16(pTeamServerInfo, (WORD)len);
				pTeamServerInfo += len;
			}

			len = pTeamServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //DanceServerCount
		for (size_t i = 0; i < 0; i++)
		{ //DanceServerInfo
			BYTE* pDanceServerInfo = pInfo;
			Write16(pDanceServerInfo, 0); //len

			Write16(pDanceServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pDanceServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pDanceServerInfo;
				Set16(pDanceServerInfo, (WORD)len);
				pDanceServerInfo += len;
			}

			len = pDanceServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //SlideServerCount
		for (size_t i = 0; i < 0; i++)
		{ //SlideServerInfo
			BYTE* pSlideServerInfo = pInfo;
			Write16(pSlideServerInfo, 0); //len

			Write16(pSlideServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pSlideServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pSlideServerInfo;
				Set16(pSlideServerInfo, (WORD)len);
				pSlideServerInfo += len;
			}

			len = pSlideServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //FBServerCount
		for (size_t i = 0; i < 0; i++)
		{ //FBServerInfo
			BYTE* pFBServerInfo = pInfo;
			Write16(pFBServerInfo, 0); //len

			Write16(pFBServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pFBServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pFBServerInfo;
				Set16(pFBServerInfo, (WORD)len);
				pFBServerInfo += len;
			}

			len = pFBServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //WlMatchSvrCount
		for (size_t i = 0; i < 0; i++)
		{ //WlMatchSvrInfo
			BYTE* pWlMatchSvrInfo = pInfo;
			Write16(pWlMatchSvrInfo, 0); //len

			Write16(pWlMatchSvrInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pWlMatchSvrInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pWlMatchSvrInfo;
				Set16(pWlMatchSvrInfo, (WORD)len);
				pWlMatchSvrInfo += len;
			}

			len = pWlMatchSvrInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //SkateServerCount
		for (size_t i = 0; i < 0; i++)
		{ //SkateServerInfo
			BYTE* pSkateServerInfo = pInfo;
			Write16(pSkateServerInfo, 0); //len

			Write16(pSkateServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pSkateServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pSkateServerInfo;
				Set16(pSkateServerInfo, (WORD)len);
				pSkateServerInfo += len;
			}

			len = pSkateServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //MRServerCount
		for (size_t i = 0; i < 0; i++)
		{ //MRServerInfo
			BYTE* pMRServerInfo = pInfo;
			Write16(pMRServerInfo, 0); //len

			Write16(pMRServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pMRServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pMRServerInfo;
				Set16(pMRServerInfo, (WORD)len);
				pMRServerInfo += len;
			}

			len = pMRServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}
		Write8(pInfo, 0); //NpcServerCount
		for (size_t i = 0; i < 0; i++)
		{ //NpcServerInfo
			BYTE* pNpcServerInfo = pInfo;
			Write16(pNpcServerInfo, 0); //len

			Write16(pNpcServerInfo, 0); //LocationID
			{ //OnlineInfo
				BYTE* pOnlineInfo = pNpcServerInfo;
				Write16(pOnlineInfo, 0); //len

				Write32(pOnlineInfo, 0); //ServerID
				Write16(pOnlineInfo, 0); //MaxPlayerNum
				Write16(pOnlineInfo, 0); //CurrentPlayerNum
				Write8(pOnlineInfo, 0); //PlayerIncPerSec
				Write32(pOnlineInfo, 0); //ServerIP
				Write16(pOnlineInfo, 0); //TcpPort
				Write16(pOnlineInfo, 0); //UdpPort
				Write32(pOnlineInfo, 0); //RandomKey
				Write8(pOnlineInfo, 0); //IsDisabledLogin
				Write8(pOnlineInfo, 0); //HasMultiIPInfo
				for (size_t i = 0; i < 0; i++)
				{ //MultiIPInfo
					BYTE* pMultiIPInfo = pOnlineInfo;
					Write16(pMultiIPInfo, 0); //len

					Write8(pMultiIPInfo, 0); //IPCount
					Write32(pMultiIPInfo, 0); //ServerIP[]
					Write8(pMultiIPInfo, 0); //DomainLen

					len = pMultiIPInfo - pOnlineInfo;
					Set16(pOnlineInfo, (WORD)len);
					pOnlineInfo += len;
				}

				len = pOnlineInfo - pNpcServerInfo;
				Set16(pNpcServerInfo, (WORD)len);
				pNpcServerInfo += len;
			}

			len = pNpcServerInfo - pInfo;
			Set16(pInfo, (WORD)len);
			pInfo += len;
		}

		len = pInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 1400, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void ResponseLmtUpdateStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write8(p, 0); //ReasonLen
	Write8(p, 1); //PlayerStatus
	Write32(p, 0); //LeftTime

	len = p - buf;
	SendToClient(Client, 382, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestChangeRecommandStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR Idle = Read8(p);

	{
		BYTE buf[8192]; BYTE* p = buf; size_t len;

		Write16(p, 0x0039); //ResultID

		len = p - buf;
		SendToClient(Client, 275, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}

void NotifyLmtMatchOver(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write8(p, 0); //OverReason
	Write16(p, 0); //Rank
	Write8(p, 0); //AwardItemNum
	Write32(p, 0); //AwardItem[]
	Write32(p, 0); //AwardLuckMoney
	Write32(p, 0); //AwardLuckyMatchPoint
	Write16(p, 0); //WinNum

	len = p - buf;
	SendToClient(Client, 805, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestLmtBuglet(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	UCHAR OpType = Read8(p);
	ResponsetLmtBuglet(Client, OpType);
}

void ResponsetLmtBuglet(std::shared_ptr<ClientNode> Client, UCHAR OpType)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //ResultID
	Write8(p, OpType); //OpType
	Write8(p, 0); //ReasonLen
	Write8(p, 0); //State
	Write16(p, 3); //LeftTimes
	Write32(p, 50); //LuckMoney

	len = p - buf;
	SendToClient(Client, 812, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void NotifyClientNoviceGuideCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "000000008B371000E20200730000009D01006C000000000000000000000000000300190000320700000001000000A800670F2B2C0000000000000019000031ED000000010000016800670F2B2C000000000000001900002B8200000001000000A800670F2B2C0000000000000000000000000000000000000000000000006C0100690000013D007000030113B3C9B3A4C8CECEF12DB7C9B3B5B4F3D1A7CCC322CDEAB3C9B7C9B3B5B4F3D1A7CCC3D6D0B5C42F433032A1BEC6AFD2C6A1BF2F43303026CCE1C9FDC4FAB5C4B7C9B3B5BCBCC7C97EB2A2BBF1B5C32F433032333330BEADD1E92F43303000E20300730000002601006C000000000000000000000000000300190000508E000000010000016800670F2B2C00000000000000190000280800000001000002D000670F2B2C000000000000001900005E8100000001000000A800670F2B2C0000000000000000000000000000000000000000000000006C0100690000013E007000030113B3C9B3A4C8CECEF12DB7C9B3B5B4F3D1A7CCC322CDEAB3C9B7C9B3B5B4F3D1A7CCC3D6D0B5C42F433032A1BED0A1C5E7A1BF2F43303026CCE1C9FDC4FAB5C4B7C9B3B5BCBCC7C97EB2A2BBF1B5C32F433032323030BEADD1E92F43303000CB05005A000000030100530000000000000000000000000002001900010AC3000000010000016800670F2B2C00000000000000190000271E00000001000000A800670F2B2C000000000000000000C800000000000000000000000000006E01006B00000963007000020113D6D5C9EDC8CECEF12DB5C0BEDFC8FCD1B5C1B73BD4DA2F433032A1BEB8F6C8CBB5C0BEDFC4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC0CFBDD6B9DCB5C0A1BF31BED62F4330300F2F433032333030BEADD1E92F43303000E40700730000009E01006C0000000000000000000000000003001900010ABE000000010000016800670F2B2C000000000000001900002F3300000001000000A800670F2B2C000000000000001900002C9600000001000000A800670F2B2C0000000000000000000000000000000000000000000000006E01006B00000965007000010113D6D5C9EDC8CECEF12DBEBACBD9C8FCD1B5C1B73BD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC0CFBDD6B9DCB5C0A1BF31BED62F4330300F2F433032313630BEADD1E92F433030013408005A0000001001005300000000000000000000000000020019000032FC00000001000000A800670F2B2C000000000000001900005BAC00000001000002D000670F2B2C0000000000000000012C0000000000000000000000000000D70200690000013F009600030113B3C9B3A4C8CECEF12DB7C9B3B5B4F3D1A7CCC322CDEAB3C9B7C9B3B5B4F3D1A7CCC3D6D0B5C42F433032A1BEBFD5C5E7A1BF2F43303026CCE1C9FDC4FAB5C4B7C9B3B5BCBCC7C97EB2A2BBF1B5C32F433032323030BEADD1E92F433030006B00000967008F00020113D6D5C9EDC8CECEF12DB5C0BEDFC8FCD1B5C1B73BD4DA2F433032A1BEB8F6C8CBB5C0BEDFC4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB9ECB5C03233C7F8A1BF31BED62F4330300F2F433032363130BEADD1E92F43303001770A00730000001101006C000000000000000000000000000300190000544C00000001000002D000670F2B2C000000000000001900003057000000030000000000670F2B2C0000000000000019000033EE00000001000000A800670F2B2C000000000000000000000000000000000000000000000001010200910000000E009600050115B3C9B3A4C8CECEF12DB3F5BCB6BCDDD5D5BFBCC8A10CBFBCC8A1B3F5BCB6BCDDD5D562CDEAB3C9CBF9D3D0B3F5BCB6BCDDD5D5BFBCCAD4BAF3A3ACBFC9D2D4BBF1B5C32F433032343030BEADD1E92F433030BACD2F433032B3F5BCB6BCDDD5D52F4330300ABBF1B5C3B3F5BCB6BCDDD5D5BAF3A3ACD6BBC4DCBDF8C8EBB3F5BCB6C6B5B5C0006D0000096900BA00010113D6D5C9EDC8CECEF12DBEBACBD9C8FCD1B5C1B73DD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB1F9B4A8BBACD1A9B3A1A1BF32BED62F4330300F2F433032353330BEADD1E92F43303001A90C005A0000009F0100530000000000000000000000C80002001900010DD200000001000000A800670F2B2C0000000000000019000029AA00000001000000A800670F2B2C0000000000000000000000000000000000000000000000014C03006B00000148009600030113B3C9B3A4C8CECEF12DB7C9B3B5B4F3D1A7CCC324CDEAB3C9B7C9B3B5B4F3D1A7CCC3D6D0B5C42F433032A1BEC2E4B5D8C5E7A1BF2F43303026CCE1C9FDC4FAB5C4B7C9B3B5BCBCC7C97EB2A2BBF1B5C32F433032343030BEADD1E92F433030006F0000096B007102010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB1F5BAA3C9B3CCB2A1BF31BED62F4330300F2F433032363230BEADD1E92F433030006F0000096C006F02020013C3BFC8D5C8CECEF12DB5C0BEDFC8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBB5C0BEDFC4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB3C7CAD0CCD8C7F8A1BF31BED62F4330300F2F433032363030BEADD1E92F43303001BE0F00730000002701006C000000000000000000000000000300190000849C00000001000002D000670F2B2C000000000000001900002E1500000001000000A800670F2B2C0000000000000019000062CA00000001000000A800670F2B2C0000000000000000000000000000000000000000000000014803006900000149007000030113B3C9B3A4C8CECEF12DB7C9B3B5B4F3D1A7CCC322CDEAB3C9B7C9B3B5B4F3D1A7CCC3D6D0B5C42F433032A1BEB5AAC6F8A1BF2F43303026CCE1C9FDC4FAB5C4B7C9B3B5BCBCC7C97EB2A2BBF1B5C32F433032353030BEADD1E92F433030006C0000014A009000030113B3C9B3A4C8CECEF12DB7C9B3B5B4F3D1A7CCC325CDEAB3C9B7C9B3B5B4F3D1A7CCC3D6D0B5C42F433032A1BE52BCFCB8B4CEBBA1BF2F43303026CCE1C9FDC4FAB5C4B7C9B3B5BCBCC7C97EB2A2BBF1B5C32F433032353030BEADD1E92F43303000700000096F006902010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB3C1CBAFC9ADC1D6A1BF33BED62F433030102F43303232333030BEADD1E92F43303001A514005A000000A001005300000000000000000000012C00020019000031F000000001000000A800670F2B2C00000000000000190000511D000000010000016800670F2B2C00000000000000000000000000000000000000000000000148030065000002FC00AB00090111B3C9B3A4C8CECEF12DBEE7C7E9C8CECEF1362F433032D0C2CAD6C4D1B6C82F433030CFC2CDEAB3C9BEE7C7E9C8CECEF12F433032A1BEC8ABCAC0BDE7B5C4B9D8D7A2A1BF2F43303010BEE7C7E9C8CECEF1CDA8B9D8BDB1C0F8006F00000971008102010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB3C7CAD0BBF0BEE6A1BF31BED62F4330300F2F433032373530BEADD1E92F43303000710000097200A202010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB2CABAE7B7E7B3B5B5BAA1BF31BED62F4330300F2F433032373530BEADD1E92F43303001CA1900730000028D01006C0000000000000000000000000003001900007CC700000001000002D000670F2B2C0000000000000019000083BB00000001000000A800670F2B2C000000000000001900005D0900000001000000A800670F2B2C000000000000000000000000000000000000000000000001540300710000097300FD02010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC2EDB4EFBCD3CBB9BCD3A1BF32BED62F4330300F2F433032383030BEADD1E92F433030006F00000974009E02010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC3B5B9E5D6AEC1B5A1BF32BED62F4330300F2F433032383030BEADD1E92F433030007100000975010802010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BED1BCD1BCCBAEC0D6D4B0A1BF32BED62F4330300F2F433032383030BEADD1E92F43303001B21E00730000001601006C000000000000000000000000000300190000512A000000010000016800670F2B2C0000000000000019000121EA00000001000000A800670F2B2C0000000000000019000062C600000001000000F000670F2B2C0000000000000000000000000000000000000000000000013C03005700000976000000080115D6D5C9EDC8CECEF12DB9DBBFB4C2BCCFF1BBD8B7C524D4DA2F433032A1BEC2BCCFF1BBD8B7C5A1BF2F433030D6D0B2A5B7C5D2BBBED6C2BCCFF1102F43303231303530BEADD1E92F433030007100000977013402010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC0EFD4BCB4F3C3B0CFD5A1BF33BED62F4330300F2F433032373530BEADD1E92F433030007100000978014502010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC0EFD4BCB0C2D4CBBBE1A1BF33BED62F4330300F2F433032373530BEADD1E92F43303001ED2300730000028E01006C0000000000000000000000000003001900005332000000010000000000670F2B2C000000000000001900005AB800000001000000A800670F2B2C00000000000000190000358C000000010000000000670F2B2C000000000000000000000000000000000000000000000001770300940000001E006900050115B3C9B3A4C8CECEF12DD6D0BCB6BCDDD5D5BFBCC8A10CBFBCC8A1D6D0BCB6BCDDD5D565CDEAB3C9CBF9D3D0D6D0BCB6BCDDD5D5BFBCCAD4BAF3A3ACBFC9D2D4BBF1B5C32F43303231303530BEADD1E92F433030BACDD6D0BCB6BCDDD5D50ABBF1B5C3D6D0BCB6BCDDD5D5BAF3A3ACBFC9D2D4BDF8C8EBB3F5BCB6C6B5B5C0BACDD6D0BCB6C6B5B5C000710000097A00B102010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BECBAEB3C7CDFEC4E1CBB9A1BF33BED62F4330300F2F433032373730BEADD1E92F433030006F0000097B011602010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC3D4CAA7D6AEB3C7A1BF33BED62F4330300F2F433032373730BEADD1E92F433030019628005A000000AA0100530000000000000000000001F4000200190000305C000000010000000000670F2B2C0000000000000019000117D800000001FFFFFFFF00670F2B2C000000000000000000000000000000000000000000000001390300560000097C000000070113D6D5C9EDC8CECEF12DD0DDCFD0C7F8B9DBB9E226BDF8C8EB2F433032A1BECDACB3C7BDBBD3D1A1BF2F433030D6D0B5C4C8CED2E2D0DDCFD0C7F80F2F433032383933BEADD1E92F43303000710000097D010402010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B741C3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC7E9C3D4B7A8C0BCCEF7A1BF33BED62F4330300F2F433032383530BEADD1E92F433030006F0000097E010B02010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC1FAC3C5D0C2B4BAA1BF33BED62F4330300F2F433032383530BEADD1E92F43303001C92D00730000028F01006C000000000000000000000000000300190000426F00000001FFFFFFFF00670F2B2C000000000000001900002E13000000010000016800670F2B2C000000000000001900012221000000320000000000670F2B2C000000000000000000000000000000000000000000000001530300720000097F000000060111D6D5C9EDC8CECEF12DC8FCB3B5B8C4D7B044CDA8B9FDC9CCB3C7C4DAC9CFB7BD2F433032A1BEC8FCB3B5C7BFBBAFA1BF2F433030BDF8C8EBC8FCB3B5B8C4D7B0BDE7C3E6A3ACB8C4D7B0D2BBB4CEC8CED2E2C8FCB3B50F2F433032383430BEADD1E92F433030006F0000098000A602010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC8B5C7C5CFC9BEB3A1BF33BED62F4330300F2F433032393030BEADD1E92F433030006F00000981007602010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEBAA3D1F3B9ABD4B0A1BF33BED62F4330300F2F433032393030BEADD1E92F433030021832005A000000AB0100530000000000000000000001F40002001900003E72000000010000016800670F2B2C0000000000000019000035DB000000010000000000670F2B2C000000000000000000000000000000000000000000000001BB04006D00000982008902010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73DC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BECAAED2BBB3C7A1BF33BED62F4330300F2F433032353730BEADD1E92F433030006D0000098300A702010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73DC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEC7EFC3FBC9BDA1BF33BED62F4330300F2F433032353730BEADD1E92F433030006F00000984012702010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BEB4BAB7E7CCC6D4CFA1BF32BED62F4330300F2F433032353730BEADD1E92F433030006F0000098500C202010013C3BFC8D5C8CECEF12DBEBACBD9C8FCD1B5C1B73FC3BFCCECD4DA2F433032A1BEB8F6C8CBBEBACBD9C4A3CABDA1BF2F433030CFC2CDEAB3C9C8FCB5C02F433032A1BECEF7B2BFBFF3C9BDA1BF32BED62F4330300F2F433032353730BEADD1E92F43303000793700730000029001006C00000000000000000000000000030019000043F800000001FFFFFFFF00670F2B2C00000000000000190000358B000000010000000000670F2B2C000000000000001900012219000000020000000000670F2B2C000000000000000000000000000000000000000000000000030000AD000037025800000000000000EE009E00000000000000000000000000050019000072B500000001000000480000000000000000000000001900006FF100000001000000480000000000000000000000001900006BCC00000001000000480000000000000000000000001900010C3E000000010000004800000000000000000000000019000071000000000100000048000000000000000000000000000000000000000000000000000000000006012C0037");
	len = p - buf;
	SendToClient(Client, 24506, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	Write32(p, 0); //Uin
	Write8(p, 0); //CurrentLev
	Write8(p, 0); //NextLevLimit
	Write8(p, 0); //LevNum
	for (size_t i = 0; i < 0; i++)
	{ //LevAndTrainingTaskCfg
		BYTE* pLevAndTrainingTaskCfg = p;
		Write16(pLevAndTrainingTaskCfg, 0); //len

		Write8(pLevAndTrainingTaskCfg, 0); //Lev
		{ //LevGiftCfgAndSatus
			BYTE* pLevGiftCfgAndSatus = pLevAndTrainingTaskCfg;
			Write16(pLevGiftCfgAndSatus, 0); //len

			Write32(pLevGiftCfgAndSatus, 0); //TaskID
			Write8(pLevGiftCfgAndSatus, 0); //Status
			{ //AwardToClient
				BYTE* pAwardToClient = pLevGiftCfgAndSatus;
				Write16(pAwardToClient, 0); //len

				Write32(pAwardToClient, 0); //Exp
				Write32(pAwardToClient, 0); //Money
				Write32(pAwardToClient, 0); //SuperMoney
				Write16(pAwardToClient, 0); //ItemNum
				for (size_t i = 0; i < 0; i++)
				{ //ItemInfo
					BYTE* pItemInfo = pAwardToClient;
					Write16(pItemInfo, 0); //len

					Write32(pItemInfo, 0); //ItemID
					Write32(pItemInfo, 0); //ItemNum
					Write32(pItemInfo, 0); //AvailPeriod
					Write8(pItemInfo, 0); //Status
					WriteUn(pItemInfo, 0); //ObtainTime
					Write32(pItemInfo, 0); //OtherAttribute
					Write16(pItemInfo, 0); //ItemType

					len = pItemInfo - pAwardToClient;
					Set16(pAwardToClient, (WORD)len);
					pAwardToClient += len;
				}
				Write32(pAwardToClient, 0); //Coupons
				Write32(pAwardToClient, 0); //GuildPoint
				Write32(pAwardToClient, 0); //LuckMoney
				Write8(pAwardToClient, 0); //ExtendInfoNum
				for (size_t i = 0; i < 0; i++)
				{ //ExtendInfoAward
					BYTE* pExtendInfoAward = pAwardToClient;
					Write16(pExtendInfoAward, 0); //len

					Write16(pExtendInfoAward, 0); //Key
					Write32(pExtendInfoAward, 0); //AddValue
					Write32(pExtendInfoAward, 0); //TotalValue
					Write8(pExtendInfoAward, 0); //ShowPriority
					Write16(pExtendInfoAward, 0); //BuyNeedScore
					Write8(pExtendInfoAward, 0); //OneMatchMaxNum

					len = pExtendInfoAward - pAwardToClient;
					Set16(pAwardToClient, (WORD)len);
					pAwardToClient += len;
				}
				Write32(pAwardToClient, 0); //SpeedCoin

				len = pAwardToClient - pLevGiftCfgAndSatus;
				Set16(pLevGiftCfgAndSatus, (WORD)len);
				pLevGiftCfgAndSatus += len;
			}

			len = pLevGiftCfgAndSatus - pLevAndTrainingTaskCfg;
			Set16(pLevAndTrainingTaskCfg, (WORD)len);
			pLevAndTrainingTaskCfg += len;
		}
		{ //TaskListCfg
			BYTE* pTaskListCfg = pLevAndTrainingTaskCfg;
			Write16(pTaskListCfg, 0); //len

			Write8(pTaskListCfg, 0); //TaskNum
			for (size_t i = 0; i < 0; i++)
			{ //TaskListStatus
				BYTE* pTaskListStatus = pTaskListCfg;
				Write16(pTaskListStatus, 0); //len

				Write32(pTaskListStatus, 0); //TaskID
				Write16(pTaskListStatus, 0); //MapID
				Write8(pTaskListStatus, 0); //Status
				Write8(pTaskListStatus, 0); //Type
				Write8(pTaskListStatus, 0); //IsFirstAwardTask
				Write8(pTaskListStatus, 0); //NameLen
				Write8(pTaskListStatus, 0); //DescribeLen
				Write8(pTaskListStatus, 0); //AwardDesLen

				len = pTaskListStatus - pTaskListCfg;
				Set16(pTaskListCfg, (WORD)len);
				pTaskListCfg += len;
			}

			len = pTaskListCfg - pLevAndTrainingTaskCfg;
			Set16(pLevAndTrainingTaskCfg, (WORD)len);
			pLevAndTrainingTaskCfg += len;
		}

		len = pLevAndTrainingTaskCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //DailyExpGitCfg
		BYTE* pDailyExpGitCfg = p;
		Write16(pDailyExpGitCfg, 0); //len

		Write8(pDailyExpGitCfg, 0); //Status
		Write16(pDailyExpGitCfg, 0); //LevLimit
		Write16(pDailyExpGitCfg, 0); //ExpLimit
		Write32(pDailyExpGitCfg, 0); //ExpHasGot
		Write32(pDailyExpGitCfg, 0); //BoxID
		{ //AwardToClient
			BYTE* pAwardToClient = pDailyExpGitCfg;
			Write16(pAwardToClient, 0); //len

			Write32(pAwardToClient, 0); //Exp
			Write32(pAwardToClient, 0); //Money
			Write32(pAwardToClient, 0); //SuperMoney
			Write16(pAwardToClient, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardToClient;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAwardToClient;
				Set16(pAwardToClient, (WORD)len);
				pAwardToClient += len;
			}
			Write32(pAwardToClient, 0); //Coupons
			Write32(pAwardToClient, 0); //GuildPoint
			Write32(pAwardToClient, 0); //LuckMoney
			Write8(pAwardToClient, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtendInfoAward
				BYTE* pExtendInfoAward = pAwardToClient;
				Write16(pExtendInfoAward, 0); //len

				Write16(pExtendInfoAward, 0); //Key
				Write32(pExtendInfoAward, 0); //AddValue
				Write32(pExtendInfoAward, 0); //TotalValue
				Write8(pExtendInfoAward, 0); //ShowPriority
				Write16(pExtendInfoAward, 0); //BuyNeedScore
				Write8(pExtendInfoAward, 0); //OneMatchMaxNum

				len = pExtendInfoAward - pAwardToClient;
				Set16(pAwardToClient, (WORD)len);
				pAwardToClient += len;
			}
			Write32(pAwardToClient, 0); //SpeedCoin

			len = pAwardToClient - pDailyExpGitCfg;
			Set16(pDailyExpGitCfg, (WORD)len);
			pDailyExpGitCfg += len;
		}

		len = pDailyExpGitCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //NoviceGuildBaseCfg
		BYTE* pNoviceGuildBaseCfg = p;
		Write16(pNoviceGuildBaseCfg, 0); //len

		Write16(pNoviceGuildBaseCfg, 0); //WaitSecond
		Write16(pNoviceGuildBaseCfg, 0); //LevCritical

		len = pNoviceGuildBaseCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24506, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseGetWizardHomeBaseInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;
	WriteHex(p, "00000000000000C7000E00000057020101000003250700710A000B3A8A634B64EC668D01000B541CAB9F64EC668E01000B5B8E50F264EC668E01000B4D4877AC64EC668E01000BA41F40A264EC668E01000B26E1FE6B64EC668E01000BCABB624764EC668E01000BCABB7B0A64EC668E01000BCB3F008564EC668E01000B11D0A6F264EC668E010067112E13000067112E130067112E13000067112E13000067112E13000067112E13000067112E130055BF374000000000000000000000000067112E13040000000F0004000000020056003E000103E9BEABC1E931BAC50000000000000000000000000271060100000014001000010004000701000467010164EC68E000000000010000000000010000000064EC668C000567112E130067112E130000000056003E000203EBBEABC1E932BAC5000000000000000000000000000001010000000500000000000400070100046602000000000000000000010000000000000000000000000000000067112E130067112E130000000000000000000000000000000000000167112E13");

	//Write16(p, 0); //ResultID
	//Write32(p, 0); //Uin
	//{ //PlayerDBWizardHomeInfo
	//	BYTE* pPlayerDBWizardHomeInfo = p;
	//	Write16(pPlayerDBWizardHomeInfo, 0); //len

	//	{ //WizardHomeInfo
	//		BYTE* pWizardHomeInfo = pPlayerDBWizardHomeInfo;
	//		Write16(pWizardHomeInfo, 0); //len

	//		Write32(pWizardHomeInfo, 0); //ProsperityDegree
	//		Write8(pWizardHomeInfo, 0); //FarmLevel
	//		Write8(pWizardHomeInfo, 0); //CollegeLevel
	//		Write8(pWizardHomeInfo, 0); //WorkShopLevel
	//		Write32(pWizardHomeInfo, 0); //PrayerTreeExp
	//		Write8(pWizardHomeInfo, 0); //PrayerTreeLevel

	//		len = pWizardHomeInfo - pPlayerDBWizardHomeInfo;
	//		Set16(pPlayerDBWizardHomeInfo, (WORD)len);
	//		pPlayerDBWizardHomeInfo += len;
	//	}
	//	{ //FriendWateringInfoList
	//		BYTE* pFriendWateringInfoList = pPlayerDBWizardHomeInfo;
	//		Write16(pFriendWateringInfoList, 0); //len

	//		Write8(pFriendWateringInfoList, 0); //FriendNum
	//		for (size_t i = 0; i < n; i++)
	//		{ //FriendWateringInfo
	//			BYTE* pFriendWateringInfo = pFriendWateringInfoList;
	//			Write16(pFriendWateringInfo, 0); //len

	//			Write32(pFriendWateringInfo, 0); //Uin
	//			Write32(pFriendWateringInfo, 0); //LastFriendWateringTime
	//			Write8(pFriendWateringInfo, 0); //FriendWateringCountOneDay

	//			len = pFriendWateringInfo - pFriendWateringInfoList;
	//			Set16(pFriendWateringInfoList, (WORD)len);
	//			pFriendWateringInfoList += len;
	//		}

	//		len = pFriendWateringInfoList - pPlayerDBWizardHomeInfo;
	//		Set16(pPlayerDBWizardHomeInfo, (WORD)len);
	//		pPlayerDBWizardHomeInfo += len;
	//	}
	//	Write8(pPlayerDBWizardHomeInfo, 0); //JobCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastJobTime
	//	Write16(pPlayerDBWizardHomeInfo, 0); //PrayerTreeWateringCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastPrayerTreeWateringTime
	//	Write8(pPlayerDBWizardHomeInfo, 0); //PrayerTreeFriendWateringCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastPrayerTreeFriendWateringTime
	//	Write16(pPlayerDBWizardHomeInfo, 0); //CliffordCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastCliffordTime
	//	Write16(pPlayerDBWizardHomeInfo, 0); //FeedCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastFeedTime
	//	Write16(pPlayerDBWizardHomeInfo, 0); //AdventureCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastAdventureTime
	//	Write16(pPlayerDBWizardHomeInfo, 0); //ChallengeCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastChallengeTime
	//	Write8(pPlayerDBWizardHomeInfo, 0); //FarmStatus
	//	Write32(pPlayerDBWizardHomeInfo, 0); //FarmUpgradeEndTime
	//	Write8(pPlayerDBWizardHomeInfo, 0); //CollegeStatus
	//	Write32(pPlayerDBWizardHomeInfo, 0); //CollegeUpgradeEndTime
	//	Write8(pPlayerDBWizardHomeInfo, 0); //WorkShopStatus
	//	Write32(pPlayerDBWizardHomeInfo, 0); //WorkShopUpgradeEndTime
	//	Write16(pPlayerDBWizardHomeInfo, 0); //PrayerTreeBeWateredByFriendCount
	//	Write32(pPlayerDBWizardHomeInfo, 0); //LastPrayerTreeBeWateredByFriendTime
	//	Write8(pPlayerDBWizardHomeInfo, 0); //LuckyFruitLevel
	//	Write32(pPlayerDBWizardHomeInfo, 0); //JobProsperityScore
	//	{ //DressInfo
	//		BYTE* pDressInfo = pPlayerDBWizardHomeInfo;
	//		Write16(pDressInfo, 0); //len

	//		Write8(pDressInfo, 0); //ColorSetLen
	//		Write8(pDressInfo, 0); //DoorPlateLen

	//		len = pDressInfo - pPlayerDBWizardHomeInfo;
	//		Set16(pPlayerDBWizardHomeInfo, (WORD)len);
	//		pPlayerDBWizardHomeInfo += len;
	//	}

	//	len = pPlayerDBWizardHomeInfo - p;
	//	Set16(p, (WORD)len);
	//	p += len;
	//}
	//Write16(p, 0); //WizardNum
	//for (size_t i = 0; i < n; i++)
	//{ //PlayerDBWizardBaseInfo
	//	BYTE* pPlayerDBWizardBaseInfo = p;
	//	Write16(pPlayerDBWizardBaseInfo, 0); //len

	//	{ //WizardBaseInfo
	//		BYTE* pWizardBaseInfo = pPlayerDBWizardBaseInfo;
	//		Write16(pWizardBaseInfo, 0); //len

	//		Write16(pWizardBaseInfo, 0); //WizardID
	//		Write16(pWizardBaseInfo, 0); //WizardType
	//		WriteString(pWizardBaseInfo, 0); //NickName[]
	//		Write32(pWizardBaseInfo, 0); //Experience
	//		Write8(pWizardBaseInfo, 0); //Level
	//		Write8(pWizardBaseInfo, 0); //Order
	//		Write32(pWizardBaseInfo, 0); //Health
	//		Write8(pWizardBaseInfo, 0); //IsRare
	//		Write8(pWizardBaseInfo, 0); //Intimacy
	//		Write8(pWizardBaseInfo, 0); //IsHuanHua
	//		Write8(pWizardBaseInfo, 0); //IsShow
	//		Write8(pWizardBaseInfo, 0); //IsCarry
	//		Write8(pWizardBaseInfo, 0); //MaxSkillNum
	//		{ //WizardSkillInfoList
	//			BYTE* pWizardSkillInfoList = pWizardBaseInfo;
	//			Write16(pWizardSkillInfoList, 0); //len

	//			Write8(pWizardSkillInfoList, 0); //SkillNum
	//			for (size_t i = 0; i < n; i++)
	//			{ //SkillInfo
	//				BYTE* pSkillInfo = pWizardSkillInfoList;
	//				Write16(pSkillInfo, 0); //len

	//				Write8(pSkillInfo, 0); //SkillId
	//				Write8(pSkillInfo, 0); //SkillLevel

	//				len = pSkillInfo - pWizardSkillInfoList;
	//				Set16(pWizardSkillInfoList, (WORD)len);
	//				pWizardSkillInfoList += len;
	//			}

	//			len = pWizardSkillInfoList - pWizardBaseInfo;
	//			Set16(pWizardBaseInfo, (WORD)len);
	//			pWizardBaseInfo += len;
	//		}
	//		Write8(pWizardBaseInfo, 0); //Status
	//		Write32(pWizardBaseInfo, 0); //EndTime
	//		Write32(pWizardBaseInfo, 0); //LeftTime
	//		Write8(pWizardBaseInfo, 0); //EvolutionBranch
	//		Write16(pWizardBaseInfo, 0); //CurAdventureMapID
	//		Write8(pWizardBaseInfo, 0); //LastunLockMapWizardLev
	//		Write16(pWizardBaseInfo, 0); //Flag
	//		Write8(pWizardBaseInfo, 0); //JobType

	//		len = pWizardBaseInfo - pPlayerDBWizardBaseInfo;
	//		Set16(pPlayerDBWizardBaseInfo, (WORD)len);
	//		pPlayerDBWizardBaseInfo += len;
	//	}
	//	Write32(pPlayerDBWizardBaseInfo, 0); //LastActionTime
	//	Write32(pPlayerDBWizardBaseInfo, 0); //LastCareTime
	//	Write16(pPlayerDBWizardBaseInfo, 0); //CareCount
	//	Write32(pPlayerDBWizardBaseInfo, 0); //LastFeedTime
	//	Write8(pPlayerDBWizardBaseInfo, 0); //FeedCount
	//	Write32(pPlayerDBWizardBaseInfo, 0); //LastFeedFruitTime
	//	Write16(pPlayerDBWizardBaseInfo, 0); //FeedFruitCount
	//	Write8(pPlayerDBWizardBaseInfo, 0); //ComputeType

	//	len = pPlayerDBWizardBaseInfo - p;
	//	Set16(p, (WORD)len);
	//	p += len;
	//}
	//Write8(p, 0); //FarmStatus
	//Write32(p, 0); //FarmUpgradeLeftTime
	//Write8(p, 0); //CollegeStatus
	//Write32(p, 0); //CollegeUpgradeLeftTime
	//Write8(p, 0); //WorkShopStatus
	//Write32(p, 0); //WorkShopUpgradeLeftTime
	//Write8(p, 0); //HasLuckyFruitToReceive
	//Write32(p, 0); //CurServerTime

	len = p - buf;
	SendToClient(Client, 11474, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void RequestChangeTeam(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	bool NewTeamID = Read8(p);

	ResponseChangeTeam(Client, NewTeamID);
}

void NotifyPlayerChangeTeam(std::shared_ptr<ClientNode> Client, UINT Uin, bool NewTeamID)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Uin); //Uin
	Write8(p, NewTeamID); //NewTeamID
	Write8(p, 0); //LoverRaceOrPro

	len = p - buf;
	SendToClient(Client, 523, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseChangeTeam(std::shared_ptr<ClientNode> Client, bool NewTeamID) {

	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write16(p, 0); //ResultID
	Write8(p, NewTeamID); //NewTeamID
	Write8(p, 0); //ReasonLen
	Write8(p, 0); //LoverRaceOrPro

	len = p - buf;
	SendToClient(Client, 128, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);


	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (Room)
	{
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				if (RoomClient != Client)
				{
					NotifyPlayerChangeTeam(RoomClient, Client->Uin, NewTeamID);
				}
			}
		}
	}
}

void NotifyFirstRechargeCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	Write32(p, Client->Uin); //Uin

	WriteHex(p, "0000000001008C00000000000009C4000027100004001F0001000051280000000000010000016800000000000000000000000000001F000200005D7500005D470001000002D000000000000000000000000000001F0001000071CB0000000000030000000000000000000000000000000000001F000100007EBD0000000009C40000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 13722, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	Write8(p, 0); //CanGainAward
	{ //FirstExchangeAwardInfo
		BYTE* pFirstExchangeAwardInfo = p;
		Write16(pFirstExchangeAwardInfo, 0); //len

		Write32(pFirstExchangeAwardInfo, 0); //Money
		Write32(pFirstExchangeAwardInfo, 0); //SuperMoney
		Write32(pFirstExchangeAwardInfo, 0); //Experience
		Write16(pFirstExchangeAwardInfo, 0); //ItemNum
		for (size_t i = 0; i < 0; i++)
		{ //ItemInfo
			BYTE* pItemInfo = pFirstExchangeAwardInfo;
			Write16(pItemInfo, 0); //len

			Write16(pItemInfo, 0); //GenderItemNum
			Write32(pItemInfo, 0); //ItemID[]
			Write16(pItemInfo, 0); //ItemNum
			Write32(pItemInfo, 0); //AvailPeriod
			Write16(pItemInfo, 0); //BroadcastType
			Write16(pItemInfo, 0); //BroadcastFlag
			Write8(pItemInfo, 0); //ShowPriority
			Write8(pItemInfo, 0); //ItemType
			Write8(pItemInfo, 0); //IgnoreItem
			Write32(pItemInfo, 0); //LimitPara
			Write8(pItemInfo, 0); //SpecialAwardCtrlType
			Write8(pItemInfo, 0); //AwardCtrlType

			len = pItemInfo - pFirstExchangeAwardInfo;
			Set16(pFirstExchangeAwardInfo, (WORD)len);
			pFirstExchangeAwardInfo += len;
		}

		len = pFirstExchangeAwardInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 13722, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyGroupBuyingrCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "01BD00000003002500001388001F00010002093B0000000000010000000000000000000000000000000000002500002710001F00010002093B0000000000030000000000000000000000000000000000002500004E20001F00010002093B0000000000060000000000000000000000000000000000000000000000000000000000000000000000000000000002000300040001000000000000000000000000001A0134DA10000186A00134DA6F000399B766E24B206704057F001A0134DA10000186A00134DA6B000399B766E24B2066FEBF7F001A0134B174000186A00134B174000399B76462E3A06463A87F001A0133C848000000000133C849000399B7599DA68059A0497F001A0134DA10000186A00134DA6F000399B766E24B206704057F001A000000000000000000000000000000000000000000000000001A000000000000000000000000000000000000000000000000001A000000000000000000000000000000000000000000000000001A000000000000000000000000000000000000000000000000001A000000000000000000000000000000000000000000000000001A0000000000000000000000000000000000000000000000000134D8860134D88E0134D88E00000000");
	len = p - buf;
	SendToClient(Client, 680, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	{ //GroupBuyingCfg
		BYTE* pGroupBuyingCfg = p;
		Write16(pGroupBuyingCfg, 0); //len

		Write32(pGroupBuyingCfg, 0); //AwardCfgNum
		for (size_t i = 0; i < 0; i++)
		{ //GroupBuyingAwardCfg
			BYTE* pGroupBuyingAwardCfg = pGroupBuyingCfg;
			Write16(pGroupBuyingAwardCfg, 0); //len

			Write32(pGroupBuyingAwardCfg, 0); //TotalPlayerCount
			{ //AwardItemInfo
				BYTE* pAwardItemInfo = pGroupBuyingAwardCfg;
				Write16(pAwardItemInfo, 0); //len

				Write16(pAwardItemInfo, 0); //GenderItemNum
				Write32(pAwardItemInfo, 0); //ItemID[]
				Write16(pAwardItemInfo, 0); //ItemNum
				Write32(pAwardItemInfo, 0); //AvailPeriod
				Write16(pAwardItemInfo, 0); //BroadcastType
				Write16(pAwardItemInfo, 0); //BroadcastFlag
				Write8(pAwardItemInfo, 0); //ShowPriority
				Write8(pAwardItemInfo, 0); //ItemType
				Write8(pAwardItemInfo, 0); //IgnoreItem
				Write32(pAwardItemInfo, 0); //LimitPara
				Write8(pAwardItemInfo, 0); //SpecialAwardCtrlType
				Write8(pAwardItemInfo, 0); //AwardCtrlType

				len = pAwardItemInfo - pGroupBuyingAwardCfg;
				Set16(pGroupBuyingAwardCfg, (WORD)len);
				pGroupBuyingAwardCfg += len;
			}

			len = pGroupBuyingAwardCfg - pGroupBuyingCfg;
			Set16(pGroupBuyingCfg, (WORD)len);
			pGroupBuyingCfg += len;
		}
		Write16(pGroupBuyingCfg, 0); //TotalAnnualFestivalTurnOnFlag
		Write16(pGroupBuyingCfg, 0); //AnnualFestivalTurnOnFlag[]
		Write16(pGroupBuyingCfg, 0); //AnnualFestivalPriority[]
		{ //OpenDateTime
			BYTE* pOpenDateTime = pGroupBuyingCfg;
			Write16(pOpenDateTime, 0); //len

			Write32(pOpenDateTime, 0); //OpenDate
			Write32(pOpenDateTime, 0); //OpenTime
			Write32(pOpenDateTime, 0); //CloseDate
			Write32(pOpenDateTime, 0); //CloseTime
			Write32(pOpenDateTime, 0); //Open
			Write32(pOpenDateTime, 0); //Close
			Write32(pOpenDateTime, 0); //OpenDate
			Write32(pOpenDateTime, 0); //OpenTime
			Write32(pOpenDateTime, 0); //CloseDate
			Write32(pOpenDateTime, 0); //CloseTime
			Write32(pOpenDateTime, 0); //Open
			Write32(pOpenDateTime, 0); //Close

			len = pOpenDateTime - pGroupBuyingCfg;
			Set16(pGroupBuyingCfg, (WORD)len);
			pGroupBuyingCfg += len;
		}

		len = pGroupBuyingCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //OpenDate
	Write32(p, 0); //CloseDate
	Write32(p, 0); //FreezeDate
	Write32(p, 0); //OpneTimeCfgNum
	for (size_t i = 0; i < 0; i++)
	{ //GangsterOpenTimeCfg
		BYTE* pGangsterOpenTimeCfg = p;
		Write16(pGangsterOpenTimeCfg, 0); //len

		Write32(pGangsterOpenTimeCfg, 0); //OpenTime
		Write32(pGangsterOpenTimeCfg, 0); //CloseTime

		len = pGangsterOpenTimeCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 680, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyFBPlayerLevInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "000000000000000100000001000000010000000100000001");
	len = p - buf;
	SendToClient(Client, 20069, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	Write32(p, 0); //Uin
	Write32(p, 0); //Lev[]

	len = p - buf;
	SendToClient(Client, 20069, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseJiaNianHuaCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "0000084203004C000249F000210000000000000000000000C80000000000000000000000000000000000000000210000003200000000000000000000000000000000000000000000000000000000000001004C00021B10002100000000000000000000012C0000000000000000000000000000000000000000210000006400000000000000000000000000000000000000000000000000000000000002004C000203A00021000000000000000000000226000000000000000000000000000000000000000021000000C8000000000000000000000000000000000000000000000000000000000000030500410000030D40003A0000000000000000000000000001001900003FD100000001000000000000000000000000000000000000000000000000000000000000000000410000061A80003A00000000000000000000000000010019000035DB000000010000000000000000000000000000000000000000000000000000000000000000007301000927C0006C00000000000000000000000000030019000028130000000100000030000000000000000000000000190000FE05000000010000000000000000000000000000000019000035DC000000010000000000000000000000000000000000000000000000000000000000000000007301000C3500006C0000000000000000000000000003001900004D2C000000020000000000000000000000000000000019000055D90000000100000000000000000000000000000000190000358B00000001000000000000000000000000000000000000000000000000000000000000000000A501000F4240009E0000000000000000000000000005001900006FF300000001FFFFFFFF0000000000000000000000001900007E9D00000001000000A800000000000000000000000019000107F5000000010000016800000000000000000000000019000103F500000001000000F000000000000000000000000019000055DF000000010000000000000000000000000000000000000000000000000000000000000000000000000000002B6C");
	len = p - buf;
	SendToClient(Client, 24076, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	return;



	Write32(p, 0); //TaskID
	Write8(p, 0); //PersonAwardNum
	for (size_t i = 0; i < 0; i++)
	{ //PersonAwardCfg
		BYTE* pPersonAwardCfg = p;
		Write16(pPersonAwardCfg, 0); //len

		Write32(pPersonAwardCfg, 0); //Time
		{ //AwardFirst
			BYTE* pAwardFirst = pPersonAwardCfg;
			Write16(pAwardFirst, 0); //len

			Write32(pAwardFirst, 0); //Exp
			Write32(pAwardFirst, 0); //Money
			Write32(pAwardFirst, 0); //SuperMoney
			Write16(pAwardFirst, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardFirst;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAwardFirst;
				Set16(pAwardFirst, (WORD)len);
				pAwardFirst += len;
			}
			Write32(pAwardFirst, 0); //Coupons
			Write32(pAwardFirst, 0); //GuildPoint
			Write32(pAwardFirst, 0); //LuckMoney
			Write8(pAwardFirst, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtendInfoAward
				BYTE* pExtendInfoAward = pAwardFirst;
				Write16(pExtendInfoAward, 0); //len

				Write16(pExtendInfoAward, 0); //Key
				Write32(pExtendInfoAward, 0); //AddValue
				Write32(pExtendInfoAward, 0); //TotalValue
				Write8(pExtendInfoAward, 0); //ShowPriority
				Write16(pExtendInfoAward, 0); //BuyNeedScore
				Write8(pExtendInfoAward, 0); //OneMatchMaxNum

				len = pExtendInfoAward - pAwardFirst;
				Set16(pAwardFirst, (WORD)len);
				pAwardFirst += len;
			}
			Write32(pAwardFirst, 0); //SpeedCoin

			len = pAwardFirst - pPersonAwardCfg;
			Set16(pPersonAwardCfg, (WORD)len);
			pPersonAwardCfg += len;
		}
		{ //AwardOther
			BYTE* pAwardOther = pPersonAwardCfg;
			Write16(pAwardOther, 0); //len

			Write32(pAwardOther, 0); //Exp
			Write32(pAwardOther, 0); //Money
			Write32(pAwardOther, 0); //SuperMoney
			Write16(pAwardOther, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardOther;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pAwardOther;
				Set16(pAwardOther, (WORD)len);
				pAwardOther += len;
			}
			Write32(pAwardOther, 0); //Coupons
			Write32(pAwardOther, 0); //GuildPoint
			Write32(pAwardOther, 0); //LuckMoney
			Write8(pAwardOther, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtendInfoAward
				BYTE* pExtendInfoAward = pAwardOther;
				Write16(pExtendInfoAward, 0); //len

				Write16(pExtendInfoAward, 0); //Key
				Write32(pExtendInfoAward, 0); //AddValue
				Write32(pExtendInfoAward, 0); //TotalValue
				Write8(pExtendInfoAward, 0); //ShowPriority
				Write16(pExtendInfoAward, 0); //BuyNeedScore
				Write8(pExtendInfoAward, 0); //OneMatchMaxNum

				len = pExtendInfoAward - pAwardOther;
				Set16(pAwardOther, (WORD)len);
				pAwardOther += len;
			}
			Write32(pAwardOther, 0); //SpeedCoin

			len = pAwardOther - pPersonAwardCfg;
			Set16(pPersonAwardCfg, (WORD)len);
			pPersonAwardCfg += len;
		}
		Write32(pPersonAwardCfg, 0); //Score

		len = pPersonAwardCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //ScoreAwardNum
	for (size_t i = 0; i < 0; i++)
	{ //ScoreAwardCfg
		BYTE* pScoreAwardCfg = p;
		Write16(pScoreAwardCfg, 0); //len

		Write8(pScoreAwardCfg, 0); //Type
		Write32(pScoreAwardCfg, 0); //StartScore
		{ //AwardInfo
			BYTE* pAwardInfo = pScoreAwardCfg;
			Write16(pAwardInfo, 0); //len

			Write32(pAwardInfo, 0); //Exp
			Write32(pAwardInfo, 0); //Money
			Write32(pAwardInfo, 0); //SuperMoney
			Write16(pAwardInfo, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pAwardInfo;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
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

		len = pScoreAwardCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //OpenDayNum
	Write32(p, 0); //FrozenSeconds
	Write16(p, 0); //ResultID
	Write16(p, 0); //CanStartResultID

	len = p - buf;
	SendToClient(Client, 24076, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


void NotifyAllAreaNameCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "03000E0000000307B5E7D0C532C7F8000D0000000206C1AACDA8C7F8000D0000000106B5E7D0C5C7F8");
	len = p - buf;
	SendToClient(Client, 24639, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write8(p, 0); //Nums
	for (size_t i = 0; i < 0; i++)
	{ //AllAreaName
		BYTE* pAllAreaName = p;
		Write16(pAllAreaName, 0); //len

		Write32(pAllAreaName, 0); //AreaId
		Write8(pAllAreaName, 0); //AreaNameLen

		len = pAllAreaName - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24639, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void NotifyAllCityCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "03FC06400164640300");
	len = p - buf;
	SendToClient(Client, 24811, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;

	Write8(p, 0); //TotalProgress
	Write8(p, 0); //TotalSkillProgess
	Write8(p, 0); //ClientReportDetailInfoSeconds

	len = p - buf;
	SendToClient(Client, 24811, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyScoreBoxCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "000000011C180000000300A900000000AA000000FA009E000000000000000000000000000500190000641500000001000002D0000000000000000000000000190000F7D000000001000002D0000000000000000000000000190000F52E00000001000002D00000000000000000000000001900007CA200000001FFFFFFFF00000000000000000000000019000081CB00000001FFFFFFFF0000000000000000000000000000000000000000000000000000000000C201000000AB000001F400B70000000000000000000000000006001900007CCD00000001000002D0000000000000000000000000190000633200000001000002D000000000000000000000000019000072B600000001000002D0000000000000000000000000190000F7A100000001FFFFFFFF00000000000000000000000019000067B500000001000002D0000000000000000000000000190001053F00000001000002D00000000000000000000000000000000000000000000000000000000000A902000000AC000003E8009E0000000000000000000000000005001900011C1A00000001FFFFFFFF0000000000000000000000001900011C1A00000001000002D00000000000000000000000001900005E8F00000001FFFFFFFF0000000000000000000000001900005DB000000001FFFFFFFF00000000000000000000000019000076BF00000001000002D000000000000000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 24423, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write8(p, 0); //Type
	Write8(p, 0); //Filter
	Write32(p, 0); //ItemID
	Write32(p, 0); //BoxAwardNum
	for (size_t i = 0; i < 0; i++)
	{ //ActiveAwardToClient
		BYTE* pActiveAwardToClient = p;
		Write16(pActiveAwardToClient, 0); //len

		Write8(pActiveAwardToClient, 0); //AwardIdx
		Write32(pActiveAwardToClient, 0); //BoxID
		Write32(pActiveAwardToClient, 0); //Limit
		{ //Award
			BYTE* pAward = pActiveAwardToClient;
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

			len = pAward - pActiveAwardToClient;
			Set16(pActiveAwardToClient, (WORD)len);
			pActiveAwardToClient += len;
		}

		len = pActiveAwardToClient - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24423, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyRedEnvelopeInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;


	WriteHex(p, "0000000000016A001E0000000A000000640000000000000000000AC7C0BAECB0FCD0C2CAD60000");
	len = p - buf;
	SendToClient(Client, 24061, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write32(p, 0); //Uin
	Write8(p, 0); //Open
	Write16(p, 0); //RedEnvelopeID
	Write16(p, 0); //CanSendNum
	Write16(p, 0); //SentNum
	Write16(p, 0); //OnceSendNum
	Write32(p, 0); //SupermoneyOnce
	Write32(p, 0); //SupermoneySend
	Write32(p, 0); //SupermoneyRecv
	Write16(p, 0); //TitleLength
	Write16(p, 0); //RecvNum
	for (size_t i = 0; i < 0; i++)
	{ //RecvRedEnvelopeInfo
		BYTE* pRecvRedEnvelopeInfo = p;
		Write16(pRecvRedEnvelopeInfo, 0); //len

		Write32(pRecvRedEnvelopeInfo, 0); //Uin
		Write16(pRecvRedEnvelopeInfo, 0); //ID
		Write8(pRecvRedEnvelopeInfo, 0); //Type
		Write32(pRecvRedEnvelopeInfo, 0); //OtherUin
		WriteString(pRecvRedEnvelopeInfo, 0); //NickName[]
		Write8(pRecvRedEnvelopeInfo, 0); //Source
		Write8(pRecvRedEnvelopeInfo, 0); //Status
		Write32(pRecvRedEnvelopeInfo, 0); //Value
		Write32(pRecvRedEnvelopeInfo, 0); //RecvTime
		Write32(pRecvRedEnvelopeInfo, 0); //OpenTime

		len = pRecvRedEnvelopeInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24061, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyPlayerRaceCardAddInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;


	Write32(p, 0); //Uin
	Write16(p, 0); //RaceCardNums
	for (size_t i = 0; i < 0; i++)
	{ //RaceCardAddValueInfo
		BYTE* pRaceCardAddValueInfo = p;
		Write16(pRaceCardAddValueInfo, 0); //len

		Write32(pRaceCardAddValueInfo, 0); //RaceCardID
		Write32(pRaceCardAddValueInfo, 0); //AddValue

		len = pRaceCardAddValueInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 16077, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyClientGolumBallAwardInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "0000000000010000000C000500720000000A006C000000000000000000000000000300190000358C0000000100000000000000000000000000000000190000305C000000010000000000000000000000000000000019000027DF0000000A0000000000000000000000000000000000000000000000000000000000000000008B0001001E00850000000000000000000000000004001900010A3100000002000000000000000000000000000000001900003063000000010000000000000000000000000000000019000055D9000000010000000000000000000000000000000019000035DB00000001000000000000000000000000000000000000000000000000000000000000000000A400020032009E000000000000000000000000000500190000616600000001000002D00000000000000000000000001900008462000000010000000000000000000000000000000019000057170000000100000000000000000000000000000000190000475E0000000100000000000000000000000000000000190000498000000001000002D00000000000000000000000000000000000000000000000000000000000A400030046009E000000000000000000000000000500190000682700000001FFFFFFFF000000000000000000000000190000845F00000001000001680000000000000000000000001900010FF600000001000001680000000000000000000000001900010A310000000500000000000000000000000000000000190000305A00000005000000000000000000000000000000000000000000000000000000000000000000A400040064009E0000000000000000000000000005001900004FF700000001FFFFFFFF000000000000000000000000190000841300000001000002D0000000000000000000000000190000633100000001000002D0000000000000000000000000190000846300000001000000000000000000000000000000001900003062000000030000000000000000000000000000000000000000000000000000000000000000");
	len = p - buf;
	SendToClient(Client, 24456, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
	return;


	Write32(p, 0); //Uin
	Write16(p, 0); //ReceivedAwardFlag
	Write32(p, 0); //OpenGolumBallNums
	Write16(p, 0); //TreasureBoxNums
	for (size_t i = 0; i < 0; i++)
	{ //GolumBallBoxBestQualityAwardInfos
		BYTE* pGolumBallBoxBestQualityAwardInfos = p;
		Write16(pGolumBallBoxBestQualityAwardInfos, 0); //len

		Write16(pGolumBallBoxBestQualityAwardInfos, 0); //BoxType
		Write16(pGolumBallBoxBestQualityAwardInfos, 0); //ActivedNums
		{ //BestQualityAward
			BYTE* pBestQualityAward = pGolumBallBoxBestQualityAwardInfos;
			Write16(pBestQualityAward, 0); //len

			Write32(pBestQualityAward, 0); //Exp
			Write32(pBestQualityAward, 0); //Money
			Write32(pBestQualityAward, 0); //SuperMoney
			Write16(pBestQualityAward, 0); //ItemNum
			for (size_t i = 0; i < 0; i++)
			{ //ItemInfo
				BYTE* pItemInfo = pBestQualityAward;
				Write16(pItemInfo, 0); //len

				Write32(pItemInfo, 0); //ItemID
				Write32(pItemInfo, 0); //ItemNum
				Write32(pItemInfo, 0); //AvailPeriod
				Write8(pItemInfo, 0); //Status
				WriteUn(pItemInfo, 0); //ObtainTime
				Write32(pItemInfo, 0); //OtherAttribute
				Write16(pItemInfo, 0); //ItemType

				len = pItemInfo - pBestQualityAward;
				Set16(pBestQualityAward, (WORD)len);
				pBestQualityAward += len;
			}
			Write32(pBestQualityAward, 0); //Coupons
			Write32(pBestQualityAward, 0); //GuildPoint
			Write32(pBestQualityAward, 0); //LuckMoney
			Write8(pBestQualityAward, 0); //ExtendInfoNum
			for (size_t i = 0; i < 0; i++)
			{ //ExtendInfoAward
				BYTE* pExtendInfoAward = pBestQualityAward;
				Write16(pExtendInfoAward, 0); //len

				Write16(pExtendInfoAward, 0); //Key
				Write32(pExtendInfoAward, 0); //AddValue
				Write32(pExtendInfoAward, 0); //TotalValue
				Write8(pExtendInfoAward, 0); //ShowPriority
				Write16(pExtendInfoAward, 0); //BuyNeedScore
				Write8(pExtendInfoAward, 0); //OneMatchMaxNum

				len = pExtendInfoAward - pBestQualityAward;
				Set16(pBestQualityAward, (WORD)len);
				pBestQualityAward += len;
			}
			Write32(pBestQualityAward, 0); //SpeedCoin

			len = pBestQualityAward - pGolumBallBoxBestQualityAwardInfos;
			Set16(pGolumBallBoxBestQualityAwardInfos, (WORD)len);
			pGolumBallBoxBestQualityAwardInfos += len;
		}

		len = pGolumBallBoxBestQualityAwardInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 24456, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyLmtGetScoreInfo(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	{ //ScoreInfo
		BYTE* pScoreInfo = p;
		Write16(pScoreInfo, 0); //len

		Write32(pScoreInfo, 1000); //Score
		Write16(pScoreInfo, 0); //Rank
		Write16(pScoreInfo, 1); //TotalPlayerNum
		Write8(pScoreInfo, 2); //RoundNum
		for (size_t i = 0; i < 2; i++)
		{ //RoundResult
			BYTE* pRoundResult = pScoreInfo;
			Write16(pRoundResult, 0); //len

			Write8(pRoundResult, 3); //Result
			Write32(pRoundResult, 0); //ScoreChg

			len = pRoundResult - pScoreInfo;
			Set16(pScoreInfo, (WORD)len);
			pScoreInfo += len;
		}
		{ //PlayerRecord
			BYTE* pPlayerRecord = pScoreInfo;
			Write16(pPlayerRecord, 0); //len

			Write32(pPlayerRecord, 1); //MatchNum
			Write8(pPlayerRecord, 0); //BestRank
			Write32(pPlayerRecord, 0); //ChamNum
			Write32(pPlayerRecord, 0); //WinNum

			len = pPlayerRecord - pScoreInfo;
			Set16(pScoreInfo, (WORD)len);
			pScoreInfo += len;
		}

		len = pScoreInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 810, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


void NotifyGangsterCfg(std::shared_ptr<ClientNode> Client)
{
	BYTE buf[8192]; BYTE* p = buf; size_t len;

	WriteHex(p, "05EA00FE000000010000006400000064000000010000001400000002000000140000002D0000001400000032000000640000000100000000000000000000000100000000000000030000000300000006000000020000000800000064000000640000000000000006000000020000009600000064000000020000009600002EE000002710000000030000000100000001000000020000012C0000000500010D8E00000001000000640000000300000003000000FA00001F40000000C80000EA600000019A000003E8000004B000000BB800003A9800007530000005DC000003E8000000C800002EE00000138801337A8C01337BAF00000000000000050000000A0000000B002600000132000000320000025800000006000000020000000100002328000000020000000100260000038E000000320000025800000006000000020000000100002328000000020000000000260000038600000032000002580000000500000002000000010000232800000002000000000026000003850000003200000258000000050000000200000001000023280000000200000000002600000387000000320000025800000006000000020000000100002328000000020000000000260000038A000000320000025800000007000000020000000100000FA0000000010000000000260000038B0000003200000258000000060000000200000001000023280000000200000000002600000388000000320000025800000005000000020000000100000FA00000000000000000002600000389000000320000025800000005000000020000000100002328000000020000000000260000038C000000320000025800000006000000020000000100002328000000020000000000260000038D0000003200000258000000060000000200000001000023280000000200000000000000060026000000010000000000000001FFFFFFFFFFFFFFFFFFFFFFFF0000000F0000000A000000000026000000020000000000000000FFFFFFFFFFFFFFFFFFFFFFFF0000000F0000000A00000000002600000003000000010000000100004E200000138800001388000000050000000A00000000002600000004000000020000000000009C400000138800000FA0000000050000000A0000000000260000000500000003000000010000271000001388000013880000000000000000000000320026000000060000000300000000000027100000138800001388000000000000000000000032000000050026000000380134AFE40134B09D00000002000A0001D4C00002D667000A0002E63000034B970026000000390134B09E0134B10F00000002000A0001D4C00002D667000A0002E63000034B9700260000003A0134B1100134B1C900000002000A0001D4C00002D667000A0002E63000034B9700260000003B0134B1CA0134B23B00000002000A0001D4C00002D667000A0002E63000034B9700260000003C0134B23C0134B2F500000002000A0001D4C00002D667000A0002E63000034B97000000030062000000060000000A000000050000000300000000002600000001000E000000640000000A0000000200000001000E000000000000000500000001002600000001000E00000032000000050000000100000001000E000000000000000A000000020062000000040000000A000000050000000300000000002600000001000E000000640000000A0000000200000001000E000000000000000500000001002600000001000E00000032000000050000000100000001000E000000000000000A000000020062000000020000000A000000050000000300000000002600000001000E000000640000000A0000000200000001000E000000000000000500000001002600000001000E00000032000000050000000100000001000E000000000000000A0000000200000006001200000001000003E70000003200010D7D001200000002000000020000006400010D7E001200000003000000020000006400010D7F001200000005000000020000006400010D80001200000007000000020000006400010D8100120000000A000000020000006400010D82");
	len = p - buf;
	SendToClient(Client, 653, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);

	int n = 0;
	{ //GangsterCfg
		BYTE* pGangsterCfg = p;
		Write16(pGangsterCfg, 0); //len

		{ //BaseCfg
			BYTE* pBaseCfg = pGangsterCfg;
			Write16(pBaseCfg, 0); //len

			Write32(pBaseCfg, 0); //EnableCreateRoomFlag
			Write32(pBaseCfg, 0); //InitGansterHp
			Write32(pBaseCfg, 0); //InitPoliceHp
			Write32(pBaseCfg, 0); //GansterRebornWaitTime
			Write32(pBaseCfg, 0); //GansterImmuneTime
			Write32(pBaseCfg, 0); //PoliceRebornWaitTime
			Write32(pBaseCfg, 0); //PoliceImmuneTime
			Write32(pBaseCfg, 0); //AddEnergyByStrikeDoor
			Write32(pBaseCfg, 0); //AddEnergyByStrikeCar
			Write32(pBaseCfg, 0); //DropHpToEnergyRate
			Write32(pBaseCfg, 0); //MaxEnergy
			Write32(pBaseCfg, 0); //GansterAddScoreByKill
			Write32(pBaseCfg, 0); //PoliceDeductScoreForDead
			Write32(pBaseCfg, 0); //GansterAddScoreByCrossRound
			Write32(pBaseCfg, 0); //PoliceAddScoreByKill
			Write32(pBaseCfg, 0); //PoliceAddScoreByCrossRound
			Write32(pBaseCfg, 0); //GansterResetPointNum
			Write32(pBaseCfg, 0); //PoliceResetPointNum
			Write32(pBaseCfg, 0); //PkDistance
			Write32(pBaseCfg, 0); //PkPrepareTime
			Write32(pBaseCfg, 0); //PkHoldonTime
			Write32(pBaseCfg, 0); //GansterPkOverLoserDropHp
			Write32(pBaseCfg, 0); //PolicePkOverLoserDropHp
			Write32(pBaseCfg, 0); //PkDropHpPerSecondGanster
			Write32(pBaseCfg, 0); //GansterRiskCompensationDistance
			Write32(pBaseCfg, 0); //GansterRiskCompensationDropHpPerSecond
			Write32(pBaseCfg, 0); //GansterRiskCompensationIncN20Rate
			Write32(pBaseCfg, 0); //PoliceRiskCompensationDistance
			Write32(pBaseCfg, 0); //PoliceRiskCompensationDropHpPerSecond
			Write32(pBaseCfg, 0); //PoliceRiskCompensationIncN20Rate
			Write32(pBaseCfg, 0); //GansterRaceBeginCountDownTime
			Write32(pBaseCfg, 0); //GansterMatchShowTime
			Write32(pBaseCfg, 0); //WinTeamScore
			Write32(pBaseCfg, 0); //DeductPoliceScoreByLeave
			Write32(pBaseCfg, 0); //DeductGansterScoreByLeave
			Write32(pBaseCfg, 0); //MultipleResult
			Write32(pBaseCfg, 0); //MaxFragmentOneDay
			Write32(pBaseCfg, 0); //MaxMultipleGameOneDay
			Write32(pBaseCfg, 0); //FragmentItemId
			Write32(pBaseCfg, 0); //LoseStreakCountThreshold
			Write32(pBaseCfg, 0); //DriftActiveProDensityLevel
			Write32(pBaseCfg, 0); //Helicopter_Missile_Count
			Write32(pBaseCfg, 0); //Helicopter_SwordEff_Count
			Write32(pBaseCfg, 0); //PKingSpeed_Space
			Write32(pBaseCfg, 0); //PKingSpeed_NO2Add
			Write32(pBaseCfg, 0); //PKingSpeed_RecoverTime
			Write32(pBaseCfg, 0); //PKingSpeed_NO2
			Write32(pBaseCfg, 0); //PKingSpeed_Normal
			Write32(pBaseCfg, 0); //PK_JumpToResult_Time
			Write32(pBaseCfg, 0); //Helicopter_Missile_LifeTime
			Write32(pBaseCfg, 0); //Helicopter_Show_Time
			Write32(pBaseCfg, 0); //Helicopter_Attack_Time
			Write32(pBaseCfg, 0); //Helicopter_Full_Limit_Time
			Write32(pBaseCfg, 0); //Helicopter_Attack_SpaceTime
			Write32(pBaseCfg, 0); //Helicopter_Explode_Height
			Write32(pBaseCfg, 0); //Skill_Gun_SpeedFator
			Write32(pBaseCfg, 0); //PKLeaveDistance
			Write32(pBaseCfg, 0); //SkillBeginDeltaTime
			Write32(pBaseCfg, 0); //MultipleGameBeginTime
			Write32(pBaseCfg, 0); //MultipleGameEndTime
			WriteUn(pBaseCfg, 0); //isMultipleTime
			Write32(pBaseCfg, 0); //MultiPleAwardPowering
			Write32(pBaseCfg, 0); //MultipleAwardTimes

			len = pBaseCfg - pGangsterCfg;
			Set16(pGangsterCfg, (WORD)len);
			pGangsterCfg += len;
		}
		Write32(pGangsterCfg, 0); //MapCfgNum
		for (size_t i = 0; i < n; i++)
		{ //MapCfg
			BYTE* pMapCfg = pGangsterCfg;
			Write16(pMapCfg, 0); //len

			Write32(pMapCfg, 0); //MapId
			Write32(pMapCfg, 0); //MinGameTime
			Write32(pMapCfg, 0); //MaxGameTime
			Write32(pMapCfg, 0); //PoliceFinishScore
			Write32(pMapCfg, 0); //GansterFinishMapRound
			Write32(pMapCfg, 0); //PoliceAddScoreByGansterLeave
			Write32(pMapCfg, 0); //Weight
			Write32(pMapCfg, 0); //MatchIndentifyFlag
			Write32(pMapCfg, 0); //Priority

			len = pMapCfg - pGangsterCfg;
			Set16(pGangsterCfg, (WORD)len);
			pGangsterCfg += len;
		}
		Write32(pGangsterCfg, 0); //SkillCfgNum
		for (size_t i = 0; i < n; i++)
		{ //SkillCfg
			BYTE* pSkillCfg = pGangsterCfg;
			Write16(pSkillCfg, 0); //len

			Write32(pSkillCfg, 0); //SkillId
			Write32(pSkillCfg, 0); //SkillAffectType
			Write32(pSkillCfg, 0); //Indentify
			Write32(pSkillCfg, 0); //ColdTime
			Write32(pSkillCfg, 0); //LifeTime
			Write32(pSkillCfg, 0); //AffectTime
			Write32(pSkillCfg, 0); //BloodHurt
			Write32(pSkillCfg, 0); //SpeedDrop
			Write32(pSkillCfg, 0); //ProtectRate

			len = pSkillCfg - pGangsterCfg;
			Set16(pGangsterCfg, (WORD)len);
			pGangsterCfg += len;
		}
		Write32(pGangsterCfg, 0); //SeasonCfgNum
		for (size_t i = 0; i < n; i++)
		{ //SeasonCfg
			BYTE* pSeasonCfg = pGangsterCfg;
			Write16(pSeasonCfg, 0); //len

			Write32(pSeasonCfg, 0); //SeasonId
			Write32(pSeasonCfg, 0); //OpenDate
			Write32(pSeasonCfg, 0); //CloseDate
			Write32(pSeasonCfg, 0); //OpneTimeCfgNum
			for (size_t i = 0; i < n; i++)
			{ //GangsterOpenTimeCfg
				BYTE* pGangsterOpenTimeCfg = pSeasonCfg;
				Write16(pGangsterOpenTimeCfg, 0); //len

				Write32(pGangsterOpenTimeCfg, 0); //OpenTime
				Write32(pGangsterOpenTimeCfg, 0); //CloseTime

				len = pGangsterOpenTimeCfg - pSeasonCfg;
				Set16(pSeasonCfg, (WORD)len);
				pSeasonCfg += len;
			}

			len = pSeasonCfg - pGangsterCfg;
			Set16(pGangsterCfg, (WORD)len);
			pGangsterCfg += len;
		}
		Write32(pGangsterCfg, 0); //ResultCfgNum
		for (size_t i = 0; i < n; i++)
		{ //ResultCfg
			BYTE* pResultCfg = pGangsterCfg;
			Write16(pResultCfg, 0); //len

			Write32(pResultCfg, 0); //PlayerNum
			Write32(pResultCfg, 0); //WinTeamBaseExp
			Write32(pResultCfg, 0); //LoseTeamBaseExp
			Write32(pResultCfg, 0); //WinTeamBaseFragment
			Write32(pResultCfg, 0); //LoseTeamBaseFragment
			{ //PoliceTeamWinExtra
				BYTE* pPoliceTeamWinExtra = pResultCfg;
				Write16(pPoliceTeamWinExtra, 0); //len

				Write32(pPoliceTeamWinExtra, 0); //PolicePlayerExtraNum
				for (size_t i = 0; i < n; i++)
				{ //PolicePlayerExtra
					BYTE* pPolicePlayerExtra = pPoliceTeamWinExtra;
					Write16(pPolicePlayerExtra, 0); //len

					Write32(pPolicePlayerExtra, 0); //HighKillPercent
					Write32(pPolicePlayerExtra, 0); //ExtraExp
					Write32(pPolicePlayerExtra, 0); //ExtraFragment

					len = pPolicePlayerExtra - pPoliceTeamWinExtra;
					Set16(pPoliceTeamWinExtra, (WORD)len);
					pPoliceTeamWinExtra += len;
				}
				Write32(pPoliceTeamWinExtra, 0); //GangsterPlayerExtraNum
				for (size_t i = 0; i < n; i++)
				{ //GangsterPlayerExtra
					BYTE* pGangsterPlayerExtra = pPoliceTeamWinExtra;
					Write16(pGangsterPlayerExtra, 0); //len

					Write32(pGangsterPlayerExtra, 0); //LowDeadPercent
					Write32(pGangsterPlayerExtra, 0); //ExtraExp
					Write32(pGangsterPlayerExtra, 0); //ExtraFragment

					len = pGangsterPlayerExtra - pPoliceTeamWinExtra;
					Set16(pPoliceTeamWinExtra, (WORD)len);
					pPoliceTeamWinExtra += len;
				}

				len = pPoliceTeamWinExtra - pResultCfg;
				Set16(pResultCfg, (WORD)len);
				pResultCfg += len;
			}
			{ //GangsterTeamWinExtra
				BYTE* pGangsterTeamWinExtra = pResultCfg;
				Write16(pGangsterTeamWinExtra, 0); //len

				Write32(pGangsterTeamWinExtra, 0); //PolicePlayerExtraNum
				for (size_t i = 0; i < n; i++)
				{ //PolicePlayerExtra
					BYTE* pPolicePlayerExtra = pGangsterTeamWinExtra;
					Write16(pPolicePlayerExtra, 0); //len

					Write32(pPolicePlayerExtra, 0); //HighKillPercent
					Write32(pPolicePlayerExtra, 0); //ExtraExp
					Write32(pPolicePlayerExtra, 0); //ExtraFragment

					len = pPolicePlayerExtra - pGangsterTeamWinExtra;
					Set16(pGangsterTeamWinExtra, (WORD)len);
					pGangsterTeamWinExtra += len;
				}
				Write32(pGangsterTeamWinExtra, 0); //GangsterPlayerExtraNum
				for (size_t i = 0; i < n; i++)
				{ //GangsterPlayerExtra
					BYTE* pGangsterPlayerExtra = pGangsterTeamWinExtra;
					Write16(pGangsterPlayerExtra, 0); //len

					Write32(pGangsterPlayerExtra, 0); //LowDeadPercent
					Write32(pGangsterPlayerExtra, 0); //ExtraExp
					Write32(pGangsterPlayerExtra, 0); //ExtraFragment

					len = pGangsterPlayerExtra - pGangsterTeamWinExtra;
					Set16(pGangsterTeamWinExtra, (WORD)len);
					pGangsterTeamWinExtra += len;
				}

				len = pGangsterTeamWinExtra - pResultCfg;
				Set16(pResultCfg, (WORD)len);
				pResultCfg += len;
			}

			len = pResultCfg - pGangsterCfg;
			Set16(pGangsterCfg, (WORD)len);
			pGangsterCfg += len;
		}
		Write32(pGangsterCfg, 0); //ExchangeCfgNum
		for (size_t i = 0; i < n; i++)
		{ //ExchangeCfg
			BYTE* pExchangeCfg = pGangsterCfg;
			Write16(pExchangeCfg, 0); //len

			Write32(pExchangeCfg, 0); //Level
			Write32(pExchangeCfg, 0); //MaxExchangeTimes
			Write32(pExchangeCfg, 0); //ConsumeFragmentNum
			Write32(pExchangeCfg, 0); //AwardId

			len = pExchangeCfg - pGangsterCfg;
			Set16(pGangsterCfg, (WORD)len);
			pGangsterCfg += len;
		}

		len = pGangsterCfg - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;
	SendToClient(Client, 653, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}


