#include "rdo_ctrls_pch.h"
#include "rdotreectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTreeCtrl
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC( RDOTreeCtrl, CTreeCtrl )

BEGIN_MESSAGE_MAP( RDOTreeCtrl, CTreeCtrl )
	//{{AFX_MSG_MAP(RDOTreeCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOTreeCtrl::RDOTreeCtrl(): CTreeCtrl()
{
}

RDOTreeCtrl::~RDOTreeCtrl()
{
}

BOOL RDOTreeCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style |= WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	return CTreeCtrl::PreCreateWindow(cs);
}

void RDOTreeCtrl::deleteChildren( const HTREEITEM parent )
{
	HTREEITEM nextItem;
	HTREEITEM childItem = GetChildItem( parent );

	while ( childItem )	{
		nextItem = GetNextItem( childItem, TVGN_NEXT );
		DeleteItem( childItem );
		childItem = nextItem;
	}

}
