#include "stdafx.h"
#include "rdoeditoredit.h"
#include "rdoeditortabctrl.h"
#include "rdoeditormainfrm.h"
#include "rdoeditorapp.h"
#include "sci/SciLexer.h"
#include "sci/LexRdo.h"
#include "sci/PropSet.h"
#include "sci/Scintilla.h"
#include "../resource.h"
#include "../Htmlhelp.h"

#include <afxadv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- CBufferDlg
// ----------------------------------------------------------------------------
class CBufferDlg: public CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );

public:
	CString bufName;
	CString bufValue;

	CBufferDlg( CString _bufName, CString _bufValue ): CDialog( IDD_BUFFER_DIALOG ), bufName( _bufName ), bufValue( _bufValue ) {};
};

void CBufferDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Text( pDX, IDC_BUFFERVALUESTATIC, bufName );
	DDX_Text( pDX, IDC_BUFFERVALUEEDIT, bufValue );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEdit
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
// RDO lexer

static char* kw0 = "$Activities $Back_picture $Body $Changes $Compare_tops $Condition \
$Constant $Decision_point $Default $End $End_picture $Evaluate_by $Frame $Function \
$Include $Operations $Parameters $Pattern $Relevant_resources $Resource_type \
$Resources $Result_values $Results $Sequence $Status $Term_condition $Time $Tracing \
$Type $Watching active AExpCalcCounter after algorithmic all and Animation before \
BExpCalcCounter bitmap Break_point by_hist Calculate_if Choice Convert_begin \
Convert_end Convert_event Convert_rule Create ellipse enumerative Erase EventCount \
exponential first FALSE Frame_file Frame_number from get_value Get_value integer \
irregular_event Keep keyboard line list longint Model_name Monitor NO NO_MORE_EVENTS \
NoChange NoCheck NonExist normal NORMAL_TERMINATION NoShow operation \
OperRuleCheckCounter OprIev_file or permanent r_rect real rect Resource_file \
Results_file rule Run_file Run_StartTime RUN_TIME_ERROR s_bmp search Seconds selected \
set Show Show_if Show_mode Show_rate some Statistic_file such_as table temporary \
Terminate_if text Time_now Trace_EndTime Trace_file Trace_StartTime triang TRUE \
uniform until USER_BREAK value watch_par watch_quant watch_state watch_value with_max \
with_min YES transparent";

static char* kw1 = "Abs ArcCos ArcSin ArcTan Cos Cotan Exist Exp Floor For_All Frac \
GetRelResNumber GetResNumber IAbs IMax IMin Int IntPower Ln Log10 Log2 LogN Max Min \
Not_Exist Not_For_All Power Round Sin Sqrt Tan";

static char* kw2 = "no_trace trace trace_all trace_stat trace_tops";

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÀàÁáÂâÃãÄäÅå¨¸ÆæÇçÈèÉéÊêËëÌìÍíÎîÏïÐðÑñÒòÓóÔôÕõÖö×÷ØøÙùÚúÛûÜüÝýÞþßÿ";

// ---------------

static const UINT FIND_REPLASE_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

#define Sci_MARKER_ERROR    0
#define Sci_MARKER_BOOKMARK 1
#define Sci_FOLDMARGIN_ID   2

BEGIN_MESSAGE_MAP( RDOEditorEdit, CWnd )

	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()

	ON_COMMAND_EX( ID_EDIT_UNDO            , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_REDO            , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_CUT             , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_COPY            , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_PASTE           , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_CLEAR           , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_COPYASRTF       , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_SELECT_ALL      , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_UPPERCASE       , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_LOWERCASE       , OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_COMMENTSELECTION, OnSciCommand )
	ON_COMMAND_EX( ID_EDIT_COMPLETEWORD    , OnSciCommand )
	ON_UPDATE_COMMAND_UI( ID_EDIT_UNDO            , OnCanUndo )
	ON_UPDATE_COMMAND_UI( ID_EDIT_REDO            , OnCanRedo )
	ON_UPDATE_COMMAND_UI( ID_EDIT_CUT             , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPY            , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_PASTE           , OnCanPaste )
	ON_UPDATE_COMMAND_UI( ID_EDIT_CLEAR           , OnCanDelete )
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPYASRTF       , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_SELECT_ALL      , OnSelectAll )
	ON_UPDATE_COMMAND_UI( ID_EDIT_UPPERCASE       , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_LOWERCASE       , OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_EDIT_COMMENTSELECTION, OnIsSelected )

	ON_COMMAND_EX( ID_SEARCH_FIND             , OnFindReplace )
	ON_COMMAND_EX( ID_SEARCH_REPLACE          , OnFindReplace )
	ON_COMMAND_EX( ID_SEARCH_FINDNEXT         , OnFindNextPrev )
	ON_COMMAND_EX( ID_SEARCH_FINDPREVIOUS     , OnFindNextPrev )
	ON_COMMAND_EX( ID_SEARCH_FINDNEXT_FAST    , OnFindNextPrevFast )
	ON_COMMAND_EX( ID_SEARCH_FINDPREVIOUS_FAST, OnFindNextPrevFast )
	ON_REGISTERED_MESSAGE( FIND_REPLASE_MSG   , OnFindReplaceMsg )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FINDNEXT    , OnUpdateFindNextPrev )
	ON_UPDATE_COMMAND_UI( ID_SEARCH_FINDPREVIOUS, OnUpdateFindNextPrev )

	ON_COMMAND_EX( ID_VIEW_TOGGLECURRENTFOLD, OnSciCommand )
	ON_COMMAND_EX( ID_VIEW_TOGGLEALLFOLDS   , OnSciCommand )

	ON_COMMAND_RANGE( ID_INSERT_PAT_PATOPERATION, ID_INSERT_FUNCTIONS_TAN, OnInsertCommand )

	ON_COMMAND_EX( ID_INSERT_BUFFER1_PASTE, OnBufferPaste )
	ON_COMMAND_EX( ID_INSERT_BUFFER2_PASTE, OnBufferPaste )
	ON_COMMAND_EX( ID_INSERT_BUFFER3_PASTE, OnBufferPaste )
	ON_COMMAND_EX( ID_INSERT_BUFFER4_PASTE, OnBufferPaste )
	ON_COMMAND_EX( ID_INSERT_BUFFER1_APPEND, OnBufferAppend )
	ON_COMMAND_EX( ID_INSERT_BUFFER2_APPEND, OnBufferAppend )
	ON_COMMAND_EX( ID_INSERT_BUFFER3_APPEND, OnBufferAppend )
	ON_COMMAND_EX( ID_INSERT_BUFFER4_APPEND, OnBufferAppend )
	ON_COMMAND_EX( ID_INSERT_BUFFER1_EDIT, OnBufferEdit )
	ON_COMMAND_EX( ID_INSERT_BUFFER2_EDIT, OnBufferEdit )
	ON_COMMAND_EX( ID_INSERT_BUFFER3_EDIT, OnBufferEdit )
	ON_COMMAND_EX( ID_INSERT_BUFFER4_EDIT, OnBufferEdit )
	ON_COMMAND_EX( ID_INSERT_BUFFER1_CLEAR, OnBufferClear )
	ON_COMMAND_EX( ID_INSERT_BUFFER2_CLEAR, OnBufferClear )
	ON_COMMAND_EX( ID_INSERT_BUFFER3_CLEAR, OnBufferClear )
	ON_COMMAND_EX( ID_INSERT_BUFFER4_CLEAR, OnBufferClear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_PASTE, OnUndateBuffer1Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_PASTE, OnUndateBuffer2Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_PASTE, OnUndateBuffer3Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_PASTE, OnUndateBuffer4Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_CLEAR, OnUndateBuffer1Clear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_CLEAR, OnUndateBuffer2Clear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_CLEAR, OnUndateBuffer3Clear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_CLEAR, OnUndateBuffer4Clear )

	ON_COMMAND( ID_HELP_KEYWORD, OnHelpKeyword )

