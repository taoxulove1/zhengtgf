/**
*\file		GuiMain.cpp
*\version	$Id$
*\author	$$$02公司
*\			$$$02公司@###.com
*\date		2005-7-5   17:14:00 中国标准时间
*\brief	游戏主界面
*
* 
*
*/

#include "public.h"
#include "../gui/include/guiTypes.h"
#include ".\guimain.h"
#include ".\gameguimanager.h"
#include "./GameAppation.h"
#include "Game.h"
#include "./GameCursor.h"
#include "game_script.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./GameState.h"
#include "./GuiPrivateChat.h"
#include "./GuiUserInfo.h"
#include "./GuiChatOutput.h"
#include "./GuiNpcDialog.h"
#include "./GuiTrade.h"
#include "./GuiSkill.h"
#include "./GuiFloatAccessKey.h"
#include "./RoleItem.h"
#include "./GuiMiniMap.h"
#include "./GameGuiFloatItem.h"
#include "./GuiPrivateChat.h"
#include "./GuiChatInvite.h"
#include "./GuiShortcutItem.h"
#include "./GuiFloatControl.h"
#include "./GuiChatSelect.h"
#include "./GuiPetDialog.h"
#include "./GuiItemDetailInfo.h"
#include "./GuiFace.h"
#include "./GuiWorldConn.h"
#include "./CharacterProperty.h"
#include "./GameAccelKeyManager.h"
#include "../Engine/include/stack_alloc.h"
#include "../Engine/include/stringex.h"
#include "./GuiTeam.h"
#include "./GuiPrivateChatSelect.h"
#include "./GuiSysChat.h"
#include "./GuiEmotionDialog.h"
#include "./GuiSystem.h"
#include "./SpriteObj.h"
#include "./GuiAdoptCartoonDialog.h"
#include "../luabind/public.h"
#include "./Chat.h"
#include "./GuiPetCtrlDialog.h"
#include "./GuiChatSelectButton.h"

#ifdef _DEBUG
#include "Country.h"
#include "../Media/SoundManager.h"
#endif 
#ifdef _DEBUG
#include ".\GuiStockDialog.h"
#include "./CartoonPet.h"
#include ".\GuiAddMemberDialog.h"
#include "../engine/include/Spell.h"
#endif

#if defined _DEBUG && defined __SPRING__
#include ".\GuiInputBox2Dialog.h"
#endif

#include "MiniUserCommand.h"
#include "LordStrikeManager.h"
#include "LordStrikeLobby.h"


#define IDC_HELP_TIP_BTN	10000
const int timeHelpButtonShown = 15000;

#define IDC_CHATTYPE_BTN	25
struct stChatTypeInfo{
	enumChatType chatType;
	int			 imgFrame;
};

#define TIRE_COLOR_STEP		4
//#define START_TIRE_VALUE	96
//#define MAX_TIRE_VALUE		287
#define START_TIRE_VALUE	12*60/5
#define MAX_TIRE_VALUE		17*60/5

#define IDC_PRIVATECHAT_BTN_BEG   5000
#define IDC_PRIVATECHAT_BTN_END   6000

static stChatTypeInfo g_chatTypeInfo[] = {
	{CHAT_TYPE_PRIVATE	,-1	},	/// 私聊频道
	{CHAT_TYPE_NINE		,3	},	/// 轻聊频道
	{CHAT_TYPE_TEAM		,12	},	/// 队伍频道
	{CHAT_TYPE_FRIEND	,15	},	/// 好友频道
	{CHAT_TYPE_GM		,0	},	/// GM聊频道
	{CHAT_TYPE_SYSTEM	,0	},	/// 系统频道
	{CHAT_TYPE_UNION	,9	},	/// 行会频道
	{CHAT_TYPE_POP		,0	},	/// 弹出式系
	{CHAT_TYPE_PERSON	,0	},	/// 个人频道
	{CHAT_TYPE_COUNTRY	,18	},	/// 国家频道
	{CHAT_TYPE_OVERMAN	,21	},	/// 师门频道
	{CHAT_TYPE_FAMILY	,24	},	/// 家族频道
};

DWORD dwAlpha_d = 255 *0.85;

static char* pkModeName[] = { "和平模式(Tab)", "全体模式(Tab)", "组队模式(Tab)", "帮会模式(Tab)", "家族模式(Tab)", "国家模式(Tab)" };

DWORD dwBtnState[] = { GUI_STATE_NORMAL,GUI_STATE_PRESSED,GUI_STATE_DISABLED };

enum enmCartCMD
{
	CartCMD_Play	= 0,
	CartCMD_Stop,
	CartCMD_Num
};

//////////////////////////////////////////////////
// access key btn
int g_nMainAccessBtnLeft = 21;
int g_nMainAccessBtnTop = 728;	
int g_MainAccessBtnSpace = 39;
///////////////////////////////////////////////////


//////////////////////////////////////////////////
// team flag
stPointI g_ptTeamHead;
stPointI g_ptExpAssgin;   // position of bmp,for draw and tip and fuck!!!
stPointI g_ptObjAssgin;
//////////////////////////////////////////////////

/**
* \brief 构造函数
* 
*	
* 
* \return 
*/

struct szCmdItem
{
	enum enmTEAMCMD
	{
		CMD_LEAVE,
		CMD_LINE1,  // sperative line 
	        CMD_ASSING_EXP_BY_LEVLE,          // 1、	等级分配
		CMD_ASSING_EXP_BY_SPECIAL,        // 2、	特殊分配
		CMD_LINE2,  // sperative line 
		CMD_PICKUP_ITEM_FREEDOM,          // 1、	自由拾取
		CMD_PICKUP_ITEM_POLL,             // 2、	轮流拾取 
		CMD_LINE3,  // sperative line 
		CMD_UNCOMBIN,
		CMD_UNFACE,
		CMD_UN_TEAM_ATTACK,
		
	};
	enmTEAMCMD  cmd;
	std::string strCmdName;
	bool        bCanUse;
};

static szCmdItem s_CmdItem[] = 
{
	{szCmdItem::CMD_LEAVE,"离开队伍",true},
	{szCmdItem::CMD_LINE1,"",false},
	{szCmdItem::CMD_ASSING_EXP_BY_LEVLE,"经验等级分配",true},
	{szCmdItem::CMD_ASSING_EXP_BY_SPECIAL,"经验特殊分配",true},
	{szCmdItem::CMD_LINE2,"",false},
	{szCmdItem::CMD_PICKUP_ITEM_FREEDOM,"物品自由拾取",true},
	{szCmdItem::CMD_PICKUP_ITEM_POLL,"物品轮流拾取",true},
	{szCmdItem::CMD_LINE3,"",false},
	{szCmdItem::CMD_UNCOMBIN,"解除合体状态",false},
	{szCmdItem::CMD_UNFACE,"解除变身状态",false},
	{szCmdItem::CMD_UN_TEAM_ATTACK,"解除阵法",false},
};

CGuiMain::CGuiMain(void)
: m_iChatType(CHAT_TYPE_NINE)
, m_iScreenWidth(0)
, m_rcIconClip(0,0,0,0)
, m_bMouseInBackground(false)
{
	FUNCTION_BEGIN;

	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		m_pBtnAccessKey[i] = NULL;
		m_pBtnAccessKeyToolTips[i] = NULL;
		m_bAccessKeyToolTipsNeedUpdate[i] = false;
	}
	m_curAccessKeyGroup = 0;
	m_mouseOverAccessKeyNo = -1;
	m_curEditAccessKeyNo = -1;

	m_aPkModeBtnList.Clear();

	m_BtnWalk = NULL;
	m_BtnRun = NULL;
	m_BtnPrivateChat = NULL;
	m_imgThreeValue = NULL;

	m_activeSkillSign = NULL;

	for( int i=0;i<10;i++ )
		m_itemNumImg[i] = NULL;

	m_aStateInfo.clear();


	//----------------------------------------------------------
	// Team flag 
	m_pTeamHeadIcon  = NULL ;
	m_pTeamExpAssignByLevel = NULL ;
	m_pTeamExpAssignBySpecial = NULL ;
	m_pTeamItemAssignByFreedom = NULL;
	m_pTeamItemAssignByPoll = NULL;	
	g_ptTeamHead.x =  0 ;
	g_ptTeamHead.y =  20;
	g_ptExpAssgin.x = 0;
	g_ptExpAssgin.y = 40;
	g_ptObjAssgin.x = 0 ;
	g_ptObjAssgin.y = 60;
	//----------------------------------------------------------

	m_pshuxBtn = NULL;
	FUNCTION_END;
}

CGuiMain::~CGuiMain()
{
	//原来根本就没有调用OnClose函数,难怪内存泄露
	FreePointerList<stHelpButtons>(m_helpBtns);
	//
	delete m_pshuxBtn;	
	m_pshuxBtn = NULL;
}

/**
* \brief 创建对话框
* 
* 
* 
* \return 
*/
void CGuiMain::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	int iGroup = 1;
	/*m_pBmpForeground.reserve(4);
	IBitmapList* pBmpList = GetDevice()->FindResource(GetGuiGraphicPackName());
	m_pBmpForeground.resize(0);
	IBitmap* pBmp;
	pBmp = pBmpList->LoadFrame(iGroup,56);
	if(pBmp) m_pBmpForeground.push_back(pBmp);
	pBmp = pBmpList->LoadFrame(iGroup,58);
	if(pBmp) m_pBmpForeground.push_back(pBmp);
	pBmp = pBmpList->LoadFrame(iGroup,60);
	if(pBmp) m_pBmpForeground.push_back(pBmp);
	pBmp = pBmpList->LoadFrame(iGroup,62);
	if(pBmp) m_pBmpForeground.push_back(pBmp);*/

	//Get active skill sign
	stResourceLocation rl;
	rl.SetFileName( GetGuiGraphicPackName() );
	rl.group = 1;
	rl.frame = 242;
	m_activeSkillSign =	 GetDevice()->FindBitmaps( &rl );

	//Get item num image on access key
	IBitmapList* pNumImglist = GetDevice()->FindResource( "data\\other.gl" );
	if ( pNumImglist )
	{
		IBitmap* pNumImg;
		for( int i=0;i<10;i++ )
		{
			pNumImg = pNumImglist->LoadFrame( 5, i );
			if ( pNumImg )
				m_itemNumImg[i] = pNumImg;
		}	
	}       	

	m_BtnWalk = GetButton( 203 );
	m_BtnRun = GetButton( 202 );
	//m_BtnAttackMode = GetButton( 16 );
	m_BtnPrivateChat = GetButton( 13 );
	m_imgThreeValue = GetImage( 3 );



	//Get access key btn
	stPointF scale(0.8f, 0.8f);
	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		m_pBtnAccessKey[i] = GetButton( 100+i );
		if ( m_pBtnAccessKey[i])
		{
			m_pBtnAccessKey[i]->SetLocation( g_nMainAccessBtnLeft+i*g_MainAccessBtnSpace, g_nMainAccessBtnTop );
			m_pBtnAccessKey[i]->SetSize( 34, 34 );
			m_pBtnAccessKey[i]->SetBitmapScale( scale );
			//Set tooltips
			if ( m_pBtnAccessKey[i]->GetToolTips() )
			{
				m_pBtnAccessKey[i]->GetToolTips()->Clear();
			}			
		}
	}


	m_aPkModeBtnList.Clear();
	m_aPkModeBtnList.SetType( BTN_LIST_TYPE_DOWN );
	m_aPkModeBtnList.SetSpace( 24 );
	for( i=0;i<PKMODE_NUM;i++ )
	{
		m_aPkModeBtnList.AddOneButton( GetButton( 30+i ) );
		if ( GetButton(30+i) && GetButton( 30+i )->GetToolTips() )
		{
			GetButton( 30+i )->GetToolTips()->SetAlignType( GUI_ALIGN_LEFT );
		}
	}
	m_aPkModeBtnList.SetLocation( stPointI(-m_x+100,-m_y+10) );
	m_aPkModeBtnList.SetActiveBtn( GetButton(30+1) );

	if ( GetButton(16) )
	{
		GetButton(16)->SetLocation( -10000,-10000 );
	}

	//Set sit down Btn tips align type
	if ( GetButton( 21 ) && GetButton( 21 )->GetToolTips() )
		GetButton( 21 )->GetToolTips()->SetAlignType( GUI_ALIGN_LEFT );

	if ( GetScene() && GetScene()->GetMainCharacter() )
	{
		m_curAccessKeyGroup = GetScene()->GetMainCharacter()->GetPreferAccessGroup();
	}

	m_menu = GetGuiManager()->CreateMenu( this );
	/*for ( int i=0;i<CMD_NUM;i++ )
		m_menu->AddMenuItem( i, szCmdName[i], CGuiMenu::MenuStyle_Command, cmdCanUse[i] );*/
	for( size_t i = 0 ; i < count_of(s_CmdItem); i++)
	{
		if( s_CmdItem[i].cmd == szCmdItem::CMD_LINE1 || s_CmdItem[i].cmd == szCmdItem::CMD_LINE2 || s_CmdItem[i].cmd == szCmdItem::CMD_LINE3)
			m_menu->AddMenuItem( 100+i, "", CGuiMenu::MenuStyle_Separate );
		else
			m_menu->AddMenuItem(i,s_CmdItem[i].strCmdName.c_str(),CGuiMenu::MenuStyle_Command,s_CmdItem[i].bCanUse);
	}
	SetPopupMenu( m_menu );
	//
	/*m_cartMenu = GetGuiManager()->CreateMenu( this );
	m_cartMenu->SetOwner( this );
	m_cartMenu->AddMenuItem( CartCMD_Play, "前进", CGuiMenu::MenuStyle_Command, true );
	m_cartMenu->AddMenuItem( CartCMD_Stop, "停止", CGuiMenu::MenuStyle_Command, true );
	m_cartMenu->SetVisible( false );*/


	m_pTableLeft = GetTable(300);
	if ( m_pTableLeft )
	{
		m_pTableLeft->m_iTableType = OBJECTCELLTYPE_EQUIP;
		m_pTableLeft->m_EquipPosition = stPointI(EQUIPCELLTYPE_LEFT,EQUIPCELLTYPE_MAKE);
	}
	
	m_pTableRight = GetTable(301);
	if (  m_pTableRight )
	{
		m_pTableRight->m_iTableType = OBJECTCELLTYPE_EQUIP;
		m_pTableRight->m_EquipPosition = stPointI(EQUIPCELLTYPE_RIGHT,EQUIPCELLTYPE_MAKE);
	}                                                                                     	
	//
	

	//GetButton( 11 )->PlayFlash( 0.002 );
         

	//----------------------------------------------------------------------
	// team flag
	stResourceLocation rlTemp;
	rlTemp.SetFileName(GetGuiGraphicPackName());
	rlTemp.group = 24;
	rlTemp.frame = 1;
	m_pTeamHeadIcon = GetDevice()->FindBitmaps(&rlTemp);     // 五角星

	 
	rlTemp.SetFileName(GetGuiGraphicPackName());
	rlTemp.group = 24;
	rlTemp.frame = 9;  // 0009 经验等级.tga
    m_pTeamExpAssignByLevel = GetDevice()->FindBitmaps(&rlTemp);


	rlTemp.SetFileName(GetGuiGraphicPackName());
	rlTemp.group = 24;
	rlTemp.frame = 10;  //  0010经验特殊.tga
	m_pTeamExpAssignBySpecial = GetDevice()->FindBitmaps(&rlTemp);
 

	rlTemp.SetFileName(GetGuiGraphicPackName());
	rlTemp.group = 24;
	rlTemp.frame = 11;  //  0011物品自由.tga
	m_pTeamItemAssignByFreedom = GetDevice()->FindBitmaps(&rlTemp);

	rlTemp.SetFileName(GetGuiGraphicPackName());
	rlTemp.group = 24;
	rlTemp.frame = 12;  //  0012物品轮流.tga
	m_pTeamItemAssignByPoll = GetDevice()->FindBitmaps(&rlTemp);


	IBitmap * pTempBitmap = NULL;
	if(m_pTeamExpAssignByLevel) 
	{
		pTempBitmap  = m_pTeamExpAssignByLevel->GetBitmap(0);
		if( pTempBitmap )
			m_rcClipTeamExpAssignByLevel = pTempBitmap->GetClipRect();
	}
	if( m_pTeamExpAssignBySpecial)
	{
		pTempBitmap = m_pTeamExpAssignBySpecial->GetBitmap(0);
		if( pTempBitmap)
			m_rcClipTeamExpAssignBySpecial = pTempBitmap->GetClipRect();
	}

	if( m_pTeamItemAssignByFreedom)
	{
		pTempBitmap = m_pTeamItemAssignByFreedom->GetBitmap(0);
		if( pTempBitmap)
			m_rcClipTeamItemAssignByFreedom = pTempBitmap->GetClipRect();
	}

	if( m_pTeamItemAssignByPoll)
	{
		pTempBitmap = m_pTeamItemAssignByPoll->GetBitmap(0);
		if( pTempBitmap)
			m_rcClipTeamItemAssignByPoll = pTempBitmap->GetClipRect();
	}
	//----------------------------------------------------------------------

	OnResetScreenSize();

	m_bEnabledSysmessage = false;
	if (initSysMessage("sysmessage\\messageSysatem.xml"))
	{
		m_bEnabledSysmessage = true;
		TRACE("@@@@@@@@@@@@@@@load系统公告xml文件成功！\n");
	}

	CanPrintCenter = false;

	FUNCTION_END;
}

/**
* \brief 初始化对话框图标
* 
* 
* 
* \return 
*/
void CGuiMain::ShowCartMenu( bool bPlay )
{
	/*if ( m_cartMenu )
	{
		m_cartMenu->SetVisible( true );
		stPointI pt = Engine_GetCursor()->GetPosition();
		m_cartMenu->SetLocation( pt.x, pt.y );

		if ( bPlay )
		{
			m_cartMenu->SetMenuEnabled( CartCMD_Play, false );
			m_cartMenu->SetMenuEnabled( CartCMD_Stop, true );
		}
		else
		{
			m_cartMenu->SetMenuEnabled( CartCMD_Play, true );
			m_cartMenu->SetMenuEnabled( CartCMD_Stop, false );
		}
	}*/
}

void CGuiMain::CloseCartMenu()
{
	/*if ( m_cartMenu )
		m_cartMenu->SetVisible( false );*/
}

/**
* \brief 初始化对话框图标
* 
* 
* 
* \return 
*/
void CGuiMain::InitIcon()
{
	FUNCTION_BEGIN;

	if ( GetScene()->GetMainCharacter() )
	{
		m_curAccessKeyGroup = GetScene()->GetMainCharacter()->GetPreferAccessGroup();

		//set character head icon
		stResourceLocation rl;
		rl.SetFileName( "data\\interfaces.gl" );
		/*rl.group = 22;
		rl.frame = 20 + GetCharTypeInfo( GetScene()->GetMainCharacter()->GetType() ).equippic;*/
		rl.group = 33;
		rl.frame = GetScene()->GetMainCharacter()->GetFace();

		IBitmaps * pBmps=  GetDevice()->FindBitmaps( &rl );

		if(pBmps && pBmps->GetBitmapCount() > 0)
		{
			IBitmap * pBmp = pBmps->GetBitmap(0);
			if(pBmp)
				m_rcIconClip = pBmp->GetClipRect();

			SetIcon( &rl );
		}
	}

	FUNCTION_END;
}

/**
* \brief 关闭对话框
* 
*	
* 
* \return 
*/
void CGuiMain::OnClose(void)
{
	FUNCTION_BEGIN;

	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		if ( m_pBtnAccessKey[i] )
			m_pBtnAccessKey[i] = NULL;
		if ( m_pBtnAccessKeyToolTips[i] )
			m_pBtnAccessKeyToolTips[i] = NULL;
	}

	m_aPkModeBtnList.Clear();

	if ( m_BtnWalk )
		m_BtnWalk = NULL;
	if ( m_BtnRun )
		m_BtnRun = NULL;

	GetGameGuiManager()->m_guiMain = NULL;

	if ( m_menu )
	{
		m_menu->Close();
		m_menu = NULL;
	}

	/*if ( m_cartMenu )
	{
		m_cartMenu->Close();
		m_cartMenu = NULL;
	}*/

	m_aStateInfo.clear();

	FUNCTION_END;
}

/**
* \brief 切换行走按钮
* 
* 
* 
* \return 
*/
void CGuiMain::OnOffWalkRunBtn()
{
	FUNCTION_BEGIN;

	if ( GetScene()->GetMainCharacter()->GetMoveSpeed() == 1 )			//m_BtnWalk->IsVisible()
	{
		if ( GetScene()->GetMainCharacter()->IsChangeFace() )
			return;
		if ( GetScene()->GetMainCharacter()->IsCombinationState() )
			return;

		GetScene()->GetMainCharacter()->SetMoveSpeed(2);
		if ( m_BtnWalk )
		{
			m_BtnWalk->SetVisible( false );
			m_BtnRun->SetVisible( true );
		}		
	}
	else
	{
		GetScene()->GetMainCharacter()->SetMoveSpeed(1);
		if ( m_BtnWalk )
		{
			m_BtnWalk->SetVisible( true );
			m_BtnRun->SetVisible( false );
		}		
	}

	FUNCTION_END;
}

void CGuiMain::SwitchWalk()
{
	OnOffWalkRunBtn();
}

/**
* \brief 关闭/打开商店对话框
* 
* 
* 
* \return 
*/
inline void OnOffSysDialog()
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiSystem && ((CGuiDialog*)GetGameGuiManager()->m_guiSystem)->IsVisible() )
	{
		((CGuiDialog*)GetGameGuiManager()->m_guiSystem)->SetVisible( false );
	}
	else
	{
		AddSystemDialog();
	}

	FUNCTION_END;
}

/**
* \brief 切换行走按钮
* 
* 脚本文件调用
* 
* \return 
*/
inline void OnOffWalkRunButton()
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiMain )
		GetGameGuiManager()->m_guiMain->OnOffWalkRunBtn();

	FUNCTION_END;
}

/**
* \brief 关闭/打开宠物对话框
* 
* 
* 
* \return 
*/
inline void OnOffPetDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiPetDlg && ((CGuiDialog*)GetGameGuiManager()->m_guiPetDlg)->IsVisible())
	{
		((CGuiDialog*)GetGameGuiManager()->m_guiPetDlg)->SetVisible(false);
	}
	else
	{
		GetGameGuiManager()->AddPetDialog();
		if(GetGameGuiManager()->m_guiPetDlg)
			GetGameGuiManager()->m_guiPetDlg->UpdateContents();
	}

	FUNCTION_END;
}

/**
* \brief 打开/关闭玩家详细信息对话框
* 
* 脚本文件调用
* 
* \return 
*/
inline void OnOffUserInfoDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiUserInfo && GetGameGuiManager()->m_guiUserInfo->IsVisible())
	{
		GetGameGuiManager()->m_guiUserInfo->SetVisible(false);
	}
	else
	{
		GetGameGuiManager()->AddUserInfo();
	}

	FUNCTION_END;
}

/**
* \brief 显示聊天输出对话框
* 
* 
* 
* \return 
*/
inline void ShowChatOutput()
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->AddChatOutput();

	FUNCTION_END;
}

/**
* \brief 隐藏聊天输出对话框
* 
* 
* 
* \return 
*/
inline void HideChatOutput()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiChatOutput && GetGameGuiManager()->m_guiChatOutput->IsVisible())
		GetGameGuiManager()->m_guiChatOutput->SetVisible(false);

	FUNCTION_END;
}

/**
* \brief 打开/关闭交易对话框
* 
* 
* 
* \return 
*/
inline void OnOffTradeDialog()
{
	FUNCTION_BEGIN;

	CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
	if( pMainRole && pMainRole->bIsShopStart() )
	{
		GetGameGuiManager()->AddClientSystemMessage("您正在摆摊，不能交易");
		return ;
	}	

	///第一次触发"点击交易按钮"事件
	OnTipHelp(USER_SETTING_HELP_TRADE);

	if(GetGameState()->IsStateItem(eGameStateItem_Normal))
	{

		//Stop follow event
		if ( GetScene()->GetMainCharacter() )
		{
			if ( GetScene()->GetMainCharacter()->IsFollowState() )
				GetScene()->GetMainCharacter()->ClearRunEvent();
		}	

		GetGameCursor()->SetCursor(CCursor::CursorType_Select,eCursorTrade);
	}

	FUNCTION_END;
}



/**
* \brief 打开/关闭组队对话框
* 
* 
* 
* \return 
*/
inline void OnOffTeamDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiTeam && ((CGuiDialog*)GetGameGuiManager()->m_guiTeam)->IsVisible())
	{
		((CGuiDialog*)GetGameGuiManager()->m_guiTeam)->SetVisible(false);
	}
	else
	{
		GetGameGuiManager()->AddTeam();
	}

	FUNCTION_END;
}

