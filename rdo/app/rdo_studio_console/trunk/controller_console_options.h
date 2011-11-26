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

// ----------------------------------------------------------------------- INCLUDES
#include <boost/program_options.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4512)
#endif // COMPILER_VISUAL_STUDIO

namespace po = boost::program_options;

class RDOControllerConsoleOptions
{
public:
	RDOControllerConsoleOptions(int argc, char *argv[]);

	void parseQuery();
	void getModelName(tstring& model_name);

private:
	void create_general_options(po::options_description& options);
	void create_additional_options(po::options_description& options);

private:
	po::options_description m_options;
	po::variables_map m_variables;
};

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_H_