#include "../engine/include/engine.h"
#include "imagic.h"
#include "ccharacter.h"
#include "iMagic.h"
#include "iLight.h"
#include "iAnimation.h"
#include <fstream>
#include "math.h"
#include "magic_sound.h"
#include "cAnimationArray.h"
#include "footprintmanager.h"
#include "../Engine/include/lookaside_alloc.h"
#include "./MapScene.h"

#ifdef _DEBUG
#include "../magic/cMagic.h"
#endif


#ifdef _DEBUG
static bool s_bDeleteCharacter = false;
static bool s_bNewCharacter = false;
#endif
//static std::lookaside_allocator<cCharacter> s_allocCharacter;
using namespace std;

int g_nCharMagicTableID = 0;
const int c_nWaitTime = 1000;
const int c_nWaitBaseTime = 960;

stCharacterSortStruct* GetCharacterOrder(int type);

int cCharacter::s_nGridX = 64/2;
int cCharacter::s_nGridY = 32/2;

//static bool s_bRideSound =false;
//////////////////////////////////////////////////////////////////////////////////////////////////
// 调试开关
// _DEBUG_RIDE   人物骑马: 打开此开关可以方便定位到底是美工图错还是编辑器配置错,而后才是程序错
// _DEBUG_MAGIC  人物生成魔法 
//#define _DEBUG_MAGIC
//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
stAni::~stAni()
{
}

enum enumFootprintType{
	eFootprintLeft = 0,
	eFootprintRight = 1,
	eFootprintStand = 2,
};

enum enumMoveType{
	eMoveWalk = 0,
	eMoveRun = 1,
};

int c_charWalkFootprint[][2] = {{0,eFootprintLeft},{8,eFootprintRight}};

// 人物的动作优先级表
/*
Ani_Null,
Ani_Bow,
Ani_Magic,
Ani_Attack,
Ani_Attack2,
Ani_Back,
Ani_Die, 
Ani_Defend,
Ani_Hurt, 
Ani_Pick_Stand,
Ani_Mining,
Ani_Pick,
Ani_Run,
Ani_AStand, 
Ani_Wait,Ani_Stand = Ani_Wait,
Ani_Walk,
Ani_Ride_Wait,
Ani_Ride_Walk,
Ani_Ride_Run,
Ani_Ride_Die,
Ani_Ride_Hurt,
Ani_Hand,	
Ani_Appear,	

Ani_Idle,
Ani_Attack_Stand,
Ani_Attack2_Stand,
Ani_Magic_Stand,
Ani_Bow_Stand,
Ani_Ride_Idle,
Ani_Num,
//*/

#include <strstream>

//{
//	//Ani_Null,Ani_Bow,Ani_Magic,Ani_Attack,Ani_Attack2,Ani_Back,Ani_Die,Ani_Defend,Ani_Hurt,Ani_Pick_Stand,Ani_Mining,Ani_Pick,Ani_Run,Ani_AStand,Ani_Wait,Ani_Walk,Ani_Ride_Wait,Ani_Ride_Walk,Ani_Ride_Run,Ani_Ride_Die,Ani_Ride_Hurt,Ani_Hand,Ani_Appear,Ani_Idle,Ani_Attack_Stand,Ani_Attack2_Stand,Ani_Magic_Stand,Ani_Bow_Stand,Ani_Ride_Idle
//	//{0,},
//}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param frame : 描述
 * \param time : 描述
 * \param pt : 描述
 * \return 返回值的描述
 */
void cCharacter::GetFrameInfo(size_t frame,DWORD & time,POINT & pt)
{
	switch(GetAction())
	{
	case Ani_Walk:
	case Ani_Run:
		{
			DWORD time_eslasped = frame * 40;
			time = time_eslasped + m_dwMoveBeginTime;
			pt.x = Time2Speed(m_xSpeed) + m_ptSrc.x;
			pt.y = Time2Speed(m_ySpeed) + m_ptSrc.y;
		}
		break;
	}
}

/**
 * \brief  设置人物（主角）的各个部分的显示顺序，以便不同的方向按相应的排序显示
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void cCharacter::InitOrder()
{
	//get aniinfo
	
	for (int j=0; j<c_nMaxDir; j++)
	{
		AniPartEnum back = Ani_Part_LeftWeapon;
		AniPartEnum front = Ani_Part_RightWeapon;

#ifdef _HAS_WEAPON_COLOR
		AniPartEnum back2 = Ani_Part_LeftWeaponColor;
		AniPartEnum front2 = Ani_Part_RightWeaponColor;
#endif

		if (j > 4)
		{
			back = Ani_Part_RightWeapon;
			front = Ani_Part_LeftWeapon;

#ifdef _HAS_WEAPON_COLOR
			back2 = Ani_Part_RightWeaponColor;
			front2 = Ani_Part_LeftWeaponColor;
#endif

		}
		BYTE* aByte = m_aOrder[j];
		int i;
		switch(j) 
		{
		case 0:
			i = 0;

#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif

			aByte[i++] = Ani_Part_LeftWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_LeftWeaponColor;
#endif
			aByte[i++] = Ani_Part_RightWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_RightWeaponColor;
#endif
			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			break;
		case 4:
			i = 0;
#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif
			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			aByte[i++] = Ani_Part_RightWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_RightWeaponColor;
#endif

			aByte[i++] = Ani_Part_LeftWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_LeftWeaponColor;
#endif

			break;
		case 1: 
		case 2: 
		case 6: 
		case 7: 
			i = 0;

#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif

			aByte[i++] = back;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = back2;
#endif
			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			aByte[i++] = front;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = front2;
#endif

			break;
		case 3:
		case 5:
			i = 0;

#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif

			aByte[i++] = back;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = back2;
#endif

			aByte[i++] = Ani_Part_Body;

			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			aByte[i++] = front;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = front2;
#endif

			break;
		default:
			;
		}
	}


	memset(s_aRideOrder,0,sizeof(s_aRideOrder));

	s_aRideOrder[1] = s_aRideOrder[2] = s_aRideOrder[3] = Ani_Part_Body;
	s_aRideOrder[7] = s_aRideOrder[6] = s_aRideOrder[5] = Ani_Part_Body;

	for (int j=0; j<c_nMaxDir; j++)
	{
		AniPartEnum back = Ani_Part_LeftWeapon;
		AniPartEnum front = Ani_Part_RightWeapon;

#ifdef _HAS_WEAPON_COLOR
		AniPartEnum back2 = Ani_Part_LeftWeaponColor;
		AniPartEnum front2 = Ani_Part_RightWeaponColor;
#endif
		if (j > 4)
		{
			back = Ani_Part_RightWeapon;
			front = Ani_Part_LeftWeapon;

#ifdef _HAS_WEAPON_COLOR
			back2 = Ani_Part_RightWeaponColor;
			front2 = Ani_Part_LeftWeaponColor;
#endif

		}
		BYTE* aByte = m_aOnRideOrder[j];
		int i;
		switch(j) 
		{
		case 0:
			i = 0;

#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif

			aByte[i++] = Ani_Part_LeftWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_LeftWeaponColor;
#endif
			aByte[i++] = Ani_Part_RightWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_RightWeaponColor;
#endif

			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			break;
		case 4:
			i = 0;
#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif
			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			aByte[i++] = Ani_Part_RightWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_RightWeaponColor;
#endif

			aByte[i++] = Ani_Part_LeftWeapon;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = Ani_Part_LeftWeaponColor;
#endif

			break;
		case 1: 
		case 2: 
		case 6: 
		case 7: 
			i = 0;

#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif
			aByte[i++] = back;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = back2;
#endif

			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			aByte[i++] = front;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = front2;
#endif

			break;
		case 3:
		case 5:
			i = 0;
#ifdef _HAS_WEAPON_SHADOW
			aByte[i++] = Ani_Part_LeftWeaponShadow;
			aByte[i++] = Ani_Part_RightWeaponShadow;
#endif

			aByte[i++] = back;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = back2;
#endif
			aByte[i++] = Ani_Part_Body;
			aByte[i++] = Ani_Part_BodySystemColor;
			aByte[i++] = Ani_Part_BodyCustomColor;
			aByte[i++] = Ani_Part_Hair;
			aByte[i++] = front;

#ifdef _HAS_WEAPON_COLOR
			aByte[i++] = front2;
#endif

			break;
		default:
			;
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
static cCharacter * s_pMainCharacter = NULL;
void cCharacter::SetMainCharacter()
{
	s_pMainCharacter = this;
}

/**
 * \brief 是否主角
 */
bool cCharacter::IsMainCharacter()
{
	return s_pMainCharacter == this || (s_pMainCharacter && s_pMainCharacter->m_pRide == this);
}


iCharacter* Character_GetMainCharacter()
{
	return s_pMainCharacter;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT Character_GetMainPosition()
{
	if(s_pMainCharacter)
		return s_pMainCharacter->GetPos();
	else return stPointI(0,0);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
cCharacter::cCharacter(void)
{
	m_dwPlayRandSoundTime = 0;
#ifdef _DEBUG
	Assert(s_bNewCharacter);
#endif
	memset(&m_crMouseOver,0,sizeof(m_crMouseOver));
	m_nSex = 0;
	m_nMainAniIndex = 0;
	m_nRunNum = 0;
	m_id = -1;
	m_bNpc = true;
	m_pOrder = NULL;

	m_ulCount = 1;

	m_fRunSpeed = 1.f;

	m_xSpeed = 0;
	m_ySpeed = 0;
	m_xPos = 0;
	m_yPos = 0;

	m_ePreAni = Ani_Null;

	m_bDie = false;

	m_nWait = 0;
	m_nWaitTime = rand() % c_nWaitTime+c_nWaitBaseTime;

	m_eAttack = Attack_Null;

	m_nRun = 0;

	m_lData = 0;
	m_nSortType = 0;

	m_pTable = NULL;

	m_pRide = NULL;
	m_bMoveable = true;

	m_pLight = LIGHT_CREATELIGHT;

	m_ptCenter = stPointI(-1,-1);

	m_bPauseMove = false;

	m_dwLayerColor = 0;
	m_fLayerScaleX = 1.0f;
	m_fLayerScaleY = 1.0f;
	m_bEnableShake = true;
	m_bAppear = false; 

	m_prevMoveAni.endTime = 0;
	m_prevMoveAni.ani = Ani_Null;

	m_bArrowInRightHand = false;

	m_dwChangeIdleTime = xtimeGetTime() + rand()%7000 + 3000;
	m_dwPlayMoveTime = 0;
}


ULONG cCharacter::Release() 
{
	int n = --m_ulCount;
	if (!m_ulCount) 
	{
#ifdef _DEBUG
		s_bDeleteCharacter = true;
#endif
		//s_allocCharacter.freePointer(this);
		delete this; 
#ifdef _DEBUG
		s_bDeleteCharacter = false;
#endif
	}
	return n;
};


void cCharacter::ReleaseAnimation()
{
	m_Animation.ReleaseAnimation();
	
	if(m_pRide)
	{
		m_pRide->FinalRelease();
		m_pRide->Release();
		m_pRide = NULL;
	}
}


cCharacter::~cCharacter(void)
{
#ifdef _DEBUG
	Assert(s_bDeleteCharacter);
#endif
	FinalRelease();
	ReleaseAnimation();
	if(s_pMainCharacter == this)
	{
		s_pMainCharacter = NULL;
	}
}

void cCharacter::FinalRelease()
{
	m_Animation.FinalRelease();
	for_each(m_listMagic.begin(),m_listMagic.end(),mem_fun<ULONG,iMagic>(&iMagic::Release));
	m_listMagic.clear();
#ifdef _GAME_TOOL
	GetRenderDump()->FinalReleaseMagic();
#endif
	for_each(m_listGenerate.begin(),m_listGenerate.end(),mem_fun<ULONG,iMagic>(&iMagic::Release));
	m_listGenerate.clear();

	SAFE_RELEASE(m_pLight);
	ClearEventList();
}

void cCharacter::ClearEventList()
{
	for(int i=0; i< m_listEvent.size(); ++i)
	{
		stEventEx & e = m_listEvent[i];
		if(e.pSender)e.pSender->Release();
	}

	m_listEvent.clear();
}

/**
 * \brief 设置人物的世界象素坐标
 * 
 * \param fx : 世界象素坐标x
 * \param fy : 世界象素坐标y
 * \return : void
 */
void cCharacter::SetPos(float fx,float fy)
{
	m_xPos = fx; m_yPos = fy;

	if (m_pRide)
		m_pRide->SetPos(fx,fy);
}

/**
 * \brief 以格子中点为目的点设置人物位置
 * 
 * \param pt : 描述
 * \return 返回值的描述
 */
void cCharacter::SetPosCenter(POINT pt)
{
	int xPos = pt.x / c_nMagicTileWidth * c_nMagicTileWidth + c_nMagicTileWidth/2;
	int yPos = pt.y / c_nMagicTileHeight * c_nMagicTileHeight + c_nMagicTileHeight/2;
	SetPos(stPointI(xPos,yPos));
}

/**
 * \brief 得到人物当前方向的各个部位的排序情况
 * 
 * \return 返回值的描述
 */
BYTE* cCharacter::GetOrder()
{
	size_t dir = GetDir();
	//size_t nAction = GetAction();
	BYTE* pOrder = NULL;
	if (!IsNpc() && m_nSortType != -1 )
		pOrder = GetCharacterOrder();  // always return null
	if (pOrder == NULL)
	{
		if(IsRide()) pOrder = m_aOnRideOrder[dir];
		else pOrder = m_aOrder[dir];
	}
	return pOrder;
}

/**
 * \brief 屏幕左上角相对世界地图起点的相对位置
 * \param ptMapOffset : 描述
 * \return 返回值的描述
 */
bool cCharacter::Draw(POINT ptMapOffset)
{
	m_Animation.DrawKeepImage(ptMapOffset,!IsNpc());
	if( m_pRide )
		m_pRide->DrawKeepImage(ptMapOffset,IsNpc());

	POINT pt ;
	if(GetAction() == Ani_FirstRun)
		pt = (POINT)GetDes() - ptMapOffset;
	else
		pt = (POINT)GetPos() - ptMapOffset;

	bool b = ( s_bDrawCover ? DrawCoverAtScreen(pt,GetOrder()) : DrawAtScreen(pt,GetOrder()) );
	return b;
}

/**
 * \brief 简短描述
 * \param ptScreen : 屏幕坐标
 * \return 返回值的描述
 */
bool cCharacter::DrawAtScreen(POINT ptScreen)
{
	//Assert(0);
	POINT pt = (POINT)GetPos() - ptScreen;
	m_Animation.DrawKeepImage(pt,!IsNpc());
	if( m_pRide )
		m_pRide->DrawKeepImage(ptScreen,IsNpc());
	return ( s_bDrawCover ? DrawCoverAtScreen(ptScreen,GetOrder()) : DrawAtScreen(ptScreen,GetOrder()) );
}

/**
 * \brief :  注意GetCenter()返回负数
 * \return 返回值的描述
 */
POINT cCharacter::GetDrawPosition()
{
	POINT pt = GetPos();
	pt += GetCenter();
	return pt;
}

/**
 * \brief 简短描述
 * \param ePart : 描述
 * \param ptMapOffset : 描述
 * \return 返回值的描述
 */
bool cCharacter::DrawPart(AniPartEnum ePart,POINT ptMapOffset)
{
	if (ePart < 0 || ePart >= GetAnimationNum())
		return false;
	POINT ptPosition = GetPos();
	ptPosition -= ptMapOffset;
	POINT point = GetCenter();
	//m_aAnimation[ePart]->Draw(ptPosition,point);
	m_Animation.DrawAtScreen(ePart,ptPosition,point);
	return true;
}

bool iCharacter::s_bHightlight = false;
bool iCharacter::s_bDrawCover = false;


bool cCharacter::DrawCoverAtScreen(POINT ptPosition,BYTE* aOrder)
{
	/*if (m_bDie)
		return false;*/

	if (m_pRide)
		m_pRide->DrawAtScreen(ptPosition);

	if (IsNpc())
	{
		stAnimationMagicDataStruct::BlendEnum eBlend = m_Animation.GetBlendType();
		for (int j=0; j<GetAnimationNum(); j++)
		{
			m_Animation.SetBlendType(j,stAnimationMagicDataStruct::Blend_AlphaSub);
			m_Animation.DrawAtScreen(j,ptPosition,GetCenter());
			m_Animation.SetBlendType(j,eBlend);
		}
	}
	else
	{
		stAnimationMagicDataStruct::BlendEnum eBlend = m_Animation.GetBlendType();
		for (int j=0; j<GetAnimationNum(); j++)
		{
			AniPartEnum ePart = (AniPartEnum )aOrder[j];
			if (ePart == Ani_Part_Null)
				continue;
			POINT point = GetCenter();
			m_Animation.SetBlendType(ePart,stAnimationMagicDataStruct::Blend_AlphaSub);
			m_Animation.DrawAtScreen(ePart,ptPosition,point);
			m_Animation.SetBlendType(ePart,eBlend);
		}
	}

	return true;
}


bool cCharacter::DrawAtScreen(POINT ptPosition,BYTE* aOrder)
{ 
	//if (m_bDie)
	//	return false;

	DrawMagicDir(ptPosition,stMoveStruct::Sort_Foot,-1);
	DrawMagicDir(ptPosition,stMoveStruct::Sort_Back,-1);

	POINT ptMagic = GetMagicPosition(ptPosition);

	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end(); ++it)
	{
		iMagic* p = *it;
		if (p->GetSort() == stMoveStruct::Sort_Null)
		{
			POINT pt = p->GetPos();
			int dir = GetDir8(pt.x,pt.y);
			if (dir == 7 || dir == 0 || dir == 1)
			{
				p->DrawSort(ptPosition,ptMagic,stMoveStruct::Sort_Null,-1);
			}
		}
	}


	int nRideOrder = 0;
	
	if (m_pRide)
	{
		size_t dir = GetDir();
		nRideOrder = s_aRideOrder[dir];
	}

	if(s_bHightlight)
	{
		const D3DXCOLOR colorDst(100.0f/255.0f,100.0f/255.0f,100.0f/255.0f,1.0f);
		if(m_crMouseOver.r < colorDst.r)
		{
			float s = Engine_GetDrawElapsedTime()/0.2f;
			D3DXColorLerp(&m_crMouseOver,&m_crMouseOver,&colorDst,s);
		}

		if(m_crMouseOver.r > colorDst.r)
		{
			m_crMouseOver = colorDst;
		}
	}
	else 
	{
		const D3DXCOLOR colorDst((DWORD)0);
		if(m_crMouseOver.r > colorDst.r)
		{
			float s = Engine_GetDrawElapsedTime()/0.1f;
			D3DXColorLerp(&m_crMouseOver,&m_crMouseOver,&colorDst,s);
		}

		if(m_crMouseOver.r < colorDst.r)
		{
			m_crMouseOver = colorDst;
		}
	}

	if(m_crMouseOver.r > 0.0f)
	{
		if (IsNpc())
		{
			if( m_pRide) m_pRide->DrawAtScreen(ptPosition);

			for (int j=0; j<GetAnimationNum(); j++)
			{
				m_Animation.DrawAtScreen(j,ptPosition,GetCenter());

				stAnimationMagicDataStruct::BlendEnum eBlend = m_Animation.GetBlendType(j);
				DWORD dwColor = m_Animation.GetColor(j);
				m_Animation.SetBlendType(j,stAnimationDataStruct::Blend_Add2);
				m_Animation.ChangerColor(j,m_crMouseOver);

				m_Animation.DrawAtScreen(j,ptPosition,GetCenter());

				m_Animation.SetBlendType(j,eBlend);
				m_Animation.ChangerColor(j,dwColor);
			}
		}
		else
		{
			DWORD dwMouseOver = m_crMouseOver;

			for (int j=0; j<GetAnimationNum(); j++)
			{

				AniPartEnum ePart = (AniPartEnum )aOrder[j];
				if (ePart == Ani_Part_Null)
					continue;

				if( m_pRide && (nRideOrder == ePart) ) 
					m_pRide->DrawAtScreen(ptPosition);

				POINT point = GetCenter();

				m_Animation.DrawAtScreen( ePart , ptPosition , point );

#ifdef _HAS_WEAPON_COLOR
				if( ePart != Ani_Part_LeftWeaponColor && ePart != Ani_Part_RightWeaponColor)
#endif
				{

					stAnimationMagicDataStruct::BlendEnum eBlend = m_Animation.GetBlendType(ePart);

					DWORD dwColor = m_Animation.GetColor( ePart );

					m_Animation.SetBlendType( ePart , stAnimationDataStruct::Blend_Add2 );
					m_Animation.ChangerColor( ePart , dwMouseOver );

					m_Animation.DrawAtScreen( ePart , ptPosition , point );

					m_Animation.SetBlendType( ePart , eBlend );
					m_Animation.ChangerColor( ePart , dwColor );
				}
			}
		}		
	}
	else
	{
		if (IsNpc())
		{
			if( m_pRide) m_pRide->DrawAtScreen(ptPosition);
			for (int j=0; j<GetAnimationNum(); j++)
			{
				m_Animation.DrawAtScreen(j,ptPosition,GetCenter());
			}
		}
		else
		{
			for (int j=0; j<GetAnimationNum(); j++)
			{
				AniPartEnum ePart = (AniPartEnum )aOrder[j];
				if (ePart == Ani_Part_Null)
					continue;

				if( ePart == Ani_Part_BodyCustomColor )
					int temp = 10;

				if( m_pRide && (nRideOrder == ePart) ) m_pRide->DrawAtScreen(ptPosition);		//

				POINT point = GetCenter();
				m_Animation.DrawAtScreen(ePart,ptPosition,point);
			}
		}
	}

	if(m_dwLayerColor != 0)
	{
		// 在上面加一层颜色
		if (IsNpc())
		{
			stPointI point(0,0);
			stPointI pt = ptPosition;
			pt += GetCenter();
			stPointF scale(m_fLayerScaleX,m_fLayerScaleY);

			if(m_fLayerScaleX != 1.0f || m_fLayerScaleY != 1.0f)
			{
				int w,h;
				if(GetMainAnimation()->GetWholeSize(w,h))
				{
					pt.x -= (m_fLayerScaleX - 1.0f) * w /2.0f;
					pt.y -= (m_fLayerScaleY - 1.0f) * h /2.0f;
				}
			}

			stAnimationMagicDataStruct::BlendEnum eBlend = GetMainAnimation()->GetBlendType();
			DWORD dwColor = GetMainAnimation()->GetAnimationColor();
			GetMainAnimation()->SetBlendType(stAnimationMagicDataStruct::Blend_IgnoreTexelColor);
			GetMainAnimation()->SetAnimationColor(m_dwLayerColor);

			GetMainAnimation()->Draw(pt,point,scale);

			GetMainAnimation()->SetBlendType(eBlend);
			GetMainAnimation()->SetAnimationColor(dwColor);

		}
		else
		{

			stPointI point(0,0);
			stPointI pt = ptPosition;
			pt += GetCenter();
			stPointF scale(m_fLayerScaleX,m_fLayerScaleY);

			if(m_fLayerScaleX != 1.0f || m_fLayerScaleY != 1.0f)
			{
				int w,h;
				if(m_Animation.GetWholeSize(Ani_Part_Body,w,h))
				{
					pt.x -= (m_fLayerScaleX - 1.0f) * w /2.0f;
					pt.y -= (m_fLayerScaleY - 1.0f) * h /2.0f;
				}
			}

			const AniPartEnum aPart[] = {Ani_Part_Hair,Ani_Part_Body,Ani_Part_BodySystemColor,Ani_Part_BodyCustomColor};


			stAnimationMagicDataStruct::BlendEnum eBlend = m_Animation.GetBlendType();
			for (int j=0; j<sizeof(aPart)/sizeof(aPart[0]); j++)
			{
				DWORD dwColor = m_Animation.GetAnimationColor(aPart[j]);

				m_Animation.SetBlendType(aPart[j],stAnimationMagicDataStruct::Blend_IgnoreTexelColor);
				m_Animation.ChangerColor(aPart[j],m_dwLayerColor);

				m_Animation.DrawAtScreen(aPart[j],pt,point,scale);

				m_Animation.SetBlendType(aPart[j],eBlend);
				m_Animation.ChangerColor(aPart[j],dwColor);
			}
		}
	}


	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end(); ++it)
	{
		iMagic* p = *it;
		if (p->GetSort() == stMoveStruct::Sort_Null)
		{
			POINT pt = p->GetPos();
			int dir = GetDir8(pt.x,pt.y);
			if (dir == 7 || dir == 0 || dir == 1)
			{
			}
			else
			{
				p->DrawSort(ptPosition,ptMagic,stMoveStruct::Sort_Null,-1);
			}
		}
	}

	//DrawMagicDir(ptPosition,stMoveStruct::Sort_Null,-1);
	DrawMagicDir(ptPosition,stMoveStruct::Sort_Front,-1);

	return true;
}


bool cCharacter::Stop(POINT pt)
{
	if (m_pRide)
		m_pRide->Stop(pt);
	SetPos(pt);
	SetDes(pt);
	return StopIn();
}

