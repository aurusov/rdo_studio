/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_serie.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_SERIE_H_
#define _RDO_STUDIO_TRACER_SERIE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_tree_item.h"
#include "app/rdo_studio/src/tracer/tracer_values.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class ChartView;
class RDOStudioChartDoc;

class TracerSerie: public ChartTreeItem
{
DECLARE_FACTORY(TracerSerie)
public:
	enum Kind
	{
		SK_PARAM = 0,
		SK_OPERATION,
		SK_RESULT,
		SK_PREVIEW
	};

	enum Marker
	{
		M_NONE = 0,
		M_CIRCLE,
		M_SQUARE,
		M_TRIANG,
		M_CROSS
	};

	typedef std::list<TracerValue*> ValuesList;

	CREF(QString) getTitle() const;
	void          setTitle(CREF(QString) value);

	Kind getKind() const;

	void addValue(TracerValue* const value);
	void getValueCount(int& count) const;

	rbool empty() const;

	ValuesList::const_iterator begin() const;
	ValuesList::const_iterator end  () const;

	void getLastValue(TracerValue*& val) const;
	virtual void getCaptions(std::vector<tstring>& captions, const int valueCount) const;

	void drawSerie(ChartView* const pView,
	               QPainter& painter,
	               const QRect& rect,
	               const QColor& color,
	               Marker marker,
	               const int markerSize,
	               const rbool draw_marker,
	               const rbool transparent_marker) const;
	void drawMarker(QPainter& painter, const int x, const int y, Marker marker, const int markerSize) const;

	void addToDoc(RDOStudioChartDoc* const pDocument);
	void removeFromDoc(RDOStudioChartDoc* const pDocument);
	rbool isInOneOrMoreDocs() const;

	rbool activateFirstDoc() const;

	typedef std::vector<QString> ExportData;
	ExportData exportData();

protected:
	TracerSerie(Kind _serieKind = SK_PREVIEW);
	virtual ~TracerSerie();

	mutable double m_minValue;
	mutable double m_maxValue;

	void getCaptionsInt   (std::vector<tstring>& captions, const int valueCount) const;
	void getCaptionsDouble(std::vector<tstring>& captions, const int valueCount) const;
	void getCaptionsBool  (std::vector<tstring>& captions, const int valueCount) const;

private:
	typedef  std::vector<RDOStudioChartDoc*>  DocumentList;

	Kind          m_kind;
	QString       m_title;
	ValuesList    m_valueList;
	int           m_valueCount;
	DocumentList  m_documentList;

	rbool isTemporaryResourceParam() const;
};

typedef  rdo::intrusive_ptr<TracerSerie>  LPTracerSerie;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_SERIE_H_
