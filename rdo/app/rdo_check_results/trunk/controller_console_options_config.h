/*!
  \copyright (c) RDO-Team, 2011
  \file      controller_console_options_config.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
#define _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

// Input param constant
const tstring ETALON_TRACE_COMMAND          = "etalon-trace";
const tstring ETALON_RESULT_COMMAND         = "etalon-result";
const tstring TRACE_COMMAND                 = "trace";
const tstring RESULT_COMMAND                = "result";
const tstring HELP_COMMAND                 = "help";
const tstring VERSION_COMMAND               = "version";

const tstring ETALON_TRACE_COMMAND_SHORT    = "T";
const tstring ETALON_RESULT_COMMAND_SHORT   = "R";
const tstring TRACE_COMMAND_SHORT           = "t";
const tstring RESULT_COMMAND_SHORT          = "r";
const tstring HELP_COMMAND_SHORT            = "h";
const tstring VERSION_COMMAND_SHORT         = "v";

// Input param comment
const tstring ETALON_TRACE_COMMENT          = ": path to the etalon trace";
const tstring ETALON_RESULT_COMMENT         = ": path to the etalon result";
const tstring TRACE_COMMENT                 = ": path to the trace";
const tstring RESULT_COMMENT                = ": path to the result";
const tstring HELP_COMMENT                  = ": display help message";
const tstring VERSION_COMMENT               = ": display program version";

// Auxiliary constants
const tstring PROGRAM_VERSION               = "0.5.0";
const tstring PROGRAM_NAME                  = "rdo check results";

const tstring NOP_STRING                    = " ";
const tstring COMMA_STRING                  = ",";

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
