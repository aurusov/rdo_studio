/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosimwin.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
	#include <conio.h>
#endif // COMPILER_VISUAL_STUDIO
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#ifdef COMPILER_GCC
//#   define DISABLE_CONVERTER
#endif // COMPILER_GCC

#include "utils/rdodebug.h"
#include "utils/rdotime.h"
#include "kernel/rdokernel.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/mbuilder/rdo_resources.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
#include "simulator/service/rdosimwin.h"

#ifndef DISABLE_CONVERTER
	#include "converter/smr2rdox/rdoparser.h"
#endif // DISABLE_CONVERTER
// --------------------------------------------------------------------------------

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
	RDOCorba_i()
	{}
	virtual ~RDOCorba_i()
	{}

	virtual PTR(rdo::compiler::parser::RDOCorba::GetRTP) getRDORTPlist (REF(::CORBA::Long) rtp_count);
	virtual PTR(rdo::compiler::parser::RDOCorba::GetRSS) getRDORSSPlist(REF(::CORBA::Long) rss_count);

	static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref);
};

PTR(rdo::compiler::parser::RDOCorba::GetRTP) RDOCorba_i::getRDORTPlist(REF(::CORBA::Long) rtp_count)
{
	//! Создаем список структур для хранения информации об искомых типах ресурсов
	rdo::compiler::parser::RDOCorba::GetRTP_var my_rtpList = new rdo::compiler::parser::RDOCorba::GetRTP;

	//! Получаем необходимые нам данные о типах ресурсов РДО
	kernel->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RTP, &my_rtpList);
	
	return my_rtpList._retn();
}

PTR(rdo::compiler::parser::RDOCorba::GetRSS) RDOCorba_i::getRDORSSPlist(REF(::CORBA::Long) rss_count)
{
	//! Создаем список структур для хранения информации об искомых ресурсах
	rdo::compiler::parser::RDOCorba::GetRSS_var my_rssList = new rdo::compiler::parser::RDOCorba::GetRSS;
	
	//! Получаем необходимые нам данные о ресурсах РДО
	kernel->sendMessage(kernel->simulator(), RDOThread::RT_CORBA_PARSER_GET_RSS, &my_rssList);
	
	return my_rssList._retn();
}

CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref, CPTR(char) ModelName)
{
	CosNaming::NamingContext_var rootContext;

	try
	{
		//! Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references("NameService");
		
		//! Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);
		if (CORBA::is_nil(rootContext))
		{
			TRACE("Failed to narrow the root naming context.");
			return 0;
		}
	}
	catch (REF(CORBA::NO_RESOURCES))
	{
		TRACE("Caught NO_RESOURCES exception. You must configure omniORB with the location of the naming service.");
		return 0;
	}
	catch (REF(CORBA::ORB::InvalidName))
	{
		//! This should not happen!
		TRACE("Service required is invalid [does not exist].");
		return 0;
	}

	try
	{
		//! Bind a context called "test" to the root context:
		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id   = (CPTR(char)) "RDO";         //! string copied
		contextName[0].kind = (CPTR(char)) "RDO_context"; //! string copied
	
		//! Note on kind: The kind field is used to indicate the type
		//! of the object. This is to avoid conventions such as that used
		//! by files (name.type -- e.g. test.ps = postscript etc.)
		CosNaming::NamingContext_var testContext;
		
		try
		{
			//! Bind the context to root.
			testContext = rootContext->bind_new_context(contextName);
		}
		catch(REF(CosNaming::NamingContext::AlreadyBound))
		{
			//! If the context already exists, this exception will be raised.
			//! In this case, just resolve the name and assign testContext
			//! to the object returned:
			CORBA::Object_var obj;
			obj = rootContext->resolve(contextName);
			
			testContext = CosNaming::NamingContext::_narrow(obj);
			
			if (CORBA::is_nil(testContext))
			{
				TRACE("Failed to narrow naming context.");
				return 0;
			}
		}

		//! Bind objref with name Echo to the testContext:
		CosNaming::Name objectName;
		objectName.length(1);
		
		//! rdo::compiler::parser::RDOParserSMRInfo parser;
		//! parser.parse();

		objectName[0].id   = ModelName;
		objectName[0].kind = (CPTR(char)) "Object";

		try
		{
			testContext->bind(objectName, objref);
		}
		catch(REF(CosNaming::NamingContext::AlreadyBound))
		{
			testContext->rebind(objectName, objref);
		}
		
		//! Note: Using rebind() will overwrite any Object previously bound
		//! to /test/Echo with obj.
		//! Alternatively, bind() can be used, which will raise a
		//! CosNaming::NamingContext::AlreadyBound exception if the name
		//! supplied is already bound to an object.
		//! Amendment: When using OrbixNames, it is necessary to first try bind
		//! and then rebind, as rebind on it’s own will throw a NotFoundexception if
		//! the Name has not already been bound. [This is incorrect behaviour -
		//! it should just bind].
	}
	catch(REF(CORBA::TRANSIENT))
	{
		TRACE("Caught system exception TRANSIENT -- unable to contact the naming service.");
		TRACE("Make sure the naming server is running and that omniORB is configured correctly.");
		return 0;
	}
	catch(REF(CORBA::SystemException) ex)
	{
		TRACE1("Caught a CORBA:: %s while using the naming service.", ex._name());
		return 0;
	}
	
	return 1;
}

ruint RDOThreadCorba::corbaRunThreadFun(PTR(void) pParam)
{
	try
	{
		int argc = 0;
		g_orb = CORBA::ORB_init(argc, NULL);
		
		CORBA::Object_var obj = g_orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
		
		PTR(RDOCorba_i) myrdocorba = new RDOCorba_i();

		PortableServer::ObjectId_var myrdocorbaid = poa->activate_object(myrdocorba);

		//! Obtain a reference to the object, and register it in
		//! the naming service.
		obj = myrdocorba->_this();

		CPTR(char) ModelName = "ЦЕХ";
		//! CPTR(char) ModelName = "СКЛАД";

		if (!bindObjectToName(g_orb, obj, ModelName))
		{
			delete myrdocorba;
			return 1;
		}

		myrdocorba->_remove_ref();
		
		PortableServer::POAManager_var pman = poa->the_POAManager();
		
		pman->activate();
		
		g_orb->run();
	}
	catch(REF(CORBA::SystemException) ex)
	{
		TRACE1("Caught CORBA::%s", ex._name());
	}
	catch(REF(CORBA::Exception))
	{
		TRACE("Caught CORBA::Exception: ");
	}	
	catch(REF(omniORB::fatalException) fe)
	{
		TRACE3("Caught omniORB::fatalException: file: %s, line: %d, mesg: %s ", fe.file(), fe.line(), fe.errmsg());
	}
	
	return 0;
}


