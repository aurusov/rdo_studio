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
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLexer
// --------------------------------------------------------------------------------
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
	int cnvwrap(void) 
	{ 
		return 1; 
	}
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
