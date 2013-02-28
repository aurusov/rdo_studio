/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_result.cpp
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_result.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

Result::Result(CREF(QString) name, Kind kind, int id)
	: Serie(SK_RESULT)
	, m_name(name)
	, m_kind(kind)
	, m_id(id)
{
	setTitle(m_name);
}

Result::~Result()
{}

CREF(QString) Result::getName() const
{
	return m_name;
}

Result::Kind Result::getKind() const
{
	return m_kind;
}

int Result::getID() const
{
	return m_id;
}

void Result::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	switch (m_kind)
	{
	case RK_WATCHQUANT:
	{
		Serie::getCaptionsInt(captions, valueCount);
		break;
	}
	case RK_WATCHSTATE:
	{
		Serie::getCaptionsBool(captions, valueCount);
		break;
	}
	case RK_WATCHPAR:
	case RK_WATCHVALUE:
	{
		Serie::getCaptionsDouble(captions, valueCount);
		break;
	}
	}
}

void Result::setValue(tstring& line, Time* const pTime, const int eventIndex)
{
	double newValue;
	boost::algorithm::trim(line);
	if (m_kind != RK_WATCHSTATE)
	{
		newValue = atof(line.c_str());
	}
	else
	{
		newValue = (line == "TRUE") ? 1 : 0;
	}
	Value* pNewValue = new Value(pTime, eventIndex, newValue);
	addValue(pNewValue);
}
