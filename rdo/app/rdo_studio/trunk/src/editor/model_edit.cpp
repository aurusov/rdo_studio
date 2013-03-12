/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      model_edit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/algorithm/string.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/model_edit.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model.h"
#include "thirdparty/scintilla/lexlib/WordList.h"
#include "thirdparty/scintilla/rdo/LexRdo.h"
#include "thirdparty/scintilla/rdo/WordListUtil.h"
#include "thirdparty/scintilla/lexlib/CharacterSet.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

Model::Model(PTR(QWidget) pParent, PTR(QWidget) pView)
	: super              (pParent)
	, m_pView            (pView  )
	, m_pLog             (NULL   )
	, m_canClearErrorLine(true   )
	, m_pPopupMenu       (NULL   )
{
	m_sciFoldMarginID = getNewMarker();
	m_sciMarkerError  = getNewMarker();

	sendEditor(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT | SC_MOD_CHANGEFOLD);

	sendEditor(SCI_SETMARGINTYPEN     , m_sciFoldMarginID, SC_MARGIN_SYMBOL);
	sendEditor(SCI_SETFOLDFLAGS, 16);
	sendEditor(SCI_SETMARGINMASKN     , m_sciFoldMarginID, SC_MASK_FOLDERS);
	sendEditor(SCI_SETMARGINSENSITIVEN, m_sciFoldMarginID, 1);

	sendEditor(SCI_AUTOCSETIGNORECASE    , 1);
	sendEditor(SCI_AUTOCSETCHOOSESINGLE  , 0);
	sendEditor(SCI_AUTOCSETDROPRESTOFWORD, true );
	sendEditor(SCI_AUTOCSETCANCELATSTART , false);
	sendEditor(SCI_AUTOCSETAUTOHIDE      , false);
	sendEditor(SCI_AUTOCSTOPS            , 0, reinterpret_cast<long>("+-*/:[](),<>=."));

	QObject::connect(this, SIGNAL(modified(int, int, int, int, const QByteArray&, int, int, int)), this, SLOT(catchModified(int, int, int, int, const QByteArray&, int, int, int)));
	QObject::connect(this, SIGNAL(marginClicked(int, int, int)), this, SLOT(catchMarginClick(int, int, int)));

	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);
	m_pPopupMenu = new QMenu(this);
	m_pPopupMenu->addMenu(pMainWindow->menuInsert);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actEditCut);
	m_pPopupMenu->addAction(pMainWindow->actEditCopy);
	m_pPopupMenu->addAction(pMainWindow->actEditPaste);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actEditSelectAll);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchFind);
	m_pPopupMenu->addAction(pMainWindow->actSearchReplace);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindNext);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarksToggle);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarkNext);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarkPrev);
	m_pPopupMenu->addAction(pMainWindow->actSearchBookmarksClearAll);
}

Model::~Model()
{}

void Model::catchModified(int modificationType, int position, int length, int linesAdded, const QByteArray& bytes, int line, int foldLevelNow, int foldLevelPrev)
{
	UNUSED(linesAdded);
	UNUSED(length    );
	UNUSED(position  );
	UNUSED(bytes     );

	if (modificationType & SC_MOD_CHANGEFOLD)
	{
		foldChanged(line, foldLevelNow, foldLevelPrev);
	}
	if (m_canClearErrorLine && hasErrorLine())
	{
		clearErrorLine();
	}
}

void Model::catchMarginClick(int position, int modifiers, int margin)
{
	if (margin == m_sciFoldMarginID)
	{
		foldMarginClick(position, modifiers);
	}
}

void Model::setEditorStyle(PTR(ModelStyle) pStyle)
{
	super::setEditorStyle(pStyle);
	if (!m_pStyle)
		return;

	// ----------
	// Fold
	ModelTheme* theme = static_cast<ModelTheme*>(m_pStyle->theme);
	QColor foldFgColor = theme->foldFgColor;
	QColor foldBgColor = theme->foldBgColor;
	switch (theme->foldStyle)
	{
	case ModelTheme::F_NONE:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		break;

	case ModelTheme::F_PLUS:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_MINUS, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_PLUS , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foldFgColor, foldBgColor);
		break;

	case ModelTheme::F_PLUSCONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_MINUS  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_PLUS   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER, foldFgColor, foldBgColor);
		break;

	case ModelTheme::F_ARROW:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_ARROWDOWN, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_ARROW    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		break;

	case ModelTheme::F_ARROWCONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_ARROWDOWN, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_ARROW    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER  , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY    , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER  , foldFgColor, foldBgColor);
		break;

	case ModelTheme::F_BOXCONNECTED:
		defineMarker(SC_MARKNUM_FOLDEROPEN   , SC_MARK_BOXMINUS, foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDER       , SC_MARK_BOXPLUS , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERSUB    , SC_MARK_VLINE   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERTAIL   , SC_MARK_LCORNER , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEREND    , SC_MARK_EMPTY   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY   , foldFgColor, foldBgColor);
		defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_LCORNER , foldFgColor, foldBgColor);
		break;

	case ModelTheme::F_CIRCLECONNECTED:
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
	sendEditor(SCI_SETMARGINWIDTHN, 2, static_cast<ModelStyle*>(m_pStyle)->margin->fold ? 16 : 0);
	sendEditor(SCI_SETMARGINWIDTHN, 1, static_cast<ModelStyle*>(m_pStyle)->margin->bookmark ? 16 : 0);
	sendEditor(SCI_SETMARGINWIDTHN, 0, static_cast<ModelStyle*>(m_pStyle)->margin->lineNumber ? 40 : 0);

	// ----------
	// Error
	defineMarker(m_sciMarkerError, SC_MARK_BACKGROUND, QColor(0xFF, 0xFF, 0xFF), static_cast<ModelTheme*>(m_pStyle->theme)->errorBgColor);
}

