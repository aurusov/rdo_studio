#ifndef RDOPARSER_TYPE_H
#define RDOPARSER_TYPE_H

#include "rdoparser_object.h"
#include <rdo_type.h>
#include <rdo_value.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType
{
public:
	RDOType(CREF(rdoRuntime::RDOType) type)
		: m_type(&type)
	{}
	CREF(rdoRuntime::RDOType)        type() const { return *m_type; }
	CPTR(rdoRuntime::RDOType) operator-> () const { return  m_type; }

	virtual tstring              name()                                const = 0;
	virtual CPTR(RDOType)        cast(CREF(RDOType) toType)            const = 0;
	virtual rdoRuntime::RDOValue cast(CREF(rdoRuntime::RDOValue) from) const = 0;

	static CREF(RDOType) getTypeByID(rdoRuntime::RDOType::TypeID typeID);

protected:
	CPTR(rdoRuntime::RDOType) m_type;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER( Class ) \
class RDOType__##Class: public RDOType \
{ \
public: \
	RDOType__##Class(): RDOType(rdoRuntime::g_##Class) {} \
	virtual tstring              name() const { return "" #Class ""; } \
	virtual const RDOType*       cast(CREF(RDOType) toType) const; \
	virtual rdoRuntime::RDOValue cast(CREF(rdoRuntime::RDOValue) from) const; \
}; \
extern RDOType__##Class g_##Class;

DEFINE_ATOM_TYPE_PARSER( unknow        );
DEFINE_ATOM_TYPE_PARSER( identificator );
DEFINE_ATOM_TYPE_PARSER( int           );
DEFINE_ATOM_TYPE_PARSER( real          );
DEFINE_ATOM_TYPE_PARSER( bool          );
DEFINE_ATOM_TYPE_PARSER( string        );

} // namespace rdoParse

#endif // RDOPARSER_TYPE_H
