/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_lexer.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLexer
// --------------------------------------------------------------------------------
inline RDOLexer::RDOLexer(PTR(Converter) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout)
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
		m_lploc->m_first_line = 0;
		m_lploc->m_first_pos  = 0;
		m_lploc->m_last_line  = 0;
		m_lploc->m_last_pos   = 0;
		m_lploc->m_first_seek = 0;
		m_lploc->m_last_seek  = 0;
	}
}

inline void RDOLexer::loc_action()
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

inline void RDOLexer::loc_delta_pos(int value)
{
	if (m_lploc)
	{
		m_lploc->m_first_pos  += value;
		m_lploc->m_last_pos   += value;
		m_lploc->m_first_seek += value;
		m_lploc->m_last_seek  += value;
	}
}

inline void RDOLexer::setvalue(int value)
{
	*m_lpval = value;
}

inline PTR(Converter) RDOLexer::converter()
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
CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
