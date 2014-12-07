#ifndef _CONVERTOR_RDOCONVERTER_LEXER_H_
#define _CONVERTOR_RDOCONVERTER_LEXER_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/grammar/rdobison.h"

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer cnvFlexLexer
#include <FlexLexer.h>
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
	RDOLexer(Converter* pParser, std::istream* yyin, std::ostream* yyout);

	void loc_init();
	void loc_action();
	void loc_delta_pos(int value);
	void setvalue(int value);

	Converter* converter();

	void enumBegin();
	void enumReset();
	bool enumEmpty();

	int* m_lpval;
	YYLTYPE* m_lploc;

protected:
	virtual int LexerInput(char* buf, int max_size);
	virtual void LexerOutput(const char* buf, int size);
	virtual void LexerError(const char* msg);

private:
	std::istream* m_yyin;
	std::ostream* m_yyout;
	Converter* m_pParser;
	bool m_enumEmpty;
	int m_array_param_cnt;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#define LEXER     reinterpret_cast<rdo::converter::smr2rdox::RDOLexer*>(lexer)
#define LEXER_POS (*LEXER->m_lploc)

#endif // _CONVERTOR_RDOCONVERTER_LEXER_H_
