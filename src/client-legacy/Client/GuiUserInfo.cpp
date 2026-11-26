/**
*\file		GuiUserInfo.cpp
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2006-7-19  21:11:00 中国标准时间
*\brief	    人物属性界面对话框
*
* 
*
*/
 

#include "public.h"
#include "./GameGuiManager.h"
#include "./guiuserinfo.h"
#include "./GameScene.h"
#include "./character.h"
#include "./MainCharacter.h"
#include "./CreateRole.h"
#include "./GuiWorldConn.h"
#include "./GuiItem.h"
#include "./RoleItem.h"
#include "./GameGuiFloatItem.h"
#include "./Country.h"


#define IDC_ADDPOINT_BASE 200
#define IDC_FIVEPOINT_BASE 300
#define IDC_FIVEPOINT_RECOVERY 400


#define UserInfo_SetStaticText(id,txt)			{CGuiStatic *p##id = GetStatic(id);if(p##id) p##id->SetText(txt);}
#define UserInfo_SetHorseText(id,txt,d)			{sprintf(szText,"%s%u",txt,pData->d);UserInfo_SetStaticText(id,szText);}
#define UserInfo_SetHorseTextPercent(id,txt,d)	{sprintf(szText,"%s%u%%",txt,pData->d);UserInfo_SetStaticText(id,szText);}
#define UserInfo_SetHorseExt(txt,mem)			{if( pData->mem > 0 )	{UserInfo_SetHorseText(nID,txt,mem); nID++;}}
#define UserInfo_SetHorseExtPercent(txt,mem)	{if( pData->mem > 0 )	{UserInfo_SetHorseTextPercent(nID,txt,mem); nID++;}}
#define UserInfo_SetHorseImage(id,img)			{CGuiImage* p = GetImage(id);if(p) p->SetImage(stResourceLocation("data\\icons.gl",5,img));}


struct stPropTxt {
	byte  nButtonId;
	byte  nProp;
	char  szTxt[64];
};

const stPropTxt  g_PropTxt[]=
{
	{1,PROPERTY_CON,"增加力量可提高角色的物理攻击力\n同时提高物理防御力和生命值上限"},
	{3,PROPERTY_STR,"增加智力可提高角色的魔法攻击力\n同时提高魔法防御力和法术值上限"},
	{2,PROPERTY_DEX,"增加敏捷可提高角色的物理防御力\n同时提高魔法防御力"},
	{4,PROPERTY_INT,"增加精神可提高角色的法术值上限\n同时提高魔法防御力"},
	{0,PROPERTY_MEN,"增加体质可提高角色的生命值上限\n同时提高物理防御力和魔法防御力"}
};

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param five : 描述
 * \return 返回值的描述
 */
const char* GetPropTxt(byte nButtonId)
{
	FUNCTION_BEGIN;
	static int size=sizeof(g_PropTxt)/sizeof(stPropTxt);


	for (int i=0;i<size;i++)
	{
		if (nButtonId==g_PropTxt[i].nButtonId)
			return g_PropTxt[i].szTxt;
	}
	
	return NULL;

	FUNCTION_END;
}



/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGuiUserInfo::CGuiUserInfo(bool bMainUser)
:m_bMainUser(bMainUser)
,m_bShowBody(true)
,m_bEquipe(false)
,m_bClothes(false)
,m_LevelNum(0)
,m_equipByMakerType(EquipByMaker_T_NONE)
{
	FUNCTION_BEGIN;

	m_bCloseIsHide = true;

	memset(&m_UserData,0,sizeof(m_UserData));

	m_vEquipByMaker.clear();

	m_listActiveItem.clear();
	
	
	FUNCTION_END;
}

void CGuiUserInfo::SetUserData(void* pData)
{
	if (m_bMainUser)
	{
		m_UserData.pMainUserData = GetScene()->GetMainCharacter()->GetProperty();
		m_UserData.pMapUserData = GetScene()->GetMainCharacter()->GetProData();
		m_UserData.pMapUserData->dwBodyColorSystem = GetScene()->GetMainCharacter()->GetBodySystemColor();

		/*stLevelDegreeDataUserCmd cmd;
		SEND_USER_CMD(cmd);*/
	}
	else
	{
		m_bEquipe = false;
		m_bClothes = false;
		m_equipByMakerType = EquipByMaker_T_NONE;
		m_listActiveItem.clear();

		//Clear EquipByMaker list
		for( int i=0;i<m_vEquipByMaker.size();i++ )
		{
			m_vEquipByMaker[i].Init();
		}
		m_vEquipByMaker.clear();

		//Init set maker name
		m_strSetMakerName[0] = 0L;

		m_UserData.pMainUserData = &(m_UserData.tMainUserData);
		m_UserData.pMapUserData = &(m_UserData.tMapUserData);
		if (pData==NULL)
			return;
		
		stSelectReturnMainUserPropertyUserCmd*  pCmd = (stSelectReturnMainUserPropertyUserCmd*) pData;
		memcpy(&(m_UserData.tMainUserData),&(pCmd->mainuser_data),sizeof(m_UserData.tMainUserData));
		memcpy(&(m_UserData.tMapUserData),&(pCmd->mapuser_data),sizeof(m_UserData.tMapUserData));

		//Modify exploit value from server
		//Scale 0.01
		m_UserData.tMapUserData.exploit = pCmd->mapuser_data.exploit / 100;

		ObjectBase_t* pObject = GetObjectBase(m_UserData.tMapUserData.sculpt.dwBodyID);
		if(pObject) m_UserData.tMapUserData.dwBodyColorSystem = pObject->dwColor;

		for (int i=0;i<m_listItem.size();i++)
		{
			m_listItem[i]->OnRemove();
			m_allocRoleItem.freePointer(m_listItem[i]);
		}
		m_listItem.clear();

		SetUserImage(EQUIPCELLTYPE_BODY);
		SetUserImage(EQUIPCELLTYPE_HANDR);
		SetUserImage(EQUIPCELLTYPE_HANDL);


		for (int i=0;i<pCmd->dwSize;i++)
		{
			CRoleItem* pItem = LOOKASIDE_ALLOC(m_allocRoleItem);
			stAddObjectPropertyUserCmd  cmd;
			memcpy(&(cmd.object),&(pCmd->object_data[i].object),sizeof(t_Object));

			cmd.object.pos.dwLocation = OBJECTCELLTYPE_EQUIPSHOW;
			//cmd.object.pos.x = 0;
			//cmd.object.pos.y = pCmd->object_data[i].byWhere;

			if( !pItem->SetAttrib(&cmd) )
			{
				m_allocRoleItem.freePointer(pItem);
				return;
			}
			m_listItem.push_back(pItem);
			
			//Refresh player equipment type
			SetEquipByName( pItem, false );
		}

		//Refresh play item list for five-type-link
		if ( !m_listItem.empty() )
		{
			for( std::vector<CRoleItem*>::iterator itor=m_listItem.begin();itor!=m_listItem.end();++itor )
			{
				ActiveItem( (*itor), false );
			}
		}
	}
	InitIcon();
}
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGuiUserInfo::OnClose(void)
{
	FUNCTION_BEGIN;

	for (int i=0;i<m_listItem.size();i++)
	{
		m_listItem[i]->OnRemove();
		m_allocRoleItem.freePointer(m_listItem[i]);
	}

	if (m_bMainUser)
		GetGameGuiManager()->m_guiUserInfo = NULL;
	else
		GetGameGuiManager()->m_guiUserInfoShow = NULL;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGuiUserInfo::OnSetCloseLocation()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();


	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());

		m_pBtnClose->SetLocation(pt.x,pt.y+4);
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
void CGuiUserInfo::InitIcon()
{
	FUNCTION_BEGIN;

	if (m_UserData.pMapUserData==NULL)
		return;

	stResourceLocation rl(GetGuiGraphicPackName(),33,m_UserData.pMapUserData->face);
	SetIcon(&rl);

	m_ptIcon.x = 33;
	m_ptIcon.y = 90;

	FUNCTION_END;
}

