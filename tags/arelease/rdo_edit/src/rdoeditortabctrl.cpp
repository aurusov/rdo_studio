#include "stdafx.h"
#include "rdoeditortabctrl.h"
#include "rdoeditorapp.h"
#include "rdoeditoreditstyle.h"
#include "sci/LexRdo.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOEditorTabCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorTabCtrl, RDOTabCtrl )
	ON_WM_CREATE()
END_MESSAGE_MAP()

RDOEditorTabCtrl::RDOEditorTabCtrl(): RDOTabCtrl()
{
}

RDOEditorTabCtrl::~RDOEditorTabCtrl()
{
	rdoEditorApp.eraseMenu( &popupMenu );
}

int RDOEditorTabCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOTabCtrl::OnCreate(lpCreateStruct) == -1 ) return -1;

	setFont( &rdoEditorApp.getFont() );

	popupMenu.CreatePopupMenu();

	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();

	rdoEditorApp.appendMenu( mainMenu, 5, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(1), 3, &popupMenu );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(1), 4, &popupMenu );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(1), 5, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(1), 10, &popupMenu );
	popupMenu.AppendMenu( MF_SEPARATOR );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(2), 0, &popupMenu );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(2), 3, &popupMenu );
	rdoEditorApp.appendMenu( mainMenu->GetSubMenu(2), 1, &popupMenu );

	RDOEditorEditStyle style;
	style.load();
	RDOEditorEdit* page1  = new RDOEditorEdit();
	RDOEditorEdit* page2  = new RDOEditorEdit();
	RDOEditorEdit* page3  = new RDOEditorEdit();
	RDOEditorEdit* page4  = new RDOEditorEdit();
	RDOEditorEdit* page5  = new RDOEditorEdit();
	RDOEditorEdit* page6  = new RDOEditorEdit();
	RDOEditorEdit* page7  = new RDOEditorEdit();
	RDOEditorEdit* page8  = new RDOEditorEdit();
	RDOEditorEdit* page9  = new RDOEditorEdit();
	RDOEditorEdit* page10 = new RDOEditorEdit();
	RDOEditorEdit* page11 = new RDOEditorEdit();

	page1->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page2->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page3->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page4->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page5->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page6->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page7->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page8->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page9->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page10->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );
	page11->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), &tab, -1 );

	page1->setEditorStyle( style );
	page2->setEditorStyle( style );
	page3->setEditorStyle( style );
	page4->setEditorStyle( style );
	page5->setEditorStyle( style );
	page6->setEditorStyle( style );
	page7->setEditorStyle( style );
	page8->setEditorStyle( style );
	page9->setEditorStyle( style );
	page10->setEditorStyle( style );
	page11->setEditorStyle( style );

	page1->setPopupMenu( &popupMenu );
	page2->setPopupMenu( &popupMenu );
	page3->setPopupMenu( &popupMenu );
	page4->setPopupMenu( &popupMenu );
	page5->setPopupMenu( &popupMenu );
	page6->setPopupMenu( &popupMenu );
	page7->setPopupMenu( &popupMenu );
	page8->setPopupMenu( &popupMenu );
	page9->setPopupMenu( &popupMenu );
	page10->setPopupMenu( &popupMenu );
	page11->setPopupMenu( &popupMenu );

	insertItem( page1, "PAT" );
	insertItem( page2, "RTP" );
	insertItem( page3, "RSS" );
	insertItem( page4, "OPR" );
	insertItem( page5, "FRM" );
	insertItem( page6, "FUN" );
	insertItem( page7, "DPT" );
	insertItem( page8, "SMR" );
	insertItem( page9, "PMD" );
	insertItem( page10, "PMV" );
	insertItem( page11, "TRC" );

	setCurrentRDOItem( RDOEDIT_PAT );

	return 0;
}

RDOEditorTabItem RDOEditorTabCtrl::getCurrentRDOItem() const
{
	switch ( tab.GetCurSel() ) {
		case  0: return RDOEDIT_PAT;
		case  1: return RDOEDIT_RTP;
		case  2: return RDOEDIT_RSS;
		case  3: return RDOEDIT_OPR;
		case  4: return RDOEDIT_FRM;
		case  5: return RDOEDIT_FUN;
		case  6: return RDOEDIT_DPT;
		case  7: return RDOEDIT_SMR;
		case  8: return RDOEDIT_PMD;
		case  9: return RDOEDIT_PMV;
		case 10: return RDOEDIT_TRC;
		default: return RDOEDIT_NONE;
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
		case RDOEDIT_PMV: index =  9; break;
		case RDOEDIT_TRC: index = 10; break;
		default: break;
	}
	if ( index != -1 ) setCurrentItem( index );
}

void RDOEditorTabCtrl::changeCurrentItem()
{
	RDOTabCtrl::changeCurrentItem();
	useFold = getCurrentRDOItem() != RDOEDIT_TRC;
}
