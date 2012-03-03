#include "pch.h"
#include "rdo_fuzzy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
RDOFuzzyValue::RDOFuzzyValue( const RDOFuzzyType& type )
	: m_type( &type )
{
}

RDOFuzzyValue::RDOFuzzyValue( const RDOFuzzyValue& value )
	: m_type( value.m_type )
{
	m_fuzzySet = value.m_fuzzySet;
}

RDOFuzzyValue::~RDOFuzzyValue()
{
}

RDOFuzzyValue& RDOFuzzyValue::append( const RDOValue& rdovalue, double appertain )
{
	if ( type().inRange(rdovalue) )
	{
		m_fuzzySet.push_back( RDOFuzzyValueItem(rdovalue, appertain) );
	}
	return *this;
}

class RDOFuzzyValueItemFind
{
public:
	RDOFuzzyValueItemFind( const RDOValue& rdovalue )
		: m_rdovalue( rdovalue )
	{
	}
	bool operator() ( const RDOFuzzyValueItem& item )
	{
		return item.m_rdovalue == m_rdovalue;
	}
private:
	const RDOValue& m_rdovalue;
};

RDOFuzzyValue RDOFuzzyValue::operator&& ( const RDOFuzzyValue& fuzzy_value ) const
{
	if ( type() != fuzzy_value.type() )
	{
		throw RDOValueException();
	}
	RDOFuzzyValue fuzzy_result( type() );
	// Найдем только пересекающие элементы и выберем минимальную функцию принадлежности
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while ( it1 != m_fuzzySet.end() )
	{
		FuzzySet::const_iterator it2 = std::find_if( fuzzy_value.m_fuzzySet.begin(), fuzzy_value.m_fuzzySet.end(), RDOFuzzyValueItemFind(it1->m_rdovalue) );
		if ( it2 != fuzzy_value.m_fuzzySet.end() )
		{
			fuzzy_result.m_fuzzySet.push_back( RDOFuzzyValueItem(it1->m_rdovalue, rdo::rmin(it1->m_appertain, it2->m_appertain)) );
		}
		it1++;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::operator|| ( const RDOFuzzyValue& fuzzy_value ) const
{
	if ( type() != fuzzy_value.type() )
	{
		throw RDOValueException();
	}
	RDOFuzzyValue fuzzy_result( type() );
	// Объединим элементы двух множеств
	// Если элемент одновременно принадлежит обоим множествам, то выберем максимальную функцию принадлежности
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while ( it1 != m_fuzzySet.end() )
	{
		FuzzySet::const_iterator it2 = std::find_if( fuzzy_value.m_fuzzySet.begin(), fuzzy_value.m_fuzzySet.end(), RDOFuzzyValueItemFind(it1->m_rdovalue) );
		if ( it2 != fuzzy_value.m_fuzzySet.end() )
		{
			fuzzy_result.m_fuzzySet.push_back( RDOFuzzyValueItem(it1->m_rdovalue, rdo::rmax(it1->m_appertain, it2->m_appertain)) );
		}
		else
		{
			// Не нашли пары
			fuzzy_result.m_fuzzySet.push_back( *it1 );
		}
		it1++;
	}
	FuzzySet::const_iterator it2 = fuzzy_value.m_fuzzySet.begin();
	while ( it2 != fuzzy_value.m_fuzzySet.end() )
	{
		FuzzySet::const_iterator it1 = std::find_if( m_fuzzySet.begin(), m_fuzzySet.end(), RDOFuzzyValueItemFind(it2->m_rdovalue) );
		if ( it1 == m_fuzzySet.end() )
		{
			// Не нашли пары
			fuzzy_result.m_fuzzySet.push_back( *it2 );
		}
		it2++;
	}
	// Отсортируем, чтобы элементы были упорядочены
	std::sort( fuzzy_result.m_fuzzySet.begin(), fuzzy_result.m_fuzzySet.end() );
	return fuzzy_result;
}

RDOValue fun_add ( const RDOValue& value1, const RDOValue& value2 ) { return value1 + value2; }
RDOValue fun_sub ( const RDOValue& value1, const RDOValue& value2 ) { return value1 - value2; }
RDOValue fun_mult( const RDOValue& value1, const RDOValue& value2 ) { return value1 * value2; }
RDOValue fun_div ( const RDOValue& value1, const RDOValue& value2 ) { return value1 / value2; }

//! Декартово произведение (попарное) элементов двух множест с применением произвольной функции fun
RDOFuzzyValue RDOFuzzyValue::ext_binary( ExtBinaryFun fun, const RDOFuzzyValue& fuzzy_value ) const
{
	typedef std::map< RDOValue, double > Values;
	Values values;
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while ( it1 != m_fuzzySet.end() )
	{
		FuzzySet::const_iterator it2 = fuzzy_value.m_fuzzySet.begin();
		while ( it2 != fuzzy_value.m_fuzzySet.end() )
		{
			RDOValue rdo_value = fun(it1->m_rdovalue, it2->m_rdovalue);
			Values::iterator val = values.find(rdo_value);
			if ( val == values.end() )
			{
				values[rdo_value] = rdo::rmin(it1->m_appertain, it2->m_appertain);
			}
			else
			{
				values[rdo_value] = rdo::rmax(val->second, rdo::rmin(it1->m_appertain, it2->m_appertain));
			}
			it2++;
		}
		it1++;
	}
	if ( !values.empty() )
	{
		RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret( type().getParent() );
		fuzzy_setDefinition->append(values.begin()->first, values.rbegin()->first);
		RDOFuzzyType* fuzzy_type  = new RDOFuzzyType( fuzzy_setDefinition );
		RDOFuzzyValue fuzzy_result( *fuzzy_type );
		Values::const_iterator val = values.begin();
		while ( val != values.end() )
		{
			fuzzy_result.append(val->first, val->second);
			val++;
		}
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOFuzzyValue RDOFuzzyValue::operator+ ( const RDOFuzzyValue& fuzzy_value ) const
{
	return ext_binary(fun_add, fuzzy_value);
}

RDOFuzzyValue RDOFuzzyValue::operator- ( const RDOFuzzyValue& fuzzy_value ) const
{
	return ext_binary(fun_sub, fuzzy_value);
}

RDOFuzzyValue RDOFuzzyValue::operator* ( const RDOFuzzyValue& fuzzy_value ) const
{
	return ext_binary(fun_mult, fuzzy_value);
}

RDOFuzzyValue RDOFuzzyValue::operator/ ( const RDOFuzzyValue& fuzzy_value ) const
{
	return ext_binary(fun_div, fuzzy_value);
}

RDOValue fun_u_minus( const RDOValue& value              ) { return -value;                                 }
RDOValue fun_u_obr  ( const RDOValue& value              ) { return RDOValue(1)/value;                      }
RDOValue fun_u_scale( const RDOValue& value, void* scale ) { return value * (*static_cast<double*>(scale)); }
RDOValue fun_u_log  ( const RDOValue& value              ) { return value > 0 ? log(value.getDouble()) : 0; }

//! Преобразование элементов через произвольную функцию fun
RDOFuzzyValue RDOFuzzyValue::ext_unary( ExtUnaryFun fun ) const
{
	typedef std::map< RDOValue, double > Values;
	Values values;
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while ( it != m_fuzzySet.end() )
	{
		RDOValue rdo_value = fun(it->m_rdovalue);
		Values::iterator val = values.find(rdo_value);
		if ( val == values.end() )
		{
			values[rdo_value] = it->m_appertain;
		}
		else
		{
			values[rdo_value] = rdo::rmax(val->second, it->m_appertain);
		}
		it++;
	}
	if ( !values.empty() )
	{
		RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret( type().getParent() );
		fuzzy_setDefinition->append(values.begin()->first, values.rbegin()->first);
		RDOFuzzyType* fuzzy_type  = new RDOFuzzyType( fuzzy_setDefinition );
		RDOFuzzyValue fuzzy_result( *fuzzy_type );
		Values::const_iterator val = values.begin();
		while ( val != values.end() )
		{
			fuzzy_result.append(val->first, val->second);
			val++;
		}
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOFuzzyValue RDOFuzzyValue::ext_unary( ExtUnaryFunP fun, void* param ) const
{
	typedef std::map< RDOValue, double > Values;
	Values values;
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while ( it != m_fuzzySet.end() )
	{
		RDOValue rdo_value = fun(it->m_rdovalue, param);
		Values::iterator val = values.find(rdo_value);
		if ( val == values.end() )
		{
			values[rdo_value] = it->m_appertain;
		}
		else
		{
			values[rdo_value] = rdo::rmax(val->second, it->m_appertain);
		}
		it++;
	}
	if ( !values.empty() )
	{
		RDOFuzzySetDefinitionRangeDiscret* fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret( type().getParent() );
		fuzzy_setDefinition->append(values.begin()->first, values.rbegin()->first);
		RDOFuzzyType* fuzzy_type  = new RDOFuzzyType( fuzzy_setDefinition );
		RDOFuzzyValue fuzzy_result( *fuzzy_type );
		Values::const_iterator val = values.begin();
		while ( val != values.end() )
		{
			fuzzy_result.append(val->first, val->second);
			val++;
		}
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOFuzzyValue RDOFuzzyValue::u_minus() const
{
	return ext_unary(fun_u_minus);
}

RDOFuzzyValue RDOFuzzyValue::u_obr() const
{
	return ext_unary(fun_u_obr);
}

RDOFuzzyValue RDOFuzzyValue::u_scale( double scale ) const
{
	return ext_unary(fun_u_scale, &scale);
}

RDOFuzzyValue RDOFuzzyValue::u_log() const
{
	return ext_unary(fun_u_log);
}

RDOFuzzyValue RDOFuzzyValue::suppliment() const
{
	return type().getSuppliment( *this );
}

RDOFuzzyValue RDOFuzzyValue::a_mult( const RDOFuzzyValue& fuzzy_value ) const
{
	if ( type() != fuzzy_value.type() )
	{
		throw RDOValueException();
	}
	RDOFuzzyValue fuzzy_result( type() );
	// Найдем только пересекающие элементы и перемножим функции принадлежности
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while ( it1 != m_fuzzySet.end() )
	{
		FuzzySet::const_iterator it2 = std::find_if( fuzzy_value.m_fuzzySet.begin(), fuzzy_value.m_fuzzySet.end(), RDOFuzzyValueItemFind(it1->m_rdovalue) );
		if ( it2 != fuzzy_value.m_fuzzySet.end() )
		{
			fuzzy_result.m_fuzzySet.push_back( RDOFuzzyValueItem(it1->m_rdovalue, it1->m_appertain * it2->m_appertain) );
		}
		it1++;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::alpha( double appertain ) const
{
	if ( appertain < 0 )
	{
		appertain = 0;
	}
	else if ( appertain > 1 )
	{
		appertain = 1;
	}
	RDOFuzzyValue fuzzy_result( type() );
	// Найдем отсечку
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while ( it != m_fuzzySet.end() )
	{
		if ( it->m_appertain >= appertain )
		{
			fuzzy_result.m_fuzzySet.push_back( RDOFuzzyValueItem(it->m_rdovalue, appertain) );
		}
		it++;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::a_con() const
{
	return a_pow( 2 );
}

RDOFuzzyValue RDOFuzzyValue::a_dil() const
{
	return a_pow( 0.5 );
}

RDOFuzzyValue RDOFuzzyValue::a_pow( double power ) const
{
	RDOFuzzyValue fuzzy_result( *this );
	// Возведём в степень
	FuzzySet::iterator it = fuzzy_result.m_fuzzySet.begin();
	while ( it != fuzzy_result.m_fuzzySet.end() )
	{
		it->m_appertain = ::pow( it->m_appertain, power );
		it++;
	}
	return fuzzy_result;
}

RDOValue RDOFuzzyValue::defuzzyfication()
{
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	if ( it == m_fuzzySet.end() )
	{
		return RDOValue();
	}
	FuzzySet::const_iterator it_next = it;
	it_next++;
	if ( it_next == m_fuzzySet.end() )
	{
		return it->m_rdovalue;
	}
	double g      = 0;
	double f      = 0;
	double x      = it->m_rdovalue.getDouble();
	double x_next = it_next->m_rdovalue.getDouble();
	double a      = it->m_appertain;
	double b      = it_next->m_appertain;
	double h      = x_next - x;
	while ( true )
	{
		double t = h*(a+b)/2.0;
		g += t*(x_next + x)/2.0;
		f += t;

		it++;
		it_next++;
		if ( it_next == m_fuzzySet.end() )
		{
			break;
		}

		x      = x_next;
		a      = b;
		x_next = it_next->m_rdovalue.getDouble();
		b      = it_next->m_appertain;
		h      = x_next - x;
	}
	return g / f;
}

std::string RDOFuzzyValue::getAsString() const
{
	if ( m_fuzzySet.empty() )
	{
		return "[empty value]";
	}
	std::string res = "";
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while ( it != m_fuzzySet.end() )
	{
		res += rdo::format("[%.2lf/%s]", it->m_appertain, it->m_rdovalue.getAsString().c_str());
		it++;
		if ( it != m_fuzzySet.end() )
		{
			res += ", ";
		}
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
RDOFuzzyType::RDOFuzzyType( RDOFuzzySetDefinition* fuzzySetDefinition )
	: RDORuntimeParent    ( fuzzySetDefinition->getParent() )
	, RDOType             ( t_fuzzy                         )
	, m_fuzzySetDefinition( fuzzySetDefinition              )
{
	m_fuzzySetDefinition->reparent(this);
}

RDOFuzzyType::~RDOFuzzyType()
{
}

std::string RDOFuzzyType::asString() const
{
	return "RDOFuzzyType";
}

bool RDOFuzzyType::inRange( const RDOValue& rdovalue ) const
{
	return m_fuzzySetDefinition->inRange( rdovalue );
}

RDOFuzzyValue RDOFuzzyType::getSuppliment( const RDOFuzzyValue& value ) const
{
	return m_fuzzySetDefinition->getSuppliment( value );
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
RDOFuzzySetDefinition::RDOFuzzySetDefinition( RDORuntimeParent* parent )
	: RDORuntimeObject( parent )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
RDOFuzzySetDefinitionFixed::RDOFuzzySetDefinitionFixed( RDORuntimeParent* parent )
	: RDOFuzzySetDefinition( parent )
{
}

RDOFuzzySetDefinitionFixed::~RDOFuzzySetDefinitionFixed()
{
}

bool RDOFuzzySetDefinitionFixed::inRange( const RDOValue& rdovalue ) const
{
	return std::find( begin(), end(), rdovalue ) != end();
}

RDOFuzzyValue RDOFuzzySetDefinitionFixed::getSuppliment( const RDOFuzzyValue& value ) const
{
	RDOFuzzyValue fuzzy_result(value.type());
	Range::const_iterator range = begin();
	while ( range != end() )
	{
		bool found = false;
		RDOFuzzyValue::FuzzySet::const_iterator val = value.begin();
		while ( val != value.end() )
		{
			if ( val->m_rdovalue == *range )
			{
				found = true;
				break;
			}
			val++;
		}
		if ( found )
		{
			double appertain = 1 - val->m_appertain;
			if ( appertain > 0 )
			{
				fuzzy_result.append( val->m_rdovalue, appertain );
			}
		}
		else
		{
			fuzzy_result.append( *range, 1 );
		}
		range++;
	}
	return fuzzy_result;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
RDOFuzzySetDefinitionRangeDiscret::RDOFuzzySetDefinitionRangeDiscret( RDORuntimeParent* parent, const RDOValue& step )
	: RDOFuzzySetDefinition( parent )
	, m_step( step )
{
}

RDOFuzzySetDefinitionRangeDiscret::~RDOFuzzySetDefinitionRangeDiscret()
{
}

bool RDOFuzzySetDefinitionRangeDiscret::inRange( const RDOValue& rdovalue ) const
{
	Ranges::const_iterator it = m_range.begin();
	while ( it != m_range.end() )
	{
		if ( rdovalue >= it->m_from && rdovalue <= it->m_till )
		{
			return true;
		}
		it++;
	}
	return false;
}

RDOFuzzyValue RDOFuzzySetDefinitionRangeDiscret::getSuppliment( const RDOFuzzyValue& value ) const
{
	RDOFuzzyValue fuzzy_result(value.type());
	Ranges::const_iterator range = m_range.begin();
	while ( range != m_range.end() )
	{
		for (RDOValue ritem = range->m_from; ritem <= range->m_till; ritem += m_step )
		{
			bool found = false;
			RDOFuzzyValue::FuzzySet::const_iterator val = value.begin();
			while ( val != value.end() )
			{
				if ( val->m_rdovalue == ritem )
				{
					found = true;
					break;
				}
				val++;
			}
			if ( found )
			{
				double appertain = 1 - val->m_appertain;
				if ( appertain > 0 )
				{
					fuzzy_result.append( val->m_rdovalue, appertain );
				}
			}
			else
			{
				fuzzy_result.append( ritem, 1 );
			}
		}
		range++;
	}
	return fuzzy_result;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
RDOFuzzyEmptyType* RDOFuzzyEmptyType::s_emptyType = NULL;

RDOFuzzyEmptyType::RDOFuzzyEmptyType( RDORuntimeParent* parent )
	: RDOFuzzyType( new RDOFuzzySetDefinitionEmpty(parent) )
{
}

RDOFuzzyEmptyType::~RDOFuzzyEmptyType()
{
	RDOFuzzyEmptyType::s_emptyType = NULL;
}

std::string RDOFuzzyEmptyType::asString() const
{
	return "[empty set]";
}

const RDOFuzzyEmptyType& RDOFuzzyEmptyType::getInstance( RDORuntimeParent* parent )
{
	if ( !RDOFuzzyEmptyType::s_emptyType )
	{
		RDOFuzzyEmptyType::s_emptyType = new RDOFuzzyEmptyType(parent);
	}
	return *RDOFuzzyEmptyType::s_emptyType;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionEmpty
// ----------------------------------------------------------------------------
RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::RDOFuzzySetDefinitionEmpty( RDORuntimeParent* parent )
	: RDOFuzzySetDefinition( parent )
{
}

RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::~RDOFuzzySetDefinitionEmpty()
{
}

bool RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::inRange( const RDOValue& rdovalue ) const
{
	return false;
}

RDOFuzzyValue RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::getSuppliment( const RDOFuzzyValue& value ) const
{
	return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(value.type().getParent()));
}

} // namespace rdoRuntime
