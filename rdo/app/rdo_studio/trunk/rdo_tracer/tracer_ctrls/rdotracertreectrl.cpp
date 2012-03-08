/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracertreectrl.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreectrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerpattern.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresult.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerTreeCtrl
// --------------------------------------------------------------------------------
SCODE RDODropSource::GiveFeedback( DROPEFFECT dropEffect )
{
	return COleDropSource::GiveFeedback( dropEffect );
}

IMPLEMENT_DYNCREATE( RDOTracerTreeCtrl, RDOTreeCtrl )

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP( RDOTracerTreeCtrl, RDOTreeCtrl )
	ON_WM_CREATE()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND( ID_CHART_ADDTONEWCHART, OnAddToNewChart )
	ON_UPDATE_COMMAND_UI( ID_CHART_ADDTONEWCHART, OnUpdateAddToNewChart )
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT( TVN_BEGINDRAG, OnDragDrop )
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_CHART_FINDINCHARTS, OnUpdateChartFindincharts)
	ON_COMMAND(ID_CHART_FINDINCHARTS, OnChartFindincharts)
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
END_MESSAGE_MAP()

RDOTracerTreeCtrl::RDOTracerTreeCtrl(): RDOTreeCtrl()
{
}

RDOTracerTreeCtrl::~RDOTracerTreeCtrl()
{
	source.Empty();
}

RDOTracerTreeItem* RDOTracerTreeCtrl::getIfItemIsDrawable ( const HTREEITEM hItem ) const
{
	RDOTracerTreeItem* res = NULL;
	if ( hItem ) {
		RDOTracerTreeItem* item = (RDOTracerTreeItem*)GetItemData( hItem );
		res = item && item->isDrawable() ? item : NULL;
	}
	return res;
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

	rootItem.setTreeItem( InsertItem( rdo::format( ID_MODEL_START ).c_str(), 0, 0 ) );
	rtpItem.setTreeItem( InsertItem( rdo::format( ID_RESOURCE_TYPES ).c_str(), 1, 1, rootItem.getTreeItem() ) );
	patItem.setTreeItem( InsertItem( rdo::format( ID_PATTERNS ).c_str(), 1, 1, rootItem.getTreeItem() ) );
	pmvItem.setTreeItem( InsertItem( rdo::format( ID_RESULTS ).c_str(), 1, 1, rootItem.getTreeItem() ) );
	Expand( rootItem.getTreeItem(), TVE_EXPAND );

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	
	BOOL maximized;
	studioApp.mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 0, &popupMenu );
	appendMenu( mainMenu->GetSubMenu( 6 + delta ), 1, &popupMenu );

	return 0;
}

void RDOTracerTreeCtrl::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
{
	RDOTreeCtrl::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOTracerTreeCtrl::setHasChildren( const RDOTracerTreeItem* item, const rbool hasChildren )
{
	TVITEM tvitem;
	tvitem.hItem = item->getTreeItem();
	tvitem.mask = TVIF_CHILDREN;
	tvitem.cChildren = hasChildren ? 1 : 0;
	SetItem( &tvitem );
}

void RDOTracerTreeCtrl::doDragDrop( RDOTracerTreeItem* item, CPoint point )
{
	UNUSED(point);

	UINT format = tracer->getClipboardFormat();
	if ( format ) {
		RDOTracerSerie** ptr = (RDOTracerSerie**)::GlobalAlloc( LMEM_FIXED, sizeof( RDOTracerSerie* ) );
		*ptr = (RDOTracerSerie*)item;
		source.CacheGlobalData( CLIPFORMAT(format), ptr );
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
	RDOTracerTreeItem* item = getIfItemIsDrawable( hitem );
	if ( item  )
		doDragDrop( item, lpnmtv->ptDrag );
	*result = 0;
}

BOOL RDOTracerTreeCtrl::setModelName( CREF(tstring) modelName )
{
	return SetItemText( rootItem.getTreeItem(), rdo::format( ID_MODEL, modelName.c_str() ).c_str() );
}

void RDOTracerTreeCtrl::addResourceType( RDOTracerResType* resType )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	resType->setTreeItem( InsertItem( mask, resType->Name.c_str(), 2, 2, 0, 0, (LPARAM)(RDOTracerTreeItem*)resType, rtpItem.getTreeItem(), TVI_LAST ) );
	setHasChildren( &rtpItem );
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
	setHasChildren( type );
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
	pat->setTreeItem( InsertItem( mask, pat->Name.c_str(), 2, 2, 0, 0, (LPARAM)(RDOTracerTreeItem*)pat, patItem.getTreeItem(), TVI_LAST ) );
	setHasChildren( &patItem );
}

void RDOTracerTreeCtrl::addOperation( RDOTracerOperationBase* opr )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	opr->setTreeItem( InsertItem( mask, opr->getName().c_str(), 5, 5, 0, 0, (LPARAM)(RDOTracerTreeItem*)opr, opr->getPattern()->getTreeItem(), TVI_LAST ) );
	setHasChildren( opr->getPattern() );
}

