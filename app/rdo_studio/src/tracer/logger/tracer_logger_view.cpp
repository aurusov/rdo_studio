/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_logger_view.cpp
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/tracer_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <QPainter>
#include <QScrollBar>
#include <QClipboard>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/logger/tracer_logger_view.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;
using namespace rdo::gui::style;

// --------------------------------------------------------------------------------
// -------------------- LogCtrlFindInList
// --------------------------------------------------------------------------------
namespace rdo { namespace gui { namespace tracer {

class LogCtrlFindInList
{
public:
	LogCtrlFindInList(int& checkCounter, const QString& strToFind, bool matchCase, bool matchWholeWord);
	bool operator() (const QString& nextStr);

	LogCtrlFindInList& operator= (const LogCtrlFindInList&)
	{
		NEVER_REACH_HERE;
		return *this;
	}

private:
	boost::optional<boost::regex> m_expression;
	int& m_checkCounter;
};

}}} // namespace rdo::gui::tracer

LogCtrlFindInList::LogCtrlFindInList(int& checkCounter, const QString& strToFind, bool matchCase, bool matchWholeWord)
	: m_checkCounter(checkCounter)
{
	QString what = matchWholeWord
		? QString("^%1$").arg(strToFind)
		: QString(".*%1.*").arg(strToFind);

	boost::regex_constants::syntax_option_type regex_constants(boost::regex::perl);

	if (!matchCase)
	{
		regex_constants |= boost::regex::icase;
	}

	try
	{
		m_expression = boost::regex(what.toStdString(), regex_constants);
	}
	catch (const std::exception&)
	{}
}

bool LogCtrlFindInList::operator()(const QString& nextStr)
{
	++m_checkCounter;

	if (!m_expression.is_initialized())
		return false;

	try
	{
		bool result = boost::regex_match(nextStr.toStdString(), m_expression.get());
		if (result)
		{
			TRACE1("found %s\n", nextStr.toStdString().c_str());
		}
		return result;
	}
	catch (const std::exception&)
	{}
	return false;
}

// --------------------------------------------------------------------------------
// -------------------- LogView::StringList
// --------------------------------------------------------------------------------
LogView::StringList::StringList()
	: m_count   (0)
	, m_cursorIt(m_list.end())
	, m_cursor  (0)
	, m_maxLegth(0)
{}

void LogView::StringList::push_back(const QString& value)
{
	m_list.push_back(value);
	++m_count;

	if (m_count == 1)
	{
		m_cursorIt = m_list.begin();
	}

	if (value.length() > m_maxLegth)
	{
		m_maxLegth = value.length();
	}
}

LogView::StringList::const_iterator LogView::StringList::begin() const
{
	return m_list.begin();
}

LogView::StringList::const_iterator LogView::StringList::end() const
{
	return m_list.end();
}

LogView::StringList::const_reverse_iterator LogView::StringList::rbegin() const
{
	return m_list.rbegin();
}

LogView::StringList::const_reverse_iterator LogView::StringList::rend() const
{
	return m_list.rend();
}

void LogView::StringList::clear()
{
	m_list.clear();
	m_count    = 0;
	m_maxLegth = 0;
}

int LogView::StringList::count() const
{
	return m_count;
}

int LogView::StringList::maxLegth() const
{
	return m_maxLegth;
}

void LogView::StringList::setCursor(int pos, int max)
{
	if (pos == m_cursor)
		return;

	if (pos == 0)
	{
		m_cursorIt = m_list.begin();
	}
	else if (pos == max)
	{
		m_cursorIt = m_list.end();
		for (int i = m_count; i > max; --i)
		{
			--m_cursorIt;
		}
	}
	else
	{
		int delta = pos - m_cursor;
		ASSERT(m_cursorIt != m_list.end());
		std::advance(m_cursorIt, delta);
	}
	m_cursor = pos;
}

