#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include"Message.h"

enum class FieldType {
	INT,
	VARCHAR,
	// 可根据需要添加更多数据类型
};
void InitDB();
void CloseDB();
void InitTaskInfo();
BOOL UserDB_Register(const char* Username, const char* Password, const char* cdk);
BOOL UserDB_RegisterNew(const char* Username, const char* Password, const char* yqm);
DWORD UserDB_GetUin(const char* ip, const char* Username, const char* Pwd);
BOOL UserDB_GetUin(UINT Uin);
void UserDB_GetAllUin(std::vector<UINT>& Uins);
void UserDB_UpdateMD5();
void UserDB_UpdateMD5(const char* Username);
std::string UserDB_GetLoginKey(UINT Uin);
void UserDB_UpdateLoginKey(UINT Uin, const char* key);
BOOL UserDB_CmpIP(const char* IP);
BOOL UserDB_SelectIP(const char* IP);
std::string UserDB_GetIP(UINT Uin);
BOOL UserDB_SelectIPNoTime(const char* IP);
void UserDB_ConnID(DWORD Uin, DWORD ConnID);
void UserDB_SelectConnID(std::vector<UINT>& UinConnids);
void UserDB_UpdateConnID(DWORD Uin);
void UserDB_UpdateConnID();
UINT UserDB_SelectConnID(DWORD Uin);
UINT UserDB_SelectUsr(DWORD ConnID);

BOOL UserDB_CmpPassword(DWORD Uin, const char* Password);
int PlayerDB_AddItem1(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status, sql::Connection* conn);

int PlayerDB_AddItem2(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status);
int PlayerDB_AddItem3(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status, bool Skip);
int PlayerDB_AddItem4(DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status);
int PlayerDB_AddItem5(DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status, sql::Connection* conn);

int PlayerDB_AddStoneCar(std::shared_ptr<ClientNode> Client);
int PlayerDB_GetStone(UINT Uin, std::vector<UINT>& vecItem);
int PlayerDB_GetItemAvailPeriod(UINT Uin, UINT Itemid);
int PlayerDB_GetKartRefit(UINT Uin, std::vector<UINT>& vecItem);
int PlayerDB_AddKartStoneGroove(UINT Uin, std::vector<UINT> vecItem);
int PlayerDB_AddKartRefit(UINT Uin, std::vector<UINT> vecItem);
DWORD PlayerDB_GetMapRecord(DWORD Uin, int MapID, std::shared_ptr<ClientNode> Client);
bool PlayerDB_SetMapRecord(DWORD Uin, int MapID, DWORD Record, std::shared_ptr<ClientNode> Client);
void UserDB_ban(UINT Uin);
void PlayerDB_AddMagicCupTempBox(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, int ItemType);
BOOL PlayerDB_SelectClientInfo(UINT Uin, ClientInfo*& clientInfo);
BOOL PlayerDB_SelectClientInfo(std::shared_ptr<ClientNode> Client);

int PlayerDB_GetItemNum(DWORD Uin, DWORD ItemID);
BOOL PlayerDB_DeleteItem(DWORD Uin, int ItemID);
void PlayerDB_Reduce(UINT Uin, UINT ItemID, int Num);
void PlayerDB_UpdateItemNum(UINT Uin, UINT ItemID, int Num);
void PlayerDB_UpdateItemWareHouse(UINT Uin, UINT ItemID, int OpType);
void PlayerDB_UpdateItemOtherAttribute(UINT Uin, UINT ItemID, int type);
int PlayerDB_SelectWareHouseItem(UINT Uin, std::vector<ItemInfo>& Items);
int PlayerDB_SelectItems(UINT Uin, std::vector<ItemInfo>& vecItem);
int PlayerDB_SelectItemsStatus(UINT Uin, std::vector<ItemInfo>& vecItem);
int PlayerDB_LoginSelectItems(UINT Uin, std::vector<ItemInfo>& vecItem);
int PlayerDB_LoginSelectOverItems(UINT Uin, std::vector<ItemInfo>& vecItem);

BOOL PlayerDB_IsHasFriend(UINT Uin, UINT FriendUin);
void PlayerDB_InsertFriend(UINT Uin, UINT FriendUin);
void PlayerDB_UpdateEngageInfo(UINT Uin, UINT EngageUin);
void PlayerDB_UpdateEngageInfoStat(std::shared_ptr<ClientNode> Client);
void PlayerDB_InsertEngageInfo(UINT Uin, UINT EngageUin);
void PlayerDB_DeleteEngageInfo(UINT Uin, UINT EngageUin);
void PlayerDB_UpdateHead(std::shared_ptr<ClientNode> Client);
void PlayerDB_UpdateNobel(UINT Uin, UINT NobleID);
void PlayerDB_InsertStoneNew();
int PlayerDB_UpdateKartStone(UINT Uin, UINT KartID, UINT StoneGrooveID, UINT SkillID);
void PlayerDB_SelectKartStone(UINT Uin, UINT KartID, std::vector<KartStoneItemInfo>& kartStoneItemInfo);


