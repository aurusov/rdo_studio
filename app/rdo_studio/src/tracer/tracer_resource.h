/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_resource.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RESOURCE_H_
#define _RDO_STUDIO_TRACER_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- Param
// --------------------------------------------------------------------------------
class ParamInfo;
PREDECLARE_POINTER(Resource);

class Param: public Serie
{
DECLARE_FACTORY(Param)
public:
	const LPResource& getResource () const;
	ParamInfo*        getParamInfo() const;

	virtual void getCaptions(std::vector<std::string>& captions, const int valueCount) const;

protected:
	ParamInfo*  paramInfo;
	LPResource  m_pResource;

private:
	Param(const LPResource& pResource);
	virtual ~Param();
};

typedef  rdo::intrusive_ptr<Param>  LPParam;

// --------------------------------------------------------------------------------
// -------------------- Resource
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ResourceType);

class Resource: public ChartTreeItem
{
DECLARE_FACTORY(Resource)
public:
	const QString& getName() const;
	void           setName(const QString& name);

	int getID() const;

	const LPResourceType& getType() const;

	void addParam(const LPParam& pParam);
	LPParam getParam(unsigned int index) const;
	int getParamIndex(const LPParam& pParam) const;
	void setParams(std::string& line, Time* const time, const int eventIndex, const bool erasing = false);
	void setErased(const bool value);
	bool isErased() const;

private:
	Resource(const LPResourceType& pResType, const QString& name, int id);
	virtual ~Resource();

	typedef  std::vector<LPParam>  ResourceParamList;

	QString            m_name;
	int                m_id;
	ResourceParamList  m_paramList;
	LPResourceType     m_pResourceType;
	bool               m_erased;
};

typedef  rdo::intrusive_ptr<Resource>  LPResource;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RESOURCE_H_
