/**
*\file		GuiItem.cpp
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2006-7-19  21:11:00 中国标准时间
*\brief	    包裹对话框
*
* 
*
*/


#include "public.h"
#include "./GameGuiManager.h"
#include ".\guiitem.h"
#include "./table.h"
#include "./RoleItem.h"
#include "./GuiMoneyInputBox.h"
#include "./GuiTrade.h"
#include "./GameState.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./GuiEquip.h"
#include "./GuiUserInfo.h"
#include "../luabind/public.h"

#define IDC_SHOWEQUIP 5
#define IDC_HIDEEQUIP 6
#define IDC_CONNIMAGE 6

void CGuiItem::bind_lua(lua_State* L)
{
	using namespace luabind;
	CGuiDialog::bind_lua(L);
	module(L)
	[
		class_<CGuiItem,CGuiDialog>("CGuiItem")
			.def("OnMoveMoney",CGuiItem::OnMoveMoney)
	];
	m_lua->SetGlobal("this",this);
}

/**
 * \brief 构造函数
 * 
 * 构造函数
 * 
 * \return 无
 */
CGuiItem::CGuiItem(void)
{
	FUNCTION_BEGIN;

	m_bCloseIsHide = true;
//	m_pEquip = NULL;
	m_bShowEquip = false;
	m_clientOffsetTopLeft = stPointI(0,0);
	m_clientOffsetRightBottom = stPointI(0,0);

	FUNCTION_END;
}

/**
 * \brief 创建对话框
 * 
 * 创建对话框
 * 
 * \return 无
 */
void CGuiItem::OnCreate()
{
	FUNCTION_BEGIN;
	MessageBox(NULL,"1","1",MB_OK);
	CGuiDialog::OnCreate();
//	m_pImage = GetImage(1);
	m_pTable = GetTable(2);
	m_pTable->m_iTableType = OBJECTCELLTYPE_COMMON;
	m_pTable->m_iTableID = 0;

	m_pMoneyBox = GetEditBox(3);
	
	m_pMoneyBox->SetEnabled(false);
	m_pGoldBox = GetEditBox(20);
	m_pGoldBox->SetEnabled(false);

	GetButton(IDC_HIDEEQUIP)->SetVisible(false);
	GetButton(IDC_SHOWEQUIP)->SetVisible(false);
	m_pMoneyBox->SetVisible(false);
	GetEditBox(11)->SetEnabled(false);
	//GetButton(12)->SetEnabled(false);
	m_bShowEquip = false;

	m_pTableLeft = GetTable(300);
	m_pTableLeft->m_iTableType = OBJECTCELLTYPE_EQUIP;
	m_pTableLeft->m_EquipPosition = stPointI(EQUIPCELLTYPE_LEFT,EQUIPCELLTYPE_PACKAGE);

	m_pTableRight = GetTable(301);
	m_pTableRight->m_iTableType = OBJECTCELLTYPE_EQUIP;
	m_pTableRight->m_EquipPosition = stPointI(EQUIPCELLTYPE_RIGHT,EQUIPCELLTYPE_PACKAGE);
	//m_pImage = GetImage(7);
	//m_pImage->SetVisible(false);

	FUNCTION_END;
}

/**
 * \brief 设置关闭按钮
 * 
 * 设置关闭按钮
 * 
 * \return 无
 */
void CGuiItem::OnSetCloseLocation()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();


	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());

		m_pBtnClose->SetLocation(pt.x-64,pt.y-2);
	}

/**
 * \brief 关闭对话框
 * 
 * 关闭对话框
 * 
 * \return 无
 */
	FUNCTION_END;
}
void CGuiItem::OnClose(void)
{
	FUNCTION_BEGIN;

	GetGameGuiManager()->m_guiItem = NULL;


	FUNCTION_END;
}

/**
 * \brief 设置玩家角色
 * 
 * 设置玩家角色
 * 
 * \param type : 角色类型
 * \return 无
 */
