#include "rdo_enum.h"
#include "rdo_fuzzy_def.h"
#include "rdo_exception.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
inline RDOValue::RDOValue(): m_type( &g_unknow )
{
}

inline RDOValue::~RDOValue()
{
	switch ( typeID() )
	{
		case RDOType::t_string       :
		case RDOType::t_identificator:
		{
			delete &__stringV();
			break;
		}
		case RDOType::t_fuzzy:
		{
			delete &__fuzzyV();
			break;
		}
	}
}

inline RDOValue::RDOValue( const RDOValue& rdovalue ):
	m_type ( rdovalue.m_type  ),
	m_value( rdovalue.m_value )
{
	switch ( typeID() )
	{
		case RDOType::t_string       :
		case RDOType::t_identificator:
		{
			m_value.s_value = new std::string( rdovalue.__stringV() );
			break;
		}
		case RDOType::t_fuzzy:
		{
			m_value.p_data = new RDOFuzzyValue( rdovalue.__fuzzyV() );
			break;
		}
	}
}

inline RDOValue::RDOValue( const RDOType& type ):
	m_type( &type )
{
	switch ( typeID() )
	{
		case RDOType::t_unknow       : break;
		case RDOType::t_int          : m_value.i_value = 0; break;
		case RDOType::t_real         : m_value.d_value = 0; break;
		case RDOType::t_enum         : m_value.i_value = 0; break;
		case RDOType::t_bool         : m_value.b_value = false; break;
		case RDOType::t_string       : m_value.s_value = new std::string(""); break;
		case RDOType::t_identificator: m_value.s_value = new std::string(""); break;
		default                      : throw RDOValueException();
	}
}

inline RDOValue::RDOValue( int value ):
	m_type( &g_int )
{
	m_value.i_value = value;
}

inline RDOValue::RDOValue( unsigned int value ):
	m_type( &g_int )
{
	m_value.i_value = value;
}

inline RDOValue::RDOValue( double value ):
	m_type( &g_real )
{
	m_value.d_value = value;
}

inline RDOValue::RDOValue( bool value ):
	m_type( &g_bool )
{
	m_value.b_value = value;
}

inline RDOValue::RDOValue( RDOEnumType& enums ):
	m_type( &enums )
{
	if ( enums.empty() )
	{
		RDOValueException();
	}
	m_value.i_value = enums.findEnum( enums.getValues()[0] );
}

inline RDOValue::RDOValue( const RDOEnumType& enums, const std::string& value ):
	m_type( &enums )
{
	m_value.i_value = enums.findEnum( value );
	if ( m_value.i_value == RDOEnumType::END )
	{
		RDOValueException();
	}
}

inline RDOValue::RDOValue( const RDOFuzzyValue& fuzzy ):
	m_type( &fuzzy.type() )
{
	m_value.p_data = new RDOFuzzyValue( fuzzy );
}

inline RDOValue::RDOValue( const std::string& value ):
	m_type( &g_string )
{
	m_value.s_value = new std::string( value );
}

inline RDOValue::RDOValue( const std::string& value, const RDOType& type ):
	m_type( &g_identificator )
{
	if ( type.id() != RDOType::t_identificator )
	{
		RDOValueException();
	}
	m_value.s_value = new std::string( value );
}

inline int RDOValue::getInt() const
{
	switch ( typeID() )
	{
		case RDOType::t_int  : return m_value.i_value;
		case RDOType::t_real : return (int)m_value.d_value;
		case RDOType::t_enum : return m_value.i_value;
		case RDOType::t_bool : return m_value.b_value ? 1 : 0;
		case RDOType::t_fuzzy: return const_cast<RDOValue*>(this)->__fuzzyV().defuzzyfication().getInt();
	}
	throw RDOValueException();
}

inline int RDOValue::getEnumAsInt() const
{
	switch ( typeID() )
	{
		case RDOType::t_int : return m_value.i_value;
		case RDOType::t_real: return (int)m_value.d_value;
		case RDOType::t_enum: return m_value.i_value;
		case RDOType::t_bool: return m_value.b_value ? 1 : 0;
	}
	throw RDOValueException();
}

inline const RDOEnumType& RDOValue::getEnum() const
{
	switch ( typeID() )
	{
		case RDOType::t_enum: return __enumT();
	}
	throw RDOValueException();
}

inline double RDOValue::getDouble() const
{
	switch ( typeID() )
	{
		case RDOType::t_int : return m_value.i_value;
		case RDOType::t_real: return m_value.d_value;
		case RDOType::t_enum: return m_value.i_value;
		case RDOType::t_bool: return m_value.b_value ? 1 : 0;
	}
	throw RDOValueException();
}

