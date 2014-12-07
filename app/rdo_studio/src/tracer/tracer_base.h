#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QString>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
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
	void setLog(LogMainWnd* pTracerLog);
	void setTree(ChartTree* pTreeCtrl);

	void startTrace();
	void getModelStructure(std::istream& stream);
	void getTraceString(std::string trace_string);
	std::string getNextValue(std::string& line);
	ChartDoc* createNewChart();
	void addChart(ChartDoc* const pDocument);
	void removeChart(ChartDoc* pDocument);
	ChartDoc* addSerieToChart(const LPSerie& pSerie, ChartDoc* pDocument = NULL);
	void updateChartsStyles() const;
	void clear();
	void setModelName(const QString& name) const;
	void setDrawTrace(const bool value);
	bool getDrawTrace() const;

protected:
	TracerBase(const std::string& _thread_name, RDOKernelGUI* _kernel_gui);
	virtual ~TracerBase();

private:
	LogMainWnd* m_pLog;
	ChartTree*  m_pChartTree;

	ParamInfo* getParam(std::istream& stream);
	ParamInfo* getParamType(std::istream& stream);

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

	void addResourceType(std::string& s, std::istream& stream);
	void addResource(std::string& s, std::istream& stream);
	void addPattern(std::string& s, std::istream& stream);
	void addOperation(std::string& s, std::istream& stream);
	void addResult(std::string& s, std::istream& stream);

	void dispatchNextString(std::string& line);

	Time* addTime(const std::string& time);
	int m_eventIndex;

	LPOperationBase getOperation(std::string& line);void startAction(std::string& line, Time* const pTime);
	void accomplishAction(std::string& line, Time* const pTime);
	void irregularEvent(std::string& line, Time* const pTime);
	void productionRule(std::string& line, Time* const pTime);

	LPResource getResource(std::string& line);
	LPResource resourceCreation(std::string& line, Time* const pTime);
	LPResource resourceElimination(std::string& line, Time* const pTime);
	enum TracerResUpdateAction
	{
		RUA_NONE, RUA_ADD, RUA_UPDATE
	};
	TracerResUpdateAction m_updateAction;
	LPResource m_pResource;
	LPResource resourceChanging(std::string& line, Time* const pTime);

	LPResult getResult(std::string& line);void resultChanging(std::string& line, Time* const pTime);

	TimeList m_timeList;

	void clearCharts();
	void deleteTrace();

	DocumentList m_documentList;
	bool         m_drawTrace;
};

}}} // namespace rdo::gui::tracer
