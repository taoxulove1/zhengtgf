# 协议文档 · 征途 3.1.46.0（自动生成）

> 由 `tools/protocol_doc.py` 从 `src/server/base/Command.h` 生成，**勿手改**。
> 统计：**47 个命令域 / 559 条命令 / 3693 个字段**

## 0. 报文封装

| 层 | 值 | 依据 |
|---|---|---|
| 头长度 | 6 字节 | `stNullUserCmd` |
| 字段 | `BYTE byCmd` + `BYTE byParam` + `DWORD dwTimestamp` | 同上 |
| opcode | `(byCmd << 8) | byParam` | 559 条全部可解析 |
| 加密 | 裸 DES-ECB，8 字节一块，无 IV/填充，尾部余数明文 | `CDes.cpp:47` |
| 长度 | 无长度前缀，按 opcode 定长 | `msgid.csv.length_rule` |

## 1. 域索引

| 域 | 名称 | 中文 | 条数 | 基数 |
|---|---|---|---|---|
| `0x01` | `LOGON_USERCMD` | 登陆指令 | 10 | 0x01xx |
| `0x02` | `TIME_USERCMD` | 时间指令 | 4 | 0x02xx |
| `0x03` | `DATA_USERCMD` | — | 10 | 0x03xx |
| `0x04` | `PROPERTY_USERCMD` | 用户属性指令 | 91 | 0x04xx |
| `0x05` | `MAPSCREEN_USERCMD` | 地图数据指令 | 32 | 0x05xx |
| `0x06` | `MOVE_USERCMD` | 用户移动指令 | 9 | 0x06xx |
| `0x0C` | `RELIVE_USERCMD` | 复活指令 | 3 | 0x0Cxx |
| `0x0E` | `CHAT_USERCMD` | 用户聊天指令 | 26 | 0x0Exx |
| `0x11` | `TRADE_USERCMD` | 交易指令 | 37 | 0x11xx |
| `0x12` | `MAGIC_USERCMD` | 魔法指令 | 15 | 0x12xx |
| `0x15` | `UNION_USERCMD` | 帮会指令 | 21 | 0x15xx |
| `0x16` | `COUNTRY_USERCMD` | 国家指令 | 78 | 0x16xx |
| `0x18` | `SELECT_USERCMD` | 选择指令 | 8 | 0x18xx |
| `0x19` | `RELATION_USERCMD` | 社会关系指令 | 6 | 0x19xx |
| `0x1A` | `SCHOOL_USERCMD` | 师徒门派关系指令 | 15 | 0x1Axx |
| `0x1B` | `SEPT_USERCMD` | 家族指令 | 21 | 0x1Bxx |
| `0x1C` | `DARE_USERCMD` | — | 1 | 0x1Cxx |
| `0x1D` | `PET_USERCMD` | 宠物指令 | 15 | 0x1Dxx |
| `0x25` | `CARTOON_USERCMD` | — | 20 | 0x25xx |
| `0x26` | `STOCK_SCENE_USERCMD` | 定义股票命令 | 1 | 0x26xx |
| `0x27` | `STOCK_BILL_USERCMD` | 定义股票命令 | 28 | 0x27xx |
| `0x28` | `VOTE_USERCMD` | 投票指令 | 3 | 0x28xx |
| `0x29` | `ARMY_USERCMD` | 军队指令 | 14 | 0x29xx |
| `0x2A` | `GEM_USERCMD` | 护宝指令 | 2 | 0x2Axx |
| `0x2F` | `RECOMMEND_USERCMD` | 定义推荐人系统命令 | 5 | 0x2Fxx |
| `0x30` | `SAFETY_USERCMD` | 定义财产保护系统指令 | 5 | 0x30xx |
| `0x33` | `SAFETY_COWBOX` | 暂定宝盒相关指令 | 3 | 0x33xx |
| `0x34` | `TURN_USERCMD` | 暂定转生相关指令 | 2 | 0x34xx |
| `0x35` | `HORSETRAINING_USERCMD` | 训马指令 | 2 | 0x35xx |
| `0x36` | `LUA_USERCMD` | LUA脚本指令 | 1 | 0x36xx |
| `0x38` | `FUJIAN_USERCMD` | 这里每个指令 都有一个public stFujianUserCmd 你可以理解为 是 stFujianUserCmd 里面的子命令 stFujianUserCmd是啥 是 54 | 6 | 0x38xx |
| `0x39` | `LIANZHAN_USERCMD` | 连斩指令 By:醉梦  2024/03/03 | 1 | 0x39xx |
| `0x3A` | `JINGCAI_USERCMD` | 功勋竞猜相关指令 By醉梦 2024/03/12 | 5 | 0x3Axx |
| `0x3B` | `XINFUBEN_USERCMD` | 功勋竞猜相关指令 | 4 | 0x3Bxx |
| `0x3C` | `ERNV_USERCMD` | 儿女指令 By:醉梦  2024/10/17 | 2 | 0x3Cxx |
| `0x3D` | `TIANXIA_USERCMD` | 天下第一相关指令 By醉梦 2024/04/27 | 5 | 0x3Dxx |
| `0x3E` | `BIKE_USERCMD` | 自行车相关指令 By醉梦 2024/11/05 | 6 | 0x3Exx |
| `0x3F` | `BIESHU_USERCMD` | 云天别墅相关命令 | 7 | 0x3Fxx |
| `0x40` | `FENGYIN_USERCMD` | 等级封印相关命令 By醉梦 2024/12/08 | 2 | 0x40xx |
| `0x41` | `HUISHOU_USERCMD` | 道具回收命令 By醉梦 2024/12/24 | 2 | 0x41xx |
| `0x42` | `ZHUANHUAN_USERCMD` | 装备转换相关命令 By醉梦 2024/12/24 | 2 | 0x42xx |
| `0x43` | `XIULIAN_USERCMD` | 能力修炼相关命令 By醉梦 2024/12/25 | 5 | 0x43xx |
| `0x45` | `ZHANCHE_USERCMD` | 战车命令 By醉梦 2025/01/04 | 11 | 0x45xx |
| `0x46` | `JIAZUBOSS_USERCMD` | 家族BOSS相关指令 By醉梦 2025/01/06 | 5 | 0x46xx |
| `0x47` | `ZUOQI_USERCMD` | 坐骑图鉴相关指令 By醉梦 2025/01/09 | 5 | 0x47xx |
| `0x48` | `MIGONG_USERCMD` | 迷宫相关指令 By醉梦 2025/01/26 | 1 | 0x48xx |
| `0x49` | `BOSSINFO_USERCMD` | BOSS信息指令集 | 2 | 0x49xx |

## 2. 命令明细（逐域）

### 0x01 `LOGON_USERCMD` · 登陆指令（10 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0101` | `stUserVerifyVerCmd` | 18 | 6 | `USER_VERIFY_VER_PARA` | 312 |
| `0x0102` | `stUserRequestLoginCmd` | 40 | 11 | `USER_REQUEST_LOGIN_PARA` | 325 |
| `0x0103` | `stServerReturnLoginFailedCmd` | 15 | 6 | `SERVER_RETURN_LOGIN_FAILED` | 365 |
| `0x0104` | `stServerReturnLoginSuccessCmd` | 28 | 12 | `SERVER_RETURN_LOGIN_OK` | 376 |
| `0x0105` | `stPasswdLogonUserCmd` | 24 | 9 | `PASSWD_LOGON_USERCMD_PARA` | 401 |
| `0x0107` | `stAccountLogonUserCmd` | 16 | 7 | `ACCOUNT_LOGON_USERCMD_PARA` | 416 |
| `0x0109` | `stPasswordLogonUserCmd` | 17 | 8 | `PASSWORD_LOGON_USERCMD_PARA` | 429 |
| `0x010A` | `stBackSelectUserCmd` | 14 | 5 | `BACKSELECT_USERCMD_PARA` | 442 |
| `0x010B` | `stJpegPassportUserCmd` | 17 | 7 | `JPEG_PASSPORT_USERCMD_PARA` | 452 |
| `0x010C` | `stCountryInfoUserCmd` | 16 | 6 | `SERVER_RETURN_COUNTRY_INFO` | 478 |

### 0x02 `TIME_USERCMD` · 时间指令（4 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0201` | `stGameTimeTimerUserCmd` | 22 | 6 | `GAMETIME_TIMER_USERCMD_PARA` | 507 |
| `0x0202` | `stRequestUserGameTimeTimerUserCmd` | 14 | 5 | `REQUESTUSERGAMETIME_TIMER_USERCMD_PARA` | 520 |
| `0x0203` | `stUserGameTimeTimerUserCmd` | 26 | 7 | `USERGAMETIME_TIMER_USERCMD_PARA` | 531 |
| `0x0204` | `stPingTimeTimerUserCmd` | 14 | 5 | `PING_TIMER_USERCMD_PARA` | 544 |

### 0x03 `DATA_USERCMD` · —（10 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0301` | `stMainUserDataUserCmd` | 14 | 5 | `MAIN_USER_DATA_USERCMD_PARA` | 1403 |
| `0x0302` | `stDecRemainderDataUserCmd` | 15 | 6 | `DECREMAINDER_DATA_USERCMD_PARA` | 1421 |
| `0x0303` | `stSetHPAndMPDataUserCmd` | 14 | 5 | `SETHPANDMP_DATA_USERCMD_PARA` | 1434 |
| `0x031C` | `stMapScreenSizeDataUserCmd` | 45 | 17 | `MAPSCREENSIZE_DATA_USERCMD_PARA` | 1451 |
| `0x031D` | `stLoadMapOKDataUserCmd` | 14 | 5 | `LOADMAPOK_DATA_USERCMD_PARA` | 1476 |
| `0x031E` | `stLevelDegreeDataUserCmd` | 16 | 6 | `LEVELDEGREE_DATA_USERCMD_PARA` | 1485 |
| `0x0333` | `stStampDataUserCmd` | 19 | 7 | `STAMP_DATA_USERCMD_PARA` | 1495 |
| `0x0334` | `stStampCheckUserCmd` | 18 | 6 | `STAMP_CHECK_USERCMD_PARA` | 1511 |
| `0x0335` | `stMergeVersionCheckUserCmd` | 18 | 6 | `MERGE_VERSION_CHECK_USERCMD_PARA` | 1524 |
| `0x03FF` | `stEndOfInitDataDataUserCmd` | 14 | 5 | `ENDOFINITDATA_DATA_USERCMD_PARA` | 1538 |

### 0x04 `PROPERTY_USERCMD` · 用户属性指令（91 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0401` | `stAddObjectPropertyUserCmd` | 15 | 6 | `ADDUSEROBJECT_PROPERTY_USERCMD_PARAMETER` | 3029 |
| `0x0403` | `stSwapObjectPropertyUserCmd` | 18 | 6 | `SWAPUSEROBJECT_PROPERTY_USERCMD_PARAMETER` | 3050 |
| `0x0406` | `stRefCountObjectPropertyUserCmd` | 22 | 7 | `REFCOUNTOBJECT_PROPERTY_USERCMD_PARAMETER` | 3085 |
| `0x0407` | `stUseObjectPropertyUserCmd` | 22 | 7 | `USEUSEROBJECT_PROPERTY_USERCMD_PARAMETER` | 3096 |
| `0x0408` | `stPickUpItemPropertyUserCmd` | 22 | 7 | `PICKUPITEM_PROPERTY_USERCMD_PARA` | 3107 |
| `0x0409` | `stFoundItemPropertyUserCmd` | 28 | 10 | `FOUNDITEM_PROPERTY_USERCMD_PARA` | 3121 |
| `0x040A` | `stUpgradeItemPropertyUserCmd` | 15 | 6 | `UPGRADEITEM_PROPERTY_USERCMD_PARA` | 3137 |
| `0x040B` | `stComposeItemPropertyUserCmd` | 16 | 7 | `COMPOSEITEM_PROPERTY_USERCMD_PARA` | 3146 |
| `0x040C` | `stAccekKeyPropertyUserCmd` | 17 | 7 | `ACCELKEY_PROPERTY_USERCMD_PARA` | 3177 |
| `0x040D` | `stAddUserSkillPropertyUserCmd` | 30 | 10 | `ADDUSERSKILL_PROPERTY_USERCMD_PARA` | 3188 |
| `0x040E` | `stUpgradeUserSkillPropertyUserCmd` | 20 | 7 | `UPGRADEUSERSKILL_PROPERTY_USERCMD_PARA` | 3204 |
| `0x0410` | `stSystemSettingsUserCmd` | 14 | 5 | `SYSTEMSETTINGS_PROPERTY_USERCMD_PARA` | 3307 |
| `0x0411` | `stDistributePointPropertyUserCmd` | 16 | 6 | `DISTRIBUTEUPOINT_PROPERTY_USERCMD_PARA` | 3326 |
| `0x0412` | `stAddSkillPointPropertyUserCmd` | 18 | 6 | `ADDSKILLPOINT_PROPERTY_USERCMD` | 3338 |
| `0x0413` | `stClearPointPropertyUserCmd` | 18 | 6 | `CLEARPOINT_PROPERTY_USERCMD_PARA` | 3349 |
| `0x0413` | `stMakeOddsPropertyUserCmd` | 18 | 6 | `MAKE_ODDS_PROPERTY_USERCMD_PARA` | 3371 |
| `0x0414` | `stMakeSelectPropertyUserCmd` | 18 | 6 | `MAKE_SELECT_PROPERTY_USERCMD_PARA` | 3382 |
| `0x0415` | `stComposeSoulStonePropertyUserCmd` | 15 | 6 | `COMPOSE_SOUL_STONE_PROPERTY_USERCMD_PARA` | 3393 |
| `0x0416` | `stEnchasePropertyUserCmd` | 15 | 6 | `ENCHASEL_PROPERTY_USERCMD_PARA` | 3403 |
| `0x0417` | `stHolePropertyUserCmd` | 24 | 9 | `HOLE_PROPERTY_USERCMD_PARA` | 3412 |
| `0x0418` | `stDecomposePropertyUserCmd` | 18 | 6 | `DECOMPOSE_PROPERTY_USERCMD_PARA` | 3440 |
| `0x0419` | `stBodyColorPropertyUserCmd` | 18 | 6 | `BODYCOLOR_PROPERTY_USERCMD_PARA` | 3449 |
| `0x041A` | `stHairPropertyUserCmd` | 19 | 7 | `HAIR_PROPERTY_USERCMD_PARA` | 3459 |
| `0x041B` | `stSelectPropertyUserCmd` | 24 | 9 | `SELECT_PROPERTY_USERCMD_PARA` | 3470 |
| `0x041D` | `stSelectMainUserPropertyUserCmd` | 19 | 7 | `SELECT_MAINUSER_PROPERTY_USERCMD_PARA` | 3483 |
| `0x041F` | `stDurabilityUserCmd` | 26 | 8 | `DURABIILTY_USERCMD_PARA` | 3533 |
| `0x0421` | `stClearSkillUserCmd` | 14 | 5 | `CLEARSKILL_USERCMD_PARA` | 3568 |
| `0x0422` | `stFacePropertyUserCmd` | 18 | 6 | `FACE_PROPERTY_USERCMD_PARA` | 3576 |
| `0x0423` | `stHonorToPkValuePropertyUserCmd` | 14 | 5 | `HONOR_TO_PKVALUE_PROPERTY_USERCMD_PARA` | 3585 |
| `0x0424` | `stScrollMapPropertyUserCmd` | 23 | 8 | `SCROLL_MAP_PROPERTY_USERCMD_PARA` | 3594 |
| `0x0425` | `stClearPointLimitPropertyUserCmd` | 23 | 7 | `CLEARPOINT_LIMIT_PROPERTY_USERCMD_PARA` | 3610 |
| `0x0426` | `stRequestIncMapPropertyUserCmd` | 14 | 5 | `REQUEST_INC_MAP_PROPERTY_USERCMD_PARA` | 3622 |
| `0x0427` | `stSelectReturnHorsePropertyUserCmd` | 14 | 5 | `SELECT_RETURN_HORSE_PROPERTY_USERCMD_PARA` | 3683 |
| `0x0428` | `stGoTrainPropertyUserCmd` | 18 | 6 | `GO_TRAIN_PROPERTY_USERCMD_PARA` | 3693 |
| `0x042D` | `stModifyPropertyUserCmd` | 25 | 10 | `MODIFY_PROPERTY_USERCMD_PARA` | 3852 |
| `0x042E` | `stMakezsPropertyUserCmd` | 25 | 10 | `MAKEZS_PROPERTY_USERCMD_PARA` | 3867 |
| `0x042F` | `stMakezqPropertyUserCmd` | 25 | 10 | `MAKEZQ_PROPERTY_USERCMD_PARA` | 3882 |
| `0x0430` | `stMakelvPropertyUserCmd` | 25 | 10 | `MAKELV_PROPERTY_USERCMD_PARA` | 3897 |
| `0x0432` | `stItemUseItemSuccessUserCmd` | 18 | 6 | `ITEN_USEITEM_SUCCESS_USERCMD` | 3925 |
| `0x0433` | `stGoFubenPropertyUserCmd` | 18 | 6 | `GO_FUBEN_PROPERTY_USERCMD_PARA` | 3937 |
| `0x0437` | `stItem6UseItemSuccessUserCmd` | 18 | 6 | `ITEN6_USEITEM_SUCCESS_USERCMD` | 3948 |
| `0x0438` | `stItem7UseItemSuccessUserCmd` | 18 | 6 | `ITEN7_USEITEM_SUCCESS_USERCMD` | 3960 |
| `0x043C` | `stItem11UseItemSuccessUserCmd` | 18 | 6 | `ITEN11_USEITEM_SUCCESS_USERCMD` | 3971 |
| `0x043D` | `stSelectFlowersPropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS_PROPERTY_USERCMD_PARA` | 3984 |
| `0x043E` | `stSelectFlowers1PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS1_PROPERTY_USERCMD_PARA` | 3996 |
| `0x043F` | `stSelectFlowers2PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS2_PROPERTY_USERCMD_PARA` | 4008 |
| `0x0440` | `stSelectFlowers3PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS3_PROPERTY_USERCMD_PARA` | 4020 |
| `0x0441` | `stSelectFlowers4PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS4_PROPERTY_USERCMD_PARA` | 4032 |
| `0x0442` | `stSelectFlowers5PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS5_PROPERTY_USERCMD_PARA` | 4044 |
| `0x0443` | `stSelectFlowers6PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS6_PROPERTY_USERCMD_PARA` | 4056 |
| `0x0444` | `stSelectFlowers7PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS7_PROPERTY_USERCMD_PARA` | 4068 |
| `0x0446` | `stSelectFlowers20PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS20_PROPERTY_USERCMD_PARA` | 4091 |
| `0x0447` | `stSelectFlowers21PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS21_PROPERTY_USERCMD_PARA` | 4103 |
| `0x0448` | `stSelectFlowers22PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS22_PROPERTY_USERCMD_PARA` | 4115 |
| `0x0449` | `stSelectFlowers23PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS23_PROPERTY_USERCMD_PARA` | 4127 |
| `0x044A` | `stSelectFlowers41PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS41_PROPERTY_USERCMD_PARA` | 4139 |
| `0x044B` | `stSelectFlowers51PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS51_PROPERTY_USERCMD_PARA` | 4151 |
| `0x044C` | `stSelectFlowers61PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS61_PROPERTY_USERCMD_PARA` | 4163 |
| `0x044D` | `stSelectFlowers71PropertyUserCmd` | 19 | 7 | `SELECT_FLOWERS71_PROPERTY_USERCMD_PARA` | 4175 |
| `0x044E` | `stScroll2MapPropertyUserCmd` | 31 | 10 | `SCROLL2_MAP_PROPERTY_USERCMD_PARA` | 4187 |
| `0x0452` | `stOneKeyEquipPropertyUserCmd` | 18 | 6 | `ONE_KEY_EQUIP_PROPERTY_USERCMD_PARA` | 4214 |
| `0x0453` | `stGetOneKeyEquipPropertyUserCmd` | 18 | 6 | `GET_ONE_KEY_EQUIP_PROPERTY_USERCMD_PARA` | 4228 |
| `0x0455` | `stGoHanbingPropertyUserCmd` | 18 | 6 | `GO_HANBING_PROPERTY_USERCMD_PARA` | 4251 |
| `0x0456` | `stGoAngelPropertyUserCmd` | 18 | 6 | `GO_ANGEL_PROPERTY_USERCMD_PARA` | 4262 |
| `0x0457` | `stGoChuanSongPropertyUserCmd` | 14 | 5 | `GO_CHUANSONG_PROPERTY_USERCMD_PARA` | 4273 |
| `0x0458` | `stGoChuanSong2PropertyUserCmd` | 14 | 5 | `GO_CHUANSONG2_PROPERTY_USERCMD_PARA` | 4284 |
| `0x0459` | `stGetChenghaoPropertyUserCmd` | 18 | 6 | `GET_CHENGHAO_PROPERTY_USERCMD_PARA` | 4329 |
| `0x045A` | `stSetChenghaoUserCmd` | 22 | 7 | `SETCHENGHAO_USERCMD` | 4357 |
| `0x045B` | `stSetNotChenghaoUserCmd` | 22 | 7 | `SETNOTCHENGHAO_USERCMD` | 4369 |
| `0x045C` | `stGetShizhuangPropertyUserCmd` | 18 | 6 | `GET_SHIZHUANG_PROPERTY_USERCMD_PARA` | 4382 |
| `0x045E` | `stSetShizhuangUserCmd` | 22 | 7 | `SETSHIZHUANG_USERCMD` | 4459 |
| `0x045F` | `stSetNotShizhuangUserCmd` | 22 | 7 | `SETNOTSHIZHUANG_USERCMD` | 4471 |
| `0x0460` | `stSetShizhuangJihuoUserCmd` | 22 | 7 | `SETSHIZHUANGJIHUO_USERCMD` | 4484 |
| `0x0461` | `stMeiRiRenWuChuanSongUserCmd` | 22 | 7 | `MEIRIRENWUCHUANSONG_USERCMD` | 4613 |
| `0x0462` | `stGoChuanSong3PropertyUserCmd` | 14 | 5 | `GO_CHUANSONG3_PROPERTY_USERCMD_PARA` | 4294 |
| `0x0463` | `stGoChuanSong4PropertyUserCmd` | 14 | 5 | `GO_CHUANSONG4_PROPERTY_USERCMD_PARA` | 4302 |
| `0x0465` | `stSetPifengUserCmd` | 22 | 7 | `SETPIFENG_USERCMD` | 4497 |
| `0x0466` | `stSetNotPifengUserCmd` | 22 | 7 | `SETNOTPIFENG_USERCMD` | 4509 |
| `0x0467` | `stSetPifengJihuoUserCmd` | 22 | 7 | `SETPIFENGJIHUO_USERCMD` | 4522 |
| `0x0468` | `stSetChibangUserCmd` | 22 | 7 | `SETCHIBANG_USERCMD` | 4535 |
| `0x0469` | `stSetNotChibangUserCmd` | 22 | 7 | `SETNOTCHIBANG_USERCMD` | 4547 |
| `0x046A` | `stSetChibangJihuoUserCmd` | 22 | 7 | `SETCHIBANGJIHUO_USERCMD` | 4560 |
| `0x046B` | `stGoChuanSong5PropertyUserCmd` | 14 | 5 | `GO_CHUANSONG5_PROPERTY_USERCMD_PARA` | 4311 |
| `0x046C` | `stGoChuanSong6PropertyUserCmd` | 14 | 5 | `GO_CHUANSONG6_PROPERTY_USERCMD_PARA` | 4319 |
| `0x046D` | `stSetZuoqiUserCmd` | 22 | 7 | `SETZUOQI_USERCMD` | 4574 |
| `0x046E` | `stSetNotZuoqiUserCmd` | 22 | 7 | `SETNOTZUOQI_USERCMD` | 4586 |
| `0x046F` | `stSetZuoqiJihuoUserCmd` | 22 | 7 | `SETZUOQIJIHUO_USERCMD` | 4599 |
| `0x0470` | `stSuiJiChuanSongUserCmd` | 14 | 5 | `SUIJICHUANSONG_USERCMD` | 4626 |
| `0x0472` | `stSetJiemianUserCmd` | 22 | 7 | `SETJIEMIAN_USERCMD` | 4635 |
| `0x0473` | `stSetNotJiemianUserCmd` | 22 | 7 | `SETNOTJIEMIAN_USERCMD` | 4647 |
| `0x0474` | `stSetJiemianJihuoUserCmd` | 22 | 7 | `SETJIEMIANJIHUO_USERCMD` | 4660 |

