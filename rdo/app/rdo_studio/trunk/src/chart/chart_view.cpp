/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view.cpp
  \author    Захаров Павел
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
#include <QtCore/qprocess.h>
#include <QtGui/qevent.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_view.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_windows_base.h"
#include "app/rdo_studio/resource.h"
#include "app/rdo_studio/src/chart/chart_view_style.h"
#include "app/rdo_studio/src/chart/chart_serie.h"
#include "app/rdo_studio/src/chart/chart_options.h"
#include "app/rdo_studio/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- ChartView
// --------------------------------------------------------------------------------
//BEGIN_MESSAGE_MAP(ChartView, CWnd)
//	ON_WM_ERASEBKGND()
//	ON_WM_HSCROLL()
//	ON_WM_KEYDOWN()
//	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
//	ON_WM_MOUSEACTIVATE()
//	ON_COMMAND(ID_CHART_OPTIONS, OnChartOptions)
//	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
//END_MESSAGE_MAP()

ChartView::ChartView(QWidget* pParent, RDOStudioChartDoc* pDocument, const rbool preview)
	: super(pParent)
	, m_pDocument(pDocument)
	, m_bmpRect(0, 0, 0, 0)
	, m_clientRect(0, 0, 0, 0)
	, m_pddSerie(NULL)
	, m_valueCountX(5)
	, m_valueCountY(5)
	, m_pYAxis(NULL)
	, m_timeWrapFlag(true)
	, m_chartRect(0, 0, 0, 0)
	, m_xMax(0)
	, m_xPos(0)
	, m_timeScale(0)
	, m_drawFromEventIndex(0)
	, m_drawToEventCount(0)
	, m_chartShift(0)
	, m_zoom(1)
	, m_zoomOld(1)
	, m_zoomAuto(1)
	, m_zoomAutoFlag(false)
	, m_scaleKoeff(1)
	, m_pStyle(NULL)
	, m_previewMode(preview)
	, m_legendRect(0, 0, 0, 0)
	, m_needDrawLegend(true)
	, m_pPopupMenu(NULL)
{
	if (m_previewMode)
		m_timeWrapFlag = false;

	//! @todo qt
	//if (!m_ddTarget.Register(this))
	//	return -1;

	if (!m_previewMode)
	{
		setStyle(&studioApp.getStyle()->style_chart, false);
	}

	if (getDocument())
	{
		recalcLayout();
		updateScrollBars();
	}

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	m_pPopupMenu = new QMenu(pParent);
	//m_pPopupMenu->addAction(pMainWindow->actEditCopy);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actViewZoomInc);
	m_pPopupMenu->addAction(pMainWindow->actViewZoomDec);
	m_pPopupMenu->addAction(pMainWindow->actViewZoomAuto);
	m_pPopupMenu->addAction(pMainWindow->actViewZoomReset);
	m_pPopupMenu->addSeparator();
	m_pPopupMenu->addAction(pMainWindow->actChartTimeWrap);
	m_pPopupMenu->addSeparator();
	//m_pPopupMenu->addAction(pMainWindow->actChartOptions);

	attachToDoc();
}

ChartView::~ChartView()
{
	if (getDocument())
	{
		getDocument()->removeFromViews(this);
	}
	delete m_pDocument;
}

