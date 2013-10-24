/*!
  \copyright (c) RDO-Team, 2011
  \file      app/rdo_console/controller_console_options.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/format.hpp>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "utils/src/locale/rdolocale.h"
#include "app/rdo_console/terminate_codes.h"
#include "app/rdo_console/controller_console_options.h"
#include "app/rdo_console/controller_console_options_config.h"
// --------------------------------------------------------------------------------

using namespace rdo;

ControllerConsoleOptions::ControllerConsoleOptions(int argc, char *argv[]) :
	m_help(false),
	m_convert(false)
{
	po::options_description options_header(boost::str(boost::format("%1%\n\n%2%")
		% rdo::version::g_versionName
		% rdo::version::g_site
	));

	po::options_description options_general("General options");
	createGeneralOptions(options_general);

	po::options_description options_convertor("Convertor options");
	createConvertorOptions(options_convertor);

	po::options_description options_additional("Compatibility options (skipped in console version)");
	createAdditionalOptions(options_additional);

	m_options.add(options_header);
	m_options.add(options_general);
	m_options.add(options_convertor);
	m_options.add(options_additional);

	try
	{
		po::store(po::parse_command_line(argc, argv, m_options), m_variables);
		po::notify(m_variables);
	}
	catch (CREF(std::exception) e)
	{
		std::wcout << rdo::locale::convertToWStr(boost::str(
			boost::format("command line error: %1%") % e.what())) << std::endl;
	}
}

ControllerConsoleOptions::~ControllerConsoleOptions()
{
}

void ControllerConsoleOptions::parseOptions()
{
	if ((m_variables.empty() || m_variables.count(HELP_COMMAND)) && 
		    !m_variables.count(LANGUAGE_COMMAND) &&
		    !m_variables.count(VERSION_COMMAND))
	{
		std::stringstream stream;
		stream << m_options;
		std::wcout << rdo::locale::convertToWStr(stream.str()) << std::endl;
		m_help = true;
	}
	else if (m_variables.count(LANGUAGE_COMMAND))
	{
		std::wcout << rdo::locale::convertToWStr("rdo language v" + RDO_LANGUAGE_VERSION + " ( supported rdox )") << std::endl;
		m_help = true;
	}
	else if (m_variables.count(VERSION_COMMAND))
	{
		std::wcout << rdo::locale::convertToWStr(rdo::version::g_versionName) << std::endl;
		m_help = true;
	}
	else if(m_variables.count(CONVERTOR_COMMAND))
	{
		m_convert = true;
	}
}

boost::filesystem::path ControllerConsoleOptions::getModelFileName() const
{
	boost::filesystem::path result;
	if (m_variables.count(MODEL_COMMAND))
	{
		tstring modelFileName = m_variables[MODEL_COMMAND].as<tstring>();
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
	if (m_variables.count(SCRIPT_COMMAND))
	{
		tstring eventsFileName = m_variables[SCRIPT_COMMAND].as<tstring>();
#if defined(OST_WINDOWS)
		eventsFileName = rdo::locale::convertFromCLocale(eventsFileName);
#elif defined(OST_LINUX)
		eventsFileName = rdo::locale::convert(eventsFileName, rdo::locale::get().utf8(), rdo::locale::get().system());
#endif
		result = rdo::locale::convertToWStr(eventsFileName);
	}
	return result;
}

rbool ControllerConsoleOptions::helpQuery()
{
	return m_help;
}

rbool ControllerConsoleOptions::convertQuery()
{
	return m_convert;
}

void ControllerConsoleOptions::createGeneralOptions(REF(po::options_description) options)
{
	options.add_options()
			((MODEL_COMMAND + COMMA_STRING + MODEL_COMMAND_SHORT).c_str(), po::value<tstring>(), MODEL_COMMENT.c_str())
			((SCRIPT_COMMAND + COMMA_STRING + SCRIPT_COMMAND_SHORT).c_str(), po::value<tstring>(), SCRIPT_COMMENT.c_str())
			((LANGUAGE_COMMAND + COMMA_STRING + LANGUAGE_COMMAND_SHORT).c_str(), LANGUAGE_COMMENT.c_str())
			((VERSION_COMMAND + COMMA_STRING + VERSION_COMMAND_SHORT).c_str(), VERSION_COMMENT.c_str())
			((HELP_COMMAND + COMMA_STRING + HELP_COMMAND_SHORT).c_str(), HELP_COMMENT.c_str());
}

void ControllerConsoleOptions::createConvertorOptions(REF(po::options_description) options)
{
	options.add_options()
		((CONVERTOR_COMMAND + COMMA_STRING + CONVERTOR_COMMAND_SHORT).c_str(), CONVERTOR_COMMENT.c_str());
}

void ControllerConsoleOptions::createAdditionalOptions(REF(po::options_description) options)
{
	options.add_options()
			(AUTO_RUN_COMMAND.c_str(), AUTO_RUN_COMMENT.c_str())
			(AUTO_EXIT_COMMAND.c_str(), AUTO_EXIT_COMMENT.c_str())
			(DONT_CLOSE_IF_ERROR_COMMAND.c_str(), DONT_CLOSE_IF_ERROR_COMMENT.c_str())
			(PLUGIN_START_COMMAND.c_str(), PLUGIN_START_COMMENT.c_str())
			(PLUGIN_AUTO_EXIT_COMMAND.c_str(), PLUGIN_AUTO_EXIT_COMMENT.c_str());	
}
