/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2009
  \brief     Нечеткая логика
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- DefineArea
// --------------------------------------------------------------------------------
inline DefineArea::DefineArea()
{
	IntervalType mType = DomainPart::open(-10e15, 10e15);
	m_domain = Domain(mType);
}

inline DefineArea::~DefineArea()
{}

inline DefineArea::DefineArea(CREF(RDOValue) leftRange, CREF(RDOValue) rightRange)
{
	IntervalType mType = DomainPart::closed(leftRange, rightRange);
	m_domain = Domain(mType);
}
inline DefineArea::DefineArea(CREF(RDOValue) value)
{
	IntervalType mType = DomainPart::closed(value,value);
	m_domain = Domain(mType);
}


// --------------------------------------------------------------------------------
// -------------------- FuzzySet
// --------------------------------------------------------------------------------
inline FuzzySet::FuzzySet()
{}

inline FuzzySet::FuzzySet(CREF(LPDefineArea) pDefineArea)
	: m_defineArea(pDefineArea)
{}
inline FuzzySet::FuzzySet(CREF(LPFuzzySet) pSet)
	: m_defineArea(pSet->m_defineArea)
	, m_fuzzySet  (pSet->m_fuzzySet)
{}
inline FuzzySet::~FuzzySet()
{}

inline void FuzzySet::setValues(CREF(FuzzySetDefinition) values)
{
	m_fuzzySet = values;
}
inline LPFuzzySet FuzzySet::append(CREF(RDOValue) rdovalue, double appertain)
{
	std::pair<FuzzySet::FuzzySetDefinition::iterator, rbool> checkValue;
	checkValue = m_fuzzySet.insert(std::pair<RDOValue,double>(rdovalue, appertain));
	return LPFuzzySet(this);
}

inline LPFuzzySet FuzzySet::operator() (CREF(RDOValue) rdovalue, double appertain)
{
	return append(rdovalue, appertain);
}

inline REF(double) FuzzySet::operator[] (CREF(RDOValue) rdovalue)
{
	return m_fuzzySet[rdovalue];
}

inline FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::find(CREF(RDOValue) rdovalue) const
{
	return m_fuzzySet.find(rdovalue);
}

inline FuzzySet::FuzzyItem FuzzySet::findValue(CREF(RDOValue) rdovalue) const
{
	FuzzySetDefinition::const_iterator found = find(rdovalue);
	if (found != end()) return FuzzyItem(found->first, found->second);
	else                return FuzzyItem(rdovalue,     0.0          );
}

inline FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::begin() const { return m_fuzzySet.begin(); }
inline FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::end  () const { return m_fuzzySet.end();   }
inline FuzzySet::FuzzySetDefinition::iterator       FuzzySet::begin()       { return m_fuzzySet.begin(); }
inline FuzzySet::FuzzySetDefinition::iterator       FuzzySet::end  ()       { return m_fuzzySet.end();   }
inline rbool                                        FuzzySet::empty() const { return m_fuzzySet.empty(); }
inline rbool FuzzySet::inRange (CREF(RDOValue) rdovalue)
{
	return m_fuzzySet.find(rdovalue) != m_fuzzySet.end();
}
// --------------------------------------------------------------------------------
// -------------------- MemberFunctionProperties
// --------------------------------------------------------------------------------
inline LPFuzzySet MemberFunctionProperties::a_con (CREF(LPFuzzySet) pSet)
{
	return a_pow(pSet, 2.0);
}
inline LPFuzzySet MemberFunctionProperties::a_dil (CREF(LPFuzzySet) pSet)
{
	return a_pow(pSet, 0.5);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyTerm
// --------------------------------------------------------------------------------
inline CREF(LPFuzzySet) RDOFuzzyTerm::getFuzzySetDefinition() const
{
	return (m_term.second);
}
inline tstring RDOFuzzyTerm::getName() const
{
	return (m_term.first);
}
inline RDOFuzzyTerm::RDOFuzzyTerm(CREF(termName) pName, CREF(LPFuzzySet) pSet)
{
    m_term = std::make_pair(pName, pSet);
}

inline RDOFuzzyTerm::~RDOFuzzyTerm()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
inline RDOLingvoVariable::TermSet::const_iterator           RDOLingvoVariable::begin() const     {return m_set.begin();}
inline RDOLingvoVariable::TermSet::const_iterator           RDOLingvoVariable::end  () const     {return m_set.end  ();}
inline RDOLingvoVariable::~RDOLingvoVariable()
{}
inline RDOLingvoVariable::RDOLingvoVariable(CREF(LPRDOFuzzyTerm) term, nameOfVariable nameOfVariable)
{
	m_name = nameOfVariable;
	m_set.insert(make_pair(term->getName(), term->getFuzzySetDefinition()));
}
inline RDOLingvoVariable::RDOLingvoVariable(CREF(RDOValue)pDefineAreaValue, CREF(LPRDOLingvoVariable) variable)
	: m_name(_T("activated"))
{
	LPDefineArea   defineArea    = rdo::Factory<DefineArea>::create(pDefineAreaValue);
	LPFuzzySet     setOfVariable = rdo::Factory<FuzzySet>::create(defineArea);
	for (RDOLingvoVariable::TermSet::const_iterator it = variable->begin(); it != variable->end(); it++)
	{
		m_set.insert(std::pair<RDOFuzzyTerm::Term::first_type,LPFuzzySet>(it->first,setOfVariable));
	}
}
inline RDOLingvoVariable::RDOLingvoVariable(CREF(RDOLingvoVariable) variable)
{
	m_set = variable.m_set;
	m_name  = variable.m_name;
}
inline void RDOLingvoVariable::setName(nameOfVariable nameVariable)
{
	m_name = nameVariable;
}
inline REF(LPFuzzySet) RDOLingvoVariable::operator[] (tstring name)
{
	return m_set[name];
}
inline void RDOLingvoVariable::append(tstring name,CREF(LPFuzzySet) fuzzySet)
{
	operator[](name) = fuzzySet;
}
// --------------------------------------------------------------------------------
// -------------------- Statement
// --------------------------------------------------------------------------------
inline void Statement::setTerm(LPRDOFuzzyTerm term)
{
	m_term = term;
}

inline void Statement::setVariable(LPRDOLingvoVariable variable)
{
	m_variable = variable;
}
inline Statement::Statement()
{}
inline Statement::~Statement()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
