#include "stdafx.h"
#include "rdostudiomodelview.h"
#include "rdostudiomodeldoc.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdo_edit/rdoeditortabctrl.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOStudioModelView
// ----------------------------------------------------------------------------
static const UINT FINDINMODEL_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

IMPLEMENT_DYNCREATE(RDOStudioModelView, RDOStudioEditBaseView)

BEGIN_MESSAGE_MAP(RDOStudioModelView, RDOStudioEditBaseView)
	//{{AFX_MSG_MAP(RDOStudioModelView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_SEARCH_FINDINMODEL, OnSearchFindInModel)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( FINDINMODEL_MSG, OnFindInModelMsg )
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
	if ( tab ) { delete tab; tab = NULL; }
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

void RDOStudioModelView::OnSearchFindInModel() 
{
	CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	pDlg->Create( true, getEdit()->getCurrentOrSelectedWord().c_str(), NULL, FR_HIDEUPDOWN, this );
}

LRESULT RDOStudioModelView::OnFindInModelMsg( WPARAM wParam, LPARAM lParam )
{
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	if ( !pDialog->IsTerminating() ) {
		studioApp.mainFrame->output.clearFind();
		studioApp.mainFrame->output.showFind();
		string findStr = pDialog->GetFindString();
		CString s;
		s.Format( ID_FINDINMODEL_BEGINMSG, findStr.c_str() );
		studioApp.mainFrame->output.appendStringToFind( (LPCTSTR)s );
		bool bMatchCase      = pDialog->MatchCase() ? true : false;
		bool bMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
		int count = 0;
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			RDOEditorEdit* edit = tab->getItemEdit( i );
			int line = 0;
			while ( line != -1 ) {
				line = edit->findLine( findStr, line, bMatchCase, bMatchWholeWord );
				if ( line != -1 ) {
					studioApp.mainFrame->output.appendStringToFind( edit->getLine( line ) );
					line++;
					count++;
				}
			}
		}
		pDialog->SendMessage( WM_CLOSE );
		if ( count ) {
			s.Format( ID_FINDINMODEL_ENDMSG_COUNT, count );
		} else {
			s.Format( ID_FINDINMODEL_ENDMSG_NOTFOUND );
		}
		studioApp.mainFrame->output.appendStringToFind( (LPCTSTR)s );
	}
	return 0;
}