END_MESSAGE_MAP()

int RDOEditorEdit::objectCount = 0;

RDOEditorEdit::RDOEditorEdit():
	CWnd(),
	sciHWND( 0 ),
	sciEditor( 0 ),
	sciFun( NULL ),
	popupMenu( NULL ),
	bufSelStart( -1 ),
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
}

RDOEditorEdit::~RDOEditorEdit()
{
	objectCount--;
	if ( !objectCount ) {
		Scintilla_ReleaseResources();
	}
}

BOOL RDOEditorEdit::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass( 0 );
	return TRUE;
}

BOOL RDOEditorEdit::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	SCNotification* scn = (SCNotification*)lParam;

	if ( scn->nmhdr.hwndFrom == sciHWND ) {
		switch( scn->nmhdr.code ) {
			case SCN_RDO_BUFFERKEY: {
				switch ( rdoEditorProject.currentBuffer ) {
					case 1: rdoEditorProject.buf1 += scn->ch; break;
					case 2: rdoEditorProject.buf2 += scn->ch; break;
					case 3: rdoEditorProject.buf3 += scn->ch; break;
					case 4: rdoEditorProject.buf4 += scn->ch; break;
				}
				break;
			}
			case SCN_RDO_POSCHANGED: {
				bufSelStart = -1;
				break;
			}
			case SCN_RDO_CLICK: {
				bufSelStart = -1;
				break;
			}
			case SCN_MODIFIED: {
				bufSelStart = -1;
				if ( scn->modificationType & SC_MOD_CHANGEFOLD ) {
					foldChanged( scn->line, scn->foldLevelNow, scn->foldLevelPrev );
					return FALSE;
				}
				if ( hasErrorLine() ) clearErrorLine();
				break;
			}
			case SCN_NEEDSHOWN: {
				ensureRangeVisible( scn->position, scn->position + scn->length, false );
				return TRUE;
			}
			case SCN_MARGINCLICK: {
				if ( scn->margin == Sci_FOLDMARGIN_ID ) {
					foldMarginClick( scn->position, scn->modifiers );
					return TRUE;
				}
				break;
			}
			case SCN_CHARADDED: {
				bufSelStart = -1;
				if ( style.tab.autoIndent && ( scn->ch == '\r' || scn->ch == '\n' ) ) autoIndent();
				break;
			}
		}
	}
	return FALSE;
}

int RDOEditorEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
	sciHWND = CreateWindowEx( 0,  "Scintilla", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 10, 10, 500, 400, m_hWnd, 0, 0, NULL );
	sciFun = (sciFunType)::SendMessage( sciHWND, SCI_GETDIRECTFUNCTION, 0, 0 );
	sciEditor = (long)::SendMessage( sciHWND, SCI_GETDIRECTPOINTER, 0, 0 );
	sendEditor( SCI_SETLEXER, SCLEX_RDO );
	int lexLanguage = sendEditor( SCI_GETLEXER );
	sendEditor( SCI_SETSTYLEBITS, 5 );
	sendEditorString( SCI_SETKEYWORDS, 0, kw0 );
	sendEditorString( SCI_SETKEYWORDS, 1, kw1 );
	sendEditorString( SCI_SETKEYWORDS, 2, kw2 );
	sendEditorString( SCI_SETWORDCHARS, 0, wordCharacters );

	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold"), "1" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold.compact"), "1" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold.flags"), "16" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold.symbols"), "1" );

	sendEditor( SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD | SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT );
	sendEditor( SCI_SETMARGINTYPEN     , Sci_FOLDMARGIN_ID, SC_MARGIN_SYMBOL );
	sendEditor( SCI_SETFOLDFLAGS, 16 );
	sendEditor( SCI_SETMARGINMASKN     , Sci_FOLDMARGIN_ID, SC_MASK_FOLDERS );
	sendEditor( SCI_SETMARGINSENSITIVEN, Sci_FOLDMARGIN_ID, 1 );

	sendEditor( SCI_AUTOCSETIGNORECASE    , 1 );
	sendEditor( SCI_AUTOCSETCHOOSESINGLE  , 0 );
	sendEditor( SCI_AUTOCSETDROPRESTOFWORD, true );
	sendEditor( SCI_AUTOCSETCANCELATSTART , false );
	sendEditor( SCI_AUTOCSETAUTOHIDE      , false );
	sendEditor( SCI_AUTOCSTOPS            , 0, (long)"+-*/:[](),<>=." );

	setViewLineNumberMargin( false );
	setViewMargin( true );
	setViewFoldMargin( true );

	sendEditor( SCI_USEPOPUP, 0 );

	return 0;
}

void RDOEditorEdit::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd );
	if ( sciHWND ) {
		CWnd* wnd = CWnd::FromHandle( sciHWND );
		if ( wnd ) wnd->SetFocus();
	}
}

void RDOEditorEdit::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	CRect r;
	GetClientRect( r );
	::MoveWindow( sciHWND, r.left, r.top, r.right - r.left, r.bottom - r.top, false );
}

void RDOEditorEdit::OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu )
{
	CWnd::OnInitMenuPopup( pPopupMenu, nIndex, bSysMenu );
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if( pwndFrame ) pwndFrame->SendMessage( WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu) );
}

