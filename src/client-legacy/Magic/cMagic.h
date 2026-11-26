#pragma once

#include "iMagic.h"
#include "cAnimationArray.h"
#include "./magic_sound.h"
#include <string>

class iAnimation;
class CSceneSound;
class iCharacter;
class iLight;


////////////////////////////////////////////////////////////////
// 调试开关
// _DEBUG_BOW    调试弓箭类魔法
////////////////////////////////////////////////////////////////

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
class cMagic : public iMagic
{
	//friend iMagic* Magic_CreateMagic(size_t typeMagic,size_t type2Magic);
public:
	cMagic(int nTable);
	virtual ~cMagic(void);
	MagicProperty m_stProperty;
	iCharacter* m_pSender;
	iCharacter* m_pDes;
	int m_nTable;
private:
	ULONG m_ulCount;										// 引用计数
	void* m_lData;

	cAnimationArray m_Animation;							//魔法图片列表
	std::vector<iMagic*> m_listFollow;                      //跟随的魔法列表
	std::list<stEvent> m_listEvent;					 		//生成的事件表

	
	CSceneSound m_Sound;
	iLight* m_pLight;
	cMagic* m_pShadow;

	int m_nSetLife;
	int m_nInitLife;

	size_t m_lType;
	size_t m_lTable;	
	bool m_bBlockGenerate;
public:
	void SetEnableShake(bool b) {}
	bool IsEnableShake() { return false;}

	int GetObjType(){ return eObjType_Magic;}

	//data
	
	void SetData(void* ldata) { m_lData = ldata;};
	void* GetData() {return m_lData;};

	void FinalRelease();

	ULONG AddRef() ;
	ULONG Release() ;

	MagicTypeEnum GetMagicType() {return m_stProperty.stMagic.eType;}; // 当前魔法类型：如 魔法结果 | 一般魔法等
	MagicAttackEnum GetAttackType() {return m_stProperty.stMagic.eAttack;};// 当前魔法攻击类型
	MagicProperty* GetProperty(){ return &m_stProperty;}// 当前魔法属性(其实就是magic.mdb对应的一条记录)
/////////////////////////////////////
	POINT GetPos(){return m_stProperty.stMove.GetPos();}
	void SetPos(POINT pt){
#ifdef _DEBUG_BOW
		if( strcmp(m_stProperty.GetName(),"两重箭施法") == 0)
			TRACE("弓箭飞行被设在: (%d,%d)\n",pt.x,pt.y);
#endif 
		m_stProperty.stMove.SetPos(pt);
	};
	POINT GetOvergroundPos();

	int GetFollowType(){return m_stProperty.stMove.eFollow; }
	bool IsFollowSender(){return GetFollowType() == (int)stMoveStruct::Move_Follow_Send;}
	int GetMoveType(){return m_stProperty.stMove.eMove;}
	//bool IsFollowType(){return GetMoveType() == stMoveStruct::Move_Follow;}

	stMoveStruct::MovePosEnum GetDesType(){return m_stProperty.stMove.eDes;}
/////////////////////////////////////
	void GetFly(POINT& ptS,POINT& ptD){m_stProperty.stMove.GetFly(ptS,ptD);};
	POINT GetStartPos(){return m_stProperty.stMove.ptStart;}
	POINT GetDes(){return m_stProperty.stMove.ptDes;}

	void Fly(const stFly& fly,int iStartSpeed = -1);
	
	iCharacter* GetSender(){return m_pSender;};
	void SetSender(iCharacter* pSender);
	iCharacter* GetTarget(){return m_pDes;}
	bool CanAttackTarget();


/////////////////////////////////////
	bool IsBreak(stBlockStruct::BlockTypeEnum e);
	int OnBreak(stBlockStruct::BlockTypeEnum n,tagListMagicGenerate& listGenerate,iCharacter* pCharacter);
/////////////////////////////////////
	bool IsDie();
	int GetLife(){return m_stProperty.stMove.GetLife();}
	void SetLife(int f){
		m_nSetLife = f; 
		m_stProperty.stMove.SetLifeOut(f);
	}
	void SetInitLife();
	
/////////////////////////////////////

	
	void SetType(size_t lType) {m_lType = lType;};  // m_lType 魔法类型
	size_t GetType() {return m_lType;};

