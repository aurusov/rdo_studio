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
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

#ifdef OST_WINDOWS
const tstring SYSTEM_OS = _T("Windows");
#elif defined( OST_LINUX )
const tstring SYSTEM_OS = _T("Linux");
#endif // OST_WINDOWS

#ifdef ARCHITECTURES_AMD64
const tstring SYSTEM_ARCHITECTURES = _T("AMD64");
#elif defined( ARCHITECTURES_X86 )
const tstring SYSTEM_ARCHITECTURES = _T("x86");
#endif // ARCHITECTURES_AMD64

// Input param constant
const tstring MODEL_COMMAND                 = _T("input");
const tstring EVENTS_COMMAND                = _T("events");
const tstring HELP_COMMAND                  = _T("help");
const tstring VERSION_COMMAND               = _T("version");
const tstring LANGUAGE_COMMAND              = _T("language");

const tstring AUTO_RUN_COMMAND              = _T("autorun");
const tstring AUTO_EXIT_COMMAND             = _T("autoexit");
const tstring DONT_CLOSE_IF_ERROR_COMMAND   = _T("dont_close_if_error");
const tstring PLUGIN_START_COMMAND          = _T("pluginstart");
const tstring PLUGIN_AUTO_EXIT_COMMAND      = _T("pluginautoexit");

const tstring MODEL_COMMAND_SHORT           = _T("i");
const tstring EVENTS_COMMAND_SHORT          = _T("e");
const tstring HELP_COMMAND_SHORT            = _T("h");
const tstring VERSION_COMMAND_SHORT         = _T("v");
const tstring LANGUAGE_COMMAND_SHORT        = _T("l");

// Input param comment
const tstring MODEL_COMMENT                 = _T(": path to model file");
const tstring EVENTS_COMMENT                = _T(": path to events file");
const tstring HELP_COMMENT                  = _T(": display help message");
const tstring VERSION_COMMENT               = _T(": display program version");
const tstring LANGUAGE_COMMENT              = _T(": display language version of rdo");

const tstring AUTO_RUN_COMMENT              = _T(": autostart program");
const tstring AUTO_EXIT_COMMENT             = _T(": autoexit program");
const tstring DONT_CLOSE_IF_ERROR_COMMENT   = _T(": don't close if error");
const tstring PLUGIN_START_COMMENT          = _T(": start plugin");
const tstring PLUGIN_AUTO_EXIT_COMMENT      = _T(": plugin auto exit");

// Auxiliary constants
const tstring PROGRAM_VERSION               = _T("1.0");
const tstring RDO_LANGUAGE_VERSION          = _T("2.0");

const tstring RDO_SITE                      = _T("( http://rdo.rk9.bmstu.ru )");
const tstring PROGRAM_NAME                  = _T("rdo console studio");

const tstring NOP_STRING   = _T(" ");
const tstring COMMA_STRING = _T(",");

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_CONFIG_H_
