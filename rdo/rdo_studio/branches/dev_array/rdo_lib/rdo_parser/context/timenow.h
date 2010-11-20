/*
 * copyright: (c) RDO-Team, 2010
 * filename : timenow.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_TIMENOW_H_
#define _RDOPARSER_CONTEXT_TIMENOW_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/context.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContextTimeNow
// ----------------------------------------------------------------------------
S_INTERFACE(IContextTimeNow)
{
	virtual rdoRuntime::LPRDOCalc getTimeNowCalc() = 0;
};
#define DECLARE_ITypeContext                \
public:                                     \
	rdoRuntime::LPRDOCalc getTimeNowCalc();

// ----------------------------------------------------------------------------
// ---------- ContextTimeNow
// ----------------------------------------------------------------------------
CLASS(ContextTimeNow): IMPLEMENTATION_OF(Context)
{
DECLARE_FACTORY(ContextTimeNow);
private:
	rdoRuntime::LPRDOCalc m_pCalc;

	DECLARE_ITypeContext;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_TIMENOW_H_
