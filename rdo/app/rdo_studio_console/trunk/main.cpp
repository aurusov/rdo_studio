/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/program_options.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4512)
#endif

#ifdef OST_WINDOWS
const tstring SYSTEM_OS = _T("Windows");
#elif OST_LINUX
const tstring SYSTEM_OS = _T("Linux");
#endif

const tstring HELP_COMMANDS     = _T("help");
const tstring LANGUAGE_COMMANDS = _T("language");
const tstring VERSION_COMMANDS  = _T("version");

const tstring HELP_COMMANDS_SHORT     = _T("h");
const tstring LANGUAGE_COMMANDS_SHORT = _T("l");
const tstring VERSION_COMMANDS_SHORT  = _T("v");

const tstring HELP_COMMENT     = _T("display help message");
const tstring LANGUAGE_COMMENT = _T("display language version of RDO");
const tstring VERSION_COMMENT  = _T("display program version");

const tstring RDO_STUDIO_CONSOLE_VERSION = _T("0.1.0");
const tstring RDO_LANGUAGE_VERSION       = _T("1.5");

const tstring RDO_SITE                         = _T("( http://rdo.rk9.bmstu.ru )");
const tstring RDO_STUDIO_CONSOLE_PROGRAMM_NAME = _T("rdo console studio");

const tstring NOP_STRING   = _T(" ");
const tstring COMMA_STRING = _T(",");

namespace po = boost::program_options;

tstring create_description_message();
void create_description(po::options_description& description);

int main(int argc, char *argv[])
{
	po::options_description description(create_description_message().c_str());
	create_description(description);
	
	po::variables_map options;
	try
	{
		po::store(po::parse_command_line(argc, argv, description), options);
	}
	catch (CREF(std::exception) e)
	{
		std::cout << _T("command line error: ") << e.what() << std::endl;
	}
	if (options.empty() || options.count(HELP_COMMANDS))
	{
		std::cout << description << std::endl;
	}
	else if (options.count(LANGUAGE_COMMANDS))
	{
		std::cout << _T("rdo language v") + RDO_LANGUAGE_VERSION << std::endl;
	}
	else if (options.count(VERSION_COMMANDS))
	{
		std::cout << RDO_STUDIO_CONSOLE_PROGRAMM_NAME + NOP_STRING + VERSION_COMMANDS_SHORT + RDO_STUDIO_CONSOLE_VERSION << std::endl;
	}
	else
	{
		// main code slot
	}
	return 0;
}

tstring create_description_message()
{
	return  RDO_STUDIO_CONSOLE_PROGRAMM_NAME + NOP_STRING + VERSION_COMMANDS_SHORT +
			RDO_STUDIO_CONSOLE_VERSION + NOP_STRING + 
			SYSTEM_OS + RDO_SITE;
}

void create_description(po::options_description& description)
{
	description.add_options()
			((HELP_COMMANDS + COMMA_STRING + HELP_COMMANDS_SHORT).c_str(), HELP_COMMENT.c_str())
			((LANGUAGE_COMMANDS + COMMA_STRING + LANGUAGE_COMMANDS_SHORT).c_str(), LANGUAGE_COMMENT.c_str())
			((VERSION_COMMANDS + COMMA_STRING + VERSION_COMMANDS_SHORT).c_str(), VERSION_COMMENT.c_str());	
}