LogView::StringList::const_iterator LogView::StringList::findString(int index) const
{
	const_iterator res;

	if (index == 0)
	{
		res = m_list.begin();
	}
	else if (index == m_cursor)
	{
		res = m_cursorIt;
	}
	else if (index == m_count - 1)
	{
		res = m_list.end();
		--res;
	}
	else
	{
		int deltaPos = index - m_cursor;
		int deltaEnd = index - (m_count - 1);
		int mod_deltaPos = deltaPos >= 0 ? deltaPos : -1 * deltaPos;
		int mod_deltaEnd = deltaEnd >= 0 ? deltaEnd : -1 * deltaEnd;
		int delta = (std::min)(index, mod_deltaPos);
		delta = (std::min)(delta, mod_deltaEnd);
		if (delta == index)
		{
			res = m_list.begin();
		}
		else if (delta == mod_deltaPos)
		{
			res = m_cursorIt;
			delta = deltaPos;
		}
		else if (delta == mod_deltaEnd)
		{
			res = m_list.end();
			--res;
			delta = deltaEnd;
		}

		if (res != m_list.end())
		{
			std::advance(res, delta);
		}
	}

	return res;
}

LogView::StringList::const_reverse_iterator LogView::StringList::rFindString(int index) const
{
	const_reverse_iterator rit(findString(index));
	return rit;
}

// --------------------------------------------------------------------------------
// -------------------- LogView::SubitemColors
// --------------------------------------------------------------------------------
LogView::SubitemColors::SubitemColors()
	: m_addingSubitems(false)
	, m_parentColor   ()
{}

LogView::SubitemColors::SubitemColors(const SubitemColors& subitemColors)
	: m_colorList     (subitemColors.m_colorList     )
	, m_addingSubitems(subitemColors.m_addingSubitems)
	, m_parentColor   (subitemColors.m_parentColor   )
{}

// --------------------------------------------------------------------------------
// -------------------- LogView::ScrollMetricVert
// --------------------------------------------------------------------------------
LogView::ScrollMetricVert::ScrollMetricVert()
	: ScrollMetric    ()
	, lastViewableLine(0)
{}

bool LogView::ScrollMetricVert::isVisible(int index) const
{
	return index >= position &&
	       index <= lastViewableLine;
}

bool LogView::ScrollMetricVert::applyInc(int delta)
{
	if (!ScrollMetric::applyInc(delta))
	{
		return false;
	}

	lastViewableLine += delta;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- LogView
// --------------------------------------------------------------------------------
LogView::LogView(QAbstractScrollArea* pParent, LogStyle* pStyle)
	: parent_type       (pParent)
	, m_pScrollArea     (pParent)
	, m_logStyle        (pStyle )
	, m_lineHeight      (0 )
	, m_charWidth       (0 )
	, m_selectedLine    (-1)
	, m_pPopupMenu      (NULL )
	, m_fullRepaintLines(0    )
	, m_focusOnly       (false)
	, m_pFindDialog     (NULL )
	, m_drawLog         (true )
{
	if (!m_logStyle)
	{
		m_logStyle = &g_pApp->getStyle()->style_trace;
	}

	connect(&getVertScrollBar(), &QScrollBar::valueChanged, this, &LogView::onVertScrollBarValueChanged);
	connect(&getHorzScrollBar(), &QScrollBar::valueChanged, this, &LogView::onHorzScrollBarValueChanged);

	QPalette p = palette();
	p.setColor(QPalette::Inactive, QPalette::Highlight,       p.color(QPalette::Active, QPalette::Highlight));
	p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
	setPalette(p);

	setFont();
	updateScrollBars();

	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);
	m_pPopupMenu = new QMenu(this);
	m_pPopupMenu->addAction(pMainWindow->actEditCopy);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actSearchFind);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindNext);
	m_pPopupMenu->addAction(pMainWindow->actSearchFindPrevious);
}

LogView::~LogView()
{
	clear();
}

