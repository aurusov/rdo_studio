/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdolocale.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_console/rdo_event.h"
#include "app/rdo_console/terminate_codes.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/rdo_console_controller.h"
// --------------------------------------------------------------------------------

typedef std::list<tstring> StringList;
namespace fs = boost::filesystem;

const tstring LOG_FILE_NAME = _T("log.txt");

static ruint g_exitCode = TERMINATION_NORMAL;

void read_events(std::istream& stream, rdo::event_list& list);
void write_build_log(std::ostream& stream, const StringList& list);

int main(int argc, PTR(char) argv[])
{
	rdo::setup_locale();

    boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();

	RDOControllerConsoleOptions options_controller(argc, argv);
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
    std::list<rdo::event> event_list;
    if(event_exist) {
        std::ifstream stream(events_file_name.c_str(), std::ios::out);
        read_events(stream, event_list);
        event_list.sort();
    }

	// simulation
	RDOKernel::init();
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::repository::RDOThreadRepository();

	PTR(RDOStudioConsoleController) pAppController = new RDOStudioConsoleController();
	ASSERT(pAppController);

	rdo::repository::RDOThreadRepository::OpenFile data(model_file_name);
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
	pAppController->broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD      );

	bool simulationSuccessfully = false;

    boost::filesystem::path path(model_file_name);
    tstring model_dir = path.parent_path().string();

	bool buildError = pAppController->buildError();
	if (buildError)
	{
		StringList buildList;
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

            if(!event_list.empty())
            {
                double runtime_time = 0;
                pAppController->broadcastMessage(RDOThread::RT_RUNTIME_GET_TIMENOW, &runtime_time);

                rdo::event_list::iterator it = event_list.begin();
                if(it->time < runtime_time)
                {
                    std::cout << "push event : " << "time : " << it->time << " name : " << it->name << std::endl;
                    event_list.pop_front();

                    for(int i = 0; i < 1000; i++)
                    {
                        ruint code = VK_SPACE;

                        pAppController->broadcastMessage(RDOThread::RT_RUNTIME_KEY_DOWN, &code);
                        pAppController->broadcastMessage(RDOThread::RT_RUNTIME_KEY_UP, &code);
                    }
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

void read_events(std::istream& stream, rdo::event_list& list)
{
    list.clear();

    if (stream.fail()) {
        exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
    }
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(stream, pt);
    
    try {
        BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, pt.get_child("events") )
        {
            rdo::event event;
            event.name = v.second.get<tstring>("<xmlattr>.name", "");
            tstring text_type = v.second.get<tstring>("type", rdo::event::any_text);
            event.type = rdo::event::text_type2type(text_type);
            event.time = v.second.get<double>("time");
            list.push_back(event);
        }
    }
    catch(...) {
        exit(TERMINATION_WITH_AN_ERROR_PARSE_EVENTS_ERROR);
    }
}

void write_build_log(std::ostream& stream, const StringList& list)
{
    if (stream.fail()) {
        exit(TERMINATION_WITH_APP_RUNTIME_ERROR);
    }
    BOOST_FOREACH( StringList::value_type const& line, list )
    {
        stream << line.c_str() << std::endl;
    }
}
