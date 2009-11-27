#ifndef RDOFILE_H
#define RDOFILE_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdotypes.h"
#include "rdomacros.h"
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

#include "rdofile.inl"

#endif //! RDOFILE_H
