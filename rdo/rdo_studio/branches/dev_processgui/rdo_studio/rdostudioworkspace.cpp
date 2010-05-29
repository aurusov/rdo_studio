#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdostudioworkspace.h"
#include "rdo_studio/rdostudioapp.h"
#include "rdo_studio/rdostudiomainfrm.h"
#include "rdo_studio/rdo_tracer/rdotracer.h"
#include "rdo_studio/rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
#include "rdo_studio/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect rect;          
	GetClientRect (&rect);
	m_strText = _T("Âîò îêíî ");
	dc.DrawText(m_strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); 

}

 int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
 {
         if (CWnd ::OnCreate(lpCreateStruct) == -1)
                 return -1;

 }

// ----------------------------------------------------------------------------
// ---------- RDOStudioWorkspace
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioWorkspace, RDOStudioDockWnd)
	//{{AFX_MSG_MAP(RDOStudioWorkspace)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioWorkspace::RDOStudioWorkspace():
	RDOStudioDockWnd(),
	frames( NULL ),
	projectBar2(NULL)
{
}

RDOStudioWorkspace::~RDOStudioWorkspace()
{
}

int RDOStudioWorkspace::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioDockWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, 0 );
	tab.modifyTabStyle( 0, TCS_MULTILINE );
//	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );

	RDOTracerTreeCtrl* trace = tracer->createTree();
	trace->Create( 0, CRect(0, 0, 0, 0), &tab, 0 );

	frames = new RDOStudioFrameTreeCtrl;
	frames->Create( 0, CRect(0, 0, 0, 0), &tab, 0 );

	m_wndView = new CChildView;
	m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), &tab, AFX_IDW_PANE_FIRST, NULL);

	projectBar2 = new RPProjectBar;
	projectBar2->Create( rdo::format( ID_DOCK_PROJECT_BAR ).c_str(), &tab, 0 );
	
	
	tab.insertItem( trace, rdo::format( IDS_TAB_TRACER ).c_str() );
	tab.insertItem( frames, rdo::format( IDS_TAB_FRAMES ).c_str() ); 
	tab.insertItem( m_wndView, rdo::format( "ÎÊÍÎ" ).c_str() );
	tab.insertItem( projectBar2, rdo::format( IDS_TAB_PAGECTRL ).c_str() );

	studioApp.mainFrame->registerCmdWnd( trace );
	studioApp.mainFrame->registerCmdWnd( frames );
	studioApp.mainFrame->registerCmdWnd( m_wndView );
	studioApp.mainFrame->registerCmdWnd( projectBar2 );

	return 0;
}
