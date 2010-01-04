/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptsome.inl
 * author   : Ћущан ƒмитрий
 * date     : 04.11.09
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome(PTR(RDOSimulator) sim, LPIBaseOperationContainer parent)
	: RDOLogicSimple(sim, parent)
{
	static_cast<RDOSimulatorTrace*>(sim)->getFreeDPTId();
}

RDODPTSome::~RDODPTSome()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
