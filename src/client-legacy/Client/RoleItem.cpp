/********************************************************************
	创建时间:	27:1:2005   15:23
	文件名: 	RoleItem.cpp
	文件类型:	cpp
	作者:		$$$03公司
	
	用途:	  角色身上的物品类
*********************************************************************/
#include "public.h"
#include "../gui/include/Guitypes.h"
#include "./GameGuiManager.h"
#include "./GuiChatSelect.h"
#include "./GuiItem.h"
#include "./GuiItemUp.h"
#include "./GuiItemMake.h"
#include "./roleitem.h"
#include "./GameGuiFloatItem.h"
#include "./Game.h"
#include "./GameCursor.h"
#include "./GuiInputBox.h"
#include "./GameScene.h"
#include "./MainCharacter.h"
#include "./GuiTrade.h"
#include "./GameState.h"
#include "./GuiNpcTrade.h"
#include "./GuiSaveBox.h"
#include "./GuiMessageBox.h"
#include "GuiShopDialog.h"
#include "./RoleSkill.h"
#include "./GuiSkill.h"
#include "./GuiUserInfo.h"
#include "./GuiNpcMake.h"
#include "./GuiMain.h"
#include "./GuiPackage.h"
#include "./GuiShopDialog.h"
#include "./GuiSendMailDialog.h"
#include "./GuiRecvMailDialog.h"
#include "./GuiAuctionDialog.h"
#include "./GuiOthersShopDialog.h"
#include "./GuiMoneyInputBox.h"
#include "./CountryInfo.h"
#include "./Country.h"
#include "./GuiClearPointDlg.h"
#include ".\GuiPetPackDialog.h"
#include "./GuiNpcDialog.h"
#include "./Particle/ParticleSysMgr.h"
#include "SafetyUser.h"

const char* c_szItemGrpPack  = "data\\items.gl";		// 物品图包
const int c_maxItemUpgrade = 14;
tItemThisID	CRoleItem::s_dwMaxThisID = 0;
bool	CRoleItem::s_bCanMoveItem = true;
stPointI g_ptMoneyPos(-1,-1);

struct stCellToItemType {
	byte  eCellType;
	byte  eItemType;
};

struct stTableContext {
	byte  eCellType;
	char  szItemType[16];
	char  szItemAction[8];
};


stCellToItemType  g_CelleToItemType[] = {
	{EQUIPCELLTYPE_HELM,ItemType_Helm},   /// 头盔
	{EQUIPCELLTYPE_BODY,ItemType_ClothBody},   /// 服装
	{EQUIPCELLTYPE_BODY,ItemType_FellBody},   /// 服装
	{EQUIPCELLTYPE_BODY,ItemType_MetalBody},   /// 服装
	{EQUIPCELLTYPE_HANDR,ItemType_Blade},  /// 右手
	{EQUIPCELLTYPE_HANDL,ItemType_Bow},  /// 左手
	{EQUIPCELLTYPE_HANDR,ItemType_Sword},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Axe},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Staff},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Hammer},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Fan},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Stick},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Arrow},  /// 右手
	{EQUIPCELLTYPE_HANDR,ItemType_Flower},  /// 右手
	{EQUIPCELLTYPE_HANDL,ItemType_Shield},  /// 左手
	{EQUIPCELLTYPE_NECKLACE,ItemType_Necklace},  /// 项链
	{EQUIPCELLTYPE_GLOVES,ItemType_Cuff},   /// 手套,手镯
	{EQUIPCELLTYPE_RING,ItemType_Fing},    /// 戒指
	{EQUIPCELLTYPE_BELT,ItemType_Caestus},   /// 腰带
	{EQUIPCELLTYPE_SHOES,ItemType_Shoes},  /// 鞋子
	{EQUIPCELLTYPE_PACKAGE,ItemType_Pack},  /// 包裹
	{EQUIPCELLTYPE_MAKE,ItemType_Change},  /// 道具
	{EQUIPCELLTYPE_ADORN,ItemType_Tonic},  /// 自动补血
	{EQUIPCELLTYPE_ADORN,ItemType_DoubleExp},  /// 双倍经验
	{EQUIPCELLTYPE_ADORN,ItemType_Honor},  /// 荣誉之球
	{EQUIPCELLTYPE_ADORN,ItemType_FashionBody},  /// 时装
	{EQUIPCELLTYPE_ADORN,ItemType_FillMP},  /// 自动补兰道具
	{EQUIPCELLTYPE_ADORN,ItemType_Amulet},  /// 护身符
};


stTableContext  g_TableContext[] = {
	{EQUIPCELLTYPE_HELM,"头部","防御力"},
	{EQUIPCELLTYPE_BODY,"身体","防御力"},
	{EQUIPCELLTYPE_HANDR,"右手","防御力"},
	{EQUIPCELLTYPE_HANDL,"左手","防御力"},
	{EQUIPCELLTYPE_NECKLACE,"颈部","防御力"},
	{EQUIPCELLTYPE_GLOVES,"手腕","防御力"},
	{EQUIPCELLTYPE_RING,"手指","防御力"},
	{EQUIPCELLTYPE_BELT,"腰部","防御力"},
	{EQUIPCELLTYPE_SHOES,"脚","防御力"},
	{EQUIPCELLTYPE_PACKAGE,"额外包裹",""},
	{EQUIPCELLTYPE_MAKE,"道具",""},
	{EQUIPCELLTYPE_ADORN,"饰品","防御力"},
};



#define NEED_ADD_REPAIR_VALUE	4

const char* ItemKind2String(BYTE kind)
{
	//物品类型, 0普通, 1蓝色, 2金色, 4神圣, 8套装
	switch( kind )
	{
	case 0: return "普通";
	case 1: return "蓝色";
	case 2: return "金色";
	case 4: return "神圣";
	case 8: return "套装";
	default:break;
	}
	return "";
}

bool IsHandsArm(byte type)
{
	switch(type) {
case ItemType_Axe:
case ItemType_Hammer:
case ItemType_Staff:
case ItemType_Stick:
	{
		return true;
	}
	}
	return false;
}


