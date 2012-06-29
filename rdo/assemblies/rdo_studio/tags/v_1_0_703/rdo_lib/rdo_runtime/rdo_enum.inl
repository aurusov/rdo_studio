namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
inline RDOEnumType::RDOEnumType(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent         )
	, RDOType         (RDOType::t_enum)
{}

inline RDOEnumType::RDOEnumType(PTR(RDORuntimeParent) parent, CREF(Enums) enums)
	: RDORuntimeObject(parent         )
	, RDOType         (RDOType::t_enum)
	, m_enum          (enums          )
{}

inline void RDOEnumType::add(CREF(tstring) next)
{
	m_enum.push_back(next);
}

inline ruint RDOEnumType::findEnum(CREF(tstring) val) const
{
	CIterator it = std::find(m_enum.begin(), m_enum.end(), val);
	return it != m_enum.end() ? it - m_enum.begin() : END;
}

inline rbool RDOEnumType::exist(CREF(tstring) val) const
{
	return findEnum(val) != END;
}

inline tstring RDOEnumType::asString() const
{
	return str();
}

inline RDOValue RDOEnumType::cast(CREF(RDOValue) from) const
{
	switch (from.typeID())
	{
		case RDOType::t_identificator: {
			return (findEnum(from.getIdentificator()) != END) ?
				RDOValue(*this, from.getIdentificator()) :
				RDOValue(g_unknow);
			break;
		}
		case RDOType::t_enum: {
			if (this == &from.type())
				return from;
			break;
		}
	}
	throw RDOTypeException();
}

inline rbool                        RDOEnumType::empty    () const { return m_enum.empty(); }
inline const RDOEnumType::CIterator RDOEnumType::begin    () const { return m_enum.begin(); }
inline const RDOEnumType::CIterator RDOEnumType::end      () const { return m_enum.end();   }
inline CREF(RDOEnumType::Enums)     RDOEnumType::getValues() const { return m_enum;	        }

inline tstring RDOEnumType::str() const
{
	tstring _str = _T("(");
	std::vector< tstring >::const_iterator it = begin();
	while (it != end())
	{
		_str += *it;
		it++;
		if (it != end())
		{
			_str += _T(", ");
		}
	}
	_str += _T(")");
	return _str;
}

} // namespace rdoRuntime
