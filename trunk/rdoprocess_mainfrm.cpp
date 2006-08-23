#include "stdafx.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_project.h"
#include "rdoprocess_app.h"
#include "ctrl/rdoprocess_pagectrl.h"
#include "ctrl/ColourPicker/ColourPopup.h"
#include "resource.h"
#include <rdoprocess_object.h>
#include <rdoprocess_object_chart.h>
#include <rdoprocess_object_flowchart.h>
#include <rdoprocess_shape.h>
#include <rdoprocess_method.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPMainFrameMsg
// ----------------------------------------------------------------------------
RPMainFrameMsg::RPMainFrameMsg(): RPObject( NULL )
{
	rpMethod::project->msg().connect( this, rp::msg::RP_FLOWSTATE_CHANGED );
	rpMethod::project->msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
	rpMethod::project->msg().connect( this, rp::msg::RP_OBJ_BEFOREDELETE );
}

void RPMainFrameMsg::notify( RPObject* from, UINT message, void* param )
{
	RPMainFrame* frame = static_cast<RPMainFrame*>(AfxGetMainWnd());
	if ( message == rp::msg::RP_FLOWSTATE_CHANGED ) {
		bool enable = *static_cast<RPProject::FlowState*>(param) != RPProject::flow_none;
		bool check_select    = false;
		bool check_connector = false;
		bool check_rotate    = false;
		if ( enable ) {
			switch ( *static_cast<RPProject::FlowState*>(param) ) {
				case RPProject::flow_select   : {
					check_select    = true;
					check_connector = false;
					check_rotate    = false;
					break;
				}
				case RPProject::flow_connector: {
					check_select    = false;
					check_connector = true;
					check_rotate    = false;
					break;
				}
				case RPProject::flow_rotate   : {
					check_select    = false;
					check_connector = false;
					check_rotate    = true;
					break;
				}
			}
		}
		frame->m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_SELECT   , enable );
		frame->m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_CONNECTOR, enable );
		frame->m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_ROTATE   , enable );
		frame->m_wndToolBar.GetToolBarCtrl().CheckButton( ID_FLOW_SELECT   , check_select );
		frame->m_wndToolBar.GetToolBarCtrl().CheckButton( ID_FLOW_CONNECTOR, check_connector );
		frame->m_wndToolBar.GetToolBarCtrl().CheckButton( ID_FLOW_ROTATE   , check_rotate );
	} else if ( message == rp::msg::RP_OBJ_SELCHANGED ) {
		bool fill_brush = false;
		bool fill_pen   = false;
		bool fill_text  = false;
		if ( from->isSelected() && from->isChartObject() ) {
			fill_pen   = static_cast<RPObjectChart*>(from)->isFlowChart() ? false : true;
			fill_brush = static_cast<RPObjectChart*>(from)->isShape() || static_cast<RPObjectChart*>(from)->isFlowChart();
			fill_text  = static_cast<RPObjectChart*>(from)->isShape();
		}
		frame->m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_BRUSH, fill_brush );
		frame->m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_PEN  , fill_pen );
		frame->m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_TEXT , fill_text );
	} else if ( message == rp::msg::RP_OBJ_BEFOREDELETE ) {
		frame->m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_BRUSH, false );
		frame->m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_PEN  , false );
		frame->m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_TEXT , false );
	}
}

