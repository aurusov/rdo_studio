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
	: RDOThread(_T("RDOThreadRDOStudioConsoleController")),
	m_state(program_start)
{
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN             );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_GET_NAME    );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE            );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE  );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE_ERROR      );
	notifies.push_back(RT_REPOSITORY_MODEL_SAVE             );
	notifies.push_back(RT_SIMULATOR_PARSE_STRING            );
	notifies.push_back(RT_SIMULATOR_PARSE_OK                );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_MODEL_START_AFTER         );
	notifies.push_back(RT_RUNTIME_MODEL_STOP_BEFORE         );
	notifies.push_back(RT_DEBUG_STRING                      );
	notifies.push_back(RT_RESULT_STRING                     );
	
	after_constructor();
}

RDOStudioConsoleController::~RDOStudioConsoleController()
{

}

rbool RDOStudioConsoleController::inProgress()
{
	rbool res = true;
	m_simulationMutex.lock();
	if(m_state != program_start)
	{
		res = m_state == running_emulation;
	}
	m_simulationMutex.unlock();
	return res;
}

rbool RDOStudioConsoleController::errorOccurred()
{
	/// @todo added errors controle
	return false;
}

rbool RDOStudioConsoleController::simulationSuccessfully()
{
	bool res = false;
	m_simulationMutex.lock();
	res = m_state == simulation_completed_successfully;
	m_simulationMutex.unlock();
	return res;
}

void RDOStudioConsoleController::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
	case RDOThread::RT_REPOSITORY_MODEL_OPEN:
		test_fun();
		break;
		
	case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
		test_fun();
		break;
		
	case RDOThread::RT_REPOSITORY_MODEL_SAVE:
		test_fun();
		break;
		
	case RDOThread::RT_REPOSITORY_MODEL_OPEN_GET_NAME:
		test_fun();
		break;
		
	case RDOThread::RT_REPOSITORY_MODEL_CLOSE:
		test_fun();
		break;
		
	case RDOThread::RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE:
		test_fun();
		break;
		
	case RDOThread::RT_REPOSITORY_MODEL_CLOSE_ERROR:
		test_fun();
		break;
		
	case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		test_fun();
		break;
		
	case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
		test_fun();
		break;
		
	case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE:
		m_simulationMutex.lock();
		m_state = running_emulation;
		m_simulationMutex.unlock();
		test_fun();
		break;
		
	case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		m_simulationMutex.lock();
		m_state = simulation_completed_successfully;
		m_simulationMutex.unlock();
		test_fun();
		break;
		
	case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		test_fun();
		break;
		
	case RDOThread::RT_SIMULATOR_PARSE_OK:
		test_fun();
		break;
		
	case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		test_fun();
		break;
		
	case RDOThread::RT_SIMULATOR_PARSE_STRING:
		test_fun();
		break;
		
	case RDOThread::RT_DEBUG_STRING:
		test_fun();
		break;
		
	case RDOThread::RT_RESULT_STRING:
		test_fun();
		break;
	}
}
