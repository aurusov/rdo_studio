#include "stdafx.h"
#include "rdoeditoredit.h"
#include "../rdostudioeditbaseview.h"
#include "../edit_ctrls/sci/LexRdo.h"
#include "../edit_ctrls/sci/PropSet.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditBufferDlg
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOEditorEdit
// ---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOEditorEdit, RDOEditorBaseEdit )
	//{{AFX_MSG_MAP(RDOEditorEdit)
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
	ON_UPDATE_COMMAND_UI( ID_EDIT_COMMENTSELECTION, OnIsSelected )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_APPEND, OnUndateBufferAppend )
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER2_EDIT, OnUpdateInsertBufferEdit)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER3_EDIT, OnUpdateInsertBufferEdit)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER4_EDIT, OnUpdateInsertBufferEdit)
	//}}AFX_MSG_MAP

	ON_COMMAND_RANGE( ID_INSERT_PAT_PATOPERATION, ID_INSERT_FUNCTIONS_TAN, OnInsertCommand )

END_MESSAGE_MAP()

RDOEditorEdit::RDOEditorEdit( RDOStudioEditBaseView* _view ):
	RDOEditorBaseEdit(),
	bufSelStart( -1 ),
	view( _view )
{
	sci_MARKER_ERROR = getNewMarker();
}

RDOEditorEdit::~RDOEditorEdit()
{
}

BOOL RDOEditorEdit::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	SCNotification* scn = reinterpret_cast<SCNotification*>(lParam);

	if ( !RDOEditorBaseEdit::OnNotify( wParam, lParam, pResult ) || ( scn->nmhdr.hwndFrom == sciHWND && scn->nmhdr.code == SCN_CHARADDED ) ) {

		if ( scn->nmhdr.hwndFrom == sciHWND ) {
			switch( scn->nmhdr.code ) {
				case SCN_RDO_BUFFERKEY: {
					if ( view ) {
						switch ( view->currentBuffer ) {
							case 1: view->buf1 += scn->ch; break;
							case 2: view->buf2 += scn->ch; break;
							case 3: view->buf3 += scn->ch; break;
							case 4: view->buf4 += scn->ch; break;
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
					if ( hasErrorLine() ) clearErrorLine();
					return TRUE;
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

	sendEditor( SCI_AUTOCSETIGNORECASE    , 1 );
	sendEditor( SCI_AUTOCSETCHOOSESINGLE  , 0 );
	sendEditor( SCI_AUTOCSETDROPRESTOFWORD, true );
	sendEditor( SCI_AUTOCSETCANCELATSTART , false );
	sendEditor( SCI_AUTOCSETAUTOHIDE      , false );
	sendEditor( SCI_AUTOCSTOPS            , 0, reinterpret_cast<long>("+-*/:[](),<>=.") );

	setViewMargin( true );
	setViewFoldMargin( true );

	return 0;
}

void RDOEditorEdit::setEditorStyle( RDOEditorEditStyle* _style )
{
	RDOEditorBaseEdit::setEditorStyle( _style );
	if ( !style ) return;

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

bool RDOEditorEdit::hasErrorLine() const
{
	int nextLine = sendEditor( SCI_MARKERNEXT, 0, 1 << sci_MARKER_ERROR );
	return nextLine >= 0;
}

void RDOEditorEdit::commentSelection() const
{
	if ( isSelected() ) {
		string startComment( "{ " );
		string endComment( " }" );
		int startCommentLength = startComment.length();
		CharacterRange cr = getSelectionRange();
		int caretPosition = getCurrentPos();
		bool moveCaret = caretPosition < cr.cpMax;
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
	if ( !static_cast<RDOEditorEditStyle*>(style)->autoComplete->useAutoComplete ) return;

	SetFocus();

	string s = kw0;
	s += " ";
	s += kw1;
	s += " ";
	s += kw2;

	WordList wl;
	wl.Set( s.c_str() );
	wl.InList( "" );
	s = "";
	for ( int i = 0; i < wl.len; i++ ) {
		s += wl.wordsNoCase[i];
		if ( i < wl.len-1 ) s += " ";
	}

	char currentLine[8000];
	int line = getCurrentLineNumber();
	sendEditor( SCI_GETLINE, line, reinterpret_cast<long>(currentLine) );

	int currentPos = getCurrentPos() - getPositionFromLine( line );

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
	if ( ((RDOEditorEditStyle*)style)->autoComplete->showFullList ) {
		list = s.c_str();
	} else {
		list = words;
		if ( !list ) list = s.c_str();
	}

	if ( list ) {

		string startKeyWord       = "";
		string startKeyWordScroll = wl.wordsNoCase[ wl.len-1 ];
		bool useReplace = false;
		if ( words ) {
			wl.Set( words );
			wl.InList( "" );
			startKeyWord = wl.wordsNoCase[0];
			if ( wl.len == 1 && strLength <= startKeyWord.length() ) {
				useReplace = true;
			}
		}
		if ( useReplace ) {
			setSelection( getCurrentPos(), getCurrentPos() - strLength );
			replaceCurrent( startKeyWord );
		} else {
			sendEditor( SCI_AUTOCSHOW, strLength, (long)list );
			sendEditorString( SCI_AUTOCSELECT, 0, startKeyWordScroll.c_str() );
			sendEditorString( SCI_AUTOCSELECT, 0, startKeyWord.c_str() );
		}
	}

	if ( words ) delete []words;
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
	bool canUseSelected = false;
	if ( bufSelStart == -1 ) {
		CharacterRange cr = getSelectionRange();
		bufSelStart       = cr.cpMin;
		pos               = cr.cpMax;
		canUseSelected    = cr.cpMin != cr.cpMax;
	}
	string s = "";
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
	pCmdUI->Enable( view && isSelected() || getCurrentPos() != getLength() );
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

	string bufName;
	string bufValue;

	bufName = format( ID_BUFFER_NAME );

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
