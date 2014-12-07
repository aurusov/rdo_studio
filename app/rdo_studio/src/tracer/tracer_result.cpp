// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_result.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

Result::Result(const QString& name, Kind kind, int id)
    : Serie(SK_RESULT)
    , m_name(name)
    , m_kind(kind)
    , m_id(id)
{
    setTitle(m_name);
}

Result::~Result()
{}

const QString& Result::getName() const
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

void Result::getCaptions(std::vector<std::string>& captions, const int valueCount) const
{
    switch (m_kind)
    {
    case RK_WATCHQUANT:
        Serie::getCaptionsInt(captions, valueCount);
        break;

    case RK_WATCHSTATE:
        Serie::getCaptionsBool(captions, valueCount);
        break;

    case RK_WATCHPAR:
    case RK_WATCHVALUE:
        Serie::getCaptionsDouble(captions, valueCount);
        break;

    default:
        NEVER_REACH_HERE;
        break;
    }
}

void Result::setValue(std::string& line, Time* const pTime, const int eventIndex)
{
    double newValue;
    boost::algorithm::trim(line);
    if (m_kind != RK_WATCHSTATE)
    {
        newValue = boost::lexical_cast<double>(line);
    }
    else
    {
        newValue = (line == "TRUE") ? 1 : 0;
    }
    Value* pNewValue = new Value(pTime, eventIndex, newValue);
    addValue(pNewValue);
}
