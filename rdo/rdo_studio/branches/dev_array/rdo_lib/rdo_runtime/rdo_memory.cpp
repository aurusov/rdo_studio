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
// ---------- RDOMemory
// ----------------------------------------------------------------------------
RDOMemory::RDOMemory()
{}

void RDOMemory::createVariable(CREF(tstring) name)
{
	std::pair<LocalMemory::iterator, rbool> result =
		m_localMemory.insert(LocalMemory::value_type(name, RDOValue()));

	ASSERT(result.second);
}

RDOValue RDOMemory::getVariable(CREF(tstring) name) const
{
	LocalMemory::const_iterator it = m_localMemory.find(name);
	ASSERT(it != m_localMemory.end());
	return it->second;
}

void RDOMemory::setVariable(CREF(tstring) name, CREF(RDOValue) varible)
{
	LocalMemory::iterator it = m_localMemory.find(name);
	ASSERT(it != m_localMemory.end());
	it->second = varible;
}

// ----------------------------------------------------------------------------
// ---------- RDOMemoryStack
// ----------------------------------------------------------------------------
RDOMemoryStack::RDOMemoryStack()
{}

void RDOMemoryStack::push(LPRDOMemory pMemory)
{
	m_pMemoryStack.push_back(pMemory);
}

void RDOMemoryStack::pop()
{
	ASSERT(!m_pMemoryStack.empty());

	m_pMemoryStack.pop_back();
}

LPRDOMemory RDOMemoryStack::top()
{
	ASSERT(!m_pMemoryStack.empty());

	return m_pMemoryStack.back();
}

CLOSE_RDO_RUNTIME_NAMESPACE