// ----------------------------------------------------------------------------
// ---------- RPMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RPMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(RPMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(RPMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_FLOW_CONNECTOR, OnUpdateFlowSelect)
	ON_COMMAND(ID_FLOW_SELECT, OnFlowSelect)
	ON_COMMAND(ID_FLOW_ROTATE, OnFlowRotate)
	ON_COMMAND(ID_FLOW_CONNECTOR, OnFlowConnector)
	ON_UPDATE_COMMAND_UI(ID_BTN_FILL_BRUSH, OnUpdateBtnFillBrush)
	ON_COMMAND(ID_BTN_FILL_BRUSH, OnBtnFillBrush)
	ON_COMMAND(ID_BTN_FILL_PEN, OnBtnFillPen)
	ON_UPDATE_COMMAND_UI(ID_FLOW_ROTATE, OnUpdateFlowSelect)
	ON_UPDATE_COMMAND_UI(ID_FLOW_SELECT, OnUpdateFlowSelect)
	ON_UPDATE_COMMAND_UI(ID_BTN_FILL_FONT, OnUpdateBtnFillBrush)
	ON_UPDATE_COMMAND_UI(ID_BTN_FILL_PEN, OnUpdateBtnFillBrush)
	ON_COMMAND(ID_BTN_FILL_TEXT, OnBtnFillText)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
	ON_MESSAGE(CPN_SELENDDEFAULT,OnSelEndDefault)
	ON_WM_SYSCOLORCHANGE()
	//  онтролы методов получают значени€ от 40000. ¬ерхна€ граница не контролируетс€. ѕересечение с ресурсами не контроилируетс€.
	ON_COMMAND_RANGE( 40000, 50000, OnMethodCommandRange )
	ON_UPDATE_COMMAND_UI_RANGE( 40000, 50000, OnMethodUpdateRange )
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

RPMainFrame::RPMainFrame():
	CMDIFrameWnd(),
	last_docked( NULL ),
	m_msg( NULL )
{
}

RPMainFrame::~RPMainFrame()
{
}

int RPMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CMDIFrameWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;

	m_wndToolBar.init( this, IDR_TOOLBAR_MAINFRAME, IDR_TOOLBAR_MAINFRAME_D );
	m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_SELECT, false );
	m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_CONNECTOR, false );
	m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_ROTATE, false );

	m_wndStyleAndColorToolBar.init( this, IDR_TOOLBAR_STYLEANDCOLOR );
	m_wndStyleAndColorToolBar.SendMessage( TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS );

	int index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_BRUSH );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );
	index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_PEN );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );
	index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_FONT );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );

	m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_BRUSH, false );
	m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_PEN  , false );
	m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_TEXT , false );

	if ( !m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) ) ) {
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	projectBar.Create( rp::format( ID_DOCK_PROJECT_BAR ).c_str(), this, 0 );
	projectBar.EnableDocking( CBRS_ALIGN_ANY );

	EnableDocking( CBRS_ALIGN_ANY );

	insertToolBar( &m_wndToolBar );
	insertToolBar( &m_wndStyleAndColorToolBar );
	DockControlBar( &projectBar, AFX_IDW_DOCKBAR_LEFT );

	m_msg = new RPMainFrameMsg();
	m_msg->setName( "__mainform_msg" );

	return 0;
}

BOOL RPMainFrame::DestroyWindow()
{
	if ( m_msg ) {
		rpMethod::project->msg().disconnect( m_msg );
		delete m_msg;
		m_msg = NULL;
	}
	return CMDIFrameWnd::DestroyWindow();
}

void RPMainFrame::insertToolBar( CToolBar* toolbar )
{
	if ( !last_docked ) {
		DockControlBar( toolbar );
	} else {
		dockControlBarBesideOf( *toolbar, *last_docked );
	}
	last_docked = toolbar;
}

void RPMainFrame::dockControlBarBesideOf( CControlBar& bar, CControlBar& baseBar )
{
	RecalcLayout( TRUE );

	CRect rect;
	baseBar.GetWindowRect( rect );

	DWORD dw = baseBar.GetBarStyle();

	UINT n = 0;
	n = ( dw & CBRS_ALIGN_TOP          ) ? AFX_IDW_DOCKBAR_TOP    : n;
	n = ( dw & CBRS_ALIGN_BOTTOM && !n ) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = ( dw & CBRS_ALIGN_LEFT   && !n ) ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = ( dw & CBRS_ALIGN_RIGHT  && !n ) ? AFX_IDW_DOCKBAR_RIGHT  : n;

	int dx = 0;
	int dy = 0;
	if ( n == AFX_IDW_DOCKBAR_TOP || n == AFX_IDW_DOCKBAR_BOTTOM ) dx = 1;
	if ( n == AFX_IDW_DOCKBAR_LEFT || n == AFX_IDW_DOCKBAR_RIGHT ) dy = 1;

	rect.OffsetRect( dx, dy );

	DockControlBar( &bar, n, rect );
	
	last_docked = &bar;
}

BOOL RPMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !CMDIFrameWnd::PreCreateWindow(cs) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

