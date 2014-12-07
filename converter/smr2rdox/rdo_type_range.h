#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

class Converter;

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRangeRange
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOTypeRangeRange);
class RDOTypeRangeRange
    : public rdo::counter_reference
    , public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOTypeRangeRange);
public:
    void              checkRange()                         const;
    void              checkValue(const LPRDOValue& pValue) const;
    const LPRDOValue& getMin    () const;
    const LPRDOValue& getMax    () const;

private:
    RDOTypeRangeRange(const LPRDOValue& pMinValue, const LPRDOValue& pMaxValue, const RDOParserSrcInfo& src_info);
    virtual ~RDOTypeRangeRange();

    LPRDOValue m_pMinValue;
    LPRDOValue m_pMaxValue;
};
DECLARE_POINTER(RDOTypeRangeRange);

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRange
// --------------------------------------------------------------------------------
template<class T>
class RDOTypeRange: public T
{
DECLARE_FACTORY(RDOTypeRange<T>);
public:
    const LPRDOTypeRangeRange& range() const;

private:
    typedef T parent_type;

    RDOTypeRange(const LPRDOTypeRangeRange& range);
    virtual ~RDOTypeRange();

    LPRDOTypeRangeRange m_pRange;

    DECLARE_ITypeConverter;
};
typedef RDOTypeRange<RDOType__int>  RDOTypeIntRange;
typedef RDOTypeRange<RDOType__real> RDOTypeRealRange;
DECLARE_POINTER(RDOTypeIntRange);
DECLARE_POINTER(RDOTypeRealRange);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#include "converter/smr2rdox/rdo_type_range-inl.h"
