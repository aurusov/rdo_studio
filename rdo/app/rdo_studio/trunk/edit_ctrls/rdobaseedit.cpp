/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobaseedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <boost/bind.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <QMessageBox>
#include <QClipboard>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
#include "app/rdo_studio/src/dialog/goto_line_dialog.h"
#include "thirdparty/scintilla/include/SciLexer.h"
#include "thirdparty/scintilla/lexlib/WordList.h"
// --------------------------------------------------------------------------------

using namespace rdoEditCtrl;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- EditBase::Group
// ---------------------------------------------------------------------------
EditBase::Group::Group()
	: bMatchCase     (false)
	, bMatchWholeWord(false)
	, bSearchDown    (true )
{}

void EditBase::Group::insert(PTR(EditBase) pEdit)
{
	m_list.push_back(pEdit);
}

EditBase::Group::List::const_iterator EditBase::Group::begin() const
{
	return m_list.begin();
}

EditBase::Group::List::const_iterator EditBase::Group::end() const
{
	return m_list.end();
}

EditBase::Group::List::const_iterator EditBase::Group::next(CREF(List::const_iterator) it) const
{
	EditBase::Group::List::const_iterator result(it);
	++result;
	if (result == m_list.end())
	{
		result = m_list.begin();
	}
	return result;
}

EditBase::Group::List::const_iterator EditBase::Group::prev(CREF(List::const_iterator) it) const
{
	EditBase::Group::List::const_iterator result(it);
	if (result == m_list.begin())
	{
		result = m_list.end();
	}
	--result;
	return result;
}

void EditBase::Group::for_each(CREF(this_method) fun) const
{
	boost::range::for_each(m_list, fun);
}

EditBase::Group::List::const_iterator EditBase::Group::find_if(CREF(this_predicate) fun) const
{
	return boost::range::find_if(m_list, fun);
}

// --------------------------------------------------------------------------------
// -------------------- EditBase
// ---------------------------------------------------------------------------

// ---------------
// RTF export

#define RTF_HEADEROPEN    "{\\rtf1\\ansi\\deff0\\deftab720"
#define RTF_FONTDEFOPEN   "{\\fonttbl"
#define RTF_FONTDEF       "{\\f%d\\fnil\\fcharset%u %s;}"
#define RTF_FONTDEFCLOSE  "}"
#define RTF_COLORDEFOPEN  "{\\colortbl"
#define RTF_COLORDEF      "\\red%d\\green%d\\blue%d;"
#define RTF_COLORDEFCLOSE "}"
#define RTF_HEADERCLOSE   "\n"
#define RTF_BODYOPEN      ""
#define RTF_BODYCLOSE     "}"

#define RTF_SETFONTFACE   "\\f"
#define RTF_SETFONTSIZE   "\\fs"
#define RTF_SETCOLOR      "\\cf"
#define RTF_SETBACKGROUND "\\highlight"
#define RTF_BOLD_ON       "\\b"
#define RTF_BOLD_OFF      "\\b0"
#define RTF_ITALIC_ON     "\\i"
#define RTF_ITALIC_OFF    "\\i0"
#define RTF_UNDERLINE_ON  "\\ul"
#define RTF_UNDERLINE_OFF "\\ulnone"
#define RTF_STRIKE_ON     "\\i"
#define RTF_STRIKE_OFF    "\\strike0"

#define RTF_EOLN "\\par\n"
#define RTF_TAB  "\\tab "

#define MAX_STYLEDEF   128
#define MAX_FONTDEF      1
#define MAX_COLORDEF    32

// ---------------

EditBase::EditBase(PTR(QWidget) pParent)
	: super               (pParent)
	, m_markerCount       (0    )
	, m_pStyle            (NULL )
	, m_pGroup            (NULL )
	, m_firstFoundPos     (-1   )
	, m_haveFound         (false)
	, m_pFindDialog       (NULL )
	, m_pFindReplaceDialog(NULL )
{
	QObject::connect(this, SIGNAL(needShown(int, int)), this, SLOT(catchNeedShown(int, int)));
	QObject::connect(this, SIGNAL(charAdded(int)),      this, SLOT(catchCharAdded(int)));
	QObject::connect(this, SIGNAL(updateUi()),          this, SLOT(onUpdateEditGUI()));
	QObject::connect(this, SIGNAL(modified(int, int, int, int, const QByteArray&, int, int, int)), this, SLOT(onUpdateModify()));
	QObject::connect(this, SIGNAL(aboutToCopy(QMimeData*)), this, SLOT(onCopyAsRTF(QMimeData*)));
	QObject::connect(this, SIGNAL(zoom(int)),this, SLOT(onViewZoomChanged(int)));

	m_sciMarkerBookmark = getNewMarker();

	sendEditor(SCI_SETLEXER, SCLEX_NULL);
	//	int lexLanguage = sendEditor(SCI_GETLEXER);
	sendEditor(SCI_SETMARGINWIDTHN, 1, 0);
	sendEditor(SCI_USEPOPUP, 0);
}

EditBase::~EditBase()
{}

void EditBase::catchNeedShown(int position, int length)
{
	ensureRangeVisible(position, position + length, false);
}

void EditBase::catchCharAdded(int ch)
{
	if (m_pStyle && m_pStyle->tab->autoIndent && (ch == '\r' || ch == '\n'))
	{
		autoIndent();
	}
}

long EditBase::sendEditorString(ruint msg, unsigned long wParam, CREF(QString) str) const
{
	return super::sends(msg, wParam, str.toLocal8Bit().constData());
}

int EditBase::getNewMarker()
{
	++m_markerCount;
	return m_markerCount;
}

void EditBase::defineMarker(int marker, int markerType, QColor fore, QColor back) const
{
	sendEditor(SCI_MARKERDEFINE,  marker, markerType);
	sendEditor(SCI_MARKERSETFORE, marker, convertColor(fore));
	sendEditor(SCI_MARKERSETBACK, marker, convertColor(back));
}

const EditBaseStyle* EditBase::getEditorStyle() const
{
	return m_pStyle;
}

