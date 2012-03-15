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
	if (!type()->inRange(rdovalue))
		throw RDOValueException();

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

inline LPRDOFuzzyValue RDOFuzzyValue::supplement() const
{
	return type()->getSupplement(LPRDOFuzzyValue(const_cast<PTR(RDOFuzzyValue)>(this)));
}

inline LPRDOFuzzyValue RDOFuzzyValue::a_con     () const { return a_pow(2.0);                   }
inline LPRDOFuzzyValue RDOFuzzyValue::a_dil     () const { return a_pow(0.5);                   }

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyType
// --------------------------------------------------------------------------------
inline RDOFuzzyType::RDOFuzzyType(CREF(LPRDOFuzzySetDefinition) pFuzzySetDefinition)
	: RDOType             (t_pointer          )
	, m_fuzzySetDefinition(pFuzzySetDefinition)
{
	//! Было
	//! m_fuzzySetDefinition->reparent(this);
	/// @todo для порядку перевести на умные указатели
}

//inline RDOFuzzyType::TermSet::const_iterator       RDOFuzzyType::begin()       { return m_termSet.begin(); }
//inline RDOFuzzyType::TermSet::const_iterator       RDOFuzzyType::end  ()       { return m_termSet.end();   }

inline RDOFuzzyType::~RDOFuzzyType()
{}

inline tstring RDOFuzzyType::name() const
{
	return _T("RDOFuzzyType");
}

inline RDOValue RDOFuzzyType::value_cast(CREF(RDOValue) from) const
{
	UNUSED(from);
	throw RDOTypeException();
}

inline rbool RDOFuzzyType::operator== (CREF(RDOFuzzyType) type) const
{
	return this == &type;
}

inline rbool RDOFuzzyType::operator!= (CREF(RDOFuzzyType) type) const
{
	return !operator==(type);
}

inline rbool RDOFuzzyType::inRange(CREF(RDOValue) rdovalue) const
{
	return m_fuzzySetDefinition->inRange(rdovalue);
}

inline LPRDOFuzzyValue RDOFuzzyType::getSupplement(CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	return m_fuzzySetDefinition->getSupplement(pFuzzyValue);
}

inline CREF(RDOFuzzyType::TermSet) RDOFuzzyType::getTermSet() const
{
	return m_termSet;
}
// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
inline RDOLingvoVariable::Set::const_iterator           RDOLingvoVariable::begin()     {return m_set.begin();}
inline RDOLingvoVariable::Set::const_iterator           RDOLingvoVariable::end  ()     {return m_set.end  ();}
inline RDOLingvoVariable::RDOLingvoVariable()
{}
inline REF(RDOLingvoVariable) RDOLingvoVariable::append(tstring term, CREF(LPRDOFuzzyValue) value)
{
	m_set[term] = value;
	return *this;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzySetDefinition
// --------------------------------------------------------------------------------
inline RDOFuzzySetDefinition::RDOFuzzySetDefinition()
{}

inline RDOFuzzySetDefinition::~RDOFuzzySetDefinition()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzySetDefinitionFixed
// --------------------------------------------------------------------------------
inline RDOFuzzySetDefinitionFixed::RDOFuzzySetDefinitionFixed()
	: RDOFuzzySetDefinition()
{}

inline RDOFuzzySetDefinitionFixed::~RDOFuzzySetDefinitionFixed()
{}

inline REF(RDOFuzzySetDefinitionFixed) RDOFuzzySetDefinitionFixed::append(CREF(RDOValue) rdovalue)
{
	m_items[rdovalue] = 1.0;
	return *this;
}

inline REF(RDOFuzzySetDefinitionFixed) RDOFuzzySetDefinitionFixed::operator() (CREF(RDOValue) rdovalue)
{
	return append(rdovalue);
}

inline rbool RDOFuzzySetDefinitionFixed::inRange(CREF(RDOValue) rdovalue) const
{
	return m_items.find(rdovalue) != m_items.end();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzySetDefinitionRangeDiscret
// --------------------------------------------------------------------------------
inline RDOFuzzySetDefinitionRangeDiscret::RDOFuzzySetDefinitionRangeDiscret(CREF(RDOValue) from, CREF(RDOValue) till, CREF(RDOValue) step)
	: RDOFuzzySetDefinition(    )
	, m_from               (from)
	, m_till               (till)
	, m_step               (step)
{}

inline RDOFuzzySetDefinitionRangeDiscret::~RDOFuzzySetDefinitionRangeDiscret()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyEmptyType
// --------------------------------------------------------------------------------
inline RDOFuzzyEmptyType::RDOFuzzyEmptyType()
	: RDOFuzzyType(rdo::Factory<RDOFuzzySetDefinitionEmpty>::create())
{}

inline RDOFuzzyEmptyType::~RDOFuzzyEmptyType()
{
	RDOFuzzyEmptyType::s_emptyType = NULL;
}

inline tstring RDOFuzzyEmptyType::asString() const
{
	return _T("[empty set]");
}

inline LPRDOFuzzyType RDOFuzzyEmptyType::getInstance()
{
	if (!RDOFuzzyEmptyType::s_emptyType)
	{
		RDOFuzzyEmptyType::s_emptyType = new RDOFuzzyEmptyType();
	}
	return LPRDOFuzzyType(RDOFuzzyEmptyType::s_emptyType);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty
// --------------------------------------------------------------------------------
inline RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::RDOFuzzySetDefinitionEmpty()
	: RDOFuzzySetDefinition()
{}

inline RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::~RDOFuzzySetDefinitionEmpty()
{}

inline rbool RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::inRange(CREF(RDOValue) rdovalue) const
{
	UNUSED(rdovalue);
	return false;
}

inline LPRDOFuzzyValue RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::getSupplement(CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	UNUSED(pFuzzyValue);
	return rdo::Factory<RDOFuzzyValue>::create(RDOFuzzyEmptyType::getInstance());
}

CLOSE_RDO_RUNTIME_NAMESPACE
