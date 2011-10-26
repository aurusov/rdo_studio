/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/program_options.hpp>

#ifdef OST_WINDOWS
const std::string SYSTEM_OS = "Windows";
#else
const std::string SYSTEM_OS = "Linux";
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------


namespace po = boost::program_options;

const std::string RDO_STUDIO_CONSOLE_VERSION = "0.1.0";
const std::string RDO_LANGUAGE_VERSION = "1.5";

int main(int argc, char *argv[])
{
    po::options_description description("rdo console studio v" + 
                                        RDO_STUDIO_CONSOLE_VERSION + " " + 
                                        SYSTEM_OS + " ( http://rdo.rk9.bmstu.ru )");
    description.add_options()
            ("help,h", "display help message")
            ("language,l", "display language version of RDO")
            ("version,v", "display program version");
    po::variables_map options;
    try {
        po::store(po::parse_command_line(argc, argv, description), options);
    } catch (const std::exception& e) {
        std::cout << "command line error: " << e.what() << std::endl;
        return 0;
    }
    if (options.empty() || options.count("help")) {
        std::cout << description << std::endl;
        return 0;
    }
    else if (options.count("language")) {
        std::cout << "rdo language v" + RDO_LANGUAGE_VERSION << std::endl;
        return 0;
    }
    else if(options.count("version")) {
        std::cout << "rdo console studio v" + RDO_STUDIO_CONSOLE_VERSION << std::endl;
        return 0;
    }
    else {
        // main code slot
    }
    return 0;
}
