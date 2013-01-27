/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerrestype.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerrestype.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerResParamInfo
// --------------------------------------------------------------------------------
TracerResParamInfo::TracerResParamInfo( const TracerResParamType type ) :
	paramType( type ),
	enumValues( NULL )
{
	if ( paramType == RDOPT_ENUMERATIVE || paramType == RDOPT_BOOL || paramType == RDOPT_STRING )
		enumValues = new RDOStringVector();
}

TracerResParamInfo::~TracerResParamInfo()
{
	if ( enumValues )
		delete enumValues;
};

int TracerResParamInfo::addEnumValue( CREF(tstring) value )
{
	if ( !enumValues )
		return -1;
	enumValues->push_back( value );
	return enumValues->size() - 1;
}

int TracerResParamInfo::addStringValue( CREF(tstring) value )
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

tstring TracerResParamInfo::getEnumValue( unsigned int index ) const
{
	if ( !enumValues )
		return nullStr;
	if ( index >= enumValues->size() || index < 0 )
		return nullStr;
	return enumValues->at( index );
}

// --------------------------------------------------------------------------------
// -------------------- TracerResType
// --------------------------------------------------------------------------------
TracerResType::TracerResType( const TracerResTypeKind kind ) :
	ChartTreeItem(),
	restypeKind( kind )
{
}

TracerResType::~TracerResType()
{
	int count = paramsInfo.size();
	for (int i = 0; i < count; i++ ) {
		delete paramsInfo.at( i );
	}
};

int TracerResType::addParamInfo( TracerResParamInfo* const value )
{
	paramsInfo.push_back( value );
	return paramsInfo.size() - 1;
}

TracerResParamInfo* TracerResType::getParamInfo( unsigned int index ) const
{
	if ( index >= paramsInfo.size() || index < 0 )
		return NULL;
	return paramsInfo.at( index );
}