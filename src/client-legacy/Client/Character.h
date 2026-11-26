#pragma once
#include "npc.h"

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */

struct stFaceInfo{
	int imgID;
};


struct stBitmapShow	// 显示控制
{
	stResourceLocation rl;
	IBitmaps * pBitmps;
	DWORD  dwStartTime;
	void Render(const POINT & pt);
	void Run();
	BYTE  alpha;
	bool IsNeedShow();
	void SetFrame(int nFrame);
	void StartShow();
	stBitmapShow();
	stRectI rcClip;
};

class CCharacter :
	public CNpc
{

#ifdef _WYC_OPT_SER_
	wyc::t_MapUserData m_data;
#else
	::t_MapUserData m_data;
#endif
	
	DWORD   m_dwStar;
	BOOL    m_bShowWarFlag;	
	int m_tiletype;

public:
	stPointI m_ptPrevGrid;
	virtual void OnGridPosChange(POINT prevGrid,POINT ptCurGrid);

	CCharacter(void);
	~CCharacter(void);
	bool IsCanAttack();
	bool IsMyFriend();
	void AfterSetAttrib(bool bUpdateImage,bool bUpdateState);
	bool SetAttrib(::t_MapUserData* pCmd);
	bool ResetImage(); 
	bool ResetState();
	enumSex GetSex() { return m_data.type == PROFESSION_1 ? eSex_Male : eSex_Female; }
	int  GetFace(){ return m_data.face;}
	void Release();
	void Run(long time_elasped);
	void Draw(POINT ptMapPixelOffset,DWORD dwColor);

	bool CanAttack();
	//bool HandleCommand();
	size_t GetType(){ return m_data.type;}
	size_t GetHairType() { return m_data.sculpt.dwHairID;}
	
	const char* GetTypeName();
	char  m_countryName[MAX_NAMESIZE];
	const char* GetCountryName();
	const char* GetUnionName() { return m_data.unionName;}
	const char* GetSeptName() { return m_data.septName;}
	const char* GetGraceName(); 
	const char* GetOfficeName();
	
	void SetBodyCustomColor(DWORD dwColor);
	void SetBodySystemColor(DWORD dwColor);
	DWORD GetBodySystemColor();
	DWORD GetBodyCustomColor() { return m_data.dwBodyColorCustom | 0xff000000;}
	DWORD GetHairColor() { return m_data.dwHairRGB | 0xff000000;}
	DWORD GetLeftWeaponColor();
	DWORD GetRightWeaponColor();
	DWORD GetGoodness() { return m_data.goodness; }
	void  SetGoodness(DWORD dwGoodness);
	DWORD GetGoodnessAfterAdjust();
	enumFiveType GetMainFive(){ return enumFiveType(0);}//if( m_data.five >= FiveType_Num ) return enumFiveType(0); return (enumFiveType)m_data.five;}
	DWORD GetCountry() { return m_data.country; }

	int ServerSkillToClientSkill(SkillBase_t* pSkill);
	t_MapUserSculpt* GetSculpt(){ return &m_data.sculpt; }
	int GetRideID();

	virtual BYTE* GetState(){
		return m_data.state;
	}

	int GetAniSpeed() { return m_data.movespeed; }
	int GetAttackSpeedServer() { return m_data.attackspeed; }

	int GetHitSound();
	int GetAttackSound();

	DWORD GetLeftWeaponID(){ return m_data.sculpt.dwLeftHandID;}
	DWORD GetRightWeaponID(){ return m_data.sculpt.dwRightHandID;}
	enumItemType GetRightWeaponType();
	enumItemType GetLeftWeaponType();
	void OnEquipChanged();
	bool IsEquipedPickResourceTool();

	void DrawName(POINT ptMapPixelOffset,DWORD dwColor = 0xffffffff);
	bool HandleCommand(stNullUserCmd* pCmd);
	void AddLight(POINT ptMapOffset);
	bool OnSetCursor();

#ifdef _WYC_OPT_SER_
	wyc::t_MapUserData* GetProData() { return &m_data; }
#else
	::t_MapUserData* GetProData() { return &m_data; }
#endif
	
	ObjectBase_t*	GetBodyObjectBase() { return GetObjectBase(m_data.sculpt.dwBodyID);}
	size_t GetLevel();

	stBitmapShow m_BitmapShow;
	void DrawEmotionIcon(stPointI ptMapPixelOffset);
	void DrawCountryWarFlag(stPointI ptMapOffset);

	virtual bool bIsShopStart();
	bool IsUnKnowPerson();


	bool IsKing(); // 是否国王
	bool IsEmperor();	// 是否皇帝
	bool IsGen();  // 是否将军
	bool IsCityLeader(); // 是否城主
	bool IsArmyer(); //是否军人
	bool IsSeptLeader();	//是否族长
	bool IsUnionLeader();	//是否帮主
	bool IsDragonTigerState();

	DWORD   GetTeamState(); //得到队伍状态信息
	void SetCountryStar(DWORD dwNewStar){
		m_dwStar = dwNewStar;
	}
	DWORD GetCountryStar(){
		return m_dwStar;
	}
	void SetShowWarFlag(BOOL bShow){
		m_bShowWarFlag = bShow;
	}
	//DWORD   GetTeamState(); //得到队伍状态信息	
};


extern  DWORD GetNameColor(DWORD dwGoodNess);