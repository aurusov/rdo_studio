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
#include "rdo_lib/rdo_parser/rdobison.h"
#include "rdo_lib/rdo_parser/rdogramma.h"

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer yyFlexLexer
#include "thirdparty/bison_flex/FlexLexer.h"
#endif
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLexer
// ----------------------------------------------------------------------------
class RDOParser;

class RDOLexer: public yyFlexLexer
{
public:
	RDOLexer(PTR(RDOParser) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout);

	void loc_init     ();
	void loc_action   ();
	void loc_delta_pos(int value);
	void setvalue     (int value);

	PTR(RDOParser)    parser();

	void  enumBegin();
	void  enumReset();
	rbool enumEmpty();

	void array_cnt_pls();
	void array_cnt_rst();
	int  array_cnt_shw();

	PTR(int)         m_lpval;
	PTR(YYLTYPE)     m_lploc;

protected:
	virtual int  LexerInput (PTR(char)  buf, int max_size);
	virtual void LexerOutput(CPTR(char) buf, int size    );
	virtual void LexerError (CPTR(char) msg);

private:
	PTR(std::istream) m_yyin;
	PTR(std::ostream) m_yyout;
	PTR(RDOParser)    m_pParser;
	rbool             m_enumEmpty;
	rsint             m_array_param_cnt;
};

CLOSE_RDO_PARSER_NAMESPACE

#define LEXER     reinterpret_cast<PTR(rdoParse::RDOLexer)>(lexer)
#define LEXER_POS (*LEXER->m_lploc)

#include "rdo_lib/rdo_parser/rdoparser_lexer.inl"

#endif //! _RDOPARSER_LEXER_H_
