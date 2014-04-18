/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_rdo.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity_i.h"
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/rdopat.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserRDOItem
// --------------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from)
	: RDOParserItem(type, parser_fun, error_fun, lexer_fun, from)
	, m_pLexer(NULL)
{}

RDOParserRDOItem::~RDOParserRDOItem()
{
	if (m_pLexer)
	{
		delete m_pLexer;
		m_pLexer = NULL;
	}
}

void RDOParserRDOItem::parse(const LPRDOParser& pParser)
{
	ASSERT(pParser);

	std::stringstream in_stream(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	switch (m_from)
	{
	case sf_repository:
		{
			rdo::repository::RDOThreadRepository::FileData fileData(m_type, in_stream);
			kernel->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
			break;
		}
	case sf_editor:
		{
			rdo::repository::RDOThreadRepository::FileData fileData(m_type, in_stream);
			kernel->sendMessage(kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &fileData);
			break;
		}
	}
	if (in_stream.good())
	{
		parse(pParser, in_stream);
	}
}

void RDOParserRDOItem::parse(const LPRDOParser& pParser, std::istream& in_stream)
{
	ASSERT(pParser  );
	ASSERT(!m_pLexer);

	std::ostringstream out_stream;
	m_pLexer = getLexer(pParser, &in_stream, &out_stream);

	if (m_pLexer)
	{
		if (m_parser_fun)
		{
			m_parser_fun(m_pLexer);
		}
		delete m_pLexer;
		m_pLexer = NULL;
	}
}

RDOLexer* RDOParserRDOItem::getLexer(const LPRDOParser& pParser, std::istream* in_stream, std::ostream* out_stream)
{
	ASSERT(pParser);
	return new RDOLexer(pParser, in_stream, out_stream);
}

std::size_t RDOParserRDOItem::lexer_loc_line()
{
	if (m_pLexer)
	{
		return m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_line : m_pLexer->lineno();
	}
	else
	{
		return std::size_t(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE);
	}
}

std::size_t RDOParserRDOItem::lexer_loc_pos()
{
	return m_pLexer && m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_pos : 0;
}

std::string RDOParserRDOItem::text() const
{
	ASSERT(m_pLexer);
	return m_pLexer->YYText();
}

CLOSE_RDO_PARSER_NAMESPACE
