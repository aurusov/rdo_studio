#ifndef _UTILS_SMART_PTR_REF_COUNTER_H_
#define _UTILS_SMART_PTR_REF_COUNTER_H_

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

#endif // _UTILS_SMART_PTR_REF_COUNTER_H_
