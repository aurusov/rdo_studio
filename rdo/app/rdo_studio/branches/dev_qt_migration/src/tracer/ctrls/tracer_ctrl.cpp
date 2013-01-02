/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl.cpp
  \author    Захаров Павел
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
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl.h"
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

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogCtrlFindInList
// --------------------------------------------------------------------------------
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
// -------------------- LogCtrl::StringList
// --------------------------------------------------------------------------------
LogCtrl::StringList::StringList()
	: m_count   (0)
	, m_cursor  (0)
	, m_cursorIt(m_list.end())
	, m_maxLegth(0)
{}

void LogCtrl::StringList::push_back(CREF(tstring) value)
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

LogCtrl::StringList::const_iterator LogCtrl::StringList::begin() const
{
	return m_list.begin();
}

LogCtrl::StringList::const_iterator LogCtrl::StringList::end() const
{
	return m_list.end();
}

LogCtrl::StringList::const_reverse_iterator LogCtrl::StringList::rbegin() const
{
	return m_list.rbegin();
}

LogCtrl::StringList::const_reverse_iterator LogCtrl::StringList::rend() const
{
	return m_list.rend();
}

void LogCtrl::StringList::clear()
{
	m_list.clear();
	m_count    = 0;
	m_maxLegth = 0;
}

rsint LogCtrl::StringList::count() const
{
	return m_count;
}

tstring::size_type LogCtrl::StringList::maxLegth() const
{
	return m_maxLegth;
}

void LogCtrl::StringList::setCursor(rsint pos, rsint max)
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
		seek(delta, m_cursorIt);
	}
	m_cursor = pos;
}

LogCtrl::StringList::const_iterator LogCtrl::StringList::findString(int index) const
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
			seek(delta, res);
		}
	}

	return res;
}

LogCtrl::StringList::const_reverse_iterator LogCtrl::StringList::rFindString(int index) const
{
	const_reverse_iterator rit(findString(index));
	return rit;
}

void LogCtrl::StringList::seek(rsint delta, REF(StringList::const_iterator) it) const
{
	ASSERT(it != m_list.end());

	if (delta > 0)
	{
		for (int i = 0; i < delta; i++)
		{
			++it;
		}
	}
	else
	{
		for (int i = delta; i < 0; i++)
		{
			--it;
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- LogCtrl::ScrollMetric
// --------------------------------------------------------------------------------
LogCtrl::ScrollMetric::ScrollMetric()
	: position(0)
	, posMax  (0)
	, pageSize(0)
{}

rbool LogCtrl::ScrollMetric::applyInc(rsint delta)
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
// -------------------- LogCtrl::ScrollMetricVert
// --------------------------------------------------------------------------------
LogCtrl::ScrollMetricVert::ScrollMetricVert()
	: ScrollMetric    ()
	, lastViewableLine(0)
{}

rbool LogCtrl::ScrollMetricVert::isVisible(rsint index) const
{
	return index >= position &&
		   index <= lastViewableLine;
}

rbool LogCtrl::ScrollMetricVert::applyInc(rsint delta)
{
	if (!ScrollMetric::applyInc(delta))
	{
		return false;
	}

	lastViewableLine += delta;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- LogCtrl
// --------------------------------------------------------------------------------
LogCtrl::LogCtrl(PTR(QAbstractScrollArea) pParent, PTR(LogStyle) pStyle)
	: parent_type(pParent)
	, m_pScrollArea(pParent)
	, lineHeight(0)
	, charWidth(0)
	, m_selectedLine(-1)
	, m_pPopupMenu(NULL)
	, fullRepaintLines(0)
	, focusOnly(false)
	, logStyle(pStyle)
	, m_pFindDialog(NULL)
	, drawLog(true)
{
	if (!logStyle)
	{
		logStyle = &studioApp.getStyle()->style_trace;
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

LogCtrl::~LogCtrl()
{
	clear();
}

REF(QScrollBar) LogCtrl::getVertScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->verticalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

REF(QScrollBar) LogCtrl::getHorzScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->horizontalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

void LogCtrl::resizeEvent(QResizeEvent* pEvent)
{
	parent_type::resizeEvent(pEvent);

	m_clientRect = QRect(QPoint(0, 0), pEvent->size());
	updateScrollBars();
}

rbool LogCtrl::getItemColors(int index, LogColorPair* &colors) const
{
	const_cast<CMutex&>(mutex).Lock();

	rbool res = true;
	SubitemColors::List::const_iterator it = m_subitemColors.m_colorList.find(index);
	if (it != m_subitemColors.m_colorList.end())
	{
		colors = (*it).second;
	}
	else
	{
		res = LogCtrl::getItemColors("", colors);
	}

	const_cast<CMutex&>(mutex).Unlock();

	return res;
}

rbool LogCtrl::getItemColors(CREF(tstring) item, LogColorPair* &colors) const
{
	return logStyle->getItemColors(item, colors);
}

void LogCtrl::paintEvent(QPaintEvent* pEvent)
{
	mutex.Lock();

	QPainter painter(this);

	if (drawLog)
	{
		if (!pEvent->rect().isEmpty() && !pEvent->rect().isNull())
		{
			painter.setFont(m_font);

			int firstLine = (std::max)(0, m_SM_Y.position + pEvent->rect().top() / lineHeight);
			int mul = pEvent->rect().bottom() / lineHeight;
			if (pEvent->rect().bottom() > mul * lineHeight)
			{
				mul++;
			}
			int lastLine = (std::min)(m_strings.count() - 1, m_SM_Y.position + mul - 1);

			LogColorPair* colors = NULL;

			int y = lineHeight * (-m_SM_Y.position + firstLine - 1);
			QRect rect(charWidth * (-m_SM_X.position), y, pEvent->rect().width() + charWidth * m_SM_X.position, lineHeight);
			QRect textRect(
				rect.left  () + logStyle->borders->horzBorder,
				rect.top   () + logStyle->borders->vertBorder,
				rect.width () - logStyle->borders->horzBorder * 2,
				rect.height() - logStyle->borders->vertBorder * 2
			);

			StringList::const_iterator it = m_strings.findString(firstLine);
			for (int i = firstLine; i < lastLine + 1; i++)
			{
				if (i != selectedLine() || focusOnly)
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

				rect    .translate(0, lineHeight);
				textRect.translate(0, lineHeight);

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

				if (i == selectedLine() && !focusOnly && colors)
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

	mutex.Unlock();

	parent_type::paintEvent(pEvent);
}

void LogCtrl::onVertScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	scrollVertically(value - m_SM_Y.position);
}

void LogCtrl::onHorzScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	scrollHorizontally(value - m_SM_X.position);
}

void LogCtrl::keyPressEvent(QKeyEvent* pEvent)
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

void LogCtrl::wheelEvent(QWheelEvent* pEvent)
{
	getVertScrollBar().setValue(getVertScrollBar().value() + (pEvent->delta() < 0 ? 1 : -1));
}

void LogCtrl::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		selectLine((std::min)(m_SM_Y.position + pEvent->pos().y() / lineHeight, m_strings.count() - 1));
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}

void LogCtrl::updateScrollBars()
{
	m_SM_Y.pageSize = m_clientRect.height() / lineHeight;
	m_SM_Y.posMax   = (std::max)(0, m_strings.count() - m_SM_Y.pageSize);
	m_SM_Y.position = (std::min)(m_SM_Y.position, m_SM_Y.posMax);

	m_SM_X.pageSize = (m_clientRect.width() - logStyle->borders->horzBorder) / charWidth;
	m_SM_X.posMax   = (std::max)(0, rsint(m_strings.maxLegth()) - m_SM_X.pageSize);
	m_SM_X.position = (std::min)(m_SM_X.position, m_SM_X.posMax);

	int mul = m_SM_Y.pageSize;
	if (mul * lineHeight < m_clientRect.height())
	{
		mul++;
	}
	m_SM_Y.lastViewableLine = m_SM_Y.position + mul - 1;

	m_strings.setCursor(m_SM_Y.position, m_SM_Y.posMax);

	getVertScrollBar().setRange   (0, m_SM_Y.posMax);
	getVertScrollBar().setPageStep(drawLog && m_SM_Y.posMax > 0 ? m_SM_Y.pageSize : 0);
	getVertScrollBar().setValue   (m_SM_Y.position);

	getHorzScrollBar().setRange   (0, m_SM_X.posMax);
	getHorzScrollBar().setPageStep(drawLog && m_SM_X.posMax > 0 ? m_SM_X.pageSize : 0);
	getHorzScrollBar().setValue   (m_SM_X.position);
}

rbool LogCtrl::scrollVertically(int inc)
{
	if (!m_SM_Y.applyInc(inc))
	{
		return false;
	}

	m_strings.setCursor(m_SM_Y.position, m_SM_Y.posMax);
	getVertScrollBar().setValue(m_SM_Y.position);
	updateWindow();
	return true;
}

rbool LogCtrl::scrollHorizontally(int inc)
{
	if (!m_SM_X.applyInc(inc))
	{
		return false;
	}

	getHorzScrollBar().setValue(m_SM_X.position);
	updateWindow();
	return true;
}

rbool LogCtrl::isFullyVisible(int index) const
{
	int lastVisible = m_SM_Y.position + m_clientRect.height() / lineHeight - 1;
	return index <= lastVisible && index >= m_SM_Y.position;
}

void LogCtrl::selectLine(int index)
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

		//makeLineVisible() scrolls to the line and repaints
		//it and nearby line if scrolling occurs.
		//If no scrolling is done repaint line
		rbool needrepaint = !makeLineVisible(selectedLine());
		if (needrepaint)
		{
			repaintLine(selectedLine());
		}

		//repaintLine() repaints line only if it's visible
		repaintLine(prevSel);
	}
}

QRect LogCtrl::getLineRect(int index) const
{
	QRect rect(m_clientRect);
	rect.setTop((index - m_SM_Y.position) * lineHeight);
	rect.setBottom((std::min)(rect.top() + lineHeight, rect.bottom()));
	return rect;
}

void LogCtrl::repaintLine(int index)
{
	if (m_SM_Y.isVisible(index))
	{
		update(getLineRect(index));
		updateWindow();
	}
}

void LogCtrl::updateWindow()
{
	update();
}

rbool LogCtrl::makeLineVisible(int index)
{
	rbool res = false;

	if (isFullyVisible(index))
	{
		return res;
	}

	int inc;
	if (m_SM_Y.position < index)
	{
		int lastVisible = m_SM_Y.position + m_clientRect.height() / lineHeight - 1;
		inc = index - lastVisible;
	}
	else
	{
		inc = index - m_SM_Y.position;
	}

	//Repainting nearby lines after scrolling
	fullRepaintLines = 2;

	res = scrollVertically(inc);

	fullRepaintLines = 0;

	return res;
}

void LogCtrl::addStringToLog(CREF(tstring) logStr)
{
	mutex.Lock();

	if (!logStr.empty())
	{
		int posstart = logStr.find_first_not_of(' ');
		int posend   = logStr.find_first_of(' ', posstart);
		tstring key  = logStr.substr(posstart, posend - posstart);
		rdo::trim(key);

		LogColorPair* colors = NULL;

		if (logStyle->getItemColors(key, colors))
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
			logStyle->getItemColors(key, m_subitemColors.m_parentColor);
		}
	}

	rbool prevVisible = m_SM_Y.isVisible(m_strings.count() - 1);

	m_strings.push_back(logStr);

	int lastString = m_strings.count() - 1;

	if (drawLog)
	{
		updateScrollBars();

		fullRepaintLines = 1;

		if ( selectedLine() != -1 && selectedLine() == lastString - 1)
		{
			setSelectedLine(lastString);
			fullRepaintLines++;
		}

		if (!isFullyVisible(lastString) && prevVisible && (!m_SM_Y.isVisible(selectedLine()) || selectedLine() == lastString))
		{
			//::SendMessage(m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL);
			scrollVertically(m_SM_Y.posMax - m_SM_Y.position);
		}
		else if (m_SM_Y.isVisible(lastString))
		{
			repaintLine(lastString);
			if (fullRepaintLines == 2)
			{
				repaintLine(lastString - 1);
			}
		}

		fullRepaintLines = 0;
	}
	else
	{
		if (selectedLine() != -1 && selectedLine() == lastString - 1)
		{
			setSelectedLine(lastString);
		}
	}

	mutex.Unlock();
}

CREF(LogStyle) LogCtrl::getStyle() const
{
	return *logStyle;
}

void LogCtrl::setStyle(LogStyle* style, rbool needRedraw)
{
	logStyle = style;
	setFont();

	updateScrollBars();

	if (needRedraw)
	{
		update();
		updateWindow();
	}
}

void LogCtrl::setFont()
{
	if (!logStyle)
	{
		return;
	}

	mutex.Lock();

	m_font = QFont(logStyle->font->name.c_str());
	m_font.setBold     (logStyle->theme->style & rdoStyle::RDOStyleFont::BOLD     );
	m_font.setItalic   (logStyle->theme->style & rdoStyle::RDOStyleFont::ITALIC   );
	m_font.setUnderline(logStyle->theme->style & rdoStyle::RDOStyleFont::UNDERLINE);
	m_font.setPointSize(logStyle->font->size);

	QFontMetrics fontMetrics(m_font);
	lineHeight = fontMetrics.height() + 2 * logStyle->borders->vertBorder;
	charWidth  = fontMetrics.averageCharWidth(); // fontMetrics.maxWidth()

	mutex.Unlock();
}

void LogCtrl::getString(int index, tstring& str) const
{
	const_cast<CMutex&>(mutex).Lock();

	if (index >= 0 && index < m_strings.count())
	{
		str.assign(*m_strings.findString(index));
	}

	const_cast<CMutex&>(mutex).Unlock();
}

int LogCtrl::getSelectedIndex() const
{
	return selectedLine();
}

void LogCtrl::getSelected(tstring& str) const
{
	getString(selectedLine(), str);
}

void LogCtrl::clear()
{
	mutex.Lock();

	m_strings.clear();
	m_subitemColors = SubitemColors();

	m_SM_X = ScrollMetric();
	m_SM_Y = ScrollMetricVert();
	setSelectedLine(-1);

	updateScrollBars();
	update();
	updateWindow();

	mutex.Unlock();
}

rsint LogCtrl::find(rbool searchDown)
{
	rsint result = -1;

	mutex.Lock();

	int startPos = selectedLine() == -1
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

	mutex.Unlock();

	return result;
}

void LogCtrl::setText(tstring text)
{
	clear();
	while (!text.empty())
	{
		ruint pos = text.find_first_of("\r\n");
		if (pos == tstring::npos)
		{
			pos = text.length();
		}
		addStringToLog(pos ? text.substr(0, pos) : "");
		text.erase(0, pos);
		text.erase(0, text.find_first_not_of("\r\n"));
	}
}

void LogCtrl::setDrawLog(rbool value)
{
	if (drawLog != value)
	{
		drawLog = value;
		updateScrollBars();
		update();
		updateWindow();
		makeLineVisible(selectedLine());
	}
}

void LogCtrl::onActivate()
{
	repaintLine(selectedLine());

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	pMainWindow->actSearchFind->setEnabled(true);
	pMainWindow->actSearchFindNext->setEnabled(true);
	pMainWindow->actSearchFindPrevious->setEnabled(true);
	connect(pMainWindow->actSearchFind,         SIGNAL(triggered(bool)), this, SLOT(onSearchFind()));
	connect(pMainWindow->actSearchFindNext,     SIGNAL(triggered(bool)), this, SLOT(onSearchFindNext()));
	connect(pMainWindow->actSearchFindPrevious, SIGNAL(triggered(bool)), this, SLOT(onSearchFindPrevious()));
	connect(pMainWindow->actHelpContext,        SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));

	setUpActionEditCopy(true);
	setUpCoordStatusBar(true);

	{
		RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
		ASSERT(pMainWindow);
		pMainWindow->updateStatusBar<RDOStudioMainFrame::SB_MODIFY>("Только чтение");
	}
}

void LogCtrl::onDeactivate()
{
	repaintLine(selectedLine());

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	pMainWindow->actSearchFind->setEnabled(false);
	pMainWindow->actSearchFindNext->setEnabled(false);
	pMainWindow->actSearchFindPrevious->setEnabled(false);
	disconnect(pMainWindow->actSearchFind,         SIGNAL(triggered(bool)), this, SLOT(onSearchFind()));
	disconnect(pMainWindow->actSearchFindNext,     SIGNAL(triggered(bool)), this, SLOT(onSearchFindNext()));
	disconnect(pMainWindow->actSearchFindPrevious, SIGNAL(triggered(bool)), this, SLOT(onSearchFindPrevious()));
	disconnect(pMainWindow->actHelpContext,        SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));

	setUpActionEditCopy(false);
	setUpCoordStatusBar(false);
	{
		RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
		ASSERT(pMainWindow);
		pMainWindow->updateStatusBar<RDOStudioMainFrame::SB_MODIFY>("");
	}
}

void LogCtrl::setUpActionEditCopy(rbool activate)
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

rbool LogCtrl::canCopy() const
{
	return selectedLine() != -1;
}

rsint LogCtrl::selectedLine() const
{
	return m_selectedLine;
}

void LogCtrl::setSelectedLine(rsint selectedLine)
{
	m_selectedLine = selectedLine;
	setUpActionEditCopy(isActivated());
	setUpCoordStatusBar(isActivated());
}

void LogCtrl::setUpCoordStatusBar(rbool activate)
{
	QString coord = activate && selectedLine() != -1
		? QString("1 : %1").arg(selectedLine())
		: QString();

	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);
	pMainWindow->updateStatusBar<RDOStudioMainFrame::SB_COORD>(coord);
}

