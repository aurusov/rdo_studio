#ifndef RDOTRACERTRACE_H
#define RDOTRACERTRACE_H
#pragma once

#include "rdotracerrdoclasses.h"
#include "../resource.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerTrace
// ----------------------------------------------------------------------------
class RDOTracerLogCtrl;
class RDOTracerTreeCtrl;
class RDOStudioChartDoc;

class RDOTracerTrace  
{
private:
	RDOTracerLogCtrl*   log;
	RDOTracerTreeCtrl*  tree;

	string modelName;
	string statusStr;
	CTime startTime;
	CTime stopTime;
	
	CStdioFile* traceFile;
	UINT        timerID;
	void cleanupObjects();

	void getStructureData();
	void parseStructureData( string& structure );
	
	vector <RDOTracerResType*> resTypes;
	void addResourceType( string& s );
	vector <RDOTracerResource*> resources;
	void addResource( string& s );
	vector <RDOTracerPattern*> patterns;
	void addPattern( string& s );
	vector <RDOTracerOperation*> operations;
	void addOperation( string& s );
	vector <RDOTracerOperation*> irregularEvents;
	void addIrregularEvent( string& s );
	vector <RDOTracerResult*> results;
	void addResult( string& s );
	
	string getNextString();
	void dispathNextString( string& line );

	RDOTracerTimeNow* addTime( string& time );
	int eventIndex;
	
	RDOTracerOperation* getOperation( string& line );
	void startAction( string& line, RDOTracerTimeNow* const time );
	void accomplishAction( string& line, RDOTracerTimeNow* const time  );
	void irregularEvent( string& line, RDOTracerTimeNow* const time  );
	void productionRule( string& line, RDOTracerTimeNow* const time  );
	
	RDOTracerResource* getResource( string& line );
	void resourceCreation( string& line, RDOTracerTimeNow* const time  );
	void resourceElimination( string& line, RDOTracerTimeNow* const time  );
	void resourceChanging( string& line, RDOTracerTimeNow* const time  );
	
	void resultChanging( string& line, RDOTracerTimeNow* const time  );

	vector <RDOTracerTimeNow*> timeVector;
	
	void deleteTrace();
	void clear();

	//void setControls( const bool bstartTrace ) const;

	void doStopTrace();

	bool tracing;
	
	UINT clipboardFormat;

	CMultiDocTemplate* chartDocTemplate;
	vector <RDOStudioChartDoc*> charts;
	void updateCharts();

public:
	RDOTracerTrace();
	virtual ~RDOTracerTrace();	
	
//	void setControls();
	CMultiDocTemplate* createDocTemplate();
	RDOTracerLogCtrl* createLog();
	RDOTracerTreeCtrl* createTree();
	
	void startTrace();
	void getModelStructure();
	void getTraceString();
	string getNextValue( string& line );
	void stopTrace();
	const bool isTracing() const;
	void registerClipboardFormat() { clipboardFormat = ::RegisterClipboardFormat( format(ID_RAO_CLIPBRD).c_str() ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( RDOStudioChartDoc* chart );
	RDOStudioChartDoc* addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	RDOTracerTimeNow* getMaxTime() const { return timeVector.back(); };
	RDOTracerTimeNow* getTime( const int index ) const;
	int getTimesCount() const { return timeVector.size(); };
};

// ----------------------------------------------------------------------------
extern RDOTracerTrace trace;

#endif // RDOTRACERTRACE_H