void ChartView::recalcLayout()
{
	RDOStudioChartDoc* doc = getDocument();
	doc->m_mutex.Lock();

	m_mutex.Lock();

	QFontMetrics titleFontMetrics(m_fontTitle);
	tstring str = doc->getTitle();
	QSize titleSize = !str.empty()
		? titleFontMetrics.size(Qt::TextSingleLine, QString::fromLocal8Bit(str.c_str()))
		: QSize(0, 0);
	m_chartRect.setTop(titleSize.height() + 5);

	QFontMetrics axisFontMetrics(m_fontAxis);
	QSize sizeMax(0, 0);
	if (m_pYAxis)
	{
		m_pYAxis->getCaptions(m_captionList, m_valueCountY);
		for (std::vector<tstring>::iterator it = m_captionList.begin(); it != m_captionList.end(); ++it)
		{
			QRect rect = axisFontMetrics.boundingRect(QString::fromLocal8Bit(it->c_str()));
			if (rect.width() > sizeMax.width())
			{
				sizeMax.setWidth(rect.width());
			}
		}
	}
	m_chartRect.setLeft(sizeMax.width() + 10);

	str = !doc->m_docTimes.empty()
		? rdo::format("%.3f", doc->m_docTimes.back()->time)
		: rdo::format("%.3f", 0);
	QSize size = axisFontMetrics.boundingRect(QString::fromLocal8Bit(str.c_str())).size();
	m_chartRect.setRight(m_clientRect.right() - size.width() - 5);
	m_chartRect.setBottom(m_clientRect.bottom() - size.height() - 7);

	if (m_needDrawLegend)
	{
		QFontMetrics legendFontMetrics(m_fontLegend);
		int count = 0;
		for (std::vector<ChartSerie*>::iterator it = doc->m_serieList.begin(); it != doc->m_serieList.end(); ++it)
		{
			size = (*it)->getLegendSize(legendFontMetrics, m_chartRect);
			if (size.width() && size.height())
			{
				if (size.width() > sizeMax.width())
				{
					sizeMax.setWidth(size.width());
				}
				if (size.height() > sizeMax.height())
				{
					sizeMax.setHeight(size.height());
				}
				count++;
			}
		}

		m_legendRect.setTop(m_chartRect.top());
		m_legendRect.setBottom(m_legendRect.top() + count * sizeMax.height() + 3 + doc->getMaxMarkerSize() / 2);
		m_legendRect.setLeft(m_chartRect.left() + (m_chartRect.width() - sizeMax.width()) / 2);
		m_legendRect.setRight(m_legendRect.left() + sizeMax.width());
		m_chartRect.setTop(m_chartRect.top() + m_legendRect.height());
	}

	if (!doc->m_docTimes.empty())
	{
		double timeRange = doc->m_docTimes.back()->time - doc->m_docTimes.front()->time;
		if (timeRange > 0)
		{
			long double timeScaleAuto = doUnwrapTime() ? (double)(m_chartRect.width() - m_pStyle->pFontsTicks->tickWidth * doc->m_ticksCount) / timeRange : (double)m_chartRect.width() / timeRange;
			m_timeScale = (double)m_pStyle->pFontsTicks->tickWidth / doc->m_minTimeOffset;
			m_zoomAuto = double(timeScaleAuto) / double(m_timeScale);
			/*if (doUnwrapTime() && auto_zoom < 0) {
			 auto_zoom = 1;
			 }*/
			if (m_zoomAutoFlag || (m_zoomAuto > 1 /*&& scale_koeff < auto_zoom*/))
			{
				m_scaleKoeff = m_zoomAuto;
			}
			if (!m_zoomAutoFlag && m_zoomAuto <= 1)
			{
				m_scaleKoeff = m_zoom;
			}
		}
		else
		{
			m_timeScale = 0;
			m_scaleKoeff = 1;
		}
	}
	else
	{
		m_timeScale = 0;
		m_scaleKoeff = 1;
	}

	m_timeScale *= m_scaleKoeff;

	m_mutex.Unlock();

	doc->m_mutex.Unlock();
}

void ChartView::setScrollPos(UINT nSBCode, UINT nPos, const rbool needUpdate)
{
	if (nSBCode == SB_HORZ)
		m_xPos = nPos;

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS;
	si.nPos = m_xPos;
	//! @todo qt
	//SetScrollInfo(nSBCode, &si, TRUE);

	if (needUpdate)
	{
		update();
	}
}

void ChartView::updateScrollBars(const rbool /*needUpdate toto qt*/)
{
	RDOStudioChartDoc* doc = getDocument();
	doc->m_mutex.Lock();

	int size;
	if (!doc->m_docTimes.empty())
	{
		size = roundDouble((doc->m_docTimes.back()->time - doc->m_docTimes.front()->time) * double(m_timeScale));
		if (doUnwrapTime())
		{
			size += m_pStyle->pFontsTicks->tickWidth * doc->m_ticksCount;
		}
	}
	else
	{
		size = 0;
	}

	m_xMax = std::max(0, size - m_chartRect.width());
	m_xPos = std::min(m_xPos, m_xMax);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = size - 1;
	si.nPage = m_chartRect.width();
	si.nPos = m_xPos;
	//! @todo qt
	//SetScrollInfo(SB_HORZ, &si, needUpdate);

	doc->m_mutex.Unlock();
}

rbool ChartView::setTo(const int fromMaxPos)
{
	rbool res = true;
	int delta = (fromMaxPos - m_xPos - m_chartRect.width()) / m_pStyle->pFontsTicks->tickWidth;
	if (delta >= 0)
	{
		res = false;
		m_drawToX = m_drawFromX;
		m_drawToEventCount = m_drawToX.eventCount - delta;
	}
	return res;
}

