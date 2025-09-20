#include "Message.h"
#include "Utils.h"
#include "Database.h"
#include <optional> // 为了使用 std::optional
#include <numeric>   // for std::accumulate
#include <codecvt>
#include "Item.h"

extern bool GetRandItemNew(std::shared_ptr<ClientNode> Client, UINT Itemid, short count, std::vector<ItemInfo>& items);

extern std::vector<MechTreasureInfo> mechTreasureInfo;
extern std::vector<MechTreasureInfo> mechExchangeTreasureInfo;
extern std::string qqurl;
extern std::shared_ptr<CarTreasure> CarTreasures;
extern int Uinew;
extern double version;
extern std::vector<MapChallengeOpenMain> mapChallengeOpenMain;
extern std::vector<MapChallengeOpenMainMap> mapChallengeCreate;

//函数用于查找 MechTreasureInfo
std::optional<MechTreasureInfo> findMechTreasureInfo(
	const std::vector<MechTreasureInfo>& mechExchangeTreasureInfo,
	UINT targetItemId,
	INT targetAvailPeriod)
{
	auto it = std::find_if(mechExchangeTreasureInfo.begin(), mechExchangeTreasureInfo.end(),
		[targetItemId, targetAvailPeriod](const MechTreasureInfo& treasureInfo) {
		return treasureInfo.ItemId == targetItemId && treasureInfo.AvailPeriod == targetAvailPeriod;
	});

	if (it != mechExchangeTreasureInfo.end()) {
		return *it; // 返回找到的 MechTreasureInfo
	}
	else {
		return std::nullopt; // 返回空的可选值
	}
}

// 主函数或适当的函数
std::vector<MechTreasureInfo> FillRandomItems(std::shared_ptr<ClientNode> Client, int num) {
	// 获取随机项
	std::vector<ItemInfo> TempItems;
	bool result = GetRandItemNew(Client, 24727, num, TempItems);

	// 用于存放随机项的结果
	std::vector<MechTreasureInfo> randomItems;

	for (size_t i = 0; i < num; i++) {
		int itemid = TempItems[i].ItemID;
		int AvailPeriod = TempItems[i].AvailPeriod;

		// 遍历 mechTreasureInfo 查找匹配的项
		for (const auto& treasureInfo : mechTreasureInfo) {
			if (treasureInfo.ItemId == itemid && treasureInfo.AvailPeriod == AvailPeriod) {
				randomItems.push_back(treasureInfo); // 将匹配项添加到 randomItems
				break; // 找到匹配后可以跳出当前循环
			}
		}
	}
	// randomItems 现在包含所有匹配的 MechTreasureInfo
	// 可以在这里进行进一步处理
	return randomItems;
}


inline void WriteRequestHead(BYTE*& ptr, DWORD Uin) {
	BYTE* pHead = ptr;
	Write16(pHead, 0); //len

	Write32(pHead, Uin); //Uin
	Write32(pHead, (DWORD)time(nullptr)); //Time
	Write8(pHead, 0); //Version
	Write8(pHead, 0); //Compress
	Write8(pHead, 0); //OptLength

	DWORD len = pHead - ptr;
	Set16(ptr, (WORD)len);
	ptr += len;
}


NOINLINE void RequestGetBeta84ColorfulGourdCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 38;
	double readValue1 = ReadValue(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000cf3");
			WriteText(pData, "base_cfg");
			WriteHex(pData, "FD00000347");
			WriteText(pData, "show_final_itemid");
			WriteValue(pData, 131937);
			WriteText(pData, "closeDate");
			WriteValue(pData, 20241117);
			WriteText(pData, "max_sage_egg_can_get_num");
			WriteValue(pData, 60);
			WriteText(pData, "iCurrentStartLev");
			WriteValue(pData, 0);
			WriteText(pData, "fenjie_item_id");
			WriteValue(pData, 132095);
			WriteText(pData, "putong_callon_cost_num");
			WriteHex(pData, "FD00000090");
			WriteValue(pData, 1);
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteValue(pData, 2);
			WriteValue(pData, 4);
			WriteValue(pData, 3);
			WriteValue(pData, 5);
			WriteValue(pData, 4);
			WriteValue(pData, 6);
			WriteValue(pData, 5);
			WriteValue(pData, 7);
			WriteValue(pData, 6);
			WriteValue(pData, 8);
			WriteValue(pData, 7);
			WriteText(pData, "baoDiLeftNum");
			WriteValue(pData, 3);
			WriteText(pData, "baoDiType");
			WriteValue(pData, 0);
			WriteText(pData, "iOpenStatus");
			WriteValue(pData, 0);
			WriteText(pData, "baodiAward");
			WriteHex(pData, "FD00000000");
			WriteText(pData, "sage_egg_itemid");
			WriteValue(pData, 132089);
			WriteText(pData, "iFreezingDate");
			WriteValue(pData, 20241118);
			WriteText(pData, "putong_callon_itemid");
			WriteValue(pData, 132096);
			WriteText(pData, "cur_sage_egg_get_num");
			WriteValue(pData, 0);
			WriteText(pData, "closeTime");
			WriteValue(pData, 235959);
			WriteText(pData, "rare_callon_cost_num");
			WriteHex(pData, "FD00000090");
			WriteValue(pData, 1);
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteValue(pData, 2);
			WriteValue(pData, 4);
			WriteValue(pData, 3);
			WriteValue(pData, 5);
			WriteValue(pData, 4);
			WriteValue(pData, 6);
			WriteValue(pData, 5);
			WriteValue(pData, 7);
			WriteValue(pData, 6);
			WriteValue(pData, 8);
			WriteValue(pData, 7);
			WriteText(pData, "openTime");
			WriteValue(pData, 0);
			WriteText(pData, "openDate");
			WriteValue(pData, 20241019);
			WriteText(pData, "exchangeSkinNeedSageNum");
			WriteValue(pData, 30);
			WriteText(pData, "confirm_switch");
			WriteValue(pData, 3);
			WriteText(pData, "season_id");
			WriteValue(pData, 1);
			WriteText(pData, "rare_callon_itemid");
			WriteValue(pData, 132097);
			WriteText(pData, "star_levup_award");
			WriteHex(pData, "FD00000428");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000016");
			WriteText(pData, "award_item_info");
			WriteHex(pData, "FD00000000");
			WriteValue(pData, 2);
			WriteHex(pData, "FD000000c4");
			WriteText(pData, "iStarLev");
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000091");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000083");
			WriteText(pData, "itemNum");
			WriteValue(pData, 1);
			WriteText(pData, "itemType");
			WriteValue(pData, 1);
			WriteText(pData, "iAvailPeriod");
			WriteValue(pData, -1);
			WriteText(pData, "itemId");
			WriteValue(pData, 132160);
			WriteText(pData, "iExchangePieceNum");
			WriteValue(pData, 1);
			WriteText(pData, "iOtherAttribute");
			WriteValue(pData, 0);
			WriteText(pData, "iReceiveStatus");
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteHex(pData, "FD0000006d");
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000050");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 132130);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 32);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteHex(pData, "FD0000006d");
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000050");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 133240);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 32);
			WriteValue(pData, 0);
			WriteValue(pData, 5);
			WriteHex(pData, "FD0000006d");
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000050");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 132224);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 42);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 32);
			WriteValue(pData, 0);
			WriteValue(pData, 6);
			WriteHex(pData, "FD0000006d");
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000050");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 133248);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 69);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 32);
			WriteValue(pData, 0);
			WriteValue(pData, 7);
			WriteHex(pData, "FD0000006d");
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000050");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 108454);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 150);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 32);
			WriteValue(pData, 0);
			WriteValue(pData, 8);
			WriteHex(pData, "FD000000bd");
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 7);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD000000a0");
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 131937);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 864);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000042");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 98830);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 864);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 32);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 24);
			WriteHex(pData, "FD00000542");
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000d4");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, -1);
			WriteText(pData, "iRareProbability");
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteText(pData, "iCtrlType");
			WriteValue(pData, 1);
			WriteText(pData, "poll_index");
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteText(pData, "iSuperMoney");
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 20);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 23057);
			WriteText(pData, "icoupons");
			WriteValue(pData, 0);
			WriteText(pData, "iProbability");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 100);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 13532);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 1000);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 100);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 15985);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 1000);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 300);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 72313);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 3000);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 500);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 72315);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 5000);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 1000);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 12376);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 100000);
			WriteValue(pData, 7);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 1000);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 12378);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 100000);
			WriteValue(pData, 8);
			WriteHex(pData, "FD00000092");
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 1000);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 34);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 35);
			WriteHex(pData, "FD00000012");
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 36);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 12380);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 38);
			WriteValue(pData, 100000);
			WriteText(pData, "ZancunItemList");
			WriteHex(pData, "FD00000000");
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta81_ReqMechSkinEntrys(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 30;
	double readValue1 = ReadValue(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000189");//固定长度
			WriteText(pData, "openMechSkinEntrys");
			WriteHex(pData, "FD00000170");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000058");//固定长度
			WriteText(pData, "skinid");
			WriteValue(pData, 123513);
			WriteText(pData, "name");
			WriteText(pData, "孙悟空召唤");
			WriteText(pData, "mech_skin_type");
			WriteValue(pData, 1);
			WriteText(pData, "priority");
			WriteValue(pData, 100);
			WriteText(pData, "isOpen");
			WriteHex(pData, "F9");//F9
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000035");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 138408);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "墨影青龙-敖丙");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 89);
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "F8");//F8
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000035");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 126355);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "源极之星X计划");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 88);
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "F9");//F9
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000036");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 133683);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "源极·烟雨天青");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 88);
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "F9");//F9
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000032");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 130539);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "奥莉娜召唤");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 66);
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "F9");//F9
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta78_MechSkinAwardOpt(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.3) return;
	Body += 29;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue(Body);
	double readValue3 = ReadValue(Body);
	int uintValue = static_cast<UINT>(readValue1);
	int uintValue2 = static_cast<UINT>(readValue2);
	int uintValue3 = static_cast<UINT>(readValue3);
	//召唤
	if (uintValue3 == 1) {
		int num = static_cast<UINT>(ReadValue(Body));
		int MyNum = PlayerDB_GetItemNum(Client->Uin, 138385);
		if (MyNum < num) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
					WriteValue(pData, 123);
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			//PrintfBody(buf.data(), len);
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}
		else
		{
			PlayerDB_Reduce(Client->Uin, 138385, num);
			NotifyUseItemResult(Client, 138385, num, PlayerDB_GetItemNum(Client->Uin, 138385));
			ItemInfo Item = { 0 };
			Item.ItemID = 138385;
			Item.ItemNum = PlayerDB_GetItemNum(Client->Uin, 138385);
			Item.AvailPeriod = 0;
			NotifyClientAddItemBox(Client, 1, { Item });
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;

			std::vector<MechTreasureInfo> randomItems = FillRandomItems(Client, num);
			if (randomItems.size() == 0) return;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHead(pBody, readValue1);
					BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
					BYTE* pAwardOptInfo = pData;
					{
						WriteTextValue(pAwardOptInfo, "m_sItemNum", num);
						WriteTextValue(pAwardOptInfo, "m_iSuperMoney", 0);
						WriteTextValue(pAwardOptInfo, "m_iCoupons", 0);
						WriteTextValue(pAwardOptInfo, "m_iSpeedCoin", 0);
						WriteText(pAwardOptInfo, "m_astItemInfo");
						{
							BYTE* pAwardOptItemInfoLen = WriteLenValueStart(pAwardOptInfo);
							BYTE* pAwardOptItemInfo = pAwardOptInfo;
							WriteValue(pAwardOptItemInfo, 1);
							WriteHex(pAwardOptItemInfo, "FD00000084");//动态长度 后面的不要改
							WriteTextValue(pAwardOptItemInfo, "m_iAvailPeriod", randomItems[0].AvailPeriod);
							WriteTextValue(pAwardOptItemInfo, "m_ushItemType", randomItems[0].item_type);
							WriteTextValue(pAwardOptItemInfo, "exchangePieceNum", randomItems[0].exchangePieceNum);
							WriteTextValue(pAwardOptItemInfo, "m_iItemNum", randomItems[0].ItemNum);
							WriteTextValue(pAwardOptItemInfo, "_idx", 1);
							WriteTextValue(pAwardOptItemInfo, "m_iItemID", randomItems[0].ItemId);
							Insertmechskinaward(Client->Uin, randomItems[0]);

							for (size_t i = 1; i < num; i++)
							{
								Insertmechskinaward(Client->Uin, randomItems[i]);
								WriteValue(pAwardOptItemInfo, i + 1);
								{
									BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pAwardOptItemInfo);
									BYTE* pAwardOptInfo2 = pAwardOptItemInfo;
									{
										WriteKeyValue(pAwardOptInfo2, 5, randomItems[i].AvailPeriod);//m_iAvailPeriod
										WriteKeyValue(pAwardOptInfo2, 6, randomItems[i].item_type);//m_ushItemType
										WriteKeyValue(pAwardOptInfo2, 7, randomItems[i].exchangePieceNum);//exchangePieceNum
										WriteKeyValue(pAwardOptInfo2, 8, randomItems[i].ItemNum);//m_iItemNum
										WriteKeyValue(pAwardOptInfo2, 9, i + 1);//_idx
										WriteKeyValue(pAwardOptInfo2, 10, randomItems[i].ItemId);//m_iItemID
										WriteLenValueEnd(pAwardOptItemInfo, pAwardOptInfo2, pAwardOptInfoLen2);
									}
								}

							}
							WriteLenValueEnd(pAwardOptInfo, pAwardOptItemInfo, pAwardOptItemInfoLen);
						}
						WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);
					}
					{
						BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pData);
						BYTE* pAwardOptInfo3 = pData;
						WriteText(pAwardOptInfo3, "supermoney");
						WriteLenValue(pAwardOptInfo3, 0);
						WriteText(pAwardOptInfo3, "coupons");
						WriteLenValue(pAwardOptInfo3, 0);
						WriteTextValue(pAwardOptInfo3, "levUpPieceNum", 0);
						WriteLenValueEnd(pData, pAwardOptInfo3, pAwardOptInfoLen3);
					}
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}

	}
	//兑换
	if (uintValue3 == 2) {
		int exchangeItemnum = static_cast<UINT>(ReadValue(Body));
		int exchangeNeedNum = 0;
		int MyNum = PlayerDB_GetItemNum(Client->Uin, 138387);
		int targetAwardIdx = static_cast<UINT>(ReadValue(Body));
		BOOL IsEnough = false;
		// 使用 std::find_if 查找
		auto it = std::find_if(mechExchangeTreasureInfo.begin(), mechExchangeTreasureInfo.end(),
			[targetAwardIdx](const MechTreasureInfo& treasureInfo) {
			return treasureInfo.award_idx == targetAwardIdx;
		});

		// 检查是否找到元素
		if (it != mechExchangeTreasureInfo.end()) {
			exchangeNeedNum = it->exchangeNeedNum * exchangeItemnum;
			IsEnough = MyNum > exchangeNeedNum ? true : false;
		}
		if (IsEnough) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			ItemInfo Item = { 0 };
			Item.ItemID = it->ItemId;
			Item.ItemNum = it->ItemNum;
			Item.AvailPeriod = it->AvailPeriod;

			Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);

			NotifyClientAddItemBox(Client, 1, { Item });
			NotifyUseItemResult2(Client, 138387, it->exchangeNeedNum * exchangeItemnum);
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHead(pBody, readValue1);
					BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
					BYTE* pAwardOptInfo = pData;
					{
						WriteTextValue(pAwardOptInfo, "m_sItemNum", 1);
						WriteTextValue(pAwardOptInfo, "m_iSuperMoney", 0);
						WriteTextValue(pAwardOptInfo, "m_iCoupons", 0);
						WriteTextValue(pAwardOptInfo, "m_iSpeedCoin", 0);
						WriteText(pAwardOptInfo, "m_astItemInfo");

						BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pAwardOptInfo);
						BYTE* pAwardOptInfo2 = pAwardOptInfo;
						WriteValue(pAwardOptInfo2, 1);
						{
							BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
							BYTE* pAwardOptInfo3 = pAwardOptInfo2;
							{
								WriteTextValue(pAwardOptInfo3, "m_iAvailPeriod", it->AvailPeriod);
								WriteTextValue(pAwardOptInfo3, "m_ushItemType", it->item_type);
								WriteTextValue(pAwardOptInfo3, "m_iItemID", it->ItemId);
								WriteTextValue(pAwardOptInfo3, "m_iItemNum", it->ItemNum);
								WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
							}
						}
						WriteLenValueEnd(pAwardOptInfo, pAwardOptInfo2, pAwardOptInfoLen2);
					}
					WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			//PrintfBody(buf.data(), len);
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}
		else {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
					WriteValue(pData, 123);
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			//PrintfBody(buf.data(), len);
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}

	}
	//领取
	if (uintValue3 == 3) {
		std::vector<ItemInfo> Items;
		ItemInfo IT;
		//第一份会有说明写死长度
		Body += 41;
		IT.AvailPeriod = static_cast<UINT>(ReadValue(Body));
		Body += 15;
		IT.ItemType = static_cast<UINT>(ReadValue(Body));
		Body += 8;
		IT.ItemID = static_cast<UINT>(ReadValue(Body));
		Body += 9;
		IT.ItemNum = static_cast<UINT>(ReadValue(Body));
		Items.push_back(IT);

		int index = 2;
		while (static_cast<UINT>(ReadValue(Body)) == index) {
			ItemInfo IT;
			Body += 7;
			IT.AvailPeriod = static_cast<UINT>(ReadValue(Body));
			Body += 2;
			IT.ItemType = static_cast<UINT>(ReadValue(Body));
			Body += 2;
			IT.ItemID = static_cast<UINT>(ReadValue(Body));
			Body += 2;
			IT.ItemNum = static_cast<UINT>(ReadValue(Body));
			Items.push_back(IT);
			index++;
		}


		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len

			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
				BYTE* pAwardOptInfo = pData;
				{
					WriteTextValue(pAwardOptInfo, "m_sItemNum", Items.size());
					WriteTextValue(pAwardOptInfo, "m_iSuperMoney", 0);
					WriteTextValue(pAwardOptInfo, "m_iCoupons", 0);
					WriteTextValue(pAwardOptInfo, "m_iSpeedCoin", 0);
					WriteText(pAwardOptInfo, "m_astItemInfo");


					ItemInfo Item = { 0 };
					Item.ItemID = Items[0].ItemID;
					Item.ItemNum = Items[0].ItemNum;
					Item.AvailPeriod = Items[0].AvailPeriod;
					if (Item.AvailPeriod > 0) {
						Item.AvailPeriod = Item.AvailPeriod * Item.ItemNum;
					}
					Updatemechskinaward(Client->Uin, "GetNum", Items[0].ItemID, Items[0].ItemNum, Items[0].AvailPeriod);
					Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);
					Item.AvailPeriod = PlayerDB_GetItemAvailPeriod(Client->Uin, Items[0].ItemID);
					NotifyClientAddItemBox(Client, 1, { Item });
					BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pAwardOptInfo);
					BYTE* pAwardOptInfo2 = pAwardOptInfo;
					WriteValue(pAwardOptInfo2, 1);
					{
						BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
						BYTE* pAwardOptInfo3 = pAwardOptInfo2;
						{
							WriteTextValue(pAwardOptInfo3, "m_iAvailPeriod", Items[0].AvailPeriod);
							WriteTextValue(pAwardOptInfo3, "m_ushItemType", Items[0].ItemType);
							WriteTextValue(pAwardOptInfo3, "m_iItemID", Items[0].ItemID);
							WriteTextValue(pAwardOptInfo3, "m_iItemNum", Items[0].ItemNum);
							WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
						}
					}
					for (size_t i = 1; i < Items.size(); i++)
					{
						ItemInfo Item = { 0 };
						Item.ItemID = Items[i].ItemID;
						Item.ItemNum = Items[i].ItemNum;
						Item.AvailPeriod = Items[i].AvailPeriod;
						if (Item.AvailPeriod > 0) {
							Item.AvailPeriod = Item.AvailPeriod * Item.ItemNum;
						}
						Updatemechskinaward(Client->Uin, "GetNum", Items[i].ItemID, Items[i].ItemNum, Items[i].AvailPeriod);
						Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);
						Item.AvailPeriod = PlayerDB_GetItemAvailPeriod(Client->Uin, Items[i].ItemID);
						NotifyClientAddItemBox(Client, 1, { Item });

						WriteValue(pAwardOptInfo2, i + 1);
						{
							BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
							BYTE* pAwardOptInfo3 = pAwardOptInfo2;
							{
								WriteKeyValue(pAwardOptInfo3, 5, Items[i].AvailPeriod);
								WriteKeyValue(pAwardOptInfo3, 6, Items[i].ItemType);
								WriteKeyValue(pAwardOptInfo3, 7, Items[i].ItemID);
								WriteKeyValue(pAwardOptInfo3, 8, Items[i].ItemNum);
								WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
							}
						}
					}
					WriteLenValueEnd(pAwardOptInfo, pAwardOptInfo2, pAwardOptInfoLen2);
				}
				WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);

				WriteBodyHeadEnd(pBody, pData);
			}

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();

		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	//分解
	if (uintValue3 == 4) {
		std::vector<ItemInfo> Items;
		ItemInfo IT;
		//第一份会有说明写死长度
		Body += 41;
		IT.AvailPeriod = static_cast<UINT>(ReadValue(Body));
		Body += 15;
		IT.ItemType = static_cast<UINT>(ReadValue(Body));
		Body += 8;
		IT.ItemID = static_cast<UINT>(ReadValue(Body));
		Body += 9;
		IT.ItemNum = static_cast<UINT>(ReadValue(Body));
		Items.push_back(IT);
		int index = 2;
		while (static_cast<UINT>(ReadValue(Body)) == index) {
			ItemInfo IT;
			Body += 7;
			IT.AvailPeriod = static_cast<UINT>(ReadValue(Body));
			Body += 2;
			IT.ItemType = static_cast<UINT>(ReadValue(Body));
			Body += 2;
			IT.ItemID = static_cast<UINT>(ReadValue(Body));
			Body += 2;
			IT.ItemNum = static_cast<UINT>(ReadValue(Body));
			Items.push_back(IT);
			index++;
		}


		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len

			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
				BYTE* pAwardOptInfo = pData;
				{
					WriteTextValue(pAwardOptInfo, "m_sItemNum", Items.size());
					WriteTextValue(pAwardOptInfo, "m_iSuperMoney", 0);
					WriteTextValue(pAwardOptInfo, "m_iCoupons", 0);
					WriteTextValue(pAwardOptInfo, "m_iSpeedCoin", 0);
					WriteText(pAwardOptInfo, "m_astItemInfo");

					int exchangePieceNum = 0;
					for (size_t i = 0; i < Items.size(); i++)
					{
						auto result = findMechTreasureInfo(mechTreasureInfo, Items[i].ItemID, Items[i].AvailPeriod);
						if (result) {
							exchangePieceNum += result->exchangePieceNum * Items[i].ItemNum;
						}
					}

					Updatemechskinaward(Client->Uin, "DecomposeNum", Items[0].ItemID, Items[0].ItemNum, Items[0].AvailPeriod);
					auto result = findMechTreasureInfo(mechTreasureInfo, Items[0].ItemID, Items[0].AvailPeriod);
					ItemInfo Item = { 0 };
					Item.ItemID = 138387;

					if (result) {
						Item.ItemNum = result->exchangePieceNum * Items[0].ItemNum;
					}
					Item.AvailPeriod = 0;
					Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);
					NotifyClientAddItemBox(Client, 1, { Item });

					BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pAwardOptInfo);
					BYTE* pAwardOptInfo2 = pAwardOptInfo;
					WriteValue(pAwardOptInfo2, 1);
					{
						BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
						BYTE* pAwardOptInfo3 = pAwardOptInfo2;
						{
							WriteTextValue(pAwardOptInfo3, "m_iAvailPeriod", Items[0].AvailPeriod);
							WriteTextValue(pAwardOptInfo3, "m_ushItemType", Items[0].ItemType);
							WriteTextValue(pAwardOptInfo3, "m_iItemID", Items[0].ItemID);
							WriteTextValue(pAwardOptInfo3, "m_iItemNum", exchangePieceNum);
							WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
						}
					}

					for (size_t i = 1; i < Items.size(); i++)
					{
						Updatemechskinaward(Client->Uin, "DecomposeNum", Items[i].ItemID, Items[i].ItemNum, Items[i].AvailPeriod);
						auto result = findMechTreasureInfo(mechTreasureInfo, Items[i].ItemID, Items[i].AvailPeriod);
						ItemInfo Item = { 0 };
						Item.ItemID = 138387;
						int exchangePieceNum = 1;
						if (result) {
							exchangePieceNum = result->exchangePieceNum;
							Item.ItemNum = result->exchangePieceNum * Items[i].ItemNum;
						}
						Item.AvailPeriod = 0;
						Item.ItemNum = PlayerDB_AddItem2(Client, Client->Uin, Item.ItemID, Item.ItemNum, Item.AvailPeriod, 0);
						NotifyClientAddItemBox(Client, 1, { Item });

						WriteValue(pAwardOptInfo2, i + 1);
						{
							BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
							BYTE* pAwardOptInfo3 = pAwardOptInfo2;
							{
								WriteKeyValue(pAwardOptInfo3, 5, Items[i].AvailPeriod);
								WriteKeyValue(pAwardOptInfo3, 6, Items[i].ItemType);
								WriteKeyValue(pAwardOptInfo3, 7, Items[i].ItemID);
								WriteKeyValue(pAwardOptInfo3, 8, Items[i].ItemNum * exchangePieceNum);
								WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
							}
						}
					}
					WriteLenValueEnd(pAwardOptInfo, pAwardOptInfo2, pAwardOptInfoLen2);

				}
				WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);
				WriteBodyHeadEnd(pBody, pData);
			}

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();

		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	//兑换商店界面
	if (uintValue3 == 5)
	{
		if (mechExchangeTreasureInfo.size() == 0) return;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len

			{
				BYTE* pData = WriteBodyHeadAb(pBody, readValue1);

				BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
				BYTE* pAwardOptInfo = pData;

				WriteValue(pAwardOptInfo, 1);
				WriteHex(pAwardOptInfo, "FD000000D3");
				WriteTextValue(pAwardOptInfo, "availperiod", mechExchangeTreasureInfo[0].AvailPeriod);
				WriteTextValue(pAwardOptInfo, "errid", mechExchangeTreasureInfo[0].errid);
				WriteTextValue(pAwardOptInfo, "exchangeNeedNum", mechExchangeTreasureInfo[0].exchangeNeedNum);
				WriteTextValue(pAwardOptInfo, "maxGetNum", mechExchangeTreasureInfo[0].maxGetNum);
				WriteTextValue(pAwardOptInfo, "award_idx", mechExchangeTreasureInfo[0].award_idx);
				WriteTextValue(pAwardOptInfo, "itemnum", mechExchangeTreasureInfo[0].ItemNum);
				WriteTextValue(pAwardOptInfo, "item_type", mechExchangeTreasureInfo[0].item_type);
				WriteTextValue(pAwardOptInfo, "status", mechExchangeTreasureInfo[0].status);
				WriteTextValue(pAwardOptInfo, "getnum", SelectmechskinawardGetNum(Client->Uin, mechExchangeTreasureInfo[0].ItemId, mechExchangeTreasureInfo[0].AvailPeriod));
				WriteText(pAwardOptInfo, "genderitem");
				WriteHex(pAwardOptInfo, "FD00000012");
				WriteValue(pAwardOptInfo, mechExchangeTreasureInfo[0].award_idx);
				WriteValue(pAwardOptInfo, mechExchangeTreasureInfo[0].ItemId);

				for (size_t i = 1; i < mechExchangeTreasureInfo.size(); i++)
				{
					WriteValue(pAwardOptInfo, mechExchangeTreasureInfo[i].award_idx);
					WriteHex(pAwardOptInfo, "FD0000007C");
					WriteKeyValue(pAwardOptInfo, 0, mechExchangeTreasureInfo[i].AvailPeriod);//availperiod
					WriteKeyValue(pAwardOptInfo, 1, mechExchangeTreasureInfo[i].errid);//errid
					WriteKeyValue(pAwardOptInfo, 2, mechExchangeTreasureInfo[i].exchangeNeedNum);//exchangeNeedNum
					WriteKeyValue(pAwardOptInfo, 3, mechExchangeTreasureInfo[i].maxGetNum);//maxGetNum
					WriteKeyValue(pAwardOptInfo, 4, mechExchangeTreasureInfo[i].award_idx);//award_idx
					WriteKeyValue(pAwardOptInfo, 5, mechExchangeTreasureInfo[i].ItemNum);//itemnum
					WriteKeyValue(pAwardOptInfo, 6, mechExchangeTreasureInfo[i].item_type);//item_type
					WriteKeyValue(pAwardOptInfo, 7, mechExchangeTreasureInfo[i].status);//status
					WriteKeyValue(pAwardOptInfo, 8, SelectmechskinawardGetNum(Client->Uin, mechExchangeTreasureInfo[i].ItemId, mechExchangeTreasureInfo[i].AvailPeriod));//getnum
					WriteKeyIndex(pAwardOptInfo, 9);
					WriteHex(pAwardOptInfo, "FD00000012");
					WriteValue(pAwardOptInfo, 1);
					WriteValue(pAwardOptInfo, mechExchangeTreasureInfo[i].ItemId);//itemid
				}


				WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);

				WriteBodyHeadEnd(pBody, pData);
			}
			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	//暂存箱，已分解，已领取
	if (uintValue3 == 6) {
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		std::vector<MechTreasureInfo> meInfos;
		Selectmechskinaward(Client->Uin, meInfos);
		for (size_t i = 0; i < meInfos.size(); i++)
		{
			auto result = findMechTreasureInfo(mechTreasureInfo, meInfos[i].ItemId, meInfos[i].AvailPeriod);
			if (result) {
				meInfos[i].exchangePieceNum = result->exchangePieceNum;
			}
		}
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len

			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
				BYTE* pAwardOptInfo = pData;
				if (meInfos.size() > 0) {
					WriteValue(pAwardOptInfo, 1);
					{

						BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pAwardOptInfo);
						BYTE* pAwardOptInfo2 = pAwardOptInfo;
						{
							WriteTextValue(pAwardOptInfo2, "ItemNum", meInfos[0].ItemNum);
							WriteTextValue(pAwardOptInfo2, "GetNum", meInfos[0].getnum);
							WriteTextValue(pAwardOptInfo2, "DecomposeNum", meInfos[0].DecomposeNum);
							WriteTextValue(pAwardOptInfo2, "ExchangePieceNum", meInfos[0].exchangePieceNum);
							WriteTextValue(pAwardOptInfo2, "Available", meInfos[0].AvailPeriod);
							WriteTextValue(pAwardOptInfo2, "ItemType", 0);
							WriteTextValue(pAwardOptInfo2, "ItemId1", meInfos[0].ItemId);
							WriteTextValue(pAwardOptInfo2, "ItemId", meInfos[0].ItemId);
							WriteLenValueEnd(pAwardOptInfo, pAwardOptInfo2, pAwardOptInfoLen2);
						}
					}
				}

				for (size_t i = 1; i < meInfos.size(); i++)
				{
					WriteValue(pAwardOptInfo, i + 1);
					{

						BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo);
						BYTE* pAwardOptInfo3 = pAwardOptInfo;
						{
							WriteKeyValue(pAwardOptInfo3, 0, meInfos[i].ItemNum);
							WriteKeyValue(pAwardOptInfo3, 1, meInfos[i].getnum);
							WriteKeyValue(pAwardOptInfo3, 2, meInfos[i].DecomposeNum);
							WriteKeyValue(pAwardOptInfo3, 3, meInfos[i].exchangePieceNum);
							WriteKeyValue(pAwardOptInfo3, 4, meInfos[i].AvailPeriod);
							WriteKeyValue(pAwardOptInfo3, 5, 0);
							WriteKeyValue(pAwardOptInfo3, 6, meInfos[i].ItemId);
							WriteKeyValue(pAwardOptInfo3, 7, meInfos[i].ItemId);
							WriteLenValueEnd(pAwardOptInfo, pAwardOptInfo3, pAwardOptInfoLen3);
						}
					}
				}
				WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);
				WriteBodyHeadEnd(pBody, pData);
			}

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();
		//PrintfBody(buf.data(), len);
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
}

