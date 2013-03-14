/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_operation.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     Операции
  \indent    4T
*/

#ifndef _LIB_RUNTIME_OPERATION_H_
#define _LIB_RUNTIME_OPERATION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/rdo_operation_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOOperation
  \brief     Операция - модифицированное продукционное правило
*/
class RDOOperation: public IBaseOperation, public IOperation, public RDOActivityPattern<RDOPatternOperation>, public RDOPatternPrior, public IOperationTrace
{
typedef RDOActivityPattern<RDOPatternOperation> pattern_type;
DEFINE_IFACTORY(RDOOperation);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IOperation)
	QUERY_INTERFACE(IOperationTrace)
QUERY_INTERFACE_END

friend class RDOTrace;
friend class RDOSimulatorTrace;
friend class RDOSimulator;

protected:
	RDOOperation(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternOperation) pPattern, rbool trace, CREF(tstring) name);
	RDOOperation(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternOperation) pPattern, rbool trace, CREF(LPRDOCalc) pCondition, CREF(tstring) name);
	virtual ~RDOOperation();
	DECLARE_IOperation;

private:
	RDOOperation(CREF(LPRDORuntime) pRuntime, CREF(RDOOperation) originForClone);

	typedef std::vector<LPIOperation> CloneList;

	CloneList m_cloneList;
	rbool     m_haveAdditionalCondition;
	LPRDOCalc m_pAdditionalCondition;
	int       m_operId;

	DECLARE_IBaseOperation;
	DECLARE_IOperationTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_OPERATION_H_
