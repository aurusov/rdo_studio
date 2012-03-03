#ifndef RDO_FUZZY_DEF_H
#define RDO_FUZZY_DEF_H

#ifndef RDO_VALUE_H
#error include "rdo_value.h" first (и вообще, вместо "rdo_fuzzy_def.h" лучше юзать "rdo_fuzzy.h")
#endif

//
// Напрямую этот файл инклюдить не надо, юзайте
// #include "rdo_fuzzy.h"
//

#include "rdo_type.h"
#include "rdoruntime_object.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValueItem
// ----------------------------------------------------------------------------
struct RDOFuzzyValueItem
{
	RDOValue m_rdovalue;
	double   m_appertain;

	RDOFuzzyValueItem()
		: m_rdovalue (RDOValue())
		, m_appertain(0         )
	{
	}
	RDOFuzzyValueItem( const RDOValue& rdovalue, double appertain )
		: m_rdovalue (rdovalue )
		, m_appertain(appertain)
	{
		if ( appertain < 0 )
		{
			m_appertain = 0;
		}
		else if ( appertain > 1 )
		{
			m_appertain = 1;
		}
	}
	bool operator< ( const RDOFuzzyValueItem& value )
	{
		return m_rdovalue < value.m_rdovalue;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
class  RDOFuzzyType;

class RDOFuzzyValue
{
public:
	RDOFuzzyValue( const RDOFuzzyType& type );
	RDOFuzzyValue( const RDOFuzzyValue& value );
	~RDOFuzzyValue();

	typedef std::vector< RDOFuzzyValueItem > FuzzySet;

	RDOFuzzyValue& append     ( const RDOValue& rdovalue, double appertain );
	RDOFuzzyValue& operator() ( const RDOValue& rdovalue, double appertain )
	{
		return append( rdovalue, appertain );
	}

	const RDOFuzzyType&       type () const { return *m_type;            }

	FuzzySet::const_iterator  begin() const { return m_fuzzySet.begin(); }
	FuzzySet::const_iterator  end  () const { return m_fuzzySet.end();   }

	/* 3.37  */  RDOFuzzyValue operator&& ( const RDOFuzzyValue& fuzzy_value ) const;
	/* 3.40  */  RDOFuzzyValue operator|| ( const RDOFuzzyValue& fuzzy_value ) const;
	/* 3.102 */  RDOFuzzyValue operator+  ( const RDOFuzzyValue& fuzzy_value ) const;
	/* 3.104 */  RDOFuzzyValue operator-  ( const RDOFuzzyValue& fuzzy_value ) const;
	/* 3.106 */  RDOFuzzyValue operator*  ( const RDOFuzzyValue& fuzzy_value ) const;
	/* 3.108 */  RDOFuzzyValue operator/  ( const RDOFuzzyValue& fuzzy_value ) const;

	/* 3.116 */  RDOFuzzyValue u_minus() const;
	/* 3.117 */  RDOFuzzyValue u_obr  () const;
	/* 3.118 */  RDOFuzzyValue u_scale( double scale ) const;
	/* 3.119 */  RDOFuzzyValue u_log  () const;

	/* 3.39  */  RDOFuzzyValue a_mult( const RDOFuzzyValue& fuzzy_value ) const;
	/* 3.48  */  RDOFuzzyValue alpha( double appertain ) const;
	/* 3.62  */  RDOFuzzyValue suppliment() const;
	/* 3.78  */  RDOFuzzyValue a_con() const;
	/* 3.79  */  RDOFuzzyValue a_dil() const;

	/* 3.272 */ RDOValue defuzzyfication();

	std::string getAsString() const;

private:
	FuzzySet             m_fuzzySet;
	const RDOFuzzyType*  m_type;

	typedef RDOValue (*ExtUnaryFun )( const RDOValue& value );
	typedef RDOValue (*ExtUnaryFunP)( const RDOValue& value, void* param );
	typedef RDOValue (*ExtBinaryFun)( const RDOValue& value1, const RDOValue& value2 );

	             RDOFuzzyValue a_pow     ( double power ) const;
	/* 3.114 */  RDOFuzzyValue ext_unary ( ExtUnaryFun  fun ) const;
	/* 3.114 */  RDOFuzzyValue ext_unary ( ExtUnaryFunP fun, void* param ) const;
	/* 3.83  */  RDOFuzzyValue ext_binary( ExtBinaryFun fun, const RDOFuzzyValue& fuzzy_value ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
class  RDOFuzzySetDefinition;

class RDOFuzzyType: public RDOType, public RDORuntimeParent
{
public:
	RDOFuzzyType( RDOFuzzySetDefinition* fuzzySetDefinition );
	virtual ~RDOFuzzyType();
	virtual std::string asString() const;

	typedef std::map< std::string, RDOFuzzyValue > Terms;

	bool operator== ( const RDOFuzzyType& type ) const
	{
		return this == &type;
	}
	bool operator!= ( const RDOFuzzyType& type ) const
	{
		return !operator==(type);
	}

	bool          inRange      ( const RDOValue& rdovalue   ) const;
	RDOFuzzyValue getSuppliment( const RDOFuzzyValue& value ) const;

private:
	Terms                  m_terms;
	RDOFuzzySetDefinition* m_fuzzySetDefinition;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinition: public RDORuntimeObject
{
public:
	RDOFuzzySetDefinition( RDORuntimeParent* parent );
	virtual ~RDOFuzzySetDefinition() {}
	virtual bool          inRange      ( const RDOValue& rdovalue   ) const = 0;
	virtual RDOFuzzyValue getSuppliment( const RDOFuzzyValue& value ) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinitionFixed: public RDOFuzzySetDefinition, private rdo::vector< RDOValue >
{
public:
	RDOFuzzySetDefinitionFixed( RDORuntimeParent* parent );
	virtual ~RDOFuzzySetDefinitionFixed();

	RDOFuzzySetDefinitionFixed& append( const RDOValue& rdovalue )
	{
		push_back( rdovalue );
		return *this;
	}
	RDOFuzzySetDefinitionFixed& operator() ( const RDOValue& rdovalue )
	{
		return append( rdovalue );
	}
	virtual bool          inRange      ( const RDOValue& rdovalue   ) const;
	virtual RDOFuzzyValue getSuppliment( const RDOFuzzyValue& value ) const;

private:
	typedef rdo::vector< RDOValue > Range;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinitionRangeDiscret: public RDOFuzzySetDefinition
{
public:
	RDOFuzzySetDefinitionRangeDiscret( RDORuntimeParent* parent, const RDOValue& step = 1 );
	virtual ~RDOFuzzySetDefinitionRangeDiscret();

	struct Range
	{
		RDOValue m_from;
		RDOValue m_till;
		
		Range( const RDOValue& from, const RDOValue& till )
			: m_from( from )
			, m_till( till )
		{
		}
	};
	typedef std::vector< Range > Ranges;

	RDOFuzzySetDefinitionRangeDiscret& append( const RDOValue& from, const RDOValue& till )
	{
		if ( from <= till )
		{
			if ( !inRange(from) && !inRange(till) )
			{
				m_range.push_back( Range(from, till) );
			}
		}
		return *this;
	}
	RDOFuzzySetDefinitionRangeDiscret& operator() ( const RDOValue& from, const RDOValue& till )
	{
		return append(from, till);
	}
	virtual bool          inRange      ( const RDOValue& rdovalue   ) const;
	virtual RDOFuzzyValue getSuppliment( const RDOFuzzyValue& value ) const;

private:
	Ranges   m_range;
	RDOValue m_step;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
class RDOFuzzyEmptyType: public RDOFuzzyType
{
public:
	static const RDOFuzzyEmptyType& getInstance( RDORuntimeParent* parent );

private:
	static RDOFuzzyEmptyType* s_emptyType;

	// ----------------------------------------------------------------------------
	// ---------- RDOFuzzySetDefinitionEmpty
	// ----------------------------------------------------------------------------
	class RDOFuzzySetDefinitionEmpty: public RDOFuzzySetDefinition
	{
	public:
		RDOFuzzySetDefinitionEmpty( RDORuntimeParent* parent );
		virtual ~RDOFuzzySetDefinitionEmpty();
		virtual bool          inRange      ( const RDOValue& rdovalue   ) const;
		virtual RDOFuzzyValue getSuppliment( const RDOFuzzyValue& value ) const;
	};

	RDOFuzzyEmptyType( RDORuntimeParent* parent );
	virtual ~RDOFuzzyEmptyType();
	virtual std::string asString() const;
};

} // namespace rdoRuntime

#endif // RDO_FUZZY_DEF_H
