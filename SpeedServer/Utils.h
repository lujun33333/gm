#pragma once
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include<stdio.h>
#include "malloc.h"
enum MsgType { Request = 0, Response = 1, Notify = 2, Ack = 3, Internal = 4 };
//逻辑 request-response 单通知发notify
#define FE_CLIENT	0
#define FE_PLAYER	1
#define FE_ROOM	2
#define FE_GAMELOGIC	3
#define FE_GAMESVRD	4
#define FE_DBSVRD	5
#define FE_LOGSVRD	6
#define FE_DIRSVRD	7
#define FE_SHOPSVRD	8
#define FE_TEAM	9
#define FE_CHATSVRD	10
#define FE_BORDERSVRD	11
#define FE_MATCHSVRD	12
#define FE_OUTDOORSVRD	13
#define FE_GUILDSVRD	18
#define FE_LUCKYMATCHROOM	38
#define FE_LOVEMATCHSVRD	59
#define FE_WIZARDDB	68
#define FE_LADDERMATCHSVRD	71
#define FE_WLGAMESVR	70
#define FE_PKLOGIC	92

#define chatpanel_friend 1
#define chatpanel_integrate 2
#define chatpanel_friendandintegrate 3
#define chatpanel_team 5
#define chatpanel_upwallandintegrate 7
#define chatpanel_wonderland 8
#define chatpanel_system 12

#define Talk_InGame 2
#define Talk_RoomBeginGameTimer 9
#define Talk_SNS 10
#define Talk_LoveMatchTeamMate 11
#define Talk_WonderLand 13
#define Talk_WonderLandGsvrdRoom 14
#define Talk_WonderLandGsvrdSqare 15
#define Talk_WonderLandFetion 16
#define Talk_CallGuildDonate 17
#define Talk_ChumCircle 18
#define Talk_ChatroomRequestPK 19
#define Talk_InMatchRoom 22


#define ID_IDENTIFY_EMPTY 0
#define ID_IDENTIFY_QQLEVEL1 1
#define ID_IDENTIFY_QQLEVEL2 2
#define ID_IDENTIFY_QQLEVEL3 4
#define ID_IDENTIFY_SPEEDMEMBER 8 //紫钻
#define ID_IDENTIFY_NETBARIP 16 //网吧IP
#define ID_IDENTIFY_PETMEMBER 32
#define ID_IDENTIFY_NETBARMATCHIP 64
#define ID_IDENTIFY_GOLDNETBARIP 128
#define ID_IDENTIFY_QQGAMEVIP 256
#define ID_IDENTIFY_HIDE_FRIEND_LOGIN_TIPS 512
#define ID_IDENTIFY_JUDGE 1024 //裁判团

#define ID_IDENTIFY_INSPECTION 4096
#define ID_IDENTIFY_PRIMARY 8192
#define ID_IDENTIFY_SENOIR 8192
#define ID_IDENTIFY_MASTER 32768
#define ID_IDENTIFY_FORBIDDEN 65536
#define ID_IDENTIFY_NOLOGINING 131072
#define ID_IDENTIFY_KING 262144 //国王
#define ID_IDENTIFY_INFANTE 524288 //亲王
#define ID_IDENTIFY_QQFLAG 1048576
#define ID_IDENTIFY_HAVEWORD 2097152
#define ID_IDENTIFY_FINISHQQFLAG 4194304
#define ID_IDENTIFY_INVISIBLEQQNUMFLAG 8388608 //隐藏QQ号
#define ID_IDENTIFY_SHOWACHIEVEMENTFLAG 16777216
#define ID_IDENTIFY_4KUBINETBARIP 33554432
#define ID_IDENTIFY_QQCLUB 67108864
#define ID_IDENTIFY_REFUSE_UNKNOWN_GAME_INVITE 134217728
#define ID_IDENTIFY_REFUSE_UNKNOWN_FRIEND_INVITE 268435456
#define ID_IDENTIFY_REFUSE_UNKNOWN_PRIVATE_CHAT 536870912
#define ID_IDENTIFY_HIDE_QQSPEED_BIRTHDAY 1073741824 //隐藏飞车生日



