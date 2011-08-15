/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      statements.h
 * @authors   Чирков Михаил, Лущан Дмитрий
 * @date      16.04.2011
 * @brief     Инструкции
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_STATEMENTS_H_
#define _LIB_RUNTIME_CALC_STATEMENTS_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOCalcNoChange
 * @brief   RDOCalc для константного параметра релевантного ресурса
 *********************************************************************************/
CALC(RDOCalcNoChange)
{
	DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcIf
 * @brief   RDOCalc для сокращенного условного оператора
 *********************************************************************************/
CALC(RDOCalcIf)
{
	DECLARE_FACTORY(RDOCalcIf)
private:
	RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pStatement;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcIfElse
 * @brief   RDOCalc для полного условного оператора
 *********************************************************************************/
CALC(RDOCalcIfElse)
{
	DECLARE_FACTORY(RDOCalcIfElse)
private:
	RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pIfStatement;
	LPRDOCalc m_pElseStatement;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcFor
 * @brief   RDOCalc для циклического оператора for
 *********************************************************************************/
CALC(RDOCalcFor)
{
	DECLARE_FACTORY(RDOCalcFor)
private:
	RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcFunReturn
 * @brief   RDOCalc для оператора выхода из функции
 *********************************************************************************/
CALC(RDOCalcFunReturn)
{
	DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(CREF(LPRDOCalc) pReturn);

	LPRDOCalc m_pReturn;
	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcFunBreak
 * @brief   RDOCalc для оператора выхода из тела цикла
 *********************************************************************************/
CALC(RDOCalcFunBreak)
{
	DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_STATEMENTS_H_
