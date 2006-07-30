#include "stdafx.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_app.h"
#include "misc/rdoprocess_string.h"
#include "ctrl/rdoprocess_pagectrl.h"
#include "ctrl/ColourPicker/ColourPopup.h"
#include "resource.h"
#include "rdoprocess_object.h"
#include "rdoprocess_object_chart.h"
#include "rdoprocess_object_flowchart.h"
#include "rdoprocess_shape.h"

#include "method/process2rdo/rdoprocess_generation_type_MJ.h" // MJ диалоговое окно кнопки типа генерирования на тулбаре

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
	rpapp.msg().connect( this, rp::msg::RP_FLOWSTATE_CHANGED );
	rpapp.msg().connect( this, rp::msg::RP_OBJ_SELCHANGED );
	rpapp.msg().connect( this, rp::msg::RP_OBJ_BEFOREDELETE );
}

void RPMainFrameMsg::notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	RPMainFrame* frame = static_cast<RPMainFrame*>(AfxGetMainWnd());
	if ( message == rp::msg::RP_FLOWSTATE_CHANGED ) {
		bool enable = wParam != RPProject::flow_none;
		bool check_select    = false;
		bool check_connector = false;
		bool check_rotate    = false;
		if ( enable ) {
			switch ( wParam ) {
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
	ON_COMMAND(ID_GENERATE, OnGenerate)
	ON_COMMAND(ID_GEN_TYPE, OnGenType)
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
	m_msg( NULL )
{
}

RPMainFrame::~RPMainFrame()
{
}

int RPMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndToolBar.LoadToolBar( IDR_MAINFRAME );
	m_wndToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	m_wndToolBar.SetWindowText( rp::format( IDS_TOOLBAR_MAIN ).c_str() );
	m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_SELECT, false );
	m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_CONNECTOR, false );
	m_wndToolBar.GetToolBarCtrl().EnableButton( ID_FLOW_ROTATE, false );

	m_wndStyleAndColorToolBar.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY );
	m_wndStyleAndColorToolBar.LoadToolBar( IDR_STYLEANDCOLOR );
	m_wndStyleAndColorToolBar.ModifyStyle( 0, TBSTYLE_FLAT );
	m_wndStyleAndColorToolBar.SendMessage( TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS );
	m_wndStyleAndColorToolBar.SetWindowText( rp::format( IDS_TOOLBAR_STYLEANDCOLOR ).c_str() );

	int index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_BRUSH );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );
	index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_PEN );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );
	index = m_wndStyleAndColorToolBar.SendMessage( TB_COMMANDTOINDEX, ID_BTN_FILL_FONT );
	m_wndStyleAndColorToolBar.SetButtonStyle( index, m_wndStyleAndColorToolBar.GetButtonStyle( index ) | TBSTYLE_DROPDOWN );

	m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_BRUSH, false );
	m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_PEN  , false );
	m_wndStyleAndColorToolBar.GetToolBarCtrl().EnableButton( ID_BTN_FILL_TEXT , false );

	//MJ start 02.04.06
	m_wndToolBlockBarMJ.CreateEx( this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	m_wndToolBlockBarMJ.LoadToolBar( TOOLBARBLOCKMJ );
	m_wndToolBlockBarMJ.ModifyStyle( 0, TBSTYLE_FLAT );
	//MJstop

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	projectBar.Create( rp::format( ID_DOCK_PROJECT_BAR ).c_str(), this, 0 );
	projectBar.EnableDocking( CBRS_ALIGN_ANY );

	CListCtrl* listctrl = new CListCtrl();
	listctrl->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl->InsertItem( 0, "test 1" );
	listctrl->InsertItem( 1, "test 2" );
	listctrl->InsertItem( 2, "test 3" );
	listctrl->InsertItem( 3, "test 4" );
	projectBar.insertPage( listctrl, "Первая страница" );

	CListCtrl* listctrl2 = new CListCtrl();
	listctrl2->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl2->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl2->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl2->InsertItem( 0, "q 1" );
	listctrl2->InsertItem( 1, "qqqqqqqqqqq 2" );
	listctrl2->InsertItem( 2, "q 3" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	listctrl2->InsertItem( 3, "q 4" );
	projectBar.insertPage( listctrl2, "Вторая страница" );

	CListCtrl* listctrl3 = new CListCtrl();
	listctrl3->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl3->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl3->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl3->InsertItem( 0, "объект 1" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	listctrl3->InsertItem( 1, "объект 2" );
	projectBar.insertPage( listctrl3, "Третья страница" );

	projectBar.selectFirst();

	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndStyleAndColorToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBlockBarMJ.EnableDocking( CBRS_ALIGN_ANY );
	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &m_wndToolBar );
	dockControlBarBesideOf( m_wndStyleAndColorToolBar, m_wndToolBar );
	dockControlBarBesideOf( m_wndToolBlockBarMJ, m_wndStyleAndColorToolBar );
	DockControlBar( &projectBar, AFX_IDW_DOCKBAR_LEFT );

	m_msg = new RPMainFrameMsg();

	return 0;
}

BOOL RPMainFrame::DestroyWindow()
{
	if ( m_msg ) {
		rpapp.msg().disconnect( m_msg );
		delete m_msg;
		m_msg = NULL;
	}
	return CMDIFrameWnd::DestroyWindow();
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
}

BOOL RPMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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
	rpapp.project()->setFlowState( RPProject::flow_select );
}

void RPMainFrame::OnFlowConnector()
{
	rpapp.project()->setFlowState( RPProject::flow_connector );
}

void RPMainFrame::OnFlowRotate()
{
	rpapp.project()->setFlowState( RPProject::flow_rotate );
}

	// MJ start 29.03.06 обработчик кнопки генерирования
void RPMainFrame::OnGenerate() 
{
	rpapp.project()->generate();
}
	// MJ stop

void RPMainFrame::OnGenType() 
{
	RP_GENERATION_TYPE_MJ dlg;
	dlg.DoModal();	
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
	RPObject* obj = rpapp.project()->getActiveObject();
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
	RPObject* obj = rpapp.project()->getActiveObject();
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
	RPObject* obj = rpapp.project()->getActiveObject();
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
