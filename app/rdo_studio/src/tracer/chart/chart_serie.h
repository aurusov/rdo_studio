/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_serie.h
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_SERIE_H_
#define _RDO_STUDIO_TRACER_CHART_SERIE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QPainter>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/tracer_serie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class ChartView;

class ChartSerie
{
public:
	ChartSerie(const LPSerie& pSerie);
	~ChartSerie();

	struct Options
	{
		QString        title;
		QColor         color;
		Serie::Marker  markerType;
		int            markerSize;
		bool           markerNeedDraw;
		bool           markerTransparent;
		bool           showInLegend;

		Options();
		bool operator== (const Options& options) const;
	};

	const LPSerie& getSerie() const;

	const Options& options() const;
	void setOptions(const Options& options);

	bool  isTracerSerie(const LPSerie& pSerie) const;
	void  drawSerie    (ChartView* const pView, QPainter& painter, const QRect& rect) const;
	void  getCaptions  (std::vector<std::string>& captions, const int valueCount) const;
	QSize getLegendSize(const QFontMetrics& fm, const QRect& rect) const;
	QSize drawLegend   (QPainter& painter, const QRect& rect, const QColor& textColor) const;

	bool  empty () const;

protected:
	LPSerie  m_pSerie;
	Options  m_options;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_SERIE_H_
