#include "LoadConfig.h"
#include "yaml-cpp/yaml.h"
#include "Message.h"
#include "DBConnection.h"
#include "Item.h"

extern std::shared_ptr<MAPPOOL> AllMap;
extern std::shared_ptr<MAPPOOL> BorderMap;
extern std::shared_ptr<MAPPOOL> EventMap;
extern std::shared_ptr<MAPPOOL> RankMap;
extern std::map<long, std::shared_ptr<RANDOMMAPPOOL>> RandomMapPools;
extern std::unordered_map<long, std::unique_ptr<PETITEM>> PetItems;//hashmap
extern std::shared_ptr<CarTreasure> CarTreasures;//夺宝配置
extern std::unordered_map<long, std::unique_ptr<TREASUREBOX>> TreasureBoxs;//hashmap
//extern TREASUREBOX* GameEndBox;
extern std::vector<ItemInfo> GameEndBox;
extern int GameEndBoxTime[4];
extern std::vector<ItemInfo> MagicLampItems;
extern std::shared_ptr<AuctionCfg> AuctionConfig;
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> RegisterBoxs;//hashmap
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> DayRaffleBoxs;//hashmap
extern std::unordered_map<UINT, std::unique_ptr<Item>> Items;
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicFormationBoxs;
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicExchangeShopCarBoxs;//hashmap
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicExchangeShopPetBoxs;//hashmap
extern std::unordered_map<long, std::shared_ptr<ItemInfo>> MagicExchangeShopSeatBoxs;//hashmap
extern std::map<UINT, std::shared_ptr<OpenBoxDef>> GameEndBox2Uin;
extern std::shared_ptr<MIRACLECFG> MiracleCfg;
extern int MagicCupExchange[7];
extern std::map<UINT, std::shared_ptr<AppellationInfo>> Appellations;
extern std::set<UINT> ClothesGMUins;  //只能刷衣服
extern std::set<UINT> CarGMUins;  //只能刷车
extern std::set<UINT> AdminUins;//赌注模式管理 
extern std::string ClothesGMMess;
extern std::string CarGMMess;
extern std::set<UINT> AllGMUins;//全部gm
extern std::string AllGMMess;
extern std::vector<MechTreasureInfo> mechExchangeTreasureInfo;
extern std::vector<MechTreasureInfo> mechTreasureInfo;
extern std::set<UINT> SpecialLoginUins; // 定义特殊登录用户集合

std::string removeSpaces(const std::string& str) {
	std::string noSpaces = str;
	noSpaces.erase(std::remove_if(noSpaces.begin(), noSpaces.end(), ::isspace), noSpaces.end());
	return noSpaces;
}

