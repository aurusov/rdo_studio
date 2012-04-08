/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_console/terminate_codes.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/rdo_console_controller.h"
// --------------------------------------------------------------------------------

int main(int argc, PTR(char) argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);

	options_controller.parseOptions();

	tstring model_name;
	options_controller.getModelName(model_name);
	
	rbool model_exist = fs::exists(model_name);

	if (model_exist)
	{
		// Init
		RDOKernel::init();
		new rdo::service::simulation::RDOThreadSimulator();
		new rdo::repository::RDOThreadRepository();

		PTR(RDOStudioConsoleController) pAppController = new RDOStudioConsoleController();
		ASSERT(pAppController);

		rdo::repository::RDOThreadRepository::OpenFile data(model_name);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD      );
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN        );

		while (!pAppController->finished())
		{
			kernel->idle();

			if (pAppController->errorOccurred())
			{
				std::cerr << _T("Run-time error") << std::endl;
				exit(TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR);
			}
		}

		bool simulationResult = pAppController->simulationSuccessfully();

		// Close
		RDOKernel::close();

		if (simulationResult)
		{
			std::cout << _T("Simulation finished successfully") << std::endl;
		}
		else
		{
			std::cout << _T("Simulation completed with errors") << std::endl;		
		}
	}
	else if(!options_controller.helpQuery())
	{
		std::cout << _T("Model does not exist") << std::endl;
		return TERMINATION_WITH_AN_ERROR_NO_MODEL;
	}
	return TERMINATION_NORMAL;
}
