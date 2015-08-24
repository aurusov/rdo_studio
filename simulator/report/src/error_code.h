#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

enum class ExitCode
{
    OK = 0,
    PARSER_ERROR,
    RUNTIME_ERROR,
    USER_BREAK,
    MODEL_NOTFOUND,
    NOMORE_EVENTS
};

}}} // namespace rdo::simulation::report
