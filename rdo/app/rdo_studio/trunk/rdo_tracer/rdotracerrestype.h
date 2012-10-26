/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerrestype.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACERRESTYPE_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACERRESTYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreeitem.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResParamInfo
// --------------------------------------------------------------------------------
enum RDOTracerResParamType {
	RDOPT_INTEGER   = 0,
	RDOPT_REAL,
	RDOPT_ENUMERATIVE,
	RDOPT_BOOL,
	RDOPT_ARRAY,
	RDOPT_STRING
};
	
typedef std::vector <tstring> RDOStringVector;

class RDOTracerResParamInfo
{
protected:
	RDOStringVector* enumValues;
	RDOTracerResParamType paramType;
public:
	RDOTracerResParamInfo( const RDOTracerResParamType type );
	virtual ~RDOTracerResParamInfo();

	tstring Name;
	RDOTracerResParamType getParamType() const { return paramType; };
	int addEnumValue  ( CREF(tstring) value );
	int addStringValue( CREF(tstring) value );
	tstring getEnumValue( unsigned int index ) const;
	int getEnumCount() const { return enumValues ? enumValues->size() : 0; };
};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResType
// --------------------------------------------------------------------------------
enum RDOTracerResTypeKind {
	RDOTK_PERMANENT = 0,
	RDOTK_TEMPORARY
};

class RDOTracerResType : public RDOTracerTreeItem
{
protected:
	std::vector <RDOTracerResParamInfo*> paramsInfo;
	RDOTracerResTypeKind restypeKind;
public:
	RDOTracerResType( const RDOTracerResTypeKind kind );
	virtual ~RDOTracerResType();

	tstring Name;
	RDOTracerResTypeKind getResTypeKind() const { return restypeKind; };
	int addParamInfo( RDOTracerResParamInfo* const value );
	RDOTracerResParamInfo* getParamInfo( unsigned int index ) const;
	const int getParamsCount() const { return paramsInfo.size(); };
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERRESTYPE_H_
