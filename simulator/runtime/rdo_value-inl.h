// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/static_assert.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class T>
inline RDOValue::RDOValue(const LPRDOType& pType, const rdo::intrusive_ptr<T>& pObject)
    : m_pType(pType)
{
    STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<T>) >= sizeof(double));

    ASSERT(m_pType);
    ASSERT(
        typeID() == RDOType::t_string        ||
        typeID() == RDOType::t_identificator ||
        typeID() == RDOType::t_pointer
    );
    new (&m_value) rdo::intrusive_ptr_interface_wrapper<T>(pObject);
    setUndefined(false);
}

template <class T>
inline rdo::intrusive_ptr<T>& RDOValue::getPointer()
{
    return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

template <class T>
inline const rdo::intrusive_ptr<T>& RDOValue::getPointer() const
{
    return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

template <class T>
inline const rdo::intrusive_ptr<typename T::value_type>& RDOValue::getPointerByType() const
{
    return getPointerByType<typename T::value_type, T>();
}

template <class V, class T>
inline const rdo::intrusive_ptr<V>& RDOValue::getPointerByType() const
{
#ifdef _DEBUG
    rdo::intrusive_ptr<T> pType = type().object_dynamic_cast<T>();
    ASSERT(pType);
#endif // _DEBUG

    return getPointer<V>();
}

template <class T>
inline const rdo::intrusive_ptr<typename T::value_type>& RDOValue::getPointerByInterface() const
{
#ifdef _DEBUG
    rdo::interface_ptr<T> pType = type().interface_dynamic_cast<T>();
    ASSERT(pType);
#endif // _DEBUG

    return getPointer<typename T::value_type>();
}

template <class T>
inline bool RDOValue::isType() const
{
    return type().object_dynamic_cast<T>();
}

template <class T>
inline T& RDOValue::__get()
{
    return *reinterpret_cast<T*>(&m_value);
}

template <class T>
inline const T& RDOValue::__get() const
{
    return *reinterpret_cast<const T*>(&m_value);
}

CLOSE_RDO_RUNTIME_NAMESPACE
