/**
 * \file      ∏¥º˛ GuiDialog.cpp
 * \version   $Id$
 * \author    –Ì≥Ø
 *            xuzhaoh@tom.com
 * \date      2005-07-05 17:02:57
 * \brief     GUI¥∞ø⁄ª˘¿‡
 * 
 *	      GUI¥∞ø⁄ª˘¿‡
 */

#include "../../engine/include/engine.h"
#include "../include/guiTypes.h"
#include "../../xml_parse/XmlParseEx.h"
#include "../../engine/include/stringex.h"
#include "../../engine/include/Animation.h"

double        CGuiDialog::s_fTimeRefresh = 0.0f;
CGuiControl* CGuiDialog::s_pControlFocus = NULL;        // The control which has focus
CGuiControl* CGuiDialog::s_pControlPressed = NULL;      // The control currently pressed
CAtlArray< GuiElementHolder* > CGuiDialog::s_DefaultElements;
CAtlArray< CGuiElement* > CGuiDialog::s_BackElements;
CGuiElement CGuiDialog::s_CapElement;  // Element for the caption

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
static stPointI s_borderTopLeft(92,96);
static stPointI s_borderRightBottom(92,184);
const char* GetGuiGraphicPackName()
{
	static char szFileName[] = "data\\interfaces.gl";
	return szFileName;
}

// 1 2 3
// 4 0 5
// 6 7 8

