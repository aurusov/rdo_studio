/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobaseedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <QtGui/qmessagebox.h>
#include <QtGui/qclipboard.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/sci/include/SciLexer.h"
#include "thirdparty/sci/lexlib/WordList.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEdit::Group
// ---------------------------------------------------------------------------
RDOBaseEdit::Group::Group()
	: bMatchCase     (false)
	, bMatchWholeWord(false)
	, bSearchDown    (true )
{}

void RDOBaseEdit::Group::insert(PTR(RDOBaseEdit) pEdit)
{
	m_list.push_back(pEdit);
}

RDOBaseEdit::Group::List::const_iterator RDOBaseEdit::Group::begin() const
{
	return m_list.begin();
}

RDOBaseEdit::Group::List::const_iterator RDOBaseEdit::Group::end() const
{
	return m_list.end();
}

void RDOBaseEdit::Group::for_each(CREF(this_method) fun) const
{
	boost::range::for_each(m_list, fun);
}

RDOBaseEdit::Group::List::const_iterator RDOBaseEdit::Group::find_if(CREF(this_predicate) fun) const
{
	return boost::range::find_if(m_list, fun);
}

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEdit
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
static const UINT FIND_REPLASE_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

// ON_UPDATE_COMMAND_UI сделано

//! @todo qt
//BEGIN_MESSAGE_MAP( RDOBaseEdit, CWnd )
//	ON_WM_SETFOCUS()
//	ON_WM_CONTEXTMENU()
//	ON_COMMAND(ID_SEARCH_FIND, OnSearchFind)
//	ON_COMMAND(ID_SEARCH_REPLACE, OnSearchReplace)
//	ON_COMMAND(ID_SEARCH_FIND_NEXT, OnSearchFindNext)
//	ON_COMMAND(ID_SEARCH_FIND_PREVIOUS, OnSearchFindPrevious)
//	ON_COMMAND(ID_SEARCH_FIND_NEXT_FAST, OnSearchFindNextFast)
//	ON_COMMAND(ID_SEARCH_FIND_PREVIOUS_FAST, OnSearchFindPreviousFast)
//	ON_UPDATE_COMMAND_UI(ID_SEARCH_FIND_NEXT, OnUpdateSearchFindNextPrev)
//	ON_UPDATE_COMMAND_UI(ID_SEARCH_FIND, OnUpdateSearchFind)
//	ON_UPDATE_COMMAND_UI(ID_SEARCH_REPLACE, OnUpdateSearchReplace)
//	ON_COMMAND( ID_SEARCH_BOOKMARK_TOGGLE  , OnBookmarkToggle )
//	ON_COMMAND( ID_SEARCH_BOOKMARK_NEXT    , OnBookmarkNext )
//	ON_COMMAND( ID_SEARCH_BOOKMARK_PREVIOUS, OnBookmarkPrev )
//	ON_COMMAND( ID_SEARCH_BOOKMARKS_CLEAR  , OnBookmarkClearAll )
//	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARK_NEXT, OnHasBookmarks )
//	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomIn)
//	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomOut)
//	ON_COMMAND(ID_VIEW_ZOOMRESET, OnViewZoomReset)
//	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARK_PREVIOUS, OnHasBookmarks )
//	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKS_CLEAR  , OnHasBookmarks )
//	ON_UPDATE_COMMAND_UI( ID_SEARCH_FIND_PREVIOUS, OnUpdateSearchFindNextPrev )
//	ON_COMMAND(ID_SEARCH_GOTO_LINE, OnSearchGotoLine)
//
//	ON_REGISTERED_MESSAGE( FIND_REPLASE_MSG, OnFindReplaceMsg )
//
//END_MESSAGE_MAP()

RDOBaseEdit::RDOBaseEdit(PTR(QWidget) pParent):
	super(pParent),
	GUI_HAS_BOOKMARK( false ),
	markerCount( 0 ),
	popupMenu( NULL ),
	style( NULL ),
	m_pGroup( NULL ),
	firstFoundPos( -1 ),
	bHaveFound( false )
{
	QObject::connect(this, SIGNAL(needShown(int, int)), this, SLOT(catchNeedShown(int, int)));
	QObject::connect(this, SIGNAL(charAdded(int)),      this, SLOT(catchCharAdded(int)));
	QObject::connect(this, SIGNAL(updateUi()),          this, SLOT(onUpdateEditGUI()));
	QObject::connect(this, SIGNAL(modified(int, int, int, int, const QByteArray&, int, int, int)), this, SLOT(onUpdateModify()));
	QObject::connect(this, SIGNAL(aboutToCopy(QMimeData*)), this, SLOT(onCopyAsRTF(QMimeData*)));

	sci_MARKER_BOOKMARK = getNewMarker();

	sendEditor( SCI_SETLEXER, SCLEX_NULL );
	//	int lexLanguage = sendEditor( SCI_GETLEXER );

	sendEditor( SCI_SETMARGINWIDTHN, 1, 0 );

	sendEditor( SCI_USEPOPUP, 0 );
}

RDOBaseEdit::~RDOBaseEdit()
{}

//! @todo qt
//void RDOBaseEdit::OnSetFocus( CWnd* pOldWnd )
//{
//	CWnd::OnSetFocus( pOldWnd );
//	if ( sciHWND ) {
//		::SetFocus( sciHWND );
//	}
//	updateAllGUI();
//}

void RDOBaseEdit::catchNeedShown(int position, int length)
{
	ensureRangeVisible(position, position + length, false);
}

void RDOBaseEdit::catchCharAdded(int ch)
{
	if ( style && style->tab->autoIndent && ( ch == '\r' || ch == '\n' ) )
		autoIndent();
}