RDOThreadCorba::RDOThreadCorba()
	: RDOThreadMT             ("RDOThreadCorba")
	, thread_corbaRunThreadFun(NULL            )
{
	//! Место для регистации сообщений корбе
	after_constructor();
}

void RDOThreadCorba::proc(REF(RDOMessageInfo) msg)
{
	//! Место для обработки сообщений корбе
}

void RDOThreadCorba::start()
{
#ifdef TR_TRACE
	trace(thread_name + " corba begin");
#endif

	//! Место для запуска корбы
	thread_corbaRunThreadFun = AfxBeginThread(corbaRunThreadFun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	thread_corbaRunThreadFun->m_bAutoDelete = false;
	thread_corbaRunThreadFun->ResumeThread();

#ifdef TR_TRACE
	trace(thread_name + " corba end");
#endif
}

void RDOThreadCorba::idle()
{
#ifdef RDO_MT
	if (broadcast_waiting || !was_start || was_close)
	{
		RDOThread::idle();
		return;
	}
#endif
	//! Вот тут основная работа корбы
}

void RDOThreadCorba::stop()
{
#ifdef TR_TRACE
	trace(thread_name + " stop begin");
#endif
	//! Место для остановки корбы
	if (g_orb != CORBA::ORB::_nil())
	{
		try
		{
			g_orb->shutdown(true);
			g_orb->destroy();
		}
		catch (...)
		{}
	}

	if (thread_corbaRunThreadFun)
	{
		DWORD res;
		while (true)
		{
			if (::GetExitCodeThread(thread_corbaRunThreadFun->m_hThread, &res))
			{
				if 	(res == STILL_ACTIVE)
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
				UNUSED(i);
			}
		}
//		thread_corbaRunThreadFun->Delete();
		delete thread_corbaRunThreadFun;
		thread_corbaRunThreadFun = NULL;
	}

	RDOThread::stop();
#ifdef TR_TRACE
	trace(thread_name + " stop end");
#endif
}

} //! namespace rdoCorba

#endif // CORBA_ENABLE


OPEN_RDO_SERVICE_SIMULATION_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeTracer
// --------------------------------------------------------------------------------
class RDORuntimeTracer: public rdo::runtime::RDOTrace, public rdo::runtime::RDOEndL
{
public:
	virtual REF(rdo::ostream)          getOStream()    { return m_stream; }
	virtual REF(rdo::runtime::RDOEndL) getEOL()        { return *this;    }

	void onEndl()
	{
		CREF(tstring) trace_str = m_stream.str();
		if (trace_str.empty()        ) return;
		if (!m_pSimulator->m_canTrace) return;
		tstring::size_type pos = 0;
		for (;;)
		{
			tstring::size_type next = trace_str.find('\n', pos);
			tstring str = trace_str.substr(pos, next-pos);
			m_pSimulator->m_pThreadRuntime->broadcastMessage(RDOThread::RT_RUNTIME_TRACE_STRING, &str, true);
			if (next == tstring::npos)
			{
				break;
			}
			pos = next + 1;
			if (pos >= trace_str.length())
			{
				break;
			}
		}
		m_stream.str("");
	}

	RDORuntimeTracer(PTR(RDOThreadSimulator) pSimulator)
		: m_pSimulator(pSimulator)
	{
		m_isNullTracer = false;
	}

private:
	PTR(RDOThreadSimulator) m_pSimulator;
	rdo::textstream         m_stream;
};

// --------------------------------------------------------------------------------
// -------------------- RDOSimResulter
// --------------------------------------------------------------------------------
class RDOSimResulter: public rdo::runtime::RDOResults
{
public:
	RDOSimResulter(PTR(RDOThreadSimulator) pSimulator, REF(rdo::ostream) stream)
		: m_pSimulator(pSimulator)
		, m_stream    (stream    )
	{}

private:
	PTR(RDOThreadSimulator) m_pSimulator;
	REF(rdo::ostream)       m_stream;
	rdo::textstream         m_buffer;

	virtual REF(rdo::ostream) getOStream()
	{
		return m_buffer;
	}

	void flush()
	{
		CREF(tstring) bufferStr = m_buffer.str();
		if (bufferStr.empty())
		{
			return;
		}

		m_stream << bufferStr;

		tstring::size_type pos = 0;
		for (;;)
		{
			tstring::size_type next = bufferStr.find('\n', pos);
			tstring str = bufferStr.substr(pos, next-pos+1);
			m_pSimulator->m_pThreadRuntime->broadcastMessage(RDOThread::RT_RESULT_STRING, &str, true);
			if (next == tstring::npos)
			{
				break;
			}
			pos = next + 1;
			if (pos >= bufferStr.length())
			{
				break;
			}
		}
		m_buffer.str("");
	}
};

// --------------------------------------------------------------------------------
// -------------------- RDOSimResultInformer
// --------------------------------------------------------------------------------
class RDOSimResultInformer: public rdo::runtime::RDOResults, public boost::noncopyable
{
public:
	RDOSimResultInformer(REF(rdo::ostream) stream)
		: m_stream(stream)
	{}

private:
	REF(rdo::ostream) m_stream;

	virtual REF(rdo::ostream) getOStream()
	{
		return m_stream;
	}

	void flush()
	{
		NEVER_REACH_HERE;
	}
};

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE

OPEN_RDO_RUNTIME_NAMESPACE
// --------------------------------------------------------------------------------
// -------------------- RDOThreadRunTime
// --------------------------------------------------------------------------------
RDOThreadRunTime::RDOThreadRunTime()
	: RDOThreadMT   ("RDOThreadRunTime")
	, m_pSimulator  (NULL                  )
	, m_runtimeError(false                 )
{
	rdo::Time time;
	
	m_timeStart = time.local();
	m_timeStart /= 1000000;

	m_pSimulator = kernel->simulator();

	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY);
	notifies.push_back(RT_RUNTIME_GET_MODE                  );
	notifies.push_back(RT_RUNTIME_SET_MODE                  );
	notifies.push_back(RT_RUNTIME_GET_SPEED                 );
	notifies.push_back(RT_RUNTIME_SET_SPEED                 );
	notifies.push_back(RT_RUNTIME_GET_SHOWRATE              );
	notifies.push_back(RT_RUNTIME_SET_SHOWRATE              );
	notifies.push_back(RT_RUNTIME_GET_TIMENOW               );
	notifies.push_back(RT_RUNTIME_GET_FRAME                 );
	notifies.push_back(RT_RUNTIME_KEY_DOWN                  );
	notifies.push_back(RT_RUNTIME_KEY_UP                    );
	notifies.push_back(RT_RUNTIME_FRAME_AREA_DOWN           );
	after_constructor();
}

