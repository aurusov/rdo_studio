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
#include <QtGui/qpainter.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class ChartView;

class ChartSerie
{
public:
	ChartSerie(CREF(LPTracerSerie) pSerie);
	~ChartSerie();

	struct Options
	{
		QString              title;
		QColor               color;
		TracerSerie::Marker  markerType;
		int                  markerSize;
		rbool                markerNeedDraw;
		rbool                markerTransparent;
		rbool                showInLegend;

		Options();
		rbool operator== (CREF(Options) options) const;
	};

	CREF(LPTracerSerie) getSerie() const;

	CREF(Options) options   () const;
	void          setOptions(CREF(Options) options);

	rbool isTracerSerie(CREF(LPTracerSerie) pSerie) const;
	void  drawSerie    (ChartView* const pView, QPainter& painter, const QRect& rect) const;
	void  getCaptions  (std::vector<tstring>& captions, const int valueCount) const;
	QSize getLegendSize(const QFontMetrics& fm, const QRect& rect) const;
	QSize drawLegend   (QPainter& painter, const QRect& rect, const QColor& textColor) const;

	rbool empty () const;

protected:
	LPTracerSerie m_pSerie;
	Options       m_options;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_SERIE_H_
