/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoredit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/algorithm/string.hpp>
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/edit/view_base.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "thirdparty/sci/rdo/LexRdo.h"
#include "thirdparty/sci/lexlib/WordList.h"
#include "thirdparty/sci/rdo/WordListUtil.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditBufferDlg
// --------------------------------------------------------------------------------
class RDOEditorEditBufferDlg: public CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );

public:
	CString bufName;
	CString bufValue;

	RDOEditorEditBufferDlg( CString _bufName, CString _bufValue ): CDialog( IDD_BUFFER ), bufName( _bufName ), bufValue( _bufValue ) {};
};

void RDOEditorEditBufferDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Text( pDX, IDC_BUFFERVALUESTATIC, bufName );
	DDX_Text( pDX, IDC_BUFFERVALUEEDIT, bufValue );
}

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEdit
// ---------------------------------------------------------------------------

// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP( RDOEditorEdit, RDOEditorBaseEdit )
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COMMENTSELECTION, OnEditCommentSelection)
	ON_COMMAND(ID_EDIT_COMPLETEWORD, OnEditCompleteWord)
	ON_COMMAND(ID_INSERT_BUFFER1_PASTE, OnInsertBuffer1Paste)
	ON_COMMAND(ID_INSERT_BUFFER2_PASTE, OnInsertBuffer2Paste)
	ON_COMMAND(ID_INSERT_BUFFER3_PASTE, OnInsertBuffer3Paste)
	ON_COMMAND(ID_INSERT_BUFFER4_PASTE, OnInsertBuffer4Paste)
	ON_COMMAND(ID_INSERT_BUFFER1_APPEND, OnInsertBuffer1Append)
	ON_COMMAND(ID_INSERT_BUFFER2_APPEND, OnInsertBuffer2Append)
	ON_COMMAND(ID_INSERT_BUFFER3_APPEND, OnInsertBuffer3Append)
	ON_COMMAND(ID_INSERT_BUFFER4_APPEND, OnInsertBuffer4Append)
	ON_COMMAND(ID_INSERT_BUFFER1_EDIT, OnInsertBuffer1Edit)
	ON_COMMAND(ID_INSERT_BUFFER2_EDIT, OnInsertBuffer2Edit)
	ON_COMMAND(ID_INSERT_BUFFER3_EDIT, OnInsertBuffer3Edit)
	ON_COMMAND(ID_INSERT_BUFFER4_EDIT, OnInsertBuffer4Edit)
	ON_COMMAND(ID_INSERT_BUFFER1_CLEAR, OnInsertBuffer1Clear)
	ON_COMMAND(ID_INSERT_BUFFER2_CLEAR, OnInsertBuffer2Clear)
	ON_COMMAND(ID_INSERT_BUFFER3_CLEAR, OnInsertBuffer3Clear)
	ON_COMMAND(ID_INSERT_BUFFER4_CLEAR, OnInsertBuffer4Clear)
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_PASTE, OnUndateBuffer1Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_PASTE, OnUndateBuffer2Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_PASTE, OnUndateBuffer3Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_PASTE, OnUndateBuffer4Paste )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER1_EDIT, OnUpdateInsertBufferEdit)
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_CLEAR, OnUndateBuffer1Clear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_CLEAR, OnUndateBuffer2Clear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_CLEAR, OnUndateBuffer3Clear )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_CLEAR, OnUndateBuffer4Clear )
	ON_COMMAND(ID_BUILDFINDLOG_GOTO_NEXT, OnGotoNext)
	ON_COMMAND(ID_BUILDFINDLOG_GOTO_PREV, OnGotoPrev)
	ON_UPDATE_COMMAND_UI(ID_BUILDFINDLOG_GOTO_NEXT, OnUpdateGotoNext)
	ON_UPDATE_COMMAND_UI(ID_BUILDFINDLOG_GOTO_PREV, OnUpdateGotoPrev)
	ON_COMMAND(ID_VIEW_TOGGLE_ALLFOLDS, OnToggleAllFolds)
	ON_COMMAND(ID_VIEW_TOGGLE_CURRENTFOLD, OnToggleCurrentFold)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLE_ALLFOLDS, OnUpdateFold)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLE_CURRENTFOLD, OnUpdateFold)
	ON_UPDATE_COMMAND_UI( ID_EDIT_COMMENTSELECTION, OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER2_EDIT, OnUpdateInsertBufferEdit)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER3_EDIT, OnUpdateInsertBufferEdit)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER4_EDIT, OnUpdateInsertBufferEdit)
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)

	ON_COMMAND_RANGE( ID_INSERT_PAT_TEMPL_OPERATION, ID_INSERT_ALGO_RETURN, OnInsertCommand )

