/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_studio_console_controller.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <boost/thread/mutex.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
// --------------------------------------------------------------------------------

class RDOStudioConsoleController: public RDOThread
{
public:
	RDOStudioConsoleController();
	virtual ~RDOStudioConsoleController();

	bool simulationFinished();

private:
	void proc(REF(RDOMessageInfo) msg);

private:
	boost::mutex m_simulation_mutex;
	rbool        m_simulation;
};
