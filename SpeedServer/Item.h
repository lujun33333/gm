#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
enum ItemType
{
	EAIT_Unknown,
	EAIT_CAR,
	EAIT_SKIN,
	EAIT_CLOTHES,
	EAIT_PROP,
	EAIT_PET,
	EAIT_STONE
};
struct CarTreasureInfo {
	UINT ItemId = 0;
	UINT ItemNum = 0;
	INT AvailPeriod = 0;
	INT OtherAttribute = 0;
	UINT iProbability = 0;
	UINT ItemType = 0;
	UINT ExchangePieceNum = 0;
	UINT CanExchange = 0;
};


struct CarTreasure {
	UINT MaxRate = 0;
	UINT MaxRate2 = 0;
	std::vector<CarTreasureInfo> Treasures;
	std::vector<CarTreasureInfo> JPTreasures;
	std::map<UINT, std::vector<CarTreasureInfo>> TreasureMap;
	std::map<UINT, std::vector<CarTreasureInfo>> PtTreasureMap;
};
class Item
{
public:
	UINT ID;
	UINT Type;
	UINT buyflag;
	UINT lock;
	UINT GameType; // 游戏模式（0普通 1 滑板 2 轮滑）
	std::string Mess;	   // 详细类型
	std::string Name;	   // 物品名称
};

void LoadItemConfig();
UINT GetItemType(UINT ID);
int countItemsWithMess(const std::string& targetMess);
