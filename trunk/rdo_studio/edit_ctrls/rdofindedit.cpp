#include "stdafx.h"
#include "rdofindedit.h"
#include "rdofindeditstyle.h"
#include "sci/SciLexer.h"
#include "sci/LexFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoBaseEdit;

static char* wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÀàÁáÂâÃãÄäÅå¨¸ÆæÇçÈèÉéÊêËëÌìÍíÎîÏïÐðÑñÒòÓóÔôÕõÖö×÷ØøÙùÚúÛûÜüÝýÞþßÿ";

// ----------------------------------------------------------------------------
// ---------- RDOFindEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOFindEdit, RDOLogEdit )
	//{{AFX_MSG_MAP(RDOFindEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOFindEdit::RDOFindEdit():
	RDOLogEdit(),
	keyword( "" )
{
}

RDOFindEdit::~RDOFindEdit()
{
}

int RDOFindEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOLogEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETLEXER, SCLEX_FIND );
	int lexLanguage = sendEditor( SCI_GETLEXER );
	sendEditor( SCI_SETSTYLEBITS, 5 );
	sendEditorString( SCI_SETWORDCHARS, 0, wordCharacters );

	return 0;
}

void RDOFindEdit::setEditorStyle( rdoBaseEdit::RDOBaseEditStyle* style )
{
	RDOLogEdit::setEditorStyle( style );

	// ----------
	// Colors
	sendEditor( SCI_STYLESETFORE, SCE_RDO_DEFAULT, ((RDOFindEditTheme*)style->theme)->defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_DEFAULT, ((RDOFindEditTheme*)style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_IDENTIFIER, ((RDOFindEditTheme*)style->theme)->identifierColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_IDENTIFIER, ((RDOFindEditTheme*)style->theme)->backgroundColor );
	sendEditor( SCI_STYLESETFORE, SCE_RDO_KEYWORD, ((RDOFindEditTheme*)style->theme)->keywordColor );
	sendEditor( SCI_STYLESETBACK, SCE_RDO_KEYWORD, ((RDOFindEditTheme*)style->theme)->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_DEFAULT, ((RDOFindEditTheme*)style->theme)->defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_DEFAULT, ((RDOFindEditTheme*)style->theme)->defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_DEFAULT, ((RDOFindEditTheme*)style->theme)->defaultStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_IDENTIFIER, ((RDOFindEditTheme*)style->theme)->identifierStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_IDENTIFIER, ((RDOFindEditTheme*)style->theme)->identifierStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_IDENTIFIER, ((RDOFindEditTheme*)style->theme)->identifierStyle & RDOFS_UNDERLINE );
	sendEditor( SCI_STYLESETBOLD     , SCE_RDO_KEYWORD, ((RDOFindEditTheme*)style->theme)->keywordStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_RDO_KEYWORD, ((RDOFindEditTheme*)style->theme)->keywordStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_RDO_KEYWORD, ((RDOFindEditTheme*)style->theme)->keywordStyle & RDOFS_UNDERLINE );

	// ----------
	// Font Name
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_DEFAULT, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_IDENTIFIER, style->font->name.c_str() );
	sendEditorString( SCI_STYLESETFONT, SCE_RDO_KEYWORD, style->font->name.c_str() );

	// ----------
	// Font Size
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_DEFAULT, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_IDENTIFIER, style->font->size );
	sendEditor( SCI_STYLESETSIZE, SCE_RDO_KEYWORD, style->font->size );

	// ----------
	// Codepage and Characterset
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_DEFAULT, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_IDENTIFIER, style->font->characterSet );
	sendEditor( SCI_STYLESETCHARACTERSET, SCE_RDO_KEYWORD, style->font->characterSet );
}

void RDOFindEdit::setKeyword( const string& _keyword )
{
	keyword = _keyword;
	sendEditorString( SCI_SETKEYWORDS, RDO_FINDLEXER_KEYWORDSINDEX, keyword.c_str() );
}
