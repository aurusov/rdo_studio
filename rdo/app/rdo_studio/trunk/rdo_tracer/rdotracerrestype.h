#ifndef RDOTRACERRESTYPE_H
#define RDOTRACERRESTYPE_H
#pragma once

#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracertreeitem.h"

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResParamInfo
// --------------------------------------------------------------------------------
enum RDOTracerResParamType {
	RDOPT_UNDEFINED = ~0,
	RDOPT_INTEGER   = 0,
	RDOPT_REAL,
	RDOPT_ENUMERATIVE,
	RDOPT_ARRAY
};
	
typedef std::vector <std::string> RDOStringVector;

class RDOTracerResParamInfo
{
protected:
	RDOStringVector* enumValues;
	RDOTracerResParamType paramType;
public:
	RDOTracerResParamInfo( const RDOTracerResParamType type );
	virtual ~RDOTracerResParamInfo();

	std::string Name;
	RDOTracerResParamType getParamType() const { return paramType; };
	int addEnumValue( const std::string& value );
	std::string getEnumValue( unsigned int index ) const;
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

	std::string Name;
	RDOTracerResTypeKind getResTypeKind() const { return restypeKind; };
	int addParamInfo( RDOTracerResParamInfo* const value );
	RDOTracerResParamInfo* getParamInfo( unsigned int index ) const;
	const int getParamsCount() const { return paramsInfo.size(); };
};

#endif // RDOTRACERRESTYPE_H