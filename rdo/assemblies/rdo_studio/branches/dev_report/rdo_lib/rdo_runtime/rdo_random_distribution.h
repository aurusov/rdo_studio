#ifndef RDO_RANDOM_DISTRIBUTION_H
#define RDO_RANDOM_DISTRIBUTION_H

#include "rdo_value.h"

namespace rdoRuntime
{

#define RDO_NATIVE_U01

class RandGenerator
{
public:
	virtual ~RandGenerator() {} // Удалять нельзя, нужен именно виртуальный диструктор для вызова из RDOCalcSeqInit::~RDOCalcSeqInit()
	void setSeed( long int seed = 123456789 )
	{
		m_seed = seed;
	}

	double u01() {
#ifdef RDO_NATIVE_U01
		m_seed = m_seed * 69069 + 1;
		return m_seed / 4294967296.0; //(long double)UINT_MAX + 1
#else
		const long int MODLUS = 2147483647;
		const long int MULT1  = 24112;
		const long int MULT2  = 26143;
		long int zi = m_seed;
		long int lowprd = (zi & 65535) * MULT1;
		long int hi31   = (zi >> 16) * MULT1 + (lowprd >> 16);
		zi = ((lowprd & 65535) - MODLUS) + ((hi31 & 32767) << 16) + (hi31 >> 15);
		if ( zi < 0 ) zi += MODLUS;
		lowprd = (zi & 65535) * MULT2;
		hi31 = (zi >> 16) * MULT2 + (lowprd >> 16);
		zi = ((lowprd & 65535) - MODLUS) + ((hi31 & 32767) << 16) + (hi31 >> 15);
		if ( zi < 0 ) zi += MODLUS;
		m_seed = zi;
//		return ((zi >> 7) + 1) / 16777216.0;       // Так в примере на FORTRAN imho
		return (((zi >> 7) | 1) + 1) / 16777216.0; // Так в примере на C
#endif
	}

protected:

#ifdef RDO_NATIVE_U01
	unsigned int m_seed;
#else
	long int m_seed;
#endif

	RandGenerator( long int seed = 123456789 ): m_seed( seed ) {}
};

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
class RandGeneratorUniform: public RandGenerator
{
public:
	RandGeneratorUniform( long int seed = 123456789 ):
		RandGenerator( seed )
	{
	}
	double next( double from, double to )
	{
		return u01() * ( to - from ) + from;
	}
};

class RandGeneratorExponential: public RandGenerator
{
public:
	RandGeneratorExponential( long int seed = 123456789 ):
		RandGenerator( seed )
	{
	}
	double next( double math )
	{
		return -log( u01() ) * math;
	}
};

class RandGeneratorNormal: public RandGenerator
{
public:
	RandGeneratorNormal( long int seed = 123456789 ):
		RandGenerator( seed )
	{
	}
	double next( double av, double var )
	{
		double ran = 0;
		for ( int i = 0; i < 12; i++ ) {
			ran += u01();
		}
		return var * (ran - 6) + av;
	}
};

// ----------------------------------------------------------------------------
// ---------- Общий класс для by_hist и enumerative
// ----------------------------------------------------------------------------
class RandGeneratorCommonNext
{
public:
	virtual ~RandGeneratorCommonNext()
	{
	}
	virtual RDOValue next() = 0;

protected:
	RandGeneratorCommonNext() {}
};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;
	RandGeneratorByHist( long int seed = 123456789 ):
		RandGeneratorUniform( seed ),
		RandGeneratorCommonNext(),
		summ( 0 )
	{
	}
};

class RandGeneratorByHistReal: public RandGeneratorByHist
{
public:
	RandGeneratorByHistReal( long int seed = 123456789 ):
		RandGeneratorByHist( seed )
	{
	}

	void addValues( double from, double to, double freq )
	{
		m_from.push_back( from );
		m_to.push_back  ( to   );
		m_freq.push_back( freq );
		summ += freq;
	}

private:
	std::vector< double > m_from;
	std::vector< double > m_to;
	std::vector< double > m_freq;

	virtual RDOValue next()
	{
		double ran1 = RandGeneratorUniform::next( 0, summ );
		double add = 0;
		std::vector< double >::size_type i;
		for ( i = 0; i < m_freq.size() - 1; i++ ) {
			if ( ran1 < m_freq[i] + add ) break;
			add += m_freq[i];
		}
		return RandGeneratorUniform::next( m_from[i], m_to[i] );
	}
};

class RandGeneratorByHistEnum: public RandGeneratorByHist
{
public:
	RandGeneratorByHistEnum( long int seed = 123456789 ):
		RandGeneratorByHist( seed )
	{
	}
	void addValues( RDOValue val, double freq )
	{
		m_vals.push_back( val );
		m_freq.push_back( freq );
		summ += freq;
	}

private:
	std::vector< RDOValue > m_vals;
	std::vector< double >   m_freq;

	virtual RDOValue next()
	{
		double ran1 = RandGeneratorUniform::next( 0, summ );
		double add = 0;
		std::vector< double >::size_type i;
		for ( i = 0; i < m_freq.size() - 1; i++ ) {
			if ( ran1 < m_freq[i] + add ) break;
			add += m_freq[i];
		}
		return m_vals[i];
	}
};

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
class RandGeneratorEnumerative: public RandGeneratorCommonNext
{
public:
	RandGeneratorEnumerative():
		RandGeneratorCommonNext(),
		m_curr( 0 )
	{
	}
	virtual ~RandGeneratorEnumerative()
	{
	}

	void addValue( RDOValue val )
	{
		m_vals.push_back( val );
	}

private:
	std::vector< RDOValue >            m_vals;
	std::vector< RDOValue >::size_type m_curr;

	virtual RDOValue next()
	{
		RDOValue res = m_vals[m_curr++];
		if ( m_curr >= m_vals.size() ) m_curr = 0;
		return res;
	}
};

} // namespace rdoParse 

#endif // RDO_RANDOM_DISTRIBUTION_H
