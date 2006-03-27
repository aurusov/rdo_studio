#include "stdafx.h"
#include "rdoprocess_connector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPConnector
// ----------------------------------------------------------------------------
RPConnector::RPConnector( RPObject* _parent, const rp::string& _name ):
	RPObjectChart( _parent, _name ),
	dock_begin( NULL ),
	dock_end( NULL )
{
}

RPConnector::~RPConnector()
{
}
