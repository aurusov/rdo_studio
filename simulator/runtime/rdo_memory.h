/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_memory.h
  \author    Чирков Михаил
  \date      02.12.2010
  \brief     Память
  \indent    4T
*/

#ifndef _LIB_RUNTIME_MEMORY_H_
#define _LIB_RUNTIME_MEMORY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <map>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOMemory
  \brief     Память
*/
PREDECLARE_POINTER(RDOMemory);
class RDOMemory: public rdo::counter_reference
{
DECLARE_FACTORY(RDOMemory);
public:
	typedef std::map<std::string, RDOValue> LocalMemory;

	void createVariable(CREF(std::string) name, CREF(RDOValue) variable);
	RDOValue getVariable(CREF(std::string) name) const;
	void setVariable(CREF(std::string) name, CREF(RDOValue) variable);
	bool findVariable(CREF(std::string) name) const;

private:
	RDOMemory();

	LocalMemory m_localMemory;
};

/*!
  \class     RDOMemoryStack
  \brief     Стэк памяти
*/
PREDECLARE_POINTER(RDOMemoryStack);
class RDOMemoryStack: public rdo::counter_reference
{
DECLARE_FACTORY(RDOMemoryStack);
public:
	typedef std::list<LPRDOMemory> MemoryStack;

	void     push  (LPRDOMemory pMemory);
	void     pop   ();

	void create(CREF(std::string) name, CREF(RDOValue) variable);
	RDOValue get(CREF(std::string) name) const;
	void set(CREF(std::string) name, CREF(RDOValue) variable);

private:
	RDOMemoryStack();

	MemoryStack m_pMemoryStack;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_MEMORY_H_
