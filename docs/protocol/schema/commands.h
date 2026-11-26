/* 由 tools/extract_msgid.py 从 src/server/base/Command.h 生成，勿手改 */
#pragma once
#include <stdint.h>
#pragma pack(push, 1)

typedef struct { uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp; } ZtCmdHeader;

/* 0x0101 USERVERIFYVER total=18 line=312 */
typedef struct stUserVerifyVerCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD version[4];
} stUserVerifyVerCmd_t;

/* 0x0102 USERREQUESTLOGIN total=40 line=325 */
typedef struct stUserRequestLoginCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char pstrName[1];
    /* +1 */ char pstrPassword[1];
    /* +2 */ WORD game[2];
    /* +2 */ WORD zone[2];
    /* +7 */ char raw[7];
    /* +13 */ char raw[13];
} stUserRequestLoginCmd_t;

/* 0x0103 SERVERRETURNLOGINFAILED total=15 line=365 */
typedef struct stServerReturnLoginFailedCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byReturnCode[1];
} stServerReturnLoginFailedCmd_t;

/* 0x0104 SERVERRETURNLOGINSUCCESS total=28 line=376 */
typedef struct stServerReturnLoginSuccessCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserID[4];
    /* +4 */ DWORD loginTempID[4];
    /* +1 */ char pstrIP[1];
    /* +2 */ WORD wdPort[2];
    /* +1 */ BYTE randnum[1];
    /* +1 */ BYTE keyOffset[1];
    /* +1 */ BYTE key[1];
} stServerReturnLoginSuccessCmd_t;

/* 0x0105 PASSWDLOGONUSER total=24 line=401 */
typedef struct stPasswdLogonUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD loginTempID[4];
    /* +4 */ DWORD dwUserID[4];
    /* +1 */ char pstrName[1];
    /* +1 */ char pstrPassword[1];
} stPasswdLogonUserCmd_t;

/* 0x0107 ACCOUNTLOGONUSER total=16 line=416 */
typedef struct stAccountLogonUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char strName[1];
    /* +1 */ char strPassword[1];
} stAccountLogonUserCmd_t;

/* 0x0109 PASSWORDLOGONUSER total=17 line=429 */
typedef struct stPasswordLogonUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char strName[1];
    /* +1 */ char strPassword[1];
    /* +1 */ char strNewPassword[1];
} stPasswordLogonUserCmd_t;

/* 0x010A BACKSELECTUSER total=14 line=442 */
typedef struct stBackSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stBackSelectUserCmd_t;

/* 0x010B JPEGPASSPORTUSER total=17 line=452 */
typedef struct stJpegPassportUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
    /* +1 */ BYTE data[1];
} stJpegPassportUserCmd_t;

/* 0x010C COUNTRYINFOUSER total=16 line=478 */
typedef struct stCountryInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
} stCountryInfoUserCmd_t;

/* 0x0201 GAMETIMETIMERUSER total=22 line=507 */
typedef struct stGameTimeTimerUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +8 */ QWORD qwGameTime[8];
} stGameTimeTimerUserCmd_t;

/* 0x0202 REQUESTUSERGAMETIMETIMERUSER total=14 line=520 */
typedef struct stRequestUserGameTimeTimerUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestUserGameTimeTimerUserCmd_t;

/* 0x0203 USERGAMETIMETIMERUSER total=26 line=531 */
typedef struct stUserGameTimeTimerUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +8 */ QWORD qwGameTime[8];
} stUserGameTimeTimerUserCmd_t;

/* 0x0204 PINGTIMETIMERUSER total=14 line=544 */
typedef struct stPingTimeTimerUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stPingTimeTimerUserCmd_t;

/* 0x0301 MAINUSERDATAUSER total=14 line=1403 */
typedef struct stMainUserDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stMainUserDataUserCmd_t;

/* 0x0302 DECREMAINDERDATAUSER total=15 line=1421 */
typedef struct stDecRemainderDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byDecType[1];
} stDecRemainderDataUserCmd_t;

/* 0x0303 SETHPANDMPDATAUSER total=14 line=1434 */
typedef struct stSetHPAndMPDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSetHPAndMPDataUserCmd_t;

/* 0x031C MAPSCREENSIZEDATAUSER total=45 line=1451 */
typedef struct stMapScreenSizeDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD width[4];
    /* +4 */ DWORD height[4];
    /* +1 */ char pstrMapName[1];
    /* +1 */ char pstrFilename[1];
    /* +4 */ DWORD setting[4];
    /* +4 */ DWORD rgb[4];
    /* +1 */ char pstrGroupName[1];
    /* +1 */ char pstrCountryName[1];
    /* +1 */ char pstrCityName[1];
    /* +4 */ int mainRoleX[4];
    /* +4 */ int mainRoleY[4];
    /* +2 */ short npc_count[2];
} stMapScreenSizeDataUserCmd_t;

/* 0x031D LOADMAPOKDATAUSER total=14 line=1476 */
typedef struct stLoadMapOKDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stLoadMapOKDataUserCmd_t;

/* 0x031E LEVELDEGREEDATAUSER total=16 line=1485 */
typedef struct stLevelDegreeDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD degree[2];
} stLevelDegreeDataUserCmd_t;

/* 0x0333 STAMPDATAUSER total=19 line=1495 */
typedef struct stStampDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
    /* +1 */ BYTE bin[1];
} stStampDataUserCmd_t;

/* 0x0334 STAMPCHECKUSER total=18 line=1511 */
typedef struct stStampCheckUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD flag[4];
} stStampCheckUserCmd_t;

/* 0x0335 MERGEVERSIONCHECKUSER total=18 line=1524 */
typedef struct stMergeVersionCheckUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMergeVersion[4];
} stMergeVersionCheckUserCmd_t;

/* 0x03FF ENDOFINITDATADATAUSER total=14 line=1538 */
typedef struct stEndOfInitDataDataUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stEndOfInitDataDataUserCmd_t;

/* 0x0401 ADDOBJECTPROPERTYUSER total=15 line=3029 */
typedef struct stAddObjectPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byActionType[1];
} stAddObjectPropertyUserCmd_t;

/* 0x0403 SWAPOBJECTPROPERTYUSER total=18 line=3050 */
typedef struct stSwapObjectPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD qwThisID[4];
} stSwapObjectPropertyUserCmd_t;

/* 0x0406 REFCOUNTOBJECTPROPERTYUSER total=22 line=3085 */
typedef struct stRefCountObjectPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD qwThisID[4];
    /* +4 */ DWORD dwNum[4];
} stRefCountObjectPropertyUserCmd_t;

/* 0x0407 USEOBJECTPROPERTYUSER total=22 line=3096 */
typedef struct stUseObjectPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD qwThisID[4];
    /* +4 */ DWORD dwNumber[4];
} stUseObjectPropertyUserCmd_t;

/* 0x0408 PICKUPITEMPROPERTYUSER total=22 line=3107 */
typedef struct stPickUpItemPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD x[4];
    /* +4 */ DWORD y[4];
} stPickUpItemPropertyUserCmd_t;

/* 0x0409 FOUNDITEMPROPERTYUSER total=28 line=3121 */
typedef struct stFoundItemPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD up_id[4];
    /* +4 */ DWORD count[4];
    /* +4 */ DWORD gem_id[4];
    /* +1 */ BYTE gem_level[1];
    /* +1 */ BYTE gem_num[1];
} stFoundItemPropertyUserCmd_t;

/* 0x040A UPGRADEITEMPROPERTYUSER total=15 line=3137 */
typedef struct stUpgradeItemPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE location[1];
} stUpgradeItemPropertyUserCmd_t;

/* 0x040B COMPOSEITEMPROPERTYUSER total=16 line=3146 */
typedef struct stComposeItemPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE per_num[1];
    /* +1 */ BYTE location[1];
} stComposeItemPropertyUserCmd_t;

/* 0x040C ACCEKKEYPROPERTYUSER total=17 line=3177 */
typedef struct stAccekKeyPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD accelNum[2];
    /* +1 */ BYTE activeGroup[1];
} stAccekKeyPropertyUserCmd_t;

/* 0x040D ADDUSERSKILLPROPERTYUSER total=30 line=3188 */
typedef struct stAddUserSkillPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSkillID[4];
    /* +2 */ WORD wdLevel[2];
    /* +2 */ WORD wdUpNum[2];
    /* +4 */ DWORD dwExperience[4];
    /* +4 */ DWORD dwMaxExperience[4];
} stAddUserSkillPropertyUserCmd_t;

/* 0x040E UPGRADEUSERSKILLPROPERTYUSER total=20 line=3204 */
typedef struct stUpgradeUserSkillPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSkillID[4];
    /* +2 */ WORD wdLevel[2];
} stUpgradeUserSkillPropertyUserCmd_t;

/* 0x0410 SYSTEMSETTINGSUSER total=14 line=3307 */
typedef struct stSystemSettingsUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSystemSettingsUserCmd_t;

/* 0x0411 DISTRIBUTEPOINTPROPERTYUSER total=16 line=3326 */
typedef struct stDistributePointPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD type[2];
} stDistributePointPropertyUserCmd_t;

/* 0x0412 ADDSKILLPOINTPROPERTYUSER total=18 line=3338 */
typedef struct stAddSkillPointPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSkillID[4];
} stAddSkillPointPropertyUserCmd_t;

/* 0x0413 CLEARPOINTPROPERTYUSER total=18 line=3349 */
typedef struct stClearPointPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwItemID[4];
} stClearPointPropertyUserCmd_t;

/* 0x0413 MAKEODDSPROPERTYUSER total=18 line=3371 */
typedef struct stMakeOddsPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD odds[4];
} stMakeOddsPropertyUserCmd_t;

/* 0x0414 MAKESELECTPROPERTYUSER total=18 line=3382 */
typedef struct stMakeSelectPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stMakeSelectPropertyUserCmd_t;

/* 0x0415 COMPOSESOULSTONEPROPERTYUSER total=15 line=3393 */
typedef struct stComposeSoulStonePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE location[1];
} stComposeSoulStonePropertyUserCmd_t;

/* 0x0416 ENCHASEPROPERTYUSER total=15 line=3403 */
typedef struct stEnchasePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE location[1];
} stEnchasePropertyUserCmd_t;

/* 0x0417 HOLEPROPERTYUSER total=24 line=3412 */
typedef struct stHolePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD up_id[4];
    /* +4 */ DWORD gem_id[4];
    /* +1 */ BYTE gem_level[1];
    /* +1 */ BYTE gem_num[1];
} stHolePropertyUserCmd_t;

/* 0x0418 DECOMPOSEPROPERTYUSER total=18 line=3440 */
typedef struct stDecomposePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD up_id[4];
} stDecomposePropertyUserCmd_t;

/* 0x0419 BODYCOLORPROPERTYUSER total=18 line=3449 */
typedef struct stBodyColorPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwBodyColorCustom[4];
} stBodyColorPropertyUserCmd_t;

/* 0x041A HAIRPROPERTYUSER total=19 line=3459 */
typedef struct stHairPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byHairType[1];
    /* +4 */ DWORD dwHairColor[4];
} stHairPropertyUserCmd_t;

/* 0x041B SELECTPROPERTYUSER total=24 line=3470 */
typedef struct stSelectPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
    /* +1 */ BYTE byOldType[1];
    /* +4 */ DWORD dwOldTempID[4];
} stSelectPropertyUserCmd_t;

/* 0x041D SELECTMAINUSERPROPERTYUSER total=19 line=3483 */
typedef struct stSelectMainUserPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectMainUserPropertyUserCmd_t;

/* 0x041F DURABILITYUSER total=26 line=3533 */
typedef struct stDurabilityUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwThisID[4];
    /* +4 */ DWORD dwDur[4];
    /* +4 */ DWORD dwMaxDur[4];
} stDurabilityUserCmd_t;

/* 0x0421 CLEARSKILLUSER total=14 line=3568 */
typedef struct stClearSkillUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stClearSkillUserCmd_t;

/* 0x0422 FACEPROPERTYUSER total=18 line=3576 */
typedef struct stFacePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwFace[4];
} stFacePropertyUserCmd_t;

/* 0x0423 HONORTOPKVALUEPROPERTYUSER total=14 line=3585 */
typedef struct stHonorToPkValuePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stHonorToPkValuePropertyUserCmd_t;

/* 0x0424 SCROLLMAPPROPERTYUSER total=23 line=3594 */
typedef struct stScrollMapPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD qwThisID[4];
    /* +4 */ DWORD size[4];
    /* +1 */ char strMapName[1];
} stScrollMapPropertyUserCmd_t;

/* 0x0425 CLEARPOINTLIMITPROPERTYUSER total=23 line=3610 */
typedef struct stClearPointLimitPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +8 */ QWORD qwThisID[8];
    /* +1 */ BYTE byProperty[1];
} stClearPointLimitPropertyUserCmd_t;

/* 0x0426 REQUESTINCMAPPROPERTYUSER total=14 line=3622 */
typedef struct stRequestIncMapPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestIncMapPropertyUserCmd_t;

/* 0x0427 SELECTRETURNHORSEPROPERTYUSER total=14 line=3683 */
typedef struct stSelectReturnHorsePropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSelectReturnHorsePropertyUserCmd_t;

/* 0x0428 GOTRAINPROPERTYUSER total=18 line=3693 */
typedef struct stGoTrainPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
} stGoTrainPropertyUserCmd_t;

/* 0x042D MODIFYPROPERTYUSER total=25 line=3852 */
typedef struct stModifyPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
    /* +4 */ DWORD up_id[4];
    /* +4 */ DWORD gem_id[4];
    /* +1 */ BYTE gem_level[1];
    /* +1 */ BYTE gem_num[1];
} stModifyPropertyUserCmd_t;

/* 0x042E MAKEZSPROPERTYUSER total=25 line=3867 */
typedef struct stMakezsPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
    /* +4 */ DWORD up_id[4];
    /* +4 */ DWORD gem_id[4];
    /* +1 */ BYTE gem_level[1];
    /* +1 */ BYTE gem_num[1];
} stMakezsPropertyUserCmd_t;

/* 0x042F MAKEZQPROPERTYUSER total=25 line=3882 */
typedef struct stMakezqPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
    /* +4 */ DWORD up_id[4];
    /* +4 */ DWORD gem_id[4];
    /* +1 */ BYTE gem_level[1];
    /* +1 */ BYTE gem_num[1];
} stMakezqPropertyUserCmd_t;

/* 0x0430 MAKELVPROPERTYUSER total=25 line=3897 */
typedef struct stMakelvPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
    /* +4 */ DWORD up_id[4];
    /* +4 */ DWORD gem_id[4];
    /* +1 */ BYTE gem_level[1];
    /* +1 */ BYTE gem_num[1];
} stMakelvPropertyUserCmd_t;

