#include "stdafx.h"
#include "rdoeditorsciedit.h"
#include "sci/SciLexer.h"
#include "sci/PropSet.h"
#include "sci/Scintilla.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEdit
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

BEGIN_MESSAGE_MAP( RDOEditorSciEdit, CWnd )
	//{{AFX_MSG_MAP(RDOEditorSciEdit)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_COPYASRTF, OnEditCopyAsRTF)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UPPERCASE, OnEditUpperCase)
	ON_COMMAND(ID_EDIT_LOWERCASE, OnEditLowerCase)
	ON_UPDATE_COMMAND_UI( ID_EDIT_SELECT_ALL      , OnSelectAll )
	ON_COMMAND(ID_SEARCH_FIND, OnSearchFind)
	ON_COMMAND(ID_SEARCH_REPLACE, OnSearchReplace)
	ON_COMMAND(ID_SEARCH_FINDNEXT, OnSearchFindNext)
	ON_COMMAND(ID_SEARCH_FINDPREVIOUS, OnSearchFindPrevious)
	ON_COMMAND(ID_SEARCH_FINDNEXT_FAST, OnSearchFindNextFast)
	ON_COMMAND(ID_SEARCH_FINDPREVIOUS_FAST, OnSearchFindPreviousFast)
	ON_UPDATE_COMMAND_UI(ID_SEARCH_FINDNEXT, OnUpdateSearchFindNextPrev)
	ON_UPDATE_COMMAND_UI(ID_SEARCH_FIND, OnUpdateSearchFind)
	ON_UPDATE_COMMAND_UI(ID_SEARCH_REPLACE, OnUpdateSearchReplace)
	ON_COMMAND(ID_VIEW_EDITOR_TOGGLECURRENTFOLD, OnViewToggleCurrentFold)
	ON_COMMAND(ID_VIEW_EDITOR_TOGGLEALLFOLDS, OnViewToggleAllFolds)
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_TOGGLECURRENTFOLD, OnUpdateFold )
	ON_COMMAND( ID_SEARCH_BOOKMARKTOGGLE  , OnBookmarkToggle )
	ON_COMMAND( ID_SEARCH_BOOKMARKNEXT    , OnBookmarkNext )
	ON_COMMAND( ID_SEARCH_BOOKMARKPREVIOUS, OnBookmarkPrev )
	ON_COMMAND( ID_SEARCH_BOOKMARKSCLEAR  , OnBookmarkClearAll )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKNEXT    , OnHasBookmarks )
	ON_COMMAND(ID_VIEW_EDITOR_WHITESPACE, OnViewWhiteSpace)
	ON_COMMAND(ID_VIEW_EDITOR_ENDOFLINE, OnViewEndOfLine)
	ON_COMMAND(ID_VIEW_EDITOR_LINENUMBERMARGIN, OnViewLineNumberMargin)
	ON_COMMAND(ID_VIEW_EDITOR_MARGIN, OnViewMargin)
	ON_COMMAND(ID_VIEW_EDITOR_FOLDMARGIN, OnViewFoldMargin)
	ON_COMMAND(ID_VIEW_EDITOR_ZOOMIN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_EDITOR_ZOOMOUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_EDITOR_ZOOMRESET, OnViewZoomReset)
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_WHITESPACE      , OnUpdateWhiteSpace )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_ENDOFLINE       , OnUpdateEndOfLine )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_LINENUMBERMARGIN, OnUpdateViewLineNumberMargin )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_MARGIN          , OnUpdateViewMargin )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_FOLDMARGIN      , OnUpdateViewFoldMargin )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_ZOOMIN          , OnUpdateZoomIn )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_ZOOMOUT         , OnUpdateZoomOut )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_ZOOMRESET       , OnUpdateZoomReset )
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPY            , OnIsSelected )
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPYASRTF       , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_UPPERCASE       , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_LOWERCASE       , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_VIEW_EDITOR_TOGGLEALLFOLDS   , OnUpdateFold )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKPREVIOUS, OnHasBookmarks )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_BOOKMARKSCLEAR  , OnHasBookmarks )
	ON_UPDATE_COMMAND_UI(ID_SEARCH_FINDPREVIOUS, OnUpdateSearchFindNextPrev)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE( FIND_REPLASE_MSG, OnFindReplaceMsg )

END_MESSAGE_MAP()

int RDOEditorSciEdit::objectCount = 0;

RDOEditorSciEdit::RDOEditorSciEdit():
	CWnd(),
	sciHWND( 0 ),
	sciEditor( 0 ),
	sciFun( NULL ),
	markerCount( 0 ),
	popupMenu( NULL ),
	style( NULL ),
	group( NULL ),
	firstFoundPos( -1 ),
	bHaveFound( false ),
	bSearchDown( true ),
	bMatchCase( false ),
	bMatchWholeWord( false ),
	findStr( "" )
{
	if ( !objectCount ) {
		Scintilla_RegisterClasses( 0 );
		Scintilla_LinkLexers();
	}
	objectCount++;

	sci_MARKER_BOOKMARK = getNewMarker();
	sci_FOLDMARGIN_ID   = getNewMarker();
}

RDOEditorSciEdit::~RDOEditorSciEdit()
{
	objectCount--;
	if ( !objectCount ) {
		Scintilla_ReleaseResources();
	}
}

