/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_logic_dptfree.h
 * @authors   Ћущан ƒмитрий
 * @date      07.11.09
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDO_LOGIC_DPTFREE_H_
#define _RDO_LOGIC_DPTFREE_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
// ********************************************************************************

/// @todo избавитьс€ от RDODPTFree

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDODPTFree
// ********************************************************************************
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

#include "rdo_lib/rdo_runtime/rdo_logic_dptfree.inl"

#endif // _RDO_LOGIC_DPTFREE_H_
