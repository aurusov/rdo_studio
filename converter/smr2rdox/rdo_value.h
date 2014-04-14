/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_value.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_VALUE_H_
#define _CONVERTOR_RDOCONVERTER_VALUE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_type.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

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
	CREF(LPRDOType)                     type() const;
	rdo::runtime::RDOType::TypeID     typeID() const;
	CREF(rdo::runtime::RDOValue)       value() const;
	CPTR(rdo::runtime::RDOValue) operator-> () const;

	bool defined() const;
	bool constant() const;

	static LPRDOValue getIdentificator(CREF(std::string) identificator);

private:
	explicit RDOValue(CREF(int) value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(std::size_t) value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(double) value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(std::string) value, CREF(RDOParserSrcInfo) src_info);

	explicit RDOValue(CREF(rdo::runtime::RDOValue) value, CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info);
	         RDOValue(CREF(LPRDOValue) pValue);
	         RDOValue(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo());
	// Для t_identificator известно только имя, но не тип
	explicit RDOValue(CREF(RDOParserSrcInfo) src_info);
	// Неопределенный тип
	         RDOValue();

	rdo::runtime::RDOValue m_value;
	LPRDOType              m_type;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#include "converter/smr2rdox/rdo_value.inl"

#endif // _CONVERTOR_RDOCONVERTER_VALUE_H_
