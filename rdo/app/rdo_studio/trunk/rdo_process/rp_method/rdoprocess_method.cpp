#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_factory.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rpMethod;

// --------------------------------------------------------------------------------
// -------------------- RPMethod
// --------------------------------------------------------------------------------
RPMethod::RPMethod( RPObject* _parent, const rp::string _name ):
	RPObject( _parent, _name ),
	pixmap_big( NULL ),
	pixmap_small( NULL )
{
}

RPMethod::~RPMethod()
{
	if ( pixmap_big ) delete pixmap_big;
	if ( pixmap_small ) delete pixmap_small;
}
