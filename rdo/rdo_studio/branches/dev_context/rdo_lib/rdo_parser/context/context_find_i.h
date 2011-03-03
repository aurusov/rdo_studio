/*
 * copyright: (c) RDO-Team, 2011
 * filename : context_find_i.h
 * author   : Урусов Андрей
 * date     : 03.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_FIND_I_H_
#define _RDOPARSER_CONTEXT_FIND_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"

#include "rdo_lib/rdo_parser/context/context.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IContextFind
// ----------------------------------------------------------------------------
OBJECT_INTERFACE(IContextFind)
{
DECLARE_FACTORY(IContextFind)
public:
	virtual LPContext find(CREF(tstring) name) const = 0;
};
#define DECLARE_IContextFind \
	LPContext find(CREF(tstring) name) const;

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_FIND_I_H_
