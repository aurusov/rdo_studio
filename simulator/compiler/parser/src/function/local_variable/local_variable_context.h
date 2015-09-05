#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/src/function/local_variable/local_variable.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class ContextLocalVariable: public Context
{
DECLARE_FACTORY(ContextLocalVariable)
public:
    void pushLocalVariable(const LPLocalVariable& pLocalVariable);
    const rdo::runtime::LPRDOCalcLocalVariableList& calc() const;

private:
    typedef  boost::function<void (const LPLocalVariable&)>  OnPushLocalVariable;

    ContextLocalVariable(const OnPushLocalVariable& onPushLocalVariable);
    virtual ~ContextLocalVariable();

    OnPushLocalVariable                       m_onPushLocalVariable;
    rdo::runtime::LPRDOCalcLocalVariableList  m_pCalcLocalVariableList;
};
DECLARE_POINTER(ContextLocalVariable);

CLOSE_RDO_PARSER_NAMESPACE
