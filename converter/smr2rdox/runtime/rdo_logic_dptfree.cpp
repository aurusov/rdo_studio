/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptfree.cpp
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      07.11.2009
  \brief     DPTFree
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/runtime/rdo_logic_dptfree.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTFree
// --------------------------------------------------------------------------------
RDODPTFree::RDODPTFree(const LPRDORuntime& pRuntime)
	: RDOLogicSimple(pRuntime, NULL)
{}

RDODPTFree::~RDODPTFree()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
