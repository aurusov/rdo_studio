// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_memory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
// --------------------------------------------------------------------------------
// -------------------- RDOMemory
// --------------------------------------------------------------------------------
RDOMemory::RDOMemory()
{}

void RDOMemory::createVariable(const std::string& name, const RDOValue& variable)
{
    std::pair<LocalMemory::iterator, bool> result =
        m_localMemory.insert(LocalMemory::value_type(name, variable));

    ASSERT(result.second);
    (void)result;
}

RDOValue RDOMemory::getVariable(const std::string& name) const
{
    LocalMemory::const_iterator it = m_localMemory.find(name);
    ASSERT(it != m_localMemory.end());
    return it->second;
}

void RDOMemory::setVariable(const std::string& name, const RDOValue& variable)
{
    LocalMemory::iterator it = m_localMemory.find(name);
    ASSERT(it != m_localMemory.end());
    it->second = variable;
}

bool RDOMemory::findVariable(const std::string& name) const
{
    return m_localMemory.find(name) != m_localMemory.end();
}

// --------------------------------------------------------------------------------
// -------------------- RDOMemoryStack
// --------------------------------------------------------------------------------
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

void RDOMemoryStack::create(const std::string& name, const RDOValue& variable)
{
    ASSERT(!m_pMemoryStack.empty());

    m_pMemoryStack.back()->createVariable(name, variable);
}

RDOValue RDOMemoryStack::get(const std::string& name) const
{
    ASSERT(!m_pMemoryStack.empty());

    MemoryStack::const_reverse_iterator stack_it = m_pMemoryStack.rbegin();
    while (stack_it != m_pMemoryStack.rend())
    {
        if((*stack_it)->findVariable(name)) break;
        else ++stack_it;
    };

    ASSERT(stack_it != m_pMemoryStack.rend());
    return (*stack_it)->getVariable(name);
}

void RDOMemoryStack::set(const std::string& name, const RDOValue& Variable)
{
    ASSERT(!m_pMemoryStack.empty());

    MemoryStack::reverse_iterator stack_it = m_pMemoryStack.rbegin();
    while (stack_it != m_pMemoryStack.rend())
    {
        if((*stack_it)->findVariable(name)) break;
        else ++stack_it;
    };

    ASSERT(stack_it != m_pMemoryStack.rend());
    (*stack_it)->setVariable(name, Variable);
}

CLOSE_RDO_RUNTIME_NAMESPACE
