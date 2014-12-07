#ifndef _UTILS_SMART_PTR_COUNTER_REFERENCE_H_
#define _UTILS_SMART_PTR_COUNTER_REFERENCE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <cstddef>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {

class counter_reference
{
template<class T> friend class intrusive_ptr;
template<class T> friend class RefCounter;
public:
	counter_reference();
	virtual ~counter_reference();

private:
	std::size_t m_intrusive_counter;
};

} // namespace rdo

#endif // _UTILS_SMART_PTR_COUNTER_REFERENCE_H_
