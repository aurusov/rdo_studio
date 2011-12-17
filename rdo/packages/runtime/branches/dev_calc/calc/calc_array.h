/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/calc/array.h
  \author    Чирков Михаил
  \date      03.12.2010
  \brief     Агрегатный тип данных - массив
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_ARRAY_H_
#define _LIB_RUNTIME_CALC_ARRAY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcArraySize
  \brief   Размер массива
*/
CALC(RDOCalcArraySize)
{
DECLARE_FACTORY(RDOCalcArraySize)
private:
	RDOCalcArraySize(CREF(LPRDOCalc) pCalc);

	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcArrayItem
  \brief   Элемент массива
*/
CALC(RDOCalcArrayItem)
{
DECLARE_FACTORY(RDOCalcArrayItem)
private:
	RDOCalcArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcSetArrayItem
  \brief   Множество элементов массива
*/
CALC(RDOCalcSetArrayItem)
{
	DECLARE_FACTORY(RDOCalcSetArrayItem)
private:
	RDOCalcSetArrayItem(CREF(LPRDOCalc) pArray, CREF(LPRDOCalc) pArrayInd, CREF(LPRDOCalc) pSetItem);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;
	LPRDOCalc m_pSetItem;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_ARRAY_H_
