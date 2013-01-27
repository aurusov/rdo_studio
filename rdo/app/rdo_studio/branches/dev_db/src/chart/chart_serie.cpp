/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_serie.cpp
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_serie.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- ChartSerie::Options
// --------------------------------------------------------------------------------
ChartSerie::Options::Options()
	: color            (RGB(0x00, 0x00, 0x00))
	, markerType       (RDOSM_NONE)
	, markerSize       (3)
	, markerNeedDraw   (true)
	, markerTransparent(true)
	, showInLegend     (true)
{}

rbool ChartSerie::Options::operator== (CREF(Options) options) const
{
	return title             == options.title
		&& color             == options.color
		&& markerType        == options.markerType
		&& markerSize        == options.markerSize
		&& markerNeedDraw    == options.markerNeedDraw
		&& markerTransparent == options.markerTransparent
		&& showInLegend      == options.showInLegend;
}

// --------------------------------------------------------------------------------
// -------------------- ChartSerie
// --------------------------------------------------------------------------------
ChartSerie::ChartSerie(TracerSerie* pSerie)
	: m_pSerie(pSerie)
{
	if (m_pSerie)
	{
		m_options.title = m_pSerie->getTitle();
	}
}

ChartSerie::~ChartSerie()
{}

TracerSerie* ChartSerie::getSerie() const
{
	return m_pSerie;
}

CREF(ChartSerie::Options) ChartSerie::options() const
{
	return m_options;
}

void ChartSerie::setOptions(CREF(Options) options)
{
	m_options = options;
}

rbool ChartSerie::isTracerSerie(const TracerSerie* pSerie) const
{
	return m_pSerie == pSerie;
}

void ChartSerie::drawSerie(RDOStudioChartView* const pView, HDC &dc, CRect &rect) const
{
	m_pSerie->drawSerie(pView, dc, rect, m_options.color, m_options.markerType, m_options.markerSize, m_options.markerNeedDraw, m_options.markerTransparent);
}

void ChartSerie::getCaptions(std::vector<tstring> &captions, const int val_count) const
{
	m_pSerie->getCaptions(captions, val_count);
}

void ChartSerie::lock()
{
	m_pSerie->mutex.Lock();
}

void ChartSerie::unlock()
{
	m_pSerie->mutex.Unlock();
}

rbool ChartSerie::empty() const
{
	return m_pSerie->empty();
}

void ChartSerie::getLegendExtent(HDC &dc, CRect& rect, SIZE& size) const
{
	size.cx = 0;
	size.cy = 0;
	if (!m_options.showInLegend)
		return;
	
	CRect tmprect;
	tmprect.left   = rect.left + 10 + m_options.markerSize * 2 + 5;
	tmprect.right  = rect.right;
	tmprect.top    = rect.top;
	tmprect.bottom = rect.bottom;
	::DrawText(dc, m_options.title.c_str(), m_options.title.length(), tmprect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CALCRECT);
	size.cy = tmprect.Height();
	if (size.cy < m_options.markerSize * 2)
	{
		size.cy = m_options.markerSize * 2;
	}
	size.cx = tmprect.right - rect.left;
	size.cy += 2;
}

void ChartSerie::drawInLegend(HDC &dc, CRect &rect, const COLORREF text_color, SIZE& size) const
{
	getLegendExtent(dc, rect, size);
	if (!m_options.showInLegend)
		return;
	
	::SetTextColor(dc, text_color);

	HPEN pen     = NULL;
	HPEN old_pen = NULL;
	
	HBRUSH brush_marker = NULL;
	HBRUSH old_brush    = NULL;
	LOGBRUSH log_brush;
	log_brush.lbStyle = m_options.markerTransparent ? BS_HOLLOW : BS_SOLID;
	log_brush.lbColor = m_options.color;
	try
	{
		pen     = ::CreatePen(PS_SOLID, 0, m_options.color);
		old_pen = (HPEN)::SelectObject(dc, pen);
		
		brush_marker = ::CreateBrushIndirect(&log_brush);
		old_brush    = (HBRUSH)::SelectObject(dc, brush_marker);
		
		int middle = rect.top + (size.cy - 2) / 2;
		if (m_options.markerNeedDraw)
		{
			m_pSerie->drawMarker(dc, rect.left + 5 + m_options.markerSize, middle, m_options.markerType, m_options.markerSize);
		}
		
		::MoveToEx(dc, rect.left, middle, (LPPOINT)NULL);
		::LineTo(dc, rect.left + 10 + m_options.markerSize * 2, middle);

		CRect tmprect;
		tmprect.CopyRect(rect);
		tmprect.left += 10 + m_options.markerSize * 2 + 5;

		::DrawText(dc, m_options.title.c_str(), m_options.title.length(), tmprect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);

		::SelectObject(dc, old_pen);
		::DeleteObject(pen);
		pen = NULL;

		::SelectObject(dc, old_brush);
		::DeleteObject(brush_marker);
		brush_marker = NULL;
	}
	catch(...)
	{
		if (pen)
		{
			::SelectObject(dc, old_pen);
			::DeleteObject(pen);
			pen = NULL;
		}
		if (brush_marker)
		{
			::SelectObject(dc, old_brush);
			::DeleteObject(brush_marker);
			brush_marker = NULL;
		}
	}
}
