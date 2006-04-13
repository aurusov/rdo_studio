#include "stdafx.h"
#include "rdoprocess_toolbar.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPToolBar
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPToolBar, CToolBar)
	//{{AFX_MSG_MAP(RPToolBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPToolBar::RPToolBar():
	CToolBar(),
	color_brush( RGB(0xFF, 0xFF, 0xA0) ),
	color_pen  ( RGB(0xA0, 0xA0, 0xFF) ),
	color_text ( RGB(0xF0, 0xF0, 0xF0) )
{
}

RPToolBar::~RPToolBar()
{
}

void RPToolBar::OnPaint()
{
	CToolBar::OnPaint();
	CClientDC dc( this );
	CPen pen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
	CBrush brush( color_brush );
	CPen* old_pen     = dc.SelectObject( &pen );
	CBrush* old_brush = dc.SelectObject( &brush );

	CToolBarCtrl& tb = GetToolBarCtrl();
	CRect rect;
	tb.GetRect( ID_BTN_FILL_BRUSH, rect );
	rect.right = rect.left + LOWORD(tb.GetButtonSize());
	rect.right  -= 3;
	rect.bottom -= 2;
	rect.left    = rect.right - 10;
	rect.top     = rect.bottom - 5;
	dc.Rectangle( rect );

	dc.SelectObject( old_brush );
	brush.DeleteObject();
	brush.CreateSolidBrush( color_pen );
	old_brush = dc.SelectObject( &brush );
	tb.GetRect( ID_BTN_FILL_PEN, rect );
	rect.right = rect.left + LOWORD(tb.GetButtonSize());
	rect.right  -= 3;
	rect.bottom -= 2;
	rect.left    = rect.right - 10;
	rect.top     = rect.bottom - 5;
	dc.Rectangle( rect );

	dc.SelectObject( old_brush );
	brush.DeleteObject();
	brush.CreateSolidBrush( color_text );
	old_brush = dc.SelectObject( &brush );
	tb.GetRect( ID_BTN_FILL_FONT, rect );
	rect.right = rect.left + LOWORD(tb.GetButtonSize());
	rect.right  -= 3;
	rect.bottom -= 2;
	rect.left    = rect.right - 10;
	rect.top     = rect.bottom - 5;
	dc.Rectangle( rect );

	dc.SelectObject( old_pen );
	dc.SelectObject( old_brush );
}
