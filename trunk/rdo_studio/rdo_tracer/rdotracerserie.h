#ifndef RDOTRACERSERIE_H
#define RDOTRACERSERIE_H
#pragma once

#include "./tracer_ctrls/rdotracertreeitem.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerSerie
// ----------------------------------------------------------------------------
enum RDOTracerSerieKind{
	RDOST_RESPARAM = 0,
	RDOST_OPERATION,
	RDOST_RESULT
};

class RDOStudioChartView;
class RDOStudioChartDoc;
class RDOTracerValue;
class RDOTracerTimeNow;

typedef list< RDOTracerValue* > valuesList;

class RDOTracerSerie : public RDOTracerTreeItem
{
protected:
	RDOTracerSerieKind serieKind;
	string title;

	valuesList values;

	bool isTemporaryResourceParam() const;

	//list< RDOTracerValue* >::const_iterator findValue( RDOTracerValue* const value ) const;

	vector< RDOStudioChartDoc* > documents;
public:
	RDOTracerSerie( RDOTracerSerieKind _serieKind );
	virtual ~RDOTracerSerie();
	
	RDOTracerSerieKind getSerieKind() const { return serieKind; };

	string getTitle() const;
	void setTitle( const string& value );

	int addValue( RDOTracerValue* const value );
	//RDOTracerValue* getValue( const int index ) const;
	//list< RDOTracerValue* >::const_iterator getValueIterator( const int index ) const;
	int getValueCount() const;
	valuesList::const_iterator begin() const { return values.begin(); };
	valuesList::const_iterator end() const { return values.end(); };
	RDOTracerValue* getLastValue() const;
//	double getMinValue() const { return minValue; };
//	double getMaxValue() const { return maxValue; };
	void getFromToValues( RDOTracerTimeNow* const from, const int fromEvent, RDOTracerTimeNow* const to, const int toEvent, RDOTracerValue* &begin, RDOTracerValue* &end ) const;
	void getMinMaxValues( RDOTracerValue* const begin, RDOTracerValue* const end, double &min, double &max ) const;

	void drawSerie( RDOStudioChartView* const view, CDC &dc, CRect &rect, const COLORREF color );

	int addToDoc( RDOStudioChartDoc* const doc );
	void removeFromDoc( RDOStudioChartDoc* const doc );
};

#endif // RDOTRACERSERIE_H