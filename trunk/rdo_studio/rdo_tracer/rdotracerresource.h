#ifndef RDOTRACERRESOURCE_H
#define RDOTRACERRESOURCE_H
#pragma once

#include "rdotracerserie.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParam
// ----------------------------------------------------------------------------
class RDOTracerResParamInfo;
class RDOTracerResource;

class RDOTracerResParam: public RDOTracerSerie
{
protected:
	RDOTracerResParamInfo* paramInfo;
	RDOTracerResource* resource;
public:
	RDOTracerResParam( RDOTracerResource* const res );
	virtual ~RDOTracerResParam();

	RDOTracerResource* getResource() const { return resource; };
	RDOTracerResParamInfo* getParamInfo() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerResource
// ----------------------------------------------------------------------------
class RDOTracerResType;

class RDOTracerResource : public RDOTracerTreeItem
{
protected:
	vector <RDOTracerResParam*> params;
	RDOTracerResType* resType;
	bool erased;
public:
	RDOTracerResource( RDOTracerResType* const type, string& name );
	virtual ~RDOTracerResource();

	string Name;
	int id;
	RDOTracerResType* getType() const { return resType; };
	int addParam( RDOTracerResParam* const value );
	RDOTracerResParam* getParam( const int index ) const;
	int getParamIndex( const RDOTracerResParam* const param ) const;
	void setParams( string& line, RDOTracerTimeNow* const time, const int eventIndex, const bool erasing = false );
	void setErased( const bool value );
	bool isErased() { return erased; };
};

#endif // RDOTRACERRESOURCE_H