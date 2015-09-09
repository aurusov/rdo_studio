// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "app/rdo_console/terminate_codes.h"
#include "app/rdo_console/options/controller_console_options.h"
#include "app/rdo_console/options/controller_console_options_config.h"
// --------------------------------------------------------------------------------

using namespace rdo;

ControllerConsoleOptions::ControllerConsoleOptions(int argc, char *argv[])
    : help(false)
    , convert(false)
{
    const po::options_description options_header(boost::str(boost::format("%1%\n\n%2%")
        % rdo::version::g_versionName
        % rdo::version::g_site
    ));

    po::options_description options_general("General options");
    createGeneralOptions(options_general);

    po::options_description options_convertor("Convertor options");
    createConvertorOptions(options_convertor);

    po::options_description options_additional("Compatibility options (skipped in console version)");
    createAdditionalOptions(options_additional);

    options.add(options_header);
    options.add(options_general);
    options.add(options_convertor);
    options.add(options_additional);

    try
    {
        po::store(po::parse_command_line(argc, argv, options), variables);
        po::notify(variables);
    }
    catch (const std::exception& e)
    {
        const std::string error = boost::str(boost::format("command line error: %1%") % e.what());
        rdo::locale::cout(error);
    }
}

ControllerConsoleOptions::~ControllerConsoleOptions()
{}

void ControllerConsoleOptions::parseOptions()
{
    if ((variables.empty() || variables.count(HELP_COMMAND)) &&
        !variables.count(LANGUAGE_COMMAND) &&
        !variables.count(VERSION_COMMAND))
    {
        std::stringstream stream;
        stream << options;
        rdo::locale::cout(stream.str());
        help = true;
    }
    else if (variables.count(LANGUAGE_COMMAND))
    {
        const std::string message("rdo language v" + RDO_LANGUAGE_VERSION + " ( supported rdox )");
        rdo::locale::cout(message);
        help = true;
    }
    else if (variables.count(VERSION_COMMAND))
    {
        rdo::locale::cout(rdo::version::g_versionName);
        help = true;
    }
    else if(variables.count(CONVERTOR_COMMAND))
    {
        convert = true;
    }
}

boost::filesystem::path ControllerConsoleOptions::getModelFileName() const
{
    boost::filesystem::path result;
    if (variables.count(MODEL_COMMAND))
    {
        std::string modelFileName = variables[MODEL_COMMAND].as<std::string>();
#if defined(OST_WINDOWS)
        modelFileName = rdo::locale::convertFromCLocale(modelFileName);
#elif defined(OST_LINUX)
        modelFileName = rdo::locale::convert(modelFileName, rdo::locale::get().utf8(), rdo::locale::get().system());
#endif
        result = rdo::locale::convertToWStr(modelFileName);
    }
    return result;
}

boost::filesystem::path ControllerConsoleOptions::getScriptFileName() const
{
    boost::filesystem::path result;
    if (variables.count(SCRIPT_COMMAND))
    {
        std::string eventsFileName = variables[SCRIPT_COMMAND].as<std::string>();
#if defined(OST_WINDOWS)
        eventsFileName = rdo::locale::convertFromCLocale(eventsFileName);
#elif defined(OST_LINUX)
        eventsFileName = rdo::locale::convert(eventsFileName, rdo::locale::get().utf8(), rdo::locale::get().system());
#endif
        result = rdo::locale::convertToWStr(eventsFileName);
    }
    return result;
}

bool ControllerConsoleOptions::helpQuery() const
{
    return help;
}

bool ControllerConsoleOptions::convertQuery() const
{
    return convert;
}

void ControllerConsoleOptions::createGeneralOptions(po::options_description& options)
{
    options.add_options()
            ((MODEL_COMMAND + COMMA_STRING + MODEL_COMMAND_SHORT).c_str(), po::value<std::string>(), MODEL_COMMENT.c_str())
            ((SCRIPT_COMMAND + COMMA_STRING + SCRIPT_COMMAND_SHORT).c_str(), po::value<std::string>(), SCRIPT_COMMENT.c_str())
            ((LANGUAGE_COMMAND + COMMA_STRING + LANGUAGE_COMMAND_SHORT).c_str(), LANGUAGE_COMMENT.c_str())
            ((VERSION_COMMAND + COMMA_STRING + VERSION_COMMAND_SHORT).c_str(), VERSION_COMMENT.c_str())
            ((HELP_COMMAND + COMMA_STRING + HELP_COMMAND_SHORT).c_str(), HELP_COMMENT.c_str());
}

void ControllerConsoleOptions::createConvertorOptions(po::options_description& options)
{
    options.add_options()
        ((CONVERTOR_COMMAND + COMMA_STRING + CONVERTOR_COMMAND_SHORT).c_str(), CONVERTOR_COMMENT.c_str());
}

void ControllerConsoleOptions::createAdditionalOptions(po::options_description& options)
{
    options.add_options()
            (AUTO_RUN_COMMAND.c_str(), AUTO_RUN_COMMENT.c_str())
            (AUTO_EXIT_COMMAND.c_str(), AUTO_EXIT_COMMENT.c_str())
            (DONT_CLOSE_IF_ERROR_COMMAND.c_str(), DONT_CLOSE_IF_ERROR_COMMENT.c_str())
            (PLUGIN_START_COMMAND.c_str(), PLUGIN_START_COMMENT.c_str())
            (PLUGIN_AUTO_EXIT_COMMAND.c_str(), PLUGIN_AUTO_EXIT_COMMENT.c_str());
}
