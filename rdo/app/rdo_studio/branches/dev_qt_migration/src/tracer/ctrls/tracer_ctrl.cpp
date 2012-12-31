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
#include <QtGui/qpainter.h>
#include <QtGui/qscrollbar.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;
using namespace rdoStyle;

namespace rdoTracerLog
{

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrlFindInList
// --------------------------------------------------------------------------------
class RDOLogCtrlFindInList
{
	RDOLogCtrl* log;
	tstring strToFind;
	rbool matchCase;
	rbool matchWholeWord;

	rbool scan(tstring::iterator &wildCards, tstring::iterator &wildend, tstring::iterator &str, tstring::iterator &strend) const;
	rbool match(tstring::iterator &wildcards, tstring::iterator &wildend, tstring::iterator &strcomp, tstring::iterator &strend) const;
public:
	RDOLogCtrlFindInList(RDOLogCtrl* _log, tstring _strToFind, rbool _matchCase, rbool _matchWholeWord);
	rbool operator()(tstring nextstr);
};

}; // namespace rdoTracerLog

RDOLogCtrlFindInList::RDOLogCtrlFindInList(RDOLogCtrl* _log, tstring _strToFind, rbool _matchCase, rbool _matchWholeWord)
	: log(_log),
	  strToFind(_strToFind),
	  matchCase(_matchCase),
	  matchWholeWord(_matchWholeWord)
{
}

rbool RDOLogCtrlFindInList::scan(tstring::iterator &wildCards, tstring::iterator &wildend, tstring::iterator &str, tstring::iterator &strend) const
{
	// remove the '?' and '*'
	for(wildCards ++; str != strend && (*wildCards == '?' || *wildCards == '*'); wildCards ++)
		if (*wildCards == '?')
		{
			str ++;
		}
	while (*wildCards == '*')
	{
		wildCards ++;
	}

	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return
	if (str == strend && wildCards != wildend)
	{
		return false;
	}
	if (str == strend && wildCards == wildend)
	{
		return true;
	}
	// else search substring
	else
	{
		tstring::iterator wdsCopy = wildCards;
		tstring::iterator strCopy = str;
		rbool res = 1;
		do
		{
			if (!match(wildCards, wildend, str, strend))
			{
				strCopy ++;
			}
			wildCards = wdsCopy;
			str		  = strCopy;
			while ((*wildCards != *str) && (str != strend))
			{
				str ++;
			}
			wdsCopy = wildCards;
			strCopy = str;
		}
		while ((str != strend) ? !match(wildCards, wildend, str, strend) : (res = false) != false);

		if (str == strend && wildCards == wildend)
		{
			return true;
		}

		return res;
	}
}

rbool RDOLogCtrlFindInList::match(tstring::iterator &wildcards, tstring::iterator &wildend, tstring::iterator &strcomp, tstring::iterator &strend) const
{
	rbool res = true;

	tstring strWild;
	tstring strComp;
	if (wildcards != wildend)
	{
		strWild.assign(&(*wildcards));
	}
	if (strcomp != strend)
	{
		strComp.assign(&(*strcomp));
	}
	tstring::iterator strWildb = strWild.begin();
	tstring::iterator strWilde = strWild.end();
	tstring::iterator strCompb = strComp.begin();
	tstring::iterator strCompe = strComp.end();

	//iterate and delete '?' and '*' one by one
	while(strWildb != strWilde && res && strCompb != strCompe)
	{
		if (*strWildb == '?')
		{
			strCompb ++;
		}
		else if (*strWildb == '*')
		{
			res = scan(strWildb, strWilde, strCompb, strCompe);
			strWildb --;
		}
		else
		{
			res = (*strWildb == *strCompb);
			strCompb ++;
		}
		strWildb ++;
	}
	while (*strWildb && *strWildb == '*' && res)
	{
		strWildb ++;
	}

	return res && strCompb == strCompe && strWildb == strWilde;
}

