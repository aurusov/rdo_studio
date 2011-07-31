/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      sequence.cpp
 * @authors   Барс Александр, Урусов Андрей
 * @date      13.03.2011
 * @brief     Последовательности
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/sequence.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcSeqInit
// ********************************************************************************
REF(RDOValue) RDOCalcSeqInit::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_gen->setSeed(m_base);
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

// ********************************************************************************
// ******************** RDOCalcSeqNextUniform
// ********************************************************************************
RDOValue RDOCalcSeqNextUniform::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble(), pRuntime->getFuncArgument(1).getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextNormal
// ********************************************************************************
RDOValue RDOCalcSeqNextNormal::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble(), pRuntime->getFuncArgument(1).getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextExponential
// ********************************************************************************
RDOValue RDOCalcSeqNextExponential::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextByHist
// ********************************************************************************
RDOValue RDOCalcSeqNextByHist::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next();
}

CLOSE_RDO_RUNTIME_NAMESPACE
