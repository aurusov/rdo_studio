/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy-inl.h
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

inline DefineArea::DefineArea(const RDOValue& leftRange, const RDOValue& rightRange)
{
	IntervalType mType = DomainPart::closed(leftRange, rightRange);
	m_domain = Domain(mType);
}
inline DefineArea::DefineArea(const RDOValue& value)
{
	IntervalType mType = DomainPart::closed(value,value);
	m_domain = Domain(mType);
}


// --------------------------------------------------------------------------------
// -------------------- FuzzySet
// --------------------------------------------------------------------------------
inline FuzzySet::FuzzySet()
{}

inline FuzzySet::FuzzySet(const LPDefineArea& pDefineArea)
	: m_defineArea(pDefineArea)
{}

inline FuzzySet::FuzzySet(const LPFuzzySet& pSet)
	: m_fuzzySet  (pSet->m_fuzzySet  )
	, m_defineArea(pSet->m_defineArea)
{}

inline FuzzySet::~FuzzySet()
{}

inline void FuzzySet::setValues(const FuzzySetDefinition& values)
{
	m_fuzzySet = values;
}

inline LPFuzzySet FuzzySet::append(const RDOValue& rdovalue, double appertain)
{
	std::pair<FuzzySet::FuzzySetDefinition::iterator, bool> checkValue;
	checkValue = m_fuzzySet.insert(std::pair<RDOValue,double>(rdovalue, appertain));
	return LPFuzzySet(this);
}

inline LPFuzzySet FuzzySet::operator() (const RDOValue& rdovalue, double appertain)
{
	return append(rdovalue, appertain);
}

inline double& FuzzySet::operator[] (const RDOValue& rdovalue)
{
	return m_fuzzySet[rdovalue];
}

inline FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::find(const RDOValue& rdovalue) const
{
	return m_fuzzySet.find(rdovalue);
}

inline FuzzySet::FuzzyItem FuzzySet::findValue(const RDOValue& rdovalue) const
{
	FuzzySetDefinition::const_iterator found = find(rdovalue);
	if (found != end()) return FuzzyItem(found->first, found->second);
	else                return FuzzyItem(rdovalue,     0.0          );
}

inline FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::begin() const { return m_fuzzySet.begin(); }
inline FuzzySet::FuzzySetDefinition::const_iterator FuzzySet::end  () const { return m_fuzzySet.end();   }
inline FuzzySet::FuzzySetDefinition::iterator       FuzzySet::begin()       { return m_fuzzySet.begin(); }
inline FuzzySet::FuzzySetDefinition::iterator       FuzzySet::end  ()       { return m_fuzzySet.end();   }
inline bool                                         FuzzySet::empty() const { return m_fuzzySet.empty(); }
inline bool FuzzySet::inRange (const RDOValue& rdovalue)
{
	return m_fuzzySet.find(rdovalue) != m_fuzzySet.end();
}
// --------------------------------------------------------------------------------
// -------------------- MemberFunctionProperties
// --------------------------------------------------------------------------------
inline LPFuzzySet MemberFunctionProperties::a_con (const LPFuzzySet& pSet)
{
	return a_pow(pSet, 2.0);
}
inline LPFuzzySet MemberFunctionProperties::a_dil (const LPFuzzySet& pSet)
{
	return a_pow(pSet, 0.5);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyTerm
// --------------------------------------------------------------------------------
inline const LPFuzzySet& RDOFuzzyTerm::getFuzzySetDefinition() const
{
	return (m_term.second);
}
inline std::string RDOFuzzyTerm::getName() const
{
	return (m_term.first);
}
inline RDOFuzzyTerm::RDOFuzzyTerm(const termName& pName, const LPFuzzySet& pSet)
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
inline RDOLingvoVariable::RDOLingvoVariable(const LPRDOFuzzyTerm& term, nameOfVariable nameOfVariable)
{
	m_name = nameOfVariable;
	m_set.insert(make_pair(term->getName(), term->getFuzzySetDefinition()));
}
inline RDOLingvoVariable::RDOLingvoVariable(const RDOValue&pDefineAreaValue, const LPRDOLingvoVariable& variable)
	: m_name("activated")
{
	LPDefineArea   defineArea    = rdo::Factory<DefineArea>::create(pDefineAreaValue);
	LPFuzzySet     setOfVariable = rdo::Factory<FuzzySet>::create(defineArea);
	for (RDOLingvoVariable::TermSet::const_iterator it = variable->begin(); it != variable->end(); it++)
	{
		m_set.insert(std::pair<RDOFuzzyTerm::Term::first_type,LPFuzzySet>(it->first,setOfVariable));
	}
}
inline RDOLingvoVariable::RDOLingvoVariable(const RDOLingvoVariable& variable)
{
	m_set = variable.m_set;
	m_name  = variable.m_name;
}
inline void RDOLingvoVariable::setName(nameOfVariable nameVariable)
{
	m_name = nameVariable;
}
inline LPFuzzySet& RDOLingvoVariable::operator[] (std::string name)
{
	return m_set[name];
}
inline void RDOLingvoVariable::append(std::string name, const LPFuzzySet& fuzzySet)
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