bool IsTonic()
{
	if (GetGameGuiManager()->m_guiUserInfo)
	{
		return GetGameGuiManager()->m_guiUserInfo->IsTonic();
	}
	return false;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param eCellType : 描述
* \param eItemType : 描述
* \return 返回值的描述
*/
bool IsCellItemType(byte eCellType,byte eItemType)
{
	FUNCTION_BEGIN;

	static int count = sizeof(g_CelleToItemType) / sizeof(stCellToItemType);

	for (int i=0;i<count;i++)
	{
		if ((g_CelleToItemType[i].eCellType==eCellType)&&(g_CelleToItemType[i].eItemType==eItemType))
		{
			return true;
		}
	}

	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param eItemType : 描述
* \return 返回值的描述
*/
byte IsEquipItem(byte eItemType)
{
	FUNCTION_BEGIN;

	static int count = sizeof(g_CelleToItemType) / sizeof(stCellToItemType);

	for (int i=0;i<count;i++)
	{
		if (g_CelleToItemType[i].eItemType==eItemType)
		{
			return g_CelleToItemType[i].eCellType;
		}
	}

	return 0;

	FUNCTION_END;
}

#define ISPROPERTY(value)  if (value!=0) nNum++

WORD GetObjectPropertyNum(t_Object*  m_pObject)
{
	WORD nNum = 0;

	ISPROPERTY(m_pObject->fivepoint);
	ISPROPERTY(m_pObject->maxmp);
	ISPROPERTY(m_pObject->mvspeed);
	ISPROPERTY(m_pObject->bang);
	ISPROPERTY(m_pObject->hpr);
	ISPROPERTY(m_pObject->mpr);
	ISPROPERTY(m_pObject->spr);
	ISPROPERTY(m_pObject->akspeed);
	ISPROPERTY(m_pObject->pdam);
	ISPROPERTY(m_pObject->pdef);

	ISPROPERTY(m_pObject->mdam);
	ISPROPERTY(m_pObject->mdef);
	ISPROPERTY(m_pObject->atrating);
	ISPROPERTY(m_pObject->akdodge);
	ISPROPERTY(m_pObject->poisondef);
	ISPROPERTY(m_pObject->lulldef);
	ISPROPERTY(m_pObject->reeldef);
	ISPROPERTY(m_pObject->evildef);
	ISPROPERTY(m_pObject->bitedef);
	ISPROPERTY(m_pObject->chaosdef);

	ISPROPERTY(m_pObject->colddef);
	ISPROPERTY(m_pObject->petrifydef);
	ISPROPERTY(m_pObject->blinddef);
	ISPROPERTY(m_pObject->stabledef);
	ISPROPERTY(m_pObject->slowdef);
	ISPROPERTY(m_pObject->luredef);
	ISPROPERTY(m_pObject->skill[0].point);
	ISPROPERTY(m_pObject->skills.point);
	ISPROPERTY(m_pObject->holy);
	ISPROPERTY(m_pObject->hpleech.odds);

	ISPROPERTY(m_pObject->mpleech.odds);
	ISPROPERTY(m_pObject->hptomp);
	ISPROPERTY(m_pObject->dhpp);
	ISPROPERTY(m_pObject->dmpp);
	ISPROPERTY(m_pObject->incgold);
	ISPROPERTY(m_pObject->doublexp);
	ISPROPERTY(m_pObject->mf);
	ISPROPERTY(m_pObject->dpdam);
	ISPROPERTY(m_pObject->dmdam);
	ISPROPERTY(m_pObject->bdam);

	ISPROPERTY(m_pObject->rdam);
	ISPROPERTY(m_pObject->ignoredef);
	ISPROPERTY(m_pObject->poison);
	ISPROPERTY(m_pObject->lull);
	ISPROPERTY(m_pObject->reel);
	ISPROPERTY(m_pObject->evil);
	ISPROPERTY(m_pObject->bite);
	ISPROPERTY(m_pObject->chaos);
	ISPROPERTY(m_pObject->cold);
	ISPROPERTY(m_pObject->petrify);

	ISPROPERTY(m_pObject->blind);
	ISPROPERTY(m_pObject->stable);
	ISPROPERTY(m_pObject->slow);
	ISPROPERTY(m_pObject->lure);

	return nNum;
}

float GetKindOdds(byte kind)
{
	float Odds = 0.0f;

	if (kind&0x4)
	{
		Odds = 2.0f;
	}
	else if (kind&0x2)
	{
		Odds = 1.5f;
	}
	else if (kind&0x1)
	{
		Odds = 1.2f;
	}
	else
		Odds = 1.0f;

	return Odds;
}

float GetObjectSellMoney(t_Object*  m_pObject)
{
	float nMoney = 0.0f;
	if (!m_pObject)
		return nMoney;

	ObjectBase_t* pObjectBase = GetObjectBase(m_pObject->dwObjectID);
	if (!pObjectBase)
		return nMoney;


	nMoney = 0.25*pObjectBase->price*(1+GetObjectPropertyNum(m_pObject)*0.1f)*GetKindOdds(m_pObject->kind);

	if (pObjectBase->price>0)
		nMoney = nMoney + 1.0f;

	return nMoney;
}


float GetDurRate(t_Object*  m_pObject)
{
	DWORD maxdur = m_pObject->maxdur;
	ObjectBase_t* pObjectBase = GetObjectBase(m_pObject->dwObjectID);
	if (pObjectBase!=NULL)
	{
		maxdur = pObjectBase->dwMaxDur;
	}
	return (float)((m_pObject->dur+49)/50)/((maxdur+49)/50);
}

DWORD GetObjectSellMoneyByDur(t_Object*  m_pObject)
{
	DWORD nMoney = 0;

	float fSell = GetObjectSellMoney(m_pObject);

	ObjectBase_t* pObjectBase = GetObjectBase(m_pObject->dwObjectID);
	if (pObjectBase!=NULL)
	{
		if (pObjectBase->dwID == 655)
		{
			fSell = 4000.0f;
		}
		else if ( pObjectBase->dwID == 881 )
		{
			fSell = 20000.0f;
		}
		else if ( pObjectBase->dwID == 685 )//魔力之源 
		{
			fSell = 4000.0f;
		}
		else if ( pObjectBase->dwID == 882 )//龙之魔力 
		{
			fSell = 20000.0f;
		}
		else if ( pObjectBase->dwID == 760 )//洗髓宝珠 
		{
			fSell = 200000.0f;
		}
		else if ( pObjectBase->dwID == 761 )//易筋宝珠 
		{
			fSell = 200000.0f;
		}
	}

	nMoney = fSell*((m_pObject->maxdur==0)?1:GetDurRate(m_pObject));

	return nMoney;
}



float GetRepairDurRate(t_Object*  m_pObject)
{
	DWORD maxdur = m_pObject->maxdur;
	ObjectBase_t* pObjectBase = GetObjectBase(m_pObject->dwObjectID);
	if (pObjectBase!=NULL)
	{
		maxdur = pObjectBase->dwMaxDur;
	}
	return (float)((m_pObject->maxdur-m_pObject->dur+49)/50)/((maxdur+49)/50);
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param m_pObject : 描述
* \return 返回值的描述
*/
DWORD GetObjectRepairMoney(t_Object*  m_pObject)
{
	FUNCTION_BEGIN;

	DWORD nMoney = 0;
	if (!m_pObject)
		return nMoney;

	ObjectBase_t* pObjectBase = GetObjectBase(m_pObject->dwObjectID);
	if (!pObjectBase)
		return nMoney;

	if (pObjectBase->dwUp==0)
		return nMoney;


	nMoney = 2*GetObjectSellMoney(m_pObject)*GetRepairDurRate(m_pObject);

	return nMoney;

	FUNCTION_END;
}


DWORD RepairMoney2RepairGold(DWORD dwObjectRepair)
{
	return (DWORD)((float)(dwObjectRepair/50)+0.99);
}


DWORD GetObjectRepairByGold(t_Object*  m_pObject)
{
	return RepairMoney2RepairGold(GetObjectRepairMoney(m_pObject));
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param dwMoney : 描述
* \return 返回值的描述
*/
const char* Money2String(DWORD dwMoney)
{
	FUNCTION_BEGIN;

	static char szText[MAX_PATH]={0};
	char szMoney[MAX_PATH]={0};
	char szTmp[MAX_NAMESIZE]={0};
	const int nIngotRate = INGOT_RATE;
	const int nLiangRate = TAEL_RATE;
	int nIngot = (int)(dwMoney / nIngotRate);
	int nLiang = (int)((dwMoney % nIngotRate) / nLiangRate);
	int nCent = (int)((dwMoney % nIngotRate) % nLiangRate);

	if (nIngot>0)
	{
		sprintf(szTmp,"%d%s",nIngot,INGOT_TXT);
		strncat(szMoney,szTmp,sizeof(szTmp));
	}

	if (nLiang>0)
	{
		sprintf(szTmp,"%d%s",nLiang,TAEL_TXT);
		strncat(szMoney,szTmp,sizeof(szTmp));
	}

	if ((nCent>0)||(strlen(szMoney)==0))
	{
		sprintf(szTmp,"%d%s",nCent,CENT_TXT);
		strncat(szMoney,szTmp,sizeof(szTmp));
	}

	strncpy(szText,szMoney,MAX_PATH);
	return szText;

	FUNCTION_END;
}

float GetItemPrice(DWORD price)
{
	float buyPrice = price;

	DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
	switch(goodness)
	{
	case GOODNESS_3:
		buyPrice *= 1.2f;
		break;
	case GOODNESS_4:
		buyPrice *= 1.5f;
		break;
	case GOODNESS_5:
	case GOODNESS_6:
		buyPrice *= 2.0f;
		break;
	}

	//Consider middle country
	if ( GetScene()->IsCurMapIsMiddleCountry() )
	{
		buyPrice *= 1.1f;
	}

	return (buyPrice + 0.99f);
}

const char* Minute2DayHourMinute(DWORD dwMinute, enmDayHourMinuteType type)
{
	FUNCTION_BEGIN;

	static char szText[MAX_PATH] = {0};
	char szMinute[MAX_PATH] = {0};
	char szTmp[MAX_NAMESIZE] = {0};
	const int nDayRate = 24 * 60;
	const int nHourRate = 60;

	int nDay = (int)( dwMinute / nDayRate );
	int nHour = (int)( (dwMinute % nDayRate) / nHourRate );
	int nMinute = (int)( (dwMinute % nDayRate) % nHourRate );

	if ( type >= TYPE_DAY )
	{
		if ( nDay > 0 )
		{
			sprintf( szTmp, "%d%s", nDay, "天" );
			strncat( szMinute, szTmp, sizeof(szTmp) );
		}
		if ( type >= TYPE_DAY_HOUR )
		{
			if ( nHour > 0 )
			{
				sprintf( szTmp, "%d%s", nHour, "时" );
				strncat( szMinute, szTmp, sizeof(szTmp) );
			}

			if ( type >= TYPE_DAY_HOUR_MINUTE )
			{
				if ( (nMinute > 0) || (strlen(szMinute) == 0) )
				{
					sprintf( szTmp, "%d%s", nMinute, "分" );
					strncat( szMinute, szTmp, sizeof(szTmp) ); 
				}
			}			
		}		
	}	

	strncpy( szText, szMinute, MAX_PATH );

	return szText;

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param strMoney : 描述
* \return 返回值的描述
*/
DWORD String2Money(const char* strMoney)
{
	FUNCTION_BEGIN;

	std::string str = strMoney;

	int nIngotPos = str.find(INGOT_TXT);
	int nTaelPos = str.find(TAEL_TXT);
	int nCentPos = str.find(CENT_TXT);

	int nIngotMoney = 0;
	int nTaelMoney = 0;
	int nCentMoney = 0;

	if (nIngotPos!=-1)
		nIngotMoney = atoi(str.substr(0,nIngotPos).c_str());

	if (nTaelPos!=-1)
	{
		if (nIngotPos!=-1)
			nTaelMoney = atoi(str.substr(nIngotPos+strlen(INGOT_TXT),nTaelPos-nIngotPos).c_str());
		else
			nTaelMoney = atoi(str.substr(0,nTaelPos).c_str());
	}

	if (nCentPos!=-1)
	{
		if (nTaelPos!=-1)
			nCentMoney = atoi(str.substr(nTaelPos+strlen(TAEL_TXT),nCentPos-nTaelPos).c_str());
		else if (nIngotPos!=-1)
			nCentMoney = atoi(str.substr(nIngotPos+strlen(TAEL_TXT),nCentPos-nIngotPos).c_str());
		else
			nCentMoney = atoi(str.substr(0,nTaelPos).c_str());
	}

	return (nIngotMoney*INGOT_RATE + nTaelMoney*TAEL_RATE + nCentMoney);

	FUNCTION_END;
}


/**
* \brief 简短描述
* 
* 详细描述
* 
* \param eCellType : 描述
* \return 返回值的描述
*/
char* GetTableContext(byte eCellType)
{
	FUNCTION_BEGIN;

	static int count = sizeof(g_TableContext) / sizeof(stTableContext);
	static char szContext[64] = {0};

	for (int i=0;i<count;i++)
	{
		if (g_TableContext[i].eCellType==eCellType)
		{
			sprintf(szContext,"%s",g_TableContext[i].szItemType);
			return szContext;
		}
	}

	return "";

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pObjectBase : 描述
* \return 返回值的描述
*/
stResourceLocation GetObjectBasePicLocation(ObjectBase_t* pObjectBase)
{
	FUNCTION_BEGIN;

	Assert(pObjectBase);
	stResourceLocation rl;
	if(!pObjectBase) return rl;
	rl.SetFileName(c_szItemGrpPack);

	rl.group = c_nGroupItemEquip;
	rl.frame = pObjectBase->dwEquipPic;

	return rl;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pObjectBase : 描述
* \return 返回值的描述
*/
stResourceLocation GetObjectBasePatternPicLocation(ObjectBase_t* pObjectBase)
{
	FUNCTION_BEGIN;

	Assert(pObjectBase);
	stResourceLocation rl;
	if(!pObjectBase) return rl;
	rl.SetFileName(c_szItemGrpPack);

	rl.group = c_nGroupItemEquipPattern;
	rl.frame = pObjectBase->dwEquipPic;

	return rl;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pObjectBase : 描述
* \return 返回值的描述
*/
stResourceLocation GetObjectBasePattern2PicLocation(ObjectBase_t* pObjectBase)
{
	FUNCTION_BEGIN;

	Assert(pObjectBase);
	stResourceLocation rl;
	if(!pObjectBase) return rl;
	rl.SetFileName(c_szItemGrpPack);

	rl.group = c_nGroupItemEquipPattern2;
	rl.frame = pObjectBase->dwEquipPic;

	return rl;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pObject : 描述
* \return 返回值的描述
*/
stResourceLocation GetItemPicLocation(t_Object * pObject)
{
	FUNCTION_BEGIN;

	ObjectBase_t* pObjectBase = GetObjectBase(pObject->dwObjectID);
	Assert(pObjectBase);
	stResourceLocation rl;
	if(!pObjectBase) return rl;
	rl.SetFileName(c_szItemGrpPack);
	/*if(pObject->pos.dwLocation == OBJECTCELLTYPE_EQUIPSHOW && pObjectBase->dwEquipPic != -1)
	{
	rl.group = c_nGroupItemEquip;
	rl.frame = pObjectBase->dwEquipPic;
	}
	else*/
	{
		rl.group = c_nGroupItemCommon;
		rl.frame = pObjectBase->dwItemPic;
	}
	return rl;

	FUNCTION_END;
}

stResourceLocation GetObjectImageLocation(ObjectBase_t * pObject)
{
	return stResourceLocation(c_szItemGrpPack,c_nGroupItemCommon,pObject->dwItemPic);
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pObject : 描述
* \return 返回值的描述
*/
stResourceLocation GetItemPatternPicLocation(t_Object * pObject)
{
	FUNCTION_BEGIN;

	ObjectBase_t* pObjectBase = GetObjectBase(pObject->dwObjectID);
	Assert(pObjectBase);
	stResourceLocation rl;
	if(!pObjectBase) return rl;
	rl.SetFileName(c_szItemGrpPack);
	/*if(pObject->pos.dwLocation == OBJECTCELLTYPE_EQUIPSHOW && pObjectBase->dwEquipPic != -1)
	{
	rl.group = c_nGroupItemEquipPattern;
	rl.frame = pObjectBase->dwEquipPic;
	}
	else*/
	{
		rl.group = c_nGroupItemCommonPattern;
		rl.frame = pObjectBase->dwItemPic;
	}
	return rl;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pObject : 描述
* \return 返回值的描述
*/
stResourceLocation GetItemPattern2PicLocation(t_Object * pObject)
{
	FUNCTION_BEGIN;

	ObjectBase_t* pObjectBase = GetObjectBase(pObject->dwObjectID);
	Assert(pObjectBase);
	stResourceLocation rl;
	if(!pObjectBase) return rl;
	rl.SetFileName(c_szItemGrpPack);
	/*if(pObject->pos.dwLocation == OBJECTCELLTYPE_EQUIPSHOW && pObjectBase->dwEquipPic != -1)
	{
	rl.group = c_nGroupItemEquipPattern2;
	rl.frame = pObjectBase->dwEquipPic;
	}
	else*/
	{
		rl.group = c_nGroupItemCommonPattern2;
		rl.frame = pObjectBase->dwItemPic;
	}
	return rl;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pos : 描述
* \return 返回值的描述
*/
CGuiTable* GetItemTable(const stObjectLocation & pos)
{
	FUNCTION_BEGIN;

	//TRACE("CGuiTable* GetItemTable:pos:Location->%d\tID:%d\tX:%d\tY:%d\t\n",pos.dwLocation,pos.dwTableID,pos.x,pos.y);
	switch(pos.dwLocation)
	{
	case OBJECTCELLTYPE_MAKE:
		{
			if (GetGameGuiManager()->m_guiMakeTo1)
			{
				if (GetGameGuiManager()->m_guiMakeTo1->m_nPos == pos.dwTableID)
				{
					switch(pos.y) 
					{
						case MAKECELLTYPE_EQUIP:
							return GetGameGuiManager()->m_guiMakeTo1->m_pEquipTable;
						case MAKECELLTYPE_MATERIAL1:
							return GetGameGuiManager()->m_guiMakeTo1->m_pMateroalTable1;
						case MAKECELLTYPE_MATERIAL2:
							return GetGameGuiManager()->m_guiMakeTo1->m_pMateroalTable2;
						case MAKECELLTYPE_MATERIAL3:
							return GetGameGuiManager()->m_guiMakeTo1->m_pMateroalTable3;
						case MAKECELLTYPE_MATERIAL4:
							return GetGameGuiManager()->m_guiMakeTo1->m_pMateroalTable4;
					}
				}
			}

			if (GetGameGuiManager()->m_guiMakeTo2)
			{
				if (GetGameGuiManager()->m_guiMakeTo2->m_nPos == pos.dwTableID)
				{
					switch(pos.y) 
					{
					case MAKECELLTYPE_EQUIP:
						return GetGameGuiManager()->m_guiMakeTo2->m_pEquipTable;
					case MAKECELLTYPE_MATERIAL1:
						return GetGameGuiManager()->m_guiMakeTo2->m_pMateroalTable1;
					case MAKECELLTYPE_MATERIAL2:
						return GetGameGuiManager()->m_guiMakeTo2->m_pMateroalTable2;
					case MAKECELLTYPE_MATERIAL3:
						return GetGameGuiManager()->m_guiMakeTo2->m_pMateroalTable3;
					case MAKECELLTYPE_MATERIAL4:
						return GetGameGuiManager()->m_guiMakeTo2->m_pMateroalTable4;
					}
				}
			}
		}
		break;
	case OBJECTCELLTYPE_EQUIP:
		switch(pos.y) 
		{
		case EQUIPCELLTYPE_PACKAGE:
			if (GetGameGuiManager()->m_guiItem)
			{
				switch(pos.x) 
				{
				case EQUIPCELLTYPE_LEFT:
					return GetGameGuiManager()->m_guiItem->m_pTableLeft;
				case EQUIPCELLTYPE_RIGHT:
					return GetGameGuiManager()->m_guiItem->m_pTableRight;
				}
			}
			break;
		case EQUIPCELLTYPE_MAKE:
			if (GetGameGuiManager()->m_guiMain)
			{
				switch(pos.x) 
				{
				case EQUIPCELLTYPE_LEFT:
					return GetGameGuiManager()->m_guiMain->m_pTableLeft;
				case EQUIPCELLTYPE_RIGHT:
					return GetGameGuiManager()->m_guiMain->m_pTableRight;
				}
			}
			break;
		default:
			return GetGameGuiManager()->m_guiItem->GetEquipTable(pos.y,pos.x);
		}
		break;
	case OBJECTCELLTYPE_EQUIPSHOW:
		if (GetGameGuiManager()->m_guiUserInfoShow)
			return GetGameGuiManager()->m_guiUserInfoShow->GetEquipTable(pos.y,pos.x);
		break;
	case OBJECTCELLTYPE_MOUSE:
		return GetGameGuiManager()->m_guiFloatItem->m_pTable;
		break;
	case OBJECTCELLTYPE_COMMON:
		return GetGameGuiManager()->m_guiItem->m_pTable;
		break;
	case OBJECTCELLTYPE_TRADE:
	case OBJECTCELLTYPE_OTHERTRADE:
		return GetGameGuiManager()->m_guiTrade->GetItemTable(pos.dwLocation);
		break;
	case OBJECTCELLTYPE_SELL:
		if(GetGameGuiManager()->m_guiNpcTrade)
			return GetGameGuiManager()->m_guiNpcTrade->m_pTable;
		else
			return NULL;
		break;
	case OBJECTCELLTYPE_STORE:
		{
			if(GetGameGuiManager()->m_guiSavingBoxDlg)
			{
				return GetGameGuiManager()->m_guiSavingBoxDlg->GetBoxTable(pos);
			}
		}
	case OBJECTCELLTYPE_PACKAGE:
		{
			if (GetGameGuiManager()->m_guiPackage1)
			{
				if (GetGameGuiManager()->m_guiPackage1->m_pTable->m_iTableID == pos.dwTableID)
				{
					return GetGameGuiManager()->m_guiPackage1->m_pTable;
				}
			}

			if (GetGameGuiManager()->m_guiPackage2)
			{
				if (GetGameGuiManager()->m_guiPackage2->m_pTable->m_iTableID == pos.dwTableID)
				{
					return GetGameGuiManager()->m_guiPackage2->m_pTable;
				}
			}
		}
		break;
	case OBJECTCELLTYPE_MYSHOP:
		{
			if(GetGameGuiManager()->m_guiShop)
			{
				return GetGameGuiManager()->m_guiShop->m_shopTable;
			}
		}		
		break;
	case OBJECTCELLTYPE_OTHERSSHOP:
		{
			if(GetGameGuiManager()->m_guiOthersShop)
			{
				return GetGameGuiManager()->m_guiOthersShop->m_shopTable;
			}
		}		
		break;
	case OBJECTCELLTYPE_MAIL:
		{
			switch( pos.dwTableID )
			{
			case 0 :
				{
					if(GetGameGuiManager()->m_guiSendMailDlg)
					{
						return GetGameGuiManager()->m_guiSendMailDlg->m_pItemTable;
					}
				}
				break;
			case 1:
				if(GetGameGuiManager()->m_guiRecvMailDlg)
				{
					return GetGameGuiManager()->m_guiRecvMailDlg->m_pItemTable;
				}
				break;
			default:
				if(GetGameGuiManager()->m_guiAuctionDlg)
				{
					return GetGameGuiManager()->m_guiAuctionDlg->GetTableByPos( pos );
				}
				break;
			}
		}
		break;
	case OBJECTCELLTYPE_COUNTRY_SAVEBOX:
		{
			if(GetGameGuiManager()->m_guiCountrySaveingBoxDlg)
				return GetGameGuiManager()->m_guiCountrySaveingBoxDlg->m_pTable;
		}
		break;
	case OBJECTCELLTYPE_PET:
		{
			if( GetGameGuiManager()->m_guiPetPackDlg )
			{
				return GetGameGuiManager()->m_guiPetPackDlg->m_pPetPackTable;
			}
		}
		break;
	}
	return NULL;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param dwLocation : 描述
* \param count : 描述
* \return 返回值的描述
*/
void SetMoneyText(DWORD dwLocation,DWORD count)
{
	FUNCTION_BEGIN;

	switch(dwLocation)
	{
	case OBJECTCELLTYPE_COMMON:
		if(GetGameGuiManager()->m_guiItem)
			GetGameGuiManager()->m_guiItem->SetMoney(count);
		break;
	case OBJECTCELLTYPE_TRADE:
	case OBJECTCELLTYPE_OTHERTRADE:
		if(GetGameGuiManager()->m_guiTrade)
			return GetGameGuiManager()->m_guiTrade->SetMoney(dwLocation,count);
		break;
	}

	FUNCTION_END;
}

inline bool bIsShopDialogShown()
{
	if(GetGameGuiManager()->m_guiShop)
	{
		return GetGameGuiManager()->m_guiShop->IsVisible();
	}
	if(GetGameGuiManager()->m_guiOthersShop)
	{
		return GetGameGuiManager()->m_guiOthersShop->IsVisible();
	}
	return false;
}

inline bool bShowMyTip(CGuiTable* pTable)
{
	if( NULL == pTable )
	{
		return false;
	}
	if( pTable->m_iTableType == OBJECTCELLTYPE_MYSHOP ||
		pTable->m_iTableType == OBJECTCELLTYPE_OTHERSSHOP ||
		(pTable->m_iTableType == OBJECTCELLTYPE_COMMON && 
		bIsShopDialogShown() ))
	{
		return true;
	}
	return false;	
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pTable : 描述
* \param fElapsedTime : 描述
* \return 返回值的描述
*/
void TableRender(CGuiTable* pTable,float fElapsedTime)
{
	FUNCTION_BEGIN;

	if( NULL == pTable )
	{
		return ;
	}

	static CToolTips s_ToolTips;
	stPointI ptCursor = Engine_GetCursor()->GetPosition();
	CGuiFloatItem* pFloatItem = NULL;
	stRectI tableRect = pTable->m_itemRect;
	bool    m_bMouseOn = false;

	pTable->m_pDialog->ClientToScreen(&tableRect);
	if(tableRect.PtInRect(ptCursor))
	{
		m_bMouseOn = true;
	}


	if ((pTable->m_iTableType==OBJECTCELLTYPE_EQUIP)&&(pFloatItem = GetGuiManager()->GetFloatItem()))
	{
		if (((CGuiUserInfo*)pTable->m_pDialog)->IsMainInfo())
		{
			switch(pTable->m_EquipPosition.y)
			{
			case EQUIPCELLTYPE_HELM:
			case EQUIPCELLTYPE_NECKLACE:
			case EQUIPCELLTYPE_GLOVES:
			case EQUIPCELLTYPE_RING:
			case EQUIPCELLTYPE_BELT:
			case EQUIPCELLTYPE_SHOES:
			case EQUIPCELLTYPE_BODY:
			case EQUIPCELLTYPE_HANDR:
			case EQUIPCELLTYPE_HANDL:
			case EQUIPCELLTYPE_ADORN:
				{
					CRoleItem* pItem = (CRoleItem*)pFloatItem->GetItem()->m_pItemData;
					if (IsCellItemType(pTable->m_EquipPosition.y,pItem->GetItemType()))
					{
						IBitmaps* pBmp=NULL;
						pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,50)));
						if (pBmp)
						{
							pBmp->RenderAni(tableRect.left-2,tableRect.top-2,xtimeGetTime(),-1,Blend_Null);
						}
					}
				}
				break;
			}
		}
	}

	if ((pTable->m_iTableType==OBJECTCELLTYPE_EQUIP)&&(pTable->m_listItem.size()==1))
	{
		CRoleItem* pItem =(CRoleItem*) ((CGuiItemCell*)pTable->m_listItem[0])->m_pItemData;
		if (pItem&&GetGameGuiManager()->m_guiUserInfo&&((pFloatItem==NULL)||(pFloatItem&&pFloatItem->GetItem()->m_pItemData!=pItem)))
		{
			if (GetGameGuiManager()->m_guiUserInfo->IsActiveItem(pItem))
			{
				IBitmaps* pBmp=NULL;
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
	}

	if ((pTable->m_iTableType==OBJECTCELLTYPE_EQUIP)&&(pTable->m_listItem.size()==0)&&m_bMouseOn && !GetGuiManager()->GetToolTips())
	{
		if (GetGuiManager()->GetMouseOverDlg()==pTable->m_pDialog)
		{
			s_ToolTips.Clear();
			s_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
			s_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			s_ToolTips.AddText(GetTableContext(pTable->m_EquipPosition.y));
			s_ToolTips.Resize();

			stPointI pt(tableRect.left + tableRect.Width()/2,tableRect.bottom);
			//s_ToolTips.Render(pt.x,pt.y);
			s_ToolTips.RenderTopLeftPrefer(tableRect,GetDevice()->GetWidth(),GetDevice()->GetHeight());
			//GetGuiManager()->SetToolTips(&s_ToolTips,pt);
		}
	}
	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pTable : 描述
* \param pCell : 描述
* \param fElapsedTime : 描述
* \return 返回值的描述
*/
void RoleItemRender(CGuiTable* pTable,CGuiItemCell* pCell,float fElapsedTime)
{
	FUNCTION_BEGIN;

	stRectI cellRect = pTable->GetCellPixelRect(pCell);

	if(pTable->m_iTableType != OBJECTCELLTYPE_MOUSE /*&& pTable->m_iTableType != OBJECTCELLTYPE_EQUIP*/)
	{
		CRoleItem* pRoleItem = (CRoleItem*)pCell->m_pItemData;
		if(pRoleItem)
		{			
			DWORD color = COLOR_ARGB(0,32,38,120); // alpha==0,去除可使用物品图标底色
			if(!pCell->IsEnabled())
				color = COLOR_ARGB(255,128,128,128);
			else if(pRoleItem->IsCanEquip())
				color = COLOR_ARGB(128,136,48,26); 
			else
			{
				stPointI ptCursor = Engine_GetCursor()->GetPosition();
				if(cellRect.PtInRect(ptCursor))
					color = COLOR_ARGB(128,46,60,24);
			}
			GetDevice()->FillRect(&cellRect,color);
		}
	}

	
	{
		CRoleItem* pRoleItem = (CRoleItem*)pCell->m_pItemData;
		if(pRoleItem)
			pCell->SetImageColor(1,pRoleItem->GetObjectBase()->dwColor);

		//Add need repair item to list
		if ( pRoleItem )
		{
			if ( pRoleItem->NeedConsiderDur() )
			{
				if ( ((pRoleItem->GetObject()->dur+49)/50 < NEED_ADD_REPAIR_VALUE) )
				{
					if ( GetScene()->GetMainCharacter() )
						GetScene()->GetMainCharacter()->AddOneNeedRepairItem( pRoleItem->GetThisID() );
				}
				else
				{
					if ( GetScene()->GetMainCharacter() )
						GetScene()->GetMainCharacter()->DeleteOneNeedRepairItem( pRoleItem->GetThisID() );
				}
			}
		}
	}

	bool m_bMouseOn = false;

	CRoleItem* pItem = (CRoleItem*)pCell->m_pItemData;
	if( pItem && pTable->m_iTableType != OBJECTCELLTYPE_MOUSE && !GetGuiManager()->GetToolTips() && GetGuiManager()->GetMouseOverDlg() == pTable->m_pDialog )
	{
		stPointI ptCursor = Engine_GetCursor()->GetPosition();

		if(cellRect.PtInRect(ptCursor))
		{
			m_bMouseOn = true;
		}

	}

	//For some special item we add used sign img
	if ( pItem )
	{
		if ( (pItem->GetObjectID() == c_nXuanYuanFu)
			|| (pItem->GetObjectID() == c_nXianTianFu)
			|| (pItem->GetObjectID() == 666) )
		{
			if ( pItem->GetObject()->maker[0] != 0 )	//Used
			{
				IBitmaps* pBmp=NULL;
				pBmp = GetDevice()->FindBitmaps(&(stResourceLocation("data\\interfaces.gl",3,300)));
				if ( pBmp )
				{
					pBmp->RenderAni(cellRect.left-56,cellRect.top-56,xtimeGetTime(),-1,Blend_Null);
				} 
			}
		}
	}

	if ( m_bMouseOn )
	{
		//If this item is one of a set we simply update tips when mouse on
		//This may be some FPS lower
		/*if ( pItem->GetObject()->kind & 8 )
			pItem->UpdateToolTips();*/

		//Show hole image and diamond
		pItem->ShowHoleImage();
		//render cell
		pCell->Render( fElapsedTime );

		//Get tips position
		stPointI pt(cellRect.left, cellRect.top - pItem->GetToolTips()->GetHeight() );
		pt.x -= pItem->GetToolTips()->GetWidth();
		//Give tips some EQ
		//First make sure tips full visible
		//Then we make sure tips not on item
		if(pt.x < 0)
			pt.x = cellRect.right;
		if( pt.x + pItem->GetToolTips()->GetWidth() > GetDevice()->GetWidth() )
			pt.x = cellRect.left - pItem->GetToolTips()->GetWidth();

		if(pt.y < 0)
			pt.y = cellRect.bottom;
		if( pt.y + pItem->GetToolTips()->GetHeight() > GetDevice()->GetHeight() )
			pt.y = GetDevice()->GetHeight() - pItem->GetToolTips()->GetHeight();

		//Render
		if (GetGameCursor()->GetCursorImageType()==eCursorRepair)
		{
			if (GetGameGuiManager()->m_guiNpcTrade)
			{
				if (GetGameGuiManager()->m_guiNpcTrade->IsRepair()==CGuiNpcDialog::eTradeRepairByGold)
				{
					pItem->SetTipFlags(pItem->GetTipFlags()|ITEMTIPS_FLAG_REPAIR_PRICEBYGOLD);
				}
				else
				{
					pItem->SetTipFlags(pItem->GetTipFlags()|ITEMTIPS_FLAG_REPAIR_PRICE);
				}
			}
			else
			{
				pItem->SetTipFlags(pItem->GetTipFlags()|ITEMTIPS_FLAG_REPAIR_PRICE);
			}
		}
		else
		{
			if ( !(pItem && pItem->GetObject()->upgrade >= 10) )
				pItem->SetTipFlags(pItem->GetTipFlags()&0xfffffffd);
		}

		pItem->ShowToolTips(pt);
	}
	else
	{
		//Hide hole image and diamond
		pItem->ShowHoleImage( true );
		//render cell
		pCell->Render( fElapsedTime );
	}


	FUNCTION_END;
}

/****************************** table 对应的 物品函数 ********************************/
//坐标点对应的格子
inline CGuiItemCell* GetTableCell(CGuiTable* pTable,POINT ptGrid)
{
	if(NULL == pTable)
	{
		return NULL;
	}

	if( pTable->IsValidGrid(ptGrid) )
	{
		return pTable->GetItemCell(ptGrid.x,ptGrid.y);
	}
	return NULL;
}

//选中的格子
inline CGuiItemCell* GetSelectCell(CGuiTable* pTable,POINT pt)
{
	if(NULL == pTable)
	{
		return NULL;
	}
	stPointI ptDestPixel = pt;

	// 计算目标格子	
	ptDestPixel.x -= pTable->GetX();
	ptDestPixel.y -= pTable->GetY();
	stPointI ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);

	return GetTableCell(pTable,ptDestGrid);
}

//查找table内格子ptGrid内的物品
inline CRoleItem* GetCellItem(CGuiTable* pTable,POINT ptGrid)
{
	CGuiItemCell* pCell = GetTableCell(pTable,ptGrid);
	if( NULL != pCell )
	{
		return (CRoleItem*)(pCell->m_pItemData);
	}	
	return NULL;
}

//得到鼠标选中的物品
inline CRoleItem* GetSelectItem(CGuiTable* pTable,POINT pt)
{
	CGuiItemCell* pCell = GetSelectCell(pTable,pt);
	if(NULL == pCell)
	{
		return NULL;
	}
	return (CRoleItem*)(pCell->m_pItemData);
}


//根据object找对应的CRoleItem
inline CRoleItem* GetItemByObject( const t_Object& object )
{
	CGuiTable* pTable = GetItemTable(object.pos);
	return GetCellItem( pTable,stPointI(object.pos.x,object.pos.y) );
}

/****************************** table 对应的 物品函数 ********************************/

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理分裂物品事件
*/
static bool OnSplitItemEvent(CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
{
	FUNCTION_BEGIN;

	if(!pTable) return false;
	CGuiItemCell* pDestCell = GetSelectCell(pTable,pt);
	if ( !pDestCell || !pDestCell->IsEnabled() )
	{
		return true;
	}
	CRoleItem* pSrcItem = CRoleItem::GetRoleItem(pDestCell);
	Assert(pSrcItem);
	if( pSrcItem && pSrcItem->GetCount() > 1 && pSrcItem->GetMaxCount() > 1 && pSrcItem->GetCellCount()<=1)
	{
		static stSplitItemData st;
		st.qwThisID = pSrcItem->GetThisID();
		st.dwCurNum = pSrcItem->GetCount();
		st.dst.dwLocation = OBJECTCELLTYPE_MOUSE;
		st.dst.dwTableID = 0;
		st.dst.x = 0;
		st.dst.y = 0;
		st.pSrcCell = pDestCell;
		CGuiInputBox* pBox = GetGameGuiManager()->AddInputBox("请输入要移动的数量：",eAction_SplitItem,&st);
		pBox->SetNumber(0,pSrcItem->GetCount());
		CRoleItem::s_bCanMoveItem = false;
		return true;
	}
	return false;

	FUNCTION_END;
}


/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理维修单个物品事件
*/
static bool OnRepairItemEvent(CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
{
	FUNCTION_BEGIN;

	CGuiItemCell* pDestCell = NULL;
	CRoleItem* pSrcItem = NULL;
	CRoleItem* pDestItem = NULL;

	stPointI ptDestGrid;
	stPointI ptDestPixel = pt;

	GetGameCursor()->SetCursor( CCursor::CursorType_Normal, eCursorNormal);

	if(!pTable) return false;

	// 计算目标格子
	ptDestPixel.x -= pTable->GetX();
	ptDestPixel.y -= pTable->GetY();
	ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);

	if(pTable->IsValidGrid(ptDestGrid) && (pDestCell = pTable->GetItemCell(ptDestGrid.x,ptDestGrid.y)) )
	{
		// 灰掉的物品不能襙作
		if(!pDestCell->IsEnabled())return true;

		pSrcItem = (CRoleItem*)pDestCell->m_pItemData;
		Assert(pSrcItem);
		if( pSrcItem)
		{
			switch(pSrcItem->GetObjectID()) 
			{
			case 690:
			case 691:
			case 692:
			case 693:
			case 694:
				{
					GameMessageBox("新手装备不能修理！");
					return true;
				}
				break;
			}

			if (GetGameGuiManager()->m_guiNpcTrade)
			{
				if (GetGameGuiManager()->m_guiNpcTrade->IsRepair()==CGuiNpcDialog::eTradeRepairByGold)
				{
					stRepairObjectGoldNpcTradeUserCmd cmd;
					cmd.id = pSrcItem->GetThisID();

					SEND_USER_CMD(cmd);
				}
				else
				{
					stRepairObjectNpcTradeUserCmd cmd;
					cmd.id = pSrcItem->GetThisID();
					cmd.gem_id = 0;

					SEND_USER_CMD(cmd);
				}

			}

			return true;
		}
	}
	return false;

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理买物品事件
*/
static bool OnBuyItemEvent(CGuiItemCell* pCell,bool bLMouseDown)
{
	FUNCTION_BEGIN;

	CRoleItem* pItem = (CRoleItem*)pCell->m_pItemData;
	Assert(pItem);
	static stObjectLocation pos;
	pos = pItem->GetLocation();

	CGuiDialog* pDlg = NULL;
	if ((pItem->GetObjectBase()->dwMaxCount>1)&&bLMouseDown)
	{
		if (pItem->GetObjectBase()->dwTradeType & eBuySilver)
		{
			pDlg =(CGuiDialog*) GetGameGuiManager()->AddMessageBox(avar("购买 %s 需要 %s，你是否要购买？",pItem->GetName(),Money2String(pItem->GetPrice( ITEM_PRICE_BUY )* pItem->GetCount())),eAction_BuyItem,&pos,c_szMBOKCancel);
		}
		else
		{
			pDlg =(CGuiDialog*) GetGameGuiManager()->AddMessageBox(avar("购买 %s 需要 %s 金子(点券)，你是否要购买？",pItem->GetName(),Money2String(pItem->GetObjectBase()->price * pItem->GetCount())),eAction_BuyItem,&pos,c_szMBOKCancel);

		}
	}
	else
	{
		if (pItem->GetObjectBase()->dwType==ItemType_DoubleExp)
		{
			pDlg =(CGuiDialog*) GetGameGuiManager()->AddMessageBox(avar("获得 %s 需要 %d 荣誉点，你是否要交换？",pItem->GetName(),pItem->GetObjectBase()->price),eAction_BuyItem,&pos,c_szMBOKCancel);
		}
		else
		{
			if (pItem->GetObjectBase()->dwTradeType & eBuySilver)
			{
				pDlg =(CGuiDialog*) GetGameGuiManager()->AddMessageBox(avar("购买 %s 需要 %s，你是否要购买？",pItem->GetName(),Money2String(pItem->GetPrice( ITEM_PRICE_BUY ))),eAction_BuyItem,&pos,c_szMBOKCancel);

			}
			else
			{
				pDlg =(CGuiDialog*) GetGameGuiManager()->AddMessageBox(avar("购买 %s 需要 %s 金子(点券)，你是否要购买？",pItem->GetName(),Money2String(pItem->GetObjectBase()->price)),eAction_BuyItem,&pos,c_szMBOKCancel);

			}
		}

		if (!bLMouseDown)
		{
			pDlg->SetVisible(false);
			((CGuiMessageBox*)pDlg)->OnButtonClick(GUI_IDOK);
			return true;
		}
	}

	if (GetGameGuiManager()->m_guiNpcDialog)
		GetGameGuiManager()->m_guiNpcDialog->m_pMsgDlg = (void*) pDlg;

	int width  = pDlg->GetWidth();
	int height = pDlg->GetHeight();

	stPointI pt = Engine_GetCursor()->GetPosition();
	pt.x -= width/2;
	pt.y -= height/2;
	if(pt.x < 0) pt.x = 0;
	if(pt.y < 0) pt.y = 0;
	if (pDlg)
	{
		pDlg->SetAlign(GUI_ALIGN_NONE);
		pDlg->SetColor(COLOR_ARGB(0xa0,255,255,255));
		pDlg->SetLocation(pt.x,pt.y);
	}
	return true;

	FUNCTION_END;
}

bool CRoleItem::IsNeedConfirmBeforeSell()
{
	//三等以上的原材料
	if ( GetItemType() == ItemType_Resource )
	{
		if ( GetObject()->upgrade > 2 )
			return true;
	}

	//9星以上的装备，包括九星的
	if ( IsEquipment() )
	{
		if ( GetObject()->upgrade > 8 )
			return true;
	}

	//神圣装备
	if ( GetObject()->kind & 0x4 )
		return true;

	//非装备以及材料
	if ( (GetItemType() == ItemType_DoubleExp) ||
		(GetItemType() == ItemType_Honor) ||
		(GetItemType() == ItemType_LevelUp) ||
		(GetItemType() == ItemType_ClearPoint) ||
		(GetItemType() == 59) ||
		(GetItemType() == ItemType_SOULSTONE) )
	{
		return true;
	}

	return false;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理卖物品事件
*/
static bool OnSellItemEvent( CGuiItemCell* pCell )
{
	FUNCTION_BEGIN;

	CRoleItem* pItem = (CRoleItem*)pCell->m_pItemData;
	Assert(pItem);
	if ( pItem->IsBind())
	{
		GetGameGuiManager()->AddMessageBox("当前物品已经被灵魂绑定，确定要卖给我吗？",eAction_Bind,(void*)pItem,c_szMBOKCancel);
		return true;
	}

	if ( pItem->IsNeedConfirmBeforeSell() )
	{
		GetGameGuiManager()->AddMessageBox( "这可是个好东西,确定要卖给我吗?", eAction_Bind, (void*)pItem, c_szMBOKCancel );
		return true;
	}

	pItem->Sell();
	return true;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCell : 描述
* \return 返回值的描述
*/
static bool OnSaveItemEvent( CGuiItemCell* pCell )
{
	FUNCTION_BEGIN;

	CRoleItem* pItem = (CRoleItem*)pCell->m_pItemData;
	Assert(pItem);
	pItem->Save();
	return true;

	FUNCTION_END;
}

/**
* \brief 处理自动装备箭支
* 
* 处理自动装备箭支
* 
* \param pCell : 描述
* \return 返回值的描述
*/
bool EquipArrow(const int& num)
{
	if (GetGameGuiManager()->m_guiUserInfo==NULL)
		return false;

	CGuiTable* pTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(EQUIPCELLTYPE_HANDL);

	if (pTable->m_listItem.size()!=1)
		return false;

	CRoleItem* pRoleItem = (CRoleItem*)pTable->m_listItem[0]->m_pItemData;
	if (pRoleItem==NULL)
		return false;

	if (pRoleItem->GetItemType()!=ItemType_Bow)
		return false;

	pTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(EQUIPCELLTYPE_HANDR);

	pRoleItem = GetScene()->GetMainCharacter()->GetCanUseArrow( num );

	if (pRoleItem==NULL)
		return false;


	// 交换物品
	stObjectLocation dst;

	PlayItemSound(pRoleItem->GetObjectBase()->itmeSound);
	dst.dwLocation = pTable->m_iTableType;
	dst.dwTableID = pTable->m_iTableID;

	dst.x = pTable->m_EquipPosition.x;
	dst.y = pTable->m_EquipPosition.y;

	pRoleItem->MoveTo(dst);

	return true;
}
/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理装备双手道具事件
*/
bool EquipHandsArm(CRoleItem* pRoleItem,CGuiTable* pDestTable)
{
	if (IsHandsArm(pRoleItem->GetItemType())==false)
		return true;

	byte ItemType = IsEquipItem(pRoleItem->GetItemType());
	if (ItemType == 0)
		return true;

	if (pDestTable->m_iTableType != OBJECTCELLTYPE_EQUIP)
		return true;

	if (pDestTable->m_EquipPosition.y != ItemType)
		return true;

	stObjectLocation pos;
	pos.dwLocation = OBJECTCELLTYPE_COMMON;
	CGuiTable* pTable = GetItemTable(pos);
	if (pTable == NULL)
		return false;

	stPointI point = pTable->GetNewPos(1,1);
	if (point == stPointI(-1,-1))
		return false;

	stObjectLocation dst;
	dst.dwLocation = pTable->m_iTableType;
	dst.dwTableID = pTable->m_iTableID;
	dst.x = point.x;
	dst.y = point.y;

	switch(ItemType)
	{
	case EQUIPCELLTYPE_HANDR:
		pTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(EQUIPCELLTYPE_HANDL);
		break;
	case EQUIPCELLTYPE_HANDL:
		pTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(EQUIPCELLTYPE_HANDR);
		break;
	default:
		return true;
	}


	CRoleItem* pSrcItem = NULL;
	if (pTable->m_listItem.size()!=1)
		return true;

	pSrcItem = (CRoleItem*)pTable->m_listItem[0]->m_pItemData;
	if (pSrcItem == NULL)
		return false;

	PlayItemSound(pSrcItem->GetObjectBase()->itmeSound);
	pSrcItem->MoveTo(dst);
	return true;
}

bool EquipItem(CRoleItem* pSrcItem)
{
	static bool bRingLeft = true;
	static bool bGloveLeft = true;
	static bool bAdornLeft = true;

	if ( GetScene()->GetMainCharacter()->IsCombinationState() )
	{
		GetGameGuiManager()->AddClientSystemMessage( "合体状态下无法更换装备!" );
		return false;
	}

	CGuiItemCell* pDestCell = NULL;

	byte ItemType = IsEquipItem(pSrcItem->GetItemType());
	if (ItemType==0)
		return false;

	DWORD dwErrorID = pSrcItem->IsCanEquip();
	if( dwErrorID != 0)
	{
		switch(dwErrorID)
		{
		case CANNOTUSE_CHARTYPE:
			GetGameGuiManager()->AddClientSystemMessage("不能装备!");
			break;
		case CANNOTUSE_LEVEL:
			if( pSrcItem->GetObject()->dwObjectID >= 101 && pSrcItem->GetObject()->dwObjectID <= 118 )
				GetGameGuiManager()->AddClientSystemMessage("等级不够,不能装备!");
			else
				GetGameGuiManager()->AddClientSystemMessage("等级不够，不能使用");
			break;
		}
		return false;
	}

	if (GetGameGuiManager()->m_guiUserInfo==NULL)
		return false;

	CGuiTable* pEquipTable = NULL;

	switch(ItemType) 
	{
	case EQUIPCELLTYPE_HELM:
	case EQUIPCELLTYPE_BODY:
	case EQUIPCELLTYPE_HANDR:
	case EQUIPCELLTYPE_HANDL:
	case EQUIPCELLTYPE_NECKLACE:
	case EQUIPCELLTYPE_BELT:
	case EQUIPCELLTYPE_SHOES:
		pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType);
		if (EquipHandsArm(pSrcItem,pEquipTable) == false)
			return false;

		break;
	case EQUIPCELLTYPE_GLOVES:
	case EQUIPCELLTYPE_RING:
		{
			pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType,EQUIPCELLTYPE_LEFT);
			if (pEquipTable == NULL)
				return false;

			pDestCell = pEquipTable->GetItemCell(0,0);
			if( pDestCell)
			{
				pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType,EQUIPCELLTYPE_RIGHT);
				pDestCell = pEquipTable->GetItemCell(0,0);
				if (pDestCell)
				{
					switch(ItemType) 
					{
					case EQUIPCELLTYPE_GLOVES:
						{
							if (bGloveLeft)
								pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType,EQUIPCELLTYPE_LEFT);
							else
								pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType,EQUIPCELLTYPE_RIGHT);

							bGloveLeft = !bGloveLeft;

						}
						break;
					case EQUIPCELLTYPE_RING:
						{
							if (bRingLeft)
								pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType,EQUIPCELLTYPE_LEFT);
							else
								pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable(ItemType,EQUIPCELLTYPE_RIGHT);

							bRingLeft = !bRingLeft;

						}
						break;
					}
				}
			}
		}
		break;
	case EQUIPCELLTYPE_ADORN:
		{
			//To see if has the same type as the two
			CRoleItem* pLeftItem = GetGameGuiManager()->m_guiUserInfo->GetEquipItem( ItemType, EQUIPCELLTYPE_LEFT );
			CRoleItem* pRightItem = GetGameGuiManager()->m_guiUserInfo->GetEquipItem( ItemType, EQUIPCELLTYPE_RIGHT );

			if ( pLeftItem && (pLeftItem->GetItemType() == pSrcItem->GetItemType()) )
				pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable( ItemType, EQUIPCELLTYPE_LEFT );
			else if ( pRightItem && (pRightItem->GetItemType() == pSrcItem->GetItemType()) )
				pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable( ItemType, EQUIPCELLTYPE_RIGHT );
			else
			{
				if ( pLeftItem )
					pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable( ItemType, EQUIPCELLTYPE_RIGHT );
				else
					pEquipTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable( ItemType, EQUIPCELLTYPE_LEFT );
			}
		}
		break;
	case EQUIPCELLTYPE_PACKAGE:
		{
			if (GetGameGuiManager()->m_guiItem==NULL)
				return false;

			pEquipTable = GetGameGuiManager()->m_guiItem->m_pTableLeft;
			if (pEquipTable == NULL)
				return false;

			pDestCell = pEquipTable->GetItemCell(0,0);
			if( pDestCell)
			{
				pEquipTable = GetGameGuiManager()->m_guiItem->m_pTableRight;
				pDestCell = pEquipTable->GetItemCell(0,0);
				if (pDestCell)
				{
					pEquipTable = NULL;
					GetGameGuiManager()->AddClientSystemMessage("包裹栏已满，不能装备");
					return false;
				}
			}
		}
		break;
	case EQUIPCELLTYPE_MAKE:
		{
			if (GetGameGuiManager()->m_guiMain==NULL)
				return false;

			pEquipTable = GetGameGuiManager()->m_guiMain->m_pTableLeft;
			if (pEquipTable == NULL)
				return false;

			pDestCell = pEquipTable->GetItemCell(0,0);
			if( pDestCell)
			{
				pEquipTable = GetGameGuiManager()->m_guiMain->m_pTableRight;
				pDestCell = pEquipTable->GetItemCell(0,0);
				if (pDestCell)
				{
					pEquipTable = NULL;
					GetGameGuiManager()->AddClientSystemMessage("道具栏已满，不能装备");
					return false;
				}
			}
		}
		break;
	}

	if (pEquipTable == NULL)
		return false;

	pDestCell = pEquipTable->GetItemCell(0,0);
	if( pDestCell)
	{
		if(!pDestCell->IsEnabled())
		{
			GetGameGuiManager()->AddClientSystemMessage("目标道具已经被锁定，无法替换装备！");
			return true;
		}
	}

	// 交换物品
	stObjectLocation dst;

	PlayItemSound(pSrcItem->GetObjectBase()->itmeSound);
	dst.dwLocation = pEquipTable->m_iTableType;
	dst.dwTableID = pEquipTable->m_iTableID;

	dst.x = pEquipTable->m_EquipPosition.x;
	dst.y = pEquipTable->m_EquipPosition.y;

	pSrcItem->MoveTo(dst);

	return true;
}

//添加交易物品
bool AddTradItem(CGuiTable* pTable,POINT ptDestGrid,CRoleItem* pSrcItem)
{
	if (pSrcItem->GetObject()->pos.dwLocation != OBJECTCELLTYPE_COMMON)
	{
		GetGameGuiManager()->AddClientSystemMessage("只有放在主包裹中的物品才能进行交易！");
		return true;
	}

	if (pSrcItem->IsBind())
	{
		GetGameGuiManager()->AddClientSystemMessage("已经被灵魂绑定的装备物品不能进行交易！");
		return true;
	}

	if (pSrcItem->GetItemType()==ItemType_Arrow)
	{
		GetGameGuiManager()->AddClientSystemMessage("箭支不能进行交易！");
		return true;
	}

	if (pSrcItem->GetItemType()==ItemType_Quest)  
	{
		if( ClearPoint_CannotTrade(pSrcItem) ) 
			return true;
		GetGameGuiManager()->AddClientSystemMessage("任务物品不能进行交易！");
		return true;
	}

	CGuiItemCell* pCell = NULL;
	int count = 0;
	stObjectLocation dst;
	dst.dwLocation = pTable->m_iTableType;
	dst.dwTableID = pTable->m_iTableID;
	dst.x = ptDestGrid.x;
	dst.y = ptDestGrid.y;


	pSrcItem->DestoryAllVirtualCell();
	pSrcItem->GetTrueCell(pCell,count);

	pSrcItem->AddShowLocation(pCell,count,dst,false,true);


	stAddObjectTradeUserCmd cmd;
	memcpy(&(cmd.object) , pSrcItem->GetObject() , sizeof(cmd.object));
	cmd.user_id = GetScene()->GetMainCharacter()->GetID();
	cmd.x = ptDestGrid.x;
	cmd.y = ptDestGrid.y;

	SEND_USER_CMD(cmd);

	return true;
}



//添加交易物品
bool AddTradItem(CGuiFloatItem* pFloatItem,CGuiTable* pTable,POINT ptDestGrid,CRoleItem* pSrcItem)
{
	if(!pTable || !pSrcItem || !pFloatItem) return false;

	if (pSrcItem->GetItemCountInCell(pFloatItem->GetItem())!=pSrcItem->GetCount())
		return true;
	return AddTradItem(pTable,ptDestGrid,pSrcItem);
}

bool SplitItem(CGuiFloatItem* pFloatItem,CRoleItem* pSrcItem,CRoleItem* pDestItem)
{
	if(!pDestItem || !pSrcItem || !pFloatItem) return false;
	//pSrcItem->UnionTo(pDestItem);
	int maxNum = pSrcItem->GetMaxCount();
	int sliptNum = maxNum - pDestItem->GetCount();
	if(sliptNum > 0)
	{
		stObjectLocation dst = pDestItem->GetLocation();
		pSrcItem->SplitTo(dst,min(sliptNum,pSrcItem->GetItemCountInCell(pFloatItem->GetItem())) );
		// 播放物品放下的声音
		PlayItemSound(pSrcItem->GetObjectBase()->itmeSound);
	}
	return true;
}

inline void ResetLocationIfStore(stObjectLocation& dst)
{
	/*
	*仓库的格子处理起来比较复杂,仓库有3个页面,把它当作一个页面来处理，
	*第二个页面的y坐标也就是实际的y坐标加上第一个页面的高度
	*第三个页面类推
	*这里是考虑放置物品到仓库的空格子时也要进行特殊处理
	*/
	if(dst.dwLocation == OBJECTCELLTYPE_STORE)
	{
		//int index = GetStoreTableIndex(dst);
		//if(index == -1) return false;
		//CMainCharacter *pMainChar = GetScene()->GetMainCharacter();
		//if(pMainChar)
		//{
		//	//这里判断目标格子是否到期
		//	if( pMainChar->GetSavingBox()->days[index] == 0 ) 
		//	{
		//		GetGameGuiManager()->AddMessageBox("你租用的仓库到期了！");
		//		return false;
		//	}
		//}
		switch(dst.dwTableID)
		{
		case 1:	dst.y += 0;break;
		case 2: dst.y += CGuiSavingBoxDialog::csShowRowCount;break;
		case 3: dst.y += CGuiSavingBoxDialog::csShowRowCount*2;break;
		}
		dst.dwTableID = 0;
	}
}

//分裂物品
bool SplitItem(CGuiFloatItem* pFloatItem,CGuiTable* pTable,POINT ptDestGrid,CRoleItem* pSrcItem)
{
	if(!pTable || !pSrcItem || !pFloatItem) return false;
	stObjectLocation dst;
	dst.dwLocation = pTable->m_iTableType;
	dst.dwTableID = pTable->m_iTableID;
	dst.x = ptDestGrid.x;
	dst.y = ptDestGrid.y;

	int num = pSrcItem->GetItemCountInCell( pFloatItem->GetItem() );
	if( num < pSrcItem->GetCount() )
	{	
		ResetLocationIfStore(dst);
		pSrcItem->SplitTo(dst,num);
	}
	else
	{
		pSrcItem->MoveTo(dst);
	}
	// 播放物品放下的声音
	PlayItemSound(pSrcItem->GetObjectBase()->itmeSound);

	return true;
}

bool IsEnabledPack(CGuiTable* pTable)
{
	if (pTable==NULL)
		return false;

	if (pTable->m_iTableType != OBJECTCELLTYPE_PACKAGE)
		return true;

	CGuiPackage* pDlg = (CGuiPackage*)pTable->m_pDialog;
	if (pDlg==NULL)
		return false;

	CRoleItem* pItem = pDlg->GetPackItem();
	if (pItem==NULL)
		return false;

	if (pItem->GetItemDur()==0)
	{
		GetGameGuiManager()->AddServerSystemMessage("当前包裹的耐久度为0，不能再存放物品。");
		return false;
	}

	return true;
}
/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理移动物品事件
*/
static bool OnMoveItemEvent( CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	CGuiItemCell* pDestCell = NULL;
	CRoleItem* pSrcItem = NULL;
	CRoleItem* pDestItem = NULL;

	stPointI ptDestGrid;
	stPointI ptDestPixel = pt;

	// 计算目标格子
	if(pTable)
	{
		ptDestPixel.x -= pTable->GetX();
		ptDestPixel.y -= pTable->GetY();
		ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);
	}
	else
	{
		ptDestGrid = stPointI(0,0);
	}

	if( pFloatItem = GetGuiManager()->GetFloatItem() )
	{
		// 如果鼠标上有物品
		Assert(pFloatItem->GetItem());
		pSrcItem = (CRoleItem*)pFloatItem->GetItem()->m_pItemData;

		//
		//If this a special table (EQUIPCELLTYPE_ADORN)
		//We do some additional operation
		if ( pTable && (pTable->m_iTableType == OBJECTCELLTYPE_EQUIP) && (ptDestGrid.y == EQUIPCELLTYPE_ADORN) )
		{
			//To see if equiped the same type item already
			CRoleItem* pItem = NULL;
			if ( ptDestGrid.x == EQUIPCELLTYPE_LEFT )
				pItem = GetGameGuiManager()->m_guiUserInfo->GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_RIGHT );
			else
				pItem = GetGameGuiManager()->m_guiUserInfo->GetEquipItem( EQUIPCELLTYPE_ADORN, EQUIPCELLTYPE_LEFT );

			if ( pItem && (pItem != pSrcItem) && (pItem->GetItemType() == pSrcItem->GetItemType()) )
			{
				int x = (ptDestGrid.x == 0) ? EQUIPCELLTYPE_RIGHT : EQUIPCELLTYPE_LEFT;
				pTable = GetGameGuiManager()->m_guiUserInfo->GetEquipTable( EQUIPCELLTYPE_ADORN, x );

				//Reset dest about
				ptDestPixel.x -= pTable->GetX();
				ptDestPixel.y -= pTable->GetY();
				ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);
			}
		}		
		//
		// 计算放下的位置
		SIZE size = pFloatItem->GetItem()->GetSize();

		bool bCanPush = false;

		if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_SELL)
		{
			// 卖物品

			return OnSellItemEvent(pFloatItem->GetItem());
		}

		if(pTable)
		{
			ptDestPixel.x -= size.cx * pTable->m_cellWidth/2 - pTable->m_cellWidth/2;
			ptDestPixel.y -= size.cy * pTable->m_cellHeight/2 - pTable->m_cellHeight/2;
			stPointI pt1;
			ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);
			stRectI rect(ptDestGrid.x,ptDestGrid.y,ptDestGrid.x + size.cx,ptDestGrid.y + size.cy);

			bCanPush = pTable->CanPush(&rect);
		}
		else
		{
			if(pSrcItem->GetMaxCount() > 1)
			{
				if(pSrcItem->GetItemCountInCell(pFloatItem->GetItem()) != pSrcItem->GetCount())
				{
					// 一部分的物品不能销毁
					return true;
				}
			}
		}
		// 客户端预判断

		if( !pTable || bCanPush )
		{
			// 如果能够放下

			Assert(pSrcItem);

			if(pTable)
			{
				stObjectLocation location  = pSrcItem->GetLocation();

				if (EquipHandsArm(pSrcItem,pTable)==false)
					return true;

				if(IsEnabledPack(pTable)==false)
					return true;


				pDestCell = pTable->GetItemCell(ptDestGrid.x,ptDestGrid.y);
				if( pDestCell )
				{
					pDestItem = (CRoleItem*)pDestCell->m_pItemData;
					Assert(pDestItem);

					//If the float item is at special table (EQUIPCELLTYPE_ADORN)
					//We do some addition operation
					if ( (pSrcItem->GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP) && (pSrcItem->GetLocation().y == EQUIPCELLTYPE_ADORN) )
					{
						//To see if dest table cell is has any item can not swap
						if ( pDestItem )
						{
							int x = (pSrcItem->GetLocation().x == 0) ? EQUIPCELLTYPE_RIGHT : EQUIPCELLTYPE_LEFT;
							CRoleItem* pSpeItem = GetGameGuiManager()->m_guiUserInfo->GetEquipItem( EQUIPCELLTYPE_ADORN, x );
							if ( pSpeItem )
							{
								if ( pSpeItem->GetItemType() == pDestItem->GetItemType() )
									return false;		
							}
						}
						
					}

					if ((pTable->m_iTableType == OBJECTCELLTYPE_TRADE) )
						return true;

					if( pDestItem == pSrcItem )
					{
						// 相同位置的物品
						pDestItem->OnRemoveCell(pFloatItem->GetItem());
						pFloatItem->DestroyCell();
						return true;
					}

					// 灰掉的物品不能襙作
					if(!pDestCell->IsEnabled())return true;

					// 修理宝石修理
					if ((pSrcItem->GetObjectID()==870)&&(pDestItem->GetObject()->maxdur<pDestItem->GetObjectBase()->dwMaxDur)&&(pTable->m_iTableType==OBJECTCELLTYPE_COMMON))
					{
						stRepairObjectNpcTradeUserCmd  cmd;

						cmd.gem_id = pSrcItem->GetThisID();
						cmd.id = pDestItem->GetThisID();
						SEND_USER_CMD(cmd);
						return true;
					}

					if( ( pDestItem->GetObjectID() == pSrcItem->GetObjectID() )
						&& (pDestItem->GetObject()->upgrade == pSrcItem->GetObject()->upgrade)
						&& pSrcItem->GetMaxCount() > 1)
					{
						// 分裂物品
						return SplitItem(pFloatItem,pSrcItem,pDestItem);
					}

					if(pSrcItem->GetMaxCount() > 1)
					{
						// 如果是可以放多个的物品，目标类型不一样，鼠标上不是拿起所有数量物品，则不能放。
						if(pSrcItem->GetItemCountInCell(pFloatItem->GetItem()) != pSrcItem->GetCount())
							return true;
					}
				}
				else
				{
					if (pTable->m_iTableType==OBJECTCELLTYPE_MAKE)
					{
						stObjectLocation dst;

						dst.dwLocation = pTable->m_iTableType;
						dst.dwTableID = pTable->m_iTableID;
						dst.x = ptDestGrid.x;
						dst.y = ptDestGrid.y;

					}


					// 目标上没物品
					if( pSrcItem->GetMaxCount() > 1 )
					{						
						//交易的格子
						if(pTable->m_iTableType == OBJECTCELLTYPE_TRADE)
						{
							return AddTradItem(pFloatItem,pTable,ptDestGrid,pSrcItem);
						}
						else
						{
							//分裂物品(放置在一个空位置)
							return SplitItem(pFloatItem,pTable,ptDestGrid,pSrcItem);
						}
					}
				}

				if(pTable->m_iTableType == OBJECTCELLTYPE_EQUIP)
				{
					// 目标是装备栏
					if( pSrcItem->IsMoppet() )
					{
						// 是纸娃娃
						if(!IsMoppet(pTable->m_EquipPosition.y))
							return true;
						ptDestGrid.x = 0;
						ptDestGrid.y = pSrcItem->GetEquipType();
					}
					else
					{
						// 其它装备
						if( pSrcItem->GetEquipType() != pTable->m_EquipPosition.y )
							return true;
						ptDestGrid = pTable->m_EquipPosition;
					}
				}

				if(pTable->m_iTableType == OBJECTCELLTYPE_TRADE)
				{
					return AddTradItem(pTable,ptDestGrid,pSrcItem);
				}

				if(pTable->m_iTableType == OBJECTCELLTYPE_OTHERTRADE)
				{
					return true;
				}
			}

			// 交换物品
			stObjectLocation dst;
			if(pTable)
			{
				// 播放物品放下的声音
				PlayItemSound(pSrcItem->GetObjectBase()->itmeSound);
				dst.dwLocation = pTable->m_iTableType;
				dst.dwTableID = pTable->m_iTableID;
			}
			else
			{
				dst.dwLocation = OBJECTCELLTYPE_NONE;
				dst.dwTableID = 0;
			}

			dst.x = ptDestGrid.x;
			dst.y = ptDestGrid.y;

			//see if we want to exchange item
			if ( pTable )
			{
				pSrcItem->MoveTo(dst);
			}
			else
			{
				if ( GetKeyState( VK_SHIFT ) >= 0 )
				{
					pSrcItem->MoveTo(dst);
				}
			}
			return true;
		}
	}
	else 
	{
		// 拿起一个物品
		if(pTable->IsValidGrid(ptDestGrid) && (pDestCell = pTable->GetItemCell(ptDestGrid.x,ptDestGrid.y)) )
		{

			// 灰掉的物品不能襙作
			if(!pDestCell->IsEnabled())return true;

			if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_SELL)
			{
				return OnBuyItemEvent(pDestCell,uMsg == WM_LBUTTONDOWN);
			}

			//对方的装备位置不能拿起来
			if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_EQUIPSHOW)
			{
				return true;
			}

			pSrcItem = (CRoleItem*)pDestCell->m_pItemData;
			Assert(pSrcItem);

			//有物品的包裹不能拿起来
			if ((pTable->m_iTableType == OBJECTCELLTYPE_EQUIP)&&(pTable->m_EquipPosition.y == EQUIPCELLTYPE_PACKAGE))
			{
				stObjectLocation  pos;
				pos.dwLocation = OBJECTCELLTYPE_PACKAGE;
				pos.dwTableID = pSrcItem->GetThisID();
				CGuiTable* pPackageTable = GetItemTable(pos);
				if (pPackageTable)
				{	
					if (pPackageTable->m_listItem.size()>0)
						return true;
					else
						pPackageTable->m_pDialog->SetVisible(false);
				}
			}

			//有物品的合成包裹不能拿起来
			if ((pTable->m_iTableType == OBJECTCELLTYPE_EQUIP)&&(pTable->m_EquipPosition.y == EQUIPCELLTYPE_MAKE))
			{
				stObjectLocation  pos;
				pos.dwLocation = OBJECTCELLTYPE_MAKE;
				pos.dwTableID = pSrcItem->GetThisID();
				pos.y = MAKECELLTYPE_EQUIP;
				CGuiTable* pPackageTable = GetItemTable(pos);
				if (pPackageTable)
				{	
					if (((CGuiMakeTo*)pPackageTable->m_pDialog)->GetItemCount()>0)
						return true;
					else
						pPackageTable->m_pDialog->SetVisible(false);
				}
			}


			stObjectLocation dst;
			dst.dwLocation = OBJECTCELLTYPE_MOUSE;
			dst.dwTableID = 0;
			dst.x = 0;
			dst.y = 0;
			// 显示到鼠标上

			int nCount = pSrcItem->GetCount();
			if(pTable->m_iTableType == OBJECTCELLTYPE_TRADE)
			{
				//

				int count;
				nCount = pSrcItem->GetItemCountInCell(pDestCell);
				pSrcItem->GetTrueCell(pDestCell,count);
				pSrcItem->DestoryAllVirtualCell();
				Assert(pDestCell);


				stRemoveObjectTradeUserCmd cmd;
				cmd.id = pSrcItem->GetThisID();
				cmd.user = GetScene()->GetMainCharacter()->GetID();

				SEND_USER_CMD(cmd);


			}
			pSrcItem->AddShowLocation(pDestCell,nCount,dst,pSrcItem->GetCount() == nCount && pTable->m_iTableType != OBJECTCELLTYPE_TRADE);

			// 播放物品拿起的声音
			PlayItemSound(g_nItemTakeUpSound);
			return true;
		}
	}
	return false;

	FUNCTION_END;
}

inline DWORD GetFireIdFromItemId(const DWORD& id)
{
	static DWORD g_FireItem[][2] = {
		{1709, 618},
		{1710, 625},
		{1711, 624},
		{1712, 619},
		{1713, 620},
		{1714, 621},
		{1715, 622},
		{1716, 623},
		{1717, 614},
		{1718, 617},
		{1719, 616},
		{1720, 615},
		{1721, 637},
		{1722, 637},
		{1723, 0},
	};

	for(int i=0;i<count_of(g_FireItem);i++ )
	{
		if ( g_FireItem[i][0] == id )
			return g_FireItem[i][1];
	}

	return -1;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理使用物品事件
*/
static bool OnUseItemEvent(CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	CGuiItemCell* pDestCell = NULL;
	CRoleItem* pSrcItem = NULL;
	CRoleItem* pDestItem = NULL;
	if(pTable)
	{
		TRACE("Table Type:%d\n",pTable->m_iTableType);
	}

	stPointI ptDestGrid;
	stPointI ptDestPixel = pt;

	// 计算目标格子
	ptDestPixel.x -= pTable->GetX();
	ptDestPixel.y -= pTable->GetY();
	ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);

	if( pFloatItem = GetGuiManager()->GetFloatItem() )
	{
	}
	else
	{
		if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_SELL)
			return false;

		if(pTable->IsValidGrid(ptDestGrid) && (pDestCell = pTable->GetItemCell(ptDestGrid.x,ptDestGrid.y)) )
		{
			if(!pDestCell->IsEnabled())
				return false;

			pSrcItem = (CRoleItem*)pDestCell->m_pItemData;
			Assert(pSrcItem);

			if ((pSrcItem->GetObjectBase()->dwType==ItemType_Pack)&&(pTable->m_iTableType==OBJECTCELLTYPE_EQUIP))
			{
				if (pTable->m_EquipPosition.y == EQUIPCELLTYPE_PACKAGE)
				{
					switch(pTable->m_EquipPosition.x) 
					{
					case EQUIPCELLTYPE_LEFT:
						GetGameGuiManager()->AddPackage1(pSrcItem);
						break;
					case EQUIPCELLTYPE_RIGHT:
						GetGameGuiManager()->AddPackage2(pSrcItem);
						break;
					}
				}
				return true;
			}


			if ((pSrcItem->GetObjectBase()->dwID==689)&&(pTable->m_iTableType==OBJECTCELLTYPE_EQUIP))
			{
				if (pTable->m_EquipPosition.y == EQUIPCELLTYPE_MAKE)
				{
					switch(pTable->m_EquipPosition.x)
					{
					case EQUIPCELLTYPE_LEFT:
						GetGameGuiManager()->AddMakeTo1(pSrcItem);
						break;
					case EQUIPCELLTYPE_RIGHT:
						GetGameGuiManager()->AddMakeTo2(pSrcItem);
						break;
					}
				}
				return true;
			}

			// use clearpint 
			if( OnUseClearPointItem(pSrcItem)) 
				return true;

			//Christ. change face  item
			if ( (pSrcItem->GetObjectBase()->dwID == 1700) && 0 )		//Not use this method
			{
				if ( GetScene()->GetMainCharacter()->IsChangeFace()
					|| GetScene()->GetMainCharacter()->IsCombinationState() )
				{
					return true;
				}
				//First set change face state
				set_state( GetScene()->GetMainCharacter()->GetState(), USTATE_CHANGEFACE );
				GetScene()->GetMainCharacter()->ResetState();
				//Then send change face cmd
				stChangeFaceMapScreenUserCmd cmd;
				cmd.dwUserTempID = GetScene()->GetMainCharacter()->GetID();
				if ( GetScene()->GetMainCharacter()->GetSex() == eSex_Male )
					cmd.dwChangeFaceID = 44031;
				else
					cmd.dwChangeFaceID = 44032;
				SEND_USER_CMD( cmd );

				//Delete this item
				stObjectLocation dst;
				dst.dwLocation = OBJECTCELLTYPE_NONE;
				//msg
				stSwapObjectPropertyUserCmd cmdDel;
				cmdDel.qwThisID = pSrcItem->GetThisID();
				cmdDel.dst = dst;
				SEND_USER_CMD(cmdDel);

				return true;
			}
			
			//Use firework item
			if ( (pSrcItem->GetItemType() == 63) && (pSrcItem->GetLocation().dwLocation == OBJECTCELLTYPE_COMMON) )
			{
				DWORD dwFireID = GetFireIdFromItemId( pSrcItem->GetObjectID() );
				if ( dwFireID != -1 )
				{
					static DWORD tLastTimeSendCMD = xtimeGetTime() - 60000;

					//We control send msg rate
					if ( xtimeGetTime()-tLastTimeSendCMD < 5000 )
						return true;

					if ( dwFireID == 0 )		//We pre play a fire work
					{
						//We play Rose Rose I LOVE U
						if ( GetParticleSysMgr() )
						{
							//First we play a firework
							//And let Rose Rose IOU be his child
							D3DXVECTOR3 vEffect;
							vEffect.x = GetDevice()->GetWidth() / 2;
							vEffect.y = GetDevice()->GetHeight() / 3;
							vEffect.z = 0;

							float fStartTime = 0.0f;

							GetParticleSysMgr()->LoadOneParticleSystem( "Spring\\SpringFireWork0", vEffect );
							CParticleSystem* pSys = GetParticleSysMgr()->GetActiveParticleSystem();
							if ( pSys )
							{
								pSys->UseCharacterOffset( false );

								//Set sys life						
								pSys->Play( pSys->GetSysLife() );
								pSys->SetLoopPlay( false );

								fStartTime = pSys->GetSuppressTimeEnd();
							}
							//Load	RoseRoseIOU
							vEffect.y = 0;
							GetParticleSysMgr()->LoadOneParticleSystem( "Spring\\RoseRoseIOU", vEffect );
							pSys = GetParticleSysMgr()->GetActiveParticleSystem();
							if ( pSys )
							{
								pSys->UseCharacterOffset( false );

								//Set sys life						
								pSys->Play( 20 );
								pSys->SetLoopPlay( false );

								pSys->SetSuppressTimeStart( 0.0f );
								pSys->SetSuppressTimeEnd( fStartTime );
							}
						}
					}
					
					//Send msg to server
					{
						 //Send action fire cmd
						stFireWorkUserCmd cmd;
						cmd.qwObjectTempID = pSrcItem->GetThisID();
						cmd.dwUserID = GetScene()->GetMainCharacter()->GetID();
						cmd.byType = MAPDATATYPE_USER;
						cmd.dwFireID = GetFireIdFromItemId( pSrcItem->GetObjectID() );
						SEND_USER_CMD( cmd );
					}

					tLastTimeSendCMD = xtimeGetTime();
				} 

				return true;
			}

			if(pSrcItem->IsCanUse()	|| (pSrcItem->GetItemType() == ItemType_BMW) )
			{
				// 使用物品
				stUseObjectPropertyUserCmd cmd;
				CMainCharacter* pMainRole = GetScene()->GetMainCharacter();
				CALL_USEITEM_P(pMainRole,pSrcItem->GetThisID());	
				return true;
			}

			if ( ( (pSrcItem->GetObjectBase()->dwType == ItemType_SkillUp) || (pSrcItem->GetObjectBase()->dwType == ItemType_ZhenSkillUp) )
				&& (pTable->m_iTableType == OBJECTCELLTYPE_COMMON) )
			{
				DWORD skillID = GetSkillIdFromBook( pSrcItem->GetObjectID() );
				if ( skillID == -1 )
					return false;

				SkillBase_t* pSkill = GetSkillBase( skillID );
				if ( NULL == pSkill )
					return false;

				CRoleSkill* pRoleSkill = NULL;				
				if ( pSkill->dwSerialType != SKILL_SERIAL_SPECIAL )
				{
					pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( skillID );

					if ( pRoleSkill )
					{
						char szMsg[256];
						sprintf( szMsg, "你已经学习了%s!", pRoleSkill->GetName() );
						GetGameGuiManager()->AddClientSystemMessage( szMsg );
						//GetGameGuiManager()->AddMessageBox( szMsg );
					}
					else
					{
						if ( GetScene()->GetMainCharacter()->IsCanStudySkill( pSkill, 1, true ) )
						{
							char szMsg[256];
							sprintf( szMsg, "确定要学习%s吗？", pSkill->pstrName );
							GetGameGuiManager()->AddMessageBox( szMsg, eAction_SkillUp, pSkill, "确定|1|Enter|取消|2|Esc" );
						}
						else
						{
							GetGameGuiManager()->AddClientSystemMessage( "条件不足,不能学习!" );
							//GetGameGuiManager()->AddMessageBox( "条件不足,不能学习!", eAction_SkillUpFail );
						}
					}		
				}
				else
				{
					pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( skillID );
					DWORD level = GetSkillLevelFromBook( pSrcItem->GetObjectID() );
					if ( level == -1 )
						return false;

					if ( pRoleSkill && (pRoleSkill->GetLevel() == level) )
					{
						char szMsg[256];
						sprintf( szMsg, "你已经学习了%s!", pRoleSkill->GetName() );
						GetGameGuiManager()->AddClientSystemMessage( szMsg );
						//GetGameGuiManager()->AddMessageBox( szMsg );
					}
					else
					{
						SkillLevelBase_t* pSkillLevel = GetSkillLevelBase( skillID, level );
						if ( pSkillLevel && GetGameGuiManager()->m_guiSkill->IsCanStudySpecialSkill((enmSkillSpecialType)pSkillLevel->dwTreeType) )	//&& GetScene()->GetMainCharacter()->IsCanStudySkill( pSkill, 1)
						{
							if ( (pSkillLevel->dwNeedSkill3Level > GetScene()->GetMainCharacter()->GetLevel())
								|| (pSkillLevel->dwNeedSkill3Level >= OPEN_SKILL_MAX_LEVEL ) )
							{
								GetGameGuiManager()->AddClientSystemMessage( "条件不足,不能学习!" );
								//GetGameGuiManager()->AddMessageBox( "条件不足,不能学习!", eAction_SkillUpFail );								
							}
							else
							{
								char szMsg[256];
								sprintf( szMsg, "确定要学习%s吗？", pSkillLevel->pstrName );
								GetGameGuiManager()->AddMessageBox( szMsg, eAction_SkillUp, pSkill, "确定|1|Enter|取消|2|Esc" );
							}
						}						
					}
				}

				return true;
			}
		}
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pTable : 描述
* \param pt : 描述
* \return 返回值的描述
*/
bool AddItemToExchange( CGuiTable* pTable, POINT pt )
{
	FUNCTION_BEGIN;

	CRoleItem* pSrcItem = GetSelectItem(pTable,pt);

	if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_SELL)
	{
		return false;
	}
	if( pSrcItem )
	{
		CGuiEditBox* pEdit = CGuiEditBox::GetLastFocusEditBox();
		//Add cur item to main edit box
		if (  pEdit && (pEdit->GetStyle() & CGuiEditBox::GUI_ES_MULTIMEDIA) )
		{			
			pEdit->RequestFocus();
			//Split item name
			char* pName;
			char namePartOne[MAX_NAMESIZE];

			pName = strpbrk( pSrcItem->GetObjectName(), "0" );
			if ( pName )
				sprintf( namePartOne, ++pName );
			else
				sprintf( namePartOne, pSrcItem->GetObjectName() );
			//End split item name
			return pEdit->AddOneMedia( pSrcItem->GetThisID(), namePartOne );
		}
	}
	return false;

	FUNCTION_END;
}

/**
* \brief 判断目标物品类型是否是17,19,25,26中的一种
* 
* 在ItemTableEvent函数中作为触发"快捷使用物品"的判断条件
* 
* \param pTable : 目标格子
* \param pt : 目标点
* \return bool
*/
inline bool IsAccelKeyObject(CGuiTable* pTable,POINT pt)
{
	DWORD kinds[5] = {17,19,25,26};
	CRoleItem* pSrcItem = GetSelectItem(pTable,pt);
	if(NULL == pSrcItem)
	{
		//Assert(0);
		return false;
	}
	for(int i = 0;i < 4;i ++)
	{
		if(kinds[i] == pSrcItem->GetItemType())
		{
			return true;
		}
	}

	return false;
}

bool MoveItem2MyShop(CRoleItem* pSrcItem)
{
	if(NULL == pSrcItem)
	{
		return true;
	}
	if( pSrcItem->IsBind() ) // 不可丢弃类物品主要有灵魂绑定的物品
	{
		GetGameGuiManager()->AddClientSystemMessage("绑定的物品不能出售！");
		return true;
	}	
	if ( pSrcItem->GetItemType()==ItemType_Quest )
	{
		if( ClearPoint_CannotMail(pSrcItem) )
			return true;
		GetGameGuiManager()->AddClientSystemMessage("任务物品不能出售！");
		return true;
	}

	if( GetGameGuiManager()->m_guiShop )
	{
		stPointI pt(0,0);
		bool bRes = GetGameGuiManager()->m_guiShop->bItemCanput(pSrcItem,pt);
		if( bRes )
		{
			//设置出售物品价格
			char szTemp[MAX_PATH];
			_snprintf( szTemp, sizeof( szTemp ), "请输入总价 %s  ", pSrcItem->GetObjectName() );
			szTemp[MAX_PATH - 1] = 0;
			CGuiMoneyInputBox* pInputBox = GetGameGuiManager()->AddMoneyInputBox(szTemp,eAction_EnterShopSalePrice,pSrcItem);
			if( pInputBox )
			{
				pInputBox->SetDefaultMoney( pSrcItem->GetPrice(ITEM_PRICE_SHOP) );
			}
		}
		return true;
	}

	return true;
}

//从自己的摊位删除物品
bool DeleteItemFromShop(CRoleItem* pSrcItem)
{
	if(NULL == pSrcItem)
	{
		return true;
	}
	pSrcItem->OnRemove();
	delete pSrcItem;
	return true;
}

//从别人的摊位购买物品
bool BuyItemFromOthersShop(CRoleItem* pItem)
{
	if(NULL == pItem)
	{
		return true;
	}

	int price = pItem->GetPrice(ITEM_PRICE_SHOP);
	if( GetScene()->GetMainCharacter()->GetMoney() >= price )
	{
		char szTemp[MAX_PATH];
		_snprintf( szTemp, sizeof( szTemp ), "购买 %s(%d) 需要 %s ,确定要买吗?", pItem->GetObjectName(),pItem->GetCount(),Money2String(price) );
		szTemp[MAX_PATH - 1] = 0;
		GetGameGuiManager()->AddMessageBox(szTemp,eAction_BuyShopItem,pItem,c_szMBOKCancel);
		return true;
	}
	else
	{
		GetGameGuiManager()->AddClientSystemMessage("对不起,你的钱不够");
		return true;
	}
	return false;
}

DWORD GetItemTotalPrice(t_Object& obj)
{
	CRoleItem* pItem = GetItemByObject(obj);
	if( pItem )
	{
		int price = pItem->GetPrice(ITEM_PRICE_SHOP);
		if( 0 !=  price)
		{	
			return price;
		}
	}
	return obj.dwNum * obj.price;
}

bool OnMoveItemOnMailTable(CGuiTable* pTable)
{
	if( !pTable || pTable->m_iTableType != OBJECTCELLTYPE_MAIL )
	{
		return true;
	}
	if( (pTable->m_iTableID == 0) || (pTable->m_iTableID == 313) )	//发件箱 || 拍卖界面
	{
		CGuiFloatItem* pFloatItem = GetGuiManager()->GetFloatItem();
		if ( pFloatItem )
		{
			CRoleItem* pItem = (CRoleItem*)pFloatItem->GetItem()->m_pItemData;
			if ( pItem )
			{
				if( pItem->IsBind() ) // 不可丢弃类物品主要有灵魂绑定的物品
				{
					return true;
				}
				else  if ( pItem->GetItemType() == ItemType_Quest )//任务物品 
				{
					if( ClearPoint_CannotTradeAndMail(pItem->GetObjectBase()->dwID))
					{
						char szTip[40] ={0};
						if( pTable->m_iTableID == 0 )
							_snprintf(szTip,sizeof(szTip),"对不起,%s不可邮寄",pItem->GetObjectBase()->strName);
						else 
							_snprintf(szTip,sizeof(szTip),"对不起,%s不可拍卖",pItem->GetObjectBase()->strName);
						szTip[sizeof(szTip)-1] = 0;
						GetGameGuiManager()->AddClientSystemMessage(szTip);
					}
					return true;
				}
				//char szTemp[64] = {0};
				//_snprintf(szTemp,64,"%d",pItem->GetObject()->dwNum);
				//szTemp[63] = 0;
				//CGuiItemCell* pCell = pFloatItem->GetItem();
				//if( pCell && pCell->GetText() != szTemp )
				//{
				//	return true;
				//}

				stObjectLocation dst;

				dst.dwLocation = OBJECTCELLTYPE_MAIL;
				dst.dwTableID = pTable->m_iTableID;
				dst.x = 0;
				dst.y = 0;
				pItem->CopyTo( dst );
				pItem->MoveBackOldLocation();
				GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorNormal);
			}
		}
		return true;
	}

	return true;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  处理装备道具事件
*/
static bool OnEquipItemEvent( CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	CGuiItemCell* pDestCell = NULL;
	CRoleItem* pSrcItem = NULL;
	CRoleItem* pDestItem = NULL;

	stPointI ptDestGrid;
	stPointI ptDestPixel = pt;

	if (pTable->m_iTableType!=OBJECTCELLTYPE_COMMON)
		return false;

	if( pFloatItem = GetGuiManager()->GetFloatItem() )
		return false;

	// 计算目标格子
	if(pTable)
	{
		ptDestPixel.x -= pTable->GetX();
		ptDestPixel.y -= pTable->GetY();
		ptDestGrid = pTable->GetGridPosByPixelPos(ptDestPixel);
	}
	else
	{
		ptDestGrid = stPointI(0,0);
	}

	pDestCell = pTable->GetItemCell(ptDestGrid.x,ptDestGrid.y);
	if( !pDestCell )
		return false;

	if(!pDestCell->IsEnabled())
		return false;

	pSrcItem = (CRoleItem*)pDestCell->m_pItemData;

	if (!pSrcItem)
		return false;

	return EquipItem(pSrcItem);

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pTable : 描述
* \param uMsg : 描述
* \param pt : 描述
* \param wParam : 描述
* \param lParam : 描述
* \return 返回值的描述
*/
//Item table event treatment
bool ItemTableEvent(CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
{
	FUNCTION_BEGIN;

	if(!CRoleItem::s_bCanMoveItem) return false;

	CRoleItem* pSrcItem = GetSelectItem(pTable,pt);
	CMainCharacter* pMainRole = GetScene()->GetMainCharacter();

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		///触发"快捷使用物品"帮助教程事件
		if(bIsUserUnSetted(USER_SETTING_HELP_ACCELKEY))
		{
			if(IsAccelKeyObject(pTable,pt))
				ShowHelpTip(USER_SETTING_HELP_ACCELKEY);
		}

		if( GetGameCursor()->GetCursorTypeEx() == eCursorGetMailItem )
		{
			CGuiTable *pMailTable = NULL;
			if( GetGameGuiManager()->m_guiSendMailDlg )
			{
				pMailTable = GetGameGuiManager()->m_guiSendMailDlg->m_pItemTable;
			}
			if( pTable && pTable->m_iTableType == OBJECTCELLTYPE_COMMON )
			{
				CRoleItem* pMailItem = GetCellItem(pMailTable,stPointI(0,0));
				if( pSrcItem )
				{
					if( pMailItem )
					{
						//找到他的主物品
						CRoleItem* pItemImageMaster = pMailItem->GetImageMasterItem();
						if( pItemImageMaster )
						{
							pItemImageMaster->ClearItemImage();
						}		
					}
					stObjectLocation dst;
					dst.dwLocation = OBJECTCELLTYPE_MAIL;
					dst.dwTableID = 0;
					dst.x = 0;
					dst.y = 0;
					pSrcItem->CopyTo(dst);

					GetGameCursor()->SetCursorTypeEx(eCursorNormal) ;
					GetGameCursor()->SetCursor(CCursor::CursorType_Normal,eCursorNormal);
				}
			}
			return true;
		}

		if( pTable && pTable->m_iTableType == OBJECTCELLTYPE_COUNTRY_SAVEBOX)
		{
			if( GetGameGuiManager()->m_guiCountrySaveingBoxDlg)
				GetGameGuiManager()->m_guiCountrySaveingBoxDlg->OnLeftClickedOnItem();
			return true;
		}

		//对于邮件系统的物品禁止用鼠标直接拖动
		if( pTable && pTable->m_iTableType == OBJECTCELLTYPE_MAIL )
		{
			return OnMoveItemOnMailTable(pTable);
		}

		//对于自己的摊位格子,屏蔽鼠标左键
		if( pTable && pTable->m_iTableType == OBJECTCELLTYPE_MYSHOP )
		{
			return false;
		}

		//如果自己的摆摊界面打开且摊位没有打开,在包裹格子内选择物品
		if( NULL == GetGuiManager()->GetFloatItem() &&
			GetGameGuiManager()->m_guiShop && 
			GetGameGuiManager()->m_guiShop->IsVisible() &&
			!pMainRole->bIsShopStart() &&
			pTable && 			
			pTable->m_iTableType == OBJECTCELLTYPE_COMMON )
		{
			//从包裹移动物品到摊位
			return MoveItem2MyShop(pSrcItem);
		}

		//如果在对方的格子内选择物品,则表示买(自己没有摆摊的情况下)
		if( pTable && 
			pTable->m_iTableType == OBJECTCELLTYPE_OTHERSSHOP &&
			!pMainRole->bIsShopStart() )
		{
			return BuyItemFromOthersShop(pSrcItem);
		}

		//如果物品处于摆摊状态,其他所有操作都被限制了
		if( pSrcItem &&
			pMainRole->bIsShopStart() )
		{
			if( GetGameGuiManager()->m_guiShop &&  
				GetGameGuiManager()->m_guiShop->IsItemIn(pSrcItem->GetThisID()) )
			{
				GetGameGuiManager()->AddClientSystemMessage("该物品正在摊位内销售,不能执行");
				return true;
			}
		}
		if( pTable && 
			pTable->m_iTableType == OBJECTCELLTYPE_OTHERSSHOP )
		{
			return true;
		}



		//如果物品有镜像则禁止左键操作
		if ( pSrcItem )
		{
			CRoleItem* pItem = pSrcItem->GetRoleItemImage();
			if ( pItem )
			{
				return true;
			}
		}
	
		if( GetKeyState(VK_CONTROL) < 0 )
		{
			// 分裂物品
			if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_SELL)
				return false;
			return OnSplitItemEvent(pTable,uMsg,pt,wParam,lParam);
		}
		else
		{
			if (GetGameCursor()->GetCursorImageType()==eCursorRepair)
			{
				return OnRepairItemEvent(pTable,uMsg,pt,wParam,lParam);
			}
			else
			{
				//GetGameCursor()->SetCursor( CCursor::CursorType_Drag, eCursorNormal );
				if ( GetKeyState( VK_SHIFT) < 0 )
				{
					//for item exchange
					return AddItemToExchange( pTable, pt );
				}
				else
				{
					return OnMoveItemEvent(pTable,uMsg,pt,wParam,lParam);
				}
			}
		}
		return false;
		break;
	case WM_RBUTTONDOWN:
		{
			//对于邮件系统的物品禁止用鼠标直接拖动
			if( pTable && pTable->m_iTableType == OBJECTCELLTYPE_MAIL )
			{
				return true;
			}

			//如果物品处于摆摊状态,其他所有操作都被限制了
			if( pSrcItem &&
				pMainRole->bIsShopStart() )
			{
				if( GetGameGuiManager()->m_guiShop &&  
					GetGameGuiManager()->m_guiShop->IsItemIn(pSrcItem->GetThisID()) )
				{
					GetGameGuiManager()->AddClientSystemMessage("该物品正在摊位内销售,不能执行");
					return true;
				}
			}
			//如果是自己的摊位格子
			if( pTable &&
				pTable->m_iTableType == OBJECTCELLTYPE_MYSHOP)
			{				
				if ( !pMainRole->bIsShopStart() )
				{
					//如果在摊位格子里面点则从摊位删除该物品
					return DeleteItemFromShop(pSrcItem);						
				}			
				else
				{
					GetGameGuiManager()->AddClientSystemMessage("该物品正在摊位内销售,不能执行");
					return true;
				}
			}
			if( pTable && 
				pTable->m_iTableType == OBJECTCELLTYPE_OTHERSSHOP )
			{
				return true;
			}
			//如果物品有镜像则禁止右键操作
			if ( pSrcItem )
			{
				CRoleItem* pItem = pSrcItem->GetRoleItemImage();
				if ( pItem )
				{
					return true;
				}
			}

			if(pTable && pTable->m_iTableType == OBJECTCELLTYPE_SELL)
			{
				return OnMoveItemEvent(pTable,uMsg,pt,wParam,lParam);
			}

			if (OnEquipItemEvent(pTable,uMsg,pt,wParam,lParam)==false)
			{				
				return OnUseItemEvent(pTable,uMsg,pt,wParam,lParam);
			}

			//;
			//if ( EquipItem(pSrcItem) == false )
			//{				
			//	return OnUseItemEvent(pTable,uMsg,pt,wParam,lParam);
			//}
			return true;
		}
		break;
	case WM_LBUTTONUP:		
		break;
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
CRoleItem::CRoleItem(void)
: m_pObjectBase(NULL)
, m_bNeedUpdateTips(false)
, m_dwTipFlags(0)
, m_dwLastUseTime(0)
, m_price(0)
{
	FUNCTION_BEGIN;

	memset(&m_object,0,sizeof(m_object));

	m_ToolTips.SetAlignType( GUI_ALIGN_LEFT );
	m_ToolTips.SetLineGaps(3);

	m_bIsImage = false;
	m_pImageItem = NULL;
	m_pMasterItem = NULL;

	FUNCTION_END;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \return 返回值的描述
*/
CRoleItem::~CRoleItem(void)
{
	FUNCTION_BEGIN;

    ClearItemImage();
	m_ToolTips.Clear();	

	//如果本身是镜像,在自己析构的时候使主物品指向自己的指针为null
	if ( m_pMasterItem )
	{
		m_pMasterItem->m_pImageItem = NULL;
	}

	FUNCTION_END;
}

void CRoleItem::ClearItemImage()
{
	if( m_pImageItem )
	{
		m_pImageItem->OnRemove();
		delete m_pImageItem;
		m_pImageItem = NULL;
	}
}

void CRoleItem::Copy(CRoleItem* pItem)
{
	if ( NULL == pItem ) return ;
	//t_Object obj = pItem->GetTObject();
	m_object = pItem->GetTObject();
	m_pObjectBase = pItem->GetObjectBase();
	m_bNeedUpdateTips = true;
	m_bIsImage = true;		//现在就成了镜像了
	m_pMasterItem = pItem;	
}

void CRoleItem::CopyTo( const stObjectLocation & dst )
{
	//镜像不能再copy了
	if ( m_bIsImage ) return ;
	m_pImageItem = new CRoleItem();
	if( m_pImageItem )
	{
		CGuiTable* pTable = ::GetItemTable(dst);
		if( pTable )
		{
			CRoleItem* pItem = ::GetCellItem(pTable,stPointI(dst.x,dst.y));
			if( pItem )
			{
				//如果想再copy一遍则直接返回
				if (pItem->GetThisID() == GetThisID())
				{
					delete m_pImageItem;
					return;
				}
				pItem->OnRemove();
				delete pItem;
				pItem = NULL;
			}
		}
		//m_pImageItem->SetObject(GetObject());
		//m_pImageItem->m_pMasterItem = this;
		m_pImageItem->Copy( this );
		m_pImageItem->GetObject()->pos = dst;
		m_pImageItem->OnUpdate(-1);
	}
}

bool CRoleItem::SetObject(t_Object* pObj)
{
	FUNCTION_BEGIN;

	if( NULL == pObj )
	{
		return false;
	}

	m_object = *pObj;

	if (m_object.upgrade > c_maxItemUpgrade )
		m_object.upgrade = c_maxItemUpgrade;

	m_pObjectBase = ::GetObjectBase(m_object.dwObjectID);
	Assert(m_pObjectBase);
	if(!m_pObjectBase)
		return false;

	SetName(m_object.strName);
	m_bNeedUpdateTips = true;

	OnUpdate(-1);

	if(m_object.qwThisID > s_dwMaxThisID)
		s_dwMaxThisID = m_object.qwThisID;


	switch(m_object.pos.dwLocation) 
	{
	case OBJECTCELLTYPE_EQUIP:
		{
			if (GetGameGuiManager()->m_guiUserInfo)
			{
				GetGameGuiManager()->m_guiUserInfo->SetUserImage(m_object.pos.y,&m_object);
			}

			GetGameGuiManager()->AddPackage(m_object.pos,this);
			GetGameGuiManager()->AddMakeTo(m_object.pos,this);
		}
		break;
	case OBJECTCELLTYPE_EQUIPSHOW:
		{
			if (GetGameGuiManager()->m_guiUserInfoShow)
			{
				GetGameGuiManager()->m_guiUserInfoShow->SetUserImage(m_object.pos.y,&m_object);
			}
		}
		break;
	}


	//Add to repair list
	if ( NeedConsiderDur() )
	{
		if ( ((m_object.dur+49)/50 < NEED_ADD_REPAIR_VALUE) )
		{
			if ( GetScene()->GetMainCharacter() )
				GetScene()->GetMainCharacter()->AddOneNeedRepairItem( m_object.qwThisID );
		}
		else
		{
			if ( GetScene()->GetMainCharacter() )
				GetScene()->GetMainCharacter()->DeleteOneNeedRepairItem( m_object.qwThisID );
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
* \param pCmd : 描述
* \return 返回值的描述
*/
bool CRoleItem::SetAttrib(stAddObjectPropertyUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	return SetObject(&(pCmd->object));
	return true;

	FUNCTION_END;
}


void CRoleItem::DestoryAllVirtualCell()
{
	if(!GetGameGuiManager()->m_guiItem) return;
	tCells::iterator it = m_cells.begin();
	++it;
	for( ; it != m_cells.end(); )
	{
		stCellInfo& info = (*it);

		if(info.pCell->m_pTable->m_pDialog == GetGuiManager()->GetFloatItem())
		{
			GetGuiManager()->SetMouseDlg(NULL);
		}

		info.pCell->m_pTable->RemoveItem(info.pCell->GetPos().x,info.pCell->GetPos().y,true);
		
		it = m_cells.erase(it);
	}
	if(!m_cells.empty())
	{
		m_cells.front().pCell->SetVisible(true);
		m_cells.front().pCell->SetEnabled(true);
		m_cells.front().count = GetCount();
		if(GetMaxCount() > 1 && GetCount() > 1)
		{
			m_cells.front().pCell->SetText(avar("%d",GetCount()));
		}
		else
		{
			m_cells.front().pCell->SetText("");
		}
	}
}

void CRoleItem::DestoryAllCell()
{
	for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		stCellInfo& info = (*it);

		if(info.pCell->m_pTable)
		{
			if(info.pCell->m_pTable->m_pDialog == GetGuiManager()->GetFloatItem())
			{
				GetGuiManager()->SetMouseDlg(NULL);
			}

			info.pCell->m_pTable->RemoveItem(info.pCell->GetPos().x,info.pCell->GetPos().y,true);
		}
	}
	m_cells.clear();
}

void CRoleItem::AccessKeyOnRemove( const DWORD newItemID )
{
	//Remove all access key of this item
	if ( GetGameGuiManager()->m_guiMain )
	{
		if ( -1 == newItemID )
		{
			DWORD typeID = IsEquipment() ? GetThisID() : GetObjectID();
			GetGameGuiManager()->m_guiMain->DeleteAccessKey( IsEquipment() ? GameAccelKeyType_Equipment : GameAccelKeyType_Item, typeID );
		}
		else
		{
			if ( GetItemType() == ItemType_FillHP )
				GetGameGuiManager()->m_guiMain->ReplaceAccessKey( GameAccelKeyType_Item, m_object.qwThisID, newItemID );
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
void CRoleItem::OnRemove()
{
	FUNCTION_BEGIN;

	if(IsMoney())
	{
		SetMoneyText(GetLocation().dwLocation,0);
	}
	/*if(GetGameState()->IsStateItem(eGameStateItem_TradeRun) && IsTradeCell())
	{
		GetGameGuiManager()->m_guiTrade->OnItemChange();
	}*/
	
	//Clear user info equip pos img
	if ( m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP )
	{
		if (GetGameGuiManager()->m_guiUserInfo)
		{
			GetGameGuiManager()->m_guiUserInfo->SetUserImage(m_object.pos.y);
		}
	}

	//包裹离开装备位置
	{
		CGuiTable* pTable = NULL;
		if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(m_object.pos.y==EQUIPCELLTYPE_PACKAGE))
		{
			stObjectLocation pos;
			pos.dwLocation = OBJECTCELLTYPE_PACKAGE;
			pos.dwTableID = m_object.qwThisID;
			pTable = GetItemTable(pos);
		}
		if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(m_object.pos.y==EQUIPCELLTYPE_MAKE))
		{
			stObjectLocation pos;
			pos.dwLocation = OBJECTCELLTYPE_MAKE;
			pos.dwTableID = m_object.qwThisID;
			pos.y = MAKECELLTYPE_EQUIP;
			pTable = GetItemTable(pos);
		}
		if (pTable)
		{
			if (pTable->m_pDialog)
			{
				pTable->m_pDialog->m_bCloseIsHide = false;
				pTable->m_pDialog->Close();
			}
		}
	}

	DestoryAllCell();

	/*if(m_pCell)
	{
	CGuiTable* pGuiTable = m_pCell->m_pTable;
	Assert(pGuiTable);
	POINT pos = m_pCell->GetPos();
	pGuiTable->RemoveItem(pos.x,pos.y,true);
	if(GetLocation().dwLocation == OBJECTCELLTYPE_MOUSE)
	{
	GetGuiManager()->SetMouseDlg(NULL);
	}
	}*/

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
void CRoleItem::OnSplit(stSplitObjectPropertyUserCmd* pCmd)
{
	FUNCTION_BEGIN;
	//宝合一个个放的功能
	//if ((pCmd->dst.dwLocation != OBJECTCELLTYPE_MAKE)||(m_object.dwNum == pCmd->dwNum))
	{
		DestoryAllVirtualCell();
	}

	CRoleItem* pDstItem = GetScene()->GetMainCharacter()->FindItemByLocation(pCmd->dst);
	DWORD num = 0;
	stObjectLocation m_oldPos = m_object.pos;

	if(pDstItem)
	{
		Assert(pDstItem->GetObjectBase()->dwID == m_object.dwObjectID);

		//Assert(0);
		//GetScene()->GetMainCharacter()->RemoveItem(pDstItem->GetThisID());

		num = pDstItem->GetCount();
	}

	Assert(pCmd->dwNum <= m_object.dwNum);

	m_object.dwNum -= pCmd->dwNum;

	stAddObjectPropertyUserCmd cmd;
	cmd.byActionType = EQUIPACTION_INIT;
	cmd.object = m_object;
	cmd.object.qwThisID = pCmd->qwNewThisID;
	cmd.object.dwNum = pCmd->dwNum + num;
	cmd.object.pos = pCmd->dst;

	if(m_object.dwNum == 0)
	{
		GetScene()->GetMainCharacter()->RemoveItem(GetThisID());
	}
	else
	{
		//宝合一个个放的功能
		//if (pCmd->dst.dwLocation == OBJECTCELLTYPE_MAKE)
		//	OnUpdate(UPDATE_FLAG_MAKECOUNT);
		//else
			OnUpdate(UPDATE_FLAG_COUNT);
	}

	GetScene()->GetMainCharacter()->AddItem(&cmd);

	/*if(GetGameState()->IsStateItem(eGameStateItem_TradeRun) && 
		( ::IsTradeCell(pCmd->dst.dwLocation) || IsTradeCell()) )
	{
		GetGameGuiManager()->m_guiTrade->OnItemChange();
	}*/

	if ((pCmd->dst.dwLocation==OBJECTCELLTYPE_MAKE)||(m_oldPos.dwLocation==OBJECTCELLTYPE_MAKE))
	{
		if (GetGameGuiManager()->m_guiMakeTo1)
		{
			GetGameGuiManager()->m_guiMakeTo1->UpdateData(true);
		}
		if (GetGameGuiManager()->m_guiMakeTo2)
		{
			GetGameGuiManager()->m_guiMakeTo2->UpdateData(true);
		}
	}

	FUNCTION_END;
}

bool CRoleItem::SetTrueCellCount(int count)
{
	if(m_cells.empty()) return false;
	m_cells.front().count = count;
	m_cells.front().pCell->SetText(avar("%d",count));
	return true;
}


/**
* \brief  为物品增加一个显示位置
*
*
*
* \return  无
*
*/
void CRoleItem::AddShowLocation(CGuiItemCell* pSrcCell,int count,const stObjectLocation & dst,bool bHideSrcLocation,bool bDisableSrcLocation)
{
	CGuiTable* pDestTable = GetItemTable(dst);
	Assert(pDestTable);

	CGuiItemCell* pDestCell = NULL;
	stRectI rect(dst.x,dst.y,dst.x + GetObjectBase()->cx,dst.y + GetObjectBase()->cy);
	int count1 = pDestTable->GetRectItem(&rect,&pDestCell,1);
	Assert(count1 <= 1);
	Assert(pDestCell == NULL);
	if(pDestCell)
	{
		pDestTable->RemoveItem(pDestCell->GetPos().x,pDestCell->GetPos().y,true);
	}

	pDestCell = new CGuiItemCell;
	pDestCell->m_pItemData = this;
	//pDestCell->SetPos(0,0);     //zzf

	pDestCell->SetPos(dst.x,dst.y);
	pDestCell->SetSize(GetObjectBase()->cx,GetObjectBase()->cy);

	pDestTable->AddItem(pDestCell);


	OnAddCell(pDestCell,count);

	if( GetObjectBase()->dwMaxCount > 1 && GetCount() > 1)
	{
		int num = GetCount() - count;
		SetTrueCellCount(num);
		pDestCell->SetText(avar("%d",count));

	}
	
	if (dst.dwLocation == OBJECTCELLTYPE_MOUSE) //zzf
	{
		GetGuiManager()->SetMouseDlg(GetGuiFloatItem()); 
	}
	else
	{
		//GetGuiManager()->SetMouseDlg(NULL); 
	}

	// 如果不是原始的格子，则删除
	CGuiItemCell* pCell;
	GetTrueCell(pCell,count1);
	if(pSrcCell != pCell)
	{
		pCell->m_pTable->RemoveItem(pCell->GetPos().x,pCell->GetPos().y,true);
	}
	OnUpdate(UPDATE_FLAG_IMAGE);
	if(bHideSrcLocation)
		pSrcCell->SetVisible(false);
	if(bDisableSrcLocation)
		pSrcCell->SetEnabled(false);
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param pCmd : 描述
* \return 返回值的描述
*/
void CRoleItem::OnSwap(stSwapObjectPropertyUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	DestoryAllVirtualCell();

	if ( pCmd->dst.dwLocation == OBJECTCELLTYPE_STORE )
		AccessKeyOnRemove();

	/*if(GetGameState()->IsStateItem(eGameStateItem_TradeRun) && 
		( ::IsTradeCell(pCmd->dst.dwLocation) || IsTradeCell()) )
	{
		GetGameGuiManager()->m_guiTrade->OnItemChange();
	}*/

	CGuiTable* pDestTable = GetItemTable(pCmd->dst);
	stObjectLocation m_oldPos = m_object.pos;
	Assert(pDestTable);

	DWORD dwFlags = UPDATE_FLAG_LOCATION;
	if(pCmd->dst.dwLocation != GetLocation().dwLocation)
		dwFlags |= UPDATE_FLAG_IMAGE;
	if ( IsArrow() )
		dwFlags |= UPDATE_FLAG_COUNT;

	if(pDestTable)
	{
		CGuiItemCell* pDestCell = NULL;
		stRectI rect(pCmd->dst.x,pCmd->dst.y,pCmd->dst.x + GetObjectBase()->cx,pCmd->dst.y + GetObjectBase()->cy);
		if(pCmd->dst.dwLocation == OBJECTCELLTYPE_STORE)
		{
			int y = pCmd->dst.y % CGuiSavingBoxDialog::csShowRowCount;
			rect.SetRect(pCmd->dst.x,y,pCmd->dst.x + GetObjectBase()->cx,y + GetObjectBase()->cy);
		}
		int count = pDestTable->GetRectItem(&rect,&pDestCell,1);
		Assert(count <= 1);

		if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(dwFlags & UPDATE_FLAG_IMAGE))
		{
			if (GetGameGuiManager()->m_guiUserInfo)
			{
				GetGameGuiManager()->m_guiUserInfo->SetUserImage(m_object.pos.y);
			}
		}

		if ((pDestTable->m_iTableType==OBJECTCELLTYPE_EQUIP)&&(dwFlags & UPDATE_FLAG_IMAGE))
		{
			if (GetGameGuiManager()->m_guiUserInfo)
			{
				GetGameGuiManager()->m_guiUserInfo->SetUserImage(pDestTable->m_EquipPosition.y,&m_object);
			}
		}

		
		if( pDestCell )
		{
			// 交换位置
			pDestTable->RemoveItem(pDestCell->GetPos().x,pDestCell->GetPos().y,false);
			pDestCell->m_pTable = NULL;

			CRoleItem* pDestItem = ( CRoleItem* )pDestCell->m_pItemData;
			Assert( pDestItem );

			if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(dwFlags & UPDATE_FLAG_IMAGE))
			{
				if (GetGameGuiManager()->m_guiUserInfo)
				{
					GetGameGuiManager()->m_guiUserInfo->SetUserImage(m_object.pos.y,pDestItem->GetObject());
				}
			}
			
			//包裹离开装备位置
			{
				CGuiTable* pTable = NULL;
				if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(m_object.pos.y==EQUIPCELLTYPE_PACKAGE))
				{
					stObjectLocation pos;
					pos.dwLocation = OBJECTCELLTYPE_PACKAGE;
					pos.dwTableID = m_object.qwThisID;
					pTable = GetItemTable(pos);
				}
				if ((pDestItem->m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(pDestItem->m_object.pos.y==EQUIPCELLTYPE_PACKAGE))
				{
					stObjectLocation pos;
					pos.dwLocation = OBJECTCELLTYPE_PACKAGE;
					pos.dwTableID = pDestItem->m_object.qwThisID;
					pTable = GetItemTable(pos);
				}
				if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(m_object.pos.y==EQUIPCELLTYPE_MAKE))
				{
					stObjectLocation pos;
					pos.dwLocation = OBJECTCELLTYPE_MAKE;
					pos.dwTableID = pDestItem->m_object.qwThisID;
					pos.y = MAKECELLTYPE_EQUIP;
					pTable = GetItemTable(pos);
				}
				if (pTable)
				{
					pTable->m_pDialog->m_bCloseIsHide = false;
					pTable->m_pDialog->Close();
				}
			}
			

			pDestItem->m_object.pos = m_object.pos;

			m_object.pos = pCmd->dst;
			OnUpdate(dwFlags);

			if ( pDestItem->IsArrow() )
				pDestItem->OnUpdate(dwFlags | UPDATE_FLAG_COUNT);
			else
				pDestItem->OnUpdate(dwFlags);
			
			//包裹加入装备位置
			{
				if (m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)
				{
					GetGameGuiManager()->AddPackage(m_object.pos,this);
					GetGameGuiManager()->AddMakeTo(m_object.pos,this);
				}
				if (pDestItem->m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)
				{
					GetGameGuiManager()->AddPackage(pDestItem->m_object.pos,pDestItem);
					GetGameGuiManager()->AddMakeTo(pDestItem->m_object.pos,pDestItem);
				}
			}

		}
		else
		{
			if( m_object.pos.dwLocation == OBJECTCELLTYPE_MOUSE )
			{
				GetGuiManager()->SetMouseDlg(NULL);
			}
			
			//包裹离开装备位置
			{
				CGuiTable* pTable = NULL;
				if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(m_object.pos.y==EQUIPCELLTYPE_PACKAGE))
				{
					stObjectLocation pos;
					pos.dwLocation = OBJECTCELLTYPE_PACKAGE;
					pos.dwTableID = m_object.qwThisID;
					pTable = GetItemTable(pos);
				}
				if ((m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)&&(m_object.pos.y==EQUIPCELLTYPE_MAKE))
				{
					stObjectLocation pos;
					pos.dwLocation = OBJECTCELLTYPE_MAKE;
					pos.dwTableID = m_object.qwThisID;
					pos.y = MAKECELLTYPE_EQUIP;
					pTable = GetItemTable(pos);
				}
				if (pTable)
				{
					pTable->m_pDialog->m_bCloseIsHide = false;
					pTable->m_pDialog->Close();
				}
			}

			m_object.pos = pCmd->dst;
			OnUpdate(dwFlags);

			//包裹加入装备位置
			if (m_object.pos.dwLocation==OBJECTCELLTYPE_EQUIP)
			{
				GetGameGuiManager()->AddPackage(m_object.pos,this);
				GetGameGuiManager()->AddMakeTo(m_object.pos,this);
			}
		}

		if ((pDestTable->m_iTableType==OBJECTCELLTYPE_MAKE)||(m_oldPos.dwLocation==OBJECTCELLTYPE_MAKE))
		{
			if (GetGameGuiManager()->m_guiMakeTo1)
			{
				GetGameGuiManager()->m_guiMakeTo1->UpdateData(true);
			}
			if (GetGameGuiManager()->m_guiMakeTo2)
			{
				GetGameGuiManager()->m_guiMakeTo2->UpdateData(true);
			}
		}

		if ((pDestTable->m_iTableType==OBJECTCELLTYPE_EQUIP)||(m_oldPos.dwLocation==OBJECTCELLTYPE_EQUIP))
		{
			GetScene()->GetMainCharacter()->RefreshActiveItem(OBJECTCELLTYPE_EQUIP);
		}
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
void CRoleItem::OnSetCount(stRefCountObjectPropertyUserCmd* pCmd)
{
	FUNCTION_BEGIN;

	m_object.dwNum = pCmd->dwNum;
	OnUpdate(UPDATE_FLAG_COUNT);
	m_bNeedUpdateTips = true;

	/*if(GetGameState()->IsStateItem(eGameStateItem_TradeRun) && 
		IsTradeCell() )
	{
		GetGameGuiManager()->m_guiTrade->OnItemChange();
	}*/

	FUNCTION_END;
}

void CRoleItem::OnRemoveCell(CGuiItemCell* pCell)
{
	tCells::iterator it = FindCell(pCell);
	if(it != m_cells.end())
	{
		if( GetCount() > 1 )
		{
			SetTrueCellCount(m_cells.front().count + (*it).count);
		}

		if(pCell->m_pTable->m_pDialog == GetGuiManager()->GetFloatItem())
			GetGuiManager()->SetMouseDlg(NULL);

		m_cells.erase(it);
	}
	if(m_cells.size() == 1) 
	{
		m_cells.front().pCell->SetVisible(true);
		m_cells.front().pCell->SetEnabled(true);
	}
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwFlags : 描述
 * \return 返回值的描述
 */
void CRoleItem::OnUpdate(DWORD dwFlags)
{
	FUNCTION_BEGIN;

	if(IsMoney())
	{
		SetMoneyText(GetLocation().dwLocation,GetCount());
		return;
	}

	CGuiTable* pTable = GetItemTable(m_object.pos);
	//Assert(pTable);
	if(!pTable) return;

	if(m_cells.empty())
	{
		CGuiItemCell* pCell = new CGuiItemCell;
		pCell->m_pItemData = this;
		SetTrueCell(pCell,GetCount());
	}

	CGuiItemCell* pCell = NULL;
	int count1;
	GetTrueCell(pCell,count1);
	Assert(pCell);
	if(!pCell) return ;

	if(dwFlags & UPDATE_FLAG_LOCATION)
	{
		DestoryAllVirtualCell();

		if(pCell->m_pTable)
		{
			pCell->m_pTable->RemoveItem(pCell->GetPos().x,pCell->GetPos().y,false);
			pCell->m_pTable = NULL;
		}

		pCell->SetSize(GetObjectBase()->cx,GetObjectBase()->cy);

		stPointI pos = stPointI(m_object.pos.x,m_object.pos.y);
		
		/*
		*仓库的格子处理起来比较复杂,仓库有3个页面,把它当作一个页面来处理，
		*第二个页面的y坐标也就是实际的y坐标加上第一个页面的高度
		*第三个页面类推
		*根据服务端发来的y坐标,计算得到相应的格子,然后把物品放到仓库对应的页面上的相应格子
		*例如pos.y == 6表示第二页的第1行
		*/
		if( pTable->m_iTableType == OBJECTCELLTYPE_STORE )
		{
			pos.y %= CGuiSavingBoxDialog::csShowRowCount;
		}
		pCell->SetPos(pos.x,pos.y);

		pTable->AddItem(pCell);

		if(m_object.pos.dwLocation == OBJECTCELLTYPE_MOUSE)
		{
			GetGuiManager()->SetMouseDlg(GetGuiFloatItem());
		}
	}

	if( dwFlags & UPDATE_FLAG_IMAGE )
	{
		SetImage();
	}
	
	//Get or Update hole image
	if ( dwFlags & UPDATE_FLAG_IMAGE )			//UPDATE_FLAG_HOLE
	{
		//SetHoleImage();
	}

	if(dwFlags & UPDATE_FLAG_COUNT)
	{

		DestoryAllVirtualCell();

		char szText[256];

		DWORD num = 0;
		if ( IsArrow() )
		{
			if ( m_object.qwThisID == INVALID_THISID )
			{
				if ( m_pObjectBase )
					num = m_pObjectBase->dwMaxDur;
			}
			else
				num = m_object.dur;
		}		
		else
			num = m_object.dwNum;

		if(num > 1)
		{
			sprintf(szText,"%u",num);
		}
		else
		{
			szText[0] = 0;
		}
		pCell->SetText(szText);
	}

	if(dwFlags & UPDATE_FLAG_MAKECOUNT)
	{
		char szText[256];
		DWORD num = IsArrow()?m_object.dur:m_object.dwNum;
		if(num > 1)
		{
			sprintf(szText,"%u",num);
		}
		else
		{
			szText[0] = 0;
		}

		for( tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		{
			stCellInfo& info = (*it);
			Assert(info.pCell);
			if(info.pCell && info.pCell->m_pTable && info.pCell->m_pTable->m_pDialog == GetGuiManager()->GetFloatItem())
			{
				info.count = GetCount();
				info.pCell->SetText(szText);
				break;
			}
			
		}
	}

	FUNCTION_END;
}

DWORD CRoleItem::GetExteriorCount()
{
	return ( IsArrow() ? m_object.dur : m_object.dwNum );
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param  : 描述
 * \return 返回值的描述
 */
//Show or hide hole image
void CRoleItem::ShowHoleImage(bool bHide /* = false  */)
{
	FUNCTION_BEGIN;

	//show or hide hole
	for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		(*it).pCell->ShowImageByType( CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE, bHide );
		//show or hide diamond
		(*it).pCell->ShowImageByType( CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND, bHide );
	}

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param id : 描述
 * \return 返回值的描述
 */
int GetDiamondResourceFrameFromID( DWORD id )
{
	switch( id )
	{
	case 680:
		return 9;
		break;
	case 748:
		return 10;
        break;
	case 749:
		return 11;
		break;
	case 750:
		return 12;
		break;
	case 751:
		return 13;
		break;
	}

	return -1;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
//Get or Update hole image
void CRoleItem::SetHoleImage()
{
	FUNCTION_BEGIN;

	//Thie time we set a special res location
	stResourceLocation rHole;
	rHole.SetFileName( "data\\interfaces.gl" );
	rHole.group = 4;
	rHole.frame = 8;

	stResourceLocation rDiamond;
	rDiamond.SetFileName( "data\\interfaces.gl" );
	rDiamond.group = 4;
	rDiamond.frame = 9;		//167

	CGuiItemCell* pCell;int count1;
	GetTrueCell(pCell,count1);
	Assert(pCell);
	//see if this res empty
	if ( !rHole.IsEmpty() )
	{
		//Get diamond num and hole position
		int numHole = 0;
		//hole size scale
		float scaleHole = 0.5f;
		float sizeHole = 50.0f;
		sizeHole *= scaleHole;
		float sizeHoleHalf = 0.5f * sizeHole;

		//item position
		stPointI sizeItem;
		stPointI offsetCell;
		ObjectBase_t* pObjectBase = ::GetObjectBase( m_object.dwObjectID );
		assert( pObjectBase );
		if( pObjectBase )
		{
			//diamond num
			for( int i=0;i<6;i++ )
			{
				if ( m_object.socket[i] != 0 )
					numHole++ ;
			}
			if ( pCell->m_pTable->m_bVirtual )
			{
				offsetCell.x = pCell->GetImage()->GetFrame(0)->GetClipRect().left;
				offsetCell.y = pCell->GetImage()->GetFrame(0)->GetClipRect().top;
				sizeItem.x = pCell->GetImage()->GetFrame(0)->GetClipRect().right - pCell->GetImage()->GetFrame(0)->GetClipRect().left;
				sizeItem.y = pCell->GetImage()->GetFrame(0)->GetClipRect().bottom - pCell->GetImage()->GetFrame(0)->GetClipRect().top;
			}
			else
			{
				offsetCell.x = 0;
				offsetCell.y = 0;
				sizeItem.x = pObjectBase->cx * CGuiTable::s_ctrlCellSize.cx;
				sizeItem.y = pObjectBase->cy * CGuiTable::s_ctrlCellSize.cy;
			}  			
		}

		stRectI rcHoleImage;
		rcHoleImage.SetRect( 0, 0, sizeHole, sizeHole );
		//We add hole and diamond
		//numHole = 3;
		switch( numHole )
		{
		case 0:		//Item has no socket
			break;
		case 1:
			{
				stRectI rcHolePos;
				//add hole
				rcHolePos.SetRect( sizeItem.x / 2.0f - sizeHoleHalf, sizeItem.y / 2.0f - sizeHoleHalf, sizeItem.x / 2.0f + sizeHoleHalf, sizeItem.y / 2.0f + sizeHoleHalf );
				rcHolePos.OffsetRect( offsetCell );
				//Then we add diamond
				for( int i=0;i<1;i++ )
				{
					for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
					{
						(*it).pCell->AddAnimation( rHole, -1, NULL, &rcHolePos, false, CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE );
						if ( (m_object.socket[i] != -1) && (m_object.socket[i] != 0) )		//
						{
							//cur hole has diamond
							/*ObjectBase_t* pDiamond = ::GetObjectBase( m_object.socket[i] );
							if ( !pDiamond )
							{
								continue;
							}
							stResourceLocation rDiamond = GetObjectBasePicLocation( pDiamond );*/
							//rDiamond.frame = 304;
							stRectI rcDiamond = rcHolePos;
							if ( GetDiamondResourceFrameFromID( m_object.socket[i] ) >= 0 )
								rDiamond.frame = GetDiamondResourceFrameFromID( m_object.socket[i] );
							else
								rDiamond.frame = 9;
							//add diamond
							(*it).pCell->AddAnimation( rDiamond, -1, NULL, &rcDiamond, false, CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND );
						}
					}
				}
			}
			break;
		case 2:
			{
				stRectI rcHolePos[2];
				//add hole
				float gapsY = (sizeItem.y-2.0f*sizeHole) / 3.0f;
				rcHolePos[0].SetRect( sizeItem.x / 2.0f - sizeHoleHalf, gapsY, sizeItem.x / 2.0f + sizeHoleHalf, gapsY+sizeHole );
				rcHolePos[1].SetRect( sizeItem.x / 2.0f - sizeHoleHalf, 2.0f*gapsY+sizeHole, sizeItem.x / 2.0f + sizeHoleHalf, sizeItem.y-gapsY );
				rcHolePos[0].OffsetRect( offsetCell );
				rcHolePos[1].OffsetRect( offsetCell );
				//Then we add diamond
				int diamondNum = 0;
				for( int i=0;i<2;i++ )
				{
					for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
					{
						(*it).pCell->AddAnimation( rHole, -1, NULL, &rcHolePos[i], false, CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE );
						if ( (m_object.socket[i] != -1) && (m_object.socket[i] != 0) )
						{
							//cur hole has diamond
							/*ObjectBase_t* pDiamond = ::GetObjectBase( m_object.socket[i] );
							if ( !pDiamond )
							{
								continue;
							}
							stResourceLocation rDiamond = GetObjectBasePicLocation( pDiamond );*/
							stRectI rcDiamond = rcHolePos[diamondNum];
							if ( GetDiamondResourceFrameFromID( m_object.socket[i] ) >= 0 )
								rDiamond.frame = GetDiamondResourceFrameFromID( m_object.socket[i] );
							else
								rDiamond.frame = 9;
							//add diamond
							(*it).pCell->AddAnimation( rDiamond, -1, NULL, &rcDiamond, false, CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND );
							diamondNum++ ;
						}
					}
				}
			}
			break;
		case 3:
			{
				stRectI rcHolePos[3];
				//add hole
				float gapsY = (sizeItem.y-3.0f*sizeHole) / 4.0f;
				rcHolePos[0].SetRect( sizeItem.x / 2.0f - sizeHoleHalf, gapsY, sizeItem.x / 2.0f + sizeHoleHalf, gapsY+sizeHole );
				rcHolePos[1].SetRect( sizeItem.x / 2.0f - sizeHoleHalf, 2.0f*gapsY+sizeHole, sizeItem.x / 2.0f + sizeHoleHalf, 2.0f*(gapsY+sizeHole) );
				rcHolePos[2].SetRect( sizeItem.x / 2.0f - sizeHoleHalf, 3.0f*gapsY+2.0f*sizeHole, sizeItem.x / 2.0f + sizeHoleHalf, sizeItem.y-gapsY );
				rcHolePos[0].OffsetRect( offsetCell );
				rcHolePos[1].OffsetRect( offsetCell );
				rcHolePos[2].OffsetRect( offsetCell );
				//Then we add diamond
				int diamondNum = 0;
				for( int i=0;i<3;i++ )
				{
					for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
					{
						(*it).pCell->AddAnimation( rHole, -1, NULL, &rcHolePos[i], false, CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE );
						if ( (m_object.socket[i] != -1) && (m_object.socket[i] != 0) )		//
						{
							////cur hole has diamond
							//ObjectBase_t* pDiamond = ::GetObjectBase( m_object.socket[i] );
							//if ( !pDiamond )
							//{
							//	continue;
							//}
							//stResourceLocation rDiamond = GetObjectBasePicLocation( pDiamond );
							stRectI rcDiamond = rcHolePos[diamondNum];
							if ( GetDiamondResourceFrameFromID( m_object.socket[i] ) >= 0 )
								rDiamond.frame = GetDiamondResourceFrameFromID( m_object.socket[i] );
							else
								rDiamond.frame = 9;
							//add diamond
							(*it).pCell->AddAnimation( rDiamond, -1, NULL, &rcDiamond, false, CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND );
							diamondNum++ ;
						}
					}
				}
			}
			break;
		case 4:
			{
				stRectI rcHolePos[4];
				//add hole
				float gapsX = (sizeItem.x-2.0f*sizeHole) / 3.0f;
				float gapsY = (sizeItem.y-2.0f*sizeHole) / 3.0f;
				rcHolePos[0].SetRect( gapsX, gapsY, gapsX+sizeHole, gapsY+sizeHole );
				rcHolePos[1].SetRect( 2.0f*gapsX+sizeHole, gapsY, sizeItem.x-gapsX, gapsY+sizeHole );
				rcHolePos[2].SetRect( gapsX, 2.0f*gapsY+sizeHole, gapsX+sizeHole, sizeItem.y-gapsY );
				rcHolePos[3].SetRect( 2.0f*gapsX+sizeHole, 2.0f*gapsY+sizeHole, sizeItem.x-gapsX, sizeItem.y-gapsY );
				rcHolePos[0].OffsetRect( offsetCell );
				rcHolePos[1].OffsetRect( offsetCell );
				rcHolePos[2].OffsetRect( offsetCell );
				rcHolePos[3].OffsetRect( offsetCell );
				//Then we add diamond
				int diamondNum = 0;
				for( int i=0;i<4;i++ )
				{
					for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
					{
						(*it).pCell->AddAnimation( rHole, -1, NULL, &rcHolePos[i], false, CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE );
						if ( (m_object.socket[i] != -1) && (m_object.socket[i] != 0) )
						{
							//cur hole has diamond
							/*ObjectBase_t* pDiamond = ::GetObjectBase( m_object.socket[i] );
							if ( !pDiamond )
							{
								continue;
							}
							stResourceLocation rDiamond = GetObjectBasePicLocation( pDiamond );*/
							stRectI rcDiamond = rcHolePos[diamondNum];
							if ( GetDiamondResourceFrameFromID( m_object.socket[i] ) >= 0 )
								rDiamond.frame = GetDiamondResourceFrameFromID( m_object.socket[i] );
							else
								rDiamond.frame = 9;
							//add diamond
							(*it).pCell->AddAnimation( rDiamond, -1, NULL, &rcDiamond, false, CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND );
							diamondNum++ ;
						}
					}
				}
			}
			
			break;
		case 5:
			{
				stRectI rcHolePos[5];
				//add hole
				float gapsX = (sizeItem.x-2.0f*sizeHole) / 3.0f;
				float gapsY = (sizeItem.y-2.0f*sizeHole) / 3.0f;
				rcHolePos[0].SetRect( gapsX, gapsY, gapsX+sizeHole, gapsY+sizeHole );
				rcHolePos[1].SetRect( 2.0f*gapsX+sizeHole, gapsY, sizeItem.x-gapsX, gapsY+sizeHole );
				rcHolePos[2].SetRect( sizeItem.x/2.0f-sizeHoleHalf, sizeItem.y/2.0f-sizeHoleHalf, sizeItem.x/2.0f+sizeHoleHalf, sizeItem.y/2.0f+sizeHoleHalf );
				rcHolePos[3].SetRect( gapsX, 2.0f*gapsY+sizeHole, gapsX+sizeHole, sizeItem.y-gapsY );
				rcHolePos[4].SetRect( 2.0f*gapsX+sizeHole, 2.0f*gapsY+sizeHole, sizeItem.x-gapsX, sizeItem.y-gapsY );
				rcHolePos[0].OffsetRect( offsetCell );
				rcHolePos[1].OffsetRect( offsetCell );
				rcHolePos[2].OffsetRect( offsetCell );
				rcHolePos[3].OffsetRect( offsetCell );
				rcHolePos[4].OffsetRect( offsetCell );
				//Then we add diamond
				int diamondNum = 0;
				for( int i=0;i<5;i++ )
				{
					for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
					{
						(*it).pCell->AddAnimation( rHole, -1, NULL, &rcHolePos[i], false, CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE );
						if ( (m_object.socket[i] != -1) && (m_object.socket[i] != 0) )
						{
							//cur hole has diamond
							/*ObjectBase_t* pDiamond = ::GetObjectBase( m_object.socket[i] );
							if ( !pDiamond )
							{
								continue;
							}
							stResourceLocation rDiamond = GetObjectBasePicLocation( pDiamond );*/
							stRectI rcDiamond = rcHolePos[diamondNum];
							if ( GetDiamondResourceFrameFromID( m_object.socket[i] ) >= 0 )
								rDiamond.frame = GetDiamondResourceFrameFromID( m_object.socket[i] );
							else
								rDiamond.frame = 9;
							//add diamond
							(*it).pCell->AddAnimation( rDiamond, -1, NULL, &rcDiamond, false, CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND );
							diamondNum++ ;
						}
					}
				}
			}
			
			break;
		case 6:
			{
				stRectI rcHolePos[6];
				//add hole
				float gapsX = (sizeItem.x-2.0f*sizeHole) / 3.0f;
				float gapsY = (sizeItem.y-3.0f*sizeHole) / 4.0f;
				rcHolePos[0].SetRect( gapsX, gapsY, gapsX+sizeHole, gapsY+sizeHole );
				rcHolePos[1].SetRect( 2.0f*gapsX+sizeHole, gapsY, sizeItem.x-gapsX, gapsY+sizeHole );
				rcHolePos[2].SetRect( gapsX, 2.0f*gapsY+sizeHole, gapsX+sizeHole, 2.0f*(gapsY+sizeHole) );
				rcHolePos[3].SetRect( 2.0f*gapsX+sizeHole, 2.0f*gapsY+sizeHole, sizeItem.x-gapsX, 2.0f*(gapsY+sizeHole) );
				rcHolePos[4].SetRect( gapsX, 3.0f*gapsY+2.0f*sizeHole, gapsX+sizeHole, sizeItem.y-gapsY );
				rcHolePos[5].SetRect( 2.0f*gapsX+sizeHole, 3.0f*gapsY+2.0f*sizeHole, sizeItem.x-gapsX, sizeItem.y-gapsY );
				rcHolePos[0].OffsetRect( offsetCell );
				rcHolePos[1].OffsetRect( offsetCell );
				rcHolePos[2].OffsetRect( offsetCell );
				rcHolePos[3].OffsetRect( offsetCell );
				rcHolePos[4].OffsetRect( offsetCell );
				rcHolePos[5].OffsetRect( offsetCell );
				//Then we add diamond
				int diamondNum = 0;
				for( int i=0;i<6;i++ )
				{
					for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
					{
						(*it).pCell->AddAnimation( rHole, -1, NULL, &rcHolePos[i], false, CGuiImage::enumUSE_TYPE::USE_TYPE_HOLE );
						if ( (m_object.socket[i] != -1) && (m_object.socket[i] != 0) )			//
						{
							//cur hole has diamond
							/*ObjectBase_t* pDiamond = ::GetObjectBase( m_object.socket[i] );
							if ( !pDiamond )
							{
								continue;
							}
							stResourceLocation rDiamond = GetObjectBasePicLocation( pDiamond );*/
							stRectI rcDiamond = rcHolePos[diamondNum];
							if ( GetDiamondResourceFrameFromID( m_object.socket[i] ) >= 0 )
								rDiamond.frame = GetDiamondResourceFrameFromID( m_object.socket[i] );
							else
								rDiamond.frame = 9;
							//add diamond
							(*it).pCell->AddAnimation( rDiamond, -1, NULL, &rcDiamond, false, CGuiImage::enumUSE_TYPE::USE_TYPE_DIAMOND );
							diamondNum++ ;
						}
					}
				}
			}
			
			break;
		default:
			break;
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
void CRoleItem::SetImage()
{
	FUNCTION_BEGIN;

	stResourceLocation rl0 = GetItemPicLocation(&m_object);
	stResourceLocation rl1 = GetItemPatternPicLocation(&m_object);
	stResourceLocation rl2 = GetItemPattern2PicLocation(&m_object);
	
	for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		(*it).pCell->Clear();
		if(!rl0.IsEmpty())
		{
			(*it).pCell->AddAnimation(rl0);
		}
		

		if(!rl1.IsEmpty())
		{
			if ( GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP )
			{
				if ( (GetLocation().y == EQUIPCELLTYPE_BODY)
					|| (GetLocation().y == EQUIPCELLTYPE_HANDR)
					|| (GetLocation().y == EQUIPCELLTYPE_HANDL) )
				{
					DWORD color = -1;
					switch( GetLocation().y )
					{
					case EQUIPCELLTYPE_BODY:
						color = GetObjectBase()->dwColor;
						break;
					case EQUIPCELLTYPE_HANDR:
						color = m_object.color;
						break;
					case EQUIPCELLTYPE_HANDL:
						color = GetScene()->GetMainCharacter()->GetLeftWeaponColor();
						break;
					}
					(*it).pCell->AddAnimation(rl1,  m_object.color);		//color
				}
			}
			else
			{
				(*it).pCell->AddAnimation(rl1, m_object.color);
			}
			
		}
		if(!rl2.IsEmpty())
		{
			if ( GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP )
			{
				if ( (GetLocation().y == EQUIPCELLTYPE_BODY)
					|| (GetLocation().y == EQUIPCELLTYPE_HANDR)
					|| (GetLocation().y == EQUIPCELLTYPE_HANDL) )
				{
					DWORD color = -1;
					switch( GetLocation().y )
					{
					case EQUIPCELLTYPE_BODY:
						color = GetScene()->GetMainCharacter()->GetBodyCustomColor();
						break;
					case EQUIPCELLTYPE_HANDR:
						color = GetScene()->GetMainCharacter()->GetRightWeaponColor();
						break;
					case EQUIPCELLTYPE_HANDL:
						color = GetScene()->GetMainCharacter()->GetLeftWeaponColor();
						break;
					}
					(*it).pCell->AddAnimation(rl2,  m_object.color);		//color
				}
			}
			else
				(*it).pCell->AddAnimation(rl2, m_object.color);
		}

		if((*it).pCell->IsNullImage(0) && (*it).pCell->IsNullImage(1) && (*it).pCell->IsNullImage(2))
		{
			rl0.SetFileName(c_szItemGrpPack);
			rl0.group = c_nGroupItemCommon;
			rl0.frame = 0;
			(*it).pCell->AddAnimation(rl0);
		}
	}

	FUNCTION_END;
}

int GetStoreTableIndex(const stObjectLocation dst)
{
	int i = CGuiSavingBoxDialog::csShowRowCount;
	if(dst.x < 0	|| dst.x >= CGuiSavingBoxDialog::csShowColCount)
		return -1;
	if(dst.y >= 0	&& dst.y < i )		return 0;
	if(dst.y >= i	&& dst.y < i*2 )	return 1;
	if(dst.y >= i*2 && dst.y < i*3)		return 2;
	return -1;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dst : 描述
 * \return 返回值的描述
 */
void CRoleItem::MoveTo(const stObjectLocation & dst)
{
	FUNCTION_BEGIN;

	stObjectLocation dstLocation = dst;

	ResetLocationIfStore(dstLocation);
	if ( (dstLocation.dwLocation == OBJECTCELLTYPE_NONE) /*&& !g_bSinglePlayer*/ )		//Give up to ground
	{
		//Change cursor to normal status
		RestoreMouseIcon();		
		//Show confirm message box
		CRoleItem* pFloatItem = (CRoleItem*)(GetGuiFloatItem()->GetItem()->m_pItemData);
		if ( pFloatItem )
		{
			if( pFloatItem->IsBind() ) // 不可丢弃类物品主要有灵魂绑定的物品
			{
				GetGameGuiManager()->AddMessageBox("该物品已经被灵魂绑定,不可丢弃!");
			}
			else  if ( pFloatItem->GetItemType() == ItemType_Quest )		//任务物品
			{
				if (!GetQueryManager()->FindListData(eAction_DelItemConfirm,1))
					GetGameGuiManager()->AddMessageBox("任务物品被销毁可能会导致放弃任务，您是否要继续", eAction_DelItemConfirm, NULL,"确定|1|Enter|取消|2|Esc");
			}
			else
			{
				if (!GetQueryManager()->FindListData(eAction_DelItemConfirm,1))
					GetGameGuiManager()->AddMessageBox("是否将物品销毁", eAction_DelItemConfirm, NULL,"确定|1|Enter|取消|2|Esc");
			}
		}
		//GetGuiManager()->SetMouseDlg( NULL );
	}	
	else
	{
		if( dst.dwLocation == OBJECTCELLTYPE_PET )
		{
			if( GetGameGuiManager()->m_guiPetPackDlg &&
				!GetGameGuiManager()->m_guiPetPackDlg->bIsCellCanUse(dst.x,dst.y) )
			{
				return ;
			}
			if (IsBind())
			{
				GetGameGuiManager()->AddClientSystemMessage("已经被灵魂绑定的装备物品不能进行交易！");
				return ;
			}
			if (GetItemType()==ItemType_Arrow)
			{
				GetGameGuiManager()->AddClientSystemMessage("箭支不能进行交易！");
				return ;
			}
			if (GetItemType()==ItemType_Quest)  
			{
				GetGameGuiManager()->AddClientSystemMessage("任务物品不能进行交易！");
				return ;
			}
		}
		stSwapObjectPropertyUserCmd cmd;
		cmd.qwThisID = GetThisID();
		cmd.dst = dstLocation;
		SEND_USER_CMD(cmd);


		if(g_bSinglePlayer)
		{
			if(dst.dwLocation == OBJECTCELLTYPE_NONE)
			{
				// 丢到地上
				stAddMapObjectMapScreenUserCmd cmd1;
				cmd1.data.dwMapObjectTempID = 1;
				cmd1.data.dwObjectID = GetObjectID();
				strcpy(cmd1.data.pstrName,GetName());
				cmd1.data.wdLevel  = 1;
				cmd1.data.wdNumber = 1;
				cmd1.data.x = GetScene()->GetMainCharacter()->GetGridPos().x;
				cmd1.data.y = GetScene()->GetMainCharacter()->GetGridPos().y;
				GetClient()->PushCommand(&cmd1,sizeof(cmd1));

				stRemoveObjectPropertyUserCmd cmd2;
				cmd2.qwThisID = GetThisID();
				GetClient()->PushCommand(&cmd2,sizeof(cmd2));
			}
			else
			{
				GetClient()->PushCommand(&cmd,sizeof(cmd));
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
 * \param dst : 描述
 * \param num : 描述
 * \return 返回值的描述
 */
void CRoleItem::SplitTo( const stObjectLocation & dst,DWORD num )
{
	FUNCTION_BEGIN;

	if( dst.dwLocation == OBJECTCELLTYPE_PET )
	{
		if( GetGameGuiManager()->m_guiPetPackDlg &&
			!GetGameGuiManager()->m_guiPetPackDlg->bIsCellCanUse(dst.x,dst.y) )
		{
			return ;
		}
		if (IsBind())
		{
			GetGameGuiManager()->AddClientSystemMessage("已经被灵魂绑定的装备物品不能进行交易！");
			return ;
		}
		if (GetItemType()==ItemType_Arrow)
		{
			GetGameGuiManager()->AddClientSystemMessage("箭支不能进行交易！");
			return ;
		}
		if (GetItemType()==ItemType_Quest)  
		{
			GetGameGuiManager()->AddClientSystemMessage("任务物品不能进行交易！");
			return ;
		}
	}
	stSplitObjectPropertyUserCmd cmd;
	cmd.qwThisID = GetThisID();
	cmd.dst = dst;
	cmd.qwNewThisID = 0;
	if( num > GetCount() )
	{
		Assert(0);
		return;
	}
	cmd.dwNum = num;
	SEND_USER_CMD(cmd);	

	if(g_bSinglePlayer)
	{
		if(!IsMoney())
		{
			CRoleItem* pDstItem = GetScene()->GetMainCharacter()->FindItemByLocation(dst);
			if(pDstItem)
			{
				stRemoveObjectPropertyUserCmd cmd1;
				cmd1.qwThisID = GetThisID();
				GetClient()->PushCommand(&cmd1,sizeof(cmd1));

				stRefCountObjectPropertyUserCmd cmd2;
				cmd2.qwThisID = pDstItem->GetThisID();
				cmd2.dwNum = pDstItem->GetCount() + num;
				GetClient()->PushCommand(&cmd2,sizeof(cmd2));
			}
			else
			{
				cmd.qwNewThisID = s_dwMaxThisID+1;
				GetClient()->PushCommand(&cmd,sizeof(cmd));
			}
		}
	}

	FUNCTION_END;
}

enmCallDutyType CRoleItem::IsCallDutyItem()
{
	enmCallDutyType type = CALL_DUTY_NUM;
	switch( GetObjectID() )
	{
	case 841:
		type = CALL_DUTY_KING;
		break;
	case 821:
		type = CALL_DUTY_UNION;
		break;
	case 831:
		type = CALL_DUTY_SEPT;
		break;
	}

	return type;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CRoleItem::Sell()
{
	FUNCTION_BEGIN;
	
	PlayItemSound(g_nMoneySound);	// 播放钱的声音


	stSellObjectNpcTradeUserCmd cmd;
	cmd.qwThisID = GetThisID();
	SEND_USER_CMD(cmd);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CRoleItem::Save()
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
bool CRoleItem::NeedConsiderDur()
{
	return ( (GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP) && (m_object.maxdur>0) && !IsCanUse() && !IsArrow() && (m_pObjectBase->dwType != ItemType_Change) );		// 
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
//bool CRoleItem::IsHorse()
//{
//	FUNCTION_BEGIN;
//
//	return ( (m_pObjectBase->dwID >= 737) && (m_pObjectBase->dwID <= 737) );		
//
//	FUNCTION_END;
//}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool CRoleItem::IsArrow()
{
	FUNCTION_BEGIN;

	return ISARRAW(m_pObjectBase->dwType);

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
DWORD  CRoleItem::GetMaxCount()
{
	FUNCTION_BEGIN;

	return m_pObjectBase->dwMaxCount;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pDestItem : 描述
 * \return 返回值的描述
 */
void CRoleItem::UnionTo(CRoleItem* pDestItem)
{
	FUNCTION_BEGIN;

	Assert(pDestItem);
	stUnionObjectPropertyUserCmd cmd;
	cmd.qwSrcThisID = GetThisID();
	cmd.qwDstThisID = pDestItem->GetThisID();
	SEND_USER_CMD(cmd);

	if( g_bSinglePlayer )
	{
		DWORD dwDestNum = getMin(pDestItem->GetCount() + GetCount(),pDestItem->GetMaxCount());
		DWORD dwMoveNum = dwDestNum - pDestItem->GetCount();
		DWORD dwSrcNum = GetCount() - dwMoveNum;

		stRefCountObjectPropertyUserCmd cmd;
		cmd.qwThisID = pDestItem->GetThisID();
		cmd.dwNum = dwDestNum;
		GetClient()->PushCommand(&cmd,sizeof(cmd));

		if(dwSrcNum == 0)
		{
			stRemoveObjectPropertyUserCmd cmd;
			cmd.qwThisID = GetThisID();
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}
		else
		{
			stRefCountObjectPropertyUserCmd cmd;
			cmd.qwThisID = GetThisID();
			cmd.dwNum = dwSrcNum;
			GetClient()->PushCommand(&cmd,sizeof(cmd));
		}
	}

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param rcBBox : 描述
 * \return 返回值的描述
 */
void CRoleItem::ShowToolTipsTopLeftPrefer( stRectI rcBBox )
{
	FUNCTION_BEGIN;

	if(GetGameState()->IsStateItem(eGameStateItem_NpcTrade))
	{
		if(GetLocation().dwLocation == OBJECTCELLTYPE_COMMON || 
			GetLocation().dwLocation == OBJECTCELLTYPE_TRADE)
		{
			CGuiNpcTrade* pDlg = GetGameGuiManager()->m_guiNpcTrade;
			Assert(pDlg);
			if(pDlg)
			{
				SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
				if(pDlg->IsBuyObject(GetObjectID()))
				{
					SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
				}

				if(pDlg->IsSellObject(GetObjectID()))
				{
					SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
				}

				if(pDlg->IsRepairObject(GetObjectID()))
				{
					SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_REPAIR_PRICE);
				}
			}
		}
		else if(GetLocation().dwLocation == OBJECTCELLTYPE_SELL)
		{
			SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
		}
		
		SetTipFlags(GetTipFlags() & (~ITEMTIPS_FLAG_RIGHT_CLICK_USE));
	}
	else
	{
		SetTipFlags(GetTipFlags() & (~ITEMTIPS_FLAG_PRICE));
		SetTipFlags(GetTipFlags() & (~ITEMTIPS_FLAG_REPAIR_PRICE));
		SetTipFlags(GetTipFlags() & (~ITEMTIPS_FLAG_REPAIR_PRICEBYGOLD));
		SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_RIGHT_CLICK_USE);
	}

	if(m_bNeedUpdateTips)
	{
		UpdateToolTips();
		m_bNeedUpdateTips = false;
	}

	m_ToolTips.RenderTopLeftPrefer( rcBBox, GetDevice()->GetWidth(), GetDevice()->GetHeight() );

	/*if (NULL == GetGuiManager()->GetToolTips() )
		GetGuiManager()->SetToolTips(&m_ToolTips,pt);*/

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
void CRoleItem::ShowToolTips(POINT pt)
{
	FUNCTION_BEGIN;

	if(GetGameState()->IsStateItem(eGameStateItem_NpcTrade))
	{
		if(GetLocation().dwLocation == OBJECTCELLTYPE_COMMON || 
			GetLocation().dwLocation == OBJECTCELLTYPE_TRADE)
		{
			CGuiNpcTrade* pDlg = GetGameGuiManager()->m_guiNpcTrade;
			Assert(pDlg);
			if(pDlg)
			{
				SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
				if(pDlg->IsBuyObject(GetObjectID()))
				{
					SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
				}

				if(pDlg->IsSellObject(GetObjectID()))
				{
					SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
				}

				if(pDlg->IsRepairObject(GetObjectID()))
				{
					SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_REPAIR_PRICE);
				}
			}
		}
		else if(GetLocation().dwLocation == OBJECTCELLTYPE_SELL)
		{
			SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_PRICE);
		}
	}
	else
	{
		if ( m_object.upgrade < 10 )
		{
			SetTipFlags(GetTipFlags() & (~ITEMTIPS_FLAG_PRICE));
			SetTipFlags(GetTipFlags() & (~ITEMTIPS_FLAG_REPAIR_PRICE));
			SetTipFlags(GetTipFlags() | ITEMTIPS_FLAG_RIGHT_CLICK_USE);
		}                                                              		
	}

	if(m_bNeedUpdateTips)
	{
		UpdateToolTips();
		m_bNeedUpdateTips = false;
	}
	
	if (NULL == GetGuiManager()->GetToolTips() )
		GetGuiManager()->SetToolTips(&m_ToolTips,pt);

	//m_ToolTips.Render(pt.x,pt.y);

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
	FUNCTION_END;
}
#define Tips_AddStringDefFont(szText) \
	if( (szText)[0] ) {\
\
	_snprintf(szTemp,sizeof(szTemp),"%s",szText);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,c_nDefTipFont);\
}\

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
#define Tips_AddString(szText) \
	if( (szText)[0] ) {\
\
	_snprintf(szTemp,sizeof(szTemp),"%s",szText);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,0);\
}\

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
#define Tips_AddText(szFormat,szText) \
	if( (szText)[0] ) {\
\
	_snprintf(szTemp,sizeof(szTemp),szFormat,szText);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,0);\
}\

#define Tips_AddNum(szFormat,nNum)\
	if(nNum){\
	_snprintf(szTemp,sizeof(szTemp),szFormat,nNum);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,0);\
	}\

#define Tips_AddAllNum(szFormat,nNum)\
	{\
	_snprintf(szTemp,sizeof(szTemp),szFormat,nNum);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,0);\
	}\

#define Tips_AddNum2(szFormat,nNum1,nNum2)\
	if(nNum1 || nNum2){\
	_snprintf(szTemp,sizeof(szTemp),szFormat,nNum1,nNum2);\
	szTemp[sizeof(szTemp)-1]=0;\
	m_ToolTips.AddText(szTemp,0);\
	}\

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pSkill : 描述
 * \param m_ToolTips : 描述
 * \param pRoleSkill : 描述
 * \return 返回值的描述
 */
void UpdateSkillToolTips(SkillBase_t* pSkill,CToolTips & m_ToolTips,CRoleSkill* pRoleSkill)
{
	FUNCTION_BEGIN;

	if ( pSkill == NULL )
		return;
	
	//make sure we got role skill
	if ( pRoleSkill == NULL )
		pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pSkill->dwID );

	DWORD colorRed = COLOR_ARGB(255,255,90,0);
	DWORD colorWhite = COLOR_ARGB(255,255,255,255);
	DWORD colorBlue = COLOR_ARGB(255,0,170,255);
	DWORD colorGreen = COLOR_ARGB(255,0,255,0);
	DWORD colorYellow = COLOR_ARGB(255,202,166,10);

	m_ToolTips.Clear();
	
	m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
	m_ToolTips.SetAlignType( GUI_ALIGN_LEFT );
	m_ToolTips.SetLineGaps( 3 );

	
	char szTemp[512];
	
	m_ToolTips.SetCurColor(colorWhite);
	#ifdef _DEBUG
		Tips_AddStringDefFont(pSkill->pstrName);
		Tips_AddNum("[%u]",pSkill->dwID);
	#else
		Tips_AddStringDefFont(pSkill->pstrName);
	#endif

	//Add use method
	/*if ( (pSkill->dwTarget == SkillTarget_Self) && (pSkill->dwType != SkillType_Passive) )
		Tips_AddString( "\n瞬间施放" );
	if	( (pSkill->dwTarget & SkillTarget_Single)
			|| (pSkill->dwTarget & SkillTarget_Place)
			|| (pSkill->dwTarget & SkillTarget_Npc)
			|| (pSkill->dwTarget & SkillTarget_Enemy) )
	{
		if ( pSkill->dwType == SkillType_Attack )
		{
			Tips_AddString( "\n右键施放" );
		}
		else if ( pSkill->dwType == SkillType_Active )
		{
			Tips_AddString( "\n右键连续施放" );
		}
	}
		
	if ( !(pSkill->dwTarget & SkillTarget_Place) && (pSkill->dwType == SkillType_StateAttack) )
	{
		Tips_AddString( "\n选中后左键施放" );	
	}*/
	if ( pSkill->dwType != SkillType_Passive )
	{
		//Add use mothod
		if ( 1 )
		{
			if ( (pSkill->dwTarget == SkillTarget_Self) && (pSkill->dwType != SkillType_Passive) )
				Tips_AddString( "\n瞬间施放" );
			if	( (pSkill->dwTarget == 42)
				|| (pSkill->dwTarget == 44) )
			{
				if ( pSkill->dwType == SkillType_Attack )
				{
					Tips_AddString( "\n右键施放" );
				}
				else if ( pSkill->dwType == SkillType_Active )
				{
					Tips_AddString( "\n右键连续施放" );
				}
			}

			if ( ( (pSkill->dwTarget == 19) || (pSkill->dwTarget == 42) || (pSkill->dwTarget == 83) )
				&& (pSkill->dwType == SkillType_StateAttack) )
			{
				Tips_AddString( "\n选中后左键施放" );	
			}
		}
		else
		{
			Tips_AddString( "\n施放方式：" );
			int nMethod = 0;
			if ( pSkill->dwUseMethod & SkillUseMethod_Self )
				Tips_AddNum( "\n    %u 自身施放", nMethod++ );

			if ( pSkill->dwUseMethod & SkillUseMethod_BlueHand )
				Tips_AddNum( "\n    %u 使用技能快捷键或者左键点击技能图标,待鼠标变色后点击施放到目标上", nMethod++ );

			if ( pSkill->dwUseMethod & SkillUseMethod_SelectFirst )
				Tips_AddNum( "\n    %u 先鼠标左键或者shift+左键选中目标,然后使用技能热键或者点击技能图标施放到目标上", nMethod++ );

			if ( pSkill->dwUseMethod & SkillUseMethod_Lock )
				Tips_AddNum( "\n    %u 点击技能图标或者使用技能热键选中此技能,然后右键点击目标施放", nMethod++ );
		}
		
		//Add attack range
		if ( pSkill->dwID == SKILL_THREATEN )
		{
			Tips_AddString( "\n攻击距离：周围八格" );
		}
		else
		{
			switch( pSkill->dwDis )
			{
			case 0:
				Tips_AddString( "\n攻击距离:自身施放" );
				break;
			case 1:
				Tips_AddString( "\n攻击距离:面对方向一格" );
				break;
			case 3:
				Tips_AddString( "\n攻击距离:面对方向三格" );
				break;
			case 11:
				Tips_AddString( "\n攻击距离:本屏内" );
				break;
			}
		}
	}
	

	/*if ( pSkill->dwType == SkillType_Attack )
		Tips_AddString( "\n法术攻击" );
	if ( pSkill->dwType == SkillType_Active )
		Tips_AddString( "\n物理攻击" );*/

	if ( pSkill->dwType != SkillType_Passive )
	{
		if ( pSkill->dwAllowRide )
		{
			Tips_AddString( "\n允许骑马使用" );
		}
		else
		{
			Tips_AddString( "\n不可骑马使用" );
		}
	}

	//end use method

	CMainCharacter* pMainData = GetScene()->GetMainCharacter();
	int nSkillLevel = (pRoleSkill == NULL)? 0 : pRoleSkill->GetTotalLevel();
	
	if (nSkillLevel != 0)
	{
		if (!pMainData)
			goto end;

		SkillLevelBase_t* pSkillLevelUp = GetSkillLevelBase( pSkill->dwID, nSkillLevel );
		if ( pSkillLevelUp == NULL )
			goto end;

		Tips_AddString( "\n" );
		
		//need level
		if ( pSkillLevelUp->dwNeedSkill3Level > GetScene()->GetMainCharacter()->GetLevel() )
			m_ToolTips.SetCurColor( colorRed );
		else
			m_ToolTips.SetCurColor( colorGreen );
		Tips_AddNum("\n需要等级 %u", pSkillLevelUp->dwNeedSkill3Level );

		//cool time
		m_ToolTips.SetCurColor(colorWhite);
		if (pSkill->dwIntervalTime>0)
			Tips_AddNum("\n冷却时间:%.2f秒",(float)pSkillLevelUp->dwIntervalTime/1000);

		//Tips_AddString("\n");
		Tips_AddNum("\n生命值消耗:%u点",pSkillLevelUp->dwHP);

		Tips_AddNum("\n法术值消耗:%u点",pSkillLevelUp->dwMP);

		//Tips_AddNum("\n体力值消耗:%u点",pSkillLevelUp->dwSP);

		Tips_AddNum("\n单体伤害损耗 %u%%", pSkillLevelUp->dwHurtAdd);

		m_ToolTips.SetCurColor(colorYellow);
		Tips_AddText("\n%s", pSkillLevelUp->strDesc);

		//Add cur disable statu
		m_ToolTips.SetCurColor(colorWhite);
		{
			int nTime = 0;
			nTime = pSkillLevelUp->dwIntervalTime - ( xtimeGetTime() - GetScene()->GetMainCharacter()->GetSkillStartTime( pSkill ) );
			nTime /= 1000;
			if ( nTime > 0 )
				Tips_AddNum( "\n剩余时间：%u秒", nTime);
		}
		//Tips_AddNum( "\n剩余时间：%u秒", (int)( (1.0f -GetScene()->GetMainCharacter()->GetSkillDisableStatus( pSkill ) ) * pSkillLevelUp->dwIntervalTime/1000 ) );
	}

	//Add item cost
	{
		int curLevel = nSkillLevel;
		if ( curLevel < 1 )
			curLevel = 1;

		SkillLevelBase_t* pSkillCurLevelBase = GetSkillLevelBase( pSkill->dwID, curLevel );
		if ( pSkillCurLevelBase )
		{
			ObjectBase_t* pObject = NULL;
			pObject = GetObjectBase(pSkillCurLevelBase->dwCostID);
			if (pObject)
			{
				CRoleItem* pRoleItem = pMainData->FindItemByConstID( pObject->dwID );
				if( pRoleItem )
				{
					if ( pRoleItem->GetExteriorCount() < pSkillCurLevelBase->dwCostNum ) 
						m_ToolTips.SetCurColor( colorRed );
					else
						m_ToolTips.SetCurColor( colorGreen );
				}
				else
					m_ToolTips.SetCurColor( colorRed );

				Tips_AddText("\n需要%s ", pObject->strName );
				Tips_AddNum( "%u", pSkillCurLevelBase->dwCostNum );
			}
		}
	}

	// up level

	bool bShowNextLevel = true;
	if ( pRoleSkill )
	{
		if ( pRoleSkill->GetLevel() >= MAX_SKILL_LEVEL )
			bShowNextLevel = false;
	}
	if ( bShowNextLevel )		//nSkillLevel < MAX_SKILL_LEVEL
	{
		SkillLevelBase* pSkillLevelBase = GetSkillLevelBase(pSkill->dwID,++nSkillLevel);

		if ((pMainData==NULL)||(pSkillLevelBase==NULL))
			goto end;
		
		m_ToolTips.AddText( "\n" );
		m_ToolTips.AddText( "\n下一等级" );
		
		//need level
		if ( pSkillLevelBase->dwNeedSkill3Level > GetScene()->GetMainCharacter()->GetLevel() )
			m_ToolTips.SetCurColor( colorRed );
		else
			m_ToolTips.SetCurColor( colorGreen );
		Tips_AddNum("\n需要等级 %u", pSkillLevelBase->dwNeedSkill3Level );
        
		m_ToolTips.SetCurColor(colorWhite);
		if (pSkillLevelBase->dwIntervalTime>0)
			Tips_AddNum("\n冷却时间:%.2f秒",(float)pSkillLevelBase->dwIntervalTime/1000);

		Tips_AddNum("\n生命值消耗:%u点  ",pSkillLevelBase->dwHP);

		Tips_AddNum("\n法术值消耗:%u点  ",pSkillLevelBase->dwMP);

		//Tips_AddNum("\n体力值消耗:%u点",pSkillLevelBase->dwSP);

		Tips_AddNum("\n单体伤害损耗 %u%%", pSkillLevelBase->dwHurtAdd);

		m_ToolTips.SetCurColor(colorYellow);
		Tips_AddText("\n%s", pSkillLevelBase->strDesc);

		if (pSkillLevelBase->dwUpItemID!=0)
		{
			if (pMainData->GetItemCount(pSkillLevelBase->dwUpItemID)>0)
				m_ToolTips.SetCurColor(colorGreen);
			else
				m_ToolTips.SetCurColor(colorRed);
			
			ObjectBase_t* pObject = NULL;
			pObject = GetObjectBase(pSkillLevelBase->dwUpItemID);
			if (pObject)
				Tips_AddText("\n需要%s", pObject->strName );
		}		

		// Get pre skill 
		if ( pSkillLevelBase->dwNeedSkill1 )
		{
			SkillBase_t* pNeedSkill = NULL;
			pNeedSkill = GetSkillBase( pSkillLevelBase->dwNeedSkill1 );
			if ( pNeedSkill )
			{
				CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pNeedSkill->dwID );
				if ( pRoleSkill && (pSkillLevelBase->dwNeedSkill1Level <= pRoleSkill->GetLevel()) )
					m_ToolTips.SetCurColor(colorGreen);
				else
					m_ToolTips.SetCurColor(colorRed);

                if ( pSkillLevelBase->dwNeedSkill1Level > 0 )
				{
					Tips_AddText( "\n需要%s:", pNeedSkill->pstrName );
					Tips_AddNum( " %u点", pSkillLevelBase->dwNeedSkill1Level );
                }				
			}
		}
		if ( pSkillLevelBase->dwNeedSkill2 )
		{
			SkillBase_t* pNeedSkill = NULL;
			pNeedSkill = GetSkillBase( pSkillLevelBase->dwNeedSkill2 );
			if ( pNeedSkill )
			{
				CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pNeedSkill->dwID );
				if ( pRoleSkill && (pSkillLevelBase->dwNeedSkill2Level <= pRoleSkill->GetLevel()) )
					m_ToolTips.SetCurColor(colorGreen);
				else
					m_ToolTips.SetCurColor(colorRed);
				
				if ( pSkillLevelBase->dwNeedSkill2Level > 0 )
				{
					Tips_AddText( "\n需要%s:", pNeedSkill->pstrName );
					Tips_AddNum( " %u点", pSkillLevelBase->dwNeedSkill2Level );
				}				
			}
		}
		/*if ( pSkillLevelBase->dwNeedSkill3 )
		{
			SkillBase_t* pNeedSkill = NULL;
			pNeedSkill = GetSkillBase( pSkillLevelBase->dwNeedSkill3 );
			if ( pNeedSkill )
			{
				CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pNeedSkill->dwID );
				if ( pRoleSkill && (pSkillLevelBase->dwNeedSkill3Level <= pRoleSkill->GetLevel()) )
					m_ToolTips.SetCurColor(colorGreen);
				else
					m_ToolTips.SetCurColor(colorRed);
				
				if ( pSkillLevelBase->dwNeedSkill3Level > 0 )
				{
					Tips_AddText( "\n需要%s:", pNeedSkill->pstrName );
					Tips_AddNum( " %u点\n", pSkillLevelBase->dwNeedSkill3Level );
				}				
			}
		}*/

		//need tree points
		int treeSkillPoints = 0xffff;
		char szDesc[64];
		if ( GetGameGuiManager()->m_guiSkill )
		{
			treeSkillPoints = GetGameGuiManager()->m_guiSkill->GetSerialSkillPoints( pSkillLevelBase->dwSerialType, pSkillLevelBase->dwTreeType );
			GetGameGuiManager()->m_guiSkill->GetNeedPointsDesc( pSkillLevelBase->dwSerialType, pSkillLevelBase->dwTreeType, szDesc );
		}
		
		if ( pSkillLevelBase->dwSkillPoint > treeSkillPoints )
			m_ToolTips.SetCurColor( colorRed );
		else
			m_ToolTips.SetCurColor( colorGreen );
		
		if ( pSkillLevelBase->dwSkillPoint > 0 )
		{
			Tips_AddText( "\n需要%s点数: ", szDesc );
			Tips_AddNum( "%u 点", pSkillLevelBase->dwSkillPoint );
		}		
	}


	{
		//Add need weapon
		std::vector<UINT, std::stack_allocator<UINT> > aid;
		aid.reserve(16);
		_parse_str_num(aid,pSkill->pstrConsume);
		static const char* weaponType[] = { "刀类", "剑类", "斧类", "锤类", "杖类", "弓类", "扇类", "棍类" };		//104 ~ 111
		
		if ( aid.size() > 0 )
		{
			//First we make sure wether we have corresponding  weapon
			{
				int count = 0;
				for( size_t i=0;i<aid.size();i++ )
				{
					if ( (aid[i] >= 104)
						&& (aid[i] <= 111) )
					{
						count += pMainData->GetItemExteriorCountByType( aid[i] );
					}
				}

				if ( count > 0 )
					m_ToolTips.SetCurColor( colorGreen );
				else
					m_ToolTips.SetCurColor( colorRed );
			}

			Tips_AddString( "\n需要如下武器类型之一：" );
			for(size_t i=0;i<aid.size();i++)
			{
				if ( (aid[i] >= 104)
					&& (aid[i] <= 111) )
				{
					Tips_AddString( weaponType[aid[i]-104] );
					if ( i < (aid.size() -1) )
						Tips_AddString( "、" );
				}
			}
		}
	}
		
end:
	m_ToolTips.Resize();

	FUNCTION_END;
}

template <class tRecord>
bool DataGreatByValue(tRecord& data1,tRecord & data2)
{
	FUNCTION_BEGIN;

	if ( data1.dwPoints >= data2.dwPoints )
	{
		if ( data1.dwPoints == data2.dwPoints  )
			return data1.dwID > data2.dwID;

		return true;
	}
	
	return false;

	FUNCTION_END;
}

void UpdateSkillUpItemToolTips( t_Object& pObject, CToolTips& m_ToolTips, DWORD m_dwTipFlags )
{
	FUNCTION_BEGIN;

	DWORD colorRed = COLOR_ARGB(255,255,90,0);
	DWORD colorWhite = COLOR_ARGB(255,255,255,255);
	DWORD colorBlue = COLOR_ARGB(255,0,170,255);
	DWORD colorGreen = COLOR_ARGB(255,0,255,0);
	DWORD colorYellow = COLOR_ARGB(255,202,166,10);

	m_ToolTips.Clear();
	m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
	m_ToolTips.SetAlignType( GUI_ALIGN_LEFT );
	m_ToolTips.SetLineGaps( 3 );
	m_ToolTips.SetCurColor( colorWhite );

	char szTemp[512];

	Tips_AddStringDefFont(pObject.strName);

	//价格
	if(m_dwTipFlags & ITEMTIPS_FLAG_PRICE)
	{
		Tips_AddString( "\n" );
		if ( pObject.pos.dwLocation == OBJECTCELLTYPE_SELL )
		{
			//Buy price
			//DWORD buyPrice = GetPrice( ITEM_PRICE_BUY );
			float buyPrice = pObject.price;
			//Make some adjust according to character goodness
			DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
			switch(goodness)
			{
			case GOODNESS_3:
				buyPrice *= 1.2f;
				break;
			case GOODNESS_4:
				buyPrice *= 1.5f;
				break;
			case GOODNESS_5:
			case GOODNESS_6:
				buyPrice *= 2.0f;
				break;
			}
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n购买价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( (DWORD)(buyPrice*pObject.dwNum + 0.99f) ) );
		}
		else if ( pObject.pos.dwLocation == OBJECTCELLTYPE_MYSHOP )
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n销售价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( GetItemTotalPrice(pObject) ) );
		}
		else if ( pObject.pos.dwLocation == OBJECTCELLTYPE_OTHERSSHOP)
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n购买价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( GetItemTotalPrice(pObject) ) );
		}
		else
		{
			//Sell price
			//DWORD sellPrice = m_object.GetPrice( ITEM_PRICE_SELL );
			float sellPrice = GetObjectSellMoneyByDur(&pObject);
			////Consider dur
			//if ( m_object.maxdur > 0 )
			//{
			//	if ( (m_pObjectBase->dwType == ItemType_Arrow) )
			//		sellPrice *= m_object.dur/m_object.maxdur;
			//	else
			//		sellPrice *= ((m_object.dur/25)*25) / m_object.maxdur;
			//}
			//
			////Make some adjust according to character goodness
			//sellPrice *= 0.50f;		
			DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
			switch(goodness)
			{
			case GOODNESS_3:
				sellPrice *= 0.7f;
				break;
			case GOODNESS_4:
				sellPrice *= 0.4f;
				break;
			case GOODNESS_5:
			case GOODNESS_6:
				sellPrice *= 0.1f;
				break;
			}
			//Tips_AddNum("\n价格 %u", (DWORD)(sellPrice + 0.99f));		//GetPrice()
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n出售价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( (DWORD)(sellPrice*pObject.dwNum + 0.99f) ) );
		}

	}

	//Get object base
	ObjectBase_t* pObjectBase = GetObjectBase( pObject.dwObjectID );

	if ( NULL == pObjectBase )
	{
		m_ToolTips.Resize();
		return;
	}

	//See if we already study this skill
	DWORD skillID = GetSkillIdFromBook( pObject.dwObjectID );
	SkillBase_t* pSkill = GetSkillBase( skillID );

	if ( NULL == pSkill )
	{
		m_ToolTips.Resize();
		return;
	}

	CRoleSkill* pRoleSkill = NULL;
	if ( skillID != -1 )
	{
		pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( skillID );
		if ( pRoleSkill )
		{
			bool bStudied = true;
			if ( pSkill->dwSerialType == SKILL_SERIAL_SPECIAL )
			{
				DWORD level = GetSkillLevelFromBook( pObject.dwObjectID );
				if ( level == -1 )
					bStudied = false;

				if ( level > pRoleSkill->GetLevel() )
					bStudied = false;

				if ( bStudied )
				{
					m_ToolTips.SetCurColor( colorRed );
					Tips_AddString( "\n已学会" );
				}
			}			
		}
		else
		{
			//Tips_AddString( "\n" );			
		}
	}
	else
	{
		m_ToolTips.Resize();
		return;
	}

	//Add skill serial and tree name
	m_ToolTips.SetCurColor( colorWhite );
	if ( (NULL != GetSkillSerialName(pSkill)) && (NULL != GetSkillTreeName(pSkill)) )
	{
		char szName[256];
		sprintf( szName, GetSkillSerialName(pSkill) );
		Tips_AddText( "\n%s", szName );
		szName[0] = 0L;
		if ( pSkill->dwType == SkillType_Passive )
			sprintf( szName, "%s被动技能", GetSkillTreeName(pSkill) );
		else
			sprintf( szName, "%s技能", GetSkillTreeName(pSkill) );
		Tips_AddString( szName );
		//Tips_AddString( "\n" );
	}

	//Show item description
	if ( ( strcmp( pObjectBase->strDesc, "无" ) != 0 )
		&& ( strcmp( pObjectBase->strDesc, "" ) != 0 ) )
	{
		m_ToolTips.SetCurColor( colorWhite );
		Tips_AddText( "\n%s", pObjectBase->strDesc );
	}

	//
	//Tips_AddString( "\n" );

	//Need skill
	//see if all pre skill has studied
	SkillLevelBase_t* pSkillLevel = NULL;
	if ( pRoleSkill )
		pSkillLevel = GetSkillLevelBase( skillID, pRoleSkill->GetLevel() + 1 );
	else
		pSkillLevel = GetSkillLevelBase( skillID, 1 );

	if ( NULL == pSkillLevel )
	{
		m_ToolTips.Resize();
		return;
	}

	if ( (pSkill->dwNeedSkill1 && pSkillLevel->dwNeedSkill1Level)
		|| (pSkill->dwNeedSkill2 && pSkillLevel->dwNeedSkill2Level)
		|| (pSkill->dwNeedSkill3 && pSkillLevel->dwNeedSkill3Level) )
	{
		Tips_AddString( "\n学习此技能需要:" );
	}

	if ( pSkill->dwNeedSkill1 && pSkillLevel->dwNeedSkill1Level )
	{
		CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pSkill->dwNeedSkill1 );
		if( pRoleSkill )
		{
			if ( pRoleSkill->GetLevel() < pSkillLevel->dwNeedSkill1Level )
				m_ToolTips.SetCurColor( colorRed );
			else
				m_ToolTips.SetCurColor( colorGreen );

			Tips_AddText( "\n%s ", pRoleSkill->GetName() );
			Tips_AddNum( "%u级", pSkillLevel->dwNeedSkill1Level );
		}
		else
		{
			SkillBase_t* pNeedSkill1 = GetSkillBase( pSkill->dwNeedSkill1 );
			if ( pNeedSkill1 )
			{
				m_ToolTips.SetCurColor( colorRed );
				Tips_AddText( "\n%s ", pNeedSkill1->pstrName );
				Tips_AddNum( "%u级", pSkillLevel->dwNeedSkill1Level );
			}
		}
	}
	if ( pSkill->dwNeedSkill2 && pSkillLevel->dwNeedSkill2Level )
	{
		CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pSkill->dwNeedSkill2 );
		if( pRoleSkill )
		{
			if ( pRoleSkill->GetLevel() < pSkillLevel->dwNeedSkill2Level )
				m_ToolTips.SetCurColor( colorRed );
			else
				m_ToolTips.SetCurColor( colorGreen );

			Tips_AddText( "\n%s ", pRoleSkill->GetName() );
			Tips_AddNum( "%u级", pSkillLevel->dwNeedSkill2Level );
		}
		else
		{
			SkillBase_t* pNeedSkill2 = GetSkillBase( pSkill->dwNeedSkill2 );
			if ( pNeedSkill2 )
			{
				m_ToolTips.SetCurColor( colorRed );
				Tips_AddText( "\n%s ", pNeedSkill2->pstrName );
				Tips_AddNum( "%u级", pSkillLevel->dwNeedSkill2Level );
			}
		}
	}
	/*if ( pSkill->dwNeedSkill3 && pSkillLevel->dwNeedSkill3Level )
	{
		CRoleSkill* pRoleSkill = GetScene()->GetMainCharacter()->FindSkillByID( pSkill->dwNeedSkill3 );
		if( pRoleSkill )
		{
			if ( pRoleSkill->GetLevel() < pSkillLevel->dwNeedSkill3Level )
				m_ToolTips.SetCurColor( colorRed );
			else
				m_ToolTips.SetCurColor( colorGreen );

			Tips_AddText( "\n%s ", pRoleSkill->GetName() );
			Tips_AddNum( "%u级", pSkillLevel->dwNeedSkill3Level );
		}
		else
		{
			SkillBase_t* pNeedSkill3 = GetSkillBase( pSkill->dwNeedSkill3 );
			if ( pNeedSkill3 )
			{
				m_ToolTips.SetCurColor( colorRed );
				Tips_AddText( "\n%s ", pNeedSkill3->pstrName );
				Tips_AddNum( "%u级", pSkillLevel->dwNeedSkill3Level );
			}
		}
	}*/

	//Need tree points
	if ( pSkillLevel->dwSkillPoint > 0 )
	{
		//see if the tree level is ok
		//Get skill at tree points
		if ( NULL == GetGameGuiManager()->m_guiSkill )
		{
			GetGameGuiManager()->AddSkill();
			if ( GetGameGuiManager()->m_guiSkill )
				GetGameGuiManager()->m_guiSkill->SetVisible( false );
		}

		//need tree points
		int treeSkillPoints = 0xffff;
		char szDesc[64];
		if ( GetGameGuiManager()->m_guiSkill )
		{
			treeSkillPoints = GetGameGuiManager()->m_guiSkill->GetSerialSkillPoints( pSkillLevel->dwSerialType, pSkillLevel->dwTreeType );
			GetGameGuiManager()->m_guiSkill->GetNeedPointsDesc( pSkillLevel->dwSerialType, pSkillLevel->dwTreeType, szDesc );
		}

		if ( treeSkillPoints < pSkillLevel->dwSkillPoint )
			m_ToolTips.SetCurColor( colorRed );
		else
			m_ToolTips.SetCurColor( colorGreen );
		if ( pSkillLevel->dwSkillPoint > 0 )
		{
			Tips_AddText( "\n需要%s点数: ", szDesc );
			Tips_AddNum( "%u 点", pSkillLevel->dwSkillPoint );
		}		
	}

	//Need weapon
	std::vector<UINT, std::stack_allocator<UINT> > aid;
	aid.reserve(16);
	_parse_str_num(aid,pSkill->pstrConsume);
	static const char* weaponType[] = { "刀类", "剑类", "斧类", "锤类", "杖类", "弓类", "扇类", "棍类" };		//104 ~ 111

	if ( aid.size() > 0 )
	{
		//First we make sure wether we have corresponding  weapon
		{
			int count = 0;
			for( size_t i=0;i<aid.size();i++ )
			{
				if ( (aid[i] >= 104)
					&& (aid[i] <= 111) )
				{
					count += GetScene()->GetMainCharacter()->GetItemExteriorCountByType( aid[i] );
				}
			}

			if ( count > 0 )
				m_ToolTips.SetCurColor( colorGreen );
			else
				m_ToolTips.SetCurColor( colorRed );
		}

		Tips_AddString( "\n需要如下武器类型之一：" );
		for(size_t i=0;i<aid.size();i++)
		{
			if ( (aid[i] >= 104)
				&& (aid[i] <= 111) )
			{
				Tips_AddString( weaponType[aid[i]-104] );
				if ( i < (aid.size() -1) )
					Tips_AddString( "、" );
			}
		}
	}

	//All tree skill points
	//Tips_AddString( "\n" );
	//Get skill tree points list
	if ( NULL == GetGameGuiManager()->m_guiSkill )
	{
		GetGameGuiManager()->AddSkill();
		if ( GetGameGuiManager()->m_guiSkill )
			GetGameGuiManager()->m_guiSkill->SetVisible( false );
	}

	if ( GetGameGuiManager()->m_guiSkill )
	{
		CGuiSkill::t_SkillTree& skillTree = GetGameGuiManager()->m_guiSkill->GetSkillTreeList();
		std::sort( skillTree.begin(), skillTree.end(), DataGreatByValue<CGuiSkill::stSkillTree> );

		for( CGuiSkill::t_SkillTree::iterator it=skillTree.begin(); it != skillTree.end(); ++it )
		{
			if ( (*it).dwPoints )
			{
				Tips_AddText( "\n已经学习%s( ", (*it).strName );
				Tips_AddNum( "%u点)", (*it).dwPoints );
				//TRACE( "\nSkill tree ID=%u, NAME=%s, POINTS=%u", (*it).dwID, (*it).strName, (*it).dwPoints );
			}
		}
	}

	m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
	Tips_AddString( "\n右键点击可以使用" );

	m_ToolTips.Resize();
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pObject : 描述
 * \param m_ToolTips : 描述
 * \param flags : 描述
 * \return 返回值的描述
 */
void UpdateObjectToolTips( ObjectBase_t* pObject , CToolTips& m_ToolTips ,DWORD flags, DWORD num)
{
	FUNCTION_BEGIN;

	m_ToolTips.Clear();
	m_ToolTips.SetAlignType( GUI_ALIGN_LEFT );
	m_ToolTips.SetLineGaps( 3 );
	m_ToolTips.SetBorderColor(0);
	m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
	m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));

	bool canUse =  enumItemType(pObject->dwType) == ItemType_Leechdom 
				|| enumItemType(pObject->dwType) == ItemType_Food 
				|| enumItemType(pObject->dwType) == ItemType_Tools
				|| enumItemType(pObject->dwType) == ItemType_Pack
				|| enumItemType(pObject->dwType) == ItemType_Move
				|| enumItemType(pObject->dwType) == ItemType_Scroll
				|| enumItemType(pObject->dwType) == ItemType_Tonic
				|| enumItemType(pObject->dwType) == ItemType_Gift
				|| enumItemType(pObject->dwType) == ItemType_MASK
				|| enumItemType(pObject->dwType) == ItemType_FillHP
				|| enumItemType(pObject->dwType) == ItemType_FillMP ;

	bool canUseLevel = pObject->needLevel <= GetScene()->GetMainCharacter()->GetLevel();

	bool bVessalType = enumItemType(pObject->dwType) == ItemType_Pack
					|| enumItemType(pObject->dwType) == ItemType_Tonic
					|| enumItemType(pObject->dwType) == ItemType_Change
					|| enumItemType(pObject->dwType) == ItemType_PackHide ;

	//Special type
	enum enumSpecialType
	{
		ST_NONE,
		ST_LeechdomFood,
	};
	enumSpecialType spType = ST_NONE;
	//Get some special type
	if ( ( enumItemType(pObject->dwType) == ItemType_Leechdom )
		|| ( enumItemType(pObject->dwType) == ItemType_Food ) )
	{
		if ( pObject->maxhp > 0 )
			spType = ST_LeechdomFood;
	}

	char szTemp[512];
	//Name
	{
		if ( (pObject->dwID == 678)
			|| (pObject->dwID == 679)

			|| (pObject->dwID == 681)
			|| (pObject->dwID == 792) 
			|| (pObject->dwID == 794)
			|| (pObject->dwID == 795)

			|| (pObject->dwID == 791)
			|| (pObject->dwID == 790)

			|| (pObject->dwID == 680)
			|| (pObject->dwID == 748)
			|| (pObject->dwID == 749)
			|| (pObject->dwID == 750)
			|| (pObject->dwID == 751)	)		//Diamond
		{
			if ( (pObject->dwID == 678)
				|| (pObject->dwID == 679) )	
			{
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
			}
			else
			{
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,137) );
			}
		}
	}
	Tips_AddStringDefFont(pObject->strName);

	Tips_AddString( "\n  " );

	//价格
	m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
	if(flags & ITEMTIPS_FLAG_PRICE)
	{
		//if ( !bVessalType )
		{
			if (pObject->dwTradeType & eBuySilver)
			{
				DWORD totalPrice = (DWORD)GetItemPrice(pObject->price)*num;
				
				if (pObject->dwType==ItemType_DoubleExp)
				{
					Tips_AddNum( "\n需要荣誉点 %d", totalPrice );
				}
				else
				{
					//This time we add revenue rate
					DWORD revRate = CCountry::Instance()->GetRevenue( totalPrice );
					Tips_AddText( "\n购买价格 %s", Money2String( totalPrice ) );
					if ( revRate > 0 )
					{
						m_ToolTips.SetCurColor( COLOR_ARGB(255,70,190,27) );
						Tips_AddText( " + %s", Money2String( revRate ) );
					}	
				}
			}
			else
			{
				Tips_AddNum( "\n购买价格 %s 金子(点券)", Money2String(pObject->price*num) );
			}

			/*float fNeedMoney1 = buyPrice*num + 0.99f;
			Tips_AddText( "\n购买价格 %s", Money2String( (DWORD)(fNeedMoney1) ) );*/
			
			//Tips_AddNum("\n价格 %u",(DWORD)(buyPrice + 0.99f));
		}		
	}

	
	//Add equip position
	static const char* equipPos[] = { "", "头部", "身体", "右手", "左手"
		,"颈部", "手腕", "手指", "腰部", "脚" };		//1 ~ 9
	static const char* equipName[] = { "衣服", "衣服", "衣服", "刀", "剑"
		,"斧", "锤", "法杖", "弓", "扇子"
		,"棍", "盾牌", "头盔", "腰带", "护腕"
		,"鞋子", "项链", "戒指" };		//101 ~ 118

	if ( (pObject->dwEquipType >= 1) && (pObject->dwEquipType <= 9) )
	{
		if ( (pObject->dwType >= 101) && (pObject->dwType <= 118) )
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n装备类型		" );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddString( equipName[pObject->dwType-101] );
		}

		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n装备位置		" );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
		Tips_AddString( equipPos[pObject->dwEquipType] );
	}
	//end equip pos
	//Add special equip position
	if ( pObject->dwType == 120 )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n装备类型		" );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
		/*if ( pObject->dwID == 875 )
		{
			Tips_AddString( "花" );
		}
		else if ( pObject->dwID == 876 )
		{
			Tips_AddString( "采集手套" );
		}
		else*/
			Tips_AddString( "特殊" );

		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n装备位置		" );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
		Tips_AddString( "右手" );
	}
	
	//需要等级
	if ( pObject->dwID == 680 )		//魂魄石
	{
		if( !canUseLevel )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,1,19));
		else
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));

		Tips_AddNum("\n需要等级 %u",pObject->needLevel);
	}
	else
	{
		if( !canUseLevel )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,1,19));
		else
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));

		if ( (pObject->needLevel>0) && !canUse )
			Tips_AddNum("\n需要等级 %u",pObject->needLevel);
	}

	//耐久度
	if ( (pObject->dwMaxDur>0) && !canUse && !ISARRAW(pObject->dwType) && !bVessalType
		&& !(pObject->dwType == ItemType_Honor)
		&& !(pObject->dwType == ItemType_DoubleExp)
		&& !(pObject->dwType == ItemType_Move)
		&& !(pObject->dwType == ItemType_Scroll)
		&& !(pObject->dwType == ItemType_Pack)
		&& !(pObject->dwType == ItemType_FashionBody)
		&& !(pObject->dwType == ItemType_FillHP)
		&& !(pObject->dwType == ItemType_FillMP) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n最大耐久度 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum("%u",(pObject->dwMaxDur+49)/50);
	}

	//For 荣誉之球
	if ( (pObject->dwType == ItemType_Honor) && (pObject->dwMaxDur > 0) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n荣誉点 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum( "%u",pObject->dwMaxDur );
	}

	//For Fill HP item
	if ( (pObject->dwType == ItemType_FillHP) && (pObject->dwMaxDur > 0) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n最大血量 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
		Tips_AddNum( "%u", pObject->dwMaxDur * 100 );	//We multi the value by 50 cause of max dur is WORD but the real value is a DWORD
	}

	//For Fill MP item
	if ( (pObject->dwType == ItemType_FillMP) && (pObject->dwMaxDur > 0) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n最大法力 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
		Tips_AddNum( "%u", pObject->dwMaxDur * 50 );	//We multi the value by 50 cause of max dur is WORD but the real value is a DWORD
	}

	//For 双倍经验道具
	if ( (pObject->dwType == ItemType_DoubleExp) && (pObject->dwMaxDur > 0) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n时间 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum( "%u 分", pObject->dwMaxDur/60 );		//Server send time in seconds
	}

	//For package item
	if ( (pObject->dwType == ItemType_Pack) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n最大时间 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		if ( pObject->dwType == ItemType_FashionBody )
		{
			Tips_AddText( "%s", Minute2DayHourMinute( pObject->dwMaxDur, TYPE_DAY_HOUR_MINUTE ) );		
		}
		else
			Tips_AddText( "%s", Minute2DayHourMinute( pObject->dwMaxDur, TYPE_DAY ) );		
	}

	//For fashion
	if ( pObject->dwType == ItemType_FashionBody )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n最大耐久度 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum("%u",pObject->dwMaxDur);
	}

	
	if ( pObject->dwType == ItemType_MASK )		//蒙面巾
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddNum( "\n使用时间 %u秒",  pObject->dwMaxDur );
	}
	if ( pObject->dwType == ItemType_Change )		//月光宝盒
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddNum( "\n最大合成次数 %u次",  pObject->dwMaxDur );
	}

	
	//增加角色属性（包括当前生命值、最大生命值、当前法术值、最大法术值、当前体力值、最大体力值）
	if( !( (spType == ST_LeechdomFood) && ( (pObject->maxhp == 8) || (pObject->maxhp == 9) ) ) )
	{
		if ( pObject->maxhp > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n增加生命值" );
			//m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum(" %u", pObject->maxhp);
		}
	}
	if( !( (spType == ST_LeechdomFood) && ( (pObject->maxhp == 10) || (pObject->maxhp == 11) ) ) )
	{
		if ( pObject->maxmp )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n增加魔法值" );
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum(" %u",pObject->maxmp);
		}
	}
	/*if( !( (spType == ST_LeechdomFood)
		&& ( (pObject->maxhp == 4)
			|| (pObject->maxhp == 5)
			|| (pObject->maxhp == 6)
			|| (pObject->maxhp == 7)
			|| (pObject->maxhp == 12) ) ) )
	{
		if ( pObject->maxsp )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n增加体力值" );
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum(" %u",pObject->maxsp);
		}
	}*/
	
	
	//攻击力（包括攻击力、远程伤害、仙术、法术）
	if ( (pObject->maxpdam > 0) && !bVessalType )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n物理攻击力 " );
		//m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u - %u",pObject->pdam, pObject->maxpdam);
	}

	/*if ( pObject->maxrdam > 0 )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n远程攻击力 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u - %u",pObject->rdam,pObject->maxrdam);
	}*/

	/*if ( pObject->maxtdam > 0 )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n仙术攻击力 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u - %u",pObject->tdam,pObject->maxtdam);
	}*/

	if ( (pObject->maxmdam > 0) && !bVessalType )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n魔法攻击力 " );
		//m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u - %u", pObject->mdam, pObject->maxmdam);
	}

	//防御力（包括防御力、法术防御力）
	if ( !canUse )
	{
		if ( pObject->pDef > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n物理防御力 " );
			//m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum("%u",pObject->pDef);
		}

		if ( pObject->mDef > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n魔法防御力 " );
			//m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum("%u",pObject->mDef);
		}
	}
	
	
	//速度（物理攻击速度、施法速度、移动速度）
	Tips_AddNum("\n攻击速度 %u",pObject->atspeed);
	Tips_AddNum("\n移动速度 %u",pObject->mvspeed);
	
	//命中率（包括命中率、法术命中率）
	Tips_AddNum("\n命中率 %u%%",pObject->atrating);

	//闪避率（包括躲避率、法术躲避率）
	Tips_AddNum("\n躲避率 %u%%",pObject->akdodge);	

	//Can used for skill tips
	m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
	if ( (pObject->dwType >=104) && (pObject->dwType<=107) )
		Tips_AddString( "\n适用于武术系技能" );
	if ( pObject->dwType == 108 )
		Tips_AddString( "\n适用于法术系技能" );
	if ( pObject->dwType == 109 )
		Tips_AddString( "\n适用于射术系技能" );
	if ( pObject->dwType == 110 )
		Tips_AddString( "\n适用于仙术系技能" );
	if ( pObject->dwType == 111 )
		Tips_AddString( "\n适用于召唤系技能" );
	//

	//Show item description
	if ( ( strcmp( pObject->strDesc, "无" ) != 0 )
		&& ( strcmp( pObject->strDesc, "" ) != 0 ) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddText( "\n%s", pObject->strDesc );
	}

	//Add tips
	m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
	if ( (pObject->dwType >= 101) && (pObject->dwType <= 118 ) )
	{
		Tips_AddString( "\n右键点击可以装备" );
	}
	else if( (pObject->dwType == 17)
		|| (pObject->dwType == 19 )
		|| (pObject->dwType == 20 )			
		|| (pObject->dwType == 25 )
		|| (pObject->dwType == 26 )			
		|| (pObject->dwType == 33 ) )		//|| (pObject->dwType == 41 )
	{
		Tips_AddString( "\n右键点击可以使用" );
	}
	else if( (pObject->dwType == 23 )
		|| (pObject->dwType == 30 )
		|| (pObject->dwType == 31 ) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
		Tips_AddString( "\n可装备至功能包裹栏" );
	}
	else if( pObject->dwType == 41 )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
		Tips_AddString( "\n可装备至功能包裹栏" );
		Tips_AddNum( "\n最大使用次数 %u", pObject->dwMaxDur );
	}
	/*else if (m_pObjectBase->dwType == 23 )
	{
	if (m_object.pos.dwLocation!=OBJECTCELLTYPE_EQUIP)
	{
	Tips_AddString( "\n（右键点击可以装备）" );
	}
	else
	{
	Tips_AddString( "\n（右键点击可以打开）" );
	}
	}*/
	
	//Add some special info
	//Add for Leechdom and food
	/*if (  spType == ST_LeechdomFood )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		switch( pObject->maxhp )
		{
		case 1:
			Tips_AddNum2( "\n%u 秒内增加攻击力 %u 点", pObject->maxmp, pObject->maxsp );
			break;
		case 2:
			Tips_AddNum2( "\n%u 秒内增加防御力 %u 点" , pObject->maxmp, pObject->maxsp );
			break;
		case 3:
			Tips_AddNum( "\n目标 %u 秒内处于中毒状态", pObject->maxmp );
			break;
		case 4:
			Tips_AddNum( "\n%u 秒内不消耗体力", pObject->maxmp );
			break;
		case 5:
			Tips_AddNum( "\n增加体力 %u%%", pObject->maxsp );
			break;
		case 6:
			Tips_AddNum2( "\n%u 秒内每秒减少体力消耗 %u 点", pObject->maxmp, pObject->maxsp );
			break;
		case 7:
			Tips_AddNum2( "\n%u 秒内每秒增加体力恢复 %u 点", pObject->maxmp, pObject->maxsp );
			break;
		case 8:
			Tips_AddNum( "\n恢复生命值 %u 点", pObject->maxhp );
			break;
		case 9:
			Tips_AddNum2( "\n%u 秒内每秒恢复生命值恢复生命 %u 点", pObject->maxmp, pObject->maxhp );
			break;
		case 10:
			Tips_AddNum( "\n恢复法术值 %u 点", pObject->maxmp );
			break;
		case 11:
			Tips_AddNum2( "\n%u 秒内每秒恢复法术值 %u 点", pObject->maxmp, pObject->maxmp );
			break;
		case 12:
			Tips_AddNum( "\n恢复体力 %u", pObject->maxsp );
			break;
		default:
			break;
		}	
	}*/

	//Tips_AddNum("\n金 %u",pObject->gold);
	//Tips_AddNum("\n木 %u",pObject->wood);
	//Tips_AddNum("\n水 %u",pObject->water);
	//Tips_AddNum("\n火 %u",pObject->fire);
	//Tips_AddNum("\n土 %u",pObject->soil);
	//Tips_AddNum("\n金系抵抗 %u",pObject->goldr);
	//Tips_AddNum("\n木系抵抗 %u",pObject->woodr);
	//Tips_AddNum("\n土系抵抗 %u",pObject->waterr);
	//Tips_AddNum("\n水系抵抗 %u",pObject->firer);
	//Tips_AddNum("\n火系抵抗 %u",pObject->soilr);
	//Tips_AddNum("\n双倍经验 %u",pObject->doublexp);
	//Tips_AddNum("\n防御力增强 %u",pObject->pdefup);
	//Tips_AddNum("\n法术防御力增强 %u",pObject->mdefup);
	//Tips_AddNum("\n抗毒增加 %u",pObject->poisondef);
	//Tips_AddNum("\n抗冰冻增加 %u",pObject->colddef);
	//Tips_AddNum("\n抗石化增加 %u",pObject->petrifydef);
	//Tips_AddNum("\n抗失明增加 %u",pObject->blinddef);
	//Tips_AddNum("\n抗混乱增加 %u",pObject->chaosdef);

	//Tips_AddNum("\n伤害反射 %u%%",pObject->reflectp);
	//Tips_AddNum("\n伤害反射 人物等级/%u",pObject->reflect);

	//Tips_AddNum("\n恢复装备耐久度 %u",pObject->durspeed);
	//Tips_AddNum("\n神圣一击 %u",pObject->holy);

	//Tips_AddNum("\n增加伤害值 %u%%",pObject->ehpp);
	//Tips_AddNum("\n增加伤害值 人物等级/%u",pObject->ehpv);

	//Tips_AddNum("\n增加攻击力 %u%%",pObject->epdamp);
	//Tips_AddNum("\n增加攻击力 人物等级/%u",pObject->erdamp);

	//Tips_AddNum("\n远程攻击力 %u%%",pObject->erdamp);
	//Tips_AddNum("\n远程攻击力 人物等级/%u",pObject->erdamv);

	//Tips_AddNum("\n增加仙术攻击 %u%%",pObject->etdamp);
	//Tips_AddNum("\n增加仙术攻击 人物等级/%u",pObject->etdamv);

	//Tips_AddNum("\n法术攻击 %u%%",pObject->emdamp);
	//Tips_AddNum("\n法术攻击 人物等级/%u",pObject->emdamv);

	//Tips_AddNum("\n五行属性增加 %u",pObject->fivepro);
	//Tips_AddNum("\n吸收生命值 %u",pObject->hpleech);
	//Tips_AddNum("\n吸收法术值 %u",pObject->mpleech);
	//Tips_AddNum("\n转换生命值为法术值 %u%%",pObject->hptomp);

	//Tips_AddNum("\n物理伤害值减少 人物等级/%u",pObject->dhpv);
	//Tips_AddNum("\n物理伤害减少 %u%%",pObject->dhpp);

	//Tips_AddNum("\n法术伤害值减少 人物等级/%u",pObject->dmpv);
	//Tips_AddNum("\n法术伤害值减少 %u%%",pObject->dmpp);

	//Tips_AddNum("\n%u%% 忽视目标防御",pObject->invtarget);
	//Tips_AddNum("\n增加银子掉落 %u%%",pObject->incgold);
	//Tips_AddNum("\n%u%% 使目标冰冻",pObject->cold);
	//Tips_AddNum("\n%u%% 使目标中毒",pObject->poison);
	//Tips_AddNum("\n%u%% 使目标石化",pObject->petrify);
	//Tips_AddNum("\n%u%% 使目标混乱",pObject->chaos);
	//Tips_AddNum("\n%u%% 使目标失明",pObject->blind);
	//Tips_AddNum("\n增加掉宝率 %u%%",pObject->mf);
	//Tips_AddNum("\n魅力增加 %u",pObject->charm);
	//Tips_AddNum("\n熟练度增加 %u%%",pObject->sleight);

	//Add tips
	/*m_ToolTips.SetCurColor( COLOR_ARGB(255,42,246,138) );
	if ( (pObject->dwType >= 1) && (pObject->dwType <= 15 ) )
	{
		Tips_AddString( "\n（右键点击可以装备）" );
	}
	else if( (pObject->dwType == 17)
		|| (pObject->dwType == 19 )
		|| (pObject->dwType == 20 )
		|| (pObject->dwType == 25 )
		|| (pObject->dwType == 26 )
		|| (pObject->dwType == 33 ) )
	{
		Tips_AddString( "\n（右键点击可以使用）" );
	}
	else if( (pObject->dwType == 23 )
		|| (pObject->dwType == 30 )
		|| (pObject->dwType == 31 ) )
	{
		Tips_AddString( "\n（鼠标点击可以打开）" );
	}*/

	m_ToolTips.Resize();

	FUNCTION_END;
}

