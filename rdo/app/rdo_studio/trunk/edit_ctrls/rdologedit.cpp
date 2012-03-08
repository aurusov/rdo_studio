/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/sci/Scintilla.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditLineInfo
// --------------------------------------------------------------------------------
RDOLogEditLineInfo::RDOLogEditLineInfo( const std::string& _message, const rdoModelObjects::RDOFileType _fileType, const int _lineNumber, const int _posInLine ):
	fileType( _fileType ),
	lineNumber( _lineNumber ),
	posInLine( _posInLine ),
	message( _message ),
	posInLog( 0 )
{
}

RDOLogEditLineInfo::~RDOLogEditLineInfo()
{
}

std::string RDOLogEditLineInfo::getMessage() const
{
	std::string file;
	switch ( fileType ) {
		case rdoModelObjects::RTP : file = "RTP" ; break;
		case rdoModelObjects::RSS : file = "RSS" ; break;
		case rdoModelObjects::EVN : file = "EVN" ; break;
		case rdoModelObjects::PAT : file = "PAT" ; break;
		case rdoModelObjects::DPT : file = "DPT" ; break;
		case rdoModelObjects::PRC : file = "PRC" ; break;
		case rdoModelObjects::PRCX: file = "PRCX"; break;
		case rdoModelObjects::FRM : file = "FRM" ; break;
		case rdoModelObjects::FUN : file = "FUN" ; break;
		case rdoModelObjects::SMR : file = "SMR" ; break;
		case rdoModelObjects::PMD : file = "PMD" ; break;
		default: file = "";
	}
	if ( lineNumber < 0 || file.empty() ) {
		return message;
	} else {
		return rdo::format( "%s (%d): %s", file.c_str(), lineNumber + 1, message.c_str() );
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOLogEdit, RDOBaseEdit )
	ON_WM_CREATE()
	ON_COMMAND(ID_BUILDFINDLOG_GOTO_NEXT, OnGotoNext)
	ON_COMMAND(ID_BUILDFINDLOG_GOTO_PREV, OnGotoPrev)
END_MESSAGE_MAP()

RDOLogEdit::RDOLogEdit():
	RDOBaseEdit(),
	current_line( -1 )
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
	if ( RDOBaseEdit::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT );

	setReadOnly( true );

	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>("withoutselectbyclick"), "1" );

	return 0;
}

void RDOLogEdit::setEditorStyle( RDOLogEditStyle* _style )
{
	RDOBaseEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Selected Line
	defineMarker( sci_MARKER_LINE, SC_MARK_BACKGROUND, RGB( 0xFF, 0xFF, 0xFF ), static_cast<RDOLogEditTheme*>(style->theme)->selectLineBgColor );
}

void RDOLogEdit::setSelectLine()
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	int pos  = sendEditor( SCI_POSITIONFROMPOINT, point.x, point.y );
	int line = getLineFromPosition( pos );
	setCurrentPos( pos );
	current_line = line;

	std::list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	for ( int i = 0; i < line; i++ ) {
		if ( it != lines.end() ) {
			it++;
		}
	}
	if ( it != lines.end() && (*it)->lineNumber != -1 ) {
		setSelectLine( line, *it );
	}
}

void RDOLogEdit::gotoNext()
{
	current_line++;
	std::list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	int i;
	for ( i = 0; i < current_line; i++ ) {
		if ( it != lines.end() ) {
			it++;
		} else {
			current_line = 0;
			break;
		}
	}
	it = lines.begin();
	for ( i = 0; i < current_line; i++ ) {
		it++;
	}
	while ( it != lines.end() && (*it)->lineNumber == -1 ) {
		it++;
		current_line++;
	}
	if ( it == lines.end() ) {
		it = lines.begin();
		current_line = 0;
		while ( it != lines.end() && (*it)->lineNumber == -1 ) {
			it++;
			current_line++;
		}
	}
	if ( it != lines.end() && (*it)->lineNumber != -1 ) {
		setSelectLine( current_line, *it, true );
	}
}

void RDOLogEdit::gotoPrev()
{
	current_line--;
	if ( current_line < 0 ) {
		current_line = lines.size() - 1;
	}
	if ( current_line < 0 ) return;

	std::list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	int i;
	for ( i = 0; i < current_line; i++ ) {
		it++;
	}
	while ( it != lines.begin() && (*it)->lineNumber == -1 ) {
		it--;
		current_line--;
	}
	if ( it == lines.begin() && (*it)->lineNumber == -1 ) {
		it = lines.end();
		current_line = lines.size();
		while ( it == lines.end() || (it != lines.begin() && (*it)->lineNumber == -1) ) {
			it--;
			current_line--;
		}
	}
	if ( it != lines.end() && (*it)->lineNumber != -1 ) {
		setSelectLine( current_line, *it, true );
	}
}

void RDOLogEdit::OnGotoNext()
{
	gotoNext();
}

void RDOLogEdit::OnGotoPrev()
{
	gotoPrev();
}

void RDOLogEdit::setSelectLine( const int line, const RDOLogEditLineInfo* lineInfo, const bool useScroll )
{
	if ( lineInfo->lineNumber != -1 ) {
		if ( sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_LINE ) != line ) {
			clearSelectLine();
			sendEditor( SCI_MARKERADD, line, sci_MARKER_LINE );
			if ( useScroll ) {
				setCurrentPos( lineInfo->posInLog );
				scrollToCarret();
			}
		}
		rdoEditor::RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			if ( tab->getCurrentRDOItem() != lineInfo->fileType ) {
				rdoEditor::RDOEditorEdit* edit = tab->getCurrentEdit();
				if ( !edit || (edit && edit->getLog() == this) ) {
					tab->setCurrentRDOItem( lineInfo->fileType );
				}
			}
			rdoEditor::RDOEditorEdit* edit = tab->getCurrentEdit();
			if ( edit && edit->getLog() == this ) {
				updateEdit( edit, lineInfo );
			}
		}
	}
}

void RDOLogEdit::updateEdit( rdoEditor::RDOEditorEdit* edit, const RDOLogEditLineInfo* lineInfo )
{
	edit->scrollToLine( lineInfo->lineNumber );
	int pos = edit->getPositionFromLine(lineInfo->lineNumber) + lineInfo->posInLine;
	edit->setCurrentPos( pos );
	edit->horzScrollToCurrentPos();
	edit->SetFocus();
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
	std::list< RDOLogEditLineInfo* >::iterator it = lines.begin();
	while ( it != lines.end() ) {
		delete *it;
		it++;
	}
	lines.clear();
	current_line = 0;
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
	std::string str = line->getMessage();
	rdo::trimRight( str );
	str += "\r\n";
	setCurrentPos( getLength() );
	appendText( str );
	line->posInLog = getLength();
	scrollToLine2( getLineCount() );
	setCurrentPos( line->posInLog );
	if ( readOnly ) {
		setReadOnly( true );
	}
	updateEditGUI();
}
