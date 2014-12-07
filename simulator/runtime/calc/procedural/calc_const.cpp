// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcConst
// --------------------------------------------------------------------------------
RDOCalcConst::RDOCalcConst(const RDOValue& value)
	: m_value(value)
{
	ASSERT(m_value.typeID() != RDOType::t_unknow);
}

const RDOValue& RDOCalcConst::getValue() const
{
	return m_value;
}

RDOValue RDOCalcConst::doCalc(const LPRDORuntime& /*pRuntime*/)
{
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
