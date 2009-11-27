/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdocalc.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "pch.h"
// ====================================================================== INCLUDES
#include <limits>
#include <math.h>
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdocalc.h"
#include "rdoprocess.h"
#include "rdo_runtime.h"
#include "rdo_activity.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalc
// ----------------------------------------------------------------------------
RDOCalc::RDOCalc(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent)
{}

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
RDOCalcGetTempResParamFRM::RDOCalcGetTempResParamFRM(PTR(RDORuntime) runtime, int _resNumb, int _parNumb)
	: RDOCalcGetResParam(runtime, _resNumb, _parNumb)
{
	runtime->connect(this, RDORuntime::RO_BEFOREDELETE);
}

REF(RDOValue) RDOCalcGetTempResParamFRM::doCalc(PTR(RDORuntime) runtime)
{
	if (m_resID >= 0)
	{
		m_value = runtime->getResParamVal(m_resID, m_paramID);
	}
	else if (m_resID == -1)
	{
		PTR(RDOEnumType) _enum = new RDOEnumType(runtime);
		_enum->add(_T("Удален"));
		m_value = RDOValue(*_enum);
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
// ---------- RDOSetRelParamCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetRelParamCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->setResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb, m_calc->calcValue(runtime));
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelParamDiapCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetRelParamDiapCalc::doCalc(PTR(RDORuntime) runtime)
{
	m_value = m_calc->calcValue(runtime);
	if (m_value < m_min_value || m_value > m_max_value)
	{
		runtime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
	runtime->setResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb, m_value);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetResourceParamCalc
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSetResourceParamCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->setResParamVal(m_resNumb, m_parNumb, m_calc->calcValue(runtime));
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
		if (m_condition->calcValue(runtime).getAsBool())
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
		if (m_condition->calcValue(runtime).getAsBool())
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
		if (!m_condition->calcValue(runtime).getAsBool())
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
		if (!m_condition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
	}
	m_value = res;
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
void RDOFunCalcSelect::prepare(PTR(RDORuntime) runtime) const
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
		if (m_condition->calcValue(runtime).getAsBool())
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
	m_select->prepare(runtime);
	rbool res = false;
	std::list<PTR(RDOResource)>::iterator it  = m_select->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_select->res_list.end();
	while (it != end && !res)
	{
		runtime->pushGroupFunc(*it);
		if (m_condition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectNotExist::doCalc(PTR(RDORuntime) runtime)
{
	m_select->prepare(runtime);
	rbool res = true;
	std::list<PTR(RDOResource)>::iterator it  = m_select->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_select->res_list.end();
	while (it != end && res)
	{
		runtime->pushGroupFunc(*it);
		if (m_condition->calcValue(runtime).getAsBool())
			res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectForAll::doCalc(PTR(RDORuntime) runtime)
{
	m_select->prepare(runtime);
	if (m_select->res_list.empty())
	{
		m_value = false;
		return m_value;
	}
	rbool res = true;
	std::list<PTR(RDOResource)>::iterator it  = m_select->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_select->res_list.end();
	while (it != end && res)
	{
		runtime->pushGroupFunc(*it);
		if (!m_condition->calcValue(runtime).getAsBool())
			res = false;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectNotForAll::doCalc(PTR(RDORuntime) runtime)
{
	m_select->prepare(runtime);
	rbool res = false;
	std::list<PTR(RDOResource)>::iterator it  = m_select->res_list.begin();
	std::list<PTR(RDOResource)>::iterator end = m_select->res_list.end();
	while (it != end && !res)
	{
		runtime->pushGroupFunc(*it);
		if (!m_condition->calcValue(runtime).getAsBool())
			res = true;
		runtime->popGroupFunc();
		it++;
	}
	m_value = res;
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectSize::doCalc(PTR(RDORuntime) runtime)
{
	m_select->prepare(runtime);
	m_value = m_select->res_list.size();
	return m_value;
}

REF(RDOValue) RDOFunCalcSelectEmpty::doCalc(PTR(RDORuntime) runtime)
{
	m_select->prepare(runtime);
	m_value = m_select->res_list.empty();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
#define DECLARE_RDO_STD_FUN_DOUBLE(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getDouble()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_DOUBLE_INT(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getInt()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getInt()); \
	return m_value; \
}

#define DECLARE_RDO_STD_FUN_INT_INT(CalcName, FunName) \
REF(RDOValue) RDOFunCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	m_value = FunName(runtime->getFuncArgument(0).getInt(), runtime->getFuncArgument(1).getInt()); \
	return m_value; \
}

double modf(double value)
{
	double tmp;
	return ::modf(value, &tmp);
}

double log2(double value)
{
	return log(value) / log(2.0);
}

double logN(double value1, double value2)
{
	return log(value1) / log(value2);
}

int round(double value)
{
	return (int)floor(value + 0.5);
}

DECLARE_RDO_STD_FUN_DOUBLE       (Sin     , sin      );
DECLARE_RDO_STD_FUN_DOUBLE       (Cos     , cos      );
DECLARE_RDO_STD_FUN_DOUBLE       (Tan     , tan      );
DECLARE_RDO_STD_FUN_DOUBLE       (Cotan   , 1.0 / tan);
DECLARE_RDO_STD_FUN_DOUBLE       (ArcCos  , acos     );
DECLARE_RDO_STD_FUN_DOUBLE       (ArcSin  , asin     );
DECLARE_RDO_STD_FUN_DOUBLE       (ArcTan  , atan     );
DECLARE_RDO_STD_FUN_DOUBLE       (Abs     , fabs     );
DECLARE_RDO_STD_FUN_DOUBLE       (Sqrt    , sqrt     );
DECLARE_RDO_STD_FUN_DOUBLE       (Round   , round    );
DECLARE_RDO_STD_FUN_DOUBLE       (Exp     , exp      );
DECLARE_RDO_STD_FUN_DOUBLE       (Floor   , floor    );
DECLARE_RDO_STD_FUN_DOUBLE       (Frac    , modf     );
DECLARE_RDO_STD_FUN_DOUBLE       (Ln      , log      );
DECLARE_RDO_STD_FUN_DOUBLE       (Log10   , log10    );
DECLARE_RDO_STD_FUN_DOUBLE       (Log2    , log2     );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(LogN    , logN     );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(Max     , max      );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(Min     , min      );
DECLARE_RDO_STD_FUN_DOUBLE_DOUBLE(Power   , pow      );
DECLARE_RDO_STD_FUN_INT          (IAbs    , abs      );
DECLARE_RDO_STD_FUN_INT_INT      (IMax    , max      );
DECLARE_RDO_STD_FUN_INT_INT      (IMin    , min      );
DECLARE_RDO_STD_FUN_INT          (Int     , int      );
DECLARE_RDO_STD_FUN_DOUBLE_INT   (IntPower, pow      );

// ----------------------------------------------------------------------------
// ---------- RDOCalcFuncParam
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcFuncParam::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getFuncArgument(m_param_number);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary
// ----------------------------------------------------------------------------
#define DECLARE_BINARY_CALC(CalcName, CalcOpr) \
REF(RDOValue) RDOCalc##CalcName::doCalc(PTR(RDORuntime) runtime) \
{ \
	runtime->inc_cnt_calc_logic(); \
	m_value = m_left->calcValue(runtime) CalcOpr m_right->calcValue(runtime); \
	return m_value; \
}

// ----------------------------------------------------------------------------
// ---------- Арифметические функции
// ----------------------------------------------------------------------------
DECLARE_BINARY_CALC(Plus , +);
DECLARE_BINARY_CALC(Minus, -);
DECLARE_BINARY_CALC(Mult , *);

REF(RDOValue) RDOCalcDiv::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_arithm();
	REF(RDOValue) rVal = m_right->calcValue(runtime);
	if (rVal == 0)
	{
		runtime->error(_T("Деление на ноль") , this);
//		runtime->error(_T("Division by zero"), this);
	}
	m_value = m_left->calcValue(runtime) / rVal;
	return m_value;
}

REF(RDOValue) RDOCalcPlusEnumSafe::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_arithm();
	m_value = m_left->calcValue(runtime).getEnumAsInt() + m_right->calcValue(runtime).getEnumAsInt();
	return m_value;
}

REF(RDOValue) RDOCalcMultEnumSafe::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_arithm();
	m_value = m_left->calcValue(runtime).getEnumAsInt() * m_right->calcValue(runtime).getEnumAsInt();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Логические функции
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcAnd::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_logic();

	if (!m_left->calcValue(runtime).getAsBool())
		return m_value_false;

	if (!m_right->calcValue(runtime).getAsBool())
		return m_value_false;

	return m_value_true;
}

REF(RDOValue) RDOCalcOr::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_logic();

	if (m_left->calcValue(runtime).getAsBool())
		return m_value_true;

	if (m_right->calcValue(runtime).getAsBool())
		return m_value_true;

	return m_value_false;
}

REF(RDOValue) RDOCalcNot::doCalc(PTR(RDORuntime) runtime)
{
	runtime->inc_cnt_calc_logic();
	m_value = !m_calc->calcValue(runtime).getAsBool();
	return m_value;
}

DECLARE_BINARY_CALC(IsEqual   , ==);
DECLARE_BINARY_CALC(IsNotEqual, !=);
DECLARE_BINARY_CALC(IsLess    , < );
DECLARE_BINARY_CALC(IsGreater , > );
DECLARE_BINARY_CALC(IsLEQ     , <=);
DECLARE_BINARY_CALC(IsGEQ     , >=);

// ----------------------------------------------------------------------------
// ---------- Унарные операции
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcCheckDiap::doCalc(PTR(RDORuntime) runtime)
{
	m_value = m_oper->calcValue(runtime);
	if (m_value < m_min_value || m_value > m_max_value)
	{
		runtime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
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
	runtime->setConstValue(m_number, m_calc->calcValue(runtime));
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
	m_value = m_function->calcValue(runtime);
	size = m_parameters.size();
	for (int i = 0; i < size; i++)
	{
		runtime->popFuncArgument();
	}
	runtime->popFuncTop();
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
REF(RDOValue) RDOSelectResourceNonExistCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcSeqInit::doCalc(PTR(RDORuntime) runtime)	
{
	m_gen->setSeed(m_base);
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

RDOValue RDOCalcSeqNextUniform::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble());
}

RDOValue RDOCalcSeqNextNormal::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble());
}

RDOValue RDOCalcSeqNextExponential::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next(runtime->getFuncArgument(0).getDouble());
}

RDOValue RDOCalcSeqNextByHist::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next();
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource
// ----------------------------------------------------------------------------
RDOCalcCreateNumberedResource::RDOCalcCreateNumberedResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: RDOCalc    (parent      )
	, type       (_type       )
	, traceFlag  (_traceFlag  )
	, number     (_number     )
	, isPermanent(_isPermanent)
{
	paramsCalcs.insert(paramsCalcs.begin(), _paramsCalcs.begin(), _paramsCalcs.end());
}

REF(RDOValue) RDOCalcCreateNumberedResource::doCalc(PTR(RDORuntime) runtime)
{
	PTR(RDOResource) res = runtime->createNewResource(type, this);
	if (!isPermanent)
	{
		res->makeTemporary(true);
	}
	res->appendParams(paramsCalcs.begin(), paramsCalcs.end());
	return m_value; // just to return something
}

PTR(RDOResource) RDOCalcCreateNumberedResource::createResource(PTR(RDORuntime) runtime) const
{
	return new RDOResource(runtime, number, type, traceFlag);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateProcessResource
// ----------------------------------------------------------------------------
RDOCalcCreateProcessResource::RDOCalcCreateProcessResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent)
	: RDOCalcCreateNumberedResource(parent, _type, _traceFlag, _paramsCalcs, _number, _isPermanent)
{}

PTR(RDOResource) RDOCalcCreateProcessResource::createResource(PTR(RDORuntime) runtime) const
{
	return new RDOPROCResource(runtime, number, type, traceFlag);
}

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource
// ----------------------------------------------------------------------------
RDOCalcCreateEmptyResource::RDOCalcCreateEmptyResource(PTR(RDORuntimeParent) parent, int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _params_default, int _rel_res_id)
	: RDOCalc   (parent     )
	, type      (_type      )
	, traceFlag (_traceFlag )
	, rel_res_id(_rel_res_id)
{
	params_default.insert(params_default.begin(), _params_default.begin(), _params_default.end());
}

REF(RDOValue) RDOCalcCreateEmptyResource::doCalc(PTR(RDORuntime) runtime)
{
	PTR(RDOResource) res = runtime->createNewResource(type, traceFlag);
	runtime->getCurrentActivity()->setRelRes(rel_res_id, res->getTraceID());
	res->appendParams(params_default.begin(), params_default.end());
	return m_value; // just to return something
}

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc(PTR(RDORuntimeParent) parent, int _rel_res_id, PTR(RDOCalc) _choice_calc, PTR(RDOCalc) _order_calc, Type _order_type)
	: RDOCalc    (parent      )
	, rel_res_id (_rel_res_id )
	, choice_calc(_choice_calc)
	, order_calc (_order_calc )
	, order_type (_order_type )
{}

REF(RDOValue) RDOSelectResourceDirectCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
	if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
	{
		runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

REF(RDOValue) RDOSelectResourceByTypeCalc::doCalc(PTR(RDORuntime) runtime)
{
	RDOValue maxVal   = -DBL_MAX;
	RDOValue minVal   = DBL_MAX;
	int res_minmax_id = -1;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end; it++)
	{
		if (*it && (*it)->checkType(resType))
		{
			int res_id = (*it)->getTraceID();

			switch (order_type)
			{
				case order_empty:
				case order_first:
				{
					runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
					{
						runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					m_value = 1;
					return m_value;
				}
				case order_with_min:
				{
					runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
					{
						runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					RDOValue tmp = order_calc->calcValue(runtime);
					if (tmp < minVal)
					{
						minVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
				case order_with_max:
				{
					runtime->getCurrentActivity()->setRelRes(rel_res_id, res_id);
					if (choice_calc && !choice_calc->calcValue(runtime).getAsBool())
					{
						runtime->getCurrentActivity()->setRelRes(rel_res_id, -1);
						continue;
					}
					RDOValue tmp = order_calc->calcValue(runtime);
					if (tmp > maxVal)
					{
						maxVal        = tmp;
						res_minmax_id = res_id;
					}
					break;
				}
			}
		}
	}

	if (res_minmax_id != -1)
	{
		runtime->getCurrentActivity()->setRelRes(rel_res_id, res_minmax_id);
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
}

void RDOSelectResourceCommonCalc::getBest(REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, PTR(RDORuntime) runtime, REF(rbool) hasBest) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < resSelectors.size(); i++)
		{
			if (!resSelectors.at(i)->callChoice(runtime))
			{
				return; // state not valid
			}
		}
		RDOValue newVal = choice_calc->calcValue(runtime);
		if (!hasBest || (useCommonWithMax && (newVal > bestVal)) ||
		   (!useCommonWithMax && (newVal < bestVal))) // found better value
		{
			for (ruint i = 0; i < resSelectors.size(); i++)
			{
				res.at(i) = runtime->getCurrentActivity()->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		runtime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		getBest(allNumbs, level+1, res, bestVal, runtime, hasBest);
	}
}

rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, ruint level, PTR(RDORuntime) runtime) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < resSelectors.size(); i++)
		{
			if (!resSelectors.at(i)->callChoice(runtime))
			{
				return false;
			}
		}
		return true;
	}
	REF(std::vector<int>) ourLevel = allNumbs.at(level);
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		runtime->getCurrentActivity()->setRelRes(level, ourLevel.at(i));
		if (getFirst(allNumbs, level+1, runtime)) return true;
	}
	return false;
}

//rbool RDOSelectResourceCommonCalc::getFirst(REF(std::vector< std::vector<int> >) allNumbs, int level,PTR(RDORuntime) runtime) const
//{
//	if (level <= 0) {
//		for (int i = 0; i < resSelectors.size(); i++) {
//			if (!resSelectors.at(i)->callChoice(runtime)) {
//				return false;
//			}
//		}
//		return true;
//	} else {
//		level--;
//		REF(std::vector<int>) ourLevel = allNumbs.at(level);
//		for (int i = 0; i < ourLevel.size(); i++) {
//			runtime->setRelRes(level, ourLevel.at(i));
//			if (getFirst(allNumbs, level, runtime)) return true;
//		}
//	}
//	return false;
//}

REF(RDOValue) RDOSelectResourceCommonCalc::doCalc(PTR(RDORuntime) runtime)
{
	std::vector< std::vector<int> > allNumbs;
	std::vector<int> res;
	for (ruint i = 0; i < resSelectors.size(); i++)
	{
		allNumbs.push_back(resSelectors.at(i)->getPossibleNumbers(runtime));
		res.push_back(runtime->getCurrentActivity()->getResByRelRes(i));
	}
	if (!choice_calc)
	{
		// first
//		if (getFirst(allNumbs, allNumbs.size(), runtime)) {
//			return true;
//		}
		if (getFirst(allNumbs, 0, runtime))
		{
			m_value = 1;
			return m_value;
		}
	}
	else
	{
		// with_min / with_max
		RDOValue bestVal = 0;
		rbool found = false;
		getBest(allNumbs, 0, res, bestVal, runtime, found);
		if (found)
		{
			for (ruint i = 0; i < res.size(); i++)
			{
				runtime->getCurrentActivity()->setRelRes(i, res.at(i));
			}
			m_value = 1;
			return m_value;
		}
	}
	m_value = 0;
	return m_value;
}

std::vector<int> RDOSelectResourceDirectCommonCalc::getPossibleNumbers(PTR(RDORuntime) runtime) const
{
	std::vector<int> res;	
	res.push_back(res_id);
	return res;
}

std::vector<int> RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(PTR(RDORuntime) runtime) const
{
	std::vector<int> res;
	RDORuntime::ResCIterator end = runtime->res_end();
	for (RDORuntime::ResCIterator it = runtime->res_begin(); it != end; it++)
	{
		if (*it == NULL)
			continue;

		if (!(*it)->checkType(resType))
			continue;

		res.push_back((*it)->getTraceID());
	}
	return res;
}

rbool RDOSelectResourceDirectCommonCalc::callChoice(PTR(RDORuntime) runtime) const
{
	return (choice_calc && !choice_calc->calcValue(runtime).getAsBool()) ? false : true;
}

rbool RDOSelectResourceByTypeCommonCalc::callChoice(PTR(RDORuntime) runtime) const
{
	return (choice_calc && !choice_calc->calcValue(runtime).getAsBool()) ? false : true;
}

CLOSE_RDO_RUNTIME_NAMESPACE
