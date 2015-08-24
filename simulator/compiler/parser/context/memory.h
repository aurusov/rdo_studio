#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/function/local_variable/local_variable.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextMemory
// --------------------------------------------------------------------------------
struct IContextMemory
{
    virtual LPLocalVariableListStack getLocalMemory() = 0;
};
#define DECLARE_IContextMemory               \
public:                                      \
    LPLocalVariableListStack getLocalMemory();

// --------------------------------------------------------------------------------
// -------------------- ContextMemory
// --------------------------------------------------------------------------------
class ContextMemory
    : public Context
    , public IContextMemory
    , public IContextFind
{
DECLARE_FACTORY(ContextMemory);
public:
    static void push();
    static void pop ();

protected:
    ContextMemory();

private:
    LPLocalVariableListStack m_pLocalVariableListStack;

    DECLARE_IContextMemory;
    virtual Context::LPFindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(ContextMemory);

CLOSE_RDO_PARSER_NAMESPACE
