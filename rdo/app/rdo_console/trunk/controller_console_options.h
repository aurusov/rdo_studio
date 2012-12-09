/*!
  \copyright (c) RDO-Team, 2011
  \file      app/rdo_console/controller_console_options.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_CONTROLLER_CONSOLE_OPTIONS_H_
#define _RDO_CONTROLLER_CONSOLE_OPTIONS_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4512)
#endif // COMPILER_VISUAL_STUDIO

#include <boost/program_options.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

namespace po = boost::program_options;

class ControllerConsoleOptions
{
public:
	ControllerConsoleOptions(int argc, char *argv[]);
	~ControllerConsoleOptions();
	
	void parseOptions();

	const tstring getModelFileName();
	const tstring getEventsFileName();
	
	rbool helpQuery();
	rbool convertQuery();
	
private:
	void createGeneralOptions(REF(po::options_description) options);
	void createConvertorOptions(REF(po::options_description) options);
	void createAdditionalOptions(REF(po::options_description) options);
	
private:
	po::options_description m_options;
	po::variables_map       m_variables;
	
	rbool                   m_help;
	rbool                   m_convert;
};

CLOSE_RDO_NAMESPACE

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (default:4512)
#endif // COMPILER_VISUAL_STUDIO

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_H_
