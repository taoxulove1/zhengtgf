/**
*\file		GuiItem.h
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2006-7-19  21:11:00 中国标准时间
*\brief	    包裹对话框
*
* 
*
*/

#pragma once
#include "..\gui\include\guidialog.h"

#include <map>
//class CGuiEquip;
/**
 * \brief 包裹对话框
 * 
 * 包裹对话框
 * 
 */
class CGuiItem :
	public CGuiDialog
{
public:
	//CGuiEquip* m_pEquip;
	///显示玩家信息
	bool m_bShowEquip;

	///包裹1
	CGuiTable* m_pTableLeft;
	///包裹2
	CGuiTable* m_pTableRight;

	CGuiItem(void);
	void OnClose(void);
	void OnCreate();
	
	///包裹网格
	CGuiTable* m_pTable;
	//CGuiImage* m_pImage;
	///银子显示框
	CGuiEditBox* m_pMoneyBox;

	CGuiEditBox* m_pGoldBox;

	void SetCharType(int type);

	CGuiTable* & GetEquipTable(int equip_type,int x = 0);

	void SetMoney(DWORD dwMoney);
	void SetGoldTicket(DWORD dwGold,DWORD dwTicket);
	DWORD GetMoney();
	void OnInitScript();

	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	void SetCharName(const char* name);
	void OnMoveMoney();

	void OnSetCloseLocation();

	HRESULT OnRender(float fElapsedTime);

	void bind_lua(lua_State* L);
};