/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
RDOCalc::RDOCalc()
{
	resetCnt();
}

RDOCalc::~RDOCalc()
{}

REF(RDOValue) RDOCalc::calcValue(PTR(RDORuntime) pRuntime)
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

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetResParam::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getResParamVal(m_resID, m_paramID);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
RDOCalcGetTempResParamFRM::RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb)
	: RDOCalcGetResParam(_resNumb, _parNumb)
{
//TODO	pRuntime->connect(this, RDORuntime::RO_BEFOREDELETE);
}

REF(RDOValue) RDOCalcGetTempResParamFRM::doCalc(PTR(RDORuntime) pRuntime)
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

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetUnknowResParam (Параметры несуществующего ресурса)
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetUnknowResParam::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->error(rdo::format(_T("Попытка использовать несуществующий ресурс: %s.%s"), m_resName.c_str(), m_parName.c_str()), this);
	return m_value;
}


// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetGroupResParam::doCalc(PTR(RDORuntime) pRuntime)
{
	PTR(RDOResource) currRes = static_cast<PTR(RDOResource)>(pRuntime->getGroupFuncRes());
	m_value = currRes->getParam(m_parNumb);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetResourceParamCalc::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->setResParamVal(m_resNumb, m_parNumb, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetPatternParamCalc::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->setPatternParameter(m_parNumb, m_val);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcPatParam::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getPatternParameter(m_numberOfParam);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTimeNow::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getTimeNow();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetSeconds::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getSeconds();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTermNow
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTermNow::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getCurrentTerm();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcIf
// ----------------------------------------------------------------------------
RDOCalcIf::RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement)
	: m_pCondition(pCondition)
	, m_pStatement(pStatement)
{
	ASSERT(m_pCondition);
	ASSERT(m_pStatement);
}

REF(RDOValue) RDOCalcIf::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = RDOValue(false);
	return (m_pCondition->calcValue(pRuntime).getAsBool()) ? m_pStatement->calcValue(pRuntime) : (m_value);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcIfElse
// ----------------------------------------------------------------------------
RDOCalcIfElse::RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement)
	: m_pCondition    (pCondition    )
	, m_pIfStatement  (pIfStatement  )
	, m_pElseStatement(pElseStatement)
{
	ASSERT(m_pCondition    );
	ASSERT(m_pIfStatement  );
	ASSERT(m_pElseStatement);

	if((m_pIfStatement->isReturn() > 0)&&(m_pElseStatement->isReturn() > 0)) upCnt();
}

