#pragma once
#include "Message.h"
#include<unordered_map>
#include <stdio.h>
#include <time.h>
#include <map>
#include "yaml-cpp/yaml.h"
#include <mutex>
#include "Database.h"
#include "Item.h"

#include <string>
#include "DBConnection.h"

UINT NpcNum = 0;
typedef struct MPhysInfo {
	UINT PID;
	UINT AffectID;
	UINT Param;
	UINT LifeTime;
}PHYSINFO;

typedef struct MCompensateParam{
	UINT Percent;
	UINT Value;
}COMPENSATEPARAM;

struct EncryptKartItem
{
	std::vector<char> EncryptData;
};
struct KartItem28346
{
	std::string ByteY;
	std::string ByteP;
};

typedef struct MCollisionBoxParam{
	UINT BoxLength;
	UINT BoxWidth;
	UINT BoxHeight;
	UINT AdjustOffset;
	UINT AdjustPickGap;
	UINT AdjustRemoteOffset;
	UINT AdjustBevelOffset;
	UINT ControllerOffset;
} COLLISIONBOXPARAM;
typedef struct MCarToCarCollisionParam {
	UINT CollisionRadius;
	UINT CollisionLength;
	UINT CollisionWidth;
	UINT CollisionHeight;
	UINT CollisionLostCtrlTime;
} CARTOCARCOLLSIONPARAM;

typedef struct MAccelStatus {
	UINT Status;
} ACCELSTATUS;

typedef struct MForwardAccel {
	UINT Key;
	UINT Value;
}FORWARDACCEL;
typedef struct MForwardDecel {
	UINT Key;
	UINT Value;
}FORWARDECEL;

typedef struct MBackwardAccel {
	UINT Key;
	UINT Value;
}BACKWARDACCEL;

typedef struct MBackwardDecel {
	UINT Key;
	UINT Value;
}BACKWARDECEL;

typedef struct MKartPhysInfo {
	UCHAR PhysInfoSize;
	PHYSINFO* PhysInfo;
	UCHAR CompensateParamSize;
	COMPENSATEPARAM* CompensateParam;
	COLLISIONBOXPARAM* CollisionBoxParam;
	CARTOCARCOLLSIONPARAM* CarToCarCollisionParam;
	UINT BaseTurnRate;
	UINT VolatileTurnRate;
	UINT MaxTurnSpeed;
	UINT MinTurnSpeed;
	UINT MaxAccuTime;
	UINT BaseAccuRate;
	UINT MaxAffectSpeed;
	UINT Gravity;
	UINT AdditionalLocalZSpeed;
	UINT StartVec;
	UINT EndVecFist;

	UINT EndVecSecon;
	UINT DirKeyForce;
	UINT DirKeyTwist;
	UINT BannerTwist;
	UINT BannerKeyTwist;
	UINT BannerVecForce;
	UINT BannerHeadForce;
	UINT SlidFricForce;

	UINT RollFricForce;
	UINT StartWec;
	UINT MaxWec;
	UINT SuaiJianTwist;
	UINT DirUpKeyForce;
	UCHAR AccelStatusSize;
	ACCELSTATUS* AccelStatus;
	UCHAR ForwardAccelSize;
	FORWARDACCEL* ForwardAccel;
	UCHAR ForwardDecelSize;
	FORWARDECEL* ForwardDecel;
	UCHAR BackwardAccelSize;
	BACKWARDACCEL* BackwardAccel;
	UCHAR BackwardDecelSize;
	BACKWARDECEL* BackwardDecel;
	UINT CollisionLoseRate;
} KARTPHYSINFO;

