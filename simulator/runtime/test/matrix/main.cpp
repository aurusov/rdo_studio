// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDORuntime_Matrix_Test
#include "utils/src/common/warning_disable.h"
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORuntime_Matrix_Test)

typedef std::vector<int> Container;
typedef std::pair<rdo::runtime::LPRDOMatrixValue, rdo::runtime::RDOValue> Matrix;

Matrix createMatrix(const Container& data)
{
    rdo::runtime::LPRDOMatrixType pType = rdo::Factory<rdo::runtime::RDOMatrixType>::create(rdo::runtime::g_INT);
    ASSERT(pType);
    rdo::runtime::LPRDOMatrixValue pValue = rdo::Factory<rdo::runtime::RDOMatrixValue>::create(pType);
    ASSERT(pValue);

    for (const auto& item: data)
        pValue->push_back(rdo::runtime::RDOValue(item));

    return std::make_pair(pValue, rdo::runtime::RDOValue(pType, pValue));
}

std::string getString(const rdo::runtime::LPRDOMatrixValue& pMatrix, const rdo::runtime::LPRDOMatrixIterator& pIt)
{
    if (!pIt->equal(pMatrix->end()))
        return pIt->getValue().getAsString();

    return "";
}

std::string getString(const rdo::runtime::RDOValue& it, const rdo::runtime::RDOValue& end)
{
    if (it != end)
        return it.getAsString();

    return "";
}

BOOST_AUTO_TEST_CASE(MatrixTestCreate)
{
    BOOST_CHECK(createMatrix({1, 2, 3}).second.getAsString() == "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE(MatrixTestInsert)
{
    Matrix matrix1 = createMatrix({1, 2, 3});
    Matrix matrix2 = createMatrix({4, 5, 6});

    matrix1.first->insert(matrix1.first->begin()->next(), matrix2.first->begin(), matrix2.first->end());

    BOOST_CHECK(matrix1.second.getAsString() == "[1, 4, 5, 6, 2, 3]");
}

BOOST_AUTO_TEST_CASE(MatrixTestErase)
{
    Matrix matrix = createMatrix({1, 2, 3});

    matrix.first->erase(matrix.first->begin()->next(), matrix.first->begin()->preInc(3));

    BOOST_CHECK(matrix.second.getAsString() == "[1]");
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPrePlus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt = matrix.first->begin();
    while (!pIt->equal(matrix.first->end()))
        result += getString(matrix.first, pIt->preInc(1));

    BOOST_CHECK(result == "23");
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPostPlus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt = matrix.first->begin();
    while (!pIt->equal(matrix.first->end()))
        result += getString(matrix.first, pIt->postInc(1));

    BOOST_CHECK(result == "123");
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPreMinus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt = matrix.first->end();
    do
        result += getString(matrix.first, pIt->preInc(-1));
    while (!pIt->equal(matrix.first->begin()));

    BOOST_CHECK(result == "321");
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPostMinus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt = matrix.first->end();
    do
        result += getString(matrix.first, pIt->postInc(-1));
    while (!pIt->equal(matrix.first->begin()));

    BOOST_CHECK(result == "32");
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePrePlus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt  = matrix.first->begin();
    rdo::runtime::LPRDOMatrixIterator pEnd = matrix.first->end  ();
    rdo::runtime::RDOValue it (pIt,  pIt );
    rdo::runtime::RDOValue end(pEnd, pEnd);
    BOOST_CHECK(it != end);

    while (it != end)
        result += getString(++it, end);

    BOOST_CHECK(result == "23");
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePostPlus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt  = matrix.first->begin();
    rdo::runtime::LPRDOMatrixIterator pEnd = matrix.first->end  ();
    rdo::runtime::RDOValue it (pIt,  pIt );
    rdo::runtime::RDOValue end(pEnd, pEnd);
    BOOST_CHECK(it != end);

    while (it != end)
        result += getString(it++, end);

    BOOST_CHECK(result == "123");
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePreMinus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt    = matrix.first->end  ();
    rdo::runtime::LPRDOMatrixIterator pBegin = matrix.first->begin();
    rdo::runtime::LPRDOMatrixIterator pEnd   = matrix.first->end  ();
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

BOOST_AUTO_TEST_CASE(MatrixTestValuePostMinus)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::string result;
    rdo::runtime::LPRDOMatrixIterator pIt    = matrix.first->end  ();
    rdo::runtime::LPRDOMatrixIterator pBegin = matrix.first->begin();
    rdo::runtime::LPRDOMatrixIterator pEnd   = matrix.first->end  ();
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

BOOST_AUTO_TEST_CASE(MatrixTestSetItem)
{
    Matrix matrix = createMatrix({1, 2, 3});

    std::size_t ind = 1;
    std::size_t item = 48;
    rdo::runtime::RDOValue index(ind);
    rdo::runtime::RDOValue value(item);
    matrix.first->setItem(index, value);

    BOOST_CHECK(matrix.second.getAsString() == "[1, 48, 3]");

    ind = 3;
    index = ind;
    bool found = false;

    try
    {
        matrix.first->setItem(index, value);
    }
    catch (const rdo::runtime::RDORuntimeException& ex)
    {
        if (!ex.message().empty())
            found = ex.message() == "Выход за пределы матрицы";
    }

    if (!found)
        BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(MatrixTestGetItem)
{
    Matrix matrix = createMatrix({1, 48, 3});

    std::size_t ind = 1;
    rdo::runtime::RDOValue index(ind);
    rdo::runtime::RDOValue value(matrix.first->getItem(index));

    BOOST_CHECK(value.getAsString() == "48");

    ind = 3;
    index = ind;
    bool found = false;

    try
    {
        matrix.first->getItem(index);
    }
    catch (const rdo::runtime::RDORuntimeException& ex)
    {
        if (!ex.message().empty())
            found = ex.message() == "Выход за пределы матрицы";
    }

    if (!found)
        BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Matrix_Test