rbool RDOLogCtrlFindInList::operator()(tstring nextstr)
{
	if (!matchWholeWord && strToFind.find_first_of("*?") == tstring::npos)
	{
		strToFind.insert(0, "*");
		strToFind += "*";
	}

	tstring str = nextstr;

	if (!matchCase)
	{
		std::transform(strToFind.begin(), strToFind.end(), strToFind.begin(), tolower);
		std::transform(str.begin(), str.end(), str.begin(), tolower);
	}

	log->posFind ++;

	if (matchWholeWord)
	{
		return strToFind == str;
	}

	tstring::iterator findstrb = strToFind.begin();
	tstring::iterator findstre = strToFind.end();
	tstring::iterator strb = str.begin();
	tstring::iterator stre = str.end();
	return match(findstrb, findstre,  strb, stre);
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrl::StringList
// --------------------------------------------------------------------------------
RDOLogCtrl::StringList::StringList()
	: m_count   (0)
	, m_cursor  (0)
	, m_cursorIt(m_list.end())
{}

void RDOLogCtrl::StringList::push_back(CREF(tstring) value)
{
	m_list.push_back(value);
	++m_count;

	if (m_count == 1)
	{
		m_cursorIt = m_list.begin();
	}
}

RDOLogCtrl::StringList::const_iterator RDOLogCtrl::StringList::begin() const
{
	return const_cast<StringList*>(this)->begin();
}

RDOLogCtrl::StringList::iterator RDOLogCtrl::StringList::begin()
{
	return m_list.end();
}

RDOLogCtrl::StringList::const_iterator RDOLogCtrl::StringList::end() const
{
	return const_cast<StringList*>(this)->end();
}

RDOLogCtrl::StringList::iterator RDOLogCtrl::StringList::end()
{
	return m_list.end();
}

RDOLogCtrl::StringList::const_reverse_iterator RDOLogCtrl::StringList::rbegin() const
{
	return m_list.rbegin();
}

RDOLogCtrl::StringList::reverse_iterator RDOLogCtrl::StringList::rbegin()
{
	return m_list.rbegin();
}

RDOLogCtrl::StringList::const_reverse_iterator RDOLogCtrl::StringList::rend() const
{
	return m_list.rend();
}

RDOLogCtrl::StringList::reverse_iterator RDOLogCtrl::StringList::rend()
{
	return m_list.rend();
}

void RDOLogCtrl::StringList::clear()
{
	m_list.clear();
	m_count = 0;
}

rsint RDOLogCtrl::StringList::count() const
{
	return m_count;
}

void RDOLogCtrl::StringList::setCursor(rsint pos, rsint max)
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

RDOLogCtrl::StringList::const_iterator RDOLogCtrl::StringList::findString(int index) const
{
	return const_cast<StringList*>(this)->findString(index);
}

RDOLogCtrl::StringList::iterator RDOLogCtrl::StringList::findString(int index)
{
	iterator res;

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
		seek(delta, res);
	}

	return res;
}

RDOLogCtrl::StringList::reverse_iterator RDOLogCtrl::StringList::rFindString(int index)
{
	reverse_iterator rit(findString(index));
	return rit;
}

void RDOLogCtrl::StringList::seek(rsint delta, REF(StringList::const_iterator) it) const
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
// -------------------- RDOLogCtrl
// --------------------------------------------------------------------------------
RDOLogCtrl::RDOLogCtrl(PTR(QAbstractScrollArea) pParent, PTR(RDOLogStyle) pStyle)
	: parent_type(pParent)
	, m_pScrollArea(pParent)
	, lineHeight(0)
	, charWidth(0)
	, maxStrWidth(0)
	, xPos(0)
	, yPos(0)
	, xMax(0)
	, yMax(0)
	, xPageSize(0)
	, yPageSize(0)
	, lastViewableLine(0)
	, selectedLine(-1)
	, fullRepaintLines(0)
	, focusOnly(false)
	, logStyle(pStyle)
	, firstFoundLine(-1)
	, posFind(-1)
	, bHaveFound(false)
	, bSearchDown(true)
	, bMatchCase(false)
	, bMatchWholeWord(false)
	, drawLog(true)
	, m_prevVertSBValue(0)
	, m_prevHorzSBValue(0)
{
	if (!logStyle)
	{
		logStyle = &studioApp.getStyle()->style_trace;
	}

	connect(&getVertScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVertScrollBarValueChanged(int)));
	connect(&getHorzScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorzScrollBarValueChanged(int)));

	setFocusPolicy(Qt::ClickFocus);

	setFont();
	updateScrollBars();
}

RDOLogCtrl::~RDOLogCtrl()
{
	clear();
}

REF(QScrollBar) RDOLogCtrl::getVertScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->verticalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

REF(QScrollBar) RDOLogCtrl::getHorzScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->horizontalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