void CGuiUserInfo::ControlImage(byte pos)
{
	CRoleItem* pRoleItem = NULL;
	CGuiImage* pImage = NULL;
	switch(pos) {
	case EQUIPCELLTYPE_ADORN:
	case EQUIPCELLTYPE_BODY:
		{
			pRoleItem = GetShowClothes();
			pImage = m_pImage[0];
			if (pRoleItem&&(pRoleItem->GetLocation().y==EQUIPCELLTYPE_ADORN)&&(pRoleItem->GetCellCount()>1))
			{
				CRoleItem* pEquipItem = GetEquipItem(EQUIPCELLTYPE_BODY);
				if (pEquipItem)
					pRoleItem = pEquipItem;
			}
			if (pRoleItem)
				SetImage(pImage,pos,pRoleItem->GetObject());
		}
		break;
	case EQUIPCELLTYPE_HANDL:
		pRoleItem = GetEquipItem(EQUIPCELLTYPE_HANDL);
		pImage = m_pImage[1];
		break;
	case EQUIPCELLTYPE_HANDR:
		pRoleItem = GetEquipItem(EQUIPCELLTYPE_HANDR);
		pImage = m_pImage[2];
		break;
	}

	if ((pImage!=NULL)&&(pRoleItem!=NULL))
	{
		if (m_pTab->GetCurItem()!=0)
		{
			pImage->SetVisible(false);
			return;
		}
		else
			pImage->SetVisible(true);

		if (pRoleItem->GetCellCount()>1)
		{
			pImage->SetVisible(false);
			if ((pos==EQUIPCELLTYPE_BODY)||(pos==EQUIPCELLTYPE_ADORN))
				m_bShowBody = true;
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
void CGuiUserInfo::OnCreate()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnCreate();

	byte nTableType = m_bMainUser?OBJECTCELLTYPE_EQUIP:OBJECTCELLTYPE_EQUIPSHOW;

	GetEquipTable(EQUIPCELLTYPE_HELM) = GetTable(100);
	GetEquipTable(EQUIPCELLTYPE_HELM)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HELM)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HELM);

	GetEquipTable(EQUIPCELLTYPE_NECKLACE) = GetTable(101);
	GetEquipTable(EQUIPCELLTYPE_NECKLACE)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_NECKLACE)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_NECKLACE);

	GetEquipTable(EQUIPCELLTYPE_GLOVES,EQUIPCELLTYPE_RIGHT) = GetTable(102);
	GetEquipTable(EQUIPCELLTYPE_GLOVES,EQUIPCELLTYPE_RIGHT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_GLOVES,EQUIPCELLTYPE_RIGHT)->m_EquipPosition = stPointI(EQUIPCELLTYPE_RIGHT,EQUIPCELLTYPE_GLOVES);

	GetEquipTable(EQUIPCELLTYPE_GLOVES,EQUIPCELLTYPE_LEFT) = GetTable(103);
	GetEquipTable(EQUIPCELLTYPE_GLOVES,EQUIPCELLTYPE_LEFT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_GLOVES,EQUIPCELLTYPE_LEFT)->m_EquipPosition = stPointI(EQUIPCELLTYPE_LEFT,EQUIPCELLTYPE_GLOVES);

	GetEquipTable(EQUIPCELLTYPE_RING,EQUIPCELLTYPE_RIGHT) = GetTable(104);
	GetEquipTable(EQUIPCELLTYPE_RING,EQUIPCELLTYPE_RIGHT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_RING,EQUIPCELLTYPE_RIGHT)->m_EquipPosition = stPointI(EQUIPCELLTYPE_RIGHT,EQUIPCELLTYPE_RING);

	GetEquipTable(EQUIPCELLTYPE_RING,EQUIPCELLTYPE_LEFT) = GetTable(105);
	GetEquipTable(EQUIPCELLTYPE_RING,EQUIPCELLTYPE_LEFT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_RING,EQUIPCELLTYPE_LEFT)->m_EquipPosition = stPointI(EQUIPCELLTYPE_LEFT,EQUIPCELLTYPE_RING);

	GetEquipTable(EQUIPCELLTYPE_SHOES) = GetTable(106);
	GetEquipTable(EQUIPCELLTYPE_SHOES)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_SHOES)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_SHOES);

	GetEquipTable(EQUIPCELLTYPE_BELT) = GetTable(107);
	GetEquipTable(EQUIPCELLTYPE_BELT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_BELT)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_BELT);

	GetEquipTable(EQUIPCELLTYPE_BODY) = GetTable(108);
	GetEquipTable(EQUIPCELLTYPE_BODY)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_BODY)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_BODY);

	GetEquipTable(EQUIPCELLTYPE_HANDR) = GetTable(109);
	GetEquipTable(EQUIPCELLTYPE_HANDR)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HANDR)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HANDR);

	GetEquipTable(EQUIPCELLTYPE_HANDL) = GetTable(110);
	GetEquipTable(EQUIPCELLTYPE_HANDL)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_HANDL)->m_EquipPosition = stPointI(0,EQUIPCELLTYPE_HANDL);

	GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_LEFT) = GetTable(111);
	GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_LEFT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_LEFT)->m_EquipPosition = stPointI(EQUIPCELLTYPE_LEFT,EQUIPCELLTYPE_ADORN);

	GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_RIGHT) = GetTable(112);
	GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_RIGHT)->m_iTableType = nTableType;
	GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_RIGHT)->m_EquipPosition = stPointI(EQUIPCELLTYPE_RIGHT,EQUIPCELLTYPE_ADORN);

	m_pImage[0]=GetImage(2);
	m_pImage[1]=GetImage(3);
	m_pImage[2]=GetImage(4);

	GetButton(5)->SetButtonGroup(1);
	GetButton(6)->SetButtonGroup(1);	
	GetButton(8)->SetButtonGroup(1);

	if( m_bMainUser )
	{
		GetButton(8)->SetVisible(false);
	}

	m_pTab = GetTab(7);
	m_pTab->AddItem("",NULL);
	m_pTab->AddItem("",NULL);	
	m_pTab->AddItem("",NULL);

	m_pTab->AddControl(0,(CGuiControl*)GetTable(100));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(101));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(102));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(103));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(104));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(105));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(106));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(107));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(108));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(109));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(110));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(111));
	m_pTab->AddControl(0,(CGuiControl*)GetTable(112));

	m_pTab->AddControl(0,(CGuiControl*)GetImage(2));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(3));
	m_pTab->AddControl(0,(CGuiControl*)GetImage(4));

	m_pTab->AddControl(1,(CGuiControl*)GetButton(200));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(201));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(202));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(203));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(204));


	m_pTab->AddControl(1,(CGuiControl*)GetButton(20));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(21));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(22));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(23));
	m_pTab->AddControl(1,(CGuiControl*)GetButton(24));

	m_pTab->AddControl(2,GetControl(300));
	m_pTab->AddControl(2,GetControl(301));
	m_pTab->AddControl(2,GetControl(302));
	m_pTab->AddControl(2,GetControl(303));
	m_pTab->AddControl(2,GetControl(304));
	m_pTab->AddControl(2,GetControl(305));
	m_pTab->AddControl(2,GetControl(306));
	m_pTab->AddControl(2,GetControl(307));
	m_pTab->AddControl(2,GetControl(308));
	m_pTab->AddControl(2,GetControl(309));
	m_pTab->AddControl(2,GetControl(314));
	m_pTab->AddControl(2,GetControl(315));
	m_pTab->AddControl(2,GetControl(316));

	m_pTab->SetVisible(false);

	OnGuiEvent(EVENT_BUTTON_CLICKED,5,GetButton(5));

	SetTitleList();

	FUNCTION_END;
}

bool CGuiUserInfo::IsTonic()
{
	FUNCTION_BEGIN;

	CRoleItem* pItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_LEFT );
	if ( pItem && (pItem->GetObjectBase()->dwType!=ItemType_Tonic) )
		pItem = NULL;

	if ( NULL == pItem )
	{
		pItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_RIGHT );
		if ( pItem && (pItem->GetObjectBase()->dwType!=ItemType_Tonic) )
			pItem = NULL;
	}

	return pItem != NULL;

	/*CGuiTable* pTable = GetEquipTable(EQUIPCELLTYPE_ADORN);
	if ((pTable)&&(pTable->m_listItem.size()>0))
	{
		CGuiItemCell* pCell = pTable->m_listItem[0];
		if (pCell&&pCell->m_pItemData)
		{
			CRoleItem* pItem = (CRoleItem*)pCell->m_pItemData;
			if (pItem->GetObjectBase()->dwType==ItemType_Tonic)
			{
				return true;
			}
		}
	}
	return false;*/

	FUNCTION_END;
}



void CGuiUserInfo::SetTitleList()
{
	m_titleList.clear();
	m_equipList.clear();
	CIni ini;
	ini.Open("data\\datas.pak","datas\\userinfo.ini");

	char szTxt1[10]={0};
	char szTxt2[10]={0};

	for (int i=1;i<=2;i++)
	{
		for (int j=1;j<=26;j++)
		{
			std::vector<UINT, std::stack_allocator<UINT> > info;
			_parse_str_num(info,ini.GetString(_itoa(i,szTxt1,10),_itoa(j,szTxt2,10),"0,0,0,0,0").c_str());

			if (info.size()==5)
			{
				stTitle  st;
				st.key = i*100+j;
				st.rect.SetRect(info[0],info[1],info[2],info[3]);
				st.index = info[4];
				
				if (st.rect!=stRectI(0,0,0,0))
					m_titleList.push_back(st);
			}
		}
	}

	for (int i=1;i<=3;i++)
	{
		std::vector<UINT, std::stack_allocator<UINT> > info;
		_parse_str_num(info,ini.GetString("3",_itoa(i,szTxt2,10),"0,0,0,0,0").c_str());

		if (info.size()==5)
		{
			stTitle  st;
			st.key = i;
			st.rect.SetRect(info[0],info[1],info[2],info[3]);
			st.index = info[4];

			if (st.rect!=stRectI(0,0,0,0))
				m_equipList.push_back(st);
		}
	}
}

