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
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (disable:4512)
#endif // COMPILER_VISUAL_STUDIO

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

namespace po = boost::program_options;

class ControllerConsoleOptions
{
public:
	ControllerConsoleOptions(int argc, char* argv[]);
	~ControllerConsoleOptions();

	void parseOptions();

	boost::filesystem::path getModelFileName () const;
	boost::filesystem::path getScriptFileName() const;

	bool helpQuery() const;
	bool convertQuery() const;

private:
	po::options_description m_options;
	po::variables_map m_variables;
	bool m_help;
	bool m_convert;

	void createGeneralOptions(po::options_description& options);
	void createConvertorOptions(po::options_description& options);
	void createAdditionalOptions(po::options_description& options);
};

} // namespace rdo

#ifdef COMPILER_VISUAL_STUDIO
#	pragma warning (default:4512)
#endif // COMPILER_VISUAL_STUDIO

#endif // _RDO_CONTROLLER_CONSOLE_OPTIONS_H_
