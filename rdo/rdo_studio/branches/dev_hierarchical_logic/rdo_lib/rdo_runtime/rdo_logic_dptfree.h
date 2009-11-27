/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptfree.h
 * author   : Ћущан ƒмитрий
 * date     : 07.11.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTFREE_H_
#define _RDO_LOGIC_DPTFREE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo_logic.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
class RDODPTFree: public RDOLogicSimple, public RDOPatternPrior
{
DEFINE_FACTORY(RDODPTFree);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTFree (RDOSimulator* sim);
	virtual ~RDODPTFree();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_logic_dptfree.inl"

#endif // _RDO_LOGIC_DPTFREE_H_
