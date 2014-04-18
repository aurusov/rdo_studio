/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_array.h
  \author    Чирков Михаил
  \date      03.12.2010
  \brief     Агрегатный тип данных - массив
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_ARRAY_H_
#define _LIB_RUNTIME_CALC_ARRAY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Размер массива
PREDECLARE_POINTER(RDOCalcArraySize);
class RDOCalcArraySize: public RDOCalc
{
DECLARE_FACTORY(RDOCalcArraySize)
private:
	RDOCalcArraySize(const LPRDOCalc& pCalc);

	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

//! Возвращает элемент массива
PREDECLARE_POINTER(RDOCalcArrayItem);
class RDOCalcArrayItem: public RDOCalc
{
DECLARE_FACTORY(RDOCalcArrayItem)
private:
	RDOCalcArrayItem(const LPRDOCalc& pArray, const LPRDOCalc& pArrayInd);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;

	DECLARE_ICalc;
};

//! Устанавливает элемент массива
PREDECLARE_POINTER(RDOCalcSetArrayItem);
class RDOCalcSetArrayItem: public RDOCalc
{
	DECLARE_FACTORY(RDOCalcSetArrayItem)
private:
	RDOCalcSetArrayItem(const LPRDOCalc& pArray, const LPRDOCalc& pArrayInd, const LPRDOCalc& pSetItem);

	LPRDOCalc m_pArray;
	LPRDOCalc m_pArrayInd;
	LPRDOCalc m_pSetItem;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_ARRAY_H_
