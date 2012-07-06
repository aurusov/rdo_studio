/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      doc_serie.h
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_CHART_DOC_SERIE_H_
#define _RDO_STUDIO_MFC_CHART_DOC_SERIE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioDocSerie
// --------------------------------------------------------------------------------
class RDOTracerTimeNow;
class RDOStudioChartView;

class RDOStudioDocSerie
{
friend class RDOStudioChartDoc;
friend class RDOStudioChartOptionsChart;
friend class RDOStudioChartOptionsSeries;
protected:
	RDOTracerSerie* serie;
	COLORREF color;
	RDOTracerSerieMarker marker;
	rbool needDrawMarker;
	int marker_size;
	tstring docSerieTitle;
	rbool showInLegend;
	rbool transparentMarker;
public:
	RDOStudioDocSerie( RDOTracerSerie* _serie );
	~RDOStudioDocSerie();
	RDOTracerSerie* getSerie() const { return serie; };
	COLORREF getColor() const { return color; };
	rbool isTracerSerie( const RDOTracerSerie* _serie ) { return serie == _serie; };
	void  drawSerie( RDOStudioChartView* const view, HDC &dc, CRect &rect ) const { serie->drawSerie( view, dc, rect, color, marker, marker_size, needDrawMarker, transparentMarker ); };
	void  getCaptions( std::vector<tstring> &captions, const int val_count ) const { serie->getCaptions( captions, val_count ); };
	void  getLegendExtent( HDC &dc, CRect& rect, SIZE& size ) const;
	void  drawInLegend( HDC &dc, CRect &rect, const COLORREF text_color, SIZE& size ) const;
	void  lock() { serie->mutex.Lock(); };
	void  unlock() { serie->mutex.Unlock(); };
	rbool empty() const { return serie->empty(); };
};

#endif // _RDO_STUDIO_MFC_CHART_DOC_SERIE_H_
