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
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCTerminate
  \brief   Процессный блок TERMINATE
*/
class RDOPROCTerminate: public RDOPROCBlock, public IBaseOperation, public boost::noncopyable
{
DEFINE_IFACTORY(RDOPROCTerminate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	int getTerm();

private:
	RDOPROCTerminate(LPIPROCProcess process, ruint _term);
	const ruint term; 
	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/terminate.inl"

#endif // _LIB_RUNTIME_PROCESS_TERMINATE_H_
