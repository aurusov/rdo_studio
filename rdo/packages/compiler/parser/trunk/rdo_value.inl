/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_value.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.04.2010
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

template <class T>
inline RDOValue::RDOValue(CREF(LPTypeInfo) pType, CREF(rdo::intrusive_ptr<T>) pObject)
	: RDOParserSrcInfo(pObject->src_info())
	, m_pType         (pType              )
{
	setPointer(pObject);
}

template <class T>
inline RDOValue::RDOValue(CREF(LPTypeInfo) pType, CREF(rdo::intrusive_ptr<T>) pObject, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_pType         (pType   )
{
	setPointer(pObject);
}

template <class T>
inline void RDOValue::setPointer(CREF(rdo::intrusive_ptr<T>) pObject)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<T>) >= sizeof(double));

	ASSERT(m_pType);
	ASSERT(pObject);
	ASSERT(
		typeID() == rdoRuntime::RDOType::t_string        ||
		typeID() == rdoRuntime::RDOType::t_identificator ||
		typeID() == rdoRuntime::RDOType::t_pointer
	);

	new (&m_buffer) rdo::intrusive_ptr_interface_wrapper<T>(pObject);
	m_value = rdoRuntime::RDOValue(m_pType->type()->type(), pObject->createRuntimeValue());
}

template <class T>
inline REF(T) RDOValue::__get()
{
	return *reinterpret_cast<PTR(T)>(&m_buffer);
}

template <class T>
inline CREF(T) RDOValue::__get() const
{
	return *reinterpret_cast<CPTR(T)>(&m_buffer);
}

template <class T>
inline CREF(rdo::intrusive_ptr<T>) RDOValue::get() const
{
	ASSERT(
		typeID() == rdoRuntime::RDOType::t_string        ||
		typeID() == rdoRuntime::RDOType::t_identificator ||
		typeID() == rdoRuntime::RDOType::t_pointer
	);

	return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

CLOSE_RDO_PARSER_NAMESPACE
