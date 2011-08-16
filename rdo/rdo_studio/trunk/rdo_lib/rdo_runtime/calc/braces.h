/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      braces.h
 * @author    Чирков Михаил
 * @date      16.04.2011
 * @brief     Список инструкций в стиле C++
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_BRACES_H_
#define _LIB_RUNTIME_CALC_BRACES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/statements.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOCalcOpenBrace
 * @brief   Открывающая скобка
 *********************************************************************************/
CALC(RDOCalcOpenBrace)
{
	DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcCloseBrace
 * @brief   Закрывающая скобка
 *********************************************************************************/
CALC(RDOCalcCloseBrace)
{
	DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcBodyBrace
 * @brief   Тело списка инструкций между скобками
 *********************************************************************************/
CALC(RDOCalcBodyBrace)
{
	DECLARE_FACTORY(RDOCalcBodyBrace)
public:
	typedef std::vector<LPRDOCalc> CalcList;

	void addCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcBodyBrace();

	CalcList m_calcList;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcFunBodyBrace
 * @brief   Функция в теле инструкций
 * @todo    что это?
 *********************************************************************************/
CALC_SUB(RDOCalcFunBodyBrace, RDOFunCalc)
{
	DECLARE_FACTORY(RDOCalcFunBodyBrace)
public:
	typedef std::vector<LPRDOCalc> CalcFunList;

	void addFunCalc(CREF(LPRDOCalc) pCalc);
	void addRetCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcFunBodyBrace();

	CalcFunList m_calcFunList;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcFunEnd
 * @brief   Окончание функции
 * @todo    что это?
 *********************************************************************************/
CALC(RDOCalcFunEnd)
{
	DECLARE_FACTORY(RDOCalcFunEnd)
private:
	RDOCalcFunEnd();

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_BRACES_H_