void ChartView::setFromTo()
{
	RDOStudioChartDoc* doc = getDocument();

	m_drawFromX.eventCount = 0;
	m_drawFromEventIndex = 0;
	m_drawToX.eventCount = 0;
	m_drawToEventCount = 0;
	m_chartShift = 0;
	m_unwrapTimesList.clear();

	if (!doUnwrapTime())
	{
		if (m_timeScale)
		{
			m_drawFromX.time = doc->m_docTimes.front()->time + (double)m_xPos / double(m_timeScale);
			if (maxXVisible())
			{
				m_drawToX.time = doc->m_docTimes.back()->time;
			}
			else
			{
				m_drawToX.time = doc->m_docTimes.front()->time + (double)(m_xPos + m_chartRect.width()) / double(m_timeScale);
			}
		}
		else
		{
			m_drawFromX.time = 0;
			m_drawToX.time = 0;
		}
	}
	else
	{
		int it_pos = 0;
		int it_max_pos = 0;
		rbool need_search_to = true;
		int ticks = 0;
		TimesList::iterator it;
		for (it = doc->m_docTimes.begin(); it != doc->m_docTimes.end(); ++it)
		{
			it_pos = roundDouble(((*it)->time - doc->m_docTimes.front()->time) * double(m_timeScale)) + ticks * m_pStyle->pFontsTicks->tickWidth;
			it_max_pos = it_pos + m_pStyle->pFontsTicks->tickWidth * (*it)->eventCount;
			if (it_pos == m_xPos)
			{
				m_drawFromX = *(*it);
				need_search_to = setTo(it_max_pos);
				break;
			}
			if (it_pos < m_xPos && (it_max_pos >= m_xPos))
			{
				m_drawFromX = *(*it);
				m_drawFromEventIndex = (m_xPos - it_pos) / m_pStyle->pFontsTicks->tickWidth;
				m_chartShift = m_xPos - (it_pos + m_drawFromEventIndex * m_pStyle->pFontsTicks->tickWidth);
				need_search_to = setTo(it_max_pos);
				break;
			}
			if (it_pos > m_xPos)
			{
				m_drawFromX.time = (*it)->time - (it_pos - m_xPos) / double(m_timeScale);
				need_search_to = setTo(0);
				break;
			}
			ticks += (*it)->eventCount;
		}
		m_unwrapTimesList.push_back(&m_drawFromX);
		if (need_search_to)
		{
			if (it != doc->m_docTimes.end() && m_drawFromX == *(*it))
			{
				ticks += (*it)->eventCount;
				it++;
			}
			if (it == doc->m_docTimes.end() && !doc->m_docTimes.empty())
			{
				m_drawToX = m_drawFromX;
				int delta = m_drawToX.eventCount * m_pStyle->pFontsTicks->tickWidth - m_chartRect.width();
				m_drawToEventCount = delta >= 0 ? roundDouble((double)delta / (double)m_pStyle->pFontsTicks->tickWidth) : m_drawToX.eventCount;
				it_max_pos = m_drawToX.eventCount * m_pStyle->pFontsTicks->tickWidth;
				if (it_max_pos > m_chartRect.width())
				{
					m_drawToEventCount = (it_max_pos - m_chartRect.width()) / m_pStyle->pFontsTicks->tickWidth;
					if (m_drawToEventCount * m_pStyle->pFontsTicks->tickWidth < m_chartRect.width())
					{
						m_drawToEventCount++;
					}
				}
				else
				{
					m_drawToEventCount = m_drawToX.eventCount;
				}
				
			}
			int pos = m_xPos + m_chartRect.width();
			for (; it != doc->m_docTimes.end(); ++it)
			{
				it_pos = roundDouble(((*it)->time - doc->m_docTimes.front()->time) * double(m_timeScale)) + ticks * m_pStyle->pFontsTicks->tickWidth;
				it_max_pos = it_pos + m_pStyle->pFontsTicks->tickWidth * (*it)->eventCount;
				if (it_pos == pos)
				{
					m_drawToX = *(*it);
					break;
				}
				if (it_pos < pos && it_max_pos >= pos)
				{
					m_drawToX = *(*it);
					m_drawToEventCount = (pos - it_pos) / m_pStyle->pFontsTicks->tickWidth;
					if (it_pos + m_drawToEventCount * m_pStyle->pFontsTicks->tickWidth < pos)
					{
						m_drawToEventCount++;
					}
					break;
				}
				if (it_pos > pos)
				{
					m_drawToX.time = (*it)->time - (it_pos - pos) / double(m_timeScale);
					break;
				}
				m_unwrapTimesList.push_back((*it));
				ticks += (*it)->eventCount;
			}
		}
		m_unwrapTimesList.push_back(&m_drawToX);
	}
}

