#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Map.h"

#include <map>
#include "yaml-cpp/yaml.h"
#include <filesystem>


std::map<UINT, std::shared_ptr<Map>> Maps;
std::map<UINT, std::vector<LevelCheckpoint>> levelCheckpoints;

bool compareByLevel(const LevelCheckpoint& a, const LevelCheckpoint& b) {
	return a.level < b.level;
}

void LoadMapConfig()
{
	Maps.clear();
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\Map.yml");
		YAML::Node map;
		for (size_t i = 0; i < Config.size(); i++)
		{
			map = Config[i];
			std::shared_ptr<Map> pMap = std::make_shared<Map>();
			Maps[map["ID"].as<UINT>()] = pMap;
			pMap->Head = map["Head"].as<UINT>();
			pMap->End = map["End"].as<UINT>();
			pMap->Loop = map["Loop"].as<bool>();
			pMap->Round = map["Round"].as<UINT>();
			pMap->Limit = map["Limit"].as<UINT>();
			map.reset();
		}

		std::filesystem::path path(".\\Map");
		if (exists(path))
		{
			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				if (std::filesystem::is_regular_file(entry))
				{
					std::vector<LevelCheckpoint> levelCheckpoint;
					auto& PathInfo = entry.path();
					if (PathInfo.extension() == ".yml")
					{
						auto path = PathInfo.string();
						YAML::Node uKartItem = YAML::LoadFile(path);
						for (const auto& level : uKartItem) {
							LevelCheckpoint lc;
							lc.level = level["Level"].as<int>();
							lc.checkpoint1 = level["CheckPoint1"].as<int>();
							lc.checkpoint2 = 0;
							if (level["CheckPoint2"])
							{
								lc.checkpoint2 = level["CheckPoint2"].as<int>();
							}
							levelCheckpoint.push_back(lc);
						}
					}
					std::sort(levelCheckpoint.begin(), levelCheckpoint.end(), compareByLevel);
					UINT MapID = atof(PathInfo.filename().string().c_str());
					levelCheckpoints[MapID] = levelCheckpoint;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf("加载地图文件出错\n");
		//exit(0);
	}
}

UINT findLevel(UINT checkpoint, UINT MapID) {
	std::vector<LevelCheckpoint> levelCheckpoint = levelCheckpoints[MapID];
	for (int i = 0; i < levelCheckpoint.size(); ++i) {
		if (checkpoint < levelCheckpoint[i].checkpoint1)
		{
			return levelCheckpoint[i].level - 1;
		}
		if (checkpoint == levelCheckpoint[i].checkpoint1)
		{
			return levelCheckpoint[i].level;
		}
	}
	for (int i = 0; i < levelCheckpoint.size(); ++i) {
		if (checkpoint < levelCheckpoint[i].checkpoint2)
		{
			return levelCheckpoint[i].level - 1;
		}
		if (checkpoint == levelCheckpoint[i].checkpoint2)
		{
			return levelCheckpoint[i].level;
		}
	}
	return 0;
}

Map* GetMap(UINT ID)
{
	return Maps[ID].get();
}