BOOL RDOEditorSciEdit::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass( 0 );
	return TRUE;
}

BOOL RDOEditorSciEdit::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	SCNotification* scn = (SCNotification*)lParam;

	if ( scn->nmhdr.hwndFrom == sciHWND ) {
		switch( scn->nmhdr.code ) {
			case SCN_MODIFIED: {
				if ( scn->modificationType & SC_MOD_CHANGEFOLD ) {
					foldChanged( scn->line, scn->foldLevelNow, scn->foldLevelPrev );
					return TRUE;
				}
				break;
			}
			case SCN_NEEDSHOWN: {
				ensureRangeVisible( scn->position, scn->position + scn->length, false );
				return TRUE;
			}
			case SCN_MARGINCLICK: {
				if ( scn->margin == sci_FOLDMARGIN_ID ) {
					foldMarginClick( scn->position, scn->modifiers );
					return TRUE;
				}
				break;
			}
			case SCN_CHARADDED: {
				if ( style->tab->autoIndent && ( scn->ch == '\r' || scn->ch == '\n' ) ) autoIndent();
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL RDOEditorSciEdit::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if ( HIWORD( wParam ) == SCEN_SETFOCUS ) {
		CWnd* parent = GetParent();
		if ( parent ) {
			parent->SendMessage( WM_COMMAND, MAKELONG(0, WM_SETFOCUS), (LPARAM)m_hWnd );
		}
	}
	return CWnd::OnCommand(wParam, lParam);
}

int RDOEditorSciEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
	sciHWND = CreateWindowEx( 0, "Scintilla", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 10, 10, 500, 400, m_hWnd, 0, 0, NULL );
	sciFun = (sciFunType)::SendMessage( sciHWND, SCI_GETDIRECTFUNCTION, 0, 0 );
	sciEditor = (long)::SendMessage( sciHWND, SCI_GETDIRECTPOINTER, 0, 0 );

	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold"), "1" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold.compact"), "1" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold.flags"), "16" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold.symbols"), "1" );

	sendEditor( SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD | SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT );
	sendEditor( SCI_SETMARGINTYPEN     , sci_FOLDMARGIN_ID, SC_MARGIN_SYMBOL );
	sendEditor( SCI_SETFOLDFLAGS, 16 );
	sendEditor( SCI_SETMARGINMASKN     , sci_FOLDMARGIN_ID, SC_MASK_FOLDERS );
	sendEditor( SCI_SETMARGINSENSITIVEN, sci_FOLDMARGIN_ID, 1 );

	setViewLineNumberMargin( false );
	setViewMargin( false );
	setViewFoldMargin( false );

	sendEditor( SCI_USEPOPUP, 0 );

	return 0;
}

void RDOEditorSciEdit::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd );
	if ( sciHWND ) {
		CWnd* wnd = CWnd::FromHandle( sciHWND );
		if ( wnd ) {
			wnd->SetFocus();
		}
	}
}

void RDOEditorSciEdit::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	CRect r;
	GetClientRect( r );
	::MoveWindow( sciHWND, r.left, r.top, r.right - r.left, r.bottom - r.top, false );
}

void RDOEditorSciEdit::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
{
	CWnd::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOEditorSciEdit::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	CWnd::OnContextMenu( pWnd, pos );
	if ( popupMenu ) popupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
}

int RDOEditorSciEdit::getNewMarker()
{
	markerCount++;
	return markerCount;
}

void RDOEditorSciEdit::defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const
{
  sendEditor( SCI_MARKERDEFINE, marker, markerType );
  sendEditor( SCI_MARKERSETFORE, marker, fore );
  sendEditor( SCI_MARKERSETBACK, marker, back );
}

