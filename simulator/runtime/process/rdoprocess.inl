/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoprocess.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      24.07.2011
  \brief     Процессы
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlock
// --------------------------------------------------------------------------------
inline RDOPROCBlock::~RDOPROCBlock()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTransact
// --------------------------------------------------------------------------------
inline LPRDOPROCResource RDOPROCTransact::getRes()
{
	return m_res;
}

inline void RDOPROCTransact::setRes(CREF(LPRDOPROCResource) pResource)
{
	m_res = pResource;
}

inline REF(LPIPROCBlock) RDOPROCTransact::getBlock()
{
	return m_block;
}

inline void RDOPROCTransact::setBlock(CREF(LPIPROCBlock) block)
{
	m_block = block;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
inline tstring RDOPROCResource::whoAreYou()
{
	return "procRes";
}

CLOSE_RDO_RUNTIME_NAMESPACE