void RDOThreadRunTime::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_THREAD_CLOSE:
		{
			broadcastMessage(RT_RUNTIME_MODEL_STOP_BEFORE);
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY:
		{
			m_runtimeError = true;
			break;
		}
		case RT_RUNTIME_GET_MODE:
		{
			*static_cast<PTR(rdo::runtime::RunTimeMode)>(msg.param) = m_pSimulator->m_pRuntime->getMode();
			break;
		}
		case RT_RUNTIME_SET_MODE:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->setMode(*static_cast<PTR(rdo::runtime::RunTimeMode)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_SPEED:
		{
			*static_cast<PTR(double)>(msg.param) = m_pSimulator->m_pRuntime->getSpeed();
			break;
		}
		case RT_RUNTIME_SET_SPEED:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->setSpeed(*static_cast<PTR(double)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_SHOWRATE:
		{
			*static_cast<PTR(double)>(msg.param) = m_pSimulator->m_pRuntime->getShowRate();
			break;
		}
		case RT_RUNTIME_SET_SHOWRATE:
		{
			msg.lock();
			m_pSimulator->m_pRuntime->setShowRate(*static_cast<PTR(double)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_TIMENOW:
		{
			*static_cast<PTR(double)>(msg.param) = m_pSimulator->m_pRuntime->getTimeNow();
			break;
		}
		case RT_RUNTIME_GET_FRAME:
		{
			msg.lock();
			PTR(GetFrame) getframe = static_cast<PTR(GetFrame)>(msg.param);
			m_pSimulator->m_pRuntime->m_frameList.at(getframe->m_number)->prepareFrame(getframe->m_pFrame, m_pSimulator->m_pRuntime);
			msg.unlock();
			break;
		}
		case RT_RUNTIME_GET_LAST_BREAKPOINT:
		{
			msg.lock();
			*static_cast<PTR(tstring)>(msg.param) = m_pSimulator->m_pRuntime->getLastBreakPointName();
			msg.unlock();
			break;
		}
		case RT_RUNTIME_KEY_DOWN:
		{
			msg.lock();
			RDOHotKey::KeyCode keyCode = *static_cast<PTR(RDOHotKey::KeyCode)>(msg.param);
			msg.unlock();

			if (m_pSimulator->m_pRuntime->hotkey().keyInModel().check(keyCode))
			{
				if (!m_pSimulator->m_pRuntime->hotkey().keyDown().down(keyCode))
				{
					m_pSimulator->m_pRuntime->setShowRate(m_pSimulator->m_pRuntime->getShowRate());
				}
			}
			break;
		}
		case RT_RUNTIME_KEY_UP:
		{
			msg.lock();
			RDOHotKey::KeyCode keyCode = *static_cast<PTR(RDOHotKey::KeyCode)>(msg.param);
			msg.unlock();
			m_pSimulator->m_pRuntime->hotkey().keyDown().up(keyCode);
			break;
		}
		case RT_RUNTIME_FRAME_AREA_DOWN:
		{
			msg.lock();
			tstring areaName = *static_cast<PTR(tstring)>(msg.param);
			msg.unlock();
			m_pSimulator->m_pRuntime->hotkey().areaList().click(areaName);
			m_pSimulator->m_pRuntime->setShowRate(m_pSimulator->m_pRuntime->getShowRate());
			break;
		}
		default: break;
	}
}

void RDOThreadRunTime::start()
{
#ifdef TR_TRACE
	trace(thread_name + " start begin");
#endif

	broadcastMessage(RT_RUNTIME_MODEL_START_BEFORE);

	PTR(RDOTrace)                 pTracer;
	PTR(rdo::runtime::RDOResults) pResults;
	PTR(rdo::runtime::RDOResults) pResultsInfo;

	//! Creating tracer and results
	pTracer = new rdo::service::simulation::RDORuntimeTracer(m_pSimulator);

	m_pSimulator->m_resultString.str("");
	pResults = new rdo::service::simulation::RDOSimResulter(m_pSimulator, m_pSimulator->m_resultString);

	m_pSimulator->m_resultInfoString.str("");
	pResultsInfo = new rdo::service::simulation::RDOSimResultInformer(m_pSimulator->m_resultInfoString);

	//! RDO config initialization
	m_pSimulator->m_pRuntime->hotkey().clear();
	m_pSimulator->m_pRuntime->setStartTime     (m_pSimulator->m_pParser->getSMR()->getRunStartTime  ());
	m_pSimulator->m_pRuntime->setTraceStartTime(m_pSimulator->m_pParser->getSMR()->getTraceStartTime());
	m_pSimulator->m_pRuntime->setTraceEndTime  (m_pSimulator->m_pParser->getSMR()->getTraceEndTime  ());

	//! Modelling
	m_pSimulator->m_canTrace = true;

	try
	{
		LPRDOThreadRunTime pThis(this);
		m_pSimulator->m_exitCode = rdo::simulation::report::EC_OK;
		m_pSimulator->m_pRuntime->rdoInit(pTracer, pResults, pResultsInfo, pThis.interface_cast<IThreadProxy>());
		switch (m_pSimulator->m_pParser->getSMR()->getShowMode())
		{
			case rdo::service::simulation::SM_NoShow   : m_pSimulator->m_pRuntime->setMode(rdo::runtime::RTM_MaxSpeed); break;
			case rdo::service::simulation::SM_Animation: m_pSimulator->m_pRuntime->setMode(rdo::runtime::RTM_Sync    ); break;
			case rdo::service::simulation::SM_Monitor  : m_pSimulator->m_pRuntime->setMode(rdo::runtime::RTM_Pause   ); break;
		}
		m_pSimulator->m_pRuntime->setShowRate(m_pSimulator->m_pParser->getSMR()->getShowRate());
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{
		m_runtimeError = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
	}
	catch (REF(rdo::runtime::RDORuntimeException) ex)
	{
		m_runtimeError = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
		tstring mess = ex.getType() + " : " + ex.message();
		RDOThreadMT::sendMessage(kernel, RDOThread::RT_DEBUG_STRING, &mess);
	}

	if (!m_runtimeError)
	{
		broadcastMessage(RT_RUNTIME_MODEL_START_AFTER);
	}

#ifdef TR_TRACE
	trace(thread_name + " start end, runing simulation");
#endif
}

void RDOThreadRunTime::idle()
{
#ifdef RDO_MT
	if (broadcast_waiting || !was_start || was_close)
	{
		RDOThread::idle();
		return;
	}
#endif
//	TRACE("R. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId());
	try
	{
		if (m_runtimeError || !m_pSimulator->m_pRuntime->rdoNext())
		{
			LPRDOThreadRunTime pThis(this); //! Увеличим на себя ссылку, чтобы нас не удалил симулятор раньше времени
			RDOThreadMT::sendMessage(this, RT_THREAD_CLOSE);
		}
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{
		m_runtimeError = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
	}
	catch (REF(rdo::runtime::RDORuntimeException) ex)
	{
		m_runtimeError = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
		tstring mess = ex.getType() + " : " + ex.message();
		RDOThreadMT::sendMessage(kernel, RDOThread::RT_DEBUG_STRING, &mess);
	}
//	catch (...) {
//		m_runtimeError = true;
//		TRACE("******************************** Ошибка rdoNext()\n");
//		m_pSimulator->m_pRuntime->onRuntimeError();
//	}
}

void RDOThreadRunTime::writeResultsInfo()
{
	switch (m_pSimulator->m_pRuntime->m_whyStop)
	{
		case rdo::simulation::report::EC_OK:
			m_pSimulator->m_pRuntime->getResultsInfo() << "$Status = " << "NORMAL_TERMINATION";
			break;
		case rdo::simulation::report::EC_NoMoreEvents:
			m_pSimulator->m_pRuntime->getResultsInfo() << "$Status = " << "NO_MORE_EVENTS";
			break;
		case rdo::simulation::report::EC_RunTimeError:
			m_pSimulator->m_pRuntime->getResultsInfo() << "$Status = " << "RUN_TIME_ERROR";
			break;
		case rdo::simulation::report::EC_UserBreak:
			m_pSimulator->m_pRuntime->getResultsInfo() << "$Status = " << "USER_BREAK";
			break;
		default:
			break;
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << "$Result_values  0  " << m_pSimulator->m_pRuntime->getTimeNow();
	
	rdo::Time time;
	
	ruint64 timeStop = time.local();
	timeStop /= 1000000;
	
	double delay = -1;

	delay = (double)(timeStop - m_timeStart);
	
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << "  " << delay / 1000.0;
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << "  ?";
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << "  EventCount           " << m_pSimulator->m_pRuntime->get_cnt_events() << "  " << (double)m_pSimulator->m_pRuntime->get_cnt_events() / m_pSimulator->m_pRuntime->getTimeNow() << "  ";
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_events() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << "?";
	}
	
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << "  OperRuleCheckCounter " << m_pSimulator->m_pRuntime->get_cnt_choice_from() << "  " << (double)m_pSimulator->m_pRuntime->get_cnt_choice_from() / m_pSimulator->m_pRuntime->getTimeNow() << "  ";
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_choice_from() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << "?";
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << "  AExpCalcCounter      " << m_pSimulator->m_pRuntime->get_cnt_calc_arithm() << "  " << (double)m_pSimulator->m_pRuntime->get_cnt_calc_arithm() / m_pSimulator->m_pRuntime->getTimeNow() << "  ";
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_calc_arithm() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << "?";
	}
	m_pSimulator->m_pRuntime->getResultsInfo() << '\n' << "  BExpCalcCounter      " << m_pSimulator->m_pRuntime->get_cnt_calc_logic() << "  " << (double)m_pSimulator->m_pRuntime->get_cnt_calc_logic() / m_pSimulator->m_pRuntime->getTimeNow() << "  ";
	if (delay != -1)
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << (ruint)((double)m_pSimulator->m_pRuntime->get_cnt_calc_logic() / delay * 1000);
	}
	else
	{
		m_pSimulator->m_pRuntime->getResultsInfo() << "?";
	}
}

void RDOThreadRunTime::stop()
{
#ifdef TR_TRACE
	trace(thread_name + " stop begin");
#endif

	try
	{
		m_pSimulator->m_pRuntime->rdoPostProcess();
		writeResultsInfo();
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{
		m_runtimeError = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
	}
	catch (REF(rdo::runtime::RDORuntimeException) ex)
	{
		m_runtimeError = true;
		m_pSimulator->m_pRuntime->onRuntimeError();
		tstring mess = ex.getType() + " : " + ex.message();
		RDOThreadMT::sendMessage(kernel, RDOThread::RT_DEBUG_STRING, &mess);
	}

	broadcastMessage(RT_RUNTIME_MODEL_STOP_AFTER);

	RDOThread::stop();

#ifdef TR_TRACE
	trace(thread_name + " stop end");
#endif
}

void RDOThreadRunTime::destroy()
{}

rbool RDOThreadRunTime::runtimeError() const
{
	return m_runtimeError;
}

void RDOThreadRunTime::sendMessage(ThreadID threadID, ruint messageID, PTR(void) pParam)
{
	PTR(RDOThread) pThread;
	switch (threadID)
	{
	case TID_REPOSITORY: pThread = kernel->repository(); break;
	default            : pThread = NULL; break;
	}

	if (pThread)
	{
		RDOThreadMT::sendMessage(pThread, static_cast<RDOTreadMessage>(messageID), pParam);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_SERVICE_SIMULATION_NAMESPACE
// --------------------------------------------------------------------------------
// -------------------- RDOThreadSimulator
// --------------------------------------------------------------------------------
RDOThreadSimulator::RDOThreadSimulator()
	: RDOThreadMT     ("RDOThreadSimulator"          )
	, m_pThreadRuntime(NULL                          )
	, m_exitCode      (rdo::simulation::report::EC_OK)
	, m_runCount      (0                             )
	, m_runNumber     (0                             )
{
	notifies.push_back(RT_STUDIO_MODEL_BUILD              );
	notifies.push_back(RT_STUDIO_MODEL_RUN                );
	notifies.push_back(RT_STUDIO_MODEL_STOP               );
	notifies.push_back(RT_SIMULATOR_GET_MODEL_STRUCTURE   );
	notifies.push_back(RT_SIMULATOR_GET_MODEL_RESULTS     );
	notifies.push_back(RT_SIMULATOR_GET_MODEL_RESULTS_INFO);
	notifies.push_back(RT_SIMULATOR_GET_MODEL_EXITCODE    );
	notifies.push_back(RT_SIMULATOR_GET_LIST              );
	notifies.push_back(RT_SIMULATOR_GET_ERRORS            );
	notifies.push_back(RT_THREAD_STOP_AFTER               );
	notifies.push_back(RT_CODECOMP_GET_DATA               );
#ifdef CORBA_ENABLE
	notifies.push_back(RT_CORBA_PARSER_GET_RTP            );
	notifies.push_back(RT_CORBA_PARSER_GET_RSS            );
	notifies.push_back(RT_CORBA_PARSER_GET_RTP_COUNT      );
	notifies.push_back(RT_CORBA_PARSER_GET_RTP_PAR_COUNT  );
#endif
	notifies.push_back(RT_PROCGUI_BLOCK_CREATE            );
	notifies.push_back(RT_PROCGUI_BLOCK_TERMINATE         );
	notifies.push_back(RT_PROCGUI_BLOCK_PROCESS           );
	after_constructor();
}

RDOThreadSimulator::~RDOThreadSimulator()
{
	terminateModel();
	closeModel    ();
}

void RDOThreadSimulator::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_STUDIO_MODEL_BUILD:
		{
			parseModel();
			break;
		}
		case RT_STUDIO_MODEL_RUN:
		{
			runModel();
			break;
		}
		case RT_STUDIO_MODEL_STOP:
		{
			stopModel();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_STRUCTURE:
		{
			msg.lock();
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_pParser->getModelStructure();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS:
		{
			msg.lock();
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_resultString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_RESULTS_INFO:
		{
			msg.lock();
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_pParser->getChanges();
			*static_cast<PTR(rdo::textstream)>(msg.param) << std::endl << std::endl;
			*static_cast<PTR(rdo::textstream)>(msg.param) << m_resultInfoString.str();
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_MODEL_EXITCODE:
		{
			msg.lock();
			*static_cast<PTR(rdo::simulation::report::RDOExitCode)>(msg.param) = m_exitCode;
			msg.unlock();
			break;
		}
		case RT_CODECOMP_GET_DATA:
		{
			codeCompletion();
			break;
		}
		case RT_PROCGUI_BLOCK_CREATE:
		{
			m_pGUIProcess = rdo::Factory<rdo::compiler::gui::ProcGUIProcess>::create(m_pRuntime);
			msg.lock();
			PTR(rdo::compiler::gui::RPShapeDataBlockCreate) pRawParams = static_cast<PTR(rdo::compiler::gui::RPShapeDataBlockCreate)>(msg.param);
			rdo::compiler::gui::LPRPShapeDataBlockCreate pParams(pRawParams);
			m_pBlock = rdo::Factory<rdo::compiler::gui::ProcGUIBlockGenerate>::create(m_pGUIProcess, m_pRuntime, m_pParser, pParams);
			msg.unlock();
			ASSERT(m_pBlock);
			pRawParams = NULL;
			m_pBlock   = NULL;
			pParams    = NULL;
			break;
		}
		case RT_PROCGUI_BLOCK_PROCESS:
		{
			ASSERT(m_pGUIProcess);
			msg.lock();
			PTR(rdo::compiler::gui::RPShapeDataBlockProcess) pRawParams = static_cast<PTR(rdo::compiler::gui::RPShapeDataBlockProcess)>(msg.param);
			rdo::compiler::gui::LPRPShapeDataBlockProcess pParams(pRawParams);
			m_pBlock = rdo::Factory<rdo::compiler::gui::ProcGUIBlockProcess>::create(m_pGUIProcess, m_pRuntime, m_pParser, pParams);
			msg.unlock();
			ASSERT(m_pBlock);
			pRawParams = NULL;
			m_pBlock   = NULL;
			pParams    = NULL;
			break;
		}
		case RT_PROCGUI_BLOCK_TERMINATE:
		{
			ASSERT(m_pGUIProcess);
			msg.lock();
			PTR(rdo::compiler::gui::RPShapeDataBlockTerminate) pRawParams = static_cast<PTR(rdo::compiler::gui::RPShapeDataBlockTerminate)>(msg.param);
			rdo::compiler::gui::LPRPShapeDataBlockTerminate pParams(pRawParams);
			m_pBlock = rdo::Factory<rdo::compiler::gui::ProcGUIBlockTerminate>::create(m_pGUIProcess, pParams);
			msg.unlock();
			ASSERT(m_pBlock);
			m_pGUIProcess->clear();
			m_pGUIProcess = NULL;
			pRawParams    = NULL;
			m_pBlock      = NULL;
			pParams       = NULL;
			break;
		}
		
#ifdef CORBA_ENABLE

		case RT_CORBA_PARSER_GET_RTP:
		{
			msg.lock();
			corbaGetRTP(*static_cast<PTR(rdo::compiler::parser::RDOCorba::GetRTP_var)>(msg.param));
			msg.unlock();
			break;
		}

		case RT_CORBA_PARSER_GET_RSS:
		{
			msg.lock();
			corbaGetRSS(*static_cast<PTR(rdo::compiler::parser::RDOCorba::GetRSS_var)>(msg.param));
			msg.unlock();
			break;
		}
#endif // CORBA_ENABLE

		case RT_SIMULATOR_GET_LIST:
		{
			msg.lock();
			PTR(GetList) getlist = static_cast<PTR(GetList)>(msg.param);
			switch (getlist->m_type)
			{
				case GetList::frames:
				{
					if (m_pRuntime)
					{
						int size = m_pRuntime->m_frameList.size();
						for (int i = 0; i < size; i++)
						{
							getlist->m_list->push_back(m_pRuntime->m_frameList.at(i)->name());
						}
					}
					break;
				}
				case GetList::bitmaps:
				{
					if (m_pRuntime)
					{
						int size = m_pRuntime->m_frameList.size();
						for (int i = 0; i < size; i++)
						{
							m_pRuntime->m_frameList.at(i)->getBitmaps(*getlist->m_list);
						}
					}
					break;
				}
			}
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_ERRORS:
		{
			SyntaxMessageList m_errorList = getErrors();
			msg.lock();
			PTR(SyntaxMessageList) msg_errors = static_cast<PTR(SyntaxMessageList)>(msg.param);
			msg_errors->assign(m_errorList.begin(), m_errorList.end());
			msg.unlock();
			break;
		}
		case RT_THREAD_STOP_AFTER:
		{
			if (msg.from == m_pThreadRuntime.get())
			{
				//! rdo::simulation::report::EC_ParserError   - Не используется в run-time
				//! rdo::simulation::report::EC_ModelNotFound - Не используется в run-time
				//! rdo::simulation::report::EC_UserBreak     - Устанавливается в m_pSimulator, перехват RT_THREAD_STOP_AFTER не срабатывает
				m_exitCode = m_pRuntime->m_whyStop;
				if (!m_pThreadRuntime->runtimeError())
				{
					//! Остановились сами нормально
					broadcastMessage(RT_SIMULATOR_MODEL_STOP_OK);
					closeModel();
					runModel();//если это не последний прогон, то запускаемся еще раз
				}
				else
				{
					//! Остановились сами, но не нормально
					broadcastMessage(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
					closeModel();
				}
				//! Треда удаляется сама, надо удалить её событие
				//! Делается это без мутексов, т.к. thread_destroy не должна использоваться в m_pThreadRuntime пока обрабатывается RT_THREAD_STOP_AFTER
#ifdef RDO_MT
				delete m_pThreadRuntime->thread_destroy;
				m_pThreadRuntime->thread_destroy = NULL;
#endif
			}
			break;
		}
		default: break;
	}
}

rbool RDOThreadSimulator::parseModel()
{
	terminateModel();
	closeModel();

	m_pParser = rdo::Factory<rdo::compiler::parser::RDOParserModel>::create();
	ASSERT(m_pParser);
	m_pParser->init();
	m_pRuntime = m_pParser->runtime();
	ASSERT(m_pRuntime);

	try
	{
		m_exitCode = rdo::simulation::report::EC_OK;
		if (m_runNumber != 0)
		{
			m_pParser->parse(m_runNumber);// parse i-го блока
		}
		else
		{
			m_pParser->parse();// parse всей модели
		}
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{
		m_exitCode = rdo::simulation::report::EC_ParserError;
		broadcastMessage(RT_SIMULATOR_PARSE_ERROR);
		closeModel();
		return false;
	}
	catch (REF(rdo::runtime::RDORuntimeException) ex)
	{
		tstring mess = ex.getType() + " : " + ex.message();
		broadcastMessage(RT_SIMULATOR_PARSE_STRING, &mess);
		m_exitCode = rdo::simulation::report::EC_ParserError;
		broadcastMessage(RT_SIMULATOR_PARSE_ERROR);
		closeModel();
		return false;
	}

	m_showMode = getInitialShowMode();
	m_showRate = getInitialShowRate();
	if (m_runNumber == 0)
	{
		m_runCount = getInitialRunCount();
	}

	broadcastMessage(RT_SIMULATOR_PARSE_OK);

	return true;
}


void RDOThreadSimulator::runModel()
{
	if (m_runNumber < m_runCount || m_runNumber ==0)
	{
		++m_runNumber;
		parseModel();

		ASSERT(m_pParser );
		ASSERT(m_pRuntime);

		m_pParser->error().clear();
		m_exitCode = rdo::simulation::report::EC_OK;
		m_pRuntime->setStudioThread(kernel->studio());
		m_pThreadRuntime = rdo::Factory<rdo::runtime::RDOThreadRunTime>::create();
	}
	else
	{
		m_runNumber = 0;
	}
}

void RDOThreadSimulator::stopModel()
{
	m_pRuntime->onUserBreak();
	m_exitCode = rdo::simulation::report::EC_UserBreak;
	terminateModel();
	m_canTrace = false;
	broadcastMessage(RT_SIMULATOR_MODEL_STOP_BY_USER);
	closeModel();
//	kernel.callback(RDOKernel::modelExit, rdo::simulation::report::EC_UserBreak);
}

void RDOThreadSimulator::terminateModel()
{
	if (m_pThreadRuntime)
	{
		//! Перестали реагировать на остановку run-time-треды, т.к. закрываем её сами
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.erase(std::find(notifies.begin(), notifies.end(), RT_THREAD_STOP_AFTER));
#ifdef RDO_MT
		notifies_mutex.Unlock();
		PTR(CEvent) thread_destroy = m_pThreadRuntime->thread_destroy;
#endif

		sendMessage(m_pThreadRuntime.get(), RDOThread::RT_THREAD_CLOSE);

#ifdef RDO_MT
		thread_destroy->Lock();
		delete thread_destroy;
#endif
		m_pThreadRuntime = NULL;

		//! Опять начали реагировать на остановку run-time-треды, чтобы обнаружить нормальное завершение модели (или по run-time-error)
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.push_back(RT_THREAD_STOP_AFTER);
#ifdef RDO_MT
		notifies_mutex.Unlock();
#endif
	}
}

void RDOThreadSimulator::closeModel()
{
/*
	try
	{
		if (m_pRuntime)
		{
			m_pRuntime = NULL;
		}
	}
	catch (...)
	{
		m_pRuntime = NULL;
		TRACE("******************************** Ошибка удаления m_pRuntime\n");
	}
*/
	m_pRuntime = NULL;
	m_pThreadRuntime = NULL;
	try
	{
		if (m_pParser)
		{
			m_pParser->deinit();
			m_pParser = NULL;
		}
	}
	catch (...)
	{
		m_pParser = NULL;
		TRACE("******************************** Ошибка удаления m_pParser\n");
	}
}

void RDOThreadSimulator::parseSMRFileInfo(REF(rdo::converter::smr2rdox::RDOSMRFileInfo) info)
{
#ifdef DISABLE_CONVERTER
	UNUSED(info);
#else
	try
	{
		rdo::converter::smr2rdox::RDOParserModel converter;
		rdo::repository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::SMR);
		kernel->sendMessage(kernel->repository(), RT_REPOSITORY_MODEL_GET_FILEINFO, &fileInfo);
		rdo::converter::smr2rdox::RDOParserModel::Result res = converter.convert(fileInfo.m_fullName, info);
		switch (res)
		{
		case rdo::converter::smr2rdox::RDOParserModel::CNV_NONE :
			{
				broadcastMessage(RT_CONVERTOR_NONE);
			}
			break;

		case rdo::converter::smr2rdox::RDOParserModel::CNV_OK   :
			{
				broadcastMessage(RT_CONVERTOR_OK);
			}
			break;

		case rdo::converter::smr2rdox::RDOParserModel::CNV_ERROR:
			{
				broadcastMessage(RT_CONVERTOR_ERROR);

				tstring mess("Ошибка конвертора\n");
				broadcastMessage(RT_DEBUG_STRING, &mess);
				CREF(rdo::converter::smr2rdox::Error::ErrorList) errorList = converter.error().getList();
				BOOST_AUTO(it, errorList.begin());
				while (it != errorList.end())
				{
					tstring text = it->getText();
					broadcastMessage(RT_DEBUG_STRING, const_cast<PTR(tstring)>(&text));
					++it;
				}
			}
			break;

		default : NEVER_REACH_HERE;
		}
	}
	catch (REF(rdo::converter::smr2rdox::RDOSyntaxException))
	{}
	catch (REF(rdo::runtime::RDORuntimeException))
	{}
	catch (...)
	{}
#endif
}

RDOThreadSimulator::SyntaxMessageList RDOThreadSimulator::getErrors()
{
	SyntaxMessageList res;

	if (!m_pParser)
	{
		return res;
	}

	res = m_pParser->error().getList();
	res.insert(res.end(), m_pRuntime->error().list().begin(), m_pRuntime->error().list().end());
	return res;
}

ShowMode RDOThreadSimulator::getInitialShowMode() const
{
	return m_pParser->getSMR()->getShowMode();
}

int RDOThreadSimulator::getInitialFrameNumber() const
{
	return m_pParser->getSMR()->getFrameNumber();
}

ruint RDOThreadSimulator::getInitialRunCount() const
{
	return m_pParser->getSMR()->getRunCount();
}

double RDOThreadSimulator::getInitialShowRate() const
{
	return m_pParser->getSMR()->getShowRate();
}

ruint RDOThreadSimulator::runNumberCheck() const
{
	return m_runCount;
}

rbool RDOThreadSimulator::OldModelCheckSim() const
{
	return m_pParser->getSMR()->OldModelCheck();
}

void RDOThreadSimulator::codeCompletion()
{}

#ifdef CORBA_ENABLE
void RDOThreadSimulator::corbaGetRTP(REF(rdo::compiler::parser::RDOCorba::GetRTP_var) my_rtpList)
{
	//! Пропарсели типы и ресурсы текста модели (текущие, а не записанные)
	rdo::compiler::parser::RDOParserCorba parser;
	try
	{
		parser.parse();
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{}
	catch (REF(rdo::runtime::RDORuntimeException))
	{}

	::CORBA::Long i = 0, j = 0;

	//! Пробежались по всем типам и переписали в RTPList
	rdo::compiler::mbuilder::RDOResTypeList rtpList(&parser);
	
	//! Считаем количество типов ресурсов
	rdo::compiler::mbuilder::RDOResTypeList::List::const_iterator rtp_it = rtpList.begin();
	
	::CORBA::Long rtp_count = 0;

	while (rtp_it != rtpList.end())
	{
		++rtp_count;
		++rtp_it;
	}

	//! Выделяем пямять под последовательность
	my_rtpList->length(rtp_count);

	//! Снова возвращаемся в начало списка типов ресурсов
	rtp_it = rtpList.begin();

	while (rtp_it != rtpList.end())
	{
		//! Создаем текстовую структуру

		my_rtpList[i].m_name = CORBA::string_dup(rtp_it->name().c_str());
		
		if ((rtp_it->getType()) == rdo::compiler::mbuilder::RDOResType::rt_permanent)
			my_rtpList[i].m_type=rdo::compiler::parser::RDOCorba::rt_permanent;
		else
			my_rtpList[i].m_type=rdo::compiler::parser::RDOCorba::rt_temporary;

		//! Считаем количество параметров i-го типа ресурса
		rdo::compiler::mbuilder::RDOResType::ParamList::List::const_iterator param_it = rtp_it->m_params.begin();
		my_rtpList[i].m_param_count = 0;

		while (param_it != rtp_it->m_params.end())
		{
			++(my_rtpList[i].m_param_count);
			++param_it;
		}
		
		//! Выделяем память под последовательность параметров i-го типа ресурсов
		my_rtpList[i].m_param.length(my_rtpList[i].m_param_count);

		//! Снова возвращаемся в начало списка параметров i-го типа ресурсов
		param_it = rtp_it->m_params.begin();

		while (param_it != rtp_it->m_params.end())
		{
			//! Добавляем в структуру параметр!!!!!!!!!!!!!!!!
			my_rtpList[i].m_param[j].m_name = CORBA::string_dup(param_it->name().c_str());

			my_rtpList[i].m_param[j].m_range_int = 0;
			my_rtpList[i].m_param[j].m_default_int_ch = 0;
			my_rtpList[i].m_param[j].m_range_double = 0;
			my_rtpList[i].m_param[j].m_default_double_ch = 0;
			my_rtpList[i].m_param[j].m_var_enum_ch = 0;	
			my_rtpList[i].m_param[j].m_default_enum_ch = 0;
			my_rtpList[i].m_param[j].m_var_enum_count = 0;

			switch (param_it->typeID())
			{
				case rdo::runtime::RDOType::t_int:
				{
					my_rtpList[i].m_param[j].m_type = rdo::compiler::parser::RDOCorba::int_type;

					if (param_it->hasRange())
					{
						my_rtpList[i].m_param[j].m_min_int = param_it->getMin()->getInt();
						my_rtpList[i].m_param[j].m_max_int = param_it->getMax()->getInt();
						my_rtpList[i].m_param[j].m_range_int = 1;

					}
					if (param_it->hasDefault())
					{
						my_rtpList[i].m_param[j].m_default_int = param_it->getDefault()->getInt();
						my_rtpList[i].m_param[j].m_default_int_ch = 1;
					}
					break;
				}
				case rdo::runtime::RDOType::t_real:
				{
					my_rtpList[i].m_param[j].m_type = rdo::compiler::parser::RDOCorba::double_type;

					if (param_it->hasRange())
					{
						my_rtpList[i].m_param[j].m_min_double = param_it->getMin()->getDouble();
						my_rtpList[i].m_param[j].m_max_double = param_it->getMax()->getDouble();
						my_rtpList[i].m_param[j].m_range_double = 1;

					}
					if (param_it->hasDefault())
					{
						my_rtpList[i].m_param[j].m_default_double = param_it->getDefault()->getDouble();
						my_rtpList[i].m_param[j].m_default_double_ch = 1;
					}
					break;
				}
				case rdo::runtime::RDOType::t_enum:
				{
					my_rtpList[i].m_param[j].m_type = rdo::compiler::parser::RDOCorba::enum_type;
					
					//! Считаем количество значений перечислимого типа
					rdo::runtime::RDOEnumType::CIterator enum_it = param_it->getEnum()->getEnums().begin();

					CORBA::Long k = 0;

					while (enum_it != param_it->getEnum()->getEnums().end())
					{
						k++;					
						enum_it++;
					}

					//! Выделяем память под последовательность значений j-го параметра перечислимого типа i-го типа ресурсов
					my_rtpList[i].m_param[j].m_var_enum.length(k);
					
					enum_it = param_it->getEnum()->getEnums().begin();
					k = 0;

					while (enum_it != param_it->getEnum()->getEnums().end())
					{
						my_rtpList[i].m_param[j].m_var_enum[k] = CORBA::string_dup(enum_it->c_str());
						enum_it++;
						k++;
					}
					
					if (param_it->hasDefault())
					{
						my_rtpList[i].m_param[j].m_default_enum = CORBA::string_dup(param_it->getDefault()->getAsString().c_str());
						my_rtpList[i].m_param[j].m_default_enum_ch = 1;
					}

					my_rtpList[i].m_param[j].m_var_enum_ch = 1;
					my_rtpList[i].m_param[j].m_var_enum_count = k;
					break;
				}
				default: break;
			}
		
			++j;
			++param_it;
		}
		j = 0;
		++i;
		++rtp_it;
	}
}

void RDOThreadSimulator::corbaGetRSS(REF(rdo::compiler::parser::RDOCorba::GetRSS_var) my_rssList)
{
	//! Пропарсели типы и ресурсы текста модели (текущие, а не записанные)
	rdo::compiler::parser::RDOParserCorba parser;
	try
	{
		parser.parse();
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{}
	catch (REF(rdo::runtime::RDORuntimeException))
	{}

	//! Пробежались по всем ресурсам и переписали в RSSList
	rdo::compiler::mbuilder::RDOResourceList rssList(&parser);
	rdo::compiler::mbuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();

	::CORBA::Long i = 0, j = 0;
	::CORBA::Long rss_count = 0;

	//! Считаем количество ресурсов
	while (rss_it != rssList.end())
	{
		++rss_count;
		++rss_it;
	}

	//! Выделяем пямять под последовательность
	my_rssList->length(rss_count);

	//! Снова возвращаемся в начало списка типов ресурсов
	rss_it = rssList.begin();

	while (rss_it != rssList.end())
	{
		//! Заполняем значения структуры
		my_rssList[i].m_name = CORBA::string_dup(rss_it->name().c_str());
		my_rssList[i].m_type = CORBA::string_dup(rss_it->getType().name().c_str());
		
		//! Считаем количество параметров i-го типа ресурса
		rdo::compiler::mbuilder::RDOResource::Params::const_iterator param_it = rss_it->begin();
		
		my_rssList[i].m_param_count = 0;

		while (param_it != rss_it->end())
		{
			++(my_rssList[i].m_param_count);
			++param_it;
		}
		
		//! Выделяем память под последовательность параметров i-го ресурса
		my_rssList[i].m_param.length(my_rssList[i].m_param_count);

		//! Снова возвращаемся в начало списка параметров i-го ресурса
		param_it = rss_it->begin();

		while (param_it != rss_it->end())
		{
			my_rssList[i].m_param[j].m_name = CORBA::string_dup(param_it->first.c_str());;
		
			switch (param_it->second.typeID())
			{
				case rdo::runtime::RDOType::t_int:
				{
					
					my_rssList[i].m_param[j].m_int = param_it->second->getInt();
					my_rssList[i].m_param[j].m_type = rdo::compiler::parser::RDOCorba::int_type;

					break;
				}
				case rdo::runtime::RDOType::t_real:
				{

					my_rssList[i].m_param[j].m_double = param_it->second->getDouble();
					my_rssList[i].m_param[j].m_type = rdo::compiler::parser::RDOCorba::double_type;
					
					break;
				}
				case rdo::runtime::RDOType::t_enum:
				{

					my_rssList[i].m_param[j].m_enum = param_it->second->getAsString().c_str();
					my_rssList[i].m_param[j].m_type = rdo::compiler::parser::RDOCorba::enum_type;

					break;
				}
				default: break;
			}

			++param_it;
			++j;
		}
	
		j = 0;
		++rss_it;
		++i;
	}

/*
	//! Пропарсели типы и ресурсы текста модели (текущие, а не записанные)
	rdo::compiler::parser::RDOParserCorba parser;
	try
	{
		parser.parse();
	}
	catch (REF(rdo::compiler::parser::RDOSyntaxException))
	{}
	catch (REF(rdo::runtime::RDORuntimeException))
	{}

	//! Пробежались по всем ресурсам и переписали в RSSList
	rdo::compiler::mbuilder::RDOResourceList rssList(&parser);
	rdo::compiler::mbuilder::RDOResourceList::List::const_iterator rss_it = rssList.begin();
	while (rss_it != rssList.end())
	{
		//! Создаем текстовую структуру
		RSS rss;
		rss.m_name = rss_it->name();
		//! Запоминаем в списке
		RSSList->push_back(rss);
		rss_it++;
	}
*/
}
#endif // CORBA_ENABLE

// --------------------------------------------------------------------------------
// -------------------- RDOThreadCodeComp
// --------------------------------------------------------------------------------
RDOThreadCodeComp::RDOThreadCodeComp()
	: RDOThreadMT("RDOThreadCodeComp")
	, m_pParser  (NULL               )
{
//	notifies.push_back(RT_CODECOMP_GET_DATA);
	after_constructor();
}

RDOThreadCodeComp::~RDOThreadCodeComp()
{}

void RDOThreadCodeComp::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_CODECOMP_GET_DATA:
		{
//			if (rdo::compiler::parser::m_pParser) m_pParser = rdo::compiler::parser::m_pParser;
			if (!m_pParser) break;
			msg.lock();
			PTR(GetCodeComp) data = static_cast<PTR(GetCodeComp)>(msg.param);
//			rdo::binarystream stream;
//			sendMessage(kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdo::repository::RDOThreadRepository::FileData(data->file, stream));
//			data->result = stream.data();
			CREF(rdo::compiler::parser::RDOParser::RTPResTypeList) rtp_list = m_pParser->getRTPResTypes(); 
			STL_FOR_ALL_CONST(rtp_list, rtp_it)
			{
				CREF(rdo::compiler::parser::RDORTPResType::ParamList) param_list = (*rtp_it)->getParams();
				rdo::compiler::parser::RDORTPResType::ParamList::const_iterator param_it = param_list.begin();
				while (param_it != param_list.end())
				{
					data->m_result += (*param_it)->name() + ' ';
					++param_it;
				}
			}
			msg.unlock();
//			if (m_pParser != rdo::compiler::parser::m_pParser) delete m_pParser;
			m_pParser = NULL;
			break;
		}
		default: break;
	}
}

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE
