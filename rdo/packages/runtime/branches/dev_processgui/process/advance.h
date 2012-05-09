/*!
  \copyright (c) RDO-Team, 2012
  \file      advance.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор ADVANCE
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_ADVANCE_H_
#define _LIB_RUNTIME_PROCESS_ADVANCE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCAdvance
  \brief   Процессный блок ADVANCE
*/
class RDOPROCAdvance: public RDOPROCBlock, public IBaseOperation, public IInternalStatisticsManager
{
DEFINE_IFACTORY(RDOPROCAdvance);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
	QUERY_INTERFACE       (IInternalStatisticsManager)
QUERY_INTERFACE_END

protected:
	LPRDOCalc pDelayCalc;

	/*!
	  \struct  LeaveTr
	  \brief   Время ухода транзакта
	  \todo    что это?
	*/
	struct LeaveTr
	{
		LPRDOPROCTransact transact;
		double            timeLeave;
		LeaveTr(CREF(LPRDOPROCTransact) _transact, double _timeLeave);
	};
	std::list<LeaveTr> leave_list;

private:
	RDOPROCAdvance(LPIPROCProcess process, CREF(LPRDOCalc) _pDelayCalc);

	int                   m_transCountProc;
	LPIInternalStatistics m_pStatistics;

	DECLARE_IBaseOperation;
	DECLARE_IInternalStatisticsManager;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/advance.inl"

#endif // _LIB_RUNTIME_PROCESS_ADVANCE_H_
