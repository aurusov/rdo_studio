#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdo_edit/rdoeditortabctrl.h"
#include "rdo_studio/rdostudioapp.h"
#include "rdo_studio/rdostudiomainfrm.h"
#include "rdo_studio/rdostudioeditbaseview.h"
#include "rdo_studio/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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

	PTR(RDOEditorEdit) pPage1  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage2  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage3  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage4  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage5  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage6  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage7  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage8  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage9  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage10 = new RDOEditorEdit( view );

	pPage1->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage2->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage3->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage4->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage5->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage6->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage7->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage8->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage9->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage10->Create( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );

	pPage1->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage2->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage3->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage4->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage5->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage6->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage7->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage8->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage9->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage10->setEditorStyle( &studioApp.mainFrame->style_editor );

	pPage1->setPopupMenu( &view->popupMenu );
	pPage2->setPopupMenu( &view->popupMenu );
	pPage3->setPopupMenu( &view->popupMenu );
	pPage4->setPopupMenu( &view->popupMenu );
	pPage5->setPopupMenu( &view->popupMenu );
	pPage6->setPopupMenu( &view->popupMenu );
	pPage7->setPopupMenu( &view->popupMenu );
	pPage8->setPopupMenu( &view->popupMenu );
	pPage9->setPopupMenu( &view->popupMenu );
	pPage10->setPopupMenu( &view->popupMenu );

	group.insert( pPage1 );
	group.insert( pPage2 );
	group.insert( pPage3 );
	group.insert( pPage4 );
	group.insert( pPage5 );
	group.insert( pPage6 );
	group.insert( pPage7 );
	group.insert( pPage8 );
	group.insert( pPage9 );
	group.insert( pPage10);
	pPage1->setGroup( &group );

	pPage2->setGroup ( &group );
	pPage3->setGroup ( &group );
	pPage4->setGroup ( &group );
	pPage5->setGroup ( &group );
	pPage6->setGroup ( &group );
	pPage7->setGroup ( &group );
	pPage8->setGroup ( &group );
	pPage9->setGroup ( &group );
	pPage10->setGroup( &group );

	insertItem( pPage1 , "RTP" );
	insertItem( pPage2 , "RSS" );
	insertItem( pPage3 , "EVN" );
	insertItem( pPage4 , "PAT" );
	insertItem( pPage5 , "DPT" );
	insertItem( pPage6 , "PRC" );
	insertItem( pPage7 , "FRM" );
	insertItem( pPage8 , "FUN" );
	insertItem( pPage9 , "SMR" );
	insertItem( pPage10, "PMD" );

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
