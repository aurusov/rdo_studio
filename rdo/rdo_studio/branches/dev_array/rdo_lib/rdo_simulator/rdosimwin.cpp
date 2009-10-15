#pragma warning(disable : 4786)

#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <sstream>
#include <algorithm>

#include "rdosimwin.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <rdotrace.h>
#include <rdo_runtime.h>
#include <rdoframe.h>
#include <rdoparser.h>
#include <rdosmr.h>
#include <rdofrm.h>
#include <rdortp.h>
#include <rdo_resources.h>
#include <rdodebug.h>

//#ifndef DISABLE_CORBA

//#define CORBA_ENABLE

#ifdef CORBA_ENABLE

#include <omniORB4/CORBA.h>

namespace rdoCorba
{
CORBA::ORB_var g_orb;

class RDOCorba_i: public POA_rdoParse::RDOCorba
{
public:
	inline RDOCorba_i() {}
	virtual ~RDOCorba_i() {}
    
	virtual rdoParse::RDOCorba::GetRTP* getRDORTPlist(::CORBA::Long& rtp_count);
	virtual rdoParse::RDOCorba::GetRSS* getRDORSSPlist(::CORBA::Long& rss_count);

	static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref);

};

rdoParse::RDOCorba::GetRTP* RDOCorba_i::getRDORTPlist(::CORBA::Long& rtp_count)
{
	//Создаем список структур для хранения информации об искомых типах ресурсов
	rdoParse::RDOCorba::GetRTP_var my_rtpList = new rdoParse::RDOCorba::GetRTP;

	//Получаем необходимые нам данные о типах ресурсов РДО
	kernel->sendMessage( kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RTP, &my_rtpList );
	
	return my_rtpList._retn();
}

rdoParse::RDOCorba::GetRSS* RDOCorba_i::getRDORSSPlist(::CORBA::Long& rss_count)
{
	//Создаем список структур для хранения информации об искомых ресурсах
	rdoParse::RDOCorba::GetRSS_var my_rssList = new rdoParse::RDOCorba::GetRSS;
	
	//Получаем необходимые нам данные о ресурсах РДО
	kernel->sendMessage( kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RSS, &my_rssList );
	
	return my_rssList._retn();
}

CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref, const char* ModelName)
{
	CosNaming::NamingContext_var rootContext;
	
	try {
	
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references("NameService");
		
		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);
		if( CORBA::is_nil(rootContext) ) {
			TRACE( "Failed to narrow the root naming context." );
			return 0;
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		TRACE( "Caught NO_RESOURCES exception. You must configure omniORB with the location of the naming service.");
		return 0;
	}
	catch (CORBA::ORB::InvalidName&) {
		// This should not happen!
		TRACE( "Service required is invalid [does not exist].");
		return 0;
	}

	try {
		// Bind a context called "test" to the root context:
		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id = (const char*) "RDO"; // string copied
		contextName[0].kind = (const char*) "RDO_context"; // string copied
	
		// Note on kind: The kind field is used to indicate the type
		// of the object. This is to avoid conventions such as that used
		// by files (name.type -- e.g. test.ps = postscript etc.)
		CosNaming::NamingContext_var testContext;
		
		try {
			// Bind the context to root.
			testContext = rootContext->bind_new_context(contextName);
		}
		catch(CosNaming::NamingContext::AlreadyBound&) {
			// If the context already exists, this exception will be raised.
			// In this case, just resolve the name and assign testContext
			// to the object returned:
			CORBA::Object_var obj;
			obj = rootContext->resolve(contextName);
			
			testContext = CosNaming::NamingContext::_narrow(obj);
			
			if( CORBA::is_nil(testContext) ) {
				TRACE( "Failed to narrow naming context.");
			return 0;
			}
		}

		// Bind objref with name Echo to the testContext:
		CosNaming::Name objectName;
		objectName.length(1);
		
		//rdoParse::RDOParserSMRInfo parser;
		//parser.parse();

		objectName[0].id = ModelName;
		objectName[0].kind = (const char*) "Object";

		try {
			testContext->bind(objectName, objref);
		}
		catch(CosNaming::NamingContext::AlreadyBound&) {
			testContext->rebind(objectName, objref);
		}
		
		// Note: Using rebind() will overwrite any Object previously bound
		// to /test/Echo with obj.
		// Alternatively, bind() can be used, which will raise a
		// CosNaming::NamingContext::AlreadyBound exception if the name
		// supplied is already bound to an object.
		// Amendment: When using OrbixNames, it is necessary to first try bind
		// and then rebind, as rebind on it’s own will throw a NotFoundexception if
		// the Name has not already been bound. [This is incorrect behaviour -
		// it should just bind].
	}
	catch(CORBA::TRANSIENT&) {
		TRACE( "Caught system exception TRANSIENT -- unable to contact the naming service.");
		TRACE( "Make sure the naming server is running and that omniORB is configured correctly.");
		return 0;
	}
	catch(CORBA::SystemException& ex) {
		TRACE1( "Caught a CORBA:: %s while using the naming service.", ex._name() );
		return 0;
	}
	
	return 1;
}


