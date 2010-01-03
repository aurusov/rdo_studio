/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofile.inl
 * author   : Урусов Андрей
 * date     : 10.05.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
#include <io.h>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

inline rbool File::exist(CREF(tstring) name)
{
	return _access(name.c_str(), 00) == 0;
}

inline rbool File::read_only(CREF(tstring) name)
{
	return _access(name.c_str(), 04) == 0 && _access(name.c_str(), 06) == -1;
}

inline rbool File::unlink(CREF(tstring) name)
{
	return _unlink(name.c_str()) == 0;
}

CLOSE_RDO_NAMESPACE