/* 0x0432 ITEMUSEITEMSUCCESSUSER total=18 line=3925 */
typedef struct stItemUseItemSuccessUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD ItemID[4];
} stItemUseItemSuccessUserCmd_t;

/* 0x0433 GOFUBENPROPERTYUSER total=18 line=3937 */
typedef struct stGoFubenPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
} stGoFubenPropertyUserCmd_t;

/* 0x0437 ITEM6USEITEMSUCCESSUSER total=18 line=3948 */
typedef struct stItem6UseItemSuccessUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD ItemID[4];
} stItem6UseItemSuccessUserCmd_t;

/* 0x0438 ITEM7USEITEMSUCCESSUSER total=18 line=3960 */
typedef struct stItem7UseItemSuccessUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD ItemID[4];
} stItem7UseItemSuccessUserCmd_t;

/* 0x043C ITEM11USEITEMSUCCESSUSER total=18 line=3971 */
typedef struct stItem11UseItemSuccessUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD ItemID[4];
} stItem11UseItemSuccessUserCmd_t;

/* 0x043D SELECTFLOWERSPROPERTYUSER total=19 line=3984 */
typedef struct stSelectFlowersPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowersPropertyUserCmd_t;

/* 0x043E SELECTFLOWERS1PROPERTYUSER total=19 line=3996 */
typedef struct stSelectFlowers1PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers1PropertyUserCmd_t;

/* 0x043F SELECTFLOWERS2PROPERTYUSER total=19 line=4008 */
typedef struct stSelectFlowers2PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers2PropertyUserCmd_t;

/* 0x0440 SELECTFLOWERS3PROPERTYUSER total=19 line=4020 */
typedef struct stSelectFlowers3PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers3PropertyUserCmd_t;

/* 0x0441 SELECTFLOWERS4PROPERTYUSER total=19 line=4032 */
typedef struct stSelectFlowers4PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers4PropertyUserCmd_t;

/* 0x0442 SELECTFLOWERS5PROPERTYUSER total=19 line=4044 */
typedef struct stSelectFlowers5PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers5PropertyUserCmd_t;

/* 0x0443 SELECTFLOWERS6PROPERTYUSER total=19 line=4056 */
typedef struct stSelectFlowers6PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers6PropertyUserCmd_t;

/* 0x0444 SELECTFLOWERS7PROPERTYUSER total=19 line=4068 */
typedef struct stSelectFlowers7PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers7PropertyUserCmd_t;

/* 0x0446 SELECTFLOWERS20PROPERTYUSER total=19 line=4091 */
typedef struct stSelectFlowers20PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers20PropertyUserCmd_t;

/* 0x0447 SELECTFLOWERS21PROPERTYUSER total=19 line=4103 */
typedef struct stSelectFlowers21PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers21PropertyUserCmd_t;

/* 0x0448 SELECTFLOWERS22PROPERTYUSER total=19 line=4115 */
typedef struct stSelectFlowers22PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers22PropertyUserCmd_t;

/* 0x0449 SELECTFLOWERS23PROPERTYUSER total=19 line=4127 */
typedef struct stSelectFlowers23PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers23PropertyUserCmd_t;

/* 0x044A SELECTFLOWERS41PROPERTYUSER total=19 line=4139 */
typedef struct stSelectFlowers41PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers41PropertyUserCmd_t;

/* 0x044B SELECTFLOWERS51PROPERTYUSER total=19 line=4151 */
typedef struct stSelectFlowers51PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers51PropertyUserCmd_t;

/* 0x044C SELECTFLOWERS61PROPERTYUSER total=19 line=4163 */
typedef struct stSelectFlowers61PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers61PropertyUserCmd_t;

/* 0x044D SELECTFLOWERS71PROPERTYUSER total=19 line=4175 */
typedef struct stSelectFlowers71PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stSelectFlowers71PropertyUserCmd_t;

/* 0x044E SCROLL2MAPPROPERTYUSER total=31 line=4187 */
typedef struct stScroll2MapPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD qwThisID[4];
    /* +4 */ DWORD xPos[4];
    /* +4 */ DWORD yPos[4];
    /* +4 */ DWORD size[4];
    /* +1 */ char strMapName[1];
} stScroll2MapPropertyUserCmd_t;

/* 0x0452 ONEKEYEQUIPPROPERTYUSER total=18 line=4214 */
typedef struct stOneKeyEquipPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
} stOneKeyEquipPropertyUserCmd_t;

/* 0x0453 GETONEKEYEQUIPPROPERTYUSER total=18 line=4228 */
typedef struct stGetOneKeyEquipPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
} stGetOneKeyEquipPropertyUserCmd_t;

/* 0x0455 GOHANBINGPROPERTYUSER total=18 line=4251 */
typedef struct stGoHanbingPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
} stGoHanbingPropertyUserCmd_t;

/* 0x0456 GOANGELPROPERTYUSER total=18 line=4262 */
typedef struct stGoAngelPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
} stGoAngelPropertyUserCmd_t;

/* 0x0457 GOCHUANSONGPROPERTYUSER total=14 line=4273 */
typedef struct stGoChuanSongPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGoChuanSongPropertyUserCmd_t;

/* 0x0458 GOCHUANSONG2PROPERTYUSER total=14 line=4284 */
typedef struct stGoChuanSong2PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGoChuanSong2PropertyUserCmd_t;

/* 0x0459 GETCHENGHAOPROPERTYUSER total=18 line=4329 */
typedef struct stGetChenghaoPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
} stGetChenghaoPropertyUserCmd_t;

/* 0x045A SETCHENGHAOUSER total=22 line=4357 */
typedef struct stSetChenghaoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD chenghaoid[4];
} stSetChenghaoUserCmd_t;

/* 0x045B SETNOTCHENGHAOUSER total=22 line=4369 */
typedef struct stSetNotChenghaoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD chenghaoid[4];
} stSetNotChenghaoUserCmd_t;

/* 0x045C GETSHIZHUANGPROPERTYUSER total=18 line=4382 */
typedef struct stGetShizhuangPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
} stGetShizhuangPropertyUserCmd_t;

/* 0x045E SETSHIZHUANGUSER total=22 line=4459 */
typedef struct stSetShizhuangUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD shizhuangid[4];
} stSetShizhuangUserCmd_t;

/* 0x045F SETNOTSHIZHUANGUSER total=22 line=4471 */
typedef struct stSetNotShizhuangUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD shizhuangid[4];
} stSetNotShizhuangUserCmd_t;

/* 0x0460 SETSHIZHUANGJIHUOUSER total=22 line=4484 */
typedef struct stSetShizhuangJihuoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD shizhuangid[4];
} stSetShizhuangJihuoUserCmd_t;

/* 0x0461 MEIRIRENWUCHUANSONGUSER total=22 line=4613 */
typedef struct stMeiRiRenWuChuanSongUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD num[4];
} stMeiRiRenWuChuanSongUserCmd_t;

/* 0x0462 GOCHUANSONG3PROPERTYUSER total=14 line=4294 */
typedef struct stGoChuanSong3PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGoChuanSong3PropertyUserCmd_t;

/* 0x0463 GOCHUANSONG4PROPERTYUSER total=14 line=4302 */
typedef struct stGoChuanSong4PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGoChuanSong4PropertyUserCmd_t;

/* 0x0465 SETPIFENGUSER total=22 line=4497 */
typedef struct stSetPifengUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD pifengid[4];
} stSetPifengUserCmd_t;

/* 0x0466 SETNOTPIFENGUSER total=22 line=4509 */
typedef struct stSetNotPifengUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD pifengid[4];
} stSetNotPifengUserCmd_t;

/* 0x0467 SETPIFENGJIHUOUSER total=22 line=4522 */
typedef struct stSetPifengJihuoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD pifengid[4];
} stSetPifengJihuoUserCmd_t;

/* 0x0468 SETCHIBANGUSER total=22 line=4535 */
typedef struct stSetChibangUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD chibangid[4];
} stSetChibangUserCmd_t;

/* 0x0469 SETNOTCHIBANGUSER total=22 line=4547 */
typedef struct stSetNotChibangUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD chibangid[4];
} stSetNotChibangUserCmd_t;

/* 0x046A SETCHIBANGJIHUOUSER total=22 line=4560 */
typedef struct stSetChibangJihuoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD chibangid[4];
} stSetChibangJihuoUserCmd_t;

/* 0x046B GOCHUANSONG5PROPERTYUSER total=14 line=4311 */
typedef struct stGoChuanSong5PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGoChuanSong5PropertyUserCmd_t;

/* 0x046C GOCHUANSONG6PROPERTYUSER total=14 line=4319 */
typedef struct stGoChuanSong6PropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGoChuanSong6PropertyUserCmd_t;

/* 0x046D SETZUOQIUSER total=22 line=4574 */
typedef struct stSetZuoqiUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD zuoqiid[4];
} stSetZuoqiUserCmd_t;

/* 0x046E SETNOTZUOQIUSER total=22 line=4586 */
typedef struct stSetNotZuoqiUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD zuoqiid[4];
} stSetNotZuoqiUserCmd_t;

/* 0x046F SETZUOQIJIHUOUSER total=22 line=4599 */
typedef struct stSetZuoqiJihuoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD zuoqiid[4];
} stSetZuoqiJihuoUserCmd_t;

/* 0x0470 SUIJICHUANSONGUSER total=14 line=4626 */
typedef struct stSuiJiChuanSongUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSuiJiChuanSongUserCmd_t;

/* 0x0472 SETJIEMIANUSER total=22 line=4635 */
typedef struct stSetJiemianUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD jiemianid[4];
} stSetJiemianUserCmd_t;

/* 0x0473 SETNOTJIEMIANUSER total=22 line=4647 */
typedef struct stSetNotJiemianUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD jiemianid[4];
} stSetNotJiemianUserCmd_t;

/* 0x0474 SETJIEMIANJIHUOUSER total=22 line=4660 */
typedef struct stSetJiemianJihuoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOldTempID[4];
    /* +4 */ DWORD jiemianid[4];
} stSetJiemianJihuoUserCmd_t;

/* 0x050A ADDUSERMAPSCREENUSER total=14 line=1564 */
typedef struct stAddUserMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddUserMapScreenUserCmd_t;

/* 0x050B REMOVEUSERMAPSCREENUSER total=18 line=1574 */
typedef struct stRemoveUserMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
} stRemoveUserMapScreenUserCmd_t;

/* 0x050C REQUESTUSERDATAMAPSCREENUSER total=20 line=1585 */
typedef struct stRequestUserDataMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
    /* +4 */ DWORD dwUserTempID[4];
} stRequestUserDataMapScreenUserCmd_t;

/* 0x050D RIDEMAPSCREENUSER total=19 line=1597 */
typedef struct stRideMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE bySwitch[1];
} stRideMapScreenUserCmd_t;

/* 0x050E REFRESHSTATEMAPSCREENUSER total=19 line=1610 */
typedef struct stRefreshStateMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE state[1];
} stRefreshStateMapScreenUserCmd_t;

/* 0x0514 ADDMAPOBJECTMAPSCREENUSER total=15 line=1628 */
typedef struct stAddMapObjectMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE action[1];
} stAddMapObjectMapScreenUserCmd_t;

/* 0x0515 REMOVEMAPOBJECTMAPSCREENUSER total=18 line=1640 */
typedef struct stRemoveMapObjectMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMapObjectTempID[4];
} stRemoveMapObjectMapScreenUserCmd_t;

/* 0x051E ADDMAPBUILDINGMAPSCREENUSER total=14 line=1650 */
typedef struct stAddMapBuildingMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddMapBuildingMapScreenUserCmd_t;

/* 0x051F REMOVEMAPBUILDINGMAPSCREENUSER total=18 line=1662 */
typedef struct stRemoveMapBuildingMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMapBuildingTempID[4];
} stRemoveMapBuildingMapScreenUserCmd_t;

/* 0x0520 CHANGEFACEMAPSCREENUSER total=22 line=1673 */
typedef struct stChangeFaceMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +4 */ DWORD dwChangeFaceID[4];
} stChangeFaceMapScreenUserCmd_t;

/* 0x0532 ADDMAPNPCMAPSCREENUSER total=15 line=1685 */
typedef struct stAddMapNpcMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAddMapNpcMapScreenUserCmd_t;

/* 0x0533 REMOVEMAPNPCMAPSCREENUSER total=18 line=1696 */
typedef struct stRemoveMapNpcMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMapNpcDataPosition[4];
} stRemoveMapNpcMapScreenUserCmd_t;

/* 0x0534 REQUESTMAPNPCDATAMAPSCREENUSER total=20 line=1706 */
typedef struct stRequestMapNpcDataMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
    /* +4 */ DWORD dwNpcTempID[4];
} stRequestMapNpcDataMapScreenUserCmd_t;

/* 0x0535 NPCHPMAPSCREENUSER total=26 line=1718 */
typedef struct stNPCHPMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMapNpcDataPosition[4];
    /* +4 */ DWORD dwHP[4];
    /* +4 */ DWORD dwMaxHP[4];
} stNPCHPMapScreenUserCmd_t;

/* 0x0536 ALLMAPSCREENUSER total=14 line=1745 */
typedef struct stAllMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAllMapScreenUserCmd_t;

/* 0x0537 MAPDATAMAPSCREENUSER total=14 line=1804 */
typedef struct stMapDataMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stMapDataMapScreenUserCmd_t;

/* 0x0538 RTSELECTEDHPMPPROPERTYUSER total=19 line=1822 */
typedef struct stRTSelectedHpMpPropertyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwTempID[4];
} stRTSelectedHpMpPropertyUserCmd_t;

/* 0x053A CLEAROBJECTOWNERMAPSCREENUSER total=18 line=1839 */
typedef struct stClearObjectOwnerMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMapObjectTempID[4];
} stClearObjectOwnerMapScreenUserCmd_t;

/* 0x053B ADDMAPNPCANDPOSMAPSCREENUSER total=14 line=1848 */
typedef struct stAddMapNpcAndPosMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddMapNpcAndPosMapScreenUserCmd_t;

/* 0x053C ADDUSERANDPOSMAPSCREENUSER total=14 line=1859 */
typedef struct stAddUserAndPosMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddUserAndPosMapScreenUserCmd_t;

/* 0x053D BATCHREMOVENPCMAPSCREENUSER total=20 line=1868 */
typedef struct stBatchRemoveNpcMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD num[2];
    /* +4 */ DWORD id[4];
} stBatchRemoveNpcMapScreenUserCmd_t;

