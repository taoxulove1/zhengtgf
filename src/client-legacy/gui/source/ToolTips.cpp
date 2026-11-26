/**
 * \file      ToolTips.cpp
 * \version   $Id$
 * \author    $$$03¹«Ë¾@###.com
 *           $$$03¹«Ë¾@###.com
 * \date      2005-07-05 17:02:57
 * \brief     ¹¤¾ßÌáÊ¾
 * 
 *	      ¹¤¾ßÌáÊ¾
 */

#include "../../engine/include/engine.h"
#include "../include/tooltips.h"
#include "../../engine/include/stack_alloc.h"
#include "../../engine/include/stringex.h"
#include <typeinfo>
#include "../../gui/include/guiTypes.h"


const stPointI s_ptTipsBorderGrpLeftTop(9,9);
const stPointI s_ptTipsBorderGrpRightBot(9,9);

static IBitmaps*	s_pBmp[9];
static stRectI		s_borderRect[9];
const int c_nDefTipFont = 1;

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param x : ÃèÊö
 * \param y : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::stTipTextCell::Render(int x,int y)
{
	int iOldFont = GetDevice()->SetFont(iFont);
	GetDevice()->DrawString(szText.c_str(),stPointI(x,y),color,eFontEffect,colorShadow);
	GetDevice()->SetFont(iOldFont);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param x : ÃèÊö
 * \param y : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::stTipImageCell::Render(int x,int y)
{
	image.Render(x,y);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param x : ÃèÊö
 * \param y : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::stTipLine::Render(int x,int y)
{
	for(size_t i=0;i<cells.size();++i)
	{
		cells[i]->Render(x,y);
		x += cells[i]->width;
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CToolTips::CToolTips(void)
: m_curColor(0xff000000)
, m_curColorShadow(0xff000000)
, m_dwBorderColor(0xff000000)
//, m_curFontEffect(FontEffect_Border)
, m_curFontEffect(FontEffect_None)
, m_width(0)
, m_height(0)
, m_dwBkColor(COLOR_ARGB(180,0,0,0))
{
	m_lines.reserve(32);
	m_alignType = GUI_ALIGN_HCENTER;

	m_gaps = 0;
	m_bShowBorderTex = true;
	m_bUseCustomeBkColor = false;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
CToolTips::~CToolTips(void)
{
	Clear();
}

void CToolTips::Clear()
{ 
	for(size_t i=0;i<m_lines.size();++i)
	{
		delete m_lines[i];
	}
	m_lines.resize(0);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::Resize()
{
	m_width = 0;
	m_height = 0;
	for(size_t i=0;i<m_lines.size();++i)
	{
		if(m_lines[i]->width > m_width) m_width = m_lines[i]->width;
		m_height += m_lines[i]->height;
	}

	m_height += m_lines.size() * m_gaps;

	//Add tips border texture size
	m_width += s_ptTipsBorderGrpLeftTop.x+s_ptTipsBorderGrpRightBot.x;
	m_height += s_ptTipsBorderGrpLeftTop.y+s_ptTipsBorderGrpRightBot.y;

	//union back color
	m_dwBkColor = COLOR_ARGB(180,0,0,0);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::InitBorderTexture()
{
	stResourceLocation rl;
	rl.SetFileName( GetGuiGraphicPackName() );
	rl.group = 0;
	rl.frame = 27;

	stRectI rcTexture;
	stRectI rects[9];

	rcTexture.SetRect(0,0,37,38);

	CuttRect9(rcTexture,s_ptTipsBorderGrpLeftTop,s_ptTipsBorderGrpRightBot,rects);
	for(size_t i=0;i<9;++i)
	{
		s_pBmp[i] = GetDevice()->FindBitmaps( &rl );
		s_borderRect[i] = rects[i];
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param pszDest : ÃèÊö
 * \param nLineLen : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
const char* str_split_begin(const char* pszSrc,char* pszDest,int nLineLen)
{
	size_t i;
	i =0;

	char * pstrDst = pszDest;

	while(*pszSrc)
	{
		if((*pszSrc) & 0x80)
		{
			if(!(*(pszSrc+1))) break;

			*pstrDst ++ = *pszSrc++;
			*pstrDst ++ = *pszSrc++;
			i+=2;

			if(i >= nLineLen)
			{
				// »»ĞĞ
				break;
			}
		}
		else
		{
			size_t iCharNum;
			if(IsNewLine(pszSrc,iCharNum))
			{
				// »»ĞĞ
				pszSrc += iCharNum;
				break;
			}
			else
			{
				*pstrDst ++ = *pszSrc++;
				++i;
				if( i >= nLineLen)
				{
					// »»ĞĞ
					break;
				}
			}
		}
	}
	*pstrDst = 0;
	return pszSrc;
}


/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszText : ÃèÊö
 * \param iFont : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::AddText(const char* pszText,int iFont)
{
	size_t icharnum = 0;
	stTipLine* pLine = NULL;
	if(m_lines.empty() || IsNewLine(pszText,icharnum))
	{
		m_lines.push_back(new stTipLine);
		pszText += icharnum;
	}
	pLine = m_lines.back();

	const int maxWidth=40;
	const int iFontWidth = 6;
	//*
	SetFrameAllocator fa;
	std::vector<std::stack_string,std::stack_allocator<std::stack_string> > vString;

	int len1 = maxWidth - pLine->width/iFontWidth;
	if(len1 > 0)
	{
		char szBuf[256];
		pszText = str_split_begin(pszText,szBuf,len1);
		vString.push_back(szBuf);
	}
	else
	{
		m_lines.push_back(new stTipLine);
		pLine = m_lines.back();
	}

	str_split(pszText,vString, maxWidth );		//	200/6 

	for(size_t i=0;i<vString.size();++i)
	{
		if(pLine == NULL)
		{
			m_lines.push_back(new stTipLine);
			pLine = m_lines.back();
		}
		pLine->AddText(vString[i].c_str(),iFont,m_curColor,m_curColorShadow,m_curFontEffect);
		pLine = NULL;
	}
	//*/
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param rl : ÃèÊö
 * \param bNextLine : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::AddAnimation(const stResourceLocation *rl,bool bNextLine)
{
	if(bNextLine || m_lines.empty())
	{
		m_lines.push_back(new stTipLine);
	}
	stTipLine* pLine = m_lines.back();
	pLine->AddAnimation(rl);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param x : ÃèÊö
 * \param y : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::RenderBorderTextuer( int x,int y )
{
	stRectI rcBack( x, y, x+m_width, y+m_height );
	stRectI rects[9];
	CuttRect9(rcBack,s_ptTipsBorderGrpLeftTop,s_ptTipsBorderGrpRightBot,rects);
	for(size_t i=1;i<9;++i)
	{
		PatchRenderImage(rects[i],s_pBmp[i],&s_borderRect[i],-1,Blend_Null,s_PatchAlignFormat[i]);
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param rcBBox : ÃèÊö
 * \param devWidth : ÃèÊö
 * \param devHeight : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------------
//Give tips some EQ
//First make sure tips full visible
//Then we make sure tips not on item
//
//	reBBox	: control bounding box(left,top,right,bottom) ,related to screen pixel pos
//	devWidth : limited range width
//	devHeight : limited range height
//
//-------------------------------------------------------------------------------
void CToolTips::RenderTopLeftPrefer( stRectI rcBBox, int devWidth, int devHeight )
{
	//Get tips position
	int x,y;
	x = rcBBox.left - m_width;
	y = rcBBox.top - m_height;

	//Adjust to valid
	if( x < 0 )
		x = rcBBox.right;
	if( x + m_width > devWidth )		//Make sure visible
		x = devWidth -m_width;

	if( y < 0 )
		y = rcBBox.bottom;
	if ( y + m_height > devHeight )		//make sure visible
		y = devHeight - m_height;

	//Call render
	//Render( x, y );

	//union back color
	m_dwBkColor = COLOR_ARGB(180,0,0,0);
	if ( NULL == GetGuiManager()->GetToolTips() )
		GetGuiManager()->SetToolTips( this, stPointI(x,y) );
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param x : ÃèÊö
 * \param y : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void CToolTips::Render(int x,int y)
{
	//First render border texture
	if ( m_bShowBorderTex )
		RenderBorderTextuer( x, y );

	//union back color
	if ( !m_bUseCustomeBkColor )
		m_dwBkColor = COLOR_ARGB(180,0,0,0);

	stRectI bkRect( x-5+s_ptTipsBorderGrpLeftTop.x,
					y-5+s_ptTipsBorderGrpLeftTop.y,
					x + m_width + 5 - s_ptTipsBorderGrpLeftTop.x,
					y + m_height + 5 - s_ptTipsBorderGrpLeftTop.y );
	
	GetDevice()->FillRect(&bkRect, m_dwBkColor);		//

	if( m_dwBorderColor & 0x00000000 )		//0xff000000
		GetDevice()->DrawRectangle(&bkRect,m_dwBorderColor);

	x += s_ptTipsBorderGrpLeftTop.x;
	y += s_ptTipsBorderGrpLeftTop.y;

	for(size_t i=0;i<m_lines.size();++i)
	{
		stTipLine& line = *m_lines[i];
		stPointI pt(x + (GetWidth() - s_ptTipsBorderGrpLeftTop.x - line.width)/2, y);
		if ( m_alignType & GUI_ALIGN_LEFT )
			pt.x = x;
		if ( m_alignType & GUI_ALIGN_RIGHT )
			pt.x = x + (GetWidth() - line.width);
		
		line.Render(pt.x,pt.y);
		y+= line.height + m_gaps;
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
const char* CToolTips::GetText()
{
	if(m_lines.empty()) return NULL;
	return m_lines[0]->GetText();
}