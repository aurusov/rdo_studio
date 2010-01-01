#ifndef RDOFILE_H
#define RDOFILE_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// ===============================================================================

namespace rdo {

class File
{
public:
	static rbool exist    (CREF(tstring) name);
	static rbool read_only(CREF(tstring) name);
	static rbool unlink   (CREF(tstring) name);
};

} //! namespace rdo

#include "rdo_common/rdofile.inl"

#endif //! RDOFILE_H