void CGuiItem::SetCharType(int type)
{
	FUNCTION_BEGIN;

	if (GetGameGuiManager()->m_guiUserInfo)
		GetGameGuiManager()->m_guiUserInfo->SetCharType(type);

	FUNCTION_END;
}

/**
 * \brief 设置银子
 * 
 * 设置银子
 * 
 * \param dwMoney : 银子数
 * \return 无
 */
void CGuiItem::SetMoney(DWORD dwMoney)
{
	FUNCTION_BEGIN;

	char szMoney[32];
	sprintf(szMoney,"%u",dwMoney);
	//m_pMoneyBox->SetText(szMoney);
	m_pMoneyBox->SetText(Money2String(dwMoney));
	FUNCTION_END;
}


void CGuiItem::SetGoldTicket(DWORD dwGold,DWORD dwTicket)
{
	char szGold[32];
	sprintf(szGold,"%s %u积分",Money2String(dwGold),dwTicket);
	//m_pMoneyBox->SetText(szMoney);
	m_pGoldBox->SetText(szGold);
}
/**
 * \brief 得到包裹银子数
 * 
 * 得到包裹银子数
 * 
 * \return 无
 */
DWORD CGuiItem::GetMoney()
{
	FUNCTION_BEGIN;

	//return strtoul(m_pMoneyBox->GetText(),NULL,10);
	return String2Money(m_pMoneyBox->GetText());

	FUNCTION_END;
}

/**
 * \brief 点击银子按钮事件
 * 
 * 点击银子按钮事件
 * 
 * \return 无
 */
void CGuiItem::OnMoveMoney()
{
	FUNCTION_BEGIN;

	if(g_pMoneyInputBox) return;

	const char* pText ;
	DWORD dwLocationDst;
	switch(GetGameState()->GetStateItem())
	{
	case eGameStateItem_TradeRun:
		pText = "要交易多少银子？";
		dwLocationDst = OBJECTCELLTYPE_TRADE;
		break;
	default:
		return;
		pText = "要扔下多少银子？";
		dwLocationDst = OBJECTCELLTYPE_NONE;
		break;
	}

	//CRoleItem* pSrcItem = GetScene()->GetMainCharacter()->GetMoneyItem(OBJECTCELLTYPE_COMMON);
	//if(!pSrcItem) return;
	static stSplitItemData st;

	st.qwThisID = 0;//pSrcItem->GetThisID();
	st.dwCurNum = String2Money(GetEditBox(11)->GetText());
	st.dst.dwLocation = dwLocationDst;
	st.dst.dwTableID = 0;
	st.dst.x = -1;
	st.dst.y = -1;

	CGuiMoneyInputBox* pBox = GetGameGuiManager()->AddMoneyInputBox(pText,eAction_SplitItem,&st);
	//pBox->SetNumber(0,pSrcItem->GetCount());
	CRoleItem::s_bCanMoveItem = false;
	g_pMoneyInputBox =(void*) pBox;

	FUNCTION_END;
}

/**
 * \brief 设置脚本函数
 * 
 * 设置脚本函数
 * 
 * \return 无
 */
void CGuiItem::OnInitScript()
{
	FUNCTION_BEGIN;

	FUNCTION_END;
}

/**
 * \brief 对话框消息处理
 * 
 * 对话框消息处理
 * 
 * \param nEvent : 消息类型
 * \param nID : 控件ID
 * \param pControl : 控件指针
 * \return 消息返回值
 */