#ifdef _DEBUG
void RPMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void RPMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif

void RPMainFrame::OnUpdateFlowSelect( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( m_wndToolBar.GetToolBarCtrl().IsButtonEnabled(pCmdUI->m_nID) );
	pCmdUI->SetCheck( m_wndToolBar.GetToolBarCtrl().IsButtonChecked(pCmdUI->m_nID) );
}

void RPMainFrame::OnFlowSelect()
{
	rpMethod::project->setFlowState( RPProject::flow_select );
}

void RPMainFrame::OnFlowConnector()
{
	rpMethod::project->setFlowState( RPProject::flow_connector );
}

void RPMainFrame::OnFlowRotate()
{
	OnSysColorChange();
	rpMethod::project->setFlowState( RPProject::flow_rotate );
}

void RPMainFrame::OnUpdateBtnFillBrush( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( m_wndStyleAndColorToolBar.GetToolBarCtrl().IsButtonEnabled(pCmdUI->m_nID) );
}

BOOL RPMainFrame::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	NMHDR* data = reinterpret_cast<NMHDR*>(lParam);
	if ( data->code == TBN_DROPDOWN ) {
		NMTOOLBAR* tb_data = reinterpret_cast<NMTOOLBAR*>(data);
		RPToolBar* tb = static_cast<RPToolBar*>(CWnd::FromHandle( data->hwndFrom ));
		CRect rect;
		tb->GetItemRect( tb->CommandToIndex( tb_data->iItem ), rect );
		tb->ClientToScreen( rect );
		unsigned int resID;
		COLORREF color;
		switch ( tb_data->iItem ) {
			case ID_BTN_FILL_BRUSH: color = tb->color_brush; resID = IDS_COLORPICKER_NONE_BRUSH; break;
			case ID_BTN_FILL_PEN  : color = tb->color_pen; resID = IDS_COLORPICKER_NONE_PEN; break;
			case ID_BTN_FILL_TEXT : color = tb->color_text; resID = IDS_COLORPICKER_NONE_TEXT; break;
			default: resID = IDS_COLORPICKER_DEFAULT; break;
		}
		new CColourPopup( tb_data->iItem, CPoint(rect.left, rect.bottom), color, this, rp::format( resID ).c_str(), rp::format( IDS_COLORPICKER_MORE ).c_str() );
		return false;
	}
	return CMDIFrameWnd::OnNotify( wParam, lParam, pResult );
}

LONG RPMainFrame::OnSelEndOK( UINT lParam, LONG wParam )
{
	COLORREF color = static_cast<COLORREF>(lParam);
	switch ( wParam ) {
		case ID_BTN_FILL_BRUSH: {
			m_wndStyleAndColorToolBar.color_brush = color;
			m_wndStyleAndColorToolBar.empty_brush = false;
			OnBtnFillBrush();
			break;
		}
		case ID_BTN_FILL_PEN  : {
			m_wndStyleAndColorToolBar.color_pen = color;
			m_wndStyleAndColorToolBar.empty_pen = false;
			OnBtnFillPen();
			break;
		}
		case ID_BTN_FILL_TEXT : {
			m_wndStyleAndColorToolBar.color_text = color;
			m_wndStyleAndColorToolBar.empty_text = false;
			OnBtnFillText();
			break;
		}
		default: break;
	}
	m_wndStyleAndColorToolBar.RedrawWindow();
	return TRUE;
}

LONG RPMainFrame::OnSelEndDefault(UINT lParam, LONG wParam)
{
	switch ( wParam ) {
		case ID_BTN_FILL_BRUSH: {
			m_wndStyleAndColorToolBar.empty_brush = true;
			OnBtnFillBrush();
			break;
		}
		case ID_BTN_FILL_PEN  : {
			m_wndStyleAndColorToolBar.empty_pen = true;
			OnBtnFillPen();
			break;
		}
		case ID_BTN_FILL_TEXT : {
			m_wndStyleAndColorToolBar.empty_text = true;
			OnBtnFillText();
			break;
		}
		default: break;
	}
	m_wndStyleAndColorToolBar.RedrawWindow();
	return TRUE;
}