/*void RDOTracerTreeCtrl::addIrregularEvent( RDOTracerOperation* opr )
{
	addOperation( opr );
}*/

void RDOTracerTreeCtrl::addResult( RDOTracerResult* res )
{
	UINT mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	res->setTreeItem( InsertItem( mask, res->getName().c_str(), 5, 5, 0, 0, (LPARAM)(RDOTracerTreeItem*)res, pmvItem.getTreeItem(), TVI_LAST ) );
	setHasChildren( &pmvItem );
}

void RDOTracerTreeCtrl::deleteChildren( const RDOTracerTreeItem* parent )
{
	RDOTreeCtrl::deleteChildren( parent->getTreeItem() );
	setHasChildren( parent, false );
}

void RDOTracerTreeCtrl::clear()
{
	deleteChildren( &rtpItem );
	deleteChildren( &patItem );
	deleteChildren( &pmvItem );
	SetItemText( rootItem.getTreeItem(), rdo::format( ID_MODEL_START ).c_str() );
}

void RDOTracerTreeCtrl::addToNewChart( const HTREEITEM hitem ) const
{
	UNUSED(hitem);

	RDOTracerTreeItem* item = getIfItemIsDrawable( GetSelectedItem() );
	if ( item ) {
		tracer->addSerieToChart( static_cast<RDOTracerSerie*>( item ) );
	}
}

rbool RDOTracerTreeCtrl::findInCharts( const HTREEITEM hitem ) const
{
	UNUSED(hitem);

	rbool res = false;
	RDOTracerTreeItem* item = getIfItemIsDrawable( GetSelectedItem() );
	RDOTracerSerie* serie = NULL;
	if ( item ) {
		serie = static_cast<RDOTracerSerie*>( item );
		res = serie->activateFirstDoc();
	}
	return res;
}

void RDOTracerTreeCtrl::OnAddToNewChart()
{
	addToNewChart( GetSelectedItem() );
}

void RDOTracerTreeCtrl::OnUpdateAddToNewChart( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( tracer->getDrawTrace() && getIfItemIsDrawable( GetSelectedItem() ) != NULL );
}

void RDOTracerTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	RDOTreeCtrl::OnLButtonDblClk(nFlags, point);

	if ( tracer->getDrawTrace() ) {
		UINT uFlags;
		HTREEITEM hitem = HitTest( point, &uFlags );

		if ( hitem && ( TVHT_ONITEM & uFlags ) ) {
			if ( !findInCharts( hitem ) )
				addToNewChart( hitem );
		}
	}
}

void RDOTracerTreeCtrl::OnRButtonDown(UINT _nFlags, CPoint point) 
{
	UNUSED(_nFlags);

	UINT uFlags;
	HTREEITEM hitem = HitTest( point, &uFlags );
	if ( hitem && ( TVHT_ONITEM & uFlags ) ) {
		SelectItem( hitem );
	}
	if ( GetFocus() != this )
		SetFocus();
	CPoint pos = point;
	ClientToScreen( &pos );
	if ( popupMenu.m_hMenu ) popupMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
}

void RDOTracerTreeCtrl::OnUpdateChartFindincharts(CCmdUI* pCmdUI) 
{
	rbool enable = false;
	if ( tracer->getDrawTrace() ) {
		RDOTracerTreeItem* item = getIfItemIsDrawable( GetSelectedItem() );
		RDOTracerSerie* serie = NULL;
		if ( item ) {
			serie = static_cast<RDOTracerSerie*>( item );
			enable = serie->isInOneOrMoreDocs();
		}
	}
	pCmdUI->Enable( enable );
}

void RDOTracerTreeCtrl::OnChartFindincharts() 
{
	findInCharts( GetSelectedItem() );
}

void RDOTracerTreeCtrl::OnHelpKeyword()
{
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_model_chart.htm";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}
