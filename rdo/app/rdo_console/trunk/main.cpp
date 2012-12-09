/*!
  \copyright (c) RDO-Team, 2011
  \file     app/rdo_console/main.cpp
  \author   Пройдаков Евгений (lord.tiran@gmail.com)
  \date     26.10.2011
  \brief    Консольная версия RDO
  \indent   4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <iostream>

#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdolocale.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_console/rdo_event.h"
#include "app/rdo_console/rdo_event_xml_parser.h"
#include "app/rdo_console/rdo_key_event_xml_reader.h"
#include "app/rdo_console/rdo_mouse_event_xml_reader.h"
#include "app/rdo_console/rdo_console_controller.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/terminate_codes.h"
// --------------------------------------------------------------------------------

namespace fs = boost::filesystem;

typedef std::list<tstring> string_list;
typedef rdo::event_xml_parser::event_list event_list;

const tstring LOG_FILE_NAME = _T("log.txt");

static ruint g_exitCode = TERMINATION_NORMAL;

void read_events(REF(std::istream) stream, REF(event_list) list);
void write_build_log(REF(std::ostream) stream, CREF(string_list) list);

int main(int argc, PTR(char) argv[])
{
	rdo::setup_locale();

	boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();

	rdo::ControllerConsoleOptions options_controller(argc, argv);
	options_controller.parseOptions();

	tstring model_file_name = options_controller.getModelFileName();
	rbool model_exist = fs::exists(model_file_name);

	tstring events_file_name = options_controller.getEventsFileName();
	rbool event_exist = fs::exists(events_file_name);

	if (options_controller.helpQuery())
	{
		return TERMINATION_NORMAL;
	}
	else if (!model_exist)
	{
		std::cerr << _T("Model file does not exist") << std::endl;
		return TERMINATION_WITH_AN_ERROR_NO_MODEL;
	}
	else if (!event_exist && !events_file_name.empty())
	{
		std::cerr << _T("Events file does not exist") << std::endl;
		return TERMINATION_WITH_AN_ERROR_NO_EVENTS;
	}

	// read events
	event_list list;
	if(event_exist) {
		std::ifstream stream(events_file_name.c_str(), std::ios::out);
		read_events(stream, list);
		list.sort();
	}

	// simulation
	RDOKernel::init();
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::repository::RDOThreadRepository();

	boost::shared_ptr<rdo::StudioConsoleController> pAppController = boost::shared_ptr<rdo::StudioConsoleController>(new rdo::StudioConsoleController());
	ASSERT(pAppController.get());

	rdo::repository::RDOThreadRepository::OpenFile data(model_file_name);
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);

	if(options_controller.convertQuery())
	{
		bool converted = false;
		while(!converted)
		{
			converted = pAppController->converted();
			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
		}
		if(pAppController->convertorError())
		{
			exit(TERMINATION_WITH_AN_ERROR_CONVERTOR_ERROR);
		}
		exit(TERMINATION_NORMAL);
	}

	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);

	bool simulationSuccessfully = false;

	boost::filesystem::path path(model_file_name);
	tstring model_dir = path.parent_path().string();

	bool buildError = pAppController->buildError();
	if (buildError)
	{
		string_list buildList;
		pAppController->getBuildLogList(buildList);

		tstring fileName(model_dir + "/" + LOG_FILE_NAME);
		boost::filesystem::remove(fileName);
		std::ofstream stream(fileName.c_str(), std::ios::out);

			write_build_log(stream, buildList);

		std::cerr << _T("Build model error") << std::endl;
		g_exitCode = TERMINATION_WITH_AN_ERROR_PARSE_MODEL_ERROR;
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

			if(!list.empty())
			{
				double runtime_time = 0;
				pAppController->broadcastMessage(RDOThread::RT_RUNTIME_GET_TIMENOW, &runtime_time);

				event_list::iterator it = list.begin();
				if((*it)->getTime() < runtime_time)
				{
					std::cout << "push event : " << "time : " << (*it)->getTime() << "  |  name : " << (*it)->getName() << std::endl;
					list.pop_front();

					ruint code = VK_SPACE;

					pAppController->broadcastMessage(RDOThread::RT_RUNTIME_KEY_DOWN, &code);
					pAppController->broadcastMessage(RDOThread::RT_RUNTIME_KEY_UP,   &code);
					pAppController->broadcastMessage(RDOThread::RT_RUNTIME_KEY_DOWN, &code);
					pAppController->broadcastMessage(RDOThread::RT_RUNTIME_KEY_UP,   &code);
				}
			}
		}
		simulationSuccessfully = pAppController->simulationSuccessfully();

		RDOKernel::close();
	}

	boost::posix_time::ptime endTime = boost::posix_time::microsec_clock::local_time();

	ruint64 simulationTimeMillisecond = ( endTime - startTime ).total_milliseconds();

	std::cout << "Total simulation time : " << simulationTimeMillisecond << " milliseconds" << std::endl;

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

void read_events(REF(std::istream) stream, REF(event_list) list)
{
	list.clear();

	if (stream.fail()) {
		exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
	}
	rdo::event_xml_parser parser;
	parser.register_parser("key", boost::shared_ptr<rdo::event_xml_reader>(new rdo::key_event_xml_reader));
	parser.register_parser("mouse", boost::shared_ptr<rdo::event_xml_reader>(new rdo::mouse_event_xml_reader));

	try
	{
		parser.parse(stream, list);
	}
	catch(...)
	{
		exit(TERMINATION_WITH_AN_ERROR_PARSE_EVENTS_ERROR);
	}
}

void write_build_log(REF(std::ostream) stream, CREF(string_list) list)
{
	if (stream.fail())
	{
		exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
	}
	BOOST_FOREACH( string_list::value_type const& line, list )
	{
		stream << line.c_str() << std::endl;
	}
}