/**
* \brief 打开/关闭技能对话框
* 
* 
* 
* \return 
*/
inline void OnOffSkillDialog()
{
	FUNCTION_BEGIN;

	OnTipHelp(USER_SETTING_HELP_SKILL);

	if( 1 )
	{
		if(GetGameGuiManager()->m_guiSkill && ((CGuiDialog*)GetGameGuiManager()->m_guiSkill)->IsVisible())
		{
			((CGuiDialog*)GetGameGuiManager()->m_guiSkill)->SetVisible(false);
		}
		else
		{
			GetGameGuiManager()->AddSkill();

			if ( GetGameGuiManager()->m_guiSkill )
				GetGameGuiManager()->m_guiSkill->UpdateCurSerialTypeAccordingToWeaponInHand();
		}
	}
	else
	{
		if(GetGameGuiManager()->m_guiSkill && ((CGuiDialog*)GetGameGuiManager()->m_guiSkill)->IsVisible())
		{
			((CGuiDialog*)GetGameGuiManager()->m_guiSkill)->SetVisible(false);
		}
		else
		{
			GetGameGuiManager()->AddSkill();
		}
	}	

	FUNCTION_END;
}

/**
* \brief 关闭小地图
* 
* 
* 
* \return 
*/
inline void CloseMiniMap()
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiMiniMap && GetGameGuiManager()->m_guiMiniMap->IsVisible() )
		GetGameGuiManager()->m_guiMiniMap->SetVisible( false );

	FUNCTION_END;
}

void CGuiMain::OnAccessBtnSelectSkill( const int& btnID, const int& skillId )
{
	SkillBase_t* pSelectSkill = GetSkillBase( skillId );
	if ( IsCanSelectInHandSkill( pSelectSkill ) )
	{
		stResourceLocation rl;
		if ( m_pBtnAccessKey[btnID] )
		{
			int fNum = 0;
			//m_pBtnAccessKey[btnID]->GetBitmapInfo( rl, fNum );
			stPointI ptCursor(0,0);
			ptCursor.x += m_pBtnAccessKey[btnID]->GetWidth()/2;
			ptCursor.y += m_pBtnAccessKey[btnID]->GetHeight()/2;
			//GetScene()->GetMainCharacter()->m_pActiveSkill = pSelectSkill;
			if ( GetGameGuiManager()->m_guiTeam )
				GetGameGuiManager()->m_guiTeam->m_pActiveSkill = pSelectSkill;
			GetGameCursor()->SetCursor( CCursor::CursorType_Drag, eCursorUseAccess );
			//Engine_GetCursor()->SetImage( &rl, ptCursor );
		}							
	}
}

bool CGuiMain::OnFindAccessSkill()
{
	//if ( GetGameCursor()->IsDrag() && (GetGameCursor()->GetCursorImageType() == eCursorFindAccess) )
	{
		int id = GetMouseOverAccessKeyNo();
		if ( -1 != id )
		{
			enumGameAccelKeyType keyType;
			DWORD dwKeyID;
			BYTE curGroup = GetCurAccessKeyGroup();
			BYTE key[4];
			key[0] = VK_F1 + id;
			key[1] = 0;
			if( GetGameAccelKeyManager()->GetCurAccelKeyID(keyType,dwKeyID, curGroup, key) )
			{
				if ( GameAccelKeyType_Skill == keyType )
				{
					SkillBase_t* pSelectSkill = GetSkillBase( dwKeyID );
					//GetScene()->GetMainCharacter()->m_pActiveSkill = pSelectSkill;
					if ( GetGameGuiManager()->m_guiTeam )
						GetGameGuiManager()->m_guiTeam->m_pActiveSkill = pSelectSkill;
					if ( pSelectSkill && ( (pSelectSkill->dwType == SkillType_State) || (pSelectSkill->dwType == SkillType_StateAttack) )
						&& (pSelectSkill->dwTarget & SkillTarget_Single)
						&& !( (pSelectSkill->dwTarget & SkillTarget_Self) && !(pSelectSkill->dwTarget & SkillTarget_Single) ) )
					{
						if ( GetGameGuiManager()->m_guiTeam )
						{
							CCharacter* pCharacter = GetGameGuiManager()->m_guiTeam->GetSelMember();
							if ( pCharacter )
							{
								CMainCharacter::stRunEvent runEvent;
								runEvent.id = pCharacter->GetID();
								runEvent.target = CMainCharacter::eTargetType_Character;
								runEvent.pSkill = pSelectSkill;
								runEvent.pt = pCharacter->GetPos();
								runEvent.speed = 0;
								runEvent.event = CMainCharacter::eEventType_Attack;

								GetScene()->GetMainCharacter()->SetRunEvent( runEvent );

								RestoreMouseIcon();
								return true;
							}
						}						
					}
				}
			}
		}
	}

	//RestoreMouseIcon();
	return false;
}
/**
* \brief 处理快捷键按钮点击事件
* 
* 
* 
* \param id : 快捷键按钮id
* \return 
*/
inline void ParseAccessBtnClickEvent( int id )
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiMain )
	{
		if ( (GetGameCursor()->IsDrag()	&& ( (GetGameCursor()->GetCursorImageType() == eCursorSetAccess) || ( GetGameCursor()->GetCursorImageType() == eCursorDictate) ) ) || GetGuiManager()->GetFloatItem()  )
			GetGameGuiManager()->m_guiMain->SetAccessKey( id );
		else
		{
			if ( GetGameCursor()->IsDrag()
				&& (GetGameCursor()->GetCursorImageType() == eCursorEditAccess) )
			{
				if ( (id != -1)
					&& !( GetGameAccelKeyManager()->m_curEditAccessKey.bMainAccessKey
					&& (GetGameAccelKeyManager()->m_curEditAccessKey.keyNo == id) )	)	//Cur pos has access key btn
				{
					GetGameGuiManager()->m_guiMain->EditAccessKeyBtn( id );
				}
				else if( GetGameAccelKeyManager()->m_curEditAccessKey.bMainAccessKey
					&& (GetGameAccelKeyManager()->m_curEditAccessKey.keyNo == id)
					&& ( (GetGameGuiManager()->m_guiMain->m_pBtnAccessKey[id]->GetDragOffset().x == 0)
					&& (GetGameGuiManager()->m_guiMain->m_pBtnAccessKey[id]->GetDragOffset().y == 0) ) )
				{
					RestoreMouseIcon();
				}
			}
			else
			{
				//Use access point item or skill
				enumGameAccelKeyType keyType;
				DWORD dwKeyID;
				BYTE curGroup = GetGameGuiManager()->m_guiMain->GetCurAccessKeyGroup();
				BYTE key[4];
				key[0] = VK_F1 + id;
				key[1] = 0;
				if(GetGameAccelKeyManager()->GetCurAccelKeyID(keyType,dwKeyID, curGroup, key))
				{
					switch( keyType )
					{
					case GameAccelKeyType_Skill:
						{
							if ( isset_state(GetScene()->GetMainCharacter()->GetState() ,USTATE_CRAZY_ACTION)
								|| isset_state(GetScene()->GetMainCharacter()->GetState(),USTATE_CRAZY) )
							{
								RestoreMouseIcon();
								break;
							}
							//We call corresponding action according to skill target type
							SkillBase_t* pSelectSkill = GetSkillBase( dwKeyID );
							if ( pSelectSkill )
							{
								if ( GetScene()->GetMainCharacter()
									&& (GetScene()->GetMainCharacter()->GetSkillDisableStatus( pSelectSkill ) < 1) )
								{
									GetGameGuiManager()->AddClientSystemMessage( "技能处于冷却中!" );
									break;
								}

								if ( GetGameCursor()->IsDrag()
									&& (GetGameCursor()->GetCursorImageType() == eCursorFindAccess) )
								{
									if ( GetGameGuiManager()->m_guiMain )
										GetGameGuiManager()->m_guiMain->OnFindAccessSkill();
								}
								else
								{
									if ( GetGameCursor()->GetCursorImageType() == eCursorUseAccess )
									{
										if ( GetGameGuiManager()->m_guiTeam && GetGameGuiManager()->m_guiTeam->GetActiveSkill() )
										{
											//GetGameGuiManager()->m_guiTeam->UseSkillOnTeamMember( false );
											if ( pSelectSkill->dwID == GetGameGuiManager()->m_guiTeam->GetActiveSkill()->dwID )
												GetScene()->GetMainCharacter()->UseSkillOnCharacter( GetScene()->GetMainCharacter() );
										}
									}
									else
									{
										if ( IsCanSelectInHandSkill( pSelectSkill )
											&& ( !GetScene()->IsAnyAliveInterestNpc() /*|| (pSelectSkill->dwUseMethod & SkillUseMethod_BlueHand)*/ )
											/*&& !IsSelectFirstSkill( pSelectSkill )*/ )
										{
											//   My pet of type summon
											CPet* pPet = NULL;
											if ( pSelectSkill->dwID == SKILL_STRENGTHEN_SUMMON )
												pPet = GetScene()->FindPetByType( PET_TYPE_SUMMON );

											if ( pPet )
											{
												if ( GetGameGuiManager()->m_guiTeam )
													GetGameGuiManager()->m_guiTeam->m_pActiveSkill = pSelectSkill;
												GetScene()->GetMainCharacter()->UseSkillOnCharacter( pPet->GetNpc() );
											}
											else
											{
												if ( GetGameGuiManager()->m_guiMain )
													GetGameGuiManager()->m_guiMain->OnAccessBtnSelectSkill( id, dwKeyID );
											}											
										}
										else
										{
											GetScene()->GetMainCharacter()->OnSelectSkill( dwKeyID );
											RestoreMouseIcon();
										}
									}								
								}
							}										
						}
						break;
					case GameAccelKeyType_Item:
						{
							CRoleItem* pItem = GetScene()->GetMainCharacter()->FindItemByID( dwKeyID);
							if ( (NULL == pItem) || !pItem->IsAccessNeedThisID() )
								pItem = GetScene()->GetMainCharacter()->FindItemByLocationConstID( dwKeyID, OBJECTCELLTYPE_COMMON );
							else
							{
								if ( pItem->GetLocation().dwLocation != OBJECTCELLTYPE_COMMON )
									pItem = NULL;
							}
							if ( pItem )
							{
								CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
								CALL_USEITEM_P(pMainRole,pItem->GetThisID());	
							}
						}						
						break;
					case GameAccelKeyType_Face:
						{
							stResourceLocation rl;
							int fNum = 0;
							if ( GetGameGuiManager()->m_guiMain->m_pBtnAccessKey[id]->GetBitmapInfo( rl, fNum ) )
							{
								//GetGameCursor()->SetImage( &rl, stPointI(15,15) );
								GetGameAccelKeyManager()->m_curEditAccessKey.bMainAccessKey = true;
								GetGameAccelKeyManager()->m_curEditAccessKey.group = GetGameGuiManager()->m_guiMain->GetCurAccessKeyGroup();
								GetGameAccelKeyManager()->m_curEditAccessKey.keyNo = id;
								GetGameAccelKeyManager()->m_curEditAccessKey.rl = rl;
								GetGameAccelKeyManager()->m_curEditAccessKey.keyType = keyType;
								GetGameAccelKeyManager()->m_curEditAccessKey.keyID = dwKeyID;
								GetEmotionInfo()->OnPreUseEmotion(dwKeyID);
								GetEmotionInfo()->SetCurIconInfo(dwKeyID,FROM_MAIN);								
							}							
						}
						break;
					case GameAccelKeyType_Equipment:
						{
							CRoleItem* pItem = GetScene()->GetMainCharacter()->FindItemByID( dwKeyID );
							if ( pItem )
								EquipItem( pItem );
						}
						break;
					}
				}
			}			
		}
	}

	FUNCTION_END;
}

/**
* \brief 切换快捷键组
* 
* 
* 
* \param bDown : 往下/上翻
* \return 
*/
inline void ChangeAccesskeyGroup( bool bDown )
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiMain )
		GetGameGuiManager()->m_guiMain->ChangeCurKeyGroup( bDown );

	FUNCTION_END;
}

/**
* \brief 发送攻击模式变化
* 
* 
* 
* \param curMode : 选中攻击模式
* \return 
*/
inline bool SendAttackMode( int curMode )
{
	FUNCTION_BEGIN;

	if ( (GetGameState()->GetState()==eGameState_Run)
		&& !GetScene()->GetMainCharacter()->IsDie() )
	{
		///第一次触发"切换攻击模式"事件
		OnTipHelp(USER_SETTING_HELP_ATTACK_MODE_CHANGE);

		static DWORD tLastTimeSendAttackMode = xtimeGetTime() - 1000;

		if ( xtimeGetTime()-tLastTimeSendAttackMode < 500 )
			return false;

		stPKModeUserCmd cmd;
		if ( curMode == 255 )
		{
			cmd.byPKMode = ( GetScene()->GetMainCharacter()->GetPKMode() + 1 ) % PKMODE_NUM;
		}
		else
		{
			cmd.byPKMode = curMode;
		}

		SEND_USER_CMD(cmd);

		tLastTimeSendAttackMode = xtimeGetTime();

		return true;
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 打开/关闭附近玩家对话框
* 
* 
* 
* \return 
*/
inline void OnOffNearbyPlayer()
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiNearby->IsVisible() )
	{
		GetGameGuiManager()->m_guiNearby->SetVisible(false);
	}
	else
	{
		GetGameGuiManager()->AddNearby();
	}

	FUNCTION_END;
}

/**
* \brief 切换攻击模式
* 
* 
* 
* \param mode : 模式id
* \return 
*/
inline bool ChangeAttackMode( int mode )
{
	FUNCTION_BEGIN;

	if ( GetGameState()->GetState() == eGameState_Run && !GetScene()->GetMainCharacter()->IsDie() )
	{
		if ( mode != GetScene()->GetMainCharacter()->GetPKMode() )
		{
			stPKModeUserCmd cmd;
			cmd.byPKMode = mode;
			SEND_USER_CMD(cmd);
			//SendAttackMode( mode );	
		}

		if ( mode == GetScene()->GetMainCharacter()->GetPKMode() )
			GetGameGuiManager()->m_guiMain->OnOffPkModeList( mode );

		return true;
	}
	return false;

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool SaveBackBufferToFile()
{
	FUNCTION_BEGIN;

	char szFileName[MAX_PATH]={0};
	char szFilePath[MAX_PATH]={0};
	tm* chattime=NULL;
	time_t times;
	time(&times);
	chattime=localtime((const time_t*)&times);

	sprintf(szFilePath,"%s\\screenshot",GetGameApplication()->m_szAppPath);
	sprintf(szFileName,"%s\\%d%d%d%d%d%d.bmp",szFilePath,chattime->tm_year+1900,
		chattime->tm_mon+1,chattime->tm_mday,chattime->tm_hour,chattime->tm_min,chattime->tm_sec);

	::CreateDirectory(szFilePath,NULL);
	if (SUCCEEDED(GetDevice()->SaveBackSurfaceToFile(szFileName,D3DXIFF_BMP,NULL)))
		return true;
	return false;

	FUNCTION_END;
}

/**
* \brief 显示/隐藏浮动控制条
* 
* 
* 
* \param bShow : 显示/隐藏
* \return 返回值的描述
*/
inline void ShowFloatControlBar(bool bShow)
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guiFloatControl->SetVisible(bShow);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
inline void OnOffShortcutItem()
{
	FUNCTION_BEGIN;

	const int nItemHeight = 78;
	if(GetGameGuiManager()->m_guiShortcutItem->IsVisible())
	{
		//GetGameGuiManager()->m_guiShortcutItem->SetVisible(false);
		stGuiAnimationInfo st;
		st.dwColorDst = GetGameGuiManager()->m_guiMain->GetColor();//COLOR_ARGB(128,255,255,255);
		st.ptDst.x = GetGameGuiManager()->m_guiShortcutItem->GetX();
		st.ptDst.y = 53+10;
		st.fTime = 0.3f;
		st.eAniComplete = GuiAniEvent_Hide;
		GetGameGuiManager()->m_guiShortcutItem->BeginAnimation(&st);
	}
	else
	{
		GetGameGuiManager()->m_guiShortcutItem->SetVisible(true);
		stGuiAnimationInfo st;
		st.dwColorDst = GetGameGuiManager()->m_guiMain->GetColor();
		st.ptDst.x = GetGameGuiManager()->m_guiShortcutItem->GetX();
		st.ptDst.y = 0;
		st.fTime = 0.2f;
		st.eAniComplete = GuiAniEvent_None;
		GetGameGuiManager()->m_guiShortcutItem->BeginAnimation(&st);
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
inline void OnOffChatTypeSelect()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiChatSelect->IsVisible())
	{
		GetGameGuiManager()->m_guiChatSelect->EndPopup();
	}
	else
	{
		GetGameGuiManager()->m_guiChatSelect->BeginPopup();
	}

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
inline CGuiDialog* AddWorldConnDialog()
{
	FUNCTION_BEGIN;

	if( true == GetChatManager().OnOpenChatDialog() )
	{
		return NULL;
	}
	else
	{
		if (GetGameGuiManager()->m_guiWorldConn)
		{
			if (GetGameGuiManager()->m_guiWorldConn->IsVisible())
			{
				GetGameGuiManager()->m_guiWorldConn->SetVisible(false);
				return GetGameGuiManager()->m_guiWorldConn;
			}
			else if( GetGameGuiManager()->m_guiAdoptCartoonDlg && 
				GetGameGuiManager()->m_guiAdoptCartoonDlg->IsVisible() )
			{
				GetGameGuiManager()->m_guiAdoptCartoonDlg->SetVisible(false);
				return GetGameGuiManager()->m_guiWorldConn;
			}
			else
				GetGameGuiManager()->m_guiWorldConn->RefreshIsOffLine();
		}
		return GetGameGuiManager()->AddWorldConn();
	}
	return NULL;

	FUNCTION_END;
}


/**
* \brief 上、下马
* 
* 向服务器发送骑马状态变更
* 
* \return 
*/
inline void OnRide()
{
	FUNCTION_BEGIN;

	if( GetScene()->GetMainCharacter()->bIsShopStart() )
	{
		GetGameGuiManager()->AddClientSystemMessage("您正在摆摊，不能骑马");
		return ;
	}


	if (GetGameState()->GetState()==eGameState_Run)
	{
		static DWORD tLastTimeSendCMD = xtimeGetTime();
		if ( (xtimeGetTime() - tLastTimeSendCMD) > 1000 )
		{
			GetScene()->GetMainCharacter()->SetClientRideState( GetScene()->GetMainCharacter()->IsServerRideState() );
			GetScene()->GetMainCharacter()->SetClientSitDown( GetScene()->GetMainCharacter()->IsServerSitDown() );
		}

		if ( GetScene()->GetMainCharacter()->IsServerSitDown() || GetScene()->GetMainCharacter()->IsSitDown() || GetScene()->GetMainCharacter()->IsClientSitDown() )
			return;

		if ( GetScene()->GetMainCharacter()
			/*&& !(GetScene()->GetMainCharacter()->IsMoving())
			&& !(GetScene()->GetMainCharacter()->IsAttackState())*/
			&& !(GetScene()->GetMainCharacter()->IsChangeFace())
			&& !(GetScene()->GetMainCharacter()->IsCombinationState()) 
			/*&& GetScene()->GetMainCharacter()->IsAniStand()*/)
		{
			stRideMapScreenUserCmd cmd;
			TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Send ride cmd\n" );
			SEND_USER_CMD(cmd);

			GetScene()->GetMainCharacter()->SetClientRideState( !GetScene()->GetMainCharacter()->IsServerRideState() );
			tLastTimeSendCMD = xtimeGetTime();
		}		
	}	

	FUNCTION_END;
}

inline void OnOffAutoAttackSummon()
{
	FUNCTION_BEGIN;

	if( GetScene()->GetMainCharacter()->bIsShopStart() )
	{
		return ;
	}
	bool bAutoPk = GetGameApplication()->bClientSetted(enumAuto_Kill_Summon);
	if( !bAutoPk )
	{
		GetGameApplication()->SetClientSet(enumAuto_Kill_Summon,true);
		GetGameGuiManager()->AddClientSystemMessage("开启自动打怪");
	}
	else
	{
		GetGameApplication()->SetClientSet(enumAuto_Kill_Summon,false);
		if ( GetScene()->GetMainCharacter() )
		{
			GetScene()->GetMainCharacter()->ClearPath();
			GetScene()->GetMainCharacter()->ClearRunEvent();
		}
		GetGameGuiManager()->AddClientSystemMessage("关闭自动打怪");
	}
	if( GetGameGuiManager()->m_guiSystem )
	{
		GetGameGuiManager()->m_guiSystem->UpdateAutoPkSwitch();
	}

	FUNCTION_END;
}

inline void OnSitDown()
{
	FUNCTION_BEGIN;

	if ( GetGameState()->GetState() == eGameState_Run )
	{
		static DWORD tLastTimeSendCMD = xtimeGetTime() - 1000;
		
		//We control send msg rate
		if ( xtimeGetTime()-tLastTimeSendCMD < 500 )
			return;

		if ( (xtimeGetTime() - tLastTimeSendCMD) > 1000 )
		{
			GetScene()->GetMainCharacter()->SetClientRideState( GetScene()->GetMainCharacter()->IsServerRideState() );
			GetScene()->GetMainCharacter()->SetClientSitDown( GetScene()->GetMainCharacter()->IsServerSitDown() );
		}

		if ( GetScene()->GetMainCharacter()
			&& !(GetScene()->GetMainCharacter()->IsMoving())
			&& !(GetScene()->GetMainCharacter()->IsServerRideState() || GetScene()->GetMainCharacter()->IsRide() || GetScene()->GetMainCharacter()->IsClientRideState() )
			&& !(GetScene()->GetMainCharacter()->IsChangeFace())
			&& !(GetScene()->GetMainCharacter()->IsCombinationState())
			&& !(GetScene()->GetMainCharacter()->IsAttackState()) )
		{
			stSitDownMoveUserCmd cmd;
			TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Send sit-down cmd\n" );
			SEND_USER_CMD( cmd );

			GetScene()->GetMainCharacter()->SetClientSitDown( !GetScene()->GetMainCharacter()->IsServerSitDown() );
			tLastTimeSendCMD = xtimeGetTime();
		}
	}

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
inline CGuiDialog * AddTaskDialog()
{
	//第一次触发"打开任务对话框"事件
	OnTipHelp(USER_SETTING_HELP_TASK);

	if(GetGameGuiManager()->m_guitaskdialog && ((CGuiDialog*)GetGameGuiManager()->m_guitaskdialog)->IsVisible())
	{
		((CGuiDialog*)GetGameGuiManager()->m_guitaskdialog)->SetVisible(false);
	}
	else
	{
		GetGameGuiManager()->AddTaskDialog();
	}
	return (CGuiDialog*)GetGameGuiManager()->m_guitaskdialog;
}

inline void OnFollowEvent()
{
	FUNCTION_BEGIN;

	if( GetScene()->GetMainCharacter()->bIsShopStart() )
	{
		GetGameGuiManager()->AddClientSystemMessage("您正在摆摊，不能跟随");
		return ;
	}

	if(GetGameState()->IsStateItem(eGameStateItem_Normal))
		GetGameCursor()->SetCursor(CCursor::CursorType_Select,eCursorFollowUser);

	FUNCTION_END;
}

inline void OnLookAtEvent()
{
	FUNCTION_BEGIN;

	if(GetGameState()->IsStateItem(eGameStateItem_Normal))
		GetGameCursor()->SetCursor(CCursor::CursorType_Select,eCursorShowUser);

	FUNCTION_END;
}

inline void OnOffFloatAccessKeyDialog()
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiFloatAcessKey && !GetGameGuiManager()->m_guiFloatAcessKey->m_bPretendedHide )
		GetGameGuiManager()->m_guiFloatAcessKey->m_bPretendedHide = true;
	else
	{
		GetGameGuiManager()->AddFloatAccessKey();
		GetGameGuiManager()->m_guiFloatAcessKey->m_bPretendedHide = false;
		GetGameGuiManager()->m_guiFloatAcessKey->ResetDlgLocation();
	}
	if(GetGameGuiManager()->m_guiMain)
	{
		GetGameGuiManager()->m_guiMain->UpdateHelpBtnState();
	}


	FUNCTION_END;
}

inline void OnOffFloatControlDialog()
{
	FUNCTION_BEGIN;

	if ( GetGameGuiManager()->m_guiFloatControl && !GetGameGuiManager()->m_guiFloatControl->m_bPretendedHide )
		GetGameGuiManager()->m_guiFloatControl->m_bPretendedHide = true;
	else
	{
		GetGameGuiManager()->AddFloatControl();
		GetGameGuiManager()->m_guiFloatControl->m_bPretendedHide = false;
	}

	FUNCTION_END;
}



/**
* \brief 初始化脚本
* 
* 
* 
* \return 
*/
void CGuiMain::OnInitScript()
{
	FUNCTION_BEGIN;

	GameScriptInit(GetScriptEngine());
	using namespace luabind;
	module(GetScriptEngine())
	[
		def("ShowHelpDialog",&ShowHelpDialog),
		def("ShowEmotionDialog",&ShowEmotionDialog),
		def("OnOffItemDialog",&OnOffItemDialog),
		def("OnOffUserInfoDialog",&OnOffUserInfoDialog),
		def("OnOffSysDialog",&OnOffSysDialog),
		def("OnOffNearbyPlayer",&OnOffNearbyPlayer),
		def("OnOffTradeDialog",&OnOffTradeDialog),
		def("OnOffTeamDialog",&OnOffTeamDialog),
		def("OnOffSkillDialog",&OnOffSkillDialog),
		def("AddWorldConnDialog",&AddWorldConnDialog),
		def("SendAttackMode",&SendAttackMode),
		def("ChangeAttackMode",&ChangeAttackMode),
		def("ParseAccessBtnClickEvent",&ParseAccessBtnClickEvent),
		def("ChangeAccesskeyGroup",&ChangeAccesskeyGroup),
		def("OnOffWalkRunButton",&OnOffWalkRunButton),
		def("CloseMiniMap",&CloseMiniMap),
		def("AddTaskDialog",&AddTaskDialog),
		def("OnOffPetDialog",&OnOffPetDialog),
		def("OnRide",&OnRide),
		def("OnFollowEvent",&OnFollowEvent),
		def("OnLookAtEvent",&OnLookAtEvent),
		def("OnOffFloatAccessKeyDialog",&OnOffFloatAccessKeyDialog),
		def("OnOffFloatControlDialog",&OnOffFloatControlDialog),
		def("OnSitDown",&OnSitDown),
		def("OnOffAutoAttackSummon",&OnOffAutoAttackSummon)
	];

	FUNCTION_END;
}

/**
* \brief 缩回PK模式列表
* 
* 
* 
* \return 
*/
void CGuiMain::TurnOffPkModeList()
{
	FUNCTION_BEGIN;

	m_aPkModeBtnList.TurnOffBtnList();

	FUNCTION_END;
}

/**
* \brief 打开/关闭ＰＫ列表
* 
* 
* 
* \param curMode : 当前的ＰＫ模式
* \return 
*/
void CGuiMain::OnOffPkModeList( int curMode )
{
	FUNCTION_BEGIN;

	m_aPkModeBtnList.OnBtnClickEvent( GetButton(30+curMode) );

	FUNCTION_END;
}

/**
* \brief 确定鼠标是否在对话框内
* 
* 
* 
* \param ptScreen : 鼠标位置
* \return 是则返回ｔｒｕｅ，否则返回ｆａｌｓｅ
*/
BOOL CGuiMain::ContainsPoint( POINT ptScreen )
{
	FUNCTION_BEGIN;

	if(CGuiDialog::ContainsPoint(ptScreen)) 
	{
		m_bMouseInBackground = true;
		return TRUE;
	}

	m_bMouseInBackground = false;

	stPointI pt1 = ptScreen;
	pt1.x -= m_x + m_clientOffsetTopLeft.x;
	pt1.y -= m_y + m_clientOffsetTopLeft.y;
	CGuiControl* p = this->GetControlAtPoint(pt1);
	if(p)
	{
		return TRUE;
	}
	return FALSE;

	FUNCTION_END;
}

BOOL CGuiMain::IsInRect(POINT ptScreen)
{
	return TRUE;
}
/**
* \brief 刷新当前ＰＫ模式列表，使之符合显示要求
* 
* 
* 
* \return 
*/
void CGuiMain::ChangeActivePkModeBtn()
{
	FUNCTION_BEGIN;

	int curPkMode;
	if ( GetScene()->GetMainCharacter() )
		curPkMode = GetScene()->GetMainCharacter()->GetPKMode();
	else
		curPkMode = 0;

	m_aPkModeBtnList.SetActiveBtn( GetButton(30+curPkMode) );

	FUNCTION_END;
}

/**
* \brief 刷新攻击模式按钮提示
* 
* 
* 
* \return 
*/
void CGuiMain::ChangeAttackModeBtnTips()
{
	FUNCTION_BEGIN;

	//if ( m_BtnAttackMode && (GetScene()->GetMainCharacter()) )
	if ( GetScene()->GetMainCharacter() )
	{
		BYTE curMode = GetScene()->GetMainCharacter()->GetPKMode();
		if ( (curMode < PKMODE_NUM) && (curMode>=0) )
		{	
			//Change active PK mode btn
			ChangeActivePkModeBtn();
		}
	}

	FUNCTION_END;
}

/**
* \brief 发送聊天信息
* 
* 
* 
* \param pEditChat : 聊天编辑框
* \return 
*/
void CGuiMain::SendChat( CGuiEditBox* pEditChat )
{
	FUNCTION_BEGIN;

	if ( NULL == pEditChat )
		return;	

	const char* pszText = pEditChat->GetText();
	char szCharText[512]={0};
	strncpy(szCharText,pszText,sizeof(szCharText));
	str_trim_left((char *)szCharText);
	str_trim_right((char *)szCharText);
	if(strlen(szCharText) == 0) return;

	const char* pszName = GetScene()->GetMainCharacter()->GetName();
	CChatManager::SendChatCmd(pEditChat,0,m_iChatType,pszName);

	// 
	if(strcmp(pszText,"//npcdlg") == 0)
	{
		if(IsFileExist("Npc交易脚本.txt"))
		{
			char * npcScript;
			FileStream file;
			file.open("Npc交易脚本.txt",FileStream::Read);
			npcScript = new char[file.getSize()+1];
			file.read(npcScript,file.getSize());
			npcScript[file.getSize()] = 0;
			CGuiNpcDialog* pDlg = GetGameGuiManager()->AddNpcDialog();
			pDlg->SetNpcScript(npcScript);
			delete [] npcScript;
		}
	}

	if(strcmp(pszText,"//cltime") == 0)
	{        
		AddClientSystemMessageF("%s %s",__DATE__,__TIME__);
	}

	if(stricmp(pszText,"//ride") == 0 && g_bSinglePlayer)
	{
		static bool bRide = false;
		bRide  = !bRide;
		stAddUserMapScreenUserCmd cmd;
		strcpy(cmd.data.name,"无");
		cmd.data = *((CCharacter*)GetScene()->GetMainCharacter())->GetProData();
		if(bRide)
			set_state(cmd.data.state,USTATE_RIDE);
		else
			clear_state(cmd.data.state,USTATE_RIDE);
		GetClient()->PushCommand(&cmd,sizeof(cmd));
	}
	if(strstr(pszText,"//skey ") == pszText)
	{
		SetFrameAllocator fa;
		std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
		_parse_str(strs,pszText,' ');
		if(strs.size() == 3)
		{
			stGameAccelKey key;
			memset(&key,0,sizeof(key));
			key.type = GameAccelKeyType_Skill;
			key.dwID = atol(strs[1].c_str());
			key.szAccelKey[0] = VK_F1 + atol(strs[2].c_str()) - 1;
			GetGameAccelKeyManager()->SetKey(key);
		}
	}
	if(g_bSinglePlayer)
	{
		stAddUserMapScreenUserCmd cmd;
		strcpy(cmd.data.name,"无");
		cmd.data = *((CCharacter*)GetScene()->GetMainCharacter())->GetProData();


		if(strstr(pszText,"//movespeed=") == pszText)
		{
			SetFrameAllocator fa;
			std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
			_parse_str(strs,pszText,'=');
			if(strs.size() == 2)
			{
				cmd.data.movespeed = atol(strs[1].c_str());
			}
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}
		if(strstr(pszText,"//attackspeed=") == pszText)
		{
			SetFrameAllocator fa;
			std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
			_parse_str(strs,pszText,'=');
			if(strs.size() == 2)
			{
				cmd.data.attackspeed = atol(strs[1].c_str());
			}
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}
		if(strstr(pszText,"//magicspeed=") == pszText)
		{
			SetFrameAllocator fa;
			std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
			_parse_str(strs,pszText,'=');
			if(strs.size() == 2)
			{
				cmd.data.attackspeed = atol(strs[1].c_str());
			}
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}

	}
#if defined _DEBUG || defined RELEASE_TEST || defined _AUTOPLAY
	if(strstr(pszText,"//callnpc ") == pszText){
		SetFrameAllocator fa;
		std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
		_parse_str(strs,pszText,' ');
		if(strs.size() == 2){
			CNpc* pNpc = GetScene()->FindNpcByID(atol(strs[1].c_str()));
			if(!pNpc)
				pNpc = GetScene()->FindNpcByName(strs[1].c_str());
			if(pNpc)
				GetScene()->GetMainCharacter()->CallNpc(pNpc);
		}
	}
	else if(strstr(pszText,"//me ") == pszText)
	{
		extern DWORD gWalkTimeElapsed;
		SetFrameAllocator fa;
		std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
		_parse_str(strs,pszText,' ');
		if(strs.size() == 2)
		{
			gWalkTimeElapsed = atol(strs[1].c_str());
			pEditChat->SetText("");
			return;
		}
	}
	else if(strstr(pszText,"//gt ") == pszText)
	{

		SetFrameAllocator fa;
		std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
		_parse_str(strs,pszText,' ');
		if(strs.size() == 2)
		{
			std::vector<int,std::stack_allocator<int> > ints;
			_parse_str_num(ints,strs[1].c_str());
			if(ints.size() == 2)
			{
				GetScene()->GetMainCharacter()->Goto(stPointI(ints[0],ints[1]));
				pEditChat->SetText("");
				return;
			}
		}
	}
#endif


	FUNCTION_END;
}

/**
* \brief 返回聊天背景颜色
* 
* 
* 
* \param dwType : 聊天类型
* \return 
*/
static DWORD GetChatBkColor(DWORD dwType)
{
	FUNCTION_BEGIN;

	return 0;

	FUNCTION_END;
}

/**
* \brief 对话框消息处理
* 
* 
* 
* \param hWnd : 
* \param uMsg : 
* \param wParam : 
* \param lParam : 
* \return 
*/
bool CGuiMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	FUNCTION_BEGIN;

	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_RETURN:
			if ( 1 )		//!(GetGameGuiManager()->m_guiChatSelect->IsVisible())
			{
				if(CLordStrikeManager::GetSingleton()->IsInLordStrike())
				{
					CLordStrikeManager::GetSingleton()->SendChat();
					break;
				}
				if ( GetGameGuiManager()->m_guiChatSelect )
				{
					//第一次触发"物品链接"事件
					OnTipHelp(USER_SETTING_HELP_ITEM_LINK);

					//第一次触发"悄悄话"事件
					OnTipHelp(USER_SETTING_HELP_PRIVATE_CHAT);

					GetGameGuiManager()->m_guiChatSelect->SetVisible( true );

					//play popup anim
					GetGameGuiManager()->m_guiChatSelect->BeginPopup();	
					GetGameGuiManager()->m_guiChatSelect->SetZOrder( GUI_DIALOG_BOTTOMMOST );
					//Set focuse to chat select dlg
					GetGuiManager()->RequestFocus( GetGameGuiManager()->m_guiChatSelect );
					GetGameGuiManager()->m_guiChatSelect->SetFocus( GetGameGuiManager()->m_guiChatSelect->GetEditChat() );
					return true;
				}
			}
			break;
		case VK_SPACE:
			if (( GetGameGuiManager()->m_guiChatSelect ) && (GetGameGuiManager()->m_guiChatOutput ))
			{
				GetGameGuiManager()->m_guiChatSelect->GetEditChat()->SetText(GetGameGuiManager()->m_guiChatOutput->GetPrivateName());
			}
			MsgProc(hWnd,WM_KEYDOWN,VK_RETURN,lParam);
			break;
		case VK_PRIOR:
			ChangeCurKeyGroup( false );
			break;
		case VK_NEXT:
			ChangeCurKeyGroup();
			break;
		case VK_HOME:
			RefreshKeyGroupState();
			break;
		case VK_END:
			RefreshKeyGroupState( MAIN_ACCESS_KEY_GROUP-1);
			break;
		case VK_DELETE:
			//Delete mouse over access key
			DeleteMouseOverAccessKey();
			break;
		case VK_UP:
			if ( GetGameGuiManager()->m_guiChatSelect )
			{
				if ( GetGameGuiManager()->m_guiChatSelect->IsVisible() )
				{
					if ( GetGameGuiManager()->m_guiChatSelect->GetEditChat() )
						GetGameGuiManager()->m_guiChatSelect->GetEditChat()->ChangeCurChatHistory( false );
				}
			}
			break;
		case VK_DOWN:
			if ( GetGameGuiManager()->m_guiChatSelect )
			{
				if ( GetGameGuiManager()->m_guiChatSelect->IsVisible() )
				{
					if ( GetGameGuiManager()->m_guiChatSelect->GetEditChat() )
						GetGameGuiManager()->m_guiChatSelect->GetEditChat()->ChangeCurChatHistory();
				}
			}
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
#ifdef _DEBUG
		case 'Q':
			{
				// 旗帜受到攻击
				
					char szLastFileName[MAX_PATH] = "sound\\npc\\60307.wav";
					if( strlen(szLastFileName) > 0 && GetSoundManager()->IsPlaying(szLastFileName))
					{  
					break;
					}
					PlayGameSound( szLastFileName,SoundType_UI);  
				

			}
			break;
#endif
		}
	}
	return CGuiDialog::MsgProc(hWnd,uMsg,wParam,lParam);

	FUNCTION_END;
}

