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
#define BOOST_TEST_MODULE RDONormalTest
#include <iostream>
#include <fstream>
#include <list>
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

const long int g_seed      = 123456789;                  //!< база генератора
const tstring  g_fileName  = _T("data_normal.txt");      //!< файл данных
const ruint    g_count     = 100000;                     //!< количество генерируемых данных
const double   g_main      = 10.0;                       //!< параметр закона
const double   g_var       = 1.0;                        //!< параметр закона
const ruint    g_precision = 20;                         //!< точность вещественного числа при выводе в поток

typedef std::list<double> Container;

BOOST_AUTO_TEST_SUITE(RDONormalTest)

template <class T, class F>
void onGenerateData(F binder)
{
	if (rdo::File::exist(g_fileName.c_str()))
		return;

	T sequence(g_seed);
	Container test;

	for (ruint i = 0; i < g_count; ++i)
	{
		test.push_back(binder.operator()(&sequence));
	}

	std::ofstream stream(g_fileName.c_str());
	stream.precision(g_precision);
	STL_FOR_ALL(test, it)
	{
		stream << *it << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(RDONormalTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorNormal>(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var));
}

BOOST_AUTO_TEST_CASE(RDONormalTestCheck)
{
	std::ifstream stream(g_fileName.c_str());
	BOOST_CHECK(stream.good());

	Container test;
	rdoRuntime::RandGeneratorNormal normal(g_seed);
	for (ruint i = 0; i < g_count; ++i)
	{
		test.push_back(normal.next(g_main, g_var));
	}

	stream.precision(g_precision);
	STL_FOR_ALL(test, it)
	{
		BOOST_CHECK(stream.good());
		double val;
		stream >> val;
		BOOST_CHECK(val == *it);
	}
}

BOOST_AUTO_TEST_SUITE_END()