#define  AddShowTitle(str,total,base,unit) \
	{\
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));\
		char szTxt[16]={0};\
		m_ToolTips.AddText(str);\
		m_ToolTips.AddText(" = ");\
		m_ToolTips.AddText(_itoa(base,szTxt,10));\
		m_ToolTips.AddText(unit);\
		if (total!=base){\
			m_ToolTips.SetCurColor(GETSHOWCOLOR(total,base));\
			m_ToolTips.AddText(total>base?" + ":" - ");\
			m_ToolTips.AddText(_itoa(abs((int)(total-base)),szTxt,10)); \
			m_ToolTips.AddText(unit);\
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));}\
	}\

void CGuiUserInfo::TipsRender()
{
	static int key = 0;

	if (m_bMainUser==false)
		return;

	if (GetGuiManager()->GetMouseOverDlg()!=this)
		return;

	stPointI pt(Engine_GetCursor()->GetPosition());
	pt -= GetLocation();

	for (int i=0;i<m_titleList.size();i++)
	{
		if (m_titleList[i].index!=m_pTab->GetCurItem())
			continue;

		if (m_titleList[i].rect.PtInRect(pt))
		{
			//if (key!=m_titleList[i].key)
			{
				m_ToolTips.Clear();
				m_ToolTips.SetBorderColor(0);
				m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
				m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));

				switch(m_titleList[i].key) {
				case 101:
				case 201:
					m_ToolTips.AddText("当前尚未分配的属性点数");
					break;
				case 102:
				case 202:
					//m_ToolTips.AddText("增加力量可增加物理攻击力\n");
					AddShowTitle("当前力量",m_UserData.pMainUserData->wdStr,m_UserData.pMainUserData->wdStdStr,"");
					break;
				case 103:
				case 203:
					//m_ToolTips.AddText("增加智力可提高魔法攻击力\n");
					AddShowTitle("当前智力",m_UserData.pMainUserData->wdInt,m_UserData.pMainUserData->wdStdInt,"");
					break;
				case 104:
				case 204:
					//m_ToolTips.AddText("增加敏捷可提高物理防御力和爆击概率\n");
					AddShowTitle("当前敏捷",m_UserData.pMainUserData->wdDex,m_UserData.pMainUserData->wdStdDex,"");
					break;
				case 105:
				case 205:
					//m_ToolTips.AddText("增加精神可提高魔法防御力和魔法值\n");
					AddShowTitle("当前精神",m_UserData.pMainUserData->wdMen,m_UserData.pMainUserData->wdStdMen,"");
					break;
				case 106:
				case 206:
					//m_ToolTips.AddText("增加体质可提高生命值\n");
					AddShowTitle("当前体质",m_UserData.pMainUserData->wdCon,m_UserData.pMainUserData->wdStdCon,"");
					break;
				case 107:
				case 114:
				case 207:
				case 214:
					//m_ToolTips.AddText("增加物理攻击力可以提高物理攻击能力");
					AddShowTitle("最小物理攻击力",m_UserData.pMainUserData->pdamage,m_UserData.pMainUserData->stdpdamage,"");
					AddShowTitle("\n最大物理攻击力",m_UserData.pMainUserData->maxpdamage,m_UserData.pMainUserData->stdpdamage,"");
					break;
				case 108:
				case 115:
				case 208:
				case 215:
					//m_ToolTips.AddText("增加魔法攻击力可以提高魔法攻击能力");
					AddShowTitle("最小魔法攻击力",m_UserData.pMainUserData->mdamage,m_UserData.pMainUserData->stdmdamage,"");
					AddShowTitle("\n最大魔法攻击力",m_UserData.pMainUserData->maxmdamage,m_UserData.pMainUserData->stdmdamage,"");
					break;
				case 109:
				case 116:
				case 209:
				case 216:
					//m_ToolTips.AddText("增加物理防御力可以提高抵御物理攻击能力");
					AddShowTitle("物理防御力",m_UserData.pMainUserData->pdefence,m_UserData.pMainUserData->stdpdefence,"");
					break;
				case 110:
				case 117:
				case 210:
				case 217:
					//m_ToolTips.AddText("增加魔法防御力可以提高抵御魔法攻击能力");
					AddShowTitle("魔法防御力",m_UserData.pMainUserData->mdefence,m_UserData.pMainUserData->stdmdefence,"");
					break;
				case 113:
				case 213:
					m_ToolTips.AddText("增加爆击可以提高攻击出现爆击几率");
					//AddShowTitle("\n爆击率",m_UserData.pMainUserData->bang,m_UserData.pMainUserData->stdbang,"%");
					break;
				case 118:
				case 218:
					m_ToolTips.AddText("对无属性或相克属性的角色和怪物有攻击加成");
					break;
				case 119:
				case 219:
					m_ToolTips.AddText("称号等级决定攻击加成的系数");
					break;
				case 120:
				case 220:
					m_ToolTips.AddText("国家评定人物官位的标准值");
					break;
				case 121:
				case 221:
					m_ToolTips.AddText("可影响人物称号的值");
					break;
				case 122:
				case 222:
					m_ToolTips.AddText("可以影响打造成功率,攻击命中率等的属性");
					break;
				case 123:
					m_ToolTips.AddText("荣誉值可换取双倍经验道具或洗PK值");
					break;
				case 223:
					m_ToolTips.AddText(avar("当前荣誉值 %u ,最大荣誉值 %u ",m_UserData.pMainUserData->honor,m_UserData.pMainUserData->maxhonor));
					break;
				case 125:
				case 225:
					//m_ToolTips.AddText("国家评定文官的标准值");
					m_ToolTips.AddText("您的角色等级在当前服务器所处的名次\n5000名以后不参与排名");
					break;
				case 126:
				case 226:
					//m_ToolTips.AddText(avar("您当前的功勋值为 %d ",m_UserData.pMapUserData->exploit));
					return;
					break;
				case 124:
				case 224:
					{
						DWORD goodness = m_UserData.pMapUserData->goodness & GOODNESS_ATT ? 0 : m_UserData.pMapUserData->goodness;
						DWORD dwNameColor = GetNameColor(goodness);
						if (dwNameColor==-1)
							m_ToolTips.AddText("当前角色的善恶程度");
						else
						{
							m_ToolTips.AddText("您由");
							m_ToolTips.SetCurColor(dwNameColor);
							m_ToolTips.AddText(avar("[%s]",GetPkModeName(goodness))); 
							m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
							m_ToolTips.AddText(avar("恢复至[普通]还剩%d分钟",abs((short)(0xffff&goodness))));
						}
					}
					break;
				}

				m_ToolTips.Resize();
			}

			stRectI rect = m_titleList[i].rect;
			rect.OffsetRect(GetX(),GetY());

			m_ToolTips.RenderTopLeftPrefer(rect,GetDevice()->GetWidth(),GetDevice()->GetHeight());
			return;
		}
	}
}

