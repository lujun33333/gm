#include "Message.h"
#include "Database.h"
#include <string>
#include<unordered_map>
#include "yaml-cpp/yaml.h"
#include "DBConnection.h"
#include "Item.h"

extern std::shared_ptr<ClientNode> GetClient(long Uin);
typedef struct PriceInfo
{
	int PriceIdx = 0;
	int PriceValue = 0;
	int BuyNum = 0;
}PRICEINFO;
typedef struct CommodityInfo {
	UINT CommodityID;
	std::vector<PRICEINFO> PriceInfo;
}COMMODITYINFO;

std::unordered_map<long, std::shared_ptr<COMMODITYINFO>> CommodityInfos;//hashmap
extern std::unordered_map<UINT, std::unique_ptr<Item>> Items;//hashmap
extern bool isLock(UINT Itemid);
void LoadCommodityInfo()
{
	CommodityInfos.clear();
	try
	{
		YAML::Node Config = YAML::LoadFile(".\\Config\\Commodity.yml");
		size_t Num = Config.size();
		YAML::Node PriceConfig;
		for (size_t i = 0; i < Num; i++)
		{
			std::shared_ptr<COMMODITYINFO> CommodityInfo = std::make_shared<COMMODITYINFO>();
			CommodityInfo->CommodityID = Config[i]["CommodityID"].as<int>();
			//printf("CommodityID:%d\n", CommodityInfo->CommodityID);
			PriceConfig = Config[i]["Price"];
			size_t PriceNum = PriceConfig.size();
			CommodityInfo->PriceInfo.resize(4);
			for (size_t j = 0; j < PriceNum; j++)
			{
				CommodityInfo->PriceInfo[j].PriceIdx = PriceConfig[j]["PriceIdx"].as<int>();
				CommodityInfo->PriceInfo[j].PriceValue = PriceConfig[j]["Value"].as<int>();
				CommodityInfo->PriceInfo[j].BuyNum = PriceConfig[j]["BuyNum"].as<int>();
				//printf("PriceIdx:%d\n", PriceInfo[j].PriceIdx);
				//printf("PriceValue:%d\n", PriceInfo[j].PriceValue);
				//printf("BuyNum:%d\n", PriceInfo[j].BuyNum);
			}
			CommodityInfos[CommodityInfo->CommodityID] = CommodityInfo;
		}
	}
	catch (const std::exception& e)
	{
		printf("Load CommodityInfo Error!\n");
		return;
	}
	printf("Load CommodityInfo Success!\n");
}

COMMODITYINFO* GetCommodityInfo(long CommodityID)
{
	COMMODITYINFO* CommodityInfo = NULL;
	auto iter = CommodityInfos.find(CommodityID);
	if (iter != CommodityInfos.end())
	{
		CommodityInfo = iter->second.get();
	}
	else
	{
	}
	return CommodityInfo;
}

void ResponseBuyCommodity(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time, const char* Reason, int BuyCount, ShoppingCar* aShoppingCar);

