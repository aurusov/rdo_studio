// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- DefineArea
// --------------------------------------------------------------------------------
DefineArea::DefineArea()
{
	IntervalType mType = DomainPart::open(-10e15, 10e15);
	m_domain = Domain(mType);
}

DefineArea::~DefineArea()
{}

DefineArea::DefineArea(const RDOValue& leftRange, const RDOValue& rightRange)
{
	IntervalType mType = DomainPart::closed(leftRange, rightRange);
	m_domain = Domain(mType);
}
DefineArea::DefineArea(const RDOValue& value)
{
	IntervalType mType = DomainPart::closed(value,value);
	m_domain = Domain(mType);
}

// --------------------------------------------------------------------------------
// -------------------- FuzzySet
// --------------------------------------------------------------------------------
RDOValue fun_add (const RDOValue& value1, const RDOValue& value2) { return value1 + value2; }
RDOValue fun_sub (const RDOValue& value1, const RDOValue& value2) { return value1 - value2; }
RDOValue fun_mult(const RDOValue& value1, const RDOValue& value2) { return value1 * value2; }
RDOValue fun_div (const RDOValue& value1, const RDOValue& value2) { return value1 / value2; }

FuzzySet::FuzzySet()
{}

FuzzySet::FuzzySet(const LPDefineArea& pDefineArea)
	: m_defineArea(pDefineArea)
{}

FuzzySet::FuzzySet(const LPFuzzySet& pSet)
	: m_fuzzySet  (pSet->m_fuzzySet  )
	, m_defineArea(pSet->m_defineArea)
{}

FuzzySet::~FuzzySet()
{}

void FuzzySet::setValues(const FuzzySetDefinition& values)
{
	m_fuzzySet = values;
}

LPFuzzySet FuzzySet::append(const RDOValue& rdovalue, double appertain)
{
	std::pair<FuzzySet::FuzzySetDefinition::iterator, bool> checkValue;
	checkValue = m_fuzzySet.insert(std::pair<RDOValue,double>(rdovalue, appertain));
	return LPFuzzySet(this);
}

LPFuzzySet FuzzySet::operator() (const RDOValue& rdovalue, double appertain)
{
	return append(rdovalue, appertain);
}

double& FuzzySet::operator[] (const RDOValue& rdovalue)
{
	return m_fuzzySet[rdovalue];
}

FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::find(const RDOValue& rdovalue) const
{
	return m_fuzzySet.find(rdovalue);
}

FuzzySet::FuzzyItem FuzzySet::findValue(const RDOValue& rdovalue) const
{
	FuzzySetDefinition::const_iterator found = find(rdovalue);
	if (found != end()) return FuzzyItem(found->first, found->second);
	else                return FuzzyItem(rdovalue,     0.0          );
}

FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::begin() const { return m_fuzzySet.begin(); }
FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::end  () const { return m_fuzzySet.end();   }
FuzzySet::FuzzySetDefinition::iterator       FuzzySet::begin()       { return m_fuzzySet.begin(); }
FuzzySet::FuzzySetDefinition::iterator       FuzzySet::end  ()       { return m_fuzzySet.end();   }
bool                                         FuzzySet::empty() const { return m_fuzzySet.empty(); }
bool FuzzySet::inRange (const RDOValue& rdovalue)
{
	return m_fuzzySet.find(rdovalue) != m_fuzzySet.end();
}

LPFuzzySet FuzzySet::operator+ (const LPFuzzySet& pSet) const
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_add, pThis, pSet);
}
LPFuzzySet FuzzySet::operator- (const LPFuzzySet& pSet) const 
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_sub, pThis, pSet);
}
LPFuzzySet FuzzySet::operator* (const LPFuzzySet& pSet) const
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_mult, pThis, pSet);
}
LPFuzzySet FuzzySet::operator/ (const LPFuzzySet& pSet) const
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_div, pThis, pSet);
}

