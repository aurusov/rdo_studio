/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_priority.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      04.11.2009
  \brief     Описание приоритета активностей. Используется при сортировке БЗ.
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PRIORITY_H_
#define _LIB_RUNTIME_PRIORITY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOPatternPrior
  \brief     Приоритет паттерна
*/
class RDOPatternPrior: public IPriority
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPriority)
QUERY_INTERFACE_END

protected:
	RDOPatternPrior();
	virtual ~RDOPatternPrior();

private:
	virtual LPRDOCalc getPrior();
	virtual rbool setPrior(CREF(LPRDOCalc) pPrior);

	LPRDOCalc m_pPrior;
};

/*!
  \class     RDODPTActivityCompare
  \brief     Компаратор приоритетов активностей
*/
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(CREF(LPRDORuntime) pRuntime);
	rbool operator() (CREF(LPIBaseOperation) pOpr1, CREF(LPIBaseOperation) pOpr2);

private:
	LPRDORuntime m_pRuntime;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_priority.inl"

#endif // _LIB_RUNTIME_PRIORITY_H_