//#define ENMCommonBoxAwardType_JiaNianHua 0
#define ENMCommonBoxAwardType_PetPK_Jingji_Day 1
#define ENMCommonBoxAwardType_PetPK_Jingji_Week 2
#define ENMCommonBoxAwardType_PetPK_JingJi_ResultAward 3
#define ENMCommonBoxAwardType_CityStar 4
#define ENMCommonBoxAwardType_JiaNianHua_GuoQing 5
#define ENMCommonBoxAwardType_NationalDayCarnivalSignUp 6
#define ENMCommonBoxAwardType_NationalDayCarnivalWeekendSaturday 7
#define ENMCommonBoxAwardType_NationalDayCarnivalWeekendSunday 8
#define ENMCommonBoxAwardType_LuckyShop 9
#define ENMCommonBoxAwardType_ChristmasDaySocket 10
#define ENMCommonBoxAwardType_ChristmasDaySafeFruit 11
#define ENMCommonBoxAwardType_FoodFightingAward 12
#define ENMCommonBoxAwardType_MagicTreeAward 13
#define ENMCommonBoxAwardType_MayDayTreasureExchange 14
#define ENMCommonBoxAwardType_GuildOnlineRegisterAward 15
#define ENMCommonBoxAwardType_ZongxiangWangli 16
#define ENMCommonBoxAwardType_HyperSpaceJumbo 17
#define ENMCommonBoxAwardType_PointChallenge_InGameLuckyBox 18
#define ENMCommonBoxAwardType_PointChallenge_Score 19
#define ENMCommonBoxAwardType_OneYuanSpike 20
#define ENMCommonBoxAwardType_SpeedFastMedalExchange 21
#define ENMCommonBoxAwardType_SpeedFastMedalFinishRound 22
#define ENMCommonBoxAwardType_PointChallenge_Score_Week 23
#define ENMCommonBoxAwardType_Activity_Return 24
#define ENMCommonBoxAwardType_Valentine_Day 25
#define ENMCommonBoxAwardType_GlobalTravel_PointBox 26
#define ENMCommonBoxAwardType_GlobalTravel_Exchange 27
#define ENMCommonBoxAwardType_Arrest_MainBox 28
#define ENMCommonBoxAwardType_GoldCoinExchange 29
#define ENMCommonBoxAwardType_TwistMachineBox 30
#define ENMCommonBoxAwardType_TenYearsExchange 31
#define ENMCommonBoxAwardType_TenYearsMedalCollect 32
#define ENMCommonBoxAwardType_TenYearsCakeFeast 33
#define ENMCommonBoxAwardType_SSCSignAward 34
#define ENMCommonBoxAwardType_18SpringFestival_FuqiExchange 35
#define ENMCommonBoxAwardType_SpringFlower_BlessPointBox 36
#define ENMCommonBoxAwardType_SpringFlower_ExchangeBox 37
#define ENMCommonBoxAwardType_SpringNewYear_ExtractLuckBox 38
#define ENMCommonBoxAwardType_DreamKiteBox 39
#define ENMCommonBoxAwardType_DreamKiteFinalBox 40
#define ENMCommonBoxAwardType_DreamKiteExchangeSpring 41
#define ENMCommonBoxAwardType_DreamKiteExchangeFool 42
#define ENMCommonBoxAwardType_DreamKiteExchangeQingming 43
#define ENMCommonBoxAwardType_DreamKiteHightDoubleBox 44
#define ENMCommonBoxAwardType_Qijige_ExchangeBox 45
#define ENMCommonBoxAwardType_FastSearchTreasureExchange 46
#define ENMCommonBoxAwardType_FastSearchTreasureFinalAward 47
#define ENMCommonBoxAwardType_IndianaChanllageScoreAward 48
#define ENMCommonBoxAwardType_LoverMatchTopRaceWeeklyAward 49
#define ENMCommonBoxAwardType_QuanmingTreasureOne 50
#define ENMCommonBoxAwardType_QuanmingTreasureTwo 51
#define ENMCommonBoxAwardType_QuanmingTreasureThree 52
#define ENMCommonBoxAwardType_QuanmingTreasureFour 53
#define ENMCommonBoxAwardType_MakeChocolateMysteriousAward 54
#define ENMCommonBoxAwardType_MakeChocolateFinalAward 55
#define ENMCommonBoxAwardType_MakeChocolateExchange 56
#define ENMCommonBoxAwardType_CrazyChase_DailyAward 57
#define ENMCommonBoxAwardType_WorldCupExchange 58
#define ENMCommonBoxAwardType_ZongziTotalFeedAward 59
#define ENMCommonBoxAwardType_ZongziFinalMustAward 60
#define ENMCommonBoxAwardType_ZongziFinalRandomAward 61
#define ENMCommonBoxAwardType_Rongyaohuigui2_7daySingUp 62
#define ENMCommonBoxAwardType_Rongyaohuigui2_ExchangeCenter 63
#define ENMCommonBoxAwardType_SummerZhanxin_WhiteStar 64
#define ENMCommonBoxAwardType_SummerZhanxin_PurpleStar 65
#define ENMCommonBoxAwardType_SummerZhanxin_YellowStar 66
#define ENMCommonBoxAwardType_SummerZhanxin_Light 67
#define ENMCommonBoxAwardType_DesperateEscape_DailyRankAward 68
#define ENMCommonBoxAwardType_RankCoinShop 69
#define ENMCommonBoxAwardType_SuperSpokesPerson_Vote 70
#define ENMCommonBoxAwardType_SuperSpokesPerson_Popularity 71
#define ENMCommonBoxAwardType_DesperateEscape_DailyPartAward 72
#define ENMCommonBoxAwardType_SummerActQixi 73
#define ENMCommonBoxAwardType_SummerActKaixue 74
#define ENMCommonBoxAwardType_SummerActZhongQiu 75
#define ENMCommonBoxAwardType_SummerActFinalAward 76
#define ENMCommonBoxAwardType_SuperRace_DayAward_Join 77
#define ENMCommonBoxAwardType_SuperRace_DayAward_UseSkill 78
#define ENMCommonBoxAwardType_SuperRace_WeekAward_TeamWin 79
#define ENMCommonBoxAwardType_SuperRace_DayAward_TeamWin 80
#define ENMCommonBoxAwardType_SpeedMemory_Box_Normal 81
#define ENMCommonBoxAwardType_SpeedMemory_Box_Boss 82
#define ENMCommonBoxAwardType_HappyLuckyCoin_YellowBaoshiExchange 83
#define ENMCommonBoxAwardType_HappyLuckyCoin_RedBaoshiExchange 84
#define ENMCommonBoxAwardType_ShuangRenChe_DayAward_Join 85
#define ENMCommonBoxAwardType_ShuangRenChe_DayAward_TeamWin 86
#define ENMCommonBoxAwardType_ShuangRenChe_WeekAward_TeamWin 87
#define ENMCommonBoxAwardType_SuperTurkey_TurkeyExchange 88
#define ENMCommonBoxAwardType_SuperTurkey_YumaoExchagne 89
#define ENMCommonBoxAwardType_BattlePass_Normal 90
#define ENMCommonBoxAwardType_BattlePass_Diamon 91
#define ENMCommonBoxAwardType_BirthdayCake_Eleven 92
#define ENMCommonBoxAwardType_BirthdayCake 93
#define ENMCommonBoxAwardType_BattlePass_OverLevel 94
#define ENMCommonBoxAwardType_BirthdayCake_Medal 95
#define ENMCommonBoxAwardType_ElevenAniversary_Exchange 96
#define ENMCommonBoxAwardType_GoldenBell_Exchange 97
#define ENMCommonBoxAwardType_GoldenBell_TotalShake 98
#define ENMCommonBoxAwardType_TimerChallge2nd_Single 99
#define ENMCommonBoxAwardType_TimerChallge2nd_Team 100
#define ENMCommonBoxAwardType_SpringBlessing_Exchange 101
#define ENMCommonBoxAwardType_BaiLiaoYinChun_ExchangeBox 102
#define ENMCommonBoxAwardType_ShortDistanceCanYuBox 103
#define ENMCommonBoxAwardType_ShortDistanceTopNBox 104
#define ENMCommonBoxAwardType_FiveDaysExchangeCars_Exchange 105
#define ENMCommonBoxAwardType_GrowUp_Exchange 106
#define ENMCommonBoxAwardType_Summer2019_AvoidRepeat_Choujiang 107
#define ENMCommonBoxAwardType_SecretCompass_Exchange 108
#define ENMCommonBoxAwardType_PaiWeiFirstWinBox 109
#define ENMCommonBoxAwardType_KartRefit_WeekAward 110
#define ENMCommonBoxAwardType_DragonBoatRace 111
#define ENMCommonBoxAwardType_SpeedWorkShop_ChouJiang 112
#define ENMCommonBoxAwardType_SpeedWorkShop_Exchange 113
#define ENMCommonBoxAwardType_ProfessionLicense_BigLevel1 114
#define ENMCommonBoxAwardType_ProfessionLicense_BigLevel2 115
#define ENMCommonBoxAwardType_ProfessionLicense_BigLevel3 116
#define ENMCommonBoxAwardType_ProfessionLicense_BigLevel4 117
#define ENMCommonBoxAwardType_ProfessionLicense_BigLevel5 118
#define ENMCommonBoxAwardType_ProfessionLicense_BigLevel6 119
#define ENMCommonBoxAwardType_BattlePass_ExchangeStore 120
#define ENMCommonBoxAwardType_Luckyball_StarBox 121
#define ENMCommonBoxAwardType_Luckyball_ChoujiangBox 122
#define ENMCommonBoxAwardType_RabbitPurseMoon_Exchange 123
#define ENMCommonBoxAwardType_TimerChallge2nd_Level 124
#define ENMCommonBoxAwardType_CarTreasure_Exchange 125
#define ENMCommonBoxAwardType_StarWish_Wish 126
#define ENMCommonBoxAwardType_StarWish_NotPutSend 127
#define ENMCommonBoxAwardType_SpeedWishPool_SilverChouJiang 128
#define ENMCommonBoxAwardType_SpeedWishPool_GlodenChouJiang 129
#define ENMCommonBoxAwardType_SpeedWishPool_Exchange 130
#define ENMCommonBoxAwardType_StarWish_TmpBox 131
#define ENMCommonBoxAwardType_RallyElimination_Day1 132
#define ENMCommonBoxAwardType_RallyElimination_Day2 133
#define ENMCommonBoxAwardType_RallyElimination_Week 134
#define ENMCommonBoxAwardType_RallyElimination_Tacit 135
#define ENMCommonBoxAwardType_LuckyHouseDiamondBox 136
#define ENMCommonBoxAwardType_LuckyHouseExchangeBox 137
#define ENMCommonBoxAwardType_SpeedWishPool_GlodenChouJiang_PreAward 138
#define ENMCommonBoxAwardType_OldTime_UserSelect 139
#define ENMCommonBoxAwardType_OldTime_System 140
#define ENMCommonBoxAwardType_WinterMemoryDrawCard 141
#define ENMCommonBoxAwardType_WinterMemoryDrawAward 142
#define ENMCommonBoxAwardType_WinterMemoryFinalAward 143
#define ENMCommonBoxAwardType_Speed12thCake_CakeChouJiang 144
#define ENMCommonBoxAwardType_Speed12thCake_WishChouJiang 145
#define ENMCommonBoxAwardType_Speed12thCake_Exchange 146
#define ENMCommonBoxAwardType_SailCollect_CollectAward 147
#define ENMCommonBoxAwardType_SailCollect_DailyBox 148
#define ENMCommonBoxAwardType_MeiShi1 149
#define ENMCommonBoxAwardType_MeiShi2 150
#define ENMCommonBoxAwardType_SpeedClass_StudyAward 151
#define ENMCommonBoxAwardType_SpeedClass_MaxStudyAward 152
#define ENMCommonBoxAwardType_SpeedClass_Exchange 153
#define ENMCommonBoxAwardType_MonsterNianRaceCanyuBox 154
#define ENMCommonBoxAwardType_MonsterNianRaceTopNBox 155
#define ENMCommonBoxAwardType_MonsterNianRaceChampionBox 156
#define ENMCommonBoxAwardType_ShenChe_ChouJiang 157
#define ENMCommonBoxAwardType_ShenChe_Exchange 158
#define ENMCommonBoxAwardType_ShenChe_ChouJiang_UserType1 159
#define ENMCommonBoxAwardType_ShenChe_ChouJiang_UserType2 160
#define ENMCommonBoxAwardType_ShenChe_ChouJiang_UserType3 161
#define ENMCommonBoxAwardType_ShenChe_ChouJiang_UserType4 162
#define ENMCommonBoxAwardType_SpringTour_NormalAward 163
#define ENMCommonBoxAwardType_SpringTour_BestAward 164
#define ENMCommonBoxAwardType_SpringTour_DistAward 165
#define ENMCommonBoxAwardType_JiJia_JiChu 166
#define ENMCommonBoxAwardType_XinChun2021_XinXiang 170
#define ENMCommonBoxAwardType_JiJia_TmpBox 171
#define ENMCommonBoxAwardType_ShiXing_ChouJiang 172
#define ENMCommonBoxAwardType_ShiXing_Huiyi 173
#define ENMCommonBoxAwardType_ShiXing_Exchange 174
#define ENMCommonBoxAwardType_ShiXing_HuiYi_UnlockStatus 175
#define ENMCommonBoxAwardType_Zongzi_Grid 176
#define ENMCommonBoxAwardType_Zongzi_Triple 177
#define ENMCommonBoxAwardType_Zongzi_TotalTriple 178
#define ENMCommonBoxAwardType_Zongzi_Join 179
#define ENMCommonBoxAwardType_HideAndSeek_DailyBox 180
#define ENMCommonBoxAwardType_MapCollect 181
#define ENMCommonBoxAwardType_JiaNianHua 182
#define ENMCommonBoxAwardType_StarRound_ChouJiang 183
#define ENMCommonBoxAwardType_StarRound_NotPutSend 184
#define ENMCommonBoxAwardType_StarRound_Exchange 185
#define ENMCommonBoxAwardType_DaZao_Workshop 186
#define ENMCommonBoxAwardType_StarRound_TmpBox 187
#define ENMCommonBoxAwardType_CheWang_Cheer 188
#define ENMCommonBoxAwardType_CheWang_Exchange 189
#define ENMCommonBoxAwardType_CheWang_Guess 190
#define ENMCommonBoxAwardType_Prelude_NormalAward 191
#define ENMCommonBoxAwardType_Prelude_LuckyAward 192
#define ENMCommonBoxAwardType_Prelude_CollectAward 193
#define ENMCommonBoxAwardType_ConsumeScoreChouJiang 194
#define ENMCommonBoxAwardType_ConsumeScoreExchange 195
#define ENMCommonBoxAwardType_MapChallenge 196
#define ENMCommonBoxAwardType_Zhongqiu_Wish 197
#define ENMCommonBoxAwardType_Speedshop_Buy 198
#define ENMCommonBoxAwardType_Speedshop_Rebate 199
#define ENMCommonBoxAwardType_Speedshop_Exchange 200
#define ENMCommonBoxAwardType_ChristmasTree_ChouJiang 201
#define ENMCommonBoxAwardType_ChristmasTree_Normal 202
#define ENMCommonBoxAwardType_SpeedIceFestival_Exchange 203
#define ENMCommonBoxAwardType_13thBlessing_Wish 204
#define ENMCommonBoxAwardType_SpeedIceFestival_DailyAward 205
#define ENMCommonBoxAwardType_EscapeClause_DailyRankAward 206
#define ENMCommonBoxAwardType_EscapeClause_DailyPartAward 207
#define ENMCommonBoxAwardType_XinChun2021_Before 208
#define ENMCommonBoxAwardType_XinChun2021_Login 209
#define ENMCommonBoxAwardType_XinChun2021_YuanXiao 210
#define ENMCommonBoxAwardType_XinChun2021_Before_SumAward 211
#define ENMCommonBoxAwardType_2021_Bottle 212
#define ENMCommonBoxAwardType_2021_Bottle_SumAward 213
#define ENMCommonBoxAwardType_2021_PinTu 214
#define ENMCommonBoxAwardType_2021_PinTu_SumAward 215
#define ENMCommonBoxAwardType_XinChun2021_SumAward 216
#define ENMCommonBoxAwardType_XinChun2021_YuanXiao_ZhiZuoTangYuan 217
#define ENMCommonBoxAwardType_FoxSpiritMatchMakerExchange 218
#define ENMCommonBoxAwardType_FoxSpiritMatchMakerNormalAward 219
#define ENMCommonBoxAwardType_BurnUniverse_DailyAward 220
#define ENMCommonBoxAwardType_SixStarArray_SixStar 221
#define ENMCommonBoxAwardType_SixStarArray_StarArray 222
#define ENMCommonBoxAwardType_QuShuiLiuShang_ChouJiang 223
#define ENMCommonBoxAwardType_QuShuiLiuShang_SumAward 224
#define ENMCommonBoxAwardType_TangDouChe_DailyAwawrd 225
#define ENMCommonBoxAwardType_TangDouChe_ExchangeAwarwd 226
#define ENMCommonBoxAwardType_TangDouChe_ExclusiveEquipment 227
#define ENMCommonBoxAwardType_LimitRecommend_Data 228
#define ENMCommonBoxAwardType_CapsuleStationChouJiang 229
#define ENMCommonBoxAwardType_CapsuleStationLeiji 230
#define ENMCommonBoxAwardType_ShootHouseBox 231
#define ENMCommonBoxAwardType_RanQingZongXia_ChouJiang 232
#define ENMCommonBoxAwardType_RanQingZongXia_MileageAward 233
#define ENMCommonBoxAwardType_RanQingZongXia_AllAreaAward 234
#define ENMCommonBoxAwardType_TimeShuttle_Normal 235
#define ENMCommonBoxAwardType_TimeShuttle_ProcessAward 236
#define ENMCommonBoxAwardType_TimeShuttle_ExchangeAward 237
#define ENMCommonBoxAwardType_KittyBox_ExchangeAward 238
#define ENMCommonBoxAwardType_QiXi_Normal 239
#define ENMCommonBoxAwardType_SuperNova_Normal 240
#define ENMCommonBoxAwardType_MemoryRace_Daily 241
#define ENMCommonBoxAwardType_MaJiangFighting_Daily 242
#define ENMCommonBoxAwardType_HalloweenAdventureChouJiang 243
#define ENMCommonBoxAwardType_SanXingDuiPanda 244
#define ENMCommonBoxAwardType_RankedMatch_LittleHeap 245
#define ENMCommonBoxAwardType_HalloweenAdventure_Exchange 246
#define ENMCommonBoxAwardType_SANXINGDUI_GoldExchange 247
#define ENMCommonBoxAwardType_SANXINGDUI_BronzeExchange 248
#define ENMCommonBoxAwardType_NovemberIPDraw 249
#define ENMCommonBoxAwardType_Bak1 250
#define ENMCommonBoxAwardType_Bak2 251
#define ENMCommonBoxAwardType_Bak3 252
#define ENMCommonBoxAwardType_Bak4 253
#define ENMCommonBoxAwardType_Bak5 254
#define ENMCommonBoxAwardType_ChristmasWish 255
#define ENMCommonBoxAwardType_Beta74_ChallengeChouJiang 256
#define ENMCommonBoxAwardType_Beta74_ChallengeBox 257
#define ENMCommonBoxAwardType_TimerChallge2nd_Call 258
#define ENMCommonBoxAwardType_Beta74_WishAward 259
#define ENMCommonBoxAwardType_GoForward_Exchange 260
#define ENMCommonBoxAwardType_14thBirthday 261
#define ENMCommonBoxAwardType_Beta75_Snow_ChouJiang 262
#define ENMCommonBoxAwardType_Beta75_Snow_Exchange 263
#define ENMCommonBoxAwardType_LeiNuoChange_Exchange 264
#define ENMCommonBoxAwardType_Beta75_Monster_ChouJiang 265
#define ENMCommonBoxAwardType_Beta75_Monster_Exchange 266
#define ENMCommonBoxAwardType_Beta75_Monster_BoxAward 267
#define ENMCommonBoxAwardType_Beta75_Redpacket_ChouJiang 268
#define ENMCommonBoxAwardType_14thPhoto_Exchange 269
#define ENMCommonBoxAwardType_Beta75_Redpacket_Exchange 270
#define ENMCommonBoxAwardType_Beta75_RankedMatch_IceMake 271
#define ENMCommonBoxAwardType_Beta75_TigerPrayYear_Exchange 272
#define ENMCommonBoxAwardType_Beta75_TigerPrayYear_ChouJiangLeiJi 273
#define ENMCommonBoxAwardType_Beta76_Magic_Exchange 274
#define ENMCommonBoxAwardType_Beta76_Cards_Common_ChouJiang 275
#define ENMCommonBoxAwardType_Beta76_Cards_Special_ChouJiang 276
#define ENMCommonBoxAwardType_Beta76_Cards_Exchange 277
#define ENMCommonBoxAwardType_Beta76_VeggieDog_ChouJiang 278
#define ENMCommonBoxAwardType_Beta76_VeggieDog_Exchange 279
#define ENMCommonBoxAwardType_Beta76_Bounty_ChouJiang 280
#define ENMCommonBoxAwardType_BaoJuan_Exchange 281
#define ENMCommonBoxAwardType_Beta76_RankedMatch_CardRace 282
#define ENMCommonBoxAwardType_Beta77_Space_Travel 283
#define ENMCommonBoxAwardType_Beta77_Space_Travel_Exchange 284
#define ENMCommonBoxAwardType_Beta77_Space_Wish 285
#define ENMCommonBoxAwardType_Beta77_Space_Treasure 286
#define ENMCommonBoxAwardType_Beta77_SkyRace 287
#define ENMCommonBoxAwardType_Beta78_StarsFighting 288
#define ENMCommonBoxAwardType_Beta78_Allstar_Vote 289
#define ENMCommonBoxAwardType_Beta78_Allstar_Exchange 290
#define ENMCommonBoxAwardType_Beta78_MidAutumn 291
#define ENMCommonBoxAwardType_Beta79_River_Treasure_ChouJiang 296
#define ENMCommonBoxAwardType_Beta79_River_Treasure_Exchange 297
#define ENMCommonBoxAwardType_Max 297 + 1

