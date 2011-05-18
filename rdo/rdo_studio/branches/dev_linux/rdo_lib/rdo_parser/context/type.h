/*
 * copyright: (c) RDO-Team, 2010
 * filename : type.h
 * author   : Урусов Андрей
 * date     : 30.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_TYPE_H_
#define _RDOPARSER_CONTEXT_TYPE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/info.h"
#include "rdo_lib/rdo_parser/context/context.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ITypeContext
// ----------------------------------------------------------------------------
S_INTERFACE(ITypeContext)
{
	virtual CREF(LPTypeInfo) getTypeInfo() const = 0;
};
#define DECLARE_ITypeContext        \
public:                             \
	CREF(LPTypeInfo) getTypeInfo() const;

// ----------------------------------------------------------------------------
// ---------- TypeContext
// ----------------------------------------------------------------------------
CLASS(TypeContext):
	    INSTANCE_OF      (Context     )
	AND IMPLEMENTATION_OF(ITypeContext)
{
DECLARE_FACTORY(TypeContext);
private:
	TypeContext(CREF(LPTypeInfo) pType);

	LPTypeInfo m_pType;

	DECLARE_ITypeContext;
};
DECLARE_POINTER(TypeContext);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_TYPE_H_
