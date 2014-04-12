/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     Функции с закладки FUN
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_FUNCTION_H_
#define _LIB_RUNTIME_CALC_FUNCTION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовый файл функций с закладки FUN
PREDECLARE_POINTER(RDOFunCalc);
class RDOFunCalc: public RDOCalc
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc);

protected:
	RDOFunCalc();
};

//! Табличная функция
PREDECLARE_POINTER(RDOFuncTableCalc);
class RDOFuncTableCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) pResult);

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgument);

	typedef  std::vector<LPRDOCalcConst>  ResultList;

	ResultList  m_pResultList;
	LPRDOCalc   m_pArgument;

	DECLARE_ICalc;
};

//! Функция типа список
PREDECLARE_POINTER(RDOFunListCalc);
class RDOFunListCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(CREF(LPRDOCalc) pCase, CREF(LPRDOCalcConst) pResult);

private:
	RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue);

	typedef  std::vector<LPRDOCalcConst>  ResultList;

	RDOCalcList     m_caseList;
	ResultList      m_resultList;
	LPRDOCalcConst  m_pDefaultValue;

	DECLARE_ICalc;
};

//! Алгоритмическая функция
PREDECLARE_POINTER(RDOFunAlgorithmicCalc);
class RDOFunAlgorithmicCalc: public RDOFunCalc
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pAction);

protected:
	RDOFunAlgorithmicCalc();

	RDOCalcList  m_conditionList;
	RDOCalcList  m_actionList;

	DECLARE_ICalc;
};

//! Параметр функции
PREDECLARE_POINTER(RDOCalcFuncParam);
class RDOCalcFuncParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(ruint paramID, CREF(RDOSrcInfo) src_info);

	ruint m_paramID;

	DECLARE_ICalc;
};

//! Получение константы с закладки FUN
PREDECLARE_POINTER(RDOCalcGetConst);
class RDOCalcGetConst: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(ruint constantID);

	ruint m_constantID;

	DECLARE_ICalc;
};

//! Инициализация константы
PREDECLARE_POINTER(RDOCalcSetConst);
class RDOCalcSetConst: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(ruint constantID, CREF(LPRDOCalc) pCalc);

	ruint      m_constantID;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

//! Вызов функции (function-caller)
PREDECLARE_POINTER(RDOCalcFunctionCaller);
class RDOCalcFunctionCaller: public RDOCalc
{
DECLARE_FACTORY(RDOCalcFunctionCaller)
public:
	void addParameter   (CREF(LPRDOCalc) pParam   );
	void setFunctionCalc(CREF(LPRDOCalc) pFunction);

	CREF(LPRDOCalc) function() const;

private:
	RDOCalcFunctionCaller(CREF(LPRDOCalc) pFunction);

	RDOCalcList  m_paramList;
	LPRDOCalc    m_pFunction;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_FUNCTION_H_
