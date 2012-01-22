#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_messages.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"

namespace rp {
// --------------------------------------------------------------------------------
// -------------------- RPMessages
// --------------------------------------------------------------------------------
msg::msg(): RPObject( NULL )
{
	connect( this, RP_OBJ_BEFOREDELETE );
}

msg::~msg()
{
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
			it = connected.erase( it );
			if ( it == connected.end() ) break;
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
			if ( it == connected.end() ) break;
		}
		it++;
	}
}

void msg::sendMessage( RPObject* from, UINT message, void* param )
{
	Connected connected_backup = connected;
	Connected::iterator it = connected_backup.find( message );
	while ( it != connected_backup.end() ) {
		it->second->notify( from, message, param );
		it++;
	}
}

void msg::notify( RPObject* from, UINT message, void* param )
{
	UNUSED(param);

	if ( message == RP_OBJ_BEFOREDELETE ) {
		disconnect( from );
	}
}

}