void LogView::push_back(const std::string& log)
{
	if (!log.empty())
	{
		int posstart = log.find_first_not_of(' ');
		int posend = log.find_first_of(' ', posstart);
		std::string key = boost::algorithm::trim_copy(log.substr(posstart, posend - posstart));

		LogColorPair colors;

		if (m_logStyle->getItemColors(key, colors))
		{
			m_subitemColors.m_addingSubitems = false;
		}
		else if (m_subitemColors.m_addingSubitems)
		{
			m_subitemColors.m_colorList.insert(SubitemColors::List::value_type(m_strings.count(), m_subitemColors.m_parentColor));
		}

		if (key == "SD")
		{
			m_subitemColors.m_addingSubitems = true;
			m_logStyle->getItemColors(key, m_subitemColors.m_parentColor);
		}
	}

	bool prevVisible = m_SM_Y.isVisible(m_strings.count() - 1);

	m_strings.push_back(QString::fromStdString(log));

	int lastString = m_strings.count() - 1;
	if (lastString == 1)
	{
		updateActionFind(isActivated());
	}

	if (m_drawLog)
	{
		updateScrollBars();

		m_fullRepaintLines = 1;

		if ( selectedLine() != -1 && selectedLine() == lastString - 1)
		{
			setSelectedLine(lastString);
			m_fullRepaintLines++;
		}

		if (!isFullyVisible(lastString) && prevVisible && (!m_SM_Y.isVisible(selectedLine()) || selectedLine() == lastString))
		{
			//::SendMessage(m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL);
			scrollVertically(m_SM_Y.posMax - m_SM_Y.position);
		}
		else if (m_SM_Y.isVisible(lastString))
		{
			repaintLine(lastString);
			if (m_fullRepaintLines == 2)
			{
				repaintLine(lastString - 1);
			}
		}

		m_fullRepaintLines = 0;
	}
	else
	{
		if (selectedLine() != -1 && selectedLine() == lastString - 1)
		{
			setSelectedLine(lastString);
		}
	}
}

void LogView::setText(std::string text)
{
	clear();
	while (!text.empty())
	{
		std::size_t pos = text.find_first_of("\r\n");
		if (pos == std::string::npos)
		{
			pos = text.length();
		}
		push_back(pos ? text.substr(0, pos) : "");
		text.erase(0, pos);
		text.erase(0, text.find_first_not_of("\r\n"));
	}
}

void LogView::clear()
{
	m_strings.clear();
	m_subitemColors = SubitemColors();

	m_SM_X = ScrollMetric();
	m_SM_Y = ScrollMetricVert();
	setSelectedLine(-1);

	updateScrollBars();
	update();
}

void LogView::selectLine(int index)
{
	if (index < 0 || index > m_strings.count() - 1 || index == selectedLine())
	{
		return;
	}

	int prevSel = selectedLine();
	int inc = (std::max)(-prevSel, (std::min)(index - prevSel, m_strings.count() - 1 - prevSel));

	if (inc)
	{
		setSelectedLine(selectedLine() + inc);

		bool needrepaint = !makeLineVisible(selectedLine());
		if (needrepaint)
		{
			repaintLine(selectedLine());
		}

		repaintLine(prevSel);
	}
}

bool LogView::getFocusOnly() const
{
	return m_focusOnly;
}

void LogView::setFocusOnly(bool value)
{
	m_focusOnly = value;
}

const LogStyle& LogView::getStyle() const
{
	return *m_logStyle;
}

void LogView::setStyle(LogStyle* style, bool needRedraw)
{
	m_logStyle = style;
	setFont();

	updateScrollBars();

	if (needRedraw)
	{
		update();
	}
}

bool LogView::getDrawLog() const
{
	return m_drawLog;
}

void LogView::setDrawLog(bool value)
{
	if (m_drawLog != value)
	{
		m_drawLog = value;
		updateScrollBars();
		update();
		makeLineVisible(selectedLine());
	}
}

bool LogView::getItemColors(int index, LogColorPair &colors) const
{
	bool res = true;
	SubitemColors::List::const_iterator it = m_subitemColors.m_colorList.find(index);
	if (it != m_subitemColors.m_colorList.end())
	{
		colors = (*it).second;
	}
	else
	{
		res = LogView::getItemColors("", colors);
	}

	return res;
}

bool LogView::getItemColors(const QString& item, LogColorPair &colors) const
{
	return m_logStyle->getItemColors(item.toStdString(), colors);
}

int LogView::selectedLine() const
{
	return m_selectedLine;
}

void LogView::setSelectedLine(int selectedLine)
{
	m_selectedLine = selectedLine;
	updateActionEditCopy(isActivated());
	updateCoordStatusBar(isActivated());
}

QString LogView::getString(int index) const
{
	QString result;

	if (index >= 0 && index < m_strings.count())
	{
		result = *m_strings.findString(index);
	}

	return result;
}

QString LogView::getSelected() const
{
	return getString(selectedLine());
}

