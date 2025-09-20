#include "Dance.h"
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <limits> 
#include "Room.h"

#include "yaml-cpp/yaml.h"
#include "Database.h"

extern std::recursive_mutex Mutex;
extern std::map<long, std::shared_ptr<RoomNode>> Rooms;

void SwapItem(std::vector<std::shared_ptr<ClientNode>>& Players, int low, int high)
{
	std::shared_ptr<ClientNode> Temp = Players[low];
	Players[low] = Players[high];
	Players[high] = Temp;
}

int getPartion(std::vector<std::shared_ptr<ClientNode>>& Players, int low, int high)
{
	int key = Players[low]->Scores;
	while (low < high)
	{
		while (low < high && key >= Players[high]->Scores)//降序排序
			high--;
		SwapItem(Players, low, high);

		while (low < high && key <= Players[low]->Scores)
			low++;
		SwapItem(Players, low, high);
	}
	return low;//返回key值的下标
}


void QuitSort(std::vector<std::shared_ptr<ClientNode>>& Players, int low, int high)
{
	if (low < high)
	{
		int keyindex = getPartion(Players, low, high);

		QuitSort(Players, low, keyindex - 1);
		QuitSort(Players, keyindex + 1, high);
	}
}

void WINAPI OnDanceTimer3(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyDanceGameOver(RoomClient);
			if (Room->ChatRoomType == 6)
			{
				//进入3d房间
				ResponseCreateRoom(RoomClient, Room, 6);
				RequestEnter3DRoom(RoomClient);
				NotifyChangeRoomOwner(RoomClient, Room);
				for (char j = 0; j < 6; j++)
				{
					std::shared_ptr<ClientNode> RoomClient2 = Room->Player[j];
					if (RoomClient2 && RoomClient2 != RoomClient)
					{
						NotifyEnterRoom(RoomClient, Room, RoomClient2, RoomClient2->SeatID);
					}
				}
			}
		}
	}
	Room->Callback = NULL;//设置回调
}
void WINAPI OnDanceTimer2(std::shared_ptr<RoomNode> Room)
{
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i])
		{
			Players[CurrentPlayers] = Room->Player[i];
			CurrentPlayers++;
		}
	}
	QuitSort(Players, 0, CurrentPlayers - 1);//快速排序
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyDanceRaceOver(RoomClient, Players, CurrentPlayers);
		}
	}
	CreateRoomTimer(Room, 10000, OnDanceTimer3);
}
void WINAPI OnDanceTimer(std::shared_ptr<RoomNode> Room)
{
	for (char i = 0; i < 6; i++)
	{
		std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
		if (RoomClient)
		{
			NotifyDanceShow(RoomClient);
		}
	}
	CreateRoomTimer(Room, 5000, OnDanceTimer2);
}

int DanceMusicDuration(int MusicID)
{
	char HasMusic = 0;
	int Duration = 1000 * 60 * 3;//默认设置2分钟自动结束 0xFFFFFFFF (-1)无限大
	//int Duration = -1;//默认设置2分钟自动结束 0xFFFFFFFF (-1)无限大
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\DanceMusic.yml");
		for (size_t i = 0; i < Config.size(); i++)
		{
			YAML::Node DanceMusic = Config[i];
			if (DanceMusic["ID"].as<int>() == MusicID)
			{
				HasMusic = 1;
				Duration = DanceMusic["Duration"].as<int>();
				break;
			}
		}
		if (!HasMusic)
		{
			//printf("歌曲ID:%d 时间缺失 默认3分钟结束!\n", MusicID);
		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
	}
	return Duration;
}

