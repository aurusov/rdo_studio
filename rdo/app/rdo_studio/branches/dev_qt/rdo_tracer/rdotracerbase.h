/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerbase.h
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACERBASE_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACERBASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

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
	void addResourceType( REF(tstring) s, rdo::textstream& stream );
	std::vector <RDOTracerResource*> resources;
	void addResource( REF(tstring) s, rdo::textstream& stream );
	std::vector <RDOTracerPattern*> patterns;
	void addPattern( REF(tstring) s, rdo::textstream& stream );
	std::vector <RDOTracerOperationBase*> operations;
	std::vector <RDOTracerEvent*> irregularEvents;
	void addOperation( REF(tstring) s, rdo::textstream& stream );
	//void addIrregularEvent( REF(tstring) s, rdo::textstream& stream );
	std::vector <RDOTracerResult*> results;
	void addResult( REF(tstring) s, rdo::textstream& stream );
	
	void dispatchNextString( REF(tstring) line );

	RDOTracerTimeNow* addTime( CREF(tstring) time );
	int eventIndex;
	
	RDOTracerOperationBase* getOperation( REF(tstring) line );
	void startAction( REF(tstring) line, RDOTracerTimeNow* const time );
	void accomplishAction( REF(tstring) line, RDOTracerTimeNow* const time  );
	void irregularEvent( REF(tstring) line, RDOTracerTimeNow* const time  );
	void productionRule( REF(tstring) line, RDOTracerTimeNow* const time  );

	RDOTracerResource* getResource( REF(tstring) line );
	RDOTracerResource* resourceCreation( REF(tstring) line, RDOTracerTimeNow* const time  );
	RDOTracerResource* resourceElimination( REF(tstring) line, RDOTracerTimeNow* const time  );
	enum RDOTracerResUpdateAction { RUA_NONE, RUA_ADD, RUA_UPDATE };
	RDOTracerResUpdateAction action;
	RDOTracerResource* resource;
	RDOTracerResource* resourceChanging( REF(tstring) line, RDOTracerTimeNow* const time  );
	
	RDOTracerResult* getResult( REF(tstring) line );
	void resultChanging( REF(tstring) line, RDOTracerTimeNow* const time  );

	std::list< RDOTracerTimeNow* > timeList;
	
	void clearCharts();
	void deleteTrace();

	UINT clipboardFormat;

	std::vector <RDOStudioChartDoc*> charts;

	rbool drawTrace;

protected:
	RDOTracerBase( CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui );
	virtual ~RDOTracerBase();

public:
	rdoTracerLog::RDOTracerLogCtrl* createLog();
	RDOTracerTreeCtrl* createTree();

	void startTrace();
	void getModelStructure( rdo::textstream& stream );
	void getTraceString( tstring trace_string );
	tstring getNextValue( REF(tstring) line );
	void registerClipboardFormat() { clipboardFormat = ::RegisterClipboardFormat( rdo::format(ID_RAO_CLIPBRD).c_str() ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( RDOStudioChartDoc* chart );
	RDOStudioChartDoc* addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	void updateChartsStyles() const;
	void clear();
	void setModelName( tstring name ) const;
	void setDrawTrace( const rbool value );
	rbool getDrawTrace() const { return drawTrace; };
	void lock()   { mutex.Lock(); };
	void unlock() { mutex.Unlock(); };
};

}; // namespace rdoTracer

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERBASE_H_
