#include "public.h"
#include "game.h"
#include "miniusercommand.h"
#include "../Magic/iMagic.h"
//#include "./KillProcess.h"
#include "./GameHookThread.h"
#include "./GameGuiManager.h"
#include "./GameTime.h"
#include "Maincharacter.h"
#include "GameScene.h"
#include "./GuiLogin.h"
#include "./GameState.h"
#include "./GameAppation.h"
#include "./GuiSelectCharDialog.h"
#include "./GuiCreateCharDialog.h"
#include "./GuiRegisterDialog.h"
#include "./GuiModifyPasswd.h"
#include "./GuiItem.h"
#include "./GuiHelp.h"
#include "./GuiSaveBox.h"
#include "./RoleItem.h"
#include "./GameCursor.h"
#include "./GuiMain.h"
#include "./GuiChatOutput.h"
#include "./GuiPrivateChat.h"
#include "./GuiTrade.h"
#include "./GuiNpcDialog.h"
#include "./GameAccelKeyManager.h"
#include "./GuiMiniMap.h"
#include "./GuiTeam.h"
#include "./GuiSkill.h"
#include "./GuiFloatAccessKey.h"
#include "./CreateRole.h"
#include "./GuiWorldConn.h"
#include "./GuiNpcMake.h"
#include "./GuiTaskDialog.h"
#include "./GuiChatSelect.h"
#include "./GuiItemDetailInfo.h"
#include "../Media/Music.h"
#include "./GuiShopDialog.h"
#include "./GuiSysChat.h"
#include "./GuiMessageBox.h"
#include "./GuiUserInfo.h"
#include "./CharacterProperty.h"
#include "./GuiNpcTrade.h"
#include "../Media/SoundManager.h"
#include "./GuiCompetition.h"
#include "./GuiEmotionDialog.h"
#include "./GuiChangeWarField.h"
#include "./GuiCountryWarQuery.h"
#include "./Mail.h"
#include "./GuiSelectDialog.h"
#include "./GuiUnoinPower.h"
#include "./GuiOthersShopDialog.h"
#include "./GuiGoldExchangeDialog.h"
#include "./Country.h"
#include "./CountryInfo.h"
#include "./CartoonPet.h"
#include "./GuiStock.h"
#include "./Item.h"
#include ".\GuiPetPackDialog.h"
#include "./GuiExitGameDlg.h"
#include "./MGNumberDlg.h"

#include "./GuiRecoSys_B_Query_TakeDlg.h"
#include "./GuiRecoSys_A_Query_TakeDlg.h"

#include ".\NetAutoconnect.h"

#include "./GuiStockDialog.h"
#include "./Chat.h"
#include "./FindDLL.h"
#include "../EncDec/include/EncDec.h"
#include <string>

#include "LordStrikeManager.h"
#include "LordStrikeLobby.h"
#include "LordStrikeGame.h"

#include "./UserLocalInfo.h"
CUserLocalInfo g_UserLocalInfo;

// mac address 
#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

static CAccoutSaveInfo g_AccountInfo;
CAccoutSaveInfo * CAccoutSaveInfo::Singleton = NULL;
//++++++++++++++++++++++++++++++++++++++++++++++++++
CAccoutSaveInfo::CAccoutSaveInfo()
{
	Assert(0 == Singleton);
	Singleton = this;

	memset(m_szUserID,0,sizeof(m_szUserID));
	memset(m_szPasswd,0,sizeof(m_szUserID));
	memset(m_szPlayerName,0,sizeof(m_szUserID));

	srand(timeGetTime());
	m_Mykey.clear();
	for(size_t i = 0 ; i < 8;i++)
		m_Mykey.push_back(rand() / 255); 

	m_pEncrypt = new CEncrypt;
}
CAccoutSaveInfo::~CAccoutSaveInfo()
{
	Singleton = 0;

	if(m_pEncrypt)
	{
		delete m_pEncrypt;
		m_pEncrypt  = NULL;
	}
}

// just for convenience,use the client's key 
void CAccoutSaveInfo::EncryptString(unsigned char * pString,int enc,size_t &llen)
{
	DES_key_schedule key;
	BYTE keyData[8];
	size_t keySize;
	Client_GetKey(m_Mykey,keyData,keySize); // not the m_LoginKey,it's 16
	m_pEncrypt->DES_set_key(keyData,&key);
	for(size_t i = 0 ; i < 8;i++)
		TRACE("%d ",keyData[i]);
	TRACE("\n");

	//Fatal error,not use strlen :  
	// size_t len = strlen((const char *)pString);
	llen = (llen  + 7) & (~7);
	size_t count = llen/8;

	for(size_t i=0;i<count;++i)
		m_pEncrypt->DES_encrypt1((pString+i*8),&key,enc);	
}


void CAccoutSaveInfo::SetUserID(const char* pszUserID )
{
	m_LenUserID = strlen(pszUserID);
	if(m_LenUserID >= sizeof(m_szUserID))
		m_LenUserID = sizeof(m_szUserID) - 1;
	strncpy(m_szUserID,pszUserID,m_LenUserID);
	m_szUserID[m_LenUserID] = 0;	
	EncryptString((unsigned char *)m_szUserID,DES_ENCRYPT,m_LenUserID);
}

void CAccoutSaveInfo::SetUserPass(const char *pszPasswd)
{
	m_lenPasswd = strlen(pszPasswd);
	if(m_lenPasswd >= sizeof(m_szPasswd))
		m_lenPasswd = sizeof(m_szPasswd) - 1;

	strncpy(m_szPasswd,pszPasswd,m_lenPasswd);
	m_szPasswd[m_lenPasswd] = 0;
	EncryptString((unsigned char *)m_szPasswd,DES_ENCRYPT,m_lenPasswd);
}

void CAccoutSaveInfo::SetUserName(const char * pszName)
{
	m_lenPlayerName = strlen(pszName);
	if(m_lenPlayerName >= sizeof(m_szPlayerName))
		m_lenPlayerName = sizeof(m_szPlayerName) - 1;
	strncpy(m_szPlayerName,pszName,m_lenPlayerName);
	m_szPlayerName[m_lenPlayerName] = 0;
	EncryptString((unsigned char *)m_szPlayerName,DES_ENCRYPT,m_lenPlayerName);
}