void LoadMechTreasureInfoConfig()
{
	try
	{
		mechExchangeTreasureInfo.clear();
		YAML::Node MechExchangeTreasureInfoNode = YAML::LoadFile(".\\GameTreasureAward\\MechTreasure\\MechExchangeTreasure.yml");
		for (size_t i = 0; i < MechExchangeTreasureInfoNode.size(); i++)
		{
			MechTreasureInfo MT;
			MT.ItemId = MechExchangeTreasureInfoNode[i]["itemid"].as<UINT>();
			MT.AvailPeriod = MechExchangeTreasureInfoNode[i]["AvailPeriod"].as<int>();
			MT.ItemNum = MechExchangeTreasureInfoNode[i]["ItemNum"].as<UINT>();
			MT.exchangeNeedNum = MechExchangeTreasureInfoNode[i]["exchangeNeedNum"].as<UINT>();
			MT.maxGetNum = MechExchangeTreasureInfoNode[i]["maxGetNum"].as<UINT>();
			MT.award_idx = MechExchangeTreasureInfoNode[i]["award_idx"].as<UINT>();
			MT.errid = MechExchangeTreasureInfoNode[i]["errid"].as<UINT>();

			MT.item_type = MechExchangeTreasureInfoNode[i]["item_type"].as<UINT>();
			MT.status = MechExchangeTreasureInfoNode[i]["status"].as<UINT>();
			MT.getnum = MechExchangeTreasureInfoNode[i]["getnum"].as<UINT>();
			mechExchangeTreasureInfo.push_back(MT);
		}

		mechTreasureInfo.clear();
		YAML::Node MechTreasureInfoNode = YAML::LoadFile(".\\GameTreasureAward\\MechTreasure\\MechTreasure.yml");
		for (size_t i = 0; i < MechTreasureInfoNode.size(); i++)
		{
			MechTreasureInfo MT;
			MT.ItemId = MechTreasureInfoNode[i]["itemid"].as<UINT>();
			MT.AvailPeriod = MechTreasureInfoNode[i]["AvailPeriod"].as<int>();
			MT.alreadyGot = MechTreasureInfoNode[i]["alreadyGot"].as<UINT>();
			MT.ItemNum = MechTreasureInfoNode[i]["itemnum"].as<UINT>();
			MT.exchangeNeedNum = MechTreasureInfoNode[i]["exchangeNeedNum"].as<UINT>();
			MT.exchangePieceNum = MechTreasureInfoNode[i]["exchangePieceNum"].as<UINT>();
			MT.item_type = MechTreasureInfoNode[i]["item_type"].as<UINT>();
			MT.award_idx = MechTreasureInfoNode[i]["award_idx"].as<UINT>();
			MT.coupons = MechTreasureInfoNode[i]["coupons"].as<UINT>();
			MT.supermoney = MechTreasureInfoNode[i]["supermoney"].as<UINT>();
			MT.merge = MechTreasureInfoNode[i]["merge"].as<UINT>();
			MT.seed = MechTreasureInfoNode[i]["seed"].as<UINT>();

			mechTreasureInfo.push_back(MT);
		}

		printf("Load MechTreasureInfo Success!\n");
	}
	catch (const std::exception&)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load MechTreasureInfo Error\n");
		//exit(0);
	}
}

void LoadAppellationConfig()
{
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\Appellations.yml");
		for (size_t i = 0; i < Config.size(); i++)
		{
			YAML::Node item = Config[i];
			std::shared_ptr<AppellationInfo> pAppellation = std::make_shared<AppellationInfo>();
			pAppellation->Type = item["Type"].as<UINT>();
			pAppellation->Level = item["Level"].as<UINT>();
			pAppellation->Difficulty = item["Difficulty"].as<UINT>();
			pAppellation->MapId = item["MapId"].as<UINT>();
			pAppellation->Value = item["Value"].as<UINT>();
			Appellations[pAppellation->Type] = pAppellation;
		}
		printf("Load AppellationConfig Success!\n");
	}
	catch (const std::exception&)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load AppellationConfig Error\n");
		//exit(0);
	}
}

void LoadRandomMapPool()
{
	try
	{
		RandomMapPools.clear();
		YAML::Node Config = YAML::LoadFile(".\\Config\\RandomMaps.yml");
		for (size_t i = 0; i < Config.size(); i++)
		{
			YAML::Node MapConfig = Config[i];
			int PoolID = MapConfig["ID"].as<int>();
			std::shared_ptr<RANDOMMAPPOOL> RandomMapPool = std::make_shared<RANDOMMAPPOOL>();
	
			YAML::Node PoolConfig = MapConfig["Maps"];
			int poolsize = PoolConfig.size();
			if (poolsize <= 0)
			{
				continue;
			}
			RandomMapPool->MapID.resize(poolsize);

			for (size_t j = 0; j < poolsize; j++)
			{
				RandomMapPool->MapID[j] = PoolConfig[j].as<int>();
			}
			RandomMapPools[PoolID] = RandomMapPool;
			PoolConfig.reset();
			MapConfig.reset();
		}
		printf("Load RandomMaps Success 全随0 速随1 人气随2 边随5 赛事8 排位16 经典13\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load RandomMaps Error\n");
	}
}

