#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
class RefCounter: public IRefCounter
{
public:
	DECLARE_IRefCounter;
};

} // namespace rdo