/**
* \brief 输出鼠标所指快捷键
* 
* 
* 
* \return 
*/
void CGuiMain::DeleteMouseOverAccessKey()
{
	FUNCTION_BEGIN;

	if ( m_curAccessKeyGroup <0 || m_curAccessKeyGroup >= MAIN_ACCESS_KEY_GROUP )
		return;
	if ( m_mouseOverAccessKeyNo <0 || m_mouseOverAccessKeyNo >= MAIN_ACCESS_KEY_NUM	)
		return;

	//Really has a key to delete
	BYTE key[4];
	key[0] = VK_F1+m_mouseOverAccessKeyNo;
	key[1] = 0;

	if ( GetGameAccelKeyManager()->DeleteOneAccelKey( key, (BYTE)m_curAccessKeyGroup ) )
	{
		RemoveAccessKeyBtnInfo( (BYTE)m_mouseOverAccessKeyNo );
	}

	FUNCTION_END;
}

/**
* \brief 删除快捷键
* 
* 
* 
* \param type : 类型
* \param id : ｉｄ
* \return 
*/
void CGuiMain::DeleteAccessKey( enumGameAccelKeyType type, DWORD& id )
{
	FUNCTION_BEGIN;

	BYTE key[4];
	BYTE group;

	while( GetGameAccelKeyManager()->DeleteAccelKey( type, id, key, group ) )
	{
		//Really deleted one key
		if ( group == m_curAccessKeyGroup )
			RemoveAccessKeyBtnInfo( key[0]-VK_F1 );
		else if( group == FLOAT_ACCESS_KEY_GROUP )
		{
			if ( GetGameGuiManager()->m_guiFloatAcessKey )
				GetGameGuiManager()->m_guiFloatAcessKey->DeleteAccessKey( key[0] );
		}
	}

	FUNCTION_END;
}

void CGuiMain::ReplaceAccessKey( enumGameAccelKeyType type, const DWORD& id, const DWORD& newID )
{
	FUNCTION_BEGIN;

	while ( GetGameAccelKeyManager()->ReplaceAccelKey( type, id, newID ) )
	{

	}

	FUNCTION_END;
}

/**
* \brief 删除快捷键对应按钮相关信息
* 
* 
* 
* \param key : 按钮ｉｄ
* \return 
*/
void CGuiMain::RemoveAccessKeyBtnInfo( BYTE key )
{
	FUNCTION_BEGIN;

	if ( key<0 || key >= MAIN_ACCESS_KEY_NUM )
		return;

	if ( m_pBtnAccessKey[key] )
	{
		stResourceLocation rl;
		m_pBtnAccessKey[key]->SetBitmap( rl, 0 );

#ifndef ACCESS_BTN_FULL_TIPS			
		m_pBtnAccessKey[key]->GetToolTips()->Clear();
#else
		m_pBtnAccessKeyToolTips[key] = NULL;
#endif

	}

	FUNCTION_END;
}

/**
* \brief 绘制三个显示条
* 
* 
* 
* \param bar : 显示条结构
* \param iCur : 当前值
* \param iMax : 最大值
* \param index : 显示条ｉｄ
* \return 
*/
void CGuiMain::RenderBar(CGuiMain::stBarShowInfo& bar,size_t iCur,size_t iMax,int index)
{
	FUNCTION_BEGIN;

	if(!bar.pBmp) return;

	if ( g_bSinglePlayer )
	{
		iCur = 100;
		iMax = 200;
	}

	if ( iMax == 0 )
		return;

	static const char* desc[] = { "生命值  当前生命值为0时人会死亡，在非战斗状态和吃药时生命值可恢复",
		"法术值  施用技能消耗法术值，在非战斗状态和吃药时法术值可恢复",
		"体力值  进行各种行动要消耗体力值，在非战斗状态和吃药时体力值可恢复",
		"经验值" };

	stRectI rc = bar.rcClip;
	stPointI pt(rc.left,rc.top);
	if(iMax)
	{
		if(bar.type == BarTypeVert)
		{
			rc.top = bar.rcClip.bottom - (long)(bar.rcClip.Height() * (float)iCur/iMax);
			pt.y = rc.top;
		}
		else
		{
			rc.right = bar.rcClip.left + bar.rcClip.Width() * (float)iCur/iMax;
			pt.x = rc.left;
		}
	}
	//pt.x += m_x;
	//pt.y += m_y;

	if( 2 == index )
	{
		//Get color according to tire value
		/*DWORD color = -1;
		int step = (GetScene()->GetMainCharacter()->GetTire() - START_TIRE_VALUE - 1) / ( ( MAX_TIRE_VALUE - START_TIRE_VALUE ) / TIRE_COLOR_STEP );
		//int step = (100+48*2 - START_TIRE_VALUE - 1) / ( ( MAX_TIRE_VALUE - START_TIRE_VALUE ) / TIRE_COLOR_STEP );
		if ( step < 0 )
		color = COLOR_ARGB(255,0,255,0);
		else
		{
		switch( step )
		{
		case 0:
		color = COLOR_ARGB(255,113,202,129);
		break;
		case 1:
		color = COLOR_ARGB(255,210,214,80);
		break;
		case 2:
		color = COLOR_ARGB(255,239,111,24);
		break;
		case 3:
		case 4:
		color = COLOR_ARGB(255,255,10,10);
		break;
		}
		}*/
		//This time we use continue change
		D3DXCOLOR color(0.0f,1.0f,0.0f,1.0f);
		/*static float tire = 0;
		if ( tire > MAX_TIRE_VALUE )
		tire = 0;
		tire += 0.1f;*/

		if ( GetScene()->GetMainCharacter()->GetTire() - START_TIRE_VALUE > 0 )		//GetScene()->GetMainCharacter()->GetTire()
		{
			color.r = (float)(GetScene()->GetMainCharacter()->GetTire()-START_TIRE_VALUE)/(float)(MAX_TIRE_VALUE-START_TIRE_VALUE);		//GetScene()->GetMainCharacter()->GetTire()
			if ( color.r > 1.0f )
				color.r = 1.0f;
			if ( color.r < 0.0f )
				color.r = 0.0f;
			color.g = 1.0f - color.r;
		}

		bar.pBmp->Render( pt.x + m_x, pt.y + m_y+1, &rc, NULL, (DWORD)color, Blend_Null );
	}
	else	
	{
		// alpha = 128 据说可以防止按键精灵	
		bar.pBmp->Render( pt.x, pt.y, &rc, NULL,COLOR_ARGB(dwAlpha_d,255,255,255));
	}

	//Render text
	char strValue[32];
	switch( index )
	{
	case 0:
		sprintf( strValue, "\n( %u / %u )", iCur, iMax );
		break;
	case 1:
		sprintf( strValue, "\n( %u / %u )", iCur, iMax );
		break;
	case 2:
		sprintf( strValue, "\n( %u / %u )", iCur, iMax );
		break;
	case 3:
		sprintf( strValue, "%u / %u", iCur, iMax );
		break;
	}
	pt.x += 5;

	//See if mouse over
	stPointI ptCursor = Engine_GetCursor()->GetPosition();
	bool bMouseOver = bar.rcClip.PtInRect( ptCursor ) && (GetGuiManager()->GetMouseOverDlg() == this);
	if ( 2 == index )
	{
		ptCursor = stPointI(ptCursor.x-m_x, ptCursor.y-m_y);
		bMouseOver = bar.rcClip.PtInRect( ptCursor );
		if ( bMouseOver )
		{
			//CToolTips tips;
			m_ExpTips.Clear();
			m_ExpTips.SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
			m_ExpTips.SetAlignType( GUI_ALIGN_LEFT );
			m_ExpTips.SetBorderColor( 0 );
			m_ExpTips.SetLineGaps( 3 );
			m_ExpTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
			m_ExpTips.AddText( "经验值" );
			char szTemp[256];
			_snprintf(szTemp,sizeof(szTemp),"\n当前经验值 %u", iCur );
			szTemp[sizeof(szTemp)-1]=0;
			m_ExpTips.AddText( szTemp );

			_snprintf(szTemp,sizeof(szTemp),"\n升至下一等级需要经验值 %u", iMax );
			szTemp[sizeof(szTemp)-1]=0;
			m_ExpTips.AddText( szTemp );

			if ( GetScene()->GetMainCharacter() )
			{
				int tire = GetScene()->GetMainCharacter()->GetTire();
				//tire = 19*60/5 + 20;
				if ( tire <= START_TIRE_VALUE )
					m_ExpTips.AddText( "\n人物当前状态：正常状态。您可以从怪物身上获得100％经验值" );
				else
				{
					int exp = 100;					
					if ( tire > START_TIRE_VALUE )
						exp = 90;
					if ( tire > 13*60/5 )
						exp = 80;
					if ( tire > 14*60/5 )
						exp = 60;
					if ( tire > 15*60/5 )
						exp = 40;
					if ( tire > 16*60/5 )
						exp = 20;
					if ( tire > 17*60/5 )
						exp = 0;

					char desc[MAX_PATH];
					desc[0] = 0;
					sprintf( desc, "\n人物当前状态：疲劳状态。您可以从怪物身上获得 %u%% 经验值", exp );

					m_ExpTips.AddText( desc );
				}
			}			

			m_ExpTips.Resize();

			//Get position
			stRectI rcTips;
			rcTips.top = pt.y + m_y;
			rcTips.bottom = rcTips.top + 39;
			rcTips.left = pt.x + m_x;

			int len = 39;
			if( m_pBtnAccessKey[1] && m_pBtnAccessKey[0] )
			{
				rcTips.left = m_pBtnAccessKey[0]->GetLocation().x + m_x;
				len = m_pBtnAccessKey[1]->GetLocation().x - m_pBtnAccessKey[0]->GetLocation().x;
			}

			int step = ( ptCursor.x - pt.x ) / len;

			rcTips.left += step * len;
			rcTips.right = rcTips.left + len - 5;

			//tips.Render( rcTips.left, rcTips.top - tips.GetHeight() );
			/*if ( NULL == GetGuiManager()->GetToolTips() )
			GetGuiManager()->SetToolTips( &m_ExpTips, stPointI(rcTips.right,rcTips.bottom) );*/
			m_ExpTips.RenderTopLeftPrefer( rcTips, GetDevice()->GetWidth(), GetDevice()->GetHeight() );	
		}
	}
	else
	{
		bMouseOver = bar.pBmp->IsAt(ptCursor,false );
		//GetDevice()->DrawString( strValue, pt );
		//GetDevice()->DrawString( strValue, pt );
		if ( bMouseOver )
		{
			m_ExpTips.Clear();
			m_ExpTips.SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
			m_ExpTips.SetAlignType( GUI_ALIGN_LEFT );
			m_ExpTips.SetBorderColor( 0 );
			m_ExpTips.SetLineGaps( 3 );
			m_ExpTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
			m_ExpTips.AddText( desc[index] );
			m_ExpTips.SetCurColor( D3DCOLOR_ARGB(255,255,0,0) );
			m_ExpTips.AddText( strValue );
			m_ExpTips.Resize();

			stPointI ptTips;
			ptTips.x = bar.rcClip.right + 5;
			ptTips.y = bar.rcClip.top - m_ExpTips.GetHeight();

			if(ptTips.y < 0)
				ptTips.y = bar.rcClip.bottom;			

			if ( NULL == GetGuiManager()->GetToolTips() )
				GetGuiManager()->SetToolTips( &m_ExpTips, ptTips );
			//tips.Render( ptTips.x, ptTips.y );
		}
	}



	FUNCTION_END;
}

