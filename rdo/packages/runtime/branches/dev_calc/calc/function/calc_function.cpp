/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     Функции с закладки FUN
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalc
// --------------------------------------------------------------------------------
RDOFunCalc::RDOFunCalc()
{}

void RDOFunCalc::addRetCalc(CREF(LPRDOCalc) pCalc)
{
	UNUSED(pCalc);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuncTableCalc
// --------------------------------------------------------------------------------
// Функция типа таблица
// --------------------------------------------------------------------------------
RDOFuncTableCalc::RDOFuncTableCalc(CREF(LPRDOCalc) pArgument)
	: m_pArgument(pArgument)
{
	ASSERT(m_pArgument);
}

void RDOFuncTableCalc::addResultCalc(CREF(LPRDOCalcConst) pResult)
{
	m_pResultList.push_back(pResult);
}

REF(RDOValue) RDOFuncTableCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ruint index = m_pArgument->calcValue(pRuntime).getUInt();
	return m_pResultList[index]->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunListCalc
// --------------------------------------------------------------------------------
// Функция типа список
// --------------------------------------------------------------------------------
RDOFunListCalc::RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue)
	: m_pDefaultValue(pDefaultValue)
{}

void RDOFunListCalc::addCase(CREF(LPRDOCalc) pCase, CREF(LPRDOCalcConst) pResult)
{
	ASSERT(pCase  );
	ASSERT(pResult);

	m_caseList  .push_back(pCase  ); 
	m_resultList.push_back(pResult);
}

REF(RDOValue) RDOFunListCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ResultList::const_iterator resultIt = m_resultList.begin();
	STL_FOR_ALL_CONST(m_caseList, caseIt)
	{
		if ((*caseIt)->calcValue(pRuntime).getAsBool())
		{
			return (*resultIt)->calcValue(pRuntime);
		}
		++resultIt;
	}
	return m_pDefaultValue->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunAlgorithmicCalc
// --------------------------------------------------------------------------------
RDOFunAlgorithmicCalc::RDOFunAlgorithmicCalc()
{
	m_value = 0;
}

void RDOFunAlgorithmicCalc::addCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pAction)
{
	ASSERT(pCondition);
	ASSERT(pAction   );

	m_conditionList.push_back(pCondition);
	m_actionList   .push_back(pAction   );
}

REF(RDOValue) RDOFunAlgorithmicCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	CalcList::const_iterator actionIt = m_actionList.begin();
	STL_FOR_ALL_CONST(m_conditionList, conditionIt)
	{
		if ((*conditionIt)->calcValue(pRuntime).getAsBool())
		{
			return (*actionIt)->calcValue(pRuntime);
		}
		++actionIt;
	}

	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	pRuntime->error().push(_T("Внутренная ошибка, RDOFunAlgorithmicCalc"), srcInfo());
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFuncParam
// --------------------------------------------------------------------------------
RDOCalcFuncParam::RDOCalcFuncParam(ruint paramID, CREF(RDOSrcInfo) src_info)
	: m_paramID(paramID)
{
	setSrcInfo(src_info);
}

REF(RDOValue) RDOCalcFuncParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_tempValueList.push_back(pRuntime->getFuncArgument(m_paramID));
	return m_tempValueList.back();
}

void RDOCalcFuncParam::reset()
{
	//! @todo никто не вызывает reset()
	m_tempValueList.clear();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetConst
// --------------------------------------------------------------------------------
RDOCalcGetConst::RDOCalcGetConst(ruint constantID)
	: m_constantID(constantID)
{}

REF(RDOValue) RDOCalcGetConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getConstValue(m_constantID);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetConst
// --------------------------------------------------------------------------------
RDOCalcSetConst::RDOCalcSetConst(ruint constantID, CREF(LPRDOCalc) pCalc)
	: m_constantID(constantID)
	, m_pCalc     (pCalc     )
{
	m_value = 0;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

REF(RDOValue) RDOCalcSetConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setConstValue(m_constantID, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunctionCaller
// --------------------------------------------------------------------------------
RDOCalcFunctionCaller::RDOCalcFunctionCaller(CREF(LPRDOCalc) pFunction)
	: m_pFunction(pFunction)
{}

void RDOCalcFunctionCaller::addParameter(CREF(LPRDOCalc) pParam)
{
	m_paramList.push_back(pParam);
}

void RDOCalcFunctionCaller::setFunctionCalc(CREF(LPRDOCalc) pFunction)
{
	m_pFunction = pFunction;
}

REF(RDOValue) RDOCalcFunctionCaller::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->pushFuncTop();
	STL_FOR_ALL_CONST(m_paramList, paramIt)
	{
		pRuntime->pushFuncArgument((*paramIt)->calcValue(pRuntime));
	}
	pRuntime->resetFuncTop(m_paramList.size());

	m_value = m_pFunction->calcValue(pRuntime);

	STL_FOR_ALL_CONST(m_paramList, paramIt)
	{
		pRuntime->popFuncArgument();
	}
	pRuntime->popFuncTop();

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
