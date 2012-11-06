/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerbase.cpp
  \author    Захаров Павел
  \date      01.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerbase.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerrestype.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresource.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerpattern.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotraceroperation.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerresult.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracervalues.h"
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_view.h"
#include "app/rdo_studio_mfc/src/chart/chart_tree.h"
#include "app/rdo_studio_mfc/src/chart/document.h"
#include "app/rdo_studio_mfc/src/chart/view.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracer;
using namespace rdoTracerLog;

// --------------------------------------------------------------------------------
// -------------------- RDOTracerBase
// --------------------------------------------------------------------------------
RDOTracerBase::RDOTracerBase( CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui ):
	RDOThreadGUI( _thread_name, _kernel_gui ),
	log( NULL ),
	tree( NULL ),
	clipboardFormat( 0 ),
	eventIndex( 0 ),
	drawTrace( true ),
	action( RUA_NONE )
{
}

RDOTracerBase::~RDOTracerBase()
{
	deleteTrace();
}

RDOTracerResParamInfo* RDOTracerBase::getParamType( rdo::textstream& stream )
{
	boost::optional<RDOTracerResParamType> parType;

	tstring parTypeName;
	stream >> parTypeName;
	if ( parTypeName == "E" )
	{
		parType = RDOPT_ENUMERATIVE;
	}
	if ( parTypeName == "I" )
	{
		parType = RDOPT_INTEGER;
	}
	else if ( parTypeName == "R" )
	{
		parType = RDOPT_REAL;
	}
	else if ( parTypeName == "B" )
	{
		parType = RDOPT_BOOL;
	}
	else if ( parTypeName == "A" )
	{
		parType = RDOPT_ARRAY;
	}
	else if ( parTypeName == "S" )
	{
		parType = RDOPT_STRING;
	}
	ASSERT(parType.is_initialized());

	RDOTracerResParamInfo* param = new RDOTracerResParamInfo( parType.get() );
	if ( parType == RDOPT_ENUMERATIVE )
	{
		ruint enum_count;
		stream >> enum_count;
		for ( ruint j = 0; j < enum_count; j++ )
		{
			tstring en_id;
			stream >> en_id;

			tstring en_name;
			stream >> en_name;
			param->addEnumValue( en_name );
		}
	}
	else if ( parType == RDOPT_BOOL )
	{
		param->addEnumValue( _T("false") );
		param->addEnumValue( _T("true" ) );
	}
	else if ( parType == RDOPT_ARRAY )
	{
		RDOTracerResParamInfo* arrayItem = getParamType(stream);
		UNUSED(arrayItem);
	}
	return param;
}

RDOTracerResParamInfo* RDOTracerBase::getParam( rdo::textstream& stream )
{
	tstring par_type;
	tstring par_name;
	stream >> par_type;
	stream >> par_name;
	RDOTracerResParamInfo* param = getParamType(stream);
	param->Name = par_name;
	return param;
}

void RDOTracerBase::addResourceType( REF(tstring) s, rdo::textstream& stream )
{
	UNUSED(s);

	RDOTracerResType* type = new RDOTracerResType( RDOTK_PERMANENT );
	stream >> type->Name;
	int paramcount;
	stream >> paramcount;
	for ( int i = 0; i < paramcount; i++ )
	{
		type->addParamInfo( getParam(stream) );
	}
	resTypes.push_back( type );
	tree->addResourceType( type );
}

void RDOTracerBase::addResource( REF(tstring) s, rdo::textstream& stream )
{
	int rtp;
	tstring res_name;
	stream >> res_name;
	stream >> rtp;
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( rtp - 1 ), res_name );
	res->id = atoi( s.c_str() );

	/*int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string rtpstr = s.substr( endpos );
	trim( rtpstr );
	string name = s.substr( pos, endpos - pos );
	trim( name );
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( rtpstr.c_str() ) - 1 ), name );
	rtpstr = getNextValue( s );
	res->id = atoi( rtpstr.c_str() );*/
	
	resources.push_back( res );
	tree->addResource( res );
}