void Model::expand(int& line, rbool doExpand, rbool force, int visLevels, int level) const
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

void Model::foldChanged(int line, int levelNow, int levelPrev) const
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

void Model::onToggleCurrentFold() const
{
	sendEditor(SCI_TOGGLEFOLD, getCurrentLineNumber());
}

void Model::onToggleAllFolds() const
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

void Model::foldMarginClick(int position, int modifiers) const
{
	int lineClick = getLineFromPosition(position);
	if ((modifiers & SCMOD_SHIFT) && (modifiers & SCMOD_CTRL))
	{
		onToggleAllFolds();
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

void Model::onEditCommentSelection() const
{
	if (!isSelected())
		return;

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

void Model::onEditCompleteWord()
{
	if (!static_cast<ModelStyle*>(m_pStyle)->autoComplete->useAutoComplete)
		return;

	setFocus();
	tstring primaryKwList;
	if (g_pModel->getTab())
	{
		//g_pApp->m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_CODECOMP_GET_DATA, &rdo::service::simulation::RDOThreadCodeComp::GetCodeComp(tab->getCurrentRDOItem(), getCurrentPos(), getCurrentLineNumber(), primaryKwList));

		rdo::service::simulation::RDOThreadSimulator::GetRTP RTPList;
		g_pApp->m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RTP, &RTPList);
		std::vector< rdo::service::simulation::RDOThreadSimulator::RTP >::iterator rtp_it = RTPList.begin();
		while (rtp_it != RTPList.end())
		{
			// Что-то делаем
			++rtp_it;
		}

		rdo::service::simulation::RDOThreadSimulator::GetRSS RSSList;
		g_pApp->m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RSS, &RSSList);
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

	class compareStringScintilla {
	public:
		bool operator()(tstring A, tstring B) {
			return CompareNCaseInsensitive(A.c_str(), B.c_str(), A.length()) < 0;
		}
	};

	compareStringScintilla functor;

	WordListUtil getList(fullWordList);
	string_list basicList = getList.getNearestWords(tstring());
	std::sort(basicList.begin(), basicList.end(), functor);
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

	while ((startPos > 0) && rdo::gui::lexer::isIdentifier(currentLine[startPos - 1]))
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
	std::sort(prioritySortedKwList.begin(), prioritySortedKwList.end(), functor);

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

	if (static_cast<PTR(ModelStyle)>(m_pStyle)->autoComplete->showFullList)
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
			sendEditorString(SCI_AUTOCSELECT, 0, startKeyWord.c_str());
		}
	}
}

void Model::setErrorLine(int line)
{
	clearErrorLine();
	if (line == -1)
	{
		line = getCurrentLineNumber();
	}
	sendEditor(SCI_MARKERADD, line, m_sciMarkerError);
}

void Model::clearErrorLine()
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerError);
	if (nextLine >= 0)
	{
		sendEditor(SCI_MARKERDELETE, nextLine, m_sciMarkerError);
		QWidget::update();
	}
}

rbool Model::hasErrorLine() const
{
	int nextLine = sendEditor(SCI_MARKERNEXT, 0, 1 << m_sciMarkerError);
	return nextLine >= 0;
}

void Model::onInsertCommand(QObject* pObject)
{
	MainWindow::InsertMenuData* pInsertMenuData = dynamic_cast<MainWindow::InsertMenuData*>(pObject);
	ASSERT(pInsertMenuData);

	replaceCurrent(
		pInsertMenuData->text().toLocal8Bit().constData(),
		pInsertMenuData->position().is_initialized()
			? pInsertMenuData->position().get()
			: -1
	);
}

CPTR(Log) Model::getLog() const
{
	return m_pLog;
}

void Model::setLog(REF(Log) pLog)
{
	m_pLog = &pLog;
}

void Model::setCanClearErrorLine(rbool value)
{
	m_canClearErrorLine = value;
}

void Model::onGotoNext()
{
	if (m_pLog)
	{
		m_pLog->gotoNext();
	}
}

void Model::onGotoPrev()
{
	if (m_pLog)
	{
		m_pLog->gotoPrev();
	}
}

void Model::onHelpContext()
{
	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if (s.find_first_of(keyword) == tstring::npos || keyword.empty())
	{
		model::TabCtrl* pTab = g_pModel->getTab();
		if (pTab)
		{
			switch(pTab->getCurrentRDOItem())
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
	g_pApp->callQtAssistant(ba);
}

void Model::onUpdateActions(rbool activated)
{
	super::onUpdateActions(activated);

	MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actEditCommentSelection,
		activated && isSelected(),
		this, &Model::onEditCommentSelection
	);

	updateAction(
		pMainWindow->actEditCompleteWord,
		activated && g_pApp->getStyle()->style_editor.autoComplete->useAutoComplete,
		this, &Model::onEditCompleteWord
	);

	updateAction(
		pMainWindow->actViewToggleCurrentFold,
		activated && !isEmpty(),
		this, &Model::onToggleCurrentFold
	);

	updateAction(
		pMainWindow->actViewToggleAllFolds,
		activated && !isEmpty(),
		this, &Model::onToggleAllFolds
	);

	updateAction(
		pMainWindow->actSearchLogNext,
		activated && m_pLog,
		this, &Model::onGotoNext
	);

	updateAction(
		pMainWindow->actSearchLogPrev,
		activated && m_pLog,
		this, &Model::onGotoPrev
	);

	pMainWindow->updateInsertMenu(
		activated,
		this, &Model::onInsertCommand
	);
}

void Model::mousePressEvent(QMouseEvent*  pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		super::mousePressEvent(pEvent);
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}
