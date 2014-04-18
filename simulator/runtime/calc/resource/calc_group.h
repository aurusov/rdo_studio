/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_group.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     Калки для группы ресурсов одного типа
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_GROUP_H_
#define _LIB_RUNTIME_CALC_RESOURCE_GROUP_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Базовый класс для групповых калков
PREDECLARE_POINTER(RDOFunCalcGroup);
class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, const LPRDOCalc& pCondition);
};

/*!
  \def     DEFINE_CALC_GROUP
  \brief   Групповые калки
*/
#define DEFINE_CALC_GROUP(CalcName)                                 \
PREDECLARE_POINTER(RDOFunCalc##CalcName);                           \
class RDOFunCalc##CalcName: public RDOFunCalcGroup                  \
{                                                                   \
DECLARE_FACTORY(RDOFunCalc##CalcName)                               \
private:                                                            \
	RDOFunCalc##CalcName(int nResType, const LPRDOCalc& pCondition) \
		: RDOFunCalcGroup(nResType, pCondition)                     \
	{}                                                              \
	DECLARE_ICalc;                                                  \
};

/*!
  \class   RDOFunCalcExist
  \brief   Логический предикат существования
*/
DEFINE_CALC_GROUP(Exist);

/*!
  \class   RDOFunCalcNotExist
  \brief   Логический предикат отрицания существования
*/
DEFINE_CALC_GROUP(NotExist);

/*!
  \class   RDOFunCalcForAll
  \brief   Логический предикат общности
*/
DEFINE_CALC_GROUP(ForAll);

/*!
  \class   RDOFunCalcNotForAll
  \brief   Логический предикат отрицания общности
*/
DEFINE_CALC_GROUP(NotForAll);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_GROUP_H_
