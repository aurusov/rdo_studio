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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

CLASS(ContextTimeNow): IMPLEMENTATION_OF(Context)
{
DECLARE_FACTORY(ContextTimeNow);
private:
	rdoRuntime::LPRDOCalc m_pCalc;
	DECLARE_IContext;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/context/timenow.inl"

#endif //! _RDOPARSER_CONTEXT_TIMENOW_H_
