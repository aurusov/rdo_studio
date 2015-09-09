// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/range/algorithm/find_if.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/function/context_function_param_definition.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextParamDefinition
// --------------------------------------------------------------------------------
ContextParamDefinition::ContextParamDefinition(const OnPushParam& onPushParam)
    : m_onPushParam(onPushParam)
{}

ContextParamDefinition::~ContextParamDefinition()
{}

void ContextParamDefinition::pushParam(const LPRDOParam& pParam)
{
    m_onPushParam(pParam);
}

CLOSE_RDO_PARSER_NAMESPACE