QScrollBar& LogView::getVertScrollBar()
{
	QScrollBar* pScrollBar = m_pScrollArea->verticalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

QScrollBar& LogView::getHorzScrollBar()
{
	QScrollBar* pScrollBar = m_pScrollArea->horizontalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

void LogView::updateScrollBars()
{
	m_SM_Y.pageSize = m_clientRect.height() / m_lineHeight;
	m_SM_Y.posMax   = (std::max)(0, m_strings.count() - m_SM_Y.pageSize);
	m_SM_Y.position = (std::min)(m_SM_Y.position, m_SM_Y.posMax);

	m_SM_X.pageSize = (m_clientRect.width() - m_logStyle->borders.horzBorder) / m_charWidth;
	m_SM_X.posMax   = (std::max)(0, int(m_strings.maxLegth()) - m_SM_X.pageSize);
	m_SM_X.position = (std::min)(m_SM_X.position, m_SM_X.posMax);

	int mul = m_SM_Y.pageSize;
	if (mul * m_lineHeight < m_clientRect.height())
	{
		mul++;
	}
	m_SM_Y.lastViewableLine = m_SM_Y.position + mul - 1;

	m_strings.setCursor(m_SM_Y.position, m_SM_Y.posMax);

	getVertScrollBar().setRange   (0, m_SM_Y.posMax);
	getVertScrollBar().setPageStep(m_drawLog && m_SM_Y.posMax > 0 ? m_SM_Y.pageSize : 0);
	getVertScrollBar().setValue   (m_SM_Y.position);

	getHorzScrollBar().setRange   (0, m_SM_X.posMax);
	getHorzScrollBar().setPageStep(m_drawLog && m_SM_X.posMax > 0 ? m_SM_X.pageSize : 0);
	getHorzScrollBar().setValue   (m_SM_X.position);
}

bool LogView::scrollVertically(int inc)
{
	if (!m_SM_Y.applyInc(inc))
	{
		return false;
	}

	m_strings.setCursor(m_SM_Y.position, m_SM_Y.posMax);
	getVertScrollBar().setValue(m_SM_Y.position);
	update();
	return true;
}

bool LogView::scrollHorizontally(int inc)
{
	if (!m_SM_X.applyInc(inc))
	{
		return false;
	}

	getHorzScrollBar().setValue(m_SM_X.position);
	update();
	return true;
}

void LogView::onVertScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	scrollVertically(value - m_SM_Y.position);
}

void LogView::onHorzScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	scrollHorizontally(value - m_SM_X.position);
}

bool LogView::makeLineVisible(int index)
{
	bool res = false;

	if (isFullyVisible(index))
	{
		return res;
	}

	int inc;
	if (m_SM_Y.position < index)
	{
		int lastVisible = m_SM_Y.position + m_clientRect.height() / m_lineHeight - 1;
		inc = index - lastVisible;
	}
	else
	{
		inc = index - m_SM_Y.position;
	}

	//Repainting nearby lines after scrolling
	m_fullRepaintLines = 2;

	res = scrollVertically(inc);

	m_fullRepaintLines = 0;

	return res;
}

bool LogView::isFullyVisible(int index) const
{
	int lastVisible = m_SM_Y.position + m_clientRect.height() / m_lineHeight - 1;
	return index <= lastVisible && index >= m_SM_Y.position;
}

QRect LogView::getLineRect(int index) const
{
	QRect rect(m_clientRect);
	rect.setTop((index - m_SM_Y.position) * m_lineHeight);
	rect.setBottom((std::min)(rect.top() + m_lineHeight, rect.bottom()));
	return rect;
}

void LogView::repaintLine(int index)
{
	if (m_SM_Y.isVisible(index))
	{
		update(getLineRect(index));
		update();
	}
}

void LogView::setFont()
{
	if (!m_logStyle)
	{
		return;
	}

	m_font = QFont(m_logStyle->font.name.c_str());
	m_font.setBold     (m_logStyle->fontStyle & StyleFont::BOLD      ? true : false);
	m_font.setItalic   (m_logStyle->fontStyle & StyleFont::ITALIC    ? true : false);
	m_font.setUnderline(m_logStyle->fontStyle & StyleFont::UNDERLINE ? true : false);
	m_font.setPointSize(m_logStyle->font.size);

	QFontMetrics fontMetrics(m_font);
	m_lineHeight = fontMetrics.height() + 2 * m_logStyle->borders.vertBorder;
	m_charWidth  = fontMetrics.averageCharWidth(); // fontMetrics.maxWidth()
}