NOINLINE void Request_Beta78_ReqMechSkinInfo(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.3) return;
	Body += 28;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue8(Body);
	int type = Read8(Body);
	double readValue3 = ReadValue(Body);
	int uintValue1 = static_cast<UINT>(readValue1);
	int uintValue2 = static_cast<UINT>(readValue2);
	int uintValue3 = static_cast<UINT>(readValue3);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//召唤一次
	if (type == 252) {
		{
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHead(pBody);
					WriteValue(pData, 1);
					WriteValue(pData, 138385);
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			//PrintfBody(buf.data(), len);
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

		}
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len

			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
				BYTE* pAwardOptInfo = pData;
				{
					WriteTextValue(pAwardOptInfo, "iCostItemID", 138385);
					WriteTextValue(pAwardOptInfo, "GenMaxLevUpItemMaxSkinLev", 4);
					WriteTextValue(pAwardOptInfo, "LevUpPieceNum", PlayerDB_GetItemNum(Client->Uin, 138388));
					WriteTextValue(pAwardOptInfo, "MaxLevUpItemID", 138389);
					WriteTextValue(pAwardOptInfo, "iCloseTime", 235959);
					WriteTextValue(pAwardOptInfo, "CostItemNum", PlayerDB_GetItemNum(Client->Uin, 138385));
					WriteTextValue(pAwardOptInfo, "iOpenTime", 0);
					WriteTextValue(pAwardOptInfo, "LevUpPieceID", 138388);
					WriteTextValue(pAwardOptInfo, "GetMaxLevItem", 0);
					WriteTextValue(pAwardOptInfo, "ExchangePieceNum", 0);
					WriteTextValue(pAwardOptInfo, "iOpenDate", 20241013);
					WriteTextValue(pAwardOptInfo, "iCostNum10Time", 10);
					WriteTextValue(pAwardOptInfo, "iCostNumOneTime", 1);
					WriteTextValue(pAwardOptInfo, "ExchangePieceID", 138387);
					WriteTextValue(pAwardOptInfo, "iCloseDate", 20241111);
					WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);
				}


				BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pData);
				BYTE* pAwardOptInfo2 = pData;
				WriteValue(pAwardOptInfo2, 1);
				{
					BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
					BYTE* pAwardOptInfo3 = pAwardOptInfo2;
					{
						WriteTextValue(pAwardOptInfo3, "availperiod", 0);
						WriteTextValue(pAwardOptInfo3, "exchangeNeedNum", 0);
						WriteTextValue(pAwardOptInfo3, "alreadyGot", 0);
						WriteTextValue(pAwardOptInfo3, "itemnum", 1);
						WriteTextValue(pAwardOptInfo3, "award_idx", 1);
						WriteTextValue(pAwardOptInfo3, "exchangePieceNum", 500);
						WriteTextValue(pAwardOptInfo3, "item_type", 1);
						WriteTextValue(pAwardOptInfo3, "coupons", 0);
						WriteTextValue(pAwardOptInfo3, "supermoney", 0);
						WriteTextValue(pAwardOptInfo3, "itemid", 138389);
						WriteTextValue(pAwardOptInfo3, "merge", 0);
						WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
					}
				}

				for (size_t i = 0; i < mechTreasureInfo.size(); i++)
				{
					WriteValue(pAwardOptInfo2, mechTreasureInfo[i].award_idx - 1);

					{
						BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
						BYTE* pAwardOptInfo3 = pAwardOptInfo2;
						{
							WriteKeyValue(pAwardOptInfo3, 15, mechTreasureInfo[i].AvailPeriod);//availperiod
							WriteKeyValue(pAwardOptInfo3, 16, mechTreasureInfo[i].exchangeNeedNum);//exchangeNeedNum
							WriteKeyValue(pAwardOptInfo3, 17, mechTreasureInfo[i].alreadyGot);//alreadyGot
							WriteKeyValue(pAwardOptInfo3, 18, mechTreasureInfo[i].ItemNum);//itemnum
							WriteKeyValue(pAwardOptInfo3, 19, mechTreasureInfo[i].award_idx - 1);//award_idx
							WriteKeyValue(pAwardOptInfo3, 20, mechTreasureInfo[i].exchangePieceNum);//exchangePieceNum
							WriteKeyValue(pAwardOptInfo3, 21, mechTreasureInfo[i].item_type);//item_type
							WriteKeyValue(pAwardOptInfo3, 22, mechTreasureInfo[i].coupons);//coupons
							WriteKeyValue(pAwardOptInfo3, 23, mechTreasureInfo[i].supermoney);//supermoney
							WriteKeyValue(pAwardOptInfo3, 24, mechTreasureInfo[i].ItemId);//itemid
							WriteKeyValue(pAwardOptInfo3, 25, mechTreasureInfo[i].merge);//merge
							WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
						}
					}
				}
				WriteLenValueEnd(pData, pAwardOptInfo2, pAwardOptInfoLen2);

				WriteBodyHeadEnd(pBody, pData);
			}

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();
		//PrintfBody(buf.data(), len);
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

	}
	else {
		//显示界面
		if (uintValue3 == 1) {
			{
				std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
				//Head
				WriteRequestHead(p, Client->Uin);
				{ //Body
					BYTE* pBody = p;
					Write16(pBody, 0); //len

					{
						BYTE* pData = WriteBodyHead(pBody);
						WriteValue(pData, 1);
						WriteValue(pData, 138385);
						WriteBodyHeadEnd(pBody, pData);
					}

					len = pBody - p;
					Set16(p, (WORD)len);
					p += len;
				}

				len = p - buf.data();
				//PrintfBody(buf.data(), len);
				SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

			}
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHead(pBody, readValue1);
					BYTE* pAwardOptInfoLen = WriteLenValueStart(pData);
					BYTE* pAwardOptInfo = pData;
					{
						WriteTextValue(pAwardOptInfo, "iCostItemID", 138385);
						WriteTextValue(pAwardOptInfo, "GenMaxLevUpItemMaxSkinLev", 4);
						WriteTextValue(pAwardOptInfo, "LevUpPieceNum", 0);
						WriteTextValue(pAwardOptInfo, "MaxLevUpItemID", 138389);
						WriteTextValue(pAwardOptInfo, "iCloseTime", 235959);
						WriteTextValue(pAwardOptInfo, "CostItemNum", PlayerDB_GetItemNum(Client->Uin, 138385));
						WriteTextValue(pAwardOptInfo, "iOpenTime", 0);
						WriteTextValue(pAwardOptInfo, "LevUpPieceID", 138388);
						WriteTextValue(pAwardOptInfo, "GetMaxLevItem", 0);
						WriteTextValue(pAwardOptInfo, "ExchangePieceNum", 0);
						WriteTextValue(pAwardOptInfo, "iOpenDate", 20241013);
						WriteTextValue(pAwardOptInfo, "iCostNum10Time", 10);
						WriteTextValue(pAwardOptInfo, "iCostNumOneTime", 1);
						WriteTextValue(pAwardOptInfo, "ExchangePieceID", 138387);
						WriteTextValue(pAwardOptInfo, "iCloseDate", 20241111);
						WriteLenValueEnd(pData, pAwardOptInfo, pAwardOptInfoLen);
					}

					BYTE* pAwardOptInfoLen2 = WriteLenValueStart(pData);
					BYTE* pAwardOptInfo2 = pData;
					WriteValue(pAwardOptInfo2, 1);
					{
						BYTE* pAwardOptInfoLen3 = WriteLenValueStart(pAwardOptInfo2);
						BYTE* pAwardOptInfo3 = pAwardOptInfo2;
						{
							WriteTextValue(pAwardOptInfo3, "availperiod", 0);
							WriteTextValue(pAwardOptInfo3, "exchangeNeedNum", 0);
							WriteTextValue(pAwardOptInfo3, "alreadyGot", 0);
							WriteTextValue(pAwardOptInfo3, "itemnum", 1);
							WriteTextValue(pAwardOptInfo3, "award_idx", 1);
							WriteTextValue(pAwardOptInfo3, "exchangePieceNum", 500);
							WriteTextValue(pAwardOptInfo3, "item_type", 1);
							WriteTextValue(pAwardOptInfo3, "coupons", 0);
							WriteTextValue(pAwardOptInfo3, "supermoney", 0);
							WriteTextValue(pAwardOptInfo3, "itemid", 138389);
							WriteTextValue(pAwardOptInfo3, "merge", 0);
							WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo3, pAwardOptInfoLen3);
						}
					}

					for (size_t i = 0; i < mechTreasureInfo.size(); i++)
					{
						WriteValue(pAwardOptInfo2, mechTreasureInfo[i].award_idx);

						{
							BYTE* pAwardOptInfoLen = WriteLenValueStart(pAwardOptInfo2);
							BYTE* pAwardOptInfo = pAwardOptInfo2;
							{
								WriteKeyValue(pAwardOptInfo, 15, mechTreasureInfo[i].AvailPeriod);//availperiod
								WriteKeyValue(pAwardOptInfo, 16, mechTreasureInfo[i].exchangeNeedNum);//exchangeNeedNum
								WriteKeyValue(pAwardOptInfo, 17, mechTreasureInfo[i].alreadyGot);//alreadyGot
								WriteKeyValue(pAwardOptInfo, 18, mechTreasureInfo[i].ItemNum);//itemnum
								WriteKeyValue(pAwardOptInfo, 19, mechTreasureInfo[i].award_idx);//award_idx
								WriteKeyValue(pAwardOptInfo, 20, mechTreasureInfo[i].exchangePieceNum);//exchangePieceNum
								WriteKeyValue(pAwardOptInfo, 21, mechTreasureInfo[i].item_type);//item_type
								WriteKeyValue(pAwardOptInfo, 22, mechTreasureInfo[i].coupons);//coupons
								WriteKeyValue(pAwardOptInfo, 23, mechTreasureInfo[i].supermoney);//supermoney
								WriteKeyValue(pAwardOptInfo, 24, mechTreasureInfo[i].ItemId);//itemid
								WriteKeyValue(pAwardOptInfo, 25, mechTreasureInfo[i].merge);//merge
								WriteLenValueEnd(pAwardOptInfo2, pAwardOptInfo, pAwardOptInfoLen);
							}
						}
					}
					WriteLenValueEnd(pData, pAwardOptInfo2, pAwardOptInfoLen2);
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			//PrintfBody(buf.data(), len);
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

		}
		else
		{
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len

				{
					BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
					WriteValue(pData, 12376);
					WriteBodyHeadEnd(pBody, pData);
				}

				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}

			len = p - buf.data();
			//PrintfBody(buf.data(), len);
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}
	}
}

