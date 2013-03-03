/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_base.h
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_BASE_H_
#define _RDO_STUDIO_TRACER_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QString>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

PREDECLARE_POINTER(ResourceType);
PREDECLARE_POINTER(Resource);
PREDECLARE_POINTER(Pattern);
PREDECLARE_POINTER(OperationBase);
PREDECLARE_POINTER(Event);
PREDECLARE_POINTER(Result);
PREDECLARE_POINTER(Serie);
class Time;
class ParamInfo;
class ChartTree;
class ChartDoc;
class LogMainWnd;

class TracerBase: public RDOThreadGUI
{
public:
	void setLog (PTR(LogMainWnd) pTracerLog);
	void setTree(PTR(ChartTree) pTreeCtrl);

	void startTrace();
	void    getModelStructure(rdo::textstream& stream);
	void    getTraceString(tstring trace_string);
	tstring getNextValue(REF(tstring) line);
	void registerClipboardFormat();
	UINT const getClipboardFormat() const;
	ChartDoc* createNewChart();
	void addChart(ChartDoc* const pDocument);
	void removeChart(ChartDoc* pDocument);
	ChartDoc* addSerieToChart(CREF(LPSerie) pSerie, ChartDoc* pDocument = NULL);
	void updateChartsStyles() const;
	void clear();
	void setModelName(CREF(QString) name) const;
	void setDrawTrace(const rbool value);
	rbool getDrawTrace() const;

protected:
	TracerBase(CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui);
	virtual ~TracerBase();

private:
	LogMainWnd* m_pLog;
	ChartTree*  m_pChartTree;

	ParamInfo* getParam(rdo::textstream& stream);
	ParamInfo* getParamType(rdo::textstream& stream);

	typedef  std::vector<LPResourceType>   ResourceTypeList;
	typedef  std::vector<LPResource>       ResourceList;
	typedef  std::vector<LPPattern>        PatternList;
	typedef  std::vector<LPOperationBase>  OperationBaseList;
	typedef  std::vector<LPEvent>          EventList;
	typedef  std::vector<LPResult>         ResultList;
	typedef  std::list<Time*>              TimeList;
	typedef  std::vector<ChartDoc*>        DocumentList;

	ResourceTypeList  m_resourceTypeList;
	ResourceList      m_resourceList;
	PatternList       m_patternList;
	OperationBaseList m_operationList;
	EventList         m_eventList;
	ResultList        m_resultList;

	void addResourceType(REF(tstring) s, rdo::textstream& stream);
	void addResource    (REF(tstring) s, rdo::textstream& stream);
	void addPattern     (REF(tstring) s, rdo::textstream& stream);
	void addOperation   (REF(tstring) s, rdo::textstream& stream);
	void addResult      (REF(tstring) s, rdo::textstream& stream);

	void dispatchNextString(REF(tstring) line);

	Time* addTime(CREF(tstring) time);
	int m_eventIndex;

	LPOperationBase getOperation(REF(tstring) line);void startAction(REF(tstring) line, Time* const pTime);
	void accomplishAction(REF(tstring) line, Time* const pTime);
	void irregularEvent(REF(tstring) line, Time* const pTime);
	void productionRule(REF(tstring) line, Time* const pTime);

	LPResource getResource(REF(tstring) line);
	LPResource resourceCreation(REF(tstring) line, Time* const pTime);
	LPResource resourceElimination(REF(tstring) line, Time* const pTime);
	enum TracerResUpdateAction
	{
		RUA_NONE, RUA_ADD, RUA_UPDATE
	};
	TracerResUpdateAction m_updateAction;
	LPResource            m_pResource;
	LPResource resourceChanging(REF(tstring) line, Time* const pTime);

	LPResult getResult(REF(tstring) line);void resultChanging(REF(tstring) line, Time* const pTime);

	TimeList m_timeList;

	void clearCharts();
	void deleteTrace();

	UINT         m_clipboardFormat;
	DocumentList m_documentList;
	rbool        m_drawTrace;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_BASE_H_
