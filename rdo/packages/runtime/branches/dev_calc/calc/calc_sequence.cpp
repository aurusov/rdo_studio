/*!
  \copyright (c) RDO-Team, 2011
  \file      sequence.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Последовательности
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/sequence.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqInit
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcSeqInit::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	m_gen->setSeed(m_base);
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNextUniform
// --------------------------------------------------------------------------------
RDOValue RDOCalcSeqNextUniform::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	RDOValue from = pRuntime->getFuncArgument(0);
	RDOValue to   = pRuntime->getFuncArgument(1);
	if (from > to)
	{
		pRuntime->error(
			rdo::format(_T("Для последовательности типа uniform нижняя граница диапазона должна быть меньше либо равна верхней, текущие значения: %s..%s")
				, from.getAsString().c_str()
				, to  .getAsString().c_str())
			, this
		);
	}
	return m_gen->next(from.getDouble(), to.getDouble());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNextNormal
// --------------------------------------------------------------------------------
RDOValue RDOCalcSeqNextNormal::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble(), pRuntime->getFuncArgument(1).getDouble());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNextExponential
// --------------------------------------------------------------------------------
RDOValue RDOCalcSeqNextExponential::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNextTriangular
// --------------------------------------------------------------------------------
RDOValue RDOCalcSeqNextTriangular::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	RDOValue from = pRuntime->getFuncArgument(0);
	RDOValue top  = pRuntime->getFuncArgument(1);
	RDOValue to   = pRuntime->getFuncArgument(2);
	if ((from > top) || (top > to))
	{
		pRuntime->error(rdo::format(_T("Для треуголного закона распределения нужно указать левую границу, точку под высотой треугольника, правую границу: %s, %s, %s")
			, from.getAsString().c_str()
			, top.getAsString().c_str()
			, to.getAsString().c_str()
		, this));
	}
	return m_gen->next(from.getDouble(), top.getDouble(), to.getDouble());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNextByHist
// --------------------------------------------------------------------------------
RDOValue RDOCalcSeqNextByHist::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return m_gen->next();
}

CLOSE_RDO_RUNTIME_NAMESPACE
