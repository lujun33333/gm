#pragma once
class Map
{
public:
	UINT Head;
	UINT End;
	UINT Round;
	UINT Limit;
	bool Loop;
};

struct LevelCheckpoint {
	int level;
	int checkpoint1;
	int checkpoint2;
};

void LoadMapConfig();
Map* GetMap(UINT ID);
UINT findLevel(UINT checkpoint, UINT MaPID);
