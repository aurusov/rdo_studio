/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_lexer.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_LEXER_H_
#define _RDOPARSER_LEXER_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdogramma.h"
#include "thirdparty/bison_flex/FlexLexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLexer
// ----------------------------------------------------------------------------
class RDOParser;

class RDOLexer: public yyFlexLexer
{
public:
	RDOLexer(PTR(RDOParser) parser, PTR(std::istream) yyin, PTR(std::ostream) yyout)
		: yyFlexLexer     (yyin, yyout)
		, m_parser        (parser     )
		, m_yyin          (yyin       )
		, m_yyout         (yyout      )
		, m_lpval         (NULL       )
		, m_lploc         (NULL       )
		, m_enum_param_cnt(0          )
	{}

	PTR(RDOParser)   m_parser;
	PTR(int)         m_lpval;
	PTR(YYLTYPE)     m_lploc;
	int              m_enum_param_cnt;

	void loc_init()
	{
		if (m_lploc)
		{
			m_lploc->first_line   = 0;
			m_lploc->first_column = 0;
			m_lploc->last_line    = 0;
			m_lploc->last_column  = 0;
		}
	}
	void loc_action()
	{
		if (m_lploc)
		{
			m_lploc->first_line   = m_lploc->last_line;
			m_lploc->first_column = m_lploc->last_column;
			for (int i = 0; i < YYLeng(); i++)
			{
				if (YYText()[i] == '\n')
				{
					m_lploc->last_line++;
					m_lploc->last_column = 0;
				}
				else if (YYText()[i] == '\r')
				{
					m_lploc->last_column = 0;
				}
				else
				{
					m_lploc->last_column++;
				}
			}
		}
	}
	void loc_delta_pos(int value)
	{
		if (m_lploc)
		{
			m_lploc->first_column += value;
			m_lploc->last_column  += value;
		}
	}
	void setvalue(int value)
	{
		*m_lpval = value;
	}

protected:
	virtual int  LexerInput (PTR(char)  buf, int max_size);
	virtual void LexerOutput(CPTR(char) buf, int size    );
	virtual void LexerError (CPTR(char) msg);

private:
	PTR(std::istream) m_yyin;
	PTR(std::ostream) m_yyout;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_LEXER_H_