/**
* \brief 绘制快捷键编号
* 
* 
* 
* \param fElapsedTime : 
* \return 
*/
//Render F1~F12 notes
HRESULT CGuiMain::RenderAccessKeyNotes( float fElapsedTime )
{
	FUNCTION_BEGIN;

	//Get access key btn
	int nLeft = 19;
	int nTop = 21;	
	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		if ( m_pBtnAccessKey[i] )
		{
			//Enable btn
			m_pBtnAccessKey[i]->SetEnabled( true );

			//Get position
			stPointI pt;
			pt.x = g_nMainAccessBtnLeft + i * g_MainAccessBtnSpace ;
			pt.y = g_nMainAccessBtnTop;

			//pt = m_pBtnAccessKey[i]->GetLocation();

			pt.x += m_x;
			pt.y += m_y;

			pt.x += nLeft;
			pt.y += nTop;

			if ( i>=9 )
				pt.x -= 4;

			char keyName[4];
			sprintf( keyName, "%s%d", "F", i+1 );

			GetDevice()->DrawString( keyName, pt );

			//Render item num
			enumGameAccelKeyType type;
			DWORD id;
			BYTE key[4];
			key[0] = VK_F1 + i;
			key[1] = 0;
			if ( GetGameAccelKeyManager()->GetAccessKeyIdAndType( key, m_curAccessKeyGroup, type, id ) )
			{
				if ( (type == GameAccelKeyType_Item) || (type == GameAccelKeyType_Equipment) )
				{
					CRoleItem* pItem = NULL;
					if ( type == GameAccelKeyType_Item )
					{
						pItem = GetScene()->GetMainCharacter()->FindItemByID( id );
						if ( (NULL == pItem) || !pItem->IsAccessNeedThisID() )
							pItem = GetScene()->GetMainCharacter()->FindItemByLocationConstID( id, OBJECTCELLTYPE_COMMON );
					}
					else if ( type == GameAccelKeyType_Equipment )
						pItem = GetScene()->GetMainCharacter()->FindItemByID( id );

					if ( pItem
						&& ( (pItem->GetLocation().dwLocation == OBJECTCELLTYPE_COMMON)
						|| (pItem->GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP)
						|| (pItem->GetLocation().dwLocation == OBJECTCELLTYPE_SELL)
						/*|| (pItem->GetLocation().dwLocation == OBJECTCELLTYPE_PACKAGE)*/
						|| (pItem->GetLocation().dwLocation == OBJECTCELLTYPE_STORE) ) )
					{
						//Get offset cause of different screen size
						stPointI pt;
						pt = m_pBtnAccessKey[i]->GetLocation();
						pt.x += m_x + 1;
						pt.y += m_y;

						if ( GetScene()->GetMainCharacter() && (type == GameAccelKeyType_Item) && !pItem->IsAccessNeedThisID() )
						{
							int itemCount = GetScene()->GetMainCharacter()->GetItemCount( pItem->GetObjectID(), OBJECTCELLTYPE_COMMON )
								/*+ GetScene()->GetMainCharacter()->GetItemCount( pItem->GetObjectID(), OBJECTCELLTYPE_PACKAGE )*/;
								if ( itemCount > 0 )
								{
									if ( 0 )
									{
										char itemNum[5];
										sprintf( itemNum, "%d", itemCount );	//
										//GetDevice()->SetFontScale( 0.5f, 0.5f );
										GetDevice()->DrawString( itemNum, pt );
										//GetDevice()->SetFontScale( 1.0f, 1.0f );
									}
									else		//This time we use num image
									{
										//Get num list
										int curNum = itemCount;
										std::list<int> numList;
										numList.clear();

										while ( curNum >= 10 )
										{
											numList.push_front( curNum-(curNum/10)*10 );
											curNum = curNum/10;
										}

										if ( curNum > 0 )
											numList.push_front( curNum );

										//Render num img list
										stRectI	rcSkill;
										rcSkill.left = m_pBtnAccessKey[i]->GetLocation().x + m_x;
										rcSkill.top = m_pBtnAccessKey[i]->GetLocation().y + m_y;
										rcSkill.right = rcSkill.left + 35;
										rcSkill.bottom = rcSkill.top + 35;

										std::list<int>::iterator it;
										for( it=numList.begin();it!=numList.end();it++ )
										{
											if ( (*it >= 0) && (*it < 10) )
											{
												if ( m_itemNumImg[*it] )
												{
													m_itemNumImg[*it]->Render( rcSkill.left, rcSkill.top );
													rcSkill.left += m_itemNumImg[*it]->GetWidth();
													rcSkill.right += m_itemNumImg[*it]->GetWidth();
												}
											}
										}
									}
								}
								else
								{
									//Delete this access key
									GetGameAccelKeyManager()->DeleteOneAccelKey( key, m_curAccessKeyGroup );
									RefreshKeyGroupState( m_curAccessKeyGroup );
								}
						}

						//Disable item icon when current equipment is equiped
						if ( type == GameAccelKeyType_Equipment )
						{
							if ( pItem->GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP )
								m_pBtnAccessKey[i]->SetEnabled( false );
						}
					}
					else
					{
						//Delete this access key
						GetGameAccelKeyManager()->DeleteOneAccelKey( key, m_curAccessKeyGroup );
						RefreshKeyGroupState( m_curAccessKeyGroup );
					}
				}
			}
		}
	}
	return S_OK;

	FUNCTION_END;
}

/**
* \brief 绘制对话框背景
* 
* 
* 
* \param fElapsedTime : 
* \return 
*/
void CGuiMain::RenderBackground(float fElapsedTime)
{
	FUNCTION_BEGIN;

	CGuiDialog::RenderBackground(fElapsedTime);

	/*for(size_t i=0;i<m_pBmpForeground.size();++i)
	{
		m_pBmpForeground[i]->Render(m_x,m_y);
	}*/

	/*if(GetGameState()->IsState(eGameState_Run))
	{
	CMainCharacter* pChar = GetScene()->GetMainCharacter();
	if(pChar)
	{
	char szPos[32];
	sprintf(szPos,"%s[%u,%u]",GetScene()->GetMapName(),pChar->GetGridPos().x,pChar->GetGridPos().y);
	GetDevice()->DrawString(szPos,stPointI(GetX() + 886,GetY() + 744),-1,FontEffect_Border);
	}
	}*/

	FUNCTION_END;
}

/**
* \brief 获取鼠标所指快捷键编号
* 
* 
* 
* \return 快捷键编号
*/
int CGuiMain::GetMouseOverAccessKeyNo()
{
	FUNCTION_BEGIN;

	if ( GetGuiManager()->GetMouseOverDlg() != this )
		return -1;
	//TRACE( "!!!!!!!!!!!!!!!!!!!!!!!!!MainDlgIsMouseOver!!!!!!!!!!!!!!!!!!\n" );
#if 0
	stPointI ptCursor = Engine_GetCursor()->GetPosition();
	ptCursor.x -= GetClientX();
	ptCursor.y -= GetClientY();

	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		if ( m_pBtnAccessKey[i]->ContainsPoint( ptCursor ) )
		{
			return i;
			break;
		}
	}
#else
	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		if ( NULL == m_pBtnAccessKey[i] )
			continue;

		if ( m_pBtnAccessKey[i]->IsMouseOver() )
		{
			//if ( !m_bAccessKeyToolTipsNeedUpdate[i] )
			{
				//all need update to false
				/*for( int n=0;n<MAIN_ACCESS_KEY_NUM;n++ )
				m_bAccessKeyToolTipsNeedUpdate[n] = false;*/

				UpdateKeyTooltips( (BYTE)i );
				//m_bAccessKeyToolTipsNeedUpdate[i] = true;
			}
			return i;
			break;
		}
	}
#endif

	/*for( i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	m_bAccessKeyToolTipsNeedUpdate[i] = false;*/

	return -1;

	FUNCTION_END;
}

/**
* \brief 绘制快捷键所指技能冷却效果
* 
* 
* 
* \param fElapsedTime : 
* \return 
*/
HRESULT CGuiMain::RenderAccessKeyBtnDisableStatus( float fElapsedTime )
{
	FUNCTION_BEGIN;

	//Get offset cause of different screen size
	stPointI ptOffset;
	ptOffset.x += m_x + 15;
	ptOffset.y += m_y + 10;

	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		enumGameAccelKeyType type;
		DWORD id;
		BYTE key[4];
		key[0] = VK_F1 + i;
		key[1] = 0;

		if ( GetGameAccelKeyManager()->GetAccessKeyIdAndType( key, m_curAccessKeyGroup, type, id ) )
		{
			switch( type )
			{
			case GameAccelKeyType_Item:	
			case GameAccelKeyType_Equipment:
				if ( m_pBtnAccessKey[i] )
				{
					CRoleItem* pItem = NULL;
					if ( type == GameAccelKeyType_Item )
					{
						pItem = GetScene()->GetMainCharacter()->FindItemByID( id );
						if ( (NULL == pItem) || !pItem->IsAccessNeedThisID() )
							pItem = GetScene()->GetMainCharacter()->FindItemByConstID( id );
					}
					else if ( type == GameAccelKeyType_Equipment )
						pItem = GetScene()->GetMainCharacter()->FindItemByID( id );

					//Add used sing img
					if ( pItem )
					{
						if ( (pItem->GetObjectID() == c_nXuanYuanFu)
							|| (pItem->GetObjectID() == c_nXianTianFu)
							|| (pItem->GetObjectID() == 666) )
						{
							if ( pItem->GetObject()->maker[0] != 0 )	//Used
							{
								IBitmaps* pBmp=NULL;
								pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,300)));
								if ( pBmp )
								{
									stRectI	rcSkill;
									rcSkill.left = m_pBtnAccessKey[i]->GetLocation().x + m_x;
									rcSkill.top = m_pBtnAccessKey[i]->GetLocation().y + m_y;
									rcSkill.right = rcSkill.left + 35;
									rcSkill.bottom = rcSkill.top + 35;
									pBmp->RenderAni( rcSkill.left-2-56, rcSkill.top-2-56, xtimeGetTime(), -1, Blend_Null);
								} 
							}
						}
					}
				}
				break;
			case GameAccelKeyType_Skill:
				if ( m_pBtnAccessKey[i] )			//
				{
					SkillBase_t* pSkill = GetSkillBase( id );
					if ( pSkill )
					{
						//TRACE( "Draw skill disable status once\n" );
						stRectI	rcSkill;
						rcSkill.left = m_pBtnAccessKey[i]->GetLocation().x + m_x;
						rcSkill.top = m_pBtnAccessKey[i]->GetLocation().y + m_y;
						rcSkill.right = rcSkill.left + 35;
						rcSkill.bottom = rcSkill.top + 35;
						GetScene()->GetMainCharacter()->DrawSkillDisableStatus( pSkill, &rcSkill );		//( m_pBtnAccessKey[i]->GetBoundingBox() )

						//Draw skill select sign
						if ( GetScene()->GetMainCharacter()->m_pActiveSkill == pSkill )
						{
							//Draw
							if ( m_activeSkillSign )
							{
								m_activeSkillSign->RenderAni( rcSkill.left-4, rcSkill.top-4, xtimeGetTime(), -1 );
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}

	return S_OK;

	FUNCTION_END;
}

/**
* \brief 绘制快捷键提示信息
* 
* 
* 
* \param fElapsedTime : 
* \return 
*/
HRESULT CGuiMain::RenderAccessKeyBtnToolTips( float fElapsedTime )
{
	FUNCTION_BEGIN;

	if ( (m_mouseOverAccessKeyNo < 0) || (m_mouseOverAccessKeyNo >= MAIN_ACCESS_KEY_NUM ) )
		return S_OK;

	//Just render mouse on btn tooltips
	//Get mouse btn access key type
	enumGameAccelKeyType type;
	DWORD id;
	BYTE key[4];
	key[0] = VK_F1 + m_mouseOverAccessKeyNo;
	key[1] = 0;
	if ( GetGameAccelKeyManager()->GetAccessKeyIdAndType( key, m_curAccessKeyGroup, type, id ) )
	{
		//Get offset cause of different screen size
		stPointI pt;
		pt = m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetLocation();
		pt.x += m_x;
		pt.y += m_y;

		pt.x += 0;
		pt.y += -10;		

		switch( type )
		{
		case GameAccelKeyType_Item:
		case GameAccelKeyType_Equipment:
			{
				CRoleItem* pItem = NULL;
				if ( type == GameAccelKeyType_Item )
				{
					pItem = GetScene()->GetMainCharacter()->FindItemByID( id );
					if ( (NULL == pItem) || !pItem->IsAccessNeedThisID() )
						pItem = GetScene()->GetMainCharacter()->FindItemByLocationConstID( id, OBJECTCELLTYPE_COMMON );
				}
				else if( type == GameAccelKeyType_Equipment )
					pItem = GetScene()->GetMainCharacter()->FindItemByID( id );

				if ( pItem )
				{
					pt.x -= pItem->GetToolTips()->GetWidth();
					pt.y -= pItem->GetToolTips()->GetHeight();

					if ( pt.x < 0 )
						pt.x = m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetLocation().x + m_x + m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetWidth()/2;

					pItem->ShowToolTips( pt );
				}
			}
			break;
		case GameAccelKeyType_Skill:
			{
				if ( m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo] && m_pBtnAccessKey[m_mouseOverAccessKeyNo] )
				{
					pt.x -= m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->GetWidth();
					pt.y -= m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->GetHeight();

					if ( pt.x < 0 )
						pt.x = m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetLocation().x + m_x + m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetWidth()/2;

					//Set the same type as item
					m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->SetBkColor(COLOR_ARGB(128,0,0,0));

					if ( NULL == GetGuiManager()->GetToolTips() )
						GetGuiManager()->SetToolTips( m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo], pt );

					//restore
					//m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->SetBkColor(COLOR_ARGB(0,255,255,255));
				}				
			}
			break;
		case GameAccelKeyType_Face:
			{
				if ( m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo] && m_pBtnAccessKey[m_mouseOverAccessKeyNo] )
				{
					pt.x -= m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->GetWidth();
					pt.y -= m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->GetHeight();

					if ( pt.x < 0 )
						pt.x = m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetLocation().x + m_x + m_pBtnAccessKey[m_mouseOverAccessKeyNo]->GetWidth()/2;

					//Set the same type as item
					m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo]->SetBkColor(COLOR_ARGB(128,0,0,0));

					if ( NULL == GetGuiManager()->GetToolTips() )
						GetGuiManager()->SetToolTips( m_pBtnAccessKeyToolTips[m_mouseOverAccessKeyNo], pt );
				}				
			}
			break;
		default:
			break;
		}
	}

	return S_OK;

	FUNCTION_END;
}

/**
* \brief 绘制对话框
* 
* 
* 
* \param fElapsedTime : 
* \return 
*/
HRESULT CGuiMain::OnRender( float fElapsedTime )
{
	FUNCTION_BEGIN;

	GetChatManager().UpdateAllInvite(fElapsedTime);

	//Draw repair list
	stPointI point(GetDevice()->GetWidth() - 200,GetDevice()->GetHeight() - 100);
	if ( GetScene()->GetMainCharacter() )
	{
		GetScene()->GetMainCharacter()->DrawNeedRepairItemList(point);
	}

	DrawScreenRightBottomMsg(point);
	DrawScreenCenterMsg();
	DrawTaskTimeMessage( stPointI(GetDevice()->GetWidth() - 400,20) );

	m_mouseOverAccessKeyNo = GetMouseOverAccessKeyNo();

	//Render dialog
	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	hr &= RenderAccessKeyBtnDisableStatus( fElapsedTime );
	hr &= RenderAccessKeyNotes( fElapsedTime );

	if( GetGuiManager()->GetMouseOverDlg() == this)
	{
		hr &= RenderAccessKeyBtnToolTips( fElapsedTime );
	}
	

	CMainCharacter* pMainChar = GetScene()->GetMainCharacter();

	
	if( pMainChar->IsDie())
	{
		RenderBar(m_aBar[0],0,pMainChar->GetMaxHP(),0);
		RenderBar(m_aBar[1],0,pMainChar->GetMaxMP(),1);
		//RenderBar(m_aBar[2],0,pMainChar->GetMaxSP(),2);
		RenderBar(m_aBar[2],0,pMainChar->GetNextexp(),2);   
	}
	else
	{
		RenderBar(m_aBar[0],pMainChar->GetHP(),pMainChar->GetMaxHP(),0);
		RenderBar(m_aBar[1],pMainChar->GetMP(),pMainChar->GetMaxMP(),1);
		//RenderBar(m_aBar[2],pMainChar->GetSP(),pMainChar->GetMaxSP(),2);
		RenderBar(m_aBar[2],pMainChar->GetExp(),pMainChar->GetNextexp(),2);

		//Render state list
		RenderStateList( fElapsedTime );

	}

	//Draw main character level
	char level[10];
	int nLevel = pMainChar->GetLevel();				//
	sprintf( level, "%d", nLevel );			
	stPointI pt(82,8);
	int offset = 0;
	if ( nLevel<10 )
		offset = 0;
	else if( nLevel< 100 )
		offset = -3;
	else
		offset = -5;

	pt.x += offset;

	stRectI rcLevel;
	rcLevel.left = pt.x - 5;
	rcLevel.right = rcLevel.left + 20;
	rcLevel.top = pt.y - 5;
	rcLevel.bottom = rcLevel.top + 20;

	GetDevice()->DrawString( level, pt );

	RenderTeamFlag();

	m_bShowTeamToolTip = false;
	RenderTeamTip();

	if(this ==  GetGuiManager()->GetMouseOverDlg())
	{
		if( !m_bShowTeamToolTip) 
		{
			if ( rcLevel.PtInRect( Engine_GetCursor()->GetPosition() ) )		//Render tips
			{

				//CToolTips tips;
				m_ExpTips.Clear();
				m_ExpTips.SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
				m_ExpTips.SetBorderColor( 0 );
				m_ExpTips.SetAlignType( GUI_ALIGN_LEFT );
				m_ExpTips.SetLineGaps( 3 );
				m_ExpTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
				m_ExpTips.AddText( "当前等级  等级升高会相应提升自身的属性" );
				m_ExpTips.Resize();

				if ( GetGuiManager()->GetToolTips() == NULL )
					GetGuiManager()->SetToolTips( &m_ExpTips, stPointI(pt.x+20,pt.y) );
			}
			//if(GetGameGuiManager()->m_guiTeam->m_aTeamMember.size())
			//{
			//	m_menu->SetEnabled(false);
			//}
			//else
			//{
			//	m_menu->SetEnabled(false);
			//}



			// Draw the icon
			if( m_rcIconClip.PtInRect( Engine_GetCursor()->GetPosition() ) )
			{
				CToolTips tips;
				tips.SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
				tips.SetBorderColor( 0 );
				tips.SetAlignType( GUI_ALIGN_LEFT );
				tips.SetLineGaps( 3 );
				tips.SetCurColor( D3DCOLOR_ARGB(255,152,244,23) );

				CMainCharacter* pMainChar = GetScene()->GetMainCharacter();

				char szValue[80];
				if( pMainChar->IsDie() )
				{
					sprintf(szValue,"生命值：%d/%d",0,pMainChar->GetMaxHP());
					tips.AddText(szValue);
					sprintf(szValue,"\n法术值：%d/%d",0,pMainChar->GetMaxMP());
					tips.AddText(szValue);
				}
				else
				{
					sprintf(szValue,"生命值：%d/%d",pMainChar->GetHP(),pMainChar->GetMaxHP());
					tips.AddText(szValue);
					sprintf(szValue,"\n法术值：%d/%d",pMainChar->GetMP(),pMainChar->GetMaxMP());
					tips.AddText(szValue);
				}

				tips.Resize();
				tips.Render( m_rcIconClip.right, m_rcIconClip.bottom );
			}	
		}
	}
	


        ///////////////////////////////////////////////////////////////////////////
	if( !GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_HELPTIP ) )
	{
		ShowAllTipBtn(false);
	}
	///////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////
        // GM公告滚动显示
	if( m_pTextSprite.get() != NULL )
	{
		m_pTextSprite->UpdateSprite(fElapsedTime);
		m_pTextSprite->RenderSprite();
	}
	/////////////////////////////////////////

	m_aniMain.Render(m_showX,m_showY);

	WORD points = GetScene()->GetMainCharacter()->GetProperty()->points;
	if (points == 0)
	{
		AddshuxBtn(false);
	}

	//static float st_flash = 0.9f;
	//static float st_flash2 = -1.0f;
	//static bool  st_dir = 0;
	//if (GetScene()->GetMainCharacter()->GetTaskList()->bIsTaskExist(2))
	//{
	//	//任务L图标的闪动
	//	
	//	if(st_flash >= 0.1f)
	//	{	
	//		GetButton(18)->PlayFlash(st_flash);
	//		if (st_dir)
	//		{
	//			st_flash += 0.03f;
	//			if (st_flash >= 0.9f) 
	//			{
	//				st_flash = 0.9f;
	//				st_dir =!st_dir;
	//			}
	//		}
	//		else
	//		{
	//			st_flash -= 0.03f;
	//			if (st_flash <= 0.1f) 
	//			{
	//				st_flash = 0.1f;
	//				st_dir =!st_dir;
	//			}
	//		}
	//		
	//		
	//	}

	//}
	//else if (st_flash2 <= 0.0f)	
	//{
	//	GetButton(18)->StopFlash();
	//	st_flash2 = -1.0f;
	//	st_flash = -1.0f;
	//}
	
	if (m_bEnabledSysmessage)
	{
		OnUpdateSysmessage(fElapsedTime);
	}

	static DWORD CanPrintCentertick = 0;
	static bool isOnce = true; 
	if( isOnce && CanPrintCentertick == 0)
	{
		CanPrintCentertick = xtimeGetTime();
		return hr;
	}
	else 
	{
		if( xtimeGetTime() - CanPrintCentertick > 5000)
		{
			CanPrintCenter = true;
			isOnce = false;
		}
	}

	return hr;

	FUNCTION_END;
}

void CGuiMain::RenderTeamFlag()
{
	FUNCTION_BEGIN;

	DWORD dwTeamState = GetScene()->GetMainCharacter()->GetTeamState();

	if( dwTeamState == TEAM_STATE_LEADER || g_bSinglePlayer)
	{		
		if( m_pTeamHeadIcon )
			m_pTeamHeadIcon->RenderAni(g_ptTeamHead.x,g_ptTeamHead.y,xtimeGetTime(),-1,Blend_Null);
		bool bRenderFlag = GetGameGuiManager()->m_guiTeam && GetGameGuiManager()->m_guiTeam->m_dwTeamMode == TEAM_NORMAL;
		if( bRenderFlag )
		{
			if( GetGameGuiManager()->m_guiTeam->m_dwCurExpAssign == TEAM_EXP_MODE_NORMAL || g_bSinglePlayer){
				if(m_pTeamExpAssignByLevel) m_pTeamExpAssignByLevel->RenderAni(g_ptExpAssgin.x,g_ptExpAssgin.y,xtimeGetTime(),-1,Blend_Null);
			}
			else if (GetGameGuiManager()->m_guiTeam->m_dwCurExpAssign == TEAM_EXP_MODE_SPECIAL || g_bSinglePlayer){
				if(m_pTeamExpAssignBySpecial) m_pTeamExpAssignBySpecial->RenderAni(g_ptExpAssgin.x,g_ptExpAssgin.y,xtimeGetTime(),-1,Blend_Null);
			}
			if(GetGameGuiManager()->m_guiTeam->m_dwCurObjAssign == TEAM_OBJ_MODE_NORMAL || g_bSinglePlayer ){
				if(m_pTeamItemAssignByFreedom ) m_pTeamItemAssignByFreedom->RenderAni(g_ptObjAssgin.x,g_ptObjAssgin.y,xtimeGetTime(),-1,Blend_Null);
			}
			else if(GetGameGuiManager()->m_guiTeam->m_dwCurObjAssign == TEAM_OBJ_MODE_SPECIAL || g_bSinglePlayer){
				if( m_pTeamItemAssignByPoll ) m_pTeamItemAssignByPoll->RenderAni(g_ptObjAssgin.x,g_ptObjAssgin.y,xtimeGetTime(),-1,Blend_Null);
			}
		}
	}

	FUNCTION_END;
}

