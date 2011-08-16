/******************************************************************************//**
 * @copyright (c) RDO-Team, 2010
 * @file      array.h
 * @author    Чирков Михаил
 * @date      03.12.2010
 * @brief     Агрегатный тип данных - массив
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_ARRAY_H_
#define _LIB_RUNTIME_CALC_ARRAY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOCalcArraySize
 * @brief   Размер массива
 *********************************************************************************/
CALC(RDOCalcArraySize)
{
DECLARE_FACTORY(RDOCalcArraySize)
private:
	RDOCalcArraySize(CREF(LPRDOCalc) pArray);

	LPRDOCalc m_pArray;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcArrayItem
 * @brief   Элемент массива
 *********************************************************************************/
CALC(RDOCalcArrayItem)
{
DECLARE_FACTORY(RDOCalcArrayItem)
private:
	RDOCalcArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcSetArrayItem
 * @brief   Множество элементов массива
 *********************************************************************************/
CALC(RDOCalcSetArrayItem)
{
	DECLARE_FACTORY(RDOCalcSetArrayItem)
private:
	RDOCalcSetArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd, CREF(LPRDOCalc) pSetItem);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;
	LPRDOCalc m_pSetItem;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_ARRAY_H_