void RDOEditorSciEdit::setEditorStyle( RDOEditorSciEditStyle* _style )
{
	style = _style;

	// ----------
	// Colors
	sendEditor( SCI_STYLESETFORE, STYLE_DEFAULT, style->theme->defaultColor );
	sendEditor( SCI_STYLESETBACK, STYLE_DEFAULT, style->theme->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , STYLE_DEFAULT, style->theme->defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , STYLE_DEFAULT, style->theme->defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, STYLE_DEFAULT, style->theme->defaultStyle & RDOFS_UNDERLINE );

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
	sendEditor( SCI_SETCARETFORE, style->theme->caretColor );
	sendEditor( SCI_SETSELBACK, true, style->theme->selectionBgColor );
	sendEditor( SCI_SETCARETWIDTH, 1 );

	// ----------
	// Bookmark
	COLORREF bookmarkFgColor = style->theme->bookmarkFgColor;
	COLORREF bookmarkBgColor = style->theme->bookmarkBgColor;
	switch ( style->theme->bookmarkStyle ) {
		case RDOBOOKMARKS_NONE     : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_EMPTY    , bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_CIRCLE   : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_CIRCLE   , bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_RECT     : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_SMALLRECT, bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_ROUNDRECT: defineMarker( sci_MARKER_BOOKMARK, SC_MARK_ROUNDRECT, bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_ARROW    : defineMarker( sci_MARKER_BOOKMARK, SC_MARK_ARROW    , bookmarkFgColor, bookmarkBgColor ); break;
	}

	// ----------
	// Fold
	COLORREF foldFgColor = style->theme->foldFgColor;
	COLORREF foldBgColor = style->theme->foldBgColor;
	switch ( style->theme->foldStyle ) {
		case RDOFOLDS_NONE:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foldFgColor, foldBgColor );
			break;
		case RDOFOLDS_PLUS:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_MINUS, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_PLUS , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foldFgColor, foldBgColor );
			break;
		case RDOFOLDS_PLUSCONNECTED:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_MINUS  , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_PLUS   , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE  , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY  , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY  , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER, foldFgColor, foldBgColor );
			break;
		case RDOFOLDS_ARROW:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_ARROWDOWN, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_ARROW    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			break;
		case RDOFOLDS_ARROWCONNECTED:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_ARROWDOWN, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_ARROW    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER  , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER  , foldFgColor, foldBgColor );
			break;
		case RDOFOLDS_BOXCONNECTED:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_BOXMINUS, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_BOXPLUS , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE   , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY   , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY   , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER , foldFgColor, foldBgColor );
			break;
		case RDOFOLDS_CIRCLECONNECTED:
			defineMarker( SC_MARKNUM_FOLDEROPEN   , SC_MARK_CIRCLEMINUS, foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDER       , SC_MARK_CIRCLEPLUS , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE      , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER    , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY      , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY      , foldFgColor, foldBgColor );
			defineMarker( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER    , foldFgColor, foldBgColor );
			break;
	}

	// ----------
	// Window
	sendEditor( SCI_SETWRAPMODE, style->window->wordWrap ? SC_WRAP_WORD : SC_WRAP_NONE );
	sendEditor( SCI_SETHSCROLLBAR, style->window->showHorzScrollBar );
}

void RDOEditorSciEdit::setGroup( RDOEditorSciEditList* _group )
{
	group = _group;
}

void RDOEditorSciEdit::OnEditUndo() 
{
	sendEditor( SCI_UNDO );
}

void RDOEditorSciEdit::OnEditRedo() 
{
	sendEditor( SCI_REDO );
}

void RDOEditorSciEdit::OnEditCut() 
{
	sendEditor( SCI_CUT );
}

void RDOEditorSciEdit::OnEditCopy() 
{
	sendEditor( SCI_COPY );
}

void RDOEditorSciEdit::OnEditPaste() 
{
	sendEditor( SCI_PASTE );
}

void RDOEditorSciEdit::OnEditClear() 
{
	sendEditor( SCI_CLEAR );
}

void RDOEditorSciEdit::OnEditCopyAsRTF() 
{
	copyAsRTF();
}

void RDOEditorSciEdit::OnEditSelectAll() 
{
	sendEditor( SCI_SELECTALL );
}

void RDOEditorSciEdit::OnEditUpperCase() 
{
	sendEditor( SCI_UPPERCASE );
}

void RDOEditorSciEdit::OnEditLowerCase() 
{
	sendEditor( SCI_LOWERCASE );
}

void RDOEditorSciEdit::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( sendEditor(SCI_CANUNDO) );
}

void RDOEditorSciEdit::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( sendEditor(SCI_CANREDO) );
}

void RDOEditorSciEdit::OnIsSelected( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( isSelected() );
}

void RDOEditorSciEdit::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !isReadOnly() && isSelected() );
}

void RDOEditorSciEdit::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( sendEditor(SCI_CANPASTE) );
}

void RDOEditorSciEdit::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( getCurrentPos() != getLength() || isSelected() );
}

void RDOEditorSciEdit::OnSelectAll( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( !isEmpty() );
}

string RDOEditorSciEdit::getCurrentWord() const
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
	string str = tr.lpstrText;
	delete []word;
	return str;
}

string RDOEditorSciEdit::getSelection() const
{
	CharacterRange cr = getSelectionRange();
	char* selection = new char[ cr.cpMax - cr.cpMin + 1 ];
	sendEditor( SCI_GETSELTEXT, 0, (long)selection );
	return selection;
}

string RDOEditorSciEdit::getCurrentOrSelectedWord() const
{
	if ( isSelected() ) {
		return getSelection();
	} else {
		return getCurrentWord();
	}
}

CharacterRange RDOEditorSciEdit::getSelectionRange() const
{
	CharacterRange crange;
	crange.cpMin = sendEditor( SCI_GETSELECTIONSTART );
	crange.cpMax = sendEditor( SCI_GETSELECTIONEND );
	return crange;
}

void RDOEditorSciEdit::gotoLineEnsureVisible( int line ) const
{
	sendEditor( SCI_ENSUREVISIBLEENFORCEPOLICY, line );
	sendEditor( SCI_GOTOLINE, line );
}

void RDOEditorSciEdit::ensureRangeVisible( int posStart, int posEnd, bool enforcePolicy ) const
{
	int lineStart = sendEditor( SCI_LINEFROMPOSITION, posStart < posEnd ? posStart : posEnd );
	int lineEnd   = sendEditor( SCI_LINEFROMPOSITION, posStart > posEnd ? posStart : posEnd );
	for ( int line = lineStart; line <= lineEnd; line++ ) {
		sendEditor( enforcePolicy ? SCI_ENSUREVISIBLEENFORCEPOLICY : SCI_ENSUREVISIBLE, line );
	}
}

