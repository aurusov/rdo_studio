#include "stdafx.h"
#include "rdostudioframeview.h"
#include "rdostudioframedoc.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioFrameView, CView)

BEGIN_MESSAGE_MAP(RDOStudioFrameView, CView)
	//{{AFX_MSG_MAP(RDOStudioFrameView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

RDOStudioFrameView::RDOStudioFrameView():
	CView(),
	frameBmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	mustBeInit( true )
{
}

RDOStudioFrameView::~RDOStudioFrameView()
{
}

BOOL RDOStudioFrameView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CView::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

int RDOStudioFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void RDOStudioFrameView::OnDraw(CDC* pDC)
{
//	RDOStudioFrameDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

	int index = model->frameManager.findFrameIndex( this );
	CSingleLock lock_draw( model->frameManager.getFrameDraw( index ) );
	lock_draw.Lock();

	if ( true /*!model->frameManager.isDeleted( index )*/ ) {

		GetClientRect( &newClientRect );

		CDC dc;
		dc.CreateCompatibleDC( pDC );
		CBitmap* pOldBitmap = dc.SelectObject( &frameBmp );

		pDC->BitBlt( 0, 0, frameBmpRect.right, frameBmpRect.bottom, &dc, 0, 0, SRCCOPY );
		COLORREF color = studioApp.mainFrame->style_frame.theme->backgroundColor;
		pDC->FillSolidRect( frameBmpRect.right, 0, newClientRect.right - frameBmpRect.right, newClientRect.bottom, color );
		pDC->FillSolidRect( 0, frameBmpRect.bottom, newClientRect.right, newClientRect.bottom - frameBmpRect.bottom, color );

		dc.SelectObject( pOldBitmap );

	}

	lock_draw.Unlock();
}

BOOL RDOStudioFrameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void RDOStudioFrameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RDOStudioFrameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void RDOStudioFrameView::AssertValid() const
{
	CView::AssertValid();
}

void RDOStudioFrameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

RDOStudioFrameDoc* RDOStudioFrameView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioFrameDoc)));
	return (RDOStudioFrameDoc*)m_pDocument;
}
#endif