#define EAIT_CAR_NEW_A 1539
#define EAIT_CAR_NEW_B 1540
#define EAIT_CAR_NEW_C 1541
#define EAIT_CAR_NEW_D 1542
#define EAIT_CAR_NEW_S 1543
#define EAIT_CAR_NEW_M 1544
#define EAIT_CAR_NEW_M0 1545
#define EAIT_CAR_NEW_M1 1546
#define EAIT_CAR_NEW_M2 1547
#define EAIT_CAR_NEW_M3 1548
#define EAIT_CAR_NEW_M4 1549
#define EAIT_CAR_NEW_L0 1550
#define EAIT_CAR_NEW_L1 1551
#define EAIT_CAR_NEW_L2 1552
#define EAIT_CAR_NEW_L3 1553
#define EAIT_CAR_NEW_L4 1554
#define EAIT_CAR_NEW_T0 1555
#define EAIT_CAR_NEW_T1 1556
#define EAIT_CAR_NEW_T2 1557
#define EAIT_CAR_NEW_T3 1558
#define EAIT_CAR_NEW_T4 1559
#define EAIT_WEAPON_G0 2050
#define EAIT_WEAPON_G1 2051
#define EAIT_WEAPON_G2 2052
#define EAIT_WEAPON_G3 2053

#pragma pack(push, 1)
struct CSHEAD
{
	unsigned int TotalLength;
	short Ver;
	short DialogID;
	int Seq;
	unsigned int Uin;
	char BodyFlag;
	unsigned char OptLength;
};

