/*!
  \copyright (c) RDO-Team, 2011
  \file      animation_frame.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      Фрейм анимации в парсере
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_ANIMATION_FRAME_H_
#define _SIMULATOR_COMPILER_PARSER_ANIMATION_FRAME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
#include "simulator/compiler/parser/src/function/function.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  frmparse(PTR(void) lexer);
int  frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void frmerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
class RDOFRMFrame
	: public RDOFRMCommandList
	, public Context
	, public IContextFind
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring) name      () const;
	LPExpression  expression() const;
	void          end       ();

	CREF(rdo::runtime::LPRDOFRMFrame) frame() const;

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo);
	virtual ~RDOFRMFrame();

	LPFunction                  m_pFunction;
	rdo::runtime::LPRDOFRMFrame m_pFrame;

	rdo::runtime::LPRDOFRMSprite list() const;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_ANIMATION_FRAME_H_