END_MESSAGE_MAP()

RDOEditorEdit::RDOEditorEdit( RDOStudioEditBaseView* _view ):
	RDOEditorBaseEdit(),
	bufSelStart( -1 ),
	view( _view ),
	log( NULL ),
	canClearErrorLine( true )
{
	sci_FOLDMARGIN_ID = getNewMarker();
	sci_MARKER_ERROR  = getNewMarker();
}

RDOEditorEdit::~RDOEditorEdit()
{
}

BOOL RDOEditorEdit::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
//	TRACE( "N. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );

	SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);

	if ( !RDOEditorBaseEdit::OnNotify( wParam, lParam, pResult ) || ( scn->nmhdr.hwndFrom == sciHWND && scn->nmhdr.code == SCN_CHARADDED ) ) {

		if ( scn->nmhdr.hwndFrom == sciHWND ) {
			switch( scn->nmhdr.code ) {
				case SCN_RDO_BUFFERKEY: {
					if ( view ) {
						switch ( view->currentBuffer ) {
							case 1: view->buf1 += static_cast<char>(scn->ch); break;
							case 2: view->buf2 += static_cast<char>(scn->ch); break;
							case 3: view->buf3 += static_cast<char>(scn->ch); break;
							case 4: view->buf4 += static_cast<char>(scn->ch); break;
						}
					}
					return TRUE;
				}
				case SCN_RDO_POSCHANGED: {
					bufSelStart = -1;
					return TRUE;
				}
				case SCN_RDO_CLICK: {
					bufSelStart = -1;
					return TRUE;
				}
				case SCN_MODIFIED: {
					bufSelStart = -1;
					if ( scn->modificationType & SC_MOD_CHANGEFOLD ) {
						foldChanged( scn->line, scn->foldLevelNow, scn->foldLevelPrev );
					}
					if ( canClearErrorLine && hasErrorLine() ) clearErrorLine();
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
					bufSelStart = -1;
					return TRUE;
				}
			}
		}
	} else {
		return TRUE;
	}
	return FALSE;
}

int RDOEditorEdit::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOEditorBaseEdit::OnCreate(lpCreateStruct) == -1 ) return -1;

	sendEditor( SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT | SC_MOD_CHANGEFOLD );

	sendEditor( SCI_SETMARGINTYPEN     , sci_FOLDMARGIN_ID, SC_MARGIN_SYMBOL );
	sendEditor( SCI_SETFOLDFLAGS, 16 );
	sendEditor( SCI_SETMARGINMASKN     , sci_FOLDMARGIN_ID, SC_MASK_FOLDERS );
	sendEditor( SCI_SETMARGINSENSITIVEN, sci_FOLDMARGIN_ID, 1 );

	sendEditor( SCI_AUTOCSETIGNORECASE    , 1 );
	sendEditor( SCI_AUTOCSETCHOOSESINGLE  , 0 );
	sendEditor( SCI_AUTOCSETDROPRESTOFWORD, true );
	sendEditor( SCI_AUTOCSETCANCELATSTART , false );
	sendEditor( SCI_AUTOCSETAUTOHIDE      , false );
	sendEditor( SCI_AUTOCSTOPS            , 0, reinterpret_cast<long>("+-*/:[](),<>=.") );

	return 0;
}

