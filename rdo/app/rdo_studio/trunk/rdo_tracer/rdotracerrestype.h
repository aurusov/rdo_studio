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
#include "app/rdo_studio/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerResParamInfo
// --------------------------------------------------------------------------------
enum TracerResParamType
{
	RDOPT_INTEGER   = 0,
	RDOPT_REAL,
	RDOPT_ENUMERATIVE,
	RDOPT_BOOL,
	RDOPT_ARRAY,
	RDOPT_STRING
};

typedef std::vector<tstring> RDOStringVector;

class TracerResParamInfo
{
public:
	TracerResParamInfo(const TracerResParamType type);
	virtual ~TracerResParamInfo();

	CREF(QString) getName() const;
	void setName(CREF(QString) name);

	TracerResParamType getParamType() const
	{
		return paramType;
	}
	int addEnumValue (CREF(tstring) value);int addStringValue(CREF(tstring) value);
	tstring getEnumValue(unsigned int index) const;
	int getEnumCount() const
	{
		return enumValues ? enumValues->size() : 0;
	}

protected:
	RDOStringVector* enumValues;
	TracerResParamType paramType;

private:
	QString m_name;
};

// --------------------------------------------------------------------------------
// -------------------- TracerResType
// --------------------------------------------------------------------------------
class TracerResourceType: public ChartTreeItem
{
DECLARE_FACTORY(TracerResourceType)
public:
	enum Kind
	{
		RDOTK_PERMANENT = 0, RDOTK_TEMPORARY
	};

	Kind getKind() const;

	CREF(QString) getName() const;
	void setName(CREF(QString) name);

	int addParamInfo(TracerResParamInfo* const value);
	TracerResParamInfo* getParamInfo(unsigned int index) const;
	int getParamsCount() const;

protected:
	typedef  std::vector<TracerResParamInfo*>  ParamInfoList;

	ParamInfoList m_paramInfoList;
	Kind          m_kind;

private:
	TracerResourceType(Kind kind);
	virtual ~TracerResourceType();

	QString m_name;
};

typedef rdo::intrusive_ptr<TracerResourceType> LPTracerResourceType;

#endif // _RDO_STUDIO_TRACER_RDOTRACERRESTYPE_H_
