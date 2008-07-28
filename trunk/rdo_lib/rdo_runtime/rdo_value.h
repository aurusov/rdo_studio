#ifndef RDO_VALUE_H
#define RDO_VALUE_H

#include "rdo_type.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOType;
class RDOEnumType;
class RDOFuzzyValue;

class RDOValue
{
public:
	RDOValue();
	~RDOValue();
	RDOValue( const RDOValue&   rdovalue );
	RDOValue( const RDOType&       type  );
	RDOValue( int                  value );
	RDOValue( unsigned int         value );
	RDOValue( double               value );
	RDOValue( bool                 value );
	RDOValue( RDOEnumType&         enums );
	RDOValue( const RDOEnumType&   enums, const std::string& value );
	RDOValue( const RDOFuzzyValue& fuzzy );
	RDOValue( const std::string&   value );
	RDOValue( const std::string&   value, const RDOType& type );

	int                getInt()           const;
	int                getEnumAsInt()     const;
	const RDOEnumType& getEnum()          const;
	double             getDouble()        const;
	bool               getBool()          const;
	const std::string& getString()        const;
	const std::string& getIdentificator() const;

	bool        getAsBool()           const;
	std::string getAsString()         const;
	std::string getAsStringForTrace() const;

	RDOValue& operator=  ( const RDOValue& rdovalue );
	bool      operator== ( const RDOValue& rdovalue ) const;
	bool      operator!= ( const RDOValue& rdovalue ) const;
	bool      operator<  ( const RDOValue& rdovalue ) const;
	bool      operator<  ( const RDOValue& rdovalue );
	bool      operator>  ( const RDOValue& rdovalue ) const;
	bool      operator<= ( const RDOValue& rdovalue ) const;
	bool      operator>= ( const RDOValue& rdovalue ) const;
	RDOValue  operator&& ( const RDOValue& rdovalue ) const;
	RDOValue  operator|| ( const RDOValue& rdovalue ) const;
	RDOValue  operator-  () const;
	void      operator+= ( const RDOValue& rdovalue );
	void      operator-= ( const RDOValue& rdovalue );
	void      operator*= ( const RDOValue& rdovalue );
	void      operator/= ( const RDOValue& rdovalue );
	RDOValue  operator+  ( const RDOValue& rdovalue ) const;
	RDOValue  operator-  ( const RDOValue& rdovalue ) const;
	RDOValue  operator*  ( const RDOValue& rdovalue ) const;
	RDOValue  operator/  ( const RDOValue& rdovalue ) const;

	const RDOType&  type() const;
	RDOType::ID     typeID() const;

private:
	const RDOType* m_type;

	const RDOEnumType&   __enumT  () const;
	      std::string&   __stringV();
	const std::string&   __stringV() const;
	      RDOFuzzyValue& __fuzzyV ();
	const RDOFuzzyValue& __fuzzyV () const;

	union {
		int          i_value;
		double       d_value;
		bool         b_value;
		std::string* s_value;
		void*        p_data;
	} m_value;
};

} // namespace rdoRuntime

#include "rdo_value.inl"

#endif // RDO_VALUE_H
