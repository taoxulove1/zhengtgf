/**
*\file		GuiNpcMake.h
*\version	$Id$
*\author	$$$01公司
*\			$$$01公司@###.com
*\date		2005-7-5   17:12:00 中国标准时间
*\brief	    打造、合成、升级、魂魄合成、镶嵌、打孔、食物烹饪对话框
*
* 
*
*/

#pragma once
#include "..\gui\include\guidialog.h"
#include "./RoleItem.h"

#define   ClewLabel          1
#define   NeedLabel          2
#define   MakeButton         3
#define   EnableMakeButton    4
#define   MaterialListBox     5
#define   PropListBox        7
#define   MakeProcess        10
#define   ChangeNum          20
#define   ProcessFinish      10000


enum MakeType{
	Body   =  1,
   Helm       ,         
   Attack     ,        
   Shield     ,        
   Caestus    ,        
   Cuff       ,        
   Shoes      ,        
   Necklace   ,        
   Fing      ,         
   Resource   ,        
   Stone     ,        
   Food1     ,          
   Food2     ,          
   Food3     ,          
   Food4     ,          
   Leechdom  ,
   Package   ,
   Paper   ,
   HorseEquip	,
};

#define  EXPFORMAKE          5

#define   ADDBUTTON          1000
#define   LESSBUTTON         2000


#define   HCCL              0        //合成配置

#define   SJCL              100      //升级配置
#define   SJJL              101
#define   SJGL              110      

#define   DZCL              203
#define   DZANGLE           200       //打造配置
#define   DZSPEED           201       //
#define   DZGL              210

#define   LHHCCL            300       //灵魂合成配置

#define   LHXQCL            400       //灵魂镶嵌配置
#define   LHXQJG            401       //灵魂镶嵌价格

#define   DKCL              500       //打孔配置
#define   DKJG              501       //打孔价格




#define   SOCKETNUM         6
//#define   MAXUPLEVEL        9
#define   CHANGENUM         5
#define   MAKESTEP          5
#define   MAKESTOPTIME      5.0f

#define   UPOBJCET          681        //升级宝石
#define   UPOBJCETBETTER     795        //精致升级宝石
#define   RATEOBJECT        683        //天佑石

#define   RATESTONE         677        //凝魂石
#define   MAKESTONE         678        //定魂石
#define   HOLESTONE         679        //天魔石
#define   KASTONE           680       //魂魄石
#define   KASTONE1          748       //魂魄石
#define   KASTONE2          749       //魂魄石
#define   KASTONE3          750       //魂魄石
#define   KASTONE4          751       //魂魄石
#define   KAMAKESTONE        794       //灵魂宝石

#define   ARMRATEOBJECT      731       //陨铁
#define   BODYRATEOBJECT     730       //玉霞丝
#define   PULLONRATEOBJECT   732       //水灵木
#define   WEARRATEOBJECT     733       //地灵石

#define   CHANGEITEM10       752       //木角鼎
#define   CHANGEITEM20       753       //铜角鼎
#define   CHANGEITEM50       754       //银角鼎
#define   CHANGEITEM100      755       //金角鼎


#define   UPADD            10

#define   NAMEFORMAT        "%s+%d"
#define   NAMEFORMAT2       "%s %s"
#define   REDCOLOR          D3DCOLOR_ARGB( 255, 255, 0, 0 )
#define   WHITECOLOR        D3DCOLOR_ARGB( 255, 255, 255, 255 )
#define   YELLOWCOLOR       D3DCOLOR_ARGB(255,255,209,0)
#define   GREENCOLOR        D3DCOLOR_ARGB(255,0,255,0)
#define   ISARM(type)       (type==ItemType_Blade)||(type==ItemType_Sword)||(type==ItemType_Axe)||(type==ItemType_Hammer) \
								||(type==ItemType_Staff)||(type==ItemType_Bow)||(type==ItemType_Fan)||(type==ItemType_Stick)||(type==ItemType_Shield)
#define   ISBODY(type)      (type==ItemType_ClothBody)||(type==ItemType_FellBody)||(type==ItemType_MetalBody)
#define   ISPULLON(type)    (type==ItemType_Shoes)||(type==ItemType_Helm)||(type==ItemType_Shield)
#define   ISWEAR(type)      (type==ItemType_Caestus)||(type==ItemType_Cuff)||(type==ItemType_Necklace)||(type==ItemType_Fing)

