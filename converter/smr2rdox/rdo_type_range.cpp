/*!
  \copyright (c) RDO-Team, 2014
  \file      rdo_type_range.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.05.2014
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type_range.h"
#include "converter/smr2rdox/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

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
		rdo::converter::smr2rdox::g_error().error(m_pMaxValue->src_info(), rdo::format("Границы диапазона должны быть одного типа, найдено: [%s] .. [%s]", m_pMinValue->type()->name().c_str(), m_pMaxValue->type()->name().c_str()));
	}
	if (m_pMinValue->value() > m_pMaxValue->value())
	{
		rdo::converter::smr2rdox::g_error().error(m_pMaxValue->src_info(), "Левая граница диапазона должна быть меньше правой");
	}
}

void RDOTypeRangeRange::checkValue(const LPRDOValue& pValue) const
{
	if (pValue->value() < m_pMinValue->value() || pValue->value() > m_pMaxValue->value())
	{
		if (pValue->src_filetype() == src_filetype() && pValue->src_pos().m_last_line == src_pos().m_last_line)
		{
			rdo::converter::smr2rdox::g_error().error(pValue->src_info(), rdo::format("Значение выходит за допустимый диапазон [%s..%s]: %s", m_pMinValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str(), pValue->value().getAsString().c_str()));
		}
		else
		{
			rdo::converter::smr2rdox::g_error().push_only(pValue->src_info(), rdo::format("Значение выходит за допустимый диапазон [%s..%s]: %s", m_pMinValue->value().getAsString().c_str(), m_pMaxValue->value().getAsString().c_str(), pValue->value().getAsString().c_str()));
			rdo::converter::smr2rdox::g_error().push_only(src_info(),         rdo::format("См. описание диапазона"));
			rdo::converter::smr2rdox::g_error().push_done();
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