//! @todo qt
//void RDOBaseEdit::OnContextMenu( CWnd* pWnd, QPoint pos )
//{
//	CWnd::OnContextMenu( pWnd, pos );
//	if ( popupMenu ) popupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x(), pos.y(), this );
//}

int RDOBaseEdit::getNewMarker()
{
	markerCount++;
	return markerCount;
}

void RDOBaseEdit::defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const
{
  sendEditor( SCI_MARKERDEFINE, marker, markerType );
  sendEditor( SCI_MARKERSETFORE, marker, fore );
  sendEditor( SCI_MARKERSETBACK, marker, back );
}

void RDOBaseEdit::setEditorStyle( RDOBaseEditStyle* _style )
{
	style = _style;
	if ( !style ) return;

	// ----------
	// Colors
	RDOBaseEditTheme* theme = static_cast<RDOBaseEditTheme*>(style->theme);
	sendEditor( SCI_STYLESETBACK, STYLE_DEFAULT, theme->defaultColor );
	sendEditor( SCI_STYLESETBACK, STYLE_DEFAULT, theme->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_TEXT_DEFAULT, theme->defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_TEXT_DEFAULT, theme->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , STYLE_DEFAULT, theme->defaultStyle & RDOStyleFont::BOLD      );
	sendEditor( SCI_STYLESETITALIC   , STYLE_DEFAULT, theme->defaultStyle & RDOStyleFont::ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, STYLE_DEFAULT, theme->defaultStyle & RDOStyleFont::UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_TEXT_DEFAULT, theme->defaultStyle & RDOStyleFont::BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_TEXT_DEFAULT, theme->defaultStyle & RDOStyleFont::ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_TEXT_DEFAULT, theme->defaultStyle & RDOStyleFont::UNDERLINE );

	// ----------
	// Font Name
	sendEditorString( SCI_STYLESETFONT, STYLE_DEFAULT, style->font->name.c_str() );

	// ----------
	// Font Size
	sendEditor( SCI_STYLESETSIZE, STYLE_DEFAULT, style->font->size );

	// ----------
	// Codepage and Characterset
	sendEditor( SCI_SETCODEPAGE, style->font->codepage );
	sendEditor( SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, style->font->characterSet );

	// ----------
	// Tabs
	sendEditor( SCI_SETTABWIDTH, style->tab->tabSize );
	sendEditor( SCI_SETINDENT, style->tab->indentSize );
	sendEditor( SCI_SETUSETABS, style->tab->useTabs );
	sendEditor( SCI_SETTABINDENTS, style->tab->tabIndents );
	sendEditor( SCI_SETBACKSPACEUNINDENTS, !style->tab->backspaceUntabs );

	// ----------
	// Caret
	sendEditor( SCI_SETCARETFORE, theme->caretColor );
	sendEditor( SCI_SETSELBACK, true, theme->selectionBgColor );
	sendEditor( SCI_SETCARETWIDTH, 1 );

	// ----------
	// Bookmark
	COLORREF bookmarkFgColor = theme->bookmarkFgColor;
	COLORREF bookmarkBgColor = theme->bookmarkBgColor;
	switch ( theme->bookmarkStyle ) {
		case RDOBOOKMARKS_NONE     : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_EMPTY    , bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_CIRCLE   : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_CIRCLE   , bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_RECT     : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_SMALLRECT, bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_ROUNDRECT: defineMarker( sci_MARKER_BOOKMARK, SC_MARK_ROUNDRECT, bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_ARROW    : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_ARROW    , bookmarkFgColor, bookmarkBgColor ); break;
	}

	// ----------
	// Window
	sendEditor( SCI_SETWRAPMODE, style->window->wordWrap ? SC_WRAP_WORD : SC_WRAP_NONE );
	sendEditor( SCI_SETHSCROLLBAR, style->window->showHorzScrollBar );
}

void RDOBaseEdit::setGroup(PTR(Group) pGroup)
{
	m_pGroup = pGroup;
}

void RDOBaseEdit::onEditUndo() 
{
	sendEditor(SCI_UNDO);
}

void RDOBaseEdit::onEditRedo() 
{
	sendEditor(SCI_REDO);
}

void RDOBaseEdit::onEditCut() 
{
	sendEditor(SCI_CUT);
}

void RDOBaseEdit::onEditCopy() 
{
	sendEditor(SCI_COPY);
}

void RDOBaseEdit::onEditPaste() 
{
	sendEditor(SCI_PASTE);
}

void RDOBaseEdit::onEditDel() 
{
	sendEditor(SCI_CLEAR);
}

void RDOBaseEdit::onEditSelectAll() 
{
	sendEditor(SCI_SELECTALL);
}

void RDOBaseEdit::onEditUpperCase() 
{
	sendEditor(SCI_UPPERCASE);
}

void RDOBaseEdit::onEditLowerCase() 
{
	sendEditor(SCI_LOWERCASE);
}

tstring RDOBaseEdit::getCurrentWord() const
{
	int pos_begin = sendEditor( SCI_WORDSTARTPOSITION, getCurrentPos(), true );
	int pos_end   = sendEditor( SCI_WORDENDPOSITION, getCurrentPos(), true );
	int len = pos_end - pos_begin + 2;
	char* word = new char[ len ];
	TextRange tr;
	tr.lpstrText  = word;
	tr.chrg.cpMin = pos_begin;
	tr.chrg.cpMax = pos_end;
	sendEditor( SCI_GETTEXTRANGE, 0, (long)&tr );
	tstring str( tr.lpstrText );
	delete[] word;
	return str;
}