void RDOEditorSciEdit::OnSearchFind() 
{
	firstFoundPos = -1;
	CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	DWORD flag = (bSearchDown ? FR_DOWN : 0) | (bMatchCase ? FR_MATCHCASE : 0) | (bMatchWholeWord ? FR_WHOLEWORD : 0);
	pDlg->Create( true, getCurrentOrSelectedWord().c_str(), NULL, flag, this );
}

void RDOEditorSciEdit::OnSearchReplace() 
{
	firstFoundPos = -1;
	CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	DWORD flag = (bSearchDown ? FR_DOWN : 0) | (bMatchCase ? FR_MATCHCASE : 0) | (bMatchWholeWord ? FR_WHOLEWORD : 0);
	pDlg->Create( false, getCurrentOrSelectedWord().c_str(), NULL, flag, this );
}

void RDOEditorSciEdit::OnSearchFindNext() 
{
	firstFoundPos = -1;
	findNext( findStr, bSearchDown, bMatchCase, bMatchWholeWord );
}

void RDOEditorSciEdit::OnSearchFindPrevious() 
{
	firstFoundPos = -1;
	findNext( findStr, !bSearchDown, bMatchCase, bMatchWholeWord );
}

void RDOEditorSciEdit::OnSearchFindNextFast() 
{
	firstFoundPos = getCurrentPos();
	findStr       = getCurrentOrSelectedWord();
	bSearchDown   = true;
	findNext( findStr, bSearchDown, bMatchCase, bMatchWholeWord );
}

void RDOEditorSciEdit::OnSearchFindPreviousFast() 
{
	firstFoundPos = getCurrentPos();
	findStr       = getCurrentOrSelectedWord();
	bSearchDown   = true;
	findNext( findStr, !bSearchDown, bMatchCase, bMatchWholeWord );
}

LRESULT RDOEditorSciEdit::OnFindReplaceMsg( WPARAM wParam, LPARAM lParam )
{
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	findStr = pDialog->GetFindString();

	if ( pDialog->IsTerminating() ) {
		firstFoundPos = -1;
		SetFocus();
		return 0;
	} else {
		bool newSearchDown     = pDialog->SearchDown() ? true : false;
		bool newMatchCase      = pDialog->MatchCase() ? true : false;
		bool newMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
		if ( newSearchDown != bSearchDown || newMatchCase != bMatchCase || newMatchWholeWord != bMatchWholeWord ) {
			firstFoundPos = -1;
		}
		bSearchDown     = newSearchDown;
		bMatchCase      = newMatchCase;
		bMatchWholeWord = newMatchWholeWord;

		if ( pDialog->FindNext() ) {

			findNext( findStr, bSearchDown, bMatchCase, bMatchWholeWord );

		} else if ( pDialog->ReplaceCurrent() ) {

			string replaceWhat = (LPCTSTR)pDialog->GetReplaceString();
			replace( findStr, replaceWhat, bSearchDown, bMatchCase, bMatchWholeWord );

		} else if ( pDialog->ReplaceAll() ) {

			string replaceWhat = (LPCTSTR)pDialog->GetReplaceString();
			replaceAll( findStr, replaceWhat, bMatchCase, bMatchWholeWord );

		}
	}
	return 0;
}

void RDOEditorSciEdit::OnUpdateSearchFindNextPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !findStr.empty() );
}

void RDOEditorSciEdit::OnUpdateSearchFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !isEmpty() );
}

void RDOEditorSciEdit::OnUpdateSearchReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !isReadOnly() && !isEmpty() );
}

void RDOEditorSciEdit::findNext( string& findWhat, const bool searchDown, const bool matchCase, const bool matchWholeWord )
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
		MessageBox( format( ID_MSG_CANTFIND, findWhat.c_str() ).c_str(), NULL, MB_OK | MB_ICONWARNING );
		SetFocus();
	} else {
		if ( firstFoundPos == -1 ) {
			firstFoundPos = posFind;
		} else if ( posFind == firstFoundPos ) {
			firstFoundPos = -1;
			bHaveFound    = false;
			MessageBox( format( ID_MSG_CANTFIND, findWhat.c_str() ).c_str(), NULL, MB_OK | MB_ICONWARNING );
			SetFocus();
			return;
		}
		bHaveFound = true;
		int start  = sendEditor( SCI_GETTARGETSTART );
		int end    = sendEditor( SCI_GETTARGETEND );
		ensureRangeVisible( start, end );
		setSelection( start, end );
	}
}

void RDOEditorSciEdit::replace( string& findWhat, string& replaceWhat, const bool searchDown, const bool matchCase, const bool matchWholeWord )
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

void RDOEditorSciEdit::replaceAll( string& findWhat, string& replaceWhat, const bool matchCase, const bool matchWholeWord )
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
		MessageBox( format( ID_MSG_CANTFIND, findWhat.c_str() ).c_str(), NULL, MB_OK | MB_ICONWARNING );
		SetFocus();
	}
}

void RDOEditorSciEdit::clearAll() const
{
	bool readOnly = isReadOnly();
	setReadOnly( false );
	sendEditor( SCI_CLEARALL );
	setReadOnly( readOnly );
}

bool RDOEditorSciEdit::bookmarkToggle( int line ) const
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

