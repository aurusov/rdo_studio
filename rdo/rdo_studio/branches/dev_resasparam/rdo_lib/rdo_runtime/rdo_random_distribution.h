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
#include "rdo_lib/rdo_runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \def       RDO_NATIVE_U01
  \brief     Законы распределения псевдослучайных чмсел в стиле РДО
*/
#define RDO_NATIVE_U01

/*!
  \class     RandGenerator
  \brief     Базовое равномерное распределение [0, 1)
*/
class RandGenerator
{
public:
	// Удалять нельзя, нужен именно виртуальный диструктор для вызова из RDOCalcSeqInit::~RDOCalcSeqInit()
	virtual ~RandGenerator();
	void setSeed(long int seed = 123456789);
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
  \class     RandGeneratorUniform
  \brief     Базовое равномерное распределение [0, 1)
*/
class RandGeneratorUniform: public RandGenerator
{
public:
	RandGeneratorUniform(long int seed = 123456789);
	double next(double from, double to);
};

/*!
  \class     RandGeneratorExponential
  \brief     Экспоненциальное распределение
*/
class RandGeneratorExponential: public RandGenerator
{
public:
	RandGeneratorExponential(long int seed = 123456789);
	double next(double math);
};

/*!
  \class     RandGeneratorNormal
  \brief     Нормальное распределение
*/
class RandGeneratorNormal: public RandGenerator
{
public:
	RandGeneratorNormal(long int seed = 123456789);
	double next(double av, double var);
};

/*!
  \class     RandGeneratorTriangular
  \brief     Треугольное распределение
*/
class RandGeneratorTriangular: public RandGenerator
{
public:
	RandGeneratorTriangular(long int seed = 123456789);
	double next(double from, double top, double to);
};

/*!
  \class     RandGeneratorCommonNext
  \brief     Общий класс для by_hist и enumerative
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
  \class     RandGeneratorByHist
  \brief     Базовый класс для гистограмм
*/
class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;
	RandGeneratorByHist(long int seed = 123456789);
};

/*!
  \class     RandGeneratorByHistReal
  \brief     Гистограмма вещественных чисел
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
  \class     RandGeneratorByHistEnum
  \brief     Гистограмма перечислимого типа
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
  \class     RandGeneratorEnumerative
  \brief     Перечень значений
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

#include "rdo_lib/rdo_runtime/rdo_random_distribution.inl"

#endif // _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
