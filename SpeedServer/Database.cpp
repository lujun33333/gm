#include "Database.h"
#include "Item.h"
#include "Message.h"

#include "iostream"
#include "ConnectionPool.h"
#include "ini.h"
#include "DBConnection.h"
#include "MD5.h"


extern BYTE shumiaoyunshu[100];
//extern BYTE chewangkaoyan[100];
extern BYTE baofengtiaozhan[2048];
extern BYTE taskaward[128];
extern BYTE StartTimerChallenge[1024];
extern BYTE StartTimerChallenge2[1024];
extern BYTE BorderOver[187];
extern BYTE RankedOver[402];

extern BYTE jump24238[91];
extern BYTE jump24255[91];
extern BYTE msg900[52];
extern UINT IsCS;
extern int ismd5;
extern std::string GetItemName(UINT Itemid);
extern int NewKart;
extern int PreventSDP[2];
extern std::shared_ptr<ClientNode> GetClient(long Uin);
extern std::mutex ChallengeMapsMutex;
extern std::vector<ChallengeMap> ChallengeAllMaps;
extern void initializeLibrary(std::vector<UINT>& library, const std::string& input);
extern double version;


ConnectionPool player;
ConnectionPool usr;

void InitTaskInfo()
{
	//模拟封包 对照数据结构
	//暴风车队NPC可用自动跑老地图104-138
	//\x01\x6e\x00\x00\x00\x01\xcc\xd8\xc0\xef\xcb\xb9\xcc\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb1\xa9\xb7\xe7\xcc\xec\xca\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x77\x00\x00\x28\x3f\x00\x00\x2c\x79\x00\x00\x2c\x75\x00\x00\x2c\x78\x00\x00\x2c\x76\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2c\x7a\x00\x00\x00\x00\x00\x00\x2c\x70\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x27\x10\x00\x22\x00\x00\x00\x41\x00\x00\x00\x0f\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x00\x00\x00
	memcpy(baofengtiaozhan, "\x00\x00\xea\x60\x00\x00\x00\x49\x00\x00\x00\x79\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x9b\x28\x9c\x8c\x6b\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x01\x6e\x00\x00\x00\x01\xcc\xd8\xc0\xef\xcb\xb9\xcc\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb1\xa9\xb7\xe7\xcc\xec\xca\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x77\x00\x00\x28\x3f\x00\x00\x2c\x79\x00\x00\x2c\x75\x00\x00\x2c\x78\x00\x00\x2c\x76\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2c\x7a\x00\x00\x00\x00\x00\x00\x2c\x70\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x08\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x27\x10\x00\x22\x00\x00\x00\x41\x00\x00\x00\x0f\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x22\x00\x00\x00\x37\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x46\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x1f\x40\x00\x00\x03\xe7\x00\x00\x00\x14\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x14\xff\xff\xff\xec\x00\x12\x00\x00\x00\x02\x00\x00\x13\x88\xff\xff\xff\xec\xff\xff\xff\x9c\x00\x00\x00\x00\x01\x6e\x00\x00\x00\x02\xbf\xad\xc0\xf2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb1\xa9\xb7\xe7\xcc\xec\xca\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x2c\x07\x00\x00\x28\x42\x00\x00\x2c\x09\x00\x00\x2c\x05\x00\x00\x2c\x08\x00\x00\x2c\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2c\x0a\x00\x00\x00\x00\x00\x00\x2c\x6b\x00\x00\x28\x7c\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x2b\xed\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x27\x10\x00\x22\x00\x00\x00\x1e\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x22\x00\x00\x00\x28\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x37\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x3c\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x9c\x00\x22\x00\x00\x00\x50\x00\x00\x00\x19\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x9c\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x1f\x40\x00\x00\x00\xc8\x00\x00\x00\x01\x00\x12\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x12\x00\x00\x00\x01\x00\x00\x03\xe8\x00\x00\x00\x00\xff\xff\xfe\x0c\x00\x12\x00\x00\x00\x02\x00\x00\x00\x00\xff\xff\xfc\x19\xff\xff\xd8\xf1\x00\x00\x00\x00\x01\x6e\x00\x00\x00\x03\xbc\xd3\xce\xc4\xcb\xb9\xc0\xfb\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb1\xa9\xb7\xe7\xcc\xec\xca\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x0d\x00\x00\x27\x8c\x00\x00\x2c\x0f\x00\x00\x2c\x0b\x00\x00\x2c\x0e\x00\x00\x2c\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2c\x10\x00\x00\x00\x00\x00\x00\x2b\x8b\x00\x00\x28\x7e\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x03\x00\x00\x29\x89\x00\x00\x00\x05\x00\x00\x27\x10\x00\x22\x00\x00\x00\x28\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x22\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x01\x2c\x00\x22\x00\x00\x00\x28\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x01\x2c\x00\x00\x00\x00\x00\x22\x00\x00\x00\x50\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x02\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x38\x00\x22\x00\x00\x00\x50\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x38\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x1f\x40\x00\x00\x03\x20\x00\x00\x00\x32\x00\x12\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x12\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\xc8\xff\xff\xff\x38\x00\x12\x00\x00\x00\x02\x00\x00\x00\x00\xff\xff\xff\xce\xff\xfe\x79\x61\x00\x00\x00\x00\x01\x6e\x00\x00\x00\x04\xd2\xc1\xc1\xab\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb1\xa9\xb7\xe7\xcc\xec\xca\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x22\x00\x00\x2c\x13\x00\x00\x27\x77\x00\x00\x27\x36\x00\x00\x2c\x11\x00\x00\x2c\x14\x00\x00\x2c\x12\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2c\x15\x00\x00\x00\x00\x00\x00\x2b\xe6\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x29\xb8\x00\x00\x28\xbc\x00\x00\x00\x05\x00\x00\x27\x10\x00\x22\x00\x00\x00\x28\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x22\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\xc8\x00\x22\x00\x00\x00\x0f\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x22\x00\x00\x00\x19\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x02\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x38\x00\x22\x00\x00\x00\x32\x00\x00\x00\x14\x00\x00\x00\x0a\x00\x00\x00\x03\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x38\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x27\x0f\x00\x00\x03\xe7\x00\x12\x00\x00\x00\x01\x00\x00\x0f\xa0\x00\x00\x00\x64\xff\xff\xfe\x0c\x00\x12\x00\x00\x00\x02\x00\x00\x00\x00\xff\xff\xfc\x19\xff\xfe\x79\x61\x00\x00\x00\x00\x01\x6e\x00\x00\x00\x05\xb6\xf7\xd7\xf4\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xb1\xa9\xb7\xe7\xcc\xec\xca\xb9\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x46\x00\x00\x27\x26\x00\x00\x2c\x9d\x00\x00\x28\x53\x00\x00\x00\x00\x00\x00\x2c\x9b\x00\x00\x2c\x9e\x00\x00\x2c\x9c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2c\x9f\x00\x00\x00\x00\x00\x00\x2b\xe5\x00\x00\x27\x2d\x00\x00\x27\x75\x00\x00\x27\x73\x00\x00\x28\x75\x00\x00\x27\x97\x00\x00\x00\x0a\x00\x00\x27\x10\x00\x22\x00\x00\x00\x28\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x22\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x01\x86\x9f\x00\x00\x00\x32\x00\x22\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x32\x00\x00\x00\x00\x00\x22\x00\x00\x00\x0c\x00\x00\x00\x02\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\x38\x00\x22\x00\x00\x00\x12\x00\x00\x00\x05\x00\x00\x00\x0a\x00\x00\x00\x05\x00\x00\x00\x05\x00\x00\x00\x01\xff\xff\xff\x38\xff\xfe\x79\x61\x04\x00\x12\x00\x00\x00\x08\x00\x00\x00\x00\x00\x0f\x42\x36\x00\x00\x27\x0f\x00\x12\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x27\x0f\x00\x00\x03\xe7\x00\x12\x00\x00\x00\x01\x00\x00\x03\xe8\x00\x00\x00\x64\xff\xff\xff\x9c\x00\x12\x00\x00\x00\x02\x00\x00\x01\xf4\xff\xff\xff\xb0\xff\xff\xd8\xf1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1928);
	memcpy(shumiaoyunshu, "\x00\x00\xea\x60\x00\x00\x00\x6e\x00\x00\x00\xc7\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xdf\x20\xd2\xed\xf9\x00\x02\x00\x02\xbf\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 98);
	//memcpy(chewangkaoyan, "\x00\x00\xea\x60\x00\x00\x00\x48\x00\x00\x00\x71\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x76\x7a\x7b\x38\xf9\x00\x03\x00\x00\xd6\xd8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",98);
	memcpy(taskaward, "\x00\x00\x27\x11\x00\x13\x00\x00\x00\x46\x01\x00\x00\x00\x00\x00\x00\x00\x0d\x00\x00\x00\x00\x00\x00\x73\x33\x00\x5b\x00\x00\x01\x90\x00\x00\xcd\x69\x00\x00\x00\x00\x00\x0F\xFF\xFF\x00\x00\x64\x00\xFF\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\xfc\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 127);
	//	memcpy(StartTimerChallenge, "\x00\x00\x1f\x40\x00\x00\x01\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\xe0\x68\x45\xe0\x16\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\xff\x00\x01\x00\x0b\x00\x00\x27\x11\x00\x00\x68\x2e\x00\x00\x00\x00\x00\x32\x00\x00\x00\x01\x64\x69\xa9\xa7\x04\x00\x89\x03\x25\x01\x01\x64\x69\xa9\xa7\x22\x22\x5a\xbe\x01\x01\x91\x8f\x12\xa1\x16\x72\x36\xcb\xae\x15\x41\x6b\x67\x98\x20\x42\xe4\x91\xe9\x21\xe8\xe8\x62\x58\x85\x00\x00\x00\x00\x04\xff\xff\x00\x00\x00\xc8\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7c\xbf\xf1\x01\x00\x1d\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x05\xbe\xaa\xaa\xe3\xcb\x3a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x0b\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x37\x00\x00\x02\x00\x19\x00\x00\x05\x00\x00\x02\x58\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x0b\xb8\x00\x19\x00\x00\x05\x00\x00\x01\x2c\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x03\xe8\x00\x00\x03\xe8\x00\x3b\x00\x01\x02\x00\x19\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x32\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x1d\x00\x01\x06\x00\x00\x00\x00\x00\x00\x9c\x40\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x27\x10\x00\x00\x00\x00\x00\x0a\x00\x00\x01\x00\x05\x00\x02\x00\x00\x00\x00\x80\x00\x00\x00\x00\x00\x00", 589);
	memcpy(StartTimerChallenge, "\x00\x00\x1f\x40\x00\x00\x01\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\xe0\x68\x45\xe0\x16\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\xff\xff\xff\xff\x00\x01\x00\x0b\x00\x00\x27\x11\x00\x00\x68\x2e\x00\x00\x00\x00\x00\x32\x00\x00\x00\x01\x64\x69\xa9\xa7\x04\x00\x89\x03\x25\x01\x01\x64\x69\xa9\xa7\x22\x22\x5a\xbe\x01\x01\x91\x8f\x12\xa1\x16\x72\x36\xcb\xae\x15\x41\x6b\x67\x98\x20\x42\xe4\x91\xe9\x21\xe8\xe8\x62\x58\x85\x00\x00\x00\x00\x04\xff\xff\x00\x00\x00\xc8\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7c\xbf\xf1\x01\x00\x1d\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x05\xbe\xaa\xaa\xe3\xcb\x3a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x0b\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x37\x00\x00\x02\x00\x19\x00\x00\x05\x00\x00\x02\x58\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x0b\xb8\x00\x19\x00\x00\x05\x00\x00\x01\x2c\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x03\xe8\x00\x00\x03\xe8\x00\x3b\x00\x01\x02\x00\x19\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x32\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x1d\x00\x01\x06\x00\x00\x00\x00\x00\x00\x9c\x40\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x27\x10\x00\x00\x00\x00\x00\x0a\x00\x00\x01\x00\x05\x00\x02\x00\x00\x00\x00\x80\x00\x00\x00\x00\x00\x00", 589);

	//memcpy(jump24238, "\x00\x00\x27\x11\x00\x00\x00\x03\x00\x00\x00\x31\x00\x00\x00\x03\x00\x00\x27\x10\x00\x00\x79\x00\x00\x00\x00\x0a\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x31\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x10\x02\x4a\x00\x00\x00\x01\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x61\x6c", 91);
	memcpy(StartTimerChallenge2, "\x00\x00\x1f\x40\x00\x00\x01\xb0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x39\x52\xf3\x7d\xac\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\xa1\x08\x00\x01\x00\x0b\x00\x00\x27\x11\x00\x01\xf2\x80\x00\x00\x00\x01\x0a\x32\x00\x00\x00\x00\x64\x72\x23\xfd\x65\x02\x30\x02\x2e\x01\x01\x64\x72\x23\xfd\x12\x8c\x17\x3e\x01\x01\x70\x4e\x24\xae\x45\xd2\xb2\xfa\x36\x7b\x4b\xe7\xe1\x03\x1e\xa8\x8e\xa1\xa6\x39\xfe\x08\xb3\x92\xe3\x00\x00\x00\x00\x04\xff\xff\x00\x00\x00\xc8\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x14\xaf\x5d\x01\x00\x35\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x01\x03\x00\x08\x00\xcc\x00\xcc\x00\x01\x00\x08\x00\xcf\x00\xcf\x00\x01\x00\x08\x00\xd2\x00\xd2\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x0a\xaa\x1d\xb2\x76\x6c\x86\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2e\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\xcb\x00\x20\x00\x00\x00\x01\x00\x16\x00\x00\x27\x11\x00\x00\x00\x03\x00\x00\x00\x0d\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x11\x00\x01\x00\x00\x00\x01\x03\x00\x08\x00\xcc\x00\x00\x00\x00\x00\x08\x00\xcf\x00\x00\x00\x00\x00\x08\x00\xd2\x00\x00\x00\x00\x00\x02\x00\x84\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x00\x42\xc1\xf7\xbb\xf0\xc1\xfa\xbe\xa7\xb8\xb3\xd3\xe8\xc8\xfc\xb3\xb5\xb7\xc9\xd0\xd0\xb5\xc4\xc4\xdc\xc1\xa6\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x00\x01\x63\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x37\x00\x00\x02\x00\x19\x00\x00\x05\x00\x00\x02\x58\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x0b\xb8\x00\x19\x00\x00\x05\x00\x00\x01\x2c\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x03\xe8\x00\x00\x03\xe8\x00\x3b\x00\x01\x02\x00\x19\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x32\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x1d\x00\x01\x06\x00\x00\x00\x00\x00\x00\x9c\x40\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x27\x10\x00\x00\x00\x00\x00\x0a\x00\x00\x01\x00\x05\x00\x02\x00\x00\x00\x07\xe0\x00\x00\x00\x00\x00\x00", 816);

	//memcpy(StartTimerChallenge2, "\x00\x00\x1f\x40\x00\x00\x01\xaf\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x39\x52\xf3\x7d\xac\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\xa1\x08\x00\x01\x00\x0b\x00\x00\x27\x11\x00\x00\x68\x2e\x00\x00\x00\x01\x0a\x32\x00\x00\x00\x00\x64\x72\x23\xfd\x65\x02\x30\x02\x2e\x01\x01\x64\x72\x23\xfd\x12\x8c\x17\x3e\x01\x01\x70\x4e\x24\xae\x45\xd2\xb2\xfa\x36\x7b\x4b\xe7\xe1\x03\x1e\xa8\x8e\xa1\xa6\x39\xfe\x08\xb3\x92\xe3\x00\x00\x00\x00\x04\xff\xff\x00\x00\x00\xc8\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x14\xaf\x5d\x01\x00\x35\x00\x00\x27\x11\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x01\x03\x00\x08\x00\xcc\x00\xcc\x00\x01\x00\x08\x00\xcf\x00\xcf\x00\x01\x00\x08\x00\xd2\x00\xd2\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x0a\xaa\x1d\xb2\x76\x6c\x86\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x2e\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\xcb\x00\x20\x00\x00\x00\x01\x00\x16\x00\x00\x27\x11\x00\x00\x00\x03\x00\x00\x00\x0d\x00\x00\x00\x00\x00\x00\x00\x10\x00\x00\x00\x03\x00\x00\x27\x11\x00\x01\x00\x00\x00\x01\x03\x00\x08\x00\xcc\x00\x00\x00\x00\x00\x08\x00\xcf\x00\x00\x00\x00\x00\x08\x00\xd2\x00\x00\x00\x00\x00\x02\x00\x84\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\xc1\xd2\xd1\xe6\xb7\xc9\xd0\xd0\x00\x42\xc1\xf7\xbb\xf0\xc1\xfa\xbe\xa7\xb8\xb3\xd3\xe8\xc8\xfc\xb3\xb5\xb7\xc9\xd0\xd0\xb5\xc4\xc4\xdc\xc1\xa6\xa3\xac\xb3\xd6\xd0\xf8\xca\xb1\xbc\xe4\x35\xc3\xeb\xa3\xac\xc0\xe4\xc8\xb4\xca\xb1\xbc\xe4\x31\x35\xc3\xeb\xa3\xac\xca\xb9\xd3\xc3\xb4\xce\xca\xfd\x31\xb4\xce\x01\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x00\x01\x63\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x37\x00\x00\x02\x00\x19\x00\x00\x05\x00\x00\x02\x58\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x0b\xb8\x00\x19\x00\x00\x05\x00\x00\x01\x2c\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x03\xe8\x00\x00\x03\xe8\x00\x3b\x00\x01\x02\x00\x19\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x32\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x1d\x00\x01\x06\x00\x00\x00\x00\x00\x00\x9c\x40\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x27\x10\x00\x00\x00\x00\x00\x0a\x00\x00\x01\x00\x05\x00\x02\x00\x00\x00\x07\xe0\x00\x00\x00\x00\x00\x00", 816);
	//4级烈焰飞行 
	//技能时长00 00 1b 58 或者00 00 13 88 后面是冷却00 00 27 10 技能次数03

	memcpy(jump24255, "\x00\x00\x00\x03\x00\x00\x00\x77\x00\xFF\xFF\xFF\x00\x01\x0f\xe3\x00\x01\x0f\xe3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x61\x6c", 91);

	memcpy(BorderOver, "\x00\xb1\x00\x00\x27\x11\x00\x02\xa2\x93\x00\x00\x00\x01\x00\x00\x00\x00\x00\x03\x02\x64\x00\x00\x01\xf4\x00\x00\x26\x56\x00\x00\x00\x64\x00\x00\x3d\x36\x00\x00\x00\x64\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x06\x00\x00\x00\x0f\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x30\x17\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x01\x01\x0f\xa0\x00\x00\x06\x00\x00\x01\x01\x00\x00\x00\x00\x00\x00\x00\x42\x68\x00\x04\x00\x00\x00\x00", 187);
	memcpy(RankedOver, "\x01\x01\x79\x00\x00\x27\x11\x00\x01\xac\x54\x00\x00\x00\x00\x00\x00\x01\x90\x00\x01\x01\x70\x00\x00\x00\x9c\x00\x03\x14\x17\x02\x00\x00\x00\x05\x00\x00\x00\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x90\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x26\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\xbb\x00\x16\x00\x00\x00\x16\x00\x00\x00\x04\x00\x00\x00\x13\x00\x00\x00\x28\x00\x00\x00\xa9\x00\x16\x00\x00\x00\x16\x00\x00\x00\x04\x00\x00\x00\x1d\x00\x00\x00\x28\x00\x00\x00\xb3\x00\x24\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x68\x00\x00\x00\x04\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x17\x00\x00\x00\x04\x00\x00\x00\x06\x00\x00\x00\x26\x00\x00\x01\x2c\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x30\x17\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x01\x01\x00\x00\x00\x06\x00\x00\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x59\xd8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 402);
	memcpy(msg900, (BYTE*)"\x00\x00\x00\x00\x27\x11\x29\xc4\xe3\xb5\xc4\xd3\xc3\xbb\xa7\xd2\xd1\xbe\xad\xd4\xda\xc1\xed\xd2\xbb\xb8\xf6\xc9\xe8\xb1\xb8\xb5\xc7\xc2\xbc\x2c\xc4\xfa\xb1\xbb\xc6\xc8\xcf\xc2\xcf\xdf\xa3\xa1\x00\x00\x00\x00", 52);

}


void InitDB()
{
	try
	{
		ini::iniReader config;
		int poolsize;
		std::string host;
		std::string user;
		std::string password;
		std::string database;
		bool ret = config.ReadConfig("mysql.ini");
		if (ret == false)
		{
			printf("读取mysql配置文件失败！\n");
			return;
		}

		poolsize = config.ReadInt("database_player", "poolsize", 1);
		host = config.ReadString("database_player", "host", "");
		user = config.ReadString("database_player", "user", "");
		password = config.ReadString("database_player", "password", "");
		database = config.ReadString("database_player", "database", "");
		player.initialize(poolsize, host, user, password, database);

		poolsize = config.ReadInt("database_user", "poolsize", 1);
		host = config.ReadString("database_user", "host", "");
		user = config.ReadString("database_user", "user", "");
		password = config.ReadString("database_user", "password", "");
		database = config.ReadString("database_user", "database", "");
		usr.initialize(poolsize, host, user, password, database);

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		return;
	}
}

void CloseDB()
{
	player.closePool();
	usr.closePool();
}



BOOL UserDB_RegisterNew(const char* Username, const char* Password, const char* yqm)
{

	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return false;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO User (Name, Password, InviteCode) VALUES(?, ?, ?);"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, Username);
		prep_stmt->setString(2, Password);
		prep_stmt->setString(3, yqm);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_RegisterNew Error" << std::endl;
		//throw;关闭
	}
}

BOOL UserDB_Register(const char* Username, const char* Password, const char* cdk)
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn)  return false;
	try
	{
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Name FROM User WHERE Name = ?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, Username);

			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			if (res->rowsCount() > 0) {
				res.reset();
				return false;
			}
			res.reset();
		}

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO User (Name, Password,regcdk) VALUES(?, ?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, Username);
		prep_stmt->setString(2, Password);
		prep_stmt->setString(3, cdk);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_Register Error" << std::endl;
		//throw;关闭
	}
}