bool CGuiItem::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	FUNCTION_BEGIN;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		switch(nID)
		{
		case IDC_SHOWEQUIP:
			pControl->SetVisible( false );
			GetButton(IDC_HIDEEQUIP)->SetVisible( true );
			
	//		m_pEquip->SetLocation( m_x , m_y + (GetHeight() - m_pEquip->GetHeight())/2 );
	//		m_pEquip->SetVisible( true ,false);

			{
			//	m_pEquip->SetColor(COLOR_ARGB(0,255,255,255));
				stGuiAnimationInfo ani;
			//	ani.ptDst = stPointI(m_x - m_pEquip->GetWidth() , m_y + (GetHeight() - m_pEquip->GetHeight())/2);
				
				ani.dwColorDst = m_dwColor;
				ani.eAniComplete = GuiAniEvent_None;
				ani.fTime = 0.2f;
			//	m_pEquip->BeginAnimation(&ani);

				//m_pImage->SetVisible(true);

				//m_pImage->SetLocation(-m_pEquip->GetWidth(),(GetHeight() - m_pEquip->GetHeight())/2);
			}

			m_bShowEquip = true;
			return true;
		case IDC_HIDEEQUIP:
			pControl->SetVisible(false);
			GetButton(IDC_SHOWEQUIP)->SetVisible(true);
			//m_pEquip->SetVisible(false);

			{
			//	m_pEquip->SetColor(COLOR_ARGB(0,255,255,255));
				stGuiAnimationInfo ani;
//				ani.ptDst = stPointI(m_x , m_y + (GetHeight() - m_pEquip->GetHeight())/2);

				ani.dwColorDst = COLOR_ARGB(50,255,255,255);
				ani.eAniComplete = GuiAniEvent_Hide;
				ani.fTime = 0.2f;
			//	m_pEquip->BeginAnimation(&ani);

				//m_pImage->SetVisible(true);
			}

			//m_pImage->SetVisible(false);
			m_bShowEquip = false;
			return true;
		}
		break;
	case EVENT_DIALOG_HIDE:
	//	m_pEquip->SetVisible( false );
		return true;
	case EVENT_DIALOG_SHOW:
		//if( m_bShowEquip )
		//	m_pEquip->SetVisible(true);
		return true;
	case EVENT_DIALOG_MOVE:
		//if(m_pEquip->IsVisible())
		{
		//	m_pEquip->SetLocation(m_x - m_pEquip->GetWidth(),m_y + (GetHeight() - m_pEquip->GetHeight())/2);
		}
		return true;
	}
	return CGuiDialog::OnGuiEvent(nEvent,nID,pControl);

	FUNCTION_END;
}

/**
 * \brief 得到网格
 * 
 * 得到网格
 * 
 * \param equip_type : 网格类型
 * \param x : 网格位置
 * \return 得到网格
 */
CGuiTable* & CGuiItem::GetEquipTable(int equip_type,int x )
{
	FUNCTION_BEGIN;

	return GetGameGuiManager()->m_guiUserInfo->GetEquipTable(equip_type,x);

	FUNCTION_END;
}

/**
 * \brief 设置角色名字
 * 
 * 设置角色名字
 * 
 * \param name : 角色名字
 * \return 无
 */
void CGuiItem::SetCharName(const char* name)
{
	FUNCTION_BEGIN;

///	m_pEquip->SetCharName(name);

	FUNCTION_END;
}

/**
 * \brief 刷新对话框
 * 
 * 刷新对话框
 * 
 * \param fElapsedTime : 刷新时间
 * \return 处理结果
 */
HRESULT CGuiItem::OnRender(float fElapsedTime)
{
	FUNCTION_BEGIN;

	switch(GetGameState()->GetStateItem())
	{
	case eGameStateItem_TradeRun:
		if (GetGameGuiManager()->m_guiTrade==NULL)
			break;

		//if (GetGameGuiManager()->m_guiTrade->IsEdit()==false)
		//	GetButton(4)->SetEnabled(false);
		//else
		//	GetButton(4)->SetEnabled(true);
		GetEditBox(11)->SetText(Money2String(String2Money(m_pMoneyBox->GetText()) - GetGameGuiManager()->m_guiTrade->GetMoney(OBJECTCELLTYPE_TRADE)));
		break;
	default:
		//GetButton(4)->SetEnabled(false);
		GetEditBox(11)->SetText(m_pMoneyBox->GetText());
		break;
	}
	GetEditBox(11)->SetEnabled(false);
	GetEditBox(11)->SetVisible(true);

	return  CGuiDialog::OnRender(fElapsedTime);

	FUNCTION_END;
}