std::unordered_map<long, KARTPHYSINFO*> KartPhysInfos;//hashmap
int LoadKartPhys()
{
	//目标文件夹路径
	//char Dirbuff[250];
	//_getcwd(Dirbuff, 250);
	//printf("%s\n", Dirbuff);
	std::string inPath = ".\\Kart\\*.yml";//遍历文件夹下的所有.yml文件 加载赛车性能
	
	//用于查找的句柄
	long handle;
	struct _finddata_t fileinfo;
	//第一次查找
	handle = _findfirst(inPath.c_str(), &fileinfo);
	if (handle == -1)
		return -1;
	printf("Wait to Load......\n");
	try {
		do
		{
			//printf("KartID:%d\n", atoi(fileinfo.name));
			std::string outPath(".\\Kart\\");
			outPath.append(fileinfo.name);
			
			
			YAML::Node config = YAML::LoadFile(outPath);
			KARTPHYSINFO* MKartPhysInfo = new KARTPHYSINFO;
			KartPhysInfos[atoi(fileinfo.name)] = MKartPhysInfo;

			YAML::Node PhysInfo = config["PhysInfo"];
			PHYSINFO* MPhysInfo = new PHYSINFO[PhysInfo.size()];
			MKartPhysInfo->PhysInfoSize = PhysInfo.size();
			YAML::Node PhysInfoParam;
			for (size_t i = 0; i < PhysInfo.size(); i++)
			{
				PhysInfoParam = PhysInfo[i];
				{ //PhysInfo
					MPhysInfo[i].PID = PhysInfoParam["PID"].as<int>();
					MPhysInfo[i].AffectID = PhysInfoParam["AffectID"].as<int>();
					MPhysInfo[i].Param = PhysInfoParam["Param"].as<int>();
					MPhysInfo[i].LifeTime = PhysInfoParam["LifeTime"].as<int>();
				}
				PhysInfoParam.reset();
			}
			PhysInfo.reset();
			MKartPhysInfo->PhysInfo = MPhysInfo;
			
			
			YAML::Node CompensateParam = config["CompensateParam"];//落后补偿 //技能id：1
			COMPENSATEPARAM* MCompensateParam = new COMPENSATEPARAM[CompensateParam.size()];
			MKartPhysInfo->CompensateParamSize = CompensateParam.size();
			YAML::Node CompensateParamParam;
			for (size_t i = 0; i < CompensateParam.size(); i++)
			{
				CompensateParamParam = CompensateParam[i];
				{ //CompensateParam
				
					MCompensateParam[i].Percent= CompensateParamParam["Percent"].as<int>();
					MCompensateParam[i].Value=CompensateParamParam["Value"].as<int>();
				}
				CompensateParamParam.reset();
			}
			CompensateParam.reset();
			MKartPhysInfo->CompensateParam = MCompensateParam;

			{ //CollisionBoxParam
				YAML::Node CollisionBoxParam = config["CollisionBoxParam"];
				COLLISIONBOXPARAM* MCollisionBoxParam = new COLLISIONBOXPARAM;
				MCollisionBoxParam->BoxLength=CollisionBoxParam["BoxLength"].as<int>();
				MCollisionBoxParam->BoxWidth=CollisionBoxParam["BoxWidth"].as<int>();
				MCollisionBoxParam->BoxHeight= CollisionBoxParam["BoxHeight"].as<int>();
				MCollisionBoxParam->AdjustOffset= CollisionBoxParam["AdjustOffset"].as<int>();
				MCollisionBoxParam->AdjustPickGap = CollisionBoxParam["AdjustPickGap"].as<int>();
				MCollisionBoxParam->AdjustRemoteOffset =CollisionBoxParam["AdjustRemoteOffset"].as<int>();
				MCollisionBoxParam->AdjustBevelOffset = CollisionBoxParam["AdjustBevelOffset"].as<int>();
				MCollisionBoxParam->ControllerOffset = CollisionBoxParam["ControllerOffset"].as<int>();
				CollisionBoxParam.reset();
				MKartPhysInfo->CollisionBoxParam = MCollisionBoxParam;
			}
		
			
			{ //CarToCarCollisionParam
				YAML::Node CarToCarCollisionParam = config["CarToCarCollisionParam"];
				CARTOCARCOLLSIONPARAM *MCarToCarCollisionParam = new CARTOCARCOLLSIONPARAM;

				MCarToCarCollisionParam->CollisionRadius= CarToCarCollisionParam["CollisionRadius"].as<int>();
				MCarToCarCollisionParam->CollisionLength = CarToCarCollisionParam["CollisionLength"].as<int>();
				MCarToCarCollisionParam->CollisionWidth = CarToCarCollisionParam["CollisionWidth"].as<int>();
				MCarToCarCollisionParam->CollisionHeight = CarToCarCollisionParam["CollisionHeight"].as<int>();
				MCarToCarCollisionParam->CollisionLostCtrlTime = CarToCarCollisionParam["CollisionLostCtrlTime"].as<int>();
				CarToCarCollisionParam.reset();
				MKartPhysInfo->CarToCarCollisionParam = MCarToCarCollisionParam;
			}

			MKartPhysInfo->BaseTurnRate= config["BaseTurnRate"].as<int>();
			MKartPhysInfo->VolatileTurnRate = config["VolatileTurnRate"].as<int>();
			MKartPhysInfo->MaxTurnSpeed = config["MaxTurnSpeed"].as<int>();
			MKartPhysInfo->MinTurnSpeed = config["MinTurnSpeed"].as<int>();
			MKartPhysInfo->MaxAccuTime = config["MaxAccuTime"].as<int>();
			MKartPhysInfo->BaseAccuRate = config["BaseAccuRate"].as<int>();
			MKartPhysInfo->MaxAffectSpeed = config["MaxAffectSpeed"].as<int>();
			MKartPhysInfo->Gravity = config["Gravity"].as<int>(); //TODO: 悬挂; 未限制最大值!
			MKartPhysInfo->AdditionalLocalZSpeed = config["AdditionalLocalZSpeed"].as<int>();
			MKartPhysInfo->StartVec = config["StartVec"].as<int>();
			MKartPhysInfo->EndVecFist = config["EndVecFist"].as<int>();
			MKartPhysInfo->EndVecSecon = config["EndVecSecon"].as<int>();
			MKartPhysInfo->DirKeyForce = config["DirKeyForce"].as<int>();
			MKartPhysInfo->DirKeyTwist = config["DirKeyTwist"].as<int>();
			MKartPhysInfo->BannerTwist = config["BannerTwist"].as<int>();
			MKartPhysInfo->BannerKeyTwist = config["BannerKeyTwist"].as<int>();
			MKartPhysInfo->BannerVecForce = config["BannerVecForce"].as<int>();
			MKartPhysInfo->BannerHeadForce = config["BannerHeadForce"].as<int>();
			MKartPhysInfo->SlidFricForce = config["SlidFricForce"].as<int>();
			MKartPhysInfo->RollFricForce = config["RollFricForce"].as<int>();
			MKartPhysInfo->StartWec = config["StartWec"].as<int>();
			MKartPhysInfo->MaxWec = config["MaxWec"].as<int>();
			MKartPhysInfo->SuaiJianTwist = config["SuaiJianTwist"].as<int>();
			MKartPhysInfo->DirUpKeyForce = config["DirUpKeyForce"].as<int>();


			YAML::Node AccelStatus = config["AccelStatus"];
			ACCELSTATUS* MAccelStatus = new ACCELSTATUS[AccelStatus.size()];
			MKartPhysInfo->AccelStatusSize = AccelStatus.size();
			YAML::Node AccelStatusParam;
			for (size_t i = 0; i < AccelStatus.size(); i++)
			{
				AccelStatusParam = AccelStatus[i];
				{ //m_astKartPhysParam[].m_aiAccelStatus[]
					MAccelStatus[i].Status= AccelStatusParam.as<int>();
				}
				AccelStatusParam.reset();
			}
			AccelStatus.reset();
			MKartPhysInfo->AccelStatus = MAccelStatus;

			YAML::Node ForwardAccel = config["ForwardAccel"];
			FORWARDACCEL* MForwardAccel = new FORWARDACCEL[ForwardAccel.size()];
			MKartPhysInfo->ForwardAccelSize = ForwardAccel.size();
			YAML::Node ForwardAccelParam;
			for (size_t i = 0; i < ForwardAccel.size(); i++)
			{
				ForwardAccelParam = ForwardAccel[i];
				{ //ForwardAccel
				
					MForwardAccel[i].Key = ForwardAccelParam["Key"].as<int>();
					MForwardAccel[i].Value = ForwardAccelParam["Value"].as<int>();
				}
				ForwardAccelParam.reset();
			}
			ForwardAccel.reset();
			MKartPhysInfo->ForwardAccel = MForwardAccel;

			YAML::Node ForwardDecel = config["ForwardDecel"];
			FORWARDECEL* MForwardDecel = new FORWARDECEL[ForwardDecel.size()];
			MKartPhysInfo->ForwardDecelSize = ForwardDecel.size();
			YAML::Node ForwardDecelParam;
			for (size_t i = 0; i < ForwardDecel.size(); i++)
			{
				ForwardDecelParam = ForwardDecel[i];
				{ //ForwardDecel
					MForwardDecel[i].Key= ForwardDecelParam["Key"].as<int>();
					MForwardDecel[i].Value = ForwardDecelParam["Value"].as<int>();
				}
				ForwardDecelParam.reset();
			}
			ForwardDecel.reset();
			MKartPhysInfo->ForwardDecel = MForwardDecel;

			YAML::Node BackwardAccel = config["BackwardAccel"];
			BACKWARDACCEL* MBackwardAccel = new BACKWARDACCEL[BackwardAccel.size()];
			MKartPhysInfo->BackwardAccelSize = BackwardAccel.size();
			YAML::Node BackwardAccelParam;
			for (size_t i = 0; i < BackwardAccel.size(); i++)
			{
				BackwardAccelParam = BackwardAccel[i];
				{ //BackwardAccel
				
					MBackwardAccel[i].Key= BackwardAccelParam["Key"].as<int>();
					MBackwardAccel[i].Value = BackwardAccelParam["Value"].as<int>();
				}
				BackwardAccelParam.reset();
			}
			BackwardAccel.reset();
			MKartPhysInfo->BackwardAccel = MBackwardAccel;


			YAML::Node BackwardDecel = config["BackwardDecel"];
			BACKWARDECEL* MBackwardDecel = new BACKWARDECEL[BackwardDecel.size()];
			MKartPhysInfo->BackwardDecelSize = BackwardDecel.size();
			YAML::Node BackwardDecelParam;
			for (size_t i = 0; i < BackwardDecel.size(); i++)
			{
				BackwardDecelParam = BackwardDecel[i];
				{ //BackwardDecel
				
					MBackwardDecel[i].Key= BackwardDecelParam["Key"].as<int>();
					MBackwardDecel[i].Value = BackwardDecelParam["Value"].as<int>();
				}
				BackwardDecelParam.reset();
			}
			BackwardDecel.reset();
			
			MKartPhysInfo->BackwardDecel = MBackwardDecel;
			//MKartPhysInfo->CollisionLoseRate = config["CollisionLoseRate"].as<int>();//没有这个属性会抛异常
			//printf("%s\n", outPath.c_str());

		} while (!_findnext(handle, &fileinfo));
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("Load KartPhys Exception!\n");
	}

	_findclose(handle);
	printf("Load KartPhys Success!\n");
	return 0;
}

KARTPHYSINFO* GetKartPhysInfo(long KartID)
{
	KARTPHYSINFO* KartPhysInfo = NULL;
	//std::lock_guard<std::recursive_mutex> lg(KartMutex);//全是读取不需要加锁
	//DWORD dwStart = GetTickCount64(); //取windows启动到现在的流逝时间(毫秒)
	auto iter = KartPhysInfos.find(KartID);
	if (iter != KartPhysInfos.end())
	{
		KartPhysInfo = iter->second;
	}
	else
	{
		printf("缺少赛车配置文件:%d\n", KartID);
	}
	//DWORD dwUsed = GetTickCount64() - dwStart; //计算该函数所消耗的时间
	//printf("Search lost:%d ms\n", dwUsed);
	return KartPhysInfo;


}

void PrintfKart127616()
{
	KARTPHYSINFO* KartPhysInfo = GetKartPhysInfo(127616);
	UINT Size = 0;
	if (!KartPhysInfo)
	{
		return;
	}
	PHYSINFO* PhysInfo = KartPhysInfo->PhysInfo;
	Size = KartPhysInfo->PhysInfoSize;
	printf("PhysInfo:\n");
	for (char i = 0; i < Size; i++)
	{
		printf("PID: %d\n", PhysInfo[i].PID);
		printf("AffectID: %d\n", PhysInfo[i].AffectID);
		printf("Param: %d\n", PhysInfo[i].Param);
		printf("LifeTime: %d\n", PhysInfo[i].LifeTime);
	}
	printf("CompensateParam:\n");
	COMPENSATEPARAM* CompensateParam = KartPhysInfo->CompensateParam;
	Size = KartPhysInfo->CompensateParamSize;
	for (char i = 0; i < Size; i++)
	{
		printf("Percent: %d\n", CompensateParam[i].Percent);
		printf("Value: %d\n", CompensateParam[i].Value);
	}
	printf("CollisionBoxParam:\n");
	COLLISIONBOXPARAM* CollisionBoxParam = KartPhysInfo->CollisionBoxParam;
	{
		printf("BoxLength: %d\n", CollisionBoxParam->BoxLength);
		printf("BoxWidth: %d\n", CollisionBoxParam->BoxWidth);
		printf("BoxHeight: %d\n", CollisionBoxParam->BoxHeight);
		printf("AdjustOffset: %d\n", CollisionBoxParam->AdjustOffset);
		printf("AdjustPickGap: %d\n", CollisionBoxParam->AdjustPickGap);
		printf("AdjustRemoteOffset: %d\n", CollisionBoxParam->AdjustRemoteOffset);
		printf("AdjustBevelOffset: %d\n", CollisionBoxParam->AdjustBevelOffset);
		printf("ControllerOffset: %d\n", CollisionBoxParam->ControllerOffset);
	}
	printf("CarToCarCollisionParam:\n");
	CARTOCARCOLLSIONPARAM* CarToCarCollisionParam = KartPhysInfo->CarToCarCollisionParam;
	{
		printf("CollisionRadius: %d\n", CarToCarCollisionParam->CollisionRadius);
		printf("CollisionLength: %d\n", CarToCarCollisionParam->CollisionLength);
		printf("CollisionWidth: %d\n", CarToCarCollisionParam->CollisionWidth);
		printf("CollisionHeight: %d\n", CarToCarCollisionParam->CollisionHeight);
		printf("CollisionLostCtrlTime: %d\n", CarToCarCollisionParam->CollisionLostCtrlTime);
		
	}

	{
		printf("BaseTurnRate: %d\n", KartPhysInfo->BaseTurnRate);
		printf("VolatileTurnRate: %d\n", KartPhysInfo->VolatileTurnRate);
		printf("MaxTurnSpeed: %d\n", KartPhysInfo->MaxTurnSpeed);
		printf("MinTurnSpeed: %d\n", KartPhysInfo->MinTurnSpeed);
		printf("MaxAccuTime: %d\n", KartPhysInfo->MaxAccuTime);
		printf("BaseAccuRate: %d\n", KartPhysInfo->BaseAccuRate);
		printf("MaxAffectSpeed: %d\n", KartPhysInfo->MaxAffectSpeed);
		printf("Gravity: %d\n", KartPhysInfo->Gravity);
		printf("AdditionalLocalZSpeed: %d\n", KartPhysInfo->AdditionalLocalZSpeed);
		printf("StartVec: %d\n", KartPhysInfo->StartVec);
		printf("EndVecFist: %d\n", KartPhysInfo->EndVecFist);
		printf("EndVecSecon: %d\n", KartPhysInfo->EndVecSecon);
		printf("DirKeyForce: %d\n", KartPhysInfo->DirKeyForce);
		printf("DirKeyTwist: %d\n", KartPhysInfo->DirKeyTwist);
		printf("BannerTwist: %d\n", KartPhysInfo->BannerTwist);
		printf("BannerKeyTwist: %d\n", KartPhysInfo->BannerKeyTwist);
		printf("BannerVecForce: %d\n", KartPhysInfo->BannerVecForce);

		printf("BannerHeadForce: %d\n", KartPhysInfo->BannerHeadForce);
		printf("SlidFricForce: %d\n", KartPhysInfo->SlidFricForce);
		printf("RollFricForce: %d\n", KartPhysInfo->RollFricForce);
		printf("StartWec: %d\n", KartPhysInfo->StartWec);
		printf("MaxWec: %d\n", KartPhysInfo->MaxWec);

		printf("SuaiJianTwist: %d\n", KartPhysInfo->SuaiJianTwist);
		printf("DirUpKeyForce: %d\n", KartPhysInfo->DirUpKeyForce);

	}
	printf("AccelStatus:\n");
	ACCELSTATUS* AccelStatus = KartPhysInfo->AccelStatus;
	Size = KartPhysInfo->AccelStatusSize;
	for (char i = 0; i < Size; i++)
	{
		printf("Percent: %d\n", AccelStatus[i].Status);
	}
	printf("ForwardAccel:\n");
	FORWARDACCEL* ForwardAccel = KartPhysInfo->ForwardAccel;
	Size = KartPhysInfo->ForwardAccelSize;
	for (char i = 0; i < Size; i++)
	{
		printf("Percent: %d\n", ForwardAccel[i].Key);
		printf("Value: %d\n", ForwardAccel[i].Value);
	}
	printf("ForwardDecel:\n");
	FORWARDECEL* ForwardDecel = KartPhysInfo->ForwardDecel;
	Size = KartPhysInfo->ForwardDecelSize;
	for (char i = 0; i < Size; i++)
	{
		printf("Percent: %d\n", ForwardDecel[i].Key);
		printf("Value: %d\n", ForwardDecel[i].Value);
	}
	printf("BackwardAccel:\n");
	BACKWARDACCEL* BackwardAccel = KartPhysInfo->BackwardAccel;
	Size = KartPhysInfo->BackwardAccelSize;
	for (char i = 0; i < Size; i++)
	{
		printf("Percent: %d\n", BackwardAccel[i].Key);
		printf("Value: %d\n", BackwardAccel[i].Value);
	}
	printf("BackwardDecel:\n");
	BACKWARDECEL* BackwardDecel = KartPhysInfo->BackwardDecel;
	Size = KartPhysInfo->BackwardDecelSize;
	for (char i = 0; i < Size; i++)
	{
		printf("Percent: %d\n", BackwardDecel[i].Key);
		printf("Value: %d\n", BackwardDecel[i].Value);
	}



}



