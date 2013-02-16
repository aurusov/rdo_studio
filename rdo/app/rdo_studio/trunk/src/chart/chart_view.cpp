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
// ON_UPDATE_COMMAND_UI сделано

BEGIN_MESSAGE_MAP(ChartView, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_CHART_TIMEWRAP, OnChartTimewrap)
	ON_UPDATE_COMMAND_UI(ID_CHART_TIMEWRAP, OnUpdateChartTimewrap)
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_INITMENUPOPUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnChartZoomZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnChartZoomZoomout)
	ON_COMMAND(ID_VIEW_ZOOMRESET, OnChartZoomResetzoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateChartZoomZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateChartZoomZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMRESET, OnUpdateChartZoomResetzoom)
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_CHART_OPTIONS, OnChartOptions)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_COMMAND(ID_VIEW_ZOOMAUTO, OnViewZoomauto)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMAUTO, OnUpdateViewZoomauto)
	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
	ON_MESSAGE(WM_USER_UPDATE_CHART_VIEW, OnUserUpdateChartView)
END_MESSAGE_MAP()

static COLORREF convertColor(CREF(QColor) color)
{
	return RGB(color.red(), color.green(), color.blue());
}

ChartView::ChartView(QWidget* pParent, RDOStudioChartDoc* pDocument, const rbool preview)
	: CWnd()
	, m_pDocument(pDocument)
	, m_pParent  (pParent  )
	, m_bmpRect(0, 0, 0, 0)
	, m_newClientRect(0, 0, 0, 0)
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
	, m_hbmp(NULL)
	, m_hbmpInit(NULL)
	, m_hfontTitle(NULL)
	, m_hfontLegend(NULL)
	, m_hfontAxis(NULL)
	, m_hfontInit(NULL)
	, m_hdc(NULL)
	, m_savedHdc(0)
	, m_hmemdc(NULL)
	, m_savedHmemdc(0)
	, m_hwnd(NULL)
{
	if (m_previewMode)
		m_timeWrapFlag = false;
}

ChartView::~ChartView()
{
	delete m_pDocument;
}

BOOL ChartView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL /*| WS_VSCROLL*/| WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//Setting class style CS_OWNDC to avoid DC releasing
	//and selecting previous objects into it
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL*/);
	return TRUE;
}

int ChartView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!m_ddTarget.Register(this))
		return -1;
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Remembering handle to the window in hwnd member
	m_hwnd = GetSafeHwnd();
	//Remembering handle to the private device context in hdc member
	if (m_hwnd)
		m_hdc = ::GetDC(m_hwnd);
	if (!m_hdc)
		return -1;

	//Creating memory device context to draw on bitmap
	m_hmemdc = ::CreateCompatibleDC(m_hdc);
	if (!m_hmemdc)
		return -1;
	//Saving the own DC and memory DC states to restore them
	//before deleting the memory DC or destroying window
	m_savedHdc = ::SaveDC(m_hdc);
	m_savedHmemdc = ::SaveDC(m_hmemdc);
	if (!m_savedHdc || !m_savedHmemdc)
		return -1;
	//Setting background mode one time in initialization.
	//We have private DC, so we needn't to reset it each time
	//we paint
	::SetBkMode(m_hmemdc, TRANSPARENT);
	//Remembering default font to select it into DC
	//when destroying window or setting new font
	m_hfontInit = (HFONT)::GetCurrentObject(m_hmemdc, OBJ_FONT);
	//Remembering default bmp to select it into DC
	//when resizing window
	m_hbmpInit = (HBITMAP)::GetCurrentObject(m_hmemdc, OBJ_BITMAP);

	if (!m_previewMode)
		setStyle(&studioApp.getStyle()->style_chart, false);

	if (getDocument())
	{
		recalcLayout();
		updateScrollBars();
	}

	m_popupMenu.CreatePopupMenu();

	if (AfxGetMainWnd())
	{
		CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
		if (mainMenu)
		{
			rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
			int delta = maximized ? 1 : 0;

			appendMenu(mainMenu->GetSubMenu(1 + delta), 4, &m_popupMenu);
			m_popupMenu.AppendMenu(MF_SEPARATOR);
			appendMenu(mainMenu->GetSubMenu(3 + delta), 6, &m_popupMenu);
			appendMenu(mainMenu->GetSubMenu(3 + delta), 7, &m_popupMenu);
			appendMenu(mainMenu->GetSubMenu(3 + delta), 8, &m_popupMenu);
			appendMenu(mainMenu->GetSubMenu(3 + delta), 9, &m_popupMenu);
			m_popupMenu.AppendMenu(MF_SEPARATOR);
			appendMenu(mainMenu->GetSubMenu(6 + delta), 4, &m_popupMenu);
			m_popupMenu.AppendMenu(MF_SEPARATOR);
			appendMenu(mainMenu->GetSubMenu(6 + delta), 6, &m_popupMenu);
		}
	}

	attachToDoc();

	return 0;
}