RDOValue fun_u_minus(const RDOValue& rdovalue             ) { return -rdovalue;                                    }
RDOValue fun_u_obr  (const RDOValue& rdovalue             ) { return RDOValue(1)/rdovalue;                         }
RDOValue fun_u_scale(const RDOValue& rdovalue, void* scale) { return rdovalue * (*static_cast<double*>(scale));    }
RDOValue fun_u_log  (const RDOValue& rdovalue             ) { return rdovalue > 0 ? log(rdovalue.getDouble()) : 0; }

LPFuzzySet MemberFunctionProperties::u_minus(const LPFuzzySet& pSet)               { return ext_unary(fun_u_minus,           pSet); }
LPFuzzySet MemberFunctionProperties::u_obr  (const LPFuzzySet& pSet)               { return ext_unary(fun_u_obr,             pSet); }
LPFuzzySet MemberFunctionProperties::u_scale(const LPFuzzySet& pSet, double scale) { return ext_unary(fun_u_scale, &scale, pSet);   }
LPFuzzySet MemberFunctionProperties::u_log  (const LPFuzzySet& pSet)               { return ext_unary(fun_u_log,             pSet); }


std::string FuzzySet::getAsString() const
{
	if (empty())
		return "[empty value]";

	std::string res = "";
	FuzzySetDefinition::const_iterator it = begin();
	while (it != end())
	{
		const bool output = it->second > 0.0;
		if (output)
			res += rdo::format("<%s/%.2lf>", it->first.getAsString().c_str(), it->second);

		++it;

		if (output && it != end())
			res += " ";
	}
	return res;
}

LPFuzzySet FuzzySet::clone() const
{
	return rdo::Factory<FuzzySet>::create(LPFuzzySet(const_cast<FuzzySet*>(this)));
}

LPFuzzySet FuzzySet::operator&& (const LPFuzzySet& pSet) const
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);
	// Найдем только пересекающие элементы и выберем минимальную функцию принадлежности
	FuzzySetDefinition::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet->find(it1->first);
		if (it2 != pSet->end())
		{
			pFuzzySetResult->operator[](it1->first) = (std::min)(it1->second, it2->second);
		}
		++it1;
	}
	return pFuzzySetResult;
}

LPFuzzySet FuzzySet::operator|| (const LPFuzzySet& pSet)const
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);
	// Объединим элементы двух множеств
	// Если элемент одновременно принадлежит обоим множествам, то выберем максимальную функцию принадлежности
	FuzzySetDefinition::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet->find(it1->first);
		if (it2 != pSet->end())
		{
			pFuzzySetResult->operator[](it1->first) = (std::max)(it1->second, it2->second);
		}
		else
		{
			// Не нашли пары
			pFuzzySetResult->operator[](it1->first) = it1->second;
		}
		++it1;
	}
	FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet->begin();
	while (it2 != pSet->end())
	{
		FuzzySetDefinition::const_iterator it1 = find(it2->first);
		if (it1 == end())
		{
			// Не нашли пары
			pFuzzySetResult->operator[](it2->first) = it2->second;
		}
		++it2;
	}
	return pFuzzySetResult;
}

// --------------------------------------------------------------------------------
// -------------------- MemberFunctionProperties
// --------------------------------------------------------------------------------
// TODO комментарии в *.h
// Декартово произведение (попарное) элементов двух множест с применением произвольной функции fun
LPFuzzySet MemberFunctionProperties::ext_binary(ExtBinaryFun fun, const LPFuzzySet& pSet1, const LPFuzzySet& pSet2)
{
	FuzzySet::FuzzySetDefinition values;
	FuzzySet::FuzzySetDefinition::const_iterator it1 = pSet1->begin();
	while (it1 != pSet1->end())
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet2->begin();
		while (it2 != pSet2->end())
		{
			RDOValue rdo_value = fun(it1->first, it2->first);
			FuzzySet::FuzzySetDefinition::iterator val = values.find(rdo_value);
			if (val == values.end())
			{
				values[rdo_value] = (std::min)(it1->second, it2->second);
			}
			else
			{
				values[rdo_value] = (std::max)(val->second, (std::min)(it1->second, it2->second));
			}
			++it2;
		}
		++it1;
	}

	if (!values.empty())
	{
		LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
		ASSERT(pFuzzySetResult);
		pFuzzySetResult->setValues(values);

		return pFuzzySetResult;
	}

	return LPFuzzySet();
}

