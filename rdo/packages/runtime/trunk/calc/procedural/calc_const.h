/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_const.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     Пустая операция
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \defgroup calc_const Константный калк
  \class    RDOCalcConst
  \brief    Константа
  \details  Возвращает константное значение
  \ingroup  calc calc_const
*/
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
public:
	//! Возвращает хранимую константу, может быть использован в parser-time
	//! \result Значение константы
	CREF(RDOValue) getValue() const;

private:
	//! Инициализируется указанным значением переменную базового класса \ref RDOCalc::m_value
	//! \param value - Значение константы
	RDOCalcConst(CREF(RDOValue) value);

	RDOValue m_value;

	//! Возвращает значение константы
	//! \param pRuntime - Не используется
	//! \result Значение константы
	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