struct MSGHEAD
{
	unsigned short MsgID;
	short MsgType;
	int MsgSeq;
	char SrcFE;
	char DstFE;
	short SrcID;
	short DstID;
	unsigned short BodyLen;
};

struct TransferHead //UDP传输头
{
	//18字节
	SHORT TotalLength;
	UCHAR Ver;
	LONG Seq;
	SHORT DialogID;
	ULONG Uin;
	SHORT TransferCmd;
	SHORT CheckSum;
	UCHAR OptLength;
	//UCHAR Option[127];
};

struct UBeta87_ReqHeadAndHeadShokAvatarCfg
{
	USHORT Headlen;
	UINT Uin;
	UINT Time;
	UCHAR Version;
	UCHAR Compress;
	UCHAR OptLength;
	//Body
	USHORT Bodylen;
	USHORT MsgLen;
	UCHAR Response[20];
	UCHAR F61;
	UINT Parameter1;
	UINT Parameter2;
};
#pragma pack(pop)

extern std::mutex g_heap_mutex;

inline void* safe_malloc(size_t size) {
	std::lock_guard<std::mutex> lock(g_heap_mutex);
	return malloc(size);
}

inline void safe_free(void* ptr) {
	std::lock_guard<std::mutex> lock(g_heap_mutex);
	free(ptr);
}

