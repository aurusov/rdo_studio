#ifndef RDOTRACERTRACE_H
#define RDOTRACERTRACE_H
#pragma once

#include <vector>
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

	CString modelName;
	CString statusStr;
	CTime startTime;
	CTime stopTime;
	
	CStdioFile* traceFile;
	UINT        timerID;
	void cleanupObjects();

	void getStructureData();
	void parseStructureData( CString& structure );
	
	vector <RDOTracerResType*> resTypes;
	void addResourceType( CString& s );
	vector <RDOTracerResource*> resources;
	void addResource( CString& s );
	vector <RDOTracerPattern*> patterns;
	void addPattern( CString& s );
	vector <RDOTracerOperation*> operations;
	void addOperation( CString& s );
	vector <RDOTracerOperation*> irregularEvents;
	void addIrregularEvent( CString& s );
	vector <RDOTracerResult*> results;
	void addResult( CString& s );
	
	CString getNextString();
	void dispathNextString( CString& line );

	RDOTracerTimeNow* addTime( CString& time );
	
	RDOTracerOperation* getOperation( CString& line );
	void startAction( CString& line, RDOTracerTimeNow* const time );
	void accomplishAction( CString& line, RDOTracerTimeNow* const time  );
	void irregularEvent( CString& line, RDOTracerTimeNow* const time  );
	void productionRule( CString& line, RDOTracerTimeNow* const time  );
	
	RDOTracerResource* getResource( CString& line );
	void resourceCreation( CString& line, RDOTracerTimeNow* const time  );
	void resourceElimination( CString& line, RDOTracerTimeNow* const time  );
	void resourceChanging( CString& line, RDOTracerTimeNow* const time  );
	
	void resultChanging( CString& line, RDOTracerTimeNow* const time  );

	vector <RDOTracerTimeNow*> timeVector;
	
	void deleteTrace();
	void clear();

	//void setControls( const bool bstartTrace ) const;

	void doStopTrace();

	bool tracing;
	
	UINT clipboardFormat;

	CMultiDocTemplate* chartDocTemplate;
	vector <RDOStudioChartDoc*> charts;

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
	CString getNextValue( CString& line );
	void stopTrace();
	const bool isTracing() const;
	void registerClipboardFormat() { CString str; str.LoadString( ID_RAO_CLIPBRD );	clipboardFormat = ::RegisterClipboardFormat( str ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( const RDOStudioChartDoc* const chart );
	RDOStudioChartDoc* addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	RDOTracerTimeNow* getMaxTime() const { return timeVector.back(); };
};

// ----------------------------------------------------------------------------
extern RDOTracerTrace trace;

#endif // RDOTRACERTRACE_H
