#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResParamInfo
// --------------------------------------------------------------------------------
RDOTracerResParamInfo::RDOTracerResParamInfo( const RDOTracerResParamType type ) :
	paramType( type ),
	enumValues( NULL )
{
	if ( paramType == RDOPT_ENUMERATIVE || paramType == RDOPT_BOOL )
		enumValues = new RDOStringVector();
}

RDOTracerResParamInfo::~RDOTracerResParamInfo()
{
	if ( enumValues )
		delete enumValues;
};

int RDOTracerResParamInfo::addEnumValue( const std::string& value )
{
	if ( !enumValues )
		return -1;
	enumValues->push_back( value );
	return enumValues->size() - 1;
}

static std::string nullStr = "";

std::string RDOTracerResParamInfo::getEnumValue( unsigned int index ) const
{
	if ( !enumValues )
		return nullStr;
	if ( index >= enumValues->size() || index < 0 )
		return nullStr;
	return enumValues->at( index );
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResType
// --------------------------------------------------------------------------------
RDOTracerResType::RDOTracerResType( const RDOTracerResTypeKind kind ) :
	RDOTracerTreeItem(),
	restypeKind( kind )
{
}

RDOTracerResType::~RDOTracerResType()
{
	int count = paramsInfo.size();
	for (int i = 0; i < count; i++ ) {
		delete paramsInfo.at( i );
	}
};

int RDOTracerResType::addParamInfo( RDOTracerResParamInfo* const value )
{
	paramsInfo.push_back( value );
	return paramsInfo.size() - 1;
}

RDOTracerResParamInfo* RDOTracerResType::getParamInfo( unsigned int index ) const
{
	if ( index >= paramsInfo.size() || index < 0 )
		return NULL;
	return paramsInfo.at( index );
}