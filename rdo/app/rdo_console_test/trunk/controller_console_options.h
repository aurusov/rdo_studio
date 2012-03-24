/*!
  \copyright (c) RDO-Team, 2011
  \file      controller_console_options.h
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

namespace po = boost::program_options;

class RDOControllerConsoleOptions
{
public:
	RDOControllerConsoleOptions(int argc, char *argv[]);
	~RDOControllerConsoleOptions();
	
	void parseOptions();
	
	rbool getEtalonTraceFileName(REF(tstring) name);
	rbool getEtalonResultFileName(REF(tstring) name);
	rbool getTraceFileName(REF(tstring) name);
	rbool getResultFileName(REF(tstring) name);
	
	rbool helpQuery();
	
private:
	template<class T>
	rbool getParameter(REF(T) name, CREF(tstring) parameter);
	void createGeneralOptions(REF(po::options_description) options);
	
private:
	po::options_description	m_options;
	po::variables_map		m_variables;
	
	rbool					m_help;
};

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (default:4512)
#endif // COMPILER_VISUAL_STUDIO

#include "app/rdo_studio_console/test/controller_console_options.inl"

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_H_