	void SetTable(size_t lTable) {m_lTable = lTable;};// m_lTable magic.mdb下的表,如0000 0001 
	size_t GetTable() {return m_lTable;};

	size_t GetMaxDir(){ return m_stProperty.stAnimation.nDir ;}
	stAnimationStruct& GetAnimationStruct(){return m_stProperty.stAnimation;}
	stCharacterStruct& GetCharacterStruct(){return m_stProperty.stCharacter;}

#ifdef _GAME_TOOL
	size_t GetDirFrame();
	size_t GetFrameOfDir();
#endif 


/////////////////////////////////////
	void SetParam(const MagicProperty& param){m_stProperty = param;}; // MagicProperty 代表magic.mdb中的一条记录

/////////////////////////////////////
	virtual bool CreateMagic(bool bLight,LPCSTR szFile);
/////////////////////////////////////
	
	virtual bool Draw(POINT ptMapOffset) ;
	//已经作了偏移的
	virtual bool DrawAtScreen(POINT ptScreen) ;
	virtual bool DrawAtScreenA(POINT ptScreen) ;

	//return life
	
	void AddFollow(iMagic* p) 
	{
		if(m_listFollow.capacity() == 0) m_listFollow.reserve(64);
		m_listFollow.push_back(p);
	}
	virtual int Run(long time_eslasped); 
	virtual bool Generate(tagListMagicGenerate& listGenerate); 
	float GetSpeed(){return m_stProperty.stMove.GetSpeed();}
	void SetSpeedAndTurn(float fx,float fy);
	void GetSpeed(float& fx,float& fy) {m_stProperty.stMove.GetSpeed(fx,fy);};

	
	bool AddLight(POINT ptMapOffset);
	bool AddLightAtScreen(POINT ptScreen);
	iLight* GetLight(){return m_pLight;}

	bool IsBreak(iCharacter* p);

	POINT m_ptCenter;
	void SetCenter(POINT pt){m_ptCenter = pt;}
	POINT GetCenter();

//	bool m_bLight;
//	void EnableLight(bool bLight){m_bLight = bLight;};

	//shadow的运动和本身一模一样
	
	void CreateShadow();

	void RemoveSubMagic();
	void SetShadowPos();

	POINT GetDrawPosition();
	size_t GetDir8();
	size_t GetDir16();
	size_t GetDir();
	void TurnTo(size_t dir);

	stMoveStruct::SortEnum GetSort(){return m_stProperty.stMove.GetSort();};

	bool IsBlockGenerate() {return m_bBlockGenerate;};
	void SetBlockGenerate(bool b){m_bBlockGenerate = b;};

	//POINT m_ptMagicAdjust;
	void OnEnd();

	//残象
	void StartKeepImage(const stKeepImageControl& st){m_Animation.StartKeepImage(st);};
	void Fade(const stFade& st) {m_Animation.Fade(st);};

	bool DrawSort(POINT ptScreen, POINT ptMagic,stMoveStruct::SortEnum e, int dir);

	std::list<stEvent>& GetEventList(){return m_listEvent;}
	void ClearEventList(){m_listEvent.clear();}
	bool OnEvent(const stEvent& st);

	//character始终在地上！
	POINT GetSortPos();
	POINT GetDefaultCenter();
	void SetAlphaRadius(float a,float r){m_stProperty.stMove.SetAlphaRadius(a,r);};

	HRESULT InvalidateDeviceObjects() ;	//before call reset alt+tab 
	HRESULT RestoreDeviceObjects() ;		//after alt+tab

	bool GetDrawRectA(POINT pt,RECT &rc);
	bool GetDrawRect(POINT p,RECT &rc);
////一些检测判断
	bool IsHasChild();
	bool IsHasFollow();
};

iMagic* GenerateAMagic(cMagic* pFather,std::string strName,int iStartSpeed = -1);
iMagic* GenerateAMagicByPos(cMagic* pFather,std::string strName,POINT ptStart,POINT ptDes,iCharacter* pDesCharacter = NULL,int iStartSpeed = -1);
iMagic* GenerateAMagicDesOff(cMagic* pFather,std::string strName,POINT ptDesOffset,int iStartSpeed = -1);