void EditBase::setEditorStyle(EditBaseStyle* pStyle)
{
	m_pStyle = pStyle;
	if (!m_pStyle)
		return;

	// ----------
	// Colors
	EditBaseTheme* theme = static_cast<EditBaseTheme*>(m_pStyle->theme);
	sendEditor(SCI_STYLESETBACK, STYLE_DEFAULT, convertColor(theme->defaultColor));
	sendEditor(SCI_STYLESETBACK, STYLE_DEFAULT, convertColor(theme->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_TEXT_DEFAULT, convertColor(theme->defaultColor));
	sendEditor(SCI_STYLESETBACK, SCE_TEXT_DEFAULT, convertColor(theme->backgroundColor));

	// ----------
	// Styles
	sendEditor(SCI_STYLESETBOLD     , STYLE_DEFAULT, theme->defaultStyle & RDOStyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , STYLE_DEFAULT, theme->defaultStyle & RDOStyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, STYLE_DEFAULT, theme->defaultStyle & RDOStyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_TEXT_DEFAULT, theme->defaultStyle & RDOStyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_TEXT_DEFAULT, theme->defaultStyle & RDOStyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_TEXT_DEFAULT, theme->defaultStyle & RDOStyleFont::UNDERLINE);

	// ----------
	// Font Name
	sendEditorString(SCI_STYLESETFONT, STYLE_DEFAULT, m_pStyle->font->name.c_str());

	// ----------
	// Font Size
	sendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT, m_pStyle->font->size);

	// ----------
	// Codepage and Characterset
	sendEditor(SCI_SETCODEPAGE, m_pStyle->font->codepage);
	sendEditor(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, m_pStyle->font->characterSet);

	// ----------
	// Tabs
	sendEditor(SCI_SETTABWIDTH, m_pStyle->tab->tabSize);
	sendEditor(SCI_SETINDENT, m_pStyle->tab->indentSize);
	sendEditor(SCI_SETUSETABS, m_pStyle->tab->useTabs);
	sendEditor(SCI_SETTABINDENTS, m_pStyle->tab->tabIndents);
	sendEditor(SCI_SETBACKSPACEUNINDENTS, !m_pStyle->tab->backspaceUntabs);

	// ----------
	// Caret
	sendEditor(SCI_SETCARETFORE, convertColor(theme->caretColor));
	sendEditor(SCI_SETSELBACK, true, convertColor(theme->selectionBgColor));
	sendEditor(SCI_SETCARETWIDTH, 1);

	// ----------
	// Bookmark
	QColor bookmarkFgColor = theme->bookmarkFgColor;
	QColor bookmarkBgColor = theme->bookmarkBgColor;
	switch (theme->bookmarkStyle)
	{
		case RDOBOOKMARKS_NONE     : defineMarker(m_sciMarkerBookmark, SC_MARK_EMPTY    , bookmarkFgColor, bookmarkBgColor); break;
		case RDOBOOKMARKS_CIRCLE   : defineMarker(m_sciMarkerBookmark, SC_MARK_CIRCLE   , bookmarkFgColor, bookmarkBgColor); break;
		case RDOBOOKMARKS_RECT     : defineMarker(m_sciMarkerBookmark, SC_MARK_SMALLRECT, bookmarkFgColor, bookmarkBgColor); break;
		case RDOBOOKMARKS_ROUNDRECT: defineMarker(m_sciMarkerBookmark, SC_MARK_ROUNDRECT, bookmarkFgColor, bookmarkBgColor); break;
		case RDOBOOKMARKS_ARROW    : defineMarker(m_sciMarkerBookmark, SC_MARK_ARROW    , bookmarkFgColor, bookmarkBgColor); break;
	}

	// ----------
	// Window
	sendEditor(SCI_SETWRAPMODE, m_pStyle->window->wordWrap ? SC_WRAP_WORD : SC_WRAP_NONE);
	sendEditor(SCI_SETHSCROLLBAR, m_pStyle->window->showHorzScrollBar);
}

void EditBase::setGroup(PTR(Group) pGroup)
{
	m_pGroup = pGroup;
}

void EditBase::onEditUndo() 
{
	sendEditor(SCI_UNDO);
}

void EditBase::onEditRedo() 
{
	sendEditor(SCI_REDO);
}

void EditBase::onEditCut() 
{
	sendEditor(SCI_CUT);
}

void EditBase::onEditCopy() 
{
	sendEditor(SCI_COPY);
}

void EditBase::onEditPaste() 
{
	sendEditor(SCI_PASTE);
}

void EditBase::onEditDel() 
{
	sendEditor(SCI_CLEAR);
}

void EditBase::onEditSelectAll() 
{
	sendEditor(SCI_SELECTALL);
}

void EditBase::onEditUpperCase() 
{
	sendEditor(SCI_UPPERCASE);
}

void EditBase::onEditLowerCase() 
{
	sendEditor(SCI_LOWERCASE);
}

tstring EditBase::getCurrentWord() const
{
	int pos_begin = sendEditor(SCI_WORDSTARTPOSITION, getCurrentPos(), true);
	int pos_end   = sendEditor(SCI_WORDENDPOSITION, getCurrentPos(), true);
	int len = pos_end - pos_begin + 2;
	char* word = new char[ len ];
	TextRange tr;
	tr.lpstrText  = word;
	tr.chrg.cpMin = pos_begin;
	tr.chrg.cpMax = pos_end;
	sendEditor(SCI_GETTEXTRANGE, 0, (long)&tr);
	tstring str(tr.lpstrText);
	delete[] word;
	return str;
}

tstring EditBase::getSelection() const
{
	CharacterRange cr = getSelectionRange();
	char* selection = new char[ cr.cpMax - cr.cpMin + 1 ];
	sendEditor(SCI_GETSELTEXT, 0, (long)selection);
	tstring str(selection);
	delete[] selection;
	return str;
}

tstring EditBase::getCurrentOrSelectedWord() const
{
	return isSelected()
		? getSelection  ()
		: getCurrentWord();
}

QString EditBase::getWordForFind() const
{
	return isSelected()
		? QString::fromLocal8Bit(getSelection().c_str())
		: !getCurrentWord().empty()
			? QString::fromLocal8Bit(getCurrentWord().c_str())
			: m_pGroup && !m_pGroup->findStr.isEmpty()
				? m_pGroup->findStr
				: QString();

}

CharacterRange EditBase::getSelectionRange() const
{
	CharacterRange crange;
	crange.cpMin = sendEditor(SCI_GETSELECTIONSTART);
	crange.cpMax = sendEditor(SCI_GETSELECTIONEND);
	return crange;
}

