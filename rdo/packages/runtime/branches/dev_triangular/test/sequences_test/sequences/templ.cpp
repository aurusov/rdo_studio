/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      2.10.2011
  \brief     Шаблоны для тестов
  \indent    4T
*/


template <class T, class F, class contstr>
void onGenerateData(F binder, contstr g_fileName)
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

template <class T, class F, class contstr>
void onCheckData(F binder, contstr g_fileName)
{
	std::ifstream stream(g_fileName.c_str());
	BOOST_CHECK(stream.good());

	Container test;
	T sequence(g_seed);
	for (ruint i = 0; i < g_count; ++i)
	{
		test.push_back(binder.operator()(&sequence));
	}

	STL_FOR_ALL(test, it)
	{
		BOOST_CHECK(stream.good());
		double val;
		stream >> val;
		BOOST_CHECK(val == *it);
	}
}