void CGuiMain::RenderTeamTip()
{
	FUNCTION_BEGIN;

	bool bRenderTip= GetGameGuiManager()->m_guiTeam && GetGameGuiManager()->m_guiTeam->m_dwTeamMode == TEAM_NORMAL;
	DWORD dwTeamState = GetScene()->GetMainCharacter()->GetTeamState();
	if( bRenderTip && dwTeamState == TEAM_STATE_LEADER || g_bSinglePlayer)
	{
		if( !g_bSinglePlayer)
			if( !GetGameGuiManager()->m_guiTeam || GetGameGuiManager()->m_guiTeam->m_aTeamMember.size() <= 0 )  return;
		DWORD dwCurExpMode = GetGameGuiManager()->m_guiTeam->m_dwCurExpAssign;

		stRectI rcTest = m_rcClipTeamExpAssignByLevel;
		rcTest.OffsetRect(g_ptExpAssgin.x,g_ptExpAssgin.y);

		bool bShowTip = rcTest.PtInRect( Engine_GetCursor()->GetPosition()  );

		if(  bShowTip)
		{
			CToolTips tips;
			tips.SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
			tips.SetBorderColor( 0 );
			tips.SetAlignType( GUI_ALIGN_LEFT );
			tips.SetLineGaps( 3 );
			tips.SetCurColor( D3DCOLOR_ARGB(255,152,244,23) );
			char szValue[80];
			if( dwCurExpMode ==  TEAM_EXP_MODE_NORMAL)
				strcpy(szValue,"等级分配:按个人等级占整个团队\n等级的百分比来分配经验");
			else  
				strcpy(szValue,"特殊分配:个人杀怪获得100%经验，\n其他每个队员获得10%经验");
			szValue[79] = 0;
			tips.AddText(szValue); 
			tips.Resize();
			tips.Render( rcTest.right, rcTest.bottom );

			m_bShowTeamToolTip = true;
		}	

		rcTest = m_rcClipTeamItemAssignByFreedom;
		rcTest.OffsetRect(g_ptObjAssgin.x,g_ptObjAssgin.y);
		bShowTip = rcTest.PtInRect( Engine_GetCursor()->GetPosition() );

		DWORD dwCurObjMode = GetGameGuiManager()->m_guiTeam->m_dwCurObjAssign;

		if(  bShowTip)
		{
			CToolTips tips;
			tips.SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
			tips.SetBorderColor( 0 );
			tips.SetAlignType( GUI_ALIGN_LEFT );
			tips.SetLineGaps( 3 );
			tips.SetCurColor( D3DCOLOR_ARGB(255,152,244,23) );
			char szValue[80];
			if( dwCurObjMode == TEAM_OBJ_MODE_NORMAL)
				strcpy(szValue,"自由拾取:物品无分配，可自由拾取");
			else  
				strcpy(szValue,"轮流拾取:物品轮流分配，分配的物品可拾取");
			szValue[79] = 0;
			tips.AddText(szValue); 
			tips.Resize();
			tips.Render( rcTest.right, rcTest.bottom );
			m_bShowTeamToolTip = true;
		}	

	}

	FUNCTION_END;
}
/**
* \brief 重设对话框尺寸
* 
* 
* 
* \return 
*/
void CGuiMain::OnResetScreenSize()
{
	FUNCTION_BEGIN;
	//*
	CGuiDialog::OnResetScreenSize();

	GetChatManager().UpdateHideChatButtonState();

	//Reset access key btn position
	if ( GetDevice()->GetWidth() == 800 )
	{
		g_nMainAccessBtnLeft = 3;	
		g_nMainAccessBtnTop = 560;

		if (GetTable(300))
		{
			GetTable(300)->SetLocation(725,560);
			GetTable(300)->ResetItemRect(stRectI(725,560,725+33,560+33));
			GetTable(300)->UpdateItemPos();
		}

		if (GetTable(301))
		{
			GetTable(301)->SetLocation(765,560);
			GetTable(301)->ResetItemRect(stRectI(765,560,765+33,560+33));
			GetTable(301)->UpdateItemPos();
		}
	}
	else
	{
		g_nMainAccessBtnLeft = 21;
		g_nMainAccessBtnTop = 728;

		if (GetTable(300))
		{
			GetTable(300)->SetLocation(928,728);
			GetTable(300)->ResetItemRect(stRectI(928,728,928+33,728+33));
			GetTable(300)->UpdateItemPos();
		}

		if (GetTable(301))
		{
			GetTable(301)->SetLocation(968,728);
			GetTable(301)->ResetItemRect(stRectI(968,728,968+33,728+33));
			GetTable(301)->UpdateItemPos();
		}
	}

	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		m_pBtnAccessKey[i] = GetButton( 100+i );
		if ( m_pBtnAccessKey[i])
		{
			m_pBtnAccessKey[i]->SetLocation( g_nMainAccessBtnLeft+i*g_MainAccessBtnSpace, g_nMainAccessBtnTop );
		}
	}

	stResourceLocation rl;
	rl.SetFileName( "data\\Interfaces.gl" );

	int grp;

	if ( GetDevice()->GetWidth() == 800 )
	{
		m_x = 0;
		m_y = 0;
		grp = 101;
		
		//We reset control img
		for( int i=2;i<250;i++ )
		{
			if ( GetButton( i ) )
			{
				if ( (i == 200)
					|| (i == 201)
					|| ( (i>=20) && (i<=24) ) )			//First we remove some control
				{
					if ( GetButton( i ) )
						GetButton( i )->SetVisible( false );
					continue;
				}
				int num = 0;
				GetButton( i )->GetBitmapInfo( rl, num );
				rl.group =grp;
				if ( abs(num) > 100 )
					num = 0;

				GetButton( i )->SetBitmap( rl, num );
				continue;
			}
			rl.SetFileName( "data\\Interfaces.gl" );

			if ( GetImage( i ) )
			{
				rl.group = grp;
				rl.frame = GetImage( i )->GetImgFrame();
				GetImage( i )->SetImage( rl );
			}
		}

		//Reset back image
		if ( GetBackImage() )
		{
			rl.group = grp;
			rl.frame = GetBackImage()->GetImgFrame();
			SetBackImage( rl );
			m_width = 800;
			m_height = 600;
			m_x = 0;
			m_y = 0;
		}
	}
	else
	{
		grp = 1;

		//We reset control img
		for( int i=2;i<250;i++ )
		{
			if ( GetButton( i ) )
			{
				if ( (i == 200)
					|| (i == 201)
					|| ( (i>=20) && (i<=24) ) )		//We re-show some control
				{
					if ( GetButton( i ) )
						GetButton( i )->SetVisible( true );
					continue;
				}
				int num = 0;
				GetButton( i )->GetBitmapInfo( rl, num );
				rl.group = grp;
				if ( abs(num) > 100 )
					num = 0;

				GetButton( i )->SetBitmap( rl, num );
				continue;
			}
			rl.SetFileName( "data\\Interfaces.gl" );

			if ( GetImage( i ) )
			{
				rl.group = grp;
				rl.frame = GetImage( i )->GetImgFrame();
				GetImage( i )->SetImage( rl );
			}
		}

		//Reset back image
		if ( GetBackImage() )
		{
			rl.group = grp;
			rl.frame = GetBackImage()->GetImgFrame();
			SetBackImage( rl );
			m_width = 1024;
			m_height = 768;
			m_x = (GetDevice()->GetWidth() - m_width) / 2;
			m_y = GetDevice()->GetHeight() - m_height;
		}	
	}

	//Reset bar img
	IBitmapList* pBmpList = GetDevice()->FindResource(GetGuiGraphicPackName());
	m_aBar.resize(0);
	stBarShowInfo bar;
	m_aBar.reserve(3);
	bar.pBmp = pBmpList->LoadFrame(grp,243);
	if(bar.pBmp)
		bar.rcClip = bar.pBmp->GetClipRect();
	else
		bar.rcClip = stRectI(0,0,0,0);

	bar.type = BarTypeVert;
	m_aBar.push_back(bar);

	bar.pBmp = pBmpList->LoadFrame(grp,244);
	if(bar.pBmp)
		bar.rcClip = bar.pBmp->GetClipRect();
	else
		bar.rcClip = stRectI(0,0,0,0);
	bar.type = BarTypeVert;
	m_aBar.push_back(bar);

	/*bar.pBmp = pBmpList->LoadFrame(grp,59);
	bar.rcClip = bar.pBmp->GetClipRect();
	bar.type = BarTypeVert;
	m_aBar.push_back(bar);*/

	bar.pBmp = pBmpList->LoadFrame(grp,61);
	bar.rcClip = bar.pBmp ? bar.pBmp->GetClipRect() : stRectI(0,0,0,0);
	bar.type = BarTypeHoriz;
	m_aBar.push_back(bar);

	if ( m_imgThreeValue )
	{
		m_imgThreeValue->SetLocation(-m_x,-m_y);
	}

	GetImage( 2 )->SetLocation( -m_x, -m_y );
	if ( GetButton(16) )
	{
		GetButton(16)->SetLocation( -10000,-10000 );
	}

	if ( GetButton( 10 ) )
	{
		GetButton( 10 )->SetLocation( -10000, -10001 );
	}

	//set icon pos
	m_ptIcon.x = 12 - m_x;
	m_ptIcon.y = 12 - m_y;

	m_aPkModeBtnList.SetLocation( stPointI(-m_x+100,-m_y+10) );
	m_aPkModeBtnList.ResetBtnLocation();
	//Set PK mode btn pos;
	ChangeActivePkModeBtn();

	RefreshKeyGroupState( m_curAccessKeyGroup );
	UpdateChatBtn();
	UpdateHelpBtnState();


	g_ptTeamHead.x = 0;
	g_ptTeamHead.y = 20;

	g_ptExpAssgin.x = 0;
	g_ptExpAssgin.y = 40;

	g_ptObjAssgin.x = 0;
	g_ptObjAssgin.y = 60;

	if (m_pshuxBtn)
	{
		int x = GetDevice()->GetWidth() - 42 - GetClientX();
		int y = GetDevice()->GetHeight()/2 - GetClientY();
		m_pshuxBtn->pBtn->SetLocation(x,y);
	}

	FUNCTION_END;
}

DWORD CGuiMain::OnHiTest(POINT ptScreen)
{
	stRectI rc;
	rc.left = 0;
	rc.top =  0 ;
	rc.right = 86;
	rc.bottom =  86;

	CGuiImage * pImage = GetImage(3);
	if( pImage && pImage->ContainsPoint(ptScreen))
		return HTCLIENT;
	return CGuiDialog::OnHiTest(ptScreen);
}
/**
* \brief 更新聊天类型按钮
* 
* 
* 
* \return 
*/
void CGuiMain::UpdateChatBtn()
{
	FUNCTION_BEGIN;

	int nY = 24;
	int iGroup;
	if(m_iScreenWidth == 800)
	{
		iGroup = 5;
	}
	else
	{
		iGroup = 5;
	}
	stResourceLocation rl;
	rl.group = iGroup;
	rl.frame = -1;
	rl.SetFileName(GetGuiGraphicPackName());
	for(size_t i=0;i<count_of(g_chatTypeInfo);++i)
	{
		if(g_chatTypeInfo[i].chatType == m_iChatType)
		{
			rl.frame = g_chatTypeInfo[i].imgFrame;
			break;
		}
	}

	FUNCTION_END;
}


/**
* \brief 设置聊天类型
* 
* 
* 
* \param iChatType : 聊天类型
* \return 
*/
void CGuiMain::SetChatType(enumChatType iChatType)
{
	FUNCTION_BEGIN;

	m_iChatType = iChatType;
	UpdateChatBtn();

	FUNCTION_END;
}


void CGuiMain::__SetEmotionAccessBtn(stResourceLocation & rl,int btnId)
{
	FUNCTION_BEGIN;

	/////////////////////////////////////////////////////////////////////////////////
	//Safe Checking 
	if( btnId < 0 || btnId >= MAIN_ACCESS_KEY_NUM)
		return;
	/////////////////////////////////////////////////////////////////////////////////
	stPointI ptOffset(0,0);
	IBitmaps * pBmps = GetDevice()->FindBitmaps(&rl);
	stRectI rcClip(0,0,0,0);
	stRectI rcBitmap(0,0,0,0);
	if( pBmps)
	{
		IBitmap * pBitmap = pBmps->GetBitmap(0);
		if (pBitmap)
		{
			rcClip = pBitmap->GetClipRect();
			rcBitmap.right = pBitmap->GetWidth();
			rcBitmap.bottom = pBitmap->GetHeight();
		}
	}
	stPointF scale(1.0f,1.0f);
	if( rcClip.Width() > 34 || rcClip.Height() > 34)
	{
		scale.x = (float)34 / (float)rcClip.Width();
		scale.y = (float)34 / (float)rcClip.Height();  
		if( scale.x < scale.y )
			scale.y = scale.x;
		else
			scale.x = scale.y;
	}		
	ptOffset.x = (34 - rcClip.Width() * scale.x )/2   - rcClip.left * scale.x;
	ptOffset.y = (34 - rcClip.Height() * scale.y )/2  - rcClip.top * scale.y;
	GetGameGuiManager()->m_guiMain->m_pBtnAccessKey[btnId]->SetLocation(g_nMainAccessBtnLeft + btnId*g_MainAccessBtnSpace+ptOffset.x, g_nMainAccessBtnTop+ptOffset.y);
	GetGameGuiManager()->m_guiMain->m_pBtnAccessKey[btnId]->SetBitmapScale( scale );
	GetGameGuiManager()->m_guiMain->m_pBtnAccessKey[btnId]->SetBitmap( rl, 1 );

	FUNCTION_END;
}
/**
* \brief 设置快捷键
* 
* 
* 
* \param keyID : 快捷键ｉｄ　
* \return 成功则返回ｔｒｕｅ，　否则返回ｆａｌｓｅ
*/
bool CGuiMain::SetAccessKey( const int& keyID )
{
	FUNCTION_BEGIN;

	if ( (keyID >= MAIN_ACCESS_KEY_NUM) || (keyID < 0) )
	{
		RestoreMouseIcon();
		return false;
	}

	if( NULL == m_pBtnAccessKey[keyID] )
	{
		RestoreMouseIcon();
		return false;
	}

	enumActiveAccessType type;
	if ( !IsAnyActiveForAccessKeySetting( type ) )
	{
		RestoreMouseIcon();
		return false;
	}
	// Reset the Btn Pos ,because it may be moved in the following
	m_pBtnAccessKey[keyID]->SetLocation( g_nMainAccessBtnLeft+keyID*g_MainAccessBtnSpace, g_nMainAccessBtnTop );

	stResourceLocation rl;
	switch( type )
	{
	case ActiveAccessSkill:
		{
			if ( GetGameGuiManager()->m_guiSkill->GetActiveSkillIconResource( rl ) )
			{
				m_pBtnAccessKey[keyID]->SetBitmapScale( skillIconScale );
				m_pBtnAccessKey[keyID]->SetBitmap( rl, 3, dwBtnState );

				//Save access key change
				stGameAccelKey key;
				key.type = GameAccelKeyType_Skill;
				key.dwID = GetGameGuiManager()->m_guiSkill->GetActiveSkillIconID();
				key.szAccelKey[0] = VK_F1 + keyID;
				key.szAccelKey[1] = 0;
				key.mGroup = (BYTE)m_curAccessKeyGroup;

				GetGameAccelKeyManager()->SetKey(key);
				GetGameAccelKeyManager()->Save( m_curAccessKeyGroup );

				//Restore mouse icon as default
				GetGameGuiManager()->m_guiSkill->SetActiveSkillIcon();
				RestoreMouseIcon();

				return true;
			}
		}
		break;
	case ActiveAccessItem:
	case ActiveAccessEquipment:
		{
			if ( GetFloatItemResource( rl ) )
			{
				m_pBtnAccessKey[keyID]->SetBitmapScale( itemIconScale );
				m_pBtnAccessKey[keyID]->SetBitmap( rl, 1 );

				//Save access key change
				stGameAccelKey key;
				key.type = (type == ActiveAccessItem ) ? GameAccelKeyType_Item : GameAccelKeyType_Equipment;
				key.dwID = GetFloatItemThisID(type == ActiveAccessEquipment);
				key.szAccelKey[0] = VK_F1 + keyID;
				key.szAccelKey[1] = 0;
				key.mGroup = (BYTE)m_curAccessKeyGroup;

				GetGameAccelKeyManager()->SetKey(key);
				GetGameAccelKeyManager()->Save( m_curAccessKeyGroup );

				//Move back float item
				MoveBackFloatItem();
				RestoreMouseIcon();

				return true;
			}
		}
		break;
	case ActiveAccessEmotion:
		{     
			ICON_ACCESS_COME_FROM _from = GetEmotionInfo()->GetComeFrom();
			if( FROM_FLOAT == _from || FROM_MAIN == _from )
			{
				if ( GetGameAccelKeyManager() && GetGameAccelKeyManager()->m_curEditAccessKey.IsValid() )
				{
					EditAccessKeyBtn(keyID);
				}
			}			
			else  if( FROM_EMOTION == _from)
			{
				if( GetEmotionInfo()->GetActiveResourceLocation(rl) )
				{
					__SetEmotionAccessBtn(rl,keyID);

					//Save access key change
					stGameAccelKey key;
					key.type = GameAccelKeyType_Face;
					key.dwID = GetEmotionInfo()->GetActiveEmotionIcon();
					key.szAccelKey[0] = VK_F1 + keyID;
					key.szAccelKey[1] = 0;
					key.mGroup = (BYTE)m_curAccessKeyGroup;  // must not be EMOTION_ACCESS_KEY_GROUP

					GetGameAccelKeyManager()->SetKey(key);
					GetGameAccelKeyManager()->Save( m_curAccessKeyGroup);	  

					//Restore mouse icon as default
					GetEmotionInfo()->ClearActiveEmotionIcon();
					RestoreMouseIcon();
				}

			}
		}
		break;
	default:
		break;
	}

	RestoreMouseIcon();
	return false;

	FUNCTION_END;
}

/**
* \brief 刷新快捷键提示信息
* 
* 
* 
* \param btnID : 快捷键ｉｄ
* \return 
*/
void CGuiMain::UpdateKeyTooltips( const BYTE& btnID )
{
	FUNCTION_BEGIN;

	if ( (btnID < 0) || (btnID>=MAIN_ACCESS_KEY_NUM ) )
		return;

	DWORD id;
	enumGameAccelKeyType type;
	BYTE key[4];
	key[0] = VK_F1+btnID;
	key[1] = 0;

	if ( GetGameAccelKeyManager()->GetAccessKeyIdAndType( key, (BYTE)m_curAccessKeyGroup, type, id ) )
	{
		switch( type )
		{
		case GameAccelKeyType_Skill:
			{
				if ( NULL == GetGameGuiManager()->m_guiSkill )
				{
					GetGameGuiManager()->AddSkill();
					if ( GetGameGuiManager()->m_guiSkill )
					{
						GetGameGuiManager()->m_guiSkill->SetVisible( false );
						m_pBtnAccessKeyToolTips[btnID] = GetGameGuiManager()->m_guiSkill->GetSkillIconTooltipsFullFromID( id );
						//GetGameGuiManager()->m_guiSkill->OnClose();
					}
				}
				else
					m_pBtnAccessKeyToolTips[btnID] = GetGameGuiManager()->m_guiSkill->GetSkillIconTooltipsFullFromID( id );
			}
			break;
		case GameAccelKeyType_Item:
			{
				//
			}
			break;
		case GameAccelKeyType_Face:
			{
				m_pBtnAccessKeyToolTips[btnID] = GetEmotionInfo()->GetToolTips(id);
			}
			break;
		default:
			break;
		}
	}	

	FUNCTION_END;
}

/**
* \brief 刷新一组快捷键
* 
* 
* 
* \param group : 组
* \return 
*/
void CGuiMain::RefreshKeyGroupState( const int& group  )
{
	FUNCTION_BEGIN;

	if ( group >= MAIN_ACCESS_KEY_GROUP )
		return;

	if ( NULL == GetGameGuiManager()->m_guiSkill )
	{
		GetGameGuiManager()->AddSkill();
		if ( GetGameGuiManager()->m_guiSkill )
			GetGameGuiManager()->m_guiSkill->SetVisible( false );
		else
			return;
	}

	//Update access key btn icon
	for( int i=0;i<MAIN_ACCESS_KEY_NUM;i++ )
	{
		DWORD id;
		enumGameAccelKeyType type;
		BYTE key[4];
		key[0] = VK_F1+i;
		key[1] = 0;

		if ( NULL == m_pBtnAccessKey[i] )
			continue;

		stResourceLocation rl;
		if ( GetGameAccelKeyManager()->GetAccessKeyIdAndType( key, (BYTE)group, type, id ) )
		{
			switch( type )
			{
			case GameAccelKeyType_Skill:
				{
					if ( GetGameGuiManager()->m_guiSkill->GetSkillIconResourceFromID( id, rl ) )
					{
						m_pBtnAccessKey[i]->SetBitmapScale( skillIconScale );
						m_pBtnAccessKey[i]->SetBitmap( rl, 2, dwBtnState );
					}
					else
					{
						m_pBtnAccessKey[i]->SetBitmap( rl, 0 );
					}
				}
				break;
			case GameAccelKeyType_Item:
			case GameAccelKeyType_Equipment:
				{
					CRoleItem* pItem = NULL;
					if ( type == GameAccelKeyType_Item )
					{
						pItem = GetScene()->GetMainCharacter()->FindItemByID( id );
						if ( (NULL == pItem) || !pItem->IsAccessNeedThisID() )
							pItem = GetScene()->GetMainCharacter()->FindItemByConstID( id );
					}
					else if( type == GameAccelKeyType_Equipment )
						pItem = GetScene()->GetMainCharacter()->FindItemByID( id );

					if ( pItem )
					{
						rl = GetItemPicLocation( pItem->GetObject() );
						m_pBtnAccessKey[i]->SetBitmapScale( itemIconScale );
						m_pBtnAccessKey[i]->SetBitmap( rl, 1 );
					}
					else
					{
						m_pBtnAccessKey[i]->SetBitmap( rl, 0 );
						m_pBtnAccessKey[i]->GetToolTips()->Clear();
					}
				}
				break;
			case GameAccelKeyType_Face:
				{
					if ( GetEmotionInfo()->GetResourceLocation( id, rl ) )
					{
						__SetEmotionAccessBtn(rl,i);
					}
					else
					{
						m_pBtnAccessKey[i]->SetBitmap( rl, 0 );
					}
				}
				break;
			default:
				break;
			}
		}
		else		//Cur btn has no access key
		{
			m_pBtnAccessKey[i]->SetBitmap( rl, 0 );
		}

		if ( m_pBtnAccessKey[i]->GetToolTips() )
			m_pBtnAccessKey[i]->GetToolTips()->Clear();

		m_bAccessKeyToolTipsNeedUpdate[i] = false;
	}


	FUNCTION_END;
}

/**
* \brief 设置快捷键组
* 
* 
* 
* \param group : 组
* \return 
*/
bool CGuiMain::SetActiveAccessKeyGroup( BYTE group )
{
	FUNCTION_BEGIN;

	if ( (group < 0) || (group>=MAIN_ACCESS_KEY_GROUP) )
		return false;

	m_curAccessKeyGroup = group;
	RefreshKeyGroupState( m_curAccessKeyGroup );

	return true;

	FUNCTION_END;
}

/**
* \brief 改变快捷键组
* 
* 
* 
* \param  : 往下／上翻
* \return 
*/
void CGuiMain::ChangeCurKeyGroup( bool bDown /* = true  */ )
{
	FUNCTION_BEGIN;

	if ( bDown )		//Add
	{
		m_curAccessKeyGroup++ ;
		if ( m_curAccessKeyGroup >= MAIN_ACCESS_KEY_GROUP )
			m_curAccessKeyGroup -= MAIN_ACCESS_KEY_GROUP;
	}
	else		//Reduce
	{
		m_curAccessKeyGroup-- ;
		if ( m_curAccessKeyGroup < 0 )
			m_curAccessKeyGroup += MAIN_ACCESS_KEY_GROUP;
	}

	if ( GetScene()->GetMainCharacter() )
		GetScene()->GetMainCharacter()->SetPreferAccessGroup( m_curAccessKeyGroup );

	RefreshKeyGroupState( m_curAccessKeyGroup );
	//GetGameAccelKeyManager()->Save( m_curAccessKeyGroup );

	FUNCTION_END;
}


/**
* \brief 确认是否需要添加快捷键
* 
* 
* 
* \param type : 快捷键类型
* \return 是则返回ｔｒｕｅ，　否则返回ｆａｌｓｅ
*/
bool CGuiMain::IsAnyActiveForAccessKeySetting( enumActiveAccessType& type )
{
	FUNCTION_BEGIN;

	//If has active skill

	if ( GetGameGuiManager()->m_guiSkill && GetGameGuiManager()->m_guiSkill->GetActiveSkillIcon() )
	{
		type = ActiveAccessSkill;
		return true;
	}

	//If has valid float item	
	CRoleItem* pRoleItem = CRoleItem::GetFloatRoleItem();
	if( NULL != pRoleItem )
	{
		if ( pRoleItem->IsCanUse()
			&& ( (pRoleItem->GetLocation().dwLocation == OBJECTCELLTYPE_COMMON)) )
		{
			type = ActiveAccessItem;
			return true;
		}

		if ( pRoleItem->IsEquipment() && !pRoleItem->IsCanEquip()
			&& ( (pRoleItem->GetLocation().dwLocation == OBJECTCELLTYPE_COMMON)
			|| (pRoleItem->GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP) ) )
		{
			type = ActiveAccessEquipment;
			return true;
		}
	}
	// If have valid emotion
	if ( GetEmotionInfo()->GetActiveEmotionIcon() )
	{
		type = ActiveAccessEmotion;
		return true;
	}
	return false;

	FUNCTION_END;
}

