/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_corba.h
  \authors   Раевнев Никита
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CORBA_H_
#define _RDOPARSER_CORBA_H_

#ifdef CORBA_ENABLE

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_rdo.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserCorbaRTP
// --------------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRTP);
private:
	RDOParserCorbaRTP()
		: RDOParserRDOItem(rdoModelObjects::RTP, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

// --------------------------------------------------------------------------------
// -------------------- RDOParserCorbaRSS
// --------------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserCorbaRSS);
private:
	RDOParserCorbaRSS()
		: RDOParserRDOItem(rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // CORBA_ENABLE

#endif // _RDOPARSER_CORBA_H_