// Преобразование элементов через произвольную функцию fun
LPFuzzySet MemberFunctionProperties::ext_unary(ExtUnaryFun fun, const LPFuzzySet& pSet)
{
	FuzzySet::FuzzySetDefinition values;
	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();
	while (it != pSet->end())
	{
		RDOValue rdo_value = fun(it->first);
		FuzzySet::FuzzySetDefinition::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->second;
		}
		else
		{
			values[rdo_value] = (std::max)(val->second, it->second);
		}
		++it;
	}

	if (!values.empty())
	{

		LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
		ASSERT(pFuzzySetResult);
		pFuzzySetResult->setValues(values);

		return pFuzzySetResult;
	}

	return LPFuzzySet();
}

LPFuzzySet MemberFunctionProperties::ext_unary(ExtUnaryFunP fun, void* pParam, const LPFuzzySet& pSet)
{
	FuzzySet::FuzzySetDefinition values;
	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();
	while (it != pSet->end())
	{
		RDOValue rdo_value = fun(it->first, pParam);
		FuzzySet::FuzzySetDefinition::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->second;
		}
		else
		{
			values[rdo_value] = (std::max)(val->second, it->second);
		}
		++it;
	}

	if (!values.empty())
	{

		LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
		ASSERT(pFuzzySetResult);
		pFuzzySetResult->setValues(values);
		return pFuzzySetResult;
	}

	return LPFuzzySet();
}

LPFuzzySet MemberFunctionProperties::a_con (const LPFuzzySet& pSet)
{
	return a_pow(pSet, 2.0);
}

LPFuzzySet MemberFunctionProperties::a_dil (const LPFuzzySet& pSet)
{
	return a_pow(pSet, 0.5);
}

LPFuzzySet MemberFunctionProperties::a_mult(const LPFuzzySet& pSet1, const LPFuzzySet& pSet2)
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);

	// Найдем только пересекающие элементы и перемножим функции принадлежности
	FuzzySet::FuzzySetDefinition::const_iterator it1 = pSet1->begin();
	for (it1 = pSet1->begin(); it1 != pSet1->end(); ++it1)
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet2->find(it1->first);
		if (it2 != pSet2->end())
		{
			pFuzzySetResult->operator[](it1->first) = it1->second * it2->second;
		}
	}
	return pFuzzySetResult;
}

LPFuzzySet MemberFunctionProperties::alpha(const LPFuzzySet& pSet,double appertain)
{
	if (appertain < 0) appertain = 0;
	else if (appertain > 1) appertain = 1;

	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);

	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();

	// Найдем отсечку
	while (it != pSet->end())
	{
		if (it->second >= appertain)
		{
			pFuzzySetResult->operator[](it->first) = pSet->operator[](it->first);
		}
		++it;
	}
	return pFuzzySetResult;
}

LPFuzzySet MemberFunctionProperties::supplement(const LPFuzzySet& pSet)
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create(pSet);
	ASSERT(pFuzzySetResult);

	FuzzySet::FuzzySetDefinition::iterator it = pFuzzySetResult->begin();
	while (it != pFuzzySetResult->end())
	{
		it->second = 1-(it->second);
		++it;
	}
	return(pFuzzySetResult);
}

LPFuzzySet MemberFunctionProperties::a_pow(LPFuzzySet pSet, double power)
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create(pSet);
	ASSERT(pFuzzySetResult);

	// Возведём в степень
	FuzzySet::FuzzySetDefinition::iterator it = pFuzzySetResult->begin();
	while (it != pFuzzySetResult->end())
	{
		it->second = pow(it->second, power);
		++it;
	}
	return pFuzzySetResult;
}

