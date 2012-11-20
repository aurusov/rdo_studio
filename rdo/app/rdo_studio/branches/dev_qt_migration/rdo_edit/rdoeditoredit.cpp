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
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/edit/view_base.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "thirdparty/sci/lexlib/WordList.h"
#include "thirdparty/sci/rdo/LexRdo.h"
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
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	CString bufName;
	CString bufValue;

	RDOEditorEditBufferDlg(CString _bufName, CString _bufValue): CDialog(IDD_BUFFER), bufName(_bufName), bufValue(_bufValue) {};
};

void RDOEditorEditBufferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_BUFFERVALUESTATIC, bufName);
	DDX_Text(pDX, IDC_BUFFERVALUEEDIT, bufValue);
}

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEdit
// ---------------------------------------------------------------------------

// ON_UPDATE_COMMAND_UI сделано

//! @todo qt
//BEGIN_MESSAGE_MAP( RDOEditorEdit, RDOEditorBaseEdit )
//	ON_COMMAND(ID_EDIT_COMMENTSELECTION, OnEditCommentSelection)
//	ON_COMMAND(ID_EDIT_COMPLETEWORD, OnEditCompleteWord)
//	ON_COMMAND(ID_INSERT_BUFFER1_PASTE, OnInsertBuffer1Paste)
//	ON_COMMAND(ID_INSERT_BUFFER2_PASTE, OnInsertBuffer2Paste)
//	ON_COMMAND(ID_INSERT_BUFFER3_PASTE, OnInsertBuffer3Paste)
//	ON_COMMAND(ID_INSERT_BUFFER4_PASTE, OnInsertBuffer4Paste)
//	ON_COMMAND(ID_INSERT_BUFFER1_APPEND, OnInsertBuffer1Append)
//	ON_COMMAND(ID_INSERT_BUFFER2_APPEND, OnInsertBuffer2Append)
//	ON_COMMAND(ID_INSERT_BUFFER3_APPEND, OnInsertBuffer3Append)
//	ON_COMMAND(ID_INSERT_BUFFER4_APPEND, OnInsertBuffer4Append)
//	ON_COMMAND(ID_INSERT_BUFFER1_EDIT, OnInsertBuffer1Edit)
//	ON_COMMAND(ID_INSERT_BUFFER2_EDIT, OnInsertBuffer2Edit)
//	ON_COMMAND(ID_INSERT_BUFFER3_EDIT, OnInsertBuffer3Edit)
//	ON_COMMAND(ID_INSERT_BUFFER4_EDIT, OnInsertBuffer4Edit)
//	ON_COMMAND(ID_INSERT_BUFFER1_CLEAR, OnInsertBuffer1Clear)
//	ON_COMMAND(ID_INSERT_BUFFER2_CLEAR, OnInsertBuffer2Clear)
//	ON_COMMAND(ID_INSERT_BUFFER3_CLEAR, OnInsertBuffer3Clear)
//	ON_COMMAND(ID_INSERT_BUFFER4_CLEAR, OnInsertBuffer4Clear)
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_PASTE, OnUndateBuffer1Paste )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_PASTE, OnUndateBuffer2Paste )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_PASTE, OnUndateBuffer3Paste )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_PASTE, OnUndateBuffer4Paste )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_APPEND, OnUndateBufferAppend )
//	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER1_EDIT, OnUpdateInsertBufferEdit)
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER1_CLEAR, OnUndateBuffer1Clear )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_CLEAR, OnUndateBuffer2Clear )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_CLEAR, OnUndateBuffer3Clear )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_CLEAR, OnUndateBuffer4Clear )
//	ON_COMMAND(ID_BUILDFINDLOG_GOTO_NEXT, OnGotoNext)
//	ON_COMMAND(ID_BUILDFINDLOG_GOTO_PREV, OnGotoPrev)
//	ON_UPDATE_COMMAND_UI(ID_BUILDFINDLOG_GOTO_NEXT, OnUpdateGotoNext)
//	ON_UPDATE_COMMAND_UI(ID_BUILDFINDLOG_GOTO_PREV, OnUpdateGotoPrev)
//	ON_COMMAND(ID_VIEW_TOGGLE_ALLFOLDS, OnToggleAllFolds)
//	ON_COMMAND(ID_VIEW_TOGGLE_CURRENTFOLD, OnToggleCurrentFold)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLE_ALLFOLDS, OnUpdateFold)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOGGLE_CURRENTFOLD, OnUpdateFold)
//	ON_UPDATE_COMMAND_UI( ID_EDIT_COMMENTSELECTION, OnIsSelected )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER2_APPEND, OnUndateBufferAppend )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER3_APPEND, OnUndateBufferAppend )
//	ON_UPDATE_COMMAND_UI( ID_INSERT_BUFFER4_APPEND, OnUndateBufferAppend )
//	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER2_EDIT, OnUpdateInsertBufferEdit)
//	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER3_EDIT, OnUpdateInsertBufferEdit)
//	ON_UPDATE_COMMAND_UI(ID_INSERT_BUFFER4_EDIT, OnUpdateInsertBufferEdit)
//	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
//
//	ON_COMMAND_RANGE( ID_INSERT_PAT_TEMPL_OPERATION, ID_INSERT_ALGO_RETURN, OnInsertCommand )
//
//END_MESSAGE_MAP()