NOINLINE void RequestMapChallenge_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{


	Body += 30;
	double readValue1 = ReadValue(Body);

	if (mapChallengeOpenMain.size() != 1) return;
	if (mapChallengeOpenMain[0].Maps.size() == 0) return;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHead(pBody, readValue1);
			BYTE* pMapChallengeLen = WriteLenValueStart(pData);
			BYTE* pMapChallenge = pData;
			WriteTextValue(pMapChallenge, "CloseTime", 1730044799);
			WriteText(pMapChallenge, "HasMapRecordData");
			WriteHex(pMapChallenge, "F8");//F8
			WriteText(pMapChallenge, "mapinfo");
			{
				BYTE* pMapChallengeLen2 = WriteLenValueStart(pMapChallenge);
				BYTE* pMapChallenge2 = pMapChallenge;
				WriteValue(pMapChallenge2, 1);
				{
					BYTE* pMapChallengeLen3 = WriteLenValueStart(pMapChallenge2);
					BYTE* pMapChallenge3 = pMapChallenge2;
					WriteTextValue(pMapChallenge3, "MapID", mapChallengeOpenMain[0].Maps[0].Mapid);
					WriteTextValue(pMapChallenge3, "MapRecordTime", PlayerDB_GetMapRecord(Client->Uin, mapChallengeOpenMain[0].Maps[0].Mapid, Client));
					WriteText(pMapChallenge3, "AssessInfo");
					{
						BYTE* pMapChallengeLen4 = WriteLenValueStart(pMapChallenge3);
						BYTE* pMapChallenge4 = pMapChallenge3;
						WriteTextValue(pMapChallenge4, "Num", mapChallengeOpenMain[0].Maps[0].Num);
						WriteTextValue(pMapChallenge4, "AssessType", 23);
						WriteText(pMapChallenge4, "Msg");
						WriteText(pMapChallenge4, mapChallengeOpenMain[0].msg.c_str());
						WriteText(pMapChallenge4, "MsgGame");
						WriteText(pMapChallenge4, "使用WCW喷{tc}次：");
						WriteTextValue(pMapChallenge4, "isfinish", 0);
						WriteLenValueEnd(pMapChallenge3, pMapChallenge4, pMapChallengeLen4);
					}
					WriteText(pMapChallenge3, "StarInfo");
					{
						BYTE* pMapChallengeLen4 = WriteLenValueStart(pMapChallenge3);
						BYTE* pMapChallenge4 = pMapChallenge3;
						WriteValue(pMapChallenge4, 1);
						WriteHex(pMapChallenge4, "FD0000001e");//固定长度
						WriteKeyValue(pMapChallenge4, 12, 0);
						WriteTextValue(pMapChallenge4, "basetime", mapChallengeOpenMain[0].Maps[0].firsttime);

						WriteValue(pMapChallenge4, 2);
						WriteHex(pMapChallenge4, "FD00000016");//固定长度
						WriteKeyValue(pMapChallenge4, 12, 0);
						WriteKeyValue(pMapChallenge4, 14, mapChallengeOpenMain[0].Maps[0].secondtime);

						WriteValue(pMapChallenge4, 3);
						WriteHex(pMapChallenge4, "FD00000016");//固定长度
						WriteKeyValue(pMapChallenge4, 12, 0);
						WriteKeyValue(pMapChallenge4, 14, mapChallengeOpenMain[0].Maps[0].thirdtime);
						WriteLenValueEnd(pMapChallenge3, pMapChallenge4, pMapChallengeLen4);
					}
					WriteLenValueEnd(pMapChallenge2, pMapChallenge3, pMapChallengeLen3);
				}
				for (size_t i = 1; i < mapChallengeOpenMain[0].Maps.size(); i++)
				{
					WriteValue(pMapChallenge2, i + 1);
					{
						BYTE* pMapChallengeLen3 = WriteLenValueStart(pMapChallenge2);
						BYTE* pMapChallenge3 = pMapChallenge2;
						WriteKeyValue(pMapChallenge3, 3, mapChallengeOpenMain[0].Maps[i].Mapid);
						WriteKeyValue(pMapChallenge3, 4, 136911);
						WriteKeyIndex(pMapChallenge3, 5);
						{
							BYTE* pMapChallengeLen4 = WriteLenValueStart(pMapChallenge3);
							BYTE* pMapChallenge4 = pMapChallenge3;
							WriteKeyValue(pMapChallenge4, 6, mapChallengeOpenMain[0].Maps[i].Num);
							WriteKeyValue(pMapChallenge4, 7, 23);
							WriteKeyIndex(pMapChallenge4, 8);
							WriteKeyIndex(pMapChallenge4, 9);
							WriteKeyIndex(pMapChallenge4, 10);
							WriteKeyIndex(pMapChallenge4, 11);
							WriteKeyValue(pMapChallenge4, 12, 0);
							WriteLenValueEnd(pMapChallenge3, pMapChallenge4, pMapChallengeLen4);
						}
						WriteKeyIndex(pMapChallenge3, 13);
						{
							BYTE* pMapChallengeLen4 = WriteLenValueStart(pMapChallenge3);
							BYTE* pMapChallenge4 = pMapChallenge3;
							WriteValue(pMapChallenge4, 1);
							WriteHex(pMapChallenge4, "FD00000016");//固定长度
							WriteKeyValue(pMapChallenge4, 12, 0);
							WriteKeyValue(pMapChallenge4, 14, mapChallengeOpenMain[0].Maps[i].firsttime);

							WriteValue(pMapChallenge4, 2);
							WriteHex(pMapChallenge4, "FD00000016");//固定长度
							WriteKeyValue(pMapChallenge4, 12, 0);
							WriteKeyValue(pMapChallenge4, 14, mapChallengeOpenMain[0].Maps[i].secondtime);

							WriteValue(pMapChallenge4, 3);
							WriteHex(pMapChallenge4, "FD00000016");//固定长度
							WriteKeyValue(pMapChallenge4, 12, 0);
							WriteKeyValue(pMapChallenge4, 14, mapChallengeOpenMain[0].Maps[i].thirdtime);
							WriteLenValueEnd(pMapChallenge3, pMapChallenge4, pMapChallengeLen4);
						}
						WriteLenValueEnd(pMapChallenge2, pMapChallenge3, pMapChallengeLen3);
					}
				}

				WriteText(pMapChallenge2, "RefreshLeftDays");
				WriteValue(pMapChallenge2, 5);
				WriteText(pMapChallenge2, "BaseCfg");
				WriteHex(pMapChallenge2, "FD00000014");//固定长度
				WriteText(pMapChallenge2, "ChannelID");
				WriteValue(pMapChallenge2, 3);
				WriteText(pMapChallenge2, "OpenTime");
				WriteValue(pMapChallenge2, 1729440000);
				WriteText(pMapChallenge2, "StarNums");
				WriteValue(pMapChallenge2, 0);
				WriteLenValueEnd(pMapChallenge, pMapChallenge2, pMapChallengeLen2);
			}
			WriteLenValueEnd(pData, pMapChallenge, pMapChallengeLen);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


std::optional<MapChallengeOpenMainMap> findMapById(const std::vector<MapChallengeOpenMainMap>& Maps, int targetMapId) {
	auto it = std::find_if(Maps.begin(), Maps.end(), [targetMapId](const MapChallengeOpenMainMap& map) {
		return map.Mapid == targetMapId;
	});

	if (it != Maps.end()) {
		return *it; // 返回找到的对象
	}

	return std::nullopt; // 未找到，返回空
}

NOINLINE void RequestMapChallenge_GetUserMapInfo(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version < 1.4) return;
	Body += 33;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue(Body);
	int uintValue2 = static_cast<UINT>(readValue2);
	if (mapChallengeOpenMain.size() != 1) return;
	if (mapChallengeOpenMain[0].Maps.size() == 0) return;
	auto result = findMapById(mapChallengeOpenMain[0].Maps, uintValue2);
	if (!result) {
		auto resultcreate = findMapById(mapChallengeCreate, uintValue2);
		if (resultcreate) {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len
				{
					BYTE* pData = WriteBodyHead(pBody, readValue1);
					BYTE* pMapChallengeLen = WriteLenValueStart(pData);
					BYTE* pMapChallenge = pData;

					WriteTextValue(pMapChallenge, "MapID", resultcreate->Mapid);
					WriteTextValue(pMapChallenge, "MapRecordTime", PlayerDB_GetMapRecord(Client->Uin, resultcreate->Mapid, Client));
					WriteText(pMapChallenge, "AssessInfo");
					{
						BYTE* pMapChallengeLen2 = WriteLenValueStart(pMapChallenge);
						BYTE* pMapChallenge2 = pMapChallenge;
						WriteTextValue(pMapChallenge2, "Num", resultcreate->Num);
						WriteTextValue(pMapChallenge2, "AssessType", 23);
						WriteText(pMapChallenge2, "Msg");
						WriteText(pMapChallenge2, resultcreate->msg.c_str());
						WriteText(pMapChallenge2, "MsgGame");
						WriteText(pMapChallenge2, "使用WCW喷{tc}次：");
						WriteTextValue(pMapChallenge2, "isfinish", 0);
						WriteLenValueEnd(pMapChallenge, pMapChallenge2, pMapChallengeLen2);
					}
					WriteTextValue(pMapChallenge, "ShowSmallIcon", 1);
					WriteText(pMapChallenge, "StarInfo");
					{
						BYTE* pMapChallengeLen3 = WriteLenValueStart(pMapChallenge);
						BYTE* pMapChallenge3 = pMapChallenge;
						WriteValue(pMapChallenge3, 1);
						WriteHex(pMapChallenge3, "FD0000001e");//固定长度
						WriteKeyValue(pMapChallenge3, 9, 0);
						WriteTextValue(pMapChallenge3, "basetime", resultcreate->firsttime);

						WriteValue(pMapChallenge3, 2);
						WriteHex(pMapChallenge3, "FD00000016");//固定长度
						WriteKeyValue(pMapChallenge3, 9, 0);
						WriteKeyValue(pMapChallenge3, 12, resultcreate->secondtime);

						WriteValue(pMapChallenge3, 3);
						WriteHex(pMapChallenge3, "FD00000016");//固定长度
						WriteKeyValue(pMapChallenge3, 9, 0);
						WriteKeyValue(pMapChallenge3, 12, resultcreate->thirdtime);
						WriteLenValueEnd(pMapChallenge, pMapChallenge3, pMapChallengeLen3);
					}
					WriteLenValueEnd(pData, pMapChallenge, pMapChallengeLen);
					WriteBodyHeadEnd(pBody, pData);
				}
				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}
			len = p - buf.data();
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}
		else {
			std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
			//Head
			WriteRequestHead(p, Client->Uin);
			{ //Body
				BYTE* pBody = p;
				Write16(pBody, 0); //len
				{
					BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
					WriteValue(pData, 12523);
					WriteHex(pData, "FD00000018");//固定长度
					WriteText(pData, "ShowSmallIcon");
					WriteValue(pData, 1);
					WriteBodyHeadEnd(pBody, pData);
				}
				len = pBody - p;
				Set16(p, (WORD)len);
				p += len;
			}
			len = p - buf.data();
			SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		}

	}
	else {
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len
			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				BYTE* pMapChallengeLen = WriteLenValueStart(pData);
				BYTE* pMapChallenge = pData;

				WriteTextValue(pMapChallenge, "MapID", result->Mapid);
				WriteTextValue(pMapChallenge, "MapRecordTime", PlayerDB_GetMapRecord(Client->Uin, result->Mapid, Client));
				WriteText(pMapChallenge, "AssessInfo");
				{
					BYTE* pMapChallengeLen2 = WriteLenValueStart(pMapChallenge);
					BYTE* pMapChallenge2 = pMapChallenge;
					WriteTextValue(pMapChallenge2, "Num", result->Num);
					WriteTextValue(pMapChallenge2, "AssessType", 23);
					WriteText(pMapChallenge2, "Msg");
					WriteText(pMapChallenge2, mapChallengeOpenMain[0].msg.c_str());
					WriteText(pMapChallenge2, "MsgGame");
					WriteText(pMapChallenge2, "使用WCW喷{tc}次：");
					WriteTextValue(pMapChallenge2, "isfinish", 0);
					WriteLenValueEnd(pMapChallenge, pMapChallenge2, pMapChallengeLen2);
				}
				WriteTextValue(pMapChallenge, "ShowSmallIcon", 1);
				WriteText(pMapChallenge, "StarInfo");
				{
					BYTE* pMapChallengeLen3 = WriteLenValueStart(pMapChallenge);
					BYTE* pMapChallenge3 = pMapChallenge;
					WriteValue(pMapChallenge3, 1);
					WriteHex(pMapChallenge3, "FD0000001e");//固定长度
					WriteKeyValue(pMapChallenge3, 9, 0);
					WriteTextValue(pMapChallenge3, "basetime", result->firsttime);

					WriteValue(pMapChallenge3, 2);
					WriteHex(pMapChallenge3, "FD00000016");//固定长度
					WriteKeyValue(pMapChallenge3, 9, 0);
					WriteKeyValue(pMapChallenge3, 12, result->secondtime);

					WriteValue(pMapChallenge3, 3);
					WriteHex(pMapChallenge3, "FD00000016");//固定长度
					WriteKeyValue(pMapChallenge3, 9, 0);
					WriteKeyValue(pMapChallenge3, 12, result->thirdtime);
					WriteLenValueEnd(pMapChallenge, pMapChallenge3, pMapChallengeLen3);
				}
				WriteLenValueEnd(pData, pMapChallenge, pMapChallengeLen);
				WriteBodyHeadEnd(pBody, pData);
			}
			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}
		len = p - buf.data();
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}

}

