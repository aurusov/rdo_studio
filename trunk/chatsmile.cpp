#include "stdafx.h"
#include "chatsmile.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSmile
// ----------------------------------------------------------------------------
CChatSmile::CChatSmile()
{
}

CChatSmile::~CChatSmile()
{
}

// ----------------------------------------------------------------------------
// ---------- CChatSmileList
// ----------------------------------------------------------------------------
CChatSmileList::CChatSmileList()
{
}

CChatSmileList::~CChatSmileList()
{
	std::vector< CChatSmile* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

CChatSmile* CChatSmileList::getSmile( const CChatSmile::Type type )
{
	std::vector< CChatSmile* >::iterator it = list.begin();
	while ( it != list.end() ) {
		if ( (*it)->type == type ) {
			return *it;
		}
		it++;
	}
	CChatSmile* smile = new CChatSmile;
	smile->type = type;
	list.push_back( smile );
	UINT res;
	switch ( type ) {
		case CChatSmile::eyes: res = IDR_EYES; break;
		default              : res = -1; break;
	}
	if ( res != -1 ) {
		smile->Create( "", WS_CHILD | WS_VISIBLE, CRect( 0, 0, 1, 1 ), &chatApp.mainFrame->childView );
		if ( smile->Load( MAKEINTRESOURCE( res ), _T("GIF") ) ) {
			smile->Draw();
		}
	}
	return smile;
}