unsigned int RDOThreadCorba::corbaRunThreadFun( void* param )
{
	try {
		int argc = 0;
		g_orb = CORBA::ORB_init(argc, NULL);
		
		CORBA::Object_var obj = g_orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
		
		RDOCorba_i* myrdocorba = new RDOCorba_i();

		PortableServer::ObjectId_var myrdocorbaid = poa->activate_object(myrdocorba);

		// Obtain a reference to the object, and register it in
		// the naming service.
		obj = myrdocorba->_this();

		const char* ModelName = "ЦЕХ";
		//const char* ModelName = "СКЛАД";

		if( !bindObjectToName(g_orb, obj, ModelName) )
		{
			delete myrdocorba;
			return 1;
		}

		myrdocorba->_remove_ref();
		
		PortableServer::POAManager_var pman = poa->the_POAManager();
		
		pman->activate();
		
		g_orb->run();
	}
	catch(CORBA::SystemException& ex) {
		TRACE1( "Caught CORBA::%s", ex._name());
	}
	catch(CORBA::Exception&) {
		TRACE( "Caught CORBA::Exception: " );
	}	
	catch(omniORB::fatalException& fe) {
		TRACE3( "Caught omniORB::fatalException: file: %s, line: %d, mesg: %s ", fe.file(), fe.line(), fe.errmsg());
	}
	
	return 0;
}


RDOThreadCorba::RDOThreadCorba():
	RDOThreadMT( "RDOThreadCorba" ),
	thread_corbaRunThreadFun( NULL )
{
	// Место для регистации сообщений корбе
	after_constructor();
}

void RDOThreadCorba::proc( RDOMessageInfo& msg )
{
	// Место для обработки сообщений корбе
}

void RDOThreadCorba::start()
{
#ifdef TR_TRACE
	trace( thread_name + " corba begin" );
#endif

	// Место для запуска корбы
	thread_corbaRunThreadFun = AfxBeginThread( corbaRunThreadFun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
	thread_corbaRunThreadFun->m_bAutoDelete = false;
	thread_corbaRunThreadFun->ResumeThread();

#ifdef TR_TRACE
	trace( thread_name + " corba end" );
#endif
}

void RDOThreadCorba::idle()
{
#ifdef RDO_MT
	if ( broadcast_waiting || !was_start || was_close ) {
		RDOThread::idle();
		return;
	}
#endif
	// Вот тут основная работа корбы
}

void RDOThreadCorba::stop()
{
#ifdef TR_TRACE
	trace( thread_name + " stop begin" );
#endif
	// Место для остановки корбы
	if ( g_orb != CORBA::ORB::_nil() )
	{
		g_orb->shutdown( true );
		g_orb->destroy();
	}

	if ( thread_corbaRunThreadFun )
	{
		DWORD res;
		while ( true )
		{
			if ( ::GetExitCodeThread( thread_corbaRunThreadFun->m_hThread, &res ) )
			{
				if 	( res == STILL_ACTIVE )
				{
					::Sleep(1);
				}
				else
				{
					break;
				}
			}
			else
			{
				int i = 0;
			}
		}
//		thread_corbaRunThreadFun->Delete();
		delete thread_corbaRunThreadFun;
		thread_corbaRunThreadFun = NULL;
	}

	RDOThread::stop();
#ifdef TR_TRACE
	trace( thread_name + " stop end" );
#endif
}

} // namespace rdoCorba

#endif // CORBA_ENABLE



namespace rdoSimulator
{
// --------------------------------------------------------------------
// ---------- RDORuntimeTracer
// --------------------------------------------------------------------
class RDORuntimeTracer: public rdoRuntime::RDOTrace, public rdoRuntime::RDOEndL
{
public:
	virtual std::ostream&        getOStream()    { return m_stream; }
	virtual rdoRuntime::RDOEndL& getEOL()        { return *this;    }

	void onEndl() {
		const std::string& trace_str = m_stream.str();
		if ( trace_str.empty()        ) return;
		if ( !m_simulator->m_canTrace ) return;
		std::string::size_type pos = 0;
		while ( true )
		{
			std::string::size_type next = trace_str.find( '\n', pos );
			std::string str = trace_str.substr( pos, next-pos );
			m_simulator->thread_runtime->broadcastMessage( RDOThread::RT_RUNTIME_TRACE_STRING, &str, true );
			if ( next == std::string::npos ) break;
			pos = next + 1;
			if ( pos >= trace_str.length() ) break;
		}
		m_stream.str("");
	}
	RDORuntimeTracer( RDOThreadSimulator* simulator ):
		m_simulator( simulator )
	{
		m_isNullTracer = false;
	}

private:
	RDOThreadSimulator* m_simulator;
	rdo::textstream   m_stream;
};

// --------------------------------------------------------------------
// ---------- RDOSimResulter
// --------------------------------------------------------------------
class RDOSimResulter: public rdoRuntime::RDOResults
{
private:
	std::ostream& stream;
	virtual std::ostream& getOStream() { return stream; }

public:
	RDOSimResulter( std::ostream& _stream ):
		stream( _stream )
	{
		isNullResult = false;
	}
};

} // namespace rdoSimulator

namespace rdoRuntime {
// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
RDOThreadRunTime::RDOThreadRunTime():
	RDOThreadMT( "RDOThreadRunTime" ),
	simulator( NULL ),
	runtime_error( false )
{
	::GetSystemTime( &time_start );

	simulator = kernel->simulator();

	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY );
	notifies.push_back( RT_RUNTIME_GET_MODE );
	notifies.push_back( RT_RUNTIME_SET_MODE );
	notifies.push_back( RT_RUNTIME_GET_SPEED );
	notifies.push_back( RT_RUNTIME_SET_SPEED );
	notifies.push_back( RT_RUNTIME_GET_SHOWRATE );
	notifies.push_back( RT_RUNTIME_SET_SHOWRATE );
	notifies.push_back( RT_RUNTIME_GET_TIMENOW );
	notifies.push_back( RT_RUNTIME_GET_FRAME );
	notifies.push_back( RT_RUNTIME_KEY_DOWN );
	notifies.push_back( RT_RUNTIME_KEY_UP );
	notifies.push_back( RT_RUNTIME_FRAME_AREA_DOWN );
	after_constructor();
}

