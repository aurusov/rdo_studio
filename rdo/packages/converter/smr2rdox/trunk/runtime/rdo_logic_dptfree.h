/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptfree.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      07.11.2009
  \brief     DPTFree
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_DPTFREE_H_
#define _LIB_RUNTIME_LOGIC_DPTFREE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/namespace.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDODPTFree
  \brief   Свободная точка принятия решений
  \todo    избавиться от нее
*/
class RDODPTFree: public RDOLogicSimple, public RDOPatternPrior
{
DEFINE_IFACTORY(RDODPTFree);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTFree (CREF(LPRDORuntime) pRuntime);
	virtual ~RDODPTFree();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "converter/smr2rdox/runtime/rdo_logic_dptfree.inl"

#endif // _LIB_RUNTIME_LOGIC_DPTFREE_H_
