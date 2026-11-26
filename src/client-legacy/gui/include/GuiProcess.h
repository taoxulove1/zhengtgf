#pragma once
#include "guicontrol.h"

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 */
class CGuiProcess :
	public CGuiControl
{
public:
	CGuiProcess(CGuiDialog *pDialog=NULL);

	void SetRange(int nMin,int nMax){ m_nMin = nMin; m_nMax = nMax; }
	int  SetPos(int nPos);
	int  GetPos(){ return m_nValue; }

	virtual void SetValue( int nValue ) { SetPos( nValue); }
	virtual int  GetValue() { return GetPos(); };

	void Render( float fElapsedTime );
	void UpdateRects();
	void UpdateProcessRect();
	void SetProcessBitmap(stResourceLocation& rl);

	static stPointI s_borderTopLeft;
	static stPointI s_borderRightBottom;

	void SetText(const char* text);

protected:
	void SetValueInternal( int nValue, bool bFromInput );
	int  ValueFromPos( int x ); 

	int m_nValue;

	int m_nMin;
	int m_nMax;

	stRectI   m_rcRender[9];

	stRectI   m_rcProcess;
	stRectI   m_rcBmpProcess;

	char m_strText[MAX_PATH];      // Window text  
};
