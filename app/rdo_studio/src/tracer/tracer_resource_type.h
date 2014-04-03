/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_resource_type.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RESOURCE_TYPE_H_
#define _RDO_STUDIO_TRACER_RESOURCE_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- ParamInfo
// --------------------------------------------------------------------------------
class ParamInfo
{
public:
	enum ParamType
	{
		PT_INTEGER = 0,
		PT_REAL,
		PT_ENUMERATIVE,
		PT_BOOL,
		PT_ARRAY,
		PT_STRING
	};

	ParamInfo(const ParamType type);
	virtual ~ParamInfo();

	CREF(QString) getName() const;
	void          setName(CREF(QString) name);

	ParamType getParamType() const;

	int     addEnumValue  (CREF(tstring) value);
	int     addStringValue(CREF(tstring) value);
	tstring getEnumValue  (unsigned int index) const;
	int     getEnumCount  () const;

private:
	typedef std::vector<tstring> EnumValueList;

	QString        m_name;
	EnumValueList* m_enumValueList;
	ParamType      m_paramType;
};

// --------------------------------------------------------------------------------
// -------------------- ResourceType
// --------------------------------------------------------------------------------
class ResourceType: public ChartTreeItem
{
DECLARE_FACTORY(ResourceType)
public:
	enum Kind
	{
		RDOTK_PERMANENT = 0, RDOTK_TEMPORARY
	};

	CREF(QString) getName() const;
	void          setName(CREF(QString) name);

	Kind getKind() const;

	int        addParamInfo(ParamInfo* const value);
	ParamInfo* getParamInfo(unsigned int index) const;
	int        getParamsCount() const;

private:
	ResourceType(Kind kind);
	virtual ~ResourceType();

	typedef  std::vector<ParamInfo*>  ParamInfoList;

	QString       m_name;
	Kind          m_kind;
	ParamInfoList m_paramInfoList;
};

typedef rdo::intrusive_ptr<ResourceType> LPResourceType;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RESOURCE_TYPE_H_