### 0x05 `MAPSCREEN_USERCMD` · 地图数据指令（32 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x050A` | `stAddUserMapScreenUserCmd` | 14 | 5 | `ADDUSER_MAPSCREEN_USERCMD_PARA` | 1564 |
| `0x050B` | `stRemoveUserMapScreenUserCmd` | 18 | 6 | `REMOVEUSER_MAPSCREEN_USERCMD_PARA` | 1574 |
| `0x050C` | `stRequestUserDataMapScreenUserCmd` | 20 | 7 | `REQUESTUSERDATA_MAPSCREEN_USERCMD_PARA` | 1585 |
| `0x050D` | `stRideMapScreenUserCmd` | 19 | 7 | `RIDE_MAPSCREEN_USERCMD_PARA` | 1597 |
| `0x050E` | `stRefreshStateMapScreenUserCmd` | 19 | 7 | `REFRESHSTATE_MAPSCREEN_USERCMD_PARA` | 1610 |
| `0x0514` | `stAddMapObjectMapScreenUserCmd` | 15 | 6 | `ADDMAPOBJECT_MAPSCREEN_USERCMD_PARA` | 1628 |
| `0x0515` | `stRemoveMapObjectMapScreenUserCmd` | 18 | 6 | `REMOVEMAPOBJECT_MAPSCREEN_USERCMD_PARA` | 1640 |
| `0x051E` | `stAddMapBuildingMapScreenUserCmd` | 14 | 5 | `ADDMAPBUILDING_MAPSCREEN_USERCMD_PARA` | 1650 |
| `0x051F` | `stRemoveMapBuildingMapScreenUserCmd` | 18 | 6 | `REMOVEMAPBUILDING_MAPSCREEN_USERCMD_PARA` | 1662 |
| `0x0520` | `stChangeFaceMapScreenUserCmd` | 22 | 7 | `CHANGEFACE_MAPSCREEN_USERCMD_PARA` | 1673 |
| `0x0532` | `stAddMapNpcMapScreenUserCmd` | 15 | 6 | `ADDMAPNPC_MAPSCREEN_USERCMD_PARA` | 1685 |
| `0x0533` | `stRemoveMapNpcMapScreenUserCmd` | 18 | 6 | `REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA` | 1696 |
| `0x0534` | `stRequestMapNpcDataMapScreenUserCmd` | 20 | 7 | `REQUESTMAPNPCDATA_MAPSCREEN_USERCMD_PARA` | 1706 |
| `0x0535` | `stNPCHPMapScreenUserCmd` | 26 | 8 | `NPCHP_MAPSCREEN_USERCMD_PARA` | 1718 |
| `0x0536` | `stAllMapScreenUserCmd` | 14 | 5 | `ALL_MAPSCREEN_USERCMD_PARA` | 1745 |
| `0x0537` | `stMapDataMapScreenUserCmd` | 14 | 5 | `MAPDATA_MAPSCREEN_USERCMD_PARA` | 1804 |
| `0x0538` | `stRTSelectedHpMpPropertyUserCmd` | 19 | 7 | `SELECTED_HPMP_PROPERTY_USERCMD_PARA` | 1822 |
| `0x053A` | `stClearObjectOwnerMapScreenUserCmd` | 18 | 6 | `CLEAR_OBJECTOWNER_MAPSCREEN_USERCMD_PARA` | 1839 |
| `0x053B` | `stAddMapNpcAndPosMapScreenUserCmd` | 14 | 5 | `ADDMAPNPC_AND_POS_MAPSCREEN_USERCMD_PARA` | 1848 |
| `0x053C` | `stAddUserAndPosMapScreenUserCmd` | 14 | 5 | `ADDUSER_AND_POS_MAPSCREEN_USERCMD_PARA` | 1859 |
| `0x053D` | `stBatchRemoveNpcMapScreenUserCmd` | 20 | 7 | `BATCHREMOVENPC_MAPSCREEN_USERCMD_PARA` | 1868 |
| `0x053D` | `stAddPetMapScreenUserCmd` | 15 | 6 | `ADD_PET_MAPSCREEN_USERCMD_PARA` | 2027 |
| `0x053E` | `stBatchRemoveUserMapScreenUserCmd` | 20 | 7 | `BATCHREMOVEUSER_MAPSCREEN_USERCMD_PARA` | 1879 |
| `0x053F` | `stSetStateMapScreenUserCmd` | 21 | 8 | `SETSTATE_MAPSCREEN_USERCMD_PARA` | 1889 |
| `0x0540` | `stClearStateMapScreenUserCmd` | 21 | 8 | `CLEARSTATE_MAPSCREEN_USERCMD_PARA` | 1900 |
| `0x0541` | `stAddMapNpcPetMapScreenUserCmd` | 14 | 5 | `ADDMAPNPCPET_MAPSCREEN_USERCMD_PARA` | 1911 |
| `0x0542` | `stAddMapPetMapScreenUserCmd` | 14 | 5 | `ADDMAPPET_MAPSCREEN_USERCMD_PARA` | 1922 |
| `0x0543` | `stGoodnessStateMapScreenUserCmd` | 22 | 7 | `GOODNESS_MAPSCREEN_USERCMD_PARA` | 1932 |
| `0x0544` | `stAddUserMapScreenStateUserCmd` | 14 | 5 | `ADDUSER_MAPSCREEN_STATE_USERCMD_PARA` | 1942 |
| `0x0545` | `stAddMapNpcMapScreenStateUserCmd` | 14 | 5 | `ADDMAPNPC_MAPSCREEN_STATE_USERCMD_PARA` | 1955 |
| `0x0548` | `stAddMapNpcPetMapScreenStateUserCmd` | 14 | 5 | `ADDMAPNPCPET_MAPSCREEN_STATE_USERCMD_PARA` | 1994 |
| `0x0549` | `stMapDataMapScreenStateUserCmd` | 14 | 5 | `MAPDATA_MAPSCREEN_STATE_USERCMD_PARA` | 2009 |

### 0x06 `MOVE_USERCMD` · 用户移动指令（9 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0602` | `stUserMoveMoveUserCmd` | 24 | 10 | `USERMOVE_MOVE_USERCMD_PARA` | 2501 |
| `0x0603` | `stUserJumptoUserCmd` | 18 | 7 | `USERJUMPTO_USERCMD_PARA` | 2518 |
| `0x060A` | `stUserInstantJumpMoveUserCmd` | 26 | 9 | `USERINSTANTJUMP_MOVE_USERCMD_PARA` | 2530 |
| `0x060C` | `stUserGotoMapMoveUserCmd` | 21 | 8 | `USERGOTOMAP_MOVE_USERCMD_PARA` | 2546 |
| `0x0614` | `stNpcMoveMoveUserCmd` | 24 | 10 | `NPCMOVE_MOVE_USERCMD_PARA` | 2559 |
| `0x061E` | `stRequestLandMassMoveUserCmd` | 14 | 5 | `REQUESTLANDMASS_MOVE_USERCMD_PARA` | 2575 |
| `0x061F` | `stResponseLandMassMoveUserCmd` | 18 | 6 | `RESPONSELANDMASS_MOVE_USERCMD_PARA` | 2593 |
| `0x0620` | `stLandMassJumpMoveUserCmd` | 14 | 5 | `LANDMASSJUMP_MOVE_USERCMD_PARA` | 2605 |
| `0x0621` | `stSitDownMoveUserCmd` | 14 | 5 | `SITDOWN_MOVE_USERCMD_PARA` | 2614 |

### 0x0C `RELIVE_USERCMD` · 复活指令（3 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0C01` | `stMainUserDeathReliveUserCmd` | 22 | 7 | `MAINUSERDEATH_RELIVE_USERCMD_PARA` | 2420 |
| `0x0C02` | `stMainUserReliveReliveUserCmd` | 18 | 6 | `MAINUSERRELIVE_RELIVE_USERCMD_PARA` | 2435 |
| `0x0C03` | `stOKReliveUserCmd` | 19 | 7 | `OK_RELIVE_USERCMD_PARA` | 2459 |

### 0x0E `CHAT_USERCMD` · 用户聊天指令（26 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x0E01` | `stChannelChatUserCmd` | 41 | 14 | `ALL_CHAT_USERCMD_PARAMETER` | 4814 |
| `0x0E01` | `stServerChannelChatUserCmd` | 39 | 13 | `ALL_CHAT_USERCMD_PARAMETER` | 4837 |
| `0x0E02` | `stRemoveTeamUserCmd` | 14 | 5 | `REMOVE_TEAM_USERCMD_PARA` | 4859 |
| `0x0E03` | `stAddTeamMemberUserCmd` | 18 | 6 | `ADD_TEAMMEMBER_USERCMD_PARA` | 4880 |
| `0x0E04` | `stRemoveTeamMemberUserCmd` | 22 | 7 | `REMOVE_TEAMMEMBER_USERCMD_PARA` | 4892 |
| `0x0E05` | `stRequestTeamUserCmd` | 22 | 7 | `REQUEST_TEAM_USERCMD_PARA` | 4914 |
| `0x0E06` | `stAnswerTeamUserCmd` | 27 | 9 | `ANSWER_TEAM_USERCMD_PARA` | 4926 |
| `0x0E07` | `stCreateChannelUserCmd` | 24 | 9 | `CREATE_CHANNEL_USERCMD_PARAMETER` | 4939 |
| `0x0E08` | `stInvite_ChannelUserCmd` | 23 | 8 | `INVITE_CHANNEL_USERCMD_PARAMETER` | 4953 |
| `0x0E09` | `stJoin_ChannelUserCmd` | 19 | 7 | `JOIN_CHANNEL_USERCMD_PARAMETER` | 4966 |
| `0x0E0A` | `stLeave_ChannelUserCmd` | 19 | 7 | `LEAVE_CHANNEL_USERCMD_PARAMETER` | 4978 |
| `0x0E0E` | `stTeamMemberOfflineUserCmd` | 18 | 6 | `TEAMMEMBER_OFFLINE_USERCMD_PARA` | 5043 |
| `0x0E0F` | `stAutoReplyUserCmd` | 15 | 6 | `AUTO_REPLY_USERCMD_PARA` | 5054 |
| `0x0E10` | `stExpModeTeamMemberUserCmd` | 15 | 6 | `EXPMODE_TEAMMEMBER_USERCMD_PARA` | 5076 |
| `0x0E11` | `stObjModeTeamMemberUserCmd` | 15 | 6 | `OBJMODE_TEAMMEMBER_USERCMD_PARA` | 5086 |
| `0x0E12` | `stTeamModeUserCmd` | 15 | 6 | `TEAM_MODE_USERCMD_PARA` | 5096 |
| `0x0E13` | `stFreshTeamMemberUserCmd` | 18 | 6 | `FRESH_TEAMMEMBER_USERCMD_PARA` | 5106 |
| `0x0E14` | `stDirtyUserUserCmd` | 15 | 6 | `DIRTY_USER_USERCMD_PARA` | 5133 |
| `0x0E15` | `stAnswerNameTeamUserCmd` | 21 | 9 | `ANSWER_NAME_TEAM_USERCMD_PARA` | 5143 |
| `0x0E16` | `stRequestNameTeamUserCmd` | 19 | 7 | `REQUEST_NAME_TEAM_USERCMD_PARA` | 5156 |
| `0x0E17` | `stRequestCountryHelpUserCmd` | 19 | 8 | `REQUEST_COUNTRY_HELP_USERCMD_PARA` | 5166 |
| `0x0E18` | `stKillForeignerUserCmd` | 16 | 7 | `KILL_FOREIGNER_USERCMD_PARA` | 5177 |
| `0x0E19` | `stRefreshBossUserCmd` | 21 | 9 | `REFRESH_BOSS_USERCMD_PARA` | 5189 |
| `0x0E1A` | `stKillBossUserCmd` | 24 | 10 | `KILL_BOSS_USERCMD_PARA` | 5205 |
| `0x0E1B` | `stFriendCongratulateUserCmd` | 27 | 9 | `FRIEND_CONGRATULATE_USERCMD_PARA` | 5221 |
| `0x0E1C` | `stFriendCongratulateInfoUserCmd` | 531 | 8 | `FRIEND_CONGRATULATEINFO_USERCMD_PARA` | 5234 |