void RDOThreadRunTime::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_THREAD_CLOSE: {
			broadcastMessage( RT_RUNTIME_MODEL_STOP_BEFORE );
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY: {
			runtime_error = true;
			break;
		}
		case RT_RUNTIME_GET_MODE: {
			*static_cast<rdoRuntime::RunTimeMode*>(msg.param) = simulator->runtime->getMode();
			break;
		}
		case RT_RUNTIME_SET_MODE: {
			msg.lock();
			simulator->runtime->setMode( *static_cast<rdoRuntime::RunTimeMode*>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_SPEED: {
			*static_cast<double*>(msg.param) = simulator->runtime->getSpeed();
			break;
		}
		case RT_RUNTIME_SET_SPEED: {
			msg.lock();
			simulator->runtime->setSpeed( *static_cast<double*>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_SHOWRATE: {
			*static_cast<double*>(msg.param) = simulator->runtime->getShowRate();
			break;
		}
		case RT_RUNTIME_SET_SHOWRATE: {
			msg.lock();
			simulator->runtime->setShowRate( *static_cast<double*>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_TIMENOW: {
			*static_cast<double*>(msg.param) = simulator->runtime->getTimeNow();
			break;
		}
		case RT_RUNTIME_GET_FRAME: {
			msg.lock();
			GetFrame* getframe = static_cast<GetFrame*>(msg.param);
			simulator->runtime->allFrames.at(getframe->frame_number)->prepareFrame( getframe->frame, simulator->runtime );
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_LAST_BREAKPOINT: {
			msg.lock();
			*static_cast<std::string*>(msg.param) = simulator->runtime->getLastBreakPointName();
			msg.unlock();
			break;
		}
		case RT_RUNTIME_KEY_DOWN: {
			msg.lock();
			if ( std::find( simulator->runtime->using_scan_codes.begin(), simulator->runtime->using_scan_codes.end(), *static_cast<unsigned int*>(msg.param) ) != simulator->runtime->using_scan_codes.end() ) {
				if ( !simulator->runtime->keyDown( *static_cast<unsigned int*>(msg.param) ) ) {
					simulator->runtime->setShowRate( simulator->runtime->getShowRate() );
				}
			}
			msg.unlock();
			break;
		}
		case RT_RUNTIME_KEY_UP: {
			msg.lock();
			simulator->runtime->keyUp( *static_cast<unsigned int*>(msg.param) );
			msg.unlock();
			break;
		}
		case RT_RUNTIME_FRAME_AREA_DOWN: {
			msg.lock();
			simulator->runtime->activeAreasMouseClicked.push_back( *static_cast<std::string*>(msg.param) );
			simulator->runtime->setShowRate( simulator->runtime->getShowRate() );
			msg.unlock();
			break;
		}
	}
}

void RDOThreadRunTime::start()
{
#ifdef TR_TRACE
	trace( thread_name + " start begin" );
#endif

	broadcastMessage( RT_RUNTIME_MODEL_START_BEFORE );

	RDOTrace* tracer;
	rdoRuntime::RDOResults* results;
	rdoRuntime::RDOResults* results_info;

	// Creating tracer and results //////////////////////////////////
	if ( !simulator->parser->getSMR()->hasFile( "Trace_file" ) ) {
		tracer = new RDOTrace();
	} else {
		tracer = new rdoSimulator::RDORuntimeTracer( simulator );
	}

	simulator->resultString.str( "" );
	if ( !simulator->parser->getSMR()->hasFile( "Statistic_file" ) ) {
		results      = new rdoRuntime::RDOResults();
	} else {
		results      = new rdoSimulator::RDOSimResulter( simulator->resultString );
	}

	simulator->resultInfoString.str( "" );
	if ( !simulator->parser->getSMR()->hasFile( "Results_file" ) ) {
		results_info = new rdoRuntime::RDOResults();
	} else {
		results_info = new rdoSimulator::RDOSimResulter( simulator->resultInfoString );
	}

	// RDO config initialization
	simulator->runtime->keysDown.clear();
	simulator->runtime->activeAreasMouseClicked.clear();
	simulator->runtime->setStartTime( simulator->parser->getSMR()->getRunStartTime() );
	simulator->runtime->setTraceStartTime( simulator->parser->getSMR()->getTraceStartTime() );
	simulator->runtime->setTraceEndTime( simulator->parser->getSMR()->getTraceEndTime() );

	// Modelling
	simulator->m_canTrace = true;

	try {
		simulator->exitCode = rdoSimulator::EC_OK;
		simulator->runtime->rdoInit( tracer, results, results_info );
		switch ( simulator->parser->getSMR()->getShowMode() ) {
			case rdoSimulator::SM_NoShow   : simulator->runtime->setMode( rdoRuntime::RTM_MaxSpeed ); break;
			case rdoSimulator::SM_Animation: simulator->runtime->setMode( rdoRuntime::RTM_Sync ); break;
			case rdoSimulator::SM_Monitor  : simulator->runtime->setMode( rdoRuntime::RTM_Pause ); break;
		}
		simulator->runtime->setShowRate( simulator->parser->getSMR()->getShowRate() );
	}
	catch ( rdoParse::RDOSyntaxException& ) {
		runtime_error = true;
		simulator->runtime->onRuntimeError();
	}
	catch ( rdoRuntime::RDORuntimeException& ex ) {
		runtime_error = true;
		simulator->runtime->onRuntimeError();
		std::string mess = ex.getType() + " : " + ex.message();
		sendMessage( kernel, RDOThread::RT_DEBUG_STRING, &mess );
	}

	if ( !runtime_error ) {
		broadcastMessage( RT_RUNTIME_MODEL_START_AFTER );
	}

#ifdef TR_TRACE
	trace( thread_name + " start end, runing simulation" );
#endif
}

void RDOThreadRunTime::idle()
{
#ifdef RDO_MT
	if ( broadcast_waiting || !was_start || was_close ) {
		RDOThread::idle();
		return;
	}
#endif
//	TRACE( "R. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	try {
		if ( runtime_error || !simulator->runtime->rdoNext() ) {
			sendMessage( this, RT_THREAD_CLOSE );
		}
	}
	catch ( rdoParse::RDOSyntaxException& ) {
		runtime_error = true;
		simulator->runtime->onRuntimeError();
	}
	catch ( rdoRuntime::RDORuntimeException& ex ) {
		runtime_error = true;
		simulator->runtime->onRuntimeError();
		std::string mess = ex.getType() + " : " + ex.message();
		sendMessage( kernel, RDOThread::RT_DEBUG_STRING, &mess );
	}
//	catch (...) {
//		runtime_error = true;
//		TRACE( "******************************** Ошибка rdoNext()\n" );
//		simulator->runtime->onRuntimeError();
//	}
}

void RDOThreadRunTime::writeResultsInfo()
{
	switch ( simulator->runtime->whyStop ) {
		case rdoSimulator::EC_OK:
			simulator->runtime->getResultsInfo() << "$Status = " << "NORMAL_TERMINATION";
			break;
		case rdoSimulator::EC_NoMoreEvents:
			simulator->runtime->getResultsInfo() << "$Status = " << "NO_MORE_EVENTS";
			break;
		case rdoSimulator::EC_RunTimeError:
			simulator->runtime->getResultsInfo() << "$Status = " << "RUN_TIME_ERROR";
			break;
		case rdoSimulator::EC_UserBreak:
			simulator->runtime->getResultsInfo() << "$Status = " << "USER_BREAK";
			break;
	}
	simulator->runtime->getResultsInfo() << '\n' << "$Result_values  0  " << simulator->runtime->getTimeNow();
	SYSTEMTIME time_stop;
	::GetSystemTime( &time_stop );
	double delay = -1;
	if ( time_start.wYear == time_stop.wYear && time_start.wMonth == time_stop.wMonth ) {
		delay = (time_stop.wDay - time_start.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - time_start.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - time_start.wMinute) * 60 * 1000 + (time_stop.wSecond - time_start.wSecond) * 1000 + (time_stop.wMilliseconds - time_start.wMilliseconds );
	} else if ( time_stop.wYear - time_start.wYear == 1 && time_start.wMonth == 12 && time_stop.wMonth == 1 ) {
		delay = (time_stop.wDay + 31 - time_start.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - time_start.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - time_start.wMinute) * 60 * 1000 + (time_stop.wSecond - time_start.wSecond) * 1000 + (time_stop.wMilliseconds - time_start.wMilliseconds );
	}
	if ( delay != -1 ) {
		simulator->runtime->getResultsInfo() << "  " << delay / 1000.0;
	} else {
		simulator->runtime->getResultsInfo() << "  ?";
	}
	simulator->runtime->getResultsInfo() << '\n' << "  EventCount           " << simulator->runtime->get_cnt_events() << "  " << (double)simulator->runtime->get_cnt_events() / simulator->runtime->getTimeNow() << "  ";
	if ( delay != -1 ) {
		simulator->runtime->getResultsInfo() << (unsigned int)((double)simulator->runtime->get_cnt_events() / delay * 1000);
	} else {
		simulator->runtime->getResultsInfo() << "?";
	}
	simulator->runtime->getResultsInfo() << '\n' << "  OperRuleCheckCounter " << simulator->runtime->get_cnt_choice_from() << "  " << (double)simulator->runtime->get_cnt_choice_from() / simulator->runtime->getTimeNow() << "  ";
	if ( delay != -1 ) {
		simulator->runtime->getResultsInfo() << (unsigned int)((double)simulator->runtime->get_cnt_choice_from() / delay * 1000);
	} else {
		simulator->runtime->getResultsInfo() << "?";
	}
	simulator->runtime->getResultsInfo() << '\n' << "  AExpCalcCounter      " << simulator->runtime->get_cnt_calc_arithm() << "  " << (double)simulator->runtime->get_cnt_calc_arithm() / simulator->runtime->getTimeNow() << "  ";
	if ( delay != -1 ) {
		simulator->runtime->getResultsInfo() << (unsigned int)((double)simulator->runtime->get_cnt_calc_arithm() / delay * 1000);
	} else {
		simulator->runtime->getResultsInfo() << "?";
	}
	simulator->runtime->getResultsInfo() << '\n' << "  BExpCalcCounter      " << simulator->runtime->get_cnt_calc_logic() << "  " << (double)simulator->runtime->get_cnt_calc_logic() / simulator->runtime->getTimeNow() << "  ";
	if ( delay != -1 ) {
		simulator->runtime->getResultsInfo() << (unsigned int)((double)simulator->runtime->get_cnt_calc_logic() / delay * 1000);
	} else {
		simulator->runtime->getResultsInfo() << "?";
	}
}

void RDOThreadRunTime::stop()
{
#ifdef TR_TRACE
	trace( thread_name + " stop begin" );
#endif

	try {
		simulator->runtime->rdoPostProcess();
		writeResultsInfo();
	}
	catch ( rdoParse::RDOSyntaxException& ) {
		runtime_error = true;
		simulator->runtime->onRuntimeError();
	}
	catch ( rdoRuntime::RDORuntimeException& ex ) {
		runtime_error = true;
		simulator->runtime->onRuntimeError();
		std::string mess = ex.getType() + " : " + ex.message();
		sendMessage( kernel, RDOThread::RT_DEBUG_STRING, &mess );
	}

	broadcastMessage( RT_RUNTIME_MODEL_STOP_AFTER );

	RDOThread::stop();

#ifdef TR_TRACE
	trace( thread_name + " stop end" );
#endif
}

} // namespace rdoRuntime

namespace rdoSimulator
{
// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
RDOThreadSimulator::RDOThreadSimulator():
	RDOThreadMT( "RDOThreadSimulator" ),
	runtime( NULL ), 
	parser( NULL ),
	thread_runtime( NULL ),
	exitCode( rdoSimulator::EC_OK )
{
	notifies.push_back( RT_STUDIO_MODEL_BUILD );
	notifies.push_back( RT_STUDIO_MODEL_RUN );
	notifies.push_back( RT_STUDIO_MODEL_STOP );
	notifies.push_back( RT_SIMULATOR_GET_MODEL_STRUCTURE );
	notifies.push_back( RT_SIMULATOR_GET_MODEL_RESULTS );
	notifies.push_back( RT_SIMULATOR_GET_MODEL_RESULTS_INFO );
	notifies.push_back( RT_SIMULATOR_GET_MODEL_EXITCODE );
	notifies.push_back( RT_SIMULATOR_GET_LIST );
	notifies.push_back( RT_SIMULATOR_GET_ERRORS );
	notifies.push_back( RT_THREAD_STOP_AFTER );
	notifies.push_back( RT_CODECOMP_GET_DATA );
	notifies.push_back( RT_CORBA_PARSER_GET_RTP );
	notifies.push_back( RT_CORBA_PARSER_GET_RSS );
	//notifies.push_back( RT_CORBA_PARSER_GET_RTP_COUNT );
	//notifies.push_back( RT_CORBA_PARSER_GET_RTP_PAR_COUNT );
	after_constructor();
}

RDOThreadSimulator::~RDOThreadSimulator()
{
	terminateModel();
	closeModel();
}

void RDOThreadSimulator::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_STUDIO_MODEL_BUILD: {
			parseModel();
			break;
		}
		case RT_STUDIO_MODEL_RUN: {
			runModel();
			break;
		}
		case RT_STUDIO_MODEL_STOP: {
			stopModel();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_STRUCTURE: {
			msg.lock();
			*static_cast<rdo::textstream*>(msg.param) << parser->getModelStructure();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS: {
			msg.lock();
			*static_cast<rdo::textstream*>(msg.param) << resultString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS_INFO: {
			msg.lock();
			*static_cast<rdo::textstream*>(msg.param) << parser->getChanges();
			*static_cast<rdo::textstream*>(msg.param) << std::endl << std::endl;
			*static_cast<rdo::textstream*>(msg.param) << resultInfoString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_EXITCODE: {
			msg.lock();
			*static_cast<rdoSimulator::RDOExitCode*>(msg.param) = exitCode;
			msg.unlock();
			break;
		}
		case RT_CODECOMP_GET_DATA: {
			codeCompletion();
			break;
		}
		
#ifdef CORBA_ENABLE

		case RT_CORBA_PARSER_GET_RTP: {
			msg.lock();
			corbaGetRTP( *static_cast<rdoParse::RDOCorba::GetRTP_var*>(msg.param) );
			msg.unlock();
			break;
		}

		case RT_CORBA_PARSER_GET_RSS: {
			msg.lock();
			corbaGetRSS( *static_cast<rdoParse::RDOCorba::GetRSS_var*>(msg.param) );
			msg.unlock();
			break;
		}
#endif // CORBA_ENABLE

		case RT_SIMULATOR_GET_LIST: {
			msg.lock();
			GetList* getlist = static_cast<GetList*>(msg.param);
			switch ( getlist->type ) {
				case GetList::frames: {
					if ( runtime ) {
						int size = runtime->allFrames.size();
						for ( int i = 0; i < size; i++ ) {
							getlist->list->push_back( runtime->allFrames.at(i)->name() );
						}
					}
					break;
				}
				case GetList::bitmaps: {
					if ( runtime ) {
						int size = runtime->allFrames.size();
						for ( int i = 0; i < size; i++ ) {
							runtime->allFrames.at(i)->getBitmaps( *getlist->list );
						}
					}
					break;
				}
			}
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_ERRORS: {
			std::vector< RDOSyntaxError > errors = getErrors();
			msg.lock();
			std::vector< RDOSyntaxError >* msg_errors = static_cast<std::vector< RDOSyntaxError >*>(msg.param);
			msg_errors->assign( errors.begin(), errors.end() );
			msg.unlock();
			break;
		}
		case RT_THREAD_STOP_AFTER: {
			if ( msg.from == thread_runtime ) {
				// rdoSimulator::EC_ParserError   - Не используется в run-time
				// rdoSimulator::EC_ModelNotFound - Не используется в run-time
				// rdoSimulator::EC_UserBreak     - Устанавливается в simulator, перехват RT_THREAD_STOP_AFTER не срабатывает
				exitCode = runtime->whyStop;
				if ( !thread_runtime->runtime_error ) {
					// Остановились сами нормально
					broadcastMessage( RT_SIMULATOR_MODEL_STOP_OK );
					closeModel();
				} else {
					// Остановились сами, но не нормально
					broadcastMessage( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
					closeModel();
				}
				// Треда удаляется сама, надо удалить её событие
				// Делается это без мутексов, т.к. thread_destroy не должна использоваться в thread_runtime пока обрабатывается RT_THREAD_STOP_AFTER
#ifdef RDO_MT
				delete thread_runtime->thread_destroy;
				thread_runtime->thread_destroy = NULL;
#endif
				thread_runtime = NULL;
			}
			break;
		}
	}
}

bool RDOThreadSimulator::parseModel()
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParserModel();
	runtime = parser->runtime();

	try {
		exitCode = rdoSimulator::EC_OK;
		parser->parse();
	}
	catch ( rdoParse::RDOSyntaxException& /*ex*/ ) {
		exitCode = rdoSimulator::EC_ParserError;
		broadcastMessage( RT_SIMULATOR_PARSE_ERROR );
		closeModel();
		return false;
	}
	catch ( rdoRuntime::RDORuntimeException& ex ) {
		std::string mess = ex.getType() + " : " + ex.message();
		broadcastMessage( RT_SIMULATOR_PARSE_STRING, &mess );
		exitCode = rdoSimulator::EC_ParserError;
		broadcastMessage( RT_SIMULATOR_PARSE_ERROR );
		closeModel();
		return false;
	}

	showMode = getInitialShowMode();
	showRate = getInitialShowRate();

	broadcastMessage( RT_SIMULATOR_PARSE_OK );

	return true;
}

void RDOThreadSimulator::runModel()
{
	if ( parseModel() ) {
		parser->m_errors.clear();
		exitCode = rdoSimulator::EC_OK;
		thread_runtime = new rdoRuntime::RDOThreadRunTime();
	}
}

void RDOThreadSimulator::stopModel()
{
	runtime->onUserBreak();
	exitCode = rdoSimulator::EC_UserBreak;
	terminateModel();
	m_canTrace = false;
	broadcastMessage( RT_SIMULATOR_MODEL_STOP_BY_USER );
	closeModel();
	// UA 19.08.04 // добавил код возврата
//	kernel.callback(RDOKernel::modelExit, rdoSimulator::EC_UserBreak);
}

void RDOThreadSimulator::terminateModel()
{
	if ( thread_runtime ) {
		// Перестали реагировать на остановку run-time-треды, т.к. закрываем её сами
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.erase( std::find(notifies.begin(), notifies.end(), RT_THREAD_STOP_AFTER) );
#ifdef RDO_MT
		notifies_mutex.Unlock();
		CEvent* thread_destroy = thread_runtime->thread_destroy;
#endif

		sendMessage( thread_runtime, RDOThread::RT_THREAD_CLOSE );

#ifdef RDO_MT
		thread_destroy->Lock();
		delete thread_destroy;
#endif
		thread_runtime = NULL;

		// Опять начали реагировать на остановку run-time-треды, чтобы обнаружить нормальное завершение модели (или по run-time-error)
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.push_back( RT_THREAD_STOP_AFTER );
#ifdef RDO_MT
		notifies_mutex.Unlock();
#endif
	}
}

void RDOThreadSimulator::closeModel()
{
/*
	try {
		if ( runtime ) {
			delete runtime;
			runtime = NULL;
		}
	} catch (...) {
		runtime = NULL;
		TRACE( "******************************** Ошибка удаления runtime\n" );
	}
*/
	runtime = NULL;
	try {
		if ( parser ) {
			delete parser;
			parser = NULL;
		}
	} catch (...) {
		parser = NULL;
		TRACE(_T("******************************** Ошибка удаления parser\n"));
	}
}

void RDOThreadSimulator::parseSMRFileInfo(rdo::textstream& smr, rdoModelObjects::RDOSMRFileInfo& info)
{
	rdoParse::RDOParserSMRInfo parser;

	try {
		info.error = false;
		parser.parse( smr );
	}
	catch ( rdoParse::RDOSyntaxException& ) {
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		info.error = true;
	}
	catch ( rdoRuntime::RDORuntimeException& ex ) {
		std::string mess = ex.getType() + " : " + ex.message();
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_STRING, &mess );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		info.error = true;
	}

	if ( !parser.hasSMR() ) {
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY );
		info.error = true;
	} else {
		info.model_name     = parser.getSMR()->getFile( "Model_name" );
		info.resource_file  = parser.getSMR()->getFile( "Resource_file" );
		info.oprIev_file    = parser.getSMR()->getFile( "OprIev_file" );
		info.frame_file     = parser.getSMR()->getFile( "Frame_file" );
		info.statistic_file = parser.getSMR()->getFile( "Statistic_file" );
		info.results_file   = parser.getSMR()->getFile( "Results_file" );
		info.trace_file     = parser.getSMR()->getFile( "Trace_file" );
	}
}

std::vector< RDOSyntaxError > RDOThreadSimulator::getErrors()
{
	std::vector< RDOSyntaxError > res;

	if ( !parser ) return res;

	res = parser->m_errors;
	res.insert( res.end(), runtime->errors.begin(), runtime->errors.end() );
	return res;
}

ShowMode RDOThreadSimulator::getInitialShowMode()
{
	return parser->getSMR()->getShowMode();
}

int RDOThreadSimulator::getInitialFrameNumber()
{
	return parser->getSMR()->getFrameNumber();
}

double RDOThreadSimulator::getInitialShowRate()
{
	return parser->getSMR()->getShowRate();
}

void RDOThreadSimulator::codeCompletion()
{
}

#ifdef CORBA_ENABLE

void RDOThreadSimulator::corbaGetRTP( rdoParse::RDOCorba::GetRTP_var& my_rtpList )
{
	// Пропарсели типы и ресурсы текста модели (текущие, а не записанные)
	rdoParse::RDOParserCorba parser;
	try {
		parser.parse();
	}
	catch ( rdoParse::RDOSyntaxException& ) {
	}
	catch ( rdoRuntime::RDORuntimeException& ) {
	}

	::CORBA::Long i = 0, j = 0;

	// Пробежались по всем типам и переписали в RTPList
	rdoMBuilder::RDOResTypeList rtpList( &parser );
	
	//Считаем количество типов ресурсов
	rdoMBuilder::RDOResTypeList::List::const_iterator rtp_it = rtpList.begin();
	
	::CORBA::Long rtp_count = 0;

	while ( rtp_it != rtpList.end() )
	{
		rtp_count++;
		rtp_it++;
	}

	//Выделяем пямять под последовательность
	my_rtpList->length( rtp_count );

	//Снова возвращаемся в начало списка типов ресурсов
	rtp_it = rtpList.begin();

	while ( rtp_it != rtpList.end() )
	{
		// Создаем текстовую структуру

		my_rtpList[i].m_name = CORBA::string_dup( rtp_it->name().c_str() );
		
		if ((rtp_it->getType()) == rdoMBuilder::RDOResType::rt_permanent )
			my_rtpList[i].m_type=rdoParse::RDOCorba::rt_permanent;
		else
			my_rtpList[i].m_type=rdoParse::RDOCorba::rt_temporary;

		//Считаем количество параметров i-го типа ресурса
		rdoMBuilder::RDOResType::ParamList::List::const_iterator param_it = rtp_it->m_params.begin();
		my_rtpList[i].m_param_count = 0;

		while ( param_it != rtp_it->m_params.end() )
		{
			my_rtpList[i].m_param_count++;
			param_it++;
		}
		
		//Выделяем память под последовательность параметров i-го типа ресурсов
		my_rtpList[i].m_param.length(my_rtpList[i].m_param_count);

		//Снова возвращаемся в начало списка параметров i-го типа ресурсов
		param_it = rtp_it->m_params.begin();

		while ( param_it != rtp_it->m_params.end() )
		{
			// Добавляем в структуру параметр!!!!!!!!!!!!!!!!
			my_rtpList[i].m_param[j].m_name = CORBA::string_dup( param_it->name().c_str() );
			
			my_rtpList[i].m_param[j].m_diap_int = 0;
			my_rtpList[i].m_param[j].m_default_int_ch = 0;
			my_rtpList[i].m_param[j].m_diap_double = 0;
			my_rtpList[i].m_param[j].m_default_double_ch = 0;
			my_rtpList[i].m_param[j].m_var_enum_ch = 0;	
			my_rtpList[i].m_param[j].m_default_enum_ch = 0;
			my_rtpList[i].m_param[j].m_var_enum_count = 0;

			switch (param_it->typeID())
			{
				case rdoRuntime::RDOType::t_int:{
					my_rtpList[i].m_param[j].m_type = rdoParse::RDOCorba::int_type;

					if ( param_it->hasDiap() )
					{
						my_rtpList[i].m_param[j].m_min_int = param_it->getMin().getInt();
						my_rtpList[i].m_param[j].m_max_int = param_it->getMax().getInt();
						my_rtpList[i].m_param[j].m_diap_int = 1;

					}
					if ( param_it->hasDefault() )
					{
						my_rtpList[i].m_param[j].m_default_int = param_it->getDefault().getInt();
						my_rtpList[i].m_param[j].m_default_int_ch = 1;
					}
					break;
				}
				case rdoRuntime::RDOType::t_real:{
					my_rtpList[i].m_param[j].m_type = rdoParse::RDOCorba::double_type;

					if ( param_it->hasDiap() )
					{
						my_rtpList[i].m_param[j].m_min_double = param_it->getMin().getDouble();
						my_rtpList[i].m_param[j].m_max_double = param_it->getMax().getDouble();
						my_rtpList[i].m_param[j].m_diap_double = 1;

					}
					if ( param_it->hasDefault() )
					{
						my_rtpList[i].m_param[j].m_default_double = param_it->getDefault().getDouble();
						my_rtpList[i].m_param[j].m_default_double_ch = 1;
					}
					break;
				}
				case rdoRuntime::RDOType::t_enum:{
					my_rtpList[i].m_param[j].m_type = rdoParse::RDOCorba::enum_type;
					
					//Считаем количество значений перечислимого типа
					rdoRuntime::RDOEnumType::CIterator enum_it = param_it->getEnum().begin();
					
					CORBA::Long k = 0;

					while ( enum_it != param_it->getEnum().end() )
					{
						k++;					
						enum_it++;
					}
					
					//Выделяем память под последовательность значений j-го параметра перечислимого типа i-го типа ресурсов
					my_rtpList[i].m_param[j].m_var_enum.length(k);
					
					enum_it = param_it->getEnum().begin();
					k = 0;

					while ( enum_it != param_it->getEnum().end() )
					{
						my_rtpList[i].m_param[j].m_var_enum[k] = CORBA::string_dup( enum_it->c_str() );
						enum_it++;
						k++;
					}
					
					if ( param_it->hasDefault() )
					{
						my_rtpList[i].m_param[j].m_default_enum = CORBA::string_dup( param_it->getDefault().getAsString().c_str() );
						my_rtpList[i].m_param[j].m_default_enum_ch = 1;
					}

					my_rtpList[i].m_param[j].m_var_enum_ch = 1;
					my_rtpList[i].m_param[j].m_var_enum_count = k;
					break;
				}
				default: break;
			}
		
			j++;
			param_it++;
		}
		j = 0;
		i++;		
		rtp_it++;
	}

}

void RDOThreadSimulator::corbaGetRSS( rdoParse::RDOCorba::GetRSS_var& my_rssList )
{
	// Пропарсели типы и ресурсы текста модели (текущие, а не записанные)
	rdoParse::RDOParserCorba parser;
	try {
		parser.parse();
	}
	catch ( rdoParse::RDOSyntaxException& ) {
	}
	catch ( rdoRuntime::RDORuntimeException& ) {
	}
	// Пробежались по всем ресурсам и переписали в RSSList
	rdoMBuilder::RDOResourceList rssList( &parser );
	rdoMBuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();

	::CORBA::Long i = 0, j = 0;
	::CORBA::Long rss_count = 0;

	//Считаем количество ресурсов
	while ( rss_it != rssList.end() )
	{
		rss_count++;
		rss_it++;
	}

	//Выделяем пямять под последовательность
	my_rssList->length( rss_count );

	//Снова возвращаемся в начало списка типов ресурсов
	rss_it = rssList.begin();

	while ( rss_it != rssList.end() )
	{
		// Заполняем значения структуры
		my_rssList[i].m_name = CORBA::string_dup( rss_it->name().c_str() );
		my_rssList[i].m_type = CORBA::string_dup( rss_it->getType().name().c_str() );
		
		//Считаем количество параметров i-го типа ресурса
		rdoMBuilder::RDOResource::Params::const_iterator param_it = rss_it->begin();
		
		my_rssList[i].m_param_count = 0;

		while ( param_it != rss_it->end() )
		{
			my_rssList[i].m_param_count++;
			param_it++;
		}
		
		//Выделяем память под последовательность параметров i-го ресурса
		my_rssList[i].m_param.length(my_rssList[i].m_param_count);

		//Снова возвращаемся в начало списка параметров i-го ресурса
		param_it = rss_it->begin();

		while ( param_it != rss_it->end() )
		{
			my_rssList[i].m_param[j].m_name = CORBA::string_dup( param_it->first.c_str() );;
		
			switch (param_it->second.typeID())
			{
				case rdoRuntime::RDOType::t_int:{
					
					my_rssList[i].m_param[j].m_int = param_it->second.getInt();
					my_rssList[i].m_param[j].m_type = rdoParse::RDOCorba::int_type;

					break;
				}
				case rdoRuntime::RDOType::t_real:{

					my_rssList[i].m_param[j].m_double = param_it->second.getDouble();
					my_rssList[i].m_param[j].m_type = rdoParse::RDOCorba::double_type;
					
					break;
				}
				case rdoRuntime::RDOType::t_enum:{

					my_rssList[i].m_param[j].m_enum = param_it->second.getAsString().c_str();
					my_rssList[i].m_param[j].m_type = rdoParse::RDOCorba::enum_type;

					break;
				}
				default: break;
			}

			param_it++;
			j++;
		}
	
		j = 0;
		rss_it++;
		i++;
	}


/*
	// Пропарсели типы и ресурсы текста модели (текущие, а не записанные)
	rdoParse::RDOParserCorba parser;
	try {
		parser.parse();
	}
	catch ( rdoParse::RDOSyntaxException& ) {
	}
	catch ( rdoRuntime::RDORuntimeException& ) {
	}
	// Пробежались по всем ресурсам и переписали в RSSList
	rdoMBuilder::RDOResourceList rssList( &parser );
	rdoMBuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();
	/*while ( rss_it != rssList.end() )
	{
		// Создаем текстовую структуру
		RSS rss;
		rss.m_name = rss_it->name();
		// Запоминаем в списке
		RSSList->push_back( rss );
		rss_it++;
	}
*/
		

}
#endif // CORBA_ENABLE

// --------------------------------------------------------------------
// ---------- RDOThreadCodeComp
// --------------------------------------------------------------------
RDOThreadCodeComp::RDOThreadCodeComp():
	RDOThreadMT( "RDOThreadCodeComp" ),
	parser( NULL )
{
//	notifies.push_back( RT_CODECOMP_GET_DATA );
	after_constructor();
}

RDOThreadCodeComp::~RDOThreadCodeComp()
{
}

void RDOThreadCodeComp::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_CODECOMP_GET_DATA: {
//			if ( rdoParse::parser ) parser = rdoParse::parser;
			if ( !parser ) break;
			msg.lock();
			GetCodeComp* data = static_cast<GetCodeComp*>(msg.param);
//			rdo::binarystream stream;
//			sendMessage( kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdoRepository::RDOThreadRepository::FileData( data->file, stream ) );
//			data->result = stream.data();
			const std::vector< rdoParse::RDORTPResType* >& rtp_list = parser->getRTPResTypes(); 
			std::vector< rdoParse::RDORTPResType* >::const_iterator rtp_it = rtp_list.begin();
			while ( rtp_it != rtp_list.end() ) {
				const std::vector< const rdoParse::RDORTPParam* >& param_list = (*rtp_it)->getParams();
				std::vector< const rdoParse::RDORTPParam* >::const_iterator param_it = param_list.begin();
				while ( param_it != param_list.end() ) {
					data->result += (*param_it)->name() + ' ';
					param_it++;
				}
				rtp_it++;
			}
			msg.unlock();
//			if ( parser != rdoParse::parser ) delete parser;
			parser = NULL;
			break;
		}
	}
}

} // namespace rdoSimulator
