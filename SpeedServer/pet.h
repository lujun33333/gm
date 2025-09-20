#pragma once

#include "yaml-cpp/yaml.h"
void LoadPetLevelConfig();
void GetPetInfo(std::shared_ptr<ClientNode> Client);
UINT GetPetLevel(UINT Exp);
UINT GetPetStatus(UINT Level, UINT PetID);
UINT GetPetMaxExp(UINT Level);
YAML::Node GetPetConfig(UINT PetID);
std::map<UINT, UINT> GetPetStrengLevel(UINT PetID, UINT StrengLevel);
void RequestGetPetInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //24528
void ResponseGetPetInfo(std::shared_ptr<ClientNode> Client); //24528
void RequestGetPetShowInfo(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); //24426
void ResponseGetPetShowInfo(std::shared_ptr<ClientNode> Client, UINT PETID); //24426
void ResponseGetPetShowMoreInfo(std::shared_ptr<ClientNode> Client, UINT PETID); //24527
void RequestUpdatePetName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen); // 178
void ResponseUpdatePetName(std::shared_ptr<ClientNode> Client);                            // 178
void NotifyBroadcastPetEvolution(std::shared_ptr<ClientNode> Client, UINT PetID, UINT Status);
PetInfo* GetPetItem(UINT PetID, UINT Uin);