RDOEditorEdit::RDOEditorEdit(PTR(QWidget) pParent, PTR(RDOStudioEditBaseView) pView)
	: RDOEditorBaseEdit(pParent)
	, view             (pView  )
	, log              (NULL   )
	, canClearErrorLine(true   )
{
	sci_FOLDMARGIN_ID = getNewMarker();
	sci_MARKER_ERROR  = getNewMarker();

	sendEditor(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT | SC_MOD_CHANGEFOLD);

	sendEditor(SCI_SETMARGINTYPEN     , sci_FOLDMARGIN_ID, SC_MARGIN_SYMBOL);
	sendEditor(SCI_SETFOLDFLAGS, 16);
	sendEditor(SCI_SETMARGINMASKN     , sci_FOLDMARGIN_ID, SC_MASK_FOLDERS);
	sendEditor(SCI_SETMARGINSENSITIVEN, sci_FOLDMARGIN_ID, 1);

	sendEditor(SCI_AUTOCSETIGNORECASE    , 1);
	sendEditor(SCI_AUTOCSETCHOOSESINGLE  , 0);
	sendEditor(SCI_AUTOCSETDROPRESTOFWORD, true );
	sendEditor(SCI_AUTOCSETCANCELATSTART , false);
	sendEditor(SCI_AUTOCSETAUTOHIDE      , false);
	sendEditor(SCI_AUTOCSTOPS            , 0, reinterpret_cast<long>("+-*/:[](),<>=."));

	QObject::connect(this, SIGNAL(modified(int, int, int, int, const QByteArray&, int, int, int)), this, SLOT(catchModified(int, int, int, int, const QByteArray&, int, int, int)));
	QObject::connect(this, SIGNAL(marginClicked(int, int, int)), this, SLOT(catchMarginClick(int, int, int)));
}

RDOEditorEdit::~RDOEditorEdit()
{}

void RDOEditorEdit::catchModified(int modificationType, int position, int length, int linesAdded, const QByteArray& bytes, int line, int foldLevelNow, int foldLevelPrev)
{
	UNUSED(linesAdded);
	UNUSED(length    );
	UNUSED(position  );
	UNUSED(bytes     );

	if (modificationType & SC_MOD_CHANGEFOLD)
	{
		foldChanged(line, foldLevelNow, foldLevelPrev);
	}
	if (canClearErrorLine && hasErrorLine())
	{
		clearErrorLine();
	}
}

void RDOEditorEdit::catchMarginClick(int position, int modifiers, int margin)
{
	if (margin == sci_FOLDMARGIN_ID)
	{
		foldMarginClick(position, modifiers);
	}
}

