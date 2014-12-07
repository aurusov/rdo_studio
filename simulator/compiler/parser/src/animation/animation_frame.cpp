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

int frmlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void frmerror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(const RDOParserSrcInfo& srcInfo)
	: RDOFRMCommandList(srcInfo)
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(function()->src_info());
	ASSERT(m_pFrame)
	RDOParser::s_parser()->runtime()->addRuntimeFrame(m_pFrame);

	RDOParser::s_parser()->insertFRMFrame(this);
}

RDOFRMFrame::~RDOFRMFrame()
{}

const rdo::runtime::LPRDOFRMFrame& RDOFRMFrame::frame() const
{
	return m_pFrame;
}

rdo::runtime::LPRDOFRMSprite RDOFRMFrame::list() const
{
	return m_pFrame;
}

CLOSE_RDO_PARSER_NAMESPACE
