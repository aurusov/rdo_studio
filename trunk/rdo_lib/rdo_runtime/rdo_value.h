#ifndef RDO_VALUE_H
#define RDO_VALUE_H

#include "rdo_type.h"
#include "rdo_enum.h"
#include "rdo_exception.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDORuntime;
class RDOFuzzyValue;

class RDOValue
{
public:
	RDOValue(): m_type( &g_unknow )
	{
	}
	~RDOValue()
	{
		switch ( typeID() )
		{
			case RDOType::t_string       :
			case RDOType::t_identificator: {
				delete m_value.s_value;
				break;
			}
		}
	}
	RDOValue( const RDOValue& rdovalue ):
		m_type ( rdovalue.m_type  ),
		m_value( rdovalue.m_value )
	{
		switch ( typeID() )
		{
			case RDOType::t_string       :
			case RDOType::t_identificator: {
				m_value.s_value = new std::string( *rdovalue.m_value.s_value );
				break;
			}
		}
	}
	RDOValue( const RDOType& type ):
		m_type( &type )
	{
		switch ( typeID() ) {
			case RDOType::t_unknow       : break;
			case RDOType::t_int          : m_value.i_value = 0; break;
			case RDOType::t_real         : m_value.d_value = 0; break;
			case RDOType::t_enum         : m_value.i_value = 0; break;
			case RDOType::t_string       : m_value.s_value = new std::string(""); break;
			case RDOType::t_identificator: m_value.s_value = new std::string(""); break;
			default                      : throw RDOValueException();
		}
	}
	RDOValue( int value ):
		m_type( &g_int )
	{
		m_value.i_value = value;
	}
	RDOValue( unsigned int value ):
		m_type( &g_int )
	{
		m_value.i_value = value;
	}
	RDOValue( double value ):
		m_type( &g_real )
	{
		m_value.d_value = value;
	}
	RDOValue( RDOEnumType& enums ):
		m_type( &enums )
	{
		if ( enums.empty() )
		{
			RDOValueException();
		}
		m_value.i_value = enums.findEnum( enums.getValues()[0] );
	}
	RDOValue( const RDOEnumType& enums, const std::string& value ):
		m_type( &enums )
	{
		m_value.i_value = enums.findEnum( value );
		if ( m_value.i_value == RDOEnumType::END )
		{
			RDOValueException();
		}
	}
	RDOValue( RDOFuzzyValue& fuzzy ):
		m_type( (RDOType*)&fuzzy )
	{
	}

	RDOValue( const std::string& value ):
		m_type( &g_string )
	{
		m_value.s_value = new std::string( value );
	}
	RDOValue( const std::string& value, const RDOType& type ):
		m_type( &g_identificator )
	{
		if ( type.id() != RDOType::t_identificator )
		{
			RDOValueException();
		}
		m_value.s_value = new std::string( value );
	}

