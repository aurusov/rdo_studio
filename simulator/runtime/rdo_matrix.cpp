// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOMatrixValue
// --------------------------------------------------------------------------------
RDOMatrixValue::RDOMatrixValue(const LPRDOMatrixType& pType)
    : m_pMatrixType(pType)
{}

RDOMatrixValue::RDOMatrixValue(const LPRDOMatrixValue& pValue)
    : m_container  (pValue->m_container  )
    , m_pMatrixType(pValue->m_pMatrixType)
{}

RDOMatrixValue::~RDOMatrixValue()
{}

const LPRDOMatrixType& RDOMatrixValue::type() const
{
    ASSERT(m_pMatrixType);
    return m_pMatrixType;
}

void RDOMatrixValue::push_back(const RDOValue& item)
{
    m_container.push_back(item);
}

LPRDOMatrixIterator RDOMatrixValue::begin()
{
    return rdo::Factory<RDOMatrixIterator>::create(m_container.begin());
}

LPRDOMatrixIterator RDOMatrixValue::end()
{
    return rdo::Factory<RDOMatrixIterator>::create(m_container.end());
}

void RDOMatrixValue::insert(const LPRDOMatrixIterator& pWhere, const LPRDOMatrixIterator& pFromFirst, const LPRDOMatrixIterator& pFromLast)
{
    ASSERT(pWhere    );
    ASSERT(pFromFirst);
    ASSERT(pFromLast );

    m_container.insert(pWhere->getIterator(), pFromFirst->getIterator(), pFromLast->getIterator());
}

void RDOMatrixValue::erase(const LPRDOMatrixIterator& pFirst, const LPRDOMatrixIterator& pLast)
{
    ASSERT(pFirst)
    ASSERT(pLast );

    m_container.erase(pFirst->getIterator(), pLast->getIterator());
}

std::string RDOMatrixValue::asString() const
{
    std::string result("[");
    for (Container::const_iterator item = m_container.begin(); item != m_container.end(); ++item)
    {
        if (item == m_container.begin())
        {
            result = rdo::format("%s%s", result.c_str(), item->getAsString().c_str());
        }
        else
        {
            result = rdo::format("%s, %s", result.c_str(), item->getAsString().c_str());
        }
    }
    return rdo::format("%s]", result.c_str());
}

std::size_t RDOMatrixValue::size() const
{
    return m_container.size();
}

const RDOValue& RDOMatrixValue::getItem(const RDOValue& index) const
{
    std::size_t ind = index.getUInt();
    if (ind >= m_container.size())
    {
        throw RDORuntimeException("Выход за пределы матрицы");
    }
    return m_container[ind];
}

void RDOMatrixValue::setItem(const RDOValue& index, const RDOValue& item)
{
    std::size_t ind = index.getUInt();
    if (ind >= m_container.size())
    {
        throw RDORuntimeException("Выход за пределы матрицы");
    }
    m_container[ind] = item;
}

// --------------------------------------------------------------------------------
// -------------------- RDOMatrixIterator
// --------------------------------------------------------------------------------
RDOMatrixIterator::RDOMatrixIterator(const LPRDOMatrixIterator& pIterator)
    : RDOType   (RDOType::Type::POINTER   )
    , m_iterator(pIterator->m_iterator)
{}

RDOMatrixIterator::RDOMatrixIterator(const RDOMatrixIterator& iterator)
    : RDOType   (RDOType::Type::POINTER )
    , m_iterator(iterator.m_iterator)
{}

RDOMatrixIterator::RDOMatrixIterator(const Iterator& iterator)
    : RDOType   (RDOType::Type::POINTER)
    , m_iterator(iterator          )
{}

RDOMatrixIterator::~RDOMatrixIterator()
{}

RDOMatrixIterator::Iterator RDOMatrixIterator::getIterator() const
{
    return m_iterator;
}

const RDOValue& RDOMatrixIterator::getValue() const
{
    return *m_iterator;
}

LPRDOMatrixIterator RDOMatrixIterator::preInc(int delta)
{
    m_iterator += delta;
    return LPRDOMatrixIterator(this);
}

LPRDOMatrixIterator RDOMatrixIterator::postInc(int delta)
{
    LPRDOMatrixIterator pPrev = rdo::Factory<RDOMatrixIterator>::create(m_iterator);
    ASSERT(pPrev);
    m_iterator += delta;
    return pPrev;
}

LPRDOMatrixIterator RDOMatrixIterator::next()
{
    return preInc(1);
}

bool RDOMatrixIterator::equal(const LPRDOMatrixIterator& pIterator) const
{
    ASSERT(pIterator);
    return m_iterator == pIterator->m_iterator;
}

LPRDOMatrixIterator RDOMatrixIterator::clone() const
{
    return rdo::Factory<RDOMatrixIterator>::create(*this);
}

std::string RDOMatrixIterator::asString() const
{
    return getValue().getAsString();
}

// --------------------------------------------------------------------------------
// -------------------- RDOMatrixType
// --------------------------------------------------------------------------------
RDOMatrixType::RDOMatrixType(const LPItemType& pItemType)
    : RDOType    (RDOType::Type::POINTER)
    , m_pItemType(pItemType         )
{}

RDOMatrixType::~RDOMatrixType()
{}

const RDOMatrixType::LPItemType& RDOMatrixType::getItemType() const
{
    return m_pItemType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
