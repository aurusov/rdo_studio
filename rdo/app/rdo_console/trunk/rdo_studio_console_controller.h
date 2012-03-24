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
#include "kernel/rdothread.h"
// --------------------------------------------------------------------------------

class RDOStudioConsoleController: public RDOThread
{
public:
	RDOStudioConsoleController();
	virtual ~RDOStudioConsoleController();

	rbool finished              () const;
	rbool errorOccurred         () const;
	rbool simulationSuccessfully() const;

private:
	enum SimulatorState
	{
		SS_UNDEFINED,
		SS_IN_PROGRESS,
		SS_FINISHED
	};

	SimulatorState        m_state;
	mutable boost::mutex  m_stateMutex;

	void proc(REF(RDOMessageInfo) msg);
};

#endif // _RDO_STUDIO_CONSOLE_CONTROLLER_H_
