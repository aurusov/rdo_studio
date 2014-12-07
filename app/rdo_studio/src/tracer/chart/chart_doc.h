#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_serie.h"
#include "app/rdo_studio/src/tracer/tracer_serie.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class Time;
class Value;
class ChartSerie;
class ChartViewStyle;

class ChartDoc
{
public:
    enum Update
    {
        U_NEW_SERIE,
        U_NEW_VALUE,
        U_TIME_TICKS,
    };

    typedef  std::list<Time*>          TimesList;
    typedef  std::vector<ChartSerie*>  SerieList;

    ChartDoc(const bool preview = false);
    virtual ~ChartDoc();

    void attachView(ChartView* pView);
    ChartView* getFirstView();

    const QString& getTitle () const;
    void           setTitle (const QString& title);
    void          autoTitle();
    static void   resetTitleIndex();

    void setStyle(ChartViewStyle* pStyle);

    void updateAllViews();

    void addSerie(const LPSerie& pSerie);
    bool serieExists(const LPSerie& pSerie) const;

    void incTimeEventsCount(Time* time);
    bool newValueToSerieAdded(Value* val);

    void addToViews     (ChartView* pWidget);
    void removeFromViews(ChartView* pWidget);

    const TimesList& getTimes    () const;
    const SerieList& getSerieList() const;

    int    getMaxMarkerSize() const;
    int    getTicksCount   () const;
    double getMinTimeOffset() const;

private:
    TimesList m_docTimes;
    TimesList::iterator m_insertedIt;
    SerieList m_serieList;
    double m_minTimeOffset;
    int m_ticksCount;
    bool m_previewMode;

    std::vector<ChartView*> m_widgetList;
    QString m_title;
    std::vector<ChartView*> m_viewList;
    static std::size_t s_titleIndex;

    int getSerieIndex(ChartSerie* serie) const;
    QColor selectColor() const;
    Serie::Marker selectMarker();

    void updateChartViews(Update updateType) const;

    void insertValue(Value* pValue);
};

}}} // namespace rdo::gui::tracer
