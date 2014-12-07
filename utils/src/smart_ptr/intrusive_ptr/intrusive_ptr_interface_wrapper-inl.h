// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/ref_counter/ref_counter_i.h"
// --------------------------------------------------------------------------------

namespace rdo {

// --------------------------------------------------------------------------------
// -------------------- RefCounter<T>
// --------------------------------------------------------------------------------
template <class T>
inline intrusive_ptr_interface_wrapper<T>::intrusive_ptr_interface_wrapper(const rdo::intrusive_ptr<T>& pPointer)
    : parent_type(pPointer)
{}

template <class T>
inline void intrusive_ptr_interface_wrapper<T>::addref()
{
    parent_type::addref();
}

template <class T>
inline void intrusive_ptr_interface_wrapper<T>::release()
{
    parent_type::release();
}

template <class T>
inline bool intrusive_ptr_interface_wrapper<T>::owner() const
{
    return parent_type::owner();
}

} // namespace rdo