tstring RDOBaseEdit::getSelection() const
{
	CharacterRange cr = getSelectionRange();
	char* selection = new char[ cr.cpMax - cr.cpMin + 1 ];
	sendEditor( SCI_GETSELTEXT, 0, (long)selection );
	tstring str( selection );
	delete[] selection;
	return str;
}

tstring RDOBaseEdit::getCurrentOrSelectedWord() const
{
	if ( isSelected() ) {
		return getSelection();
	} else {
		return getCurrentWord();
	}
}

tstring RDOBaseEdit::getWordForFind() const
{
	if ( isSelected() ) {
		return getSelection();
	} else if ( m_pGroup && !m_pGroup->findStr.empty() ) {
		return m_pGroup->findStr;
	} else {
		return getCurrentWord();
	}
}

CharacterRange RDOBaseEdit::getSelectionRange() const
{
	CharacterRange crange;
	crange.cpMin = sendEditor( SCI_GETSELECTIONSTART );
	crange.cpMax = sendEditor( SCI_GETSELECTIONEND );
	return crange;
}

void RDOBaseEdit::gotoLineEnsureVisible( int line ) const
{
	sendEditor( SCI_ENSUREVISIBLEENFORCEPOLICY, line );
	sendEditor( SCI_GOTOLINE, line );
}

void RDOBaseEdit::ensureRangeVisible( int posStart, int posEnd, rbool enforcePolicy ) const
{
	int lineStart = getLineFromPosition( posStart < posEnd ? posStart : posEnd );
	int lineEnd   = getLineFromPosition( posStart > posEnd ? posStart : posEnd );
	for ( int line = lineStart; line <= lineEnd; line++ ) {
		sendEditor( enforcePolicy ? SCI_ENSUREVISIBLEENFORCEPOLICY : SCI_ENSUREVISIBLE, line );
	}
}

void RDOBaseEdit::OnSearchFind() 
{
	//! @todo qt
	//firstFoundPos = -1;
	//CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	//DWORD flag = m_pGroup ? ((m_pGroup->bSearchDown ? FR_DOWN : 0) | (m_pGroup->bMatchCase ? FR_MATCHCASE : 0) | (m_pGroup->bMatchWholeWord ? FR_WHOLEWORD : 0)) : 0;
	//pDlg->Create( true, getWordForFind().c_str(), NULL, flag, this );
}

void RDOBaseEdit::OnSearchReplace() 
{
	//! @todo qt
	//firstFoundPos = -1;
	//CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	//DWORD flag = m_pGroup ? ((m_pGroup->bSearchDown ? FR_DOWN : 0) | (m_pGroup->bMatchCase ? FR_MATCHCASE : 0) | (m_pGroup->bMatchWholeWord ? FR_WHOLEWORD : 0)) : 0;
	//pDlg->Create( false, getWordForFind().c_str(), m_pGroup ? m_pGroup->replaceStr.c_str() : NULL, flag, this );
}

void RDOBaseEdit::OnSearchFindNext() 
{
	firstFoundPos = -1;
	if ( m_pGroup ) {
		findNext( m_pGroup->findStr, m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );
	}
}

void RDOBaseEdit::OnSearchFindPrevious() 
{
	firstFoundPos = -1;
	if ( m_pGroup ) {
		findNext( m_pGroup->findStr, !m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );
	}
}

void RDOBaseEdit::OnSearchFindNextFast() 
{
	firstFoundPos = getCurrentPos();
	if ( m_pGroup ) {
		m_pGroup->findStr     = getWordForFind();
		m_pGroup->bSearchDown = true;
		findNext( m_pGroup->findStr, m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );
	}
}

void RDOBaseEdit::OnSearchFindPreviousFast() 
{
	firstFoundPos = getCurrentPos();
	if ( m_pGroup ) {
		m_pGroup->findStr     = getWordForFind();
		m_pGroup->bSearchDown = true;
		findNext( m_pGroup->findStr, !m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );
	}
}

LRESULT RDOBaseEdit::OnFindReplaceMsg( WPARAM /*wParam*/, LPARAM lParam )
{
	//! @todo qt
	//if ( !m_pGroup ) return 0;

	//CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	//m_pGroup->findStr = pDialog->GetFindString();

	//if ( pDialog->IsTerminating() ) {
	//	firstFoundPos = -1;
	//	SetFocus();
	//	return 0;
	//} else {
	//	rbool newSearchDown     = pDialog->SearchDown() ? true : false;
	//	rbool newMatchCase      = pDialog->MatchCase() ? true : false;
	//	rbool newMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
	//	if ( newSearchDown != m_pGroup->bSearchDown || newMatchCase != m_pGroup->bMatchCase || newMatchWholeWord != m_pGroup->bMatchWholeWord ) {
	//		firstFoundPos = -1;
	//	}
	//	m_pGroup->bSearchDown     = newSearchDown;
	//	m_pGroup->bMatchCase      = newMatchCase;
	//	m_pGroup->bMatchWholeWord = newMatchWholeWord;

	//	if ( pDialog->FindNext() ) {

	//		findNext( m_pGroup->findStr, m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );

	//	} else if ( pDialog->ReplaceCurrent() ) {

	//		m_pGroup->replaceStr = static_cast<LPCTSTR>(pDialog->GetReplaceString());
	//		replace( m_pGroup->findStr, m_pGroup->replaceStr, m_pGroup->bSearchDown, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );

	//	} else if ( pDialog->ReplaceAll() ) {

	//		m_pGroup->replaceStr = static_cast<LPCTSTR>(pDialog->GetReplaceString());
	//		replaceAll( m_pGroup->findStr, m_pGroup->replaceStr, m_pGroup->bMatchCase, m_pGroup->bMatchWholeWord );

	//	}
	//}
	return 0;
}