void UserDB_UpdateConnID(DWORD Uin)
{//Lupin
	if (Uin < 10000) return;
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET ConnID=null WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin - 10000);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_UpdateConnID1 Error\n");

	}
}


void UserDB_UpdateConnID()
{//Lupin
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET ConnID=null;"));
		prep_stmt->clearParameters();
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_UpdateConnID2 Error\n");

	}
}


UINT UserDB_SelectConnID(DWORD Uin)
{//Lupin
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	UINT ConnID = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ConnID FROM User WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin - 10000);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			ConnID = res->getUInt("ConnID");
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_SelectConnID1 Error\n");
	}
	return ConnID;
}

UINT UserDB_SelectUsr(DWORD ConnID)
{//Lupin
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	UINT ret = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM User WHERE ConnID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, ConnID);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			ret = 1;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_SelectUsr Error\n");
	}
	return ret;
}



void UserDB_ConnID(DWORD Uin, DWORD ConnID)
{//Lupin
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET ConnID=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, ConnID);
		prep_stmt->setUInt(2, Uin - 10000);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_ConnID Error\n");

	}
}

void UserDB_SelectConnID(std::vector<UINT>& UinConnids)
{//Lupin
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ConnID FROM User WHERE ConnID > 0;"));
		prep_stmt->clearParameters();

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			UinConnids.push_back(res->getUInt("ConnID"));
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_SelectConnID2 Error\n");

	}
}

std::string UserDB_GetLoginKey(UINT Uin)
{
	std::string key = "";
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::string sql = "SELECT LoginKey FROM User WHERE Uin = ?;";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin - 10000);

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		if (res->rowsCount() > 0) {
			res->next();
			key = UTF2GB(res->getString("LoginKey").c_str());
		}
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_GetLoginKey Error\n");
	}
	return key;
}

void UserDB_UpdateLoginKey(UINT Uin, const char* key)
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET LoginKey=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, key);
		prep_stmt->setUInt(2, Uin - 10000);
		prep_stmt->executeUpdate();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_UpdateLoginKey Error\n");

	}
}

BOOL UserDB_CmpIP(const char* IP)
{
	BOOL ret = FALSE;
	UINT connid = 0;
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT connid  FROM User  WHERE ip=? and isban is null;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, IP);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			res->next();
			connid = res->getUInt("connid");
			if (connid > 0) {
				ret = TRUE;
			}
		}
		res.reset();
		return ret;

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_CmpIP Error\n");
	}
}

std::string UserDB_GetIP(UINT Uin)
{
	std::string Ip = "";
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ip  FROM User  WHERE Uin =?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin - 10000);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			res->next();
			Ip = UTF2GB(res->getString("ip").c_str());
		}
		res.reset();
		return Ip;

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_GetIP Error\n");
	}

}

BOOL UserDB_SelectIP(const char* IP)
{
	BOOL ret = FALSE;
	UINT connid = 0;
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1  FROM User  WHERE ip=? and isban is null;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, IP);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			ret = TRUE;
		}
		res.reset();
		return ret;

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_SelectIP Error\n");
	}

}

BOOL UserDB_SelectIPNoTime(const char* IP)
{
	BOOL ret = FALSE;
	UINT connid = 0;
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection();
	try
	{
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1  FROM User  WHERE ip=? and isban is null and STR_TO_DATE(lastlogintime, '%Y-%m-%d %H:%i:%s') >= NOW() - INTERVAL 8 HOUR;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, IP);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			ret = TRUE;
		}
		res.reset();
		return ret;

	}
	catch (const std::exception& e)
	{
		printf(e.what());
		printf("UserDB_SelectIPNoTime Error\n");
	}

}


void UserDB_GetAllUin(std::vector<UINT>& Uins)
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		Uins.clear();
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Uin FROM User"));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			Uins.push_back(res->getUInt("Uin"));
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_GetAllUin Error" << std::endl;
		//throw;关闭
	}
}

void UserDB_UpdateMD5(const char* Username)
{

	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET NameEncryp=? WHERE Name=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, md5(Username));
		prep_stmt->setString(2, Username);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_UpdateMD51 Error" << std::endl;
		//throw;关闭
	}
}

void UserDB_UpdateMD5()
{

	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Name  FROM User WHere NameEncryp Is null;"));
		prep_stmt->clearParameters();

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			UserDB_UpdateMD5(res->getString("Name").c_str());
		}

		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_UpdateMD52 Error" << std::endl;
		//throw;关闭
	}
}


DWORD UserDB_GetUin(const char* ip, const char* Username, const char* Pwd)
{

	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		UINT uin = 0;
		int IsBan = 0;
		std::string pwd;
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置
		std::string sql;
		/*if (PreventSDP[0] > 0) {
			sql = "SELECT Uin,Password,IsBan FROM User WHERE BINARY  NameEncryp = ?;";
		}
		else {
			sql = "SELECT Uin,Password,IsBan FROM User WHERE BINARY  Name = ?;";
		}*/

		sql = "SELECT Uin,Password,IsBan FROM User WHERE BINARY  Name = ?;";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, Username);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			uin = res->getUInt("Uin");
			pwd = res->getString("Password");
			IsBan = res->getInt("IsBan");
		}
		res.reset();

		if (pwd.size() != 0) {
			uin += 10000; //必须大于10000,否则会出问题
			if (ismd5) {
				pwd = md5(pwd);
			}

			if (std::string(Pwd) != pwd)
			{
				uin = 0;
			}
			if (IsBan == 1)
			{
				uin = 1;
			}
		}
		else {
			uin = 0;
		}

		if (uin > 10000)
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET IP = ?,LastLoginTime = ?  WHERE Uin = ?;"));
			prep_stmt->clearParameters();
			prep_stmt->setString(1, ip);

			// 获取当前时间，并转换为指定格式的字符串
			auto now = std::chrono::system_clock::now();
			auto now_time_t = std::chrono::system_clock::to_time_t(now);
			struct tm timeinfo;
			char buffer[20]; // 用于存储时间字符串
			localtime_s(&timeinfo,&now_time_t);
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
			prep_stmt->setString(2, buffer);


			prep_stmt->setUInt(3, uin - 10000);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		return uin;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_GetUin1 Error" << std::endl;
		//throw;关闭
	}
}

BOOL UserDB_GetUin(UINT Uin)
{
	bool ret = false;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{

		std::string sql = "SELECT Uin FROM Baseinfo WHERE Uin = ?;";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			ret = true;
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_GetUin2 Error" << std::endl;
		//throw;关闭
	}
}


BOOL UserDB_CmpPassword(DWORD Uin, const char* Password)
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn)  return false;
	try
	{
		std::string pwd;
		BOOL ret = FALSE;
		UINT isban = 0;
		// 创建准备好的语句对象，使用占位符 ? 指定参数位置

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Password,isban  FROM User  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin - 10000);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			while (res->next()) {
				pwd = res->getString("Password");
				isban = res->getInt("isban");
				break;
			}
			if (ismd5) {
				pwd = md5(pwd);
			}
			if (pwd == std::string(Password) && isban != 1)
			{
				ret = TRUE;
			}
		}
		// 释放资源
		res.reset();
		return ret;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_CmpPassword Error" << std::endl;
		//throw;关闭
	}
}

bool isFiveOrSixDigit(DWORD ItemID) {
	// Convert ItemID to string to count digits
	std::string strID = std::to_string(ItemID);

	// Check if the length of the string is either 5 or 6
	return (strID.length() == 5 || strID.length() == 6);
}

