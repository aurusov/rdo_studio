/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      2.10.2011
  \brief     Тест законов распределения
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOSequencesTest
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "utils/platform.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#define  __SCANF  sscanf_s
#else  // not COMPILER_VISUAL_STUDIO
	#define  __SCANF  sscanf
#endif // COMPILER_VISUAL_STUDIO

typedef std::vector<double> Container;
typedef std::vector<ruint>  ContainerInt;
typedef const tstring contstr;

const long int g_seed                 = 123456789;                  //!< база генератора
contstr        g_filePath             = _T("../../test/sequences/");//!< путь к файлам относительно проекта
contstr        g_fileNormalName       = _T("data_normal.txt");      //!< файл данных
contstr        g_fileUniformName      = _T("data_uniform.txt");     //!< файл данных
contstr        g_fileExponentialName  = _T("data_exponential.txt"); //!< файл данных
contstr        g_fileTriangularName   = _T("data_trinagular.txt");  //!< файл данных

const ruint    g_count                = 100000;                     //!< количество генерируемых данных
const double   g_main                 = 10.0;                       //!< параметр закона экспоненциального и нормального
const double   g_var                  = 1.0;                        //!< параметр закона нормального
const double   g_from                 = 1.0;                        //!< параметр закона равномерного и треугольного
const double   g_to                   = 7.0;                        //!< параметр закона равномерного и треугольного
const double   g_top                  = 5.0;                        //!< параметр закона треугольного
#if defined(ARCHITECTURE_AMD64) || defined(ARCHITECTURE_X86)
const ruint    g_precision            = 20;                         //!< точность вещественного числа при выводе в поток
#elif defined(ARCHITECTURE_ARM)
const ruint    g_precision            = 14;                         //!< точность вещественного числа при выводе в поток
#endif

const ruint    g_countOfExamples      = 2000;                       //!< количество чисел в выборке
const ruint    g_countOfR             = 39;                         //!< число разрядов
const double   pi                     = 3.141592653;                //!< фундаментальная константа
const double   g_ksiEtalon            = 50.9985;                    //!< табличное значение. 95% вероятность того, что это действительно тот самый закон распределения

// --------------------------------------------------------------------------------
// -------Templates
// --------------------------------------------------------------------------------
template <class T, class F, class contstr>
void onGenerateData(F binder, contstr g_fileName)
{
	if (rdo::File::exist(g_fileName.c_str()))
		return;

	T sequence(g_seed);
	Container test;
	test.reserve(g_count);

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
	Container orig;
	while (stream.good())
	{
		double value;
		stream >> value;
		std::stringstream s;
		s.precision(g_precision);
		s << value;
		s >> value;
		orig.push_back(value);
	}

	Container test;
	test.reserve(g_count);
	T sequence(g_seed);
	for (ruint i = 0; i < g_count; ++i)
	{
		std::stringstream s;
		s.precision(g_precision);
		s << binder.operator()(&sequence);
		double value;
		s >> value;
		test.push_back(value);
	}

	Container::const_iterator origIt = orig.begin();
	stream.precision(g_precision);
	STL_FOR_ALL(test, it)
	{
		BOOST_CHECK(origIt != orig.end());
		rbool check = *it == *origIt;
		BOOST_CHECK(check);
		if (!check)
		{
			std::cout.precision(g_precision);
			std::cout << *it << std::endl;
			std::cout << *origIt << std::endl;
		}
		++origIt;
	}
}

template <class T,class F>
double  area (F binder, double n, double m)
{
	double k = 1;
	double S1 = 1;
	double S2 = 0;
	ruint t = 10;
	while (fabs(S1-S2)/S1 > 0.01)
	{
		S2 = S1;
		S1 = 0;
		for (ruint g = 0; g < t + 1; ++g)
		{
			if ((g == 0) || (g == t - 1))
				k = 0.5;
			S1 += k*(binder.operator()(n + g*(m-n)/t));
			k = 1;
		}
		S1 *= (m-n)/t;
		t  *= 10;
	}
	return S1;
}

template <class T, class G, class F, class S>
void onCheckKsi(F binder, S binderSeq, double left, double right)
{
	Container x;
	x.reserve(g_countOfR + 1);
	double elem = (right-left)/(g_countOfR*1.0);	//расстояние между точками на прямой

	for (ruint i = 0; i < g_countOfR + 1; ++i)
	{
		x.push_back(left + elem*i);
	}

	Container vb;									//контейнер для хранения выборки
	vb.reserve(g_countOfExamples);

	G sequence(g_seed);								//выборка
	for (ruint i = 0; i < g_countOfExamples; ++i)
	{
		vb.push_back(binderSeq.operator()(&sequence));
	}

	Container f_vb;									//контейнер для храниения количества попаданий на интервал
	f_vb.reserve(g_countOfR);

	for(ruint i = 0; i < g_countOfR; ++i)			//нахождение количества попаданий на интервал
	{
		ruint freq = 0;
		for(ruint k = 0; k < g_countOfExamples; ++k)
		{
			if((vb[k] > x[i]) & (vb[k] <= x[i+1]))
			{
				++freq;
			}
		}
		f_vb.push_back(freq);
	}

	Container F_etalon;
	F_etalon.reserve(g_countOfR);

	for (ruint i = 0; i < g_countOfR; ++i)
	{
		F_etalon.push_back(area<T>(binder, x[i], x[i+1]));
	}

	double ksi = 0;
	for(ruint i = 0; i < g_countOfR; ++i)
	{
		double ksiTemp = F_etalon[i]*g_countOfExamples;
		ksi += (f_vb[i] - ksiTemp)*(f_vb[i] - ksiTemp)/ksiTemp;
	}
	BOOST_CHECK(ksi <= g_ksiEtalon);
			if (ksi >  g_ksiEtalon)
	{
		std::cout << ksi << std::endl;
	}
}
// --------------------------------------------------------------------------------