void CGuiMain::EditAccessKeyBtn( const DWORD& id )
{
	if ( (id >= 0) && (id <= 11) )
	{
		if ( GetGameAccelKeyManager() && GetGameAccelKeyManager()->m_curEditAccessKey.IsValid() )
		{
			enumGameAccelKeyType keyType;
			DWORD dwKeyID;
			BYTE curGroup = m_curAccessKeyGroup;
			BYTE key[4];
			key[0] = VK_F1 + id;
			key[1] = 0;
			if( GetGameAccelKeyManager()->GetCurAccelKeyID(keyType,dwKeyID, curGroup, key) )		//cur btn has an access key
			{
				if ( GetGameAccelKeyManager()->m_curEditAccessKey.bMainAccessKey )
				{
					//Exchange access key
					//Change btn bitmap
					if ( m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]
					&& m_pBtnAccessKey[id] )
					{
						m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetLocation( g_nMainAccessBtnLeft+GetGameAccelKeyManager()->m_curEditAccessKey.keyNo*g_MainAccessBtnSpace, g_nMainAccessBtnTop );

						int frmNum;
						//set old
						stResourceLocation rlOld;
						stResourceLocation rl;
						m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->GetBitmapInfo( rlOld, frmNum );
						m_pBtnAccessKey[id]->GetBitmapInfo( rl, frmNum );
						if ( keyType == GameAccelKeyType_Skill )
						{
							m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmapScale( skillIconScale );
							m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmap( rl, 3, dwBtnState );
						}
						else if( keyType == GameAccelKeyType_Face)
						{
							__SetEmotionAccessBtn(rl,GetGameAccelKeyManager()->m_curEditAccessKey.keyNo);
						}
						else
						{
							m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmapScale( itemIconScale );
							m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmap( rl, 1 );
						}


						//set new
						if ( GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Skill )
						{
							m_pBtnAccessKey[id]->SetBitmapScale( skillIconScale );
							m_pBtnAccessKey[id]->SetBitmap( rlOld, 3, dwBtnState );
						}
						else if( GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Face)
						{
							__SetEmotionAccessBtn(rlOld,id);
						}
						else
						{
							m_pBtnAccessKey[id]->SetBitmapScale( itemIconScale );
							m_pBtnAccessKey[id]->SetBitmap( rlOld, 1 );
						}								
					}
					//delete old two
					{
						//delete old edit
						{
							BYTE key[4];
							key[0] = VK_F1+GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
							key[1] = 0;
							GetGameAccelKeyManager()->DeleteOneAccelKey( key, GetGameAccelKeyManager()->m_curEditAccessKey.group );
						}

						//delete old cur
						{
							BYTE key[4];
							key[0] = VK_F1 + id;
							key[1] = 0;
							GetGameAccelKeyManager()->DeleteOneAccelKey( key, m_curAccessKeyGroup );
						}
					}
					//Add new two
					{
						//add new edit
						{
							stGameAccelKey key;
							key.type = keyType;
							key.dwID = dwKeyID;
							key.szAccelKey[0] = VK_F1 + GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
							key.szAccelKey[1] = 0;
							key.mGroup = (BYTE)GetGameAccelKeyManager()->m_curEditAccessKey.group;

							GetGameAccelKeyManager()->SetKey(key);
							GetGameAccelKeyManager()->Save( GetGameAccelKeyManager()->m_curEditAccessKey.group );
						}
						//add new cur
						{
							stGameAccelKey key;
							key.type = GetGameAccelKeyManager()->m_curEditAccessKey.keyType;
							key.dwID = GetGameAccelKeyManager()->m_curEditAccessKey.keyID;
							key.szAccelKey[0] = VK_F1 + id;
							key.szAccelKey[1] = 0;
							key.mGroup = (BYTE)m_curAccessKeyGroup;

							GetGameAccelKeyManager()->SetKey(key);
							GetGameAccelKeyManager()->Save( m_curAccessKeyGroup );
						}
					}

					RefreshKeyGroupState( m_curAccessKeyGroup );

					RestoreMouseIcon();
				}
				else
				{
					if ( GetGameGuiManager()->m_guiFloatAcessKey
						&& GetGameGuiManager()->m_guiFloatAcessKey->IsVisible() )
					{
						CGuiFloatAccessKey* pFloatAccessKeydlg = GetGameGuiManager()->m_guiFloatAcessKey;
						//exchange with float cur access key
						//Change btn bitmap
						if ( pFloatAccessKeydlg->m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]
						&& m_pBtnAccessKey[id] )
						{
							int __keyno = GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
							if ( __keyno != 0 )
								m_pBtnAccessKey[__keyno]->SetLocation( g_nFloatAccessBtnLeft+(__keyno-1)*g_FloatAccessBtnSpace, g_nFloatAccessBtnTop );
							else
								m_pBtnAccessKey[__keyno]->SetLocation( g_nFloatAccessBtnLeft+(FLOAT_ACCESS_KEY_NUM-1)*g_FloatAccessBtnSpace, g_nFloatAccessBtnTop );


							int frmNum;
							//set old
							stResourceLocation rlOld;
							stResourceLocation rl;
							pFloatAccessKeydlg->m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->GetBitmapInfo( rlOld, frmNum );
							m_pBtnAccessKey[id]->GetBitmapInfo( rl, frmNum );
							if ( keyType == GameAccelKeyType_Skill )
							{
								pFloatAccessKeydlg->m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmapScale( skillIconScale );
								pFloatAccessKeydlg->m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmap( rl, 3, dwBtnState );
							}
							else if( keyType == GameAccelKeyType_Face)
							{
								pFloatAccessKeydlg->__SetEmotionAccessBtn(rl,GetGameAccelKeyManager()->m_curEditAccessKey.keyNo);
							}
							else
							{
								pFloatAccessKeydlg->m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmapScale( itemIconScale );
								pFloatAccessKeydlg->m_pBtnAccessKey[GetGameAccelKeyManager()->m_curEditAccessKey.keyNo]->SetBitmap( rl, 1 );
							}									

							//set new
							if ( GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Skill )
							{
								m_pBtnAccessKey[id]->SetBitmapScale( skillIconScale );
								m_pBtnAccessKey[id]->SetBitmap( rlOld, 3, dwBtnState );
							}
							else if( GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Face)
							{
								__SetEmotionAccessBtn(rlOld,id);
							}
							else
							{
								m_pBtnAccessKey[id]->SetBitmapScale( itemIconScale );
								m_pBtnAccessKey[id]->SetBitmap( rlOld, 1 );
							}									
						}
						//delete old two
						{
							//delete old edit
							{
								BYTE key[4];
								key[0] = '0'+GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
								key[1] = 0;
								GetGameAccelKeyManager()->DeleteOneAccelKey( key, GetGameAccelKeyManager()->m_curEditAccessKey.group );
							}

							//delete old cur
							{
								BYTE key[4];
								key[0] = VK_F1 + id;
								key[1] = 0;
								GetGameAccelKeyManager()->DeleteOneAccelKey( key, GetCurAccessKeyGroup() );
							}
						}
						//Add new two
						{
							//add new edit
							{
								stGameAccelKey key;
								key.type = keyType;
								key.dwID = dwKeyID;
								key.szAccelKey[0] = '0' + GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
								key.szAccelKey[1] = 0;
								key.mGroup = (BYTE)GetGameAccelKeyManager()->m_curEditAccessKey.group;

								GetGameAccelKeyManager()->SetKey(key);
								GetGameAccelKeyManager()->Save( GetGameAccelKeyManager()->m_curEditAccessKey.group );
							}
							//add new cur
							{
								stGameAccelKey key;
								key.type = GetGameAccelKeyManager()->m_curEditAccessKey.keyType;
								key.dwID = GetGameAccelKeyManager()->m_curEditAccessKey.keyID;
								key.szAccelKey[0] = VK_F1 + id;
								key.szAccelKey[1] = 0;
								key.mGroup = GetCurAccessKeyGroup();

								GetGameAccelKeyManager()->SetKey(key);
								GetGameAccelKeyManager()->Save( GetCurAccessKeyGroup() );
							}
						}

						pFloatAccessKeydlg->RefreshKeyGroupState();
						RefreshKeyGroupState( GetCurAccessKeyGroup() );

						RestoreMouseIcon();
					}
				}
			}
			else		//cur btn has no access key
			{
				if ( GetGameAccelKeyManager()->m_curEditAccessKey.bMainAccessKey )
				{
					//delete old edit
					{
						BYTE key[4];
						key[0] = VK_F1+GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
						key[1] = 0;
						GetGameAccelKeyManager()->DeleteOneAccelKey( key, GetGameAccelKeyManager()->m_curEditAccessKey.group );
					}
					RemoveAccessKeyBtnInfo( (BYTE)GetGameAccelKeyManager()->m_curEditAccessKey.keyNo );

					m_pBtnAccessKey[id]->SetLocation(g_nMainAccessBtnLeft+id*g_MainAccessBtnSpace, g_nMainAccessBtnTop);
					//save cur access to this pos
					if ( GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Skill )
					{
						m_pBtnAccessKey[id]->SetBitmapScale( skillIconScale );
						m_pBtnAccessKey[id]->SetBitmap( GetGameAccelKeyManager()->m_curEditAccessKey.rl, 3, dwBtnState );
					}
					else if ( (GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Item)
						|| (GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Equipment) )
					{
						m_pBtnAccessKey[id]->SetBitmapScale( itemIconScale );
						m_pBtnAccessKey[id]->SetBitmap( GetGameAccelKeyManager()->m_curEditAccessKey.rl, 1 );
					}
					else if  (GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Face)
					{
						__SetEmotionAccessBtn(GetGameAccelKeyManager()->m_curEditAccessKey.rl,id);
					}



					//Save access key change
					stGameAccelKey key;
					key.type = GetGameAccelKeyManager()->m_curEditAccessKey.keyType;
					key.dwID = GetGameAccelKeyManager()->m_curEditAccessKey.keyID;
					key.szAccelKey[0] = VK_F1 + id;
					key.szAccelKey[1] = 0;
					key.mGroup = (BYTE)m_curAccessKeyGroup;

					GetGameAccelKeyManager()->SetKey(key);
					GetGameAccelKeyManager()->Save( m_curAccessKeyGroup );

					RestoreMouseIcon();
				}
				else
				{
					if ( GetGameGuiManager()->m_guiFloatAcessKey
						&& GetGameGuiManager()->m_guiFloatAcessKey->IsVisible() )
					{
						CGuiFloatAccessKey* pFloatAccessKey = GetGameGuiManager()->m_guiFloatAcessKey;
						//delete old edit
						{
							BYTE key[4];
							key[0] = '0' + GetGameAccelKeyManager()->m_curEditAccessKey.keyNo;
							key[1] = 0;
							GetGameAccelKeyManager()->DeleteOneAccelKey( key, GetGameAccelKeyManager()->m_curEditAccessKey.group );
						}
						pFloatAccessKey->RemoveAccessKeyBtnInfo( (BYTE)GetGameAccelKeyManager()->m_curEditAccessKey.keyNo );

						m_pBtnAccessKey[id]->SetLocation(g_nMainAccessBtnLeft+id*g_MainAccessBtnSpace, g_nMainAccessBtnTop);
						//put to main access key
						if ( GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Skill )
						{
							m_pBtnAccessKey[id]->SetBitmapScale( skillIconScale );
							m_pBtnAccessKey[id]->SetBitmap( GetGameAccelKeyManager()->m_curEditAccessKey.rl, 3, dwBtnState );
						}
						else if ( (GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Item)
							|| (GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Equipment) )
						{
							m_pBtnAccessKey[id]->SetBitmapScale( itemIconScale );
							m_pBtnAccessKey[id]->SetBitmap( GetGameAccelKeyManager()->m_curEditAccessKey.rl, 1 );
						}
						else if  (GetGameAccelKeyManager()->m_curEditAccessKey.keyType == GameAccelKeyType_Face)
						{
							__SetEmotionAccessBtn(GetGameAccelKeyManager()->m_curEditAccessKey.rl,id);
						}

						//Save access key change
						stGameAccelKey key;
						key.type = GetGameAccelKeyManager()->m_curEditAccessKey.keyType;
						key.dwID = GetGameAccelKeyManager()->m_curEditAccessKey.keyID;
						key.szAccelKey[0] = VK_F1 + id;
						key.szAccelKey[1] = 0;
						key.mGroup = (BYTE)GetCurAccessKeyGroup();

						GetGameAccelKeyManager()->SetKey(key);
						GetGameAccelKeyManager()->Save( GetCurAccessKeyGroup() );
					}

					RestoreMouseIcon();
				}
			}
		}
	}
}

/**
* \brief 处理对话框控件事件
* 
* 
* 
* \param nEvent : 
* \param nID : 
* \param pControl : 
* \return 
*/
bool CGuiMain::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch( nEvent )
	{
	case EVENT_BUTTON_DRAG_MOVE:
		if ( (nID >= 100) && (nID <= 111) )
		{
			if ( !GetGameCursor()->IsDrag()
				&& !(GetGameCursor()->GetCursorImageType() == eCursorSetAccess)
				&& !(GetGameCursor()->GetCursorImageType() == eCursorUseAccess)
				&& !(GetGameCursor()->GetCursorImageType() == eCursorFindAccess) )
			{
				enumGameAccelKeyType keyType;
				DWORD dwKeyID;
				BYTE curGroup = m_curAccessKeyGroup;
				BYTE key[4];
				key[0] = VK_F1 + nID - 100;
				key[1] = 0;
				if( GetGameAccelKeyManager()->GetCurAccelKeyID(keyType,dwKeyID, curGroup, key) )
				{
					if ( m_pBtnAccessKey[nID-100] )
					{
						if ( (abs(m_pBtnAccessKey[nID-100]->GetDragOffset().x) >= 5)
							|| (abs(m_pBtnAccessKey[nID-100]->GetDragOffset().y) >= 5) )
						{
							stResourceLocation rl;
							int fNum = 0;
							if ( m_pBtnAccessKey[nID-100]->GetBitmapInfo( rl, fNum ) )
							{
								GetGameCursor()->SetCursor( CCursor::CursorType_Drag, eCursorEditAccess );
								GetGameCursor()->SetImage( &rl, stPointI(15,15) );
								if ( GetGameAccelKeyManager() )
								{
									GetGameAccelKeyManager()->m_curEditAccessKey.bMainAccessKey = true;
									GetGameAccelKeyManager()->m_curEditAccessKey.group = m_curAccessKeyGroup;
									GetGameAccelKeyManager()->m_curEditAccessKey.keyNo = nID - 100;
									GetGameAccelKeyManager()->m_curEditAccessKey.rl = rl;
									GetGameAccelKeyManager()->m_curEditAccessKey.keyType = keyType;
									GetGameAccelKeyManager()->m_curEditAccessKey.keyID = dwKeyID;
									if( GameAccelKeyType_Face == keyType )
									{
										GetEmotionInfo()->SetCurIconInfo(dwKeyID,FROM_MAIN);
									}
								}
							}
						}
					}			
				}					
			}
		}
		break;
	case EVENT_BUTTON_DRAG_END:
		if ( (nID >= 100) && (nID <= 111) )
		{
		}
		break;
	case EVENT_LBUTTON_DOWN:		
		break;
	case EVENT_BUTTON_CLICKED://帮助教程按钮
		{
			if (nID == -1020) 
			{
				if (m_pshuxBtn)
				{
					GetGameGuiManager()->AddUserInfo(1);
					WORD skillpoints = GetScene()->GetMainCharacter()->GetProperty()->skillPoint;
					if (skillpoints >= 5)
					{
						GetGameGuiManager()->AddSkill(true);
					}					
				}
				return true;
			}
			
			for(stHelpButtons::iterator it = m_helpBtns.begin();
				it != m_helpBtns.end(); ++it)
			{
				if(nID ==  (*it)->iBtnId)
				{
					(*it)->pBtn->SetVisible(false);
					OnHelpTipBtn((*it)->type);
					return true;
				}
			}

			if ( GetChatManager().OnReOpenChatDialog( nID ) )
			{
				return true;
			}
			if( CLordStrikeManager::GetSingleton()->OnReOpenLSDialog(nID))
			{
				return true;
			}
		}
		break;
	case EVENT_DIALOG_COMMAND:
		{
			if ( m_menu->IsVisible() )
			{
				switch( nID )
				{
				//----------------------------------------------------------
				// Team Msg 
				case szCmdItem::CMD_LEAVE:
					{
						GetGameGuiManager()->m_guiTeam->LeaveTeam();
						return true;			
					}
					break;
				case szCmdItem::CMD_UNCOMBIN:
					{
						stUnCombinUserCmd cmd;
						cmd.type = UN_STATE_COMBIN;
						SEND_USER_CMD( cmd );
						return true;
					}
					break;
				case szCmdItem::CMD_ASSING_EXP_BY_LEVLE:// 1、等级分配
					{
						m_menu->SetMenuItemCheck(szCmdItem::CMD_ASSING_EXP_BY_LEVLE,true);
						m_menu->SetMenuItemCheck(szCmdItem::CMD_ASSING_EXP_BY_SPECIAL,false);
						GetGameGuiManager()->m_guiTeam->SetExpAssign(TEAM_EXP_MODE_NORMAL); 
					}
					break;
				case szCmdItem::CMD_ASSING_EXP_BY_SPECIAL: // 2、特殊分配
					{
						m_menu->SetMenuItemCheck(szCmdItem::CMD_ASSING_EXP_BY_SPECIAL,true);
						m_menu->SetMenuItemCheck(szCmdItem::CMD_ASSING_EXP_BY_LEVLE,false);
						GetGameGuiManager()->m_guiTeam->SetExpAssign(TEAM_EXP_MODE_SPECIAL);
					}
					break;
				case szCmdItem::CMD_PICKUP_ITEM_FREEDOM:// 1、自由拾取
					{
						m_menu->SetMenuItemCheck(szCmdItem::CMD_PICKUP_ITEM_FREEDOM,true);
						m_menu->SetMenuItemCheck(szCmdItem::CMD_PICKUP_ITEM_POLL,false);
						GetGameGuiManager()->m_guiTeam->SetPickupItemAssing(TEAM_OBJ_MODE_NORMAL);
					}
					break;
				case szCmdItem::CMD_PICKUP_ITEM_POLL: // 2、	轮流拾取 
					{ 
						m_menu->SetMenuItemCheck(szCmdItem::CMD_PICKUP_ITEM_POLL,true);
						m_menu->SetMenuItemCheck(szCmdItem::CMD_PICKUP_ITEM_FREEDOM,false);
						GetGameGuiManager()->m_guiTeam->SetPickupItemAssing(TEAM_OBJ_MODE_SPECIAL);
					}
					break;
				//----------------------------------------------------------
				case szCmdItem::CMD_UNFACE:
					{
						stUnCombinUserCmd cmd;
						cmd.type = UN_STATE_CHANGE_FACE;
						SEND_USER_CMD( cmd );
						return true;
					}
					break;
				case szCmdItem::CMD_UN_TEAM_ATTACK:
					{
						stUnCombinUserCmd cmd;
						cmd.type = (enmUnStateType)GetScene()->GetMainCharacter()->GetTeamAttackState();
						SEND_USER_CMD( cmd );
						return true;
					}
					break;
				}
			}
			/*else if ( m_cartMenu->IsVisible() )
			{
				switch( nID )
				{
				case CartCMD_Play:
				case CartCMD_Stop:
					{
						stCartControlQuestUserCmd cmd;
						SEND_USER_CMD( cmd );
					}
					break;
				}
				m_cartMenu->SetVisible( false );
				break;			
			}*/			
		}		
		break;	
	}

	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}


CGuiButton* CGuiMain::InsertPrivateButton(int nPos)
{
	static int nHeight=7,nWidth=9,nIcon=32,nNum=7;
	stPointI ptStart(990,200);
	if (GetDevice()->GetWidth() == 800)
	{
		ptStart.x = 766;
	}
	else
	{
		ptStart.x = 990;
	}
	int nID = IDC_PRIVATECHAT_BTN_BEG+nPos;
	CGuiButton* pButton = GetButton(nID);

	if (pButton==NULL)
	{
		if(FAILED(AddButton(nID,NULL,0,0,0,0,0,0,&(pButton))))
		{
			return NULL;
		}
		stPointI stPos=ptStart;
		stPos += stPointI(nPos/nNum*(nIcon+nHeight)*(-1),nPos%nNum*(nIcon+nWidth));
		pButton->SetLocation(stPos.x,stPos.y);
	}

	return pButton;
}
/**
* \brief 处理弹出菜单
* 
* 
* 
* \return 
*/
bool CGuiMain::OnContextMenu()
{
	FUNCTION_BEGIN;
	if( m_pPopupMenu )
	{

		stRectI rc;
		rc.left = 5;
		rc.right = 80;
		rc.top = 10;
		rc.bottom =90;

		if( rc.PtInRect( Engine_GetCursor()->GetPosition() ) )
		{
			if ( GetScene()->GetMainCharacter() )
			{
				m_menu->SetMenuEnabled( szCmdItem::CMD_UNCOMBIN, GetScene()->GetMainCharacter()->IsCombinationState() );
				m_menu->SetMenuEnabled( szCmdItem::CMD_UNFACE, GetScene()->GetMainCharacter()->IsChangeFace() );
			}

			{
				//get team attack state
				bool bCanUnTeamAttack = GetScene()->GetMainCharacter()->IsTeamLeader();
				bCanUnTeamAttack &= (GetScene()->GetMainCharacter()->GetTeamAttackState() != -1);

				m_menu->SetMenuEnabled( szCmdItem::CMD_UN_TEAM_ATTACK, bCanUnTeamAttack );
			}

			if ( GetGameGuiManager()->m_guiTeam )
			{
				m_menu->SetMenuEnabled( szCmdItem::CMD_LEAVE, ( (GetGameGuiManager()->m_guiTeam->m_aTeamMember.size() > 0) ) );
				//DWORD dwMainID = GetScene()->GetMainCharacter()->GetID();
				//bool bEnable = GetGameGuiManager()->m_guiTeam->IsTeamHead(dwMainID);
				bool bEnable =  (GetScene()->GetMainCharacter()->GetTeamState() == TEAM_STATE_LEADER) ;
				bEnable = bEnable && (GetGameGuiManager()->m_guiTeam->m_aTeamMember.size() > 0);
				bEnable = bEnable && (GetGameGuiManager()->m_guiTeam->m_dwTeamMode == TEAM_NORMAL);
			    m_menu->SetMenuEnabled( szCmdItem::CMD_ASSING_EXP_BY_LEVLE, bEnable);
				m_menu->SetMenuEnabled( szCmdItem::CMD_ASSING_EXP_BY_SPECIAL, bEnable);
				m_menu->SetMenuEnabled( szCmdItem::CMD_PICKUP_ITEM_FREEDOM,bEnable);
				m_menu->SetMenuEnabled( szCmdItem::CMD_PICKUP_ITEM_POLL, bEnable);
	
				if( bEnable)
				{
					m_menu->SetVisible(true);
					if( GetGameGuiManager()->m_guiTeam->m_dwCurExpAssign == TEAM_EXP_MODE_NORMAL)
						OnGuiEvent(EVENT_DIALOG_COMMAND,szCmdItem::CMD_ASSING_EXP_BY_LEVLE,NULL);
					else if( GetGameGuiManager()->m_guiTeam->m_dwCurExpAssign == TEAM_EXP_MODE_SPECIAL)
						OnGuiEvent(EVENT_DIALOG_COMMAND,szCmdItem::CMD_ASSING_EXP_BY_SPECIAL,NULL);
	
					if( GetGameGuiManager()->m_guiTeam->m_dwCurObjAssign == TEAM_OBJ_MODE_NORMAL)
						OnGuiEvent(EVENT_DIALOG_COMMAND,szCmdItem::CMD_PICKUP_ITEM_FREEDOM,NULL);
					else if( GetGameGuiManager()->m_guiTeam->m_dwCurObjAssign == TEAM_OBJ_MODE_NORMAL)
						OnGuiEvent(EVENT_DIALOG_COMMAND,szCmdItem::CMD_PICKUP_ITEM_POLL,NULL);
				}					
			
			}


			m_pPopupMenu->Popup();
			return true;
			
		} 
	}
	return false;
	FUNCTION_END;
}

/**
* \brief 获取状态图标ｉｄ
* 
* 
* 
* \param state : 状态
* \return 状态图标ｉｄ
*/
//State info about
DWORD CGuiMain::GetStateIconID( DWORD state )
{
	FUNCTION_BEGIN;

	for(tTableCharStateBase::iterator it = g_tableCharStateBase.begin(); it != g_tableCharStateBase.end(); ++it)
	{
		if((*it).dwID >= MAX_STATE)
			return -1;

		if ( (*it).dwID == state )
			return (*it).dwStateID;
	}

	return -1;

	FUNCTION_END;
}

bool bStateNeedDesc(DWORD state)
{
	if ( (state == USTATE_SUMMON_STRENGTH)
		|| (state == USTATE_COMBIN_GOD_FIGHTER)
		|| (state == USTATE_ROCK)
		|| (state == USTATE_BOMB)
		|| (state == USTATE_COOL_TOUCH)
		|| IsTeamAttackState(state) )
	{
		return false;
	}

	return true;
}

/**
* \brief 获取状态描述
* 
* 
* 
* \param state : 状态
* \return 状态描述
*/
//State info about
void CGuiMain::GetStateDesc( DWORD state, stStateTips& desc )
{
	FUNCTION_BEGIN;

	for(tTableCharStateBase::iterator it = g_tableCharStateBase.begin(); it != g_tableCharStateBase.end(); ++it)
	{
		if((*it).dwID >= MAX_STATE)
		{
			sprintf( desc.strDesc, "" );
			sprintf( desc.strName, "" );
			return;
		}

		if ( (*it).dwID == state )
		{
			stStateInfo*  stateInfo = GetMouseOverStateInfo();
			if ( stateInfo )
			{
				sprintf( desc.strName, "%s", (*it).strName );
				if ( 1 )		//!(stateInfo->bMainCharacter)
				{
					if ( (stateInfo->rate > 0) && bStateNeedDesc( state ) && !( (0 == (*it).dwType) && (stateInfo->rate > 100) ) )
						sprintf( desc.strDesc, "%s%d%s", (*it).strDesc, stateInfo->rate, ( (*it).dwType ) ? "点\n" : "%\n" );
					else
						sprintf( desc.strDesc, "%s\n", (*it).strDesc );
				}		
				return;
			}			
		}
	}

	sprintf( desc.strDesc, "" );
	sprintf( desc.strName, "" );

	FUNCTION_END;
}