std::string CAccoutSaveInfo::GetUserID()
{
	char szUserID[48];
	memcpy(szUserID,m_szUserID,m_LenUserID);
	szUserID[m_LenUserID] = 0;
	EncryptString((unsigned char *)szUserID,DES_DECRYPT,m_LenUserID);
	return szUserID;
}
std::string CAccoutSaveInfo::GetUserPass()
{
	char szPasswd[33];
	memcpy(szPasswd,m_szPasswd,m_lenPasswd);
	szPasswd[m_lenPasswd] = 0;
	EncryptString((unsigned char *)szPasswd,DES_DECRYPT,m_lenPasswd);
	return szPasswd;
}
std::string CAccoutSaveInfo::GetUserName()
{
	char szPlayerName[48];
	memcpy(szPlayerName,m_szPlayerName,m_lenPlayerName);
	szPlayerName[m_lenPlayerName] = 0;
	EncryptString((unsigned char *)szPlayerName,DES_DECRYPT,m_lenPlayerName);
	return szPlayerName;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++

bool g_bSinglePlayer  = false;
#define RETURN(Val) { retval = (Val); goto _ret;}
const char* g_pszLoginErrorMessage = NULL;

int g_nItemTakeUpSound = 0;
int g_nItemAirSound = 0;
int g_nMoneySound = 0;
//Jpeg passport memstream
std::vector<BYTE> g_JpegPassport;

//Item exchange map
mapItemExchange	g_itemExchangeList;

std::vector<std::string> g_vecInteriorMapName;


DWORD g_dwGameStartTime = 0;
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
#define WM_MUSIC_EVENT (WM_APP + 101)
CMusic* GetGameBackMusic()
{
	FUNCTION_BEGIN;

	static CMusic sm(Engine_GetHWnd(),WM_MUSIC_EVENT);
	return &sm;

	FUNCTION_END;
}

void LoadInteriorName()
{
	FUNCTION_BEGIN;

	/*std::ifstream f("data\\map\\interior.txt");
	while (f.good())
	{
	std::string s;
	getline(f,s);
	g_vecInteriorMapName.push_back(s);
	}*/

	TiXmlDocument doc;
	Stream* pStream = OpenPackFileForRead("data\\map.pak","map\\interior.xml");
	if(!pStream)
	{
		Engine_WriteLog("装载 interior.xml.失败\n");
		return ;
	}
	if(!LoadXMLFromStream(pStream,doc))
	{
		ClosePackFileForRead(pStream);
		Engine_WriteLog("解析 interior.xml 失败\n");
		return;
	}
	TiXmlElement* pMaps = doc.FirstChildElement("maps");
	if(pMaps)
	{
		TiXmlElement* pmap = pMaps->FirstChildElement("map");
		while(pmap)
		{

			const char* pname = pmap->Attribute("name");
			if( pname )
			{
				std::string strName = pname;
				g_vecInteriorMapName.push_back(strName);
			}

			pmap = pmap->NextSiblingElement("map");
		}
	}
	ClosePackFileForRead(pStream);


	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszFileName : 描述
* \param dwFadeInTime : 描述
* \return 返回值的描述
*/
void PlayBackMusic(const char* pszFileName,int dwFadeInTime,int iMinInternal,int iMaxInternal,bool bForce)
{
	FUNCTION_BEGIN;

	if(pszFileName == NULL || pszFileName[0] == 0)
	{
		GetGameBackMusic()->PlayNext("",dwFadeInTime,iMinInternal,iMaxInternal,!bForce);
	}
	else
	{
		char szFileName[MAX_PATH];
		//sprintf(szFileName,"sound\\music\\%s",pszFileName);
		sprintf(szFileName,"data\\music\\%s",pszFileName);
		if(strrchr(szFileName,'.') == NULL)
		{
			strcat(szFileName,".mp3");
		}
		GetGameBackMusic()->PlayNext(szFileName,dwFadeInTime,iMinInternal,iMaxInternal,!bForce);
	}

	FUNCTION_END;
}

bool IsBackMusicPlaying()
{
	return GetGameBackMusic()->IsPlaying();
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszFileName : 描述
* \return 返回值的描述
*/
void PlayGameSound(const char* pszFileName,enumSoundType type)
{
	FUNCTION_BEGIN;

	GetSoundManager()->Play(pszFileName,type);

	FUNCTION_END;
}


enum KILL_TYPE
{
	KILL_FOREIGN,   // 本国人杀外国人
	KILL_NATIVE,    // 外国人杀本国人
};
// 播放本国人杀外国人或者外国人杀本国人的ui音效，包含时间间隔时间和不能同时播放控制
void PlayKillerSound(const KILL_TYPE &  killtype)
{
	struct stKillSound
	{
		KILL_TYPE type;
		DWORD     dwSound;
	};

	const stKillSound c_KillItem[] = 
	{
		{ KILL_NATIVE, 10160},
		{ KILL_FOREIGN, 10161},
	};

	static char szLastFileName[MAX_PATH] = {0};
	if( strlen(szLastFileName) > 0 && GetSoundManager()->IsPlaying(szLastFileName))
	{  
		return;
	}

	for( size_t i = 0 ; i < count_of(c_KillItem) ; i++)
	{
		if( c_KillItem[i].type == killtype)
		{
			memset(szLastFileName,0,MAX_PATH);
			sprintf(szLastFileName,"sound\\ui\\%d.wav", c_KillItem[i].dwSound );
			szLastFileName[MAX_PATH-1] = 0;
			PlayGameSound( szLastFileName,SoundType_UI);   
			break;
		}
	}
}

// 旗帜受到攻击
void PlayBannerSound()
{
	char szLastFileName[MAX_PATH] = "sound\\npc\\60307.wav";
	if( strlen(szLastFileName) > 0 && GetSoundManager()->IsPlaying(szLastFileName))
	{  
		return;
	}
	PlayGameSound( szLastFileName,SoundType_UI);  
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param dwSoundID : 描述
* \return 返回值的描述
*/
void PlayItemSound(DWORD dwSoundID)
{
	FUNCTION_BEGIN;

	char szFileName[MAX_PATH];
	sprintf(szFileName,"sound\\item\\%05d.wav",dwSoundID);
	PlayGameSound(szFileName,SoundType_UI);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param dwSoundID : 描述
* \return 返回值的描述
*/
void PlayUISound(DWORD dwSoundID)
{
	FUNCTION_BEGIN;

	char szFileName[MAX_PATH];
	sprintf(szFileName,"sound\\ui\\%05d.wav",dwSoundID);
	PlayGameSound(szFileName,SoundType_UI);

	FUNCTION_END;
}

/**
* \brief 恢复鼠标默认状态
* 
* 恢复鼠标默认状态
* 
* \return 无
*/
void RestoreMouseIcon()
{
	FUNCTION_BEGIN;

	GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorNormal);

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CreateGameSystem()
{
	FUNCTION_BEGIN;

	Magic_InitModule();

	//GetHookThread()->BeginThread(GameHookThread,0,GetHookThread());

	if(!LoadTables()) 
	{
		Engine_WriteLog("LoadTables failed!\n");
		return false;
	}

	LoadInteriorName();

	//----------------------------------------------------------------------
	extern int g_defaultButtonClickSound;
	extern int g_defaultTabClickSound;
	extern int g_defaultDlgOpenSound;
	extern int g_defaultDlgHideSound;
	CIni ini;
	ini.Open("data\\datas.pak","datas\\sound.ini");
	g_nItemTakeUpSound = ini.GetInt("item","takeup",0);
	g_nItemAirSound = ini.GetInt("item","air",0);
	g_nMoneySound = ini.GetInt("item","money",0);
	g_defaultButtonClickSound = ini.GetInt("UI","buttonclick",0);
	g_defaultTabClickSound = ini.GetInt("UI","tabclick",0);
	g_defaultDlgOpenSound = ini.GetInt("UI","dlgopen",0);
	g_defaultDlgHideSound = ini.GetInt("UI","dlghide",0);


	/////////////////////////////////////////////////////////////////
	float factor = ini.GetFloat("soundfactor","scene",1.0f);
	GetSoundManager()->SetVolumeFactor(SoundType_Scene,factor);
	factor = ini.GetFloat("soundfactor","circumstance",1.0f);
	GetSoundManager()->SetVolumeFactor(SoundType_Circumstance,factor);

	factor = ini.GetFloat("soundfactor","ui",1.0f);
	GetSoundManager()->SetVolumeFactor(SoundType_UI,factor);
	factor = ini.GetFloat("soundfactor","music",1.0f);
	GetSoundManager()->SetVolumeFactor(SoundType_Music,factor);
	//----------------------------------------------------------------------

	if(!InitGame())
		return false;
	//PlayBackMusic("login.mp3");

#ifndef _AUTOPLAY
	//GetClientKillProcess()->CreateFromClient();
#endif
	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void DestroyGameSystem()
{
	FUNCTION_BEGIN;
/*#ifndef _AUTOPLAY
	GetClientKillProcess()->End();
#endif
	GetServerKillProcess()->End();
*/
	GetGameBackMusic()->UnInit();

	char tempSave[MAX_PATH];
	sprintf(tempSave,"data\\userdb%d%s",g_UserLocalInfo.GetCurZone(),".xml");
	g_UserLocalInfo.SaveUserLocalFile(tempSave);

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param bConnServer : 描述
* \return 返回值的描述
*/
bool InitGame(bool bConnServer)
{
	FUNCTION_BEGIN;

	if( bConnServer)
	{
		if(g_bSinglePlayer) GetClient()->Create();

		GetClient()->m_bLoginRecv = true;
		GetClient()->m_bLoginSend = true;
	}



	//退到登陆画面或退到选择人物界面是要调用
	ResetLoginDevice();

	GetGameAccelKeyManager()->Release();

	GetGameGuiManager()->Init();
	GetGameGuiManager()->AddGuiLogin();
	GetGameGuiManager()->AddItem();
	GetGameGuiManager()->AddSavingBoxDlg();
	GetGameGuiManager()->AddFloatItem();
	GetGameGuiManager()->AddTrade();
	GetGameGuiManager()->AddMiniMap();
	GetGameGuiManager()->AddWorldConn();
//GetGameGuiManager()->AddNearby();
	GetGameGuiManager()->AddTaskDialog();
//	GetGameGuiManager()->AddUserInfo();
	GetGameGuiManager()->AddMain();
	GetGameGuiManager()->AddPetPackDlg();

	GetGameGuiManager()->AddItemDetailInfo();
	GetGameGuiManager()->m_guiItemDetailInfo->SetVisible( false );

	//GetGameGuiManager()->AddChatInvate();	

	GetGameGuiManager()->m_guiMiniMap->SetVisible(false);
	GetGameGuiManager()->m_guiItem->SetVisible(false);
	GetGameGuiManager()->m_guiSavingBoxDlg->SetVisible(false);
	GetGameGuiManager()->m_guiTrade->SetVisible(false);
	GetGameGuiManager()->m_guiWorldConn->SetVisible(false);
//	GetGameGuiManager()->m_guiNearby->SetVisible(false);
	GetGameGuiManager()->m_guitaskdialog->SetVisible(false);
	//GetGameGuiManager()->m_guiUserInfo->SetVisible(false);
	GetGameGuiManager()->m_guiMain->SetVisible(false);
//GetGameGuiManager()->m_guiInvite->SetVisible(false);
	GetGameGuiManager()->m_guiPetPackDlg->SetVisible(false);

	//Connnect logon server
	if (bConnServer)
	{
#ifndef _LOGIN_SERVER_MANUAL
		if ( !g_bSinglePlayer && !ConnectionLoginServer() )
		{
			GetGameGuiManager()->AddMessageBox("连接服务器失败！",eAction_ConnectLoginFailed);
		}
#endif
	}

	//
	GetScene()->Create();


	/*if (g_bSinglePlayer)
	{
	CGuiUnoinPower* pDlg = GetGameGuiManager()->AddUnoinPowerDialog("aaaaaaaa");
	BYTE power[2]={0};
	set_state(power,CHANGE_ALIAS);
	set_state(power,ADD_MEMBER);
	pDlg->SetPower(power);
	}*/

	//////////////////////////////////////////////////////////////////////////
	g_WeakCountry3[0] = 0;
	g_WeakCountry3[1] = 0;
	g_WeakCountry3[2] = 0;
	//////////////////////////////////////////////////////////////////////////

	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void UninitGame()
{
	FUNCTION_BEGIN;
	GetGameGuiManager()->EndNpcDialog();
	GetClient()->Destroy();
	GetScene()->Destroy();
	GetGameGuiManager()->Release();
	GetGameState()->SetState(eGameState_Null);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool StartGame()
{
	FUNCTION_BEGIN;
	//进入游戏时要调用
	ClearResEntryGame();

	GetDevice()->SetRenderScene(true);
	GetDevice()->Reset(Engine_GetAppation()->GetConfigBase()->screenWidth,Engine_GetAppation()->GetConfigBase()->screenHeight,Engine_GetAppation()->GetConfigBase()->colorBits,GetDevice()->GetFlags());

	//GetGameTime()->InitTimestamp();

	GetGameState()->SetState(eGameState_Run);
	GetGameState()->SetStateItem(eGameStateItem_Normal);

	if(GetGameGuiManager()->m_guiLogin)
	{
		GetGameGuiManager()->m_guiLogin->Close();
		GetGameGuiManager()->m_guiLogin	= NULL;
	}
	if(GetGameGuiManager()->m_guiSelectChar)
	{
		GetGameGuiManager()->m_guiSelectChar->Close();
		GetGameGuiManager()->m_guiSelectChar = NULL;
	}

	//UnLoad login effect
	/*if ( GetGameApplication() )
	GetGameApplication()->UnInitLoginCloud();

	#ifdef PARTICLE_SYSTEM
	if ( GetParticleSysMgr() )
	{
	//GetParticleSysMgr()->LoadOneParticleSystem( "ParticleEffectA8" );
	//GetParticleSysMgr()->GetActiveParticleSystem()->SetPositionInGameSpace( D3DXVECTOR3(500,400,0) );
	//GetParticleSysMgr()->GetActiveParticleSystem()->Play( GetParticleSysMgr()->GetActiveParticleSystem()->GetSysLife() );
	}
	GetGameApplication()->InitParticleDevice();
	#endif
	*/
	CGameAppation* pApp = GetGameApplication();
	if(pApp)
	{
		pApp->UnInitLoginCloud();
#ifdef PARTICLE_SYSTEM
		pApp->InitParticleDevice();
#endif
	}

	GetGameGuiManager()->AddChatSelect();
	GetGameGuiManager()->AddChatOutput();
	GetGameGuiManager()->AddSysChat();
	GetGameGuiManager()->AddShortcutItem();
	GetGameGuiManager()->AddFloatControl();
	GetGameGuiManager()->AddFloatAccessKey();
	GetGameGuiManager()->AddTeam();
	GetGameGuiManager()->AddSkill(false);
	GetGameGuiManager()->AddCharacterProperty(false);
	GetGameGuiManager()->m_guiTeam->SetVisible(false);
	GetGameGuiManager()->m_guiChatSelect->EndPopup();
	CRoleItem::s_bCanMoveItem = true;
	GetGameGuiManager()->m_guiItem->SetCharType(GetScene()->GetMainCharacter()->GetType());
	GetGameGuiManager()->m_guiMain->SetVisible(true);
	//GetGameBackMusic()->StopPlay(10000);
	GetQueryManager()->ClearList();

	g_dwGameStartTime = xtimeGetTime();

	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void ExitGame()
{
	FUNCTION_BEGIN;

	Engine_GetAppation()->Exit();

	FUNCTION_END;
}

bool IsConnectionLoginServer(std::vector<std::string> & aAddress,std::vector<int> & aPorts)
{
	if(GetClient()->GetState() != net::CSocket::eSocketState_Connection)
		return false;

	for(int i=0; i< aAddress.size(); ++i)
	{
		const char* pszAddress = aAddress[i].c_str();
		DWORD nPort  = (i >= aPorts.size() ? aPorts.back() : aPorts[i]);

		if( GetClient()->GetTargetIP() == net::AddressToIP(pszAddress) && GetClient()->GetTargetPort() == nPort )
		{
			return true;
		}
	}
	return false;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool ConnectionLoginServer()
{
	FUNCTION_BEGIN;

	std::vector<std::string> aAddress;
	std::vector<int> aPorts;

	_parse_str(aAddress,GetGameApplication()->GetConfig()->szLoginAddress,':');
	_parse_str_num(aPorts,GetGameApplication()->GetConfig()->szLoginPort);

	if(IsConnectionLoginServer(aAddress,aPorts))
		return true;

	for(int i=0; i< aAddress.size(); ++i)
	{
		const char* pszAddress = aAddress[i].c_str();
		DWORD nPort  = (i >= aPorts.size() ? aPorts.back() : aPorts[i]);

		GetClient()->Destroy();

		if(GetClient()->Connect(pszAddress,nPort))
		{
			GetGameState()->SetState(eGameState_Login);
			g_JpegPassport.resize(0);

			stUserVerifyVerCmd cmd;

#ifdef _AUTOPLAY
			cmd.version = GetGameVersion();
#else
			cmd.version = ZTGAME_VERSION;
#endif

			cmd.version = 1999;
			SEND_USER_CMD(cmd);	
			TRACE("-1.1-版本验证命令的长度- %d -版本号- %d - \n",sizeof(cmd),cmd.version);
			return true;
		}
	}

	return false;

	FUNCTION_END;
}

// This function fetches the MAC address of the localhost by fetching the 
// information through GetAdapatersInfo.  It does not rely on the NETBIOS
// protocol and the ethernet adapter need not be connect to a network.
//
// Supported in Windows NT/2000/XP   
// Supported in Windows 95/98/Me
//
// Supports multiple NIC cards on a PC.
bool GetMACaddress(std::string & strOutter)
{
	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS);			// Verify return value is valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
	do {
		std::string strReturn;

		char acMAC[18];	
		memset(acMAC,0,sizeof(acMAC));

		sprintf(acMAC,"%02X%02X%02X%02X%02X%02X",pAdapterInfo->Address[0]
		, pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], 
			pAdapterInfo->Address[3], 
			pAdapterInfo->Address[4], 
			pAdapterInfo->Address[5]);
		strReturn = acMAC;
		if(!strReturn.empty())
		{
			strOutter = strReturn;
			return true;
		}
		pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
	}
	while(pAdapterInfo);	
	// Terminate if last adapter
	return false;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszUserID : 描述
* \param pszPasswd : 描述
* \param NULL : 描述
* \return 返回值的描述
*/
bool LoginGame(const char* pszUserID,const char* pszPasswd, const char* pszJpegPassport = NULL )
{
	FUNCTION_BEGIN;

	if(g_bSinglePlayer)
	{
		if ( GetGameApplication() )
			GetGameApplication()->UnInitLoginCloud();

		{
			stMapScreenSizeDataUserCmd cmd;
			strcpy(cmd.pstrMapName,"宋国·凤凰城");
			strcpy(cmd.pstrFilename,GetGameApplication()->GetConfig()->szInitMapName);
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stMainUserDataUserCmd cmd;
			cmd.data.dwUserTempID = 1;


			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stAddUserMapScreenUserCmd cmd;
			strcpy(cmd.data.name,"卡尔马克思");
			cmd.data.dwUserTempID = 1;
			cmd.data.face  = 1;
			//cmd.data.type = PROFESSION_7;
			cmd.data.type = PROFESSION_1;
			cmd.data.sculpt.dwHorseID = 0;
			cmd.data.sculpt.dwHairID = 1;
			cmd.data.sculpt.dwBodyID = -1;
			cmd.data.sculpt.dwLeftHandID = -1;
			cmd.data.sculpt.dwRightHandID = -1;
			cmd.data.movespeed = 640;
			cmd.data.attackspeed = 100;
			//cmd.data.attackspeed = 100;
			//cmd.data.type = 1;
			memset(cmd.data.state,0,sizeof(cmd.data.state));
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stRTMagicPosUserCmd cmd;
			cmd.pos.dwTempID = 1;
			cmd.byTarget= MAPDATATYPE_USER;
			cmd.pos.x = GetGameApplication()->GetConfig()->ptInit.x;
			cmd.pos.y = GetGameApplication()->GetConfig()->ptInit.y;
			cmd.pos.byDir = _DIR_DOWN;

			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stEndOfInitDataDataUserCmd cmd;
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stAddMapNpcMapScreenUserCmd cmd;
			cmd.data.dwMapNpcDataPosition = 1;
			cmd.data.dwNpcDataID = 11021;//9009;//50043;//10004;//鸭子
			memset(cmd.data.byState,0,sizeof(cmd.data.byState));
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stRTMagicPosUserCmd cmd;
			cmd.pos.dwTempID = 1;
			cmd.byTarget= MAPDATATYPE_NPC;
			cmd.pos.x = GetGameApplication()->GetConfig()->ptInit.x + 2;
			cmd.pos.y = GetGameApplication()->GetConfig()->ptInit.y + 2;
			cmd.pos.byDir = _DIR_DOWN;
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stAddMapNpcMapScreenUserCmd cmd;
			cmd.data.dwMapNpcDataPosition = 2;
			cmd.data.dwNpcDataID = 100;
			memset(cmd.data.byState,0,sizeof(cmd.data.byState));
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			stRTMagicPosUserCmd cmd;
			cmd.pos.dwTempID = 2;
			cmd.byTarget= MAPDATATYPE_NPC;
			cmd.pos.x = GetGameApplication()->GetConfig()->ptInit.x + 4;
			cmd.pos.y = GetGameApplication()->GetConfig()->ptInit.y + 4;
			cmd.pos.byDir = _DIR_DOWN;
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

		{
			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 50;
				cmd.object.dwObjectID = 532;
				cmd.object.qwThisID   =50000;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 0;
				cmd.object.pos.y = 0;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}

			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 23;
				cmd.object.dwObjectID = 532;
				cmd.object.qwThisID   =50001;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 0;
				cmd.object.pos.y = 1;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}
			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 810;
				cmd.object.qwThisID   =50002;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 0;
				cmd.object.pos.y = 2;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}
			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 677;
				cmd.object.qwThisID   =50003;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 0;
				cmd.object.pos.y = 3;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}
			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 677;
				cmd.object.qwThisID   =50004;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 0;
				cmd.object.pos.y = 4;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}

			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 10000;
				cmd.object.dwObjectID = 665;
				cmd.object.qwThisID   =50005;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = -1;
				cmd.object.pos.y = -1;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}

			{

				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 664;
				cmd.object.qwThisID   = 10000;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_EQUIP;
				cmd.object.pos.dwTableID = 0;
				cmd.object.dur = 0;
				cmd.object.pos.x = EQUIPCELLTYPE_LEFT;
				cmd.object.pos.y = EQUIPCELLTYPE_PACKAGE;
				strcpy(cmd.object.strName,GetObjectBase(cmd.object.dwObjectID)->strName);
				GetClient()->PushCommand(&cmd,sizeof(cmd));

			}

			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 689;
				cmd.object.qwThisID   =50010;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 2;
				cmd.object.pos.y = 3;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}

			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 689;
				cmd.object.qwThisID   =50015;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 3;
				cmd.object.pos.y = 3;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}

			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 662;
				cmd.object.qwThisID   = 10001;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_EQUIP;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = EQUIPCELLTYPE_RIGHT;
				cmd.object.pos.y = EQUIPCELLTYPE_PACKAGE;
				strcpy(cmd.object.strName,GetObjectBase(cmd.object.dwObjectID)->strName);
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}





			{
				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 680;
				cmd.object.qwThisID   = 10003;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_PACKAGE;
				cmd.object.pos.dwTableID = 10000;
				cmd.object.pos.x = 2;
				cmd.object.pos.y = 2;
				strcpy(cmd.object.strName,GetObjectBase(cmd.object.dwObjectID)->strName);
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}


			{

				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 663;
				cmd.object.qwThisID   = 10004;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 4;
				cmd.object.pos.y = 4;
				strcpy(cmd.object.strName,GetObjectBase(cmd.object.dwObjectID)->strName);
				GetClient()->PushCommand(&cmd,sizeof(cmd));

			}

			{

				stAddObjectPropertyUserCmd cmd;
				cmd.byActionType = EQUIPACTION_INIT;
				memset(&cmd.object,0,sizeof(cmd.object));
				cmd.object.dwNum = 1;
				cmd.object.dwObjectID = 10009;
				cmd.object.qwThisID   = 10020;
				cmd.object.pos.dwLocation = OBJECTCELLTYPE_COMMON;
				cmd.object.pos.dwTableID = 0;
				cmd.object.pos.x = 5;
				cmd.object.pos.y = 4;
				strcpy(cmd.object.strName,GetObjectBase(cmd.object.dwObjectID)->strName);
				GetClient()->PushCommand(&cmd,sizeof(cmd));

			}


		}
		GetClient()->SetState(net::CSocket::eSocketState_Connection);
		GetGameState()->SetState(eGameState_Login);


		CAccoutSaveInfo::GetSingleton()->SetUserID(pszUserID);
		CAccoutSaveInfo::GetSingleton()->SetUserID(pszPasswd);

		GetGameAccelKeyManager()->Init();
		return true;
	}
	else
	{
#ifdef _LOGIN_SERVER_MANUAL
		if( ConnectionLoginServer() )
#else 
		if( 1 )		//ConnectionLoginServer()
#endif 
		{
			///*
			stUserRequestLoginCmd cmd;
			strncpy(cmd.pstrName,pszUserID,sizeof(cmd.pstrName));
			strncpy(cmd.pstrPassword,pszPasswd,sizeof(cmd.pstrPassword));

			CAccoutSaveInfo::GetSingleton()->SetUserID(pszUserID);
			CAccoutSaveInfo::GetSingleton()->SetUserPass(pszPasswd);

			cmd.game = 0;
			cmd.zone = GetGameApplication()->GetConfig()->nZone;

			//Add JPEG passport
			strncpy( cmd.jpegPassport, pszJpegPassport, sizeof( cmd.jpegPassport) );

			// get mac 
			std::string strMac;
			if( GetMACaddress(strMac))
			{
				strncpy(cmd.mac_addr,strMac.c_str(),sizeof(cmd.mac_addr));
				cmd.mac_addr[sizeof(cmd.mac_addr)-1] = 0;
			}
			else 
				cmd.mac_addr[0] = 0;

			LoginSafeSendCommand();

			SEND_USER_CMD(cmd);
			//*/
			return true;
		}
		return false;
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param bShowMessage : 描述
* \return 返回值的描述
*/
void LogoutGame(bool bShowMessage)
{
	FUNCTION_BEGIN;

	if(bShowMessage)
	{
		if(GetGameGuiManager()->m_guiExitGame && GetGameGuiManager()->m_guiExitGame->IsVisible())
			GetGameGuiManager()->m_guiExitGame->Close();
		else
			GetGameGuiManager()->AddExitGame();
		//GetGameGuiManager()->AddMessageBox("确定要登出游戏吗？",eAction_Logout,NULL,c_szMBOKCancel);
	}
	else
	{
		/*GetClient()->Destroy();*/
		//GetGameAccelKeyManager()->Save();
		//Delete our interest npc
		//if ( GetScene() )
		GetScene()->UnSelectInterestNpc();

		GetGameAccelKeyManager()->Release();
		stBackSelectUserCmd cmd;
		SEND_USER_CMD(cmd);
		if(g_bSinglePlayer)
		{
			GetClient()->SetState(net::CSocket::eSocketState_Disconnect);
		}
	}

	char tempSave[MAX_PATH];
	sprintf(tempSave,"data\\userdb%d%s",g_UserLocalInfo.GetCurZone(),".xml");
	g_UserLocalInfo.SaveUserLocalFile(tempSave);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
//#pragma optimize("agst",off)
bool ParseLoginMessage(stNullUserCmd* pCmd,size_t size)
{
	bool retval;
	FUNCTION_BEGIN;

	retval = false;

	s_bLoginEcho = true;


	static const char* c_szErrorMessage[] = {
		"服务器内部错误",
			"版本错误，请下载最新客户端",
			"服务器内部错误",
			"数据库出错",
			"帐号密码错误",
			"修改密码成功",
			"帐号正在被使用中",
			"帐号被封",
			"服务器维护中，请稍后登陆",
			"用户已满，请选择另一组服务器",
			"账号已经存在",
			"注册账号成功",

			"角色名重复",
			"用户档案不存在",
			"用户名重复",
			"服务器繁忙",
			"计费失败",
			"图形验证码输入错误",
			"帐号被锁定",
			"帐号待激活",
			"封测区仅限封测玩家登陆"
	};
	g_pszLoginErrorMessage = NULL;
	switch(pCmd->byParameterType)
	{
		//Add JPEG passport validate func
	case JPEG_PASSPORT_USERCMD_PARA:		// Get JPEG passport
		{
			stJpegPassportUserCmd* cmd = (stJpegPassportUserCmd*)pCmd;

			g_JpegPassport.resize(cmd->size);
			memcpy(&g_JpegPassport[0],cmd->data,cmd->size);
			RETURN(true);
		}
		break;
	case SERVER_RETURN_COUNTRY_INFO:
		{
			stCountryInfoUserCmd* pCmdInfo = (stCountryInfoUserCmd*)pCmd;
			CCountry::Instance()->SetCountryList(pCmdInfo);
			RETURN(true);
		}
		break;
	case SERVER_RETURN_LOGIN_FAILED:	// 登陆失败
		{
			stServerReturnLoginFailedCmd * cmd = (stServerReturnLoginFailedCmd*)pCmd;
			if(cmd->byReturnCode >= count_of(c_szErrorMessage))
				cmd->byReturnCode = 0;
			switch(cmd->byReturnCode)
			{
			case LOGON_RETURN_ACCOUNTSUCCESS:			// 注册账号成功
				GetGameGuiManager()->AddGuiLogin();
				GameMessageBox(c_szErrorMessage[cmd->byReturnCode]);
				break;	
			case LOGIN_RETURN_CHANGEPASSWORD:			// 修改密码成功
				GetGameGuiManager()->AddGuiLogin();
				GameMessageBox(c_szErrorMessage[cmd->byReturnCode]);
				break;
			case LOGIN_RETURN_ACCOUNTEXIST:				// 账号已经存在
				GameMessageBox(c_szErrorMessage[cmd->byReturnCode]);
				break;
			case LOGIN_RETURN_USERDATANOEXIST:			// 档案不存在
				CreateRole();
				break;
			case LOGIN_RETURN_CHARNAMEREPEAT:			// 角色名重复
				GameMessageBox(c_szErrorMessage[cmd->byReturnCode]);
				if (GetCreateRoleManager()->m_guiRoleSel)
				{
					GetCreateRoleManager()->m_guiRoleSel->SetEnabled(true);
				}
				if( GetCreateRoleManager()->m_guiRoleCreate)
				{
					GetCreateRoleManager()->m_guiRoleCreate->SetEnabled(true);
				}
				break;
			case  LOGIN_RETURN_LOCK:	 // 帐号被锁定
			case  LOGIN_RETURN_WAITACTIVE:   //  帐号待激活               
				GameMessageBox(c_szErrorMessage[cmd->byReturnCode]);
				break;
			case  LOGIN_RETURN_NEWUSER_OLDZONE:   //  封测区仅限封测玩家登陆
				GameMessageBox(c_szErrorMessage[cmd->byReturnCode]);
				break;
			case LOGIN_RETURN_VERSIONERROR: //版本错误
#ifdef _AUTOPLAY
				g_GameVersion = GetNewGameVersion(g_GameVersion);
#endif
				GetGameGuiManager()->AddMessageBox(c_szErrorMessage[cmd->byReturnCode],eAction_ConnectLoginFailed);
				break;
			default:
				g_pszLoginErrorMessage = c_szErrorMessage[cmd->byReturnCode];
				if (! GetGameState()->IsState(eGameState_Login) )
					ShowMessage(c_szErrorMessage[cmd->byReturnCode]);
				GetGameGuiManager()->AddGuiLogin();
				break;
			}
			RETURN(true);
		}
		break; 
	case SERVER_RETURN_LOGIN_OK:	// 登陆成功
		{
			char tempSave[MAX_PATH];
			sprintf(tempSave,"data\\userdb%d%s",g_UserLocalInfo.GetCurZone(),".xml");
			char tempLoad[MAX_PATH];
			sprintf(tempLoad,"userdb%d%s",g_UserLocalInfo.GetCurZone(),".xml");

			/*WIN32_FIND_DATA FileData; 
			HANDLE hSearch; 

			hSearch = FindFirstFile(tempSave, &FileData); 
			if (hSearch == INVALID_HANDLE_VALUE) 
			{ 
			g_UserLocalInfo.SaveUserLocalFile(tempSave); 

			}*/ 
			if ( !IsFileExist( tempSave ) )
			{
				g_UserLocalInfo.SaveUserLocalFile( tempSave );
			}

			g_UserLocalInfo.LoadUserLocalInfo(tempLoad);

			CNetAutoConnect::Instance()->LoginGameServer((stServerReturnLoginSuccessCmd*)pCmd);
			RETURN(true);
		}
		break;
	}

	RETURN(false);
_ret:
	;
	FUNCTION_END;
	return retval;

}
//#pragma optimize("",on)


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseTimerMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParameterType)
	{
	case GAMETIME_TIMER_USERCMD_PARA:				// 网关向用户发送游戏时间
		GetGameTime()->SetGameTime((stGameTimeTimerUserCmd*)pCmd);
		//GetGameTime()->InitTimestamp();
		return true;
		break;
	case REQUESTUSERGAMETIME_TIMER_USERCMD_PARA:	// 网关向用户请求时间
		GetGameTime()->Confirm();
		return true;
		break;
	case PING_TIMER_USERCMD_PARA:
		GetGameGuiManager()->m_guiMiniMap->EchoPing();
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;

	FUNCTION_END;
}

void ParsestScrollMapPropertyUserCmd(stScrollMapPropertyUserCmd* cmd)
{
	if( NULL != cmd && cmd->size > 0 )
	{
		if( GetGameGuiManager()->m_guiSelectDialog )
		{
			GetGameGuiManager()->m_guiSelectDialog->Close();
		}
		CGuiSelectDialog* pDlg = GetGameGuiManager()->AddSelectDlg();
		if( pDlg )
		{
			for( int i = 0; i< cmd->size; ++i)
			{
				pDlg->AddListItem(cmd->mapname[i].strMapName,(void *)(cmd->qwThisID));
			}			
		}	
	}
	else
	{
		GetGameGuiManager()->AddClientSystemMessage("服务器发过来的列表为空,可能是因为你当面没有地方可以跳");
	}
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParsePropertyMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;
	switch( pCmd->byParam )
	{
	case ADDUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
		GetScene()->GetMainCharacter()->AddItem((stAddObjectPropertyUserCmd*)pCmd);
		return true;
	case CLEARSKILL_USERCMD_PARA:
		GetScene()->GetMainCharacter()->RemoveAllSkill();
		return true;
	case REMOVEUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
		GetScene()->GetMainCharacter()->RemoveItem((stRemoveObjectPropertyUserCmd*)pCmd);
		return true;
	case SWAPUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
		GetScene()->GetMainCharacter()->SwapItem((stSwapObjectPropertyUserCmd*)pCmd);
		return true;
	case SPLITUSEROBJECT_PROPERTY_USERCMD_PARAMETER:
		GetScene()->GetMainCharacter()->SplitItem((stSplitObjectPropertyUserCmd*)pCmd);
		return true;
	case REFCOUNTOBJECT_PROPERTY_USERCMD_PARAMETER:
		GetScene()->GetMainCharacter()->RefItemCount((stRefCountObjectPropertyUserCmd*)pCmd);
		return true;
	case ACCELKEY_PROPERTY_USERCMD_PARA:	// 
		GetGameAccelKeyManager()->SetAttrib((stAccekKeyPropertyUserCmd*)pCmd);
		return true;
	case ADDUSERSKILL_PROPERTY_USERCMD_PARA:
		GetScene()->GetMainCharacter()->AddSkill((stAddUserSkillPropertyUserCmd*)pCmd);
		return true;
	case REMOVEUSERSKILL_PROPERTY_USERCMD_PARAMETER:
		GetScene()->GetMainCharacter()->RemoveSkill((stRemoveUserSkillPropertyUserCmd*)pCmd);
		return true;
	case SYSTEMSETTINGS_PROPERTY_USERCMD_PARA:
		GetGameApplication()->SetServerConfigData( &((stSystemSettingsUserCmd*)pCmd)->data );
		if (GetGameGuiManager()->m_guiUserInfo)
			GetGameGuiManager()->m_guiUserInfo->RefreshShowState();
		return true;
	case RESPONSE_PROPERTY_USERCMD_PARA:
		ResponseMake((stResponsePropertyUserCmd*)pCmd);
		return true;
	case FOUNDITEM_PROPERTY_USERCMD_PARA:
		if(GetGameGuiManager()->m_guiNpcMake)
		{
			GetGameGuiManager()->m_guiNpcMake->DisMake((stFoundItemPropertyUserCmd*)pCmd);
		}
		return true;
	case DURABIILTY_USERCMD_PARA:	// 刷新耐久度
		{
			stDurabilityUserCmd * _pCmd = (stDurabilityUserCmd*)pCmd;

			CMainCharacter* pMainRole = (CMainCharacter*)GetScene()->GetMainCharacter();
			CRoleItem* pItem = pMainRole->FindItemByID(_pCmd->dwThisID);
			if(pItem)
			{
				pItem->OnUpdateDur(_pCmd->dwDur,_pCmd->dwMaxDur);
			}
			if (GetGameGuiManager()->m_guiNpcTrade)
				GetGameGuiManager()->m_guiNpcTrade->RefreshRepairMoney();

		}
		return true;
	case SELECT_RETURN_STATES_PROPERTY_USERCMD_PARA:
		{
			stSelectReturnStatesPropertyUserCmd* cmd = (stSelectReturnStatesPropertyUserCmd*)pCmd;

			if ( ( GetScene()->m_pInterestNpc && (cmd->byType == MAPDATATYPE_NPC) )		//Other NPC( this time just the interest npc )
				|| ( GetScene()->m_pInterestNpc &&  (cmd->byType == MAPDATATYPE_USER) && (cmd->dwTempID == GetScene()->m_pInterestNpc->GetID() ) )  )	//Other player( this time just the interest )
			{
				CNpc* pNpc = NULL;
				if ( cmd->byType == MAPDATATYPE_USER )
					pNpc = GetScene()->FindCharacterByID( cmd->dwTempID );
				else
					pNpc = GetScene()->FindNpcByID( cmd->dwTempID );

				if ( pNpc )
				{
					return pNpc->PushCommand( pCmd, size );
				}
			}
			else if( (cmd->byType == MAPDATATYPE_USER) && (GetScene()->GetMainCharacter()->GetID() == cmd->dwTempID) )		//Main character
			{
				return GetScene()->GetMainCharacter()->PushCommand( pCmd, size );
			}
		}
		break;
	case SELECT_RETURN_MAINUSER_PROPERTY_USERCMD_PARA:
		{
			stSelectReturnMainUserPropertyUserCmd* cmd = (stSelectReturnMainUserPropertyUserCmd*)pCmd;
			CGuiUserInfo* pDlg = GetGameGuiManager()->AddUserInfoShowDlg();
			if (pDlg)
			{
				pDlg->SetCharType(cmd->mapuser_data.type,cmd);
				pDlg->SetHorseData(NULL);
				return true;
			}
		}
		break;
	case SELECT_RETURN_HORSE_PROPERTY_USERCMD_PARA:
		{
			stSelectReturnHorsePropertyUserCmd* cmd = (stSelectReturnHorsePropertyUserCmd *)pCmd;
			CGuiUserInfo* pDlg = GetGameGuiManager()->AddUserInfoShowDlg();
			if (pDlg)
			{
				pDlg->SetHorseData( &(cmd->data) );
				return true;
			}
		}
		break;
	case SCROLL_MAP_PROPERTY_USERCMD_PARA:
		{
			stScrollMapPropertyUserCmd *cmd = (stScrollMapPropertyUserCmd *)pCmd;
			ParsestScrollMapPropertyUserCmd(cmd);
		}
		break;
	case ADDUSEROBJECT_LIST_PROPERTY_USERCMD_PARAMETER:
		{
			stAddObjectListPropertyUserCmd* pCmdInfo = (stAddObjectListPropertyUserCmd*)pCmd;
			for( int i = 0;i < pCmdInfo->num; i++ )
			{				
				GetScene()->GetMainCharacter()->AddItem(pCmdInfo->list[i].object,pCmdInfo->list[i].byActionType);
			}
		}
		break;
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseDataMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;
	switch(pCmd->byParameterType)
	{

#ifdef _WYC_OPT_SER_
	case MERGE_VERSION_CHECK_USERCMD_PARA:
		{
			stMergeVersionCheckUserCmd* pCmdInfo = (stMergeVersionCheckUserCmd*)pCmd;
			if (pCmdInfo->dwMergeVersion != g_UserLocalInfo.GetCurVersion() ) 
			{
				g_UserLocalInfo.SetCurVersion(pCmdInfo->dwMergeVersion);
				g_UserLocalInfo.Clear();
				TRACE("##wyc--收到合并版本消息--版本不一样--wyc##\n");
			}
			else
			{
				TRACE("##wyc--收到合并版本消息--版本一样--wyc##\n");
			}			
		}
		return true;
#endif
	case MAIN_USER_DATA_USERCMD_PARA:		// 主人物信息
		//GetScene()->m_pMainCharacter->m_dwID = ((stMainUserDataUserCmd*)pCmd)->data.dwUserTempID;
		return GetScene()->AddMainCharacter(&((stMainUserDataUserCmd*)pCmd)->data);
	case SETHPANDMP_DATA_USERCMD_PARA:		// 设置用户生命和魔法
		//TRACE( "SetMainUserHP = %u", ((stSetHPAndMPDataUserCmd*)pCmd)->wdHP ); 
		return GetScene()->m_pMainCharacter->PushCommand(pCmd,size);
	case MAPSCREENSIZE_DATA_USERCMD_PARA:	// 初始化地图
		{
			if (GetGameGuiManager())
			{
				GetGameGuiManager()->EndNpcDialog();
				GetGameGuiManager()->CloseAllModalDialog();
				if (GetGameGuiManager()->m_guiOthersShop) 
				{
					GetGameGuiManager()->m_guiOthersShop->Close();
				}
				if (GetGameGuiManager()->m_guitaskdialog) 
				{
					GetGameGuiManager()->m_guitaskdialog->Close();
				}
			}
			DWORD t1 = xtimeGetTime();
			GetScene()->InitMap((stMapScreenSizeDataUserCmd*)pCmd);
#if defined _DEBUG || defined RELEASE_TEST
			Engine_WriteLogF("初始化地图时间 %d\n",xtimeGetTime()-t1);
#endif
			return true;
		}
		return true;
	case ENDOFINITDATA_DATA_USERCMD_PARA:	// 接收数据完成，可以启动游戏了.
		{
			//Request self level degree
			/*stLevelDegreeDataUserCmd cmd;
			SEND_USER_CMD(cmd);*/

			// [solved the team's bug]
			// when the server changes,this msg will be received,
			// and the team' info must be clear.
			if(GetGameGuiManager()->m_guiTeam)
				GetGameGuiManager()->m_guiTeam->BreakTeam();
			StartGame();
		}		
		return true;
	case LEVELDEGREE_DATA_USERCMD_PARA: //刷新自己的排名
		{
			if (GetGameGuiManager()->m_guiUserInfo)
				GetGameGuiManager()->m_guiUserInfo->SetLevelNum(((stLevelDegreeDataUserCmd*)pCmd)->degree);
		}
		return true;
		//#ifdef __NEW_CHECK__
		//	case STAMP_DATA_USERCMD_PARA: // 接收特征码
		//		{
		//			GetServerKillProcess()->ReceiveFromServer((const stStampDataUserCmd*)pCmd);
		//
		//		}
		//		return true;
		//#else
		//	case STAMP_CHECK_USERCMD_PARA:  
		//		{
		//
		//			GetServerKillProcess()->InstructorProcess((const stStampCheckUserCmd*)pCmd);
		//		}
		//		return true;
		//	case STAMP_DATA_USERCMD_PARA: // 接收特征码
		//		{
		//#if defined _DEBUG || defined RELEASE_TEST
		//			Engine_WriteLogF("收到特征码\n");
		//#endif
		//			GetServerKillProcess()->ReceiveFromServer((const stStampDataUserCmd*)pCmd);
		//
		//		}
		//		return true;
		//#endif
	}
	return false;

	FUNCTION_END;
}

bool ParseMapDataMessage(stMapDataMapScreenUserCmd* cmd)
{
	MapData_ItemHeader* phdr = (MapData_ItemHeader*)(&cmd->mdih);
	while(phdr->size)
	{
		switch(phdr->type)
		{
		case MAPDATATYPE_NPC:
			{
				t_MapNpcDataPos* pData = (t_MapNpcDataPos*)(phdr+1);
				t_MapNpcDataPos* pEnd = pData + phdr->size;
				for(;pData < pEnd;++pData)
				{
					CNpc* pNpc = GetScene()->AddNpc( pData );

					if( pNpc != NULL )
					{
						stPointI ptGrid (pData->x,pData->y);
						if(pNpc->GetGridPos() != ptGrid)
						{
							stRTMagicPosUserCmd  cmd;
							cmd.pos.x = pData->x;
							cmd.pos.y = pData->y;
							cmd.pos.byDir = pData->byDir;
							cmd.byTarget = MAPDATATYPE_NPC;
							pNpc->PushCommand(&cmd,sizeof(cmd));
						}
					}
				}
				phdr = (MapData_ItemHeader*)pData;
			}
			break;
		case MAPDATATYPE_USER:
			{
				t_MapUserDataPos* pData = (t_MapUserDataPos*)(phdr+1);
				t_MapUserDataPos* pEnd = pData + phdr->size;
				for(;pData < pEnd;++pData)
				{
					CNpc* pNpc = GetScene()->AddCharacter( pData );

					if( pNpc != NULL )
					{
						stPointI ptGrid (pData->x,pData->y);
						if(pNpc->GetGridPos() != ptGrid)
						{
							stRTMagicPosUserCmd  cmd;
							cmd.pos.x = pData->x;
							cmd.pos.y = pData->y;
							cmd.pos.byDir = pData->byDir;
							cmd.byTarget = MAPDATATYPE_USER;
							pNpc->PushCommand(&cmd,sizeof(cmd));
						}
					}
				}
				phdr = (MapData_ItemHeader*)pData;
			}
			break;
		case MAPDATATYPE_BUILDING:
			{
				t_MapBuildingDataPos* pData = (t_MapBuildingDataPos*)(phdr+1);
				t_MapBuildingDataPos* pEnd = pData + phdr->size;
				for(;pData < pEnd;++pData)
				{
					GetScene()->AddBuild( pData );
				}
				phdr = (MapData_ItemHeader*)pData;
			}
			break;
		case MAPDATATYPE_ITEM:
			{
				t_MapObjectData* pData = (t_MapObjectData*)(phdr+1);
				t_MapObjectData* pEnd = pData + phdr->size;
				for(;pData < pEnd;++pData)
				{
					GetScene()->AddItem( pData ,false);
				}
				phdr = (MapData_ItemHeader*)pData;
			}
			break;
		case MAPDATATYPE_PET:
			{
				Debug_Int(MAPDATATYPE_PET);
				t_MapPetData* pData = (t_MapPetData*)(phdr+1);
				t_MapPetData* pEnd = pData + phdr->size;
				for(;pData < pEnd;++pData)
				{
					CNpc* pNpc = GetScene()->FindNpcByID(pData->tempID);
					if( pNpc != NULL )
					{
						pNpc->AddPetData(pData);
					}
				}
				phdr = (MapData_ItemHeader*)pData;
			}
			break;
		default :
			return false;
		}
	}
	return true;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
//#pragma optimize("agst",off)
inline bool ParseMapScreenMessage(stNullUserCmd* pCmd,size_t size)
{
	bool bResult;

	FUNCTION_BEGIN;

	bResult = false;
	switch(pCmd->byParameterType)
	{
	case GOODNESS_MAPSCREEN_USERCMD_PARA:			// 设置角色善恶值
		//if(sizeof(stGoodnessStateMapScreenUserCmd) == size)
		{
			stGoodnessStateMapScreenUserCmd* ptr = (stGoodnessStateMapScreenUserCmd*)pCmd;
			if (GetScene())
			{
				if (GetScene()->FindCharacterByID(ptr->dwTempID)) 
				{
					GetScene()->FindCharacterByID(ptr->dwTempID)->SetGoodness(ptr->dwGoodness);
					bResult = true;
				}	
			}							
		}
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	case ADDUSER_MAPSCREEN_STATE_USERCMD_PARA:
		{
			stAddUserMapScreenStateUserCmd * __pcmd = (stAddUserMapScreenStateUserCmd *)pCmd;

			stAddUserMapScreenUserCmd oldcmd;
			memset(&oldcmd.data,0,sizeof(oldcmd.data));

			memcpy(&oldcmd.data,&__pcmd->data,sizeof(t_UserData));

			for(size_t i = 0 ; i < __pcmd->data.num; ++i)
			{
				WORD wState = __pcmd->data.state[i];
				set_state(oldcmd.data.state,wState);
			}  
			bResult = ParseMapScreenMessage(&oldcmd,sizeof(oldcmd));
		}
		break;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	case ADDUSER_MAPSCREEN_USERCMD_PARA:			// 添加用户
		if(sizeof(stAddUserMapScreenUserCmd) == size)
		{
			GetScene()->AddCharacter( &((stAddUserMapScreenUserCmd*)pCmd)->data );
			bResult = true;
		}
		break;
	case REMOVEUSER_MAPSCREEN_USERCMD_PARA:		// 删除用户
		{
			GetScene()->RemoveCharacter( (stRemoveUserMapScreenUserCmd*)pCmd );
			bResult = true;
		}
		break;
	case ADDMAPOBJECT_MAPSCREEN_USERCMD_PARA:		// 添加物品
		bResult =  GetScene()->AddItem( &((stAddMapObjectMapScreenUserCmd*)pCmd)->data, ((stAddMapObjectMapScreenUserCmd*)pCmd)->action == OBJECTACTION_DROP);
		break;
	case REMOVEMAPOBJECT_MAPSCREEN_USERCMD_PARA:	// 删除物品
		bResult = GetScene()->RemoveItem( (stRemoveMapObjectMapScreenUserCmd*)pCmd );
		break;
	case ADDMAPBUILDING_MAPSCREEN_USERCMD_PARA:	// 添加建筑
		bResult = GetScene()->AddBuild( &((stAddMapBuildingMapScreenUserCmd*)pCmd)->data );
		break;
	case REMOVEMAPBUILDING_MAPSCREEN_USERCMD_PARA:	// 删除建筑
		bResult = GetScene()->RemoveBuild( (stRemoveMapBuildingMapScreenUserCmd*) pCmd );
		break;


		////////////////////////////////////////////////////////////////////////////////////////
	case ADDMAPNPC_MAPSCREEN_STATE_USERCMD_PARA:
		{
			stAddMapNpcMapScreenStateUserCmd * __pcmd = (stAddMapNpcMapScreenStateUserCmd *)pCmd;

			stAddMapNpcMapScreenUserCmd oldcmd;
			memset(&oldcmd.data,0,sizeof(oldcmd.data));

			memcpy(&oldcmd.data,&__pcmd->data,sizeof(t_NpcData));

			for(size_t i = 0 ; i < __pcmd->data.num; ++i)
			{
				WORD wState = __pcmd->data.state[i];
				set_state(oldcmd.data.byState,wState);
			}  
			bResult = ParseMapScreenMessage(&oldcmd,sizeof(oldcmd));
		}
		break;
		////////////////////////////////////////////////////////////////////////////////////////
	case ADDMAPNPC_MAPSCREEN_USERCMD_PARA:			// 添加NPC
		{
			GetScene()->AddNpc( &((stAddMapNpcMapScreenUserCmd*)pCmd)->data );
			bResult = true;
		}
		break;
	case REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA:		// 删除NPC
		bResult = GetScene()->RemoveNpc( (stRemoveMapNpcMapScreenUserCmd*) pCmd );
		break;
	case BATCHREMOVENPC_MAPSCREEN_USERCMD_PARA:		// 批量删除NPC
		{
			stBatchRemoveNpcMapScreenUserCmd* ptr = (stBatchRemoveNpcMapScreenUserCmd*)pCmd;
			for(size_t i=0; i<ptr->num;++i){
				stRemoveMapNpcMapScreenUserCmd _cmd;
				_cmd.dwMapNpcDataPosition = ptr->id[i];
				GetScene()->RemoveNpc( &_cmd );
			}
		}
		break;
	case BATCHREMOVEUSER_MAPSCREEN_USERCMD_PARA:	// 批量删除USER
		{
			stBatchRemoveUserMapScreenUserCmd* ptr = (stBatchRemoveUserMapScreenUserCmd*)pCmd;
			for(size_t i=0; i<ptr->num;++i){
				stRemoveUserMapScreenUserCmd _cmd;
				_cmd.dwUserTempID = ptr->id[i];
				GetScene()->RemoveCharacter( &_cmd );
			}
		}
		break;
	case ALL_MAPSCREEN_USERCMD_PARA:				// 对9屏用户和NPC	
		GetScene()->OnAllCharPostion( ( stAllMapScreenUserCmd* )pCmd );
		bResult = true;
		break;
	case REFRESHSTATE_MAPSCREEN_USERCMD_PARA:		// 更新人物状态
		{
			stRefreshStateMapScreenUserCmd* cmd = (stRefreshStateMapScreenUserCmd*)pCmd;
			CNpc* pNpc = GetScene()->FindCharacterByID(cmd->dwUserTempID);
			if(pNpc)
			{
				bResult = pNpc->PushCommand(pCmd,size);
			}
		}
		break;
	case NPCHP_MAPSCREEN_USERCMD_PARA:// 设置NPC的HP
		{
			stNPCHPMapScreenUserCmd* cmd = (stNPCHPMapScreenUserCmd*)pCmd;
			CNpc* pNpc = GetScene()->FindNpcByID(cmd->dwMapNpcDataPosition);
			if(pNpc)
			{
				bResult = pNpc->PushCommand(pCmd,size);
			}
		}
		break;
	case SELECTED_HPMP_PROPERTY_USERCMD_PARA: // 通知选择自己的玩家hp和mp的变化
		{
			stRTSelectedHpMpPropertyUserCmd* p = (stRTSelectedHpMpPropertyUserCmd*)pCmd;
			if(p->byType == MAPDATATYPE_NPC)
			{
				CNpc* pNpc = GetScene()->FindNpcByID(p->dwTempID);
				if(pNpc)
				{
					bResult = pNpc->PushCommand(pCmd,size);
				}
			}
			else if(p->byType == MAPDATATYPE_USER)
			{
				CCharacter* pChar = GetScene()->FindCharacterByID(p->dwTempID);
				if(pChar)
				{
					bResult = pChar->PushCommand(pCmd,size);
				}
			}
		}
		break;
	case MAPDATA_MAPSCREEN_USERCMD_PARA:			// 地图数据打包发送
		{
			bResult = ParseMapDataMessage((stMapDataMapScreenUserCmd*)pCmd);
		}
		break;


		//////////////fffffffffffffffffffffffffffffffffffffffffffffffffffff
	case ADDMAPNPC_AND_POS_MAPSCREEN_STATE_USERCMD_PARA:
		{
			stAddMapNpcAndPosMapScreenStateUserCmd * __pcmd = (stAddMapNpcAndPosMapScreenStateUserCmd *)pCmd;

			stAddMapNpcAndPosMapScreenUserCmd oldcmd;
			memset(&oldcmd.data,0,sizeof(oldcmd.data));

			memcpy(&oldcmd.data,&__pcmd->data,sizeof(t_NpcData));

			oldcmd.data.x = __pcmd->data.x;
			oldcmd.data.y = __pcmd->data.y;
			oldcmd.data.byDir = __pcmd->data.byDir;

			for(size_t i = 0 ; i < __pcmd->data.num; ++i)
			{
				WORD wState = __pcmd->data.state[i];
				set_state(oldcmd.data.byState,wState);
			}  
			bResult = ParseMapScreenMessage(&oldcmd,sizeof(oldcmd));

		}
		break;
		////////////////fffffffffffffffffffffffffffffffffffffffffffffffffffff

	case ADDMAPNPC_AND_POS_MAPSCREEN_USERCMD_PARA:			//Add npc protocol after adjust
		{
			stAddMapNpcAndPosMapScreenUserCmd* cmd = (stAddMapNpcAndPosMapScreenUserCmd*)pCmd;
			t_MapNpcDataPos* pData = &(cmd->data);

			CNpc* pNpc = GetScene()->AddNpc( pData );
			if( pNpc != NULL )
			{
				stPointI ptGrid (pData->x,pData->y);
				if(pNpc->GetGridPos() != ptGrid)
				{
					stRTMagicPosUserCmd  cmd;
					cmd.pos.x = pData->x;
					cmd.pos.y = pData->y;
					cmd.pos.byDir = pData->byDir;
					cmd.byTarget = MAPDATATYPE_NPC;
					pNpc->PushCommand(&cmd,sizeof(cmd));
				}
				bResult = true;
			}
		}
		break;
		//////////////fffffffffffffffffffffffffffffffffffffffffffffffffffff
	case ADDUSER_AND_POS_MAPSCREEN_STATE_USERCMD_PARA:
		{
			stAddUserAndPosMapScreenStateUserCmd * __pcmd = (stAddUserAndPosMapScreenStateUserCmd *)pCmd;

			stAddUserAndPosMapScreenUserCmd oldcmd;
			memset(&oldcmd.data,0,sizeof(oldcmd.data));

			memcpy(&oldcmd.data,&__pcmd->data,sizeof(t_UserData));

			oldcmd.data.x = __pcmd->data.x;
			oldcmd.data.y = __pcmd->data.y;
			oldcmd.data.byDir = __pcmd->data.byDir;

			for(size_t i = 0 ; i < __pcmd->data.num; ++i)
			{
				WORD wState = __pcmd->data.state[i];
				set_state(oldcmd.data.state,wState);
			}  

			bResult = ParseMapScreenMessage(&oldcmd,sizeof(oldcmd));

		}
		break;
		////////////////fffffffffffffffffffffffffffffffffffffffffffffffffffff
	case ADDUSER_AND_POS_MAPSCREEN_USERCMD_PARA:		//Add user protocol after adjust
		{
			stAddUserAndPosMapScreenUserCmd* cmd = (stAddUserAndPosMapScreenUserCmd*)pCmd;
			t_MapUserDataPos* pData = &(cmd->data);

			CNpc* pNpc = GetScene()->AddCharacter( pData );

			if( pNpc != NULL )
			{
				stPointI ptGrid (pData->x,pData->y);
				if(pNpc->GetGridPos() != ptGrid)
				{
					stRTMagicPosUserCmd  cmd;
					cmd.pos.x = pData->x;
					cmd.pos.y = pData->y;
					cmd.pos.byDir = pData->byDir;
					cmd.byTarget = MAPDATATYPE_USER;
					pNpc->PushCommand(&cmd,sizeof(cmd));
				}
			}
			bResult = true;
		}
		break;
	case CHANGEFACE_MAPSCREEN_USERCMD_PARA:			//人物使用易容术
		{
			stChangeFaceMapScreenUserCmd* cmd = (stChangeFaceMapScreenUserCmd*)pCmd;
			CNpc* pNpc = GetScene()->FindCharacterByID(cmd->dwUserTempID);
			if ( pNpc )
			{
				bResult = pNpc->ChangeFace( cmd->dwChangeFaceID );
			}
		}
		break;
	case CLEAR_OBJECTOWNER_MAPSCREEN_USERCMD_PARA: //清除地图上物品保护
		{
			stClearObjectOwnerMapScreenUserCmd * cmd = (stClearObjectOwnerMapScreenUserCmd*)pCmd;
			CItem* pItem = GetScene()->FindItemByID(cmd->dwMapObjectTempID);
			if( pItem)
			{
				pItem->LoseOwner();
			}
		}
		break;
	case SETSTATE_MAPSCREEN_USERCMD_PARA:
		{
			stSetStateMapScreenUserCmd* cmd = (stSetStateMapScreenUserCmd*)pCmd;
			if ( cmd->dwUserTempID != -1 )
			{
				CNpc* pNpc = NULL;
				if ( cmd->type == MAPDATATYPE_USER )
					pNpc = GetScene()->FindCharacterByID( cmd->dwUserTempID );
				else
					pNpc = GetScene()->FindNpcByID( cmd->dwUserTempID );

				if ( pNpc )
				{
					pNpc->PushCommand(cmd,sizeof(*cmd));
					/*set_state( pNpc->GetState(), cmd->wdState );
					pNpc->ResetState();*/					
				}
			}
			bResult = true;
		}
		break;
	case CLEARSTATE_MAPSCREEN_USERCMD_PARA:
		{
			stClearStateMapScreenUserCmd* cmd = (stClearStateMapScreenUserCmd*)pCmd;
			if ( cmd->dwUserTempID != -1 )
			{
				CNpc* pNpc = NULL;
				if ( cmd->type == MAPDATATYPE_USER )
					pNpc = GetScene()->FindCharacterByID( cmd->dwUserTempID );
				else
					pNpc = GetScene()->FindNpcByID( cmd->dwUserTempID );

				if ( pNpc )
				{
					clear_state( pNpc->GetState(), cmd->wdState );
					pNpc->ResetState();
				}
			}
			bResult = true;
		}
		break;

		//////////////////////////////////////////////////////////////////////////////////////
	case ADDMAPNPCPET_MAPSCREEN_STATE_USERCMD_PARA:
		{
			stAddMapNpcPetMapScreenStateUserCmd * __pcmd= (stAddMapNpcPetMapScreenStateUserCmd *)pCmd;
			stAddMapNpcPetMapScreenUserCmd oldcmd;
			memset(&oldcmd.data,0,sizeof(oldcmd.data));
			memset(&oldcmd.pet,0,sizeof(oldcmd.pet));

			memcpy(&oldcmd.pet,&__pcmd->pet,sizeof(t_MapPetData));
			memcpy(&oldcmd.data,&__pcmd->data,sizeof(t_MapNpcData));

			oldcmd.data.x = __pcmd->data.x;
			oldcmd.data.y = __pcmd->data.y;
			oldcmd.data.byDir = __pcmd->data.byDir;



			for(size_t i = 0 ; i < __pcmd->data.num; ++i)
			{
				WORD wState = __pcmd->data.state[i];
				set_state(oldcmd.data.byState,wState);
			}  
			bResult = ParseMapScreenMessage(&oldcmd,sizeof(oldcmd));


		}
		break;
		//////////////////////////////////////////////////////////////////////////////////////

	case ADDMAPNPCPET_MAPSCREEN_USERCMD_PARA:
		{
			stAddMapNpcPetMapScreenUserCmd* pCmdInfo = (stAddMapNpcPetMapScreenUserCmd*)pCmd;
			//#ifdef  _DEBUG
			//			AddClientSystemMessageF("stAddMapNpcPetMapScreenUserCmd :%s",pCmdInfo->pet.name);
			//#endif
			CNpc* pNpc = GetScene()->AddNpc( &(pCmdInfo->data) );
			if( pNpc != NULL )
			{
				pNpc->AddPetData( &(pCmdInfo->pet) );
				stPointI ptGrid (pCmdInfo->data.x,pCmdInfo->data.y);
				if(pNpc->GetGridPos() != ptGrid)
				{
					stRTMagicPosUserCmd  cmd;
					cmd.pos.x = pCmdInfo->data.x;
					cmd.pos.y = pCmdInfo->data.y;
					cmd.pos.byDir = pCmdInfo->data.byDir;
					cmd.byTarget = MAPDATATYPE_NPC;
					pNpc->PushCommand(&cmd,sizeof(cmd));
				}
			}
			return true;
		}
		break;
	case ADDMAPPET_MAPSCREEN_USERCMD_PARA:
		{
			stAddMapPetMapScreenUserCmd* pCmdInfo = (stAddMapPetMapScreenUserCmd*)pCmd;
			//#ifdef  _DEBUG
			//			AddClientSystemMessageF("stAddMapPetMapScreenUserCmd :%s",pCmdInfo->pet.name);
			//#endif
			CNpc* pNpc = GetScene()->FindNpcByID(pCmdInfo->pet.tempID);
			if( pNpc != NULL )
			{
				pNpc->AddPetData( &(pCmdInfo->pet) );
			}
			return true;
		}
		break;
	}
	FUNCTION_END;
	return bResult;
}
//#pragma optimize("",on)

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/

inline bool ConvertOtherMagicAttackCmd(stRTOtherMagicUserCmd* p)
{
	DWORD dwID = -1;
	bool retval = false;

	stRTMagicUserCmd cmd;

	cmd.dwUserTempID = p->dwUserTempID;
	cmd.byTarget = p->byTarget;
	cmd.bySrc = p->bySrc;
	cmd.dwSrcTempID = p->dwSrcTempID;

	cmd.byRetcode = RTMAGIC_SUCCESS;
	cmd.byDirect = 0;
	cmd.dwHP = 0;
	cmd.wdHP = 0;
	cmd.byLuck = 0;

	//Send cmd
	dwID = p->dwUserTempID;
	bool bNpc = (p->byTarget == MAPDATATYPE_NPC);
	if(dwID != -1)
	{
		CNpc* pNpc = NULL;
		if(bNpc)
			pNpc = GetScene()->FindNpcByID(dwID);
		else
			pNpc = GetScene()->FindCharacterByID(dwID);

		if ( NULL == pNpc )
			return false;
		if ( pNpc->IsMainRole() )
			return false;
		if ( pNpc->IsMyPet() )
			return false;
		if ( GetScene()->m_pInterestNpc && (GetScene()->m_pInterestNpc == pNpc) )
			return false;

		CNpc* pAttacker = NULL;
		if ( p->dwSrcTempID != -1 )
		{
			if ( p->bySrc == MAPDATATYPE_NPC )
				pAttacker = GetScene()->FindNpcByID( p->dwSrcTempID );
			else
				pAttacker = GetScene()->FindCharacterByID( p->dwSrcTempID );
		}

		if ( NULL == pAttacker )
			return false;
		if ( pAttacker->IsMainRole() )
			return false;
		if ( pAttacker->IsMyPet() )
			return false;
		if ( GetScene()->m_pInterestNpc && (GetScene()->m_pInterestNpc == pAttacker) )
			return false;


		retval = pNpc->PushCommand( &cmd, sizeof(stRTMagicUserCmd) );
	}

	return retval;
}
//#pragma optimize("agst",off)
inline bool ParseMagicMessage(stNullUserCmd* pCmd,size_t size)
{
	bool retval;
	FUNCTION_BEGIN;

	retval = false;
	bool bNpc = false;
	bool bNeedRequestOper = false;
	DWORD dwID = -1;
	switch(pCmd->byParameterType)
	{
	case MAGIC_USERCMD_PARA:		// 攻击指令
		{
			stAttackMagicUserCmd* p = (stAttackMagicUserCmd*) pCmd;
			dwID = p->dwUserTempID;
			bNpc = (p->byAttackType == ATTACKTYPE_N2U || p->byAttackType == ATTACKTYPE_N2N);
			bNeedRequestOper = true;
		}
		break;
	case RTMAGIC_USERCMD_PARA:			// 攻击返回
		{
			stRTMagicUserCmd* p = (stRTMagicUserCmd*) pCmd;
			dwID = p->dwUserTempID;
			bNpc = (p->byTarget == MAPDATATYPE_NPC);
		}
		break;
	case RT_OTHER_MAGIC_USERCMD_PARA:	//非主角的攻击返回
		{
			return ConvertOtherMagicAttackCmd( (stRTOtherMagicUserCmd*)pCmd );			
		}
		break;
	case OBJECT_HPMP_POP_PARA:			//Get object statu pop value
		{
			stObjectHpMpPopUserCmd* p = (stObjectHpMpPopUserCmd*) pCmd;
			dwID = p->dwUserTempID;
			bNpc = (p->byTarget == MAPDATATYPE_NPC);
		}
		break;
	case RTMAGIC_POS_USERCMD_PARA:			// 返回目标坐标
		{
			stRTMagicPosUserCmd* p = (stRTMagicPosUserCmd*)pCmd;
			dwID = p->pos.dwTempID;
			bNpc = (p->byTarget == MAPDATATYPE_NPC);

			bNeedRequestOper = true;
		}
		break;
	case NPCDEATH_USERCMD_PARA:			// NPC死亡
		{
			stNpcDeathUserCmd* p = (stNpcDeathUserCmd*)pCmd;
			dwID = p->dwNpcTempID;
			bNpc = true;

			/*{
				CNpc* pNpc = GetScene()->FindNpcByID(dwID);
				if( pNpc )
				{
					pNpc->SetDie( true );
				}
				return true;
			}*/
		}
		break;
	case BACKOFF_USERCMD_PARA:	// 防御方后退
		{
			stBackOffMagicUserCmd* p = (stBackOffMagicUserCmd*)pCmd;
			dwID = p->dwTempID;
			bNpc = (p->byType == MAPDATATYPE_NPC);
		}
		break;
	case LEVELUP_USERCMD_PARA:	// 人物等级提升
		{
			stLevelUpUserCmd* p = (stLevelUpUserCmd*)pCmd;
			dwID = p->dwUserTempID;
			bNpc = false;
		}
		break;
	case OBTAINEXP_USERCMD_PARA:	// 攻击Npc得到经验
		{
			GetScene()->GetMainCharacter()->PushCommand(pCmd,size);
		}
		break;
	case PKMODE_USERCMD_PARA:
		{
			if ( ( ((stPKModeUserCmd*)pCmd)->byPKMode < PKMODE_NUM) && ( ((stPKModeUserCmd*)pCmd)->byPKMode >= 0 ) )
			{
				GetScene()->GetMainCharacter()->SetPKMode( ((stPKModeUserCmd*)pCmd)->byPKMode );
				if ( GetGameGuiManager()->m_guiMain )
					GetGameGuiManager()->m_guiMain->ChangeAttackModeBtnTips();
			}			
		}
		break;
	case DURABIILTY_USERCMD_PARA:	// 刷新耐久度
		{
			dwID = GetScene()->GetMainCharacter()->m_dwID;
			bNpc = false;
		}
		break;
	case OBJECT_EXP_USERCMD_PARA:	// 刷新道具经验值
		{
			stObjectExpUserCmd* cmd = (stObjectExpUserCmd *)pCmd;
			if ( GetScene()->GetMainCharacter() )
			{
				CRoleItem* pItem = GetScene()->GetMainCharacter()->FindItemByID( cmd->id );
				if ( pItem )
					pItem->UpdateExp( cmd->exp );
			}			
		}
		break;
	case FIREWORK_USERCMD_PARA:
		{
			stFireWorkUserCmd* cmd = (stFireWorkUserCmd*)pCmd;
			dwID = cmd->dwUserID;
			bNpc = (cmd->byType == MAPDATATYPE_NPC);
		}
		break;
	case TIMESYNC_USERCMD_PARA:
		{
			stTimeSyncUserCmd* cmd = (stTimeSyncUserCmd*)pCmd;
			if ( GetScene() )
			{
				GetScene()->StartSpringComingEffect( cmd->serverTime );
				GetScene()->m_ServerTime = cmd->serverTime;
			}
		}
		retval = true;
		break;
	}
	if(dwID != -1)
	{
		CNpc* pNpc = NULL;
		if(bNpc)
			pNpc = GetScene()->FindNpcByID(dwID);
		else
			pNpc = GetScene()->FindCharacterByID(dwID);

		if(NULL != pNpc) 
		{
			retval = pNpc->PushCommand(pCmd,size);
		}
		else
		{
			TRACE("未找到 %s , ID: %u\n",(bNpc ? "NPC" : "Character"),dwID);
		}

	}

	FUNCTION_END;
	return retval;
}
//#pragma optimize("",on)

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
/*
inline bool ParseMoveMessage(stNullUserCmd* pCmd,size_t size)
{
bool retval;

FUNCTION_BEGIN;

retval = false;
switch(pCmd->byParameterType) 
{
case NPCMOVE_MOVE_USERCMD_PARA:
{
//CNpc* pNpc = GetScene()->FindAndRequestNpc(((stNpcMoveMoveUserCmd*)pCmd)->dwNpcTempID);
CNpc* pNpc = GetScene()->FindNpcByID(((stNpcMoveMoveUserCmd*)pCmd)->dwNpcTempID);
if(pNpc) 
retval = pNpc->PushCommand(pCmd,size);
else
Engine_WriteLogF("[NPCMOVE_MOVE_USERCMD_PARA] 未找到NPC(%d)\n", ((stNpcMoveMoveUserCmd*)pCmd)->dwNpcTempID);
}
break;
case USERMOVE_MOVE_USERCMD_PARA:
case USERINSTANTJUMP_MOVE_USERCMD_PARA:
{
//CCharacter* pCharacter = GetScene()->FindAndRequestCharacter(((stUserMoveMoveUserCmd*)pCmd)->dwUserTempID);
CCharacter* pCharacter = GetScene()->FindCharacterByID(((stUserMoveMoveUserCmd*)pCmd)->dwUserTempID);
if(pCharacter) 
retval = pCharacter->PushCommand(pCmd,size);
else
Engine_WriteLogF("[%s] 未找到USER(%d)\n", 
pCmd->byParam == USERINSTANTJUMP_MOVE_USERCMD_PARA ? "USERINSTANTJUMP_MOVE_USERCMD_PARA" : "USERINSTANTJUMP_MOVE_USERCMD_PARA",
((stUserMoveMoveUserCmd*)pCmd)->dwUserTempID);
}
break;
case RESPONSELANDMASS_MOVE_USERCMD_PARA:
break;
case LANDMASSJUMP_MOVE_USERCMD_PARA:
break;
}

FUNCTION_END;
return retval;
}
//*/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseSelectMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case USERINFO_SELECT_USERCMD_PARA:	// 服务器返回角色列表
		{
			TRACE("收到服务器返回的角色列表！\n");

			switch(GetGameState()->GetState())
			{
			case eGameState_Null:
				break;
			case eGameState_Login:
				break;
			case eGameState_Run:
				{  // 游戏正常退出，返回到角色选择界面
					GetScene()->Destroy();
					GetGameGuiManager()->Release();
					InitGame(false);
					GetGameState()->SetState(eGameState_Login); 
				}
				break;
			}
			stUserInfoUserCmd * cmd = (stUserInfoUserCmd*)pCmd;
			CreateRole(cmd->charInfo);
#ifdef ___AUTO___NET___CONNECT___
			CNetAutoConnect::Instance()->EndAutoConnect();
#endif 
			//	if(GetGameGuiManager()->m_guiLogin)GetGameGuiManager()->m_guiLogin->SetVisible(false);

			return true;
		}
		break;
	case DELETE_ERROR_SELECT_USERCMD_PARA: 
		{
			GameMessageBox("数字密码错误！");
			if(GetCreateRoleManager()->m_guiRoleSel)
				GetCreateRoleManager()->m_guiRoleSel->SetEnabled(true);
			return true;
		}
		break;
	case CHECKNAME_SELECT_USERCMD_PARA:
		{
			switch(((stCheckNameSelectUserCmd*)pCmd)->err_code) 
			{
			case 0:	
				{
					GetCreateRoleManager()->CreateUserSuccess();	
				}
				break;
			case 1:
				GameMessageBox("角色名重复！");
				if( GetCreateRoleManager()->m_guiRoleCreate)
					GetCreateRoleManager()->m_guiRoleCreate->SetEnabled(true);
				if( GetCreateRoleManager()->m_guiRoleSel)
					GetCreateRoleManager()->m_guiRoleSel->SetEnabled(true);
				GetCreateRoleManager()->m_bInCreating = true;
				GetCreateRoleManager()->m_guiRoleCreate->ReLoad();
				break;
			case 2:
				GameMessageBox("角色名不合法！");
				if( GetCreateRoleManager()->m_guiRoleCreate)
					GetCreateRoleManager()->m_guiRoleCreate->SetEnabled(true);
				if( GetCreateRoleManager()->m_guiRoleSel)
					GetCreateRoleManager()->m_guiRoleSel->SetEnabled(true);
				GetCreateRoleManager()->m_bInCreating = true;
				GetCreateRoleManager()->m_guiRoleCreate->ReLoad();
				break;
			}
			return true;
		}
		break;
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseReliveMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case MAINUSERDEATH_RELIVE_USERCMD_PARA:		// 主用户死亡指令
		{
			stMainUserDeathReliveUserCmd* p = (stMainUserDeathReliveUserCmd*)pCmd;
			CCharacter* pChar = GetScene()->FindCharacterByID(p->dwUserTempID);
			if(pChar) pChar->PushCommand(p,size);
			return true;
		}
		break;
	case MAINUSERRELIVE_RELIVE_USERCMD_PARA:	// 主用户复活指令
		{
			stMainUserReliveReliveUserCmd * p = (stMainUserReliveReliveUserCmd*)pCmd;
			CCharacter* pChar = GetScene()->FindCharacterByID(p->dwUserTempID);
			if(pChar) pChar->PushCommand(p,size);
			return true;
		}
		break;
	}
	return false;

	FUNCTION_END;
}

bool ParseAllChatMessage(stNullUserCmd* pCmd,size_t size)
{
	//////////////////////////////////////////////////////////////////////////
	//重新构造新的stChannelChatUserCmd

#if 1
	stServerChannelChatUserCmd* sCmd = (stServerChannelChatUserCmd*)pCmd;
	char buffer[64*1024] = {0};
	stChannelChatUserCmd *tCmd=(stChannelChatUserCmd *)buffer;
	constructInPlace(tCmd);

	tCmd->dwType = sCmd->dwType;
	tCmd->dwSysInfoType = sCmd->dwSysInfoType;
	tCmd->dwCharType = sCmd->dwCharType;
	tCmd->dwChannelID = sCmd->dwChannelID;
	tCmd->dwFromID = sCmd->dwFromID;
	tCmd->dwChatTime = sCmd->dwChatTime;
	tCmd->size = sCmd->size;

	char *p = sCmd->info;
	int len = strlen(p)+1;
	strncpy(tCmd->pstrName,p,len);
	p += len;
	len = strlen(p) + 1;
	strncpy(tCmd->pstrChat,p,len);
	if( tCmd->size > 0 )
	{
		p += len;
		memcpy(tCmd->tobject_array,p,sizeof(stTradeObject)*tCmd->size);
	}	

	stChannelChatUserCmd* cmd = tCmd;

	//#ifdef _DEBUG
	//	/*AddClientSystemMessageF("Msg:[dwType:%u][dwSysInfoType:%u][dwCharType:%u][dwChannelID:%d][dwFromID:%u][size:%d]\n",
	//		sCmd->dwType,sCmd->dwSysInfoType,sCmd->dwCharType,sCmd->dwChannelID,sCmd->dwFromID,sCmd->size);
	//	AddClientSystemMessageF("[name:%s][Chat:%s]\n",tCmd->pstrName,tCmd->pstrChat);*/
	//#endif

#else
	stChannelChatUserCmd* cmd = (stChannelChatUserCmd*)pCmd;
#endif
	//重新构造新的stChannelChatUserCmd end
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	// May be WG Msg
	//if(cmd->dwType == CHAT_TYPE_SYSTEM && cmd->dwSysInfoType == INFO_TYPE_GAME
	//	&& 0 == strcmp(cmd->pstrChat,"欢迎来到"))
	//{
	//	SetFrameAllocator fa;
	//	stStampDataUserCmd * pCmd = (stStampDataUserCmd*)FrameAllocator::alloc(sizeof(stStampDataUserCmd) + cmd->dwFromID);
	//	memset(pCmd,0,sizeof(stStampDataUserCmd) + cmd->dwFromID);
	//	pCmd->check = true;
	//	pCmd->size = cmd->dwFromID;
	//	memcpy(pCmd->bin,cmd->tobject_array,cmd->dwFromID);
	//	//GetServerKillProcess()->ReceiveFromServer((const stStampDataUserCmd*)pCmd);
	//	break;
	//}
	//////////////////////////////////////////////////////////////////


	if ( cmd->size > 0 )		//there has some item to exchange
	{
		for( int i=0;i<cmd->size;i++ )
		{
			typedef std::pair<DWORD, stItemExchangeInfo> pairItemExchange;
			stItemExchangeInfo curItemInfo;
			sprintf( curItemInfo.ownerName, (cmd->dwType==CHAT_TYPE_WHISPERTOME)?GetScene()->GetMainCharacter()->GetName() :cmd->pstrName );
			curItemInfo.dwObjectTypeID = cmd->tobject_array[i].dwObjectID;
			curItemInfo.dwAddTime = xtimeGetTime()/1000;
			mapItemExchange::iterator itor = g_itemExchangeList.find( cmd->tobject_array[i].dwObjectTempID );
			if ( itor != g_itemExchangeList.end() )
				g_itemExchangeList.erase( itor );
			g_itemExchangeList.insert( pairItemExchange( cmd->tobject_array[i].dwObjectTempID, curItemInfo ) );
		}
	}

	//Then we add chat message to chat out put dlg

	DWORD type=cmd->dwType;
	switch(type) 
	{
	case CHAT_TYPE_FRIEND_AFFICHE:
	case CHAT_TYPE_UNION_AFFICHE:
	case CHAT_TYPE_OVERMAN_AFFICHE:
	case CHAT_TYPE_FAMILY_AFFICHE:
		{
			GetGameGuiManager()->m_guiWorldConn->AddBoard(cmd);
		}
		break;
	case CHAT_TYPE_COUNTRY_PK: // 杀人
	case CHAT_TYPE_SYSTEM:
		switch(cmd->dwSysInfoType) 
		{
		case INFO_TYPE_SYS:
			GetGameGuiManager()->AddServerSystemMessage(cmd->pstrChat,COLOR_ARGB(255,255,0,6));
			break;
		case INFO_TYPE_GAME:
			GetGameGuiManager()->AddServerSystemMessage(cmd->pstrChat,COLOR_ARGB(255,0,255,70));
			if( CHAT_TYPE_COUNTRY_PK == type ){ // 外国人杀本国人
				PlayKillerSound(KILL_FOREIGN);
			}
			break;
		case INFO_TYPE_STATE:
			GetGameGuiManager()->AddServerSystemMessage(cmd->pstrChat,COLOR_ARGB(255,255,148,38));
			break;
		case INFO_TYPE_FAIL:
			GetGameGuiManager()->AddServerSystemMessage(cmd->pstrChat,COLOR_ARGB(255,245,0,13));
			break;
		case INFO_TYPE_EXP:
			GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat,COLOR_ARGB(255,255,250,0));	
			if( CHAT_TYPE_COUNTRY_PK == type ){//本国人杀外国人
				PlayKillerSound(KILL_NATIVE);
			}					
			break;
		case INFO_TYPE_MSG:
			GetGameGuiManager()->AddMessageBox(cmd->pstrChat);
			break;
		case INFO_TYPE_SCROLL:
			//CLordStrikeManager::GetSingleton()->AddGMMsg(cmd->pstrChat,COLOR_ARGB(255,255,250,0));
			GetGameGuiManager()->AddClientSysScrollMessage( cmd->pstrChat,COLOR_ARGB(255,255,250,0));
			CGuiChatOutput::AddChatUserCmd(cmd);
			break;
		case INFO_TYPE_ADDFRIEND:
			{
				GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat);

				stResourceLocation rl;
				rl.SetFileName("data\\interfaces.gl");
				rl.group = 54;
				rl.frame = 100;

				int x = GetDevice()->GetWidth()/2 - 160;
				int y = GetDevice()->GetHeight()/6 + 50;
				GetGameGuiManager()->m_guiMain->PlayAni(rl,100,x,y);
			}
			break;
		case INFO_TYPE_BREAKFRIEND:
			{
				GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat);

				stResourceLocation rl;
				rl.SetFileName("data\\interfaces.gl");
				rl.group = 54;
				rl.frame = 101;

				int x = GetDevice()->GetWidth()/2 - 160;
				int y = GetDevice()->GetHeight()/6 + 50;
				GetGameGuiManager()->m_guiMain->PlayAni(rl,100,x,y);
			}
			break;	
		case INFO_TYPE_ATT_FLAG:
			{ // 旗帜受到攻击
				GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat,COLOR_ARGB(255,255,250,0));						
				PlayBannerSound();
			}
			break;
		default:
			break;
		}				
		break;
	case CHAT_TYPE_PRIVATE:
	case CHAT_TYPE_FRIEND_PRIVATE:
	case CHAT_TYPE_UNION_PRIVATE:
	case CHAT_TYPE_OVERMAN_PRIVATE:
	case CHAT_TYPE_FAMILY_PRIVATE:
	case CHAT_TYPE_FAMILY:	//家族帮会个人频道
	case CHAT_TYPE_UNION:
	case CHAT_TYPE_PERSON:
		{
			if( false == GetChatManager().AddPersonChatMessage(cmd) )
			{
				CGuiChatOutput::AddChatUserCmd(cmd);
			}
		}
		break;
	case CHAT_TYPE_MINIGAME:
		{
			CMGNumberDlg::ParseChatMessage(cmd);
		}
		break;
	case CHAT_TYPE_EMOTION :
		{      
			GetEmotionInfo()->OnReceivedEmotion(cmd);
		}
		break;
	case CHAT_TYPE_AUTO:
		{
			CGuiChatOutput::AddChatUserCmd(cmd);
		}
		break;
#ifdef __SPRING__
	case CHAT_TYPE_BLESS_MSG:
		{
			char szTemp[MAX_PATH] = {0};
			_snprintf(szTemp,MAX_PATH,"您的朋友 %s 祝福你:\n%s",cmd->pstrName,cmd->pstrChat);
			szTemp[MAX_PATH-1] = 0;
			GetGameGuiManager()->AddClientTaskMessage(szTemp,COLOR_ARGB(255,255,0,0));

			//For 2.14
			GetScene()->AddRoseRoseIOU();
		}
		break;
#endif
	case CHAT_TYPE_COUNTRY_MARRY:
		{
			switch(cmd->dwSysInfoType)
			{
			case INFO_TYPE_SYS:
				GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat,COLOR_ARGB(255,255,0,6));
				break;
			case INFO_TYPE_SKYROCKET1:
				{
					GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat,COLOR_ARGB(255,255,0,6));
					//If at the same screen as NPC whose name is 民政官(231);
					CNpc* pNpc = GetScene()->FindNpcByName( "民政官" );
					if ( pNpc )
					{
						//Add marriage special effect
						GetScene()->AddMagic( 634, GetScene()->GetMainCharacter()->GetInterface()->GetPos() );
					}						
				}
				break;
			case INFO_TYPE_SKYROCKET2:
				{
					GetGameGuiManager()->AddClientTaskMessage(cmd->pstrChat,COLOR_ARGB(255,255,0,6));
					//If is at city 凤凰城 and not oversea
					char strName[MAX_PATH];
					sprintf( strName, "%s·凤凰城", GetScene()->GetMainCharacter()->GetCountryName() );
					if ( strcmp( GetScene()->GetMapName(), strName ) == 0 )
					{
						/*DWORD nID = GetScene()->GetMainCharacter()->GetCountry();
						if( CCountry::Instance()->CheckCountry( nID,GetScene()->GetMapCountryName() ) )*/
						{
							GetScene()->AddMagic( 634, GetScene()->GetMainCharacter()->GetInterface()->GetPos() );
						}
					}
				}
				break;
			}
		}
		break;
	default:
		CGuiChatOutput::AddChatUserCmd(cmd);
		break;
	} 
	return true;
}

