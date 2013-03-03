/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_edit.cpp
  \author    Óğóñîâ Àíäğåé (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QProcess>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/find_edit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "thirdparty/scintilla/include/SciLexer.h"
#include "thirdparty/scintilla/rdo/LexFind.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;
using namespace rdo::gui::style;

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÀàÁáÂâÃãÄäÅå¨¸ÆæÇçÈèÉéÊêËëÌìÍíÎîÏïĞğÑñÒòÓóÔôÕõÖö×÷ØøÙùÚúÛûÜüİıŞşßÿ";

Find::Find(PTR(QWidget) pParent)
	: super(pParent)
	, PopupMenu(pParent)
{
	sendEditor(SCI_SETLEXER, SCLEX_FIND);
	//	int lexLanguage = sendEditor(SCI_GETLEXER);
	sendEditor(SCI_SETSTYLEBITS, 5);
	sendEditorString(SCI_SETWORDCHARS, 0, wordCharacters);
}

Find::~Find()
{}

void Find::setEditorStyle(FindStyle* pStyle)
{
	super::setEditorStyle(pStyle);
	if (!m_pStyle)
	{
		return;
	}

	// ----------
	// Colors
	FindTheme* theme = static_cast<FindTheme*>(m_pStyle->theme);
	sendEditor(SCI_STYLESETFORE, SCE_FIND_DEFAULT, convertColor(theme->defaultColor));
	sendEditor(SCI_STYLESETBACK, SCE_FIND_DEFAULT, convertColor(theme->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_FIND_KEYWORD, convertColor(theme->keywordColor));
	sendEditor(SCI_STYLESETBACK, SCE_FIND_KEYWORD, convertColor(theme->backgroundColor));

	// ----------
	// Styles
	sendEditor(SCI_STYLESETBOLD     , SCE_FIND_DEFAULT, theme->defaultStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_FIND_DEFAULT, theme->defaultStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_FIND_DEFAULT, theme->defaultStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_FIND_KEYWORD, theme->keywordStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_FIND_KEYWORD, theme->keywordStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_FIND_KEYWORD, theme->keywordStyle & StyleFont::UNDERLINE);

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

void Find::setKeyword(CREF(QString) keyword, const rbool matchCase) const
{
	sendEditorString(SCI_SETPROPERTY, reinterpret_cast<unsigned long>("find_matchcase"), matchCase ? "1" : "0");
	sendEditorString(SCI_SETKEYWORDS, SCI_RDO_ENDOFLINEONLY_KEYWORDSINDEX, keyword);
}

void Find::onHelpContext()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_run.htm#output_find\n");
	g_pApp->callQtAssistant(ba);
}

void Find::mousePressEvent(QMouseEvent* pEvent)
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