void ChartView::drawTitle(QPainter& painter, const QRect& chartRect)
{
	QRect rect(m_clientRect);
	rect.setTop(0);
	rect.setBottom(chartRect.top());

	painter.setFont(m_fontTitle);
	painter.setPen(m_pStyle->getTheme()->titleFGColor);
	painter.drawText(rect, Qt::AlignHCenter | Qt::AlignTop, QString::fromLocal8Bit(getDocument()->getTitle().c_str()));
}

void ChartView::drawLegend(QPainter& painter, const QRect& legendRect)
{
	RDOStudioChartDoc* doc = getDocument();
	QRect rect(legendRect);
	painter.setFont(m_fontLegend);
	for (std::vector<ChartSerie*>::iterator it = doc->m_serieList.begin(); it != doc->m_serieList.end(); ++it)
	{
		QSize size = (*it)->drawLegend(painter, rect, m_pStyle->getTheme()->legendFgColor);
		rect.setTop(rect.top() + size.height());
	}
}

void ChartView::drawYAxis(QPainter& painter, const QRect& chartRect, const ChartSerie* axisValues)
{
	QRect tmprect(m_clientRect);
	tmprect.setRight(chartRect.left() - 5);
	tmprect.setLeft(5);
	if (axisValues)
	{
		int count;
		axisValues->getSerie()->getValueCount(count);
		if (count)
		{
			painter.setFont(m_fontAxis);
			painter.setPen(m_pStyle->getTheme()->axisFgColor);

			int count = m_captionList.size();
			int heightoffset = roundDouble((double)chartRect.height() / (double)(count - 1));
			tmprect.setTop(chartRect.bottom());
			int index = 0;
			for (std::vector<tstring>::iterator it = m_captionList.begin(); it != m_captionList.end(); ++it)
			{
				index++;
				painter.drawText(tmprect, Qt::AlignRight, QString::fromLocal8Bit(it->c_str()));
				if (index != 1 && index < count)
				{
					QPainterPath path;
					path.moveTo(tmprect.right() + 2, tmprect.top());
					path.lineTo(chartRect.left(), tmprect.top());
					painter.drawPath(path);
				}
				if (index == count - 1)
				{
					tmprect.setTop(chartRect.top());
				}
				else
				{
					tmprect.setTop(tmprect.top() - heightoffset);
				}
			}
		}
	}
}

void ChartView::drawXAxis(QPainter& painter, const QRect& chartRect)
{
	QRect tmprect(chartRect);
	tmprect.setTop(chartRect.bottom() + 3);
	tmprect.setLeft(chartRect.left());
	tmprect.setBottom(m_clientRect.bottom());
	tmprect.setRight(m_clientRect.right() - 5);

	RDOStudioChartDoc* doc = getDocument();
	if (!doc->m_docTimes.empty())
	{
		tstring formatstr = "%.3f";

		painter.setFont(m_fontAxis);
		painter.setPen(m_pStyle->getTheme()->axisFgColor);

		if (!doUnwrapTime())
		{
			double valoffset = 0;
			int widthoffset = chartRect.width() / (m_valueCountX - 1);
			if (m_drawToX != m_drawFromX)
			{
				valoffset = (m_drawToX.time - m_drawFromX.time) / (m_valueCountX - 1);
			}
			double valo = m_drawFromX.time;
			int x = chartRect.left();
			tstring str = rdo::format(formatstr.c_str(), valo);
			tmprect.setLeft(x);
			painter.drawText(tmprect, Qt::AlignLeft, QString::fromLocal8Bit(str.c_str()));
			valo += valoffset;
			x += widthoffset;
			if (valoffset)
			{
				for (int i = 1; i < m_valueCountX; i++)
				{
					str = rdo::format(formatstr.c_str(), valo);
					tmprect.setLeft(x);
					painter.drawText(tmprect, Qt::AlignLeft, QString::fromLocal8Bit(str.c_str()));
					if (i != m_valueCountX - 1)
					{
						QPainterPath path;
						path.moveTo(x, chartRect.bottom());
						path.lineTo(x, chartRect.bottom() + 3);
						painter.drawPath(path);
					}
					valo += valoffset;
					x += widthoffset;
				}
			}
		}
		else
		{
			int ticks = 0;
			tstring str;
			int lastx = 0;
			QSize sz;
			for (TimesList::iterator it = m_unwrapTimesList.begin(); it != m_unwrapTimesList.end(); ++it)
			{
				tmprect.setLeft(chartRect.left() + (LONG)(((*it)->time - m_unwrapTimesList.front()->time) * m_timeScale + ticks * m_pStyle->pFontsTicks->tickWidth - m_chartShift));
				tmprect.setLeft(std::min(tmprect.left(), chartRect.right() - 1));
				str = rdo::format(formatstr.c_str(), (*it)->time);
				if (*(*it) == m_drawFromX)
				{
					tmprect.setLeft(tmprect.left() + m_chartShift);
				}
				if (tmprect.left() > chartRect.right())
				{
					tmprect.setLeft(chartRect.right());
				}

				if (tmprect.left() > lastx)
				{
					painter.drawText(tmprect, Qt::AlignLeft, QString::fromLocal8Bit(str.c_str()));
					if (tmprect.left() != chartRect.left() && tmprect.left() != chartRect.right())
					{
						QPainterPath path;
						path.moveTo(tmprect.left(), chartRect.bottom());
						path.lineTo(tmprect.left(), chartRect.bottom() + 3);
						painter.drawPath(path);
					}
					QSize size = painter.fontMetrics().boundingRect(QString::fromLocal8Bit(str.c_str())).size();
					lastx = tmprect.left() + size.width();
				}

				ticks += (*it)->eventCount;
				if (*(*it) == m_drawFromX)
				{
					ticks -= m_drawFromEventIndex;
				}
			}
		}
	}
}

