/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocalc.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Вычислитель
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalc
// --------------------------------------------------------------------------------
RDOCalc::RDOCalc()
{}

RDOCalc::~RDOCalc()
{}

REF(RDOValue) RDOCalc::calcValue(CREF(LPRDORuntime) pRuntime)
{
	try
	{
#ifdef _DEBUG
/*
		if (src_text().empty())
		{
			TRACE(_T("%d\n"), sizeof(tstring));
		}
		else if (src_text().length() < 500)
		{
			TRACE(_T("calc: %s\n"), src_text().c_str());
			if (src_text() == _T("класс"))
			{
				TRACE(_T("calc: %s\n"), src_text().c_str());
			}
		}
		else 
		{
			tstring str = src_text();
			str.resize(500);
			TRACE(_T("calc: %s\n"), str.c_str());
		}
*/
#endif
		return doCalc(pRuntime);
	}
	catch (REF(RDORuntimeException))
	{
		if (pRuntime->errors.empty())
		{
			pRuntime->error(_T("ошибка в"), this);
//			pRuntime->error(_T("in"),       this);
		}
		else
		{
			pRuntime->error(_T(""), this);
		}
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResParam (Параметры постоянного ресурса)
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetResParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getResParamVal(m_resID, m_paramID);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResID (Получение ID ресурса по калку)
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetResID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_pCalcGetResource->calcValue(pRuntime).getPointer<RDOResource>();
	ASSERT(pResource);
	m_value = pResource->getTraceID();//TODO:здесь должен быть метод, возвращающий ID ресурса, а не типа.
	return m_value;
}
// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResParam2 (Параметры постоянного ресурса как параметра функции)
// --------------------------------------------------------------------------------
/*REF(RDOValue) RDOCalcGetResParam2::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getResParamVal(m_resID, m_paramID);
	return m_value;
}
*/
// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// --------------------------------------------------------------------------------
RDOCalcGetTempResParamFRM::RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb)
	: RDOCalcGetResParam(_resNumb, _parNumb)
{
/// @todo pRuntime->connect(this, RDORuntime::RO_BEFOREDELETE);
}

REF(RDOValue) RDOCalcGetTempResParamFRM::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (m_resID >= 0)
	{
		m_value = pRuntime->getResParamVal(m_resID, m_paramID);
	}
	else if (m_resID == -1)
	{
		LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
		ASSERT(pEnum);
		pEnum->add(_T("Удален"));
		m_value = RDOValue(pEnum);
		m_resID = -2;
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetUnknowResParam (Параметры несуществующего ресурса)
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetUnknowResParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->error(rdo::format(_T("Попытка использовать несуществующий ресурс: %s.%s"), m_resName.c_str(), m_parName.c_str()), this);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetGroupResParam
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetGroupResParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pCurrRes = pRuntime->getGroupFuncRes();
	ASSERT(pCurrRes);
	m_value = pCurrRes->getParam(m_parNumb);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParamCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOSetResourceParamCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setResParamVal(m_resNumb, m_parNumb, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetPatternParamCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOSetPatternParamCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setPatternParameter(m_parNumb, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPatParam
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcPatParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getPatternParameter(m_numberOfParam);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTimeNow
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTimeNow::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getTimeNow();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetSeconds
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetSeconds::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getSeconds();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTermNow
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTermNow::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getCurrentTerm();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunListCalc
// --------------------------------------------------------------------------------
// Функция типа таблица
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFuncTableCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	int index = m_pArgCalc->calcValue( pRuntime ).getInt();
	return m_results.at(index)->calcValue( pRuntime );
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunListCalc
// --------------------------------------------------------------------------------
// Функция типа список
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunListCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	size_t size = m_cases.size();
	for (ruint i = 0; i < size; i++)
	{
		LPRDOCalc cas = m_cases[i];
		if (cas->calcValue( pRuntime ).getAsBool())
		{
			return m_results[i]->calcValue(pRuntime);
		}
	}
	return m_pDefaultValue->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunAlgorithmicCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunAlgorithmicCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	int size = m_conditions.size();
	for (int i = 0; i < size; i++)
	{
		if (m_conditions[i]->calcValue(pRuntime).getAsBool())
		{
			return m_actions[i]->calcValue(pRuntime);
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	pRuntime->error(_T("Внутренная ошибка, RDOFunAlgorithmicCalc"), this);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunAlgorithmicDiapCalc
// --------------------------------------------------------------------------------
REF(RDOValue) RDOFunAlgorithmicDiapCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	int size = m_conditions.size();
	for (int i = 0; i < size; i++)
	{
		if (m_conditions[i]->calcValue(pRuntime).getAsBool())
		{
			m_value = m_actions[i]->calcValue(pRuntime);
			if (m_value < m_min_value || m_value > m_max_value)
			{
				pRuntime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
			}
			return m_value;
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	pRuntime->error(_T("Внутренная ошибка, RDOFunAlgorithmicDiapCalc"), this);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFuncParam
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcFuncParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getFuncArgument(m_param_number);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetConst / RDOCalcSetConst
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getConstValue(m_number);
	return m_value;
}

REF(RDOValue) RDOCalcSetConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setConstValue(m_number, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunctionCall
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcFunctionCall::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->pushFuncTop();
	int size = m_parameters.size();
	for (int i = 0; i < size; i++)
	{
		RDOValue arg = m_parameters[i]->calcValue(pRuntime);
		pRuntime->pushFuncArgument(arg);
	}
	pRuntime->resetFuncTop(m_parameters.size());
	m_value = m_pFunction->calcValue(pRuntime);
	size = m_parameters.size();
	for (int i = 0; i < size; i++)
	{
		pRuntime->popFuncArgument();
	}
	pRuntime->popFuncTop();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
