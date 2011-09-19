/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_lexer.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_LEXER_H_
#define _CONVERTOR_RDOCONVERTER_LEXER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdobison.h"
#include "converter/smr2rdox/grammar/rdogramma.h"

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer cnvFlexLexer
#include "thirdparty/bison_flex/FlexLexer.h"
#endif
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLexer
// --------------------------------------------------------------------------------
class Converter;

class RDOLexer: public yyFlexLexer
{
public:
	RDOLexer(PTR(Converter) pParser, PTR(std::istream) yyin, PTR(std::ostream) yyout);

	void loc_init     ();
	void loc_action   ();
	void loc_delta_pos(int value);
	void setvalue     (int value);

	PTR(Converter)    converter();

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
	PTR(Converter)    m_pParser;
	rbool             m_enumEmpty;
	rsint             m_array_param_cnt;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#define LEXER     reinterpret_cast<PTR(rdoConverter::RDOLexer)>(lexer)
#define LEXER_POS (*LEXER->m_lploc)

#include "converter/smr2rdox/rdoparser_lexer.inl"

#endif // _CONVERTOR_RDOCONVERTER_LEXER_H_
