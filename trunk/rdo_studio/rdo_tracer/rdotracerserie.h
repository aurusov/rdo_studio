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

class RDOStudioChartView;
class RDOStudioChartDoc;
class RDOTracerValue;
class RDOTracerTimeNow;

typedef std::list< RDOTracerValue* > valuesList;

class RDOTracerSerie : public RDOTracerTreeItem
{
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

	void drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect, const COLORREF color ) const;
	void drawMarker( CDC &dc, const int x, const int y, const COLORREF color ) const;

	int addToDoc( RDOStudioChartDoc* const doc );
	void removeFromDoc( RDOStudioChartDoc* const doc );
};

#endif // RDOTRACERSERIE_H