void CGuiUserInfo::RefreshGoodnessOnXiXinGeMianState()
{
	DWORD goodness = m_UserData.pMapUserData->goodness & GOODNESS_ATT ? 0 : m_UserData.pMapUserData->goodness;
	if ( goodness == 0 )
		return;

	const int cStep = 60 * 1000;
	int nUnit = 1;
	static DWORD tLastTime = xtimeGetTime() - cStep;
	bool bXiXinGeMianState = false;
	if ( m_bMainUser )
	{
		if ( GetScene()->GetMainCharacter() && isset_state( GetScene()->GetMainCharacter()->GetState(), USTATE_XIXINGEMIA ) )
			bXiXinGeMianState = true;			
	}
	else
	{
		if ( GetScene()->m_pInterestNpc && GetScene()->m_pInterestNpc->IsCharacter() && isset_state( GetScene()->m_pInterestNpc->GetState(), USTATE_XIXINGEMIA ) )
			bXiXinGeMianState = true;	
	}

	if ( bXiXinGeMianState )
		nUnit = 3;

	//Reduce 
	if ( (xtimeGetTime() - tLastTime) >= cStep )
	{
		if ( m_UserData.pMapUserData->goodness >= nUnit )
			m_UserData.pMapUserData->goodness -= nUnit;
		else
			m_UserData.pMapUserData->goodness = 0;
		tLastTime = xtimeGetTime();
	}
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
bool CGuiUserInfo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	FUNCTION_BEGIN;

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			if (m_bMainUser)
			{
				stPointI pt(Engine_GetCursor()->GetPosition());
				pt -= GetLocation();

				for (int i=0;i<m_equipList.size();i++)
				{
					if (m_equipList[i].index!=m_pTab->GetCurItem())
						continue;

					if (m_equipList[i].rect.PtInRect(pt))
					{
						switch(m_equipList[i].key) {
						case 1:  //身体装备
							{
								CRoleItem* pRoleItem = NULL;
								if (GetGuiFloatItem()->GetItem())
									pRoleItem = (CRoleItem*)(GetGuiFloatItem()->GetItem()->m_pItemData);
								if (pRoleItem==NULL)								
									pRoleItem = GetShowClothes();
								if (pRoleItem)
								{
									switch(pRoleItem->GetLocation().y) {
									case EQUIPCELLTYPE_BODY:
										ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_BODY),WM_LBUTTONDOWN,stPointI(350,250),0,0);
										break;
									case EQUIPCELLTYPE_ADORN:
										{
											switch( pRoleItem->GetLocation().x ){
											case EQUIPCELLTYPE_LEFT:
												ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_LEFT),WM_LBUTTONDOWN,stPointI(60,175),0,0);
												break;
											case EQUIPCELLTYPE_RIGHT:
												ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_RIGHT),WM_LBUTTONDOWN,stPointI(320,140),0,0);
												break;
											}	
										}										
										break;
									default:
										if (pRoleItem->GetItemType()==ItemType_FashionBody)
										{
											switch( pRoleItem->GetLocation().x ){
											case EQUIPCELLTYPE_LEFT:
												ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_LEFT),WM_LBUTTONDOWN,stPointI(60,175),0,0);
												break;
											case EQUIPCELLTYPE_RIGHT:
												ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_ADORN,EQUIPCELLTYPE_RIGHT),WM_LBUTTONDOWN,stPointI(320,140),0,0);
												break;
													}
										}
										else
											ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_BODY),WM_LBUTTONDOWN,stPointI(350,250),0,0);
										break;
									}
								}
							}
							break;
						case 2:  //右手装备
							ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_HANDR),WM_LBUTTONDOWN,stPointI(60,210),0,0);
							break;
						case 3:  //左手装备
							ItemTableEvent(GetEquipTable(EQUIPCELLTYPE_HANDL),WM_LBUTTONDOWN,stPointI(60,250),0,0);
							break;
						}
					}
				}
			}
		}
		break;
	}
	return CGuiDialog::MsgProc(hWnd,uMsg,wParam,lParam);

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
void CGuiUserInfo::InitCustomElements()
{
	FUNCTION_BEGIN;

	//-------------------------------------
	// CGuiButton - Button 小的
	//-------------------------------------
	const char* szGrpFileName = "data\\interfaces.gl";
	stResourceLocation rl;
	rl.SetFileName(GetGuiGraphicPackName());
	rl.group = 3;
	rl.frame = 1;
	
	
	stRectI rcTexture;
	CGuiElement Element;

	rcTexture.SetRect( 0, 0, 19, 20 );
	Element.SetTexture( &rl, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(0, 0, 0, 0);
	Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(255, 255, 255, 255);

	Element.FontColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.FontColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(0, 0, 0, 0);
	Element.FontColor.States[ GUI_STATE_MOUSEOVER ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.FontColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	// Assign the Element
	SetCustomElement( GUI_CONTROL_BUTTON, 0, &Element );

	rcTexture.SetRect( 19, 0, 19 * 2, 20 );
	Element.SetTexture( &rl, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ GUI_STATE_NORMAL ] = 0;
	Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = 0;
	Element.TextureColor.States[ GUI_STATE_FOCUS ] = 0;

	Element.FontColor.States[ GUI_STATE_NORMAL ] = 0;
	Element.FontColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.FontColor.States[ GUI_STATE_MOUSEOVER ] = 0;
	Element.FontColor.States[ GUI_STATE_FOCUS ] = 0;
	// Assign the Element
	SetCustomElement( GUI_CONTROL_BUTTON, 1, &Element );

	rcTexture.SetRect( 19 * 2, 0, 19 * 3, 20 );
	Element.SetTexture( &rl, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ GUI_STATE_NORMAL ] = 0;
	Element.TextureColor.States[ GUI_STATE_PRESSED ] = 0;
	Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = 0;
	Element.TextureColor.States[ GUI_STATE_FOCUS ] = 0;
	Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(255, 255, 255, 255);

	Element.FontColor.States[ GUI_STATE_NORMAL ] = 0;
	Element.FontColor.States[ GUI_STATE_PRESSED ] = 0;
	Element.FontColor.States[ GUI_STATE_MOUSEOVER ] = 0;
	Element.FontColor.States[ GUI_STATE_FOCUS ] = 0;
	Element.FontColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	// Assign the Element
	SetCustomElement( GUI_CONTROL_BUTTON, 2, &Element );
	

	FUNCTION_END;
}
*/

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param fElapsedTime : 描述
 * \return 返回值的描述
 */
HRESULT CGuiUserInfo::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	bool bShowBody = m_bShowBody;

	ControlImage(EQUIPCELLTYPE_BODY);
	ControlImage(EQUIPCELLTYPE_HANDL);
	ControlImage(EQUIPCELLTYPE_HANDR);

	HRESULT hr = CGuiDialog::OnRender(fElapsedTime);


	int iOldFont = GetDevice()->SetFont(1);

	// 名字
	stRectI rect(113,101,351,117);
	rect.OffsetRect(m_x,m_y);
	GetDevice()->DrawString(m_UserData.pMapUserData->name,rect,ColorBlend(GetNameColor(m_UserData.pMapUserData->goodness),0xffffffff),DT_VCENTER|DT_CENTER);

	switch(m_pTab->GetCurItem())
	{
	case 0:
		{
			ShowUserInfoText1();

			m_pImage[0]->Render(fElapsedTime);
			
			BodyRender(GetLocation());

			m_pImage[1]->Render(fElapsedTime);
			m_pImage[2]->Render(fElapsedTime);

			ShowEquipByName();

			ShowActiveFiveEffect();
		}
		break;
	case 1:
		{
			ShowUserInfoText2();
		}
		break;
	}

	TipsRender();

	GetDevice()->SetFont(iOldFont);
	
	m_bShowBody = bShowBody;

	RefreshGoodnessOnXiXinGeMianState();

	return hr;

	FUNCTION_END;
}

void CGuiUserInfo::ShowActiveFiveEffect()
{
	if ( m_bMainUser )
		return;

	if ( m_listActiveItem.empty() )
		return;

	for( std::vector<CRoleItem*>::iterator itor=m_listActiveItem.begin();itor!=m_listActiveItem.end();++itor )
	{
		CRoleItem* pItem = (*itor);

		if ( pItem && (pItem->GetObject()->fiveset[0] == 0) )
			continue;

		IBitmaps* pBmp=NULL;

		stRectI tableRect = stRectI(-1000,-1000,0,0);
		CGuiTable* pTable = GetEquipTable( pItem->GetLocation().y, pItem->GetLocation().x );
		if ( pTable )
		{
			tableRect = pTable->m_itemRect;
			pTable->m_pDialog->ClientToScreen( &tableRect );
		}
			

		switch(pItem->GetObject()->fivetype)
		{
		case FIVE_METAL:
			{
				pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,100)));
				if (pBmp)
				{
					pBmp->RenderAni(tableRect.left-112,tableRect.top-112,xtimeGetTime(),-1,Blend_Null);
				}
			}
			break;
		case FIVE_WOOD:
			{
				pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,101)));
				if (pBmp)
				{
					pBmp->RenderAni(tableRect.left-112,tableRect.top-112,xtimeGetTime(),-1,Blend_Null);
				}
			}
			break;
		case FIVE_SOIL:
			{
				pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,102)));
				if (pBmp)
				{
					pBmp->RenderAni(tableRect.left-112,tableRect.top-112,xtimeGetTime(),-1,Blend_Null);
				}
			}
			break;
		case FIVE_WATER:
			{
				pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,103)));
				if (pBmp)
				{
					pBmp->RenderAni(tableRect.left-112,tableRect.top-112,xtimeGetTime(),-1,Blend_Null);
				}
			}
			break;
		case FIVE_FIRE:
			{
				pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,104)));
				if (pBmp)
				{
					pBmp->RenderAni(tableRect.left-112,tableRect.top-112,xtimeGetTime(),-1,Blend_Null);
				}
			}
			break;
		}
	}
}

#define Tips_AddNum(szFormat,nNum)\
	if(nNum){\
	_snprintf(szTemp,sizeof(szTemp),szFormat,nNum);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,0);\
	}\

