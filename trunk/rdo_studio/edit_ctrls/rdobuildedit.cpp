#include "stdafx.h"
#include "rdobuildedit.h"
#include "sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOBuildEditLineInfo
// ----------------------------------------------------------------------------
RDOBuildEditLineInfo::RDOBuildEditLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber, bool _error ):
	RDOLogEditLineInfo( _message, _fileType, _lineNumber ),
	error( _error )
{
}

RDOBuildEditLineInfo::~RDOBuildEditLineInfo()
{
}

string RDOBuildEditLineInfo::getMessage() const
{
	string file;
	switch ( fileType ) {
		case rdoModelObjects::PAT: file = "PAT"; break;
		case rdoModelObjects::RTP: file = "RTP"; break;
		case rdoModelObjects::RSS: file = "RSS"; break;
		case rdoModelObjects::OPR: file = "OPR"; break;
		case rdoModelObjects::FRM: file = "FRM"; break;
		case rdoModelObjects::FUN: file = "FUN"; break;
		case rdoModelObjects::DPT: file = "DPT"; break;
		case rdoModelObjects::SMR: file = "SMR"; break;
		case rdoModelObjects::PMD: file = "PMD"; break;
		default: file = "";
	}
	if ( lineNumber == -1 || file.empty() ) {
		return message;
	} else {
		string s = error ? "error" : "warning";
		return format( "%s (%d): %s: %s", file.c_str(), lineNumber + 1, s.c_str(), message.c_str() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOBuildEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOBuildEdit, RDOLogEdit )
	//{{AFX_MSG_MAP(RDOBuildEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOBuildEdit::RDOBuildEdit(): RDOLogEdit()
{
}

RDOBuildEdit::~RDOBuildEdit()
{
}

int RDOBuildEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOLogEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETLEXER, SCLEX_TEXT );
	int lexLanguage = sendEditor( SCI_GETLEXER );

	return 0;
}

void RDOBuildEdit::setEditorStyle( RDOLogEditStyle* _style )
{
	RDOLogEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Colors
	sendEditor( SCI_STYLESETFORE, SCE_TEXT_DEFAULT, style->theme->defaultColor );
	sendEditor( SCI_STYLESETBACK, SCE_TEXT_DEFAULT, style->theme->backgroundColor );

	// ----------
	// Styles
	sendEditor( SCI_STYLESETBOLD     , SCE_TEXT_DEFAULT, style->theme->defaultStyle & RDOFS_BOLD      );
	sendEditor( SCI_STYLESETITALIC   , SCE_TEXT_DEFAULT, style->theme->defaultStyle & RDOFS_ITALIC    );
	sendEditor( SCI_STYLESETUNDERLINE, SCE_TEXT_DEFAULT, style->theme->defaultStyle & RDOFS_UNDERLINE );
}
