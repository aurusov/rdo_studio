// ----------------------------------------------------------------------- INCLUDES
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem/fstream.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/locale/rdolocale.h"
#include "utils/src/file/rdofile.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/runtime/mode.h"
#include "simulator/service/src/simulator.h"
#include "app/rdo_console/events/rdo_event.h"
#include "app/rdo_console/events/rdo_key_event.h"
#include "app/rdo_console/events/rdo_mouse_event.h"
#include "app/rdo_console/events/xml_parser/rdo_event_xml_parser.h"
#include "app/rdo_console/events/xml_parser/rdo_key_event_xml_reader.h"
#include "app/rdo_console/events/xml_parser/rdo_mouse_event_xml_reader.h"
#include "app/rdo_console/controller/rdo_console_controller.h"
#include "app/rdo_console/options/controller_console_options.h"
#include "app/rdo_console/terminate_codes.h"
// --------------------------------------------------------------------------------

typedef std::list<std::string> string_list;
typedef rdo::event_xml_parser::event_container event_container;

const boost::filesystem::path LOG_FILE_NAME = "log.txt";

void read_events(std::istream& stream, event_container& container);
void write_build_log(std::ostream& stream, const string_list& list);
bool run(rdo::console_controller* pAppController, event_container& container);
void process_event(rdo::console_controller* pAppController, event_container& container);

int main(int argc, char* argv[])
{
	std::size_t exitCode = TERMINATION_NORMAL;

	rdo::locale::init();

	boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();

	rdo::ControllerConsoleOptions optionsController(argc, argv);
	optionsController.parseOptions();

	const boost::filesystem::path modelFileName = optionsController.getModelFileName();
	const bool modelExist = rdo::File::exist(modelFileName);

	const boost::filesystem::path eventsFileName = optionsController.getScriptFileName();
	const bool eventExist = rdo::File::exist(eventsFileName);

	if (optionsController.helpQuery())
	{
		exit(TERMINATION_NORMAL);
	}
	else if (!modelExist)
	{
		std::cerr << "Model file does not exist" << std::endl;
		exit(TERMINATION_WITH_AN_ERROR_NO_MODEL);
	}
	else if (!eventExist && !eventsFileName.empty())
	{
		std::cerr << "Events file does not exist" << std::endl;
		exit(TERMINATION_WITH_AN_ERROR_NO_EVENTS);
	}

	// read events
	event_container container;
	if (eventExist) {
		boost::filesystem::ifstream stream(eventsFileName, std::ios::out);
		read_events(stream, container);
	}

	// simulation
	RDOKernel::init();
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::repository::RDOThreadRepository();

	rdo::console_controller* pAppController = new rdo::console_controller();

	rdo::repository::RDOThreadRepository::OpenFile data(modelFileName);
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);

	if (optionsController.convertQuery())
	{
		bool converted = false;
		while (!converted)
		{
			converted = pAppController->converted();
			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
		}
		if (pAppController->convertorError())
		{
			exit(TERMINATION_WITH_AN_ERROR_CONVERTOR_ERROR);
		}
		exit(TERMINATION_NORMAL);
	}

	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);

	bool simulationSuccessfully = false;

	const bool buildError = pAppController->buildError();
	if (buildError)
	{
		string_list buildList;
		pAppController->getBuildLogList(buildList);

		const boost::filesystem::path fileName = modelFileName.parent_path() / LOG_FILE_NAME;
		boost::filesystem::remove(fileName);
		boost::filesystem::ofstream stream(fileName, std::ios::out);

		write_build_log(stream, buildList);

		std::cerr << "Build model error" << std::endl;
		exitCode = TERMINATION_WITH_AN_ERROR_PARSE_MODEL_ERROR;
	}
	else
	{
		simulationSuccessfully = run(pAppController, container);
		RDOKernel::close();
	}

	const boost::posix_time::ptime endTime = boost::posix_time::microsec_clock::local_time();
	const uint64_t simulationTimeMillisecond = (endTime - startTime).total_milliseconds();
	rdo::locale::cout(boost::str(boost::format("Total simulation time : %1% milliseconds") % simulationTimeMillisecond));

	if (simulationSuccessfully)
	{
		rdo::locale::cout("Simulation finished successfully");
		exitCode = TERMINATION_NORMAL;
	}
	else
	{
		rdo::locale::cout("Simulation completed with errors");
	}
	return exitCode;
}

void read_events(std::istream& stream, event_container& container)
{
	container.clear();

	if (stream.fail())
	{
		exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
	}
	rdo::event_xml_parser parser;
	parser.register_parser("key", boost::shared_ptr<rdo::event_xml_reader>(new rdo::key_event_xml_reader));
	parser.register_parser("mouse", boost::shared_ptr<rdo::event_xml_reader>(new rdo::mouse_event_xml_reader));

	try
	{
		parser.parse(stream, container);
	}
	catch (...)
	{
		exit(TERMINATION_WITH_AN_ERROR_PARSE_EVENTS_ERROR);
	}
}

void write_build_log(std::ostream& stream, const string_list& list)
{
	if (stream.fail())
	{
		exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
	}
	for (const string_list::value_type& line: list)
	{
		stream << line.c_str() << std::endl;
	}
}

bool run(rdo::console_controller* pAppController, event_container& container)
{
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN);

	rdo::runtime::RunTimeMode runtimeMode = rdo::runtime::RTM_MaxSpeed;
	pAppController->broadcastMessage(RDOThread::RT_RUNTIME_SET_MODE, &runtimeMode);

	while (!pAppController->finished())
	{
		kernel->idle();

		if (pAppController->runtimeError())
		{
			rdo::locale::cout("Run-time error");
			exit(TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR);
		}
		process_event(pAppController, container);
	}
	return pAppController->simulationSuccessfully();
}

void process_event(rdo::console_controller* pAppController, event_container& container)
{
	double runtime_time = 0;
	pAppController->broadcastMessage(RDOThread::RT_RUNTIME_GET_TIMENOW, &runtime_time);

	if(!container.empty())
	{
		event_container::const_iterator it = container.begin();
		if (it->first < runtime_time)
		{
			const std::string eventName = boost::str(boost::format("process event : name : %1%  |  time : %2%")
				% it->second->getName()
				% it->second->getTime()
			);
			rdo::locale::cout(eventName);

			rdo::event::types type = it->second->getType();

			switch (type)
			{
			case rdo::event::key:
				{
				std::size_t code = static_cast<rdo::key_event*>(it->second.get())->getKeyCode();
					rdo::key_event::states state = static_cast<rdo::key_event*>(it->second.get())->getState();

					rdo::console_controller::RDOTreadMessage message_type;
					switch (state)
					{
					case rdo::key_event::press:
						message_type = RDOThread::RT_RUNTIME_KEY_DOWN;
						break;
					case rdo::key_event::release:
						message_type = RDOThread::RT_RUNTIME_KEY_UP;
						break;
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
