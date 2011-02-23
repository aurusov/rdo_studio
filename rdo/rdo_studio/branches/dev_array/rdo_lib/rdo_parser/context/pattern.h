/*
 * copyright: (c) RDO-Team, 2010
 * filename : pattern.h
 * author   : Урусов Андрей
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONEXT_PATTERN_H_
#define _CONEXT_PATTERN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/context/memory.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextPattern
// ----------------------------------------------------------------------------
CLASS(ContextPattern): INSTANCE_OF(ContextMemory)
{
DECLARE_FACTORY(ContextPattern);
private:
	ContextPattern();
};
DECLARE_POINTER(ContextPattern);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _CONEXT_PATTERN_H_
