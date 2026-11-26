#include "../engine/include/engine.h"
#include ".\footprintmanager.h"
#include "public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param szFileName : ÃèÊö
 * \param group : ÃèÊö
 * \param frame : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void stFootprint::SetImage(const char* szFileName,int group,int frame)
{
	IBitmapList* p = ::GetDevice()->FindResource(szFileName,c_nMagicResourceFlags,Usage_Npc);
	assert(p);
	if(!p) return;

	m_pTex = p->LoadFrame(0,group,frame);
	assert(m_pTex);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CFootprintManager::CFootprintManager(void)
{
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CFootprintManager::~CFootprintManager(void)
{
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CFootprintManager::Run()
{
	DWORD time = xtimeGetTime();
	const int da = 80;
	
	for(std::list<stFootprint>::iterator it = m_footprints.begin(); it != m_footprints.end();)
	{
		int alpha = 255 - (time-(*it).time) * da / 1000;
		if(alpha <= 0)
		{
			it = m_footprints.erase(it);
		}
		else
		{
			(*it).SetAlpha(alpha);
			++it;
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param ptMapOffset : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CFootprintManager::Draw(const POINT & ptMapOffset)
{
	const POINT ptCenter = {-128,-150};
	for(std::list<stFootprint>::iterator it = m_footprints.begin(); it != m_footprints.end();++it)
	{
		if((*it).m_pTex)
		{
			stPointF pt((*it).pt.x + ptCenter.x - ptMapOffset.x,(*it).pt.y + ptCenter.y - ptMapOffset.y);
			(*it).m_pTex->Render(pt.x,pt.y,(*it).color);
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CFootprintManager& GetFootprintManager()
{
	static CFootprintManager fm;
	return fm;
}