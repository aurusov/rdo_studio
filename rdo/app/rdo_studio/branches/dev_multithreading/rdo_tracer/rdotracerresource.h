#ifndef RDOTRACERRESOURCE_H
#define RDOTRACERRESOURCE_H

#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResParam
// --------------------------------------------------------------------------------
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

	virtual void getCaptions( std::vector<tstring> &captions, const int val_count ) const;
};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResource
// --------------------------------------------------------------------------------
class RDOTracerResType;

class RDOTracerResource : public RDOTracerTreeItem
{
protected:
	CMutex mutex;
	std::vector <RDOTracerResParam*> params;
	RDOTracerResType* resType;
	rbool erased;
public:
	RDOTracerResource( RDOTracerResType* const type, CREF(tstring) name );
	virtual ~RDOTracerResource();

	tstring Name;
	int id;
	RDOTracerResType* getType() const { return resType; };
	void addParam( RDOTracerResParam* const value );
	RDOTracerResParam* getParam( unsigned int index ) const;
	int getParamIndex( const RDOTracerResParam* const param ) const;
	void  setParams( tstring& line, RDOTracerTimeNow* const time, const int eventIndex, const rbool erasing = false );
	void  setErased( const rbool value );
	rbool isErased() { return erased; };
};

#endif // RDOTRACERRESOURCE_H