bool RDOEditorSciEdit::bookmarkNext( const bool canLoop, const bool fromCurrentLine, bool* wasLoop ) const
{
	bool wasFound = false;
	bool was_loop = false;

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

bool RDOEditorSciEdit::bookmarkPrev( const bool canLoop, const bool fromCurrentLine, bool* wasLoop ) const
{
	bool wasFound = false;
	bool was_loop = false;

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

void RDOEditorSciEdit::bookmarkClearAll() const
{
	sendEditor( SCI_MARKERDELETEALL, sci_MARKER_BOOKMARK );
}

bool RDOEditorSciEdit::hasBookmarks() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_BOOKMARK );
	return nextLine >= 0;
}

void RDOEditorSciEdit::copyAsRTF()
{
	if ( isSelected() ) {

		CSharedFile memFile;

		CharacterRange cr = getSelectionRange();
		saveAsRTF( memFile, cr.cpMin, cr.cpMax );

		HGLOBAL hData = memFile.Detach();
		if ( OpenClipboard() ) {
			::EmptyClipboard();
			::SetClipboardData( ::RegisterClipboardFormat( CF_RTF ), hData );
			CloseClipboard();
		}

	}
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEdit: some functions for RTF export ---------- BEGIN
// ----------------------------------------------------------------------------
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
		strcat (delta, RTF_SETFONTFACE);
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
		strcat (delta, RTF_SETFONTSIZE);
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
		strcat (delta, RTF_SETCOLOR);
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
		strcat (delta, RTF_SETBACKGROUND);
		lastOffset = strlen(delta);
		memcpy(delta + lastOffset, last + offset, len);
		delta[lastOffset + len] = '\0';
	}
	offset = currentOffset + 2;
	// bold
	if (last[offset] != current[offset]) {
		if (current[offset] == '\\') { // turn on
			memmove (last + offset, last + offset + 1, lastLen-- - offset);
			strcat (delta, RTF_BOLD_ON);
			offset += 2;
		} else { // turn off
			memmove (last + offset + 1, last + offset, ++lastLen - offset);
			last[offset] = '0';
			strcat (delta, RTF_BOLD_OFF);
			offset += 3;
		}
	} else
		offset += current[offset] == '\\' ? 2 : 3;
	// italic
	if (last[offset] != current[offset]) {
		if (current[offset] == '\\') { // turn on
			memmove (last + offset, last + offset + 1, lastLen-- - offset);
			strcat (delta, RTF_ITALIC_ON);
		} else { // turn off
			memmove (last + offset + 1, last + offset, ++lastLen - offset);
			last[offset] = '0';
			strcat (delta, RTF_ITALIC_OFF);
		}
	}
	if (*delta) {
		lastOffset = strlen(delta);
		delta[lastOffset] = ' ';
		delta[lastOffset + 1] = '\0';
	}
}
// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEdit: some functions for RTF export ---------- END
// ----------------------------------------------------------------------------

