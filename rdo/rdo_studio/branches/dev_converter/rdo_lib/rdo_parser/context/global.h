/*
 * copyright: (c) RDO-Team, 2010
 * filename : global.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_GLOBAL_H_
#define _RDOPARSER_CONTEXT_GLOBAL_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContextGlobal
// ----------------------------------------------------------------------------
S_INTERFACE(IContextGlobal)
{
	virtual LPContext findContext(CREF(RDOValue) value) = 0;
};
#define DECLARE_IContextGlobal                   \
public:                                          \
	LPContext findContext(CREF(RDOValue) value);

// ----------------------------------------------------------------------------
// ---------- ContextGlobal
// ----------------------------------------------------------------------------
CLASS(ContextGlobal): INSTANCE_OF(Context)
{
DECLARE_FACTORY(ContextGlobal);
private:
	ContextGlobal();
	virtual ~ContextGlobal();

	LPContext m_pTimeNow;

	DECLARE_IContextGlobal;
};
DECLARE_POINTER(ContextGlobal);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_GLOBAL_H_
