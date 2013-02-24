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
#include "app/rdo_studio/src/tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- TracerResourceParam
// --------------------------------------------------------------------------------
class TracerResourceParamInfo;
PREDECLARE_POINTER(TracerResource);

class TracerResourceParam: public TracerSerie
{
DECLARE_FACTORY(TracerResourceParam)
public:
	CREF(LPTracerResource)   getResource () const;
	TracerResourceParamInfo* getParamInfo() const;

	virtual void getCaptions(std::vector<tstring>& captions, const int valueCount) const;

protected:
	TracerResourceParamInfo* paramInfo;
	LPTracerResource         m_pResource;

private:
	TracerResourceParam(CREF(LPTracerResource) pResource);
	virtual ~TracerResourceParam();
};

typedef  rdo::intrusive_ptr<TracerResourceParam>  LPTracerResourceParam;

// --------------------------------------------------------------------------------
// -------------------- TracerResource
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(TracerResourceType);

class TracerResource: public ChartTreeItem
{
DECLARE_FACTORY(TracerResource)
public:
	CREF(QString) getName() const;
	void          setName(CREF(QString) name);

	int getID() const;

	CREF(LPTracerResourceType) getType() const;

	void addParam(CREF(LPTracerResourceParam) pParam);
	LPTracerResourceParam getParam(unsigned int index) const;
	int getParamIndex(CREF(LPTracerResourceParam) pParam) const;
	void setParams(tstring& line, TracerTimeNow* const time, const int eventIndex, const rbool erasing = false);
	void setErased(const rbool value);
	rbool isErased() const;

private:
	TracerResource(CREF(LPTracerResourceType) pResType, CREF(QString) name, int id);
	virtual ~TracerResource();

	typedef  std::vector<LPTracerResourceParam>  ResourceParamList;

	QString               m_name;
	int                   m_id;
	ResourceParamList     m_paramList;
	LPTracerResourceType  m_pResourceType;
	rbool                 m_erased;
};

typedef  rdo::intrusive_ptr<TracerResource>  LPTracerResource;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESOURCE_H_
