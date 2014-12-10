// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <boost/thread/locks.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdokernel.h"
#include "simulator/service/src/simulator.h"
#include "simulator/report/src/build_edit_line_info.h"
#include "simulator/report/src/error_code.h"
#include "app/rdo_console/controller/rdo_console_controller.h"
// --------------------------------------------------------------------------------

using namespace rdo;
using namespace rdo::simulation::report;

#define MUTEXT_PROTECTION(A) boost::lock_guard<boost::mutex> lg_##__LINE__(A);

ConsoleController::ConsoleController()
    : RDOThread("RDOThreadStudioConsoleController")
    , state(SimulatorState::UNDEFINED)
    , converted(false)
    , buildError(false)
    , runtimeError(false)
    , convertorError(false)
{
    notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
    notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        ); // TODO : wait
    notifies.push_back(RT_RUNTIME_MODEL_STOP_AFTER          );
    notifies.push_back(RT_SIMULATOR_PARSE_OK                );
    notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
    notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR         );
    notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   );
    notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
    notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
    notifies.push_back(RT_CONVERTOR_NONE                    );
    notifies.push_back(RT_CONVERTOR_OK                      );
    notifies.push_back(RT_CONVERTOR_ERROR                   );

    after_constructor();
}

ConsoleController::~ConsoleController()
{}

bool ConsoleController::isFinished() const
{
    bool res = true;
    {
        MUTEXT_PROTECTION(stateMutex);
        res = (state == SimulatorState::FINISHED);
    }
    return res;
}

bool ConsoleController::isConverted() const
{
    return converted;
}

bool ConsoleController::isSimulationSuccessfully()
{
    rdo::simulation::report::RDOExitCode exitCode;
    sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &exitCode);
    return exitCode == rdo::simulation::report::EC_OK;
}

bool ConsoleController::isBuildError() const
{
    return buildError;
}

bool ConsoleController::isRuntimeError() const
{
    return runtimeError;
}

bool ConsoleController::isConvertorError() const
{
    return convertorError;
}

void ConsoleController::getBuildLog(std::list<std::string>& list) const
{
    list = buildLog;
}

void ConsoleController::proc(RDOThread::RDOMessageInfo& msg)
{
    switch (msg.message)
    {
    case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
        break;

    case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
        {
            MUTEXT_PROTECTION(stateMutex);
            state = SimulatorState::IN_PROGRESS;
        }
        break;

    case RDOThread::RT_RUNTIME_MODEL_STOP_AFTER:
        {
            MUTEXT_PROTECTION(stateMutex);
            state = SimulatorState::FINISHED;
        }
        break;

    case RDOThread::RT_SIMULATOR_PARSE_OK:
        break;

    case RDOThread::RT_SIMULATOR_PARSE_ERROR:
        {
            buildError = true;
            std::vector<FileMessage> errors;
            sendMessage(kernel->simulator(), RT_SIMULATOR_GET_ERRORS, &errors);
            appendToBuildLog(errors);
        }
        break;

    case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR:
        break;

    case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY:
        break;

    case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
        break;

    case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
        runtimeError = true;
        break;

    case RDOThread::RT_CONVERTOR_NONE:
        converted = true;
        convertorError = false;
        break;

    case RDOThread::RT_CONVERTOR_OK:
        converted = true;
        convertorError = false;
        break;

    case RDOThread::RT_CONVERTOR_ERROR:
        converted = true;
        convertorError = true;
        break;

    default:
        break;
    }
}

void ConsoleController::appendToBuildLog(std::vector<FileMessage>& errors)
{
    for (const FileMessage& error: errors)
    {
        const BuildEditLineInfo info(error);
        const std::string line = info.getMessage();
        buildLog.push_back(line);
    }
}
