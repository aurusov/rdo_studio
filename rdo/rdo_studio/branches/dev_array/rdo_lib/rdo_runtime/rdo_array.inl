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
	: m_container (value.m_container)
	, m_arrayType (value.m_arrayType)
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
	m_container.push_back(pArray);
}

inline RDOArrayValue::Container::iterator RDOArrayValue::containerBegin()
{
	return m_container.begin();
}

inline RDOArrayValue::Container::iterator RDOArrayValue::containerEnd()
{
	return m_container.end();
}

inline void RDOArrayValue::insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.insert(itr,itrFst,itrLst);
}

inline void RDOArrayValue::eraseItems(Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.erase(itrFst,itrLst);
}

inline tstring RDOArrayValue::getAsString() const
{
	tstring ArrayName = _T("[");
	STL_FOR_ALL_CONST(Container, m_container, it)
	{
		ArrayName += it->getAsString();
		if(it != --m_container.end())
			ArrayName += _T(", ");
	}
	return ArrayName += _T("]");
}

inline CREF(RDOValue) RDOArrayValue::operator[] (CREF(RDOValue) ind)
{
	return m_container[ind.getInt()];
}

// ----------------------------------------------------------------------------
// ---------- RDOArrayIterator
// ----------------------------------------------------------------------------
inline RDOArrayIterator::RDOArrayIterator(CREF(RDOArrayIterator) iterator)
	: m_iterator(iterator.m_iterator)
{}

inline RDOArrayIterator::RDOArrayIterator(CREF(Iterator) iterator)
	: m_iterator(iterator)
{}

inline RDOArrayIterator::Iterator RDOArrayIterator::getIterator() const
{
	return m_iterator;
}

inline RDOArrayIterator::Iterator RDOArrayIterator::operator+ (rsint num)
{
	return m_iterator + num;
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