/* 0x053D ADDPETMAPSCREENUSER total=15 line=2027 */
typedef struct stAddPetMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE size[1];
} stAddPetMapScreenUserCmd_t;

/* 0x053E BATCHREMOVEUSERMAPSCREENUSER total=20 line=1879 */
typedef struct stBatchRemoveUserMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD num[2];
    /* +4 */ DWORD id[4];
} stBatchRemoveUserMapScreenUserCmd_t;

/* 0x053F SETSTATEMAPSCREENUSER total=21 line=1889 */
typedef struct stSetStateMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
    /* +4 */ DWORD dwTempID[4];
    /* +2 */ WORD wdState[2];
} stSetStateMapScreenUserCmd_t;

/* 0x0540 CLEARSTATEMAPSCREENUSER total=21 line=1900 */
typedef struct stClearStateMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
    /* +4 */ DWORD dwTempID[4];
    /* +2 */ WORD wdState[2];
} stClearStateMapScreenUserCmd_t;

/* 0x0541 ADDMAPNPCPETMAPSCREENUSER total=14 line=1911 */
typedef struct stAddMapNpcPetMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddMapNpcPetMapScreenUserCmd_t;

/* 0x0542 ADDMAPPETMAPSCREENUSER total=14 line=1922 */
typedef struct stAddMapPetMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddMapPetMapScreenUserCmd_t;

/* 0x0543 GOODNESSSTATEMAPSCREENUSER total=22 line=1932 */
typedef struct stGoodnessStateMapScreenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTempID[4];
    /* +4 */ DWORD dwGoodness[4];
} stGoodnessStateMapScreenUserCmd_t;

/* 0x0544 ADDUSERMAPSCREENSTATEUSER total=14 line=1942 */
typedef struct stAddUserMapScreenStateUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddUserMapScreenStateUserCmd_t;

/* 0x0545 ADDMAPNPCMAPSCREENSTATEUSER total=14 line=1955 */
typedef struct stAddMapNpcMapScreenStateUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddMapNpcMapScreenStateUserCmd_t;

/* 0x0548 ADDMAPNPCPETMAPSCREENSTATEUSER total=14 line=1994 */
typedef struct stAddMapNpcPetMapScreenStateUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddMapNpcPetMapScreenStateUserCmd_t;

/* 0x0549 MAPDATAMAPSCREENSTATEUSER total=14 line=2009 */
typedef struct stMapDataMapScreenStateUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stMapDataMapScreenStateUserCmd_t;

/* 0x0602 USERMOVEMOVEUSER total=24 line=2501 */
typedef struct stUserMoveMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE byDirect[1];
    /* +1 */ BYTE bySpeed[1];
    /* +2 */ WORD x[2];
    /* +2 */ WORD y[2];
} stUserMoveMoveUserCmd_t;

/* 0x0603 USERJUMPTOUSER total=18 line=2518 */
typedef struct stUserJumptoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD x[2];
    /* +2 */ WORD y[2];
} stUserJumptoUserCmd_t;

/* 0x060A USERINSTANTJUMPMOVEUSER total=26 line=2530 */
typedef struct stUserInstantJumpMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +4 */ DWORD dwNextScreenPosition[4];
    /* +2 */ WORD wdNextOffsetX[2];
    /* +2 */ WORD wdNextOffsetY[2];
} stUserInstantJumpMoveUserCmd_t;

/* 0x060C USERGOTOMAPMOVEUSER total=21 line=2546 */
typedef struct stUserGotoMapMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD IsBackToCity[2];
    /* +4 */ DWORD dwMapType[4];
    /* +1 */ char mapName[1];
} stUserGotoMapMoveUserCmd_t;

/* 0x0614 NPCMOVEMOVEUSER total=24 line=2559 */
typedef struct stNpcMoveMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNpcTempID[4];
    /* +1 */ BYTE byDirect[1];
    /* +1 */ BYTE bySpeed[1];
    /* +2 */ WORD x[2];
    /* +2 */ WORD y[2];
} stNpcMoveMoveUserCmd_t;

/* 0x061E REQUESTLANDMASSMOVEUSER total=14 line=2575 */
typedef struct stRequestLandMassMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestLandMassMoveUserCmd_t;

/* 0x061F RESPONSELANDMASSMOVEUSER total=18 line=2593 */
typedef struct stResponseLandMassMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ int size[4];
} stResponseLandMassMoveUserCmd_t;

/* 0x0620 LANDMASSJUMPMOVEUSER total=14 line=2605 */
typedef struct stLandMassJumpMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stLandMassJumpMoveUserCmd_t;

/* 0x0621 SITDOWNMOVEUSER total=14 line=2614 */
typedef struct stSitDownMoveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSitDownMoveUserCmd_t;

/* 0x0C01 MAINUSERDEATHRELIVEUSER total=22 line=2420 */
typedef struct stMainUserDeathReliveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +4 */ DWORD deathType[4];
} stMainUserDeathReliveUserCmd_t;

/* 0x0C02 MAINUSERRELIVERELIVEUSER total=18 line=2435 */
typedef struct stMainUserReliveReliveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
} stMainUserReliveReliveUserCmd_t;

/* 0x0C03 OKRELIVEUSER total=19 line=2459 */
typedef struct stOKReliveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE byType[1];
} stOKReliveUserCmd_t;

/* 0x0E01 CHANNELCHATUSER total=41 line=4814 */
typedef struct stChannelChatUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwType[4];
    /* +4 */ DWORD dwSysInfoType[4];
    /* +4 */ DWORD dwCharType[4];
    /* +4 */ DWORD dwChannelID[4];
    /* +4 */ DWORD dwFromID[4];
    /* +4 */ DWORD dwChatTime[4];
    /* +1 */ BYTE size[1];
    /* +1 */ char pstrName[1];
    /* +1 */ char pstrChat[1];
} stChannelChatUserCmd_t;

/* 0x0E01 SERVERCHANNELCHATUSER total=39 line=4837 */
typedef struct stServerChannelChatUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwType[4];
    /* +4 */ DWORD dwSysInfoType[4];
    /* +4 */ DWORD dwCharType[4];
    /* +4 */ DWORD dwChannelID[4];
    /* +4 */ DWORD dwFromID[4];
    /* +4 */ DWORD dwChatTime[4];
    /* +1 */ BYTE size[1];
    /* +0 */ char raw[0];
} stServerChannelChatUserCmd_t;

/* 0x0E02 REMOVETEAMUSER total=14 line=4859 */
typedef struct stRemoveTeamUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRemoveTeamUserCmd_t;

/* 0x0E03 ADDTEAMMEMBERUSER total=18 line=4880 */
typedef struct stAddTeamMemberUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTeamID[4];
} stAddTeamMemberUserCmd_t;

/* 0x0E04 REMOVETEAMMEMBERUSER total=22 line=4892 */
typedef struct stRemoveTeamMemberUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTeamID[4];
    /* +4 */ DWORD dwTempID[4];
} stRemoveTeamMemberUserCmd_t;

/* 0x0E05 REQUESTTEAMUSER total=22 line=4914 */
typedef struct stRequestTeamUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwAnswerUserID[4];
    /* +4 */ DWORD dwTeamMode[4];
} stRequestTeamUserCmd_t;

/* 0x0E06 ANSWERTEAMUSER total=27 line=4926 */
typedef struct stAnswerTeamUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwRequestUserID[4];
    /* +4 */ DWORD dwAnswerUserID[4];
    /* +1 */ BYTE byAgree[1];
    /* +4 */ DWORD dwTeamMode[4];
} stAnswerTeamUserCmd_t;

/* 0x0E07 CREATECHANNELUSER total=24 line=4939 */
typedef struct stCreateChannelUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwChannelID[4];
    /* +4 */ DWORD dwClientID[4];
    /* +1 */ char name[1];
    /* +1 */ char name2[1];
} stCreateChannelUserCmd_t;

/* 0x0E08 INVITE_CHANNELUSER total=23 line=4953 */
typedef struct stInvite_ChannelUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwChannelID[4];
    /* +1 */ char name[1];
    /* +4 */ DWORD dwCharType[4];
} stInvite_ChannelUserCmd_t;

/* 0x0E09 JOIN_CHANNELUSER total=19 line=4966 */
typedef struct stJoin_ChannelUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwChannelID[4];
    /* +1 */ char name[1];
} stJoin_ChannelUserCmd_t;

/* 0x0E0A LEAVE_CHANNELUSER total=19 line=4978 */
typedef struct stLeave_ChannelUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwChannelID[4];
    /* +1 */ char name[1];
} stLeave_ChannelUserCmd_t;

/* 0x0E0E TEAMMEMBEROFFLINEUSER total=18 line=5043 */
typedef struct stTeamMemberOfflineUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTempID[4];
} stTeamMemberOfflineUserCmd_t;

/* 0x0E0F AUTOREPLYUSER total=15 line=5054 */
typedef struct stAutoReplyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char text[1];
} stAutoReplyUserCmd_t;

/* 0x0E10 EXPMODETEAMMEMBERUSER total=15 line=5076 */
typedef struct stExpModeTeamMemberUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stExpModeTeamMemberUserCmd_t;

/* 0x0E11 OBJMODETEAMMEMBERUSER total=15 line=5086 */
typedef struct stObjModeTeamMemberUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stObjModeTeamMemberUserCmd_t;

/* 0x0E12 TEAMMODEUSER total=15 line=5096 */
typedef struct stTeamModeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stTeamModeUserCmd_t;

/* 0x0E13 FRESHTEAMMEMBERUSER total=18 line=5106 */
typedef struct stFreshTeamMemberUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTempID[4];
} stFreshTeamMemberUserCmd_t;

/* 0x0E14 DIRTYUSERUSER total=15 line=5133 */
typedef struct stDirtyUserUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
} stDirtyUserUserCmd_t;

/* 0x0E15 ANSWERNAMETEAMUSER total=21 line=5143 */
typedef struct stAnswerNameTeamUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char byRequestUserName[1];
    /* +1 */ char byAnswerUserName[1];
    /* +1 */ BYTE byAgree[1];
    /* +4 */ DWORD dwTeamMode[4];
} stAnswerNameTeamUserCmd_t;

/* 0x0E16 REQUESTNAMETEAMUSER total=19 line=5156 */
typedef struct stRequestNameTeamUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char byAnswerUserName[1];
    /* +4 */ DWORD dwTeamMode[4];
} stRequestNameTeamUserCmd_t;

/* 0x0E17 REQUESTCOUNTRYHELPUSER total=19 line=5166 */
typedef struct stRequestCountryHelpUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char mapName[1];
    /* +2 */ WORD x[2];
    /* +2 */ WORD y[2];
} stRequestCountryHelpUserCmd_t;

/* 0x0E18 KILLFOREIGNERUSER total=16 line=5177 */
typedef struct stKillForeignerUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char heroName[1];
    /* +1 */ char mapName[1];
} stKillForeignerUserCmd_t;

/* 0x0E19 REFRESHBOSSUSER total=21 line=5189 */
typedef struct stRefreshBossUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD npcid[4];
    /* +1 */ BYTE country[1];
    /* +1 */ char mapName[1];
    /* +1 */ BYTE time[1];
} stRefreshBossUserCmd_t;

/* 0x0E1A KILLBOSSUSER total=24 line=5205 */
typedef struct stKillBossUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD npcid[4];
    /* +1 */ char name[1];
    /* +1 */ char mapName[1];
    /* +2 */ WORD x[2];
    /* +2 */ WORD y[2];
} stKillBossUserCmd_t;

/* 0x0E1B FRIENDCONGRATULATEUSER total=27 line=5221 */
typedef struct stFriendCongratulateUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD playerid[4];
    /* +1 */ char name[1];
    /* +4 */ DWORD dwLevel[4];
    /* +4 */ DWORD dwType[4];
} stFriendCongratulateUserCmd_t;

/* 0x0E1C FRIENDCONGRATULATEINFOUSER total=531 line=5234 */
typedef struct stFriendCongratulateInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD PlayerId[4];
    /* +1 */ char name[1];
    /* +512 */ char raw[512];
} stFriendCongratulateInfoUserCmd_t;

/* 0x1100 REQUESTTRADEUSER total=22 line=5264 */
typedef struct stRequestTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwAskerTempID[4];
    /* +4 */ DWORD dwAnswerTempID[4];
} stRequestTradeUserCmd_t;

/* 0x1101 ANSWERTRADEUSER total=23 line=5276 */
typedef struct stAnswerTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byAgree[1];
    /* +4 */ DWORD dwAskerTempID[4];
    /* +4 */ DWORD dwAnswerTempID[4];
} stAnswerTradeUserCmd_t;

/* 0x1102 BEGINTRADEUSER total=22 line=5290 */
typedef struct stBeginTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwAskerTempID[4];
    /* +4 */ DWORD dwAnswerTempID[4];
} stBeginTradeUserCmd_t;

/* 0x1105 COMMITTRADEUSER total=18 line=5302 */
typedef struct stCommitTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
} stCommitTradeUserCmd_t;

/* 0x1106 CANCELTRADEUSER total=18 line=5313 */
typedef struct stCancelTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
} stCancelTradeUserCmd_t;

/* 0x1107 FINISHTRADEUSER total=14 line=5324 */
typedef struct stFinishTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFinishTradeUserCmd_t;

/* 0x1108 ADDOBJECTTRADEUSER total=20 line=5334 */
typedef struct stAddObjectTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD user_id[4];
    /* +1 */ BYTE x[1];
    /* +1 */ BYTE y[1];
} stAddObjectTradeUserCmd_t;

/* 0x1109 REMOVEOBJECTTRADEUSER total=22 line=5349 */
typedef struct stRemoveObjectTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD user_id[4];
    /* +4 */ DWORD object_id[4];
} stRemoveObjectTradeUserCmd_t;

/* 0x110A VISITNPCTRADEUSER total=20 line=5361 */
typedef struct stVisitNpcTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNpcTempID[4];
    /* +1 */ BYTE byReturn[1];
    /* +1 */ char raw[1];
} stVisitNpcTradeUserCmd_t;

/* 0x110B BUYOBJECTNPCTRADEUSER total=23 line=5376 */
typedef struct stBuyObjectNpcTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwObjectID[4];
    /* +4 */ DWORD dwNum[4];
    /* +1 */ BYTE itemLevel[1];
} stBuyObjectNpcTradeUserCmd_t;

/* 0x110C SELLOBJECTNPCTRADEUSER total=18 line=5393 */
typedef struct stSellObjectNpcTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD qwThisID[4];
} stSellObjectNpcTradeUserCmd_t;