REF(RDOValue) RDOCalcIfElse::doCalc(PTR(RDORuntime) pRuntime)
{
	return (m_pCondition->calcValue(pRuntime).getAsBool()) ? m_pIfStatement->calcValue(pRuntime) : m_pElseStatement->calcValue(pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFor
// ----------------------------------------------------------------------------
RDOCalcFor::RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement)
	:m_pDeclaration(pDeclaration)
	,m_pCondition  (pCondition  )
	,m_pExpression (pExpression )
	,m_pStatement  (pStatement  )
{
	ASSERT(m_pDeclaration);
	ASSERT(m_pCondition  );
	ASSERT(m_pExpression );
	ASSERT(m_pStatement  );

	if(m_pStatement->isReturn()) upCnt();
}

REF(RDOValue) RDOCalcFor::doCalc(PTR(RDORuntime) pRuntime)
{
	if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_CONTINUE)
	{
		m_value = m_pDeclaration->calcValue(pRuntime);
		while (m_pCondition->calcValue(pRuntime).getAsBool())
		{
			m_value = m_pExpression->calcValue(pRuntime);
			m_value = m_pStatement->calcValue(pRuntime);
			if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
			{
				pRuntime->setFunBreakFlag(RDORuntime::FBF_CONTINUE);
				return m_value;
			}
			if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
			{
				return m_value;
			}
		}
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateLocalVariable
// ----------------------------------------------------------------------------
RDOCalcCreateLocalVariable::RDOCalcCreateLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

REF(RDOValue) RDOCalcCreateLocalVariable::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->create(m_name);
	m_value = RDOValue();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcSetLocalVariable
// ----------------------------------------------------------------------------
RDOCalcSetLocalVariable::RDOCalcSetLocalVariable(CREF(tstring) name, CREF(LPRDOCalc) pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

REF(RDOValue) RDOCalcSetLocalVariable::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->set(m_name, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetLocalVariable
// ----------------------------------------------------------------------------
RDOCalcGetLocalVariable::RDOCalcGetLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

REF(RDOValue) RDOCalcGetLocalVariable::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getMemoryStack()->get(m_name);
	return m_value;
}


// ----------------------------------------------------------------------------
// ---------- RDOCalcLocalVariableList
// ----------------------------------------------------------------------------
RDOCalcLocalVariableList::RDOCalcLocalVariableList()
{}

void RDOCalcLocalVariableList::addCalcLocalVariable(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcLocalVariableList.push_back(pCalc);
}

REF(RDOValue) RDOCalcLocalVariableList::doCalc(PTR(RDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcLocalVariableList, calc_it)
	{
		(*calc_it)->calcValue(pRuntime);
	}

	m_value = RDOValue(m_calcLocalVariableList.size());
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcOpenBrace
// ----------------------------------------------------------------------------
RDOCalcOpenBrace::RDOCalcOpenBrace()
{}

REF(RDOValue) RDOCalcOpenBrace::doCalc(PTR(RDORuntime) pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCloseBrace
// ----------------------------------------------------------------------------
RDOCalcCloseBrace::RDOCalcCloseBrace()
{}

REF(RDOValue) RDOCalcCloseBrace::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunList
// ----------------------------------------------------------------------------
RDOCalcFunList::RDOCalcFunList()
{}

void RDOCalcFunList::addFunCalc(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcFunList.push_back(pCalc);

	if (pCalc->isReturn())
	{
		upCnt();
	}
}

void RDOCalcFunList::addRetCalc(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	LPRDOCalc pCalcReturn = rdo::Factory<rdoRuntime::RDOCalcFunReturn>::create(pCalc);
	m_calcFunList.insert(--m_calcFunList.end(), pCalcReturn);
}

REF(RDOValue) RDOCalcFunList::doCalc(PTR(RDORuntime) pRuntime)
{
	if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_CONTINUE)
	{
		m_value = RDOValue(m_calcFunList.size());
		STL_FOR_ALL(m_calcFunList, calc_it)
		{
			LPRDOCalc calc = (*calc_it);
			(*calc_it)->calcValue(pRuntime);
			if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
			{
				m_calcFunList.back()->calcValue(pRuntime);
				pRuntime->setFunBreakFlag(RDORuntime::FBF_CONTINUE);
				return m_value;
			}
			if (pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
			{
				m_value = (*calc_it)->calcValue(pRuntime);
				m_calcFunList.back()->calcValue(pRuntime);
				return m_value;
			}
		}
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunBreak
// ----------------------------------------------------------------------------
RDOCalcFunBreak::RDOCalcFunBreak()
{}

REF(RDOValue) RDOCalcFunBreak::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_BREAK);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunReturn
// ----------------------------------------------------------------------------
RDOCalcFunReturn::RDOCalcFunReturn(CREF(LPRDOCalc) pReturn)
	: m_pReturn(pReturn)
{
	upCnt();
}

REF(RDOValue) RDOCalcFunReturn::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_RETURN);
	return m_pReturn->calcValue(pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcList
// ----------------------------------------------------------------------------
RDOCalcList::RDOCalcList()
{}

void RDOCalcList::addCalc(CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);

	if(pCalc->isReturn()) upCnt();
}

REF(RDOValue) RDOCalcList::doCalc(PTR(RDORuntime) pRuntime)
{
	STL_FOR_ALL(m_calcList, calc_it)
	{
		(*calc_it)->calcValue(pRuntime);
	}

	m_value = RDOValue(m_calcList.size());
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcNoChange
// ----------------------------------------------------------------------------
RDOCalcNoChange::RDOCalcNoChange()
{}

REF(RDOValue) RDOCalcNoChange::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = RDOValue(0);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа таблица
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFuncTableCalc::doCalc(PTR(RDORuntime) pRuntime)
{
	int index = m_pArgCalc->calcValue( pRuntime ).getInt();
	return m_results.at(index)->calcValue( pRuntime );
}

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunListCalc::doCalc(PTR(RDORuntime) pRuntime)
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

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunAlgorithmicCalc::doCalc(PTR(RDORuntime) pRuntime)
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

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunAlgorithmicDiapCalc::doCalc(PTR(RDORuntime) pRuntime)
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

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcExist::doCalc(PTR(RDORuntime) pRuntime)
{
	rbool res = false;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && !res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcNotExist::doCalc(PTR(RDORuntime) pRuntime)
{
	rbool res = true;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcForAll::doCalc(PTR(RDORuntime) pRuntime)
{
	rbool first_found = false;
	rbool res = true;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
		{
			res = false;
		}
		else if (!first_found)
		{
			first_found = true;
		}
		pRuntime->popGroupFunc();
	}
	m_value = first_found ? res : false;
	return m_value;
}

REF(RDOValue) RDOFunCalcNotForAll::doCalc(PTR(RDORuntime) pRuntime)
{
	rbool res = false;
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end && !res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
void RDOFunCalcSelect::prepare(PTR(RDORuntime) pRuntime)
{
	res_list.clear();
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res_list.push_back(*it);
		pRuntime->popGroupFunc();
	}
}

REF(RDOValue) RDOFunCalcSelect::doCalc(PTR(RDORuntime) pRuntime)
{
	prepare(pRuntime);
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectExist::doCalc(PTR(RDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = false;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectNotExist::doCalc(PTR(RDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = true;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectForAll::doCalc(PTR(RDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	if (m_pSelect->res_list.empty())
	{
		m_value = false;
		return m_value;
	}
	rbool res = true;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = false;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectNotForAll::doCalc(PTR(RDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	rbool res = false;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		pRuntime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(pRuntime).getAsBool())
			res = true;
		pRuntime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectSize::doCalc(PTR(RDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	m_value = m_pSelect->res_list.size();
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectEmpty::doCalc(PTR(RDORuntime) pRuntime)
{
	m_pSelect->prepare(pRuntime);
	m_value = m_pSelect->res_list.empty();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcFuncParam::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getFuncArgument(m_param_number);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetConst::doCalc(PTR(RDORuntime) pRuntime)
{
	m_value = pRuntime->getConstValue(m_number);
	return m_value;
}

REF(RDOValue) RDOCalcSetConst::doCalc(PTR(RDORuntime) pRuntime)
{
	pRuntime->setConstValue(m_number, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcFunctionCall::doCalc(PTR(RDORuntime) pRuntime)
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