void RDOEditorEdit::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	CWnd::OnContextMenu( pWnd, pos );
	if ( popupMenu ) popupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this );
}

void RDOEditorEdit::defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const
{
  sendEditor( SCI_MARKERDEFINE, marker, markerType );
  sendEditor( SCI_MARKERSETFORE, marker, fore );
  sendEditor( SCI_MARKERSETBACK, marker, back );
}

void RDOEditorEdit::setEditorStyle( const RDOEditorEditStyle& _style )
{
	style = _style;

	// ----------
	// Colors
	sendEditor( SCI_STYLESETFORE, STYLE_DEFAULT, style.theme.defaultColor );
	sendEditor( SCI_STYLESETBACK, STYLE_DEFAULT, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_DEFAULT, style.theme.defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_DEFAULT, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_IDENTIFIER, style.theme.identifierColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_IDENTIFIER, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_KEYWORD, style.theme.keywordColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_KEYWORD, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_FUNCTION, style.theme.functionsColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_FUNCTION, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_TRACE, style.theme.traceColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_TRACE, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_COMMENT, style.theme.commentColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_COMMENT, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_NUMBER, style.theme.numberColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_NUMBER, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_STRING, style.theme.stringColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_STRING, style.theme.backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_OPERATOR, style.theme.operatorColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_OPERATOR, style.theme.backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , STYLE_DEFAULT, style.theme.defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , STYLE_DEFAULT, style.theme.defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, STYLE_DEFAULT, style.theme.defaultStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_DEFAULT, style.theme.defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_DEFAULT, style.theme.defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_DEFAULT, style.theme.defaultStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_IDENTIFIER, style.theme.identifierStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_IDENTIFIER, style.theme.identifierStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_IDENTIFIER, style.theme.identifierStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_KEYWORD, style.theme.keywordStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_KEYWORD, style.theme.keywordStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_KEYWORD, style.theme.keywordStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_FUNCTION, style.theme.functionsStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_FUNCTION, style.theme.functionsStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_FUNCTION, style.theme.functionsStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_TRACE, style.theme.traceStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_TRACE, style.theme.traceStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_TRACE, style.theme.traceStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_COMMENT, style.theme.commentStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_COMMENT, style.theme.commentStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_COMMENT, style.theme.commentStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_NUMBER, style.theme.numberStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_NUMBER, style.theme.numberStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_NUMBER, style.theme.numberStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_STRING, style.theme.stringStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_STRING, style.theme.stringStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_STRING, style.theme.stringStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_OPERATOR, style.theme.operatorStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_OPERATOR, style.theme.operatorStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_OPERATOR, style.theme.operatorStyle & RDOFS_UNDERLINE );

	// ----------
	// Font Name
	sendEditorString( SCI_STYLESETFONT, STYLE_DEFAULT, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_DEFAULT, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_IDENTIFIER, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_KEYWORD, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_FUNCTION, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_TRACE, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_COMMENT, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_NUMBER, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_STRING, style.font.name );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_OPERATOR, style.font.name );

	// ----------
	// Font Size
	sendEditor( SCI_STYLESETSIZE, STYLE_DEFAULT, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_DEFAULT, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_IDENTIFIER, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_KEYWORD, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_FUNCTION, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_TRACE, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_COMMENT, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_NUMBER, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_STRING, style.font.size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_OPERATOR, style.font.size );

	// ----------
	// Codepage and Characterset
	sendEditor( SCI_SETCODEPAGE, style.font.codepage );
	sendEditor( SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_DEFAULT, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_IDENTIFIER, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_KEYWORD, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_FUNCTION, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_TRACE, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_COMMENT, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_NUMBER, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_STRING, style.font.characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_OPERATOR, style.font.characterSet );

	// ----------
	// Tabs
	sendEditor( SCI_SETTABWIDTH, style.tab.tabSize );
	sendEditor( SCI_SETINDENT, style.tab.indentSize );
	sendEditor( SCI_SETUSETABS, style.tab.useTabs );
	sendEditor( SCI_SETTABINDENTS, style.tab.tabIndents );
	sendEditor( SCI_SETBACKSPACEUNINDENTS, !style.tab.backspaceUntabs );

	// ----------
	// Caret
	sendEditor( SCI_SETCARETFORE, style.theme.caretColor );
	sendEditor( SCI_SETSELBACK, true, style.theme.selectionBgColor );
	sendEditor( SCI_SETCARETWIDTH, 1 );

	// ----------
	// Bookmark
	COLORREF bookmarkFgColor = style.theme.bookmarkFgColor;
	COLORREF bookmarkBgColor = style.theme.bookmarkBgColor;
	switch ( style.theme.bookmarkStyle ) {
		case RDOBOOKMARKS_NONE     : defineMarker( Sci_MARKER_BOOKMARK, SC_MARK_EMPTY    , bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_CIRCLE   : defineMarker( Sci_MARKER_BOOKMARK, SC_MARK_CIRCLE   , bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_RECT     : defineMarker( Sci_MARKER_BOOKMARK, SC_MARK_SMALLRECT, bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_ROUNDRECT: defineMarker( Sci_MARKER_BOOKMARK, SC_MARK_ROUNDRECT, bookmarkFgColor, bookmarkBgColor ); break;
		case RDOBOOKMARKS_ARROW    : defineMarker( Sci_MARKER_BOOKMARK, SC_MARK_ARROW    , bookmarkFgColor, bookmarkBgColor ); break;
	}

	// ----------
	// Fold
	COLORREF foldFgColor = style.theme.foldFgColor;
	COLORREF foldBgColor = style.theme.foldBgColor;
	switch ( style.theme.foldStyle ) {
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
	// Error
	defineMarker( Sci_MARKER_ERROR, SC_MARK_BACKGROUND, RGB( 0xFF, 0xFF, 0xFF ), style.theme.errorBgColor );

	// ----------
	// Window
	sendEditor( SCI_SETWRAPMODE, style.window.wordWrap ? SC_WRAP_WORD : SC_WRAP_NONE );
	sendEditor( SCI_SETHSCROLLBAR, style.window.showHorzScrollBar );
}

void RDOEditorEdit::OnSciCommand( UINT nID )
{
	switch ( nID ) {
		case ID_EDIT_UNDO             : sendEditor( SCI_UNDO ); break;
		case ID_EDIT_REDO             : sendEditor( SCI_REDO ); break;
		case ID_EDIT_CUT              : sendEditor( SCI_CUT ); break;
		case ID_EDIT_COPY             : sendEditor( SCI_COPY ); break;
		case ID_EDIT_PASTE            : sendEditor( SCI_PASTE ); break;
		case ID_EDIT_CLEAR            : sendEditor( SCI_CLEAR ); break;
		case ID_EDIT_COPYASRTF        : copyAsRTF(); break;
		case ID_EDIT_SELECT_ALL       : sendEditor( SCI_SELECTALL ); break;
		case ID_EDIT_UPPERCASE        : sendEditor( SCI_UPPERCASE ); break;
		case ID_EDIT_LOWERCASE        : sendEditor( SCI_LOWERCASE ); break;
		case ID_EDIT_COMMENTSELECTION : commentSelection(); break;
		case ID_EDIT_COMPLETEWORD     : completeWord(); break;
		case ID_VIEW_TOGGLECURRENTFOLD: sendEditor( SCI_TOGGLEFOLD, getCurrentLineNumber() ); break;
		case ID_VIEW_TOGGLEALLFOLDS   : toggleAllFolds(); break;
	}
}

CString RDOEditorEdit::getCurrentWord() const
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
	CString str = tr.lpstrText;
	delete []word;
	return str;
}

CString RDOEditorEdit::getSelection() const
{
	CharacterRange cr = getSelectionRange();
	char* selection = new char[ cr.cpMax - cr.cpMin + 1 ];
	sendEditor( SCI_GETSELTEXT, 0, (long)selection );
	return selection;
}

CString RDOEditorEdit::getCurrentOrSelectedWord() const
{
	if ( isSelected() ) {
		return getSelection();
	} else {
		return getCurrentWord();
	}
}

CharacterRange RDOEditorEdit::getSelectionRange() const
{
	CharacterRange crange;
	crange.cpMin = sendEditor( SCI_GETSELECTIONSTART );
	crange.cpMax = sendEditor( SCI_GETSELECTIONEND );
	return crange;
}

void RDOEditorEdit::gotoLineEnsureVisible( int line ) const
{
	sendEditor( SCI_ENSUREVISIBLEENFORCEPOLICY, line );
	sendEditor( SCI_GOTOLINE, line );
}

void RDOEditorEdit::ensureRangeVisible( int posStart, int posEnd, bool enforcePolicy ) const
{
	int lineStart = sendEditor( SCI_LINEFROMPOSITION, posStart < posEnd ? posStart : posEnd );
	int lineEnd   = sendEditor( SCI_LINEFROMPOSITION, posStart > posEnd ? posStart : posEnd );
	for ( int line = lineStart; line <= lineEnd; line++ ) {
		sendEditor( enforcePolicy ? SCI_ENSUREVISIBLEENFORCEPOLICY : SCI_ENSUREVISIBLE, line );
	}
}

void RDOEditorEdit::OnFindReplace( UINT nID )
{
	firstFoundPos = -1;
	CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	DWORD flag = (bSearchDown ? FR_DOWN : 0) | (bMatchCase ? FR_MATCHCASE : 0) | (bMatchWholeWord ? FR_WHOLEWORD : 0);
	switch ( nID ) {
		case ID_SEARCH_FIND   : pDlg->Create( true, getCurrentOrSelectedWord(), NULL, flag, this ); break;
		case ID_SEARCH_REPLACE: pDlg->Create( false, getCurrentOrSelectedWord(), NULL, flag, this ); break;
	}
}

void RDOEditorEdit::OnFindNextPrev( UINT nID )
{
	firstFoundPos = -1;
	switch ( nID ) {
		case ID_SEARCH_FINDNEXT    : findNext( findStr, bSearchDown, bMatchCase, bMatchWholeWord ); break;
		case ID_SEARCH_FINDPREVIOUS: findNext( findStr, !bSearchDown, bMatchCase, bMatchWholeWord ); break;
	}
}

void RDOEditorEdit::OnFindNextPrevFast( UINT nID )
{
	firstFoundPos = getCurrentPos();
	findStr       = getCurrentOrSelectedWord();
	bSearchDown   = true;
	switch ( nID ) {
		case ID_SEARCH_FINDNEXT_FAST    : findNext( findStr, bSearchDown, bMatchCase, bMatchWholeWord ); break;
		case ID_SEARCH_FINDPREVIOUS_FAST: findNext( findStr, !bSearchDown, bMatchCase, bMatchWholeWord ); break;
	}
}

LRESULT RDOEditorEdit::OnFindReplaceMsg( WPARAM wParam, LPARAM lParam )
{
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	findStr = pDialog->GetFindString();

	if ( pDialog->IsTerminating() ) {
		firstFoundPos = -1;
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

			replace( findStr, pDialog->GetReplaceString(), bSearchDown, bMatchCase, bMatchWholeWord );

		} else if ( pDialog->ReplaceAll() ) {

			replaceAll( findStr, pDialog->GetReplaceString(), bMatchCase, bMatchWholeWord );

		}
	}
	return 0;
}

void RDOEditorEdit::findNext( CString& findWhat, const bool searchDown, const bool matchCase, const bool matchWholeWord )
{
	int findLen = findWhat.GetLength();
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
	int posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat );
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
		posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat );
	}
	if ( posFind == -1 ) {
		firstFoundPos = -1;
		bHaveFound    = false;
		CString s;
		s.Format( ID_MSG_CANTFIND, findWhat );
		MessageBox( s, NULL, MB_OK | MB_ICONWARNING );
	} else {
		if ( firstFoundPos == -1 ) {
			firstFoundPos = posFind;
		} else if ( posFind == firstFoundPos ) {
			firstFoundPos = -1;
			bHaveFound    = false;
			CString s;
			s.Format( ID_MSG_CANTFIND, findWhat );
			MessageBox( s, NULL, MB_OK | MB_ICONWARNING );
			return;
		}
		bHaveFound = true;
		int start  = sendEditor( SCI_GETTARGETSTART );
		int end    = sendEditor( SCI_GETTARGETEND );
		ensureRangeVisible( start, end );
		setSelection( start, end );
	}
}