int PlayerDB_AddItem1(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status, sql::Connection* conn)
{
	if (!isFiveOrSixDigit(ItemID) || Uin < 10000) return ItemNum;

	int count = 0;
	BOOL isExist = false;
	int ItemType = 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM Item  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			count = res->getInt("ItemNum");
			isExist = true;
		}
		res.reset();



		if (isExist) {
			if (GetItemType(ItemID) == EAIT_PROP || GetItemType(ItemID) == EAIT_STONE) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum = ? WHERE Uin = ? AND ItemID = ? ;"));
				prep_stmt->clearParameters();
				prep_stmt->setInt(1, ItemNum + count);
				prep_stmt->setUInt(2, Uin);
				prep_stmt->setInt(3, ItemID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else {
				if (AvailPeriod == -1) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = -1,ItemNum = 1 WHERE Uin = ? AND ItemID = ? ;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (AvailPeriod > 0) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = ?,ObtainTime = UNIX_TIMESTAMP(),ItemNum = 1 WHERE Uin = ? AND ItemID = ?;"));
					prep_stmt->clearParameters();
					prep_stmt->setInt(1, AvailPeriod);
					prep_stmt->setUInt(2, Uin);
					prep_stmt->setInt(3, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				count = 0;
			}

		}
		else {
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				if (NewKart != 0) {
					goto insert;
				}
				ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM kartstonenew  WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				while (res->next()) {
					ID = 1;
				}
				res.reset();
				prep_stmt.reset();


				if (ID == 0)//不存在重复的宝石ID号 才加宝石和改装信息
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO kartstonenew (Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

					NotifyUpdateKartSkillStoneInfoNew(Client, ItemID);
				}

				{
					ID = 0;
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM KartRefit WHERE Uin=? AND KartID=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					// 处理结果
					while (res->next()) {
						ID = 1;
					}
					prep_stmt.reset();
					res.reset();
				}
				//改装信息
				//printf("ID：%d\n", ID);
				if (ID == 0)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit(Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
			}
			else if (GetItemType(ItemID) == EAIT_PET)
			{ // 宠物信息
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Pet(Uin,PetId,Name,Experience,PL) VALUES (?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				prep_stmt->setString(3, G2U(GetItemName(ItemID).c_str()));
				prep_stmt->setInt(4, 238966);
				prep_stmt->setInt(5, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
		insert:
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->setInt(3, ItemNum);
			prep_stmt->setInt(4, AvailPeriod);
			prep_stmt->setInt(5, Status);
			prep_stmt->setInt(6, ItemType);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddItem1 Error" << std::endl;
		//throw;关闭
	}

	return ItemNum + count;
}

int PlayerDB_AddItem2(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status)
{
	if (!isFiveOrSixDigit(ItemID) || Uin < 10000) return ItemNum;

	int count = 0;
	BOOL isExist = false;
	int ItemType = 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return ItemNum;
	conn->setAutoCommit(false);
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM Item  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			count = res->getInt("ItemNum");
			isExist = true;
		}
		res.reset();



		if (isExist) {
			if (GetItemType(ItemID) == EAIT_PROP || GetItemType(ItemID) == EAIT_STONE || GetItemType(ItemID) == EAIT_CLOTHES) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum = ? WHERE Uin = ? AND ItemID = ? ;"));
				prep_stmt->clearParameters();
				prep_stmt->setInt(1, ItemNum + count);
				prep_stmt->setUInt(2, Uin);
				prep_stmt->setInt(3, ItemID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else {
				if (AvailPeriod == -1) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = -1,ItemNum = 1 WHERE Uin = ? AND ItemID = ? ;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (AvailPeriod > 0) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum = 1,AvailPeriod = CASE WHEN UNIX_TIMESTAMP() - ObtainTime < AvailPeriod * 3600 THEN AvailPeriod + ? ELSE ? END,ObtainTime = UNIX_TIMESTAMP() WHERE Uin = ? AND ItemID = ? ;"));
					prep_stmt->clearParameters();
					prep_stmt->setInt(1, AvailPeriod);
					prep_stmt->setInt(2, AvailPeriod);
					prep_stmt->setUInt(3, Uin);
					prep_stmt->setInt(4, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				count = 0;
			}

		}
		else {
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM kartstonenew  WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				while (res->next()) {
					ID = 1;
				}
				res.reset();
				prep_stmt.reset();


				if (ID == 0)//不存在重复的宝石ID号 才加宝石和改装信息
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO kartstonenew (Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

					NotifyUpdateKartSkillStoneInfoNew(Client, ItemID);
				}

				{
					ID = 0;
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM KartRefit WHERE Uin=? AND KartID=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					// 处理结果
					while (res->next()) {
						ID = 1;
					}
					prep_stmt.reset();
					res.reset();
				}
				//改装信息
				//printf("ID：%d\n", ID);
				if (ID == 0)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit(Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
			}
			else if (GetItemType(ItemID) == EAIT_PET)
			{ // 宠物信息
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Pet(Uin,PetId,Name,Experience,PL) VALUES (?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				prep_stmt->setString(3, G2U(GetItemName(ItemID).c_str()));
				prep_stmt->setInt(4, 238966);
				prep_stmt->setInt(5, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->setInt(3, ItemNum);
			prep_stmt->setInt(4, AvailPeriod);
			prep_stmt->setInt(5, Status);
			prep_stmt->setInt(6, ItemType);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		conn->commit();
		conn->setAutoCommit(true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddItem2 Error" << std::endl;
		//throw;关闭
	}

	return ItemNum + count;
}

int PlayerDB_AddItem3(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status, bool Skip)
{
	if (!isFiveOrSixDigit(ItemID) || Uin < 10000) return ItemNum;

	int count = 0;
	BOOL isExist = false;
	int ItemType = 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return ItemNum;
	conn->setAutoCommit(false);
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM Item  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			count = res->getInt("ItemNum");
			isExist = true;
		}
		res.reset();



		if (isExist) {
			if (GetItemType(ItemID) == EAIT_PROP || GetItemType(ItemID) == EAIT_STONE) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum = ? WHERE Uin = ? AND ItemID = ? ;"));
				prep_stmt->clearParameters();
				prep_stmt->setInt(1, ItemNum + count);
				prep_stmt->setUInt(2, Uin);
				prep_stmt->setInt(3, ItemID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else {
				if (AvailPeriod == -1) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = -1,ItemNum = 1 WHERE Uin = ? AND ItemID = ? ;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (AvailPeriod > 0) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = ?,ObtainTime = UNIX_TIMESTAMP(),ItemNum = 1 WHERE Uin = ? AND ItemID = ?;"));
					prep_stmt->clearParameters();
					prep_stmt->setInt(1, AvailPeriod);
					prep_stmt->setUInt(2, Uin);
					prep_stmt->setInt(3, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				count = 0;
			}

		}
		else {
			if (GetItemType(ItemID) == EAIT_CAR && !Skip)
			{
				ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM kartstonenew  WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				while (res->next()) {
					ID = 1;
				}
				res.reset();
				prep_stmt.reset();


				if (ID == 0)//不存在重复的宝石ID号 才加宝石和改装信息
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO kartstonenew (Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
					NotifyUpdateKartSkillStoneInfoNew(Client, ItemID);
				}

				{
					ID = 0;
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM KartRefit WHERE Uin=? AND KartID=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					// 处理结果
					while (res->next()) {
						ID = 1;
					}
					prep_stmt.reset();
					res.reset();
				}
				//改装信息
				//printf("ID：%d\n", ID);
				if (ID == 0)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit(Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
			}
			else if (GetItemType(ItemID) == EAIT_PET)
			{ // 宠物信息
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Pet(Uin,PetId,Name,Experience,PL) VALUES (?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				prep_stmt->setString(3, G2U(GetItemName(ItemID).c_str()));
				prep_stmt->setInt(4, 238966);
				prep_stmt->setInt(5, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->setInt(3, ItemNum);
			prep_stmt->setInt(4, AvailPeriod);
			prep_stmt->setInt(5, Status);
			prep_stmt->setInt(6, ItemType);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		conn->commit();
		conn->setAutoCommit(true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddItem3 Error" << std::endl;
		//throw;关闭
	}

	return ItemNum + count;
}


int PlayerDB_AddItem4(DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status)
{
	if (!isFiveOrSixDigit(ItemID) || Uin < 10000) return ItemNum;

	int count = 0;
	BOOL isExist = false;
	int ItemType = 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return ItemNum;
	conn->setAutoCommit(false);
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM Item  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			count = res->getInt("ItemNum");
			isExist = true;
		}
		res.reset();


		if (isExist) {
			if (GetItemType(ItemID) == EAIT_PROP || GetItemType(ItemID) == EAIT_STONE) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum = ? WHERE Uin = ? AND ItemID = ? ;"));
				prep_stmt->clearParameters();
				prep_stmt->setInt(1, ItemNum + count);
				prep_stmt->setUInt(2, Uin);
				prep_stmt->setInt(3, ItemID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else {
				if (AvailPeriod == -1) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = -1,ItemNum = 1 WHERE Uin = ? AND ItemID = ? ;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (AvailPeriod > 0) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = ?,ObtainTime = UNIX_TIMESTAMP(),ItemNum = 1 WHERE Uin = ? AND ItemID = ?;"));
					prep_stmt->clearParameters();
					prep_stmt->setInt(1, AvailPeriod);
					prep_stmt->setUInt(2, Uin);
					prep_stmt->setInt(3, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				count = 0;
			}

		}
		else {
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM kartstonenew  WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				while (res->next()) {
					ID = 1;
				}
				res.reset();
				prep_stmt.reset();


				if (ID == 0)//不存在重复的宝石ID号 才加宝石和改装信息
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO kartstonenew (Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}

				{
					ID = 0;
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM KartRefit WHERE Uin=? AND KartID=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					// 处理结果
					while (res->next()) {
						ID = 1;
					}
					prep_stmt.reset();
					res.reset();
				}
				//改装信息
				//printf("ID：%d\n", ID);
				if (ID == 0)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit(Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
			}
			else if (GetItemType(ItemID) == EAIT_PET)
			{ // 宠物信息
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Pet(Uin,PetId,Name,Experience,PL) VALUES (?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				prep_stmt->setString(3, G2U(GetItemName(ItemID).c_str()));
				prep_stmt->setInt(4, 238966);
				prep_stmt->setInt(5, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->setInt(3, ItemNum);
			prep_stmt->setInt(4, AvailPeriod);
			prep_stmt->setInt(5, Status);
			prep_stmt->setInt(6, ItemType);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		conn->commit();
		conn->setAutoCommit(true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddItem4 Error" << std::endl;
		//throw;关闭
	}

	return ItemNum + count;

}

int PlayerDB_AddItem5(DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, bool Status, sql::Connection* conn)
{
	if (!isFiveOrSixDigit(ItemID) || Uin < 10000) return ItemNum;

	int count = 0;
	BOOL isExist = false;
	int ItemType = 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM Item  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			count = res->getInt("ItemNum");
			isExist = true;
		}
		res.reset();



		if (isExist) {
			if (GetItemType(ItemID) == EAIT_PROP || GetItemType(ItemID) == EAIT_STONE) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum = ? WHERE Uin = ? AND ItemID = ? ;"));
				prep_stmt->clearParameters();
				prep_stmt->setInt(1, ItemNum + count);
				prep_stmt->setUInt(2, Uin);
				prep_stmt->setInt(3, ItemID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
			else {
				if (AvailPeriod == -1) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = -1,ItemNum = 1 WHERE Uin = ? AND ItemID = ? ;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				else if (AvailPeriod > 0) {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET AvailPeriod = ?,ObtainTime = UNIX_TIMESTAMP(),ItemNum = 1 WHERE Uin = ? AND ItemID = ?;"));
					prep_stmt->clearParameters();
					prep_stmt->setInt(1, AvailPeriod);
					prep_stmt->setUInt(2, Uin);
					prep_stmt->setInt(3, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
				count = 0;
			}

		}
		else {
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM kartstonenew  WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				while (res->next()) {
					ID = 1;
				}
				res.reset();
				prep_stmt.reset();


				if (ID == 0)//不存在重复的宝石ID号 才加宝石和改装信息
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO kartstonenew (Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
					//NotifyUpdateKartSkillStoneInfoNew(Client, ItemID);
				}

				{
					ID = 0;
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM KartRefit WHERE Uin=? AND KartID=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					// 执行查询
					std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
					// 处理结果
					while (res->next()) {
						ID = 1;
					}
					prep_stmt.reset();
					res.reset();
				}
				//改装信息
				//printf("ID：%d\n", ID);
				if (ID == 0)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit(Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
			}
			else if (GetItemType(ItemID) == EAIT_PET)
			{ // 宠物信息
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Pet(Uin,PetId,Name,Experience,PL) VALUES (?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				prep_stmt->setString(3, G2U(GetItemName(ItemID).c_str()));
				prep_stmt->setInt(4, 238966);
				prep_stmt->setInt(5, 0);
				prep_stmt->executeUpdate();
				prep_stmt.reset();

			}
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Item (Uin,ItemID,ItemNum,AvailPeriod,Status,ItemType) VALUES (?,?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->setInt(3, ItemNum);
			prep_stmt->setInt(4, AvailPeriod);
			prep_stmt->setInt(5, Status);
			prep_stmt->setInt(6, ItemType);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddItem5 Error" << std::endl;
		//throw;关闭
	}

	return ItemNum + count;

}

// 锁映射表
std::unordered_map<UINT, std::mutex> mutex_map;

// 获取或插入一个锁
std::mutex& get_or_create_mutex(UINT uin) {
	// 使用 std::lock_guard 来确保线程安全
	static std::mutex map_mutex; // 保护 map 的访问
	std::lock_guard<std::mutex> guard(map_mutex);

	// 如果 uin 对应的锁已经存在，直接返回
	auto it = mutex_map.find(uin);
	if (it != mutex_map.end()) {
		return it->second;
	}

	// 否则，创建新的锁并插入到 map 中
	auto& new_mutex = mutex_map[uin]; // std::unordered_map 的 operator[] 会自动插入
	return new_mutex;
}

int PlayerDB_GetItemAvailPeriod(UINT Uin, UINT Itemid)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int AvailPeriod = 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select AvailPeriod from item  where uin = ? And Itemid =? "));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, Itemid);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			AvailPeriod = res->getInt("AvailPeriod");
		}
		return AvailPeriod;
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetStoneCar Error" << std::endl;
		//throw;关闭
	}

	return 1;

}



int PlayerDB_AddStoneCar(std::shared_ptr<ClientNode> Client)
{
	if (Client->Uin < 10000) return 0;

	// 获取锁
	std::mutex& mtx = get_or_create_mutex(Client->Uin);
	std::lock_guard<std::mutex> lock(mtx);

	//std::cout << Client->Uin << std::endl;
	std::vector<UINT> items;

	PlayerDB_GetStone(Client->Uin, items);
	for (size_t i = 0; i < items.size(); i++)
	{
		if (GetItemType(items[i]) == EAIT_CAR) {
			NotifyUpdateKartSkillStoneInfoNew(Client, items[i]);
		}
	}
	if (items.size() > 0) {
		PlayerDB_AddKartStoneGroove(Client->Uin, items);
	}


	items.clear();
	PlayerDB_GetKartRefit(Client->Uin, items);
	if (items.size() > 0) {
		PlayerDB_AddKartRefit(Client->Uin, items);
	}
	return 1;
}



int PlayerDB_GetStone(UINT Uin, std::vector<UINT>& vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select itemid from item a,itemallnew b where a.Itemid = b.id and b.Type = 'EAIT_CAR' and a.uin = ? and a.Itemid not in(select kartid from kartstonenew where uin = ?) "));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			UINT ItemID = res->getInt("Itemid");
			vecItem.push_back(ItemID);
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetStoneCar Error" << std::endl;
		//throw;关闭
	}

	return 1;

}


int PlayerDB_GetKartRefit(UINT Uin, std::vector<UINT>& vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select itemid from item a,itemallnew b where a.Itemid = b.id and b.Type = 'EAIT_CAR' and a.uin = ? and a.Itemid not in(select kartid from kartrefit where uin = ?)"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			UINT ItemID = res->getInt("Itemid");
			vecItem.push_back(ItemID);
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetStoneCar Error" << std::endl;
		//throw;关闭
	}

	return 1;

}

int PlayerDB_AddKartStoneGroove(UINT Uin, std::vector<UINT> vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	conn->setAutoCommit(false);
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		// 获取开始时间点
		//auto start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < vecItem.size(); i++)
		{
			UINT ItemID = vecItem[i];
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				int ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM kartstonenew  WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				if (res->rowsCount() > 0) {
					ID = 1;
				}
				res.reset();
				prep_stmt.reset();


				if (ID == 0)//不存在重复的宝石ID号 才加宝石和改装信息
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO kartstonenew (Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();
				}
			}
		}
		conn->commit();
		conn->setAutoCommit(true);

		// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// 输出执行时间
		//std::cout << "宝石执行时间: " << duration.count() << " 毫秒" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddKartStoneGroove Error" << std::endl;
		//throw;关闭
	}

	return 1;

}

int PlayerDB_AddKartRefit(UINT Uin, std::vector<UINT> vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	conn->setAutoCommit(false);
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	try
	{
		// 获取开始时间点
		//auto start = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < vecItem.size(); i++)
		{
			UINT ItemID = vecItem[i];
			if (GetItemType(ItemID) == EAIT_CAR)
			{
				int ItemType = 1;
				//注意这里先查询一下有就不加不然重复加会出bug
				int ID = 0;

				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM KartRefit WHERE Uin=? AND KartID=?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				// 执行查询
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				// 处理结果
				if (res->rowsCount() > 0) {
					ID = 1;
				}
				prep_stmt.reset();
				res.reset();

				//改装信息
				//printf("ID：%d\n", ID);
				if (ID == 0)
				{
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit(Uin,KartID) VALUES (?,?);"));
					prep_stmt->clearParameters();
					prep_stmt->setUInt(1, Uin);
					prep_stmt->setInt(2, ItemID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
			}
		}
		conn->commit();
		conn->setAutoCommit(true);
		// 获取结束时间点
		//auto end = std::chrono::high_resolution_clock::now();

		// 计算时间差
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// 输出执行时间
		//std::cout << "改装执行时间: " << duration.count() << " 毫秒" << std::endl;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddKartRefit Error" << std::endl;
		//throw;关闭
	}
	return 1;
}

DWORD PlayerDB_GetMapRecord(DWORD Uin, int MapID, std::shared_ptr<ClientNode> Client)
{
	int ret = -1;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Record  FROM MapRecord  WHERE Uin=? AND MapID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, MapID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			ret = res->getInt("Record");
		}
		res.reset();

		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetMapRecord Error" << std::endl;
		//throw;关闭
	}
}

bool PlayerDB_SetMapRecord(DWORD Uin, int MapID, DWORD Record, std::shared_ptr<ClientNode> Client)
{
	bool NewRecord = true;
	// 创建准备好的语句对象，使用占位符 ? 指定参数位置
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Record  FROM MapRecord  WHERE Uin=? AND MapID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, MapID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());

		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0)
		{
			while (res->next()) {
				//printf("%d\n", res->getInt("Record"));
				if (res->getInt("Record") < Record) {
					NewRecord = false;
				}
				else {
					std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE MapRecord SET Record=?  WHERE Uin=? AND MapID=?;"));
					prep_stmt->clearParameters();
					prep_stmt->setInt(1, Record);
					prep_stmt->setUInt(2, Uin);
					prep_stmt->setInt(3, MapID);
					prep_stmt->executeUpdate();
					prep_stmt.reset();

				}
				break;
			}
		}
		else
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO MapRecord (Uin, MapID, Record) VALUES(?, ?, ?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, MapID);
			prep_stmt->setInt(3, Record);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		res.reset();

		return NewRecord;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SetMapRecord Error" << std::endl;
		//throw;关闭
	}
}

void UserDB_ban(UINT Uin)
{
	DBConnection myDBConnection(usr); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE User SET isban=1 WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Uin - 10000);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UserDB_ban Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_AddMagicCupTempBox(std::shared_ptr<ClientNode> Client, DWORD Uin, DWORD ItemID, DWORD ItemNum, int AvailPeriod, int ItemType)
{
	int result;
	int count = 0;
	BOOL isExist = FALSE;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM MagicCupTempBox  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			isExist = TRUE;
			count = res->getInt("ItemNum");
		}
		res.reset();

		if (isExist) {
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE MagicCupTempBox SET ItemNum=?  WHERE Uin=? AND ItemID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setInt(1, ItemNum + count);
			prep_stmt->setUInt(2, Uin);
			prep_stmt->setInt(3, ItemID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		else
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO MagicCupTempBox (Uin,ItemID,ItemNum,AvailPeriod,ItemType) VALUES (?,?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->setInt(3, ItemNum);
			prep_stmt->setInt(4, AvailPeriod);
			prep_stmt->setInt(5, ItemType);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddMagicCupTempBox Error" << std::endl;
		//throw;关闭
	}
}

BOOL PlayerDB_SelectClientInfo(UINT Uin, ClientInfo*& clientInfo)
{
	int ret = false;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return false;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT *  FROM BaseInfo  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			ret = true;
			clientInfo[0].NickName = UTF2GB(res->getString("NickName").c_str());
			clientInfo[0].Gender = res->getUInt("Gender");
			clientInfo[0].Country = res->getUInt("Country");
			clientInfo[0].License = res->getUInt("License");
			clientInfo[0].Experience = res->getUInt("Experience");
			clientInfo[0].SuperMoney = res->getUInt("SuperMoney");
			clientInfo[0].Money = res->getUInt("Money");
			clientInfo[0].WinNum = res->getUInt("WinNum");
			clientInfo[0].SecondNum = res->getUInt("SecondNum");
			clientInfo[0].ThirdNum = res->getUInt("ThirdNum");
			clientInfo[0].TotalNum = res->getUInt("TotalNum");
			clientInfo[0].CurHonor = res->getUInt("CurHonor");
			clientInfo[0].TotalHonor = res->getUInt("TotalHonor");
			clientInfo[0].TodayHonor = res->getUInt("TodayHonor");
			clientInfo[0].RelaxTime = res->getUInt("RelaxTime");
			clientInfo[0].MonthDurationBefor = res->getUInt("MonthDurationBefor");
			clientInfo[0].MonthDurationCur = res->getUInt("MonthDurationCur");
			clientInfo[0].Charm = res->getUInt("Charm");
			clientInfo[0].DurationGame = res->getUInt("DurationGame");
			clientInfo[0].DanceExp = res->getUInt("DanceExp");
			clientInfo[0].Coupons = res->getUInt("Coupons");
			clientInfo[0].Admiration = res->getUInt("Admiration");
			clientInfo[0].LuckMoney = res->getUInt("LuckMoney");
			clientInfo[0].TeamWorkExp = res->getUInt("TeamWorkExp");
			clientInfo[0].AchievePoint = res->getUInt("AchievePoint");
			clientInfo[0].RegTime = res->getUInt("RegTime");
			clientInfo[0].Signature = UTF2GB(res->getString("Signature").c_str());
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectClientInfo Error" << std::endl;
		//throw;关闭
	}
}

BOOL PlayerDB_SelectClientInfo(std::shared_ptr<ClientNode> Client)
{
	int ret = false;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return false;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT *  FROM BaseInfo  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			ret = true;
			Client->fps = res->getUInt("fps");
			Client->SwitchFlag1 = res->getUInt("SwitchFlag1");
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectClientInfo Error" << std::endl;
		//throw;关闭
	}
}



int PlayerDB_GetItemNum(DWORD Uin, DWORD ItemID)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM Item WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			ItemNum = res->getInt("ItemNum");
			/*if (ItemNum == 0)
				PlayerDB_DeleteItem(Uin, ItemID);*/
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetItemNum Error" << std::endl;
		//throw;关闭
	}
	return ItemNum;
}


BOOL PlayerDB_DeleteItem(DWORD Uin, int ItemID)
{
	if (ItemID == 10020)
	{
		return FALSE;
	}

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return FALSE;
	conn->setAutoCommit(false);
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM Item  WHERE  Uin=? AND ItemID = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setInt(2, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

		if (GetItemType(ItemID) == EAIT_CAR) {
			//删除改装信息
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM KartRefit  WHERE  Uin=? AND KartID = ?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setInt(2, ItemID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();

			{
				//删除宝石信息
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM kartstonenew  WHERE  Uin=? AND KartID = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->setInt(2, ItemID);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}

		}
		conn->commit();
		conn->setAutoCommit(true);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_DeleteItem Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_Reduce(UINT Uin, UINT ItemID, int Num)
{
	UINT ItemNum = PlayerDB_GetItemNum(Uin, ItemID);
	if (ItemNum == Num) {
		PlayerDB_DeleteItem(Uin, ItemID);
		return;
	}

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum=ItemNum-? WHERE Uin = ? AND ItemID = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Num);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setInt(3, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_Reduce Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateItemNum(UINT Uin, UINT ItemID, int Num)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET ItemNum=? WHERE Uin = ? AND ItemID = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Num);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setInt(3, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateItemNum Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateItemWareHouse(UINT Uin, UINT ItemID, int OpType)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET WareHouse=? WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, OpType == 1 ? 0 : 1);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setUInt(3, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateItemWareHouse Error" << std::endl;
		//throw;关闭
	}
}


void PlayerDB_UpdateItemOtherAttribute(UINT Uin, UINT ItemID, int type)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Item SET OtherAttribute=?  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, type);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setUInt(3, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateItemOtherAttribute Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_SelectWareHouseItem(UINT Uin, std::vector<ItemInfo>& Items)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,AvailPeriod,Status,ObtainTime  FROM Item WHERE Uin = ? and WareHouse = 1;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			Items[ItemNum].ItemID = res->getUInt("ItemID");
			Items[ItemNum].ItemNum = res->getUInt("ItemNum");
			Items[ItemNum].AvailPeriod = res->getInt("AvailPeriod");
			Items[ItemNum].Status = res->getUInt("Status");
			Items[ItemNum].ObtainTime = res->getUInt("ObtainTime");
			Items[ItemNum].OtherAttribute = 0;
			Items[ItemNum].ItemType = 0;
			ItemNum++;
			if (ItemNum >= 600)
			{
				break;
			}
		}
		res.reset();
		return ItemNum;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectWareHouseItem Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_SelectItems(UINT Uin, std::vector<ItemInfo>& vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,AvailPeriod,Status,ObtainTime,OtherAttribute  FROM Item WHERE Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		ItemNum = res->rowsCount();

		for (size_t i = 0; i < ItemNum; i++)
		{
			ItemInfo TI;
			if (res->next()) {
				TI.ItemID = res->getUInt("ItemID");
				TI.ItemNum = res->getUInt("ItemNum");
				TI.AvailPeriod = res->getInt("AvailPeriod");
				TI.Status = res->getUInt("Status");
				TI.ObtainTime = res->getUInt("ObtainTime");
				TI.OtherAttribute = res->getUInt("OtherAttribute");
				TI.ItemType = 0;
			}

			vecItem.push_back(TI);
		}
		res.reset();
		return ItemNum;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectItems Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_SelectItemsStatus(UINT Uin, std::vector<ItemInfo>& vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,AvailPeriod,Status,ObtainTime,OtherAttribute  FROM Item WHERE Uin = ? And Status = 1;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		ItemNum = res->rowsCount();

		for (size_t i = 0; i < ItemNum; i++)
		{
			ItemInfo TI;
			if (res->next()) {
				TI.ItemID = res->getUInt("ItemID");
				TI.ItemNum = res->getUInt("ItemNum");
				TI.AvailPeriod = res->getInt("AvailPeriod");
				TI.Status = res->getUInt("Status");
				TI.ObtainTime = res->getUInt("ObtainTime");
				TI.OtherAttribute = res->getUInt("OtherAttribute");
				TI.ItemType = 0;
			}

			vecItem.push_back(TI);
		}
		res.reset();
		return ItemNum;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectItems Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_LoginSelectItems(UINT Uin, std::vector<ItemInfo>& vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,AvailPeriod,Status,ObtainTime,OtherAttribute  FROM Item WHERE Uin = ? AND (AvailPeriod <= 0 OR (AvailPeriod * 60 * 60 + ObtainTime) > UNIX_TIMESTAMP()); "));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		ItemNum = res->rowsCount();

		for (size_t i = 0; i < ItemNum; i++)
		{
			if (res->next()) {
				ItemInfo TI;
				TI.ItemID = res->getUInt("ItemID");
				TI.ItemNum = res->getUInt("ItemNum");
				TI.AvailPeriod = res->getInt("AvailPeriod");
				TI.Status = res->getUInt("Status");
				TI.ObtainTime = res->getUInt("ObtainTime");
				TI.OtherAttribute = res->getUInt("OtherAttribute");
				TI.ItemType = 0;
				vecItem.push_back(TI);
			}
		}
		res.reset();
		return ItemNum;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectItems Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_LoginSelectOverItems(UINT Uin, std::vector<ItemInfo>& vecItem)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,AvailPeriod,Status,ObtainTime,OtherAttribute  FROM Item WHERE Uin = ? AND (AvailPeriod > 0 and (AvailPeriod * 60 * 60 + ObtainTime) < UNIX_TIMESTAMP()); "));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		ItemNum = res->rowsCount();

		for (size_t i = 0; i < ItemNum; i++)
		{
			if (res->next()) {
				ItemInfo TI;
				TI.ItemID = res->getUInt("ItemID");
				TI.ItemNum = res->getUInt("ItemNum");
				TI.AvailPeriod = res->getInt("AvailPeriod");
				TI.Status = res->getUInt("Status");
				TI.ObtainTime = res->getUInt("ObtainTime");
				TI.OtherAttribute = res->getUInt("OtherAttribute");
				TI.ItemType = 0;
				vecItem.push_back(TI);
			}
		}
		res.reset();

		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update Item Set Status = 0 WHERE Uin = ? AND (AvailPeriod > 0 and (AvailPeriod * 60 * 60 + ObtainTime) < UNIX_TIMESTAMP());"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

		{
			int hasCar = 0;
			std::unique_ptr<sql::PreparedStatement>prep_stmt(conn->prepareStatement("SELECT Count(1)  FROM Item a,basegametypestatus b WHERE a.uin = b.uin and a.Itemid = b.KartID and a.Uin =? AND (AvailPeriod > 0 and (AvailPeriod * 60 * 60 + ObtainTime) < UNIX_TIMESTAMP());"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			// 处理结果
			while (res->next()) {
				hasCar = res->getUInt(1);
			}
			res.reset();

			if (hasCar) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update basegametypestatus Set Kartid = 10020 WHERE Uin = ?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, Uin);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
		}

		return ItemNum;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectItems Error" << std::endl;
		//throw;关闭
	}
}


BOOL PlayerDB_IsHasFriend(UINT Uin, UINT FriendUin)
{
	BOOL haveFriend = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return FALSE;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM FriendInfo Where Uin=? and FriendUin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, FriendUin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			haveFriend = 1;
			break;
		}
		res.reset();
		return haveFriend;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_IsHasFriend Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_InsertFriend(UINT Uin, UINT FriendUin)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO FriendInfo VALUES(?, ?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, FriendUin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertFriend Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateEngageInfo(UINT Uin, UINT EngageUin)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE EngageInfo SET EngageUin=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, EngageUin);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateEngageInfo Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateEngageInfoStat(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE EngageInfo SET Baby1EquipStat=?,Baby2EquipStat=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->LoverBaby1EquipStat);
		prep_stmt->setUInt(2, Client->LoverBaby2EquipStat);
		prep_stmt->setUInt(3, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateEngageInfoStat Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_InsertEngageInfo(UINT Uin, UINT EngageUin)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO EngageInfo VALUES(?, ?,0,0);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, EngageUin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertEngageInfo Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_DeleteEngageInfo(UINT Uin, UINT EngageUin)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM EngageInfo WHERE Uin = ? or Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, EngageUin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_DeleteEngageInfo Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateHead(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET UserHeads=?,UserHeadsBox=?,UserFloor=? WHERE Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Client->UserHeads);
		prep_stmt->setInt(2, Client->UserHeadsBox);
		prep_stmt->setInt(3, Client->UserFloor);
		prep_stmt->setUInt(4, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateHead Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateNobel(UINT Uin, UINT NobleID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET NobleID = ?  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, NobleID);
		prep_stmt->setUInt(2, Uin);

		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateNobel Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_InsertStoneNew2(UINT id, UINT Uin, UINT KartId, sql::Connection* conn)
{
	try
	{
		std::string sql = "Select *  FROM kartstone Where Id = ?";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, id);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		UINT SkillStoneID1 = 0;
		UINT SkillStoneID2 = 0;
		UINT SkillStoneID3 = 0;
		UINT SkillStoneID4 = 0;
		UINT SkillStoneID5 = 0;

		if (res->next()) {
			SkillStoneID1 = res->getUInt("SkillStoneID");
		}
		if (res->next()) {
			SkillStoneID2 = res->getUInt("SkillStoneID");
		}
		if (res->next()) {
			SkillStoneID3 = res->getUInt("SkillStoneID");
		}
		if (res->next()) {
			SkillStoneID4 = res->getUInt("SkillStoneID");
		}
		if (res->next()) {
			SkillStoneID5 = res->getUInt("SkillStoneID");
		}
		res.reset();

		int has = 0;
		{

			std::string sql = "Select *  FROM kartstonenew Where Uin= ? and Kartid = ?";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, KartId);
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			if (res->rowsCount() > 0) {

				has = 1;
			}
			res.reset();
		}
		if (has == 0)
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Insert Into kartstonenew(Uin,KartID,SkillStoneID1,SkillStoneID2,SkillStoneID3,SkillStoneID4,SkillStoneID5) Values(?,?,?,?,?,?,?)"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, KartId);
			prep_stmt->setUInt(3, SkillStoneID1);
			prep_stmt->setUInt(4, SkillStoneID2);
			prep_stmt->setUInt(5, SkillStoneID3);
			prep_stmt->setUInt(6, SkillStoneID4);
			prep_stmt->setUInt(7, SkillStoneID5);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertStoneNew2 Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_InsertStoneNew()
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	conn->setAutoCommit(false);
	try
	{
		int count = 0; // 计数器
		const int progressThreshold = 100000; // 每处理 100 条记录打印一次进度
		std::string sql = "Select *  FROM kartstonegroove";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		std::cout << "总共 " << res->rowsCount() << " 条记录" << std::endl;
		while (res->next()) {
			UINT Uin = res->getUInt("Uin");
			UINT KartID = res->getUInt("KartID");
			int id = res->getInt("ID");
			PlayerDB_InsertStoneNew2(id, Uin, KartID, conn);
			// 更新计数器
			count++;
			//每 progressThreshold 条记录打印一次进度
			if (count % progressThreshold == 0) {
				std::cout << "进度: 已处理 " << count << " 条记录" << std::endl;
			}
		}
		res.reset();
		conn->commit();
		conn->setAutoCommit(true);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertStoneNew Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_UpdateKartStone(UINT Uin, UINT KartID, UINT StoneGrooveID, UINT SkillID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		int id = -1;
		std::string sql = "SELECT SkillStoneID" + std::to_string(StoneGrooveID) + "  FROM kartstonenew  WHERE Uin=? AND KartID=?;";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, KartID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next()) {
			id = res->getInt(1);
		}
		res.reset();

		{
			sql = "UPDATE kartstonenew SET SkillStoneID" + std::to_string(StoneGrooveID) + " = ?,StoneUseOccaType" + std::to_string(StoneGrooveID) + " =? WHERE Uin = ? AND KartID = ? ; ";
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, SkillID);
			prep_stmt->setUInt(2, 0);
			prep_stmt->setUInt(3, Uin);
			prep_stmt->setUInt(4, KartID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		return id;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateKartStone Error" << std::endl;
		//throw;关闭
	}
}


void PlayerDB_SelectKartStone(UINT Uin, UINT KartID, std::vector<KartStoneItemInfo>& kartStoneItemInfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT *  FROM kartstonenew WHERE Uin =? and KartID = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, KartID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next()) {
			for (size_t i = 1; i <= 5; i++)
			{
				KartStoneItemInfo KI;
				KI.StoneUseOccaType = res->getUInt("StoneUseOccaType" + std::to_string(i));
				KI.SkillStoneID = res->getUInt("SkillStoneID" + std::to_string(i));
				kartStoneItemInfo.push_back(KI);
			}
			break;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectKartStone Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdatMagicLampInfo(UINT Uin, UINT ItemID, int LightOn, int LuckyValue)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE MagicLampInfo SET LightOn=?,LuckyValue=?  WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, LightOn);
		prep_stmt->setUInt(2, LuckyValue);
		prep_stmt->setUInt(3, Uin);
		prep_stmt->setUInt(4, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdatMagicLampInfo Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_DeleteMagicCupTempBox(UINT Uin, UINT ItemID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM MagicCupTempBox WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_DeleteMagicCupTempBox Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_GetMagicLampNum(DWORD Uin, DWORD ItemID)
{
	int CurrentRound = -1;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select LightOn from MagicLampInfo where Uin=? and ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, ItemID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			res->next();
			CurrentRound = res->getInt("LightOn");
		}
		res.reset();
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_GetMagicLampNum Error" << std::endl;
		//throw;关闭
	}
	return CurrentRound;
}

void PlayerDB_InsertMagicLamp(DWORD Uin, DWORD ItemID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO MagicLampInfo(Uin,ItemID) VALUES(?, ?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertMagicLamp Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateBugletTalk(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET BugletTalk=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->BugletTalk);
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateBugletTalk Error" << std::endl;
		//throw;关闭
	}
}

UINT PlayerDB_SelectGuildID(UINT GuildID, std::string& NickName)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	UINT Uin = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT GI.Uin,B.NickName FROM GuildInfo GI join BaseInfo B on GI.Uin=B.Uin where GI.Duty=0 and GI.GuildID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, GuildID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next()) {
			Uin = res->getUInt("Uin");
			NickName = UTF2GB(res->getString("NickName").c_str());
		}
		res.reset();
		return Uin;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectGuildID Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_SelectGuildInfo(UINT GuildID, Guidinfo*& guidinfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	UINT Uin = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ID,Name,EnlistType,RankLevel,Score,Attribute,MaxMember,CurrentMember,TeamAddition,Prosperity,Level FROM GuildList where ID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, GuildID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			guidinfo[0].ID = res->getUInt("ID");
			guidinfo[0].RankLevel = res->getUInt("RankLevel");
			guidinfo[0].Score = res->getUInt("Score");
			guidinfo[0].Attribute = res->getUInt("Attribute");
			guidinfo[0].MaxMember = res->getUInt("MaxMember");
			guidinfo[0].CurrentMember = res->getUInt("CurrentMember");
			guidinfo[0].TeamAddition = res->getUInt("TeamAddition");
			guidinfo[0].Prosperity = res->getUInt("Prosperity");
			guidinfo[0].Level = res->getUInt("Level");
			guidinfo[0].Name = UTF2GB(res->getString("Name").c_str());
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectGuildInfo Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_SelectGuildInfoByUin(UINT Uin, Guidinfo*& guidinfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT GI.GuildID,GI.Duty,GL.Name FROM GuildInfo GI join GuildList GL on GI.GuildID=GL.ID where GI.Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			guidinfo[0].ID = res->getUInt("GuildID");
			guidinfo[0].Duty = res->getUInt("Duty");
			guidinfo[0].Name = UTF2GB(res->getString("Name").c_str());
			break;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectGuildInfoByUin Error" << std::endl;
		//throw;关闭
	}
}


UINT PlayerDB_SelectGuildInfoAll(Guidinfo*& guidinfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	UINT count = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM GuildList Order By Score desc;"));
		prep_stmt->clearParameters();
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		count = res->rowsCount();
		guidinfo = new Guidinfo[count];
		memset(guidinfo, 0, sizeof(Guidinfo) * count);
		// 处理结果
		for (size_t i = 0; i < count; i++)
		{
			if (res->next()) {
				guidinfo[i].ID = res->getUInt("ID");
				guidinfo[i].RankLevel = res->getUInt("RankLevel");
				guidinfo[i].Score = res->getUInt("Score");
				guidinfo[i].Attribute = res->getUInt("Attribute");
				guidinfo[i].MaxMember = res->getUInt("MaxMember");
				guidinfo[i].CurrentMember = res->getUInt("CurrentMember");
				guidinfo[i].TeamAddition = res->getUInt("TeamAddition");
				guidinfo[i].Prosperity = res->getUInt("Prosperity");
				guidinfo[i].Level = res->getUInt("Level");

				guidinfo[i].Name = UTF2GB(res->getString("Name").c_str());
				guidinfo[i].EnlistType = res->getUInt("EnlistType");
			}
		}
		res.reset();
		return count;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectGuildInfoAll Error" << std::endl;
		//throw;关闭
	}
}


UINT PlayerDB_SelectGuildMember(UINT GuildID, Guidinfo*& guidinfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	UINT count = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT GI.Uin,GI.Duty,GI.MemProsperity,B.NickName FROM GuildInfo GI join BaseInfo B on GI.Uin=B.Uin where GuildID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, GuildID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();

		count = res->rowsCount();
		guidinfo = new Guidinfo[count];
		memset(guidinfo, 0, sizeof(Guidinfo) * count);
		// 处理结果
		for (size_t i = 0; i < count; i++)
		{
			if (res->next()) {
				guidinfo[i].Uin = res->getUInt("Uin");
				guidinfo[i].Duty = res->getUInt("Duty");
				guidinfo[i].MemProsperity = res->getUInt("MemProsperity");
				guidinfo[i].NickName = UTF2GB(res->getString("NickName").c_str());
			}
		}
		res.reset();
		return count;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectGuildMember Error" << std::endl;
		//throw;关闭
	}
}



void PlayerDB_InsertGuildList(UINT Uin, char* newGuildname)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("insert into GuildList(name,Score,Attribute,MaxMember,CurrentMember,Prosperity,Level,TeamAddition,RankLevel,LeaderUin) values(?,1,33,20,1,1,1,1,1,?);"));
		prep_stmt->clearParameters();
		if (newGuildname)
		{
			prep_stmt->setString(1, newGuildname);
		}
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertGuildList Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateGuildList(UINT ID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE GuildList SET CurrentMember=CurrentMember+1 WHERE ID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, ID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateGuildList Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateGuildListEnlistType(UINT ID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE GuildList SET EnlistType=3 WHERE ID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, ID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateGuildListEnlistType Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateGuildListCurrentMember(UINT ID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE GuildList SET CurrentMember=CurrentMember-1,EnlistType=2 WHERE ID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, ID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateGuildListCurrentMember Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_InsertGuildInfo(DWORD Uin, UINT GuildID, UINT Duty)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("insert into GuildInfo(Uin,GuildID,Duty) values(?,?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, GuildID);
		prep_stmt->setUInt(3, Duty);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertGuildInfo Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_DeleteGuildInfo(DWORD Uin)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM GuildInfo  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_DeleteGuildInfo Error" << std::endl;
		//throw;关闭
	}
}

void  PlayerDB_UpdateGuildInfoDuty(UINT Uin, UINT Duty)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update  GuildInfo Set Duty = ?  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Duty);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateGuildInfoDuty Error" << std::endl;
		//throw;关闭
	}
}


UINT GetGuildIDByUin(UINT uin) {
	DBConnection myDBConnection(player);
	auto conn = myDBConnection.getConnection();
	if (!conn) return 0;

	try {
		std::unique_ptr<sql::PreparedStatement> stmt(
			conn->prepareStatement("SELECT GuildID FROM GuildInfo WHERE Uin = ? LIMIT 1;")
		);
		stmt->setUInt(1, uin);
		auto res = stmt->executeQuery();
		return res->next() ? res->getUInt("GuildID") : 0;
	}
	catch (const std::exception& e) {
		std::cerr << "GetGuildID Error: " << e.what() << std::endl;
		return 0;
	}
}


void PlayerDB_UpdateGuildInfoDuty1(UINT Uin, UINT Duty, bool isTransferLeader)
{
	DBConnection myDBConnection(player);
	auto conn = myDBConnection.getConnection();
	if (!conn) return;

	try {
		conn->setAutoCommit(false);

		// 新增：获取目标用户的公会ID（关键修复）
		UINT guildId = 0;
		{
			std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(
				"SELECT GuildID FROM GuildInfo WHERE Uin = ? LIMIT 1;"
			));
			stmt->setUInt(1, Uin);
			auto res = stmt->executeQuery();
			if (res->next()) {
				guildId = res->getUInt("GuildID");
			}
		}

		if (isTransferLeader && Duty == 0 && guildId != 0) {
			// 限定公会范围清除原队长（关键修复）
			std::unique_ptr<sql::PreparedStatement> clear_stmt(conn->prepareStatement(
				"UPDATE GuildInfo SET Duty = 7 WHERE Duty = 0 AND GuildID = ?;" // 新增AND条件
			));
			clear_stmt->setUInt(1, guildId);
			clear_stmt->executeUpdate();
		}

		// 原更新逻辑保持不变
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(
			"UPDATE GuildInfo SET Duty = ? WHERE Uin = ?;"
		));
		prep_stmt->setUInt(1, Duty);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();

		conn->commit();
	}
	catch (...) {
		conn->rollback();
		throw;
	}
}


void PlayerDB_UpdateGuildListScore(UINT GuildID, UINT Score, UINT Prosperity)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE  GuildList Set Score = Score +?,Prosperity = Prosperity+?  WHERE ID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Score);
		prep_stmt->setUInt(2, Prosperity);
		prep_stmt->setUInt(3, GuildID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateGuildListScore Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateGuildInfoScore(UINT Uin, UINT GuildID, UINT MeProsperity)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE  GuildInfo Set MemProsperity = MemProsperity + ?  WHERE Uin = ? and GuildID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, MeProsperity);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setUInt(3, GuildID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateGuildInfoScore Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_InsertKartRefit(DWORD Uin, UINT KartItemID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO KartRefit (Uin,KartID) VALUES (?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, KartItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertKartRefit Error" << std::endl;
		//throw;关闭
	}
}

void PlayerDB_UpdateKartRefit(DWORD Uin, UINT* RefitItemID, UINT ShapeSuitID, UINT ShapeLegendSuitID, UINT SkinSuitID, UINT KartItemID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		const char* sql = nullptr;
		sql = "UPDATE KartRefit SET KartHeadRefitItemID=?, KartTailRefitItemID=?, KartFlankRefitItemID=?, "
			"KartTireRefitItemID=?, ShapeSuitID=?,ShapeLegendSuitID=?, SkinSuitID=? WHERE Uin=? AND KartID=?;";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, RefitItemID[0]);//车头
		prep_stmt->setUInt(2, RefitItemID[1]);//尾翼
		prep_stmt->setUInt(3, RefitItemID[2]);//侧翼
		prep_stmt->setUInt(4, RefitItemID[3]);//车胎

		prep_stmt->setUInt(5, ShapeSuitID);//赛车皮肤
		//prep_stmt->setUInt(6, LegendSuitLevel);//赛车传奇皮肤等级
		//prep_stmt->setUInt(7, LegendSuitLevelChoice);//赛车传奇皮肤等级标志
		prep_stmt->setUInt(6, ShapeLegendSuitID);//赛车传奇皮肤ID
		prep_stmt->setUInt(7, SkinSuitID);//赛车皮肤
		prep_stmt->setUInt(8, Uin);
		prep_stmt->setUInt(9, KartItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateKartRefit Error" << std::endl;
		//throw;关闭
	}
}


void PlayerDB_UpNickName(DWORD Uin, std::string& NickName)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET NickName=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, NickName);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpNickName Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_SelectNickName(UINT Uin, std::string& NickName)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT NickName FROM BaseInfo  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			NickName = UTF2GB(res->getString("NickName").c_str());
			break;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectNickName Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_SelectMapInfo(UINT Uin, std::vector<MapInfo>& mapInfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT MapID,Record  FROM MapRecord WHERE Uin = ?;"));

		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			MapInfo MI;
			MI.MapID = res->getUInt("MapID");
			MI.Record = res->getUInt("Record");
			mapInfo.push_back(MI);
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectMapInfo Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_SelectPetInfo(UINT Uin, std::vector<PetInfo>& petInfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT PetId,Name,Experience,PL,Status,StrengLevel FROM Pet WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			PetInfo PI;
			PI.ID = res->getUInt("PetId");
			PI.Exp = res->getUInt("Experience");
			PI.PL = res->getUInt("PL");
			PI.Status = res->getUInt("Status");
			PI.StrengLevel = res->getUInt("StrengLevel");
			petInfo.push_back(PI);
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectPetInfo Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_SelectRankInfo(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT GradeScore,GradeLevel,ChildLevel,TotalScore,PromotionMatch,CurrentPromotionRoundNums,CurrentPromotionWinRoundNums,CurrentPromotionLoseRoundNums,PromotionRecordTag FROM RankInfo Where Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			Client->GradeScore = res->getUInt(1);
			Client->GradeLevel = res->getUInt(2);
			Client->ChildLevel = res->getUInt(3);
			Client->TotalScore = res->getUInt("TotalScore");
			Client->PromotionMatch = res->getUInt("PromotionMatch");
			Client->CurrentPromotionRoundNums = res->getUInt("CurrentPromotionRoundNums");
			Client->CurrentPromotionWinRoundNums = res->getUInt("CurrentPromotionWinRoundNums");
			Client->CurrentPromotionLoseRoundNums = res->getUInt("CurrentPromotionLoseRoundNums");
			Client->PromotionRecordTag = res->getUInt("PromotionRecordTag");

			if (Client->GradeLevel > 7)
			{
				Client->GradeLevel = 7;
			}
		}
		else
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO RankInfo(Uin, GradeScore, GradeLevel, ChildLevel) VALUES(? , 0, 0, 3);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectRankInfo Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_GetMagicCupTempBoxNum(DWORD Uin, DWORD ItemID)
{
	int Itemnum = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum FROM MagicCupTempBox  WHERE Uin=? and ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, ItemID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			res->next();
			Itemnum = res->getInt("ItemNum");
		}
		res.reset();
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_GetMagicCupTempBoxNum Error" << std::endl;
		//throw;关闭
	}
	return Itemnum;
}

int  PlayerDB_GetMagicCupTempBox(DWORD Uin, std::vector<ItemInfo>& Items)
{
	int num = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemID,ItemNum,ItemType FROM MagicCupTempBox  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			Items[num].ItemID = res->getUInt("ItemID");
			Items[num].ItemNum = res->getUInt("ItemNum");
			Items[num].ItemType = res->getUInt("ItemType");
			num++;
			if (num == 1000)//不超过1000
			{
				break;
			}
		}
		res.reset();
		return num;
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_GetMagicCupTempBox Error" << std::endl;
		//throw;关闭
	}
}

void  PlayerDB_GetMagicFormationNum(DWORD Uin, std::unordered_map<int, int>& Items)
{
	int num = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemID FROM MagicFormation  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			UINT ID = res->getUInt("ItemID");
			Items[ID] = 1;
		}
		res.reset();
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_GetMagicFormationNum Error" << std::endl;
		//throw;关闭
	}
}

void  PlayerDB_InsertMagicFormation(DWORD Uin, UINT ItemID)
{
	int num = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO MagicFormation(Uin,ItemID) VALUES(?, ?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_InsertMagicFormation Error" << std::endl;
		//throw;关闭
	}
}

void  PlayerDB_GetMagicShopInfo(DWORD Uin, std::unordered_map<int, int>& Items)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemID FROM MagicShopInfo  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			UINT ID = res->getUInt("ItemID");
			Items[ID] = 1;
		}
		res.reset();
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_GetMagicShopInfo Error" << std::endl;
		//throw;关闭
	}
}