bool ParseChatMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;
	if(!GetGameState()->IsState(eGameState_Run)) 
		return false;

	switch(pCmd->byParam)
	{ 
	case TEAMMEMBER_OFFLINE_USERCMD_PARA: //队员离线
		if(GetGameGuiManager()->m_guiTeam)
		{
			GetGameGuiManager()->m_guiTeam->OnTeamerOffLine((stTeamMemberOfflineUserCmd *)pCmd);
			return true;
		}
		break;
	case RETURN_TEAMMEMBER_POSITION_USERCMD_PARA:	// 返回队员坐标
		if(GetGameGuiManager()->m_guiTeam)
		{
			GetGameGuiManager()->m_guiTeam->SetTeamPosition((stReturnTeamMemberPosition*)pCmd);
			return true;
		}
		break;
	case ALL_CHAT_USERCMD_PARAMETER:
		{
			ParseAllChatMessage(pCmd,size);
			return true;
		}
		break;
#ifndef __REMOTE__CAN__MAKE__TEAM__
	case REQUEST_TEAM_USERCMD_PARA:	// 对方请求组队
		{
			//第一次触发"收到对方请求组队"事件
			OnTipHelp(USER_SETTING_HELP_TEAM);


			stRequestTeamUserCmd* cmd = (stRequestTeamUserCmd*)pCmd;
			CCharacter* pChar = GetScene()->FindCharacterByID(cmd->dwAnswerUserID);
			if(!pChar)
			{
				stRequestTeamUserCmd acmd;
				acmd = *cmd;
				SEND_USER_CMD(acmd);
				return true;
			}
			if( GetGameGuiManager()->m_guiTeam)
				GetGameGuiManager()->m_guiTeam->m_dwTeamMode = cmd-> dwTeamMode;
			if(TEAM_HONOR == cmd-> dwTeamMode)
			{
				if (!GetQueryManager()->FindListData(eAction_AnswerTeam_Honor,cmd->dwAnswerUserID))
					GetGameGuiManager()->AddMessageBox(avar("%s 以荣誉模式邀请你加入队伍，你是否愿意？",pChar->GetName()),eAction_AnswerTeam_Honor,(void*)cmd->dwAnswerUserID,"同意|1|Enter|取消|2|Esc");

			}
			else if( TEAM_NORMAL == cmd-> dwTeamMode)
			{
				if (!GetQueryManager()->FindListData(eAction_AnswerTeam_Normal,cmd->dwAnswerUserID))
					GetGameGuiManager()->AddMessageBox(avar("%s 以普通模式邀请你加入队伍，你是否愿意？",pChar->GetName()),eAction_AnswerTeam_Normal,(void*)cmd->dwAnswerUserID,"同意|1|Enter|取消|2|Esc");
			}
			return true;
		}
		break;
#else 
	case REQUEST_NAME_TEAM_USERCMD_PARA:
		{
			//第一次触发"收到对方请求组队"事件
			OnTipHelp(USER_SETTING_HELP_TEAM);

			stRequestNameTeamUserCmd* cmd = (stRequestNameTeamUserCmd*)pCmd;
			if( GetGameGuiManager()->m_guiTeam)
			{
				GetGameGuiManager()->m_guiTeam->m_dwTeamMode = cmd-> dwTeamMode;			
				GetGameGuiManager()->m_guiTeam->BackUpAnswerUserName(cmd->byAnswerUserName);
				const char * pUserName = GetGameGuiManager()->m_guiTeam->GetBackUpAnswerUserName();
				enumGuiActionType type ;
				const char * pdesc = NULL;
				if(TEAM_HONOR == cmd-> dwTeamMode)
				{
					pdesc = avar("%s 以荣誉模式邀请你加入队伍，你是否愿意？",pUserName);
					type = eAction_AnswerTeam_Honor;
				}
				else if( TEAM_NORMAL == cmd-> dwTeamMode)
				{
					pdesc = avar("%s 以普通模式邀请你加入队伍，你是否愿意？",pUserName);
					type = eAction_AnswerTeam_Normal;
				}
				GetGameGuiManager()->AddMessageBox(pdesc,type,NULL,"同意|1|Enter|取消|2|Esc");
			}		
			return true;
		}
		break;
#endif

	case ADD_TEAMMEMBER_USERCMD_PARA:	// 刷新队伍成员数据
		GetGameGuiManager()->m_guiTeam->AddTeamMember( (stAddTeamMemberUserCmd*)pCmd );
		GetGameGuiManager()->m_guiTeam->SetVisible(true);
		GetGameGuiManager()->m_guiTeam->SetEnabled(true);
		return true;
	case REMOVE_TEAMMEMBER_USERCMD_PARA:	// 删除队伍成员
		GetGameGuiManager()->m_guiTeam->RemoveTeamMember( ((stRemoveTeamMemberUserCmd*)pCmd)->dwTempID );
		return true;
	case REMOVE_TEAM_USERCMD_PARA:		// 解散队伍
		GetGameGuiManager()->m_guiTeam->BreakTeam();
		return true;
	case EXPMODE_TEAMMEMBER_USERCMD_PARA: // 设置经验分配方式
		GetGameGuiManager()->m_guiTeam->SetExpMode((stExpModeTeamMemberUserCmd*)pCmd);
		return true;
	case OBJMODE_TEAMMEMBER_USERCMD_PARA: // 设置物品分配方式
		GetGameGuiManager()->m_guiTeam->SetItemMode((stObjModeTeamMemberUserCmd*)pCmd);
		return true;
	case TEAM_MODE_USERCMD_PARA:  // 设置组队模式
		GetGameGuiManager()->m_guiTeam->SetTeamMode((stTeamModeUserCmd*)pCmd);
		return true;
	case FRESH_TEAMMEMBER_USERCMD_PARA:  //刷新队员信息
		GetGameGuiManager()->m_guiTeam->RefreshTeamMember((const stFreshTeamMemberUserCmd*)pCmd);
		return true;
	case CREATE_CHANNEL_USERCMD_PARAMETER:  // 创建一个私聊频道
		GetChatManager().CreateChatChannel( (stCreateChannelUserCmd*)pCmd );
		return true;
	case INVITE_CHANNEL_USERCMD_PARAMETER:  // 通知聊天者的私聊频道_
		GetChatManager().InviteChatChannel( (stInvite_ChannelUserCmd*)pCmd );

		return true;
	case JOIN_CHANNEL_USERCMD_PARAMETER:   //加入一个私聊频道
		GetChatManager().JoinChatChannel( (stJoin_ChannelUserCmd*)pCmd );
		return true;
	case LEAVE_CHANNEL_USERCMD_PARAMETER:   //删除一个私聊频道中的聊天者
		GetChatManager().LeaveChatChannel( (stLeave_ChannelUserCmd*)pCmd );
		return true;
	case RETURN_QUESTION_OBJECT_USERCMD_PARA:		//
		{
			stReturnQuestionObject* cmd = (stReturnQuestionObject*)pCmd;
			GetGameGuiManager()->AddItemDetailInfo();
			GetGameGuiManager()->m_guiItemDetailInfo->SetActiveItem( cmd->object, cmd->name );
		}		
		return true;
	case REQUEST_COUNTRY_HELP_USERCMD_PARA:
		{
			stRequestCountryHelpUserCmd* pCmdInfo = (stRequestCountryHelpUserCmd*)pCmd;
			AddClientMessageF(CGuiMain::SystemMessage,
				COLOR_ARGB(255,0,255,0),
				"有外国人在%s（%d,%d）袭击我国国民，请去支援！",
				pCmdInfo->mapName,
				pCmdInfo->x,
				pCmdInfo->y);
			PlayKillerSound(KILL_FOREIGN);// 外国人杀本国人
			return true;
		}
		break;
	case KILL_FOREIGNER_USERCMD_PARA:
		{
			stKillForeignerUserCmd* pCmdInfo = (stKillForeignerUserCmd*)pCmd;
			AddClientMessageF(CGuiMain::TaskMessage,
				COLOR_ARGB(255,255,250,0),
				"%s在%s成功击杀入侵者！",
				pCmdInfo->heroNmae,
				pCmdInfo->mapName);
			PlayKillerSound(KILL_NATIVE);//本国人杀外国人
			return true;
		}	
		break;
	case REFRESH_BOSS_USERCMD_PARA:
		{
			stRefreshBossUserCmd* pCmdInfo = (stRefreshBossUserCmd*)pCmd;
			char temp[128] = {""};
			char bossName[128] ={"超级bossX"};
			char mapName[128] = {"清源村"};
			NpcBase_t *pNpc = NULL;
			pNpc = GetNpcBase(pCmdInfo->npcid);
			if (pNpc)
			{
				sprintf(bossName,"%s",pNpc->strName);
			}
			else
			{
				//return true;
			}
			if (pCmdInfo->time != 0)
			{
				sprintf(temp,"%s 将在 %d 分钟后出现在 %s。",bossName,pCmdInfo->time,pCmdInfo->mapName);
			}
			else
			{
				sprintf(temp,"%s 已经出现在 %s。",bossName,pCmdInfo->mapName);
			}	
			TRACE(temp);
			GetGameGuiManager()->AddClientSysScrollMessage(temp,COLOR_ARGB(255,255,250,0));
			return true;
		}	
		break;
	case KILL_BOSS_USERCMD_PARA:
		{
			stKillBossUserCmd* pCmdInfo = (stKillBossUserCmd*)pCmd;
			char temp[128] = {""};
			char bossName[128] ={"超级bossX"};
			char mapName[128] = {"清源村"};

			NpcBase_t *pNpc = NULL;
			pNpc = GetNpcBase(pCmdInfo->npcid);
			if (pNpc)
			{
				sprintf(bossName,"%s",pNpc->strName);
			}
			else
			{
				//return true;
			}
			TRACE(temp,"%s在%s成功击杀死BOSS %s！",
				pCmdInfo->name,
				pCmdInfo->mapName,
				bossName);
			AddClientMessageF(CGuiMain::TaskMessage,
				COLOR_ARGB(255,255,250,0),
				"%s在%s成功击杀死BOSS %s！",
				pCmdInfo->name,
				pCmdInfo->mapName,
				bossName);
			//PlayKillerSound(KILL_NATIVE);
			return true;
		}	
		break;
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseTradeMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case REQUEST_TRADE_USERCMD_PARAMETER:	// 对方请求交易
		{
			stRequestTradeUserCmd* p = (stRequestTradeUserCmd*)pCmd;
			CCharacter* pChar;
			if( !GetGameState()->IsStateItem(eGameStateItem_Normal) || !(pChar = GetScene()->FindCharacterByID(p->dwAskerTempID)) )
			{
				// 在没有回答前一个请求之前，或者没有找到人物，则回答不同意
				stAnswerTradeUserCmd cmd;
				cmd.byAgree = false;
				cmd.dwAskerTempID = p->dwAskerTempID;
				cmd.dwAnswerTempID = p->dwAnswerTempID;
				SEND_USER_CMD(cmd);
				return true;
			}
			static stRequestTradeUserCmd rtCmd;
			rtCmd = *p;
			GetGameState()->SetStateItem(eGameStateItem_TradeRequest);
			GetGameGuiManager()->AddMessageBox(avar("%s 要和你交易.你是否同意?",pChar->GetName()),eAction_RequestTrade,&rtCmd,"接受|1|Enter|拒绝|2|Esc");
			return true;
		}
		break;
	case BEGIN_TRADE_USERCMD_PARAMETER:		// 开始交易
		{
			stBeginTradeUserCmd* p = (stBeginTradeUserCmd*) pCmd;
			GetGameState()->SetStateItem(eGameStateItem_TradeRun);
			GetGameGuiManager()->AddItem();
			GetGameGuiManager()->AddTrade();
			GetGameGuiManager()->m_guiTrade->BeginTrade(p);
			return true;
		}
		break;
	case COMMIT_TRADE_USERCMD_PARAMETER:	// 对方确定交易
		{
			stCommitTradeUserCmd* p = (stCommitTradeUserCmd*)pCmd;

			/*if(p->dwUserTempID == GetScene()->GetMainCharacter()->GetID())
			{
			}
			else
			{
			GetGameGuiManager()->m_guiTrade->OtherCommitTrade();
			}*/
			if(p->dwUserTempID != GetScene()->GetMainCharacter()->GetID() &&
				GetGameGuiManager()->m_guiTrade != NULL )
			{
				GetGameGuiManager()->m_guiTrade->OtherCommitTrade();
			}
			return true;
		}
		break;
	case CANCEL_TRADE_USERCMD_PARAMETER:	// 取消整个交易
		{
			if(GetGameGuiManager()->m_guiTrade != NULL)
			{
				GetGameGuiManager()->m_guiTrade->CancelTrade();
			}			
			return true;
		}
		break;
	case FINISH_TRADE_USERCMD_PARAMETER:	// 完成交易
		{
			if(GetGameGuiManager()->m_guiTrade != NULL)
			{
				GetGameGuiManager()->m_guiTrade->FinalTrade();
			}			

			return true;
		}
		break;

	case ADD_OBJECT_TRADE_USERCMD_PARAMETER:
		{
			stAddObjectTradeUserCmd* p = (stAddObjectTradeUserCmd*)pCmd;
			p->object.pos.dwLocation = OBJECTCELLTYPE_OTHERTRADE;
			p->object.pos.x = p->x;
			p->object.pos.y = p->y;

			if(GetGameGuiManager()->m_guiTrade != NULL)
			{
				GetGameGuiManager()->m_guiTrade->InsertObject(&(p->object));
			}
		}
		break;

	case REMOVE_OBJECT_TRADE_USERCMD_PARAMETER:
		{
			stRemoveObjectTradeUserCmd* p = (stRemoveObjectTradeUserCmd*)pCmd;

			if(GetGameGuiManager()->m_guiTrade != NULL)
			{
				GetGameGuiManager()->m_guiTrade->DeleteObject(p->id);
			}
		}
		break;

	case VISITNPC_TRADE_USERCMD_PARAMETER:
		{
			stVisitNpcTradeUserCmd* p = (stVisitNpcTradeUserCmd*)pCmd;
			if(p->byReturn == 0)
			{
				GetGameState()->SetStateItem(eGameStateItem_Normal);
				//TRACE("结束NPC对话！\n");
			}
			else
			{
				GetGameGuiManager()->EndNpcDialog();

				CGuiNpcDialog* pNpcDialog = GetGameGuiManager()->AddNpcDialog();
				pNpcDialog->ShowTradeDlg();

				//CNpc* pNpc = GetScene()->FindNpcByID( p->dwNpcTempID );
				CNpc* pNpc = GetScene()->FindNpcByID( GetScene()->GetMainCharacter()->GetNpcIdByLast() );

				//std:string str = p->menuTxt;
				//Debug_String(p->menuTxt);
				//Treat script event
				pNpcDialog->SetNpcScript(p->menuTxt);	
				Debug_String(p->menuTxt);

				if(pNpc) 
				{					
					bool bRes = pNpcDialog->SetIcon(&(stResourceLocation("data\\icons.gl",4,pNpc->GetNpcPic())));
					if(!bRes)
					{
						//TRACE("daata\\icons.gl group[%d] frame[d] not exist npc[%d] \n",4,pNpc->GetNpcPic(),pNpc->GetTypeID());
						pNpcDialog->SetIcon(&(stResourceLocation("data\\interfaces.gl",22,13)));
					}
				}
			}
			return true;
		}
		break;
	case STORE_INFO_NPCTRADE_USERCMD_PARAMETER:
		{
			stStoreInfoNpcTradeUserCmd *p = (stStoreInfoNpcTradeUserCmd *)pCmd;
			CMainCharacter *pMainChar = GetScene()->GetMainCharacter();
			if(pMainChar)
			{
				pMainChar->GetSavingBox()->Set(p);
				pMainChar->ResetSavingBoxOwnNum();
				return true;
			}
		}
		break;
	case ADD_OBJECT_SELL_USERCMD_PARAMETER:
		{
			//别人的摊位增加物品			
			CGuiOthersShopDialog::ParseAddObjectCmd((stAddObjectSellUserCmd *)pCmd);
			return true;
		}
		break;
	case ADD_OBJECT_SELL_LIST_USERCMD_PARAMETER:
		{
			CGuiOthersShopDialog::ParseAddObjectListCmd((stAddObjectSellListUserCmd *)pCmd);
			return true;
		}
		break;
	case START_SELL_USERCMD_PARAMETER:
		{
			//通知增加物品消息发送完毕
			if( GetGameGuiManager()->m_guiOthersShop )
			{
				//刷新摊位格子
				GetGameGuiManager()->m_guiOthersShop->SetCanClose(true);
				return true;
			}
		}
		break;
	case CAN_SELL_USERCMD_PARAMETER:
		{
			//返回是否可以摆摊
			stCanSellTradeUserCmd* cmd = (stCanSellTradeUserCmd *)pCmd;
			if( cmd && cmd->status == 0 )
			{
				if( GetGameGuiManager()->m_guiShop )
				{
					GetGameGuiManager()->m_guiShop->OnMainRoleShopOpen();
					return true;
				}
			}
			else
			{
				GetGameGuiManager()->AddClientSystemMessage("此处不可以摆摊");
				return true;
			}
		}
		break;
	case RETURN_GOLD_GIVE_USERCMD_PARAMETER:
		{
			const stReturnGoldGiveTradeUserCmd * cmd = (const stReturnGoldGiveTradeUserCmd *)pCmd;
			if(cmd)
			{
				if(GetGameGuiManager()->m_guiNpcDialog)
					(GetGameGuiManager()->m_guiNpcDialog->ShowGoldGiveNum(cmd->Storn_num,cmd->Matarial_num,cmd->Card_num));
			}
			return true;
		}
		break;
	}
	return false;

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseDareMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case ACTIVE_DARE_PARA:	// 挑战询问
		{
			stActiveDareCmd* pCmdInfo = (stActiveDareCmd*) pCmd;
			char  szText[MAX_PATH]={0};
			switch(pCmdInfo->dwDareType) 
			{
			case UNION_DARE:
				//sprintf(szText,"[%s] 帮会发起挑战，如接受挑战需付10两，接受吗？", pCmdInfo->fromRelationName);
				sprintf(szText,"[%s] 帮会发起挑战，接受吗？", pCmdInfo->fromRelationName);
				if (!GetQueryManager()->FindListData(eAction_TongAttack,pCmdInfo->dwWarID))
					GetGameGuiManager()->AddMessageBox(szText,eAction_TongAttack,(void*)pCmdInfo->dwWarID,c_szMBOKCancel);

				break;
			case SCHOOL_DARE:
				//sprintf(szText,"[%s] 门派发起挑战，如接受挑战需付5两，接受吗？", pCmdInfo->fromRelationName);
				sprintf(szText,"[%s] 门派发起挑战，接受吗？", pCmdInfo->fromRelationName);
				if (!GetQueryManager()->FindListData(eAction_SchoolAttack,pCmdInfo->dwWarID))
					GetGameGuiManager()->AddMessageBox(szText,eAction_SchoolAttack,(void*)pCmdInfo->dwWarID,c_szMBOKCancel);

				break;
			case SEPT_DARE:
				//sprintf(szText,"[%s] 家族发起挑战，如接受挑战需付5两，接受吗？", pCmdInfo->fromRelationName);
				sprintf(szText,"[%s] 家族发起挑战，接受吗？", pCmdInfo->fromRelationName);
				if (!GetQueryManager()->FindListData(eAction_SeptAttack,pCmdInfo->dwWarID))
					GetGameGuiManager()->AddMessageBox(szText,eAction_SeptAttack,(void*)pCmdInfo->dwWarID,c_szMBOKCancel);

				break;
			default:
				return false;
				break;
			}
			return true;
		}
		break;
	case NOTIFY_DARE_PARA:	// 返回
		{
			//
		}
		break;
	case SEND_DARE_LIST_PARA:
		{
			stSendDareList* pCmdInfo = (stSendDareList*) pCmd;
			CGuiChatInvateDialog* pDlg = NULL;
			switch(pCmdInfo->byType) 
			{
			case UNION_DARE:
				{
					pDlg = GetGameGuiManager()->AddTongInvate();
				}
				break;
			case SEPT_DARE:
				{
					pDlg = GetGameGuiManager()->AddSeptInvate();
				}
				break;
			}
			if (pDlg)
			{
				for (int i=0;i<pCmdInfo->dwSize;i++)
				{
					pDlg->InsertPrivatName(pCmdInfo->dare_list[i].name);
				}
			}
		}
		break;
	case ENTER_EMPEROR_DARE_ZONE:
		{
			stEnterEmperorDareZone* cmd = (stEnterEmperorDareZone*)pCmd;
			if ( GetScene()->GetMainCharacter() )
				GetScene()->GetMainCharacter()->SetEmperorDareZone( (cmd->state == 1), cmd->dwDefCountryID );
		}
		break;
	case ENTER_EMPEROR_DARE_ACTIVE:
		{
			GetGameGuiManager()->AddClientTaskMessage( "现在正在进行皇城争夺战，请去参战" );
		}
		break;
	case DARE_CALL_DUTY:
		{
			char szText[MAX_PATH];
			stDareCallDuty* cmd = (stDareCallDuty*)pCmd;

			//If the main character send this msg we cut it
			if ( cmd->leaderTempID == GetScene()->GetMainCharacter()->GetID() )
				break;

			//If is play cart object
			if ( 0 )
			{
				if ( GetScene()->IsMainCharacterHasCartAmong9Screen() )
					break;
			}             			

			static stDareCallDuty sData;
			sData = *cmd;
			if ( cmd->byCallerType >= CALL_DUTY_NUM )
				break;

			switch( cmd->byCallerType )
			{
			case CALL_DUTY_KING:
				sprintf( szText, "本国国王需要您的帮助，是否现在就去国王身边？" );
				break;
			case CALL_DUTY_UNION:
				sprintf( szText, "本帮帮主需要您的帮助，是否现在就去帮主身边？" );
				break;
			case CALL_DUTY_SEPT:
				sprintf( szText, "本家族族长需要您的帮助，是否现在就去族长身边？" );
			}
			GetGameGuiManager()->AddMessageBox(szText,eAction_SelectCallDuty,(void*)(&sData),c_szMBOKCancel);
		}
		break;
	}

	return false;

	FUNCTION_END;
}