inline int GetFiveBack(const int& fiveType)
{
	int ret = fiveType+1;
	if ( ret >= FIVE_NONE )
		ret -= FIVE_NONE;

	return ret;
}

inline int GetFiveBorn(const int& fiveType)
{
	if ( fiveType < 0 || fiveType >= FIVE_NONE )
		return 0;

	int ret = fiveType + 3;
	if ( ret >= FIVE_NONE )
		ret -= FIVE_NONE;

	return ret;
}

inline int GetFiveBornFrom(const int& fiveType)
{
	if ( fiveType < 0 || fiveType >= FIVE_NONE )
		return 0;

	int ret = fiveType - 3;
	if ( ret < 0 )
		ret += FIVE_NONE;

	return ret;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void CRoleItem::UpdateToolTips()
{
	FUNCTION_BEGIN;

	//make sure we have base object info
	if ( NULL == m_pObjectBase )
		m_pObjectBase = ::GetObjectBase( m_object.dwObjectID );	

	if ( m_price != 0 )
	{
		m_dwTipFlags |= ITEMTIPS_FLAG_PRICE;
	}
#ifdef _AUTOPLAY
	m_dwTipFlags |= ITEMTIPS_FLAG_PRICE | ITEMTIPS_FLAG_REPAIR_PRICE | ITEMTIPS_FLAG_REPAIR_PRICEBYGOLD;
#endif
	UpdateMyObjectToolTips( m_object, m_pObjectBase, m_ToolTips, m_dwTipFlags );
	FUNCTION_END;
}



/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param m_object : 描述
 * \param m_pObjectBase : 描述
 * \param m_ToolTips : 描述
 * \param m_dwTipFlags : 描述
 * \return 返回值的描述
 */
void UpdateMyObjectToolTips( t_Object& m_object, ObjectBase_t* m_pObjectBase, CToolTips& m_ToolTips, DWORD m_dwTipFlags )
{
	FUNCTION_BEGIN;

	if(m_object.qwThisID == INVALID_THISID )
	{
		UpdateObjectToolTips(m_pObjectBase,m_ToolTips,m_dwTipFlags, m_object.dwNum);
		return;
	}

	if ( (enumItemType(m_pObjectBase->dwType) == ItemType_SkillUp)
		|| (m_pObjectBase->dwType == 51) )
	{
		UpdateSkillUpItemToolTips( m_object, m_ToolTips, m_dwTipFlags );
		return;
	}

	DWORD dwCanEquip = 0;
	{
		if(0 == (m_pObjectBase->dwCharType & GetScene()->GetMainCharacter()->GetType()))
			dwCanEquip = CANNOTUSE_CHARTYPE;
		if(m_object.needlevel > GetScene()->GetMainCharacter()->GetLevel())
			dwCanEquip = CANNOTUSE_LEVEL;
	}

	enumItemType itemType = enumItemType( m_pObjectBase->dwType );
	bool bCanUse = itemType == ItemType_Leechdom 
		|| itemType == ItemType_Food 
		|| itemType == ItemType_Tools
		|| itemType == ItemType_Pack
		|| itemType == ItemType_Move
		|| itemType == ItemType_Scroll
		|| itemType == ItemType_Tonic
		|| itemType == ItemType_Gift
		|| itemType == ItemType_Change
		|| itemType == ItemType_MASK
		|| itemType == ItemType_FillHP
		|| itemType == ItemType_FillMP;

	bool bVessalType = itemType == ItemType_Pack
		|| itemType == ItemType_Tonic
		|| itemType == ItemType_Change
		|| itemType == ItemType_PackHide ;

	bool bDefenceEquip = itemType == 101
		|| itemType == 102
		|| itemType == 103
		|| itemType == 112
		|| itemType == 113
		|| itemType == 114
		|| itemType == 115
		|| itemType == 116 ;

	bool bAttackEquip = itemType == 104
		|| itemType == 105
		|| itemType == 106
		|| itemType == 107
		|| itemType == 108
		|| itemType == 109
		|| itemType == 110
		|| itemType == 111
		|| itemType == 117
		|| itemType == 118 ;

	//Special type
	enum enumSpecialType
	{
		ST_NONE,
		ST_LeechdomFood,
	};
	enumSpecialType spType = ST_NONE;
	//Get some special type
	if ( ( enumItemType(m_pObjectBase->dwType) == ItemType_Leechdom )
		|| ( enumItemType(m_pObjectBase->dwType) == ItemType_Food ) )
	{
		if ( m_object.maxhp > 0 )
			spType = ST_LeechdomFood;
	}

	m_ToolTips.Clear();
	m_ToolTips.SetBorderColor(0);
	m_ToolTips.SetBkColor(COLOR_ARGB(128,0,0,0));
	m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
	char szTemp[512];
	//Name
	//if ( m_object.kind & 8 )		//set
	//	m_ToolTips.SetCurColor( COLOR_ARGB(255,146,77,22) );
	if( m_object.kind & 4 )
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,255,137) );
	else if( m_object.kind &2 )
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,222,3) );
	else if( m_object.kind & 1 )
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
	else
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );

	//Get diamond about
	int diamondNum = 0;
	for( int i=0;i<6;i++ )
	{
		if ( m_object.socket[i] == 1 )
			diamondNum++ ;
	}
	/*if ( diamondNum > 0 )
	Tips_AddString( "镶了魂魄的" );*/

	//Just a test
	/*m_object.strName[0] = 0L;
	sprintf( m_object.strName, "MeiLiDe0HuDie" );
	m_object.kind = 1;
	m_pObjectBase->dwID == 678;*/
	//End test

	//Split item name
	char* pName;
	char namePartOne[MAX_NAMESIZE];
	char namePartTwo[MAX_NAMESIZE];
	char nameFiveFull[MAX_NAMESIZE];

	namePartOne[0] = 0L;
	namePartTwo[0] = 0L;
	nameFiveFull[0] = 0L;

	pName = strpbrk( m_object.strName, "0" );
	if ( pName )
	{
		sprintf( namePartOne, ++pName );
		int len = strlen(m_object.strName)-strlen(namePartOne)-1;
		strncpy( namePartTwo, m_object.strName, len );
		namePartTwo[len] = 0;
	}
	else
		sprintf( namePartOne, m_object.strName );

	//Add five type name to partone
	if ( (m_object.fivetype >= FIVE_METAL) && (m_object.fivetype < FIVE_NONE) )
	{
		strcat( nameFiveFull, "【" );
		strcat( nameFiveFull, fiveName[m_object.fivetype] );
		strcat( nameFiveFull, "】" );
	}

	//End split item name

	/*static char* upgradeName[] = { " 一等", " 二等", " 三等", " 四等", " 五等",
			" 六等", " 七等", " 八等", " 九等", " 十等", "十一等", "十二等", "十三等", "十四等", "十五等" };*/

	if ( 4 & m_object.kind )				//神圣道具
	{
		//Tips_AddStringDefFont( "神圣" );
		Tips_AddStringDefFont( namePartOne );

		if ( strlen(nameFiveFull) > 0 )
		{
			//Back cur color
			DWORD color = m_ToolTips.GetCurColor();
			m_ToolTips.SetCurColor( fiveColor[m_object.fivetype] );
			Tips_AddStringDefFont( nameFiveFull );
			m_ToolTips.SetCurColor( color );
		}

		//Add name part two
		if ( strlen(namePartTwo) > 0 )
		{
			Tips_AddString( "\n" );
			Tips_AddStringDefFont( namePartTwo );
		}

		//Add grade
		char szUpgrade[MAX_NAMESIZE];
		szUpgrade[0] = 0L;
		for( int k=0;k<m_object.upgrade;k++ )
			strcat( szUpgrade, "★" );
		szUpgrade[strlen(szUpgrade)] = 0L;

		//Set color
		switch( m_object.upgrade )
		{
		case 0:
			break;
		case 1:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
			break;
		case 2:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,86,234) );
			break;
		case 3:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,86,234) );
			break;
		case 4:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,222,3) );
			break;
		case 5:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,222,3) );
			break;
		case 6:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,0) );
			break;
		case 7:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,0) );
			break;
		case 8:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,113,0) );
			break;
		case 9:
		case 10:
		case 11:
			m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
			break;
		}

		if ( strlen(szUpgrade) > 0 )
		{
			Tips_AddStringDefFont( "\n" );
			if ( (m_pObjectBase->dwType >= ItemType_ClothBody) && (m_pObjectBase->dwType <= ItemType_Fing)
				&& ( (m_object.upgrade == 10) || (m_object.upgrade == 11) || (m_object.upgrade == 12) )	)
			{
				stResourceLocation rHole;
				rHole.SetFileName( "data\\interfaces.gl" );
				rHole.group = 4;						
				rHole.frame = (m_object.upgrade == 10) ? 14 : 15 ;
				if (m_object.upgrade == 12)
				{
					rHole.frame = 50;
				}
				for( int i=0;i<m_object.upgrade;i++ )
					m_ToolTips.AddAnimation( &rHole, false );
			}
			else
				Tips_AddStringDefFont( szUpgrade );
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
			|| (m_pObjectBase->dwID == 877) ) )	//Set id limit		//劳动原料类		m_pObjectBase->dwType == 16
		{
			//Set color
			switch( m_object.upgrade )
			{
			case 0:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
				break;
			case 1:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,86,234) );
				break;
			case 2:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,222,3) );
				break;
			case 3:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,0) );
				break;
			case 4:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
				break;
			case 5:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
				break;
			case 6:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
				break;
			case 7:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
				break;
			case 8:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
				break;
			case 9:
				m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
				break;
			}

			Tips_AddStringDefFont( namePartOne );

			if ( strlen(nameFiveFull) > 0 )
			{
				//Back cur color
				DWORD color = m_ToolTips.GetCurColor();
				m_ToolTips.SetCurColor( fiveColor[m_object.fivetype] );
				Tips_AddStringDefFont( nameFiveFull );
				m_ToolTips.SetCurColor( color );
			}

			//Add upgrade
			Tips_AddStringDefFont( upgradeName[m_object.upgrade] );	

			//Add name part two
			if ( strlen(namePartTwo) > 0 )
			{
				Tips_AddString( "\n" );
				Tips_AddStringDefFont( namePartTwo );
			}
		}
		else
		{
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
					switch( m_object.upgrade )
					{
					case 0:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
						break;
					case 1:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,174,239) );
						break;
					case 2:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,8,251,231) );
						break;
					case 3:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,182,243,18) );
						break;
					case 4:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,246,101,44) );
						break;
					case 5:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,253,90,236) );
						break;
					case 6:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,222,3) );
						break;
					case 7:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,154,63,232) );
						break;
					case 8:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,188,66,210) );
						break;
					case 9:
					case 10:
					case 11:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
						break;
					default:
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
						break;
					}
				}
				else if ( (m_pObjectBase->dwID == 681)
					|| (m_pObjectBase->dwID == 792) 
					|| (m_pObjectBase->dwID == 794)
					|| (m_pObjectBase->dwID == 795) )
				{
					if ( (m_pObjectBase->dwID == 681) || (m_pObjectBase->dwID == 795) )
						m_ToolTips.SetCurColor( m_pObjectBase->attackSound | 0xff000000);
					else
						m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,137) );
				}
				else if ( (m_pObjectBase->dwID == 791)
					|| (m_pObjectBase->dwID == 790) )
				{
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,137) );
				}
				else if ( (m_pObjectBase->dwID == 680)
					|| (m_pObjectBase->dwID == 748)
					|| (m_pObjectBase->dwID == 749)
					|| (m_pObjectBase->dwID == 750)
					|| (m_pObjectBase->dwID == 751) )
				{
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,137) );
				}
				else
				{
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
				}

				Tips_AddStringDefFont( namePartOne );	//m_object.strName

				if ( strlen(nameFiveFull) > 0 )
				{
					//Back cur color
					DWORD color = m_ToolTips.GetCurColor();
					m_ToolTips.SetCurColor( fiveColor[m_object.fivetype] );
					Tips_AddStringDefFont( nameFiveFull );
					m_ToolTips.SetCurColor( color );
				}

				if ( (m_pObjectBase->dwID == 678)
					|| (m_pObjectBase->dwID == 679) )
				{
					Tips_AddStringDefFont( upgradeName[m_object.upgrade] );
				}

				if ( strlen( namePartTwo ) > 0 )
				{
					Tips_AddString( "\n" );
					Tips_AddStringDefFont( namePartTwo );
				}
			}
			else
			{
				if ( (m_pObjectBase->dwType == ItemType_ZhenSkillUp)
					|| (m_pObjectBase->dwID == 756)
					|| (m_pObjectBase->dwID == 757)
					|| (m_pObjectBase->dwID == 760)
					|| (m_pObjectBase->dwID == 761) )
                    m_ToolTips.SetCurColor( m_pObjectBase->attackSound | 0xff000000);

				Tips_AddStringDefFont( namePartOne );	//m_object.strName

				if ( strlen(nameFiveFull) > 0 )
				{
					//Back cur color
					DWORD color = m_ToolTips.GetCurColor();
					m_ToolTips.SetCurColor( fiveColor[m_object.fivetype] );
					Tips_AddStringDefFont( nameFiveFull );
					m_ToolTips.SetCurColor( color );
				}

				if ( strlen( namePartTwo ) > 0 )
				{
					Tips_AddString( "\n" );
					Tips_AddStringDefFont( namePartTwo );
				}

				//Add grade
				char szUpgrade[MAX_NAMESIZE];
				szUpgrade[0] = 0L;
				for( int k=0;k<m_object.upgrade;k++ )
					strcat( szUpgrade, "★" );
				szUpgrade[strlen(szUpgrade)] = 0L;

				switch( m_object.upgrade )
				{
				case 0:
					break;
				case 1:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,255,255) );
					break;
				case 2:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,86,234) );
					break;
				case 3:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,86,234) );
					break;
				case 4:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,222,3) );
					break;
				case 5:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,222,3) );
					break;
				case 6:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,0) );
					break;
				case 7:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,0,255,0) );
					break;
				case 8:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,255,113,0) );
					break;
				case 9:
				case 10:
				case 11:
					m_ToolTips.SetCurColor( D3DCOLOR_ARGB(255,211,0,237) );
					break;
				}

				if ( strlen(szUpgrade) > 0 )
				{
					Tips_AddStringDefFont( "\n" );
					if ( (m_pObjectBase->dwType >= ItemType_ClothBody) && (m_pObjectBase->dwType <= ItemType_Fing)
						&& ( (m_object.upgrade == 10) || (m_object.upgrade == 11) || (m_object.upgrade == 12)) )
					{
						stResourceLocation rHole;
						rHole.SetFileName( "data\\interfaces.gl" );
						rHole.group = 4;						
						rHole.frame = (m_object.upgrade == 10) ? 14 : 15 ;
						if (m_object.upgrade == 12)
						{
							rHole.frame = 50;
						}
						for( int i=0;i<m_object.upgrade;i++ )
							m_ToolTips.AddAnimation( &rHole, false );
					}
					else
						Tips_AddStringDefFont( szUpgrade );
				}
			}			
		}
	}

	//打造者
	if ( m_object.dwObjectID != 666 )
	{
		if( (m_object.dwObjectID != c_nHuoYunFu)	&&
			(m_object.dwObjectID != c_nXianTianFu) &&
			(m_object.dwObjectID != c_nXuanYuanFu) &&
			(m_object.dwObjectID != 675) &&
			(m_object.dwObjectID != 676) &&
			(m_object.dwObjectID != 786) &&  // 小护身符
			(m_object.dwObjectID != 787))    // 大护身符
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,113,0) );
			Tips_AddText("\n%s打造",m_object.maker );
		}
	}

	// Bind state
	if ( 1 == m_object.bind )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,122,210,58) );
		Tips_AddString( "\n已绑定" );
	}

	//Blank line
	Tips_AddString( "\n    " );

	//If this is a ItemType_ZhenSkillUp type item
	//Add serial type
	if ( m_pObjectBase->dwType == ItemType_ZhenSkillUp )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n阵法名称 " );
		if ( (m_pObjectBase->dwID >= 8300) && (m_pObjectBase->dwID <= 8399) )
		{
			Tips_AddString( "奇兵阵法" );
		}
		else if ( (m_pObjectBase->dwID >= 8400) && (m_pObjectBase->dwID <= 8499) )
		{
			Tips_AddString( "诡流阵法" );
		}
		else if ( (m_pObjectBase->dwID >= 8500) && (m_pObjectBase->dwID <= 8599) )
		{
			Tips_AddString( "天书阵法" );
		}
	}

	//Repair price
	if(m_dwTipFlags & ITEMTIPS_FLAG_REPAIR_PRICE)
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n修理价格 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,239,179,94) );
		Tips_AddText( "%s", Money2String(GetObjectRepairMoney(&m_object)) );		//GetPrice()
	}

	//Repair price by gold
	if(m_dwTipFlags & ITEMTIPS_FLAG_REPAIR_PRICEBYGOLD)
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n修理价格 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,239,179,94) );
		Tips_AddText("%s 金子",Money2String(GetObjectRepairByGold(&m_object)) );		//GetPrice()
	}

	//价格
	if(m_dwTipFlags & ITEMTIPS_FLAG_PRICE)
	{
		if ( m_object.pos.dwLocation == OBJECTCELLTYPE_SELL )
		{
			//Buy price
			//DWORD buyPrice = GetPrice( ITEM_PRICE_BUY );
			float buyPrice = m_object.price;
			//Make some adjust according to character goodness
			DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
			switch(goodness)
			{
			case GOODNESS_3:
				buyPrice *= 1.2f;
				break;
			case GOODNESS_4:
				buyPrice *= 1.5f;
				break;
			case GOODNESS_5:
			case GOODNESS_6:
				buyPrice *= 2.0f;
				break;
			}
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n购买价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( (DWORD)(buyPrice*m_object.dwNum + 0.99f) ) );
		}
		else if ( m_object.pos.dwLocation == OBJECTCELLTYPE_MYSHOP )
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n销售价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( GetItemTotalPrice(m_object) ) );
		}
		else if ( m_object.pos.dwLocation == OBJECTCELLTYPE_OTHERSSHOP)
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n购买价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( GetItemTotalPrice(m_object) ) );
		}
		else
		{
			//Sell price
			//DWORD sellPrice = m_object.GetPrice( ITEM_PRICE_SELL );
			float sellPrice = GetObjectSellMoneyByDur(&m_object);
			////Consider dur
			//if ( m_object.maxdur > 0 )
			//{
			//	if ( (m_pObjectBase->dwType == ItemType_Arrow) )
			//		sellPrice *= m_object.dur/m_object.maxdur;
			//	else
			//		sellPrice *= ((m_object.dur/25)*25) / m_object.maxdur;
			//}
			//
			////Make some adjust according to character goodness
			//sellPrice *= 0.50f;		
			DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
			switch(goodness)
			{
			case GOODNESS_3:
				sellPrice *= 0.7f;
				break;
			case GOODNESS_4:
				sellPrice *= 0.4f;
				break;
			case GOODNESS_5:
			case GOODNESS_6:
				sellPrice *= 0.1f;
				break;
			}
			//Tips_AddNum("\n价格 %u", (DWORD)(sellPrice + 0.99f));		//GetPrice()
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n出售价格 " );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddText( "%s", Money2String( (DWORD)(sellPrice*m_object.dwNum + 0.99f) ) );
		}

	}

	//Add equip position
	static const char* equipPos[] = { "", "头部", "身体", "右手", "左手"
		,"颈部", "腕部", "手指", "腰部", "足部" };		//1 ~ 9
	static const char* equipName[] = { "衣服", "衣服", "衣服", "刀", "剑"
		,"斧", "锤", "法杖", "弓", "扇子"
		,"棍", "盾牌", "头盔", "腰带", "护腕"
		,"鞋子", "项链", "戒指" };		//101 ~ 118

	if ( (m_pObjectBase->dwEquipType >= 1) && (m_pObjectBase->dwEquipType <= 9) )
	{
		if ( (m_pObjectBase->dwType >= 101) && (m_pObjectBase->dwType <= 118) )
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n装备类型		" );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddString( equipName[m_pObjectBase->dwType-101] );
		}

		if ( m_pObjectBase->dwType != 120 )
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
			Tips_AddString( "\n装备位置		" );
			m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
			Tips_AddString( equipPos[m_pObjectBase->dwEquipType] );
		}		
	}
	//end equip pos

	//Add special equip position
	if ( m_pObjectBase->dwType == 120 )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n装备类型		" );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
		/*if ( m_pObjectBase->dwID == 875 )
		{
			Tips_AddString( "花" );
		}
		else if ( m_pObjectBase->dwID == 876 )
		{
			Tips_AddString( "采集手套" );
		}
		else*/
			Tips_AddString( "特殊" );

		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n装备位置		" );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,150,226,76) );
		Tips_AddString( "右手" );
	}

	//需要等级
	if ( m_object.dwObjectID == 680 )		//魂魄石
	{
		if( m_object.needlevel <= GetScene()->GetMainCharacter()->GetLevel() )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		else
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,0,30));
		Tips_AddNum("\n需要等级 %u",m_object.needlevel);

		
		
	}
	else
	{
		if( m_object.needlevel <= GetScene()->GetMainCharacter()->GetLevel() )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		else
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,0,30));
		if ( (m_object.needlevel>0) && !bCanUse )
			Tips_AddNum("\n需要等级 %u",m_object.needlevel);
	}

	//Add equipment type that can use
	if ( m_pObjectBase->dwType == ItemType_SOULSTONE )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,150,226,76));
		switch( m_object.dwObjectID )
		{
		case 748:
			Tips_AddString( "\n可镶嵌在衣服和武器上" );
			break;
		case 749:
			Tips_AddString( "\n可镶嵌在头盔和鞋子上" );
			break;
		case 750:
			Tips_AddString( "\n可镶嵌在腰带和护腕上" );
			break;
		case 751:
			Tips_AddString( "\n可镶嵌在项链和戒指上" );
			break;
		}
	}
	

	/*//使用次数
	if ( ((m_pObjectBase->dwType==ItemType_Change) )
	&& ((m_object.maxdur-m_object.dur) > 0) )
	{
	m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
	Tips_AddString( "\n使用次数 " );
	m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
	Tips_AddNum("%u",m_object.maxdur-m_object.dur);			//m_pObjectBase->dwMaxDur/25
	}*/

	//For mask
	if ( (m_pObjectBase->dwType==ItemType_Change)
		|| (m_pObjectBase->dwType == ItemType_Move)
		|| (m_pObjectBase->dwType == ItemType_Scroll)
		|| (m_pObjectBase->dwType == ItemType_Scroll)
		/*|| (m_pObjectBase->dwType == ItemType_MASK)*/ )
	{
		if ( (m_object.maxdur-m_object.dur) > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n使用次数 " );
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum("%u",m_object.maxdur-m_object.dur);			//m_pObjectBase->dwMaxDur/25
		}

		if ( m_object.dur > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n剩余使用次数 " );
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum("%u",m_object.dur);			//m_pObjectBase->dwMaxDur/25
		}	
	}
	//耐久度
	if ( (m_object.maxdur>0)
		&& !bCanUse
		&& !ISARRAW(m_pObjectBase->dwType)
		&& !(m_pObjectBase->dwType == ItemType_MASK)
		&& !bVessalType
		/*&& (m_object.pos.dwLocation == OBJECTCELLTYPE_EQUIP)*/		//We can see dur anywhere
		&& !(m_pObjectBase->dwType == 53)
		&& !(m_pObjectBase->dwType == 52)
		&& !(m_pObjectBase->dwType == ItemType_Amulet) //护身符
		&& !(m_pObjectBase->dwType == ItemType_Move)
		&& !(m_pObjectBase->dwType == ItemType_Scroll)
		&& !(m_pObjectBase->dwType == ItemType_Pack)
		&& !(m_pObjectBase->dwType == ItemType_FashionBody)
		&& !(m_pObjectBase->dwType == ItemType_FillHP)
		&& !(m_pObjectBase->dwType == ItemType_FillMP) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
        Tips_AddString( "\n耐久度 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u/%u",(m_object.dur+49)/50,(m_object.maxdur+49)/50);			//m_pObjectBase->dwMaxDur/25
	}

	//For Mask
	if ( m_pObjectBase->dwType == ItemType_MASK )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n耐久度 " );

		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u/%u", m_object.dur, m_object.maxdur);			//m_pObjectBase->dwMaxDur/25
	}

	//For 荣誉之球
	if ( (m_pObjectBase->dwType == ItemType_Honor) &&(m_object.dur > 0) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n荣誉点 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum( "%u",m_object.dur );
	}

	//For Fill HP Item
	if ( (m_pObjectBase->dwType == ItemType_FillHP) && (m_object.dur > 0) )
	{
		//Attention: We multi the value by 100 cause of max dur is WORD but the real value is a DWORD
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n剩余血量 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
        Tips_AddNum( "%u", m_object.dur * 100 );

		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n最大血量 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
		Tips_AddNum( "%u", m_object.maxdur * 100 );
	}

	//For Fill MP Item
	if ( (m_pObjectBase->dwType == ItemType_FillMP) && (m_object.dur > 0) )
	{
		//Attention: We multi the value by 50 cause of max dur is WORD but the real value is a DWORD
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n剩余法力 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
		Tips_AddNum( "%u", m_object.dur * 50 );

		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n最大法力 " );
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
		Tips_AddNum( "%u", m_object.maxdur * 50 );
	}

	//For 双倍经验道具
	if ( (m_pObjectBase->dwType == ItemType_DoubleExp) && (m_object.dur > 0) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n时间 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum( "%u 分",m_object.dur/60 );		//Server send time in seconds
	}

	//For 自动喝药道具
	if ( (m_pObjectBase->dwType == ItemType_Tonic) && (m_object.dur > 0) )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n耐久度 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		//Tips_AddNum( "%u", m_object.dur );
		Tips_AddNum2("%u/%u",m_object.dur,m_object.maxdur);
	}

	//For package item
	if ( (m_pObjectBase->dwType == ItemType_Pack) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n最大时间 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		if ( m_pObjectBase->dwType == ItemType_FashionBody )
		{
            Tips_AddText( "%s", Minute2DayHourMinute( m_object.maxdur, TYPE_DAY_HOUR_MINUTE ) );	//3505
		}
		else
			Tips_AddText( "%s", Minute2DayHourMinute( m_object.maxdur, TYPE_DAY ) );

		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddString( "\n剩余时间 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		if ( m_pObjectBase->dwType == ItemType_FashionBody )
		{
			Tips_AddText( "%s", Minute2DayHourMinute( m_object.dur, TYPE_DAY_HOUR_MINUTE ) );	//2885
		}
		else
			Tips_AddText( "%s", Minute2DayHourMinute( m_object.dur, TYPE_DAY ) );
	}

	//For fashion
	if ( m_pObjectBase->dwType == ItemType_FashionBody )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n耐久度 " );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u/%u",m_object.dur,m_object.maxdur);			//m_pObjectBase->dwMaxDur/25
	}

	//Just for 记忆蝴蝶  小护身符 大护身符
	//Show map name and position
	if ( m_object.dwObjectID == 666 || m_object.dwObjectID == 786 || m_object.dwObjectID == 787)
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddText( "\n%s", m_object.maker );
		Tips_AddNum2( " (%u, %u)", m_object.durpoint, m_object.dursecond );
	}
	//End

	//
	if ( (m_object.dwObjectID == c_nXuanYuanFu) || (m_object.dwObjectID == c_nXianTianFu) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddText( "\n%s", m_object.maker );
		Tips_AddNum2( " (%u, %u)", m_object.durpoint, m_object.dursecond );
	}


	//Equipment property

	//攻击力（包括攻击力、远程伤害、仙术、法术）
	//Just a range 
	if ( (m_object.maxpdamage  || m_object.pdamage)
		&& !bVessalType )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n物理攻击力 " );
		if ( ( (m_object.maxpdamage > m_pObjectBase->maxpdam) || (m_object.pdamage > m_pObjectBase->pdam) ) )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u - %u",m_object.pdamage, m_object.maxpdamage);
	}

	//Just a range
	if ( ( m_object.maxmdamage || m_object.mdamage )
		&& !bVessalType  )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n魔法攻击力 " );
		if ( (m_object.maxmdamage > m_pObjectBase->maxmdam) || (m_object.mdamage > m_pObjectBase->mdam) )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum2("%u - %u",m_object.mdamage,m_object.maxmdamage);
	}

	//防御力（包括防御力、法术防御力）
	if ( m_object.pdefence > 0 )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n物理防御力 " );
		if ( m_object.pdefence > m_pObjectBase->pDef )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum("%u",m_object.pdefence);
	}


	if ( m_object.mdefence > 0 )
	{
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
		Tips_AddString( "\n魔法防御力 " );
		if ( m_object.mdefence  > m_pObjectBase->mDef )
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum("%u",m_object.mdefence);
	}

	m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
	//速度（物理攻击速度、施法速度、移动速度）
	Tips_AddNum("\n攻击速度 %u",m_object.akspeed);
	//Tips_AddNum("\n施法速度 %u",m_object.mgspeed);
	Tips_AddNum("\n移动速度 %u",m_object.mvspeed);

	m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
	//命中率（包括命中率、法术命中率）
	Tips_AddNum("\n命中率 %u%%",m_object.atrating);
	//Tips_AddNum("\n法术命中率 %u%%",m_object.mgrating);
	//闪避率（包括躲避率、法术躲避率）
	Tips_AddNum("\n躲避率 %u%%",m_object.akdodge);
	//Tips_AddNum("\n法术躲避率 %u%%",m_object.mgdodge);
	//双倍经验
	//Tips_AddNum("\n%u %%双倍经验值获得",m_object.doublexp);

	m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
	//恢复类（包括生命值恢复、法术值恢复、体力值恢复、装备耐久值恢复）
	if ( !( (spType == ST_LeechdomFood)	&& ( (m_object.maxhp == 8) || (m_object.maxhp == 9) ) ) )
		Tips_AddNum("\n生命值恢复 %u",m_object.hpr);
	if ( !( (spType == ST_LeechdomFood)	&& m_object.maxhp == 10) )
		Tips_AddNum("\n法术值恢复 %u",m_object.mpr);
	/*if ( !( (spType == ST_LeechdomFood)
		&& ( (m_object.maxhp == 4)
		|| (m_object.maxhp == 5)
		|| (m_object.maxhp == 6)
		|| (m_object.maxhp == 7)
		|| (m_object.maxhp == 12) ) ) )
		Tips_AddNum("\n体力值恢复 %u",m_object.spr);*/
	if ( (m_object.dwObjectID != 666)
		&& (m_object.dwObjectID != 786)//小护身符
		&& (m_object.dwObjectID != 787)//大护身符
		&& (m_pObjectBase->dwType != ItemType_Move)
		&& (m_pObjectBase->dwType != ItemType_Scroll)
		&& (m_pObjectBase->dwType != ItemType_Pack)
		&& (m_pObjectBase->dwType != ItemType_FashionBody) )
	{
		Tips_AddNum2("\n恢复装备耐久度 %u 于 %u 秒内",m_object.durpoint,m_object.dursecond);
	}

	m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
	//增加角色属性（包括当前生命值、最大生命值、当前法术值、最大法术值、当前体力值、最大体力值）
	if( !( (spType == ST_LeechdomFood) && ( (m_object.maxhp == 8) || (m_object.maxhp == 9) ) ) )
	{
		if ( m_object.maxhp > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n增加生命值" );

			if ( m_object.maxhp > m_pObjectBase->maxhp )
				m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));

			Tips_AddNum(" %u", m_object.maxhp);
		}
	}

	if( !( (spType == ST_LeechdomFood) && ( (m_object.maxhp == 10) || (m_object.maxhp == 11) ) ) )
	{
		if ( m_object.maxmp > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n增加法术值" );
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum(" %u",m_object.maxmp);
		}
	}

	/*if ( !( (spType == ST_LeechdomFood)
		&& ( (m_object.maxhp == 4)
		|| (m_object.maxhp == 5)
		|| (m_object.maxhp == 6)
		|| (m_object.maxhp == 7)
		|| (m_object.maxhp == 12) ) ) )
	{
		if ( m_object.maxsp > 0 )
		{
			m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));
			Tips_AddString( "\n增加体力值" );
			m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
			Tips_AddNum(" %u",m_object.maxsp);
		}

	}*/
	
	//Blue equipment
	//增加攻击力（包括增加攻击力x％、增加攻击力（人物等级/x）、增加仙术x%、增加仙术(人物等级/x)、增加法术攻击力x%、增加法术攻击力(人物等级/x)）
	m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
	if ( !( (spType == ST_LeechdomFood)	&& (m_object.maxhp == 1) ) )
		Tips_AddNum("\n增加物理攻击力 %u%%",m_object.pdam);
	Tips_AddNum("\n增加魔法攻击力 %u%%",m_object.mdam);
	//增加防御力（包括防御力增强、法术防御力增强）
	if( !( (spType == ST_LeechdomFood) && (m_object.maxhp == 2) ) )
		Tips_AddNum("\n物理防御力增强 %u%%",m_object.pdef);
	Tips_AddNum("\n魔法防御力增强 %u%%",m_object.mdef);

	//Add equipment special addation
	m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
	Tips_AddNum( "\n增加出现重击几率 %u%%", m_object.bang );

	Tips_AddNum("\n减少进入中毒状态几率 %u%%",m_object.poisondef>100 ? 100 : m_object.poisondef);
	Tips_AddNum("\n减少进入冰冻状态几率 %u%%",m_object.colddef>100 ? 100 : m_object.colddef);
	Tips_AddNum("\n减少进入石化状态几率 %u%%",m_object.petrifydef>100 ? 100: m_object.petrifydef);
	Tips_AddNum("\n减少进入失明状态几率 %u%%",m_object.blinddef>100 ? 100 : m_object.blinddef);
	Tips_AddNum("\n减少进入混乱状态几率 %u%%",m_object.chaosdef>100 ? 100 : m_object.chaosdef);

	Tips_AddNum("\n减少进入麻痹状态几率 %u%%",m_object.lulldef>100 ? 100 : m_object.lulldef);
	Tips_AddNum("\n减少进入眩晕状态几率 %u%%",m_object.reeldef>100 ? 100 : m_object.reeldef);
	Tips_AddNum("\n减少进入噬魔状态几率 %u%%",m_object.evildef>100 ? 100 : m_object.evildef);
	Tips_AddNum("\n减少进入噬力状态几率 %u%%",m_object.bitedef>100 ? 100 : m_object.bitedef);
	Tips_AddNum("\n减少进入定身状态几率 %u%%",m_object.stabledef>100 ? 100 : m_object.stabledef);
	Tips_AddNum("\n减少进入减速状态几率 %u%%",m_object.slowdef>100 ? 100 : m_object.slowdef);
	Tips_AddNum("\n减少进入诱惑状态几率 %u%%",m_object.luredef>100 ? 100 : m_object.luredef);

	Tips_AddNum("\n增加 %u%% 几率使敌人进入中毒状态",m_object.poison);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入冰冻状态",m_object.cold);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入石化状态",m_object.petrify);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入失明状态",m_object.blind);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入混乱状态",m_object.chaos);

	Tips_AddNum("\n增加 %u%% 几率使敌人进入麻痹状态",m_object.lull);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入眩晕状态",m_object.reel);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入噬魔状态",m_object.evil);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入噬力状态",m_object.bite);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入定身状态",m_object.stable);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入减速状态",m_object.slow);
	Tips_AddNum("\n增加 %u%% 几率使敌人进入诱惑状态",m_object.lure);

	//Add equipment detail property
	Tips_AddNum("\n力量 +%u", m_object.str);
	Tips_AddNum("\n智力 +%u", m_object.inte);
	Tips_AddNum("\n敏捷 +%u", m_object.dex);
	Tips_AddNum("\n精神 +%u", m_object.spi);
	Tips_AddNum("\n体质 +%u", m_object.con);

	//Skill effect
	{
		int num = 0;
		for( int k=0;k<10;k++ )
		{
			SkillBase_t* pSkill = GetSkillBase( m_object.skill[k].id );
			if ( pSkill )
			{
				if ( num == 0 )
					Tips_AddString( "\n" );
				num++ ;
				Tips_AddText( "%s +", pSkill->pstrName );
				Tips_AddNum( "%u", m_object.skill[k].point );
				if ( num >= 2 )
					num = 0;
			}
		}
	}

	{
		static const char* SkillSerialName[] = { "武术系", "射术系", "法术系", "召唤系", "仙术系", "阵法" };
		if ( (m_object.skills.id > 0) && (m_object.skills.id < 7) )
		{
			Tips_AddText( "\n%s全系技能 +", SkillSerialName[m_object.skills.id-1] );
			Tips_AddNum( "%u", m_object.skills.point );
		}
	}

	//Add experience
	//if ( m_pObjectBase->dwType != ItemType_Pack )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,0,86,234) );
		Tips_AddNum( "\n附加经验 %u", m_object.exp );
	}	

	//Green equipment
	//增加伤害值（包括增加伤害值x％、增加伤害值（人物等级/x）、增加远程伤害x%、增加远程伤害（人物等级/x）、）
	m_ToolTips.SetCurColor(COLOR_ARGB(255,0,255,137));
	//if( !( (spType == ST_LeechdomFood) && (m_object.damagebonus != 0) ) )
	//{
	//	Tips_AddNum("\n伤害损耗 %u%%",m_object.damagebonus);
	//	//Tips_AddNum("\n增加伤害值(人物等级/%u)",m_object.ehpv);
	//}

	if ( m_object.hpleech.effect > 0 )
		Tips_AddNum2("\n%u%% 的几率转化对敌人的伤害为自身生命 %u 点",m_object.hpleech.odds, m_object.hpleech.effect);
	if ( m_object.mpleech.effect > 0 )
		Tips_AddNum2("\n%u%% 的几率转化对敌人的伤害为自身法术 %u 点",m_object.mpleech.odds, m_object.mpleech.effect);
	////特殊类（神圣一击、转换生命值为法术值x％、x%忽视目标防御）
	m_ToolTips.SetCurColor(COLOR_ARGB(255,42,246,138));
	Tips_AddNum("\n神圣一击几率 %u%%", m_object.holy);
	//End add equipment special addation
	m_ToolTips.SetCurColor(COLOR_ARGB(255,42,246,138));
	Tips_AddNum("\n伤害 %u%% 转化为法术消耗",m_object.hptomp);
	Tips_AddNum("\n增加银子掉落 %u%%",m_object.incgold);
	Tips_AddNum("\n增加掉宝率 %u%%",m_object.mf);
	Tips_AddNum("\n双倍经验 %u%%",m_object.doublexp);
	//Add damage
	//Tips_AddNum( "\n增加伤害值 %u%%", m_object.damage );

	//Golden equipment
	if ( (m_object.kind & 8) )			//&& !(g_tableItemSetBase.empty())
	{
		//Show set property
		//stSetObjectPropertyActive setActive;
		//if ( GetScene()->GetMainCharacter() )
		//{
		//	GetScene()->GetMainCharacter()->GetSetActiveByName( m_object.set, setActive );			//"幽冥"
		//}
		////Draw
		//static const char* property[] = { "\n增加攻击力2％",
		//								"\n增加法术攻击力2％", 
		//								"\n增加远程伤害2％",
		//								"\n增加仙术2％",
		//								"\n增加防御力2％",
		//								"\n增加法术防御力2％",
		//								"\n增加生命值5％", 
		//								"\n增加法术值5％", 
		//								"\n增加体力值5％", 
		//								"\n伤害反射10%", };
		//for( int i=0;i<SET_PROPERTY_NUM;i++ )
		//{
		//	if ( setActive.bActive[i] )
		//		m_ToolTips.SetCurColor( COLOR_ARGB(255,226,146,33) );
		//	else
		//		m_ToolTips.SetCurColor( COLOR_ARGB(255,92,83,79) );
		//	Tips_AddString( property[i] );
		//}
		/*m_ToolTips.SetCurColor( COLOR_ARGB(255,226,146,33) );
		Tips_AddNum( "\n物理伤害减少 %u%%", m_object.dpdam );
		Tips_AddNum( "\n魔法伤害减少 %u%%", m_object.dmdam );
		Tips_AddNum( "\n增加伤害 %u%%", m_object.bdam );
		Tips_AddNum( "\n伤害反射 %u%%", m_object.rdam );
		Tips_AddNum( "\n%%%u忽视目标防御", m_object.ignoredef );*/
	}

	/*//Just a test
	m_object.fivetype = 1;
	m_object.fivepoint = 33;

	m_object.fiveset[0] = 10101;	//fiveType=4
	m_object.fiveset[1] = 10751;	//fiveType=2
	m_object.fiveset[2] = 10801;	//fiveType=5
	m_object.fiveset[3] = 10851;	//fiveType=3
	m_object.fiveset[4] = 0;
	//End test*/

	//Five
	/*if ( (m_object.fivetype >= FIVE_METAL) && (m_object.fivetype<FIVE_NONE) && (m_object.fivepoint > 0) )
	{
		m_ToolTips.SetCurColor( fiveColor[m_object.fivetype] );
		if ( bDefenceEquip )
		{
			Tips_AddText( "\n减少来自无五行与%s属性目标的", fiveName[GetFiveBack(m_object.fivetype)] );
			Tips_AddNum( "伤害 %u%%", m_object.fivepoint );
		}

		if ( bAttackEquip )
		{
			Tips_AddText( "\n对无五行与%s属性目标", fiveName[GetFiveBack(m_object.fivetype)] );
			Tips_AddNum( "伤害上升 %u%%", m_object.fivepoint );
		}
	}*/
	//End five

	//Five set
	static const char* fiveDesc[] = { "\n   物理伤害减少 ", "\n   魔法伤害减少 ",
		"\n   增加伤害 ", "\n   伤害反射 ", "忽视目标防御" };
	struct fivePro{
		BYTE fiveValue;
		BYTE proType;
	};
	fivePro fiveProperty[FIVE_NONE];
	for( int k=0;k<FIVE_NONE;k++ )
	{
		fiveProperty[k].fiveValue = 0;
		fiveProperty[k].proType = FIVE_NONE;
	}
	int index = 0;
	if ( m_object.dpdam )
	{
		fiveProperty[index].fiveValue = m_object.dpdam;
		fiveProperty[index++].proType = 0;
	}
	if ( m_object.dmdam )
	{
		fiveProperty[index].fiveValue = m_object.dmdam;
		fiveProperty[index++].proType = 1;
	}
	if ( m_object.bdam )
	{
		fiveProperty[index].fiveValue = m_object.bdam;
		fiveProperty[index++].proType = 2;
	}
	if ( m_object.rdam )
	{
		fiveProperty[index].fiveValue = m_object.rdam;
		fiveProperty[index++].proType = 3;
	}
	if ( m_object.ignoredef )
	{
		fiveProperty[index].fiveValue = m_object.ignoredef;
		fiveProperty[index++].proType = 4;
	}

	int nNextFiveType = GetFiveBorn( m_object.fivetype );
	for( int five=0;five<FIVE_NONE;five++ )
	{
		if ( m_object.fiveset[five] == 0 )
			break;

		if ( GetScene()->GetMainCharacter() )
		{
			//See if active five property
			bool bActiveFive = m_object.pos.dwLocation == OBJECTCELLTYPE_EQUIP;
			
			CRoleItem* pItem = NULL;			
			if ( m_object.pos.dwLocation == OBJECTCELLTYPE_EQUIPSHOW )	//This item belong to other play
			{
				if ( GetGameGuiManager()->m_guiUserInfoShow )
				{
					pItem = GetGameGuiManager()->m_guiUserInfoShow->FindItemInEquipListByFiveTypeConstID( m_object.dwObjectID, m_object.fivetype );
				}
			}
			else
				pItem = GetScene()->GetMainCharacter()->FindItemByLocationFiveTypeConstID( m_object.fiveset[five], OBJECTCELLTYPE_EQUIP, nNextFiveType );
			
			if ( NULL == pItem )
			{
				bActiveFive = false;
				//pItem = GetScene()->GetMainCharacter()->FindItemByConstID( m_object.fiveset[five] );
			}

			ObjectBase_t* pObjectBase = GetObjectBase( m_object.fiveset[five] );
			if ( pObjectBase == NULL )
				break;

			{
				//if ( five > 0 )
				{
					if ( pItem && (pItem->GetObject()->fivetype != nNextFiveType) )
						bActiveFive = false;
				}

				//Add property tips
				if ( five == 0 )
				{
					m_ToolTips.SetCurColor( COLOR_ARGB(255,87,37,142) );
					Tips_AddString( "\n灵魂锁链：" );
				}

				if ( bActiveFive )
					m_ToolTips.SetCurColor( COLOR_ARGB(255,87,37,142) );
				else
					m_ToolTips.SetCurColor( COLOR_ARGB(255,73,73,71) );
				Tips_AddNum( "\n(%u)", five+1 );
				Tips_AddText( "%s", pObjectBase->strName );
				if ( 0/*pItem*/ )
				{
					Tips_AddText( "【%s】", fiveName[pItem->GetObject()->fivetype] );
				}
				else
				{
					Tips_AddText( "【%s】", fiveName[nNextFiveType] );
				}

				if ( fiveProperty[five].fiveValue && (fiveProperty[five].proType != FIVE_NONE) )
				{
					if ( fiveProperty[five].proType != (FIVE_NONE-1) )
					{
						Tips_AddText( "%s", fiveDesc[fiveProperty[five].proType] );
						Tips_AddNum( "%u%%", fiveProperty[five].fiveValue );
					}
					else
					{
						Tips_AddNum( "\n   %u%% ", fiveProperty[five].fiveValue );
						Tips_AddText( "%s", fiveDesc[fiveProperty[five].proType] );
					}
				}

				/*if ( pItem )
					nNextFiveType = GetFiveBorn( pItem->GetObject()->fivetype );
				else*/
					nNextFiveType = GetFiveBorn( nNextFiveType );


				/*switch( five )
				{
				if ( fiveProperty[five].proType != (FIVE_NONE-1) )
				{
				Tips_AddText( "%s", fiveDesc[fiveProperty[five].proType] );
				Tips_AddNum( "%u%%", fiveProperty[five].fiveValue );
				}
				else
				{
				Tips_AddNum( "\n   %u%% ", fiveProperty[five].fiveValue );
				Tips_AddText( "%s", fiveDesc[fiveProperty[five].proType] );
				}
				}


				/*switch( five )
				{
				if ( fiveProperty[five].proType != (FIVE_NONE-1) )
				{
				Tips_AddText( "%s", fiveDesc[fiveProperty[five].proType] );
				Tips_AddNum( "%u%%", fiveProperty[five].fiveValue );
				}
				else
				{
				Tips_AddNum( "\n   %u%% ", fiveProperty[five].fiveValue );
				Tips_AddText( "%s", fiveDesc[fiveProperty[five].proType] );
				}
				}


				/*switch( five )
				{
				case 0:
				Tips_AddNum( "\n   物理伤害减少 %u%%", m_object.dpdam );
				break;
				case 1:
				Tips_AddNum( "\n   魔法伤害减少 %u%%", m_object.dmdam );
				break;
				case 2:
				Tips_AddNum( "\n   增加伤害 %u%%", m_object.bdam );
				break;
				case 3:
				Tips_AddNum( "\n   伤害反射 %u%%", m_object.rdam );
				break;
				case 4:
				Tips_AddNum( "\n   %u%% 忽视目标防御", m_object.ignoredef );
				break;
				}								

				Tips_AddNum( "\n   %u%% 忽视目标防御", m_object.ignoredef );
				break;
				}				*/				

			}
		}
	}
	//End five set
	//end equipment property

	//孔
	//Get hole about
	{
		int holeNum = 0;
		int diamondNum = 0;
		for( int i=0;i<6;i++ )
		{
			if ( m_object.socket[i] != 0 )
			{
				holeNum++ ;
				if ( m_object.socket[i] != -1 )
					diamondNum++ ;
			}
		}
		m_ToolTips.SetCurColor(COLOR_ARGB(255,0,86,234));
		Tips_AddNum( "\n有凹槽 %u", holeNum );
		m_ToolTips.SetCurColor(COLOR_ARGB(255,255,255,255));

		//Add hole & diamond image
		stResourceLocation rHole;
		rHole.SetFileName( "data\\interfaces.gl" );
		rHole.group = 4;
		rHole.frame = 8;

		stResourceLocation rHoleDiamond;
		rHoleDiamond.SetFileName( "data\\interfaces.gl" );
		rHoleDiamond.group = 4;
		rHoleDiamond.frame = 9;

		int k;
		int num = 0;
		if ( holeNum > 0 )
		{
			Tips_AddString( "\n" );
			for( k=0;k<6;k++ )
			{
				if ( m_object.socket[k] !=0 )
				{
					if ( m_object.socket[k] != -1 )		//diamond
					{
						int imgID = GetDiamondResourceFrameFromID( m_object.socket[k] );
						if ( imgID != -1 )
						{
							num++ ;
							rHoleDiamond.frame = imgID;
							m_ToolTips.AddAnimation( &rHoleDiamond, false );
							if ( num >= 3 )
							{
								num = 0;
								Tips_AddString( "\n" );
							}
						}
					}
					else								//hole
					{
						num++ ;
						m_ToolTips.AddAnimation( &rHole, false );
						if ( num >= 3 )
						{
							num = 0;
							Tips_AddString( "\n" );
						}
					}
				}
			}
			/*if ( diamondNum > 0 )
			{
			for( k=0;k<diamondNum;k++ )
			{
			num++ ;
			m_ToolTips.AddAnimation( &rHoleDiamond, false );
			if ( num >= 3 )
			{
			num = 0;
			Tips_AddString( "\n" );
			}
			}
			}

			if ( (holeNum - diamondNum) > 0 )
			{
			for( k=0;k<holeNum-diamondNum;k++ )
			{
			num++ ;					
			m_ToolTips.AddAnimation( &rHole, false );
			if ( num >= 3 )
			{
			num = 0;
			Tips_AddString( "\n" );
			}
			}
			}*/
		}
	}			


	//Can used for skill tips
	m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
	if ( (m_pObjectBase->dwType >=104) && (m_pObjectBase->dwType<=107) )
		Tips_AddString( "\n适用于武术系技能" );
	if ( m_pObjectBase->dwType == 108 )
		Tips_AddString( "\n适用于法术系技能" );
	if ( m_pObjectBase->dwType == 109 )
		Tips_AddString( "\n适用于射术系技能" );
	if ( m_pObjectBase->dwType == 110 )
		Tips_AddString( "\n适用于仙术系技能" );
	if ( m_pObjectBase->dwType == 111 )
		Tips_AddString( "\n适用于召唤系技能" );
	//

	//Show item description
	if ( ( strcmp( m_pObjectBase->strDesc, "无" ) != 0 )
		&& ( strcmp( m_pObjectBase->strDesc, "" ) != 0 ) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,255,255,255) );
		Tips_AddText( "\n%s", m_pObjectBase->strDesc );
	}

	if ( m_dwTipFlags & ITEMTIPS_FLAG_RIGHT_CLICK_USE )
	{
		//Add tips
		m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
		if ( (m_pObjectBase->dwType >= 101) && (m_pObjectBase->dwType <= 118 )
			|| (m_pObjectBase->dwType == 30 )
			|| (m_pObjectBase->dwType == 52 )
			|| (m_pObjectBase->dwType == 53 )
			|| (m_pObjectBase->dwType == 119 )
			|| (m_pObjectBase->dwType == ItemType_Amulet ) 
			|| (m_pObjectBase->dwType == 62 )
			|| (m_pObjectBase->dwType == 120 ) )
		{
			Tips_AddString( "\n右键点击可以装备" );
		}
		else if( (m_pObjectBase->dwType == 17)
			|| (m_pObjectBase->dwType == 19 )
			|| (m_pObjectBase->dwType == 20 )			
			|| (m_pObjectBase->dwType == 25 )
			|| (m_pObjectBase->dwType == 26 )			
			|| (m_pObjectBase->dwType == 33 )
			|| (m_pObjectBase->dwType == 42 )
			|| (m_pObjectBase->dwType == 54 )
			|| (m_pObjectBase->dwType == 55 )
			|| (m_pObjectBase->dwType == 51 )			
			|| (m_pObjectBase->dwType == 57 )
			|| (m_pObjectBase->dwType == 58 )
			|| (m_pObjectBase->dwType == 59 )
			|| (m_pObjectBase->dwType == 60 )
			|| (m_pObjectBase->dwType == 63 )
			|| (m_pObjectBase->dwType == 46 )
			|| (m_pObjectBase->dwID == 756) || (m_pObjectBase->dwID == 757) )		//|| (m_pObjectBase->dwType == 41 )
		{
			Tips_AddString( "\n右键点击可以使用" );
		}
		else if( m_pObjectBase->dwType == 23 )
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
			Tips_AddString( "\n可装备至包裹栏" );
		}
		else if( (m_pObjectBase->dwType == 41))//细分 task3605
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
			Tips_AddString( "\n可装备至功能栏" );
		}
		/*else if (m_pObjectBase->dwType == 23 )
		{
			if (m_object.pos.dwLocation!=OBJECTCELLTYPE_EQUIP)
			{
				Tips_AddString( "\n（右键点击可以装备）" );
			}
		}
		*/
		else if( (m_pObjectBase->dwType == 31 ))//细分 task3605
		{
			m_ToolTips.SetCurColor( COLOR_ARGB(255,121,100,36) );
			Tips_AddString( "\n（右键点击可以打开）" );
		}
		
	}

	//Add set maker name
	if ( IsBelongToMakerSet( m_object ) )
	{
		m_ToolTips.SetCurColor( COLOR_ARGB(255,120,240,30) );
		Tips_AddText( "\n%s 打造的套装", m_object.maker );
	}

	m_ToolTips.Resize();

	FUNCTION_END;
}


