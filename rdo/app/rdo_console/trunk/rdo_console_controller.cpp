/*!
  \copyright (c) RDO-Team, 2011
  \file      app/rdo_console/rdo_console_controller.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <boost/thread/locks.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "simulator/report/build_edit_line_info.h"
#include "app/rdo_console/rdo_console_controller.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;

#define MUTEXT_PROTECTION(A) boost::lock_guard<boost::mutex> lg_##__LINE__(A);

RDOStudioConsoleController::RDOStudioConsoleController()
	: RDOThread(_T("RDOThreadRDOStudioConsoleController"))
	, m_state(SS_UNDEFINED)
	, m_buildError(false)
	, m_runtimeError(false)
	, m_exitCode(rdo::simulation::report::EC_OK)
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
	{
		MUTEXT_PROTECTION(m_stateMutex);
		res = (m_state == SS_FINISHED);
	}
	return res;
}

rbool RDOStudioConsoleController::buildError () const
{
	return m_buildError;
}

rbool RDOStudioConsoleController::runtimeError() const
{
	return m_runtimeError;
}

rbool RDOStudioConsoleController::simulationSuccessfully()
{
	sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
	return m_exitCode == rdo::simulation::report::EC_OK;
}

void RDOStudioConsoleController::getBuildLogList(StringList& list) const
{
	list = m_buildLogList;
}

void RDOStudioConsoleController::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
	case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
		break;

	case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		{
			MUTEXT_PROTECTION(m_stateMutex);
			m_state = SS_IN_PROGRESS;
		}
		break;

	case RDOThread::RT_RUNTIME_MODEL_STOP_AFTER:
		{
			MUTEXT_PROTECTION(m_stateMutex);
			m_state = SS_FINISHED;
		}
		break;

	case RDOThread::RT_SIMULATOR_PARSE_OK:
		break;

	case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		{
			m_buildError = true;
			std::vector<FileMessage> errors;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_ERRORS, &errors);
			fillBuildLogList(errors);
		}
		break;

	case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR:
		break;

	case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY:
		break;

	case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		break;

	case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		m_runtimeError = true;
		break;
	}
}

void RDOStudioConsoleController::fillBuildLogList(REF(std::vector<FileMessage>) errors)
{
	STL_FOR_ALL_CONST(errors, it)
	{
		BuildEditLineInfo info(*it);
		tstring line = info.getMessage();
		m_buildLogList.push_back(line);
	}
}