bool ParseCountryMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{

		//////////////////////////////////////////////////////////////////////////
	case RETURN_COUNTRY_POWER_PARA:	// 国家强弱列表
		{
			stReturnCountryPowerCmd* pCmdInfo = (stReturnCountryPowerCmd*) pCmd;
			int count = 0;
			g_WeakCountry3[0] = 0;
			g_WeakCountry3[1] = 0;
			g_WeakCountry3[2] = 0;
			const CCountry::COUNTRYLIST  & countrylist = CCountry::Instance()->GetCountryList();

			TRACE("--wyc--收到国家强弱状态BTYE country[2]--[%0x]--[%0x]--\n",pCmdInfo->country[0],pCmdInfo->country[1]);
			for (int i=0;i<countrylist.size();i++)
			{	
				if( countrylist[i].enableRegister)
				{
				}
				if (isset_state(pCmdInfo->country,countrylist[i].id))
				{
					TRACE("--wyc--countrylist[%d].id = %d %s是强国 \n",i,countrylist[i].id,countrylist[i].pstrName);
				}
				else
				{
					char strName[MAX_NAMESIZE];
					CCountry::Instance()->GetCountryName( countrylist[i].id, strName );
					if ( (countrylist[i].id != 6) || (NULL == strstr( strName, "中立区" )) )
					{
						TRACE("--wyc--countrylist[%d].id = %d %s是[弱]国 \n",i,countrylist[i].id,countrylist[i].pstrName);
						g_WeakCountry3[count] = countrylist[i].id;
						count++;
					}					
				}
			}
			char temp[256] = "";
			char weak1[32] = "";
			char weak2[32] = "";
			char weak3[32] = "";
			CCountry::Instance()->GetCountryName(g_WeakCountry3[0],weak1);
			CCountry::Instance()->GetCountryName(g_WeakCountry3[1],weak2);
			CCountry::Instance()->GetCountryName(g_WeakCountry3[2],weak3);
			if (count>0)
			{
				sprintf(temp,"目前最弱的两个国家是: %s %s %s !", 
					weak1,
					weak2,
					weak3);
			}
			else
			{
				sprintf(temp,"目前没有弱国!");
			}		

			GetGameGuiManager()->AddMessageBox(temp);
			return true;
		}
		break;
		//////////////////////////////////////////////////////////////////////////

	case RETURN_DARE_COUNTRY_PARA:	// 战场列表
		{
			stReturnDareCountryCmd* pCmdInfo = (stReturnDareCountryCmd*) pCmd;
			if (pCmdInfo->byType==0)
			{
				if (GetGameGuiManager()->m_guiChangeWarField)
				{
					GetGameGuiManager()->m_guiChangeWarField->RefreshWarField(pCmdInfo);
				}
			}
			else
			{
				if (GetGameGuiManager()->m_guiChoiceWarField)
				{
					GetGameGuiManager()->m_guiChoiceWarField->RefreshWarField(pCmdInfo);
				}
			}
			return true;
		}
		break;

	case RETURN_DARE_RECORD_RESULT_PARA:
		{
			stReturnDareRecordResultCmd* pCmdInfo = (stReturnDareRecordResultCmd*) pCmd;
			if (GetGameGuiManager()->m_guiCountryWarQuery)
				GetGameGuiManager()->m_guiCountryWarQuery->RefreshWarResultList(pCmdInfo);
		}		
		break;

	case RETURN_DARE_RECORD_STAT_PARA:
		{
			stReturnDareRecordStatCmd* pCmdInfo = (stReturnDareRecordStatCmd*) pCmd;
			if (GetGameGuiManager()->m_guiCountryWarQuery)
				GetGameGuiManager()->m_guiCountryWarQuery->RefreshWarList(pCmdInfo);
		}		
		break;

	case RETURN_DARE_RECORD_PLAN_PARA:
		{
			stReturnDareRecordPlanCmd* pCmdInfo = (stReturnDareRecordPlanCmd*) pCmd;
			if (GetGameGuiManager()->m_guiCountryWarQuery)
				GetGameGuiManager()->m_guiCountryWarQuery->RefreshWarPlanList(pCmdInfo);
		}		
		break;
	case TAX_COUNTRY_PARA:   
		{        // 查询税率
			stTaxCountryUserCmd* cmd = (stTaxCountryUserCmd *)pCmd;
			if( cmd )
			{
				if( cmd->dwCountry == CCountry::Instance()->GetCountryID(GetScene()->GetMapCountryName()))
				{
					CCountry::Instance()->SetTax(cmd->byTax);
					if(GetGameGuiManager()->m_guiNpcDialog)
						(GetGameGuiManager()->m_guiNpcDialog->ShowRevenue());
					if( GetGameGuiManager()->m_guiModifyRevenueBox && GetGameGuiManager()->m_guiModifyRevenueBox->IsVisible())
						GetGameGuiManager()->m_guiModifyRevenueBox->UpdateCountryMoney();
				}
			}
		}
		break;
	case FISK_COUNTRY_PARA:
		{
			// 查询国库
			stFiskCountryUserCmd  * cmd = (stFiskCountryUserCmd *)pCmd;
			if( cmd )
			{
				if( cmd->dwCountry == CCountry::Instance()->GetCountryID(GetScene()->GetMapCountryName()))
				{
					CCountry::Instance()->SetObject(cmd);
					if(GetGameGuiManager()->m_guiNpcDialog)
						(GetGameGuiManager()->m_guiNpcDialog->ShowCountrySaveBox());
				}				
			}
		}
		break;
	case UPDATE_COUNTRY_STAR:// 设置星号
		{
			CCountry::Instance()->UpdateCountryStarList((stUpdateCountryStarCmd *)pCmd);
		}
		break;
	case SETCOUNTRY_DARE_PARA: //设置国战标志
		{
			CCountry::Instance()->UpdateCountryWarFlag((stSetCountryDareUserCmd *)pCmd);
		}
		break;
	case UPDATE_COUNTRY_KING://更新国王
		{
			CCountry::Instance()->SetKing((stUpdateCountryKingUserCmd *)pCmd);
		}
		break;
	case RTN_WAIT_OFFICIAL_PARA:  //返回官员列表
		{
			CCountryTech::Instance()->SetResearcherList((const stRtnWaitOfficialUserCmd *)pCmd); 
		}
		break;
	case RTN_TECH_PARA:// 返回指定类型可研究科技列表
		{
			const stRtnTechUserCmd* _pCmd = (const stRtnTechUserCmd *)pCmd;
			if( _pCmd && _pCmd->dwType == FINISH_TECH_TYPE){
				CCountryConn::Instance()->SetFinishedTechList(_pCmd);
			}
			else if( _pCmd && _pCmd->dwType == ACTIVE_TECH_TYPE){
				CCountryTech::Instance()->SetActiveTechList(_pCmd);
			}
			else if( _pCmd && _pCmd->dwType == WAIT_TECH_TYPE ){
				CCountryTech::Instance()->SetWatingTechList(_pCmd);
			}
		}
		break;
	case CONFIRM_SEARCHER_PARA:
		{
			CCountryTech::Instance()->EchoKingSetResearcherEvent((const stConfirmSearcherUserCmd *)pCmd);
		}
		break;
	case ANSWER_COUNTRY_DARE_PARA:
		{
			GetGameGuiManager()->AddMessageBox("现在是国战时刻，你愿意参加国战吗?",eAction_TakeCountryWar,NULL,c_szMBOKCancel);
		}
		break;
	case DISENABLEL_OGIN_COUNTRY_PARA:
		{
			const stDisableLoginCountryCmd * pcmd = (const stDisableLoginCountryCmd *)pCmd;
			if( pcmd->status == FALSE_REGISTER_COUNTRY)
				GetGameGuiManager()->AddMessageBox("这个国家暂时不能注册！");
			else if (pcmd->status == FALSE_LOGIN_COUNTRY)
			{
				GetGameGuiManager()->AddMessageBox("这个国家暂时不能登陆！");
				if(GetCreateRoleManager()->m_guiRoleSel)
					GetCreateRoleManager()->m_guiRoleSel->SetEnabled(true);
			}
		}
		break;
	case ANTI_DARE_COUNTRY_FORMAL_PARA: // // 申请国战反攻
		{

		}
		break;
	case COUNTRY_NOTE_PARA:  // 国王发公告了
		{
			const stCountryNoteCmd * __pCmd = (const stCountryNoteCmd *)pCmd;
			CCountryConn::Instance()->AddKingAfficle(__pCmd);

		}
		break;
	case RET_GEN_COUNTRY_PARA: 
		{
			const stRetGenCountryCmd  * __pcmd = (const stRetGenCountryCmd  *)pCmd;
			CCountryConn::Instance()->EchoForGenInfo(__pcmd);
			if(GetGameGuiManager()->m_guiNpcDialog)
				GetGameGuiManager()->m_guiNpcDialog->ResetState();
		}
		break;
	case RTN_KING_LIST_PARA : //返回国王排行榜
		{
			const stRtnKingListCmd  * __pcmd = (const stRtnKingListCmd  * )pCmd;
			CCountryConn::Instance()->SetKingSortList(__pcmd);
		}
		break;
	case RTN_CITY_OWNER_LIST_PARA:
		{
			const stRtnCityOwnerListCmd * __pcmd = (const stRtnCityOwnerListCmd *)pCmd;
			CCountryConn::Instance()->SetCityOwnerList(__pcmd);
		}
		break;
	case RTN_NPC_OWNER_LIST_PARA:
		{
			const stRtnNpcOwnerListCmd  * __pcmd = (const stRtnNpcOwnerListCmd  *)pCmd;
			CCountryConn::Instance()->SetNpcOwnerList(__pcmd);
		}
		break;
	}

	return false;

	FUNCTION_END;
}


