/*!
  \copyright (c) RDO-Team, 2012
  \file      assign.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор ASSIGN
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_ASSIGN_H_
#define _LIB_RUNTIME_PROCESS_ASSIGN_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCAssign
  \brief   Процессный блок ASSIGN
*/
class RDOPROCAssign: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCAssign);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

private:
	RDOPROCAssign(LPIPROCProcess process, CREF(LPRDOCalc) pValue, int Id_res, int Id_param);

	LPRDOCalc pParamValue;
	int       t_resId;
	int       t_parId;

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/assign.inl"

#endif // _LIB_RUNTIME_PROCESS_ASSIGN_H_
