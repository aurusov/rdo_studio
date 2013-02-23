/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerbase.h
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_RDOTRACERBASE_H_
#define _RDO_STUDIO_TRACER_RDOTRACERBASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qstring.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "app/rdo_studio/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- TracerBase
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(TracerResType);
PREDECLARE_POINTER(TracerResource);
PREDECLARE_POINTER(TracerPattern);
PREDECLARE_POINTER(TracerOperationBase);
PREDECLARE_POINTER(TracerEvent);
PREDECLARE_POINTER(TracerResult);
class TracerTimeNow;
PREDECLARE_POINTER(TracerSerie);
class TracerResParamInfo;

class ChartTree;
class RDOStudioChartDoc;

namespace rdo { namespace gui { namespace tracer {

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
	RDOStudioChartDoc* createNewChart();
	void addChart(RDOStudioChartDoc* const chart);
	void removeChart(RDOStudioChartDoc* chart);
	RDOStudioChartDoc* addSerieToChart(CREF(LPTracerSerie) pSerie, RDOStudioChartDoc* chart = NULL);
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

	TracerResParamInfo* getParam(rdo::textstream& stream);
	TracerResParamInfo* getParamType(rdo::textstream& stream);

	typedef  std::vector<LPTracerResType>        ResourceTypeList;
	typedef  std::vector<LPTracerResource>       ResourceList;
	typedef  std::vector<LPTracerPattern>        PatternList;
	typedef  std::vector<LPTracerOperationBase>  OperationBaseList;
	typedef  std::vector<LPTracerEvent>          EventList;
	typedef  std::vector<LPTracerResult>         ResultList;
	typedef  std::list<TracerTimeNow*>           TimeList;
	typedef  std::vector<RDOStudioChartDoc*>     DocumentList;

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

	TracerTimeNow* addTime(CREF(tstring) time);
	int m_eventIndex;

	LPTracerOperationBase getOperation(REF(tstring) line);void startAction(REF(tstring) line, TracerTimeNow* const time);
	void accomplishAction(REF(tstring) line, TracerTimeNow* const time);
	void irregularEvent(REF(tstring) line, TracerTimeNow* const time);
	void productionRule(REF(tstring) line, TracerTimeNow* const time);

	LPTracerResource getResource(REF(tstring) line);
	LPTracerResource resourceCreation(REF(tstring) line, TracerTimeNow* const time);
	LPTracerResource resourceElimination(REF(tstring) line, TracerTimeNow* const time);
	enum TracerResUpdateAction
	{
		RUA_NONE, RUA_ADD, RUA_UPDATE
	};
	TracerResUpdateAction m_updateAction;
	LPTracerResource      m_pResource;
	LPTracerResource resourceChanging(REF(tstring) line, TracerTimeNow* const time);

	LPTracerResult getResult(REF(tstring) line);void resultChanging(REF(tstring) line, TracerTimeNow* const time);

	TimeList m_timeList;

	void clearCharts();
	void deleteTrace();

	UINT         m_clipboardFormat;
	DocumentList m_documentList;
	rbool        m_drawTrace;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RDOTRACERBASE_H_
