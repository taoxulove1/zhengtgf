/********************************************************************
	创建时间:	27:1:2005   15:24
	文件名: 	RoleItem.h
	文件类型:	h
	作者:		$$$03公司
	
	用途:	  角色身上的物品类
*********************************************************************/
#pragma once
#include "..\magic\objectbase.h"
#include "./command.h"
#include "./Object.h"
#include "../gui/include/ToolTips.h"

#define   ISARRAW(type) ( (type == ItemType_Arrow ) )
extern const char* c_szItemGrpPack;		// 物品图包
const int c_nGroupItemCommon = 0;
const int c_nGroupItemCommonPattern = 1;
const int c_nGroupItemCommonPattern2 = 2;
const int c_nGroupItemEquip = 3;
const int c_nGroupItemEquipPattern = 4;
const int c_nGroupItemEquipPattern2 = 5;
const int c_nGroupItemGround = 6;
const int c_nGroupItemStartOnGround = 9;
const int c_nGroupItemEndOnGround = 10;

const int c_nHuoYunFu = 672;		//火云符
const int c_nXianTianFu = 673;		//玄天符
const int c_nXuanYuanFu = 674;		//轩辕符

extern stResourceLocation GetObjectBasePicLocation(ObjectBase_t* pObjectBase);
extern stResourceLocation GetObjectBasePatternPicLocation(ObjectBase_t* pObjectBase);
extern stResourceLocation GetObjectBasePattern2PicLocation(ObjectBase_t* pObjectBase);

extern stResourceLocation GetItemPicLocation(t_Object * pObject);
extern stResourceLocation GetItemPatternPicLocation(t_Object * pObject);
extern stResourceLocation GetItemPattern2PicLocation(t_Object * pObject);
extern const char* GetFloatItemName();
extern DWORD GetFloatItemThisID(bool bEquipment);
extern void MoveBackFloatItem();
extern bool GetFloatItemResource(stResourceLocation& rl);

static const char* fiveName[] = { "风", "雨", "雷", "电", "火", "" };
static const DWORD fiveColor[] = { 
		COLOR_ARGB(255,171,160,0),
		COLOR_ARGB(255,88,133,38),
		COLOR_ARGB(255,166,124,81),
		COLOR_ARGB(255,0,174,239),
		COLOR_ARGB(255,255,58,43),
		COLOR_ARGB(255,255,255,255), };

enum enmDayHourMinuteType{
	TYPE_DAY,
	TYPE_DAY_HOUR,
	TYPE_DAY_HOUR_MINUTE,
};

extern const int c_maxItemUpgrade;
static const char* upgradeName[] = { " 一等", " 二等", " 三等", " 四等", " 五等",
" 六等", " 七等", " 八等", " 九等", " 十等", "十一等", "十二等", "十三等", "十四等", "十五等" };

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
class CGuiTable;
class CRoleItem;

extern CGuiTable* GetItemTable(const stObjectLocation & pos);
extern bool EquipItem(CRoleItem* pSrcItem);
bool ItemTableEvent(CGuiTable* pTable,UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);

extern stPointI g_ptMoneyPos;

class CGuiItemCell;
struct stSplitItemData{
	QWORD qwThisID;
	DWORD dwCurNum;
	CGuiItemCell* pSrcCell;
	stObjectLocation dst;
};

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param iEquipType : 描述
 * \return 返回值的描述
 */