bool ParseQuizMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case CREATE_QUIZ_PARA:	// 智力问答
		{
			stCreateQuiz* pCmdInfo = (stCreateQuiz*) pCmd;
			if (pCmdInfo->byType==CREATE_QUESTION)
			{
				if(GetGameGuiManager()->m_guiNpcDialog)
				{
					GetGameGuiManager()->m_guiNpcDialog->m_pMsgDlg = (void*)GetGameGuiManager()->AddMessageBox(avar("进行智力问答需要花费%s银子，是否确定？",Money2String(pCmdInfo->dwMoney)),eAction_CompetitionNpc,NULL,c_szMBOKCancel);
					return true;
				}
			}
		}
		break;
	case QUERY_QUIZ_PARA:  // 全国智力竞赛
		{
			stQueryQuiz* pCmdInfo = (stQueryQuiz*) pCmd;

			switch(pCmdInfo->byQuizType) 
			{
			case 0:
				{
					GetGameGuiManager()->AddMessageBox("大家好！现在是智力竞赛报名时间，想要参加智力竞赛请在下面选择确定？",eAction_Competition,NULL,c_szMBOKCancel);
				}
				break;
			case 2:
				{
					GetGameGuiManager()->AddMessageBox("现在是《》为新手玩家专门准备的“全国智力竞赛——新手专场”，仅限20级以下的玩家参与，您是否要参加？",eAction_Competition,NULL,c_szMBOKCancel);
				}
				break;
			}
			return true;
		}
		break;

	case QUESTION_QUIZ_PARA: //题目
		{
			if (GetGameGuiManager()->m_guiCompetition)
				GetGameGuiManager()->m_guiCompetition->SetQuestion((stQuestionQuiz*)pCmd);
			return true;
		}
		break;

	case QUIZ_POS_LIST_PARA://玩家排名列表
		{
			if (GetGameGuiManager()->m_guiCompetition)
				GetGameGuiManager()->m_guiCompetition->SetQuizList((stQuizPosList*)pCmd);
			return true;
		}
		break;

	case QUIZ_PARAM_PARA: //开始竞赛
		{
			switch(((stQuizParam*)pCmd)->byType) {
	case QUIZ_WORLD:
		GetGameGuiManager()->AddCompetition(CGuiNpcDialog::eCompetition);
		break;
	case QUIZ_PERSONAL:
		GetGameGuiManager()->AddCompetition(CGuiNpcDialog::eCompetitionNpc);
		break;
			}

			if (GetGameGuiManager()->m_guiCompetition)
				GetGameGuiManager()->m_guiCompetition->SetQuizParam((stQuizParam*)pCmd);
			return true;
		}	
		break;

	case ANSWER_RETURN_QUIZ_PARA: //答题结果
		{
			if (GetGameGuiManager()->m_guiCompetition)
				GetGameGuiManager()->m_guiCompetition->SetResult((stAnswerReturnQuiz*)pCmd);
			return true;
		}
		break;

	case QUIZ_CUR_SCORE_PARA: //文采和经验
		{
			if (GetGameGuiManager()->m_guiCompetition)
				GetGameGuiManager()->m_guiCompetition->SetQuizCurScore((stQuizCurScore*)pCmd);
			return true;
		}
		break;

	case QUIZ_EXIT_PARA: //退出
		{
			if (GetGameGuiManager()->m_guiCompetition)
				GetGameGuiManager()->m_guiCompetition->QuitCompetition();
			return true;
		}
		break;
	default:
		break;
	}

	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParsePetMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;
	switch(pCmd->byParam)
	{
	case SETAI_PET_PARA:			//设置宠物的AI模式
		{
			stSetAIPetCmd *pcmd = (stSetAIPetCmd *)pCmd;
			CPet* pPet = GetScene()->FindPetByType( pcmd->type );
			if(pPet)
			{
				pPet->SetAI( pcmd->mode );
				return true;
			}
		}			
		break;
	case REFRESHDATA_PET_PARA:		//返回宠物的数据
		{
			stRefreshDataPetCmd *pcmd = (stRefreshDataPetCmd *)pCmd;
			CPet* pPet = GetScene()->AddPet( pcmd->type,pcmd->id );
			if(pPet) 
			{
				pPet->SetAttrib( pcmd->data );
				return true;
			}
		}
		break;
	case REQUESTDATA_PET_PARA:		//请求宠物的数据
	case CHANGENAME_PET_PARA:		//改宠物名字
		break;
	case ADDPET_PET_PARA:			//增加宠物  
		{
			stAddPetPetCmd* pcmd = (stAddPetPetCmd*)pCmd;
			GetScene()->AddPet( pcmd->type,pcmd->id );	
			return true;
		}
		break;
	case DELPET_PET_PARA:			//删除宠物 
		{
			GetScene()->RemovePet( (stDelPetPetCmd*)pCmd );
			return true;
		}
		break;
	case HORSEDATA_PET_PARA:		//马匹的数据
		{
			stHorseDataPetCmd* pcmd = (stHorseDataPetCmd*)pCmd;			
			CHorse* pHorse = static_cast<CHorse*>(GetScene()->AddPet( pcmd->type,pcmd->id ));
			if(pHorse)
			{
				pHorse->SetAttrib(pcmd->data);
				return true;
			}
		}
		break;
	case HP_EXP_PET_PARA:		//同步刷新替身宝宝的经验值
		{
			stHpExpPetCmd* pcmd = (stHpExpPetCmd*)pCmd;			
			CPet* pPet = GetScene()->FindPetByType( pcmd->type );
			if(pPet)
			{
				pPet->Update((stHpExpPetCmd*)pcmd);
				return true;
			}
		}
	default:
		return false;
		break;
	}

	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \param size : 描述
