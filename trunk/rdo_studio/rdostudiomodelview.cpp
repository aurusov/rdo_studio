#include "stdafx.h"
#include "rdostudiomodelview.h"
#include "rdostudiomodeldoc.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "edit_ctrls/rdofindedit.h"
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
	ON_COMMAND(ID_SEARCH_FIND_INMODEL, OnSearchFindInModel)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( FINDINMODEL_MSG, OnFindInModelMsg )
	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR          , OnUpdateCoordStatusBar )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR         , OnUpdateModifyStatusBar )
	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITE_STATUSBAR, OnUpdateInsertOverwriteStatusBar )
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

void RDOStudioModelView::OnDraw(CDC* /*pDC*/)
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

	tab->Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, 0 );

	return 0;
}

BOOL RDOStudioModelView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( tab->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return RDOStudioEditBaseView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void RDOStudioModelView::OnSetFocus(CWnd* pOldWnd)
{
	RDOStudioEditBaseView::OnSetFocus( pOldWnd );
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

LRESULT RDOStudioModelView::OnFindInModelMsg( WPARAM /*wParam*/, LPARAM lParam )
{
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	if ( !pDialog->IsTerminating() ) {
		studioApp.mainFrame->output.clearFind();
		studioApp.mainFrame->output.showFind();
		string findStr       = pDialog->GetFindString();
		bool bMatchCase      = pDialog->MatchCase() ? true : false;
		bool bMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
		studioApp.mainFrame->output.getFind()->setKeyword( findStr, bMatchCase );
		studioApp.mainFrame->output.appendStringToFind( rdo::format( ID_FINDINMODEL_BEGINMSG, findStr.c_str() ) );
		int count = 0;
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			RDOEditorEdit* edit = tab->getItemEdit( i );
			int pos  = 0;
			int line = 0;
			while ( pos != -1 ) {
				pos = edit->findPos( findStr, line, bMatchCase, bMatchWholeWord );
				if ( pos != -1 ) {
					line = edit->getLineFromPosition( pos );
					studioApp.mainFrame->output.appendStringToFind( edit->getLine( line ), tab->indexToType( i ), line, pos - edit->getPositionFromLine( line ) );
					line++;
					count++;
				}
			}
		}
		pDialog->SendMessage( WM_CLOSE );
		string s;
		if ( count ) {
			s = rdo::format( ID_FINDINMODEL_ENDMSG_COUNT, count );
		} else {
			s = rdo::format( ID_FINDINMODEL_ENDMSG_NOTFOUND, findStr.c_str() );
		}
		studioApp.mainFrame->output.appendStringToFind( s );
	}
	return 0;
}

void RDOStudioModelView::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string str = "";
	RDOEditorEdit* edit = getEdit();
	if ( edit ) {
		int x = edit->getCurrentColumnNumber() + 1;
		int y = edit->getCurrentLineNumber() + 1;
		str = rdo::format( "%d: %d", x, y );
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOStudioModelView::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string str = "";
	RDOEditorEdit* edit = getEdit();
	if ( edit ) {
		if ( edit->isReadOnly() ) {
			str = rdo::format( ID_STATUSBAR_READONLY );
		} else if ( edit->isModify() ) {
			str = rdo::format( ID_STATUSBAR_MODIFIED );
		}
	}
	pCmdUI->SetText( str.c_str() );
}

void RDOStudioModelView::OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	string str = "";
	RDOEditorEdit* edit = getEdit();
	if ( edit && edit->isOverwrite() ) {
		str = rdo::format( ID_STATUSBAR_OVERWRITE );
	}
	pCmdUI->SetText( str.c_str() );
}
