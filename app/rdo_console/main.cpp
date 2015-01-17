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

typedef std::list<std::string> Strings;
typedef rdo::EventXmlParser::Events Events;

const boost::filesystem::path LOG_FILE_NAME = "log.txt";

void readEvents(std::istream& stream, Events& container);
void writeBuildLog(std::ostream& stream, const Strings& list);
bool run(rdo::ConsoleController* pAppController, Events& container);
void processEvent(rdo::ConsoleController* pAppController, Events& container);

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
    Events container;
    if (eventExist)
    {
        boost::filesystem::ifstream stream(eventsFileName, std::ios::out);
        readEvents(stream, container);
    }

    // simulation
    RDOKernel::init();
    new rdo::service::simulation::RDOThreadSimulator();
    new rdo::repository::RDOThreadRepository();

    rdo::ConsoleController* pAppController = new rdo::ConsoleController();

    rdo::repository::RDOThreadRepository::OpenFile data(modelFileName);
    pAppController->broadcastMessage(RDOThread::Message::STUDIO_MODEL_OPEN, &data);

    if (optionsController.convertQuery())
    {
        bool converted = false;
        while (!converted)
        {
            converted = pAppController->isConverted();
            boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        }

        if (pAppController->isConvertorError())
            exit(TERMINATION_WITH_AN_ERROR_CONVERTOR_ERROR);

        exit(TERMINATION_NORMAL);
    }

    pAppController->broadcastMessage(RDOThread::Message::STUDIO_MODEL_BUILD);

    bool simulationSuccessfully = false;

    const bool buildError = pAppController->isBuildError();
    if (buildError)
    {
        Strings buildList;
        pAppController->getBuildLog(buildList);

        const boost::filesystem::path fileName = modelFileName.parent_path() / LOG_FILE_NAME;
        boost::filesystem::remove(fileName);
        boost::filesystem::ofstream stream(fileName, std::ios::out);

        writeBuildLog(stream, buildList);

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

void readEvents(std::istream& stream, Events& container)
{
    container.clear();

    if (stream.fail())
        exit(TERMINATION_WITH_APP_RUNTIME_ERROR);

    rdo::EventXmlParser parser;
    parser.registerParser("key", std::make_shared<rdo::KeyEventXmlReader>());
    parser.registerParser("mouse", std::make_shared<rdo::MouseEventXmlReader>());

    try
    {
        parser.parse(stream, container);
    }
    catch (...)
    {
        exit(TERMINATION_WITH_AN_ERROR_PARSE_EVENTS_ERROR);
    }
}

void writeBuildLog(std::ostream& stream, const Strings& list)
{
    if (stream.fail())
        exit(TERMINATION_WITH_APP_RUNTIME_ERROR);

    for (const Strings::value_type& line: list)
        stream << line.c_str() << std::endl;
}

bool run(rdo::ConsoleController* pAppController, Events& container)
{
    pAppController->broadcastMessage(RDOThread::Message::STUDIO_MODEL_RUN);

    rdo::runtime::RunTimeMode runtimeMode = rdo::runtime::RunTimeMode::MAX_SPEED;
    pAppController->broadcastMessage(RDOThread::Message::RUNTIME_SET_MODE, &runtimeMode);

    while (!pAppController->isFinished())
    {
        kernel->idle();

        if (pAppController->isRuntimeError())
        {
            rdo::locale::cout("Run-time error");
            exit(TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR);
        }
        processEvent(pAppController, container);
    }
    return pAppController->isSimulationSuccessfully();
}

RDOThread::Message getMessageType(rdo::KeyEvent::State state)
{
    switch (state)
    {
    case rdo::KeyEvent::State::PRESS: return RDOThread::Message::RUNTIME_KEY_DOWN;
    case rdo::KeyEvent::State::RELEASE: return RDOThread::Message::RUNTIME_KEY_UP;
    }

    throw std::runtime_error("Unexpected event state");
}

void processEvent(rdo::ConsoleController* pAppController, Events& container)
{
    double runtime_time = 0;
    pAppController->broadcastMessage(RDOThread::Message::RUNTIME_GET_TIMENOW, &runtime_time);

    if(!container.empty())
    {
        Events::const_iterator it = container.begin();
        if (it->first < runtime_time)
        {
            const auto eventName = boost::str(boost::format("process event : name : %1%  |  time : %2%")
                % it->second->getName()
                % it->second->getTime());
            rdo::locale::cout(eventName);

            rdo::Event::Type type = it->second->getType();

            switch (type)
            {
            case rdo::Event::Type::KEY:
                {
                    std::size_t code = static_cast<rdo::KeyEvent*>(it->second.get())->getKeyCode();
                    const rdo::KeyEvent::State state = static_cast<rdo::KeyEvent*>(it->second.get())->getState();

                    const rdo::ConsoleController::Message message_type = getMessageType(state);
                    pAppController->broadcastMessage(message_type, &code);
                }
                break;

            case rdo::Event::Type::MOUSE:
                // TODO : complete me
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