* \return 返回值的描述
*/
bool ParseTaskMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	CMainCharacter* pMainChar = GetScene()->GetMainCharacter();
	if( !pMainChar ) return false;

	CTask* ptask = pMainChar->GetTaskList();
	if( !ptask ) return false;

	DWORD dwTaskID = 0;
	switch(pCmd->byParam)
	{
	case RET_VALID_QUEST_PARA:
		{
			stRetValidQuestUserCmd	*p = (stRetValidQuestUserCmd*)pCmd;			
			if(GetGameGuiManager()->m_guitaskdialog)
			{
				TRACE(p->content);
				GetGameGuiManager()->m_guitaskdialog->UpdateTaskKing(p->content);
			}
		}
		break;
	case QUEST_INFO_PARA:
		{
			stQuestInfoUserCmd	*p = (stQuestInfoUserCmd	*)pCmd;			
			ptask->AddQuestVars(p);
			dwTaskID = p->id;
		}
		break;
	case QUEST_VARS_PARA:
		{
			stQuestVarsUserCmd	*p = (stQuestVarsUserCmd	*)pCmd;
			ptask->UpdateQuestVars(p);
		}		
		break;
	case ABANDON_QUEST_PARA:
		{
			stAbandonQuestUserCmd	*p = (stAbandonQuestUserCmd	*)pCmd;
			ptask->RemoveTask(p->id);
		}
		break;
	default:
		return false;
		break;
	}
	if(GetGameGuiManager()->m_guitaskdialog)
	{
		GetGameGuiManager()->m_guitaskdialog->UpdateTaskContent();
		if( dwTaskID != 0 ) 
		{
			GetGameGuiManager()->m_guitaskdialog->ShowLastTask(dwTaskID);
			return true;
		}
	}
	return true;

	FUNCTION_END;
}

