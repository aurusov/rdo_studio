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
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_console/terminate_codes.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/rdo_console_controller.h"
// --------------------------------------------------------------------------------

namespace fs = boost::filesystem;

const tstring LOG_FILE_NAME = _T("log.txt");

static ruint g_exitCode = TERMINATION_NORMAL;

int main(int argc, PTR(char) argv[])
{
	RDOControllerConsoleOptions options_controller(argc, argv);
	options_controller.parseOptions();

	tstring model_name;
	options_controller.getModelName(model_name);
	rbool model_exist = fs::exists(model_name);

	boost::filesystem::path path(model_name);
	tstring model_dir = path.parent_path().string();

	if (options_controller.helpQuery())
	{
		return TERMINATION_NORMAL;
	}
	else if (!model_exist && !options_controller.helpQuery())
	{
		std::cout << _T("Model does not exist") << std::endl;
		return TERMINATION_WITH_AN_ERROR_NO_MODEL;
	}

	// simulation

	RDOKernel::init();
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::repository::RDOThreadRepository();

	PTR(RDOStudioConsoleController) pAppController = new RDOStudioConsoleController();
	ASSERT(pAppController);

	rdo::repository::RDOThreadRepository::OpenFile data(model_name);
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD      );

	bool simulationSuccessfully = false;

	bool buildError = pAppController->buildError();
	if (buildError)
	{
		StringList buildList;
		pAppController->getBuildLogList(buildList);

		tstring fileName(model_dir + "/" + LOG_FILE_NAME);
		boost::filesystem::remove(fileName);
		std::ofstream stream(fileName.c_str(), std::ios::out);

		stream.clear();
		if (stream.fail()) {
			return TERMINATION_WITH_APP_RUNTIME_ERROR;
		}
		for (StringList::const_iterator it = buildList.begin(); it != buildList.end(); ++it)
		{
			stream << it->c_str() << std::endl;
		}
		stream.close();

		std::cout << _T("Build model error") << std::endl;
		g_exitCode = TERMINATION_WITH_AN_ERROR_PARSE_ERROR;
	}
	else
	{
		pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN);

		while (!pAppController->finished())
		{
			kernel->idle();

			if (pAppController->runtimeError())
			{
				std::cout << _T("Run-time error") << std::endl;
				g_exitCode = TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR;
			}
		}
		simulationSuccessfully = pAppController->simulationSuccessfully();

		RDOKernel::close();
	}
	if (simulationSuccessfully)
	{
		std::cout << _T("Simulation finished successfully") << std::endl;
		g_exitCode = TERMINATION_NORMAL;
	}
	else
	{
		std::cout << _T("Simulation completed with errors") << std::endl;
	}
	return g_exitCode;
}
