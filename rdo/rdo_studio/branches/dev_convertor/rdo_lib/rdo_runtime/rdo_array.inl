/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.inl
 * author   : Чирков Михаил
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
inline RDOArrayValue::RDOArrayValue(CREF(LPRDOArrayType) pType)
	: m_pArrayType(pType)
{}

inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
	: m_pArrayType(value.m_pArrayType)
{
	m_arrayValue = value.m_arrayValue;
}

inline RDOArrayValue::~RDOArrayValue()
{}

inline CREF(LPRDOArrayType) RDOArrayValue::type() const
{
	return m_pArrayType;
}

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType()
	: RDOType(RDOType::t_array)
{}

inline RDOArrayType::RDOArrayType(CREF(LPArrayType) pArrayType)
	: RDOType     (RDOType::t_array)
	, m_pArrayType(pArrayType      )
{}

inline RDOValue RDOArrayType::cast(CREF(RDOValue) from) const
{
	switch (from.typeID())
	{
		case RDOType::t_array: 
		{
			if (this == from.type().get())
				return from;
			break;
		}
	}
	throw RDOTypeException();
}

//inline tstring RDOArrayType::asString() const
//{
//	ASSERT(m_arrayType);
//	return rdo::format(_T("array<%s>"), m_arrayType->asString().c_str());
//}

CLOSE_RDO_RUNTIME_NAMESPACE
