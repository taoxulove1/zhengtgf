#include "public.h"
#include "../gui/include/guiTypes.h"
#include ".\gameguimanager.h"
#include ".\guilogin.h"
#include ".\guiKeyBoard.h"
#include ".\guiMain.h"
#include "../gui/include/guiManager.h"
#include ".\guichatoutput.h"
#include ".\guimessagebox.h"
#include "./GuiCreateCharDialog.h"
#include "./GuiRegisterDialog.h"
#include "./GuiSelectCharDialog.h"
#include "..\engine\include\stack_alloc.h"
#include "./GuiSystem.h"
#include "./GuiModifyPasswd.h"
#include "./GuiItem.h"
#include "./GuiHelp.h"
#include "./GameGuiFloatItem.h"
#include "./GuiInputBox.h"
#include "./GuiMoneyInputBox.h"
#include "./GuiUserInfo.h"
#include "./GuiPrivateChat.h"
#include "./GuiNpcDialog.h"
#include "./GuiTrade.h"
#include "./GuiNpcTrade.h"
#include "./GuiMiniMap.h"
#include "./GuiTeam.h"
#include "./GuiItemUp.h"
#include "./GuiItemMake.h"
#include "./GuiChatInvite.h"
#include "./GuiSkill.h"
#include "./CreateRole.h"

#include "./GuiEquip.h"
#include "./MainCharacter.h"
#include "./GameScene.h"


#include "GuiTaskDialog.h"
#include "./GuiWorldConn.h"
#include "./GuiBallot.h"
#include "./GuiFloatControl.h"
#include "./GuiFloatAccessKey.h"
#include "./GuiShortcutItem.h"
#include "./GuiChatSelect.h"
#include "./GuiItemDetailInfo.h"
#include "./GuiFace.h"
#include "./GuiSysChat.h"
#include "./GuiWorldBoard.h"
#include "./GuiNpcMake.h"
#include "./GuiSaveBox.h"
#include "./CharacterProperty.h"

#include "./GuiColorDialog.h"
#include "./GuiReliveDlg.h"
#include "./GuiShopDialog.h"
#include "./GuiHairStyleDialog.h"
#include "./GuiClothColorDialog.h"
#include "./GuiHeadListDialog.h"
#include "./GuiPetDialog.h"
#include ".\GuiPetCtrlDialog.h"
#include "./GuiPrivateChatSelect.h"
#include ".\gameappation.h"
#include "./GuiPackage.h"
#include ".\GuiSaveBoxContinueDialog.h"
#include ".\NationalMapDialog.h"
#include "./GuiChatSelectButton.h"
#include "./GuiCompetition.h"
#include "./GuiPasswordDlg.h"
#include "./GuiSendMailDialog.h"
#include "./GuiRecvMailDialog.h"

#include "./GuiGiveGiftDialog.h"

#include "./GuiRecoSys_A_InputDlg.h"
#include "./GuiRecoSys_A_Query_TakeDlg.h"
#include "./GuiRecoSys_B_Query_TakeDlg.h"

//师徒
#include "./GuiMasterBountyDlg.h"
#include "./GuiPrenticeBountyDlg.h"

#include "./MiniGameListDlg.h"
#include "./MGMoneyInputDlg.h"
#include "./MGNumberDlg.h"

#include "./GuiAuctionDialog.h"

#include "./GuiEmotionDialog.h"
#include "./GuiGmMsgBoard.h"
#include "./GuiChangeCountry.h"
#include "./GuiChangeWarField.h"
#include "./GuiSelectDialog.h"
#include "./GuiCountryWarQuery.h"
#include ".\gamestate.h"
#include "./GuiAuctionGoldCoinInputDialog.h"
#include "./GuiAuctionMoneyInputDialog.h"
#include "./GuiUnoinPower.h"
#include "./GuiOthersShopDialog.h"
#include "./GuiGoldExchangeDialog.h"
#include "./GuiAutoReplySettingDlg.h"
#include "./CountryInfo.h"
#include "./GuiExitGameDlg.h"
#include ".\GuiAdoptCartoonDialog.h"
#include ".\GuiClearPointDlg.h"
#include ".\GuiCartoonAddCentDialog.h"
#include "./GuiApplyConn.h"
#include ".\GuiPetPackDialog.h"
#include ".\GuiInputBox2Dialog.h"
#include "./GuiAddNearByChat.h"
#include ".\GuiAddMemberDialog.h"
#include ".\GuiStockDialog.h"
#include "./GuiSelectMarryType.h"
#include "./GuiSelectSeptDareType.h"
#include "./GuiNameMoneyInputboxDialog.h"
#include "./GuiInputBoxFive.h"
#include "./Chat.h"
#include ".\gameappation.h"

#include "./GuiTaskDialog.h"
//#include <boost/regex.hpp>
#include <ostream>
//#include "../greta/regexpr2.h"
//using namespace regex;

#define RETURN(Val) { retval = (Val); goto _ret;}

const char* c_szMBOK		=	"确定|1|Enter";
const char* c_szMBOKCancel	=	"确定|1|Enter|取消|2|Esc";
const char* c_szMBYesNo		=	"是|1|Enter|否|2|Esc";

CGameGuiManager g_GameGuiManager;

void* g_pMoneyInputBox = NULL;

const char* c_szGuiPack = "data\\gui.pak";

//////////////////////////////////////////////////////////////////////////
// 下列宏定义的目的是为了降低定义类似Add***Dialog函数的复杂性
// 其中:
// f	filename
// p	m_p***Dialog	GameGuiManager里定义的指向CGui***Dialog的指针
// c	CGui***Dialog	
// a?	CGui***Dialog构造函数的参数

#define GUIMANAGER_LOADXML(f,p)			HRESULT  hr=GetGuiManager()->LoadGuiFromXML(c_szGuiPack,f,p);if(FAILED(hr)){delete p;p = NULL;Debug_String(f);return NULL;}
#define GUIMANAGER_RETURN(p)			p->SetEnabled(true);p->SetVisible(true);return p;

#define GUIMANAGER_ADD_DIALOG(c,p,f)				if(!p) {p = new c();GUIMANAGER_LOADXML(f,p)} GUIMANAGER_RETURN(p);
#define GUIMANAGER_ADD_DIALOG1(c,p,f,a1)			if(!p) {p = new c(a1);GUIMANAGER_LOADXML(f,p)} GUIMANAGER_RETURN(p);
#define GUIMANAGER_ADD_DIALOG2(c,p,f,a1,a2)			if(!p) {p = new c(a1,a2);GUIMANAGER_LOADXML(f,p)} GUIMANAGER_RETURN(p);
#define GUIMANAGER_ADD_DIALOG3(c,p,f,a1,a2,a3)		if(!p) {p = new c(a1,a2,a3);GUIMANAGER_LOADXML(f,p)} GUIMANAGER_RETURN(p);
#define GUIMANAGER_ADD_DIALOG4(c,p,f,a1,a2,a3,a4)	if(!p) {p = new c(a1,a2,a3,a4);GUIMANAGER_LOADXML(f,p)} GUIMANAGER_RETURN(p);
#define GUIMANAGER_SAFE_CLOSE_DIALOG(p)				if(p)  {p->Close();p = NULL;}
//
//////////////////////////////////////////////////////////////////////////

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pszText : 描述
 * \param type : 描述
 * \param pData : 描述
 * \param pszButtons : 描述
 * \param bModal : 描述
 * \return 返回值的描述
 */
