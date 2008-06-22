#include "pch.h"
#include "rdo_type.h"
#include "rdo_exception.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
const RDOType& RDOType::getTypeByID( ID id )
{
	switch ( id )
	{
		case t_unknow       : return g_unknow;
		case t_int          : return g_int;
		case t_real         : return g_real;
		case t_string       : return g_string;
		case t_identificator: return g_identificator;
	}
	throw RDOTypeException();
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE( Class ) RDOType__##Class g_##Class;

DECLARE_ATOM_TYPE( unknow        );
DECLARE_ATOM_TYPE( int           );
DECLARE_ATOM_TYPE( real          );
DECLARE_ATOM_TYPE( string        );
DECLARE_ATOM_TYPE( identificator );

} // namespace rdoRuntime
