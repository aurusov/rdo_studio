/*!
  \copyright (c) RDO-Team, 2011
  \file	    app/rdo_console/main.cpp
  \author   Пройдаков Евгений (lord.tiran@gmail.com)
  \date	    26.10.2011
  \brief	Консольная версия RDO
  \indent   4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <iostream>

#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdolocale.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_console/rdo_event.h"
#include "app/rdo_console/rdo_key_event.h"
#include "app/rdo_console/rdo_mouse_event.h"
#include "app/rdo_console/rdo_event_xml_parser.h"
#include "app/rdo_console/rdo_key_event_xml_reader.h"
#include "app/rdo_console/rdo_mouse_event_xml_reader.h"
#include "app/rdo_console/rdo_console_controller.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/terminate_codes.h"
// --------------------------------------------------------------------------------

namespace fs = boost::filesystem;

typedef std::list<tstring> string_list;
typedef rdo::event_xml_parser::event_container event_container;

const tstring LOG_FILE_NAME = "log.txt";

void read_events(REF(std::istream) stream, REF(event_container) container);
void write_build_log(REF(std::ostream) stream, CREF(string_list) list);
bool run(PTR(rdo::console_controller) pAppController, REF(event_container) container);
void process_event(PTR(rdo::console_controller) pAppController, REF(event_container) container);

int main(int argc, PTR(char) argv[])
{
	ruint exitCode = TERMINATION_NORMAL;

	rdo::setup_locale();

	boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();

	rdo::ControllerConsoleOptions options_controller(argc, argv);
	options_controller.parseOptions();

	tstring model_file_name = options_controller.getModelFileName();
	rbool model_exist = fs::exists(model_file_name);

	tstring events_file_name = options_controller.getScriptFileName();
	rbool event_exist = fs::exists(events_file_name);

	if (options_controller.helpQuery())
	{
		exit(TERMINATION_NORMAL);
	}
	else if (!model_exist)
	{
		std::cerr << "Model file does not exist" << std::endl;
		exit(TERMINATION_WITH_AN_ERROR_NO_MODEL);
	}
	else if (!event_exist && !events_file_name.empty())
	{
		std::cerr << "Events file does not exist" << std::endl;
		exit(TERMINATION_WITH_AN_ERROR_NO_EVENTS);
	}

	// read events
	event_container container;
	if(event_exist) {
		std::ifstream stream(events_file_name.c_str(), std::ios::out);
		read_events(stream, container);
	}

	// simulation
	RDOKernel::init();
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::repository::RDOThreadRepository();

	rdo::console_controller* pAppController = new rdo::console_controller();

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

		std::cerr << "Build model error" << std::endl;
		exitCode = TERMINATION_WITH_AN_ERROR_PARSE_MODEL_ERROR;
	}
	else
	{
		simulationSuccessfully = run(pAppController, container);
		RDOKernel::close();
	}

	boost::posix_time::ptime endTime = boost::posix_time::microsec_clock::local_time();
	ruint64 simulationTimeMillisecond = ( endTime - startTime ).total_milliseconds();
	std::cout << "Total simulation time : " << simulationTimeMillisecond << " milliseconds" << std::endl;

	if (simulationSuccessfully)
	{
		std::cout << "Simulation finished successfully" << std::endl;
		exitCode = TERMINATION_NORMAL;
	}
	else
	{
		std::cout << "Simulation completed with errors" << std::endl;
	}
	return exitCode;
}

void read_events(REF(std::istream) stream, REF(event_container) container)
{
	container.clear();

	if (stream.fail()) {
		exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
	}
	rdo::event_xml_parser parser;
	parser.register_parser("key", boost::shared_ptr<rdo::event_xml_reader>(new rdo::key_event_xml_reader));
	parser.register_parser("mouse", boost::shared_ptr<rdo::event_xml_reader>(new rdo::mouse_event_xml_reader));

	try
	{
		parser.parse(stream, container);
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

bool run(PTR(rdo::console_controller) pAppController, REF(event_container) container)
{
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN);

	while (!pAppController->finished())
	{
		kernel->idle();

		if (pAppController->runtimeError())
		{
			std::cout << "Run-time error" << std::endl;
			exit(TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR);
		}
        process_event(pAppController, container);
	}
	return pAppController->simulationSuccessfully();
}

void process_event(PTR(rdo::console_controller) pAppController, REF(event_container) container)
{
	double runtime_time = 0;
	pAppController->broadcastMessage(RDOThread::RT_RUNTIME_GET_TIMENOW, &runtime_time);

	if(!container.empty())
	{
		event_container::iterator it = container.begin();
		if(it->first < runtime_time)
		{
            std::cout << "process event : " << "name : " << it->second->getName() << "  |  " << "time : " << it->second->getTime() << std::endl;

			rdo::event::types type = it->second->getType();

			switch (type) {
			case rdo::event::key:
			{
				ruint code = static_cast<rdo::key_event*>(it->second.get())->getKeyCode();
				rdo::key_event::states state = static_cast<rdo::key_event*>(it->second.get())->getState();

				rdo::console_controller::RDOTreadMessage message_type;
				if(state == rdo::key_event::press) {
					message_type = RDOThread::RT_RUNTIME_KEY_DOWN;
				}
				else if(state == rdo::key_event::release) {
					message_type = RDOThread::RT_RUNTIME_KEY_UP;
				}
				else {
					NEVER_REACH_HERE;
				}
				pAppController->broadcastMessage(message_type, &code);
			}
				break;

			case rdo::event::mouse:
				/// @todo : complete me
				NEVER_REACH_HERE;
				break;

			default:
				NEVER_REACH_HERE;
				break;
			}
			container.erase(it);
		}
	}
}
