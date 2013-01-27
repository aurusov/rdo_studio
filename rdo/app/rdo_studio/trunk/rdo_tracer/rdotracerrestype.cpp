/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerrestype.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
// --------------------------------------------------------------------------------

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
	if ( paramType == RDOPT_ENUMERATIVE || paramType == RDOPT_BOOL || paramType == RDOPT_STRING )
		enumValues = new RDOStringVector();
}

RDOTracerResParamInfo::~RDOTracerResParamInfo()
{
	if ( enumValues )
		delete enumValues;
};

int RDOTracerResParamInfo::addEnumValue( CREF(tstring) value )
{
	if ( !enumValues )
		return -1;
	enumValues->push_back( value );
	return enumValues->size() - 1;
}

int RDOTracerResParamInfo::addStringValue( CREF(tstring) value )
{
	if ( !enumValues )
		return -1;

	RDOStringVector::const_iterator it = std::find(enumValues->begin(), enumValues->end(), value);
	if (it != enumValues->end())
	{
		return it - enumValues->begin();
	}

	enumValues->push_back( value );
	return enumValues->size() - 1;
}

static tstring nullStr = "";

tstring RDOTracerResParamInfo::getEnumValue( unsigned int index ) const
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
	ChartTreeItem(),
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