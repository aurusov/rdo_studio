#ifndef RDOTRACERTBASE_H
#define RDOTRACERTBASE_H
#pragma once

#include "../resource.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerBase
// ----------------------------------------------------------------------------
class RDOTracerResType;
class RDOTracerResource;
class RDOTracerPattern;
class RDOTracerOperationBase;
class RDOTracerEvent;
class RDOTracerResult;
class RDOTracerTimeNow;
class RDOTracerSerie;

class RDOTracerTreeCtrl;
class RDOStudioChartDoc;

namespace rdoTracerLog {
class RDOTracerLogCtrl;
}

namespace rdoTracer {

class RDOTracerBase
{
private:
	rdoTracerLog::RDOTracerLogCtrl* log;
	RDOTracerTreeCtrl*  tree;

	std::vector <RDOTracerResType*> resTypes;
	void addResourceType( std::string& s, std::stringstream& stream );
	std::vector <RDOTracerResource*> resources;
	void addResource( std::string& s );
	std::vector <RDOTracerPattern*> patterns;
	void addPattern( std::string& s );
	std::vector <RDOTracerOperationBase*> operations;
	void addOperation( std::string& s );
	std::vector <RDOTracerEvent*> irregularEvents;
	void addIrregularEvent( std::string& s );
	std::vector <RDOTracerResult*> results;
	void addResult( std::string& s );
	
	void dispathNextString( std::string& line );

	RDOTracerTimeNow* addTime( std::string& time );
	int eventIndex;
	
	RDOTracerOperationBase* getOperation( std::string& line );
	void startAction( std::string& line, RDOTracerTimeNow* const time );
	void accomplishAction( std::string& line, RDOTracerTimeNow* const time  );
	void irregularEvent( std::string& line, RDOTracerTimeNow* const time  );
	void productionRule( std::string& line, RDOTracerTimeNow* const time  );
	
	RDOTracerResource* getResource( std::string& line );
	void resourceCreation( std::string& line, RDOTracerTimeNow* const time  );
	void resourceElimination( std::string& line, RDOTracerTimeNow* const time  );
	void resourceChanging( std::string& line, RDOTracerTimeNow* const time  );
	
	RDOTracerResult* getResult( std::string& line );
	void resultChanging( std::string& line, RDOTracerTimeNow* const time  );

	std::list< RDOTracerTimeNow* > timeList;
	
	void deleteTrace();

	UINT clipboardFormat;

	CMultiDocTemplate* chartDocTemplate;
	std::vector <RDOStudioChartDoc*> charts;
	void updateCharts();

public:
	RDOTracerBase();
	virtual ~RDOTracerBase();	
	
	CMultiDocTemplate* createDocTemplate();
	rdoTracerLog::RDOTracerLogCtrl* createLog();
	RDOTracerTreeCtrl* createTree();
	
	void startTrace();
	void getModelStructure( std::stringstream& stream );
	void getTraceString( std::string trace_string );
	std::string getNextValue( std::string& line );
	void registerClipboardFormat() { clipboardFormat = ::RegisterClipboardFormat( format(ID_RAO_CLIPBRD).c_str() ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( RDOStudioChartDoc* chart );
	RDOStudioChartDoc* addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	RDOTracerTimeNow* getMaxTime() const { return timeList.empty() ? NULL : timeList.back(); };
	void updateChartsStyles() const;
	void clear();
	void setModelName( std::string name ) const;
};

}; // namespace rdoTracer

#endif // RDOTRACERTBASE_H
