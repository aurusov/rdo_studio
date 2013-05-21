/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_lexer.cpp
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
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLexer
// --------------------------------------------------------------------------------
RDOLexer::RDOLexer(CREF(LPRDOParser) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout)
	: yyFlexLexer(yyin, yyout)
	, m_lpval    (NULL       )
	, m_lploc    (NULL       )
	, m_yyin     (yyin       )
	, m_yyout    (yyout      )
	, m_pParser  (pParser    )
	, m_enumEmpty(true       )
	, m_array_param_cnt(0    )
{}

CREF(LPRDOParser) RDOLexer::parser() const
{
	return m_pParser;
}

#ifdef YY_INTERACTIVE
int RDOLexer::LexerInput(PTR(char) buf, int /* max_size */)
#else
int RDOLexer::LexerInput(PTR(char) buf, int max_size)
#endif
{
	if (m_yyin->eof() || m_yyin->fail())
		return 0;

#ifdef YY_INTERACTIVE
	m_yyin->get(buf[0]);

	if (m_yyin->eof())
		return 0;

	if (m_yyin->bad())
		return -1;

	return 1;
#else
	(void) m_yyin->read(buf, max_size);

	if (m_yyin->bad())
	{
		return -1;
	}
	else
	{
		return m_yyin->gcount();
	}
#endif
}

void RDOLexer::LexerOutput(CPTR(char) buf, int size)
{
	(void) m_yyout->write(buf, size);
}

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

void RDOLexer::LexerError(const char msg[])
{
	std::cerr << msg << '\n';
	exit(YY_EXIT_FAILURE);
}

extern "C"
{
	int yywrap(void) 
	{ 
		return 1; 
	}
}

CLOSE_RDO_PARSER_NAMESPACE
