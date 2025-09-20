#pragma once
#include "Message.h"
class RoomNode;
int DanceMusicDuration(int MusicID);


void NotifyDanceGameBegin(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);
void NotifyDanceRaceBegin(std::shared_ptr<ClientNode> Client, std::shared_ptr<RoomNode> Room);
void RequestDancePrepareReady(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);


void RequestDanceChangeMusic(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDanceChangeMusic(std::shared_ptr<ClientNode> Client, int MusicID);

void RequestDanceChangeMode(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDanceChangeMode(std::shared_ptr<ClientNode> Client, int Mode);

void RequestDanceChangeRandKeyFlag(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDanceChangeRandKeyFlag(std::shared_ptr<ClientNode> Client, BYTE RandKeyFlag);

void RequestDanceStepStatistics(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseDanceStepStatistics(std::shared_ptr<ClientNode> Client, UINT Uin, UINT ScoresInc, UINT ScoresTotal);
void NotifyDanceStepStatistics(std::shared_ptr<ClientNode> Client, UINT Uin, UINT ScoresInc, UINT ScoresTotal);

void RequestChangeScene(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseChangeScene(std::shared_ptr<ClientNode> Client, int SceneID);

void RequestLoverDanceChangeOneMode(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void ResponseLoverDanceChangeOneMode(std::shared_ptr<ClientNode> Client, int Mode);
void RequestLoverDanceStepStatistics(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen);
void NotifyLoverDanceStepStatistics(std::shared_ptr<ClientNode> Client, char TeamID, UINT ScoresInc, UINT ScoresTotal);


void NotifyDanceShow(std::shared_ptr<ClientNode> Client);
void NotifyDanceRaceOver(std::shared_ptr<ClientNode> Client, std::vector<std::shared_ptr<ClientNode>>& Players, char CurrentPlayers);
void NotifyDanceGameOver(std::shared_ptr<ClientNode> Client);

