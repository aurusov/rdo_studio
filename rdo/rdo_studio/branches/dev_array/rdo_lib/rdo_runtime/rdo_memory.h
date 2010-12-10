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
// ---------- RDOMemory
// ----------------------------------------------------------------------------
OBJECT(RDOMemory)
{
DECLARE_FACTORY(RDOMemory);
public:
	typedef std::map<tstring, RDOValue> LocalMemory;

	void     createVariable(CREF(tstring) name);
	RDOValue getVariable   (CREF(tstring) name) const;
	void     setVariable   (CREF(tstring) name, CREF(RDOValue) Variable);
	rbool    findVariable   (CREF(tstring) name) const;

private:
	RDOMemory();

	LocalMemory m_localMemory;
};

// ----------------------------------------------------------------------------
// ---------- RDOMemoryStack
// ----------------------------------------------------------------------------
OBJECT(RDOMemoryStack)
{
DECLARE_FACTORY(RDOMemoryStack);
public:
	typedef std::list<LPRDOMemory> MemoryStack;

	void     push  (LPRDOMemory pMemory);
	void     pop   ();

	void     create(CREF(tstring) name);
	RDOValue get   (CREF(tstring) name) const;
	void     set   (CREF(tstring) name, CREF(RDOValue) Variable);

private:
	RDOMemoryStack();

	MemoryStack m_pMemoryStack;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_MEMORY_H_
