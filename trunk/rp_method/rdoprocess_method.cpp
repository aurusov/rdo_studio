#include "stdafx.h"
#include "rdoprocess_method.h"
#include "rdoprocess_factory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rpMethod;

// ----------------------------------------------------------------------------
// ---------- RPMethod
// ----------------------------------------------------------------------------
RPMethod::RPMethod( RPObject* _parent ):
	RPObject( _parent, "method" ),
	pixmap_big( NULL ),
	pixmap_small( NULL )
{
}

RPMethod::~RPMethod()
{
	if ( pixmap_big ) delete pixmap_big;
	if ( pixmap_small ) delete pixmap_small;
}
