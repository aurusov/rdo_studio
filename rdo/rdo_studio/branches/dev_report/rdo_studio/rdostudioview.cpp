#include "stdafx.h"
#include "rdostudioview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOStudioView, CView)

BEGIN_MESSAGE_MAP(RDOStudioView, CView)
	//{{AFX_MSG_MAP(RDOStudioView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioView::RDOStudioView(): CView()
{
}

RDOStudioView::~RDOStudioView()
{
}

#ifdef _DEBUG
void RDOStudioView::AssertValid() const
{
	CView::AssertValid();
}

void RDOStudioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
