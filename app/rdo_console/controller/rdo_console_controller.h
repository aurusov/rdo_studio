/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_console_controller.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_STUDIO_CONSOLE_CONTROLLER_H_
#define _RDO_STUDIO_CONSOLE_CONTROLLER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
#include <boost/thread/mutex.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/report/src/error_code.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {

class console_controller: public RDOThread
{
public:
	typedef  std::list<std::string>  StringList;

	console_controller();
	virtual ~console_controller();

	bool finished() const;
	bool converted() const;
	bool simulationSuccessfully();

	bool buildError() const;
	bool runtimeError() const;
	bool convertorError() const;

	void getBuildLogList(StringList& list) const;

private:
	typedef rdo::simulation::report::FileMessage FileMessage;
	typedef rdo::simulation::report::RDOExitCode RDOExitCode;

private:
	void proc(RDOMessageInfo& msg);
	void fillBuildLogList(std::vector<FileMessage>& errors);

private:
	enum SimulatorState
	{
		SS_UNDEFINED,
		SS_IN_PROGRESS,
		SS_FINISHED
	};

	SimulatorState m_state;
	bool m_converted;
	bool m_buildError;
	bool m_runtimeError;
	bool m_convertorError;
	RDOExitCode m_exitCode;
	StringList m_buildLogList;

	mutable boost::mutex m_stateMutex;
};

} // namespace rdo

#endif // _RDO_STUDIO_CONSOLE_CONTROLLER_H_
