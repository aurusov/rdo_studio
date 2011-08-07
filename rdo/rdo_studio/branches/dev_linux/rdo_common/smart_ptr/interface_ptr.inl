/*
 * copyright: (c) RDO-Team, 2010
 * filename : interface_ptr.inl
 * author   : Урусов Андрей
 * date     : 12.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdodebug.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
inline void CounterReferenceReal<T>::addref()
{
        PTR(counter_reference) pCounter = dynamic_cast<PTR(counter_reference)>(this);
        ASSERT(pCounter);
        pCounter->m_intrusive_counter++;
}

template<class T>
inline void CounterReferenceReal<T>::release()
{
        PTR(counter_reference) pCounter = dynamic_cast<PTR(counter_reference)>(this);
        ASSERT(pCounter);
        pCounter->m_intrusive_counter--;
        if (pCounter->m_intrusive_counter == 0)
        {
                PTR(T) pObject = dynamic_cast<PTR(T)>(this);
                ASSERT(pObject);
                Factory<T>::destroy(pObject);
        }
}

template<class T>
inline interface_ptr<T>::interface_ptr()
        : m_pInterface(NULL)
        , m_pCounter  (NULL)
{}

template<class T>
inline interface_ptr<T>::interface_ptr(PTR(T) pInterface, LPICounterReference pCounter)
        : m_pInterface(pInterface)
        , m_pCounter  (pCounter  )
{
#ifdef WIN32
        if (m_pInterface)
                m_pCounter->addref();
#endif
}

template<class T>
inline interface_ptr<T>::interface_ptr(CREF(this_type) sptr)
        : m_pInterface(sptr.m_pInterface)
        , m_pCounter  (sptr.m_pCounter  )
{
#ifdef WIN32
        if (m_pInterface)
                m_pCounter->addref();
#endif
}

template<class T>
inline interface_ptr<T>::~interface_ptr()
{
#ifdef WIN32
        if (m_pInterface)
                m_pCounter->release();
#endif
}

template<class T>
inline REF(typename interface_ptr<T>::this_type) interface_ptr<T>::operator= (CREF(this_type) sptr)
{
#ifdef WIN32
        if (m_pInterface)
                m_pCounter->release();
#endif

        m_pInterface = sptr.m_pInterface;
        m_pCounter   = sptr.m_pCounter;

#ifdef WIN32
        if (m_pInterface)
                m_pCounter->addref();
#endif

        return *this;
}

template<class T>
inline interface_ptr<T>::operator rbool () const
{
        return m_pInterface != NULL;
}

template<class T>
inline CPTR(T) interface_ptr<T>::operator-> () const
{
        return m_pInterface;
}

template<class T>
inline PTR(T) interface_ptr<T>::operator-> ()
{
        return m_pInterface;
}

CLOSE_RDO_NAMESPACE
