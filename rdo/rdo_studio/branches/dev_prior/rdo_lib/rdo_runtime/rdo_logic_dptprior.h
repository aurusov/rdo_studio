/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptprior.h
 * author   : Ћущан ƒмитрий
 * date     : 04.11.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTPRIOR_H_
#define _RDO_LOGIC_DPTPRIOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo_logic.h"
#include "rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODPTOrderPrior
// ----------------------------------------------------------------------------
class RDODPTPriorOrder
{
public:
	static LPIBaseOperation sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container);
};

// ----------------------------------------------------------------------------
// ---------- RDOLogicPrior
// ----------------------------------------------------------------------------
class RDOLogicPrior: public RDOLogic<RDODPTPriorOrder>
{
protected:
	DEFINE_FACTORY(RDOLogicPrior);

	RDOLogicPrior()
		: RDOLogic<RDODPTPriorOrder>()
	{}
	virtual ~RDOLogicPrior()
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
class RDODPTPrior: public RDOLogicPrior, public RDOPatternPrior
{
DEFINE_FACTORY(RDODPTPrior);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogicPrior)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTPrior(RDOSimulator* sim);
	virtual ~RDODPTPrior();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_logic_dptprior.inl"

#endif // _RDO_LOGIC_DPTPRIOR_H_