void RDOEditorEdit::setEditorStyle( RDOEditorEditStyle* _style )
{
	RDOEditorBaseEdit::setEditorStyle( _style );
	if ( !style ) return;

	// ----------
	// Fold
	RDOEditorEditTheme* theme = static_cast<RDOEditorEditTheme*>(style->theme);
	COLORREF foldFgColor = theme->foldFgColor;
	COLORREF foldBgColor = theme->foldBgColor;
	switch ( theme->foldStyle ) {
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
	tstring cf_prop( "CommentFold" );
	tstring cf_val_1( "1" );
	tstring cf_val_0( "0" );
	sendEditorString( SCI_SETPROPERTY, reinterpret_cast<unsigned long>(cf_prop.c_str()), theme->commentFold ? cf_val_1.c_str() : cf_val_0.c_str() );
	sendEditor( SCI_COLOURISE, 0, -1 );

	// ----------
	// Margin
	sendEditor( SCI_SETMARGINWIDTHN, 2, static_cast<RDOEditorEditStyle*>(style)->margin->fold ? 16 : 0 );
	sendEditor( SCI_SETMARGINWIDTHN, 1, static_cast<RDOEditorEditStyle*>(style)->margin->bookmark ? 16 : 0 );
	sendEditor( SCI_SETMARGINWIDTHN, 0, static_cast<RDOEditorEditStyle*>(style)->margin->lineNumber ? 40 : 0 );

	// ----------
	// Error
	defineMarker( sci_MARKER_ERROR, SC_MARK_BACKGROUND, RGB( 0xFF, 0xFF, 0xFF ), static_cast<RDOEditorEditTheme*>(style->theme)->errorBgColor );
}

void RDOEditorEdit::OnEditCommentSelection() 
{
	commentSelection();
}

void RDOEditorEdit::OnEditCompleteWord() 
{
	completeWord();
}

void RDOEditorEdit::expand( int& line, rbool doExpand, rbool force, int visLevels, int level ) const
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

void RDOEditorEdit::toggleCurrentFold() const
{
	sendEditor( SCI_TOGGLEFOLD, getCurrentLineNumber() );
}

void RDOEditorEdit::toggleAllFolds() const
{
	sendEditor( SCI_COLOURISE, 0, -1 );
	int maxLine = getLineCount();
	rbool expanding = true;
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

void RDOEditorEdit::foldMarginClick( int position, int modifiers ) const
{
	int lineClick = getLineFromPosition( position );
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

void RDOEditorEdit::OnToggleCurrentFold()
{
	toggleCurrentFold();
}

void RDOEditorEdit::OnToggleAllFolds()
{
	toggleAllFolds();
}

void RDOEditorEdit::OnUpdateFold(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( !GUI_IS_EMPTY );
}

void RDOEditorEdit::commentSelection() const
{
	if ( GUI_IS_SELECTED ) {
		tstring startComment( "/*" );
		tstring endComment( "*/" );
		int startCommentLength = startComment.length();
		CharacterRange cr = getSelectionRange();
		int caretPosition = getCurrentPos();
		rbool moveCaret = caretPosition < cr.cpMax;
		sendEditor( SCI_BEGINUNDOACTION );
		sendEditorString( SCI_INSERTTEXT, cr.cpMin, startComment.c_str() );
		cr.cpMax += startCommentLength;
		cr.cpMin += startCommentLength;
		sendEditorString( SCI_INSERTTEXT, cr.cpMax, endComment.c_str() );
		if ( moveCaret ) {
			sendEditor( SCI_GOTOPOS      , cr.cpMax );
			sendEditor( SCI_SETCURRENTPOS, cr.cpMin );
		} else {
			setSelection( cr.cpMin, cr.cpMax );
		}
		sendEditor( SCI_ENDUNDOACTION );
	}
}

void RDOEditorEdit::completeWord()
{
	if (!static_cast<RDOEditorEditStyle*>(style)->autoComplete->useAutoComplete)
		return;

	SetFocus();
	tstring s;
	RDOEditorTabCtrl* tab = model->getTab();
	if (tab)
	{
		//studioApp.m_pStudioGUI->sendMessage( kernel->simulator(), RDOThread::RT_CODECOMP_GET_DATA, &rdo::service::simulation::RDOThreadCodeComp::GetCodeComp( tab->getCurrentRDOItem(), getCurrentPos(), getCurrentLineNumber(), s ) );

		rdo::service::simulation::RDOThreadSimulator::GetRTP RTPList;
		studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RTP, &RTPList);
		std::vector< rdo::service::simulation::RDOThreadSimulator::RTP >::iterator rtp_it = RTPList.begin();
		while (rtp_it != RTPList.end())
		{
			// Что-то делаем
			++rtp_it;
		}

		rdo::service::simulation::RDOThreadSimulator::GetRSS RSSList;
		studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RSS, &RSSList);
		std::vector< rdo::service::simulation::RDOThreadSimulator::RSS >::iterator rss_it = RSSList.begin();
		while (rss_it != RSSList.end())
		{
			// Что-то делаем
			++rss_it;
		}
	}

	if (s.empty())
	{
		s = getAllKW();
	}

	WordList wl;
	wl.Set(s.c_str());
	wl.InList("");
	s = "";

	typedef std::vector<tstring> string_list;

	WordListUtil wlist(wl);
	string_list temp = wlist.GetNearestWords("");
	for(string_list::const_iterator it = temp.begin(); it != temp.end(); ++it)
	{
		s += *it;
		if (it != temp.end() - 1)
			s += " ";
	}
	char currentLine[8000];
	int line = getCurrentLineNumber();
	sendEditor( SCI_GETLINE, line, reinterpret_cast<long>(currentLine) );

	int currentPos = getCurrentPos() - getPositionFromLine( line );

	int startPos = currentPos;

	while ((startPos > 0) && isRDOLexerIdentifier(currentLine[startPos - 1]))
	{
		startPos--;
	}

	currentLine[ currentPos ] = '\0';
	const char* str = currentLine + startPos;
	unsigned int strLength = currentPos - startPos;

	string_list words = wlist.GetNearestWords(str);
	if(words.empty())
		words = wlist.GetNearestWords("");

	string_list::const_iterator it = words.begin();
	tstring stWord = *it;
	std::sort(words.begin(), words.end());

	tstring pstr = "";
	for(string_list::const_iterator it = words.begin(); it != words.end(); ++it) 
	{
		pstr += (*it);
		if (it != words.end() - 1)
			pstr += " ";
	}
	LPCTSTR list;
	if (static_cast<PTR(RDOEditorEditStyle)>(style)->autoComplete->showFullList)
	{
		list = s.c_str();
	}
	else
	{
		list = pstr.c_str();
		if (!list)
		{
			list = s.c_str();
		}
	}

	if (list) 
	{
		tstring startKeyWord           = "";
		tstring startKeyWordScroll     = stWord;
		rbool useReplace = false;
		if (pstr.c_str())
		{
			wl.Clear();
			wl.Set(pstr.c_str());
			wl.InList("");
			startKeyWord = stWord;
			if (words.size() == 1 && strLength <= startKeyWord.length() && !boost::ifind_first(startKeyWord, str))
			{
				useReplace = true;
			}
		}

		while (startKeyWord.find('?') != tstring::npos)
		{
			tstring::size_type pos1 = startKeyWord.find('?');
			tstring::size_type pos2 = startKeyWord.find(' ', pos1);
			startKeyWord.erase(pos1, pos2 - pos1);
		}
		while (startKeyWordScroll.find('?') != tstring::npos)
		{
			tstring::size_type pos1 = startKeyWordScroll.find('?');
			tstring::size_type pos2 = startKeyWordScroll.find(' ', pos1);
			startKeyWordScroll.erase(pos1, pos2 - pos1);
		}

		if (useReplace)
		{
			setSelection  (getCurrentPos(), getCurrentPos() - strLength);
			replaceCurrent(startKeyWord);
		}
		else
		{
			sendEditor      (SCI_AUTOCSHOW,   strLength, (long)list);
			sendEditorString(SCI_AUTOCSELECT, 0, startKeyWordScroll.c_str());
			sendEditorString(SCI_AUTOCSELECT, 0, startKeyWord.c_str());
		}
	}
}

void RDOEditorEdit::setErrorLine( int line )
{
	clearErrorLine();
	if ( line == -1 ) line = getCurrentLineNumber();
	sendEditor( SCI_MARKERADD, line, sci_MARKER_ERROR );
}

void RDOEditorEdit::clearErrorLine()
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_ERROR );
	if ( nextLine >= 0 ) {
		sendEditor( SCI_MARKERDELETE, nextLine, sci_MARKER_ERROR );
		RedrawWindow();
	}
}