void ChartView::drawGrid(QPainter& painter, const QRect& chartRect)
{
	QBrush brush(m_pStyle->getTheme()->chartBgColor);
	painter.setBrush(brush);
	painter.drawRect(chartRect);

	if (doUnwrapTime())
	{
		QRect rect(chartRect);
		rect.adjust(-1, -1, -1, -1);
		QRect tmprect(rect);

		//! @todo qt
		//::IntersectClipRect(m_hmemdc, rect.left, rect.top, rect.right, rect.bottom);
		int ticks = 0;
		TimesList::iterator it = m_unwrapTimesList.begin();
		if (m_drawFromX == m_drawToX)
		{
			++it;
		}
		//For drawing solid rect
		painter.setBrush(m_pStyle->getTheme()->timeBgColor);
		for (; it != m_unwrapTimesList.end(); ++it)
		{
			int width = (*it)->eventCount * m_pStyle->pFontsTicks->tickWidth;
			tmprect.setLeft(rect.left() + (LONG)(((*it)->time - m_unwrapTimesList.front()->time) * m_timeScale + ticks * m_pStyle->pFontsTicks->tickWidth - m_chartShift));
			if (tmprect.left() < rect.left())
				tmprect.setLeft(rect.left());
			if (*(*it) == m_drawFromX)
			{
				width -= m_drawFromEventIndex * m_pStyle->pFontsTicks->tickWidth + m_chartShift;
			}
			if (*(*it) == m_drawToX)
			{
				width = m_drawToEventCount * m_pStyle->pFontsTicks->tickWidth;
			}
			tmprect.setRight(tmprect.left() + width);
			if (tmprect.right() > rect.right())
				tmprect.setRight(rect.right());
			painter.drawRect(tmprect);
			ticks += (*it)->eventCount;
			if (*(*it) == m_drawFromX)
			{
				ticks -= m_drawFromEventIndex;
			}
		}
	}
}

void ChartView::copyToClipboard()
{
	//! todo qt
	//if (!OpenClipboard() || !::EmptyClipboard())
	//	return;

	//m_mutex.Lock();

	//if (m_hmemdc)
	//{
	//	HBITMAP hbm = ::CreateCompatibleBitmap(m_hmemdc, m_clientRect.width(), m_clientRect.height());
	//	if (hbm)
	//	{
	//		HDC hdcDst = ::CreateCompatibleDC(m_hmemdc);
	//		if (hdcDst)
	//		{
	//			HGDIOBJ oldObj = ::SelectObject(hdcDst, hbm);
	//			::BitBlt(hdcDst, 0, 0, m_clientRect.width(), m_clientRect.height(), m_hmemdc, m_clientRect.left, m_clientRect.top, SRCCOPY);
	//			::SelectObject(hdcDst, oldObj);
	//			::DeleteDC(hdcDst);
	//			::SetClipboardData(CF_BITMAP, hbm);
	//		}
	//		else
	//		{
	//			::DeleteObject(hbm);
	//		}
	//	}
	//}

	//CloseClipboard();

	//m_mutex.Unlock();
}