void LoadMapPool()
{
	
	int result;
	AllMap = std::make_shared<MAPPOOL>();
	AllMap->MapID.resize(500);
	AllMap->Size = 0;
	BorderMap = std::make_shared<MAPPOOL>();
	BorderMap->MapID.resize(500);
	BorderMap->Size = 0;
	EventMap = std::make_shared<MAPPOOL>();
	EventMap->MapID.resize(500);
	EventMap->Size = 0;
	RankMap = std::make_shared<MAPPOOL>();
	RankMap->MapID.resize(500);
	RankMap->Size = 0;
	USHORT AllMapID = 0, BorderMapID = 0, EventMapID = 0, RankMapID = 0;

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM MapRand;"));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			AllMapID = res->getUInt(1);
			if (AllMapID != 0)
			{
				AllMap->MapID[AllMap->Size] = AllMapID;
				AllMap->Size++;
			}
			BorderMapID = res->getUInt(2);
			if (BorderMapID != 0)
			{
				BorderMap->MapID[BorderMap->Size] = BorderMapID;
				BorderMap->Size++;
			}
			EventMapID = res->getUInt(3);
			if (EventMapID != 0)
			{
				EventMap->MapID[EventMap->Size] = EventMapID;
				EventMap->Size++;
			}
			RankMapID = res->getUInt(4);
			if (RankMapID != 0)
			{
				RankMap->MapID[RankMap->Size] = RankMapID;
				RankMap->Size++;
			}
		}

		res.reset();

	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("LoadMapPool error\n");
	}
}

void LoadPetItem()
{
	
	int result;

	PetItems.clear();

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select * from PetItem;"));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			PETITEM* PetItem = new PETITEM;
			memset(PetItem, 0, sizeof(PETITEM));
			PetItem->PetID = res->getUInt(1);

			std::string newname = UTF2GB(res->getString(2).c_str());
			memcpy(PetItem->PetName, newname.c_str(), newname.length());

			PetItem->Level = res->getUInt(3);
			PetItem->Status = res->getUInt(4);
			PetItem->StrengLevel = res->getUInt(5);
			PetItem->SkillID1 = res->getUInt(6);
			PetItem->SkillValue1 = res->getUInt(7);
			PetItem->SkillID2 = res->getUInt(8);
			PetItem->SkillValue2 = res->getUInt(9);
			PetItem->SkillID3 = res->getUInt(10);
			PetItem->SkillValue3 = res->getUInt(11);
			PetItems[PetItem->PetID] = std::unique_ptr<PETITEM>(PetItem);
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("LoadPetItem error\n");
	}
	printf("Load PetItem Success!\n");

}

void LoadCarTreasure()
{
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\CarTreasure.yml");
		CarTreasures = std::make_shared<CarTreasure>();
		YAML::Node ItemsCfg = Config["Items"];
		if (ItemsCfg && ItemsCfg.size() > 1) {
			for (size_t i = 0; i < ItemsCfg.size(); i++)
			{
				YAML::Node Cfg = ItemsCfg[i];
				CarTreasureInfo Treasure;
				Treasure.ItemId = Cfg["ItemId"].as<UINT>();
				Treasure.ItemNum = Cfg["ItemNum"].as<UINT>();
				Treasure.AvailPeriod = Cfg["AvailPeriod"].as<INT>();
				if (Cfg["ItemType"]) {
					Treasure.ItemType = Cfg["ItemType"].as<UINT>();
				}
				if (Cfg["ExchangePieceNum"]) {
					Treasure.ExchangePieceNum = Cfg["ExchangePieceNum"].as<UINT>();
				}
				if (Cfg["CanExchange"]) {
					Treasure.CanExchange = Cfg["CanExchange"].as<UINT>();
				}
				if (Cfg["Probability"]) {

					Treasure.iProbability = Cfg["Probability"].as<UINT>();
				}
				if (Cfg["OtherAttribute"]) {

					Treasure.OtherAttribute = Cfg["OtherAttribute"].as<UINT>();
				}
				if (Treasure.iProbability > CarTreasures->MaxRate) {//看你妈呢 我比你大你就得用我的值
					CarTreasures->MaxRate = Treasure.iProbability;
				}
				//非常复杂的map
				CarTreasures->TreasureMap[Treasure.iProbability].push_back(Treasure);

				CarTreasures->Treasures.push_back(Treasure);

				if (Treasure.ItemType == 1) {//我比较贪 极品全部给我把

					CarTreasures->JPTreasures.push_back(Treasure);
				}
				else {
					if (Treasure.iProbability > CarTreasures->MaxRate2) {//看你妈呢 我比你大你就得用我的值
						CarTreasures->MaxRate2 = Treasure.iProbability;
					}
					CarTreasures->PtTreasureMap[Treasure.iProbability].push_back(Treasure);

				}

				Cfg.reset();
			}
		}
		printf("Load CarTreasure Success!\n");
	}
	catch (const std::exception&)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);

		printf("Load CarTreasure Error!\n");
	}
}

