/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#include <io.h>
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	#include <unistd.h>
#endif // COMPILER_GCC

#include <fstream>
#include <boost/filesystem.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

inline rbool File::create(CREF(tstring) name)
{
	return create(name, _T(""));
}

inline rbool File::create(CREF(tstring) name, CREF(tstring) content)
{
	std::ofstream file(name.c_str(), std::ios::out | std::ios::binary);
	file << content.c_str() << std::endl;
	file.close();
	return true;
}

inline rbool File::exist(CREF(tstring) name)
{
	return  boost::filesystem::exists(name.c_str());
}

inline rbool File::read_only(CREF(tstring) name)
{
#ifdef COMPILER_VISUAL_STUDIO
	return _access(name.c_str(), 04) == 0 && _access(name.c_str(), 06) == -1;
#endif  // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	return access(name.c_str(), R_OK) == 0 && access(name.c_str(), W_OK) == -1;
#endif // COMPILER_GCC
}

inline rbool File::unlink(CREF(tstring) name)
{
	return boost::filesystem::remove(name.c_str());
}

CLOSE_RDO_NAMESPACE
