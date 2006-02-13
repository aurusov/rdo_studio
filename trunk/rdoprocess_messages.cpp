#include "stdafx.h"
#include "rdoprocess_messages.h"
#include "rdoprocess_object.h"
#include "rdoprocess_app.h"

namespace rp {
// ----------------------------------------------------------------------------
// ---------- RPMessages
// ----------------------------------------------------------------------------
const UINT msg::RP_OBJ_BEFOREDELETE  = ::RegisterWindowMessage( "RP_OBJ_BEFOREDELETE" );
const UINT msg::RP_OBJ_NAMECHANGED   = ::RegisterWindowMessage( "RP_OBJ_NAMECHANGED" );
const UINT msg::RP_OBJ_SELCHANGED    = ::RegisterWindowMessage( "RP_OBJ_SELCHANGED" );
const UINT msg::RP_FLOWSTATE_CHANGED = ::RegisterWindowMessage( "RP_FLOWSTATE_CHANGED" );

msg::msg(): RPObject( NULL )
{
	setName( "__msg" );
	connect( this, RP_OBJ_BEFOREDELETE );
}

msg::~msg()
{
	rpapp._msg = NULL;
}

void msg::connect( RPObject* to, UINT message )
{
	Connected::iterator it = connected.find( message );
	while ( it != connected.end() ) {
		if ( it->second == to ) break;
		it++;
	}
	if ( it == connected.end() )
		connected.insert( Connected::value_type( message, to ) );
}

void msg::disconnect( RPObject* to, UINT message )
{
	Connected::iterator it = connected.find( message );
	while ( it != connected.end() ) {
		if ( it->second == to ) {
			connected.erase( it );
			break;
		}
		it++;
	}
}

void msg::disconnect( RPObject* to )
{
	Connected::iterator it = connected.begin();
	while ( it != connected.end() ) {
		if ( it->second == to ) {
			it = connected.erase( it );
		}
		it++;
	}
}

void msg::sendMessage( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	Connected connected_backup = connected;
	Connected::iterator it = connected_backup.find( message );
	while ( it != connected_backup.end() ) {
		it->second->notify( from, message, wParam, lParam );
		it++;
	}
}

void msg::notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( message == RP_OBJ_BEFOREDELETE ) {
		disconnect( from );
	}
}

}