/* 0x110D REPAIROBJECTNPCTRADEUSER total=22 line=5404 */
typedef struct stRepairObjectNpcTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
    /* +4 */ DWORD gem_id[4];
} stRepairObjectNpcTradeUserCmd_t;

/* 0x110E SELLHORSENPCTRADEUSER total=15 line=5416 */
typedef struct stSellHorseNpcTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE action[1];
} stSellHorseNpcTradeUserCmd_t;

/* 0x110F STOREINFONPCTRADEUSER total=16 line=5428 */
typedef struct stStoreInfoNpcTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE page[1];
    /* +1 */ BYTE day[1];
} stStoreInfoNpcTradeUserCmd_t;

/* 0x1110 SELLTRADEUSER total=14 line=5441 */
typedef struct stSellTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSellTradeUserCmd_t;

/* 0x1111 FINISHSELLUSER total=14 line=5451 */
typedef struct stFinishSellUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFinishSellUserCmd_t;

/* 0x1112 ADDOBJECTSELLUSER total=20 line=5461 */
typedef struct stAddObjectSellUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD price[4];
    /* +1 */ BYTE x[1];
    /* +1 */ BYTE y[1];
} stAddObjectSellUserCmd_t;

/* 0x1113 REMOVEOBJECTSELLUSER total=18 line=5475 */
typedef struct stRemoveObjectSellUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD object_id[4];
} stRemoveObjectSellUserCmd_t;

/* 0x1114 REQUESTSELLINFOUSER total=18 line=5486 */
typedef struct stRequestSellInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD temp_id[4];
} stRequestSellInfoUserCmd_t;

/* 0x1115 REQUESTSELLBUYUSER total=22 line=5497 */
typedef struct stRequestSellBuyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD temp_id[4];
    /* +4 */ DWORD object_id[4];
} stRequestSellBuyUserCmd_t;

/* 0x1116 UPDATESTOREPASSUSER total=30 line=5509 */
typedef struct stUpdateStorePassUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +8 */ char raw[8];
    /* +8 */ char raw[8];
} stUpdateStorePassUserCmd_t;

/* 0x1117 CANSELLTRADEUSER total=15 line=5521 */
typedef struct stCanSellTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE status[1];
} stCanSellTradeUserCmd_t;

/* 0x1118 GOLDGIVETRADEUSER total=15 line=5541 */
typedef struct stGoldGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
} stGoldGiveTradeUserCmd_t;

/* 0x111A REQUESTGOLDGIVETRADEUSER total=14 line=5561 */
typedef struct stRequestGoldGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestGoldGiveTradeUserCmd_t;

/* 0x111A REQUESTMATARIALGIVETRADEUSER total=14 line=5730 */
typedef struct stRequestMatarialGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestMatarialGiveTradeUserCmd_t;

/* 0x111B RETURNGOLDGIVETRADEUSER total=26 line=5570 */
typedef struct stReturnGoldGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD Storn_num[4];
    /* +4 */ DWORD Matarial_num[4];
    /* +4 */ DWORD Card_num[4];
} stReturnGoldGiveTradeUserCmd_t;

/* 0x111B RETURNMATARIALGIVETRADEUSER total=18 line=5739 */
typedef struct stReturnMatarialGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD num[4];
} stReturnMatarialGiveTradeUserCmd_t;

/* 0x111C ADDOBJECTSELLLISTUSER total=22 line=5582 */
typedef struct stAddObjectSellListUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD num[2];
    /* +4 */ DWORD price[4];
    /* +1 */ BYTE x[1];
    /* +1 */ BYTE y[1];
} stAddObjectSellListUserCmd_t;

/* 0x111D REQUESTADDOBJECTSELLUSER total=26 line=5600 */
typedef struct stRequestAddObjectSellUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD num[2];
    /* +4 */ DWORD qwThisID[4];
    /* +4 */ DWORD price[4];
    /* +1 */ BYTE x[1];
    /* +1 */ BYTE y[1];
} stRequestAddObjectSellUserCmd_t;

/* 0x111E BUYOBJECTSALARYTRADEUSER total=23 line=5619 */
typedef struct stBuyObjectSalaryTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwObjectID[4];
    /* +4 */ DWORD dwNum[4];
    /* +1 */ BYTE itemLevel[1];
} stBuyObjectSalaryTradeUserCmd_t;

/* 0x111E MATARIALGIVETRADEUSER total=14 line=5721 */
typedef struct stMatarialgiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stMatarialgiveTradeUserCmd_t;

/* 0x111F BUYOBJECTTICKETTRADEUSER total=23 line=5637 */
typedef struct stBuyObjectTicketTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwObjectID[4];
    /* +4 */ DWORD dwNum[4];
    /* +1 */ BYTE itemLevel[1];
} stBuyObjectTicketTradeUserCmd_t;

/* 0x1120 BUYOBJECTCHONGZHITRADEUSER total=23 line=5655 */
typedef struct stBuyObjectChongzhiTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwObjectID[4];
    /* +4 */ DWORD dwNum[4];
    /* +1 */ BYTE itemLevel[1];
} stBuyObjectChongzhiTradeUserCmd_t;

/* 0x1121 BUYOBJECTMALLUSER total=23 line=5672 */
typedef struct stBuyObjectMallUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwObjectID[4];
    /* +4 */ DWORD dwNum[4];
    /* +1 */ BYTE itemLevel[1];
} stBuyObjectMallUserCmd_t;

/* 0x1122 FORTGOLDGIVETRADEUSER total=15 line=5688 */
typedef struct stFortGoldGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE type[1];
} stFortGoldGiveTradeUserCmd_t;

/* 0x1123 REQUESTFORTUNGIVETRADEUSER total=14 line=5697 */
typedef struct stRequestFortunGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestFortunGiveTradeUserCmd_t;

/* 0x1124 RETURNFORTUNFUNDGIVETRADEUSER total=26 line=5706 */
typedef struct stReturnFortunFundGiveTradeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD Fortun0_num[4];
    /* +4 */ DWORD Fortun1_num[4];
    /* +4 */ DWORD Fortun2_num[4];
} stReturnFortunFundGiveTradeUserCmd_t;

/* 0x1205 ATTACKMAGICUSER total=35 line=2143 */
typedef struct stAttackMagicUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +4 */ DWORD dwDefenceTempID[4];
    /* +2 */ WORD wdMagicType[2];
    /* +2 */ WORD xDes[2];
    /* +2 */ WORD yDes[2];
    /* +1 */ BYTE byDirect[1];
    /* +1 */ BYTE byAttackType[1];
    /* +1 */ BYTE byAction[1];
    /* +4 */ DWORD dwTempIDList[4];
} stAttackMagicUserCmd_t;

/* 0x1206 RTMAGICUSER total=35 line=2185 */
typedef struct stRTMagicUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE byTarget[1];
    /* +1 */ BYTE byRetcode[1];
    /* +1 */ BYTE byDirect[1];
    /* +1 */ BYTE bySrc[1];
    /* +4 */ DWORD dwSrcTempID[4];
    /* +4 */ DWORD dwHP[4];
    /* +4 */ DWORD dwReserved[4];
    /* +1 */ BYTE byLuck[1];
} stRTMagicUserCmd_t;

/* 0x1207 BACKOFFMAGICUSER total=28 line=2252 */
typedef struct stBackOffMagicUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTempID[4];
    /* +1 */ BYTE byType[1];
    /* +1 */ BYTE byDirect[1];
    /* +4 */ DWORD x[4];
    /* +4 */ DWORD y[4];
} stBackOffMagicUserCmd_t;

/* 0x1208 OBTAINEXPUSER total=19 line=2267 */
typedef struct stObtainExpUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwTempID[4];
    /* +1 */ BYTE byType[1];
} stObtainExpUserCmd_t;

/* 0x1209 NPCDEATHUSER total=18 line=2281 */
typedef struct stNpcDeathUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNpcTempID[4];
} stNpcDeathUserCmd_t;

/* 0x120A LEVELUPUSER total=18 line=2292 */
typedef struct stLevelUpUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
} stLevelUpUserCmd_t;

/* 0x120B OBJECTHPMPPOPUSER total=23 line=2224 */
typedef struct stObjectHpMpPopUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE byTarget[1];
    /* +4 */ int vChange[4];
} stObjectHpMpPopUserCmd_t;

/* 0x120C RTMAGICPOSUSER total=15 line=2240 */
typedef struct stRTMagicPosUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byTarget[1];
} stRTMagicPosUserCmd_t;

/* 0x1214 PKMODEUSER total=15 line=2317 */
typedef struct stPKModeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byPKMode[1];
} stPKModeUserCmd_t;

/* 0x1216 OBJECTEXPUSER total=22 line=2325 */
typedef struct stObjectExpUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
    /* +4 */ DWORD exp[4];
} stObjectExpUserCmd_t;

/* 0x1217 UNCOMBINUSER total=14 line=2345 */
typedef struct stUnCombinUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stUnCombinUserCmd_t;

/* 0x1218 FIREWORKUSER total=31 line=2355 */
typedef struct stFireWorkUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +8 */ QWORD qwObjectTempID[8];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwUserID[4];
    /* +4 */ DWORD dwFireID[4];
} stFireWorkUserCmd_t;

/* 0x1219 TIMESYNCUSER total=14 line=2369 */
typedef struct stTimeSyncUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTimeSyncUserCmd_t;

/* 0x121A SWITCHCHARACTEROBSTACLEUSER total=14 line=2380 */
typedef struct stSwitchCharacterObstacleUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSwitchCharacterObstacleUserCmd_t;

/* 0x121B RTOTHERMAGICUSER total=24 line=2390 */
typedef struct stRTOtherMagicUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byTarget[1];
    /* +4 */ DWORD dwUserTempID[4];
    /* +1 */ BYTE bySrc[1];
    /* +4 */ DWORD dwSrcTempID[4];
} stRTOtherMagicUserCmd_t;

/* 0x1501 UNIONSTATUSCHECK total=14 line=5769 */
typedef struct stUnionStatusCheckCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stUnionStatusCheckCmd_t;

/* 0x1502 CREATEUNION total=15 line=5779 */
typedef struct stCreateUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char UnionName[1];
} stCreateUnionCmd_t;

/* 0x1503 SERVERRETURNUNIONCHECK total=14 line=5790 */
typedef struct stServerReturnUnionCheckCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stServerReturnUnionCheckCmd_t;

/* 0x1505 ALLUNIONMEMBER total=16 line=5811 */
typedef struct stAllUnionMemberCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
} stAllUnionMemberCmd_t;

/* 0x1506 RETURNUNIONMEMBERINFO total=16 line=5837 */
typedef struct stReturnUnionMemberInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char alias[1];
    /* +1 */ BYTE byPower[1];
} stReturnUnionMemberInfoCmd_t;

/* 0x1507 ADDMEMBERTOUNION total=21 line=5856 */
typedef struct stAddMemberToUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char memberName[1];
    /* +4 */ DWORD memberID[4];
    /* +1 */ char unionName[1];
    /* +1 */ BYTE byState[1];
} stAddMemberToUnionCmd_t;

/* 0x1508 FIREMEMBERFROMUNION total=15 line=5870 */
typedef struct stFireMemberFromUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char memberName[1];
} stFireMemberFromUnionCmd_t;

/* 0x150B UNIONBASEINFO total=289 line=5911 */
typedef struct stUnionBaseInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char unionName[1];
    /* +1 */ char master[1];
    /* +2 */ WORD wdLevel[2];
    /* +8 */ QWORD qwExp[8];
    /* +255 */ char raw[255];
    /* +4 */ DWORD dwMana[4];
    /* +4 */ DWORD dwAction[4];
} stUnionBaseInfoCmd_t;

/* 0x150C UNIONMEMBERLEAVEUNION total=14 line=5928 */
typedef struct stUnionMemberLeaveUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stUnionMemberLeaveUnionCmd_t;

/* 0x150D VOTELISTUNION total=20 line=5945 */
typedef struct stVoteListUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char unionName[1];
    /* +4 */ DWORD dwSize[4];
    /* +1 */ BYTE flag[1];
} stVoteListUnionCmd_t;

/* 0x150E VOTEUNION total=16 line=5959 */
typedef struct stVoteUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char unionName[1];
    /* +1 */ BYTE bySuccess[1];
} stVoteUnionCmd_t;

/* 0x150F NOTEUNION total=269 line=5971 */
typedef struct stNoteUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +255 */ char raw[255];
} stNoteUnionCmd_t;

/* 0x1518 LISTUNION total=19 line=6114 */
typedef struct stListUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
    /* +1 */ BYTE flag[1];
} stListUnionCmd_t;

/* 0x1519 REQUESTJOINUNION total=16 line=6127 */
typedef struct stRequestJoinUnionCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ BYTE status[1];
} stRequestJoinUnionCmd_t;

/* 0x151A REQUESTABJURATIONCITYN total=14 line=6139 */
typedef struct stRequestAbjurationCitynCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestAbjurationCitynCmd_t;

/* 0x151B REQUNIONNAME total=18 line=6149 */
typedef struct stReqUnionNameCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUnionID[4];
} stReqUnionNameCmd_t;

/* 0x151C RTNUNIONNAME total=19 line=6162 */
typedef struct stRtnUnionNameCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUnionID[4];
    /* +1 */ char name[1];
} stRtnUnionNameCmd_t;

/* 0x151D REQUNIONNAMELLIST total=19 line=6177 */
typedef struct stReqUnionNameLlistCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE num[1];
    /* +4 */ DWORD dwUnionID[4];
} stReqUnionNameLlistCmd_t;

/* 0x151E RTNUNIONNAMELIST total=20 line=6190 */
typedef struct stRtnUnionNameListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE num[1];
    /* +4 */ DWORD dwUnionID[4];
    /* +1 */ char name[1];
} stRtnUnionNameListCmd_t;

/* 0x151F APPOINTCITYCATCHER total=15 line=6207 */
typedef struct stAppointCityCatcherCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointCityCatcherCmd_t;

/* 0x1520 CANCELCITYCATCHER total=14 line=6220 */
typedef struct stCancelCityCatcherCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stCancelCityCatcherCmd_t;

/* 0x1601 CHANGECOUNTRY total=18 line=6247 */
typedef struct stChangeCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwToCountryID[4];
} stChangeCountryCmd_t;

/* 0x1602 REQUESTDARECOUNTRY total=15 line=6265 */
typedef struct stRequestDareCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stRequestDareCountryCmd_t;

/* 0x1603 RETURNDARECOUNTRY total=19 line=6283 */
typedef struct stReturnDareCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
    /* +1 */ BYTE byType[1];
} stReturnDareCountryCmd_t;

/* 0x1604 TRANSDARECOUNTRY total=18 line=6299 */
typedef struct stTransDareCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountryID[4];
} stTransDareCountryCmd_t;

