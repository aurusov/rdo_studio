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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContext
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(Context);

S_INTERFACE(IContext)
{};
#define DECLARE_IContext

// ----------------------------------------------------------------------------
// ---------- Context
// ----------------------------------------------------------------------------
OBJECT(Context) IS IMPLEMENTATION_OF(IContext)
{
DECLARE_FACTORY(Context);
protected:
	virtual ~Context();

private:
	DECLARE_IContext;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_H_