void RDOEditorSciEdit::saveAsRTF( CFile& file, int start, int end ) const
{
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

	string saveStr;

	saveStr = "";
	saveStr += RTF_HEADEROPEN;
	saveStr += RTF_FONTDEFOPEN;

	strncpy( *fonts, style->font->name.c_str(), MAX_FONTDEF );
	saveStr += format( RTF_FONTDEF, 0, style->font->characterSet, style->font->name.c_str() );
	strncpy( *colors, "#000000", MAX_COLORDEF );

	for ( int istyle = 0; istyle <= STYLE_DEFAULT; istyle++ ) {
		if ( style->theme->styleUsing( istyle ) ) {
			sprintf( lastStyle, RTF_SETFONTFACE "%d", fontCount-1 );
			sprintf( lastStyle + strlen(lastStyle), RTF_SETFONTSIZE "%d", style->font->size * 2 );
			if ( style->theme->styleDefault( istyle ) ) {
				strncpy( colors[colorCount++], style->theme->styleBGColorToHEX( istyle ).c_str(), MAX_COLORDEF );
			}
			strncpy( colors[colorCount++], style->theme->styleFGColorToHEX( istyle ).c_str(), MAX_COLORDEF );
			bool bold   = style->theme->styleBold( istyle );
			bool italic = style->theme->styleItalic( istyle );
			sprintf( lastStyle + strlen(lastStyle), RTF_SETCOLOR "%d", colorCount-1 );
			sprintf( lastStyle + strlen(lastStyle), RTF_SETBACKGROUND "%d", 1 );
			strcat( lastStyle, bold ? RTF_BOLD_ON : RTF_BOLD_OFF );
			strcat( lastStyle, italic ? RTF_ITALIC_ON : RTF_ITALIC_OFF );
			strncpy( styles[istyle], lastStyle, MAX_STYLEDEF );
		}
	}
	saveStr += RTF_FONTDEFCLOSE;
	saveStr += RTF_COLORDEFOPEN;

	for ( i = 0; i < colorCount; i++ ) {
		saveStr += format( RTF_COLORDEF, GetHexByte(colors[i] + 1), GetHexByte(colors[i] + 3), GetHexByte(colors[i] + 5) );
	}

	saveStr += format( RTF_COLORDEFCLOSE RTF_HEADERCLOSE RTF_BODYOPEN RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0 ", style->font->size * 2 );

	sprintf( lastStyle, RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0" RTF_SETBACKGROUND "0" RTF_BOLD_OFF RTF_ITALIC_OFF, style->font->size * 2 );

	bool prevCR = false;
	int styleCurrent = -1;
	for ( i = start; i < end; i++ ) {
		char ch   = (char)sendEditor( SCI_GETCHARAT, i );
		int style = sendEditor( SCI_GETSTYLEAT, i );
		if ( style != styleCurrent ) {
			GetRTFStyleChange( deltaStyle, lastStyle, styles[style] );
			if ( *deltaStyle ) saveStr += deltaStyle;
			styleCurrent = style;
		}
		if (ch == '{')       saveStr += "\\{";
		else if (ch == '}')  saveStr += "\\}";
		else if (ch == '\\') saveStr += "\\\\";
		else if (ch == '\t') saveStr += RTF_TAB;
		else if (ch == '\n') {
			if ( !prevCR ) saveStr += RTF_EOLN;
		} else if (ch == '\r') saveStr += RTF_EOLN;
		else saveStr += ch;

		prevCR = ch == '\r';
	}
	saveStr += RTF_BODYCLOSE;

	file.Write( saveStr.c_str(), saveStr.length() );
}

void RDOEditorSciEdit::expand( int& line, bool doExpand, bool force, int visLevels, int level ) const
{
	int lineMaxSubord = sendEditor( SCI_GETLASTCHILD, line, level & SC_FOLDLEVELNUMBERMASK );
	line++;
	while ( line <= lineMaxSubord ) {
		if ( force ) {
			if ( visLevels > 0 )
				sendEditor( SCI_SHOWLINES, line, line );
			else
				sendEditor( SCI_HIDELINES, line, line );
		} else {
			if ( doExpand ) sendEditor( SCI_SHOWLINES, line, line );
		}
		int levelLine = level;
		if ( levelLine == -1 ) levelLine = sendEditor( SCI_GETFOLDLEVEL, line );
		if ( levelLine & SC_FOLDLEVELHEADERFLAG ) {
			if ( force ) {
				if ( visLevels > 1 )
					sendEditor( SCI_SETFOLDEXPANDED, line, 1 );
				else
					sendEditor( SCI_SETFOLDEXPANDED, line, 0 );
				expand( line, doExpand, force, visLevels - 1 );
			} else {
				if ( doExpand ) {
					if ( !sendEditor(SCI_GETFOLDEXPANDED, line) ) sendEditor( SCI_SETFOLDEXPANDED, line, 1 );
					expand( line, true, force, visLevels - 1 );
				} else {
					expand( line, false, force, visLevels - 1 );
				}
			}
		} else {
			line++;
		}
	}
}

void RDOEditorSciEdit::foldChanged( int line, int levelNow, int levelPrev ) const
{
	if ( levelNow & SC_FOLDLEVELHEADERFLAG ) {
		if ( !(levelPrev & SC_FOLDLEVELHEADERFLAG) ) {
			sendEditor( SCI_SETFOLDEXPANDED, line, 1 );
		}
	} else if ( levelPrev & SC_FOLDLEVELHEADERFLAG ) {
		if ( !sendEditor( SCI_GETFOLDEXPANDED, line ) ) {
			expand( line, true, false, 0, levelPrev );
		}
	}
}

void RDOEditorSciEdit::toggleAllFolds() const
{
	sendEditor( SCI_COLOURISE, 0, -1 );
	int maxLine = getLineCount();
	bool expanding = true;
	for ( int lineSeek = 0; lineSeek < maxLine; lineSeek++ ) {
		if ( sendEditor(SCI_GETFOLDLEVEL, lineSeek) & SC_FOLDLEVELHEADERFLAG ) {
			expanding = !sendEditor( SCI_GETFOLDEXPANDED, lineSeek );
			break;
		}
	}
	for ( int line = 0; line < maxLine; line++ ) {
		int level = sendEditor(SCI_GETFOLDLEVEL, line);
		if ( (level & SC_FOLDLEVELHEADERFLAG) &&
		     (SC_FOLDLEVELBASE == (level & SC_FOLDLEVELNUMBERMASK)) ) {
			if ( expanding ) {
				sendEditor( SCI_SETFOLDEXPANDED, line, 1 );
				expand( line, true, false, 0, level );
				line--;
			} else {
				int lineMaxSubord = sendEditor( SCI_GETLASTCHILD, line, -1 );
				sendEditor( SCI_SETFOLDEXPANDED, line, 0 );
				if ( lineMaxSubord > line ) {
					sendEditor( SCI_HIDELINES, line + 1, lineMaxSubord );
				}
			}
		}
	}
}

void RDOEditorSciEdit::foldMarginClick( int position, int modifiers ) const
{
	int lineClick = sendEditor( SCI_LINEFROMPOSITION, position );
	if ( (modifiers & SCMOD_SHIFT) && (modifiers & SCMOD_CTRL) ) {
		toggleAllFolds();
	} else {
		int levelClick = sendEditor( SCI_GETFOLDLEVEL, lineClick );
		if ( levelClick & SC_FOLDLEVELHEADERFLAG ) {
			if ( modifiers & SCMOD_SHIFT ) {
				// Ensure all children visible
				sendEditor( SCI_SETFOLDEXPANDED, lineClick, 1 );
				expand( lineClick, true, true, 100, levelClick );
			} else if ( modifiers & SCMOD_CTRL ) {
				if ( sendEditor(SCI_GETFOLDEXPANDED, lineClick) ) {
					// Contract this line and all children
					sendEditor( SCI_SETFOLDEXPANDED, lineClick, 0 );
					expand( lineClick, false, true, 0, levelClick );
				} else {
					// Expand this line and all children
					sendEditor( SCI_SETFOLDEXPANDED, lineClick, 1 );
					expand( lineClick, true, true, 100, levelClick );
				}
			} else {
				// Toggle this line
				sendEditor( SCI_TOGGLEFOLD, lineClick );
			}
		}
	}
}

void RDOEditorSciEdit::OnViewToggleCurrentFold() 
{
	sendEditor( SCI_TOGGLEFOLD, getCurrentLineNumber() );
}

void RDOEditorSciEdit::OnViewToggleAllFolds() 
{
	toggleAllFolds();
}

void RDOEditorSciEdit::OnUpdateFold( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( !isEmpty() );
}

void RDOEditorSciEdit::replaceCurrent( const string str, const int changePosValue ) const
{
	int pos;
	if ( changePosValue != -1 ) pos = getCurrentPos();

	sendEditor( SCI_REPLACESEL, 0, (long)str.c_str() );

	if ( changePosValue != -1 ) {
		pos += changePosValue;
		setCurrentPos( pos );
	}
}

void RDOEditorSciEdit::appendText( const string& str ) const
{
	sendEditor( SCI_INSERTTEXT, getLength(), (long)str.c_str() );
}

void RDOEditorSciEdit::setCurrentPos( const int value ) const
{
	sendEditor( SCI_SETCURRENTPOS, value );
	sendEditor( SCI_SETSELECTIONSTART, value );
	sendEditor( SCI_SETSELECTIONEND, value );
}

void RDOEditorSciEdit::setCurrentPos( const int line, int pos_in_line, const bool convert_rdo_tab ) const
{
	int pos = getPositionFromLine( line );
	int line_length = sendEditor( SCI_LINELENGTH, line );
	char currentLine[8000];
	bool canUseLine = false;
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
		int tab_size = style->tab->tabSize;
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

bool RDOEditorSciEdit::isLineVisible( const int line ) const
{
	int first_line = sendEditor( SCI_GETFIRSTVISIBLELINE );
	int last_line = first_line + sendEditor( SCI_LINESONSCREEN );
	return line >= first_line && line <= last_line;
}

void RDOEditorSciEdit::scrollToLine( const int line, int setPosition ) const
{
	sendEditor( SCI_LINESCROLL, 0, line );
	if ( setPosition != -1 ) {
		sendEditor( SCI_GOTOPOS, setPosition );
		setCurrentPos( setPosition );
	}
}

void RDOEditorSciEdit::load( strstream& stream ) const
{
	bool readOnly = isReadOnly();
	setReadOnly( false );

	sendEditor( SCI_ADDTEXT, stream.pcount(), (long)stream.str() );
	stream.freeze( false );

	setReadOnly( readOnly );
}

void RDOEditorSciEdit::save( strstream& stream ) const
{
	char currentLine[8000];
	int cnt = getLineCount();
	for ( int i = 0; i < cnt; i++ ) {
		int length = sendEditor( SCI_LINELENGTH, i );
		sendEditor( SCI_GETLINE, i, (long)currentLine );
		string str( currentLine, length );
		static char szDelims[] = "\n\r";
		str.erase( str.find_last_not_of( szDelims ) + 1, string::npos );
		if ( i == cnt - 1 && str.empty() ) {
		} else {
			stream << str << endl;
		}
	}
}

int RDOEditorSciEdit::indentOfBlock( int line ) const
{
	if ( line < 0 ) return 0;
	return sendEditor( SCI_GETLINEINDENTATION, line );
}

void RDOEditorSciEdit::setLineIndentation( int line, int indent ) const
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

void RDOEditorSciEdit::autoIndent() const
{
	CharacterRange crange = getSelectionRange();
	int selStart      = crange.cpMin;
	int curLine       = getCurrentLineNumber();
	int thisLineStart = getPositionFromLine( curLine );
	int indentSize    = sendEditor( SCI_GETINDENT );
	int indentBlock   = indentOfBlock( curLine - 1 );

	if ( selStart == thisLineStart ) {
		setLineIndentation( curLine, indentBlock );
	}
}

void RDOEditorSciEdit::OnBookmarkToggle()
{
	bookmarkToggle();
}

void RDOEditorSciEdit::OnBookmarkNext()
{
	if ( bookmarkNext( false, true ) ) return;

	if ( !group ) {

		bookmarkNext();

	} else {

		RDOEditorSciEditListIterator it = group->begin();
		while ( it ) {
			if ( *it == this ) break;
			it++;
		}
		if ( !(*it) ) return;

		bool allItem = false;
		bool wasLoop = true;
		while ( !allItem && *it && wasLoop ) {
			it++;
			if ( it == group->end() ) {
				it = group->begin();
			}
			if ( *it == this ) {
				allItem = true;
			} else {
				(*it)->bookmarkNext( false, false, &wasLoop );
			}
		}
		if ( !allItem ) {
			(*it)->SetFocus();
		} else {
			(*it)->bookmarkNext();
		}
	}
}

void RDOEditorSciEdit::OnBookmarkPrev()
{
	if ( bookmarkPrev( false, true ) ) return;

	if ( !group ) {

		bookmarkPrev();

	} else {

		RDOEditorSciEditListIterator it = group->begin();
		while ( it ) {
			if ( *it == this ) break;
			it++;
		}
		if ( !(*it) ) return;

		bool allItem = false;
		bool wasLoop = true;
		while ( !allItem && *it && wasLoop ) {
			if ( it == group->begin() ) {
				it = group->end();
			}
			it--;
			if ( *it == this ) {
				allItem = true;
			} else {
				(*it)->bookmarkPrev( false, false, &wasLoop );
			}
		}
		if ( !allItem ) {
			(*it)->SetFocus();
		} else {
			(*it)->bookmarkPrev();
		}
	}
}

void RDOEditorSciEdit::OnBookmarkClearAll()
{
	if ( !group ) {
		bookmarkClearAll();
	} else {
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->bookmarkClearAll();
		}
	}
}

void RDOEditorSciEdit::OnHasBookmarks( CCmdUI* pCmdUI )
{
	bool flag = false;
	if ( !group ) {
		flag = hasBookmarks();
	} else {
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			flag = (*it)->hasBookmarks();
			if ( flag ) break;
		}
	}
	pCmdUI->Enable( flag );
}

void RDOEditorSciEdit::OnViewWhiteSpace() 
{
	if ( !group ) {
		setViewWhiteSpace( !isViewWhiteSpace() );
	} else {
		bool flag = !isViewWhiteSpace();
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->setViewWhiteSpace( flag );
		}
	}
}