void EditBase::gotoLineEnsureVisible(int line) const
{
	sendEditor(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
	sendEditor(SCI_GOTOLINE, line);
}

void EditBase::ensureRangeVisible(int posStart, int posEnd, rbool enforcePolicy) const
{
	int lineStart = getLineFromPosition(posStart < posEnd ? posStart : posEnd);
	int lineEnd   = getLineFromPosition(posStart > posEnd ? posStart : posEnd);
	for (int line = lineStart; line <= lineEnd; ++line)
	{
		sendEditor(enforcePolicy ? SCI_ENSUREVISIBLEENFORCEPOLICY : SCI_ENSUREVISIBLE, line);
	}
}

void EditBase::onSearchFind() 
{
	m_findSettings.what = QString::fromLocal8Bit(getCurrentOrSelectedWord().c_str());

	if (!m_pFindDialog)
	{
		m_pFindDialog = new FindDialog(
			this,
			boost::bind(&EditBase::onFindDlgFind,  this, _1),
			boost::bind(&EditBase::onFindDlgClose, this)
		);
	}

	if(m_pGroup)
	{
		m_findSettings.what.isEmpty() 
			? m_findSettings.what = m_pGroup->findStr
			: m_pGroup->findStr = m_findSettings.what;
		m_findReplaceSettings.searchDown = m_pGroup->bSearchDown;
		m_findReplaceSettings.matchCase = m_pGroup->bMatchCase;
		m_findReplaceSettings.matchWholeWord = m_pGroup->bMatchWholeWord;
	}

	m_pFindDialog->setSettings(m_findSettings);
	m_pFindDialog->show();
	m_pFindDialog->raise();
	m_pFindDialog->activateWindow();
}

void EditBase::onFindDlgFind(CREF(FindDialog::Settings) settings)
{
	m_findSettings = settings;
	if (m_pGroup)
	{
		m_pGroup->findStr = m_findSettings.what;
	}
	onSearchFindNext();
	updateActionFind(isActivated());
}

void EditBase::onFindDlgClose()
{
	m_pFindDialog = NULL;
}

void EditBase::onSearchFindNext() 
{
	findNext(
		m_pGroup->findStr.isEmpty()
			? m_findSettings.what
			: m_pGroup->findStr,
		m_findSettings.searchDown,
		m_findSettings.matchCase,
		m_findSettings.matchWholeWord
	);
}

void EditBase::onSearchFindPrevious() 
{
	findNext(
		m_pGroup->findStr.isEmpty()
			? m_findSettings.what
			: m_pGroup->findStr,
		!m_findSettings.searchDown,
		m_findSettings.matchCase,
		m_findSettings.matchWholeWord
	);
}

void EditBase::onSearchFindNextCurrent() 
{
	if (m_pGroup)
	{
		m_pGroup->findStr     = QString::fromLocal8Bit(getCurrentWord().c_str());
		m_pGroup->bSearchDown = true;
		findNext(m_pGroup->findStr, m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord);
	}
}

void EditBase::onSearchFindPreviousCurrent() 
{
	if (m_pGroup)
	{
		m_pGroup->findStr     = QString::fromLocal8Bit(getCurrentWord().c_str());
		m_pGroup->bSearchDown = true;
		findNext(m_pGroup->findStr, !m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord);
	}
}

void EditBase::findNext(CREF(QString) findWhat, rbool searchDown, rbool matchCase, rbool matchWholeWord)
{
	int findLen = findWhat.length();
	if (!findLen)
		return;

	if (!getSelection().empty() && !m_haveFound)
	{
		setCurrentPos(sendEditor(searchDown ? SCI_GETSELECTIONSTART : SCI_GETSELECTIONEND));
	}
	else
	{
		if (!getCurrentWord().empty() && !m_haveFound)
		{
			setCurrentPos(sendEditor(searchDown ? SCI_WORDSTARTPOSITION : SCI_WORDENDPOSITION, getCurrentPos(), true));
		}
	}

	CharacterRange cr = getSelectionRange();
	int startPosition = cr.cpMax;
	int endPosition   = getLength();
	if (!searchDown)
	{
		startPosition = cr.cpMin;
		endPosition   = 0;
	}

	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor(SCI_SETTARGETSTART, startPosition);
	sendEditor(SCI_SETTARGETEND, endPosition);
	sendEditor(SCI_SETSEARCHFLAGS, flags);
	int posFind = sendEditorString(SCI_SEARCHINTARGET, findLen, findWhat);
	if (posFind == -1)
	{
		if (!searchDown)
		{
			startPosition = getLength();
			endPosition   = 0;
		}
		else
		{
			startPosition = 0;
			endPosition   = getLength();
		}
		sendEditor(SCI_SETTARGETSTART, startPosition);
		sendEditor(SCI_SETTARGETEND, endPosition);
		posFind = sendEditorString(SCI_SEARCHINTARGET, findLen, findWhat);
	}
	if (posFind == -1)
	{
		m_firstFoundPos = -1;
		m_haveFound     = false;
		showFindWarning(findWhat);
		//! @todo возможно, надо убрать
		setFocus();
	}
	else
	{
		if (m_firstFoundPos == -1)
		{
			m_firstFoundPos = posFind;
		}
		else if (posFind == m_firstFoundPos)
		{
			m_firstFoundPos = -1;
			m_haveFound     = false;
			showFindWarning(findWhat);
			//! @todo возможно, надо убрать
			setFocus();
			return;
		}
		m_haveFound = true;
		int start   = sendEditor(SCI_GETTARGETSTART);
		int end     = sendEditor(SCI_GETTARGETEND);
		ensureRangeVisible(start, end);
		setSelection(start, end);
	}
}

void EditBase::onSearchReplace() 
{
	m_findReplaceSettings.what = QString::fromLocal8Bit(getCurrentOrSelectedWord().c_str());

	if (!m_pFindReplaceDialog)
	{
		m_pFindReplaceDialog = new FindReplaceDialog(
			this,
			boost::bind(&EditBase::onFindReplaceDlgFind,       this, _1),
			boost::bind(&EditBase::onFindReplaceDlgReplace,    this, _1),
			boost::bind(&EditBase::onFindReplaceDlgReplaceAll, this, _1),
			boost::bind(&EditBase::onFindReplaceDlgClose,      this)
		);
	}
	
	if(m_pGroup)
	{
		m_findReplaceSettings.what.isEmpty() 
			? m_findReplaceSettings.what = m_pGroup->findStr
			: m_pGroup->findStr = m_findReplaceSettings.what;
		m_findReplaceSettings.searchDown = m_pGroup->bSearchDown;
		m_findReplaceSettings.matchCase = m_pGroup->bMatchCase;
		m_findReplaceSettings.matchWholeWord = m_pGroup->bMatchWholeWord;
	}

	m_pFindReplaceDialog->setSettings(m_findReplaceSettings);
	m_pFindReplaceDialog->show();
	m_pFindReplaceDialog->raise();
	m_pFindReplaceDialog->activateWindow();
}

void EditBase::onFindReplaceDlgFind(CREF(FindReplaceDialog::Settings) settings)
{
	m_findReplaceSettings = settings;
	if (m_pGroup)
	{
		m_pGroup->findStr = m_findReplaceSettings.what;
	}
	findNext(m_findReplaceSettings.what, m_findReplaceSettings.searchDown, m_findReplaceSettings.matchCase, m_findReplaceSettings.matchWholeWord);
	updateActionFind(isActivated());
}

void EditBase::onFindReplaceDlgReplace(CREF(FindReplaceDialog::Settings) settings)
{
	m_findReplaceSettings = settings;
	if (m_pGroup)
	{
		m_pGroup->findStr = m_findReplaceSettings.what;
		m_pGroup->replaceStr = m_findReplaceSettings.byWhat;
	}
	if ((!getSelection().empty() && !m_haveFound) || (!getCurrentWord().empty() && !m_haveFound))
	{
		findNext(m_findReplaceSettings.what, m_findReplaceSettings.searchDown, m_findReplaceSettings.matchCase, m_findReplaceSettings.matchWholeWord);	
	}
	replace(m_findReplaceSettings.what, m_findReplaceSettings.byWhat, m_findReplaceSettings.searchDown, m_findReplaceSettings.matchCase, m_findReplaceSettings.matchWholeWord);
	updateActionFind(isActivated());
}

void EditBase::onFindReplaceDlgReplaceAll(CREF(FindReplaceDialog::Settings) settings)
{
	m_findReplaceSettings = settings;
	if (m_pGroup)
	{
		m_pGroup->findStr    = m_findReplaceSettings.what;
		m_pGroup->replaceStr = m_findReplaceSettings.byWhat;
	}
	replaceAll(m_findReplaceSettings.what, m_findReplaceSettings.byWhat, m_findReplaceSettings.matchCase, m_findReplaceSettings.matchWholeWord);
	updateActionFind(isActivated());
}

void EditBase::onFindReplaceDlgClose()
{
	m_pFindReplaceDialog = NULL;
}

void EditBase::showFindWarning(CREF(QString) findWhat)
{
	QMessageBox::warning(this, QString::fromStdWString(L"Результаты поиска"), QString::fromStdWString(L"Невозможно найти строчку '%1'.").arg(findWhat));
}

void EditBase::replace(CREF(QString) findWhat, CREF(QString) replaceWhat, rbool searchDown, rbool matchCase, rbool matchWholeWord)
{
	if (m_haveFound)
	{
		int replaceLen = replaceWhat.length();
		CharacterRange cr = getSelectionRange();
		if (cr.cpMin == cr.cpMax)
		{
			cr.cpMin = sendEditor(SCI_WORDSTARTPOSITION, getCurrentPos(), true);
			cr.cpMax = cr.cpMin + getCurrentWord().length();
		}
		sendEditor(SCI_SETTARGETSTART, cr.cpMin);
		sendEditor(SCI_SETTARGETEND,   cr.cpMax);
		int lenReplaced = replaceLen;
		sendEditorString(SCI_REPLACETARGET, replaceLen, replaceWhat);
		setSelection(cr.cpMin + lenReplaced, cr.cpMin);
		m_haveFound = false;
	}
	findNext(findWhat, searchDown, matchCase, matchWholeWord);
}

void EditBase::replaceAll(CREF(QString) findWhat, CREF(QString) replaceWhat, rbool matchCase, rbool matchWholeWord)
{
	int findLen = findWhat.length();
	if (!findLen)
		return;

	int startPosition = 0;
	int endPosition   = getLength();

	int replaceLen = replaceWhat.length();
	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor(SCI_SETTARGETSTART, startPosition);
	sendEditor(SCI_SETTARGETEND, endPosition);
	sendEditor(SCI_SETSEARCHFLAGS, flags);
	int posFind = sendEditorString(SCI_SEARCHINTARGET, findLen, findWhat);

	if ((posFind != -1) && (posFind <= endPosition))
	{
		int lastMatch = posFind;
		sendEditor(SCI_BEGINUNDOACTION);
		while (posFind != -1)
		{
			int lenTarget = sendEditor(SCI_GETTARGETEND) - sendEditor(SCI_GETTARGETSTART);
			int lenReplaced = replaceLen;
			sendEditorString(SCI_REPLACETARGET, replaceLen, replaceWhat);
			endPosition += lenReplaced - lenTarget;
			lastMatch    = posFind + lenReplaced;
			if (lenTarget <= 0)
				++lastMatch;
			sendEditor(SCI_SETTARGETSTART, lastMatch);
			sendEditor(SCI_SETTARGETEND, endPosition);
			posFind = sendEditorString(SCI_SEARCHINTARGET, findLen, findWhat);
		}
		setSelection(lastMatch, lastMatch);
		sendEditor(SCI_ENDUNDOACTION);
	}
	else
	{
		showFindWarning(findWhat);
		//! @todo возможно, надо убрать
		setFocus();
	}
}

void EditBase::clearAll()
{
	rbool readOnly = isReadOnly();
	setReadOnly(false);
	sendEditor(SCI_CLEARALL);
	setReadOnly(readOnly);
}

rbool EditBase::bookmarkToggle(int line) const
{
	if (line == -1)
	{
		line = getCurrentLineNumber();
	}
	int state = sendEditor(SCI_MARKERGET, line);
	if (state & (1 << m_sciMarkerBookmark))
	{
		sendEditor(SCI_MARKERDELETE, line, m_sciMarkerBookmark);
		return false;
	}
	else
	{
		sendEditor(SCI_MARKERADD, line, m_sciMarkerBookmark);
		return true;
	}
}

rbool EditBase::bookmarkNext(rbool canLoop, rbool fromCurrentLine) const
{
	rbool wasFound = false;
	rbool wasLoop  = false;

	int line = fromCurrentLine
		? getCurrentLineNumber()
		: -1;

	int nextLine = sendEditor(SCI_MARKERNEXT, line + 1, 1 << m_sciMarkerBookmark);
	if (nextLine < 0)
	{
		wasLoop = true;
		if (canLoop)
		{
			nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerBookmark);
		}
	}
	if (((canLoop && wasLoop) || !wasLoop) && nextLine >= 0 && nextLine != line)
	{
		gotoLineEnsureVisible(nextLine);
		wasFound = true;
	}

	return wasFound;
}

rbool EditBase::bookmarkPrev(rbool canLoop, rbool fromCurrentLine) const
{
	rbool wasFound = false;
	rbool wasLoop  = false;

	int lineCount = getLineCount();
	int line = fromCurrentLine
		? getCurrentLineNumber()
		: lineCount + 1;

	int prevLine  = sendEditor(SCI_MARKERPREVIOUS, line - 1, 1 << m_sciMarkerBookmark);
	if (prevLine < 0)
	{
		wasLoop = true;
		if (canLoop)
		{
			prevLine = sendEditor(SCI_MARKERPREVIOUS, lineCount, 1 << m_sciMarkerBookmark);
		}
	}
	if (((canLoop && wasLoop) || !wasLoop) && prevLine >= 0 && prevLine != line)
	{
		gotoLineEnsureVisible(prevLine);
		wasFound = true;
	}

	return wasFound;
}

void EditBase::bookmarkClearAll() const
{
	sendEditor(SCI_MARKERDELETEALL, m_sciMarkerBookmark);
}

rbool EditBase::hasBookmarks() const
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerBookmark);
	return nextLine >= 0;
}

void EditBase::onCopyAsRTF(QMimeData* pMimeData)
{
	if (!isSelected())
		return;

	CharacterRange cr = getSelectionRange();
	tstring result = saveAsRTF(cr.cpMin, cr.cpMax);
	if (result.empty())
		return;

	QByteArray ba;
	ba.append(result.c_str());
	//! @todo для линуха надо будет использовать "text/rtf" ?
	pMimeData->setData("Rich Text Format", ba);
}