void PlayerDB_UpdatMagicLampInfo(UINT Uin, UINT ItemID, int LightOn, int LuckyValue);
void PlayerDB_DeleteMagicCupTempBox(UINT Uin, UINT ItemID);
int PlayerDB_GetMagicLampNum(DWORD Uin, DWORD ItemID);
void PlayerDB_InsertMagicLamp(DWORD Uin, DWORD ItemID);
void PlayerDB_UpdateBugletTalk(std::shared_ptr<ClientNode> Client);

UINT PlayerDB_SelectGuildID(UINT GuildID, std::string& NickName);
void PlayerDB_SelectGuildInfo(UINT GuildID, Guidinfo*& guidinfo);
void PlayerDB_SelectGuildInfoByUin(UINT Uin, Guidinfo*& guidinfo);
UINT PlayerDB_SelectGuildInfoAll(Guidinfo*& guidinfo);
UINT PlayerDB_SelectGuildMember(UINT GuildID, Guidinfo*& guidinfo);
void PlayerDB_InsertGuildList(UINT Uin, char* newGuildname);
void PlayerDB_UpdateGuildList(UINT ID);
void PlayerDB_UpdateGuildListEnlistType(UINT ID);
void PlayerDB_UpdateGuildListCurrentMember(UINT ID);
void PlayerDB_InsertGuildInfo(DWORD Uin, UINT GuildID, UINT Duty);
void PlayerDB_DeleteGuildInfo(DWORD Uin);
void PlayerDB_UpdateGuildInfoDuty(UINT Uin, UINT Duty);
void PlayerDB_UpdateGuildInfoDuty1(UINT Uin, UINT Duty, bool isTransferLeader = false);
UINT GetGuildIDByUin(UINT uin);

void PlayerDB_UpdateGuildListScore(UINT GuildID, UINT Score, UINT Prosperity);
void PlayerDB_UpdateGuildInfoScore(UINT Uin, UINT GuildID, UINT MeProsperity);

void PlayerDB_InsertKartRefit(DWORD Uin, UINT KartItemID);
void PlayerDB_UpdateKartRefit(DWORD Uin, UINT* RefitItemID, UINT ShapeSuitID, UINT ShapeLegendSuitID, UINT SkinSuitID, UINT KartItemID);
void PlayerDB_UpNickName(DWORD Uin, std::string& NickName);
int PlayerDB_SelectNickName(UINT Uin, std::string& NickName);
int PlayerDB_SelectMapInfo(UINT Uin, std::vector<MapInfo>& mapInfo);
int PlayerDB_SelectPetInfo(UINT Uin, std::vector<PetInfo>& petInfo);
int PlayerDB_SelectRankInfo(std::shared_ptr<ClientNode> Client);

int PlayerDB_GetMagicCupTempBoxNum(DWORD Uin, DWORD ItemID);
int PlayerDB_GetMagicCupTempBox(DWORD Uin, std::vector<ItemInfo>& Items);

void PlayerDB_GetMagicFormationNum(DWORD Uin, std::unordered_map<int, int>& Items);
void PlayerDB_InsertMagicFormation(DWORD Uin, UINT ItemID);
void PlayerDB_GetMagicShopInfo(DWORD Uin, std::unordered_map<int, int>& Items);
void PlayerDB_InsertMagicShopInfo(DWORD Uin, UINT ItemID);

int PlayerDB_GetPetLuck(DWORD Uin, UINT Petid);
void PlayerDB_UpdatePetLuck(DWORD Uin, UINT Petid, UINT Luck);
void PlayerDB_UpdatePetStrengLevel(DWORD Uin, UINT Petid, UINT StrengLevel);

void PlayerDB_InsertAuctionUnPayInfo(DWORD Uin, UINT BidPrice, AuctionItemInfo AuctionItems);
void PlayerDB_DeleteAuctionInfo(UINT CommodityId);
void PlayerDB_DeleteAuctionUnPayInfo(UINT Uin, UINT CommodityId);
UINT PlayerDB_SelectAuctionUnPayInfo(DWORD Uin, std::vector<AuctionItemInfo>& auction);

int PlayerDB_SelectPlayerInfo(std::shared_ptr<ClientNode> Client, std::vector<PlayerInfo>& playerInfo);
int PlayerDB_SelectPlayerInfo(UINT Uin, std::vector<PlayerInfo>& playerInfo);
int PlayerDB_UpdatePlayerInfo(std::shared_ptr<ClientNode> Client, char* CurLoginTm);
int PlayerDB_InsertPlayer(UINT Uin);
int PlayerDB_InsertBaseInfo(UINT Uin, char* newname, UINT Gender, UINT Country);
int PlayerDB_UpdateBaseInfo(UINT Uin, std::string qqNumber);
int PlayerDB_UpdateBaseInfoFps(std::shared_ptr<ClientNode> Client);
int PlayerDB_SelectBaseInfoKartLibaray(UINT Uin, std::vector<UINT>& myKartLibrary);
int PlayerDB_UpdateBaseInfoKartLibaray(std::shared_ptr<ClientNode> Client);
int PlayerDB_SeleteBaseInfoHead(UINT Uin, std::vector<PlayerInfo>& playerInfo);

