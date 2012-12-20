/*!
  \copyright (c) RDO-Team, 2011
  \file      animation_frame.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      Фрейм анимации в парсере
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void frmerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo)
	: RDOFRMCommandList(srcInfo)
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(function()->src_info());
	ASSERT(m_pFrame)
	RDOParser::s_parser()->runtime()->addRuntimeFrame(m_pFrame);

	RDOParser::s_parser()->insertFRMFrame(this);
}

RDOFRMFrame::~RDOFRMFrame()
{}

CREF(rdo::runtime::LPRDOFRMFrame) RDOFRMFrame::frame() const
{
	return m_pFrame;
}

rdo::runtime::LPRDOFRMSprite RDOFRMFrame::list() const
{
	return m_pFrame;
}

CLOSE_RDO_PARSER_NAMESPACE