typedef struct MKartAltas {
	UINT ID;
	UINT KartJetNum;
}KARTALTAS;

typedef struct MKartAltasList {
	std::vector<KARTALTAS> KartAltas;
}KARTALTASLIST;
std::shared_ptr<KARTALTASLIST> MKartAltasList = NULL;
void LoadKartAltasList()
{

	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\KartAltasList.yml");
		MKartAltasList = std::make_shared<KARTALTASLIST>();//新建赛车车库列表
		YAML::Node KartAltasCfg;
		size_t Num = Config.size();
		MKartAltasList->KartAltas.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			KartAltasCfg = Config[i];
			MKartAltasList->KartAltas[i].ID= KartAltasCfg["ID"].as<int>();
			MKartAltasList->KartAltas[i].KartJetNum = KartAltasCfg["KartJetNum"].as<int>();
			KartAltasCfg.reset();
		}
		printf("Load KartAltas Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load KartAltas Exception!\n");
	}
	
}


typedef struct SkillStoneCfg
{
	UINT StoneID;
	UINT ActiveCondVal1;
	UINT UseCountUpperlimit;
	UINT ActiveSuccessProb;
	UINT CoolTime;
	UINT GenResultVal1;
}SKILLSTONECFG;

typedef struct SingleSkillStoneCfg
{
	std::vector<SKILLSTONECFG> SkillStoneCfg;
}SINGLESKILLSTONECFG;

typedef struct ItemSkillStone
{
	UINT ItemID;
	std::vector<SkillStoneCfg> StoneCfg;
}ITEMSKILLSTONE;

std::vector<ItemSkillStone> itemSkillStone;

