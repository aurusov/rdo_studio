/*!
  \copyright (c) RDO-Team, 2012
  \file      terminate.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор TERMINATE
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_TERMINATE_H_
#define _LIB_RUNTIME_PROCESS_TERMINATE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCTerminate
  \brief   Процессный блок TERMINATE
*/
class RDOPROCTerminate: public RDOPROCBlock, public IBaseOperation, public IInternalStatisticsManager, public boost::noncopyable
{
DEFINE_IFACTORY(RDOPROCTerminate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
	QUERY_INTERFACE       (IInternalStatisticsManager)
QUERY_INTERFACE_END

private:
	RDOPROCTerminate(CREF(LPIPROCProcess) pProcess, CREF(LPRDOCalc) pCalc);

	ruint                  m_terminatedTransactCount;
	LPRDOCalc              m_pTermCalc;
	LPIInternalStatistics  m_pStatistics;

	DECLARE_IBaseOperation;
	DECLARE_IInternalStatisticsManager;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/terminate.inl"

#endif // _LIB_RUNTIME_PROCESS_TERMINATE_H_