void RDOBaseEdit::OnUpdateSearchFindNextPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pGroup ? !m_pGroup->findStr.empty() : false );
}

void RDOBaseEdit::OnUpdateSearchFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !isEmpty() );
}

void RDOBaseEdit::OnUpdateSearchReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !isReadOnly() && !isEmpty() );
}

void RDOBaseEdit::findNext( REF(tstring) findWhat, const rbool searchDown, const rbool matchCase, const rbool matchWholeWord )
{
	int findLen = findWhat.length();
	if ( !findLen ) return;

	CharacterRange cr = getSelectionRange();
	int startPosition = cr.cpMax;
	int endPosition   = getLength();
	if ( !searchDown ) {
		startPosition = cr.cpMin - 1;
		endPosition   = 0;
	}

	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor( SCI_SETTARGETSTART, startPosition );
	sendEditor( SCI_SETTARGETEND, endPosition );
	sendEditor( SCI_SETSEARCHFLAGS, flags );
	int posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat.c_str() );
	if ( posFind == -1 ) {
		if ( !searchDown ) {
			startPosition = getLength();
			endPosition   = 0;
		} else {
			startPosition = 0;
			endPosition   = getLength();
		}
		sendEditor( SCI_SETTARGETSTART, startPosition );
		sendEditor( SCI_SETTARGETEND, endPosition );
		posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat.c_str() );
	}
	if ( posFind == -1 ) {
		firstFoundPos = -1;
		bHaveFound    = false;
		QMessageBox::warning(this, "Результаты поиска", rdo::format(ID_MSG_CANTFIND, findWhat.c_str()).c_str());
		//! @todo возможно, надо убрать
		setFocus();
	} else {
		if ( firstFoundPos == -1 ) {
			firstFoundPos = posFind;
		} else if ( posFind == firstFoundPos ) {
			firstFoundPos = -1;
			bHaveFound    = false;
			QMessageBox::warning(this, "Результаты поиска", rdo::format(ID_MSG_CANTFIND, findWhat.c_str()).c_str());
			//! @todo возможно, надо убрать
			setFocus();
			return;
		}
		bHaveFound = true;
		int start  = sendEditor( SCI_GETTARGETSTART );
		int end    = sendEditor( SCI_GETTARGETEND );
		ensureRangeVisible( start, end );
		setSelection( start, end );
	}
}

void RDOBaseEdit::replace( REF(tstring) findWhat, REF(tstring) replaceWhat, const rbool searchDown, const rbool matchCase, const rbool matchWholeWord )
{
	if ( bHaveFound ) {
		int replaceLen = replaceWhat.length();
		CharacterRange cr = getSelectionRange();
		sendEditor( SCI_SETTARGETSTART, cr.cpMin );
		sendEditor( SCI_SETTARGETEND, cr.cpMax );
		int lenReplaced = replaceLen;
		sendEditorString( SCI_REPLACETARGET, replaceLen, replaceWhat.c_str() );
		setSelection( cr.cpMin + lenReplaced, cr.cpMin );
		bHaveFound = false;
	}
	findNext( findWhat, searchDown, matchCase, matchWholeWord );
}

void RDOBaseEdit::replaceAll( REF(tstring) findWhat, REF(tstring) replaceWhat, const rbool matchCase, const rbool matchWholeWord )
{
	int findLen = findWhat.length();
	if ( !findLen ) return;

	int startPosition = 0;
	int endPosition   = getLength();

	int replaceLen = replaceWhat.length();
	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor( SCI_SETTARGETSTART, startPosition );
	sendEditor( SCI_SETTARGETEND, endPosition );
	sendEditor( SCI_SETSEARCHFLAGS, flags );
	int posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat.c_str() );

	if ( (posFind != -1) && (posFind <= endPosition) ) {
		int lastMatch = posFind;
		sendEditor( SCI_BEGINUNDOACTION );
		while ( posFind != -1 ) {
			int lenTarget = sendEditor( SCI_GETTARGETEND ) - sendEditor( SCI_GETTARGETSTART );
			int lenReplaced = replaceLen;
			sendEditorString( SCI_REPLACETARGET, replaceLen, replaceWhat.c_str() );
			endPosition += lenReplaced - lenTarget;
			lastMatch    = posFind + lenReplaced;
			if ( lenTarget <= 0 ) lastMatch++;
			sendEditor( SCI_SETTARGETSTART, lastMatch );
			sendEditor( SCI_SETTARGETEND, endPosition );
			posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat.c_str() );
		}
		setSelection( lastMatch, lastMatch );
		sendEditor( SCI_ENDUNDOACTION );
	} else {
		QMessageBox::warning(this, "Результаты поиска", rdo::format(ID_MSG_CANTFIND, findWhat.c_str()).c_str());
		//! @todo возможно, надо убрать
		setFocus();
	}
}

void RDOBaseEdit::clearAll()
{
	rbool readOnly = isReadOnly();
	setReadOnly( false );
	sendEditor( SCI_CLEARALL );
	setReadOnly( readOnly );
}

rbool RDOBaseEdit::bookmarkToggle( int line ) const
{
	if ( line == -1 ) line = getCurrentLineNumber();
	int state = sendEditor( SCI_MARKERGET, line );
	if ( state & (1 << sci_MARKER_BOOKMARK) ) {
		sendEditor( SCI_MARKERDELETE, line, sci_MARKER_BOOKMARK );
		return false;
	} else {
		sendEditor( SCI_MARKERADD, line, sci_MARKER_BOOKMARK );
		return true;
	}
}