### 0x11 `TRADE_USERCMD` · 交易指令（37 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1100` | `stRequestTradeUserCmd` | 22 | 7 | `REQUEST_TRADE_USERCMD_PARAMETER` | 5264 |
| `0x1101` | `stAnswerTradeUserCmd` | 23 | 8 | `ANSWER_TRADE_USERCMD_PARAMETER` | 5276 |
| `0x1102` | `stBeginTradeUserCmd` | 22 | 7 | `BEGIN_TRADE_USERCMD_PARAMETER` | 5290 |
| `0x1105` | `stCommitTradeUserCmd` | 18 | 6 | `COMMIT_TRADE_USERCMD_PARAMETER` | 5302 |
| `0x1106` | `stCancelTradeUserCmd` | 18 | 6 | `CANCEL_TRADE_USERCMD_PARAMETER` | 5313 |
| `0x1107` | `stFinishTradeUserCmd` | 14 | 5 | `FINISH_TRADE_USERCMD_PARAMETER` | 5324 |
| `0x1108` | `stAddObjectTradeUserCmd` | 20 | 8 | `ADD_OBJECT_TRADE_USERCMD_PARAMETER` | 5334 |
| `0x1109` | `stRemoveObjectTradeUserCmd` | 22 | 7 | `REMOVE_OBJECT_TRADE_USERCMD_PARAMETER` | 5349 |
| `0x110A` | `stVisitNpcTradeUserCmd` | 20 | 8 | `VISITNPC_TRADE_USERCMD_PARAMETER` | 5361 |
| `0x110B` | `stBuyObjectNpcTradeUserCmd` | 23 | 8 | `BUYOBJECT_NPCTRADE_USERCMD_PARAMETER` | 5376 |
| `0x110C` | `stSellObjectNpcTradeUserCmd` | 18 | 6 | `SELLOBJECT_NPCTRADE_USERCMD_PARAMETER` | 5393 |
| `0x110D` | `stRepairObjectNpcTradeUserCmd` | 22 | 7 | `REPAIROBJECT_NPCTRADE_USERCMD_PARAMETER` | 5404 |
| `0x110E` | `stSellHorseNpcTradeUserCmd` | 15 | 6 | `SELLHORSE_NPCTRADE_USERCMD_PARAMETER` | 5416 |
| `0x110F` | `stStoreInfoNpcTradeUserCmd` | 16 | 7 | `STORE_INFO_NPCTRADE_USERCMD_PARAMETER` | 5428 |
| `0x1110` | `stSellTradeUserCmd` | 14 | 5 | `START_SELL_USERCMD_PARAMETER` | 5441 |
| `0x1111` | `stFinishSellUserCmd` | 14 | 5 | `FINISH_SELL_USERCMD_PARAMETER` | 5451 |
| `0x1112` | `stAddObjectSellUserCmd` | 20 | 8 | `ADD_OBJECT_SELL_USERCMD_PARAMETER` | 5461 |
| `0x1113` | `stRemoveObjectSellUserCmd` | 18 | 6 | `REMOVE_OBJECT_SELL_USERCMD_PARAMETER` | 5475 |
| `0x1114` | `stRequestSellInfoUserCmd` | 18 | 6 | `REQUEST_SELL_INFO_USERCMD_PARAMETER` | 5486 |
| `0x1115` | `stRequestSellBuyUserCmd` | 22 | 7 | `REQUEST_SELL_BUY_USERCMD_PARAMETER` | 5497 |
| `0x1116` | `stUpdateStorePassUserCmd` | 30 | 7 | `UPDATE_STORE_PASS_USERCMD_PARAMETER` | 5509 |
| `0x1117` | `stCanSellTradeUserCmd` | 15 | 6 | `CAN_SELL_USERCMD_PARAMETER` | 5521 |
| `0x1118` | `stGoldGiveTradeUserCmd` | 15 | 6 | `GOLD_GIVE_USERCMD_PARAMETER` | 5541 |
| `0x111A` | `stRequestGoldGiveTradeUserCmd` | 14 | 5 | `REQUEST_GOLD_GIVE_USERCMD_PARAMETER` | 5561 |
| `0x111A` | `stRequestMatarialGiveTradeUserCmd` | 14 | 5 | `REQUEST_GOLD_GIVE_USERCMD_PARAMETER` | 5730 |
| `0x111B` | `stReturnGoldGiveTradeUserCmd` | 26 | 8 | `RETURN_GOLD_GIVE_USERCMD_PARAMETER` | 5570 |
| `0x111B` | `stReturnMatarialGiveTradeUserCmd` | 18 | 6 | `RETURN_GOLD_GIVE_USERCMD_PARAMETER` | 5739 |
| `0x111C` | `stAddObjectSellListUserCmd` | 22 | 9 | `ADD_OBJECT_SELL_LIST_USERCMD_PARAMETER` | 5582 |
| `0x111D` | `stRequestAddObjectSellUserCmd` | 26 | 10 | `REQUEST_ADD_OBJECT_SELL_USERCMD_PARAMETER` | 5600 |
| `0x111E` | `stBuyObjectSalaryTradeUserCmd` | 23 | 8 | `BUYOBJECT_SALARYTRADE_USERCMD_PARAMETER` | 5619 |
| `0x111E` | `stMatarialgiveTradeUserCmd` | 14 | 5 | `MATARIAL_GIVE_USERCMD_PARAMETER` | 5721 |
| `0x111F` | `stBuyObjectTicketTradeUserCmd` | 23 | 8 | `BUYOBJECT_TICKETTRADE_USERCMD_PARAMETER` | 5637 |
| `0x1120` | `stBuyObjectChongzhiTradeUserCmd` | 23 | 8 | `BUYOBJECT_CHONGZHITRADE_USERCMD_PARAMETER` | 5655 |
| `0x1121` | `stBuyObjectMallUserCmd` | 23 | 8 | `BUYOBJECT_MALL_USERCMD_PARAMETER` | 5672 |
| `0x1122` | `stFortGoldGiveTradeUserCmd` | 15 | 6 | `FORTGOLD_GIVE_USERCMD_PARAMETER` | 5688 |
| `0x1123` | `stRequestFortunGiveTradeUserCmd` | 14 | 5 | `REQUEST_FORTUN_GIVE_USERCMD_PARAMETER` | 5697 |
| `0x1124` | `stReturnFortunFundGiveTradeUserCmd` | 26 | 8 | `RETURN_FORTUNFUND_GIVE_USERCMD_PARAMETER` | 5706 |

### 0x12 `MAGIC_USERCMD` · 魔法指令（15 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1205` | `stAttackMagicUserCmd` | 35 | 14 | `MAGIC_USERCMD_PARA` | 2143 |
| `0x1206` | `stRTMagicUserCmd` | 35 | 14 | `RTMAGIC_USERCMD_PARA` | 2185 |
| `0x1207` | `stBackOffMagicUserCmd` | 28 | 10 | `BACKOFF_USERCMD_PARA` | 2252 |
| `0x1208` | `stObtainExpUserCmd` | 19 | 7 | `OBTAINEXP_USERCMD_PARA` | 2267 |
| `0x1209` | `stNpcDeathUserCmd` | 18 | 6 | `NPCDEATH_USERCMD_PARA` | 2281 |
| `0x120A` | `stLevelUpUserCmd` | 18 | 6 | `LEVELUP_USERCMD_PARA` | 2292 |
| `0x120B` | `stObjectHpMpPopUserCmd` | 23 | 8 | `OBJECT_HPMP_POP_PARA` | 2224 |
| `0x120C` | `stRTMagicPosUserCmd` | 15 | 6 | `RTMAGIC_POS_USERCMD_PARA` | 2240 |
| `0x1214` | `stPKModeUserCmd` | 15 | 6 | `PKMODE_USERCMD_PARA` | 2317 |
| `0x1216` | `stObjectExpUserCmd` | 22 | 7 | `OBJECT_EXP_USERCMD_PARA` | 2325 |
| `0x1217` | `stUnCombinUserCmd` | 14 | 5 | `UNCOMBIN_USERCMD_PARA` | 2345 |
| `0x1218` | `stFireWorkUserCmd` | 31 | 9 | `FIREWORK_USERCMD_PARA` | 2355 |
| `0x1219` | `stTimeSyncUserCmd` | 14 | 5 | `TIMESYNC_USERCMD_PARA` | 2369 |
| `0x121A` | `stSwitchCharacterObstacleUserCmd` | 14 | 5 | `SWITCH_CHARACTER_OBSTACLE_USERCMD_PARA` | 2380 |
| `0x121B` | `stRTOtherMagicUserCmd` | 24 | 9 | `RT_OTHER_MAGIC_USERCMD_PARA` | 2390 |

### 0x15 `UNION_USERCMD` · 帮会指令（21 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1501` | `stUnionStatusCheckCmd` | 14 | 5 | `UNION_STATUS_CHECK_PARA` | 5769 |
| `0x1502` | `stCreateUnionCmd` | 15 | 6 | `CREATE_UNION_PARA` | 5779 |
| `0x1503` | `stServerReturnUnionCheckCmd` | 14 | 5 | `SERVER_RETURN_UNION_CREATE_FAILED` | 5790 |
| `0x1505` | `stAllUnionMemberCmd` | 16 | 6 | `ALL_UNION_MEMBER_PARA` | 5811 |
| `0x1506` | `stReturnUnionMemberInfoCmd` | 16 | 7 | `RETURN_UNION_MEMBER_INFO_PARA` | 5837 |
| `0x1507` | `stAddMemberToUnionCmd` | 21 | 9 | `ADD_MEMBER_TO_UNION_PARA` | 5856 |
| `0x1508` | `stFireMemberFromUnionCmd` | 15 | 6 | `FIRE_MEMBER_FROM_UNION_PARA` | 5870 |
| `0x150B` | `stUnionBaseInfoCmd` | 289 | 12 | `UNION_BASE_INFO_PARA` | 5911 |
| `0x150C` | `stUnionMemberLeaveUnionCmd` | 14 | 5 | `UNIONMEMBER_LEAVE_UNION_PARA` | 5928 |
| `0x150D` | `stVoteListUnionCmd` | 20 | 8 | `VOTELIST_UNION_PARA` | 5945 |
| `0x150E` | `stVoteUnionCmd` | 16 | 7 | `VOTE_UNION_PARA` | 5959 |
| `0x150F` | `stNoteUnionCmd` | 269 | 6 | `NOTE_UNION_PARA` | 5971 |
| `0x1518` | `stListUnionCmd` | 19 | 7 | `LIST_UNION_PARA` | 6114 |
| `0x1519` | `stRequestJoinUnionCmd` | 16 | 7 | `REQUEST_JOIN_UNION_PARA` | 6127 |
| `0x151A` | `stRequestAbjurationCitynCmd` | 14 | 5 | `REQUEST_ABJURATION_CITY_PARA` | 6139 |
| `0x151B` | `stReqUnionNameCmd` | 18 | 6 | `REQ_UNION_NAME_PARA` | 6149 |
| `0x151C` | `stRtnUnionNameCmd` | 19 | 7 | `RTN_UNION_NAME_PARA` | 6162 |
| `0x151D` | `stReqUnionNameLlistCmd` | 19 | 7 | `REQ_UNION_NAME_LIST_PARA` | 6177 |
| `0x151E` | `stRtnUnionNameListCmd` | 20 | 8 | `RTN_UNION_NAME_LIST_PARA` | 6190 |
| `0x151F` | `stAppointCityCatcherCmd` | 15 | 6 | `APPOINT_CITY_CATCHER_PARA` | 6207 |
| `0x1520` | `stCancelCityCatcherCmd` | 14 | 5 | `CANCEL_CITY_CATCHER_PARA` | 6220 |

### 0x16 `COUNTRY_USERCMD` · 国家指令（78 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1601` | `stChangeCountryCmd` | 18 | 6 | `CHANGE_COUNTRY_PARA` | 6247 |
| `0x1602` | `stRequestDareCountryCmd` | 15 | 6 | `REQUEST_DARE_COUNTRY_PARA` | 6265 |
| `0x1603` | `stReturnDareCountryCmd` | 19 | 7 | `RETURN_DARE_COUNTRY_PARA` | 6283 |
| `0x1604` | `stTransDareCountryCmd` | 18 | 6 | `TRANS_DARE_COUNTRY_PARA` | 6299 |
| `0x1605` | `stDareCountryFormalCmd` | 18 | 6 | `DARE_COUNTRY_FORMAL_PARA` | 6312 |
| `0x1606` | `stRequestDareRecordCmd` | 15 | 6 | `REQUEST_DARE_RECORD_PARA` | 6331 |
| `0x1607` | `stReturnDareRecordResultCmd` | 18 | 6 | `RETURN_DARE_RECORD_RESULT_PARA` | 6353 |
| `0x1608` | `stReturnDareRecordStatCmd` | 18 | 6 | `RETURN_DARE_RECORD_STAT_PARA` | 6375 |
| `0x1609` | `stReturnDareRecordPlanCmd` | 18 | 6 | `RETURN_DARE_RECORD_PLAN_PARA` | 6396 |
| `0x160A` | `stCancelCountryCmd` | 14 | 5 | `CANCEL_COUNTRY_PARA` | 6410 |
| `0x160B` | `stApplyCountryCmd` | 18 | 6 | `APPLY_COUNTRY_PARA` | 6420 |
| `0x160C` | `stTaxCountryUserCmd` | 19 | 7 | `TAX_COUNTRY_PARA` | 6432 |
| `0x160D` | `stFiskCountryUserCmd` | 42 | 9 | `FISK_COUNTRY_PARA` | 6444 |
| `0x160E` | `stSetTaxCountryUserCmd` | 19 | 7 | `SETTAX_COUNTRY_PARA` | 6462 |
| `0x160F` | `stUpdateCountryStarCmd` | 18 | 6 | `UPDATE_COUNTRY_STAR` | 6480 |
| `0x1610` | `stContributeCountryMaterialCmd` | 18 | 6 | `CONTRIBUTE_COUNTRY_MATERIAL` | 6494 |
| `0x1611` | `stSetCountryDareUserCmd` | 20 | 8 | `SETCOUNTRY_DARE_PARA` | 6506 |
| `0x1612` | `stUpdateCountryKingUserCmd` | 16 | 7 | `UPDATE_COUNTRY_KING` | 6524 |
| `0x1613` | `stAnswerCountryDareUserCmd` | 15 | 6 | `ANSWER_COUNTRY_DARE_PARA` | 6544 |
| `0x1614` | `stSelectTransLevelUserCmd` | 18 | 6 | `SELECT_TRANS_LEVEL` | 6557 |
| `0x1615` | `stForbidTalkCountryUserCmd` | 15 | 6 | `FORBID_TALK_COUNTRY_PARA` | 6570 |
| `0x1616` | `stReqTechUserCmd` | 18 | 6 | `REQ_TECH_PARA` | 6589 |
| `0x1617` | `stRtnTechUserCmd` | 22 | 7 | `RTN_TECH_PARA` | 6611 |
| `0x1618` | `stReqWaitOfficialUserCmd` | 14 | 5 | `REQ_WAIT_OFFICIAL_PARA` | 6627 |
| `0x1619` | `stRtnWaitOfficialUserCmd` | 18 | 6 | `RTN_WAIT_OFFICIAL_PARA` | 6643 |
| `0x161A` | `stSetTechSearchUserCmd` | 22 | 7 | `SET_TECH_SEARCH_PARA` | 6657 |
| `0x161B` | `stUpTechDegreeUserCmd` | 18 | 6 | `UP_TECH_DEGREE_PARA` | 6672 |
| `0x161C` | `stConfirmSearcherUserCmd` | 19 | 7 | `CONFIRM_SEARCHER_PARA` | 6692 |
| `0x161D` | `stDisableLoginCountryCmd` | 14 | 5 | `DISENABLEL_OGIN_COUNTRY_PARA` | 6712 |
| `0x161E` | `stCancelTechSearchUserCmd` | 18 | 6 | `CANCEL_TECH_SEARCH_PARA` | 6723 |
| `0x161F` | `stAntiDareCountryFormalCmd` | 18 | 6 | `ANTI_DARE_COUNTRY_FORMAL_PARA` | 6736 |
| `0x1620` | `stCountryNoteCmd` | 269 | 6 | `COUNTRY_NOTE_PARA` | 6748 |
| `0x1621` | `stReqCountryNoteCmd` | 14 | 5 | `REQ_COUNTRY_NOTE_PARA` | 6760 |
| `0x1622` | `stKingPunishCountryCmd` | 19 | 7 | `KING_PUNISH_COUNTRY_PARA` | 6770 |
| `0x1623` | `stEmperorPunishCountryCmd` | 19 | 7 | `EMPEROR_PUNISH_COUNTRY_PARA` | 6782 |
| `0x1624` | `stReqGenCountryCmd` | 14 | 5 | `REQ_GEN_COUNTRY_PARA` | 6794 |
| `0x1625` | `stRetGenCountryCmd` | 26 | 8 | `RET_GEN_COUNTRY_PARA` | 6804 |
| `0x1626` | `stAppointDiplomatCmd` | 15 | 6 | `APPOINT_DIPLOMAT_PARA` | 6817 |
| `0x1627` | `stCancelDiplomatCmd` | 14 | 5 | `CANCEL_DIPLOMAT_PARA` | 6830 |
| `0x1628` | `stAppointCatcherCmd` | 15 | 6 | `APPOINT_CATCHER_PARA` | 6841 |
| `0x1629` | `stCancelCatcherCmd` | 15 | 6 | `CANCEL_CATCHER_PARA` | 6854 |
| `0x162A` | `stAppointCatchexCmd` | 15 | 6 | `APPOINT_CATCHEX_PARA` | 6867 |
| `0x162B` | `stCancelCatchexCmd` | 15 | 6 | `CANCEL_CATCHEX_PARA` | 6880 |
| `0x162C` | `stAppointCensorCmd` | 15 | 6 | `APPOINT_CENSOR_PARA` | 6893 |
| `0x162D` | `stCancelCensorCmd` | 15 | 6 | `CANCEL_CENSOR_PARA` | 6906 |
| `0x162E` | `stAppointCensosCmd` | 15 | 6 | `APPOINT_CENSOS_PARA` | 6919 |
| `0x162F` | `stCancelCensosCmd` | 15 | 6 | `CANCEL_CENSOS_PARA` | 6932 |
| `0x1630` | `stAppointYuanshuaiCmd` | 15 | 6 | `APPOINT_YUANSHUAI_PARA` | 6945 |
| `0x1631` | `stCancelYuanshuaiCmd` | 15 | 6 | `CANCEL_YUANSHUAI_PARA` | 6958 |
| `0x1632` | `stAppointZaixiangCmd` | 15 | 6 | `APPOINT_ZAIXIANG_PARA` | 6971 |
| `0x1633` | `stCancelZaixiangCmd` | 15 | 6 | `CANCEL_ZAIXIANG_PARA` | 6984 |
| `0x1634` | `stRequestCountryPowerCmd` | 14 | 5 | `REQUEST_COUNTRY_POWER_PARA` | 6997 |
| `0x1635` | `stReturnCountryPowerCmd` | 15 | 6 | `RETURN_COUNTRY_POWER_PARA` | 7007 |
| `0x1636` | `stReqDailyEmperorMoneyCmd` | 14 | 5 | `REQ_DAILY_EMPEROR_MONEY` | 7018 |
| `0x1637` | `stReqKingListCmd` | 14 | 5 | `REQ_KING_LIST_PARA` | 7028 |
| `0x1638` | `stRtnKingListCmd` | 15 | 6 | `RTN_KING_LIST_PARA` | 7045 |
| `0x1639` | `stReqCityOwnerListCmd` | 14 | 5 | `REQ_CITY_OWNER_LIST_PARA` | 7058 |
| `0x163A` | `stRtnCityOwnerListCmd` | 18 | 8 | `RTN_CITY_OWNER_LIST_PARA` | 7068 |
| `0x163B` | `stReqNpcOwnerListCmd` | 14 | 5 | `REQ_NPC_OWNER_LIST_PARA` | 7085 |
| `0x163C` | `stRtnNpcOwnerListCmd` | 26 | 11 | `RTN_NPC_OWNER_LIST_PARA` | 7094 |
| `0x163D` | `stUpdateCountryCatcherUserCmd` | 16 | 7 | `UPDATE_COUNTRY_CATCHER` | 7114 |
| `0x163E` | `stUpdateCountryCatchexUserCmd` | 16 | 7 | `UPDATE_COUNTRY_CATCHEX` | 7128 |
| `0x163F` | `stUpdateCountryCensorUserCmd` | 16 | 7 | `UPDATE_COUNTRY_CENSOR` | 7142 |
| `0x1640` | `stUpdateCountryCensosUserCmd` | 16 | 7 | `UPDATE_COUNTRY_CENSOS` | 7156 |
| `0x1641` | `stUpdateCountryYuanshuaiUserCmd` | 16 | 7 | `UPDATE_COUNTRY_YUANSHUAI` | 7170 |
| `0x1642` | `stUpdateCountryZaixiangUserCmd` | 16 | 7 | `UPDATE_COUNTRY_ZAIXIANG` | 7184 |
| `0x1643` | `stReqDailyKingMoneyCmd` | 14 | 5 | `REQ_DAILY_KING_MONEY` | 7198 |
| `0x1644` | `stReqDailyYuanshuaiMoneyCmd` | 14 | 5 | `REQ_DAILY_YUANSHUAI_MONEY` | 7208 |
| `0x1645` | `stReqDailyZaixiangMoneyCmd` | 14 | 5 | `REQ_DAILY_ZAIXIANG_MONEY` | 7218 |
| `0x1646` | `stReqDailyCatcherMoneyCmd` | 14 | 5 | `REQ_DAILY_CATCHER_MONEY` | 7228 |
| `0x1647` | `stReqDailyCensorMoneyCmd` | 14 | 5 | `REQ_DAILY_CENSOR_MONEY` | 7238 |
| `0x1648` | `stReqDailyCatchexMoneyCmd` | 14 | 5 | `REQ_DAILY_CATCHEX_MONEY` | 7248 |
| `0x1649` | `stReqDailyCensosMoneyCmd` | 14 | 5 | `REQ_DAILY_CENSOS_MONEY` | 7258 |
| `0x164C` | `stChangeCountrySalaryCmd` | 18 | 6 | `CHANGE_COUNTRY_SALARY_PARA` | 7268 |
| `0x164D` | `stAddCountrySalaryCmd` | 14 | 5 | `ADD_COUNTRY_SALARY_PARA` | 7280 |
| `0x164E` | `stAddCountryLoginCmd` | 14 | 5 | `ADD_COUNTRY_LOGIN_PARA` | 7290 |
| `0x164F` | `stAddCountryVipCmd` | 14 | 5 | `ADD_COUNTRY_VIP_PARA` | 7300 |
| `0x1650` | `stAddCountryChouCmd` | 14 | 5 | `ADD_COUNTRY_CHOU_PARA` | 7310 |

