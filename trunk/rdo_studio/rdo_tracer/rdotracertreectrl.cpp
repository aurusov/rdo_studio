#include "stdafx.h"
#include "rdotracertreectrl.h"
#include "rdotracertrace.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTracerTreeCtrl
// ----------------------------------------------------------------------------
SCODE RDODropSource::GiveFeedback( DROPEFFECT dropEffect )
{
	return COleDropSource::GiveFeedback( dropEffect );
}

IMPLEMENT_DYNCREATE( RDOTracerTreeCtrl, RDOTreeCtrl )

BEGIN_MESSAGE_MAP( RDOTracerTreeCtrl, RDOTreeCtrl )
	//{{AFX_MSG_MAP(RDOTracerTreeCtrl)
	ON_WM_CREATE()
	ON_COMMAND( ID_CHART_ADDTONEWCHART, OnAddToNewChart )
	ON_UPDATE_COMMAND_UI( ID_CHART_ADDTONEWCHART, OnUpdateAddToNewChart )
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT( TVN_BEGINDRAG, OnDragDrop )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOTracerTreeCtrl::RDOTracerTreeCtrl(): RDOTreeCtrl()
{
}

RDOTracerTreeCtrl::~RDOTracerTreeCtrl()
{
	source.Empty();
}

BOOL RDOTracerTreeCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASBUTTONS /*| TVS_LINESATROOT*/ | TVS_HASLINES/* | TVS_DISABLEDRAGDROP*/;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return RDOTreeCtrl::PreCreateWindow(cs);
}

int RDOTracerTreeCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOTreeCtrl::OnCreate( lpCreateStruct ) == -1 ) return -1;

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 5, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_TREECTRL );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );

	CString str;
	str.LoadString( ID_MODEL_START );
	rootItem = InsertItem( str, 0, 0 );
	str.LoadString( ID_RESOURCE_TYPES );
	rtpItem = InsertItem( str, 1, 1, rootItem );
	str.LoadString( ID_PATTERNS );
	patItem = InsertItem( str, 1, 1, rootItem );
	str.LoadString( ID_RESULTS );
	pmvItem = InsertItem( str, 1, 1, rootItem );
	Expand( rootItem, TVE_EXPAND );

	return 0;
}

void RDOTracerTreeCtrl::setHasChildren( const HTREEITEM item )
{
	TVITEM tvitem;
	tvitem.hItem = item;
	tvitem.mask = TVIF_CHILDREN;
	tvitem.cChildren = 1;
	SetItem( &tvitem );
}

void RDOTracerTreeCtrl::doDragDrop( RDOTracerTreeItem* item, CPoint point )
{
	UINT format = trace.getClipboardFormat();
	if ( format ) {
		RDOTracerSerie** ptr = (RDOTracerSerie**)::GlobalAlloc( LMEM_FIXED, sizeof( RDOTracerSerie* ) );
		*ptr = (RDOTracerSerie*)item;
		source.CacheGlobalData( format, ptr );
		source.DoDragDrop( DROPEFFECT_COPY, NULL, &dropsource );
		source.Empty();
		// Dont call ::GlobalFree( ptr ), because
		// COleDataSource::Empty() calls ::ReleaseStgMedium() for
		// each allocated storage medium. By Microsoft's default
		// STGMEDIUM::punkForRelease == NULL,
		// so ::ReleaseStgMedium() calls ::GlobalFree()
		// for each allocated STGMEDIUM::TYMED_HGLOBAL.
		// ::GlobalFlags( ptr ) returns GMEM_INVALID_HANDLE
		// if HGLOBAL is not a valid handle.
	}
}

void RDOTracerTreeCtrl::OnDragDrop ( NMHDR * pNotifyStruct, LRESULT* result )
{
	LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)pNotifyStruct;
	HTREEITEM hitem = lpnmtv->itemNew.hItem;
	if ( hitem ) {
		RDOTracerTreeItem* item = (RDOTracerTreeItem*)GetItemData( hitem );
		//RDOTracerSerie* serie = (RDOTracerSerie*)tree.GetItemData( hitem );
		if ( hitem != rootItem && item && item->isDrawable() )
			doDragDrop( item, lpnmtv->ptDrag );
	}
	*result = 0;
}