std::shared_ptr<SINGLESKILLSTONECFG> MSingleSkillStoneCfg;
void LoadSkillStoneCfg()
{
	try
	{
		itemSkillStone.clear();

		YAML::Node Config = YAML::LoadFile(".\\Config\\SingleSkillStoneCfg.yml");
		YAML::Node ItemStoneCfg = Config["ItemCfg"];
		for (size_t i = 0; i < ItemStoneCfg.size(); i++)
		{
			ItemSkillStone IS;
			IS.ItemID = ItemStoneCfg[i]["ItemId"].as<int>();
			YAML::Node StoneCfg = ItemStoneCfg[i]["StoneId"];
			for (size_t j = 0;  j < StoneCfg.size();  j++)
			{
				SkillStoneCfg SC;
				SC.StoneID = StoneCfg[j]["StoneID"].as<int>();
				IS.StoneCfg.push_back(SC);
			}
			itemSkillStone.push_back(IS);
		}


		YAML::Node StoneCfg = Config["SingleSkillStoneCfg"];
		YAML::Node SingleSkillStoneCfg;
		size_t Num = StoneCfg.size();
		MSingleSkillStoneCfg = std::make_shared<SINGLESKILLSTONECFG>();
		MSingleSkillStoneCfg->SkillStoneCfg.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			SingleSkillStoneCfg = StoneCfg[i];
			MSingleSkillStoneCfg->SkillStoneCfg[i].StoneID = SingleSkillStoneCfg["StoneID"].as<int>();
			MSingleSkillStoneCfg->SkillStoneCfg[i].ActiveCondVal1 = SingleSkillStoneCfg["ActiveCondVal1"].as<int>();
			MSingleSkillStoneCfg->SkillStoneCfg[i].UseCountUpperlimit = SingleSkillStoneCfg["UseCountUpperlimit"].as<int>();
			MSingleSkillStoneCfg->SkillStoneCfg[i].ActiveSuccessProb = SingleSkillStoneCfg["ActiveSuccessProb"].as<int>();
			MSingleSkillStoneCfg->SkillStoneCfg[i].CoolTime = SingleSkillStoneCfg["CoolTime"].as<int>();
			MSingleSkillStoneCfg->SkillStoneCfg[i].GenResultVal1 = SingleSkillStoneCfg["GenResultVal1"].as<int>();
			SingleSkillStoneCfg.reset();
		}
		printf("Load SingleSkillStoneCfg Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load SingleSkillStoneCfg Error!\n");
	}
	
}

typedef struct NPCKartAIPhysPara {
	UINT NpcN2OIntervalTime;
	UINT NpcTriggerN2ORate;
	UINT NpcRestCheckTime;
	UINT NpcFixVelocity;
	UINT NpcFixVelocityRate;
	UINT NpcRunDisHulanThreshold;
	UINT NpcSJetRate;
	UINT NpcTurnAjustDisHulanRate;
	std::vector<FORWARDACCEL> ForwardAccel;
	std::vector<BACKWARDACCEL> BackwardAccel;
	UINT StraightLenThreshold;
	UINT NpcDriftRate;
	std::vector<COMPENSATEPARAM> CompensateParam;
	UINT N2OStartTime;
	UINT NpcN2OMaxCount;
	UINT Dis2FirstPlay;
	UINT LostControlTime;
	UINT LostInterval;
	UINT LostControlCount;

} NPCKARTPHYSPARAM;

typedef struct NPCKartAIPhysParaInfo {
	std::vector<NPCKARTPHYSPARAM> NPCKartAIPhysPara;
	UCHAR Level = 12;
}NPCKARTAIPHYSPARAINFO;
std::shared_ptr<NPCKARTAIPHYSPARAINFO> MNPCKartAIPhysParaCfg = NULL;
std::shared_ptr<NPCKARTAIPHYSPARAINFO> MNPCPlayerKartAIPhysParaCfg = NULL;
std::shared_ptr<NPCKARTAIPHYSPARAINFO> MNPCRankedMatchKartAIPhysParaCfg = NULL;