rbool RDOEditorEdit::hasErrorLine() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_ERROR );
	return nextLine >= 0;
}

void RDOEditorEdit::OnInsertCommand( UINT nID )
{
	CString s = "";
	if ( !s.LoadString( nID ) ) {
		AfxGetMainWnd()->GetMenu()->GetMenuString( nID, s, MF_BYCOMMAND );
	}

	int incPos = -1;

	switch ( nID ) {
		case ID_INSERT_PAT_TEMPL_OPERATION  :
		case ID_INSERT_PAT_TEMPL_EVENT:
		case ID_INSERT_PAT_TEMPL_KEYBOARD:
		case ID_INSERT_PAT_TEMPL_RULE       : incPos = 9;  break;
		case ID_INSERT_RTP_RTPPERMANENT     :
		case ID_INSERT_RTP_RTPTEMPORARY     : incPos = 15; break;
		case ID_INSERT_RSS_RSS              : incPos = 13; break;
		case ID_INSERT_ALGO_ELSE            : incPos = 6;  break;
		case ID_INSERT_FRM_FRM              : incPos = 7;  break;
		case ID_INSERT_FUN_FUN              :
		case ID_INSERT_FUN_SQN              : incPos = 10; break;
		case ID_INSERT_FUN_CNS              : incPos = 12; break;
		case ID_INSERT_DPT_TEMPL_SEARCH     :
		case ID_INSERT_DPT_TEMPL_SOME       : incPos = 16; break;
		case ID_INSERT_DPT_TEMPL_PRIOR      : incPos = 16; break;
		case ID_INSERT_DPT_ACTIV            : incPos = 14; break;
		case ID_INSERT_SMR_SMR              : incPos = 17; break;
		case ID_INSERT_PMD_PMD              : incPos = 11; break;
		case ID_INSERT_ALGO_FOR             : incPos = 5;  break;
		case ID_INSERT_ALGO_IF              :
		case ID_INSERT_ALGO_IF_ELSE         : incPos = 4;  break;
	}

	replaceCurrent( static_cast<LPCTSTR>(s), incPos );
}

