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
#include "simulator/runtime/rdo_array.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовая групповая функция
PREDECLARE_POINTER(RDOFunCalcSelect);
class RDOFunCalcSelect: public RDOFunCalcGroup
{
DECLARE_FACTORY(RDOFunCalcSelect)
public:
	mutable std::list<LPRDOResource> res_list;
	void prepare(const LPRDORuntime& pRuntime);

	const LPIResourceType& getResType();

private:
	RDOFunCalcSelect(const LPIResourceType& pResType, int nResType, const LPRDOCalc& pCondition);

	LPIResourceType m_pResType;

	DECLARE_ICalc;
};

//! Базовая класс для операторов Select
PREDECLARE_POINTER(RDOFunCalcSelectBase);
class RDOFunCalcSelectBase: public RDOFunCalc
{
protected:
	RDOFunCalcSelectBase(const LPRDOFunCalcSelect& pSelect, const LPRDOCalc& pCondition);

	LPRDOFunCalcSelect m_pSelect;
	LPRDOCalc          m_pCondition;
};

/*!
  \def     DEFINE_CALC_SELECT_GROUP
  \brief   Select-группа
*/
#define DEFINE_CALC_SELECT_GROUP(CalcName)                    \
PREDECLARE_POINTER(RDOFunCalcSelect##CalcName);               \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase \
{                                                             \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName)                   \
private:                                                      \
	RDOFunCalcSelect##CalcName(const LPRDOFunCalcSelect& pSelect, const LPRDOCalc& pCondition) \
		: RDOFunCalcSelectBase(pSelect, pCondition)           \
	{}                                                        \
	DECLARE_ICalc;                                            \
};

/*!
  \def     DEFINE_CALC_SELECT_METHOD
  \brief   Select-метод
*/
#define DEFINE_CALC_SELECT_METHOD(CalcName)                      \
PREDECLARE_POINTER(RDOFunCalcSelect##CalcName);                  \
class RDOFunCalcSelect##CalcName: public RDOFunCalcSelectBase    \
{                                                                \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName)                      \
private:                                                         \
	RDOFunCalcSelect##CalcName(const LPRDOFunCalcSelect& pSelect) \
		: RDOFunCalcSelectBase(pSelect, NULL)                    \
	{}                                                           \
	DECLARE_ICalc;                                               \
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

/*!
  \class   RDOFunCalcSelectArray
  \brief   Получение массива из множества
*/
DEFINE_CALC_SELECT_METHOD(Array    );

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_SELECT_H_
