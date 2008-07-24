#include "pch.h"
#include "rdo_fuzzy.h"
#include "rdo_value.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
RDOFuzzyType::RDOFuzzyType()
	: RDOType( t_fuzzy )
{
}

std::string RDOFuzzyType::asString() const
{
	return "RDOFuzzyType";
}

bool RDOFuzzyType::inRange() const
{
	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValueItem
// ----------------------------------------------------------------------------
struct RDOFuzzyValueItem
{
	RDOValue m_value;
	double   m_appertain;

	RDOFuzzyValueItem()
		: m_value    (RDOValue())
		, m_appertain(0         )
	{
	}
	RDOFuzzyValueItem( RDOValue value, double appertain )
		: m_value    (value    )
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
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
RDOFuzzyValue::RDOFuzzyValue( const RDOFuzzyType& type )
	: m_type( &type )
{
	m_set = new FuzzySet;
}

RDOFuzzyValue::RDOFuzzyValue( const RDOFuzzyValue& value )
	: m_type( value.m_type )
{
	m_set = new FuzzySet;
	*m_set = *value.m_set;
}

RDOValue RDOFuzzyValue::defuzzyfication()
{
	return 1;
}

} // namespace rdoRuntime