#define   SUCCESSSOUND      "sound\\item\\05d.wav"
#define   FAILEDSOUND       "sound\\item\\05d.wav"

#define   NEEDMONEY         "%d"


#define   CHANGEDEFCLEW        "提示信息：\n  单次使用材料的最大数量为50"
#define   CHANGEDEFNEED        "需求信息："
#define   CHANGEPRICENEED      "\n  %s合成的基础价格为: %d"
#define   CHANGEAMTNEED        "\n  该次合成需要的银子为: %d"

#define   UPDEFCLEW        "提示信息：\n  升级物品必须使用对应等级的圣灵石，同时可以加上天佑石增加升级成功率"
#define   UPDEFNEED        "需求信息："
#define   UPNEED           "\n  需要银子: %d\n  升级该物品需要 [圣灵石%s]"
#define   UPRATE           "该次升级物品的成功率: %d%%"

#define   MAKEDEFCLEW        "提示信息：\n  原料的好坏直接影响打造物品好坏"
#define   MAKEDEFNEED        "需求信息："


#define   KAMAKEDEFCLEW        "提示信息：\n  请根据需求信息进行镶嵌操作"
#define   KAMAKEDEFNEED        "需求信息："
#define   KAMAKEOBJECT         "\n  %s  有凹槽（%d） 可镶嵌（%d）"
#define   KAMAKENEED           "\n  需要银子: %d\n  升级该物品需要 [定魂石%s]\n  需要任意等级魂魄一个"

#define   KACHANGEDEFCLEW        "提示信息：\n  两个魂魄石和一个相应等级凝魂石可以合成一个高级魂魄石"
#define   KACHANGEDEFNEED        "需求信息："
#define   KACHANGENEED           "\n  需要银子: %d\n  需要 [凝魂石%s] 一个，最多为两个"
#define   KACHANGERATE           "\n  该次合成的成功率: %d%%"


#define   HOLEMAKEDEFCLEW        "提示信息：\n  给物品增加第几个孔就需要几等级的天魔石"
#define   HOLEMAKEDEFNEED        "需求信息："
#define   HOLEMAKENEED        "\n  需要银子: %d\n  给此物品增加凹槽需要 [天魔石%s] 一个"

#define   SHOWENABLEMAKE        "显示可造"
#define   SHOWALLMAKE           "显示全部"

struct stPropInfo {
	void*            pObject;
	DWORD            ObjectID;
	WORD             nLevel;  
	WORD             nCanSocket;
	WORD             nUseSocket;
	DWORD            socket[SOCKETNUM]; //孔
	char             name[MAX_NAMESIZE];
	bool             bArm;
	bool             bObject;
	DWORD            type;
	bool             bAddIn;
	stPropInfo(){
		pObject = NULL;
		bObject = false;
		ObjectID = 0;
		nLevel =0;
		nCanSocket =0;
		nUseSocket =0;
		memset(socket,0,sizeof(socket));
		name[0]=0;
		bArm=false;
		bAddIn=false;
	}
	
	ObjectBase_t* GetObjectBase()
	{
		if (bObject)
			return (ObjectBase_t*)pObject;

		return  ((CRoleItem*)pObject)->GetObjectBase();
	}
};   //道具信息

struct stMaterialInfo {
	void*            pObject;
	DWORD            ObjectID;
	char             name[MAX_NAMESIZE];
	WORD             nLevel;  
	byte             bAdd;
	int              nLeaveNum;
	int              nUseNum;
	int              nStepNum;
	int              nMaxUseNum;
	int              nNeedNum;
	DWORD            type;
	WORD             nNeedLevel;
	stMaterialInfo(){
		pObject = NULL;
		ObjectID = 0;
		name[0]=0;
		bAdd =0;
		nLevel =0;
		nLeaveNum=0;
		nUseNum=0;
		nStepNum=0;
		nMaxUseNum=0;
		nNeedNum=0;
		nNeedLevel=0;
	}

	ObjectBase_t* GetObjectBase()
	{
		if (bAdd!=0)
			return  (ObjectBase_t*)pObject;
		else
			return  ((CRoleItem*)pObject)->GetObjectBase(); 
	}