void ChartView::setZoom(double new_zoom, const rbool force_update)
{
	UNUSED(force_update);

	/*scale_koeff = new_zoom;
	 if (scale_koeff < auto_zoom) {
	 scale_koeff = auto_zoom;
	 }*/
	if (doUnwrapTime() && new_zoom < 1)
	{
		new_zoom = 1;
	}
	else if (!doUnwrapTime() && new_zoom < m_zoomAuto)
	{
		new_zoom = m_zoomAuto;
	}
	//if (zoom != new_zoom || scale_koeff != new_zoom || force_update) {
		m_zoom = new_zoom;
		m_scaleKoeff = m_zoom;
		recalcLayout();
		updateScrollBars();
		update();
	//}
}

DROPEFFECT ChartView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	UNUSED(dwKeyState);
	UNUSED(point);

	HGLOBAL glb = NULL;
	UINT format = g_pTracer->getClipboardFormat();
	if (pDataObject->IsDataAvailable(CLIPFORMAT(format)))
	{
		glb = pDataObject->GetGlobalData(CLIPFORMAT(format));
		if (glb)
		{
			m_pddSerie = *(TracerSerie**)::GlobalLock(glb);
			::GlobalUnlock(glb);
			::GlobalFree(glb);
		}
		if (!getDocument()->serieExists(m_pddSerie))
		{
			return DROPEFFECT_COPY;
		}
		else
		{
			m_pddSerie = NULL;
		}
	}
	return DROPEFFECT_NONE;
}

void ChartView::OnDragLeave()
{
	m_pddSerie = NULL;
}

DROPEFFECT ChartView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	UNUSED(dwKeyState);
	UNUSED(point);

	return (pDataObject->IsDataAvailable(CLIPFORMAT(g_pTracer->getClipboardFormat())) && m_pddSerie)
		? DROPEFFECT_COPY
		: DROPEFFECT_NONE;
}

BOOL ChartView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	UNUSED(pDataObject);
	UNUSED(dropEffect);
	UNUSED(point);

	getDocument()->addSerie(m_pddSerie);
	m_pddSerie = NULL;
	return TRUE;
}

RDOStudioChartDoc* ChartView::getDocument()
{
	return m_pDocument;
}

BOOL ChartView::OnEraseBkgnd(CDC* pDC)
{
	UNUSED(pDC);
	return TRUE;
}

void ChartView::resizeEvent(QResizeEvent* pEvent)
{
	super::resizeEvent(pEvent);

	m_mutex.Lock();

	m_clientRect = QRect(QPoint(0, 0), pEvent->size());
	if (getDocument())
	{
		recalcLayout();
		updateScrollBars(false);
		//setZoom(zoom);
	}

	m_mutex.Unlock();
}

void ChartView::onChartTimeWrap()
{
	m_timeWrapFlag = !m_timeWrapFlag;
	recalcLayout();
	updateScrollBars();
	update();
	onUpdateActions(isActivated());
}

void ChartView::onUserUpdateChartView(ruint updateType)
{
	if (doUnwrapTime() || updateType != UPDATE_TIMETICKS)
	{
		updateView();
	}
}

void ChartView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNUSED(nPos);
	UNUSED(pScrollBar);

	int inc = 0;
	SCROLLINFO si;
	si.cbSize = sizeof(si);

	switch (nSBCode)
	{
	case SB_PAGEUP:
		inc = -m_chartRect.width();
		break;

	case SB_PAGEDOWN:
		inc = m_chartRect.width();
		break;

	case SB_LINEUP:
		inc = -m_pStyle->pFontsTicks->tickWidth;
		break;

	case SB_LINEDOWN:
		inc = m_pStyle->pFontsTicks->tickWidth;
		break;

	case SB_THUMBTRACK:
	{
		//! @todo qt
		//GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
		inc = si.nTrackPos - m_xPos;
		break;
	}
	default:
		inc = 0;
	}

	if (!inc)
		return;

	// If applying the horizontal scrolling increment does not 
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if (inc == std::max(-m_xPos, std::min(inc, m_xMax - m_xPos)))
	{
		m_xPos += inc;
		setScrollPos(SB_HORZ, m_xPos);
	}
}

void ChartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNUSED(nRepCnt);
	UNUSED(nFlags);

	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;
	rbool ctrl = ((::GetKeyState(VK_CONTROL) & 0x80000000) != 0);
	rbool side = false;
	int pos = 0;

	switch (nChar)
	{
	case VK_LEFT:
	{
		if (ctrl)
		{
			side = true;
		}
		else
		{
			scrollNotify = SB_LINEUP;
			msg = WM_HSCROLL;
		}
		break;
	}
	case VK_RIGHT:
	{
		if (ctrl)
		{
			side = true;
			pos = m_xMax;
		}
		else
		{
			scrollNotify = SB_LINEDOWN;
			msg = WM_HSCROLL;
		}
		break;
	}
	default:
		break;
	}

	if (scrollNotify != 0xFFFF)
	{
		//! @todo qt
		//::SendMessage(m_hWnd, msg, MAKELONG(scrollNotify, pos), NULL);
	}

	if (side)
	{
		setScrollPos(SB_HORZ, pos);
	}
}

