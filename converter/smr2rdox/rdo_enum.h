/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_enum.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_ENUM_H_
#define _CONVERTOR_RDOCONVERTER_ENUM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type.h"
#include "converter/smr2rdox/rdo_value.h"
#include "simulator/runtime/rdo_enum.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOEnumType);

class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
	void add(const LPRDOValue& pNext);

	rdo::runtime::LPRDOEnumType getEnums() const
	{
		return m_pType.object_static_cast<rdo::runtime::RDOEnumType>();
	}

	bool operator== (const RDOEnumType& pEnumType) const
	{
		return getEnums()->getValues() == pEnumType.getEnums()->getValues();
	}
	bool operator!= (const RDOEnumType& pEnumType) const
	{
		return !operator==(pEnumType);
	}

	DECLARE_ITypeConverter;
	DECLARE_IModelStructure;

private:
	RDOEnumType         ();
	RDOEnumType         (const rdo::runtime::LPRDOEnumType& pEnumType);
	virtual ~RDOEnumType();
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_ENUM_H_