void RDOEditorEdit::replace( CString& findWhat, CString& replaceWhat, const bool searchDown, const bool matchCase, const bool matchWholeWord )
{
	if ( bHaveFound ) {
		int replaceLen = replaceWhat.GetLength();
		CharacterRange cr = getSelectionRange();
		sendEditor( SCI_SETTARGETSTART, cr.cpMin );
		sendEditor( SCI_SETTARGETEND, cr.cpMax );
		int lenReplaced = replaceLen;
		sendEditorString( SCI_REPLACETARGET, replaceLen, replaceWhat );
		setSelection( cr.cpMin + lenReplaced, cr.cpMin );
		bHaveFound = false;
	}
	findNext( findWhat, searchDown, matchCase, matchWholeWord );
}

void RDOEditorEdit::replaceAll( CString& findWhat, CString& replaceWhat, const bool matchCase, const bool matchWholeWord )
{
	int findLen = findWhat.GetLength();
	if ( !findLen ) return;

	int startPosition = 0;
	int endPosition   = getLength();

	int replaceLen = replaceWhat.GetLength();
	int flags = (matchCase ? SCFIND_MATCHCASE : 0) | (matchWholeWord ? SCFIND_WHOLEWORD : 0);

	sendEditor( SCI_SETTARGETSTART, startPosition );
	sendEditor( SCI_SETTARGETEND, endPosition );
	sendEditor( SCI_SETSEARCHFLAGS, flags );
	int posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat );

	if ( (posFind != -1) && (posFind <= endPosition) ) {
		int lastMatch = posFind;
		sendEditor( SCI_BEGINUNDOACTION );
		while ( posFind != -1 ) {
			int lenTarget = sendEditor( SCI_GETTARGETEND ) - sendEditor( SCI_GETTARGETSTART );
			int lenReplaced = replaceLen;
			sendEditorString( SCI_REPLACETARGET, replaceLen, replaceWhat );
			endPosition += lenReplaced - lenTarget;
			lastMatch    = posFind + lenReplaced;
			if ( lenTarget <= 0 ) lastMatch++;
			sendEditor( SCI_SETTARGETSTART, lastMatch );
			sendEditor( SCI_SETTARGETEND, endPosition );
			posFind = sendEditorString( SCI_SEARCHINTARGET, findLen, findWhat );
		}
		setSelection( lastMatch, lastMatch );
		sendEditor( SCI_ENDUNDOACTION );
	} else {
		CString s;
		s.Format( ID_MSG_CANTFIND, findWhat );
		MessageBox( s, NULL, MB_OK | MB_ICONWARNING );
	}
}