void  PlayerDB_InsertMagicShopInfo(DWORD Uin, UINT ItemID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO MagicShopInfo(Uin,ItemID) VALUES(?, ?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, ItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception  e)
	{
		std::cerr << e.what() << "PlayerDB_InsertMagicShopInfo Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_GetPetLuck(DWORD Uin, UINT Petid)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	int Luckvalue = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT luckvalue FROM pet  WHERE Uin=? and petid = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, Petid);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		if (res->rowsCount() > 0) {
			res->next();
			Luckvalue = res->getUInt("luckvalue");
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetPetLuck Error" << std::endl;
		//throw;关闭
	}
	return Luckvalue;
}

void PlayerDB_UpdatePetLuck(DWORD Uin, UINT Petid, UINT Luck)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE pet set LuckValue = LuckValue + ?  WHERE Uin=? AND PetId=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Luck);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setUInt(3, Petid);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdatePetLuck Error" << std::endl;
		//throw;关闭
	}

}


void PlayerDB_UpdatePetStrengLevel(DWORD Uin, UINT Petid, UINT StrengLevel)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE pet SET StrengLevel = ?,Luckvalue = 0  WHERE Uin=? AND PetId=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, StrengLevel);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->setUInt(3, Petid);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdatePetStrengLevel Error" << std::endl;
		//throw;关闭
	}

}


