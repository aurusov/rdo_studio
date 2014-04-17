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
	const LPRDOType& type() const;
	rdo::runtime::RDOType::TypeID typeID() const;
	const rdo::runtime::RDOValue& value() const;
	const rdo::runtime::RDOValue* operator->() const;

	bool defined() const;
	bool constant() const;

	static LPRDOValue getIdentificator(const std::string& identificator);

private:
	explicit RDOValue(const int& value, const RDOParserSrcInfo& src_info);
	explicit RDOValue(const std::size_t& value, const RDOParserSrcInfo& src_info);
	explicit RDOValue(const double& value, const RDOParserSrcInfo& src_info);
	explicit RDOValue(const std::string& value, const RDOParserSrcInfo& src_info);

	explicit RDOValue(const rdo::runtime::RDOValue& value, const LPRDOType& type, const RDOParserSrcInfo& src_info);
	         RDOValue(const LPRDOValue& pValue);
	         RDOValue(const LPRDOType& type, const RDOParserSrcInfo& src_info = RDOParserSrcInfo());
	// Для t_identificator известно только имя, но не тип
	explicit RDOValue(const RDOParserSrcInfo& src_info);
	// Неопределенный тип
	         RDOValue();

	rdo::runtime::RDOValue m_value;
	LPRDOType              m_type;
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#include "converter/smr2rdox/rdo_value.inl"

#endif // _CONVERTOR_RDOCONVERTER_VALUE_H_