NOINLINE void Request_Beta87_ReqGetQQUrlHead(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.3) return;
	Body += 28;
	double readValue = ReadValue(Body);
	double intValue = 0;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHead(pBody, readValue);
			{
				BYTE* pQQHeadUrlInfoLen = WriteLenValueStart(pData);
				BYTE* pQQHeadUrlInfo = pData;

				WriteTextValue(pQQHeadUrlInfo, "Uin", Client->Uin);
				WriteTextValue(pQQHeadUrlInfo, "FaceFlag", 60);
				WriteTextValue(pQQHeadUrlInfo, "SysID", 0);
				WriteTextValue(pQQHeadUrlInfo, "FaceType", 1);
				WriteText(pQQHeadUrlInfo, "URL");
				WriteText(pQQHeadUrlInfo, Client->QQHeadUrl, 105);
				//WriteText(pQQHeadUrlInfo, "http://thirdqq.qlogo.cn/g?b=oidb&k=xmnpMxy0IfZQAia7cpTz8sQ&kti=Zuv-egv7oIE&s=", 77);
				//WriteText(pQQHeadUrlInfo, "https://q.qlogo.cn/headimg_dl?dst_uin=1728510546&spec=640&img_type=jpg", 70);
				WriteTextValue(pQQHeadUrlInfo, "TimeStamp", (DWORD)time(nullptr));

				WriteLenValueEnd(pData, pQQHeadUrlInfo, pQQHeadUrlInfoLen);
			}
			WriteBodyHeadEnd(pBody, pData);
		}

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta87_ReqSelfHeadAndHeadShokInfo(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 39;
	double readValue = ReadValue(Body);
	double intValue = 0;
	bool isQQhead = Client->UserHeads == 0 ? true : false;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHead(pBody, readValue);
			{//STHandInfo				
				BYTE* pSTHandInfoLen = WriteLenValueStart(pData);
				BYTE* pSTHandInfo = pData;

				WriteTextValue(pSTHandInfo, "Uin", Client->Uin);
				WriteText(pSTHandInfo, "ActiveHeadIdList", 16);
				WriteLenValue(pSTHandInfo, 0);
				WriteText(pSTHandInfo, "EquipBackPlateId", 16);
				{
					BYTE* pEquipBackPlateInfolen = WriteLenValueStart(pSTHandInfo);
					BYTE* pEquipBackPlateInfo = pSTHandInfo;

					WriteTextValue(pEquipBackPlateInfo, "Id", Client->UserFloor);
					WriteTextValue(pEquipBackPlateInfo, "LastActiveTime", -1);
					WriteTextValue(pEquipBackPlateInfo, "ExpirationTime", -1);

					WriteLenValueEnd(pSTHandInfo, pEquipBackPlateInfo, pEquipBackPlateInfolen);
				}

				if (isQQhead == true) {
					WriteText(pSTHandInfo, "QQHeadUrlInfo", 13);
					BYTE* pQQHeadUrlInfoLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pQQHeadUrlInfo = pSTHandInfo;
					{
						WriteKeyValue(pQQHeadUrlInfo, 0, Client->Uin);
						WriteTextValue(pQQHeadUrlInfo, "FaceFlag", 60);
						WriteTextValue(pQQHeadUrlInfo, "SysID", 0);
						WriteTextValue(pQQHeadUrlInfo, "FaceType", 1);
						WriteText(pQQHeadUrlInfo, "URL");
						WriteText(pQQHeadUrlInfo, Client->QQHeadUrl, 105);
						//WriteText(pQQHeadUrlInfo, "http://thirdqq.qlogo.cn/g?b=oidb&k=xmnpMxy0IfZQAia7cpTz8sQ&kti=Zuv-egv7oIE&s=", 77);
						//WriteText(pQQHeadUrlInfo, "https://q.qlogo.cn/headimg_dl?dst_uin=1728510546&spec=640&img_type=jpg", 70);
						WriteTextValue(pQQHeadUrlInfo, "TimeStamp", (DWORD)time(nullptr));
					}

					WriteLenValueEnd(pSTHandInfo, pQQHeadUrlInfo, pQQHeadUrlInfoLen);
					WriteTextValue(pSTHandInfo, "UseQQHead", 1);
				}
				else {
					WriteText(pSTHandInfo, "QQHeadUrlInfo");
					WriteLenValue(pSTHandInfo, 0);
					WriteTextValue(pSTHandInfo, "UseQQHead", 0);
				}
				WriteText(pSTHandInfo, "EquipHeadId");
				{//EquipHeadInfo 装备头像信息
					BYTE* pEquipHeadInfolen = WriteLenValueStart(pSTHandInfo);
					BYTE* pEquipHeadInfo = pSTHandInfo;

					WriteKeyValue(pEquipHeadInfo, 3, Client->UserHeads);
					WriteKeyValue(pEquipHeadInfo, 4, -1);
					WriteKeyValue(pEquipHeadInfo, 5, -1);
					WriteTextValue(pEquipHeadInfo, "QQHeadLegallyTime", 0);
					WriteLenValueEnd(pSTHandInfo, pEquipHeadInfo, pEquipHeadInfolen);
				}
				WriteText(pSTHandInfo, "EquipHeadShokId");
				{//EquipHeadShokIdInfo	装备头像框信息
					BYTE* pEquipHeadShokIdInfoLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pEquipHeadShokIdInfo = pSTHandInfo;

					WriteKeyValue(pEquipHeadShokIdInfo, 3, Client->UserHeadsBox);
					WriteKeyValue(pEquipHeadShokIdInfo, 4, -1);
					WriteKeyValue(pEquipHeadShokIdInfo, 5, -1);
					WriteLenValueEnd(pSTHandInfo, pEquipHeadShokIdInfo, pEquipHeadShokIdInfoLen);

				}
				WriteText(pSTHandInfo, "ActiveHeadShokIdList");
				{//ActiveHeadShokIdListInfo 拥有的头像框列表信息
					BYTE* pActiveHeadShokIdListInfoLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pHeadShokIdListInfo = pSTHandInfo;

					constexpr unsigned int ActiveHeadShokIdList[] = {
						2,//16周年头像框
						4,//涂抹大乱斗头像框
						5,//巅峰赛头像框
						6,//龙之传人头像框
						7,//青龙占星头像框
						8,//烛龙弄焰头像框
						9,//冰夷踏浪头像框
						10,//应龙驭风头像框
						11,//极星木兰头像框
						12,//雷诺之力头像框
						13,//四君子头像框
						14,//2023冠军头像框
						15,//2023亚军头像框
						16,//2023季军头像框
						17,//机械青龙王头像框
						18,//三海神龙头像框
						19,//龙腾九霄
						20,//罗小黑头像框
						21,//罗小黑头像框
						22,//罗小黑头像框
						23,//罗小黑头像框
						24,//孔明头像框
						25,//樱花头像框
						26,//小黄鸭头像框
						27,//雾山五行头像框
						28,//神奥丁头像框
						29,//北欧众神头像框
						30,//
						31,//大耳狗头像框
					};
					double HeadShokIndex = 1;
					constexpr unsigned int HeadShokNums = sizeof(ActiveHeadShokIdList) / sizeof(ActiveHeadShokIdList[0]); //

					for (size_t i = 0; i < HeadShokNums; i++) {
						//HeadShokIdInfo
						BYTE* pHeadShokIdInfo = pHeadShokIdListInfo;

						WriteValue(pHeadShokIdInfo, HeadShokIndex);

						{//HeadShokInfo
							BYTE* pHeadShokIdInfoLen = WriteLenValueStart(pHeadShokIdInfo);
							BYTE* pHeadShokInfo = pHeadShokIdInfo;
							WriteKeyValue(pHeadShokInfo, 3, ActiveHeadShokIdList[i]);
							WriteKeyValue(pHeadShokInfo, 4, 0);
							WriteKeyValue(pHeadShokInfo, 5, 0);

							WriteLenValueEnd(pHeadShokIdInfo, pHeadShokInfo, pHeadShokIdInfoLen);
						}

						len = pHeadShokIdInfo - pHeadShokIdListInfo;
						pHeadShokIdListInfo += len;
						++HeadShokIndex;
					}

					WriteLenValueEnd(pSTHandInfo, pHeadShokIdListInfo, pActiveHeadShokIdListInfoLen);

				}
				WriteText(pSTHandInfo, "ActiveBackPlateIdList");
				{//ActiveBackPlateIdListInfo 拥有的社交框信息
					BYTE* pActiveBackPlateListInfoLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pHeadShokIdListInfo = pSTHandInfo;

					constexpr unsigned int ActiveBackPlateIdList[] = {
						1,//默认面板
						2,//小樱面板
						3,
						4,
						5,
						6,
						7,
						8,
						9,
					};
					double HeadShokIndex = 1;
					constexpr unsigned int HeadShokNums = sizeof(ActiveBackPlateIdList) / sizeof(ActiveBackPlateIdList[0]); //

					for (size_t i = 0; i < HeadShokNums; i++) {
						//HeadShokIdInfo
						BYTE* pHeadShokIdInfo = pHeadShokIdListInfo;

						WriteValue(pHeadShokIdInfo, HeadShokIndex);

						{//HeadShokInfo
							BYTE* pHeadShokInfo = pHeadShokIdInfo;

							Write8(pHeadShokInfo, 253);//FD	
							BYTE* pHeadShokIdInfoLen = pHeadShokInfo;
							Write32(pHeadShokInfo, 0);//HeadShokIdInfo len 33

							WriteKeyValue(pHeadShokInfo, 3, ActiveBackPlateIdList[i]);
							WriteKeyValue(pHeadShokInfo, 4, 0);
							WriteKeyValue(pHeadShokInfo, 5, 0);

							len = pHeadShokInfo - pHeadShokIdInfo;
							Set32(pHeadShokIdInfoLen, len - 5);
							pHeadShokIdInfo += len;
						}

						len = pHeadShokIdInfo - pHeadShokIdListInfo;
						pHeadShokIdListInfo += len;
						++HeadShokIndex;
					}

					WriteLenValueEnd(pSTHandInfo, pHeadShokIdListInfo, pActiveBackPlateListInfoLen);
				}

				WriteTextValue(pSTHandInfo, "ResultID", 0);

				WriteLenValueEnd(pData, pSTHandInfo, pSTHandInfoLen);
			}
			WriteBodyHeadEnd(pBody, pData);
		}

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta87_ReqOpHeadAndHeadShok(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 33;
	UINT Parameter1 = Read32(Body);
	UINT Parameter2 = Read32(Body);
	Body += 1;
	UINT Parameter3 = Read32(Body);
	UINT Parameter4 = Read32(Body);
	Body += 1;
	double ChangeType = ReadValue(Body);//1 更换头像 2 更换头像框 3 底板
	double HeadValue = ReadValue(Body);//切换的头像id
	if (HeadValue > 100) {//恢复头像框和底板
		Client->UserHeads = 1;
		Client->UserHeadsBox = 1;
		Client->UserFloor = 1;
		PlayerDB_UpdateHead(Client);
	}
	else if (ChangeType == 2) {
		Client->UserHeadsBox = HeadValue;
		PlayerDB_UpdateHead(Client);
	}
	else if (ChangeType == 1) {
		Client->UserHeads = HeadValue;
		PlayerDB_UpdateHead(Client);
	}
	else if (ChangeType == 3) {
		Client->UserFloor = HeadValue;
		PlayerDB_UpdateHead(Client);
	}


	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
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

		{
			BYTE* pData = pBody;
			Write16(pData, 0); //MsgLen

			memset(pData, 0, 20);
			memcpy(pData, "DispatchCoResponse", 18);
			pData += 20;

			Write8(pData, 246);//F6
			Write32(pData, Parameter1);
			Write32(pData, Parameter2);

			Write8(pData, 246);//F6
			Write32(pData, 0);
			Write32(pData, 0);


			len = pData - pBody;
			//printf("Request_Power_GetStatus Uin:%d len:%d\n", Client->Uin, len);

			Set16(pBody, (WORD)len - 2);
			pBody += len;

		}

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	/*char* pMsg = EByteToHexUA2((const unsigned char*)buf, len);
	printf("Request_Beta87_GetLoginCfg	Hex:%s\n", pMsg);*/

	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta87_ReqSetOrUnSetUseQQHead(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//更换头像
	Body += 35;
	UINT Parameter1 = Read32(Body);
	UINT Parameter2 = Read32(Body);
	Body += 1;
	double retValue1 = Kget<double>(Body, 0);//1 切换QQ头像 0普通头像
	if (retValue1 == 1) {//切换QQ头像
		Client->UserHeads = 0;
		PlayerDB_UpdateHead(Client);
	}
	//printf("Request_Beta87_ReqSetOrUnSetUseQQHead	Parameter1:%d Parameter2:%d Parameter3:%d Parameter4:%d\n", Parameter1, Parameter2, Parameter3, Parameter4);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
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

		{
			BYTE* pData = pBody;
			Write16(pData, 0); //MsgLen

			memset(pData, 0, 20);
			memcpy(pData, "DispatchCoResponse", 18);
			pData += 20;

			Write8(pData, 246);//F6
			Write32(pData, Parameter1);
			Write32(pData, Parameter2);

			Write8(pData, 246);//F6
			Write32(pData, 0);
			Write32(pData, 0);


			len = pData - pBody;
			//printf("Request_Power_GetStatus Uin:%d len:%d\n", Client->Uin, len);

			Set16(pBody, (WORD)len - 2);
			pBody += len;

		}

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	/*char* pMsg = EByteToHexUA2((const unsigned char*)buf, len);
	printf("Request_Beta87_GetLoginCfg	Hex:%s\n", pMsg);*/

	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta87_ReqBatchHeadAndHeadShokInfo(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.3) return;
	Body += 40;
	double readValue1 = ReadValue(Body);
	Body += 2;
	int MsgLen = Read16(Body);
	UINT UINItem = MsgLen / 18;
	Body += 1;
	//1查找好友 10红人 可以合并
	if (UINItem == 1) {

		double readValue2 = ReadValue(Body);
		double readValue3 = ReadValue(Body);
		UINT uintValue = static_cast<UINT>(readValue3);
		std::vector<PlayerInfo> playerInfo;
		PlayerDB_SeleteBaseInfoHead(uintValue, playerInfo);
		if (playerInfo.size() == 0) return;
		double intValue = 0;
		bool isQQhead = playerInfo[0].UserHeads == 0 ? true : false;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len
			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				{//STHandInfo				
					BYTE* pSTHandInfoLen = WriteLenValueStart(pData);
					BYTE* pSTHandInfo = pData;
					WriteValue(pSTHandInfo, readValue2);

					BYTE* pSTHandInfoLen2 = WriteLenValueStart(pSTHandInfo);
					BYTE* pSTHandInfo2 = pSTHandInfo;
					WriteText(pSTHandInfo2, "Uin");

					WriteValue(pSTHandInfo2, readValue3);

					WriteText(pSTHandInfo2, "EquipHeadId");
					{
						BYTE* pEquipBackPlateInfolen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pEquipBackPlateInfo = pSTHandInfo2;

						WriteTextValue(pEquipBackPlateInfo, "Id", playerInfo[0].UserHeads);
						WriteTextValue(pEquipBackPlateInfo, "LastActiveTime", -1);
						WriteTextValue(pEquipBackPlateInfo, "ExpirationTime", -1);
						WriteTextValue(pEquipBackPlateInfo, "QQHeadLegallyTime", 0);
						WriteLenValueEnd(pSTHandInfo2, pEquipBackPlateInfo, pEquipBackPlateInfolen);

					}

					WriteText(pSTHandInfo2, "EquipHeadShokId");
					{//EquipHeadShokIdInfo	装备头像框信息
						BYTE* pEquipHeadShokIdInfoLen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pEquipHeadShokIdInfo = pSTHandInfo2;

						WriteKeyValue(pEquipHeadShokIdInfo, 2, playerInfo[0].UserHeadsBox);
						WriteKeyValue(pEquipHeadShokIdInfo, 3, -1);
						WriteKeyValue(pEquipHeadShokIdInfo, 4, -1);
						WriteLenValueEnd(pSTHandInfo2, pEquipHeadShokIdInfo, pEquipHeadShokIdInfoLen);

					}
					WriteText(pSTHandInfo2, "EquipBackPlateId");
					{
						BYTE* pEquipBackPlateInfolen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pEquipBackPlateInfo = pSTHandInfo2;

						WriteKeyValue(pEquipBackPlateInfo, 2, playerInfo[0].UserFloor);
						WriteKeyValue(pEquipBackPlateInfo, 3, -1);
						WriteKeyValue(pEquipBackPlateInfo, 4, -1);
						WriteLenValueEnd(pSTHandInfo2, pEquipBackPlateInfo, pEquipBackPlateInfolen);
					}
					if (isQQhead == true) {
						WriteText(pSTHandInfo2, "QQHeadUrlInfo", 13);
						BYTE* pQQHeadUrlInfoLen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pQQHeadUrlInfo = pSTHandInfo2;
						{
							WriteKeyValue(pQQHeadUrlInfo, 0, readValue3);
							WriteTextValue(pQQHeadUrlInfo, "FaceFlag", 60);
							WriteTextValue(pQQHeadUrlInfo, "SysID", 0);
							WriteTextValue(pQQHeadUrlInfo, "FaceType", 1);
							WriteText(pQQHeadUrlInfo, "URL");

							// 替换 QQ 号
							std::string url = qqurl;
							size_t pos = url.find("QQ号");
							if (pos != std::string::npos) {
								url.replace(pos, 3, playerInfo[0].qqNumber); // 3 是 "QQ号" 的长度
							}

							WriteText(pQQHeadUrlInfo, url.c_str(), 105);
							//WriteText(pQQHeadUrlInfo, "http://thirdqq.qlogo.cn/g?b=oidb&k=xmnpMxy0IfZQAia7cpTz8sQ&kti=Zuv-egv7oIE&s=", 77);
							WriteTextValue(pQQHeadUrlInfo, "TimeStamp", (DWORD)time(nullptr));
						}

						WriteLenValueEnd(pSTHandInfo2, pQQHeadUrlInfo, pQQHeadUrlInfoLen);
						WriteTextValue(pSTHandInfo2, "UseQQHead", 1);
					}
					else {
						WriteText(pSTHandInfo2, "QQHeadUrlInfo");
						WriteLenValue(pSTHandInfo2, 0);
						WriteTextValue(pSTHandInfo2, "UseQQHead", 0);
					}

					WriteLenValueEnd(pSTHandInfo, pSTHandInfo2, pSTHandInfoLen2);

					WriteLenValueEnd(pData, pSTHandInfo2, pSTHandInfoLen);
				}
				WriteBodyHeadEnd(pBody, pData);
			}

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();

		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (UINItem <= 10) {
		std::vector<UINT> UINS(UINItem);
		std::vector<PlayerInfo> playerInfo;
		for (size_t i = 0; i < UINItem; i++)
		{
			UINT Index = ReadValue(Body);
			UINT UIN = ReadValue(Body);
			UINS[i] = UIN;
			PlayerDB_SeleteBaseInfoHead(UINS[i], playerInfo);
			playerInfo[i].Index = Index;
		}

		if (playerInfo.size() == 0) return;

		int indexToMove = -1;
		for (size_t i = 0; i < playerInfo.size(); ++i) {
			if (playerInfo[i].UserHeads == 0) {
				indexToMove = static_cast<int>(i);
				break; 
			}
		}

		if (indexToMove != -1) {
			std::swap(playerInfo[0], playerInfo[indexToMove]);
		}

		bool isQQhead = playerInfo[0].UserHeads == 0 ? true : false;
		//isQQhead = true;
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len
			{
				BYTE* pData = WriteBodyHead(pBody, readValue1);
				{//STHandInfo				
					BYTE* pSTHandInfoLen = WriteLenValueStart(pData);
					BYTE* pSTHandInfo = pData;
					WriteValue(pSTHandInfo, playerInfo[0].Index);


					BYTE* pSTHandInfoLen2 = WriteLenValueStart(pSTHandInfo);
					BYTE* pSTHandInfo2 = pSTHandInfo;
					WriteText(pSTHandInfo2, "Uin");
					WriteValue(pSTHandInfo2, playerInfo[0].Uin);
					WriteText(pSTHandInfo2, "EquipHeadId");
					{
						BYTE* pEquipBackPlateInfolen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pEquipBackPlateInfo = pSTHandInfo2;

						WriteTextValue(pEquipBackPlateInfo, "Id", playerInfo[0].UserHeads);
						WriteTextValue(pEquipBackPlateInfo, "LastActiveTime", -1);
						WriteTextValue(pEquipBackPlateInfo, "ExpirationTime", -1);
						WriteTextValue(pEquipBackPlateInfo, "QQHeadLegallyTime", 0);
						WriteLenValueEnd(pSTHandInfo2, pEquipBackPlateInfo, pEquipBackPlateInfolen);

					}
					WriteText(pSTHandInfo2, "EquipHeadShokId");
					{//EquipHeadShokIdInfo	装备头像框信息
						BYTE* pEquipHeadShokIdInfoLen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pEquipHeadShokIdInfo = pSTHandInfo2;

						WriteKeyValue(pEquipHeadShokIdInfo, 2, playerInfo[0].UserHeadsBox);
						WriteKeyValue(pEquipHeadShokIdInfo, 3, -1);
						WriteKeyValue(pEquipHeadShokIdInfo, 4, -1);
						WriteLenValueEnd(pSTHandInfo2, pEquipHeadShokIdInfo, pEquipHeadShokIdInfoLen);

					}
					WriteText(pSTHandInfo2, "EquipBackPlateId");
					{
						BYTE* pEquipBackPlateInfolen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pEquipBackPlateInfo = pSTHandInfo2;

						WriteKeyValue(pEquipBackPlateInfo, 2, playerInfo[0].UserFloor);
						WriteKeyValue(pEquipBackPlateInfo, 3, -1);
						WriteKeyValue(pEquipBackPlateInfo, 4, -1);
						WriteLenValueEnd(pSTHandInfo2, pEquipBackPlateInfo, pEquipBackPlateInfolen);
					}
					if (isQQhead == true) {
						WriteText(pSTHandInfo2, "QQHeadUrlInfo", 13);
						BYTE* pQQHeadUrlInfoLen = WriteLenValueStart(pSTHandInfo2);
						BYTE* pQQHeadUrlInfo = pSTHandInfo2;
						{
							WriteKeyValue(pQQHeadUrlInfo, 0, playerInfo[0].Uin);
							WriteTextValue(pQQHeadUrlInfo, "FaceFlag", 60);
							WriteTextValue(pQQHeadUrlInfo, "SysID", 0);
							WriteTextValue(pQQHeadUrlInfo, "FaceType", 1);
							WriteText(pQQHeadUrlInfo, "URL");

							// 替换 QQ 号
							std::string url = qqurl;
							size_t pos = url.find("QQ号");
							if (pos != std::string::npos) {
								url.replace(pos, 3, playerInfo[0].qqNumber); // 3 是 "QQ号" 的长度
							}

							WriteText(pQQHeadUrlInfo, url.c_str(), 105);
							//WriteText(pQQHeadUrlInfo, "http://thirdqq.qlogo.cn/g?b=oidb&k=xmnpMxy0IfZQAia7cpTz8sQ&kti=Zuv-egv7oIE&s=", 77);
							WriteTextValue(pQQHeadUrlInfo, "TimeStamp", (DWORD)time(nullptr));
						}

						WriteLenValueEnd(pSTHandInfo2, pQQHeadUrlInfo, pQQHeadUrlInfoLen);
						WriteTextValue(pSTHandInfo2, "UseQQHead", 1);
					}
					else {
						//WriteText(pSTHandInfo2, "QQHeadUrlInfo");
						//WriteLenValue(pSTHandInfo2, 0);
						WriteTextValue(pSTHandInfo2, "UseQQHead", 0);
					}
					WriteLenValueEnd(pSTHandInfo, pSTHandInfo2, pSTHandInfoLen2);


					for (size_t i = 1; i < playerInfo.size(); i++)
					{

						char isQQheadUin;
						isQQheadUin = playerInfo[i].UserHeads == 0 ? true : false;

						WriteValue(pSTHandInfo, playerInfo[i].Index);
						BYTE* pUinInfolen = WriteLenValueStart(pSTHandInfo);
						BYTE* pUinInfo = pSTHandInfo;

						WriteKeyValue(pUinInfo, 0, playerInfo[i].Uin);

						WriteHex(pUinInfo, "FB01FD00000021");
						WriteKeyValue(pUinInfo, 2, playerInfo[i].UserHeads);
						WriteKeyValue(pUinInfo, 3, -1);
						WriteKeyValue(pUinInfo, 4, -1);
						WriteKeyValue(pUinInfo, 5, 0);

						WriteHex(pUinInfo, "FB06FD00000021");
						WriteKeyValue(pUinInfo, 2, playerInfo[i].UserHeadsBox);//头像框
						WriteKeyValue(pUinInfo, 3, -1);
						WriteKeyValue(pUinInfo, 4, -1);


						WriteHex(pUinInfo, "FB07FD00000021");
						WriteKeyValue(pUinInfo, 2, playerInfo[i].UserFloor);
						WriteKeyValue(pUinInfo, 3, -1);
						WriteKeyValue(pUinInfo, 4, -1);

						if (isQQheadUin) {
							WriteHex(pUinInfo, "FB08");
							BYTE* pQQHeadUrlInfoLen = WriteLenValueStart(pUinInfo);
							BYTE* pQQHeadUrlInfo = pUinInfo;
							WriteKeyValue(pQQHeadUrlInfo, 0, playerInfo[i].Uin);
							WriteKeyValue(pQQHeadUrlInfo, 9, 60);
							WriteKeyValue(pQQHeadUrlInfo, 10, 0);
							WriteKeyValue(pQQHeadUrlInfo, 11, 1);
							WriteHex(pQQHeadUrlInfo, "FB0C");
							std::string url = qqurl;
							size_t pos = url.find("QQ号");
							if (pos != std::string::npos) {
								url.replace(pos, 3, playerInfo[i].qqNumber); // 3 是 "QQ号" 的长度
							}
							WriteText(pQQHeadUrlInfo, url.c_str());
							WriteKeyValue(pQQHeadUrlInfo, 14, 1716997832);
							WriteKeyValue(pQQHeadUrlInfo, 15, 1);
							WriteLenValueEnd(pUinInfo, pQQHeadUrlInfo, pQQHeadUrlInfoLen);
						}
						else {
							if (isQQhead) {
								WriteKeyValue(pUinInfo, 15, 0);
							}
							else {
								WriteKeyValue(pUinInfo, 8, 0);
							}

						}

						WriteLenValueEnd(pSTHandInfo, pUinInfo, pUinInfolen);
					}

					WriteLenValueEnd(pData, pSTHandInfo, pSTHandInfoLen);
				}
				WriteBodyHeadEnd(pBody, pData);
			}

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();
		//PrintfBody(Client, 24251, buf.data(), len);

		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}

}

NOINLINE void Beta87_ReqHeadAndHeadShokAvatarCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 40;
	double readValue = ReadValue(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHead(pBody, readValue);
			WriteValue(pData, 1);
			WriteF9(pData);
			{//STHandInfo				
				BYTE* pSTHandInfoLen = WriteLenValueStart(pData);
				BYTE* pSTHandInfo = pData;

				WriteText(pSTHandInfo, "m_stHeadShokCfg");
				{
					BYTE* pHeadShokCfgLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pHeadShokCfg = pSTHandInfo;

					{
						WriteValue(pHeadShokCfg, 1);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteText(pHeadShok, "m_chAcquisitionChannel");
						WriteText(pHeadShok);
						WriteTextValue(pHeadShok, "m_uchDefault", 1);
						WriteTextValue(pHeadShok, "m_chDesc", 2);
						WriteTextValue(pHeadShok, "m_ushHeadId", 1);
						WriteTextTwo(pHeadShok, "m_chHeadName", "取消装备头像框");

						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 2);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "16周年庆活动获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "粉紫爱心于顶端闪烁，引人注目，赛博朋克风的幻彩圆环增添活力！");
						WriteKeyValue(pHeadShok, 5, 2);
						WriteKeyText(pHeadShok, 6, "16周年头像框");

						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 3);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与涂抹大乱斗获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "绚烂的色彩不经意交融，看似随意，实则充满艺术感，青春气息扑面而来。");
						WriteKeyValue(pHeadShok, 5, 4);
						WriteKeyText(pHeadShok, 6, "涂抹大乱斗头像框");

						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 4);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "巅峰赛季结算获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "钛银拼接纯金外框彰显机械感，好似王者登临，立于极速之巅，更显尊贵霸气。");
						WriteKeyValue(pHeadShok, 5, 5);
						WriteKeyText(pHeadShok, 6, "巅峰赛头像框");

						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 5);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与车王手册获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "红色龙头威猛无比，龙目如珠似玉，随风飘飞的金麟象征蒸蒸日上，坚定守护泱泱华夏。");
						WriteKeyValue(pHeadShok, 5, 6);
						WriteKeyText(pHeadShok, 6, "龙之传人头像框");
						WriteText(pHeadShok, "m_iNeedBelongItemList");
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 135172);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 6);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与山海神龙获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "天地之灵汇聚，神木逐渐生长，风云变幻，雷电交织，神力灌注，神龙诞于山林。");
						WriteKeyValue(pHeadShok, 5, 7);
						WriteKeyText(pHeadShok, 6, "青龙占星头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 135165);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 7);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与山海神龙获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "炽焰在神力的浇灌下现出龙形，凶猛非常，幽色焰火隐隐燃烧，为之蓄力。");
						WriteKeyValue(pHeadShok, 5, 8);
						WriteKeyText(pHeadShok, 6, "烛龙弄焰头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 135167);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 8);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与山海神龙获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "风起云涌，迅速汇聚一处幻作龙形，蕴含其中的神力乍现，掀起飓风似焰。");
						WriteKeyValue(pHeadShok, 5, 10);
						WriteKeyText(pHeadShok, 6, "应龙驭风头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 135166);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 9);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与山海神龙获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "浪涛翻涌呈现一片祥瑞之象，神龙遨游其中掀起层层浪花，惠泽万物。");
						WriteKeyValue(pHeadShok, 5, 9);
						WriteKeyText(pHeadShok, 6, "冰夷踏浪头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 135168);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 10);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与木兰活动获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "持剑可卫国保家，别簪可贤惠持家，以小小身躯凭借坚定意志成就一代传奇。");
						WriteKeyValue(pHeadShok, 5, 11);
						WriteKeyText(pHeadShok, 6, "极星木兰头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 136725);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 11);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与雷因的装甲车间获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "雷诺重工倾力打造而成，专属深绿一眼便知其所属，瞩目的标识置于顶端异常坚定。");
						WriteKeyValue(pHeadShok, 5, 12);
						WriteKeyText(pHeadShok, 6, "雷诺之力头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 136722);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 12);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与国风四君子获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "春风抚落梅花瓣，吹到了坚毅的青竹周围，不知不觉间菊花盛开，兰花于一侧恬静美好。");
						WriteKeyValue(pHeadShok, 5, 13);
						WriteKeyText(pHeadShok, 6, "四君子头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 129288);
							WriteValue(pNeedBelong, 2);
							WriteValue(pNeedBelong, 132211);
							WriteValue(pNeedBelong, 3);
							WriteValue(pNeedBelong, 133688);
							WriteValue(pNeedBelong, 4);
							WriteValue(pNeedBelong, 135170);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}


						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 13);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "2023车王冠军获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "深红宝石蕴藏着选手们的热情与心血，金冠稳固戴于顶端，这份荣誉无可磨灭。");
						WriteKeyValue(pHeadShok, 5, 14);
						WriteKeyText(pHeadShok, 6, "2023冠军头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 14);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "2023车王亚军获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "深蓝宝石象征深不可测的能量，尚未完全被发掘，银冠加冕，亦是对实力的肯定。");
						WriteKeyValue(pHeadShok, 5, 15);
						WriteKeyText(pHeadShok, 6, "2023亚军头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 15);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "2023车王季军获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "金铜打磨得十分光滑，波光流转间是一次赛事的纪念，亦是对未来生活的勉励。");
						WriteKeyValue(pHeadShok, 5, 16);
						WriteKeyText(pHeadShok, 6, "2023季军头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 16);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与神龙召唤获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "坚毅的眼神令人无处可逃也不敢直视，以大地之力铸成青环，守护一方圣土。");
						WriteKeyValue(pHeadShok, 5, 17);
						WriteKeyText(pHeadShok, 6, "机械青龙王头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 136604);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 17);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与山海神龙获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "山海变换，四龙终汇聚，神力聚集不可抵挡，圣洁的神光引人探究。");
						WriteKeyValue(pHeadShok, 5, 18);
						WriteKeyText(pHeadShok, 6, "山海神龙头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 135165);
							WriteValue(pNeedBelong, 2);
							WriteValue(pNeedBelong, 135166);
							WriteValue(pNeedBelong, 3);
							WriteValue(pNeedBelong, 135167);
							WriteValue(pNeedBelong, 4);
							WriteValue(pNeedBelong, 135168);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 18);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "获得龙腾九霄后获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "青色闪电附着，巨龙的眼睛，深邃如宇宙，幽光四射，散发龙族威严霸气。");
						WriteKeyValue(pHeadShok, 5, 19);
						WriteKeyText(pHeadShok, 6, "龙腾九霄头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 136712);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 19);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与罗小黑活动获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "一侧绿芽悄然绽放，充满生机，罗小黑休憩于一旁，一片春意盎然。");
						WriteKeyValue(pHeadShok, 5, 20);
						WriteKeyText(pHeadShok, 6, "罗小黑头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 20);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "获得终极烈焰新星-孔明后获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "以孔明羽扇纶巾为灵感冶炼而成，银框图样融合道法精神，白鹤羽扇化作轻羽落在两侧，蓝色晶钻点缀，定能闪耀赛场。");
						WriteKeyValue(pHeadShok, 5, 24);
						WriteKeyText(pHeadShok, 6, "孔明头像框");
						WriteKeyIndex(pHeadShok, 20);
						{
							BYTE* pNeedBelongLen = WriteLenValueStart(pHeadShok);
							BYTE* pNeedBelong = pHeadShok;
							WriteValue(pNeedBelong, 1);
							WriteValue(pNeedBelong, 138418);

							WriteLenValueEnd(pHeadShok, pNeedBelong, pNeedBelongLen);
						}
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 21);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与小樱的风之牌获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "纯银圆框闪耀煜煜光辉，好似魔法旋转萦绕。");
						WriteKeyValue(pHeadShok, 5, 25);
						WriteKeyText(pHeadShok, 6, "木之本樱头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 22);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "参与贵妃羞花活动获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "贵妃醉酒,贵妃羞花。");
						WriteKeyValue(pHeadShok, 5, 23);
						WriteKeyText(pHeadShok, 6, "玉环头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 23);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "获得墨影青龙-赛车后获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "贵妃醉酒,贵妃羞花。");
						WriteKeyValue(pHeadShok, 5, 22);
						WriteKeyText(pHeadShok, 6, "墨影青龙头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 24);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "获得青龙战姬-赛车后获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "贵妃醉酒,贵妃羞花。");
						WriteKeyValue(pHeadShok, 5, 21);
						WriteKeyText(pHeadShok, 6, "青龙战姬头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 26);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "天蓝与灿烂黄还有蔚蓝的涂料交织在一起");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "小黄鸭");
						WriteKeyValue(pHeadShok, 5, 26);
						WriteKeyText(pHeadShok, 6, "小黄鸭头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}
					{
						WriteValue(pHeadShokCfg, 27);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "免费获得");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "雾山五行联动专属头像框。");
						WriteKeyValue(pHeadShok, 5, 27);
						WriteKeyText(pHeadShok, 6, "雾山五行头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}

					{
						WriteValue(pHeadShokCfg, 28);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "北欧众神");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "神秘符号中心宝石，金紫蓝光环环绕，翅膀星芒映辉煌。");
						WriteKeyValue(pHeadShok, 5, 28);
						WriteKeyText(pHeadShok, 6, "北欧众神头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}

					{
						WriteValue(pHeadShokCfg, 29);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "神奥丁");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "星辰漫天，遨游天地。");
						WriteKeyValue(pHeadShok, 5, 29);
						WriteKeyText(pHeadShok, 6, "神奥丁头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}

					{
						WriteValue(pHeadShokCfg, 30);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "维舞清歌");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "在头像框的顶部和底部，各有一个类似叶子形状的设计，颜色为绿色和黄色相间，增添了一种自然和生机勃勃的感觉。");
						WriteKeyValue(pHeadShok, 5, 30);
						WriteKeyText(pHeadShok, 6, "维舞清歌头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}

					{
						WriteValue(pHeadShokCfg, 31);
						BYTE* pHeadShokLen = WriteLenValueStart(pHeadShokCfg);
						BYTE* pHeadShok = pHeadShokCfg;
						WriteKeyText(pHeadShok, 1, "大耳狗");
						WriteKeyValue(pHeadShok, 3, 1);
						WriteKeyText(pHeadShok, 4, "物品暂无介绍...........");
						WriteKeyValue(pHeadShok, 5, 31);
						WriteKeyText(pHeadShok, 6, "大耳狗头像框");
						WriteLenValueEnd(pHeadShokCfg, pHeadShok, pHeadShokLen);
					}


					WriteLenValueEnd(pSTHandInfo, pHeadShokCfg, pHeadShokCfgLen);

				}
				WriteText(pSTHandInfo, "m_stHeadCfg");
				{
					BYTE* pHeadCfgLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pHeadCfg = pSTHandInfo;

					{
						WriteValue(pHeadCfg, 1);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "可爱靓丽的Q版小橘子，具有特殊羁绊能力，给予无限祝福。让开朗率真的小橘子陪在你左右，助你驰骋赛场，勇夺桂冠。");
						WriteKeyValue(pHead, 5, 1);
						WriteKeyText(pHead, 6, "Q版小橘子");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 2);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "帅气精致的Q版白勇太，对赛车有着极致的热情，拥有成为飞车大陆第一高手的梦想，与他相伴，一起驰骋飞车大陆。");
						WriteKeyValue(pHead, 5, 2);
						WriteKeyText(pHead, 6, "Q版白勇太");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 3);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "镜，集美貌与智慧于一身，是极星动力现任代言人，飞车大陆四大车神之一，与她为伴是成为车王的开始。");
						WriteKeyValue(pHead, 5, 3);
						WriteKeyText(pHead, 6, "Q版镜");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 4);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "玥，一位实力深不可测的赛车手，有着无畏一切的勇气，有着对赛车一往无前的执着，与玥为伴，终将璀璨。");
						WriteKeyValue(pHead, 5, 4);
						WriteKeyText(pHead, 6, "Q版玥");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 5);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "帅气高冷的西泽尔，是特洛伊工业现任代言人，飞车大陆四大车神之一，至高车技，无人可比，有他相伴，成为车神指日可待！");
						WriteKeyValue(pHead, 5, 5);
						WriteKeyText(pHead, 6, "Q版西泽尔");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 6);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "流火女王库洛迪卡，清纯可爱的外表下隐藏着一颗竞技的心，历经无数场实战，无一败绩，与她为伴，未来不可估量。");
						WriteKeyValue(pHead, 5, 6);
						WriteKeyText(pHead, 6, "Q版库洛迪卡");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 7);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "聪明绝顶的穆尼克拉是阿尔法科技现任代言人，飞车大陆四大车神之一，擅用科技改装，是每一位车手的福音。");
						WriteKeyValue(pHead, 5, 7);
						WriteKeyText(pHead, 6, "Q版穆尼克拉");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 8);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "雷因，雷诺重工现任代言人，飞车大陆四大车神之一，拥有无畏一切的勇气，精通赛车改装，是每一位车手都想相交的车神。");
						WriteKeyValue(pHead, 5, 8);
						WriteKeyText(pHead, 6, "Q版雷因");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 9);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "帅气精致的脸庞俘获众多异性，这是为高玩车手打造的专属头像，提高运气，助你在对抗中化险为夷。");
						WriteKeyValue(pHead, 5, 9);
						WriteKeyText(pHead, 6, "帅哥头像");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 10);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "可爱脸庞衬托灵动眼眸，一颦一笑百媚丛生，令异性为之倾倒，是女子赛车手的专属头像，内含车神祈福之力，助你在对局中一往无前。");
						WriteKeyValue(pHead, 5, 10);
						WriteKeyText(pHead, 6, "靓女头像");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}
					{
						WriteValue(pHeadCfg, 11);
						BYTE* pHeadLen = WriteLenValueStart(pHeadCfg);
						BYTE* pHead = pHeadCfg;
						WriteKeyText(pHead, 1, "免费获得");
						WriteKeyValue(pHead, 3, 1);
						WriteKeyText(pHead, 4, "飞车之旅，畅享速度与柔情，唯美留言，诠释挚爱与自由。");
						WriteKeyValue(pHead, 5, 11);
						WriteKeyText(pHead, 6, "QQ飞车");


						WriteLenValueEnd(pHeadCfg, pHead, pHeadLen);
					}


					WriteLenValueEnd(pSTHandInfo, pHeadCfg, pHeadCfgLen);

				}
				WriteText(pSTHandInfo, "m_stBackPlateCfg");
				{
					BYTE* pBackPlateCfgLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pBackPlateCfg = pSTHandInfo;

					{
						WriteValue(pBackPlateCfg, 1);
						BYTE* pBackPlateLen = WriteLenValueStart(pBackPlateCfg);
						BYTE* pBackPlate = pBackPlateCfg;
						WriteKeyText(pBackPlate, 1, "自动获得");
						WriteKeyValue(pBackPlate, 3, 1);
						WriteKeyText(pBackPlate, 4, "默认名片底板");
						WriteKeyValue(pBackPlate, 5, 1);
						WriteKeyText(pBackPlate, 6, "默认");


						WriteLenValueEnd(pBackPlateCfg, pBackPlate, pBackPlateLen);
					}
					{
						WriteValue(pBackPlateCfg, 2);
						BYTE* pBackPlateLen = WriteLenValueStart(pBackPlateCfg);
						BYTE* pBackPlate = pBackPlateCfg;
						WriteKeyText(pBackPlate, 1, "参与樱之魔法牌活动获得");
						WriteKeyValue(pBackPlate, 3, 1);
						WriteKeyText(pBackPlate, 4, "小樱手持魔杖，身着白天鹅礼服，可爱优雅，头戴水晶冠，扬起自信的微笑，是最耀眼的存在，每时每刻都会被吸引视线。");
						WriteKeyValue(pBackPlate, 5, 2);
						WriteKeyText(pBackPlate, 6, "百变小樱名片底板");


						WriteLenValueEnd(pBackPlateCfg, pBackPlate, pBackPlateLen);
					}

					{
						WriteValue(pBackPlateCfg, 3);
						BYTE* pBackPlateLen = WriteLenValueStart(pBackPlateCfg);
						BYTE* pBackPlate = pBackPlateCfg;
						WriteKeyText(pBackPlate, 1, "雾山五行");
						WriteKeyValue(pBackPlate, 3, 1);
						WriteKeyText(pBackPlate, 4, "免费获得~");
						WriteKeyValue(pBackPlate, 5, 3);
						WriteKeyText(pBackPlate, 6, "雾山五行名片底板");


						WriteLenValueEnd(pBackPlateCfg, pBackPlate, pBackPlateLen);
					}

					{
						WriteValue(pBackPlateCfg, 4);
						BYTE* pBackPlateLen = WriteLenValueStart(pBackPlateCfg);
						BYTE* pBackPlate = pBackPlateCfg;
						WriteKeyText(pBackPlate, 1, "飞驰新疆");
						WriteKeyValue(pBackPlate, 3, 1);
						WriteKeyText(pBackPlate, 4, "免费获得~");
						WriteKeyValue(pBackPlate, 5, 4);
						WriteKeyText(pBackPlate, 6, "飞驰新疆名片底板");


						WriteLenValueEnd(pBackPlateCfg, pBackPlate, pBackPlateLen);
					}

					{
						WriteValue(pBackPlateCfg, 5);
						BYTE* pBackPlateLen = WriteLenValueStart(pBackPlateCfg);
						BYTE* pBackPlate = pBackPlateCfg;
						WriteKeyText(pBackPlate, 1, "大耳狗");
						WriteKeyValue(pBackPlate, 3, 1);
						WriteKeyText(pBackPlate, 4, "免费获得~");
						WriteKeyValue(pBackPlate, 5, 5);
						WriteKeyText(pBackPlate, 6, "大耳狗名片底板");


						WriteLenValueEnd(pBackPlateCfg, pBackPlate, pBackPlateLen);
					}

					WriteLenValueEnd(pSTHandInfo, pBackPlateCfg, pBackPlateCfgLen);
				}
				WriteLenValueEnd(pData, pSTHandInfo, pSTHandInfoLen);
			}
			WriteBodyHeadEnd(pBody, pData);
		}

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

