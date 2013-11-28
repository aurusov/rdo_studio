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
#include "simulator/compiler/parser/rdoparser_corba.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/rdortp.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainer
// --------------------------------------------------------------------------------
RDOParserContainer::RDOParserContainer()
{}

RDOParserContainer::~RDOParserContainer()
{}

void RDOParserContainer::clear()
{
	m_list.clear();
}

void RDOParserContainer::insert(CREF(LPRDOParserItem) pParser)
{
	ASSERT(pParser);
	m_list.push_back(pParser);
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerModel
// --------------------------------------------------------------------------------
RDOParserContainerModel::RDOParserContainerModel()
	: RDOParserContainer()
{
	insert(rdo::Factory<RDOParserSTDFUN> ::create());
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_file_parse, smr_file_error, smr_file_lex));

	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RTP, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RTP, evnparse, evnerror, evnlex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RSS, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::RSS, evnparse, evnerror, evnlex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, proc_rtp_parse, proc_rtp_error, proc_rtp_lex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, proc_rss_parse, proc_rss_error, proc_rss_lex));
	insert(rdo::Factory<RDOParserRTPPost>::create());
#ifdef CORBA_ENABLE
	insert(rdo::Factory<RDOParserCorbaRTP>::create());
#endif
#ifdef CORBA_ENABLE
	insert(rdo::Factory<RDOParserCorbaRSS>::create());
#endif
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FUN, funparse, funerror, funlex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::EVN, evn_preparse_parse, evn_preparse_error, evn_preparse_lex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::EVN, evnparse, evnerror, evnlex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PAT, patparse, paterror, patlex));
	insert(rdo::Factory<RDOParserEVNPost>::create());
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::DPT, dptparse, dpterror, dptlex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PRC, proc_opr_parse, proc_opr_error, proc_opr_lex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::FRM, frmparse, frmerror, frmlex));
	insert(rdo::Factory<RDOParserRSSPost>::create());
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::PMD, pmdparse, pmderror, pmdlex));
	insert(rdo::Factory<RDOParserRDOItem>::create(rdoModelObjects::SMR, smr_sim_parse, smr_sim_error, smr_sim_lex));
	insert(rdo::Factory<RDOParserSMRPost>::create());
}

CLOSE_RDO_PARSER_NAMESPACE