void NotifyClientUpdateMoney(std::shared_ptr<ClientNode> Client)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data(); size_t len;

	Write32(p, Client->Uin); //Uin
	{ //CurrentMoney
		BYTE* pCurrentMoney = p;
		Write16(pCurrentMoney, 0); //len

		Write32(pCurrentMoney, Client->Money); //Money
		Write32(pCurrentMoney, Client->SuperMoney); //SuperMoney
		Write32(pCurrentMoney, 0); //Coupons 消费券
		Write32(pCurrentMoney, 0); //LuckMoney
		Write32(pCurrentMoney, 0); //ConsumeScore
		Write32(pCurrentMoney, 0); //HistoryConsumeScore
		Write32(pCurrentMoney, 0); //SpeedCoin

		len = pCurrentMoney - p;
		Set16(p, (WORD)len);
		p += len;
	}
	{ //IncMoney
		BYTE* pIncMoney = p;
		Write16(pIncMoney, 0); //len

		Write32(pIncMoney, 0); //Money
		Write32(pIncMoney, 0); //SuperMoney
		Write32(pIncMoney, 0); //Coupons
		Write32(pIncMoney, 0); //LuckMoney
		Write32(pIncMoney, 0); //ConsumeScore
		Write32(pIncMoney, 0); //HistoryConsumeScore
		Write32(pIncMoney, 0); //SpeedCoin

		len = pIncMoney - p;
		Set16(p, (WORD)len);
		p += len;
	}
	Write32(p, 0); //Flag

	len = p - buf.data();
	SendToClient(Client, 528, buf.data(), len, Client->ConnID, FE_PLAYER, Client->ConnID, Notify);
}
void RequestBuyCommodity(std::shared_ptr<ClientNode> Client, BYTE* Body, size_t BodyLen)
{
	
	
	int result;

	std::string Reason = "";
	char buffer[16] = "";
	ItemInfo Items1 = { 0 };

	UINT Uin = Read32(Body);
	UINT Time = Read32(Body);

	//NickName[]
	Body += MaxNickName;


	int ReqBuySeq = Read32(Body);
	UINT DstUin = Read32(Body);
	UINT PlayerExp = Read32(Body);

	if (SelectClientExp(Uin) != PlayerExp) {
		return;
	}
	USHORT CommodityType = Read16(Body);
	int PayType = Read32(Body);
	//printf("PayType:%d\n", PayType);
	int BuyCount = Read32(Body);
	if (BuyCount == 0)
	{
		return;
	}
	Client = GetClient(Uin);
	if (!Client)
	{
		return;
	}
	
	SelectClientExp(Client);
	ShoppingCar* aShoppingCar = new ShoppingCar[1];
	if (!aShoppingCar)
	{
		return;
	}
	int PriceValue = 0;
	int BuyNum = 1;
	int RemainSuperMoney = -1;
	for (int i = 0; i < 1; i++)
	{ //ShoppingCar
		BYTE* pShoppingCar = Body;
		size_t len = Read16(pShoppingCar);
		int type = 0; //
		aShoppingCar[i].CommodityID = Read32(pShoppingCar);
		aShoppingCar[i].PriceIdx = Read32(pShoppingCar);
		aShoppingCar[i].DiscountCardID = Read32(pShoppingCar);

		_itoa_s(aShoppingCar[i].CommodityID, buffer, 10);
		Reason = Reason + "CommodityID:" + buffer;
		_itoa_s(aShoppingCar[i].PriceIdx, buffer, 10);
		Reason = Reason + ", PriceIdx:" + buffer;
		_itoa_s(aShoppingCar[i].PriceIdx, buffer, 10);
		Reason = Reason + ", DiscountCardID:" + buffer;
		Reason += "\n";
		COMMODITYINFO* CommnodityInfo = GetCommodityInfo(aShoppingCar[i].CommodityID);
		if (!CommnodityInfo)
		{
			Reason += "/C03该物品不可购买,请联系服主!";
			break;
		}

		if (isLock(aShoppingCar[i].CommodityID)) {
			Reason += "/C03物品被锁住，请联系服主";
			break;
		}

		if ( Client->admin == 1) {
			Reason += "/C03购买成功";
			Items1.ItemNum = PlayerDB_AddItem2(Client, Uin, aShoppingCar[i].CommodityID, 1, 0, false);
			Items1.ItemID = aShoppingCar[i].CommodityID;
			Items1.ObtainTime = 0xFFFF;
			NotifyClientAddItem(Client, 1, { Items1 });

		}
		else {
			if (CommnodityInfo)
			{
				PriceValue = CommnodityInfo->PriceInfo[aShoppingCar[i].PriceIdx].PriceValue;
				BuyNum = CommnodityInfo->PriceInfo[aShoppingCar[i].PriceIdx].BuyNum;
				if (PriceValue < 0)
				{
					RemainSuperMoney = ((Client->SuperMoney) + PriceValue);
				}
				else
				{
					RemainSuperMoney = ((Client->SuperMoney) - PriceValue);
				}
				//printf("RemainSuperMoney:%d\n", RemainSuperMoney);
				if (RemainSuperMoney >= 0)
				{
					Reason += "/C03购买成功";
					Client->SuperMoney = RemainSuperMoney;
					NotifyClientUpdateMoney(Client);

					UpdateClientBase(Client);
					Items1.ItemNum = PlayerDB_AddItem2(Client, Uin, aShoppingCar[i].CommodityID, BuyNum, 0, false);
					Items1.ItemID = aShoppingCar[i].CommodityID;
					Items1.ObtainTime = 0xFFFF;
					NotifyClientAddItem(Client, 1, { Items1 });
				}
				else
				{
					Reason += "/C03购买失败,点券不足,还差【";
					Reason.append(std::to_string(RemainSuperMoney * -1));
					Reason.append("】点卷");
				}
			}
			else {
				auto iter = Items.find(aShoppingCar[i].CommodityID);
				if (iter != Items.end()) {
					std::unique_ptr<Item>& foundItem = iter->second;
					if (foundItem) {
						if (foundItem->buyflag == 1) {
							INT RemainSuperMoney = (Client->SuperMoney) - PriceValue;
							if (RemainSuperMoney >= 0)
							{
								Reason += "/C03购买成功";
								Client->SuperMoney = RemainSuperMoney;
								NotifyClientUpdateMoney(Client);
								UpdateClientBase(Client);
								Items1.ItemNum = PlayerDB_AddItem2(Client, DstUin, aShoppingCar[i].CommodityID, BuyNum, 0, false);
								Items1.ItemID = aShoppingCar[i].CommodityID;
								Items1.ObtainTime = 0xFFFF;
								NotifyClientAddItem(Client, 1, { Items1 });//通知客户端添加道具
							}
						}
						else {
							Reason += "/C03该物品不允许购买，请联系服主";
						}
					}
					else {
						Reason += "/C03未知错误";
					}
					
				}
				else
				{
					Reason += "/C03未配置该物品，请联系服主";
				}
			}
		}

		
		
		//Body += len;
	}

	//short AttachInfoLen = Read16(Body);
	//UINT OtherInfo = Read32(Body);
	//UINT FirstLoginTime = Read32(Body);
	//UINT BuyNum = Read32(Body);
	//UINT GuildID = Read32(Body);
	//int ReduceCouponsID = Read32(Body);


	ResponseBuyCommodity(Client, Uin, Time, Reason.c_str(), BuyCount, aShoppingCar);
	delete[] aShoppingCar;
	//NotifyClientUpdateMoney(Client);
}




