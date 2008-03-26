#ifndef RDOPARSER_CORBA_H
#define RDOPARSER_CORBA_H

#include "rdoparser_rdo.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
public:
	RDOParserCorbaRTP( RDOParser* _parser ): RDOParserRDOItem( _parser, rdoModelObjects::RTP, NULL, NULL, NULL ) {};
	virtual void parse( StreamFrom streamFrom = sf_repository );
};

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
public:
	RDOParserCorbaRSS( RDOParser* _parser ): RDOParserRDOItem( _parser, rdoModelObjects::RSS, NULL, NULL, NULL ) {};
	virtual void parse( StreamFrom streamFrom = sf_repository );
};

} // namespace rdoParse

#endif // RDOPARSER_CORBA_H
