/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_sequence.cpp
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
#include "simulator/runtime/calc/calc_sequence.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqInit
// --------------------------------------------------------------------------------
RDOCalcSeqInit::RDOCalcSeqInit(int base, PTR(RandGenerator) gen)
	: m_base(base)
	, m_gen (gen )
{}

void RDOCalcSeqInit::setBase(int base)
{
	m_base = base;
}

RDOValue RDOCalcSeqInit::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	m_gen->setSeed(m_base);
	return RDOValue();
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNext
// --------------------------------------------------------------------------------
RDOCalcSeqNext::Range::Range()
	: m_min(0)
	, m_max(0)
{}

RDOCalcSeqNext::Range::Range(CREF(double) min, CREF(double) max)
	: m_min(min)
	, m_max(max)
{}

RDOCalcSeqNext::RDOCalcSeqNext()
	: m_res_real(true )
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSeqNextUniform
// --------------------------------------------------------------------------------
RDOValue RDOCalcSeqNextUniform::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	RDOValue from = pRuntime->getFuncArgument(0);
	RDOValue to   = pRuntime->getFuncArgument(1);
	if (from > to)
	{
		pRuntime->error().push(
			rdo::format(_T("Для последовательности типа uniform нижняя граница диапазона должна быть меньше либо равна верхней, текущие значения: %s..%s")
				, from.getAsString().c_str()
				, to  .getAsString().c_str())
			, srcInfo()
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
		pRuntime->error().push(
			rdo::format(_T("Для треуголного закона распределения нужно указать левую границу, точку под высотой треугольника, правую границу: %s, %s, %s")
				, from.getAsString().c_str()
				, top.getAsString().c_str()
				, to.getAsString().c_str())
			, srcInfo()
		);
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
