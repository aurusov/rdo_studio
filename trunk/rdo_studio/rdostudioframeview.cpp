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
	clientBmpRect( 0, 0, 0, 0 ),
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
	RDOStudioFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSingleLock lock( &pDoc->frameUsed );
	lock.Lock();

	GetClientRect( &newClientRect );
//	CRect rect;
//	rect.CopyRect( &newClientRect );

	if ( newClientRect.Width() > clientBmpRect.Width() || newClientRect.Height() > clientBmpRect.Height() ) {
		if ( clientBmp.GetSafeHandle() ) clientBmp.DeleteObject();
		clientBmp.CreateCompatibleBitmap( pDC, newClientRect.Width(), newClientRect.Height() );
		clientBmpRect = newClientRect;
	}

	CDC dc;
	dc.CreateCompatibleDC( pDC );
	CBitmap* pOldBitmap = dc.SelectObject( &clientBmp );

	int oldBkMode = dc.SetBkMode( TRANSPARENT );

	dc.FillSolidRect( newClientRect, studioApp.mainFrame->style_frame.theme->backgroundColor );

	CDC dc2;
	dc2.CreateCompatibleDC( pDC );
	CBitmap* pOldBitmap2 = dc2.SelectObject( &frameBmp );
	dc.BitBlt( 0, 0, newClientRect.Width(), newClientRect.Height(), &dc2, 0, 0, SRCCOPY );
	dc2.SelectObject( pOldBitmap2 );

	pDC->BitBlt( 0, 0, newClientRect.Width(), newClientRect.Height(), &dc, 0, 0, SRCCOPY );
	dc.SetBkMode( oldBkMode );
	dc.SelectObject( pOldBitmap );

	lock.Unlock();
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