LPRDOLingvoVariable MemberFunctionProperties::fuzzyfication(const RDOValue&value, const LPRDOLingvoVariable& variable)
{
	LPRDOLingvoVariable pVariable = rdo::Factory<RDOLingvoVariable>::create(value, variable);
	for (RDOLingvoVariable::TermSet::const_iterator it = variable->begin(); it != variable->end(); ++it)
	{
		LPFuzzySet pSet = rdo::Factory<FuzzySet>::create(it->second);
		FuzzySet::FuzzyItem item = pSet->findValue(value);
		LPFuzzySet newSet = rdo::Factory<FuzzySet>::create();
		newSet->append(item.first, item.second);
		pVariable->append(it->first, newSet);
	}
	return pVariable;
}

RDOValue MemberFunctionProperties::defuzzyfication(const LPFuzzySet& pSet)
{
	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();
	if (it == pSet->end())
		return RDOValue();

	FuzzySet::FuzzySetDefinition::const_iterator it_next = it;
	++it_next;
	if (it_next == pSet->end())
		return it->first;

	double g      = 0;
	double f      = 0;
	double x      = it->first.getDouble();
	double x_next = it_next->first.getDouble();
	double a      = it->second;
	double b      = it_next->second;
	double h      = x_next - x;
	for (;;)
	{
		double t = h*(a+b)/2.0;
		g += t*(x_next + x)/2.0;
		f += t;

		++it;
		++it_next;
		if (it_next == pSet->end())
			break;

		x      = x_next;
		a      = b;
		x_next = it_next->first.getDouble();
		b      = it_next->second;
		h      = x_next - x;
	}
	return g / f;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyTerm
// --------------------------------------------------------------------------------
const LPFuzzySet& RDOFuzzyTerm::getFuzzySetDefinition() const
{
	return (m_term.second);
}
std::string RDOFuzzyTerm::getName() const
{
	return (m_term.first);
}
RDOFuzzyTerm::RDOFuzzyTerm(const termName& pName, const LPFuzzySet& pSet)
{
    m_term = std::make_pair(pName, pSet);
}

RDOFuzzyTerm::~RDOFuzzyTerm()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
RDOLingvoVariable::TermSet::const_iterator RDOLingvoVariable::begin() const     {return m_set.begin();}
RDOLingvoVariable::TermSet::const_iterator RDOLingvoVariable::end  () const     {return m_set.end  ();}
RDOLingvoVariable::RDOLingvoVariable(const LPRDOFuzzyTerm& term, nameOfVariable nameOfVariable)
{
	m_name = nameOfVariable;
	m_set.insert(make_pair(term->getName(), term->getFuzzySetDefinition()));
}

RDOLingvoVariable::RDOLingvoVariable(const RDOValue&pDefineAreaValue, const LPRDOLingvoVariable& variable)
	: m_name("activated")
{
	LPDefineArea   defineArea    = rdo::Factory<DefineArea>::create(pDefineAreaValue);
	LPFuzzySet     setOfVariable = rdo::Factory<FuzzySet>::create(defineArea);
	for (RDOLingvoVariable::TermSet::const_iterator it = variable->begin(); it != variable->end(); it++)
	{
		m_set.insert(std::pair<RDOFuzzyTerm::Term::first_type,LPFuzzySet>(it->first,setOfVariable));
	}
}

RDOLingvoVariable::RDOLingvoVariable(const RDOLingvoVariable& variable)
{
	m_set = variable.m_set;
	m_name  = variable.m_name;
}

RDOLingvoVariable::~RDOLingvoVariable()
{}

void RDOLingvoVariable::setName(nameOfVariable nameVariable)
{
	m_name = nameVariable;
}

LPFuzzySet& RDOLingvoVariable::operator[] (std::string name)
{
	return m_set[name];
}

void RDOLingvoVariable::append(std::string name, const LPFuzzySet& fuzzySet)
{
	operator[](name) = fuzzySet;
}

// --------------------------------------------------------------------------------
// -------------------- Statement
// --------------------------------------------------------------------------------
void Statement::setTerm(LPRDOFuzzyTerm term)
{
	m_term = term;
}

void Statement::setVariable(LPRDOLingvoVariable variable)
{
	m_variable = variable;
}

Statement::Statement()
{}

Statement::~Statement()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
