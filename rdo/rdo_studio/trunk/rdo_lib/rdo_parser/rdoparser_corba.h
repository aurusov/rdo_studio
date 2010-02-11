/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_corba.h
 * author   : Раевнев Никита, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CORBA_H_
#define _RDOPARSER_CORBA_H_

#ifdef CORBA_ENABLE

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
public:
	RDOParserCorbaRTP(PTR(RDOParser) parser)
		: RDOParserRDOItem(parser, rdoModelObjects::RTP, NULL, NULL, NULL)
	{}
	virtual void parse();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
public:
	RDOParserCorbaRSS(PTR(RDOParser) parser)
		: RDOParserRDOItem(parser, rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! CORBA_ENABLE

#endif //! _RDOPARSER_CORBA_H_
