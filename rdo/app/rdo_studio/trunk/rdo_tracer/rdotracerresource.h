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
PREDECLARE_POINTER(TracerResource);

class TracerResParam: public TracerSerie
{
DECLARE_FACTORY(TracerResParam)
public:
	CREF(LPTracerResource) getResource() const
	{
		return m_pResource;
	}

	TracerResParamInfo* getParamInfo() const;

	virtual void getCaptions(std::vector<tstring> &captions, const int val_count) const;

protected:
	TracerResParamInfo* paramInfo;
	LPTracerResource    m_pResource;

private:
	TracerResParam(CREF(LPTracerResource) pResource);
	virtual ~TracerResParam();
};

typedef  rdo::intrusive_ptr<TracerResParam>  LPTracerResParam;

// --------------------------------------------------------------------------------
// -------------------- TracerResource
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(TracerResType);

class TracerResource: public ChartTreeItem
{
DECLARE_FACTORY(TracerResource)
public:
	tstring Name;
	int     id;

	CREF(LPTracerResType) getType() const
	{
		return m_pResType;
	}

	void addParam(CREF(LPTracerResParam) pParam);
	LPTracerResParam getParam(unsigned int index) const;
	int getParamIndex(CREF(LPTracerResParam) pParam) const;
	void setParams(tstring& line, TracerTimeNow* const time, const int eventIndex, const rbool erasing = false);
	void setErased(const rbool value);
	rbool isErased()
	{
		return erased;
	}

protected:
	std::vector<LPTracerResParam> m_paramList;
	LPTracerResType               m_pResType;
	rbool erased;

private:
	TracerResource(CREF(LPTracerResType) pResType, CREF(tstring) name);
	virtual ~TracerResource();
};

typedef  rdo::intrusive_ptr<TracerResource>  LPTracerResource;

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESOURCE_H_
