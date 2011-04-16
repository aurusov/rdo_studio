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

REF(RDOValue) RDOCalc::calcValue(PTR(RDORuntime) runtime)
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
		return doCalc(runtime);
	}
	catch (REF(RDORuntimeException))
	{
		if (runtime->errors.empty())
		{
			runtime->error(_T("ошибка в"), this);
//			runtime->error(_T("in"),       this);
		}
		else
		{
			runtime->error(_T(""), this);
		}
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetResParam (Параметры постоянного ресурса)
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetResParam::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getResParamVal(m_resID, m_paramID);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTempResParamFRM (Параметры временного ресурса для FRM)
// ----------------------------------------------------------------------------
RDOCalcGetTempResParamFRM::RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb)
	: RDOCalcGetResParam(_resNumb, _parNumb)
{
//TODO	runtime->connect(this, RDORuntime::RO_BEFOREDELETE);
}

REF(RDOValue) RDOCalcGetTempResParamFRM::doCalc(PTR(RDORuntime) runtime)
{
	if (m_resID >= 0)
	{
		m_value = runtime->getResParamVal(m_resID, m_paramID);
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
REF(RDOValue) RDOCalcGetUnknowResParam::doCalc(PTR(RDORuntime) runtime)
{
	runtime->error(rdo::format(_T("Попытка использовать несуществующий ресурс: %s.%s"), m_resName.c_str(), m_parName.c_str()), this);
	return m_value;
}


// ----------------------------------------------------------------------------
// ---------- RDOCalcGetGroupResParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetGroupResParam::doCalc(PTR(RDORuntime) runtime)
{
	PTR(RDOResource) currRes = static_cast<PTR(RDOResource)>(runtime->getGroupFuncRes());
	m_value = currRes->getParam(m_parNumb);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetRelevantResParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetRelevantResParam::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetRelParamDiapCalc::doCalc(PTR(RDORuntime) runtime)
{
	m_pCalc->calcValue(runtime);
	m_value = runtime->getResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb);
	if (m_value < m_min_value || m_value > m_max_value)
	{
		runtime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetResourceParamCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->setResParamVal(m_resNumb, m_parNumb, m_pCalc->calcValue(runtime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcEraseRes
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcEraseRes::doCalc(PTR(RDORuntime) runtime)
{
	runtime->onEraseRes(runtime->getCurrentActivity()->getResByRelRes(m_rel_res_id), this);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetPatternParamCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetPatternParamCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->setPatternParameter(m_parNumb, m_val);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcPatParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcPatParam::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getPatternParameter(m_numberOfParam);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTimeNow
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTimeNow::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getTimeNow();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetSeconds
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetSeconds::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getSeconds();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetTermNow
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTermNow::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getCurrentTerm();
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

REF(RDOValue) RDOCalcIf::doCalc(PTR(RDORuntime) runtime)
{
	m_value = RDOValue(false);
	return (m_pCondition->calcValue(runtime).getAsBool()) ? m_pStatement->calcValue(runtime) : (m_value);
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

REF(RDOValue) RDOCalcIfElse::doCalc(PTR(RDORuntime) runtime)
{
	return (m_pCondition->calcValue(runtime).getAsBool()) ? m_pIfStatement->calcValue(runtime) : m_pElseStatement->calcValue(runtime);
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

REF(RDOValue) RDOCalcCreateLocalVariable::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getMemoryStack()->create(m_name);
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

REF(RDOValue) RDOCalcSetLocalVariable::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getMemoryStack()->set(m_name, m_pCalc->calcValue(runtime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetLocalVariable
// ----------------------------------------------------------------------------
RDOCalcGetLocalVariable::RDOCalcGetLocalVariable(CREF(tstring) name)
	: m_name(name)
{}

REF(RDOValue) RDOCalcGetLocalVariable::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getMemoryStack()->get(m_name);
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

REF(RDOValue) RDOCalcLocalVariableList::doCalc(PTR(RDORuntime) runtime)
{
	STL_FOR_ALL(m_calcLocalVariableList, calc_it)
	{
		(*calc_it)->calcValue(runtime);
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

REF(RDOValue) RDOCalcList::doCalc(PTR(RDORuntime) runtime)
{
	STL_FOR_ALL(m_calcList, calc_it)
	{
		(*calc_it)->calcValue(runtime);
	}

	m_value = RDOValue(m_calcList.size());
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcNoChange
// ----------------------------------------------------------------------------
RDOCalcNoChange::RDOCalcNoChange()
{}

REF(RDOValue) RDOCalcNoChange::doCalc(PTR(RDORuntime) runtime)
{
	m_value = RDOValue(0);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа таблица
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFuncTableCalc::doCalc(PTR(RDORuntime) runtime)
{
	int index = m_pArgCalc->calcValue( runtime ).getInt();
	return m_results.at(index)->calcValue( runtime );
}

// ----------------------------------------------------------------------------
// ---------- RDOFunListCalc
// ----------------------------------------------------------------------------
// Функция типа список
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunListCalc::doCalc(PTR(RDORuntime) runtime)
{
	size_t size = m_cases.size();
	for (ruint i = 0; i < size; i++)
	{
		LPRDOCalc cas = m_cases[i];
		if (cas->calcValue( runtime ).getAsBool())
		{
			return m_results[i]->calcValue(runtime);
		}
	}
	return m_pDefaultValue->calcValue(runtime);
}

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunAlgorithmicCalc::doCalc(PTR(RDORuntime) runtime)
{
	int size = m_conditions.size();
	for (int i = 0; i < size; i++)
	{
		if (m_conditions[i]->calcValue(runtime).getAsBool())
		{
			return m_actions[i]->calcValue(runtime);
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	runtime->error(_T("Внутренная ошибка, RDOFunAlgorithmicCalc"), this);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunAlgorithmicDiapCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunAlgorithmicDiapCalc::doCalc(PTR(RDORuntime) runtime)
{
	int size = m_conditions.size();
	for (int i = 0; i < size; i++)
	{
		if (m_conditions[i]->calcValue(runtime).getAsBool())
		{
			m_value = m_actions[i]->calcValue(runtime);
			if (m_value < m_min_value || m_value > m_max_value)
			{
				runtime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
			}
			return m_value;
		}
	}
	// До сюда дело дойти не должно, т.к. последний conditions должен быть значением по-умолчанию
	runtime->error(_T("Внутренная ошибка, RDOFunAlgorithmicDiapCalc"), this);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
REF(RDOValue) RDOFunCalcExist::doCalc(PTR(RDORuntime) runtime)
{
	rbool res = false;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end && !res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		runtime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcNotExist::doCalc(PTR(RDORuntime) runtime)
{
	rbool res = true;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end && res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		runtime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(runtime).getAsBool())
			res = false;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcForAll::doCalc(PTR(RDORuntime) runtime)
{
	rbool first_found = false;
	rbool res = true;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end && res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		runtime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(runtime).getAsBool())
		{
			res = false;
		}
		else if (!first_found)
		{
			first_found = true;
		}
		runtime->popGroupFunc();
	}
	m_value = first_found ? res : false;
	return m_value;
}

REF(RDOValue) RDOFunCalcNotForAll::doCalc(PTR(RDORuntime) runtime)
{
	rbool res = false;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end && !res; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		runtime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
void RDOFunCalcSelect::prepare(PTR(RDORuntime) runtime)
{
	res_list.clear();
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(m_nResType))
			continue;

		runtime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(runtime).getAsBool())
			res_list.push_back(*it);
		runtime->popGroupFunc();
	}
}

REF(RDOValue) RDOFunCalcSelect::doCalc(PTR(RDORuntime) runtime)
{
	prepare(runtime);
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectExist::doCalc(PTR(RDORuntime) runtime)
{
	m_pSelect->prepare(runtime);
	rbool res = false;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		runtime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectNotExist::doCalc(PTR(RDORuntime) runtime)
{
	m_pSelect->prepare(runtime);
	rbool res = true;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && res)
	{
		runtime->pushGroupFunc(*it);
		if (m_pCondition->calcValue(runtime).getAsBool())
			res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectForAll::doCalc(PTR(RDORuntime) runtime)
{
	m_pSelect->prepare(runtime);
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
		runtime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(runtime).getAsBool())
			res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectNotForAll::doCalc(PTR(RDORuntime) runtime)
{
	m_pSelect->prepare(runtime);
	rbool res = false;
	std::list<PTR(RDOResource)>::iterator it  = m_pSelect->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_pSelect->res_list.end();
	while (it != end && !res)
	{
		runtime->pushGroupFunc(*it);
		if (!m_pCondition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectSize::doCalc(PTR(RDORuntime) runtime)
{
	m_pSelect->prepare(runtime);
	m_value = m_pSelect->res_list.size();
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectEmpty::doCalc(PTR(RDORuntime) runtime)
{
	m_pSelect->prepare(runtime);
	m_value = m_pSelect->res_list.empty();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcFuncParam::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getFuncArgument(m_param_number);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcGetConst / RDOCalcSetConst
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetConst::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getConstValue(m_number);
	return m_value;
}

REF(RDOValue) RDOCalcSetConst::doCalc(PTR(RDORuntime) runtime)
{
	runtime->setConstValue(m_number, m_pCalc->calcValue(runtime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcFunctionCall
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcFunctionCall::doCalc(PTR(RDORuntime) runtime)
{
	runtime->pushFuncTop();
	int size = m_parameters.size();
	for (int i = 0; i < size; i++)
	{
		RDOValue arg = m_parameters[i]->calcValue(runtime);
		runtime->pushFuncArgument(arg);
	}
	runtime->resetFuncTop(m_parameters.size());
	m_value = m_pFunction->calcValue(runtime);
	size = m_parameters.size();
	for (int i = 0; i < size; i++)
	{
		runtime->popFuncArgument();
	}
	runtime->popFuncTop();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
