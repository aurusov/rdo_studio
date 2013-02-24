/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerresource.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/rdotracerresource.h"
#include "app/rdo_studio/src/tracer/rdotracerrestype.h"
#include "app/rdo_studio/src/tracer/rdotracervalues.h"
#include "app/rdo_studio/src/tracer/rdotracer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- TracerResourceParam
// --------------------------------------------------------------------------------
TracerResourceParam::TracerResourceParam(CREF(LPTracerResource) pResource)
	: TracerSerie(SK_PARAM)
	, m_pResource(pResource)
{}

TracerResourceParam::~TracerResourceParam()
{}

CREF(LPTracerResource) TracerResourceParam::getResource() const
{
	return m_pResource;
}

TracerResourceParamInfo* TracerResourceParam::getParamInfo() const
{
	LPTracerResourceParam pThis(const_cast<TracerResourceParam*>(this));
	int index = m_pResource->getParamIndex(pThis);
	return index != -1
		? m_pResource->getType()->getParamInfo(index)
		: NULL;
}

void TracerResourceParam::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	switch (getParamInfo()->getParamType())
	{
	case TracerResourceParamInfo::PT_INTEGER:
		TracerSerie::getCaptionsInt(captions, valueCount);
		break;

	case TracerResourceParamInfo::PT_REAL:
		TracerSerie::getCaptionsDouble(captions, valueCount);
		break;

	case TracerResourceParamInfo::PT_ENUMERATIVE:
	case TracerResourceParamInfo::PT_BOOL:
	case TracerResourceParamInfo::PT_STRING:
	{
		TracerSerie::getCaptions(captions, valueCount);
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
// -------------------- TracerResource
// --------------------------------------------------------------------------------
TracerResource::TracerResource(CREF(LPTracerResourceType) pResType, CREF(QString) name, int id)
	: ChartTreeItem()
	, m_erased(false)
	, m_pResourceType(pResType)
	, m_name(name)
	, m_id(id)
{
	int count = m_pResourceType->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		addParam(rdo::Factory<TracerResourceParam>::create(this));
	}
}

TracerResource::~TracerResource()
{}

CREF(QString) TracerResource::getName() const
{
	return m_name;
}

void TracerResource::setName(CREF(QString) name)
{
	m_name = name;
}

int TracerResource::getID() const
{
	return m_id;
}

CREF(LPTracerResourceType) TracerResource::getType() const
{
	return m_pResourceType;
}

void TracerResource::addParam(CREF(LPTracerResourceParam) pParam)
{
	ASSERT(pParam);
	pParam->setTitle(m_name + "." + m_pResourceType->getParamInfo(m_paramList.size())->getName());
	m_paramList.push_back(pParam);
}

LPTracerResourceParam TracerResource::getParam(unsigned int index) const
{
	if (index >= m_paramList.size() || index < 0)
		return LPTracerResourceParam();
	return m_paramList.at(index);
}

int TracerResource::getParamIndex(CREF(LPTracerResourceParam) pParam) const
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

void TracerResource::setParams(tstring& line, TracerTimeNow* const pTime, const int eventIndex, const rbool erasing)
{
	int count = m_paramList.size();
	for (int i = 0; i < count; i++)
	{
		TracerValue* pPrevValue;
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
			case TracerResourceParamInfo::PT_BOOL:
				newValue = nextValue == _T("true") ? 1.0 : 0.0;
				break;

			case TracerResourceParamInfo::PT_STRING:
				newValue = m_pResourceType->getParamInfo(i)->addStringValue(nextValue);
				break;

			default:
				newValue = atof(nextValue.c_str());
				break;
			}
		}

		TracerValue* pNewValue = new TracerValue(pTime, eventIndex, newValue);
		m_paramList.at(i)->addValue(pNewValue);
	}
}

void TracerResource::setErased(const rbool value)
{
	if (m_erased != value)
	{
		m_erased = value;
	}
}

rbool TracerResource::isErased() const
{
	return m_erased;
}