	DWORD GetObjectBaseID()
	{
		if (!pObject)
			return 0;

		if (bAdd!=0)
			return  ObjectID;
		else
			return  ((CRoleItem*)pObject)->GetObjectBase()->dwID;
	}
};   //材料信息

struct stBtnGroup {
	int           index;
	CGuiButton*    pButtonAdd;
	CGuiButton*    pButtonLess;
};   //材料对应的按纽组信息


enum enumDataType {
	eMaterial,        //材料
	eRate,            //几率
	ePrice,
	eRateItem,        //几率材料
};

enum enumGuessSpeed {
	eSlow = 1 ,
	eMidd = 2 ,
	eFast = 3 ,
};

struct stMakeDataMainItem {
	byte         state;
	enumDataType  type;
	WORD         MainItem;
};

struct stSpeed {
	float fTime;
	int   angle;
};


typedef std::map<DWORD,std::string >   STRINGMAP;
typedef std::vector<stMaterialInfo*>   SELECTINFO; 
typedef std::vector<stSpeed>          SPEEDINFO;


/**
 * \brief 打造转盘对话框
 * 
 * 打造转盘对话框
 * 
 */
class CMakeGuess :
	public CGuiDialog
{
	///当前的速度
	enumGuessSpeed  m_eState;
	///打造结果
	WORD          m_MakeResult;
	///转盘的控制结构
	SPEEDINFO      m_playCtrl;
	///转的角度
	float         m_fAngle;
	///转的结束角度
	float         m_fEnd;
	///转的停止度数
	stPointI       m_ptAngle;
	///得到转盘速度的配置文件类型
	CIni          m_Ini;

	void          SetPlayCtrl();
	///转盘的相对位置
	stPointI       m_ptRender;
	
public:
	///是否开始转
	bool          m_bStart;
	///当前转的时间
	float         m_fRunTime;

public:
	CMakeGuess(stPointI pt);
	
	HRESULT OnRender(float fElapsedTime);
	void   RenderBackground(float fElapsedTime);

	void InitData();

	void SetSpeed(enumGuessSpeed state){ m_eState = state; SetPlayCtrl(); }
	enumGuessSpeed GetSpeed(){ return m_eState; }

	void SetResult(WORD result){ m_MakeResult = result; }
	WORD GetResult(){ return m_MakeResult; }

	void SetEndAngle(DWORD value);
	void SetRenderPt(stPointI pt){ m_ptRender = pt;}

	
};

/**
 * \brief 打造数据结构类
 * 
 * 打造数据结构类
 * 
 */
class CMakeData
{
	///从配置文件中读取数据
	std::vector<UINT, std::stack_allocator<UINT> > m_Info;
	///配置文件的材料数据
	STRINGMAP                      m_MaterialInfo;
	///当前选中的材料数据
	SELECTINFO                     m_SelectInfo;
	///读取打造数据
	CIni          m_Ini;

	void InitData();
	void LoadData();
	int level_of_material(DWORD id, DWORD num, WORD level,WORD needlevel,DWORD obid1,DWORD obid2,WORD level1,WORD level2);

	
	struct Odds
	{
		int per;
		int luck;
		double material_level;
		int skill_level;
		int odds;
		int odds_gem;
	};
	///打造的几率数据
	double sum_current;
	///打造的几率数据
	double sum_base;
	///打造的材料等级
	double material_level;
	///打造的几率
	Odds odds;
	

public:
	CMakeData();
	~CMakeData();
	void clearMaterial() { m_MaterialInfo.clear(); }
	void insertMaterial(DWORD dwID,WORD nSetp,WORD nMax,byte bAdd);

	void clearSelectInfo() { m_SelectInfo.clear(); }
	void insertSelectData(stMaterialInfo* p);
	void deleteSelectData(stMaterialInfo* p);
	SELECTINFO& getSelectData(){ return m_SelectInfo;}
	stMaterialInfo* findSelectDataByObjectID(DWORD dwID);
	int findSelectDataByObjectNum(DWORD dwID,int nLevel=-1);
	int getSelectDataByObjectMaxLevel(DWORD dwID);
	bool findSelectDataByData(stMaterialInfo* pData);
	DWORD  getMakeRateObject(int type);
	int    findKaStoneNum();
	stMaterialInfo*  findKaStoneSelectData();
	CIni*  GetIni(){ return &m_Ini;};
	

	bool   IsUpItem(stPropInfo* pData);
	bool   IsHoleMake(stPropInfo* pData);
	bool   IsChangeItem(stPropInfo* pData,int nAdd);
	bool   IsMakeItem(char* strItemNeed,DWORD dwLevel);
	bool   IsKaMake(stPropInfo* pData);
	bool   IsKaChange(stPropInfo* pData);
	bool   IsCanSkill(char* strSkillNeed,char** szSkillShow);
	bool   CheckMoney(DWORD nMoney);
	void   GetNameAndLevel(char* pDest,const char* pName,int nLevel,int type,byte maxlevel);
	bool   CheckCondition(stPointI condition,int n,stPointI* pPt);
	WORD   CheckKaMake(DWORD* pSocket,WORD& nUseSocket);
	bool   CheckChange(DWORD dwCurrLevel,DWORD dwMaxLevel,byte eState);
	bool   CheckItemNum(DWORD dwID,DWORD num);
	bool   CheckItemNum(DWORD dwID,DWORD num,byte nLevel);
	bool   CheckMakeItemNum(DWORD dwID,DWORD num,byte nLevel);
	int    GetPt_y(int x,int n,stPointI* pPt);
	int    GetBackoutMoney(CRoleItem* pRoleItem);

	void InitOdds()
	{
		sum_current=0;
		sum_base=0;
		material_level=0;
		memset(&odds,0,sizeof(Odds));
	}
	void SetOdds(t_MainUserData* pUserData,int nSkillLevel,int hole);
	int odds_of_white(const ObjectBase_t* ob);
	int odds_of_blue(const ObjectBase_t* ob);
	int odds_of_gold(const ObjectBase_t* ob);
	int odds_of_holy(int object);
	int odds_of_property(int object, int property);


	int  GetMainItem(byte state,enumDataType type);
	std::vector<UINT, std::stack_allocator<UINT> >& GetInfo(int nMainItem,DWORD nSubItem,char* szDefault);
};

/**
 * \brief 打造,合成,升级等对话框类
 * 
 * 打造,合成,升级等对话框类
 * 
 */
class CGuiNpcMake :
	public CGuiDialog
{
	///打造数据类
	CMakeData      m_MakeData;
	///打造类型(合成,升级,打造等)
	byte          m_eState;
	///是否显示满足打造条件
	bool          m_bEnableMake;
	///材料列表框
	CGuiListBox*   m_pMaterial;
	///打造物品列表框
	CGuiListBox*   m_pProp;
	///打造按钮
	CGuiButton*    m_pMake;
	///打造提示
	CGuiStatic*    m_pClew;
	///打造银子需求
	CGuiStatic*    m_pNeed;
	///打造进度条
	CGuiProcess*   m_pProcess;
	///打造转盘
	CMakeGuess*    m_pMakeGuess;
	///是否满足打造条件
	bool           m_bPass;
	///是否有足够银两
	bool		   m_bMoneyOk;
	///进度条开始
	bool           m_bProcess;
	///帮助信息
	CToolTips      m_ToolTips;
	///物品的基本资料
	t_Object       m_Object;
	///银子显示的颜色
	DWORD          m_MoneyColor;
	///合成的物品ID
	DWORD          m_dwChangeID;

	bool           m_bShowExpTitle;

	///材料物品的基本结构
	std::vector<ObjectBase_t*>    m_aMakeObject;
	///要打造物品的基本结构
	std::vector<ObjectBase_t*>    m_aMakeToObject;
	///材料打造结构
	std::vector<stMaterialInfo>   m_aMaterialInfoList;
	///到打造物品的结构
	std::vector<stPropInfo>       m_aPropInfoList;
	///材料物品的按钮
	std::vector<stBtnGroup>       m_aBtnGroupList;
	///服务器脚本数据保存
	std::vector<std::string>      m_params;

	CAnimation     m_aniMake;
	
	void CreateButtons();
	void RefreshGui();
	void RefreshMaterialData(int n=0,stPointI* pPt=NULL);
	void RefreshPropData(bool bGroup = false);
	void ResetButtonState(int index);
	void MaterialBtnClick(UINT nID);
	void ResetListItem(int index);
	bool SendCmd();
	void RefreshPropDataByGroup();
	void RefreshPropDataByNormal();
	void SelectDefProp();
	void InitMaterial();
	void InitContext();
	bool CheckConditionByKaChange(stPointI condition,int n,stPointI* pPt);
	void RefreshData();
	int  GetMaterialData(DWORD id,int nLevel = -1);
	bool IsRateItem(DWORD id);
	void CanMake();
	int  GetChangeNumBySingle();

	void UpdateInfo();
	void UpdateGui();

	void ShowMoneyLackInfo();

	void UpdateChangeInfo();
	void UpdateChangeGui();

	void UpdateUpInfo();
	void UpdateUpGui();

	void UpdateMakeInfo();
	void UpdateMakeGui();

	void UpdateKaMakeInfo();
	void UpdateKaMakeGui();

	void UpdateKaChangeInfo();
	void UpdateKaChangeGui();

	void UpdateHoleMakeInfo();
	void UpdateHoleMakeGui();

	void UpdateBackoutInfo();
	void UpdateBackoutGui();

	void UpdateMakeFoodInfo();
	void UpdateMakeFoodGui();

public:
	CGuiNpcMake(byte eState);

	void OnClose(void);
	void OnCreate();
	HRESULT OnRender(float fElapsedTime);
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	void OnSetCloseLocation();
	void InitCustomElements();

	void SetMakeParam(std::vector<std::string> & params,int n = 0,stPointI* pPt = 0);
	void SetMakeToParam(std::vector<std::string> & params);

	void LoadData(std::vector<std::string> & params,bool bReLoad=false,DWORD dwChangeID=0);
	void Response(stResponsePropertyUserCmd* pCmd);
	void SendGuessFinish();
	void DisMake(stFoundItemPropertyUserCmd* pCmd);
	
};

