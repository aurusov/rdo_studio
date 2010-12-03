/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_memory.h
 * author   : Чирков Михаил
 * date     : 02.12.2010
 * bref     : 
 * indent   : 4T
 */
#ifndef _RDO_MEMORY_H_
#define _RDO_MEMORY_H_

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
#include <string>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLocalMemory
// ----------------------------------------------------------------------------
OBJECT(RDOLocalMemory)
{
DECLARE_FACTORY(RDOLocalMemory);
public:
	typedef std::map<tstring, RDOValue> LocalMemory;

	void     createLocalVarible(CREF(tstring) varibleName);
	void     setLocalVarible(CREF(tstring) varibleName, CREF(RDOValue) varible);
	RDOValue getLocalVarible(CREF(tstring) varibleName) const;

private:
	RDOLocalMemory();
	LocalMemory m_localMemory;

};

// ----------------------------------------------------------------------------
// ---------- RDOMemoryStack
// ----------------------------------------------------------------------------
OBJECT(RDOMemoryStack)
{
DECLARE_FACTORY(RDOMemoryStack);
public:
	typedef std::list<LPRDOLocalMemory> MemoryStack;

	void             pushLocalMemory(LPRDOLocalMemory pMemory);
	LPRDOLocalMemory getLocalMemory();
	void             popLocalMemory();

private:
	RDOMemoryStack();
	MemoryStack m_pMemoryStack;

};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_MEMORY_H_
