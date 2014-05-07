/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_value.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_VALUE_H_
#define _RDOPARSER_VALUE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr_interface_wrapper.h"
#include "utils/src/stuff/explicit_value.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOValue);
class RDOValue
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOValue);
public:
	const LPTypeInfo&              typeInfo() const;
	rdo::runtime::RDOType::TypeID  typeID  () const;
	 rdo::runtime::RDOValue&       value   ();
	const rdo::runtime::RDOValue&  value   () const;

	bool defined () const;
	bool constant() const;

	template <class T>
	const rdo::intrusive_ptr<T>& get() const;

	static LPRDOValue getIdentificator(const std::string& identificator);
	static LPRDOValue getUnknow(const RDOParserSrcInfo& src_info);

private:
	// Неопределенный тип
	RDOValue();

	// Для t_identificator известно только имя, но не тип
	RDOValue(const RDOParserSrcInfo& src_info);

	// От копии
	RDOValue(const LPRDOValue& pValue);

	// Он стандартных типов
	RDOValue(const rdo::explicit_value<int>& value, const RDOParserSrcInfo& src_info);
	RDOValue(const rdo::explicit_value<std::size_t>& value, const RDOParserSrcInfo& src_info);
	RDOValue(const rdo::explicit_value<double>& value, const RDOParserSrcInfo& src_info);
	RDOValue(const rdo::explicit_value<std::string>& value, const RDOParserSrcInfo& src_info);

	// От типа
	RDOValue(const LPTypeInfo& pType);
	RDOValue(const LPTypeInfo& pType, const RDOParserSrcInfo& src_info);

	// От типа и объекта
	template <class T>
	RDOValue(const LPTypeInfo& pType, const rdo::intrusive_ptr<T>& pObject);
	template <class T>
	RDOValue(const LPTypeInfo& pType, const rdo::intrusive_ptr<T>& pObject, const RDOParserSrcInfo& src_info);

	// От рантайм RDOValue
	RDOValue(const rdo::runtime::RDOValue& value, const RDOParserSrcInfo& src_info, const LPTypeInfo& pType);

 	template <class T>
	void setPointer(const rdo::intrusive_ptr<T>& pObject);

	//! Тип контейнера значения, размер определяется по максимальному размеру типа данных
	typedef unsigned char Value[sizeof(rdo::intrusive_ptr_interface_wrapper<RDOValue>)];

	Value                  m_buffer;
	rdo::runtime::RDOValue m_value;
	LPTypeInfo             m_pType;

	template <class T>
	T& __get();

	template <class T>
	const T& __get() const;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdo_value-inl.h"

#endif // _RDOPARSER_VALUE_H_
