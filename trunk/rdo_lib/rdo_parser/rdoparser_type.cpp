#include "pch.h"
#include "rdoparser_type.h"
#include <rdo_exception.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
const RDOType& RDOType::getTypeByID( rdoRuntime::RDOType::ID id )
{
	switch ( id )
	{
		case rdoRuntime::RDOType::t_unknow       : return g_unknow;
		case rdoRuntime::RDOType::t_int          : return g_int;
		case rdoRuntime::RDOType::t_real         : return g_real;
		case rdoRuntime::RDOType::t_bool         : return g_bool;
		case rdoRuntime::RDOType::t_string       : return g_string;
		case rdoRuntime::RDOType::t_identificator: return g_identificator;
	}
	throw rdoRuntime::RDOTypeException();
}

const RDOType* RDOType__unknow::cast( const RDOType& toType ) const
{
	return NULL;
}

const RDOType* RDOType__int::cast( const RDOType& toType ) const
{
	switch ( toType->id() )
	{
		case rdoRuntime::RDOType__int::t_int : return &g_int;
		case rdoRuntime::RDOType__int::t_real: return &g_real;
	}
	return NULL;
}

const RDOType* RDOType__real::cast( const RDOType& toType ) const
{
	switch ( toType->id() )
	{
		case rdoRuntime::RDOType__int::t_int :
		case rdoRuntime::RDOType__int::t_real: return &g_real;
	}
	return NULL;
}

const RDOType* RDOType__string::cast( const RDOType& toType ) const
{
	switch ( toType->id() )
	{
		case rdoRuntime::RDOType__int::t_string: return &g_string;
	}
	return NULL;
}

const RDOType* RDOType__identificator::cast( const RDOType& toType ) const
{
	return NULL;
}

const RDOType* RDOType__bool::cast( const RDOType& toType ) const
{
	switch ( toType->id() )
	{
		case rdoRuntime::RDOType__int::t_bool: return &g_bool;
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER( Class ) RDOType__##Class g_##Class;

DECLARE_ATOM_TYPE_PARSER( unknow        );
DECLARE_ATOM_TYPE_PARSER( identificator );
DECLARE_ATOM_TYPE_PARSER( int           );
DECLARE_ATOM_TYPE_PARSER( real          );
DECLARE_ATOM_TYPE_PARSER( bool          );
DECLARE_ATOM_TYPE_PARSER( string        );

} // namespace rdoParse
