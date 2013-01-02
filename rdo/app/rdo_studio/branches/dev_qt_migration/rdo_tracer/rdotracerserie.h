/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerserie.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACERSERIE_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACERSERIE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerSerie
// --------------------------------------------------------------------------------
enum TracerSerieKind{
	RDOST_RESPARAM = 0,
	RDOST_OPERATION,
	RDOST_RESULT,
	RDOST_PREVIEW
};

enum TracerSerieMarker{
	RDOSM_NONE = 0,
	RDOSM_CIRCLE,
	RDOSM_SQUARE,
	RDOSM_RHOMB,
	RDOSM_CROSS
};

class RDOStudioChartView;
class RDOStudioChartDoc;
class TracerValue;
class TracerTimeNow;

typedef std::list< TracerValue* > valuesList;

class TracerSerie : public ChartTreeItem
{
friend class RDOStudioDocSerie;

protected:
	CMutex mutex;

	TracerSerieKind serieKind;
	tstring title;

	valuesList values;
	mutable double minValue;
	mutable double maxValue;
	int value_count;

	rbool isTemporaryResourceParam() const;

	std::vector< RDOStudioChartDoc* > documents;

	void getCaptionsInt( std::vector<tstring> &captions, const int val_count ) const;
	void getCaptionsDouble( std::vector<tstring> &captions, const int val_count ) const;
	void getCaptionsBool( std::vector<tstring> &captions, const int val_count ) const;

public:
	TracerSerie( TracerSerieKind _serieKind = RDOST_PREVIEW );
	virtual ~TracerSerie();
	
	TracerSerieKind getSerieKind() const { return serieKind; };

	tstring getTitle() const;
	void setTitle( CREF(tstring) value );

	void  addValue( TracerValue* const value );
	void  getValueCount( int& count ) const;
	rbool empty() const { return values.empty(); };
	valuesList::const_iterator begin() const { return values.begin(); };
	valuesList::const_iterator end() const { return values.end(); };
	void getLastValue( TracerValue*& val ) const;
	//double getMinValue() const { return minValue; };
	//double getMaxValue() const { return maxValue; };
	virtual void getCaptions( std::vector<tstring> &captions, const int val_count ) const;

	void drawSerie( RDOStudioChartView* const view, HDC &dc, CRect &rect, const COLORREF color, TracerSerieMarker marker, const int marker_size, const rbool draw_marker, const rbool transparent_marker ) const;
	void drawMarker( HDC &dc, const int x, const int y, TracerSerieMarker marker, const int marker_size ) const;

	void  addToDoc( RDOStudioChartDoc* const doc );
	void  removeFromDoc( RDOStudioChartDoc* const doc );
	rbool isInOneOrMoreDocs() const { return !documents.empty(); };
	rbool activateFirstDoc() const;

	typedef std::vector<tstring> ExportData;
	ExportData exportData();
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERSERIE_H_
