/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptsome.h
 * author   : Ћущан ƒмитрий
 * date     : 04.11.09
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTSOME_H_
#define _RDO_LOGIC_DPTSOME_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo_logic.h"
#include "rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogicSimple, public RDOPatternPrior
{
DEFINE_FACTORY(RDODPTSome);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	RDODPTSome (RDOSimulator* sim);
	virtual ~RDODPTSome();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_logic_dptsome.inl"

#endif // _RDO_LOGIC_DPTSOME_H_
