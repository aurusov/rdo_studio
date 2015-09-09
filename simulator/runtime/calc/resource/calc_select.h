#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_group.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFunCalcSelect);
class RDOFunCalcSelect: public RDOFunCalcGroup
{
DECLARE_FACTORY(RDOFunCalcSelect)
public:
    mutable std::list<LPRDOResource> res_list;
    void prepare(const LPRDORuntime& pRuntime);

    const LPIResourceType& getResType();

private:
    RDOFunCalcSelect(const LPIResourceType& pResType, int nResType, const LPRDOCalc& pCondition);

    LPIResourceType m_pResType;

    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOFunCalcSelectBase);
class RDOFunCalcSelectBase: public RDOFunCalc
{
protected:
    RDOFunCalcSelectBase(const LPRDOFunCalcSelect& pSelect, const LPRDOCalc& pCondition);

    LPRDOFunCalcSelect m_pSelect;
    LPRDOCalc          m_pCondition;
};

#define DEFINE_CALC_SELECT_GROUP(CalcName)                    \
PREDECLARE_POINTER(RDOFunCalcSelect##CalcName);               \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{                                                             \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName)                   \
private:                                                      \
    RDOFunCalcSelect##CalcName(const LPRDOFunCalcSelect& pSelect, const LPRDOCalc& pCondition) \
        : RDOFunCalcSelectBase(pSelect, pCondition)           \
    {}                                                        \
    DECLARE_ICalc;                                            \
};

#define DEFINE_CALC_SELECT_METHOD(CalcName)                      \
PREDECLARE_POINTER(RDOFunCalcSelect##CalcName);                  \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase    \
{                                                                \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName)                      \
private:                                                         \
    RDOFunCalcSelect##CalcName(const LPRDOFunCalcSelect& pSelect) \
        : RDOFunCalcSelectBase(pSelect, NULL)                    \
    {}                                                           \
    DECLARE_ICalc;                                               \
};

DEFINE_CALC_SELECT_GROUP(Exist);

DEFINE_CALC_SELECT_GROUP (NotExist );

DEFINE_CALC_SELECT_GROUP (ForAll   );

DEFINE_CALC_SELECT_GROUP (NotForAll);

DEFINE_CALC_SELECT_METHOD(Empty    );

DEFINE_CALC_SELECT_METHOD(Size     );

DEFINE_CALC_SELECT_METHOD(Array    );

CLOSE_RDO_RUNTIME_NAMESPACE