static DWORD s_PatchAlignFormat[] = {
	DT_LEFT | DT_TOP , DT_LEFT | DT_TOP , DT_RIGHT | DT_TOP,
	DT_LEFT | DT_TOP , DT_LEFT | DT_TOP , DT_RIGHT | DT_TOP,
	DT_LEFT | DT_BOTTOM , DT_LEFT | DT_BOTTOM, DT_RIGHT | DT_BOTTOM,
};

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param rect : √Ë ˆ
 * \param pBmp : √Ë ˆ
 * \param NULL : √Ë ˆ
 * \param -1 : √Ë ˆ
 * \param Blend_Null : √Ë ˆ
 * \param DT_LEFT : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void PatchRenderImage(const stRectI & rect,IBitmap* pBmp,const stRectI * clip = NULL,DWORD color = -1,BlendType blend = Blend_Null,DWORD dwAlignFormat = DT_TOP | DT_LEFT)
{
	if(!pBmp) return;
	stRectI rcClip;
	if(clip == NULL)
	{
		rcClip.SetRect(0,0,pBmp->GetWidth(),pBmp->GetHeight());
	}
	else
	{
		rcClip = * clip;
	}
	int clipHeight = rcClip.Height();
	int clipWidth = rcClip.Width();

	
	if(dwAlignFormat & DT_BOTTOM)
	{
		if(dwAlignFormat & DT_RIGHT)
		{
			for(int y = rect.bottom; y > rect.top; y -= clipHeight)
			{
				for(int x = rect.right; x > rect.left; x -= clipWidth)
				{
					stRectI destRect(x - clipWidth,y - clipHeight,x,y);
					destRect &= rect;

					stRectI bmpClip(rcClip.right - destRect.Width(),rcClip.bottom - destRect.Height(),rcClip.right,rcClip.bottom);

					pBmp->Render(destRect.left,destRect.top,&bmpClip,NULL,color,blend);
				}
			}
		}
		else
		{
			for(int y = rect.bottom; y > rect.top; y -= clipHeight)
			{
				for(int x = rect.left; x < rect.right; x += clipWidth)
				{
					stRectI destRect(x,y - clipHeight,x + clipWidth,y);
					destRect &= rect;

					stRectI bmpClip(rcClip.left,rcClip.bottom - destRect.Height(),rcClip.left + destRect.Width(),rcClip.bottom);
					pBmp->Render(destRect.left,destRect.top,&bmpClip,NULL,color,blend);
				}
			}
		}
	}
	else
	{
		if(dwAlignFormat & DT_RIGHT)
		{
			for(int y = rect.top; y < rect.bottom; y += clipHeight)
			{
				for(int x = rect.right; x > rect.left; x -= clipWidth)
				{
					stRectI destRect(x - clipWidth,y,x,y + clipHeight);
					destRect &= rect;
					stRectI bmpClip(rcClip.right - destRect.Width(),rcClip.top,rcClip.right,rcClip.top + destRect.Height());
					pBmp->Render(destRect.left,destRect.top,&bmpClip,NULL,color,blend);
				}
			}
		}
		else
		{
			for(int y = rect.top; y < rect.bottom; y += clipHeight)
			{
				for(int x = rect.left; x < rect.right; x += clipWidth)
				{
					stRectI destRect(x,y,x + clipWidth,y + clipHeight);
					destRect &= rect;
					stRectI bmpClip(rcClip.left,rcClip.top,rcClip.left + destRect.Width(),rcClip.top + destRect.Height());
					pBmp->Render(destRect.left,destRect.top,&bmpClip,NULL,color,blend);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// CGuiDialog class
//--------------------------------------------------------------------------------------

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiDialog::CGuiDialog(DWORD dwStyle)
: m_dwStyle(dwStyle)
, m_bBottomMost(false)
, m_bTopMost(false)
, m_pBackImage(NULL)
, m_bClosed(false)
, m_bEnabled(true)
, m_bVisible(true)
, m_pBtnClose(NULL)
, m_minSize(10,10)
, m_maxSize(1024,768)
, m_eAlign(GUI_ALIGN_NONE)
, m_lua(NULL)
, m_dwColor(0xffffffff)
, m_bCloseIsHide(false)
, m_borderTopLeft(s_borderTopLeft)
, m_borderRightBottom(s_borderRightBottom)
{
	m_ID = 0;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
	m_bDrag = false;
	m_bSizing = false;
    m_bCaption = false;
    m_bMinimized = false;
    m_szCaption[0] = L'\0';
    m_nCaptionHeight = 18;

    m_colorTopLeft = COLOR_ARGB(255,98,138,206);
    m_colorTopRight = COLOR_ARGB(255,54,105,192);
    m_colorBottomLeft = COLOR_ARGB(255,54,105,192);
    m_colorBottomRight = COLOR_ARGB(255,10,73,179);

    //m_pCallbackEvent = NULL;

    m_fTimeLastRefresh = 0;

    m_pControlMouseOver = NULL;

    m_nDefaultControlID = 0xffff;
    m_bNonUserEvents = false;
    m_bKeyboardInput = true;
    m_bMouseInput = true;

    //InitDefaultElements();
	m_szName[0] = 0;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiDialog::~CGuiDialog()
{
    int i=0;
	delete m_lua;
	m_lua = NULL;
    RemoveAllControls();

    //m_Fonts.RemoveAll();
    //m_Textures.RemoveAll();

    for( i=0; i < m_DefaultElements.GetCount(); i++ )
    {
        GuiElementHolder* pElementHolder = m_DefaultElements.GetAt( i );
        SAFE_DELETE( pElementHolder );
    }

    m_DefaultElements.RemoveAll();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::RemoveControl(CGuiControl* pControl)
{
	for( int i=0; i < m_Controls.GetCount(); i++ )
	{
		CGuiControl* pControl = m_Controls.GetAt( i );
		if( pControl == pControl)
		{
			// Clean focus first
			ClearFocus();

			// Clear references to this control
			if( s_pControlFocus == pControl )
				s_pControlFocus = NULL;
			if( s_pControlPressed == pControl )
				s_pControlPressed = NULL;
			if( m_pControlMouseOver == pControl )
				m_pControlMouseOver = NULL;

			SAFE_DELETE( pControl );
			m_Controls.RemoveAt( i );

			return;
		}
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::RemoveControl( int ID )
{
    for( int i=0; i < m_Controls.GetCount(); i++ )
    {
        CGuiControl* pControl = m_Controls.GetAt( i );
        if( pControl->GetID() == ID )
        {
            // Clean focus first
            ClearFocus();

            // Clear references to this control
            if( s_pControlFocus == pControl )
                s_pControlFocus = NULL;
            if( s_pControlPressed == pControl )
                s_pControlPressed = NULL;
            if( m_pControlMouseOver == pControl )
                m_pControlMouseOver = NULL;

            SAFE_DELETE( pControl );
            m_Controls.RemoveAt( i );

            return;
        }
    }
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::RemoveAllControls()
{
	if( s_pControlFocus && s_pControlFocus->m_pDialog == this )
		s_pControlFocus = NULL;
	if( s_pControlPressed && s_pControlPressed->m_pDialog == this )
		s_pControlPressed = NULL;
	m_pControlMouseOver = NULL;

	for( int i=0; i < m_Controls.GetCount(); i++ )
	{
		CGuiControl* pControl = m_Controls.GetAt( i );
		SAFE_DELETE( pControl );
	}

	m_Controls.RemoveAll();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::Refresh()
{
	if( s_pControlFocus )
		s_pControlFocus->OnFocusOut();

	if( m_pControlMouseOver )
		m_pControlMouseOver->OnMouseLeave();

	s_pControlFocus = NULL;
	s_pControlPressed = NULL;
	m_pControlMouseOver = NULL;

	for( int i=0; i < m_Controls.GetCount(); i++ )
	{
		CGuiControl* pControl = m_Controls.GetAt(i);
		pControl->Refresh();
	}

	if( m_bKeyboardInput )
		FocusDefaultControl();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
CGuiElement* CGuiDialog::GetCaptionElement()
{
	return &s_CapElement;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param fElapsedTime : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::OnRender( float fElapsedTime )
{   
	// See if the dialog needs to be refreshed
	if( m_fTimeLastRefresh < s_fTimeRefresh )
	{
		m_fTimeLastRefresh = Engine_GetTime();
		Refresh();
	}

	if( !m_bMinimized && !m_pBackImage && (GetStyle() & Gui_WS_NOSKIN))
	{
		stRectI capRect(m_x,m_y,m_x + m_width,m_y + m_height);
		GetDevice()->FillRect(&capRect,m_colorTopLeft,m_colorTopRight,m_colorBottomLeft,m_colorBottomRight);
	}

	// Render the caption if it's enabled.
	if( m_bCaption )
	{
		// DrawSprite will offset the rect down by
		// m_nCaptionHeight, so adjust the rect higher
		// here to negate the effect.
		stRectI rc(0, -m_nCaptionHeight, m_width, 0 );
		DrawSprite( GetCaptionElement(), &rc );
		rc.left += 5; // Make a left margin
		char szOutput[256];
		strncpy( szOutput, m_szCaption, 256 );
		szOutput[255] = 0;
		if( m_bMinimized )
			strncat( szOutput, " (Minimized)", 256 - lstrlen( szOutput ) );
		DrawText( szOutput, GetCaptionElement(), &rc, FontEffect_Shadow );
	}

	// If the dialog is minimized, skip rendering
	// its controls.
	if( !m_bMinimized )
	{
		if( 0 == (GetStyle() & Gui_WS_NOSKIN) && !m_pBackImage)
		{
			int count = s_BackElements.GetCount();
			for(size_t i=0;i<count;++i)
			{
				//DrawSprite(s_BackElements[i],&m_BackRects[i]);
				PatchDrawSprite(s_BackElements[i],&m_BackRects[i],s_PatchAlignFormat[i]);
			}
		}

		bool bFocusPanel = (s_pControlFocus && (s_pControlFocus->GetType() == GUI_CONTROL_TAB || s_pControlFocus->GetType() == GUI_CONTROL_PANEL));
		for( int i=0; i < m_Controls.GetCount(); i++ )
		{
			CGuiControl* pControl = m_Controls.GetAt(i);   

			// Focused control is drawn last
			if( pControl == s_pControlFocus && !bFocusPanel )
				continue;
			if(pControl->IsVisible())
				pControl->Render(fElapsedTime );
		}

		if( s_pControlFocus != NULL && s_pControlFocus->m_pDialog == this && !bFocusPanel && s_pControlFocus->IsVisible())
			s_pControlFocus->Render(fElapsedTime );
	}
	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nEvent : √Ë ˆ
 * \param nID : √Ë ˆ
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool CGuiDialog::OnGuiEvent(UINT nEvent,UINT nID,CGuiControl* pControl)
{
	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		switch(nID)
		{
		case GUI_CLOSEBOX_ID:
			if(m_bCloseIsHide) SetVisible(false);
			else	Close();
			return true;
		}
		break;
	}
	return false;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nEvent : √Ë ˆ
 * \param bTriggeredByUser : √Ë ˆ
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::SendEvent( UINT nEvent, bool bTriggeredByUser, CGuiControl* pControl )
{
	// If no callback has been registered there's nowhere to send the event to
	//if( m_pCallbackEvent == NULL )
	//	return;

	// Discard events triggered programatically if these types of events haven't been
	// enabled
	if( !bTriggeredByUser && !m_bNonUserEvents )
	{
		return;
	}

	if(m_lua)
	{
		/*
		lua_getglobal(*m_lua, "OnGuiEvent");
		lua_State* L = *m_lua;
		if(lua_type(L,-1) == LUA_TFUNCTION)
		{
			push_lua_value(*m_lua,(int)nEvent);
			push_lua_value(*m_lua,(int)(pControl ? pControl->GetID() : -1));
			push_lua_value(*m_lua,(int)pControl);
			lua_call(*m_lua,3,1);

			bool bResult = false;

			if(match_lua_value(type2type<bool>(),*m_lua,-1))
			{
				bResult = get_lua_value(type2type<bool>(),*m_lua,-1);
				lua_pop(*m_lua, 1);
			}

			if(bResult) return ;
		}
		//*/
		bool bResult = false;
		lua_function_call(*m_lua,"OnGuiEvent",bResult,(int)nEvent,(int)(pControl ? pControl->GetID() : -1),static_cast<tGuiControlHandle>(pControl));
		if(bResult) return ;
	}

	OnGuiEvent(nEvent,(pControl ? pControl->GetID() : -1),pControl);
	
	//m_pCallbackEvent( nEvent, pControl->GetID(), pControl );
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param index : √Ë ˆ
 * \param strFaceName : √Ë ˆ
 * \param height : √Ë ˆ
 * \param weight : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::SetFont( UINT index, const char* strFaceName, LONG height, LONG weight )
{
	/*
	// Make sure the list is at least as large as the index being set
	UINT i;
	for( i=m_Fonts.GetCount(); i <= index; i++ )
	{
	m_Fonts.Add( -1 );
	}

	int iFont = GetGlobalDialogResourceManager()->AddFont( strFaceName, height, weight );
	m_Fonts.SetAt( index, iFont );
	//*/

	return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param hWnd : √Ë ˆ
 * \param uMsg : √Ë ˆ
 * \param wParam : √Ë ˆ
 * \param lParam : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
bool CGuiDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//if(!m_bEnable || !m_bVisible || m_bClosed) return false;

	if(uMsg == WM_LBUTTONDOWN)
	{
		int iiiiii=0;
	}
	bool bHandled = false;
	static POINT ptDrag;
	static enumSizingType s_eSizingType;
	static POINT ptSizing;
	// If caption is enable, check for clicks in the caption area.
	if(m_bDrag)
	{
		if( uMsg == WM_LBUTTONUP )
		{
			//ReleaseCapture();
			m_bDrag = false;
			//m_bMinimized = !m_bMinimized;
			return true;
		}
		else if( uMsg == WM_MOUSEMOVE)
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			m_x = mousePoint.x + ptDrag.x;
			m_y = mousePoint.y + ptDrag.y;
		}
		return true;
	}
	else if(m_bSizing)
	{
		if( uMsg == WM_LBUTTONUP )
		{
			m_bSizing = false;
			return true;
		}
		else if( uMsg == WM_MOUSEMOVE )
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			stPointI pt(mousePoint.x + ptSizing.x,mousePoint.y + ptSizing.y);
			if(s_eSizingType & eSizingType_Top)
			{
				int bottom = m_y + m_height;
				m_y = pt.y;
				m_height = bottom - m_y;
				if(m_height < m_minSize.y)
				{	
					m_y = bottom - m_minSize.y;
					m_height = m_minSize.y;
				}
				else if(m_height > m_maxSize.y)
				{
					m_y = bottom - m_maxSize.y;
					m_height = m_maxSize.y;
				}
			}
			else if(s_eSizingType & eSizingType_Bottom)
			{
				m_height = pt.y - m_y;
				if(m_height < m_minSize.y)
				{
					m_height = m_minSize.y;
				}
				else if(m_height > m_maxSize.y)
				{
					m_height = m_maxSize.y;
				}
			}
			
			if(s_eSizingType & eSizingType_Left)
			{
				int right = m_x + m_width;
				m_x = pt.x;
				m_width = right - m_x;
				if(m_width < m_minSize.x)
				{	
					m_x = right - m_minSize.x;
					m_height = m_minSize.x;
				}
				else if(m_width > m_maxSize.x)
				{
					m_x = right - m_maxSize.x;
					m_width = m_maxSize.x;
				}
			}
			else if(s_eSizingType & eSizingType_Right)
			{
				m_width = pt.x - m_x;
				if(m_width < m_minSize.x)
				{
					m_width = m_minSize.x;
				}
				else if(m_width > m_maxSize.x)
				{
					m_width = m_maxSize.x;
				}
			}
			SendEvent(EVENT_DIALOG_RESIZE,true,NULL);
			UpdateLocation();
		}
		return true;
	}
	else
	{
		/*
		if( uMsg == WM_LBUTTONDOWN /*|| uMsg == WM_LBUTTONDBLCLK*/  /*)
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			DWORD dwHiTest = OnHiTest(mousePoint);
			if(dwHiTest != HTERROR && dwHiTest != HTTRANSPARENT)
			{
				if(GetGuiManager()->CanHaveFocus(this))
				{
					GetGuiManager()->RequestFocus(this);
				}
				bHandled = true;
			}
			if(dwHiTest == HTCAPTION)
			{
				if(GetGuiManager()->CanHaveFocus(this))
				{
					m_bDrag = true;
					//SetCapture( Engine_GetHwnd() );
					ptDrag.x = m_x - mousePoint.x;
					ptDrag.y = m_y - mousePoint.y;
					return true;
				}
			}
		}
			//*/
	}

	// If the dialog is minimized, don't send any messages to controls.
	if( m_bMinimized )
		return bHandled;

	// If a control is in focus, it belongs to this dialog, and it's enabled, then give
	// it the first chance at handling the message.
	if( s_pControlFocus && 
		s_pControlFocus->m_pDialog == this && 
		s_pControlFocus->IsEnabled() )
	{
		// If the control MsgProc handles it, then we don't.
		if( s_pControlFocus->MsgProc( uMsg, wParam, lParam ) )
			return true;
	}

	switch( uMsg )
	{
	case WM_ACTIVATEAPP:
		// Call OnFocusIn()/OnFocusOut() of the control that currently has the focus
		// as the application is activated/deactivated.  This matches the Windows
		// behavior.
		if( s_pControlFocus && 
			s_pControlFocus->m_pDialog == this && 
			s_pControlFocus->IsEnabled() )
		{
			if( wParam )
				s_pControlFocus->OnFocusIn();
			else
				s_pControlFocus->OnFocusOut();
		}
		break;

		// Keyboard messages
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{

#ifdef _DEBUG
			if(uMsg == WM_KEYDOWN)
			{
				//TRACE("WM_KEYDOWN wParam:%u\n",wParam);
				if(wParam == 83 && GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_CONTROL) < 0)
				{
					// Ctrl + Shift + S
					char szFileName[MAX_PATH]="";
					char szFileTitle[MAX_PATH]="";
					OPENFILENAME fn;
					memset(&fn,0,sizeof(OPENFILENAME));
					fn.lStructSize = sizeof(OPENFILENAME);
					fn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
					fn.lpstrDefExt = _T("xml");
					fn.lpstrFilter = _T("*.xml\0*.xml");
					fn.lpstrFile = szFileName;
					fn.nMaxFile = MAX_PATH;
					fn.lpstrTitle = "«Î—°‘ÒŒƒº˛√˚";
					fn.hwndOwner = Engine_GetHWnd();
					fn.hInstance = ::GetModuleHandle(NULL);
					fn.lpstrFileTitle = szFileTitle;
					fn.nMaxFileTitle = MAX_PATH;
					if(GetSaveFileName(&fn))
					{
						SaveGuiToXML(szFileName);
					}
					else
					{
						DWORD dw = CommDlgExtendedError();
						TRACE("GetSaveFileName Error : %u\n",dw);
					}
					SetCurrentDirectory(Engine_GetAppation()->GetAppPath());
				}
				if(wParam == 75 && GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_CONTROL) < 0)
				{
					// ±£¥Ê
					// Ctrl + Shift + K
					char szFileName[MAX_PATH]="";
					char szFileTitle[MAX_PATH]="";
					OPENFILENAME fn;
					memset(&fn,0,sizeof(OPENFILENAME));
					fn.lStructSize = sizeof(OPENFILENAME);
					fn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
					fn.lpstrDefExt = _T("xml");
					fn.lpstrFilter = _T("*.xml\0*.xml");
					fn.lpstrFile = szFileName;
					fn.nMaxFile = MAX_PATH;
					fn.lpstrTitle = "«Î—°‘Ò±£¥Êµƒ∆§∑ÙŒƒº˛√˚";
					fn.hwndOwner = Engine_GetHWnd();
					fn.hInstance = ::GetModuleHandle(NULL);
					fn.lpstrFileTitle = szFileTitle;
					fn.nMaxFileTitle = MAX_PATH;
					if(GetSaveFileName(&fn))
					{
						SaveDefaultElementToXML(szFileName);
					}
					else
					{
						DWORD dw = CommDlgExtendedError();
						TRACE("GetSaveFileName Error : %u\n",dw);
					}
					SetCurrentDirectory(Engine_GetAppation()->GetAppPath());
				}
			}
#endif
			// If a control is in focus, it belongs to this dialog, and it's enabled, then give
			// it the first chance at handling the message.
			if( s_pControlFocus && 
				s_pControlFocus->m_pDialog == this && 
				s_pControlFocus->IsEnabled() )
			{
				if( s_pControlFocus->HandleKeyboard( uMsg, wParam, lParam ) )
					return true;
			}

			// Not yet handled, see if this matches a control's hotkey
			// Activate the hotkey if the focus doesn't belong to an
			// edit box.
			if( uMsg == WM_KEYDOWN /*&& ( !s_pControlFocus ||
				( s_pControlFocus->GetType() != GUI_CONTROL_EDITBOX
				&& s_pControlFocus->GetType() != GUI_CONTROL_MLEDITBOX ) ) */)
			{
				for( int i=0; i < m_Controls.GetCount(); i++ )
				{
					CGuiControl* pControl = m_Controls.GetAt( i );
					if( pControl->GetHotkey().IsKeyDown() )
					{
						pControl->OnHotkey();
						return true;
					}
				}
			}

			// Not yet handled, check for focus messages
			if( uMsg == WM_KEYDOWN )
			{
				// If keyboard input is not enabled, this message should be ignored
				if( !m_bKeyboardInput )
					return bHandled;

				switch( wParam )
				{
				case VK_RIGHT:
				case VK_DOWN:
					if( s_pControlFocus != NULL )
					{
						OnCycleFocus( true );
						return true;
					}
					break;

				case VK_LEFT:
				case VK_UP:
					if( s_pControlFocus != NULL )
					{
						OnCycleFocus( false );
						return true;
					}
					break;

				case VK_TAB: 
					if( s_pControlFocus == NULL )
					{
						FocusDefaultControl();
					}
					else
					{
						bool bShiftDown = ((GetAsyncKeyState( VK_SHIFT ) & 0x8000) != 0);
						OnCycleFocus( !bShiftDown );
					}
					return true;
				}
			}

			break;
		}


		// Mouse messages
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{

			// If not accepting mouse input, return false to indicate the message should still 
			// be handled by the application (usually to move the camera).
			if( !m_bMouseInput )
				return bHandled;

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			mousePoint.x -= m_x;
			mousePoint.y -= m_y;

			// If caption is enabled, offset the Y coordinate by the negative of its height.
			if( m_bCaption )
				mousePoint.y -= m_nCaptionHeight;

			// If a control is in focus, it belongs to this dialog, and it's enabled, then give
			// it the first chance at handling the message.
			if( s_pControlFocus && 
				s_pControlFocus->m_pDialog == this && 
				s_pControlFocus->IsEnabled() )
			{
				if( s_pControlFocus->HandleMouse( uMsg, mousePoint, wParam, lParam ) )
					return true;
			}

			// Not yet handled, see if the mouse is over any controls
			CGuiControl* pControl = GetControlAtPoint( mousePoint );
			if( pControl != NULL && pControl->IsEnabled() )
			{
				bool b = pControl->HandleMouse( uMsg, mousePoint, wParam, lParam );
				if( b )
					return true;
			}
			else
			{
				// Mouse not over any controls in this dialog, if there was a control
				// which had focus it just lost it
				if( uMsg == WM_LBUTTONDOWN && 
					s_pControlFocus && 
					s_pControlFocus->GetType() != GUI_CONTROL_EDITBOX && 
					s_pControlFocus->GetType() != GUI_CONTROL_MLEDITBOX &&
					s_pControlFocus->m_pDialog == this )
				{
					s_pControlFocus->OnFocusOut();
					s_pControlFocus = NULL;
				}
			}

			// Still not handled, hand this off to the dialog. Return false to indicate the
			// message should still be handled by the application (usually to move the camera).
			switch( uMsg )
			{
			case WM_MOUSEMOVE:
				OnMouseMove( mousePoint );
				return bHandled;
			}

			break;
		}
	}
	if(!bHandled)
	{
		if( uMsg == WM_LBUTTONDOWN /*|| uMsg == WM_LBUTTONDBLCLK*/  )
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			DWORD dwHiTest = OnHiTest(mousePoint);
			//*
			if(dwHiTest != HTERROR && dwHiTest != HTTRANSPARENT)
			{
				if(GetGuiManager()->CanHaveFocus(this))
				{
					GetGuiManager()->RequestFocus(this);
				}
				bHandled = true;
			}
			//*/
			if(dwHiTest == HTCAPTION)
			{
				if(GetGuiManager()->CanHaveFocus(this))
				{
					if(0 ==(GetStyle() & GUI_WS_NOMOVE))
					{
						m_bDrag = true;
						//SetCapture( Engine_GetHwnd() );
						ptDrag.x = m_x - mousePoint.x;
						ptDrag.y = m_y - mousePoint.y;
						return true;
					}
					return false;
				}
			}
			enumSizingType sizingType = HiTestCodeToSizingType(dwHiTest);
			if(sizingType != eSizingType_None)
			{
				s_eSizingType = sizingType;
				if(GetGuiManager()->CanHaveFocus(this))
				{
					m_bSizing = true;
					stPointI pt2(0,0);
					if(sizingType & eSizingType_Left)
					{
						pt2.x = m_x;
					}
					else if(sizingType & eSizingType_Right)
					{
						pt2.x = m_x + m_width;
					}

					if(sizingType & eSizingType_Top)
					{
						pt2.y = m_y;
					}
					else if(sizingType & eSizingType_Bottom)
					{
						pt2.y = m_y + m_height;
					}
					ptSizing.x = pt2.x - mousePoint.x;
					ptSizing.y = pt2.y - mousePoint.y;
					return true;
				}
			}
		}
	}
	return bHandled;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pt : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiControl* CGuiDialog::GetControlAtPoint( POINT pt )
{
	// Search through all child controls for the first one which
	// contains the mouse point
	for( int i=m_Controls.GetCount()-1; i >=0; i-- )
	{
		CGuiControl* pControl = m_Controls.GetAt(i);

		if( pControl == NULL )
		{
			continue;
		}

		// We only return the current control if it is visible
		// and enabled.  Because GetControlAtPoint() is used to do mouse
		// hittest, it makes sense to perform this filtering.
		if( pControl->ContainsPoint( pt ) && pControl->IsEnabled() && pControl->IsVisible() )
		{
			return pControl;
		}
	}

	return NULL;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
bool CGuiDialog::GetControlEnabled( int ID )
{
	CGuiControl* pControl = GetControl( ID );
	if( pControl == NULL )
		return false;

	return pControl->IsEnabled();
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param bEnabled : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::SetControlEnabled( int ID, bool bEnabled )
{
	CGuiControl* pControl = GetControl( ID );
	if( pControl == NULL )
		return;

	pControl->SetEnabled( bEnabled );
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pt : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::OnMouseUp( POINT pt )
{
	s_pControlPressed = NULL;
	m_pControlMouseOver = NULL;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pt : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::OnMouseMove( POINT pt )
{
	// Figure out which control the mouse is over now
	CGuiControl* pControl = GetControlAtPoint( pt );

	// If the mouse is still over the same control, nothing needs to be done
	if( pControl == m_pControlMouseOver )
		return;

	// Handle mouse leaving the old control
	if( m_pControlMouseOver )
		m_pControlMouseOver->OnMouseLeave();

	// Handle mouse entering the new control
	m_pControlMouseOver = pControl;
	if( pControl != NULL )
		m_pControlMouseOver->OnMouseEnter();
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nControlType : √Ë ˆ
 * \param iElement : √Ë ˆ
 * \param pElement : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::SetDefaultElement( UINT nControlType, UINT iElement, CGuiElement* pElement )
{
	// If this Element type already exist in the list, simply update the stored Element
	for( int i=0; i < s_DefaultElements.GetCount(); i++ )
	{
		GuiElementHolder* pElementHolder = s_DefaultElements.GetAt( i );

		if( pElementHolder->nControlType == nControlType &&
			pElementHolder->iElement == iElement )
		{
			pElementHolder->Element = *pElement;
			return S_OK;
		}
	}

	// Otherwise, add a new entry
	GuiElementHolder* pNewHolder;
	pNewHolder = new GuiElementHolder;
	if( pNewHolder == NULL )
		return E_OUTOFMEMORY;

	pNewHolder->nControlType = nControlType;
	pNewHolder->iElement = iElement;
	pNewHolder->Element = *pElement;

	s_DefaultElements.Add( pNewHolder );
	return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nControlType : √Ë ˆ
 * \param iElement : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiElement* CGuiDialog::GetDefaultElement( UINT nControlType, UINT iElement )
{
	for( int i=0; i < m_DefaultElements.GetCount(); i++ )
	{
		GuiElementHolder* pElementHolder = m_DefaultElements.GetAt( i );

		if( pElementHolder->nControlType == nControlType &&
			pElementHolder->iElement == iElement )
		{
			return &pElementHolder->Element;
		}
	}

	for( int i=0; i < s_DefaultElements.GetCount(); i++ )
	{
		GuiElementHolder* pElementHolder = s_DefaultElements.GetAt( i );

		if( pElementHolder->nControlType == nControlType &&
			pElementHolder->iElement == iElement )
		{
			return &pElementHolder->Element;
		}
	}

	return NULL;
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddStatic( int ID, const char* strText, int x, int y, int width, int height, bool bIsDefault, CGuiStatic** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiStatic* pStatic = new CGuiStatic( this );

	if( ppCreated != NULL )
		*ppCreated = pStatic;

	if( pStatic == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pStatic );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index
	pStatic->SetID( ID ); 
	pStatic->SetText( strText );
	pStatic->SetLocation( x, y );
	pStatic->SetSize( width, height );
	pStatic->m_bIsDefault = bIsDefault;

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param rl : √Ë ˆ
 * \param imgRect : √Ë ˆ
 * \param color : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetBackImage(const stResourceLocation& rl,const RECT * imgRect,DWORD color,int x,int y)
{
	if(!m_pBackImage)
	{
		AddImage(0,x,y,rl,imgRect,color,-1,-1,CGuiImage::FULLSIZE_IMAGE,&m_pBackImage);
	}
	else
	{
		m_pBackImage->SetLocation(x,y);
		m_pBackImage->SetImage(rl,imgRect,color);
	}
	m_pBackImage->m_bIsBack = true;
	m_pBackImage->SetZOrder(GUI_CONTROL_BOTTOM);
	m_width = m_pBackImage->GetX() + m_pBackImage->GetWidth();
	m_height = m_pBackImage->GetY() + m_pBackImage->GetHeight();
	UpdateLocation();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::AddItemCell(int ID,int x,int y,CGuiItemCell** ppCreated)
{
	HRESULT hr = S_OK;
	CGuiItemCell* pImage = new CGuiItemCell( this );
	if(ppCreated != NULL)
		*ppCreated = pImage;

	if(pImage == NULL)
		return E_OUTOFMEMORY;

	hr = AddControl(pImage);
	if(FAILED(hr)) 
		return hr;
	pImage->SetID( ID );
	pImage->SetLocation(x,y);
	pImage->SetSize(0,0);
	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param cellWidth : √Ë ˆ
 * \param cellHeight : √Ë ˆ
 * \param colCount : √Ë ˆ
 * \param rowCount : √Ë ˆ
 * \param showRowCount : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::AddTable(int ID,int x,int y,int cellWidth,int cellHeight,int colCount,int rowCount,int showRowCount,CGuiTable** ppCreated)
{
	HRESULT hr = S_OK;
	CGuiTable* pImage = new CGuiTable( this );

	if(ppCreated != NULL)
		*ppCreated = pImage;

	if(pImage == NULL)
		return E_OUTOFMEMORY;

	pImage->m_cellWidth = cellWidth;
	pImage->m_cellHeight = cellHeight;
	pImage->m_rowCount = rowCount;
	pImage->m_colCount = colCount;
	pImage->m_showRowCount = showRowCount;
	pImage->SetID( ID );
	pImage->SetLocation(x,y);
	pImage->SetSize(colCount * cellWidth,showRowCount*cellHeight);

	hr = AddControl(pImage);
	if(FAILED(hr)) 
		return hr;

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param rl : √Ë ˆ
 * \param imgRect : √Ë ˆ
 * \param dwColor : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param dwStyle : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::AddImage(int ID,int x,int y,const stResourceLocation & rl,const RECT * imgRect,DWORD dwColor,int width,int height,DWORD dwStyle,CGuiImage ** ppCreated)
{
	HRESULT hr = S_OK;
	CGuiImage* pImage = new CGuiImage( this );

	if(ppCreated != NULL)
		*ppCreated = pImage;

	if(pImage == NULL)
		return E_OUTOFMEMORY;

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param nHotkey : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
	hr = AddControl(pImage);
	if(FAILED(hr)) 
		return hr;
	pImage->SetID( ID );
	pImage->SetLocation(x,y);
	pImage->SetSize(width,height);
	pImage->SetStyle(dwStyle);
	return pImage->SetImage(rl,imgRect,dwColor);
}
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddButton( int ID, const char* strText, int x, int y, int width, int height, const char* nHotkey, bool bIsDefault, CGuiButton** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiButton* pButton = new CGuiButton( this );

	if( ppCreated != NULL )
		*ppCreated = pButton;

	if( pButton == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pButton );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index
	pButton->SetID( ID ); 
	pButton->SetText( strText );
	pButton->SetLocation( x, y );
	pButton->SetSize( width, height );
	if(nHotkey)
		pButton->SetHotkey( nHotkey );
	pButton->m_bIsDefault = bIsDefault;

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param style : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::AddTab( int ID,int x,int y,int width,int height,DWORD style,CGuiTab** ppCreated)
{
	HRESULT hr = S_OK;
	CGuiTab* pTab = new CGuiTab(this);
	if(ppCreated != NULL)
		*ppCreated = pTab;

	if(pTab == NULL) return E_OUTOFMEMORY;

	hr = AddControl(pTab);
	if(FAILED(hr))
		return hr;

	pTab->SetStyle(style);
	pTab->SetID(ID);
	pTab->SetLocation(x,y);
	pTab->SetSize(width,height);

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param bChecked : √Ë ˆ
 * \param nHotkey : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddCheckBox( int ID, const char* strText, int x, int y, int width, int height, bool bChecked, const char* nHotkey, bool bIsDefault, CGuiCheckBox** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiCheckBox* pCheckBox = new CGuiCheckBox( this );

	if( ppCreated != NULL )
		*ppCreated = pCheckBox;

	if( pCheckBox == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pCheckBox );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index
	pCheckBox->SetID( ID ); 
	pCheckBox->SetText( strText );
	pCheckBox->SetLocation( x, y );
	pCheckBox->SetSize( width, height );
	if(nHotkey)
		pCheckBox->SetHotkey( nHotkey );
	pCheckBox->m_bIsDefault = bIsDefault;
	pCheckBox->SetChecked( bChecked );

	return S_OK;
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param nButtonGroup : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param bChecked : √Ë ˆ
 * \param nHotkey : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddRadioButton( int ID, UINT nButtonGroup, const char* strText, int x, int y, int width, int height, bool bChecked, const char* nHotkey, bool bIsDefault, CGuiRadioButton** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiRadioButton* pRadioButton = new CGuiRadioButton( this );

	if( ppCreated != NULL )
		*ppCreated = pRadioButton;

	if( pRadioButton == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pRadioButton );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index
	pRadioButton->SetID( ID ); 
	pRadioButton->SetText( strText );
	pRadioButton->SetButtonGroup( nButtonGroup );
	pRadioButton->SetLocation( x, y );
	pRadioButton->SetSize( width, height );
	if(nHotkey)
		pRadioButton->SetHotkey( nHotkey );
	pRadioButton->SetChecked( bChecked,true );
	pRadioButton->m_bIsDefault = bIsDefault;
	pRadioButton->SetChecked( bChecked ,true);

	return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param nHotkey : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddComboBox( int ID, int x, int y, int width, int height, const char* nHotkey, bool bIsDefault, CGuiComboBox** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiComboBox* pComboBox = new CGuiComboBox( this );

	if( ppCreated != NULL )
		*ppCreated = pComboBox;

	if( pComboBox == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pComboBox );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index
	pComboBox->SetID( ID ); 
	pComboBox->SetLocation( x, y );
	pComboBox->SetSize( width, height );
	if(nHotkey)
		pComboBox->SetHotkey( nHotkey );
	pComboBox->m_bIsDefault = bIsDefault;

	return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param min : √Ë ˆ
 * \param max : √Ë ˆ
 * \param pagesize : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::AddScrollBar( int ID, int x, int y, int width, int height, int min, int max, int pagesize, bool bIsDefault, CGuiScrollBar** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiScrollBar* pScrollBar = new CGuiScrollBar( this );

	if( ppCreated != NULL )
		*ppCreated = pScrollBar;

	if( pScrollBar == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pScrollBar );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index

	pScrollBar->SetID( ID ); 
	pScrollBar->SetLocation( x, y );
	pScrollBar->SetSize( width, height );
	pScrollBar->m_bIsDefault = bIsDefault;
	pScrollBar->SetTrackRange(min,max);
	pScrollBar->SetTrackPos( min );
	pScrollBar->SetPageSize(pagesize);

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param min : √Ë ˆ
 * \param max : √Ë ˆ
 * \param value : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddSlider( int ID, int x, int y, int width, int height, int min, int max, int value, bool bIsDefault, CGuiSlider** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiSlider* pSlider = new CGuiSlider( this );

	if( ppCreated != NULL )
		*ppCreated = pSlider;

	if( pSlider == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pSlider );
	if( FAILED(hr) )
		return hr;

	// Set the ID and list index
	pSlider->SetID( ID ); 
	pSlider->SetLocation( x, y );
	pSlider->SetSize( width, height );
	pSlider->m_bIsDefault = bIsDefault;
	pSlider->SetRange( min, max );
	pSlider->SetValue( value );

	return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param dwStyle : √Ë ˆ
 * \param isDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::AddMLEditBox(int ID,const char* strText,int x,int y,int width,int height,DWORD dwStyle,bool isDefault,CGuiMLEditBox** ppCreated)
{
	HRESULT hr = S_OK;
	CGuiMLEditBox * pMLEditBox = new CGuiMLEditBox(this,dwStyle);
	if(ppCreated ) *ppCreated = pMLEditBox;

	if(!pMLEditBox) return E_OUTOFMEMORY;

	hr = AddControl(pMLEditBox);
	if(FAILED(hr)) return hr;

	pMLEditBox->SetID(ID);
	pMLEditBox->SetLocation(x,y);
	pMLEditBox->SetSize(width,height);
	pMLEditBox->m_bIsDefault = isDefault;

	if(strText)
	{
		pMLEditBox->SetText(strText);
	}

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddEditBox( int ID, const char* strText, int x, int y, int width, int height, bool bIsDefault, CGuiEditBox** ppCreated )
{
	HRESULT hr = S_OK;

	CGuiEditBox *pEditBox = new CGuiEditBox( this );

	if( ppCreated != NULL )
		*ppCreated = pEditBox;

	if( pEditBox == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pEditBox );
	if( FAILED(hr) )
		return hr;

	// Set the ID and position
	pEditBox->SetID( ID ); 
	pEditBox->SetLocation( x, y );
	pEditBox->SetSize( width, height );
	pEditBox->m_bIsDefault = bIsDefault;
	
	if( strText )
		pEditBox->SetText( strText );
	if( CGuiDialog::s_pControlFocus == NULL )
		this->SetFocus(pEditBox);
	return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param strText : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param bIsDefault : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
/*
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddIMEEditBox( int ID, const char* strText, int x, int y, int width, int height, bool bIsDefault, CGuiImeEditBox** ppCreated )
{
HRESULT hr = S_OK;
CGuiImeEditBox *pEditBox = new CGuiImeEditBox( this );

if( ppCreated != NULL )
*ppCreated = pEditBox;

if( pEditBox == NULL )
return E_OUTOFMEMORY;

hr = AddControl( pEditBox );
if( FAILED(hr) )
return hr;

// Set the ID and position
pEditBox->SetID( ID ); 
pEditBox->SetLocation( x, y );
pEditBox->SetCount( width, height );
pEditBox->m_bIsDefault = bIsDefault;

if( strText )
pEditBox->SetText( strText );

return S_OK;
}
//*/


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param x : √Ë ˆ
 * \param y : √Ë ˆ
 * \param width : √Ë ˆ
 * \param height : √Ë ˆ
 * \param dwStyle : √Ë ˆ
 * \param ppCreated : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddListBox( int ID, int x, int y, int width, int height, DWORD dwStyle, CGuiListBox** ppCreated )
{
	HRESULT hr = S_OK;
	CGuiListBox *pListBox = new CGuiListBox( this );

	if( ppCreated != NULL )
		*ppCreated = pListBox;

	if( pListBox == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pListBox );
	if( FAILED(hr) )
		return hr;

	// Set the ID and position
	pListBox->SetID( ID );
	pListBox->SetLocation( x, y );
	pListBox->SetSize( width, height );
	pListBox->SetStyle( dwStyle );

	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::OnCreate()
{
	if( 0 == (GetStyle() & Gui_WS_NOSKIN) )
	{
		m_bCaption = false;
	}

	if( GetStyle() & GUI_WS_CLOSEBOX )
	{
		// ”–πÿ±’∞¥≈•
		int width = 15;
		int height = 15;
		AddButton(GUI_CLOSEBOX_ID,NULL,GetWidth()-3-width,-2 - height,width,height,0,false,&m_pBtnClose);
		m_pBtnClose->SetButtonType(CGuiButton::ButtonType_CloseBox);
		InitControl(m_pBtnClose,GUI_CONTROL_CLOSEBOX);
		m_pBtnClose->SetLocation(GetWidth()-3-width,-2 - height);
	}
	UpdateLocation();
	SendEvent(EVENT_DIALOG_CREATE,true,NULL);
	if(m_dwStyle & GUI_WS_RESIZEING)
	{
		SendEvent(EVENT_DIALOG_RESIZE,true,NULL);
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::InitControl( CGuiControl* pControl )
{
    HRESULT hr;

    if( pControl == NULL )
        return E_INVALIDARG;

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \param iControlType : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
    return InitControl(pControl,pControl->GetType());
}
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::InitControl( CGuiControl* pControl ,int iControlType)
{
	HRESULT hr;

	if( pControl == NULL )
		return E_INVALIDARG;


	// Look for a default Element entries
	for(int i=0;1;i++)
	{
		CGuiElement* pElement = GetDefaultElement(iControlType,i);
		if(!pElement) break;
		pControl->SetElement( i, pElement );
	}

	return ( pControl->OnInit() );
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::AddControl( CGuiControl* pControl )
{
    HRESULT hr = S_OK;

    hr = InitControl( pControl );
    if( FAILED(hr) )
        return DXTRACE_ERR( "CGuiDialog::InitControl", hr );

    // Add to the list
    hr = m_Controls.Add( pControl );
    if( FAILED(hr) )
    {
        return DXTRACE_ERR( "CAtlArray::Add", hr );
    }

    return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiControl* CGuiDialog::GetControl( int ID )
{
    // Try to find the control with the given ID
    for( int i=0; i < m_Controls.GetCount(); i++ )
    {
        CGuiControl* pControl = m_Controls.GetAt( i );

        if( pControl->GetID() == ID )
        {
            return pControl;
        }
    }

    // Not found
    return NULL;
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \param nControlType : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiControl* CGuiDialog::GetControl( int ID, UINT nControlType )
{
    // Try to find the control with the given ID
    for( int i=0; i < m_Controls.GetCount(); i++ )
    {
        CGuiControl* pControl = m_Controls.GetAt( i );

        if( pControl->GetID() == ID && pControl->GetType() == nControlType )
        {
            return pControl;
        }
    }

    // Not found
    return NULL;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiControl* CGuiDialog::GetNextControl( CGuiControl* pControl )
{
    int index = ControlToIndex(pControl) + 1;

    // Cycle through dialogs in the loop to find the next control. Note
    // that if only one control exists in all looped dialogs it will
    // be the returned 'next' control.
    if( index >= (int)m_Controls.GetCount() )
    {
		index = 0;
    }

    return m_Controls.GetAt( index );
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
CGuiControl* CGuiDialog::GetPrevControl( CGuiControl* pControl )
{
    int index = ControlToIndex(pControl) - 1;
    // Cycle through dialogs in the loop to find the next control. Note
    // that if only one control exists in all looped dialogs it will
    // be the returned 'previous' control.
    if( index < 0 )
    {
        index = m_Controls.GetCount() - 1;
    }
    
    return m_Controls.GetAt( index );    
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nButtonGroup : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::ClearRadioButtonGroup( UINT nButtonGroup )
{
    // Find all radio buttons with the given group number
    for( int i=0; i < m_Controls.GetCount(); i++ )
    {
        CGuiControl* pControl = m_Controls.GetAt( i );

        if( pControl->GetType() == GUI_CONTROL_RADIOBUTTON )
        {
            CGuiRadioButton* pRadioButton = (CGuiRadioButton*) pControl;

            if( pRadioButton->GetButtonGroup() == nButtonGroup )
                pRadioButton->SetChecked( false, false );
        }
    }
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ID : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::ClearComboBox( int ID )
{
    CGuiComboBox* pComboBox = GetComboBox( ID );
    if( pComboBox == NULL )
        return;

    pComboBox->RemoveAllItems();
}




/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::RequestFocus( CGuiControl* pControl )
{
	if(!GetGuiManager()->CanHaveFocus(this)) return;

    if( s_pControlFocus == pControl )
        return;

    if( !pControl->CanHaveFocus() )
        return;

    if( s_pControlFocus )
        s_pControlFocus->OnFocusOut();

    pControl->OnFocusIn();
    s_pControlFocus = pControl;

	GetGuiManager()->RequestFocus(this);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pRect : √Ë ˆ
 * \param color : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::DrawRectangle(const RECT* pRect, DWORD color)
{
	stRectI rcScreen = *pRect;
    rcScreen.OffsetRect( m_x, m_y );
	
    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
        rcScreen.OffsetRect( 0, m_nCaptionHeight );

	if(GetDevice()->DrawRectangle(&rcScreen,color))
		return S_OK;

	return S_FALSE;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pRect : √Ë ˆ
 * \param color : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::DrawRect( const RECT* pRect, DWORD color )
{
    stRectI rcScreen = *pRect;
    rcScreen.OffsetRect( m_x, m_y );
	
    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
        rcScreen.OffsetRect( 0, m_nCaptionHeight );

	if(GetDevice()->FillRect(&rcScreen,ColorBlend(m_dwColor,color)))
		return S_OK;
	return S_FALSE;

	/*
    GUI_SCREEN_VERTEX vertices[4] =
    {
        (float) rcScreen.left -0.5f,  (float) rcScreen.top -0.5f,    0.5f, 1.0f, color, 0, 0,
        (float) rcScreen.right -0.5f, (float) rcScreen.top -0.5f,    0.5f, 1.0f, color, 0, 0, 
        (float) rcScreen.right -0.5f, (float) rcScreen.bottom -0.5f, 0.5f, 1.0f, color, 0, 0, 
        (float) rcScreen.left -0.5f,  (float) rcScreen.bottom -0.5f, 0.5f, 1.0f, color, 0, 0,
    };

    IDirect3DDevice9* pd3dDevice = GetGlobalDialogResourceManager()->GetD3DDevice();

    // Since we're doing our own drawing here we need to flush the sprites
    GetGlobalDialogResourceManager()->m_pSprite->Flush();
    IDirect3DVertexDeclaration9 *pDecl = NULL;
    pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
    pd3dDevice->SetFVF( GUI_SCREEN_VERTEX::FVF );

    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

    pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(GUI_SCREEN_VERTEX) );

    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

    // Restore the vertex decl
    pd3dDevice->SetVertexDeclaration( pDecl );
    pDecl->Release();
	//*/
    return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param apPoints : √Ë ˆ
 * \param nNumPoints : √Ë ˆ
 * \param color : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::DrawPolyLine( POINT* apPoints, UINT nNumPoints, DWORD color )
{
	if(GetDevice()->DrawPolyLine(apPoints,nNumPoints,color))
		return S_OK;
	return S_FALSE;

	/*
    GUI_SCREEN_VERTEX* vertices = new GUI_SCREEN_VERTEX[ nNumPoints ];
    if( vertices == NULL )
        return E_OUTOFMEMORY;

    GUI_SCREEN_VERTEX* pVertex = vertices;
    POINT* pt = apPoints;
    for( UINT i=0; i < nNumPoints; i++ )
    {
        pVertex->x = m_x + (float) pt->x;
        pVertex->y = m_y + (float) pt->y;
        pVertex->z = 0.5f;
        pVertex->h = 1.0f;
        pVertex->color = color;
        pVertex->tu = 0.0f;
        pVertex->tv = 0.0f;

        pVertex++;
        pt++;
    }

    IDirect3DDevice9* pd3dDevice = GetGlobalDialogResourceManager()->GetD3DDevice();

    // Since we're doing our own drawing here we need to flush the sprites
    GetGlobalDialogResourceManager()->m_pSprite->Flush();
    IDirect3DVertexDeclaration9 *pDecl = NULL;
    pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
    pd3dDevice->SetFVF( GUI_SCREEN_VERTEX::FVF );

    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

    pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nNumPoints - 1, vertices, sizeof(GUI_SCREEN_VERTEX) );

    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

    // Restore the vertex decl
    pd3dDevice->SetVertexDeclaration( pDecl );
    pDecl->Release();

    SAFE_DELETE_ARRAY( vertices );
    return S_OK;
	//*/
}
 

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pElement : √Ë ˆ
 * \param prcDest : √Ë ˆ
 * \param dwAlignFormat : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
HRESULT CGuiDialog::PatchDrawSprite(CGuiElement* pElement, RECT* prcDest,DWORD dwAlignFormat)
{
    // No need to draw fully transparent layers
    if( pElement->TextureColor.Current.a < 1.0f/16.0f )
        return S_OK;

	if( pElement->pBmp == NULL)
		return S_OK;

    stRectI & rcTexture = pElement->rcTexture;
    
    stRectI rcScreen = *prcDest;
	rcScreen.OffsetRect(m_x,m_y);

    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
		rcScreen.OffsetRect( 0, m_nCaptionHeight );
    
	/*
	stPointF scale((float) rcScreen.Width() / rcTexture.Width(),(float) rcScreen.Height() / rcTexture.Height());
	//stPointF scale(1.0,1.0f);
	if(pElement->pBmp->Render(rcScreen.left,rcScreen.top,&rcTexture,&scale,ColorBlend(m_dwColor,(DWORD)pElement->TextureColor.Current)))
		return S_OK;
	//*/
	PatchRenderImage(rcScreen,pElement->pBmp,&rcTexture,ColorBlend(m_dwColor,(DWORD)pElement->TextureColor.Current),Blend_Null,dwAlignFormat);
	return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pElement : √Ë ˆ
 * \param prcDest : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::DrawSprite( CGuiElement* pElement, RECT* prcDest )
{
    // No need to draw fully transparent layers
    if( pElement->TextureColor.Current.a < 1.0f/16.0f )
        return S_OK;

	if( pElement->pBmp == NULL)
		return S_OK;

    stRectI & rcTexture = pElement->rcTexture;
    
    stRectI rcScreen = *prcDest;
	rcScreen.OffsetRect(m_x,m_y);

    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
		rcScreen.OffsetRect( 0, m_nCaptionHeight );
    
	stPointF scale((float) rcScreen.Width() / rcTexture.Width(),(float) rcScreen.Height() / rcTexture.Height());
	//stPointF scale(1.0,1.0f);
	if(pElement->pBmp->Render(rcScreen.left,rcScreen.top,&rcTexture,&scale,ColorBlend(m_dwColor,(DWORD)pElement->TextureColor.Current)))
		return S_OK;

	return S_FALSE;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param strText : √Ë ˆ
 * \param pElement : √Ë ˆ
 * \param prcDest : √Ë ˆ
 * \param nCount : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::CalcTextRect( const char* strText, CGuiElement* pElement, RECT* prcDest, int nCount )
{
	int iFont = GetDevice()->SetFont(pElement->iFont);
	GetDevice()->GetStringRect(strText,nCount,*prcDest,pElement->dwTextFormat);
	GetDevice()->SetFont(iFont);
	return S_OK;
	/*
    HRESULT hr = S_OK;

    DXUTFontNode* pFontNode = GetFont( pElement->iFont );
    DWORD dwTextFormat = pElement->dwTextFormat | DT_CALCRECT;
    // Since we are only computing the rectangle, we don't need a sprite.
    hr = pFontNode->pFont->DrawText( NULL, strText, nCount, prcDest, dwTextFormat, pElement->FontColor.Current );
    if( FAILED(hr) )
        return hr;
	//*/

    return S_OK;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param strText : √Ë ˆ
 * \param pElement : √Ë ˆ
 * \param prcDest : √Ë ˆ
 * \param fontEffect : √Ë ˆ
 * \param nCount : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::DrawText( const WCHAR* strText, CGuiElement* pElement, RECT* prcDest, enumFontEffect fontEffect, int nCount )
{
    HRESULT hr = S_OK;

    // No need to draw fully transparent layers
    if( pElement->FontColor.Current.a < 1.0f/16.0f )
        return S_OK;

    stRectI rcScreen = *prcDest;
	rcScreen.OffsetRect( m_x, m_y );

    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
        rcScreen.OffsetRect( 0, m_nCaptionHeight );
	
	int iFont = GetDevice()->SetFont(pElement->iFont);
	
	GetDevice()->DrawString(strText,nCount,rcScreen,pElement->FontColor.Current,pElement->dwTextFormat,fontEffect,COLOR_ARGB(DWORD(pElement->FontColor.Current.a * 255), 0, 0, 0));
	
	GetDevice()->SetFont(iFont);

    return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param strText : √Ë ˆ
 * \param pElement : √Ë ˆ
 * \param prcDest : √Ë ˆ
 * \param fontEffect : √Ë ˆ
 * \param nCount : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
HRESULT CGuiDialog::DrawText( const char* strText, CGuiElement* pElement, RECT* prcDest, enumFontEffect fontEffect, int nCount )
{
    HRESULT hr = S_OK;

    // No need to draw fully transparent layers
    if( pElement->FontColor.Current.a < 1.0f/16.0f )
        return S_OK;

    stRectI rcScreen = *prcDest;
	rcScreen.OffsetRect( m_x, m_y );

    // If caption is enabled, offset the Y position by its height.
    if( m_bCaption )
        rcScreen.OffsetRect( 0, m_nCaptionHeight );
	
	int iFont = GetDevice()->SetFont(pElement->iFont);
	
	//bShadow = false;

	GetDevice()->DrawString(strText,nCount,rcScreen,pElement->FontColor.Current,pElement->dwTextFormat,fontEffect,COLOR_ARGB(DWORD(pElement->FontColor.Current.a * 255), 0, 0, 0));
	
	GetDevice()->SetFont(iFont);

    return S_OK;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param colorTopLeft : √Ë ˆ
 * \param colorTopRight : √Ë ˆ
 * \param colorBottomLeft : √Ë ˆ
 * \param colorBottomRight : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::SetBackgroundColors( DWORD colorTopLeft, DWORD colorTopRight, DWORD colorBottomLeft, DWORD colorBottomRight )
{
    m_colorTopLeft = colorTopLeft;
    m_colorTopRight = colorTopRight;
    m_colorBottomLeft = colorBottomLeft;
    m_colorBottomRight = colorBottomRight;
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::ClearFocus()
{
    if( s_pControlFocus )
    {
        s_pControlFocus->OnFocusOut();
        s_pControlFocus = NULL;
    }
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetFocus(CGuiControl* pControl)
{
	if(!pControl || pControl->m_pDialog != this) return;
	if(!IsFocus())
	{
		return;
	}
	ClearFocus();
	s_pControlFocus = pControl;
	s_pControlFocus->OnFocusIn();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::FocusDefaultControl()
{
    // Check for default control in this dialog
    for( int i=0; i < m_Controls.GetCount(); i++ )
    {
        CGuiControl* pControl = m_Controls.GetAt( i );
        if( pControl->m_bIsDefault )
        {
            // Remove focus from the current control
            ClearFocus();

            // Give focus to the default control
            s_pControlFocus = pControl;
            s_pControlFocus->OnFocusIn();
            return;
        }
    }
}



/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param bForward : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::OnCycleFocus( bool bForward )
{
    // This should only be handled by the dialog which owns the focused control, and 
    // only if a control currently has focus
    if( s_pControlFocus == NULL || s_pControlFocus->m_pDialog != this )
        return;

    CGuiControl* pControl = s_pControlFocus;
    for( int i=0; i < 0xffff; i++ )
    {
        pControl = (bForward) ? GetNextControl( pControl ) : GetPrevControl( pControl );
        
        // If we've gone in a full circle then focus doesn't change
        if( pControl == s_pControlFocus )
            return;

        // If the dialog accepts keybord input and the control can have focus then
        // move focus
        if( pControl->m_pDialog->m_bKeyboardInput && pControl->CanHaveInputFocus() )
        {
            s_pControlFocus->OnFocusOut();
            s_pControlFocus = pControl;
            s_pControlFocus->OnFocusIn();
            return;
        }
    }

    // If we reached this point, the chain of dialogs didn't form a complete loop
    DXTRACE_ERR( "CGuiDialog: Multiple dialogs are improperly chained together", E_FAIL );
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::OnMouseEnter( CGuiControl* pControl )
{
    if( pControl == NULL )
        return;

    //pControl->m_bMouseOver = true;
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
//--------------------------------------------------------------------------------------
void CGuiDialog::OnMouseLeave( CGuiControl* pControl )
{
    if( pControl == NULL )
        return;

    //pControl->m_bMouseOver = false;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::UninitDefaultElement()
{
	for(int i=0; i < s_DefaultElements.GetCount(); i++ )
	{
		GuiElementHolder* pElementHolder = s_DefaultElements.GetAt( i );
		SAFE_DELETE( pElementHolder );
	}

	s_DefaultElements.RemoveAll();

	for(int i=0; i < s_BackElements.GetCount(); i++ )
	{
		CGuiElement* pElement = s_BackElements.GetAt( i );
		SAFE_DELETE( pElement );
	}

	s_BackElements.RemoveAll();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::InitCustomElements()
{
	SetFont( 0, "Arial", 14, FW_NORMAL );
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param parent : √Ë ˆ
 * \param guiColor : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
inline void GuiElementBlendColorToXML(TiXmlElement* parent,GuiBlendColor* guiColor)
{
	TiXmlElement* blendColor = parent->InsertEndChild(TiXmlElement("blendColor"))->ToElement();
	XML_SetColorAttribute(blendColor,"defaultColor",guiColor->defaultColor);
	for(size_t i = 0;i < count_of(guiColor->States); ++i)
	{
		TiXmlElement* item = blendColor->InsertEndChild(TiXmlElement("state"))->ToElement();
		item->SetAttribute("state",GetGuiControlStateDesc(i));
		XML_SetColorAttribute(item,"color",guiColor->States[i]);
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param parent : √Ë ˆ
 * \param pElement : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
inline void GuiElementToXML(TiXmlElement* parent,CGuiElement* pElement)
{
	IBitmap* pBmp = pElement->pBmp;
	if(pBmp)
	{
		TiXmlElement* image = parent->InsertEndChild(TiXmlElement("image"))->ToElement();
		image->SetAttribute("graphicPack",pBmp->GetPackName());
		image->SetAttribute("group",(int)pBmp->GetGroup());
		image->SetAttribute("frame",(int)pBmp->GetFrame());
		char szRect[64];
		pElement->rcTexture.ToString(szRect,sizeof(szRect));
		image->SetAttribute("rect",szRect);
		GuiElementBlendColorToXML(image,&pElement->TextureColor);
	}
	TiXmlElement* font = parent->InsertEndChild(TiXmlElement("font"))->ToElement();
	font->SetAttribute("font",pElement->iFont);
	XML_SetAttribute(font,"format",pElement->dwTextFormat,16);
	GuiElementBlendColorToXML(font,&pElement->FontColor);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param parent : √Ë ˆ
 * \param guiColor : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
inline void GuiElementBlendColorFromXML(TiXmlElement* parent,GuiBlendColor* guiColor)
{
	TiXmlElement* blendColor = parent->FirstChildElement("blendColor");
	if(!blendColor) return;
	guiColor->defaultColor = XML_GetColorAttribute(blendColor,"defaultColor",0xffffffff);
	TiXmlElement* state = blendColor->FirstChildElement("state");
	while(state)
	{
		const char* szState = state->Attribute("state");
		if(szState)
		{
			int iState = DescToGuiControlState(szState);
			guiColor->States[iState] = XML_GetColorAttribute(state,"color",0);
		}
		state = state->NextSiblingElement("state");
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param parent : √Ë ˆ
 * \param pElement : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
inline void GuiElementFromXML(TiXmlElement* parent,CGuiElement* pElement)
{
	TiXmlElement* image = parent->FirstChildElement("image");
	if(image)
	{
		const char* szGraphicPack = image->Attribute("graphicPack");
		const char * szRect = image->Attribute("rect");
		if(szGraphicPack && szRect)
		{
			int iGroup,iFrame;
			image->Attribute("group",&iGroup);
			image->Attribute("frame",&iFrame);
			stResourceLocation rl;
			if(strncpy(rl.szFileName,szGraphicPack,sizeof(rl.szFileName)))
			{
				rl.group = iGroup;
				rl.frame = iFrame;
				stRectI rect;
				rect.FromString(szRect);
				DWORD defaultColor = 0xffffffff;
				TiXmlElement* blendColor = image->FirstChildElement("blendColor");
				if(blendColor)
					defaultColor = XML_GetColorAttribute(blendColor,"defaultColor",0xffffffff);
				pElement->SetTexture(&rl,&rect,defaultColor);
				GuiElementBlendColorFromXML(image,&pElement->TextureColor);
			}
		}
	}
	TiXmlElement* font = parent->FirstChildElement("font");
	if(font)
	{
		int iFont;
		DWORD defaultColor = 0xffffffff;
		TiXmlElement* blendColor = font->FirstChildElement("blendColor");
		if(blendColor)
			defaultColor = XML_GetColorAttribute(blendColor,"defaultColor",0xffffffff);

		font->Attribute("font",&iFont);
		pElement->dwTextFormat = DT_LEFT | DT_TOP;
		DWORD dwTextFormat;
		XML_QueryAttribute(font,"format",16,(int*)&dwTextFormat);
		pElement->SetFont(iFont,defaultColor,dwTextFormat);
		GuiElementBlendColorFromXML(font,&pElement->FontColor);
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pStream : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool CGuiDialog::LoadDefaultElementFromXML(Stream* pStream)
{
	TiXmlDocument doc;

	if(!LoadXMLFromStream(pStream,doc))
		return false;

	TiXmlElement* skin = doc.FirstChildElement("skin");
	TiXmlElement* defaltElements = skin->FirstChildElement("defaltElements");

	TiXmlElement* dialog = defaltElements->FirstChildElement("dialog");
	if(dialog)
	{
		TiXmlElement* caption = dialog->FirstChildElement("caption");
		GuiElementFromXML(caption,&s_CapElement);
		// Pre-blend as we don't need to transition the state
		s_CapElement.TextureColor.Blend( GUI_STATE_NORMAL, 10.0f );
		s_CapElement.FontColor.Blend( GUI_STATE_NORMAL, 10.0f );
	}

	TiXmlElement* controls = defaltElements->FirstChildElement("controls");
	TiXmlElement* control = controls->FirstChildElement("control");
	while(control)
	{
		int iControlType;

		const char* szControlType = control->Attribute("type");
		if(szControlType)
		{
			iControlType = DescToGuiControlType(szControlType);
			TiXmlElement* item = control->FirstChildElement("element");
			while(item)
			{
				int iElement;
				item->Attribute("index",&iElement);

				GuiElementHolder Element;
				Element.nControlType = iControlType;
				Element.iElement = iElement;
				GuiElementFromXML(item,&Element.Element);
				SetDefaultElement(iControlType,iElement,&Element.Element);
				item = item->NextSiblingElement("element");
			}
		}
		control = control->NextSiblingElement("control");
	}

	return true;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pszZipPackName : √Ë ˆ
 * \param pszFileName : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool CGuiDialog::LoadDefaultElementFromXML(const char* pszZipPackName,const char* pszFileName)
{
	Stream* pStream = OpenPackFileForRead(pszZipPackName,pszFileName);
	if(!pStream) return false;
	bool b = LoadDefaultElementFromXML(pStream);
	ClosePackFileForRead(pStream);
	return b;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pszFileName : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SaveDefaultElementToXML(const char* pszFileName)
{
	TiXmlDocument doc; 
	doc.InsertEndChild(TiXmlDeclaration("1.0","gb2312",""));

	TiXmlElement* skin = XML_FindAndAddChildNode(&doc,"skin");
	TiXmlElement* defaltElements = XML_FindAndAddChildNode(skin,"defaltElements");

	TiXmlElement* dialog = XML_FindAndAddChildNode(defaltElements,"dialog");

	TiXmlElement* caption = XML_FindAndAddChildNode(dialog,"caption");
	GuiElementToXML(caption,&s_CapElement);

	TiXmlElement* controls = XML_FindAndAddChildNode(defaltElements,"controls");

	TiXmlElement* control = NULL;
	int iControlType;
	for(int i = 0;i < s_DefaultElements.GetCount();++i)
	{
		GuiElementHolder* pElementHolder = s_DefaultElements[i];
		
		if(i == 0 || pElementHolder->nControlType != iControlType)
		{
			iControlType = pElementHolder->nControlType;
			control = controls->InsertEndChild(TiXmlElement("control"))->ToElement();
			control->SetAttribute("type",GetGuiControlTypeDesc(pElementHolder->nControlType));
		}

		TiXmlElement* item = control->InsertEndChild(TiXmlElement("element"))->ToElement();
		item->SetAttribute("index",(int)pElementHolder->iElement);
		CGuiElement* pElement = &pElementHolder->Element;
		GuiElementToXML(item,pElement);
	}
	doc.SaveFile(pszFileName);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param p1 : √Ë ˆ
 * \param p2 : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int compareGuiElementHolder(const void *p1, const void *p2) 
{
	GuiElementHolder* pElementHolder1 = *(GuiElementHolder**)p1;
	GuiElementHolder* pElementHolder2 = *(GuiElementHolder**)p2;
	return ((pElementHolder1->nControlType << 16) |(pElementHolder1->iElement))
		- ((pElementHolder2->nControlType << 16) |(pElementHolder2->iElement));
}
//--------------------------------------------------------------------------------------
void CGuiDialog::InitDefaultElements()
{
	//if(LoadDefaultElementFromXML("data\\gui.pak","gui\\DefaultSkin.xml"))
	//	return;

    const char* szGrpFileName = "data\\interfaces.gl";
    
    CGuiElement Element;
    stRectI rcTexture;

	stResourceLocation rl;
	rl.SetFileName(GetGuiGraphicPackName());
	rl.group = 0;
	rl.frame = 0;

    //-------------------------------------
    // Element for the caption
    //-------------------------------------
    s_CapElement.SetFont( 0 );
    rcTexture.SetRect( 17, 269, 241, 287 );
    s_CapElement.SetTexture( &rl, &rcTexture );
    s_CapElement.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(255, 255, 255, 255);
    s_CapElement.FontColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(255, 255, 255, 255);
    s_CapElement.SetFont( 0, D3DCOLOR_ARGB(255, 255, 255, 255), DT_LEFT | DT_VCENTER );
    // Pre-blend as we don't need to transition the state
    s_CapElement.TextureColor.Blend( GUI_STATE_NORMAL, 10.0f );
    s_CapElement.FontColor.Blend( GUI_STATE_NORMAL, 10.0f );
	
	//-------------------------------------
	// Element for the back
	//-------------------------------------
	{
		rl.group = 0;
		rl.frame = 2;
		stRectI rects[9];
		stRectI totalRect(0,0,520,358);
		CuttRect9(totalRect,s_borderTopLeft,s_borderRightBottom,rects);
		s_BackElements.SetCount(count_of(rects));
		for(size_t i=0;i<count_of(rects);++i)
		{
			CGuiElement* pEmt = new CGuiElement;
			pEmt->SetTexture(&rl,&rects[i]);
			pEmt->FontColor.Current = COLOR_ARGB(255,255,255,255);
			pEmt->TextureColor.Current = COLOR_ARGB(255,255,255,255);
			s_BackElements[i] = pEmt;
		}
	}

	rl.group = 0;
	rl.frame = 0;
    //-------------------------------------
    // CGuiStatic
    //-------------------------------------
	Element.SetFont( 0 ,COLOR_ARGB(255,255,255,255),DT_TOP | DT_LEFT);
    Element.FontColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );

    // Assign the Element
    SetDefaultElement( GUI_CONTROL_STATIC, 0, &Element );
    
    //-------------------------------------
    // CGuiButton - Button
    //-------------------------------------
    rcTexture.SetRect( 0, 0, 136, 54 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0 );
    Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(150, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(200, 255, 255, 255);
    Element.FontColor.States[ GUI_STATE_MOUSEOVER ] = D3DCOLOR_ARGB(255, 0, 0, 0);
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_BUTTON, 0, &Element );
    
    //-------------------------------------
    // CGuiButton - Fill layer
    //-------------------------------------
    rcTexture.SetRect( 136, 0, 272, 54 );
    Element.SetTexture( &rl, &rcTexture, D3DCOLOR_ARGB(0, 255, 255, 255) );
    Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = D3DCOLOR_ARGB(160, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(60, 0, 0, 0);
    Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(30, 255, 255, 255);
    
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_BUTTON, 1, &Element );


    //-------------------------------------
    // CGuiCheckBox - Box
    //-------------------------------------
    rcTexture.SetRect( 0, 54, 27, 81 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0, D3DCOLOR_ARGB(255, 255, 255, 255), DT_LEFT | DT_VCENTER );
    Element.FontColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );
    Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(150, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(200, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 255, 255, 255);
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_CHECKBOX, 0, &Element );


    //-------------------------------------
    // CGuiCheckBox - Check
    //-------------------------------------
    rcTexture.SetRect( 27, 54, 54, 81 );
    Element.SetTexture( &rl, &rcTexture );
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_CHECKBOX, 1, &Element );


    //-------------------------------------
    // CGuiRadioButton - Box
    //-------------------------------------
    rcTexture.SetRect( 54, 54, 81, 81 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0, D3DCOLOR_ARGB(255, 255, 255, 255), DT_LEFT | DT_VCENTER );
    Element.FontColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );
    Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(150, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(200, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 255, 255, 255);
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_RADIOBUTTON, 0, &Element );


    //-------------------------------------
    // CGuiRadioButton - Check
    //-------------------------------------
    rcTexture.SetRect( 81, 54, 108, 81 );
    Element.SetTexture( &rl, &rcTexture );
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_RADIOBUTTON, 1, &Element );


    //-------------------------------------
    // CGuiComboBox - Main
    //-------------------------------------
    rcTexture.SetRect( 7, 81, 247, 123 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0 );
    Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(150, 200, 200, 200);
    Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(170, 230, 230, 230);
    Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(70, 200, 200, 200);
    Element.FontColor.States[ GUI_STATE_MOUSEOVER ] = D3DCOLOR_ARGB(255, 0, 0, 0);
    Element.FontColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 0, 0, 0);
    Element.FontColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(200, 200, 200, 200);
    
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_COMBOBOX, 0, &Element );


    //-------------------------------------
    // CGuiComboBox - Button
    //-------------------------------------
    rcTexture.SetRect( 272, 0, 325, 49 );
    Element.SetTexture( &rl, &rcTexture );
    Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(150, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 150, 150, 150);
    Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(200, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(70, 255, 255, 255);
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_COMBOBOX, 1, &Element );


    //-------------------------------------
    // CGuiComboBox - Dropdown
    //-------------------------------------
    rcTexture.SetRect( 7, 123, 241, 265 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0, D3DCOLOR_ARGB(255, 0, 0, 0), DT_LEFT | DT_TOP );
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_COMBOBOX, 2, &Element );


    //-------------------------------------
    // CGuiComboBox - Selection
    //-------------------------------------
    rcTexture.SetRect( 7, 266, 241, 289 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0, D3DCOLOR_ARGB(255, 255, 255, 255), DT_LEFT | DT_TOP );
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_COMBOBOX, 3, &Element );


    //-------------------------------------
    // CGuiSlider - Track
    //-------------------------------------
    rcTexture.SetRect( 1, 290, 280, 331 );
    Element.SetTexture( &rl, &rcTexture );
    Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(150, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(200, 255, 255, 255);
    Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(70, 255, 255, 255);
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_SLIDER, 0, &Element );

    //-------------------------------------
    // CGuiSlider - Button
    //-------------------------------------
    rcTexture.SetRect( 248, 55, 289, 96 );
    Element.SetTexture( &rl, &rcTexture );

    // Assign the Element
    SetDefaultElement( GUI_CONTROL_SLIDER, 1, &Element );

    //-------------------------------------
    // CGuiScrollBar - Track
    //-------------------------------------
    rcTexture.SetRect( 243, 144, 265, 155 );
    Element.SetTexture( &rl, &rcTexture );
    Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(255, 200, 200, 200);
    
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_SCROLLBAR, 0, &Element );

    //-------------------------------------
    // CGuiScrollBar - Up Arrow
    //-------------------------------------
    rcTexture.SetRect( 243, 124, 265, 144 );
    Element.SetTexture( &rl, &rcTexture );
    Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(255, 200, 200, 200);
    
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_SCROLLBAR, 1, &Element );

    //-------------------------------------
    // CGuiScrollBar - Down Arrow
    //-------------------------------------
    rcTexture.SetRect( 243, 155, 265, 176 );
    Element.SetTexture( &rl, &rcTexture );
    Element.TextureColor.States[ GUI_STATE_DISABLED ] = D3DCOLOR_ARGB(255, 200, 200, 200);
    
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_SCROLLBAR, 2, &Element );

    //-------------------------------------
    // CGuiScrollBar - Button
    //-------------------------------------
    rcTexture.SetRect( 266, 123, 286, 167 );
    Element.SetTexture( &rl, &rcTexture );
    
    // Assign the Element
    SetDefaultElement( GUI_CONTROL_SCROLLBAR, 3, &Element );


    //-------------------------------------
    // CGuiEditBox
    //-------------------------------------
    // Element assignment:
    //   0 - text area
    //   1 - top left border
    //   2 - top border
    //   3 - top right border
    //   4 - left border
    //   5 - right border
    //   6 - lower left border
    //   7 - lower border
    //   8 - lower right border

    Element.SetFont( 0, D3DCOLOR_ARGB( 255, 0, 0, 0 ), DT_LEFT | DT_TOP | DT_SINGLELINE);

    // Assign the style
    rcTexture.SetRect( 14, 91, 241, 113 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 0, &Element );

    rcTexture.SetRect( 8, 82, 14, 91 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 1, &Element );

    rcTexture.SetRect( 14, 82, 241, 91 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 2, &Element );

    rcTexture.SetRect(241, 82, 246, 91 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 3, &Element );

    rcTexture.SetRect(8, 91, 14, 113 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 4, &Element );

    rcTexture.SetRect(241, 91, 246, 113 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 5, &Element );

    rcTexture.SetRect(8, 113, 14, 121 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 6, &Element );

    rcTexture.SetRect(14, 113, 241, 121 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 7, &Element );

    rcTexture.SetRect(241, 113, 246, 121 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_EDITBOX, 8, &Element );


    //-------------------------------------
    // CGuiMLEditBox
    //-------------------------------------
    // Element assignment:
    //   0 - text area
    //   1 - top left border
    //   2 - top border
    //   3 - top right border
    //   4 - left border
    //   5 - right border
    //   6 - lower left border
    //   7 - lower border
    //   8 - lower right border

    Element.SetFont( 0, D3DCOLOR_ARGB( 255, 0, 0, 0 ), DT_LEFT | DT_TOP | DT_SINGLELINE);

    // Assign the style
    rcTexture.SetRect( 15, 93, 240, 112 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 0, &Element );

    rcTexture.SetRect( 8, 82, 15, 93 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 1, &Element );

    rcTexture.SetRect( 15, 82, 240, 93 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 2, &Element );

    rcTexture.SetRect(240, 82, 246, 93 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 3, &Element );

    rcTexture.SetRect(8, 93, 15, 112 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 4, &Element );

    rcTexture.SetRect(240, 93, 246, 112 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 5, &Element );

    rcTexture.SetRect(8, 112, 15, 121 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 6, &Element );

    rcTexture.SetRect(15, 112, 240, 121 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 7, &Element );

    rcTexture.SetRect(240, 112, 246, 121 );
    Element.SetTexture( &rl, &rcTexture );
    SetDefaultElement( GUI_CONTROL_MLEDITBOX, 8, &Element );


	//-------------------------------------
	// CGuiPanel
	//-------------------------------------
	// Element assignment:
	//   0 - text area
	//   1 - top left border
	//   2 - top border
	//   3 - top right border
	//   4 - left border
	//   5 - right border
	//   6 - lower left border
	//   7 - lower border
	//   8 - lower right border

	stRectI rects[9];
	stRectI totalRect(0,340,65,398);
	CuttRect9(totalRect,stPointI(11,11),stPointI(11,11),rects);
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 0, 0, 0 ), DT_CENTER | DT_VCENTER);
	
	// Assign the style
	int i=0;
	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 0, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 1, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 2, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 3, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 4, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 5, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 6, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 7, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	SetDefaultElement( GUI_CONTROL_PANEL, 8, &Element );


	//-------------------------------------
	// CGuiTabItem
	//-------------------------------------
	// Element assignment:
	//   0 - text area
	//   1 - top left border
	//   2 - top border
	//   3 - top right border
	//   4 - left border
	//   5 - right border
	//   6 - lower left border
	//   7 - lower border
	//   8 - lower right border

	totalRect.SetRect(0,400,83,432);
	CuttRect9(totalRect,stPointI(5,5),stPointI(5,5),rects);
	i = 0;
	// ≤ª—°÷–µƒÕº∆¨
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 255, 255, 255 ), DT_CENTER | DT_VCENTER);
	Element.FontColor.States[GUI_STATE_PRESSED] = 0;
	Element.FontColor.States[GUI_STATE_MOUSEOVER] = 0;

	// Assign the style
	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	
	SetDefaultElement( GUI_CONTROL_TAB, 0, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 1, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 2, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 3, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 4, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 5, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 6, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 7, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_PRESSED] = 0;
	Element.TextureColor.States[GUI_STATE_MOUSEOVER] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 8, &Element );

	//----------------------------------------------------
	//----------------------------------------------------
	// —°÷–µƒÕº∆¨
	i = 0;
	Element.SetFont( 0, D3DCOLOR_ARGB( 0, 0, 0, 0 ), DT_CENTER | DT_VCENTER);
	Element.FontColor.States[GUI_STATE_NORMAL] = 0;

	// Assign the style
	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 9, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 10, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 11, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 12, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 13, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 14, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 15, &Element );

	Element.SetTexture( &rl, &rects[i++] );
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 16, &Element );

	Element.SetTexture( &rl, &rects[i++]);
	Element.TextureColor.States[GUI_STATE_NORMAL] = 0;
	SetDefaultElement( GUI_CONTROL_TAB, 17, &Element );
	//----------------------------------------------------


    //-------------------------------------
    // CGuiListBox - Main
    //-------------------------------------

    rcTexture.SetRect(13, 124, 241, 265 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0, D3DCOLOR_ARGB(255, 0, 0, 0), DT_LEFT | DT_TOP );

    // Assign the Element
    SetDefaultElement( GUI_CONTROL_LISTBOX, 0, &Element );

    //-------------------------------------
    // CGuiListBox - Selection
    //-------------------------------------

    rcTexture.SetRect(17, 269, 241, 287 );
    Element.SetTexture( &rl, &rcTexture );
    Element.SetFont( 0, D3DCOLOR_ARGB(255, 255, 255, 255), DT_LEFT | DT_TOP );

    // Assign the Element
    SetDefaultElement( GUI_CONTROL_LISTBOX, 1, &Element );

	
	//--------------------------------------
	// CGuiTable - Texture
	//--------------------------------------

	rl.frame = 1;
	RECT rcTableTexture[] = {
		{0,0,46,45},
		{46,0,46+33,45},
		{79,0,79+47,45},

		{0,45,46,45+33},
		{46,45,46+33,45+33},
		{79,45,79+47,45+33},

		{0,78,46,78+47},
		{46,78,46+33,78+47},
		{79,78,79+47,78+47},
	};

	for(size_t i=0;i<9;++i)
	{
		Element.SetTexture( &rl, &rcTableTexture[i] );
		SetDefaultElement( GUI_CONTROL_TABLE , i , &Element);
	}
	//-------------------------------------

	//-------------------------------------
	// CloseBox
	//-------------------------------------
	rl.group = 0;
	rl.frame = 0;
	Element.SetFont(0);
	rcTexture.SetRect(245,229,245+19,229+19);
	Element.SetTexture(&rl,&rcTexture);
	Element.TextureColor.States[ GUI_STATE_NORMAL ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.TextureColor.States[ GUI_STATE_PRESSED ] = 0;
	Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = 0;
	Element.TextureColor.States[ GUI_STATE_FOCUS ] = 0;
	SetDefaultElement(GUI_CONTROL_CLOSEBOX,0,&Element);

	rcTexture.SetRect(264,229,264+19,229+19);
	Element.SetTexture(&rl,&rcTexture);
	Element.TextureColor.States[ GUI_STATE_NORMAL ] = 0;
	Element.TextureColor.States[ GUI_STATE_PRESSED ] = 0;
	Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.TextureColor.States[ GUI_STATE_FOCUS ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	SetDefaultElement(GUI_CONTROL_CLOSEBOX,1,&Element);

	rcTexture.SetRect(283,229,283+19,229+19);
	Element.SetTexture(&rl,&rcTexture);
	Element.TextureColor.States[ GUI_STATE_NORMAL ] = 0;
	Element.TextureColor.States[ GUI_STATE_PRESSED ] = D3DCOLOR_ARGB(255, 255, 255, 255);
	Element.TextureColor.States[ GUI_STATE_MOUSEOVER ] = 0;
	Element.TextureColor.States[ GUI_STATE_FOCUS ] = 0;
	SetDefaultElement(GUI_CONTROL_CLOSEBOX,2,&Element);
	//-------------------------------------------------

	qsort(&s_DefaultElements[0],s_DefaultElements.GetCount(),sizeof(GuiElementHolder*),compareGuiElementHolder);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \param index : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetControlZOrder(CGuiControl* pControl,int index)
{
	int oldIndex = FindControl(pControl);
	ATLASSERT(oldIndex != -1);
	if(oldIndex == -1) return;
	if(oldIndex == index) return;
	if(oldIndex < index)
	{
		memmove(&m_Controls[oldIndex],&m_Controls[oldIndex+1],sizeof(CGuiControl*) * (index - oldIndex));
	}
	else
	{
		memmove(&m_Controls[index + 1],&m_Controls[index],sizeof(CGuiControl*) * (oldIndex - index));
	}
	m_Controls[index] = pControl;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int  CGuiDialog::FindControl(CGuiControl * pControl)
{
	for(int i=0 ; i < m_Controls.GetCount(); ++i)
	{
		if(m_Controls[i] == pControl)
			return i;
	}
	return -1;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pControl : √Ë ˆ
 * \param pInsertAfter : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetControlZOrder(CGuiControl* pControl,CGuiControl * pInsertAfter)
{
	switch((int)(pInsertAfter))
	{
	case GUI_CONTROL_TOP:
		SetControlZOrder(pControl,m_Controls.GetCount()-1);
		return;
	case GUI_CONTROL_BOTTOM:
		SetControlZOrder(pControl,0);
		return;
	}
	int index = FindControl(pInsertAfter);
	ATLASSERT(index != -1);
	if(index == -1)
		return;

	SetControlZOrder(pControl,index);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pInsertAfter : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetZOrder(CGuiDialog * pInsertAfter)
{
	GetGuiManager()->SetDialogZOrder(this,pInsertAfter);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::OnClose()
{
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::Close(void)
{
	m_bClosed = true;
	m_bEnabled = false;
	m_bVisible = false;
	GetGuiManager()->OnCloseDlg(this);
	OnClose();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ptScreen : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
BOOL CGuiDialog::ContainsPoint(const POINT & ptScreen)
{
	if(IsInRect(ptScreen))
	{
		if(m_pBackImage)
		{
			POINT pt = {ptScreen.x - (m_x + m_pBackImage->GetX()), ptScreen.y - (m_y + m_pBackImage->GetY())};
			return m_pBackImage->ContainsPoint(pt);
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ptScreen : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
BOOL CGuiDialog::IsInRect(const POINT & ptScreen)
{
	return (ptScreen.x >= m_x && ptScreen.x < m_x + m_width && ptScreen.y >= m_y && ptScreen.y < m_y + m_height);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param ptScreen : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
DWORD CGuiDialog::OnHiTest(const POINT & ptScreen)
{
	if(!IsInRect(ptScreen))
		return HTERROR;
	if(!ContainsPoint(ptScreen)) 
		return HTTRANSPARENT;
	if( ( GetStyle() & GUI_WS_RESIZEING ) || ( GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_CONTROL) < 0) )
	{
		const int borderWidth = 3;
		stRectI rect(m_x,m_y,m_x + m_width,m_y + m_height);
		stRectI rect2 = rect;
		rect2.InflateRect(-borderWidth-1,-borderWidth-1);
		if(rect.PtInRect(ptScreen) && !rect2.PtInRect(ptScreen))
		{
			if(ptScreen.x - rect.left <= borderWidth)
			{
				if(ptScreen.y - rect.top <= borderWidth)
					return HTTOPLEFT;
				else if(rect.bottom - ptScreen.y <= borderWidth)
					return HTBOTTOMLEFT;
				return HTLEFT;
			}
			else if(rect.right - ptScreen.x <= borderWidth)
			{
				if(ptScreen.y - rect.top <= borderWidth)
					return HTTOPRIGHT;
				else if(rect.bottom - ptScreen.y <= borderWidth)
					return HTBOTTOMRIGHT;
				return HTRIGHT;
			}
			else
			{
				if(ptScreen.y - rect.top <= borderWidth)
					return HTTOP;
				else if(rect.bottom - ptScreen.y <= borderWidth)
					return HTBOTTOM;
			}
		}
	}
	
	if(m_bCaption)
	{
		if(ptScreen.y - m_y < m_nCaptionHeight) return HTCAPTION;
		return HTCLIENT;
	}
	return HTCAPTION;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pszFileName : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
// ±£¥Ê¥∞ø⁄≈‰÷√Œ™XMLŒƒº˛
HRESULT CGuiDialog::SaveGuiToXML(const char* pszFileName)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(pszFileName))
		doc.InsertEndChild(TiXmlDeclaration("1.0","gb2312",""));

	TiXmlElement* gui = XML_FindAndAddChildNode							(&doc,"gui");

	/*
	const char* szComment1 = "\r\n\
	enum GUI_CONTROL_TYPE \r\n\
	{ \r\n\
		GUI_CONTROL_BUTTON		=	0, \r\n\
		GUI_CONTROL_STATIC		=	1, \r\n\
		GUI_CONTROL_CHECKBOX	=	2,\r\n\
		GUI_CONTROL_RADIOBUTTON	=	3,\r\n\
		GUI_CONTROL_COMBOBOX	=	4,\r\n\
		GUI_CONTROL_SLIDER		=	5,\r\n\
		GUI_CONTROL_EDITBOX		=	6,\r\n\
		GUI_CONTROL_MLEDITBOX	=	7,\r\n\
		GUI_CONTROL_LISTBOX		=	8,\r\n\
		GUI_CONTROL_SCROLLBAR	=	9,\r\n\
		GUI_CONTROL_IMAGE		=	10,\r\n\
		GUI_CONTROL_ITEMCELL	=	11,\r\n\
		GUI_CONTROL_TABLE		=	12,\r\n\
\r\n\
		// Ãÿ ‚øÿº˛\r\n\
		GUI_CONTROL_CLOSEBOX	=	-1;\r\n\
	};\r\n\
\r\n\
	enum GUI_CONTROL_STATE\r\n\
	{\r\n\
		GUI_STATE_NORMAL	=	0,\r\n\
		GUI_STATE_DISABLED	=	1,\r\n\
		GUI_STATE_HIDDEN	=	2,\r\n\
		GUI_STATE_FOCUS		=	3,\r\n\
		GUI_STATE_MOUSEOVER	=	4,\r\n\
		GUI_STATE_PRESSED	=	5,\r\n\
	};";

	TiXmlComment* pComment = XML_FindAndAddComment(gui);
	pComment->SetValue(szComment1);
	//*/

	TiXmlElement* dialog = XML_FindAndAddMatchingChildNode(gui,"dialog","id",GetID());

	if(strcmp(GetName(),""))
		dialog->SetAttribute("name",GetName());

	XML_SetAttribute(dialog,"style",GetStyle(),16);
	dialog->SetAttribute("caption",m_szCaption);
	
	stRectI dlgRect(m_x,m_y,m_x + m_width,m_y + m_height);
	char szRect[64];
	dlgRect.ToString(szRect,sizeof(szRect));
	dialog->SetAttribute("rect",szRect);
	/*
	dialog->SetAttribute("x",m_x);
	dialog->SetAttribute("y",m_y);
	dialog->SetAttribute("width",m_width);
	dialog->SetAttribute("height",m_height);
	//*/

	XML_SetColorAttribute(dialog,"colortopleft",m_colorTopLeft);
	XML_SetColorAttribute(dialog,"colortopright",m_colorTopRight);
	XML_SetColorAttribute(dialog,"colorbottomleft",m_colorBottomLeft);
	XML_SetColorAttribute(dialog,"colorbottomright",m_colorBottomRight);
	XML_SetAttribute(dialog,"align",m_eAlign,16);
	TiXmlElement* controls = XML_FindAndAddChildNode(dialog,"controls");

	const char* szScript = this->GetScript();
	if(szScript)
	{
		TiXmlElement* script = XML_FindAndAddChildNode(dialog,"script");
		XML_SetNodeText(script,szScript);
	}

	for(size_t i=0;i<m_Controls.GetCount();++i)
	{
		CGuiControl* pControl = m_Controls[i];
		if(pControl->GetID() == GUI_CLOSEBOX_ID) continue;
		TiXmlElement* item = XML_FindAndAddMatchingChildNode(controls,"item","id",pControl->GetID());
		
		item->SetAttribute("type",GetGuiControlTypeDesc(pControl->GetType()));
		XML_SetAttribute(item,"style",pControl->GetStyle(),16);

		/*
		item->SetAttribute("x",pControl->GetX());
		item->SetAttribute("y",pControl->GetY());
		item->SetAttribute("width",pControl->GetWidth());
		item->SetAttribute("height",pControl->GetHeight());
		//*/
		stRectI controlRect(pControl->GetX(),pControl->GetY(),pControl->GetX() + pControl->GetWidth(),pControl->GetY() + pControl->GetHeight());
		controlRect.ToString(szRect,sizeof(szRect));
		item->SetAttribute("rect",szRect);
		item->SetAttribute("hotkey",pControl->GetHotkey().GetAccelKey().c_str());
		item->SetAttribute("isdefault",(int)pControl->m_bIsDefault);
		//XML_SetAttribute(item,"textformat",pControl->GetTextFromat(),16);

		XML_SetColorAttribute(item,"color",pControl->GetColor());
		XML_SetColorAttribute(item,"textcolor",pControl->GetTextColor());

		switch(pControl->GetType())
		{
		case GUI_CONTROL_BUTTON:
			{
				XML_SetNodeText(item,pControl->GetText());
				CGuiButton* pBtn = (CGuiButton*)pControl;
				if(pBtn->GetButtonType() == CGuiButton::ButtonType_Bitmap && pBtn->GetElementCount() > 0 && pBtn->GetElement(0)->pBmp )
				{
					IBitmap* pBmp = pBtn->GetElement(0)->pBmp;
					item->SetAttribute("pack",pBmp->GetPackName());
					item->SetAttribute("group",(int)pBmp->GetGroup());
					item->SetAttribute("frame",(int)pBmp->GetFrame());
					item->SetAttribute("framenum",(int)pBtn->GetElementCount());
				}
			}
			break;
		case GUI_CONTROL_STATIC: 
			XML_SetNodeText(item,pControl->GetText());
			break;
		case GUI_CONTROL_CHECKBOX:
			XML_SetNodeText(item,pControl->GetText());
			break;
		case GUI_CONTROL_RADIOBUTTON:
			{
				CGuiRadioButton* pRadio = (CGuiRadioButton*)pControl;
				item->SetAttribute("group",pRadio->GetButtonGroup());
				XML_SetNodeText(item,pControl->GetText());
			}
			break;
		case GUI_CONTROL_COMBOBOX:
			{
				CGuiComboBox* pComboBox = (CGuiComboBox*)pControl;
				if(pComboBox->GetItemCount())
				{
					TiXmlElement* datas = XML_FindAndAddChildNode(item,"datas");
					datas->Clear();
					for(size_t i=0;i<pComboBox->GetItemCount();++i)
					{
						TiXmlElement* item = XML_FindAndAddMatchingTextChildNode(datas,"item",pComboBox->GetItemText(i));
						//item->SetAttribute("data",(int)pComboBox->GetItemData(i));
					}
				}
			}
			break;
		case GUI_CONTROL_SLIDER:
			{
				CGuiSlider* pSlider = (CGuiSlider*)pControl;
				item->SetAttribute("min",pSlider->GetRangeMin());
				item->SetAttribute("max",pSlider->GetRangeMax());
			}
			break;
		case GUI_CONTROL_EDITBOX:
			{
				CGuiEditBox* pEdit = (CGuiEditBox*)pControl;
				item->SetAttribute("maxCharCount",pEdit->GetMaxCharCount());
				if( pEdit->GetStyle() & CGuiEditBox::GUI_ES_NUMBER )
				{
					item->SetAttribute("minNumber",pEdit->GetMinNumber());
					item->SetAttribute("maxNumber",pEdit->GetMaxNumber());
				}
				XML_SetColorAttribute(item,"caretcolor",pEdit->GetCaretColor());
			}
			break;
		case GUI_CONTROL_MLEDITBOX:
			break;
		case GUI_CONTROL_LISTBOX:
			{
				CGuiListBox* pListBox = (CGuiListBox*)pControl;
				if(pListBox->GetItemCount())
				{
					TiXmlElement* datas = XML_FindAndAddChildNode(item,"datas");
					datas->Clear();
					for(size_t i=0;i<pListBox->GetItemCount();++i)
					{
						XML_FindAndAddMatchingTextChildNode(datas,"item",pListBox->GetItemText(i));
					}
				}
			}
			break;
		case GUI_CONTROL_TAB:
			{
				CGuiTab* pListBox = (CGuiTab*)pControl;
				if(pListBox->GetItemCount())
				{
					TiXmlElement* datas = XML_FindAndAddChildNode(item,"datas");
					datas->Clear();
					for(size_t i=0;i<pListBox->GetItemCount();++i)
					{
						XML_FindAndAddMatchingTextChildNode(datas,"item",pListBox->GetItemText(i));
					}
				}
			}
			break;
		case GUI_CONTROL_SCROLLBAR:
			{
				CGuiScrollBar* pScroll = (CGuiScrollBar*)pControl;
				item->SetAttribute("min",pScroll->GetTrackStart());
				item->SetAttribute("max",pScroll->GetTrackEnd());
				item->SetAttribute("pagesize",pScroll->GetPageSize());
			}
			break;
		case GUI_CONTROL_IMAGE:
			{
				CGuiImage* pImage = (CGuiImage*)pControl;
				if(pImage->GetImgPackName())
				{
					item->SetAttribute("pack",pImage->GetImgPackName());
					item->SetAttribute("group",pImage->GetImgGroup());
					item->SetAttribute("frame",pImage->GetImgFrame());
					item->SetAttribute("backgroud",(int)pImage->m_bIsBack);
				}
			}
			break;
		case GUI_CONTROL_TABLE:
			{
				CGuiTable* pTable = (CGuiTable*)pControl;
				item->SetAttribute("cellWidth",pTable->m_cellWidth);
				item->SetAttribute("cellHeight",pTable->m_cellHeight);
				item->SetAttribute("rowCount",pTable->m_rowCount);
				item->SetAttribute("showRowCount",pTable->m_showRowCount);
				item->SetAttribute("colCount",pTable->m_colCount);
			}
			break;
		}
	}

	if(doc.SaveFile(pszFileName))
		return S_OK;

	return S_FALSE;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
CGuiDialog* CGuiDialog::GetNextDialog()
{
	return GetGuiManager()->GetNextDialog(this);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
CGuiDialog* CGuiDialog::GetPrevDialog()
{
	return GetGuiManager()->GetPrevDialog(this);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool CGuiDialog::IsFocus()
{
	return GetGuiManager()->IsFocus(this);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pt : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::ClientToScreen(POINT* pt)
{
	pt->x += m_x;
	pt->y += m_y;
	if(m_bCaption)
		pt->y += m_nCaptionHeight;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param rc : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::ClientToScreen(RECT* rc)
{
	rc->left += m_x;
	rc->right += m_x;
	rc->top += m_y;
	rc->bottom += m_y;
	if(m_bCaption)
	{
		rc->top += m_nCaptionHeight;
		rc->bottom += m_nCaptionHeight;
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pt : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::ScreenToClient(POINT* pt)
{
	pt->x -= m_x;
	pt->y -= m_y;
	if(m_bCaption)
		pt->y -= m_nCaptionHeight;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param rc : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::ScreenToClient(RECT* rc)
{
	rc->left -= m_x;
	rc->right -= m_x;
	rc->top -= m_y;
	rc->bottom -= m_y;
	if(m_bCaption)
	{
		rc->top -= m_nCaptionHeight;
		rc->bottom -= m_nCaptionHeight;
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::UpdateLocation()
{
	if(m_eAlign & GUI_ALIGN_TOP)
	{
		m_y = 0;
	}
	else if(m_eAlign & GUI_ALIGN_VCENTER)
	{
		m_y = (GetDevice()->GetHeight()-GetHeight())/2;
	}
	else if(m_eAlign & GUI_ALIGN_BOTTOM)
	{
		m_y = GetDevice()->GetHeight() - GetHeight();

	}

	if(m_eAlign & GUI_ALIGN_LEFT)
	{
		m_x = 0;
	}
	else if(m_eAlign & GUI_ALIGN_HCENTER)
	{
		m_x = (GetDevice()->GetWidth() - GetWidth())/2;
	}
	else if(m_eAlign & GUI_ALIGN_RIGHT)
	{
		m_x = GetDevice()->GetWidth() - GetWidth();
	}

	if(m_pBtnClose)
	{
		int width = 15;
		int height = 15;
		m_pBtnClose->SetLocation(GetWidth()-3-width,-2 - height);
	}
	
	stRectI rc1(0,0,GetWidth(),GetHeight());
	CuttRect9(rc1,m_borderTopLeft,m_borderRightBottom,m_BackRects);
	for(size_t i=0;i<9;++i)
	{
		m_BackRects[i] &= rc1;
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int CGuiDialog::GetClientWidth()
{
	return GetWidth();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int CGuiDialog::GetClientHeight()
{
	if(m_bCaption) return GetHeight() - m_nCaptionHeight;
	return GetHeight();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param script : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetScript(const char* script)
{
#ifdef _DEBUG
	m_strScript = script;
#endif
	if(m_lua == NULL)
	{
		m_lua = new CLuaState(true);
		GuiScriptInit(*m_lua);
	}

	lua_dostring((*m_lua),script);
	set_lua_globalvalue(*m_lua,"thisHandle",static_cast<tGuiDialogHandle>(this));
	OnInitScript();
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::OnInitScript()
{
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nIDFirstButton : √Ë ˆ
 * \param nIDLastButton : √Ë ˆ
 * \param nIDCheckButton : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::CheckRadioButton(int nIDFirstButton,int nIDLastButton,int nIDCheckButton )
{
	CGuiCheckBox* pCheckBox = GetRadioButton(nIDCheckButton);
	if(pCheckBox) pCheckBox->SetChecked(true);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param nIDFirstButton : √Ë ˆ
 * \param nIDLastButton : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int CGuiDialog::GetCheckedRadioButton(int nIDFirstButton,int nIDLastButton )
{
	for(int nID = nIDFirstButton; nID <= nIDLastButton;nID++)
	{
		CGuiCheckBox* pCheckBox = GetRadioButton(nID);
		if(pCheckBox && pCheckBox->GetChecked())
		{
			return nID - nIDFirstButton;
		}
	}
	return 0;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param b : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetEnabled(bool b)
{
	m_bEnabled = b;
	if(!b)
	{
		if(IsFocus()) GetGuiManager()->ResetFocus();
	}
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param b : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void CGuiDialog::SetVisible(bool b)
{
	m_bVisible = b;
	if(!b)
	{
		if(IsFocus()) GetGuiManager()->ResetFocus();
	}
}