void NotifyDanceGameBegin(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;
	Write32(p, 8000); // NormalLoadTime

	Write32(p, Client->GameID);		  // GameID
	Write32(p, (DWORD)time(nullptr)); // Seed

	Write8(p, Room->CurrentPlayerNum);

	int Mode = 3;
	if (Room->SubGameMode == 2)
	{
		Mode = 1;
	}
	else if (Room->SubGameMode == 1)
	{
		Mode = 3;
	}


	Write32(p, Mode);		   // Mode
	Write32(p, Room->SceneID); // SceneID
	Write32(p, Room->MusicID); // MusicID

	Write8(p, 0);					   // RandKeyFlag
	Write8(p, Room->CurrentPlayerNum); // PlayerNum
	for (size_t i = 0; i < 6; i++)
	{ // LoverDanceStartGameInfo
		if (Room->Player[i])
		{
			BYTE* pLoverDanceStartGameInfo = p;
			Write16(pLoverDanceStartGameInfo, 0); // len

			Write32(pLoverDanceStartGameInfo, Room->Player[i]->Uin); // Uin
			Write32(pLoverDanceStartGameInfo, Mode);				 // Mode
			Write8(pLoverDanceStartGameInfo, 0);					 // RandKeyFlag
			Write8(pLoverDanceStartGameInfo, 0);					 // TeamID

			len = pLoverDanceStartGameInfo - p;
			Set16(p, (WORD)len);
			p += len;
		}
	}
	/*
m_astLoverDanceStartGameInfo[].m_uiUin
m_astLoverDanceStartGameInfo[].m_iMode
m_astLoverDanceStartGameInfo[].m_bRandKeyFlag
m_astLoverDanceStartGameInfo[].m_ucTeamID
	*/

	len = p - buf.data();
	SendToClient(Client, 726, buf.data(), len, Client->GameID, FE_PLAYER, Client->ConnID, Notify);
}

void RequestDancePrepareReady(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	/*
m_iMode
m_iSceneID
m_szSceneHash[]:
m_iMusicID
m_szMusicHash[]:
m_bRandKeyFlag
m_uiClothesWeight
	*/
	if (Client->RoomID)
	{
		Client->IsReady = TRUE;
		std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
		if (!Room)
		{
			return;
		}
		BOOL All = TRUE;
		for (char i = 0; i < 6; i++)
		{
			std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
			if (RoomClient)
			{
				if (RoomClient->IsReady == FALSE)
				{
					All = FALSE;
					break;
				}
			}
		}
		if (All)
		{
			Sleep(15000);//等待加载
			for (char i = 0; i < 6; i++)
			{
				std::shared_ptr<ClientNode> RoomClient = Room->Player[i];
				if (RoomClient)
				{
					NotifyDanceRaceBegin(RoomClient, Room);
				}
			}
		}
	}
}

void NotifyDanceRaceBegin(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room)
{
	std::vector<std::shared_ptr<ClientNode>> Players(6);
	char CurrentPlayers = 0;
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i])
		{
			Players[CurrentPlayers] = Room->Player[i];
			CurrentPlayers++;
		}
	}

	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 9000); // CountDownTime

	Write8(p, CurrentPlayers); // PlayerNum
	for (char i = 0; i < CurrentPlayers; i++)
	{
		Write32(p, Players[i]->Uin); // Uin[]
	}

	len = p - buf.data();
	SendToClient(Client, 728, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyDanceChangeMusic(std::shared_ptr<ClientNode> Client,UINT Uin,int MusicID)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write32(p, MusicID); //NewMusicID
	Write32(p, 0); //PreKnownMusicID

	len = p - buf.data();
	SendToClient(Client, 721, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestDanceChangeMusic(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int MusicID = Read32(Body);

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	Room->MusicID = MusicID;

	ResponseDanceChangeMusic(Client, MusicID);
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i] && Room->Player[i] != Client)
		{
			NotifyDanceChangeMusic(Room->Player[i], Uin, MusicID);
		}
	}
}
void ResponseDanceChangeMusic(std::shared_ptr<ClientNode> Client, int MusicID)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);		 // ResultID
	Write32(p, MusicID); // NewMusicID

	Write8(p, 0); // ReasonLen

	Write32(p, 0); // PreKnownMusicID

	len = p - buf.data();
	SendToClient(Client, 253, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}

void NotifyDanceChangeMode(std::shared_ptr<ClientNode> Client,UINT Uin,int Mode)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write32(p, Mode); //NewMode
	Write32(p, 0); //PreKnownMusicID

	len = p - buf.data();
	SendToClient(Client, 723, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestDanceChangeMode(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int Mode = Read32(Body);
	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	ResponseDanceChangeMode(Client, Mode);
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i] && Room->Player[i] != Client)
		{
			NotifyDanceChangeMode(Room->Player[i], Uin, Mode);
		}
	}
}
void ResponseDanceChangeMode(std::shared_ptr<ClientNode> Client, int Mode)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);	  // ResultID
	Write32(p, Mode); // NewMode

	Write8(p, 0); // ReasonLen

	Write32(p, 0); // PreKnownMusicID

	len = p - buf.data();
	SendToClient(Client, 257, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}

