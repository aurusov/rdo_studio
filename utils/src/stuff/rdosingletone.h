#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
class SingleTone
{
public:
    typedef T             object_type;
    typedef SingleTone<T> this_type;

    SingleTone()
        : m_object(NULL)
    {}
    ~SingleTone()
    {
        if (m_object)
            delete m_object;
    }
    T* instance()
    {
        if (!m_object)
            m_object = new T();
        return m_object;
    }
    template <typename P1>
    T* instance(const P1& p1)
    {
        if (!m_object)
            m_object = new T(p1);
        return m_object;
    }

private:
    T* m_object;
};

} // namespace rdo