	int getInt() const
	{
		switch ( typeID() )
		{
			case RDOType::t_int : return m_value.i_value;
			case RDOType::t_real: return (int)m_value.d_value;
			case RDOType::t_enum: return m_value.i_value;
		}
		throw RDOValueException();
	}
	int getEnumAsInt() const
	{
		switch ( typeID() )
		{
			case RDOType::t_int : return m_value.i_value;
			case RDOType::t_real: return (int)m_value.d_value;
			case RDOType::t_enum: return m_value.i_value;
		}
		throw RDOValueException();
	}
	const RDOEnumType& getEnum() const
	{
		switch ( typeID() )
		{
			case RDOType::t_enum: return *__enum();
		}
		throw RDOValueException();
	}
	double getDouble() const
	{
		switch ( typeID() )
		{
			case RDOType::t_int : return m_value.i_value;
			case RDOType::t_real: return m_value.d_value;
			case RDOType::t_enum: return m_value.i_value;
		}
		throw RDOValueException();
	}
	bool getBool() const
	{
		switch ( typeID() )
		{
			case RDOType::t_int   : return m_value.i_value           ? true : false;
			case RDOType::t_real  : return m_value.d_value           ? true : false;
			case RDOType::t_enum  : return m_value.i_value           ? true : false;
			case RDOType::t_string: return !m_value.s_value->empty() ? true : false;
		}
		throw RDOValueException();
	}
	const std::string& getString() const
	{
		switch ( typeID() )
		{
			case RDOType::t_string: return *m_value.s_value;
		}
		throw RDOValueException();
	}
	const std::string& getIdentificator() const
	{
		switch ( typeID() )
		{
			case RDOType::t_identificator: return *m_value.s_value;
		}
		throw RDOValueException();
	}
	std::string getAsString() const
	{
		switch ( typeID() )
		{
			case RDOType::t_int          : return rdo::format( "%d", m_value.i_value );
			case RDOType::t_real         : return rdo::toString( m_value.d_value );
			case RDOType::t_enum         : return __enum()->getValues().at( m_value.i_value );
			case RDOType::t_string       : return *m_value.s_value;
			case RDOType::t_identificator: return *m_value.s_value;
		}
		throw RDOValueException();
	}
	std::string getAsStringForTrace() const
	{
		switch ( typeID() )
		{
			case RDOType::t_int   : return rdo::format( "%d", m_value.i_value );
			case RDOType::t_real  : return rdo::toString( m_value.d_value );
			case RDOType::t_enum  : return rdo::format( "%d", m_value.i_value );
			case RDOType::t_string: return *m_value.s_value;
		}
		throw RDOValueException();
	}
	RDOValue& operator= ( const RDOValue& rdovalue )
	{
		switch ( typeID() )
		{
			case RDOType::t_string       :
			case RDOType::t_identificator:
			{
				delete m_value.s_value;
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
				m_value.s_value = new std::string( *rdovalue.m_value.s_value );
				break;
			}
		}
		return *this;
	}
	bool operator== ( const RDOValue& rdovalue ) const
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
			case RDOType::t_enum:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_enum: return m_type == rdovalue.m_type && m_value.i_value == rdovalue.m_value.i_value;
				}
				break;
			}
			case RDOType::t_string:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_string: return *m_value.s_value == *rdovalue.m_value.s_value;
				}
				break;
			}
			case RDOType::t_identificator:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_identificator: return *m_value.s_value == *rdovalue.m_value.s_value;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	bool operator!= ( const RDOValue& rdovalue ) const
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
			case RDOType::t_enum:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_enum: return m_value.i_value != rdovalue.m_value.i_value;
				}
				break;
			}
			case RDOType::t_string:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_string: return *m_value.s_value != *rdovalue.m_value.s_value;
				}
				break;
			}
			case RDOType::t_identificator:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_identificator: return *m_value.s_value != *rdovalue.m_value.s_value;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	bool operator< ( const RDOValue& rdovalue ) const
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
			case RDOType::t_string:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_string: return *m_value.s_value < *rdovalue.m_value.s_value;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	bool operator< ( const RDOValue& rdovalue )
	{
		return static_cast<const RDOValue*>(this)->operator< ( rdovalue );
	}
	bool operator> ( const RDOValue& rdovalue ) const
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
			case RDOType::t_string:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_string: return *m_value.s_value > *rdovalue.m_value.s_value;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	bool operator<= ( const RDOValue& rdovalue ) const
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
			case RDOType::t_string:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_string: return *m_value.s_value <= *rdovalue.m_value.s_value;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	bool operator>= ( const RDOValue& rdovalue ) const
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
			case RDOType::t_string:
			{
				switch ( rdovalue.typeID() )
				{
					case RDOType::t_string: return *m_value.s_value >= *rdovalue.m_value.s_value;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	RDOValue operator- () const
	{
		RDOValue rdovalue( *this );
		switch ( typeID() )
		{
			case RDOType::t_int : rdovalue.m_value.i_value = -m_value.i_value; break;
			case RDOType::t_real: rdovalue.m_value.d_value = -m_value.d_value; break;
			default             : throw RDOValueException();
		}
		return rdovalue;
	}
	void operator+= ( const RDOValue& rdovalue )
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
					case RDOType::t_string: *m_value.s_value += *rdovalue.m_value.s_value; return;
				}
				break;
			}
		}
		throw RDOValueException();
	}
	void operator-= ( const RDOValue& rdovalue )
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
		}
		throw RDOValueException();
	}
	void operator*= ( const RDOValue& rdovalue )
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
		}
		throw RDOValueException();
	}
	void operator/= ( const RDOValue& rdovalue )
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
		}
		throw RDOValueException();
	}
	RDOValue operator+ ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 += rdovalue;
		return value2;
	}
	RDOValue operator- ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 -= rdovalue;
		return value2;
	}
	RDOValue operator* ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 *= rdovalue;
		return value2;
	}
	RDOValue operator/ ( const RDOValue& rdovalue ) const
	{
		RDOValue value2( *this );
		value2 /= rdovalue;
		return value2;
	}

	const RDOType&  type() const   { return *m_type;     }
	RDOType::ID     typeID() const { return type().id(); }

private:
	const RDOType* m_type;

	const RDOEnumType*   __enum()   const { return static_cast<const RDOEnumType*>(m_type);                     }
	const RDOFuzzyValue* __fuzzy()  const { return reinterpret_cast<const RDOFuzzyValue*>(m_type);          }

	union {
		int          i_value;
		double       d_value;
		std::string* s_value;
	} m_value;
};

inline std::ostream& operator<< ( std::ostream& stream, const RDOValue& rdovalue )
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

} // namespace rdoRuntime

#endif // RDO_VALUE_H