void RequestDanceChangeRandKeyFlag(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	BYTE RandKeyFlag = Read8(Body);

	ResponseDanceChangeRandKeyFlag(Client, RandKeyFlag);
}
void ResponseDanceChangeRandKeyFlag(std::shared_ptr<ClientNode> Client, BYTE RandKeyFlag)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); // ResultID

	Write8(p, RandKeyFlag); // NewRandKeyFlag

	Write8(p, 0); // ReasonLen

	len = p - buf.data();
	SendToClient(Client, 258, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}
void NotifyChangeScene(std::shared_ptr<ClientNode> Client,UINT Uin,int SceneID)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, Uin); //Uin
	Write32(p, SceneID); //NewSceneID

	len = p - buf.data();
	SendToClient(Client, 722, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestChangeScene(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int SceneID = Read32(Body);

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	Room->SceneID = SceneID;
	ResponseChangeScene(Client, SceneID);
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i] && Room->Player[i] != Client)
		{
			NotifyChangeScene(Room->Player[i], Uin, SceneID);
		}
	}
}
void ResponseChangeScene(std::shared_ptr<ClientNode> Client, int SceneID)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);		 // ResultID
	Write32(p, SceneID); // NewSceneID

	Write8(p, 0); // ReasonLen

	len = p - buf.data();
	SendToClient(Client, 254, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}

void RequestDanceStepStatistics(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	WORD len;
	UINT ScoresInc = 0;

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int SegID = Read32(Body);
	int StepID = Read32(Body);
	int Checksum = Read32(Body);
	short Hits = Read16(Body);

	struct
	{
		short First;
		short Last;
	} Combo = { 0, 0 };
	{ // Combo
		BYTE* pCombo = Body;
		len = Read16(pCombo);

		Combo.First = Read16(pCombo);
		Combo.Last = Read16(pCombo);
		// printf("First:%d\tLast:%d\t", Combo.First, Combo.Last);

		Body += len;
	}

	short Decision = Read16(Body);

	// printf("Hits:%d\tDecision:%d\n", Hits, Decision);

	ScoresInc = Combo.Last * Decision;
	Client->Scores += ScoresInc;
	ResponseDanceStepStatistics(Client, Uin, ScoresInc, Client->Scores);

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i] && Room->Player[i] != Client)
		{
			NotifyDanceStepStatistics(Room->Player[i], Uin, ScoresInc, Client->Scores);
		}
	}
}
void ResponseDanceStepStatistics(std::shared_ptr<ClientNode> Client, UINT Uin, UINT ScoresInc, UINT ScoresTotal)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); // ResultID
	Write32(p, Uin);
	Write32(p, ScoresInc);
	Write32(p, ScoresTotal);

	len = p - buf.data();
	SendToClient(Client, 260, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Response);
}
void NotifyDanceStepStatistics(std::shared_ptr<ClientNode> Client, UINT Uin, UINT ScoresInc, UINT ScoresTotal)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, Uin);
	Write32(p, ScoresInc);
	Write32(p, ScoresTotal);

	len = p - buf.data();
	SendToClient(Client, 732, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyDanceRank(std::shared_ptr<ClientNode> Client, UINT Uin, UINT ScoresInc, UINT ScoresTotal)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	/*
m_bPlayerNum
m_auiUin[]
	*/

	len = p - buf.data();
	SendToClient(Client, 733, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void RequestLoverDanceChangeOneMode(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int Mode = Read32(Body);

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	ResponseLoverDanceChangeOneMode(Client, Mode);
}
void ResponseLoverDanceChangeOneMode(std::shared_ptr<ClientNode> Client, int Mode)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);	  // ResultID
	Write32(p, Mode); // NewMode

	Write8(p, 0); // ReasonLen

	len = p - buf.data();
	SendToClient(Client, 262, buf.data(), len, Client->RoomID, FE_ROOM, Client->ConnID, Response);
}

