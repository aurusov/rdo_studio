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
#include <memcheck.h>
#include <rdoparser.h>
#include <rdosmr.h>
#include <rdofrm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoSimulator
{
// --------------------------------------------------------------------
// ---------- RDORuntimeTracer
// --------------------------------------------------------------------
class RDORuntimeTracer: public rdoRuntime::RDOTrace, public rdoRuntime::RDOEndL
{
private:
	RDOThreadSimulator* simulator;
	std::stringstream stream;

public:
	std::ostream& getOStream() { return stream; }
	rdoRuntime::RDOEndL& getEOL() { return *this; }
	void onEndl() {
		const std::string& trace_str = stream.str();
		if ( trace_str.empty() ) return;
		if ( !simulator->canTrace ) return;
		int pos = 0;
		for (;;) {
			int next = trace_str.find( '\n', pos );
			std::string str = trace_str.substr( pos, next-pos );
			simulator->thread_runtime->broadcastMessage( RDOThread::RT_RUNTIME_TRACE_STRING, &str );
			if ( next == std::string::npos ) break;
			pos = next + 1;
			if ( pos >= trace_str.length() ) break;
		}
		stream.str("");
	}
	RDORuntimeTracer( RDOThreadSimulator* _simulator ):
		simulator( _simulator )
	{
		isNullTracer = false;
	}
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
	simulator->canTrace = true;

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
		std::string mess = ex.getType() + " : " + ex.mess;
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
		std::string mess = ex.getType() + " : " + ex.mess;
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
		std::string mess = ex.getType() + " : " + ex.mess;
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
			*static_cast<std::stringstream*>(msg.param) << parser->getModelStructure().str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS: {
			msg.lock();
			*static_cast<std::stringstream*>(msg.param) << resultString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS_INFO: {
			msg.lock();
			*static_cast<std::stringstream*>(msg.param) << parser->getChanges();
			*static_cast<std::stringstream*>(msg.param) << std::endl << std::endl;
			*static_cast<std::stringstream*>(msg.param) << resultInfoString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_EXITCODE: {
			msg.lock();
			*static_cast<rdoSimulator::RDOExitCode*>(msg.param) = exitCode;
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_LIST: {
			msg.lock();
			GetList* getlist = static_cast<GetList*>(msg.param);
			switch ( getlist->type ) {
				case GetList::frames: {
					if ( runtime ) {
						int size = runtime->allFrames.size();
						for ( int i = 0; i < size; i++ ) {
							getlist->list->push_back( runtime->allFrames.at(i)->getName() );
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

	parser = new rdoParse::RDOParser();
	runtime = parser->runtime;

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
		std::string mess = ex.getType() + " : " + ex.mess;
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
		parser->errors.clear();
		exitCode = rdoSimulator::EC_OK;
		thread_runtime = new rdoRuntime::RDOThreadRunTime();
	}
}

void RDOThreadSimulator::stopModel()
{
	runtime->onUserBreak();
	exitCode = rdoSimulator::EC_UserBreak;
	terminateModel();
	canTrace = false;
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
//	try {
		if ( runtime ) {
			delete runtime;
			runtime = NULL;
		}
//	} catch (...) {
//		runtime = NULL;
//		TRACE( "******************************** Ошибка удаления runtime\n" );
//	}
	try {
		if ( parser ) {
			delete parser;
			parser = NULL;
		}
	} catch (...) {
		parser = NULL;
		TRACE( "******************************** Ошибка удаления parser\n" );
	}
}

void RDOThreadSimulator::parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info )
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runtime;

	try {
		info.error = false;
		parser->parse( rdoModelObjects::obPRE, smr );
	}
	catch ( rdoParse::RDOSyntaxException& ) {
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
//		closeModel();
		info.error = true;
//		return;
	}
	catch ( rdoRuntime::RDORuntimeException& ex ) {
		std::string mess = ex.getType() + " : " + ex.mess;
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_STRING, &mess );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
//		closeModel();
		info.error = true;
//		return;
	}

	if ( !parser->hasSMR() ) {
//		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_STRING, &std::string("В smr-файле не найдено имя модели") );
//		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_STRING, &std::string("SMR File seems to be empty\n") );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY );
//		closeModel();
		info.error = true;
//		return;
	} else {
		info.model_name     = parser->getSMR()->getFile( "Model_name" );
		info.resource_file  = parser->getSMR()->getFile( "Resource_file" );
		info.oprIev_file    = parser->getSMR()->getFile( "OprIev_file" );
		info.frame_file     = parser->getSMR()->getFile( "Frame_file" );
		info.statistic_file = parser->getSMR()->getFile( "Statistic_file" );
		info.results_file   = parser->getSMR()->getFile( "Results_file" );
		info.trace_file     = parser->getSMR()->getFile( "Trace_file" );
	}

	closeModel();
}

std::vector< RDOSyntaxError > RDOThreadSimulator::getErrors()
{
	std::vector< RDOSyntaxError > res;

	if ( !parser ) return res;

	res = parser->errors;
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

// --------------------------------------------------------------------
// ---------- RDOThreadCodeComp
// --------------------------------------------------------------------
RDOThreadCodeComp::RDOThreadCodeComp():
	RDOThreadMT( "RDOThreadCodeComp" ),
	parser( NULL )
{
	notifies.push_back( RT_CODECOMP_GET_DATA );
	after_constructor();
}

RDOThreadCodeComp::~RDOThreadCodeComp()
{
}

void RDOThreadCodeComp::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_CODECOMP_GET_DATA: {
			if ( rdoParse::parser ) parser = rdoParse::parser;
			if ( !parser ) break;
			msg.lock();
			GetCodeComp* data = static_cast<GetCodeComp*>(msg.param);
//			rdo::binarystream stream;
//			sendMessage( kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdoRepository::RDOThreadRepository::FileData( data->file, stream ) );
//			data->result = stream.data();
			const std::vector< rdoParse::RDORTPResType* >& rtp_list = parser->getRTPResType(); 
			std::vector< rdoParse::RDORTPResType* >::const_iterator rtp_it = rtp_list.begin();
			while ( rtp_it != rtp_list.end() ) {
				const std::vector< const rdoParse::RDORTPParam* >& param_list = (*rtp_it)->getParams();
				std::vector< const rdoParse::RDORTPParam* >::const_iterator param_it = param_list.begin();
				while ( param_it != param_list.end() ) {
					data->result += (*param_it)->getName() + ' ';
					param_it++;
				}
				rtp_it++;
			}
			msg.unlock();
			if ( parser != rdoParse::parser ) delete parser;
			parser = NULL;
			break;
		}
	}
}

} // namespace rdoSimulator