/**
* \brief 添加一种状态
* 
* 详细描述
* 
* \param bMainCharacter : 是否为自己
* \param bSkill : 是否为技能
* \param dwID : 状态ｉｄ
* \param time : 状态起始时间
* \param rate : 状态特征值
* \return 
*/
//Add one state
#define GOD_GIVE_SOUND_EFFECT_SKILL		451
void CGuiMain::AddOneState( bool bMainCharacter, bool bSkill, DWORD dwID, DWORD time, DWORD rate )
{
	FUNCTION_BEGIN;

	if ( !bMainCharacter && ( (dwID == USTATE_CHANGEFACE) || (dwID == USTATE_COMMON_TEMP_USE) ) )
		return;


	#ifdef _DEBUG
							static int  test= 0;
							char szTest[120];
							_snprintf(szTest,120,"收到状态改变消息！！！ %d",test++);
							szTest[119] = 0 ;

							GetGameGuiManager()->AddClientSystemMessage(szTest);
							GetGameGuiManager()->AddClientSystemMessage( avar( "%u", dwID ) );
#endif 


	//We do some special treatment with " rate " for special state
	if ( USTATE_POISON_EQUIPMENT == dwID )
	{
		rate *= 5;
	}
	else if ( USTATE_RED_FIRE_JUE == dwID )
	{
		rate *= 20;
	}
	else if ( USTATE_MAGIC_DEFENCE == dwID )
	{
		rate = rate + 1;
	}
	else if ( USTATE_GOD_GIVE_SOUND == dwID )
	{
		if ( rate >= GOD_GIVE_SOUND_EFFECT_SKILL )
			rate = 20 + ( rate - GOD_GIVE_SOUND_EFFECT_SKILL ) * 5;
		else
			rate = 20;
	}
	else if ( USTATE_STAND_WITH__GHOST == dwID )
	{
		rate /= 10;
	}
	else if ( USTATE_DEFENCE_RING_EFFECT == dwID )
	{
		if ( rate >= 524 ) 
			rate = 50 + ( rate - 524 ) * 25;
		else
			rate = 50;
	}
	else if ( USTATE_REVERT == dwID )
	{
		if ( rate >= 687 )
			rate = 200 + ( rate - 687 ) * 50;
		else
			rate = 200;
	}
	else if ( USTATE_COLD_ICE_SKILL == dwID )
	{
		if ( rate >= 1 )
			rate = 4 + (rate - 1) * 2;
		else
			rate = 4;
	}
	//End treat with special state

	//to see if we already has this state in list
	for( std::vector<CGuiMain::stStateInfo>::iterator itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		if ( (itor->bMainCharacter == bMainCharacter)
			&& (itor->bSkill == bSkill )
			&& (itor->dwID == dwID) )
		{
			//yes we already has one
			//we replace this to new
			itor->time = time;
			itor->startTime = xtimeGetTime();
			itor->rate = rate;


			if ( itor->time > 5 )
				itor->StopFlash();

			return;
		}
	}	

	stStateInfo stInfo;
	stInfo.bMainCharacter = bMainCharacter;
	stInfo.bSkill = bSkill;
	stInfo.dwID = dwID;
	stInfo.time = time;
	stInfo.rate = rate;
	stInfo.startTime = xtimeGetTime();

	//Get icon
	stResourceLocation rl;
	strcpy( rl.szFileName, "data\\icons.gl" );
	if ( bSkill )
	{
		//Get skill icon
		rl.group = 1;
		rl.frame = GetSkillBase( dwID )->dwIcon;
	}
	else
	{
		//Get state icon
		rl.group = 1;
		rl.frame = GetStateIconID( dwID );
	}
	stInfo.pIcon = GetDevice()->FindBitmaps( &rl );

	if ( NULL == stInfo.pIcon )		//has no icon may be need not show this state icon
		return;

	//Get icon pos
	stPointI pt;
	if ( bMainCharacter )
	{
		stInfo.ptPos.y = 90;
		stInfo.ptPos.x = 20 + GetMainCharacterStateIconNum() * 35; 
	}
	else
	{
		stInfo.ptPos.y = 90;
		stInfo.ptPos.x = 250 + GetActiveNpcStateIconNum() * 35;
	}

	//push to list
	m_aStateInfo.push_back( stInfo );

	FUNCTION_END;
}

/**
* \brief 获取当前选中ＮＰＣ状态图标数目
* 
* 
* 
* \return 当前选中ＮＰＣ状态图标数目
*/
int CGuiMain::GetActiveNpcStateIconNum()
{
	FUNCTION_BEGIN;

	int num = 0;
	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		if ( !( (*itor).bMainCharacter ) )
			num++ ;
	}

	return num;

	FUNCTION_END;
}

/**
* \brief 获取自身状态图标数目
* 
* 
* 
* \return 自身状态图标数目
*/
int CGuiMain::GetMainCharacterStateIconNum()
{
	FUNCTION_BEGIN;

	int num = 0;
	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		if ( (*itor).bMainCharacter )
			num++ ;
	}

	return num;	

	FUNCTION_END;
}

/**
* \brief 删除所有状态显示
* 
* 
* 
* \param bMainCharacter : 是否自己
* \return 成功则返回ｔｒｕｅ，　否则返回ｆａｌｓｅ
*/
//Delete all state
bool CGuiMain::DeleteAllState( bool bMainCharacter )
{
	FUNCTION_BEGIN;

	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		if ( (*itor).bMainCharacter == bMainCharacter )
		{
#ifdef _DEBUG
			GetGameGuiManager()->AddClientSystemMessage( "状态删除！！！" );
			GetGameGuiManager()->AddClientSystemMessage( avar( "%u", (*itor).dwID ) );
#endif 
			SimpleRemoveFromArray( m_aStateInfo, itor - m_aStateInfo.begin() );
			return true;
		}
	}

	return false;

	FUNCTION_END;
}

bool CGuiMain::DeleteInterestNpcStateList()
{
	FUNCTION_BEGIN;

	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		if ( (*itor).bMainCharacter == false )
		{
			SimpleRemoveFromArray( m_aStateInfo, itor - m_aStateInfo.begin() );
			return true;
		}
	}

	return false;

	FUNCTION_END;
}

/**
* \brief 删除一种状态
* 
* 
* 
* \param bMainCharacter : 是否自己
* \param bSkill : 是否技能
* \param dwID : 状态ｉｄ
* \return 
*/
//Delete one state
void CGuiMain::DeleteOneState( bool bMainCharacter, bool bSkill, DWORD dwID )
{
	FUNCTION_BEGIN;

	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		if ( ( (*itor).bMainCharacter == bMainCharacter )
			&& ( (*itor).bSkill == bSkill )
			&& ( (*itor).dwID == dwID ) )
		{			
#ifdef _DEBUG

			GetGameGuiManager()->AddClientSystemMessage( "状态删除！！！" );
			GetGameGuiManager()->AddClientSystemMessage( avar( "%u", dwID ) );

#endif 
			SimpleRemoveFromArray( m_aStateInfo, itor - m_aStateInfo.begin() );
			break;
		}
	}

	//Re arrange icon pos
	// depreciated 
	/*int num1 = 0;
	int num2 = 0;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
	if ( (*itor).bMainCharacter )
	{
	(*itor).ptPos.x = 20 + num1 * 35;
	num1++ ;
	}
	else
	{
	(*itor).ptPos.x = 250 + num2 * 35;
	num2++ ;
	}
	}*/

	FUNCTION_END;
}

/**
* \brief 绘制状态图标
* 
* 
* 
* \return 
*/
//Render state list
void CGuiMain::RenderStateList(	float fElapsedTime )
{
	FUNCTION_BEGIN;

	if ( m_aStateInfo.empty() )
		return;

	std::vector<stStateInfo>::iterator itor;
	stPointF scale( 30.0f/42.0f, 30.0f/42.0f );
	/*for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
	if ( (*itor).pIcon )
	{
	if ( (*itor).bFlashState )
	{
	static int dir = -1;
	if ( ((*itor).color.a < 0.1f) && (dir == -1) )
	dir = 1;
	else if( ((*itor).color.a > 0.9f) && (dir == 1) )
	dir = -1;

	(*itor).color += D3DXCOLOR( (*itor).fFlashRate*dir, (*itor).fFlashRate*dir, (*itor).fFlashRate*dir, (*itor).fFlashRate*dir );
	}

	if ( (*itor).bMainCharacter )
	(*itor).pIcon->RenderAni( (*itor).ptPos.x, (*itor).ptPos.y, xtimeGetTime(), NULL, &scale, (DWORD)(*itor).color );
	else
	{
	if ( GetGameGuiManager()->m_guiCharacterProperty && GetGameGuiManager()->m_guiCharacterProperty->IsVisible() )
	(*itor).pIcon->RenderAni( (*itor).ptPos.x, (*itor).ptPos.y, xtimeGetTime(), NULL, &scale, (DWORD)(*itor).color );
	}
	}
	}*/

	// only show the recent 12 states
	//Get main character state num
	int stateNum = 0, stateNum1 = 0;
	int nPerLine = 5;
	for( itor=m_aStateInfo.begin(); itor!=m_aStateInfo.end(); itor++ )
	{
		if ( (*itor).bMainCharacter )
			stateNum++ ;
		else
			stateNum1++ ;
	}
	//Get interest npc state num
	itor = m_aStateInfo.begin();
	/*int nCookie = m_aStateInfo.size() - 12;
	while(nCookie-- > 0)
	{
	itor++;
	} */

	for(int nCounter=0, nCounter1=0 ; itor != m_aStateInfo.end(); itor++ )
	{
		if ( (*itor).pIcon )
		{
			//Get color while flash state
			if ( (*itor).bFlashState )
			{
				static int dir = -1;
				if ( ((*itor).color.a < 0.1f) && (dir == -1) )
					dir = 1;
				else if( ((*itor).color.a > 0.9f) && (dir == 1) )
					dir = -1;

				(*itor).color += D3DXCOLOR( (*itor).fFlashRate*fElapsedTime*dir, (*itor).fFlashRate*fElapsedTime*dir, (*itor).fFlashRate*fElapsedTime*dir, (*itor).fFlashRate*fElapsedTime*dir );
			}

			if ( (*itor).bMainCharacter )
			{
				if ( stateNum-- <= nPerLine*2 )
				{
					if ( nCounter <= nPerLine*2 )
					{
						stPointI pt(87,37);
						pt.x += ((nCounter>=nPerLine)?(nCounter-nPerLine):nCounter) * 35;
						pt.y += ((nCounter>=nPerLine)?1:0) * 35;
						(*itor).ptPos.x = pt.x;
						(*itor).ptPos.y = pt.y;
						(*itor).pIcon->RenderAni( pt.x,pt.y, xtimeGetTime(), NULL, &scale, (DWORD)(*itor).color );
					}				
					nCounter++ ;
				}				
			}
			else
			{
				if ( stateNum1-- <= nPerLine*2 )
				{
					if ( GetGameGuiManager()->m_guiCharacterProperty && GetGameGuiManager()->m_guiCharacterProperty->IsVisible() )
					{
						if ( nCounter1 <= nPerLine*2 )
						{
							stPointI pt(342,37);
							pt.x += ((nCounter1>=nPerLine)?(nCounter1-nPerLine):nCounter1) * 35;
							pt.y += ((nCounter1>=nPerLine)?1:0) * 35;
							(*itor).ptPos.x = pt.x;
							(*itor).ptPos.y = pt.y;
							(*itor).pIcon->RenderAni( pt.x, pt.y, xtimeGetTime(), NULL, &scale, (DWORD)(*itor).color );
						}					
					}
					nCounter1++ ;
				}				
			}
		}
	}

	RenderStateTooltips( xtimeGetTime() );

	FUNCTION_END;
}

/**
* \brief 获取鼠标所指状态
* 
* 详细描述
* 
* \return 鼠标所指状态
*/
//Get mouse over state icon
CGuiMain::stStateInfo* CGuiMain::GetMouseOverStateInfo()
{
	FUNCTION_BEGIN;

	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		stPointI ptCursor = Engine_GetCursor()->GetPosition();
		stRectI rcIcon;
		rcIcon.left = (*itor).ptPos.x;
		rcIcon.right = (*itor).ptPos.x + 30;
		rcIcon.top = (*itor).ptPos.y;
		rcIcon.bottom = (*itor).ptPos.y + 30;

		if ( rcIcon.PtInRect( ptCursor ) )
		{
			return &(*itor);
		}
	}

	return NULL;

	FUNCTION_END;
}

bool IsStateBuffNeedFlush( const DWORD& dwState )
{
	if ( (dwState == USTATE_RELIVERING)
		|| (dwState == USTATE_DEFENCE_RING)		
		|| (dwState == USTATE_REJECT_LIGHT_RING) )
	{
		return false;
	}

	return true;
}

/**
* \brief 更新状态列表
* 
* 
* 
* \param curTime : 当前时间（绝对）
* \return 
*/
//Update state list
void CGuiMain::UpdateStateList( DWORD curTime )
{
	FUNCTION_BEGIN;

	if ( m_aStateInfo.empty() )
		return;

	std::vector<stStateInfo>::iterator itor;
	for( itor=m_aStateInfo.begin();itor!=m_aStateInfo.end();itor++ )
	{
		//If our interest npc is die we delete all about state
		if ( (NULL == GetScene()->m_pInterestNpc) || (GetScene()->m_pInterestNpc->IsDie()) )
		{
			if ( !((*itor).bMainCharacter ) )
			{
				DeleteOneState( false, (*itor).bSkill, (*itor).dwID );
				break;
			}
		}

		//delete all those state whose left time < 0
		int leftTime = (*itor).time - (curTime - (*itor).startTime)/1000;

		if ( (leftTime <= 5) && !IsTeamAttackState( (*itor).dwID ) && IsStateBuffNeedFlush( (*itor).dwID ) )
		{
			if ( !(*itor).bFlashState )
			{
				//Add tips
				char szName[MAX_NAMESIZE];
				szName[0] = 0L;
				for(tTableCharStateBase::iterator it = g_tableCharStateBase.begin(); it != g_tableCharStateBase.end(); ++it)
				{
					if ( (*it).dwID == (*itor).dwID )
						sprintf( szName, (*it).strName );
				}

				char szTemp[256];
				if( (*itor).bMainCharacter )
					sprintf( szTemp, "您身上的 %s 状态将于五秒内消失！", szName );
				else
				{
					if( GetScene()->m_pInterestNpc )
						sprintf( szTemp, "%s身上的 %s 状态将于五秒内消失！", GetScene()->m_pInterestNpc->GetName(), szName );
					else
						sprintf( szTemp, " %s 状态将于五秒内消失！", szName );
				}
				GetGameGuiManager()->AddClientSystemMessage( szTemp );

				//Play flash
				(*itor).PlayFlash( 1.0f / 0.6f );
			}			
		}

		//We just do delete state when state is deleted by server
		/*if ( leftTime < 0 )
		{
			DeleteOneState( (*itor).bMainCharacter, (*itor).bSkill, (*itor).dwID );
			return;
		}*/
	}

	FUNCTION_END;
}

#define Tips_AddNum(szFormat,nNum)\
	if(nNum){\
	_snprintf(szTemp,sizeof(szTemp),szFormat,nNum);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_stateToolTips.AddText(szTemp,0);\
	}\

#define Tips_AddString(szText) \
	if( (szText)[0] ) {\
	\
	_snprintf(szTemp,sizeof(szTemp),"%s",szText);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_stateToolTips.AddText(szTemp,0);\
	}\

/**
* \brief 刷新状态提示信息
* 
* 
* 
* \param ptPos : 位置
* \param curTime : 当前时间（绝对）
* \return 
*/
//Update state icon tooltips
void CGuiMain::UpdateStateTooltips( stPointI& ptPos, DWORD curTime )
{
	FUNCTION_BEGIN;

	stStateInfo*  stateInfo = GetMouseOverStateInfo();
	char szTemp[256];
	if ( stateInfo != NULL )
	{
		m_stateToolTips.Clear();
		m_stateToolTips.SetAlignType( GUI_ALIGN_LEFT );
		m_stateToolTips.SetLineGaps( 3 );
		m_stateToolTips.SetBorderColor(0);
		m_stateToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
		//Get pos
		ptPos.x = stateInfo->ptPos.x + 30;
		ptPos.y = stateInfo->ptPos.y + 30;
		//Get content
		if ( 0 )		//Get skill tooltips		stateInfo->bSkill
		{
			SkillBase_t* pSkill;
			pSkill = GetSkillBase( stateInfo->dwID );
			if ( pSkill )
			{
				m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
				m_stateToolTips.AddText( pSkill->pstrName );
				m_stateToolTips.AddText( "\n\n" );
				m_stateToolTips.AddText( pSkill->strDesc, 0 );

				int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
				if ( leftTime > 0 )
					Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}
		}
		else		//Get state tooltips
		{
			stStateTips stateTips;
			GetStateDesc( stateInfo->dwID, stateTips );
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( stateTips.strName );
			m_stateToolTips.AddText( "\n\n" );
			m_stateToolTips.AddText( stateTips.strDesc, 0 );
			//if ( !(stateInfo->bMainCharacter) )
			if ( (stateInfo->dwID != USTATE_PK)
				&& !IsTeamAttackState(stateInfo->dwID)
				&& IsStateBuffNeedFlush(stateInfo->dwID) )
			{
				//Tips_AddNum( "持续 %u 秒\n", stateInfo->time );
				int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
				if ( leftTime > 0 )
				{
					Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
				}
				else
					Tips_AddString( "\n剩余时间 N/A" );
			}
			/*switch( stateInfo->dwID )
			{
			case USTATE_FREEZE:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "冰冻" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "移动速度降低,持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}
			break;
			case USTATE_CONFUSION:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "混乱" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "操作反相(点哪里跑向反方向),持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}				
			break;
			case USTATE_BLED:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "流血" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "生命值持续下降,每秒下降 %u 点,", stateInfo->rate );
			Tips_AddNum( "\n持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}
			break;
			case USTATE_PALSY:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "麻痹" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "不能移动,可以使用物品,持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}				
			break;
			case USTATE_BLIND:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "失明" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "画面变黑,除了中心点的角色周围8格以外,看不到任何范围外的东西,持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}
			break;
			case USTATE_ROCK:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "石化" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "不能移动,不能使用物品,不能使用技能,持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}
			break;
			case USTATE_POISON:
			{
			m_stateToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			m_stateToolTips.AddText( "中毒" );
			m_stateToolTips.AddText( "\n\n  " );
			Tips_AddNum( "生命值持续下降,每秒下降 %u 点,", stateInfo->rate );
			Tips_AddNum( "\n持续 %u 秒\n", stateInfo->time );
			int leftTime = stateInfo->time - (curTime - stateInfo->startTime) / 1000;
			if ( leftTime > 0 )
			Tips_AddNum( "\n剩余时间 %u 秒", leftTime );
			}
			break;
			}*/			
		}

		m_stateToolTips.Resize();
	}
	else
		m_stateToolTips.Clear();

	FUNCTION_END;
}

/**
* \brief 绘制状态提示
* 
* 
* 
* \param curTime : 当前时间（绝对）
* \return 
*/
//Render state icon tooltips
void CGuiMain::RenderStateTooltips(	DWORD curTime )
{
	FUNCTION_BEGIN;

	stPointI ptPos;
	UpdateStateList( curTime );
	UpdateStateTooltips( ptPos, curTime );
	if ( !(m_stateToolTips.Empty()) )
		m_stateToolTips.Render( ptPos.x, ptPos.y );

	FUNCTION_END;
}

/**
* \brief 升级属性点还有剩余提示按钮按钮
* 
* 
* 
* \bool nShow : 显示或者隐藏(默认显示)
* \return 
*/
void CGuiMain::AddshuxBtn(bool nShow /* = true */)
{
	stHelpButton *pHbtn = GetHideshuxBtn();
	if(!pHbtn) 
	{
		pHbtn = new stHelpButton();
		Assert(pHbtn);
		pHbtn->iBtnId = -1020;//bird
		int x = GetDevice()->GetWidth() - 42 - GetClientX();
		int y = GetDevice()->GetHeight()/2 - GetClientY();
		if(FAILED(AddButton(pHbtn->iBtnId,NULL,x,y,0,0,0,0,&(pHbtn->pBtn))))
		{
			delete pHbtn;
			return ;
		}
		pHbtn->pBtn->SetButtonType(CGuiButton::ButtonType_Bitmap);
		pHbtn->pBtn->SetBitmap(stResourceLocation("data\\Interfaces.gl",131,1),1);
		m_pshuxBtn = pHbtn;
	}
	Assert(pHbtn);
	pHbtn->type = USER_SETTING_SHOW_SHUXTIP;
	pHbtn->dwStartTime = xtimeGetTime();
	pHbtn->pBtn->SetToolTips("您还有未分配的人物属性点和技能点，请进入相应界面进行点数分配");
	pHbtn->pBtn->SetVisible(nShow);
}

/**
* \brief 显示帮助教程按钮
* 
* 
* 
* \enumUserSetting type : 教程类型
* \return 
*/
void CGuiMain::ShowHelpTipBtn(enumUserSetting type)
{
	FUNCTION_BEGIN;

	if( bIsTypeExist(type) ) return ;
	stHelpButton *pHbtn = GetHideBtn();
	if(!pHbtn) 
	{
		pHbtn = new stHelpButton();
		Assert(pHbtn);
		pHbtn->iBtnId = GetNextHelpBtnId();
		if(FAILED(AddButton(pHbtn->iBtnId,NULL,0,0,0,0,0,0,&(pHbtn->pBtn))))
		{
			delete pHbtn;
			return ;
		}
		pHbtn->pBtn->SetButtonType(CGuiButton::ButtonType_Bitmap);
		pHbtn->pBtn->SetBitmap(stResourceLocation("data\\Interfaces.gl",1,232),1);
		m_helpBtns.push_back(pHbtn);	
	}
	Assert(pHbtn);
	pHbtn->type = type;
	pHbtn->dwStartTime = xtimeGetTime();
	pHbtn->pBtn->SetVisible(true);

#define ShowCase(t,text) {case t: pHbtn->pBtn->SetToolTips(text);}break
	switch( type )
	{
		ShowCase(USER_SETTING_HELP_MOVE,"移动教程");
		ShowCase(USER_SETTING_HELP_NPC,"NPC教程");
		ShowCase(USER_SETTING_HELP_NPCDIALOG,"NPC对话教程");
		ShowCase(USER_SETTING_HELP_ATTACK,"攻击教程");

		ShowCase(USER_SETTING_HELP_PK,"PK教程");
		ShowCase(USER_SETTING_HELP_CLEARREDNAME,"消除红名教程");
		ShowCase(USER_SETTING_HELP_TASK,"任务教程");
		ShowCase(USER_SETTING_HELP_RIGHTBTN,"右键选项教程");

		ShowCase(USER_SETTING_HELP_TRADE,"交易教程");
		ShowCase(USER_SETTING_HELP_TEAM,"组队教程");
		ShowCase(USER_SETTING_HELP_MEMBERHEAD,"队员头像教程");
		ShowCase(USER_SETTING_HELP_ACCELKEY,"快捷使用物品教程");

		ShowCase(USER_SETTING_HELP_PACK,"包裹教程");
		ShowCase(USER_SETTING_HELP_EQUIP,"如何装备物品教程");
		ShowCase(USER_SETTING_HELP_EXPACK,"额外包裹教程");
		ShowCase(USER_SETTING_HELP_SKILL,"技能教程");

		ShowCase(USER_SETTING_HELP_ADDPROPPOINT,"加点属性教程");
		ShowCase(USER_SETTING_HELP_STUDY,"技能学习教程");
		ShowCase(USER_SETTING_HELP_AUTO_FIND_PATH,"自动寻路教程");
		ShowCase(USER_SETTING_HELP_ITEM_LINK,"物品链接教程");
		ShowCase(USER_SETTING_HELP_PRIVATE_CHAT,"悄悄话教程");

		ShowCase(USER_SETTING_HELP_MATERIAL_COMPOSE,"原料合成教程");
		//ShowCase(USER_SETTING_HELP_SOULSTONEL_COMPOSE,"魂魄合成教程");
		ShowCase(USER_SETTING_HELP_SOULSTONE_MAKE,"魂魄镶嵌教程");
		ShowCase(USER_SETTING_HELP_ITEM_UPGRID,"道具升级教程");

		ShowCase(USER_SETTING_HELP_ATTACK_MODE_CHANGE,"攻击模式切换教程");
		ShowCase(USER_SETTING_HELP_CHAT,"聊天教程");
		ShowCase(USER_SETTING_HELP_SELECT,"选择目标教程");
		ShowCase(USER_SETTING_HELP_ALT_SELECT_NPC,"alt选中npc教程");
	default:
		break;
	}

	UpdateHelpBtnState();

	FUNCTION_END;
}

/**
* \brief 帮助教程按钮响应函数(打开相应的帮助页面)
* 
* 同时更新该设置，下次就不会再显示按钮了
* 
* \enumUserSetting type : 教程类型
* \return 
*/
void CGuiMain::UpdateHelpBtnState()
{
	FUNCTION_BEGIN;

	stRectI rcClip = GetClipRect();
	int yPixel = rcClip.top - 120;

	//这里不能直接用m_guiFloatAcessKey的GetLocation来计算按钮的位置
	//就算能调的差不多,但是一修改分辨率,位置就又不对了,实在不好伺候
	//还是以m_guiChatSelect(聊天输入框为准),总算能凑活
	//if(GetGameGuiManager()->m_guiFloatAcessKey && !GetGameGuiManager()->m_guiFloatAcessKey->m_bPretendedHide)
	//{
	//	//stPointI pt = GetGameGuiManager()->m_guiFloatAcessKey->GetLocation();
	//	if(GetGameGuiManager()->m_guiChatSelect && GetGameGuiManager()->m_guiChatSelect->IsVisible())
	//	{
	//		stPointI pt = GetGameGuiManager()->m_guiChatSelect->GetLocation();
	//		yPixel = rcClip.top + pt.y - 120 - GetClientY();
	//	}
	//	else
	//	{
	//		yPixel = rcClip.top - 80 - GetClientY();
	//	}
	//}
	//else
	//{
	//	if(GetGameGuiManager()->m_guiChatSelect && GetGameGuiManager()->m_guiChatSelect->IsVisible())
	//	{
	//		stPointI pt = GetGameGuiManager()->m_guiChatSelect->GetLocation();
	//		yPixel = rcClip.top + pt.y - 80 - GetClientY();
	//	}		
	//}
	///这里的做法是排列按钮,8个一行,逐渐往上排,虽然有点累赘,但还是很壮观
	int i = 0;
	int left = (GetDevice()->GetHeight() < 768)?322:422;
	for(stHelpButtons::iterator it = m_helpBtns.begin();
		it != m_helpBtns.end(); ++it)
	{
		stHelpButton *helpBtn = (*it);
		stPointI location = stPointI(rcClip.left + left + 50*(i%8) ,yPixel - 45 *(int) (i/8));		
		if(helpBtn->pBtn->IsVisible())
		{
			helpBtn->pBtn->SetLocation(location.x,location.y);
			++i;
		}		
	}

	FUNCTION_END;
}

