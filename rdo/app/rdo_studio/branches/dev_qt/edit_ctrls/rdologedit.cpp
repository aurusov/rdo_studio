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
#include "simulator/report//error_code.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/sci/Scintilla.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#	define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOLogEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOLogEdit, RDOBaseEdit )
	ON_WM_CREATE()
	ON_COMMAND(ID_BUILDFINDLOG_GOTO_NEXT, OnGotoNext)
	ON_COMMAND(ID_BUILDFINDLOG_GOTO_PREV, OnGotoPrev)
END_MESSAGE_MAP()

RDOLogEdit::RDOLogEdit()
	: RDOBaseEdit  ()
	, m_currentLine(-1)
{
	setCurrentLine(-1);
	m_sciMarkerLine = getNewMarker();
}

RDOLogEdit::~RDOLogEdit()
{
	clearLines();
}

void RDOLogEdit::setEditorStyle( RDOLogEditStyle* style )
{
	RDOBaseEdit::setEditorStyle( style );
	if ( !style ) 
	{
		return;
	}

	// ----------
	// Selected Line
	defineMarker( m_sciMarkerLine, SC_MARK_BACKGROUND, RGB( 0xFF, 0xFF, 0xFF ), static_cast<RDOLogEditTheme*>(style->theme)->selectLineBgColor );
}

void RDOLogEdit::gotoPrev()
{
	m_currentLine--;
	if ( m_currentLine < 0 ) {
		m_currentLine = m_lines.size() - 1;
	}
	if ( m_currentLine < 0 ) return;

	RDOLogEditLineInfoList::iterator it = m_lines.begin();
	int i;
	for ( i = 0; i < m_currentLine; i++ ) {
		it++;
	}
	while ( it != m_lines.begin() && (*it)->getLineNumber() == -1 ) {
		it--;
		m_currentLine--;
	}
	if ( it == m_lines.begin() && (*it)->getLineNumber() == -1 ) {
		it = m_lines.end();
		m_currentLine = m_lines.size();
		while ( it == m_lines.end() || (it != m_lines.begin() && (*it)->getLineNumber() == -1) ) {
			it--;
			m_currentLine--;
		}
	}
	if ( it != m_lines.end() && (*it)->getLineNumber() != -1 ) {
		setSelectLine( m_currentLine, *it, true );
	}
}

void RDOLogEdit::getLines( REF(RDOLogEditLineInfoList) lines ) const
{
	lines = m_lines;
}

rsint RDOLogEdit::getCurrentLine() const
{
	return m_currentLine;
}

rsint RDOLogEdit::getSciMarkerLine() const
{
	return m_sciMarkerLine;
}

void RDOLogEdit::setCurrentLine(rsint currentLine)
{
	m_currentLine = currentLine;
}

void RDOLogEdit::setSciMarkerLine( rsint sciMarkerLine )
{
	m_sciMarkerLine = sciMarkerLine;
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
	}
	else
	{
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

void RDOLogEdit::setSelectLine()
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	int pos  = sendEditor( SCI_POSITIONFROMPOINT, point.x, point.y );
	int line = getLineFromPosition( pos );
	setCurrentPos( pos );
	m_currentLine = line;

	std::list< RDOLogEditLineInfo* >::iterator it = m_lines.begin();
	for ( int i = 0; i < line; i++ ) {
		if ( it != m_lines.end() ) {
			it++;
		}
	}
	if ( it != m_lines.end() && (*it)->getLineNumber() != -1 ) {
		setSelectLine( line, *it );
	}
}

void RDOLogEdit::gotoNext()
{
	m_currentLine++;
	std::list< RDOLogEditLineInfo* >::iterator it = m_lines.begin();
	int i;
	for ( i = 0; i < m_currentLine; i++ ) {
		if ( it != m_lines.end() ) {
			it++;
		} else {
			m_currentLine = 0;
			break;
		}
	}
	it = m_lines.begin();
	for ( i = 0; i < m_currentLine; i++ ) {
		it++;
	}
	while ( it != m_lines.end() && (*it)->getLineNumber() == -1 ) {
		it++;
		m_currentLine++;
	}
	if ( it == m_lines.end() ) {
		it = m_lines.begin();
		m_currentLine = 0;
		while ( it != m_lines.end() && (*it)->getLineNumber() == -1 ) {
			it++;
			m_currentLine++;
		}
	}
	if ( it != m_lines.end() && (*it)->getLineNumber() != -1 ) {
		setSelectLine( m_currentLine, *it, true );
	}
}

void RDOLogEdit::clearAll()
{
	RDOBaseEdit::clearAll();
	clearLines();
}

void RDOLogEdit::appendLine( RDOLogEditLineInfo* line )
{
	m_lines.push_back( line );
	rbool readOnly = isReadOnly();
	if ( readOnly )
	{
		setReadOnly( false );
	}
	tstring str = line->getMessage();
	rdo::trimRight( str );
	str += "\r\n";
	setCurrentPos( getLength() );
	appendText( str );
	line->setPosInLog( getLength() );
	scrollToLine2( getLineCount() );
	setCurrentPos( line->getPosInLog() );
	if ( readOnly )
	{
		setReadOnly( true );
	}
	updateEditGUI();
}

void RDOLogEdit::OnGotoNext()
{
	gotoNext();
}

void RDOLogEdit::OnGotoPrev()
{
	gotoPrev();
}

void RDOLogEdit::setSelectLine( const int line, const RDOLogEditLineInfo* lineInfo, const rbool useScroll )
{
	if ( lineInfo->getLineNumber() != -1 ) {
		if ( sendEditor( SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine ) != line ) {
			clearSelectLine();
			sendEditor( SCI_MARKERADD, line, m_sciMarkerLine );
			if ( useScroll ) {
				setCurrentPos( lineInfo->getPosInLog() );
				scrollToCarret();
			}
		}
		rdoEditor::RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			if ( tab->getCurrentRDOItem() != lineInfo->getFileType() ) {
				rdoEditor::RDOEditorEdit* edit = tab->getCurrentEdit();
				if ( !edit || (edit && edit->getLog() == this) ) {
					tab->setCurrentRDOItem( lineInfo->getFileType() );
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
	edit->scrollToLine( lineInfo->getLineNumber() );
	int pos = edit->getPositionFromLine(lineInfo->getLineNumber()) + lineInfo->getPosInLine();
	edit->setCurrentPos( pos );
	edit->horzScrollToCurrentPos();
	edit->SetFocus();
}

void RDOLogEdit::clearSelectLine()
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine );
	if ( nextLine >= 0 ) {
		sendEditor( SCI_MARKERDELETE, nextLine, m_sciMarkerLine );
		RedrawWindow();
	}
}

rbool RDOLogEdit::hasSelectLine() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << m_sciMarkerLine );
	return nextLine >= 0;
}

void RDOLogEdit::clearLines()
{
	std::list< RDOLogEditLineInfo* >::iterator it = m_lines.begin();
	while ( it != m_lines.end() ) {
		delete *it;
		it++;
	}
	m_lines.clear();
	m_currentLine = 0;
}