void LogView::updateActionFind(bool activated)
{
	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actSearchFind,
		activated && m_strings.count(),
		this, &LogView::onSearchFind
	);

	bool findNextPrev = activated && !m_findSettings.what.isEmpty();
	updateAction(
		pMainWindow->actSearchFindNext,
		findNextPrev,
		this, &LogView::onSearchFindNext
	);

	updateAction(
		pMainWindow->actSearchFindPrevious,
		findNextPrev,
		this, &LogView::onSearchFindPrevious
	);
}

void LogView::updateActionEditCopy(bool activated)
{
	Ui::MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actEditCopy,
		activated && canCopy(),
		this, &LogView::onEditCopy
	);
}

void LogView::updateCoordStatusBar(bool activated)
{
	QString coord = activated && selectedLine() != -1
		? QString("1 : %1").arg(selectedLine())
		: QString();

	MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->statusBar()->update<StatusBar::SB_COORD>(coord);
}

bool LogView::canCopy() const
{
	return selectedLine() != -1;
}

int LogView::find(bool searchDown)
{
	int result = -1;

	int startPos = selectedLine() == -1
		? searchDown
			? 0
			: m_strings.count()
		: searchDown
			? selectedLine() + 1
			: selectedLine() - 1;

	int checkCounter = 0;
	LogCtrlFindInList findInList(checkCounter, m_findSettings.what, m_findSettings.matchCase, m_findSettings.matchWholeWord);

	bool found = searchDown
		? std::find_if(m_strings.findString(startPos), m_strings.end(), findInList) != m_strings.end()
		: std::find_if(m_strings.rFindString(startPos + 1), m_strings.rend(), findInList) != m_strings.rend();

	if (found)
	{
		result = startPos + (checkCounter - 1) * (searchDown ? 1 : -1);
	}

	return result;
}

void LogView::onFindDlgFind(const FindDialog::Settings& settings)
{
	m_findSettings = settings;
	updateActionFind(isActivated());
	onSearchFindNext();
}

void LogView::onFindDlgClose()
{
	m_pFindDialog = NULL;
}

void LogView::onSearchFind()
{
	m_findSettings.what = getSelected();

	if (!m_pFindDialog)
	{
		m_pFindDialog = new FindDialog(
			this,
			boost::bind(&LogView::onFindDlgFind, this, _1),
			boost::bind(&LogView::onFindDlgClose, this)
		);
	}

	m_pFindDialog->setSettings(m_findSettings);
	m_pFindDialog->show();
	m_pFindDialog->raise();
	m_pFindDialog->activateWindow();
}

void LogView::onSearchFindNext()
{
	selectLine(find(m_findSettings.searchDown));
}

void LogView::onSearchFindPrevious()
{
	selectLine(find(!m_findSettings.searchDown));
}

void LogView::resizeEvent(QResizeEvent* pEvent)
{
	parent_type::resizeEvent(pEvent);

	m_clientRect = QRect(QPoint(0, 0), pEvent->size());
	updateScrollBars();
}

void LogView::paintEvent(QPaintEvent* pEvent)
{
	QPainter painter(this);

	if (m_drawLog)
	{
		if (!pEvent->rect().isEmpty() && !pEvent->rect().isNull())
		{
			painter.setFont(m_font);

			int firstLine = (std::max)(0, m_SM_Y.position + pEvent->rect().top() / m_lineHeight);
			int mul = pEvent->rect().bottom() / m_lineHeight;
			if (pEvent->rect().bottom() > mul * m_lineHeight)
			{
				mul++;
			}
			int lastLine = (std::min)(m_strings.count() - 1, m_SM_Y.position + mul - 1);

			LogColorPair colors;

			int y = m_lineHeight * (-m_SM_Y.position + firstLine - 1);
			QRect rect(m_charWidth * (-m_SM_X.position), y, pEvent->rect().width() + m_charWidth * m_SM_X.position, m_lineHeight);
			QRect textRect(
				rect.left  () + m_logStyle->borders.horzBorder,
				rect.top   () + m_logStyle->borders.vertBorder,
				rect.width () - m_logStyle->borders.horzBorder * 2,
				rect.height() - m_logStyle->borders.vertBorder * 2
			);

			StringList::const_iterator it = m_strings.findString(firstLine);
			for (int i = firstLine; i < lastLine + 1; i++)
			{
				if (i != selectedLine() || m_focusOnly)
				{
					if (!getItemColors((*it), colors))
					{
						getItemColors(i, colors);
					}
				}
				else
				{
					colors.foregroundColor = palette().color(QPalette::HighlightedText);
					colors.backgroundColor = palette().color(QPalette::Highlight);
				}

				rect    .translate(0, m_lineHeight);
				textRect.translate(0, m_lineHeight);

				//Main drawing cycle
				painter.setBackgroundMode(Qt::TransparentMode);
				painter.fillRect(rect, colors.backgroundColor);
				painter.setPen  (colors.foregroundColor);
				painter.drawText(textRect, *it);
				//End of main drawing cycle :)

				if (i == selectedLine() && hasFocus())
				{
					QRect focusRect(m_clientRect);
					focusRect.setTop   (rect.top   ());
					focusRect.setBottom(rect.bottom());
					QStyleOptionFocusRect option;
					option.initFrom(this);
					option.state |= QStyle::State_KeyboardFocusChange;
					option.backgroundColor = palette().color(QPalette::Background);
					option.rect = focusRect;
					style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
				}

				++it;

				//! @todo необходимо ли, когда colors не указатель?
				//if (i == selectedLine() && !m_focusOnly)
				//{
				//	colors = NULL;
				//}
			}

			getItemColors("", colors);

			painter.fillRect(
				pEvent->rect().left(),
				rect.bottom(),
				pEvent->rect().width(),
				pEvent->rect().height() - rect.bottom(),
				colors.backgroundColor
			);
		}
	}
	else
	{
		LogColorPair colors;
		getItemColors("", colors);

		painter.fillRect(
			m_clientRect,
			colors.backgroundColor
		);
	}

	parent_type::paintEvent(pEvent);
}

void LogView::keyPressEvent(QKeyEvent* pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Up:
		selectLine(selectedLine() - 1);
		break;

	case Qt::Key_Down:
		selectLine(selectedLine() + 1);
		break;

	case Qt::Key_PageUp:
		selectLine((std::max)(selectedLine() - m_SM_Y.pageSize, 0));
		break;

	case Qt::Key_PageDown:
		selectLine((std::min)(selectedLine() + m_SM_Y.pageSize, m_strings.count() - 1));
		break;

	case Qt::Key_Home:
		selectLine(0);
		break;

	case Qt::Key_End:
		selectLine(m_strings.count() - 1);
		break;

	default:
		break;
	}
}

void LogView::wheelEvent(QWheelEvent* pEvent)
{
	getVertScrollBar().setValue(getVertScrollBar().value() + (pEvent->delta() < 0 ? 1 : -1));
}

void LogView::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		selectLine((std::min)(m_SM_Y.position + pEvent->pos().y() / m_lineHeight, m_strings.count() - 1));
	}
}

void LogView::contextMenuEvent(QContextMenuEvent* pEvent)
{
	m_pPopupMenu->exec(pEvent->globalPos());
}

void LogView::onUpdateActions(bool activated)
{
	repaintLine(selectedLine());

	MainWindow* pMainWindow = g_pApp->getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actHelpContext,
		activated,
		this, &LogView::onHelpContext
	);

	updateActionFind    (activated);
	updateActionEditCopy(activated);
	updateCoordStatusBar(activated);

	pMainWindow->statusBar()->update<StatusBar::SB_MODIFY>(activated
		? "Только чтение"
		: QString()
	);
}

void LogView::onEditCopy()
{
	QApplication::clipboard()->setText(getSelected());
}

void LogView::onHelpContext()
{
	QString line = getSelected().simplified();

	QString keyword("trc");
	if (!line.isEmpty())
	{
		keyword = line.left(line.indexOf(' '));

		if (!keyword.isEmpty())
		{
			LogColorPair colors;
			if (!m_logStyle->getItemColors(keyword.toStdString(), colors))
			{
				getItemColors(selectedLine(), colors);
				if (colors == m_logStyle->sd)
				{
					keyword = "SD";
				}
			}
		}
	}

	QByteArray ba;
	ba.append("activateKeyword ");
	ba.append(keyword);
	ba.append("\n");
	g_pApp->callQtAssistant(ba);
}
