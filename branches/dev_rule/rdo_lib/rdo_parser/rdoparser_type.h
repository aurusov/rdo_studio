#ifndef RDOPARSER_TYPE_H
#define RDOPARSER_TYPE_H

#include "rdoparser_object.h"
#include <rdo_type.h>

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType
{
public:
	RDOType( const rdoRuntime::RDOType& type ):
		m_type( &type )
	{
	}
	const rdoRuntime::RDOType&        type() const { return *m_type; }
	const rdoRuntime::RDOType* operator-> () const { return  m_type; }

	virtual std::string    name() const = 0;
	virtual const RDOType* cast( const RDOType& toType ) const = 0;

	static const RDOType& getTypeByID( rdoRuntime::RDOType::ID id );

protected:
	const rdoRuntime::RDOType* m_type;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER( Class ) \
class RDOType__##Class: public RDOType \
{ \
public: \
	RDOType__##Class(): RDOType(rdoRuntime::g_##Class) {} \
	virtual std::string    name() const { return "" #Class ""; } \
	virtual const RDOType* cast( const RDOType& toType ) const; \
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
