// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_range.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRange
// --------------------------------------------------------------------------------
template<class T>
inline RDOTypeRange<T>::RDOTypeRange(const LPRDOTypeRangeRange& range)
    : T      (     )
    , m_pRange(range)
{
    ASSERT(m_pRange);
}

template<class T>
inline RDOTypeRange<T>::~RDOTypeRange()
{}

template<class T>
inline std::string RDOTypeRange<T>::name() const
{
    return rdo::format("%s %s", T::name().c_str(), m_pRange->src_text().c_str());
}

template<class T>
inline LPRDOType RDOTypeRange<T>::type_cast(const LPRDOType& from, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    return parent_type::type_cast(from, from_src_info, to_src_info, src_info);
}

template<class T>
inline LPRDOValue RDOTypeRange<T>::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
    LPRDOValue pToValue = T::value_cast(pFrom, to_src_info, src_info);
    m_pRange->checkValue(pToValue);
    return pToValue;
}

template<class T>
inline rdo::runtime::LPRDOCalc RDOTypeRange<T>::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& pType) const
{
    return rdo::Factory<rdo::runtime::RDOCalcCheckRange>::create(range()->getMin()->value(), range()->getMax()->value(), T::calc_cast(pCalc, pType));
}

template<class T>
inline const LPRDOTypeRangeRange& RDOTypeRange<T>::range() const
{
    return m_pRange;
}

template<>
inline LPRDOValue RDOTypeRange<RDOType__int>::get_default() const
{
    return rdo::Factory<RDOValue>::create(m_pRange->getMin()->value(), rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo());
}

template<>
inline LPRDOValue RDOTypeRange<RDOType__real>::get_default() const
{
    return rdo::Factory<RDOValue>::create(m_pRange->getMin()->value(), rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