void RDOTracerBase::addPattern( REF(tstring) s, rdo::textstream& stream )
{
	UNUSED(s);

	tstring pat_name;
	stream >> pat_name;
	tstring pat_type;
	stream >> pat_type;
	RDOTracerPatternKind kind;
	if ( pat_type == "A" )
		kind = RDOPK_OPERATION;
	else if ( pat_type == "I" )
		kind = RDOPK_IRREGULAREVENT;
	else if ( pat_type == "R" )
		kind = RDOPK_RULE;
	else if ( pat_type == "K" )
		kind = RDOPK_KEYBOARD;
	else kind = RDOPK_UNDEFINED;

	RDOTracerPattern* pat = new RDOTracerPattern( kind );
	pat->Name = pat_name;

	/*string str = s;
	string part = getNextValue( str );
	string name = getNextValue( str );
	part = getNextValue( str );
	RDOTracerPatternKind kind;
	if ( part == "A" )
		kind = RDOPK_OPERATION;
	else if ( part == "I" )
		kind = RDOPK_IRREGULAREVENT;
	else if ( part == "R" )
		kind = RDOPK_RULE;
	else if ( part == "K" )
		kind = RDOPK_KEYBOARD;
	RDOTracerPattern* pat = new RDOTracerPattern( kind );
	pat->Name = name;*/

	patterns.push_back( pat );
	tree->addPattern( pat );
	int rel_res_count;
	tstring dummy;
	stream >> rel_res_count;
	for( int i = 0; i < rel_res_count; i++ )
		stream >> dummy;
}

void RDOTracerBase::addOperation( REF(tstring) s, rdo::textstream& stream )
{
	UNUSED(s);

	tstring opr_name;
	stream >> opr_name;
	int pat_id;
	stream >> pat_id;
	RDOTracerPattern* pattern = patterns.at( pat_id - 1 );
	
	RDOTracerOperationBase* opr = NULL;
	
	if ( pattern->getPatternKind() != RDOPK_RULE && pattern->getPatternKind() != RDOPK_IRREGULAREVENT )
		opr = new RDOTracerOperation( pattern );
	else
		opr = new RDOTracerEvent( pattern );
	
	opr->setName( opr_name );

	if ( pattern->getPatternKind() != RDOPK_IRREGULAREVENT ) {
		operations.push_back( opr );
	} else {
		irregularEvents.push_back( static_cast<RDOTracerEvent*>(opr) );
	}
	
	tree->addOperation( opr );

	/*int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string patstr = s.substr( endpos );
	trim( patstr );
	RDOTracerPattern* pattern = patterns.at( atoi( patstr.c_str() ) -1 );
	
	RDOTracerOperationBase* opr = NULL;

	if ( pattern->getPatternKind() != RDOPK_RULE )
		opr = new RDOTracerOperation( pattern );
	else
		opr = new RDOTracerEvent( pattern );

	patstr = s.substr( pos, endpos - pos );
	trim( patstr );
	opr->setName( patstr );
	
	operations.push_back( opr );
	tree->addOperation( opr );*/
}

/*void RDOTracerBase::addIrregularEvent( REF(tstring) s, rdo::textstream& stream )
{
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string patstr = s.substr( endpos );
	trim( patstr );
	RDOTracerEvent* event = new RDOTracerEvent( patterns.at( atoi( patstr.c_str() ) -1 ) );

	patstr = s.substr( pos, endpos - pos );
	trim( patstr );
	event->setName( patstr );
	
	irregularEvents.push_back( event );
	tree->addOperation( event );
}*/

void RDOTracerBase::addResult( REF(tstring) s, rdo::textstream& stream )
{
	int resid;
	stream >> resid;
	tstring res_kind;
	stream >> res_kind;
	RDOTracerResultKind resKind;
	if ( res_kind == "watch_par" )
		resKind = RDORK_WATCHPAR;
	else if ( res_kind == "watch_state" )
		resKind = RDORK_WATCHSTATE;
	else if ( res_kind == "watch_quant" )
		resKind = RDORK_WATCHQUANT;
	else if ( res_kind == "watch_value" )
		resKind = RDORK_WATCHVALUE;
	else resKind = RDORK_UNDEFINED;
	ASSERT(resKind != RDORK_UNDEFINED);

	RDOTracerResult* res = new RDOTracerResult( resKind );
	res->setName( s );
	res->id = resid;
	/*string str = s;
	string name = getNextValue( str );
	string part = getNextValue( str );
	int resid = atoi( part.c_str() );
	part = getNextValue( str );
	RDOTracerResultKind resKind;
	if ( part == "watch_par" )
		resKind = RDORK_WATCHPAR;
	else if ( part == "watch_state" )
		resKind = RDORK_WATCHSTATE;
	else if ( part == "watch_quant" )
		resKind = RDORK_WATCHQUANT;
	else if ( part == "watch_value" )
		resKind = RDORK_WATCHVALUE;
	RDOTracerResult* res = new RDOTracerResult( resKind );
	res->setName( name );
	res->id = resid;*/
	results.push_back( res );
	tree->addResult( res );
}

void RDOTracerBase::dispatchNextString( REF(tstring) line )
{
	if ( line.empty() )
		return;

	tstring key = getNextValue( line );
	RDOTracerTimeNow* timeNow;
	if ( key != "SO" && key.find( "ST" ) == tstring::npos && key != "SD" && key.find( "SE" ) == tstring::npos )
		timeNow = addTime( getNextValue( line ) );
	else
		timeNow = timeList.back();

	if ( key == "ES" ) {
	} else if ( key == "EB" ) {
		startAction( line, timeNow );
	} else if ( key == "EF" ) {
		accomplishAction( line, timeNow );
	} else if ( key == "EI" ) {
		irregularEvent( line, timeNow );
	} else if ( key == "ER" ) {
		productionRule( line, timeNow );
	} else if ( key == "RC" || key == "SRC" ) {
		resource = resourceCreation( line, timeNow );
		action = RUA_ADD;
#ifdef RDOSIM_COMPATIBLE
	} else if ( key == "RE" || key == "SRE" ) {
		tstring copy1 = line;
		resource = resourceElimination( line, timeNow );
		if ( !resource ) {
			tstring copy2 = copy1;
			resource = resourceCreation( copy1, timeNow );
			tree->addResource( resource );
			resource = resourceElimination( copy2, timeNow );
		}
		action = RUA_UPDATE;
	} else if ( key == "RK" || key == "SRK" ) {
		tstring copy = line;
		RDOTracerResource* res = resourceChanging( line, timeNow );
		if ( !res ) {
			resource = resourceCreation( copy, timeNow );
			action = RUA_ADD;
		}
#else
	} else if ( key == "RK" || key == "SRK" || key == "RE" || key == "SRE" ) {
		rbool re = key == "RE" || key == "SRE";
		tstring copy1 = line;
		RDOTracerResource* res = resourceChanging( line, timeNow );
		if ( !res ) {
			tstring copy2 = copy1;
			resource = resourceCreation( copy2, timeNow );
			tree->addResource( resource );
		}
		if ( re ) {
			resource = resourceElimination( copy1, timeNow );
			action = RUA_UPDATE;
		}
#endif
	} else if ( key == "V" ) {
		resultChanging( line, timeNow );
	}/* else if ( key == "$Status" ) {
	} else if ( key.Find( "DPS", 0) != -1 ) {
	} else if ( key == "SB" ) {
	} else if ( key == "SO" ) {
	} else if ( key == "STN" ) {
	} else if ( key == "STD" ) {
	} else if ( key == "STR" ) {
	} else if ( key == "SD" ) {
	} else if ( key == "SES" ) {
	} else if ( key == "SEN" ) {
	} else if ( key == "SEM" ) {
	} else if ( key == "SEF" ) {
	} else if ( key == "SEU" ) {
	}*/
}

tstring RDOTracerBase::getNextValue( REF(tstring) line )
{
	int posstart = line.find_first_not_of( ' ' );
	int posend;
	tstring res;
	if (line[posstart] == _T('['))
	{
		//! @todo Массив просто игнорируется, график по нему не строится. Заплатка.
		posend = line.find_first_of( ']', posstart );
		res = _T("0");
	}
	else
	{
		posend = line.find_first_of( ' ', posstart );
		res = line.substr( posstart, posend - posstart );
	}
	line.erase( 0, posend + 1 );
	rdo::trim( res );
	return res;
}

RDOTracerTimeNow* RDOTracerBase::addTime( CREF(tstring) time )
{
	double val   = atof( time.c_str() );
	rbool  empty = timeList.empty();
	RDOTracerTimeNow* last = NULL;
	if ( !empty ) last = timeList.back();
	if ( empty || last->time != val ) {
		RDOTracerTimeNow* timeNow = new RDOTracerTimeNow( val );
		timeList.push_back( timeNow );
		eventIndex = 0;
		for ( std::vector <RDOTracerOperationBase*>::iterator it = operations.begin(); it != operations.end(); it++ ) {
			(*it)->monitorTime( timeNow, eventIndex );
		}
		for ( std::vector <RDOTracerEvent*>::iterator it_ie = irregularEvents.begin(); it_ie != irregularEvents.end(); it_ie++ ) {
			(*it_ie)->monitorTime( timeNow, eventIndex );
		}
	} else {
		last->eventCount ++;
		eventIndex ++;
		for ( std::vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
			(*it)->lock();
			(*it)->incTimeEventsCount( last );
			(*it)->unlock();
		}
	}
	return timeList.back();
}

RDOTracerOperationBase* RDOTracerBase::getOperation( REF(tstring) line )
{
	getNextValue( line );
	return operations.at( atoi( getNextValue( line ).c_str() ) - 1 );
}

void RDOTracerBase::startAction( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	static_cast<RDOTracerOperation*>(getOperation( line ))->start( time, eventIndex );
}

void RDOTracerBase::accomplishAction( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	static_cast<RDOTracerOperation*>(getOperation( line ))->accomplish( time, eventIndex );
}

void RDOTracerBase::irregularEvent( REF(tstring) line, RDOTracerTimeNow* const time  )
{
#ifdef RDOSIM_COMPATIBLE
	irregularEvents.at( atoi( getNextValue( line ).c_str() ) - 1 )->occurs( time, eventIndex );
#else
	irregularEvents.at( atoi( getNextValue( line ).c_str() ) - 1 )->occurs( time, eventIndex );
#endif
}

void RDOTracerBase::productionRule( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	static_cast<RDOTracerEvent*>(getOperation( line ))->occurs( time, eventIndex );
}

RDOTracerResource* RDOTracerBase::getResource( REF(tstring) line )
{
	getNextValue( line );
	RDOTracerResource* res = NULL;
	int findid = atoi( getNextValue( line ).c_str() );
	int i = 0;
	for ( std::vector< RDOTracerResource* >::iterator it = resources.begin(); it != resources.end(); it++ ) {
		if ( (*it)->id == findid )
		if ( (*it)->id == findid && !(*it)->isErased() ) {
			res = *it;
			break;
		}
		i++;
	}
	return res;
}

RDOTracerResource* RDOTracerBase::resourceCreation( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	ruint typeID = atoi( getNextValue( line ).c_str() ) - 1;
	ASSERT(typeID < resTypes.size());
	RDOTracerResType* type = resTypes.at( typeID );
	int id = atoi( getNextValue( line ).c_str() );
	RDOTracerResource* res = new RDOTracerResource( type, rdo::format( "%s #%d", type->Name.c_str(), id ) );
	res->id = id;
	res->setParams( line, time, eventIndex );

	resources.push_back( res );
	//tree->addResource( res );
	return res;
}

