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
// -------------------- TracerBase
// --------------------------------------------------------------------------------
class TracerResType;
class TracerResource;
class TracerPattern;
class TracerOperationBase;
class TracerEvent;
class TracerResult;
class TracerTimeNow;
class TracerSerie;
class TracerResParamInfo;

class ChartTree;
class RDOStudioChartDoc;

namespace rdo { namespace gui { namespace tracer {

class LogCtrlView;

class TracerBase: public RDOThreadGUI
{
private:
	CMutex mutex;

	LogCtrlView* log;
	ChartTree*  tree;

	TracerResParamInfo* getParam( rdo::textstream& stream );
	TracerResParamInfo* getParamType( rdo::textstream& stream );
	std::vector <TracerResType*> resTypes;
	void addResourceType( REF(tstring) s, rdo::textstream& stream );
	std::vector <TracerResource*> resources;
	void addResource( REF(tstring) s, rdo::textstream& stream );
	std::vector <TracerPattern*> patterns;
	void addPattern( REF(tstring) s, rdo::textstream& stream );
	std::vector <TracerOperationBase*> operations;
	std::vector <TracerEvent*> irregularEvents;
	void addOperation( REF(tstring) s, rdo::textstream& stream );
	//void addIrregularEvent( REF(tstring) s, rdo::textstream& stream );
	std::vector <TracerResult*> results;
	void addResult( REF(tstring) s, rdo::textstream& stream );
	
	void dispatchNextString( REF(tstring) line );

	TracerTimeNow* addTime( CREF(tstring) time );
	int eventIndex;
	
	TracerOperationBase* getOperation( REF(tstring) line );
	void startAction( REF(tstring) line, TracerTimeNow* const time );
	void accomplishAction( REF(tstring) line, TracerTimeNow* const time  );
	void irregularEvent( REF(tstring) line, TracerTimeNow* const time  );
	void productionRule( REF(tstring) line, TracerTimeNow* const time  );

	TracerResource* getResource( REF(tstring) line );
	TracerResource* resourceCreation( REF(tstring) line, TracerTimeNow* const time  );
	TracerResource* resourceElimination( REF(tstring) line, TracerTimeNow* const time  );
	enum TracerResUpdateAction { RUA_NONE, RUA_ADD, RUA_UPDATE };
	TracerResUpdateAction action;
	TracerResource* resource;
	TracerResource* resourceChanging( REF(tstring) line, TracerTimeNow* const time  );
	
	TracerResult* getResult( REF(tstring) line );
	void resultChanging( REF(tstring) line, TracerTimeNow* const time  );

	std::list< TracerTimeNow* > timeList;
	
	void clearCharts();
	void deleteTrace();

	UINT clipboardFormat;

	std::vector <RDOStudioChartDoc*> charts;

	rbool drawTrace;

protected:
	TracerBase( CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui );
	virtual ~TracerBase();

public:
	void setLog (PTR(LogCtrlView) pTracerLog);
	void setTree(PTR(ChartTree)   pTreeCtrl );

	void startTrace();
	void getModelStructure( rdo::textstream& stream );
	void getTraceString( tstring trace_string );
	tstring getNextValue( REF(tstring) line );
	void registerClipboardFormat() { clipboardFormat = ::RegisterClipboardFormat( rdo::format(ID_RAO_CLIPBRD).c_str() ); }
	UINT const getClipboardFormat() const { return clipboardFormat; }
	RDOStudioChartDoc* createNewChart();
	void addChart( RDOStudioChartDoc* const chart );
	void removeChart( RDOStudioChartDoc* chart );
	RDOStudioChartDoc* addSerieToChart( TracerSerie* const serie, RDOStudioChartDoc* chart = NULL );
	void updateChartsStyles() const;
	void clear();
	void setModelName( tstring name ) const;
	void setDrawTrace( const rbool value );
	rbool getDrawTrace() const { return drawTrace; };
	void lock()   { mutex.Lock(); };
	void unlock() { mutex.Unlock(); };
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERBASE_H_