bool ParseMailMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case ADD_LIST_MAIL_PARA:
		{
			stAddListMail* p = (stAddListMail*) pCmd;
			GetScene()->GetMainCharacter()->GetMailManager()->ParseMailTitle(p);
			return true;
		}
	case CONTENT_MAIL_PARA:
		{
			stContentMail* p = (stContentMail*) pCmd;
			GetScene()->GetMainCharacter()->GetMailManager()->ParseMailContent(p);
			return true;
		}
	case NOTIFY_NEW_MAIL_PARA:
		{
			//stNotifyNewMail* p = (stNotifyNewMail*) pCmd;
			GetScene()->GetMainCharacter()->GetMailManager()->SetNewMailNotify();
			return true;
		}
		break;
	case DEL_MAIL_PARA:
		{
			stDelMail* p = (stDelMail*) pCmd;
			if(NULL != p)
			{
				GetScene()->GetMainCharacter()->GetMailManager()->OnRemoveMail(p->mailID);
			}			
			return true;
		}
		break;
	case REMOVE_ITEM_MAIL_PARA:
		{
			stRemoveItemMail* p = (stRemoveItemMail*) pCmd;
			if(NULL != p)
			{
				GetScene()->GetMainCharacter()->GetMailManager()->DeleteMailItem(p->mailID);
			}
			return true;
		}
	default:
		break;
	}

	return false;
	FUNCTION_END;
}


bool ParseAuctionMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case ADD_LIST_AUCTION_PARA:
		{
			stAddListAuction* cmd = (stAddListAuction*)(pCmd);
			GetScene()->GetAuction()->ParseAddListAuctionCmd(cmd);

			return true;
		}
		break;
	case REMOVE_LIST_AUCTION_PARA:
		{
			stRemoveListAuction* cmd = (stRemoveListAuction*)(pCmd);
			GetScene()->GetAuction()->ParseRemoveListAuctionCmd(cmd);
			return true;
		}
		break;
	}

	return false;
	FUNCTION_END;
}

inline bool ParseGoldMessageRetCode(BYTE byReturn,std::string &strRet)
{
	char cRet[4][20] = {"兑换成功 ","服务器忙","兑换系统停止服务","失败"};
	switch( byReturn )
	{
	case REDEEM_SUCCESS:
		strRet = cRet[REDEEM_SUCCESS];
		return true;
	case REDEEM_BUSY:
		strRet = cRet[REDEEM_BUSY];
		return false;
	case REDEEM_STOP:
		strRet = cRet[REDEEM_STOP];
		return false;
	case REDEEM_FAIL:
		strRet = cRet[REDEEM_FAIL];
		return false;
	default:
		strRet = cRet[REDEEM_FAIL];
		return false;
	}
	return false;
}

bool ParseGoldMessage(stNullUserCmd* pCmd,size_t size)
{
	CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
	if( !pMainRole )
	{
		return false;
	}
	std::string strRet;
	switch(pCmd->byParam)
	{
	case REDEEM_GOLD_PARA:	//兑换金子返回
		{
			stRedeemGold* cmd = (stRedeemGold*)(pCmd);			
			bool bRes = ParseGoldMessageRetCode( cmd->byReturn,strRet );
			if( bRes )
			{
				pMainRole->SetPointNum( cmd->dwBalance );
				pMainRole->SetGoldNum( cmd->dwNum );
				//Debug_Int(cmd->dwBalance);
				//Debug_Int(cmd->dwNum);
			}
			else
			{
				GetGameGuiManager()->AddClientChatMessage(strRet.c_str());
			}
			return true;
		}
		break;
	case REDEEM_MONTH_CARD_PARA:	//兑换月卡返回
		{
			stRedeemMonthCard* cmd = (stRedeemMonthCard*)(pCmd);			
			bool bRes = ParseGoldMessageRetCode( cmd->byReturn,strRet );
			if( bRes )
			{
				pMainRole->SetPointNum( cmd->dwBalance );
				//Debug_Int(cmd->dwBalance);
			}
			else
			{
				GetGameGuiManager()->AddClientChatMessage(strRet.c_str());
			}
			return true;
		}			
		break;
	case RETURN_CARD_AND_GOLD_PARA:	//查询返回(月卡、金子)
		{
			stReturnCardAndGold* cmd = (stReturnCardAndGold*)(pCmd);			
			bool bRes = ParseGoldMessageRetCode( cmd->byReturn,strRet );
			if( bRes )
			{
				pMainRole->SetGoldNum( cmd->dwGold );
				pMainRole->SetMonthCard( cmd->dwMonthCard );
			}
			else
			{
				GetGameGuiManager()->AddClientChatMessage(strRet.c_str());
			}
			return true;
		}			
		break;
	case RETURN_REQUEST_POINT_PARA:	//查询点卡返回
		{
			stReturnRequestPoint* cmd = (stReturnRequestPoint*)(pCmd);			
			bool bRes = ParseGoldMessageRetCode( cmd->byReturn,strRet );
			if( bRes )
			{
				pMainRole->SetPointNum( cmd->dwPoint );
				if ( GetGameGuiManager()->m_guiGoldExchangeDlg )
				{
					GetGameGuiManager()->m_guiGoldExchangeDlg->UpdatePointNum();
				}
			}
			else
			{
				GetGameGuiManager()->AddClientChatMessage(strRet.c_str());
			}
			return true;
		}			
		break;
	case STOP_SERVICE_GOLD_PARA:
		{
			GetGameGuiManager()->AddMessageBox("金子系统停止服务.");
			return true;
		}
		break;
	case RETURN_CONSUME_CARD_PARA:
		{
			stReturnConSumeCardCard* cmd = (stReturnConSumeCardCard*)pCmd;
			switch( cmd->byReturn )
			{
			case REDEEM_SUCCESS:
				GetGameGuiManager()->AddMessageBox( "兑换成功" );
				break;
			case REDEEM_BUSY:
				GetGameGuiManager()->AddMessageBox( "服务器忙" );
				break;
			case REDEEM_STOP:
				GetGameGuiManager()->AddMessageBox( "兑换系统停止服务" );
				break;
			case REDEEM_FAIL:
				GetGameGuiManager()->AddMessageBox( "兑换失败" );
				break;
			}

			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool ParseCartoonMessage(stNullUserCmd* pCmd,size_t size)
{
	CCartoonPetList* pCartoonList = GetScene()->GetCartoonPetList();
	CWaitingCartoonPetList* pWaitCartoonList = GetScene()->GetWaitCartoonPetList();
	switch( pCmd->byParam )
	{
	case ADD_CARTOON_PARA:
		{
			pCartoonList->AddCartoon( (stAddCartoonCmd*)pCmd );
			return true;
		}
		break;
	case REMOVE_CARTOON_PARA:
		{
			pCartoonList->RemoveCartoon( (stRemoveCartoonCmd*)pCmd );
			return true;
		}
		break;
	case ADD_WAITING_CARTOON_PARA:
		{
			pWaitCartoonList->AddCartoonPet( (stAddWaitingCartoonCmd*)pCmd );
			return true;
		}
		break;
	case REMOVE_WAITING_CARTOON_PARA:
		{
			pWaitCartoonList->RemoveCartoonPet( (stRemoveWaitingCartoonCmd*)pCmd );
			return true;
		}
		break;
	case HP_EXP_CARTOON_PARA:
		{
			pCartoonList->UpdateCartoonPet( (stHpExpCartoonCmd*)pCmd );
			return true;
		}
		break;
	case SELL_ALL_CARTOON_PARA:
		{
			GetScene()->GetMainCharacter()->RemoveItemByLocationType(OBJECTCELLTYPE_PET);
			return true;
		}
		break;
	case CONSIGN_CARTOON_PARA:
		{
			stConsignCartoonCmd* cmd = (stConsignCartoonCmd*)(pCmd);
			char szTemp[128];
			_snprintf(szTemp,128,"您的好友 %s 请求您领养替身宝宝",cmd->name);
			szTemp[127] = 0;
			GetGameGuiManager()->AddMessageBox(szTemp,eAction_ConsignCartoon,(void*)(cmd->cartoonID),"接受|1|Enter|拒绝|2|Esc");			
		}
	default:
		break;
	}
	return false;
}

bool ParseVoteMessage(stNullUserCmd* pCmd,size_t size)
{
	switch(pCmd->byParam)
	{
	case RETURN_VOTE_LIST_PARA:	//返回投票列表
		{
			const stReturnVoteListUserCmd* cmd = (const stReturnVoteListUserCmd*)pCmd;
			CCountryTech::Instance()->SetVoteList(cmd);
			return true;
		}
		break;
	}
	return false;
}

bool ParseArmyMessage(stNullUserCmd * pCmd,size_t size)
{
	FUNCTION_BEGIN;
	switch(pCmd->byParam)
	{
	case REQ_WAIT_GEN_PARA:  //  侯选将军列表
		{
			const stRtnWaitGenUserCmd* cmd = (const stRtnWaitGenUserCmd*)pCmd;
			CArmyManager::GetSingleton()->SetWaitGenList(cmd); 
		}
		break;
	case REQ_ARMY_GEN_PARA : //询问是否同意成为将军
		{
			GetGameGuiManager()->AddMessageBox("您是否同意成为将军?",eAction_CreateGen,NULL,"确定|1|Enter|取消|2|Esc");
		}
		break;
	case RTN_ARMY_LIST_PARA: // 返回军队列表
		{
			const stRtnArmyListUserCmd* cmd = (const stRtnArmyListUserCmd*)pCmd;

			if( cmd->byType == COUNTRY_ARMY_LIST)
			{
				CCountryConn::Instance()->SetCountryArmyList(cmd);

			}
			else if(cmd->byType == CITY_ARMY_LIST)
			{
				CArmyManager::GetSingleton()->SetCityArmyList(cmd);
			}
		}
		break;
	case RTN_ARMY_SPEC_PARA: // 返回军队详细信息
		{
			const stRtnArmySpecUserCmd * cmd = (const stRtnArmySpecUserCmd *)pCmd;
			//	CArmyManager::GetSingleton()->SetCityArmyItem(cmd);
			CCountryConn::Instance()->SetCountryArmyItem(cmd);
		}
		break;
	case ADD_ARMY_CAPTAIN_PARA: 
		{
			const stAddArmyCaptainUserCmd * __pCmd = (const stAddArmyCaptainUserCmd *)pCmd;

			CArmyManager::GetSingleton()->CopyAddArmyCaptionUserCmd(__pCmd);

			char szTip[100];
			_snprintf(szTip,100,"招收你为%s军队的成员，你是否愿意?",__pCmd->armyName);
			szTip[99] = 0;
			GetGameGuiManager()->AddMessageBox(szTip,eAction_RecruitArmy,NULL,c_szMBOKCancel);
		}
		break;
	}
	FUNCTION_END;
	return false;
}

bool ParseStockSceneMessage(stNullUserCmd* pCmd,size_t size)
{
	switch(pCmd->byParam)
	{
	case 1:
		break;
	case 2:
		break;
	}
	return false;
}

bool ParseStockBillMessage(stNullUserCmd* pCmd,size_t size)
{
	static char szStockReturn[][MAX_PATH] =
	{
		{"登陆成功！"},
		{"请先登陆股票系统！"},
		{"启用帐号成功！"},
		{"更改密码成功！"},
		{"密码错误！"},
		{"两次输入新密码不一致！"},
		{"密码不能为空！"},
		{"密码太短！"},
		{"帐号已经存在！"},
		{"服务器错误！"},
		{"卖单不能超过10条！"},
		{"买单不能超过10条！"}
	};

	switch(pCmd->byParam)
	{
	case RETUEN_PASSWORD_STOCKPARA:
		{
			stReturnPasswordStockIserCmd* pCmdInfo = (stReturnPasswordStockIserCmd*)(pCmd);
			if(pCmdInfo->byReturn != STOCK_LOGIN_OK)
			{
				GameMessageBox(szStockReturn[pCmdInfo->byReturn]);
			}		
			switch(pCmdInfo->byReturn) 
			{
			case STOCK_LOGIN_OK:
				GetStockAccount()->SetLogin(true);
				break;
			case STOCK_OPEN_OK:
				GetStockAccount()->SetActive(true);
				break;
			}
			return true;
		}
		break;
	case RETURN_FUND_STOCKPARA:
		{
			stReturnFundStockUserCmd* pCmdInfo = (stReturnFundStockUserCmd*)(pCmd);

			GetStockAccount()->SetCountFinance(pCmdInfo->dwMoney,pCmdInfo->dwGold);
			return true;
		}
		break;
	case CONSIGN_CLEAN_LIST_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				GetGameGuiManager()->m_guiStockDlg->CleanConsignBill();
				return true;
			}
		}
		break;
	case RETURN_CANCEL_LIST_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				stReturnCancelListStockUserCmd* pCmdInfo = (stReturnCancelListStockUserCmd*)(pCmd);

				if (pCmdInfo->byReturn==STOCK_CANCEL_OK)
				{
					GameMessageBox("撤单成功！");
					GetGameGuiManager()->m_guiStockDlg->CancelStock(pCmdInfo->id,(StockType)pCmdInfo->byType);
				}
				else
					GameMessageBox("撤单失败！");
				return true;
			}
		}
		break;
	case CONSIGN_GOLD_LIST_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				stConsignGoldListStockUserCmd* pCmdInfo = (stConsignGoldListStockUserCmd*)(pCmd);
				GetGameGuiManager()->m_guiStockDlg->ParseConsignBillCmd(pCmdInfo->size,pCmdInfo->list,STOCK_GOLD);
				return true;
			}
		}
		break;
	case CONSIGN_MONEY_LIST_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				stConsignMoneyListStockUserCmd* pCmdInfo = (stConsignMoneyListStockUserCmd*)(pCmd);
				GetGameGuiManager()->m_guiStockDlg->ParseConsignBillCmd(pCmdInfo->size,pCmdInfo->list,STOCK_MONEY);		
				return true;	
			}
		}
		break;
	case SELF_HISTORY_GOLD_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				stSelfHistoryGoldStockUserCmd* pCmdInfo = (stSelfHistoryGoldStockUserCmd*)(pCmd);
				GetGameGuiManager()->m_guiStockDlg->ParseSelfHistoryCmd(pCmdInfo->size,pCmdInfo->list,STOCK_GOLD);
				return true;
			}
		}
		break;
	case SELF_HISTORY_MONEY_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				stSelfHistoryMoneyStockUserCmd* pCmdInfo = (stSelfHistoryMoneyStockUserCmd*)(pCmd);
				GetGameGuiManager()->m_guiStockDlg->ParseSelfHistoryCmd(pCmdInfo->size,pCmdInfo->list,STOCK_MONEY);
				return true;
			}
		}
		break;
	case FIRSTFIVE_LIST_STOCKPARA:
		{
			if (GetGameGuiManager()->m_guiStockDlg)
			{
				GetGameGuiManager()->m_guiStockDlg->ParseFirst10Cmd((stFirstFiveListStockUserCmd*)(pCmd));
				return true;
			}
		}
		break;
	case REQUEST_STATE_STOCKPARA:
		{
			stRequestStateStockUserCmd* pCmdInfo = (stRequestStateStockUserCmd*)(pCmd);
			GetStockAccount()->SetActive(pCmdInfo->active);
			GetStockAccount()->SetLogin(pCmdInfo->login);
			return true;
		}
		break;
	}
	return false;
}

