/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_lexer.inl
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLexer
// ----------------------------------------------------------------------------
inline RDOLexer::RDOLexer(PTR(RDOParser) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout)
	: yyFlexLexer(yyin, yyout)
	, m_pParser  (pParser    )
	, m_yyin     (yyin       )
	, m_yyout    (yyout      )
	, m_lpval    (NULL       )
	, m_lploc    (NULL       )
	, m_enumEmpty(true       )
	, m_array_param_cnt(0    )
{}

inline void RDOLexer::loc_init()
{
	if (m_lploc)
	{
		m_lploc->first_line   = 0;
		m_lploc->first_column = 0;
		m_lploc->last_line    = 0;
		m_lploc->last_column  = 0;
	}
}

inline void RDOLexer::loc_action()
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

inline void RDOLexer::loc_delta_pos(int value)
{
	if (m_lploc)
	{
		m_lploc->first_column += value;
		m_lploc->last_column  += value;
	}
}

inline void RDOLexer::setvalue(int value)
{
	*m_lpval = value;
}

inline PTR(RDOParser) RDOLexer::parser()
{
	return m_pParser;
}

inline void RDOLexer::enumBegin()
{
	m_enumEmpty = false;
}

inline void RDOLexer::enumReset()
{
	m_enumEmpty = true;
}

inline rbool RDOLexer::enumEmpty()
{
	return m_enumEmpty;
}
inline void RDOLexer::array_cnt_pls()
{
	m_array_param_cnt++;
}
inline void RDOLexer::array_cnt_rst()
{
	m_array_param_cnt = 0;
}
inline rsint RDOLexer::array_cnt_shw()
{
	return m_array_param_cnt;
}
CLOSE_RDO_PARSER_NAMESPACE
