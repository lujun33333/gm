#include "Message.h"
#include "Database.h"
#include <mutex>
#include <stdio.h>
#include "z_debug.h"
#include "ConnectionPool.h"
#include "DBConnection.h"

std::mutex RegsiterMutex;
extern ConnectionPool usr;
extern UINT isinvite;
extern int isLog;
extern int PreventSDP[2];
extern std::vector<std::string> IpList;
void MyResponseGetUin(std::shared_ptr<ClientNode> Client, UINT Uin)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, 0);
	Write32(p, Uin);
	if (Uin > 10000)
	{
		CheckDupUser(Uin, NULL);
		std::string RandomKey = GetKey(Uin);
		if (RandomKey.empty())
		{
			std::lock_guard<std::mutex> lg(KeysMutex);
			gen_randomcdk(RandomKey.data(), 16);
			Keys[Uin] = RandomKey;
			UserDB_UpdateLoginKey(Uin, RandomKey.c_str());
		}
		memcpy(p, RandomKey.c_str(), 16);
		p += 16;

	}

	len = p - buf.data();
	SendToClient(Client, 1, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Response);

}

void MyRequestGetUin(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{

	size_t len;
	char UserName[64] = { 0 };
	char Password[64] = { 0 };
	DWORD Uin = 0;
	len = Read8(Body);
	if (len > 64)
	{
		goto Reply;
	}
	memcpy(UserName, Body, len);
	Body += len;

	len = Read8(Body);
	if (len > 64)
	{
		goto Reply;
	}
	memcpy(Password, Body, len);
	Body += len;

	Uin = UserDB_GetUin(Client->Ip, UserName, Password);

Reply:

	//printf("UserName:%s\n", UserName);
	if (isLog == 1) {
		auto ServerLogger = spdlog::get("SpeedServer_logger");
		ServerLogger->info("[玩家登陆][Uin:{:d}][账号:{:s}][密码:{:s}]", Uin, UserName, Password);
		ServerLogger->flush();
	}


	MyResponseGetUin(Client, Uin);
}

void MyResponseRegister(std::shared_ptr<ClientNode> Client, USHORT Result)
{
	std::vector<BYTE> buf(819200); BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;

	Write16(p, Result);

	len = p - buf.data();
	SendToClient(Client, 2, buf.data(), len, Client->GameID, FE_GAMELOGIC, Client->ConnID, Response);
}


void MyRequestRegister(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	
	int result;
	char cdkactive = 0;
	USHORT Result = 1;
	size_t len;

	len = Read8(Body);
	char* UserName = new char[len + 1];
	memcpy(UserName, Body, len);
	Body += len;
	UserName[len] = 0;

	len = Read8(Body);
	char* Password = new char[len + 1];
	memcpy(Password, Body, len);
	Body += len;
	Password[len] = 0;

	len = Read8(Body);
	char* cdk = new char[len + 1];
	memcpy(cdk, Body, len);
	Body += len;
	cdk[len] = 0;

	auto ServerLogger = spdlog::get("SpeedServer_logger");
	//ServerLogger->info("[赛车文件异常][Uin:{:d}][ConnID:{:d}][MapID:{:d}]", Players[erri]->Uin, Players[erri]->KartID, Players[erri]->KartSkinID);
	ServerLogger->info("[用户注册][Uin:{:d}][账号:{:s}][密码:{:s}]", Client->Uin, UserName, Password);
	ServerLogger->flush();
	std::lock_guard<std::mutex> lg(RegsiterMutex);
	int KeyIndex = 0;
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select KeyIndex from CdKey where KeyValue=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, cdk);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			KeyIndex = res->getUInt("KeyIndex");
			break;
		}
		res.reset();
		if (KeyIndex > 0) {
			cdkactive = 1;
		}

		if (cdkactive)
		{
			Result = UserDB_Register(UserName, Password,cdk) ? 0 : 1;
			if (Result == 0) {

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM CdKey WHERE KeyIndex=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, KeyIndex);
				prep_stmt->executeUpdate();

				prep_stmt.reset();

			}
		}
		else {
			Result = 2; //CDK不存在
		}


		delete[] UserName;
		delete[] Password;
		delete[] cdk;
		MyResponseRegister(Client, Result);
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("MyRequestRegister Error\n");
		//throw;关闭
	}
}

void MyRequestRegisterNew(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	size_t len;

	len = Read8(Body);
	char* UserName = new char[len + 1];
	memcpy(UserName, Body, len);
	Body += len;
	UserName[len] = 0;

	len = Read8(Body);
	char* Password = new char[len + 1];
	memcpy(Password, Body, len);
	Body += len;
	Password[len] = 0;

	len = Read8(Body);
	char* cdk = new char[len + 1];
	memcpy(cdk, Body, len);
	Body += len;
	cdk[len] = 0;

	len = Read8(Body);
	char* yqm = new char[len + 1];
	memcpy(yqm, Body, len);
	Body += len;
	yqm[len] = 0;


	auto ServerLogger = spdlog::get("SpeedServer_logger");
	//ServerLogger->info("[赛车文件异常][Uin:{:d}][ConnID:{:d}][MapID:{:d}]", Players[erri]->Uin, Players[erri]->KartID, Players[erri]->KartSkinID);
	ServerLogger->info("[用户注册][Uin:{:d}][账号:{:s}][密码:{:s}][邀请码:{:s}]", Client->Uin, UserName, Password, yqm);
	ServerLogger->flush();

	int cdkactive = 0;
	USHORT Result = 1;
	int KeyIndex = 0;
	std::lock_guard<std::mutex> lg(RegsiterMutex);
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select KeyIndex from CdKey where KeyValue=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, cdk);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			KeyIndex = res->getUInt("KeyIndex");
			break;
		}
		res.reset();

		if (KeyIndex > 0) {
			cdkactive = 1;
		}
		else
		{
			Result = 2; //CDK不存在
			goto Reply;
		}

		if (isinvite == 1) {

			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select MyInviteCode from user where MyInviteCode=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, yqm);

			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			if (res->rowsCount() == 0) {
				res.reset();
				Result = 3; //邀请码不存在
				goto Reply;
			}

		}


		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Uin FROM User WHERE Name = ?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, UserName);

			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			if (res->rowsCount() > 0) {
				res.reset();
				Result = 4; //账号已存在
				goto Reply;
			}
		}

		if (cdkactive == 1)
		{
			if (isinvite == 1) {
				Result = UserDB_RegisterNew(UserName, Password, yqm) ? 0 : 1;
			}
			else
			{
				Result = UserDB_Register(UserName, Password, cdk) ? 0 : 1;
			}
			if (Result == 0) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM CdKey WHERE KeyIndex=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, KeyIndex);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
		}

	Reply:

		delete[] UserName;
		delete[] Password;
		delete[] cdk;
		delete[] yqm;
		MyResponseRegister(Client, Result);
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("MyRequestRegisterNew Error\n");
		//throw;关闭
	}


}
