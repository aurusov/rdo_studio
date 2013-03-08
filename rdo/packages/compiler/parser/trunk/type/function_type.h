/*!
  \copyright (c) RDO-Team, 2012
  \file      function_type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.12.2012
  \brief     Тип функции
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_FUNCTION_TYPE_H_
#define _RDOPARSER_TYPE_FUNCTION_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- FunctionParamType
// --------------------------------------------------------------------------------
class FunctionParamType
	: public RDOType
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(FunctionParamType);
public:
	typedef  std::list<LPTypeInfo>  ParamList;

	CREF(ParamList) paramList() const;

private:
	FunctionParamType(CREF(ParamList) paramList, CREF(RDOParserSrcInfo) srcInfo);
	virtual ~FunctionParamType();

	ParamList  m_paramList;

	virtual CREF(rdo::runtime::LPRDOType)    type() const;
	virtual rdo::runtime::RDOType::TypeID  typeID() const;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(FunctionParamType);

// --------------------------------------------------------------------------------
// -------------------- FunctionType
// --------------------------------------------------------------------------------
class FunctionType
	: public RDOType
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(FunctionType);
public:
	CREF(LPTypeInfo)           returnType() const;
	CREF(LPFunctionParamType)  paramType () const;

private:
	FunctionType(CREF(LPTypeInfo) pReturnType, CREF(LPFunctionParamType) pParamType, CREF(RDOParserSrcInfo) srcInfo);
	virtual ~FunctionType();

	LPTypeInfo           m_pReturnType;
	LPFunctionParamType  m_pParamType;

	virtual CREF(rdo::runtime::LPRDOType)    type() const;
	virtual rdo::runtime::RDOType::TypeID  typeID() const;

	DECLARE_IType;
	DECLARE_IModelStructure;
};
DECLARE_POINTER(FunctionType);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_FUNCTION_TYPE_H_
