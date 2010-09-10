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
	: m_arrayType(&type)
{}

inline RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
: m_Container (value.m_Container)
, m_arrayType (value.m_arrayType )
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

inline RDOArrayValue::Container::iterator RDOArrayValue::m_containerBegin()
{
	return m_Container.begin();
}

inline RDOArrayValue::Container::iterator RDOArrayValue::m_containerEnd()
{
	return m_Container.end();
}

inline tstring RDOArrayValue::getAsString() const
{
	tstring ArrayName = _T("[");
	STL_FOR_ALL_CONST(Container, m_Container, it)
	{
		ArrayName += it->getAsString();
		if(it != --m_Container.end())
			ArrayName += _T(", ");
	}
	return ArrayName += _T("]");
}

inline CREF(RDOValue) RDOArrayValue::extractItem(rsint num) const
{
	return m_Container[num];
}
//inline rbool RDOArrayValue::comparDim(CREF(RDOArrayValue) a_value)
//{
//	if(a_value.m_Container.size() == m_Container.size())
//	{
//		rsint ind = 0;
//		do 
//		{
//			if((a_value.m_Container.front().typeID() == RDOType::t_array)&&(m_Container.front().typeID() == RDOType::t_array))
//			{
//				m_Container[ind].getArray().comparDim(a_value.m_Container[ind].getArray());
//				ind++;
//			}
//			else
//			{
//				if(!((a_value.m_Container.front().typeID() == RDOType::t_array)&&(m_Container.front().typeID() == RDOType::t_array))) break;
//				else return false;
//			}
//		} while (ind < a_value.m_Container.size());
//		return true;
//	}
//	else return false;
//}

inline CREF(RDOArrayValue) RDOArrayValue::operator+ (CREF(RDOValue) rdovalue) 
{
	STL_FOR_ALL(Container, m_Container, it)
	{
		*it += rdovalue;
	}
	return *this;
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
