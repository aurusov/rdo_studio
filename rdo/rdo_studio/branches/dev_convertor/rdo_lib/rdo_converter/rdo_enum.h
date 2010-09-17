/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCONVERTER_ENUM_H_
#define _RDOCONVERTER_ENUM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_type.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_enum.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
	void add(CREF(RDOValue) next);

	CREF(rdoRuntime::RDOEnumType) getEnums() const
	{
		return *static_cast<CPTR(rdoRuntime::RDOEnumType)>(m_type);
	}

	rbool operator== (CREF(RDOEnumType) enums) const
	{
		return __enum()->getValues() == enums.__enum()->getValues();
	}
	rbool operator!= (CREF(RDOEnumType) enums) const
	{
		return !operator==(enums);
	}

	DECLARE_IType;
	DECLARE_IModelStructure;

private:
	RDOEnumType         ();
	RDOEnumType         (CREF(rdoRuntime::RDOEnumType) enumType);
	virtual ~RDOEnumType();

	PTR(rdoRuntime::RDOEnumType) __enum() const { return static_cast<PTR(rdoRuntime::RDOEnumType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type)); }
};
DECLARE_POINTER(RDOEnumType);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _RDOCONVERTER_ENUM_H_
