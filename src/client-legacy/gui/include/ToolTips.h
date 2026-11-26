/**
 * \file      ToolTips.h
 * \version   $Id$
 * \author    $$$03公司@###.com
 *           $$$03公司@###.com
 * \date      2006-7-19  21:11:00
 * \brief     工具提示
 * 
 *	      工具提示
 */

#pragma once

#include "../../engine/include/Animation.h"

extern const int c_nDefTipFont;

/**
 * \brief 简短描述
 * 
 * 详细描述
 * 
 */
class CToolTips
{
	class stTipCellBase
	{
	public:
		int		width;
		int		height;
		virtual void Render(int x,int y) = 0;
		virtual ~stTipCellBase(){}
		virtual const char* GetText(){ return NULL;}
	};
	
	class stTipTextCell : public stTipCellBase
	{
	public:
		DWORD			color;
		DWORD			colorShadow;
		enumFontEffect	eFontEffect;
		std::string		szText;
		int				iFont;
		stTipTextCell(const char* pszText,int i_iFont,DWORD dwColor = -1,DWORD dwColorShadow=0xff000000,enumFontEffect fontEffect = FontEffect_Border)
			: iFont(i_iFont),eFontEffect(fontEffect) , color(dwColor) , colorShadow(dwColorShadow)
		{
			szText = pszText;
			width = szText.length() * GetDevice()->GetFontWidth(iFont);
			height = GetDevice()->GetFontHeight(iFont);
		}

		void Render(int x,int y);
		const char* GetText(){ return szText.c_str();}
		~stTipTextCell(){}
	};

	class stTipImageCell : public stTipCellBase
	{
	public:
		CAnimation image;
		
		void Render(int x,int y);

		stTipImageCell(const stResourceLocation* rl)
		{
			image.Create(rl);
			width = image.GetWidth();
			height = image.GetHeight();
		}
		~stTipImageCell(){}
	};

	class stTipLine{
	public:
		std::vector<stTipCellBase*> cells;
		int width,height;

		const char* GetText()
		{
			if(cells.empty()) return NULL;
			return cells[0]->GetText();
		}

		void AddText(const char* pszText,int iFont,DWORD color,DWORD colorShadow,enumFontEffect eFontEffect)
		{
			stTipTextCell* p = new stTipTextCell(pszText,iFont,color,colorShadow,eFontEffect);
			cells.push_back(p);
			if(p->height > height) height = p->height;
			width += p->width;
		}
		
		void AddAnimation(const stResourceLocation* rl)
		{
			stTipImageCell* p = new stTipImageCell(rl);
			cells.push_back(p);
			if(p->height > height) height = p->height;
			width += p->width;
		}

		stTipLine() : width(0) , height(0){}

		~stTipLine()
		{
			for(size_t i=0;i<cells.size();++i)
			{
				delete cells[i];
			}
			cells.clear();
		}
		void Render(int x,int y);
	};

	std::vector<stTipLine*> m_lines;
	DWORD m_curColor;
	DWORD m_curColorShadow;
	DWORD m_dwBorderColor;
	enumFontEffect m_curFontEffect;
	DWORD	m_alignType;

	DWORD m_dwBkColor;

	int m_gaps;		//gaps between line

	int m_width;
	int m_height;

	bool	m_bShowBorderTex;
	bool	m_bUseCustomeBkColor;

public:
	
	CToolTips(void);
	~CToolTips(void);

	void Clear();
	bool Empty() { return m_lines.empty(); }
	DWORD SetCurColor(DWORD color){ DWORD tmp = color; m_curColor = color; return tmp;}
	DWORD SetCurShadowColor(DWORD color) { DWORD tmp = m_curColorShadow; m_curColorShadow = color; return tmp;}
	enumFontEffect SetCurFontEffect(enumFontEffect e) { enumFontEffect eTmp = m_curFontEffect; m_curFontEffect = e; return eTmp;}
	void SetBkColor(DWORD color) { m_dwBkColor = color;}
	void SetAlignType(DWORD alignType) { m_alignType = alignType; }
	void SetBorderColor(DWORD color ) { m_dwBorderColor = color; }
	void SetBorderTexVisible(bool bShow = true) { m_bShowBorderTex = bShow; }
	void SetUseCustomeBkColor(bool bCustom = true) { m_bUseCustomeBkColor = bCustom; }
	void Resize();	

	void SetLineGaps(int gaps) { m_gaps = gaps; }
	int GetLineGaps() { return m_gaps; }

	int GetWidth(){ return m_width;}
	int GetHeight(){ return m_height;}
	DWORD GetAlignType() { return m_alignType; }
	DWORD GetCurColor() { return m_curColor; }
	
	//Border texture
	static void InitBorderTexture();

	void AddText(const char* pszText,int iFont = c_nDefTipFont);
	void AddAnimation(const stResourceLocation *rl,bool bNextLine);
	void RenderBorderTextuer(int x,int y);
	void Render(int x,int y);
	void RenderTopLeftPrefer(stRectI rcBBox, int devWidth, int devHeight);
	const char* GetText();
};
