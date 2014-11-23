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
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer yyFlexLexer
#include <FlexLexer.h>
#endif
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLexer
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);

class RDOLexer: public yyFlexLexer
{
public:
	RDOLexer(const LPRDOParser& pParser, std::istream* yyin, std::ostream* yyout);

	void loc_init();
	void loc_action();
	void loc_delta_pos(int value);
	void setvalue(int value);

	const LPRDOParser& parser() const;

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
	LPRDOParser m_pParser;
	bool m_enumEmpty;
	int m_array_param_cnt;
};

CLOSE_RDO_PARSER_NAMESPACE

#define LEXER     reinterpret_cast<rdo::compiler::parser::RDOLexer*>(lexer)
#define LEXER_POS (*LEXER->m_lploc)

#endif // _RDOPARSER_LEXER_H_