void  ActiveItem(CRoleItem* pRoleItem, bool bMainCharacter)
{
	FUNCTION_BEGIN;

	if (pRoleItem==NULL)
		return;

	t_Object& m_object=*(pRoleItem->GetObject());

	int nNextFiveType = GetFiveBorn( m_object.fivetype );
	for( int five=0;five<FIVE_NONE;five++ )
	{
		if ( m_object.fiveset[five] == 0 )
			break;

		if ( GetScene() && GetScene()->GetMainCharacter() )
		{
			CRoleItem* pItem = NULL;
			if ( bMainCharacter )
				pItem = GetScene()->GetMainCharacter()->FindItemByLocationFiveTypeConstID( m_object.fiveset[five], OBJECTCELLTYPE_EQUIP, nNextFiveType );
			else
			{
				if ( GetGameGuiManager()->m_guiUserInfoShow )
					pItem = GetGameGuiManager()->m_guiUserInfoShow->FindItemInEquipListByFiveTypeConstID( m_object.fiveset[five], nNextFiveType );
			}

			if ( NULL == pItem )
				break;

			ObjectBase_t* pObjectBase = GetObjectBase( m_object.fiveset[five] );
			if ( pObjectBase == NULL )
				break;

			if ( pItem && (pItem->GetObject()->fivetype != nNextFiveType) )
				break;

			if ( bMainCharacter )
			{
				if (GetGameGuiManager()->m_guiUserInfo)
				{
					GetGameGuiManager()->m_guiUserInfo->InsertActiveItem(pRoleItem);
					//GetGameGuiManager()->m_guiUserInfo->InsertActiveItem(pItem);
				}
			}
			else
			{
				if (GetGameGuiManager()->m_guiUserInfoShow)
				{
					GetGameGuiManager()->m_guiUserInfoShow->InsertActiveItem(pRoleItem);
					//GetGameGuiManager()->m_guiUserInfo->InsertActiveItem(pItem);
				}
			}
			
		}

		nNextFiveType = GetFiveBorn( nNextFiveType );			
	}

	FUNCTION_END;
}