void RDOLogCtrl::resizeEvent(QResizeEvent* pEvent)
{
	parent_type::resizeEvent(pEvent);

	QRect prevClientRect(m_clientRect);
	m_clientRect = QRect(QPoint(0, 0), pEvent->size());

	QRect newWindowRect(
		mapToGlobal(m_clientRect.topLeft()),
		mapToGlobal(m_clientRect.bottomRight())
	);

	int prevYPos = yPos;
	int prevXPos = xPos;
	updateScrollBars();

	rbool lastLineVisible = isFullyVisible(m_strings.count() - 1);
	rbool lastCharVisible = maxStrWidth == xPos + m_clientRect.width() / charWidth;

	rbool fullVisibleVert = !yPos && lastLineVisible;
	rbool fullVisibleHorz = !xPos && lastCharVisible;

	rbool needShiftVert = yPos < prevYPos && !fullVisibleVert;
	rbool needShiftHorz = xPos < prevXPos && !fullVisibleHorz;

	rbool topChanged = m_prevWindowRect.top() != newWindowRect.top();
	int dx = m_clientRect.right() - prevClientRect.right();
	int dy = m_clientRect.bottom() - prevClientRect.bottom();

	int mul = m_clientRect.height() / lineHeight;
	if (mul * lineHeight < m_clientRect.height())
	{
		++mul;
	}
	lastViewableLine = yPos + mul - 1;

	m_prevWindowRect = newWindowRect;

	if (!topChanged)
	{
		update(m_clientRect);

		if (dx < 0 && dy < 0)
		{
			return;
		}

		QRegion bottomRgn = dy
			? QRegion(m_clientRect.left(), prevClientRect.bottom() - 1, m_clientRect.right(), m_clientRect.bottom())
			: QRegion(0, 0, 0, 0);

		//Substracting 1 pixel to remove old focus rectangle.
		QRegion rightRgn = dx
			? QRegion(prevClientRect.right() - 1, m_clientRect.top(), m_clientRect.right(), m_clientRect.bottom())
			: QRegion(0, 0, 0, 0);

		QRegion invalidRgn = bottomRgn.united(rightRgn);

		if (invalidRgn.isEmpty())
		{
			invalidRgn = QRegion(m_clientRect.left(), m_clientRect.top(), m_clientRect.right(), m_clientRect.bottom());
		}
		else if (needShiftVert || needShiftHorz)
		{
			if (needShiftVert)
			{
				getVertScrollBar().setValue(lineHeight * (prevYPos - yPos));

				if (dx)
				{
					update(rightRgn);
				}
			}

			if (needShiftHorz)
			{
				getHorzScrollBar().setValue(charWidth * (prevXPos - xPos));

				if (isVisible(selectedLine))
				{
					update(getLineRect(selectedLine));
				}

				if (dy)
				{
					update(bottomRgn);
				}
			}
			return;
		}

		update(invalidRgn);
	}
	else
	{
		update(m_clientRect);
	}
}

rbool RDOLogCtrl::getItemColors(int index, RDOLogColorPair* &colors) const
{
	return logStyle->getItemColors(index, colors);
}

rbool RDOLogCtrl::getItemColors(CREF(tstring) item, RDOLogColorPair* &colors) const
{
	return logStyle->getItemColors(item, colors);
}

