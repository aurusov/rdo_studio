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
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOParserItem::RDOParserItem()
	: m_type      (rdoModelObjects::PAT)
	, m_parser_fun(NULL                )
	, m_error_fun (NULL                )
	, m_lexer_fun (NULL                )
	, m_from      (sf_repository       )
{}

RDOParserItem::RDOParserItem(
		rdoModelObjects::RDOFileType type,
		t_bison_parse_fun            parser_fun,
		t_bison_error_fun            error_fun,
		t_flex_lexer_fun             lexer_fun,
		StreamFrom                   from
)
	: m_type      (type      )
	, m_parser_fun(parser_fun)
	, m_error_fun (error_fun )
	, m_lexer_fun (lexer_fun )
	, m_from      (from      )
{}

RDOParserItem::~RDOParserItem()
{}

void RDOParserItem::parse(const LPRDOParser& /*pParser*/, std::istream& /*in_stream*/)
{}

std::size_t RDOParserItem::lexer_loc_line()
{
	return std::size_t(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE);
}

std::size_t RDOParserItem::lexer_loc_pos()
{
	return 0;
}

CLOSE_RDO_PARSER_NAMESPACE
