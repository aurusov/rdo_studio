// ----------------------------------------------------------------------- INCLUDES
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/range.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOTypeRangeRange::RDOTypeRangeRange(const LPRDOValue& pMinValue, const LPRDOValue& pMaxValue, const RDOParserSrcInfo& src_info)
    : RDOParserSrcInfo(src_info )
    , m_pMinValue     (pMinValue)
    , m_pMaxValue     (pMaxValue)
{
    ASSERT(m_pMinValue->defined());
    ASSERT(m_pMaxValue->defined());
    setSrcText(rdo::format("[%s..%s]", m_pMinValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str()));
}

RDOTypeRangeRange::~RDOTypeRangeRange()
{}

void RDOTypeRangeRange::checkRange() const
{
    if (m_pMinValue->typeID() != m_pMaxValue->typeID())
    {
        parser::g_error().error(m_pMaxValue->src_info(), rdo::format("Границы диапазона должны быть одного типа, найдено: [%s] .. [%s]", m_pMinValue->typeInfo()->itype()->name().c_str(), m_pMaxValue->typeInfo()->itype()->name().c_str()));
    }
    if (m_pMinValue->value() > m_pMaxValue->value())
    {
        parser::g_error().error(m_pMaxValue->src_info(), "Левая граница диапазона должна быть меньше правой");
    }
}

void RDOTypeRangeRange::checkValue(const LPRDOValue& pValue) const
{
    ASSERT(pValue);

    if (pValue->value() < m_pMinValue->value() || pValue->value() > m_pMaxValue->value())
    {
        if (pValue->src_filetype() == src_filetype() && pValue->src_pos().m_last_line == src_pos().m_last_line)
        {
            parser::g_error().error(pValue->src_info(), rdo::format("Значение выходит за допустимый диапазон [%s..%s]: %s", m_pMinValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str(), pValue->value().getAsString().c_str()));
        }
        else
        {
            parser::g_error().push_only(pValue->src_info(), rdo::format("Значение выходит за допустимый диапазон [%s..%s]: %s", m_pMinValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str(), pValue->value().getAsString().c_str()));
            parser::g_error().push_only(src_info(),         rdo::format("См. описание диапазона"));
            parser::g_error().push_done();
        }
    }
}

const LPRDOValue& RDOTypeRangeRange::getMin() const
{
    return m_pMinValue;
}

const LPRDOValue& RDOTypeRangeRange::getMax() const
{
    return m_pMaxValue;
}

CLOSE_RDO_PARSER_NAMESPACE
