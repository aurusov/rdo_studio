#ifndef RDOSTUDIODOCSERIE_H
#define RDOSTUDIODOCSERIE_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdo_tracer/rdotracerserie.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioDocSerie
// ----------------------------------------------------------------------------
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
	bool needDrawMarker;
	int marker_size;
	std::string docSerieTitle;
	bool showInLegend;
public:
	RDOStudioDocSerie( RDOTracerSerie* _serie );
	~RDOStudioDocSerie();
	RDOTracerSerie* getSerie() const { return serie; };
	COLORREF getColor() const { return color; };
	//bool operator ==( const RDOTracerSerie* _serie ) const { return serie == _serie; };
	bool isTracerSerie( const RDOTracerSerie* _serie ) { return serie == _serie; };
	void drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect ) const { serie->drawSerie( view, dc, rect, color, marker, marker_size, needDrawMarker ); };
	void getCaptions( std::vector<std::string> &captions, const int val_count ) const { serie->getCaptions( captions, val_count ); };
	CSize getLegendExtent( CDC &dc, CFont& font, CRect& rect ) const;
	CSize drawInLegend( CDC &dc, CRect &rect, CFont& font, const COLORREF text_color ) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIODOCSERIE_H
