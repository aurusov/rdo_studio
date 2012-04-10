/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_braces.h
  \author    Чирков Михаил
  \date      16.04.2011
  \brief     Список инструкций в стиле C++
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Открывающая скобка
CALC(RDOCalcOpenBrace)
{
DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECLARE_ICalc;
};

//! Закрывающая скобка
CALC(RDOCalcCloseBrace)
{
DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECLARE_ICalc;
};

//! Вызывает инструкции из списка
CALC(RDOCalcBodyBrace)
{
DECLARE_FACTORY(RDOCalcBodyBrace)
public:
	void addCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcBodyBrace();

	RDOCalcList m_calcList;

	DECLARE_ICalc;
};

//! Вызывает инструкции из списка и обрабатывает команды return и break
CALC_SUB(RDOCalcFunBodyBrace, RDOFunCalc)
{
DECLARE_FACTORY(RDOCalcFunBodyBrace)
public:
	typedef std::vector<LPRDOCalc> CalcFunList;

	void addFunCalc(CREF(LPRDOCalc) pCalc);
	void addRetCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcFunBodyBrace();

	RDOCalcList m_calcFunList;

	DECLARE_ICalc;
};

//! Окончание функции
CALC(RDOCalcFunEnd)
{
DECLARE_FACTORY(RDOCalcFunEnd)
private:
	RDOCalcFunEnd();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_
