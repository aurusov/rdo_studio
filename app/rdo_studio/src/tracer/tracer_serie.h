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
class ChartDoc;

class Serie: public ChartTreeItem
{
DECLARE_FACTORY(Serie)
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

	typedef std::list<Value*> ValuesList;

	const QString& getTitle() const;
	void           setTitle(const QString& value);

	Kind getKind() const;

	void addValue(Value* const value);
	void getValueCount(int& count) const;

	bool empty() const;

	ValuesList::const_iterator begin() const;
	ValuesList::const_iterator end  () const;

	void getLastValue(Value*& val) const;
	virtual void getCaptions(std::vector<std::string>& captions, const int valueCount) const;

	void drawSerie(ChartView* const pView,
	               QPainter& painter,
	               const QRect& rect,
	               const QColor& color,
	               Marker marker,
	               const int markerSize,
	               const bool draw_marker,
	               const bool transparent_marker) const;
	void drawMarker(QPainter& painter, const int x, const int y, Marker marker, const int markerSize) const;

	void addToDoc(ChartDoc* const pDocument);
	void removeFromDoc(ChartDoc* const pDocument);
	bool isInOneOrMoreDocs() const;

	bool activateFirstDoc() const;

	typedef std::vector<QString> ExportData;
	ExportData exportData();

protected:
	Serie(Kind _serieKind = SK_PREVIEW);
	virtual ~Serie();

	mutable double m_minValue;
	mutable double m_maxValue;

	void getCaptionsInt(std::vector<std::string>& captions, const int valueCount) const;
	void getCaptionsDouble(std::vector<std::string>& captions, const int valueCount) const;
	void getCaptionsBool(std::vector<std::string>& captions, const int valueCount) const;

private:
	typedef  std::vector<ChartDoc*>  DocumentList;

	Kind          m_kind;
	QString       m_title;
	ValuesList    m_valueList;
	int           m_valueCount;
	DocumentList  m_documentList;

	bool isTemporaryResourceParam() const;
};

typedef  rdo::intrusive_ptr<Serie>  LPSerie;

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_SERIE_H_
