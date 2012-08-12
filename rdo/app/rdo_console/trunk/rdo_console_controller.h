/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_studio_console_controller.h
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
#include "utils/rdotypes.h"
#include "kernel/rdothread.h"
#include "simulator/report/error_code.h"
// --------------------------------------------------------------------------------

class RDOStudioConsoleController: public RDOThread
{
public:
	RDOStudioConsoleController();
	virtual ~RDOStudioConsoleController();

	rbool buildError            () const;
	rbool finished              () const;
	rbool runtimeError          () const;
	rbool simulationSuccessfully();

	void getBuildLogList(StringList& list) const;

private:
	typedef rdo::simulation::report::RDOSyntaxMessage RDOSyntaxMessage;
	typedef rdo::simulation::report::RDOExitCode      RDOExitCode;

private:
	void proc(REF(RDOMessageInfo) msg);
	void fillBuildLogList(std::vector<RDOSyntaxMessage>& errors);

private:
	enum SimulatorState
	{
		SS_UNDEFINED,
		SS_IN_PROGRESS,
		SS_FINISHED
	};

	SimulatorState       m_state;
	rbool                m_buildError; 
	rbool                m_runtimeError;
	RDOExitCode          m_exitCode;
	StringList         m_buildLogList;

	mutable boost::mutex m_stateMutex;
};

#endif // _RDO_STUDIO_CONSOLE_CONTROLLER_H_
