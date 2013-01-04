/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_view.cpp
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <QtGui/qpainter.h>
#include <QtGui/qscrollbar.h>
#include <QtGui/qclipboard.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/log_view.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- LogCtrlFindInList
// --------------------------------------------------------------------------------
namespace rdo { namespace gui { namespace tracer {

class LogCtrlFindInList
{
public:
	LogCtrlFindInList(REF(rsint) checkCounter, CREF(tstring) strToFind, rbool matchCase, rbool matchWholeWord);
	rbool operator() (CREF(tstring) nextstr);

private:
	boost::optional<boost::regex> m_expression;
	REF(rsint)                    m_checkCounter;
};

}}} // namespace rdo::gui::tracer

LogCtrlFindInList::LogCtrlFindInList(REF(rsint) checkCounter, CREF(tstring) strToFind, rbool matchCase, rbool matchWholeWord)
	: m_checkCounter(checkCounter)
{
	tstring what = matchWholeWord
		? rdo::format("^%s$",   strToFind.c_str())
		: rdo::format(".*%s.*", strToFind.c_str());

	boost::regex_constants::syntax_option_type regex_constants(boost::regex::perl);

	if (!matchCase)
	{
		regex_constants |= boost::regex::icase;
	}

	try
	{
		m_expression = boost::regex(what, regex_constants);
	}
	catch (const std::exception&)
	{}
}

rbool LogCtrlFindInList::operator()(CREF(tstring) nextstr)
{
	++m_checkCounter;

	if (!m_expression.is_initialized())
		return false;

	try
	{
		rbool result = boost::regex_match(nextstr, m_expression.get());
		if (result)
		{
			TRACE1("found %s\n", nextstr.c_str());
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
	, m_cursor  (0)
	, m_cursorIt(m_list.end())
	, m_maxLegth(0)
{}

void LogView::StringList::push_back(CREF(tstring) value)
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

rsint LogView::StringList::count() const
{
	return m_count;
}

tstring::size_type LogView::StringList::maxLegth() const
{
	return m_maxLegth;
}

void LogView::StringList::setCursor(rsint pos, rsint max)
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
		for (rsint i = m_count; i > max; --i)
		{
			--m_cursorIt;
		}
	}
	else
	{
		rsint delta = pos - m_cursor;
		seek(delta, m_cursorIt);
	}
	m_cursor = pos;
}

LogView::StringList::const_iterator LogView::StringList::findString(rsint index) const
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
		rsint deltaPos = index - m_cursor;
		rsint deltaEnd = index - (m_count - 1);
		rsint mod_deltaPos = deltaPos >= 0 ? deltaPos : -1 * deltaPos;
		rsint mod_deltaEnd = deltaEnd >= 0 ? deltaEnd : -1 * deltaEnd;
		rsint delta = (std::min)(index, mod_deltaPos);
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
			seek(delta, res);
		}
	}

	return res;
}

LogView::StringList::const_reverse_iterator LogView::StringList::rFindString(rsint index) const
{
	const_reverse_iterator rit(findString(index));
	return rit;
}

void LogView::StringList::seek(rsint delta, REF(StringList::const_iterator) it) const
{
	ASSERT(it != m_list.end());

	if (delta > 0)
	{
		for (rsint i = 0; i < delta; i++)
		{
			++it;
		}
	}
	else
	{
		for (rsint i = delta; i < 0; i++)
		{
			--it;
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- LogView::SubitemColors
// --------------------------------------------------------------------------------
LogView::SubitemColors::SubitemColors()
	: m_addingSubitems(false)
	, m_parentColor   (NULL )
{}

LogView::SubitemColors::SubitemColors(CREF(SubitemColors) subitemColors)
	: m_colorList     (subitemColors.m_colorList     )
	, m_addingSubitems(subitemColors.m_addingSubitems)
	, m_parentColor   (subitemColors.m_parentColor   )
{}

// --------------------------------------------------------------------------------
// -------------------- LogView::ScrollMetric
// --------------------------------------------------------------------------------
LogView::ScrollMetric::ScrollMetric()
	: position(0)
	, posMax  (0)
	, pageSize(0)
{}

rbool LogView::ScrollMetric::applyInc(rsint delta)
{
	if (delta == 0)
	{
		return false;
	}

	if (position + delta < 0 || position + delta > posMax)
	{
		return false;
	}

	position += delta;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- LogView::ScrollMetricVert
// --------------------------------------------------------------------------------
LogView::ScrollMetricVert::ScrollMetricVert()
	: ScrollMetric    ()
	, lastViewableLine(0)
{}

rbool LogView::ScrollMetricVert::isVisible(rsint index) const
{
	return index >= position &&
		   index <= lastViewableLine;
}

rbool LogView::ScrollMetricVert::applyInc(rsint delta)
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
LogView::LogView(PTR(QAbstractScrollArea) pParent, PTR(LogStyle) pStyle)
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
		m_logStyle = &studioApp.getStyle()->style_trace;
	}

	connect(&getVertScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVertScrollBarValueChanged(int)));
	connect(&getHorzScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorzScrollBarValueChanged(int)));

	QPalette p = palette();
	p.setColor(QPalette::Inactive, QPalette::Highlight,       p.color(QPalette::Active, QPalette::Highlight));
	p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
	setPalette(p);

	setFont();
	updateScrollBars();

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
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

void LogView::push_back(CREF(tstring) log)
{
	if (!log.empty())
	{
		rsint posstart = log.find_first_not_of(' ');
		rsint posend   = log.find_first_of(' ', posstart);
		tstring key  = log.substr(posstart, posend - posstart);
		rdo::trim(key);

		LogColorPair* colors = NULL;

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

	rbool prevVisible = m_SM_Y.isVisible(m_strings.count() - 1);

	m_strings.push_back(log);

	rsint lastString = m_strings.count() - 1;
	if (lastString == 1)
	{
		setUpActionFind(isActivated());
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

void LogView::setText(tstring text)
{
	clear();
	while (!text.empty())
	{
		ruint pos = text.find_first_of("\r\n");
		if (pos == tstring::npos)
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

void LogView::selectLine(rsint index)
{
	if (index < 0 || index > m_strings.count() - 1 || index == selectedLine())
	{
		return;
	}

	rsint prevSel = selectedLine();
	rsint inc = (std::max)(-prevSel, (std::min)(index - prevSel, m_strings.count() - 1 - prevSel));

	if (inc)
	{
		setSelectedLine(selectedLine() + inc);

		rbool needrepaint = !makeLineVisible(selectedLine());
		if (needrepaint)
		{
			repaintLine(selectedLine());
		}

		repaintLine(prevSel);
	}
}

rbool LogView::getFocusOnly() const
{
	return m_focusOnly;
}

void LogView::setFocusOnly(rbool value)
{
	m_focusOnly = value;
}

CREF(LogStyle) LogView::getStyle() const
{
	return *m_logStyle;
}

void LogView::setStyle(LogStyle* style, rbool needRedraw)
{
	m_logStyle = style;
	setFont();

	updateScrollBars();

	if (needRedraw)
	{
		update();
	}
}

rbool LogView::getDrawLog() const
{
	return m_drawLog;
}

void LogView::setDrawLog(rbool value)
{
	if (m_drawLog != value)
	{
		m_drawLog = value;
		updateScrollBars();
		update();
		makeLineVisible(selectedLine());
	}
}

rbool LogView::getItemColors(rsint index, LogColorPair* &colors) const
{
	rbool res = true;
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

rbool LogView::getItemColors(CREF(tstring) item, LogColorPair* &colors) const
{
	return m_logStyle->getItemColors(item, colors);
}

rsint LogView::selectedLine() const
{
	return m_selectedLine;
}

void LogView::setSelectedLine(rsint selectedLine)
{
	m_selectedLine = selectedLine;
	setUpActionEditCopy(isActivated());
	setUpCoordStatusBar(isActivated());
}

tstring LogView::getString(rsint index) const
{
	tstring result;

	if (index >= 0 && index < m_strings.count())
	{
		result = *m_strings.findString(index);
	}

	return result;
}

tstring LogView::getSelected() const
{
	return getString(selectedLine());
}

REF(QScrollBar) LogView::getVertScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->verticalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

REF(QScrollBar) LogView::getHorzScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->horizontalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

void LogView::updateScrollBars()
{
	m_SM_Y.pageSize = m_clientRect.height() / m_lineHeight;
	m_SM_Y.posMax   = (std::max)(0, m_strings.count() - m_SM_Y.pageSize);
	m_SM_Y.position = (std::min)(m_SM_Y.position, m_SM_Y.posMax);

	m_SM_X.pageSize = (m_clientRect.width() - m_logStyle->borders->horzBorder) / m_charWidth;
	m_SM_X.posMax   = (std::max)(0, rsint(m_strings.maxLegth()) - m_SM_X.pageSize);
	m_SM_X.position = (std::min)(m_SM_X.position, m_SM_X.posMax);

	rsint mul = m_SM_Y.pageSize;
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

rbool LogView::scrollVertically(rsint inc)
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

rbool LogView::scrollHorizontally(rsint inc)
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

rbool LogView::makeLineVisible(rsint index)
{
	rbool res = false;

	if (isFullyVisible(index))
	{
		return res;
	}

	rsint inc;
	if (m_SM_Y.position < index)
	{
		rsint lastVisible = m_SM_Y.position + m_clientRect.height() / m_lineHeight - 1;
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

rbool LogView::isFullyVisible(rsint index) const
{
	rsint lastVisible = m_SM_Y.position + m_clientRect.height() / m_lineHeight - 1;
	return index <= lastVisible && index >= m_SM_Y.position;
}

QRect LogView::getLineRect(rsint index) const
{
	QRect rect(m_clientRect);
	rect.setTop((index - m_SM_Y.position) * m_lineHeight);
	rect.setBottom((std::min)(rect.top() + m_lineHeight, rect.bottom()));
	return rect;
}

void LogView::repaintLine(rsint index)
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

	m_font = QFont(m_logStyle->font->name.c_str());
	m_font.setBold     (m_logStyle->theme->style & rdoStyle::RDOStyleFont::BOLD     );
	m_font.setItalic   (m_logStyle->theme->style & rdoStyle::RDOStyleFont::ITALIC   );
	m_font.setUnderline(m_logStyle->theme->style & rdoStyle::RDOStyleFont::UNDERLINE);
	m_font.setPointSize(m_logStyle->font->size);

	QFontMetrics fontMetrics(m_font);
	m_lineHeight = fontMetrics.height() + 2 * m_logStyle->borders->vertBorder;
	m_charWidth  = fontMetrics.averageCharWidth(); // fontMetrics.maxWidth()
}

void LogView::setUpActionFind(rbool activate)
{
	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	if (activate && m_strings.count())
	{
		if (!pMainWindow->actSearchFind->isEnabled())
		{
			pMainWindow->actSearchFind->setEnabled(true);
			connect(pMainWindow->actSearchFind, SIGNAL(triggered(bool)), this, SLOT(onSearchFind()));
		}
	}
	else
	{
		if (pMainWindow->actSearchFind->isEnabled())
		{
			pMainWindow->actSearchFind->setEnabled(false);
			disconnect(pMainWindow->actSearchFind, SIGNAL(triggered(bool)), this, SLOT(onSearchFind()));
		}
	}

	if (activate && !m_findSettings.what.empty())
	{
		if (!pMainWindow->actSearchFindNext->isEnabled())
		{
			pMainWindow->actSearchFindNext->setEnabled(true);
			pMainWindow->actSearchFindPrevious->setEnabled(true);
			connect(pMainWindow->actSearchFindNext,     SIGNAL(triggered(bool)), this, SLOT(onSearchFindNext()));
			connect(pMainWindow->actSearchFindPrevious, SIGNAL(triggered(bool)), this, SLOT(onSearchFindPrevious()));
		}
	}
	else
	{
		if (pMainWindow->actSearchFindNext->isEnabled())
		{
			pMainWindow->actSearchFindNext->setEnabled(false);
			pMainWindow->actSearchFindPrevious->setEnabled(false);
			disconnect(pMainWindow->actSearchFindNext,     SIGNAL(triggered(bool)), this, SLOT(onSearchFindNext()));
			disconnect(pMainWindow->actSearchFindPrevious, SIGNAL(triggered(bool)), this, SLOT(onSearchFindPrevious()));
		}
	}
}

void LogView::setUpActionEditCopy(rbool activate)
{
	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	if (activate && canCopy())
	{
		if (!pMainWindow->actEditCopy->isEnabled())
		{
			pMainWindow->actEditCopy->setEnabled(true);
			connect(pMainWindow->actEditCopy, SIGNAL(triggered(bool)), this, SLOT(onEditCopy()));
		}
	}
	else
	{
		if (pMainWindow->actEditCopy->isEnabled())
		{
			pMainWindow->actEditCopy->setEnabled(false);
			disconnect(pMainWindow->actEditCopy, SIGNAL(triggered(bool)), this, SLOT(onEditCopy()));
		}
	}
}

void LogView::setUpCoordStatusBar(rbool activate)
{
	QString coord = activate && selectedLine() != -1
		? QString("1 : %1").arg(selectedLine())
		: QString();

	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->statusBar()->update<StatusBar::SB_COORD>(coord);
}

rbool LogView::canCopy() const
{
	return selectedLine() != -1;
}

rsint LogView::find(rbool searchDown)
{
	rsint result = -1;

	rsint startPos = selectedLine() == -1
		? searchDown
			? 0
			: m_strings.count()
		: searchDown
			? selectedLine() + 1
			: selectedLine() - 1;

	rsint checkCounter = 0;
	LogCtrlFindInList findInList(checkCounter, m_findSettings.what, m_findSettings.matchCase, m_findSettings.matchWholeWord);

	rbool found = searchDown
		? std::find_if(m_strings.findString(startPos), m_strings.end(), findInList) != m_strings.end()
		: std::find_if(m_strings.rFindString(startPos + 1), m_strings.rend(), findInList) != m_strings.rend();

	if (found)
	{
		result = startPos + (checkCounter - 1) * (searchDown ? 1 : -1);
	}

	return result;
}

void LogView::onFindDlgFind(CREF(FindDialog::Settings) settings)
{
	m_findSettings = settings;
	setUpActionFind(isActivated());
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

			rsint firstLine = (std::max)(0, m_SM_Y.position + pEvent->rect().top() / m_lineHeight);
			rsint mul = pEvent->rect().bottom() / m_lineHeight;
			if (pEvent->rect().bottom() > mul * m_lineHeight)
			{
				mul++;
			}
			rsint lastLine = (std::min)(m_strings.count() - 1, m_SM_Y.position + mul - 1);

			LogColorPair* colors = NULL;

			rsint y = m_lineHeight * (-m_SM_Y.position + firstLine - 1);
			QRect rect(m_charWidth * (-m_SM_X.position), y, pEvent->rect().width() + m_charWidth * m_SM_X.position, m_lineHeight);
			QRect textRect(
				rect.left  () + m_logStyle->borders->horzBorder,
				rect.top   () + m_logStyle->borders->vertBorder,
				rect.width () - m_logStyle->borders->horzBorder * 2,
				rect.height() - m_logStyle->borders->vertBorder * 2
			);

			StringList::const_iterator it = m_strings.findString(firstLine);
			for (rsint i = firstLine; i < lastLine + 1; i++)
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
					colors = new LogColorPair();
					colors->foregroundColor = palette().color(QPalette::HighlightedText);
					colors->backgroundColor = palette().color(QPalette::Highlight);
				}

				rect    .translate(0, m_lineHeight);
				textRect.translate(0, m_lineHeight);

				//Main drawing cycle
				painter.setBackgroundMode(Qt::TransparentMode);
				painter.fillRect(rect, colors->backgroundColor);
				painter.setPen  (colors->foregroundColor);
				painter.drawText(
					textRect,
					QString::fromStdString(*it)
				);
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

				if (i == selectedLine() && !m_focusOnly && colors)
				{
					delete colors;
					colors = NULL;
				}
			}

			getItemColors("", colors);

			painter.fillRect(
				pEvent->rect().left(),
				rect.bottom(),
				pEvent->rect().width(),
				pEvent->rect().height() - rect.bottom(),
				colors->backgroundColor
			);
		}
	}
	else
	{
		LogColorPair* colors = NULL;
		getItemColors("", colors);

		painter.fillRect(
			m_clientRect,
			colors->backgroundColor
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
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}

void LogView::onActivate()
{
	repaintLine(selectedLine());

	connect(studioApp.getMainWndUI()->actHelpContext, SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));

	setUpActionFind    (true);
	setUpActionEditCopy(true);
	setUpCoordStatusBar(true);

	studioApp.getMainWndUI()->statusBar()->update<StatusBar::SB_MODIFY>("Только чтение");
}

void LogView::onDeactivate()
{
	repaintLine(selectedLine());

	disconnect(studioApp.getMainWndUI()->actHelpContext, SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));

	setUpActionFind    (false);
	setUpActionEditCopy(false);
	setUpCoordStatusBar(false);

	studioApp.getMainWndUI()->statusBar()->update<StatusBar::SB_MODIFY>("");
}

void LogView::onEditCopy()
{
	QApplication::clipboard()->setText(QString::fromStdString(getSelected()));
}

void LogView::onHelpContext()
{
	tstring line = getSelected();

	tstring keyword = "trc";
	if (!line.empty())
	{
		rsint posstart = line.find_first_not_of(' ');
		rsint posend   = line.find_first_of(' ', posstart);
		keyword      = line.substr(posstart, posend - posstart);
		rdo::trim(keyword);

		if (!keyword.empty())
		{
			LogColorPair* colors;
			if (!m_logStyle->getItemColors(keyword, colors))
			{
				getItemColors(selectedLine(), colors);
				if (*colors == m_logStyle->theme->sd)
				{
					keyword = "SD";
				}
			}
		}
	}

	QByteArray ba;
	ba.append("activateKeyword ");
	ba.append(keyword.c_str());
	ba.append("\n");
	studioApp.callQtAssistant(ba);
}
