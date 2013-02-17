/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerresource.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERRESOURCE_H_
#define _RDO_STUDIO_TRACER_RDOTRACERRESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerResParam
// --------------------------------------------------------------------------------
class TracerResParamInfo;
class TracerResource;

class TracerResParam: public TracerSerie
{
public:
	TracerResParam(TracerResource* const res);
	virtual ~TracerResParam();

	TracerResource* getResource() const
	{
		return resource;
	}

	TracerResParamInfo* getParamInfo() const;

	virtual void getCaptions(std::vector<tstring> &captions, const int val_count) const;

protected:
	TracerResParamInfo* paramInfo;
	TracerResource* resource;
};

// --------------------------------------------------------------------------------
// -------------------- TracerResource
// --------------------------------------------------------------------------------
class TracerResType;

class TracerResource: public ChartTreeItem
{
public:
	TracerResource( TracerResType* const type, CREF(tstring) name );
	virtual ~TracerResource();

	tstring Name;
	int id;
	TracerResType* getType() const
	{
		return resType;
	}

	void addParam(TracerResParam* const value);
	TracerResParam* getParam(unsigned int index) const;
	int getParamIndex(const TracerResParam* const param) const;
	void setParams(tstring& line, TracerTimeNow* const time, const int eventIndex, const rbool erasing = false);
	void setErased(const rbool value);
	rbool isErased()
	{
		return erased;
	}

protected:
	CMutex mutex;
	std::vector<TracerResParam*> params;
	TracerResType* resType;
	rbool erased;
};

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESOURCE_H_
