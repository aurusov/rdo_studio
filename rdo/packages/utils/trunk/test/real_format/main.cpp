/*!
  \copyright (c) RDO-Team, 2012
  \file      main.cpp
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      13.05.2012
  \brief     Тест точности вывода дробных чисел
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#define BOOST_TEST_MODULE RDORealFormatTest
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORealFormatTest)

BOOST_AUTO_TEST_CASE(MantissaPrecisionSet)
{
#ifdef COMPILER_VISUAL_STUDIO
	_set_output_format(_TWO_DIGIT_EXPONENT);
#endif
}

BOOST_AUTO_TEST_CASE(MantissaPrecision2)
{
	double value = 10e+37;
	std::stringstream stream;

	stream << value;

	BOOST_CHECK(stream.str() == "1e+38");
}

BOOST_AUTO_TEST_CASE(MantissaPrecision3)
{
	double value = 10e+237;
	std::stringstream stream;

	stream << value;

	BOOST_CHECK(stream.str() == "1e+238");
}

BOOST_AUTO_TEST_SUITE_END() // RDORealFormatTest
