#include "stdafx.h"
#include "rdostudioframetreectrl.h"
#include "rdostudiomodel.h"
#include "rdostudioframemanager.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameTreeCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioFrameTreeCtrl, RDOTreeCtrl)
	//{{AFX_MSG_MAP(RDOStudioFrameTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioFrameTreeCtrl::RDOStudioFrameTreeCtrl()
{
}

RDOStudioFrameTreeCtrl::~RDOStudioFrameTreeCtrl()
{
}

BOOL RDOStudioFrameTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int RDOStudioFrameTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOTreeCtrl::OnCreate(lpCreateStruct) == -1 ) return -1;

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 5, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_FRAMESTREECTRL );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );
	InsertItem( format( IDS_FRAMES ).c_str(), 0, 0 );

	return 0;
}

void RDOStudioFrameTreeCtrl::expand()
{
	Expand( GetRootItem(), TVE_EXPAND );
}

void RDOStudioFrameTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	RDOTreeCtrl::OnLButtonDblClk(nFlags, point);

	if ( model->getShowMode() != RDOSimulatorNS::SM_NoShow ) {

		UINT uFlags;
		HTREEITEM hitem = HitTest( point, &uFlags );

		if ( hitem && ( TVHT_ONITEM & uFlags ) && hitem != GetRootItem() ) {
			int frame_index = model->frameManager.findFrameIndex( hitem );
			if ( frame_index != -1 ) {
				RDOStudioFrameDoc* doc = model->frameManager.getFrameDoc( frame_index );
				if ( !doc ) {
					doc = model->frameManager.connectFrameDoc( hitem );
					if ( doc ) {
						doc->SetTitle( format( IDS_FRAMENAME, model->frameManager.getFrameName( frame_index ).c_str() ).c_str()  );
					}
				} else {
					studioApp.mainFrame->MDIActivate( doc->getView()->GetParentFrame() );
				}
			}
		}
	}
}
