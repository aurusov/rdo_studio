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
#include <map>
#include <cmath>
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------
using namespace rdoRuntime;

BOOST_AUTO_TEST_CASE(RDOTriangularTest)
{
	RandGeneratorTriangular triang(123456789);			//создаю объект, генерящий числа по треугольному закону

	typedef std::map<int, double> Container;			//даю более красивое название контейнеру map
	Container m_base, m_forTest;						//создаю два контейнера
	std::ifstream  testFile("Test.txt");				//открываю файл, котором будут лежать образцовые данные.

	for (int i = 0; i < 100000; i++)					//контейнер забивается числами, нагенеренными тестируемым объектом
	{
		m_forTest.insert(Container::value_type(i, 0.00001*floor(100000*triang.next(1,5,7)+0.5)));	//в контейнере значения следуют парами (порядковый номер и сгенеренное число)
	}
	
	//Этот блок используeтся для записи эталонного файла. + надо изменить std::ifstream на std::ofstream
	/*
	Container::const_iterator it = m_forTest.begin();
	while (it != m_forTest.end())
	{
		testFile << it->second << std::endl;
		++it;
	}
	*/
	int t=0;
	char a[8];
	char* b;
	Container::iterator iter = m_base.begin();
	while (t != 100000)
	{
		testFile >> a;
		m_base.insert(Container::value_type(t, strtod(a,&b)));
		++t;
	}
	BOOST_CHECK(m_base == m_forTest);
	//boost::equal_pointees(&m_base, &m_forTest);
	testFile.close();
	int k = 1;
}