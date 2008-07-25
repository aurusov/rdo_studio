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
	RDOFuzzyValueItem( const RDOValue& value, double appertain )
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
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
RDOFuzzyType::RDOFuzzyType( RDOFuzzySetDefinition* setDefinition )
	: RDORuntimeObject( setDefinition->getParent() )
	, RDOType         ( t_fuzzy       )
	, m_setDefinition ( setDefinition )
{
}

std::string RDOFuzzyType::asString() const
{
	return "RDOFuzzyType";
}

bool RDOFuzzyType::inRange( const RDOValue& value ) const
{
	return m_setDefinition->inRange( value );
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
RDOFuzzySetDefinition::RDOFuzzySetDefinition( RDORuntimeParent* parent )
	: RDORuntimeObject( parent )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValidSet
// ----------------------------------------------------------------------------
RDOFuzzyFixedSet::RDOFuzzyFixedSet( RDORuntimeParent* parent )
	: RDOFuzzySetDefinition( parent )
	, m_set( NULL )
{
	m_set = new FuzzySet();
}

RDOFuzzyFixedSet::~RDOFuzzyFixedSet()
{
	if ( m_set )
	{
		delete m_set;
		m_set = NULL;
	}
}

void RDOFuzzyFixedSet::append( const RDOValue& value, double appertain )
{
	m_set->push_back( RDOFuzzyValueItem(value, appertain) );
}

bool RDOFuzzyFixedSet::inRange( const RDOValue& value ) const
{
	return true;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
RDOFuzzyValue::RDOFuzzyValue( const RDOFuzzyType& type )
	: m_type( &type )
	, m_set ( NULL  )
{
	m_set = new FuzzySet();
}

RDOFuzzyValue::RDOFuzzyValue( const RDOFuzzyValue& value )
	: m_type( value.m_type )
	, m_set ( NULL         )
{
	m_set = new FuzzySet();
	*m_set = *value.m_set;
}

RDOFuzzyValue::~RDOFuzzyValue()
{
	if ( m_set )
	{
		delete m_set;
		m_set = NULL;
	}
}

RDOValue RDOFuzzyValue::defuzzyfication()
{
	return 1;
}

} // namespace rdoRuntime