int LoadTreasureBox()
{
	//目标文件夹路径
	std::string inPath = ".\\Treasure\\*.yml";//遍历文件夹下的所有.yml文件 加载宝箱
	TreasureBoxs.clear();
	//用于查找的句柄
	intptr_t handle;
	struct _finddata_t fileinfo;
	//第一次查找
	handle = _findfirst(inPath.c_str(), &fileinfo);
	if (handle == -1)
		return -1;
	YAML::Node config;
	YAML::Node PoolConfig;
	YAML::Node SeedCfg;
	YAML::Node TreasureCfg;
	YAML::Node ItemCfg;
	UINT ConfigSize = 0;
	UINT ItemNum = 0;
	try {
		do
		{
			std::string outPath(".\\Treasure\\");
			outPath.append(fileinfo.name);

			config = YAML::LoadFile(outPath);
			//printf("TreasureID:%d\n", atoi(fileinfo.name));
			TREASUREBOX* TreasureBox = new TREASUREBOX;
			TreasureBoxs[atoi(fileinfo.name)] = std::unique_ptr<TREASUREBOX>(TreasureBox);
			TreasureBox->LuckyValue = config["LuckyValue"].as<int>();
			PoolConfig = config["Pool"];
			ConfigSize = PoolConfig.size();
			TreasureBox->TreasureAwards.resize(ConfigSize);
			for (int i = 0; i < ConfigSize; i++)
			{
				SeedCfg = PoolConfig[i];
				TreasureBox->TreasureAwards[i].Seed = SeedCfg["Seed"].as<int>();
				TreasureBox->TreasureAwards[i].ItemType = SeedCfg["ItemType"].as<int>();

				//printf("Seed:%d\n", TreasureBox->TreasureAwards[i].Seed);
				TreasureCfg = SeedCfg["Items"];

				ItemNum = TreasureCfg.size();
				
				TreasureBox->TreasureAwards[i].AwardItems.resize(ItemNum);
				for (int j = 0; j < ItemNum; j++)
				{
					ItemCfg = TreasureCfg[j];

					TreasureBox->TreasureAwards[i].AwardItems[j].ItemID = ItemCfg["ItemId"].as<int>();
					TreasureBox->TreasureAwards[i].AwardItems[j].ItemNum = ItemCfg["ItemNum"].as<int>();
					TreasureBox->TreasureAwards[i].AwardItems[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
					ItemCfg.reset();
					//printf("ItemId:%d\n", TreasureBox->TreasureAwards[i].AwardItems[j].ItemID);
				}
				
				TreasureCfg.reset();
				SeedCfg.reset();
			}

		} while (!_findnext(handle, &fileinfo));
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load TreasureBox Exception!\n");
	}

	_findclose(handle);
	printf("Load TreasureBox Success!\n");


	GameEndBox.clear();
	GameEndBox2Uin.clear();
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\Treasure.yml");
		GameEndBoxTime[0] = Config["on"].as<int>();
		GameEndBoxTime[1] = Config["multiple"].as<int>();
		GameEndBoxTime[2] = Config["start"].as<int>();
		GameEndBoxTime[3] = Config["end"].as<int>();
		YAML::Node ItemList = Config["Items"];
		for (size_t i = 0; i < ItemList.size(); i++)
		{
			ItemInfo IT;
			IT.ItemID = ItemList[i]["ItemId"].as<INT>();
			IT.ItemNum = ItemList[i]["ItemNum"].as<INT>();
			IT.AvailPeriod = ItemList[i]["AvailPeriod"].as<INT>();
			IT.ItemType = ItemList[i]["ItemType"].as<INT>();
			GameEndBox.push_back(IT);
		}


		YAML::Node UinList = Config["Uins"];
		for (size_t i = 0; i < UinList.size(); i++)
		{
			YAML::Node Node = UinList[i];
			std::shared_ptr<OpenBoxDef> Box = std::make_shared<OpenBoxDef>();
			Box->Uin = Node["Uin"].as<UINT>();
			Box->BoxID = Node["ItemId"].as<UINT>();
			Box->ItemNum = Node["ItemNum"].as<UINT>();
			Box->BoxType = Node["ItemType"].as<UINT>();
			GameEndBox2Uin[Node["Uin"].as<UINT>()] = Box;
		}
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load GameEndBox Exception!\n");
	}
	printf("Load GameEndBox Success!\n");
	return 0;
}

