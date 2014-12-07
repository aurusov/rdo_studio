#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

template <bool>
struct CompileTimeError;

template <>
struct CompileTimeError<true> {};

#define STATIC_ASSERT(expr) \
	rdo::CompileTimeError<((expr) != 0)>();

} // namespace rdo