/* 0x1605 DARECOUNTRYFORMAL total=18 line=6312 */
typedef struct stDareCountryFormalCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountryID[4];
} stDareCountryFormalCmd_t;

/* 0x1606 REQUESTDARERECORD total=15 line=6331 */
typedef struct stRequestDareRecordCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stRequestDareRecordCmd_t;

/* 0x1607 RETURNDARERECORDRESULT total=18 line=6353 */
typedef struct stReturnDareRecordResultCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
} stReturnDareRecordResultCmd_t;

/* 0x1608 RETURNDARERECORDSTAT total=18 line=6375 */
typedef struct stReturnDareRecordStatCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
} stReturnDareRecordStatCmd_t;

/* 0x1609 RETURNDARERECORDPLAN total=18 line=6396 */
typedef struct stReturnDareRecordPlanCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
} stReturnDareRecordPlanCmd_t;

/* 0x160A CANCELCOUNTRY total=14 line=6410 */
typedef struct stCancelCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stCancelCountryCmd_t;

/* 0x160B APPLYCOUNTRY total=18 line=6420 */
typedef struct stApplyCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwToCountryID[4];
} stApplyCountryCmd_t;

/* 0x160C TAXCOUNTRYUSER total=19 line=6432 */
typedef struct stTaxCountryUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountry[4];
    /* +1 */ BYTE byTax[1];
} stTaxCountryUserCmd_t;

/* 0x160D FISKCOUNTRYUSER total=42 line=6444 */
typedef struct stFiskCountryUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountry[4];
    /* +8 */ QWORD qwGold[8];
    /* +8 */ QWORD qwMaterial[8];
    /* +8 */ QWORD qwStock[8];
} stFiskCountryUserCmd_t;

/* 0x160E SETTAXCOUNTRYUSER total=19 line=6462 */
typedef struct stSetTaxCountryUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountry[4];
    /* +1 */ BYTE byTax[1];
} stSetTaxCountryUserCmd_t;

/* 0x160F UPDATECOUNTRYSTAR total=18 line=6480 */
typedef struct stUpdateCountryStarCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
} stUpdateCountryStarCmd_t;

/* 0x1610 CONTRIBUTECOUNTRYMATERIAL total=18 line=6494 */
typedef struct stContributeCountryMaterialCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemID[4];
} stContributeCountryMaterialCmd_t;

/* 0x1611 SETCOUNTRYDAREUSER total=20 line=6506 */
typedef struct stSetCountryDareUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountry[4];
    /* +1 */ BYTE byStatus[1];
    /* +1 */ BYTE byResult[1];
} stSetCountryDareUserCmd_t;

/* 0x1612 UPDATECOUNTRYKINGUSER total=16 line=6524 */
typedef struct stUpdateCountryKingUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char kingName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryKingUserCmd_t;

/* 0x1613 ANSWERCOUNTRYDAREUSER total=15 line=6544 */
typedef struct stAnswerCountryDareUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byStatus[1];
} stAnswerCountryDareUserCmd_t;

/* 0x1614 SELECTTRANSLEVELUSER total=18 line=6557 */
typedef struct stSelectTransLevelUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwLevel[4];
} stSelectTransLevelUserCmd_t;

/* 0x1615 FORBIDTALKCOUNTRYUSER total=15 line=6570 */
typedef struct stForbidTalkCountryUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stForbidTalkCountryUserCmd_t;

/* 0x1616 REQTECHUSER total=18 line=6589 */
typedef struct stReqTechUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwType[4];
} stReqTechUserCmd_t;

/* 0x1617 RTNTECHUSER total=22 line=6611 */
typedef struct stRtnTechUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwType[4];
    /* +4 */ DWORD dwSize[4];
} stRtnTechUserCmd_t;

/* 0x1618 REQWAITOFFICIALUSER total=14 line=6627 */
typedef struct stReqWaitOfficialUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqWaitOfficialUserCmd_t;

/* 0x1619 RTNWAITOFFICIALUSER total=18 line=6643 */
typedef struct stRtnWaitOfficialUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
} stRtnWaitOfficialUserCmd_t;

/* 0x161A SETTECHSEARCHUSER total=22 line=6657 */
typedef struct stSetTechSearchUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOption[4];
    /* +4 */ DWORD dwCharID[4];
} stSetTechSearchUserCmd_t;

/* 0x161B UPTECHDEGREEUSER total=18 line=6672 */
typedef struct stUpTechDegreeUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOption[4];
} stUpTechDegreeUserCmd_t;

/* 0x161C CONFIRMSEARCHERUSER total=19 line=6692 */
typedef struct stConfirmSearcherUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOption[4];
    /* +1 */ BYTE byStatus[1];
} stConfirmSearcherUserCmd_t;

/* 0x161D DISABLELOGINCOUNTRY total=14 line=6712 */
typedef struct stDisableLoginCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stDisableLoginCountryCmd_t;

/* 0x161E CANCELTECHSEARCHUSER total=18 line=6723 */
typedef struct stCancelTechSearchUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwOption[4];
} stCancelTechSearchUserCmd_t;

/* 0x161F ANTIDARECOUNTRYFORMAL total=18 line=6736 */
typedef struct stAntiDareCountryFormalCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwCountryID[4];
} stAntiDareCountryFormalCmd_t;

/* 0x1620 COUNTRYNOTE total=269 line=6748 */
typedef struct stCountryNoteCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +255 */ char raw[255];
} stCountryNoteCmd_t;

/* 0x1621 REQCOUNTRYNOTE total=14 line=6760 */
typedef struct stReqCountryNoteCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqCountryNoteCmd_t;

/* 0x1622 KINGPUNISHCOUNTRY total=19 line=6770 */
typedef struct stKingPunishCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +4 */ DWORD method[4];
} stKingPunishCountryCmd_t;

/* 0x1623 EMPERORPUNISHCOUNTRY total=19 line=6782 */
typedef struct stEmperorPunishCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +4 */ DWORD method[4];
} stEmperorPunishCountryCmd_t;

/* 0x1624 REQGENCOUNTRY total=14 line=6794 */
typedef struct stReqGenCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqGenCountryCmd_t;

/* 0x1625 RETGENCOUNTRY total=26 line=6804 */
typedef struct stRetGenCountryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
    /* +4 */ DWORD exp[4];
    /* +4 */ DWORD maxExp[4];
} stRetGenCountryCmd_t;

/* 0x1626 APPOINTDIPLOMAT total=15 line=6817 */
typedef struct stAppointDiplomatCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointDiplomatCmd_t;

/* 0x1627 CANCELDIPLOMAT total=14 line=6830 */
typedef struct stCancelDiplomatCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stCancelDiplomatCmd_t;

/* 0x1628 APPOINTCATCHER total=15 line=6841 */
typedef struct stAppointCatcherCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointCatcherCmd_t;

/* 0x1629 CANCELCATCHER total=15 line=6854 */
typedef struct stCancelCatcherCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stCancelCatcherCmd_t;

/* 0x162A APPOINTCATCHEX total=15 line=6867 */
typedef struct stAppointCatchexCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointCatchexCmd_t;

/* 0x162B CANCELCATCHEX total=15 line=6880 */
typedef struct stCancelCatchexCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stCancelCatchexCmd_t;

/* 0x162C APPOINTCENSOR total=15 line=6893 */
typedef struct stAppointCensorCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointCensorCmd_t;

/* 0x162D CANCELCENSOR total=15 line=6906 */
typedef struct stCancelCensorCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stCancelCensorCmd_t;

/* 0x162E APPOINTCENSOS total=15 line=6919 */
typedef struct stAppointCensosCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointCensosCmd_t;

/* 0x162F CANCELCENSOS total=15 line=6932 */
typedef struct stCancelCensosCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stCancelCensosCmd_t;

/* 0x1630 APPOINTYUANSHUAI total=15 line=6945 */
typedef struct stAppointYuanshuaiCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointYuanshuaiCmd_t;

/* 0x1631 CANCELYUANSHUAI total=15 line=6958 */
typedef struct stCancelYuanshuaiCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stCancelYuanshuaiCmd_t;

/* 0x1632 APPOINTZAIXIANG total=15 line=6971 */
typedef struct stAppointZaixiangCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stAppointZaixiangCmd_t;

/* 0x1633 CANCELZAIXIANG total=15 line=6984 */
typedef struct stCancelZaixiangCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stCancelZaixiangCmd_t;

/* 0x1634 REQUESTCOUNTRYPOWER total=14 line=6997 */
typedef struct stRequestCountryPowerCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestCountryPowerCmd_t;

/* 0x1635 RETURNCOUNTRYPOWER total=15 line=7007 */
typedef struct stReturnCountryPowerCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE country[1];
} stReturnCountryPowerCmd_t;

/* 0x1636 REQDAILYEMPERORMONEY total=14 line=7018 */
typedef struct stReqDailyEmperorMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyEmperorMoneyCmd_t;

/* 0x1637 REQKINGLIST total=14 line=7028 */
typedef struct stReqKingListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqKingListCmd_t;

/* 0x1638 RTNKINGLIST total=15 line=7045 */
typedef struct stRtnKingListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE size[1];
} stRtnKingListCmd_t;

/* 0x1639 REQCITYOWNERLIST total=14 line=7058 */
typedef struct stReqCityOwnerListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqCityOwnerListCmd_t;

/* 0x163A RTNCITYOWNERLIST total=18 line=7068 */
typedef struct stRtnCityOwnerListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
    /* +1 */ char cityName[1];
    /* +1 */ char unionName[1];
} stRtnCityOwnerListCmd_t;

/* 0x163B REQNPCOWNERLIST total=14 line=7085 */
typedef struct stReqNpcOwnerListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqNpcOwnerListCmd_t;

/* 0x163C RTNNPCOWNERLIST total=26 line=7094 */
typedef struct stRtnNpcOwnerListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
    /* +4 */ DWORD npcID[4];
    /* +1 */ char mapName[1];
    /* +2 */ WORD x[2];
    /* +2 */ WORD y[2];
    /* +1 */ char septName[1];
} stRtnNpcOwnerListCmd_t;

/* 0x163D UPDATECOUNTRYCATCHERUSER total=16 line=7114 */
typedef struct stUpdateCountryCatcherUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char catcherName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryCatcherUserCmd_t;

/* 0x163E UPDATECOUNTRYCATCHEXUSER total=16 line=7128 */
typedef struct stUpdateCountryCatchexUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char catchexName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryCatchexUserCmd_t;

/* 0x163F UPDATECOUNTRYCENSORUSER total=16 line=7142 */
typedef struct stUpdateCountryCensorUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char censorName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryCensorUserCmd_t;

/* 0x1640 UPDATECOUNTRYCENSOSUSER total=16 line=7156 */
typedef struct stUpdateCountryCensosUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char censosName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryCensosUserCmd_t;

/* 0x1641 UPDATECOUNTRYYUANSHUAIUSER total=16 line=7170 */
typedef struct stUpdateCountryYuanshuaiUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char yuanshuaiName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryYuanshuaiUserCmd_t;

/* 0x1642 UPDATECOUNTRYZAIXIANGUSER total=16 line=7184 */
typedef struct stUpdateCountryZaixiangUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char zaixiangName[1];
    /* +1 */ BYTE isEmperor[1];
} stUpdateCountryZaixiangUserCmd_t;

/* 0x1643 REQDAILYKINGMONEY total=14 line=7198 */
typedef struct stReqDailyKingMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyKingMoneyCmd_t;

/* 0x1644 REQDAILYYUANSHUAIMONEY total=14 line=7208 */
typedef struct stReqDailyYuanshuaiMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyYuanshuaiMoneyCmd_t;

/* 0x1645 REQDAILYZAIXIANGMONEY total=14 line=7218 */
typedef struct stReqDailyZaixiangMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyZaixiangMoneyCmd_t;

/* 0x1646 REQDAILYCATCHERMONEY total=14 line=7228 */
typedef struct stReqDailyCatcherMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyCatcherMoneyCmd_t;

/* 0x1647 REQDAILYCENSORMONEY total=14 line=7238 */
typedef struct stReqDailyCensorMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyCensorMoneyCmd_t;

/* 0x1648 REQDAILYCATCHEXMONEY total=14 line=7248 */
typedef struct stReqDailyCatchexMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyCatchexMoneyCmd_t;

/* 0x1649 REQDAILYCENSOSMONEY total=14 line=7258 */
typedef struct stReqDailyCensosMoneyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDailyCensosMoneyCmd_t;

/* 0x164C CHANGECOUNTRYSALARY total=18 line=7268 */
typedef struct stChangeCountrySalaryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwToCountryID[4];
} stChangeCountrySalaryCmd_t;

/* 0x164D ADDCOUNTRYSALARY total=14 line=7280 */
typedef struct stAddCountrySalaryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddCountrySalaryCmd_t;

/* 0x164E ADDCOUNTRYLOGIN total=14 line=7290 */
typedef struct stAddCountryLoginCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddCountryLoginCmd_t;

/* 0x164F ADDCOUNTRYVIP total=14 line=7300 */
typedef struct stAddCountryVipCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddCountryVipCmd_t;

/* 0x1650 ADDCOUNTRYCHOU total=14 line=7310 */
typedef struct stAddCountryChouCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stAddCountryChouCmd_t;

/* 0x1801 USERINFOUSER total=17 line=2729 */
typedef struct stUserInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
    /* +1 */ BYTE data[1];
} stUserInfoUserCmd_t;

/* 0x1802 CREATESELECTUSER total=26 line=2761 */
typedef struct stCreateSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char strUserName[1];
    /* +2 */ WORD charType[2];
    /* +1 */ BYTE byHairType[1];
    /* +4 */ DWORD byRGB[4];
    /* +2 */ WORD country[2];
    /* +2 */ WORD five[2];
} stCreateSelectUserCmd_t;

/* 0x1803 LOGINSELECTUSER total=25 line=2778 */
typedef struct stLoginSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD charNo[4];
    /* +7 */ char raw[7];
} stLoginSelectUserCmd_t;

/* 0x1804 DELETESELECTUSER total=19 line=2791 */
typedef struct stDeleteSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD charNo[4];
    /* +1 */ char numPassword[1];
} stDeleteSelectUserCmd_t;

/* 0x1805 CHECKNAMESELECTUSER total=16 line=2805 */
typedef struct stCheckNameSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ BYTE err_code[1];
} stCheckNameSelectUserCmd_t;

/* 0x1806 RETURNDELETESELECTUSER total=16 line=2820 */
typedef struct stReturnDeleteSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ BYTE err_code[1];
} stReturnDeleteSelectUserCmd_t;

