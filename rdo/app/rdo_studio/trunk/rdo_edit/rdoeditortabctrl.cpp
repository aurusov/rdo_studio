/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditortabctrl.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/edit/view_base.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorTabCtrl
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorTabCtrl, RDOTabCtrl )
	//{{AFX_MSG_MAP(RDOEditorTabCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOEditorTabCtrl::RDOEditorTabCtrl( RDOStudioEditBaseView* _view ):
	RDOTabCtrl(),
	view( _view )
{}

RDOEditorTabCtrl::~RDOEditorTabCtrl()
{}

BOOL RDOEditorTabCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !RDOTabCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

int RDOEditorTabCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOTabCtrl::OnCreate(lpCreateStruct) == -1 ) return -1;

	RDOEditorEdit* page1  = new RDOEditorEdit( view );
	RDOEditorEdit* page2  = new RDOEditorEdit( view );
	RDOEditorEdit* page3  = new RDOEditorEdit( view );
	RDOEditorEdit* page4  = new RDOEditorEdit( view );
	RDOEditorEdit* page5  = new RDOEditorEdit( view );
	RDOEditorEdit* page6  = new RDOEditorEdit( view );
	RDOEditorEdit* page7  = new RDOEditorEdit( view );
	RDOEditorEdit* page8  = new RDOEditorEdit( view );
	RDOEditorEdit* page9  = new RDOEditorEdit( view );
	RDOEditorEdit* page10 = new RDOEditorEdit( view );

	page1->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page2->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page3->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page4->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page5->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page6->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page7->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page8->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page9->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	page10->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );

	page1->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page2->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page3->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page4->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page5->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page6->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page7->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page8->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page9->setEditorStyle ( &studioApp.mainFrame->style_editor );
	page10->setEditorStyle( &studioApp.mainFrame->style_editor );

	page1->setPopupMenu ( &view->popupMenu );
	page2->setPopupMenu ( &view->popupMenu );
	page3->setPopupMenu ( &view->popupMenu );
	page4->setPopupMenu ( &view->popupMenu );
	page5->setPopupMenu ( &view->popupMenu );
	page6->setPopupMenu ( &view->popupMenu );
	page7->setPopupMenu ( &view->popupMenu );
	page8->setPopupMenu ( &view->popupMenu );
	page9->setPopupMenu ( &view->popupMenu );
	page10->setPopupMenu( &view->popupMenu );

	group.insert( page1 );
	group.insert( page2 );
	group.insert( page3 );
	group.insert( page4 );
	group.insert( page5 );
	group.insert( page6 );
	group.insert( page7 );
	group.insert( page8 );
	group.insert( page9 );
	group.insert( page10);
	page1->setGroup( &group );

	page2->setGroup ( &group );
	page3->setGroup ( &group );
	page4->setGroup ( &group );
	page5->setGroup ( &group );
	page6->setGroup ( &group );
	page7->setGroup ( &group );
	page8->setGroup ( &group );
	page9->setGroup ( &group );
	page10->setGroup( &group );

	insertItem( page1 , "RTP" );
	insertItem( page2 , "RSS" );
	insertItem( page3 , "EVN" );
	insertItem( page4 , "PAT" );
	insertItem( page5 , "DPT" );
	insertItem( page6 , "PRC" );
	insertItem( page7 , "FRM" );
	insertItem( page8 , "FUN" );
	insertItem( page9 , "SMR" );
	insertItem( page10, "PMD" );

	setCurrentRDOItem( rdoModelObjects::RTP );

	return 0;
}

rdoModelObjects::RDOFileType RDOEditorTabCtrl::indexToType( const int index ) const
{
	switch ( index )
	{
		case  0: return rdoModelObjects::RTP;
		case  1: return rdoModelObjects::RSS;
		case  2: return rdoModelObjects::EVN;
		case  3: return rdoModelObjects::PAT;
		case  4: return rdoModelObjects::DPT;
		case  5: return rdoModelObjects::PRC;
		case  6: return rdoModelObjects::FRM;
		case  7: return rdoModelObjects::FUN;
		case  8: return rdoModelObjects::SMR;
		case  9: return rdoModelObjects::PMD;
		default: return rdoModelObjects::PAT;
	}
}

int RDOEditorTabCtrl::typeToIndex( const rdoModelObjects::RDOFileType type ) const
{
	switch ( type )
	{
		case rdoModelObjects::RTP: return 0;
		case rdoModelObjects::RSS: return 1;
		case rdoModelObjects::EVN: return 2;
		case rdoModelObjects::PAT: return 3;
		case rdoModelObjects::DPT: return 4;
		case rdoModelObjects::PRC: return 5;
		case rdoModelObjects::FRM: return 6;
		case rdoModelObjects::FUN: return 7;
		case rdoModelObjects::SMR: return 8;
		case rdoModelObjects::PMD: return 9;
	}
	return -1;
}

void RDOEditorTabCtrl::setCurrentRDOItem( const rdoModelObjects::RDOFileType type )
{
	int index = typeToIndex( type );
	if ( index != -1 ) setCurrentItem( index );
}

RDOEditorEdit* RDOEditorTabCtrl::getItemEdit( const rdoModelObjects::RDOFileType type ) const
{
	int index = typeToIndex( type );
	return index != -1 ? getItemEdit( index ) : NULL;
}