void ChartView::OnEditCopy()
{
	copyToClipboard();
}

void ChartView::onViewZoomIn()
{
	if (m_zoomAutoFlag)
	{
		m_zoomAutoFlag = !m_zoomAutoFlag;
	}
	double delta = m_zoom < 1 ? 0.1 : 0.5;
	if (m_zoom + delta > 1 && m_zoom + delta - 1 < delta)
	{
		setZoom(1);
	}
	else
	{
		setZoom(m_zoom + delta);
	}
	onUpdateActions(isActivated());
}

void ChartView::onViewZoomOut()
{
	if (m_zoomAutoFlag)
	{
		m_zoomAutoFlag = !m_zoomAutoFlag;
	}
	double delta = m_zoom > 1 ? -0.5 : -0.1;
	double zoom_new = m_zoom + delta;
	setZoom(zoom_new);
	onUpdateActions(isActivated());
}

void ChartView::onViewZoomReset()
{
	if (m_zoomAutoFlag)
	{
		m_zoomAutoFlag = !m_zoomAutoFlag;
	}
	setZoom(1);
	onUpdateActions(isActivated());
}

const RDOStudioChartViewStyle& ChartView::getStyle() const
{
	return (*m_pStyle);
}

void ChartView::setFonts(const rbool needRedraw)
{
	UNUSED(needRedraw);

	if (!m_pStyle)
		return;
	
	m_mutex.Lock();

	RDOStudioChartViewTheme* pChartTheme = static_cast<RDOStudioChartViewTheme*>(m_pStyle->theme);

	m_fontAxis = QFont(m_pStyle->font->name.c_str());
	m_fontAxis.setBold     (pChartTheme->defaultStyle & RDOStyleFont::BOLD     );
	m_fontAxis.setItalic   (pChartTheme->defaultStyle & RDOStyleFont::ITALIC   );
	m_fontAxis.setUnderline(pChartTheme->defaultStyle & RDOStyleFont::UNDERLINE);
	m_fontAxis.setPointSize(m_pStyle->font->size);

	m_fontTitle = QFont(m_pStyle->font->name.c_str());
	m_fontTitle.setBold     (pChartTheme->titleStyle & RDOStyleFont::BOLD     );
	m_fontTitle.setItalic   (pChartTheme->titleStyle & RDOStyleFont::ITALIC   );
	m_fontTitle.setUnderline(pChartTheme->titleStyle & RDOStyleFont::UNDERLINE);
	m_fontTitle.setPointSize(m_pStyle->pFontsTicks->titleFontSize);

	m_fontLegend = QFont(m_pStyle->font->name.c_str());
	m_fontLegend.setBold     (pChartTheme->legendStyle & RDOStyleFont::BOLD     );
	m_fontLegend.setItalic   (pChartTheme->legendStyle & RDOStyleFont::ITALIC   );
	m_fontLegend.setUnderline(pChartTheme->legendStyle & RDOStyleFont::UNDERLINE);
	m_fontLegend.setPointSize(m_pStyle->pFontsTicks->legendFontSize);

	m_mutex.Unlock();
}

void ChartView::setStyle(RDOStudioChartViewStyle* pStyle, const rbool needRedraw)
{
	m_pStyle = pStyle;

	setFonts(false);

	/*if (previewMode) {
	 auto_zoom = 1;
	 setZoom(1);
	 }*/

	if (needRedraw)
	{
		recalcLayout();
		updateScrollBars();
		update();
	}
}

void ChartView::setPreviwMode(rbool value)
{
	m_previewMode = value;
}

void ChartView::OnChartOptions()
{
	RDOStudioChartOptions dlg(this);
	dlg.DoModal();
}

void ChartView::updateView()
{
	getDocument()->lock();
	rbool lastvisible = maxXVisible();
	recalcLayout();
	updateScrollBars(false);
	if (lastvisible && !maxXVisible())
	{
		setScrollPos(SB_HORZ, m_xMax, false);
	}
	parentWidget()->update();
	updateScrollBars(true);
	getDocument()->unlock();
}

void ChartView::attachToDoc()
{
	getDocument()->addToViews(this);
}

