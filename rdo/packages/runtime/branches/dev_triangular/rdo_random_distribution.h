/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_random_distribution.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.04.2008
  \brief     Датчики псевдослучайных чисел
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
#define _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Законы распределения псевдослучайных чисел в стиле РДО
#define RDO_NATIVE_U01

//! @brief Базовое равномерное распределение [0, 1)
class RandGenerator
{
public:
	//! Удалять нельзя, нужен именно виртуальный деструктор для вызова из \ref RDOCalcSeqInit::~RDOCalcSeqInit()
	virtual ~RandGenerator();

	//! Установка базы генератора
	//! @param seed - База генератора
	void setSeed(long int seed = 123456789);

	//! @brief  Генерация псевдо-случайного числа на интевале [0, 1) по равномерному закону распределения
	//! @result Псевдо-случайное число, значение в дипазоне [0, 1)
	double u01();

protected:

#ifdef RDO_NATIVE_U01
	unsigned int m_seed;
#else
	long int m_seed;
#endif
	RandGenerator(long int seed = 123456789);
};

/*!
  \class          RandGeneratorUniform
  \brief          Равномерное распределение [0, 1)
  \param   seed   База генератора псевдослучайных чисел (long int = 123456789)
  \param   from   Нижняя граница диапазона распределения (double)
  \param   to     Верхняя граница диапазона распределения (double)
*/
class RandGeneratorUniform: public RandGenerator
{
public:
	RandGeneratorUniform(long int seed = 123456789);
	double next(double from, double to);
};

/*!
  \class          RandGeneratorUniform
  \brief          Экспоненциальное распределение
  \param   seed   База генератора псевдослучайных чисел (long int = 123456789)
  \param   math   Математическое ожидание закона распределения (double)
  */
class RandGeneratorExponential: public RandGenerator
{
public:
	RandGeneratorExponential(long int seed = 123456789);
	double next(double math);
};

/*!
  \class          RandGeneratorNormal
  \brief          Нормальное распределение
  \param   seed   База генератора псевдослучайных чисел (long int = 123456789)
  \param   av     Среднее значение последовательности (double)
  \param   var    Среднеквадратическое отклонение (double)
  */
class RandGeneratorNormal: public RandGenerator
{
public:
	RandGeneratorNormal(long int seed = 123456789);
	double next(double av, double var);
};

/*!
  \class          RandGeneratorTriangular
  \brief          Треугольное распределение
  \param   seed   База генератора псевдослучайных чисел (long int = 123456789)
  \param   from   Левая граница интервала распределения (double)
  \param   top    Точка под вершиной треугольника (double)
  \param   to     Правая граница интервала распределения (double)
*/
class RandGeneratorTriangular: public RandGenerator
{
public:
	RandGeneratorTriangular(long int seed = 123456789);
	double next(double from, double top, double to);
};

/*!
  \class          RandGeneratorCommonNext
  \brief          Общий класс для by_hist и enumerative
*/
class RandGeneratorCommonNext
{
public:
	virtual ~RandGeneratorCommonNext();
	virtual RDOValue next() = 0;

protected:
	RandGeneratorCommonNext();
};

/*!
  \class          RandGeneratorByHist
  \brief          Базовый класс для гистограмм
  \param   seed   База генератора чисел (long int = 123456789)
*/
class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;
	RandGeneratorByHist(long int seed = 123456789);
};

/*!
  \class          RandGeneratorByHistReal
  \brief          Гистограмма вещественных чисел
  \param   seed   База генератора чисел (long int = 123456789)
  \param   from   Нижняя граница диапазона (double)
  \param   to     Верхняя граница диапазона (double)
  \param   freq   Относительная частота попадания последовательности в заданный интервал (double)
*/
class RandGeneratorByHistReal: public RandGeneratorByHist
{
public:
	RandGeneratorByHistReal(long int seed = 123456789);
	void addValues(double from, double to, double freq);

private:
	std::vector< double > m_from;
	std::vector< double > m_to;
	std::vector< double > m_freq;

	virtual RDOValue next();
};

/*!
  \class          RandGeneratorByHistEnum
  \brief          Гистограмма перечислимого типа
  \param   seed   База генератора чисел (long int = 123456789)
  \param   val    Имя перечислимого типа (RDOValue)
  \param   freq   Относительная частота попадания последовательности в заданный интервал (double)
*/
class RandGeneratorByHistEnum: public RandGeneratorByHist
{
public:
	RandGeneratorByHistEnum(long int seed = 123456789);
	void addValues(RDOValue val, double freq);

private:
	std::vector< RDOValue > m_vals;
	std::vector< double >   m_freq;

	virtual RDOValue next();
};

/*!
  \class         RandGeneratorEnumerative
  \brief         Перечень значений
  \param   val   Имя вещественной константы или Имя значения в соответсвии с типом значений последовательности (RDOValue)
*/
class RandGeneratorEnumerative: public RandGeneratorCommonNext
{
public:
	RandGeneratorEnumerative();
	virtual ~RandGeneratorEnumerative();

	void addValue(RDOValue val);

private:
	std::vector<RDOValue>            m_vals;
	std::vector<RDOValue>::size_type m_curr;

	virtual RDOValue next();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_random_distribution.inl"

#endif // _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
