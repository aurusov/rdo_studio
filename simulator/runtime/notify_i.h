#ifndef _LIB_RUNTIME_NOTIFY_I_H_
#define _LIB_RUNTIME_NOTIFY_I_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

class INotify: public virtual rdo::counter_reference
{
public:
	virtual void notify(std::size_t message, void* pParam) = 0;
};
#define DECLARE_INotify \
	virtual void notify(std::size_t message, void* pParam);

#endif // _LIB_RUNTIME_NOTIFY_I_H_
