#include "public.h"
#include ".\character.h"
#include "./GameGuiManager.h"
#include "./GuiItem.h"
#include "./GuiSkill.h"
#include "./GuiMain.h"
#include "./GuiFloatAccessKey.h"
#include "./Character.h"
#include "./GameScene.h"
#include "./GameCursor.h"
#include "./GuiUserInfo.h"
#include "./GuiTeam.h"
#include "./GuiWorldConn.h"
#include "./Game.h"
#include "./RoleItem.h"
#include "./CharacterProperty.h"
#include "./GuiShopDialog.h"
#include "./GuiOthersShopDialog.h"
#include "./Country.h"
#include "./GameAppation.h"

#define HAND_ATTACK_SOUND 20008
#define HAND_HIT_SOUND	 10008
#define BOW_EMPTY_ATTACK_SOUND 20008
#define BOW_EMPTY_HIT_SOUND	10008

#define KEEP_SHOW_TIME  8000
#define START_HIDE_TIME 7000

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */

bool ShowCountryStar(DWORD star,char* pStr,DWORD& color)
{
	FUNCTION_BEGIN;

	static char szText[64];
	
	szText[0]=0;
	
	if (pStr==NULL) return false;
	if (star==0) return false;
	if (star>36) star = 36;
	star--;
	int nRate = star / 9;
	int nMod = star % 9;


	for (int i=0;i<=nMod;i++)
		strncat(szText,"★",sizeof(szText));
	
	switch(nRate) {
	case 0:
		color = COLOR_ARGB(255,255,255,255);
		break;
	case 1:
		color = COLOR_ARGB(255,0,86,234);
		break;
	case 2:
		color = COLOR_ARGB(255,255,222,3);
		break;
	case 3:
		color = COLOR_ARGB(255,0,255,137);
		break;
	}
	
	
	sprintf(pStr,"%s",szText);
	return true;

	FUNCTION_END;
}

stBitmapShow::stBitmapShow()
{
	rl.SetFileName("data\\icons.gl");
	rl.group = 7;
	pBitmps = NULL;
	alpha = 0;
	rcClip.SetRect(0,0,0,0);
}

inline bool stBitmapShow::IsNeedShow()
{
	if( pBitmps==NULL || alpha == 0 ) 
		return false;
	else 
		return true;
}

void stBitmapShow::StartShow()
{
	dwStartTime = xtimeGetTime();
	alpha = 255;
}

void stBitmapShow::SetFrame(int nFrame)
{
	if( nFrame == rl.frame) return; // must be in the same group,else will make a error 
	rl.frame = nFrame;
	pBitmps = GetDevice()->FindBitmaps(&rl);
	if( pBitmps)
	{
		IBitmap * pBitmap =  pBitmps->GetBitmap(pBitmps->GetFrameCount()/2);
		if( pBitmap)
		{
			rcClip = pBitmap->GetClipRect();
		}
	}	
}

void stBitmapShow::Run()
{
	if(IsNeedShow())
	{
		DWORD curtime = xtimeGetTime();
		DWORD diff = curtime - dwStartTime;
		if( diff > KEEP_SHOW_TIME )
		{
			//pBitmps  = NULL;
			alpha = 0 ;
			return ;
		}
		int alpha = 255;
		if(diff > START_HIDE_TIME) 
			alpha = (KEEP_SHOW_TIME - curtime ) *255 /( KEEP_SHOW_TIME - START_HIDE_TIME);
	}
}

void stBitmapShow::Render(const POINT & pt)
{
	if(IsNeedShow())
	{	
		stPointI pt2 = pt;
		pt2.x -= (rcClip.left + rcClip.Width()/2);
		pt2.y -= (rcClip.top+rcClip.Height());
		pBitmps->RenderAni(pt2.x,pt2.y,xtimeGetTime(),NULL,NULL,COLOR_ARGB(alpha,255,255,255),Blend_Null);  
	}
}


DWORD GetNameColor(DWORD dwGoodNess)
{
	FUNCTION_BEGIN;

	DWORD dwColor = -1;

	if (dwGoodNess & GOODNESS_ATT)
	{
		dwColor=COLOR_ARGB(255,63,43,0);
	}
	else
	{
		DWORD dw=0x0000FFFF;
		short goodness=(short)(dwGoodNess & dw);
		switch(goodness) {
		case GOODNESS_2_1:
		case GOODNESS_2_2:
			dwColor=COLOR_ARGB(255,255,255,255);
			break;
		default:
			{
				if (goodness<(short)(GOODNESS_1 & 0x0000FFFF))
					dwColor=COLOR_ARGB(255,3,106,288);
				else if (goodness<(short)(GOODNESS_2_1 & dw))
					dwColor=COLOR_ARGB(255,36,203,173);
				else if (goodness<=(short)(GOODNESS_3 & dw))
					dwColor=COLOR_ARGB(255,255,155,21);
				else if (goodness<=(short)(GOODNESS_4 & dw))
					dwColor=COLOR_ARGB(255,225,97,21);
				else if (goodness<=(short)(GOODNESS_5 & dw))
					dwColor=COLOR_ARGB(255,200,33,58);
				else 
					dwColor=COLOR_ARGB(255,225,18,24);
			}
			break;
		}
	}

	return dwColor;

	FUNCTION_END;
}


