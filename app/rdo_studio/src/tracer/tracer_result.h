#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class Result: public Serie
{
DECLARE_FACTORY(Result)
public:
    enum class Kind
    {
        UNDEFINED  = ~0,
        WATCHPAR   = 0,
        WATCHSTATE,
        WATCHQUANT,
        WATCHVALUE
    };

    const QString& getName() const;
    Kind           getKind() const;
    int            getID  () const;

    virtual void getCaptions(std::vector<std::string>& captions, const int valueCount) const;
    void setValue(std::string& line, Time* const time, const int eventIndex);

private:
    Result(const QString& name, Kind kind, int id);
    virtual ~Result();

    QString  m_name;
    Kind     m_kind;
    int      m_id;
};

typedef  rdo::intrusive_ptr<Result>  LPResult;

}}} // namespace rdo::gui::tracer