void CGuiUserInfo::ShowEquipByName()
{
	IBitmaps* pBmp=NULL;

	int frame = -1;
	if ( m_equipByMakerType >= EquipByMaker_T_WONDER_BLUE )
	{
		switch( m_equipByMakerType )
		{
		case EquipByMaker_T_WONDER_BLUE:
			frame = 201;
			break;
		case EquipByMaker_T_WONDER_GOLD:
			frame = 200;
			break;
		case EquipByMaker_T_WONDER_HOLY:
			frame = 202;
			break;
		}
	}

	if ( frame > -1 )
	{
		//Add special effect
		pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,frame)));
		if ( pBmp )
		{
			pBmp->RenderAni(m_x,m_y,xtimeGetTime(),-1,Blend_Null);
		}
	}


	/*
	if (m_equipByName.numHoly>=10)
	{
		pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,202)));
		if (pBmp)
		{
			pBmp->RenderAni(m_x,m_y,xtimeGetTime(),-1,Blend_Null);
		}
	}
	else if(m_equipByName.numHoly>=6)
	{

	}
	else if(m_equipByName.numGlod>=10)
	{
		pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,200)));
		if (pBmp)
		{
			pBmp->RenderAni(m_x,m_y,xtimeGetTime(),-1,Blend_Null);
		}

	}
	else if(m_equipByName.numGlod>=6)
	{

	}
	else if(m_equipByName.numBlue>=10)
	{
		pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,201)));
		if (pBmp)
		{
			pBmp->RenderAni(m_x,m_y,xtimeGetTime(),-1,Blend_Null);
		}

	}
	else if(m_equipByName.numBlue>=6)
	{

	}  */

	//if (pBmp!=NULL)
	if( m_equipByMakerType > EquipByMaker_T_NONE )
	{
		if (GetGuiManager()->GetMouseOverDlg()==this)
		{
			stPointI pt(Engine_GetCursor()->GetPosition());
			pt -= GetLocation();

			for (int i=0;i<m_equipList.size();i++)
			{
				if (m_equipList[i].index!=m_pTab->GetCurItem())
					continue;

				if (m_equipList[i].rect.PtInRect(pt))
				{
					switch(m_equipList[i].key) {
					case 1:  //身体装备
						{
//#if defined _DEBUG || defined RELEASE_TEST
							if ( (m_equipByMakerType > EquipByMaker_T_NONE) && (m_equipByMakerType <= EquipByMaker_T_WONDER_HOLY) )
							{
								stRectI rect = m_equipList[i].rect;
								rect.OffsetRect(GetX(),GetY());

								m_ToolTips.Clear();
								m_ToolTips.SetBorderColor(0);
								m_ToolTips.SetAlignType(GUI_ALIGN_LEFT);
								m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
								m_ToolTips.SetCurColor(COLOR_ARGB(255,244,154,193));

								static const int value[6][4] = { {2,4,4,4},{2,4,4,4},{2,4,4,4},{5,8,8,8},{5,8,8,8},{5,8,8,8} };
								static const char* strSetName[] = { "地之侠义", "地之神佑", "地之圣威", "天之英雄", "天之屠魔", "天之圣尊" };

								char szTemp[512];
								m_ToolTips.AddText( strSetName[m_equipByMakerType] );
								Tips_AddNum( "\n增加攻击 %u%% ", value[m_equipByMakerType][0] );
								Tips_AddNum( "\n增加防御 %u%% ", value[m_equipByMakerType][1] );
								Tips_AddNum( "\n增加生命值上限 %u%% ", value[m_equipByMakerType][2] );
								Tips_AddNum( "\n增加法术值上限 %u%% ", value[m_equipByMakerType][3] );
								m_ToolTips.Resize();

								m_ToolTips.RenderTopLeftPrefer(rect,GetDevice()->GetWidth(),GetDevice()->GetHeight());

							}
//#endif
						}
						break;
					case 2:  //右手装备
						{

						}
						break;
					case 3:  //左手装备
						{

						}
						break;
					}
					break;
				}
			}
		}
	}
}


void CGuiUserInfo::ShowUserInfoText2()
{
	stPointI ptText ;
	int dy = 0;

	if (m_bMainUser==false)
		return;
	

	// 等级 
	stRectI rect(113,130,351,144);
	rect.OffsetRect(m_x,m_y);
	GetDevice()->DrawString(avar("%u级",m_UserData.pMainUserData->level ),rect,0xffffffff,DT_VCENTER|DT_CENTER);


	GetDevice()->SetFont(0);

	
	ptText = stPointI(124,167) + GetLocation();
	
	char szCountryName[MAX_NAMESIZE] = {0};
	CCountry::Instance()->GetCountryName(m_UserData.pMapUserData->country,szCountryName);
	GetDevice()->DrawString(avar("【%s】",szCountryName),ptText);

	dy=24;
	ptText.y += dy;
	GetDevice()->DrawString(GetGameGuiManager()->m_guiWorldConn->m_TongInfo.unionName,ptText);

	ptText.y += dy;
	GetDevice()->DrawString(GetGameGuiManager()->m_guiWorldConn->m_SeptInfo.septName,ptText);

	ptText.y += dy;
	//m_UserData.pMapUserData->exploit = 1000;
	//sprintf(m_UserData.pMapUserData->caption,"城主");
	GetDevice()->DrawString(GetKnightName(m_UserData.pMapUserData->exploit),ptText);

	ptText.y += dy;
	/*m_UserData.pMapUserData->grace = 1000;*/
	GetDevice()->DrawString(GetCivilianName(m_UserData.pMapUserData->grace),ptText);
	
	//m_UserData.pMapUserData->star = 0;

	ptText.y += dy;
	DWORD goodness = m_UserData.pMapUserData->goodness & GOODNESS_ATT ? 0 : m_UserData.pMapUserData->goodness;
	GetDevice()->DrawString(GetPkModeName(goodness),ptText,GetNameColor(goodness));


	

	ptText = stPointI(290,167) + GetLocation();

	//GetDevice()->DrawString(avar("%u",640 + (640 - m_UserData.pMapUserData->movespeed)),ptText);
	if (m_UserData.pMainUserData->fivetype<FIVE_NONE)
		GetDevice()->DrawString(c_szFiveName[m_UserData.pMainUserData->fivetype],ptText);

	dy=24;
	ptText.y += dy;
	if (m_UserData.pMainUserData->fivetype<FIVE_NONE)
		GetDevice()->DrawString(avar("%s %d级",c_szFiveToOtherName[m_UserData.pMainUserData->fivetype],m_UserData.pMainUserData->fivepoint),ptText);

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMapUserData->exploit),ptText);

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMapUserData->grace),ptText);

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->lucky),ptText);

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u / %u",m_UserData.pMainUserData->honor,m_UserData.pMainUserData->maxhonor),ptText);




	ptText = stPointI(145,333) + GetLocation();

	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->points),ptText,0xffffffff );

	dy=26;
	//ptText.x=138;
	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->wdStr),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->wdStr,m_UserData.pMainUserData->wdStdStr));

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->wdInt),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->wdInt,m_UserData.pMainUserData->wdStdInt));

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->wdDex),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->wdDex,m_UserData.pMainUserData->wdStdDex));

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->wdMen),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->wdMen,m_UserData.pMainUserData->wdStdMen));

	ptText.y += dy;
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->wdCon),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->wdCon,m_UserData.pMainUserData->wdStdCon));






	ptText = stPointI(295,333) + GetLocation();

	if ((m_LevelNum!=0)&&(m_LevelNum!=(WORD)-1))
		GetDevice()->DrawString(avar("%u",m_LevelNum),ptText,0xffffffff );

	dy = 26;
	ptText.x -= 30;
	ptText.y += dy;
	// 武力(物理攻击)
	GetDevice()->DrawString(avar("%u - %u",m_UserData.pMainUserData->pdamage,m_UserData.pMainUserData->maxpdamage),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->pdamage,m_UserData.pMainUserData->stdpdamage));
	ptText.y += dy;
	// 魔法攻击
	GetDevice()->DrawString(avar("%u - %u",m_UserData.pMainUserData->mdamage,m_UserData.pMainUserData->maxmdamage),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->mdamage,m_UserData.pMainUserData->stdmdamage));
	ptText.y += dy;

	// 物理防御
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->pdefence),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->pdefence,m_UserData.pMainUserData->stdpdefence));
	ptText.y += dy;
	// 魔法防御
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->mdefence),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->mdefence,m_UserData.pMainUserData->stdmdefence));
	ptText.y += dy;
	// 重击
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->bang*100),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->bang,m_UserData.pMainUserData->stdbang) );


	/*
	// 生命值
	GetDevice()->DrawString(avar("%u/%u",pMainChar->GetHP(),pMainChar->GetMaxHP()),ptText);
	ptText.y += dy;
	// 法术值
	GetDevice()->DrawString(avar("%u/%u",pMainChar->GetMP(),pMainChar->GetMaxMP()),ptText);
	ptText.y += dy;
	// 体力值
	GetDevice()->DrawString(avar("%u/%u",pMainChar->GetSP(),pMainChar->GetMaxSP()),ptText);
	ptText.y += dy;

	// 命中率
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->attackrating),ptText);
	ptText.y += 12;
	// 躲避率
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->attackdodge),ptText );
	ptText.y += 12;
	*/

	
	
}


void CGuiUserInfo::ShowUserInfoText1()
{
	stPointI ptText ;

	
	if (m_bMainUser==false)
		return;

	GetDevice()->SetFont(0);
	ptText = stPointI(100,454) + GetLocation();

	// 武力(物理攻击)
	GetDevice()->DrawString(avar("%u - %u",m_UserData.pMainUserData->pdamage,m_UserData.pMainUserData->maxpdamage),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->pdamage,m_UserData.pMainUserData->stdpdamage));
	ptText.y += 16;
	// 魔法攻击
	GetDevice()->DrawString(avar("%u - %u",m_UserData.pMainUserData->mdamage,m_UserData.pMainUserData->maxmdamage),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->mdamage,m_UserData.pMainUserData->stdmdamage));


	ptText = stPointI(275,454) + GetLocation();
	
	// 物理防御
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->pdefence),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->pdefence,m_UserData.pMainUserData->stdpdefence));
	ptText.y += 16;
	// 魔法防御
	GetDevice()->DrawString(avar("%u",m_UserData.pMainUserData->mdefence),ptText,GETSHOWCOLOR(m_UserData.pMainUserData->mdefence,m_UserData.pMainUserData->stdmdefence));

}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pTable : 描述
 * \param ptRender : 描述
 * \return 返回值的描述
 */
