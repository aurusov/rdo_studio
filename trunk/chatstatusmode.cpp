#include "stdafx.h"
#include "chatstatusmode.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatStatusMode
// ----------------------------------------------------------------------------
CChatStatusMode::CChatStatusMode():
	type( CSMT_Online ),
	info( "" )
{
	name = format( IDS_STATUSMODE_ONLINE );
}

CChatStatusMode::CChatStatusMode( const CChatStatusMode& statusMode )
{
	*this = statusMode;
}

CChatStatusMode::~CChatStatusMode()
{
}

CChatStatusMode& CChatStatusMode::operator= ( const CChatStatusMode& statusMode )
{
	type = statusMode.type;
	name = statusMode.name;
	info = statusMode.info;
	return *this;
}

bool CChatStatusMode::operator== ( const CChatStatusMode& statusMode )
{
	return type == statusMode.type &&
	       name == statusMode.name &&
	       info == statusMode.info;
}

bool CChatStatusMode::operator!= ( const CChatStatusMode& statusMode )
{
	return !( *this == statusMode );
}

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeList
// ----------------------------------------------------------------------------
CChatStatusModeList::CChatStatusModeList()
{
}

CChatStatusModeList::~CChatStatusModeList()
{
	clear();
}

void CChatStatusModeList::clear()
{
	std::vector< CChatStatusMode* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

CChatStatusModeList& CChatStatusModeList::operator= ( const CChatStatusModeList& statusModeList )
{
	clear();
	std::vector< CChatStatusMode* >::const_iterator it = statusModeList.list.begin();
	while ( it != statusModeList.list.end() ) {
		CChatStatusMode* status = *it++;
		list.push_back( new CChatStatusMode( *status ) );
	}
	return *this;
}

bool CChatStatusModeList::operator== ( const CChatStatusModeList& statusModeList )
{
	std::vector< CChatStatusMode* >::const_iterator it1 = list.begin();
	std::vector< CChatStatusMode* >::const_iterator it2 = statusModeList.list.begin();
	while ( it1 != list.end() && it2 != statusModeList.list.end() ) {
		CChatStatusMode* status1 = *it1++;
		CChatStatusMode* status2 = *it2++;
		if ( *status1 != *status2 ) {
			return false;
		};
	}
	return true;
}

bool CChatStatusModeList::operator!= ( const CChatStatusModeList& statusModeList )
{
	return !( *this == statusModeList );
}

void CChatStatusModeList::init()
{
	CWinApp* app = AfxGetApp();
	CChatStatusMode* statusMode;
	statusMode = new CChatStatusMode();
	statusMode->type = CSMT_Online;
	statusMode->name = app->GetProfileString( "StatusMode\\Online", "name", getDefaultName( CSMT_Online ).c_str() );
	statusMode->info = app->GetProfileString( "StatusMode\\Online", "info", "" );
	list.push_back( statusMode );
	statusMode = new CChatStatusMode();
	statusMode->type = CSMT_Away;
	statusMode->name = app->GetProfileString( "StatusMode\\Away", "name", getDefaultName( CSMT_Away ).c_str() );
	statusMode->info = app->GetProfileString( "StatusMode\\Away", "info", "" );
	list.push_back( statusMode );
	statusMode = new CChatStatusMode();
	statusMode->type = CSMT_NotAvailible;
	statusMode->name = app->GetProfileString( "StatusMode\\NotAvailible", "name", getDefaultName( CSMT_NotAvailible ).c_str() );
	statusMode->info = app->GetProfileString( "StatusMode\\NotAvailible", "info", "" );
	list.push_back( statusMode );
}

void CChatStatusModeList::saveSetting() const
{
	CWinApp* app = AfxGetApp();
	CChatStatusMode* statusMode;
	statusMode = getStatusMode( CSMT_Online );
	app->WriteProfileString( "StatusMode\\Online", "name", statusMode->name.c_str() );
	app->WriteProfileString( "StatusMode\\Online", "info", statusMode->info.c_str() );
	statusMode = getStatusMode( CSMT_Away );
	app->WriteProfileString( "StatusMode\\Away", "name", statusMode->name.c_str() );
	app->WriteProfileString( "StatusMode\\Away", "info", statusMode->info.c_str() );
	statusMode = getStatusMode( CSMT_NotAvailible );
	app->WriteProfileString( "StatusMode\\NotAvailible", "name", statusMode->name.c_str() );
	app->WriteProfileString( "StatusMode\\NotAvailible", "info", statusMode->info.c_str() );
}

CChatStatusMode* CChatStatusModeList::getStatusMode( const CChatStatusModeType statusModeType ) const
{
	std::vector< CChatStatusMode* >::const_iterator it = list.begin();
	while ( it != list.end() ) {
		if ( (*it)->type == statusModeType ) {
			return *it;
		}
		it++;
	}
	return NULL;
}

std::string CChatStatusModeList::getDefaultName( const CChatStatusModeType statusModeType ) const
{
	std::string s;
	int res;
	switch ( statusModeType ) {
		case CSMT_Online       : res = IDS_STATUSMODE_ONLINE; break;
		case CSMT_Away         : res = IDS_STATUSMODE_AWAY; break;
		case CSMT_NotAvailible : res = IDS_STATUSMODE_NOTAVAILIBLE; break;
		default                : res = -1;
	}
	if ( res != -1 ) {
		s = format( res );
	} else {
		s = "";
	}
	return s;
}

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeDialog
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatStatusModeDialog, CDialog )
	//{{AFX_MSG_MAP(CChatStatusModeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatStatusModeDialog::CChatStatusModeDialog( UINT nIDTemplate, CWnd* pParentWnd ):
	CDialog( nIDTemplate, pParentWnd ),
	info( "" ),
	useAsDefault( 0 )
{
}

CChatStatusModeDialog::~CChatStatusModeDialog()
{
}

void CChatStatusModeDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Text( pDX, IDC_INFO_EDIT, info );
	DDX_Check( pDX, IDC_INFO_CHECK, useAsDefault );
}

int CChatStatusModeDialog::DoModal()
{
	return CDialog::DoModal();
}
