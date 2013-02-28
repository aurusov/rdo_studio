/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_resource_type.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_resource_type.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- ParamInfo
// --------------------------------------------------------------------------------
ParamInfo::ParamInfo(const ParamType type)
	: m_paramType(type)
	, m_enumValueList(NULL)
{
	if (m_paramType == PT_ENUMERATIVE || m_paramType == PT_BOOL || m_paramType == PT_STRING)
	{
		m_enumValueList = new EnumValueList();
	}
}

ParamInfo::~ParamInfo()
{
	if (m_enumValueList)
	{
		delete m_enumValueList;
	}
}

CREF(QString) ParamInfo::getName() const
{
	return m_name;
}

void ParamInfo::setName(CREF(QString) name)
{
	m_name = name;
}

ParamInfo::ParamType ParamInfo::getParamType() const
{
	return m_paramType;
}

int ParamInfo::addEnumValue(CREF(tstring) value)
{
	if (!m_enumValueList)
		return -1;
	m_enumValueList->push_back(value);
	return m_enumValueList->size() - 1;
}

int ParamInfo::addStringValue(CREF(tstring) value)
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

tstring ParamInfo::getEnumValue(unsigned int index) const
{
	if (!m_enumValueList)
		return nullStr;
	if (index >= m_enumValueList->size() || index < 0)
		return nullStr;
	return m_enumValueList->at(index);
}

int ParamInfo::getEnumCount() const
{
	return m_enumValueList ? m_enumValueList->size() : 0;
}

// --------------------------------------------------------------------------------
// -------------------- ResourceType
// --------------------------------------------------------------------------------
ResourceType::ResourceType(Kind kind)
	: ChartTreeItem()
	, m_kind(kind)
{}

ResourceType::~ResourceType()
{
	int count = m_paramInfoList.size();
	for (int i = 0; i < count; i++)
	{
		delete m_paramInfoList.at(i);
	}
}

CREF(QString) ResourceType::getName() const
{
	return m_name;
}

void ResourceType::setName(CREF(QString) name)
{
	m_name = name;
}

ResourceType::Kind ResourceType::getKind() const
{
	return m_kind;
}

int ResourceType::addParamInfo(ParamInfo* const value)
{
	m_paramInfoList.push_back(value);
	return m_paramInfoList.size() - 1;
}

ParamInfo* ResourceType::getParamInfo(unsigned int index) const
{
	if (index >= m_paramInfoList.size() || index < 0)
		return NULL;
	return m_paramInfoList.at(index);
}

int ResourceType::getParamsCount() const
{
	return m_paramInfoList.size();
}
