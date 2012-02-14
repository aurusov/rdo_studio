/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_lexer.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_LEXER_H_
#define _RDOPARSER_LEXER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/grammar/rdobison.h"
#include "utils/smart_ptr/intrusive_ptr.h"

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer yyFlexLexer
#include "thirdparty/bison_flex/include/FlexLexer.h"
#endif
// --------------------------------------------------------------------------------

OPEN_RDO_PARSE_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLexer
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);

class RDOLexer: public yyFlexLexer
{
public:
	RDOLexer(CREF(LPRDOParser) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout);

	void loc_init     ();
	void loc_action   ();
	void loc_delta_pos(int value);
	void setvalue     (int value);

	CREF(LPRDOParser) parser() const;

	void  enumBegin();
	void  enumReset();
	rbool enumEmpty();

	void  array_cnt_pls();
	void  array_cnt_rst();
	rsint array_cnt_shw();

	PTR(int)         m_lpval;
	PTR(YYLTYPE)     m_lploc;

protected:
	virtual int  LexerInput (PTR(char)  buf, int max_size);
	virtual void LexerOutput(CPTR(char) buf, int size    );
	virtual void LexerError (CPTR(char) msg);

private:
	PTR(std::istream) m_yyin;
	PTR(std::ostream) m_yyout;
	LPRDOParser       m_pParser;
	rbool             m_enumEmpty;
	rsint             m_array_param_cnt;
};

CLOSE_RDO_PARSE_NAMESPACE

#define LEXER     reinterpret_cast<PTR(rdoParse::RDOLexer)>(lexer)
#define LEXER_POS (*LEXER->m_lploc)

#include "simulator/compiler/parser/rdoparser_lexer.inl"

#endif // _RDOPARSER_LEXER_H_
