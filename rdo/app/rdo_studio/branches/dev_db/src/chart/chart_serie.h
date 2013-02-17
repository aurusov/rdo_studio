/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_serie.h
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_CHART_SERIE_H_
#define _RDO_STUDIO_CHART_SERIE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- ChartSerie
// --------------------------------------------------------------------------------
class ChartView;

class ChartSerie
{
public:
	ChartSerie(TracerSerie* pSerie);
	~ChartSerie();

	struct Options
	{
		tstring            title;
		COLORREF           color;
		TracerSerieMarker  markerType;
		int                markerSize;
		rbool              markerNeedDraw;
		rbool              markerTransparent;
		rbool              showInLegend;

		Options();
		rbool operator== (CREF(Options) options) const;
	};

	TracerSerie* getSerie() const;

	CREF(Options) options   () const;
	void          setOptions(CREF(Options) options);

	rbool isTracerSerie  (const TracerSerie* pSerie) const;
	void  drawSerie      (ChartView* const pView, HDC &dc, CRect &rect) const;
	void  getCaptions    (std::vector<tstring> &captions, const int val_count) const;
	void  getLegendExtent(HDC &dc, CRect& rect, SIZE& size) const;
	void  drawInLegend   (HDC &dc, CRect &rect, const COLORREF text_color, SIZE& size) const;

	void  lock  ();
	void  unlock();
	rbool empty () const;

protected:
	TracerSerie*  m_pSerie;
	Options       m_options;
};

#endif // _RDO_STUDIO_CHART_SERIE_H_
