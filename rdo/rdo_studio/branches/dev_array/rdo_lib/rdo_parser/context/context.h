/*
 * copyright: (c) RDO-Team, 2010
 * filename : context.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_H_
#define _RDOPARSER_CONTEXT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"

#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/local_variable.h"

#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOParser;

// ----------------------------------------------------------------------------
// ---------- IContext
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(Context);

S_INTERFACE(IContext)
{
	virtual LPContext             parser        (PTR(RDOParser) pParser, CREF(RDOValue) value) = 0;
	virtual rdoRuntime::LPRDOCalc getCalc       ()                                             = 0;
	virtual LPLocalVariableList   getLocalMemory()                                             = 0;
};

#define DECLARE_IContext                                                                \
	LPContext             parser        (PTR(RDOParser) pParser, CREF(RDOValue) value); \
	rdoRuntime::LPRDOCalc getCalc       ();                                             \
	LPLocalVariableList   getLocalMemory();

// ----------------------------------------------------------------------------
// ---------- Context
// ----------------------------------------------------------------------------
OBJECT(Context) IS IMPLEMENTATION_OF(IContext)
{
public:
	virtual ~Context() {};
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_H_
