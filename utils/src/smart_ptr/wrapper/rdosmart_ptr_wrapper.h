#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

namespace rdo {

class ISmartPtrWrapper
{
public:
    virtual void destroy() = 0;
    virtual void* getSmartPtr() = 0;
    virtual const counter_reference* getRefCounter() const = 0;
};
typedef ISmartPtrWrapper* LPISmartPtrWrapper;

#define DECLARE_ISmartPtrWrapper \
    virtual void destroy();      \
    virtual void* getSmartPtr(); \
    virtual const counter_reference* getRefCounter() const;

template<class T>
class smart_ptr_wrapper: public ISmartPtrWrapper
{
public:
    smart_ptr_wrapper(const intrusive_ptr<T>& intrusive_ptr)
        : m_intrusive_ptr(intrusive_ptr)
    {}
    virtual ~smart_ptr_wrapper()
    {}
    void destroy()
    {
        delete this;
    }
    void* getSmartPtr()
    {
        return &m_intrusive_ptr;
    }
    const counter_reference* getRefCounter() const
    {
        return m_intrusive_ptr.m_object;
    }
    const intrusive_ptr<T>& get() const
    {
        return m_intrusive_ptr;
    }

private:
    intrusive_ptr<T> m_intrusive_ptr;
};

class smart_ptr_wrapper_caster
{
public:
    smart_ptr_wrapper_caster(const LPISmartPtrWrapper& pISmartPtrWrapper)
        : m_pISmartPtrWrapper(pISmartPtrWrapper)
    {
        ASSERT(m_pISmartPtrWrapper);
    }
    ~smart_ptr_wrapper_caster()
    {
        m_pISmartPtrWrapper->destroy();
    }

    template <class T>
    intrusive_ptr<T> cast() const
    {
        rdo::smart_ptr_wrapper<T>* pSmartPtrWrapper = dynamic_cast<rdo::smart_ptr_wrapper<T>*>(m_pISmartPtrWrapper);
        return pSmartPtrWrapper ? pSmartPtrWrapper->get() : intrusive_ptr<T>();
    }

private:
    LPISmartPtrWrapper m_pISmartPtrWrapper;
};

} // namespace rdo
