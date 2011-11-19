/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_random_distribution.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      02.08.2011
  \brief     Датчики псевдослучайных чисел
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RandGenerator
// --------------------------------------------------------------------------------
inline RandGenerator::RandGenerator(long int seed)
	: m_seed(seed)
{}

inline RandGenerator::~RandGenerator()
{}

inline void RandGenerator::setSeed(long int seed)
{
	m_seed = seed;
}

inline double RandGenerator::u01()
{
#ifdef RDO_NATIVE_U01
	m_seed = m_seed * 69069 + 1;
	return m_seed / 4294967296.0;
#else
	const long int MODLUS = 2147483647;
	const long int MULT1  = 24112;
	const long int MULT2  = 26143;
	long int zi = m_seed;
	long int lowprd = (zi & 65535) * MULT1;
	long int hi31   = (zi >> 16) * MULT1 + (lowprd >> 16);
	zi = ((lowprd & 65535) - MODLUS) + ((hi31 & 32767) << 16) + (hi31 >> 15);
	if (zi < 0) zi += MODLUS;
	lowprd = (zi & 65535) * MULT2;
	hi31 = (zi >> 16) * MULT2 + (lowprd >> 16);
	zi = ((lowprd & 65535) - MODLUS) + ((hi31 & 32767) << 16) + (hi31 >> 15);
	if (zi < 0) zi += MODLUS;
	m_seed = zi;
	//return ((zi >> 7) + 1) / 16777216.0;     // Так в примере на FORTRAN imho
	return (((zi >> 7) | 1) + 1) / 16777216.0; // Так в примере на C
#endif
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorUniform
// --------------------------------------------------------------------------------
inline RandGeneratorUniform::RandGeneratorUniform(long int seed)
	: RandGenerator(seed)
{}

inline double RandGeneratorUniform::next(double from, double to)
{
	return u01() * (to - from) + from;
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorExponential
// --------------------------------------------------------------------------------
inline RandGeneratorExponential::RandGeneratorExponential(long int seed)
	: RandGenerator(seed)
{}

inline double RandGeneratorExponential::next(double math)
{
	return -log(u01()) * math;
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorNormal
// --------------------------------------------------------------------------------
inline RandGeneratorNormal::RandGeneratorNormal(long int seed)
	: RandGenerator(seed)
{}

inline double RandGeneratorNormal::next(double av, double var)
{
	double ran = 0;
	for (int i = 0; i < 12; ++i)
	{
		ran += u01();
	}
	return var * (ran - 6) + av;
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorTriangular
// --------------------------------------------------------------------------------
inline RandGeneratorTriangular::RandGeneratorTriangular(long int seed)
	: RandGenerator(seed)
{}

inline double RandGeneratorTriangular::next(double from, double top, double to)
{
	double result;
	if (u01() > (top-from)/(to-from))
	{
		result = -(to-top)*(sqrt(u01()) - 1);
	}
	else
	{
		result = (top-from)*(sqrt(u01()) - 1);
	}
	return result + top;
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorCommonNext
// --------------------------------------------------------------------------------
inline RandGeneratorCommonNext::RandGeneratorCommonNext()
{}

inline RandGeneratorCommonNext::~RandGeneratorCommonNext()
{}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorByHist
// --------------------------------------------------------------------------------
inline RandGeneratorByHist::RandGeneratorByHist(long int seed)
	: RandGeneratorUniform   (seed)
	, RandGeneratorCommonNext(    )
	, summ                   (0   )
{}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorByHistReal
// --------------------------------------------------------------------------------
inline RandGeneratorByHistReal::RandGeneratorByHistReal(long int seed)
	: RandGeneratorByHist(seed)
{}

inline void RandGeneratorByHistReal::addValues(double from, double to, double freq)
{
	m_from.push_back(from);
	m_to.push_back  (to  );
	m_freq.push_back(freq);
	summ += freq;
}

inline RDOValue RandGeneratorByHistReal::next()
{
	double ran1 = RandGeneratorUniform::next(0, summ);
	double add = 0;
	std::vector<double>::size_type i;
	for (i = 0; i < m_freq.size() - 1; ++i)
	{
		if (ran1 < m_freq[i] + add) break;
		add += m_freq[i];
	}
	return RandGeneratorUniform::next(m_from[i], m_to[i]);
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorByHistEnum
// --------------------------------------------------------------------------------
inline RandGeneratorByHistEnum::RandGeneratorByHistEnum(long int seed)
	: RandGeneratorByHist(seed)
{}

inline void RandGeneratorByHistEnum::addValues(RDOValue val, double freq)
{
	m_vals.push_back(val);
	m_freq.push_back(freq);
	summ += freq;
}

inline RDOValue RandGeneratorByHistEnum::next()
{
	double ran1 = RandGeneratorUniform::next(0, summ);
	double add = 0;
	std::vector<double>::size_type i;
	for (i = 0; i < m_freq.size() - 1; ++i)
	{
		if (ran1 < m_freq[i] + add) break;
		add += m_freq[i];
	}
	return m_vals[i];
}

// --------------------------------------------------------------------------------
// -------------------- RandGeneratorEnumerative
// --------------------------------------------------------------------------------
inline RandGeneratorEnumerative::RandGeneratorEnumerative()
	: RandGeneratorCommonNext( )
	, m_curr                 (0)
{}

inline RandGeneratorEnumerative::~RandGeneratorEnumerative()
{}

inline void RandGeneratorEnumerative::addValue(RDOValue val)
{
	m_vals.push_back(val);
}

inline RDOValue RandGeneratorEnumerative::next()
{
	RDOValue res = m_vals[m_curr++];
	if (m_curr >= m_vals.size()) m_curr = 0;
	return res;
}

CLOSE_RDO_RUNTIME_NAMESPACE