void PlayerDB_InsertAuctionUnPayInfo(DWORD Uin, UINT BidPrice, AuctionItemInfo AuctionItems)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO AuctionUnPayInfo(BidUin,CommodityId,BidPrice,ItemId,ItemAvailPeriod,ItemNumber) VALUES(?,?,?,?,?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, AuctionItems.CommodityId);
		prep_stmt->setUInt(3, BidPrice);
		prep_stmt->setUInt(4, AuctionItems.ItemID);
		prep_stmt->setInt(5, AuctionItems.AvailPeriod);
		prep_stmt->setUInt(6, AuctionItems.ItemNum);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertAuctionUnPayInfo Error" << std::endl;
		//throw;关闭
	}

}

void PlayerDB_DeleteAuctionInfo(UINT CommodityId)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM AuctionInfo WHERE CommodityId=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, CommodityId);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_DeleteAuctionInfo Error" << std::endl;
		//throw;关闭
	}

}

void PlayerDB_DeleteAuctionUnPayInfo(UINT Uin, UINT CommodityId)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("DELETE FROM AuctionUnPayInfo WHERE CommodityId=? and BidUin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, CommodityId);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_DeleteAuctionUnPayInfo Error" << std::endl;
		//throw;关闭
	}

}

UINT PlayerDB_SelectAuctionUnPayInfo(DWORD Uin, std::vector<AuctionItemInfo>& auction)
{
	UINT UnPayNumber = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select * from AuctionUnPayInfo where BidUin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		UnPayNumber = res->rowsCount();
		while (res->next()) {
			AuctionItemInfo AI;
			AI.CommodityId = res->getInt("CommodityId");
			AI.BidPrice = res->getUInt("BidPrice");
			AI.ItemID = res->getUInt("ItemId");
			AI.AvailPeriod = res->getInt("ItemAvailPeriod");
			AI.ItemNum = res->getUInt("ItemNumber");
			auction.push_back(AI);
		}
		res.reset();
		return UnPayNumber;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectAuctionUnPayInfo Error" << std::endl;
		//throw;关闭
	}

}


