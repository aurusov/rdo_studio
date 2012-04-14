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

}

//inline void DefineArea::appendDomain(CREF(DomainPart))
//{
//	m_domain.assignDomain(CREF(DomainPart));
//}

//inline rbool DefineArea::empty() const
//{
//	if (m_domain.lower() == m_domain.upper())
//		return true;
//}
//inline rbool DefineArea::inDomain(RDOValue pValue)const
//{
//	if (pValue <= m_domain.lower() || pValue >= m_domain.upper())
//		return false;
//}

// --------------------------------------------------------------------------------
// -------------------- FuzzySet
// --------------------------------------------------------------------------------
inline FuzzySet::FuzzySet()
{}
inline FuzzySet::FuzzySet(CREF(LPRDOFuzzyType) pType)
	: m_pType(pType)
{}

inline FuzzySet::FuzzySet(CREF(LPRDOFuzzyType)pType, CREF(DefineArea) pDefineArea)
	: m_pType     (pType)
	, m_defineArea(pDefineArea)
{}
inline FuzzySet::FuzzySet(CREF(LPFuzzySet) pSet)
	: m_pType     (pSet->m_pType)
	, m_fuzzySet  (pSet->m_fuzzySet)
	, m_defineArea(pSet->m_defineArea)
{}
inline FuzzySet::FuzzySet(CREF(FuzzySetDefinition) pSetDefinition)
	: m_fuzzySet(pSetDefinition)
{}
inline FuzzySet::~FuzzySet()
{}

inline void FuzzySet::setValues(CREF(FuzzySetDefinition) values)
{
	m_fuzzySet = values;
}
inline REF(FuzzySet) FuzzySet::append(CREF(RDOValue) rdovalue, double appertain)
{
	operator[](rdovalue) = appertain;
	return *this;
}

inline REF(FuzzySet) FuzzySet::operator() (CREF(RDOValue) rdovalue, double appertain)
{
	return append(rdovalue, appertain);
}

inline REF(double) FuzzySet::operator[] (CREF(RDOValue) rdovalue)
{
//	if (!type()->inRange(rdovalue))
//		throw RDOValueException();

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
inline CREF(LPRDOFuzzyType)                         FuzzySet::type () const { return m_pType;            }
inline rbool FuzzySet::inRange (CREF(RDOValue) rdovalue)
{
	return m_fuzzySet.find(rdovalue) != m_fuzzySet.end();
}
//inline double FuzzySet::appertain(CREF(RDOValue) value)
//{
//	FuzzySetDefinition::const_iterator it = m_fuzzySet.find(value);
//	return it->second;
//}
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
// -------------------- RDOFuzzyType
// --------------------------------------------------------------------------------
inline RDOFuzzyType::RDOFuzzyType()
	: RDOType (t_pointer)
{
	//! Было
	//! m_fuzzySetDefinition->reparent(this);
	/// @todo для порядку перевести на умные указатели
}
inline RDOFuzzyType::~RDOFuzzyType()
{}

inline RDOValue RDOFuzzyType::value_cast(CREF(RDOValue) from) const
{
	UNUSED(from);
	throw RDOTypeException();
}

//inline rbool RDOFuzzyType::operator== (CREF(RDOFuzzyType) type) const
//{
//	return this == &type;
//}
//
//inline rbool RDOFuzzyType::operator!= (CREF(RDOFuzzyType) type) const
//{
//	return !operator==(type);
//}

//inline LPFuzzySet RDOFuzzyType::getSupplement(CREF(LPFuzzySet) pFuzzyValue) const
//{
//	return m_fuzzySetDefinition->getSupplement(pFuzzyValue);
//}

//inline LPRDOFuzzySetDefinitionDefinition RDOFuzzyType::getDefinition() const
//{
//	return m_fuzzySetDefinition;
//}
// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyTerm
// --------------------------------------------------------------------------------
inline CREF(FuzzySet) RDOFuzzyTerm::getFuzzySetDefinition() const
{
	return (m_term.second);
}
inline tstring RDOFuzzyTerm::getName() const
{
	return (m_term.first);
}
inline RDOFuzzyTerm::RDOFuzzyTerm()
{
	termName name = _T("undefined");
	m_term.first = name;
}
inline RDOFuzzyTerm::RDOFuzzyTerm(CREF(termName) pName, CREF(FuzzySet) pSet)
{
	m_term = std::make_pair(pName,pSet);
}

inline RDOFuzzyTerm::~RDOFuzzyTerm()
{}


// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
inline RDOLingvoVariable::TermSet::const_iterator           RDOLingvoVariable::begin()     {return m_set.begin();}
inline RDOLingvoVariable::TermSet::const_iterator           RDOLingvoVariable::end  ()     {return m_set.end  ();}
inline RDOLingvoVariable::RDOLingvoVariable()
{}
inline RDOLingvoVariable::~RDOLingvoVariable()
{}
inline RDOLingvoVariable RDOLingvoVariable::append(CREF(RDOFuzzyTerm::Term) pair)
{
	m_set.insert(pair);
	return *this;
}
inline RDOLingvoVariable::RDOLingvoVariable(CREF(RDOLingvoVariable) variable) 
{
	m_set = variable.m_set;
	name  = variable.name;
}

CLOSE_RDO_RUNTIME_NAMESPACE