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
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_base.h"
#include "converter/smr2rdox/rdoparser_rdo.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdoopr.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdosmr.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdopmd.h"
#include "converter/smr2rdox/rdortp.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
RDOParserItem::RDOParserItem()
	: m_type      (rdoModelObjectsConvertor::PAT_IN)
	, m_parser_fun(NULL                            )
	, m_error_fun (NULL                            )
	, m_lexer_fun (NULL                            )
	, m_needStream(true                            )
{
	init();
}

RDOParserItem::RDOParserItem(rdoModelObjectsConvertor::RDOFileTypeIn type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun)
	: m_type      (type      )
	, m_parser_fun(parser_fun)
	, m_error_fun (error_fun )
	, m_lexer_fun (lexer_fun )
	, m_needStream(true      )
{
	init();
}

RDOParserItem::~RDOParserItem()
{}

void RDOParserItem::init()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainer
// --------------------------------------------------------------------------------
RDOParserContainer::RDOParserContainer()
{}

RDOParserContainer::~RDOParserContainer()
{}

ruint RDOParserContainer::insert(rdoModelObjectsConvertor::RDOParseType type, CREF(LPRDOParserItem) pParser)
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
				index++;
				it++;
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

void RDOParserContainer::getMinMax(rdoModelObjectsConvertor::RDOParseType type, REF(ruint) min, REF(ruint) max)
{
	switch (type)
	{
		case rdoModelObjectsConvertor::obPRE : min = 100;  max = 199;  break;
		case rdoModelObjectsConvertor::obRTP : min = 200;  max = 299;  break;
		case rdoModelObjectsConvertor::obRSS : min = 300;  max = 399;  break;
		case rdoModelObjectsConvertor::obFUN : min = 400;  max = 499;  break;
		case rdoModelObjectsConvertor::obPAT : min = 500;  max = 599;  break;
		case rdoModelObjectsConvertor::obOPR : min = 600;  max = 699;  break;
		case rdoModelObjectsConvertor::obDPT : min = 700;  max = 799;  break;
		case rdoModelObjectsConvertor::obPMD : min = 800;  max = 899;  break;
		case rdoModelObjectsConvertor::obFRM : min = 900;  max = 999;  break;
		case rdoModelObjectsConvertor::obSMR : min = 1000; max = 1099; break;
		case rdoModelObjectsConvertor::obPOST: min = 1100; max = 1199; break;
		default                              : min = ruint(UNDEFINED_ID); max = ruint(UNDEFINED_ID); break;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerModel
// --------------------------------------------------------------------------------
RDOParserContainerModel::RDOParserContainerModel()
	: RDOParserContainer()
{
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserSTDFUN> ::create());
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR_IN, cnv_smr_file_parse, cnv_smr_file_error, cnv_smr_file_lex));
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::RTP_IN, cnv_rtpparse, cnv_rtperror, cnv_rtplex));
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT_IN, cnv_proc_rtp_parse, cnv_proc_rtp_error, cnv_proc_rtp_lex));
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRSS>    ::create());
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT_IN, cnv_proc_rss_parse, cnv_proc_rss_error, cnv_proc_rss_lex));
	insert(rdoModelObjectsConvertor::obFUN, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::FUN_IN, cnv_funparse, cnv_funerror, cnv_funlex));
	insert(rdoModelObjectsConvertor::obPAT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::PAT_IN, cnv_patparse, cnv_paterror, cnv_patlex));
	insert(rdoModelObjectsConvertor::obOPR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::OPR_IN, cnv_oprparse, cnv_oprerror, cnv_oprlex));
	insert(rdoModelObjectsConvertor::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT_IN, cnv_proc_opr_parse, cnv_proc_opr_error, cnv_proc_opr_lex));
	insert(rdoModelObjectsConvertor::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT_IN, cnv_dptparse, cnv_dpterror, cnv_dptlex));
	insert(rdoModelObjectsConvertor::obPMD, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::PMD_IN, cnv_pmdparse, cnv_pmderror, cnv_pmdlex));
	insert(rdoModelObjectsConvertor::obFRM, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::FRM_IN, cnv_frmparse, cnv_frmerror, cnv_frmlex));
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRSSPost>::create());
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR_IN, cnv_smr_sim_parse, cnv_smr_sim_error, cnv_smr_sim_lex));
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerSMRInfo
// --------------------------------------------------------------------------------
RDOParserContainerSMRInfo::RDOParserContainerSMRInfo()
	: RDOParserContainer()
{
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR_IN, cnv_smr_file_parse, cnv_smr_file_error, cnv_smr_file_lex));
}

CLOSE_RDO_CONVERTER_NAMESPACE