inline bool RDOValue::getBool() const
{
	switch ( typeID() )
	{
		case RDOType::t_bool  : return m_value.b_value;
	}
	throw RDOValueException();
}

inline bool RDOValue::getAsBool() const
{
	switch ( typeID() )
	{
		case RDOType::t_int   : return m_value.i_value      ? true : false;
		case RDOType::t_real  : return m_value.d_value      ? true : false;
		case RDOType::t_enum  : return m_value.i_value      ? true : false;
		case RDOType::t_string: return !__stringV().empty() ? true : false;
		case RDOType::t_bool  : return m_value.b_value;
	}
	throw RDOValueException();
}

inline const std::string& RDOValue::getString() const
{
	switch ( typeID() )
	{
		case RDOType::t_string: return __stringV();
	}
	throw RDOValueException();
}

inline const std::string& RDOValue::getIdentificator() const
{
	switch ( typeID() )
	{
		case RDOType::t_identificator: return __stringV();
	}
	throw RDOValueException();
}

inline std::string RDOValue::getAsString() const
{
	switch ( typeID() )
	{
		case RDOType::t_int          : return rdo::format( "%d", m_value.i_value );
		case RDOType::t_real         : return rdo::toString( m_value.d_value );
		case RDOType::t_enum         : return __enumT().getValues().at( m_value.i_value );
		case RDOType::t_bool         : return m_value.b_value ? "true" : "false";
		case RDOType::t_string       : return __stringV();
		case RDOType::t_identificator: return __stringV();
		case RDOType::t_fuzzy        : return __fuzzyV().getAsString();
	}
	throw RDOValueException();
}

inline std::string RDOValue::getAsStringForTrace() const
{
	switch ( typeID() )
	{
		case RDOType::t_int   : return rdo::format( "%d", m_value.i_value );
		case RDOType::t_real  : return rdo::toString( m_value.d_value );
		case RDOType::t_enum  : return rdo::format( "%d", m_value.i_value );
		case RDOType::t_bool  : return m_value.b_value ? "true" : "false";
		case RDOType::t_string: return __stringV();
	}
	throw RDOValueException();
}

inline RDOValue& RDOValue::operator= ( const RDOValue& rdovalue )
{
	switch ( typeID() )
	{
		case RDOType::t_string       :
		case RDOType::t_identificator:
		{
			delete &__stringV();
			break;
		}
	}
	m_type  = rdovalue.m_type;
	m_value = rdovalue.m_value;
	switch ( typeID() )
	{
		case RDOType::t_string       :
		case RDOType::t_identificator:
		{
			m_value.s_value = new std::string( rdovalue.__stringV() );
			break;
		}
	}
	return *this;
}