/* 0x1807 REQUESTSELECTUSER total=14 line=2836 */
typedef struct stRequestSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestSelectUserCmd_t;

/* 0x1808 DELETEERRORSELECTUSER total=14 line=2846 */
typedef struct stDeleteErrorSelectUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stDeleteErrorSelectUserCmd_t;

/* 0x1901 SENDRELATIONLIST total=16 line=7930 */
typedef struct stSendRelationListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
} stSendRelationListCmd_t;

/* 0x1903 RELATIONSTATUS total=36 line=7953 */
typedef struct stRelationStatusCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ BYTE type[1];
    /* +2 */ WORD occupation[2];
    /* +2 */ WORD user_level[2];
    /* +2 */ WORD level[2];
    /* +4 */ DWORD userid[4];
    /* +1 */ BYTE byState[1];
    /* +4 */ DWORD exploit[4];
    /* +4 */ DWORD country[4];
    /* +1 */ char unionName[1];
} stRelationStatusCmd_t;

/* 0x1904 MARRYSTATUSCHECK total=15 line=7998 */
typedef struct stMarryStatusCheckCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byStep[1];
} stMarryStatusCheckCmd_t;

/* 0x1905 UNMARRY total=14 line=8010 */
typedef struct stUnmarryCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stUnmarryCmd_t;

/* 0x1906 REQUSERSORTINFO total=14 line=8049 */
typedef struct stReqUserSortInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqUserSortInfoCmd_t;

/* 0x1907 RETUSERSORTINFO total=16 line=8058 */
typedef struct stRetUserSortInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
} stRetUserSortInfoCmd_t;

/* 0x1A01 ADDMEMBERTOSCHOOL total=30 line=8093 */
typedef struct stAddMemberToSchoolCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char memberName[1];
    /* +4 */ DWORD memberID[4];
    /* +4 */ DWORD degree[4];
    /* +1 */ char schoolName[1];
    /* +1 */ char prename[1];
    /* +1 */ BYTE byState[1];
    /* +2 */ WORD wdOccupation[2];
    /* +2 */ WORD level[2];
} stAddMemberToSchoolCmd_t;

/* 0x1A02 FIREMEMBERFROMSCHOOL total=15 line=8116 */
typedef struct stFireMemberFromSchoolCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char memberName[1];
} stFireMemberFromSchoolCmd_t;

/* 0x1A03 SCHOOLSTATUSCHECK total=14 line=8127 */
typedef struct stSchoolStatusCheckCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSchoolStatusCheckCmd_t;

/* 0x1A04 CREATESCHOOL total=15 line=8137 */
typedef struct stCreateSchoolCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char SchoolName[1];
} stCreateSchoolCmd_t;

/* 0x1A05 SCHOOLMEMBERLEAVESCHOOL total=14 line=8148 */
typedef struct stSchoolMemberLeaveSchoolCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSchoolMemberLeaveSchoolCmd_t;

/* 0x1A06 SENDLISTSCHOOL total=21 line=8194 */
typedef struct stSendListSchoolCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMasterTempID[4];
    /* +1 */ BYTE byPackageTag[1];
    /* +2 */ WORD size[2];
} stSendListSchoolCmd_t;

/* 0x1A07 SENDMEMBERINFO total=16 line=8216 */
typedef struct stSendMemberInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
} stSendMemberInfoCmd_t;

/* 0x1A09 SCHOOLMEMBERSTATUS total=24 line=8250 */
typedef struct stSchoolMemberStatusCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ BYTE byState[1];
    /* +2 */ WORD level[2];
    /* +4 */ DWORD country[4];
    /* +1 */ char unionName[1];
    /* +1 */ char septName[1];
} stSchoolMemberStatusCmd_t;

/* 0x1A0A SCHOOLBULLETIN total=17 line=8273 */
typedef struct stSchoolBulletinCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byState[1];
    /* +2 */ WORD wdSize[2];
    /* +0 */ char raw[0];
} stSchoolBulletinCmd_t;

/* 0x1A0B DESTROYSCHOOL total=14 line=8286 */
typedef struct stDestroySchoolCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stDestroySchoolCmd_t;

/* 0x1A0C SCHOOLBULLETINNOTIFY total=14 line=8295 */
typedef struct stSchoolBulletinNotifyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSchoolBulletinNotifyCmd_t;

/* 0x1A0D REQMASTERBOUNTYUSER total=15 line=8312 */
typedef struct stReqMasterBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byState[1];
} stReqMasterBountyUserCmd_t;

/* 0x1A0E REQPRENTICEBOUNTYUSER total=15 line=8324 */
typedef struct stReqPrenticeBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byState[1];
} stReqPrenticeBountyUserCmd_t;

/* 0x1A0F RTNMASTERBOUNTYUSER total=22 line=8336 */
typedef struct stRtnMasterBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwBalance[4];
    /* +4 */ DWORD dwTotal[4];
} stRtnMasterBountyUserCmd_t;

/* 0x1A10 RTNPRENTICEBOUNTYUSER total=27 line=8351 */
typedef struct stRtnPrenticeBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwBalance[4];
    /* +4 */ DWORD dwTotal[4];
    /* +4 */ DWORD dwLastLevel[4];
    /* +1 */ char name[1];
} stRtnPrenticeBountyUserCmd_t;

/* 0x1B01 SEPTSTATUSCHECK total=14 line=8494 */
typedef struct stSeptStatusCheckCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSeptStatusCheckCmd_t;

/* 0x1B02 CREATESEPT total=15 line=8505 */
typedef struct stCreateSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char SeptName[1];
} stCreateSeptCmd_t;

/* 0x1B03 SERVERRETURNSEPTCHECK total=14 line=8516 */
typedef struct stServerReturnSeptCheckCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stServerReturnSeptCheckCmd_t;

/* 0x1B05 ALLSEPTMEMBER total=16 line=8536 */
typedef struct stAllSeptMemberCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +2 */ WORD size[2];
} stAllSeptMemberCmd_t;

/* 0x1B07 ADDMEMBERTOSEPT total=21 line=8555 */
typedef struct stAddMemberToSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char memberName[1];
    /* +4 */ DWORD memberID[4];
    /* +1 */ char septName[1];
    /* +1 */ BYTE byState[1];
} stAddMemberToSeptCmd_t;

/* 0x1B08 FIREMEMBERFROMSEPT total=15 line=8569 */
typedef struct stFireMemberFromSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char memberName[1];
} stFireMemberFromSeptCmd_t;

/* 0x1B0B SEPTBASEINFO total=283 line=8610 */
typedef struct stSeptBaseInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwMasterTempID[4];
    /* +1 */ char septName[1];
    /* +1 */ char master[1];
    /* +255 */ char raw[255];
    /* +4 */ DWORD dwRepute[4];
    /* +4 */ DWORD dwLevel[4];
} stSeptBaseInfoCmd_t;

/* 0x1B0C SEPTMEMBERLEAVESEPT total=14 line=8627 */
typedef struct stSeptMemberLeaveSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSeptMemberLeaveSeptCmd_t;

/* 0x1B0D VOTELISTSEPT total=20 line=8644 */
typedef struct stVoteListSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char septName[1];
    /* +4 */ DWORD dwSize[4];
    /* +1 */ BYTE flag[1];
} stVoteListSeptCmd_t;

/* 0x1B0E VOTESEPT total=16 line=8658 */
typedef struct stVoteSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char septName[1];
    /* +1 */ BYTE bySuccess[1];
} stVoteSeptCmd_t;

/* 0x1B0F NOTESEPT total=269 line=8670 */
typedef struct stNoteSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +255 */ char raw[255];
} stNoteSeptCmd_t;

/* 0x1B14 LISTSEPT total=19 line=8755 */
typedef struct stListSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
    /* +1 */ BYTE flag[1];
} stListSeptCmd_t;

/* 0x1B15 REQUESTJOINSEPT total=16 line=8774 */
typedef struct stRequestJoinSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ BYTE status[1];
} stRequestJoinSeptCmd_t;

/* 0x1B16 REQUESTNPCINFOSEPT total=14 line=8786 */
typedef struct stRequestNpcInfoSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestNpcInfoSeptCmd_t;

/* 0x1B17 REQUESTSEPTEXP total=14 line=8796 */
typedef struct stRequestSeptExpCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestSeptExpCmd_t;

/* 0x1B18 REQUESTABJURATIONSEPTNPCSEPT total=14 line=8806 */
typedef struct stRequestAbjurationSeptNpcSeptCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestAbjurationSeptNpcSeptCmd_t;

/* 0x1B19 REQSEPTNAME total=18 line=8816 */
typedef struct stReqSeptNameCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSeptID[4];
} stReqSeptNameCmd_t;

/* 0x1B1A RTNSEPTNAME total=19 line=8829 */
typedef struct stRtnSeptNameCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSeptID[4];
    /* +1 */ char name[1];
} stRtnSeptNameCmd_t;

/* 0x1B1B REQSEPTNAMELIST total=19 line=8843 */
typedef struct stReqSeptNameListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE num[1];
    /* +4 */ DWORD dwSeptID[4];
} stReqSeptNameListCmd_t;

/* 0x1B1C RTNSEPTNAMELIST total=20 line=8856 */
typedef struct stRtnSeptNameListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE num[1];
    /* +4 */ DWORD dwSeptID[4];
    /* +1 */ char name[1];
} stRtnSeptNameListCmd_t;

/* 0x1B1D REQUESTSEPTNORMALEXP total=14 line=8873 */
typedef struct stRequestSeptNormalExpCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestSeptNormalExpCmd_t;

/* 0x1C01 ACTIVEDARE total=30 line=7726 */
typedef struct stActiveDareCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ char fromRelationName[1];
    /* +1 */ char toRelationName[1];
    /* +4 */ DWORD dwWarID[4];
    /* +4 */ DWORD dwDareType[4];
    /* +4 */ DWORD dwMsgType[4];
    /* +1 */ BYTE byDareRepute[1];
} stActiveDareCmd_t;

/* 0x1D01 SETAIPET total=14 line=8908 */
typedef struct stSetAIPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSetAIPetCmd_t;

/* 0x1D02 REQUESTDATAPET total=14 line=8965 */
typedef struct stRequestDataPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestDataPetCmd_t;

/* 0x1D03 REFRESHDATAPET total=18 line=8976 */
typedef struct stRefreshDataPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stRefreshDataPetCmd_t;

/* 0x1D04 CHANGENAMEPET total=15 line=8991 */
typedef struct stChangeNamePetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stChangeNamePetCmd_t;

/* 0x1D05 ADDPETPET total=18 line=9003 */
typedef struct stAddPetPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stAddPetPetCmd_t;

/* 0x1D06 DELPETPET total=18 line=9015 */
typedef struct stDelPetPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stDelPetPetCmd_t;

/* 0x1D07 DELSKILLPET total=18 line=9027 */
typedef struct stDelSkillPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stDelSkillPetCmd_t;

/* 0x1D08 CALLHORSEPET total=14 line=9039 */
typedef struct stCallHorsePetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stCallHorsePetCmd_t;

/* 0x1D09 PUTHORSEPET total=14 line=9049 */
typedef struct stPutHorsePetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stPutHorsePetCmd_t;

/* 0x1D0A HORSEDATAPET total=18 line=9059 */
typedef struct stHorseDataPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stHorseDataPetCmd_t;

/* 0x1D0B SETTARGETPET total=22 line=9073 */
typedef struct stSetTargetPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD targetType[4];
    /* +4 */ DWORD id[4];
} stSetTargetPetCmd_t;

/* 0x1D0B CHANGEHORSEPET total=18 line=9123 */
typedef struct stChangeHorsePetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stChangeHorsePetCmd_t;

/* 0x1D0C HPEXPPET total=26 line=9086 */
typedef struct stHpExpPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
    /* +4 */ DWORD hp[4];
    /* +4 */ DWORD exp[4];
} stHpExpPetCmd_t;

/* 0x1D0D CALLFABAOPET total=18 line=9102 */
typedef struct stCallFabaoPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stCallFabaoPetCmd_t;

/* 0x1D0E PUTFABAOPET total=14 line=9113 */
typedef struct stPutFabaoPetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stPutFabaoPetCmd_t;

/* 0x2501 BUYCARTOON total=18 line=10206 */
typedef struct stBuyCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD npcID[4];
} stBuyCartoonCmd_t;

/* 0x2502 ADDCARTOON total=18 line=10216 */
typedef struct stAddCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stAddCartoonCmd_t;

/* 0x2503 REMOVECARTOON total=18 line=10228 */
typedef struct stRemoveCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stRemoveCartoonCmd_t;

/* 0x2504 FOLLOWCARTOON total=18 line=10238 */
typedef struct stFollowCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stFollowCartoonCmd_t;

/* 0x2505 LETOUTCARTOON total=18 line=10248 */
typedef struct stLetOutCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stLetOutCartoonCmd_t;

/* 0x2506 ADOPTCARTOON total=18 line=10258 */
typedef struct stAdoptCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stAdoptCartoonCmd_t;

/* 0x2507 RETURNCARTOON total=18 line=10268 */
typedef struct stReturnCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stReturnCartoonCmd_t;

/* 0x2508 GETBACKCARTOON total=14 line=10278 */
typedef struct stGetBackCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stGetBackCartoonCmd_t;

/* 0x2509 DRAWEXPCARTOON total=18 line=10287 */
typedef struct stDrawExpCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stDrawExpCartoonCmd_t;

/* 0x250A CHANGENAMECARTOON total=19 line=10297 */
typedef struct stChangeNameCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +1 */ char name[1];
} stChangeNameCartoonCmd_t;

/* 0x250B ADDWAITINGCARTOON total=29 line=10308 */
typedef struct stAddWaitingCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +4 */ DWORD npcID[4];
    /* +1 */ char name[1];
    /* +1 */ char masterName[1];
    /* +1 */ BYTE state[1];
    /* +4 */ DWORD time[4];
} stAddWaitingCartoonCmd_t;

/* 0x250C REMOVEWAITINGCARTOON total=18 line=10332 */
typedef struct stRemoveWaitingCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stRemoveWaitingCartoonCmd_t;

/* 0x250D REQUESTLISTCARTOON total=14 line=10342 */
typedef struct stRequestListCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestListCartoonCmd_t;

/* 0x250E SALECARTOON total=18 line=10351 */
typedef struct stSaleCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
} stSaleCartoonCmd_t;

/* 0x250F CHARGECARTOON total=22 line=10363 */
typedef struct stChargeCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +4 */ DWORD time[4];
} stChargeCartoonCmd_t;

