/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_select.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Калки команды Select
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_SELECT_H_
#define _LIB_RUNTIME_CALC_RESOURCE_SELECT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_group.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовая групповая функция
CALC_SUB(RDOFunCalcSelect, RDOFunCalcGroup)
{
DECLARE_FACTORY(RDOFunCalcSelect)
public:
	mutable std::list<LPRDOResource> res_list;
	void prepare(CREF(LPRDORuntime) pRuntime);

private:
	RDOFunCalcSelect(int nResType, CREF(LPRDOCalc) pCondition);

	DECLARE_ICalc;
};

//! Базовая класс для операторов Select
CALC_SUB(RDOFunCalcSelectBase, RDOFunCalc)
{
protected:
	RDOFunCalcSelectBase(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition);

	LPRDOFunCalcSelect m_pSelect;
	LPRDOCalc          m_pCondition;
};

/*!
  \def     DEFINE_CALC_SELECT_GROUP
  \brief   Select-группа
*/
#define DEFINE_CALC_SELECT_GROUP(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcSelectBase(pSelect, pCondition) \
	{} \
	DECLARE_ICalc; \
};

/*!
  \def     DEFINE_CALC_SELECT_METHOD
  \brief   Select-метод
*/
#define DEFINE_CALC_SELECT_METHOD(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect) \
		: RDOFunCalcSelectBase(pSelect, NULL) \
	{} \
	DECLARE_ICalc; \
};

/*!
  \class   RDOFunCalcSelectExist
  \brief   Квантор существования
*/
DEFINE_CALC_SELECT_GROUP(Exist);

/*!
  \class   RDOFunCalcSelectNotExist
  \brief   Обратный квантор существования
*/
DEFINE_CALC_SELECT_GROUP (NotExist );

/*!
  \class   RDOFunCalcSelectForAll
  \brief   Проверка условия для каждого элемента множества
*/
DEFINE_CALC_SELECT_GROUP (ForAll   );

/*!
  \class   RDOFunCalcSelectNotForAll
  \brief   Обратная проверка условия для каждого элемента множества
*/
DEFINE_CALC_SELECT_GROUP (NotForAll);

/*!
  \class   RDOFunCalcSelectEmpty
  \brief   Проверка множества на пустоту
*/
DEFINE_CALC_SELECT_METHOD(Empty    );

/*!
  \class   RDOFunCalcSelectSize
  \brief   Размер множества
*/
DEFINE_CALC_SELECT_METHOD(Size     );

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_SELECT_H_
