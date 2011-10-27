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

#ifdef OST_WINDOWS
const tstring SYSTEM_OS = _T("Windows");
#else
const tstring SYSTEM_OS = _T("Linux");
#endif

namespace po = boost::program_options;

const tstring RDO_STUDIO_CONSOLE_VERSION = _T("0.1.0");
const tstring RDO_LANGUAGE_VERSION       = _T("1.5");

int main(int argc, char *argv[])
{
	po::options_description description(_T("rdo console studio v") + 
	                                    RDO_STUDIO_CONSOLE_VERSION + " " + 
	                                    SYSTEM_OS + _T(" ( http://rdo.rk9.bmstu.ru )"));
	description.add_options()
	        _T(("help,h", "display help message"))
	        _T(("language,l", "display language version of RDO"))
	        _T(("version,v", "display program version"));
	po::variables_map options;
	try
	{
		po::store(po::parse_command_line(argc, argv, description), options);
	}
	catch (CREF(std::exception) e)
	{
		std::cout << _T("command line error: ") << e.what() << std::endl;
	}
	if (options.empty() || options.count(_T("help")))
	{
		std::cout << description << std::endl;
	}
	else if (options.count(_T("language")))
	{
		std::cout << _T("rdo language v") + RDO_LANGUAGE_VERSION << std::endl;
	}
	else if (options.count(_T("version")))
	{
		std::cout << _T("rdo console studio v") + RDO_STUDIO_CONSOLE_VERSION << std::endl;
	}
	else
	{
		// main code slot
	}
	return 0;
}
