#include "stdafx.h"
#include "rdoeditorbaseedit.h"
#include "../edit_ctrls/sci/SciLexer.h"
#include "../resource.h"
//#include "../Htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseEdit
// ---------------------------------------------------------------------------

// ---------------
// RDO lexer

char* RDOEditorBaseEdit::kw0 = "$Activities $Back_picture $Body $Changes $Compare_tops $Condition \
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
with_min YES transparent —ËÒÚÂÏÌÓÂ_‚ÂÏˇ";

char* RDOEditorBaseEdit::kw1 = "Abs ArcCos ArcSin ArcTan Cos Cotan Exist Exp Floor For_All Frac \
GetRelResNumber GetResNumber IAbs IMax IMin Int IntPower Ln Log10 Log2 LogN Max Min \
Not_Exist Not_For_All Power Round Sin Sqrt Tan";

char* RDOEditorBaseEdit::kw2 = "no_trace trace trace_all trace_stat trace_tops";

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ¿‡¡·¬‚√„ƒ‰≈Â®∏∆Ê«Á»Ë…È ÍÀÎÃÏÕÌŒÓœÔ–—Ò“Ú”Û‘Ù’ı÷ˆ◊˜ÿ¯Ÿ˘⁄˙€˚‹¸›˝ﬁ˛ﬂˇ";

// ---------------

BEGIN_MESSAGE_MAP( RDOEditorBaseEdit, RDOBaseEdit )
	//{{AFX_MSG_MAP(RDOEditorBaseEdit)
	ON_WM_CREATE()
	ON_COMMAND(ID_GOTONEXT, OnGotoNext)
	ON_UPDATE_COMMAND_UI(ID_GOTONEXT, OnUpdateGotoNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOEditorBaseEdit::RDOEditorBaseEdit():
	RDOBaseEdit(),
	log( NULL )
{
}

RDOEditorBaseEdit::~RDOEditorBaseEdit()
{
}

int RDOEditorBaseEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOBaseEdit::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETLEXER, SCLEX_RDO );
	int lexLanguage = sendEditor( SCI_GETLEXER );
	sendEditor( SCI_SETSTYLEBITS, 5 );
	sendEditorString( SCI_SETKEYWORDS, 0, kw0 );
	sendEditorString( SCI_SETKEYWORDS, 1, kw1 );
	sendEditorString( SCI_SETKEYWORDS, 2, kw2 );
	sendEditorString( SCI_SETWORDCHARS, 0, wordCharacters );

	return 0;
}

