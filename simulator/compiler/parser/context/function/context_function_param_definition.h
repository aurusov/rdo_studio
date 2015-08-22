#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextParamDefinitionManager
// --------------------------------------------------------------------------------
PREDECLARE_OBJECT_INTERFACE(IContextParamDefinitionManager)
struct IContextParamDefinitionManager: public rdo::RefCounter<IContextParamDefinitionManager>
{
DECLARE_FACTORY(IContextParamDefinitionManager)
public:
    virtual void pushParamDefinitionContext() = 0;
    virtual void popParamDefinitionContext () = 0;

protected:
    IContextParamDefinitionManager()
    {}
    virtual ~IContextParamDefinitionManager()
    {}
};

#define DECLARE_IContextParamDefinitionManager \
    void pushParamDefinitionContext();         \
    void popParamDefinitionContext ();

// --------------------------------------------------------------------------------
// -------------------- ContextParamDefinition
// --------------------------------------------------------------------------------
class ContextParamDefinition: public Context
{
DECLARE_FACTORY(ContextParamDefinition)
public:
    void pushParam(const LPRDOParam& pParam);

private:
    typedef  boost::function<void (const LPRDOParam&)>  OnPushParam;

    ContextParamDefinition(const OnPushParam& onPushParam);
    virtual ~ContextParamDefinition();

    OnPushParam m_onPushParam;
};
DECLARE_POINTER(ContextParamDefinition);

CLOSE_RDO_PARSER_NAMESPACE