template <typename T>
T Kget(const BYTE* b, size_t offset) {
	T value{};
	memcpy(&value, b + offset, sizeof(T));
	return value;
}
template <typename T>
T z_get(BYTE* b) {
	return *(T*)(b);
}
template <typename T>
void z_set(BYTE* b, T val) {
	*(T*)(b) = val;
}

inline BYTE Get8(BYTE* ptr)
{
	return *(BYTE*)ptr;
}
inline WORD Get16(BYTE* ptr)
{
	return ntohs(*(WORD*)ptr);
}
inline DWORD Get32(BYTE* ptr)
{
	return ntohl(*(DWORD*)ptr);
}

inline void Set8(BYTE* ptr, BYTE val)
{
	*(BYTE*)ptr = val;
}
inline void Set16(BYTE* ptr, WORD val)
{
	*(WORD*)ptr = htons(val);
}
inline void Set32(BYTE* ptr, DWORD val)
{
	*(DWORD*)ptr = htonl(val);
}
//读对应字节数据 并移动指针到下一个位置
inline BYTE Read8(BYTE*& ptr)
{
	return *(BYTE*)ptr++;
}
inline WORD Read16(BYTE*& ptr)
{
	return ntohs(*reinterpret_cast<WORD*&>(ptr)++);
}
inline DWORD Read32(BYTE*& ptr)
{
	return ntohl(*reinterpret_cast<DWORD*&>(ptr)++);
}

