/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_matrix.inl
 * author   : Чирков Михаил
 * date     : 01.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOMatrixValue
// ----------------------------------------------------------------------------
inline RDOMatrixValue::RDOMatrixValue(CREF(RDOMatrixType) type)
	: m_matrixType(&type)
{}

inline RDOMatrixValue::RDOMatrixValue(CREF(RDOMatrixValue) value)
	: m_container (value.m_container)
	, m_matrixType (value.m_matrixType)
{}

inline RDOMatrixValue::~RDOMatrixValue()
{}

inline CREF(RDOMatrixType) RDOMatrixValue::type() const
{
	ASSERT(m_matrixType);
	return *m_matrixType;
}

inline void RDOMatrixValue::insertItem(CREF(RDOValue) pMatrix)
{
	m_container.push_back(pMatrix);
}

inline RDOMatrixValue::Container::iterator RDOMatrixValue::containerBegin()
{
	return m_container.begin();
}

inline RDOMatrixValue::Container::iterator RDOMatrixValue::containerEnd()
{
	return m_container.end();
}

inline void RDOMatrixValue::insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.insert(itr,itrFst,itrLst);
}

inline void RDOMatrixValue::eraseItems(Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.erase(itrFst,itrLst);
}

inline tstring RDOMatrixValue::getAsString() const
{
	tstring MatrixName = _T("[");
	STL_FOR_ALL_CONST(Container, m_container, it)
	{
		MatrixName += it->getAsString();
		if(it != --m_container.end())
			MatrixName += _T(", ");
	}
	return MatrixName += _T("]");
}

inline CREF(RDOValue) RDOMatrixValue::operator[] (CREF(RDOValue) ind)
{
	return m_container[ind.getInt()];
}

// ----------------------------------------------------------------------------
// ---------- RDOMatrixIterator
// ----------------------------------------------------------------------------
inline RDOMatrixIterator::RDOMatrixIterator(CREF(RDOMatrixIterator) iterator)
	: m_iterator(iterator.m_iterator)
{}

inline RDOMatrixIterator::RDOMatrixIterator(CREF(Iterator) iterator)
	: m_iterator(iterator)
{}

inline RDOMatrixIterator::Iterator RDOMatrixIterator::getIterator() const
{
	return m_iterator;
}

inline RDOMatrixIterator::Iterator RDOMatrixIterator::operator+ (rsint num)
{
	return m_iterator + num;
}

inline RDOMatrixIterator::Iterator RDOMatrixIterator::operator- (rsint num)
{
	return m_iterator - num;
}

inline RDOValue RDOMatrixIterator::getValue() const
{
	return *m_iterator;
}

inline rbool RDOMatrixIterator::operator== (CREF(RDOMatrixIterator) iterator) const
{
	return m_iterator == iterator.m_iterator;
}

// ----------------------------------------------------------------------------
// ---------- RDOMatrixType
// ----------------------------------------------------------------------------
inline RDOMatrixType::RDOMatrixType(PTR(RDORuntimeParent) parent, MatrixType pMatrixType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_matrix)
	, m_pMatrixType    (pMatrixType      )
{}

inline RDOMatrixType::MatrixType RDOMatrixType::getMatrixType() const
{
	return m_pMatrixType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
