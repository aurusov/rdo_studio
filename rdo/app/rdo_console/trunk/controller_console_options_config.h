/*!
  \copyright (c) RDO-Team, 2011
  \file      app/rdo_console/controller_console_options_config.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
#define _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "build_version.h"
// --------------------------------------------------------------------------------

// Input param constant
const tstring MODEL_COMMAND                 = "input";
const tstring SCRIPT_COMMAND                = "script";
const tstring HELP_COMMAND                  = "help";
const tstring VERSION_COMMAND               = "version";
const tstring LANGUAGE_COMMAND              = "language";
const tstring CONVERTOR_COMMAND             = "convert";

const tstring AUTO_RUN_COMMAND              = "autorun";
const tstring AUTO_EXIT_COMMAND             = "autoexit";
const tstring DONT_CLOSE_IF_ERROR_COMMAND   = "dont_close_if_error";
const tstring PLUGIN_START_COMMAND          = "pluginstart";
const tstring PLUGIN_AUTO_EXIT_COMMAND      = "pluginautoexit";

const tstring MODEL_COMMAND_SHORT           = "i";
const tstring SCRIPT_COMMAND_SHORT          = "s";
const tstring HELP_COMMAND_SHORT            = "h";
const tstring VERSION_COMMAND_SHORT         = "v";
const tstring LANGUAGE_COMMAND_SHORT        = "l";
const tstring CONVERTOR_COMMAND_SHORT       = "c";

// Input param comment
const tstring MODEL_COMMENT                 = ": path to model file";
const tstring SCRIPT_COMMENT                = ": path to script file";
const tstring HELP_COMMENT                  = ": display help message";
const tstring VERSION_COMMENT               = ": display program version";
const tstring LANGUAGE_COMMENT              = ": display language version of rdo";

const tstring CONVERTOR_COMMENT             = ": run convertor smr to rdox";

const tstring AUTO_RUN_COMMENT              = ": autostart program";
const tstring AUTO_EXIT_COMMENT             = ": autoexit program";
const tstring DONT_CLOSE_IF_ERROR_COMMENT   = ": don't close if error";
const tstring PLUGIN_START_COMMENT          = ": start plugin";
const tstring PLUGIN_AUTO_EXIT_COMMENT      = ": plugin auto exit";

// Auxiliary constants
const tstring RDO_LANGUAGE_VERSION          = "2.0";

const tstring NOP_STRING   = " ";
const tstring COMMA_STRING = ",";

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
