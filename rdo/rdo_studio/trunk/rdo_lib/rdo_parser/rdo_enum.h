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
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_enum.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOTypeParam
{
public:
	RDORTPEnum(CPTR(RDOParserObject) parent, CREF(RDOValue) first);
	virtual ~RDORTPEnum();

	void add(CREF(RDOValue) next);
	rdoRuntime::RDOValue          findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const;
	rdoRuntime::RDOValue          getFirstValue() const;
	CREF(rdoRuntime::RDOEnumType) getEnums     () const { return *static_cast<CPTR(rdoRuntime::RDOEnumType)>(m_type); }

	rbool operator== (CREF(RDORTPEnum) enums) const
	{
		return __enum()->getValues() == enums.__enum()->getValues();
	}
	rbool operator!= (CREF(RDORTPEnum) enums) const
	{
		return !operator==(enums);
	}

	virtual tstring              name()                                const { return _T("enum"); }
	virtual CPTR(RDOType)        cast(CREF(RDOType) toType)            const;
	virtual rdoRuntime::RDOValue cast(CREF(rdoRuntime::RDOValue) from) const;

private:
	PTR(rdoRuntime::RDOEnumType) __enum() const { return static_cast<PTR(rdoRuntime::RDOEnumType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type)); }
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ENUM_H_
