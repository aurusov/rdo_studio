#include "pch.h"

#include "rdoparser_base.h"
#include "rdoparser_rdo.h"
#include "rdoparser_corba.h"
#include "rdopat.h"
#include "rdoopr.h"
#include "rdodpt.h"
#include "rdosmr.h"
#include "rdofrm.h"
#include "rdopmd.h"

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserContainer
// ----------------------------------------------------------------------------
RDOParserContainer::RDOParserContainer( RDOParser* parser ):
	RDODeletable( parser )
{
}

int RDOParserContainer::insert( rdoModelObjects::RDOParseType type, RDOParserItem* parser )
{
	int min, max;
	RDOParserContainer::getMinMax( type, min, max );
	if ( min != -1 && max != -1 ) {
		List::iterator it = m_list.find( min );
		if ( it == m_list.end() ) {
			m_list[min] = parser;
			return min;
		} else {
			int index = it->first;
			while ( it != m_list.end() && it->first <= max ) {
				index++;
				it++;
			}
			if ( index <= max ) {
				m_list[index] = parser;
				return index;
			} else {
				return 0;
			}
		}
	}
	return 0;
}

void RDOParserContainer::getMinMax( rdoModelObjects::RDOParseType type, int& min, int& max )
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

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerModel
// ----------------------------------------------------------------------------
RDOParserContainerModel::RDOParserContainerModel( RDOParser* parser ):
	RDOParserContainer( parser )
{
	insert( rdoModelObjects::obPRE, new RDOParserSTDFUN( m_parser ) );
	insert( rdoModelObjects::obPRE, new RDOParserRDOItem( m_parser, rdoModelObjects::SMR, smr_file_parse, smr_file_error, smr_file_lex ) );
	insert( rdoModelObjects::obRTP, new RDOParserRDOItem( m_parser, rdoModelObjects::RTP, rtpparse, rtperror, rtplex ) );
	insert( rdoModelObjects::obRTP, new RDOParserRDOItem( m_parser, rdoModelObjects::DPT, proc_rtp_parse, proc_rtp_error, proc_rtp_lex ) );
#ifdef CORBA_ENABLE
	insert( rdoModelObjects::obRTP, new RDOParserCorbaRTP( m_parser ) );
#endif
	insert( rdoModelObjects::obRSS, new RDOParserRSS( m_parser ) );
	insert( rdoModelObjects::obRSS, new RDOParserRDOItem( m_parser, rdoModelObjects::DPT, proc_rss_parse, proc_rss_error, proc_rss_lex ) );
#ifdef CORBA_ENABLE
	insert( rdoModelObjects::obRSS, new RDOParserCorbaRSS( m_parser ) );
#endif
	insert( rdoModelObjects::obFUN, new RDOParserRDOItem( m_parser, rdoModelObjects::FUN, funparse, funerror, funlex ) );
	insert( rdoModelObjects::obPAT, new RDOParserRDOItem( m_parser, rdoModelObjects::PAT, patparse, paterror, patlex ) );
	insert( rdoModelObjects::obOPR, new RDOParserRDOItem( m_parser, rdoModelObjects::OPR, oprparse, oprerror, oprlex ) );
	insert( rdoModelObjects::obOPR, new RDOParserRDOItem( m_parser, rdoModelObjects::DPT, proc_opr_parse, proc_opr_error, proc_opr_lex ) );
	insert( rdoModelObjects::obDPT, new RDOParserRDOItem( m_parser, rdoModelObjects::DPT, dptparse, dpterror, dptlex ) );
	insert( rdoModelObjects::obPMD, new RDOParserRDOItem( m_parser, rdoModelObjects::PMD, pmdparse, pmderror, pmdlex ) );
	insert( rdoModelObjects::obFRM, new RDOParserRDOItem( m_parser, rdoModelObjects::FRM, frmparse, frmerror, frmlex ) );
	insert( rdoModelObjects::obSMR, new RDOParserRSSPost( m_parser ) );
	insert( rdoModelObjects::obSMR, new RDOParserRDOItem( m_parser, rdoModelObjects::SMR, smr_sim_parse, smr_sim_error, smr_sim_lex ) );
}

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerSMRInfo
// ----------------------------------------------------------------------------
RDOParserContainerSMRInfo::RDOParserContainerSMRInfo( RDOParser* parser ):
	RDOParserContainer( parser )
{
	insert( rdoModelObjects::obPRE, new RDOParserRDOItem( m_parser, rdoModelObjects::SMR, smr_file_parse, smr_file_error, smr_file_lex ) );
}

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerCorba
// ----------------------------------------------------------------------------
RDOParserContainerCorba::RDOParserContainerCorba( RDOParser* parser ):
	RDOParserContainer( parser )
{
	insert( rdoModelObjects::obRTP, new RDOParserRDOItem( m_parser, rdoModelObjects::RTP, rtpparse, rtperror, rtplex, RDOParserItem::sf_editor ) );
	insert( rdoModelObjects::obRTP, new RDOParserRDOItem( m_parser, rdoModelObjects::DPT, proc_rtp_parse, proc_rtp_error, proc_rtp_lex, RDOParserItem::sf_editor ) );
	insert( rdoModelObjects::obRSS, new RDOParserRSS( m_parser, RDOParserItem::sf_editor ) );
	insert( rdoModelObjects::obRSS, new RDOParserRDOItem( m_parser, rdoModelObjects::DPT, proc_rss_parse, proc_rss_error, proc_rss_lex, RDOParserItem::sf_editor ) );
	insert( rdoModelObjects::obSMR, new RDOParserRSSPost( m_parser ) );
}

} // namespace rdoParse