### 0x18 `SELECT_USERCMD` · 选择指令（8 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1801` | `stUserInfoUserCmd` | 17 | 7 | `USERINFO_SELECT_USERCMD_PARA` | 2729 |
| `0x1802` | `stCreateSelectUserCmd` | 26 | 11 | `CREATE_SELECT_USERCMD_PARA` | 2761 |
| `0x1803` | `stLoginSelectUserCmd` | 25 | 7 | `LOGIN_SELECT_USERCMD_PARA` | 2778 |
| `0x1804` | `stDeleteSelectUserCmd` | 19 | 7 | `DELETE_SELECT_USERCMD_PARA` | 2791 |
| `0x1805` | `stCheckNameSelectUserCmd` | 16 | 7 | `CHECKNAME_SELECT_USERCMD_PARA` | 2805 |
| `0x1806` | `stReturnDeleteSelectUserCmd` | 16 | 7 | `RETURN_DELETE_SELECT_USERCMD_PARA` | 2820 |
| `0x1807` | `stRequestSelectUserCmd` | 14 | 5 | `REQUEST_COUNTRY_SELECT_USERCMD_PARA` | 2836 |
| `0x1808` | `stDeleteErrorSelectUserCmd` | 14 | 5 | `DELETE_ERROR_SELECT_USERCMD_PARA` | 2846 |

### 0x19 `RELATION_USERCMD` · 社会关系指令（6 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1901` | `stSendRelationListCmd` | 16 | 6 | `RELATION_SEND_RELATIONLIST_PARA` | 7930 |
| `0x1903` | `stRelationStatusCmd` | 36 | 15 | `RELATION_STATUS_PARA` | 7953 |
| `0x1904` | `stMarryStatusCheckCmd` | 15 | 6 | `MARRY_STATUS_CHECK_PARA` | 7998 |
| `0x1905` | `stUnmarryCmd` | 14 | 5 | `UNMARRY_PARA` | 8010 |
| `0x1906` | `stReqUserSortInfoCmd` | 14 | 5 | `REQ_USER_SORT_INFO_PARA` | 8049 |
| `0x1907` | `stRetUserSortInfoCmd` | 16 | 6 | `RET_USER_SORT_INFO_PARA` | 8058 |

### 0x1A `SCHOOL_USERCMD` · 师徒门派关系指令（15 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1A01` | `stAddMemberToSchoolCmd` | 30 | 13 | `ADD_MEMBER_TO_SCHOOL_PARA` | 8093 |
| `0x1A02` | `stFireMemberFromSchoolCmd` | 15 | 6 | `FIRE_MEMBER_FROM_SCHOOL_PARA` | 8116 |
| `0x1A03` | `stSchoolStatusCheckCmd` | 14 | 5 | `SCHOOL_STATUS_CHECK_PARA` | 8127 |
| `0x1A04` | `stCreateSchoolCmd` | 15 | 6 | `CREATE_SCHOOL_PARA` | 8137 |
| `0x1A05` | `stSchoolMemberLeaveSchoolCmd` | 14 | 5 | `SCHOOLMEMBER_LEAVE_SCHOOL_PARA` | 8148 |
| `0x1A06` | `stSendListSchoolCmd` | 21 | 8 | `SENDLIST_SCHOOL_PARA` | 8194 |
| `0x1A07` | `stSendMemberInfoCmd` | 16 | 6 | `SENDMEMBERINFO_SCHOOL_PARA` | 8216 |
| `0x1A09` | `stSchoolMemberStatusCmd` | 24 | 11 | `SCHOOL_MEMBER_STATUS_PARA` | 8250 |
| `0x1A0A` | `stSchoolBulletinCmd` | 17 | 8 | `SCHOOL_BULLETIN__PARA` | 8273 |
| `0x1A0B` | `stDestroySchoolCmd` | 14 | 5 | `DESTROY_SCHOOL_PARA` | 8286 |
| `0x1A0C` | `stSchoolBulletinNotifyCmd` | 14 | 5 | `SCHOOL_BULLETIN_NOTIFY_PARA` | 8295 |
| `0x1A0D` | `stReqMasterBountyUserCmd` | 15 | 6 | `REQ_MASTER_BOUNTY_PARA` | 8312 |
| `0x1A0E` | `stReqPrenticeBountyUserCmd` | 15 | 6 | `REQ_PRENTICE_BOUNTY_PARA` | 8324 |
| `0x1A0F` | `stRtnMasterBountyUserCmd` | 22 | 7 | `RTN_MASTER_BOUNTY_PARA` | 8336 |
| `0x1A10` | `stRtnPrenticeBountyUserCmd` | 27 | 9 | `RTN_PRENTICE_BOUNTY_PARA` | 8351 |

### 0x1B `SEPT_USERCMD` · 家族指令（21 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1B01` | `stSeptStatusCheckCmd` | 14 | 5 | `SEPT_STATUS_CHECK_PARA` | 8494 |
| `0x1B02` | `stCreateSeptCmd` | 15 | 6 | `CREATE_SEPT_PARA` | 8505 |
| `0x1B03` | `stServerReturnSeptCheckCmd` | 14 | 5 | `SERVER_RETURN_SEPT_CREATE_FAILED` | 8516 |
| `0x1B05` | `stAllSeptMemberCmd` | 16 | 6 | `ALL_SEPT_MEMBER_PARA` | 8536 |
| `0x1B07` | `stAddMemberToSeptCmd` | 21 | 9 | `ADD_MEMBER_TO_SEPT_PARA` | 8555 |
| `0x1B08` | `stFireMemberFromSeptCmd` | 15 | 6 | `FIRE_MEMBER_FROM_SEPT_PARA` | 8569 |
| `0x1B0B` | `stSeptBaseInfoCmd` | 283 | 11 | `SEPT_BASE_INFO_PARA` | 8610 |
| `0x1B0C` | `stSeptMemberLeaveSeptCmd` | 14 | 5 | `SEPTMEMBER_LEAVE_SEPT_PARA` | 8627 |
| `0x1B0D` | `stVoteListSeptCmd` | 20 | 8 | `VOTELIST_SEPT_PARA` | 8644 |
| `0x1B0E` | `stVoteSeptCmd` | 16 | 7 | `VOTE_SEPT_PARA` | 8658 |
| `0x1B0F` | `stNoteSeptCmd` | 269 | 6 | `NOTE_SEPT_PARA` | 8670 |
| `0x1B14` | `stListSeptCmd` | 19 | 7 | `LIST_SEPT_PARA` | 8755 |
| `0x1B15` | `stRequestJoinSeptCmd` | 16 | 7 | `REQUEST_JOIN_SEPT_PARA` | 8774 |
| `0x1B16` | `stRequestNpcInfoSeptCmd` | 14 | 5 | `REQUEST_NPCINFO_SEPT_PARA` | 8786 |
| `0x1B17` | `stRequestSeptExpCmd` | 14 | 5 | `REQUEST_SEPT_EXP_PARA` | 8796 |
| `0x1B18` | `stRequestAbjurationSeptNpcSeptCmd` | 14 | 5 | `REQUEST_ABJURATION_SEPTNPC_PARA` | 8806 |
| `0x1B19` | `stReqSeptNameCmd` | 18 | 6 | `REQ_SEPT_NAME_PARA` | 8816 |
| `0x1B1A` | `stRtnSeptNameCmd` | 19 | 7 | `RTN_SEPT_NAME_PARA` | 8829 |
| `0x1B1B` | `stReqSeptNameListCmd` | 19 | 7 | `REQ_SEPT_NAME_LIST_PARA` | 8843 |
| `0x1B1C` | `stRtnSeptNameListCmd` | 20 | 8 | `RTN_SEPT_NAME_LIST_PARA` | 8856 |
| `0x1B1D` | `stRequestSeptNormalExpCmd` | 14 | 5 | `REQUEST_SEPT_NORMAL_EXP_PARA` | 8873 |

### 0x1C `DARE_USERCMD` · —（1 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1C01` | `stActiveDareCmd` | 30 | 12 | `ACTIVE_DARE_PARA` | 7726 |

### 0x1D `PET_USERCMD` · 宠物指令（15 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x1D01` | `stSetAIPetCmd` | 14 | 5 | `SETAI_PET_PARA` | 8908 |
| `0x1D02` | `stRequestDataPetCmd` | 14 | 5 | `REQUESTDATA_PET_PARA` | 8965 |
| `0x1D03` | `stRefreshDataPetCmd` | 18 | 6 | `REFRESHDATA_PET_PARA` | 8976 |
| `0x1D04` | `stChangeNamePetCmd` | 15 | 6 | `CHANGENAME_PET_PARA` | 8991 |
| `0x1D05` | `stAddPetPetCmd` | 18 | 6 | `ADDPET_PET_PARA` | 9003 |
| `0x1D06` | `stDelPetPetCmd` | 18 | 6 | `DELPET_PET_PARA` | 9015 |
| `0x1D07` | `stDelSkillPetCmd` | 18 | 6 | `DELSKILL_PET_PARA` | 9027 |
| `0x1D08` | `stCallHorsePetCmd` | 14 | 5 | `CALLHORSE_PET_PARA` | 9039 |
| `0x1D09` | `stPutHorsePetCmd` | 14 | 5 | `PUTHORSE_PET_PARA` | 9049 |
| `0x1D0A` | `stHorseDataPetCmd` | 18 | 6 | `HORSEDATA_PET_PARA` | 9059 |
| `0x1D0B` | `stSetTargetPetCmd` | 22 | 7 | `SETTARGET_PET_PARA` | 9073 |
| `0x1D0B` | `stChangeHorsePetCmd` | 18 | 6 | `CHANGEHORSE_PET_PARA` | 9123 |
| `0x1D0C` | `stHpExpPetCmd` | 26 | 8 | `HP_EXP_PET_PARA` | 9086 |
| `0x1D0D` | `stCallFabaoPetCmd` | 18 | 6 | `CALLFABAO_PET_PARA` | 9102 |
| `0x1D0E` | `stPutFabaoPetCmd` | 14 | 5 | `PUTFABAO_PET_PARA` | 9113 |

### 0x25 `CARTOON_USERCMD` · —（20 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2501` | `stBuyCartoonCmd` | 18 | 6 | `BUY_CARTOON_PARA` | 10206 |
| `0x2502` | `stAddCartoonCmd` | 18 | 6 | `ADD_CARTOON_PARA` | 10216 |
| `0x2503` | `stRemoveCartoonCmd` | 18 | 6 | `REMOVE_CARTOON_PARA` | 10228 |
| `0x2504` | `stFollowCartoonCmd` | 18 | 6 | `FOLLOW_CARTOON_PARA` | 10238 |
| `0x2505` | `stLetOutCartoonCmd` | 18 | 6 | `LETOUT_CARTOON_PARA` | 10248 |
| `0x2506` | `stAdoptCartoonCmd` | 18 | 6 | `ADOPT_CARTOON_PARA` | 10258 |
| `0x2507` | `stReturnCartoonCmd` | 18 | 6 | `RETURN_CARTOON_PARA` | 10268 |
| `0x2508` | `stGetBackCartoonCmd` | 14 | 5 | `GETBACK_CARTOON_PARA` | 10278 |
| `0x2509` | `stDrawExpCartoonCmd` | 18 | 6 | `DRAWEXP_CARTOON_PARA` | 10287 |
| `0x250A` | `stChangeNameCartoonCmd` | 19 | 7 | `CHANGENAME_CARTOON_PARA` | 10297 |
| `0x250B` | `stAddWaitingCartoonCmd` | 29 | 11 | `ADD_WAITING_CARTOON_PARA` | 10308 |
| `0x250C` | `stRemoveWaitingCartoonCmd` | 18 | 6 | `REMOVE_WAITING_CARTOON_PARA` | 10332 |
| `0x250D` | `stRequestListCartoonCmd` | 14 | 5 | `REQUEST_LIST_CARTOON_PARA` | 10342 |
| `0x250E` | `stSaleCartoonCmd` | 18 | 6 | `SALE_CARTOON_PARA` | 10351 |
| `0x250F` | `stChargeCartoonCmd` | 22 | 7 | `CHARGE_CARTOON_PARA` | 10363 |
| `0x2510` | `stHpExpCartoonCmd` | 26 | 8 | `HP_EXP_CARTOON_PARA` | 10374 |
| `0x2511` | `stSellAllCartoonCmd` | 14 | 5 | `SELL_ALL_CARTOON_PARA` | 10386 |
| `0x2512` | `stConsignCartoonCmd` | 19 | 7 | `CONSIGN_CARTOON_PARA` | 10395 |
| `0x2513` | `stConsignRetCartoonCmd` | 19 | 7 | `CONSIGN_RET_CARTOON_PARA` | 10406 |
| `0x2514` | `stRepairCartoonCmd` | 19 | 7 | `REPAIR_CARTOON_PARA` | 10417 |

### 0x26 `STOCK_SCENE_USERCMD` · 定义股票命令（1 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2605` | `stTransferFundStockSaveUserCmd` | 22 | 7 | `TRANSFER_FUND_SAVE_STOCKPARA` | 9511 |

### 0x27 `STOCK_BILL_USERCMD` · 定义股票命令（28 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2701` | `stPassowrdStockUserCmd` | 17 | 8 | `PASSWORD_STOCKPARA` | 9447 |
| `0x2702` | `stReturnPasswordStockIserCmd` | 15 | 6 | `RETUEN_PASSWORD_STOCKPARA` | 9478 |
| `0x2703` | `stQueryFundStockUserCmd` | 14 | 5 | `QUERY_FUND_STOCKPARA` | 9488 |
| `0x2704` | `stReturnFundStockUserCmd` | 22 | 7 | `RETURN_FUND_STOCKPARA` | 9499 |
| `0x2706` | `stTransferFundStockFetchUserCmd` | 22 | 7 | `TRANSFER_FUND_FETCH_STOCKPARA` | 9523 |
| `0x2707` | `stConsignGoldStockUserCmd` | 18 | 6 | `CONSIGN_GOLD_STOCKPARA` | 9535 |
| `0x2708` | `stConsignMoneyStockUserCmd` | 18 | 6 | `CONSIGN_MONEY_STOCKPARA` | 9546 |
| `0x2709` | `stConsignCancelGoldStockUserCmd` | 18 | 6 | `CONSIGN_CANCEL_GOLD_STOCKPARA` | 9556 |
| `0x270A` | `stConsignCancelMoneyStockUserCmd` | 18 | 6 | `CONSIGN_CANCEL_MONEY_STOCKPARA` | 9567 |
| `0x270B` | `stRequestConsignListStockUserCmd` | 14 | 5 | `REQUEST_CONSIGN_LIST_STOCKPARA` | 9589 |
| `0x270C` | `stConsignGoldListStockUserCmd` | 18 | 6 | `CONSIGN_GOLD_LIST_STOCKPARA` | 9599 |
| `0x270D` | `stConsignMoneyListStockUserCmd` | 18 | 6 | `CONSIGN_MONEY_LIST_STOCKPARA` | 9610 |
| `0x270E` | `stRequestCancelListStockUserCmd` | 19 | 7 | `REQUEST_CANCEL_LIST_STOCKPARA` | 9621 |
| `0x270F` | `stReturnCancelListStockUserCmd` | 20 | 8 | `RETURN_CANCEL_LIST_STOCKPARA` | 9637 |
| `0x2710` | `stRequestFirstTenListStockUserCmd` | 14 | 5 | `REQUEST_FIRSTTEN_LIST_STOCKPARA` | 9649 |
| `0x2711` | `stFirstFiveListStockUserCmd` | 19 | 7 | `FIRSTFIVE_LIST_STOCKPARA` | 9664 |
| `0x2712` | `stPutListStockUserCmd` | 23 | 8 | `PUT_LIST_STOCKPARA` | 9677 |
| `0x2713` | `stReturnPutListStockUserCmd` | 14 | 5 | `RETURN_PUT_LIST_STOCKPARA` | 9689 |
| `0x2714` | `stRequstHistoryStockUserCmd` | 22 | 7 | `REQUEST_HISTORY_STOCKPARA` | 9712 |
| `0x2715` | `stNewHistoryStockUserCmd` | 14 | 5 | `NEW_HISTORY_STOCKPARA` | 9725 |
| `0x2716` | `stHistoryGoldStockUserCmd` | 18 | 6 | `HISTORY_GOLD_STOCKPARA` | 9736 |
| `0x2717` | `stHistoryMoneyStockUserCmd` | 18 | 6 | `HISTORY_MONEY_STOCKPARA` | 9747 |
| `0x2718` | `stRequstSelfHistoryStockUserCmd` | 22 | 7 | `REQUEST_SELF_HISTORY_STOCKPARA` | 9758 |
| `0x2719` | `stSelfHistoryGoldStockUserCmd` | 18 | 6 | `SELF_HISTORY_GOLD_STOCKPARA` | 9784 |
| `0x271A` | `stSelfHistoryMoneyStockUserCmd` | 18 | 6 | `SELF_HISTORY_MONEY_STOCKPARA` | 9795 |
| `0x271B` | `stStopServiceStockUserCmd` | 14 | 5 | `STOP_SERVICE_STOCKPARA` | 9806 |
| `0x271C` | `stConsignCleanListStockUserCmd` | 14 | 5 | `CONSIGN_CLEAN_LIST_STOCKPARA` | 9816 |
| `0x271D` | `stRequestStateStockUserCmd` | 14 | 5 | `REQUEST_STATE_STOCKPARA` | 9826 |

### 0x28 `VOTE_USERCMD` · 投票指令（3 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2801` | `stRequestVoteListUserCmd` | 15 | 6 | `REQUEST_VOTE_LIST_PARA` | 7341 |
| `0x2802` | `stReturnVoteListUserCmd` | 23 | 8 | `RETURN_VOTE_LIST_PARA` | 7361 |
| `0x2803` | `stCommitVoteUserCmd` | 22 | 7 | `COMMIT_VOTE_PARA` | 7379 |

### 0x29 `ARMY_USERCMD` · 军队指令（14 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2901` | `stCreateArmyUserCmd` | 16 | 7 | `CREATE_ARMY_PARA` | 7408 |
| `0x2902` | `stReqWaitGenUserCmd` | 14 | 5 | `REQ_WAIT_GEN_PARA` | 7423 |
| `0x2902` | `stRtnWaitGenUserCmd` | 18 | 6 | `REQ_WAIT_GEN_PARA` | 7439 |
| `0x2904` | `stReqArmyGenUserCmd` | 15 | 6 | `REQ_ARMY_GEN_PARA` | 7460 |
| `0x2905` | `stReqArmyListUserCmd` | 15 | 6 | `REQ_ARMY_LIST_PARA` | 7479 |
| `0x2906` | `stRtnArmyListUserCmd` | 19 | 7 | `RTN_ARMY_LIST_PARA` | 7501 |
| `0x2907` | `stReqArmySpecUserCmd` | 18 | 6 | `REQ_ARMY_SPEC_PARA` | 7516 |
| `0x2908` | `stRtnArmySpecUserCmd` | 21 | 9 | `RTN_ARMY_SPEC_PARA` | 7534 |
| `0x2909` | `stExitArmyUserCmd` | 14 | 5 | `EXIT_ARMY_PARA` | 7554 |
| `0x290A` | `stRemoveArmyUserCmd` | 18 | 6 | `REMOVE_ARMY_PARA` | 7564 |
| `0x290B` | `stChangeArmyNameUserCmd` | 19 | 7 | `CHANGE_ARMY_NAME_PARA` | 7577 |
| `0x290C` | `stAddArmyCaptainUserCmd` | 25 | 10 | `ADD_ARMY_CAPTAIN_PARA` | 7597 |
| `0x290C` | `stChangeArmyGenUserCmd` | 19 | 7 | `CHANGE_ARMY_GEN_PARA` | 7631 |
| `0x290D` | `stFireArmyCaptainUserCmd` | 18 | 6 | `FIRE_ARMY_CAPTAIN_PARA` | 7618 |

### 0x2A `GEM_USERCMD` · 护宝指令（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2A01` | `stReqDragonUserCmd` | 14 | 5 | `REQUEST_DRAGON_PARA` | 7661 |
| `0x2A02` | `stReqTigerUserCmd` | 14 | 5 | `REQUEST_TIGER_PARA` | 7671 |

### 0x2F `RECOMMEND_USERCMD` · 定义推荐人系统命令（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x2F01` | `stSetRecommendUserCmd` | 15 | 6 | `SET_RECOMMEND_PARA` | 11231 |
| `0x2F02` | `stReqRecommendBountyUserCmd` | 15 | 6 | `REQ_RECOMMEND_BOUNTY_PARA` | 11250 |
| `0x2F03` | `stReqRecommendedBountyUserCmd` | 15 | 6 | `REQ_RECOMMENDED_BOUNTY_PARA` | 11262 |
| `0x2F04` | `stRtnRecommendBountyUserCmd` | 26 | 8 | `RTN_RECOMMEND_BOUNTY_PARA` | 11281 |
| `0x2F05` | `stRtnRecommendedBountyUserCmd` | 27 | 9 | `RTN_RECOMMENDED_BOUNTY_PARA` | 11299 |

### 0x30 `SAFETY_USERCMD` · 定义财产保护系统指令（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3001` | `stSetSafetyUserCmd` | 19 | 7 | `SET_SAFETY_PARA` | 11333 |
| `0x3002` | `stNotifySafetyUserCmd` | 16 | 7 | `NOTIFY_SAFETY_PARA` | 11354 |
| `0x3003` | `stSetTempUnSafetyUserCmd` | 18 | 6 | `SET_TEMP_UNSAFETY_PARA` | 11369 |
| `0x3004` | `stSetSafetyDetailUserCmd` | 19 | 7 | `SET_SAFETY_DETAIL_PARA` | 11389 |
| `0x3005` | `stSetSafetyPasswordUserCmd` | 20 | 8 | `SET_SAFETY_PASSWORD_PARA` | 11403 |

### 0x33 `SAFETY_COWBOX` · 暂定宝盒相关指令（3 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3301` | `stSetCowBoxKeyCmd` | 22 | 7 | `SET_COWBOX_KEY_PARAMETER` | 12064 |
| `0x3302` | `stGetCowBoxInitCmd` | 22 | 7 | `GET_COWBOX_INIT_OPEN` | 12076 |
| `0x3303` | `stSetCowBoxTiquCmd` | 14 | 5 | `SET_COWBOX_TIQU_ITEM` | 12091 |

### 0x34 `TURN_USERCMD` · 暂定转生相关指令（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3401` | `stTurnRequestCmd` | 14 | 5 | `SET_TRUN_REQUEST_PARAMETER` | 11434 |
| `0x3402` | `stTurnResultCmd` | 14 | 5 | `SET_TRUN_RESULT_PARAMETER` | 11451 |

### 0x35 `HORSETRAINING_USERCMD` · 训马指令（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3501` | `stHorseTrainingUserCmd` | 18 | 6 | `HORSETRAINING_USERCMD_TEY` | 11996 |
| `0x3502` | `stHouseFaceTrainingUserCmd` | 22 | 7 | `HOUSRFACETRAINING_USERCMD_TEY` | 12008 |

### 0x36 `LUA_USERCMD` · LUA脚本指令（1 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3601` | `stLuaInputBoxUserCmd` | 264 | 7 | `LUA_INPUTBOX_USERCMD_TEY` | 12035 |

### 0x38 `FUJIAN_USERCMD` · 这里每个指令 都有一个public stFujianUserCmd 你可以理解为 是 stFujianUserCmd 里面的子命令 stFujianUserCmd是啥 是 54（6 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3801` | `stFujianJianzhongUserCmd` | 14 | 5 | `FUJIANJIANZHONG_USERCMD` | 12169 |
| `0x3805` | `stFujianZtzUserCmd` | 14 | 5 | `FUJIANZTZ_USERCMD` | 12221 |
| `0x3807` | `stFujianShengqiUserCmd` | 14 | 5 | `FUJIANSHENGQI_USERCMD` | 12329 |
| `0x380A` | `stFujianShengxiaoUserCmd` | 14 | 5 | `FUJIANSHENGXIAO_USERCMD` | 12277 |
| `0x3811` | `stFujianJingmaiUserCmd` | 14 | 5 | `FUJIANJINGMAI_USERCMD` | 12370 |
| `0x3815` | `stFujianTouxianUserCmd` | 14 | 5 | `FUJIANJTOUXIAN_USERCMD` | 12414 |

### 0x39 `LIANZHAN_USERCMD` · 连斩指令 By:醉梦  2024/03/03（1 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3901` | `stLianzhanstartUserCmd` | 22 | 7 | `LIANZHANSTART_USERCMD` | 12481 |

### 0x3A `JINGCAI_USERCMD` · 功勋竞猜相关指令 By醉梦 2024/03/12（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3A01` | `stJingcaiXiazhuCmd` | 22 | 7 | `SET_COWBOX_KEY_PARAMETER` | 12512 |
| `0x3A02` | `stJingcaiInfoCmd` | 14 | 5 | `JINGCAI_INFO` | 12525 |
| `0x3A03` | `stJingcaiReturnInfoCmd` | 30 | 9 | `JINGCAI_RETURNINFO` | 12535 |
| `0x3A04` | `stJingcaiReturnPigTimeCmd` | 14 | 5 | `JINGCAI_RETURNPIGTIME` | 12554 |
| `0x3A05` | `stJingcaiReturnKillPigTimeCmd` | 14 | 5 | `JINGCAI_RETURNKILLPIGTIME` | 12565 |

### 0x3B `XINFUBEN_USERCMD` · 功勋竞猜相关指令（4 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3B01` | `stFubenInfoCmd` | 14 | 5 | `FUBEN_INFO` | 12601 |
| `0x3B02` | `stFubenReturnInfoCmd` | 34 | 10 | `FUBEN_RETURNINFO` | 12611 |
| `0x3B03` | `stFubenInCmd` | 22 | 7 | `FUBEN_IN` | 12626 |
| `0x3B04` | `stFubenBuyCmd` | 18 | 6 | `FUBEN_BUY` | 12638 |

### 0x3C `ERNV_USERCMD` · 儿女指令 By:醉梦  2024/10/17（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3C01` | `stErnvInfoUserCmd` | 14 | 5 | `ERNVINFO_USERCMD` | 12668 |
| `0x3C03` | `stErnvPutUserCmd` | 22 | 7 | `ERNVPUT_USERCMD` | 12709 |

### 0x3D `TIANXIA_USERCMD` · 天下第一相关指令 By醉梦 2024/04/27（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3D01` | `stTianxiaInfoCmd` | 14 | 5 | `TIANXIA_INFO` | 12788 |
| `0x3D02` | `stTianxiaReturnInfoCmd` | 14 | 5 | `TIANXIA_RETURNINFO` | 12807 |
| `0x3D03` | `stTianxiaBaomingCmd` | 14 | 5 | `TIANXIA_BAOMING` | 12818 |
| `0x3D04` | `stTianxiaBaominBackCmd` | 14 | 5 | `TIANXIA_BAOMING_BACK` | 12828 |
| `0x3D05` | `stTianxiaDuanweiCmd` | 14 | 5 | `TIANXIA_DUANWEI` | 12838 |

### 0x3E `BIKE_USERCMD` · 自行车相关指令 By醉梦 2024/11/05（6 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3E01` | `stBikeStartCmd` | 14 | 5 | `BIKE_START` | 12866 |
| `0x3E02` | `stDelBikeCmd` | 14 | 5 | `BIKE_DEL` | 12877 |
| `0x3E03` | `stBikeScnlCmd` | 18 | 6 | `BIKE_SCNL` | 12887 |
| `0x3E04` | `stBikeSkillCmd` | 26 | 8 | `BIKE_SKILL` | 12900 |
| `0x3E05` | `stBikeAdditemCmd` | 18 | 6 | `BIKE_ADDITEM` | 12913 |
| `0x3E06` | `stBikeStateCmd` | 18 | 6 | `BIKE_STATE_RETURN` | 12925 |

### 0x3F `BIESHU_USERCMD` · 云天别墅相关命令（7 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x3F01` | `stBieShuInfoCmd` | 14 | 5 | `BIESHU_INFO` | 12955 |
| `0x3F02` | `stBieShuInfoReturnCmd` | 38 | 11 | `BIESHU_INFO_RETURN` | 12965 |
| `0x3F03` | `stBieShuInCmd` | 14 | 5 | `BIESHU_IN` | 12981 |
| `0x3F04` | `stBieShuJiHuoCmd` | 18 | 6 | `BIESHU_JIHUO` | 12991 |
| `0x3F05` | `stBieShuJiHuoReturnCmd` | 18 | 6 | `BIESHU_JIHUO_RETURN` | 13003 |
| `0x3F06` | `stBieShuLevelUpCmd` | 18 | 6 | `BIESHU_LEVELUP` | 13014 |
| `0x3F07` | `stBieShuLevenUpReturnCmd` | 18 | 6 | `BIESHU_LEVELUP_RETURN` | 13025 |

### 0x40 `FENGYIN_USERCMD` · 等级封印相关命令 By醉梦 2024/12/08（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4001` | `stFengyinInfoCmd` | 14 | 5 | `FENGYIN_INFO` | 13056 |
| `0x4002` | `stFengyinInfoReturnCmd` | 18 | 6 | `FENGYIN_INFO_RETURN` | 13066 |

### 0x41 `HUISHOU_USERCMD` · 道具回收命令 By醉梦 2024/12/24（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4101` | `stHuishouInfoCmd` | 14 | 5 | `HUISHOU_INFO` | 13096 |
| `0x4102` | `stHuishouInfoReturnCmd` | 14 | 5 | `HUISHOU_INFO_RETURN` | 13114 |

### 0x42 `ZHUANHUAN_USERCMD` · 装备转换相关命令 By醉梦 2024/12/24（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4201` | `stZhuanhuanCmd` | 26 | 8 | `ZHUANHUAN_START` | 13167 |
| `0x4202` | `stZhuanhuanReturnCmd` | 18 | 6 | `ZHUANHUAN_RETURN` | 13180 |

### 0x43 `XIULIAN_USERCMD` · 能力修炼相关命令 By醉梦 2024/12/25（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4301` | `stXiulianInfoCmd` | 14 | 5 | `XIULIAN_INFO` | 13210 |
| `0x4302` | `stXiulianInfoReturnCmd` | 98 | 26 | `XIULIANINFO_RETURN` | 13220 |
| `0x4303` | `stXiulianStartCmd` | 18 | 6 | `XIULIAN_START` | 13255 |
| `0x4304` | `stXiulianReturnCmd` | 18 | 6 | `XIULIAN_RETURN` | 13266 |
| `0x4305` | `stXiulianResetCmd` | 14 | 5 | `XIULIAN_RESET` | 13277 |

### 0x45 `ZHANCHE_USERCMD` · 战车命令 By醉梦 2025/01/04（11 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4501` | `stZhancheListCmd` | 14 | 5 | `ZHANCHE_LIST` | 13421 |
| `0x4502` | `stZhancheListReturnCmd` | 14 | 5 | `ZHANCHE_LISTRETURN` | 13442 |
| `0x4503` | `stZhancheLingyongCmd` | 18 | 6 | `ZHANCHE_LINGYONG` | 13453 |
| `0x4504` | `stZhancheLingyongReturnCmd` | 18 | 6 | `ZHANCHE_LINGYONG_RETURN` | 13464 |
| `0x4505` | `stZhancheAddCmd` | 14 | 5 | `ZHANCHE_ADD` | 13475 |
| `0x4506` | `stZhancheTihuanCmd` | 14 | 5 | `ZHANCHE_TIHUAN` | 13485 |
| `0x4507` | `stZhancheShouhuiCmd` | 14 | 5 | `ZHANCHE_SHOUHUI` | 13495 |
| `0x4508` | `stZhancheZhaohuanCmd` | 14 | 5 | `ZHANCHE_ZHAOHUAN` | 13505 |
| `0x4509` | `stZhancheGuihuanCmd` | 14 | 5 | `ZHANCHE_GUIHUAN` | 13515 |
| `0x450A` | `stZhancheTuoliCmd` | 14 | 5 | `ZHANCHE_TUOLI` | 13525 |
| `0x450B` | `stZhancheSkillCmd` | 26 | 8 | `ZHANCHE_SKILL` | 13535 |

### 0x46 `JIAZUBOSS_USERCMD` · 家族BOSS相关指令 By醉梦 2025/01/06（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4601` | `stJiazuBossInfoCmd` | 14 | 5 | `JIAZUBOSS_INFO` | 13564 |
| `0x4602` | `stJiazuBossInfoReturnCmd` | 22 | 7 | `JIAZUBOSS_INFO_RETURN` | 13574 |
| `0x4603` | `stJiazuBossWeiyangCmd` | 18 | 6 | `JIAZUBOSS_WEIYANG` | 13587 |
| `0x4604` | `stJiazuBossWeiyangReturnCmd` | 18 | 6 | `JIAZUBOSS_WEIYANG_RETURN` | 13598 |
| `0x4605` | `stJiazuBossZhaohuanCmd` | 14 | 5 | `JIAZUBOSS_ZHAOHUAN` | 13609 |

### 0x47 `ZUOQI_USERCMD` · 坐骑图鉴相关指令 By醉梦 2025/01/09（5 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4701` | `stZuoqiInfoCmd` | 14 | 5 | `ZUOQI_INFO` | 13636 |
| `0x4702` | `stZuoqiInfoReturnCmd` | 82 | 22 | `ZUOQI_INFO_RETURN` | 13646 |
| `0x4703` | `stZuoqiLevelUpCmd` | 18 | 6 | `ZUOQI_LEVELUP` | 13673 |
| `0x4704` | `stZuoqiLevelUpReturnCmd` | 18 | 6 | `ZUOQI_LEVELUP_RETURN` | 13684 |
| `0x4705` | `stZuoqiLingQuCmd` | 14 | 5 | `ZUOQI_LINGQU` | 13695 |