void RDOLogCtrl::paintEvent(QPaintEvent* pEvent)
{
	mutex.Lock();

	QPainter painter(this);

	if (drawLog)
	{
		if (!pEvent->rect().isEmpty() && !pEvent->rect().isNull())
		{
			painter.setFont(m_font);

			int firstLine = (std::max)(0, yPos + pEvent->rect().top() / lineHeight);
			int mul = pEvent->rect().bottom() / lineHeight;
			if (pEvent->rect().bottom() > mul * lineHeight)
			{
				mul++;
			}
			int lastLine = (std::min)(m_strings.count() - 1, yPos + mul - 1);

			RDOLogColorPair* colors = NULL;

			int y = lineHeight * (-yPos + firstLine - 1);
			QRect rect(charWidth * (-xPos), y, pEvent->rect().width() + charWidth * xPos, lineHeight);
			QRect textRect(
				rect.left  () + logStyle->borders->horzBorder,
				rect.top   () + logStyle->borders->vertBorder,
				rect.width () - logStyle->borders->horzBorder * 2,
				rect.height() - logStyle->borders->vertBorder * 2
			);

			StringList::const_iterator it = m_strings.findString(firstLine);
			for (int i = firstLine; i < lastLine + 1; i++)
			{
				if (i != selectedLine || focusOnly)
				{
					if (!getItemColors((*it), colors))
					{
						getItemColors(i, colors);
					}
				}
				else
				{
					colors = new RDOLogColorPair();
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

				if (i == selectedLine && hasFocus())
				{
					QRect focusRect(m_clientRect);
					focusRect.setTop   (rect.top   ());
					focusRect.setBottom(rect.bottom());
					QStyleOptionFocusRect option;
					option.initFrom(this);
					option.backgroundColor = palette().color(QPalette::Background);
					option.rect = focusRect;
					style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
				}

				++it;

				if (i == selectedLine && !focusOnly && colors)
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
		RDOLogColorPair* colors = NULL;
		getItemColors("", colors);

		painter.fillRect(
			m_clientRect,
			colors->backgroundColor
		);
	}

	mutex.Unlock();

	parent_type::paintEvent(pEvent);
}

void RDOLogCtrl::onVertScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	int inc = value - m_prevVertSBValue;
	TRACE3("onVertScrollBarValueChanged %d, %d, %d\n", value, m_prevVertSBValue, inc);
	m_prevVertSBValue = value;
	scrollVertically(inc);
}

void RDOLogCtrl::onHorzScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	int inc = value - m_prevHorzSBValue;
	TRACE3("onHorzScrollBarValueChanged %d, %d, %d\n", value, m_prevVertSBValue, inc);
	m_prevHorzSBValue = value;
	scrollHorizontally(inc);
}

//! @todo qt
//void RDOLogCtrl::OnSetFocus(CWnd* pOldWnd)
//{
//	CWnd::OnSetFocus(pOldWnd);
//	repaintLine(selectedLine);
//}
//
//void RDOLogCtrl::OnKillFocus(CWnd* pNewWnd)
//{
//	CWnd::OnKillFocus(pNewWnd);
//	repaintLine(selectedLine);
//}

void RDOLogCtrl::keyPressEvent(QKeyEvent* pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Up:
		selectLine(selectedLine - 1);
		break;

	case Qt::Key_PageUp:
		selectLine((std::max)(selectedLine - yPageSize, 0));
		break;

	case Qt::Key_PageDown:
		selectLine((std::min)(selectedLine + yPageSize, m_strings.count() - 1));
		break;

	case Qt::Key_Down:
		selectLine(selectedLine + 1);
		break;

	case Qt::Key_Home:
		selectLine(0);
		break;

	case Qt::Key_End:
		selectLine(m_strings.count() - 1);
		break;

	case Qt::Key_Left:
		getHorzScrollBar().setValue(getHorzScrollBar().value() - 1);
		break;

	case Qt::Key_Right:
		getHorzScrollBar().setValue(getHorzScrollBar().value() + 1);
		break;

	default:
		break;
	}
}

void RDOLogCtrl::wheelEvent(QWheelEvent* pEvent)
{
	getVertScrollBar().setValue(getVertScrollBar().value() + (pEvent->delta() < 0 ? 1 : -1));
}

void RDOLogCtrl::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		//! @todo qt
		//	SetFocus();
		selectLine((std::min)(yPos + pEvent->pos().y() / lineHeight, m_strings.count() - 1));
	}
}

void RDOLogCtrl::recalcWidth(int newMaxStrWidth)
{
	if (maxStrWidth < newMaxStrWidth)
	{
		maxStrWidth = newMaxStrWidth;
	}
}

void RDOLogCtrl::updateScrollBars()
{
	yPageSize = m_clientRect.height() / lineHeight;
	yMax = (std::max)(0, m_strings.count() - yPageSize);
	yPos = (std::min)(yPos, yMax);

	xPageSize = (m_clientRect.width() - logStyle->borders->horzBorder) / charWidth;
	xMax = (std::max)(0, maxStrWidth - xPageSize);
	xPos = (std::min)(xPos, xMax);

	int mul = yPageSize;
	if (mul * lineHeight < m_clientRect.height())
	{
		mul++;
	}
	lastViewableLine = yPos + mul - 1;

	m_strings.setCursor(yPos, yMax);

	getVertScrollBar().setRange   (0, yMax);
	getVertScrollBar().setPageStep(drawLog && yMax > 0 ? yPageSize : 0);
	getVertScrollBar().setValue   (yPos);

	getHorzScrollBar().setRange   (0, xMax);
	getHorzScrollBar().setPageStep(drawLog && xMax > 0 ? xPageSize : 0);
	getHorzScrollBar().setValue   (xPos);
}

