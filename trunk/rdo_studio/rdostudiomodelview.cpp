#include "stdafx.h"
#include "rdostudiomodelview.h"
#include "rdostudiomodeldoc.h"
#include "rdo_edit/rdoeditortabctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOStudioModelView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioModelView, RDOStudioEditBaseView)

BEGIN_MESSAGE_MAP(RDOStudioModelView, RDOStudioEditBaseView)
	//{{AFX_MSG_MAP(RDOStudioModelView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, RDOStudioEditBaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, RDOStudioEditBaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, RDOStudioEditBaseView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioModelView::RDOStudioModelView(): RDOStudioEditBaseView()
{
	tab = new RDOEditorTabCtrl( this );
}

RDOStudioModelView::~RDOStudioModelView()
{
	delete tab;
}

BOOL RDOStudioModelView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !RDOStudioEditBaseView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

void RDOStudioModelView::OnDraw(CDC* pDC)
{
	RDOStudioModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL RDOStudioModelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void RDOStudioModelView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RDOStudioModelView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void RDOStudioModelView::AssertValid() const
{
	RDOStudioEditBaseView::AssertValid();
}

void RDOStudioModelView::Dump(CDumpContext& dc) const
{
	RDOStudioEditBaseView::Dump(dc);
}

RDOStudioModelDoc* RDOStudioModelView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioModelDoc)));
	return (RDOStudioModelDoc*)m_pDocument;
}
#endif

int RDOStudioModelView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioEditBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	tab->Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, -1 );

	return 0;
}

BOOL RDOStudioModelView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( tab->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioEditBaseView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void RDOStudioModelView::OnSetFocus(CWnd* pOldWnd) 
{
	RDOStudioEditBaseView::OnSetFocus(pOldWnd);
	tab->SetFocus();
}

void RDOStudioModelView::OnSize(UINT nType, int cx, int cy) 
{
	RDOStudioEditBaseView::OnSize(nType, cx, cy);
	
	CRect r;
	GetClientRect( r );
	tab->MoveWindow( r );
}

RDOEditorEdit* RDOStudioModelView::getEdit() const
{
	return tab->getCurrentEdit();
}
