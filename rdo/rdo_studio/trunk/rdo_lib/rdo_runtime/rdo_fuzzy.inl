namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValueItem
// ----------------------------------------------------------------------------
inline RDOFuzzyValueItem::RDOFuzzyValueItem()
	: m_rdovalue (RDOValue())
	, m_appertain(0         )
{}

inline RDOFuzzyValueItem::RDOFuzzyValueItem(CREF(RDOValue) rdovalue, double appertain)
	: m_rdovalue (rdovalue )
	, m_appertain(appertain)
{
	     if (appertain < 0) m_appertain = 0;
	else if (appertain > 1) m_appertain = 1;
}

inline rbool RDOFuzzyValueItem::operator< (CREF(RDOFuzzyValueItem) value) const
{
	return m_rdovalue < value.m_rdovalue;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
inline RDOFuzzyValue::RDOFuzzyValue(CREF(RDOFuzzyType) type)
	: m_type(&type)
{}

inline RDOFuzzyValue::RDOFuzzyValue(CREF(RDOFuzzyValue) value)
	: m_type(value.m_type)
{
	m_fuzzySet = value.m_fuzzySet;
}

inline RDOFuzzyValue::~RDOFuzzyValue()
{}

inline REF(RDOFuzzyValue) RDOFuzzyValue::append(CREF(RDOValue) rdovalue, double appertain)
{
	if (type().inRange(rdovalue))
	{
		m_fuzzySet.push_back(RDOFuzzyValueItem(rdovalue, appertain));
	}
	return *this;
}

inline CREF(RDOFuzzyType)                      RDOFuzzyValue::type () const { return *m_type;            }
inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::begin() const { return m_fuzzySet.begin(); }
inline RDOFuzzyValue::FuzzySet::const_iterator RDOFuzzyValue::end  () const { return m_fuzzySet.end();   }

RDOValue fun_add (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 + value2; }
RDOValue fun_sub (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 - value2; }
RDOValue fun_mult(CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 * value2; }
RDOValue fun_div (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 / value2; }

inline RDOFuzzyValue RDOFuzzyValue::operator+ (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_add,  fuzzy_value); }
inline RDOFuzzyValue RDOFuzzyValue::operator- (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_sub,  fuzzy_value); }
inline RDOFuzzyValue RDOFuzzyValue::operator* (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_mult, fuzzy_value); }
inline RDOFuzzyValue RDOFuzzyValue::operator/ (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_div,  fuzzy_value); }

RDOValue fun_u_minus(CREF(RDOValue) value                 ) { return -value;                                     }
RDOValue fun_u_obr  (CREF(RDOValue) value                 ) { return RDOValue(1)/value;                          }
RDOValue fun_u_scale(CREF(RDOValue) value, PTR(void) scale) { return value * (*static_cast<PTR(double)>(scale)); }
RDOValue fun_u_log  (CREF(RDOValue) value                 ) { return value > 0 ? log(value.getDouble()) : 0;     }

inline RDOFuzzyValue RDOFuzzyValue::u_minus()             const { return ext_unary(fun_u_minus        ); }
inline RDOFuzzyValue RDOFuzzyValue::u_obr  ()             const { return ext_unary(fun_u_obr          ); }
inline RDOFuzzyValue RDOFuzzyValue::u_scale(double scale) const { return ext_unary(fun_u_scale, &scale); }
inline RDOFuzzyValue RDOFuzzyValue::u_log  ()             const { return ext_unary(fun_u_log          ); }

inline RDOFuzzyValue RDOFuzzyValue::suppliment() const { return type().getSuppliment(*this); }
inline RDOFuzzyValue RDOFuzzyValue::a_con     () const { return a_pow(2.0);                  }
inline RDOFuzzyValue RDOFuzzyValue::a_dil     () const { return a_pow(0.5);                  }

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
inline RDOFuzzyType::RDOFuzzyType(PTR(RDOFuzzySetDefinition) fuzzySetDefinition)
	: RDORuntimeParent    (fuzzySetDefinition->getParent())
	, RDOType             (t_fuzzy                        )
	, m_fuzzySetDefinition(fuzzySetDefinition             )
{
	m_fuzzySetDefinition->reparent(this);
}

