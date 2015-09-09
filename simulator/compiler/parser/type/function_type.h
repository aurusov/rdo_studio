#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- FunctionParamType
// --------------------------------------------------------------------------------
class FunctionParamType
    : public rdo::runtime::RDOType
    , public IType
    , public IModelStructure
    , public RDOParserSrcInfo
{
DECLARE_FACTORY(FunctionParamType);
public:
    typedef  std::list<LPTypeInfo>  ParamList;

    const ParamList& paramList() const;

private:
    FunctionParamType(const ParamList& paramList, const RDOParserSrcInfo& srcInfo);
    virtual ~FunctionParamType();

    ParamList  m_paramList;

    virtual const rdo::runtime::LPRDOType& type() const;
    virtual rdo::runtime::RDOType::Type typeID() const;

    DECLARE_IType;
    DECLARE_IModelStructure;
};
DECLARE_POINTER(FunctionParamType);

// --------------------------------------------------------------------------------
// -------------------- FunctionType
// --------------------------------------------------------------------------------
class FunctionType
    : public rdo::runtime::RDOType
    , public IType
    , public IModelStructure
    , public RDOParserSrcInfo
{
DECLARE_FACTORY(FunctionType);
public:
    const LPTypeInfo&           returnType() const;
    const LPFunctionParamType&  paramType () const;

private:
    FunctionType(const LPTypeInfo& pReturnType, const LPFunctionParamType& pParamType, const RDOParserSrcInfo& srcInfo);
    virtual ~FunctionType();

    LPTypeInfo           m_pReturnType;
    LPFunctionParamType  m_pParamType;

    virtual rdo::runtime::LPRDOType type() const;
    virtual rdo::runtime::RDOType::Type typeID() const;

    DECLARE_IType;
    DECLARE_IModelStructure;
};
DECLARE_POINTER(FunctionType);

CLOSE_RDO_PARSER_NAMESPACE