void RDOEditorEdit::clearAll() const
{
	bool readOnly = isReadOnly();
	setReadOnly( false );
	sendEditor(SCI_CLEARALL);
	setReadOnly( readOnly );
};

bool RDOEditorEdit::bookmarkToggle( int line ) const
{
	if ( line == -1 ) line = getCurrentLineNumber();
	int state = sendEditor( SCI_MARKERGET, line );
	if ( state & (1 << Sci_MARKER_BOOKMARK) ) {
		sendEditor( SCI_MARKERDELETE, line, Sci_MARKER_BOOKMARK );
		return false;
	} else {
		sendEditor( SCI_MARKERADD, line, Sci_MARKER_BOOKMARK );
		return true;
	}
}

bool RDOEditorEdit::bookmarkNext( const bool canLoop, const bool fromCurrentLine, bool* wasLoop ) const
{
	bool wasFound = false;
	bool was_loop = false;

	int line = -1;
	if ( fromCurrentLine ) line = getCurrentLineNumber();

	int nextLine = sendEditor( SCI_MARKERNEXT, line + 1, 1 << Sci_MARKER_BOOKMARK );
	if ( nextLine < 0 ) {
		was_loop = true;
		if ( canLoop ) nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << Sci_MARKER_BOOKMARK );
	}
	if ( (( canLoop && was_loop ) || !was_loop ) && nextLine >= 0 && nextLine != line ) {
		gotoLineEnsureVisible( nextLine );
		wasFound = true;
	}
	if ( wasLoop ) *wasLoop = was_loop;
	return wasFound;
}

bool RDOEditorEdit::bookmarkPrev( const bool canLoop, const bool fromCurrentLine, bool* wasLoop ) const
{
	bool wasFound = false;
	bool was_loop = false;

	int lineCount = getLineCount();
	int line = lineCount + 1;
	if ( fromCurrentLine ) line = getCurrentLineNumber();

	int prevLine  = sendEditor( SCI_MARKERPREVIOUS, line - 1, 1 << Sci_MARKER_BOOKMARK );
	if ( prevLine < 0 ) {
		was_loop = true;
		if ( canLoop ) prevLine = sendEditor( SCI_MARKERPREVIOUS, lineCount, 1 << Sci_MARKER_BOOKMARK );
	}
	if ( (( canLoop && was_loop ) || !was_loop ) && prevLine >= 0 && prevLine != line ) {
		gotoLineEnsureVisible( prevLine );
		wasFound = true;
	}
	if ( wasLoop ) *wasLoop = was_loop;
	return wasFound;
}

void RDOEditorEdit::bookmarkClearAll() const
{
	sendEditor( SCI_MARKERDELETEALL, Sci_MARKER_BOOKMARK );
}

bool RDOEditorEdit::hasBookmarks() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << Sci_MARKER_BOOKMARK );
	return nextLine >= 0;
}

void RDOEditorEdit::setErrorLine( int line )
{
	clearErrorLine();
	if ( line == -1 ) line = getCurrentLineNumber();
	int state = sendEditor( SCI_MARKERGET, line );
	sendEditor( SCI_MARKERADD, line, Sci_MARKER_ERROR );
}

void RDOEditorEdit::clearErrorLine()
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << Sci_MARKER_ERROR );
	if ( nextLine >= 0 ) {
		sendEditor( SCI_MARKERDELETE, nextLine, Sci_MARKER_ERROR );
		RedrawWindow();
	}
}

bool RDOEditorEdit::hasErrorLine() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << Sci_MARKER_ERROR );
	return nextLine >= 0;
}

void RDOEditorEdit::copyAsRTF()
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

void RDOEditorEdit::commentSelection() const
{
	if ( isSelected() ) {
		CString startComment( "{ " );
		CString endComment( " }" );
		int startCommentLength = startComment.GetLength();
		CharacterRange cr = getSelectionRange();
		int caretPosition = getCurrentPos();
		bool moveCaret = caretPosition < cr.cpMax;
		sendEditor( SCI_BEGINUNDOACTION );
		sendEditorString( SCI_INSERTTEXT, cr.cpMin, startComment );
		cr.cpMax += startCommentLength;
		cr.cpMin += startCommentLength;
		sendEditorString( SCI_INSERTTEXT, cr.cpMax, endComment );
		if ( moveCaret ) {
			sendEditor( SCI_GOTOPOS      , cr.cpMax );
			sendEditor( SCI_SETCURRENTPOS, cr.cpMin );
		} else {
			setSelection( cr.cpMin, cr.cpMax );
		}
		sendEditor( SCI_ENDUNDOACTION );
	}
}