void ResponseBuyCommodity(std::shared_ptr<ClientNode> Client, UINT Uin, UINT Time, const char* Reason, int BuyCount, ShoppingCar* aShoppingCar)
{
	std::vector<BYTE> buf(819200);BYTE* p = buf.data();
	//BYTE* p = buf;
	size_t len;


	Write16(p, 0); //ResultID
	Write32(p, Uin); //Uin
	Write32(p, Time); //Time

	memset(p, 0, MaxNickName); //NickName[]
	p += MaxNickName;

	Write32(p, 0); //ReqBuySeq
	Write32(p, 0); //DstUin
	Write32(p, 0); //PlayerExp

	Write16(p, 0); //CommodityType
	Write32(p, 0); //PayType
	Write32(p, BuyCount); //BuyCount
	for (int i = 0; i < BuyCount; i++)
	{
		BYTE* pShoppingCar = p;
		Write16(pShoppingCar, 0); //len

		Write32(pShoppingCar, aShoppingCar[i].CommodityID); //CommodityID
		Write32(pShoppingCar, aShoppingCar[i].PriceIdx); //PriceIdx
		Write32(pShoppingCar, aShoppingCar[i].DiscountCardID); //DiscountCardID

		len = pShoppingCar - p;
		Set16(p, (WORD)len);
		p += len;
	}

	Write16(p, 0); //AttachInfoLen
	Write32(p, 0); //OtherInfo
	Write32(p, 0); //FirstLoginTime

	len = strlen(Reason);
	Write16(p, (WORD)len); //ReasonLen
	memcpy(p, Reason, len);
	p += len;

	Write16(p, 0); //QuerySigLen
	Write8(p, 0); //CommMbItem
	Write8(p, 0); //MbItemNum
	/*
m_stItem[].m_cMbItemId
m_stItem[].m_cUse
m_stItem[].m_wContentLen
	*/


	Write32(p, 0); //ReduceCouponsID

	{ //MoneyInfo
		BYTE* pMoneyInfo = p;
		Write16(pMoneyInfo, 0); //len

		Write32(pMoneyInfo, 0); //money
		Write32(pMoneyInfo, 0); //superMoney
		Write32(pMoneyInfo, 0); //coupons
		Write32(pMoneyInfo, 0); //luckMoney
		Write32(pMoneyInfo, 0); //qbqd

		len = pMoneyInfo - p;
		Set16(p, (WORD)len);
		p += len;
	}

	len = p - buf.data();
	SendToClient(Client, 20000, buf.data(), len, -1, FE_SHOPSVRD, Client->ConnID, Response);

}
