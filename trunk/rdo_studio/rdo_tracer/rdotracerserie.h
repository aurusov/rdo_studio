#ifndef RDOTRACERSERIE_H
#define RDOTRACERSERIE_H
#pragma once

#include "tracer_ctrls/rdotracertreeitem.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerSerie
// ----------------------------------------------------------------------------
enum RDOTracerSerieKind{
	RDOST_RESPARAM = 0,
	RDOST_OPERATION,
	RDOST_RESULT,
	RDOST_PREVIEW
};

enum RDOTracerSerieMarker{
	RDOSM_NONE = 0,
	RDOSM_CIRCLE,
	RDOSM_SQUARE,
	RDOSM_RHOMB,
	RDOSM_CROSS
};

class RDOStudioChartView;
class RDOStudioChartDoc;
class RDOTracerValue;
class RDOTracerTimeNow;

typedef std::list< RDOTracerValue* > valuesList;

class RDOTracerSerie : public RDOTracerTreeItem
{
friend class RDOStudioDocSerie;

protected:
	RDOTracerSerieKind serieKind;
	std::string title;

	valuesList values;
	double minValue;
	double maxValue;

	bool isTemporaryResourceParam() const;

	std::vector< RDOStudioChartDoc* > documents;
public:
	RDOTracerSerie( RDOTracerSerieKind _serieKind = RDOST_PREVIEW );
	virtual ~RDOTracerSerie();
	
	RDOTracerSerieKind getSerieKind() const { return serieKind; };

	std::string getTitle() const;
	void setTitle( const std::string& value );

	int addValue( RDOTracerValue* const value );
	int getValueCount() const;
	valuesList::const_iterator begin() const { return values.begin(); };
	valuesList::const_iterator end() const { return values.end(); };
	RDOTracerValue* getLastValue() const;
	double getMinValue() const { return minValue; };
	double getMaxValue() const { return maxValue; };
	virtual void getCaptions( std::vector<std::string> &captions, const int val_count ) const;
	void getCaptionsInt( std::vector<std::string> &captions, const int val_count ) const;
	void getCaptionsDouble( std::vector<std::string> &captions, const int val_count ) const;
	void getCaptionsBool( std::vector<std::string> &captions, const int val_count ) const;

	void drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect, const COLORREF color, RDOTracerSerieMarker marker, const int marker_size, const bool draw_marker, const bool transparent_marker ) const;
	void drawMarker( CDC &dc, const int x, const int y, const COLORREF color, RDOTracerSerieMarker marker, const int marker_size, const bool transparent_marker ) const;
	void drawSircle( CDC &dc, CRect& rect, const COLORREF color ) const;
	void drawSquare( CDC &dc, CRect& rect, const COLORREF color ) const;
	void drawRhomb( CDC &dc, CRect& rect, const COLORREF color ) const;
	void drawCross( CDC &dc, CRect& rect, const COLORREF color ) const;

	int addToDoc( RDOStudioChartDoc* const doc );
	void removeFromDoc( RDOStudioChartDoc* const doc );
	bool isInOneOrMoreDocs() const { return !documents.empty(); };
	bool activateFirstDoc() const;
};

#endif // RDOTRACERSERIE_H