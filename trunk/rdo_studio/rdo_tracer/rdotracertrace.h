#ifndef RDOTRACERTRACE_H
#define RDOTRACERTRACE_H
#pragma once

#include "../resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------
class RDOTracerResType;
class RDOTracerResource;
class RDOTracerPattern;
class RDOTracerOperation;
class RDOTracerResult;
class RDOTracerTimeNow;
class RDOTracerSerie;

class RDOTracerTreeCtrl;
class RDOStudioChartDoc;

namespace rdoTracerLog {
class RDOTracerLogCtrl;
}

namespace rdoTracer {

class RDOTracer  
{
private:
	rdoTracerLog::RDOTracerLogCtrl* log;
	RDOTracerTreeCtrl*  tree;

	std::string modelName;
	std::string statusStr;
	CTime startTime;
	CTime stopTime;
	
	CStdioFile* traceFile;
	UINT        timerID;
	void cleanupObjects();

	void getStructureData();
	void parseStructureData( std::string& structure );
	
	std::vector <RDOTracerResType*> resTypes;
	void addResourceType( std::string& s );
	std::vector <RDOTracerResource*> resources;
	void addResource( std::string& s );
	std::vector <RDOTracerPattern*> patterns;
	void addPattern( std::string& s );
	std::vector <RDOTracerOperation*> operations;
	void addOperation( std::string& s );
	std::vector <RDOTracerOperation*> irregularEvents;
	void addIrregularEvent( std::string& s );
	std::vector <RDOTracerResult*> results;
	void addResult( std::string& s );
	
	std::string getNextString();
	void dispathNextString( std::string& line );

	RDOTracerTimeNow* addTime( std::string& time );
	int eventIndex;
	
	RDOTracerOperation* getOperation( std::string& line );
	void startAction( std::string& line, RDOTracerTimeNow* const time );
	void accomplishAction( std::string& line, RDOTracerTimeNow* const time  );
	void irregularEvent( std::string& line, RDOTracerTimeNow* const time  );
	void productionRule( std::string& line, RDOTracerTimeNow* const time  );
	
	RDOTracerResource* getResource( std::string& line );
	void resourceCreation( std::string& line, RDOTracerTimeNow* const time  );
	void resourceElimination( std::string& line, RDOTracerTimeNow* const time  );
	void resourceChanging( std::string& line, RDOTracerTimeNow* const time  );
	
	void resultChanging( std::string& line, RDOTracerTimeNow* const time  );

	std::list< RDOTracerTimeNow* > timeList;
	//double minTimeDifference;
	
	void deleteTrace();
	void clear();

	//void setControls( const bool bstartTrace ) const;

	void doStopTrace();

	bool tracing;
	
	UINT clipboardFormat;

	CMultiDocTemplate* chartDocTemplate;
	std::vector <RDOStudioChartDoc*> charts;
	void updateCharts();

public:
	RDOTracer();
	virtual ~RDOTracer();	
	
//	void setControls();
	CMultiDocTemplate* createDocTemplate();
	rdoTracerLog::RDOTracerLogCtrl* createLog();
	RDOTracerTreeCtrl* createTree();
	
	void startTrace();
	void getModelStructure();
	void getTraceString();
	std::string getNextValue( std::string& line );
	void stopTrace();
	const bool isTracing() const;
	void registerClipboardFormat() { clipboardFormat = ::RegisterClipboardFormat( format(ID_RAO_CLIPBRD).c_str() ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( RDOStudioChartDoc* chart );
	RDOStudioChartDoc* addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	RDOTracerTimeNow* getMaxTime() const { return timeList.empty() ? NULL : timeList.back(); };
	RDOTracerTimeNow* getTime( const int index ) const;
	int getTimesCount() const { return timeList.size(); };
//	double getMinTimeDifference() const { return minTimeDifference; };
	void updateChartsStyles() const;
};

}; // namespace rdoTracer

// ----------------------------------------------------------------------------
extern rdoTracer::RDOTracer tracer;

#endif // RDOTRACERTRACE_H
