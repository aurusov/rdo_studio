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

	rbool simulationFinished();

private:
	boost::optional<rbool>  m_simulation;
	boost::mutex            m_simulationMutex;

	void proc(REF(RDOMessageInfo) msg);
};

#endif // _RDO_STUDIO_CONSOLE_CONTROLLER_H_