// --------------------------------------------------------------------------------
// -------------------- EditBase: some functions for RTF export ---------- BEGIN
// --------------------------------------------------------------------------------
int GetHexChar(char ch) // 'H'
{
	return ch > '9' ? (ch | 0x20) - 'a' + 10 : ch - '0';
}

int GetHexByte(const char *hexbyte) // "HH"
{
	return (GetHexChar(*hexbyte) << 4) | GetHexChar(hexbyte[1]);
}

void GetRTFStyleChange(char *delta, char *last, const char *current) // \f0\fs20\cf0\highlight0\b0\i0
{
	int lastLen = strlen(last), offset = 2, lastOffset, currentOffset, len;
	*delta = '\0';
	// font face
	lastOffset = offset + 1;
	while (last[lastOffset] != '\\')
		lastOffset++;
	currentOffset = offset + 1;
	while (current[currentOffset] != '\\')
		currentOffset++;
	if (lastOffset != currentOffset ||        // change
	        strncmp(last + offset, current + offset, lastOffset - offset)) {
		if (lastOffset != currentOffset) {
			memmove (last + currentOffset, last + lastOffset, lastLen - lastOffset + 1);
			lastLen += currentOffset - lastOffset;
		}
		len = currentOffset - offset;
		memcpy(last + offset, current + offset, len);
#pragma warning(disable: 4996)
		strcat (delta, RTF_SETFONTFACE);
#pragma warning(default: 4996)
		lastOffset = strlen(delta);
		memcpy(delta + lastOffset, last + offset, len);
		delta[lastOffset + len] = '\0';
	}
	offset = currentOffset + 3;
	// size
	lastOffset = offset + 1;
	while (last[lastOffset] != '\\')
		lastOffset++;
	currentOffset = offset + 1;
	while (current[currentOffset] != '\\')
		currentOffset++;
	if (lastOffset != currentOffset ||        // change
	        strncmp(last + offset, current + offset, lastOffset - offset)) {
		if (lastOffset != currentOffset) {
			memmove (last + currentOffset, last + lastOffset, lastLen - lastOffset + 1);
			lastLen += currentOffset - lastOffset;
		}
		len = currentOffset - offset;
		memcpy(last + offset, current + offset, len);
#pragma warning(disable: 4996)
		strcat (delta, RTF_SETFONTSIZE);
#pragma warning(default: 4996)
		lastOffset = strlen(delta);
		memcpy(delta + lastOffset, last + offset, len);
		delta[lastOffset + len] = '\0';
	}
	offset = currentOffset + 3;
	// color
	lastOffset = offset + 1;
	while (last[lastOffset] != '\\')
		lastOffset++;
	currentOffset = offset + 1;
	while (current[currentOffset] != '\\')
		currentOffset++;
	if (lastOffset != currentOffset ||        // change
	        strncmp(last + offset, current + offset, lastOffset - offset)) {
		if (lastOffset != currentOffset) {
			memmove (last + currentOffset, last + lastOffset, lastLen - lastOffset + 1);
			lastLen += currentOffset - lastOffset;
		}
		len = currentOffset - offset;
		memcpy(last + offset, current + offset, len);
#pragma warning(disable: 4996)
		strcat (delta, RTF_SETCOLOR);
#pragma warning(default: 4996)
		lastOffset = strlen(delta);
		memcpy(delta + lastOffset, last + offset, len);
		delta[lastOffset + len] = '\0';
	}
	offset = currentOffset + 10;
	// background
	lastOffset = offset + 1;
	while (last[lastOffset] != '\\')
		lastOffset++;
	currentOffset = offset + 1;
	while (current[currentOffset] != '\\')
		currentOffset++;
	if (lastOffset != currentOffset ||        // change
	        strncmp(last + offset, current + offset, lastOffset - offset)) {
		if (lastOffset != currentOffset) {
			memmove (last + currentOffset, last + lastOffset, lastLen - lastOffset + 1);
			lastLen += currentOffset - lastOffset;
		}
		len = currentOffset - offset;
		memcpy(last + offset, current + offset, len);
#pragma warning(disable: 4996)
		strcat (delta, RTF_SETBACKGROUND);
#pragma warning(default: 4996)
		lastOffset = strlen(delta);
		memcpy(delta + lastOffset, last + offset, len);
		delta[lastOffset + len] = '\0';
	}
	offset = currentOffset + 2;
	// bold
	if (last[offset] != current[offset]) {
		if (current[offset] == '\\') { // turn on
			memmove (last + offset, last + offset + 1, lastLen-- - offset);
#pragma warning(disable: 4996)
			strcat (delta, RTF_BOLD_ON);
#pragma warning(default: 4996)
			offset += 2;
		} else { // turn off
			memmove (last + offset + 1, last + offset, ++lastLen - offset);
			last[offset] = '0';
#pragma warning(disable: 4996)
			strcat (delta, RTF_BOLD_OFF);
#pragma warning(default: 4996)
			offset += 3;
		}
	} else
		offset += current[offset] == '\\' ? 2 : 3;
	// italic
	if (last[offset] != current[offset]) {
		if (current[offset] == '\\') { // turn on
			memmove (last + offset, last + offset + 1, lastLen-- - offset);
#pragma warning(disable: 4996)
			strcat (delta, RTF_ITALIC_ON);
#pragma warning(default: 4996)
		} else { // turn off
			memmove (last + offset + 1, last + offset, ++lastLen - offset);
			last[offset] = '0';
#pragma warning(disable: 4996)
			strcat (delta, RTF_ITALIC_OFF);
#pragma warning(default: 4996)
		}
	}
	if (*delta) {
		lastOffset = strlen(delta);
		delta[lastOffset] = ' ';
		delta[lastOffset + 1] = '\0';
	}
}
// --------------------------------------------------------------------------------
// -------------------- EditBase: some functions for RTF export ---------- END
// --------------------------------------------------------------------------------

