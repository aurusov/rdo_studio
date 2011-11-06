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
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

PREDECLARE_POINTER(RDOArrayValue);

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
OBJECT(RDOValue) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOValue);
public:
	CREF(LPTypeInfo)             typeInfo() const;
	rdoRuntime::RDOType::TypeID  typeID  () const;
	CREF(rdoRuntime::RDOValue)   value   () const;

	rbool defined () const;
	rbool constant() const;

	template <class T>
	CREF(rdo::intrusive_ptr<T>) get() const;

	static LPRDOValue getIdentificator(CREF(tstring) identificator);

private:
	explicit RDOValue(CREF(rsint)   value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(ruint)   value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(double)  value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(tstring) value, CREF(RDOParserSrcInfo) src_info);

	template <class T>
	RDOValue(CREF(LPTypeInfo) pType, CREF(rdo::intrusive_ptr<T>) pObject);
	template <class T>
	RDOValue(CREF(LPTypeInfo) pType, CREF(rdo::intrusive_ptr<T>) pObject, CREF(RDOParserSrcInfo) src_info);

	explicit RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType);
	         RDOValue(CREF(LPRDOValue) pValue);
	         RDOValue(CREF(LPTypeInfo) pType);
	// Для t_identificator известно только имя, но не тип
	explicit RDOValue(CREF(RDOParserSrcInfo) src_info);
	// Неопределенный тип
	         RDOValue();

 	template <class T>
	void setPointer(CREF(rdo::intrusive_ptr<T>) pObject);

	//! Тип контейнера значения, размер определяется по максимальному размеру типа данных
	typedef rbyte Value[sizeof(rdo::intrusive_ptr_interface_wrapper<RDOValue>)];

	Value                m_buffer;
	rdoRuntime::RDOValue m_value;
	LPTypeInfo           m_pType;

	template <class T>
	REF(T) __get();

	template <class T>
	CREF(T) __get() const;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdo_value.inl"

#endif // _RDOPARSER_VALUE_H_
