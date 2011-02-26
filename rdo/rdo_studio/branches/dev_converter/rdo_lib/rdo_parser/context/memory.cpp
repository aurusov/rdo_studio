/*
 * copyright: (c) RDO-Team, 2010
 * filename : memory.cpp
 * author   : Урусов Андрей
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/memory.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextMemory
// ----------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
	m_pLocalVariableListStack = rdo::Factory<LocalVariableListStack>::create();
	ASSERT(m_pLocalVariableListStack);
}

LPLocalVariableListStack ContextMemory::getLocalMemory()
{
	return m_pLocalVariableListStack;
}

CLOSE_RDO_PARSER_NAMESPACE
