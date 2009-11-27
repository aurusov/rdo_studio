// ====================================================================== INCLUDES
#include <io.h>
// ====================================================================== SYNOPSIS
#include "rdotypes.h"
#include "rdomacros.h"
// ===============================================================================

namespace rdo
{

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

} //! namespace rdo