RDOTracerResource* RDOTracerBase::resourceElimination( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	if ( !res ) return NULL;
#ifdef RDOSIM_COMPATIBLE
	res->setParams( line, time, eventIndex, true );
#else
	res->setParams( line, time, eventIndex, false );
#endif
	res->setErased( true );
	//tree->updateResource( res );
	return res;
}

RDOTracerResource* RDOTracerBase::resourceChanging( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	if ( res ) {
		res->setParams( line, time, eventIndex );
	}
	return res;
}

RDOTracerResult* RDOTracerBase::getResult( REF(tstring) line )
{
	RDOTracerResult* res = NULL;
	int findid = atoi( getNextValue( line ).c_str() );
	for ( std::vector< RDOTracerResult* >::iterator it = results.begin(); it != results.end(); it++ ) {
		if ( (*it)->id == findid ) {
			res = *it;
			break;
		}
	}
	return res;
}

void RDOTracerBase::resultChanging( REF(tstring) line, RDOTracerTimeNow* const time  )
{
	getResult( line )->setValue( line, time, eventIndex );
}

void RDOTracerBase::deleteTrace()
{
	mutex.Lock();
	
	int count = resources.size();
	int i;
	for ( i = 0; i < count; i++ ) {
		delete resources.at( i );
	}
	resources.clear();
	count = resTypes.size();
	for ( i = 0; i < count; i++ ) {
		delete resTypes.at( i );
	}
	resTypes.clear();
	count = operations.size();
	for ( i = 0; i < count; i++ ) {
		delete operations.at( i );
	}
	operations.clear();
	count = irregularEvents.size();
	for ( i = 0; i < count; i++ ) {
		delete irregularEvents.at( i );
	}
	irregularEvents.clear();
	count = patterns.size();
	for ( i = 0; i < count; i++ ) {
		delete patterns.at( i );
	}
	patterns.clear();
	count = results.size();
	for ( i = 0; i < count; i++ ) {
		delete results.at( i );
	}
	results.clear();
	std::list< RDOTracerTimeNow* >::iterator it = timeList.begin();
	while ( it != timeList.end() ) {
		delete *it;
		it++;
	}
	timeList.clear();

	mutex.Unlock();
}

void RDOTracerBase::clear()
{
	mutex.Lock();

	clearCharts();
	deleteTrace();
	if( tree )
		tree->clear();
	if ( log )
		log->view().clear();

	mutex.Unlock();
}

void RDOTracerBase::clearCharts()
{
	mutex.Lock();

	while (!charts.empty())
	{
		charts.front()->getFirstView()->getQtParent()->parentWidget()->close();
	}

	mutex.Unlock();
}

void RDOTracerBase::setLog(PTR(rdoTracerLog::RDOTracerLogCtrl) pTracerLog)
{
	log = pTracerLog;
}

void RDOTracerBase::setTree(PTR(ChartTree) pTreeCtrl)
{
	tree = pTreeCtrl;
}