bool ParseGiftMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case LIST_GIFT_PARA:		//发送活动列表
		{
			stListGift* p = (stListGift*)pCmd;
			CGuiNpcDialog* pDlg = GetGameGuiManager()->AddNpcDialog();
			if( pDlg )
			{
				pDlg->AddGiftListByCmd(p);
				return true;
			}			
		}		
		break;
	case DETAIL_GIFT_PARA:		//发送活动内容
		{
			stDetailGift* p = (stDetailGift*)pCmd;
			CGuiNpcDialog* pDlg = GetGameGuiManager()->AddNpcDialog();
			if( pDlg )
			{
				pDlg->AddGiftDetailByCmd(p);
				return true;
			}
		}
		break;
	case WINNER_GIFT_PARA:
		{
			stWinnerGift* p = (stWinnerGift*)pCmd;
			CGuiNpcDialog* pDlg = GetGameGuiManager()->AddNpcDialog();
			if( pDlg )
			{
				pDlg->AddWinnerListByCmd(p);
				return true;
			}
		}
		break;
	}
	return false;
	FUNCTION_END;
}



bool ParseRecoSysMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case RTN_RECOMMEND_BOUNTY_PARA:// 返回推荐人奖励
		{
			TRACE("@@@@@@---返回推荐人奖励---wyc");
			stRtnRecommendBountyUserCmd* p = (stRtnRecommendBountyUserCmd*)pCmd;
			GetGameGuiManager()->AddRecoSys_A_Query_TakeDlg()->OnRefreshUIData(p);
		}
		break;
	case RTN_RECOMMENDED_BOUNTY_PARA:// 返回被推荐人奖励
		{
			TRACE("@@@@@@---返回[被]推荐人奖励---wyc");
			stRtnRecommendedBountyUserCmd* p = (stRtnRecommendedBountyUserCmd*)pCmd;
			GetGameGuiManager()->AddRecoSys_B_Query_TakeDlg()->OnRefreshUIData(p);
		}
		break;
	}
	return false;
	FUNCTION_END;
}


extern bool ParseLordLobbyMessage(stCommonMiniGameCmd* pCmd,size_t size);
extern bool ParseLordGameMessage(stCardMiniGameCmd *pcmd,size_t size);
bool ParseMiniGameMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case INVITE_MINI_PARA:
		{	
			TRACE("---mini Game-- 收到邀请消息--");
			static stInviteMiniGame p ;
			memcpy(&p,pCmd,sizeof(stInviteMiniGame));
			char temp[256] = "";
			if (GetScene()->FindCharacterByID(p.tempid))
			{
				sprintf(temp,"%s邀请您玩比数字游戏,投注额为%s,您同意吗?",
					GetScene()->FindCharacterByID(p.tempid)->GetName(),
					Money2String(p.money));
				GetGameGuiManager()->AddMessageBox(temp,eAction_MiniGame_Invite,(void*)(&p),c_szMBOKCancel);
			}
			else
			{	

			}
		}
		break;
	case DICE_NUM_MINI_PARA:
		{		
			TRACE("---mini Game-- 收到Dice num消息--");
			stDiceNumMiniGame* p = (stDiceNumMiniGame*)pCmd;

			if (GetGameGuiManager()->m_MGNumberDlg)
			{
				GetGameGuiManager()->m_MGNumberDlg->SetDiceNum(p->tempid,p->num);
			}

		}
		break;
	case RESULT_MINI_PARA:
		{		
			TRACE("---mini Game-- 收到结果消息--");
			stResultMiniGame* p = (stResultMiniGame*)pCmd;
			if (GetGameGuiManager()->m_MGNumberDlg)
			{
				GetGameGuiManager()->m_MGNumberDlg->onResult(p->res);
			}
		}
		break;
	case END_MINI_PARA:
		{		
			TRACE("---mini Game-- 收到结束消息--");
			stEndMiniGame* p = (stEndMiniGame*)pCmd;
			if (p->reason == 0)
			{
				if (GetGameGuiManager()->m_MGNumberDlg)
				{					
					GetGameGuiManager()->AddMessageBox("本轮游戏结束!",eAction_MiniGame_GameOver);
				}	
			}
			if (p->reason == 1)
			{
				if (GetGameGuiManager()->m_MGNumberDlg)
				{					
					GetGameGuiManager()->AddMessageBox("一方主动离开或掉线,游戏结束!",eAction_MiniGame_GameOver);
				}
			}

		}
		break;
	case START_MINI_PARA:
		{	
			TRACE("---mini Game-- 收到show游戏窗口消息--");
			if (!GetGameGuiManager()->m_MGNumberDlg)
			{
				static stStartMiniGame p;
				memcpy(&p,pCmd,sizeof(stStartMiniGame));			
				GetGameGuiManager()->AddMGNumberDlg(eAction_MiniGame_Start,(void*)(&p));
			}
			else
			{
				TRACE("---mini Game-- 收到重新开始消息--");
				stStartMiniGame* p = (stStartMiniGame*)pCmd;
				GetGameGuiManager()->m_MGNumberDlg->Restart(p->money);
			}

		}
		break;
	case ENTER_MINI_PARA:
		{	
			TRACE("---mini Game-- 收到游戏开始消息--");
			GetGameGuiManager()->m_MGNumberDlg->OnStart();
		}
		break;
	case COMMON_MINI_PARA:
		{
			stCommonMiniGameCmd * __Pcmd = (stCommonMiniGameCmd *)pCmd;
			return ParseLordLobbyMessage(__Pcmd,size);

		}
		break;
	case CARD_MINI_PARA:
		{
			stCardMiniGameCmd * __pcmd = (stCardMiniGameCmd *)pCmd;
			return ParseLordGameMessage(__pcmd,size);
		}
		break;
	}
	return false;
	FUNCTION_END;
}


bool ParseALLYMessage(stNullUserCmd* pCmd,size_t size)
{
	FUNCTION_BEGIN;

	switch(pCmd->byParam)
	{
	case REQ_COUNTRY_ALLY_PARA:		 
		{
			assert(GetScene()->GetMainCharacter()->IsKing());
			if(!GetScene()->GetMainCharacter()->IsKing())
				return true;
			const stReqCountryAlly  * lpcmd = (const stReqCountryAlly*)pCmd;
			if( !lpcmd )
				return true;
			if(lpcmd->dwAllyCountryID != GetScene()->GetMainCharacter()->GetCountry())
				return true;
			if( !GetScene()->GetMainCharacter()->IsKing())
				return true;

			char szText[120] = {0};
			char szSendCountryName[40];
			if( !CCountry::Instance()->GetCountryName(lpcmd->dwCountryID,szSendCountryName))
			{
				return true;
			}
			szSendCountryName[39] = 0;
			_snprintf(szText,sizeof(szText),"%s欲与我国建立联盟，是否同意？",szSendCountryName);
			szText[119] = 0;

			GetGameGuiManager()->AddMessageBox(szText,eAction_ConfirmAllyCountry,(void*)lpcmd->dwCountryID,c_szMBOKCancel);
		}
		break;
	case UPDATE_COUNTRY_ALLY_PARA:
		{
			CCountry::Instance()->SetAllyCountry((const stUpdateCountryAlly *)pCmd);
		}
		break;
	case RTN_COUNTRY_ALLY_INFO_PARA:
		{
			CCountry::Instance()->SetAllyCountryInfo((const stRtnCountryAllyInfo *)pCmd);
			if(GetGameGuiManager()->m_guiNpcDialog)
				GetGameGuiManager()->m_guiNpcDialog->RefreshAllyCountryInfo();
		}
	}
	return false;
	FUNCTION_END;
}


 
void HandleCommand()
{
	for(;;)
	{
		//触发皇城争夺战提示
		/*if ( !GetScene()->m_IsHaveSendRoralCityWarMsg )
		{
		if ( GetScene()->IsRoralCityWarTime() )
		{
		if ( g_dwGameStartTime != 0 && xtimeGetTime() - g_dwGameStartTime > 20000 )
		{
		GetGameGuiManager()->AddClientTaskMessage( "现在正在进行皇城争夺战，请去参战" );

		GetScene()->m_IsHaveSendRoralCityWarMsg = true;
		}
		}			
		}*/


		//触发"第一次进入游戏2分钟"
		if(bIsUserUnSetted(USER_SETTING_HELP_NPC))
		{
			if(g_dwGameStartTime != 0 && xtimeGetTime() - g_dwGameStartTime > 120000)
				ShowHelpTip(USER_SETTING_HELP_NPC);
		}

		//触发"道具升级"
		if(bIsUserUnSetted(USER_SETTING_HELP_CHAT))
		{
			if(g_dwGameStartTime != 0 && xtimeGetTime() - g_dwGameStartTime > 300000)
				ShowHelpTip(USER_SETTING_HELP_CHAT);
		}				

		DWORD dwMark = FrameAllocator::getWaterMark();
		std::vector<BYTE ,std::stack_allocator<BYTE> > vecMsg;

		bool bHaveMsg;

		if(bHaveMsg = GetClient()->PopMsg(vecMsg))
		{
			CHECK_RET_ADDR

				bool bParase;
			bParase = false;
			stNullUserCmd* pCmd = (stNullUserCmd*)&vecMsg[0];
			size_t size = vecMsg.capacity();
			TRACE("收到指令： %u:%u\n",pCmd->byCmd,pCmd->byParam);
			switch(pCmd->byCmdType)
			{
				/// 空指令
			case NULL_USERCMD:
				break;
				/// 推荐人系统指令
			case RECOMMEND_USERCMD:
				bParase = ParseRecoSysMessage(pCmd,size);
				break;
				/// 小游戏指令
			case MINIGAME_USERCMD:
				bParase = ParseMiniGameMessage(pCmd,size);
				break;
				/// 登陆指令
			case LOGON_USERCMD:
				bParase = ParseLoginMessage(pCmd,size);
				break;
			case SELECT_USERCMD:	// 选择角色指令
				bParase = ParseSelectMessage(pCmd,size);
				break;	
				/// 数据指令
			case DATA_USERCMD:
				bParase = ParseDataMessage(pCmd,size);
				break;
				/// 道具指令（用户物品、装备、技能、属性等）
			case PROPERTY_USERCMD:
				bParase = ParsePropertyMessage(pCmd,size);
				break;
				/// 地图指令
			case MAPSCREEN_USERCMD:
				bParase = ParseMapScreenMessage(pCmd,size);
				break;
				/// 移动指令
			case MOVE_USERCMD:
				//bParase = ParseMoveMessage(pCmd,size);
				switch(pCmd->byParameterType) 
				{
				case NPCMOVE_MOVE_USERCMD_PARA:
					{
						CNpc* pNpc = GetScene()->FindNpcByID(((stNpcMoveMoveUserCmd*)pCmd)->dwNpcTempID);
						if(pNpc) 
							bParase = pNpc->PushCommand(pCmd,size);
						else
							Engine_WriteLogF("[NPCMOVE_MOVE_USERCMD_PARA] 未找到NPC(%d)\n", ((stNpcMoveMoveUserCmd*)pCmd)->dwNpcTempID);
					}
					break;
				case USERMOVE_MOVE_USERCMD_PARA:
				case USERINSTANTJUMP_MOVE_USERCMD_PARA:
					{
						CCharacter* pCharacter = GetScene()->FindCharacterByID(((stUserMoveMoveUserCmd*)pCmd)->dwUserTempID);
						if(pCharacter) 
							bParase = pCharacter->PushCommand(pCmd,size);
						else
							Engine_WriteLogF("[%s] 未找到USER(%d)\n", 
							pCmd->byParam == USERINSTANTJUMP_MOVE_USERCMD_PARA ? "USERINSTANTJUMP_MOVE_USERCMD_PARA" : "USERINSTANTJUMP_MOVE_USERCMD_PARA",
							((stUserMoveMoveUserCmd*)pCmd)->dwUserTempID);
					}
					break;
				case RESPONSELANDMASS_MOVE_USERCMD_PARA:
					break;
				case LANDMASSJUMP_MOVE_USERCMD_PARA:
					break;
				}
				break;
				/// 建造指令
			case BUILD_USERCMD:
				break;
			case MAKEOBJECT_USERCMD:			/// 打造指令
				break;
			case RELIVE_USERCMD:
				bParase = ParseReliveMessage(pCmd,size);			/// 复活指令
				break;
			case TIME_USERCMD:			/// 时间指令
				bParase = ParseTimerMessage(pCmd,size);
				break;
				/// 聊天指令
			case CHAT_USERCMD:
				bParase = ParseChatMessage(pCmd,size);
				break;
				/// 离开指令
			case LEAVEONLINE_USERCMD:
				break;
				/// 交易指令
			case TRADE_USERCMD:
				bParase = ParseTradeMessage(pCmd,size);
				break;
				/// 魔法指令
			case MAGIC_USERCMD:
				bParase = ParseMagicMessage(pCmd,size);
				break;
				/// 行会指令
			case UNION_USERCMD:
				/// 社会关系
			case RELATION_USERCMD:
				/// 门派关系
			case SCHOOL_USERCMD:
				//家族关系
			case SEPT_USERCMD:
				bParase = GetGameGuiManager()->m_guiWorldConn->ParseMessage(pCmd,size);
				break;			
			case COUNTRY_USERCMD:		/// 国家指令
				bParase = ParseCountryMessage(pCmd,size);
				break;
			case TASK_USERCMD:			/// 任务指令
				bParase = ParseTaskMessage(pCmd,size);
				break;
			case PET_USERCMD://宠物指令
				bParase = ParsePetMessage(pCmd,size);
				break;
			case DARE_USERCMD://帮战斗，族战斗，派战斗
				bParase = ParseDareMessage(pCmd,size);
				break;
			case QUIZ_USERCMD://智力竞赛
				bParase = ParseQuizMessage(pCmd,size);
				break;
			case MAIL_USERCMD://邮件系统
				bParase = ParseMailMessage(pCmd,size);
				break;
			case AUCTION_USERCMD: //拍卖
				bParase = ParseAuctionMessage(pCmd,size);
				break;
			case GOLD_USERCMD:	//金子
				bParase = ParseGoldMessage(pCmd,size);
				break;
			case CARTOON_USERCMD:	//替身宠物
				bParase = ParseCartoonMessage(pCmd,size);
				break;
			case STOCK_SCENE_USERCMD: //股票
				bParase = ParseStockSceneMessage(pCmd,size);
				break;	
			case STOCK_BILL_USERCMD:  //股票
				bParase = ParseStockBillMessage(pCmd,size);
				break;
			case VOTE_USERCMD:
				bParase = ParseVoteMessage(pCmd,size);
				break;
			case ARMY_USERCMD: //军队
				bParase = ParseArmyMessage(pCmd,size);
				break;
			case GIFT_USERCMD:
				bParase = ParseGiftMessage(pCmd,size);
				break;
			case ALLY_USERCMD:
				bParase = ParseALLYMessage(pCmd,size);
				break;
			case SAFETY_USERCMD:
				{
					extern bool ParseSafetyMessage(stNullUserCmd* pCmd,size_t size);
					bParase = ParseSafetyMessage(pCmd,size);
				}
				break;
			}
		}

		FrameAllocator::setWaterMark(dwMark);

		if(!bHaveMsg) break;
	}
}