void RDOEditorEdit::completeWord() const
{
	if ( !style.autoComplete.useAutoComplete ) return;

	CString s = kw0;
	s += " ";
	s += kw1;
	s += " ";
	s += kw2;

	WordList wl;
	wl.Set( s );
	wl.InList( "" );
	s = "";
	for ( int i = 0; i < wl.len; i++ ) {
		s += wl.wordsNoCase[i];
		if ( i < wl.len-1 ) s += " ";
	}

	char currentLine[8000];
	int line = getCurrentLineNumber();
	sendEditor( SCI_GETLINE, line, (long)currentLine );

	int currentPos = getCurrentPos();
	int lineStart  = sendEditor( SCI_POSITIONFROMLINE, line );
	currentPos -= lineStart;

	int startPos     = currentPos;
	int startPos_old = startPos;

	while ( (startPos > 0) && isRDOLexerIdentifier( currentLine[startPos - 1] ) ) {
		startPos--;
	}

	currentLine[ currentPos ] = '\0';
	const char* str = currentLine + startPos;
	int strLength = currentPos - startPos;

	const char* word = NULL;
	int wordLength = strLength + 1;
	if ( wordLength <= 0 ) wordLength = 1;
	while ( !word && wordLength > 1 ) {
		wordLength--;
		word = wl.GetNearestWord( str, wordLength, true );
	}
	char* words = wl.GetNearestWords( str, wordLength, true );

	LPCTSTR list;
	if ( style.autoComplete.showFullList ) {
		list = s;
	} else {
		list = words;
		if ( !list ) list = s;
	}

	if ( list ) {

		CString startKeyWord       = "";
		CString startKeyWordScroll = wl.wordsNoCase[ wl.len-1 ];
		bool useReplace = false;
		if ( words ) {
			wl.Set( words );
			wl.InList( "" );
			startKeyWord = wl.wordsNoCase[0];
			if ( wl.len == 1 && strLength <= startKeyWord.GetLength() ) {
				useReplace = true;
			}
		}
		if ( useReplace ) {
			setSelection( getCurrentPos(), getCurrentPos() - strLength );
			replaceCurrent( startKeyWord );
		} else {
			sendEditor( SCI_AUTOCSHOW, strLength, (long)list );
			sendEditor( SCI_AUTOCSELECT, 0, (long)((LPCTSTR)startKeyWordScroll) );
			sendEditor( SCI_AUTOCSELECT, 0, (long)((LPCTSTR)startKeyWord) );
		}
	}

	if ( words ) delete []words;
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEdit: some functions for RTF export ---------- BEGIN
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
// ---------- RDOEditorEdit: some functions for RTF export ---------- END
// ----------------------------------------------------------------------------

void RDOEditorEdit::saveAsRTF( CFile& file, int start, int end ) const
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

	CString str;
	CString saveStr;

	saveStr = "";
	saveStr += RTF_HEADEROPEN;
	saveStr += RTF_FONTDEFOPEN;

	strncpy( *fonts, style.font.name, MAX_FONTDEF );
	str.Format( RTF_FONTDEF, 0, style.font.characterSet, style.font.name );
	saveStr += str;
	strncpy( *colors, "#000000", MAX_COLORDEF );

	for ( int istyle = 0; istyle <= STYLE_DEFAULT; istyle++ ) {

		if ( istyle >= SCE_RDO_DEFAULT && istyle <= SCE_RDO_OPERATOR ) {

			sprintf( lastStyle, RTF_SETFONTFACE "%d", fontCount-1 );
			sprintf( lastStyle + strlen(lastStyle), RTF_SETFONTSIZE "%d", style.font.size * 2 );

			bool bold   = false;
			bool italic = false;

			switch ( istyle ) {
				case SCE_RDO_DEFAULT   : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.backgroundColor )   , MAX_COLORDEF ); strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.defaultColor )   , MAX_COLORDEF ); bold = style.theme.defaultStyle    & RDOFS_BOLD ? true : false; italic = style.theme.defaultStyle    & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_IDENTIFIER: strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.identifierColor ), MAX_COLORDEF ); bold = style.theme.identifierStyle & RDOFS_BOLD ? true : false; italic = style.theme.identifierStyle & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_KEYWORD   : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.keywordColor )   , MAX_COLORDEF ); bold = style.theme.keywordStyle    & RDOFS_BOLD ? true : false; italic = style.theme.keywordStyle    & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_FUNCTION  : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.functionsColor ) , MAX_COLORDEF ); bold = style.theme.functionsStyle  & RDOFS_BOLD ? true : false; italic = style.theme.functionsStyle  & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_TRACE     : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.traceColor )     , MAX_COLORDEF ); bold = style.theme.traceStyle      & RDOFS_BOLD ? true : false; italic = style.theme.traceStyle      & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_COMMENT   : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.commentColor )   , MAX_COLORDEF ); bold = style.theme.commentStyle    & RDOFS_BOLD ? true : false; italic = style.theme.commentStyle    & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_NUMBER    : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.numberColor )    , MAX_COLORDEF ); bold = style.theme.numberStyle     & RDOFS_BOLD ? true : false; italic = style.theme.numberStyle     & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_STRING    : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.stringColor )    , MAX_COLORDEF ); bold = style.theme.stringStyle     & RDOFS_BOLD ? true : false; italic = style.theme.stringStyle     & RDOFS_ITALIC ? true : false; break;
				case SCE_RDO_OPERATOR  : strncpy( colors[colorCount++], RDOEditorEditTheme::colorToHEX( style.theme.operatorColor )  , MAX_COLORDEF ); bold = style.theme.operatorStyle   & RDOFS_BOLD ? true : false; italic = style.theme.operatorStyle   & RDOFS_ITALIC ? true : false; break;
			}

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
		str.Format( RTF_COLORDEF, GetHexByte(colors[i] + 1), GetHexByte(colors[i] + 3), GetHexByte(colors[i] + 5) );
		saveStr += str;
	}

	str.Format( RTF_COLORDEFCLOSE RTF_HEADERCLOSE RTF_BODYOPEN RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0 ", style.font.size * 2 );
	saveStr += str;

	sprintf( lastStyle, RTF_SETFONTFACE "0" RTF_SETFONTSIZE "%d" RTF_SETCOLOR "0" RTF_SETBACKGROUND "0" RTF_BOLD_OFF RTF_ITALIC_OFF, style.font.size * 2 );

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

	file.Write( saveStr, saveStr.GetLength() );
}

void RDOEditorEdit::expand( int& line, bool doExpand, bool force, int visLevels, int level ) const
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

void RDOEditorEdit::foldChanged( int line, int levelNow, int levelPrev ) const
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

