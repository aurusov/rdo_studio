/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptprior.h
 * author   : ����� �������
 * date     : 04.11.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTPRIOR_H_
#define _RDO_LOGIC_DPTPRIOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOrderDPTPrior
// ----------------------------------------------------------------------------
class RDOOrderDPTPrior
{
public:
	static LPIBaseOperation sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container);
};

// ----------------------------------------------------------------------------
// ---------- RDOLogicDPTPrior
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
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

#include "rdo_logic_dptprior.inl"

#endif // _RDO_LOGIC_DPTPRIOR_H_
