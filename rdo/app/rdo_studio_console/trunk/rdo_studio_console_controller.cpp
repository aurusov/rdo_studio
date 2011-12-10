/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_studio_console_controller.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_console/rdo_studio_console_controller.h"
// --------------------------------------------------------------------------------

void test_fun()
{
	int i = 0;
	UNUSED(i);
}

RDOStudioConsoleController::RDOStudioConsoleController()
	: RDOThread(_T("RDOThreadRDOStudioConsoleController"))
	, m_state  (SS_UNDEFINED)
{
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_MODEL_STOP_AFTER          );
	notifies.push_back(RT_SIMULATOR_PARSE_OK                );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR         );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);

	after_constructor();
}

RDOStudioConsoleController::~RDOStudioConsoleController()
{
}

rbool RDOStudioConsoleController::finished() const
{
	rbool res = true;
	m_stateMutex.lock();
	res = m_state == SS_FINISHED;
	m_stateMutex.unlock();
	return res;
}

rbool RDOStudioConsoleController::errorOccurred() const
{
	/// @todo added errors controle
	return false;
}

rbool RDOStudioConsoleController::simulationSuccessfully() const
{
	bool res = false;
	m_stateMutex.lock();
	//! @todo запросить код возврата у симулятора (RT_SIMULATOR_GET_MODEL_EXITCODE)
	res = true;
	m_stateMutex.unlock();
	return res;
}

void RDOStudioConsoleController::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
	case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
		test_fun();
		break;

	case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		m_stateMutex.lock();
		m_state = SS_IN_PROGRESS;
		m_stateMutex.unlock();
		test_fun();
		break;

	case RDOThread::RT_RUNTIME_MODEL_STOP_AFTER:
		m_stateMutex.lock();
		m_state = SS_FINISHED;
		m_stateMutex.unlock();
		test_fun();
		break;

	case RDOThread::RT_SIMULATOR_PARSE_OK:
		test_fun();
		break;

	case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		test_fun();
		break;

	case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR:
		test_fun();
		break;

	case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY:
		test_fun();
		break;

	case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		test_fun();
		break;

	case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		test_fun();
		break;
	}
}