CGuiMessageBox* GameMessageBox(const char* pszText,enumGuiActionType type,void* pData,const char* pszButtons,bool bModal)
{
	FUNCTION_BEGIN;

	return GetGameGuiManager()->AddMessageBox(pszText,type,pData,pszButtons,bModal);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGameGuiManager::CGameGuiManager(void)
{
	FUNCTION_BEGIN;

	///Init();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGameGuiManager::~CGameGuiManager(void)
{
	FUNCTION_BEGIN;

	//Release();

	FUNCTION_END;
}

void CGameGuiManager::InitCreate()
{
	GetGuiManager()->InitCreate();
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGameGuiManager::Init()
{
	FUNCTION_BEGIN;

	m_bInitMainCharacterInfo = false;
	m_guiLogin	=	NULL;
	m_guiKeyBoard   =       NULL;
	m_guiMain	=	NULL;
	m_guiChatOutput = NULL;
	m_guiSystem = NULL;
	m_guiSelectChar = NULL;
	m_guiCreateCharDialog = NULL;
//	m_guiRegisterDialog = NULL;
//	m_guiModifyPasswd = NULL;
	m_guiItem = NULL;
	m_guiHelp = NULL;
	m_guiEmotionDlg = NULL;
	m_guiFloatItem = NULL;
	m_guiUserInfo = NULL;
	m_guiUserInfoShow = NULL;
	m_guiNpcDialog = NULL;
	m_guiTrade = NULL;
	m_guiMiniMap = NULL;
	m_guiSysChat = NULL;
	m_guiNpcTrade = NULL;
	m_guiTeam = NULL;
	m_guiSkill = NULL;
	m_guiSkill = NULL;
	
	m_guiSavingBoxDlg = NULL;
	m_guiShop = NULL;
	m_guiOthersShop = NULL;
	m_guiWorldConn = NULL;
	m_guiFloatControl = NULL;
	m_guiFloatAcessKey = NULL;
	m_guiShortcutItem = NULL;
	m_guiChatSelect = NULL;
	m_guiItemDetailInfo = NULL;
	m_guitaskdialog = NULL;
	m_guiReliveDlg = NULL;
	m_guiFace = NULL;
	m_guiLageMap = NULL;
	m_guiNpcMake = NULL;
	m_guiMakeGuess =NULL;
	m_guiCharacterProperty = NULL;
	m_guiBallot = NULL;
	m_guiHairStyleDlg = NULL;
	m_guiHeadListDlg = NULL;
	m_guiClothColorDlg = NULL;
	m_guiPetDlg = NULL;
	m_guiPrivateChatSelect = NULL;
	m_guiPackage1 = NULL;
	m_guiPackage2 = NULL;

	m_guiPetFloatDlg = NULL;
	//m_guiSummonFloatDlg = NULL;
	m_guiSaveBoxContinueDlg = NULL;
	m_guiNationalMap = NULL;
	m_guiPasswordDlg = NULL;
	m_guiMakeTo1 = NULL;
	m_guiMakeTo2 = NULL;
	m_guiNearby = NULL;
	m_guiChatButton = NULL;
	m_guiInvite = NULL;
	m_guiCompetition = NULL;
	m_guiRecvMailDlg = NULL;
	m_guiSendMailDlg = NULL;
	m_guiSelectDialog = NULL;
	m_guiSelectDialog = NULL;
	m_guiAucMoneyInputDlg = NULL;
	m_guiAucGodlCoinInputDlg = NULL;
	m_guiGoldExchangeDlg = NULL;

	m_guiModifyRevenueBox = NULL;
	m_guiAdoptCartoonDlg = NULL;
	m_guiCartoonAddCentDlg = NULL;

	m_guiCountrySaveingBoxDlg = NULL;
	m_guiAutoReply = NULL;
	m_guiCountryTechDlg      = NULL; 
	m_guiCountry             = NULL;
	m_guiApplyConn           = NULL;
	m_guiPetPackDlg			 = NULL;
	m_guiInputBox2Dlg		 = NULL;
	m_guiAddMemberDlg		 = NULL;
	m_guiStockDlg			 = NULL;
	m_guiNameMoneyInputDlg	 = NULL;
	m_pUserListDlg			 = NULL;

	m_guiExitGame = NULL;
	m_guiClearPoint      = NULL;
	
	extern bool ItemTableEvent(CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	extern void RoleItemRender(CGuiTable* pTable,CGuiItemCell* pCell,float fElapsedTime);
	extern void TableRender(CGuiTable* pTable,float fElapsedTime);
	g_fnItemTableEvent = ItemTableEvent;
	g_fnItemRender = RoleItemRender;
	g_fnTableRender = TableRender;

	GetGuiManager()->Init();

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CGameGuiManager::Release()
{
	FUNCTION_BEGIN;

	GetCreateRoleManager()->Release();
	GetChatManager().RemoveAllChannels();

	if(m_guiFloatItem)
	{
		m_guiFloatItem->OnClose();
		delete m_guiFloatItem;
		m_guiFloatItem = NULL;
	}
	
	GetGuiManager()->CloseAllDialog();
	GetGuiManager()->SetMouseDlg(NULL);
	Init();
	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGameGuiFloatItem*	  CGameGuiManager::AddFloatItem()
{
	FUNCTION_BEGIN;

	if(!m_guiFloatItem)
	{
		m_guiFloatItem = new CGameGuiFloatItem;
		m_guiFloatItem->OnCreate();
	}
	return m_guiFloatItem;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGuiItem*			  CGameGuiManager::AddItem()
{
	FUNCTION_BEGIN;

	//第一次触发"打开包裹对话框"事件
	OnTipHelp(USER_SETTING_HELP_PACK);

	CMainCharacter* pMainChar = GetScene()->GetMainCharacter();
	if(!m_guiItem)
	{
		CGuiItem* pDlg = new CGuiItem;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\ItemDialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiItem = pDlg;
		
		/*CGuiEquip* pEquipDlg;
		pEquipDlg = AddEquip(pMainChar ? pMainChar->GetName() :  "",NULL);
		pEquipDlg->m_bCloseIsHide = true;
		if(pEquipDlg->GetCloseButton())
			pEquipDlg->GetCloseButton()->SetVisible( false );
		pDlg->m_pEquip = pEquipDlg;
		pEquipDlg->SetVisible(false);
		pEquipDlg->SetConnVisible(true);*/
	}

	m_guiItem->SetVisible(true);
	m_guiItem->SetEnabled(true);

	GetGameApplication()->m_bESCHook = true;
	return m_guiItem;

	FUNCTION_END;
}
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
CGuiHelp*			  CGameGuiManager::AddHelp(const char *pszHelpFileName)
{
	FUNCTION_BEGIN;

	if(!m_guiHelp)
	{
		CGuiHelp* pDlg = new CGuiHelp();
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\helpdialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiHelp = pDlg;
	}
	m_guiHelp->LoadFile(pszHelpFileName);
	m_guiHelp->SetEnabled(true);
	m_guiHelp->SetVisible(true);
	m_guiHelp->BerthShow();

	GetGameApplication()->m_bESCHook = true;
	return m_guiHelp;

	FUNCTION_END;
}


CGuiGmMsgBoxDialog* CGameGuiManager::AddGmMsgBoxDlg()
{
	FUNCTION_BEGIN;

	if(!m_guiGmMsgBoxDialog)
	{
		CGuiGmMsgBoxDialog* pDlg = new CGuiGmMsgBoxDialog();
		if( !pDlg ) return NULL;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\GMMsgBoardDialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiGmMsgBoxDialog = pDlg;
	}
	m_guiGmMsgBoxDialog->SetEnabled(true);
	m_guiGmMsgBoxDialog->SetVisible(true);

	GetGameApplication()->m_bESCHook = true;
	return m_guiGmMsgBoxDialog;

	FUNCTION_END;
}

CGuiEmotionDialog* CGameGuiManager::AddEmotionDlg()
{
	FUNCTION_BEGIN;

	if(!m_guiEmotionDlg)
	{
		CGuiEmotionDialog* pDlg = new CGuiEmotionDialog();
		if( !pDlg ) return NULL;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\emotion.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiEmotionDlg = pDlg;
	}
	m_guiEmotionDlg->SetEnabled(true);
	m_guiEmotionDlg->SetVisible(true);
//	m_guiEmotionDlg->BerthShow(); 

	GetGameApplication()->m_bESCHook = true;
	return m_guiEmotionDlg;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param Action : 描述
 * \return 返回值的描述
 */
CGuiBallot*  CGameGuiManager::AddBallot(enumGuiActionType Action)
{
	FUNCTION_BEGIN;

	if(!m_guiBallot)
	{
		CGuiBallot* pDlg = new CGuiBallot(Action);

		HRESULT hr;
		switch(Action) {
		case eAction_Tong:
			hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\tongBallotdialog.xml",pDlg);
			break;
		case eAction_Sept:
			hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\septBallotdialog.xml",pDlg);
			break;
		}
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiBallot = pDlg;
	}

	m_guiBallot->SetVisible(true);
	m_guiBallot->SetEnabled(true);
	return m_guiBallot;

	FUNCTION_END;
}


CGuiApplyConn*  CGameGuiManager::AddApplyConn()
{
	FUNCTION_BEGIN;

	if(!m_guiApplyConn)
	{
		CGuiApplyConn* pDlg = new CGuiApplyConn();

		HRESULT hr;
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\applyconndialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiApplyConn = pDlg;
	}

	m_guiApplyConn->SetVisible(true);
	m_guiApplyConn->SetEnabled(true);
	return m_guiApplyConn;

	FUNCTION_END;
}


CGuiMakeTo*	 CGameGuiManager::AddMakeTo1(CRoleItem* pRoleItem)
{
	FUNCTION_BEGIN;

	if(!m_guiMakeTo1)
	{
		CGuiMakeTo* pDlg = new CGuiMakeTo(pRoleItem);
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MakeToDialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiMakeTo1 = pDlg;

	}
	else
	{
		m_guiMakeTo1->InitTableInfo( pRoleItem );
		m_guiMakeTo1->RefreshTableInfo();
	}

	m_guiMakeTo1->SetVisible(true);
	m_guiMakeTo1->SetEnabled(true);


	return m_guiMakeTo1;

	FUNCTION_END;
}

CGuiMakeTo*	 CGameGuiManager::AddMakeTo2(CRoleItem* pRoleItem)
{
	FUNCTION_BEGIN;

	if(!m_guiMakeTo2)
	{
		CGuiMakeTo* pDlg = new CGuiMakeTo(pRoleItem);
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MakeToDialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiMakeTo2= pDlg;

	}
	else
	{
		m_guiMakeTo2->InitTableInfo( pRoleItem );
		m_guiMakeTo2->RefreshTableInfo();
	}

	m_guiMakeTo2->SetVisible(true);
	m_guiMakeTo2->SetEnabled(true);


	return m_guiMakeTo2;

	FUNCTION_END;
}

CGuiPackage*   CGameGuiManager::AddPackage1(CRoleItem* pRoleItem)
{
	FUNCTION_BEGIN;
	if(!m_guiPackage1)
	{
		CGuiPackage* pDlg = new CGuiPackage(pRoleItem);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\packagedialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiPackage1 = pDlg;
	}

	m_guiPackage1->SetVisible(true);
	m_guiPackage1->SetEnabled(true);
	return m_guiPackage1;

	FUNCTION_END;

}


CGuiPackage*   CGameGuiManager::AddPackage2(CRoleItem* pRoleItem)
{
	FUNCTION_BEGIN;
	if(!m_guiPackage2)
	{
		CGuiPackage* pDlg = new CGuiPackage(pRoleItem);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\packagedialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiPackage2 = pDlg;
	}

	m_guiPackage2->SetVisible(true);
	m_guiPackage2->SetEnabled(true);
	return m_guiPackage2;

	FUNCTION_END;
}

CGuiPackage*   CGameGuiManager::AddPackage(stObjectLocation pos,CRoleItem* pItem,bool bShow)
{
	if ((pos.dwLocation!=OBJECTCELLTYPE_EQUIP)||(pos.y!=EQUIPCELLTYPE_PACKAGE))
		return NULL;

	CGuiPackage* pDlg = NULL;
	switch(pos.x) {
	case EQUIPCELLTYPE_LEFT:
		pDlg = AddPackage1(pItem);
		break;
	case EQUIPCELLTYPE_RIGHT:
		pDlg = AddPackage2(pItem);
		break;
	}

	if (pDlg)
		pDlg->SetVisible(bShow);

	return pDlg;
}


CGuiChangeCountry *	 CGameGuiManager::AddChangeCountryDialog()
{
	FUNCTION_BEGIN;
	if(!m_guiChangeCountry)
	{
		CGuiChangeCountry* pDlg = new CGuiChangeCountry();
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\changecountrydialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiChangeCountry = pDlg;
	}

	m_guiChangeCountry->SetVisible(true);
	m_guiChangeCountry->SetEnabled(true);
	return m_guiChangeCountry;

	FUNCTION_END;
}

CGuiChangeWarField *	 CGameGuiManager::AddChangeWarFieldDialog()
{
	FUNCTION_BEGIN;
	if(!m_guiChangeWarField)
	{
		CGuiChangeWarField* pDlg = new CGuiChangeWarField(CGuiNpcDialog::eChangeWarField);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\changewarfielddialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiChangeWarField = pDlg;
	}

	m_guiChangeWarField->SetVisible(true);
	m_guiChangeWarField->SetEnabled(true);
	return m_guiChangeWarField;

	FUNCTION_END;
}

CGuiChangeWarField *	 CGameGuiManager::AddChoiceWarFieldDialog()
{
	FUNCTION_BEGIN;
	if(!m_guiChoiceWarField)
	{
		CGuiChangeWarField* pDlg = new CGuiChangeWarField(CGuiNpcDialog::eChoiceWarField);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\choicewarfielddialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiChoiceWarField = pDlg;
	}

	m_guiChoiceWarField->SetVisible(true);
	m_guiChoiceWarField->SetEnabled(true);
	return m_guiChoiceWarField;

	FUNCTION_END;
}

CGuiCountryWarQuery *	 CGameGuiManager::AddCountryWarQueryDialog()
{
	FUNCTION_BEGIN;
	if(!m_guiCountryWarQuery)
	{
		CGuiCountryWarQuery* pDlg = new CGuiCountryWarQuery();
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\countrywarquerydialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiCountryWarQuery = pDlg;
	}

	m_guiCountryWarQuery->SetVisible(true);
	m_guiCountryWarQuery->SetEnabled(true);
	return m_guiCountryWarQuery;

	FUNCTION_END;
}

CGuiUnoinPower*	 CGameGuiManager::AddUnoinPowerDialog(char* name)
{
	FUNCTION_BEGIN;
	if(!m_guiUnoinPower)
	{
		CGuiUnoinPower* pDlg = new CGuiUnoinPower(name);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\unoinpowerdialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiUnoinPower = pDlg;
	}

	m_guiUnoinPower->SetVisible(true);
	m_guiUnoinPower->SetEnabled(true);
	return m_guiUnoinPower;

	FUNCTION_END;
}

CGuiMakeTo*   CGameGuiManager::AddMakeTo(stObjectLocation pos,CRoleItem* pItem,bool bShow)
{
	if ((pos.dwLocation!=OBJECTCELLTYPE_EQUIP)||(pos.y!=EQUIPCELLTYPE_MAKE))
		return NULL;

	CGuiMakeTo* pDlg = NULL;
	switch(pos.x) {
	case EQUIPCELLTYPE_LEFT:
		pDlg = AddMakeTo1(pItem);
		break;
	case EQUIPCELLTYPE_RIGHT:
		pDlg = AddMakeTo2(pItem);
		break;
	}

	if (pDlg)
		pDlg->SetVisible(bShow);

	return pDlg;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiCharacterProperty* CGameGuiManager::AddCharacterProperty(bool bShow)
{
	FUNCTION_BEGIN;

	CGuiDialog * pCurFocusDlg = NULL;
	CGuiControl* pCurFocusControl = NULL;
	pCurFocusDlg = GetGuiManager()->GetFocusDialog();
	if ( pCurFocusDlg )
		pCurFocusControl = pCurFocusDlg->GetControlFocus();	

	if(!m_guiCharacterProperty)
	{
		CGuiCharacterProperty* pDlg = new CGuiCharacterProperty();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\CharacterProperty.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiCharacterProperty = pDlg;
	}
	m_guiCharacterProperty->SetEnabled(bShow);
	m_guiCharacterProperty->SetVisible(bShow);

	if ( pCurFocusDlg && pCurFocusControl )
	{
		GetGuiManager()->RequestFocus( pCurFocusDlg );
		pCurFocusDlg->SetFocus( pCurFocusControl );
	}

	GetGameApplication()->m_bESCHook = true;
	return m_guiCharacterProperty;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiWorldConn*  CGameGuiManager::AddWorldConn(CChatChannel* pChannel)
{
	FUNCTION_BEGIN;

	if(!m_guiWorldConn)
	{
		CGuiWorldConn* pDlg = new CGuiWorldConn;
		HRESULT  hr=E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\WorldConnDialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiWorldConn = pDlg;
	}
	m_guiWorldConn->SetChatChannel(pChannel);
	m_guiWorldConn->SetVisible(true);
	m_guiWorldConn->SetEnabled(true);
	GetGuiManager()->RequestFocus(m_guiWorldConn);
	return m_guiWorldConn;

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiWorldConn*  CGameGuiManager::AddNearby(CChatChannel* pChannel)
{
	FUNCTION_BEGIN;

	if(!m_guiNearby)
	{
		CGuiWorldConn* pDlg = new CGuiWorldConn;
		HRESULT  hr=E_FAIL; 
		Engine_WriteLogF("step 1\n");
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\nearbydialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiNearby = pDlg;

		m_guiNearby->OnGuiEvent(EVENT_BUTTON_CLICKED,NEARBYPAGE,NULL);
		m_guiNearby->GetButton(TONGBUTTON)->SetVisible(false);
		m_guiNearby->GetButton(FRIENDBUTTON)->SetVisible(false);
		m_guiNearby->GetButton(SEPTBUTTON)->SetVisible(false);
		m_guiNearby->GetImage(2)->SetVisible(false);
		m_guiNearby->GetCheckBox(1)->SetVisible(false);
	}
	m_guiNearby->SetChatChannel(pChannel);
	m_guiNearby->SetVisible(true);
	m_guiNearby->SetEnabled(true);

	GetGameApplication()->m_bESCHook = true;
	return m_guiNearby;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiLogin* CGameGuiManager::AddGuiLogin()
{
	FUNCTION_BEGIN;

	if(!m_guiLogin)
	{
		LoadLoginRes();

		extern void ResetLoginDevice();

		ResetLoginDevice();

		CGuiLogin* pDlg = new CGuiLogin;
		HRESULT hr;
		if( CPixelContext::IsLogin800By600())
			hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\logindialog800.xml",pDlg);
		else 
			hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\LoginDialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiLogin = pDlg;
	}
	m_guiLogin->SetVisible(true);
	m_guiLogin->SetEnabled(true);

	//Add login cloud effect
	if ( GetGameApplication() )
		GetGameApplication()->InitLoginCloud();

	return m_guiLogin;

	FUNCTION_END;
}

CGuiKeyBoardDlg* CGameGuiManager::AddKeyBoardDlg()
{
	FUNCTION_BEGIN;

	if(!m_guiKeyBoard)
	{
		CGuiKeyBoardDlg * pDlg = new CGuiKeyBoardDlg;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\GuiKeyBoard.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiKeyBoard = pDlg;
	}
	m_guiKeyBoard->SetVisible(true);
	m_guiKeyBoard->SetEnabled(true);

	//m_guiKeyBoard->SetZOrder(GUI_DIALOG_TOPMOST);

	return m_guiKeyBoard;

	FUNCTION_END;
}
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiMain*  CGameGuiManager::AddMain()
{
	FUNCTION_BEGIN;

	if(!m_guiMain)
	{
		CGuiMain* pDlg = new CGuiMain;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MainDialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiMain = pDlg;
	}

	m_guiMain->SetVisible(true);
	return m_guiMain;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiChatOutput* CGameGuiManager::AddChatOutput()
{
	FUNCTION_BEGIN;

	if(!m_guiChatOutput)
	{
		CGuiChatOutput* pDlg = new CGuiChatOutput;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\ChatOutputDialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiChatOutput = pDlg;
	}
	m_guiChatOutput->SetVisible(true);
	m_guiChatOutput->SetEnabled(true);
	return m_guiChatOutput;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiSystem* CGameGuiManager::AddSystem()
{
	FUNCTION_BEGIN;

	if(!m_guiSystem)
	{
		CGuiSystem * pDlg = new CGuiSystem;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\SystemDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiSystem = pDlg;
	}
	m_guiSystem->SetVisible(true);

	GetGameApplication()->m_bESCHook = true;
	return m_guiSystem;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
//Add item detail info dlg
CGuiItemDetailInfo* CGameGuiManager::AddItemDetailInfo()
{
	FUNCTION_BEGIN;

	if ( !m_guiItemDetailInfo )
	{
		CGuiItemDetailInfo* pDlg = new CGuiItemDetailInfo;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML( c_szGuiPack, "gui\\ItemDetailInfoDialog.xml", pDlg );
		if ( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}

		m_guiItemDetailInfo = pDlg;
	}

	m_guiItemDetailInfo->SetVisible( true );
	return m_guiItemDetailInfo;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiSelectCharDialog* CGameGuiManager::AddSelectCharDialog()
{
	FUNCTION_BEGIN;

	if(!m_guiSelectChar)
	{
		CGuiSelectCharDialog * pDlg = new CGuiSelectCharDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\SelectCharDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiSelectChar = pDlg;
	}
	m_guiSelectChar->SetVisible(true);
	return m_guiSelectChar;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiCreateCharDialog* CGameGuiManager::AddCreateCharDialog()
{
	FUNCTION_BEGIN;

	if(!m_guiCreateCharDialog)
	{
		CGuiCreateCharDialog * pDlg = new CGuiCreateCharDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\CreateCharDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiCreateCharDialog = pDlg;
	}
	m_guiCreateCharDialog->SetVisible(true);
	m_guiCreateCharDialog->SetEnabled(true);
	return m_guiCreateCharDialog;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述

CGuiRegisterDialog*   CGameGuiManager::AddRegisterDialog()
{
FUNCTION_BEGIN;

if(!m_guiRegisterDialog)
{
CGuiRegisterDialog * pDlg = new CGuiRegisterDialog;
HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\RegisterDialog.xml",pDlg);
if(FAILED(hr))
{
delete pDlg;
return NULL;
}
m_guiRegisterDialog = pDlg;
}
m_guiRegisterDialog->SetVisible(true);
m_guiRegisterDialog->SetEnabled(true);
return m_guiRegisterDialog;

FUNCTION_END;
}
*/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszText : 描述
* \return 返回值的描述
*/
static SIZE GetMessageBoxTextSize(const char* pszText)
{
	FUNCTION_BEGIN;

	const int maxTextWidth = 360;
	int iFont = GetDevice()->SetFont(1);
	SIZE sz = GetDevice()->GetStringSize(pszText,-1,maxTextWidth);
	GetDevice()->SetFont(iFont);
	return sz;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszText : 描述
* \param type : 描述
* \param pData : 描述
* \return 返回值的描述
*/
CGuiInputBox* CGameGuiManager::AddInputBox(const char* pszText,enumGuiActionType type,void* pData )
{
	FUNCTION_BEGIN;

	CGuiInputBox* pDlg = new CGuiInputBox(pszText,type,pData);
	HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\InputBoxDialog.xml",pDlg);
	if(FAILED(hr))
	{
		delete pDlg;
		return NULL;
	}
	return pDlg;

	FUNCTION_END;
}

CGuiInputBox2Dialog*  CGameGuiManager::AddInputBox2(const char* pszText1,const char* pszText2,enumGuiActionType type,void* pData)
{
	FUNCTION_BEGIN;

	GUIMANAGER_ADD_DIALOG4(CGuiInputBox2Dialog,m_guiInputBox2Dlg,"gui\\InputBox2Dialog.xml",pszText1,pszText2,type,pData);

	//if(!m_guiInputBox2Dlg)
	//{
	//	CGuiInputBox2Dialog* pDlg = new CGuiInputBox2Dialog(pszText1,pszText2,type,pData);
	//	HRESULT  hr = E_FAIL; 
	//	hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,,pDlg);

	//	if(FAILED(hr)) 
	//	{
	//		delete pDlg;
	//		return NULL;
	//	}
	//	m_guiInputBox2Dlg = pDlg;	
	//}
	//m_guiInputBox2Dlg->SetEnabled(true);
	//m_guiInputBox2Dlg->SetVisible(true);
	//return m_guiInputBox2Dlg;

	FUNCTION_END;
}

CGuiInputBoxFiveDialog* CGameGuiManager::AddInputBoxFiveDialog( const char* strTitle )
{
	FUNCTION_BEGIN;

	GUIMANAGER_ADD_DIALOG1( CGuiInputBoxFiveDialog, m_guiInputBoxFiveDlg, "gui\\InputBoxFiveDialog.xml", strTitle );

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszText : 描述
* \param type : 描述
* \param pData : 描述
* \return 返回值的描述
*/
CGuiMoneyInputBox* CGameGuiManager::AddMoneyInputBox(const char* pszText,enumGuiActionType type,void* pData )
{
	FUNCTION_BEGIN;

	CGuiMoneyInputBox* pDlg = new CGuiMoneyInputBox(pszText,type,pData);
	HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MoneyInputBoxDialog.xml",pDlg);
	if(FAILED(hr))
	{
		delete pDlg;
		return NULL;
	}
	return pDlg;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszCaption : 描述
* \param pszTitle : 描述
* \param pszText : 描述
* \param type : 描述
* \param bEdit : 描述
* \param pData : 描述
* \return 返回值的描述
*/
CGuiWorldBoard* CGameGuiManager::AddWorldBoard(const char* pszCaption,const char* pszTitle,const char* pszText,
											   enumGuiActionType type,bool bEdit,void* pData )
{
	FUNCTION_BEGIN;

	CGuiWorldBoard* pDlg = new CGuiWorldBoard(pszCaption,pszTitle,pszText,type,bEdit,pData);
	HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\WorldBoardDialog.xml",pDlg);
	if(FAILED(hr))
	{
		delete pDlg;
		return NULL;
	}
	return pDlg;

	FUNCTION_END;
}

CGuiChatInvateDialog* CGameGuiManager::AddChatInvate(void* pData)
{
	FUNCTION_BEGIN;

	if(!m_guiInvite)
	{
		CGuiChatInvateDialog * pDlg = new CGuiChatInvateDialog(CGuiChatInvateDialog::eChat);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\chatinvite.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiInvite = pDlg;
		m_guiInvite->SetCloseIsHide(true);
	}
	m_guiInvite->SetData(pData);
	m_guiInvite->SetVisible(true);
	m_guiInvite->SetEnabled(true);
	GetGuiManager()->RequestFocus(m_guiInvite);
	m_guiInvite->SetFocus((CGuiControl*)m_guiInvite->GetButton(2));

	GetGameApplication()->m_bESCHook = true;
	return m_guiInvite;

	FUNCTION_END;
}

CGuiChatInvateDialog* CGameGuiManager::AddTongInvate(void* pData )
{
	FUNCTION_BEGIN;


	CGuiChatInvateDialog * pDlg = new CGuiChatInvateDialog(CGuiChatInvateDialog::eTong);
	HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\tonginvite.xml",pDlg);
	if(FAILED(hr))
	{
		delete pDlg;
		return NULL;
	}
	pDlg->SetCloseIsHide(false);
	pDlg->SetData(pData);
	pDlg->SetVisible(true);
	pDlg->SetEnabled(true);
	return pDlg;

	FUNCTION_END;
}
CGuiChatInvateDialog* CGameGuiManager::AddSeptInvate(void* pData )
{
	FUNCTION_BEGIN;

	CGuiChatInvateDialog * pDlg = new CGuiChatInvateDialog(CGuiChatInvateDialog::eSept);
	HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\septinvite.xml",pDlg);
	if(FAILED(hr))
	{
		delete pDlg;
		return NULL;
	}
	pDlg->SetCloseIsHide(false);
	pDlg->SetData(pData);
	pDlg->SetVisible(true);
	pDlg->SetEnabled(true);
	return pDlg;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszText : 描述
* \param type : 描述
* \param pData : 描述
* \param pszButtons : 描述
* \param bModal : 描述
* \return 返回值的描述
*/
CGuiMessageBox* CGameGuiManager::AddMessageBox(const char* pszText,enumGuiActionType type,void* pData,const char* pszButtons,bool bModal)
{
	CGuiMessageBox* retval;

	FUNCTION_BEGIN_NEW;
	HRESULT hr;
	SetFrameAllocator fa;
	std::vector<CGuiButton*, std::stack_allocator<CGuiButton* > > vBtn;
	std::vector<std::stack_string, std::stack_allocator<std::stack_string> > results;
	const int cxBorder = 60;
	const int topBorder = 10;
	const int bottomBorder = 60;
	const int nCaptionHeight = 0;
	SIZE textSize = GetMessageBoxTextSize(pszText);

	SIZE boxSize = {cxBorder*2+textSize.cx ,topBorder + bottomBorder + textSize.cy+nCaptionHeight };
	CGuiMessageBox* pMsgBox = new CGuiMessageBox(type,pData);

	hr = GetGuiManager()->AddDialog(GetGuiManager()->GenerateDialogID(),
		0,0,
		boxSize.cx + CGuiDialog::s_smClientOffsetTopLeft.x + CGuiDialog::s_smClientOffsetRightBottom.x,
		boxSize.cy + CGuiDialog::s_smClientOffsetTopLeft.y + CGuiDialog::s_smClientOffsetRightBottom.y,
		GUI_WS_TOPMOST | (bModal ? GUI_WS_MODAL : 0),"","",pMsgBox);
	if(FAILED(hr))
	{
		delete pMsgBox;
		RETURN( NULL );
	}

	//rpattern pat("\\|");


	_parse_str(results,pszButtons,'|');
	//pat.split(pszButtons,results);

	CGuiStatic* pStatic;
	pMsgBox->AddStatic(1000,pszText,cxBorder,topBorder,textSize.cx,textSize.cy,false,&pStatic);
	pStatic->SetTextColor(COLOR_ARGB(255,204,165,10));

	//-------------------------------------------------------------------------
	// 确定按钮位置
	//-------------------------------------------------------------------------

	int y0 = topBorder + textSize.cy + 20;
	int totalWidth = 0;

	for(size_t i=0;i<results.size();i+=3)
	{
		if( i+2 >= results.size()) break;

		int id = strtoul(results[i+1].c_str(),NULL,10);


		CGuiButton * pBtn = NULL;
		pMsgBox->AddButton(id,results[i].c_str(),0,y0,100,100,results[i+2].c_str(),false,&pBtn);

		//---------------------------------------------------------------------------
		extern int g_defaultButtonClickSound;
		pBtn->SetSoundIndex(g_defaultButtonClickSound);
		//---------------------------------------------------------------------------

		vBtn.push_back(pBtn);
		totalWidth += pBtn->GetWidth();
	}

	int nBtnSpace = 0;
	if( vBtn.size() == 2) nBtnSpace = 20;
	else if(vBtn.size() == 3) nBtnSpace = 15 ;
	else nBtnSpace = 8 ;

	int x0 = ( boxSize.cx - (totalWidth + ((int)vBtn.size() - 1) * nBtnSpace) )/2;

	int x = x0;
	for(size_t i=0;i<vBtn.size();++i)
	{
		SIZE sz = CGuiButton::CalcuSize(vBtn[i]->GetText());

		vBtn[i]->SetLocation(x,y0);

		x += nBtnSpace;
		x += vBtn[i]->GetWidth();
	}
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	pMsgBox->SetAlign((GUI_ALIGN_TYPE)(GUI_ALIGN_VCENTER | GUI_ALIGN_HCENTER));
	RETURN(pMsgBox);

_ret:
	;
	FUNCTION_END_NEW
	return retval;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiUserInfo* CGameGuiManager::AddUserInfo(int nTABId)
{
	FUNCTION_BEGIN;

	if(!m_guiUserInfo)
	{
		CGuiUserInfo * pDlg = new CGuiUserInfo;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\UserInfoDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiUserInfo = pDlg;
	}
	m_guiUserInfo->RefreshButtonState();
	m_guiUserInfo->SetVisible(true);
	m_guiUserInfo->SetEnabled(true);
		
	switch(nTABId) {
	case 0:
		{
			m_guiUserInfo->GetTab(7)->SetCurItem(0);
			m_guiUserInfo->GetButton(5)->SetChecked(true);
			m_guiUserInfo->SetBackImage(stResourceLocation("data\\interfaces.gl",3,0));
			m_guiUserInfo->RefreshButtonState();
		}
		break;
	case 1:
		{
			m_guiUserInfo->GetTab(7)->SetCurItem(1);
			m_guiUserInfo->GetButton(6)->SetChecked(true);
			m_guiUserInfo->SetBackImage(stResourceLocation("data\\interfaces.gl",3,57));
			m_guiUserInfo->RefreshButtonState();
		}
		break;
	default:
		;
	}
	

	GetGameApplication()->m_bESCHook = true;
	return m_guiUserInfo;

	FUNCTION_END;
}


CGuiUserInfo* CGameGuiManager::AddUserInfoShowDlg()
{
	FUNCTION_BEGIN;

	if(!m_guiUserInfoShow)
	{
		CGuiUserInfo * pDlg = new CGuiUserInfo(false);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\UserInfoDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiUserInfoShow = pDlg;
		m_guiUserInfoShow->SetLocation(0,200);
	}
	m_guiUserInfoShow->RefreshButtonState();
	m_guiUserInfoShow->SetVisible(true);
	m_guiUserInfoShow->SetEnabled(true);
	return m_guiUserInfoShow;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiNpcDialog*	CGameGuiManager::AddNpcDialog()
{
	FUNCTION_BEGIN;

	if(!m_guiNpcDialog)
	{
		CGuiNpcDialog * pDlg = new CGuiNpcDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\NpcDialog.xml",pDlg);
		if(FAILED(hr))
		{
			//delete pDlg;
			return NULL;
		}
		m_guiNpcDialog = pDlg;
	}
	m_guiNpcDialog->SetVisible(true);
	m_guiNpcDialog->SetEnabled(true);
	return m_guiNpcDialog;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiTrade*	CGameGuiManager::AddTrade()
{
	FUNCTION_BEGIN;

	if(!m_guiTrade)
	{
		CGuiTrade * pDlg = new CGuiTrade;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\TradeDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiTrade = pDlg;
	}
	m_guiTrade->SetVisible(true);
	m_guiTrade->SetEnabled(true);
	return m_guiTrade;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszMsg : 描述
* \param color : 描述
* \return 返回值的描述
*/
void CGameGuiManager::AddServerSystemMessage(const char* pszMsg,DWORD color)
{
	DEBUG_INT_AT_FUNCTION_BEGIN()
	if(m_guiMain)
	{
		m_guiMain->AddMessage(pszMsg,CGuiMain::SystemMessage,color);
	}
	DEBUG_INT_AT_FUNCTION_END
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pszMsg : 描述
* \param color : 描述
* \return 返回值的描述
*/
void CGameGuiManager::AddClientSystemMessage(const char* pszMsg,DWORD color)
{
	DEBUG_INT_AT_FUNCTION_BEGIN()

	if(m_guiMain)
	{
		m_guiMain->AddMessage(pszMsg,CGuiMain::SystemMessage,color);
	}

	DEBUG_INT_AT_FUNCTION_END;
}

void CGameGuiManager::AddClientTaskMessage(const char* pszMsg,DWORD color )
{
	DEBUG_INT_AT_FUNCTION_BEGIN()

	if(m_guiMain)
	{
		m_guiMain->AddMessage(pszMsg,CGuiMain::TaskMessage,color);
	}

	DEBUG_INT_AT_FUNCTION_END;
}

void CGameGuiManager::AddClientSysScrollMessage(const char * pszMsg,DWORD color)
{
	DEBUG_INT_AT_FUNCTION_BEGIN()

	if(m_guiMain)
	{
		m_guiMain->AddMessage(pszMsg,CGuiMain::ScrollMessage,color);
	}

	DEBUG_INT_AT_FUNCTION_END;
}

void CGameGuiManager::AddClientChatMessage(const char* pszMsg,DWORD color)
{
	DEBUG_INT_AT_FUNCTION_BEGIN()

	if(m_guiChatOutput)
		m_guiChatOutput->AddMessage(pszMsg,color);

	DEBUG_INT_AT_FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param bRepair : 描述
* \return 返回值的描述
*/
CGuiNpcTrade*		  CGameGuiManager::AddNpcTrade(DWORD dwRepair)
{
	FUNCTION_BEGIN;

	if(!m_guiNpcTrade)
	{
		CGuiNpcTrade * pDlg = new CGuiNpcTrade;
		HRESULT hr;
		if(dwRepair!=CGuiNpcDialog::eTrade)
			hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\npcrepairtradedialog.xml",pDlg);
		else
			hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\NpcTradeDialog.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiNpcTrade = pDlg;
	}
	m_guiNpcTrade->SetRepair(dwRepair);
	m_guiNpcTrade->RefreshRepairMoney();
	m_guiNpcTrade->SetVisible(true);
	m_guiNpcTrade->SetEnabled(true);
	return m_guiNpcTrade;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGameGuiManager::EndNpcDialog()
{
	FUNCTION_BEGIN;

	if(m_guiGoldExchangeDlg) m_guiGoldExchangeDlg->Close();
	if(m_guiAuctionDlg) m_guiAuctionDlg->Close();
	if(m_guiRecvMailDlg) m_guiRecvMailDlg->Close();
	if(m_guiSendMailDlg) m_guiSendMailDlg->Close();
	if(m_guiHairStyleDlg) m_guiHairStyleDlg->Close();
	if(m_guiHeadListDlg) m_guiHeadListDlg->Close();
	if(m_guiClothColorDlg) m_guiClothColorDlg->Close();
	if(m_guiNpcTrade) m_guiNpcTrade->Close();
	if(m_guiNpcMake) m_guiNpcMake->Close();
	if(m_guiBallot) m_guiBallot->Close();
	if(m_guiSavingBoxDlg) m_guiSavingBoxDlg->SetVisible(false);
	if(m_guiSaveBoxContinueDlg) m_guiSaveBoxContinueDlg->Close();
	if(m_guiNpcDialog) m_guiNpcDialog->Close();

	GetGameState()->SetStateItem(eGameStateItem_Normal);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGameGuiManager::CloseAllModalDialog()
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
CGuiMiniMap* CGameGuiManager::AddMiniMap()
{
	FUNCTION_BEGIN;

	if(!m_guiMiniMap)
	{
		CGuiMiniMap* pDlg = new CGuiMiniMap;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MiniMap.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiMiniMap = pDlg;
		//pDlg->SetLocation(0,GetDevice()->GetHeight() - 300);
	}
	m_guiMiniMap->SetVisible(true);
	m_guiMiniMap->SetEnabled(true);
	return m_guiMiniMap;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiTeam* CGameGuiManager::AddTeam()
{
	FUNCTION_BEGIN;

	if(!m_guiTeam)
	{
		CGuiTeam* pDlg = new CGuiTeam;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\TeamDialog.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiTeam = pDlg;
	}
	m_guiTeam->SetVisible(true);
	m_guiTeam->SetEnabled(true);
	return m_guiTeam;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiSkill* CGameGuiManager::AddSkill(bool bShow)
{
	FUNCTION_BEGIN;
	if(!m_guiSkill)
	{
		CGuiSkill* pDlg = new CGuiSkill;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\SkillTreeDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiSkill = pDlg;
	}
	m_guiSkill->SetVisible(bShow);
	m_guiSkill->SetEnabled(bShow);

	GetGameApplication()->m_bESCHook = true;
	return m_guiSkill;

	FUNCTION_END;
}

void ResetDevice()
{
	int nWidth = GetDevice()->GetWidth();
	int nHeight = GetDevice()->GetHeight();
	if(nWidth < 1024 && nHeight < 768)
	{
               if(nWidth == 800 && nHeight == 600)
	       {}
	       else 
			GetDevice()->Reset(800,600,GetDevice()->GetPixelBits(),GetDevice()->GetFlags());
	}
}

CGuiCompetition*    CGameGuiManager::AddCompetition(byte eState)
{
	FUNCTION_BEGIN;

	if (m_guiCompetition)
	{
		m_guiCompetition->Close();
	}

	if(!m_guiCompetition)
	{
		ResetDevice();

		std::string strXmlFile;
		CGuiCompetition* pDlg = new CGuiCompetition(eState);
		if (GetDevice()->GetWidth()==800)
		{
			switch(eState) {
			case CGuiNpcDialog::eCompetitionNpc:
				strXmlFile = "gui\\CompetitionNpcdialog800600.xml";
				break;
			case CGuiNpcDialog::eCompetition:
				strXmlFile = "gui\\Competitiondialog800600.xml";
				break;
			}
		}
		else
		{
			switch(eState) {
			case CGuiNpcDialog::eCompetitionNpc:
				strXmlFile = "gui\\CompetitionNpcdialog.xml";
				break;
			case CGuiNpcDialog::eCompetition:
				strXmlFile = "gui\\Competitiondialog.xml";
				break;
			}
		}
		
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,strXmlFile.c_str(),pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiCompetition = pDlg;
	}

	m_guiCompetition->SetVisible(true);
	m_guiCompetition->SetEnabled(true);
	return m_guiCompetition;

	FUNCTION_END;
}
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param eState : 描述
* \return 返回值的描述
*/
CGuiNpcMake*  CGameGuiManager::AddNpcMake(byte eState)
{
	FUNCTION_BEGIN;

	if(!m_guiNpcMake)
	{
		std::string strXmlFile;
		CGuiNpcMake* pDlg = new CGuiNpcMake(eState);
		switch(eState) {
		case CGuiNpcDialog::eMake:
			strXmlFile = "gui\\itemmakedialog.xml";
			break;
		case CGuiNpcDialog::eChange:
			strXmlFile = "gui\\itemchangedialog.xml";
			break;
		case CGuiNpcDialog::eUp:
			strXmlFile = "gui\\itemupdialog.xml";
			break;
		case CGuiNpcDialog::eKaChange:
			strXmlFile = "gui\\itemkachangedialog.xml";
			break;
		case CGuiNpcDialog::eKaMake:
			strXmlFile = "gui\\itemkamakedialog.xml";
			break;
		case CGuiNpcDialog::eHoleMake:
			strXmlFile = "gui\\itemholemakedialog.xml";
			break;
		case CGuiNpcDialog::eBackout:
			strXmlFile = "gui\\itembackoutdialog.xml";
			break;
		case CGuiNpcDialog::eMakeFood:
			strXmlFile = "gui\\itemmakefooddialog.xml";
			break;
		case CGuiNpcDialog::eMakeDrug:
			strXmlFile = "gui\\itemmakedrugdialog.xml";
			break;
		}
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,strXmlFile.c_str(),pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiNpcMake = pDlg;
	}
	m_guiNpcMake->SetVisible(true);
	m_guiNpcMake->SetEnabled(true);
	return m_guiNpcMake;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param name : 描述
* \param pData : 描述
* \return 返回值的描述

CGuiEquip*	CGameGuiManager::AddEquip( const char* name,void* pData )
{
FUNCTION_BEGIN;

CGuiEquip* pDlg = FindEquip(name);

if( pDlg == NULL )
{
pDlg = new CGuiEquip;
HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\equipdialog.xml",pDlg);
if(FAILED(hr))
{
delete pDlg;
return NULL;
}
}

pDlg->SetConnVisible(false);
pDlg->SetEquipData(pData);
pDlg->SetCharName(name);
return pDlg;

FUNCTION_END;
} */

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param name : 描述
* \return 返回值的描述

CGameGuiManager::tListEquip::iterator CGameGuiManager::FindEquipIt(const char* name)
{
FUNCTION_BEGIN;

for(tListEquip::iterator it = m_aGuiEquip.begin(); it != m_aGuiEquip.end(); ++it)
{
if(strcmp((*it)->GetCharName() ,name) == 0)
return it;
}
return m_aGuiEquip.end();

FUNCTION_END;
}
*/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param name : 描述
* \return 返回值的描述

CGuiEquip*	CGameGuiManager::FindEquip(const char* name)
{
FUNCTION_BEGIN;

tListEquip::iterator it = FindEquipIt(name);
if(it == m_aGuiEquip.end()) return NULL;
return *it;

FUNCTION_END;
}
*/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pDlg : 描述
* \return 返回值的描述

void CGameGuiManager::RemoveEquip(CGuiEquip* pDlg)
{
FUNCTION_BEGIN;

for(size_t i=0;i<m_aGuiEquip.size();++i)
{
if(m_aGuiEquip[i] == pDlg)
{
SimpleRemoveFromArray(m_aGuiEquip,i);
break;
}
}

FUNCTION_END;
}
*/
/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiFloatControl*	 CGameGuiManager::AddFloatControl()
{
	FUNCTION_BEGIN;

	if(!m_guiFloatControl)
	{
		CGuiFloatControl* pDlg = new CGuiFloatControl;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\floatmaindialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiFloatControl = pDlg;
	}
	m_guiFloatControl->SetVisible(true);
	m_guiFloatControl->SetEnabled(true);
	return m_guiFloatControl;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiFloatAccessKey*		CGameGuiManager::AddFloatAccessKey()
{
	FUNCTION_BEGIN;

	if(!m_guiFloatAcessKey)
	{
		CGuiFloatAccessKey* pDlg = new CGuiFloatAccessKey;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\FloatAccessKeyDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiFloatAcessKey = pDlg;
	}
	m_guiFloatAcessKey->SetVisible(true);
	m_guiFloatAcessKey->SetEnabled(true);
	return m_guiFloatAcessKey;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiShortcutItem*	 CGameGuiManager::AddShortcutItem()
{
	FUNCTION_BEGIN;

	if(!m_guiShortcutItem)
	{
		CGuiShortcutItem* pDlg = new CGuiShortcutItem;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\shortcutitem.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiShortcutItem = pDlg;
	}
	m_guiShortcutItem->SetVisible(true);
	m_guiShortcutItem->SetEnabled(true);
	return m_guiShortcutItem;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiChatSelect*		 CGameGuiManager::AddChatSelect(bool bShow)
{
	FUNCTION_BEGIN;

	if(!m_guiChatSelect)
	{
		CGuiChatSelect* pDlg = new CGuiChatSelect;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\chatselect.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiChatSelect = pDlg;
		AddChatButton()->EndPopup();;
	}
	m_guiChatSelect->SetVisible(bShow);
	m_guiChatSelect->SetEnabled(bShow);
	return m_guiChatSelect;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiFace*  CGameGuiManager::AddFace()
{
	FUNCTION_BEGIN;

	if(!m_guiFace)
	{
		CGuiFace* pDlg = new CGuiFace;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\FaceDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiFace = pDlg;
	}
	m_guiFace->SetVisible(false);
	m_guiFace->SetEnabled(true);
	return m_guiFace;

	FUNCTION_END;
}


CGuiChatSelectButton*  CGameGuiManager::AddChatButton()
{
	FUNCTION_BEGIN;

	if(!m_guiChatButton)
	{
		CGuiChatSelectButton* pDlg = new CGuiChatSelectButton;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\ChatButtonDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiChatButton = pDlg;
	}
	m_guiChatButton->SetVisible(true);
	m_guiChatButton->SetEnabled(true);
	return m_guiChatButton;

	FUNCTION_END;
}


CGuiPrivateChatSelect*  CGameGuiManager::AddPrivateChatSelect()
{
	FUNCTION_BEGIN;

	if(!m_guiPrivateChatSelect)
	{
		CGuiPrivateChatSelect* pDlg = new CGuiPrivateChatSelect;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\privatechatselectDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiPrivateChatSelect = pDlg;
	}
	m_guiPrivateChatSelect->SetVisible(false);
	m_guiPrivateChatSelect->SetEnabled(true);
	return m_guiPrivateChatSelect;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiSysChat*  CGameGuiManager::AddSysChat()
{
	FUNCTION_BEGIN;

	if(!m_guiSysChat)
	{
		CGuiSysChat* pDlg = new CGuiSysChat;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\SysChatDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiSysChat = pDlg;
	}
	m_guiSysChat->SetVisible(false);
	m_guiSysChat->SetEnabled(false);
	return m_guiSysChat;

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pt : 描述
* \return 返回值的描述
*/
CMakeGuess*  CGameGuiManager::AddMakeGuess(stPointI pt)
{
	FUNCTION_BEGIN;

	if(!m_guiMakeGuess)
	{
		CMakeGuess* pDlg = new CMakeGuess(pt);
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\FaceDialog.xml",pDlg);
		if( FAILED( hr ) )
		{
			delete pDlg;
			return NULL;
		}
		m_guiMakeGuess = pDlg;
	}
	m_guiMakeGuess->SetRenderPt(pt);
	m_guiMakeGuess->SetVisible(false);
	m_guiMakeGuess->SetEnabled(true);
	return m_guiMakeGuess;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void	CGameGuiManager::OnNpcActionEnd()
{
	FUNCTION_BEGIN;

	if(m_guiNpcDialog)
		m_guiNpcDialog->OnActionEnd();

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiSavingBoxDialog*			  CGameGuiManager::AddSavingBoxDlg()
{
	FUNCTION_BEGIN;

	if(!m_guiSavingBoxDlg)
	{
		CGuiSavingBoxDialog* pDlg = new CGuiSavingBoxDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\savingbox.xml",pDlg);
		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiSavingBoxDlg = pDlg;
	}

	m_guiSavingBoxDlg->SetVisible(true);
	m_guiSavingBoxDlg->SetEnabled(true);
	return m_guiSavingBoxDlg;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiLageMap* CGameGuiManager::AddLageMap()
{
	FUNCTION_BEGIN;

	if(!m_guiLageMap)
	{
		CGuiLageMap * pDlg = new CGuiLageMap;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\LageMap.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiLageMap = pDlg;
	}

	GetGameApplication()->m_bESCHook = true;
	return m_guiLageMap;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void CGameGuiManager::InitMainCharacterInfo()
{
	FUNCTION_BEGIN;

	if(!m_bInitMainCharacterInfo)
	{
		m_guiTrade->InitIcon();
		m_guiUserInfo->InitIcon();
		m_guiMain->InitIcon();

		//Here we only init PkMode list one time
		//cause the PK MODE in main character attribute is sometimes wrong later
		GetScene()->GetMainCharacter()->InitPKMode();
		m_guiMain->ChangeAttackModeBtnTips();		

		//m_guiMain->RefreshKeyGroupState( m_guiMain->GetCurAccessKeyGroup() );
		m_guiFloatAcessKey->RefreshKeyGroupState();

		m_guiSkill->RefreshState();

		if ( m_guiItem )
		{
			m_guiItem->SetGoldTicket( GetScene()->GetMainCharacter()->GetProperty()->gold,
				GetScene()->GetMainCharacter()->GetProperty()->ticket );
		}

		//Refresh maker equipment special effect
		//GetScene()->GetMainCharacter()->RefreshActiveItem( OBJECTCELLTYPE_EQUIP );

		m_bInitMainCharacterInfo = true;
	}

	FUNCTION_END;
}

/**
* \brief 添加发型对话框
* 
* 跟发型师npc对话,供玩家换发型、发色对话框界面
* 
* \return CGuiHairStyleDialog对话框指针 打开失败NULL
*/
CGuiHairStyleDialog *CGameGuiManager::AddHairDlg()
{
	FUNCTION_BEGIN;

	if( !m_guiHairStyleDlg )
	{
		CGuiHairStyleDialog * pDlg = new CGuiHairStyleDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\HairStyle.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiHairStyleDlg = pDlg;
	}

	m_guiHairStyleDlg->SetVisible(true);
	m_guiHairStyleDlg->SetEnabled(true);
	return m_guiHairStyleDlg;

	FUNCTION_END;
}

/**
* \brief 修改头像对话框
* 
* 跟npc对话,供玩家换头像对话框界面
* 
*/
CGuiHeadListDialog *CGameGuiManager::AddHeadListDlg()
{
	FUNCTION_BEGIN;

	if( !m_guiHeadListDlg )
	{
		CGuiHeadListDialog * pDlg = new CGuiHeadListDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\HeadList.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiHeadListDlg = pDlg;
	}

	m_guiHeadListDlg->SetVisible(true);
	m_guiHeadListDlg->SetEnabled(true);
	return m_guiHeadListDlg;

	FUNCTION_END;
	return NULL;
}

/**
* \brief 添加服装换色对话框
* 
* 跟服装师npc对话,供玩家换服装颜色
* 
* \return CGuiClothColorDialog对话框指针 打开失败NULL
*/
CGuiClothColorDialog *CGameGuiManager::AddClothColorDlg()
{
	FUNCTION_BEGIN;

	if( !m_guiClothColorDlg )
	{
		CGuiClothColorDialog * pDlg = new CGuiClothColorDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\ClothColor.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guiClothColorDlg = pDlg;
	}

	m_guiClothColorDlg->SetVisible(true);
	m_guiClothColorDlg->SetEnabled(true);
	return m_guiClothColorDlg;

	FUNCTION_END;
}

/**
* \brief 添加任务显示对话框
* 
* \return CGuiTaskDialog对话框指针 打开失败NULL
*/
CGuiTaskDialog * CGameGuiManager::AddTaskDialog()
{
	FUNCTION_BEGIN;

	if( !m_guitaskdialog )
	{
		CGuiTaskDialog * pDlg = new CGuiTaskDialog;
		HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\task.xml",pDlg);
		if(FAILED(hr))
		{
			delete pDlg;
			return NULL;
		}
		m_guitaskdialog = pDlg;
		m_guitaskdialog->UpdateTaskContent();
	}

	m_guitaskdialog->SetVisible(true);
	m_guitaskdialog->SetEnabled(true);

	//向服务器发任务王检索命令
	stReqValidQuestUserCmd	cmd;
	SEND_USER_CMD(cmd);
	TRACE("____检索任务王命令___wyc");

	GetGameApplication()->m_bESCHook = true;
	return m_guitaskdialog;

	FUNCTION_END;
}

/**
* \brief 添加摆摊对话框
* 
* \return CGuiShopDialog对话框指针 打开失败NULL
*/
CGuiShopDialog*	CGameGuiManager::AddShop(bool bShow)
{
	FUNCTION_BEGIN;

	if( NULL == m_guiShop )
	{
		CGuiShopDialog* pDlg = new CGuiShopDialog(CGuiShopDialog::ShopType::MyShop);
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\shop.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiShop = pDlg;	
	}
	m_guiShop->SetEnabled(bShow);
	m_guiShop->SetVisible(bShow);

    GetGameApplication()->m_bESCHook = true;
	return m_guiShop;

	FUNCTION_END;
}

CGuiOthersShopDialog*		CGameGuiManager::AddOthersShop(bool bShow)
{
	FUNCTION_BEGIN;

	if( NULL == m_guiOthersShop )
	{
		CGuiOthersShopDialog* pDlg = new CGuiOthersShopDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\othersshop.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiOthersShop = pDlg;	
	}
	m_guiOthersShop->SetEnabled(bShow);
	m_guiOthersShop->SetVisible(bShow);
	return m_guiOthersShop;

	FUNCTION_END;
}

/**
* \brief 添加重生对话框
* 
* \param dwTempUserID : 角色临时id
* \return CGuiReliveDlg对话框指针 打开失败NULL
*/
CGuiReliveDlg *CGameGuiManager::AddReliveDlg(DWORD dwTempUserID,DWORD dwDeadType)
{
	FUNCTION_BEGIN;

	if( m_guiNationalMap && m_guiNationalMap->IsVisible() )
	{
		m_guiNationalMap->Close();
	}

	GUIMANAGER_ADD_DIALOG2(CGuiReliveDlg,m_guiReliveDlg,"gui\\relive.xml",dwTempUserID,dwDeadType);

	FUNCTION_END;
}

CGuiSelectMarryTypeDlg* CGameGuiManager::AddSelectMarryTypeDlg()
{
	FUNCTION_BEGIN;

	CGuiSelectMarryTypeDlg* pDlg = new CGuiSelectMarryTypeDlg();
	HRESULT hr = GetGuiManager()->LoadGuiFromXML( c_szGuiPack, "gui\\SelectMarryType.xml", pDlg );
	if ( FAILED(hr) )
	{
		delete pDlg;
		return NULL;
	}

	m_guiSelectMarryTypeDlg = pDlg;
	return m_guiSelectMarryTypeDlg;

	FUNCTION_END;
}

CGuiSelectSeptDareTypeDlg* CGameGuiManager::AddSelectSeptDareTypeDlg()
{
	FUNCTION_BEGIN;

	CGuiSelectSeptDareTypeDlg* pDlg = new CGuiSelectSeptDareTypeDlg();
	HRESULT hr = GetGuiManager()->LoadGuiFromXML( c_szGuiPack, "gui\\SelectSeptDareTypeDialog.xml", pDlg );
	if ( FAILED(hr) )
	{
		delete pDlg;
		return NULL;
	}

	m_guiSelectSeptDareTypeDlg = pDlg;
	return m_guiSelectSeptDareTypeDlg;

	FUNCTION_END;
}


/**
* \brief 添加颜色选择对话框
* 
* 根据类型、初始颜色打开颜色对话框
* 
* \param Action : 打开类型
* \param color : 初始颜色
* \param pData : 用于回传消息到打开颜色对话框的请求者
* \return CGuiColorDialog对话框指针 打开失败NULL
*/
CGuiColorDialog* CGameGuiManager::AddColorDialog(enumGuiActionType Action,DWORD color,void *pData)
{
	FUNCTION_BEGIN;

	CGuiColorDialog* pDlg = new CGuiColorDialog(Action,pData);
	HRESULT hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\colordialog.xml",pDlg);
	if( FAILED( hr ) )
	{
		delete pDlg;
		return NULL;
	}
	pDlg->SetDefaultColor(color);
	pDlg->SetVisible(true);
	pDlg->SetEnabled(true);
	return pDlg;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiPetDialog* CGameGuiManager::AddPetDialog()
{
	FUNCTION_BEGIN;

	if(!m_guiPetDlg)
	{
		CGuiPetDialog* pDlg = new CGuiPetDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\pet.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiPetDlg = pDlg;	
	}
	m_guiPetDlg->SetEnabled(true);
	m_guiPetDlg->SetVisible(true);

	GetGameApplication()->m_bESCHook = true;
	return m_guiPetDlg;

	FUNCTION_END;
}

//发件箱
CGuiSendMailDialog* CGameGuiManager::AddSendMailDlg(bool bShow)
{
	FUNCTION_BEGIN;

	if(!m_guiSendMailDlg)
	{
		CGuiSendMailDialog* pDlg = new CGuiSendMailDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\sendmaildialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiSendMailDlg = pDlg;	
	}
	m_guiSendMailDlg->SetEnabled(bShow);
	m_guiSendMailDlg->SetVisible(bShow);
	return m_guiSendMailDlg;

	FUNCTION_END;
}

//收件箱
CGuiRecvMailDialog* CGameGuiManager::AddRecvMailDlg(bool bShow)
{
	FUNCTION_BEGIN;

	CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
	if( pMainRole )
	{
		pMainRole->GetMailManager()->RequestMailIitle();
	}

	if(!m_guiRecvMailDlg)
	{
		CGuiRecvMailDialog* pDlg = new CGuiRecvMailDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\recvmaildialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiRecvMailDlg = pDlg;	
	}
	m_guiRecvMailDlg->SetEnabled(bShow);
	m_guiRecvMailDlg->SetVisible(bShow);
	return m_guiRecvMailDlg;

	FUNCTION_END;
}

//礼品发放对话框
CGuiGiveGiftDialog* CGameGuiManager::AddGiveGiftDlg(bool bShow)
{
	FUNCTION_BEGIN;

	if(!m_guiGiveGiftDlg)
	{
		CGuiGiveGiftDialog* pDlg = new CGuiGiveGiftDialog();
		HRESULT  hr = E_FAIL; 
		
		//D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml -->this is my local dir (wyc)

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\givegiftDialog.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiGiveGiftDlg = pDlg;	
	}
	m_guiGiveGiftDlg->SetEnabled(bShow);
	m_guiGiveGiftDlg->SetVisible(bShow);
	return m_guiGiveGiftDlg;

	FUNCTION_END;
}

//////////////////////////////////////////////////////////////////////////
//推荐人系统
//////////////////////////////////////////////////////////////////////////
//#define  _WYC_TEST_//注意关闭
CGuiRecoSys_A_InputDlg* CGameGuiManager::AddRecoSys_A_InputDlg(bool bShow /* = true */)
{
	FUNCTION_BEGIN;

	if(!m_guiRecoSys_A_InputDlg)
	{
		CGuiRecoSys_A_InputDlg* pDlg = new CGuiRecoSys_A_InputDlg();
		HRESULT  hr = E_FAIL; 

		//D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml -->this is my local dir (wyc)

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\RecoSys_A_InputDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\RecoSys_A_InputDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiRecoSys_A_InputDlg = pDlg;	
	}
	m_guiRecoSys_A_InputDlg->SetEnabled(bShow);
	m_guiRecoSys_A_InputDlg->SetVisible(bShow);
	return m_guiRecoSys_A_InputDlg;

	FUNCTION_END;
}

CGuiRecoSys_B_Query_TakeDlg* CGameGuiManager::AddRecoSys_B_Query_TakeDlg(bool bShow /* = true */)
{
	FUNCTION_BEGIN;

	if(!m_guiRecoSys_B_Query_TakeDlg)
	{
		CGuiRecoSys_B_Query_TakeDlg* pDlg = new CGuiRecoSys_B_Query_TakeDlg();
		HRESULT  hr = E_FAIL; 

		//D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml -->this is my local dir (wyc)

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\RecoSys_B_Query_TakeDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\RecoSys_B_Query_TakeDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiRecoSys_B_Query_TakeDlg = pDlg;	
	}
	m_guiRecoSys_B_Query_TakeDlg->SetEnabled(bShow);
	m_guiRecoSys_B_Query_TakeDlg->SetVisible(bShow);
	return m_guiRecoSys_B_Query_TakeDlg;

	FUNCTION_END;
}

CGuiRecoSys_A_Query_TakeDlg* CGameGuiManager::AddRecoSys_A_Query_TakeDlg(bool bShow /* = true */)
{
	FUNCTION_BEGIN;

	if(!m_guiRecoSys_A_Query_TakeDlg)
	{
		CGuiRecoSys_A_Query_TakeDlg* pDlg = new CGuiRecoSys_A_Query_TakeDlg();
		HRESULT  hr = E_FAIL; 

		//D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml -->this is my local dir (wyc)

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\RecoSys_A_Query_TakeDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\RecoSys_A_Query_TakeDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiRecoSys_A_Query_TakeDlg = pDlg;	
	}
	m_guiRecoSys_A_Query_TakeDlg->SetEnabled(bShow);
	m_guiRecoSys_A_Query_TakeDlg->SetVisible(bShow);
	return m_guiRecoSys_A_Query_TakeDlg;

	FUNCTION_END;
}
//师徒系统
CGuiMasterBountyDlg* CGameGuiManager::AddMasterBountyDlg(bool bShow /* = true */)
{
	FUNCTION_BEGIN;

	if(!m_guiMasterBountyDlg)
	{
		CGuiMasterBountyDlg* pDlg = new CGuiMasterBountyDlg();
		HRESULT  hr = E_FAIL; 

		//D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml -->this is my local dir (wyc)

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\MasterBountyDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MasterBountyDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiMasterBountyDlg = pDlg;	
	}
	m_guiMasterBountyDlg->SetEnabled(bShow);
	m_guiMasterBountyDlg->SetVisible(bShow);
	return m_guiMasterBountyDlg;

	FUNCTION_END;
}

CGuiPrenticeBountyDlg* CGameGuiManager::AddPrenticeBountyDlg(bool bShow /* = true */)
{
	FUNCTION_BEGIN;

	if(!m_guiPrenticeBountyDlg)
	{
		CGuiPrenticeBountyDlg* pDlg = new CGuiPrenticeBountyDlg();
		HRESULT  hr = E_FAIL; 

		//D:\\Client\\main\\runclient\\data\\gui\\givegiftDialog.xml -->this is my local dir (wyc)

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\PrenticeBountyDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\PrenticeBountyDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiPrenticeBountyDlg = pDlg;	
	}
	m_guiPrenticeBountyDlg->SetEnabled(bShow);
	m_guiPrenticeBountyDlg->SetVisible(bShow);
	return m_guiPrenticeBountyDlg;

	FUNCTION_END;
}

//////////////////////////////////////////////////////////////////////////
//minigamelistdlg
CMiniGameListDlg* CGameGuiManager::AddMiniGameListDlg(bool bShow)
{
	FUNCTION_BEGIN;

	if(!m_MiniGameListDlg)
	{
		CMiniGameListDlg* pDlg = new CMiniGameListDlg();
		HRESULT  hr = E_FAIL; 

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\MiniGameListDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MiniGameListDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_MiniGameListDlg = pDlg;	
	}
	m_MiniGameListDlg->SetEnabled(bShow);
	m_MiniGameListDlg->SetVisible(bShow);
	return m_MiniGameListDlg;

	FUNCTION_END;
}
CMGMoneyInputDlg* CGameGuiManager::AddMGMoneyInputDlg(bool bShow)
{
	FUNCTION_BEGIN;

	if(!m_MGMoneyInputDlg)
	{
		CMGMoneyInputDlg* pDlg = new CMGMoneyInputDlg();
		HRESULT  hr = E_FAIL; 

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\moneyinputboxdialog.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\moneyinputboxdialog.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_MGMoneyInputDlg = pDlg;	
	}
	m_MGMoneyInputDlg->SetEnabled(bShow);
	m_MGMoneyInputDlg->SetVisible(bShow);
	return m_MGMoneyInputDlg;

	FUNCTION_END;
}
CMGNumberDlg* CGameGuiManager::AddMGNumberDlg(enumGuiActionType type,void* pData)
{
	FUNCTION_BEGIN;

	if(!m_MGNumberDlg)
	{
		CMGNumberDlg* pDlg = new CMGNumberDlg();
		HRESULT  hr = E_FAIL; 

#ifdef _WYC_TEST_
		hr = GetGuiManager()->LoadGuiFromXML("D:\\Client\\main\\runclient\\data\\gui\\MGNumberDlg.xml",pDlg);
#else
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\MGNumberDlg.xml",pDlg);
#endif

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_MGNumberDlg = pDlg;	
	}
	m_MGNumberDlg->OnInitInfo(pData);
	m_MGNumberDlg->SetEnabled(true);
	m_MGNumberDlg->SetVisible(true);
	return m_MGNumberDlg;

	FUNCTION_END;
}
//////////////////////////////////////////////////////////////////////////
CGuiAuctionDialog* CGameGuiManager::AddAuctionDlg()
{
	FUNCTION_BEGIN;

	if(!m_guiAuctionDlg)
	{
		CGuiAuctionDialog* pDlg = new CGuiAuctionDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\auction.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiAuctionDlg = pDlg;	
	}
	m_guiAuctionDlg->SetEnabled(true);
	m_guiAuctionDlg->SetVisible(true);
	return m_guiAuctionDlg;

	FUNCTION_END;
}

CGuiAutoReplySettingDlg* CGameGuiManager::AddAutoReply(CGuiMenu* pMenu)
{
	FUNCTION_BEGIN;

	if(!m_guiAutoReply)
	{
		CGuiAutoReplySettingDlg* pDlg = new CGuiAutoReplySettingDlg();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\autoreplydialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiAutoReply = pDlg;	
	}
	m_guiAutoReply->SetEnabled(true);
	m_guiAutoReply->SetVisible(true);
	m_guiAutoReply->SetAutoReplyMenu( pMenu );
	return m_guiAutoReply;

	FUNCTION_END;
}

CGuiSelectDialog* CGameGuiManager::AddSelectDlg(bool bShow)
{
	FUNCTION_BEGIN;

	if(!m_guiSelectDialog)
	{
		CGuiSelectDialog* pDlg = new CGuiSelectDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\selectdialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiSelectDialog = pDlg;	
	}
	m_guiSelectDialog->SetEnabled(bShow);
	m_guiSelectDialog->SetVisible(bShow);
	return m_guiSelectDialog;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiPetCtrlDialog*  CGameGuiManager::AddPetFloatDialog(petType type)
{
	FUNCTION_BEGIN;

	if(!m_guiPetFloatDlg)
	{
		CGuiPetCtrlDialog* pDlg = new CGuiPetCtrlDialog( type );
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\petfloatdialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiPetFloatDlg = pDlg;	
	}
	m_guiPetFloatDlg->UpdatePetInfo(type);
	m_guiPetFloatDlg->SetEnabled(true);
	m_guiPetFloatDlg->SetVisible(true);
	return m_guiPetFloatDlg;

	FUNCTION_END;
}

CGuiUserListDialog*		CGameGuiManager::AddUserListDialog(CChatChannel* pChannel)
{
	if(!m_pUserListDlg)
	{
		CGuiUserListDialog* pDlg = new CGuiUserListDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\chatworlddialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_pUserListDlg = pDlg;	
	}
	m_pUserListDlg->SetChannel(pChannel);
	m_pUserListDlg->SetEnabled(true);
	m_pUserListDlg->SetVisible(true);
	return m_pUserListDlg;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CNationalMapDialog* CGameGuiManager::AddNationalMap()
{
	if(!m_guiNationalMap)
	{
		CNationalMapDialog* pDlg = new CNationalMapDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\worldmap.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiNationalMap = pDlg;	
	}

	m_guiNationalMap->SetEnabled(true);
	m_guiNationalMap->SetVisible(true);

	GetGameApplication()->m_bESCHook = true;
	return m_guiNationalMap;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CGuiSaveBoxContinueDialog* CGameGuiManager::AddSaveBoxContinueDlg(BYTE box)
{
	FUNCTION_BEGIN;

	if(!m_guiSaveBoxContinueDlg)
	{
		CGuiSaveBoxContinueDialog* pDlg = new CGuiSaveBoxContinueDialog();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\savingboxcontinue.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiSaveBoxContinueDlg = pDlg;	
	}
	m_guiSaveBoxContinueDlg->SetEnabled(true);
	m_guiSaveBoxContinueDlg->SetBoxID(box);
	m_guiSaveBoxContinueDlg->SetVisible(true);
	return m_guiSaveBoxContinueDlg;

	FUNCTION_END;
}

CGuiAuctionMoneyInputDialog* CGameGuiManager::AddAucMoneyInputDlg(DWORD dwAuctionId,int type)
{
	FUNCTION_BEGIN;

	if(m_guiAucMoneyInputDlg)
	{
		m_guiAucMoneyInputDlg->Close();
	}

	if(!m_guiAucMoneyInputDlg)
	{
		CGuiAuctionMoneyInputDialog* pDlg = new CGuiAuctionMoneyInputDialog(type);
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\auctionmoneyinputboxdialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiAucMoneyInputDlg = pDlg;	
	}
	m_guiAucMoneyInputDlg->m_dwAuctionId = dwAuctionId;
	m_guiAucMoneyInputDlg->SetEnabled(true);
	m_guiAucMoneyInputDlg->SetVisible(true);
	return m_guiAucMoneyInputDlg;

	FUNCTION_END;
}

CGuiAuctionGoldCoinInputDialog*	CGameGuiManager::AddAucGoldCoinInputDlg(DWORD dwAuctionId)
{
	FUNCTION_BEGIN;

	GUIMANAGER_ADD_DIALOG(CGuiAuctionGoldCoinInputDialog,m_guiAucGodlCoinInputDlg,"gui\\auctiongoldcoin.xml");

	FUNCTION_END;
}

CGuiGoldExchangeDialog* CGameGuiManager::AddGoldExchangeDlg()
{
	FUNCTION_BEGIN;

	GUIMANAGER_ADD_DIALOG(CGuiGoldExchangeDialog,m_guiGoldExchangeDlg,"gui\\goldexchangedialog.xml");

	FUNCTION_END;
}


CGuiPasswordDlg*	CGameGuiManager::AddPasswordDlg(enumPasswordType type,const char* pszLabel,void* pData)
{
	if(!m_guiPasswordDlg)
	{
		CGuiPasswordDlg* pDlg = new CGuiPasswordDlg(type,pszLabel,pData);
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"password.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiPasswordDlg = pDlg;	
	}
	m_guiPasswordDlg->SetEnabled(true);
	m_guiPasswordDlg->SetVisible(true);
	return m_guiPasswordDlg;
}

CGuiModifyPasswordDlg*	CGameGuiManager::AddModifyPasswordDlg(enumPasswordType type,const char* pszLabel1,const char* pszLabel2,void* pData)
{
	GUIMANAGER_ADD_DIALOG4(CGuiModifyPasswordDlg,m_guiModifyPasswordDlg,"gui\\modifypassword.xml",type,pszLabel1,pszLabel2,pData);
}

CGuiModifyRevenueBox * CGameGuiManager::AddModifyRevenueBox()
{
	if(!m_guiModifyRevenueBox)
	{
		CGuiModifyRevenueBox* pDlg = new CGuiModifyRevenueBox();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\ModifyRevenue.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiModifyRevenueBox = pDlg;	
	}
	m_guiModifyRevenueBox->SetEnabled(true);
	m_guiModifyRevenueBox->SetVisible(true);
	return m_guiModifyRevenueBox;
}

CGuiAdoptCartoonDialog* CGameGuiManager::AddAdoptCartoonPetDialog()
{
	GUIMANAGER_ADD_DIALOG(CGuiAdoptCartoonDialog,m_guiAdoptCartoonDlg,"gui\\adoptcartoonpet.xml");
}

CGuiCountrySavingBoxDlg * CGameGuiManager::AddCountrySaveingBoxDlg()
{
	if(!m_guiCountrySaveingBoxDlg)
	{
		CGuiCountrySavingBoxDlg* pDlg = new CGuiCountrySavingBoxDlg();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\CountrySaveingBox.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiCountrySaveingBoxDlg = pDlg;	
	}
	m_guiCountrySaveingBoxDlg->SetEnabled(true);
	m_guiCountrySaveingBoxDlg->SetVisible(true);
	return m_guiCountrySaveingBoxDlg;
}

CGuiCountryTechDlg   * CGameGuiManager::AddCountryTechDlg()
{
	if(!m_guiCountryTechDlg)
	{
		CGuiCountryTechDlg* pDlg = new CGuiCountryTechDlg();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\CountryTechDialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiCountryTechDlg = pDlg;	
	}
	m_guiCountryTechDlg->SetEnabled(true);
	m_guiCountryTechDlg->SetVisible(true);
	return m_guiCountryTechDlg;
}

CGuiCountry * CGameGuiManager::AddCountryDlg()
{
	if(!m_guiCountry)
	{
		CGuiCountry* pDlg = new CGuiCountry();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\CountryDialog.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiCountry = pDlg;	
	}
	m_guiCountry->SetEnabled(true);
	m_guiCountry->SetVisible(true);
	return m_guiCountry;
}

CGuiClearPointDlg * CGameGuiManager::AddClearPointDlg()
{
	if(!m_guiClearPoint)
	{
		CGuiClearPointDlg* pDlg = new CGuiClearPointDlg();
		HRESULT  hr = E_FAIL; 
		hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\clearpoint.xml",pDlg);

		if(FAILED(hr)) 
		{
			delete pDlg;
			return NULL;
		}
		m_guiClearPoint = pDlg;	
	}
	m_guiClearPoint->SetEnabled(true);
	m_guiClearPoint->SetVisible(true);
	return m_guiClearPoint;
}

CGuiExitGameDlg*   CGameGuiManager::AddExitGame()
{
      if( !m_guiExitGame )
      {
	      CGuiExitGameDlg* pDlg = new CGuiExitGameDlg();
	      HRESULT  hr = E_FAIL; 
	      hr = GetGuiManager()->LoadGuiFromXML(c_szGuiPack,"gui\\exitgame.xml",pDlg);

	      if(FAILED(hr)) 
	      {
		      delete pDlg;
		      return NULL;
	      }
	      m_guiExitGame = pDlg;	
      }
      m_guiExitGame->SetEnabled(true);
      m_guiExitGame->SetVisible(true);
      stPointI ptLocation;
      ptLocation.x = GetDevice()->GetWidth()/2 - m_guiExitGame->GetWidth()/2;
      ptLocation.y = GetDevice()->GetHeight()/2 - m_guiExitGame->GetHeight()/2;
      m_guiExitGame->SetLocation(ptLocation.x,ptLocation.y);
      return m_guiExitGame;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
void OnOffItemDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiItem && GetGameGuiManager()->m_guiItem->IsVisible())
	{
		GetGameGuiManager()->m_guiItem->SetVisible(false);
	}
	else GetGameGuiManager()->AddItem();

	FUNCTION_END;
}

void ShowHelpDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiHelp && GetGameGuiManager()->m_guiHelp->IsVisible() && !GetGameGuiManager()->m_guiHelp->IsAnchor())
		GetGameGuiManager()->m_guiHelp->SetVisible(false);
	else 
	{
		GetGameGuiManager()->AddHelp();		
	}

	FUNCTION_END;
}

void ShowEmotionDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiEmotionDlg && GetGameGuiManager()->m_guiEmotionDlg->IsVisible() && !GetGameGuiManager()->m_guiEmotionDlg->IsAnchor())
		GetGameGuiManager()->m_guiEmotionDlg->SetVisible(false);
	else 
	{
		GetGameGuiManager()->AddEmotionDlg();		
	}

	FUNCTION_END;
}

void OnOffNatioanalMapDialog()
{
	FUNCTION_BEGIN;

	if(GetGameGuiManager()->m_guiNationalMap && GetGameGuiManager()->m_guiNationalMap->IsVisible() && !GetGameGuiManager()->m_guiNationalMap->IsAnchor())
		GetGameGuiManager()->m_guiNationalMap->SetVisible(false);
	else 
	{
		GetGameGuiManager()->AddNationalMap();		
	}

	FUNCTION_END;
}

CGuiCartoonAddCentDialog* CGameGuiManager::AddCartoonAddCentDialog(DWORD dwCartoonID)
{
	GUIMANAGER_ADD_DIALOG1(CGuiCartoonAddCentDialog,m_guiCartoonAddCentDlg,"gui\\cartoonaddcent.xml",dwCartoonID);
}

CGuiPetPackDialog* CGameGuiManager::AddPetPackDlg()
{
	GUIMANAGER_ADD_DIALOG(CGuiPetPackDialog,m_guiPetPackDlg,"gui\\petpackdialog.xml");
}

CGuiAddMemberDialog*	CGameGuiManager::AddAddMemberDialog(enumGuiActionType type,const char* title)
{
	GUIMANAGER_SAFE_CLOSE_DIALOG(m_guiAddMemberDlg);
	GUIMANAGER_ADD_DIALOG2(CGuiAddMemberDialog,m_guiAddMemberDlg,"gui\\addmemberdialog.xml",type,title);
}

CGuiStockDialog* CGameGuiManager::AddStockDialog()
{
	GUIMANAGER_ADD_DIALOG(CGuiStockDialog,m_guiStockDlg,"gui\\stock.xml");	
}

bool CGameGuiManager::IsPackgetFull()
{
	if(m_guiItem && !m_guiItem->m_pTable->IsFull())
		return false;
	if(m_guiPackage1 && !m_guiPackage1->m_pTable->IsFull())
		return false;
	if(m_guiPackage2 && !m_guiPackage2->m_pTable->IsFull())
		return false;
	return true;
}

CGuiNameMoneyInputBoxDialog* CGameGuiManager::AddNameMoneyInputDlg(const char* pszText1,const char* pszText2,enumGuiActionType type,void* pData)
{
	GUIMANAGER_ADD_DIALOG4(CGuiNameMoneyInputBoxDialog,m_guiNameMoneyInputDlg,"gui\\namemoneyinputboxdialog.xml",pszText1,pszText2,type,pData);	
}


void AddClientMessageV(DWORD type,DWORD color,const char* szFmt,va_list argptr)
{
	char szString[1024];
	_vsnprintf(szString,1024,szFmt,argptr);
	szString[1023] = 0;
	if(GetGameGuiManager()->m_guiMain)
	{
		GetGameGuiManager()->m_guiMain->AddMessage(szString,(CGuiMain::enumMessageType)type,color);
	}
}

void AddClientMessageF(DWORD type,DWORD color,const char* szFmt,...)
{
	va_list args;
	va_start( args, szFmt );
	AddClientMessageV(type,color,szFmt,args);
	va_end(args);
}

void CGameGuiManager::CloseAll1_15TypeWnd()
{
	//1 大陆地图
	if (GetGameGuiManager()->m_guiNationalMap)
	{
		GetGameGuiManager()->m_guiNationalMap->Close();
		GetGameGuiManager()->m_guiNationalMap = NULL;
	}
	//2 当前小地图
	if (GetGameGuiManager()->m_guiLageMap)
	{
		GetGameGuiManager()->m_guiLageMap->Close();
		GetGameGuiManager()->m_guiLageMap = NULL;
	}
	//3 私聊对话邀请界面
	if (GetGameGuiManager()->m_guiInvite)
	{
		GetGameGuiManager()->m_guiInvite->Close();
		GetGameGuiManager()->m_guiInvite = NULL;
	}
	//4 情感界面
	if (GetGameGuiManager()->m_guiEmotionDlg)
	{
		GetGameGuiManager()->m_guiEmotionDlg->Close();
		GetGameGuiManager()->m_guiEmotionDlg = NULL;
	}
	//5 社会关系界面
	if (1)
	{
	}
	//6 系统设置
	if (GetGameGuiManager()->m_guiSystem)
	{
		GetGameGuiManager()->m_guiSystem->Close();
		GetGameGuiManager()->m_guiSystem = NULL;
	}
	//7 附近玩家界面//不可以关闭
	if (1 && GetGameGuiManager()->m_guiNearby && GetGameGuiManager()->m_guiNearby->IsVisible())
	{
		GetGameGuiManager()->m_guiNearby->SetVisible(false);		
	}
	//8 帮助
	if (GetGameGuiManager()->m_guiHelp)
	{
		GetGameGuiManager()->m_guiHelp->Close();
		GetGameGuiManager()->m_guiHelp = NULL;
	}
	//9 开始摆摊界面
	if (GetGameGuiManager()->m_guiShop && GetGameGuiManager()->m_guiShop->IsVisible())
	{
		GetGameGuiManager()->m_guiShop->SetVisible(false);		
	}
	//10 任务列表界面
	if (GetGameGuiManager()->m_guitaskdialog)
	{
		GetGameGuiManager()->m_guitaskdialog->Close();
		GetGameGuiManager()->m_guitaskdialog = NULL;
	}
	//11 人物属性界面//不可以关闭
	if (1 && GetGameGuiManager()->m_guiUserInfo && GetGameGuiManager()->m_guiUserInfo->IsVisible())
	{
		GetGameGuiManager()->m_guiUserInfo->SetVisible( false );		
	}
	//12 技能//不可以关闭
	if (1 && GetGameGuiManager()->m_guiSkill && GetGameGuiManager()->m_guiSkill->IsVisible())
	{
		GetGameGuiManager()->m_guiSkill->SetVisible( false );
	}	
	//13 包裹界面//不可以关闭
	if (1 && GetGameGuiManager()->m_guiItem && GetGameGuiManager()->m_guiItem->IsVisible())
	{
		GetGameGuiManager()->m_guiItem->SetVisible(false);
	}
	//14 坐骑界面（宠物）
	if (GetGameGuiManager()->m_guiPetDlg)
	{
		GetGameGuiManager()->m_guiPetDlg->Close();
		GetGameGuiManager()->m_guiPetDlg = NULL;
	}
	//15 GM留言
	if (GetGameGuiManager()->m_guiGmMsgBoxDialog) 
	{
		GetGameGuiManager()->m_guiGmMsgBoxDialog->Close();
		GetGameGuiManager()->m_guiGmMsgBoxDialog = NULL;
	}

	GetGameApplication()->m_bESCHook = false;
}
