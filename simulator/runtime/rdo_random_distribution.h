#ifndef _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
#define _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

#define RDO_NATIVE_U01

class RandGenerator
{
public:
	virtual ~RandGenerator();

	void setSeed(long int seed = 123456789);

	double u01();

protected:

#ifdef RDO_NATIVE_U01
	std::uint32_t m_seed;
#else
	long int m_seed;
#endif
	RandGenerator(long int seed = 123456789);
};

class RandGeneratorUniform: public RandGenerator
{
public:
	RandGeneratorUniform(long int seed = 123456789);

	double next(double from, double to);
};

class RandGeneratorExponential: public RandGenerator
{
public:
	RandGeneratorExponential(long int seed = 123456789);

	double next(double math);
};

class RandGeneratorNormal: public RandGenerator
{
public:
	RandGeneratorNormal(long int seed = 123456789);

	double next(double av, double var);
};

class RandGeneratorTriangular: public RandGenerator
{
public:
	RandGeneratorTriangular(long int seed = 123456789);

	double next(double from, double top, double to);
};

class RandGeneratorCommonNext
{
public:
	virtual ~RandGeneratorCommonNext();
	virtual RDOValue next() = 0;

protected:
	RandGeneratorCommonNext();
};

class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;

	RandGeneratorByHist(long int seed = 123456789);
};

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

#endif // _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