int PlayerDB_SelectPlayerInfo(std::shared_ptr<ClientNode> Client, std::vector<PlayerInfo>& playerInfo)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT VipFlag,LastLoginTime,NobleID,NobleLevel,NoblePoint,GuildVipLevel,GuildVipPoint,LoverVipLevel,LoverVipPoint,EmperorLevel,EmperorPoint,GardenLevel,GardenPoint FROM Player WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			ret = 1;
			res->next();
			PlayerInfo PI;
			PI.VipFlag = res->getUInt("VipFlag");
			PI.NobleID = res->getUInt("NobleID");
			PI.NobleLevel = res->getUInt("NobleLevel");
			PI.NoblePoint = res->getUInt("NoblePoint");
			PI.GuildVipLevel = res->getUInt("GuildVipLevel");
			PI.GuildVipPoint = res->getUInt("GuildVipPoint");
			PI.LoverVipLevel = res->getUInt("LoverVipLevel");
			PI.LoverVipPoint = res->getUInt("LoverVipPoint");

			PI.EmperorLevel = res->getUInt("EmperorLevel");
			PI.EmperorPoint = res->getUInt("EmperorPoint");
			PI.GardenLevel = res->getUInt("GardenLevel");
			PI.GardenPoint = res->getUInt("GardenPoint");
			PI.LastLoginTime = UTF2GB(res->getString("LastLoginTime").c_str());
			playerInfo.push_back(PI);
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectPlayerInfo Error" << std::endl;
		//throw;关闭
	}

}

int PlayerDB_SelectPlayerInfo(UINT Uin, std::vector<PlayerInfo>& playerInfo)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT VipFlag,LastLoginTime,NobleID,NobleLevel,NoblePoint,GuildVipLevel,GuildVipPoint,LoverVipLevel,LoverVipPoint,EmperorLevel,EmperorPoint,GardenLevel,GardenPoint FROM Player WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			ret = 1;
			res->next();
			PlayerInfo PI;
			PI.VipFlag = res->getUInt("VipFlag");
			PI.NobleID = res->getUInt("NobleID");
			PI.NobleLevel = res->getUInt("NobleLevel");
			PI.NoblePoint = res->getUInt("NoblePoint");
			PI.GuildVipLevel = res->getUInt("GuildVipLevel");
			PI.GuildVipPoint = res->getUInt("GuildVipPoint");
			PI.LoverVipLevel = res->getUInt("LoverVipLevel");
			PI.LoverVipPoint = res->getUInt("LoverVipPoint");

			PI.EmperorLevel = res->getUInt("EmperorLevel");
			PI.EmperorPoint = res->getUInt("EmperorPoint");
			PI.GardenLevel = res->getUInt("GardenLevel");
			PI.GardenPoint = res->getUInt("GardenPoint");
			PI.LastLoginTime = UTF2GB(res->getString("LastLoginTime").c_str());
			playerInfo.push_back(PI);
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectPlayerInfo Error" << std::endl;
		//throw;关闭
	}

}


int PlayerDB_UpdatePlayerInfo(std::shared_ptr<ClientNode> Client, char* CurLoginTm)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Player SET LastLoginTime=?,NobleLevel=?,NoblePoint=?,GuildVipPoint=?,LoverVipPoint=?,EmperorPoint=?,GardenLevel=?,GardenPoint=? WHERE Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, CurLoginTm);
		prep_stmt->setUInt(2, Client->NobleLevel);
		prep_stmt->setUInt(3, Client->NoblePoint);
		prep_stmt->setUInt(4, Client->GuildVipPoint);
		prep_stmt->setUInt(5, Client->LoverVipPoint);
		prep_stmt->setUInt(6, Client->EmperorPoint);
		prep_stmt->setUInt(7, Client->GardenLevel);
		prep_stmt->setUInt(8, Client->GardenPoint);
		prep_stmt->setUInt(9, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdatePlayerInfo Error" << std::endl;
		//throw;关闭
	}

}


int PlayerDB_InsertPlayer(UINT Uin)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Player (Uin,VipFlag,IsInTopList,NobleID) VALUES (?,?,?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, 0x0107);
		prep_stmt->setUInt(3, 1);
		prep_stmt->setUInt(4, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_InsertPlayer Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_InsertBaseInfo(UINT Uin, char* newname, UINT Gender, UINT Country)
{
	DBConnection myDBConnection(player);
	auto conn = myDBConnection.getConnection();
	if (!conn) return 0;

	try {
		// 设置 SuperMoney 初始值为 1888888
		std::unique_ptr<sql::PreparedStatement> prep_stmt(
			conn->prepareStatement("INSERT INTO BaseInfo (Uin, NickName, Gender, Country, SuperMoney) VALUES (?, ?, ?, ?, 1888888);")
		);

		prep_stmt->setUInt(1, Uin);

		if (newname) {
			prep_stmt->setString(2, newname);
			delete[] newname;
			newname = NULL;
		}

		prep_stmt->setUInt(3, Gender);
		prep_stmt->setUInt(4, Country);

		prep_stmt->executeUpdate();

		// 提交事务（如果需要）
		conn->commit();

		prep_stmt.reset();

		// 插入 BaseGameTypeStatus
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(
				conn->prepareStatement("INSERT INTO BaseGameTypeStatus (Uin) VALUES (?);")
			);

			prep_stmt->setUInt(1, Uin);
			prep_stmt->executeUpdate();
			conn->commit();
			prep_stmt.reset();
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << " - PlayerDB_InsertBaseInfo Error" << std::endl;
	}

	return 1;
}


int PlayerDB_UpdateBaseInfo(UINT Uin, std::string qqNumber)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update BaseInfo Set QQ =? WHere Uin =?"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, qqNumber);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateBaseInfo Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_UpdateBaseInfoFps(std::shared_ptr<ClientNode> Client)
{
	Client->fps = (Client->fps == 0) ? 2 : 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update BaseInfo Set fps =? WHere Uin =?"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->fps);
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
		return Client->fps;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateBaseInfoFps Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_SelectBaseInfoKartLibaray(UINT Uin, std::vector<UINT>& myKartLibrary)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Select KartLibaray From BaseInfo  WHere Uin =?"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			std::string KartLibaray = res->getString("KartLibaray");
			initializeLibrary(myKartLibrary, KartLibaray); // 调用外部方法初始化
			break;
		}
		res.reset();
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectBaseInfoKartLibaray Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_UpdateBaseInfoKartLibaray(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::ostringstream oss;
		for (size_t i = 0; i < Client->MyKartLibrary.size(); ++i) {
			oss << Client->MyKartLibrary[i];
			if (i < Client->MyKartLibrary.size() - 1) {
				oss << '^'; // 添加分隔符
			}
		}
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update BaseInfo Set KartLibaray =? WHere Uin =?"));
		prep_stmt->clearParameters();
		prep_stmt->setString(1, oss.str());
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateBaseInfoKartLibaray Error" << std::endl;
		//throw;关闭
	}
}


int PlayerDB_SeleteBaseInfoHead(UINT Uin, std::vector<PlayerInfo>& playerInfo)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Select UserHeads,UserHeadsBox,UserFloor,QQ From BaseInfo Where Uin =?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);

		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			PlayerInfo PI;
			PI.UserHeads = res->getUInt("UserHeads");
			PI.UserHeadsBox = res->getUInt("UserHeadsBox");
			PI.UserFloor = res->getUInt("UserFloor");
			PI.qqNumber = res->getString("QQ");
			PI.Uin = Uin;
			playerInfo.push_back(PI);
			break;
		}
		res.reset();
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SeleteBaseInfoHead Error" << std::endl;
		//throw;关闭
	}
}



int PlayerDB_SelectEngageInfo(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT EngageUin,Baby1EquipStat,Baby2EquipStat FROM EngageInfo WHERE Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			Client->EngageUin = res->getUInt("EngageUin");
			Client->LoverBaby1EquipStat = res->getUInt("Baby1EquipStat");
			Client->LoverBaby2EquipStat = res->getUInt("Baby2EquipStat");
			break;
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectEngageInfo Error" << std::endl;
		//throw;关闭
	}
}



int PlayerDB_SelectComItemTableNum(std::shared_ptr<ClientNode> Client) {

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection();  if (!conn) return 0;
	int ItemNum = 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT count(*) FROM RetComItemTable WHERE UIN=? And OpNum >0;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			ItemNum = res->getUInt(1);
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectComItemTableNum Error" << std::endl;
		//throw;关闭
	}
	return ItemNum;
}


void PlayerDB_SelectComItemTable(std::shared_ptr<ClientNode> Client, BYTE*& pData, double& intValue) {

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	int count = 0;
	bool first = false;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemId,OpNum,Aviable FROM RetComItemTable WHERE UIN=? And OpNum >0;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next())
		{
			count++;
			if (first == false) {
				first = true;
				Write8(pData, 246);//F6
				intValue = 1;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 253); //FD
				Write32(pData, 0x00000051);
				Write8(pData, 250);//FA
				Write8(pData, 6);//len
				memcpy(pData, "itemID", 6);
				pData += 6;
				Write8(pData, 246);//F6
				intValue = res->getUInt("ItemId");
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 7);//len
				memcpy(pData, "bOpType", 7);
				pData += 7;
				Write8(pData, 246);//F6
				intValue = 0;
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 8);//len
				memcpy(pData, "iAviable", 8);
				pData += 8;
				Write8(pData, 246);//F6
				intValue = res->getInt("Aviable");
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 250);//FA
				Write8(pData, 5);//len
				memcpy(pData, "OpNum", 5);
				pData += 5;
				Write8(pData, 246);//F6
				intValue = res->getUInt("OpNum");
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);  // 移动到下一个数据的位置

				Write8(pData, 251);  // FB
				Write8(pData, 4);  // Param_fb=4
				Write8(pData, 246); // F6
				intValue = 1; // Param_f6=0.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

			}
			else {
				Write8(pData, 246);  // F6
				intValue = count;  // Param_f6=2.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

				Write8(pData, 253); // FD
				Write32(pData, 0x00000037);  // Param_fd=00000037

				Write8(pData, 251);  // FB
				Write8(pData, 17);  // Param_fb=17
				Write8(pData, 246); // F6
				intValue = res->getUInt("ItemId"); // Param_f6=115165.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

				Write8(pData, 251);  // FB
				Write8(pData, 18);  // Param_fb=18
				Write8(pData, 246); // F6
				intValue = 0.0; // Param_f6=0.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

				Write8(pData, 251);  // FB
				Write8(pData, 19);  // Param_fb=19
				Write8(pData, 246); // F6
				intValue = res->getInt("Aviable");// Param_f6=720.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

				Write8(pData, 251);  // FB
				Write8(pData, 20);  // Param_fb=20
				Write8(pData, 246); // F6
				intValue = res->getUInt("OpNum"); // Param_f6=1.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

				Write8(pData, 251);  // FB
				Write8(pData, 4);  // Param_fb=4
				Write8(pData, 246); // F6
				intValue = 1; // Param_f6=0.0
				std::memcpy(pData, &intValue, sizeof(double));
				pData += sizeof(double);

			}
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SelectComItemTable Error" << std::endl;
		//throw;关闭
	}

}


void PlayerDB_AddComItemTable(std::shared_ptr<ClientNode> Client, UINT ItemId, int Aviable, int num)
{
	if (num == 0) {
		PlayerDB_AddItem2(Client, Client->Uin, ItemId, 1, 0, false);
		return;
	}
	bool has = false;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT UIN FROM RetComItemTable WHERE UIN=? And ItemId = ? And Aviable = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->setUInt(2, ItemId);
		prep_stmt->setInt(3, Aviable);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			has = true;
		}
		res.reset();

		if (has == false) {
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO RetComItemTable(Uin,ItemId,Aviable,ExchangePieceNum) VALUES(?,?,?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, ItemId);
			prep_stmt->setInt(3, Aviable);
			prep_stmt->setUInt(4, num);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		else {
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE RetComItemTable Set OpNum = OpNum+1 WHERE UIN=? And ItemId = ? And Aviable = ?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Client->Uin);
			prep_stmt->setUInt(2, ItemId);
			prep_stmt->setInt(3, Aviable);;
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_AddComItemTable Error" << std::endl;
		//throw;关闭
	}
}
void PlayerDB_UpdateComItemTable(std::shared_ptr<ClientNode> Client, UINT ItemId, int Aviable, int num)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE RetComItemTable Set OpNum = OpNum-? WHERE UIN=? And ItemId = ? And Aviable = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, num);
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->setUInt(3, ItemId);
		prep_stmt->setInt(4, Aviable);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_UpdateComItemTable Error" << std::endl;
		//throw;关闭
	}
}

int PlayerDB_GetExchangePieceNum(std::shared_ptr<ClientNode> Client, UINT ItemId, int Aviable, UINT ItemNum)
{
	int itemnum = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ExchangePieceNum FROM RetComItemTable WHERE UIN=? And ItemId = ? And Aviable = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->setUInt(2, ItemId);
		prep_stmt->setInt(3, Aviable);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			itemnum = res->getUInt("ExchangePieceNum") * ItemNum;
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_GetExchangePieceNum Error" << std::endl;
		//throw;关闭
	}
	return itemnum;
}


void PlayerDB_SetRankedInfo(std::shared_ptr<ClientNode> Client)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE RankInfo SET GradeLevel=?,ChildLevel=?,GradeScore=?,TotalScore=?,PromotionMatch=?,CurrentPromotionRoundNums=?,CurrentPromotionWinRoundNums=?,CurrentPromotionLoseRoundNums=?,PromotionRecordTag=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Client->GradeLevel);
		prep_stmt->setInt(2, Client->ChildLevel);
		prep_stmt->setInt(3, Client->GradeScore);
		prep_stmt->setInt(4, Client->TotalScore);
		prep_stmt->setInt(5, Client->PromotionMatch);
		prep_stmt->setInt(6, Client->CurrentPromotionRoundNums);
		prep_stmt->setInt(7, Client->CurrentPromotionWinRoundNums);
		prep_stmt->setInt(8, Client->CurrentPromotionLoseRoundNums);
		prep_stmt->setInt(9, Client->PromotionRecordTag);
		prep_stmt->setUInt(10, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "PlayerDB_SetRankedInfo Error" << std::endl;
		//throw;关闭
	}
}


BOOL isDBtHasItem(std::shared_ptr<ClientNode> Client, int ItemID, int count)
{
	int ItemNum = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return FALSE;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT ItemNum from Item WHERE Uin=? AND ItemID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->setInt(2, ItemID);
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		// 处理结果
		while (res->next()) {
			ItemNum = res->getInt("ItemNum");
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "isDBtHasItem Error" << std::endl;
		//throw;关闭
	}
	if (ItemNum >= count)
	{
		return TRUE;
	}
	return FALSE;
}




void UpdateClientBase(std::shared_ptr<ClientNode> Client) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET Experience=?,SuperMoney=?,TotalHonor=?,DanceExp=?,Charm=?,Money=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Experience);
		prep_stmt->setUInt(2, Client->SuperMoney);
		prep_stmt->setUInt(3, Client->TotalHonor);
		prep_stmt->setUInt(4, Client->DanceExp);
		prep_stmt->setUInt(5, Client->Charm);
		prep_stmt->setUInt(6, Client->Money);
		prep_stmt->setUInt(7, Client->Uin);
		prep_stmt->executeUpdate();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateClientExp Error" << std::endl;
		//throw;关闭

	}
}

void UpdateGuildInfo(std::shared_ptr<ClientNode> Client, int score)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt1(conn->prepareStatement("UPDATE GuildList SET Score=Score+?,Prosperity=Prosperity+? WHERE ID=?;"));
		prep_stmt1->clearParameters();
		prep_stmt1->setUInt(1, score);
		prep_stmt1->setUInt(2, score);
		prep_stmt1->setUInt(3, Client->GuildID);

		prep_stmt1->executeUpdate();

		std::unique_ptr<sql::PreparedStatement> prep_stmt2(conn->prepareStatement("UPDATE GuildInfo SET MemProsperity=MemProsperity+? WHERE Uin=? and GuildID=?;"));
		prep_stmt2->clearParameters();
		prep_stmt2->setUInt(1, score);
		prep_stmt2->setUInt(2, Client->Uin);
		prep_stmt2->setUInt(3, Client->GuildID);

		prep_stmt2->executeUpdate();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateClientExp Error" << std::endl;
		//throw;关闭

	}
}


void SelectClientExp(std::shared_ptr<ClientNode> Client) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Experience,SuperMoney,TotalHonor,DanceExp,Charm,Money FROM BaseInfo WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);;
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			Client->Experience = res->getUInt("Experience");
			Client->SuperMoney = res->getUInt("SuperMoney");
			Client->TotalHonor = res->getUInt("TotalHonor");
			Client->DanceExp = res->getUInt("DanceExp");
			Client->Charm = res->getUInt("Charm");
			Client->Money = res->getUInt("Money");
		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectClientExp Error" << std::endl;
		//throw;关闭
	}
}


