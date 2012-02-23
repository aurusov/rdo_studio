/*!
  \copyright (c) RDO-Team, 2011
  \file      result_group_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Интерфейс группы собираемых показателей
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RESULT_GROUP_I_H_
#define _LIB_RUNTIME_RESULT_GROUP_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/result/result_i.h"
// --------------------------------------------------------------------------------

/*!
  \interface IResultGroup
  \brief     Интерфейс группы собираемых показателей
*/
class IResultGroup
{
public:
	virtual void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onAppend(CREF(LPIResult)                pResult ) = 0;
};

#define DECLARE_IResultGroup                                \
	void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onAppend(CREF(LPIResult)                pResult );

#endif // _LIB_RUNTIME_RESULT_GROUP_I_H_
