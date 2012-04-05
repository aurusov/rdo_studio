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
// -------------------- RDOFuzzyValue
// --------------------------------------------------------------------------------
inline RDOFuzzyValue::RDOFuzzyValue(CREF(LPRDOFuzzyType) pType)
	: m_pType(pType)
{}

inline RDOFuzzyValue::RDOFuzzyValue(CREF(LPRDOFuzzyValue) pFuzzyValue)
	: m_pType(pFuzzyValue->m_pType)
{
	m_fuzzySet = pFuzzyValue->m_fuzzySet;
	m_Domain   = pFuzzyValue->m_Domain;
}
inline RDOFuzzyValue::RDOFuzzyValue(CREF(RDOValue) range1,CREF(RDOValue) range2)
	: m_pType()
{
	m_Domain = boost::icl::construct<Domain>(range1,range2,boost::icl::interval_bounds::closed());
}

inline RDOFuzzyValue::~RDOFuzzyValue()
{}

inline REF(RDOFuzzyValue) RDOFuzzyValue::append(CREF(RDOValue) rdovalue, double appertain)
{
	operator[](rdovalue) = appertain;
	return *this;
}

inline REF(RDOFuzzyValue) RDOFuzzyValue::operator() (CREF(RDOValue) rdovalue, double appertain)
{
	return append(rdovalue, appertain);
}

inline REF(double) RDOFuzzyValue::operator[] (CREF(RDOValue) rdovalue)
{
//	if (!type()->inRange(rdovalue))
//		throw RDOValueException();

	return m_fuzzySet[rdovalue];
}

inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::find(CREF(RDOValue) rdovalue) const
{
	return m_fuzzySet.find(rdovalue);
}

inline RDOFuzzyValue::FuzzyItem RDOFuzzyValue::findValue(CREF(RDOValue) rdovalue) const
{
	FuzzySet::const_iterator found = find(rdovalue);
	if (found != end()) return FuzzyItem(found->first, found->second);
	else                return FuzzyItem(rdovalue,     0.0          );
}

inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::begin() const { return m_fuzzySet.begin(); }
inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::end  () const { return m_fuzzySet.end();   }
inline RDOFuzzyValue::FuzzySet::iterator       RDOFuzzyValue::begin()       { return m_fuzzySet.begin(); }
inline RDOFuzzyValue::FuzzySet::iterator       RDOFuzzyValue::end  ()       { return m_fuzzySet.end();   }
inline rbool                                   RDOFuzzyValue::empty() const { return m_fuzzySet.empty(); }
inline CREF(LPRDOFuzzyType)                    RDOFuzzyValue::type () const { return m_pType;            }

//inline LPRDOFuzzyValue RDOFuzzyValue::supplement() const
//{
//	return type()->getSupplement(LPRDOFuzzyValue(const_cast<PTR(RDOFuzzyValue)>(this)));
//}

inline LPRDOFuzzyValue RDOFuzzyValue::a_con     () const { return a_pow(2.0);                   }
inline LPRDOFuzzyValue RDOFuzzyValue::a_dil     () const { return a_pow(0.5);                   }
inline           rbool RDOFuzzyValue::inRange   (CREF(RDOValue) rdovalue)
{
	return m_fuzzySet.find(rdovalue) != m_fuzzySet.end();
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

//inline LPRDOFuzzyValue RDOFuzzyType::getSupplement(CREF(LPRDOFuzzyValue) pFuzzyValue) const
//{
//	return m_fuzzySetDefinition->getSupplement(pFuzzyValue);
//}

//inline LPRDOFuzzySetDefinition RDOFuzzyType::getDefinition() const
//{
//	return m_fuzzySetDefinition;
//}
// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyTerm
// --------------------------------------------------------------------------------
inline LPRDOFuzzyValue RDOFuzzyTerm::getFuzzySet() const
{
	return (m_fuzzySet);
}
inline tstring RDOFuzzyTerm::getName() const
{
	return (m_name);
}
inline RDOFuzzyTerm::RDOFuzzyTerm()
{}

inline RDOFuzzyTerm::~RDOFuzzyTerm()
{}


// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
inline RDOLingvoVariable::Set::const_iterator           RDOLingvoVariable::begin()     {return m_set.begin();}
inline RDOLingvoVariable::Set::const_iterator           RDOLingvoVariable::end  ()     {return m_set.end  ();}
inline RDOLingvoVariable::RDOLingvoVariable()
{}
inline RDOLingvoVariable::~RDOLingvoVariable()
{}
inline RDOLingvoVariable RDOLingvoVariable::append(CREF(TermInVariable) pair)
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
