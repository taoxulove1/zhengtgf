#include "public.h"
#include ".\item.h"
#include "./Game.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./RoleItem.h"
#include "./GameCursor.h"
#include ".\GameAppation.h"
#include "./GameGuiManager.h"
#include ".\GuiTeam.h"
#include "../Magic/magic_sound.h"
/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CItem::CItem(void)
: m_pObjectBase(NULL)
, m_dwLevel(0)
, m_y(0)
, m_fSpeed(0.0f)
, m_fAcceleration(0.0f)
, m_image(NULL)
, m_star(NULL)
, m_dwOwner(0)
#ifdef _AUTOPLAY
, m_LastTryPickTime(0)
, m_LastSendPickCmdTime(0)
#endif
{
	FUNCTION_BEGIN;

	m_bMouseInName = false;
	m_dwSceneObjectType = OBJECT_ITEM;
	m_nextShowTime = 0;
	m_endShowTime = 0;
	m_bShowStar = false;

	m_stateOfItem = eState_Comm;
	m_timeOfStartOnGround = 0;

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CItem::~CItem(void)
{
	FUNCTION_BEGIN;

	if( GetScene()->m_pMouseOverObject == this){
		GetScene()->m_pMouseOverObject = NULL;
	}
	FUNCTION_END;
}

DWORD CItem::GetBaseID()
{
	FUNCTION_BEGIN;

	if ( m_pObjectBase )
		return m_pObjectBase->dwID;
	else
		return -1;

	FUNCTION_END;
}

DWORD CItem::GetItemObjectType()
{
	FUNCTION_BEGIN;

	if ( m_pObjectBase )
		return m_pObjectBase->dwType;
	else
		return -1;

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pData : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::SetAttrib(t_MapObjectData* pData,bool bDrop)
{
	FUNCTION_BEGIN;

	m_dwOwner = pData->dwOwner;

	SetName(pData->pstrName);

	//Debug_String(GetName());

	m_dwID = pData->dwMapObjectTempID;
	ObjectBase_t* pObjectBase = GetObjectBase(pData->dwObjectID);
	bool bResetImage = (m_pObjectBase != pObjectBase);
	m_pObjectBase = pObjectBase;

	m_dwLevel = pData->wdLevel;
	m_dwCount = pData->wdNumber;
	m_upgrade = pData->upgrade;
	if( m_upgrade > c_maxItemUpgrade )
		m_upgrade = c_maxItemUpgrade;
	m_kind = pData->kind;

	if(UpdateGridPosition(stPointI(pData->x,pData->y)))
	{
	}
	if(bResetImage)
	{
		ResetImage();
	}

	if(bDrop)
	{
		m_State = State_Up;
		//m_y = m_ptPixelOffset.y;
		m_y = GetMapScene()->GridCenterToPixelPos(stPointI(pData->x, pData->y)).y;
		m_ptPixelOffset.y -= 30 + (m_ptPixelOffset.y - GetMapScene()->GridCenterToPixelPos(stPointI(pData->x, pData->y)).y );
		m_fSpeed = -1000 + rand()%400 - 200;
		m_fAcceleration = 10000;

		// ²¥·ÅÎïÆ·ÔÚ¿ÕÖÐµÄÉùÒô
		//PlayItemSound(g_nItemAirSound);
		char szSound[MAX_PATH];
		sprintf(szSound,"%d",g_nItemAirSound);
		PlayItemSound(NULL,stPointI(m_ptGrid.x * GRID_WIDTH + GRID_WIDTH/2,m_ptGrid.y * GRID_HEIGHT + GRID_HEIGHT/2),stPointI(0,0),szSound,false);
	}
	else
		m_State = State_Static;

	m_stateOfItem = eState_Comm;
	DWORD dwId = GetBaseID(); 
	if( (dwId == c_nHuoYunFu) ||
		(dwId == c_nXianTianFu) ||
		(dwId == c_nXuanYuanFu) )
	{
		m_stateOfItem = eState_Start;
		m_timeOfStartOnGround = xtimeGetTime();
	}

	FUNCTION_END;
}

void CItem::LoseOwner()
{
	m_dwOwner = 0;  // 0 ´ú±íÃ»ÓÐonwer
	ResetImage();

}
/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::ResetImage()
{
	FUNCTION_BEGIN;

	if(m_pObjectBase)
	{
		stResourceLocation rl;
		rl.SetFileName(c_szItemGrpPack);
		rl.group = c_nGroupItemGround;
		rl.frame = m_pObjectBase->dwGroundPic;
		m_image = GetDevice()->FindBitmaps(&rl);

		rl.group = c_nGroupItemStartOnGround;
		m_imgStart = GetDevice()->FindBitmaps(&rl);

		rl.group = c_nGroupItemEndOnGround;
		m_imgEnd = GetDevice()->FindBitmaps(&rl);
	}
	stResourceLocation rl;
	rl.SetFileName("data\\other.gl");
	DWORD dwMainID = GetScene()->GetMainCharacter()->GetID();
	if( m_dwOwner == 0 || m_dwOwner == dwMainID 
		|| (      GetGameGuiManager()->m_guiTeam 
		      &&  NULL!=GetGameGuiManager()->m_guiTeam->FindMember(dwMainID)
		      &&  GetGameGuiManager()->m_guiTeam->m_dwCurObjAssign == TEAM_OBJ_MODE_NORMAL
		   ) 
          )
	{ //¿ÉÒÔ¼ñ
		rl.group = 4;
		rl.frame = 1;
		m_star = GetDevice()->FindBitmaps(&rl);
	}
	else 
	{
		rl.group = 4;
		rl.frame = 0;
		m_star = GetDevice()->FindBitmaps(&rl);

	}
	
	FUNCTION_END;
}

void CItem::OnRemove()
{
	FUNCTION_BEGIN;

	m_stateOfItem = eState_End;
	m_timeOfStartOnGround = xtimeGetTime();
	m_bDeleted = true;
		 
	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param time_elapsed : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::Run(long time_elapsed)
{
	FUNCTION_BEGIN;

	if(m_stateOfItem == eState_Start)
	{
		if( xtimeGetTime() - m_timeOfStartOnGround > 600 )
		{
			m_stateOfItem = eState_Comm;
		}
	}

	if(m_stateOfItem == eState_End)
	{
		if( xtimeGetTime() - m_timeOfStartOnGround > 600 )
		{
			m_bDeleted = true;
		}
	}

	if(m_State == State_Up)
	{
		m_ptPixelOffset.y += time_elapsed / 1000.f * m_fSpeed;
		m_fSpeed += time_elapsed / 1000.f * m_fAcceleration;
		if (m_ptPixelOffset.y > m_y)
		{
			m_ptPixelOffset.y = m_y;
			m_State = State_Static;
			if(m_pObjectBase)
			{
				char szSound[MAX_PATH];
				sprintf(szSound,"%d",m_pObjectBase->itmeSound);
				PlayItemSound(NULL,stPointI(m_ptGrid.x * GRID_WIDTH + GRID_WIDTH/2,m_ptGrid.y * GRID_HEIGHT + GRID_HEIGHT/2),stPointI(0,0),szSound,false);
				//PlayItemSound(m_pObjectBase->itmeSound);
			}
		}
	}
	else
	{
		if(!m_bShowStar)
		{
			if(xtimeGetTime() > m_nextShowTime)
			{
				m_nextShowTime = 3000 + rand() % 3000 + xtimeGetTime();
				m_endShowTime = 1200 + rand() % 800 + xtimeGetTime();
				m_bShowStar = true;
			}
		}
		else
		{
			if(xtimeGetTime() > m_endShowTime)
			{
				m_bShowStar = false;
			}
		}
	}

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param ptMapPixelOffset : ÃèÊö
 * \param dwColor : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::Draw(POINT ptMapPixelOffset,DWORD dwColor)
{
	FUNCTION_BEGIN;

	if( m_stateOfItem == eState_Start )
	{
		if(m_imgStart)
		{
			stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
			pt.x -= GetWidth()/2;
			pt.y -= GetHeight()/2;
			m_imgStart->RenderAni(pt.x,pt.y,xtimeGetTime(),dwColor);
		}
		return ;
	}

	if( m_stateOfItem == eState_Comm )
	{
		if(m_image)
		{
			stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
			pt.x -= GetWidth()/2;
			pt.y -= GetHeight()/2;
			m_image->RenderAni(pt.x,pt.y,xtimeGetTime(),dwColor);
		}
		if( (GetBaseID() == c_nHuoYunFu) ||
			(GetBaseID() == c_nXianTianFu) ||
			(GetBaseID() == c_nXuanYuanFu) )  return;
	}

	if( m_stateOfItem == eState_End )
	{
		if(m_imgEnd)
		{
			stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
			pt.x -= GetWidth()/2;
			pt.y -= GetHeight()/2;
			m_imgEnd->RenderAni(pt.x,pt.y,xtimeGetTime(),dwColor);
		}
		return ;
	}
	
	if(m_bShowStar)
	{
		if(m_star && m_star->GetBitmapCount())
		{
			stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
			pt.x -= m_star->GetBitmap(0)->GetWidth()/2;
			pt.y -= m_star->GetBitmap(0)->GetHeight()/2;
			m_star->RenderAni(pt.x,pt.y,xtimeGetTime(),dwColor);
		}
	}

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
bool CItem::InScreen()
{
	FUNCTION_BEGIN;

	stRectI rect(m_ptPixelOffset.x,m_ptPixelOffset.y , m_ptPixelOffset.x + GetWidth(),m_ptPixelOffset.y + GetHeight());
	rect &= GetScene()->GetDrawPixelRect();
	return !rect.IsRectEmpty();

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param ptMapPixelOffset : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::DrawBack(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	if ( m_State != State_Static )
		return;

	m_bMouseInName = false;
	if( GetGameApplication()->GetConfig()->bSystemSetted(USER_SETTING_SHOW_ITEM) ||
		GetScene()->m_pMouseOverObject == this || Engine_GetState()->IsAltDown())
	{
		stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
		
		int iFontWidth = GetDevice()->GetFontWidth(1);
		int iFontHeight = GetDevice()->GetFontHeight(1) * m_textHeight;

		int cx = strlen(GetName()) * iFontWidth;
		pt.y -= (GetHeight()/2 + 6 + iFontHeight);
		pt.x -= (cx/2 + 3);

		stRectI rect(pt.x,pt.y,pt.x + cx + 6,pt.y + (6 + 12) * m_textHeight);

		m_bMouseInName = rect.PtInRect(Engine_GetCursor()->GetPosition());

		if(m_bMouseInName) GetDevice()->FillRect(&rect,COLOR_ARGB(128,128,128,128));
		else		GetDevice()->FillRect(&rect,COLOR_ARGB(128,0,0,0));
	}

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param ptMapPixelOffset : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::DrawName(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	m_textHeight = 0;
	
	if ( NULL == m_pObjectBase )
	{
		stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
		pt.y -= (GetHeight()/2 + 3);
		stRectI rcText(pt.x - 500,-100,pt.x + 500,pt.y);
		if(m_dwCount > 1)
		{
			char szText[MAX_PATH];
			_snprintf(szText,sizeof(szText),"%s %u",GetName(),m_dwCount);
			szText[sizeof(szText)-1] = 0;
			GetDevice()->DrawString(szText,rcText,-1,DT_BOTTOM | DT_CENTER);
		}
		else
		{
			GetDevice()->DrawString(GetName(),rcText,-1,DT_BOTTOM | DT_CENTER);
		}

		m_textHeight = 1;
	}
	else
	{		
		//Validate item grade
		if ( m_upgrade > 14 )
			m_upgrade = 14;

		stPointI pt = m_ptPixelOffset - ptMapPixelOffset;
		pt.y -= (GetHeight()/2 + 3);
		stRectI rcText(pt.x - 500,-100,pt.x + 500,pt.y);

		DWORD dwId = GetBaseID(); 
		if( (dwId == c_nHuoYunFu) ||
			(dwId == c_nXianTianFu) ||
			(dwId == c_nXuanYuanFu) )
		{			
			stPointI ptTemp = m_ptPixelOffset - ptMapPixelOffset;
			stRectI rcTemp(ptTemp.x - 500,ptTemp.y - 170,ptTemp.x + 500,ptTemp.y - 150);
			char szpos[64] = {'0'};
#ifdef _DEBUG
			stPointI ptCurGrid = GetCurGridPos();
			sprintf(szpos,"%s,%d,%d",GetName(),ptCurGrid.x,ptCurGrid.y);
#else
			sprintf(szpos,"%s",GetName());
#endif
			GetDevice()->DrawString(szpos,rcTemp,-1,DT_BOTTOM | DT_CENTER);
			return ;
		}

		int textHeight = 15;
		//Set the name same as in item package
		//Name
		DWORD dwColor = COLOR_ARGB(255,255,255,255);
		DWORD gradeColor = COLOR_ARGB(255,255,255,255);
		//if ( m_kind & 8 )		//set
		//	dwColor = COLOR_ARGB(255,146,77,22);
		if( m_kind & 4 )
			dwColor = COLOR_ARGB(255,0,255,137);
		else if( m_kind &2 )
			dwColor = COLOR_ARGB(255,255,222,3);
		else if( m_kind & 1 )
			dwColor = COLOR_ARGB(255,0,86,234);

		//Split item name
		char* pName;
		char namePartOne[MAX_NAMESIZE];
		char namePartTwo[MAX_NAMESIZE];
		char szCount[MAX_NAMESIZE];
		if ( m_dwCount > 1 )
			sprintf( szCount, " %u", m_dwCount );
		else
			szCount[0] = 0L;

		pName = strpbrk( GetName(), "0" );
		if ( pName )
		{
			sprintf( namePartOne, ++pName );
			int len = strlen(GetName())-strlen(namePartOne)-1;
			strncpy( namePartTwo, GetName(), len );
			namePartTwo[len] = 0;
		}
		else
		{
			sprintf( namePartTwo, GetName() );
			sprintf( namePartOne, "" );
		}
		//End split item name
		
		/*static char* upgradeName[] = { " Ò»µÈ", " ¶þµÈ", " ÈýµÈ", " ËÄµÈ", " ÎåµÈ",
			" ÁùµÈ", " ÆßµÈ", " °ËµÈ", " ¾ÅµÈ", " Ê®µÈ", "Ê®Ò»µÈ", "Ê®¶þµÈ", "Ê®ÈýµÈ", "Ê®ËÄµÈ", "Ê®ÎåµÈ" };*/

		if ( 4 & m_kind )				//ÉñÊ¥µÀ¾ß
		{
			//Add grade
			char szUpgrade[MAX_PATH];
			szUpgrade[0] = 0L;
			
			for( int i=0;i<m_upgrade;i++ )
			{
				strcat( szUpgrade, "¡ï" );
			}
			szUpgrade[strlen(szUpgrade)] = 0L;

			switch( m_upgrade )
			{
			case 0:
				break;
			case 1:
				gradeColor = D3DCOLOR_ARGB(255,255,255,255);
				break;
			case 2:
				gradeColor =  D3DCOLOR_ARGB(255,0,86,234);
				break;
			case 3:
				gradeColor =  D3DCOLOR_ARGB(255,0,86,234);
				break;
			case 4:
				gradeColor =  D3DCOLOR_ARGB(255,255,222,3);
				break;
			case 5:
				gradeColor =  D3DCOLOR_ARGB(255,255,222,3);
				break;
			case 6:
				gradeColor =  D3DCOLOR_ARGB(255,0,255,0);
				break;
			case 7:
				gradeColor =  D3DCOLOR_ARGB(255,0,255,0);
				break;
			case 8:
				gradeColor =  D3DCOLOR_ARGB(255,255,113,0);
				break;
			case 9:
			case 10:
			case 11:
				gradeColor =  D3DCOLOR_ARGB(255,211,0,237);
				break;
			}
			
			//Add grade
			if ( strlen( szUpgrade ) > 0 )
			{
				GetDevice()->DrawString( szUpgrade, rcText, gradeColor, DT_BOTTOM | DT_CENTER );
				m_textHeight++ ;
			}

			//Add part one name
			char szFullNameOne[256];
			if ( m_dwCount > 1 )
				sprintf( szFullNameOne, "%s %u", namePartOne, m_dwCount );
			else
				sprintf( szFullNameOne, "%s", namePartOne );

			if ( strlen(szFullNameOne) > 0 )
			{
				if ( m_textHeight > 0 )
					rcText.bottom -= textHeight;
				GetDevice()->DrawString( szFullNameOne, rcText, dwColor, DT_BOTTOM | DT_CENTER );
				m_textHeight++ ;
			}

			//Add name part two
			if ( strlen(namePartTwo) > 0 )
			{
				if ( m_textHeight > 0 )
					rcText.bottom -= textHeight;
				GetDevice()->DrawString( namePartTwo, rcText, dwColor, DT_BOTTOM | DT_CENTER );
				m_textHeight++ ;
			}			
		}
		else
		{
			if( ( (m_pObjectBase->dwType == ItemType_Resource)
					|| (m_pObjectBase->dwType == ItemType_SOULSTONE) 
					|| (m_pObjectBase->dwType == ItemType_LevelUp) )
				 && ( (m_pObjectBase->dwID == 506 )
					|| (m_pObjectBase->dwID == 507)
					|| (m_pObjectBase->dwID == 516)
					|| (m_pObjectBase->dwID == 517)
					|| (m_pObjectBase->dwID == 526)
					|| (m_pObjectBase->dwID == 527)
					|| (m_pObjectBase->dwID == 536)
					|| (m_pObjectBase->dwID == 537)
					|| (m_pObjectBase->dwID == 546)
					|| (m_pObjectBase->dwID == 547)
					|| (m_pObjectBase->dwID == 877) ) )			//ÀÍ¶¯Ô­ÁÏÀà		m_pObjectBase->dwType == 16
			{
				char szUpgrade[MAX_PATH];
				szUpgrade[0] = 0L;

				switch( m_upgrade )
				{
				case 0:
					gradeColor = D3DCOLOR_ARGB(255,255,255,255);
					break;
				case 1:
					gradeColor = D3DCOLOR_ARGB(255,0,86,234);
					break;
				case 2:
					gradeColor = D3DCOLOR_ARGB(255,255,222,3);
					break;
				case 3:
					gradeColor = D3DCOLOR_ARGB(255,0,255,0);
					break;
				case 4:
					gradeColor = D3DCOLOR_ARGB(255,211,0,237);
					break;
				case 5:
					gradeColor = D3DCOLOR_ARGB(255,211,0,237);
					break;
				case 6:
					gradeColor = D3DCOLOR_ARGB(255,211,0,237);
					break;
				case 7:
					gradeColor = D3DCOLOR_ARGB(255,211,0,237);
					break;
				case 8:
					gradeColor = D3DCOLOR_ARGB(255,211,0,237);
					break;
				case 9:
					gradeColor = D3DCOLOR_ARGB(255,255,255,255);
					break;
				}

				if ( strlen(namePartOne) > 0 )
				{
					GetDevice()->DrawString( namePartTwo, rcText, gradeColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;

					if ( m_dwCount > 1 )
					{
						strcat( szUpgrade, namePartOne );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, upgradeName[m_upgrade] );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, szCount );
					}
					else
					{
						strcat( szUpgrade, namePartOne );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, upgradeName[m_upgrade] );
					}

					szUpgrade[strlen(szUpgrade)] = 0L;
					if ( m_textHeight > 0 )
						rcText.bottom -= textHeight;
					GetDevice()->DrawString( szUpgrade, rcText, gradeColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;
				}
				else
				{
					if ( m_dwCount > 1 )
					{
						strcat( szUpgrade, namePartTwo );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, upgradeName[m_upgrade] );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, szCount );
					}
					else
					{
						strcat( szUpgrade, namePartTwo );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, upgradeName[m_upgrade] );
					}

					szUpgrade[strlen(szUpgrade)] = 0L;
					GetDevice()->DrawString( szUpgrade, rcText, gradeColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;
				}
			}
			else
			{
				//Add grade
				char szUpgrade[MAX_PATH];
				szUpgrade[0] = 0L;

				if ( (m_pObjectBase->dwID == 678)
					|| (m_pObjectBase->dwID == 679)

					|| (m_pObjectBase->dwID == 681)
					|| (m_pObjectBase->dwID == 792) 
					|| (m_pObjectBase->dwID == 794)
					|| (m_pObjectBase->dwID == 795)

					|| (m_pObjectBase->dwID == 791)
					|| (m_pObjectBase->dwID == 790)

					|| (m_pObjectBase->dwID == 680)
					|| (m_pObjectBase->dwID == 748)
					|| (m_pObjectBase->dwID == 749)
					|| (m_pObjectBase->dwID == 750)
					|| (m_pObjectBase->dwID == 751)	)		//Diamond
				{
					if ( (m_pObjectBase->dwID == 678)
						|| (m_pObjectBase->dwID == 679) )
					{
						switch( m_upgrade )
						{
						case 0:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,255,255,255);
							break;
						case 1:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,0,174,239);
							break;
						case 2:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,8,251,231);
							break;
						case 3:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,182,243,18);
							break;
						case 4:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,246,101,44);
							break;
						case 5:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,253,90,236);
							break;
						case 6:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,255,222,3);
							break;
						case 7:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,154,63,232);
							break;
						case 8:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,188,66,210);
							break;
						case 9:
						case 10:
						case 11:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,211,0,237);
							break;
						default:
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
							gradeColor = D3DCOLOR_ARGB(255,255,255,255);
							break;
						}
					}
					else if ( (m_pObjectBase->dwID == 681)
						|| (m_pObjectBase->dwID == 792) 
						|| (m_pObjectBase->dwID == 794)
						|| (m_pObjectBase->dwID == 795) )
					{
						if ( (m_pObjectBase->dwID == 681) || (m_pObjectBase->dwID == 795) )
							dwColor = m_pObjectBase->attackSound | 0xff000000;
						else
							dwColor = D3DCOLOR_ARGB(255,255,255,255);
						gradeColor = D3DCOLOR_ARGB(255,0,255,137);
					}
					else if ( (m_pObjectBase->dwID == 791)
						|| (m_pObjectBase->dwID == 790) )
					{
						dwColor = D3DCOLOR_ARGB(255,255,255,255);
						gradeColor = D3DCOLOR_ARGB(255,0,255,137);
					}
					else if ( (m_pObjectBase->dwID == 680)
						|| (m_pObjectBase->dwID == 748)
						|| (m_pObjectBase->dwID == 749)
						|| (m_pObjectBase->dwID == 750)
						|| (m_pObjectBase->dwID == 751) )
					{
						dwColor = D3DCOLOR_ARGB(255,255,255,255);
						gradeColor = D3DCOLOR_ARGB(255,0,255,137);
					}
					else
					{
						dwColor = D3DCOLOR_ARGB(255,255,255,255);
						gradeColor = D3DCOLOR_ARGB(255,255,255,255);
					}

					if ( (m_pObjectBase->dwID == 678) || (m_pObjectBase->dwID == 679) )
					{
						strcat( szUpgrade, upgradeName[m_upgrade] );
					}
				}
				else
				{
					gradeColor = COLOR_ARGB(255,255,255,255);
					for( int i=0;i<m_upgrade;i++ )
						strcat( szUpgrade, "¡ï" );
				}

				//Add grade
				szUpgrade[strlen(szUpgrade)] = 0L;
				if ( strlen( szUpgrade ) > 0 )
				{
					GetDevice()->DrawString( szUpgrade, rcText, gradeColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;
					rcText.bottom -= textHeight;
				}
				szUpgrade[0] = 0L;
				
				//Add name
				if ( strlen(namePartOne) > 0 )
				{
					GetDevice()->DrawString( namePartTwo, rcText, dwColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;
					
					if ( m_dwCount > 1 )
					{
						strcat( szUpgrade, namePartOne );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, szCount );
					}
					else
						strcat( szUpgrade, namePartOne );

					szUpgrade[strlen(szUpgrade)] = 0L;
					rcText.bottom -= textHeight;
					GetDevice()->DrawString( szUpgrade, rcText, dwColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;
				}
				else
				{
					if ( m_dwCount > 1 )
					{
						strcat( szUpgrade, namePartTwo );
						strcat( szUpgrade, " " );
						strcat( szUpgrade, szCount );
					}
					else
						strcat( szUpgrade, namePartTwo );

					szUpgrade[strlen(szUpgrade)] = 0L;
					GetDevice()->DrawString( szUpgrade, rcText, dwColor, DT_BOTTOM | DT_CENTER );
					m_textHeight++ ;
				}
			}
		}	
	}

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param ptMapPixelOffset : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CItem::DrawText(POINT ptMapPixelOffset)
{
	FUNCTION_BEGIN;

	if ( m_State != State_Static )
		return;

	if( GetGameApplication()->GetConfig()->bSystemSetted(USER_SETTING_SHOW_ITEM) ||
		GetScene()->m_pMouseOverObject == this || Engine_GetState()->IsAltDown())
		DrawName(ptMapPixelOffset);

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
POINT CItem::GetSortPos()
{
	FUNCTION_BEGIN;

	return stPointI(m_ptPixelOffset.x,m_y - GRID_HEIGHT/2);

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pt : ÃèÊö
 * \param bJustInRect : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
bool CItem::IsAtPixelPos(POINT pt, bool bJustInRect)
{
	FUNCTION_BEGIN;

	if(m_bMouseInName) return true;
	stPointI pt2 = m_ptPixelOffset;
	pt2.x -= GetWidth()/2;
	pt2.y -= GetHeight()/2;
	if(bJustInRect)
	{ 
		stRectI rc(pt2.x,pt2.y,pt2.x + GetWidth(),pt2.y + GetHeight());
		return rc.PtInRect(pt);
	}
	pt.x -= pt2.x;
	pt.y -= pt2.y;
	
	if(m_image && m_image->GetFrameCount())
		return ( m_image->GetBitmap(0)->GetAlpha(pt) != 0 );

	return false;

	FUNCTION_END;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
bool CItem::OnSetCursor()
{
	FUNCTION_BEGIN;

	DWORD dwId = GetBaseID(); 
	if( (dwId == c_nHuoYunFu) ||
		(dwId == c_nXianTianFu) ||
		(dwId == c_nXuanYuanFu) )
	{
		return false;
	}	

	if (( GetGameCursor()->GetCursorImageType() == eCursorUseAccess ) 
		||( GetGameCursor()->GetCursorImageType() == eCursorShowUser )
		||( GetGameCursor()->GetCursorImageType() == eCursorFollowUser )
		||( GetGameCursor()->GetCursorImageType() == eCursorTrade ) 
		||( GetGameCursor()->GetCursorImageType() == eCursorPetAttack )
		||( GetGameCursor()->GetCursorTypeEx() == eCursorGetMailItem ) )
		return false;

	GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorGetItem);
	return true;

	FUNCTION_END;
}
