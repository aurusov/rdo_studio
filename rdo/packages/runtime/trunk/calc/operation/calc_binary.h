/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_binary.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Бинарные операторы
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_BINARY_H_
#define _LIB_RUNTIME_CALC_OPERATION_BINARY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовый класс для бинарных операторов
class RDOCalcBinaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(LPRDOCalc) pFirst, CREF(LPRDOCalc) pSecond);

protected:
	RDOCalcBinaryBase(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;
};

/*!
  \class   RDOCalcBinary
  \tparam  ret_type  Возвращаемое значение
  \tparam  pOperator Оператор
  \tparam  CalcType  Тип бинарного оператора
  \brief   Бинарный оператор
*/
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue)) const, typename OperatorType::Type CalcType>
class RDOCalcBinary: public RDOCalcBinaryBase
{
friend class rdo::Factory<RDOCalcBinary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)(CREF(RDOValue)) const;

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

	static RDOSrcInfo     getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);
	static value_operator getOperation    ();

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

private:
	//! Выполняет бинарную операцию \ref value_operator над \ref m_pLeft и \ref m_pRight
	//! \param pRuntime - указатель на объект runtime'а.
	//!                   Используется для доступа к БД модели, системному времени, генерации ошибок и др.
	//! \exception RDORuntimeException
	//! \result Вычесленное калком значение
	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_binary.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_BINARY_H_