tstring EditBase::saveAsRTF(int start, int end) const
{
	tstring saveStr;

	if (!m_pStyle)
	{
		return saveStr;
	}

	int lengthDoc = getLength();
	if (end < 0) end = lengthDoc;
	sendEditor(SCI_COLOURISE, 0, -1);
	char styles[STYLE_DEFAULT + 1][MAX_STYLEDEF];
	char fonts[STYLE_DEFAULT + 1][MAX_FONTDEF];
	char colors[STYLE_DEFAULT + 1][MAX_COLORDEF];
	char lastStyle[MAX_STYLEDEF], deltaStyle[MAX_STYLEDEF];
	int fontCount  = 1;
	int colorCount = 1;
	int i;

	saveStr = "";
	saveStr += RTF_HEADEROPEN;
	saveStr += RTF_FONTDEFOPEN;

#pragma warning(disable: 4996)
	strncpy(*fonts, m_pStyle->font->name.c_str(), MAX_FONTDEF);
#pragma warning(default: 4996)
	saveStr += rdo::format(RTF_FONTDEF, 0, m_pStyle->font->characterSet, m_pStyle->font->name.c_str());
#pragma warning(disable: 4996)
	strncpy(*colors, "#000000", MAX_COLORDEF);
#pragma warning(default: 4996)

	EditBaseTheme* theme = static_cast<EditBaseTheme*>(m_pStyle->theme);

	for (int istyle = 0; istyle <= STYLE_DEFAULT; istyle++) {
		if (theme->styleUsing(istyle)) {
#pragma warning(disable: 4996)
			sprintf(lastStyle, RTF_SETFONTFACE "%d", fontCount-1);
			sprintf(lastStyle + strlen(lastStyle), RTF_SETFONTSIZE "%d", m_pStyle->font->size * 2);
#pragma warning(default: 4996)
			if (theme->styleDefault(istyle)) {
#pragma warning(disable: 4996)
				strncpy(colors[colorCount++], theme->styleBGColorToHEX(istyle).c_str(), MAX_COLORDEF);
#pragma warning(default: 4996)
			}
#pragma warning(disable: 4996)
			strncpy(colors[colorCount++], theme->styleFGColorToHEX(istyle).c_str(), MAX_COLORDEF);
#pragma warning(default: 4996)
			rbool bold   = theme->styleBold(istyle);
			rbool italic = theme->styleItalic(istyle);
#pragma warning(disable: 4996)
			sprintf(lastStyle + strlen(lastStyle), RTF_SETCOLOR "%d", colorCount-1);
			sprintf(lastStyle + strlen(lastStyle), RTF_SETBACKGROUND "%d", 1);
			strcat(lastStyle, bold ? RTF_BOLD_ON : RTF_BOLD_OFF);
			strcat(lastStyle, italic ? RTF_ITALIC_ON : RTF_ITALIC_OFF);
			strncpy(styles[istyle], lastStyle, MAX_STYLEDEF);
#pragma warning(default: 4996)
		}
	}
	saveStr += RTF_FONTDEFCLOSE;
	saveStr += RTF_COLORDEFOPEN;

	for (i = 0; i < colorCount; i++) {
		saveStr += rdo::format(RTF_COLORDEF, GetHexByte(colors[i] + 1), GetHexByte(colors[i] + 3), GetHexByte(colors[i] + 5));
	}

	saveStr += rdo::format(RTF_COLORDEFCLOSE RTF_HEADERCLOSE RTF_BODYOPEN RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0 ", m_pStyle->font->size * 2);

#pragma warning(disable: 4996)
	sprintf(lastStyle, RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0" RTF_SETBACKGROUND "0" RTF_BOLD_OFF RTF_ITALIC_OFF, m_pStyle->font->size * 2);
#pragma warning(default: 4996)

	tstring::size_type prevLength = saveStr.length();
	rbool prevCR = false;
	int styleCurrent = -1;
	for (i = start; i < end; i++)
	{
		int m_pStyle = sendEditor(SCI_GETSTYLEAT, i);
		if (!theme->styleUsing(m_pStyle))
		{
			continue;
		}

		char ch = (char)sendEditor(SCI_GETCHARAT, i);
		if (m_pStyle != styleCurrent)
		{
			GetRTFStyleChange(deltaStyle, lastStyle, styles[m_pStyle]);
			if (*deltaStyle)
			{
				saveStr += deltaStyle;
			}
			styleCurrent = m_pStyle;
		}
		if (ch == '\\')
		{
			saveStr += "\\\\";
		}
		else if (ch == '\t')
		{
			saveStr += RTF_TAB;
		}
		else if (ch == '\n')
		{
			if (!prevCR)
			{
				saveStr += RTF_EOLN;
			}
		}
		else if (ch == '\r')
		{
			saveStr += RTF_EOLN;
		}
		else
		{
			saveStr += ch;
		}

		prevCR = ch == '\r';
	}
	rbool wasGenerated = prevLength != saveStr.length();
	if (wasGenerated)
	{
		saveStr += RTF_BODYCLOSE;
	}
	else
	{
		saveStr.clear();
	}

	return saveStr;
}
#pragma warning(default: 4996)

void EditBase::setCurrentPos(const int value) const
{
	sendEditor(SCI_SETCURRENTPOS, value);
	sendEditor(SCI_SETSELECTIONSTART, value);
	sendEditor(SCI_SETSELECTIONEND, value);
}

void EditBase::setCurrentPos(const int line, int pos_in_line, const rbool convert_rdo_tab) const
{
	int pos = getPositionFromLine(line);
	int line_length = sendEditor(SCI_LINELENGTH, line);
	char currentLine[8000];
	rbool canUseLine = false;
	if (line_length < 8000)
	{
		sendEditor(SCI_GETLINE, line, (long)currentLine);
		while (currentLine[line_length-1] == 0x0A || currentLine[line_length-1] == 0x0D)
		{
			--line_length;
		}
		canUseLine = true;
	}
	else
	{
		line_length -= 2;
	}

	int new_line_length = line_length;

	if (canUseLine && convert_rdo_tab)
	{
		int tab_size = m_pStyle ? m_pStyle->tab->tabSize : 8;
		int spaces = 0;
		for (int i = 0; i < line_length; i++)
		{
			if (currentLine[i] == ' ')
			{
				spaces++;
			}
			else if (currentLine[i] == '\t')
			{
				int delta = spaces % 8;
				pos_in_line     -= 7 - delta;
				new_line_length += tab_size + delta;
				spaces = 0;
			}
			else
			{
				spaces = 0;
			}
		}
	}

	pos += (std::min)(new_line_length, pos_in_line);

	int line_to_scroll = line > 0 ? line - 1 : 0;
	scrollToLine(line_to_scroll);
	sendEditor(SCI_GOTOPOS, pos);

	setCurrentPos(pos);
}

rbool EditBase::isLineVisible(const int line) const
{
	int first_line = sendEditor(SCI_GETFIRSTVISIBLELINE);
	int last_line = first_line + sendEditor(SCI_LINESONSCREEN);
	return line >= first_line && line <= last_line;
}

void EditBase::appendText(CREF(QString) str) const
{
	QByteArray text = str.toLocal8Bit();
	sendEditorString(SCI_ADDTEXT, text.size(), text.constData());
}

void EditBase::scrollToLine(const int line) const
{
	sendEditor(SCI_LINESCROLL, 0, line - sendEditor(SCI_GETFIRSTVISIBLELINE) - sendEditor(SCI_LINESONSCREEN)/3);
	setCurrentPos(getPositionFromLine(line));
}

void EditBase::scrollToLine2(const int line) const
{
	sendEditor(SCI_LINESCROLL, 0, line);
}

void EditBase::scrollToCarret() const
{
	sendEditor(SCI_SCROLLCARET);
}

void EditBase::horzScrollToCurrentPos() const
{
	setCurrentPos(getCurrentPos());
	sendEditor(SCI_REPLACESEL, 0, 0);
}

void EditBase::load(rdo::stream& stream)
{
	rbool readOnly = isReadOnly();
	setReadOnly(false);

	sendEditorString(SCI_ADDTEXT, stream.str().length(), &stream.str()[0]);

	setReadOnly(readOnly);
}

void EditBase::save(rdo::stream& stream) const
{
	int len = getLength();
	std::vector<char> str;
	str.resize(len + 1);
	sendEditorString(SCI_GETTEXT, len + 1, &str[0]);
//	str[len] = "\0";
//	str.resize(len);
	stream.str(&str[0]); // qq
}

int EditBase::indentOfBlock(int line) const
{
	if (line < 0)
		return 0;

	return sendEditor(SCI_GETLINEINDENTATION, line);
}

void EditBase::setLineIndentation(int line, int indent) const
{
	if (indent < 0) return;
	CharacterRange crange = getSelectionRange();
	int posBefore = sendEditor(SCI_GETLINEINDENTPOSITION, line);
	sendEditor(SCI_SETLINEINDENTATION, line, indent);
	int posAfter = sendEditor(SCI_GETLINEINDENTPOSITION, line);
	int posDifference = posAfter - posBefore;
	if (posAfter > posBefore)
	{
		if (crange.cpMin >= posBefore)
		{
			crange.cpMin += posDifference;
		}
		if (crange.cpMax >= posBefore)
		{
			crange.cpMax += posDifference;
		}
	}
	else if (posAfter < posBefore)
	{
		if (crange.cpMin >= posAfter)
		{
			if (crange.cpMin >= posBefore)
			{
				crange.cpMin += posDifference;
			}
			else
			{
				crange.cpMin = posAfter;
			}
		}
		if (crange.cpMax >= posAfter)
		{
			if (crange.cpMax >= posBefore)
			{
				crange.cpMax += posDifference;
			}
			else
			{
				crange.cpMax = posAfter;
			}
		}
	}
	setSelection(crange.cpMin, crange.cpMax);
}

void EditBase::autoIndent() const
{
	CharacterRange crange = getSelectionRange();
	int selStart      = crange.cpMin;
	int curLine       = getCurrentLineNumber();
	int thisLineStart = getPositionFromLine(curLine);
	int indentBlock   = indentOfBlock(curLine - 1);

	if (selStart == thisLineStart)
	{
		setLineIndentation(curLine, indentBlock);
	}
}

void EditBase::onSearchBookmarkToggle()
{
	bookmarkToggle();
}

void EditBase::onSearchBookmarkNext() const
{
	onSearchBookmarkNextPrev(
		boost::bind(&EditBase::bookmarkNext, _1, _2, _3),
		boost::bind(&EditBase::Group::next, m_pGroup, _1)
	);
}

void EditBase::onSearchBookmarkPrev() const
{
	onSearchBookmarkNextPrev(
		boost::bind(&EditBase::bookmarkPrev, _1, _2, _3),
		boost::bind(&EditBase::Group::prev, m_pGroup, _1)
	);
}

void EditBase::onSearchBookmarkNextPrev(
	const boost::function<rbool (const EditBase*, rbool, rbool)>& nextPrevFun,
	const boost::function<Group::List::const_iterator (const Group::List::const_iterator& it)>& nextPrevGroup
) const
{
	if (nextPrevFun(this, false, true))
	{
		return;
	}

	if (!m_pGroup)
	{
		nextPrevFun(this, true, true);
	}
	else
	{
		Group::List::const_iterator it = std::find(m_pGroup->begin(), m_pGroup->end(), this);
		ASSERT(it != m_pGroup->end());

		for (;;)
		{
			it = nextPrevGroup(it);

			if (*it == this)
			{
				break;
			}

			if (nextPrevFun(*it, false, false))
			{
				QWidget* pParent = (*it)->parentWidget()->parentWidget();
				ASSERT(pParent);
				QTabWidget* pTabWidget = dynamic_cast<QTabWidget*>(pParent);
				if (pTabWidget)
				{
					pTabWidget->setCurrentWidget(*it);
				}
				break;
			}
		}
	}
}

void EditBase::onSearchBookmarkClearAll()
{
	methodOfGroup(boost::bind(&EditBase::bookmarkClearAll, _1));
}

void EditBase::onUpdateEditGUI()
{
	onUpdateActions(isActivated());
}

void EditBase::onViewShowWhiteSpace() 
{
	methodOfGroup(boost::bind(&EditBase::setViewWhiteSpace, _1, !isViewWhiteSpace()));
}

void EditBase::onViewShowEndOfLine() 
{
	methodOfGroup(boost::bind(&EditBase::setViewEndOfLine, _1, !isViewEndOfLine()));
}

void EditBase::onViewZoomChanged(int)
{
	onUpdateActions(isActivated());
}

void EditBase::onViewZoomInc()
{
	methodOfGroup(boost::bind(&EditBase::zoomIn, _1));
	onUpdateActions(isActivated());
}

void EditBase::onViewZoomDec() 
{
	methodOfGroup(boost::bind(&EditBase::zoomOut, _1));
	onUpdateActions(isActivated());
}

void EditBase::onViewZoomReset() 
{
	methodOfGroup(boost::bind(&EditBase::resetZoom, _1));
	onUpdateActions(isActivated());
}

int EditBase::findPos(CREF(QString) findWhat, const int startFromLine, const rbool matchCase, const rbool matchWholeWord) const
{
	int findLen = findWhat.length();
	if (!findLen)
		return -1;

	int startPosition = getPositionFromLine(startFromLine);
	int endPosition   = getLength();

	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor(SCI_SETTARGETSTART, startPosition);
	sendEditor(SCI_SETTARGETEND, endPosition);
	sendEditor(SCI_SETSEARCHFLAGS, flags);
	return sendEditorString(SCI_SEARCHINTARGET, findLen, findWhat);
}

tstring EditBase::getLine(const int line) const
{
	int length = sendEditor(SCI_LINELENGTH, line);
	tstring str;
	str.resize(length);
	sendEditor(SCI_GETLINE, line, (long)str.data());
	return str;
}

void EditBase::onSearchGotoLine()
{
	GoToLineDialog dialog(this, getCurrentLineNumber() + 1, getLineCount());
	if (dialog.exec() == QDialog::Accepted)
	{
		setCurrentPos(dialog.getLine() - 1, 0);
	}
}

void EditBase::focusInEvent(QFocusEvent* pEvent)
{
	super::focusInEvent(pEvent);
	activate(pEvent);
}

void EditBase::focusOutEvent(QFocusEvent* pEvent)
{
	super::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void EditBase::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actEditUndo,
		activated && sendEditor(SCI_CANUNDO),
		this, &EditBase::onEditUndo
	);
	updateAction(
		pMainWindow->actEditRedo,
		activated && sendEditor(SCI_CANREDO),
		this, &EditBase::onEditRedo
	);
	updateAction(
		pMainWindow->actEditCut,
		activated && !isReadOnly() && isSelected(),
		this, &EditBase::onEditCut
	);
	updateAction(
		pMainWindow->actEditCopy,
		activated && isSelected(),
		this, &EditBase::onEditCopy
	);
	updateAction(
		pMainWindow->actEditPaste,
		activated && sendEditor(SCI_CANPASTE),
		this, &EditBase::onEditPaste
	);
	updateAction(
		pMainWindow->actEditDel,
		activated && !isReadOnly() && (getCurrentPos() != getLength() || isSelected()),
		this, &EditBase::onEditDel
	);
	updateAction(
		pMainWindow->actEditSelectAll,
		activated && !isEmpty(),
		this, &EditBase::onEditSelectAll
	);
	updateAction(
		pMainWindow->actEditUpperCase,
		activated && !isReadOnly() && isSelected(),
		this, &EditBase::onEditUpperCase
	);
	updateAction(
		pMainWindow->actEditLowerCase,
		activated && !isReadOnly() && isSelected(),
		this, &EditBase::onEditLowerCase
	);
	updateAction(
		pMainWindow->actViewShowWhiteSpace,
		activated,
		this, &EditBase::onViewShowWhiteSpace
	);
	pMainWindow->actViewShowWhiteSpace->setChecked(isViewWhiteSpace());

	updateAction(
		pMainWindow->actViewShowEndOfLine,
		activated,
		this, &EditBase::onViewShowEndOfLine
	);
	pMainWindow->actViewShowEndOfLine->setChecked(isViewEndOfLine());

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, &EditBase::onHelpContext
	);
	updateAction(
		pMainWindow->actViewZoomInc,
		activated &&  getZoom() < 20,
		this, &EditBase::onViewZoomInc
	);
	updateAction(
		pMainWindow->actViewZoomDec,
		activated && getZoom() > -10,
		this, &EditBase::onViewZoomDec
	);
	updateAction(
		pMainWindow->actViewZoomReset,
		activated && getZoom(),
		this, &EditBase::onViewZoomReset
	);

	rbool hasBookmark = predicateOfGroup(boost::bind(&EditBase::hasBookmarks, _1));

	updateAction(
		pMainWindow->actSearchBookmarkNext,
		activated && hasBookmark,
		this, &EditBase::onSearchBookmarkNext
	);
	updateAction(
		pMainWindow->actSearchBookmarkPrev,
		activated && hasBookmark,
		this, &EditBase::onSearchBookmarkPrev
	);
	updateAction(
		pMainWindow->actSearchBookmarksToggle,
		activated,
		this, &EditBase::onSearchBookmarkToggle
	);
	updateAction(
		pMainWindow->actSearchBookmarksClearAll,
		activated && hasBookmark,
		this, &EditBase::onSearchBookmarkClearAll
	);
	updateAction(
		pMainWindow->actSearchGotoLine,
		activated,
		this, &EditBase::onSearchGotoLine
	);
	updateAction(
		pMainWindow->actSearchFind,
		activated,
		this, &EditBase::onSearchFind
	);

	updateActionFind(activated);

	QString modify = activated
		? isReadOnly()
			? QString::fromStdWString(L"Только чтение")
			: isModify()
				? QString::fromStdWString(L"Изменён")
				: QString()
		: QString();

	pMainWindow->statusBar()->update<StatusBar::SB_MODIFY>(modify);

	QString coord = activated
		? QString("%1: %2").arg(getCurrentColumnNumber() + 1).arg(getCurrentLineNumber() + 1)
		: QString();

	pMainWindow->statusBar()->update<StatusBar::SB_COORD>(coord);

	QString overwrite = activated && sendEditor(SCI_GETOVERTYPE)
		? QString::fromStdWString(L"Замена")
		: QString();

	pMainWindow->statusBar()->update<StatusBar::SB_OVERWRITE>(overwrite);
}

