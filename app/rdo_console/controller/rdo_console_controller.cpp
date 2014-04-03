/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_console_controller.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <boost/thread/locks.hpp>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "simulator/service/src/simulator.h"
#include "simulator/report/src/build_edit_line_info.h"
#include "app/rdo_console/controller/rdo_console_controller.h"
// --------------------------------------------------------------------------------

using namespace rdo;
using namespace rdo::simulation::report;

#define MUTEXT_PROTECTION(A) boost::lock_guard<boost::mutex> lg_##__LINE__(A);

console_controller::console_controller()
	: RDOThread("RDOThreadStudioConsoleController")
	, m_state(SS_UNDEFINED)
	, m_converted(false)
	, m_buildError(false)
	, m_runtimeError(false)
	, m_convertorError(false)
	, m_exitCode(rdo::simulation::report::EC_OK)
{
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        ); /// @todo : wait
	notifies.push_back(RT_RUNTIME_MODEL_STOP_AFTER          );
	notifies.push_back(RT_SIMULATOR_PARSE_OK                );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR         );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_CONVERTOR_NONE                    );
	notifies.push_back(RT_CONVERTOR_OK                      );
	notifies.push_back(RT_CONVERTOR_ERROR                   );

	after_constructor();
}

console_controller::~console_controller()
{}

rbool console_controller::finished() const
{
	rbool res = true;
	{
		MUTEXT_PROTECTION(m_stateMutex);
		res = (m_state == SS_FINISHED);
	}
	return res;
}

rbool console_controller::converted() const
{
	return m_converted;
}

rbool console_controller::simulationSuccessfully()
{
	sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
	return m_exitCode == rdo::simulation::report::EC_OK;
}

rbool console_controller::buildError() const
{
	return m_buildError;
}

rbool console_controller::runtimeError() const
{
	return m_runtimeError;
}

rbool console_controller::convertorError() const
{
	return m_convertorError;
}

void console_controller::getBuildLogList(StringList& list) const
{
	list = m_buildLogList;
}

void console_controller::proc(REF(RDOThread::RDOMessageInfo) msg)
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

	case RDOThread::RT_CONVERTOR_NONE:
		m_converted = true;
		m_convertorError = false;
		break;

	case RDOThread::RT_CONVERTOR_OK:
		m_converted = true;
		m_convertorError = false;
		break;

	case RDOThread::RT_CONVERTOR_ERROR:
		m_converted = true;
		m_convertorError = true;
		break;

	default:
		break;
	}
}

void console_controller::fillBuildLogList(REF(std::vector<FileMessage>) errors)
{
	BOOST_FOREACH(const FileMessage& error, errors)
	{
		const BuildEditLineInfo info(error);
		const tstring line = info.getMessage();
		m_buildLogList.push_back(line);
	}
}
