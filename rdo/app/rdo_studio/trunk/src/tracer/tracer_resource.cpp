/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_resource.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_resource.h"
#include "app/rdo_studio/src/tracer/tracer_resource_type.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- Param
// --------------------------------------------------------------------------------
Param::Param(CREF(LPResource) pResource)
	: Serie(SK_PARAM)
	, m_pResource(pResource)
{}

Param::~Param()
{}

CREF(LPResource) Param::getResource() const
{
	return m_pResource;
}

ParamInfo* Param::getParamInfo() const
{
	LPParam pThis(const_cast<Param*>(this));
	int index = m_pResource->getParamIndex(pThis);
	return index != -1
		? m_pResource->getType()->getParamInfo(index)
		: NULL;
}

void Param::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	switch (getParamInfo()->getParamType())
	{
	case ParamInfo::PT_INTEGER:
		Serie::getCaptionsInt(captions, valueCount);
		break;

	case ParamInfo::PT_REAL:
		Serie::getCaptionsDouble(captions, valueCount);
		break;

	case ParamInfo::PT_ENUMERATIVE:
	case ParamInfo::PT_BOOL:
	case ParamInfo::PT_STRING:
	{
		Serie::getCaptions(captions, valueCount);
		int delta = getParamInfo()->getEnumCount();
		m_minValue = 0;
		m_maxValue = delta - 1;
		int real_val_count = valueCount;
		if (delta > real_val_count)
		{
			while ((int)((delta - 1) / (real_val_count - 1)) != ((double)(delta - 1) / (real_val_count - 1)))
				real_val_count--;
		}
		else
		{
			real_val_count = delta;
		}
		int valo = (int)m_minValue;
		int valoffset = real_val_count != 1 ? (delta - 1) / (real_val_count - 1) : 0;
		for (int i = 0; i < real_val_count; i++)
		{
			captions.push_back(getParamInfo()->getEnumValue(valo));
			valo += valoffset;
		}
		break;
	}
	}
}

// --------------------------------------------------------------------------------
// -------------------- Resource
// --------------------------------------------------------------------------------
Resource::Resource(CREF(LPResourceType) pResType, CREF(QString) name, int id)
	: ChartTreeItem()
	, m_erased(false)
	, m_pResourceType(pResType)
	, m_name(name)
	, m_id(id)
{
	int count = m_pResourceType->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		addParam(rdo::Factory<Param>::create(this));
	}
}

Resource::~Resource()
{}

CREF(QString) Resource::getName() const
{
	return m_name;
}

void Resource::setName(CREF(QString) name)
{
	m_name = name;
}

int Resource::getID() const
{
	return m_id;
}

CREF(LPResourceType) Resource::getType() const
{
	return m_pResourceType;
}

void Resource::addParam(CREF(LPParam) pParam)
{
	ASSERT(pParam);
	pParam->setTitle(m_name + "." + m_pResourceType->getParamInfo(m_paramList.size())->getName());
	m_paramList.push_back(pParam);
}

LPParam Resource::getParam(unsigned int index) const
{
	if (index >= m_paramList.size() || index < 0)
		return LPParam();
	return m_paramList.at(index);
}

int Resource::getParamIndex(CREF(LPParam) pParam) const
{
	int count = m_paramList.size();
	for (int i = 0; i < count; i++)
	{
		if (m_paramList.at(i) == pParam)
		{
			return i;
		}
	}
	return -1;
}

void Resource::setParams(tstring& line, Time* const pTime, const int eventIndex, const rbool erasing)
{
	int count = m_paramList.size();
	for (int i = 0; i < count; i++)
	{
		Value* pPrevValue;
		m_paramList.at(i)->getLastValue(pPrevValue);
		tstring nextValue = g_pTracer->getNextValue(line);
		double newValue;
		if (erasing)
		{
			newValue = pPrevValue->getValue();
		}
		else
		{
			switch (m_pResourceType->getParamInfo(i)->getParamType())
			{
			case ParamInfo::PT_BOOL:
				newValue = nextValue == _T("true") ? 1.0 : 0.0;
				break;

			case ParamInfo::PT_STRING:
				newValue = m_pResourceType->getParamInfo(i)->addStringValue(nextValue);
				break;

			default:
				newValue = atof(nextValue.c_str());
				break;
			}
		}

		Value* pNewValue = new Value(pTime, eventIndex, newValue);
		m_paramList.at(i)->addValue(pNewValue);
	}
}

void Resource::setErased(const rbool value)
{
	if (m_erased != value)
	{
		m_erased = value;
	}
}

rbool Resource::isErased() const
{
	return m_erased;
}
