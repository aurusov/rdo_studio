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

inline rbool File::splitpath(CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt)
{
	char _drive[_MAX_DRIVE];
	char _dir  [_MAX_DIR  ];
	char _name [_MAX_FNAME];
	char _ext  [_MAX_EXT  ];

	if (_splitpath_s(name.c_str(), _drive, _MAX_DRIVE, _dir, _MAX_DIR, _name, _MAX_FNAME, _ext, _MAX_EXT) != 0)
		return false;

	fileDir  = rdo::format(_T("%s%s"), _drive, _dir);
	fileName = _name;
	fileExt  = _ext;
	return true;
}

CLOSE_RDO_NAMESPACE
