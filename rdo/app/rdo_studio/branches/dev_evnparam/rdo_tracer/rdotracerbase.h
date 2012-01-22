#ifndef RDOTRACERTBASE_H
#define RDOTRACERTBASE_H
#pragma once

#include "app/rdo_studio_mfc/resource.h"
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "utils/rdostream.h"

// --------------------------------------------------------------------------------
// -------------------- RDOTracerBase
// --------------------------------------------------------------------------------
class RDOTracerResType;
class RDOTracerResource;
class RDOTracerPattern;
class RDOTracerOperationBase;
class RDOTracerEvent;
class RDOTracerResult;
class RDOTracerTimeNow;
class RDOTracerSerie;
class RDOTracerResParamInfo;

class RDOTracerTreeCtrl;
class RDOStudioChartDoc;

namespace rdoTracerLog {
class RDOTracerLogCtrl;
}

namespace rdoTracer {

class RDOTracerBase: public RDOThreadGUI
{
private:
	CMutex mutex;

	rdoTracerLog::RDOTracerLogCtrl* log;
	RDOTracerTreeCtrl*  tree;

	RDOTracerResParamInfo* getParam( rdo::textstream& stream );
	RDOTracerResParamInfo* getParamType( rdo::textstream& stream );
	std::vector <RDOTracerResType*> resTypes;
	void addResourceType( std::string& s, rdo::textstream& stream );
	std::vector <RDOTracerResource*> resources;
	void addResource( std::string& s, rdo::textstream& stream );
	std::vector <RDOTracerPattern*> patterns;
	void addPattern( std::string& s, rdo::textstream& stream );
	std::vector <RDOTracerOperationBase*> operations;
	std::vector <RDOTracerEvent*> irregularEvents;
	void addOperation( std::string& s, rdo::textstream& stream );
	//void addIrregularEvent( std::string& s, rdo::textstream& stream );
	std::vector <RDOTracerResult*> results;
	void addResult( std::string& s, rdo::textstream& stream );
	
	void dispatchNextString( std::string& line );

	RDOTracerTimeNow* addTime( const std::string& time );
	int eventIndex;
	
	RDOTracerOperationBase* getOperation( std::string& line );
	void startAction( std::string& line, RDOTracerTimeNow* const time );
	void accomplishAction( std::string& line, RDOTracerTimeNow* const time  );
	void irregularEvent( std::string& line, RDOTracerTimeNow* const time  );
	void productionRule( std::string& line, RDOTracerTimeNow* const time  );

	RDOTracerResource* getResource( std::string& line );
	RDOTracerResource* resourceCreation( std::string& line, RDOTracerTimeNow* const time  );
	RDOTracerResource* resourceElimination( std::string& line, RDOTracerTimeNow* const time  );
	enum RDOTracerResUpdateAction { RUA_NONE, RUA_ADD, RUA_UPDATE };
	RDOTracerResUpdateAction action;
	RDOTracerResource* resource;
	RDOTracerResource* resourceChanging( std::string& line, RDOTracerTimeNow* const time  );
	
	RDOTracerResult* getResult( std::string& line );
	void resultChanging( std::string& line, RDOTracerTimeNow* const time  );

	std::list< RDOTracerTimeNow* > timeList;
	
	void clearCharts();
	void deleteTrace();

	UINT clipboardFormat;

	CMultiDocTemplate* chartDocTemplate;
	std::vector <RDOStudioChartDoc*> charts;

	bool drawTrace;

protected:
	RDOTracerBase( const std::string& _thread_name, RDOKernelGUI* _kernel_gui );
	virtual ~RDOTracerBase();

public:
	CMultiDocTemplate* createDocTemplate();
	rdoTracerLog::RDOTracerLogCtrl* createLog();
	RDOTracerTreeCtrl* createTree();

	void startTrace();
	void getModelStructure( rdo::textstream& stream );
	void getTraceString( std::string trace_string );
	std::string getNextValue( std::string& line );
	void registerClipboardFormat() { clipboardFormat = ::RegisterClipboardFormat( rdo::format(ID_RAO_CLIPBRD).c_str() ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( RDOStudioChartDoc* chart );
	RDOStudioChartDoc* addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	void updateChartsStyles() const;
	void clear();
	void setModelName( std::string name ) const;
	void setDrawTrace( const bool value );
	bool getDrawTrace() const { return drawTrace; };
	void lock()   { mutex.Lock(); };
	void unlock() { mutex.Unlock(); };
};

}; // namespace rdoTracer

#endif // RDOTRACERTBASE_H
