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
#include "app/rdo_studio/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerResParam
// --------------------------------------------------------------------------------
TracerResParam::TracerResParam(CREF(LPTracerResource) pResource)
	: TracerSerie(RDOST_RESPARAM)
	, m_pResource(pResource)
{}

TracerResParam::~TracerResParam()
{}

TracerResParamInfo* TracerResParam::getParamInfo() const
{
	LPTracerResParam pThis(const_cast<TracerResParam*>(this));
	int index = m_pResource->getParamIndex(pThis);
	return index != -1
		? m_pResource->getType()->getParamInfo(index)
		: NULL;
}

void TracerResParam::getCaptions(std::vector<tstring> &captions, const int val_count) const
{
	switch (getParamInfo()->getParamType())
	{
	case RDOPT_INTEGER:
		TracerSerie::getCaptionsInt(captions, val_count);
		break;

	case RDOPT_REAL:
		TracerSerie::getCaptionsDouble(captions, val_count);
		break;

	case RDOPT_ENUMERATIVE:
	case RDOPT_BOOL:
	case RDOPT_STRING:
	{
		TracerSerie::getCaptions(captions, val_count);
		int delta = getParamInfo()->getEnumCount();
		minValue = 0;
		maxValue = delta - 1;
		int real_val_count = val_count;
		if (delta > real_val_count)
		{
			while ((int)((delta - 1) / (real_val_count - 1)) != ((double)(delta - 1) / (real_val_count - 1)))
				real_val_count--;
		}
		else
		{
			real_val_count = delta;
		}
		int valo = (int)minValue;
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
TracerResource::TracerResource(CREF(LPTracerResType) pResType, CREF(tstring) name)
	: ChartTreeItem()
	, erased(false)
	, m_pResType(pResType)
	, Name(name)
	, id(0)
{
	int count = m_pResType->getParamsCount();
	for (int i = 0; i < count; i++)
	{
		addParam(rdo::Factory<TracerResParam>::create(this));
	}
}

TracerResource::~TracerResource()
{}

void TracerResource::addParam(CREF(LPTracerResParam) pParam)
{
	ASSERT(pParam);
	pParam->setTitle(Name + "." + m_pResType->getParamInfo(m_paramList.size())->Name);
	m_paramList.push_back(pParam);
}

LPTracerResParam TracerResource::getParam(unsigned int index) const
{
	if (index >= m_paramList.size() || index < 0)
		return LPTracerResParam();
	return m_paramList.at(index);
}

int TracerResource::getParamIndex(CREF(LPTracerResParam) pParam) const
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

void TracerResource::setParams(tstring& line, TracerTimeNow* const time, const int eventIndex, const rbool erasing)
{
	int count = m_paramList.size();
	for (int i = 0; i < count; i++)
	{
		TracerValue* prevval;
		m_paramList.at(i)->getLastValue(prevval);
		tstring nextValue = g_pTracer->getNextValue(line);
		double newval;
		if (erasing)
		{
			newval = prevval->value;
		}
		else
		{
			switch (m_pResType->getParamInfo(i)->getParamType())
			{
			case RDOPT_BOOL:
				newval = nextValue == _T("true") ? 1.0 : 0.0;
				break;

			case RDOPT_STRING:
				newval = m_pResType->getParamInfo(i)->addStringValue(nextValue);
				break;

			default:
				newval = atof(nextValue.c_str());
				break;
			}
		}

		if (true /*!prevval || erasing || prevval->value != newval*/)
		{
			TracerValue* newvalue = new TracerValue(time, eventIndex);
			newvalue->value = newval;
			m_paramList.at(i)->addValue(newvalue);
		}
	}
}

void TracerResource::setErased(const rbool value)
{
	if (erased != value)
	{
		erased = value;
	}
}
