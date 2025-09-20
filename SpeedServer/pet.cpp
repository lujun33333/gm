#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Message.h"
#include "Pet.h"
#include <map>
#include "DBConnection.h"
std::map<UINT, UINT> Levels;
std::map<UINT, UINT> PetStrengs;
std::map<UINT, UINT> PetHuanHuas;
UINT PetItemID;
UINT PetItemNum;
UINT PetSuperMoney;

extern std::string GetItemName(UINT Itemid);
void LoadPetLevelConfig()
{
	//return;
	try
	{
		Levels.clear();
		PetStrengs.clear();
		PetHuanHuas.clear();
		YAML::Node Config = YAML::LoadFile(".\\Config\\Pet.yml");

		for (size_t i = 0; i < Config["Levels"].size(); i++)
		{
			YAML::Node Level = Config["Levels"][i];

			Levels[Level["Level"].as<UINT>()] = Level["Exp"].as<UINT>();
		}
		for (size_t i = 0; i < Config["PetStrengs"].size(); i++)
		{
			YAML::Node PetStreng = Config["PetStrengs"][i];
			PetStrengs[PetStreng["ID"].as<UINT>()] = PetStreng["ID"].as<UINT>();
		}
		for (size_t i = 0; i < Config["PetHuanHuas"].size(); i++)
		{
			YAML::Node PetHuanHua = Config["PetHuanHuas"][i];
			PetHuanHuas[PetHuanHua["ID"].as<UINT>()] = PetHuanHua["ID"].as<UINT>();
		}
		for (size_t i = 0; i < Config["PetAllStatus"].size(); i++)
		{
			YAML::Node PetAllStatus = Config["PetAllStatus"][i];
			PetStrengs[PetAllStatus["ID"].as<UINT>()] = PetAllStatus["ID"].as<UINT>();
			PetHuanHuas[PetAllStatus["ID"].as<UINT>()] = PetAllStatus["ID"].as<UINT>();
		}

		YAML::Node PetItem = Config["PetItem"];
		if (PetItem.size() > 0) {
			PetItemID = PetItem[0]["ItemId"].as<int>();
			PetItemNum = PetItem[0]["ItemNum"].as<int>();
			PetSuperMoney = PetItem[0]["SuperMoney"].as<int>();
		}

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("Load PetStrengs Error\n");
		exit(0);
	}
	printf("Load PetStrengs Success!\n");
}
PetInfo* GetPetItem(UINT PetID, UINT Uin)
{
	PetInfo* pPetItem = new PetInfo;
	pPetItem->ID = -1;

	DBConnection myDBConnection(player);auto conn = myDBConnection.getConnection(); if (!conn) return pPetItem;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Name,Experience,PL,Status,StrengLevel FROM Pet WHERE Uin=? AND PetId=?;"));

		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, PetID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			pPetItem->ID = PetID;
			char PetName[23] = "";
			memcpy(PetName, res->getString("Name").c_str(), 23);
			sprintf_s(pPetItem->Name, PetName);

			pPetItem->Exp = res->getUInt("Experience");
			pPetItem->PL = res->getUInt("PL");
			pPetItem->Level = GetPetLevel(pPetItem->Exp);
			pPetItem->MaxExp = GetPetMaxExp(pPetItem->Level);
			pPetItem->Status = res->getUInt("Status");
			pPetItem->StrengLevel = res->getUInt("StrengLevel");
			break;
		}
		res.reset();
		
		return pPetItem;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetPetItem error\n");
	}
}


void GetPetInfo(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player);auto conn = myDBConnection.getConnection(); if (!conn) return;
	bool hasPet = false;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Name,Experience,PL,Status,StrengLevel FROM Pet WHERE Uin=? AND PetId=?;"));

		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->setUInt(2, Client->Pet->ID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			hasPet = true;
			char PetName[23] = "";
			memcpy(PetName, res->getString("Name").c_str(), 23);
			printf_s(Client->Pet->Name, PetName);

			Client->Pet->Exp = res->getUInt("Experience");;
			Client->Pet->PL = res->getUInt("PL");
			Client->Pet->Level = GetPetLevel(Client->Pet->Exp);
			Client->Pet->MaxExp = GetPetMaxExp(Client->Pet->Level);
			Client->Pet->Status = res->getUInt("Status");
			Client->Pet->StrengLevel = res->getUInt("StrengLevel");
			break;
		}
		res.reset();

		if (!hasPet)
		{

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Pet(Uin,PetId,Name,Experience,PL) VALUES (?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, Client->Pet->ID);
			prep_stmt->setString(3, G2U(GetItemName(Client->Pet->ID).c_str()));
			prep_stmt->setUInt(4, 238966);
			prep_stmt->setUInt(5, 0);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
			GetPetInfo(Client);
		}
		
	}
	catch (const std::exception& e)
	{
		
		printf(e.what());
		printf("GetPetInfo error\n");
	}
}

