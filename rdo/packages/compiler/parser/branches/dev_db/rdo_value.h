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
#include "utils/smart_ptr/intrusive_ptr.h"
#include "utils/smart_ptr/intrusive_ptr_interface_wrapper.h"
#include "utils/explicit_value.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/headers/type/type_db_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
OBJECT(RDOValue) IS INSTANCE_OF(RDOParserSrcInfo)
	AND INSTANCE_OF(rdo::runtime::ISerializeTypeInDB)
{
DECLARE_FACTORY(RDOValue);
public:
	CREF(LPTypeInfo)               typeInfo() const;
	rdo::runtime::RDOType::TypeID  typeID  () const;
	 REF(rdo::runtime::RDOValue)   value   ();
	CREF(rdo::runtime::RDOValue)   value   () const;

	rbool defined () const;
	rbool constant() const;

	template <class T>
	CREF(rdo::intrusive_ptr<T>) get() const;

	static LPRDOValue getIdentificator(CREF(tstring)     identificator);
	static LPRDOValue getUnknow       (CREF(RDOParserSrcInfo) src_info);

	virtual void serializeInDB(REF(IDB) db) const;

private:
	// Неопределенный тип
	RDOValue();

	// Для t_identificator известно только имя, но не тип
	RDOValue(CREF(RDOParserSrcInfo) src_info);

	// От копии
	RDOValue(CREF(LPRDOValue) pValue);

	// Он стандартных типов
	RDOValue(CREF(rdo::explicit_value<rsint>)   value, CREF(RDOParserSrcInfo) src_info);
	RDOValue(CREF(rdo::explicit_value<ruint>)   value, CREF(RDOParserSrcInfo) src_info);
	RDOValue(CREF(rdo::explicit_value<double>)  value, CREF(RDOParserSrcInfo) src_info);
	RDOValue(CREF(rdo::explicit_value<tstring>) value, CREF(RDOParserSrcInfo) src_info);

	// От типа
	RDOValue(CREF(LPTypeInfo) pType);
	RDOValue(CREF(LPTypeInfo) pType, CREF(RDOParserSrcInfo) src_info);

	// От типа и объекта
	template <class T>
	RDOValue(CREF(LPTypeInfo) pType, CREF(rdo::intrusive_ptr<T>) pObject);
	template <class T>
	RDOValue(CREF(LPTypeInfo) pType, CREF(rdo::intrusive_ptr<T>) pObject, CREF(RDOParserSrcInfo) src_info);

	// От рантайм RDOValue
	RDOValue(CREF(rdo::runtime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType);

 	template <class T>
	void setPointer(CREF(rdo::intrusive_ptr<T>) pObject);

	//! Тип контейнера значения, размер определяется по максимальному размеру типа данных
	typedef rbyte Value[sizeof(rdo::intrusive_ptr_interface_wrapper<RDOValue>)];

	Value                  m_buffer;
	rdo::runtime::RDOValue m_value;
	LPTypeInfo             m_pType;

	template <class T>
	REF(T) __get();

	template <class T>
	CREF(T) __get() const;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdo_value.inl"

#endif // _RDOPARSER_VALUE_H_
