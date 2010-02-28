/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_ENUM_H_
#define _RDOPARSER_ENUM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_enum.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
class RDOEnumType: public RDOType
{
DECLARE_FACTORY(RDOEnumType);
public:
	void add(CREF(RDOValue) next);
//	rdoRuntime::RDOValue          findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const;
//	rdoRuntime::RDOValue          getFirstValue() const;
	CREF(rdoRuntime::RDOEnumType) getEnums     () const { return *static_cast<CPTR(rdoRuntime::RDOEnumType)>(m_type); }

	rbool operator== (CREF(RDOEnumType) enums) const
	{
		return __enum()->getValues() == enums.__enum()->getValues();
	}
	rbool operator!= (CREF(RDOEnumType) enums) const
	{
		return !operator==(enums);
	}
	virtual tstring name() const;

private:
	RDOEnumType         ();
	virtual ~RDOEnumType();

	PTR(rdoRuntime::RDOEnumType) __enum() const { return static_cast<PTR(rdoRuntime::RDOEnumType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type)); }

	virtual CPTR(RDOType)        type_cast (CREF(RDOType)              from) const;
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const;
	DECLARE_IModelStructure;
};
typedef rdo::smart_ptr<RDOEnumType> LPRDOEnumType;

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ENUM_H_
