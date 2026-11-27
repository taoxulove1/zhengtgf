/* Packed layouts verified against the server Command.h (32-bit ABI). */
#pragma once
#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint8_t byCmd;
    uint8_t byParam;
    uint32_t dwTimestamp;
} ZtP2_stNullUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t version;
} ZtP2_stUserVerifyVerCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint8_t pstrName[48];
    uint8_t pstrPassword[16];
    uint16_t game;
    uint16_t zone;
    uint8_t jpegPassport[7];
    uint8_t mac_addr[13];
} ZtP2_stUserRequestLoginCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint8_t byReturnCode;
} ZtP2_stServerReturnLoginFailedCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t dwUserID;
    uint32_t loginTempID;
    uint8_t pstrIP[16];
    uint16_t wdPort;
    uint8_t key[256];
} ZtP2_stServerReturnLoginSuccessCmd;

typedef struct {
    uint32_t id;
    uint8_t enableRegister;
    uint8_t enableLogin;
    uint8_t pstrName[32];
} ZtP2_Country_Info;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint16_t size;
} ZtP2_stCountryInfoUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
} ZtP2_stRequestSelectUserCmd;

typedef struct {
    uint32_t id;
    uint8_t name[33];
    uint16_t type;
    uint16_t level;
    uint32_t mapid;
    uint8_t mapName[33];
    uint16_t country;
    uint16_t face;
    uint8_t countryName[33];
    uint32_t bitmask;
} ZtP2_SelectUserInfo;

typedef struct {
    ZtP2_stNullUserCmd header;
    ZtP2_SelectUserInfo charInfo[2];
    uint16_t size;
} ZtP2_stUserInfoUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t charNo;
    uint8_t jpegPassport[7];
} ZtP2_stLoginSelectUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t width;
    uint32_t height;
    uint8_t pstrMapName[32];
    uint8_t pstrFilename[32];
    uint32_t setting;
    uint32_t rgb;
    uint8_t pstrGroupName[32];
    uint8_t pstrCountryName[32];
    uint8_t pstrCityName[32];
    int32_t mainRoleX;
    int32_t mainRoleY;
    int16_t npc_count;
} ZtP2_stMapScreenSizeDataUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
} ZtP2_stLoadMapOKDataUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
} ZtP2_stEndOfInitDataDataUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
} ZtP2_stPingTimeTimerUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t dwUserTempID;
    uint8_t byDirect;
    uint8_t bySpeed;
    uint16_t x;
    uint16_t y;
} ZtP2_stUserMoveMoveUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint8_t byActionType;
    uint8_t object[4390];
} ZtP2_stAddObjectPropertyUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t qwThisID;
} ZtP2_stRemoveObjectPropertyUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t qwThisID;
    uint32_t dwNum;
} ZtP2_stRefCountObjectPropertyUserCmd;

typedef struct {
    ZtP2_stNullUserCmd header;
    uint32_t qwThisID;
    uint32_t dwNumber;
} ZtP2_stUseObjectPropertyUserCmd;

#pragma pack(pop)
