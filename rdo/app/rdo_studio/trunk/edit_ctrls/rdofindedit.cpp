/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindedit.cpp
  \author    Óğóñîâ Àíäğåé (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "thirdparty/scintilla/include/SciLexer.h"
#include "thirdparty/scintilla/rdo/LexFind.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;
using namespace rdoStyle;

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÀàÁáÂâÃãÄäÅå¨¸ÆæÇçÈèÉéÊêËëÌìÍíÎîÏïĞğÑñÒòÓóÔôÕõÖö×÷ØøÙùÚúÛûÜüİıŞşßÿ";

// --------------------------------------------------------------------------------
// -------------------- RDOFindEdit
// ---------------------------------------------------------------------------
RDOFindEdit::RDOFindEdit(PTR(QWidget) pParent)
	: super(pParent)
	, EditWithReadOnlyPopupMenu(pParent)
{
	sendEditor(SCI_SETLEXER, SCLEX_FIND);
	//	int lexLanguage = sendEditor(SCI_GETLEXER);
	sendEditor(SCI_SETSTYLEBITS, 5);
	sendEditorString(SCI_SETWORDCHARS, 0, wordCharacters);
}

RDOFindEdit::~RDOFindEdit()
{}

void RDOFindEdit::setEditorStyle(RDOFindEditStyle* pStyle)
{
	super::setEditorStyle(pStyle);
	if (!m_pStyle)
	{
		return;
	}

	// ----------
	// Colors
	RDOFindEditTheme* theme = static_cast<RDOFindEditTheme*>(m_pStyle->theme);
	sendEditor(SCI_STYLESETFORE, SCE_FIND_DEFAULT, theme->defaultColor);
	sendEditor(SCI_STYLESETBACK, SCE_FIND_DEFAULT, theme->backgroundColor);
	sendEditor(SCI_STYLESETFORE, SCE_FIND_KEYWORD, theme->keywordColor);
	sendEditor(SCI_STYLESETBACK, SCE_FIND_KEYWORD, theme->backgroundColor);

	// ----------
	// Styles
	sendEditor(SCI_STYLESETBOLD     , SCE_FIND_DEFAULT, theme->defaultStyle & RDOStyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_FIND_DEFAULT, theme->defaultStyle & RDOStyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_FIND_DEFAULT, theme->defaultStyle & RDOStyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_FIND_KEYWORD, theme->keywordStyle & RDOStyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_FIND_KEYWORD, theme->keywordStyle & RDOStyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_FIND_KEYWORD, theme->keywordStyle & RDOStyleFont::UNDERLINE);

	// ----------
	// Font Name
	sendEditorString(SCI_STYLESETFONT, SCE_FIND_DEFAULT, m_pStyle->font->name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_FIND_KEYWORD, m_pStyle->font->name.c_str());

	// ----------
	// Font Size
	sendEditor(SCI_STYLESETSIZE, SCE_FIND_DEFAULT, m_pStyle->font->size);
	sendEditor(SCI_STYLESETSIZE, SCE_FIND_KEYWORD, m_pStyle->font->size);

	// ----------
	// Codepage and Characterset
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_FIND_DEFAULT, m_pStyle->font->characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_FIND_KEYWORD, m_pStyle->font->characterSet);
}

void RDOFindEdit::setKeyword(CREF(tstring) keyword, const rbool matchCase) const
{
	sendEditorString(SCI_SETPROPERTY, reinterpret_cast<unsigned long>("find_matchcase"), matchCase ? "1" : "0");
	sendEditorString(SCI_SETKEYWORDS, SCI_RDO_ENDOFLINEONLY_KEYWORDSINDEX, keyword.c_str());
}

void RDOFindEdit::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_find\n");
	studioApp.callQtAssistant(ba);
}

void RDOFindEdit::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		super::mousePressEvent(pEvent);
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}
