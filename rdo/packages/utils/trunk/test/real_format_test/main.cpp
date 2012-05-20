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

BOOST_AUTO_TEST_CASE(MantissaPrecision)
{
	double value = 10e+007;
	std::stringstream stream;
	stream << value;

	std::string str = stream.str();
	std::cout << str;

	BOOST_CHECK(stream.str() == "1e+008");
}

BOOST_AUTO_TEST_SUITE_END() // RDORealFormatTest
