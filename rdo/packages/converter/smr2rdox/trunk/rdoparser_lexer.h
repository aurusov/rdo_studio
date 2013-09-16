/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_lexer.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_LEXER_H_
#define _CONVERTOR_RDOCONVERTER_LEXER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/grammar/rdobison.h"

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer cnvFlexLexer
#include "thirdparty/bison_flex/include/FlexLexer.h"
#endif
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#define LEXER     reinterpret_cast<PTR(rdo::converter::smr2rdox::RDOLexer)>(lexer)
#define LEXER_POS (*LEXER->m_lploc)

#include "converter/smr2rdox/rdoparser_lexer.inl"

#endif // _CONVERTOR_RDOCONVERTER_LEXER_H_
