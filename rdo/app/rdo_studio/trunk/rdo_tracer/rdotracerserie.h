/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerserie.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERSERIE_H_
#define _RDO_STUDIO_TRACER_RDOTRACERSERIE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_tree_item.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerSerie
// --------------------------------------------------------------------------------
enum TracerSerieKind
{
	RDOST_RESPARAM = 0,
	RDOST_OPERATION,
	RDOST_RESULT,
	RDOST_PREVIEW
};

enum TracerSerieMarker
{
	RDOSM_NONE = 0,
	RDOSM_CIRCLE,
	RDOSM_SQUARE,
	RDOSM_TRIANG,
	RDOSM_CROSS
};

class ChartView;
class RDOStudioChartDoc;
class TracerValue;
class TracerTimeNow;

typedef std::list<TracerValue*> valuesList;

class TracerSerie: public ChartTreeItem
{
DECLARE_FACTORY(TracerSerie)
friend class ChartSerie;

public:
	TracerSerieKind getSerieKind() const
	{
		return serieKind;
	}

	CREF(QString) getTitle() const;
	void setTitle(CREF(QString) value);

	void addValue(TracerValue* const value);
	void getValueCount(int& count) const;
	rbool empty() const
	{
		return values.empty();
	}

	valuesList::const_iterator begin() const
	{
		return values.begin();
	}

	valuesList::const_iterator end() const
	{
		return values.end();
	}

	void getLastValue(TracerValue*& val) const;
	virtual void getCaptions(std::vector<tstring>& captions, const int valueCount) const;

	void drawSerie(ChartView* const view,
	               QPainter& painter,
	               const QRect& rect,
	               const QColor& color,
	               TracerSerieMarker marker,
	               const int markerSize,
	               const rbool draw_marker,
	               const rbool transparent_marker) const;
	void drawMarker(QPainter& painter, const int x, const int y, TracerSerieMarker marker, const int markerSize) const;

	void addToDoc(RDOStudioChartDoc* const doc);
	void removeFromDoc(RDOStudioChartDoc* const doc);
	rbool isInOneOrMoreDocs() const
	{
		return !documents.empty();
	}

	rbool activateFirstDoc() const;

	typedef std::vector<QString> ExportData;
	ExportData exportData();

protected:
	TracerSerie(TracerSerieKind _serieKind = RDOST_PREVIEW);
	virtual ~TracerSerie();

	TracerSerieKind serieKind;
	QString         title;

	valuesList values;
	mutable double minValue;
	mutable double maxValue;
	int value_count;

	rbool isTemporaryResourceParam() const;

	std::vector<RDOStudioChartDoc*> documents;

	void getCaptionsInt   (std::vector<tstring>& captions, const int valueCount) const;
	void getCaptionsDouble(std::vector<tstring>& captions, const int valueCount) const;
	void getCaptionsBool  (std::vector<tstring>& captions, const int valueCount) const;
};

typedef  rdo::intrusive_ptr<TracerSerie>  LPTracerSerie;

#endif // _RDO_STUDIO_TRACER_RDOTRACERSERIE_H_
