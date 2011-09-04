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
#include "rdo_common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef OS_WINDOWS
	#include <io.h>
#endif

#include <fstream>
#include <boost/filesystem.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
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
#ifdef OS_WINDOWS
	return _access(name.c_str(), 04) == 0 && _access(name.c_str(), 06) == -1;
#else  // not OS_WINDOWS
	//! @todo написать реализацию под линуха
	return true;
#endif // OS_WINDOWS
}

inline rbool File::unlink(CREF(tstring) name)
{
	return boost::filesystem::remove(name.c_str());
}

CLOSE_RDO_NAMESPACE
