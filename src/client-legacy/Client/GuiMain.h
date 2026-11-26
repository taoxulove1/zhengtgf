/**
 *\file		GuiMain.h
 *\version	$Id$
 *\author	$$$02公司
 *\			$$$02公司@###.com
 *\date		2006-7-19  21:11:00 中国标准时间
 *\brief	游戏主界面
 *
 * 
 *
*/

#pragma once
#include "..\gui\include\guidialog.h"
#include "./GuiButtonList.h"

#include <vector>

static const stPointF skillIconScale(0.8f, 0.8f);
static const stPointF itemIconScale(1.0f, 1.0f);

extern int g_nMainAccessBtnLeft ;
extern int g_nMainAccessBtnTop ;	
extern int g_MainAccessBtnSpace;

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
class CGMTextEffect;
class CGuiMain :
	public CGuiDialog
{
	CAnimation     m_aniMain;
	int            m_showX;
	int            m_showY;
	
	// 图标裁减区
	stRectI      m_rcIconClip;

	enumChatType m_iChatType;
	int m_iScreenWidth;
	//std::vector<IBitmap*> m_pBmpForeground;
	///步行按钮
	CGuiButton* m_BtnWalk;
	///跑步按钮
	CGuiButton* m_BtnRun;
	///攻击模式按钮
	//CGuiButton*	m_BtnAttackMode;
	///三值显示条
	CGuiImage*	m_imgThreeValue;
	
	///快捷键提示
	CToolTips*	m_pBtnAccessKeyToolTips[MAIN_ACCESS_KEY_NUM];
	///快捷键提示是否需要更新
	bool		m_bAccessKeyToolTipsNeedUpdate[MAIN_ACCESS_KEY_NUM];
	///当前快捷键组
	int	m_curAccessKeyGroup;
	///鼠标所指快捷键id
	int m_mouseOverAccessKeyNo;
	//////////////////////////////////////////////////////////////////////////
	int m_curEditAccessKeyNo;
	///经验条提示
	CToolTips	m_ExpTips;
	//
	
	///pk 模式选择按钮
	//CGuiButton* m_pBtnPkMode[PKMODE_NUM];
	CGuiButtonList m_aPkModeBtnList;
	CGuiMenu*	m_menu;
	//CGuiMenu*	m_cartMenu;
	enum enumBarType{
		BarTypeVert,
		BarTypeHoriz,
	};

	struct stBarShowInfo{
		stRectI rcClip;
		enumBarType type; 
		IBitmap* pBmp;
	};
	
	

	std::vector<stBarShowInfo> m_aBar;
	void RenderBar(CGuiMain::stBarShowInfo& bar,size_t iCur,size_t iMax,int index);
	void UpdateChatBtn();
	
	struct stStateInfo
	{
		bool bMainCharacter;
		bool bSkill;
		DWORD dwID;
		IBitmaps* pIcon;
		stPointI ptPos;
		DWORD time;
		DWORD startTime;
		DWORD rate;
		bool bFlashState;
		float fFlashRate;
		D3DXCOLOR color;

		stStateInfo()
		{
			color = -1;
			bFlashState = false;
			fFlashRate = 0.12f;
		}

		void PlayFlash(float fRate = 0.12f)
		{
			bFlashState = true;
			fFlashRate = fRate;
			color = -1;
		}

		void StopFlash()
		{
			bFlashState = false;
			color = -1;
		}
	};

	struct stStateTips
	{
		char	strName[32];
		char	strDesc[512];
	};

	std::vector<stStateInfo> m_aStateInfo;
	CToolTips	m_stateToolTips;

	BOOL ContainsPoint(POINT  ptScreen);
public:
	///快捷键按钮
	CGuiButton* m_pBtnAccessKey[MAIN_ACCESS_KEY_NUM];
	//Skill access btn active sign
	IBitmaps* m_activeSkillSign;
	//Item num image on access key
	IBitmap* m_itemNumImg[10];

	/// 鼠标是否在背景图片的有效区域
	bool	m_bMouseInBackground;
public:
	enum enumActiveAccessType{
		ActiveAccessSkill,
		ActiveAccessItem,
                ActiveAccessEmotion,
		ActiveAccessEquipment,
		ActiveAccessNums,
	};
	enum enumMessageType
	{
		TaskMessage = 1,
		ZoneMessage = 2,
		ExpMessage = 3,
		SystemMessage = 4,
		ScrollMessage = 5,
		LuckAttackMessage = 6,
	};

	CGuiButton* m_BtnPrivateChat;
	///包裹1
	CGuiTable* m_pTableLeft;
	///包裹2
	CGuiTable* m_pTableRight;
public:
	void SetChatType(enumChatType iChatType);
	CGuiMain(void);
	~CGuiMain();
	void OnCreate();
	void OnClose(void);
	void OnInitScript();
	void InitIcon();
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SendChat(CGuiEditBox* pEditChat);
	void RenderBackground(float fElapsedTime);
	HRESULT RenderAccessKeyNotes(float fElapsedTime);
	HRESULT RenderAccessKeyBtnToolTips(float fElapsedTime);
	HRESULT RenderAccessKeyBtnDisableStatus(float fElapsedTime);
	void OnResetScreenSize();
	HRESULT OnRender( float fElapsedTime );
	DWORD OnHiTest(POINT ptScreen);
	//cart menu
	void ShowCartMenu( bool bPlay );
	void CloseCartMenu();
	void PlayAni(stResourceLocation& rl,int nSpeed,int x,int y,bool bLoop=false);

	//State about
	DWORD	GetStateIconID(DWORD state);
	void	GetStateDesc(DWORD state, stStateTips& desc);
	int		GetMainCharacterStateIconNum();
	int		GetActiveNpcStateIconNum();
	void AddOneState(bool bMainCharacter, bool bSkill, DWORD dwID, DWORD time, DWORD rate);
	void DeleteOneState(bool bMainCharacter, bool bSkill, DWORD dwID);
	bool DeleteInterestNpcStateList();
	bool DeleteAllState(bool bMainCharacter);
	void RenderStateList( float fElapsedTime );
	void RenderStateTooltips(DWORD curTime);
	void UpdateStateList(DWORD curTime);
	void UpdateStateTooltips( stPointI& ptPos, DWORD curTime );
	stStateInfo* GetMouseOverStateInfo();
	//

	//Set access key
	bool SetAccessKey( const int& keyID );
	bool IsAnyActiveForAccessKeySetting( enumActiveAccessType& type );
	BYTE GetCurAccessKeyGroup() { return m_curAccessKeyGroup; }
	bool SetActiveAccessKeyGroup(BYTE group);
	void OnAccessBtnSelectSkill( const int& btnID, const int& skillId );
	bool OnFindAccessSkill();
	void EditAccessKeyBtn(const DWORD& id);
	
	void RefreshKeyGroupState( const int& group = 0 );
	void UpdateKeyTooltips( const BYTE& btnID );
	void ChangeCurKeyGroup( bool bDown = true );

	void UpdateAccessKeyBtnIcon();
	int  GetMouseOverAccessKeyNo();
	int	 GetCurAccessKeyNo() { return m_mouseOverAccessKeyNo; }
	void DeleteMouseOverAccessKey();
	void DeleteAccessKey(enumGameAccelKeyType type, DWORD& id);
	void ReplaceAccessKey(enumGameAccelKeyType type, const DWORD& id, const DWORD& newID);
	void RemoveAccessKeyBtnInfo( BYTE key );

	//Change attack mode tips
	void ChangeAttackModeBtnTips();
	void ChangeActivePkModeBtn();
	void TurnOffPkModeList();
	void OnOffPkModeList(int curMode);
    bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	void OnOffWalkRunBtn();
	void SwitchWalk();
	bool OnContextMenu();
	CGuiButton* InsertPrivateButton(int nPos);

	//Get chat type
	int GetChatType() { return m_iChatType; }


	void DrawScreenCenterMsg();
	void DrawScreenRightBottomMsg(stPointI& pt);
	void AddMessage(const char* msg,enumMessageType type,DWORD color = -1);

	void AddTaskTimeMessage(DWORD dwTaskID,const char* title,DWORD dwTime);
	void DrawTaskTimeMessage( const POINT& pt );
	void RemoveTaskTimeMessage(DWORD dwTaskID);

	void __SetEmotionAccessBtn(stResourceLocation & rl,int keyID);

	void AddGMTextSprite(const char * pchar,DWORD clr);

	//升级属性点还有剩余提示按钮相关,无则创建,并按照nShow操作
	void AddshuxBtn(bool nShow = true);
	
private:
	std::auto_ptr<CGMTextEffect> m_pTextSprite;

	struct stHelpButton
	{
		CGuiButton		*pBtn;
		enumUserSetting type;
		DWORD			dwStartTime;
		int				iBtnId;

		stHelpButton()
		{
			pBtn = NULL;
			type = USER_SETTING_TEAM;//忽略
			dwStartTime = xtimeGetTime();
		}
		void Hide()
		{
			if(pBtn) 
			{
				pBtn->SetToolTips("");
				pBtn->SetVisible(false);
			}
		}
	};

	typedef std::vector<stHelpButton *> stHelpButtons;
	stHelpButtons m_helpBtns;

	stHelpButton *m_pshuxBtn;//升级属性点还有剩余提示
	inline stHelpButton *GetHideshuxBtn(){return m_pshuxBtn;}

private:
	inline int GetNextHelpBtnId();
	inline stHelpButton *GetHideBtn( );
	bool bIsTypeExist(enumUserSetting type)
	{
		for(stHelpButtons::iterator it = m_helpBtns.begin();
			it != m_helpBtns.end(); ++it)
		{
			if((*it)->type == type) return true;
		}
		return false;
	}
public:
	void UpdateHelpBtnState();
	void ShowHelpTipBtn(enumUserSetting type);
	void OnHelpTipBtn(enumUserSetting type);
	void ShowAllTipBtn(bool bShow = true);
	size_t GetShownHelpTipNum();
	BOOL IsInRect(POINT ptScreen);


private:
	struct stScreenMessage
	{
		std::string strMsg;
		DWORD dwShowTime;
		DWORD dwTotalShowTime;
		DWORD dwColor;
		stPointF scale;
		enumMessageType type;
		int	  alpha;

		stScreenMessage()
		{
			type = TaskMessage;
			strMsg = "";
			dwShowTime = 0;
			dwColor = -1;
			scale = stPointF(1.0f,1.0f);
			dwTotalShowTime = 10000;
		}
	};
	//任务消息
	typedef std::vector<stScreenMessage> TaskMsgList;
	typedef TaskMsgList::iterator TaskMsgListIt;
	TaskMsgList m_taskMsgList;

	//经验值消息
	struct stExpMessage
	{
		char msg[MAX_CHATINFO];
		DWORD dwColor;
		DWORD dwTime;
		enumMessageType type;
	};
	typedef std::vector<stExpMessage> ExpMessageList;
	typedef ExpMessageList::iterator ExpMessageListIt;
	ExpMessageList m_expMsgList;

	struct stTaskTimeMessage
	{
		DWORD		dwTaskID;
		std::string	strTaskTitle;
		DWORD		dwTaskTime;
		DWORD		dwBeginTime;
	};
	typedef std::vector<stTaskTimeMessage>	TaskTimeMsgList;
	typedef TaskTimeMsgList::iterator		TaskTimeMsgListIt;
	TaskTimeMsgList	m_listTaskTime;

	//
	void __AddMessage(const char* msg,enumMessageType type,DWORD color);

	/////////////////////////////////////
	IBitmaps * m_pTeamHeadIcon;
	IBitmaps * m_pTeamExpAssignByLevel;
	IBitmaps * m_pTeamExpAssignBySpecial;
	IBitmaps * m_pTeamItemAssignByFreedom;
	IBitmaps * m_pTeamItemAssignByPoll;
	stRectI    m_rcClipTeamExpAssignByLevel;
	stRectI    m_rcClipTeamExpAssignBySpecial;
	stRectI    m_rcClipTeamItemAssignByFreedom;
	stRectI    m_rcClipTeamItemAssignByPoll;

	///////////////////////////////////////////////
	// tooltips
	void     RenderTeamFlag();
	void     RenderTeamTip();
	bool     m_bShowTeamToolTip;
	//////////////////////////////////////////////
public:
	bool isHasStateSkill(DWORD dwID);

public:
	//系统公告 in Client
	bool initSysMessage(const char* pszFileName);	//
	bool m_bEnabledSysmessage;						//1=on,0=off;
	void OnUpdateSysmessage(float fElapsedTime);
	//系统公告相关变量
	struct stSysMessage 
	{
		std::string starttime;
		std::string endtime;
		int order;
		int login;
		int interval;
		int states;//本轮怎么发的状态
		DWORD dwCurTime;
		int j;
		std::vector<std::string> pmessgae;
		stSysMessage()
		{
			starttime ="000";
			endtime ="111";
			order = 0;
			login = 0;
			interval = -1;
			states = 0;//0=没有发过，1＝发过，下回继续，2＝只发一下，3＝永久不再发
			dwCurTime = 0;
			j = 0;
		}
	};
	std::vector<stSysMessage> m_SysMessage;
	//
public:
	bool CanPrintCenter;
};

inline bool bIsUserUnSetted(enumUserSetting type);
inline void ShowHelpTip(enumUserSetting type);
// inline void OnTipHelp(enumUserSetting type,bool bCheck = true);
inline void OnTipHelp(enumUserSetting type,bool bCheck= true)
{
	if(bIsUserUnSetted(type)) { if( bCheck ) ShowHelpTip(type);}
}


extern bool SendAttackMode(int curMode);
extern bool ChangeAttackMode(int mode);
extern bool SaveBackBufferToFile();
extern void OnSitDown();
extern void OnOffAutoAttackSummon();
extern void OnRide();
extern void OnOffSysDialog();