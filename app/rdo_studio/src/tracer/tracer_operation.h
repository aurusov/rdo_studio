#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
#include "app/rdo_studio/src/tracer/tracer_pattern.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- OperationBase
// --------------------------------------------------------------------------------
class OperationBase: public Serie
{
DECLARE_FACTORY(OperationBase)
public:
    const QString& getName() const;

    LPPattern getPattern() const;

    virtual void getCaptions(std::vector<std::string>& captions, const int valueCount) const;
    virtual void monitorTime(Time* const pTime, const int eventIndex);

protected:
    OperationBase(const LPPattern& pPattern, const QString& name);
    virtual ~OperationBase();

    void incOperationsCount(Time* const pTime, const int eventIndex);

private:
    LPPattern  m_pPattern;
    QString    m_name;
};

typedef  rdo::intrusive_ptr<OperationBase>  LPOperationBase;

// --------------------------------------------------------------------------------
// -------------------- Operation
// --------------------------------------------------------------------------------
class Operation: public OperationBase
{
DECLARE_FACTORY(Operation)
public:
    void start(Time* const pTime, const int eventIndex);
    void accomplish(Time* const pTime, const int eventIndex);

private:
    Operation(const LPPattern& pPattern, const QString& name);
    virtual ~Operation();
};

typedef  rdo::intrusive_ptr<Operation>  LPOperation;

// --------------------------------------------------------------------------------
// -------------------- Event
// --------------------------------------------------------------------------------
class Event: public OperationBase
{
DECLARE_FACTORY(Event)
public:
    void occurs(Time* const pTime, const int eventIndex);
    virtual void monitorTime(Time* const pTime, const int eventIndex);

private:
    Event(const LPPattern& pPattern, const QString& name);
    virtual ~Event();
};

typedef  rdo::intrusive_ptr<Event>  LPEvent;

}}} // namespace rdo::gui::tracer
