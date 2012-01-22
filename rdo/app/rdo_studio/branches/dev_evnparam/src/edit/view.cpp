#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/src/edit/view.h"
#include "app/rdo_studio_mfc/src/edit/document.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditView
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioEditView, RDOStudioEditBaseView)

BEGIN_MESSAGE_MAP(RDOStudioEditView, RDOStudioEditBaseView)
	//{{AFX_MSG_MAP(RDOStudioEditView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, RDOStudioEditBaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, RDOStudioEditBaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, RDOStudioEditBaseView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioEditView::RDOStudioEditView(): RDOStudioEditBaseView()
{
	edit = new RDOEditorEdit( this );
}

RDOStudioEditView::~RDOStudioEditView()
{
	delete edit;
}

BOOL RDOStudioEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !RDOStudioEditBaseView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

void RDOStudioEditView::OnDraw(CDC* /*pDC*/)
{
	RDOStudioEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL RDOStudioEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void RDOStudioEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RDOStudioEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void RDOStudioEditView::AssertValid() const
{
	RDOStudioEditBaseView::AssertValid();
}

void RDOStudioEditView::Dump(CDumpContext& dc) const
{
	RDOStudioEditBaseView::Dump(dc);
}

RDOStudioEditDoc* RDOStudioEditView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioEditDoc)));
	return (RDOStudioEditDoc*)m_pDocument;
}
#endif

int RDOStudioEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RDOStudioEditBaseView::OnCreate(lpCreateStruct) == -1) return -1;

	edit->setEditorStyle( &studioApp.mainFrame->style_editor );
	edit->Create( NULL, NULL, WS_VISIBLE, CRect(0, 0, 100, 100), this, 0 );
	edit->setPopupMenu( &popupMenu );

	return 0;
}

BOOL RDOStudioEditView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( edit->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioEditBaseView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void RDOStudioEditView::OnSetFocus(CWnd* pOldWnd)
{
	RDOStudioEditBaseView::OnSetFocus( pOldWnd );
	edit->SetFocus();
}

void RDOStudioEditView::OnSize(UINT nType, int cx, int cy) 
{
	RDOStudioEditBaseView::OnSize(nType, cx, cy);
	
	CRect r;
	GetClientRect( r );
	edit->MoveWindow( r );
}

RDOEditorEdit* RDOStudioEditView::getEdit() const
{
	return edit;
}
