/*
 * copyright: (c) RDO-Team, 2011
 * filename : std_fun.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 02.04.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_STD_FUN_H_
#define _RDOPARSER_STD_FUN_H_

// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSTDFUN);
private:
	RDOParserSTDFUN()
		: RDOParserItem(rdoModelObjects::FUN, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_STD_FUN_H_
