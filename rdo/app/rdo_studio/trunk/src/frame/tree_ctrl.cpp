/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/tree_ctrl.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/frame/manager.h"
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTreeCtrl
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioFrameTreeCtrl, RDOTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
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
	bmp.LoadBitmap( IDB_FRAMES_TREECTRL );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );
	InsertItem( rdo::format( IDS_FRAMES ).c_str(), 0, 0 );

	return 0;
}

void RDOStudioFrameTreeCtrl::expand()
{
	Expand( GetRootItem(), TVE_EXPAND );
}

void RDOStudioFrameTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	RDOTreeCtrl::OnLButtonDblClk(nFlags, point);

	if ( model->getRuntimeMode() != rdoRuntime::RTM_MaxSpeed ) {

		UINT uFlags;
		HTREEITEM hitem = HitTest( point, &uFlags );

		if ( hitem && ( TVHT_ONITEM & uFlags ) && hitem != GetRootItem() ) {
			int index = model->m_frameManager.findFrameIndex( hitem );
			if ( index != -1 ) {
				RDOStudioFrameDoc* doc = model->m_frameManager.getFrameDoc( index );
				if ( !doc ) {
					model->m_frameManager.connectFrameDoc( index );
				} else {
					studioApp.m_pMainFrame->MDIActivate( doc->getView()->GetParentFrame() );
				}
			}
		}
	}
}

void RDOStudioFrameTreeCtrl::OnHelpKeyword()
{
	tstring filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;
	filename += "::/html/work_model_frame.htm#frame";
	::HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );
}
