/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptfree.inl
 * author   : Ћущан ƒмитрий
 * date     : 07.11.09
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
inline RDODPTFree::RDODPTFree(PTR(RDOSimulator) sim)
	: RDOLogicSimple(sim)
{}

inline RDODPTFree::~RDODPTFree()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
