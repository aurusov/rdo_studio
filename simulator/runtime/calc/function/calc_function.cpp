// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
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

void RDOFunCalc::addRetCalc(const LPRDOCalc& /*pCalc*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFuncTableCalc
// --------------------------------------------------------------------------------
// Функция типа таблица
// --------------------------------------------------------------------------------
RDOFuncTableCalc::RDOFuncTableCalc(const LPRDOCalc& pArgument)
	: m_pArgument(pArgument)
{
	ASSERT(m_pArgument);
}

void RDOFuncTableCalc::addResultCalc(const LPRDOCalcConst& pResult)
{
	m_pResultList.push_back(pResult);
}

RDOValue RDOFuncTableCalc::doCalc(const LPRDORuntime& pRuntime)
{
	const std::size_t index = m_pArgument->calcValue(pRuntime).getUInt();
	return m_pResultList[index]->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunListCalc
// --------------------------------------------------------------------------------
// Функция типа список
// --------------------------------------------------------------------------------
RDOFunListCalc::RDOFunListCalc(const LPRDOCalcConst& pDefaultValue)
	: m_pDefaultValue(pDefaultValue)
{}

void RDOFunListCalc::addCase(const LPRDOCalc& pCase, const LPRDOCalcConst& pResult)
{
	ASSERT(pCase  );
	ASSERT(pResult);

	m_caseList  .push_back(pCase  ); 
	m_resultList.push_back(pResult);
}

RDOValue RDOFunListCalc::doCalc(const LPRDORuntime& pRuntime)
{
	ResultList::const_iterator resultIt = m_resultList.begin();
	for (const auto& calc: m_caseList)
	{
		if (calc->calcValue(pRuntime).getAsBool())
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
{}

void RDOFunAlgorithmicCalc::addCalcIf(const LPRDOCalc& pCondition, const LPRDOCalc& pAction)
{
	ASSERT(pCondition);
	ASSERT(pAction   );

	m_conditionList.push_back(pCondition);
	m_actionList   .push_back(pAction   );
}

RDOValue RDOFunAlgorithmicCalc::doCalc(const LPRDORuntime& pRuntime)
{
	RDOCalcList::const_iterator actionIt = m_actionList.begin();
	for (const auto& condition: m_conditionList)
	{
		if (condition->calcValue(pRuntime).getAsBool())
		{
			return (*actionIt)->calcValue(pRuntime);
		}
		++actionIt;
	}

	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по молчанию
	pRuntime->error().push("Внутренная ошибка, RDOFunAlgorithmicCalc", srcInfo());
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFuncParam
// --------------------------------------------------------------------------------
RDOCalcFuncParam::RDOCalcFuncParam(std::size_t paramID, const RDOSrcInfo& src_info)
	: m_paramID(paramID)
{
	setSrcInfo(src_info);
}

RDOValue RDOCalcFuncParam::doCalc(const LPRDORuntime& pRuntime)
{
	return pRuntime->getFuncArgument(m_paramID);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetConst
// --------------------------------------------------------------------------------
RDOCalcGetConst::RDOCalcGetConst(std::size_t constantID)
	: m_constantID(constantID)
{}

RDOValue RDOCalcGetConst::doCalc(const LPRDORuntime& pRuntime)
{
	return pRuntime->getConstValue(m_constantID);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetConst
// --------------------------------------------------------------------------------
RDOCalcSetConst::RDOCalcSetConst(std::size_t constantID, const LPRDOCalc& pCalc)
	: m_constantID(constantID)
	, m_pCalc     (pCalc     )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

RDOValue RDOCalcSetConst::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->setConstValue(m_constantID, m_pCalc->calcValue(pRuntime));
	return RDOValue();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunctionCaller
// --------------------------------------------------------------------------------
RDOCalcFunctionCaller::RDOCalcFunctionCaller(const LPRDOCalc& pFunction)
	: m_pFunction(pFunction)
{}

void RDOCalcFunctionCaller::addParameter(const LPRDOCalc& pParam)
{
	m_paramList.push_back(pParam);
}

void RDOCalcFunctionCaller::setFunctionCalc(const LPRDOCalc& pFunction)
{
	m_pFunction = pFunction;
}

const LPRDOCalc& RDOCalcFunctionCaller::function() const
{
	return m_pFunction;
}

RDOValue RDOCalcFunctionCaller::doCalc(const LPRDORuntime& pRuntime)
{
	pRuntime->pushFuncTop();
	for (const auto& param: m_paramList)
	{
		pRuntime->pushFuncArgument(param->calcValue(pRuntime));
	}
	pRuntime->resetFuncTop(m_paramList.size());

	RDOValue value = m_pFunction->calcValue(pRuntime);

	for (const auto& param: m_paramList)
	{
		pRuntime->popFuncArgument();
	}
	pRuntime->popFuncTop();

	return value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
