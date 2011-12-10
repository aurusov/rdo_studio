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

int main(int argc, PTR(char) argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);

	options_controller.parseQuery();

	tstring model_name;
	options_controller.getModelName(model_name);
	
	if (!model_name.empty())
	{
		// Init
		RDOKernel::init();
		new rdoSimulator::RDOThreadSimulator();
		new rdoRepository::RDOThreadRepository();

		PTR(RDOStudioConsoleController) pAppController = new RDOStudioConsoleController();
		ASSERT(pAppController);

		rdoRepository::RDOThreadRepository::OpenFile data(model_name);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN  );

		while (!pAppController->finished())
		{
			kernel->idle();

			if (pAppController->errorOccurred())
			{
				std::cerr << _T("  run-time error  ") << std::endl;
				exit(1);
			}
		}

		bool simulationResult = pAppController->simulationSuccessfully();

		// Close
		RDOKernel::close();

		if (simulationResult)
		{
			std::cout << _T("  simulation finished successfully  ") << std::endl;
		}
		else
		{
			std::cout << _T("  simulation completed with errors  ") << std::endl;		
		}
	}
	return 0;
}
