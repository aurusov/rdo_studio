/*!
  \copyright (c) RDO-Team, 2011
  \file      controller_console_options.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "app/rdo_console_test/controller_console_options.h"
#include "app/rdo_console_test/controller_console_options_config.h"
// --------------------------------------------------------------------------------

const tstring program_description = PROGRAM_NAME + NOP_STRING + VERSION_COMMAND_SHORT + PROGRAM_VERSION;

RDOControllerConsoleOptions::RDOControllerConsoleOptions(int argc, char *argv[]) :
	m_help(false)
{
	po::options_description options_header(program_description);
	
	po::options_description options_general(_T("General options"));
	createGeneralOptions(options_general);
	
	m_options.add(options_header);
	m_options.add(options_general);
	
	try
	{
		po::store(po::parse_command_line(argc, argv, m_options), m_variables);
		po::notify(m_variables);
	}
	catch (CREF(std::exception) e)
	{
		std::cout << _T("command line error: ") << e.what() << std::endl;
	}
}

RDOControllerConsoleOptions::~RDOControllerConsoleOptions()
{
	
}

void RDOControllerConsoleOptions::parseOptions()
{
	if (m_variables.empty() || m_variables.count(CHELP_COMMAND) && !m_variables.count(VERSION_COMMAND))
	{
		std::cout << m_options << std::endl;
		m_help = true;
	}
	else if (m_variables.count(VERSION_COMMAND))
	{
		std::cout << PROGRAM_NAME + NOP_STRING + VERSION_COMMAND_SHORT + PROGRAM_VERSION << std::endl;
		m_help = true;
	}
}

rbool RDOControllerConsoleOptions::getEtalonTraceFileName(REF(tstring) name)
{
	return getParameter(name, ETALON_TRACE_COMMAND);
}

rbool RDOControllerConsoleOptions::getEtalonResultFileName(REF(tstring) name)
{
	return getParameter(name, ETALON_RESULT_COMMAND);
}

rbool RDOControllerConsoleOptions::getTraceFileName(REF(tstring) name)
{
	return getParameter(name, TRACE_COMMAND);
}

rbool RDOControllerConsoleOptions::getResultFileName(REF(tstring) name)
{
	return getParameter(name, RESULT_COMMAND);
}

rbool RDOControllerConsoleOptions::helpQuery()
{
	return m_help;
}

void RDOControllerConsoleOptions::createGeneralOptions(REF(po::options_description) options)
{
	options.add_options()
			((ETALON_TRACE_COMMAND + COMMA_STRING + ETALON_TRACE_COMMAND_SHORT).c_str(), po::value<tstring>(), ETALON_TRACE_COMMENT.c_str())
			((ETALON_RESULT_COMMAND + COMMA_STRING + ETALON_RESULT_COMMAND_SHORT).c_str(), po::value<tstring>(), ETALON_TRACE_COMMENT.c_str())
			((TRACE_COMMAND + COMMA_STRING + TRACE_COMMAND_SHORT).c_str(), po::value<tstring>(), TRACE_COMMENT.c_str())
			((RESULT_COMMAND + COMMA_STRING + RESULT_COMMAND_SHORT).c_str(), po::value<tstring>(), RESULT_COMMENT.c_str())
			((VERSION_COMMAND + COMMA_STRING + VERSION_COMMAND_SHORT).c_str(), VERSION_COMMENT.c_str())
			((CHELP_COMMAND + COMMA_STRING + HELP_COMMAND_SHORT).c_str(), HELP_COMMENT.c_str());
}
