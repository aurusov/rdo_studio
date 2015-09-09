// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/context/stack.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Context::FunctionWrapper
// --------------------------------------------------------------------------------
template <class Function>
Context::FunctionWrapper<Function>::FunctionWrapper(const Function& function)
    : function(function)
{}

// --------------------------------------------------------------------------------
// -------------------- Context::Params
// --------------------------------------------------------------------------------
template <class T>
T Context::Params::get(const std::string& name) const
{
    std::map<std::string, boost::any>::const_iterator it = find(name);
    ASSERT(it != end());
    return boost::any_cast<T>(it->second);
}

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
template <class T>
inline rdo::intrusive_ptr<T> Context::cast()
{
    LPContext pThis = this;
    rdo::intrusive_ptr<T> pThisResult = pThis.object_dynamic_cast<T>();
    if (pThisResult)
    {
        return pThisResult;
    }
    LPContext pPrev = m_pContextStack->prev(pThis);
    return pPrev ? pPrev->cast<T>() : rdo::intrusive_ptr<T>();
}

template <class T>
inline rdo::interface_ptr<T> Context::interface_cast()
{
    LPContext pThis = this;
    rdo::interface_ptr<T> pThisResult = pThis.interface_dynamic_cast<T>();
    if (pThisResult)
    {
        return pThisResult;
    }
    LPContext pPrev = m_pContextStack->prev(pThis);
    return pPrev ? pPrev->interface_cast<T>() : rdo::interface_ptr<T>();
}

CLOSE_RDO_PARSER_NAMESPACE
