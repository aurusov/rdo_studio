#include "stdafx.h"
#include "rdostudioframestreectrl.h"
#include "rdostudiomodel.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "resource.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioFramesTreeCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioFramesTreeCtrl, RDOTreeCtrl)
	//{{AFX_MSG_MAP(RDOStudioFramesTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

vector< RDOStudioFramesTreeCtrl::Item* > RDOStudioFramesTreeCtrl::frames;

RDOStudioFramesTreeCtrl::RDOStudioFramesTreeCtrl()
{
}

RDOStudioFramesTreeCtrl::~RDOStudioFramesTreeCtrl()
{
	std::vector< Item* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		delete *it++;
	};
}

BOOL RDOStudioFramesTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int RDOStudioFramesTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void RDOStudioFramesTreeCtrl::insertItem( const string& name )
{
	Item* item = new Item;
	item->frame_doc = NULL;
	item->hitem     = InsertItem( name.c_str(), 1, 1, GetRootItem() );
	frames.push_back( item );
}

void RDOStudioFramesTreeCtrl::clear()
{
	deleteChildren( GetRootItem() );
	std::vector< Item* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		(*it)->frame_doc->OnCloseDocument();
		delete *it++;
	};
	frames.clear();
}

void RDOStudioFramesTreeCtrl::expand()
{
	Expand( GetRootItem(), TVE_EXPAND );
}

void RDOStudioFramesTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	RDOTreeCtrl::OnLButtonDblClk(nFlags, point);

	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( TVHT_ONITEM & uFlags ) && hitem != GetRootItem() ) {
		RDOStudioFrameDoc* doc = NULL;
		std::vector< Item* >::iterator it = frames.begin();
		while ( it != frames.end() ) {
			if ( (*it)->hitem == hitem ) {
				doc = (*it)->frame_doc;
				break;
			}
			it++;
		};
		if ( !doc || !model->isValidFrameDoc( doc ) ) {
			doc = model->addNewFrameDoc();
			(*it)->frame_doc = doc;
			doc->SetTitle( format( IDS_FRAMENAME, static_cast<LPCTSTR>(GetItemText( hitem )) ).c_str()  );
		} else {
			studioApp.mainFrame->MDIActivate( doc->getView()->GetParentFrame() );
		}
/*
		RDOStudioFrameDoc* doc = reinterpret_cast<RDOStudioFrameDoc*>(GetItemData( hitem ));
		if ( !doc || !model->isValidFrameDoc( doc ) ) {
			doc = model->addNewFrameDoc();
			SetItemData( hitem, reinterpret_cast<DWORD>(doc) );
			doc->SetTitle( format( IDS_FRAMENAME, static_cast<LPCTSTR>(GetItemText( hitem )) ).c_str()  );
		} else {
			studioApp.mainFrame->MDIActivate( doc->getView()->GetParentFrame() );
		}
*/
	}
}
