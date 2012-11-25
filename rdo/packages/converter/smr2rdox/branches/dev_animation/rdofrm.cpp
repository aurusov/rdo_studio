/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.cpp
  \authors   ¡‡Ò ¿ÎÂÍÒ‡Ì‰
  \authors   ”ÛÒÓ‚ ¿Ì‰ÂÈ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/update/update.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_frmerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic)
	: RDOParserSrcInfo(src_info)
	, m_itemCount     (0)
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(src_info, pLogic ? pLogic->getCalc() : rdo::runtime::LPRDOCalc(NULL));
	Converter::s_converter()->insertFRMFrame(this);
}

void RDOFRMFrame::setShowIfBlock(CREF(Seek) firstSeek)
{
	if (m_firstSeek.is_initialized() && m_lastSeek.is_initialized() && m_itemCount > 1)
	{
		LPDocUpdate pOpenBraceInsert = rdo::Factory<UpdateInsert>::create(
			*m_firstSeek,
			_T("\n{")
		);
		ASSERT(pOpenBraceInsert);
		Converter::s_converter()->insertDocUpdate(pOpenBraceInsert);

		LPDocUpdate pCloseBraceInsert = rdo::Factory<UpdateInsert>::create(
			*m_lastSeek,
			_T("\n}")
		);
		ASSERT(pCloseBraceInsert);
		Converter::s_converter()->insertDocUpdate(pCloseBraceInsert);
	}

	m_itemCount = 0;
	m_firstSeek = firstSeek;
	m_lastSeek.reset();
}

void RDOFRMFrame::addItem(CREF(rdo::runtime::LPRDOCalc), ruint lastSeek)
{
	++m_itemCount;
	m_lastSeek = lastSeek;
}

void RDOFRMFrame::setFrameConditionPos(ruint firstSeek, ruint lastSeek)
{
	m_frame—onditionPos = std::make_pair(firstSeek, lastSeek);
}

void RDOFRMFrame::onAfterBackPicture(ruint lastSeek)
{
	if (!m_frame—onditionPos.is_initialized())
		return;

	LPDocUpdate pFrameConditionBeginInsert = rdo::Factory<UpdateInsert>::create(
		lastSeek,
		_T("\n// ”ÒÎÓ‚ËÂ ÔÓÍ‡Á‡ ÙÂÈÏ‡\nif (!(")
	);
	ASSERT(pFrameConditionBeginInsert);
	Converter::s_converter()->insertDocUpdate(pFrameConditionBeginInsert);

	LPDocUpdate pConditionMove = rdo::Factory<UpdateMove>::create(
		m_frame—onditionPos->first,
		m_frame—onditionPos->second,
		lastSeek
	);
	ASSERT(pConditionMove);
	Converter::s_converter()->insertDocUpdate(pConditionMove);

	LPDocUpdate pFrameConditionEndInsert = rdo::Factory<UpdateInsert>::create(
		lastSeek,
		_T("))\n   return;\n")
	);
	ASSERT(pFrameConditionEndInsert);
	Converter::s_converter()->insertDocUpdate(pFrameConditionEndInsert);
}

CREF(RDOFRMFrame::Seek) RDOFRMFrame::getFirstSeek() const
{
	return m_firstSeek;
}

CREF(RDOFRMFrame::Seek) RDOFRMFrame::getLastSeek() const
{
	return m_lastSeek;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