bool cCharacter::ChangeIdleAction()
{
	int n  = m_pTable->GetPropertyCount();
	if(n > Ani_Idle0 && GetAction() == Ani_Wait)
	{
		// 有几组待机动作
		if(xtimeGetTime() > m_dwChangeIdleTime)
		{
			m_dwChangeIdleTime = xtimeGetTime() + rand()%7000 + 1000;

			AniTypeEnum nAni = Ani_Wait;
			stNumPercent randNum[16];
			size_t count = 0;
			size_t index = 0;
			randNum[count++].num = Ani_Wait;
			for( ; index < count_of(randNum) && index < 4 && m_pTable->GetPropertyCount() > Ani_Idle0 + index; ++ index)
			{
				if( IsExistAction((AniTypeEnum)(Ani_Idle0 + index)) )
				{
					randNum[count++].num = Ani_Idle0 + index;
				}
			}
			if( count > 1 )
			{
				int percent = 100/(count);
				randNum[0].percent = 100 - (count-1) * percent;
				for(size_t i=1;i<count;++i)
				{
					randNum[i].percent = percent;
				}
				if(!GetRandNumByPercent(randNum,count,(int&)nAni)) nAni = Ani_Wait;
			}
			if(nAni != Ani_Wait)
			{
				Action(nAni,true);
				return true;
			}
		}
	}
	return false;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::StopIn()
{
	if (IsMovingBreak()) // 运动时由于受到攻击而中断
	{
		Action(GetPreAction());
		//WalkTo(GetDes(),true);
		return true;
	}
	m_nWait = 0;
	m_nWaitTime = rand()%c_nWaitTime+c_nWaitBaseTime;

	AniTypeEnum nAni = Ani_Wait;
	if ( GetAction() != Ani_Null )
		nAni = GetAniNextAction(GetAction());
	if ( nAni != Ani_Null )
	{
		Action(nAni);
		m_Animation.ClearKeepImage();
		if(m_pRide)
			m_pRide->ClearKeepImage();
		return true;
	}
	return false;
}

void cCharacter::Idle()
{
	//Action(Ani_Idle);
	Action(Ani_Wait);
}

void cCharacter::AssertActionEnd(AniTypeEnum nPrev)
{
	if (IsMoving(GetAction()))
	{
		POINT pt = GetPos();
		if (GetAniPri(nPrev) <= GetAniPri(GetAction()))
		{
			if (nPrev != Ani_Hurt)
			{}
			//Assert(pt.x % s_nGridX == 0 && pt.y % s_nGridY == 0);
		}
	}
}

bool cCharacter::AssertActionBefore(AniTypeEnum nNext)
{
	AniTypeEnum ani = GetAction();
	if (IsMoving(GetAction()))
	{
		POINT pt = GetPos();
		if (GetAniPri(nNext) <= GetAniPri(ani))
		{
			if (nNext == Ani_Hurt)
				return true;
			if (pt.x % s_nGridX == 0 && pt.y % s_nGridY == 0) // 人物必须在格子的中点
				;
			else
				return false;
		}
	}
	if (ani == nNext)
	{
		if (ani == Ani_Walk || ani == Ani_Run || ani == Ani_Wait /*|| ani == Ani_Idle*/ || ani == Ani_Appear ||
			ani == Ani_Ride_Walk || ani == Ani_Ride_Run || ani == Ani_Ride_Wait /*|| ani == Ani_Ride_Idle*/ )
			;
		else
			return false;
	}
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::Action(AniTypeEnum n,bool bServerLogic)
{
	if (m_pRide)
		m_pRide->Action(n,bServerLogic);

	if (n == Ani_Null)
		return false;
	m_bPauseMove = false;
	if(m_pRide)m_pRide->m_bPauseMove = false;

	if (!bServerLogic && !AssertActionBefore(n))
	{
		return false;
	}

	AniTypeEnum nAni = GetAction();

	 
	if (IsRide())
	{
		n = GetAniRideAction(n);
		if (n == Ani_Null)
			return false;
	}
	else
	{
		n = GetAniUnrideAction(n);
		if (n == Ani_Null)
			return false;
	}
 
	if (!bServerLogic && nAni == n)
	{
		if ( nAni != Ani_Appear )
			return true;
	}

	 

	AniTypeEnum n1 = GetAction();
	m_ePreAni = GetAction();
	//	SetPrevAniEndTime(m_ePreAni,m_nRun);	

	  

	if (!ActionAni(n,GetAnimationStruct(n),bServerLogic))
	{
		for(size_t i=0;i<MAX_EVENT;++i)
		{
			const stEvent& st = GetEvent(n,i);
			if (st.Run(0,0,1,0))
				OnEvent(st,i,this);
		}
		if (n != Ani_Wait && n != Ani_Ride_Wait)
			StopIn();
	}

	
	AssertActionEnd(GetPreAction());  // do nothing


	SetPrevAniEndTime(GetAction(),m_nRun + GetAnimationTime());
	

	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ani : 描述
 * \return 返回值的描述
 */
void cCharacter::PlayActionSound(AniTypeEnum ani)
{
	SoundEnum e;
	size_t nKind = 0;
	bool bLoop = IsAniLoop(GetAction());
	if (IsNpc())
	{
		e = Sound_Npc;
		nKind = GetMainAnimation()->GetAnimationID();
	}
	else
	{
		switch (ani)
		{
		case Ani_Hurt:
		case Ani_Die:
		case Ani_Ride_Hurt:
		case Ani_Ride_Die:
			if (IsFemale())
				e = Sound_Female;
			else
				e = Sound_Male;
			break;
		default:
			e = Sound_Character;
			break;
		}
	}
	char sSound[256];
 
	GetAniSound(ani,sSound,256);
	if (sSound[0] != 0 && strcmp(sSound , "null"))
	{
		DWORD time = xtimeGetTime();
		if(ani == Ani_Wait)
		{
			if(time > m_dwPlayRandSoundTime)
			{
				if(m_dwPlayRandSoundTime != 0)
					PlayCharacterSound(NULL,GetPos(),GetMoveSpeed(),e,nKind,sSound,false,IsMainCharacter());
				m_dwPlayRandSoundTime = xtimeGetTime() + rand() % 30000 + 8000;
			}
		}
		else if(time - m_aAni[ani].nPrevPlaySoundTime >= m_Animation.GetAnimationTime() * 2/3){
			m_aAni[ani].nPrevPlaySoundTime = time;
			PlayCharacterSound(NULL,GetPos(),GetMoveSpeed(),e,nKind,sSound,false,IsMainCharacter());
			
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
void cCharacter::PlayCurSound()
{
	//if(GetAction() == Ani_Appear) return;
	PlayActionSound(GetAction());
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT cCharacter::GetMoveSpeed()
{
	if (IsMoving() && !m_bPauseMove)
	{
		return stPointI(m_xSpeed,m_ySpeed);
	}
	return stPointI(0,0);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param index : 描述
 * \return 返回值的描述
 */
void cCharacter::PlayFxSound(int index)
{
	AniTypeEnum ani = GetAction();
	stSoundFx& fx = GetSoundFx( GetAction() );
	if (fx.nStartID > 0  && fx.nNum > 0)
	{
		if( IsMoveAni(ani) )
		{
			char szSound[MAX_SOUNDNAME];
			sprintf( szSound,"%d" , rand() % fx.nNum +  fx.nStartID);

			//TRACE("******* sound event %d,sound %s \n",index,szSound );

			if(xtimeGetTime() - m_dwPlayMoveTime > 150){
				if(ani == Ani_Ride_Walk || ani == Ani_Ride_Run)
					;
				else
					PlayCharacterSound(NULL,GetPos(),GetMoveSpeed(),Sound_Ground,0,szSound,false,IsMainCharacter());
				m_dwPlayMoveTime = xtimeGetTime();
			}
		}
		else
		{
			char szSound[MAX_SOUNDNAME];
			sprintf( szSound,"%d",fx.nStartID + rand()%fx.nNum );

			PlayCharacterSound(NULL,GetPos(),GetMoveSpeed(),Sound_Ground,0,szSound,false,IsMainCharacter());

		}
	}
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nAni : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::ActionAni(int nAni,bool bServerLogic)
{
	return ActionAni(nAni,GetAnimationStruct(nAni));
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nAni : 描述
 * \param stAnimation : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::ActionAni(int nAni,const stAnimationStruct& stAnimation,bool bServerLogic)
{
	bool b = false;

	stAnimationStruct st = stAnimation;
	int oldGroup = st.nGroup; 
	if(!IsNpc())
	{
		//&& GetPartID(Ani_Part_LeftWeapon) <= 0)
		if( GetPartID(Ani_Part_RightWeapon) <= 0  || m_bArrowInRightHand )
		//if( GetPartID(Ani_Part_RightWeapon) <= 0 && !m_bArrowInRightHand )
		{
			switch(nAni)
			{
			case Ani_Ride_Wait:
				st.nGroup = 25;
				break;
			case Ani_Ride_Walk:
				st.nGroup = 26;
				break;
			case Ani_Ride_Run:
				st.nGroup = 27;
				break;
			}
			
		}
	}

	st.nAnispeed = GetAniSpeed((AniTypeEnum)nAni);
	b = m_Animation.Action(nAni,st,bServerLogic);

#ifdef _DEBUG_RIDE    // 验证人物和骑马的设置一致性可打开此调试
	if(m_id == 0 )  // 主角
	{
		int nDir = GetDir();
		int nFrame = GetFrame();
		int nAction = GetAction();
		const stAnimationStruct& _ani = GetAnimationStruct(nAction);
		TRACE("主角当前方向为%d,当前帧数为%d,动作为%d,动画组:%d,动画方向:%d,\n",nDir,nFrame,nAction,_ani.nGroup,_ani.nDir);
	}
	if(  m_id == 113)  // 马
	{
		int nDir = GetDir();
		int nFrame = GetFrame();
		int nAction = GetAction();
		const stAnimationStruct& _ani = GetAnimationStruct(nAction);
		TRACE("马匹当前方向为%d,当前帧数为%d,动作为%d,动画组:%d,动画方向:%d,\n",nDir,nFrame,nAction,_ani.nGroup,_ani.nDir);
	}
#endif 
	if (!b)
		return false;

	if( oldGroup != st.nGroup )
	{
		// 头发转换为原来的Group
		st.nGroup = oldGroup;
		iAnimation* pAni = m_Animation.GetAnimation( Ani_Part_Hair );
		if(pAni)
		{
			pAni->Action( nAni,st,bServerLogic );
		}
	}

	if(nAni == Ani_Run || nAni == Ani_Ride_Run)
	{
		DWORD dwTime = xtimeGetTime();
		if(m_prevMoveAni.ani == nAni && ( m_prevMoveAni.endTime < dwTime || xtimeGetTime() - m_prevMoveAni.endTime < 300 ) )
		{
			int nFrameNum = GetFrameOfDir( nAni );
			if( m_prevMoveAni.endFrame < nFrameNum )
			{
				m_Animation.SetDirFrame(m_prevMoveAni.endFrame); // 设置当前方向的当前帧为上次结束时的帧(接着跑)
				m_Animation.AddRunTime((m_prevMoveAni.endFrame - (int)m_prevMoveAni.endFrame) * st.nAnispeed); // 弥补float到int的损失
			}
		}
		m_prevMoveAni.ani = (AniTypeEnum)nAni;
	}
	else if( nAni != Ani_Wait && nAni != Ani_Ride_Wait )
	{
		m_prevMoveAni.ani = (AniTypeEnum)nAni;
	}

	for (int i=0; i<MAX_EVENT; i++)
	{
		const stEvent& st = GetEvent(GetAction(),i);
		if (st.Run0())   // always be false 
			OnEvent(st,i,this);
	}

	m_ptCenter = stPointI(-1,-1);

	int soundFrame = GetSoundFrame((AniTypeEnum)nAni);
	if(soundFrame == 0)
		PlayCurSound();

	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::IsAttacking()
{
	int n = GetAction();
	return (n == Ani_Magic || n == Ani_Bow || n == Ani_Attack || n == Ani_Attack2);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptDes : 描述
 * \return 返回值的描述
 */
void cCharacter::TurnTo(POINT ptDes)
{
	POINT pt = ptDes;
	pt -= GetPos();
	Turn(pt);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
void cCharacter::TurnBack()
{
	m_Animation.TurnBack();
	if(m_pRide)
		m_pRide->TurnBack();
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param iFrame : 描述
 * \return 返回值的描述
 */
void cCharacter::ChangeFrame(size_t iFrame)
{
	m_Animation.ChangeFrame(iFrame);
	if (m_pRide)
		m_pRide->ChangeFrame(iFrame);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 描述
 * \return 返回值的描述
 */
void cCharacter::TurnTo(size_t n)
{
	if (n >= 0 && n < 8)
	{
		m_Animation.TurnTo(n);
		if (m_pRide)
			m_pRide->TurnTo(n);
	}
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pt : 描述
 * \return 返回值的描述
 */
void cCharacter::Turn(POINT pt)
{
	m_Animation.Turn(pt);
	if (m_pRide)
		m_pRide->Turn(pt);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pt : 描述
 * \return 返回值的描述
 */
bool cCharacter::Contain(POINT pt)
{
	RECT r = {-c_nMagicTileWidth/2,-c_nMagicTileHeight/2,c_nMagicTileWidth/2,c_nMagicTileHeight/2};
	POINT pt1 = GetPos();
	OffsetRect(&r,pt1.x,pt1.y);
	return 0 != PtInRect(&r,pt);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nSortType : 描述
 * \param aID : 描述
 * \param bAppear : 描述
 * \return 返回值的描述
 */
bool cCharacter::ReCreate(int nSortType, const std::vector<int>& aID,bool bAppear)
{
	cAnimation aniState;
	bool bCopyState = false;
	if(GetMainAnimation())
	{
		aniState.CopyState((cAnimation*)GetMainAnimation());
		bCopyState = true;
	}
	AniTypeEnum ani = GetAction();
	ReleaseAnimation();
	if (!InitCreate(nSortType,aID,bAppear))
		return false;
	Action(ani);
	if(bCopyState && ani != Ani_Null)
	{
		m_Animation.CopyState(&aniState);
	}

	return true;
}

bool cCharacter::CreateCharacter(int nSortType, const std::vector<int>& aID, bool bAppear)
{
	if (GetAnimationNum() == 0)
		return InitCreate(nSortType,aID,bAppear);
	if(ReCreate(nSortType,aID,bAppear))
	{
		//if(bAppear) this->PlayActionSound(Ani_Appear);
		return true;
	}
	return false;
}

bool cCharacter::Init(bool bAppear)
{
	if (m_pRide)
		m_pRide->Init(bAppear);

	m_bDie = false;
	m_ePreAni = Ani_Null;
	m_ptDes = GetPos();
	StopIn();

	if (!bAppear || !Action(Ani_Appear))
		Action(Ani_Wait);
	TurnTo(0);
	ChangeAlpha(0xff);
	return true;
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nSortType : 描述
 * \param aID : aID里面如果只有一个值，代表时npc; 如果有多个值，代表是人物
 * \param bAppear : 描述
 * \return 返回值的描述
 */
bool cCharacter::InitCreate(int nSortType, const std::vector<int>& aID, bool bAppear)
{
	int id = 0;
	if (aID.size() == 1)
		id = aID[0];
	if (id < 0 || id > 9999)
		return false;
	m_id = id;

	iPropertyTable* p = Property_GetTableManager()->GetiPropertyTable(Property_Character,id);
	Assert(p);
	size_t num = p->GetPropertyCount();

	m_pTable = p;

	m_nSortType = nSortType;
	//m_pOrder = ::GetCharacterOrder(m_nSortType);
	m_pOrder = NULL;
	int n = 0;

	const char* aType[] = 
	{
#ifdef _HAS_WEAPON_SHADOW
		"shadow\\weaponl",
		"shadow\\weaponr",
#endif
		"body",
		"weaponl",
		"weaponr",
		"hair",
		"pattern\\Body1",
		"pattern\\Body2",

#ifdef _HAS_WEAPON_COLOR
		"pattern\\weaponl",
		"pattern\\weaponr",
#endif
		"npc",
		"pattern\\npc"
	};

	int ani = Ani_Wait;
	const stAnimationStruct& stAni = GetAnimationStruct(ani);
	stAnimationStruct st = stAni;
	m_nMainAniIndex = 0;
	if (aID.size() == 1)
	{
		m_bNpc = true;
		iAnimation* pAni;
		pAni = Magic_CreateAnimation();
		int n = aID[0];

		if(!pAni->CreateAnimation("npc",n,ani,st,Usage_Npc))
			m_nMainAniIndex = 1;

		m_Animation.Add(pAni);
		pAni = Magic_CreateAnimation();
		if(!pAni->CreateAnimation("pattern\\npc",n,ani,st,Usage_Npc))
			m_nMainAniIndex = 0;
		m_Animation.Add(pAni);
	}
	else
	{
		m_nMainAniIndex = Ani_Part_Body;
		m_bNpc = false;
		m_Animation.Reserve(aID.size());
		for (size_t i=0; i<aID.size(); i++)
		{
			iAnimation* pAni = Magic_CreateAnimation();
			int n = aID[i];
			pAni->CreateAnimation(aType[i],n,ani,st,Usage_Character);
			m_Animation.Add(pAni);
		}
	}
	m_Animation.SetMainAniIndex(m_nMainAniIndex);
	m_ptCenter = GetDefaultCenter();

	if(m_pTable)
		for(size_t i=0;i<Ani_Num;++i)
		{
			if(i < m_pTable->GetPropertyCount())
			{
				m_aAni[i].nDelayTime = GetAniInfo(i).nDelay;
				m_aAni[i].fMoveSpeed = GetAniInfo(i).nMoveSpeed;
				m_aAni[i].nSpeed = GetAnimationStruct(i).nAnispeed;
			}
		}

	if (bAppear)  // 重生
		Appear();

	return true;
};

/**
 * \brief 跟随发送者的这些魔法产生子物体，放入pListMagic中,注意在run()中调用
 */
bool cCharacter::Generate(tagListMagicGenerate* plistMagic)
{
	tagListMagicGenerate& listMagic = *plistMagic;
	for (std::vector<iMagic* >::iterator it = m_listMagic.begin(); it != m_listMagic.end(); ++it)
	{
		iMagic* p = *it;

//#ifdef _DEBUG_MAGIC
//		cMagic * pMagic  = reinterpret_cast<cMagic*>(p);
//		if(pMagic)
//			TRACE("人物跟随魔法 %s 产生了\n",pMagic->m_stProperty.GetName());
//#endif

		p->Generate(listMagic);
	}
	if (!m_listGenerate.empty())
	{
		//listMagic.insert(listMagic.end(),m_listGenerate.begin(),m_listGenerate.end());
		for(std::vector<iMagic*>::iterator iter1 = m_listGenerate.begin(); iter1 != m_listGenerate.end();iter1++)
		{
			listMagic.push_back((*iter1));
		}
		m_listGenerate.resize(0);
	}
	
	// Some magics have died 
	std::vector<iMagic* >::iterator iter2 = find_if( m_listMagic.begin(),m_listMagic.end(),mem_fun<bool,iMagic>(&iMagic::IsDie) );
	if (iter2 != m_listMagic.end())
	{
#ifdef _GAME_TOOL
			GetRenderDump()->RemoveWatchMagic(*iter2);
#endif
#ifdef _DEBUG_MAGIC
			cMagic * pMagic  = reinterpret_cast<cMagic*>(*iter2);
			if(pMagic)
				TRACE("人物跟随魔法 %s 删除了\n",pMagic->m_stProperty.GetName());
#endif
			(*iter2)->Release();
			
			m_listMagic.erase(iter2);
	}
	return true;
}

float c_fMin = 0.0001f;

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nAni : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsMoving(AniTypeEnum nAni)
{
	int i = GetAniMoveSpeed(nAni);
	return (GetAniMoveSpeed(nAni) > c_fMin);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::IsMovingBreak()
{
	AniTypeEnum nAni = GetAction();
	if (nAni == Ani_Hurt || nAni == Ani_Ride_Hurt)
	{
		return IsMoving(GetPreAction());
	}
	return false;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::IsMoving()
{
	AniTypeEnum nAni = GetAction();
	return IsMoving(nAni);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param a : 描述
 * \param b : 描述
 * \return 返回值的描述
 */
inline bool fgreat(float a, float b)
{
	//return a > b;
	return (a - b) > -0.001f;
}
inline bool fless(float a, float b)
{
	//return a < b;
	return (a - b) < +0.001f;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param st : 描述
 * \param indexEvent : 描述
 * \param pSender : 描述
 * \return 返回值的描述
 */
bool cCharacter::OnEvent(const stEvent& st,int indexEvent,iObj * pSender)
{
	if (st.eEvent == stEvent::Event_Music)
		PlayFxSound(indexEvent);
	else if(st.eEvent == stEvent::Event_Shake)
	{
		m_bEnableShake = false;
		void Magic_Shake();
		Magic_Shake();
	}
	else if (st.eEvent == stEvent::Event_Magic)
	{
		//必须是follow的魔法
		int id = GetMagicTableID();
		int magicid = Magic_GetMagicID(st.szEventData,id);
		if (IsMagicRing(magicid))
			SetMagicRing(magicid,0,GetMagicTableID());
		//		else if (IsMagicAttackRing(magicid))
		//			SetMagicAttackRing(magicid,0,GetMagicTableID());
		//		else if (IsMagicEffect(magicid))
		//			SetMagicEffect(magicid);
		else
		{
			iMagic* pMagic = Magic_CreateMagic(magicid,id);
			if (pMagic != NULL)
			{
				pMagic->CreateMagic(IsLight());
				pMagic->TurnTo(GetDir());
				stFly fly;
				fly.pDes = NULL;
				fly.ptDes = GetPos();
				fly.pSender = this;
				fly.ptStart = GetPos();
				fly.ptPos = GetPos();
				pMagic->Fly(fly);
				OnMagicBegin(pMagic);
				if (pMagic->IsFollowSender())
					AddMagic(pMagic);
				else
					AddGenerate(pMagic);
			}
		}
	}
	else
	{
		stEventEx st0;
		memcpy(&st0,&st,sizeof(stEvent));
		st0.pSender = pSender;
		if(pSender) pSender->AddRef();

		m_listEvent.push_back(st0);

		return false;
	}
	return true;
}
typedef int t_footprintFrame[2][2];


/**
 * \brief 人物run()
 * 
 * 详细描述　更新人物信息　
 * 
 * \param time_eslasped : 描述
 * \return 返回值的描述
 */
//#define DEBUG_NORMAL2			5	// 普通输出信息
//extern void _DEBUG_MESSAGE( HWND hWnd, int iErrorCode, const char * pMessage, ... );
int cCharacter::Run(long time_eslasped)
{
	m_bEnableShake = true;
	time_eslasped *= m_fRunSpeed;

	int preFrame = m_Animation.GetDirFrame();

	if(!m_bPauseMove)
	{
		m_Animation.Run(time_eslasped);
	}
	
	if (m_pRide)
		m_pRide->Run(time_eslasped);

	int nowFrame = m_Animation.GetDirFrame();
	int allframe = m_Animation.GetFrameOfDir();

	//TRACE("preFrame %d, nowFrame %d, allFrame %d\n",preFrame,nowFrame,allframe);
	m_Animation.KeepImage(time_eslasped,GetPos(),GetCenter(),GetOrder());
	if( m_pRide)
		m_pRide->SetKeepImage(time_eslasped,GetPos(),GetCenter(),GetOrder());

	m_Animation.Fade(time_eslasped);

	//always add;
	m_nRun+=time_eslasped;	

	
	/*if ( m_bDie )
		return false;*/

	//_DEBUG_MESSAGE(NULL,DEBUG_NORMAL2,"for (std::list<iMagic* >::iterator it = m_listMagic.begin(); it != m_listMagic.end(); ++it)");
	for (std::vector<iMagic* >::iterator it = m_listMagic.begin(); it != m_listMagic.end();++it )
	{

		iMagic* p = *it;
		p->Run(time_eslasped);
		std::list<stEvent>& elist = p->GetEventList();
		for (list<stEvent>::iterator ite = elist.begin(); ite != elist.end(); ++ ite)
			OnEvent(*ite,-1,p);
		p->ClearEventList();
		/*if(p->IsDie())
		{
		it = m_listMagic.erase(it);
		}
		else it++;*/
	}

	float xPos = m_xPos;
	float yPos = m_yPos;
	for (int i=0; i<MAX_EVENT; i++)
	{
		AniTypeEnum aniType = GetAction();
		const stEvent& st = GetEvent(aniType,i);
		if (st.Run(time_eslasped,preFrame,nowFrame,allframe)){
			if(st.eEvent == stEvent::Event_Music)
				int iiiiii=0;
			OnEvent(st,i,this);
		}
	}
	
	/*if(GetAction() == Ani_Ride_Run || GetAction() == Ani_Ride_Walk)
	{
		stEvent eventWalk[2];
		eventWalk[0].eEvent = stEvent::Event_Music;
		eventWalk[0].eGenerate = stEvent::Event_Generate_FrameStart;
		eventWalk[0].nEventFrame = 1;

		eventWalk[1].eEvent = stEvent::Event_Music;
		eventWalk[1].eGenerate = stEvent::Event_Generate_FrameStart;
		eventWalk[1].nEventFrame = 5;

		stEvent eventRun[2];

		eventRun[0].eEvent = stEvent::Event_Music;
		eventRun[0].eGenerate = stEvent::Event_Generate_FrameStart;
		eventRun[0].nEventFrame = 1;

		eventRun[1].eEvent = stEvent::Event_Music;
		eventRun[1].eGenerate = stEvent::Event_Generate_FrameStart;
		eventRun[1].nEventFrame = 5;

		stEvent* pEvent = ( GetAction() == Ani_Ride_Walk ? eventWalk : eventRun );
		for (int i=0; i<2; i++)
		{
			AniTypeEnum aniType = GetAction();
			if (pEvent[i].Run(time_eslasped,preFrame,nowFrame,allframe))
				OnEvent(pEvent[i],i,this);
		}
	}*/

	if(IsHaveFootprint())
	{
		switch(GetAction())
		{
		case Ani_Walk:
		case Ani_Run:
			{
				t_footprintFrame frameRun = {{0,eFootprintRight},{GetFrameOfDir()/2,eFootprintLeft}};
				t_footprintFrame frameWalk = {{0,eFootprintLeft},{GetFrameOfDir()/2,eFootprintRight}};
				t_footprintFrame * frameInfo = (GetAction() == Ani_Run ? &frameRun : &frameWalk);
				for(size_t i=0;i<2;++i)
				{
					if(preFrame <= (*frameInfo)[i][0] && nowFrame > (*frameInfo)[i][0])
					{
						DWORD time0;
						POINT pt;
						GetFrameInfo((*frameInfo)[i][0],time0,pt);
						DWORD time2 = xtimeGetTime();
						DWORD time1 = time2-time0;
						GetFootprintManager().AddFootprint(m_strFootprint.c_str(),(*frameInfo)[i][1],GetDir(),pt,time1);
						break;
					}
				}
			}
			break;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//_DEBUG_MESSAGE(NULL,DEBUG_NORMAL2,"int soundFrame = GetSoundFrame(GetAction());");
	int soundFrame = GetSoundFrame(GetAction());
	if (soundFrame == nowFrame) //may be  >now Frame && <= preFrame
	{
		PlayCurSound();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////

	if (IsMoving() && !m_bPauseMove)
	{
		if (m_xSpeed == 0 && m_ySpeed == 0)
			StopIn();
		else
		{
			xPos += Time2Speed(m_xSpeed);
			yPos += Time2Speed(m_ySpeed);
			//Assert(xPos > 0 && xPos < 800 && yPos > 0 && yPos < 600);

			bool bStop = false;
			if (abs(m_xSpeed) >= abs(m_ySpeed))
			{
				if (m_xSpeed >= 0 && fgreat(xPos , m_ptDes.x))
					bStop = true;
				else if (m_xSpeed < 0 && fless(xPos , m_ptDes.x))
					bStop = true;
			}
			else
			{
				if (m_ySpeed >= 0 && fgreat(yPos , m_ptDes.y))
					bStop = true;
				else if (m_ySpeed < 0 && fless(yPos , m_ptDes.y))
					bStop = true;
			}
			if (bStop)
			{
				m_xSpeed = 0;
				m_ySpeed = 0;
				xPos = m_ptDes.x;
				yPos = m_ptDes.y;
			}
		}
	}
	else if (GetMainAnimation()->IsDie() && GetAction() == Ani_Die)
		m_bDie = true;
	else if (GetMainAnimation()->IsStoped())
	{
		if (IsMovingBreak())
		{
			TurnTo(m_ptDes);
		 
			Action(GetPreAction());
		}
		else if (GetAniPri(GetAction()) == Ani_Pri_Stand)
		{
			m_nWait += time_eslasped;
 
			int time = GetMainAnimation()->GetAnimationTime();
			
			//魔法攻击战立时间太长，其他的照旧
			if( GetAction() == Ani_Magic_Stand )
			{
			       if( m_nWait > 1360)
			       {
				       Idle();
				       m_nWait = 0;
				       m_nWaitTime = rand()%c_nWaitTime+c_nWaitBaseTime;
			       }

			}
			else 
			{
				if (m_nWait > (time+m_nWaitTime))//4 sec
				{
					Idle();
					m_nWait = 0;
					m_nWaitTime = rand()%c_nWaitTime+c_nWaitBaseTime;
				}
			}
			
		}
		else
			StopIn();
	}
	/*
	if (m_pRide)
	{
	m_xPos = m_pRide->m_xPos;
	m_yPos = m_pRide->m_yPos;
	}
	else if (xPos != m_xPos || yPos != m_yPos)
	SetPos(xPos,yPos);
	*/
	//注意这样的话马的速度都是一样了
	if (xPos != m_xPos || yPos != m_yPos)
	{
		SetPos(xPos,yPos);
	}

//#pragma message(ATTENTION(因为编辑器数据错误，在此只能强行判断不是人物才执行 ChangeIdleAction))
	if(m_id != 0)
		ChangeIdleAction();

	return true;
}

/**
 * \brief 得到当前方向
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
size_t cCharacter::GetDir()
{
	//for body should not be null;
	size_t n = GetMainAnimation()->GetDir();
	Assert(n >= 0 && n < 8);
	return n;
}

/**
 * \brief 得到当前相对帧
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
size_t cCharacter::GetDirFrame()
{
	return GetMainAnimation()->GetDirFrame();
}

/**
 * \brief 得到当前绝对帧
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
size_t cCharacter::GetFrame()
{
	return GetMainAnimation()->GetFrame();
}

/**
 * \brief 动作Ani下每个方向的帧数
 * 
 * 详细描述
 * 
 * \param Ani : 描述
 * \return 返回值的描述
 */
size_t cCharacter::GetFrameOfDir(int Ani)
{
	return GetMainAnimation()->GetFrameOfDir(GetAnimationStruct(Ani).nGroup);
}

/**
 * \brief 得到每个方向的帧数
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
size_t cCharacter::GetFrameOfDir()
{
	return GetMainAnimation()->GetFrameOfDir();
}

/**
 * \brief 设置当前方向的当前帧为当前方向的nFrame帧
 * 
 * 详细描述
 * 
 * \param nFrame : 描述
 * \return 返回值的描述
 */
void cCharacter::SetDirFrame(int nFrame)
{
	m_Animation.SetDirFrame(nFrame);
	if(m_pRide)m_pRide->SetDirFrame(nFrame);
}

/**
 * \brief 当前动作
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
AniTypeEnum cCharacter::GetAction()
{
	return (AniTypeEnum)GetMainAnimation()->GetAction();
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
//////////////////////////////////////////////////////////////////////////
//do action
//////////////////////////////////////////////////////////////////////////
bool cCharacter::TurnTo(size_t n,bool bServerLogic)
{
	//if (!bServerLogic && !CanChangeToAction(Ani_Pick))
	//	return false;

	TurnTo(n);
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT cCharacter::GetPos()
{
	POINT pt = {m_xPos+0.5f,m_yPos+0.5f};
	return pt;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pt : 描述
 * \return 返回值的描述
 */
void cCharacter::SetDes(POINT pt)
{
	//Assert(pt.x % s_nGridX == 0 && pt.y % s_nGridY == 0);
	//TRACE("*************88pt.x % s_nGridX != 0 && pt.y % s_nGridY != 0\n");
	m_ptDes = pt;
}

const double c_sqrt2 = sqrt((double)2.0);

/**
 * \brief　设置运动参数
 * 
 * 详细描述
 * 
 * \param ptDesPoint : 世界象素坐标(务必为格子的中点)
 * \param Ani : 运动动作
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::MoveTo(POINT ptDesPoint,AniTypeEnum Ani,bool bServerLogic)
{
	if (!IsMoveable())
		return false;

	if (m_pRide)
	{
		m_pRide->MoveTo(ptDesPoint,Ani,bServerLogic);
	}


	int fSpeed = GetMoveSpeed(Ani);

	SetSrc(GetPos());

	POINT ptDes = ptDesPoint;
	POINT ptSpeed = ptDes - GetPos();
	if (ptSpeed.x == 0 && ptSpeed.y == 0)
	{
		SetDes(ptDes);
		return true;
	}

	Turn(ptSpeed);
	if (Ani == Ani_Back)
		TurnBack();

    //  switch to the corresponding animation,just so so 
	if(Ani == Ani_Run || Ani == Ani_Walk)
	{
		//*
		if(m_bAppear && m_nRunNum == 0 && IsNpc() && HaveAniStruct(Ani_FirstRun) && GetAnimationStruct(Ani_FirstRun).strAnimation[0] && stricmp("null",GetAnimationStruct(Ani_FirstRun).strAnimation))
		{ 
			Action(Ani_FirstRun,bServerLogic);
		}
		else
			//*/
		{
			Action(Ani,bServerLogic);
		}
		m_nRunNum++;
	}
	else
	{
		Action(Ani,bServerLogic);
	}

	SetDes(ptDes);

	// 注意m_xSpeed值与fSpeed相同，而后者与anispeed速度又是相匹配的
	m_xSpeed = 0;
	m_ySpeed = 0;
	if (ptSpeed.x > 0)
		m_xSpeed = fSpeed;
	else if (ptSpeed.x < 0)
		m_xSpeed = -fSpeed;
	if (ptSpeed.y > 0)
		m_ySpeed = fSpeed / 2;
	else if (ptSpeed.y < 0)
		m_ySpeed = -fSpeed / 2;

	m_dwMoveBeginTime = xtimeGetTime();

	if(IsHaveFootprint())
	{
		//GetFootprintManager().AddFootprint(m_strFootprint.c_str(),eFootprintStand,GetDir(),GetPos());
		//GetFootprintManager().AddFootprint(m_strFootprint.c_str(),Ani == Ani_Walk ? eFootprintLeft : eFootprintRight,GetDir(),GetPos());
	}

	AniTypeEnum ani1;
	if(IsRide()) ani1 = this->GetAniRideAction(Ani);
	else	ani1 = this->GetAniUnrideAction(Ani);

	if(ani1 == Ani_Ride_Run || ani1 == Ani_Run)
	{
		int dis0;

		if(ani1 ==  Ani_Ride_Run)
			dis0 = 3;
		else
			dis0 = 2;

		stPointI ptCur = GetPos();
		int dis = max(abs(ptDes.x - ptCur.x)/c_nMagicTileWidth,abs(ptDes.y - ptCur.y)/c_nMagicTileHeight);

		int nFrameNum = GetFrameOfDir(ani1);
		float nCurFrameNum = nFrameNum * dis/dis0;

		m_prevMoveAni.endFrame = m_Animation.GetDirFrame() + nCurFrameNum;
		if(m_prevMoveAni.endFrame >= nFrameNum) m_prevMoveAni.endFrame -= nFrameNum;

		m_prevMoveAni.endTime = xtimeGetTime() + dis * c_nMagicTileWidth * 1000 / GetMoveSpeed(ani1) ;

	}

	return true;
}

void cCharacter::ChangeSpeed(int nXspeed,int nYspeed)
{
      m_xSpeed = nXspeed;
      m_ySpeed = nYspeed;
}
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptDes : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::WalkTo(POINT ptDes,bool bServerLogic)
{
	if (!bServerLogic && !CanChangeToAction(Ani_Walk))
		return false;
	return MoveTo(ptDes,Ani_Walk,false);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptDes : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::RunTo(POINT ptDes,bool bServerLogic)
{
	if (!bServerLogic && !CanChangeToAction(Ani_Run))
		return false;

	bool b = MoveTo(ptDes,Ani_Run,false); // prepare the parameters of movement to ok 

	
	return b;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pDes : 描述
 * \param nMagic : 描述
 * \param ppMagic : 描述
 * \param bServerLogic : 描述
 * \param nSetLife : 描述
 * \param nAction : 描述
 * \return 返回值的描述
 */
enumCreateMagicResult cCharacter::Magic(iCharacter* pDes, size_t nMagic,iMagic** ppMagic,bool bServerLogic,int nSetLife,AniTypeEnum nAction)
{
	//if (IsRide())
	//	return false;
	if (IsMagicRing(nMagic) || IsMagicAttackRing(nMagic))
		return (SetMagicRing(nMagic,nSetLife,GetMagicTableID()) ? CreateMagicResult_Succeeded : CreateMagicResult_OtherError);
	//	if (IsMagicEffect(nMagic))
	//		return SetMagicEffect(nMagic);

	Assert(pDes);
	stFly fly;
	fly.pDes = pDes;
	fly.ptDes = pDes->GetPos();
	fly.pSender = this;
	fly.ptStart = GetPos();
	fly.ptPos = GetPos();
	return Magic(fly,nMagic,ppMagic,bServerLogic,nSetLife,nAction);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptPoint : 描述
 * \param nMagic : 描述
 * \param ppMagic : 描述
 * \param bServerLogic : 描述
 * \param nSetLife : 描述
 * \param nAction : 描述
 * \return 返回值的描述
 */
enumCreateMagicResult cCharacter::Magic(POINT ptPoint, size_t nMagic,iMagic** ppMagic,bool bServerLogic,int nSetLife,AniTypeEnum nAction)
{
	//if (IsRide())
	//	return false;
	if (IsMagicRing(nMagic) || IsMagicAttackRing(nMagic))
		return (SetMagicRing(nMagic,nSetLife,GetMagicTableID()) ? CreateMagicResult_Succeeded : CreateMagicResult_OtherError);
	//	if (IsMagicEffect(nMagic))
	//		return SetMagicEffect(nMagic);

	stFly fly;
	fly.pDes = NULL;
	fly.ptDes = ptPoint;
	fly.pSender = this;
	fly.ptStart = GetPos();
	fly.ptPos = GetPos();
	return Magic(fly,nMagic,ppMagic,bServerLogic,nSetLife,nAction);
}
extern POINT c_aDirPoint[8];
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param fly : 描述
 * \param nMagic : 描述
 * \param ppMagic : 描述
 * \param bServerLogic : 描述
 * \param nSetLife : 描述
 * \param nAction : 描述
 * \return 返回值的描述
 */
enumCreateMagicResult cCharacter::Magic(stFly& fly, size_t nMagic,iMagic** ppMagic,bool bServerLogic,int nSetLife,AniTypeEnum nAction)
{
	MagicTypeEnum e = (MagicTypeEnum) nMagic;
	int id = GetMagicTableID();
	iMagic* p = Magic_CreateMagic(nMagic,id);
	if (p == NULL)
		return CreateMagicResult_NoFindMagic;

	//if ( bServerLogic )
		p->SetLife(nSetLife);

	AniTypeEnum nAni = Ani_Null;
	stCharacterStruct & st = p->GetCharacterStruct();
	nAni = st.eAnimation;
	if(nAction != Ani_Null)
		nAni = nAction;

	if(nAni != Ani_Null)
	{
		if(!CanChangeToAction(nAni))
		{
			p->Release();
			return CreateMagicResult_AniError;
		}
	}

	POINT point = fly.ptDes;
	POINT ptStart = GetPos();
	TurnTo(point);
	if (nAni != Ani_Null)
	{
		if (nAni == Ani_Run)
		{
			size_t dir = GetDir();
			point = c_aDirPoint[dir];
			point.x *= 64 * 5;
			point.y *= 32 * 5;
			RunTo(point+GetPos(),true);
			//modify the point and fly.ptDes
		}
		else
		{
			if(!Action(nAni,bServerLogic))
			{
				p->Release();
				return CreateMagicResult_AniError;
			}
			//if(m_pRide) m_pRide->Action(nAni,bServerLogic);
		}
	}

	p->CreateMagic(IsLight());

#ifdef _DEBUG
	cMagic * pMagic = (cMagic *)p;
	int iii = 0 ;
#endif 
	p->Fly(fly);

	if(fly.ptDes.x == fly.ptPos.x && fly.ptDes.y == fly.ptPos.y)
		p->TurnTo(this->GetDir());
	OnMagicBegin(p);

	if (p->IsFollowSender())
	{
		AddMagic(p);
		*ppMagic = NULL;
	}
	else
		*ppMagic = p;
	return CreateMagicResult_Succeeded;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptDes : 描述
 * \return 返回值的描述
 */
void GetTileCenter(POINT& ptDes)
{
	ptDes.x /= c_nMagicTileWidth;
	ptDes.x *= c_nMagicTileWidth;
	ptDes.x += c_nMagicTileWidth/2;
	ptDes.y /= c_nMagicTileHeight;
	ptDes.y *= c_nMagicTileHeight;
	ptDes.y += c_nMagicTileHeight/2;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \param p : 描述
 * \return 返回值的描述
 */
void cCharacter::OnMagicEffect(stCharacterStruct::EffectEnum e,iMagic* p)
{
	switch(e) 
	{
	case stCharacterStruct::Character_Trans:
		ChangeAlpha(128);
		break;
	case stCharacterStruct::Character_FadeOut:
		{
			stFade st(true);
			st.nEnd = 128;
			Fade(st);
		}
		break;
	case stCharacterStruct::Character_Jump:
		{
			POINT ptDes = p->GetPos();
			GetTileCenter(ptDes);
			SetPos(ptDes);
		}
		break;
	case stCharacterStruct::Character_FadeIn:
		{
			stFade st(false);
			//st.nStart = 128;
			Fade(st);
		}
		break;
	case stCharacterStruct::Character_Image:
		{
			stKeepImageControl st;
			StartKeepImage(st);
			if( m_pRide)
				m_pRide->StartKeepImage(st);
		}
		break;
	case stCharacterStruct::Character_Move:
		SetMoveable(true);
		break;
	case stCharacterStruct::Character_NoMove:
		SetMoveable(false);
		break;
	}
}

/**
 * \brief 在魔法p的条件下，角色的消失效果
 * 
 * 详细描述
 * 
 * \param p : 描述
 * \return 返回值的描述
 */
void cCharacter::OnMagicEnd(iMagic* p)
{
	OnMagicEffect(p->GetCharacterStruct().eEndEffect1,p);
	OnMagicEffect(p->GetCharacterStruct().eEndEffect2,p);
}

/**
 * \brief 　在魔法p的条件下，角色的发射效果
 * 
 * 详细描述
 * 
 * \param p : 描述
 * \return 返回值的描述
 */
void cCharacter::OnMagicBegin(iMagic* p)
{
	OnMagicEffect(p->GetCharacterStruct().eBeginEffect1,p);
	OnMagicEffect(p->GetCharacterStruct().eBeginEffect2,p);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::Hurt()
{
	if (!CanChangeToAction(Ani_Hurt))
		return false;
	m_Animation.ClearKeepImage();
	if( m_pRide )
		m_pRide->ClearKeepImage();
	//	if (!IsMoving())
	//		return false;
	Action(Ani_Hurt);
	if (m_pRide)
		m_pRide->Hurt();
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptDes : 描述
 * \param bServerLogic : 描述
 * \return 返回值的描述
 */
bool cCharacter::Back(POINT ptDes,bool bServerLogic)
{
	if (!bServerLogic && !CanChangeToAction(Ani_Back))
		return false;
	PauseMove(false);
	if(bServerLogic && (GetAction() == Ani_Run || GetAction()==Ani_Walk)) 
	{
		int iiiii=0;
		Stop(this->GetPos());
	}
	return MoveTo(ptDes,Ani_Back,bServerLogic);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::Defend()
{
	/*
	if (!CanChangeToAction(Ani_Defend))
	return false;
	Action(Ani_Defend);
	if (m_pRide)
	m_pRide->Defend();
	//*/
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::DieEnd()
{
	Die();
	m_Animation.ToAniEnd();
	if(m_pRide)m_pRide->m_Animation.ToAniEnd();
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::Die()
{
	if(GetAction() == Ani_Die) return false;
	if (!CanChangeToAction(Ani_Die))
		return false;
	m_Animation.ClearKeepImage();
	if( m_pRide )
		m_pRide->ClearKeepImage();
	Action(Ani_Die);
	if (m_pRide)
		m_pRide->Die();
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::Appear()
{
	Action(Ani_Appear);
	if (m_pRide)
		m_pRide->Appear();
	m_bAppear = true;
	//PlayActionSound(Ani_Appear);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//do action end
//////////////////////////////////////////////////////////////////////////
void cCharacter::UpdateAnimation()
{
	m_Animation.UpdateAnimation();
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param part : 描述
 * \param id : 描述
 * \return 返回值的描述
 */
void cCharacter::ChangePartID(AniPartEnum part, int id)
{
	m_Animation.ChangePartID(part,id);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nAction : 描述
 * \return 返回值的描述
 */
bool cCharacter::CanChangeToAction2(AniTypeEnum nAction)
{
	if (GetCurrentAniPri() > GetAniPri(nAction))
		return false;
	//if (GetCurrentAniPri() > GetAniPri(nAction))
	//	return false;
	return true;
}

#define GET_MIN(X,Y) ((X) < (Y) ? (X) : (Y))

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param nAction : 描述
 * \return 返回值的描述
 */
bool cCharacter::CanChangeToAction(AniTypeEnum nAction)
{

	if (GetCurrentAniPri() >= GetAniPri(nAction))
	{
		//return false;
		///*
		if((nAction == Ani_Back || nAction == Ani_Hurt )
			&& (GetAction() == Ani_Run || GetAction() == Ani_Walk || GetAction() == Ani_Ride_Walk || GetAction() == Ani_Ride_Run ))
		{
			//if(GetDirFrame() != GetFrameOfDir()-1)
			//	return false;
			//return false;
		}
		else
		{
			return false;
		}
		//*/
	}

	if(IsAnyAttackAni(nAction))
	{
		AniTypeEnum curAni = GetAction();
		if(IsAnyAttackAni(curAni))
				return false;

		switch(curAni)
		{
		case Ani_Magic_Stand:
		case Ani_Magic2_Stand:
		case Ani_Magic3_Stand:
		case Ani_Ride_Magic_Stand:
			if ( (int)m_nRun - GetPrevAniEndTime(Ani_Magic) < GetAniDelay(Ani_Magic) )
				return false;
			break;
		case Ani_Attack_Stand:
		case Ani_Attack2_Stand:
		case Ani_Attack3_Stand:
		case Ani_Ride_Attack_Stand:
			if ( (int)m_nRun - GetPrevAniEndTime(Ani_Attack) < GetAniDelay(Ani_Attack) )
			{
				//TRACE( "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Current still in attack stand animation!&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" );
				return false;
			}
			break;
		case Ani_Bow_Stand:
		case Ani_Bow2_Stand:
			if ( (int)m_nRun - GetPrevAniEndTime(Ani_Bow) < GetAniDelay(Ani_Bow) )
				return false;
			break;
		}
		if ( (int)m_nRun - GetPrevAniEndTime(nAction) < GetAniDelay(nAction) )
			return false;
	}
	else
	{
		if ((int)m_nRun - GetPrevAniEndTime(nAction) < GetAniDelay(nAction) )
			return false;
	}

	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param p : 描述
 * \param bJustInRect : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsAt(POINT p, bool bJustInRect)
{
	if(!GetMainAnimation()) 
		return false;

	POINT pt = p;
	POINT pos = GetPos();
	pt -= pos;
	
	if(bJustInRect)
	{
		if(pt.x < - c_nMagicTileWidth/2 || pt.x > c_nMagicTileHeight/2) 
			return false;
	}
	POINT point = GetCenter();
	
	if (GetMainAnimation()->IsAt(pt,point,bJustInRect))
		return true;

	iAnimation* pAni = m_Animation.GetAnimation(Ani_Part_BodySystemColor);
	if (pAni)
	{
		if (pAni->IsAt(pt,point,bJustInRect))
			return true;
	}

	pAni = m_Animation.GetAnimation(Ani_Part_BodyCustomColor);
	if (pAni)
	{
		if (pAni->IsAt(pt,point,bJustInRect))
			return true;
	}

	if (m_pRide)
		return m_pRide->IsAt(p,bJustInRect);
	return false;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::IsReachDes()
{
	float f1 = fabs((float)(m_ptDes.x - m_xPos));
	float f2 = fabs((float)(m_ptDes.y - m_yPos));
	return fabs((float)(m_ptDes.x - m_xPos))<0.01f && fabs((float)(m_ptDes.y - m_yPos))<0.01f;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptMapOffset : 描述
 * \return 返回值的描述
 */
bool cCharacter::AddLight(POINT ptMapOffset)
{
	POINT ptScreen = (POINT)GetPos() - ptMapOffset;
	for_each(m_listMagic.begin(),m_listMagic.end(),bind2nd(mem_fun1<bool,iMagic,POINT>(&iMagic::AddLightAtScreen),ptScreen));
	if (m_pLight)
		return m_pLight->DrawAtScreen(ptScreen);
	return false;
}

/**
 * \brief 每做完这么一个动作x方向(level,水平，呵呵)应该移动的格子数
 * 
 * 详细描述 做完动作n的某个方向的所有帧(完整的做一个动作)的这些时间里应该走过的格子数
 * 
 * \param n : 描述
 * \return 返回值的描述
 */
int cCharacter::GetMoveSpeedLevel(AniTypeEnum n)
{
	iProperty* p = m_pTable->GetProperty(n);
	Assert(p);
	CharacterProperty* pP = (CharacterProperty*) p;
	DWORD dwTime = GetFrameOfDir(n) * pP->stAnimation[0].nAnispeed;  //nAnispeed 每帧多少毫秒
	// nMoveSpeed : pixel / sec 
	float fLevel = (pP->stAniinfo.nMoveSpeed * dwTime)/ (float)(c_nMagicTileWidth * 1000);
	fLevel += 0.5f;
	return (int)fLevel;
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 描述
 * \return 返回值的描述
 */
float cCharacter::GetMoveSpeed(AniTypeEnum n)
{
	if (IsRide())
	{
		n = GetAniRideAction(n);
	}
	
	int fspeed = m_aAni[n].nSpeed;

	if (fspeed == 0)
		return GetAniInfo(n).nMoveSpeed;

	return m_aAni[n].fMoveSpeed;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptPos : 描述
 * \return 返回值的描述
 */
POINT cCharacter::GetMagicPosition(POINT ptPos)
{
	return ptPos + GetMagicAdjust();
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT cCharacter::GetMagicPosition()
{
	return GetMagicPosition(GetPos());
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT cCharacter::GetMagicAdjust()
{
	stPointI pt(0,0);
	pt.y = GetAnimationStruct(GetAction()).nMagicAdjust;
	return pt;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::IsWait()
{
	if (m_bPauseMove)
		return true;
	AniTypeEnum ani = GetAction();
	switch(ani) {
		//case Ani_Idle:
	case Ani_Wait:
	case Ani_Ride_Wait:
	case Ani_Hurt:
	case Ani_Sit:
		//case Ani_Ride_Idle:
		break;
	default:
		return false;
	}
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param monsterID : 描述
 * \return 返回值的描述
 */
bool cCharacter::SetRide(int monsterID)
{
	if (monsterID == 0 && m_pRide == NULL)
		return true;
	if (monsterID != 0 && m_pRide != NULL)
		return true;

	if(m_pRide)
	{
		m_pRide->Release();
		m_pRide = NULL;
	}
	if ( monsterID != 0 )
	{
		m_pRide =(cCharacter*) Character_CreateCharacter();
		vector<int> aID;
		aID.push_back( monsterID );
		m_pRide->CreateCharacter(0,aID,false);
		m_pRide->SetPos( GetPos() );
		for(size_t i=0;i<Ani_Num;++i)
		{
			m_pRide->m_aAni[i].nDelayTime = m_aAni[i].nDelayTime;
			m_pRide->m_aAni[i].nSpeed = m_aAni[i].nSpeed;
			m_pRide->m_aAni[i].fMoveSpeed = m_aAni[i].fMoveSpeed;
		}

	}
	if (IsMoving())
		MoveTo(GetDes(),GetAction(),true);
	else
		Action(GetAction(),true);

	TurnTo(GetDir());
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptDes : 描述
 * \param maxDir : 描述
 * \return 返回值的描述
 */
int cCharacter::GetDir(POINT ptDes,int maxDir)
{
	POINT pt = ptDes;
	pt -= GetPos();
	int dir = GetDir8(pt.x,pt.y);
	return dir;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptScreen : 描述
 * \param e : 描述
 * \param nDir : 描述
 * \return 返回值的描述
 */
bool cCharacter::DrawMagicDir(POINT ptScreen,int e,int nDir)
{
	if ( !GetMapScene()->IsNeedDrawMagicEffect() )
		return false;

	if (m_listMagic.empty())
		return false;

	if ( m_bDie )
		return false;

	m_ptMagicPos = ptScreen;
	if ( IsRide() )
		m_ptMagicPos.y -= 20;

	POINT ptMagic = GetMagicPosition(ptScreen);
	bool bDraw = false;
	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end(); ++it)
	{
		iMagic* p = *it;
		if ( p->GetProperty()->stMove.eMove == stMoveDataStruct::Move_Static )
		{
			if (p->DrawSort(m_ptMagicPos,ptMagic,(stMoveStruct::SortEnum)e,nDir))		//ptScreen
				bDraw = true;
		}
		else
		{
			if (p->DrawSort(ptScreen,ptMagic,(stMoveStruct::SortEnum)e,nDir))		//ptScreen
				bDraw = true;
		}
	}
	return bDraw;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptOffset : 描述
 * \return 返回值的描述
 */
bool cCharacter::DrawMagicLowest(POINT ptOffset)
{
	if ( !GetMapScene()->IsNeedDrawMagicEffect() )
		return false;

	if (m_listMagic.empty())
		return false;
	POINT ptPosition = GetPos();
	ptPosition -= ptOffset;
	return DrawMagicDir(ptPosition,stMoveStruct::Sort_Lowest,-1);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ptOffset : 描述
 * \return 返回值的描述
 */
bool cCharacter::DrawMagicFrontest(POINT ptOffset)
{
	if ( !GetMapScene()->IsNeedDrawMagicEffect() )
		return false;

	if (m_listMagic.empty())
		return false;
	POINT ptPosition = GetPos();
	ptPosition -= ptOffset;
	return DrawMagicDir(ptPosition,stMoveStruct::Sort_Frontest,-1);
}

bool cCharacter::DrawMagicAboveAll(POINT ptOffset)
{
	if ( !GetMapScene()->IsNeedDrawMagicEffect() )
		return false;

	if (m_listMagic.empty())
		return false;
	POINT ptPosition = GetPos();
	ptPosition -= ptOffset;
	return DrawMagicDir(ptPosition,stMoveStruct::Sort_AboveAll,-1);
}
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param szSound : 描述
 * \return 返回值的描述
 */
void cCharacter::PlayHitSound(LPCSTR szSound)
{
	PlayCharacterSound(NULL,GetPos(),GetMoveSpeed(),Sound_Character,0,szSound,false,IsMainCharacter());
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 动作号
 * \param num : 第几个动作,总为０
 * \return 返回值的描述
 */
const stAnimationStruct& cCharacter::GetAnimationStruct(int n,int num)
{
	iProperty* p = m_pTable->GetProperty(n);
	Assert(p);
	CharacterProperty* pP = (CharacterProperty*) p;
	return pP->stAnimation[num];
};

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 动作号
 * \return 返回值的描述
 */
bool cCharacter::HaveAniStruct(int n)
{
	return (NULL != m_pTable->GetProperty(n));
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 描述
 * \return 返回值的描述
 */
const stAniInfo& cCharacter::GetAniInfo(int n)
{
	iProperty* p = m_pTable->GetProperty(n);
	//Assert(p);
	if ( p )
	{
		CharacterProperty* pP = (CharacterProperty*) p;
		return pP->stAniinfo;
	}
	else
	{
		static stAniInfo aniInfo;		
		return aniInfo;
	}
	
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 魔法n
 * \param i : 第i个事件
 * \return 返回值的描述
 */
const stEvent& cCharacter::GetEvent(int n,int i)
{
	Assert(i>=0 && i<MAX_EVENT);
	iProperty* p = m_pTable->GetProperty(n);
	Assert(p);
	CharacterProperty* pP = (CharacterProperty*) p;
	return pP->aEvent[i];
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
bool cCharacter::IsLight()
{
	bool bLight = false;
	if (m_pLight)
		bLight = m_pLight->IsEnable();
	return bLight;
}
/*
void cCharacter::OnHit(int magic, POINT ptKillerPos)
{
if (magic <= 0)
return;
POINT pt = ptKillerPos;
pt -= GetPos();
int dir = GetDir8(pt.x,pt.y);
if (IsMagicEffect(magic))
{
SetMagicEffect(magic,0);
return;
}
}
*/

/**
* \brief 回到普通速度
* 
* 详细描述
* 
* \param e : 描述
* \return 返回值的描述
*/
void cCharacter::ResetSpeed() 
{
	m_fRunSpeed = 1.f;
	for (int i=0; i<Ani_Num; i++)
	{
		m_aAni[i].nDelayTime = 0;
		m_aAni[i].nSpeed = 0;
	}
	return;
}

/**
* \brief
* 
* 详细描述
* 
* \param e : 动作
* \param ms: 新的每帧所要用的毫秒数
* \return 返回值的描述
*/
bool cCharacter::ChangeSpeed( AniTypeEnum e, float ms )
{
	m_aAni[e].nSpeed = ms;
	int n = GetFrameOfDir(e);
	if(n == 0) n = 8;
	if(e == Ani_Walk || e == Ani_Run || e == Ani_Ride_Run || e == Ani_Ride_Walk)
	{
		int nMinTime =(16 * 20)/n;
		if(m_aAni[e].nSpeed < nMinTime) m_aAni[e].nSpeed = nMinTime;
		m_aAni[e].fMoveSpeed = ( GetMoveSpeedLevel(e) * c_nMagicTileWidth * 1000 / ( ms * GetFrameOfDir(e) ) );
	}
	return false;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \return 返回值的描述
 */
float cCharacter::GetAniSpeed( AniTypeEnum e)
{
	if(m_aAni[e].nSpeed) return m_aAni[e].nSpeed;
	return GetAnimationStruct(e).nAnispeed;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \return 返回值的描述
 */
//去掉某个类型的所有魔法
void cCharacter::RemoveMagic(MagicTypeEnum e)
{
	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end();)
	{
		iMagic* p = (*it);
		if (p->GetMagicType() == e)
		{
			//p->RemoveSubMagic();
			p->Release();
			it = m_listMagic.erase(it);
#ifdef _DEBUG_MAGIC
			cMagic * pMagic = (cMagic *)p;
			LPCSTR lpName = pMagic->m_stProperty.GetName();
			TRACE("人物已经删除跟随魔法%s\n",lpName);
#endif
#ifdef _GAME_TOOL
			GetRenderDump()->RemoveWatchMagic(p);
#endif

		}
		else
			++it;
	}
	return;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param magicType : 描述
 * \param magicTable : 描述
 * \return 返回值的描述
 */
void cCharacter::RemoveAMagic(size_t magicType,size_t magicTable)
{
	int nMagicID[1024];
	int nMagicCount = 0;
	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end();)
	{
		iMagic* p = (*it);
		if (p->GetType() == magicType && p->GetTable() == magicTable)
		{
			MagicProperty* pro = p->GetProperty();

			for(size_t i=0;i<MAX_GENERATE;i++)
			{
				if(pro->aGenerate[i].strGenerateType[0])
				{
					nMagicID[nMagicCount++] = Magic_GetMagicID(pro->aGenerate[i].strGenerateType,this->GetMagicTableID());
				}
			}
#ifdef _DEBUG_MAGIC
			cMagic * pMagic  = (cMagic *)p;
			LPCSTR lpName = pMagic->m_stProperty.GetName();
			TRACE("人物已经删除跟随魔法%s\n",lpName);
#endif
			p->RemoveSubMagic();
			p->Release();
			it = m_listMagic.erase(it);
#ifdef _GAME_TOOL
			GetRenderDump()->RemoveWatchMagic(p);
#endif
		}
		else
		{
			++it;
		}
	}

	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end();)
	{
		iMagic* p = (*it);
		bool bFind = false;
		for(size_t i=0;i<nMagicCount;++i)
		{	
			if(p->GetType() == nMagicID[i] && p->GetTable() == magicTable)
			{
#ifdef _DEBUG_MAGIC
				cMagic * pMagic  = (cMagic *)p;
				LPCSTR lpName = pMagic->m_stProperty.GetName();
				TRACE("人物已经删除跟随魔法%s\n",lpName);
#endif
				p->RemoveSubMagic();
				p->Release();
				it = m_listMagic.erase(it);
#ifdef _GAME_TOOL
				GetRenderDump()->RemoveWatchMagic(p);
#endif
				bFind = true;
				break;
			}
		}

		if(!bFind)
		{
			++it;
		}
	}
	return;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param magicType : 魔法id
 * \param nSetLife : 描述
 * \param table : 魔法属性表id
 * \return 返回值的描述
 */
bool cCharacter::SetMagic(size_t magicType,int nSetLife,int table)
{
	if (magicType < 0)
		return false;
	iMagic* pMagic = Magic_CreateMagic(magicType,table);
	if (!pMagic)
		return false;
	pMagic->CreateMagic(IsLight());
	pMagic->SetLife(nSetLife);
	pMagic->SetSender(this);
	pMagic->SetPos(this->GetPos());
	AddMagic(pMagic);
	OnMagicBegin(pMagic);

	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param magicType : 描述
 * \param nSetLife : 描述
 * \param table : 描述
 * \return 返回值的描述
 */
//简便的效果方式
bool cCharacter::SetMagicEffect(size_t magicType,int nSetLife,int table)
{
	return SetMagic(magicType,nSetLife,table);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param szAni : 描述
 * \return 返回值的描述
 */
bool cCharacter::SetMagicEffect(LPCSTR szAni)
{
	if (szAni[0] == 0)
		return false;
	if (strcmp(szAni,"null") == 0)
		return false;

	iMagic* pMagic = Magic_CreateMagic("命中效果",0);
	if (!pMagic)
		return false;
	pMagic->CreateMagic(IsLight(),szAni);
	pMagic->SetSender(this);
	AddMagic(pMagic);
	return true;
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param magicType : 描述
 * \param magicTable : 描述
 * \return 返回值的描述
 */
iMagic* cCharacter::FindMagic(size_t magicType,size_t magicTable)
{
	for (vector<iMagic*>::iterator it = m_listMagic.begin(); it != m_listMagic.end();++it)
	{
		size_t type = (*it)->GetType();
		size_t table = (*it)->GetTable();
		if (type == magicType && table == magicTable)
			return (*it);
	}
	return NULL;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param p : 描述
 * \return 返回值的描述
 */
bool cCharacter::AddMagicRing(iMagic* p)
{
	RemoveAMagic(p->GetType(),p->GetTable());
	AddMagic(p);
	return true;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param magicType : 描述
 * \param nSetLife : 描述
 * \param table : 描述
 * \return 返回值的描述
 */
bool cCharacter::SetMagicRing(size_t magicType,int nSetLife,int table)
{
	iMagic* pMagic = FindMagic(magicType,table);
	if (pMagic)
	{
		pMagic->SetLife(nSetLife);
		pMagic->SetInitLife();
		return true;
	}
	return SetMagic(magicType,nSetLife,table);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \param magicType : 描述
 * \param table : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsMagic( MagicTypeEnum e,size_t magicType,int table)
{
	int id = table;
	if (id < 0)
		id = GetMagicTableID();
	MagicProperty* p = Property_GetTableManager()->GetMagicProperty(magicType,id);
	if (p == NULL)
		return false;
	return p->stMagic.eType == e;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
int Character_InitCharacter()
{
	cCharacter::InitOrder();
	return 0;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
EXPORT_API iCharacter* Character_CreateCharacter()
{
#ifdef _DEBUG
	s_bNewCharacter = true;
#endif
	//cCharacter* p = LOOKASIDE_ALLOC(s_allocCharacter);
	cCharacter* p = new cCharacter;
#ifdef _DEBUG
	s_bNewCharacter = false;
#endif
	return p;
	//return new cCharacter;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
int cCharacter::GetImageHeight()
{
	POINT p = GetImageOffset();
	POINT pt = GetDefaultCenter();
	int h = - pt.y - p.y;
	return h;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT cCharacter::GetImageOffset()
{
	POINT r={0,0};
	iAnimation* p = GetMainAnimation();
	if (p)
	{
		IBitmap* pTex = p->GetTexture();
		if (pTex)
			return pTex->GetClipOffset();
	}
	return r;
}

void cCharacter::SetDefaultCenter( stPointI ptCenter )
{
	m_ptCenter = ptCenter;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
POINT cCharacter::GetDefaultCenter()
{
	stPointI ptCenter = stPointI(-1,-1);
	iAnimation* pAni = GetMainAnimation();
	if (pAni == NULL)
		return ptCenter;
	int w=0;
	int h=0;
	if (!pAni->GetWholeSize(w,h))
		return ptCenter;
	if (w > 1024 || h > 1024)
	{
		Assert(0);
	}
	else if (w > 512 || h > 512)
	{
		ptCenter.x = -512;
		ptCenter.y = -512-22; //662
	}
	else if (w > 256 || h > 256)
	{
		ptCenter.x = -256;
		ptCenter.y = -256-22; //278 -- 308??
	}
	else
		ptCenter = stPointI(-128,-150);
	return ptCenter;
}



/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
HRESULT cCharacter::InvalidateDeviceObjects()
{
	m_Animation.InvalidateDeviceObjects();
	std::for_each(m_listMagic.begin(),m_listMagic.end(),std::mem_fun<HRESULT,iMagic>(&iMagic::InvalidateDeviceObjects));
	std::for_each(m_listGenerate.begin(),m_listGenerate.end(),std::mem_fun<HRESULT,iMagic>(&iMagic::InvalidateDeviceObjects));
	if (m_pRide)
		m_pRide->InvalidateDeviceObjects();
	return S_OK;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \return 返回值的描述
 */
HRESULT cCharacter::RestoreDeviceObjects()
{
	m_Animation.RestoreDeviceObjects();
	std::for_each(m_listMagic.begin(),m_listMagic.end(),std::mem_fun<HRESULT,iMagic>(&iMagic::RestoreDeviceObjects));	
	std::for_each(m_listGenerate.begin(),m_listGenerate.end(),std::mem_fun<HRESULT,iMagic>(&iMagic::RestoreDeviceObjects));	
	if (m_pRide)
		m_pRide->RestoreDeviceObjects();
	return S_OK;
}

POINT cCharacter::GetCenter()
{
	if (m_ptCenter == stPointI(-1,-1))
		m_ptCenter = GetDefaultCenter();
	return m_ptCenter;
}
 
/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param pt : 描述
 * \param rc : 描述
 * \return 返回值的描述
 */
bool cCharacter::GetDrawRect(POINT pt,RECT & rc)
{
	return m_Animation.GetDrawRect(pt,GetCenter(),rc);
	/*
	iAnimation* pAni = GetMainAnimation();
	if(!pAni) return false;
	int w,h;
	if(!pAni->GetWholeSize(w,h)) return false;
	POINT ptCenter = GetCenter();
	rc.left = pt.x + ptCenter.x;
	rc.top = pt.y + ptCenter.y;
	rc.right = rc.left + w;
	rc.bottom = rc.top + h;
	return true;
	//*/
}



/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ani : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsExistAction(AniTypeEnum ani)
{
	iAnimation* pMainAni = GetMainAnimation();
	if(!pMainAni) return false;
	return pMainAni->IsExistAction(GetAnimationStruct(ani,0).nGroup);
}

/**
 * \brief 是否光环魔法
 * 
 * 详细描述
 * 
 * \param magicType : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsMagicRing(size_t magicType)
{
	MagicProperty* p = Property_GetTableManager()->GetMagicProperty(magicType,GetMagicTableID());
	return (p && p->stMagic.eType == Magic_Ring);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param magicType : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsMagicAttackRing(size_t magicType)
{
	MagicProperty* p = Property_GetTableManager()->GetMagicProperty(magicType,GetMagicTableID());
	return (p && p->stMagic.eType == Magic_Attack_Ring);
}


/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \return 返回值的描述
 */
int cCharacter::GetAniDelay(AniTypeEnum e)
{
	if(IsAttackAni((AniTypeEnum)e)) e = Ani_Attack;
	else if(IsMagicAni((AniTypeEnum)e)) e = Ani_Magic;
	else if(IsBowAni((AniTypeEnum)e)) e = Ani_Bow;

	return m_aAni[e].nDelayTime;

	/*if (m_aAni[e].nDelayTime)
		return m_aAni[e].nDelayTime;
	return GetWaitSpeed(e);*/
}

int cCharacter::GetWaitSpeed(AniTypeEnum e){
	if(IsAttackAni((AniTypeEnum)e)) e = Ani_Attack;
	else if(IsMagicAni((AniTypeEnum)e)) e = Ani_Magic;
	else if(IsBowAni((AniTypeEnum)e)) e = Ani_Bow;

	return GetAniInfo(e).nDelay;//间隔时间
}

/**
 * \brief 改变某个动作的等待时间,也就是必须过这么多时间后才能再利用这个动作
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \param ms : 描述
 * \return 返回值的描述
 */
bool cCharacter::ChangeWaitSpeed(AniTypeEnum e, int ms)
{
	if(IsAttackAni((AniTypeEnum)e)) e = Ani_Attack;
	else if(IsMagicAni((AniTypeEnum)e)) e = Ani_Magic;
	else if(IsBowAni((AniTypeEnum)e)) e = Ani_Bow;

	m_aAni[e].nDelayTime = ms;
	return false;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \return 返回值的描述
 */
int cCharacter::GetPrevAniEndTime(int e)
{
	if(IsAttackAni((AniTypeEnum)e)) e = Ani_Attack;
	else if(IsMagicAni((AniTypeEnum)e)) e = Ani_Magic;
	else if(IsBowAni((AniTypeEnum)e)) e = Ani_Bow;

	return m_aAni[e].nPrevAniEndTime;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param e : 描述
 * \param n : 描述
 * \return 返回值的描述
 */
void cCharacter::SetPrevAniEndTime(int e,int n)
{
	if(IsAttackAni((AniTypeEnum)e)) e = Ani_Attack;
	else if(IsMagicAni((AniTypeEnum)e)) e = Ani_Magic;
	else if(IsBowAni((AniTypeEnum)e)) e = Ani_Bow;
	m_aAni[e].nPrevAniEndTime = n;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param time_eslasped : 描述
 * \return 返回值的描述
 */
void cCharacter::UpdatePrevAniEndTime(long time_eslasped)
{
	AniTypeEnum e = GetAction();

	if(IsAttackAni((AniTypeEnum)e)) e = Ani_Attack;
	else if(IsMagicAni((AniTypeEnum)e)) e = Ani_Magic;
	else if(IsBowAni((AniTypeEnum)e)) e = Ani_Bow;

	m_aAni[e].nPrevAniEndTime += time_eslasped;
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param n : 描述
 * \return 返回值的描述
 */
bool cCharacter::IsHasAniImage(AniTypeEnum n)
{
	return m_Animation.IsExistAction(GetAnimationStruct(n).nGroup);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param ani1 : 描述
 * \param ani2 : 描述
 * \param ani3 : 描述
 * \return 返回值的描述
 */
AniTypeEnum cCharacter::GetRandAni(AniTypeEnum ani1,AniTypeEnum ani2,AniTypeEnum ani3)
{
	AniTypeEnum ani[3];
	int num = 0;
	if(ani1 != Ani_Null && IsHasAniImage(ani1))
		ani[num++] = ani1;

	if(ani2 != Ani_Null && IsHasAniImage(ani2))
		ani[num++] = ani2;

	if(ani3 != Ani_Null && IsHasAniImage(ani3))
		ani[num++] = ani3;

	if(num == 0) return Ani_Null;
	return ani[rand() % num];
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param iGroup : 描述
 * \param frame : 描述
 * \return 返回值的描述
 */
bool CharacterIsHasAni(int iGroup,int frame)
{
	static IBitmapList* pList = GetDevice()->FindResource("data\\body.gl");

	IBitmaps * p = pList->GetBitmaps(iGroup,frame);
	return p->GetFrameCount() != 0;
}

/**
* \brief 简短描述
* 
* 详细描述
* 
* \param fSpeed : 完整作某个动作(播完某个动作的某个方向的所有帧)所需要的时间 / 正常运动时间
* \return 返回值的描述
*/
void cCharacter::SetMoveSpeed( float fSpeed )
{
	int level = GetMoveSpeedLevel(Ani_Walk);
	iProperty* p = m_pTable->GetProperty(Ani_Walk);
	Assert(p);
	CharacterProperty* pP = (CharacterProperty*) p;
	SetMoveSpeed( (DWORD)(fSpeed * (level * c_nMagicTileWidth * 1000)/ pP->stAniinfo.nMoveSpeed) );
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param dwAniTime : 完整作某个动作(播完某个动作的某个方向的所有帧)所需要的时间
 * \return 返回值的描述
 */
void cCharacter::SetMoveSpeed( DWORD dwAniTime )
{
	ChangeSpeed(Ani_Walk , (float)dwAniTime/GetFrameOfDir(Ani_Walk) );			//  * 16.0f / (float)GetFrameOfDir(Ani_Walk) 
	ChangeSpeed(Ani_Run , (float)dwAniTime/GetFrameOfDir(Ani_Run));
	ChangeSpeed(Ani_Ride_Walk , (float)dwAniTime/GetFrameOfDir(Ani_Ride_Walk));
	ChangeSpeed(Ani_Ride_Run , (float)dwAniTime/GetFrameOfDir(Ani_Ride_Run));
	if(IsRide()) m_pRide->SetMoveSpeed(dwAniTime);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param percentIntervalTime : 描述
 * \return 返回值的描述
 */
void cCharacter::SetAttackSpeed(float percentIntervalTime)
{
	AniTypeEnum ani[] = { Ani_Attack , Ani_Attack2 , Ani_Attack3 , Ani_Ride_Attack , Ani_Bow , Ani_Bow2 };
	AniTypeEnum ani1[] = { Ani_Attack_Stand , Ani_Attack2_Stand , Ani_Attack3_Stand , Ani_Ride_Attack_Stand , Ani_Bow_Stand , Ani_Bow2_Stand };
	for(size_t i=0;i<count_of(ani);++i)
	{
		iProperty* p = m_pTable->GetProperty(ani[i]);
		CharacterProperty* pP = (CharacterProperty*) p;

		DWORD dwTime = pP->stAniinfo.nDelay * percentIntervalTime;
		ChangeWaitSpeed(ani[i],dwTime);
		int n = GetFrameOfDir(ani1[i]);
		if(n == 0) n = 6;

		ChangeSpeed( ani1[i],max(1,dwTime / n) );
	}
	if(IsRide()) m_pRide->SetAttackSpeed(percentIntervalTime);
}

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 * \param percentIntervalTime : 描述
 * \return 返回值的描述
 */
void cCharacter::SetMagicSpeed(float percentIntervalTime)
{
	AniTypeEnum ani[] = {Ani_Magic,Ani_Magic2,Ani_Magic3,Ani_Ride_Magic};
	AniTypeEnum ani1[] = {Ani_Magic_Stand,Ani_Magic2_Stand,Ani_Magic3_Stand,Ani_Ride_Magic_Stand};
	for(size_t i=0;i<count_of(ani);++i)
	{
		iProperty* p = m_pTable->GetProperty(ani[i]);
		Assert(p);
		CharacterProperty* pP = (CharacterProperty*) p;
		DWORD dwTime = pP->stAniinfo.nDelay * percentIntervalTime;

		if ( 0 )			//IsMainCharacter()
		{
			//ChangeWaitSpeed(ani[i],dwTime);//改变间隔时间
			//When active attack we don't need any wait ani
			ChangeWaitSpeed(ani[i],10);//改变间隔时间
			int n = GetFrameOfDir(ani1[i]);
			if(n == 0) n = 8;
			ChangeSpeed( ani1[i],max(1,dwTime/n) );  // 改变动作ani1[i]的移动速度(至少１ms)
			//We make magic ani speed slower some externt for active attack
			ChangeSpeed( ani[i], max(1,dwTime*1.42f/n) );
		}
		else
		{
			ChangeWaitSpeed(ani[i],dwTime);//改变间隔时间
			int n = GetFrameOfDir(ani1[i]);
			if(n == 0) n = 8;
			ChangeSpeed( ani1[i],max(1,dwTime/n) );  // 改变动作ani1[i]的移动速度(至少１ms)
		}		
	}
	if(IsRide()) m_pRide->SetMagicSpeed(percentIntervalTime);
}

void cCharacter::SetRunAniSpeed(float newms)
{
	//m_aAni[Ani_Run].nOldSpeed = m_aAni[Ani_Run].nSpeed;
	//m_aAni[Ani_Run].fOldMoveSpeed  = m_aAni[Ani_Run].fMoveSpeed;
    
	ChangeSpeed(Ani_Run,newms);
}

void cCharacter::RestoreRunAniSpeed()
{
	//m_aAni[Ani_Run].nSpeed = m_aAni[Ani_Run].nOldSpeed;
	//m_aAni[Ani_Run].fMoveSpeed  = m_aAni[Ani_Run].fOldMoveSpeed;

	ChangeSpeed((AniTypeEnum)Ani_Run,(float)m_aAni[Ani_Run].nSpeed);
   
}
bool cCharacter::FindAndAddMagic(iMagic * p)
{
	if( !p->IsHasChild()) 
	{

		int iCount = 0;
		for(std::vector <iMagic*>::iterator  iter = m_listMagic.begin(); iter != m_listMagic.end() ; iter++)
		{
			iMagic * pMagic = *iter;
			if( 0 == strcmp( pMagic->GetProperty()->GetName(),p->GetProperty()->GetName()) )
			{
				//(*iter)->Release();
				//SimpleRemoveFromArray(m_listMagic,iter-m_listMagic.begin());
				if( ++iCount >= 3)
				     return false;
			} 
		}
	}	
	AddMagic(p);
	return true;
}

bool cCharacter::CanChangeRideState()
{
	AniTypeEnum ani = GetAction();
	if(Ani_Wait == ani || Ani_Ride_Wait == ani)
		return true;
	//if(IsMoving() && m_xSpeed == 0 && m_ySpeed == 0)
	//	return true;
	return false;
}
