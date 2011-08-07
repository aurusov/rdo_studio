/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofile.inl
 * author   : Урусов Андрей, Evgeny Proydakov
 * date     : 10.05.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
#ifdef WIN32
#include <io.h>
#endif
#include <fstream>
#include <boost/filesystem.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

inline rbool File::create(CREF(tstring) name)
{
	return create(name, "");
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
#ifdef WIN32
	return _access(name.c_str(), 04) == 0 && _access(name.c_str(), 06) == -1;
#else
        // TODO
	return true;
#endif
}

inline rbool File::unlink(CREF(tstring) name)
{
	return boost::filesystem::remove(name.c_str());
}

CLOSE_RDO_NAMESPACE
