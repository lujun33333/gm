#pragma once

#include "Message.h"
#include "ConnectionPool.h"
void InitChatRoom();
class ChatRoomNode
{
public:
	UINT ID;
	const char* Name;
	short CurrPlayerNum;
	short MaxPlayerNum;
	int RoomType;
	short SceneID;
	int x;
	int y;
	int z;
	std::shared_ptr<ClientNode> Player[99];
};
constexpr int ChatRoomNum = 7;
extern ChatRoomNode ChatRooms[ChatRoomNum];
extern ConnectionPool player;


void RequestEnterOutdoor(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void RequestChangeChairState(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

void LeaveOutdoor(std::shared_ptr<ClientNode> Client);
void RequestLeaveOutdoor(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLeaveOutdoor(std::shared_ptr<ClientNode> Client);
void NotifyLeaveOutdoor(std::shared_ptr<ClientNode> Client, UINT Uin);

void RequestHeroMove(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);