void RDOEditorEdit::setEditorStyle(PTR(RDOEditorEditStyle) pStyle)
{
	RDOEditorBaseEdit::setEditorStyle(pStyle);
	if (!style)
		return;

	// ----------
	// Fold
	RDOEditorEditTheme* theme = static_cast<RDOEditorEditTheme*>(style->theme);
	COLORREF foldFgColor = theme->foldFgColor;
	COLORREF foldBgColor = theme->foldBgColor;
	switch (theme->foldStyle)
	{
	case RDOFOLDS_NONE:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		break;

	case RDOFOLDS_PLUS:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_MINUS, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_PLUS , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		break;

	case RDOFOLDS_PLUSCONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_MINUS  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_PLUS   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER, foldFgColor, foldBgColor);
		break;

	case RDOFOLDS_ARROW:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_ARROWDOWN, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_ARROW    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		break;

	case RDOFOLDS_ARROWCONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_ARROWDOWN, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_ARROW    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER  , foldFgColor, foldBgColor);
		break;

	case RDOFOLDS_BOXCONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_BOXMINUS, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_BOXPLUS , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER , foldFgColor, foldBgColor);
		break;

	case RDOFOLDS_CIRCLECONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_CIRCLEMINUS, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_CIRCLEPLUS , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE      , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY      , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY      , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER    , foldFgColor, foldBgColor);
		break;
	}
	tstring cf_prop("CommentFold");
	tstring cf_val_1("1");
	tstring cf_val_0("0");
	sendEditorString(SCI_SETPROPERTY, reinterpret_cast<unsigned long>(cf_prop.c_str()), theme->commentFold ? cf_val_1.c_str() : cf_val_0.c_str());
	sendEditor(SCI_COLOURISE, 0, -1);

	// ----------
	// Margin
	sendEditor(SCI_SETMARGINWIDTHN, 2, static_cast<RDOEditorEditStyle*>(style)->margin->fold ? 16 : 0);
	sendEditor(SCI_SETMARGINWIDTHN, 1, static_cast<RDOEditorEditStyle*>(style)->margin->bookmark ? 16 : 0);
	sendEditor(SCI_SETMARGINWIDTHN, 0, static_cast<RDOEditorEditStyle*>(style)->margin->lineNumber ? 40 : 0);

	// ----------
	// Error
	defineMarker(sci_MARKER_ERROR, SC_MARK_BACKGROUND, RGB(0xFF, 0xFF, 0xFF), static_cast<RDOEditorEditTheme*>(style->theme)->errorBgColor);
}

void RDOEditorEdit::OnEditCommentSelection() 
{
	commentSelection();
}

void RDOEditorEdit::OnEditCompleteWord() 
{
	completeWord();
}

void RDOEditorEdit::expand(int& line, rbool doExpand, rbool force, int visLevels, int level) const
{
	int lineMaxSubord = sendEditor(SCI_GETLASTCHILD, line, level & SC_FOLDLEVELNUMBERMASK);
	line++;
	while (line <= lineMaxSubord)
	{
		if (force)
		{
			sendEditor(
				visLevels > 0
					? SCI_SHOWLINES
					: SCI_HIDELINES,
				line,
				line
			);
		}
		else
		{
			if (doExpand)
			{
				sendEditor(SCI_SHOWLINES, line, line);
			}
		}
		int levelLine = level;
		if (levelLine == -1)
		{
			levelLine = sendEditor(SCI_GETFOLDLEVEL, line);
		}
		if (levelLine & SC_FOLDLEVELHEADERFLAG)
		{
			if (force)
			{
				sendEditor(
					SCI_SETFOLDEXPANDED,
					line,
					visLevels > 1
						? 1
						: 0
				);
				expand(line, doExpand, force, visLevels - 1);
			}
			else
			{
				if (doExpand)
				{
					if (!sendEditor(SCI_GETFOLDEXPANDED, line))
					{
						sendEditor(SCI_SETFOLDEXPANDED, line, 1);
					}
					expand(line, true, force, visLevels - 1);
				}
				else
				{
					expand(line, false, force, visLevels - 1);
				}
			}
		}
		else
		{
			line++;
		}
	}
}

void RDOEditorEdit::foldChanged(int line, int levelNow, int levelPrev) const
{
	if (levelNow & SC_FOLDLEVELHEADERFLAG)
	{
		if (!(levelPrev & SC_FOLDLEVELHEADERFLAG))
		{
			sendEditor(SCI_SETFOLDEXPANDED, line, 1);
		}
	}
	else if (levelPrev & SC_FOLDLEVELHEADERFLAG)
	{
		if (!sendEditor(SCI_GETFOLDEXPANDED, line))
		{
			expand(line, true, false, 0, levelPrev);
		}
	}
}

void RDOEditorEdit::toggleCurrentFold() const
{
	sendEditor(SCI_TOGGLEFOLD, getCurrentLineNumber());
}

