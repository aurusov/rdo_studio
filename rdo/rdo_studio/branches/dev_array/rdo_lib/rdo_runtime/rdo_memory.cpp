/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_memory.h
 * author   : Чирков Михаил
 * date     : 02.12.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_memory.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
// ----------------------------------------------------------------------------
// ---------- RDOLocalMemory
// ----------------------------------------------------------------------------
RDOLocalMemory::RDOLocalMemory()
{}

void RDOLocalMemory::createLocalVarible(CREF(tstring) varibleName)
{
	std::pair<LocalMemory::iterator, rbool> result =
		m_localMemory.insert(LocalMemory::value_type(varibleName, RDOValue()));

	ASSERT(result.second);
}

void RDOLocalMemory::setLocalVarible(CREF(tstring) varibleName, CREF(RDOValue) varible)
{
	LocalMemory::iterator it = m_localMemory.find(varibleName);
	ASSERT(it != m_localMemory.end());
	it->second = varible;
}

RDOValue RDOLocalMemory::getLocalVarible(CREF(tstring) varibleName) const
{
	LocalMemory::const_iterator it = m_localMemory.find(varibleName);
	ASSERT(it != m_localMemory.end());
	return it->second;
}

// ----------------------------------------------------------------------------
// ---------- RDOMemoryStack
// ----------------------------------------------------------------------------
RDOMemoryStack::RDOMemoryStack()
{}

void RDOMemoryStack::pushLocalMemory(LPRDOLocalMemory pMemory)
{
	m_pMemoryStack.push_back(pMemory);
}

LPRDOLocalMemory RDOMemoryStack::getLocalMemory()
{
	return m_pMemoryStack.back();
}

void RDOMemoryStack::popLocalMemory()
{
	m_pMemoryStack.pop_back();
}

CLOSE_RDO_RUNTIME_NAMESPACE
