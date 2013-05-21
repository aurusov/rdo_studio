/*!
  \copyright (c) RDO-Team, 2012
  \file      animation_sprite.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     Спрайт анимации в парсере
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_sprite.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOFRMSprite::RDOFRMSprite(CREF(RDOParserSrcInfo) src_info)
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
