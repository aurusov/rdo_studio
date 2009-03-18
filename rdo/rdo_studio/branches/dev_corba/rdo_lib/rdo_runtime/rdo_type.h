#ifndef RDO_TYPE_H
#define RDO_TYPE_H

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------
class RDOType
{
public:
	enum ID {
		t_unknow = 0,
		t_identificator,
		t_int,
		t_real,
		t_bool,
		t_string,
		t_enum,
		t_fuzzy
	};

	RDOType( ID id ):
		m_id( id )
	{
	}

	ID id() const { return m_id; }

	bool operator!= ( RDOType& type )
	{
		return m_id != type.m_id;
	}
	virtual std::string asString() const = 0;

	static const RDOType& getTypeByID( ID id );

private:
	ID m_id;
};

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE
// ----------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE( Class, ClassName ) \
class RDOType__##Class: public RDOType \
{ \
public: \
	RDOType__##Class(): RDOType(t_##Class) {} \
	virtual std::string asString() const { return ClassName; } \
}; \
extern RDOType__##Class g_##Class;

DEFINE_ATOM_TYPE( unknow,        "unknow"        );
DEFINE_ATOM_TYPE( identificator, "identificator" );
DEFINE_ATOM_TYPE( int,           "integer"       );
DEFINE_ATOM_TYPE( real,          "real"          );
DEFINE_ATOM_TYPE( bool,          "bool"          );
DEFINE_ATOM_TYPE( string,        "string"        );

} // namespace rdoRuntime

#endif // RDO_TYPE_H
