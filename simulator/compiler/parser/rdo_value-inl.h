// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

template <class T>
inline RDOValue::RDOValue(const LPTypeInfo& pType, const rdo::intrusive_ptr<T>& pObject)
	: RDOParserSrcInfo(pObject->src_info())
	, m_pType         (pType              )
{
	setPointer(pObject);
}

template <class T>
inline RDOValue::RDOValue(const LPTypeInfo& pType, const rdo::intrusive_ptr<T>& pObject, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_pType         (pType   )
{
	setPointer(pObject);
}

template <class T>
inline void RDOValue::setPointer(const rdo::intrusive_ptr<T>& pObject)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<T>) >= sizeof(double));

	ASSERT(m_pType);
	ASSERT(pObject);
	ASSERT(
		typeID() == rdo::runtime::RDOType::t_string        ||
		typeID() == rdo::runtime::RDOType::t_identificator ||
		typeID() == rdo::runtime::RDOType::t_pointer
	);

	new (&m_buffer) rdo::intrusive_ptr_interface_wrapper<T>(pObject);
	m_value = rdo::runtime::RDOValue(m_pType->type(), pObject->createRuntimeValue());
}

template <class T>
inline T& RDOValue::__get()
{
	return *reinterpret_cast<T*>(&m_buffer);
}

template <class T>
inline const T& RDOValue::__get() const
{
	return *reinterpret_cast<const T*>(&m_buffer);
}

template <class T>
inline const rdo::intrusive_ptr<T>& RDOValue::get() const
{
	ASSERT(
		typeID() == rdo::runtime::RDOType::t_string        ||
		typeID() == rdo::runtime::RDOType::t_identificator ||
		typeID() == rdo::runtime::RDOType::t_pointer
	);

	return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

CLOSE_RDO_PARSER_NAMESPACE
