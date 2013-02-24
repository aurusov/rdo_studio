/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerresult.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerresult.h"
#include "app/rdo_studio/rdo_tracer/rdotracervalues.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerResult
// --------------------------------------------------------------------------------
TracerResult::TracerResult(CREF(QString) name, Kind kind, int id)
	: TracerSerie(SK_RESULT)
	, m_name(name)
	, m_kind(kind)
	, m_id(id)
{
	if (title.isEmpty())
	{
		title = m_name;
	}
}

TracerResult::~TracerResult()
{}

CREF(QString) TracerResult::getName() const
{
	return m_name;
}

TracerResult::Kind TracerResult::getKind() const
{
	return m_kind;
}

int TracerResult::getID() const
{
	return m_id;
}

void TracerResult::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	switch (m_kind)
	{
	case RK_WATCHQUANT:
	{
		TracerSerie::getCaptionsInt(captions, valueCount);
		break;
	}
	case RK_WATCHSTATE:
	{
		TracerSerie::getCaptionsBool(captions, valueCount);
		break;
	}
	case RK_WATCHPAR:
	case RK_WATCHVALUE:
	{
		TracerSerie::getCaptionsDouble(captions, valueCount);
		break;
	}
	}
}

void TracerResult::setValue(tstring& line, TracerTimeNow* const pTime, const int eventIndex)
{
	TracerValue* pNewValue = new TracerValue(pTime, eventIndex);
	double newValue;
	rdo::trim(line);
	if (m_kind != RK_WATCHSTATE)
	{
		newValue = atof(line.c_str());
	}
	else
	{
		newValue = (line == "TRUE") ? 1 : 0;
	}
	pNewValue->value = newValue;
	addValue(pNewValue);
}
