#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_base.h"
#include "rdoparser_rdo.h"
#include "rdopat.h"
#include "rdoopr.h"
#include "rdodpt.h"
#include "rdosmr.h"
#include "rdofrm.h"
#include "rdopmd.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserList
// ----------------------------------------------------------------------------
RDOParserList::RDOParserList( RDOParser* _parser ):
	parser( _parser )
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
	insertParser( rdoModelObjects::obPRE, new RDOParserSMR1( parser ) );
	insertParser( rdoModelObjects::obPRE, new RDOParserSTDFUN( parser ) );
	insertParser( rdoModelObjects::obRTP, new RDOParserRDO( parser, rdoModelObjects::RTP, rtpparse, rtperror, rtplex ) );
	insertParser( rdoModelObjects::obRSS, new RDOParserRSS( parser ) );
	insertParser( rdoModelObjects::obRSS, new RDOParserRDO( parser, rdoModelObjects::DPT, dpt_rss_parse, dpt_rss_error, dpt_rss_lex ) );
	insertParser( rdoModelObjects::obFUN, new RDOParserRDO( parser, rdoModelObjects::FUN, funparse, funerror, funlex ) );
	insertParser( rdoModelObjects::obPAT, new RDOParserRDO( parser, rdoModelObjects::PAT, patparse, paterror, patlex ) );
	insertParser( rdoModelObjects::obOPR, new RDOParserRDO( parser, rdoModelObjects::OPR, oprparse, oprerror, oprlex ) );
	insertParser( rdoModelObjects::obOPR, new RDOParserRDO( parser, rdoModelObjects::DPT, dpt_opr_parse, dpt_opr_error, dpt_opr_lex ) );
	insertParser( rdoModelObjects::obDPT, new RDOParserRDO( parser, rdoModelObjects::DPT, dptparse, dpterror, dptlex ) );
	insertParser( rdoModelObjects::obPMD, new RDOParserRDO( parser, rdoModelObjects::PMD, pmdparse, pmderror, pmdlex ) );
	insertParser( rdoModelObjects::obFRM, new RDOParserRDO( parser, rdoModelObjects::FRM, frmparse, frmerror, frmlex ) );
	insertParser( rdoModelObjects::obSMR, new RDOParserRSSPost( parser ) );
	insertParser( rdoModelObjects::obSMR, new RDOParserRDO( parser, rdoModelObjects::SMR, smr2parse, smr2error, smr2lex ) );
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
			int index = it->first;
			while ( it != list.end() && it->first <= max ) {
				index++;
				it++;
			}
			if ( index <= max ) {
				list[index] = parser;
				return index;
			} else {
				return 0;
			}
		}
	}
	return 0;
}

void RDOParserList::getParserMinMax( rdoModelObjects::RDOParseType type, int& min, int& max )
{
	switch ( type ) {
		case rdoModelObjects::obPRE : min = 100;  max = 199;  break;
		case rdoModelObjects::obRTP : min = 200;  max = 299;  break;
		case rdoModelObjects::obRSS : min = 300;  max = 399;  break;
		case rdoModelObjects::obFUN : min = 400;  max = 499;  break;
		case rdoModelObjects::obPAT : min = 500;  max = 599;  break;
		case rdoModelObjects::obOPR : min = 600;  max = 699;  break;
		case rdoModelObjects::obDPT : min = 700;  max = 799;  break;
		case rdoModelObjects::obPMD : min = 800;  max = 899;  break;
		case rdoModelObjects::obFRM : min = 900;  max = 999;  break;
		case rdoModelObjects::obSMR : min = 1000; max = 1099; break;
		case rdoModelObjects::obPOST: min = 1100; max = 1199; break;
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