rbool RDOBaseEdit::bookmarkNext( const rbool canLoop, const rbool fromCurrentLine, rbool* wasLoop ) const
{
	rbool wasFound = false;
	rbool was_loop = false;

	int line = -1;
	if ( fromCurrentLine ) line = getCurrentLineNumber();

	int nextLine = sendEditor( SCI_MARKERNEXT, line + 1, 1 << sci_MARKER_BOOKMARK );
	if ( nextLine < 0 ) {
		was_loop = true;
		if ( canLoop ) nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_BOOKMARK );
	}
	if ( (( canLoop && was_loop ) || !was_loop ) && nextLine >= 0 && nextLine != line ) {
		gotoLineEnsureVisible( nextLine );
		wasFound = true;
	}
	if ( wasLoop ) *wasLoop = was_loop;
	return wasFound;
}

rbool RDOBaseEdit::bookmarkPrev( const rbool canLoop, const rbool fromCurrentLine, rbool* wasLoop ) const
{
	rbool wasFound = false;
	rbool was_loop = false;

	int lineCount = getLineCount();
	int line = lineCount + 1;
	if ( fromCurrentLine ) line = getCurrentLineNumber();

	int prevLine  = sendEditor( SCI_MARKERPREVIOUS, line - 1, 1 << sci_MARKER_BOOKMARK );
	if ( prevLine < 0 ) {
		was_loop = true;
		if ( canLoop ) prevLine = sendEditor( SCI_MARKERPREVIOUS, lineCount, 1 << sci_MARKER_BOOKMARK );
	}
	if ( (( canLoop && was_loop ) || !was_loop ) && prevLine >= 0 && prevLine != line ) {
		gotoLineEnsureVisible( prevLine );
		wasFound = true;
	}
	if ( wasLoop ) *wasLoop = was_loop;
	return wasFound;
}

void RDOBaseEdit::bookmarkClearAll() const
{
	sendEditor( SCI_MARKERDELETEALL, sci_MARKER_BOOKMARK );
}

rbool RDOBaseEdit::hasBookmarks() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_BOOKMARK );
	return nextLine >= 0;
}

void RDOBaseEdit::onCopyAsRTF(QMimeData* pMimeData)
{
	if (!isSelected())
		return;

	CharacterRange cr = getSelectionRange();
	tstring result = saveAsRTF(cr.cpMin, cr.cpMax);
	if (result.empty())
		return;

	QByteArray ba;
	ba.append(QString::fromStdString(result));
	//! @todo для линуха надо будет использовать "text/rtf" ?
	pMimeData->setData("Rich Text Format", ba);
}

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEdit: some functions for RTF export ---------- BEGIN
// --------------------------------------------------------------------------------
int GetHexChar( char ch ) // 'H'
{
	return ch > '9' ? (ch | 0x20) - 'a' + 10 : ch - '0';
}

int GetHexByte( const char *hexbyte ) // "HH"
{
	return (GetHexChar(*hexbyte) << 4) | GetHexChar(hexbyte[1]);
}

void GetRTFStyleChange( char *delta, char *last, const char *current ) // \f0\fs20\cf0\highlight0\b0\i0
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
// -------------------- RDOBaseEdit: some functions for RTF export ---------- END
// --------------------------------------------------------------------------------

