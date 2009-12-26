/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_opr.h
 * author   : Ћущан ƒмитрий
 * date     : 05.11.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_OPR_H_
#define _RDO_LOGIC_OPR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo_logic.h"
#include "rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
class RDOOperations: public RDOLogicSimple, public RDOPatternPrior
{
DEFINE_FACTORY(RDOOperations);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDOOperations (RDOSimulator* sim);
	virtual ~RDOOperations();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_logic_opr.inl"

#endif // _RDO_LOGIC_OPR_H_
