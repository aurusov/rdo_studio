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

	PTR(RDOEditorEdit) pPage1  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage2  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage3  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage4  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage5  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage6  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage7  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage8  = new RDOEditorEdit( view );
	PTR(RDOEditorEdit) pPage9  = new RDOEditorEdit( view );

	pPage1->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage2->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage3->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage4->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage5->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage6->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage7->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage8->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );
	pPage9->Create ( NULL, NULL, 0, CRect(0, 0, 0, 0), getTabAsParent(), 0 );

	pPage1->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage2->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage3->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage4->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage5->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage6->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage7->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage8->setEditorStyle( &studioApp.mainFrame->style_editor );
	pPage9->setEditorStyle( &studioApp.mainFrame->style_editor );

	pPage1->setPopupMenu( &view->popupMenu );
	pPage2->setPopupMenu( &view->popupMenu );
	pPage3->setPopupMenu( &view->popupMenu );
	pPage4->setPopupMenu( &view->popupMenu );
	pPage5->setPopupMenu( &view->popupMenu );
	pPage6->setPopupMenu( &view->popupMenu );
	pPage7->setPopupMenu( &view->popupMenu );
	pPage8->setPopupMenu( &view->popupMenu );
	pPage9->setPopupMenu( &view->popupMenu );

	group.insert( pPage1 );
	group.insert( pPage2 );
	group.insert( pPage3 );
	group.insert( pPage4 );
	group.insert( pPage5 );
	group.insert( pPage6 );
	group.insert( pPage7 );
	group.insert( pPage8 );
	group.insert( pPage9 );
	pPage1->setGroup( &group );
	pPage2->setGroup( &group );
	pPage3->setGroup( &group );
	pPage4->setGroup( &group );
	pPage5->setGroup( &group );
	pPage6->setGroup( &group );
	pPage7->setGroup( &group );
	pPage8->setGroup( &group );
	pPage9->setGroup( &group );

	insertItem(pPage1, _T("PAT"));
	insertItem(pPage2, _T("RTP"));
	insertItem(pPage3, _T("RSS"));
	insertItem(pPage4, _T("OPR"));
	insertItem(pPage5, _T("FRM"));
	insertItem(pPage6, _T("FUN"));
	insertItem(pPage7, _T("DPT"));
	insertItem(pPage8, _T("SMR"));
	insertItem(pPage9, _T("PMD"));

	setCurrentRDOItem( rdoModelObjects::PAT );

	return 0;
}

rdoModelObjects::RDOFileType RDOEditorTabCtrl::indexToType( const int index ) const
{
	switch ( index ) {
		case  0: return rdoModelObjects::PAT;
		case  1: return rdoModelObjects::RTP;
		case  2: return rdoModelObjects::RSS;
		case  3: return rdoModelObjects::OPR;
		case  4: return rdoModelObjects::FRM;
		case  5: return rdoModelObjects::FUN;
		case  6: return rdoModelObjects::DPT;
		case  7: return rdoModelObjects::SMR;
		case  8: return rdoModelObjects::PMD;
		default: NEVER_REACH_HERE;
	}
}

int RDOEditorTabCtrl::typeToIndex( const rdoModelObjects::RDOFileType type ) const
{
	switch ( type ) {
		case rdoModelObjects::PAT     : return 0;
		case rdoModelObjects::RTP     : return 1;
		case rdoModelObjects::RSS     : return 2;
		case rdoModelObjects::OPR     : return 3;
		case rdoModelObjects::FRM     : return 4;
		case rdoModelObjects::FUN     : return 5;
		case rdoModelObjects::DPT     : return 6;
		case rdoModelObjects::SMR     : return 7;
		case rdoModelObjects::PMD     : return 8;
	}
	NEVER_REACH_HERE;
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