void RDOEditorEdit::toggleAllFolds() const
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
				if ( lineMaxSubord > line )
					sendEditor(SCI_HIDELINES, line + 1, lineMaxSubord);
			}
		}
	}
}

void RDOEditorEdit::foldMarginClick( int position, int modifiers ) const
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

void RDOEditorEdit::replaceCurrent( const CString str, const int changePosValue ) const
{
	int pos;
	if ( changePosValue != -1 ) pos = getCurrentPos();

	sendEditor( SCI_REPLACESEL, 0, (long)((LPCTSTR)str) );

	if ( changePosValue != -1 ) {
		pos += changePosValue;
		setCurrentPos( pos );
	}
}

void RDOEditorEdit::setCurrentPos( const int value ) const
{
	sendEditor( SCI_SETCURRENTPOS, value );
	sendEditor( SCI_SETSELECTIONSTART, value );
	sendEditor( SCI_SETSELECTIONEND, value );
}

void RDOEditorEdit::setCurrentPos( const int line, int pos_in_line, const bool convert_rdo_tab ) const
{
	int pos = sendEditor( SCI_POSITIONFROMLINE, line );
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
		int tab_size = style.tab.tabSize;
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
	sendEditor( SCI_LINESCROLL, 0, line_to_scroll );
	sendEditor( SCI_GOTOPOS, pos );

	setCurrentPos( pos );
}

void RDOEditorEdit::OnInsertCommand( UINT nID )
{
	CString s = "";
	if ( !s.LoadString( nID ) ) {
		AfxGetMainWnd()->GetMenu()->GetMenuString( nID, s, MF_BYCOMMAND );
	}

	int incPos = -1;

	switch ( nID ) {
		case ID_INSERT_PAT_PATOPERATION     :
		case ID_INSERT_PAT_PATIRREGULAREVENT:
		case ID_INSERT_PAT_PATRULE          : incPos = 9; break;
		case ID_INSERT_RTP_RTPPERMANENT     :
		case ID_INSERT_RTP_RTPTEMPORARY     : incPos = 15; break;
		case ID_INSERT_RSS_RSS              : incPos = 13; break;
		case ID_INSERT_OPR_OPR              : incPos = 14; break;
		case ID_INSERT_FRM_FRM              : incPos = 7; break;
		case ID_INSERT_FUN_FUN              :
		case ID_INSERT_FUN_SQN              : incPos = 10; break;
		case ID_INSERT_FUN_CNS              : incPos = 12; break;
		case ID_INSERT_DPT_DPTSEARCH        :
		case ID_INSERT_DPT_DPTSOME          : incPos = 16; break;
		case ID_INSERT_DPT_ACTIV            : incPos = 14; break;
		case ID_INSERT_SMR_SMR              : incPos = 17; break;
		case ID_INSERT_PMD_PMD              : incPos = 11; break;
	}

	replaceCurrent( s, incPos );
}

void RDOEditorEdit::OnBufferPaste( UINT nID )
{
	CString s;
	switch ( nID ) {
		case ID_INSERT_BUFFER1_PASTE: s = rdoEditorProject.buf1; rdoEditorProject.currentBuffer = 1; break;
		case ID_INSERT_BUFFER2_PASTE: s = rdoEditorProject.buf2; rdoEditorProject.currentBuffer = 2; break;
		case ID_INSERT_BUFFER3_PASTE: s = rdoEditorProject.buf3; rdoEditorProject.currentBuffer = 3; break;
		case ID_INSERT_BUFFER4_PASTE: s = rdoEditorProject.buf4; rdoEditorProject.currentBuffer = 4; break;
	}

	replaceCurrent( s );
}

void RDOEditorEdit::OnBufferAppend( UINT nID )
{
	int pos = getCurrentPos();
	if ( pos == getLength() ) return;
	bool canUseSelected = false;
	if ( bufSelStart == -1 ) {
		CharacterRange cr = getSelectionRange();
		bufSelStart       = cr.cpMin;
		pos               = cr.cpMax;
		canUseSelected    = cr.cpMin != cr.cpMax;
	}
	CString s = "";
	UINT timer_ID = 0;
	switch ( nID ) {
		case ID_INSERT_BUFFER1_APPEND: {
			if ( rdoEditorProject.resetBuf1 ) {
				rdoEditorProject.resetBuf1 = false;
			} else {
				s = rdoEditorProject.buf1;
			}
			timer_ID = 1;
			break;
		}
		case ID_INSERT_BUFFER2_APPEND: {
			if ( rdoEditorProject.resetBuf2 ) {
				rdoEditorProject.resetBuf2 = false;
			} else {
				s = rdoEditorProject.buf2;
			}
			timer_ID = 2;
			break;
		}
		case ID_INSERT_BUFFER3_APPEND: {
			if ( rdoEditorProject.resetBuf3 ) {
				rdoEditorProject.resetBuf3 = false;
			} else {
				s = rdoEditorProject.buf3;
			}
			timer_ID = 3;
			break;
		}
		case ID_INSERT_BUFFER4_APPEND: {
			if ( rdoEditorProject.resetBuf4 ) {
				rdoEditorProject.resetBuf4 = false;
			} else {
				s = rdoEditorProject.buf4;
			}
			timer_ID = 4;
			break;
		}
	}

	if ( timer_ID ) {
		((RDOEditorMainFrame*)AfxGetMainWnd())->restartBufTimer( timer_ID );
	}

	if ( canUseSelected ) {
		s += getSelection();
	} else {
		char c = (char)sendEditor( SCI_GETCHARAT, pos );
		int line = sendEditor( SCI_LINEFROMPOSITION, pos );
		s += c;
		pos++;
		if ( c == '\r' || c == '\n' ) {
			while ( line == sendEditor( SCI_LINEFROMPOSITION, pos ) && ( (char)sendEditor( SCI_GETCHARAT, pos ) == '\r' || (char)sendEditor( SCI_GETCHARAT, pos ) == '\n' ) ) {
				s += (char)sendEditor( SCI_GETCHARAT, pos );
				pos++;
			}
		}
	}
	setCurrentPos( pos );
	setSelection( bufSelStart, pos );
	switch ( nID ) {
		case ID_INSERT_BUFFER1_APPEND: rdoEditorProject.buf1 = s; rdoEditorProject.currentBuffer = 1; break;
		case ID_INSERT_BUFFER2_APPEND: rdoEditorProject.buf2 = s; rdoEditorProject.currentBuffer = 2; break;
		case ID_INSERT_BUFFER3_APPEND: rdoEditorProject.buf3 = s; rdoEditorProject.currentBuffer = 3; break;
		case ID_INSERT_BUFFER4_APPEND: rdoEditorProject.buf4 = s; rdoEditorProject.currentBuffer = 4; break;
	}
}

