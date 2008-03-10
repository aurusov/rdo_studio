#ifndef RDOPARSER_CORBA_H
#define RDOPARSER_CORBA_H

#include "rdoparser_rdo.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDO
{
public:
	RDOParserCorbaRTP( RDOParser* _parser ): RDOParserRDO( _parser, rdoModelObjects::RTP, NULL, NULL, NULL ) {};
	virtual void parse();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDO
{
public:
	RDOParserCorbaRSS( RDOParser* _parser ): RDOParserRDO( _parser, rdoModelObjects::RSS, NULL, NULL, NULL ) {};
	virtual void parse();
};

} // namespace rdoParse

#endif // RDOPARSER_CORBA_H