inline bool IsMoppet(DWORD iEquipType)
{
	return iEquipType == EQUIPCELLTYPE_BODY || iEquipType == EQUIPCELLTYPE_HANDL || iEquipType == EQUIPCELLTYPE_HANDR;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwLocation : 描述
 * \return 返回值的描述
 */
inline bool IsTradeCell(DWORD dwLocation)
{
	return dwLocation == OBJECTCELLTYPE_TRADE || dwLocation == OBJECTCELLTYPE_OTHERTRADE;
}

enum 
{
	eTip_ShowName	= 0x01,		//显示物品名字
	eTip_ShowPrice	= 0x02,		//显示物品价格
	eTip_ShowColor	= 0x04,		//显示物品颜色
	eTip_ShopLevel  = 0x08,		//显示物品等级
};


#define ITEMTIPS_FLAG_PRICE				0x1		// 价格
#define ITEMTIPS_FLAG_REPAIR_PRICE		0x2		// 修理价格
#define ITEMTIPS_FLAG_RIGHT_CLICK_USE	0x4		// 右键点击可以使用
#define ITEMTIPS_FLAG_REPAIR_PRICEBYGOLD		0x8		// 修理价格用金子

#define CANNOTUSE_LEVEL 0x1
#define CANNOTUSE_CHARTYPE 0x2

#define INGOT_TXT                    "锭" 
#define TAEL_TXT                     "两"
#define CENT_TXT                     "文"

#define INGOT_RATE                   10000
#define TAEL_RATE                    100

#define GETMONEYINGOT(money)	( (money) / INGOT_RATE )
#define GETMONEYTAEL(money)		( ( ( money % INGOT_RATE ) / TAEL_RATE ) )
#define GETMONEYCENT(money)		( ( ( money % INGOT_RATE ) % TAEL_RATE ) )

enum
{
	ITEM_PRICE_COMMON	= 0,
	ITEM_PRICE_SELL		= 1,
	ITEM_PRICE_SHOP		= 2,
	ITEM_PRICE_BUY		= -1
	
};

class CGuiItemCell;
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
class CRoleItem :
	public CObjectBase
{
	t_Object		m_object;
	
	struct stCellInfo{
		CGuiItemCell* pCell;
		int count;
	};

	typedef std::vector<stCellInfo> tCells;
	tCells	m_cells;

	ObjectBase_t*	m_pObjectBase;
	
	CToolTips m_ToolTips;
	DWORD	  m_dwTipFlags;
	bool	  m_bNeedUpdateTips;

	bool		m_bIsImage;		//是否是镜像
	CRoleItem*	m_pImageItem;	//镜像的物品
	CRoleItem*	m_pMasterItem;	//如果是镜像，该指针指向物品主体

	DWORD     m_dwLastUseTime;

	int			m_price;
public:
	
	enum {
		UPDATE_FLAG_LOCATION	= 0x1,
		UPDATE_FLAG_IMAGE		= 0x2,
		UPDATE_FLAG_COUNT		= 0x4,
		UPDATE_FLAG_HOLE		= 0x5,
		UPDATE_FLAG_MAKECOUNT   = 0x8,
	};

	void Copy(CRoleItem* pItem);
	void CopyTo( const stObjectLocation & dst );	//在dst位置放一个该物品的镜像
	void ClearItemImage();
	CRoleItem* GetRoleItemImage() { return m_pImageItem; }
	CRoleItem* GetImageMasterItem() { return m_pMasterItem;}
	void SetShopPrice(int price) { m_price = price; }

	static bool	s_bCanMoveItem;
	static tItemThisID s_dwMaxThisID;

	CRoleItem(void);
	~CRoleItem(void);

	void AccessKeyOnRemove(const DWORD newItemID = -1);

	CToolTips* GetToolTips(){ return &m_ToolTips;}

	int GetCellCount() { return m_cells.size(); }
	
	void SetTrueCell(CGuiItemCell* pCell,int count)
	{
		tCells::iterator it = FindCell(pCell);
		if(it == m_cells.end())
		{
			stCellInfo info;
			info.count = count;
			info.pCell = pCell;
			m_cells.push_back(info);
			if(m_cells.size() > 1)
			{
				m_cells.back() = m_cells.front();
				m_cells.front() = info;
			}
		}
		else
		{
			(*it).count = count;
			if(it != m_cells.begin())
			{
				stCellInfo info = (*it);
				(*it) = m_cells.front();
				m_cells.front() = info;
			}
		}
	}

	bool SetTrueCellCount(int count);

	bool GetTrueCell(CGuiItemCell* & pCell,int & count)
	{
		if(m_cells.empty()) return false;
		pCell = m_cells.front().pCell;
		count = m_cells.front().count;
		return true;
	}

	int GetItemCountInCell(CGuiItemCell* pCell)
	{
		tCells::iterator it = FindCell(pCell);
		if(it != m_cells.end())
			return (*it).count;
		return 0;
	}

	tCells::iterator FindCell(CGuiItemCell* pCell)
	{
		for(tCells::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
		{
			if((*it).pCell == pCell)
				return it;
		}
		return m_cells.end();
	}

	void OnAddCell(CGuiItemCell* pCell,int count)
	{
		tCells::iterator it = FindCell(pCell);
		if(it == m_cells.end())
		{
			stCellInfo info;
			info.count = count;
			info.pCell = pCell;
			m_cells.push_back(info);
		}
		else
		{
			(*it).count = count;
		}
	}

	void OnRemoveCell(CGuiItemCell* pCell);
	void DestoryAllVirtualCell();
	void DestoryAllCell();

	DWORD GetTipFlags(){ return m_dwTipFlags;}
	void SetTipFlags(DWORD dwFlags);
	void UpdateToolTips();
	void ShowToolTips(POINT pt);
	void ShowToolTipsTopLeftPrefer( stRectI rcBBox );
	bool IsBind(){ return m_object.bind==0?false:true; }
	bool IsNeedConfirmBeforeSell();

	void UpdateExp( DWORD exp ) { m_object.exp = exp; UpdateToolTips(); }
	
	t_Object* GetObject()
	{
		return &m_object;
	}

	t_Object GetTObject()
	{
		return m_object;
	}

	ObjectBase_t* GetObjectBase()
	{
		return m_pObjectBase;
	}
	bool SetObject(t_Object* pObj);
	bool SetAttrib(stAddObjectPropertyUserCmd* pCmd);
	void OnRemove();
	void OnSwap(stSwapObjectPropertyUserCmd* pCmd);
	void OnSplit(stSplitObjectPropertyUserCmd* pCmd);
	void OnSetCount(stRefCountObjectPropertyUserCmd* pCmd);
	void OnUpdate(DWORD dwFlags);
	void SetImage();
	void SetHoleImage();
	void ShowHoleImage( bool bHide = false );
	
	void MoveTo(const stObjectLocation & dst);

	void SplitTo(const stObjectLocation & dst,DWORD num);
	void UnionTo(CRoleItem* pDestItem);

	DWORD  GetMaxCount();
	DWORD  GetCount() { return m_object.dwNum;}
	DWORD  GetExteriorCount();

	WORD GetItemDur() { return m_object.dur; }
	
	bool IsFull() { return GetCount() >= GetMaxCount();}

	tItemThisID GetThisID(){ return m_object.qwThisID; }
	DWORD GetObjectID(){ return m_object.dwObjectID;}

	const stObjectLocation& GetLocation(){ return m_object.pos; }
	DWORD GetEquipType(){ return m_pObjectBase->dwEquipType;}
	const char* GetObjectName() { return m_object.strName; } 
	
	bool IsMoppet()
	{
		return ::IsMoppet(GetEquipType());
	}
	enumItemType GetItemType(){ return enumItemType(m_pObjectBase->dwType); }

	inline bool IsCanUse();
	
	bool IsEquipment()
	{
		return ( (GetItemType() >= ItemType_ClothBody) && (GetItemType() <= ItemType_Fing) )
				|| (GetItemType() == ItemType_Arrow)
				|| (GetItemType() == 30)
				|| (GetItemType() == 52)
				|| (GetItemType() == 53)
				|| (GetItemType() == ItemType_BMW)
				|| (GetItemType() == ItemType_Flower) ;
	}

	bool IsAccessNeedThisID()
	{
		return (GetItemType() == ItemType_FillHP) || (GetItemType() == ItemType_FillMP);
	}

	bool IsVessalType()
	{
		return (GetItemType() == ItemType_Pack)
				|| (GetItemType() == ItemType_Tonic)
				|| (GetItemType() == ItemType_Change)
				|| (GetItemType() == ItemType_PackHide) ;
	}
	bool IsClothType()
	{
		return 	(GetItemType() == ItemType_ClothBody)
			|| (GetItemType() == ItemType_FellBody)
			|| (GetItemType() == ItemType_MetalBody) ;
	}

	bool IsTradeCell() { return ::IsTradeCell(m_object.pos.dwLocation); }

	DWORD GetPrice(int flag = ITEM_PRICE_COMMON);	//see enum
	DWORD GetRepairPrice(int flag = ITEM_PRICE_COMMON);		//see enum
	void Sell();
	void Save();

	//bool IsHorse();
	bool IsArrow();

	bool NeedConsiderDur();

	bool IsMoney() { return GetLocation().x == (WORD)g_ptMoneyPos.x && GetLocation().y == (WORD)g_ptMoneyPos.y;}

	DWORD IsCanEquip();
	void OnUpdateDur(DWORD dwDur,DWORD dwMaxDur);

	//void BackupOldLocation( const stObjectLocation& oldLocation ) { return; }
	void MoveBackOldLocation();

	void OnDataUpdate()
	{
		m_bNeedUpdateTips = true;
	}

	void AddShowLocation(CGuiItemCell* pSrcCell,int count,const stObjectLocation & dst,bool bHideSrcLocation = true,bool bDisableSrcLocation = true);

	bool CanUseItemNow();

	bool IsBelongToMakerSet();

	enmCallDutyType IsCallDutyItem();


	/////////////////////////////////////////////////////
	static CRoleItem* GetRoleItem(const CGuiItemCell* pCell);
	static CRoleItem* GetFloatRoleItem();
};

class CRoleSkill;
extern void UpdateMyObjectToolTips( t_Object& m_object, ObjectBase_t* m_pObjectBase, CToolTips& m_ToolTips, DWORD m_dwTipFlags );
extern void UpdateObjectToolTips( ObjectBase_t* pObject , CToolTips& m_ToolTips ,DWORD flags,DWORD num=1);
extern void UpdateSkillUpItemToolTips( t_Object& pObject, CToolTips& m_ToolTips, DWORD m_dwTipFlags = -1 );
extern void UpdateSkillToolTips(SkillBase_t* pSkill,CToolTips & m_ToolTips,CRoleSkill* pRoleSkill);
extern DWORD GetObjectRepairMoney(t_Object*  m_pObject);
extern DWORD GetObjectRepairByGold(t_Object*  m_pObject);
extern const char* Money2String(DWORD dwMoney);
extern const char* Minute2DayHourMinute(DWORD dwMinute, enmDayHourMinuteType type);
extern DWORD String2Money(const char* strMoney);
extern stResourceLocation GetObjectImageLocation(ObjectBase_t * pObject);
extern bool EquipArrow(const int& num);
int GetStoreTableIndex(const stObjectLocation dst);

extern CGuiItemCell* GetTableCell(CGuiTable* pTable,POINT ptGrid);
extern CGuiItemCell* GetSelectCell(CGuiTable* pTable,POINT pt);
extern CRoleItem*	 GetCellItem(CGuiTable* pTable,POINT ptGrid);
extern CRoleItem*	 GetSelectItem(CGuiTable* pTable,POINT pt);
extern CRoleItem*	 GetItemByObject( const t_Object& object );
extern bool          IsTonic();
extern void     	 ActiveItem(CRoleItem* pItem, bool bMainCharacter = true);
extern bool          IsShowActiveItem(CRoleItem* pRoleItem);
extern bool			 IsBelongToMakerSet(const t_Object& object);

extern const char* ItemKind2String(BYTE kind);