void CGuiUserInfo::BodyRender(stPointI ptRender)
{
	FUNCTION_BEGIN;

	static stPointI pos(0,0);
	
	//if(pTable->IsVisible())
	{
		//if(!pTable->GetItemCell(0,0))
		if(m_bShowBody)
		{
			for(size_t i=0;i<count_of(m_aniBody);++i)
			{
				{
					if(i == 1) 
						m_aniBody[i].SetColor(m_UserData.pMapUserData->dwBodyColorSystem);
					else if(i==2)
						m_aniBody[i].SetColor(m_UserData.pMapUserData->dwBodyColorCustom | 0xff000000);
					else m_aniBody[i].SetColor(-1);
					m_aniBody[i].SetBlendType(Blend_Null);
				}
				stPointI pt = pos;
				pt.x += ptRender.x;
				pt.y += ptRender.y;
				//m_aniBody[i].SetColor();
				m_aniBody[i].Render(pt.x,pt.y);
			}
		}
	}

	if(m_UserData.pMapUserData->sculpt.dwHairID != 0)
	{
		const stCharTypeInfo& info = GetCharTypeInfo(m_UserData.pMapUserData->type);
		HairStyle_t* p = GetHairStyle(m_UserData.pMapUserData->sculpt.dwHairID);
		if(p)
		{
			stResourceLocation rl("data\\items.gl",info.equipHairPic,p->dwEquipPicID);
			IBitmaps* pBmps = GetDevice()->FindBitmaps(&rl);
			if(pBmps)
			{
				stPointI pt = pos;
				pt.x += ptRender.x;
				pt.y += ptRender.y;
				pBmps->RenderAni(pt.x,pt.y,xtimeGetTime(),m_UserData.pMapUserData->dwHairRGB | 0xff000000);
			}
		}
	}

	FUNCTION_END;
}

bool CGuiUserInfo::IsActiveItem(CRoleItem* pItem)
{
	for (int i=0;i<m_listActiveItem.size();i++)
	{
		if ((m_listActiveItem[i]==pItem)&&IsShowActiveItem(pItem))
			return true;
	}

	return false;
}


void CGuiUserInfo::SetEquipByName( CRoleItem* pItem, bool bMainCharacter /* = true */ )
{
	if (pItem==NULL)
		return;

	CGuiFloatItem* pFloatItem = GetGuiManager()->GetFloatItem();
	
	if ((pItem->GetItemType()==ItemType_Shield)||(pItem->GetItemType()==ItemType_Arrow))
		return;

	if ( NULL == m_UserData.pMapUserData )
		return;
	if ( strlen(pItem->GetObject()->maker) > 0 )
	{
		std::vector<stEquipByMaker>::iterator itor;
		for( itor = m_vEquipByMaker.begin();itor!=m_vEquipByMaker.end();itor++ )
		{
			if ( strcmp((*itor).strMakerName, pItem->GetObject()->maker) == 0 )
				break;
		}

		if ( itor == m_vEquipByMaker.end() )		//Add as new
		{
			stEquipByMaker byMaker;
			byMaker.Init();
			sprintf( byMaker.strMakerName, pItem->GetObject()->maker );
			m_vEquipByMaker.push_back( byMaker );
			itor = m_vEquipByMaker.end() -1;			
		}

		//Set set num
		if ( pItem->GetObject()->bind == 1 )
		{
			if ( pItem->GetObject()->kind & 0x4 )
			{
				(*itor).numSetList.numHoly++ ;
			}
			else if ( pItem->GetObject()->kind & 0x2 )
			{
				(*itor).numSetList.numGlod++ ;
			}
			else if ( pItem->GetObject()->kind & 0x1 )
			{
				(*itor).numSetList.numBlue++ ;
			}
		}

		enmEuipByMakerType typeWonder = EquipByMaker_T_NONE;
		enmEuipByMakerType typeBug = EquipByMaker_T_NONE;
		for( itor = m_vEquipByMaker.begin();itor!=m_vEquipByMaker.end();itor++ )
		{
			bool bGetNewSet = false;
			if ( (*itor).numSetList.numHoly >= 10)
			{
				if ( typeWonder < EquipByMaker_T_WONDER_HOLY )
				{
					typeWonder = EquipByMaker_T_WONDER_HOLY;
					bGetNewSet = true;
				}
			}
			else if ( (*itor).numSetList.numHoly >= 6 )
			{
				if ( typeBug < EquipByMaker_T_BUG_HOLY )
				{
					typeBug = EquipByMaker_T_BUG_HOLY;
					bGetNewSet = true;
				}
			}
			else if ( (*itor).numSetList.numGlod >= 10)
			{
				if ( typeWonder < EquipByMaker_T_WONDER_GOLD )
				{
					typeWonder = EquipByMaker_T_WONDER_GOLD;
					bGetNewSet = true;
				}
			}
			else if ( (*itor).numSetList.numGlod >= 6 )
			{
				if ( typeBug < EquipByMaker_T_BUG_GOLD )
				{
					typeBug = EquipByMaker_T_BUG_GOLD;
					bGetNewSet = true;
				}
			}
			else if ( (*itor).numSetList.numBlue >= 10)
			{
				if ( typeWonder < EquipByMaker_T_WONDER_BLUE )
				{
					typeWonder = EquipByMaker_T_WONDER_BLUE;
					bGetNewSet = true;
				}
			}
			else if ( (*itor).numSetList.numBlue >= 6 )
			{
				if ( typeBug < EquipByMaker_T_BUG_BLUE )
				{
					typeBug = EquipByMaker_T_BUG_BLUE;
					bGetNewSet = true;
				}
			}

			//Remember set maker
			if ( bGetNewSet )
			{
				m_strSetMakerName[0] = 0L;
				strcpy( m_strSetMakerName, (*itor).strMakerName );
			}
		}

		if ( typeWonder > EquipByMaker_T_NONE )
			m_equipByMakerType = typeWonder;			
		else if ( typeBug > EquipByMaker_T_NONE )
			m_equipByMakerType = typeBug;
		else
			m_equipByMakerType = EquipByMaker_T_NONE;
	}

}

char* CGuiUserInfo::GetSetMakerName()
{
	if ( strlen( m_strSetMakerName ) <= 0 )
		return NULL;

	return m_strSetMakerName;
}

BYTE CGuiUserInfo::GetSetEquipType()
{
	switch( m_equipByMakerType )
	{
	case EquipByMaker_T_NONE:
		return 0;
		break;
	case EquipByMaker_T_BUG_BLUE:
	case EquipByMaker_T_WONDER_BLUE:
		return 1;
		break;
	case EquipByMaker_T_BUG_GOLD:
	case EquipByMaker_T_WONDER_GOLD:
		return 2;
		break;
	case EquipByMaker_T_BUG_HOLY:
	case EquipByMaker_T_WONDER_HOLY:
		return 4;
		break;
	}

	return 0;
}

void CGuiUserInfo::RefreshActiveItem()
{
	CRoleItem*  pItem = NULL;

	m_listActiveItem.clear();

	//Clear EquipByMaker list
	for( int i=0;i<m_vEquipByMaker.size();i++ )
	{
		m_vEquipByMaker[i].Init();
	}
	m_vEquipByMaker.clear();
	m_strSetMakerName[0] = 0L;
	
	stObjectLocation location;

	location.dwLocation = OBJECTCELLTYPE_EQUIP;
	location.dwTableID = 0;
	location.x = 0;
	location.y = EQUIPCELLTYPE_HELM;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}
	
	location.y = EQUIPCELLTYPE_BODY;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.y = EQUIPCELLTYPE_HANDR;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.y = EQUIPCELLTYPE_HANDL;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.y = EQUIPCELLTYPE_NECKLACE;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.y = EQUIPCELLTYPE_BELT;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.y = EQUIPCELLTYPE_SHOES;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.x = EQUIPCELLTYPE_LEFT;
	location.y = EQUIPCELLTYPE_ADORN;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.x = EQUIPCELLTYPE_RIGHT;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.x = EQUIPCELLTYPE_LEFT;
	location.y = EQUIPCELLTYPE_GLOVES;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.x = EQUIPCELLTYPE_RIGHT;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.x = EQUIPCELLTYPE_LEFT;
	location.y = EQUIPCELLTYPE_RING;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}

	location.x = EQUIPCELLTYPE_RIGHT;
	pItem = GetScene()->GetMainCharacter()->FindItemByLocation(location);
	if (pItem!=NULL)
	{
		ActiveItem(pItem);
		SetEquipByName(pItem);
	}
}