size_t CGuiMain::GetShownHelpTipNum()
{
	if(m_helpBtns.empty()) return 0;
	return m_helpBtns.size();
}

/**
* \brief 帮助教程按钮响应函数(打开相应的帮助页面)
* 
* 同时更新该设置，下次就不会再显示按钮了
* 
* \enumUserSetting type : 教程类型
* \return 
*/
void CGuiMain::OnHelpTipBtn(enumUserSetting type)
{
	FUNCTION_BEGIN;

	if( type < USER_SETTING_HELP_MOVE )
		return ;
#define HelpCase(type,file) {case type: GetGameGuiManager()->AddHelp(file);}break
	switch( type )
	{
		HelpCase(USER_SETTING_HELP_MOVE,"data\\help\\move.xml");
		HelpCase(USER_SETTING_HELP_NPC,"data\\help\\npc.xml");
		HelpCase(USER_SETTING_HELP_NPCDIALOG,"data\\help\\npcchat.xml");
		HelpCase(USER_SETTING_HELP_ATTACK,"data\\help\\attact.xml");
		HelpCase(USER_SETTING_HELP_PK,"data\\help\\pk.xml");
		HelpCase(USER_SETTING_HELP_CLEARREDNAME,"data\\help\\killer.xml");
		HelpCase(USER_SETTING_HELP_TASK,"data\\help\\quest.xml");
		HelpCase(USER_SETTING_HELP_RIGHTBTN,"data\\help\\right.xml");
		HelpCase(USER_SETTING_HELP_TRADE,"data\\help\\trade.xml");
		HelpCase(USER_SETTING_HELP_TEAM,"data\\help\\team.xml");
		HelpCase(USER_SETTING_HELP_MEMBERHEAD,"data\\help\\pic.xml");
		HelpCase(USER_SETTING_HELP_ACCELKEY,"data\\help\\quick.xml");
		HelpCase(USER_SETTING_HELP_PACK,"data\\help\\bag.xml");
		HelpCase(USER_SETTING_HELP_EQUIP,"data\\help\\wear.xml");
		HelpCase(USER_SETTING_HELP_EXPACK,"data\\help\\exbag.xml");
		HelpCase(USER_SETTING_HELP_SKILL,"data\\help\\skilluse.xml");
		HelpCase(USER_SETTING_HELP_ADDPROPPOINT,"data\\help\\point.xml");
		HelpCase(USER_SETTING_HELP_STUDY,"data\\help\\skilllearn.xml");
		HelpCase(USER_SETTING_HELP_AUTO_FIND_PATH,"data\\help\\auto.xml");
		HelpCase(USER_SETTING_HELP_ITEM_LINK,"data\\help\\item.xml");
		HelpCase(USER_SETTING_HELP_PRIVATE_CHAT,"data\\help\\chat.xml");
		HelpCase(USER_SETTING_HELP_MATERIAL_COMPOSE,"data\\help\\material.xml");
		//HelpCase(USER_SETTING_HELP_SOULSTONEL_COMPOSE,"data\\help\\combine.xml");
		HelpCase(USER_SETTING_HELP_SOULSTONE_MAKE,"data\\help\\insert.xml");
		HelpCase(USER_SETTING_HELP_ITEM_UPGRID,"data\\help\\itemup.xml");
		HelpCase(USER_SETTING_HELP_ATTACK_MODE_CHANGE,"data\\help\\switch.xml");
		HelpCase(USER_SETTING_HELP_CHAT,"data\\help\\talk.xml");
		HelpCase(USER_SETTING_HELP_SELECT,"data\\help\\choice.xml");	
		HelpCase(USER_SETTING_HELP_ALT_SELECT_NPC,"data\\help\\alt.xml");
	default:
		break;
	}
	GetGameApplication()->GetConfig()->ClearSystemSetOnServer(type);
	UpdateHelpBtnState();

	FUNCTION_END;
}

/**
* \brief 获取一个id，用于帮助教程按钮
* 
* 
* 
* \return 
*/
inline int CGuiMain::GetNextHelpBtnId()
{
	if(m_helpBtns.empty()) return IDC_HELP_TIP_BTN;
	else 
	{
		return m_helpBtns[m_helpBtns.size() - 1]->iBtnId + 1;
	}
}

/**
* \brief 找出一个此前隐藏的帮助教程按钮
* 
* 用于显示一个新的帮助教程
* 
* \return stHelpButton 指针，NULL 表示没有找到
*/
inline CGuiMain::stHelpButton *CGuiMain::GetHideBtn( )
{
	for(stHelpButtons::iterator it = m_helpBtns.begin();
		it != m_helpBtns.end(); ++it)
	{
		stHelpButton *helpBtn = (*it);
		if( !helpBtn->pBtn->IsVisible() ) return helpBtn;
	}
	return NULL;
}

/**
* \brief 显示/隐藏所有按钮(调试用)
* 
* 
* 
* \bool bShow : 显示/隐藏
* \return 
*/
void CGuiMain::ShowAllTipBtn(bool bShow)
{
	FUNCTION_BEGIN;

	for(stHelpButtons::iterator it = m_helpBtns.begin();
		it != m_helpBtns.end(); ++it)
	{
		(*it)->pBtn->SetVisible(bShow);
	}	

	FUNCTION_END;
}

/**
* \brief 该类型的教程有没有打开过
* 
* 且帮助教程开关打开
* 
* \enumUserSetting type : 教程类型
* \return true if 没有打开 false if 打开过
*/
inline bool bIsUserUnSetted(enumUserSetting type)
{	
	return (true == GetGameApplication()->bSystemSetted(type) &&
		true == GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_HELPTIP ));
}

/**
* \brief 显示对应教程按钮
* 
* 
* 
* \enumUserSetting type : 教程类型
* \return 
*/
inline void ShowHelpTip(enumUserSetting type)
{
	if ( GetGameGuiManager()->m_guiMain )
	{
		GetGameGuiManager()->m_guiMain->ShowHelpTipBtn(type);
	}
}


/**
* \brief 显示对应教程按钮
* 
* 之所以分开判断是否设置和显示按钮，是因为某些教程的判断极为耗时，因此应该先判断是否没有设置该教程
* 然后再判断教程的触发条件，最后才显示按钮
* 
* \enumUserSetting type : 教程类型
* \return 
*/


void CGuiMain::DrawScreenCenterMsg()
{
	FUNCTION_BEGIN;
	if(!m_taskMsgList.empty() && CanPrintCenter)
	{
		int yOffset = GetDevice()->GetHeight()/6 + 40;
		DWORD curTime = xtimeGetTime();	

		int iFont = 0;
		if( GetDevice()->GetHeight() < 768 )
		{
			iFont = GetDevice()->SetFont(1);
		}
		else
		{
			iFont = GetDevice()->SetFont(2);
		}

		for(int i = m_taskMsgList.size() - 1 ; i >= 0 ;i --)
		{
			DWORD subTime = curTime - m_taskMsgList[i].dwShowTime;
			DWORD dwBlendColor = -1;
			DWORD dwStopTime = m_taskMsgList[i].dwTotalShowTime * 0.7f;
			if(subTime <= dwStopTime) 
			{
				m_taskMsgList[i].alpha = 255;
			}
			else if( subTime > m_taskMsgList[i].dwTotalShowTime )
			{
				m_taskMsgList[i].alpha = 0;
			}
			else
			{
				m_taskMsgList[i].alpha = ( m_taskMsgList[i].dwTotalShowTime - subTime )*255 /( m_taskMsgList[i].dwTotalShowTime * 0.3f );
				dwBlendColor = COLOR_ARGB(m_taskMsgList[i].alpha/8,255,255,255);
			}

			m_taskMsgList[i].dwColor &= 0x00ffffff;
			m_taskMsgList[i].dwColor |= m_taskMsgList[i].alpha << 24;

			stRectI rcTaskMsg(0,
				yOffset,
				GetDevice()->GetWidth(),
				yOffset + GetDevice()->GetFontHeight()*m_taskMsgList[i].scale.y);

			GetDevice()->DrawString(m_taskMsgList[i].strMsg.c_str(),
				strlen(m_taskMsgList[i].strMsg.c_str()),
				rcTaskMsg,
				m_taskMsgList[i].dwColor,
				DT_CENTER|DT_VCENTER,
				FontEffect_Border,
				ColorBlend(dwBlendColor,c_dwClrDefaultShadow));
			yOffset -= GetDevice()->GetFontHeight();
		}
		GetDevice()->SetFont(iFont);

		TaskMsgListIt itMsg = m_taskMsgList.begin();
		for(;itMsg != m_taskMsgList.end() && (itMsg->alpha < 30) ;++itMsg)
		{}

		if( itMsg != m_taskMsgList.begin())
			m_taskMsgList.erase(m_taskMsgList.begin(),itMsg);
	}
	FUNCTION_END;
}

void CGuiMain::DrawScreenRightBottomMsg(stPointI& pt)
{
	FUNCTION_BEGIN;

	if(!m_expMsgList.empty())
	{
		//错开一点
		pt.y -= 30;
		const DWORD cdwMaxExpMessageShowTime = 14000;
		const DWORD cdwMaxSysMessageShowTime = 30000;


		DWORD time = xtimeGetTime();
		for(int i = m_expMsgList.size() - 1 ; i >= 0 ;i --)
		{
			int alpha = 255;
			DWORD dwBlendColor = -1;

			if(m_expMsgList[i].type == ExpMessage)
			{
				DWORD dwStopTime = cdwMaxExpMessageShowTime*0.7;
				DWORD subTime = time - m_expMsgList[i].dwTime;
				if(subTime > dwStopTime)
				{
					alpha = ( cdwMaxExpMessageShowTime - subTime )*255 /( cdwMaxExpMessageShowTime - dwStopTime );
				}
			}
			if(m_expMsgList[i].type == SystemMessage)
			{
				DWORD dwStopTime = cdwMaxSysMessageShowTime*0.7;
				DWORD subTime = time - m_expMsgList[i].dwTime;
				if(subTime > dwStopTime)
				{
					alpha = ( cdwMaxSysMessageShowTime - subTime )*255 /( cdwMaxSysMessageShowTime - dwStopTime );
				}
			}			
			dwBlendColor = COLOR_ARGB(alpha,255,255,255);
			m_expMsgList[i].dwColor &= 0x00ffffff;
			m_expMsgList[i].dwColor |= alpha << 24;

			GetDevice()->DrawString(m_expMsgList[i].msg,pt,m_expMsgList[i].dwColor,
				FontEffect_Border,ColorBlend(dwBlendColor,c_dwClrDefaultShadow));
			pt.y -= (GetDevice()->GetFontHeight()+3);
		}

		ExpMessageListIt itExp = m_expMsgList.begin();
		for(;itExp != m_expMsgList.end();++itExp)
		{

			if(itExp->type == ExpMessage && time - itExp->dwTime > cdwMaxExpMessageShowTime*0.9) 
			{
				m_expMsgList.erase(itExp);
				return ;
			}
			if(itExp->type == SystemMessage && time - itExp->dwTime > cdwMaxSysMessageShowTime*0.9)
			{
				m_expMsgList.erase(itExp);
				return ;
			}
		}			
	}

	FUNCTION_END;
}

void CGuiMain::__AddMessage(const char* msg,enumMessageType type,DWORD color)
{	
	if(!msg || strlen(msg)<1) return ;

	switch(type)
	{
	case LuckAttackMessage:				//神圣一击消息，显示在屏幕中间
		{
			stScreenMessage taskMsg;
			taskMsg.scale = stPointF(1.0f,1.0f);
			taskMsg.dwColor = color;
			taskMsg.dwShowTime = xtimeGetTime();
			taskMsg.alpha = 255;
			taskMsg.type = LuckAttackMessage;
			taskMsg.dwTotalShowTime = 5000;
			taskMsg.strMsg = msg;
			m_taskMsgList.push_back(taskMsg);
		}
		break;
	case TaskMessage:					//任务消息,显示在屏幕中间
		{
			for(TaskMsgListIt it = m_taskMsgList.begin();
				it != m_taskMsgList.end();++it)
			{
				if(strcmp((it)->strMsg.c_str(),msg) == 0)
				{
					m_taskMsgList.erase( it );
					break;
				}
			}

			int len = 20;
			if( GetDevice()->GetHeight() < 768 )
			{
				len = GetDevice()->GetWidth()/GetDevice()->GetFontWidth(1);
			}
			else
			{
				len = GetDevice()->GetWidth()/GetDevice()->GetFontWidth(2);
			}
			int length = (int )(len * 0.6f);
			
			typedef std::vector<std::string>	strVec;
			SetFrameAllocator fa;
			std::vector<std::stack_string,std::stack_allocator<std::stack_string> > strs;
			_parse_str(strs,msg,'\n');
			stScreenMessage sMsg;
			sMsg.scale = stPointF(1.0f,1.0f);
			sMsg.dwColor = color;
			sMsg.dwShowTime = xtimeGetTime();
			sMsg.alpha = 255;
			sMsg.type = TaskMessage;
			sMsg.dwTotalShowTime = 10000;
			for ( int i = 0; i < strs.size(); ++i )
			{		
				if( strs[i].size() > length )
				{
					strVec	tmpStrs;
					str_split<strVec>(strs[i].c_str(),tmpStrs,length);
					for(size_t j = 0;j < tmpStrs.size();++j)
					{
						sMsg.strMsg = tmpStrs[j].c_str();
						m_taskMsgList.push_back(sMsg);
					}
				}
				else
				{
					sMsg.strMsg = strs[i].c_str();
					m_taskMsgList.push_back(sMsg);
				}
			}
		}
		break;
	case ZoneMessage:				//地图区域消息,显示在屏幕中间
		{
			//把地图区域信息文字放在任务消息里一起显示,只是颜色和文字大小不同
			for(TaskMsgListIt it = m_taskMsgList.begin();
				it != m_taskMsgList.end();++it)
			{
				if( it->type == ZoneMessage )
				{
					m_taskMsgList.erase(it);
					break;
				}
			}
			stScreenMessage zoneMsg;
			zoneMsg.scale = stPointF(1.1f,1.1f);
			zoneMsg.strMsg = msg;
			zoneMsg.dwColor = color;
			zoneMsg.dwShowTime = xtimeGetTime();
			zoneMsg.alpha = 255;
			zoneMsg.type = ZoneMessage;	
			zoneMsg.dwTotalShowTime = 10000;
			m_taskMsgList.push_back(zoneMsg);			
		}
		break;
	case ExpMessage:			//经验值消息,显示在屏幕右下角
	case SystemMessage:			//系统消息,显示在屏幕右下角
		{
			typedef std::vector<std::string> strMsgVec;
			strMsgVec vec;
			str_split<strMsgVec>(msg,vec,30);
			if(vec.empty()) return ;
			DWORD time = xtimeGetTime();
			for(int i = 0;i < vec.size();i++)			
			{
				stExpMessage exp;
				exp.type = type;
				exp.dwTime = time;
				strcpy(exp.msg,vec[i].c_str());
				exp.dwColor = color;
				m_expMsgList.push_back(exp);
			}
			////控制只显示30行
			const int iMaxShowLine = (GetDevice()->GetHeight() < 768)?18:30;
			if(m_expMsgList.size() > iMaxShowLine)
			{
				ExpMessageListIt it = m_expMsgList.begin();
				for(int i = 0;i < m_expMsgList.size()-iMaxShowLine;i++)
				{
					++it;
				}
				m_expMsgList.erase(m_expMsgList.begin(),it);
			}
		}
		break;
	case ScrollMessage:
		{ 
             AddGMTextSprite(msg,color);
		}
		break;
	}
}

void CGuiMain::AddMessage(const char* msg,enumMessageType type,DWORD color)
{
	DEBUG_INT_AT_FUNCTION_BEGIN()

		__AddMessage(msg,type,color);

	DEBUG_INT_AT_FUNCTION_END
}

inline int mmTime2Minute(DWORD dwTime)
{
	return dwTime / 60000;
}

inline int mmTime2Second(DWORD dwTime)
{
	return dwTime / 1000;
}

void CGuiMain::DrawTaskTimeMessage(const POINT& pt)
{
	if ( m_listTaskTime.empty() )
	{
		return;
	}

	stPointI ptPos = pt;
	int iFont = GetDevice()->SetFont(1);
	for(TaskTimeMsgListIt it = m_listTaskTime.begin();
		it != m_listTaskTime.end();++it )
	{
		DWORD dwThisTime = xtimeGetTime();
		DWORD dwTime = dwThisTime - it->dwBeginTime;
		int second = it->dwTaskTime - mmTime2Second(dwTime);
		
		char szTemp[MAX_PATH];
		DWORD dwColor = COLOR_ARGB(255,0,255,0);
		if ( second < 1 )
		{
			it = m_listTaskTime.erase( it ) - 1;
			continue;
		}
		else if ( second < 60 )
		{
			_snprintf(szTemp,MAX_PATH - 1,"%s %d秒",it->strTaskTitle.c_str(),second);
			dwColor = COLOR_ARGB(255,255,0,0);
		}
		else if( second < 3600 )
		{
			int minute = second / 60;
			_snprintf(szTemp,MAX_PATH - 1,"%s %d分%d秒",it->strTaskTitle.c_str(),minute%60,second%60);
		}
		else
		{
			int minute = second / 60;
			_snprintf(szTemp,MAX_PATH - 1,"%s %d小时%d分%d秒",it->strTaskTitle.c_str(),minute/60,minute%60,second%60);
		}
		GetDevice()->DrawString(szTemp,ptPos,dwColor,
			FontEffect_Border,ColorBlend(-1,c_dwClrDefaultShadow));
		ptPos.y += GetDevice()->GetFontHeight();
	}
	GetDevice()->SetFont(iFont);
}

void CGuiMain::AddTaskTimeMessage(DWORD dwTaskID,const char* title,DWORD dwTime)
{
	for(TaskTimeMsgListIt it = m_listTaskTime.begin();
		it != m_listTaskTime.end();++it )
	{
		if( (*it).dwTaskID == dwTaskID )
		{
			it->strTaskTitle = title;
			it->dwTaskTime = dwTime;
			it->dwBeginTime = xtimeGetTime();			
			return;
		}
	}
	stTaskTimeMessage msg;
	msg.dwTaskID = dwTaskID;
	msg.strTaskTitle = title;
	msg.dwTaskTime = dwTime;
	msg.dwBeginTime = xtimeGetTime();			
	m_listTaskTime.push_back(msg);
}

void CGuiMain::RemoveTaskTimeMessage(DWORD dwTaskID)
{
	for(TaskTimeMsgListIt it = m_listTaskTime.begin();
		it != m_listTaskTime.end();++it )
	{
		if( (*it).dwTaskID == dwTaskID )
		{
			m_listTaskTime.erase( it );
			return ;
		}
	}
}

void CGuiMain::AddGMTextSprite(const char * pMsg,DWORD clr)
{
	FUNCTION_BEGIN;

	BOOL bFirst = FALSE;
	if( m_pTextSprite.get() == NULL)
	{
		m_pTextSprite = std::auto_ptr<CGMTextEffect>(new CGMTextEffect);
		if (!m_pTextSprite.get())  return;
		bFirst = TRUE;
	}

	m_pTextSprite->AddText(pMsg);

	if( bFirst )
	{
		m_pTextSprite->SetTextColor(COLOR_ARGB(255,255,255,0));
		m_pTextSprite->SetBkColor(COLOR_ARGB(128,0,0,0));
		m_pTextSprite->SetFont(1);
		m_pTextSprite->SetBackGround(TRUE);
		m_pTextSprite->SetSpeed(stPointF(-50,0));
		m_pTextSprite->SetRenderRect(stRectI(100,50-5,GetDevice()->GetWidth() - 100,80-5));//(-5)微调滚动条位置
	}

	if( !m_pTextSprite->IsPlaying() )
	{
		m_pTextSprite->BeginSprite(false);
	}

	FUNCTION_END;
}

void CGuiMain::PlayAni(stResourceLocation& rl,int nSpeed,int x,int y,bool bLoop)
{
	m_aniMain.Create(&rl);
	m_aniMain.SetSpeed(nSpeed);
	m_aniMain.SetLoopPlay(bLoop);

	m_showX = x;
	m_showY = y;
}

bool CGuiMain::isHasStateSkill(DWORD dwID)
{
	SkillBase_t* pSkill = GetSkillBase(dwID);
	for(std::vector<stStateInfo>::iterator it = m_aStateInfo.begin(); it != m_aStateInfo.end(); ++it)
	{
		if((*it).bMainCharacter){
			CharacterStateBase_t* p = GetCharacterStateBase((*it).dwID);
			if(p && p->dwStateID == pSkill->dwIcon){
				return true;
			}
		}
	}
	return false;
}

bool CGuiMain::initSysMessage(const char* pszFileName)
{
	FUNCTION_BEGIN;

	TiXmlDocument doc;
	Stream* pStream = OpenPackFileForRead("",pszFileName);
	if(!pStream)
	{
		Engine_WriteLog("装载 系统公告本地文件 失败\n");
		return false;
	}
	if(!LoadXMLFromStream(pStream,doc))
	{
		CloseResFile(pStream);
		Engine_WriteLog("解析 系统公告本地文件 失败\n");
		return false;
	}
	TiXmlElement* proot = doc.FirstChildElement("messageSystem");
	if(proot)
	{
		
		TiXmlElement* pp = proot->FirstChildElement("page");
		while(pp)
		{
			stSysMessage pTemp;
			if (pp->Attribute("starttime"))
			{
				pTemp.starttime		= pp->Attribute("starttime");
			}
			else
			{
				pTemp.starttime = "000";
			}
			if (pp->Attribute("login"))
			{
				pTemp.login			= atoi(pp->Attribute("login"));
			}
			else
			{
				pTemp.login = 0;
			}
			if (pp->Attribute("interval"))
			{
				pTemp.interval		= atoi(pp->Attribute("interval"));
			}
			else
			{
				pTemp.interval = -1;
			}
			if (pp->Attribute("endtime"))
			{
				pTemp.endtime		= pp->Attribute("endtime");
			}
			else
			{
				pTemp.endtime = "111";
			}
			if (pp->Attribute("order"))
			{
				pTemp.order			= atoi(pp->Attribute("order"));
			}		
			else
			{
				pTemp.order = 0;
			}
	
			TiXmlElement* pmessage = pp->FirstChildElement("message");
			while (pmessage)
			{
				pTemp.pmessgae.push_back(XML_GetNodeText(pmessage));
				pmessage = pmessage->NextSiblingElement("message");
			}
			m_SysMessage.push_back(pTemp);
			pp = pp->NextSiblingElement("page");
		}
	}
	ClosePackFileForRead(pStream);

//#ifdef _DEBUG
//	for(int i=0;i<m_SysMessage.size();i++)
//	{
//		for(;m_SysMessage[i].j<m_SysMessage[i].pmessgae.size();m_SysMessage[i].j++)
//		{
//			TRACE("------%s-----\n",m_SysMessage[i].pmessgae[m_SysMessage[i].j].c_str());
//		}		
//	}	
//#endif
	

	return true;
	FUNCTION_END;
}

void CGuiMain::OnUpdateSysmessage(float fElapsedTime)
{
	for(int i=0;i<m_SysMessage.size();i++)
	{
		switch(m_SysMessage[i].states)
		{
		case 0:
			if (m_SysMessage[i].login == 1)
			{
				for(int j=0;j<m_SysMessage[i].pmessgae.size();j++)
				{
					GetGameGuiManager()->AddClientSystemMessage(m_SysMessage[i].pmessgae[j].c_str());
				}
				m_SysMessage[i].states = 2;
			}
			else
			{
				m_SysMessage[i].states = 1;
			}
			break;
		case 1:
			if( m_SysMessage[i].dwCurTime == 0)
			{
				m_SysMessage[i].dwCurTime = xtimeGetTime();
				return ;
			}
			else 
			{
				if( xtimeGetTime() - m_SysMessage[i].dwCurTime > m_SysMessage[i].interval*1000)
				{					
					if (m_SysMessage[i].j < m_SysMessage[i].pmessgae.size())
					{
						GetGameGuiManager()->AddClientSystemMessage(m_SysMessage[i].pmessgae[m_SysMessage[i].j].c_str());
						m_SysMessage[i].j++;
					}
					else
					{
						m_SysMessage[i].j = 0;
					}
					
					m_SysMessage[i].dwCurTime = xtimeGetTime();
				}	
			}
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			;
		}			
	}
	
}