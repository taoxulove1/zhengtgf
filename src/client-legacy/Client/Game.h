#ifndef _GAME_GAME_H
#define _GAME_GAME_H

#include "./UserLocalInfo.h"

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
enum enumSoundType;
class CMusic;
struct stNullUserCmd;
extern bool CreateGameSystem();
extern void DestroyGameSystem();
extern bool InitGame(bool bConnServer = true);
extern void UninitGame();
extern void ExitGame();
extern void HandleCommand();
extern bool LoginGame(const char* pszUserID,const char* pszPasswd, const char* pszJpegPassport );
extern void LogoutGame(bool bShowMessage = false);
extern bool ConnectionLoginServer();


extern bool g_bSinglePlayer;	// 没有服务器的状态，单机状态
extern const char* g_pszLoginErrorMessage;
extern CUserLocalInfo g_UserLocalInfo; //用户本地信息文件
//Jpeg passport MemStream
extern std::vector<BYTE> g_JpegPassport;
extern CMusic* GetGameBackMusic();
extern const DWORD c_dwMusicFadeTime;
extern void PlayBackMusic(const char* pszFileName,int dwFadeInTime = c_dwMusicFadeTime,int iMinInternal = 0,int iMaxInternal=0,bool bForce = true);
extern bool IsBackMusicPlaying();
extern void PlayGameSound(const char* pszFileName,enumSoundType type = enumSoundType(-1) );
extern void PlayItemSound(DWORD dwSoundID);
extern void PlayUISound(DWORD dwSoundID);
extern void RestoreMouseIcon();

extern int g_nItemTakeUpSound;
extern int g_nItemAirSound;
extern int g_nMoneySound;
//ItemExchangeMap
struct stItemExchangeInfo
{
	DWORD	dwObjectTypeID;
	char ownerName[MAX_NAMESIZE];
	DWORD	dwAddTime;
	//t_Object itemInfo;
};
typedef std::map<DWORD,stItemExchangeInfo>		mapItemExchange;
extern mapItemExchange		g_itemExchangeList;
extern std::vector<std::string> g_vecInteriorMapName;
extern bool g_bSinglePlayer;

class IEncrypt;
class CAccoutSaveInfo
{
public:
	CAccoutSaveInfo();
	~CAccoutSaveInfo();

	// just for convenience,use the client's key ~.~
	void SetUserID(const char* pszUserID );
	void SetUserPass(const char *pszPasswd);
	void SetUserName(const char * pszName);
	std::string GetUserID();
	std::string GetUserPass();
	std::string GetUserName();

	std :: list<BYTE> m_Mykey;
  
	static CAccoutSaveInfo* GetSingleton()
	{
		Assert(Singleton != 0);
		return Singleton;
	}
protected:
	static CAccoutSaveInfo* Singleton;

private:
	char       m_szUserID[48];
	size_t     m_LenUserID;

	char        m_szPasswd[33];
	size_t      m_lenPasswd;

	char		m_szPlayerName[33];
	size_t        m_lenPlayerName;

	IEncrypt*			m_pEncrypt;
	void EncryptString(unsigned char * pString,int enc,size_t &len);


};

////////////////////////////////////////////////////////////////
// 返回登陆界面是否自动重连
#define _LOGIN_SERVER_MANUAL
////////////////////////////////////////////////////////////////
//

static int g_WeakCountry3[24];

//

#endif