inline RDOFuzzyType::~RDOFuzzyType()
{}

inline tstring RDOFuzzyType::asString() const
{
	return "RDOFuzzyType";
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

inline RDOFuzzyValue RDOFuzzyType::getSuppliment(CREF(RDOFuzzyValue) value) const
{
	return m_fuzzySetDefinition->getSuppliment(value);
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
inline RDOFuzzySetDefinition::RDOFuzzySetDefinition(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent)
{}

inline RDOFuzzySetDefinition::~RDOFuzzySetDefinition()
{}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
inline RDOFuzzySetDefinitionFixed::RDOFuzzySetDefinitionFixed(PTR(RDORuntimeParent) parent)
	: RDOFuzzySetDefinition(parent)
{}

inline RDOFuzzySetDefinitionFixed::~RDOFuzzySetDefinitionFixed()
{}

inline REF(RDOFuzzySetDefinitionFixed) RDOFuzzySetDefinitionFixed::append(CREF(RDOValue) rdovalue)
{
	push_back(rdovalue);
	return *this;
}

inline REF(RDOFuzzySetDefinitionFixed) RDOFuzzySetDefinitionFixed::operator() (CREF(RDOValue) rdovalue)
{
	return append(rdovalue);
}

inline rbool RDOFuzzySetDefinitionFixed::inRange(CREF(RDOValue) rdovalue) const
{
	return std::find(begin(), end(), rdovalue) != end();
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
inline RDOFuzzySetDefinitionRangeDiscret::RDOFuzzySetDefinitionRangeDiscret(PTR(RDORuntimeParent) parent, CREF(RDOValue) step)
	: RDOFuzzySetDefinition(parent)
	, m_step               (step  )
{}

inline RDOFuzzySetDefinitionRangeDiscret::~RDOFuzzySetDefinitionRangeDiscret()
{}

inline RDOFuzzySetDefinitionRangeDiscret::Range::Range(CREF(RDOValue) from, CREF(RDOValue) till)
	: m_from(from)
	, m_till(till)
{}

inline REF(RDOFuzzySetDefinitionRangeDiscret) RDOFuzzySetDefinitionRangeDiscret::append(CREF(RDOValue) from, CREF(RDOValue) till)
{
	if (from <= till)
	{
		if (!inRange(from) && !inRange(till))
		{
			m_range.push_back(Range(from, till));
		}
	}
	return *this;
}

inline REF(RDOFuzzySetDefinitionRangeDiscret) RDOFuzzySetDefinitionRangeDiscret::operator() (CREF(RDOValue) from, CREF(RDOValue) till)
{
	return append(from, till);
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
inline RDOFuzzyEmptyType::RDOFuzzyEmptyType(PTR(RDORuntimeParent) parent)
	: RDOFuzzyType(new RDOFuzzySetDefinitionEmpty(parent))
{}

inline RDOFuzzyEmptyType::~RDOFuzzyEmptyType()
{
	RDOFuzzyEmptyType::s_emptyType = NULL;
}

inline tstring RDOFuzzyEmptyType::asString() const
{
	return _T("[empty set]");
}

inline CREF(RDOFuzzyEmptyType) RDOFuzzyEmptyType::getInstance(PTR(RDORuntimeParent) parent)
{
	if (!RDOFuzzyEmptyType::s_emptyType)
	{
		RDOFuzzyEmptyType::s_emptyType = new RDOFuzzyEmptyType(parent);
	}
	return *RDOFuzzyEmptyType::s_emptyType;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionEmpty
// ----------------------------------------------------------------------------
inline RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::RDOFuzzySetDefinitionEmpty(PTR(RDORuntimeParent) parent)
	: RDOFuzzySetDefinition(parent)
{}

inline RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::~RDOFuzzySetDefinitionEmpty()
{}

inline rbool RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::inRange(CREF(RDOValue) rdovalue) const
{
	return false;
}

inline RDOFuzzyValue RDOFuzzyEmptyType::RDOFuzzySetDefinitionEmpty::getSuppliment(CREF(RDOFuzzyValue) value) const
{
	return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(value.type().getParent()));
}

} // namespace rdoRuntime
