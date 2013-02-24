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
#include "app/rdo_studio/src/tracer/chart/chart_serie.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;

// --------------------------------------------------------------------------------
// -------------------- ChartSerie::Options
// --------------------------------------------------------------------------------
ChartSerie::Options::Options()
	: color            (QColor(0x00, 0x00, 0x00))
	, markerType       (Serie::M_NONE)
	, markerSize       (4)
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
ChartSerie::ChartSerie(CREF(LPSerie) pSerie)
	: m_pSerie(pSerie)
{
	if (m_pSerie)
	{
		m_options.title = m_pSerie->getTitle();
	}
}

ChartSerie::~ChartSerie()
{}

CREF(LPSerie) ChartSerie::getSerie() const
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

rbool ChartSerie::isTracerSerie(CREF(LPSerie) pSerie) const
{
	return m_pSerie == pSerie;
}

void ChartSerie::drawSerie(ChartView* const pView, QPainter& painter, const QRect& rect) const
{
	m_pSerie->drawSerie(pView, painter, rect, m_options.color, m_options.markerType, m_options.markerSize, m_options.markerNeedDraw, m_options.markerTransparent);
}

void ChartSerie::getCaptions(std::vector<tstring>& captions, const int valueCount) const
{
	m_pSerie->getCaptions(captions, valueCount);
}

rbool ChartSerie::empty() const
{
	return m_pSerie->empty();
}

QSize ChartSerie::getLegendSize(const QFontMetrics& fm, const QRect& rect) const
{
	QSize size(0, 0);
	if (!m_options.showInLegend || rect.isEmpty())
		return size;

	QRect tmprect;
	int markerAreaWidth = 10 + m_options.markerSize * 2 + 5;
	tmprect.setLeft(rect.left() + markerAreaWidth);
	tmprect.setRight(rect.right());
	tmprect.setTop(rect.top());
	tmprect.setBottom(rect.bottom());
	tmprect = fm.boundingRect(tmprect, Qt::AlignLeft | Qt::TextSingleLine, m_options.title);
	size.setHeight(tmprect.height());
	if (size.height() < m_options.markerSize * 2)
	{
		size.setHeight(m_options.markerSize * 2);
	}
	size.setWidth(tmprect.right() - rect.left());
	size.setHeight(size.height() + 2);
	if (size.width() > rect.width())
	{
		size.setWidth(rect.width());
	}
	return size;
}

QSize ChartSerie::drawLegend(QPainter& painter, const QRect& rect, const QColor& textColor) const
{
	QSize size = getLegendSize(painter.fontMetrics(), rect);
	if (!m_options.showInLegend || size.isEmpty())
		return size;

	QPoint markerCenter(5 + m_options.markerSize, rect.top() + (size.height() - 2) / 2);
	if (m_options.markerNeedDraw)
	{
		QPen   pen(m_options.color);
		QBrush brush(m_options.color, m_options.markerTransparent ? Qt::NoBrush : Qt::SolidPattern);
		painter.setPen(pen);
		painter.setBrush(brush);
		m_pSerie->drawMarker(painter, rect.left() + markerCenter.x(), markerCenter.y(), m_options.markerType, m_options.markerSize);
	}

	painter.drawLine(rect.left(), markerCenter.y(), rect.left() + markerCenter.x() * 2, markerCenter.y());

	QRect tmprect(rect);
	tmprect.setLeft(tmprect.left() + markerCenter.x() * 2 + 5);

	if (tmprect.isEmpty())
		return size;

	painter.setPen(textColor);
	painter.drawText(tmprect, Qt::AlignLeft | Qt::TextSingleLine, painter.fontMetrics().elidedText(m_options.title, Qt::ElideRight, tmprect.width()));

	return size;
}
