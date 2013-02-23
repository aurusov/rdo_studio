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
	void getModelStructure(rdo::textstream& stream);
	void getTraceString(tstring trace_string);
	tstring getNextValue(REF(tstring) line);
	void registerClipboardFormat()
	{
		clipboardFormat = ::RegisterClipboardFormat(rdo::format(ID_RAO_CLIPBRD).c_str());
	}
	UINT const getClipboardFormat() const
	{
		return clipboardFormat;
	}
	RDOStudioChartDoc* createNewChart();
	void addChart(RDOStudioChartDoc* const chart);
	void removeChart(RDOStudioChartDoc* chart);
	RDOStudioChartDoc* addSerieToChart(CREF(LPTracerSerie) pSerie, RDOStudioChartDoc* chart = NULL);
	void updateChartsStyles() const;
	void clear();void setModelName(CREF(QString) name) const;
	void setDrawTrace(const rbool value);
	rbool getDrawTrace() const;

protected:
	TracerBase(CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui);
	virtual ~TracerBase();

private:
	LogMainWnd* log;
	ChartTree* tree;

	TracerResParamInfo* getParam(rdo::textstream& stream);
	TracerResParamInfo* getParamType(rdo::textstream& stream);

	typedef  std::vector<LPTracerResType>  ResTypeList;

	ResTypeList resTypes;
	void addResourceType(REF(tstring) s, rdo::textstream& stream);

	std::vector<LPTracerResource> resources;
	void addResource(REF(tstring) s, rdo::textstream& stream);

	std::vector<LPTracerPattern> patterns;
	void addPattern(REF(tstring) s, rdo::textstream& stream);

	std::vector<LPTracerOperationBase> operations;
	std::vector<LPTracerEvent> irregularEvents;
	void addOperation(REF(tstring) s, rdo::textstream& stream);
	//void addIrregularEvent(REF(tstring) s, rdo::textstream& stream);

	std::vector<LPTracerResult> results;
	void addResult(REF(tstring) s, rdo::textstream& stream);

	void dispatchNextString(REF(tstring) line);

	TracerTimeNow* addTime(CREF(tstring) time);
	int eventIndex;

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
	TracerResUpdateAction action;
	LPTracerResource resource;
	LPTracerResource resourceChanging(REF(tstring) line, TracerTimeNow* const time);

	LPTracerResult getResult(REF(tstring) line);void resultChanging(REF(tstring) line, TracerTimeNow* const time);

	std::list<TracerTimeNow*> timeList;

	void clearCharts();
	void deleteTrace();

	UINT clipboardFormat;

	std::vector<RDOStudioChartDoc*> charts;

	rbool drawTrace;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_RDOTRACERBASE_H_