BOOL RDOTracerTreeCtrl::setModelName( const string& modelName )
{
	return SetItemText( rootItem, format( ID_MODEL, modelName.c_str() ).c_str() );
}

void RDOTracerTreeCtrl::addResourceType( RDOTracerResType* resType )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	resType->setTreeItem( InsertItem( mask, resType->Name.c_str(), 2, 2, 0, 0, (LPARAM)(RDOTracerTreeItem*)resType, rtpItem, TVI_LAST ) );
	setHasChildren( rtpItem );
}

void RDOTracerTreeCtrl::addResource( RDOTracerResource* res )
{
	UINT mask =  TVIF_PARAM | TVIF_TEXT;
	RDOTracerResType* type = res->getType();
	res->setTreeItem( InsertItem( mask, res->Name.c_str(), 0, 0, 0, 0, (LPARAM)(RDOTracerTreeItem*)res, type->getTreeItem(), TVI_LAST ) );
	int count = type->getParamsCount();
	mask |= ( TVIF_IMAGE | TVIF_SELECTEDIMAGE );
	for ( int i = 0; i < count; i++ )
		res->getParam( i )->setTreeItem( InsertItem( mask, type->getParamInfo( i )->Name.c_str(), 5, 5, 0, 0, (LPARAM)(RDOTracerTreeItem*)res->getParam( i ), res->getTreeItem(), TVI_LAST ) );
	updateResource( res );
	setHasChildren( type->getTreeItem() );
}

void RDOTracerTreeCtrl::updateResource( RDOTracerResource* const res )
{
	int index = 3;
	if ( res->isErased() )
		index = 6;
	SetItemImage( res->getTreeItem(), index, index );
}

void RDOTracerTreeCtrl::addPattern( RDOTracerPattern* pat )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	pat->setTreeItem( InsertItem( mask, pat->Name.c_str(), 2, 2, 0, 0, (LPARAM)(RDOTracerTreeItem*)pat, patItem, TVI_LAST ) );
	setHasChildren( patItem );
}

void RDOTracerTreeCtrl::addOperation( RDOTracerOperation* opr )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	opr->setTreeItem( InsertItem( mask, opr->getName().c_str(), 5, 5, 0, 0, (LPARAM)(RDOTracerTreeItem*)opr, opr->getPattern()->getTreeItem(), TVI_LAST ) );
	setHasChildren( opr->getPattern()->getTreeItem() );
}

void RDOTracerTreeCtrl::addIrregularEvent( RDOTracerOperation* opr )
{
	addOperation( opr );
}

void RDOTracerTreeCtrl::addResult( RDOTracerResult* res )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	res->setTreeItem( InsertItem( mask, res->getName().c_str(), 5, 5, 0, 0, (LPARAM)(RDOTracerTreeItem*)res, pmvItem, TVI_LAST ) );
	setHasChildren( pmvItem );
}

void RDOTracerTreeCtrl::clear()
{
	deleteChildren( rtpItem );
	deleteChildren( patItem );
	deleteChildren( pmvItem );
	CString str;
	str.LoadString( ID_MODEL_START );
	SetItemText( rootItem, str );
}

void RDOTracerTreeCtrl::addToNewChart( RDOTracerTreeItem* const item )
{
	if ( item && item->isDrawable() )
		trace.addSerieToChart( (RDOTracerSerie*)item );
}

void RDOTracerTreeCtrl::OnAddToNewChart()
{
	addToNewChart( (RDOTracerTreeItem*)GetItemData( GetSelectedItem() ) );
}

void RDOTracerTreeCtrl::OnUpdateAddToNewChart( CCmdUI* pCmdUI )
{
	bool enable = false;
	HTREEITEM hitem = GetSelectedItem();
	if ( hitem ) {
		RDOTracerTreeItem* item = (RDOTracerTreeItem*)GetItemData( hitem );
		enable = item && item->isDrawable();
	}
	pCmdUI->Enable( enable );
}

void RDOTracerTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	RDOTreeCtrl::OnLButtonDblClk(nFlags, point);
	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );

	if ( hitem && ( TVHT_ONITEM & uFlags ) ) {
		RDOTracerTreeItem* item = (RDOTracerTreeItem*)GetItemData( hitem );
		if ( hitem != rootItem )
			addToNewChart( item );
	}
}