void LogCtrl::onEditCopy()
{
	tstring selected;
	getSelected(selected);

	QApplication::clipboard()->setText(QString::fromStdString(selected));
}

void LogCtrl::onSearchFind()
{
	getSelected(m_findSettings.what);

	if (!m_pFindDialog)
	{
		m_pFindDialog = new FindDialog(
			this,
			boost::bind(&LogCtrl::onFindDlgFind, this, _1),
			boost::bind(&LogCtrl::onFindDlgClose, this)
		);
	}

	m_pFindDialog->setSettings(m_findSettings);
	m_pFindDialog->show();
	m_pFindDialog->raise();
	m_pFindDialog->activateWindow();
}

void LogCtrl::onFindDlgClose()
{
	m_pFindDialog = NULL;
}

void LogCtrl::onFindDlgFind(CREF(FindDialog::Settings) settings)
{
	m_findSettings = settings;
	onSearchFindNext();
}

void LogCtrl::onSearchFindNext()
{
	selectLine(find(m_findSettings.searchDown));
}

void LogCtrl::onSearchFindPrevious()
{
	selectLine(find(!m_findSettings.searchDown));
}

void LogCtrl::onHelpContext()
{
	tstring line;

	getSelected(line);

	tstring keyword = "trc";
	if (!line.empty())
	{
		int posstart = line.find_first_not_of(' ');
		int posend   = line.find_first_of(' ', posstart);
		keyword      = line.substr(posstart, posend - posstart);
		rdo::trim(keyword);

		if (!keyword.empty())
		{
			LogColorPair* colors;
			if (!logStyle->getItemColors(keyword, colors))
			{
				getItemColors(selectedLine(), colors);
				if (*colors == static_cast<TracerLogTheme*>(logStyle->theme)->sd)
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