void LoadMagicLampCfg()
{
	int allitem;
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\MagicLampCfg.yml");
		allitem = Config.size();
		MagicLampItems.resize(allitem);
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Config[j];//0开头
			MagicLampItems[j].ItemID = ItemCfg["ItemId"].as<int>();
			MagicLampItems[j].ItemNum = ItemCfg["ItemNum"].as<int>();
			MagicLampItems[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			MagicLampItems[j].ItemType = ItemCfg["ItemType"].as<int>();
			ItemCfg.reset();
		}
		printf("Load MagicLampCfg Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load MagicLampCfg Error!\n");
	}
}

void LoadAuctionCfg()
{
	int allitem = 0;
	try
	{
		AuctionConfig = std::make_shared<AuctionCfg>();
		YAML::Node Config = YAML::LoadFile(".\\Config\\AuctionCfg.yml");
		AuctionConfig->Open = Config["Open"].as<int>();
		AuctionConfig->Content = Config["Content"].as<std::string>();

		YAML::Node AuctionCfgItems = Config["Items"];
		allitem = AuctionCfgItems.size();
		AuctionConfig->AuctionItemNum = allitem;
		AuctionConfig->AuctionItems.resize(allitem);

		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = AuctionCfgItems[j];
			AuctionConfig->AuctionItems[j].ItemID = ItemCfg["ItemId"].as<int>();
			AuctionConfig->AuctionItems[j].ItemNum = ItemCfg["ItemNum"].as<int>();
			AuctionConfig->AuctionItems[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			AuctionConfig->AuctionItems[j].StartingPrice = ItemCfg["StartingPrice"].as<int>();
			AuctionConfig->AuctionItems[j].IncreasePrice = ItemCfg["IncreasePrice"].as<int>();
			AuctionConfig->AuctionItems[j].CommodityId = ItemCfg["CommodityId"].as<int>();
			AuctionConfig->AuctionItems[j].Time = ItemCfg["Time"].as<std::string>();
			ItemCfg.reset();
		}
		printf("Load AuctionCfg Success!\n");
	}
	catch (const std::exception&)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load AuctionCfg Error!\n");
	}
}