/* 0x2510 HPEXPCARTOON total=26 line=10374 */
typedef struct stHpExpCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +4 */ DWORD sp[4];
    /* +4 */ DWORD exp[4];
} stHpExpCartoonCmd_t;

/* 0x2511 SELLALLCARTOON total=14 line=10386 */
typedef struct stSellAllCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSellAllCartoonCmd_t;

/* 0x2512 CONSIGNCARTOON total=19 line=10395 */
typedef struct stConsignCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +1 */ char name[1];
} stConsignCartoonCmd_t;

/* 0x2513 CONSIGNRETCARTOON total=19 line=10406 */
typedef struct stConsignRetCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +1 */ BYTE ret[1];
} stConsignRetCartoonCmd_t;

/* 0x2514 REPAIRCARTOON total=19 line=10417 */
typedef struct stRepairCartoonCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD cartoonID[4];
    /* +1 */ BYTE repair[1];
} stRepairCartoonCmd_t;

/* 0x2605 TRANSFERFUNDSTOCKSAVEUSER total=22 line=9511 */
typedef struct stTransferFundStockSaveUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwGold[4];
    /* +4 */ DWORD dwMoney[4];
} stTransferFundStockSaveUserCmd_t;

/* 0x2701 PASSOWRDSTOCKUSER total=17 line=9447 */
typedef struct stPassowrdStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char byPawword[1];
    /* +1 */ char byNew1[1];
    /* +1 */ char byNew2[1];
} stPassowrdStockUserCmd_t;

/* 0x2702 RETURNPASSWORDSTOCKISER total=15 line=9478 */
typedef struct stReturnPasswordStockIserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byReturn[1];
} stReturnPasswordStockIserCmd_t;

/* 0x2703 QUERYFUNDSTOCKUSER total=14 line=9488 */
typedef struct stQueryFundStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stQueryFundStockUserCmd_t;

/* 0x2704 RETURNFUNDSTOCKUSER total=22 line=9499 */
typedef struct stReturnFundStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwGold[4];
    /* +4 */ DWORD dwMoney[4];
} stReturnFundStockUserCmd_t;

/* 0x2706 TRANSFERFUNDSTOCKFETCHUSER total=22 line=9523 */
typedef struct stTransferFundStockFetchUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwGold[4];
    /* +4 */ DWORD dwMoney[4];
} stTransferFundStockFetchUserCmd_t;

/* 0x2707 CONSIGNGOLDSTOCKUSER total=18 line=9535 */
typedef struct stConsignGoldStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNum[4];
} stConsignGoldStockUserCmd_t;

/* 0x2708 CONSIGNMONEYSTOCKUSER total=18 line=9546 */
typedef struct stConsignMoneyStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNum[4];
} stConsignMoneyStockUserCmd_t;

/* 0x2709 CONSIGNCANCELGOLDSTOCKUSER total=18 line=9556 */
typedef struct stConsignCancelGoldStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNum[4];
} stConsignCancelGoldStockUserCmd_t;

/* 0x270A CONSIGNCANCELMONEYSTOCKUSER total=18 line=9567 */
typedef struct stConsignCancelMoneyStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNum[4];
} stConsignCancelMoneyStockUserCmd_t;

/* 0x270B REQUESTCONSIGNLISTSTOCKUSER total=14 line=9589 */
typedef struct stRequestConsignListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestConsignListStockUserCmd_t;

/* 0x270C CONSIGNGOLDLISTSTOCKUSER total=18 line=9599 */
typedef struct stConsignGoldListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
} stConsignGoldListStockUserCmd_t;

/* 0x270D CONSIGNMONEYLISTSTOCKUSER total=18 line=9610 */
typedef struct stConsignMoneyListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
} stConsignMoneyListStockUserCmd_t;

/* 0x270E REQUESTCANCELLISTSTOCKUSER total=19 line=9621 */
typedef struct stRequestCancelListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
    /* +1 */ BYTE byType[1];
} stRequestCancelListStockUserCmd_t;

/* 0x270F RETURNCANCELLISTSTOCKUSER total=20 line=9637 */
typedef struct stReturnCancelListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
    /* +1 */ BYTE byType[1];
    /* +1 */ BYTE byReturn[1];
} stReturnCancelListStockUserCmd_t;

/* 0x2710 REQUESTFIRSTTENLISTSTOCKUSER total=14 line=9649 */
typedef struct stRequestFirstTenListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestFirstTenListStockUserCmd_t;

/* 0x2711 FIRSTFIVELISTSTOCKUSER total=19 line=9664 */
typedef struct stFirstFiveListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD size[4];
} stFirstFiveListStockUserCmd_t;

/* 0x2712 PUTLISTSTOCKUSER total=23 line=9677 */
typedef struct stPutListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwNum[4];
    /* +4 */ DWORD dwPrice[4];
    /* +1 */ BYTE byType[1];
} stPutListStockUserCmd_t;

/* 0x2713 RETURNPUTLISTSTOCKUSER total=14 line=9689 */
typedef struct stReturnPutListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReturnPutListStockUserCmd_t;

/* 0x2714 REQUSTHISTORYSTOCKUSER total=22 line=9712 */
typedef struct stRequstHistoryStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD begintime[4];
    /* +4 */ DWORD num[4];
} stRequstHistoryStockUserCmd_t;

/* 0x2715 NEWHISTORYSTOCKUSER total=14 line=9725 */
typedef struct stNewHistoryStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stNewHistoryStockUserCmd_t;

/* 0x2716 HISTORYGOLDSTOCKUSER total=18 line=9736 */
typedef struct stHistoryGoldStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
} stHistoryGoldStockUserCmd_t;

/* 0x2717 HISTORYMONEYSTOCKUSER total=18 line=9747 */
typedef struct stHistoryMoneyStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
} stHistoryMoneyStockUserCmd_t;

/* 0x2718 REQUSTSELFHISTORYSTOCKUSER total=22 line=9758 */
typedef struct stRequstSelfHistoryStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD begintime[4];
    /* +4 */ DWORD num[4];
} stRequstSelfHistoryStockUserCmd_t;

/* 0x2719 SELFHISTORYGOLDSTOCKUSER total=18 line=9784 */
typedef struct stSelfHistoryGoldStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
} stSelfHistoryGoldStockUserCmd_t;

/* 0x271A SELFHISTORYMONEYSTOCKUSER total=18 line=9795 */
typedef struct stSelfHistoryMoneyStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD size[4];
} stSelfHistoryMoneyStockUserCmd_t;

/* 0x271B STOPSERVICESTOCKUSER total=14 line=9806 */
typedef struct stStopServiceStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stStopServiceStockUserCmd_t;

/* 0x271C CONSIGNCLEANLISTSTOCKUSER total=14 line=9816 */
typedef struct stConsignCleanListStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stConsignCleanListStockUserCmd_t;

/* 0x271D REQUESTSTATESTOCKUSER total=14 line=9826 */
typedef struct stRequestStateStockUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stRequestStateStockUserCmd_t;

/* 0x2801 REQUESTVOTELISTUSER total=15 line=7341 */
typedef struct stRequestVoteListUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stRequestVoteListUserCmd_t;

/* 0x2802 RETURNVOTELISTUSER total=23 line=7361 */
typedef struct stReturnVoteListUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwVoteID[4];
    /* +1 */ BYTE byType[1];
    /* +4 */ DWORD dwSize[4];
} stReturnVoteListUserCmd_t;

/* 0x2803 COMMITVOTEUSER total=22 line=7379 */
typedef struct stCommitVoteUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwVoteID[4];
    /* +4 */ DWORD dwOption[4];
} stCommitVoteUserCmd_t;

/* 0x2901 CREATEARMYUSER total=16 line=7408 */
typedef struct stCreateArmyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char armyName[1];
    /* +1 */ char genName[1];
} stCreateArmyUserCmd_t;

/* 0x2902 REQWAITGENUSER total=14 line=7423 */
typedef struct stReqWaitGenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqWaitGenUserCmd_t;

/* 0x2902 RTNWAITGENUSER total=18 line=7439 */
typedef struct stRtnWaitGenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
} stRtnWaitGenUserCmd_t;

/* 0x2904 REQARMYGENUSER total=15 line=7460 */
typedef struct stReqArmyGenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byStatus[1];
} stReqArmyGenUserCmd_t;

/* 0x2905 REQARMYLISTUSER total=15 line=7479 */
typedef struct stReqArmyListUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byType[1];
} stReqArmyListUserCmd_t;

/* 0x2906 RTNARMYLISTUSER total=19 line=7501 */
typedef struct stRtnArmyListUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwSize[4];
    /* +1 */ BYTE byType[1];
} stRtnArmyListUserCmd_t;

/* 0x2907 REQARMYSPECUSER total=18 line=7516 */
typedef struct stReqArmySpecUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwArmyID[4];
} stReqArmySpecUserCmd_t;

/* 0x2908 RTNARMYSPECUSER total=21 line=7534 */
typedef struct stRtnArmySpecUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
    /* +1 */ char cityname[1];
    /* +1 */ char genname[1];
    /* +4 */ DWORD dwSize[4];
} stRtnArmySpecUserCmd_t;

/* 0x2909 EXITARMYUSER total=14 line=7554 */
typedef struct stExitArmyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stExitArmyUserCmd_t;

/* 0x290A REMOVEARMYUSER total=18 line=7564 */
typedef struct stRemoveArmyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwArmyID[4];
} stRemoveArmyUserCmd_t;

/* 0x290B CHANGEARMYNAMEUSER total=19 line=7577 */
typedef struct stChangeArmyNameUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwArmyID[4];
    /* +1 */ char newArmyName[1];
} stChangeArmyNameUserCmd_t;

/* 0x290C ADDARMYCAPTAINUSER total=25 line=7597 */
typedef struct stAddArmyCaptainUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char capName[1];
    /* +1 */ char armyName[1];
    /* +4 */ DWORD armyID[4];
    /* +4 */ DWORD capID[4];
    /* +1 */ BYTE byState[1];
} stAddArmyCaptainUserCmd_t;

/* 0x290C CHANGEARMYGENUSER total=19 line=7631 */
typedef struct stChangeArmyGenUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwArmyID[4];
    /* +1 */ char newArmyGen[1];
} stChangeArmyGenUserCmd_t;

/* 0x290D FIREARMYCAPTAINUSER total=18 line=7618 */
typedef struct stFireArmyCaptainUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwUserID[4];
} stFireArmyCaptainUserCmd_t;

/* 0x2A01 REQDRAGONUSER total=14 line=7661 */
typedef struct stReqDragonUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqDragonUserCmd_t;

/* 0x2A02 REQTIGERUSER total=14 line=7671 */
typedef struct stReqTigerUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqTigerUserCmd_t;

/* 0x2F01 SETRECOMMENDUSER total=15 line=11231 */
typedef struct stSetRecommendUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char name[1];
} stSetRecommendUserCmd_t;

/* 0x2F02 REQRECOMMENDBOUNTYUSER total=15 line=11250 */
typedef struct stReqRecommendBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byState[1];
} stReqRecommendBountyUserCmd_t;

/* 0x2F03 REQRECOMMENDEDBOUNTYUSER total=15 line=11262 */
typedef struct stReqRecommendedBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byState[1];
} stReqRecommendedBountyUserCmd_t;

/* 0x2F04 RTNRECOMMENDBOUNTYUSER total=26 line=11281 */
typedef struct stRtnRecommendBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwBalance[4];
    /* +4 */ DWORD dwTotal[4];
    /* +4 */ DWORD dwSize[4];
} stRtnRecommendBountyUserCmd_t;

/* 0x2F05 RTNRECOMMENDEDBOUNTYUSER total=27 line=11299 */
typedef struct stRtnRecommendedBountyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD dwBalance[4];
    /* +4 */ DWORD dwTotal[4];
    /* +4 */ DWORD dwLastLevel[4];
    /* +1 */ char name[1];
} stRtnRecommendedBountyUserCmd_t;

/* 0x3001 SETSAFETYUSER total=19 line=11333 */
typedef struct stSetSafetyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD pwd[4];
    /* +1 */ BYTE state[1];
} stSetSafetyUserCmd_t;

/* 0x3002 NOTIFYSAFETYUSER total=16 line=11354 */
typedef struct stNotifySafetyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ BYTE byState[1];
    /* +1 */ BYTE safe_setup[1];
} stNotifySafetyUserCmd_t;

/* 0x3003 SETTEMPUNSAFETYUSER total=18 line=11369 */
typedef struct stSetTempUnSafetyUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD pwd[4];
} stSetTempUnSafetyUserCmd_t;

/* 0x3004 SETSAFETYDETAILUSER total=19 line=11389 */
typedef struct stSetSafetyDetailUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD pwd[4];
    /* +1 */ BYTE safe_setup[1];
} stSetSafetyDetailUserCmd_t;

/* 0x3005 SETSAFETYPASSWORDUSER total=20 line=11403 */
typedef struct stSetSafetyPasswordUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD accid[4];
    /* +1 */ char oldPwd[1];
    /* +1 */ char newPwd[1];
} stSetSafetyPasswordUserCmd_t;

/* 0x3301 SETCOWBOXKEY total=22 line=12064 */
typedef struct stSetCowBoxKeyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD Key_id[4];
    /* +4 */ DWORD qwThisID[4];
} stSetCowBoxKeyCmd_t;

/* 0x3302 GETCOWBOXINIT total=22 line=12076 */
typedef struct stGetCowBoxInitCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ int item_Ti[4];
    /* +4 */ DWORD Key_id[4];
} stGetCowBoxInitCmd_t;

/* 0x3303 SETCOWBOXTIQU total=14 line=12091 */
typedef struct stSetCowBoxTiquCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stSetCowBoxTiquCmd_t;

/* 0x3401 TURNREQUEST total=14 line=11434 */
typedef struct stTurnRequestCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTurnRequestCmd_t;

/* 0x3402 TURNRESULT total=14 line=11451 */
typedef struct stTurnResultCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTurnResultCmd_t;

/* 0x3501 HORSETRAININGUSER total=18 line=11996 */
typedef struct stHorseTrainingUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ int CmdTey[4];
} stHorseTrainingUserCmd_t;

/* 0x3502 HOUSEFACETRAININGUSER total=22 line=12008 */
typedef struct stHouseFaceTrainingUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ int CmdTey[4];
    /* +4 */ int FaceTey[4];
} stHouseFaceTrainingUserCmd_t;

/* 0x3601 LUAINPUTBOXUSER total=264 line=12035 */
typedef struct stLuaInputBoxUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +200 */ char raw[200];
    /* +50 */ char raw[50];
} stLuaInputBoxUserCmd_t;

/* 0x3801 FUJIANJIANZHONGUSER total=14 line=12169 */
typedef struct stFujianJianzhongUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFujianJianzhongUserCmd_t;

/* 0x3805 FUJIANZTZUSER total=14 line=12221 */
typedef struct stFujianZtzUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFujianZtzUserCmd_t;

/* 0x3807 FUJIANSHENGQIUSER total=14 line=12329 */
typedef struct stFujianShengqiUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFujianShengqiUserCmd_t;

/* 0x380A FUJIANSHENGXIAOUSER total=14 line=12277 */
typedef struct stFujianShengxiaoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFujianShengxiaoUserCmd_t;

/* 0x3811 FUJIANJINGMAIUSER total=14 line=12370 */
typedef struct stFujianJingmaiUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFujianJingmaiUserCmd_t;

/* 0x3815 FUJIANTOUXIANUSER total=14 line=12414 */
typedef struct stFujianTouxianUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFujianTouxianUserCmd_t;

/* 0x3901 LIANZHANSTARTUSER total=22 line=12481 */
typedef struct stLianzhanstartUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD lianzhantime[4];
    /* +4 */ DWORD lianzhannum[4];
} stLianzhanstartUserCmd_t;

/* 0x3A01 JINGCAIXIAZHU total=22 line=12512 */
typedef struct stJingcaiXiazhuCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ int index[4];
    /* +4 */ DWORD itemUID[4];
} stJingcaiXiazhuCmd_t;

/* 0x3A02 JINGCAIINFO total=14 line=12525 */
typedef struct stJingcaiInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stJingcaiInfoCmd_t;

/* 0x3A03 JINGCAIRETURNINFO total=30 line=12535 */
typedef struct stJingcaiReturnInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ int index[4];
    /* +4 */ int object_id[4];
    /* +4 */ int object_level[4];
    /* +4 */ int object_num[4];
} stJingcaiReturnInfoCmd_t;

/* 0x3A04 JINGCAIRETURNPIGTIME total=14 line=12554 */
typedef struct stJingcaiReturnPigTimeCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stJingcaiReturnPigTimeCmd_t;

/* 0x3A05 JINGCAIRETURNKILLPIGTIME total=14 line=12565 */
typedef struct stJingcaiReturnKillPigTimeCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stJingcaiReturnKillPigTimeCmd_t;

/* 0x3B01 FUBENINFO total=14 line=12601 */
typedef struct stFubenInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFubenInfoCmd_t;

/* 0x3B02 FUBENRETURNINFO total=34 line=12611 */
typedef struct stFubenReturnInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD fba1[4];
    /* +4 */ DWORD fba2[4];
    /* +4 */ DWORD fba3[4];
    /* +4 */ DWORD fba4[4];
    /* +4 */ DWORD fba5[4];
} stFubenReturnInfoCmd_t;

/* 0x3B03 FUBENIN total=22 line=12626 */
typedef struct stFubenInCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD type[4];
    /* +4 */ DWORD select[4];
} stFubenInCmd_t;

/* 0x3B04 FUBENBUY total=18 line=12638 */
typedef struct stFubenBuyCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD select[4];
} stFubenBuyCmd_t;

/* 0x3C01 ERNVINFOUSER total=14 line=12668 */
typedef struct stErnvInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stErnvInfoUserCmd_t;

/* 0x3C03 ERNVPUTUSER total=22 line=12709 */
typedef struct stErnvPutUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD babytype[4];
    /* +4 */ DWORD state[4];
} stErnvPutUserCmd_t;

/* 0x3D01 TIANXIAINFO total=14 line=12788 */
typedef struct stTianxiaInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTianxiaInfoCmd_t;

/* 0x3D02 TIANXIARETURNINFO total=14 line=12807 */
typedef struct stTianxiaReturnInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTianxiaReturnInfoCmd_t;

/* 0x3D03 TIANXIABAOMING total=14 line=12818 */
typedef struct stTianxiaBaomingCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTianxiaBaomingCmd_t;

/* 0x3D04 TIANXIABAOMINBACK total=14 line=12828 */
typedef struct stTianxiaBaominBackCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTianxiaBaominBackCmd_t;

/* 0x3D05 TIANXIADUANWEI total=14 line=12838 */
typedef struct stTianxiaDuanweiCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stTianxiaDuanweiCmd_t;

/* 0x3E01 BIKESTART total=14 line=12866 */
typedef struct stBikeStartCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stBikeStartCmd_t;

/* 0x3E02 DELBIKE total=14 line=12877 */
typedef struct stDelBikeCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stDelBikeCmd_t;

/* 0x3E03 BIKESCNL total=18 line=12887 */
typedef struct stBikeScnlCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD npcid[4];
} stBikeScnlCmd_t;

/* 0x3E04 BIKESKILL total=26 line=12900 */
typedef struct stBikeSkillCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD type[4];
    /* +4 */ DWORD playerid[4];
    /* +4 */ DWORD itemID[4];
} stBikeSkillCmd_t;

/* 0x3E05 BIKEADDITEM total=18 line=12913 */
typedef struct stBikeAdditemCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemID[4];
} stBikeAdditemCmd_t;

/* 0x3E06 BIKESTATE total=18 line=12925 */
typedef struct stBikeStateCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stBikeStateCmd_t;

/* 0x3F01 BIESHUINFO total=14 line=12955 */
typedef struct stBieShuInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stBieShuInfoCmd_t;

/* 0x3F02 BIESHUINFORETURN total=38 line=12965 */
typedef struct stBieShuInfoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD bieshu_level[4];
    /* +4 */ DWORD bieshu_exp[4];
    /* +4 */ DWORD bieshu_state[4];
    /* +4 */ DWORD bieshu_gong[4];
    /* +4 */ DWORD bieshu_fang[4];
    /* +4 */ DWORD bieshu_hp[4];
} stBieShuInfoReturnCmd_t;

/* 0x3F03 BIESHUIN total=14 line=12981 */
typedef struct stBieShuInCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stBieShuInCmd_t;

/* 0x3F04 BIESHUJIHUO total=18 line=12991 */
typedef struct stBieShuJiHuoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemID[4];
} stBieShuJiHuoCmd_t;

/* 0x3F05 BIESHUJIHUORETURN total=18 line=13003 */
typedef struct stBieShuJiHuoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stBieShuJiHuoReturnCmd_t;

/* 0x3F06 BIESHULEVELUP total=18 line=13014 */
typedef struct stBieShuLevelUpCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemID[4];
} stBieShuLevelUpCmd_t;

/* 0x3F07 BIESHULEVENUPRETURN total=18 line=13025 */
typedef struct stBieShuLevenUpReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stBieShuLevenUpReturnCmd_t;

/* 0x4001 FENGYININFO total=14 line=13056 */
typedef struct stFengyinInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stFengyinInfoCmd_t;

/* 0x4002 FENGYININFORETURN total=18 line=13066 */
typedef struct stFengyinInfoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
} stFengyinInfoReturnCmd_t;

/* 0x4101 HUISHOUINFO total=14 line=13096 */
typedef struct stHuishouInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stHuishouInfoCmd_t;

/* 0x4102 HUISHOUINFORETURN total=14 line=13114 */
typedef struct stHuishouInfoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stHuishouInfoReturnCmd_t;

/* 0x4201 ZHUANHUAN total=26 line=13167 */
typedef struct stZhuanhuanCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemUID[4];
    /* +4 */ DWORD zhiye[4];
    /* +4 */ DWORD type[4];
} stZhuanhuanCmd_t;

/* 0x4202 ZHUANHUANRETURN total=18 line=13180 */
typedef struct stZhuanhuanReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stZhuanhuanReturnCmd_t;

/* 0x4301 XIULIANINFO total=14 line=13210 */
typedef struct stXiulianInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stXiulianInfoCmd_t;

/* 0x4302 XIULIANINFORETURN total=98 line=13220 */
typedef struct stXiulianInfoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD xiulianexp[4];
    /* +4 */ DWORD level1[4];
    /* +4 */ DWORD level2[4];
    /* +4 */ DWORD level3[4];
    /* +4 */ DWORD level4[4];
    /* +4 */ DWORD level5[4];
    /* +4 */ DWORD exp1[4];
    /* +4 */ DWORD exp2[4];
    /* +4 */ DWORD exp3[4];
    /* +4 */ DWORD exp4[4];
    /* +4 */ DWORD exp5[4];
    /* +4 */ DWORD money1[4];
    /* +4 */ DWORD money2[4];
    /* +4 */ DWORD money3[4];
    /* +4 */ DWORD money4[4];
    /* +4 */ DWORD money5[4];
    /* +4 */ DWORD num1[4];
    /* +4 */ DWORD num2[4];
    /* +4 */ DWORD num3[4];
    /* +4 */ DWORD num4[4];
    /* +4 */ DWORD num5[4];
} stXiulianInfoReturnCmd_t;

/* 0x4303 XIULIANSTART total=18 line=13255 */
typedef struct stXiulianStartCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD type[4];
} stXiulianStartCmd_t;

/* 0x4304 XIULIANRETURN total=18 line=13266 */
typedef struct stXiulianReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stXiulianReturnCmd_t;

/* 0x4305 XIULIANRESET total=14 line=13277 */
typedef struct stXiulianResetCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stXiulianResetCmd_t;

/* 0x4501 ZHANCHELIST total=14 line=13421 */
typedef struct stZhancheListCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheListCmd_t;

/* 0x4502 ZHANCHELISTRETURN total=14 line=13442 */
typedef struct stZhancheListReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheListReturnCmd_t;

/* 0x4503 ZHANCHELINGYONG total=18 line=13453 */
typedef struct stZhancheLingyongCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD id[4];
} stZhancheLingyongCmd_t;

/* 0x4504 ZHANCHELINGYONGRETURN total=18 line=13464 */
typedef struct stZhancheLingyongReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stZhancheLingyongReturnCmd_t;

/* 0x4505 ZHANCHEADD total=14 line=13475 */
typedef struct stZhancheAddCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheAddCmd_t;

/* 0x4506 ZHANCHETIHUAN total=14 line=13485 */
typedef struct stZhancheTihuanCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheTihuanCmd_t;

/* 0x4507 ZHANCHESHOUHUI total=14 line=13495 */
typedef struct stZhancheShouhuiCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheShouhuiCmd_t;

/* 0x4508 ZHANCHEZHAOHUAN total=14 line=13505 */
typedef struct stZhancheZhaohuanCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheZhaohuanCmd_t;

/* 0x4509 ZHANCHEGUIHUAN total=14 line=13515 */
typedef struct stZhancheGuihuanCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheGuihuanCmd_t;

/* 0x450A ZHANCHETUOLI total=14 line=13525 */
typedef struct stZhancheTuoliCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZhancheTuoliCmd_t;

/* 0x450B ZHANCHESKILL total=26 line=13535 */
typedef struct stZhancheSkillCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD type[4];
    /* +4 */ DWORD playerid[4];
    /* +4 */ DWORD itemID[4];
} stZhancheSkillCmd_t;

/* 0x4601 JIAZUBOSSINFO total=14 line=13564 */
typedef struct stJiazuBossInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stJiazuBossInfoCmd_t;

/* 0x4602 JIAZUBOSSINFORETURN total=22 line=13574 */
typedef struct stJiazuBossInfoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
    /* +4 */ DWORD exp[4];
} stJiazuBossInfoReturnCmd_t;

/* 0x4603 JIAZUBOSSWEIYANG total=18 line=13587 */
typedef struct stJiazuBossWeiyangCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemid[4];
} stJiazuBossWeiyangCmd_t;

/* 0x4604 JIAZUBOSSWEIYANGRETURN total=18 line=13598 */
typedef struct stJiazuBossWeiyangReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stJiazuBossWeiyangReturnCmd_t;

/* 0x4605 JIAZUBOSSZHAOHUAN total=14 line=13609 */
typedef struct stJiazuBossZhaohuanCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stJiazuBossZhaohuanCmd_t;

/* 0x4701 ZUOQIINFO total=14 line=13636 */
typedef struct stZuoqiInfoCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZuoqiInfoCmd_t;

/* 0x4702 ZUOQIINFORETURN total=82 line=13646 */
typedef struct stZuoqiInfoReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD level[4];
    /* +4 */ DWORD exp[4];
    /* +4 */ DWORD num1[4];
    /* +4 */ DWORD num2[4];
    /* +4 */ DWORD num3[4];
    /* +4 */ DWORD num4[4];
    /* +4 */ DWORD num5[4];
    /* +4 */ DWORD num6[4];
    /* +4 */ DWORD num7[4];
    /* +4 */ DWORD num8[4];
    /* +4 */ DWORD num9[4];
    /* +4 */ DWORD num10[4];
    /* +4 */ DWORD num11[4];
    /* +4 */ DWORD num12[4];
    /* +4 */ DWORD num13[4];
    /* +4 */ DWORD num14[4];
    /* +4 */ DWORD num15[4];
} stZuoqiInfoReturnCmd_t;

/* 0x4703 ZUOQILEVELUP total=18 line=13673 */
typedef struct stZuoqiLevelUpCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD itemid[4];
} stZuoqiLevelUpCmd_t;

/* 0x4704 ZUOQILEVELUPRETURN total=18 line=13684 */
typedef struct stZuoqiLevelUpReturnCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD state[4];
} stZuoqiLevelUpReturnCmd_t;

/* 0x4705 ZUOQILINGQU total=14 line=13695 */
typedef struct stZuoqiLingQuCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stZuoqiLingQuCmd_t;

/* 0x4801 MIGONGNEXT total=18 line=13722 */
typedef struct stMigongNextCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +4 */ DWORD npcid[4];
} stMigongNextCmd_t;

/* 0x4901 REQKILLERBOSSINFOUSER total=14 line=12118 */
typedef struct stReqKillerBossInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
} stReqKillerBossInfoUserCmd_t;

/* 0x4902 RUNKILLERBOSSINFOUSER total=39 line=12128 */
typedef struct stRunKillerBossInfoUserCmd {
    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;
    /* +1 */ BYTE byCmd[1];
    /* +1 */ BYTE byParam[1];
    /* +1 */ BYTE byCmdType[1];
    /* +1 */ BYTE byParameterType[1];
    /* +4 */ DWORD dwTimestamp[4];
    /* +1 */ char MapName[1];
    /* +2 */ WORD TypeID[2];
    /* +2 */ WORD Pagenum[2];
    /* +2 */ WORD Number[2];
    /* +4 */ DWORD NpcID[4];
    /* +4 */ DWORD CountryID[4];
    /* +2 */ WORD NpcState[2];
    /* +8 */ QWORD ReviveTime[8];
} stRunKillerBossInfoUserCmd_t;

#pragma pack(pop)
