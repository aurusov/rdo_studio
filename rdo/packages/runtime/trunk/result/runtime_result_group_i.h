/*!
  \copyright (c) RDO-Team, 2011
  \file      runtime_result_group_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Интерфейс группы собираемых показателей
  \indent    4T
*/

#ifndef _LIB_RUNTIME_POKAZ_GROUP_I_H_
#define _LIB_RUNTIME_POKAZ_GROUP_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/result/runtime_result_i.h"
// --------------------------------------------------------------------------------

/*!
  \interface IPokazGroup
  \brief     Интерфейс группы собираемых показателей
*/
class IPokazGroup
{
public:
	virtual void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onAppend(CREF(LPIPokaz)                 pResult ) = 0;
};

#define DECLARE_IPokazGroup                                 \
	void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onAppend(CREF(LPIPokaz)                 pResult );

#endif // _LIB_RUNTIME_POKAZ_GROUP_I_H_
