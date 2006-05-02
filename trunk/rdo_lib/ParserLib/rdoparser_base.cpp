#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_base.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdoopr.h"
#include "rdodpt.h"
#include "rdosmr.h"
#include "rdofrm.h"
#include "rdopmd.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserBase
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- RDOParserList
// ----------------------------------------------------------------------------
RDOParserList::RDOParserList()
{
}

RDOParserList::~RDOParserList()
{
	clear();
}

void RDOParserList::clear()
{
	std::map< int, RDOParserBase* >::reverse_iterator it = list.rbegin();
	while ( it != list.rend() ) {
		delete it->second;
		it++;
	}
	list.clear();
}

void RDOParserList::reset()
{
	clear();
	insertParser( rdoModelObjects::obPRE, new RDOParserSMR1( smr1parse, smr1error, smr1lex ) );
	insertParser( rdoModelObjects::obRTP, new RDOParserRDO( rdoModelObjects::RTP, rtpparse, rtperror, rtplex ) );
	insertParser( rdoModelObjects::obRSS, new RDOParserRDO( rdoModelObjects::RSS, rssparse, rsserror, rsslex ) );
	insertParser( rdoModelObjects::obFUN, new RDOParserFUN( funparse, funerror, funlex ) );
	insertParser( rdoModelObjects::obPAT, new RDOParserRDO( rdoModelObjects::PAT, patparse, paterror, patlex ) );
	insertParser( rdoModelObjects::obOPR, new RDOParserRDO( rdoModelObjects::OPR, oprparse, oprerror, oprlex ) );
	insertParser( rdoModelObjects::obDPT, new RDOParserRDO( rdoModelObjects::DPT, dptparse, dpterror, dptlex ) );
	insertParser( rdoModelObjects::obPMD, new RDOParserRDO( rdoModelObjects::PMD, pmdparse, pmderror, pmdlex ) );
	insertParser( rdoModelObjects::obFRM, new RDOParserRDO( rdoModelObjects::FRM, frmparse, frmerror, frmlex ) );
	insertParser( rdoModelObjects::obSMR, new RDOParserRDO( rdoModelObjects::SMR, smr2parse, smr2error, smr2lex ) );
}

int RDOParserList::insertParser( rdoModelObjects::RDOParseType type, RDOParserBase* parser )
{
	int min, max;
	RDOParserList::getParserMinMax( type, min, max );
	if ( min != -1 && max != -1 ) {
		std::map< int, RDOParserBase* >::iterator it = list.find( min );
		if ( it == list.end() ) {
			list[min] = parser;
			return min;
		} else {
			it == list.lower_bound( max + 1 );
			if ( it == list.end() ) return 0;
			int index = it->first + 1;
			list[index] = parser;
			return index;
		}
	}
	return 0;
}

void RDOParserList::getParserMinMax( rdoModelObjects::RDOParseType type, int& min, int& max )
{
	switch ( type ) {
		case rdoModelObjects::obPRE : min = 1;    max = 100; break;
		case rdoModelObjects::obRTP : min = 101;  max = 200; break;
		case rdoModelObjects::obRSS : min = 201;  max = 300; break;
		case rdoModelObjects::obFUN : min = 301;  max = 400; break;
		case rdoModelObjects::obPAT : min = 401;  max = 500; break;
		case rdoModelObjects::obOPR : min = 501;  max = 600; break;
		case rdoModelObjects::obDPT : min = 601;  max = 700; break;
		case rdoModelObjects::obPMD : min = 701;  max = 800; break;
		case rdoModelObjects::obFRM : min = 801;  max = 900; break;
		case rdoModelObjects::obSMR : min = 901;  max = 1000; break;
		case rdoModelObjects::obPOST: min = 1001; max = 1100; break;
		default: min = -1; max = -1; break;
	}
}

std::list< rdoModelObjects::RDOFileType > RDOParserList::getParserFiles( int files )
{
	std::list< rdoModelObjects::RDOFileType > list;
	if ( files & rdoModelObjects::obRTP ) list.push_back( rdoModelObjects::RTP );
	if ( files & rdoModelObjects::obRSS ) list.push_back( rdoModelObjects::RSS );
	if ( files & rdoModelObjects::obFUN ) list.push_back( rdoModelObjects::FUN );
	if ( files & rdoModelObjects::obPAT ) list.push_back( rdoModelObjects::PAT );
	if ( files & rdoModelObjects::obOPR ) list.push_back( rdoModelObjects::OPR );
	if ( files & rdoModelObjects::obDPT ) list.push_back( rdoModelObjects::DPT );
	if ( files & rdoModelObjects::obPMD ) list.push_back( rdoModelObjects::PMD );
	if ( files & rdoModelObjects::obFRM ) list.push_back( rdoModelObjects::FRM );
	if ( files & rdoModelObjects::obSMR ) list.push_back( rdoModelObjects::SMR );
	return list;
}

} // namespace rdoParse