inline std::string ReadString(BYTE*& ptr, const int len)
{
    // 创建一个长度为 len 的字符串，并用 ptr 指向的数据初始化
    std::string Val(reinterpret_cast<const char*>(ptr), len);
    ptr += len; // 移动指针
    return Val;
}


inline void Write8(BYTE*& ptr, BYTE val)
{
	*(BYTE*)ptr++ = val;
}
inline void Write16(BYTE*& ptr, WORD val)
{
	*reinterpret_cast<WORD*&>(ptr)++ = htons(val);
}
inline void Write32(BYTE*& ptr, DWORD val)
{
	*reinterpret_cast<DWORD*&>(ptr)++ = htonl(val);
}

//写出普通doublevalue 写出F6开头的参数
inline void WriteValue(BYTE*& ptr, DWORD Value1, DWORD Value2)
{
	Write8(ptr, 246);
	Write32(ptr, Value1);
	Write32(ptr, Value2);
}
inline void WriteValue(BYTE*& ptr, double& intValue)
{
	Write8(ptr, 246);
	memcpy(ptr, &intValue, sizeof(double));
	ptr += sizeof(double);
}
inline void WriteValue(BYTE*& ptr, int intValue)
{
	double Value = intValue;//当前底板
	Write8(ptr, 246);
	memcpy(ptr, &Value, sizeof(double));
	ptr += sizeof(double);
}
//属性编号 写出FB开头的参数
inline void WriteKeyIndex(BYTE*& ptr, DWORD val)
{
	Write8(ptr, 251);//FB
	Write8(ptr, val);
}