CCharacter::CCharacter(void)
: m_tiletype(-1)
, m_ptPrevGrid(-1,-1)
, m_dwStar(0)
, m_bShowWarFlag(FALSE)
{
	FUNCTION_BEGIN;

	m_dwSceneObjectType |= OBJECT_CHARACTER;
	memset(&m_data,0,sizeof(m_data));
	m_data.sculpt.dwBodyID = -1;
	memset(m_countryName,0,MAX_NAMESIZE);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CCharacter::~CCharacter(void)
{
	FUNCTION_BEGIN;

	Release();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CCharacter::Release()
{
	FUNCTION_BEGIN;

	//If this our interest 
	if ( GetScene()->m_pInterestNpc && ( GetScene()->m_pInterestNpc->IsCharacter() ) )
	{
		if ( GetScene()->m_pInterestNpc == this )
		{
			GetScene()->UnSelectInterestNpc();
			if ( GetGameGuiManager()->m_guiCharacterProperty )
				GetGameGuiManager()->m_guiCharacterProperty->SetVisible( false );
		}
	}
	
	if( GetScene()->m_pMouseOverObject == this){
		GetScene()->m_pMouseOverObject = NULL;
	}

	//Delete this from Sept && Tong Name request list
	//g_UserLocalInfo.DeleteCharacter( this );

	memset(&m_data,0,sizeof(m_data));
	CNpc::Release();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pCmd : 描述
 * \return 返回值的描述
 */
bool CCharacter::SetAttrib(::t_MapUserData* pCmd)
{
	FUNCTION_BEGIN;

#ifdef _WYC_OPT_SER_
	//优化:服务器传递用户信息,帮派等只传id号,客户端做一个对应关系
	//为了避免程序结构大范围改变,采取收到消息后,先拷贝到一个临时结构,然后再这个结构里面赋值
	wyc::t_MapUserData temp;
	bool isFindid2String_union = false;
	bool isFindid2String_sept = false;
	memcpy(&temp,pCmd,sizeof(temp));
	
	strcpy(temp.unionName,"");
	strcpy(temp.septName,"");

	if ( pCmd->dwUnionID != 0 )
	{
		g_UserLocalInfo.m_pIter = g_UserLocalInfo.m_mapUnionName.find(pCmd->dwUnionID);
		if (g_UserLocalInfo.m_pIter == g_UserLocalInfo.m_mapUnionName.end())
		{
			//从request id map 里面确定是否请求过
			g_UserLocalInfo.m_pRequestIter = g_UserLocalInfo.curRequesetUnionId.find(pCmd->dwUnionID);
			if (g_UserLocalInfo.m_pRequestIter == g_UserLocalInfo.curRequesetUnionId.end())
			{
				//向服务器请求帮会名称,并加入请求id队,防止多次向服务器请求
				GetScene()->AddRequestUnionSeptNameCmd( CGameScene::RequestCmd_UnionName, temp.dwUnionID );
				/*stReqUnionNameCmd cmd;
				cmd.dwUnionID = temp.dwUnionID;
				SEND_USER_CMD( cmd );*/

				vRequestCharacter	vCharacter;
				vCharacter.clear();
				vCharacter.push_back( GetID() );
				g_UserLocalInfo.curRequesetUnionId.insert( D_Character_Pair ( temp.dwUnionID , vCharacter) );
				TRACE("--wyc__向服务器请求帮会名称__wyc--\n");
			}
			else
			{
				vRequestCharacter& vCharacter = (*(g_UserLocalInfo.m_pRequestIter)).second;
				vRequestCharacter::iterator iter;
				for( iter=vCharacter.begin();iter!=vCharacter.end();++iter )
				{
					if ( (*iter) == GetID() )
						break;
				}
				if ( iter == vCharacter.end() )
					vCharacter.push_back( GetID() );
			}
		}
		else
		{
			strcpy(temp.unionName,g_UserLocalInfo.m_mapUnionName.find(pCmd->dwUnionID)->second.c_str());
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	if ( pCmd->dwSeptID != 0 )
	{
		g_UserLocalInfo.m_pIter = g_UserLocalInfo.m_mapSpetName.find(pCmd->dwSeptID);
		if (g_UserLocalInfo.m_pIter == g_UserLocalInfo.m_mapSpetName.end())
		{
			//从request id map 里面确定是否请求过
			g_UserLocalInfo.m_pRequestIter = g_UserLocalInfo.curRequesetSpetId.find(pCmd->dwSeptID);
			if (g_UserLocalInfo.m_pRequestIter == g_UserLocalInfo.curRequesetSpetId.end())
			{
				//向服务器请求家族名称,并加入请求id队,防止多次向服务器请求
				GetScene()->AddRequestUnionSeptNameCmd( CGameScene::RequestCmd_SeptName, temp.dwSeptID );
				/*stReqSeptNameCmd cmd;
				cmd.dwSeptID = temp.dwSeptID;
				SEND_USER_CMD( cmd );*/
				vRequestCharacter	vCharacter;
				vCharacter.clear();
				vCharacter.push_back( GetID() );
				g_UserLocalInfo.curRequesetSpetId.insert( D_Character_Pair ( temp.dwSeptID , vCharacter) );
				TRACE("--wyc__向服务器请求家族名称__wyc--\n");
			}
			else
			{
				vRequestCharacter& vCharacter = (*(g_UserLocalInfo.m_pRequestIter)).second;
				vRequestCharacter::iterator iter;
				for( iter=vCharacter.begin();iter!=vCharacter.end();++iter )
				{
					if ( (*iter) == GetID() )
						break;
				}
				if ( iter == vCharacter.end() )
					vCharacter.push_back( GetID() );
			}
		}
		else
		{
			strcpy(temp.septName,g_UserLocalInfo.m_mapSpetName.find(pCmd->dwSeptID)->second.c_str());
		}
	}
			
	//////////////////////////////////////////////////////////////////////////
	pCmd = &temp;
    //////////////////////////////////////////////////////////////////////////
#endif

	bool bShopStart = bIsShopStart() ;

	bool bNeedRide = isset_state(pCmd->state,USTATE_RIDE) && !IsRide();
	bool bUpdateImage = (0 != memcmp(&m_data.sculpt,&pCmd->sculpt,sizeof(t_MapUserSculpt)) || bNeedRide) && !IsChangeFace() && !IsCombinationState();
	//bool bUpdateState = (memcmp(m_data.state,pCmd->state,sizeof(m_data.state)) != 0);
	bool bUpdateState = (memcmp(m_data.state,pCmd->state,sizeof(m_data.state)) != 0) || 
		pCmd->dwBodyColorCustom != m_data.dwBodyColorCustom ||
		pCmd->dwHairRGB != m_data.dwHairRGB;

	if( false == bIsShopStart() )
	{
		if( GetGameGuiManager()->m_guiOthersShop )
		{
			if ( GetID() == GetGameGuiManager()->m_guiOthersShop->GetOwnerID() )
			{
				GetGameGuiManager()->m_guiOthersShop->Close();
			}
		}
	}

	bool bIsMainRole = IsMainRole();

	if ( bIsMainRole && (m_data.movespeed < pCmd->movespeed) )
	{
		if ( GetScene()->GetMainCharacter() )
			GetScene()->GetMainCharacter()->ClearRushOutState();
	}

	//Wether need update caused by weapon equipment color change
	bool bNeedUpdateItemByEquipColor = (pCmd->dwBodyColorSystem != m_data.dwBodyColorSystem)
									||  (pCmd->dwLeftWeaponColor != m_data.dwLeftWeaponColor)
									||  (pCmd->dwRightWeaponColor != m_data.dwRightWeaponColor)
									||  (pCmd->dwBodyColorCustom != m_data.dwBodyColorCustom);
	
	//We don't use level for main character
	int oldLevel = m_data.level;
	//m_data = *pCmd;
	m_data = temp;

	if ( bIsMainRole )
	{
		m_data.level = oldLevel;
	}

	//Modify exploit value form server
	//Scale 0.01
	m_data.exploit = pCmd->exploit / 100;

	if( bIsMainRole )
	{
		if( GetGameGuiManager()->m_guiMain)
			GetGameGuiManager()->m_guiMain->InitIcon();
	}
	else
	{     
		if(  GetGameGuiManager()->m_guiCharacterProperty && GetGameGuiManager()->m_guiCharacterProperty->IsVisible() && GetScene()->m_pInterestNpc && GetScene()->m_pInterestNpc->GetID() == this->GetID())
		{
			GetGameGuiManager()->m_guiCharacterProperty->InitIcon();
		}
	}

	SetName(pCmd->name);
	m_dwID = pCmd->dwUserTempID;
	m_bDeleted = false;
	AfterSetAttrib(bUpdateImage,bUpdateState);

	//设置摊位的名字
	if( bIsShopStart() )
	{
		char shopname[MAX_PATH];
		memset(shopname,0,MAX_PATH);
		sprintf(shopname,"%s 的摊位",GetName());
		m_ShopNameTip.SetShopName(shopname);
	}
	
	memset(m_countryName,0,MAX_NAMESIZE);

	CCountry::Instance()->GetCountryName(GetCountry(),m_countryName);

	if( bIsMainRole )
	{
		//触发"消除红名"事件
		if(bIsUserUnSetted(USER_SETTING_HELP_CLEARREDNAME))
		{
			DWORD goodness[4]={GOODNESS_4,GOODNESS_5,GOODNESS_6,GOODNESS_7};

			for(int i = 0;i <4;++i) 
			{
				if(pCmd->goodness == goodness[i] )
				{
					ShowHelpTip(USER_SETTING_HELP_CLEARREDNAME);
					break;
				}
			}			
		}
		CAccoutSaveInfo::GetSingleton()->SetUserName(pCmd->name);


		GetGameGuiManager()->InitMainCharacterInfo();
		if ( GetGameGuiManager()->m_guiSkill )
			GetGameGuiManager()->m_guiSkill->InitIcon();
		GetGameGuiManager()->m_guiItem->SetCharName(GetName());

		// Init access key status
		if ( GetGameGuiManager()->m_guiMain )
			GetGameGuiManager()->m_guiMain->RefreshKeyGroupState( GetScene()->GetMainCharacter()->GetPreferAccessGroup() );
		if ( GetGameGuiManager()->m_guiFloatAcessKey )
			GetGameGuiManager()->m_guiFloatAcessKey->RefreshKeyGroupState();


		//Update item property just when equipment color changed
		if ( GetScene()->GetMainCharacter() )
		{
			if ( bNeedUpdateItemByEquipColor )
			{
				DWORD flag = CRoleItem::UPDATE_FLAG_IMAGE;

				GetScene()->GetMainCharacter()->UpdateAllItems( flag );
			}	
			GetScene()->GetMainCharacter()->ResetHorseOwnStatus();
		}
	}

	
	if( FALSE ==  CCountry::Instance()->GetCountryStar(GetCountry(),m_dwStar) ){
	       m_dwStar =  0 ;
	}

	CCountry::stDareCountryInfo & dareinfo = CCountry::Instance()->GetCountryWarInfo();
	m_bShowWarFlag =  dareinfo.IsCountryInWar(GetCountry());
        if( !m_bShowWarFlag )
	{
		if( GetScene()->GetMainCharacter()->GetCountry() == GetCountry() )
		{//与主角同国
			m_bShowWarFlag = dareinfo.IsAnyCountryInWar();
		}
	}
	return true;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwColor : 描述
 * \return 返回值的描述
 */
void CCharacter::SetBodyCustomColor(DWORD dwColor)
{
	m_pCharacter->ChangeColor(Ani_Part_BodyCustomColor,dwColor);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwColor : 描述
 * \return 返回值的描述
 */
void CCharacter::SetBodySystemColor(DWORD dwColor)
{
	m_pCharacter->ChangeColor(Ani_Part_BodySystemColor,dwColor);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param bUpdateImage : 描述
 * \param bUpdateState : 描述
 * \return 返回值的描述
 */
void CCharacter::AfterSetAttrib(bool bUpdateImage,bool bUpdateState)
{
	FUNCTION_BEGIN;

	CNpc::AfterSetAttrib(bUpdateImage,bUpdateState);
	m_pCharacter->SetSex(GetSex()); 
	
	m_pCharacter->ChangeColor(Ani_Part_BodySystemColor,GetBodySystemColor());
	m_pCharacter->ChangeColor(Ani_Part_BodyCustomColor,GetBodyCustomColor());
	m_pCharacter->ChangeColor(Ani_Part_Hair,GetHairColor());
	m_pCharacter->ChangeColor(Ani_Part_LeftWeaponColor,GetLeftWeaponColor());
	m_pCharacter->ChangeColor(Ani_Part_RightWeaponColor,GetRightWeaponColor());

	if(IsMainRole())
	{
		GetGameGuiManager()->m_guiItem->SetCharType(GetType());
		GetGameGuiManager()->m_guiUserInfo->RefreshBodyColor(GetBodyCustomColor());
	}

	m_pCharacter->SetSex(GetSex());

	if ( IsCombinationState() )
		GetInterface()->SetMoveSpeed( (DWORD)m_data.movespeed/2 );
	else
		GetInterface()->SetMoveSpeed( (DWORD)m_data.movespeed );

	if ( IsMainRole() )
		TRACE( "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Server change move speed to =%d$$$$$$$$$$$$$$$$$$$$$$$$$\n", m_data.movespeed ); 

	GetInterface()->SetAttackSpeed(m_data.attackspeed/100.0f);

	GetInterface()->SetMagicSpeed(m_data.attackspeed/100.0f);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool CCharacter::ResetState()
{
	CNpc::ResetState();
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool CCharacter::ResetImage()
{
	FUNCTION_BEGIN;

	int dwPicHairID	=	0;
	int dwPicBodyID	=	0;
	int dwPicWeaponLID=	0;
	int dwPicWeaponRID=	0;

	int dwPicWeaponLColorID = 0;
	int dwPicWeaponRColorID = 0;
	int dwPicBodyColorID = 0;

	//服装
	ObjectBase_t* pBody = GetObjectBase(m_data.sculpt.dwBodyID);
	if(pBody) {
		dwPicBodyID = pBody->dwActionPic;
		if(pBody->dwChannelNum != -1)
			dwPicBodyColorID = pBody->dwChannelNum;
		else
			dwPicBodyColorID = dwPicBodyID;
	}

	//左手武器
	ObjectBase_t* pWeaponL = GetObjectBase(m_data.sculpt.dwLeftHandID);
	if(pWeaponL) {
		dwPicWeaponLID = pWeaponL->dwActionPic;
		if(pWeaponL->dwChannelNum != -1)
			dwPicWeaponLColorID = pWeaponL->dwChannelNum;
		//else
		//	dwPicWeaponLColorID = dwPicWeaponLID;
	}

	//右手武器
	ObjectBase_t* pWeaponR = GetObjectBase(m_data.sculpt.dwRightHandID);
	if(pWeaponR) {
		dwPicWeaponRID = pWeaponR->dwActionPic;
		if(pWeaponR->dwChannelNum != -1)
			dwPicWeaponRColorID = pWeaponR->dwChannelNum;
		//else
		//	dwPicWeaponRColorID = dwPicWeaponRID;
	}

	if (pWeaponR)
		m_pCharacter->SetArrowInRightHand( pWeaponR->dwType == ItemType_Arrow );
	else
		m_pCharacter->SetArrowInRightHand( false );


	//发型
	dwPicHairID = m_data.sculpt.dwHairID;

	if( dwPicBodyID == 0 )
	{
		dwPicBodyID = GetCharTypeInfo(GetType()).actionpic;
	}

	HairStyle_t* p = GetHairStyle(m_data.sculpt.dwHairID);
	if(p)	dwPicHairID =  p->dwActionPicID;

	//dwPicHairID = GetCharTypeInfo(GetType()).hair[GetHairType()];
	static std::vector<int> aID;

	aID.resize(Ani_Part_Num);

#ifdef _HAS_WEAPON_SHADOW
	aID[Ani_Part_LeftWeaponShadow] = dwPicWeaponLID;
	aID[Ani_Part_RightWeaponShadow] = dwPicWeaponRID;
#endif

	aID[Ani_Part_LeftWeapon] = dwPicWeaponLID;
	aID[Ani_Part_RightWeapon] = dwPicWeaponRID;

#ifdef _HAS_WEAPON_COLOR
	aID[Ani_Part_LeftWeaponColor] = dwPicWeaponLColorID;
	aID[Ani_Part_RightWeaponColor] = dwPicWeaponRColorID;
#endif

	aID[Ani_Part_Body] = dwPicBodyID;
	aID[Ani_Part_BodySystemColor] = dwPicBodyColorID;
	aID[Ani_Part_BodyCustomColor] = dwPicBodyID;
	aID[Ani_Part_Hair] = dwPicHairID;

	m_pCharacter->CreateCharacter(0,aID,false);

	if(m_pCharacter->IsWait())
	{
		//m_pCharacter->TurnTo(m_data.);
	}
	OnEquipChanged();
	//m_pCharacter->ChangeColor(Ani_Part_BodySystemColor,GetRandColor24());
	//m_pCharacter->ChangeColor(Ani_Part_BodyCustomColor,GetRandColor24());

	//In case of is ride state
	if(!m_bDied)
	{
		bool bRide = isset_state(GetState(),USTATE_RIDE);
		if( bRide )
			OnRide(bRide);
	}

	return true;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param time_elasped : 描述
 * \return 返回值的描述
 */
void CCharacter::Run(long time_elasped)
{
	FUNCTION_BEGIN;

	CNpc::Run(time_elasped);
	if(m_ptPrevGrid != m_ptGrid)
	{
		OnGridPosChange(m_ptPrevGrid,m_ptGrid);
		m_ptPrevGrid = m_ptGrid;
	}
    m_BitmapShow.Run();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptMapPixelOffset : 描述
 * \param dwColor : 描述
 * \return 返回值的描述
 */
void CCharacter::Draw(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;
	
	CNpc::Draw(ptMapPixelOffset,dwColor);


	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	描述: 		  能否被主角攻击
*/
bool CCharacter::IsCanAttack()
{
	FUNCTION_BEGIN;
	
	if ( this == GetScene()->GetMainCharacter() )
		return false;

	//Decide if can be attack by Main Character according to cur Attack Mode
	switch( GetScene()->GetMainCharacter()->GetPKMode() )
	{
	case PKMODE_NORMAL:		//和平模式:此种模式下对所有目标人物进行攻击不会造成伤害
		return false;
		break;
	case PKMODE_ENTIRE:		//全体模式:此种模式下对所有目标人物进行攻击会造成伤害
		break;
	case PKMODE_TEAM:		//组队模式:此种模式下只对非队友目标人物攻击才能造成伤害
		{
			if ( GetGameGuiManager()->m_guiTeam )
			{
				if ( GetGameGuiManager()->m_guiTeam->FindMember( GetID() ) )		//Is team member	GetName()
					return false;
			}
		}
		break;
	case PKMODE_TONG:		//帮会模式:此种模式下只对非同一帮会目标人物攻击才能造成伤害
		{
			if ( GetGameGuiManager()->m_guiWorldConn )
			{
				if ( GetGameGuiManager()->m_guiWorldConn->FindTongMember( GetName() ) )		// Is tong member
					return false;
			}
		}
		break;
	case PKMODE_SEPT:		//家族模式:此模式下只对非同一家族目标人物攻击才能造成伤害
		{
			if ( GetGameGuiManager()->m_guiWorldConn )
			{
				if ( GetGameGuiManager()->m_guiWorldConn->FindSeptMember( GetName() ) )		//Is sept member
					return false;
			}
		}
		break;
	//case PKMODE_SCHOOL:		//师门模式:此模式下只对非同一师门目标人物攻击才能造成伤害
	//	{
	//		if ( GetGameGuiManager()->m_guiWorldConn )
	//		{
	//			if ( GetGameGuiManager()->m_guiWorldConn->FindTeacherMember( GetName() ) )		//Is teacher member
	//				return false;
	//		}
	//	}
	//	break;
	case PKMODE_COUNTRY:	//国家模式:此种模式下只对非同一国家目标人物攻击才能造成伤害
		{               //  而且国战期间对盟国人也不能攻击
			if ( GetCountry() == GetScene()->GetMainCharacter()->GetCountry() || CCountry::Instance()->IsAllyCountryAndInWar(GetCountry()) )
				return false;
		}
		break;
	//case PKMODE_CHALLENGE:	//挑战模式
	//	{
	//		//TBD
	//	}
	//	break;
	case PKMODE_GOODNESS:
		{
			DWORD dw=0x0000FFFF;
			short goodness=(short)(m_data.goodness & dw);
			switch(goodness) {
			case GOODNESS_2_1:
			case GOODNESS_2_2:
				break;
			default:
				break;
			}

			//TBD
			return true;
		}
		break;
	case PKMODE_ALLY:
		{
			if ( (GetCountry() == GetScene()->GetMainCharacter()->GetCountry())
				|| (CCountry::Instance()->IsAllyCountry(GetCountry())) )
			{
				return false;
			}
		}		
		break;
	}

	return true;

	FUNCTION_END;
}

bool CCharacter::IsMyFriend()
{
	FUNCTION_BEGIN;

	if ( this == GetScene()->GetMainCharacter() )
		return true;

	//Is team member
	if ( GetGameGuiManager()->m_guiTeam && GetGameGuiManager()->m_guiTeam->FindMember( GetID() ) )
		return true;

	//Decide if can be attack by Main Character according to cur Attack Mode
	switch( GetScene()->GetMainCharacter()->GetPKMode() )
	{
	case PKMODE_NORMAL:		//和平模式:此种模式下对所有目标人物进行攻击不会造成伤害
		break;
	case PKMODE_ENTIRE:		//全体模式:此种模式下对所有目标人物进行攻击会造成伤害
		break;
	case PKMODE_TEAM:		//组队模式:此种模式下只对非队友目标人物攻击才能造成伤害
		break;
	case PKMODE_TONG:		//帮会模式:此种模式下只对非同一帮会目标人物攻击才能造成伤害
		{
			if ( GetGameGuiManager()->m_guiWorldConn )
			{
				if ( GetGameGuiManager()->m_guiWorldConn->FindTongMember( GetName() ) )		// Is tong member
					return true;
			}
		}
		break;
	case PKMODE_SEPT:		//家族模式:此模式下只对非同一家族目标人物攻击才能造成伤害
		{
			if ( GetGameGuiManager()->m_guiWorldConn )
			{
				if ( GetGameGuiManager()->m_guiWorldConn->FindSeptMember( GetName() ) )		//Is sept member
					return true;
			}
		}
		break;
		//case PKMODE_SCHOOL:		//师门模式:此模式下只对非同一师门目标人物攻击才能造成伤害
		//	{
		//		if ( GetGameGuiManager()->m_guiWorldConn )
		//		{
		//			if ( GetGameGuiManager()->m_guiWorldConn->FindTeacherMember( GetName() ) )		//Is teacher member
		//				return false;
		//		}
		//	}
		//	break;
	case PKMODE_COUNTRY:	//国家模式:此种模式下只对非同一国家目标人物攻击才能造成伤害
		{               //  而且国战期间对盟国人也不能攻击
			if ( GetCountry() == GetScene()->GetMainCharacter()->GetCountry() || CCountry::Instance()->IsAllyCountryAndInWar(GetCountry()) )
				return true;
		}
		break;
		//case PKMODE_CHALLENGE:	//挑战模式
		//	{
		//		//TBD
		//	}
		//	break;
	case PKMODE_GOODNESS:
		break;
	case PKMODE_ALLY:
		{
			if ( (GetCountry() == GetScene()->GetMainCharacter()->GetCountry())
				|| (CCountry::Instance()->IsAllyCountry(GetCountry())) )
			{
				return true;
			}
		}		
		break;
	}

	return false;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	描述: 		  能否攻击别人
*/
bool CCharacter::CanAttack()
{
	FUNCTION_BEGIN;

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
const char* CCharacter::GetTypeName()
{
	FUNCTION_BEGIN;

	return GetCharTypeInfo(GetType()).name;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
DWORD CCharacter::GetBodySystemColor() 
{
	FUNCTION_BEGIN;

	ObjectBase_t* pBody = GetObjectBase(m_data.sculpt.dwBodyID);
	if(pBody) return pBody->dwColor;
	return -1;
	//return m_data.dwBodyColorSystem | 0xff000000;

	FUNCTION_END;
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
DWORD CCharacter::GetGoodnessAfterAdjust()
{
	FUNCTION_BEGIN;

	if( (m_data.goodness & 0x0000FFFF) == GOODNESS_7 )
	{
		return GOODNESS_7;
	}

	DWORD good = 0x0000FFFF & m_data.goodness;
	if(good < -60)
	{
		good = GOODNESS_0;
	}
	else if(good < 0 && good >= -60) 
	{
		good = GOODNESS_1;
	}
	else if(good == 0)
	{
		good = GOODNESS_2_1;
	}
	else if(good > 0 && good <= 60)
	{
		good = GOODNESS_3;
	}
	else if(good > 60 && good <= 120) 
	{
		good = GOODNESS_4;
	}
	else if(good > 120 && good <= 180) 
	{
		good = GOODNESS_5;
	}
	else if(good > 180 && good < 10000)
	{
		good = GOODNESS_6;
	}

	return good;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
/*
DWORD CCharacter::GetNameColor()
{
	FUNCTION_BEGIN;

	DWORD dwColor = -1;

	if (m_data.goodness & GOODNESS_ATT)
	{
		dwColor=COLOR_ARGB(255,63,43,0);
	}
	else
	{
		DWORD dw=0x0000FFFF;
		short goodness=(short)(m_data.goodness & dw);
		switch(goodness) {
		case GOODNESS_2_1:
		case GOODNESS_2_2:
			dwColor=COLOR_ARGB(255,255,255,255);
			break;
		default:
			{
				if (goodness<(short)(GOODNESS_1 & 0x0000FFFF))
					dwColor=COLOR_ARGB(255,3,106,288);
				else if (goodness<(short)(GOODNESS_2_1 & dw))
					dwColor=COLOR_ARGB(255,36,203,173);
				else if (goodness<(short)(GOODNESS_3 & dw))
					dwColor=COLOR_ARGB(255,255,155,21);
				else if (goodness<(short)(GOODNESS_4 & dw))
					dwColor=COLOR_ARGB(255,225,97,21);
				else if (goodness<(short)(GOODNESS_5 & dw))
					dwColor=COLOR_ARGB(255,200,33,58);
				else 
					dwColor=COLOR_ARGB(255,225,18,24);
			}
			break;
		}
	}

	return dwColor;

	FUNCTION_END;
}
*/

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptMapPixelOffset : 描述
 * \param dwColor : 描述
 * \return 返回值的描述
 */
bool CCharacter::IsUnKnowPerson()
{
	FUNCTION_BEGIN;
	#define  UNKNOWPERSON      "蒙面人"

	if (strcmp(GetName(),UNKNOWPERSON)==0)
		return true;

	return false;

	FUNCTION_END;

}

void CCharacter::DrawName(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;

	DWORD dwTitleColor=COLOR_ARGB(255,255,255,255);
	DWORD dwMasterColor=COLOR_ARGB(255,30,150,43);

	dwColor = GetNameColor(m_data.goodness);

	if(isset_state(GetState(),USTATE_PK))
	{
		dwColor = COLOR_ARGB(255,0,255,137);
	}

	if (GetScene()->GetMainCharacter()->GetProperty()->country!=GetProData()->country)
	{
		if( !CCountry::Instance()->IsAllyCountry(GetProData()->country))
		{
			dwColor = COLOR_ARGB(255,236,0,140);
		}
		else
		{ //盟国用户在小地图及平时人名显示采用粉色
			dwColor = COLOR_ARGB(255,255,128,192);
		}
	}
	
	
	if(isset_state(GetState(),USTATE_WAR))
	{
		dwColor = COLOR_ARGB(255,80,66,159);
	}

	if(GetName())
	{
		stPointI pt0 = GetPos() - ptMapPixelOffset;
		//int height = GetImageHeight();
		int nLine = 0;

		int iFontHeight = GetDevice()->GetFontHeight();
		
		char szTmp[256]={0};
		char szShow[256]={0};

		stRectI rcText(pt0.x - 500,pt0.y - m_iDrawTop,pt0.x + 500,0xffff);
		stRectI rcSwitchText = rcText;

#if 0//_DEBUG

		if (IsUnKnowPerson() )
		{
			dwColor = COLOR_ARGB(255,255,255,255);
			rcText.top-=iFontHeight;
			GetDevice()->DrawString(GetName(),rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
		}
		else if ( IsChangeFace() )
		{	
			NpcBase_t* pNpcBase = GetNpcBase( m_proChangingFace.m_dwChangingFaceID );
			if ( pNpcBase )
				sprintf( szShow, pNpcBase->strName );
			else
				sprintf( szShow, "" );
			
			rcText.top-=iFontHeight;
			GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
		}
		else
		{
			int nStrLen = 0;
			//stRectI rectApp;

			if (GetScene()->m_pMouseOverObject == this)
			{

				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_GRACENAME ))
				{
					nStrLen = strlen(GetGraceName());
					if (nStrLen>0)
					{
						/*rcText.top-=iFontHeight;
						rcText.left = pt0.x-(nStrLen*8+40)/2;
						rcText.right = pt0.x+(nStrLen*8+40)/2;
						rectApp=rcText;
						rectApp.left += 40;
						sprintf(szShow,"称号 ");
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_LEFT,FontEffect_Border);*/
						rcText.top-=iFontHeight;

						sprintf(szShow,"%s",GetGraceName());
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
				}
				
				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_OFFICENAME ))
				{
					nStrLen = strlen(GetOfficeName());
					if (nStrLen>0)
					{
						/*rcText.top-=iFontHeight;
						rcText.left = pt0.x-(nStrLen*8+40)/2;
						rcText.right = pt0.x+(nStrLen*8+40)/2;
						rectApp=rcText;
						rectApp.left += 40;
						sprintf(szShow,"官职 ");
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_LEFT,FontEffect_Border);*/
						rcText.top-=iFontHeight;

						sprintf(szShow,"%s",GetOfficeName());
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
					
				}

				
				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_SEPTNAME ))
				{
					nStrLen = strlen(GetSeptName());
					if (nStrLen>0)
					{
						rcText.top-=iFontHeight;

						sprintf(szShow,"%s 【家】",GetSeptName());
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
				}


				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_UNIONNAME ))
				{
					nStrLen = strlen(GetUnionName());
					if (nStrLen>0)
					{
						rcText.top-=iFontHeight;
						
						sprintf(szShow,"%s 【帮】",GetUnionName());
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
				}
			
			}

			//if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_COUNTRYNAME ))
			//{
			//	rcText.top-=iFontHeight;
			//	rcText.left=pt0.x - 500;
			//	rcText.right =pt0.x + 500;
			//	sprintf(szShow,"【%s】",GetCountryName());
			//	GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);

			//}
			DWORD dwStarColor;
			if (ShowCountryStar(m_dwStar,szShow,dwStarColor))
			{
				rcText.top-=iFontHeight;
				GetDevice()->DrawString(szShow,rcText,dwStarColor,DT_TOP | DT_CENTER,FontEffect_Border);
			}
			
		
			rcText.top-=iFontHeight;
			sprintf(szShow,"%s 【%s】(%u)(%u,%u)",GetName(),GetCountryName(),GetID(),GetGridPos().x,GetGridPos().y);
			GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
			
			nStrLen = strlen(GetProData()->caption);
			if (nStrLen>0)
			{
				rcText.top-=iFontHeight;

				sprintf(szShow,"%s",GetProData()->caption);
				GetDevice()->DrawString(szShow,rcText,dwMasterColor,DT_TOP | DT_CENTER,FontEffect_Border);

			}
		
		}
#else

		if ( IsChangeFace() )
		{
			NpcBase_t* pNpcBase = GetNpcBase( m_proChangingFace.m_dwChangingFaceID );
			if ( pNpcBase )
				sprintf( szShow, "%s(%u级)", pNpcBase->strName, m_proChangingFace.m_dwChangingFaceLevel );
			else
				sprintf( szShow, "" );

			rcText.top-=iFontHeight;
			m_iDrawTop += iFontHeight;
			GetDevice()->DrawString(szShow,rcText,-1,DT_TOP | DT_CENTER,FontEffect_Border);
		}
		else if (IsUnKnowPerson() )
		{
			if (m_data.goodness & GOODNESS_ATT)
				dwColor=COLOR_ARGB(255,63,43,0);
			else
				dwColor = COLOR_ARGB(255,255,255,255);

			rcText.top-=iFontHeight;
			m_iDrawTop += iFontHeight;
			GetDevice()->DrawString(GetName(),rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
		}		
		else
		{
			int nStrLen = 0;
			//stRectI rectApp;

			DWORD dwStarColor;
			if (ShowCountryStar(m_dwStar,szShow,dwStarColor))//五星
			{
				bool isShow5 = true; 
				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_PLAYERNAME ))
				{
					if (GetProData()->country == GetScene()->GetMainCharacter()->GetProperty()->country)
					{
						if (GetScene()->m_pMouseOverObject == this)
						{
							isShow5 = true;
						}
						else
						{
							isShow5 = false;
						}	
					}
					else
					{
						isShow5 = true;
					}
				}
				else
				{
					isShow5 = true;
				}
				//////////////////////////////////////////////////////////////////////////
				if (isShow5)
				{
					rcText.top-=iFontHeight;
					m_iDrawTop += iFontHeight;
					GetDevice()->DrawString(szShow,rcText,dwStarColor,DT_TOP | DT_CENTER,FontEffect_Border);
				}				
			}

			static CAnimation image;
			if ( NULL == image.GetImages() )
			{
				stResourceLocation rl;
				rl.SetFileName( "data\\interfaces.gl" );
				rl.group = 4;						
				rl.frame = 16;
				image.Create( &rl );
			}			

			rcText.top-=iFontHeight;
			m_iDrawTop += iFontHeight;
		#ifdef _DEBUG
			if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_PLAYERNAME ))
			{
				if (GetProData()->country == GetScene()->GetMainCharacter()->GetProperty()->country)
				{
					if (GetScene()->m_pMouseOverObject == this)
					{
						sprintf( szShow,"%s 【%s】(%d)",GetName(),GetCountryName(),GetID() );
					}
					else
					{
						sprintf( szShow,"%s(%d)",GetName(),GetID() );
					}	
				}
				else
				{
					sprintf( szShow,"%s 【%s】(%d)",GetName(),GetCountryName(),GetID() );
				}
			}
			else
			{
				sprintf( szShow,"%s 【%s】(%d)",GetName(),GetCountryName(),GetID() );
			}						
		#else
			if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_PLAYERNAME ))
			{
				if (GetProData()->country == GetScene()->GetMainCharacter()->GetProperty()->country)
				{
					if (GetScene()->m_pMouseOverObject == this)
					{
						sprintf( szShow,"%s 【%s】",GetName(),GetCountryName());
					}
					else
					{
						sprintf( szShow,"%s",GetName());
					}	
				}
				else
				{
					sprintf( szShow,"%s 【%s】",GetName(),GetCountryName());
				}
			}
			else
			{
				sprintf( szShow,"%s 【%s】",GetName(),GetCountryName());
			}		
		#endif
			if ( CCountry::Instance()->IsEmperorAt()
				&& CCountry::Instance()->CheckCountry(GetCountry(), GetScene()->GetMapCountryName()) )
			{
				stPointI pt;
				pt.x = rcText.left + (rcText.Width() - strlen(szShow)*GetDevice()->GetFontWidth()) / 2 - image.GetWidth() - 2;
				pt.y = rcText.top;
				image.Render( pt.x, pt.y );
			}
			GetDevice()->DrawString(szShow,rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);

			nStrLen = strlen(GetProData()->caption);
			if (nStrLen>0)
			{
				rcText.top-=iFontHeight;
				m_iDrawTop += iFontHeight;
				sprintf(szShow,"%s",GetProData()->caption);
				GetDevice()->DrawString(szShow,rcText,dwMasterColor,DT_TOP | DT_CENTER,FontEffect_Border);

			}
			else
			{
				 //1为队长，2为将军
				if( GetProData()->dwArmyState == 1) // 为队长
				{
					rcText.top-=iFontHeight;
					m_iDrawTop += iFontHeight;
					sprintf(szShow,"%s","队长");
					GetDevice()->DrawString(szShow,rcText,dwMasterColor,DT_TOP | DT_CENTER,FontEffect_Border);
    
				}
				else if(GetProData()->dwArmyState == 2) // 为将军
				{
					rcText.top-=iFontHeight;
					m_iDrawTop += iFontHeight;
					sprintf(szShow,"%s","将军");
					GetDevice()->DrawString(szShow,rcText,dwMasterColor,DT_TOP | DT_CENTER,FontEffect_Border);

				}
			}

			
			
			if (GetScene()->m_pMouseOverObject == this)
			{
				rcSwitchText.top-=iFontHeight;

				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_UNIONNAME ))
				{
					nStrLen = strlen(GetUnionName());
					if (nStrLen>0)
					{
						rcSwitchText.top+=iFontHeight;

						sprintf(szShow,"%s 【帮】",GetUnionName());
						GetDevice()->DrawString(szShow,rcSwitchText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
				}

				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_SEPTNAME ))
				{
					nStrLen = strlen(GetSeptName());
					if (nStrLen>0)
					{
						rcSwitchText.top+=iFontHeight;

						sprintf(szShow,"%s 【家】",GetSeptName());
						GetDevice()->DrawString(szShow,rcSwitchText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
				}


				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_OFFICENAME ))
				{
					nStrLen = strlen(GetOfficeName());
					if (nStrLen>0)
					{
						/*rcText.top-=iFontHeight;
						rcText.left = pt0.x-(nStrLen*8+40)/2;
						rcText.right = pt0.x+(nStrLen*8+40)/2;
						rectApp=rcText;
						rectApp.left += 40;
						sprintf(szShow,"官职 ");
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_LEFT,FontEffect_Border);*/
						rcSwitchText.top+=iFontHeight;

						sprintf(szShow,"%s",GetOfficeName());
						GetDevice()->DrawString(szShow,rcSwitchText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}

				}


				if (GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_GRACENAME ))
				{
					nStrLen = strlen(GetGraceName());
					if (nStrLen>0)
					{
						/*rcText.top-=iFontHeight;
						rcText.left = pt0.x-(nStrLen*8+40)/2;
						rcText.right = pt0.x+(nStrLen*8+40)/2;
						rectApp=rcText;
						rectApp.left += 40;
						sprintf(szShow,"称号 ");
						GetDevice()->DrawString(szShow,rcText,dwTitleColor,DT_TOP | DT_LEFT,FontEffect_Border);*/
						rcSwitchText.top+=iFontHeight;

						sprintf(szShow,"%s",GetGraceName());
						GetDevice()->DrawString(szShow,rcSwitchText,dwTitleColor,DT_TOP | DT_CENTER,FontEffect_Border);
					}
				}

			}


		}		
#endif
	}

	//CNpc::DrawName(ptMapPixelOffset,dwColor);

	FUNCTION_END;
}



void CCharacter::DrawCountryWarFlag(stPointI ptMapPixelOffset)
{
	FUNCTION_BEGIN;

        if( m_bShowWarFlag )
	{
		stPointI ptChar = GetPos();
		ptChar -= ptMapPixelOffset;
		ptChar.y -= m_iDrawTop;
		
		int iFontHeight = GetDevice()->GetFontHeight();

		DWORD dwColor = GetNameColor(m_data.goodness);

		if (GetScene()->GetMainCharacter()->GetProperty()->country!=GetProData()->country)
		{
			if( !CCountry::Instance()->IsAllyCountry(GetProData()->country))
			{
				if( CCountry::Instance()->IsDareCountryInWar(GetProData()->country))
				{
					dwColor = COLOR_ARGB(255,80,66,159);
				}	
				else 
				{
					dwColor = COLOR_ARGB(255,236,0,140);
				}
			}
		}	 	

		stRectI rcText(ptChar.x - 20,ptChar.y - iFontHeight,ptChar.x + 20,ptChar.y);
		GetDevice()->DrawString("国战",rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
	}

	//Draw emperor dare flag
	if ( GetScene()->GetMainCharacter() && GetScene()->GetMainCharacter()->GetEmperorDareZoneInfo().bNeedDraw )
	{
		stPointI ptChar = GetPos();
		ptChar -= ptMapPixelOffset;
		ptChar.y -= m_iDrawTop;

		int iFontHeight = GetDevice()->GetFontHeight();

		DWORD dwColor = GetNameColor(m_data.goodness);

		//Adjust color according to main character's dare type
		CMainCharacter::stEmperorDareZone& info = GetScene()->GetMainCharacter()->GetEmperorDareZoneInfo();
		if ( info.dwDefID != GetCountry() )
			dwColor = COLOR_ARGB(255,236,0,140);

		stRectI rcText(ptChar.x - 20,ptChar.y - iFontHeight,ptChar.x + 20,ptChar.y);
		GetDevice()->DrawString("皇城战",rcText,dwColor,DT_TOP | DT_CENTER,FontEffect_Border);
	}

	FUNCTION_END;
}

void CCharacter::DrawEmotionIcon(stPointI ptMapPixelOffset)
{
        FUNCTION_BEGIN;

	if(m_BitmapShow.IsNeedShow())
	{
		stPointI ptChar = GetPos();
		ptChar -= ptMapPixelOffset;
		ptChar.y -= m_iDrawTop;
		m_BitmapShow.Render(ptChar);
		m_iDrawTop -= (m_BitmapShow.rcClip.top+m_BitmapShow.rcClip.Height());
	}		

	FUNCTION_END;
}
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pSkill : 描述
 * \return 返回值的描述
 */
int CCharacter::ServerSkillToClientSkill(SkillBase_t* pSkill)
{
	FUNCTION_BEGIN;

	if( pSkill )
	{
		switch(pSkill->dwType)
		{
		case SkillType_Attack:
		case SkillType_StateAttack:
		case SkillType_Active:
			return pSkill->dwPic;
			break;
		}
	}
	return -1;

	FUNCTION_END;
}


/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  获得击中（近身攻击）的声音
*/
int CCharacter::GetHitSound()
{
	FUNCTION_BEGIN;

	int sound = HAND_HIT_SOUND;

	ObjectBase_t* pLObject = GetObjectBase(GetLeftWeaponID());
	ObjectBase_t* pRObject = GetObjectBase(GetRightWeaponID());

	if(pLObject && pLObject->dwType == ItemType_Bow)
	{
		if(pRObject)
			return pLObject->hitSound;
		else
			return BOW_EMPTY_HIT_SOUND;
	}

	if(pRObject)
	{
		sound = pRObject->hitSound;
	}
	return sound;

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  获得挥动武器的声音
*/
int CCharacter::GetAttackSound()
{
	FUNCTION_BEGIN;

	int sound = HAND_ATTACK_SOUND;
	ObjectBase_t* pLObject = GetObjectBase(GetLeftWeaponID());
	ObjectBase_t* pRObject = GetObjectBase(GetRightWeaponID());
	if( pLObject && pLObject->dwType == ItemType_Bow )
	{
		if(pRObject)
			return pLObject->attackSound;
		else 
			return BOW_EMPTY_ATTACK_SOUND;
	}

	if(pRObject)
	{
		sound = pRObject->attackSound;
	}

	return sound;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CCharacter::OnEquipChanged()
{
	FUNCTION_BEGIN;

	int sound = GetAttackSound();
	if(sound)
	{
		char szSound[MAX_PATH];
		sprintf(szSound,"%d",sound);
		m_pCharacter->SetSound(Ani_Attack,szSound);
		m_pCharacter->SetSound(Ani_Attack2,szSound);
		m_pCharacter->SetSound(Ani_Attack3,szSound);
		m_pCharacter->SetSound(Ani_Ride_Attack,szSound);
		m_pCharacter->SetSound(Ani_Ride_Bow,szSound);
		m_pCharacter->SetSound(Ani_Bow,szSound);
	}
	else
	{		
		m_pCharacter->ResetSound(Ani_Attack);
		m_pCharacter->ResetSound(Ani_Attack2);
		m_pCharacter->ResetSound(Ani_Attack3);
		m_pCharacter->ResetSound(Ani_Ride_Attack);

		m_pCharacter->ResetSound(Ani_Bow);
		m_pCharacter->ResetSound(Ani_Ride_Bow);
	}
	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pCmd : 描述
 * \return 返回值的描述
 */
bool CCharacter::HandleCommand(stNullUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	switch(pCmd->byCmdType)
	{
	case MAPSCREEN_USERCMD:
		switch(pCmd->byParam)
		{
		case REFRESHSTATE_MAPSCREEN_USERCMD_PARA:			// 刷新角色状态
			{
				stRefreshStateMapScreenUserCmd* p = (stRefreshStateMapScreenUserCmd*)pCmd;
				memcpy(m_data.state,p->state,sizeof(m_data.state));
				AfterSetAttrib(false,true);
				return true;
			}
			break;
		}
		break;
	}
	
	return CNpc::HandleCommand(pCmd);

	FUNCTION_END;
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptMapOffset : 描述
 * \return 返回值的描述
 */
void CCharacter::AddLight(POINT ptMapOffset)
{
	FUNCTION_BEGIN;

	
	m_pCharacter->AddLight(ptMapOffset);
	/*if(GetGameGuiManager()->m_guiTeam->FindMember(GetName()))
	{
	}*/
	if( GetMaxHP() && GetHP() )
	{
		// 是队友
		GetLightMap()->AddLight(GetPos()-ptMapOffset,GetScene()->GetLightRadius()/2,-1);
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
bool CCharacter::OnSetCursor()
{
	FUNCTION_BEGIN;

	if ( (GetGameCursor()->GetCursorImageType() == eCursorUseAccess)
		||( GetGameCursor()->GetCursorImageType() == eCursorShowUser )
		||( GetGameCursor()->GetCursorImageType() == eCursorFollowUser )
		||( GetGameCursor()->GetCursorImageType() == eCursorTrade )
		||( GetGameCursor()->GetCursorImageType() == eCursorPetAttack)
		||( GetGameCursor()->GetCursorTypeEx() == eCursorGetMailItem) )
		return false;

	if(IsCanAttack())
	{
		GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorAttack);
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
enumItemType CCharacter::GetRightWeaponType()
{
	FUNCTION_BEGIN;

	ObjectBase_t* pObject = GetObjectBase( GetRightWeaponID() );
	if(!pObject) return ItemType_None;
	return (enumItemType)pObject->dwType;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
enumItemType CCharacter::GetLeftWeaponType()
{
	FUNCTION_BEGIN;

	ObjectBase_t* pObject = GetObjectBase( GetLeftWeaponID() );
	if(!pObject) return ItemType_None;
	return (enumItemType)pObject->dwType;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param prevGrid : 描述
 * \param ptCurGrid : 描述
 * \return 返回值的描述
 */
void CCharacter::OnGridPosChange(POINT prevGrid,POINT ptCurGrid)
{
	FUNCTION_BEGIN;

	stGroundMusic* music = NULL;
	int curTileType = GetScene()->GetMapTileType(ptCurGrid);
	if( curTileType != m_tiletype )
	{
		m_tiletype = curTileType;
		music = GetScene()->GetMapTileMusic(curTileType);
		if( music )
		{
			//Engine_WriteLogF("进入地表类型 %d \n",music->)
			//得到地表的属性，设置声音。

			stSoundFx fx;
			if( music->walk.size() > 1 )
			{
				fx.nNum = music->walk[1];
				fx.nStartID = music->walk[0];
				m_pCharacter->SetSoundFx(Ani_Walk,fx);
			}

			if( music->run.size() > 1 )
			{
				fx.nNum = music->run[1];
				fx.nStartID = music->run[0];
				m_pCharacter->SetSoundFx(Ani_Run,fx);
			}
		}
	}

	FUNCTION_END;
}

const char* CCharacter::GetGraceName()
{
	return GetCivilianName(m_data.grace);
}
const char* CCharacter::GetOfficeName()
{
	return GetKnightName(m_data.exploit);
}

bool CCharacter::bIsShopStart()
{
	return isset_state(GetState(),USTATE_PRIVATE_STORE);
	return CNpc::bIsShopStart();
}

bool CCharacter::IsSeptLeader()
{
	FUNCTION_BEGIN;
	
	if ( NULL == GetGameGuiManager()->m_guiWorldConn )
		return false;

	CGuiWorldConn* pDlg = GetGameGuiManager()->m_guiWorldConn;
	if ( strlen( pDlg->m_SeptInfo.master ) == 0 )
		return false;

	if ( strcmp( pDlg->m_SeptInfo.master, GetName() ) != 0 )
		return false;

	return true;

	FUNCTION_END;
}

bool CCharacter::IsUnionLeader()
{
	FUNCTION_BEGIN;

	if ( NULL == GetGameGuiManager()->m_guiWorldConn )
		return false;

	CGuiWorldConn* pDlg = GetGameGuiManager()->m_guiWorldConn;
	if ( strlen( pDlg->m_TongInfo.master ) == 0 )
		return false;

	if ( strcmp( pDlg->m_TongInfo.master, GetName() ) != 0 )
		return false;

	return true;

	FUNCTION_END;
}

bool CCharacter::IsKing()
{
	FUNCTION_BEGIN;

	if ( IsEmperor() )
		return true;

	char caption [MAX_NAMESIZE] = {0};
	strcpy(caption,CCharacter::m_data.caption);
	str_trim_right(caption);
	str_trim_left(caption);

	size_t len = strlen(caption);
	if( len < 4)  return false;

	char * pLast4 = (char * )(caption + (len - 4) * sizeof(char));
	if( (0 == strcmp(pLast4,"国王"))
		|| (0 == strcmp(pLast4,"女王")))
		return true;
	return false;

	FUNCTION_END;
}

bool CCharacter::IsEmperor()
{
	FUNCTION_BEGIN;

	char caption [MAX_NAMESIZE] = {0};
	strcpy(caption,CCharacter::m_data.caption);
	str_trim_right(caption);
	str_trim_left(caption);

	size_t len = strlen(caption);
	if( len < 4)  return false;

	char * pLast4 = (char * )(caption + (len - 4) * sizeof(char));
	if( ( (0 == strcmp(pLast4,"皇帝")) && (GetType() & 1) )
		|| ( (0 == strcmp(pLast4,"女皇")) && (GetType() & 2) ) )
		return true;

	return false;

	FUNCTION_END;
}

bool CCharacter::IsCityLeader()
{
	FUNCTION_BEGIN;

	char caption [MAX_NAMESIZE] = {0};
	strcpy(caption,CCharacter::m_data.caption);
	str_trim_right(caption);
	str_trim_left(caption);

	size_t len = strlen(caption);
	if( len < 4)  return false;

	char * pLast4 = (char * )(caption + (len - 4) * sizeof(char));
	if( 0 == strcmp(pLast4,"城主"))
		return true;
	return false;

	FUNCTION_END;
}
bool CCharacter::IsGen()
{
	FUNCTION_BEGIN;

	 return  m_data.dwArmyState == 2;
	
	 FUNCTION_END;
}
bool CCharacter::IsArmyer()
{
	return m_data.dwArmyState != 0;
}

size_t CCharacter::GetLevel()
{
	return m_data.level;
}

const char* CCharacter::GetCountryName()
{	
	return m_countryName;
}

DWORD CCharacter::GetTeamState()
{
	return m_data.dwTeamState;
}

bool CCharacter::IsEquipedPickResourceTool()
{
	if ( !IsMainRole() )
		return false;

	if (GetRightWeaponID() != 876)
		return false;

	//Is dur ok
	CRoleItem* pItem = GetScene()->GetMainCharacter()->FindItemByLocationConstID( 876, OBJECTCELLTYPE_EQUIP );
	if ( NULL == pItem )
		return false;

	if ( pItem->GetObject()->dur <= 0 )
		return false;

	return true;
}

DWORD CCharacter::GetLeftWeaponColor() 
{
	//左手武器
	ObjectBase_t* pWeaponL = GetObjectBase(m_data.sculpt.dwLeftHandID);	
	if(pWeaponL)
		return pWeaponL->dwColor;
	return 0;
}

DWORD CCharacter::GetRightWeaponColor()
{
	//左手武器
	ObjectBase_t* pWeaponR = GetObjectBase(m_data.sculpt.dwRightHandID);
	if(pWeaponR)
		return pWeaponR->dwColor;
	return 0;
}

bool CCharacter::IsDragonTigerState()
{
	BYTE* byState = GetState();
	if ( isset_state(byState, USTATE_TOGETHER_WITH_DRAGON)
		|| isset_state(byState, USTATE_TOGETHER_WITH_TIGER) )
	{
		return true;
	}

	return false;
}

int CCharacter::GetRideID()
{
	return m_data.sculpt.dwHorseID > 0 ? m_data.sculpt.dwHorseID : CNpc::GetRideID(); 
}

void CCharacter::SetGoodness(DWORD dwGoodness)
{
	m_data.goodness = dwGoodness;
}