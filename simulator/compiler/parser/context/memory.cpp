// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextMemory
// --------------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
    m_pLocalVariableListStack = rdo::Factory<LocalVariableListStack>::create();
    ASSERT(m_pLocalVariableListStack);
}

LPLocalVariableListStack ContextMemory::getLocalMemory()
{
    return m_pLocalVariableListStack;
}

Context::LPFindResult ContextMemory::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
    if (method == Context::METHOD_OPERATOR_DOT)
    {
        LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(params.identifier());
        if (pLocalVariable)
            return rdo::Factory<FindResult>::create(SwitchContext(pLocalVariable, params));
    }

    if (method == Context::METHOD_GET)
    {
        LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(params.identifier());
        if (pLocalVariable)
            return pLocalVariable->find(Context::METHOD_GET, params, srcInfo);
    }

    if (method == Context::METHOD_SET)
    {
        LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(params.identifier());
        if (pLocalVariable)
            return pLocalVariable->find(Context::METHOD_SET, params, srcInfo);
    }

    return rdo::Factory<FindResult>::create();
}

void ContextMemory::push()
{
    LPContext pContext = RDOParser::s_parser()->context();
    ASSERT(pContext);
    LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
    ASSERT(pContextMemory);

    LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
    ASSERT(pLocalVariableListStack);

    LPLocalVariableList pLocalVariableList = rdo::Factory<LocalVariableList>::create();
    ASSERT(pLocalVariableList);
    pLocalVariableListStack->push(pLocalVariableList);
}

void ContextMemory::pop()
{
    LPContext pContext = RDOParser::s_parser()->context();
    ASSERT(pContext);
    LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
    ASSERT(pContextMemory);

    LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
    ASSERT(pLocalVariableListStack);

    pLocalVariableListStack->pop();
}

CLOSE_RDO_PARSER_NAMESPACE