void EditBase::updateActionFind(rbool activated)
{
	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actSearchFind,
		activated && !isEmpty(),
		this, &EditBase::onSearchFind
	);

	updateAction(
		pMainWindow->actSearchReplace,
		activated && !isEmpty() && !isReadOnly(),
		this, &EditBase::onSearchReplace
	);

	rbool findNextPrev = activated && (!m_findReplaceSettings.what.isEmpty() || !m_findSettings.what.isEmpty() || (m_pGroup && !m_pGroup->findStr.isEmpty()));
	updateAction(
		pMainWindow->actSearchFindNext,
		findNextPrev,
		this, &EditBase::onSearchFindNext
	);

	updateAction(
		pMainWindow->actSearchFindPrevious,
		findNextPrev,
		this, &EditBase::onSearchFindPrevious
	);

	updateAction(
		pMainWindow->actSearchFindNextCurrent,
		activated,
		this, &EditBase::onSearchFindNextCurrent
	);

	updateAction(
		pMainWindow->actSearchFindPreviousCurrent,
		activated,
		this, &EditBase::onSearchFindPreviousCurrent
	);
}

void EditBase::onUpdateModify()
{
	emit modifyChanged(isModify());
}

rbool EditBase::isViewWhiteSpace() const
{
	return sendEditor(SCI_GETVIEWWS) != SCWS_INVISIBLE;
}

void EditBase::setViewWhiteSpace(rbool value)
{
	sendEditor(SCI_SETVIEWWS, value ? SCWS_VISIBLEALWAYS : SCWS_INVISIBLE);
}

rbool EditBase::isViewEndOfLine() const
{
	return sendEditor(SCI_GETVIEWEOL) ? true : false;
}

void EditBase::setViewEndOfLine(rbool value)
{
	sendEditor(SCI_SETVIEWEOL, value);
}

void EditBase::methodOfGroup(CREF(this_method) fun)
{
	m_pGroup
		? m_pGroup->for_each(fun)
		: fun(this);
}

rbool EditBase::predicateOfGroup(CREF(this_predicate) fun) const
{
	return m_pGroup
		? m_pGroup->find_if(fun) != m_pGroup->end()
		: fun(this);
}