rbool RDOLogCtrl::scrollVertically(int inc)
{
	rbool res = false;
	if (!inc)
	{
		return res;
	}

	// If applying the vertical scrolling increment does not
	// take the scrolling position out of the scrolling range,
	// increment the scrolling position, adjust the position
	// of the scroll box, and update the window.
	if (inc == (std::max)(-yPos, (std::min)(inc, yMax - yPos)))
	{
		yPos += inc;
		m_strings.setCursor(yPos, yMax);
		lastViewableLine += inc;

		updateWindow();
		res = true;

		TRACE1("scrollVertically %d ok\n", inc);
	}
	else
	{
		TRACE1("scrollVertically %d error\n", inc);
	}

	return res;
}

rbool RDOLogCtrl::scrollHorizontally(int inc)
{
	rbool res = false;
	if (!inc)
	{
		return res;
	}

	// If applying the horizontal scrolling increment does not
	// take the scrolling position out of the scrolling range,
	// increment the scrolling position, adjust the position
	// of the scroll box, and update the window.
	if (inc == (std::max)(-xPos, (std::min)(inc, xMax - xPos)))
	{
		xPos += inc;

		if (isVisible(selectedLine))
		{
			update(getLineRect(selectedLine));
		}

		updateWindow();
		res = true;
	}
	return res;
}

rbool RDOLogCtrl::isVisible(int index) const
{
	return index <= lastViewableLine && index >= yPos;
}

rbool RDOLogCtrl::isFullyVisible(int index) const
{
	int lastVisible = yPos + m_clientRect.height() / lineHeight - 1;
	return index <= lastVisible && index >= yPos;
}

void RDOLogCtrl::selectLine(int index)
{
	if (index < 0 || index > m_strings.count() - 1 || index == selectedLine)
	{
		return;
	}

	int prevSel = selectedLine;
	int inc = (std::max)(-prevSel, (std::min)(index - prevSel, m_strings.count() - 1 - prevSel));

	if (inc)
	{
		selectedLine += inc;

		//makeLineVisible() scrolls to the line and repaints
		//it and nearby line if scrolling occurs.
		//If no scrolling is done repaint line
		rbool needrepaint = !makeLineVisible(selectedLine);
		if (needrepaint)
		{
			repaintLine(selectedLine);
		}

		//repaintLine() repaints line only if it's visible
		repaintLine(prevSel);
	}
}

QRect RDOLogCtrl::getLineRect(int index) const
{
	QRect rect(m_clientRect);
	rect.setTop((index - yPos) * lineHeight);
	rect.setBottom((std::min)(rect.top() + lineHeight, rect.bottom()));
	return rect;
}

void RDOLogCtrl::repaintLine(int index)
{
	if (isVisible(index))
	{
		update(getLineRect(index));
		updateWindow();
	}
}

void RDOLogCtrl::updateWindow()
{
	update();
}

rbool RDOLogCtrl::makeLineVisible(int index)
{
	rbool res = false;

	if (isFullyVisible(index))
	{
		return res;
	}

	int inc;
	if (yPos < index)
	{
		int lastVisible = yPos + m_clientRect.height() / lineHeight - 1;
		inc = index - lastVisible;
	}
	else
	{
		inc = index - yPos;
	}

	//Repainting nearby lines after scrolling
	fullRepaintLines = 2;

	res = scrollVertically(inc);

	fullRepaintLines = 0;

	return res;
}

void RDOLogCtrl::addStringToLog(CREF(tstring) logStr)
{
	mutex.Lock();

//! @todo qt
	//if (!hwnd)
	//	return;

	rbool prevVisible = isVisible(m_strings.count() - 1);

	m_strings.push_back(logStr);

	recalcWidth(logStr.length());

	int lastString = m_strings.count() - 1;

	if (drawLog)
	{
		updateScrollBars();

		fullRepaintLines = 1;

		if ( selectedLine != -1 && selectedLine == lastString - 1)
		{
			selectedLine = lastString;
			fullRepaintLines ++;
		}

		if (!isFullyVisible(lastString) && prevVisible && (!isVisible(selectedLine) || selectedLine == lastString))
		{
			//::SendMessage(m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL);
			scrollVertically(yMax - yPos);
		}
		else if (isVisible(lastString))
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
		if (selectedLine != -1 && selectedLine == lastString - 1)
		{
			selectedLine = lastString;
		}
	}

	mutex.Unlock();
}