void RDOEditorSciEdit::OnViewEndOfLine() 
{
	if ( !group ) {
		setEndOfLine( !isViewEndOfLine() );
	} else {
		bool flag = !isViewEndOfLine();
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->setEndOfLine( flag );
		}
	}
}

void RDOEditorSciEdit::OnViewLineNumberMargin() 
{
	if ( !group ) {
		setViewLineNumberMargin( !isViewLineNumberMargin() );
	} else {
		bool flag = !isViewLineNumberMargin();
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->setViewLineNumberMargin( flag );
		}
	}
}

void RDOEditorSciEdit::OnViewMargin() 
{
	if ( !group ) {
		setViewMargin( !isViewMargin() );
	} else {
		bool flag = !isViewMargin();
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->setViewMargin( flag );
		}
	}
}

void RDOEditorSciEdit::OnViewFoldMargin() 
{
	if ( !group ) {
		setViewFoldMargin( !isViewFoldMargin() );
	} else {
		bool flag = !isViewFoldMargin();
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->setViewFoldMargin( flag );
		}
	}
}

void RDOEditorSciEdit::OnViewZoomIn() 
{
	if ( !group ) {
		zoomIn();
	} else {
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->zoomIn();
		}
	}
}

void RDOEditorSciEdit::OnViewZoomOut() 
{
	if ( !group ) {
		zoomOut();
	} else {
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->zoomOut();
		}
	}
}

