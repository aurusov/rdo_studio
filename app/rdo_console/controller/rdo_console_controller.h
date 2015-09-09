#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
#include <boost/thread/mutex.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "simulator/report/src/file_message.h"
// --------------------------------------------------------------------------------

namespace rdo {

class ConsoleController: public RDOThread
{
public:
    ConsoleController();
    virtual ~ConsoleController();

    bool isFinished() const;
    bool isConverted() const;
    bool isSimulationSuccessfully();

    bool isBuildError() const;
    bool isRuntimeError() const;
    bool isConvertorError() const;

    void getBuildLog(std::list<std::string>& list) const;

private:
    typedef rdo::simulation::report::FileMessage FileMessage;

    enum class SimulatorState
    {
        UNDEFINED,
        IN_PROGRESS,
        FINISHED
    };

    SimulatorState state;
    mutable boost::mutex stateMutex;

    bool converted;
    bool buildError;
    bool runtimeError;
    bool convertorError;
    std::list<std::string> buildLog;

    void proc(RDOMessageInfo& msg);
    void appendToBuildLog(std::vector<FileMessage>& errors);
};

} // namespace rdo
