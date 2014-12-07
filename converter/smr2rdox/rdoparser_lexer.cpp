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
RDOLexer::RDOLexer(Converter* pParser, std::istream* yyin, std::ostream* yyout)
	: yyFlexLexer(yyin, yyout)
	, m_lpval    (NULL       )
	, m_lploc    (NULL       )
	, m_yyin     (yyin       )
	, m_yyout    (yyout      )
	, m_pParser  (pParser    )
	, m_enumEmpty(true       )
	, m_array_param_cnt(0    )
{}

void RDOLexer::loc_init()
{
	if (m_lploc)
	{
		m_lploc->m_first_line = 0;
		m_lploc->m_first_pos  = 0;
		m_lploc->m_last_line  = 0;
		m_lploc->m_last_pos   = 0;
		m_lploc->m_first_seek = 0;
		m_lploc->m_last_seek  = 0;
	}
}

void RDOLexer::loc_action()
{
	if (m_lploc)
	{
		m_lploc->m_first_line = m_lploc->m_last_line;
		m_lploc->m_first_pos  = m_lploc->m_last_pos;
		m_lploc->m_first_seek = m_lploc->m_last_seek;
		for (int i = 0; i < YYLeng(); i++)
		{
			switch (YYText()[i])
			{
			case '\n':
				m_lploc->m_last_line++;
				m_lploc->m_last_pos = 0;
				break;
			case '\r':
				m_lploc->m_last_pos = 0;
				break;
			default:
				m_lploc->m_last_pos++;
				break;
			}

			m_lploc->m_last_seek++;
		}
	}
}

void RDOLexer::loc_delta_pos(int value)
{
	if (m_lploc)
	{
		m_lploc->m_first_pos  += value;
		m_lploc->m_last_pos   += value;
		m_lploc->m_first_seek += value;
		m_lploc->m_last_seek  += value;
	}
}

void RDOLexer::setvalue(int value)
{
	*m_lpval = value;
}

Converter* RDOLexer::converter()
{
	return m_pParser;
}

void RDOLexer::enumBegin()
{
	m_enumEmpty = false;
}

void RDOLexer::enumReset()
{
	m_enumEmpty = true;
}

bool RDOLexer::enumEmpty()
{
	return m_enumEmpty;
}

#ifdef YY_INTERACTIVE
int RDOLexer::LexerInput(char* buf, int /* max_size */)
#else
int RDOLexer::LexerInput(char* buf, int max_size)
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
		return static_cast<int>(m_yyin->gcount());
	}
#endif
}

void RDOLexer::LexerOutput(const char* buf, int size)
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
