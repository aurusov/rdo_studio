#include "stdafx.h"
#include "rdoeditortabctrl.h"
#include "../rdostudioapp.h"
#include "../rdostudiomainfrm.h"
#include "../rdostudioeditbaseview.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorTabCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorTabCtrl, RDOTabCtrl )
	//{{AFX_MSG_MAP(RDOEditorTabCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOEditorTabCtrl::RDOEditorTabCtrl( RDOStudioEditBaseView* _view ):
	RDOTabCtrl(),
	view( _view )
{
}

RDOEditorTabCtrl::~RDOEditorTabCtrl()
{
}

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

	page1->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page2->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page3->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page4->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page5->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page6->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page7->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page8->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );
	page9->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), -1 );

	page1->setEditorStyle( &studioApp.mainFrame->style_editor );
	page2->setEditorStyle( &studioApp.mainFrame->style_editor );
	page3->setEditorStyle( &studioApp.mainFrame->style_editor );
	page4->setEditorStyle( &studioApp.mainFrame->style_editor );
	page5->setEditorStyle( &studioApp.mainFrame->style_editor );
	page6->setEditorStyle( &studioApp.mainFrame->style_editor );
	page7->setEditorStyle( &studioApp.mainFrame->style_editor );
	page8->setEditorStyle( &studioApp.mainFrame->style_editor );
	page9->setEditorStyle( &studioApp.mainFrame->style_editor );

	page1->setPopupMenu( &view->popupMenu );
	page2->setPopupMenu( &view->popupMenu );
	page3->setPopupMenu( &view->popupMenu );
	page4->setPopupMenu( &view->popupMenu );
	page5->setPopupMenu( &view->popupMenu );
	page6->setPopupMenu( &view->popupMenu );
	page7->setPopupMenu( &view->popupMenu );
	page8->setPopupMenu( &view->popupMenu );
	page9->setPopupMenu( &view->popupMenu );

	group.insert( group.end(), page1 );
	group.insert( group.end(), page2 );
	group.insert( group.end(), page3 );
	group.insert( group.end(), page4 );
	group.insert( group.end(), page5 );
	group.insert( group.end(), page6 );
	group.insert( group.end(), page7 );
	group.insert( group.end(), page8 );
	group.insert( group.end(), page9 );
	page1->setGroup( &group );
	page2->setGroup( &group );
	page3->setGroup( &group );
	page4->setGroup( &group );
	page5->setGroup( &group );
	page6->setGroup( &group );
	page7->setGroup( &group );
	page8->setGroup( &group );
	page9->setGroup( &group );

	insertItem( page1, "PAT" );
	insertItem( page2, "RTP" );
	insertItem( page3, "RSS" );
	insertItem( page4, "OPR" );
	insertItem( page5, "FRM" );
	insertItem( page6, "FUN" );
	insertItem( page7, "DPT" );
	insertItem( page8, "SMR" );
	insertItem( page9, "PMD" );

	setCurrentRDOItem( RDOEDIT_PAT );

	return 0;
}

RDOEditorTabItem RDOEditorTabCtrl::getCurrentRDOItem() const
{
	switch ( getCurrentIndex() ) {
		case  0: return RDOEDIT_PAT;
		case  1: return RDOEDIT_RTP;
		case  2: return RDOEDIT_RSS;
		case  3: return RDOEDIT_OPR;
		case  4: return RDOEDIT_FRM;
		case  5: return RDOEDIT_FUN;
		case  6: return RDOEDIT_DPT;
		case  7: return RDOEDIT_SMR;
		case  8: return RDOEDIT_PMD;
		default: return RDOEDIT_PAT;
	}
}

void RDOEditorTabCtrl::setCurrentRDOItem( const RDOEditorTabItem item )
{
	int index = -1;
	switch ( item ) {
		case RDOEDIT_PAT: index =  0; break;
		case RDOEDIT_RTP: index =  1; break;
		case RDOEDIT_RSS: index =  2; break;
		case RDOEDIT_OPR: index =  3; break;
		case RDOEDIT_FRM: index =  4; break;
		case RDOEDIT_FUN: index =  5; break;
		case RDOEDIT_DPT: index =  6; break;
		case RDOEDIT_SMR: index =  7; break;
		case RDOEDIT_PMD: index =  8; break;
	}
	if ( index != -1 ) setCurrentItem( index );
}
