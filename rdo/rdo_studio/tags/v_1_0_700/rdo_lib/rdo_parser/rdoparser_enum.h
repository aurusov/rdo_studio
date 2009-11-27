#ifndef RDOPARSER_ENUM_H
#define RDOPARSER_ENUM_H

#include "rdoparser_type.h"
#include "rdoparser_object.h"
#include "rdoparser_value.h"
#include <rdo_enum.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOType, public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPEnum(CPTR(RDOParserObject) _parent, CREF(RDOValue) first);
	virtual ~RDORTPEnum();

	void add(CREF(RDOValue) next);
	rdoRuntime::RDOValue          findEnumValueWithThrow(CREF(RDOParserSrcInfo) val_src_info, CREF(tstring) val) const;
	rdoRuntime::RDOValue          getFirstValue() const;
	CREF(rdoRuntime::RDOEnumType) getEnums     () const { return *static_cast<const rdoRuntime::RDOEnumType*>(m_type); }

	bool operator== (CREF(RDORTPEnum) enums) const
	{
		return __enum()->getValues() == enums.__enum()->getValues();
	}
	bool operator!= (CREF(RDORTPEnum) enums) const
	{
		return !operator==(enums);
	}

	virtual tstring              name()                                const { return _T("enum"); }
	virtual CPTR(RDOType)        cast(CREF(RDOType) toType)            const;
	virtual rdoRuntime::RDOValue cast(CREF(rdoRuntime::RDOValue) from) const;

private:
	PTR(rdoRuntime::RDOEnumType) __enum() const { return static_cast<PTR(rdoRuntime::RDOEnumType)>(const_cast<PTR(rdoRuntime::RDOType)>(m_type)); }
};

} // namespace rdoParse

#endif // RDOPARSER_ENUM_H