void RDOEditorEdit::toggleAllFolds() const
{
	sendEditor(SCI_COLOURISE, 0, -1);
	int maxLine = getLineCount();
	rbool expanding = true;
	for (int lineSeek = 0; lineSeek < maxLine; lineSeek++)
	{
		if (sendEditor(SCI_GETFOLDLEVEL, lineSeek) & SC_FOLDLEVELHEADERFLAG)
		{
			expanding = !sendEditor(SCI_GETFOLDEXPANDED, lineSeek);
			break;
		}
	}
	for (int line = 0; line < maxLine; line++)
	{
		int level = sendEditor(SCI_GETFOLDLEVEL, line);
		if ((level & SC_FOLDLEVELHEADERFLAG) &&
		    (SC_FOLDLEVELBASE == (level & SC_FOLDLEVELNUMBERMASK)))
		{
			if (expanding)
			{
				sendEditor(SCI_SETFOLDEXPANDED, line, 1);
				expand(line, true, false, 0, level);
				line--;
			}
			else
			{
				int lineMaxSubord = sendEditor(SCI_GETLASTCHILD, line, -1);
				sendEditor(SCI_SETFOLDEXPANDED, line, 0);
				if (lineMaxSubord > line)
				{
					sendEditor(SCI_HIDELINES, line + 1, lineMaxSubord);
				}
			}
		}
	}
}

void RDOEditorEdit::foldMarginClick(int position, int modifiers) const
{
	int lineClick = getLineFromPosition(position);
	if ((modifiers & SCMOD_SHIFT) && (modifiers & SCMOD_CTRL))
	{
		toggleAllFolds();
	}
	else
	{
		int levelClick = sendEditor(SCI_GETFOLDLEVEL, lineClick);
		if (levelClick & SC_FOLDLEVELHEADERFLAG)
		{
			if (modifiers & SCMOD_SHIFT)
			{
				// Ensure all children visible
				sendEditor(SCI_SETFOLDEXPANDED, lineClick, 1);
				expand(lineClick, true, true, 100, levelClick);
			}
			else if (modifiers & SCMOD_CTRL)
			{
				if (sendEditor(SCI_GETFOLDEXPANDED, lineClick))
				{
					// Contract this line and all children
					sendEditor(SCI_SETFOLDEXPANDED, lineClick, 0);
					expand(lineClick, false, true, 0, levelClick);
				}
				else
				{
					// Expand this line and all children
					sendEditor(SCI_SETFOLDEXPANDED, lineClick, 1);
					expand(lineClick, true, true, 100, levelClick);
				}
			}
			else
			{
				// Toggle this line
				sendEditor(SCI_TOGGLEFOLD, lineClick);
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
	pCmdUI->Enable(!GUI_IS_EMPTY);
}

void RDOEditorEdit::commentSelection() const
{
	if (GUI_IS_SELECTED)
	{
		tstring startComment("/*");
		tstring endComment("*/");
		int startCommentLength = startComment.length();
		CharacterRange cr = getSelectionRange();
		int caretPosition = getCurrentPos();
		rbool moveCaret   = caretPosition < cr.cpMax;
		sendEditor(SCI_BEGINUNDOACTION);
		sendEditorString(SCI_INSERTTEXT, cr.cpMin, startComment.c_str());
		cr.cpMax += startCommentLength;
		cr.cpMin += startCommentLength;
		sendEditorString(SCI_INSERTTEXT, cr.cpMax, endComment.c_str());
		if (moveCaret)
		{
			sendEditor(SCI_GOTOPOS,       cr.cpMax);
			sendEditor(SCI_SETCURRENTPOS, cr.cpMin);
		}
		else
		{
			setSelection(cr.cpMin, cr.cpMax);
		}
		sendEditor(SCI_ENDUNDOACTION);
	}
}

void RDOEditorEdit::completeWord()
{
	if (!static_cast<RDOEditorEditStyle*>(style)->autoComplete->useAutoComplete)
		return;

	setFocus();
	tstring primaryKwList;
	if (model->getTab())
	{
		//studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_CODECOMP_GET_DATA, &rdo::service::simulation::RDOThreadCodeComp::GetCodeComp(tab->getCurrentRDOItem(), getCurrentPos(), getCurrentLineNumber(), primaryKwList));

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

	if (primaryKwList.empty())
	{
		primaryKwList = getAllKW();
	}

	WordList fullWordList;
	fullWordList.Set(primaryKwList.c_str());
	fullWordList.InList("");
	primaryKwList = "";

	typedef std::vector<tstring> string_list;

	WordListUtil getList(fullWordList);
	string_list basicList = getList.getNearestWords(tstring());
	for (string_list::const_iterator it = basicList.begin(); it != basicList.end(); ++it)
	{
		primaryKwList += *it;
		if (it != basicList.end() - 1)
		{
			primaryKwList += " ";
		}
	}
	char currentLine[8000];
	int line = getCurrentLineNumber();
	sendEditor(SCI_GETLINE, line, reinterpret_cast<long>(currentLine));

	int currentPos = getCurrentPos() - getPositionFromLine(line);

	int startPos = currentPos;

	while ((startPos > 0) && isRDOLexerIdentifier(currentLine[startPos - 1]))
	{
		startPos--;
	}
	currentLine[ currentPos ] = '\0';
	const char*  userPattern = currentLine + startPos;
	unsigned int userPatternLength = currentPos - startPos;

	string_list prioritySortedKwList = getList.getNearestWords(userPattern);
	if (prioritySortedKwList.empty())
	{
		prioritySortedKwList = basicList;
	}

	string_list::const_iterator it = prioritySortedKwList.begin();
	tstring stWord = *it;
	std::sort(prioritySortedKwList.begin(), prioritySortedKwList.end());

	tstring foundKeyWords = "";
	for (string_list::const_iterator it = prioritySortedKwList.begin(); it != prioritySortedKwList.end(); ++it) 
	{
		foundKeyWords += (*it);
		if (it != prioritySortedKwList.end() - 1)
		{
			foundKeyWords += " ";
		}
	}
	LPCTSTR list;
	if (static_cast<PTR(RDOEditorEditStyle)>(style)->autoComplete->showFullList)
	{
		list = primaryKwList.c_str();
	}
	else
	{
		list = foundKeyWords.c_str();
		if (!list)
		{
			list = primaryKwList.c_str();
		}
	}

	if (list) 
	{
		tstring startKeyWord       = "";
		tstring startKeyWordScroll = stWord;
		rbool useReplace = false;
		if (foundKeyWords.c_str())
		{
			fullWordList.Clear();
			fullWordList.Set(foundKeyWords.c_str());
			fullWordList.InList("");
			startKeyWord = stWord;
			if (prioritySortedKwList.size() == 1 && userPatternLength <= startKeyWord.length() && boost::ifind_first(startKeyWord, userPattern).begin() == startKeyWord.begin())
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
			setSelection  (getCurrentPos(), getCurrentPos() - userPatternLength);
			replaceCurrent(startKeyWord);
		}
		else
		{
			sendEditor      (SCI_AUTOCSHOW,   userPatternLength, (long)list);
			sendEditorString(SCI_AUTOCSELECT, 0, startKeyWordScroll.c_str());
			sendEditorString(SCI_AUTOCSELECT, 0, startKeyWord.c_str());
		}
	}
}

void RDOEditorEdit::setErrorLine(int line)
{
	clearErrorLine();
	if (line == -1)
	{
		line = getCurrentLineNumber();
	}
	sendEditor(SCI_MARKERADD, line, sci_MARKER_ERROR);
}

void RDOEditorEdit::clearErrorLine()
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << sci_MARKER_ERROR);
	if (nextLine >= 0)
	{
		sendEditor(SCI_MARKERDELETE, nextLine, sci_MARKER_ERROR);
		QWidget::update();
	}
}

rbool RDOEditorEdit::hasErrorLine() const
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << sci_MARKER_ERROR);
	return nextLine >= 0;
}