void RDOEditorSciEdit::OnViewZoomReset() 
{
	if ( !group ) {
		resetZoom();
	} else {
		for ( RDOEditorSciEditListIterator it = group->begin(); it != group->end(); it++ ) {
			(*it)->resetZoom();
		}
	}
}

void RDOEditorSciEdit::OnUpdateWhiteSpace( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck( isViewWhiteSpace() );
}

void RDOEditorSciEdit::OnUpdateEndOfLine( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck( isViewEndOfLine() );
}

void RDOEditorSciEdit::OnUpdateViewLineNumberMargin( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck( isViewLineNumberMargin() );
}

void RDOEditorSciEdit::OnUpdateViewMargin( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck( isViewMargin() );
}

void RDOEditorSciEdit::OnUpdateViewFoldMargin( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck( isViewFoldMargin() );
}

void RDOEditorSciEdit::OnUpdateZoomIn( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( getZoom() < 20 );
}

void RDOEditorSciEdit::OnUpdateZoomOut( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( getZoom() > -10 );
}

void RDOEditorSciEdit::OnUpdateZoomReset( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( getZoom() );
}

int RDOEditorSciEdit::findLine( string& findWhat, const int startFromLine, const bool matchCase, const bool matchWholeWord ) const
{
	int findLen = findWhat.length();
	if ( !findLen ) return -1;

	int startPosition = getPositionFromLine( startFromLine );
	int endPosition   = getLength();

	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor( SCI_SETTARGETSTART, startPosition );
	sendEditor( SCI_SETTARGETEND, endPosition );
	sendEditor( SCI_SETSEARCHFLAGS, flags );
	int posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat.c_str() );
	if ( posFind != -1 ) {
		return sendEditor( SCI_LINEFROMPOSITION, posFind );
	}
	return -1;
}

string RDOEditorSciEdit::getLine( const int line ) const
{
	int length = sendEditor( SCI_LINELENGTH, line );
	string str;
	str.resize( length );
	sendEditor( SCI_GETLINE, line, (long)str.data() );
	return str;
}
