#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_docview.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_flowchart.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPDoc
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RPDoc, CDocument)

BEGIN_MESSAGE_MAP(RPDoc, CDocument)
	ON_COMMAND_RANGE( 20000, 30000, OnMethodCommandRange )
	ON_UPDATE_COMMAND_UI_RANGE( 20000, 30000, OnMethodUpdateRange )
END_MESSAGE_MAP()

RPDoc::RPDoc()
{
}

RPDoc::~RPDoc()
{
}

BOOL RPDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RPDoc::Serialize( CArchive& ar )
{
	if ( ar.IsStoring() ) {
		/// @todo add storing code here
	} else {
		/// @todo add loading code here
	}
}

RPView* RPDoc::getView() const
{
	POSITION pos = GetFirstViewPosition();
	return static_cast<RPView*>(GetNextView( pos ));
}

#ifdef _DEBUG
void RPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void RPDoc::OnMethodCommandRange( UINT id )
{
	rpMethod::RPMethod* method = rpMethod::project->getMethodByButton( id );
	if ( method ) {
		method->buttonCommand( id );
	}
}

void RPDoc::OnMethodUpdateRange( CCmdUI* pCmdUI )
{
	rpMethod::RPMethod* method = rpMethod::project->getMethodByButton( pCmdUI->m_nID );
	if ( method ) {
		RPCtrlToolbar::ButtonUpdate button_update( pCmdUI->m_nID );
		method->buttonUpdate( button_update );
		pCmdUI->Enable( button_update.enable );
	}
}

// --------------------------------------------------------------------------------
// -------------------- RPView
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RPView, CView)

BEGIN_MESSAGE_MAP(RPView, CView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

RPView::RPView():
	CView(),
	flowchart( NULL )
{
}

RPView::~RPView()
{
}

BOOL RPView::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

void RPView::OnDestroy()
{
	if ( flowchart ) {
		flowchart->SendMessage( WM_CLOSE );
		delete flowchart;
	}
}

void RPView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus( pOldWnd );
	if ( flowchart ) flowchart->SetFocus();
}

BOOL RPView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if ( flowchart && flowchart->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ) return TRUE;
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void RPView::OnSize( UINT nType, int cx, int cy ) 
{
	CView::OnSize( nType, cx, cy );
	if ( flowchart ) {
		CRect rect;
		GetClientRect( rect );
		flowchart->MoveWindow( rect );
	}
}

void RPView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	target.Register( this );
}

void RPView::makeFlowChartWnd( RPObjectFlowChart* flowobj )
{
	flowchart = new RPFlowChart( flowobj, GetDocument() );
	CRect rect;
	GetClientRect( &rect );
	flowchart->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, this, AFX_IDW_PANE_FIRST, NULL );
	flowchart->init();
}

void RPView::OnDraw(CDC* /*pDC*/)
{
	RPDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	if ( !pDoc ) return;
}

const RPObjectClassInfo* RPView::getSrcClassInfo( COleDataObject* pDataObject ) const
{
	if ( !pDataObject->IsDataAvailable( CF_TEXT ) ) {
		return NULL;
	}
	HGLOBAL hglobal = pDataObject->GetGlobalData( CF_TEXT );
	if ( hglobal ) {
		CMemFile sf( static_cast<BYTE*>(::GlobalLock(hglobal)), ::GlobalSize(hglobal) );
		CString buffer;
		LPSTR str = buffer.GetBufferSetLength( ::GlobalSize(hglobal) );
		sf.Read( str, ::GlobalSize(hglobal) );
		::GlobalUnlock( hglobal );
		rp::string src_class_name( buffer );
		const RPObjectClassInfo* src_class_info = rpMethod::factory->getClassInfo( src_class_name );
		const RPObjectClassInfo* trg_class_info = flowchart->getObjectFlowChart().getClassInfo();
		if ( src_class_info && trg_class_info && src_class_info->getMethod() && src_class_info->getMethod() == trg_class_info->getMethod() ) {
			return src_class_info;
		} else {
			return NULL;
		}
	}
	return NULL;
}

DROPEFFECT RPView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	UNUSED(dwKeyState);

	const RPObjectClassInfo* class_info = getSrcClassInfo( pDataObject );
	if ( class_info ) {
		flowchart->getObjectFlowChart().onDragEnter( class_info, rp::point( point.x - 16, point.y - 16 ) );
		return DROPEFFECT_MOVE;
	} else {
		return DROPEFFECT_NONE;
	}
}

DROPEFFECT RPView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	UNUSED(dwKeyState);

	if ( getSrcClassInfo( pDataObject ) ) {
		flowchart->getObjectFlowChart().onDragOver( rp::point( point.x - 16, point.y - 16 ) );
		return DROPEFFECT_MOVE;
	} else {
		return DROPEFFECT_NONE;
	}
}

void RPView::OnDragLeave()
{
	flowchart->getObjectFlowChart().onDragLeave();
}

BOOL RPView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	UNUSED(dropEffect);

	if ( getSrcClassInfo( pDataObject ) ) {
		flowchart->getObjectFlowChart().onDrop( rp::point( point.x - 16, point.y - 16 ) );
		return true;
	} else {
		return false;
	}
}

BOOL RPView::OnPreparePrinting( CPrintInfo* pInfo )
{
	return DoPreparePrinting(pInfo);
}

void RPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void RPView::AssertValid() const
{
	CView::AssertValid();
}

void RPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

RPDoc* RPView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RPDoc)));
	return reinterpret_cast<RPDoc*>(m_pDocument);
}
#endif //_DEBUG
