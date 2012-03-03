/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_sequence.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     Последовательности
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_SEQUENCE_H_
#define _LIB_RUNTIME_CALC_SEQUENCE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Инициализация последовательности псевдослучайных чисел
CALC(RDOCalcSeqInit)
{
DECLARE_FACTORY(RDOCalcSeqInit)
public:
	void setBase(int base);

private:
	RDOCalcSeqInit(int base, PTR(RandGenerator) gen);
	virtual ~RDOCalcSeqInit();

	int                m_base;
	PTR(RandGenerator) m_gen;

	DECLARE_ICalc;
};

//! Вычисляет следующее значение последовательности псевдослучайных чисел
CALC_SUB(RDOCalcSeqNext, RDOFunCalc)
{
public:
	struct Range
	{
		double  m_min;
		double  m_max;

		Range();
		Range(CREF(double) min, CREF(double) max);
	};

	rbool                   m_res_real;
	boost::optional<Range>  m_range;

protected:
	RDOCalcSeqNext();

	virtual RDOValue getNextValue(CREF(LPRDORuntime) pRuntime) = 0;
};

//! Псевдослучайное распределение
template<class T>
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
protected:
	RDOCalcRandomDistribution(PTR(T) gen);
	virtual ~RDOCalcRandomDistribution();
	PTR(T) m_gen;

private:
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);
};

/*!
  \def     DEFINE_RANDON_DISTRIBUTION
  \brief   Макрос для разных законов распределения
*/
#define DEFINE_RANDON_DISTRIBUTION(CalcName, Distribution)                  \
CALC_SUB(RDOCalcSeqNext##CalcName, RDOCalcRandomDistribution<Distribution>) \
{                                                                           \
DECLARE_FACTORY(RDOCalcSeqNext##CalcName)                                   \
private:                                                                    \
	RDOCalcSeqNext##CalcName(PTR(Distribution) gen)                         \
		: RDOCalcRandomDistribution<Distribution>(gen)                      \
	{}                                                                      \
                                                                            \
	virtual RDOValue getNextValue(CREF(LPRDORuntime) pRuntime);             \
};

/*!
  \class   RDOCalcSeqNextUniform
  \brief   Следующее значение псевдослучайной величины, распределенной равномерно
*/
DEFINE_RANDON_DISTRIBUTION(Uniform, RandGeneratorUniform);

/*!
  \class   RDOCalcSeqNextNormal
  \brief   Следующее значение псевдослучайной величины, распределенной по нормальному закону
*/
DEFINE_RANDON_DISTRIBUTION(Normal, RandGeneratorNormal);

/*!
  \class   RDOCalcSeqNextExponential
  \brief   Следующее значение псевдослучайной величины, распределенной по экспоненциальному закону
*/
DEFINE_RANDON_DISTRIBUTION(Exponential, RandGeneratorExponential);

/*!
  \class   RDOCalcSeqNextTriangular
  \brief   Следующее значение псевдослучайной величины, распределенной по треугольному закону
*/
DEFINE_RANDON_DISTRIBUTION(Triangular, RandGeneratorTriangular);

/*!
  \class   RDOCalcSeqNextByHist
  \brief   Следующее значение псевдослучайной величины, распределенной по заданной гистограмме
*/
DEFINE_RANDON_DISTRIBUTION(ByHist, RandGeneratorCommonNext);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_sequence.inl"

#endif // _LIB_RUNTIME_CALC_SEQUENCE_H_