### 0x48 `MIGONG_USERCMD` · 迷宫相关指令 By醉梦 2025/01/26（1 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4801` | `stMigongNextCmd` | 18 | 6 | `MIGONG_NEXT` | 13722 |

### 0x49 `BOSSINFO_USERCMD` · BOSS信息指令集（2 条）

| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |
|---|---|---|---|---|---|
| `0x4901` | `stReqKillerBossInfoUserCmd` | 14 | 5 | `REQ_KILLER_BOSSINFO_USERCMD` | 12118 |
| `0x4902` | `stRunKillerBossInfoUserCmd` | 39 | 13 | `RUN_KILLER_BOSSINFO_USERCMD` | 12128 |

## 3. 字段明细

完整偏移见 `fields.csv`；此处每结构体列一次，一条不漏。


#### `0x0101` `stUserVerifyVerCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | version | 4 |

#### `0x0102` `stUserRequestLoginCmd` · 总长 40 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | pstrName | 1 |
| 15 | char | pstrPassword | 1 |
| 16 | WORD | game | 2 |
| 18 | WORD | zone | 2 |
| 20 | char | raw | 7 |
| 27 | char | raw | 13 |

#### `0x0103` `stServerReturnLoginFailedCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byReturnCode | 1 |

#### `0x0104` `stServerReturnLoginSuccessCmd` · 总长 28 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserID | 4 |
| 18 | DWORD | loginTempID | 4 |
| 22 | char | pstrIP | 1 |
| 23 | WORD | wdPort | 2 |
| 25 | BYTE | randnum | 1 |
| 26 | BYTE | keyOffset | 1 |
| 27 | BYTE | key | 1 |

#### `0x0105` `stPasswdLogonUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | loginTempID | 4 |
| 18 | DWORD | dwUserID | 4 |
| 22 | char | pstrName | 1 |
| 23 | char | pstrPassword | 1 |

#### `0x0107` `stAccountLogonUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | strName | 1 |
| 15 | char | strPassword | 1 |

#### `0x0109` `stPasswordLogonUserCmd` · 总长 17 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | strName | 1 |
| 15 | char | strPassword | 1 |
| 16 | char | strNewPassword | 1 |

#### `0x010A` `stBackSelectUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x010B` `stJpegPassportUserCmd` · 总长 17 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |
| 16 | BYTE | data | 1 |

#### `0x010C` `stCountryInfoUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |

#### `0x0201` `stGameTimeTimerUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | QWORD | qwGameTime | 8 |

#### `0x0202` `stRequestUserGameTimeTimerUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0203` `stUserGameTimeTimerUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | QWORD | qwGameTime | 8 |

#### `0x0204` `stPingTimeTimerUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0301` `stMainUserDataUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0302` `stDecRemainderDataUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byDecType | 1 |

#### `0x0303` `stSetHPAndMPDataUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x031C` `stMapScreenSizeDataUserCmd` · 总长 45 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | width | 4 |
| 18 | DWORD | height | 4 |
| 22 | char | pstrMapName | 1 |
| 23 | char | pstrFilename | 1 |
| 24 | DWORD | setting | 4 |
| 28 | DWORD | rgb | 4 |
| 32 | char | pstrGroupName | 1 |
| 33 | char | pstrCountryName | 1 |
| 34 | char | pstrCityName | 1 |
| 35 | int | mainRoleX | 4 |
| 39 | int | mainRoleY | 4 |
| 43 | short | npc_count | 2 |

#### `0x031D` `stLoadMapOKDataUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x031E` `stLevelDegreeDataUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | degree | 2 |

#### `0x0333` `stStampDataUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |
| 18 | BYTE | bin | 1 |

#### `0x0334` `stStampCheckUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | flag | 4 |

#### `0x0335` `stMergeVersionCheckUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMergeVersion | 4 |

#### `0x03FF` `stEndOfInitDataDataUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0401` `stAddObjectPropertyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byActionType | 1 |

#### `0x0403` `stSwapObjectPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | qwThisID | 4 |

#### `0x0406` `stRefCountObjectPropertyUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | qwThisID | 4 |
| 18 | DWORD | dwNum | 4 |

#### `0x0407` `stUseObjectPropertyUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | qwThisID | 4 |
| 18 | DWORD | dwNumber | 4 |

#### `0x0408` `stPickUpItemPropertyUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | x | 4 |
| 18 | DWORD | y | 4 |

#### `0x0409` `stFoundItemPropertyUserCmd` · 总长 28 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | up_id | 4 |
| 18 | DWORD | count | 4 |
| 22 | DWORD | gem_id | 4 |
| 26 | BYTE | gem_level | 1 |
| 27 | BYTE | gem_num | 1 |

#### `0x040A` `stUpgradeItemPropertyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | location | 1 |

#### `0x040B` `stComposeItemPropertyUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | per_num | 1 |
| 15 | BYTE | location | 1 |

#### `0x040C` `stAccekKeyPropertyUserCmd` · 总长 17 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | accelNum | 2 |
| 16 | BYTE | activeGroup | 1 |

#### `0x040D` `stAddUserSkillPropertyUserCmd` · 总长 30 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSkillID | 4 |
| 18 | WORD | wdLevel | 2 |
| 20 | WORD | wdUpNum | 2 |
| 22 | DWORD | dwExperience | 4 |
| 26 | DWORD | dwMaxExperience | 4 |

#### `0x040E` `stUpgradeUserSkillPropertyUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSkillID | 4 |
| 18 | WORD | wdLevel | 2 |

#### `0x0410` `stSystemSettingsUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0411` `stDistributePointPropertyUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | type | 2 |

#### `0x0412` `stAddSkillPointPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSkillID | 4 |

#### `0x0413` `stClearPointPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwItemID | 4 |

#### `0x0413` `stMakeOddsPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | odds | 4 |

#### `0x0414` `stMakeSelectPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x0415` `stComposeSoulStonePropertyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | location | 1 |

#### `0x0416` `stEnchasePropertyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | location | 1 |

#### `0x0417` `stHolePropertyUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | up_id | 4 |
| 18 | DWORD | gem_id | 4 |
| 22 | BYTE | gem_level | 1 |
| 23 | BYTE | gem_num | 1 |

#### `0x0418` `stDecomposePropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | up_id | 4 |

#### `0x0419` `stBodyColorPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwBodyColorCustom | 4 |

#### `0x041A` `stHairPropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byHairType | 1 |
| 15 | DWORD | dwHairColor | 4 |

#### `0x041B` `stSelectPropertyUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |
| 19 | BYTE | byOldType | 1 |
| 20 | DWORD | dwOldTempID | 4 |

#### `0x041D` `stSelectMainUserPropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x041F` `stDurabilityUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwThisID | 4 |
| 18 | DWORD | dwDur | 4 |
| 22 | DWORD | dwMaxDur | 4 |

#### `0x0421` `stClearSkillUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0422` `stFacePropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwFace | 4 |

#### `0x0423` `stHonorToPkValuePropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0424` `stScrollMapPropertyUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | qwThisID | 4 |
| 18 | DWORD | size | 4 |
| 22 | char | strMapName | 1 |

#### `0x0425` `stClearPointLimitPropertyUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | QWORD | qwThisID | 8 |
| 22 | BYTE | byProperty | 1 |

#### `0x0426` `stRequestIncMapPropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0427` `stSelectReturnHorsePropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0428` `stGoTrainPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |

#### `0x042D` `stModifyPropertyUserCmd` · 总长 25 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |
| 15 | DWORD | up_id | 4 |
| 19 | DWORD | gem_id | 4 |
| 23 | BYTE | gem_level | 1 |
| 24 | BYTE | gem_num | 1 |

#### `0x042E` `stMakezsPropertyUserCmd` · 总长 25 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |
| 15 | DWORD | up_id | 4 |
| 19 | DWORD | gem_id | 4 |
| 23 | BYTE | gem_level | 1 |
| 24 | BYTE | gem_num | 1 |

#### `0x042F` `stMakezqPropertyUserCmd` · 总长 25 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |
| 15 | DWORD | up_id | 4 |
| 19 | DWORD | gem_id | 4 |
| 23 | BYTE | gem_level | 1 |
| 24 | BYTE | gem_num | 1 |

#### `0x0430` `stMakelvPropertyUserCmd` · 总长 25 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |
| 15 | DWORD | up_id | 4 |
| 19 | DWORD | gem_id | 4 |
| 23 | BYTE | gem_level | 1 |
| 24 | BYTE | gem_num | 1 |

#### `0x0432` `stItemUseItemSuccessUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | ItemID | 4 |

#### `0x0433` `stGoFubenPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |

#### `0x0437` `stItem6UseItemSuccessUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | ItemID | 4 |

#### `0x0438` `stItem7UseItemSuccessUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | ItemID | 4 |

#### `0x043C` `stItem11UseItemSuccessUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | ItemID | 4 |

#### `0x043D` `stSelectFlowersPropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x043E` `stSelectFlowers1PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x043F` `stSelectFlowers2PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0440` `stSelectFlowers3PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0441` `stSelectFlowers4PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0442` `stSelectFlowers5PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0443` `stSelectFlowers6PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0444` `stSelectFlowers7PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0446` `stSelectFlowers20PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0447` `stSelectFlowers21PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0448` `stSelectFlowers22PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x0449` `stSelectFlowers23PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x044A` `stSelectFlowers41PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x044B` `stSelectFlowers51PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x044C` `stSelectFlowers61PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x044D` `stSelectFlowers71PropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x044E` `stScroll2MapPropertyUserCmd` · 总长 31 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | qwThisID | 4 |
| 18 | DWORD | xPos | 4 |
| 22 | DWORD | yPos | 4 |
| 26 | DWORD | size | 4 |
| 30 | char | strMapName | 1 |

#### `0x0452` `stOneKeyEquipPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |

#### `0x0453` `stGetOneKeyEquipPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |

#### `0x0455` `stGoHanbingPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |

#### `0x0456` `stGoAngelPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |

#### `0x0457` `stGoChuanSongPropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0458` `stGoChuanSong2PropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0459` `stGetChenghaoPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |

#### `0x045A` `stSetChenghaoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | chenghaoid | 4 |

#### `0x045B` `stSetNotChenghaoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | chenghaoid | 4 |

#### `0x045C` `stGetShizhuangPropertyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |

#### `0x045E` `stSetShizhuangUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | shizhuangid | 4 |

#### `0x045F` `stSetNotShizhuangUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | shizhuangid | 4 |

#### `0x0460` `stSetShizhuangJihuoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | shizhuangid | 4 |

#### `0x0461` `stMeiRiRenWuChuanSongUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | num | 4 |

#### `0x0462` `stGoChuanSong3PropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0463` `stGoChuanSong4PropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0465` `stSetPifengUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | pifengid | 4 |

#### `0x0466` `stSetNotPifengUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | pifengid | 4 |

#### `0x0467` `stSetPifengJihuoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | pifengid | 4 |

#### `0x0468` `stSetChibangUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | chibangid | 4 |

#### `0x0469` `stSetNotChibangUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | chibangid | 4 |

#### `0x046A` `stSetChibangJihuoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | chibangid | 4 |

#### `0x046B` `stGoChuanSong5PropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x046C` `stGoChuanSong6PropertyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x046D` `stSetZuoqiUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | zuoqiid | 4 |

#### `0x046E` `stSetNotZuoqiUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | zuoqiid | 4 |

#### `0x046F` `stSetZuoqiJihuoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | zuoqiid | 4 |

#### `0x0470` `stSuiJiChuanSongUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0472` `stSetJiemianUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | jiemianid | 4 |

#### `0x0473` `stSetNotJiemianUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | jiemianid | 4 |

#### `0x0474` `stSetJiemianJihuoUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOldTempID | 4 |
| 18 | DWORD | jiemianid | 4 |

#### `0x050A` `stAddUserMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x050B` `stRemoveUserMapScreenUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |

#### `0x050C` `stRequestUserDataMapScreenUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |
| 16 | DWORD | dwUserTempID | 4 |

#### `0x050D` `stRideMapScreenUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | BYTE | bySwitch | 1 |

#### `0x050E` `stRefreshStateMapScreenUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | BYTE | state | 1 |

#### `0x0514` `stAddMapObjectMapScreenUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | action | 1 |

#### `0x0515` `stRemoveMapObjectMapScreenUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMapObjectTempID | 4 |

#### `0x051E` `stAddMapBuildingMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x051F` `stRemoveMapBuildingMapScreenUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMapBuildingTempID | 4 |

#### `0x0520` `stChangeFaceMapScreenUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | DWORD | dwChangeFaceID | 4 |

#### `0x0532` `stAddMapNpcMapScreenUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x0533` `stRemoveMapNpcMapScreenUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMapNpcDataPosition | 4 |

#### `0x0534` `stRequestMapNpcDataMapScreenUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |
| 16 | DWORD | dwNpcTempID | 4 |

#### `0x0535` `stNPCHPMapScreenUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMapNpcDataPosition | 4 |
| 18 | DWORD | dwHP | 4 |
| 22 | DWORD | dwMaxHP | 4 |

#### `0x0536` `stAllMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0537` `stMapDataMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0538` `stRTSelectedHpMpPropertyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | dwTempID | 4 |

#### `0x053A` `stClearObjectOwnerMapScreenUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMapObjectTempID | 4 |

#### `0x053B` `stAddMapNpcAndPosMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x053C` `stAddUserAndPosMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x053D` `stBatchRemoveNpcMapScreenUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | num | 2 |
| 16 | DWORD | id | 4 |

#### `0x053D` `stAddPetMapScreenUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | size | 1 |

#### `0x053E` `stBatchRemoveUserMapScreenUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | num | 2 |
| 16 | DWORD | id | 4 |

#### `0x053F` `stSetStateMapScreenUserCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |
| 15 | DWORD | dwTempID | 4 |
| 19 | WORD | wdState | 2 |

#### `0x0540` `stClearStateMapScreenUserCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |
| 15 | DWORD | dwTempID | 4 |
| 19 | WORD | wdState | 2 |

#### `0x0541` `stAddMapNpcPetMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0542` `stAddMapPetMapScreenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0543` `stGoodnessStateMapScreenUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTempID | 4 |
| 18 | DWORD | dwGoodness | 4 |

#### `0x0544` `stAddUserMapScreenStateUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0545` `stAddMapNpcMapScreenStateUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0548` `stAddMapNpcPetMapScreenStateUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0549` `stMapDataMapScreenStateUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0602` `stUserMoveMoveUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | BYTE | byDirect | 1 |
| 19 | BYTE | bySpeed | 1 |
| 20 | WORD | x | 2 |
| 22 | WORD | y | 2 |

#### `0x0603` `stUserJumptoUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | x | 2 |
| 16 | WORD | y | 2 |

#### `0x060A` `stUserInstantJumpMoveUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | DWORD | dwNextScreenPosition | 4 |
| 22 | WORD | wdNextOffsetX | 2 |
| 24 | WORD | wdNextOffsetY | 2 |

#### `0x060C` `stUserGotoMapMoveUserCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | IsBackToCity | 2 |
| 16 | DWORD | dwMapType | 4 |
| 20 | char | mapName | 1 |

#### `0x0614` `stNpcMoveMoveUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNpcTempID | 4 |
| 18 | BYTE | byDirect | 1 |
| 19 | BYTE | bySpeed | 1 |
| 20 | WORD | x | 2 |
| 22 | WORD | y | 2 |

#### `0x061E` `stRequestLandMassMoveUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x061F` `stResponseLandMassMoveUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | int | size | 4 |

#### `0x0620` `stLandMassJumpMoveUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0621` `stSitDownMoveUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0C01` `stMainUserDeathReliveUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | DWORD | deathType | 4 |

#### `0x0C02` `stMainUserReliveReliveUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |

#### `0x0C03` `stOKReliveUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | BYTE | byType | 1 |

#### `0x0E01` `stChannelChatUserCmd` · 总长 41 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwType | 4 |
| 18 | DWORD | dwSysInfoType | 4 |
| 22 | DWORD | dwCharType | 4 |
| 26 | DWORD | dwChannelID | 4 |
| 30 | DWORD | dwFromID | 4 |
| 34 | DWORD | dwChatTime | 4 |
| 38 | BYTE | size | 1 |
| 39 | char | pstrName | 1 |
| 40 | char | pstrChat | 1 |

#### `0x0E01` `stServerChannelChatUserCmd` · 总长 39 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwType | 4 |
| 18 | DWORD | dwSysInfoType | 4 |
| 22 | DWORD | dwCharType | 4 |
| 26 | DWORD | dwChannelID | 4 |
| 30 | DWORD | dwFromID | 4 |
| 34 | DWORD | dwChatTime | 4 |
| 38 | BYTE | size | 1 |
| 39 | char | raw | 0 |

#### `0x0E02` `stRemoveTeamUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x0E03` `stAddTeamMemberUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTeamID | 4 |

#### `0x0E04` `stRemoveTeamMemberUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTeamID | 4 |
| 18 | DWORD | dwTempID | 4 |

#### `0x0E05` `stRequestTeamUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwAnswerUserID | 4 |
| 18 | DWORD | dwTeamMode | 4 |

#### `0x0E06` `stAnswerTeamUserCmd` · 总长 27 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwRequestUserID | 4 |
| 18 | DWORD | dwAnswerUserID | 4 |
| 22 | BYTE | byAgree | 1 |
| 23 | DWORD | dwTeamMode | 4 |

#### `0x0E07` `stCreateChannelUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwChannelID | 4 |
| 18 | DWORD | dwClientID | 4 |
| 22 | char | name | 1 |
| 23 | char | name2 | 1 |

#### `0x0E08` `stInvite_ChannelUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwChannelID | 4 |
| 18 | char | name | 1 |
| 19 | DWORD | dwCharType | 4 |

#### `0x0E09` `stJoin_ChannelUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwChannelID | 4 |
| 18 | char | name | 1 |

#### `0x0E0A` `stLeave_ChannelUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwChannelID | 4 |
| 18 | char | name | 1 |

#### `0x0E0E` `stTeamMemberOfflineUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTempID | 4 |

#### `0x0E0F` `stAutoReplyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | text | 1 |

#### `0x0E10` `stExpModeTeamMemberUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x0E11` `stObjModeTeamMemberUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x0E12` `stTeamModeUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x0E13` `stFreshTeamMemberUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTempID | 4 |

#### `0x0E14` `stDirtyUserUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |

#### `0x0E15` `stAnswerNameTeamUserCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | byRequestUserName | 1 |
| 15 | char | byAnswerUserName | 1 |
| 16 | BYTE | byAgree | 1 |
| 17 | DWORD | dwTeamMode | 4 |

#### `0x0E16` `stRequestNameTeamUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | byAnswerUserName | 1 |
| 15 | DWORD | dwTeamMode | 4 |

#### `0x0E17` `stRequestCountryHelpUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | mapName | 1 |
| 15 | WORD | x | 2 |
| 17 | WORD | y | 2 |

#### `0x0E18` `stKillForeignerUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | heroName | 1 |
| 15 | char | mapName | 1 |

#### `0x0E19` `stRefreshBossUserCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | npcid | 4 |
| 18 | BYTE | country | 1 |
| 19 | char | mapName | 1 |
| 20 | BYTE | time | 1 |

#### `0x0E1A` `stKillBossUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | npcid | 4 |
| 18 | char | name | 1 |
| 19 | char | mapName | 1 |
| 20 | WORD | x | 2 |
| 22 | WORD | y | 2 |

#### `0x0E1B` `stFriendCongratulateUserCmd` · 总长 27 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | playerid | 4 |
| 18 | char | name | 1 |
| 19 | DWORD | dwLevel | 4 |
| 23 | DWORD | dwType | 4 |

#### `0x0E1C` `stFriendCongratulateInfoUserCmd` · 总长 531 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | PlayerId | 4 |
| 18 | char | name | 1 |
| 19 | char | raw | 512 |

#### `0x1100` `stRequestTradeUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwAskerTempID | 4 |
| 18 | DWORD | dwAnswerTempID | 4 |

#### `0x1101` `stAnswerTradeUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byAgree | 1 |
| 15 | DWORD | dwAskerTempID | 4 |
| 19 | DWORD | dwAnswerTempID | 4 |

#### `0x1102` `stBeginTradeUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwAskerTempID | 4 |
| 18 | DWORD | dwAnswerTempID | 4 |

#### `0x1105` `stCommitTradeUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |

#### `0x1106` `stCancelTradeUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |

#### `0x1107` `stFinishTradeUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1108` `stAddObjectTradeUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | user_id | 4 |
| 18 | BYTE | x | 1 |
| 19 | BYTE | y | 1 |

#### `0x1109` `stRemoveObjectTradeUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | user_id | 4 |
| 18 | DWORD | object_id | 4 |

#### `0x110A` `stVisitNpcTradeUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNpcTempID | 4 |
| 18 | BYTE | byReturn | 1 |
| 19 | char | raw | 1 |

#### `0x110B` `stBuyObjectNpcTradeUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwObjectID | 4 |
| 18 | DWORD | dwNum | 4 |
| 22 | BYTE | itemLevel | 1 |

#### `0x110C` `stSellObjectNpcTradeUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | qwThisID | 4 |

#### `0x110D` `stRepairObjectNpcTradeUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |
| 18 | DWORD | gem_id | 4 |

#### `0x110E` `stSellHorseNpcTradeUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | action | 1 |

#### `0x110F` `stStoreInfoNpcTradeUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | page | 1 |
| 15 | BYTE | day | 1 |

#### `0x1110` `stSellTradeUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1111` `stFinishSellUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1112` `stAddObjectSellUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | price | 4 |
| 18 | BYTE | x | 1 |
| 19 | BYTE | y | 1 |

#### `0x1113` `stRemoveObjectSellUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | object_id | 4 |

#### `0x1114` `stRequestSellInfoUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | temp_id | 4 |

#### `0x1115` `stRequestSellBuyUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | temp_id | 4 |
| 18 | DWORD | object_id | 4 |

#### `0x1116` `stUpdateStorePassUserCmd` · 总长 30 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | raw | 8 |
| 22 | char | raw | 8 |

#### `0x1117` `stCanSellTradeUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | status | 1 |

#### `0x1118` `stGoldGiveTradeUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |

#### `0x111A` `stRequestGoldGiveTradeUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x111A` `stRequestMatarialGiveTradeUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x111B` `stReturnGoldGiveTradeUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | Storn_num | 4 |
| 18 | DWORD | Matarial_num | 4 |
| 22 | DWORD | Card_num | 4 |

#### `0x111B` `stReturnMatarialGiveTradeUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | num | 4 |

#### `0x111C` `stAddObjectSellListUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | num | 2 |
| 16 | DWORD | price | 4 |
| 20 | BYTE | x | 1 |
| 21 | BYTE | y | 1 |

#### `0x111D` `stRequestAddObjectSellUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | num | 2 |
| 16 | DWORD | qwThisID | 4 |
| 20 | DWORD | price | 4 |
| 24 | BYTE | x | 1 |
| 25 | BYTE | y | 1 |

#### `0x111E` `stBuyObjectSalaryTradeUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwObjectID | 4 |
| 18 | DWORD | dwNum | 4 |
| 22 | BYTE | itemLevel | 1 |

#### `0x111E` `stMatarialgiveTradeUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x111F` `stBuyObjectTicketTradeUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwObjectID | 4 |
| 18 | DWORD | dwNum | 4 |
| 22 | BYTE | itemLevel | 1 |

#### `0x1120` `stBuyObjectChongzhiTradeUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwObjectID | 4 |
| 18 | DWORD | dwNum | 4 |
| 22 | BYTE | itemLevel | 1 |

#### `0x1121` `stBuyObjectMallUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwObjectID | 4 |
| 18 | DWORD | dwNum | 4 |
| 22 | BYTE | itemLevel | 1 |

#### `0x1122` `stFortGoldGiveTradeUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | type | 1 |

#### `0x1123` `stRequestFortunGiveTradeUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1124` `stReturnFortunFundGiveTradeUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | Fortun0_num | 4 |
| 18 | DWORD | Fortun1_num | 4 |
| 22 | DWORD | Fortun2_num | 4 |

#### `0x1205` `stAttackMagicUserCmd` · 总长 35 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | DWORD | dwDefenceTempID | 4 |
| 22 | WORD | wdMagicType | 2 |
| 24 | WORD | xDes | 2 |
| 26 | WORD | yDes | 2 |
| 28 | BYTE | byDirect | 1 |
| 29 | BYTE | byAttackType | 1 |
| 30 | BYTE | byAction | 1 |
| 31 | DWORD | dwTempIDList | 4 |

#### `0x1206` `stRTMagicUserCmd` · 总长 35 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | BYTE | byTarget | 1 |
| 19 | BYTE | byRetcode | 1 |
| 20 | BYTE | byDirect | 1 |
| 21 | BYTE | bySrc | 1 |
| 22 | DWORD | dwSrcTempID | 4 |
| 26 | DWORD | dwHP | 4 |
| 30 | DWORD | dwReserved | 4 |
| 34 | BYTE | byLuck | 1 |

#### `0x1207` `stBackOffMagicUserCmd` · 总长 28 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTempID | 4 |
| 18 | BYTE | byType | 1 |
| 19 | BYTE | byDirect | 1 |
| 20 | DWORD | x | 4 |
| 24 | DWORD | y | 4 |

#### `0x1208` `stObtainExpUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwTempID | 4 |
| 18 | BYTE | byType | 1 |

#### `0x1209` `stNpcDeathUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNpcTempID | 4 |

#### `0x120A` `stLevelUpUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |

#### `0x120B` `stObjectHpMpPopUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserTempID | 4 |
| 18 | BYTE | byTarget | 1 |
| 19 | int | vChange | 4 |

#### `0x120C` `stRTMagicPosUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byTarget | 1 |

#### `0x1214` `stPKModeUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byPKMode | 1 |

#### `0x1216` `stObjectExpUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |
| 18 | DWORD | exp | 4 |

#### `0x1217` `stUnCombinUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1218` `stFireWorkUserCmd` · 总长 31 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | QWORD | qwObjectTempID | 8 |
| 22 | BYTE | byType | 1 |
| 23 | DWORD | dwUserID | 4 |
| 27 | DWORD | dwFireID | 4 |

#### `0x1219` `stTimeSyncUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x121A` `stSwitchCharacterObstacleUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x121B` `stRTOtherMagicUserCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byTarget | 1 |
| 15 | DWORD | dwUserTempID | 4 |
| 19 | BYTE | bySrc | 1 |
| 20 | DWORD | dwSrcTempID | 4 |

#### `0x1501` `stUnionStatusCheckCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1502` `stCreateUnionCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | UnionName | 1 |

#### `0x1503` `stServerReturnUnionCheckCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1505` `stAllUnionMemberCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |

#### `0x1506` `stReturnUnionMemberInfoCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | alias | 1 |
| 15 | BYTE | byPower | 1 |

#### `0x1507` `stAddMemberToUnionCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | memberName | 1 |
| 15 | DWORD | memberID | 4 |
| 19 | char | unionName | 1 |
| 20 | BYTE | byState | 1 |

#### `0x1508` `stFireMemberFromUnionCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | memberName | 1 |

#### `0x150B` `stUnionBaseInfoCmd` · 总长 289 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | unionName | 1 |
| 15 | char | master | 1 |
| 16 | WORD | wdLevel | 2 |
| 18 | QWORD | qwExp | 8 |
| 26 | char | raw | 255 |
| 281 | DWORD | dwMana | 4 |
| 285 | DWORD | dwAction | 4 |

#### `0x150C` `stUnionMemberLeaveUnionCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x150D` `stVoteListUnionCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | unionName | 1 |
| 15 | DWORD | dwSize | 4 |
| 19 | BYTE | flag | 1 |

#### `0x150E` `stVoteUnionCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | unionName | 1 |
| 15 | BYTE | bySuccess | 1 |

#### `0x150F` `stNoteUnionCmd` · 总长 269 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | raw | 255 |

#### `0x1518` `stListUnionCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |
| 18 | BYTE | flag | 1 |

#### `0x1519` `stRequestJoinUnionCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | BYTE | status | 1 |

#### `0x151A` `stRequestAbjurationCitynCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x151B` `stReqUnionNameCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUnionID | 4 |

#### `0x151C` `stRtnUnionNameCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUnionID | 4 |
| 18 | char | name | 1 |

#### `0x151D` `stReqUnionNameLlistCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | num | 1 |
| 15 | DWORD | dwUnionID | 4 |

#### `0x151E` `stRtnUnionNameListCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | num | 1 |
| 15 | DWORD | dwUnionID | 4 |
| 19 | char | name | 1 |

#### `0x151F` `stAppointCityCatcherCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1520` `stCancelCityCatcherCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1601` `stChangeCountryCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwToCountryID | 4 |

#### `0x1602` `stRequestDareCountryCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x1603` `stReturnDareCountryCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |
| 18 | BYTE | byType | 1 |

#### `0x1604` `stTransDareCountryCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountryID | 4 |

#### `0x1605` `stDareCountryFormalCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountryID | 4 |

#### `0x1606` `stRequestDareRecordCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x1607` `stReturnDareRecordResultCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |

#### `0x1608` `stReturnDareRecordStatCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |

#### `0x1609` `stReturnDareRecordPlanCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |

#### `0x160A` `stCancelCountryCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x160B` `stApplyCountryCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwToCountryID | 4 |

#### `0x160C` `stTaxCountryUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountry | 4 |
| 18 | BYTE | byTax | 1 |

#### `0x160D` `stFiskCountryUserCmd` · 总长 42 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountry | 4 |
| 18 | QWORD | qwGold | 8 |
| 26 | QWORD | qwMaterial | 8 |
| 34 | QWORD | qwStock | 8 |

#### `0x160E` `stSetTaxCountryUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountry | 4 |
| 18 | BYTE | byTax | 1 |

#### `0x160F` `stUpdateCountryStarCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |

#### `0x1610` `stContributeCountryMaterialCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemID | 4 |

#### `0x1611` `stSetCountryDareUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountry | 4 |
| 18 | BYTE | byStatus | 1 |
| 19 | BYTE | byResult | 1 |

#### `0x1612` `stUpdateCountryKingUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | kingName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x1613` `stAnswerCountryDareUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byStatus | 1 |

#### `0x1614` `stSelectTransLevelUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwLevel | 4 |

#### `0x1615` `stForbidTalkCountryUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1616` `stReqTechUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwType | 4 |

#### `0x1617` `stRtnTechUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwType | 4 |
| 18 | DWORD | dwSize | 4 |

#### `0x1618` `stReqWaitOfficialUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1619` `stRtnWaitOfficialUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |

#### `0x161A` `stSetTechSearchUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOption | 4 |
| 18 | DWORD | dwCharID | 4 |

#### `0x161B` `stUpTechDegreeUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOption | 4 |

#### `0x161C` `stConfirmSearcherUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOption | 4 |
| 18 | BYTE | byStatus | 1 |

#### `0x161D` `stDisableLoginCountryCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x161E` `stCancelTechSearchUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwOption | 4 |

#### `0x161F` `stAntiDareCountryFormalCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwCountryID | 4 |

#### `0x1620` `stCountryNoteCmd` · 总长 269 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | raw | 255 |

#### `0x1621` `stReqCountryNoteCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1622` `stKingPunishCountryCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | DWORD | method | 4 |

#### `0x1623` `stEmperorPunishCountryCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | DWORD | method | 4 |

#### `0x1624` `stReqGenCountryCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1625` `stRetGenCountryCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |
| 18 | DWORD | exp | 4 |
| 22 | DWORD | maxExp | 4 |

#### `0x1626` `stAppointDiplomatCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1627` `stCancelDiplomatCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1628` `stAppointCatcherCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1629` `stCancelCatcherCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x162A` `stAppointCatchexCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x162B` `stCancelCatchexCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x162C` `stAppointCensorCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x162D` `stCancelCensorCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x162E` `stAppointCensosCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x162F` `stCancelCensosCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1630` `stAppointYuanshuaiCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1631` `stCancelYuanshuaiCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1632` `stAppointZaixiangCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1633` `stCancelZaixiangCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1634` `stRequestCountryPowerCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1635` `stReturnCountryPowerCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | country | 1 |

#### `0x1636` `stReqDailyEmperorMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1637` `stReqKingListCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1638` `stRtnKingListCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | size | 1 |

#### `0x1639` `stReqCityOwnerListCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x163A` `stRtnCityOwnerListCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |
| 16 | char | cityName | 1 |
| 17 | char | unionName | 1 |

#### `0x163B` `stReqNpcOwnerListCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x163C` `stRtnNpcOwnerListCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |
| 16 | DWORD | npcID | 4 |
| 20 | char | mapName | 1 |
| 21 | WORD | x | 2 |
| 23 | WORD | y | 2 |
| 25 | char | septName | 1 |

#### `0x163D` `stUpdateCountryCatcherUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | catcherName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x163E` `stUpdateCountryCatchexUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | catchexName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x163F` `stUpdateCountryCensorUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | censorName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x1640` `stUpdateCountryCensosUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | censosName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x1641` `stUpdateCountryYuanshuaiUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | yuanshuaiName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x1642` `stUpdateCountryZaixiangUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | zaixiangName | 1 |
| 15 | BYTE | isEmperor | 1 |

#### `0x1643` `stReqDailyKingMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1644` `stReqDailyYuanshuaiMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1645` `stReqDailyZaixiangMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1646` `stReqDailyCatcherMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1647` `stReqDailyCensorMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1648` `stReqDailyCatchexMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1649` `stReqDailyCensosMoneyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x164C` `stChangeCountrySalaryCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwToCountryID | 4 |

#### `0x164D` `stAddCountrySalaryCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x164E` `stAddCountryLoginCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x164F` `stAddCountryVipCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1650` `stAddCountryChouCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1801` `stUserInfoUserCmd` · 总长 17 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |
| 16 | BYTE | data | 1 |

#### `0x1802` `stCreateSelectUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | strUserName | 1 |
| 15 | WORD | charType | 2 |
| 17 | BYTE | byHairType | 1 |
| 18 | DWORD | byRGB | 4 |
| 22 | WORD | country | 2 |
| 24 | WORD | five | 2 |

#### `0x1803` `stLoginSelectUserCmd` · 总长 25 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | charNo | 4 |
| 18 | char | raw | 7 |

#### `0x1804` `stDeleteSelectUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | charNo | 4 |
| 18 | char | numPassword | 1 |

#### `0x1805` `stCheckNameSelectUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | BYTE | err_code | 1 |

#### `0x1806` `stReturnDeleteSelectUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | BYTE | err_code | 1 |

#### `0x1807` `stRequestSelectUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1808` `stDeleteErrorSelectUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1901` `stSendRelationListCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |

#### `0x1903` `stRelationStatusCmd` · 总长 36 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | BYTE | type | 1 |
| 16 | WORD | occupation | 2 |
| 18 | WORD | user_level | 2 |
| 20 | WORD | level | 2 |
| 22 | DWORD | userid | 4 |
| 26 | BYTE | byState | 1 |
| 27 | DWORD | exploit | 4 |
| 31 | DWORD | country | 4 |
| 35 | char | unionName | 1 |

#### `0x1904` `stMarryStatusCheckCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byStep | 1 |

#### `0x1905` `stUnmarryCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1906` `stReqUserSortInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1907` `stRetUserSortInfoCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |

#### `0x1A01` `stAddMemberToSchoolCmd` · 总长 30 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | memberName | 1 |
| 15 | DWORD | memberID | 4 |
| 19 | DWORD | degree | 4 |
| 23 | char | schoolName | 1 |
| 24 | char | prename | 1 |
| 25 | BYTE | byState | 1 |
| 26 | WORD | wdOccupation | 2 |
| 28 | WORD | level | 2 |

#### `0x1A02` `stFireMemberFromSchoolCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | memberName | 1 |

#### `0x1A03` `stSchoolStatusCheckCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1A04` `stCreateSchoolCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | SchoolName | 1 |

#### `0x1A05` `stSchoolMemberLeaveSchoolCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1A06` `stSendListSchoolCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMasterTempID | 4 |
| 18 | BYTE | byPackageTag | 1 |
| 19 | WORD | size | 2 |

#### `0x1A07` `stSendMemberInfoCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |

#### `0x1A09` `stSchoolMemberStatusCmd` · 总长 24 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | BYTE | byState | 1 |
| 16 | WORD | level | 2 |
| 18 | DWORD | country | 4 |
| 22 | char | unionName | 1 |
| 23 | char | septName | 1 |

#### `0x1A0A` `stSchoolBulletinCmd` · 总长 17 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byState | 1 |
| 15 | WORD | wdSize | 2 |
| 17 | char | raw | 0 |

#### `0x1A0B` `stDestroySchoolCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1A0C` `stSchoolBulletinNotifyCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1A0D` `stReqMasterBountyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byState | 1 |