tstring RDOBaseEdit::saveAsRTF(int start, int end) const
{
	tstring saveStr;

	if (!style)
	{
		return saveStr;
	}

	int lengthDoc = getLength();
	if ( end < 0 ) end = lengthDoc;
	sendEditor( SCI_COLOURISE, 0, -1 );
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
	strncpy( *fonts, style->font->name.c_str(), MAX_FONTDEF );
#pragma warning(default: 4996)
	saveStr += rdo::format( RTF_FONTDEF, 0, style->font->characterSet, style->font->name.c_str() );
#pragma warning(disable: 4996)
	strncpy( *colors, "#000000", MAX_COLORDEF );
#pragma warning(default: 4996)

	RDOBaseEditTheme* theme = static_cast<RDOBaseEditTheme*>(style->theme);

	for ( int istyle = 0; istyle <= STYLE_DEFAULT; istyle++ ) {
		if ( theme->styleUsing( istyle ) ) {
#pragma warning(disable: 4996)
			sprintf( lastStyle, RTF_SETFONTFACE "%d", fontCount-1 );
			sprintf( lastStyle + strlen(lastStyle), RTF_SETFONTSIZE "%d", style->font->size * 2 );
#pragma warning(default: 4996)
			if ( theme->styleDefault( istyle ) ) {
#pragma warning(disable: 4996)
				strncpy( colors[colorCount++], theme->styleBGColorToHEX( istyle ).c_str(), MAX_COLORDEF );
#pragma warning(default: 4996)
			}
#pragma warning(disable: 4996)
			strncpy( colors[colorCount++], theme->styleFGColorToHEX( istyle ).c_str(), MAX_COLORDEF );
#pragma warning(default: 4996)
			rbool bold   = theme->styleBold( istyle );
			rbool italic = theme->styleItalic( istyle );
#pragma warning(disable: 4996)
			sprintf( lastStyle + strlen(lastStyle), RTF_SETCOLOR "%d", colorCount-1 );
			sprintf( lastStyle + strlen(lastStyle), RTF_SETBACKGROUND "%d", 1 );
			strcat( lastStyle, bold ? RTF_BOLD_ON : RTF_BOLD_OFF );
			strcat( lastStyle, italic ? RTF_ITALIC_ON : RTF_ITALIC_OFF );
			strncpy( styles[istyle], lastStyle, MAX_STYLEDEF );
#pragma warning(default: 4996)
		}
	}
	saveStr += RTF_FONTDEFCLOSE;
	saveStr += RTF_COLORDEFOPEN;

	for ( i = 0; i < colorCount; i++ ) {
		saveStr += rdo::format( RTF_COLORDEF, GetHexByte(colors[i] + 1), GetHexByte(colors[i] + 3), GetHexByte(colors[i] + 5) );
	}

	saveStr += rdo::format( RTF_COLORDEFCLOSE RTF_HEADERCLOSE RTF_BODYOPEN RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0 ", style->font->size * 2 );

#pragma warning(disable: 4996)
	sprintf( lastStyle, RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0" RTF_SETBACKGROUND "0" RTF_BOLD_OFF RTF_ITALIC_OFF, style->font->size * 2 );
#pragma warning(default: 4996)

	tstring::size_type prevLength = saveStr.length();
	rbool prevCR = false;
	int styleCurrent = -1;
	for ( i = start; i < end; i++ ) {
		int style = sendEditor( SCI_GETSTYLEAT, i );
		if (!theme->styleUsing(style))
			continue;
		char ch   = (char)sendEditor( SCI_GETCHARAT, i );
		if ( style != styleCurrent ) {
			GetRTFStyleChange( deltaStyle, lastStyle, styles[style] );
			if ( *deltaStyle ) saveStr += deltaStyle;
			styleCurrent = style;
		}
		if (ch == '\\') saveStr += "\\\\";
		else if (ch == '\t') saveStr += RTF_TAB;
		else if (ch == '\n') {
			if ( !prevCR ) saveStr += RTF_EOLN;
		} else if (ch == '\r') saveStr += RTF_EOLN;
		else saveStr += ch;

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

void RDOBaseEdit::setCurrentPos( const int value ) const
{
	sendEditor( SCI_SETCURRENTPOS, value );
	sendEditor( SCI_SETSELECTIONSTART, value );
	sendEditor( SCI_SETSELECTIONEND, value );
}

void RDOBaseEdit::setCurrentPos( const int line, int pos_in_line, const rbool convert_rdo_tab ) const
{
	int pos = getPositionFromLine( line );
	int line_length = sendEditor( SCI_LINELENGTH, line );
	char currentLine[8000];
	rbool canUseLine = false;
	if ( line_length < 8000 ) {
		sendEditor( SCI_GETLINE, line, (long)currentLine );
		while ( currentLine[line_length-1] == 0x0A || currentLine[line_length-1] == 0x0D ) {
			line_length--;
		}
		canUseLine = true;
	} else {
		line_length -= 2;
	}

	int new_line_length = line_length;

	if ( canUseLine && convert_rdo_tab ) {
		int tab_size = style ? style->tab->tabSize : 8;
		int spaces = 0;
		for ( int i = 0; i < line_length; i++ ) {
			if ( currentLine[i] == ' ' ) {
				spaces++;
			} else if ( currentLine[i] == '\t' ) {
				int delta = spaces % 8;
				pos_in_line     -= 7 - delta;
				new_line_length += tab_size + delta;
				spaces = 0;
			} else {
				spaces = 0;
			}
		}
	}

	if ( new_line_length < pos_in_line ) {
		pos += new_line_length;
	} else {
		pos += pos_in_line;
	}

	int line_to_scroll = line > 0 ? line - 1 : 0;
	scrollToLine( line_to_scroll );
	sendEditor( SCI_GOTOPOS, pos );

	setCurrentPos( pos );
}

rbool RDOBaseEdit::isLineVisible( const int line ) const
{
	int first_line = sendEditor( SCI_GETFIRSTVISIBLELINE );
	int last_line = first_line + sendEditor( SCI_LINESONSCREEN );
	return line >= first_line && line <= last_line;
}

void RDOBaseEdit::appendText( CREF(tstring) str ) const
{
	sendEditorString( SCI_ADDTEXT, str.length(), str.c_str() );
}

void RDOBaseEdit::scrollToLine( const int line ) const
{
	sendEditor( SCI_LINESCROLL, 0, line - sendEditor( SCI_GETFIRSTVISIBLELINE ) - sendEditor( SCI_LINESONSCREEN )/3 );
	setCurrentPos( getPositionFromLine( line ) );
}

void RDOBaseEdit::scrollToLine2( const int line ) const
{
	sendEditor( SCI_LINESCROLL, 0, line );
}

void RDOBaseEdit::scrollToCarret() const
{
	sendEditor( SCI_SCROLLCARET );
}

void RDOBaseEdit::horzScrollToCurrentPos() const
{
	setCurrentPos( getCurrentPos() );
	sendEditor( SCI_REPLACESEL, 0, 0 );
}

void RDOBaseEdit::load( rdo::stream& stream )
{
	rbool readOnly = isReadOnly();
	setReadOnly( false );

	sendEditorString( SCI_ADDTEXT, stream.str().length(), &stream.str()[0] );

	setReadOnly( readOnly );
}

void RDOBaseEdit::save( rdo::stream& stream ) const
{
	int len = getLength();
	std::vector<char> str;
	str.resize( len + 1 );
	sendEditorString( SCI_GETTEXT, len + 1, &str[0] );
//	str[len] = "\0";
//	str.resize( len );
	stream.str( &str[0] ); // qq
}

int RDOBaseEdit::indentOfBlock( int line ) const
{
	if ( line < 0 ) return 0;
	return sendEditor( SCI_GETLINEINDENTATION, line );
}

void RDOBaseEdit::setLineIndentation( int line, int indent ) const
{
	if ( indent < 0 ) return;
	CharacterRange crange = getSelectionRange();
	int posBefore = sendEditor( SCI_GETLINEINDENTPOSITION, line );
	sendEditor( SCI_SETLINEINDENTATION, line, indent );
	int posAfter = sendEditor( SCI_GETLINEINDENTPOSITION, line );
	int posDifference = posAfter - posBefore;
	if ( posAfter > posBefore ) {
		if ( crange.cpMin >= posBefore ) {
			crange.cpMin += posDifference;
		}
		if ( crange.cpMax >= posBefore ) {
			crange.cpMax += posDifference;
		}
	} else if ( posAfter < posBefore ) {
		if ( crange.cpMin >= posAfter ) {
			if ( crange.cpMin >= posBefore ) {
				crange.cpMin += posDifference;
			} else {
				crange.cpMin = posAfter;
			}
		}
		if ( crange.cpMax >= posAfter ) {
			if ( crange.cpMax >= posBefore ) {
				crange.cpMax += posDifference;
			} else {
				crange.cpMax = posAfter;
			}
		}
	}
	setSelection( crange.cpMin, crange.cpMax );
}

void RDOBaseEdit::autoIndent() const
{
	CharacterRange crange = getSelectionRange();
	int selStart      = crange.cpMin;
	int curLine       = getCurrentLineNumber();
	int thisLineStart = getPositionFromLine( curLine );
	int indentBlock   = indentOfBlock( curLine - 1 );

	if ( selStart == thisLineStart ) {
		setLineIndentation( curLine, indentBlock );
	}
}

void RDOBaseEdit::onBookmarkToggle()
{
	bookmarkToggle();
	updateBookmarksGUI();
}

void RDOBaseEdit::onBookmarkNext()
{
	if ( bookmarkNext( false, true ) ) return;

	if ( !m_pGroup ) {

		bookmarkNext();

	} else {

		Group::List::const_iterator it = m_pGroup->begin();
		while ( it != m_pGroup->end() ) {
			if ( *it == this ) break;
			it++;
		}
		if ( !(*it) ) return;

		rbool allItem = false;
		rbool wasLoop = true;
		while ( !allItem && *it && wasLoop ) {
			it++;
			if ( it == m_pGroup->end() ) {
				it = m_pGroup->begin();
			}
			if ( *it == this ) {
				allItem = true;
			} else {
				(*it)->bookmarkNext( false, false, &wasLoop );
			}
		}
		if ( !allItem ) {
			(*it)->setFocus();
		} else {
			(*it)->bookmarkNext();
		}
	}
}

void RDOBaseEdit::onBookmarkPrev()
{
	if ( bookmarkPrev( false, true ) ) return;

	if ( !m_pGroup ) {

		bookmarkPrev();

	} else {

		Group::List::const_iterator it = m_pGroup->begin();
		while ( it != m_pGroup->end() ) {
			if ( *it == this ) break;
			it++;
		}
		if ( !(*it) ) return;

		rbool allItem = false;
		rbool wasLoop = true;
		while ( !allItem && *it && wasLoop ) {
			if ( it == m_pGroup->begin() ) {
				it = m_pGroup->end();
			}
			it--;
			if ( *it == this ) {
				allItem = true;
			} else {
				(*it)->bookmarkPrev( false, false, &wasLoop );
			}
		}
		if ( !allItem ) {
			(*it)->setFocus();
		} else {
			(*it)->bookmarkPrev();
		}
	}
}

void RDOBaseEdit::onBookmarkClearAll()
{
	methodOfGroup(boost::bind(&RDOBaseEdit::bookmarkClearAll, _1));
	updateBookmarksGUI();
}

void RDOBaseEdit::updateBookmarksGUI()
{
	GUI_HAS_BOOKMARK = predicateOfGroup(boost::bind(&RDOBaseEdit::hasBookmarks, _1));
}

void RDOBaseEdit::onUpdateEditGUI()
{
	onUpdateActions(isActivated());
}

void RDOBaseEdit::updateAllGUI()
{
	onUpdateEditGUI();
	updateBookmarksGUI();
}

void RDOBaseEdit::onViewShowWhiteSpace() 
{
	methodOfGroup(boost::bind(&RDOBaseEdit::setViewWhiteSpace, _1, !isViewWhiteSpace()));
}

void RDOBaseEdit::onViewShowEndOfLine() 
{
	methodOfGroup(boost::bind(&RDOBaseEdit::setViewEndOfLine, _1, !isViewEndOfLine()));
}

void RDOBaseEdit::onViewZoomInc() 
{
	methodOfGroup(boost::bind(&RDOBaseEdit::zoomIn, _1));
}

void RDOBaseEdit::onViewZoomDec() 
{
	methodOfGroup(boost::bind(&RDOBaseEdit::zoomOut, _1));
}

void RDOBaseEdit::onViewZoomReset() 
{
	methodOfGroup(boost::bind(&RDOBaseEdit::resetZoom, _1));
}

int RDOBaseEdit::findPos( REF(tstring) findWhat, const int startFromLine, const rbool matchCase, const rbool matchWholeWord ) const
{
	int findLen = findWhat.length();
	if ( !findLen ) return -1;

	int startPosition = getPositionFromLine( startFromLine );
	int endPosition   = getLength();

	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor( SCI_SETTARGETSTART, startPosition );
	sendEditor( SCI_SETTARGETEND, endPosition );
	sendEditor( SCI_SETSEARCHFLAGS, flags );
	return sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat.c_str() );
}

tstring RDOBaseEdit::getLine( const int line ) const
{
	int length = sendEditor( SCI_LINELENGTH, line );
	tstring str;
	str.resize( length );
	sendEditor( SCI_GETLINE, line, (long)str.data() );
	return str;
}

// --------------------------------------------------------------------------------
// -------------------- RDOGotoDlg
// --------------------------------------------------------------------------------
class RDOGotoDlg: public CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX )
	{
		CDialog::DoDataExchange( pDX );
		DDX_Text(pDX, IDC_GOTO_EDIT, line);
	}

public:
	RDOGotoDlg( CWnd* pParentWnd, int _line ):
		CDialog( IDD_GOTO_LINE_DIALOG, pParentWnd ),
		line( _line )
	{
	}
	int line;
};

void RDOBaseEdit::OnSearchGotoLine()
{
	//! @todo qt
	//RDOGotoDlg dialog( this, getCurrentLineNumber() + 1 );
	//if ( dialog.DoModal() == IDOK ) {
	//	if ( dialog.line - 1 > getLineCount() ) {
	//		dialog.line = getLineCount() + 1;
	//	}
	//	setCurrentPos( dialog.line - 1, 0 );
	//}
}

void RDOBaseEdit::focusInEvent(QFocusEvent* pEvent)
{
	super::focusInEvent(pEvent);
	activate(pEvent);
}

void RDOBaseEdit::focusOutEvent(QFocusEvent* pEvent)
{
	super::focusOutEvent(pEvent);
	deactivate(pEvent);
}

void RDOBaseEdit::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actEditUndo,
		activated && sendEditor(SCI_CANUNDO),
		this, "onEditUndo()"
	);
	updateAction(
		pMainWindow->actEditRedo,
		activated && sendEditor(SCI_CANREDO),
		this, "onEditRedo()"
	);
	updateAction(
		pMainWindow->actEditCut,
		activated && !isReadOnly() && isSelected(),
		this, "onEditCut()"
	);
	updateAction(
		pMainWindow->actEditCopy,
		activated && isSelected(),
		this, "onEditCopy()"
	);
	updateAction(
		pMainWindow->actEditPaste,
		activated && sendEditor(SCI_CANPASTE),
		this, "onEditPaste()"
	);
	updateAction(
		pMainWindow->actEditDel,
		activated && !isReadOnly() && (getCurrentPos() != getLength() || isSelected()),
		this, "onEditDel()"
	);
	updateAction(
		pMainWindow->actEditSelectAll,
		activated && !isEmpty(),
		this, "onEditSelectAll()"
	);
	updateAction(
		pMainWindow->actEditUpperCase,
		activated && !isReadOnly() && isSelected(),
		this, "onEditUpperCase()"
	);
	updateAction(
		pMainWindow->actEditLowerCase,
		activated && !isReadOnly() && isSelected(),
		this, "onEditLowerCase()"
	);
	updateAction(
		pMainWindow->actViewShowWhiteSpace,
		activated,
		this, "onViewShowWhiteSpace()"
	);
	pMainWindow->actViewShowWhiteSpace->setChecked(isViewWhiteSpace());

	updateAction(
		pMainWindow->actViewShowEndOfLine,
		activated,
		this, "onViewShowEndOfLine()"
	);
	pMainWindow->actViewShowEndOfLine->setChecked(isViewEndOfLine());

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, "onHelpContext()"
	);
	updateAction(
		pMainWindow->actViewZoomInc,
		activated &&  getZoom() < 20,
		this, "onViewZoomInc()"
	);
	updateAction(
		pMainWindow->actViewZoomDec,
		activated && getZoom() > -10,
		this, "onViewZoomDec()"
	);
	updateAction(
		pMainWindow->actViewZoomReset,
		activated && getZoom(),
		this, "onViewZoomReset()"
	);
	updateAction(
		pMainWindow->actSearchBookmarkNext,
		activated && GUI_HAS_BOOKMARK,
		this, "onBookmarkNext()"
	);
	updateAction(
		pMainWindow->actSearchBookmarkPrev,
		activated && GUI_HAS_BOOKMARK,
		this, "onBookmarkPrev()"
	);
	updateAction(
		pMainWindow->actSearchBookmarksToggle,
		activated,
		this, "onBookmarkToggle()"
	);
	updateAction(
		pMainWindow->actSearchBookmarksClearAll,
		activated && GUI_HAS_BOOKMARK,
		this, "onBookmarkClearAll()"
	);

	QString modify = activated
		? isReadOnly()
			? QString("Только чтение")
			: isModify()
				? QString("Изменён")
				: QString()
		: QString();

	pMainWindow->statusBar()->update<StatusBar::SB_MODIFY>(modify);

	QString coord = activated
		? QString("%1: %2").arg(getCurrentColumnNumber() + 1).arg(getCurrentLineNumber() + 1)
		: QString();

	pMainWindow->statusBar()->update<StatusBar::SB_COORD>(coord);

	QString overwrite = activated && sendEditor(SCI_GETOVERTYPE)
		? QString("Замена")
		: QString();

	pMainWindow->statusBar()->update<StatusBar::SB_OVERWRITE>(overwrite);
}

void RDOBaseEdit::onUpdateModify()
{
	emit modifyChanged(isModify());
}

rbool RDOBaseEdit::isViewWhiteSpace() const
{
	return sendEditor(SCI_GETVIEWWS) != SCWS_INVISIBLE;
}

void RDOBaseEdit::setViewWhiteSpace(rbool value)
{
	sendEditor(SCI_SETVIEWWS, value ? SCWS_VISIBLEALWAYS : SCWS_INVISIBLE);
}

rbool RDOBaseEdit::isViewEndOfLine() const
{
	return sendEditor(SCI_GETVIEWEOL) ? true : false;
}

void RDOBaseEdit::setViewEndOfLine(rbool value)
{
	sendEditor(SCI_SETVIEWEOL, value);
}

void RDOBaseEdit::methodOfGroup(CREF(this_method) fun)
{
	m_pGroup ? m_pGroup->for_each(fun) : fun(this);
}

rbool RDOBaseEdit::predicateOfGroup(CREF(this_predicate) fun) const
{
	return m_pGroup
		? m_pGroup->find_if(fun) != m_pGroup->end()
		: fun(this);
}
