/*!
  \copyright (c) RDO-Team, 2012
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      13.05.2012
  \brief     Тест точности вывода дробных чисел
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#define BOOST_TEST_MODULE RDORealFormatTest
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORealFormatTest)

BOOST_AUTO_TEST_CASE(Case1)
{
    std::stringstream stream;
    stream << 0.00001;
    
    std::string data;
    stream >> data;

    int size = data.size();

    BOOST_CHECK(size == 6);
}

BOOST_AUTO_TEST_SUITE_END() // RDORealFormatTest