UINT SelectClientExp(UINT Uin) {
	UINT exp;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Experience FROM BaseInfo WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);;
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			res->next();
			exp = res->getInt("Experience");

		}
		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectClientExp Error" << std::endl;
		//throw;关闭
	}
	return exp;
}


BOOLEAN SelectSuperItem(std::shared_ptr<ClientNode> Client, UINT Itemid) {
	BOOLEAN Ret = false;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return FALSE;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM Item Where ItemId=? and Status=1 and Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Itemid);
		prep_stmt->setUInt(2, Client->Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->rowsCount() > 0) {
			Ret = true;
		}
		res.reset();
		return Ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectSuperItem Error" << std::endl;
		//throw;关闭
	}
}

void InsertSign3Info(std::shared_ptr<ClientNode> Client) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO Sign3Info(Uin) VALUES(?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "InsertSign3Info Error" << std::endl;
		//throw;关闭
	}
}

void UpdateSign3Info(std::shared_ptr<ClientNode> Client, char NextSignDay, char* CurTm, int SignWeek) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Sign3Info SET SignNum=?,SignTime=?,SignWeek=?  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, NextSignDay);
		prep_stmt->setString(2, CurTm);
		prep_stmt->setInt(3, SignWeek);
		prep_stmt->setUInt(4, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateSign3Info Error" << std::endl;
		//throw;关闭

	}
}

void UpdateSign3InfoWeek(std::shared_ptr<ClientNode> Client) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE Sign3Info SET SignWeek=0  WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateSign3InfoWeek Error" << std::endl;
		//throw;关闭

	}
}

void UpdateBuglet(std::shared_ptr<ClientNode> Client) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE BaseInfo SET BugletTalk=? WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Client->BugletTalk);
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateBuglet Error" << std::endl;
		//throw;关闭
	}
}

void UpdateClientKart(std::shared_ptr<ClientNode> Client, int Speed2Weight, int DriftVecWeight, int AdditionalZSpeedWeight, int KartItemID) {
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(
			conn->prepareStatement("UPDATE KartRefit SET RefitCout=RefitCout+1, Speed2Weight=?,DriftVecWeight=?,AdditionalZSpeedWeight=? WHERE Uin=? AND KartID=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Speed2Weight);
		prep_stmt->setUInt(2, DriftVecWeight);
		prep_stmt->setUInt(3, AdditionalZSpeedWeight);
		prep_stmt->setUInt(4, Client->Uin);
		prep_stmt->setUInt(5, KartItemID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateClientKart Error" << std::endl;
		//throw;关闭
	}
}


std::vector<int> executeQueryRow(const std::string& sqlQuery, const std::string& queryName, const std::vector<int>& conditionValues) {
	DBConnection myDBConnection(player);
	std::vector<int> resultRow;
	auto conn = myDBConnection.getConnection(); if (!conn) return resultRow;
	try {

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sqlQuery));
		for (size_t i = 0; i < conditionValues.size(); ++i) {
			prep_stmt->setUInt(i + 1, conditionValues[i]);
		}
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());

		// 获取结果集的元数据
		int num_cols = res->getMetaData()->getColumnCount();

		// 如果结果集非空，取第一行数据
		if (res->next()) {
			for (int i = 1; i <= num_cols; ++i) {
				resultRow.push_back(res->getInt(i));
			}
		}


		// 重置智能指针，释放资源
		res.reset();
		prep_stmt.reset();

		// 返回第一行结果
		return resultRow;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << " Error executing query: " << queryName << std::endl;
		//throw;关闭
	}
}

std::vector<int> executeQueryCol(const std::string& sqlQuery, const std::string& queryName, const std::vector<int>& conditionValues) {
	DBConnection myDBConnection(player);
	std::vector<int> resultRow;
	auto conn = myDBConnection.getConnection(); if (!conn) return resultRow;
	try {

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sqlQuery));
		for (size_t i = 0; i < conditionValues.size(); ++i) {
			prep_stmt->setUInt(i + 1, conditionValues[i]);
		}
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());

		// 获取结果集的元数据
		int num_cols = res->getMetaData()->getColumnCount();

		// 如果结果集非空
		while (res->next()) {
			resultRow.push_back(res->getInt(1));
		}

		// 重置智能指针，释放资源
		res.reset();
		prep_stmt.reset();

		// 返回结果
		return resultRow;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << " Error executing query: " << queryName << std::endl;
		//throw;关闭
	}
}



////std::vector<int>		RowData;
//typedef std::vector<int>		RowData;
//typedef std::vector<RowData>	TableData;
//
//int CommonSelectTable(std::string tableName, std::string where, TableData& tableData) 
//{
//	res = select(...);
//	int row = res.row;
//	int colum = res.column;
//
//	for (int i = 0; i < row; i++) {
//		resRow = res[i];
//		RowData rowData;
//		for (int j = 0; j < colum; j++) {
//			rowData.push_back(resRow[j]);
//		}
//		tableData.push_back(rowData);
//	}
//}


std::vector<int> executeQueryRow(const std::string& sqlQuery, const std::string& queryName, const std::vector<std::string>& conditionValues) {
	DBConnection myDBConnection(player);
	std::vector<int> resultRow;
	auto conn = myDBConnection.getConnection(); if (!conn) return resultRow;
	try {

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sqlQuery));
		for (size_t i = 0; i < conditionValues.size(); ++i) {
			prep_stmt->setString(i + 1, conditionValues[i]);
		}
		// 执行查询
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());

		// 获取结果集的元数据
		int num_cols = res->getMetaData()->getColumnCount();

		// 如果结果集非空，取第一行数据
		if (res->next()) {
			for (int i = 1; i <= num_cols; ++i) {
				resultRow.push_back(res->getInt(i));
			}
		}


		// 重置智能指针，释放资源
		res.reset();
		prep_stmt.reset();

		// 返回第一行结果
		return resultRow;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << " Error executing query: " << queryName << std::endl;
		//throw;关闭
	}
}


void SelectTopList(const std::string& sqlQuery, std::vector<UINT>& List) {

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		int index = 0;
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sqlQuery));
		prep_stmt->clearParameters();
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		while (res->next())
		{
			List[index] = res->getInt("Uin");
			index++;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectTopList Error" << std::endl;
		////throw;关闭
	}
}

UINT SelectKartRefitItemInfo(UINT Uin, KartRefitItemInfo*& KartRefit)
{
	UINT count = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(
			"SELECT RefitCout,MaxFlags,WWeight,SpeedWeight,JetWeight,SJetWeight,AccuWeight,ShapeRefitCount,KartHeadRefitItemID,KartTailRefitItemID,KartFlankRefitItemID,KartTireRefitItemID,AntiCollisionWeight,SecondRefitCount,Speed2Weight,DriftVecWeight,AdditionalZSpeedWeight,ShapeSuitID,LegendSuitLevel,LegendSuitLevelChoice,ShapeLegendSuitID,KartID  FROM KartRefit WHERE Uin=?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		count = res->rowsCount();
		KartRefit = new KartRefitItemInfo[count];
		memset(KartRefit, 0, sizeof(KartRefitItemInfo) * count);
		for (size_t i = 0; i < count; i++)
		{
			if (res->next()) {
				KartRefit[i].RefitCout = res->getUInt("RefitCout");
				KartRefit[i].MaxFlags = res->getUInt("MaxFlags");
				KartRefit[i].MaxFlags = res->getUInt("WWeight");
				KartRefit[i].SpeedWeight = res->getUInt("SpeedWeight");
				KartRefit[i].JetWeight = res->getUInt("JetWeight");
				KartRefit[i].SJetWeight = res->getUInt("SJetWeight");
				KartRefit[i].AccuWeight = res->getUInt("AccuWeight");

				KartRefit[i].ShapeRefitCount = res->getUInt("ShapeRefitCount");
				KartRefit[i].KartHeadRefitItemID = res->getUInt("KartHeadRefitItemID");
				KartRefit[i].KartTailRefitItemID = res->getUInt("KartTailRefitItemID");
				KartRefit[i].KartFlankRefitItemID = res->getUInt("KartFlankRefitItemID");
				KartRefit[i].KartTireRefitItemID = res->getUInt("KartTireRefitItemID");
				KartRefit[i].AntiCollisionWeight = res->getUInt("AntiCollisionWeight");

				KartRefit[i].SecondRefitCount = res->getUInt("SecondRefitCount");
				KartRefit[i].Speed2Weight = res->getUInt("Speed2Weight");
				KartRefit[i].DriftVecWeight = res->getUInt("DriftVecWeight");
				KartRefit[i].AdditionalZSpeedWeight = res->getUInt("AdditionalZSpeedWeight");

				KartRefit[i].ShapeSuitID = res->getUInt("ShapeSuitID");
				KartRefit[i].LegendSuitLevel = res->getUInt("LegendSuitLevel");
				KartRefit[i].LegendSuitLevelChoice = res->getUInt("LegendSuitLevelChoice");
				KartRefit[i].ShapeLegendSuitID = res->getUInt("ShapeLegendSuitID");
				KartRefit[i].KartId = res->getUInt("KartID");


			}
		}
		res.reset();
		return count;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectKartRefitItemInfo Error" << std::endl;
		////throw;关闭
	}
}

UINT SelectKartRefitItemInfo(UINT Uin, std::vector<KartRefitItemInfo>& KartRefit)
{
	UINT count = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(
			"SELECT KartID,RefitCout,MaxFlags,WWeight,SpeedWeight,JetWeight,SJetWeight,AccuWeight,AntiCollisionWeight,SecondRefitCount,ShapeRefitCount,KartHeadRefitItemID,KartTailRefitItemID,KartFlankRefitItemID,KartTireRefitItemID,ShapeSuitID,Speed2Weight,DriftVecWeight,AdditionalZSpeedWeight,LegendSuitLevel,LegendSuitLevelChoice,ShapeLegendSuitID,SkinSuitID FROM KartRefit WHERE Uin = ? order by KartID DESC;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next())
		{
			KartRefitItemInfo KaIf;
			KaIf.RefitCout = res->getUInt("RefitCout");
			KaIf.MaxFlags = res->getUInt("MaxFlags");
			KaIf.WWeight = res->getUInt("WWeight");
			KaIf.SpeedWeight = res->getUInt("SpeedWeight");
			KaIf.JetWeight = res->getUInt("JetWeight");
			KaIf.SJetWeight = res->getUInt("SJetWeight");
			KaIf.AccuWeight = res->getUInt("AccuWeight");
			KaIf.ShapeRefitCount = res->getUInt("ShapeRefitCount");
			KaIf.KartHeadRefitItemID = res->getUInt("KartHeadRefitItemID");
			KaIf.KartTailRefitItemID = res->getUInt("KartTailRefitItemID");
			KaIf.KartFlankRefitItemID = res->getUInt("KartFlankRefitItemID");
			KaIf.KartTireRefitItemID = res->getUInt("KartTireRefitItemID");
			KaIf.AntiCollisionWeight = res->getUInt("AntiCollisionWeight");
			KaIf.SecondRefitCount = res->getUInt("SecondRefitCount");
			KaIf.Speed2Weight = res->getUInt("Speed2Weight");
			KaIf.DriftVecWeight = res->getUInt("DriftVecWeight");
			KaIf.AdditionalZSpeedWeight = res->getUInt("AdditionalZSpeedWeight");
			KaIf.ShapeSuitID = res->getUInt("ShapeSuitID");
			KaIf.LegendSuitLevel = res->getUInt("LegendSuitLevel");
			KaIf.LegendSuitLevelChoice = res->getUInt("LegendSuitLevelChoice");
			KaIf.ShapeLegendSuitID = res->getUInt("ShapeLegendSuitID");
			KaIf.KartId = res->getUInt("KartID");
			KaIf.SkinSuitID = res->getUInt("SkinSuitID");
			KartRefit.push_back(KaIf);
		}
		res.reset();
		return count;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectKartRefitItemInfo Error" << std::endl;
		////throw;关闭
	}
}


void SelectOneKartRefitItemInfo(UINT Uin, UINT KartID, KartRefitItemInfo*& KartRefit)
{
	UINT count = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{

		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("select * from KartRefit where Uin=? and KartID=? limit 0,1;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, KartID);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		KartRefit = new KartRefitItemInfo[1];
		memset(KartRefit, 0, sizeof(KartRefitItemInfo) * 1);

		if (res->next()) {
			KartRefit[0].RefitCout = res->getUInt("RefitCout");
			KartRefit[0].MaxFlags = res->getUInt("MaxFlags");
			KartRefit[0].WWeight = res->getUInt("WWeight");
			KartRefit[0].SpeedWeight = res->getUInt("SpeedWeight");
			KartRefit[0].JetWeight = res->getUInt("JetWeight");
			KartRefit[0].SJetWeight = res->getUInt("SJetWeight");
			KartRefit[0].AccuWeight = res->getUInt("AccuWeight");

			KartRefit[0].ShapeRefitCount = res->getUInt("ShapeRefitCount");
			KartRefit[0].KartHeadRefitItemID = res->getUInt("KartHeadRefitItemID");
			KartRefit[0].KartTailRefitItemID = res->getUInt("KartTailRefitItemID");
			KartRefit[0].KartFlankRefitItemID = res->getUInt("KartFlankRefitItemID");
			KartRefit[0].KartTireRefitItemID = res->getUInt("KartTireRefitItemID");
			KartRefit[0].AntiCollisionWeight = res->getUInt("AntiCollisionWeight");

			KartRefit[0].SecondRefitCount = res->getUInt("SecondRefitCount");
			KartRefit[0].Speed2Weight = res->getUInt("Speed2Weight");
			KartRefit[0].DriftVecWeight = res->getUInt("DriftVecWeight");
			KartRefit[0].AdditionalZSpeedWeight = res->getUInt("AdditionalZSpeedWeight");

			KartRefit[0].ShapeSuitID = res->getUInt("ShapeSuitID");
			KartRefit[0].LegendSuitLevel = res->getUInt("LegendSuitLevel");
			KartRefit[0].LegendSuitLevelChoice = res->getUInt("LegendSuitLevelChoice");
			KartRefit[0].ShapeLegendSuitID = res->getUInt("ShapeLegendSuitID");
			KartRefit[0].KartId = res->getUInt("KartID");
			KartRefit[0].SkinSuitID = res->getUInt("SkinSuitID");
		}

		res.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectOneKartRefitItemInfo Error" << std::endl;
		////throw;关闭
	}
}

void UpdateOneKartRefitItemInfo(UINT Uin, UINT KartID, KartRefitItemInfo*& KartRefit)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		if (KartRefit) {
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("UPDATE KartRefit SET RefitCout=RefitCout+1, SJetWeight=?,AccuWeight=?,JetWeight=?,WWeight=?,SpeedWeight=?,AntiCollisionWeight=?  WHERE Uin=? AND KartID=?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, KartRefit->SJetWeight);
			prep_stmt->setUInt(2, KartRefit->AccuWeight);
			prep_stmt->setUInt(3, KartRefit->JetWeight);
			prep_stmt->setUInt(4, KartRefit->WWeight);
			prep_stmt->setUInt(5, KartRefit->SpeedWeight);
			prep_stmt->setUInt(6, KartRefit->AntiCollisionWeight);
			prep_stmt->setUInt(7, Uin);
			prep_stmt->setUInt(8, KartID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
			delete[] KartRefit;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateOneKartRefitItemInfo Error" << std::endl;
		////throw;关闭
	}
}


void InsertBattleRecord(PlayerRaceInfo* ArrPlayerRaceInfo, UINT Mapid, UINT GameMode)
{
	if (version < 1.1) return;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		conn->setAutoCommit(false);
		UINT recordid = 0;
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO BattleRecord(mapid,datetime,gamemode) VALUES (?,?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Mapid);
		prep_stmt->setInt(2, (DWORD)time(nullptr));
		prep_stmt->setInt(3, GameMode);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

		{
			//获取最新ID号 同步获取 
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT recordid FROM BattleRecord ORDER BY recordid DESC LIMIT 1;"));
			prep_stmt->clearParameters();
			// 执行查询
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			// 处理结果
			while (res->next()) {
				recordid = res->getInt("recordid");
			}
			prep_stmt.reset();
			res.reset();
		}

		for (int i = 0; i < 6; i++)
		{
			if (ArrPlayerRaceInfo[i].Uin > 10000 && ArrPlayerRaceInfo[i].Uin != -1) {
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO BattleDetails(recordid,uin,mapid,ranking,gamemode,kartid,time,speed,collision,nitrogen,drift,RankGradeLevel) VALUES (?,?,?,?,?,?,?,?,?,?,?,?);"));
				prep_stmt->clearParameters();
				prep_stmt->setInt(1, recordid);
				prep_stmt->setUInt(2, ArrPlayerRaceInfo[i].Uin);
				prep_stmt->setInt(3, Mapid);
				prep_stmt->setInt(4, ArrPlayerRaceInfo[i].FinishTime == -1 ? -1 : i + 1);
				prep_stmt->setInt(5, GameMode);
				prep_stmt->setInt(6, ArrPlayerRaceInfo[i].kartid);
				prep_stmt->setInt(7, ArrPlayerRaceInfo[i].FinishTime);
				prep_stmt->setInt(8, ArrPlayerRaceInfo[i].HighestSpeed);
				prep_stmt->setInt(9, ArrPlayerRaceInfo[i].CollisionCount);
				prep_stmt->setInt(10, ArrPlayerRaceInfo[i].N2OGatherCount);
				prep_stmt->setInt(11, ArrPlayerRaceInfo[i].DriftCount);
				prep_stmt->setInt(12, ArrPlayerRaceInfo[i].RankGradeLevel);
				prep_stmt->executeUpdate();
				prep_stmt.reset();
			}
		}
		conn->commit();
		conn->setAutoCommit(true);

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "InsertBattleRecord Error" << std::endl;
		////throw;关闭
	}
}

void UpdateAppellationType(UINT Uin, int type)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("update BaseInfo set AppellationType = ? where Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, type);
		prep_stmt->setUInt(2, Uin);
		prep_stmt->executeUpdate();
		prep_stmt.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateAppellationType Error" << std::endl;
		////throw;关闭
	}
}


