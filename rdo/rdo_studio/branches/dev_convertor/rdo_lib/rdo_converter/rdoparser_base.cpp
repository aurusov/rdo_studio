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
// ---------- RDOParserItem
// ----------------------------------------------------------------------------
void RDOParserItem::insertDocUpdate(CREF(LPDocUpdate) pDocUpdate)
{
	ASSERT(pDocUpdate);
	m_docUpdateList.push_back(pDocUpdate);
}

void RDOParserItem::convert(REF(LPDocument) pDocument, REF(std::istream) streamIn) const
{
	ASSERT(pDocument);

	STL_FOR_ALL_CONST(m_docUpdateList, it)
	{
		LPDocUpdate pUpdate = *it;
		ASSERT(pUpdate);
		pUpdate->apply(pDocument, streamIn);
	}
}

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
		default                              : min = UNDEFINED_ID; max = UNDEFINED_ID; break;
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
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::RTP, cnv_rtpparse, cnv_rtperror, cnv_rtplex));
	insert(rdoModelObjectsConvertor::obRTP, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, cnv_proc_rtp_parse, cnv_proc_rtp_error, cnv_proc_rtp_lex));
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRSS>    ::create());
	insert(rdoModelObjectsConvertor::obRSS, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, cnv_proc_rss_parse, cnv_proc_rss_error, cnv_proc_rss_lex));
	insert(rdoModelObjectsConvertor::obFUN, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::FUN, cnv_funparse, cnv_funerror, cnv_funlex));
	insert(rdoModelObjectsConvertor::obPAT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::PAT, cnv_patparse, cnv_paterror, cnv_patlex));
	insert(rdoModelObjectsConvertor::obOPR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::OPR, cnv_oprparse, cnv_oprerror, cnv_oprlex));
	insert(rdoModelObjectsConvertor::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, cnv_proc_opr_parse, cnv_proc_opr_error, cnv_proc_opr_lex));
	insert(rdoModelObjectsConvertor::obDPT, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::DPT, cnv_dptparse, cnv_dpterror, cnv_dptlex));
	insert(rdoModelObjectsConvertor::obPMD, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::PMD, cnv_pmdparse, cnv_pmderror, cnv_pmdlex));
	insert(rdoModelObjectsConvertor::obFRM, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::FRM, cnv_frmparse, cnv_frmerror, cnv_frmlex));
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRSSPost>::create());
	insert(rdoModelObjectsConvertor::obSMR, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR, cnv_smr_sim_parse, cnv_smr_sim_error, cnv_smr_sim_lex));
}

// ----------------------------------------------------------------------------
// ---------- RDOParserContainerSMRInfo
// ----------------------------------------------------------------------------
RDOParserContainerSMRInfo::RDOParserContainerSMRInfo()
	: RDOParserContainer()
{
	insert(rdoModelObjectsConvertor::obPRE, rdo::Factory<RDOParserRDOItem>::create(rdoModelObjectsConvertor::SMR, cnv_smr_file_parse, cnv_smr_file_error, cnv_smr_file_lex));
}

CLOSE_RDO_CONVERTER_NAMESPACE
