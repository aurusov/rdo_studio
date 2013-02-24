/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerrestype.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_
#define _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- TracerResourceParamInfo
// --------------------------------------------------------------------------------
class TracerResourceParamInfo
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

	TracerResourceParamInfo(const ParamType type);
	virtual ~TracerResourceParamInfo();

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
// -------------------- TracerResourceType
// --------------------------------------------------------------------------------
class TracerResourceType: public ChartTreeItem
{
DECLARE_FACTORY(TracerResourceType)
public:
	enum Kind
	{
		RDOTK_PERMANENT = 0, RDOTK_TEMPORARY
	};

	CREF(QString) getName() const;
	void          setName(CREF(QString) name);

	Kind getKind() const;

	int addParamInfo(TracerResourceParamInfo* const value);
	TracerResourceParamInfo* getParamInfo(unsigned int index) const;
	int getParamsCount() const;

private:
	TracerResourceType(Kind kind);
	virtual ~TracerResourceType();

	typedef  std::vector<TracerResourceParamInfo*>  ParamInfoList;

	QString       m_name;
	Kind          m_kind;
	ParamInfoList m_paramInfoList;
};

typedef rdo::intrusive_ptr<TracerResourceType> LPTracerResourceType;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_
