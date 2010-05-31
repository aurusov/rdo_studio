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

inline CREF(RDOArrayType) RDOArrayValue::type() const
{
	return *m_arrayTape;
}

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
inline RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent, ArrayType pArrayType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_pArrayType    (pArrayType      )
{}

inline RDOArrayType::ArrayType RDOArrayType::getArrayType() const
{
	return m_pArrayType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
