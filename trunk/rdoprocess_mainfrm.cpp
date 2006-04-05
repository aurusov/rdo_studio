#include "stdafx.h"
#include "rdoprocess_mainfrm.h"
#include "rdoprocess_app.h"
#include "rdoprocess_string.h"
#include "ctrl/rdoprocess_pagectrl.h"
#include "resource.h"
#include "rdoprocess_object.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RPMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(RPMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(RPMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_FLOW_CONNECTOR, OnUpdateFlowConnector)
	ON_UPDATE_COMMAND_UI(ID_FLOW_ROTATE, OnUpdateFlowRotate)
	ON_UPDATE_COMMAND_UI(ID_FLOW_SELECT, OnUpdateFlowSelect)
	ON_COMMAND(ID_FLOW_SELECT, OnFlowSelect)
	ON_COMMAND(ID_FLOW_ROTATE, OnFlowRotate)
	ON_COMMAND(ID_FLOW_CONNECTOR, OnFlowConnector)
	ON_COMMAND(ID_GENERATE, OnGenerate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

RPMainFrame::RPMainFrame()
{
}

RPMainFrame::~RPMainFrame()
{
}

int RPMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	bool winxp = false;
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	if ( ::GetVersionEx( &osv ) ) {
		winxp = osv.dwMajorVersion >= 5 && osv.dwMinorVersion == 1;
	}
	m_wndToolBar.CreateEx( this, winxp ? 0 : TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	m_wndToolBar.LoadToolBar( IDR_MAINFRAME );

	//MJ start 02.04.06
	m_wndToolBlockBarMJ.CreateEx( this, winxp ? 0 : TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_SIZE_DYNAMIC );
	m_wndToolBlockBarMJ.LoadToolBar(TOOLBARBLOCKMJ);
	//MJstop


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	projectBar.Create( rp::string::format( ID_DOCK_PROJECT_BAR ).c_str(), this, 0 );
	projectBar.EnableDocking( CBRS_ALIGN_ANY );

	CListCtrl* listctrl = new CListCtrl();
	listctrl->Create( LVS_LIST | LVS_SINGLESEL, CRect(0,0,1,1), projectBar.prepareNewPage(), -1 );
	::SetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE, ::GetWindowLong( listctrl->m_hWnd, GWL_EXSTYLE ) | WS_EX_CLIENTEDGE );
	listctrl->InsertItem( 0, "test 1" );
	listctrl->InsertItem( 1, "test 2" );
	listctrl->InsertItem( 2, "test 3" );
	listctrl->InsertItem( 3, "test 4" );
	projectBar.insertPage( listctrl, "Basic Process" );

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
	projectBar.insertPage( listctrl2, "Second Page" );

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

	EnableDocking( CBRS_ALIGN_ANY );

	DockControlBar( &m_wndToolBar );
	DockControlBar( &projectBar, AFX_IDW_DOCKBAR_LEFT );

	return 0;
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
	pCmdUI->Enable( rpapp.project().hasChild() );
	pCmdUI->SetCheck( rpapp.project().getFlowState() == RPProject::flow_select );
}

void RPMainFrame::OnUpdateFlowConnector( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( rpapp.project().hasChild() );
	pCmdUI->SetCheck( rpapp.project().getFlowState() == RPProject::flow_connector );
}

void RPMainFrame::OnUpdateFlowRotate( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( rpapp.project().hasChild() );
	pCmdUI->SetCheck( rpapp.project().getFlowState() == RPProject::flow_rotate );
}

void RPMainFrame::OnFlowSelect()
{
	rpapp.project().setFlowState( RPProject::flow_select );
}

void RPMainFrame::OnFlowConnector()
{
	rpapp.project().setFlowState( RPProject::flow_connector );
}

void RPMainFrame::OnFlowRotate()
{
	rpapp.project().setFlowState( RPProject::flow_rotate );
}

	// MJ start 29.03.06 обработчик кнопки генерирования
void RPMainFrame::OnGenerate() 
{
	
	rpapp.RDOfiles->pattern.open("aaa\\PATTERN.pat");
	rpapp.RDOfiles->resourse.open("aaa\\RESOURSE.res");
	rpapp.RDOfiles->function.open("aaa\\FUNCTION.fun");
	
	rpapp.project().list_name();
	
	rpapp.RDOfiles->pattern.close();
	rpapp.RDOfiles->resourse.close();
	rpapp.RDOfiles->function.close();

}
	// MJ stop