void RDOEditorEdit::OnInsertBuffer1Paste() 
{
	if ( view ) {
		view->currentBuffer = 1;
		replaceCurrent( view->buf1 );
	}
}

void RDOEditorEdit::OnInsertBuffer2Paste() 
{
	if ( view ) {
		view->currentBuffer = 2;
		replaceCurrent( view->buf2 );
	}
}

void RDOEditorEdit::OnInsertBuffer3Paste() 
{
	if ( view ) {
		view->currentBuffer = 3;
		replaceCurrent( view->buf3 );
	}
}

void RDOEditorEdit::OnInsertBuffer4Paste() 
{
	if ( view ) {
		view->currentBuffer = 4;
		replaceCurrent( view->buf4 );
	}
}

void RDOEditorEdit::OnUndateBuffer1Paste( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf1.empty() );
}

void RDOEditorEdit::OnUndateBuffer2Paste( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf2.empty() );
}

void RDOEditorEdit::OnUndateBuffer3Paste( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf3.empty() );
}

void RDOEditorEdit::OnUndateBuffer4Paste( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf4.empty() );
}

void RDOEditorEdit::OnInsertBuffer1Append() 
{
	onBufferAppend( 1 );
}

void RDOEditorEdit::OnInsertBuffer2Append() 
{
	onBufferAppend( 2 );
}

