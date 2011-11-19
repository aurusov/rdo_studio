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
#include <utils/platform.h>
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

#ifndef OST_WINDOWS

#define sscanf_s( fmt, ... ) scanf( scanf_validate( fmt, __FILE__, __LINE__ ), __VA_ARGS__ )

const char* scanf_validate( const char* fmt, const char* file, long line )
{
    const char* p = fmt;
    while (1)
    {
        p = strstr( p, "%s" );
        if (p == NULL) break;
        if ((p == fmt) || (*(p-1) != '%'))
        {
            fprintf( stderr, "Hey, you used \"%%s\" in %s: line %d!\n", file, line );
            abort();
        }
    }
    return fmt;
}
#endif 

typedef std::vector<double> Container;
typedef const tstring contstr;

const long int g_seed                 = 123456789;                  //!< база генератора
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
const ruint    g_precision            = 20;                         //!< точность вещественного числа при выводе в поток

const ruint    g_countOfExamples      = 2000;                       //!< количество чисел в выборке
const ruint    g_countOfFree          = 39;                         //!< число разрядов
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
	Container test(g_count);

	for (ruint i = 0; i < g_count; ++i)
	{
		test[i] = binder.operator()(&sequence);
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

	Container test(g_count);
	T sequence(g_seed);
	for (ruint i = 0; i < g_count; ++i)
	{
		test[i] = binder.operator()(&sequence);
	}

	stream.precision(g_precision);
	STL_FOR_ALL(test, it)
	{
		BOOST_CHECK(stream.good());
		tstring str;
		stream >> str;

		double val;
		BOOST_CHECK(sscanf_s(str.c_str(), _T("%lf"), &val) == 1);
		BOOST_CHECK(val == *it);
				if (val != *it)
		{
			std::cout.precision(g_precision);
			std::cout << *it << std::endl;
			std::cout << val << std::endl;
		}
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
	const ruint  countOfxI  = g_countOfFree*1000;	//количество отрезков, на 1 меньше числа точек
	const double areaOfI    = 1/(g_countOfFree*1.0);			//площадь на отрезке

	Container xTemp(countOfxI + 1);			//создаю временный контейнер значений, на основе которых потом буду считать границы участка
	double elem = (right-left)/(countOfxI);	//расстояние между точками на прямой, между временными значениями

	for (ruint i = 0; i < countOfxI + 1; ++i)//загоняю элементарные значения в контейнер. решил, что на три порядка больше точек (чем колчество участков) даст достаточную точность при вычислениях
	{
		xTemp[i] = left + elem*i;
	}

	Container x;							//контейнер для границ интервалов, которые будут участововать в вычислениях
	x.push_back(left);						//левая граница будет в начале списка, правая граница или близкая к ней точка - right

	for (ruint i = 1; i < countOfxI; ++i)
	{
		if (fabs(area<T>(binder, x.back(), xTemp[i]) - areaOfI) < fabs(area<T>(binder, x.back(), xTemp[i+1]) - areaOfI))
		{
			x.push_back(xTemp[i]);
		}
	}
	if(x.size() == g_countOfFree)			//обрабатывается случай, когда площадь на последнем интервале окажется чуть меньше чем отведенная ему доля площади. погрешность ожидаестся очень малая из-за этого. проблема могла возникнуть при посчете количества попаданий в последний интервал. а его бы не было
	{
		x.push_back(right);
	}
	else if(x.back() < right)				//случай, когда последний интервал заканчивается раньше конца заданного интервала
	{
		x.pop_back();
		x.push_back(right);
	}

	//итак, теперь у нас есть границы интервалов

	Container vb(g_countOfExamples);		//контейнер для хранения выборки

	G sequence(g_seed);						//выборка
	for (ruint i = 0; i < g_countOfExamples; ++i)
	{
		vb[i] = binderSeq.operator()(&sequence);
	}

	Container fI(g_countOfFree);			//контейнер для храниения количества попаданий на интервал
	
	for(ruint i = 0; i < g_countOfFree; ++i)	//нахождение количества попаданий на интервал
	{
		ruint freq = 0;
		for(ruint k = 0; k < g_countOfExamples; ++k)
		{
			if((vb[k] > x[i]) & (vb[k] <= x[i+1]))
			{
				++freq;
			}
		}
		fI[i] = freq;
	}

	double ksi = 0;
	double Fi = g_countOfExamples/g_countOfFree;		//вычисление самой формулы
	for(ruint i = 0; i < g_countOfFree; ++i)
	{
		ksi += ((fI[i] - Fi)*(fI[i] - Fi))/Fi;
	}

	BOOST_CHECK(ksi < g_ksiEtalon);
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
		return 1/(sqrt(2*pi)*m_var*exp((x - m_main)*(x - m_main)/(2*m_var*m_var))); //функция плотности распределения вероятности. по определению для закона. таких будет еще 3 шутки.
	}

private:
	double m_main;
	double m_var;
};

BOOST_AUTO_TEST_SUITE(RDOSequencesTest)

// --------------------------------------------------------------------------------
// -------Normal sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDONormalTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorNormal>
		(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);
}

BOOST_AUTO_TEST_CASE(RDONormalTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorNormal>
		(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);

	SequenceNormal normal(g_main, g_var);
	onCheckKsi<SequenceNormal, rdoRuntime::RandGeneratorNormal>
		(boost::bind(&SequenceNormal::get, normal, _1),
		boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var),
		g_main-4*sqrt(g_var),
		g_main+4*sqrt(g_var));
}

// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Uniform sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOUniformTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorUniform>
		(boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to), g_fileUniformName);
}

BOOST_AUTO_TEST_CASE(RDOUniformTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorUniform>
		(boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to), g_fileUniformName);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Exponential sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOExponentialTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorExponential>
		(boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main), g_fileExponentialName);
}

BOOST_AUTO_TEST_CASE(RDOExponentialTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorExponential>
		(boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main), g_fileExponentialName);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Triangular sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOTriangularTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorTriangular>
		(boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_fileTriangularName);
}

BOOST_AUTO_TEST_CASE(RDOTriangularTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorTriangular>
		(boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_fileTriangularName);
}
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
