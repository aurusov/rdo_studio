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
inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayType) type)
	: m_arrayTape(&type)
{}

inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
	: m_arrayTape(value.m_arrayTape)
{
	m_arrayValue = value.m_arrayValue;
}

inline RDOArrayValue::~RDOArrayValue()
{}

inline CREF(RDOArrayType)       RDOArrayValue::type() const { return *m_arrayTape; }

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent)
	: RDORuntimeObject(parent         )
	, RDOType         (RDOType::t_array)
{}
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent, CREF(ArrayType) arrayType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_arrayType     (arrayType       )
{}

inline RDOValue RDOArrayType::cast(CREF(RDOValue) from) const
{
	switch (from.typeID())
	{
		case RDOType::t_array: 
		{
			if (this == &from.type())
				return from;
			break;
		}
	}
	throw RDOTypeException();
}

inline tstring RDOArrayType::asString() const
{
	ASSERT(m_arrayType);
	return rdo::format(_T("array<%s>"), m_arrayType->asString().c_str());
}

CLOSE_RDO_RUNTIME_NAMESPACE