void RDOEditorEdit::OnInsertCommand(UINT nID)
{
	CString s = "";
	if (!s.LoadString(nID))
	{
		AfxGetMainWnd()->GetMenu()->GetMenuString(nID, s, MF_BYCOMMAND);
	}

	int incPos = -1;

	switch (nID) {
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

	replaceCurrent(static_cast<LPCTSTR>(s), incPos);
}

CPTR(rdoEditCtrl::RDOLogEdit) RDOEditorEdit::getLog() const
{
	return log;
}

void RDOEditorEdit::setLog(REF(RDOLogEdit) log)
{
	this->log = &log;
}

void RDOEditorEdit::OnGotoNext()
{
	if (log)
	{
		log->gotoNext();
	}
}

void RDOEditorEdit::OnGotoPrev()
{
	if (log)
	{
		log->gotoPrev();
	}
}

void RDOEditorEdit::OnUpdateGotoNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(log ? true : false);
}

void RDOEditorEdit::OnUpdateGotoPrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(log ? true : false);
}

void RDOEditorEdit::OnHelpKeyword()
{
	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if (s.find_first_of(keyword) == tstring::npos || keyword.empty())
	{
		RDOEditorTabCtrl* tab = model->getTab();
		if (tab)
		{
			switch(tab->getCurrentRDOItem())
			{
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
