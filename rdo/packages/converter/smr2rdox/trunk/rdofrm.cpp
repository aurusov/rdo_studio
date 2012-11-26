/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
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
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo)
	: RDOParserSrcInfo(srcInfo)
	, m_itemCount     (0)
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(srcInfo);
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
	m_frameConditionPos = std::make_pair(firstSeek, lastSeek);
}

void RDOFRMFrame::onAfterBackPicture(ruint lastSeek)
{
	if (!m_frameConditionPos.is_initialized())
		return;

	LPDocUpdate pFrameConditionBeginInsert = rdo::Factory<UpdateInsert>::create(
		lastSeek,
		_T("\n// Условие показа фрейма\nif (!(")
	);
	ASSERT(pFrameConditionBeginInsert);
	Converter::s_converter()->insertDocUpdate(pFrameConditionBeginInsert);

	LPDocUpdate pConditionMove = rdo::Factory<UpdateMove>::create(
		m_frameConditionPos->first,
		m_frameConditionPos->second,
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