int SelectAppellationType(UINT Uin)
{
	int AppellationType = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Select AppellationType From  BaseInfo  where Uin = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setInt(1, Uin);

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			AppellationType = res->getInt("AppellationType");
		}
		res.reset();

		return AppellationType;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectAppellationType Error" << std::endl;
		////throw;关闭
	}


}


void SelectGmTime(int Time)
{
	int AppellationType = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT Uin FROM baseinfo WHERE IFNULL(gm, 0) > 1 And IFNULL(admin, 0)= 0 And DATE_ADD(gmtime, INTERVAL ? DAY) < NOW();"));
		prep_stmt->clearParameters();

		prep_stmt->setInt(1, Time);

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			UINT Uin = res->getUInt("Uin");
			UserDB_ban(Uin);
			std::shared_ptr<ClientNode> ClientTemp = GetClient(Uin);
			if (ClientTemp) {
				NotifyKickPlayer(ClientTemp, "Gm时间到期，你已经被封号！", 3);
				memcpy(ClientTemp->Key, "", 16);
				ClientTemp->IsLogin = FALSE;
				ClientTemp->Server->Disconnect(ClientTemp->ConnID);
				ClientTemp->deleteflag = 1;
				//ClientTemp = NULL;
			}
		}
		res.reset();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectGmTime Error" << std::endl;
		////throw;关闭
	}


}



int SelectChallengeMap(UINT Mapid, UINT RankID, std::vector<ChallengeMap>& ChallengeMaps)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	ChallengeMaps.clear();
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM mapchallengerank WHERE Mapid =? and rankid = ?;"));
		prep_stmt->clearParameters();

		prep_stmt->setUInt(1, Mapid);
		prep_stmt->setUInt(2, RankID);

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			ChallengeMap CM;
			CM.MapID = res->getInt("Mapid");
			CM.RankID = res->getInt("RankID");
			CM.Uin = res->getInt("Uin");
			CM.Name = UTF2GB(res->getString("NickName").c_str());

			//CM.Name = res->getString("NickName").c_str();
			CM.GuildID = res->getInt("GuildID");
			CM.GuildName = UTF2GB(res->getString("GuildName").c_str());
			
			//CM.GuildName = res->getString("GuildName");
			CM.UseTime = res->getInt("UseTime");
			CM.KartID = res->getInt("KartID");
			CM.FileMd5 = res->getString("FileMd5");
			CM.UUid = res->getString("UUid");
			CM.LastUUid = res->getString("UUid");
			CM.KeepCnt = res->getInt("KeepCnt");
			CM.ChallengeCnt = res->getInt("ChallengeCnt");
			ChallengeMaps.push_back(CM);
		}
		res.reset();
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectChallengeMap Error" << std::endl;
		////throw;关闭
	}


}

int SelectChallengeMap(std::vector<ChallengeMap>& ChallengeMaps)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	ChallengeMaps.clear();
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM mapchallengerank WHERE rankid = 1 Limit 10;"));
		prep_stmt->clearParameters();
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next()) {
			ChallengeMap CM;
			CM.MapID = res->getInt("Mapid");
			CM.RankID = res->getInt("RankID");
			CM.Uin = res->getUInt("Uin");
			CM.Name = UTF2GB(res->getString("NickName").c_str());
			
			//CM.Name = res->getString("NickName").c_str();
			CM.GuildID = res->getInt("GuildID");
			CM.GuildName = UTF2GB(res->getString("GuildName").c_str());
			
			//CM.GuildName = res->getString("GuildName");
			CM.UseTime = res->getInt("UseTime");
			CM.KartID = res->getInt("KartID");
			CM.FileMd5 = res->getString("FileMd5");
			CM.UUid = res->getString("UUid");
			CM.LastUUid = res->getString("UUid");
			ChallengeMaps.push_back(CM);
		}
		res.reset();
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectChallengeMap2 Error" << std::endl;
		////throw;关闭
	}


}

int SelectChallengeMap(std::shared_ptr<ClientNode> Client, std::vector<ChallengeMap>& ChallengeMaps)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	ChallengeMaps.clear();
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("(SELECT * FROM mapchallengerank WHERE  GuildID = ? AND uin = ? Order by Rankid asc) UNION ALL (SELECT * FROM mapchallengerank WHERE GuildID = ? AND uin <> ? Order by Rankid asc) LIMIT 1;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Client->GuildID);
		prep_stmt->setUInt(2, Client->Uin);
		prep_stmt->setUInt(3, Client->GuildID);
		prep_stmt->setUInt(4, Client->Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			ChallengeMap CM;
			CM.MapID = res->getInt("Mapid");
			CM.RankID = res->getInt("RankID");
			CM.Uin = res->getUInt("Uin");
			CM.Name = UTF2GB(res->getString("NickName").c_str());
			

			//CM.Name = res->getString("NickName").c_str();
			CM.GuildID = res->getInt("GuildID");
			CM.GuildName = UTF2GB(res->getString("GuildName").c_str());
			
			//CM.GuildName = res->getString("GuildName");
			CM.UseTime = res->getInt("UseTime");
			CM.KartID = res->getInt("KartID");
			CM.FileMd5 = res->getString("FileMd5");
			CM.UUid = res->getString("UUid");
			CM.LastUUid = res->getString("UUid");
			ChallengeMaps.push_back(CM);
		}
		res.reset();
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectChallengeMap3 Error" << std::endl;
		////throw;关闭
	}


}



int UpdateChallengeMapCNT(UINT Mapid, UINT RankID)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update mapchallengerank Set keepCnt = keepCnt + 1 WHERE Mapid =? and rankid = ?;"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, Mapid);
		prep_stmt->setUInt(2, RankID);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "UpdateChallengeMapCNT Error" << std::endl;
		////throw;关闭
	}


}



int SelectChallengeMapTime(std::shared_ptr<ClientNode> Client)
{
	if (Client->ChallengeMaps.size() != 1) return 0;
	int UserTime = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM mapchallengerank WHERE Mapid =? and rankid = ?;"));
		prep_stmt->clearParameters();

		prep_stmt->setUInt(1, Client->ChallengeMaps[0].MapID);
		prep_stmt->setUInt(2, Client->ChallengeMaps[0].RankID);

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			UserTime = res->getInt("UseTime");
		}
		res.reset();
		return UserTime;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectChallengeMapTime Error" << std::endl;
		////throw;关闭
	}


}

bool CompareChallengeMaps(const ChallengeMap& a, const ChallengeMap& b) {
	if (a.MapID == b.MapID) {
		return a.RankID < b.RankID; // 如果 MapID 相同，则比较 RankID
	}
	return a.MapID < b.MapID; // 否则比较 MapID
}

int SelectChallengeAllMap(std::vector<ChallengeMap>& ChallengeAllMaps, std::vector<ChallengeMapsInfo> ChallengeMaps)
{

	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	std::lock_guard<std::mutex> lg(ChallengeMapsMutex);
	ChallengeAllMaps.clear();
	try
	{
		for (size_t i = 0; i < ChallengeMaps.size(); i++)
		{
			if (ChallengeMaps[i].IsOpen == 0) continue;
			for (size_t j = 1; j <= 5; j++)
			{
				std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM mapchallengerank Where Mapid=? and RankID =?;"));
				prep_stmt->clearParameters();
				prep_stmt->setUInt(1, ChallengeMaps[i].MapID);
				prep_stmt->setUInt(2, j);
				std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
				prep_stmt.reset();
				ChallengeMap CM;
				if (res->next()) {
					CM.MapID = res->getUInt("Mapid");
					CM.RankID = res->getUInt("RankID");
					CM.Uin = res->getUInt("Uin");
					CM.Name = res->getString("NickName");
					CM.GuildID = res->getInt("GuildID");
					CM.GuildName = res->getString("GuildName");
					CM.UseTime = res->getInt("UseTime");
					CM.KartID = res->getInt("KartID");
					CM.FileMd5 = res->getString("FileMd5");
					CM.UUid = res->getString("UUid");
					CM.LastUUid = res->getString("UUid");
					CM.KeepCnt = res->getUInt("KeepCnt");
					CM.ChallengeCnt = res->getUInt("ChallengeCnt");
					CM.Status = 0;

				}
				else {
					CM.MapID = ChallengeMaps[i].MapID;
					CM.RankID = j;
					CM.Uin = 0;
					CM.Name = "";
					CM.GuildID = 0;
					CM.GuildName = "";
					CM.UseTime = 99999999;
					CM.KartID = 0;
					CM.FileMd5 = "";
					CM.UUid = "";
					CM.LastUUid = "";
					CM.KeepCnt = 0;
					CM.ChallengeCnt = 0;
					CM.Status = 0;
				}
				ChallengeAllMaps.push_back(CM);
				res.reset();
			}

		}
		// 排序
		std::sort(ChallengeAllMaps.begin(), ChallengeAllMaps.end(), CompareChallengeMaps);

		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectChallengeMap Error" << std::endl;
		////throw;关闭
	}


}


int InSertChallengeMap(std::shared_ptr<ClientNode> Client, ChallengeMap* CM, std::string FilePath)
{
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		conn->setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("delete from  mapchallengerank Where Mapid=? And Rankid =?;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, CM->MapID);
			prep_stmt->setUInt(2, CM->RankID);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}


		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO mapchallengerank (Mapid, rankid, uin, nickname, guildid, guildname, usetime,kartid,uuid,lastuuid,filemd5,file) VALUES(?, ?, ?,?,?,?, ?, ?,?,?,?,?);"));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, CM->MapID);
		prep_stmt->setUInt(2, CM->RankID);
		prep_stmt->setUInt(3, CM->Uin);
		char* NickName = G2U(CM->Name.c_str());
		char* GuildName = G2U(CM->GuildName.c_str());
		prep_stmt->setString(4, NickName);
		prep_stmt->setUInt(5, CM->GuildID);
		prep_stmt->setString(6, GuildName);
		prep_stmt->setUInt(7, CM->UseTime);
		prep_stmt->setUInt(8, CM->KartID);
		prep_stmt->setString(9, CM->UUid);
		prep_stmt->setString(10, CM->UUid);
		prep_stmt->setString(11, CM->FileMd5);

		std::ifstream file(FilePath, std::ios::binary);
		if (!file) {
			throw std::runtime_error("Could not open file: " + FilePath);
		}
		prep_stmt->setBlob(12, &file);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

		conn->setAutoCommit(true);
		conn->commit();
		if (NickName) {
			delete[] NickName;
			NickName = NULL;
		}
		if (GuildName) {
			delete[] GuildName;
			GuildName = NULL;
		}
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "InSertChallengeMap Error" << std::endl;
		////throw;关闭
	}


}



int SelectBaseInfoEffect(UINT Uin)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT SpeclEffect FROM BaseInfo WHERE Uin = ?;"));
		prep_stmt->clearParameters();

		prep_stmt->setUInt(1, Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			ret = res->getUInt("SpeclEffect");
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectBaseInfoEffect Error" << std::endl;
		////throw;关闭
	}
}


int SelectMyKartLibarayNum(UINT Uin, char* Mess)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::string sql = "SELECT count(*) FROM Item a,ItemallNew b WHERE a.itemid = b.id and b.mess = ? and a.Uin = ?";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();

		prep_stmt->setString(1, Mess);
		prep_stmt->setUInt(2, Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			ret = res->getUInt(1);
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectMyKartLibarayNum Error" << std::endl;
		////throw;关闭
	}
}

int SelectMyKartLibarayAllNum(char* Mess)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::string sql = "SELECT count(*) FROM ItemallNew b WHERE  b.mess = ?";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();

		prep_stmt->setString(1, Mess);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		if (res->next()) {
			ret = res->getUInt(1);
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectMyKartLibarayAllNum Error" << std::endl;
		////throw;关闭
	}
}


int Selectmechskinaward(UINT Uin, std::vector<MechTreasureInfo>& MechTreasureInfos)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT * FROM mechskinaward WHERE Uin = ?;"));
		prep_stmt->clearParameters();

		prep_stmt->setUInt(1, Uin);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next()) {
			MechTreasureInfo MI;
			MI.ItemId = res->getUInt("Itemid");
			MI.ItemNum = res->getUInt("itemNum");
			MI.getnum = res->getUInt("GetNum");
			MI.DecomposeNum = res->getUInt("DecomposeNum");
			MI.AvailPeriod = res->getInt("Available");
			MechTreasureInfos.push_back(MI);
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "Selectmechskinaward Error" << std::endl;
		////throw;关闭
	}


}

int SelectmechskinawardGetNum(UINT Uin, UINT Itemid, int Available)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT getNum FROM mechskinaward WHERE Uin = ? And Itemid =? And Available =?;"));
		prep_stmt->clearParameters();

		prep_stmt->setUInt(1, Uin);
		prep_stmt->setUInt(2, Itemid);
		prep_stmt->setInt(3, Available);
		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		prep_stmt.reset();
		while (res->next()) {
			ret = res->getInt("getNum");
		}
		res.reset();
		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "SelectmechskinawardGetNum Error" << std::endl;
		////throw;关闭
	}


}


int Insertmechskinaward(UINT Uin, MechTreasureInfo mechTreasureInfo)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		bool has = false;
		{
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("SELECT 1 FROM mechskinaward WHERE Uin = ? ANd itemid =? and Available =? ;"));
			prep_stmt->clearParameters();

			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, mechTreasureInfo.ItemId);
			prep_stmt->setInt(3, mechTreasureInfo.AvailPeriod);
			std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
			prep_stmt.reset();
			while (res->next()) {
				has = true;
			}
			res.reset();
		}

		if (has) {
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("Update mechskinaward Set itemNum = itemNum + ? Where  Uin = ? ANd itemid =? and Available =? ;"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, mechTreasureInfo.ItemNum);
			prep_stmt->setUInt(2, Uin);
			prep_stmt->setUInt(3, mechTreasureInfo.ItemId);
			prep_stmt->setInt(4, mechTreasureInfo.AvailPeriod);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}
		else {
			std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement("INSERT INTO mechskinaward (Uin, Itemid, itemNum,GetNum,DecomposeNum,Available,exchangePieceNum) VALUES(?, ?, ?,?, ?, ?,?);"));
			prep_stmt->clearParameters();
			prep_stmt->setUInt(1, Uin);
			prep_stmt->setUInt(2, mechTreasureInfo.ItemId);
			prep_stmt->setUInt(3, mechTreasureInfo.ItemNum);
			prep_stmt->setUInt(4, mechTreasureInfo.getnum);
			prep_stmt->setUInt(5, mechTreasureInfo.DecomposeNum);
			prep_stmt->setInt(6, mechTreasureInfo.AvailPeriod);
			prep_stmt->setInt(7, mechTreasureInfo.exchangeNeedNum);
			prep_stmt->executeUpdate();
			prep_stmt.reset();
		}


		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "Insertmechskinaward Error" << std::endl;
		////throw;关闭
	}


}

int Updatemechskinaward(UINT Uin, std::string Col, UINT Itemid, int itemnum, int Available)
{
	int ret = 0;
	DBConnection myDBConnection(player); auto conn = myDBConnection.getConnection(); if (!conn) return 0;
	try
	{
		std::string sql = "Update mechskinaward Set " + Col + " = " + Col + " + ?,itemNum = itemNum - ? WHere Uin = ? and itemid = ? and Available = ?";
		std::unique_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(sql));
		prep_stmt->clearParameters();
		prep_stmt->setUInt(1, itemnum);
		prep_stmt->setUInt(2, itemnum);
		prep_stmt->setUInt(3, Uin);
		prep_stmt->setUInt(4, Itemid);
		prep_stmt->setInt(5, Available);
		prep_stmt->executeUpdate();
		prep_stmt.reset();

		return ret;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "Updatemechskinaward Error" << std::endl;
		////throw;关闭
	}


}