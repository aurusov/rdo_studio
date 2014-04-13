/*!
  \copyright (c) RDO-Team, 2011
  \file      controller_console_options_config.h
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
const std::string MODEL_COMMAND                 = "input";
const std::string SCRIPT_COMMAND                = "script";
const std::string HELP_COMMAND                  = "help";
const std::string VERSION_COMMAND               = "version";
const std::string LANGUAGE_COMMAND              = "language";
const std::string CONVERTOR_COMMAND             = "convert";

const std::string AUTO_RUN_COMMAND              = "autorun";
const std::string AUTO_EXIT_COMMAND             = "autoexit";
const std::string DONT_CLOSE_IF_ERROR_COMMAND   = "dont_close_if_error";
const std::string PLUGIN_START_COMMAND          = "pluginstart";
const std::string PLUGIN_AUTO_EXIT_COMMAND      = "pluginautoexit";

const std::string MODEL_COMMAND_SHORT           = "i";
const std::string SCRIPT_COMMAND_SHORT          = "s";
const std::string HELP_COMMAND_SHORT            = "h";
const std::string VERSION_COMMAND_SHORT         = "v";
const std::string LANGUAGE_COMMAND_SHORT        = "l";
const std::string CONVERTOR_COMMAND_SHORT       = "c";

// Input param comment
const std::string MODEL_COMMENT                 = ": path to model file";
const std::string SCRIPT_COMMENT                = ": path to script file";
const std::string HELP_COMMENT                  = ": display help message";
const std::string VERSION_COMMENT               = ": display program version";
const std::string LANGUAGE_COMMENT              = ": display language version of rdo";

const std::string CONVERTOR_COMMENT             = ": run convertor smr to rdox";

const std::string AUTO_RUN_COMMENT              = ": autostart program";
const std::string AUTO_EXIT_COMMENT             = ": autoexit program";
const std::string DONT_CLOSE_IF_ERROR_COMMENT   = ": don't close if error";
const std::string PLUGIN_START_COMMENT          = ": start plugin";
const std::string PLUGIN_AUTO_EXIT_COMMENT      = ": plugin auto exit";

// Auxiliary constants
const std::string RDO_LANGUAGE_VERSION          = "2.0";

const std::string NOP_STRING   = " ";
const std::string COMMA_STRING = ",";

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