NOINLINE void Beta87_ReqHeadAndHeadShokActiveItemCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 44;
	double readValue = ReadValue(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	struct ActiveHeadItemList // 自动喇叭喊话
	{
		int ActivePeriod;
		int ActiveHeadId;
		int ActiveItemId;
	};
	ActiveHeadItemList ActiveHeadItemListCfg[] =
	{
		{-1,1,138047},
		{-1,2,138048},
		{-1,3,138049},
		{-1,4,138050},
		{-1,5,138051},
		{-1,6,138052},
		{-1,7,138053},
		{-1,8,138054},
		{-1,9,138055},
		{-1,10,138056},
	};
	UINT ActiveHeadtNum = sizeof(ActiveHeadItemListCfg) / sizeof(ActiveHeadItemListCfg[0]);
	ActiveHeadItemList ActiveHeadShokItemListCfg[] =
	{
		{-1,2,138057},
		{-1,4,138059},
		{-1,5,138060},
		{-1,6,138061},
		{-1,7,138062},
		{-1,8,138063},
		{-1,10,138064},
		{-1,11,138066},
		{-1,12,138067},
		{-1,14,138069},
		{-1,15,138070},
		{-1,16,138071},
		{-1,16,138071},
		{-1,17,138072},
		{-1,18,138073},
		{-1,19,139768},
		{-1,20,139769},
		{-1,24,139773},
		{-1,25,139774},
		{-1,26,144320},
		{-1,27,144321},
		{-1,28,144515},
		{-1,29,144516},
		{-1,30,146373},
		{-1,31,147937},
	};
	UINT ActiveHeadShoktNum = sizeof(ActiveHeadShokItemListCfg) / sizeof(ActiveHeadShokItemListCfg[0]);

	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHead(pBody, readValue);
			WriteF9(pData);
			{
				BYTE* pSTHandInfoLen = WriteLenValueStart(pData);
				BYTE* pSTHandInfo = pData;
				WriteText(pSTHandInfo, "m_stActiveHeadItemList");
				{//ActiveHeadItemList
					BYTE* pActiveHeadLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pActiveHead = pSTHandInfo;
					for (size_t i = 0; i < ActiveHeadtNum; i++) {
						if (i == 0) {
							WriteValue(pActiveHead, i + 1);
							WriteLenValue(pActiveHead, 80);
							WriteTextValue(pActiveHead, "m_iActivePeriod", ActiveHeadItemListCfg[i].ActivePeriod);
							WriteTextValue(pActiveHead, "m_uiActiveHeadId", ActiveHeadItemListCfg[i].ActiveHeadId);
							WriteTextValue(pActiveHead, "m_uiActiveItemId", ActiveHeadItemListCfg[i].ActiveItemId);
						}
						else {
							WriteValue(pActiveHead, i + 1);
							WriteLenValue(pActiveHead, 33);
							WriteKeyValue(pActiveHead, 1, ActiveHeadItemListCfg[i].ActivePeriod);
							WriteKeyValue(pActiveHead, 2, ActiveHeadItemListCfg[i].ActiveHeadId);
							WriteKeyValue(pActiveHead, 3, ActiveHeadItemListCfg[i].ActiveItemId);
						}
					}
					WriteLenValueEnd(pSTHandInfo, pActiveHead, pActiveHeadLen);
				}


				WriteText(pSTHandInfo, "m_stActiveHeadShokItemList");
				{//ActiveHeadShok
					BYTE* pActiveHeadShokLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pActiveHeadShok = pSTHandInfo;
					for (size_t i = 0; i < ActiveHeadShoktNum; i++) {
						WriteValue(pActiveHeadShok, i + 1);
						WriteLenValue(pActiveHeadShok, 33);
						WriteKeyValue(pActiveHeadShok, 1, ActiveHeadShokItemListCfg[i].ActivePeriod);
						WriteKeyValue(pActiveHeadShok, 2, ActiveHeadShokItemListCfg[i].ActiveHeadId);
						WriteKeyValue(pActiveHeadShok, 3, ActiveHeadShokItemListCfg[i].ActiveItemId);
					}
					WriteLenValueEnd(pSTHandInfo, pActiveHeadShok, pActiveHeadShokLen);
				}



				WriteText(pSTHandInfo, "m_stActiveBackPlateItemList");
				{//ActiveBackPlate
					BYTE* pActiveBackPlateLen = WriteLenValueStart(pSTHandInfo);
					BYTE* pActiveBackPlate = pSTHandInfo;
					{
						WriteValue(pActiveBackPlate, 1);
						WriteLenValue(pActiveBackPlate, 33);
						WriteKeyValue(pActiveBackPlate, 1, -1);
						WriteKeyValue(pActiveBackPlate, 2, 2.0);
						WriteKeyValue(pActiveBackPlate, 3, 140027);
					}
					WriteLenValueEnd(pSTHandInfo, pActiveBackPlate, pActiveBackPlateLen);
				}
				WriteLenValueEnd(pData, pSTHandInfo, pSTHandInfoLen);
			}
			WriteBodyHeadEnd(pBody, pData);
		}

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE int CarTreasureCfgData(BYTE*& pData, double& intValue) {

	int len = 0;
	{
		Write8(pData, 246);//F6
		intValue = 1;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 253);//FD	
		Write32(pData, 0x000000b1);//消息数据长度 InfoLen
		len += 0x000000b1;
		Write8(pData, 250);//FA
		Write8(pData, 7);//len
		memcpy(pData, "itemNum", 7);
		pData += 7;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].ItemNum;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 250);//FA
		Write8(pData, 8);//len
		memcpy(pData, "itemType", 8);
		pData += 8;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].ItemType;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 250);//FA
		Write8(pData, 12);//len
		memcpy(pData, "iAvailPeriod", 12);
		pData += 12;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].AvailPeriod;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置


		Write8(pData, 250);//FA
		Write8(pData, 12);//len
		memcpy(pData, "iCanExchange", 12);
		pData += 12;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].CanExchange;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 250);//FA
		Write8(pData, 15);//len
		memcpy(pData, "iOtherAttribute", 15);
		pData += 15;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].OtherAttribute;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 250);//FA
		Write8(pData, 6);//len
		memcpy(pData, "itemId", 6);
		pData += 6;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].ItemId;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 250);//FA
		Write8(pData, 12);//len
		memcpy(pData, "iProbability", 12);
		pData += 12;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].iProbability;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置

		Write8(pData, 250);//FA
		Write8(pData, 17);//len
		memcpy(pData, "iExchangePieceNum", 17);
		pData += 17;

		Write8(pData, 246);//F6
		intValue = CarTreasures->Treasures[0].ExchangePieceNum;
		std::memcpy(pData, &intValue, sizeof(double));
		pData += sizeof(double);  // 移动到下一个数据的位置
	}
	for (size_t i = 1; i < CarTreasures->Treasures.size(); i++)
	{
		if (GetItemType(CarTreasures->Treasures[i].ItemId) == EAIT_CAR || CarTreasures->Treasures[i].ExchangePieceNum > 0) {
			Write8(pData, 246);  // F6
			intValue = i + 1;  // Param_f6=2.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 253); // FD
			Write32(pData, 0x00000058);  // Param_fd28=00000058
			len += 0x00000058;
			Write8(pData, 251);  // FB
			Write8(pData, 3);  // Param_fb=3
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ItemNum; // Param_f6=1.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 4);  // Param_fb=4
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ItemType; // Param_f6=1.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 5);  // Param_fb=5
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].AvailPeriod; // Param_f6=360.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 6);  // Param_fb=6
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].CanExchange; // Param_f6=0.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 7);  // Param_fb=7
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].OtherAttribute; // Param_f6=1.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 8);  // Param_fb=8
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ItemId; // Param_f6=129099.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 9);  // Param_fb=9
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].iProbability; // Param_f6=10.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 10); // Param_fb=10
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ExchangePieceNum; // Param_f6=20.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);
		}
		else {
			Write8(pData, 246);  // F6
			intValue = i + 1;  // Param_f6=2.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 253); // FD
			Write32(pData, 0x0000004d);  // Param_fd28=0000004d
			len += 0x0000004d;
			Write8(pData, 251);  // FB
			Write8(pData, 3);  // Param_fb=3
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ItemNum; // Param_f6=1.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 4);  // Param_fb=4
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ItemType; // Param_f6=0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 5);  // Param_fb=5
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].AvailPeriod; // Param_f6=0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 8);  // Param_fb=8
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ItemId; // Param_f6=21977.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 9);  // Param_fb=9
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].iProbability; // Param_f6=14000.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 6);  // Param_fb=6
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].OtherAttribute; // Param_f6=0.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);

			Write8(pData, 251);  // FB
			Write8(pData, 7);  // Param_fb=7
			Write8(pData, 246); // F6
			intValue = CarTreasures->Treasures[i].ExchangePieceNum; // Param_f6=0.0
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);
		}

	}
	return len;
}

NOINLINE void RequestGetCarTreasureNoSelfSelectCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 42;
	UINT Parameter1 = Read32(Body);
	UINT Parameter2 = Read32(Body);

	BYTE* buf = new BYTE[8192]; BYTE* p = buf; size_t len;
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

		{
			BYTE* pData = pBody;
			Write16(pData, 0); //MsgLen

			memset(pData, 0, 20);
			memcpy(pData, "DispatchCoResponse", 18);
			pData += 20;

			Write8(pData, 246);
			Write32(pData, Parameter1);
			Write32(pData, Parameter2);

			Write8(pData, 246);
			Write32(pData, 0);
			Write32(pData, 0);

			Write8(pData, 253);//FD	
			int ComItemTableNum = PlayerDB_SelectComItemTableNum(Client);
			int Num = 4583;
			int AddNum = 0;
			if (ComItemTableNum == 1) {
				AddNum += 95;
			}
			else if (ComItemTableNum > 1) {
				AddNum += 95;
				AddNum += 69 * (ComItemTableNum - 1);
			}
			BYTE* pPetNum = pData;
			Write32(pData, 0);//消息数据长度 InfoLen
			Set32(pPetNum, Num + AddNum);//消息数据长度 InfoLen

			Write8(pData, 250);//FA
			Write8(pData, 11);//len
			memcpy(pData, "iCostItemID", 11);
			pData += 11;

			Write8(pData, 246);//F6
			double intValue = 99662;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 10);//len
			memcpy(pData, "iCloseDate", 10);
			pData += 10;

			Write8(pData, 246);//F6
			intValue = 20240430;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 14);//len
			memcpy(pData, "award_poll_cfg", 14);
			pData += 14;

			Write8(pData, 253);//FD	
			BYTE* pCarTreasureNum = pData;
			//Write32(pData, 0x000010d3);//消息数据长度 InfoLen
			Write32(pData, 0);//消息数据长度 InfoLen
			int CfgLen = CarTreasureCfgData(pData, intValue);
			Set32(pCarTreasureNum, (WORD)(CfgLen + 588));





			Write8(pData, 250);//FA
			Write8(pData, 9);//len
			memcpy(pData, "iOpenTime", 9);
			pData += 9;
			Write8(pData, 246);//F6
			intValue = 0;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 20);//len
			memcpy(pData, "iExchangePieceItemID", 20);
			pData += 20;
			Write8(pData, 246);//F6
			intValue = 94880;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 16);//len
			memcpy(pData, "RetDecpItemTable", 16);
			pData += 16;

			Write8(pData, 253); //FD
			Write32(pData, 0);

			Write8(pData, 250);//FA
			Write8(pData, 15);//len
			memcpy(pData, "RetRecItemTable", 15);
			pData += 15;

			Write8(pData, 253); //FD
			Write32(pData, 0);

			Write8(pData, 250);//FA
			Write8(pData, 10);//len
			memcpy(pData, "iCloseTime", 10);
			pData += 10;
			Write8(pData, 246);//F6
			intValue = 235959;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 15);//len
			memcpy(pData, "RetComItemTable", 15);
			pData += 15;

			if (ComItemTableNum > 0)
			{
				Write8(pData, 253); //FD 
				Write32(pData, AddNum);
				PlayerDB_SelectComItemTable(Client, pData, intValue);
			}
			else
			{//如果没有暂存就走这个
				Write8(pData, 253); //FD
				Write32(pData, 0);
			}

			Write8(pData, 250);//FA
			Write8(pData, 16);//len
			memcpy(pData, "iCaostNumTenTime", 16);
			pData += 16;
			Write8(pData, 246);//F6
			intValue = 10;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 15);//len
			memcpy(pData, "iCostNumOneTime", 15);
			pData += 15;
			Write8(pData, 246);//F6
			intValue = 1;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置

			Write8(pData, 250);//FA
			Write8(pData, 9);//len
			memcpy(pData, "iOpenDate", 9);
			pData += 9;
			Write8(pData, 246);//F6
			intValue = 20240401;
			std::memcpy(pData, &intValue, sizeof(double));
			pData += sizeof(double);  // 移动到下一个数据的位置


			len = pData - pBody;
			Set16(pBody, (WORD)len);
			pBody += len;

		}

		Write16(pBody, 0x0000);

		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf;

	SendToClient(Client, 24251, buf, len, Client->ConnID, FE_PLAYER, Client->ConnID, Response); delete[] buf;
}

