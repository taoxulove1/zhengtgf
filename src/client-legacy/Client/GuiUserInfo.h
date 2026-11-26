/**
*\file		GuiUserInfo.h
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2006-7-19  21:11:00 中国标准时间
*\brief	    人物属性界面对话框
*
* 
*
*/


#pragma once
#include "..\gui\include\guidialog.h"



#define   GETSHOWCOLOR(total,base)  total==base?0xffffffff:(total>base?0xff00ff00:0xffff0000)




/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */

class CGuiUserInfo :
	public CGuiDialog
{
	struct stTitle{
		int      key;
		stRectI  rect;
		byte     index;
	};

	struct stEquipByName{
		byte  numBlue;
		byte  numGlod;
		byte  numHoly;
	};

	struct stEquipByMaker{
		char strMakerName[MAX_NAMESIZE];
		stEquipByName numSetList;

		void Init()
		{
			strMakerName[0] = 0L;
			numSetList.numBlue = 0;
			numSetList.numGlod = 0;
			numSetList.numHoly = 0;
		}
	};
	enum enmEuipByMakerType{
		EquipByMaker_T_NONE	= -1,
		EquipByMaker_T_BUG_BLUE,
		EquipByMaker_T_BUG_GOLD,
		EquipByMaker_T_BUG_HOLY,
		EquipByMaker_T_WONDER_BLUE,
		EquipByMaker_T_WONDER_GOLD,
		EquipByMaker_T_WONDER_HOLY,
	};

	struct stUserData{
		::t_MapUserData*     pMapUserData;
		t_MainUserData*    pMainUserData;
		::t_MapUserData      tMapUserData;
		t_MainUserData     tMainUserData;
	};


	std::map<int,CGuiTable*> m_tables;
	CGuiTab*               m_pTab;
	bool                   m_bMainUser;
	CGuiImage*              m_pImage[3];
	CToolTips               m_ToolTips;
	WORD                   m_LevelNum;
	
	stUserData              m_UserData;
	
	std::vector<stTitle>     m_titleList;
	std::vector<stTitle>     m_equipList;
	bool                     m_bShowBody;

	std::vector<stEquipByMaker>		m_vEquipByMaker;
	enmEuipByMakerType		m_equipByMakerType;
	char					m_strSetMakerName[MAX_NAMESIZE];

	bool                  m_bEquipe ;
	bool                  m_bClothes ;
	std::vector<CRoleItem*> m_listActiveItem;

	std::lookaside_allocator<CRoleItem> m_allocRoleItem;
	std::vector<CRoleItem*> m_listItem;
	
	
	void SetUserBody(byte EquipPosition,t_Object* pObject);
	void SetImage(CGuiImage* pImage,byte EquipPosition,t_Object* pObject);
	void SetTitleList();
	void TipsRender();
	void SetUserData(void* pData);
	void ShowUserInfoText1();
	void ShowUserInfoText2();
	void ControlImage(byte pos);
	void ShowEquipByName();
	void RefreshGoodnessOnXiXinGeMianState();

public:
	void SetHorseData( t_HorseData* pData );
	//CAnimation m_aniBlaze;
	//IBitmap* m_bmpSex[2];
	CAnimation m_aniBody[3];

	CGuiUserInfo(bool bMainUser = true);
	void OnClose(void);
	char* GetSetMakerName();
	BYTE GetSetEquipType();

	//void InitCustomElements();
	HRESULT OnRender(float fElapsedTime);
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void RefreshButtonState();
	void OnCreate();
	void OnSetCloseLocation();
	void BodyRender(stPointI ptRender);
	void SetCharType(int type,void* pData = NULL);
	void SetUserImage(byte EquipPosition,t_Object* pObject = NULL);
	void RefreshShowState();
	void SetLevelNum(WORD LevelNum)
	{
		if (LevelNum!=0)
			m_LevelNum = LevelNum;
		else
			m_LevelNum = (WORD)-1;
	}

	void InitIcon();
	bool IsMainInfo(){ return m_bMainUser;}
	bool IsEquipedAutoHpItem(bool bMainUser = true);

	void RefreshBodyColor(DWORD color);
	bool IsTonic();
	CRoleItem* GetShowClothes();
	bool IsActiveItem(CRoleItem* pItem);
	void RefreshActiveItem();
	void InsertActiveItem(CRoleItem* pRoleItem);
	void SetEquipByName(CRoleItem* pItem, bool bMainCharacter = true);

	CGuiTable* & GetEquipTable(int equip_type,int x = 0)
	{
		int value = (equip_type << 16 | x);
		return m_tables[value];
	}

	bool IsAdornObject(DWORD dwObjectID);
	bool IsAdornObjectByType(DWORD  dwType);

	//
	CRoleItem* FindItemInEquipListByFiveTypeConstID(const DWORD& dwID, const DWORD& fiveType);
	void ShowActiveFiveEffect();

	CRoleItem* GetEquipItem(int equip_type,int x = 0);

};


