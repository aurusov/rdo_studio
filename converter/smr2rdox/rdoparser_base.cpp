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

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserItem
// --------------------------------------------------------------------------------
RDOParserItem::RDOParserItem()
    : m_type(rdo::converter::smr2rdox::FileTypeIn::PAT)
    , m_parser_fun(NULL)
    , m_lexer_fun (NULL)
    , m_needStream(true)
{
    init();
}

RDOParserItem::RDOParserItem(rdo::converter::smr2rdox::FileTypeIn type, t_bison_parse_fun parser_fun, t_flex_lexer_fun lexer_fun)
    : m_type      (type      )
    , m_parser_fun(parser_fun)
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

std::size_t RDOParserContainer::insert(rdo::converter::smr2rdox::ParseType type, const LPRDOParserItem& pParser)
{
    ASSERT(pParser);

    std::size_t min, max;
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
            std::size_t index = it->first;
            while (it != m_list.end() && it->first <= max)
            {
                index++;
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

void RDOParserContainer::getMinMax(rdo::converter::smr2rdox::ParseType type, std::size_t& min, std::size_t& max)
{
    switch (type)
    {
        case rdo::converter::smr2rdox::ParseType::PRE : min = 100;  max = 199;  break;
        case rdo::converter::smr2rdox::ParseType::RTP : min = 200;  max = 299;  break;
        case rdo::converter::smr2rdox::ParseType::RSS : min = 300;  max = 399;  break;
        case rdo::converter::smr2rdox::ParseType::FUN : min = 400;  max = 499;  break;
        case rdo::converter::smr2rdox::ParseType::PAT : min = 500;  max = 599;  break;
        case rdo::converter::smr2rdox::ParseType::OPR : min = 600;  max = 699;  break;
        case rdo::converter::smr2rdox::ParseType::DPT : min = 700;  max = 799;  break;
        case rdo::converter::smr2rdox::ParseType::PMD : min = 800;  max = 899;  break;
        case rdo::converter::smr2rdox::ParseType::FRM : min = 900;  max = 999;  break;
        case rdo::converter::smr2rdox::ParseType::SMR : min = 1000; max = 1099; break;
        case rdo::converter::smr2rdox::ParseType::POST: min = 1100; max = 1199; break;
        default: min = std::size_t(UNDEFINED_ID); max = std::size_t(UNDEFINED_ID); break;
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerModel
// --------------------------------------------------------------------------------
RDOParserContainerModel::RDOParserContainerModel()
    : RDOParserContainer()
{
    insert(rdo::converter::smr2rdox::ParseType::PRE, rdo::Factory<RDOParserSTDFUN> ::create());
    insert(rdo::converter::smr2rdox::ParseType::PRE, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::SMR, cnv_smr_file_parse, cnv_smr_file_lex));
    insert(rdo::converter::smr2rdox::ParseType::RTP, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::RTP, cnv_rtpparse, cnv_rtplex));
    insert(rdo::converter::smr2rdox::ParseType::RTP, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::DPT, cnv_proc_rtp_parse, cnv_proc_rtp_lex));
    insert(rdo::converter::smr2rdox::ParseType::RSS, rdo::Factory<RDOParserRSS>    ::create());
    insert(rdo::converter::smr2rdox::ParseType::RSS, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::DPT, cnv_proc_rss_parse, cnv_proc_rss_lex));
    insert(rdo::converter::smr2rdox::ParseType::FUN, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::FUN, cnv_funparse, cnv_funlex));
    insert(rdo::converter::smr2rdox::ParseType::PAT, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::PAT, cnv_patparse, cnv_patlex));
    insert(rdo::converter::smr2rdox::ParseType::OPR, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::OPR, cnv_oprparse, cnv_oprlex));
    insert(rdo::converter::smr2rdox::ParseType::DPT, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::DPT, cnv_proc_opr_parse, cnv_proc_opr_lex));
    insert(rdo::converter::smr2rdox::ParseType::DPT, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::DPT, cnv_dptparse, cnv_dptlex));
    insert(rdo::converter::smr2rdox::ParseType::PMD, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::PMD, cnv_pmdparse, cnv_pmdlex));
    insert(rdo::converter::smr2rdox::ParseType::FRM, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::FRM, cnv_frmparse, cnv_frmlex));
    insert(rdo::converter::smr2rdox::ParseType::SMR, rdo::Factory<RDOParserRSSPost>::create());
    insert(rdo::converter::smr2rdox::ParseType::SMR, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::SMR, cnv_smr_sim_parse, cnv_smr_sim_lex));
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserContainerSMRInfo
// --------------------------------------------------------------------------------
RDOParserContainerSMRInfo::RDOParserContainerSMRInfo()
    : RDOParserContainer()
{
    insert(rdo::converter::smr2rdox::ParseType::PRE, rdo::Factory<RDOParserRDOItem>::create(rdo::converter::smr2rdox::FileTypeIn::SMR, cnv_smr_file_parse, cnv_smr_file_lex));
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
