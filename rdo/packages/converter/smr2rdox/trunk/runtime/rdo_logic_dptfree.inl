/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptfree.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      07.11.2009
  \brief     DPTFree
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTFree
// --------------------------------------------------------------------------------
inline RDODPTFree::RDODPTFree(CREF(LPRDORuntime) pRuntime)
	: RDOLogicSimple(pRuntime, NULL)
{}

inline RDODPTFree::~RDODPTFree()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