void RequestLoverDanceStepStatistics(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	WORD len;
	UINT ScoresInc = 0;

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	int SegID = Read32(Body);
	int StepID = Read32(Body);
	int Checksum = Read32(Body);
	short Hits = Read16(Body);

	struct
	{
		short First;
		short Last;
	} Combo = { 0, 0 };
	{ // Combo
		BYTE* pCombo = Body;
		len = Read16(pCombo);

		Combo.First = Read16(pCombo);
		Combo.Last = Read16(pCombo);
		// printf("First:%d\tLast:%d\t", Combo.First, Combo.Last);

		Body += len;
	}

	short Decision = Read16(Body);
	short NormalKeyTotalHits = Read16(Body);

	// printf("Hits:%d\tDecision:%d\tNormalKeyTotalHits:%d\n", Hits, Decision, NormalKeyTotalHits);

	ScoresInc = Combo.Last * Decision;
	Client->Scores += ScoresInc;

	std::shared_ptr<RoomNode> Room = GetRoom(Client->RoomID);
	if (!Room)
	{
		return;
	}
	for (char i = 0; i < 6; i++)
	{
		if (Room->Player[i] && Room->Player[i] != Client)
		{
			NotifyLoverDanceStepStatistics(Room->Player[i], 0, ScoresInc, Client->Scores);
		}
	}
}
void NotifyLoverDanceStepStatistics(std::shared_ptr<ClientNode> Client, char TeamID, UINT ScoresInc, UINT ScoresTotal)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write8(p, TeamID);
	Write32(p, ScoresInc);
	Write32(p, ScoresTotal);
	Write8(p, 0);  // Cooper
	Write8(p, 0);  // CooperScoresInc
	Write32(p, 0); // TotalCooper

	len = p - buf.data();
	SendToClient(Client, 735, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyDanceShow(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 3000); // ShowTime

	len = p - buf.data();
	SendToClient(Client, 729, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyDanceRaceOver(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>>& Players, char CurrentPlayers)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	
	int result;
	SelectClientExp(Client);

	Client->DanceExp += 1000;//增加舞力
	//Client->Experience += 25000;//+10000经验

	Client->Experience = (Client->Experience > UINT_MAX - 25000) ? UINT_MAX : Client->Experience + 25000;
	Client->Money += 1000;//+1000酷比

	UpdateClientBase(Client);

	Write8(p, CurrentPlayers); //PlayerNum


	for (size_t i = 0; i < CurrentPlayers; i++)
	{ //DanceScores
		std::shared_ptr<ClientNode> RoomClient = Players[i];
		BYTE* pDanceScores = p;
		Write16(pDanceScores, 0); //len

		Write32(pDanceScores, RoomClient->Uin); //Uin
		Write32(pDanceScores, 0); //Rank
		Write32(pDanceScores, RoomClient->Scores); //Score
		Write32(pDanceScores, 1000); //DanceExpInc
		Write32(pDanceScores, RoomClient->DanceExp); //TotalDanceExp
		Write32(pDanceScores, 0); //MaxCombo
		Write32(pDanceScores, 0); //MaxPerfect
		Write32(pDanceScores, 25000); //ExpInc
		Write32(pDanceScores, RoomClient->Experience); //TotalExp
		Write32(pDanceScores, 1000); //MoneyInc
		Write32(pDanceScores, RoomClient->Money); //TotalMoney
		Write8(pDanceScores, 0); //AwardNum
		//Write32(pDanceScores, 0); //SpecialAward[]
		Write32(pDanceScores, 0); //Degree
		Write32(pDanceScores, 0); //LoverVipDanceExpMulti

		len = pDanceScores - p;
		Set16(p, (WORD)len);
		p += len;


	}

	Write8(p, 0);  // WinTeamID
	Write32(p, 0); // AwardTime

	Write8(p, 0); // TeamNum

	len = p - buf.data();
	SendToClient(Client, 730, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}

void NotifyDanceGameOver(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); // ResultID

	len = p - buf.data();
	SendToClient(Client, 731, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Notify);
}


void ResponseDanceGetRoomList(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;
	int RoomNum = 0;
	Write16(p, 0); //ResultID
	Write32(p, Client->Uin); //Uin
	Write16(p, 0); //CurTotalRoomNum
	Write16(p, 0); //StartIdx
	BYTE* pRoomNum = p;
	Write16(p, 0); //RoomNum
	std::lock_guard<std::recursive_mutex> lg(Mutex);
	std::map<long, std::shared_ptr<RoomNode>>::iterator iter = Rooms.begin();;
	for (; iter != Rooms.end(); iter++)
	{ //DanceRooms
		std::shared_ptr<RoomNode> Room = iter->second;
		if (Room->BaseGameMode != 4)
		{
			continue;
		}
		RoomNum++;
		BYTE* pDanceRooms = p;
		Write16(pDanceRooms, 0); //len

		Write16(pDanceRooms, Room->ID); //RoomID
		Write16(pDanceRooms, 1); 
		Write16(pDanceRooms, 1); 
		Write8(pDanceRooms, Room->CurrentPlayerNum); //CurrentPlayerNum
		Write8(pDanceRooms, Room->SeatNum); //TotalSeatNum
		if (Room->Callback == NULL && Room->SeatNum > 1)
		{
			Write8(pDanceRooms, 1); 
		}
		else
			Write8(pDanceRooms, 0);
		Write32(pDanceRooms, 0); //Mode
		Write32(pDanceRooms, 0); //SceneID
		Write32(pDanceRooms, 0); //MusicID
		Write8(pDanceRooms, 0); //RandKeyFlag
		Write8(pDanceRooms, Room->Flag); //Flag
		memcpy(pDanceRooms, Room->Name, 22);
		pDanceRooms += 22;
		Write16(pDanceRooms, 0); //SvrId
		Write16(pDanceRooms, 0); //SceneID
		Write8(pDanceRooms, 0); //CurrentOBNum
		Write8(pDanceRooms, 0); //TotalOBNum
		Write8(pDanceRooms, 0); //AllowAutoStart

		len = pDanceRooms - p;
		Set16(p, (WORD)len);
		p += len;

	}
	Set16(pRoomNum, RoomNum);

	Write8(p, 0); //ReasonLen
	Write16(p, 0); //BaseGameMode
	Write16(p, 0); //SubGameMode
	Write8(p, 0); //RoomFlag

	len = p - buf.data();
	SendToClient(Client, 251, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestDanceGetRoomList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short StartRoomIdx = Read16(p);
	short GetRoomNum = Read16(p);
	short BaseGameMode = Read16(p);
	short SubGameMode = Read16(p);
	bool RoomFlag = Read8(p);
	ResponseDanceGetRoomList(Client);
}

void ResponseDanceEnterRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, char SeatID)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write16(p, 0); //ResultID
	Write16(p, Room->ID); //RoomID
	Write16(p, Room->BaseGameMode); //BaseGameMode
	Write16(p, Room->SubGameMode); //SubGameMode
	memset(p, 0, 22);
	p += 22;
	Write8(p, 6); //SeatNum
	Write16(p, 0x18c); //Flag
	Write8(p, SeatID); //SeatID
	Write8(p, 0); //TeamID
	Write32(p, 0); //Mode
	Write32(p, 0); //SceneID
	Write32(p, 0); //MusicID
	Write8(p, 0); //RandKeyFlag
	Write16(p, Room->RoomOwnerID); //RoomOwnerID
	for (size_t i = 0; i < 6; i++)
	{
		Write8(p, 1);
	}
	Write8(p, 0); //PlayerCount

	Write8(p, 0); //ReasonLen
	Write8(p, 0); //PlayerCount2

	Write8(p, 0); //DebutOrX5
	Write32(p, 0); //PreKnownMusicID
	Write8(p, 0); //HasMoreInfo
	Write16(p, 0); //KartNum

	Write8(p, 0); //RoomFlag
	Write8(p, 0); //MaxOBNum
	Write8(p, 0); //AllowAutoStart
	Write8(p, 0); //GVoiceRoomNameLen
	Write8(p, 0); //GVoiceReasonLen

	len = p - buf.data();
	SendToClient(Client, 252, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}

void RequestDanceEnterRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	LeaveRoom(Client, 0);
	LeaveBorderRoom(Client, 0);
	LeaveOutdoor(Client);//出房间

	BYTE* p = Body; WORD len;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	short RoomID = Read16(p);
	char Password[16] = { 0 };
	memcpy(Password, p, 16);
	p += 16;
	BYTE InviteType = Read8(Body);
	UINT InviterUin = Read32(Body);
	bool RoomFlag = Read8(p);
	std::shared_ptr<RoomNode> Room = GetRoom(RoomID);
	if (!Room)
	{
		return;
	}
	if (Room->Callback != NULL || Room->SeatNum == Room->CurrentPlayerNum)
	{
		return;
	}
	for (int i = 0; i < 15; i++)
	{
		if (Password[i + 1] != Room->Password[i + 1])
		{    //默认传0x30
			//从第1位开始比较 0位不管
			ResponseStartGame(Client, "房间密码错误!");
			return;
		}
	}
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

			if (Room->ChatRoomType == 6)
			{
				//进入3d房间
				ResponseCreateRoom(Client, Room, 6);
				RequestEnter3DRoom(Client);
				NotifyChangeRoomOwner(Client, Room);
			}
			else
			{
				ResponseDanceEnterRoom(Client, Room, i);
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
			break;
		}
	}
}


