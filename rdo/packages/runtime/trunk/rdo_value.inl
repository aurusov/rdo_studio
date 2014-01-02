/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_value.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.07.2008
  \brief     RDOValue
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/static_assert.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

template <class T>
inline RDOValue::RDOValue(CREF(LPRDOType) pType, CREF(rdo::intrusive_ptr<T>) pObject)
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

inline CREF(LPRDOType) RDOValue::type() const
{
	return m_pType;
}

inline RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->typeID();
}

inline REF(rdo::ostream) operator<< (REF(rdo::ostream) stream, CREF(RDOValue) rdovalue)
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

template <class T>
inline REF(rdo::intrusive_ptr<T>) RDOValue::getPointer()
{
	return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

template <class T>
inline CREF(rdo::intrusive_ptr<T>) RDOValue::getPointer() const
{
	return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

template <class T>
inline CREF(rdo::intrusive_ptr<typename T::value_type>) RDOValue::getPointerByType() const
{
	struct Pair
	{
		typedef  T                       Type;
		typedef  typename T::value_type  Value;
	};

	return getPointerByType<Pair>();
}

template <class Pair>
inline CREF(rdo::intrusive_ptr<typename Pair::Value>) RDOValue::getPointerByType() const
{
#ifdef _DEBUG
	rdo::intrusive_ptr<typename Pair::Type> pType = type().object_dynamic_cast<typename Pair::Type>();
	ASSERT(pType);
#endif // _DEBUG

	return getPointer<typename Pair::Value>();
}

template <class T>
inline CREF(rdo::intrusive_ptr<typename T::value_type>) RDOValue::getPointerByInterface() const
{
#ifdef _DEBUG
	rdo::interface_ptr<T> pType = type().interface_dynamic_cast<T>();
	ASSERT(pType);
#endif // _DEBUG

	return getPointer<typename T::value_type>();
}

template <class T>
inline rbool RDOValue::isType() const
{
	return type().object_dynamic_cast<T>();
}

template <class T>
inline REF(T) RDOValue::__get()
{
	return *reinterpret_cast<PTR(T)>(&m_value);
}

template <class T>
inline CREF(T) RDOValue::__get() const
{
	return *reinterpret_cast<CPTR(T)>(&m_value);
}

CLOSE_RDO_RUNTIME_NAMESPACE