//读取64位double参数 主要针对F6开头的参数
inline double ReadValue(BYTE*& ptr)
{
	double retValue1 = Kget<double>(ptr, 0);
	ptr += 9;
	return retValue1;
}

inline double ReadValue8(BYTE*& ptr)
{
	double retValue1 = Kget<double>(ptr, 0);
	ptr += 8;
	return retValue1;
}

//写出标题值 写出FA开头的参数
inline void WriteText(BYTE*& ptr)
{
	Write8(ptr, 250);
	Write8(ptr, 0);//len
}
inline void WriteText(BYTE*& ptr, const char* Reason)
{
	DWORD len = strlen(Reason);
	Write8(ptr, 250);
	Write8(ptr, len);//len
	memcpy(ptr, Reason, len);
	ptr += len;
}
inline void WriteText(BYTE*& ptr, const char* Reason, DWORD len)
{
	Write8(ptr, 250);
	Write8(ptr, len);//len
	memcpy(ptr, Reason, len);
	ptr += len;
}


//标题值一起写出 FB+F6
inline void WriteKeyValue(BYTE*& ptr, DWORD val, double& Value) {
	WriteKeyIndex(ptr, val);
	WriteValue(ptr, Value);
}
inline void WriteKeyValue(BYTE*& ptr, DWORD val, int Value) {
	WriteKeyIndex(ptr, val);
	WriteValue(ptr, Value);
}
inline void WriteKeyText(BYTE*& ptr, DWORD val, const char* Reason) {
	WriteKeyIndex(ptr, val);
	DWORD len = strlen(Reason);
	WriteText(ptr, Reason, len);
}

//动态长度 start offlent 长度偏移某些特殊情况下需要
inline BYTE* WriteLenValueStart(BYTE*& ptr)
{
	Write8(ptr, 253);//FD	
	BYTE* ptrTemp = ptr;
	Write32(ptr, 0);//Infolen
	return ptrTemp;
}
inline void WriteLenValueEnd(BYTE*& ParentByte, BYTE*& ThisByte, BYTE*& lenByte)
{
	DWORD len = ThisByte - ParentByte;
	Set32(lenByte, len);
	ParentByte += len;
}
inline void WriteLenValueEnd(BYTE*& ParentByte, BYTE*& ThisByte, BYTE*& lenByte, INT offLen)
{
	DWORD len = ThisByte - ParentByte;
	Set32(lenByte, len + offLen);
	ParentByte += len;
}
inline void WriteLenValue(BYTE*& ptr, BYTE val)
{
	Write8(ptr, 253);//FD
	Write32(ptr, val);
}

inline void WriteTextValue(BYTE*& ptr, const char* Reason, DWORD len, double& Value) {
	WriteText(ptr, Reason, len);
	WriteValue(ptr, Value);
}
inline void WriteTextValue(BYTE*& ptr, const char* Reason, double& Value) {
	DWORD len = strlen(Reason);
	WriteText(ptr, Reason, len);
	WriteValue(ptr, Value);
}
inline void WriteTextValue(BYTE*& ptr, const char* Reason, DWORD len, DWORD Value) {
	WriteText(ptr, Reason, len);
	WriteValue(ptr, Value);
}
inline void WriteTextValue(BYTE*& ptr, const char* Reason, DWORD Value) {
	DWORD len = strlen(Reason);
	WriteText(ptr, Reason, len);
	WriteValue(ptr, Value);
}
inline void WriteTextTwo(BYTE*& ptr, const char* Reason, const char* Reason1) {
	DWORD len = strlen(Reason);
	WriteText(ptr, Reason, len);
	len = strlen(Reason1);
	WriteText(ptr, Reason1, len);
}



inline BYTE* WriteBodyHead(BYTE*& ptr, double& Value) {
	BYTE* pData = ptr;
	Write16(pData, 0); //MsgLen
	memset(pData, 0, 20);
	memcpy(pData, "DispatchCoResponse", 18);
	pData += 20;
	WriteValue(pData, Value);
	WriteValue(pData, 0);
	return pData;
}
inline void WriteStringNew(BYTE*& ptr, const char* val, int len)
{
	memset(ptr, 0, len);
	memcpy(ptr, val, len);
	ptr += len;
}

inline BYTE* WriteBodyHead(BYTE*& ptr) {
	BYTE* pData = ptr;
	Write16(pData, 0); //MsgLen
	WriteStringNew(pData, "OnNofityActivityCostNumNotEnough", 34);
	return pData;
}

inline BYTE* WriteBodyHeadAb(BYTE*& ptr, double& Value) {
	BYTE* pData = ptr;
	Write16(pData, 0); //MsgLen
	memset(pData, 0, 20);
	memcpy(pData, "DispatchCoResponse", 18);
	pData += 20;
	WriteValue(pData, Value);
	return pData;
}


