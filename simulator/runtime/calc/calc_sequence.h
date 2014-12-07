#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalcSeqInit);
class RDOCalcSeqInit: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSeqInit)
public:
	void setBase(int base);

private:
	RDOCalcSeqInit(int base, RandGenerator* gen);
	virtual ~RDOCalcSeqInit();

	int m_base;
	RandGenerator* m_gen;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcSeqNext);
class RDOCalcSeqNext: public RDOFunCalc
{
public:
	struct Range
	{
		double  m_min;
		double  m_max;

		Range();
		Range(const double& min, const double& max);
	};

	bool m_res_real;
	boost::optional<Range> m_range;

protected:
	RDOCalcSeqNext();

	virtual RDOValue getNextValue(const LPRDORuntime& pRuntime) = 0;
};

template<class T>
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
protected:
	RDOCalcRandomDistribution(T* gen);
	virtual ~RDOCalcRandomDistribution();
	T* m_gen;

private:
	DECLARE_ICalc;
};

#define DEFINE_RANDON_DISTRIBUTION(CalcName, Distribution)                     \
PREDECLARE_POINTER(RDOCalcSeqNext##CalcName);                                  \
class RDOCalcSeqNext##CalcName: public RDOCalcRandomDistribution<Distribution> \
{                                                                              \
DECLARE_FACTORY(RDOCalcSeqNext##CalcName)                                      \
private:                                                                       \
	RDOCalcSeqNext##CalcName(Distribution* gen)                                \
		: RDOCalcRandomDistribution<Distribution>(gen)                         \
	{}                                                                         \
	                                                                           \
	virtual RDOValue getNextValue(const LPRDORuntime& pRuntime);               \
};

DEFINE_RANDON_DISTRIBUTION(Uniform, RandGeneratorUniform);

DEFINE_RANDON_DISTRIBUTION(Normal, RandGeneratorNormal);

DEFINE_RANDON_DISTRIBUTION(Exponential, RandGeneratorExponential);

DEFINE_RANDON_DISTRIBUTION(Triangular, RandGeneratorTriangular);

DEFINE_RANDON_DISTRIBUTION(ByHist, RandGeneratorCommonNext);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_sequence-inl.h"