NOINLINE int GetRate(int Ran) {
	for (auto& obj : CarTreasures->TreasureMap) {
		if (Ran <= obj.first) {
			return obj.first;
		}
	}
}
NOINLINE int GetRate2(int Ran) {
	for (auto& obj : CarTreasures->PtTreasureMap) {
		if (Ran <= obj.first) {
			return obj.first;
		}
	}
}
static unsigned char acdatatest[] = {
  0x00, 0x0D, 0xAF, 0x9E, 0x9A, 0x79, 0x66, 0x24, 0x82, 0x90, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x00, 0xD4, 0x44, 0x69, 0x73, 0x70, 0x61, 0x74, 0x63, 0x68, 0x43, 0x6F, 0x52, 0x65, 0x73,
  0x70, 0x6F, 0x6E, 0x73, 0x65, 0x00, 0x00, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x40, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x00, 0x00, 0xA9,
  0xFA, 0x0D, 0x6D, 0x5F, 0x61, 0x73, 0x74, 0x49, 0x74, 0x65, 0x6D, 0x49, 0x6E, 0x66, 0x6F, 0xFD, 0x00, 0x00, 0x00, 0x68, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xFD,
  0x00, 0x00, 0x00, 0x5A, 0xFA, 0x0E, 0x6D, 0x5F, 0x69, 0x41, 0x76, 0x61, 0x69, 0x6C, 0x50, 0x65, 0x72, 0x69, 0x6F, 0x64, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x86, 0x40, 0xFA,
  0x0D, 0x6D, 0x5F, 0x75, 0x73, 0x68, 0x49, 0x74, 0x65, 0x6D, 0x54, 0x79, 0x70, 0x65, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xFA, 0x0A, 0x6D, 0x5F, 0x69, 0x49, 0x74,
  0x65, 0x6D, 0x4E, 0x75, 0x6D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xFA, 0x09, 0x6D, 0x5F, 0x69, 0x49, 0x74, 0x65, 0x6D, 0x49, 0x44, 0xF6, 0x00, 0x00, 0x00, 0x00,
  0xF0, 0xD8, 0xFE, 0x40, 0xFA, 0x0A, 0x6D, 0x5F, 0x73, 0x49, 0x74, 0x65, 0x6D, 0x4E, 0x75, 0x6D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xFA, 0x0D, 0x6D, 0x5F, 0x69,
  0x53, 0x75, 0x70, 0x65, 0x72, 0x4D, 0x6F, 0x6E, 0x65, 0x79, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31
};
static unsigned char acduobaoquanbugou[] = {
   0x00, 0x0D, 0x6D, 0x28, 0xC0, 0x2B, 0x66, 0x24, 0xEE, 0x14, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x81, 0x44, 0x69, 0x73, 0x70, 0x61, 0x74, 0x63, 0x68, 0x43, 0x6F, 0x52, 0x65, 0x73,
  0x70, 0x6F, 0x6E, 0x73, 0x65, 0x00, 0x00, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x40, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x5E, 0x40, 0xFD, 0x00, 0x00, 0x00, 0x56,
  0xFA, 0x0A, 0x6D, 0x5F, 0x73, 0x49, 0x74, 0x65, 0x6D, 0x4E, 0x75, 0x6D, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x0D, 0x6D, 0x5F, 0x69, 0x53, 0x75, 0x70, 0x65,
  0x72, 0x4D, 0x6F, 0x6E, 0x65, 0x79, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x0A, 0x6D, 0x5F, 0x69, 0x43, 0x6F, 0x75, 0x70, 0x6F, 0x6E, 0x73, 0xF6, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x0D, 0x6D, 0x5F, 0x61, 0x73, 0x74, 0x49, 0x74, 0x65, 0x6D, 0x49, 0x6E, 0x66, 0x6F, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

NOINLINE void RequestGetCarTreasureNoSelfSelectAward(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 44;
	UINT Parameter1 = Read32(Body);
	UINT Parameter2 = Read32(Body);
	Body += 1;
	UINT Parameter3 = Read32(Body);
	UINT Parameter4 = Read32(Body);
	Body += 1;
	UINT Parameter5 = Read32(Body);
	UINT Parameter6 = Read32(Body);
	Body += 1;
	UINT Parameter7 = Read32(Body);
	Body += 1;
	double retValue1 = Kget<double>(Body, 0);
	Body += 9;
	UINT Parameter8 = Read32(Body);
	Body += 10;
	double retValue2 = Kget<double>(Body, 0);
	Body += 22;
	double retValue3 = Kget<double>(Body, 0);
	Body += 17;
	double retValue4 = Kget<double>(Body, 0);


	if (Parameter6 == 61503 && PlayerDB_GetItemNum(Client->Uin, 99662) >= 1) //单次抽奖
	{
		{
			PlayerDB_Reduce(Client->Uin, 99662, 1);
			NotifyUseItemResult(Client, 99662, 1, PlayerDB_GetItemNum(Client->Uin, 99662));
		}
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

			{
				BYTE* pData = pBody;
				Write16(pData, 0); //MsgLen

				memset(pData, 0, 20);
				memcpy(pData, "DispatchCoResponse", 18);
				pData += 20;
			
				int IndexId;
					
				int randomID = 0;
				std::random_device rd;
				std::mt19937 eng(rd());
				std::uniform_int_distribution<int> dis(1, CarTreasures->MaxRate);
				randomID = dis(eng);
				IndexId = GetRate(randomID);
					
				if (CarTreasures->TreasureMap[IndexId].size() > 1) {
					std::random_device rd;
					std::mt19937 eng(rd());
					std::uniform_int_distribution<int> dis(1, CarTreasures->TreasureMap[IndexId].size() - 1);
					randomID = dis(eng);
				}
				if (CarTreasures->TreasureMap[IndexId].size() <= randomID){
					return;
				}
				CarTreasureInfo Temp = CarTreasures->TreasureMap[IndexId][randomID];
				
				PlayerDB_AddComItemTable(Client, Temp.ItemId, Temp.AvailPeriod, Temp.ExchangePieceNum);
				//Write8(pData, 246);//F6
				//std::memcpy(pData, &retValue, sizeof(double));
				//pData += sizeof(double);  // 移动到下一个数据的位置

				//Write8(pData, 246);//F6
				//double intValue = 0;
				//std::memcpy(pData, &intValue, sizeof(double));
				//pData += sizeof(double);  // 移动到下一个数据的位置
				Write8(pData, 246);
				Write32(pData, Parameter1);
				Write32(pData, Parameter2);

				Write8(pData, 246);
				Write32(pData, 0);
				Write32(pData, 0);

				Write8(pData, 253);//FD	
				Write32(pData, 0x000000BE);//消息数据长度 InfoLen

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "m_sItemNum", 10);
				pData += 10;

				Write8(pData, 246);//F6
				double intValue = 1;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_iSuperMoney", 13);
				pData += 13;

				Write8(pData, 246);//F6
				intValue = 0;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "m_iCoupons", 10);
				pData += 10;

				Write8(pData, 246);//F6
				intValue = 0;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_astItemInfo", 13);
				pData += 13;

				Write8(pData, 253); //FD
				Write32(pData, 0x00000068);

				{ //抽奖得到的单个物品的信息
					Write8(pData, 246);//F6
					intValue = 1;
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 253); //FD
					Write32(pData, 0x0000005A);

					Write8(pData, 250);//FA
					Write8(pData, 14);//len
					memcpy(pData, "m_iAvailPeriod", 14);
					pData += 14;

					Write8(pData, 246);//F6
					intValue = Temp.AvailPeriod;  //期限
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 250);//FA
					Write8(pData, 13);//len
					memcpy(pData, "m_ushItemType", 13);
					pData += 13;

					Write8(pData, 246);//F6
					intValue = Temp.ItemType;//物品类型
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 250);//FA
					Write8(pData, 10);//len
					memcpy(pData, "m_iItemNum", 10);
					pData += 10;

					Write8(pData, 246);//F6
					intValue = Temp.ItemNum;  //数量
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 250);//FA
					Write8(pData, 9);//len
					memcpy(pData, "m_iItemID", 9);
					pData += 9;
					intValue = Temp.ItemId;
					Write8(pData, 246);//F6
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置
				}
				Write8(pData, 253); //FD
				Write32(pData, 0x0000001E);

				Write8(pData, 250);//FA
				Write8(pData, 6);//len
				memcpy(pData, "Conpus", 6);
				pData += 6;

				Write8(pData, 253); //FD
				Write32(pData, 0);

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "SuperMoney", 10);
				pData += 10;

				Write8(pData, 253); //FD
				Write32(pData, 0);


				len = pData - pBody;
				Set16(pBody, (WORD)len);
				pBody += len;
			}

			Write16(pBody, 0xFD7D);

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();

		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		//delete[] buf;
	}
	else if (Parameter6 == 9280 && PlayerDB_GetItemNum(Client->Uin, 99662) >= 10)
	{
		{
			PlayerDB_Reduce(Client->Uin, 99662, 10);
			NotifyUseItemResult(Client, 99662, 10, PlayerDB_GetItemNum(Client->Uin, 99662));
		}
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

			{
				BYTE* pData = pBody;
				Write16(pData, 0); //MsgLen

				memset(pData, 0, 20);
				memcpy(pData, "DispatchCoResponse", 18);
				pData += 20;

				//Write8(pData, 246);//F6
				//std::memcpy(pData, &retValue, sizeof(double));
				//pData += sizeof(double);  // 移动到下一个数据的位置

				//Write8(pData, 246);//F6
				//double intValue = 0;
				//std::memcpy(pData, &intValue, sizeof(double));
				//pData += sizeof(double);  // 移动到下一个数据的位置
				Write8(pData, 246);
				Write32(pData, Parameter1);
				Write32(pData, Parameter2);

				Write8(pData, 246);
				Write32(pData, 0);
				Write32(pData, 0);

				Write8(pData, 253);//FD	
				Write32(pData, 0x000002C8);//消息数据长度 InfoLen

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "m_sItemNum", 10);
				pData += 10;

				Write8(pData, 246);//F6
				double intValue = 10;   //总的获奖物品数目
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_iSuperMoney", 13);
				pData += 13;

				Write8(pData, 246);//F6
				intValue = 0;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "m_iCoupons", 10);
				pData += 10;

				Write8(pData, 246);//F6
				intValue = 0;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_astItemInfo", 13);
				pData += 13;

				Write8(pData, 253); //FD
				Write32(pData, 0x00000272);

				int randomID;
				if (CarTreasures->JPTreasures.size() == 1) {
					randomID = 0;
				}
				else {
					std::random_device rd;
					std::mt19937 eng(rd());

					std::uniform_int_distribution<int> dis(0, CarTreasures->JPTreasures.size() - 1);
					randomID = dis(eng);
				}
				if (CarTreasures->JPTreasures.size() <= randomID) {
					return;
				}
				CarTreasureInfo Temp = CarTreasures->JPTreasures[randomID];

				{//十连抽第一个物品作为极品奖励，单独放奖池
					Write8(pData, 246);//F6
					intValue = 1;
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 253); //FD
					Write32(pData, 0x0000005A);

					Write8(pData, 250);//FA
					Write8(pData, 14);//len
					memcpy(pData, "m_iAvailPeriod", 14);
					pData += 14;

					Write8(pData, 246);//F6
					intValue = Temp.AvailPeriod; //期限
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 250);//FA
					Write8(pData, 13);//len
					memcpy(pData, "m_ushItemType", 13);
					pData += 13;

					Write8(pData, 246);//F6
					intValue = Temp.ItemType;
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 250);//FA
					Write8(pData, 10);//len
					memcpy(pData, "m_iItemNum", 10);
					pData += 10;

					Write8(pData, 246);//F6
					intValue = Temp.ItemNum;
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置

					Write8(pData, 250);//FA
					Write8(pData, 9);//len
					memcpy(pData, "m_iItemID", 9);
					pData += 9;

					Write8(pData, 246);//F6     //第一个物品
					intValue = Temp.ItemId; //物品id
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置 第一个结束
				}
				//DB_Begin(PlayerEm);//开启事务
				PlayerDB_AddComItemTable(Client, Temp.ItemId, Temp.AvailPeriod, Temp.ExchangePieceNum);
				for (size_t i = 0; i < 9; i++) {//十连抽后续奖励，次品
					{
						int IndexId;
						{
							int randomID;
							std::random_device rd;
							std::mt19937 eng(rd());
							std::uniform_int_distribution<int> dis(1, CarTreasures->MaxRate2);
							randomID = dis(eng);
							IndexId = GetRate2(randomID);
						}
						{
							if (CarTreasures->PtTreasureMap[IndexId].size() == 1) {
								Temp = CarTreasures->PtTreasureMap[IndexId][0];
							}
							else {
								int randomID;
								std::random_device rd;
								std::mt19937 eng(rd());
								std::uniform_int_distribution<int> dis(0, CarTreasures->PtTreasureMap[IndexId].size() - 1);
								randomID = dis(eng);
								if (CarTreasures->PtTreasureMap[IndexId].size() <= randomID) {
									return;
								}
								Temp = CarTreasures->PtTreasureMap[IndexId][randomID];
							}

						}
					}
					PlayerDB_AddComItemTable(Client, Temp.ItemId, Temp.AvailPeriod, Temp.ExchangePieceNum);
					Write8(pData, 246);//F6
					intValue = 2 + i;
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);

					Write8(pData, 253); //FD
					Write32(pData, 0x0000002C);

					Write8(pData, 251);//FB   
					Write8(pData, 4);//标签4，为期限
					Write8(pData, 246);//F6
					intValue = Temp.AvailPeriod; //期限
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);
					Write8(pData, 251);//FB
					Write8(pData, 5);
					Write8(pData, 246);//F6
					intValue = 0;
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);
					Write8(pData, 251);//FB
					Write8(pData, 6);
					Write8(pData, 246);//F6
					intValue = Temp.ItemNum;//数量
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);
					Write8(pData, 251);//FB
					Write8(pData, 7);

					Write8(pData, 246);//F6     //第二个物品
					intValue = Temp.ItemId; //物品id
					std::memcpy(pData, &intValue, sizeof(double));
					pData += sizeof(double);  // 移动到下一个数据的位置 第二个结束
				}
				Write8(pData, 253); //FD
				Write32(pData, 0x0000001E);
				//DB_Commit(PlayerEm);//提交事务
				Write8(pData, 250);//FA
				Write8(pData, 6);//len
				memcpy(pData, "Conpus", 6);
				pData += 6;

				Write8(pData, 253); //FD
				Write32(pData, 0);

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "SuperMoney", 10);
				pData += 10;

				Write8(pData, 253); //FD
				Write32(pData, 0);


				len = pData - pBody;
				Set16(pBody, (WORD)len);
				pBody += len;
			}

			Write16(pBody, 0x0042);

			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}

		len = p - buf.data();

		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response); //delete[] buf;
	}
	else if (Parameter4 == 61503)
	{
		UBeta87_ReqHeadAndHeadShokAvatarCfg* pMsg = reinterpret_cast<UBeta87_ReqHeadAndHeadShokAvatarCfg*>(acdatatest);
		pMsg->Uin = htonl(Client->Uin);
		pMsg->Time = htonl((DWORD)time(nullptr));
		pMsg->Parameter1 = htonl(Parameter1);
		pMsg->Parameter2 = htonl(Parameter2);
		SendToClient(Client, 24251, reinterpret_cast<PBYTE>(pMsg), 231, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
		PlayerDB_UpdateComItemTable(Client, retValue4, retValue3, retValue2);
		if (retValue3 > 0) {
			int AvailPeriod = retValue3 * retValue2;
			int ItemNum = retValue2;
			int ItemID = retValue4;
			PlayerDB_AddItem2(Client, Client->Uin, retValue4, 1, AvailPeriod, false);
			{
				std::vector<BYTE> buf(819200); BYTE* p = buf.data();
				size_t len;

				Write32(p, Client->Uin); // Uin
				Write32(p, Client->Uin); // SrcUin
				Write16(p, ItemNum);	 // ItemNum
				{
					for (size_t i = 0; i < ItemNum; i++)
					{
						BYTE* pItemInfo = p;
						Write16(pItemInfo, 0); // len

						Write32(pItemInfo, ItemID);
						Write32(pItemInfo, ItemNum);
						Write32(pItemInfo, AvailPeriod);
						Write8(pItemInfo, 0);
						Write32(pItemInfo, 0x66248290);
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
		else {
			PlayerDB_AddItem2(Client, Client->Uin, retValue4, 1, retValue3, false);
			{
				int AvailPeriod = retValue3;
				int ItemNum = retValue2;
				int ItemID = retValue4;
				std::vector<BYTE> buf(819200); BYTE* p = buf.data();
				size_t len;

				Write32(p, Client->Uin); // Uin
				Write32(p, Client->Uin); // SrcUin
				Write16(p, ItemNum);	 // ItemNum
				{
					for (size_t i = 0; i < ItemNum; i++)
					{
						BYTE* pItemInfo = p;
						Write16(pItemInfo, 0); // len

						Write32(pItemInfo, ItemID);
						Write32(pItemInfo, ItemNum);
						Write32(pItemInfo, AvailPeriod);
						Write8(pItemInfo, 0);
						Write32(pItemInfo, 0x66248290);
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
	else if (Parameter4 == 64 && Parameter7 == 71)//分解
	{
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
		UINT ExchangePieceNum = PlayerDB_GetExchangePieceNum(Client, retValue4, retValue3, retValue2);
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
		{//Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len
			{
				BYTE* pData = pBody;
				Write16(pData, 0); //MsgLen

				memset(pData, 0, 20);
				memcpy(pData, "DispatchCoResponse", 18);
				pData += 20;

				Write8(pData, 246);
				Write32(pData, Parameter1);
				Write32(pData, Parameter2);

				Write8(pData, 246);
				Write32(pData, 0);
				Write32(pData, 0);
				Write8(pData, 253);//FD	
				Write32(pData, 0x000000a9);//消息数据长度 InfoLen

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_astItemInfo", 13);
				pData += 13;

				Write8(pData, 253); //FD
				Write32(pData, 0x00000068);

				Write8(pData, 246);//F6
				double intValue = 1;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 253); //FD
				Write32(pData, 0x0000005A);

				Write8(pData, 250);//FA
				Write8(pData, 14);//len
				memcpy(pData, "m_iAvailPeriod", 14);
				pData += 14;
				Write8(pData, 246);//F6
				intValue = 0;  //期限
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_ushItemType", 13);
				pData += 13;
				Write8(pData, 246);//F6
				intValue = 0;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "m_iItemNum", 10);
				pData += 10;
				Write8(pData, 246);//F6

				intValue = ExchangePieceNum;  //数量   需要查库看这个物品的分解数量，根据retValue4
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 9);//len
				memcpy(pData, "m_iItemID", 9);
				pData += 9;

				Write8(pData, 246);//F6
				intValue = retValue4; //物品id
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置 

				Write8(pData, 250);//FA
				Write8(pData, 10);//len
				memcpy(pData, "m_sItemNum", 10);
				pData += 10;
				Write8(pData, 246);//F6
				intValue = 1; //物品 
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 13);//len
				memcpy(pData, "m_iSuperMoney", 13);
				pData += 13;
				Write8(pData, 246);//F6
				intValue = 0; //物品
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				len = pData - pBody;
				Set16(pBody, (WORD)len);
				pBody += len;
			}
			Write16(pBody, 0xFD7D);
			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}
		len = p - buf.data();
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response); //delete[] buf;
		PlayerDB_UpdateComItemTable(Client, retValue4, retValue3, retValue2);
		if (retValue3 > 0) {
			int AvailPeriod = 0;
			int ItemNum = ExchangePieceNum;
			int ItemID = 94880;
			PlayerDB_AddItem2(Client, Client->Uin, ItemID, ItemNum, AvailPeriod, false);
			{
				std::vector<BYTE> buf(819200); BYTE* p = buf.data();
				size_t len;

				Write32(p, Client->Uin); // Uin
				Write32(p, Client->Uin); // SrcUin
				Write16(p, 1);	 // ItemNum
				int ItemSpNum = PlayerDB_GetItemNum(Client->Uin, 94880);
				{
					for (size_t i = 0; i < 1; i++)
					{
						BYTE* pItemInfo = p;
						Write16(pItemInfo, 0); // len

						Write32(pItemInfo, ItemID);
						Write32(pItemInfo, ItemSpNum);
						Write32(pItemInfo, AvailPeriod);
						Write8(pItemInfo, 0);
						Write32(pItemInfo, 0x66248290);
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
		else {

			{
				int AvailPeriod = 0;
				int ItemNum = ExchangePieceNum;
				int ItemID = 94880;
				PlayerDB_AddItem2(Client, Client->Uin, 94880, ItemNum, AvailPeriod, false);
				std::vector<BYTE> buf(819200); BYTE* p = buf.data();
				size_t len;
				int ItemSpNum = PlayerDB_GetItemNum(Client->Uin, 94880);
				Write32(p, Client->Uin); // Uin
				Write32(p, Client->Uin); // SrcUin
				Write16(p, 1);	 // ItemNum
				{
					for (size_t i = 0; i < 1; i++)
					{
						BYTE* pItemInfo = p;
						Write16(pItemInfo, 0); // len

						Write32(pItemInfo, ItemID);
						Write32(pItemInfo, ItemSpNum);
						Write32(pItemInfo, AvailPeriod);
						Write8(pItemInfo, 0);
						Write32(pItemInfo, 0x66248290);
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
	else if (Parameter4 == 64 && Parameter7 > 71)//全部分解
	{
		ResponseStartGame(Client, "不允许全部分解！");
		//BYTE* buf = new BYTE[8192]; //BYTE* p = buf; size_t len;
		//UINT ExchangePieceNum = PlayerDB_GetExchangePieceNum(Client, retValue4, retValue3, retValue2);
		//{ //Head
		//	BYTE* pHead = p;
		//	Write16(pHead, 0); //len

		//	Write32(pHead, Client->Uin); //Uin
		//	Write32(pHead, (DWORD)time(nullptr)); //Time
		//	Write8(pHead, 0); //Version
		//	Write8(pHead, 0); //Compress
		//	Write8(pHead, 0); //OptLength

		//	len = pHead - p;
		//	Set16(p, (WORD)len);
		//	p += len;
		//}
		//{//Body
		//	BYTE* pBody = p;
		//	Write16(pBody, 0); //len
		//	{
		//		BYTE* pData = pBody;
		//		Write16(pData, 0); //MsgLen

		//		memset(pData, 0, 20);
		//		memcpy(pData, "DispatchCoResponse", 18);
		//		pData += 20;

		//		Write8(pData, 246);
		//		Write32(pData, Parameter1);
		//		Write32(pData, Parameter2);

		//		Write8(pData, 246);
		//		Write32(pData, 0);
		//		Write32(pData, 0);
		//		Write8(pData, 253);//FD	
		//		Write32(pData, 0x000000a9);//消息数据长度 InfoLen

		//		Write8(pData, 250);//FA
		//		Write8(pData, 13);//len
		//		memcpy(pData, "m_astItemInfo", 13);
		//		pData += 13;

		//		Write8(pData, 253); //FD
		//		Write32(pData, 0x00000068);

		//		Write8(pData, 246);//F6
		//		double intValue = 1;
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置

		//		Write8(pData, 253); //FD
		//		Write32(pData, 0x0000005A);

		//		Write8(pData, 250);//FA
		//		Write8(pData, 14);//len
		//		memcpy(pData, "m_iAvailPeriod", 14);
		//		pData += 14;
		//		Write8(pData, 246);//F6
		//		intValue = 0;  //期限
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置

		//		Write8(pData, 250);//FA
		//		Write8(pData, 13);//len
		//		memcpy(pData, "m_ushItemType", 13);
		//		pData += 13;
		//		Write8(pData, 246);//F6
		//		intValue = 0;
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置

		//		Write8(pData, 250);//FA
		//		Write8(pData, 10);//len
		//		memcpy(pData, "m_iItemNum", 10);
		//		pData += 10;
		//		Write8(pData, 246);//F6

		//		intValue = ExchangePieceNum;  //数量   需要查库看这个物品的分解数量，根据retValue4
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置

		//		Write8(pData, 250);//FA
		//		Write8(pData, 9);//len
		//		memcpy(pData, "m_iItemID", 9);
		//		pData += 9;

		//		Write8(pData, 246);//F6
		//		intValue = retValue4; //物品id
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置 

		//		Write8(pData, 250);//FA
		//		Write8(pData, 10);//len
		//		memcpy(pData, "m_sItemNum", 10);
		//		pData += 10;
		//		Write8(pData, 246);//F6
		//		intValue = 1; //物品 
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置

		//		Write8(pData, 250);//FA
		//		Write8(pData, 13);//len
		//		memcpy(pData, "m_iSuperMoney", 13);
		//		pData += 13;
		//		Write8(pData, 246);//F6
		//		intValue = 0; //物品
		//		std::memcpy(pData, &intValue, sizeof(double));
		//		pData += sizeof(double);  // 移动到下一个数据的位置

		//		len = pData - pBody;
		//		Set16(pBody, (WORD)len);
		//		pBody += len;
		//	}
		//	Write16(pBody, 0xFD7D);
		//	len = pBody - p;
		//	Set16(p, (WORD)len);
		//	p += len;
		//}
		//len = p - buf.data();
		//SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response); delete[] buf;
		//PlayerDB_UpdateComItemTable(Client, retValue4, retValue3, retValue2);
		//if (retValue3 > 0) {
		//	int AvailPeriod = 0;
		//	int ItemNum = ExchangePieceNum;
		//	int ItemID = 94880;
		//	PlayerDB_AddItem(Client->Uin, ItemID, ItemNum, AvailPeriod, false);
		//	{
		//		BYTE* buf = new BYTE[8192]; if (!buf) { return; }
		//		//BYTE* p = buf;
		//		size_t len;

		//		Write32(p, Client->Uin); // Uin
		//		Write32(p, Client->Uin); // SrcUin
		//		Write16(p, 1);	 // ItemNum
		//		int ItemSpNum = PlayerDB_GetItemNum(Client->Uin, 94880);
		//		{
		//			for (size_t i = 0; i < 1; i++)
		//			{
		//				BYTE* pItemInfo = p;
		//				Write16(pItemInfo, 0); // len

		//				Write32(pItemInfo, ItemID);
		//				Write32(pItemInfo, ItemSpNum);
		//				Write32(pItemInfo, AvailPeriod);
		//				Write8(pItemInfo, 0);
		//				Write32(pItemInfo, 0x66248290);
		//				Write32(pItemInfo, 0);
		//				Write16(pItemInfo, 0); // ItemType
		//				len = pItemInfo - p;
		//				Set16(p, (WORD)len);
		//				p += len;
		//			}
		//		}
		//		Write16(p, 0); // AttachLen
		//		Write16(p, 0); // AddKartNum
		//		Write8(p, 0); // IncRedPacketNum

		//		len = p - buf.data();
		//		SendToClient(Client, 529, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify); delete[] buf;
		//	}
		//}
		//else {

		//	{
		//		int AvailPeriod = 0;
		//		int ItemNum = ExchangePieceNum;
		//		int ItemID = 94880;
		//		PlayerDB_AddItem(Client->Uin, 94880, ItemNum, AvailPeriod, false);
		//		BYTE* buf = new BYTE[8192]; if (!buf) { return; }
		//		//BYTE* p = buf;
		//		size_t len;
		//		int ItemSpNum = PlayerDB_GetItemNum(Client->Uin, 94880);
		//		Write32(p, Client->Uin); // Uin
		//		Write32(p, Client->Uin); // SrcUin
		//		Write16(p, 1);	 // ItemNum
		//		{
		//			for (size_t i = 0; i < 1; i++)
		//			{
		//				BYTE* pItemInfo = p;
		//				Write16(pItemInfo, 0); // len

		//				Write32(pItemInfo, ItemID);
		//				Write32(pItemInfo, ItemSpNum);
		//				Write32(pItemInfo, AvailPeriod);
		//				Write8(pItemInfo, 0);
		//				Write32(pItemInfo, 0x66248290);
		//				Write32(pItemInfo, 0);
		//				Write16(pItemInfo, 0); // ItemType
		//				len = pItemInfo - p;
		//				Set16(p, (WORD)len);
		//				p += len;
		//			}
		//		}
		//		Write16(p, 0); // AttachLen
		//		Write16(p, 0); // AddKartNum
		//		Write8(p, 0); // IncRedPacketNum

		//		len = p - buf.data();
		//		SendToClient(Client, 529, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify); delete[] buf;
		//	}
		//}
	}
	else
	{
		UBeta87_ReqHeadAndHeadShokAvatarCfg* pMsg = reinterpret_cast<UBeta87_ReqHeadAndHeadShokAvatarCfg*>(acduobaoquanbugou);
		pMsg->Uin = htonl(Client->Uin);
		pMsg->Time = htonl((DWORD)time(nullptr));
		pMsg->Parameter1 = htonl(Parameter1);
		pMsg->Parameter2 = htonl(Parameter2);
		SendToClient(Client, 24251, reinterpret_cast<PBYTE>(pMsg), 148, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}

}

NOINLINE void Request_beta91_Mountain_Music_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 35;
	double readValue1 = ReadValue(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000603");//固定长度
			WriteText(pData, "playerInfo");
			WriteHex(pData, "FD00000119");//固定长度
			WriteText(pData, "choujiangProcessAwardStatusTable");
			WriteHex(pData, "FD00000048");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteValue(pData, 0);
			WriteText(pData, "choujiangItemID");
			WriteValue(pData, 146370);
			WriteText(pData, "choujiangItemNum");
			WriteValue(pData, 50);
			WriteText(pData, "choujiangNum");
			WriteValue(pData, 0);
			WriteText(pData, "playAwardStatus");
			WriteHex(pData, "FD00000048");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteValue(pData, 0);
			WriteText(pData, "playAward");
			WriteHex(pData, "FD00000062");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000054");//固定长度
			WriteText(pData, "type");
			WriteValue(pData, 1);
			WriteText(pData, "m_ushItemType");
			WriteValue(pData, 0);
			WriteText(pData, "m_iSuperMoney");
			WriteValue(pData, 888);
			WriteText(pData, "m_iCoupons");
			WriteValue(pData, 0);
			WriteText(pData, "baseCfg");
			WriteHex(pData, "FD00000026");//固定长度
			WriteText(pData, "endTime");
			WriteValue(pData, 1730908799);
			WriteText(pData, "beginTime");
			WriteValue(pData, 1727712000);
			WriteText(pData, "dayTaskTable");
			WriteHex(pData, "FD0000023f");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000de");//固定长度
			WriteText(pData, "m_ucTaskID");
			WriteValue(pData, 5350);
			WriteText(pData, "m_uiAwardMoveNums");
			WriteValue(pData, 1);
			WriteText(pData, "closeDate");
			WriteValue(pData, 20241106);
			WriteText(pData, "m_uiProcessRate");
			WriteValue(pData, 1);
			WriteText(pData, "m_uiTaskFinishNums");
			WriteValue(pData, 1);
			WriteText(pData, "openDate");
			WriteValue(pData, 20241001);
			WriteText(pData, "ExecutePeriod");
			WriteValue(pData, 1);
			WriteText(pData, "receiveStatus");
			WriteValue(pData, 0);
			WriteText(pData, "m_uiAlreadyAwardNums");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 5351);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 20241106);
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 5352);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 20241106);
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 5353);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 20241106);
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 45);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 60);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 0);
			WriteText(pData, "chouJiangProcessAward");
			WriteHex(pData, "FD000001c5");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000085");//固定长度
			WriteText(pData, "m_iAvailPeriod");
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 0);
			WriteText(pData, "m_iItemID");
			WriteValue(pData, 146277);
			WriteText(pData, "m_iItemNum");
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteText(pData, "ProcessValue");
			WriteValue(pData, 12);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 25);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 146294);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 24);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 25);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 146625);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 36);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 25);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 146304);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 48);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_DaZaoWorkshop_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 31;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD0000097a");//固定长度
			WriteText(pData, "ExchangeScoreByDraw");
			WriteValue(pData, 1);
			WriteText(pData, "TimeTaskTable");
			WriteHex(pData, "FD0000009b");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000008d");//固定长度
			WriteText(pData, "m_ucTaskID");
			WriteValue(pData, 4963);
			WriteText(pData, "m_uiTaskFinishNums");
			WriteValue(pData, 2);
			WriteText(pData, "openDate");
			WriteValue(pData, 20241001);
			WriteText(pData, "ExecutePeriod");
			WriteValue(pData, 1);
			WriteText(pData, "closeDate");
			WriteValue(pData, 20241007);
			WriteText(pData, "m_uiAwardMoveNums");
			WriteValue(pData, 1);
			WriteText(pData, "itemnum_task");
			WriteValue(pData, 41);
			WriteText(pData, "TaskTable");
			WriteHex(pData, "FD000002d6");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000093");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 5203);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 20241208);
			WriteText(pData, "m_uiProcessRate");
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteText(pData, "receiveStatus");
			WriteValue(pData, 0);
			WriteText(pData, "m_uiAlreadyAwardNums");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 5204);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 20241208);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 5205);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 20241208);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 80);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 80);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 1);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 5206);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 15);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 20241208);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 80);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1000);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 0);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 4961);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 20241208);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 0);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 4962);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 20241208);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 20241001);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 0);
			WriteText(pData, "BaseCfg");
			WriteHex(pData, "FD00000411");//固定长度
			WriteText(pData, "ThemeID");
			WriteValue(pData, 4);
			WriteText(pData, "HighProbEnd");
			WriteValue(pData, 1728316799);
			WriteText(pData, "ChongNengCfg");
			WriteHex(pData, "FD000000e1");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000005d");//固定长度
			WriteText(pData, "CostItemNum");
			WriteHex(pData, "FD00000024");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteValue(pData, 1);
			WriteText(pData, "ProcessIncProp");
			WriteHex(pData, "FD00000012");//固定长度
			WriteValue(pData, 10);
			WriteValue(pData, 10000);
			WriteValue(pData, 10);
			WriteHex(pData, "FD00000068");//固定长度
			WriteKeyIndex(pData, 17);
			WriteHex(pData, "FD00000024");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 10);
			WriteValue(pData, 2);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "FD00000036");//固定长度
			WriteValue(pData, 200);
			WriteValue(pData, 25);
			WriteValue(pData, 100);
			WriteValue(pData, 1000);
			WriteValue(pData, 150);
			WriteValue(pData, 50);
			WriteText(pData, "SeasonID");
			WriteValue(pData, 14);
			WriteText(pData, "HighProbBegin");
			WriteValue(pData, 1727712000);
			WriteText(pData, "DateBeginStr");
			WriteText(pData, "2024-10-01 0-0-0");
			WriteText(pData, "HighProbEndStr");
			WriteText(pData, "2024-10-07 23-59-59");
			WriteText(pData, "DateEndStr");
			WriteText(pData, "2024-12-15 23-59-59");
			WriteText(pData, "prob_version");
			WriteValue(pData, 0);
			WriteText(pData, "DateMiddle");
			WriteValue(pData, 1733673599);
			WriteText(pData, "HighProbBeginStr");
			WriteKeyIndex(pData, 22);
			WriteText(pData, "Version2");
			WriteHex(pData, "FD0000013d");//固定长度
			WriteText(pData, "BoxEndTimeStr");
			WriteKeyIndex(pData, 24);
			WriteText(pData, "CarAItemID");
			WriteValue(pData, 146565);
			WriteText(pData, "CarAPeriod");
			WriteValue(pData, 1680);
			WriteText(pData, "PermenantCfg");
			WriteHex(pData, "FD00000025");//固定长度
			WriteText(pData, "TotalScore");
			WriteValue(pData, 3050);
			WriteText(pData, "Ratio");
			WriteValue(pData, 10);
			WriteText(pData, "BoxEndTime");
			WriteValue(pData, 1728316799);
			WriteText(pData, "ZhuanHuanCfg");
			WriteHex(pData, "FD00000064");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000000b");//固定长度
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 168);
			WriteValue(pData, 2);
			WriteHex(pData, "FD0000000b");//固定长度
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 360);
			WriteValue(pData, 3);
			WriteHex(pData, "FD0000000b");//固定长度
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 720);
			WriteValue(pData, 0);
			WriteHex(pData, "FD0000000b");//固定长度
			WriteKeyIndex(pData, 33);
			WriteValue(pData, 72);
			WriteText(pData, "BoxItemID");
			WriteValue(pData, 112674);
			WriteText(pData, "CardPeriod");
			WriteValue(pData, 1680);
			WriteText(pData, "CardItemID");
			WriteValue(pData, 146364);
			WriteText(pData, "ItemID_Array");
			WriteHex(pData, "FD00000024");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 146363);
			WriteValue(pData, 2);
			WriteValue(pData, 146362);
			WriteText(pData, "DateMiddleStr");
			WriteText(pData, "2024-12-08 23-59-59");
			WriteText(pData, "DateBegin");
			WriteValue(pData, 1727712000);
			WriteText(pData, "Exchange_Score");
			WriteValue(pData, 1);
			WriteText(pData, "DateEnd");
			WriteValue(pData, 1734278399);
			WriteText(pData, "chongneng_cnt");
			WriteValue(pData, 50);
			WriteText(pData, "ChongNengOnce_ProcessIncNum");
			WriteValue(pData, 10);
			WriteText(pData, "ExchangeScore");
			WriteValue(pData, 0);
			WriteText(pData, "V2Info");
			WriteHex(pData, "FD000000cf");//固定长度
			WriteText(pData, "alreadyOP");
			WriteValue(pData, 0);
			WriteText(pData, "Zhuanhuan_Period");
			WriteValue(pData, 72);
			WriteText(pData, "GotPermenant_NeedItem");
			WriteHex(pData, "FD00000064");//固定长度
			WriteValue(pData, 146363);
			WriteHex(pData, "FD00000024");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 291);
			WriteValue(pData, 2);
			WriteValue(pData, 305);
			WriteValue(pData, 146362);
			WriteHex(pData, "FD00000024");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 264);
			WriteValue(pData, 2);
			WriteValue(pData, 305);
			WriteText(pData, "OpStatus_HuodongBegin");
			WriteValue(pData, 1);
			WriteText(pData, "ProcessTotal");
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 39);
			WriteValue(pData, 112674);
			WriteText(pData, "itemid_task");
			WriteValue(pData, 146362);
			WriteKeyIndex(pData, 37);
			WriteValue(pData, 1728316799);
			WriteText(pData, "step1_resultid");
			WriteValue(pData, 0);
			WriteText(pData, "itemnum");
			WriteValue(pData, 14);
			WriteText(pData, "itemid");
			WriteValue(pData, 146363);
			WriteText(pData, "CurrentWeekActivity");
			WriteValue(pData, 80);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta72ReqGodOfWarBoxCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 29;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000a66");//固定长度
			WriteText(pData, "CurrentMulAwardIndex");
			WriteValue(pData, 0);
			WriteText(pData, "discountCfg");
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "numB");
			WriteValue(pData, 12);
			WriteText(pData, "iOpenTime");
			WriteValue(pData, 0);
			WriteText(pData, "roundTriggerA");
			WriteValue(pData, 10);
			WriteText(pData, "GodWarBoxReceiveStatus");
			WriteValue(pData, 0);
			WriteText(pData, "iCloseTime");
			WriteValue(pData, 235959);
			WriteText(pData, "TreasureAwardHasGotStatus");
			WriteValue(pData, 0);
			WriteText(pData, "TriggerNumN");
			WriteValue(pData, 4);
			WriteText(pData, "iCloseDate");
			WriteValue(pData, 20241111);
			WriteText(pData, "GodWarBoxTriggerNum");
			WriteValue(pData, 0);
			WriteText(pData, "currCostItemNum");
			WriteValue(pData, 1);
			WriteText(pData, "AwardCfg");
			WriteHex(pData, "FD00000838");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000c8");//固定长度
			WriteText(pData, "itemID");
			WriteHex(pData, "FD00000057");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000049");//固定长度
			WriteText(pData, "itemNum");
			WriteValue(pData, 1);
			WriteText(pData, "item_type");
			WriteValue(pData, 1);
			WriteText(pData, "id");
			WriteValue(pData, 120646);
			WriteText(pData, "availperiod");
			WriteValue(pData, -1);
			WriteText(pData, "alread_got");
			WriteHex(pData, "F9");//F9
			WriteText(pData, "item_index");
			WriteValue(pData, 1);
			WriteText(pData, "fenjie_num");
			WriteValue(pData, 450);
			WriteText(pData, "conpousNum");
			WriteValue(pData, 0);
			WriteText(pData, "superMoneyNum");
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 146547);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 450);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 66598);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 175);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 95683);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 115);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 102100);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 90);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 146445);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 50);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 7);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 147595);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 7);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 27);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 8);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 145375);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 8);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 20);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 9);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 134321);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 9);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 17);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 10);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 147587);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 11);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 145367);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 11);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 12);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 134313);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 12);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 13);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 146389);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 13);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 14);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 69938);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 14);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 15);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 21977);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 15);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 16);
			WriteHex(pData, "FD00000070");//固定长度
			WriteKeyIndex(pData, 13);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 96593);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 18);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 16);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteText(pData, "TotaltreasureAwardNum");
			WriteValue(pData, 0);
			WriteText(pData, "currRoundNum");
			WriteValue(pData, 1);
			WriteText(pData, "theme_idx");
			WriteValue(pData, 12);
			WriteText(pData, "callon_item_id");
			WriteValue(pData, 115241);
			WriteText(pData, "ZanCunItemList");
			WriteHex(pData, "FD00000056");//固定长度
			WriteText(pData, "m_sItemNum");
			WriteValue(pData, 0);
			WriteText(pData, "m_iSuperMoney");
			WriteValue(pData, 0);
			WriteText(pData, "m_iCoupons");
			WriteValue(pData, 0);
			WriteText(pData, "m_astItemInfo");
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "SeasonId");
			WriteValue(pData, 15);
			WriteText(pData, "iOpenDate");
			WriteValue(pData, 20241001);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

