/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_base.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_base.h"
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/parser/std_fun.h"
#ifdef CORBA_ENABLE
#include "simulator/compiler/parser/rdoparser_corba.h"
#endif
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/rdortp.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
RDOParserItem::RDOParserItem()
    : m_type      (rdoModelObjects::PAT)
    , m_parser_fun(NULL                )
    , m_error_fun (NULL                )
    , m_lexer_fun (NULL                )
    , m_from      (sf_repository       )
{}

RDOParserItem::RDOParserItem(rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from)
    : m_type      (type      )
    , m_parser_fun(parser_fun)
    , m_error_fun (error_fun )
    , m_lexer_fun (lexer_fun )
    , m_from      (from      )
{}

RDOParserItem::~RDOParserItem()
{}

void RDOParserItem::parse(CREF(LPRDOParser) pParser, REF(std::istream) in_stream)
{
    UNUSED(pParser  );
    UNUSED(in_stream);
}

ruint RDOParserItem::lexer_loc_line()
{
    return ruint(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE);
}

ruint RDOParserItem::lexer_loc_pos()
{
    return 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainer
// --------------------------------------------------------------------------------
RDOParserContainer::RDOParserContainer()
{}

RDOParserContainer::~RDOParserContainer()
{}

RDOParserContainer::Iterator RDOParserContainer::begin()
{
    return m_list.begin();
}

RDOParserContainer::Iterator RDOParserContainer::end()
{
    return m_list.end();
}

RDOParserContainer::Iterator RDOParserContainer::find(ruint index)
{
    return m_list.find(index);
}

void RDOParserContainer::clear()
{
	m_list.clear();
}

ruint RDOParserContainer::insert(rdoModelObjects::RDOParseType type, CREF(LPRDOParserItem) pParser)
{
	ASSERT(pParser);

	ruint min, max;
	RDOParserContainer::getMinMax(type, min, max);
	if (min != UNDEFINED_ID && max != UNDEFINED_ID)
	{
		List::iterator it = m_list.find(min);
		if (it == m_list.end())
		{
			m_list[min] = pParser;
			return min;
		}
		else
		{
			ruint index = it->first;
			while (it != m_list.end() && it->first <= max)
			{
				++index;
				++it;
			}
			if (index <= max)
			{
				m_list[index] = pParser;
				return index;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

void RDOParserContainer::getMinMax(rdoModelObjects::RDOParseType type, REF(ruint) min, REF(ruint) max)
{
	switch (type)
	{
		case rdoModelObjects::obPRE : min = 100;  max = 199;  break;
		case rdoModelObjects::obRTP : min = 200;  max = 299;  break;
		case rdoModelObjects::obRSS : min = 300;  max = 399;  break;
		case rdoModelObjects::obFUN : min = 400;  max = 499;  break;
		case rdoModelObjects::obPMD : min = 500;  max = 599;  break;
		case rdoModelObjects::obPRC : min = 600;  max = 699;  break;
		case rdoModelObjects::obEVN : min = 700;  max = 799;  break;
		case rdoModelObjects::obPAT : min = 800;  max = 899;  break;
		case rdoModelObjects::obDPT : min = 900;  max = 999;  break;
		case rdoModelObjects::obFRM : min = 1000; max = 1099; break;
		case rdoModelObjects::obSMR : min = 1100; max = 1199; break;
		case rdoModelObjects::obPOST: min = 1200; max = 1299; break;
		default                     : min = ruint(UNDEFINED_ID); max = ruint(UNDEFINED_ID); break;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerModel
// --------------------------------------------------------------------------------
RDOParserContainerModel::RDOParserContainerModel()
	: RDOParserContainer()
{
	insert(rdoModelObjects::obPRE, rdo::Factory<RDOParserSTDFUN> ::create());
	insert(rdoModelObjects::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_file_parse, smr_file_error, smr_file_lex));
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RTP, rtpparse, rtperror, rtplex));
	insert(rdoModelObjects::obRSS, rdo::Factory<RDOParserRSS>    ::create());
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, proc_rtp_parse, proc_rtp_error, proc_rtp_lex));
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, proc_rss_parse, proc_rss_error, proc_rss_lex));
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserRTPPost>::create());
#ifdef CORBA_ENABLE
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserCorbaRTP>::create());
	insert(rdoModelObjects::obRSS, rdo::Factory<RDOParserCorbaRSS>::create());
#endif
	insert(rdoModelObjects::obFUN, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FUN, funparse, funerror, funlex));
	insert(rdoModelObjects::obPMD, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PMD, pmdparse, pmderror, pmdlex));
	insert(rdoModelObjects::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::EVN, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	insert(rdoModelObjects::obEVN, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::EVN, evnparse, evnerror, evnlex));
	insert(rdoModelObjects::obPAT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PAT, patparse, paterror, patlex));
	insert(rdoModelObjects::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::DPT, dptparse, dpterror, dptlex));
	insert(rdoModelObjects::obPRC, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, proc_opr_parse, proc_opr_error, proc_opr_lex));
	insert(rdoModelObjects::obFRM, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FRM, frmparse, frmerror, frmlex));
	insert(rdoModelObjects::obSMR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_sim_parse, smr_sim_error, smr_sim_lex));
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerSMRInfo
// --------------------------------------------------------------------------------
RDOParserContainerSMRInfo::RDOParserContainerSMRInfo()
	: RDOParserContainer()
{
	insert(rdoModelObjects::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_file_parse, smr_file_error, smr_file_lex));
}

#ifdef CORBA_ENABLE
// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerCorba
// --------------------------------------------------------------------------------
RDOParserContainerCorba::RDOParserContainerCorba()
	: RDOParserContainer()
{
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RTP, rtpparse, rtperror, rtplex, RDOParserItem::sf_editor));
	insert(rdoModelObjects::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::DPT, proc_rtp_parse, proc_rtp_error, proc_rtp_lex, RDOParserItem::sf_editor));
	insert(rdoModelObjects::obRSS, rdo::Factory<RDOParserRSS>    ::create(RDOParserItem::sf_editor));
	insert(rdoModelObjects::obRSS, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::DPT, proc_rss_parse, proc_rss_error, proc_rss_lex, RDOParserItem::sf_editor));
	insert(rdoModelObjects::obSMR, rdo::Factory<RDOParserRSSPost>::create());
}
#endif

CLOSE_RDO_PARSER_NAMESPACE
