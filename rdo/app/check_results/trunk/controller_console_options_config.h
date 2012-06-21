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
const tstring ETALON_TRACE_COMMAND          = _T("etalon-trace");
const tstring ETALON_RESULT_COMMAND         = _T("etalon-result");
const tstring TRACE_COMMAND                 = _T("trace");
const tstring RESULT_COMMAND                = _T("result");
const tstring CHELP_COMMAND                 = _T("help");
const tstring VERSION_COMMAND               = _T("version");

const tstring ETALON_TRACE_COMMAND_SHORT    = _T("T");
const tstring ETALON_RESULT_COMMAND_SHORT   = _T("R");
const tstring TRACE_COMMAND_SHORT           = _T("t");
const tstring RESULT_COMMAND_SHORT          = _T("r");
const tstring HELP_COMMAND_SHORT            = _T("h");
const tstring VERSION_COMMAND_SHORT         = _T("v");

// Input param comment
const tstring ETALON_TRACE_COMMENT          = _T(": path to the etalon trace");
const tstring ETALON_RESULT_COMMENT         = _T(": path to the etalon result");
const tstring TRACE_COMMENT                 = _T(": path to the trace");
const tstring RESULT_COMMENT                = _T(": path to the result");
const tstring HELP_COMMENT                  = _T(": display help message");
const tstring VERSION_COMMENT               = _T(": display program version");

// Auxiliary constants
const tstring PROGRAM_VERSION               = _T("0.5.0");
const tstring PROGRAM_NAME                  = _T("rdo check results");

const tstring NOP_STRING                    = _T(" ");
const tstring COMMA_STRING                  = _T(",");

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
