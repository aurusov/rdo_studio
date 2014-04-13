/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_relevant.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.12.2009
  \brief     RDOCalc для подбора релевантных ресурсов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_RELEVANT_H_
#define _LIB_RUNTIME_CALC_RESOURCE_RELEVANT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Получить ресурс по ID релевантного ресурса
PREDECLARE_POINTER(RDOGetResourceByRelevantResourceID);
class RDOGetResourceByRelevantResourceID: public RDOCalc
{
DECLARE_FACTORY(RDOGetResourceByRelevantResourceID)
private:
	RDOGetResourceByRelevantResourceID(ruint relevantResourceID);

	ruint m_relevantResourceID;

	DECLARE_ICalc;
};

//! Удаление релевантного ресурса
PREDECLARE_POINTER(RDOEraseResRelCalc);
class RDOEraseResRelCalc: public RDOCalc
{
DECLARE_FACTORY(RDOEraseResRelCalc)
public:
	CREF(std::string) getName() const;

private:
	RDOEraseResRelCalc(ruint relResID, CREF(std::string) relResName);

	ruint m_relResID;
	std::string m_relResName;

	DECLARE_ICalc;
};

//! Возвращает ресурс групповой функции
PREDECLARE_POINTER(RDOCalcGetGroupFunctionResource);
class RDOCalcGetGroupFunctionResource: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetGroupFunctionResource)
private:
	RDOCalcGetGroupFunctionResource();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_RELEVANT_H_