class SequenceNormal
{
public:
	SequenceNormal(double main, double var)
		: m_main(main)
		, m_var (var )
	{}

	double get(double x) const
	{
		return 1/(sqrt(2*pi)*m_var*exp((x - m_main)*(x - m_main)/(2*m_var*m_var)));
	}

private:
	double m_main;
	double m_var;
};

class SequenceExponential
{
public:
	SequenceExponential(double main)
		: m_main(main)
	{}

	double get(double x) const
	{
		return 1/(m_main*exp(x/m_main)); 
	}

private:
	double m_main;
};

class SequenceUniform
{
public:
	SequenceUniform(double min, double max)
		: m_min(min)
		, m_max(max)
	{}

	double get(double x) const
	{
		UNUSED(x);
		return 1/(m_max-m_min); 
	}

private:
	double m_min;
	double m_max;
};

class SequenceTriangular
{
public:
	SequenceTriangular(double min, double top, double max)
		: m_min(min-top)
		, m_top(top)
		, m_max(max-top)
	{}

	double get(double x) const
	{
		x -= m_top;
		double temp;
		if (x < 0)
		{
			temp = -2*x/((m_max - m_min)*m_min) + 2/(m_max - m_min);
		}
		else
		{
			temp = -2*x/((m_max - m_min)*m_max) + 2/(m_max - m_min);
		}
		return temp; 
	}

private:
	double m_min;
	double m_top;
	double m_max;
};

BOOST_AUTO_TEST_SUITE(RDOSequencesTest)

// --------------------------------------------------------------------------------
// -------Normal sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDONormalTestCreate)
{
	onGenerateData<rdo::runtime::RandGeneratorNormal>
		(boost::bind(&rdo::runtime::RandGeneratorNormal::next, _1, g_main, g_var), g_filePath + g_fileNormalName);
}

BOOST_AUTO_TEST_CASE(RDONormalTestCheck)
{
	onCheckData<rdo::runtime::RandGeneratorNormal>
		(boost::bind(&rdo::runtime::RandGeneratorNormal::next, _1, g_main, g_var), g_filePath + g_fileNormalName);

	SequenceNormal normal(g_main, g_var);
	onCheckKsi<SequenceNormal, rdo::runtime::RandGeneratorNormal>
		(boost::bind(&SequenceNormal::get, normal, _1),
		boost::bind(&rdo::runtime::RandGeneratorNormal::next, _1, g_main, g_var),
		g_main-4*g_var,
		g_main+4*g_var);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Uniform sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOUniformTestCreate)
{
	onGenerateData<rdo::runtime::RandGeneratorUniform>
		(boost::bind(&rdo::runtime::RandGeneratorUniform::next, _1, g_from, g_to), g_filePath + g_fileUniformName);
}

BOOST_AUTO_TEST_CASE(RDOUniformTestCheck)
{
	onCheckData<rdo::runtime::RandGeneratorUniform>
		(boost::bind(&rdo::runtime::RandGeneratorUniform::next, _1, g_from, g_to), g_filePath + g_fileUniformName);

	SequenceUniform uniform(g_from, g_to);
	onCheckKsi<SequenceUniform, rdo::runtime::RandGeneratorUniform>
		(boost::bind(&SequenceUniform::get, uniform, _1),
		boost::bind(&rdo::runtime::RandGeneratorUniform::next, _1, g_from, g_to),
		g_from,
		g_to);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Exponential sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOExponentialTestCreate)
{
	onGenerateData<rdo::runtime::RandGeneratorExponential>
		(boost::bind(&rdo::runtime::RandGeneratorExponential::next, _1, g_main), g_filePath + g_fileExponentialName);
}

BOOST_AUTO_TEST_CASE(RDOExponentialTestCheck)
{
	onCheckData<rdo::runtime::RandGeneratorExponential>
		(boost::bind(&rdo::runtime::RandGeneratorExponential::next, _1, g_main), g_filePath + g_fileExponentialName);

	SequenceExponential exponential(g_main);
	onCheckKsi<SequenceExponential, rdo::runtime::RandGeneratorExponential>
		(boost::bind(&SequenceExponential::get, exponential, _1),
		boost::bind(&rdo::runtime::RandGeneratorExponential::next, _1, g_main),
		0,
		7*g_main);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Triangular sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOTriangularTestCreate)
{
	onGenerateData<rdo::runtime::RandGeneratorTriangular>
		(boost::bind(&rdo::runtime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_filePath + g_fileTriangularName);
}

BOOST_AUTO_TEST_CASE(RDOTriangularTestCheck)
{
	onCheckData<rdo::runtime::RandGeneratorTriangular>
		(boost::bind(&rdo::runtime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_filePath + g_fileTriangularName);

	SequenceTriangular triangular(g_from, g_top, g_to);
	onCheckKsi<SequenceTriangular, rdo::runtime::RandGeneratorTriangular>
		(boost::bind(&SequenceTriangular::get, triangular, _1),
		boost::bind(&rdo::runtime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to),
		g_from,
		g_to);
}
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