NOINLINE void Request_Beta91_Silk_Road_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 30;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD000004ab");//固定长度
			WriteText(pData, "playerInfo");
			WriteHex(pData, "FD00000139");//固定长度
			WriteText(pData, "silkRoadRewardStatus");
			WriteHex(pData, "FD00000036");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteValue(pData, 1);
			WriteText(pData, "todayFinishNum");
			WriteValue(pData, 0);
			WriteText(pData, "newRoad");
			WriteHex(pData, "FD0000002a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000000");//固定长度
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000000");//固定长度
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "silkRoadProcess");
			WriteHex(pData, "FD00000036");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteValue(pData, 2);
			WriteValue(pData, 4);
			WriteValue(pData, 3);
			WriteValue(pData, 3);
			WriteText(pData, "firstTimeTable");
			WriteHex(pData, "FD00000036");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteValue(pData, 0);
			WriteText(pData, "roadLength");
			WriteHex(pData, "FD00000036");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteValue(pData, 2);
			WriteValue(pData, 4);
			WriteValue(pData, 3);
			WriteValue(pData, 3);
			WriteText(pData, "baseCfg");
			WriteHex(pData, "FD0000007f");//固定长度
			WriteText(pData, "SeasonID");
			WriteValue(pData, 1);
			WriteText(pData, "ThemeID");
			WriteValue(pData, 1);
			WriteText(pData, "DayTaskNum");
			WriteValue(pData, 2);
			WriteText(pData, "endTime");
			WriteValue(pData, 1730908799);
			WriteText(pData, "beginTime");
			WriteValue(pData, 1726070400);
			WriteText(pData, "TaskContent");
			WriteText(pData, "任意完成一局游戏");
			WriteText(pData, "roadProcessAward");
			WriteHex(pData, "FD000001c4");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000c0");//固定长度
			WriteText(pData, "type");
			WriteValue(pData, 1);
			WriteText(pData, "item");
			WriteHex(pData, "FD00000068");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000005a");//固定长度
			WriteText(pData, "m_iAvailPeriod");
			WriteValue(pData, 0);
			WriteText(pData, "m_ushItemType");
			WriteValue(pData, 0);
			WriteText(pData, "m_iItemNum");
			WriteValue(pData, 1);
			WriteText(pData, "m_iItemID");
			WriteValue(pData, 146373);
			WriteText(pData, "m_iSuperMoney");
			WriteValue(pData, 0);
			WriteText(pData, "m_iCoupons");
			WriteValue(pData, 0);
			WriteText(pData, "roadId");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD0000006d");//固定长度
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 17);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 18);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 146573);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 2);
			WriteValue(pData, 3);
			WriteHex(pData, "FD0000006d");//固定长度
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 17);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 18);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 144281);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 3);
			WriteText(pData, "dayTaskAward");
			WriteHex(pData, "FD0000009f");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 888);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000062");//固定长度
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 17);
			WriteHex(pData, "FD0000003a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 146358);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 23);
			WriteValue(pData, 0);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

NOINLINE void Request_Beta87_Dragon_City_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 32;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000804");//固定长度
			WriteText(pData, "AwardCfg");
			WriteHex(pData, "FD00000641");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000b1");//固定长度
			WriteText(pData, "m_iAvailPeriod");
			WriteValue(pData, -1);
			WriteText(pData, "m_ushItemType");
			WriteValue(pData, 1);
			WriteText(pData, "m_iSuperMoney");
			WriteValue(pData, 0);
			WriteText(pData, "m_iItemID");
			WriteValue(pData, 81293);
			WriteText(pData, "m_usShowPriority");
			WriteValue(pData, 1);
			WriteText(pData, "m_iCoupons");
			WriteValue(pData, 0);
			WriteText(pData, "m_bAlreadGot");
			WriteHex(pData, "F9");//F9
			WriteText(pData, "m_iItemNum");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 146548);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 88683);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 116737);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 123400);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 147679);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 7);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 147739);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 8);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 132313);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 9);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 147671);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 10);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 147731);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 11);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 132308);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 12);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 146387);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 13);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 146410);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 14);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 146428);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 15);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 147901);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteValue(pData, 16);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 147907);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 7);
			WriteHex(pData, "F9");//F9
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 1);
			WriteText(pData, "PlayerInfo");
			WriteHex(pData, "FD000000b3");//固定长度
			WriteText(pData, "discountCostItemNum");
			WriteValue(pData, 0);
			WriteText(pData, "costItemNum");
			WriteValue(pData, 1);
			WriteText(pData, "drawItemId");
			WriteValue(pData, 144339);
			WriteText(pData, "discountSplSec");
			WriteValue(pData, 0);
			WriteText(pData, "discountFlag");
			WriteHex(pData, "F9");//F9
			WriteText(pData, "discountPercent");
			WriteValue(pData, 0);
			WriteText(pData, "drawItemCnt");
			WriteValue(pData, 0);
			WriteText(pData, "curDraw");
			WriteValue(pData, 0);
			WriteText(pData, "BaseCfg");
			WriteHex(pData, "FD0000008c");//固定长度
			WriteText(pData, "closeTime");
			WriteValue(pData, 235959);
			WriteText(pData, "freezingDate");
			WriteValue(pData, 20241118);
			WriteText(pData, "openDate");
			WriteValue(pData, 20241019);
			WriteText(pData, "theme_idx");
			WriteValue(pData, 2);
			WriteText(pData, "closeDate");
			WriteValue(pData, 20241117);
			WriteText(pData, "openTime");
			WriteValue(pData, 0);
			WriteText(pData, "seasonId");
			WriteValue(pData, 2);
			WriteText(pData, "BoxInfo");
			WriteHex(pData, "FD00000048");//固定长度
			WriteText(pData, "MinDrawCount");
			WriteValue(pData, 12);
			WriteText(pData, "AccumulateCount");
			WriteValue(pData, 0);
			WriteText(pData, "SplDrawCount");
			WriteValue(pData, 12);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

NOINLINE void Request_Beta87_Dragon_City_AwardOpt(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 33;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue(Body);
	int intValue1 = static_cast<UINT>(readValue1);
	int intValue2 = static_cast<UINT>(readValue2);

	if (intValue2 == 5) {
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len
			{
				BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
				WriteValue(pData, 0);
				WriteHex(pData, "FD00000103");//固定长度
				WriteValue(pData, 1);
				WriteHex(pData, "FD000000f5");//固定长度
				WriteText(pData, "maxGetNum");
				WriteValue(pData, 1);
				WriteText(pData, "iCostSpeedCoin");
				WriteValue(pData, 100);
				WriteText(pData, "item");
				WriteHex(pData, "FD00000068");//固定长度
				WriteValue(pData, 1);
				WriteHex(pData, "FD0000005a");//固定长度
				WriteText(pData, "m_iAvailPeriod");
				WriteValue(pData, 0);
				WriteText(pData, "m_ushItemType");
				WriteValue(pData, 0);
				WriteText(pData, "m_iItemNum");
				WriteValue(pData, 10);
				WriteText(pData, "m_iItemID");
				WriteValue(pData, 144339);
				WriteText(pData, "status");
				WriteValue(pData, 1);
				WriteText(pData, "getNum");
				WriteValue(pData, 0);
				WriteText(pData, "specialFlag");
				WriteValue(pData, 1);
				WriteText(pData, "iCostBaseSpeedCoin");
				WriteValue(pData, 200);
				WriteBodyHeadEnd(pBody, pData);
			}
			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}
		len = p - buf.data();
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}
	else if (intValue2 == 3) {
		std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
		//Head
		WriteRequestHead(p, Client->Uin);
		{ //Body
			BYTE* pBody = p;
			Write16(pBody, 0); //len
			{
				BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
				WriteValue(pData, 0);
				WriteHex(pData, "FD00000056");//固定长度
				WriteText(pData, "m_sItemNum");
				WriteValue(pData, 0);
				WriteText(pData, "m_iSuperMoney");
				WriteValue(pData, 0);
				WriteText(pData, "m_iCoupons");
				WriteValue(pData, 0);
				WriteText(pData, "m_astItemInfo");
				WriteHex(pData, "FD00000000");//固定长度
				WriteBodyHeadEnd(pBody, pData);
			}
			len = pBody - p;
			Set16(p, (WORD)len);
			p += len;
		}
		len = p - buf.data();
		SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
	}


}

NOINLINE void Request_RequestBeta90_SkinTreasureCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 33;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00001616");//固定长度
			WriteText(pData, "iTotalDrawNum");
			WriteValue(pData, 0);
			WriteText(pData, "award_poll_cfg");
			WriteHex(pData, "FD00001473");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000bc");//固定长度
			WriteText(pData, "itemNum");
			WriteValue(pData, 1);
			WriteText(pData, "itemType");
			WriteValue(pData, 3);
			WriteText(pData, "iAvailPeriod");
			WriteValue(pData, 0);
			WriteText(pData, "iCanExchange");
			WriteValue(pData, 0);
			WriteText(pData, "iOtherAttribute");
			WriteValue(pData, 0);
			WriteText(pData, "baoji_type");
			WriteValue(pData, 2);
			WriteText(pData, "itemId");
			WriteValue(pData, 144343);
			WriteText(pData, "iProbability");
			WriteValue(pData, 5);
			WriteText(pData, "iPollId");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD0000007f");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 129099);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteText(pData, "iExchangePieceNum");
			WriteValue(pData, 1500);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 3);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 129099);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 100);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 4);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 106673);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1500);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 5);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 106673);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 150);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 6);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 86998);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1500);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 7);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 86998);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 150);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 8);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 91957);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1500);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 9);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 91957);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 150);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 10);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 81293);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1500);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 11);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 81293);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 150);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 12);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 113417);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 13);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 113417);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1200);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 14);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 96788);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 15);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 96788);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1300);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 16);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 97535);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 17);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 97535);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1300);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 18);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 144343);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 19);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98264);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 20);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98264);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1300);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 21);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98265);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 22);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98265);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1300);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 23);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98266);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 24);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98266);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1300);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 25);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98267);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 26);
			WriteHex(pData, "FD0000006e");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 168);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 98267);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1300);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteValue(pData, 27);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 40000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 28);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 20000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 29);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 30);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 3000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 31);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 1000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 32);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 300);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 33);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 100);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 146232);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 100);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 34);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 21978);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 5750);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 35);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 21983);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 9000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 36);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 21989);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 9000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 37);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 21977);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 9000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 38);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 65025);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 39);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 65031);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 40);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 65028);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 41);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 12376);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 42);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 12377);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 43);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 12378);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 44);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 12380);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteValue(pData, 45);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 2);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 3);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 5);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 6);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 96597);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 14000);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 0);
			WriteText(pData, "iOpenTime");
			WriteValue(pData, 0);
			WriteText(pData, "RetRecItemTable");
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "iCloseTime");
			WriteValue(pData, 235959);
			WriteText(pData, "score_discount_cfg");
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "UseDiscountNum");
			WriteValue(pData, 0);
			WriteText(pData, "iLeftDrawNum");
			WriteValue(pData, 300);
			WriteText(pData, "iCostNumOneTime");
			WriteValue(pData, 1);
			WriteText(pData, "RetDecpItemTable");
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "iExchangePieceItemID");
			WriteValue(pData, 94880);
			WriteText(pData, "iCostItemID");
			WriteValue(pData, 144342);
			WriteText(pData, "RetComItemTable");
			WriteHex(pData, "FD00000000");//固定长度
			WriteText(pData, "iRealSkinId");
			WriteValue(pData, 2);
			WriteText(pData, "iCloseDate");
			WriteValue(pData, 20241031);
			WriteText(pData, "iCaostNumTenTime");
			WriteValue(pData, 10);
			WriteText(pData, "coupons_index");
			WriteValue(pData, 0);
			WriteText(pData, "iOpenDate");
			WriteValue(pData, 20241019);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta77_ReqNiChangPavilionCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 34;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteValue(pData, 1);
			WriteHex(pData, "FD000006b3");//固定长度
			WriteText(pData, "base_cfg");
			WriteHex(pData, "FD0000004e");//固定长度
			WriteText(pData, "closeTime");
			WriteValue(pData, 235959);
			WriteText(pData, "openDate");
			WriteValue(pData, 20220714);
			WriteText(pData, "closeDate");
			WriteValue(pData, 20330714);
			WriteText(pData, "openTime");
			WriteValue(pData, 1);
			WriteText(pData, "ni_chang_item_info");
			WriteHex(pData, "FD0000063d");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000001d4");//固定长度
			WriteText(pData, "coloration_cfg");
			WriteHex(pData, "FD00000161");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000068");//固定长度
			WriteText(pData, "coloration_id");
			WriteValue(pData, 1);
			WriteText(pData, "coloration_item_normal_cost");
			WriteValue(pData, 124648);
			WriteText(pData, "coloration_item_normal_cost_num");
			WriteValue(pData, 10);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteText(pData, "item_sort_type");
			WriteValue(pData, 1);
			WriteText(pData, "item_on_sale");
			WriteValue(pData, 1);
			WriteText(pData, "item_sort_classify");
			WriteValue(pData, 3);
			WriteText(pData, "itemId");
			WriteValue(pData, 99060);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000161");//固定长度
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "FD0000011a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteText(pData, "sub_item_list");
			WriteHex(pData, "FD00000000");//固定长度
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 13);
			WriteValue(pData, 99070);
			WriteValue(pData, 3);
			WriteHex(pData, "FD0000014d");//固定长度
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "FD0000011a");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 13);
			WriteValue(pData, 123003);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000183");//固定长度
			WriteKeyIndex(pData, 6);
			WriteHex(pData, "FD00000149");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 4);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 5);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 6);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteValue(pData, 7);
			WriteHex(pData, "FD00000021");//固定长度
			WriteKeyIndex(pData, 7);
			WriteValue(pData, 7);
			WriteKeyIndex(pData, 8);
			WriteValue(pData, 124648);
			WriteKeyIndex(pData, 9);
			WriteValue(pData, 10);
			WriteKeyIndex(pData, 10);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 11);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 14);
			WriteHex(pData, "FD00000000");//固定长度
			WriteKeyIndex(pData, 12);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 13);
			WriteValue(pData, 123013);
			WriteHex(pData, "FD00000000");//固定长度
			WriteHex(pData, "FD00000000");//固定长度
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


