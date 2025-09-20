#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Item.h"

#include <unordered_map>
#include "yaml-cpp/yaml.h"
#include <filesystem>
std::unordered_map<UINT, std::unique_ptr<Item>> Items;//hashmap

std::map<UINT, std::vector<CarTreasureInfo>> CommonBoxAwards;
void LoadItemConfig()
{
	Items.clear();
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\Item.yml");
		YAML::Node item;
		for (size_t i = 0; i < Config.size(); i++)
		{
			item = Config[i];
			Item* pItem = new Item;
			pItem->ID = item["ID"].as<UINT>();
			//ItemsName[item["ID"].as<UINT>()] = item["Name"].as<std::string>();

			std::string Type = item["Type"].as<std::string>();
			pItem->Type = EAIT_Unknown;
			if (Type == "EAIT_CAR")
			{
				pItem->Type = EAIT_CAR;
			}
			else if (Type == "EAIT_SKIN")
			{
				pItem->Type = EAIT_SKIN;
			}
			else if (Type == "EAIT_CLOTHES")
			{
				pItem->Type = EAIT_CLOTHES;
			}
			else if (Type == "EAIT_PROP")
			{
				pItem->Type = EAIT_PROP;
			}
			else if (Type == "EAIT_PET")
			{
				pItem->Type = EAIT_PET;
			}
			else if (Type == "EAIT_STONE")
			{
				pItem->Type = EAIT_STONE;
			}
			Items[pItem->ID] = std::unique_ptr<Item>(pItem);
			//item.reset();
		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load Item Error!\n");
		//exit(0);
	}
	//printf("Load Item Success! [GetItemType%d]\n", GetItemType(127618));

	printf("Load Item Success!\n");
}
void LoadCommonBoxAwards()
{
	try
	{
		CommonBoxAwards.clear();
		std::filesystem::path path(".\\GameTreasureAward\\CommonBoxAward");
		if (exists(path))
		{
			auto begin = std::filesystem::recursive_directory_iterator(path); // 获取文件系统迭代器
			auto end = std::filesystem::recursive_directory_iterator();		  // end迭代器
			for (auto& it = begin; it != end; it++)
			{
				auto& entry = *it;
				if (std::filesystem::is_regular_file(entry))
				{ // 是否是普通文件
					auto& PathInfo = entry.path();
					if (PathInfo.extension() == ".yml")
					{
						UINT Type;
						Type = atof(PathInfo.filename().string().c_str());
						auto path = PathInfo.string();
						const YAML::Node CommonBoxAward = YAML::LoadFile(path);
						for (size_t i = 0; i < CommonBoxAward.size(); i++) {
							CarTreasureInfo CarT;
							CarT.ItemId = CommonBoxAward[i]["ItemId"].as<UINT>();
							CarT.AvailPeriod = CommonBoxAward[i]["AvailPeriod"].as<INT>();
							CarT.ItemNum = CommonBoxAward[i]["ItemNum"].as<UINT>();
							CarT.ExchangePieceNum = CommonBoxAward[i]["PieceNum"].as<UINT>();
							CommonBoxAwards[Type].push_back(CarT);
						}

					}
				}
			}
		}
		printf("Load CommonBoxAward Success!\n");
	}
	catch (const std::exception&)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("Load CommonBoxAward Error!\n");
	}
}

UINT GetItemType(UINT ID)
{
	std::unique_ptr<Item>& pItem = Items[ID];
	UINT Type = EAIT_Unknown;
	if (pItem)
	{
		Type = pItem->Type;
	}
	//printf("Type:%d\n", Type);
	return Type;
}


UINT GetItemGameType(UINT ID)
{
	std::unique_ptr<Item>& pItem = Items[ID];
	UINT GameType = 0;
	if (pItem)
	{
		GameType = pItem->GameType;
	}
	return GameType;
}


std::string GetItemMess(UINT ID)
{
	std::unique_ptr<Item>& pItem = Items[ID];
	std::string Mess = "";
	if (pItem)
	{
		Mess = pItem->Mess;
	}
	return Mess;
}

int countItemsWithMess( const std::string& targetMess) {
	int count = 0;

	for (const auto& pair : Items) {
		if (pair.second->Mess == targetMess) {
			++count;
		}
	}

	return count;
}
