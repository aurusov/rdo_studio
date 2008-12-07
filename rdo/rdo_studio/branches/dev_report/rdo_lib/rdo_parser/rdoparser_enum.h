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
	RDORTPEnum( const RDOParserObject* _parent, const RDOValue& first );
	virtual ~RDORTPEnum();

	void add( const RDOValue& next );
	rdoRuntime::RDOValue     findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const;
	rdoRuntime::RDOValue     getFirstValue() const;
	const rdoRuntime::RDOEnumType& getEnums() const { return *static_cast<const rdoRuntime::RDOEnumType*>(m_type); }

	bool operator== ( const RDORTPEnum& enums ) const
	{
		return __enum()->getValues() == enums.__enum()->getValues();
	}
	bool operator!= ( const RDORTPEnum& enums ) const
	{
		return !operator==( enums );
	}

	virtual std::string    name() const { return "enum"; }
	virtual const RDOType* cast( const RDOType& toType ) const;

private:
	rdoRuntime::RDOEnumType* __enum() const { return static_cast<rdoRuntime::RDOEnumType*>(const_cast<rdoRuntime::RDOType*>(m_type)); }
};

} // namespace rdoParse

#endif // RDOPARSER_ENUM_H
