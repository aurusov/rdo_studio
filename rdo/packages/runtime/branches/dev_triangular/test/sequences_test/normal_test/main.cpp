/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      2.10.2011
  \brief     Тест нормального закона
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
const tstring  g_fileName = _T("data_normal.txt");      //!< файл данных
const ruint    g_count    = 100000;                     //!< количество генерируемых данных
const double   g_main     = 10.0;                       //!< параметр закона
const double   g_var      = 1.0;                        //!< параметр закона

typedef std::list <double> Container;

BOOST_AUTO_TEST_SUITE(RDOTriangularTest)

BOOST_AUTO_TEST_CASE(RDOTriangularTestCreate)
{
	if (rdo::File::exist(g_fileName.c_str()))
		return;

	rdoRuntime::RandGeneratorNormal normal(g_seed);
	Container base;

	for (int i = 0; i < g_count; ++i)
	{
		base.push_back(normal.next(g_main,g_var));
	}

	std::ofstream strm(g_fileName.c_str());
	
	strm.precision(20);

	STL_FOR_ALL(base,it)
	{
		strm << *it << std::endl;
	}
	strm.close();
}

BOOST_AUTO_TEST_CASE(RDOTriangularTestCheck)
{
	std::ifstream strm(g_fileName.c_str());
	BOOST_CHECK(strm.good());

	Container test,base;
	rdoRuntime::RandGeneratorNormal normal(g_seed);
	for (int i = 0; i < g_count; ++i)
	{
		test.push_back(normal.next(g_main,g_var));
	}

	strm.precision(20);

	STL_FOR_ALL(test,it)
	{
		BOOST_CHECK(strm.good());
		double val;
		strm >> val;
		BOOST_CHECK(val == *it);
	}

	strm.close();
}

BOOST_AUTO_TEST_SUITE_END()