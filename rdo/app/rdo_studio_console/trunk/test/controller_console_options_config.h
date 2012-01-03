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
const tstring INPUT_MODEL_COMMAND           = _T("input");
const tstring CHELP_COMMAND                 = _T("help");
const tstring VERSION_COMMAND               = _T("version");

const tstring INPUT_MODEL_COMMAND_SHORT     = _T("i");
const tstring HELP_COMMAND_SHORT            = _T("h");
const tstring VERSION_COMMAND_SHORT         = _T("v");

// Input param comment
const tstring MODEL_COMMENT                 = _T(": path to test model");
const tstring HELP_COMMENT                  = _T(": display help message");
const tstring VERSION_COMMENT               = _T(": display program version");

// Auxiliary constants
const tstring PROGRAM_VERSION               = _T("0.3");
const tstring PROGRAM_NAME                  = _T("rdo test");

const tstring NOP_STRING                    = _T(" ");
const tstring COMMA_STRING                  = _T(",");

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