int PlayerDB_SelectEngageInfo(std::shared_ptr<ClientNode> Client);


void PlayerDB_AddComItemTable(std::shared_ptr<ClientNode> Client, UINT ItemId, int Aviable, int num);
void PlayerDB_UpdateComItemTable(std::shared_ptr<ClientNode> Client, UINT ItemId, int Aviable, int num);
void PlayerDB_SelectComItemTable(std::shared_ptr<ClientNode> Client, BYTE*& pData, double& intValue);
int PlayerDB_SelectComItemTableNum(std::shared_ptr<ClientNode> Client);
int PlayerDB_GetExchangePieceNum(std::shared_ptr<ClientNode> Client, UINT ItemId, int Aviable, UINT ItemNum);
void PlayerDB_SetRankedInfo(std::shared_ptr<ClientNode> Client);
BOOL isDBtHasItem(std::shared_ptr<ClientNode> Client, int ItemID, int count);
void UpdateClientBase(std::shared_ptr<ClientNode> Client);
void UpdateGuildInfo(std::shared_ptr<ClientNode> Client, int score);
void SelectClientExp(std::shared_ptr<ClientNode> Client);
UINT SelectClientExp(UINT Uin);
BOOLEAN SelectSuperItem(std::shared_ptr<ClientNode> Client, UINT Itemid);
void InsertSign3Info(std::shared_ptr<ClientNode> Client);
void UpdateSign3Info(std::shared_ptr<ClientNode> Client, char NextSignDay, char* CurTm, int SignWeek);
void UpdateSign3InfoWeek(std::shared_ptr<ClientNode> Client);
void UpdateBuglet(std::shared_ptr<ClientNode> Client);
std::vector<int> executeQueryRow(const std::string& sqlQuery, const std::string& queryName, const std::vector<int>& conditionValues);
std::vector<int> executeQueryCol(const std::string& sqlQuery, const std::string& queryName, const std::vector<int>& conditionValues);
std::vector<int> executeQueryRow(const std::string& sqlQuery, const std::string& queryName, const std::vector<std::string>& conditionValues);
void UpdateClientKart(std::shared_ptr<ClientNode> Client, int Speed2Weight, int DriftVecWeight, int AdditionalZSpeedWeight, int KartItemID);

void SelectTopList(const std::string& sqlQuery, std::vector<UINT>& List);
UINT SelectKartRefitItemInfo(UINT Uin, KartRefitItemInfo*& KartRefit);
UINT SelectKartRefitItemInfo(UINT Uin, std::vector<KartRefitItemInfo>& KartRefit);
void SelectOneKartRefitItemInfo(UINT Uin, UINT KartID, KartRefitItemInfo*& KartRefit);
void UpdateOneKartRefitItemInfo(UINT Uin, UINT KartID, KartRefitItemInfo*& KartRefit);

void InsertBattleRecord(PlayerRaceInfo* ArrPlayerRaceInfo, UINT Mapid, UINT GameMode);
void UpdateAppellationType(UINT Uin, int type);
int SelectAppellationType(UINT Uin);
void SelectGmTime(int Time);
int SelectChallengeMap(UINT Mapid, UINT RankID, std::vector<ChallengeMap>& ChallengeMaps);
int SelectChallengeMap(std::vector<ChallengeMap>& ChallengeMaps);
int SelectChallengeMap(std::shared_ptr<ClientNode> Client, std::vector<ChallengeMap>& ChallengeMaps);
int UpdateChallengeMapCNT(UINT Mapid, UINT RankID);
int SelectChallengeMapTime(std::shared_ptr<ClientNode> Client);
int SelectChallengeAllMap(std::vector<ChallengeMap>& ChallengeAllMaps, std::vector<ChallengeMapsInfo> ChallengeMaps);
int InSertChallengeMap(std::shared_ptr<ClientNode> Client, ChallengeMap* CM, std::string FilePath);
int SelectBaseInfoEffect(UINT Uin);
int SelectMyKartLibarayNum(UINT Uin, char* Mess);
int SelectMyKartLibarayAllNum(char* Mess);

int Selectmechskinaward(UINT Uin, std::vector<MechTreasureInfo>& MechTreasureInfos);
int SelectmechskinawardGetNum(UINT Uin, UINT Itemid, int Available);
int Insertmechskinaward(UINT Uin, MechTreasureInfo mechTreasureInfo);
int Updatemechskinaward(UINT Uin, std::string Col, UINT Itemid, int itemnum, int Available);