QWidget* ChartView::getQtParent()
{
	return m_pParent;
}

void ChartView::recalcLayout()
{
	RDOStudioChartDoc* doc = getDocument();
	doc->m_mutex.Lock();

	m_mutex.Lock();

	::SelectObject(m_hmemdc, m_hfontTitle);

	CRect tmprect;
	tstring str = doc->getTitle();
	if (!str.empty())
	{
		tmprect.CopyRect(&m_newClientRect);
		::DrawText(m_hmemdc, str.c_str(), str.length(), tmprect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
	}
	else
	{
		tmprect = CRect(0, 0, 0, 0);
	}
	m_chartRect.top = tmprect.Height() + 5;

	::SelectObject(m_hmemdc, m_hfontAxis);

	SIZE sz;
	SIZE size_max;
	size_max.cx = 0;
	size_max.cy = 0;
	if (m_pYAxis)
	{
		m_pYAxis->getCaptions(m_captionList, m_valueCountY);
		for (std::vector<tstring>::iterator it = m_captionList.begin(); it != m_captionList.end(); it++)
		{
			::GetTextExtentPoint32(m_hmemdc, (*it).c_str(), (*it).length(), &sz);
			if (sz.cx > size_max.cx)
				size_max.cx = sz.cx;
		}
	}
	m_chartRect.left = size_max.cx + 10;

	str = !doc->m_docTimes.empty() ? rdo::format("%.3f", doc->m_docTimes.back()->time) : rdo::format("%.3f", 0);
	::GetTextExtentPoint32(m_hmemdc, str.c_str(), str.length(), &sz);
	m_chartRect.right = m_newClientRect.right - sz.cx - 5;

	m_chartRect.bottom = m_newClientRect.bottom - sz.cy - 7;

	if (m_needDrawLegend)
	{

		::SelectObject(m_hmemdc, m_hfontLegend);

		int count = 0;
		for (std::vector<ChartSerie*>::iterator it = doc->m_serieList.begin(); it != doc->m_serieList.end(); it++)
		{
			(*it)->getLegendExtent(m_hmemdc, m_chartRect, sz);
			if (sz.cx && sz.cy)
			{
				if (sz.cx > size_max.cx)
					size_max.cx = sz.cx;
				if (sz.cy > size_max.cy)
					size_max.cy = sz.cy;
				count++;
			}
		}

		m_legendRect.top = m_chartRect.top;
		m_legendRect.bottom = m_legendRect.top + count * size_max.cy + 3 + doc->getMaxMarkerSize() / 2;
		m_legendRect.left = m_chartRect.left + (m_chartRect.Width() - size_max.cx) / 2;
		m_legendRect.right = m_legendRect.left + size_max.cx;
		m_chartRect.top += m_legendRect.Height();
	}

	if (!doc->m_docTimes.empty())
	{
		double timeRange = doc->m_docTimes.back()->time - doc->m_docTimes.front()->time;
		if (timeRange > 0)
		{
			long double timeScaleAuto = doUnwrapTime() ? (double)(m_chartRect.Width() - m_pStyle->pFontsTicks->tickWidth * doc->m_ticksCount) / timeRange : (double)m_chartRect.Width() / timeRange;
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

void ChartView::setScrollPos(UINT nSBCode, UINT nPos, const rbool need_update)
{
	if (nSBCode == SB_HORZ)
		m_xPos = nPos;

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS;
	si.nPos = m_xPos;
	SetScrollInfo(nSBCode, &si, TRUE);

	if (need_update)
	{
		Invalidate();
		updateWindow();
	}
}

void ChartView::updateScrollBars(const rbool need_update)
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

	m_xMax = std::max(0, size - m_chartRect.Width());
	m_xPos = std::min(m_xPos, m_xMax);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = size - 1;
	si.nPage = m_chartRect.Width();
	si.nPos = m_xPos;
	SetScrollInfo(SB_HORZ, &si, need_update);

	doc->m_mutex.Unlock();
}

rbool ChartView::setTo(const int from_max_pos)
{
	rbool res = true;
	int delta = (from_max_pos - m_xPos - m_chartRect.Width()) / m_pStyle->pFontsTicks->tickWidth;
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
				m_drawToX.time = doc->m_docTimes.front()->time + (double)(m_xPos + m_chartRect.Width()) / double(m_timeScale);
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
		for (it = doc->m_docTimes.begin(); it != doc->m_docTimes.end(); it++)
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
				int delta = m_drawToX.eventCount * m_pStyle->pFontsTicks->tickWidth - m_chartRect.Width();
				m_drawToEventCount = delta >= 0 ? roundDouble((double)delta / (double)m_pStyle->pFontsTicks->tickWidth) : m_drawToX.eventCount;
				it_max_pos = m_drawToX.eventCount * m_pStyle->pFontsTicks->tickWidth;
				if (it_max_pos > m_chartRect.Width())
				{
					m_drawToEventCount = (it_max_pos - m_chartRect.Width()) / m_pStyle->pFontsTicks->tickWidth;
					if (m_drawToEventCount * m_pStyle->pFontsTicks->tickWidth < m_chartRect.Width())
						m_drawToEventCount++;
				}
				else
				{
					m_drawToEventCount = m_drawToX.eventCount;
				}
				
			}
			int pos = m_xPos + m_chartRect.Width();
			for (; it != doc->m_docTimes.end(); it++)
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
						m_drawToEventCount++;
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

void ChartView::drawTitle(CRect& chartRect)
{
	CRect tmprect;
	tmprect.CopyRect(&m_newClientRect);
	tmprect.top = 0;
	tmprect.bottom = chartRect.top;

	::SelectObject(m_hmemdc, m_hfontTitle);
	::SetTextColor(m_hmemdc, convertColor(m_pStyle->getTheme()->titleFGColor));

	tstring str = getDocument()->getTitle();
	::DrawText(m_hmemdc, str.c_str(), str.length(), tmprect, DT_CENTER | DT_WORDBREAK);
}

void ChartView::drawLegend(CRect& legendRect)
{
	RDOStudioChartDoc* doc = getDocument();
	CRect tmprect;
	tmprect.CopyRect(&legendRect);
	SIZE size;
	::SelectObject(m_hmemdc, m_hfontLegend);
	for (std::vector<ChartSerie*>::iterator it = doc->m_serieList.begin(); it != doc->m_serieList.end(); ++it)
	{
		(*it)->drawInLegend(m_hmemdc, tmprect, convertColor(m_pStyle->getTheme()->legendFgColor), size);
		tmprect.top += size.cy;
	}
}

void ChartView::drawYAxis(CRect& chartRect, const ChartSerie* axisValues)
{
	CRect tmprect;
	tmprect.CopyRect(&m_newClientRect);
	tmprect.right = chartRect.left - 5;
	tmprect.left = 5;
	if (axisValues)
	{
		int count;
		axisValues->getSerie()->getValueCount(count);
		if (count)
		{
			::SelectObject(m_hmemdc, m_hfontAxis);
			::SetTextColor(m_hmemdc, convertColor(m_pStyle->getTheme()->axisFgColor));

			int count = m_captionList.size();
			int heightoffset = roundDouble((double)chartRect.Height() / (double)(count - 1));
			tmprect.top = chartRect.bottom;
			int index = 0;
			for (std::vector<tstring>::iterator it = m_captionList.begin(); it != m_captionList.end(); it++)
			{
				index++;
				::DrawText(m_hmemdc, (*it).c_str(), (*it).length(), tmprect, DT_RIGHT);
				if (index != 1 && index < count)
				{
					::MoveToEx(m_hmemdc, tmprect.right + 2, tmprect.top, (LPPOINT)NULL);
					::LineTo(m_hmemdc, chartRect.left, tmprect.top);
				}
				if (index == count - 1)
				{
					tmprect.top = chartRect.top;
				}
				else
				{
					tmprect.top -= heightoffset;
				}
			}
		}
	}
}

void ChartView::drawXAxis(CRect& chartRect)
{
	CRect tmprect;
	tmprect.CopyRect(&chartRect);
	tmprect.top = chartRect.bottom + 3;
	tmprect.left = chartRect.left;
	tmprect.bottom = m_newClientRect.bottom;
	tmprect.right = m_newClientRect.right - 5;

	RDOStudioChartDoc* doc = getDocument();
	if (!doc->m_docTimes.empty())
	{
		tstring formatstr = "%.3f";

		::SelectObject(m_hmemdc, m_hfontAxis);
		::SetTextColor(m_hmemdc, convertColor(m_pStyle->getTheme()->axisFgColor));

		if (!doUnwrapTime())
		{
			double valoffset = 0;
			int widthoffset = chartRect.Width() / (m_valueCountX - 1);
			if (m_drawToX != m_drawFromX)
			{
				valoffset = (m_drawToX.time - m_drawFromX.time) / (m_valueCountX - 1);
			}
			double valo = m_drawFromX.time;
			int x = chartRect.left;
			tstring str = rdo::format(formatstr.c_str(), valo);
			tmprect.left = x;
			::DrawText(m_hmemdc, str.c_str(), str.length(), tmprect, DT_LEFT);
			valo += valoffset;
			x += widthoffset;
			if (valoffset)
			{
				for (int i = 1; i < m_valueCountX; i++)
				{
					str = rdo::format(formatstr.c_str(), valo);
					tmprect.left = x;
					::DrawText(m_hmemdc, str.c_str(), str.length(), tmprect, DT_LEFT);
					if (i != m_valueCountX - 1)
					{
						::MoveToEx(m_hmemdc, x, chartRect.bottom, (LPPOINT)NULL);
						::LineTo(m_hmemdc, x, chartRect.bottom + 3);
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
			SIZE sz;
			for (TimesList::iterator it = m_unwrapTimesList.begin(); it != m_unwrapTimesList.end(); it++)
			{
				tmprect.left = chartRect.left + (LONG)(((*it)->time - m_unwrapTimesList.front()->time) * m_timeScale + ticks * m_pStyle->pFontsTicks->tickWidth - m_chartShift);
				tmprect.left = std::min(tmprect.left, chartRect.right - 1);
				str = rdo::format(formatstr.c_str(), (*it)->time);
				if (*(*it) == m_drawFromX)
				{
					tmprect.left += m_chartShift;
				}
				if (tmprect.left > chartRect.right)
				{
					tmprect.left = chartRect.right;
				}

				if (tmprect.left > lastx)
				{
					::DrawText(m_hmemdc, str.c_str(), str.length(), tmprect, DT_LEFT);
					if (tmprect.left != chartRect.left && tmprect.left != chartRect.right)
					{
						::MoveToEx(m_hmemdc, tmprect.left, chartRect.bottom, (LPPOINT)NULL);
						::LineTo(m_hmemdc, tmprect.left, chartRect.bottom + 3);
					}
					::GetTextExtentPoint32(m_hmemdc, str.c_str(), str.length(), &sz);
					lastx = tmprect.left + sz.cx;
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

void ChartView::drawGrid(CRect& chartRect)
{
	HBRUSH brush_chart = NULL;
	HBRUSH old_brush = NULL;
	try
	{
		brush_chart = ::CreateSolidBrush(convertColor(m_pStyle->getTheme()->chartBgColor));
		old_brush = (HBRUSH)::SelectObject(m_hmemdc, brush_chart);
		::Rectangle(m_hmemdc, chartRect.left, chartRect.top, chartRect.right, chartRect.bottom);
		::SelectObject(m_hmemdc, old_brush);
		::DeleteObject(brush_chart);
		brush_chart = NULL;
	}
	catch (...)
	{
		if (brush_chart)
		{
			::SelectObject(m_hmemdc, old_brush);
			::DeleteObject(brush_chart);
			brush_chart = NULL;
		}
	}

	if (doUnwrapTime())
	{
		CRect rect;
		rect.CopyRect(chartRect);
		rect.InflateRect(-1, -1);
		CRect tmprect;
		tmprect.CopyRect(&rect);

		::IntersectClipRect(m_hmemdc, rect.left, rect.top, rect.right, rect.bottom);
		int ticks = 0;
		TimesList::iterator it = m_unwrapTimesList.begin();
		if (m_drawFromX == m_drawToX)
		{
			it++;
		}
		//For drawing solid rect
		::SetBkColor(m_hmemdc, convertColor(m_pStyle->getTheme()->timeBgColor));
		for (; it != m_unwrapTimesList.end(); it++)
		{
			int width = (*it)->eventCount * m_pStyle->pFontsTicks->tickWidth;
			tmprect.left = rect.left + (LONG)(((*it)->time - m_unwrapTimesList.front()->time) * m_timeScale + ticks * m_pStyle->pFontsTicks->tickWidth - m_chartShift);
			if (tmprect.left < rect.left)
				tmprect.left = rect.left;
			if (*(*it) == m_drawFromX)
			{
				width -= m_drawFromEventIndex * m_pStyle->pFontsTicks->tickWidth + m_chartShift;
			}
			if (*(*it) == m_drawToX)
			{
				width = m_drawToEventCount * m_pStyle->pFontsTicks->tickWidth;
			}
			tmprect.right = tmprect.left + width;
			if (tmprect.right > rect.right)
				tmprect.right = rect.right;
			//MFC's FillSolidRect do the same thing
			::ExtTextOut(m_hmemdc, 0, 0, ETO_OPAQUE, tmprect, NULL, 0, NULL);
			ticks += (*it)->eventCount;
			if (*(*it) == m_drawFromX)
			{
				ticks -= m_drawFromEventIndex;
			}
		}
		::SelectClipRgn(m_hmemdc, NULL);
	}
}

void ChartView::copyToClipboard()
{
	if (!OpenClipboard() || !::EmptyClipboard())
		return;

	m_mutex.Lock();

	if (m_hmemdc)
	{
		HBITMAP hbm = ::CreateCompatibleBitmap(m_hmemdc, m_newClientRect.Width(), m_newClientRect.Height());
		if (hbm)
		{
			HDC hdcDst = ::CreateCompatibleDC(m_hmemdc);
			if (hdcDst)
			{
				HGDIOBJ oldObj = ::SelectObject(hdcDst, hbm);
				::BitBlt(hdcDst, 0, 0, m_newClientRect.Width(), m_newClientRect.Height(), m_hmemdc, m_newClientRect.left, m_newClientRect.top, SRCCOPY);
				::SelectObject(hdcDst, oldObj);
				::DeleteDC(hdcDst);
				::SetClipboardData(CF_BITMAP, hbm);
			}
			else
			{
				::DeleteObject(hbm);
			}
		}
	}

	CloseClipboard();

	m_mutex.Unlock();
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
		Invalidate();
		updateWindow();
	//}
}

void ChartView::onDraw()
{
	//Document and view are locked from OnPaint()

	RDOStudioChartDoc* doc = getDocument();

	CRect rect;
	rect.CopyRect(&m_newClientRect);

	//MFC's FillSolidRect do the same thing
	::SetBkColor(m_hmemdc, convertColor(m_pStyle->theme->backgroundColor));
	::ExtTextOut(m_hmemdc, 0, 0, ETO_OPAQUE, m_newClientRect, NULL, 0, NULL);

	drawTitle(m_chartRect);

	if (m_needDrawLegend)
	{
		drawLegend(m_legendRect);
	}

	if (m_chartRect.Height() > 0 && m_chartRect.Width() > 0)
	{
		HPEN pen_chart = NULL;
		HPEN old_pen = NULL;
		try
		{
			pen_chart = ::CreatePen(PS_SOLID, 0, convertColor(m_pStyle->getTheme()->defaultColor));
			old_pen = (HPEN)::SelectObject(m_hmemdc, pen_chart);

			drawYAxis(m_chartRect, m_pYAxis);
			setFromTo();
			drawXAxis(m_chartRect);
			drawGrid(m_chartRect);

			::SelectObject(m_hmemdc, old_pen);
			::DeleteObject(pen_chart);
			pen_chart = NULL;
		}
		catch (...)
		{
			if (pen_chart)
			{
				::SelectObject(m_hmemdc, old_pen);
				::DeleteObject(pen_chart);
				pen_chart = NULL;
			}
		}

		for (std::vector<ChartSerie*>::iterator it = doc->m_serieList.begin(); it != doc->m_serieList.end(); ++it)
		{
			(*it)->drawSerie(this, m_hmemdc, m_chartRect);
		}
	}

	::BitBlt(m_hdc, 0, 0, m_newClientRect.Width(), m_newClientRect.Height(), m_hmemdc, 0, 0, SRCCOPY);
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

void ChartView::OnSize(UINT nType, int cx, int cy)
{
	UNUSED(nType);
	UNUSED(cx);
	UNUSED(cy);

	m_mutex.Lock();

	GetClientRect(&m_newClientRect);

	if (m_newClientRect.Width() > m_bmpRect.Width() || m_newClientRect.Height() > m_bmpRect.Height())
	{
		if (m_hbmp)
		{
			::SelectObject(m_hmemdc, m_hbmpInit);
			::DeleteObject (m_hbmp);
		}
		m_hbmp = CreateCompatibleBitmap(m_hdc, m_newClientRect.Width(), m_newClientRect.Height());
		::SelectObject(m_hmemdc, m_hbmp);
		m_bmpRect = m_newClientRect;
	}

	if (getDocument())
	{
		recalcLayout();
		updateScrollBars(false);
		//setZoom(zoom);
	}

	m_mutex.Unlock();
}

void ChartView::OnChartTimewrap()
{
	m_timeWrapFlag = !m_timeWrapFlag;
	recalcLayout();
	updateScrollBars();
	Invalidate();
	updateWindow();
}

void ChartView::OnUpdateChartTimewrap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(!canUnwrapTime() || m_timeWrapFlag);
	pCmdUI->Enable(canUnwrapTime());
}

LRESULT ChartView::OnUserUpdateChartView(WPARAM wParam, LPARAM lParam)
{
	UNUSED(lParam);

	if (doUnwrapTime() || wParam != UPDATE_TIMETICKS)
	{
		updateView();
	}
	return 0;
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
		inc = -m_chartRect.Width();
		break;

	case SB_PAGEDOWN:
		inc = m_chartRect.Width();
		break;

	case SB_LINEUP:
		inc = -m_pStyle->pFontsTicks->tickWidth;
		break;

	case SB_LINEDOWN:
		inc = m_pStyle->pFontsTicks->tickWidth;
		break;

	case SB_THUMBTRACK:
	{
		GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
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
		::SendMessage(m_hWnd, msg, MAKELONG(scrollNotify, pos), NULL);
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

void ChartView::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CFrameWnd* pwndFrame = (CFrameWnd*)AfxGetMainWnd();
	if (pwndFrame)
	{
		pwndFrame->SendMessage(WM_INITMENUPOPUP, WPARAM(pPopupMenu->m_hMenu), MAKELPARAM(nIndex, bSysMenu));
	}
}

void ChartView::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	if (m_previewMode)
		return;

	CWnd::OnContextMenu(pWnd, pos);
	if (m_popupMenu.m_hMenu)
	{
		m_popupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
	}
}

void ChartView::OnChartZoomZoomin()
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
}

void ChartView::OnChartZoomZoomout()
{
	if (m_zoomAutoFlag)
	{
		m_zoomAutoFlag = !m_zoomAutoFlag;
	}
	double delta = m_zoom > 1 ? -0.5 : -0.1;
	double zoom_new = m_zoom + delta;
	setZoom(zoom_new);
}

void ChartView::OnChartZoomResetzoom()
{
	if (m_zoomAutoFlag)
	{
		m_zoomAutoFlag = !m_zoomAutoFlag;
	}
	setZoom(1);
}

void ChartView::OnUpdateChartZoomZoomin(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_zoomAutoFlag && m_zoom < 5);
}

void ChartView::OnUpdateChartZoomZoomout(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_zoomAutoFlag && ((!doUnwrapTime() && m_zoom > m_zoomAuto) || (doUnwrapTime() && m_zoom > 1)));
}

void ChartView::OnUpdateChartZoomResetzoom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_zoom != 1);
}

const RDOStudioChartViewStyle& ChartView::getStyle() const
{
	return (*m_pStyle);
}

int ChartView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (m_previewMode)
	{
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	else
	{
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
}

void ChartView::setFonts(const rbool needRedraw)
{
	UNUSED(needRedraw);

	if (!m_pStyle)
		return;
	
	m_mutex.Lock();

	LOGFONT lf;
	RDOStudioChartViewTheme* chart_theme = static_cast<RDOStudioChartViewTheme*>(m_pStyle->theme);
	::SelectObject(m_hdc, m_hfontInit);

	if (!m_hfontAxis || ::DeleteObject(m_hfontAxis))
	{
		memset(&lf, 0, sizeof(lf));
		// The negative is to allow for leading
		lf.lfHeight = -MulDiv(m_pStyle->font->size, ::GetDeviceCaps(m_hdc, LOGPIXELSY), 72);
		lf.lfWeight = chart_theme->defaultStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = chart_theme->defaultStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->defaultStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet = BYTE(m_pStyle->font->characterSet);
#pragma warning(disable: 4996)
		strcpy(lf.lfFaceName, m_pStyle->font->name.c_str());
#pragma warning(default: 4996)
		m_hfontAxis = ::CreateFontIndirect(&lf);
	}

	if (!m_hfontTitle || ::DeleteObject(m_hfontTitle))
	{
		memset(&lf, 0, sizeof(lf));
		// The negative is to allow for leading
		lf.lfHeight = -MulDiv(m_pStyle->pFontsTicks->titleFontSize, ::GetDeviceCaps(m_hdc, LOGPIXELSY), 72);
		lf.lfWeight = chart_theme->titleStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = chart_theme->titleStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->titleStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet = BYTE(m_pStyle->font->characterSet);
#pragma warning(disable: 4996)
		strcpy(lf.lfFaceName, m_pStyle->font->name.c_str());
#pragma warning(default: 4996)

		m_hfontTitle = ::CreateFontIndirect(&lf);
	}
	if (!m_hfontLegend || ::DeleteObject(m_hfontLegend))
	{
		memset(&lf, 0, sizeof(lf));
		// The negative is to allow for leading
		lf.lfHeight = -MulDiv(m_pStyle->pFontsTicks->legendFontSize, ::GetDeviceCaps(m_hdc, LOGPIXELSY), 72);
		lf.lfWeight = chart_theme->legendStyle & RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = chart_theme->legendStyle & RDOStyleFont::ITALIC;
		lf.lfUnderline = chart_theme->legendStyle & RDOStyleFont::UNDERLINE;
		lf.lfCharSet = BYTE(m_pStyle->font->characterSet);
#pragma warning(disable: 4996)
		strcpy(lf.lfFaceName, m_pStyle->font->name.c_str());
#pragma warning(default: 4996)

		m_hfontLegend = ::CreateFontIndirect(&lf);
	}

	m_mutex.Unlock();
}

void ChartView::setStyle(RDOStudioChartViewStyle* _style, const rbool needRedraw)
{
	m_pStyle = _style;

	setFonts(false);

	/*if (previewMode) {
	 auto_zoom = 1;
	 setZoom(1);
	 }*/

	if (needRedraw)
	{
		recalcLayout();
		updateScrollBars();
		Invalidate();
		updateWindow();
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

void ChartView::updateWindow()
{
	CRgn updateRgn;
	int rgn_type = GetUpdateRgn(&updateRgn);
	if (rgn_type != NULLREGION && rgn_type != ERROR)
	{
		SendNotifyMessage(WM_PAINT, 0, 0);
	}
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
	getQtParent()->update();
	updateScrollBars(true);
	getDocument()->unlock();
}

void ChartView::OnDestroy()
{
	if (getDocument())
	{
		getDocument()->removeFromViews(GetSafeHwnd());
	}
	if (m_hdc)
	{
		::RestoreDC(m_hdc, m_savedHdc);
	}
	if (m_hmemdc)
	{
		::RestoreDC(m_hmemdc, m_savedHmemdc);
		::DeleteDC (m_hmemdc);
	}
	if (m_hfontTitle)
	{
		::DeleteObject (m_hfontTitle);
	}
	if (m_hfontLegend)
	{
		::DeleteObject (m_hfontLegend);
	}
	if (m_hfontAxis)
	{
		::DeleteObject (m_hfontAxis);
	}
	if (m_hbmp)
	{
		::DeleteObject (m_hbmp);
	}
	CWnd::OnDestroy();
}

void ChartView::attachToDoc()
{
	getDocument()->addToViews(GetSafeHwnd());
}

void ChartView::OnPaint()
{
	RDOStudioChartDoc* doc = getDocument();
	doc->m_mutex.Lock();

	m_mutex.Lock();

	//MFC's realization
	/*PAINTSTRUCT ps;
	 CDC* dc = BeginPaint(&ps);
	 OnPrepareDC(dc);
	 OnDraw(dc);
	 EndPaint(&ps);*/

	PAINTSTRUCT ps;
	::BeginPaint(m_hwnd, &ps);
	onDraw();
	::EndPaint(m_hwnd, &ps);

	m_mutex.Unlock();

	doc->m_mutex.Unlock();
}

void ChartView::OnViewZoomauto()
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
}

void ChartView::OnUpdateViewZoomauto(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_zoomAutoFlag);
}

void ChartView::OnHelpKeyword()
{
	QByteArray ba;
	ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm\n");
	studioApp.callQtAssistant(ba);
}

ChartViewMainWnd::ChartViewMainWnd(RDOStudioChartDoc* pDocument, const rbool preview)
	: m_pContext (NULL     )
	, m_pDocument(pDocument)
	, m_preview  (preview  )
{}

ChartViewMainWnd::~ChartViewMainWnd()
{
	m_thisCWnd.Detach();
}

rbool ChartViewMainWnd::init()
{
	m_thisCWnd.Attach(reinterpret_cast<HWND>(winId()));

	m_pContext = new ChartView(this, m_pDocument, m_preview);
	m_pContext->Create(NULL, NULL, 0, CRect(0, 0, 100, 100), &m_thisCWnd, 0);

	return true;
}

ChartView* ChartViewMainWnd::getContext()
{
	return m_pContext;
}

void ChartViewMainWnd::resizeEvent(PTR(QResizeEvent) event)
{
	parent_type::resizeEvent(event);

	if (!m_pContext)
		return;

	QSize size(event->size());
	m_pContext->MoveWindow(0, 0, size.width(), size.height());
}

void ChartViewMainWnd::paintEvent(PTR(QPaintEvent) event)
{
	parent_type::paintEvent(event);

	if (!m_pContext)
		return;

	CRect rect(event->rect().left(), event->rect().top(), event->rect().right(), event->rect().bottom());
	m_pContext->InvalidateRect(&rect);
}

void ChartViewMainWnd::closeEvent(PTR(QCloseEvent) event)
{
	ASSERT(m_pContext);
	m_pContext->DestroyWindow();
	delete m_pContext;

	parent_type::closeEvent(event);
}
