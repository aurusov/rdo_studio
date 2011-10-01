/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      12.09.2011
  \brief     Тест треугольного закона
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOTriangularTest
#include <iostream>
#include <fstream>
#include <list>
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

const long int g_seed     = 123456789;                  //!< база генератора
const tstring  g_fileName = _T("data_triangular.txt");  //!< файл данных
const ruint    g_count    = 100000;                     //!< количество генерируемых данных
const double   g_from     = 1.0;                        //!< параметр закона
const double   g_top      = 5.0;                        //!< параметр закона
const double   g_to       = 7.0;                        //!< параметр закона

typedef std::list<double> Container;

BOOST_AUTO_TEST_SUITE(RDOTriangularTest)

BOOST_AUTO_TEST_CASE(RDOTriangularTest_GenerateData)
{
	if (rdo::File::exist(g_fileName))
	{
		return;
	}

	rdoRuntime::RandGeneratorTriangular triang(g_seed);
	Container                           container;

	for (ruint i = 0; i < g_count; i++)
	{
		container.push_back(triang.next(g_from, g_top, g_to));
	}

	std::ofstream stream(g_fileName.c_str());
	BOOST_CHECK(stream.good());

	stream.precision(20);
	STL_FOR_ALL_CONST(container, it)
	{
		stream << *it << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(RDOTriangularTest_CheckData)
{
	BOOST_CHECK(rdo::File::exist(g_fileName));

	rdoRuntime::RandGeneratorTriangular triang(g_seed);
	Container                           container;

	for (ruint i = 0; i < g_count; i++)
	{
		container.push_back(triang.next(g_from, g_top, g_to));
	}

	std::ifstream stream(g_fileName.c_str());
	BOOST_CHECK(stream.good());

	STL_FOR_ALL_CONST(container, it)
	{
		BOOST_CHECK(stream.good());
		double value;
		stream >> value;
		BOOST_CHECK(value == *it);
	}
}

BOOST_AUTO_TEST_SUITE_END()