UINT GetPetLevel(UINT Exp)
{
	UINT Level = 0;
	for (size_t i = 1; i <= 30; i++)
	{
		if (Exp < Levels[i])
		{
			break;
		}
		Level++;
	}
	return Level;
}

UINT GetPetStatus(UINT Level, UINT PetID)
{
	UINT Status = 1;
	if (Level >= 30)
	{
		Status = 3;
		if (PetHuanHuas[PetID]) {
			Status = 4;
		}
		else {
			PetHuanHuas.erase(PetID);
		}
		if (PetStrengs[PetID]) {
			Status = 4;
		}
		else {
			PetStrengs.erase(PetID);
		}
	}
	else if (Level >= 20)
	{
		Status = 2;
	}
	return Status;
}

UINT GetPetMaxExp(UINT Level)
{
	if (Level < 30)
	{
		Level += 1;
	}
	return Levels[Level];
}

std::map<UINT, UINT> GetPetStrengLevel(UINT PetID, UINT StrengLevel)
{
	char FilePath[MAX_PATH];
	YAML::Node Config;
	std::map<UINT, UINT> StrengLevelMap;
	try
	{
		sprintf_s(FilePath, ".\\Pet\\%d.yml", PetID);
		bool isFileExists;
		struct stat buffer;
		isFileExists = (stat(FilePath, &buffer) == 0);

		int CompensateParamPet = 1;
		int CrashCompensatePowerPet = 2;
		int SpeedWeightPet = 4;
		int JetWeightPet = 6;
		int SJetWeightPet = 7;
		int AccuWeightPet = 8;

		StrengLevelMap[CompensateParamPet] = 0;
		StrengLevelMap[CrashCompensatePowerPet] = 0;
		StrengLevelMap[SpeedWeightPet] = 0;
		StrengLevelMap[JetWeightPet] = 0;
		StrengLevelMap[SJetWeightPet] = 0;
		StrengLevelMap[AccuWeightPet] = 0;

		if (isFileExists)
		{
			Config = YAML::LoadFile(FilePath);
			for (size_t i = 0; i < Config.size(); i++)
			{
				if (StrengLevel < i + 1) {
					break;
				}
				std::string StrengLevel = "StrengLevel" + std::to_string(i + 1);
				YAML::Node StrengLevelSkill = Config[StrengLevel];

				for (size_t j = 0; j < StrengLevelSkill.size(); j++)
				{
					switch (StrengLevelSkill[j]["SkillID"].as<UINT>())
					{
					case 1://落后补偿
						StrengLevelMap[1] += StrengLevelSkill[j]["Value"].as<int>();
						break;
					case 2://碰撞保护
						StrengLevelMap[2] += StrengLevelSkill[j]["Value"].as<int>();
						break;
					case 4://引擎宝宝
						StrengLevelMap[4] += StrengLevelSkill[j]["Value"].as<int>();
						break;
					case 6://燃料宝宝
						StrengLevelMap[6] += StrengLevelSkill[j]["Value"].as<int>();
						break;
					case 7://点火宝宝
						StrengLevelMap[7] += StrengLevelSkill[j]["Value"].as<int>();
						break;
					case 8://进气宝宝
						StrengLevelMap[8] += StrengLevelSkill[j]["Value"].as<int>();
						break;
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("GetPetStrengLevel Error\n");

	}
	return StrengLevelMap;
}


YAML::Node GetPetConfig(UINT PetID)
{
	char FilePath[MAX_PATH];
	YAML::Node Config;

	sprintf_s(FilePath, ".\\Pet\\%d.yml", PetID);
	bool isFileExists;
	struct stat buffer;
	isFileExists = (stat(FilePath, &buffer) == 0);

	if (isFileExists)
	{
		Config = YAML::LoadFile(FilePath);
	}
	else
	{
		printf("缺少 Pet %d 的配置文件\n", PetID);;
	}

	return Config;
}

void RequestUpdatePetName(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;

	UINT Uin = Read32(p);
	UINT Time = Read32(p);

	char* PetName = (char*)p;

	DBConnection myDBConnection(player);auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Pet SET Name=? WHERE Uin = ? AND PetId = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, G2U(PetName));
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->setUInt(3, Client->Pet->ID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
		

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("RequestUpdatePetName Error\n");
		
	}

	ResponseUpdatePetName(Client);
}

void ResponseUpdatePetName(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write32(p, 0); // Result

	len = p - buf.data();
	SendToClient(Client, 178, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void RequestChangePetEquipStatus(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	BYTE* p = Body;
	UINT Uin = Read32(p);
	UINT Time = Read32(p);
	int PetID = Read32(p);
	int NewStatus = Read8(p);
	
	


	DBConnection myDBConnection(player);auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Pet SET Status=? WHERE Uin=? and PetID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, NewStatus);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setUInt(3, PetID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
		

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("RequestChangePetEquipStatus Error\n");
		
	}

	ResponseChangePetEquipStatus(Client, NewStatus);
	NotifyBroadcastPetEvolution(Client, PetID, NewStatus);
}

void ResponseChangePetEquipStatus(std::shared_ptr<ClientNode> Client, int NewStatus)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0); // ResultID
	Write8(p, NewStatus);  // NewStatus
	Write16(p, 0); // ReasonLen

	len = p - buf.data();
	SendToClient(Client, 15398, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}
void NotifyBroadcastPetEvolution(std::shared_ptr<ClientNode> Client, UINT PetID, UINT Status)
{//Lupin
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	{ //PetInfo
		BYTE* pPetInfo = p;
		Write16(pPetInfo, 0); //len
		Write32(pPetInfo, Client->Uin); //Uin
		Write32(pPetInfo, PetID); //PetId
		memcpy(pPetInfo, Client->Pet->Name, 23);
		pPetInfo += 23;
		Write8(pPetInfo, 0); //IsProper
		Write16(pPetInfo, 30); //Level
		Write8(pPetInfo, Status); //Status
		Write16(pPetInfo, 3); //PetSkillNum

		Write32(pPetInfo, Client->Pet->StrengLevel); //StrengLevel

		len = pPetInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();

	SendToClient(Client, 587, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyPetStrengCfgInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data(); size_t len;
	try
	{
		{
			std::map<UINT, UINT>::iterator iter;
			for (iter = PetStrengs.begin(); iter != PetStrengs.end();) {
				YAML::Node PetSkills = GetPetConfig(iter->second);
				//printf("%d", PetSkills.size());
				if (PetSkills.size() != 10) {
					iter = PetStrengs.erase(iter);
				}
				else {

					++iter;
				}
			}
		}
		Write32(p, PetItemID); //StrengCardId
		Write32(p, PetStrengs.size()); //PetStrengNum
		std::map<UINT, UINT>::iterator iter;
		for (iter = PetStrengs.begin(); iter != PetStrengs.end(); iter++)
		{ //PetStrengInfos
			YAML::Node PetSkills = GetPetConfig(iter->second);
			BYTE* pPetStrengInfos = p;
			Write16(pPetStrengInfos, 0); //len

			Write32(pPetStrengInfos, iter->second); //Petid
			Write32(pPetStrengInfos, 10); //LevelNum
			for (size_t i = 0; i < 10; i++)
			{ //PetStrengLevels
				BYTE* pPetStrengLevels = pPetStrengInfos;
				Write16(pPetStrengLevels, 0); //len
				Write32(pPetStrengLevels, i + 1); //Level
				Write32(pPetStrengLevels, 0); //SuccProb
				Write32(pPetStrengLevels, PetItemNum * (i + 1)); //ConsCardNum
				Write32(pPetStrengLevels, PetSuperMoney * (i + 1)); //ConsSuperMoney
				std::string StrengLevel = "StrengLevel" + std::to_string(i + 1);
				YAML::Node StrengLevelSkill = PetSkills[StrengLevel];

				Write32(pPetStrengLevels, StrengLevelSkill.size()); //SkillNum
				for (size_t j = 0; j < StrengLevelSkill.size(); j++)
				{ //SkillInfos
					BYTE* pSkillInfos = pPetStrengLevels;
					Write16(pSkillInfos, 0); //len

					Write16(pSkillInfos, StrengLevelSkill[j]["SkillID"].as<int>()); //SkillID

					Write8(pSkillInfos, j + 1); //Status
					Write8(pSkillInfos, 1); //Active
					Write32(pSkillInfos, StrengLevelSkill[j]["Value"].as<int>()); //Value


					len = pSkillInfos - pPetStrengLevels;
					Set16(pPetStrengLevels, (WORD)len);
					pPetStrengLevels += len;
				}
				Write8(pPetStrengLevels, 0); //Type
				Write32(pPetStrengLevels, 0); //HuanHuaItemPeriod
				Write32(pPetStrengLevels, 0); //BeginDate
				Write32(pPetStrengLevels, 0); //BeginTime
				Write32(pPetStrengLevels, 0); //BonusProb
				Write32(pPetStrengLevels, 0); //BonusProbBeginDate
				Write32(pPetStrengLevels, 0); //BonusProbBeginTime
				Write32(pPetStrengLevels, 0); //BonusProbEndDate
				Write32(pPetStrengLevels, 0); //BonusProbEndTime
				Write32(pPetStrengLevels, 0); //BaojiBeginNum
				Write32(pPetStrengLevels, 0); //BaojiInc

				len = pPetStrengLevels - pPetStrengInfos;
				Set16(pPetStrengInfos, (WORD)len);
				pPetStrengInfos += len;
			}
			Write32(pPetStrengInfos, 0); //HuanHuaItemID

			len = pPetStrengInfos - p;
			Set16(p, (WORD)len);
			p += len;
		}
		Write8(p, 0); //HasMoreInfo

		len = p - buf.data();
		SendToClient(Client, 20100, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
		//delete[] buf;
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("NotifyPetStrengCfgInfo Error\n");
		//throw;关闭
	}
}
void NotifyPetStrengCfgMoreInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, 0); //PetStrengNum
	for (size_t i = 0; i < 0; i++)
	{ //PetStrengInfos
		BYTE* pPetStrengInfos = p;
		Write16(pPetStrengInfos, 0); //len

		Write32(pPetStrengInfos, 0); //Petid
		Write32(pPetStrengInfos, 0); //LevelNum
		for (size_t i = 0; i < 0; i++)
		{ //PetStrengLevels
			BYTE* pPetStrengLevels = pPetStrengInfos;
			Write16(pPetStrengLevels, 0); //len

			Write32(pPetStrengLevels, 0); //Level
			Write32(pPetStrengLevels, 0); //SuccProb
			Write32(pPetStrengLevels, 0); //ConsCardNum
			Write32(pPetStrengLevels, 0); //ConsSuperMoney
			Write32(pPetStrengLevels, 0); //SkillNum
			for (size_t i = 0; i < 0; i++)
			{ //SkillInfos
				BYTE* pSkillInfos = pPetStrengLevels;
				Write16(pSkillInfos, 0); //len

				Write16(pSkillInfos, 0); //SkillID
				Write8(pSkillInfos, 0); //Status
				Write8(pSkillInfos, 0); //Active
				Write32(pSkillInfos, 0); //Value

				len = pSkillInfos - pPetStrengLevels;
				Set16(pPetStrengLevels, (WORD)len);
				pPetStrengLevels += len;
			}
			Write8(pPetStrengLevels, 0); //Type
			Write32(pPetStrengLevels, 0); //HuanHuaItemPeriod
			Write32(pPetStrengLevels, 0); //BeginDate
			Write32(pPetStrengLevels, 0); //BeginTime
			Write32(pPetStrengLevels, 0); //BonusProb
			Write32(pPetStrengLevels, 0); //BonusProbBeginDate
			Write32(pPetStrengLevels, 0); //BonusProbBeginTime
			Write32(pPetStrengLevels, 0); //BonusProbEndDate
			Write32(pPetStrengLevels, 0); //BonusProbEndTime
			Write32(pPetStrengLevels, 0); //BaojiBeginNum
			Write32(pPetStrengLevels, 0); //BaojiInc

			len = pPetStrengLevels - pPetStrengInfos;
			Set16(pPetStrengInfos, (WORD)len);
			pPetStrengInfos += len;
		}
		Write32(pPetStrengInfos, 0); //HuanHuaItemID

		len = pPetStrengInfos - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write8(p, 0); //HasNext

	len = p - buf.data();
	SendToClient(Client, 28228, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void NotifyPetStrengPetID(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, PetStrengs.size()); //PetNum
	std::map<UINT, UINT>::iterator iter;
	for (iter = PetStrengs.begin(); iter != PetStrengs.end(); iter++) {
		Write32(p, iter->second); //PetID[]
	}
	len = p - buf.data();
	SendToClient(Client, 24074, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}

void ResponseGetPetInfo(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	Write16(p, 1); //PetNum
	for (size_t i = 0; i < 1; i++)
	{ //PetInfo
		BYTE* pPetInfo = p;
		Write16(pPetInfo, 0); //len

		Write32(pPetInfo, Client->Uin); //Uin
		Write32(pPetInfo, Client->Pet->ID); //PetId
		memcpy(pPetInfo, Client->Pet->Name, 23);
		pPetInfo += 23;
		Write8(pPetInfo, 1); //IsProper
		Write16(pPetInfo, 30); //LevelLimit
		Write16(pPetInfo, 30); //Level
		Write32(pPetInfo, Client->Pet->Exp); //CurLevelExp
		Write32(pPetInfo, Client->Pet->Exp); //CurLevelMaxExp
		Write8(pPetInfo, Client->Pet->Status); //Status
		Write16(pPetInfo, 100); //PL
		Write16(pPetInfo, 100); //TotalPL
		

		PETITEM* PetItem = NULL;
		PetItem = GetPetItem(Client->Pet->ID);
		if (PetItem) {
			UINT curskill1 = PetItem->SkillID1;
			UINT curskillv1 = PetItem->SkillValue1;
			UINT curskill2 = PetItem->SkillID2;
			UINT curskillv2 = PetItem->SkillValue2;
			UINT curskill3 = PetItem->SkillID3;
			UINT curskillv3 = PetItem->SkillValue3;
			Write16(pPetInfo, 3); //PetSkillNum

			{
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); //len
				Write16(pPetSkillList, curskill1); //SkillID
				Write8(pPetSkillList, 1); //Status
				Write8(pPetSkillList, 1); //Active
				Write32(pPetSkillList, curskillv1); //Value
				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			{
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); //len
				Write16(pPetSkillList, curskill2); //SkillID
				Write8(pPetSkillList, 2); //Status
				Write8(pPetSkillList, 1); //Active
				Write32(pPetSkillList, curskillv2); //Value
				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
			{
				BYTE* pPetSkillList = pPetInfo;
				Write16(pPetSkillList, 0); //len
				Write16(pPetSkillList, curskill3); //SkillID
				Write8(pPetSkillList, 3); //Status
				Write8(pPetSkillList, 1); //Active
				Write32(pPetSkillList, curskillv3); //Value
				len = pPetSkillList - pPetInfo;
				Set16(pPetInfo, (WORD)len);
				pPetInfo += len;
			}
		}
		else
		{
			Write16(pPetInfo, 0); //PetSkillNum
		}
		
		Write32(pPetInfo, Client->Pet->StrengLevel); //StrengLevel
		Write8(pPetInfo, 4); //TopStatus
		{ //CommonInfo
			BYTE* pCommonInfo = pPetInfo;
			Write16(pCommonInfo, 0); //len

			Write32(pCommonInfo, 0); //HuanHuaLevel
			Write8(pCommonInfo, 0); //CanHuanHua

			len = pCommonInfo - pPetInfo;
			Set16(pPetInfo, (WORD)len);
			pPetInfo += len;
		}

		len = pPetInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 24528, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Response);
}