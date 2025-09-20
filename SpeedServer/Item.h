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
	UINT GameType; // ��Ϸģʽ��0��ͨ 1 ���� 2 �ֻ���
	std::string Mess;	   // ��ϸ����
	std::string Name;	   // ��Ʒ����
};

void LoadItemConfig();
UINT GetItemType(UINT ID);
int countItemsWithMess(const std::string& targetMess);