#### `0x1A0E` `stReqPrenticeBountyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byState | 1 |

#### `0x1A0F` `stRtnMasterBountyUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwBalance | 4 |
| 18 | DWORD | dwTotal | 4 |

#### `0x1A10` `stRtnPrenticeBountyUserCmd` · 总长 27 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwBalance | 4 |
| 18 | DWORD | dwTotal | 4 |
| 22 | DWORD | dwLastLevel | 4 |
| 26 | char | name | 1 |

#### `0x1B01` `stSeptStatusCheckCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1B02` `stCreateSeptCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | SeptName | 1 |

#### `0x1B03` `stServerReturnSeptCheckCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1B05` `stAllSeptMemberCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | WORD | size | 2 |

#### `0x1B07` `stAddMemberToSeptCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | memberName | 1 |
| 15 | DWORD | memberID | 4 |
| 19 | char | septName | 1 |
| 20 | BYTE | byState | 1 |

#### `0x1B08` `stFireMemberFromSeptCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | memberName | 1 |

#### `0x1B0B` `stSeptBaseInfoCmd` · 总长 283 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwMasterTempID | 4 |
| 18 | char | septName | 1 |
| 19 | char | master | 1 |
| 20 | char | raw | 255 |
| 275 | DWORD | dwRepute | 4 |
| 279 | DWORD | dwLevel | 4 |

#### `0x1B0C` `stSeptMemberLeaveSeptCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1B0D` `stVoteListSeptCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | septName | 1 |
| 15 | DWORD | dwSize | 4 |
| 19 | BYTE | flag | 1 |

#### `0x1B0E` `stVoteSeptCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | septName | 1 |
| 15 | BYTE | bySuccess | 1 |

#### `0x1B0F` `stNoteSeptCmd` · 总长 269 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | raw | 255 |

#### `0x1B14` `stListSeptCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |
| 18 | BYTE | flag | 1 |

#### `0x1B15` `stRequestJoinSeptCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | BYTE | status | 1 |

#### `0x1B16` `stRequestNpcInfoSeptCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1B17` `stRequestSeptExpCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1B18` `stRequestAbjurationSeptNpcSeptCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1B19` `stReqSeptNameCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSeptID | 4 |

#### `0x1B1A` `stRtnSeptNameCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSeptID | 4 |
| 18 | char | name | 1 |

#### `0x1B1B` `stReqSeptNameListCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | num | 1 |
| 15 | DWORD | dwSeptID | 4 |

#### `0x1B1C` `stRtnSeptNameListCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | num | 1 |
| 15 | DWORD | dwSeptID | 4 |
| 19 | char | name | 1 |

#### `0x1B1D` `stRequestSeptNormalExpCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1C01` `stActiveDareCmd` · 总长 30 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | char | fromRelationName | 1 |
| 16 | char | toRelationName | 1 |
| 17 | DWORD | dwWarID | 4 |
| 21 | DWORD | dwDareType | 4 |
| 25 | DWORD | dwMsgType | 4 |
| 29 | BYTE | byDareRepute | 1 |

#### `0x1D01` `stSetAIPetCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1D02` `stRequestDataPetCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1D03` `stRefreshDataPetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D04` `stChangeNamePetCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x1D05` `stAddPetPetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D06` `stDelPetPetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D07` `stDelSkillPetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D08` `stCallHorsePetCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1D09` `stPutHorsePetCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x1D0A` `stHorseDataPetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D0B` `stSetTargetPetCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | targetType | 4 |
| 18 | DWORD | id | 4 |

#### `0x1D0B` `stChangeHorsePetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D0C` `stHpExpPetCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |
| 18 | DWORD | hp | 4 |
| 22 | DWORD | exp | 4 |

#### `0x1D0D` `stCallFabaoPetCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x1D0E` `stPutFabaoPetCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2501` `stBuyCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | npcID | 4 |

#### `0x2502` `stAddCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x2503` `stRemoveCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x2504` `stFollowCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x2505` `stLetOutCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x2506` `stAdoptCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x2507` `stReturnCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x2508` `stGetBackCartoonCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2509` `stDrawExpCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x250A` `stChangeNameCartoonCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | char | name | 1 |

#### `0x250B` `stAddWaitingCartoonCmd` · 总长 29 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | DWORD | npcID | 4 |
| 22 | char | name | 1 |
| 23 | char | masterName | 1 |
| 24 | BYTE | state | 1 |
| 25 | DWORD | time | 4 |

#### `0x250C` `stRemoveWaitingCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x250D` `stRequestListCartoonCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x250E` `stSaleCartoonCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |

#### `0x250F` `stChargeCartoonCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | DWORD | time | 4 |

#### `0x2510` `stHpExpCartoonCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | DWORD | sp | 4 |
| 22 | DWORD | exp | 4 |

#### `0x2511` `stSellAllCartoonCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2512` `stConsignCartoonCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | char | name | 1 |

#### `0x2513` `stConsignRetCartoonCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | BYTE | ret | 1 |

#### `0x2514` `stRepairCartoonCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | cartoonID | 4 |
| 18 | BYTE | repair | 1 |

#### `0x2605` `stTransferFundStockSaveUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwGold | 4 |
| 18 | DWORD | dwMoney | 4 |

#### `0x2701` `stPassowrdStockUserCmd` · 总长 17 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | byPawword | 1 |
| 15 | char | byNew1 | 1 |
| 16 | char | byNew2 | 1 |

#### `0x2702` `stReturnPasswordStockIserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byReturn | 1 |

#### `0x2703` `stQueryFundStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2704` `stReturnFundStockUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwGold | 4 |
| 18 | DWORD | dwMoney | 4 |

#### `0x2706` `stTransferFundStockFetchUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwGold | 4 |
| 18 | DWORD | dwMoney | 4 |

#### `0x2707` `stConsignGoldStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNum | 4 |

#### `0x2708` `stConsignMoneyStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNum | 4 |

#### `0x2709` `stConsignCancelGoldStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNum | 4 |

#### `0x270A` `stConsignCancelMoneyStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNum | 4 |

#### `0x270B` `stRequestConsignListStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x270C` `stConsignGoldListStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |

#### `0x270D` `stConsignMoneyListStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |

#### `0x270E` `stRequestCancelListStockUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |
| 18 | BYTE | byType | 1 |

#### `0x270F` `stReturnCancelListStockUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |
| 18 | BYTE | byType | 1 |
| 19 | BYTE | byReturn | 1 |

#### `0x2710` `stRequestFirstTenListStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2711` `stFirstFiveListStockUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |
| 15 | DWORD | size | 4 |

#### `0x2712` `stPutListStockUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwNum | 4 |
| 18 | DWORD | dwPrice | 4 |
| 22 | BYTE | byType | 1 |

#### `0x2713` `stReturnPutListStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2714` `stRequstHistoryStockUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | begintime | 4 |
| 18 | DWORD | num | 4 |

#### `0x2715` `stNewHistoryStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2716` `stHistoryGoldStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |

#### `0x2717` `stHistoryMoneyStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |

#### `0x2718` `stRequstSelfHistoryStockUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | begintime | 4 |
| 18 | DWORD | num | 4 |

#### `0x2719` `stSelfHistoryGoldStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |

#### `0x271A` `stSelfHistoryMoneyStockUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | size | 4 |

#### `0x271B` `stStopServiceStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x271C` `stConsignCleanListStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x271D` `stRequestStateStockUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2801` `stRequestVoteListUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x2802` `stReturnVoteListUserCmd` · 总长 23 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwVoteID | 4 |
| 18 | BYTE | byType | 1 |
| 19 | DWORD | dwSize | 4 |

#### `0x2803` `stCommitVoteUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwVoteID | 4 |
| 18 | DWORD | dwOption | 4 |

#### `0x2901` `stCreateArmyUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | armyName | 1 |
| 15 | char | genName | 1 |

#### `0x2902` `stReqWaitGenUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2902` `stRtnWaitGenUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |

#### `0x2904` `stReqArmyGenUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byStatus | 1 |

#### `0x2905` `stReqArmyListUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byType | 1 |

#### `0x2906` `stRtnArmyListUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwSize | 4 |
| 18 | BYTE | byType | 1 |

#### `0x2907` `stReqArmySpecUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwArmyID | 4 |

#### `0x2908` `stRtnArmySpecUserCmd` · 总长 21 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |
| 15 | char | cityname | 1 |
| 16 | char | genname | 1 |
| 17 | DWORD | dwSize | 4 |

#### `0x2909` `stExitArmyUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x290A` `stRemoveArmyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwArmyID | 4 |

#### `0x290B` `stChangeArmyNameUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwArmyID | 4 |
| 18 | char | newArmyName | 1 |

#### `0x290C` `stAddArmyCaptainUserCmd` · 总长 25 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | capName | 1 |
| 15 | char | armyName | 1 |
| 16 | DWORD | armyID | 4 |
| 20 | DWORD | capID | 4 |
| 24 | BYTE | byState | 1 |

#### `0x290C` `stChangeArmyGenUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwArmyID | 4 |
| 18 | char | newArmyGen | 1 |

#### `0x290D` `stFireArmyCaptainUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwUserID | 4 |

#### `0x2A01` `stReqDragonUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2A02` `stReqTigerUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x2F01` `stSetRecommendUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | name | 1 |

#### `0x2F02` `stReqRecommendBountyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byState | 1 |

#### `0x2F03` `stReqRecommendedBountyUserCmd` · 总长 15 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byState | 1 |

#### `0x2F04` `stRtnRecommendBountyUserCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwBalance | 4 |
| 18 | DWORD | dwTotal | 4 |
| 22 | DWORD | dwSize | 4 |

#### `0x2F05` `stRtnRecommendedBountyUserCmd` · 总长 27 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | dwBalance | 4 |
| 18 | DWORD | dwTotal | 4 |
| 22 | DWORD | dwLastLevel | 4 |
| 26 | char | name | 1 |

#### `0x3001` `stSetSafetyUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | pwd | 4 |
| 18 | BYTE | state | 1 |

#### `0x3002` `stNotifySafetyUserCmd` · 总长 16 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | BYTE | byState | 1 |
| 15 | BYTE | safe_setup | 1 |

#### `0x3003` `stSetTempUnSafetyUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | pwd | 4 |

#### `0x3004` `stSetSafetyDetailUserCmd` · 总长 19 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | pwd | 4 |
| 18 | BYTE | safe_setup | 1 |

#### `0x3005` `stSetSafetyPasswordUserCmd` · 总长 20 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | accid | 4 |
| 18 | char | oldPwd | 1 |
| 19 | char | newPwd | 1 |

#### `0x3301` `stSetCowBoxKeyCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | Key_id | 4 |
| 18 | DWORD | qwThisID | 4 |

#### `0x3302` `stGetCowBoxInitCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | int | item_Ti | 4 |
| 18 | DWORD | Key_id | 4 |

#### `0x3303` `stSetCowBoxTiquCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3401` `stTurnRequestCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3402` `stTurnResultCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3501` `stHorseTrainingUserCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | int | CmdTey | 4 |

#### `0x3502` `stHouseFaceTrainingUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | int | CmdTey | 4 |
| 18 | int | FaceTey | 4 |

#### `0x3601` `stLuaInputBoxUserCmd` · 总长 264 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | raw | 200 |
| 214 | char | raw | 50 |

#### `0x3801` `stFujianJianzhongUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3805` `stFujianZtzUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3807` `stFujianShengqiUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x380A` `stFujianShengxiaoUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3811` `stFujianJingmaiUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3815` `stFujianTouxianUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3901` `stLianzhanstartUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | lianzhantime | 4 |
| 18 | DWORD | lianzhannum | 4 |

#### `0x3A01` `stJingcaiXiazhuCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | int | index | 4 |
| 18 | DWORD | itemUID | 4 |

#### `0x3A02` `stJingcaiInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3A03` `stJingcaiReturnInfoCmd` · 总长 30 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | int | index | 4 |
| 18 | int | object_id | 4 |
| 22 | int | object_level | 4 |
| 26 | int | object_num | 4 |

#### `0x3A04` `stJingcaiReturnPigTimeCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3A05` `stJingcaiReturnKillPigTimeCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3B01` `stFubenInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3B02` `stFubenReturnInfoCmd` · 总长 34 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | fba1 | 4 |
| 18 | DWORD | fba2 | 4 |
| 22 | DWORD | fba3 | 4 |
| 26 | DWORD | fba4 | 4 |
| 30 | DWORD | fba5 | 4 |

#### `0x3B03` `stFubenInCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | type | 4 |
| 18 | DWORD | select | 4 |

#### `0x3B04` `stFubenBuyCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | select | 4 |

#### `0x3C01` `stErnvInfoUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3C03` `stErnvPutUserCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | babytype | 4 |
| 18 | DWORD | state | 4 |

#### `0x3D01` `stTianxiaInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3D02` `stTianxiaReturnInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3D03` `stTianxiaBaomingCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3D04` `stTianxiaBaominBackCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3D05` `stTianxiaDuanweiCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3E01` `stBikeStartCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3E02` `stDelBikeCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3E03` `stBikeScnlCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | npcid | 4 |

#### `0x3E04` `stBikeSkillCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | type | 4 |
| 18 | DWORD | playerid | 4 |
| 22 | DWORD | itemID | 4 |

#### `0x3E05` `stBikeAdditemCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemID | 4 |

#### `0x3E06` `stBikeStateCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x3F01` `stBieShuInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3F02` `stBieShuInfoReturnCmd` · 总长 38 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | bieshu_level | 4 |
| 18 | DWORD | bieshu_exp | 4 |
| 22 | DWORD | bieshu_state | 4 |
| 26 | DWORD | bieshu_gong | 4 |
| 30 | DWORD | bieshu_fang | 4 |
| 34 | DWORD | bieshu_hp | 4 |

#### `0x3F03` `stBieShuInCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x3F04` `stBieShuJiHuoCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemID | 4 |

#### `0x3F05` `stBieShuJiHuoReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x3F06` `stBieShuLevelUpCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemID | 4 |

#### `0x3F07` `stBieShuLevenUpReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x4001` `stFengyinInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4002` `stFengyinInfoReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |

#### `0x4101` `stHuishouInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4102` `stHuishouInfoReturnCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4201` `stZhuanhuanCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemUID | 4 |
| 18 | DWORD | zhiye | 4 |
| 22 | DWORD | type | 4 |

#### `0x4202` `stZhuanhuanReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x4301` `stXiulianInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4302` `stXiulianInfoReturnCmd` · 总长 98 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | xiulianexp | 4 |
| 18 | DWORD | level1 | 4 |
| 22 | DWORD | level2 | 4 |
| 26 | DWORD | level3 | 4 |
| 30 | DWORD | level4 | 4 |
| 34 | DWORD | level5 | 4 |
| 38 | DWORD | exp1 | 4 |
| 42 | DWORD | exp2 | 4 |
| 46 | DWORD | exp3 | 4 |
| 50 | DWORD | exp4 | 4 |
| 54 | DWORD | exp5 | 4 |
| 58 | DWORD | money1 | 4 |
| 62 | DWORD | money2 | 4 |
| 66 | DWORD | money3 | 4 |
| 70 | DWORD | money4 | 4 |
| 74 | DWORD | money5 | 4 |
| 78 | DWORD | num1 | 4 |
| 82 | DWORD | num2 | 4 |
| 86 | DWORD | num3 | 4 |
| 90 | DWORD | num4 | 4 |
| 94 | DWORD | num5 | 4 |

#### `0x4303` `stXiulianStartCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | type | 4 |

#### `0x4304` `stXiulianReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x4305` `stXiulianResetCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4501` `stZhancheListCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4502` `stZhancheListReturnCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4503` `stZhancheLingyongCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | id | 4 |

#### `0x4504` `stZhancheLingyongReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x4505` `stZhancheAddCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4506` `stZhancheTihuanCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4507` `stZhancheShouhuiCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4508` `stZhancheZhaohuanCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4509` `stZhancheGuihuanCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x450A` `stZhancheTuoliCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x450B` `stZhancheSkillCmd` · 总长 26 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | type | 4 |
| 18 | DWORD | playerid | 4 |
| 22 | DWORD | itemID | 4 |

#### `0x4601` `stJiazuBossInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4602` `stJiazuBossInfoReturnCmd` · 总长 22 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |
| 18 | DWORD | exp | 4 |

#### `0x4603` `stJiazuBossWeiyangCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemid | 4 |

#### `0x4604` `stJiazuBossWeiyangReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x4605` `stJiazuBossZhaohuanCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4701` `stZuoqiInfoCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4702` `stZuoqiInfoReturnCmd` · 总长 82 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | level | 4 |
| 18 | DWORD | exp | 4 |
| 22 | DWORD | num1 | 4 |
| 26 | DWORD | num2 | 4 |
| 30 | DWORD | num3 | 4 |
| 34 | DWORD | num4 | 4 |
| 38 | DWORD | num5 | 4 |
| 42 | DWORD | num6 | 4 |
| 46 | DWORD | num7 | 4 |
| 50 | DWORD | num8 | 4 |
| 54 | DWORD | num9 | 4 |
| 58 | DWORD | num10 | 4 |
| 62 | DWORD | num11 | 4 |
| 66 | DWORD | num12 | 4 |
| 70 | DWORD | num13 | 4 |
| 74 | DWORD | num14 | 4 |
| 78 | DWORD | num15 | 4 |

#### `0x4703` `stZuoqiLevelUpCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | itemid | 4 |

#### `0x4704` `stZuoqiLevelUpReturnCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | state | 4 |

#### `0x4705` `stZuoqiLingQuCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4801` `stMigongNextCmd` · 总长 18 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | DWORD | npcid | 4 |

#### `0x4901` `stReqKillerBossInfoUserCmd` · 总长 14 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |

#### `0x4902` `stRunKillerBossInfoUserCmd` · 总长 39 字节

| 偏移 | 类型 | 字段 | 字节 |
|---|---|---|---|
| 0 | BYTE | byCmd | 1 |
| 1 | BYTE | byParam | 1 |
| 2 | DWORD | dwTimestamp | 4 |
| 6 | BYTE | byCmd | 1 |
| 7 | BYTE | byParam | 1 |
| 8 | BYTE | byCmdType | 1 |
| 9 | BYTE | byParameterType | 1 |
| 10 | DWORD | dwTimestamp | 4 |
| 14 | char | MapName | 1 |
| 15 | WORD | TypeID | 2 |
| 17 | WORD | Pagenum | 2 |
| 19 | WORD | Number | 2 |
| 21 | DWORD | NpcID | 4 |
| 25 | DWORD | CountryID | 4 |
| 29 | WORD | NpcState | 2 |
| 31 | QWORD | ReviveTime | 8 |
