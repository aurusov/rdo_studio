#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOSelectResourceCalc);
class RDOSelectResourceCalc: public RDOCalc
{
public:
    enum class Type
    {
        EMPTY = 0,
        FIRST,
        WITH_MIN,
        WITH_MAX
    };

    typedef std::size_t ResourceID;
    typedef std::vector<ResourceID> ResourceIDList;
    typedef std::vector<ResourceIDList> ResourceIDTable;

protected:
    RDOSelectResourceCalc(ResourceID relResID, const LPRDOCalc& pCalcChoiceFrom, const LPRDOCalc& pCalcOrder, Type orderType = Type::EMPTY);

    ResourceID  m_relResID;
    LPRDOCalc   m_pCalcChoiceFrom;
    LPRDOCalc   m_pCalcOrder;
    Type        m_orderType;
};

PREDECLARE_POINTER(RDOSelectResourceNonExistCalc);
class RDOSelectResourceNonExistCalc: public RDOSelectResourceCalc
{
DECLARE_FACTORY(RDOSelectResourceNonExistCalc)
private:
    RDOSelectResourceNonExistCalc(ResourceID relResID);
    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOSelectResourceDirectCalc);
class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
DECLARE_FACTORY(RDOSelectResourceDirectCalc)
protected:
    RDOSelectResourceDirectCalc(ResourceID relResID, ResourceID resID, const LPRDOCalc& pCalcChoiceFrom = NULL, const LPRDOCalc& pCalcOrder = NULL, Type orderType = Type::EMPTY);

    ResourceID m_resID;

    virtual bool compare(const LPRDOCalc& pCalc) const;

    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOSelectResourceByTypeCalc);
class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
DECLARE_FACTORY(RDOSelectResourceByTypeCalc)
protected:
    RDOSelectResourceByTypeCalc(ResourceID relResID, ResourceID resTypeID, const LPRDOCalc& pChoiceCalc = NULL, const LPRDOCalc& pOrderCalc = NULL, Type orderType = Type::EMPTY);

    ResourceID m_resTypeID;

    DECLARE_ICalc;
};

PREDECLARE_OBJECT_INTERFACE(IRDOSelectResourceCommon)
struct IRDOSelectResourceCommon: public rdo::RefCounter<IRDOSelectResourceCommon>
{
DECLARE_FACTORY(IRDOSelectResourceCommon)
public:
    virtual void getPossibleNumbers(const LPRDORuntime& pRuntime,
                                    RDOSelectResourceCalc::ResourceIDList& resourceIDList) const = 0;
    virtual bool callChoice(const LPRDORuntime& pRuntime) const = 0;

protected:
    IRDOSelectResourceCommon();
    virtual ~IRDOSelectResourceCommon();
};

PREDECLARE_POINTER(RDOSelectResourceCommonCalc);
class RDOSelectResourceCommonCalc: public RDOCalc
{
DECLARE_FACTORY(RDOSelectResourceCommonCalc)
private:
    typedef  std::vector<LPIRDOSelectResourceCommon>  SelectResourceCommonList;
    typedef  RDOSelectResourceCalc::ResourceIDList    ResourceIDList;
    typedef  RDOSelectResourceCalc::ResourceIDTable   ResourceIDTable;

    RDOSelectResourceCommonCalc(const SelectResourceCommonList& resSelectorList, bool useCommonWithMax, const LPRDOCalc& pCalcChoiceFrom);

    LPRDOCalc m_pCalcChoiceFrom;
    SelectResourceCommonList m_resSelectorList;
    bool m_useCommonWithMax;

    void getBest(ResourceIDTable& allNumbs, std::size_t level, ResourceIDList& res, RDOValue& bestVal, const LPRDORuntime& pRuntime, bool& hasBest) const;
    bool getFirst(ResourceIDTable& allNumbs, std::size_t level, const LPRDORuntime& pRuntime) const;

    DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOSelectResourceDirectCommonCalc);
class RDOSelectResourceDirectCommonCalc
    : public RDOSelectResourceDirectCalc
    , public IRDOSelectResourceCommon
{
DECLARE_FACTORY(RDOSelectResourceDirectCommonCalc)
public:
    virtual void getPossibleNumbers(const LPRDORuntime& pRuntime, ResourceIDList& resourceIDList) const;
    virtual bool callChoice (const LPRDORuntime& pRuntime) const;

private:
    RDOSelectResourceDirectCommonCalc(ResourceID relResID, ResourceID resID, const LPRDOCalc& pCalcChoiceFrom = NULL, const LPRDOCalc& pCalcOrder = NULL, Type orderType = Type::EMPTY);
    virtual ~RDOSelectResourceDirectCommonCalc();
};

PREDECLARE_POINTER(RDOSelectResourceByTypeCommonCalc);
class RDOSelectResourceByTypeCommonCalc
    : public RDOSelectResourceByTypeCalc
    , public IRDOSelectResourceCommon
{
DECLARE_FACTORY(RDOSelectResourceByTypeCommonCalc)
public:
    virtual void getPossibleNumbers(const LPRDORuntime& pRuntime, ResourceIDList& resourceIDList) const;
    virtual bool callChoice(const LPRDORuntime& pRuntime) const;

private:
    RDOSelectResourceByTypeCommonCalc(ResourceID relResID, ResourceID resTypeID, const LPRDOCalc& pChoiceCalc = NULL, const LPRDOCalc& pOrderCalc = NULL, Type orderType = Type::EMPTY);
    virtual ~RDOSelectResourceByTypeCommonCalc();
};

CLOSE_RDO_RUNTIME_NAMESPACE