inline bool RDOValue::operator== ( const RDOValue& rdovalue ) const
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.i_value == rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.i_value == rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.d_value  == rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.d_value  == rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value == rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_enum:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_enum: if (m_type == rdovalue.m_type) return m_value.i_value == rdovalue.m_value.i_value; break;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: return __stringV() == rdovalue.__stringV();
			}
			break;
		}
		case RDOType::t_identificator:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_identificator: return __stringV() == rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline bool RDOValue::operator!= ( const RDOValue& rdovalue ) const
{
	switch ( typeID() ) 
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.i_value != rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.i_value != rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.d_value != rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.d_value != rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value != rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_enum:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_enum: if (m_type == rdovalue.m_type) return m_value.i_value != rdovalue.m_value.i_value; break;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: return __stringV() != rdovalue.__stringV();
			}
			break;
		}
		case RDOType::t_identificator:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_identificator: return __stringV() != rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline bool RDOValue::operator< ( const RDOValue& rdovalue ) const
{
	switch ( typeID() ) {
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.i_value < rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.i_value < rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.d_value < rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.d_value < rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value < rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: return __stringV() < rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline bool RDOValue::operator< ( const RDOValue& rdovalue )
{
	return static_cast<const RDOValue*>(this)->operator< ( rdovalue );
}

inline bool RDOValue::operator> ( const RDOValue& rdovalue ) const
{
	switch ( typeID() ) {
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.i_value > rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.i_value > rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.d_value > rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.d_value > rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value > rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: return __stringV() > rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline bool RDOValue::operator<= ( const RDOValue& rdovalue ) const
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.i_value <= rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.i_value <= rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.d_value <= rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.d_value <= rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value <= rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: return __stringV() <= rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline bool RDOValue::operator>= ( const RDOValue& rdovalue ) const
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.i_value >= rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.i_value >= rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : return m_value.d_value >= rdovalue.m_value.i_value;
				case RDOType::t_real: return m_value.d_value >= rdovalue.m_value.d_value;
			}
			break;
		}
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value >= rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: return __stringV() >= rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator&& ( const RDOValue& rdovalue ) const
{
	switch ( typeID() )
	{
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value && rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_fuzzy:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_fuzzy: return __fuzzyV() && rdovalue.__fuzzyV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator|| ( const RDOValue& rdovalue ) const
{
	switch ( typeID() )
	{
		case RDOType::t_bool:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_bool: return m_value.b_value || rdovalue.m_value.b_value;
			}
			break;
		}
		case RDOType::t_fuzzy:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_fuzzy: return __fuzzyV() || rdovalue.__fuzzyV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator- () const
{
	RDOValue rdovalue( *this );
	switch ( typeID() )
	{
		case RDOType::t_int  : rdovalue.m_value.i_value = -m_value.i_value; break;
		case RDOType::t_real : rdovalue.m_value.d_value = -m_value.d_value; break;
		case RDOType::t_bool : rdovalue.m_value.b_value = !m_value.b_value; break;
		case RDOType::t_fuzzy: rdovalue.__fuzzyV()      = __fuzzyV().u_minus(); break;
		default              : throw RDOValueException();
	}
	return rdovalue;
}

inline void RDOValue::operator+= ( const RDOValue& rdovalue )
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.i_value += rdovalue.getInt(); return;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.d_value += rdovalue.getDouble(); return;
			}
			break;
		}
		case RDOType::t_string:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_string: __stringV() += rdovalue.__stringV(); return;
			}
			break;
		}
		case RDOType::t_fuzzy:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() + rdovalue.__fuzzyV(); return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline void RDOValue::operator-= ( const RDOValue& rdovalue )
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.i_value -= rdovalue.getInt(); return;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.d_value -= rdovalue.getDouble(); return;
			}
			break;
		}
		case RDOType::t_fuzzy:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() - rdovalue.__fuzzyV(); return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline void RDOValue::operator*= ( const RDOValue& rdovalue )
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int : m_value.i_value *= rdovalue.m_value.i_value; return;
				case RDOType::t_real: m_value.d_value = ((double)m_value.i_value) * rdovalue.m_value.d_value; m_type = &g_real; return;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.d_value *= rdovalue.getDouble(); return;
			}
			break;
		}
		case RDOType::t_fuzzy:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() * rdovalue.__fuzzyV(); return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline void RDOValue::operator/= ( const RDOValue& rdovalue )
{
	switch ( typeID() )
	{
		case RDOType::t_int:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.d_value = ((double)m_value.i_value) / rdovalue.getDouble(); m_type = &g_real; return;
			}
			break;
		}
		case RDOType::t_real:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_int :
				case RDOType::t_real: m_value.d_value = m_value.d_value / rdovalue.getDouble(); return;
			}
			break;
		}
		case RDOType::t_fuzzy:
		{
			switch ( rdovalue.typeID() )
			{
				case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() / rdovalue.__fuzzyV(); return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator+ ( const RDOValue& rdovalue ) const
{
	RDOValue value2( *this );
	value2 += rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator- ( const RDOValue& rdovalue ) const
{
	RDOValue value2( *this );
	value2 -= rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator* ( const RDOValue& rdovalue ) const
{
	RDOValue value2( *this );
	value2 *= rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator/ ( const RDOValue& rdovalue ) const
{
	RDOValue value2( *this );
	value2 /= rdovalue;
	return value2;
}

inline const RDOType& RDOValue::type() const
{
	return *m_type;
}

inline RDOType::ID RDOValue::typeID() const
{
	return type().id();
}

inline const RDOEnumType& RDOValue::__enumT() const
{
	return *static_cast<const RDOEnumType*>(m_type);
}

inline std::string& RDOValue::__stringV()
{
	return *m_value.s_value;
}

inline const std::string& RDOValue::__stringV() const
{
	return *m_value.s_value;
}

inline RDOFuzzyValue& RDOValue::__fuzzyV()
{
	return *static_cast<RDOFuzzyValue*>(m_value.p_data);
}

inline const RDOFuzzyValue& RDOValue::__fuzzyV() const
{
	return *static_cast<const RDOFuzzyValue*>(m_value.p_data);
}

inline std::ostream& operator<< ( std::ostream& stream, const RDOValue& rdovalue )
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

} // namespace rdoRuntime
