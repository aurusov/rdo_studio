/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_toolbar.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      12.04.2006
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_toolbar.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPToolbarToolTip
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPToolbarToolTip, CToolBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void RPToolbarToolTip::updateToolTips()
{
	std::map< unsigned int, rp::string >::iterator it = button_text.begin();
	while ( it != button_text.end() ) {
		toolTip.DelTool( this, it->first + 1 );
		it++;
	}
//	toolTip.AddTool( this, "" );
	CToolBarCtrl& tb_ctrl = GetToolBarCtrl();
	for ( int i = 0; i < tb_ctrl.GetButtonCount(); i++ ) {
		CRect rect;
		GetItemRect( i, &rect );
		toolTip.AddTool( this, button_text[i].c_str(), rect, i+1 );
	}
}

void RPToolbarToolTip::OnSize( UINT nType, int cx, int cy )
{
	CToolBar::OnSize( nType, cx, cy );
	updateToolTips();
}

// --------------------------------------------------------------------------------
// -------------------- RPCtrlToolbarMFC
// --------------------------------------------------------------------------------
RPCtrlToolbarMFC::RPCtrlToolbarMFC( CWnd* parent ):
	RPCtrlToolbar()
{
	toolbar.CreateEx( parent, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	toolbar.LoadToolBar( IDR_TOOLBAR_EMPTY );
	toolbar.ModifyStyle( 0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT );
	toolbar.SetWindowText( rp::format( IDR_TOOLBAR_EMPTY ).c_str() );
	toolbar.EnableDocking( CBRS_ALIGN_ANY );
	toolbar.toolTip.Create( &toolbar );
//	toolbar.toolTip.AddTool( &toolbar, "Панель", 0 );
}

RPCtrlToolbarMFC::~RPCtrlToolbarMFC()
{
	std::list< RPPixmap* >::iterator it = bitmaps.begin();
	while ( it != bitmaps.end() ) {
		delete *it;
		it++;
	}
	bitmaps.clear();
}

int RPCtrlToolbarMFC::insertButton( rpMethod::RPMethod* method, char* xpm[], const rp::string& tool_tip )
{
	CToolBarCtrl& tb_ctrl = toolbar.GetToolBarCtrl();
	if ( tb_ctrl.GetButtonCount() == 1 ) {
		TBBUTTON btn;
		if ( tb_ctrl.GetButton( 0, &btn ) && btn.idCommand == ID_BTN_EMPTY ) {
			tb_ctrl.DeleteButton( 0 );
		}
	}

	RPPixmap* bmp_xpm = new RPPixmap( xpm );
	bitmaps.push_back( bmp_xpm );
	tb_ctrl.AddBitmap( 1, &bmp_xpm->getCBitmap() );

	TBBUTTON btn;
	btn.iBitmap   = tb_ctrl.GetButtonCount() + 1;
	btn.idCommand = rpMethod::project->getNewButtonID( method );
	btn.fsState   = TBSTATE_ENABLED;
	btn.fsStyle   = TBSTYLE_BUTTON;
#if _MSC_VER > 1200
	btn.dwData    = reinterpret_cast<DWORD_PTR>(method);
#else
	btn.dwData    = reinterpret_cast<DWORD>(method);
#endif
	btn.iString   = -1;
	tb_ctrl.AddButtons( 1, &btn );

	toolbar.button_text[tb_ctrl.GetButtonCount()-1] = tool_tip;
	toolbar.updateToolTips();

	return btn.idCommand;
}

void RPCtrlToolbarMFC::setCaption( const rp::string& caption )
{
	toolbar.SetWindowText( caption.c_str() );
}

// --------------------------------------------------------------------------------
// -------------------- RDOProcToolBar
// --------------------------------------------------------------------------------
void RDOProcToolBar::init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID )
{
	CreateEx( parent, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	LoadToolBar( tbResID );
	ModifyStyle( 0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT );
	SetWindowText( rp::format( tbResID ).c_str() );

	if ( tbDisabledImageResID != -1 ) {
		disabledImage.Create( tbDisabledImageResID, 16, 0, 0xFF00FF );
		GetToolBarCtrl().SetDisabledImageList( &disabledImage );
	}

	EnableDocking( CBRS_ALIGN_ANY );
}

// --------------------------------------------------------------------------------
// -------------------- RPToolBar
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPToolBar, RDOProcToolBar)
	ON_WM_PAINT()
END_MESSAGE_MAP()

RPToolBar::RPToolBar():
	RDOProcToolBar(),
	color_brush( RGB(0xFF, 0xFF, 0xA0) ),
	color_pen  ( RGB(0xA0, 0xA0, 0xFF) ),
	color_text ( RGB(0xF0, 0xF0, 0xF0) ),
	empty_brush( false ),
	empty_pen( false ),
	empty_text( false )
{
}

RPToolBar::~RPToolBar()
{
}

void RPToolBar::OnPaint()
{
	RDOProcToolBar::OnPaint();
	CClientDC dc( this );

	CToolBarCtrl& tb = GetToolBarCtrl();
	CRect  rect;
	CPen   pen;
	CBrush brush;
	if ( !empty_brush && tb.IsButtonEnabled(ID_BTN_FILL_BRUSH) ) {
		pen.CreatePen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
		brush.CreateSolidBrush( color_brush );
	} else {
		pen.CreatePen( PS_SOLID, 1, RGB(0x80, 0x80, 0x80) );
		brush.CreateStockObject( NULL_BRUSH );
	}
	CPen*   old_pen   = dc.SelectObject( &pen );
	CBrush* old_brush = dc.SelectObject( &brush );
	tb.GetRect( ID_BTN_FILL_BRUSH, rect );
	rect.right = rect.left + LOWORD(tb.GetButtonSize());
	rect.right  -= 3;
	rect.bottom -= 2;
	rect.left    = rect.right - 10;
	rect.top     = rect.bottom - 5;
	dc.Rectangle( rect );

	dc.SelectObject( old_pen );
	dc.SelectObject( old_brush );
	pen.DeleteObject();
	brush.DeleteObject();
	if ( !empty_pen && tb.IsButtonEnabled(ID_BTN_FILL_PEN) ) {
		pen.CreatePen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
		brush.CreateSolidBrush( color_pen );
	} else {
		pen.CreatePen( PS_SOLID, 1, RGB(0x80, 0x80, 0x80) );
		brush.CreateStockObject( NULL_BRUSH );
	}
	old_pen   = dc.SelectObject( &pen );
	old_brush = dc.SelectObject( &brush );
	tb.GetRect( ID_BTN_FILL_PEN, rect );
	rect.right = rect.left + LOWORD(tb.GetButtonSize());
	rect.right  -= 3;
	rect.bottom -= 2;
	rect.left    = rect.right - 10;
	rect.top     = rect.bottom - 5;
	dc.Rectangle( rect );

	dc.SelectObject( old_pen );
	dc.SelectObject( old_brush );
	pen.DeleteObject();
	brush.DeleteObject();
	if ( !empty_text && tb.IsButtonEnabled(ID_BTN_FILL_FONT) ) {
		pen.CreatePen( PS_SOLID, 1, RGB(0x00, 0x00, 0x00) );
		brush.CreateSolidBrush( color_text );
	} else {
		pen.CreatePen( PS_SOLID, 1, RGB(0x80, 0x80, 0x80) );
		brush.CreateStockObject( NULL_BRUSH );
	}
	old_pen   = dc.SelectObject( &pen );
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