bool IsBelongToMakerSet(const t_Object& object)
{
	if ( object.kind == 0 )		//White equip
		return false;

	if ( object.bind != 1 )		//Not binded
		return false;

	if ( strlen(object.maker) <= 0 )	//Is not be maked
		return false;

	stObjectLocation pos = object.pos;
	if ( pos.dwLocation == OBJECTCELLTYPE_EQUIP )	//It's yours
	{
		
		if ( GetGameGuiManager()->m_guiUserInfo )
		{
			if ( !(GetGameGuiManager()->m_guiUserInfo->GetSetEquipType() & object.kind) )	
				return false;
			if ( GetGameGuiManager()->m_guiUserInfo->GetSetMakerName() )
			{
				if ( strcmp( object.maker, GetGameGuiManager()->m_guiUserInfo->GetSetMakerName() ) == 0 )
					return true;
			}
		}		
	}
	else if ( pos.dwLocation == OBJECTCELLTYPE_EQUIPSHOW )		//Other player show
	{
		if ( GetGameGuiManager()->m_guiUserInfoShow )
		{
			if ( GetGameGuiManager()->m_guiUserInfoShow->GetSetEquipType() != object.kind )		//Not the same kind
				return false;
			if ( GetGameGuiManager()->m_guiUserInfoShow->GetSetMakerName() )
			{
				if ( strcmp( object.maker, GetGameGuiManager()->m_guiUserInfoShow->GetSetMakerName() ) == 0 )
					return true;
			}                   			
		}	
	}

	return false;
}

