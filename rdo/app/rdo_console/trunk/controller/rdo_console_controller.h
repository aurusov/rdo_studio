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
#include "utils/src/common/rdotypes.h"
#include "kernel/rdothread.h"
#include "simulator/report/src/error_code.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {

class console_controller: public RDOThread
{
public:
	typedef  std::list<tstring>  StringList;

	console_controller();
	virtual ~console_controller();

	rbool finished              () const;
	rbool converted             () const;
	rbool simulationSuccessfully();

	rbool buildError            () const;
	rbool runtimeError          () const;
	rbool convertorError        () const;

	void getBuildLogList(StringList& list) const;

private:
	typedef rdo::simulation::report::FileMessage FileMessage;
	typedef rdo::simulation::report::RDOExitCode RDOExitCode;

private:
	void proc(REF(RDOMessageInfo) msg);
	void fillBuildLogList(REF(std::vector<FileMessage>) errors);

private:
	enum SimulatorState
	{
		SS_UNDEFINED,
		SS_IN_PROGRESS,
		SS_FINISHED
	};

	SimulatorState       m_state;
	rbool                m_converted;
	rbool                m_buildError;
	rbool                m_runtimeError;
	rbool                m_convertorError;
	RDOExitCode          m_exitCode;
	StringList           m_buildLogList;

	mutable boost::mutex m_stateMutex;
};

} // namespace rdo

#endif // _RDO_STUDIO_CONSOLE_CONTROLLER_H_