CREF(RDOLogStyle) RDOLogCtrl::getStyle() const
{
	return *logStyle;
}

void RDOLogCtrl::setStyle(RDOLogStyle* style, rbool needRedraw)
{
	logStyle = style;
	setFont();

	recalcWidth(maxStrWidth);
	updateScrollBars();

	if (needRedraw)
	{
		update();
		updateWindow();
	}
}

void RDOLogCtrl::setFont()
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

void RDOLogCtrl::getString(int index, tstring& str) const
{
	const_cast<CMutex&>(mutex).Lock();

	tstring res = "";

	if (index >= 0 && index < m_strings.count())
	{
		str.assign(*m_strings.findString(index));
	}

	const_cast<CMutex&>(mutex).Unlock();
}

int RDOLogCtrl::getSelectedIndex() const
{
	return selectedLine;
}

void RDOLogCtrl::getSelected(tstring& str) const
{
	getString(selectedLine, str);
}

void RDOLogCtrl::copy()
{
//! @todo qt
//	if (canCopy()) {
//		if (!OpenClipboard() || !::EmptyClipboard())
//			return;
//		tstring str;
//		getSelected(str);
//		char* ptr = (char*)::LocalAlloc(LMEM_FIXED, str.length() + 1);
//#pragma warning(disable: 4996)
//		strcpy(ptr, str.c_str());
//#pragma warning(default: 4996)
//		::SetClipboardData(CF_TEXT, ptr);
//		CloseClipboard();
//	}
}

void RDOLogCtrl::clear()
{
	mutex.Lock();

	m_strings.clear();

	maxStrWidth       = 0;
	lastViewableLine  = 0;
	selectedLine = -1;

	updateScrollBars();
	update();
	updateWindow();

	mutex.Unlock();
}

void RDOLogCtrl::find(int& result, rbool searchDown, rbool matchCase, rbool matchWholeWord)
{
	mutex.Lock();

	result = -1;

	tstring strtofind = findStr;

	StringList::iterator it;
	StringList::reverse_iterator it_r;

	int startPos = selectedLine + 1;
	int endPos = m_strings.count() - 1;
	if (!searchDown)
	{
		startPos = selectedLine - 1;
		endPos   = 0;
	}

	posFind = -1;
	if (searchDown)
	{
		it = std::find_if(
			m_strings.findString(startPos),
			m_strings.end(),
			RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
		);
		if (it == m_strings.end())
		{
			posFind = -1;
			startPos = 0;
			endPos   = m_strings.count() - 1;
			it = std::find_if(
				m_strings.begin(),
				m_strings.end(),
				RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
			);
		}
		if (it == m_strings.end())
		{
			posFind = -1;
		}
		else
		{
			posFind += startPos;
		}
	}
	else
	{
		it_r = std::find_if(
			m_strings.rFindString(startPos + 1),
			m_strings.rend(),
			RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
		);
		if (it_r == m_strings.rend())
		{
			posFind = -1;
			startPos = m_strings.count() - 1;
			endPos   = 0;
			it_r = std::find_if(
				m_strings.rbegin(),
				m_strings.rend(),
				RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
			);
		}
		posFind = it_r == m_strings.rend()
			? -1
			: startPos - posFind;
	}

	if (posFind == -1)
	{
		firstFoundLine = -1;
		bHaveFound    = false;
		result = -1;
	}
	else
	{
		bHaveFound = true;
		result = posFind;
		if (firstFoundLine == -1)
		{
			firstFoundLine = posFind;
		}
		else if (posFind == firstFoundLine)
		{
			firstFoundLine = -1;
			bHaveFound    = false;
			result = -1;
		}
	}

	mutex.Unlock();
}

void RDOLogCtrl::setText(tstring text)
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

void RDOLogCtrl::setDrawLog(rbool value)
{
	if (drawLog != value)
	{
		drawLog = value;
		updateScrollBars();
		update();
		updateWindow();
		makeLineVisible(selectedLine);
	}
}
