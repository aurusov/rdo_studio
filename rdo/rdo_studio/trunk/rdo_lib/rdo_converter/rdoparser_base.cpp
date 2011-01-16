/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_base.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdoparser_base.h"
#include "rdo_lib/rdo_converter/rdoparser_rdo.h"
#include "rdo_lib/rdo_converter/rdoparser_corba.h"
#include "rdo_lib/rdo_converter/rdopat.h"
#include "rdo_lib/rdo_converter/rdoopr.h"
#include "rdo_lib/rdo_converter/rdodpt.h"
#include "rdo_lib/rdo_converter/rdosmr.h"
#include "rdo_lib/rdo_converter/rdofrm.h"
#include "rdo_lib/rdo_converter/rdopmd.h"
#include "rdo_lib/rdo_converter/rdortp.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserContainer
// ----------------------------------------------------------------------------
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
		default                     : min = UNDEFINED_ID; max = UNDEFINED_ID; break;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerModel
// ----------------------------------------------------------------------------
RDOParserContainerModel::RDOParserContainerModel()
	: RDOParserContainer()
{
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserSTDFUN> ::create());
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR, cnv_smr_file_parse, cnv_smr_file_error, cnv_smr_file_lex));
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::RTP, rtpparse, rtperror, rtplex));
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, proc_rtp_parse, proc_rtp_error, proc_rtp_lex));
#ifdef CORBA_ENABLE
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserCorbaRTP>::create());
#endif
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRSS>    ::create());
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, proc_rss_parse, proc_rss_error, proc_rss_lex));
#ifdef CORBA_ENABLE
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserCorbaRSS>::create());
#endif
	insert(rdoModelObjectsConvertor::obFUN, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::FUN, funparse, funerror, funlex));
	insert(rdoModelObjectsConvertor::obPAT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::PAT, patparse, paterror, patlex));
	insert(rdoModelObjectsConvertor::obOPR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::OPR, oprparse, oprerror, oprlex));
	insert(rdoModelObjectsConvertor::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, proc_opr_parse, proc_opr_error, proc_opr_lex));
	insert(rdoModelObjectsConvertor::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, dptparse, dpterror, dptlex));
	insert(rdoModelObjectsConvertor::obPMD, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::PMD, pmdparse, pmderror, pmdlex));
	insert(rdoModelObjectsConvertor::obFRM, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::FRM, frmparse, frmerror, frmlex));
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRSSPost>::create());
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR, smr_sim_parse, smr_sim_error, smr_sim_lex));
}

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerSMRInfo
// ----------------------------------------------------------------------------
RDOParserContainerSMRInfo::RDOParserContainerSMRInfo()
	: RDOParserContainer()
{
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR, cnv_smr_file_parse, cnv_smr_file_error, cnv_smr_file_lex));
}

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerCorba
// ----------------------------------------------------------------------------
RDOParserContainerCorba::RDOParserContainerCorba()
	: RDOParserContainer()
{
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::RTP, rtpparse, rtperror, rtplex, RDOParserItem::sf_editor));
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, proc_rtp_parse, proc_rtp_error, proc_rtp_lex, RDOParserItem::sf_editor));
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRSS>    ::create(RDOParserItem::sf_editor));
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, proc_rss_parse, proc_rss_error, proc_rss_lex, RDOParserItem::sf_editor));
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRSSPost>::create());
}

CLOSE_RDO_CONVERTER_NAMESPACE
