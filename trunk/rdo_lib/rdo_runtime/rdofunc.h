#ifndef RDOFUNC_H
#define RDOFUNC_H

namespace rdoRuntime
{

class RandGenerator
{
protected:
// Так было
	unsigned int seed;
// Так стало, хотя можно оставить и unsigned int imho
//	long int seed;
	RandGenerator( long int _seed = 123456789 ): seed( _seed ) {}

public:
	virtual ~RandGenerator() {} // Удалять нельзя, нужен именно виртуальный диструктор для вызова из RDOCalcSeqInit::~RDOCalcSeqInit()
	void setSeed( long int _seed = 123456789 ) { seed = _seed; }
	double u01() {
// Так было
		seed = seed * 69069 + 1;
		return seed / 4294967296.0; //(long double)UINT_MAX + 1
// Так стало
/*
		const long int MODLUS = 2147483647;
		const long int MULT1  = 24112;
		const long int MULT2  = 26143;
		long int zi = seed;
		long int lowprd = (zi & 65535) * MULT1;
		long int hi31   = (zi >> 16) * MULT1 + (lowprd >> 16);
		zi = ((lowprd & 65535) - MODLUS) + ((hi31 & 32767) << 16) + (hi31 >> 15);
		if ( zi < 0 ) zi += MODLUS;
		lowprd = (zi & 65535) * MULT2;
		hi31 = (zi >> 16) * MULT2 + (lowprd >> 16);
		zi = ((lowprd & 65535) - MODLUS) + ((hi31 & 32767) << 16) + (hi31 >> 15);
		if ( zi < 0 ) zi += MODLUS;
		seed = zi;
//		return ((zi >> 7) + 1) / 16777216.0;       // Так в примере на FORTRAN imho
		return (((zi >> 7) | 1) + 1) / 16777216.0; // Так в примере на C
*/
	}
};

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
class RandGeneratorUniform: public RandGenerator
{
public:
	RandGeneratorUniform( long int _seed = 123456789 ):
		RandGenerator( _seed )
	{
	}
	double next( double from, double to ) {
		return u01() * ( to - from ) + from;
	}
};

class RandGeneratorExponential: public RandGenerator
{
public:
	RandGeneratorExponential( long int _seed = 123456789 ):
		RandGenerator( _seed )
	{
	}
	double next( double math ) {
		return -log( u01() ) * math;
	}
};

class RandGeneratorNormal: public RandGenerator
{
public:
	RandGeneratorNormal( long int _seed = 123456789 ):
		RandGenerator( _seed )
	{
	}
	double next( double av, double var ) {
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
protected:
	RandGeneratorCommonNext() {}

public:
	virtual double next() = 0;
};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;
	RandGeneratorByHist( long int _seed = 123456789 ):
		RandGeneratorUniform( _seed ),
		RandGeneratorCommonNext(),
		summ( 0 )
	{
	}
};

class RandGeneratorByHistReal: public RandGeneratorByHist
{
private:
	std::vector< double > from;
	std::vector< double > to;
	std::vector< double > freq;

	virtual double next() {
		double ran1 = RandGeneratorUniform::next( 0, summ );
		double add = 0;
		for ( int i = 0; i < freq.size() - 1; i++ ) {
			if ( ran1 < freq[i] + add ) break;
			add += freq[i];
		}
		return RandGeneratorUniform::next( from[i], to[i] );
	}

public:
	RandGeneratorByHistReal( long int _seed = 123456789 ):
		RandGeneratorByHist( _seed )
	{
	}
	void addValues( double _from, double _to, double _freq ) {
		from.push_back( _from );
		to.push_back( _to );
		freq.push_back( _freq );
		summ += _freq;
	}
};

class RandGeneratorByHistEnum: public RandGeneratorByHist
{
private:
	std::vector< int >    vals;
	std::vector< double > freq;

	virtual double next() {
		double ran1 = RandGeneratorUniform::next( 0, summ );
		double add = 0;
		for ( int i = 0; i < freq.size() - 1; i++ ) {
			if ( ran1 < freq[i] + add ) break;
			add += freq[i];
		}
		return vals[i];
	}

public:
	RandGeneratorByHistEnum( long int _seed = 123456789 ):
		RandGeneratorByHist( _seed )
	{
	}
	void addValues( int _val, double _freq ) {
		vals.push_back( _val );
		freq.push_back( _freq );
		summ += _freq;
	}
};

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
class RandGeneratorEnumerative: public RandGeneratorCommonNext
{
private:
	std::vector< double > vals;
	int curr;

	virtual double next() {
		double res = vals[curr++];
		if ( curr >= vals.size() ) curr = 0;
		return res;
	}

public:
	RandGeneratorEnumerative():
		RandGeneratorCommonNext(),
		curr( 0 )
	{
	}
	void addValue( double val ) {
		vals.push_back( val );
	}
};

} // namespace rdoParse 

#endif // RDOFUNC_H