void RPMainFrame::OnBtnFillBrush()
{
	RPObject* obj = rpMethod::project->getActiveObject();
	if ( obj ) {
		if ( obj->isChartObject() ) {
			CBrush brush;
			if ( !m_wndStyleAndColorToolBar.empty_brush ) {
				brush.CreateSolidBrush( m_wndStyleAndColorToolBar.color_brush );
			} else {
				brush.CreateStockObject( NULL_BRUSH );
			}
			if ( static_cast<RPObjectChart*>(obj)->isFlowChart() ) {
				if ( m_wndStyleAndColorToolBar.empty_brush ) {
					brush.DeleteObject();
					brush.CreateSolidBrush( RGB(0xFF, 0xFF, 0xFF) );
				}
				static_cast<RPObjectFlowChart*>(obj)->setBgBrush( brush );
			} else if ( static_cast<RPObjectChart*>(obj)->isShape() ) {
				static_cast<RPShape*>(obj)->setBgBrush( brush );
			}
		}
	}
}

void RPMainFrame::OnBtnFillPen()
{
	RPObject* obj = rpMethod::project->getActiveObject();
	if ( obj && obj->isChartObject() ) {
		const CPen& old_pen = static_cast<RPObjectChart*>(obj)->getDefaultPen();
		CPen pen;
		bool flag = false;
		if ( !m_wndStyleAndColorToolBar.empty_pen ) {
			LOGPEN lp;
			if ( const_cast<CPen&>(old_pen).GetLogPen( &lp ) ) {
				lp.lopnColor = m_wndStyleAndColorToolBar.color_pen;
				flag = pen.CreatePenIndirect( &lp ) ? true : false;
			} else {
				EXTLOGPEN exlp;
				if ( const_cast<CPen&>(old_pen).GetExtLogPen( &exlp ) ) {
					LOGBRUSH lb;
					lb.lbStyle = exlp.elpBrushStyle;
					lb.lbColor = m_wndStyleAndColorToolBar.color_pen;
					lb.lbHatch = 0;
					flag = pen.CreatePen( exlp.elpPenStyle, exlp.elpWidth, &lb ) ? true : false;
				}
			}
		} else {
			LOGPEN lp;
			if ( const_cast<CPen&>(old_pen).GetLogPen( &lp ) ) {
				lp.lopnStyle = PS_NULL;
				flag = pen.CreatePenIndirect( &lp ) ? true : false;
			} else {
				EXTLOGPEN exlp;
				if ( const_cast<CPen&>(old_pen).GetExtLogPen( &exlp ) ) {
					LOGBRUSH lb;
					lb.lbStyle = BS_NULL;
					lb.lbHatch = 0;
					flag = pen.CreatePen( exlp.elpPenStyle, exlp.elpWidth, &lb ) ? true : false;
				}
			}
		}
		if ( flag ) static_cast<RPObjectChart*>(obj)->setPen( pen );
	}
}

void RPMainFrame::OnBtnFillText()
{
	RPObject* obj = rpMethod::project->getActiveObject();
	if ( obj && obj->isChartObject() && static_cast<RPObjectChart*>(obj)->isShape() ) {
		LOGFONT lf;
		COLORREF color;
		if ( const_cast<CFont&>(static_cast<RPShape*>(obj)->getTextFont( color )).GetLogFont( &lf ) ) {
			CFont font;
			if ( font.CreateFontIndirect( &lf ) ) {
				static_cast<RPShape*>(obj)->setTextFont( font, m_wndStyleAndColorToolBar.color_text, !m_wndStyleAndColorToolBar.empty_text );
			}
		}
	}
}

void RPMainFrame::OnSysColorChange()
{
}

void RPMainFrame::OnMethodCommandRange( UINT id )
{
	rpMethod::RPMethod* method = rpMethod::project->getMethodByButton( id );
	if ( method ) {
		method->buttonCommand( id );
	}
}

void RPMainFrame::OnMethodUpdateRange( CCmdUI* pCmdUI )
{
	rpMethod::RPMethod* method = rpMethod::project->getMethodByButton( pCmdUI->m_nID );
	if ( method ) {
		RPCtrlToolbar::ButtonUpdate button_update( pCmdUI->m_nID );
		method->buttonUpdate( button_update );
		pCmdUI->Enable( button_update.enable );
	}
}
