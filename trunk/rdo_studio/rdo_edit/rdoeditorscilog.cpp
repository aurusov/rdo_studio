#include "stdafx.h"
#include "rdoeditorscilog.h"
#include "../rdostudioapp.h"
#include "../rdostudiomodel.h"
#include "rdoeditortabctrl.h"
#include "sci/LexRdo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLogLineInfo
// ----------------------------------------------------------------------------
RDOEditorSciLogLineInfo::RDOEditorSciLogLineInfo( const string& _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber ):
	fileType( _fileType ),
	lineNumber( _lineNumber ),
	message( _message )
{
}

RDOEditorSciLogLineInfo::~RDOEditorSciLogLineInfo()
{
}

string RDOEditorSciLogLineInfo::getMessage() const
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
	if ( file.empty() || lineNumber == -1 ) {
		return message;
	} else {
		return format( "%s (%d): %s", file.c_str(), lineNumber + 1, message.c_str() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLog
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorSciLog, RDOEditorSciEdit )
	//{{AFX_MSG_MAP(RDOEditorSciLog)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOEditorSciLog::RDOEditorSciLog(): RDOEditorSciEdit()
{
	sci_MARKER_LINE = getNewMarker();
}

RDOEditorSciLog::~RDOEditorSciLog()
{
}

BOOL RDOEditorSciLog::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	if ( !RDOEditorSciEdit::OnNotify( wParam, lParam, pResult ) ) {

		SCNotification* scn = (SCNotification*)lParam;
		if ( scn->nmhdr.hwndFrom == sciHWND ) {
			switch( scn->nmhdr.code ) {
				case SCN_RDO_AFTERDBLCLICK: {
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

int RDOEditorSciLog::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOEditorSciEdit::OnCreate(lpCreateStruct) == -1 ) return -1;
	return 0;
}

void RDOEditorSciLog::setEditorStyle( RDOEditorSciEditStyle* style )
{
	RDOEditorSciEdit::setEditorStyle( style );

	// ----------
	// Selected Line
	defineMarker( sci_MARKER_LINE, SC_MARK_BACKGROUND, RGB( 0xFF, 0xFF, 0xFF ), ((RDOEditorSciLogTheme*)style->theme)->selectLineBgColor );
}

void RDOEditorSciLog::setSelectLine()
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	int pos  = sendEditor( SCI_POSITIONFROMPOINT, point.x, point.y );
	int line = sendEditor( SCI_LINEFROMPOSITION, pos );
	setCurrentPos( pos );
	if ( sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE ) != line ) {
		clearSelectLine();
		sendEditor( SCI_MARKERADD, line, sci_MARKER_LINE );
	}
	list< RDOEditorSciLogLineInfo >::iterator it = lines.begin();
	for ( int i = 0; i < line; i++ ) {
		if ( it != lines.end() ) {
			it++;
		}
	}
	if ( it != lines.end() && (*it).lineNumber != -1 ) {
		RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			RDOEditorTabItem tabItem;
			switch ( (*it).fileType ) {
				case rdoModelObjects::PAT: tabItem = RDOEDIT_PAT; break;
				case rdoModelObjects::RTP: tabItem = RDOEDIT_RTP; break;
				case rdoModelObjects::RSS: tabItem = RDOEDIT_RSS; break;
				case rdoModelObjects::OPR: tabItem = RDOEDIT_OPR; break;
				case rdoModelObjects::FRM: tabItem = RDOEDIT_FRM; break;
				case rdoModelObjects::FUN: tabItem = RDOEDIT_FUN; break;
				case rdoModelObjects::DPT: tabItem = RDOEDIT_DPT; break;
				case rdoModelObjects::SMR: tabItem = RDOEDIT_SMR; break;
				case rdoModelObjects::PMD: tabItem = RDOEDIT_PMD; break;
				default: tabItem = RDOEDIT_PAT;
			}
			if ( tab->getCurrentRDOItem() != tabItem ) {
				tab->setCurrentRDOItem( tabItem );
			}
			RDOEditorSciEdit* edit = tab->getCurrentEdit();
			if ( edit ) {
				edit->scrollToLine( (*it).lineNumber, edit->getPositionFromLine( (*it).lineNumber ) );
				edit->SetFocus();
			}
		}
	}
}

void RDOEditorSciLog::clearSelectLine()
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE );
	if ( nextLine >= 0 ) {
		sendEditor( SCI_MARKERDELETE, nextLine, sci_MARKER_LINE );
		RedrawWindow();
	}
}

bool RDOEditorSciLog::hasSelectLine() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE );
	return nextLine >= 0;
}

void RDOEditorSciLog::appendLine( const RDOEditorSciLogLineInfo& line )
{
	lines.push_back( line );
	bool readOnly = isReadOnly();
	if ( readOnly ) {
		setReadOnly( false );
	}
	bool scroll = isLineVisible( getLineCount() - 1 );
	appendText( line.getMessage() );
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