void RDOTracerBase::getModelStructure( rdo::textstream& stream )
{
	mutex.Lock();

	tstring s;

	while( !stream.eof() ) {
		stream >> s;
		if ( !s.empty() ) {
			tstring::size_type pos = tstring::npos;
			if ( s.find( "$Resource_type" ) != tstring::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Resources" );

					// AB if model has no $Resources at all,
					// we get endless cycle here
					if(pos == tstring::npos)
						pos = s.find( "$Pattern" );

					if ( !s.empty() && pos == tstring::npos )
						addResourceType( s, stream );
				} while ( pos == tstring::npos && !stream.eof() );
			}
			if ( s.find( "$Resources" ) != tstring::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Pattern" );
					if ( !s.empty() && pos == tstring::npos )
						addResource( s, stream );
				} while ( pos == tstring::npos && !stream.eof() );
			}
			if ( s.find( "$Pattern" ) != tstring::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Activities" );
					if ( !s.empty() && pos == tstring::npos )
						addPattern( s, stream );
				} while ( pos == tstring::npos && !stream.eof() );
			}
			if ( s.find( "$Activities" ) != tstring::npos ) {
				do {
					stream >> s;
					pos = s.find( "$Watching" );
					if ( !s.empty() && pos == tstring::npos )
						addOperation( s, stream );
				} while ( pos == tstring::npos && !stream.eof() );
			}
			if ( s.find( "$Watching" ) != tstring::npos && !stream.eof() ) {
				do {
					s = "";
					stream >> s;
					if ( !s.empty() )
						addResult( s, stream );
				} while ( !stream.eof() );
			}
		}
	}

	mutex.Unlock();

	/*stream >> s;

	if ( s.find( "$Resource_type" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addResourceType( s, stream );
		} while ( s != "" );
	}
	
	if ( s.find( "$Resources" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addResource( s, stream );
		} while ( s != "" );
	}

	if ( s.find( "$Pattern" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addPattern( s, stream );
		} while ( s != "" );
	}

	if ( s.find( "$Activities" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addOperation( s, stream );
		} while ( s != "" );
		do {
			stream >> s;
			if ( !s.empty() )
				addIrregularEvent( s, stream );
		} while ( s != "" );
	}

	if ( s.find( "$Watching" ) != string::npos ) {
		do {
			stream >> s;
			if ( !s.empty() )
				addResult( s, stream );
		} while ( s != "" );
	}*/
}

void RDOTracerBase::getTraceString( tstring trace_string )
{
	mutex.Lock();

	if ( log ) {
		log->view().addStringToLog( trace_string );
	}

	action = RUA_NONE;
	resource = NULL;

	dispatchNextString( trace_string );

	mutex.Unlock();

	switch( action ) {
		case RUA_ADD: {
			tree->addResource( resource );
			break;
		}
		case RUA_UPDATE: {
			tree->updateResource( resource );
			break;
		}
		default: {
			break;
		}
	}
}

RDOStudioChartDoc* RDOTracerBase::createNewChart()
{
	RDOStudioChartDoc*    pDoc  = new RDOStudioChartDoc   ();
	RDOStudioChartViewQt* pView = new RDOStudioChartViewQt(pDoc, false);
	studioApp.getIMainWnd()->addSubWindow(pView);
	pDoc->attachView(pView->getContext());
	pDoc->autoTitle();

	pView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/mdi_chart.png")));

	return pDoc;
}

RDOStudioChartDoc* RDOTracerBase::addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart )
{
	mutex.Lock();

	if ( !chart )
		chart = createNewChart();
	if( chart ) {
		chart->addSerie( serie );
	}
	
	mutex.Unlock();

	return chart;
}

void RDOTracerBase::addChart( RDOStudioChartDoc* const chart )
{
	mutex.Lock();
	
	charts.push_back( chart );

	mutex.Unlock();
}

void RDOTracerBase::removeChart( RDOStudioChartDoc* chart )
{
	mutex.Lock();
	
	for ( std::vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
		if ( (*it) == chart ) {
			charts.erase( it );
			break;
		}
	}

	mutex.Unlock();
}

void RDOTracerBase::updateChartsStyles() const
{
	const_cast<CMutex&>(mutex).Lock();

	BOOST_FOREACH(RDOStudioChartDoc* pDoc, charts)
	{
		pDoc->setStyle(&studioApp.getStyle()->style_chart);
	}

	const_cast<CMutex&>(mutex).Unlock();
}

void RDOTracerBase::setModelName( tstring name ) const
{
	if ( tree ) tree->setModelName( name );
}

void RDOTracerBase::setDrawTrace( const rbool value )
{
	if ( drawTrace != value ) {
		drawTrace = value;
		if ( !drawTrace )
			clearCharts();
		log->view().setDrawLog( value );
	}
}