void LoadNPCKartAIPhysParaInfo()
{
	try
	{
		YAML::Node NPCKartAICfg = YAML::LoadFile(".\\Bot\\NPCKartAIPhysParaCfg.yml");
		YAML::Node NPCKartAIPhysCfg = NPCKartAICfg["NPCKartAIPhysParaInfo"];
		size_t Num = NPCKartAIPhysCfg.size();
		MNPCKartAIPhysParaCfg = std::make_shared<NPCKARTAIPHYSPARAINFO>();
		MNPCKartAIPhysParaCfg->Level = Num;
		MNPCKartAIPhysParaCfg->NPCKartAIPhysPara.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{ //NPCKartAIPhysPara
			YAML::Node NPCKartAIPhysParaCfg = NPCKartAIPhysCfg[i]["NPCKartAIPhysPara"];
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OIntervalTime = NPCKartAIPhysParaCfg["NpcN2OIntervalTime"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTriggerN2ORate = NPCKartAIPhysParaCfg["NpcTriggerN2ORate"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRestCheckTime = NPCKartAIPhysParaCfg["NpcRestCheckTime"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocity = NPCKartAIPhysParaCfg["NpcFixVelocity"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocityRate = NPCKartAIPhysParaCfg["NpcFixVelocityRate"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRunDisHulanThreshold = NPCKartAIPhysParaCfg["NpcRunDisHulanThreshold"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcSJetRate = NPCKartAIPhysParaCfg["NpcSJetRate"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTurnAjustDisHulanRate = NPCKartAIPhysParaCfg["NpcTurnAjustDisHulanRate"].as<int>();


			YAML::Node ForwardAccelCfg = NPCKartAIPhysParaCfg["ForwardAccel"];
			size_t ForwardAccelNum = ForwardAccelCfg.size();
			std::vector<FORWARDACCEL> MForwardAccel(ForwardAccelNum);
			for (size_t j = 0; j < ForwardAccelNum; j++)
			{ //ForwardAccel
				MForwardAccel[j].Key = ForwardAccelCfg[j]["Key"].as<int>();
				MForwardAccel[j].Value = ForwardAccelCfg[j]["Value"].as<int>();
			}
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel = MForwardAccel;


			YAML::Node BackwardAccelCfg = NPCKartAIPhysParaCfg["BackwardAccel"];
			size_t BackwardAccelNum = BackwardAccelCfg.size();
			std::vector<BACKWARDACCEL> MBackwardAccel(BackwardAccelNum);
			for (size_t j = 0; j < BackwardAccelNum; j++)
			{ //BackwardAccel
				
				MBackwardAccel[j].Key = BackwardAccelCfg[j]["Key"].as<int>();
				MBackwardAccel[j].Value = BackwardAccelCfg[j]["Value"].as<int>();
			}
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel = MBackwardAccel;

			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCKartAIPhysParaCfg["StraightLenThreshold"].as<int>();
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCKartAIPhysParaCfg["NpcDriftRate"].as<int>();

			

			YAML::Node CompensateParamCfg = NPCKartAIPhysParaCfg["CompensateParam"];
			size_t CompensateParamNum = CompensateParamCfg.size();
			std::vector<COMPENSATEPARAM> MCompensateParam(CompensateParamNum);
			for (size_t j = 0; j < CompensateParamNum; j++)
			{ //CompensateParam
				MCompensateParam[j].Percent = CompensateParamCfg[j]["Percent"].as<int>();
				MCompensateParam[j].Value = CompensateParamCfg[j]["Value"].as<int>();
			}
			MNPCKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam = MCompensateParam;
		
			NPCKartAIPhysParaCfg.reset();
		}
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf(" Load NPCKartAIPhysParaCfg exception!\n");
	}

	try
	{
		YAML::Node NPCPlayerKartAICfg = YAML::LoadFile(".\\Bot\\NPCPlayerKartAIPhysParaCfg.yml");
		YAML::Node NPCPlayerKartAIPhysCfg = NPCPlayerKartAICfg["NPCKartAIPhysParaInfo"];
		size_t Num = NPCPlayerKartAIPhysCfg.size();
		MNPCPlayerKartAIPhysParaCfg = std::make_shared<NPCKARTAIPHYSPARAINFO>();
		MNPCPlayerKartAIPhysParaCfg->Level = Num;
		MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara.resize(Num);

		for (size_t i = 0; i < Num; i++)
		{ //NPCKartAIPhysPara
			YAML::Node NPCPlayerCKartAIPhysParaCfg = NPCPlayerKartAIPhysCfg[i]["NPCKartAIPhysPara"];
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OIntervalTime = NPCPlayerCKartAIPhysParaCfg["NpcN2OIntervalTime"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTriggerN2ORate = NPCPlayerCKartAIPhysParaCfg["NpcTriggerN2ORate"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRestCheckTime = NPCPlayerCKartAIPhysParaCfg["NpcRestCheckTime"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocity = NPCPlayerCKartAIPhysParaCfg["NpcFixVelocity"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocityRate = NPCPlayerCKartAIPhysParaCfg["NpcFixVelocityRate"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRunDisHulanThreshold = NPCPlayerCKartAIPhysParaCfg["NpcRunDisHulanThreshold"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcSJetRate = NPCPlayerCKartAIPhysParaCfg["NpcSJetRate"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTurnAjustDisHulanRate = NPCPlayerCKartAIPhysParaCfg["NpcTurnAjustDisHulanRate"].as<int>();


			YAML::Node ForwardAccelCfg = NPCPlayerCKartAIPhysParaCfg["ForwardAccel"];
			size_t ForwardAccelNum = ForwardAccelCfg.size();

			std::vector<FORWARDACCEL> MForwardAccel(ForwardAccelNum);
			for (size_t j = 0; j < ForwardAccelNum; j++)
			{ //ForwardAccel
				MForwardAccel[j].Key = ForwardAccelCfg[j]["Key"].as<int>();
				MForwardAccel[j].Value = ForwardAccelCfg[j]["Value"].as<int>();
			}
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel = MForwardAccel;


			YAML::Node BackwardAccelCfg = NPCPlayerCKartAIPhysParaCfg["BackwardAccel"];
			size_t BackwardAccelNum = BackwardAccelCfg.size();
			std::vector<BACKWARDACCEL> MBackwardAccel(BackwardAccelNum);
			for (size_t j = 0; j < BackwardAccelNum; j++)
			{ //BackwardAccel

				MBackwardAccel[j].Key = BackwardAccelCfg[j]["Key"].as<int>();
				MBackwardAccel[j].Value = BackwardAccelCfg[j]["Value"].as<int>();
			}
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel = MBackwardAccel;

			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCPlayerCKartAIPhysParaCfg["StraightLenThreshold"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCPlayerCKartAIPhysParaCfg["NpcDriftRate"].as<int>();



			YAML::Node CompensateParamCfg = NPCPlayerCKartAIPhysParaCfg["CompensateParam"];
			size_t CompensateParamNum = CompensateParamCfg.size();
			std::vector<COMPENSATEPARAM> MCompensateParam(CompensateParamNum);
			for (size_t j = 0; j < CompensateParamNum; j++)
			{ //CompensateParam
				MCompensateParam[j].Percent = CompensateParamCfg[j]["Percent"].as<int>();
				MCompensateParam[j].Value = CompensateParamCfg[j]["Value"].as<int>();
			}
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam = MCompensateParam;
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCPlayerCKartAIPhysParaCfg["N2OStartTime"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCPlayerCKartAIPhysParaCfg["NpcN2OMaxCount"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCPlayerCKartAIPhysParaCfg["Dis2FirstPlay"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCPlayerCKartAIPhysParaCfg["LostControlTime"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCPlayerCKartAIPhysParaCfg["LostInterval"].as<int>();
			MNPCPlayerKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCPlayerCKartAIPhysParaCfg["LostControlCount"].as<int>();

			NPCPlayerCKartAIPhysParaCfg.reset();
		}

	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf(" Load NPCPlayerKartAIPhysParaCfg exception!\n");
	}

	try
	{
		YAML::Node NPCKartAICfg = YAML::LoadFile(".\\Bot\\NPCRankedMatchKartAIPhysParaCfg.yml");
		YAML::Node NPCKartAIPhysCfg = NPCKartAICfg["NPCKartAIPhysParaInfo"];
		size_t Num = NPCKartAIPhysCfg.size();
		MNPCRankedMatchKartAIPhysParaCfg = std::make_shared<NPCKARTAIPHYSPARAINFO>();
		MNPCRankedMatchKartAIPhysParaCfg->Level = Num;
		MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara.resize(Num);
		const int MAX_NPC_AI_LEVEL_NUM = 12;

		for (size_t i = 0; i < MAX_NPC_AI_LEVEL_NUM; i++)
		{ //NPCKartAIPhysPara
			YAML::Node NPCKartAIPhysParaCfg = NPCKartAIPhysCfg[i]["NPCKartAIPhysPara"];
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcN2OIntervalTime = NPCKartAIPhysParaCfg["NpcN2OIntervalTime"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTriggerN2ORate = NPCKartAIPhysParaCfg["NpcTriggerN2ORate"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRestCheckTime = NPCKartAIPhysParaCfg["NpcRestCheckTime"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocity = NPCKartAIPhysParaCfg["NpcFixVelocity"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcFixVelocityRate = NPCKartAIPhysParaCfg["NpcFixVelocityRate"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcRunDisHulanThreshold = NPCKartAIPhysParaCfg["NpcRunDisHulanThreshold"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcSJetRate = NPCKartAIPhysParaCfg["NpcSJetRate"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcTurnAjustDisHulanRate = NPCKartAIPhysParaCfg["NpcTurnAjustDisHulanRate"].as<int>();


			YAML::Node ForwardAccelCfg = NPCKartAIPhysParaCfg["ForwardAccel"];
			size_t ForwardAccelNum = ForwardAccelCfg.size();
			std::vector<FORWARDACCEL> MForwardAccel(ForwardAccelNum);
			for (size_t j = 0; j < ForwardAccelNum; j++)
			{ //ForwardAccel
				MForwardAccel[j].Key = ForwardAccelCfg[j]["Key"].as<int>();
				MForwardAccel[j].Value = ForwardAccelCfg[j]["Value"].as<int>();
			}
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].ForwardAccel = MForwardAccel;


			YAML::Node BackwardAccelCfg = NPCKartAIPhysParaCfg["BackwardAccel"];
			size_t BackwardAccelNum = BackwardAccelCfg.size();
			std::vector<BACKWARDACCEL> MBackwardAccel(BackwardAccelNum);
			for (size_t j = 0; j < BackwardAccelNum; j++)
			{ //BackwardAccel

				MBackwardAccel[j].Key = BackwardAccelCfg[j]["Key"].as<int>();
				MBackwardAccel[j].Value = BackwardAccelCfg[j]["Value"].as<int>();

			}
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].BackwardAccel = MBackwardAccel;

			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCKartAIPhysParaCfg["StraightLenThreshold"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCKartAIPhysParaCfg["NpcDriftRate"].as<int>();



			YAML::Node CompensateParamCfg = NPCKartAIPhysParaCfg["CompensateParam"];
			size_t CompensateParamNum = CompensateParamCfg.size();
			std::vector<COMPENSATEPARAM> MCompensateParam(CompensateParamNum);
			for (size_t j = 0; j < CompensateParamNum; j++)
			{ //CompensateParam
				MCompensateParam[j].Percent = CompensateParamCfg[j]["Percent"].as<int>();
				MCompensateParam[j].Value = CompensateParamCfg[j]["Value"].as<int>();
			}
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].CompensateParam = MCompensateParam;
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCKartAIPhysParaCfg["N2OStartTime"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCKartAIPhysParaCfg["NpcN2OMaxCount"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCKartAIPhysParaCfg["Dis2FirstPlay"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCKartAIPhysParaCfg["LostControlTime"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].StraightLenThreshold = NPCKartAIPhysParaCfg["LostInterval"].as<int>();
			MNPCRankedMatchKartAIPhysParaCfg->NPCKartAIPhysPara[i].NpcDriftRate = NPCKartAIPhysParaCfg["LostControlCount"].as<int>();

			NPCKartAIPhysParaCfg.reset();
		}
		printf("Load NPCKartAIPhysParaCfg Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf(" Load NPCRankedMatchKartAIPhysParaCfg exception!\n");
	}
	
	
}

typedef struct TopListDesc {
	UINT ID;
	char Name[MaxNickName];
	char ValueDesc[MaxNickName];
	char TitleName[MaxNickName];
	BOOL IsEnableSelfRank;
} TOPLISTDESC;
std::vector<TOPLISTDESC> TopListDescs;

void LoadTopListDesc()
{
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\TopListDesc-QQSpeed.yml");//转换TopListDesc-QQSpeed.yml为ansi编码 否则utf-8可能乱码报错
		USHORT Count = (USHORT)Config.size();
		YAML::Node Descs;
		TopListDescs.resize(Count);
		for (USHORT i = 0; i < Count; i++)
		{ //Descs
			Descs = Config[i];
			TopListDescs[i].ID = Descs["ID"].as<int>();
			std::string Name = Descs["Name"].as<std::string>();
			std::string ValueDesc = Descs["ValueDesc"].as<std::string>();
			std::string TitleName = Descs["TitleName"].as<std::string>();
			memset(TopListDescs[i].Name, 0, MaxNickName);
			memcpy(TopListDescs[i].Name, Name.c_str(), Name.length());
			memset(TopListDescs[i].ValueDesc, 0, MaxNickName);
			memcpy(TopListDescs[i].ValueDesc, ValueDesc.c_str(), ValueDesc.length());
			memset(TopListDescs[i].TitleName, 0, MaxNickName);
			memcpy(TopListDescs[i].TitleName, TitleName.c_str(), TitleName.length());
			TopListDescs[i].IsEnableSelfRank = Descs["IsEnableSelfRank"].as<bool>();
			Descs.reset();
		}
		printf("Load TopListDesc Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load TopListDesc Error\n");
	}
	
}

typedef struct NPCListInfo {
	UINT NpcID;
	UINT MaxLevel;
	char RoomName[17];
	UINT RoleID;
	UINT HairID;
	UINT FaceID;
	UINT GlassID;
	UINT CoatID;
	UINT GloveID;
	UINT TrousersID;
	UINT PersonarID;
	UINT TattooID;
	UINT WingID;
	UINT DecoratorID;
	UINT KartID;
	UINT ColorID;
	UINT TrackID;
	UINT EffectID;
	UINT BrandID;
	UINT TailID;
}NPCLISTINFO;
std::vector<NPCLISTINFO> NpcListInfos;

void LoadNPCListInfo()
{
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Bot\\NPCListInfo.yml");
		size_t Num = Config.size();
		NpcNum = Num;
		NpcListInfos.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			//NPCInfo
			NpcListInfos[i].NpcID = Config[i]["NpcID"].as<int>();
			std::string RoomName = Config[i]["RoomName"].as<std::string>();
			memset(NpcListInfos[i].RoomName, 0, 17);
			memcpy(NpcListInfos[i].RoomName, RoomName.c_str(), RoomName.length());

			NpcListInfos[i].RoleID = Config[i]["RoleID"].as<int>();
			NpcListInfos[i].HairID = Config[i]["HairID"].as<int>();
			NpcListInfos[i].FaceID = Config[i]["FaceID"].as<int>();
			NpcListInfos[i].GlassID = Config[i]["GlassID"].as<int>();
			NpcListInfos[i].CoatID = Config[i]["CoatID"].as<int>();
			NpcListInfos[i].GloveID = Config[i]["GloveID"].as<int>();
			NpcListInfos[i].TrousersID = Config[i]["TrousersID"].as<int>();
			NpcListInfos[i].PersonarID = Config[i]["PersonarID"].as<int>();
			NpcListInfos[i].TattooID = Config[i]["TattooID"].as<int>();
			NpcListInfos[i].WingID = Config[i]["WingID"].as<int>();
			NpcListInfos[i].DecoratorID = Config[i]["DecoratorID"].as<int>();
			NpcListInfos[i].DecoratorID = Config[i]["DecoratorID"].as<int>();
			NpcListInfos[i].KartID = Config[i]["KartID"].as<int>();
			NpcListInfos[i].ColorID = Config[i]["ColorID"].as<int>();
			NpcListInfos[i].TrackID = Config[i]["TrackID"].as<int>();
			NpcListInfos[i].EffectID = Config[i]["EffectID"].as<int>();
			NpcListInfos[i].BrandID = Config[i]["BrandID"].as<int>();
			NpcListInfos[i].TailID = Config[i]["TailID"].as<int>();
			NpcListInfos[i].MaxLevel = Config[i]["MaxLevel"].as<int>();
		}
		printf("Load NPCListInfo Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load NPCListInfo Error!\n");
	}

}

typedef struct KartSkill
{
	UINT SuperN2O1;
	UINT SuperN2O2;
	UINT SkillHide;
	UINT SkillView;
	UINT ConvertSpeed;
	UINT SuperN2ORate;
	UINT SuperN2OTime;
}KARTSKILL;
std::unordered_map<long, std::shared_ptr<KARTSKILL>> KartSkills;//hashmap
void LoadKartSkill()
{
	KartSkills.clear();
	//查询赛车技能
	DBConnection myDBConnection(player);auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM KartSkill;"));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			std::shared_ptr<KARTSKILL> KartSkill = std::make_shared<KARTSKILL>();
			KartSkills[res->getUInt(1)] = KartSkill;
			KartSkill->SuperN2O1 = res->getUInt(2);
			KartSkill->SuperN2O2 = res->getUInt(3);
			KartSkill->SkillHide = res->getUInt(4);
			KartSkill->SkillView = res->getUInt(5);
			KartSkill->ConvertSpeed = res->getUInt(6);
			KartSkill->SuperN2ORate = res->getUInt(7);
			KartSkill->SuperN2OTime = res->getUInt(8);
		}
		res.reset();
		printf("Load KartSkill Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("LoadKartSkill error\n");
	}	
}

KARTSKILL* GetKartSkill(long KartID)
{
	KARTSKILL* KartSkill = NULL;
	auto iter = KartSkills.find(KartID);
	if (iter != KartSkills.end())
	{
		KartSkill = iter->second.get();
	}
	else
	{
	}

	return KartSkill;
}

typedef struct Sign3Award
{
	UINT Experience;
	UINT SuperMoney;
	std::vector<ItemInfo> Items;
}SIGN3AWARD;
std::vector<SIGN3AWARD> Sign3Awrad;
void LoadSign3Cfg()
{
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\Sign3Cfg.yml");
		YAML::Node ItemsCfg;
		YAML::Node ItemCfg;
		int ItemNum = 0;
		size_t Num = Config.size();//7个

		Sign3Awrad.resize(Num);
		for (size_t i = 0; i < Num; i++)
		{
			Sign3Awrad[i].Experience = Config[i]["Experience"].as<int>();
			Sign3Awrad[i].SuperMoney = Config[i]["SuperMoney"].as<int>();
			//	printf("Experience:%d\n", Sign3Awrad[i].Experience);
			//	printf("SuperMoney:%d\n", Sign3Awrad[i].SuperMoney);
			ItemsCfg = Config[i]["Items"];
			ItemNum = ItemsCfg.size();
			Sign3Awrad[i].Items.resize(ItemNum);
			for (int j = 0; j < ItemNum; j++)
			{
				ItemCfg = ItemsCfg[j];

				Sign3Awrad[i].Items[j].ItemID = ItemCfg["ItemId"].as<int>();
				Sign3Awrad[i].Items[j].ItemNum = ItemCfg["ItemNum"].as<int>();
				Sign3Awrad[i].Items[j].AvailPeriod = ItemCfg["AvailPeriod"].as<int>();
				Sign3Awrad[i].Items[j].ItemType = ItemCfg["ItemType"].as<int>();
				//printf("ItemId:%d\n", Sign3Awrad[i].Items[j].ItemID);
				ItemCfg.reset();
			}	
		}
		printf("Load Sign3Cfg Success!\n");
	}
	catch (const std::exception& e)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		printf(e.what());
		printf("Load Sign3Cfg Error!\n");
	}
	
}

void initGameBegin();