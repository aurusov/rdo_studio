#include "stdafx.h"
#include "rdologedit.h"
#include "../rdostudioapp.h"
#include "../rdostudiomodel.h"
#include "../rdo_edit/rdoeditortabctrl.h"
#include "sci/Scintilla.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoBaseEdit;

// ----------------------------------------------------------------------------
// ---------- RDOLogEditLineInfo
// ----------------------------------------------------------------------------
RDOLogEditLineInfo::RDOLogEditLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber, const int _posInLine ):
	fileType( _fileType ),
	lineNumber( _lineNumber ),
	posInLine( _posInLine ),
	message( _message )
{
}

RDOLogEditLineInfo::~RDOLogEditLineInfo()
{
}

string RDOLogEditLineInfo::getMessage() const
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
		return format( "%s (%d): %s", file.c_str(), lineNumber + 1, message.c_str() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOLogEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOLogEdit, RDOBaseEdit )
	//{{AFX_MSG_MAP(RDOLogEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOLogEdit::RDOLogEdit(): RDOBaseEdit()
{
	sci_MARKER_LINE = getNewMarker();
}

RDOLogEdit::~RDOLogEdit()
{
	clearLines();
}

BOOL RDOLogEdit::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( !RDOBaseEdit::OnNotify( wParam, lParam, pResult ) ) {

		SCNotification* scn = (SCNotification*)lParam;
		if ( scn->nmhdr.hwndFrom == sciHWND ) {
			switch( scn->nmhdr.code ) {
				case SCN_DOUBLECLICK: {
					setSelectLine();
					return TRUE;
				}
				case SCN_MODIFIED: {
					if ( hasSelectLine() ) clearSelectLine();
					return TRUE;
				}
			}
		}
	} else {
		return TRUE;
	}
	return FALSE;
}

int RDOLogEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOBaseEdit ::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("withoutselectbyclick"), "1" );

	return 0;
}

void RDOLogEdit::setEditorStyle( RDOLogEditStyle* _style )
{
	RDOBaseEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Selected Line
	defineMarker( sci_MARKER_LINE, SC_MARK_BACKGROUND, RGB( 0xFF, 0xFF, 0xFF ), ((RDOLogEditTheme*)style->theme)->selectLineBgColor );
}

void RDOLogEdit::setSelectLine()
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	int pos  = sendEditor( SCI_POSITIONFROMPOINT, point.x, point.y );
	int line = getLineFromPosition( pos );
	setCurrentPos( pos );

	list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	for ( int i = 0; i < line; i++ ) {
		if ( it != lines.end() ) {
			it++;
		}
	}
	if ( it != lines.end() && (*it)->lineNumber != -1 ) {
		if ( sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE ) != line ) {
			clearSelectLine();
			sendEditor( SCI_MARKERADD, line, sci_MARKER_LINE );
		}
		rdoEditor::RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			rdoEditor::RDOEditorTabItem tabItem;
			switch ( (*it)->fileType ) {
				case rdoModelObjects::PAT: tabItem = rdoEditor::RDOEDIT_PAT; break;
				case rdoModelObjects::RTP: tabItem = rdoEditor::RDOEDIT_RTP; break;
				case rdoModelObjects::RSS: tabItem = rdoEditor::RDOEDIT_RSS; break;
				case rdoModelObjects::OPR: tabItem = rdoEditor::RDOEDIT_OPR; break;
				case rdoModelObjects::FRM: tabItem = rdoEditor::RDOEDIT_FRM; break;
				case rdoModelObjects::FUN: tabItem = rdoEditor::RDOEDIT_FUN; break;
				case rdoModelObjects::DPT: tabItem = rdoEditor::RDOEDIT_DPT; break;
				case rdoModelObjects::SMR: tabItem = rdoEditor::RDOEDIT_SMR; break;
				case rdoModelObjects::PMD: tabItem = rdoEditor::RDOEDIT_PMD; break;
				default: tabItem = rdoEditor::RDOEDIT_PAT;
			}
			if ( tab->getCurrentRDOItem() != tabItem ) {
				tab->setCurrentRDOItem( tabItem );
			}
			rdoBaseEdit::RDOBaseEdit* edit = tab->getCurrentEdit();
			if ( edit ) {
				edit->scrollToLine( (*it)->lineNumber );
				int pos = edit->getPositionFromLine((*it)->lineNumber) + (*it)->posInLine;
				edit->setCurrentPos( pos );
				edit->horzScrollToCurrentPos();
				edit->SetFocus();
			}
		}
	}
}

void RDOLogEdit::clearSelectLine()
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE );
	if ( nextLine >= 0 ) {
		sendEditor( SCI_MARKERDELETE, nextLine, sci_MARKER_LINE );
		RedrawWindow();
	}
}

bool RDOLogEdit::hasSelectLine() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE );
	return nextLine >= 0;
}

void RDOLogEdit::clearLines()
{
	list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	while ( it != lines.end() ) {
		delete *it;
		it++;
	}
	lines.clear();
}

void RDOLogEdit::clearAll()
{
	RDOBaseEdit::clearAll();
	clearLines();
}

void RDOLogEdit::appendLine( RDOLogEditLineInfo* line )
{
	lines.push_back( line );
	bool readOnly = isReadOnly();
	if ( readOnly ) {
		setReadOnly( false );
	}
	bool scroll = isLineVisible( getLineCount() - 1 );
	string str = line->getMessage();
	trimRight( str );
	str += "\r\n";
	appendText( str );
	if ( scroll ) {
		int line = getLineCount();
		int line_to_scroll = line > 0 ? line - 1 : 0;
		scrollToLine( line_to_scroll );
		setCurrentPos( getLength() );
	}
	if ( readOnly ) {
		setReadOnly( true );
	}
}
