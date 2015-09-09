// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDORuntime_Array_Test
#include "utils/src/common/warning_disable.h"
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORuntime_Array_Test)

typedef std::vector<int> Container;
typedef std::pair<rdo::runtime::LPRDOArrayValue, rdo::runtime::RDOValue> Array;

Array createArray(const Container& data)
{
    rdo::runtime::LPRDOArrayType pType = rdo::Factory<rdo::runtime::RDOArrayType>::create(rdo::runtime::g_INT);
    ASSERT(pType);
    rdo::runtime::LPRDOArrayValue pValue = rdo::Factory<rdo::runtime::RDOArrayValue>::create(pType);
    ASSERT(pValue);

    for (const auto& item: data)
        pValue->push_back(rdo::runtime::RDOValue(item));

    return std::make_pair(pValue, rdo::runtime::RDOValue(pType, pValue));
}

std::string getString(const rdo::runtime::LPRDOArrayValue& pArray, const rdo::runtime::LPRDOArrayIterator& pIt)
{
    return !pIt->equal(pArray->end())
            ? pIt->getValue().getAsString()
            : std::string();
}

std::string getString(const rdo::runtime::RDOValue& it, const rdo::runtime::RDOValue& end)
{
    return it != end
            ? it.getAsString()
            : std::string();
}

BOOST_AUTO_TEST_CASE(ArrayTestCreate)
{
    BOOST_CHECK(createArray({1, 2, 3}).second.getAsString() == "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE(ArrayTestInsert)
{
    Array array1 = createArray({1, 2, 3});
    Array array2 = createArray({4, 5, 6});

    array1.first->insert(array1.first->begin()->next(), array2.first->begin(), array2.first->end());

    BOOST_CHECK(array1.second.getAsString() == "[1, 4, 5, 6, 2, 3]");
}

BOOST_AUTO_TEST_CASE(ArrayTestErase)
{
    Array array = createArray({1, 2, 3});

    array.first->erase(array.first->begin()->next(), array.first->begin()->preInc(3));

    BOOST_CHECK(array.second.getAsString() == "[1]");
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPrePlus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt = array.first->begin();
    while (!pIt->equal(array.first->end()))
        result += getString(array.first, pIt->preInc(1));

    BOOST_CHECK(result == "23");
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPostPlus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt = array.first->begin();
    while (!pIt->equal(array.first->end()))
        result += getString(array.first, pIt->postInc(1));

    BOOST_CHECK(result == "123");
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPreMinus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt = array.first->end();
    do
        result += getString(array.first, pIt->preInc(-1));
    while (!pIt->equal(array.first->begin()));

    BOOST_CHECK(result == "321");
}

BOOST_AUTO_TEST_CASE(ArrayTestIteratorPostMinus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt = array.first->end();
    do
        result += getString(array.first, pIt->postInc(-1));
    while (!pIt->equal(array.first->begin()));

    BOOST_CHECK(result == "32");
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePrePlus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt  = array.first->begin();
    rdo::runtime::LPRDOArrayIterator pEnd = array.first->end  ();
    rdo::runtime::RDOValue it (pIt,  pIt );
    rdo::runtime::RDOValue end(pEnd, pEnd);
    BOOST_CHECK(it != end);

    while (it != end)
        result += getString(++it, end);

    BOOST_CHECK(result == "23");
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePostPlus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt  = array.first->begin();
    rdo::runtime::LPRDOArrayIterator pEnd = array.first->end  ();
    rdo::runtime::RDOValue it (pIt,  pIt );
    rdo::runtime::RDOValue end(pEnd, pEnd);
    BOOST_CHECK(it != end);

    while (it != end)
        result += getString(it++, end);

    BOOST_CHECK(result == "123");
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePreMinus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt    = array.first->end  ();
    rdo::runtime::LPRDOArrayIterator pBegin = array.first->begin();
    rdo::runtime::LPRDOArrayIterator pEnd   = array.first->end  ();
    rdo::runtime::RDOValue it   (pIt,    pIt   );
    rdo::runtime::RDOValue begin(pBegin, pBegin);
    rdo::runtime::RDOValue end  (pEnd,   pEnd  );
    BOOST_CHECK(it != begin);
    BOOST_CHECK(it == end  );

    do
        result += getString(--it, end);
    while (it != begin);
    BOOST_CHECK(result == "321");
}

BOOST_AUTO_TEST_CASE(ArrayTestValuePostMinus)
{
    Array array = createArray({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOArrayIterator pIt    = array.first->end  ();
    rdo::runtime::LPRDOArrayIterator pBegin = array.first->begin();
    rdo::runtime::LPRDOArrayIterator pEnd   = array.first->end  ();
    rdo::runtime::RDOValue it   (pIt,    pIt   );
    rdo::runtime::RDOValue begin(pBegin, pBegin);
    rdo::runtime::RDOValue end  (pEnd,   pEnd  );
    BOOST_CHECK(it != begin);
    BOOST_CHECK(it == end  );

    do
        result += getString(it--, end);
    while (it != begin);
    BOOST_CHECK(result == "32");
}

BOOST_AUTO_TEST_CASE(ArrayTestSetItem)
{
    Array array = createArray({1, 2, 3});

    std::size_t ind = 1;
    std::size_t item = 48;
    rdo::runtime::RDOValue index(ind);
    rdo::runtime::RDOValue value(item);
    array.first->setItem(index, value);

    BOOST_CHECK(array.second.getAsString() == "[1, 48, 3]");

    ind = 3;
    index = ind;
    bool found = false;

    try
    {
        array.first->setItem(index, value);
    }
    catch (const rdo::runtime::RDORuntimeException& ex)
    {
        if (!ex.message().empty())
            found = ex.message() == "Выход за пределы массива";
    }

    if (!found)
        BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(ArrayTestGetItem)
{
    Array array = createArray({1, 48, 3});

    std::size_t ind = 1;
    rdo::runtime::RDOValue index(ind);
    rdo::runtime::RDOValue value(array.first->getItem(index));

    BOOST_CHECK(value.getAsString() == "48");

    ind = 3;
    index = ind;
    bool found = false;

    try
    {
        array.first->getItem(index);
    }
    catch (const rdo::runtime::RDORuntimeException& ex)
    {
        if (!ex.message().empty())
            found = ex.message() == "Выход за пределы массива";
    }

    if (!found)
        BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Array_Test
