/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptsome.cpp
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      04.11.2009
  \brief     DPTSome
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic_dptsome.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
RDODPTSome::RDODPTSome(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent)
	: RDOLogicSimple(pRuntime, parent)
{
	pRuntime->getFreeDPTId();
}

RDODPTSome::~RDODPTSome()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