bool CRoleItem::IsBelongToMakerSet()
{
	return ::IsBelongToMakerSet( m_object );	
}

bool IsShowActiveItem(CRoleItem* pRoleItem)
{
	FUNCTION_BEGIN;

	if (pRoleItem==NULL)
		return false;

	t_Object& m_object=*(pRoleItem->GetObject());

	if ( m_object.fiveset[0] == 0 )
		return false;

	return true;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param flag : 描述
 * \return 返回值的描述
 */
DWORD CRoleItem::GetPrice( int flag )
{
	FUNCTION_BEGIN;

	float ret = m_pObjectBase->price;

	switch( flag )
	{
	case ITEM_PRICE_SHOP:
		return m_price;
	case ITEM_PRICE_COMMON:		//base price
		{
			if(GetThisID() == INVALID_THISID)
				return ret;
			return 0;
		}
		break;
	case ITEM_PRICE_SELL:		//sell price
		{
			float sellPrice = ret;
			//Consider dur
			if ( m_object.maxdur > 0 )
			{
				if ( (m_pObjectBase->dwType == ItemType_Arrow) )
					sellPrice *= m_object.dur/m_object.maxdur;
				else
					sellPrice *= ((m_object.dur/25)*25) / m_object.maxdur;
			}			

			//Make some adjust according to character goodness
			sellPrice *= 0.25f;
			
			DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
			switch(goodness)
			{
			case GOODNESS_3:
				sellPrice *= 0.7f;
				break;
			case GOODNESS_4:
				sellPrice *= 0.4f;
				break;
			case GOODNESS_5:
			case GOODNESS_6:
				sellPrice *= 0.1f ;
				break;
			}

			return (DWORD)(sellPrice + 0.99f);
		}
		break;
	case ITEM_PRICE_BUY:	//buy price
		{
			float buyPrice = ret;			
			//Make some adjust according to character goodness
			DWORD goodness = GetScene()->GetMainCharacter()->GetGoodnessAfterAdjust();
			switch(goodness)
			{
			case GOODNESS_3:
				buyPrice *= 1.2f;
				break;
			case GOODNESS_4:
				buyPrice *= 1.5f;
				break;
			case GOODNESS_5:
			case GOODNESS_6:
				buyPrice *= 2.0f;
				break;
			}

			//Consider middle country
			if ( GetScene()->IsCurMapIsMiddleCountry() )
				buyPrice *= 1.1f;

			return (DWORD)(buyPrice + 0.99f);
		}
		break;
	}

	return ret;
	return 0;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param flag : 描述
 * \return 返回值的描述
 */
DWORD CRoleItem::GetRepairPrice( int flag )
{
	FUNCTION_BEGIN;

	return GetPrice( flag );

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwFlags : 描述
 * \return 返回值的描述
 */
void CRoleItem::SetTipFlags(DWORD dwFlags)
{
	FUNCTION_BEGIN;

	if(m_dwTipFlags != dwFlags)
	{
		m_dwTipFlags = dwFlags;
		m_bNeedUpdateTips = true;
	}

	FUNCTION_END;
}

/*
*	函数:		
*	返回值：	
*	参数：		
*	
*	用途: 		  能否装备
*/
DWORD CRoleItem::IsCanEquip()
{
	FUNCTION_BEGIN;

	if(0 == (m_pObjectBase->dwCharType & GetScene()->GetMainCharacter()->GetType()))
	{
		return CANNOTUSE_CHARTYPE;
	}

	if ( (m_pObjectBase->dwType == ItemType_Tonic )
		|| (m_pObjectBase->dwType == ItemType_DoubleExp )
		|| (m_pObjectBase->dwType == ItemType_Honor )
		|| (m_pObjectBase->dwType == ItemType_Flower) )
	{
		return 0;
	}

	if(m_object.needlevel > GetScene()->GetMainCharacter()->GetLevel())
	{
		return CANNOTUSE_LEVEL;
	}
	return 0;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwDur : 描述
 * \return 返回值的描述
 */
void CRoleItem::OnUpdateDur(DWORD dwDur,DWORD dwMaxDur)
{
	FUNCTION_BEGIN;

	m_object.dur =dwDur;
	m_object.maxdur = dwMaxDur;
	m_bNeedUpdateTips = true;

	//Add to repair list
	if ( NeedConsiderDur() )
	{
		if ( (m_object.dur + 49)/50 < NEED_ADD_REPAIR_VALUE )
		{
			if ( GetScene()->GetMainCharacter() )
				GetScene()->GetMainCharacter()->AddOneNeedRepairItem( m_object.qwThisID );
		}
		else
		{
			if ( GetScene()->GetMainCharacter() )
				GetScene()->GetMainCharacter()->DeleteOneNeedRepairItem( m_object.qwThisID );
		}
	}

	if (IsArrow())
		OnUpdate(UPDATE_FLAG_COUNT);

	//Arrow is special 
	if ( IsArrow() && (m_object.pos.dwLocation == OBJECTCELLTYPE_EQUIP) )
	{
		if ( GetExteriorCount() < 20 )
		{
			GetScene()->GetMainCharacter()->AddOneNeedRepairItem( m_object.qwThisID );
		}
		else
		{
			GetScene()->GetMainCharacter()->DeleteOneNeedRepairItem( m_object.qwThisID );
		}		
	}

	//Auto equip fill-mp item
	if ( ( (GetItemType() == ItemType_FillMP) || (GetItemType() == ItemType_Tonic) )
		&& (m_object.dur == 0)
		&& (GetLocation().dwLocation == OBJECTCELLTYPE_EQUIP)
		&& (m_object.bind == 1) )
	{
		GetScene()->GetMainCharacter()->AutoEquipFillItem( GetLocation().x, GetItemType() );
	}

	//Big fill-hp item access key change
	if ( (GetItemType() == ItemType_FillHP)
		&& (dwDur == 0)
		&& (GetLocation().dwLocation == OBJECTCELLTYPE_COMMON)
		&& (m_object.bind == 1) )
	{
		DWORD otherItemID = GetScene()->GetMainCharacter()->FindOtherItemWithSameTypeID( GetThisID(), GetObjectID() );
		if ( otherItemID != -1 )
			AccessKeyOnRemove( otherItemID );
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
//Move float item back
void CRoleItem::MoveBackOldLocation()
{
	FUNCTION_BEGIN;

	DestoryAllVirtualCell();
	//MoveTo( m_oldLocation );

	FUNCTION_END;
}

inline bool CRoleItem::IsCanUse()
{ 
	// 打造字段数值为0  打造类型字段为34 同时物品类型为34
	if( GetItemType() == ItemType_Quest )
	{
		if( m_pObjectBase )
		{
			if( m_pObjectBase->dwMake == 0 || m_pObjectBase->maketype == 34)
			{
				return true;
			}
		}
	}

	if ( (GetObjectID() == 821)
		 || (GetObjectID() == 831)
		 || (GetObjectID() == 841) )
	{
		return true;
	}

	return	GetItemType() == ItemType_Leechdom 
		|| GetItemType() == ItemType_Food 
		|| GetItemType() == ItemType_Tools
		|| GetItemType() == ItemType_Pack
		|| GetItemType() == ItemType_Move
		|| GetItemType() == ItemType_Scroll
		|| GetItemType() == ItemType_Gift
		|| GetItemType() == ItemType_MASK
		|| GetItemType() == ItemType_State
		|| GetItemType() == ItemType_FillHP
		|| GetItemType() == ItemType_FillMP
		|| GetItemType() == 60;
}		


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void MoveBackFloatItem()
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	if ( pFloatItem = GetGuiManager()->GetFloatItem() )
	{
		CRoleItem* pRoleItem = (CRoleItem*) pFloatItem->GetItem()->m_pItemData;
		pRoleItem->MoveBackOldLocation();
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
//Get float item name
const char* GetFloatItemName()
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	if ( pFloatItem = GetGuiManager()->GetFloatItem() )
	{
		CRoleItem* pRoleItem = (CRoleItem*) pFloatItem->GetItem()->m_pItemData;
		return pRoleItem->GetName();
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
//Get float item this ID
DWORD GetFloatItemThisID( bool bEquipment )
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	if ( pFloatItem = GetGuiManager()->GetFloatItem() )
	{
		CRoleItem* pRoleItem = (CRoleItem*) pFloatItem->GetItem()->m_pItemData;
		if ( bEquipment )
			return pRoleItem->GetThisID();
		else
		{
			if ( pRoleItem->IsAccessNeedThisID() )
				return pRoleItem->GetThisID();
			else
				return pRoleItem->GetObjectID();
		}
	}
	return -1;

	FUNCTION_END;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param rl : 描述
 * \return 返回值的描述
 */
//get float item resource location
bool GetFloatItemResource(stResourceLocation& rl)
{
	FUNCTION_BEGIN;

	CGuiFloatItem* pFloatItem;
	if ( pFloatItem = GetGuiManager()->GetFloatItem() )
	{
		CRoleItem* pRoleItem = (CRoleItem*) pFloatItem->GetItem()->m_pItemData;
		
		rl = GetItemPicLocation( pRoleItem->GetObject() );
		return true;
	}
	return false;

	FUNCTION_END;
}


bool CRoleItem::CanUseItemNow()
{
	FUNCTION_BEGIN;

	//make sure we have base object info
	if ( NULL == m_pObjectBase )
		m_pObjectBase = ::GetObjectBase( m_object.dwObjectID );

	if( !m_pObjectBase )
		return false;
	
	DWORD dwNowTime = xtimeGetTime();
	if( dwNowTime >= (m_pObjectBase->dwIntervalTime * 1000 + m_dwLastUseTime) )
	{
		m_dwLastUseTime = dwNowTime;
		return true;
	}
	return false;

	FUNCTION_END;
}

CRoleItem* CRoleItem::GetRoleItem(const CGuiItemCell* pCell)
{
	if( pCell )
	{
		return (CRoleItem*)(pCell->m_pItemData);
	}
	return NULL;
}

CRoleItem* CRoleItem::GetFloatRoleItem()
{
	CGuiFloatItem* pFloatItem = GetGuiManager()->GetFloatItem();
	if( pFloatItem )
	{
		CGuiItemCell* pCell = pFloatItem->GetItem();
		return CRoleItem::GetRoleItem(pCell);
	}
	return NULL;
}