void CGuiUserInfo::InsertActiveItem(CRoleItem* pItem)
{
	if (IsActiveItem(pItem)==false)
	{
		m_listActiveItem.push_back(pItem);
	}
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param type : 描述
 * \return 返回值的描述
 */
void CGuiUserInfo::SetCharType(int type,void* pData)
{
	FUNCTION_BEGIN;

	const stCharTypeInfo& info = GetCharTypeInfo(type);


	//GetBackImage()->SetImage(stResourceLocation(GetGuiGraphicPackName(),12,info.background));

	stResourceLocation rl;
	rl.SetFileName(c_szItemGrpPack);
	rl.group = c_nGroupItemEquip;
	rl.frame = info.equippic;
	m_aniBody[0].Create(&rl);
	rl.group = c_nGroupItemEquipPattern;
	m_aniBody[1].Create(&rl);
	rl.group = c_nGroupItemEquipPattern2;
	m_aniBody[2].Create(&rl);
	
	SetUserData(pData);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGuiUserInfo::RefreshButtonState()
{
	FUNCTION_BEGIN;

//
	//以前这里有一个bug，当用别的页面退出再打开时，属性加点按钮就在不该出现的页面出现了
	int curTab = GetTab(7)->GetCurItem();
	CMainCharacter *pMainChar = GetScene()->GetMainCharacter();
	if(! pMainChar) return ;
	switch(curTab)
	{
	case 0://属性
		InitIcon();
		break;
	case 1:
		{
			static bool bInit = false;
			bool bEnable = pMainChar->GetPoint() > 0;
			for(size_t i=0;i<5;++i)
			{
				GetButton(IDC_ADDPOINT_BASE + i)->SetEnabled(bEnable);
				GetButton(IDC_ADDPOINT_BASE + i)->SetVisible(m_bMainUser);
			}

			GetButton(20)->SetVisible(m_bMainUser);
			GetButton(21)->SetVisible(m_bMainUser);
			GetButton(22)->SetVisible(m_bMainUser);
			GetButton(23)->SetVisible(m_bMainUser);
			GetButton(24)->SetVisible(m_bMainUser);

			for(size_t i=0;i<5;++i)
			{
				GetButton(IDC_ADDPOINT_BASE + i)->GetToolTips()->Clear();
				GetButton(IDC_ADDPOINT_BASE + i)->GetToolTips()->SetBkColor(COLOR_ARGB(128,0,0,0));
				GetButton(IDC_ADDPOINT_BASE + i)->GetToolTips()->SetCurColor(COLOR_ARGB(255,255,255,255));
				GetButton(IDC_ADDPOINT_BASE + i)->GetToolTips()->AddText(GetPropTxt(i));
				GetButton(IDC_ADDPOINT_BASE + i)->GetToolTips()->Resize();
			}
			GetButton(IDC_ADDPOINT_BASE + 1)->SetTipsShowType(GUI_ALIGN_LEFTBUTTOM);
			bInit = true;
			InitIcon();
		}
		break;
	}
//

	
	//if(pMainChar)
	//{
	//	/*

	//	for(size_t i=0;i<5;++i)
	//	{
	//		GetButton(IDC_FIVEPOINT_RECOVERY + i)->GetToolTips()->Clear();
	//	}
	//	
	//	char szText[MAX_PATH]={0};
	//
	//	// 金抵抗
	//	sprintf(szText,CURRFIVECONTEXT,"金",GetScene()->GetMainCharacter()->GetProperty()->metalresistance);
	//	GetButton(IDC_FIVEPOINT_RECOVERY + 0)->GetToolTips()->AddText(szText);

	//	// 木抵抗
	//	sprintf(szText,CURRFIVECONTEXT,"木",GetScene()->GetMainCharacter()->GetProperty()->woodresistance);
	//	GetButton(IDC_FIVEPOINT_RECOVERY + 1)->GetToolTips()->AddText(szText);

	//	// 水抵抗
	//	sprintf(szText,CURRFIVECONTEXT,"水",GetScene()->GetMainCharacter()->GetProperty()->waterresistance);
	//	GetButton(IDC_FIVEPOINT_RECOVERY + 2)->GetToolTips()->AddText(szText);


	//	// 火抵抗
	//	sprintf(szText,CURRFIVECONTEXT,"火",GetScene()->GetMainCharacter()->GetProperty()->fireresistance);
	//	GetButton(IDC_FIVEPOINT_RECOVERY + 3)->GetToolTips()->AddText(szText);


	//	// 土抵抗
	//	sprintf(szText,CURRFIVECONTEXT,"土",GetScene()->GetMainCharacter()->GetProperty()->soilresistance);
	//	GetButton(IDC_FIVEPOINT_RECOVERY + 4)->GetToolTips()->AddText(szText);

	//	for(size_t i=0;i<5;++i)
	//	{
	//		GetButton(IDC_FIVEPOINT_RECOVERY + i)->GetToolTips()->Resize();
	//	}

	//	*/
	//}

	FUNCTION_END;
}


void CGuiUserInfo::RefreshShowState()
{
	GetButton(20)->SetChecked(GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_COUNTRYNAME ));
	GetButton(21)->SetChecked(GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_UNIONNAME ));
	GetButton(22)->SetChecked(GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_SEPTNAME ));
	GetButton(23)->SetChecked(GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_OFFICENAME ));
	GetButton(24)->SetChecked(GetGameApplication()->bSystemSetted( USER_SETTING_SHOW_GRACENAME ));
}
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nEvent : 描述
 * \param nID : 描述
 * \param pControl : 描述
 * \return 返回值的描述
 */

CRoleItem* CGuiUserInfo::GetShowClothes()
{
	CRoleItem* pRoleItem = GetEquipItem(EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_LEFT);

	if ((pRoleItem)&&(pRoleItem->GetObjectBase()->dwType!=ItemType_FashionBody))
		pRoleItem = NULL;

	if ( pRoleItem == NULL )
	{
		pRoleItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_RIGHT );
		if ((pRoleItem)&&(pRoleItem->GetObjectBase()->dwType!=ItemType_FashionBody))
			pRoleItem = NULL;
	}

	if (pRoleItem == NULL)
		pRoleItem = GetEquipItem(EQUIPCELLTYPE_BODY);

	return  pRoleItem;
}


bool CGuiUserInfo::IsAdornObject(DWORD dwObjectID)
{
	CRoleItem* pRoleItem = GetEquipItem(EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_LEFT);

	if ((pRoleItem)&&(pRoleItem->GetObjectBase()->dwID!=dwObjectID))
		pRoleItem = NULL;
	
	if ( pRoleItem == NULL )
	{
		pRoleItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_RIGHT );
		if ((pRoleItem)&&(pRoleItem->GetObjectBase()->dwID!=dwObjectID))
			pRoleItem = NULL;
	}

	return pRoleItem != NULL;
}

bool CGuiUserInfo::IsAdornObjectByType(DWORD  dwType)
{
	CRoleItem* pRoleItem = GetEquipItem(EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_LEFT);

	if ((pRoleItem)&&(pRoleItem->GetObjectBase()->dwType != dwType))
		pRoleItem = NULL;

	if ( pRoleItem == NULL )
	{
		pRoleItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_RIGHT );
		if ((pRoleItem)&&(pRoleItem->GetObjectBase()->dwType != dwType))
			pRoleItem = NULL;
	}

	return pRoleItem != NULL;
}


bool CGuiUserInfo::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nID) {
			case 5:
				m_pTab->SetCurItem(0);
				GetButton(5)->SetChecked(true);
				SetBackImage(stResourceLocation("data\\interfaces.gl",3,0));
				RefreshButtonState();
				break;
			case 6:
				m_pTab->SetCurItem(1);
				GetButton(6)->SetChecked(true);
				SetBackImage(stResourceLocation("data\\interfaces.gl",3,57));
				RefreshButtonState();
				break;
			case 8:
				{
					m_pTab->SetCurItem(2);
					GetButton(8)->SetChecked(true);
					SetBackImage(stResourceLocation("data\\interfaces.gl",25,0));
				}
				break;
			case 20:
				GetButton(nID)->SetChecked(!(GetButton(nID)->GetChecked()));
				if(GetButton(nID)->GetChecked()) 
					GetGameApplication()->SetSystemSetOnServer(USER_SETTING_SHOW_COUNTRYNAME);
				else 
					GetGameApplication()->ClearSystemSetOnServer(USER_SETTING_SHOW_COUNTRYNAME);
				break;
			case 21:
				GetButton(nID)->SetChecked(!(GetButton(nID)->GetChecked()));
				if(GetButton(nID)->GetChecked())
					GetGameApplication()->SetSystemSetOnServer(USER_SETTING_SHOW_UNIONNAME);
				else 
					GetGameApplication()->ClearSystemSetOnServer(USER_SETTING_SHOW_UNIONNAME);
				break;
			case 22:
				GetButton(nID)->SetChecked(!(GetButton(nID)->GetChecked()));
				if(GetButton(nID)->GetChecked())
					GetGameApplication()->SetSystemSetOnServer(USER_SETTING_SHOW_SEPTNAME);
				else 
					GetGameApplication()->ClearSystemSetOnServer(USER_SETTING_SHOW_SEPTNAME);
				break;
			case 23:
				GetButton(nID)->SetChecked(!(GetButton(nID)->GetChecked()));
				if(GetButton(nID)->GetChecked())  
					GetGameApplication()->SetSystemSetOnServer(USER_SETTING_SHOW_OFFICENAME);
				else 
					GetGameApplication()->ClearSystemSetOnServer(USER_SETTING_SHOW_OFFICENAME);
				break;
			case 24:
				GetButton(nID)->SetChecked(!(GetButton(nID)->GetChecked()));
				if(GetButton(nID)->GetChecked())  
					GetGameApplication()->SetSystemSetOnServer(USER_SETTING_SHOW_GRACENAME);
				else 
					GetGameApplication()->ClearSystemSetOnServer(USER_SETTING_SHOW_GRACENAME);
				break;
			default:
				if(nID >= IDC_ADDPOINT_BASE  && nID < IDC_ADDPOINT_BASE + 5)
				{
					stDistributePointPropertyUserCmd cmd;
					cmd.type = nID - IDC_ADDPOINT_BASE;
					SEND_USER_CMD(cmd);
					return true;
				}
			}
			
		}
		break;
	}
	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}