void RDOEditorEdit::OnInsertBuffer3Append() 
{
	onBufferAppend( 3 );
}

void RDOEditorEdit::OnInsertBuffer4Append() 
{
	onBufferAppend( 4 );
}

void RDOEditorEdit::onBufferAppend( const int bufIndex )
{
	if ( !view ) return;

	int pos = getCurrentPos();
	if ( pos == getLength() ) return;
	rbool canUseSelected = false;
	if ( bufSelStart == -1 ) {
		CharacterRange cr = getSelectionRange();
		bufSelStart       = cr.cpMin;
		pos               = cr.cpMax;
		canUseSelected    = cr.cpMin != cr.cpMax;
	}
	tstring s = "";
	switch ( bufIndex ) {
		case 1: {
			if ( view->resetBuf1 ) {
				view->resetBuf1 = false;
			} else {
				s = view->buf1;
			}
			break;
		}
		case 2: {
			if ( view->resetBuf2 ) {
				view->resetBuf2 = false;
			} else {
				s = view->buf2;
			}
			break;
		}
		case 3: {
			if ( view->resetBuf3 ) {
				view->resetBuf3 = false;
			} else {
				s = view->buf3;
			}
			break;
		}
		case 4: {
			if ( view->resetBuf4 ) {
				view->resetBuf4 = false;
			} else {
				s = view->buf4;
			}
			break;
		}
	}

	view->restartBufTimer( bufIndex );

	if ( canUseSelected ) {
		s += getSelection();
	} else {
		char c = static_cast<char>(sendEditor( SCI_GETCHARAT, pos ));
		int line = getLineFromPosition( pos );
		s += c;
		pos++;
		if ( c == '\r' || c == '\n' ) {
			while ( line == getLineFromPosition( pos ) && ( static_cast<char>(sendEditor( SCI_GETCHARAT, pos )) == '\r' || static_cast<char>(sendEditor( SCI_GETCHARAT, pos )) == '\n' ) ) {
				s += static_cast<char>(sendEditor( SCI_GETCHARAT, pos ));
				pos++;
			}
		}
	}
	setCurrentPos( pos );
	setSelection( bufSelStart, pos );
	switch ( bufIndex ) {
		case 1: view->buf1 = s; break;
		case 2: view->buf2 = s; break;
		case 3: view->buf3 = s; break;
		case 4: view->buf4 = s; break;
	}
	view->currentBuffer = bufIndex;
}

void RDOEditorEdit::OnUndateBufferAppend( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && GUI_IS_SELECTED || getCurrentPos() != getLength() );
}

void RDOEditorEdit::OnInsertBuffer1Edit() 
{
	onBufferEdit( 1 );
}

void RDOEditorEdit::OnInsertBuffer2Edit() 
{
	onBufferEdit( 2 );
}

void RDOEditorEdit::OnInsertBuffer3Edit() 
{
	onBufferEdit( 3 );
}

void RDOEditorEdit::OnInsertBuffer4Edit() 
{
	onBufferEdit( 4 );
}

