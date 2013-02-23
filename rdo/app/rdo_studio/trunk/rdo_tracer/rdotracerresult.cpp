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
TracerResult::TracerResult(const TracerResultKind kind)
	: TracerSerie(RDOST_RESULT),
	  resultKind(kind),
	  id(0)
{}

TracerResult::~TracerResult()
{}

CREF(QString) TracerResult::getName() const
{
	return m_name;
}

void TracerResult::setName(CREF(QString) name)
{
	m_name = name;
	if (title.isEmpty())
	{
		title = m_name;
	}
}

void TracerResult::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	switch (resultKind)
	{
	case RDORK_WATCHQUANT:
	{
		TracerSerie::getCaptionsInt(captions, valueCount);
		break;
	}
	case RDORK_WATCHSTATE:
	{
		TracerSerie::getCaptionsBool(captions, valueCount);
		break;
	}
	case RDORK_WATCHPAR:
	case RDORK_WATCHVALUE:
	{
		TracerSerie::getCaptionsDouble(captions, valueCount);
		break;
	}
	}
}

void TracerResult::setValue(tstring& line, TracerTimeNow* const time, const int eventIndex)
{
	TracerValue* newvalue = new TracerValue(time, eventIndex);
	double newval;
	rdo::trim(line);
	if (resultKind != RDORK_WATCHSTATE)
	{
		newval = atof(line.c_str());
	}
	else
	{
		newval = (line == "TRUE") ? 1 : 0;
	}
	newvalue->value = newval;
	addValue(newvalue);
}
