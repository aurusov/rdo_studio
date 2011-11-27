/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_console/controller_console_options.h"
#include "app/rdo_studio_console/rdo_studio_console_controller.h"
// --------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);

	options_controller.parseQuery();

	tstring model_name;
	options_controller.getModelName(model_name);
	
	if(!model_name.empty())
	{
		// Init
		RDOKernel::init();
		new rdoSimulator::RDOThreadSimulator();
		new rdoRepository::RDOThreadRepository();

		RDOStudioConsoleController* app_controller = new RDOStudioConsoleController();

		rdoRepository::RDOThreadRepository::OpenFile data(model_name);
		app_controller->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
		app_controller->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);
		app_controller->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN  );

		while(app_controller->simulationFinished())
		{
			kernel->idle();
		}

		// Close
		RDOKernel::close();

		std::cout << "  simulation finished successfully  " << std::endl;
	}
	return 0;
}
