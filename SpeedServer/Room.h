#pragma once
#include "Message.h"


#ifdef ZingSpeed
constexpr int MaxRoomName = 66;
#else
constexpr int MaxRoomName = 22;
#endif


class RoomNode;
typedef void(WINAPI* RoomOnTimer)(std::shared_ptr<RoomNode>);

class RoomNode
{
public:
	UINT ID = 0;
	char Name[MaxRoomName];
	USHORT RoomOwnerID = 0;
	USHORT ServerID = 0;
	USHORT BaseGameMode = 0;
	USHORT SubGameMode = 0;
	BYTE SeatNum = 0;
	BYTE CurrentPlayerNum = 0;
	USHORT Flag = 0x18C;//标志位
	char Password[16] = { 0 };//房间密码
	std::vector<std::shared_ptr<ClientNode>> Player;
	char SeatState[6] = { 1 };//座位状态
	HANDLE Timer = NULL;
	RoomOnTimer Callback = NULL;//std::shared_ptr<RoomNode> 指针 
	BYTE ChatRoomType = 0;
	BYTE PresetOBNum = 0;
	//
	BYTE AllowAutoStart = 0;
	UINT MapID = 0;
	UINT Israce = 0;
	UINT IsAnonymous = 0;
	UINT isSkip = 0;
	UINT IsStake = 0;
	UINT IsBottomGame = 0;
	UINT BottomGameNum = 0;
	UINT BottomPlayerNum = 0;
	std::vector<CMatchBanMapInfo> MatchBanMapInfo;
	std::string GVoiceRoomName = "";
	class
	{
	public:
		char Season;
		char Timeslot;
		char Weather;
	} STDynamicInfo;

	//Dance
	int SceneID = 0;
	int MusicID = 0;
	UINT startStatus = 1;
	//
	//char BorderFlag;
	USHORT matchQueueType = 0;
	char CountDownFlag = 0;
	char ranknumber = 0;
	DWORD dwStart = 0;
	DWORD dwEnd = 0;
	RoomNode()
	{
		Player.resize(6);
	}
	std::shared_mutex mutex; 
};

std::shared_ptr<RoomNode> GetRoom(long RoomID);
std::shared_ptr<RoomNode> GetBorderRoom(long RoomID);
void LeaveRoom(std::shared_ptr<ClientNode> Client, short LeaveType);
void LeaveBorderRoom(std::shared_ptr<ClientNode> Client, short LeaveType);
void PrintRoom();
void CreateRoomTimer(std::shared_ptr<RoomNode> Room, int Timeout, RoomOnTimer Callback);


void RequestGetRoomList(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseGetRoomList(std::shared_ptr<ClientNode> Client, USHORT BaseGameMode, USHORT SubGameMode, std::vector<std::shared_ptr<RoomNode>>& AllRooms, int RoomNum, int StartRoomIdx,  BYTE RoomFlag);
void StartSingleGame(std::shared_ptr<ClientNode> Client, BYTE* Body);
void RequestCreateRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseCreateRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, BYTE ChatRoomType);

void RequestEnterRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseEnterRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, char SeatID,BOOL HasMoreInfo,char index);
void NotifyEnterRoom(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, std::shared_ptr<ClientNode> RoomClient, char SeatID);
void NotifyEnterRoom1(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room, std::shared_ptr<ClientNode> RoomClient, char SeatID);

void RequestLeaveRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLeaveRoom(std::shared_ptr<ClientNode> Client, short LeaveType);
void NotifyLeaveRoom(std::shared_ptr<ClientNode> Client, UINT uin, char SeatID, bool IsChangeRoomOwner, short NewRoomOwnerID, short LeaveType);

void RequestStartGame(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseStartGame(std::shared_ptr<ClientNode> Client, const char* Reason);

void RequestChangeMap(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeMap(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);
void NotifyChangeMap(std::shared_ptr<ClientNode> Client, UINT Uin, std::shared_ptr<RoomNode> Room);

void RequestChangeSeatState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeSeatState(std::shared_ptr<ClientNode> Client, BYTE SeatID, BYTE SeatState);

void RequestKickFromRoom(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseKickFromRoom(std::shared_ptr<ClientNode> Client, UINT DstUin);

void RequestChangeReadyState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeReadyState(std::shared_ptr<ClientNode> Client, bool ReadyState);

void RequestTalk(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseTalk(std::shared_ptr<ClientNode> Client, BYTE Type, BYTE ClientUseFlag, UINT DestPlayerUin);
void NotifyTalk(std::shared_ptr<ClientNode> Client, std::shared_ptr<ClientNode> RoomClient, BYTE Type, BYTE ClientUseFlag, UINT DestPlayerUin, short ContentLength, const char* Content);
void RequestChangeRoomOwner(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyChangeRoomOwner(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);