#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ITypeContext
// --------------------------------------------------------------------------------
struct ITypeContext
{
	virtual const LPTypeInfo& getTypeInfo() const = 0;
};
#define DECLARE_ITypeContext        \
public:                             \
	const LPTypeInfo& getTypeInfo() const;

// --------------------------------------------------------------------------------
// -------------------- TypeContext
// --------------------------------------------------------------------------------
class TypeContext
	: public Context
	, public ITypeContext
{
DECLARE_FACTORY(TypeContext);
private:
	TypeContext(const LPTypeInfo& pType);

	LPTypeInfo m_pType;

	DECLARE_ITypeContext;
};
DECLARE_POINTER(TypeContext);

CLOSE_RDO_PARSER_NAMESPACE