void LoadRegisterBox()
{
	size_t allitem;
	try
	{
		RegisterBoxs.clear();
		YAML::Node Config = YAML::LoadFile(".\\Config\\RegisterBox.yml");
		allitem = Config.size();
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Config[j];//0开头
			std::shared_ptr<ItemInfo> it = std::make_shared<ItemInfo>(); 
			it->ItemID = ItemCfg["ItemId"].as<int>();
			it->ItemNum = ItemCfg["ItemNum"].as<int>();
			it->ObtainTime = ItemCfg["ObtainTime"].as<int>();
			it->AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			RegisterBoxs[j] = it;
			ItemCfg.reset();
		}
		printf("Load RegisterBox Success!\n");
	}
	catch (const std::exception& e)
	{
		/*if (Items)
		{
			delete[] Items;
		}*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load RegisterBox Error!\n");
	}

	Items.clear();
	try
	{
		int result;
		DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ID,buyflag,Name,type,Gametype,Mess,lockid FROM ItemAllNew  where id > 0 "));
		prep_stmt->clearParameters();
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		while (res->next()) {
			UINT ID = res->getUInt("ID");
			UINT buyflag = res->getUInt("buyflag");
			UINT GameType = res->getUInt("Gametype");
			UINT lock = res->getUInt("lockid");

			std::string name = UTF2GB(res->getString("Name").c_str());
			std::string type = UTF2GB(res->getString("type").c_str());
			std::string Mess = UTF2GB(res->getString("Mess").c_str());
	
			Item* pItem = new Item;
			pItem->ID = ID;
			pItem->buyflag = buyflag;
			pItem->lock = lock;
			pItem->Type = EAIT_Unknown;
			pItem->Mess = removeSpaces(Mess);

			if (type == "EAIT_CAR")
			{
				pItem->Type = EAIT_CAR;
				if (Mess == "K0级轮滑" || Mess == "K1级轮滑" || Mess == "K2级轮滑") {
					pItem->GameType = 2;
				}
				else if (Mess == "L0级滑板" || Mess == "L1级滑板" || Mess == "L2级滑板" || Mess == "L3级滑板") {

					pItem->GameType = 1;
				}
				else
				{
					pItem->GameType = 0;
				}
			}
			else if (type == "EAIT_SKIN")
			{
				pItem->Type = EAIT_SKIN;
			}
			else if (type == "EAIT_CLOTHES")
			{
				pItem->Type = EAIT_CLOTHES;
			}
			else if (type == "EAIT_PROP")
			{
				pItem->Type = EAIT_PROP;
			}
			else if (type == "EAIT_PET")
			{
				pItem->Type = EAIT_PET;
			}
			else if (type == "EAIT_STONE")
			{
				pItem->Type = EAIT_STONE;
			}
			pItem->Name = name;
			Items[pItem->ID] = std::unique_ptr<Item>(pItem);
		}

		printf("Load ItemAll Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load ItemAll Error!\n");
	}
}

void LoadDayRaffleBox()
{
	size_t allitem;
	try
	{
		DayRaffleBoxs.clear();
		YAML::Node Config = YAML::LoadFile(".\\Config\\DayRaffle.yml");
		allitem = Config.size();
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Config[j];//0开头
			std::shared_ptr<ItemInfo> it = std::make_shared<ItemInfo>();
			it->ItemID = ItemCfg["ItemId"].as<int>();
			it->ItemNum = ItemCfg["ItemNum"].as<int>();
			it->ObtainTime = ItemCfg["ObtainTime"].as<int>();
			it->AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			DayRaffleBoxs[j] = it;
			ItemCfg.reset();
		}
		printf("Load DayRaffle Success!\n");
	}
	catch (const std::exception& e)
	{
		/*if (Items)
		{
			delete[] Items;
		}*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load DayRaffle Error!\n");
	}


}

void LoadMagicFormationBoxs()
{
	size_t allitem;
	try
	{
		MagicFormationBoxs.clear();
		YAML::Node Config = YAML::LoadFile(".\\Config\\MagicFormation.yml");
		allitem = Config.size();
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Config[j];//0开头
			std::shared_ptr<ItemInfo> it = std::make_shared<ItemInfo>();
			it->ItemID = ItemCfg["ItemId"].as<int>();
			it->ItemNum = ItemCfg["ItemNum"].as<int>();
			it->ObtainTime = ItemCfg["ObtainTime"].as<int>();
			it->AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			MagicFormationBoxs[j] = it;
			ItemCfg.reset();
		}
		printf("Load MagicFormationBoxs Success!\n");
	}
	catch (const std::exception& e)
	{
		/*if (Items)
		{
			delete[] Items;
		}*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load MagicFormationBoxs Error!\n");
	}


}

void LoadMagicExchangeShopBoxs()
{
	size_t allitem;

	try
	{
		MagicExchangeShopCarBoxs.clear();
		MagicExchangeShopPetBoxs.clear();
		MagicExchangeShopSeatBoxs.clear();
		YAML::Node Config = YAML::LoadFile(".\\Config\\MagicExchangeShop.yml");

		YAML::Node Car = Config["EAIT_CAR"];
		allitem = Car.size();
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Car[j];//0开头
			std::shared_ptr<ItemInfo> it = std::make_shared<ItemInfo>();
			it->ItemID = ItemCfg["ItemId"].as<int>();
			it->ItemNum = ItemCfg["ItemNum"].as<int>();
			it->AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			//it->ComsumeItemId = ItemCfg["ComsumeItemId"].as<int>();
			it->ComsumeItemNum = ItemCfg["ComsumeItemNum"].as<int>();
			MagicExchangeShopCarBoxs[j] = it;
			ItemCfg.reset();
		}

		YAML::Node Pet = Config["EAIT_PET"];
		allitem = Pet.size();
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Pet[j];//0开头
			std::shared_ptr<ItemInfo> it = std::make_shared<ItemInfo>();
			it->ItemID = ItemCfg["ItemId"].as<int>();
			it->ItemNum = ItemCfg["ItemNum"].as<int>();
			it->AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			//it->ComsumeItemId = ItemCfg["ComsumeItemId"].as<int>();
			it->ComsumeItemNum = ItemCfg["ComsumeItemNum"].as<int>();
			MagicExchangeShopPetBoxs[j] = it;
			ItemCfg.reset();
		}

		YAML::Node Seat = Config["EAIT_SEAT"];
		allitem = Seat.size();
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = Seat[j];//0开头
			std::shared_ptr<ItemInfo> it = std::make_shared<ItemInfo>();
			it->ItemID = ItemCfg["ItemId"].as<int>();
			it->ItemNum = ItemCfg["ItemNum"].as<int>();
			it->AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			//it->ComsumeItemId = ItemCfg["ComsumeItemId"].as<int>();
			it->ComsumeItemNum = ItemCfg["ComsumeItemNum"].as<int>();
			MagicExchangeShopSeatBoxs[j] = it;
			ItemCfg.reset();
		}
		printf("Load MagicExchangeShopBoxs Success!\n");
	}
	catch (const std::exception& e)
	{
		/*if (Items)
		{
			delete[] Items;
		}*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load MagicExchangeShopBoxs Error!\n");
	}

}

void LoadMiracleCfg()
{
	int allitem;
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\MiracleCfg.yml");
		YAML::Node MagicCupExchangeConfig = Config["MagicCupExchange"];
		MagicCupExchange[EAIT_CAR] = MagicCupExchangeConfig["EAIT_CAR"].as<int>();
		MagicCupExchange[EAIT_SKIN] = MagicCupExchangeConfig["EAIT_SKIN"].as<int>();
		MagicCupExchange[EAIT_CLOTHES] = MagicCupExchangeConfig["EAIT_CLOTHES"].as<int>();
		MagicCupExchange[EAIT_PROP] = MagicCupExchangeConfig["EAIT_PROP"].as<int>();
		MagicCupExchange[EAIT_PET] = MagicCupExchangeConfig["EAIT_PET"].as<int>();
		MagicCupExchange[EAIT_STONE] = MagicCupExchangeConfig["EAIT_STONE"].as<int>();
		printf("Load MagicCupExchangeConfig Success!\n");

		YAML::Node MiracleConfig = Config["MiracleCfg"];
		allitem = MiracleConfig.size();
		MiracleCfg = std::make_shared<MIRACLECFG>();
		MiracleCfg->MiracleItems.resize(allitem);
		for (int j = 0; j < allitem; j++)
		{
			YAML::Node ItemCfg = MiracleConfig[j];//0开头
			MiracleCfg->MiracleItems[j].ItemID = ItemCfg["ItemId"].as<int>();
			MiracleCfg->MiracleItems[j].ItemNum = ItemCfg["ItemNum"].as<int>();
			MiracleCfg->MiracleItems[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
			MiracleCfg->MiracleItems[j].ItemType = ItemCfg["ItemType"].as<int>();
			MiracleCfg->MiracleItems[j].ComsumeItemId = ItemCfg["ComsumeItemId"].as<int>();
			MiracleCfg->MiracleItems[j].ComsumeItemNum = ItemCfg["ComsumeItemNum"].as<int>();
			ItemCfg.reset();
		}
		printf("Load MiracleCfg Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load MiracleCfg Error!\n");
	}
}

void LoadUserPermissionsConfig()
{
	ClothesGMUins.clear();
	CarGMUins.clear();
	AllGMUins.clear();
	AdminUins.clear(); // 清空管理员集合

	try
	{
		YAML::Node config = YAML::LoadFile(".\\Config\\AUserPermissions.yml");
		YAML::Node UserPermissionsList = config["UserPermissionsList"];
		if (UserPermissionsList)
		{
			YAML::Node ClothesGMUinsNode = UserPermissionsList["ClothesGMUins"];
			if (ClothesGMUinsNode)
			{
				ClothesGMMess = removeSpaces(ClothesGMUinsNode["Mess"].as<std::string>());
				for (size_t i = 0; i < ClothesGMUinsNode["Uins"].size(); i++)
				{
					ClothesGMUins.insert(ClothesGMUinsNode["Uins"][i]["Uin"].as<UINT>());
				}
			}
			YAML::Node CarGMUinsNode = UserPermissionsList["CarGMUins"];
			if (CarGMUinsNode)
			{
				CarGMMess = removeSpaces(CarGMUinsNode["Mess"].as<std::string>());
				for (size_t i = 0; i < CarGMUinsNode["Uins"].size(); i++)
				{
					CarGMUins.insert(CarGMUinsNode["Uins"][i]["Uin"].as<UINT>());
				}
			}

			YAML::Node AllGMUinsNode = UserPermissionsList["AllGMUins"];
			if (AllGMUinsNode)
			{
				CarGMMess = removeSpaces(AllGMUinsNode["Mess"].as<std::string>());
				for (size_t i = 0; i < AllGMUinsNode["Uins"].size(); i++)
				{
					AllGMUins.insert(AllGMUinsNode["Uins"][i]["Uin"].as<UINT>());
				}
			}

			// 加载管理员权限用户 ID
			YAML::Node AdminUinsNode = UserPermissionsList["AdminUins"];
			if (AdminUinsNode)
			{
				for (size_t i = 0; i < AdminUinsNode["Uins"].size(); i++)
				{
					AdminUins.insert(AdminUinsNode["Uins"][i]["Uin"].as<UINT>());
				}
			}
			// 加载 SpecialLoginUins
			YAML::Node SpecialLoginUinsNode = UserPermissionsList["SpecialLoginUins"];
			if (SpecialLoginUinsNode)
			{
				for (size_t i = 0; i < SpecialLoginUinsNode["Uins"].size(); i++)
				{
					SpecialLoginUins.insert(SpecialLoginUinsNode["Uins"][i]["Uin"].as<UINT>());
				}
			}
		}


		printf("Load UserPermissionsConfig Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load UserPermissionsConfig Error!\n");
	}
}

