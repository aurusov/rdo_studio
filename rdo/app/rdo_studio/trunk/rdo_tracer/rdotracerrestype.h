/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerrestype.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_
#define _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerResParamInfo
// --------------------------------------------------------------------------------
enum TracerResParamType {
	RDOPT_INTEGER   = 0,
	RDOPT_REAL,
	RDOPT_ENUMERATIVE,
	RDOPT_BOOL,
	RDOPT_ARRAY,
	RDOPT_STRING
};
	
typedef std::vector <tstring> RDOStringVector;

class TracerResParamInfo
{
protected:
	RDOStringVector* enumValues;
	TracerResParamType paramType;
public:
	TracerResParamInfo( const TracerResParamType type );
	virtual ~TracerResParamInfo();

	tstring Name;
	TracerResParamType getParamType() const { return paramType; };
	int addEnumValue  ( CREF(tstring) value );
	int addStringValue( CREF(tstring) value );
	tstring getEnumValue( unsigned int index ) const;
	int getEnumCount() const { return enumValues ? enumValues->size() : 0; };
};

// --------------------------------------------------------------------------------
// -------------------- TracerResType
// --------------------------------------------------------------------------------
enum TracerResTypeKind {
	RDOTK_PERMANENT = 0,
	RDOTK_TEMPORARY
};

class TracerResType : public ChartTreeItem
{
protected:
	std::vector <TracerResParamInfo*> paramsInfo;
	TracerResTypeKind restypeKind;
public:
	TracerResType( const TracerResTypeKind kind );
	virtual ~TracerResType();

	tstring Name;
	TracerResTypeKind getResTypeKind() const { return restypeKind; };
	int addParamInfo( TracerResParamInfo* const value );
	TracerResParamInfo* getParamInfo( unsigned int index ) const;
	const int getParamsCount() const { return paramsInfo.size(); };
};

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_
