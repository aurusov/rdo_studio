/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_logic_dptprior.h
 * @authors   Ћущан ƒмитрий
 * @date      04.11.2009
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_LOGIC_DPTPRIOR_
#define _LIB_RUNTIME_LOGIC_DPTPRIOR_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOOrderDPTPrior
 * @brief   unknown
 *********************************************************************************/
class RDOOrderDPTPrior
{
public:
	static LPIBaseOperation sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container);
};

/******************************************************************************//**
 * @class   RDOLogicDPTPrior
 * @brief   unknown
 *********************************************************************************/
class RDOLogicDPTPrior: public RDOLogic<RDOOrderDPTPrior>
{
protected:
	DEFINE_IFACTORY(RDOLogicDPTPrior);

	RDOLogicDPTPrior(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent)
		: RDOLogic<RDOOrderDPTPrior>(pRuntime, parent)
	{}
	virtual ~RDOLogicDPTPrior()
	{}
};

/******************************************************************************//**
 * @class   RDOLogicDPTPrior
 * @brief   unknown
 *********************************************************************************/
class RDODPTPrior: public RDOLogicDPTPrior, public RDOPatternPrior
{
DEFINE_IFACTORY(RDODPTPrior);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogicDPTPrior)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTPrior(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent = NULL);
	virtual ~RDODPTPrior();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_logic_dptprior.inl"

#endif // _LIB_RUNTIME_LOGIC_DPTPRIOR_
