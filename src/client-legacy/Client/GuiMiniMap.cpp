#include "public.h"
#include "miniusercommand.h"
#include "game_script.h"
#include "./Game.h"
#include "./GameGuiManager.h"
#include ".\guiminimap.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./CharacterProperty.h"
#include "../RenderD3D/include/RenderD3D.h"
#include "../RenderD3D/include/D3DDevice.h"
#include "../RenderD3D/include/TextureManager.h"
#include "../RenderD3D/include/CustomTexture.h"
#include "./GuiTeam.h"
#include ".\guiworldconn.h"
#include "./GuiPrivateChat.h"
#include "./GuiAutoReplySettingDlg.h"
#include "../luabind/public.h"
#include <numeric>
#include "Country.h"
#include "./Chat.h"
#include "./LordStrikeManager.h"
#include "./LordStrikeLobby.h"
#include "./GameAppation.h"
#include "SafetyUser.h"


void CGuiMiniMap::bind_lua(lua_State* L)
{
	using namespace luabind;
	CGuiDialog::bind_lua(L);
	module(L)
	[
		class_<CGuiMiniMap,CGuiDialog>("CGuiMiniMap")
			.def("OnOffMiniMap",&CGuiMiniMap::OnOffMiniMap)
			.def("OnOffLageMap",&CGuiMiniMap::OnOffLageMap)
			.def("OnLargeMiniMap",&CGuiMiniMap::OnLargeMiniMap)
			.def("OnSmallMiniMap",&CGuiMiniMap::OnSmallMiniMap)
			.def("OnAddPrivateChat",&CGuiMiniMap::OnAddPrivateChat)
			.def("OnAddGMMsg",&CGuiMiniMap::OnAddGMMsg)
			.def("OnAutoReply",&CGuiMiniMap::OnAutoReply)
			.def("OnAutoReplyMenuCmd",&CGuiMiniMap::OnAutoReplyMenuCmd)
			.def("OnPlayMiniGame",&CGuiMiniMap::OnPlayMiniGame)
			.def("OnClickSafetyUser",&CGuiMiniMap::OnClickSafetyUser)			
	];
	m_lua->SetGlobal("this",this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
static stPing c_PingDesc[3] = 
{
	{PING_EQUO_SMALL,"数据传送快速,PING值:1-200",COLOR_ARGB(255,0,255,0)},  // 绿色
	{PING_EQUO_NORMAL,"数据传送正常,PING值:200-600",COLOR_ARGB(255,255,242,0)},     // 黄色
	{PING_EQUO_BIG,"数据传送迟缓,PING值:600以上",COLOR_ARGB(255,255,0,0)},       //红色
};
//////////////////////////////////////////////////////////////////////////////////////////////////

 ////////////////////////////////////////////////////////////////////////////////////////
static int nlogtime = 0;
extern void SendError(const char* pszUserDesc,bool bDebug = false);

#ifdef _DEBUG
#define __Engine_WriteLogF(szFmt)\
{\
	nlogtime++;\
	if( 5 >= nlogtime )\
{\
	Engine_WriteLogF(szFmt);\
	if( 5 == nlogtime)\
	SendError("小地图画不出来！",true);\
}\
}
#else 
#define __Engine_WriteLogF(szFmt) __noop
#endif 

#define MiniMap_ButtonSetVisible(id,bShow)	{CGuiButton* pBtn = GetButton(id);if( NULL != pBtn ){ pBtn->SetVisible(bShow);}}

/////////////////////////////////////////////////////////////////////////////////////////


/***************CNameTip******************/

//const int CNameTip::m_scShowTime = 14000;
///横边切割宽度
const int CNameTip::m_scWidthIncise = 10;
///纵边切割宽度
const int CNameTip::m_scHeightIncise = 10;

/***************CNameTip******************/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CNameTip::CNameTip()
{
	FUNCTION_BEGIN;

	stResourceLocation rl;
	rl.SetFileName( GetGuiGraphicPackName() );
	rl.group = 0;
	rl.frame = 28;
	m_pBmpRect = GetDevice()->FindBitmaps( &rl );
	Assert(m_pBmpRect);

	stPointI s_ptBorderIncise(m_scWidthIncise,m_scHeightIncise);
	stRectI rcTexture;
	stRectI rects[9];
	rcTexture.SetRect(0,0,114,32);
	CuttRect9(rcTexture,s_ptBorderIncise,s_ptBorderIncise,rects);
	for(size_t i=0;i < 9;++i)
	{
		m_bmpRect[i] = rects[i];
	}

	m_width = 180;
	m_height = 100;
	m_cellHeight = 20;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CNameTip::Render(POINT pt)
{
	FUNCTION_BEGIN;

	if(m_strTextVec.empty()) return ;
	for(size_t i = 0;i < 9; ++i)
	{
		stRectI rc = m_renderRect[i];
		rc.OffsetRect(pt.x - m_width / 2,pt.y - m_height/2);
		PatchRenderImage(rc,m_pBmpRect,&m_bmpRect[i],-1,Blend_Null,s_PatchAlignFormat[i]);
	}
	int iFont = GetDevice()->SetFont(1);
	stRectI rc = m_renderRect[1];
	rc.OffsetRect(pt.x - m_width / 2,pt.y - m_height/2);

	for(size_t j = 0;j < m_strTextVec.size();++j)
	{			
		stRectI cel( pt.x - m_width / 2 - 5,
			rc.top + 10 + j * m_cellHeight, 
			pt.x + m_width / 2 + 5,
			rc.top + 10 + ( j  + 1 ) * m_cellHeight );

		GetDevice()->DrawString(m_strTextVec[j].c_str(),cel,-1,DT_CENTER,
			FontEffect_Border,ColorBlend(-1,c_dwClrDefaultShadow));	
	}
	GetDevice()->SetFont(iFont);

	FUNCTION_END;
}
/**
* \brief 添加文字
* 
* 添加文字
* 
* \return 
*/
void CNameTip::AddText(const char *cText,bool bAsLineSymbol) 
{
	FUNCTION_BEGIN;

	if(!bAsLineSymbol && !m_strTextVec.empty()) m_strTextVec.clear();
	if(!cText || strlen(cText) < 1) 
	{
		return ;
	}
	if( bAsLineSymbol )
		str_split<strVec>(cText,m_strTextVec,-1);
	else
		str_split<strVec>(cText,m_strTextVec,10);

	if( !bAsLineSymbol )
	      CalculateCutRect();
	FUNCTION_END;
}


void CNameTip::CalculateCutRect()
{
	stPointI s_ptBorderIncise(m_scWidthIncise,m_scHeightIncise);

	if(!m_strTextVec.empty()) 
	{
		stRectI rc(0,0,50,25);
		GetDevice()->GetStringRect(m_strTextVec[0].c_str(),m_strTextVec[0].length(),rc,DT_CENTER);
		if(m_strTextVec.size() > 1) 
			m_width = 200;
		else 
		{
			m_width = rc.Width() + 50;
			m_width = (m_width > 70) ? m_width : 70;
		}
		m_cellHeight = rc.Height() + 5;
		m_height = m_strTextVec.size() * m_cellHeight + 20;
	}

	stRectI renderBack(0,0,m_width,m_height);
	CuttRect9(renderBack,s_ptBorderIncise,s_ptBorderIncise,m_renderRect);
}

CNameTip *GetNameTip()
{
	static CNameTip tip;
	return &tip;
}



/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiMiniMap::CGuiMiniMap(void)
: m_pMemBmp(NULL)
, m_pRenderBmp(NULL)
, m_pBackAlpha(NULL)
, m_dwUpdateTime(0)
, m_dwUpdateCMDTime(0)
, m_bRenderTarget(false)
, m_bUpdateSuccess(false)
, m_nScale(1)
, m_pExitIcon(NULL)
, m_pExitToolTips(NULL)
, m_pNpcIcon(NULL)
, m_pCoupleCharacterIcon(NULL)
, m_rcText(45,33,200,47)
, m_rcTextLeft(45,33,122,47)
, m_rcTextRight(122,33,200,47)
, m_AutoReplyMenu(NULL)
, m_ptBmpScale(32,32)
{
	FUNCTION_BEGIN;

	m_BtnShow = NULL;
	m_BtnHide = NULL;
	m_BtnLargeMap = NULL;
	m_imgCircle = NULL;
	m_nSBWidth = 22;
	m_clientOffsetTopLeft = stPointI(3,18);
	m_clientOffsetRightBottom = stPointI(3,3);
	m_ptCaptionOffset = stPointI(0,0);
	m_minSize = stPointI(200,100);

	
	m_rlMiniBtn.SetFileName("data\\interfaces.gl");
	m_rlMiniBtn.group = 1;
	m_rlMiniBtn.frame = 498;
 
	m_dwStartTimeMiniBtn = 0;

	FUNCTION_END
}

CGuiMiniMap::~CGuiMiniMap()
{
	FUNCTION_BEGIN;
	if( m_pExitToolTips )
	{
		delete m_pExitToolTips;
		m_pExitToolTips = NULL;
	}

	FUNCTION_END
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiMiniMap::OnClose(void)
{
	FUNCTION_BEGIN;

	m_BtnShow = NULL;
	m_BtnHide = NULL;
	m_BtnLargeMap = NULL;
	m_imgCircle = NULL;

	ReleaseMiniMap();

	UnInitDevice();

	GetGameGuiManager()->m_guiMiniMap = NULL;

	if(m_pBackAlpha)
	{
		GetTextureManager()->DeleteCustomeTexture(m_pBackAlpha);
		m_pBackAlpha = NULL;
	}
	if( m_AutoReplyMenu )
	{
		m_AutoReplyMenu->Close();
		m_AutoReplyMenu = NULL;

		if( GetGameGuiManager()->m_guiAutoReply )
		{
			GetGameGuiManager()->m_guiAutoReply->SetAutoReplyMenu(NULL);
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
void CGuiMiniMap::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	m_BtnHide = GetButton( 100 );
	m_BtnShow = GetButton( 101 );
	m_BtnLargeMap = GetButton( 102 );
	m_BtnShow->SetVisible( false );

	m_imgCircle = GetImage( 2 );

	m_pBackAlpha = GetTextureManager()->CreateCurstomTexture(256,256,D3DFMT_A4R4G4B4,0);
	UpdateLocation(true);

	m_Ping = PING_EQUO_NORMAL;
	SetPing(m_Ping);
	m_vecTimeStamp.clear();

	stResourceLocation rl(GetGuiGraphicPackName(),1,412);
	m_pExitIcon = GetDevice()->FindBitmaps(&rl);
	if( m_pExitIcon )
	{
		IBitmap * pBitmap = m_pExitIcon->GetBitmap(0);
		m_rcExitClip = pBitmap->GetClipRect();
	}

	rl.frame = 416;
	m_pNpcIcon = GetDevice()->FindBitmaps(&rl);

	m_NewMailBtn = GetButton(108);

	m_AutoReplyMenu = GetGuiManager()->CreateMenu( this );
	if( m_AutoReplyMenu )
	{
		int size = sizeof(g_sAutoReplyCmd)/sizeof(stAutoReply);
	
		for ( int i = 0; i < size; i++ )
		{
			char text[MAX_PATH];
			_snprintf(text,MAX_PATH,"%d.%s",i+1,g_sAutoReplyCmd[i].szReplyText);
			text[MAX_PATH - 1] = 0;
			m_AutoReplyMenu->AddMenuItem(g_sAutoReplyCmd[i].type,text,CGuiMenu::MenuStyle_Command);
		}
		m_AutoReplyMenu->AddMenuItem(AutoReply_Null,"",CGuiMenu::MenuStyle_Separate);

		bool bSettedAutoReply = GetGameApplication()->GetConfig()->bClientSetted(enumAuto_Reply);
		m_AutoReplyMenu->AddMenuItem(AutoReply_Switch,"关闭自动回复",CGuiMenu::MenuStyle_Command,bSettedAutoReply);

		GetButton(109)->SetToolTips("自动回复:关闭");
	}
	
	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param ptScreen : 描述
* \return 返回值的描述
*/
BOOL CGuiMiniMap::ContainsPoint(POINT  ptScreen)
{
	FUNCTION_BEGIN;

	if(CGuiDialog::ContainsPoint(ptScreen)) return TRUE;
	stPointI pt1 = ptScreen;
	pt1.x -= m_x + m_clientOffsetTopLeft.x;
	pt1.y -= m_y + m_clientOffsetTopLeft.y;
	if(this->GetControlAtPoint(pt1)) return TRUE;
	return FALSE;

	FUNCTION_END;
}

void CGuiMiniMap::OnPlayMiniGame()
{
        FUNCTION_BEGIN;
 
	CLordStrikeManager::GetSingleton()->LogintoLobby();
 
	FUNCTION_END;
}

void CGuiMiniMap::OnClickSafetyUser()
{
	CSafetyUser::Instance()->OnBtnClick();
}

void CGuiMiniMap::UpdateSafetyUser()
{
 
		CGuiButton *pBtn = GetButton(111);
		if( pBtn )
		{
			//pBtn->SetButtonType(CGuiButton::ButtonType_Bitmap);
	
			if(CSafetyUser::Instance()->IsStateSafetyOpen())
			{
				pBtn->SetToolTips("安全保护打开!");
				pBtn->SetBitmap(stResourceLocation("data\\Interfaces.gl",137,4),3);
			}
			else if( CSafetyUser::Instance()->IsStateTmpClose() ||  CSafetyUser::Instance()->IsStateClose())
			{
				if(CSafetyUser::Instance()->IsStateTmpClose() )
					pBtn->SetToolTips("安全保护关闭!");
				else 
					pBtn->SetToolTips("安全保护暂时关闭!");
				pBtn->SetBitmap(stResourceLocation("data\\Interfaces.gl",137,1),3);
			}

		}
}
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiMiniMap::OnOffMiniMap()
{
	FUNCTION_BEGIN;

	if(GetScene()->IsDarkScene())
	{
		ShowMiniMap(false);
	}
	else
	{
		if ( m_BtnHide->IsVisible() )
			ShowMiniMap(false);
		else
			ShowMiniMap(true);

	}
	
	FUNCTION_END;
}

void CGuiMiniMap::OnAddGMMsg()
{
	FUNCTION_BEGIN;
	GetGameGuiManager()->AddGmMsgBoxDlg();
	FUNCTION_END;
}

void CGuiMiniMap::OnAutoReply()
{
	FUNCTION_BEGIN;

	if( m_AutoReplyMenu )
	{
		SetPopupMenu( m_AutoReplyMenu );
		m_AutoReplyMenu->Popup();
	}	

	FUNCTION_END;
}

void CGuiMiniMap::OnAutoReplyMenuCmd(int nID)
{
	FUNCTION_BEGIN;

	if( !m_AutoReplyMenu )
	{
		return ;
	}

	switch( nID )
	{
	case AutoReply_ComebackSoon:
	case AutoReply_BusyNow:
	case AutoReply_MeetingNow:
	case AutoReply_PhoneNow:
	case AutoReply_EatingNow:
	case AutoReply_NotHere:
	case AutoReply_RestingNow:
		{
			GetGameApplication()->SetAutoReplyType( (enumAutoReplyType)nID );
			m_AutoReplyMenu->SetMenuItemCheck(nID);
			GetGameApplication()->GetConfig()->SetClientSet( enumAuto_Reply,true );
			m_AutoReplyMenu->SetMenuItemEnbale(AutoReply_Switch,true);
			char szTemp[MAX_PATH];
			_snprintf(szTemp,MAX_PATH,"自动回复:%s",g_sAutoReplyCmd[nID].szReplyText);
			szTemp[MAX_PATH-1] = 0;
			SetAutoReplyButtonToolTips(szTemp);
		}
		break;
	case AutoReply_UserDefine:
		{
			GetGameGuiManager()->AddAutoReply(m_AutoReplyMenu);	
		}
		break;
	case AutoReply_Switch:
		{
			bool bAutoReply = GetGameApplication()->GetConfig()->bClientSetted(enumAuto_Reply);
			GetGameApplication()->GetConfig()->SetClientSet( enumAuto_Reply,!bAutoReply);
			m_AutoReplyMenu->SetMenuItemEnbale(AutoReply_Switch,!bAutoReply);	
			if( bAutoReply )
			{
				m_AutoReplyMenu->ClearAllCheck();
				SetAutoReplyButtonToolTips("自动回复:关闭");
			}
		}
		break;
	}
	FUNCTION_END;
}

void CGuiMiniMap::SetAutoReplyButtonToolTips(const char* tips)
{
	GetButton(109)->SetToolTips(tips);
}

void CGuiMiniMap::ShowMiniMap(bool bShow)
{
	FUNCTION_BEGIN;

#define MiniMap_SetVisible(id,b) {CGuiControl* p = GetControl(id);if(p){p->SetVisible(b);}}
	m_BtnHide->SetVisible( bShow );
	m_BtnShow->SetVisible( !bShow );
	m_BtnLargeMap->SetVisible( bShow );
	m_imgCircle->SetVisible( bShow );

	MiniMap_SetVisible(103,bShow);
	MiniMap_SetVisible(104,bShow);
	MiniMap_SetVisible(105,bShow);
	MiniMap_SetVisible(106,bShow);
	MiniMap_SetVisible(107,bShow);
	MiniMap_SetVisible(109,bShow);
	MiniMap_SetVisible(110,bShow);

	bool bNewMail =  GetScene()->GetMainCharacter()->GetMailManager()->bIsHaveNewMail();
	if( m_NewMailBtn && bNewMail && bShow )
	{
		m_NewMailBtn->SetVisible(true);
	}
	else
	{
		m_NewMailBtn->SetVisible(false);
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
void CGuiMiniMap::OnOffLageMap()
{
	FUNCTION_BEGIN;

	if( GetScene()->IsDarkScene() )
	{
		return ;
	}
	if(m_pMemBmp)
	{
		if(GetGameGuiManager()->m_guiLageMap)
			GetGameGuiManager()->m_guiLageMap->Close();
		else
		{
			CGuiLageMap* pMap = GetGameGuiManager()->AddLageMap();
			if(pMap) 
			{
				pMap->m_pBmp = m_pMemBmp;
				pMap->InitRect();
				pMap->m_pVecInterestNpc = &m_vecIntersetNpc;
				pMap->m_pVecCoupleCharacter = &m_Couple;
			}
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
void CGuiMiniMap::OnSmallMiniMap()
{
	FUNCTION_BEGIN;

	if ( m_nScale > 0.25f )
	{
		if ( m_nScale > 1.0f )
			m_nScale -= 0.5f;
		else
			m_nScale -= 0.25f;
		UpdateRenderBmp();
	}
	if ( m_nScale <= 0.25f )
		GetButton( 104 )->SetEnabled( false );

	if ( (m_nScale < 1.5f) && !(GetButton( 103 )->IsEnabled()) )
		GetButton( 103 )->SetEnabled( true );

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiMiniMap::OnLargeMiniMap()
{
	FUNCTION_BEGIN;
	
	if ( m_nScale < 1.5f )
	{
		if ( m_nScale >= 1.0f )
			m_nScale += 0.5f;
		else
			m_nScale += 0.25f;
		UpdateRenderBmp();
	}

	if ( m_nScale >= 1.5f )
		GetButton( 103 )->SetEnabled( false );

	if ( (m_nScale > 0.25f ) && !(GetButton( 104 )->IsEnabled()) )
		GetButton( 104 )->SetEnabled( true );
	FUNCTION_END;
}

void CGuiMiniMap::OnAddPrivateChat()
{
	FUNCTION_BEGIN;
	
	if(  GetGameGuiManager()->m_guiCharacterProperty && GetGameGuiManager()->m_guiCharacterProperty->IsVisible() && GetScene()->m_pInterestNpc && GetScene()->m_pInterestNpc->IsCharacter())
	{
		GetChatManager().PrivateChatWith( GetScene()->m_pInterestNpc->GetName() );
	}
	else
	{
		GetGameGuiManager()->AddChatInvate();
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
void CGuiMiniMap::OnInitScript()
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiMiniMap::UpdateLocation(bool bReAlign)
{
	FUNCTION_BEGIN;

	m_y = -26;
	m_x = GetDevice()->GetWidth() - 204;
	m_borderTopLeft = stPointI(0,0);
	m_borderRightBottom = stPointI(0,0);
	m_clientOffsetTopLeft = stPointI(0,0);
	m_clientOffsetRightBottom = stPointI(0,0);
	m_clientOffsetTopLeft = stPointI(0,0);

	m_width = 204;
	m_height = 205;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiMiniMap::ReleaseMiniMap()
{
	FUNCTION_BEGIN;

	m_pMemBmp = NULL;

	FUNCTION_END;
}



/**
* \brief 简短描述
* 
* 详细描述
* 
* \param szMapName : 描述
* \return 返回值的描述
*/
bool CGuiMiniMap::LoadMiniMap(const char* szMapName)
{
	FUNCTION_BEGIN;

	ReleaseMiniMap();
	CIni ini;
	ini.Open("data\\datas.pak","datas\\minimap.ini");
	int id = ini.GetInt("map",szMapName,0);
	stResourceLocation rl("data\\minimap.pak",0,id);
	if(id <= 0)
		Engine_WriteLogF("MiniMap %s = %d\n",szMapName,id);
	m_pMemBmp = GetDevice()->FindBitmap(&rl);
	if( m_pMemBmp )
	{
		/////////////////////////////////////////////////////////////
		m_ptBmpScale.x  = (float)(GetScene()->GetWidth() * GRID_WIDTH)  / m_pMemBmp->GetWidth();
		m_ptBmpScale.y = (float)(GetScene()->GetHeight() * GRID_HEIGHT) / m_pMemBmp->GetHeight(); 
		/////////////////////////////////////////////////////////////
	}	

	stResourceLocation rl2(GetGuiGraphicPackName(),20,4);
	m_pMainRoleIcon = GetDevice()->FindBitmaps(&rl2);
	rl2.frame = 3;
	m_pTeamIcon     = GetDevice()->FindBitmaps(&rl2);
	rl2.frame = 1;
	m_pTaskNpcIcon  = GetDevice()->FindBitmaps(&rl2);

	rl2.frame = 2;
	m_pPreTaskNpcIcon = GetDevice()->FindBitmaps(&rl2);

	rl2.frame = 6;
	m_pCoupleCharacterIcon = GetDevice()->FindBitmaps(&rl2);

	InitTextEffect();	


	return true;

	FUNCTION_END;
}

void CGuiMiniMap::InitTextEffect()
{
	FUNCTION_BEGIN;

	m_TextSprite.Stop();
	m_TextSprite.SetTextColor(COLOR_ARGB(255,255,255,255));
	m_TextSprite.SetBackGround(FALSE);
	m_TextSprite.SetFont(0);
	m_TextSprite.SetSpeed(stPointF(-30,0));
	stRectI rcText = m_rcText;
	rcText.OffsetRect(m_x,m_y);
	m_TextSprite.SetRenderRect(rcText);
	rcText = m_rcTextLeft;
	rcText.OffsetRect(m_x,m_y);
	m_TextSprite.rcRenderLeft = rcText;
	rcText = m_rcTextRight;
	rcText.OffsetRect(m_x,m_y);
	m_TextSprite.rcRenderRight = rcText;
	
	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CGuiMiniMap::InitDevice()
{
	FUNCTION_BEGIN;

	if(m_pRenderBmp == NULL)
	{
		D3DDISPLAYMODE mode;
		GetD3DDevice()->__GetDisplayMode(&mode);
		if(GetD3DDevice()->IsDoubleTexture())
			m_pRenderBmp = GetTextureManager()->CreateRenderTargetTexture(256,256,D3DFMT_R5G6B5);
		else
			m_pRenderBmp = GetTextureManager()->CreateRenderTargetTexture(256,256,mode.Format);
	}
		
	if(!m_pRenderBmp) return false;
		
	return m_pRenderBmp->Init();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiMiniMap::UnInitDevice()
{
	FUNCTION_BEGIN;

	if(m_pRenderBmp)
	{
		GetTextureManager()->DeleteCustomeTexture(m_pRenderBmp);
		m_pRenderBmp = NULL;
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
void CGuiMiniMap::Update()
{
	FUNCTION_BEGIN;

	if ( m_BtnShow->IsVisible() )
		return;

	if(xtimeGetTime() - m_dwUpdateTime > 640)
	{
		m_dwUpdateTime = xtimeGetTime();
		m_bUpdateSuccess = UpdateRenderBmp();
		UpdateExitFlag();
	}

	if( m_dwUpdateCMDTime == 0)
		m_dwUpdateCMDTime = xtimeGetTime();
	DWORD dwDiff = xtimeGetTime() - m_dwUpdateCMDTime;
	if( dwDiff > 60000  * 3 ) // 每隔3分钟重新发起1次小ping 
	{
		if(m_vecTimeStamp.size() == 0)  // 3分钟内还没有收到ping echo，不用说，很慢了
		{
			m_vecTimeStamp.clear();
			m_Ping = PING_EQUO_BIG;
			SetPing(m_Ping);

		}
		m_dwUpdateCMDTime = xtimeGetTime();
		stPingTimeTimerUserCmd  cmd;
		SEND_USER_CMD(cmd);	
	}


	bool bNewMail =  GetScene()->GetMainCharacter()->GetMailManager()->bIsHaveNewMail();
	if( m_NewMailBtn )
	{
		m_NewMailBtn->SetVisible(bNewMail);
	}

	if( xtimeGetTime()  - m_dwStartTimeMiniBtn  >= 500)
	{
		CGuiButton * pBtn = GetButton(110);
		if(pBtn)
		{
			
			if(m_rlMiniBtn.frame == 500)
			{
				m_rlMiniBtn.frame = 498;
				pBtn->SetBitmap(m_rlMiniBtn,3);
			}
			else 
			{
				m_rlMiniBtn.frame = 500;
				pBtn->SetBitmap(m_rlMiniBtn,1);
			}
		}	
		m_dwStartTimeMiniBtn = xtimeGetTime();
	}
	FUNCTION_END;
}


///////////////////////////////////////////////////////////////////////////////
 inline float standAngle(float alpha)
{
	FUNCTION_BEGIN;

	if(alpha < 0) 
	{
		float f = -alpha;
		int i = f/(PI*2.0f);
		alpha = PI*2.0f - (f - i*PI*2.0f);
	}

	if(alpha > PI*2.0f) 
	{
		float f = alpha;
		int i = f/(PI*2.0f);
		alpha = f - i*PI*2.0f;
	}
	return alpha;
	FUNCTION_END;
}
////////////////////////////////////////////////////////////////////////////////

void CGuiMiniMap::UpdateExitFlag()
{
	FUNCTION_BEGIN;

	CGameScene* pScene = GetScene();
	assert(pScene != NULL);

	const float f_radius = m_rectAlphaUV.Width()/2;

	m_ExitInfo.clear();
	stPointI ptGridPos = GetScene()->GetMainCharacter()->GetGridPos();


	// circle center 
	int nCircleX = f_radius + m_x + 54;
	int nCircleY = f_radius + m_y + 54;

	
	std::map<DWORD,CGameScene::stExitInfo> & Exits= pScene->GetExitInfo();
	for(std::map<DWORD,CGameScene::stExitInfo>::iterator it = Exits.begin();it != Exits.end(); ++it)
	{
		CGameScene::stExitInfo & ExitInfo = it->second;

		float detax = ExitInfo.x - ptGridPos.x;
		float detay = ExitInfo.y - ptGridPos.y;
		detay = - detay;     // 将游戏坐标系转成我所熟悉的标准笛卡尔坐标系
		/* if( detax * detax + detay * detay < f_radius * f_radius)
		{    
		break;
		}*/
		float theta;
		if( ExitInfo.x == ptGridPos.x )
		{
			theta = PI /2.0f;
			if( ExitInfo.y > ptGridPos.y)
				theta += PI;
		}
		else
		{
			theta = atan((float)(detay / detax));
			if( theta <= 0)
			{
				if( detax < 0) 
				{
					// the 2 quadrant
					theta += PI;
				}
				else
				{
					// the 4 quadrant
					theta += 2 *PI;
				}
			}
			else
			{
				if( detax < 0)
				{
					// the 3 quadrant 
					theta  += PI;
				}
			}
		}
		theta = standAngle(theta);

		float xx = cos(theta) * f_radius;
		float yy = sin(theta) * f_radius;		

		szExitFlag flag;
		flag.x = nCircleX + xx ;
		flag.y = nCircleY - yy;
		flag.theta =  theta;
		strncpy(flag.szName,ExitInfo.szName,sizeof(ExitInfo.szName));
		m_ExitInfo.push_back(flag);
	}

	FUNCTION_END;
}


void CGuiMiniMap::EchoPing()
{
	FUNCTION_BEGIN;

	DWORD diff = xtimeGetTime() - m_dwUpdateCMDTime;
	m_vecTimeStamp.push_back(diff);

	
	//DWORD nAvrPing = accumulate(m_vecTimeStamp.begin(),m_vecTimeStamp.end(),0) / m_vecTimeStamp.size();

	m_vecTimeStamp.clear();
	if( diff < 200 )
		m_Ping = PING_EQUO_SMALL;
	else if( diff < 600 )
		m_Ping = PING_EQUO_NORMAL;
	else 
		m_Ping = PING_EQUO_BIG;
	SetPing(m_Ping);
	     

	FUNCTION_END;
}



/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CGuiMiniMap::UpdateRenderBmp()
{
	FUNCTION_BEGIN;
	HRESULT hr;
	
	if(!m_pMemBmp) 	return false;

	if(!GetD3DDevice()->TestCooperativeLevel())
	{	 
		if(!m_pRenderBmp) return false;

		m_pRenderBmp->EndRenderTarget();

		return false;
	}

	if(!InitDevice())    return false;

	if(!m_pRenderBmp->BeginRenderTarget())		return false;


	if(!GetD3DDevice()->BeginScene())
	{
		m_pRenderBmp->EndRenderTarget();
		return false;
	}

	stRectI rcBack(0,0,256,256);
	D3DRECT d3dRectBack = {0,0,256,256};
	hr = GetD3DDevice()->__Clear(1,&d3dRectBack,D3DCLEAR_TARGET,0,1.0f,0);

	bool bShowAll = GetGameApplication()->GetConfig()->bSystemSetted(USER_SETTING_SHOW_ALLMINIMAP);

	CGameScene* pScene = GetScene();
	
#if 1
	stRectI rcMapPixel,rcMiniMapPixel;
	stPointI ptMapPixelOffset(0,0),ptMiniMapPixelOffset(0,0);


	//////////////////////////////////////////////////////////////////////////
	//const int bmpScale = 32;
	//int minMapScale = bmpScale / m_nScale;
	// If the map is very small,the ignorance of float-int may be catastrophic.
	stPointF minMapScale;
	minMapScale.x  = m_ptBmpScale.x / m_nScale;
	minMapScale.y = m_ptBmpScale.y / m_nScale;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	const SIZE miniMapSize = {256,256};
	const SIZE mapSize = { 256 * minMapScale.x,256 * minMapScale.y };

	stPointI ptMainPixel = GetScene()->GetMainCharacter()->GetPos();

	// 对应的大地图的像素区域 
	rcMapPixel.SetRect(ptMainPixel.x - mapSize.cx / 2, ptMainPixel.y - mapSize.cy / 2,
		ptMainPixel.x - mapSize.cx / 2 + mapSize.cx, ptMainPixel.y - mapSize.cy / 2 + mapSize.cy);

	stRectI rcMapTotalPixel(0,0,GetScene()->GetWidth() * GRID_WIDTH, GetScene()->GetHeight() * GRID_HEIGHT);

	if(rcMapPixel.left < 0)	ptMapPixelOffset.x = -rcMapPixel.left;
	if(rcMapPixel.top < 0) ptMapPixelOffset.y = -rcMapPixel.top;

	rcMapPixel &= rcMapTotalPixel;

	// 小地图的像素区域
	rcMiniMapPixel.SetRect(rcMapPixel.left/minMapScale.x,rcMapPixel.top/minMapScale.y,rcMapPixel.right/minMapScale.x,rcMapPixel.bottom/minMapScale.y);
	ptMiniMapPixelOffset = stPointI(ptMapPixelOffset.x/minMapScale.x,ptMapPixelOffset.y/minMapScale.y);

	if(m_pMemBmp) 
	{
		stRectI rcBmpRect(rcMapPixel.left/m_ptBmpScale.x,rcMapPixel.top/m_ptBmpScale.y,rcMapPixel.right/m_ptBmpScale.x,rcMapPixel.bottom/m_ptBmpScale.y);
		stPointF scale((float)rcMiniMapPixel.Width()/(float)rcBmpRect.Width(),(float)rcMiniMapPixel.Height()/(float)rcBmpRect.Height());
		m_pMemBmp->Render(ptMiniMapPixelOffset.x,ptMiniMapPixelOffset.y,&rcBmpRect,&scale);
	}
	stPointI MiniMapCenterPt(128,128);
	GetNameTip()->Close();
	//int mw,mh;
	//mw = max(2,2 * m_nScale);
	//mh = max(2,2 * m_nScale);
	int mw = 20;
	int mh = 20;

	stPointI MinimapLoca = GetLocation();
	stPointI ptCursor = Engine_GetCursor()->GetPosition();
	stPointI pointCur = ptCursor - MinimapLoca;

	for(CGameScene::tListNpc::iterator it = pScene->m_listNpc.begin(); it != pScene->m_listNpc.end(); ++it)
	{
		if(isset_state((*it)->GetState(), USTATE_HIDE_PET_TRAINING))
			continue;
		stPointI ptPos = (*it)->GetPos();
		ptPos.x -= rcMapPixel.left;
		ptPos.y -= rcMapPixel.top;
		ptPos.x/=minMapScale.x;
		ptPos.y/=minMapScale.y;
		ptPos.x += ptMiniMapPixelOffset.x;
		ptPos.y += ptMiniMapPixelOffset.y;
		if(isset_state((*it)->GetState(),USTATE_FINISH_QUEST) && m_pTaskNpcIcon)
		{	
			//可以交任务的npc
			stRectI rect(ptPos.x-mw/2,ptPos.y-mh/2,ptPos.x + mw,ptPos.y + mh);
			m_pTaskNpcIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),-1,Blend_Null);

			stRectI rc = rect;
			rc.OffsetRect(MinimapLoca);
			if(rc.PtInRect(ptCursor))
			{
				GetNameTip()->AddText((*it)->GetName());
				//GetNameTip()->DrawText((*it)->GetName(),MiniMapCenterPt);
			}
			continue;
		}
		if(isset_state((*it)->GetState(),USTATE_START_QUEST) && m_pPreTaskNpcIcon)
		{	
			//可以交任务的npc
			stRectI rect(ptPos.x-mw/2,ptPos.y-mh/2,ptPos.x + mw,ptPos.y + mh);
			m_pPreTaskNpcIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),-1,Blend_Null);

			stRectI rc = rect;
			rc.OffsetRect(MinimapLoca);
			if(rc.PtInRect(ptCursor))
			{
				GetNameTip()->AddText((*it)->GetName());
			}
			continue;
		}
		else if( bShowAll )
		{
			DWORD color = 0;
			CNpc * pNpc = *it;
			
			const petType & pettype = pNpc->GetPetType();
			bool bIsDie = pNpc->IsDie(); 

			if( pettype != PET_TYPE_NOTPET)
			{
				switch(pettype)
				{
				case PET_TYPE_NOTPET:
					break;
				case PET_TYPE_GUARDNPC: //例如镖车
					{
						DWORD dwMasterID = pNpc->GetMasterID();
						bool bGuard = false;
						if( dwMasterID != -1 )
						{ // 盟国镖车在小地图上显示为紫点；
							CCharacter * pMaster = GetScene()->FindCharacterByID(dwMasterID);
							if( pMaster)
							{	
								if( CCountry::Instance()->IsAllyCountry(pMaster->GetCountry()) )
								{
									color = COLOR_ARGB(255,128,0,128);
									bGuard = true;
								}
							}
						}
						if( !bGuard )
						{
							color = COLOR_ARGB(255,204,0,255);
						}
					}
					break;
				case PET_TYPE_PET: // 宠物
					color = COLOR_ARGB(255,0,48,255);
					break;
				case PET_TYPE_TOTEM: //图腾
					color = COLOR_ARGB(255,255,51,204);
					break;
				case PET_TYPE_SUMMON://召唤兽类
					color = COLOR_ARGB(255,102,255,204);
					break;
				}
			}
			else
			{
				enumNpcTypeItem dwType = pNpc->GetNpcTypeItem();
				switch(dwType)
				{
				case 1:// 怪物
				case 2:
				case 3:
				case 4:
				case 5:
				case 0:
				case 10:
				case 21:
				case NpcTypeItem_ExGold://紫色怪
					color = COLOR_ARGB(255,255,0,0);
					break;
				case 6: // 除怪物以外的功能npc
				case 7:
				case 15:
				case 16:
				case 19:
				case 8:
				case 24: //邮递功能npc
				case 25:
					color = COLOR_ARGB(255,255,255,255);
					break;
				}
			}			
			
			stRectI rect(ptPos.x-2,ptPos.y-2,ptPos.x + 2,ptPos.y + 2);
			if( color != 0 && !bIsDie) //&& COLOR_GET_ALPHA(color) > 0x0f)
			{
				if(m_pNpcIcon )
					m_pNpcIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),color,Blend_Null);
			}
		}
	}

        ///////////////////////////////////////////////////////////////////////////////
	m_Couple.clear();
	m_listTeam.clear();
	m_UnKnownPerson.clear();
	CCharacter * pMainCharacter = NULL;
	if( bShowAll)
	{
		m_Family.clear();
		m_OtherNation.clear();
		m_AllyNation.clear();
		m_Friend.clear();
		m_Tong.clear();
		m_PkOver.clear();	
		m_General.clear();
	}

	CGuiTeam *pTeamDlg = GetGameGuiManager()->m_guiTeam;

	// 注意不能用GetCountry()，因为当变成蒙面巾后，服务器会清除CCharacter的goodness和country，
	// 所以应该用CMainCharacter的Property
	DWORD dwOwnCountry  =  pScene->m_pMainCharacter->GetProperty()->country;

	for(CGameScene::tListCharacter::iterator it = pScene->m_listCharacter.begin(); it != pScene->m_listCharacter.end(); ++it)
	{
		if(isset_state((*it)->GetState(), USTATE_HIDE_PET_TRAINING))
			continue;

		CCharacter * pCharacter = *it;
		
		if((*it)->IsMainRole())
		{
			pMainCharacter = pCharacter;
			continue;
		}

		if( (*it)->IsUnKnowPerson())
		{
			m_UnKnownPerson.push_back(pCharacter);
			continue;
		}

		if( GetGameGuiManager()->m_guiWorldConn  )
		{
			// 夫妻 m_Couple
			if( GetGameGuiManager()->m_guiWorldConn->IsMyRelation(pCharacter->GetName(),RELATE_CONSORT))
			{
				m_Couple.push_back(pCharacter);
				continue;
			}
		}
		if( !pCharacter->IsMainRole() && pTeamDlg && pTeamDlg->FindMember( pCharacter->GetID() ) )
		{
			m_listTeam.push_back(*it);
			continue;
		}
		if( bShowAll)
		{
			if( GetGameGuiManager()->m_guiWorldConn  )
			{
				// 家族成员m_Family
				if( GetGameGuiManager()->m_guiWorldConn->IsMyRelation(pCharacter->GetName(),SEPT))
				{
					m_Family.push_back(pCharacter);
					continue;
				}
			}
			// 别国玩家
			if( (*it)->GetCountry() !=  dwOwnCountry )
			{
				if( CCountry::Instance()->IsAllyCountry(   (*it)->GetCountry()  ) )
				{
					m_AllyNation.push_back(pCharacter);
				}
				else
				{
					m_OtherNation.push_back(pCharacter);
				}
				continue;
			}
			if( GetGameGuiManager()->m_guiWorldConn  )
			{
				// 好友m_Friend
				if( GetGameGuiManager()->m_guiWorldConn->IsMyRelation(pCharacter->GetName(),FRIEND))
				{
					m_Friend.push_back(pCharacter);
					continue;
				}
				// 帮会成员m_Tong
				if( GetGameGuiManager()->m_guiWorldConn->IsMyRelation(pCharacter->GetName(),TONG))
				{
					m_Tong.push_back(pCharacter);
					continue;
				}
			}
			// pk值>=121
			if( pCharacter->GetGoodness() >= 121)
			{
				m_PkOver.push_back(pCharacter);
				continue;
			}
			// 一般玩家(本国)
			{
				m_General.push_back(pCharacter);
				continue;
			}
		}
	}
	// Draw list as order
	/////////////////////////////////////////////////////////////////
	// Draw Rectangle 
	// Adjust character pos
#define MiniMap_ADJUST_POS(pCharacter)\
	stPointI ptPos = pCharacter->GetPos();\
	ptPos.x -= rcMapPixel.left;\
	ptPos.y -= rcMapPixel.top;\
	ptPos.x/=minMapScale.x;\
	ptPos.y/=minMapScale.y;\
	ptPos.x += ptMiniMapPixelOffset.x;\
	ptPos.y += ptMiniMapPixelOffset.y;

	// Draw list as order
#define MiniMap_DRAW_LIST(List,clr)\
	for( IterList iter= List.begin() ; iter != List.end() ; iter++)\
	{\
	CCharacter * pCharacter = *iter;\
	MiniMap_ADJUST_POS(pCharacter);\
	stRectI rect(ptPos.x-2,ptPos.y-2,ptPos.x + 2,ptPos.y + 2);\
	if(m_pNpcIcon)\
	m_pNpcIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),clr,Blend_Null);\
	}	

	if(bShowAll)
	{
		MiniMap_DRAW_LIST(m_General,COLOR_ARGB(255,0,255,0));
		static DWORD dwLastTime = 0;
		if( (xtimeGetTime() - dwLastTime) > 1000)
		{
			dwLastTime = xtimeGetTime();
			MiniMap_DRAW_LIST(m_PkOver,COLOR_ARGB(255,255,0,0));
		}
		MiniMap_DRAW_LIST(m_Tong,COLOR_ARGB(255,93,169,212));
		MiniMap_DRAW_LIST(m_Friend,COLOR_ARGB(255,255,202,7));
		MiniMap_DRAW_LIST(m_OtherNation,COLOR_ARGB(255,0,0,0));
		MiniMap_DRAW_LIST(m_AllyNation,COLOR_ARGB(255,255,128,192));
		MiniMap_DRAW_LIST(m_UnKnownPerson,COLOR_ARGB(255,0,0,0));		
		MiniMap_DRAW_LIST(m_Family,COLOR_ARGB(255,135,122,138));
	}

	/////////////////////////////////////////////////////////////
	// Draw Icon
#define MiniMap_DRAW_ICON(pIcon,pCharacter)\
	if(pIcon)\
	{\
	stRectI rect(ptPos.x-mw/2,ptPos.y-mh/2,ptPos.x + mw,ptPos.y + mh);\
	stRectI rc = rect;\
	rc.OffsetRect(MinimapLoca);\
	pIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),-1,Blend_Null);\
	if(rc.PtInRect(ptCursor))\
	{\
	GetNameTip()->AddText(pCharacter->GetName());\
	}\
	}

	/////////////////////////////////////////////////////////////////
	//Draw Icon
	for(IterList iter = m_listTeam.begin() ; iter!= m_listTeam.end() ;iter++)
	{
		CCharacter * pCharacter = *iter;
		MiniMap_ADJUST_POS(pCharacter);
		MiniMap_DRAW_ICON(m_pTeamIcon,pCharacter);
	}

	for(IterList iter = m_Couple.begin() ; iter!= m_Couple.end() ;iter++)
	{
		CCharacter * pCharacter = *iter;
		MiniMap_ADJUST_POS(pCharacter);
		MiniMap_DRAW_ICON(m_pCoupleCharacterIcon,pCharacter);
	}

	if( pMainCharacter != NULL)
	{
		MiniMap_ADJUST_POS(pMainCharacter);
		MiniMap_DRAW_ICON(m_pMainRoleIcon,pMainCharacter);
	}
        /////////////////////////////////////////////////////////////////

	if(!GetD3DDevice()->IsDoubleTexture())
	{
		if(InitBackAlpha())
		{
			m_pBackAlpha->Render(0,0,0xff000000,Blend_MulAlpha);
		}
	}
	else
	{
	}

	GetNameTip()->Render(MiniMapCenterPt);
#endif

	GetD3DDevice()->EndScene();
	bool bResult = m_pRenderBmp->UpdateRenderTarget();

	m_pRenderBmp->EndRenderTarget();

	return bResult;

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
bool CGuiMiniMap::InitBackAlpha()
{
	FUNCTION_BEGIN;

	if(!m_pBackAlpha->IsCreated())
	{
		m_pBackAlpha->Init();
		Stream* pStream = OpenPackFileForRead("data\\datas.pak","datas\\image\\minimapback.tga");
		if(pStream)
		{
			m_pBackAlpha->LoadFromTgaStream(pStream,&m_rectAlphaUV);
			ClosePackFileForRead(pStream);
		}
	}
	return m_pBackAlpha->IsCreated();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param fElapsedTime : 描述
* \return 返回值的描述
*/
HRESULT CGuiMiniMap::OnRender( float fElapsedTime )
{
	FUNCTION_BEGIN;

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);

	if ( m_BtnShow->IsVisible() )
		return S_OK;

	if(m_bUpdateSuccess)
	{
		if( InitBackAlpha() && m_pRenderBmp->IsCreated())
		{
			stRectI rect(m_x + m_rectAlphaUV.left,m_y + m_rectAlphaUV.top,m_x  + m_rectAlphaUV.right,m_y + m_rectAlphaUV.bottom);
			GetD3DDevice()->RenderBlendTexture(m_pRenderBmp,m_pBackAlpha,&stRectF(rect),&stRectF(m_rectAlphaUV),&stRectF(m_rectAlphaUV),COLOR_ARGB(180,255,255,255));
		}
		//m_pRenderBmp->Render(m_x,m_y);
	}
	
	if( m_pExitIcon && m_ExitInfo.size() > 0 )
	{
		DWORD clr = COLOR_ARGB(255,255,255,255);

		for(int i =  0 ;i < m_ExitInfo.size(); i++)
		{
			szExitFlag & exit = m_ExitInfo[i];

			float OffsetAngle = angle2radian(180); // 图片要转180度
			OffsetAngle = standAngle(OffsetAngle - exit.theta);

			stPointI ptPoint(exit.x,exit.y);
			ptPoint.x += (m_rcExitClip.right - m_rcExitClip.left) /2;
			ptPoint.y += (m_rcExitClip.bottom - m_rcExitClip.top) /2;

			//GetDevice()->SetRotation(ptPoint,OffsetAngle);

			//m_pExitIcon->RenderAni(exit.x,exit.y,timeGetTime(),clr,Blend_Null);

			//GetDevice()->IdentityTransform();
		}
	}

	RenderTip();

	if(m_TextSprite.IsAnimation())
	{
		stPointI ptCursor = Engine_GetCursor()->GetPosition(); 
		if( m_TextSprite.rcRenderLeft.PtInRect(ptCursor))
		{
			m_TextSprite.m_MouseMode = CMiniMapTextEffect::MouseOnLeft;
		}
		else if(m_TextSprite.rcRenderRight.PtInRect(ptCursor))
		{
			m_TextSprite.m_MouseMode = CMiniMapTextEffect::MouseOnRight;
		}
		else
		{
			m_TextSprite.m_MouseMode = CMiniMapTextEffect::MouseOnNull;
		}
	}

	/*if(m_bUpdateSuccess)
	{
	stPointI ptScreen(GetClientX(),GetClientY());
	stRectI rcClip( (256 - GetClientWidth())/2, (256 - GetClientHeight())/2,
	(256 - GetClientWidth())/2 + GetClientWidth(), (256 - GetClientHeight())/2 + GetClientHeight() );

	if(rcClip.left < 0)
	{
	ptScreen.x -= rcClip.left;
	rcClip.left = 0;
	}

	if(rcClip.top < 0)
	{
	ptScreen.y -= rcClip.top;
	rcClip.top = 0;
	}

	if(rcClip.right > 256) rcClip.right = 256;
	if(rcClip.bottom > 256) rcClip.bottom = 256;		

	m_pRenderBmp->Render(ptScreen.x,ptScreen.y,&rcClip, NULL, 0xffffffff );		

	}*/

	return S_OK;

	FUNCTION_END;
}

void CGuiMiniMap::SetPing(const PING_STATUS & ping )
{

	FUNCTION_BEGIN;
	assert(ping >= PING_EQUO_SMALL && ping <= PING_EQUO_BIG);
	m_Ping = ping ;
	GetImage(106)->SetColor(c_PingDesc[ping].clr);

	FUNCTION_END;
}

void CGuiMiniMap::RenderTip()
{
	FUNCTION_BEGIN;

	if( GetGuiManager()->GetMouseOverDlg() != this)
		return;

	stPointI ptCursor = Engine_GetCursor()->GetPosition(); 
	POINT pt;
	pt.x = ptCursor.x - m_x;
	pt.y = ptCursor.y - m_y;
	if( GetImage(106)->ContainsPoint(pt) )
	{
		CToolTips * pToolTips = GetImage(106)->GetToolTips();
		pToolTips->Clear();
		pToolTips->SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
		pToolTips->SetAlignType( GUI_ALIGN_LEFT );
		pToolTips->SetBorderColor( 0 );
		pToolTips->SetLineGaps( 3 );
		pToolTips->SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
		pToolTips->AddText( c_PingDesc[m_Ping].szTip);
		pToolTips->Resize();
		ptCursor.x -= pToolTips->GetWidth();
		GetGuiManager()->SetToolTips(pToolTips,ptCursor);
	}

	for(int i =0 ;i < m_ExitInfo.size() ;i ++)
	{
		szExitFlag & flag = m_ExitInfo[i];
		stRectI rcClip = m_rcExitClip;
		rcClip.OffsetRect(flag.x,flag.y);

		if( rcClip.PtInRect(ptCursor))
		{
			if( m_pExitToolTips == NULL )
				m_pExitToolTips = new CToolTips;
			if( m_pExitToolTips)
			{
				m_pExitToolTips->Clear();
				m_pExitToolTips->SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
				m_pExitToolTips->SetAlignType( GUI_ALIGN_LEFT );
				m_pExitToolTips->SetBorderColor( 0 );
				m_pExitToolTips->SetLineGaps( 3 );
				m_pExitToolTips->SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
				m_pExitToolTips->AddText(flag.szName);
				m_pExitToolTips->Resize();
				ptCursor.x -= m_pExitToolTips->GetWidth();
				GetGuiManager()->SetToolTips(m_pExitToolTips,ptCursor);
			}
		}
	}	

        CGuiButton * pGMButton = GetButton(107);
	if (pGMButton  && pGMButton->ContainsPoint(pt))
	{
		CToolTips * pToolTips = pGMButton->GetToolTips();
		pToolTips->Clear();
		pToolTips->SetBkColor( D3DCOLOR_ARGB(128,0,0,0) );
		pToolTips->SetAlignType( GUI_ALIGN_LEFT );
		pToolTips->SetBorderColor( 0 );
		pToolTips->SetLineGaps( 3 );
		pToolTips->SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
		pToolTips->AddText( "GM留言");
		pToolTips->Resize();
		ptCursor.x -= pToolTips->GetWidth();
		GetGuiManager()->SetToolTips(pToolTips,ptCursor);  
	}

	FUNCTION_END;
}
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param fElapsedTime : 描述
* \return 返回值的描述
*/
void CGuiMiniMap::RenderBackground(float fElapsedTime)
{
	FUNCTION_BEGIN;

	CGuiDialog::RenderBackground(fElapsedTime);

	stPointI ptGridPos = GetScene()->GetMainCharacter()->GetGridPos();
	const char * pszInfo = avar("%s [%d,%d]",GetScene()->GetMapName(),ptGridPos.x,ptGridPos.y); 
       
	m_TextSprite.SetText(pszInfo);
	m_TextSprite.BeginSprite(true);
	m_TextSprite.UpdateSprite(fElapsedTime);
	m_TextSprite.RenderSprite();
	

	FUNCTION_END;
}

void CGuiMiniMap::OnResetScreenSize()
{
	FUNCTION_BEGIN;
        
	CGuiDialog::OnResetScreenSize();

	InitTextEffect();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param hWnd : 描述
* \param uMsg : 描述
* \param wParam : 描述
* \param lParam : 描述
* \return 返回值的描述
*/
bool CGuiMiniMap::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	FUNCTION_BEGIN;

	switch(uMsg)
	{
	case WM_KEYDOWN:
#ifdef _DEBUG
		switch(wParam) 
		{
		case '1':
			SetPing(PING_EQUO_BIG);
			break;
		}
#endif 
		break;
	}
	return CGuiDialog::MsgProc(hWnd,uMsg,wParam,lParam);

	FUNCTION_END;
}

BOOL CGuiMiniMap::IsInRect(POINT ptScreen)
{
	return (ptScreen.x >= m_x && ptScreen.x < m_x + m_width && ptScreen.y >= m_y && ptScreen.y < m_y + m_height + 20);
}

void CGuiMiniMap::ResetMapNpc(tListMapNpc& listMapNpc)
{
	FUNCTION_BEGIN;


	m_vecIntersetNpc.clear();

	for( size_t i=0;i<listMapNpc.size();++i )
	{	
		stMapFunctionNpc & npc = listMapNpc[i];
		NpcBase_t * pData = GetNpcBase( npc.dwID );
		if (pData)
		{
			if( pData->dwType == NpcTypeItem_Trade 	|| pData->dwType ==  NpcTypeItem_Task  ||   pData->dwType == NpcTypeItem_MobileTrade 
				|| pData->dwType == NpcTypeItem_MailMan	|| pData->dwType == NpcTypeItem_Auction)
			{
				stInterestNpc InterestNpc;
				memcpy(&InterestNpc,&npc,sizeof(stMapFunctionNpc));
				InterestNpc.dwType = pData->dwType;
				m_vecIntersetNpc.push_back(InterestNpc);
			}
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
void CGuiLageMap::OnClose()
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guiLageMap = NULL;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiLageMap::CGuiLageMap()
: m_pFuncNpcIcon ( NULL )
, m_pVecInterestNpc(NULL)
, m_pCoupleCharacterIcon(NULL)
, m_pVecCoupleCharacter(NULL)
{
	FUNCTION_BEGIN;

	m_bSmDialog = true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiLageMap::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();


	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiLageMap::OnResetScreenSize()
{
	FUNCTION_BEGIN;

	InitRect();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiLageMap::InitRect()
{
	FUNCTION_BEGIN;

	stPointI ltOffset(14,14),rbOffset(14,14);
	stRectI rc(ltOffset.x,ltOffset.y,GetDevice()->GetWidth()-rbOffset.x,GetDevice()->GetHeight()-rbOffset.y);
	int x,y,width,height;
	//if(m_pBmp->GetWidth() > rc.Width() || m_pBmp->GetHeight() > rc.Height())
	{
		//if(m_pBmp->GetWidth()/m_pBmp->GetHeight() > rc.Width()/rc.Height())
		if(m_pBmp->GetWidth() * rc.Height() > rc.Width() * m_pBmp->GetHeight())
		{
			// 图片比区域长
			x = rc.left;
			width = rc.Width();
			height = width * m_pBmp->GetHeight()/m_pBmp->GetWidth();
			y = (rc.Height() - height)/2 + rc.top;
		}
		else
		{
			y = rc.top;
			height = rc.Height();
			width = height * m_pBmp->GetWidth()/m_pBmp->GetHeight();
			x = (rc.Width()-width)/2 + rc.left;
		}
		m_scale.x = (float)width/(float)m_pBmp->GetWidth();
		m_scale.y = m_scale.x;
	}
	/*else
	{
	x = (rc.Width() - m_pBmp->GetWidth())/2 + rc.left;
	y = (rc.Height() - m_pBmp->GetHeight())/2 + rc.top;
	width = m_pBmp->GetWidth();
	height = m_pBmp->GetHeight();
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	}*/

	m_bmpRect.left = x;
	m_bmpRect.top = y;
	m_bmpRect.right = x + width;
	m_bmpRect.bottom = y + height;
	m_x = x - ltOffset.x;
	m_y = y - ltOffset.y;
	m_width = width + ltOffset.x + rbOffset.x;
	m_height = height + ltOffset.y + rbOffset.y;
	UpdateLocation();
	GetZoneInfo();

	RequestTeamerPos();

	stResourceLocation rl2(GetGuiGraphicPackName(),20,4);
	m_pMainRoleIcon = GetDevice()->FindBitmaps(&rl2);
	rl2.frame = 3;
	m_pTeamIcon     = GetDevice()->FindBitmaps(&rl2);

	rl2.frame = 5;
	m_pFuncNpcIcon = GetDevice()->FindBitmaps(&rl2);

	rl2.frame = 6;
	m_pCoupleCharacterIcon = GetDevice()->FindBitmaps(&rl2);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiLageMap::GetZoneInfo()
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}


#define MiniMap_AddLineSymbolToTip(pszText)				  \
	if(pszText)						  \
	{							  \
	_snprintf(szNameTip,sizeof(szNameTip),"%s\n",pszText);  \
	szNameTip[sizeof(szNameTip)-1]=0;			  \
	GetNameTip()->AddText(szNameTip,true);                    \
	}	
#ifdef _DEBUG
#ifndef __DEBUG__RECTANGEL__
#define __DEBUG__RECTANGEL__
#endif
#else
#ifdef __DEBUG__RECTANGEL__
#undef __DEBUG__RECTANGEL__
#endif 
#endif
#ifdef __DEBUG__RECTANGEL__
#define MiniMap_Debug_Rectangle_(Rect1,Rect2)\
	DWORD dwYellow = COLOR_ARGB(255,143,241,84);\
	DWORD dwBlue   = COLOR_ARGB(255,154,108,217);\
	GetDevice()->DrawRectangle(&Rect1,dwYellow);\
	GetDevice()->DrawRectangle(&Rect2,dwBlue);  
#else
#define MiniMap_Debug_Rectangle_(Rect1,Rect2) __noop
#endif  

/**
* \brief 如果有队员，且大地图打开状态，定时请求队员位置
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGuiLageMap::RequestTeamerPos()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiTeam && GetGameGuiManager()->m_guiTeam->IsTeam())               
	{
		GetGameGuiManager()->m_guiTeam->RequestTeamPosition();
	}	

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param fElapsedTime : 描述
* \return 返回值的描述
*/
void CGuiLageMap::RenderBackground(float fElapsedTime)
{
	FUNCTION_BEGIN;

	CGuiDialog::RenderBackground(fElapsedTime);
	m_pBmp->Render(m_bmpRect.left,m_bmpRect.top,NULL,&m_scale);

	CGameScene* pScene = GetScene();
	int width = pScene->GetWidth();
	int height = pScene->GetHeight();
	int dstW = m_bmpRect.Width();
	int dstH = m_bmpRect.Height();

	//绘制地图区域名信息
	int iFont = GetDevice()->SetFont(1);
	std::map<DWORD,CGameScene::stZoneInfo> & zoneList = pScene->GetZoneInfoList();
	for(std::map<DWORD,CGameScene::stZoneInfo>::iterator it = zoneList.begin();
		it != zoneList.end(); ++it)
	{
		stRectI rect;
		CGameScene::stZoneInfo & zoneInfo = it->second;
		rect.left = zoneInfo.rect.left * dstW / width;
		rect.right = zoneInfo.rect.right * dstW / width;
		rect.top = zoneInfo.rect.top * dstH / height;
		rect.bottom = zoneInfo.rect.bottom * dstH / height;
		rect.OffsetRect(stPointI(m_bmpRect.left,m_bmpRect.top));
		rect.left -= 25;
		rect.right += 25;
		GetDevice()->DrawString(zoneInfo.szName,rect,COLOR_ARGB(180,255,0,0),DT_CENTER | DT_VCENTER,FontEffect_Border);
	}
	GetDevice()->SetFont(iFont);

	GetNameTip()->Close();
	stPointI mapLoca = GetLocation();
	stPointI ptCursor = Engine_GetCursor()->GetPosition();

	int mw = (m_pTeamIcon)?(m_pTeamIcon->GetBitmap(0)->GetWidth()): 20;
	int mh = (m_pTeamIcon)?(m_pTeamIcon->GetBitmap(0)->GetHeight()): 20;


	//////////////////////////////////////////////////////////////////////////////////
	// 当前光标36*36像素范围的npc显示图标，名字显示在一起 

	int _mw = (m_pTeamIcon)?(m_pTeamIcon->GetBitmap(0)->GetClipWidth()): 15;
	int _mh = (m_pTeamIcon)?(m_pTeamIcon->GetBitmap(0)->GetClipHeight()): 15;

	stRectI  _rcCheck;
	_rcCheck.left = ptCursor.x - _mw;
	_rcCheck.top = ptCursor.y - _mh;
	_rcCheck.right = ptCursor.x +  _mw ;
	_rcCheck.bottom = ptCursor.y + _mh ;
	char szNameTip[MAX_NAMESIZE+4];

	//绘制队员图标
	if(GetGameGuiManager()->m_guiTeam && m_pTeamIcon) 
	{
		CMainCharacter* pMainCharacter = GetScene()->GetMainCharacter();
		CGuiTeam::tListTeamMemvber & memberList = GetGameGuiManager()->m_guiTeam->m_aTeamMember;
		for(CGuiTeam::tListTeamMemvber::iterator it = memberList.begin(); it != memberList.end(); ++it)
		{
			//if( strcmp((*it).data.pstrName,pMainCharacter->GetName()) && (*it).x != -1 
			if(   (*it).data.dwTempID != pMainCharacter->GetID() 
 				&& 0 == strcmp((*it).szMapName,GetScene()->m_szMapName) )
			{
				stPointI ptLeftTop((*it).x* dstW/width  + m_bmpRect.left,(*it).y*  dstH/height + m_bmpRect.top);
				stRectI rect(ptLeftTop.x - mw/2,ptLeftTop.y - mh/2,ptLeftTop.x + mw/2,ptLeftTop.y + mh/2);
				stRectI rc = rect;

				m_pTeamIcon->RenderAni(rect.left,rect.top ,xtimeGetTime(),-1,Blend_Null);

				MiniMap_Debug_Rectangle_(rect,_rcCheck);
				stRectI _rect = rect & _rcCheck;

				if( _rect == rect )
				{
					MiniMap_AddLineSymbolToTip((*it).data.pstrName);
				}
			}
		}
	}
	
	// TODO:绘制主角的图标
	if(m_pMainRoleIcon)
	{
		stPointI pt = GetScene()->GetMainCharacter()->GetGridPos();
		pt.x = pt.x * dstW/width + m_bmpRect.left;
		pt.y = pt.y * dstH/height + m_bmpRect.top;

		stRectI rect(pt.x ,pt.y,pt.x + mw,pt.y + mh);

		m_pMainRoleIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),-1,Blend_Null);

		MiniMap_Debug_Rectangle_(rect,_rcCheck);              
		
		stRectI _rect = rect & _rcCheck;

		if( _rect == rect )
		{
			MiniMap_AddLineSymbolToTip(GetScene()->GetMainCharacter()->GetName());
		}
	}
	//pBitmap->Render(GetScene()->GetMainCharacter()->GetPos().x*dstW / width ,GetScene()->GetMainCharacter()->GetPos().y* dstH / height);

	// 显示买卖和功能类npc
	if( m_pFuncNpcIcon && m_pVecInterestNpc)
	{
		for( int i = 0 ; i < m_pVecInterestNpc->size() ; i++ )
		{
			const stInterestNpc & npc =  (*m_pVecInterestNpc)[i];
			POINT   pt = npc.ptPos;
			pt.x = pt.x * dstW/width;
			pt.y = pt.y * dstH/height;

			stRectI rect(pt.x ,pt.y,pt.x + mw,pt.y + mh);

			rect.OffsetRect(stPointI(m_bmpRect.left,m_bmpRect.top)); 

			m_pFuncNpcIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),-1,Blend_Null);
		
			MiniMap_Debug_Rectangle_(rect,_rcCheck);

			stRectI _rect = rect & _rcCheck;

			if( _rect == rect )
			{	
				MiniMap_AddLineSymbolToTip(npc.szName);
			}
		}
	}

	//绘制夫妻图标
	if(m_pCoupleCharacterIcon) 
	{
		for(IterList it = m_pVecCoupleCharacter->begin(); it != m_pVecCoupleCharacter->end(); ++it)
		{
			CCharacter * pCharacter = *it;
			stPointI pt =  pCharacter->GetPos();
			pt.x = pt.x * dstW/width + m_bmpRect.left;
			pt.y = pt.y * dstH/height + m_bmpRect.top;
			stRectI rect(pt.x ,pt.y,pt.x + mw,pt.y + mh);

			m_pCoupleCharacterIcon->RenderAni(rect.left,rect.top,xtimeGetTime(),-1,Blend_Null);

			MiniMap_Debug_Rectangle_(rect,_rcCheck);              

			stRectI _rect = rect & _rcCheck;

			if( _rect == rect )
			{
				MiniMap_AddLineSymbolToTip(GetScene()->GetMainCharacter()->GetName());
			}
		}
	}

	char mouseXYStr[24] = {"0,0"};
	POINT msPt = Engine_GetCursor()->GetPosition();
	if (m_bmpRect.PtInRect(msPt))
	{		
		msPt.x = (msPt.x - m_bmpRect.left)*(width*64/dstW);
		msPt.y = (msPt.y - m_bmpRect.top)*(height*32/dstH);
		sprintf(mouseXYStr,"(%d,%d)",msPt.x/64,msPt.y/32);
		GetNameTip()->m_strTextVec.push_back(mouseXYStr);
	}

	GetNameTip()->CalculateCutRect();
	GetNameTip()->Render(ptCursor-stPointI(0,40));
   

	FUNCTION_END;
}

bool CGuiLageMap::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CGuiDialog::MsgProc( hWnd, uMsg, wParam, lParam ) )
		return true;

	switch( uMsg )
	{
    case WM_LBUTTONUP:
		{
			//Get offset
			stPointI ptDest = Engine_GetCursor()->GetPosition();
			ptDest.x -= GetClientX();
			ptDest.y -= GetClientY();

			CGameScene* pScene = GetScene();
			int width = pScene->GetWidth();
			int height = pScene->GetHeight();
			int dstW = m_bmpRect.Width();
			int dstH = m_bmpRect.Height();

			stPointI ptGrid;
			ptGrid.x = (float)ptDest.x / dstW * width;
			ptGrid.y = (float)ptDest.y / dstH * height;
			//Set run event

			GetScene()->GetMainCharacter()->Goto( ptGrid );

			return true;
		}
    	break;
    }

	return false;
}

