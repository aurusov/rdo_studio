// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_sprite.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOFRMSprite::RDOFRMSprite(const RDOParserSrcInfo& src_info)
    : RDOFRMCommandList(src_info)
{
    m_pSprite = rdo::Factory<rdo::runtime::RDOFRMSprite>::create(function()->src_info());
    ASSERT(m_pSprite)

    RDOParser::s_parser()->insertFRMSprite(this);
}

RDOFRMSprite::~RDOFRMSprite()
{}

rdo::runtime::LPRDOFRMSprite RDOFRMSprite::list() const
{
    return m_pSprite;
}

CLOSE_RDO_PARSER_NAMESPACE
