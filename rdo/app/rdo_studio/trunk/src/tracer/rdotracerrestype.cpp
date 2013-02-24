/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerrestype.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerrestype.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerResourceParamInfo
// --------------------------------------------------------------------------------
TracerResourceParamInfo::TracerResourceParamInfo(const ParamType type)
	: m_paramType(type)
	, m_enumValueList(NULL)
{
	if (m_paramType == PT_ENUMERATIVE || m_paramType == PT_BOOL || m_paramType == PT_STRING)
	{
		m_enumValueList = new EnumValueList();
	}
}

TracerResourceParamInfo::~TracerResourceParamInfo()
{
	if (m_enumValueList)
	{
		delete m_enumValueList;
	}
}

CREF(QString) TracerResourceParamInfo::getName() const
{
	return m_name;
}

void TracerResourceParamInfo::setName(CREF(QString) name)
{
	m_name = name;
}

TracerResourceParamInfo::ParamType TracerResourceParamInfo::getParamType() const
{
	return m_paramType;
}

int TracerResourceParamInfo::addEnumValue(CREF(tstring) value)
{
	if (!m_enumValueList)
		return -1;
	m_enumValueList->push_back(value);
	return m_enumValueList->size() - 1;
}

int TracerResourceParamInfo::addStringValue(CREF(tstring) value)
{
	if (!m_enumValueList)
		return -1;

	EnumValueList::const_iterator it = std::find(m_enumValueList->begin(), m_enumValueList->end(), value);
	if (it != m_enumValueList->end())
	{
		return it - m_enumValueList->begin();
	}

	m_enumValueList->push_back(value);
	return m_enumValueList->size() - 1;
}

static tstring nullStr = "";

tstring TracerResourceParamInfo::getEnumValue(unsigned int index) const
{
	if (!m_enumValueList)
		return nullStr;
	if (index >= m_enumValueList->size() || index < 0)
		return nullStr;
	return m_enumValueList->at(index);
}

int TracerResourceParamInfo::getEnumCount() const
{
	return m_enumValueList ? m_enumValueList->size() : 0;
}

// --------------------------------------------------------------------------------
// -------------------- TracerResourceType
// --------------------------------------------------------------------------------
TracerResourceType::TracerResourceType(Kind kind)
	: ChartTreeItem()
	, m_kind(kind)
{}

TracerResourceType::~TracerResourceType()
{
	int count = m_paramInfoList.size();
	for (int i = 0; i < count; i++)
	{
		delete m_paramInfoList.at(i);
	}
}

CREF(QString) TracerResourceType::getName() const
{
	return m_name;
}

void TracerResourceType::setName(CREF(QString) name)
{
	m_name = name;
}

TracerResourceType::Kind TracerResourceType::getKind() const
{
	return m_kind;
}

int TracerResourceType::addParamInfo(TracerResourceParamInfo* const value)
{
	m_paramInfoList.push_back(value);
	return m_paramInfoList.size() - 1;
}

TracerResourceParamInfo* TracerResourceType::getParamInfo(unsigned int index) const
{
	if (index >= m_paramInfoList.size() || index < 0)
		return NULL;
	return m_paramInfoList.at(index);
}

int TracerResourceType::getParamsCount() const
{
	return m_paramInfoList.size();
}
