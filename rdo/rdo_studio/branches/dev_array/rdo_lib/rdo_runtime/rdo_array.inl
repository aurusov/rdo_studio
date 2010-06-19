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
inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
	: m_Container (value.m_Container)
	, m_arrayType (value.m_arrayType )
{}

inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayType) type)
	: m_arrayType(&type)
{}

inline RDOArrayValue::~RDOArrayValue()
{}

inline CREF(RDOArrayType) RDOArrayValue::type() const
{
	ASSERT(m_arrayType);
	return *m_arrayType;
}

inline void RDOArrayValue::insertItem(CREF(RDOValue) pArray)
{
	m_Container.push_back(pArray);
}

inline RDOArrayValue::Container::const_iterator RDOArrayValue::m_containerBegin()
{
	return m_Container.begin();
}

inline RDOArrayValue::Container::const_iterator RDOArrayValue::m_containerEnd()
{
	return m_Container.end();
}

//inline tstring RDOArrayValue::getAsString()
//{
//	tstring ArrayName = rdo::format(_T('['));
//	STL_FOR_ALL_CONST(ArrayValue, m_arrayValue, it)
//	{
//		ArrayName += it->getAsString();
//	}
//	return ArrayName += _T(']');
//}

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
