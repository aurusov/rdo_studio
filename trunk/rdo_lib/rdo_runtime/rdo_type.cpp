#include "pch.h"
#include "rdo_type.h"
#include "rdo_exception.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
const RDOType& RDOType::getTypeByID( ID id )
{
	switch ( id )
	{
		case t_unknow       : return g_unknow;
		case t_identificator: return g_identificator;
		case t_int          : return g_int;
		case t_real         : return g_real;
		case t_bool         : return g_bool;
		case t_string       : return g_string;
	}
	throw RDOTypeException();
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE( Class ) RDOType__##Class g_##Class;

DECLARE_ATOM_TYPE( unknow        );
DECLARE_ATOM_TYPE( identificator );
DECLARE_ATOM_TYPE( int           );
DECLARE_ATOM_TYPE( real          );
DECLARE_ATOM_TYPE( bool          );
DECLARE_ATOM_TYPE( string        );

} // namespace rdoRuntime