void ChartView::paintEvent(QPaintEvent* pEvent)
{
	QPainter painter(this);

	RDOStudioChartDoc* doc = getDocument();
	doc->m_mutex.Lock();

	m_mutex.Lock();

	painter.fillRect(m_clientRect, m_pStyle->theme->backgroundColor);

	drawTitle(painter, m_chartRect);

	if (m_needDrawLegend)
	{
		drawLegend(painter, m_legendRect);
	}

	if (m_chartRect.height() > 0 && m_chartRect.width() > 0)
	{
		painter.setPen(m_pStyle->getTheme()->defaultColor);

		drawYAxis(painter, m_chartRect, m_pYAxis);
		setFromTo();
		drawXAxis(painter, m_chartRect);
		drawGrid(painter, m_chartRect);

		for (std::vector<ChartSerie*>::iterator it = doc->m_serieList.begin(); it != doc->m_serieList.end(); ++it)
		{
			(*it)->drawSerie(this, painter, m_chartRect);
		}
	}

	m_mutex.Unlock();

	doc->m_mutex.Unlock();
}

void ChartView::onViewZoomAuto()
{
	m_zoomAutoFlag = !m_zoomAutoFlag;
	if (!m_zoomAutoFlag)
	{
		setZoom(m_zoomOld, true);
	}
	else
	{
		m_zoomOld = m_zoom;
		setZoom(m_zoomAuto, true);
	}
	onUpdateActions(isActivated());
}

void ChartView::OnHelpKeyword()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
	studioApp.callQtAssistant(ba);
}

rbool ChartView::canUnwrapTime() const
{
	return m_scaleKoeff >= 1 && !m_zoomAutoFlag;
}

rbool ChartView::doUnwrapTime() const
{
	return canUnwrapTime() && !m_timeWrapFlag;
}

rbool ChartView::minXVisible() const
{
	return m_xPos == 0;
}

rbool ChartView::maxXVisible() const
{
	return m_xPos == m_xMax;
}

void ChartView::onUpdateActions(rbool activated)
{
	RDOStudioMainFrame* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	updateAction(
		pMainWindow->actViewZoomInc,
		activated && !m_zoomAutoFlag && m_zoom < 5,
		this, &ChartView::onViewZoomIn
	);

	updateAction(
		pMainWindow->actViewZoomDec,
		activated && !m_zoomAutoFlag && ((!doUnwrapTime() && m_zoom > m_zoomAuto) || (doUnwrapTime() && m_zoom > 1)),
		this, &ChartView::onViewZoomOut
	);

	updateAction(
		pMainWindow->actViewZoomReset,
		activated && m_zoom != 1,
		this, &ChartView::onViewZoomReset
	);

	pMainWindow->actViewZoomAuto->setCheckable(true);
	pMainWindow->actViewZoomAuto->setChecked(m_zoomAutoFlag);

	updateAction(
		pMainWindow->actViewZoomAuto,
		activated,
		this, &ChartView::onViewZoomAuto
	);

	pMainWindow->actChartTimeWrap->setCheckable(true);
	pMainWindow->actChartTimeWrap->setChecked(!canUnwrapTime() || m_timeWrapFlag);

	updateAction(
		pMainWindow->actChartTimeWrap,
		activated && canUnwrapTime(),
		this, &ChartView::onChartTimeWrap
	);
}

ChartViewMainWnd::ChartViewMainWnd(PTR(QWidget) pParent, PTR(RDOStudioChartDoc) pDocument, rbool preview)
	: super(pParent)
{
	ChartView* pContext = new ChartView(this, pDocument, preview);
	pContext->show();
	setViewport(pContext);
}

ChartViewMainWnd::~ChartViewMainWnd()
{}

ChartView& ChartViewMainWnd::view()
{
	return *static_cast<PTR(ChartView)>(viewport());
}

rbool ChartViewMainWnd::viewportEvent(PTR(QEvent) pEvent)
{
	UNUSED(pEvent);
	return false;
}

void ChartViewMainWnd::focusInEvent(QFocusEvent* pEvent)
{
	static_cast<QObject*>(viewport())->event(pEvent);
}

void ChartViewMainWnd::focusOutEvent(QFocusEvent* pEvent)
{
	static_cast<QObject*>(viewport())->event(pEvent);
}

void ChartViewMainWnd::keyPressEvent(PTR(QKeyEvent) pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Up      :
	case Qt::Key_Down    :
	case Qt::Key_PageUp  :
	case Qt::Key_PageDown:
	case Qt::Key_Home    :
	case Qt::Key_End     :
		static_cast<QObject*>(viewport())->event(pEvent);
		break;

	default:
		super::keyPressEvent(pEvent);
		break;
	}
}

void ChartView::mousePressEvent(PTR(QMouseEvent) pEvent)
{
	if (pEvent->button() == Qt::RightButton && !m_previewMode)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}