inline BYTE* WriteClietVerifyHead(BYTE*& ptr, double& Value) {
	BYTE* pBody = ptr;
	Write16(pBody, 0); //len
	Write16(pBody, 31); //MsgLen
	memcpy(pBody, "OnNotifyServerVerify", 21);
	pBody += 22;


	WriteValue(pBody, Value);

	Write16(pBody, 0);
	return pBody;
}
inline void WriteClietVerifyHeadEnd(BYTE*& ParentByte, BYTE*& ThisByte)
{
	DWORD len = ThisByte - ParentByte;
	Set16(ParentByte, (WORD)len);
	ParentByte += len;
}
inline void WriteBodyHeadEnd(BYTE*& ParentByte, BYTE*& ThisByte)
{
	DWORD len = ThisByte - ParentByte;
	Set16(ParentByte, (WORD)len);
	ParentByte += len;
	Write16(ParentByte, 0);
}
inline void WriteF9(BYTE*& ptr)
{
	Write8(ptr, 249);
}inline void WriteF8(BYTE*& ptr)
{
	Write8(ptr, 248);
}



inline void WriteString(BYTE*& ptr, const char* string)
{
	int strsize = strlen(string);
	//printf("%s\n", string);
	for (int i = 0; i < strsize; i++)
	{
		*(BYTE*)ptr++ = *(string + i);
	}
;
}

inline void WriteString_MC(BYTE*& ptr, const char* val)
{
	int len = strlen(val);
	Write8(ptr, len);
	memcpy(ptr, val, len);
	ptr += len;
}

inline void WriteString16(BYTE*& ptr, const char* val)
{
	int len = strlen(val);
	Write16(ptr, len);
	memcpy(ptr, val, len);
	ptr += len;
}

inline void WriteString32(BYTE*& ptr, const char* val)
{
	int len = strlen(val);
	Write32(ptr, len);
	memcpy(ptr, val, len);
	ptr += len;
}

inline void WriteString_MC(BYTE*& ptr, const char* val, bool IsWrite8)
{
	int len = strlen(val);
	if (IsWrite8)
	{
		Write8(ptr, len);
	}
	memcpy(ptr, val, len);
	ptr += len;
}
inline void WriteString(BYTE*& ptr, const char* val, int len)
{
	Write8(ptr, len);
	memcpy(ptr, val, len);
	ptr += len;
}



inline void WriteUn(BYTE*& ptr, DWORD val)
{
	*reinterpret_cast<DWORD*&>(ptr)++ = htonl(val);
}

inline void WriteHex(BYTE*& ptr, const char* szHex)
{
	if (!szHex || !ptr)
		return; // 返回，不做任何操作

	size_t len = strlen(szHex);

	// 确保长度是偶数
	if (len % 2 != 0)
		return; // 返回，不做任何操作

	for (size_t i = 0; i < len; i += 2)
	{
		// 将每一对十六进制字符转换为一个字节
		unsigned int byte;
		sscanf_s(szHex + i, "%2X", &byte);
		*ptr++ = static_cast<BYTE>(byte); // 将转换后的字节写入指针并自增
	}
}

inline void Write8Hex(BYTE*& ptr, const char* szHex)
{
	if (!szHex || !ptr)
		return; // 返回，不做任何操作

	size_t len = strlen(szHex);

	// 确保长度是偶数
	if (len % 2 != 0)
		return; // 返回，不做任何操作

	Write8(ptr, len/2);
	for (size_t i = 0; i < len; i += 2)
	{
		// 将每一对十六进制字符转换为一个字节
		unsigned int byte;
		sscanf_s(szHex + i, "%2x", &byte);
		*ptr++ = static_cast<BYTE>(byte); // 将转换后的字节写入指针并自增
	}
}


inline char* EByteToHexUA2(const unsigned char* vByte, const int vLen)
{
	//把字节码转为十六进制码，一个字节两个十六进制，内部为字符串分配空间
	if (!vByte)
		return nullptr;
	int iLen = vLen;
	if (iLen)
	{
		char* tmp = new char[iLen * 2 + 1]; // 一个字节两个十六进制码，最后要多一个'/0'

		int tmp2;
		for (int i = 0; i < iLen; i++)
		{
			tmp2 = (int)(vByte[i]) / 16;
			tmp[i * 2] = (char)(tmp2 + ((tmp2 > 9) ? 'A' - 10 : '0'));
			tmp2 = (int)(vByte[i]) % 16;
			tmp[i * 2 + 1] = (char)(tmp2 + ((tmp2 > 9) ? 'A' - 10 : '0'));
		}
		tmp[iLen * 2] = '\0';//字符串0结尾
		return tmp;
	}
	return nullptr;
}

inline unsigned char* __stdcall EHexToByte(const char* szHex)
{
	// 把十六进制字符串，转为字节码，每两个十六进制字符作为一个字节
	if (!szHex)
		return nullptr;

	int iLen = strlen(szHex);

	if (iLen <= 0 || 0 != iLen % 2)
		return nullptr;

	unsigned char* pbBuf = new unsigned char[iLen / 2]; // 数据缓冲区

	int tmp1, tmp2;
	for (int i = 0; i < iLen / 2; i++)
	{
		tmp1 = (int)szHex[i * 2] - (((int)szHex[i * 2] >= 'A') ? 'A' - 10 : '0');

		if (tmp1 >= 16)
			return nullptr;

		tmp2 = (int)szHex[i * 2 + 1] - (((int)szHex[i * 2 + 1] >= 'A') ? 'A' - 10 : '0');

		if (tmp2 >= 16)
			return nullptr;

		pbBuf[i] = (tmp1 * 16 + tmp2);
	}

	return pbBuf;
}