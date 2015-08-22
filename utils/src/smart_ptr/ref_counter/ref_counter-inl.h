// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
inline void RefCounter<T>::addref()
{
    counter_reference* pCounter = dynamic_cast<counter_reference*>(this);
    ASSERT(pCounter);
    pCounter->m_intrusive_counter++;
}

template<class T>
inline void RefCounter<T>::release()
{
    counter_reference* pCounter = dynamic_cast<counter_reference*>(this);
    ASSERT(pCounter);
    pCounter->m_intrusive_counter--;
    if (pCounter->m_intrusive_counter == 0)
    {
        T* pObject = dynamic_cast<T*>(this);
        ASSERT(pObject);
        Factory<T>::destroy(pObject);
    }
}

template<class T>
inline bool RefCounter<T>::owner() const
{
    const counter_reference* pCounter = dynamic_cast<const counter_reference*>(this);
    ASSERT(pCounter);
    return pCounter->m_intrusive_counter == 1;
}

} // namespace rdo
