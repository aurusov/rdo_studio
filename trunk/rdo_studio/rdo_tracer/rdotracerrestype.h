#ifndef RDOTRACERRESTYPE_H
#define RDOTRACERRESTYPE_H
#pragma once

#include "./tracer_ctrls/rdotracertreeitem.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParamInfo
// ----------------------------------------------------------------------------
enum RDOTracerResParamType {
	RDOPT_INTEGER = 0,
	RDOPT_REAL,
	RDOPT_ENUMERATIVE
};
	
typedef vector <string> RDOStringVector;

class RDOTracerResParamInfo
{
protected:
	RDOStringVector* enumValues;
	RDOTracerResParamType paramType;
public:
	RDOTracerResParamInfo( const RDOTracerResParamType type );
	virtual ~RDOTracerResParamInfo();

	string Name;
	RDOTracerResParamType getParamType() const { return paramType; };
	int addEnumValue( const string& value );
	string getEnumValue( const int index ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerResType
// ----------------------------------------------------------------------------
enum RDOTracerResTypeKind {
	RDOTK_PERMANENT = 0,
	RDOTK_TEMPORARY
};

class RDOTracerResType : public RDOTracerTreeItem
{
protected:
	vector <RDOTracerResParamInfo*> paramsInfo;
	RDOTracerResTypeKind restypeKind;
public:
	RDOTracerResType( const RDOTracerResTypeKind kind );
	virtual ~RDOTracerResType();

	string Name;
	RDOTracerResTypeKind getResTypeKind() const { return restypeKind; };
	int addParamInfo( RDOTracerResParamInfo* const value );
	RDOTracerResParamInfo* getParamInfo( const int index ) const;
	const int getParamsCount() const { return paramsInfo.size(); };
};

#endif // RDOTRACERRESTYPE_H