void RDOEditorEdit::OnBufferEdit( UINT nID )
{
	CString bufName;
	CString bufValue;

	bufName.LoadString( ID_BUFFER_NAME );

	switch ( nID ) {
		case ID_INSERT_BUFFER1_EDIT: bufName += " 1:"; bufValue = rdoEditorProject.buf1; break;
		case ID_INSERT_BUFFER2_EDIT: bufName += " 2:"; bufValue = rdoEditorProject.buf2; break;
		case ID_INSERT_BUFFER3_EDIT: bufName += " 3:"; bufValue = rdoEditorProject.buf3; break;
		case ID_INSERT_BUFFER4_EDIT: bufName += " 4:"; bufValue = rdoEditorProject.buf4; break;
	}

	if ( bufValue.IsEmpty() ) {
		RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
		bufValue = tabCtrl->getCurrentEdit()->getCurrentOrSelectedWord();
	}
	CBufferDlg dlg( bufName, bufValue );

	if ( dlg.DoModal() == IDOK ) {

		switch ( nID ) {
			case ID_INSERT_BUFFER1_EDIT: rdoEditorProject.buf1 = dlg.bufValue; rdoEditorProject.currentBuffer = 1; break;
			case ID_INSERT_BUFFER2_EDIT: rdoEditorProject.buf2 = dlg.bufValue; rdoEditorProject.currentBuffer = 2; break;
			case ID_INSERT_BUFFER3_EDIT: rdoEditorProject.buf3 = dlg.bufValue; rdoEditorProject.currentBuffer = 3; break;
			case ID_INSERT_BUFFER4_EDIT: rdoEditorProject.buf4 = dlg.bufValue; rdoEditorProject.currentBuffer = 4; break;
		}

	}
}

void RDOEditorEdit::OnBufferClear( UINT nID )
{
	switch ( nID ) {
		case ID_INSERT_BUFFER1_CLEAR: rdoEditorProject.buf1 = ""; rdoEditorProject.currentBuffer = 1; break;
		case ID_INSERT_BUFFER2_CLEAR: rdoEditorProject.buf2 = ""; rdoEditorProject.currentBuffer = 2; break;
		case ID_INSERT_BUFFER3_CLEAR: rdoEditorProject.buf3 = ""; rdoEditorProject.currentBuffer = 3; break;
		case ID_INSERT_BUFFER4_CLEAR: rdoEditorProject.buf4 = ""; rdoEditorProject.currentBuffer = 4; break;
	}
}

void RDOEditorEdit::load( CFile& file ) const
{
	bool readOnly = isReadOnly();
	setReadOnly( false );

	const int blockSize = 65536;
	char data[blockSize + 1];
	int lengthDoc = file.GetLength();
	TextRange tr;
	tr.lpstrText = data;
	for ( int i = 0; i < lengthDoc; i += blockSize ) {
		int grabSize = lengthDoc - i;
		if ( grabSize > blockSize ) grabSize = blockSize;
		tr.chrg.cpMin = i;
		tr.chrg.cpMax = i + grabSize;
		file.Read( data, grabSize );
		sendEditor( SCI_ADDTEXT, grabSize, (long)data );
	}

	setReadOnly( readOnly );
}

void RDOEditorEdit::save( CFile& file ) const
{
	const int blockSize = 65536;
	char data[blockSize + 1];
	int lengthDoc = getLength();
	TextRange tr;
	tr.lpstrText = data;
	for ( int i = 0; i < lengthDoc; i += blockSize ) {
		int grabSize = lengthDoc - i;
		if ( grabSize > blockSize ) grabSize = blockSize;
		tr.chrg.cpMin = i;
		tr.chrg.cpMax = i + grabSize;
		sendEditor( SCI_GETTEXTRANGE, 0, (long)&tr );
		file.Write( data, grabSize );
	}
}

int RDOEditorEdit::indentOfBlock( int line ) const
{
	if ( line < 0 ) return 0;
	return sendEditor( SCI_GETLINEINDENTATION, line );
}

void RDOEditorEdit::setLineIndentation( int line, int indent ) const
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

void RDOEditorEdit::autoIndent() const
{
	CharacterRange crange = getSelectionRange();
	int selStart      = crange.cpMin;
	int curLine       = getCurrentLineNumber();
	int thisLineStart = sendEditor( SCI_POSITIONFROMLINE, curLine );
	int indentSize    = sendEditor( SCI_GETINDENT );
	int indentBlock   = indentOfBlock( curLine - 1 );

	if ( selStart == thisLineStart ) {
		setLineIndentation( curLine, indentBlock );
	}
}

void RDOEditorEdit::OnHelpKeyword()
{
	CString filename = rdoEditorApp.getFullHelpFileName( "RAO-language.chm" );
	if ( filename.IsEmpty() ) return;

	CString keyword = getCurrentOrSelectedWord();

	CString s = kw0;
	s += " ";
	s += kw1;
	s += " ";
	s += kw2;

	if ( s.Find( keyword ) == -1 || keyword.IsEmpty() ) {
		RDOEditorTabCtrl* tabCtrl = &((RDOEditorMainFrame*)AfxGetMainWnd())->childView.tab;
		RDOEditorTabItem item = tabCtrl->getCurrentRDOItem();
		switch( item ) {
			case RDOEDIT_PAT: keyword = "pat"; break;
			case RDOEDIT_RTP: keyword = "rtp"; break;
			case RDOEDIT_RSS: keyword = "rss"; break;
			case RDOEDIT_OPR: keyword = "opr"; break;
			case RDOEDIT_FRM: keyword = "frm"; break;
			case RDOEDIT_FUN: keyword = "fun"; break;
			case RDOEDIT_DPT: keyword = "dpt"; break;
			case RDOEDIT_SMR: keyword = "smr"; break;
			case RDOEDIT_PMD: keyword = "pmd"; break;
			case RDOEDIT_PMV: keyword = "pmv"; break;
			case RDOEDIT_TRC: keyword = "trc"; break;
			default:          keyword = ""; break;
		}
	}

	HH_AKLINK link;
	::ZeroMemory( &link, sizeof( HH_AKLINK ) );
	link.cbStruct     = sizeof( HH_AKLINK );
	link.fIndexOnFail = TRUE;
	link.pszKeywords  = keyword;

	::HtmlHelp( ::GetDesktopWindow(), filename, HH_KEYWORD_LOOKUP, (DWORD)&link );
}