class CGuiMakeTo :
	public CGuiDialog
{
	CRoleItem*     m_pRoleItem;
	///打造数据类
	CMakeData      m_MakeData;
	///打造类型(合成,升级等)
	byte          m_eState;
	///打造按钮
	CGuiButton*    m_pMake;
	///是否满足打造条件
	bool           m_bPass;

	CGuiTab*       m_pTab;
	
	CGuiStatic*    m_pClew;
	
	CGuiStatic*    m_pNeed;

	CGuiStatic*    m_pContext;

	DWORD          m_MoneyColor;

	CAnimation     m_aniMake;

	char           m_szSubTxt[MAX_PATH];

	byte           m_nLocationPos;            

	///材料物品的基本结构
	std::vector<ObjectBase_t*>    m_aMakeObject;
	std::vector<CRoleItem*>      m_aMateroalObject;

	
	void SetContext(char* pContext,bool bShow){
		if (bShow==false) return;
		m_pContext->SetText(pContext);
	}

   void FlashItemData();

   void InsertItemData(CGuiTable* pTable);

	int GetChangeNumBySingle();

	void  InitChange();
	bool  UpdateChange(bool bShowContext);
	
	void  InitUp();
	bool  UpdateUp(bool bShowContext);

	void  InitKaChange();
	bool  UpdateKaChange(bool bShowContext);

	void  InitKaMake();
	bool  UpdateKaMake(bool bShowContext);

	void  InitData();
	bool  SendCmd();



public:
	CGuiMakeTo(CRoleItem* pRoleItem);

	void OnClose(void);
	void OnCreate();
	void InitTableInfo(CRoleItem* pRoleItem);
	void RefreshTableInfo();
	HRESULT OnRender(float fElapsedTime);
	bool OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl);
	void OnSetCloseLocation();
	void SetMakeParam(std::vector<std::string> & params);
	bool UpdateData(bool bShowContext);
	void Response(stResponsePropertyUserCmd* pCmd);

	int  GetItemCount();



	///装备格子
	CGuiTable*    m_pEquipTable;
	///材料格子1
	CGuiTable*    m_pMateroalTable1;
	///材料格子2
	CGuiTable*    m_pMateroalTable2;
	///材料格子3
	CGuiTable*    m_pMateroalTable3;
	///材料格子4
	CGuiTable*    m_pMateroalTable4;

	DWORD         m_nPos;

};

extern void ResponseMake(stResponsePropertyUserCmd* pCmd);