void CGuiUserInfo::SetUserBody(byte EquipPosition,t_Object* pObject)
{
	switch(EquipPosition) {
	case EQUIPCELLTYPE_BODY:
		{
			if (pObject == NULL)
				m_bEquipe = false;
			else
				m_bEquipe = true;

			if (m_bClothes==false)
				SetImage(m_pImage[0],EquipPosition,pObject);
		}
		break;
	case EQUIPCELLTYPE_ADORN:
		{
			if ((pObject == NULL)||(pObject&&GetObjectBase(pObject->dwObjectID)->dwType!=ItemType_FashionBody))
				m_bClothes = false;
			else
				m_bClothes = true;

			if (m_bClothes)
				SetImage(m_pImage[0],EquipPosition,pObject);
			else if (m_bEquipe)
				SetImage(m_pImage[0],EquipPosition,GetEquipItem(EQUIPCELLTYPE_BODY)->GetObject());
			else
				SetImage(m_pImage[0],EquipPosition,NULL);
		}
		break;
	}

	if ((m_bEquipe==false)&&(m_bClothes==false))
		m_bShowBody = true;
	else
		m_bShowBody = false;
	
}

void CGuiUserInfo::SetUserImage(byte EquipPosition,t_Object* pObject)
{
	switch(EquipPosition) {
	case EQUIPCELLTYPE_BODY:
	case EQUIPCELLTYPE_ADORN:
		SetUserBody(EquipPosition,pObject);
		/*if (pObject == NULL)
			m_bShowBody = true;
		else
			m_bShowBody = false;
		SetImage(m_pImage[0],EquipPosition,pObject);*/
		break;
	case EQUIPCELLTYPE_HANDL:
		SetImage(m_pImage[1],EquipPosition,pObject);
		break;
	case EQUIPCELLTYPE_HANDR:
		{
			//if ( pObject )
			//{
			//	ObjectBase_t* pObjectBase = GetObjectBase( pObject->dwObjectID );
			//	if ( pObjectBase )
			//		GetScene()->GetMainCharacter()->GetInterface()->SetArrowInRightHand( pObjectBase->dwType == ItemType_Arrow );
			//}
			//else
			//	GetScene()->GetMainCharacter()->GetInterface()->SetArrowInRightHand( false );
			SetImage(m_pImage[2],EquipPosition,pObject);
		}
		
		break;
	}

}

void CGuiUserInfo::SetImage(CGuiImage* pImage,byte EquipPosition,t_Object* pObject)
{
	if (pImage== NULL)
		return;
	
	pImage->Clear();

	if (pObject==NULL)
		return;
	
	ObjectBase_t* pObjectBase = GetObjectBase(pObject->dwObjectID);

	if (pObjectBase==NULL)
		return;

	stResourceLocation rl0 = GetObjectBasePicLocation(pObjectBase);
	stResourceLocation rl1 = GetObjectBasePatternPicLocation(pObjectBase);
	stResourceLocation rl2 = GetObjectBasePattern2PicLocation(pObjectBase);

	
	
	if(!rl0.IsEmpty())
	{
		pImage->AddAnimation(rl0);
	}
	

	if(!rl1.IsEmpty())
	{	
		pImage->AddAnimation(rl1,  pObjectBase->dwColor);		//color
			
	}

	if(!rl2.IsEmpty())
	{
		pImage->AddAnimation(rl2,  pObject->color);		//color
			
	}

}

bool CGuiUserInfo::IsEquipedAutoHpItem( bool bMainUser /* = true */ )
{
	if ( bMainUser && !m_bMainUser )
		return false;

	if ( !bMainUser && m_bMainUser )
		return false;

	CRoleItem* pItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_LEFT );
	if ( pItem && (pItem->GetObjectID() != 686) )
		pItem = NULL;

	if ( NULL == pItem )
	{
		pItem = GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_RIGHT );
		if ( pItem && (pItem->GetObjectID() != 686) )
			pItem = NULL;
	}

	return pItem != NULL;
}

CRoleItem* CGuiUserInfo::GetEquipItem( int equip_type,int x /* = 0 */ )
{
	CRoleItem* pRoleItem = NULL;

	CGuiTable* pTable = GetEquipTable(equip_type, x);
	if (pTable)
	{
		if (pTable->m_listItem.size()>0)
		{
			pRoleItem =(CRoleItem*) pTable->m_listItem[0]->m_pItemData;
		}
	}

	return pRoleItem;
}

void CGuiUserInfo::RefreshBodyColor(DWORD color)
{
	CRoleItem* pRoleItem = GetShowClothes();

	if (pRoleItem)
	{
		t_Object* pObject = pRoleItem->GetObject();
		if (pObject)
		{
			pObject->color = color;
			SetUserImage(pRoleItem->GetLocation().y,pObject);
		}
	}

}

CRoleItem* CGuiUserInfo::FindItemInEquipListByFiveTypeConstID( const DWORD& dwID, const DWORD& fiveType )
{
	if ( m_bMainUser )
		return NULL;

	for(std::vector<CRoleItem*>::iterator itor=m_listItem.begin();itor!=m_listItem.end();++itor)
	{
		t_Object* obj = (*itor)->GetObject();
		if ( (obj->dwObjectID == dwID) && (obj->fivetype == fiveType) )
		{
			return (*itor);
			break;
		}
	}

	return NULL;
}

void CGuiUserInfo::SetHorseData( t_HorseData* pData )
{
	if( pData )
	{
		//
		char szText[32];
		int nID = 300;

		NpcBase_t* pNpcBase = GetNpcBase(pData->id);
		if( pNpcBase )
		{
			UserInfo_SetHorseImage(314,pNpcBase->dwImage);
		}

		UserInfo_SetStaticText(315,pData->name);
		UserInfo_SetHorseText(316,"等级:",lv);

		UserInfo_SetHorseExt("增加主人力量:",str);
		UserInfo_SetHorseExt("增加主人智力:",intel);
		UserInfo_SetHorseExt("增加主人敏捷:",dex);
		UserInfo_SetHorseExt("增加主人精神:",men);
		UserInfo_SetHorseExt("增加主人体质:",con);
		UserInfo_SetHorseExt("增加主人速度:",speed);

		UserInfo_SetHorseExtPercent("增加主人物攻:",pdam);
		UserInfo_SetHorseExtPercent("增加主人物防:",pdef);
		UserInfo_SetHorseExtPercent("增加主人魔攻:",mdam);
		UserInfo_SetHorseExtPercent("增加主人魔防:",mdef);

		UserInfo_SetHorseExt("增加主人最大生命值:",maxhp);
		UserInfo_SetHorseExt("增加主人最大魔法值:",maxmp);
		UserInfo_SetHorseExt("增加主人抗减速",slowRes);
		UserInfo_SetHorseExt("增加主人抗失明",blindRes);
		UserInfo_SetHorseExt("增加主人抗石化",petrifyRes);
		UserInfo_SetHorseExt("增加主人抗冰冻",freezeRes);
		UserInfo_SetHorseExt("增加主人抗混乱",chaosRes);
		UserInfo_SetHorseExt("增加主人抗晕眩",faintRes);
		UserInfo_SetHorseExt("增加主人抗麻痹",lullRes);
		UserInfo_SetHorseExt("增加主人抗毒",poisonRes);
		return ;		
	}
	else
	{
		//
		UserInfo_SetHorseImage(314,-1);
		UserInfo_SetStaticText(300,"");
		UserInfo_SetStaticText(301,"");
		UserInfo_SetStaticText(302,"");
		UserInfo_SetStaticText(303,"");
		UserInfo_SetStaticText(304,"");
		UserInfo_SetStaticText(305,"");
		UserInfo_SetStaticText(306,"");
		UserInfo_SetStaticText(307,"");
		UserInfo_SetStaticText(308,"");
		UserInfo_SetStaticText(309,"");
		UserInfo_SetStaticText(315,"");
		UserInfo_SetStaticText(316,"");
	}
}