NOINLINE void Request_Beta91_Xinjiang_MainStation_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 41;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000418");//固定长度
			WriteText(pData, "huodongCfg");
			WriteHex(pData, "FD0000025e");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD00000083");//固定长度
			WriteText(pData, "EndTime");
			WriteValue(pData, 1728316799);
			WriteText(pData, "DateEndStr");
			WriteText(pData, "2024-10-07 23-59-59");
			WriteText(pData, "BeginTime");
			WriteValue(pData, 1726761600);
			WriteText(pData, "DateBeginStr");
			WriteText(pData, "2024-09-20 00-00-00");
			WriteText(pData, "HuodongName");
			WriteText(pData, "心想事成屋");
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000054");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 1730908799);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "2024-11-06 23-59-59");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 1726070400);
			WriteKeyIndex(pData, 5);
			WriteText(pData, "2024-09-12 00-00-00");
			WriteKeyIndex(pData, 7);
			WriteText(pData, "点亮丝绸之路");
			WriteValue(pData, 3);
			WriteHex(pData, "FD0000002c");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 1730908799);
			WriteKeyIndex(pData, 2);
			WriteKeyIndex(pData, 9);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 1726070400);
			WriteKeyIndex(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteKeyIndex(pData, 7);
			WriteText(pData, "冲刺大闯关");
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000050");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 1733673599);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "2024-12-08 23-59-59");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 1727712000);
			WriteKeyIndex(pData, 5);
			WriteText(pData, "2024-10-01 00-00-00");
			WriteKeyIndex(pData, 7);
			WriteText(pData, "筑梦合约");
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000055");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 1730735999);
			WriteKeyIndex(pData, 2);
			WriteText(pData, "2024-11-04 23-59-59");
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 1726416000);
			WriteKeyIndex(pData, 5);
			WriteText(pData, "2024-09-16 00-00-00");
			WriteKeyIndex(pData, 7);
			WriteText(pData, "S45主题排位赛");
			WriteValue(pData, 6);
			WriteHex(pData, "FD0000002a");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 1730908799);
			WriteKeyIndex(pData, 2);
			WriteKeyIndex(pData, 9);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 1727712000);
			WriteKeyIndex(pData, 5);
			WriteKeyIndex(pData, 14);
			WriteKeyIndex(pData, 7);
			WriteText(pData, "天山音韵");
			WriteValue(pData, 7);
			WriteHex(pData, "FD0000002a");//固定长度
			WriteKeyIndex(pData, 1);
			WriteValue(pData, 1728316799);
			WriteKeyIndex(pData, 2);
			WriteKeyIndex(pData, 3);
			WriteKeyIndex(pData, 4);
			WriteValue(pData, 1726070400);
			WriteKeyIndex(pData, 5);
			WriteKeyIndex(pData, 10);
			WriteKeyIndex(pData, 7);
			WriteText(pData, "万里行疆");
			WriteText(pData, "baseCfg");
			WriteHex(pData, "FD00000026");//固定长度
			WriteText(pData, "endTime");
			WriteValue(pData, 1730908799);
			WriteText(pData, "beginTime");
			WriteValue(pData, 1726070400);
			WriteText(pData, "Award");
			WriteHex(pData, "FD000000bd");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000af");//固定长度
			WriteText(pData, "type");
			WriteValue(pData, 1);
			WriteText(pData, "item");
			WriteHex(pData, "FD00000068");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000005a");//固定长度
			WriteText(pData, "m_iAvailPeriod");
			WriteValue(pData, 0);
			WriteText(pData, "m_ushItemType");
			WriteValue(pData, 0);
			WriteText(pData, "m_iItemNum");
			WriteValue(pData, 1);
			WriteText(pData, "m_iItemID");
			WriteValue(pData, 146357);
			WriteText(pData, "m_iSuperMoney");
			WriteValue(pData, 0);
			WriteText(pData, "m_iCoupons");
			WriteValue(pData, 0);
			WriteText(pData, "playerInfo");
			WriteHex(pData, "FD0000009b");//固定长度
			WriteText(pData, "mainStationStatusTable");
			WriteHex(pData, "FD0000007e");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteValue(pData, 0);
			WriteValue(pData, 5);
			WriteValue(pData, 0);
			WriteValue(pData, 6);
			WriteValue(pData, 0);
			WriteValue(pData, 7);
			WriteValue(pData, 0);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void safeDelete(char*& ptr) {
	if (ptr) {
		delete ptr;
		ptr = nullptr;
	}
}

NOINLINE void Request_CarLibrary_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	Body += 28;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;


	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000364");//固定长度
			WriteText(pData, "CollectValueInLevelTotal");
			WriteValue(pData, 50000);
			WriteText(pData, "CollectValue");
			WriteValue(pData, 1000);
			WriteText(pData, "PosInfo");
			WriteHex(pData, "FD000000c6");//固定长度
			for (size_t i = 0; i < Client->MyKartLibrary.size(); i++)
			{
				WriteValue(pData, i + 1);
				WriteValue(pData, Client->MyKartLibrary[i]);
			}
			char* T1S = G2U("T1机甲赛车");
			char* T2S = G2U("T2机甲赛车");
			char* T3S = G2U("T3机甲赛车");
			char* AS = G2U("A级赛车");
			char* SS = G2U("S级赛车");
			char* Suit = G2U("赛车皮肤");

			WriteText(pData, "HistoryHighestCollectValue");
			WriteValue(pData, 1000);
			WriteText(pData, "T1num");
			WriteValue(pData, SelectMyKartLibarayNum(Client->Uin, T1S));
			WriteText(pData, "T2num");
			WriteValue(pData, SelectMyKartLibarayNum(Client->Uin, T2S));
			WriteText(pData, "T3num");
			WriteValue(pData, SelectMyKartLibarayNum(Client->Uin, T3S));
			WriteText(pData, "FlagValue");
			WriteHex(pData, "FD00000036");//固定长度
			WriteValue(pData, 29);
			WriteValue(pData, 1);
			WriteValue(pData, 27);
			WriteValue(pData, 1);
			WriteValue(pData, 28);
			WriteValue(pData, 1);
			WriteText(pData, "Suitnum");
			WriteValue(pData, SelectMyKartLibarayNum(Client->Uin, Suit));
			WriteText(pData, "BaseCfg");
			WriteHex(pData, "FD0000010c");//固定长度
			WriteText(pData, "AllowPlaceTPos");
			WriteHex(pData, "FD0000005a");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 7);
			WriteValue(pData, 2);
			WriteValue(pData, 8);
			WriteValue(pData, 3);
			WriteValue(pData, 9);
			WriteValue(pData, 4);
			WriteValue(pData, 10);
			WriteValue(pData, 5);
			WriteValue(pData, 11);
			WriteText(pData, "SnumTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(SS));
			WriteText(pData, "MaxCollectValueLevel");
			WriteValue(pData, 10);
			WriteText(pData, "SuitnumTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(Suit));
			WriteText(pData, "T2numTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(T2S));
			WriteText(pData, "T1numTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(T1S));
			WriteText(pData, "AnumTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(AS));
			WriteText(pData, "T3numTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(T3S));
			WriteText(pData, "CollectValueLevel");
			WriteValue(pData, 10);
			WriteText(pData, "Anum");
			WriteValue(pData, SelectMyKartLibarayNum(Client->Uin, AS));
			WriteText(pData, "ForbidShow");
			WriteValue(pData, 0);
			WriteText(pData, "other");
			WriteValue(pData, 15);
			WriteText(pData, "ShowNum");
			WriteValue(pData, 11);
			WriteText(pData, "CollectValueInLevel");
			WriteValue(pData, 50000);
			WriteText(pData, "Snum");
			WriteValue(pData, SelectMyKartLibarayNum(Client->Uin, SS));
			WriteBodyHeadEnd(pBody, pData);
			safeDelete(T1S);
			safeDelete(T2S);
			safeDelete(T3S);
			safeDelete(AS);
			safeDelete(SS);
			safeDelete(Suit);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);


}

NOINLINE void Request_CarLibraryOp(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.3) return;
	Body += 18;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue(Body);
	double readValue3 = ReadValue(Body);
	double readValue4 = ReadValue(Body);
	int intValue1 = static_cast<UINT>(readValue1);
	int intValue2 = static_cast<UINT>(readValue2);
	int intValue3 = static_cast<UINT>(readValue3);
	int intValue4 = static_cast<UINT>(readValue4);
	if (intValue4 - 1 < Client->MyKartLibrary.size() && intValue4 - 1 >= 0) { // 检查下标是否有效
		Client->MyKartLibrary[intValue4 - 1] = intValue3;
		PlayerDB_UpdateBaseInfoKartLibaray(Client);
	}


	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000000");//固定长度
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_CarLibrary_QueryOtherData(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.3) return;
	Body += 31;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue(Body);
	
	int intValue1 = static_cast<UINT>(readValue1);
	int intValue2 = static_cast<UINT>(readValue2);
	std::vector<UINT> myKart;
	PlayerDB_SelectBaseInfoKartLibaray(intValue2, myKart);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD0000031e");//固定长度
			WriteText(pData, "CollectValueInLevelTotal");
			WriteValue(pData, 50000);
			WriteText(pData, "CollectValue");
			WriteValue(pData, 1000);
			WriteText(pData, "PosInfo");
			WriteHex(pData, "FD000000c6");//固定长度
			for (size_t i = 0; i < myKart.size(); i++)
			{
				WriteValue(pData, i + 1);
				WriteValue(pData, myKart[i]);
			}
			char* T1S = G2U("T1机甲赛车");
			char* T2S = G2U("T2机甲赛车");
			char* T3S = G2U("T3机甲赛车");
			char* AS = G2U("A级赛车");
			char* SS = G2U("S级赛车");
			char* Suit = G2U("赛车皮肤");


			WriteText(pData, "HistoryHighestCollectValue");
			WriteValue(pData, 1000);
			WriteText(pData, "T1num");
			WriteValue(pData, SelectMyKartLibarayNum(intValue2, T1S));
			WriteText(pData, "T2num");
			WriteValue(pData, SelectMyKartLibarayNum(intValue2, T2S));
			WriteText(pData, "T3num");
			WriteValue(pData, SelectMyKartLibarayNum(intValue2, T3S));

			WriteText(pData, "Suitnum");
			WriteValue(pData, SelectMyKartLibarayNum(intValue2, Suit));
			WriteText(pData, "BaseCfg");
			WriteHex(pData, "FD0000010c");//固定长度
			WriteText(pData, "AllowPlaceTPos");
			WriteHex(pData, "FD0000005a");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 7);
			WriteValue(pData, 2);
			WriteValue(pData, 8);
			WriteValue(pData, 3);
			WriteValue(pData, 9);
			WriteValue(pData, 4);
			WriteValue(pData, 10);
			WriteValue(pData, 5);
			WriteValue(pData, 11);
			WriteText(pData, "SnumTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(SS));
			WriteText(pData, "MaxCollectValueLevel");
			WriteValue(pData, 10);
			WriteText(pData, "SuitnumTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(Suit));
			WriteText(pData, "T2numTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(T2S));
			WriteText(pData, "T1numTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(T1S));
			WriteText(pData, "AnumTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(AS));
			WriteText(pData, "T3numTotal");
			WriteValue(pData, SelectMyKartLibarayAllNum(T3S));
			WriteText(pData, "CollectValueLevel");
			WriteValue(pData, 10);
			WriteText(pData, "Anum");
			WriteValue(pData, SelectMyKartLibarayNum(intValue2, AS));
			WriteText(pData, "ForbidShow");
			WriteValue(pData, 0);
			WriteText(pData, "other");
			WriteValue(pData, 15);
			WriteText(pData, "ShowNum");
			WriteValue(pData, 11);
			WriteText(pData, "CollectValueInLevel");
			WriteValue(pData, 50000);
			WriteText(pData, "Snum");
			WriteValue(pData, SelectMyKartLibarayNum(intValue2, SS));
			WriteBodyHeadEnd(pBody, pData);
			safeDelete(T1S);
			safeDelete(T2S);
			safeDelete(T3S);
			safeDelete(AS);
			safeDelete(SS);
			safeDelete(Suit);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



NOINLINE void Request_LimitRecommend_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 32;
	double readValue1 = ReadValue(Body);
	double readValue2 = ReadValue(Body);
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD000000cf");//固定长度
			WriteText(pData, "TopListOptimize");
			WriteValue(pData, 1);
			WriteText(pData, "EnableAsyncLoad");
			WriteValue(pData, 0);
			WriteText(pData, "UseOriTransLogic");
			WriteValue(pData, 1);
			WriteText(pData, "IsUrlWithSign");
			WriteValue(pData, 0);
			WriteText(pData, "IsNewRecordSnapshot");
			WriteValue(pData, 0);
			WriteText(pData, "TransAllTCP");
			WriteValue(pData, 1);
			WriteText(pData, "NetPersentPerFrame");
			WriteValue(pData, 1000);
			WriteText(pData, "Win7Enable64");
			WriteValue(pData, 1);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}


NOINLINE void Request_Beta87_GetLoginCfg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 24;
	double readValue1 = ReadValue(Body);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}



NOINLINE void Request_Power_GetStatus(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	//
	Body += 21;
	double readValue1 = ReadValue(Body);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteHex(pData, "FD000000a9");//固定长度
			WriteText(pData, "power_blevel");
			WriteValue(pData, 5);
			WriteText(pData, "name");
			WriteText(pData, "边境荣誉特权");
			WriteText(pData, "simple_desc");
			WriteText(pData, "边境对局获得额外荣誉");
			WriteText(pData, "status");
			WriteValue(pData, 0);
			WriteText(pData, "desc");
			WriteText(pData, "每局边境比赛获得额外点荣誉，享受多倍加成(初级+5，高级+10，专业+15)");
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

NOINLINE void Request_Beta91_Cinamon_Cafe_MainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{
	if (version <= 1.4) return;

	Body += 33;
	double readValue1 = ReadValue(Body);

	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
	//Head
	WriteRequestHead(p, Client->Uin);
	{ //Body
		BYTE* pBody = p;
		Write16(pBody, 0); //len
		{
			BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
			WriteValue(pData, 0);
			WriteHex(pData, "FD00000775");//固定长度
			WriteText(pData, "playerInfo");
			WriteHex(pData, "FD000001ad");//固定长度
			WriteText(pData, "choujiangProcessAwardStatusTable");
			WriteHex(pData, "FD0000006c");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteValue(pData, 0);
			WriteValue(pData, 5);
			WriteValue(pData, 0);
			WriteValue(pData, 6);
			WriteValue(pData, 0);
			WriteText(pData, "takeAwayTaskID");
			WriteValue(pData, 0);
			WriteText(pData, "choujiangItemNum");
			WriteValue(pData, 1);
			WriteText(pData, "cafeValue");
			WriteValue(pData, 0);
			WriteText(pData, "cafeStatusTable");
			WriteHex(pData, "FD0000006c");//固定长度
			WriteValue(pData, 1);
			WriteValue(pData, 0);
			WriteValue(pData, 2);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteValue(pData, 0);
			WriteValue(pData, 4);
			WriteValue(pData, 0);
			WriteValue(pData, 5);
			WriteValue(pData, 0);
			WriteValue(pData, 6);
			WriteValue(pData, 0);
			WriteText(pData, "choujiangGuranteeNum");
			WriteValue(pData, 1);
			WriteText(pData, "choujiangNum");
			WriteValue(pData, 1);
			WriteText(pData, "choujiangItemID");
			WriteValue(pData, 146371);
			WriteText(pData, "baseCfg");
			WriteHex(pData, "FD0000003d");//固定长度
			WriteText(pData, "endTime");
			WriteValue(pData, 1732982399);
			WriteText(pData, "beginTime");
			WriteValue(pData, 1730390400);
			WriteText(pData, "guaranteeNum");
			WriteValue(pData, 5);
			WriteText(pData, "dayTaskTable");
			WriteHex(pData, "FD0000023f");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD000000de");//固定长度
			WriteText(pData, "m_ucTaskID");
			WriteValue(pData, 5270);
			WriteText(pData, "m_uiAwardMoveNums");
			WriteValue(pData, 1);
			WriteText(pData, "closeDate");
			WriteValue(pData, 20241130);
			WriteText(pData, "m_uiProcessRate");
			WriteValue(pData, 1);
			WriteText(pData, "m_uiTaskFinishNums");
			WriteValue(pData, 1);
			WriteText(pData, "openDate");
			WriteValue(pData, 20241101);
			WriteText(pData, "ExecutePeriod");
			WriteValue(pData, 1);
			WriteText(pData, "receiveStatus");
			WriteValue(pData, 0);
			WriteText(pData, "m_uiAlreadyAwardNums");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 5271);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 20241130);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 20241101);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 5272);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 20241130);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 20241101);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 1);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000063");//固定长度
			WriteKeyIndex(pData, 14);
			WriteValue(pData, 5273);
			WriteKeyIndex(pData, 15);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 16);
			WriteValue(pData, 20241130);
			WriteKeyIndex(pData, 17);
			WriteValue(pData, 30);
			WriteKeyIndex(pData, 18);
			WriteValue(pData, 80);
			WriteKeyIndex(pData, 19);
			WriteValue(pData, 20241101);
			WriteKeyIndex(pData, 20);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 21);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 22);
			WriteValue(pData, 0);
			WriteText(pData, "chouJiangProcessAward");
			WriteHex(pData, "FD000002fe");//固定长度
			WriteValue(pData, 1);
			WriteHex(pData, "FD0000006b");//固定长度
			WriteText(pData, "type");
			WriteValue(pData, 1);
			WriteText(pData, "m_ushItemType");
			WriteValue(pData, 0);
			WriteText(pData, "m_iSuperMoney");
			WriteValue(pData, 1688);
			WriteText(pData, "m_iCoupons");
			WriteValue(pData, 0);
			WriteText(pData, "ProcessValue");
			WriteValue(pData, 1);
			WriteValue(pData, 2);
			WriteHex(pData, "FD00000079");//固定长度
			WriteText(pData, "m_iAvailPeriod");
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 0);
			WriteText(pData, "m_iItemID");
			WriteValue(pData, 22013);
			WriteText(pData, "m_iItemNum");
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 2);
			WriteValue(pData, 3);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 21977);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 2);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 3);
			WriteValue(pData, 4);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 29);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 21983);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 3);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 4);
			WriteValue(pData, 5);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 29);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 147937);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 5);
			WriteValue(pData, 6);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 29);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 146972);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 6);
			WriteValue(pData, 7);
			WriteHex(pData, "FD00000058");//固定长度
			WriteKeyIndex(pData, 29);
			WriteValue(pData, -1);
			WriteKeyIndex(pData, 24);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 25);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 26);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 30);
			WriteValue(pData, 146980);
			WriteKeyIndex(pData, 31);
			WriteValue(pData, 1);
			WriteKeyIndex(pData, 27);
			WriteValue(pData, 0);
			WriteKeyIndex(pData, 28);
			WriteValue(pData, 6);
			WriteBodyHeadEnd(pBody, pData);
		}
		len = pBody - p;
		Set16(p, (WORD)len);
		p += len;
	}
	len = p - buf.data();
	SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);

}

NOINLINE void Request_ConsumeScore_OpenMainDlg(std::shared_ptr<ClientNode> Client, BYTE* Body)
{

	Body += 30;
	double readValue1 = ReadValue(Body);
	int intValue = static_cast<UINT>(readValue1);;

std::vector<BYTE> buf(819200); BYTE* p = buf.data(); WORD len;
//Head
WriteRequestHead(p, Client->Uin);
{ //Body
	BYTE* pBody = p;
	Write16(pBody, 0); //len
	{
		BYTE* pData = WriteBodyHeadAb(pBody, readValue1);
		WriteValue(pData, 0);
		WriteHex(pData, "FD000001dd");//固定长度
		WriteText(pData, "ConsumeScore");
		WriteValue(pData, 1515);
		WriteText(pData, "wishvalue_cur");
		WriteValue(pData, 7);
		WriteText(pData, "OnceScore");
		WriteValue(pData, 100);
		WriteText(pData, "TenScore");
		WriteValue(pData, 1000);
		WriteText(pData, "BaseCfg");
		WriteHex(pData, "FD00000143");//固定长度
		WriteText(pData, "DateEndStr");
		WriteText(pData, "2024-12-31 23-59-59");
		WriteText(pData, "ConsumeScoreOverTipsTimeDiff");
		WriteValue(pData, 2592000);
		WriteText(pData, "ExchangeStore_DateBeginStr");
		WriteText(pData, "2024-10-01 00-00-00");
		WriteText(pData, "ConsumeScoreOverTipsTime");
		WriteValue(pData, 1733112000);
		WriteText(pData, "SeasonID");
		WriteValue(pData, 14);
		WriteText(pData, "DateBegin");
		WriteValue(pData, 1727712000);
		WriteText(pData, "ExchangeStore_DateEndStr");
		WriteKeyIndex(pData, 6);
		WriteText(pData, "ExchangeStore_DateEnd");
		WriteValue(pData, 1735660799);
		WriteText(pData, "DateEnd");
		WriteValue(pData, 1735660799);
		WriteText(pData, "ExchangeStore_DateBegin");
		WriteValue(pData, 1727712000);
		WriteText(pData, "DateBeginStr");
		WriteKeyIndex(pData, 9);
		WriteText(pData, "wishvalue_max");
		WriteValue(pData, 6000);
		WriteText(pData, "HistoryConsumeScore");
		WriteValue(pData, 150731);
		WriteBodyHeadEnd(pBody, pData);
	}
	len = pBody - p;
	Set16(p, (WORD)len);
	p += len;
}
len = p - buf.data();
SendToClient(Client, 24251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}