void RDOEditorBaseEdit::setEditorStyle( RDOEditorBaseEditStyle* _style )
{
	RDOBaseEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Colors
	sendEditor( SCI_STYLESETFORE, SCE_RDO_DEFAULT, static_cast<RDOEditorEditTheme*>(style->theme)->defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_DEFAULT, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_IDENTIFIER, static_cast<RDOEditorEditTheme*>(style->theme)->identifierColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_IDENTIFIER, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_KEYWORD, static_cast<RDOEditorEditTheme*>(style->theme)->keywordColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_KEYWORD, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_FUNCTION, static_cast<RDOEditorEditTheme*>(style->theme)->functionsColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_FUNCTION, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_TRACE, static_cast<RDOEditorEditTheme*>(style->theme)->traceColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_TRACE, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_COMMENT, static_cast<RDOEditorEditTheme*>(style->theme)->commentColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_COMMENT, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_NUMBER, static_cast<RDOEditorEditTheme*>(style->theme)->numberColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_NUMBER, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_STRING, static_cast<RDOEditorEditTheme*>(style->theme)->stringColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_STRING, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_OPERATOR, static_cast<RDOEditorEditTheme*>(style->theme)->operatorColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_OPERATOR, static_cast<RDOEditorEditTheme*>(style->theme)->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_DEFAULT, static_cast<RDOEditorEditTheme*>(style->theme)->defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_DEFAULT, static_cast<RDOEditorEditTheme*>(style->theme)->defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_DEFAULT, static_cast<RDOEditorEditTheme*>(style->theme)->defaultStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_IDENTIFIER, static_cast<RDOEditorEditTheme*>(style->theme)->identifierStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_IDENTIFIER, static_cast<RDOEditorEditTheme*>(style->theme)->identifierStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_IDENTIFIER, static_cast<RDOEditorEditTheme*>(style->theme)->identifierStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_KEYWORD, static_cast<RDOEditorEditTheme*>(style->theme)->keywordStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_KEYWORD, static_cast<RDOEditorEditTheme*>(style->theme)->keywordStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_KEYWORD, static_cast<RDOEditorEditTheme*>(style->theme)->keywordStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_FUNCTION, static_cast<RDOEditorEditTheme*>(style->theme)->functionsStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_FUNCTION, static_cast<RDOEditorEditTheme*>(style->theme)->functionsStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_FUNCTION, static_cast<RDOEditorEditTheme*>(style->theme)->functionsStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_TRACE, static_cast<RDOEditorEditTheme*>(style->theme)->traceStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_TRACE, static_cast<RDOEditorEditTheme*>(style->theme)->traceStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_TRACE, static_cast<RDOEditorEditTheme*>(style->theme)->traceStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_COMMENT, static_cast<RDOEditorEditTheme*>(style->theme)->commentStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_COMMENT, static_cast<RDOEditorEditTheme*>(style->theme)->commentStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_COMMENT, static_cast<RDOEditorEditTheme*>(style->theme)->commentStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_NUMBER, static_cast<RDOEditorEditTheme*>(style->theme)->numberStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_NUMBER, static_cast<RDOEditorEditTheme*>(style->theme)->numberStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_NUMBER, static_cast<RDOEditorEditTheme*>(style->theme)->numberStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_STRING, static_cast<RDOEditorEditTheme*>(style->theme)->stringStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_STRING, static_cast<RDOEditorEditTheme*>(style->theme)->stringStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_STRING, static_cast<RDOEditorEditTheme*>(style->theme)->stringStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_OPERATOR, static_cast<RDOEditorEditTheme*>(style->theme)->operatorStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_OPERATOR, static_cast<RDOEditorEditTheme*>(style->theme)->operatorStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_OPERATOR, static_cast<RDOEditorEditTheme*>(style->theme)->operatorStyle & RDOFS_UNDERLINE );

	// ----------
	// Font Name
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_DEFAULT, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_IDENTIFIER, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_KEYWORD, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_FUNCTION, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_TRACE, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_COMMENT, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_NUMBER, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_STRING, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_OPERATOR, style->font->name.c_str() );

	// ----------
	// Font Size
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_DEFAULT, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_IDENTIFIER, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_KEYWORD, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_FUNCTION, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_TRACE, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_COMMENT, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_NUMBER, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_STRING, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_OPERATOR, style->font->size );

	// ----------
	// Codepage and Characterset
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_DEFAULT, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_IDENTIFIER, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_KEYWORD, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_FUNCTION, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_TRACE, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_COMMENT, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_NUMBER, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_STRING, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_OPERATOR, style->font->characterSet );
}

void RDOEditorBaseEdit::setLog( RDOLogEdit& _log )
{
	log = &_log;
}

void RDOEditorBaseEdit::OnGotoNext()
{
	if ( log ) {
		log->gotoNext();
	}
}

void RDOEditorBaseEdit::OnUpdateGotoNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( log ? true : false );
}

void RDOEditorBaseEdit::replaceCurrent( const string str, const int changePosValue ) const
{
	int pos;
	if ( changePosValue != -1 ) pos = getCurrentPos();

	sendEditor( SCI_REPLACESEL, 0, (long)str.c_str() );

	if ( changePosValue != -1 ) {
		pos += changePosValue;
		setCurrentPos( pos );
	}
}

/*
void RDOEditorBaseEdit::OnHelpKeyword()
{
	CString filename; // = rdoEditorApp.getFullHelpFileName( "RAO-language.chm" );
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

//	HH_AKLINK link;
//	::ZeroMemory( &link, sizeof( HH_AKLINK ) );
//	link.cbStruct     = sizeof( HH_AKLINK );
//	link.fIndexOnFail = TRUE;
//	link.pszKeywords  = keyword;

//	::HtmlHelp( ::GetDesktopWindow(), filename, HH_KEYWORD_LOOKUP, (DWORD)&link );
}
*/