void RDOEditorEdit::onBufferEdit( const int bufIndex )
{
	if ( !view ) return;

	tstring bufName;
	tstring bufValue;

	bufName = rdo::format( ID_BUFFER_NAME );

	switch ( bufIndex ) {
		case 1: bufName += " 1:"; bufValue = view->buf1; break;
		case 2: bufName += " 2:"; bufValue = view->buf2; break;
		case 3: bufName += " 3:"; bufValue = view->buf3; break;
		case 4: bufName += " 4:"; bufValue = view->buf4; break;
	}

	if ( bufValue.empty() ) {
		bufValue = getCurrentOrSelectedWord();
	}
	RDOEditorEditBufferDlg dlg( bufName.c_str(), bufValue.c_str() );

	if ( dlg.DoModal() == IDOK ) {

		switch ( bufIndex ) {
			case 1: view->buf1 = dlg.bufValue; break;
			case 2: view->buf2 = dlg.bufValue; break;
			case 3: view->buf3 = dlg.bufValue; break;
			case 4: view->buf4 = dlg.bufValue; break;
		}
		view->currentBuffer = bufIndex;
	}
}

void RDOEditorEdit::OnUpdateInsertBufferEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( view != NULL );
}

void RDOEditorEdit::OnInsertBuffer1Clear() 
{
	if ( view ) {
		view->buf1 = "";
		view->currentBuffer = 1;
	}
}

void RDOEditorEdit::OnInsertBuffer2Clear() 
{
	if ( view ) {
		view->buf2 = "";
		view->currentBuffer = 2;
	}
}

void RDOEditorEdit::OnInsertBuffer3Clear() 
{
	if ( view ) {
		view->buf3 = "";
		view->currentBuffer = 3;
	}
}

void RDOEditorEdit::OnInsertBuffer4Clear() 
{
	if ( view ) {
		view->buf4 = "";
		view->currentBuffer = 4;
	}
}

void RDOEditorEdit::OnUndateBuffer1Clear( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf1.empty() );
}

void RDOEditorEdit::OnUndateBuffer2Clear( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf2.empty() );
}

void RDOEditorEdit::OnUndateBuffer3Clear( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf3.empty() );
}

void RDOEditorEdit::OnUndateBuffer4Clear( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( view && !view->buf4.empty() );
}

const rdoEditCtrl::RDOLogEdit* RDOEditorEdit::getLog() const
{
	return log;
}

void RDOEditorEdit::setLog( RDOLogEdit& _log )
{
	log = &_log;
}

void RDOEditorEdit::OnGotoNext()
{
	if ( log ) {
		log->gotoNext();
	}
}

void RDOEditorEdit::OnGotoPrev()
{
	if ( log ) {
		log->gotoPrev();
	}
}

void RDOEditorEdit::OnUpdateGotoNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( log ? true : false );
}

void RDOEditorEdit::OnUpdateGotoPrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( log ? true : false );
}

void RDOEditorEdit::OnHelpKeyword()
{
	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if ( s.find_first_of( keyword ) == tstring::npos || keyword.empty() ) {
		RDOEditorTabCtrl* tab = model->getTab();
		if ( tab ) {
			switch( tab->getCurrentRDOItem() ) {
				case rdoModelObjects::RTP: keyword = "rtp"; break;
				case rdoModelObjects::RSS: keyword = "rss"; break;
				case rdoModelObjects::EVN: keyword = "evn"; break;
				case rdoModelObjects::PAT: keyword = "pat"; break;
				case rdoModelObjects::DPT: keyword = "dpt"; break;
				case rdoModelObjects::PRC: keyword = "prc"; break;
				case rdoModelObjects::FRM: keyword = "frm"; break;
				case rdoModelObjects::FUN: keyword = "fun"; break;
				case rdoModelObjects::SMR: keyword = "smr"; break;
				case rdoModelObjects::PMD: keyword = "pmd"; break;
				default:                   keyword = ""; break;
			}
		}
	}

	QByteArray ba;
	ba.append("activateKeyword ");
	ba.append(keyword.c_str());
	ba.append("\n");
	studioApp.callQtAssistant(ba);
}
