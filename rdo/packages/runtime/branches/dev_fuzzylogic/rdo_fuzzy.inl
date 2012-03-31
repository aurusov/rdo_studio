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
// -------------------- RDOFuzzyValueRange
// --------------------------------------------------------------------------------
inline RDOFuzzyValueRange::RDOValue RDOFuzzyValueRange::findValue(CREF(RDOValue) rdovalue) const {return m_range.find(rdovalue);}
inline RDOFuzzyValueRange::Range::const_iterator RDOFuzzyValueRange::begin() const { return m_range.begin(); }
inline RDOFuzzyValueRange::Range::const_iterator RDOFuzzyValueRange::end  () const { return m_range.end();   }
inline rbool                                     RDOFuzzyValueRange::empty() const { return m_range.empty(); }

inline rbool RDOFuzzyValueRange::checkRange(RDOValue checkValue)
{
	return this->findValue(checkValue) != 0;
}
inline CREF(RDOValue) RDOFuzzyValueRange::getRange()
{
	return m_range;
}
inline RDOFuzzyValueRange::RDOFuzzyValueRange(LPRDOType typeOfValue)
{
	LPRDOValue emptyValue = rdo::Factory<RDOValue>::create(typeOfValue);
	m_range.insert(emptyValue);
}
inline RDOFuzzyValueRange::RDOFuzzyValueRange(CREF(RDOValue) range1,CREF(RDOValue) range2)
{
	Range::iterator it = m_range.begin();
	for(it = m_range.begin(); it != range2; it++)
			m_range.insert(*it);
}
// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyMembershipZ
// --------------------------------------------------------------------------------
RDOFuzzyMembershipZ::RDOFuzzyMembershipZ(double range1, double range2)
	: m_range1(range1)
	, m_range2(range2)
{}

inline double RDOFuzzyMembershipZ::getValue(double x)
{
	if(x <= range1)
			return (0)
		else if(x >= range2)
			return 1
			else return ('len dumat seychas')
}


// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyValue
// --------------------------------------------------------------------------------
inline RDOFuzzyValue::RDOFuzzyValue()
	: m_Range()
{
	RDOFuzzyValueRange::Range::const_iterator it = begin();
	for(it = begin(); it != end(); it++)
	m_fuzzySet[pRange[it]] = 1;
}

inline RDOFuzzyValue::RDOFuzzyValue(CREF(LPRDOFuzzyValueRange) pRange)
	: m_Range(pRange)
{
	RDOFuzzyValueRange::Range::const_iterator it = begin();
	for(it = begin(); it != end(); it++)
		m_fuzzySet[pRange[it]] = 1;
}

inline RDOFuzzyValue::RDOFuzzyValue(CREF(LPRDOFuzzyValueRange) pRange, CREF(LPRDOFuzzyMembership) pMembership)
	: m_Range(pRange)
{
	RDOFuzzyValueRange::Range::const_iterator it = begin();
	for(it = begin(); it != end(); it++)
		m_fuzzySet[pRange[it]] = pMembership->getValue(pRange[it]);
}

inline RDOFuzzyValue::RDOFuzzyValue(CREF(LPRDOFuzzyValue) pFuzzyValue)
{
	m_fuzzySet        = pFuzzyValue->m_fuzzySet;
	m_Range           = pFuzzyValue->m_Range;
}

inline RDOFuzzyValue::~RDOFuzzyValue()
{}

inline double RDOFuzzyValue::getAppertain(CREF(RDOValue) value)
{
	operator[](rdovalue) = appertain;
	double castValue = value->getDouble();
	return (m_fuzzyMembershipForm->getValue(castValue));
}
+inline REF(RDOFuzzyValue) RDOFuzzyValue::append(CREF(RDOValue) rdovalue)
+{
	+	operator[](rdovalue) = this->getAppertain(rdovalue);
	return *this;
}

inline REF(RDOFuzzyValue) RDOFuzzyValue::operator() (CREF(RDOValue) rdovalue)
{
	return append(rdovalue);
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
inline CREF(LPRDOType) RDOFuzzyValue::type()
{
	FuzzySet::iterator it = m_fuzzySet->begin();
	return (it->first->type());
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyTerm
// --------------------------------------------------------------------------------
inline RDOFuzzyTerm::~RDOFuzzyTerm()
{}

inline tstring RDOFuzzyTerm::name() const
{
	return _T("RDOFuzzyTerm");
}

inline RDOValue RDOFuzzyTerm::value_cast(CREF(RDOValue) from) const
{
	UNUSED(from);
	throw RDOTypeException();
}

//inline rbool RDOFuzzyType::operator== (CREF(RDOFuzzyType) type) const
//{
//	return this == &type;
//}

//inline rbool RDOFuzzyType::operator!= (CREF(RDOFuzzyType) type) const
//{
//	return !operator==(type);
//}

//inline LPRDOFuzzyValue RDOFuzzyTerm::getSupplement(CREF(LPRDOFuzzyValue) pFuzzyValue) const
//{
//	return m_fuzzySetDefinition->getSupplement(pFuzzyValue);
//}

//inline LPRDOFuzzySetDefinition RDOFuzzyType::getDefinition() const
//{
//	return m_fuzzySetDefinition;
//}
inline CREF(RDOFuzzyTerm::Term) RDOFuzzyTerm::getTerm() const
{
	return m_termSet;
}
// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
inline RDOLingvoVariable::RDOLingvoVariable()
	: name("")
{
	RDOLingvoVariable::TermSet::const_iterator it = m_TermSet.begin();
	for(it = begin(); it != end(); it++)
		TermSet[it] = rdo:
}

inline RDOLingvoVariable RDOLingvoVariable::append(CREF(RDOFuzzyTerm) term)
{
	m_TermSet->insert(term);
}

inline RDOLingvoVariable::TermSet::const_iterator           RDOLingvoVariable::begin()     {return m_set.begin();}
inline RDOLingvoVariable::TermSet::const_iterator           RDOLingvoVariable::end  ()     {return m_set.end  ();}

CLOSE_RDO_RUNTIME_NAMESPACE
