#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_keyboard_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOEvent;
class RDORule;
class RDOKeyboard;

PREDECLARE_POINTER(RDOPattern);
class RDOPattern
    : public virtual rdo::counter_reference
    , public RDORuntimeObject
    , public RDOTraceableObject
{
DECLARE_FACTORY(RDOPattern);

public:
    void addPreSelectRelRes(const LPRDOCalc& pCalc);

protected:
    RDOPattern(bool trace);
    virtual ~RDOPattern();

    typedef  std::vector<LPRDOCalc>                   CalcList;
    typedef  std::vector<RDOResource::ConvertStatus>  ConvertStatusList;

    CalcList m_preSelectRelRes;

    void preSelectRelRes(const LPRDORuntime& pRuntime);

    void runCalcs(CalcList& calcList, const LPRDORuntime& pRuntime);
    bool runCalcsBool(CalcList& calcList, const LPRDORuntime& pRuntime);
};

class RDOPatternEvent: public RDOPattern
{
DECLARE_FACTORY(RDOPatternEvent);
friend class RDOEvent;

public:
    void addConvertorStatus(RDOResource::ConvertStatus status);

    void addConvertorCalc  (const LPRDOCalc& pCalc);
    void addEraseCalc      (const LPRDOCalc& pCalc);

    void convertEvent(const LPRDORuntime& pRuntime);
    void convertErase(const LPRDORuntime& pRuntime);

    double getNextTimeInterval(const LPRDORuntime& pRuntime);

    LPIEvent createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& oprName);

private:
    RDOPatternEvent(bool trace);
    virtual ~RDOPatternEvent();

    LPRDOCalc         m_timeCalc;
    CalcList          m_convertor;
    ConvertStatusList m_convertorStatus;
    CalcList          m_erase;
};
DECLARE_POINTER(RDOPatternEvent);

class RDOPatternRule: public RDOPattern
{
DECLARE_FACTORY(RDOPatternRule);
friend class RDORule;

public:
    void addConvertorStatus(RDOResource::ConvertStatus status);

    void addChoiceFromCalc (const LPRDOCalc& pCalc);
    void addConvertorCalc  (const LPRDOCalc& pCalc);
    void addEraseCalc      (const LPRDOCalc& pCalc);

    void convertRule (const LPRDORuntime& pRuntime);
    void convertErase(const LPRDORuntime& pRuntime);

    bool choiceFrom (const LPRDORuntime& pRuntime);

    LPIRule createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName);
    LPIRule createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& condition, const std::string& _oprName);

private:
    RDOPatternRule(bool trace);
    virtual ~RDOPatternRule();

    CalcList          m_choiceFrom;
    CalcList          m_convertor;
    ConvertStatusList m_convertorStatus;
    CalcList          m_erase;
};
DECLARE_POINTER(RDOPatternRule);

class RDOPatternOperation: public RDOPattern
{
DECLARE_FACTORY(RDOPatternOperation);
friend class RDOOperation;

public:
    void addConvertorBeginStatus(RDOResource::ConvertStatus status);
    void addConvertorEndStatus  (RDOResource::ConvertStatus status);

    void addChoiceFromCalc      (const LPRDOCalc& pCalc);
    void addConvertorBeginCalc  (const LPRDOCalc& pCalc);
    void addConvertorEndCalc    (const LPRDOCalc& pCalc);
    void addEraseBeginCalc      (const LPRDOCalc& pCalc);
    void addEraseEndCalc        (const LPRDOCalc& pCalc);
    void setTime                (const LPRDOCalc& pCalc);

    void convertBegin           (const LPRDORuntime& pRuntime);
    void convertEnd             (const LPRDORuntime& pRuntime);
    void convertBeginErase      (const LPRDORuntime& pRuntime);
    void convertEndErase        (const LPRDORuntime& pRuntime);

    bool choiceFrom(const LPRDORuntime& pRuntime);
    double getNextTimeInterval(const LPRDORuntime& pRuntime);

    LPIOperation createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName);
    LPIOperation createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& condition, const std::string& _oprName);

protected:
    RDOPatternOperation(bool trace);
    virtual ~RDOPatternOperation();

private:
    LPRDOCalc         m_timeCalc;
    CalcList          m_choiceFrom;

    CalcList          m_convertorBegin;
    ConvertStatusList m_convertorBeginStatus;
    CalcList          m_eraseBegin;

    CalcList          m_convertorEnd;
    ConvertStatusList m_convertorEndStatus;
    CalcList          m_eraseEnd;
};
DECLARE_POINTER(RDOPatternOperation);

class RDOPatternKeyboard: public RDOPatternOperation
{
DECLARE_FACTORY(RDOPatternKeyboard);
public:
    LPIKeyboard createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const std::string& _oprName);
    LPIKeyboard createActivity(LPIBaseOperationContainer pLogic, const LPRDORuntime& pRuntime, const LPRDOCalc& condition, const std::string& _oprName);

private:
    RDOPatternKeyboard(bool trace);
    virtual ~RDOPatternKeyboard();
};
DECLARE_POINTER(RDOPatternKeyboard);

CLOSE_RDO_RUNTIME_NAMESPACE
