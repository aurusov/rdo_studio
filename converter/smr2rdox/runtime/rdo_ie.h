#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPatternIrregEvent
// --------------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPattern
{
friend class RDOIrregEvent;

public:
    RDOPatternIrregEvent(bool trace);

    void addConvertorCalc  (const LPRDOCalc& pCalc           ) { m_convertor.push_back(pCalc);        }
    void addConvertorStatus(RDOResource::ConvertStatus status) { m_convertorStatus.push_back(status); }
    void addEraseCalc      (const LPRDOCalc& pCalc           ) { m_erase.push_back(pCalc);            }
    void setTime           (const LPRDOCalc& timeCalc        ) { m_timeCalc = timeCalc;                  }

    void convertEvent(const LPRDORuntime& pRuntime)
    {
        preSelectRelRes(pRuntime);
        runCalcs(m_convertor, pRuntime);
    }
    void convertErase(const LPRDORuntime& pRuntime)
    {
        runCalcs(m_erase, pRuntime);
    }

    double getNextTimeInterval(const LPRDORuntime& pRuntime);

private:
    LPRDOCalc         m_timeCalc;
    CalcList          m_convertor;
    ConvertStatusList m_convertorStatus;
    CalcList          m_erase;
};

// --------------------------------------------------------------------------------
// -------------------- RDOIrregEvent
// --------------------------------------------------------------------------------
class RDOIrregEvent: public IBaseOperation, public RDOActivityPattern<RDOPatternIrregEvent>
{
DECLARE_FACTORY(RDOIrregEvent);
typedef RDOActivityPattern<RDOPatternIrregEvent> pattern_type;
friend class RDOTrace;

private:
    RDOIrregEvent(RDOPatternIrregEvent* pPattern, bool trace, const std::string& name);

    double  m_time;

    void   convertEvent       (const LPRDORuntime& pRuntime);
    double getNextTimeInterval(const LPRDORuntime& pRuntime);

    virtual void onBeforeIrregularEvent(const LPRDORuntime& pRuntime);
    virtual void onAfterIrregularEvent (const LPRDORuntime& pRuntime);

    DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE
