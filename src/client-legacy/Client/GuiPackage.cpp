#include "public.h"
#include "./GameGuiManager.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./RoleItem.h"

#include ".\guipackage.h"

CGuiPackage::CGuiPackage(CRoleItem* pRoleItem)
:m_pRoleItem(pRoleItem)
{
	m_bCloseIsHide = true;
}

void CGuiPackage::OnCreate()
{
	CGuiDialog::OnCreate();

	this->SetCaptionText(m_pRoleItem->GetObjectName());

	m_pTable = GetTable(2);

	m_pTable->m_iTableType = OBJECTCELLTYPE_PACKAGE;
	m_pTable->m_iTableID = m_pRoleItem->GetThisID();
	m_pTable->m_EquipPosition = stPointI(m_pRoleItem->GetLocation().x,m_pRoleItem->GetLocation().y);
	
	m_pTable->m_showRowCount = m_pRoleItem->GetObjectBase()->maxmdam;
	m_pTable->ResetSize(m_pRoleItem->GetObjectBase()->maxpdam,
		m_pRoleItem->GetObjectBase()->maxmdam);
	GetStatic(10)->SetText(m_pRoleItem->GetName());

	switch(m_pRoleItem->GetObjectBase()->dwID) {
	case 662:
		SetBackImage(stResourceLocation("data\\interfaces.gl",4,20));
		break;
	case 663:
		SetBackImage(stResourceLocation("data\\interfaces.gl",4,21));
		break;
	case 664:
		SetBackImage(stResourceLocation("data\\interfaces.gl",4,22));
		break;
	case 810:
		SetBackImage(stResourceLocation("data\\interfaces.gl",4,23));
		break;
	case 811:
		SetBackImage(stResourceLocation("data\\interfaces.gl",4,24));
		break;
	}

}

void CGuiPackage::OnSetCloseLocation()
{
	FUNCTION_BEGIN;

	CGuiDialog::OnSetCloseLocation();


	if(m_pBtnClose)
	{
		stPointI pt(m_pBtnClose->GetLocation());

		m_pBtnClose->SetLocation(pt.x-63,pt.y);

		//switch(m_pRoleItem->GetObjectBase()->dwID) {
		//case 662:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//case 663:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//case 664:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//case 810:
		//	m_pBtnClose->SetLocation(pt.x-63,pt.y);
		//	break;
		//}
	}


	FUNCTION_END;
}

void CGuiPackage::OnClose(void)
{
	if (!m_bCloseIsHide)
	{
		switch(m_pTable->m_EquipPosition.x) {
		case EQUIPCELLTYPE_LEFT:
			GetGameGuiManager()->m_guiPackage1 = NULL;
			break;
		case EQUIPCELLTYPE_RIGHT:
			GetGameGuiManager()->m_guiPackage2 = NULL;
			break;
		}
	}
}
