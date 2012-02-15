/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptsome.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      04.11.2009
  \brief     DPTSome
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_DPTSOME_H_
#define _LIB_RUNTIME_LOGIC_DPTSOME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDODPTSome
  \brief   Точка принятия решений DPTSome
*/
class RDODPTSome: public RDOLogicSimple, public RDOPatternPrior
{
DEFINE_IFACTORY(RDODPTSome);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogicSimple)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTSome (CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent = NULL);
	virtual ~RDODPTSome();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_logic_dptsome.inl"

#endif // _LIB_RUNTIME_LOGIC_DPTSOME_H_
