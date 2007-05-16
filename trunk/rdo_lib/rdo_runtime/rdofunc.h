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
	void setSeed( long int _seed = 123456789 ) { seed = _seed; }
	virtual ~RandGenerator() {};
	double u01() {
// Так было
		seed = seed * 69069 + 1;
		return seed / ((long double)0xFFFFFFFF + 1);
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

class RandGeneratorUniform: public RandGenerator
{
public:
	RandGeneratorUniform( long int _seed = 123456789 ): RandGenerator( _seed ) {}
	double next( double from, double to ) {
		return u01() * ( to - from ) + from;
	}
};

class RandGeneratorExponential: public RandGenerator
{
public:
	RandGeneratorExponential( long int _seed = 123456789 ): RandGenerator( _seed ) {}
	double next( double math ) {
		return -log( u01() ) * math;
	}
};

class RandGeneratorNormal: public RandGenerator
{
public:
	RandGeneratorNormal( long int _seed = 123456789 ): RandGenerator( _seed ) {}
	double next( double av, double var ) {
		double ran = 0;
		for ( int i = 0; i < 12; i++ ) {
			ran += u01();
		}
		return var * (ran - 6) + av;
	}
};

class RandGeneratorByHistCommon: public RandGeneratorUniform
{
protected:
	double summ;
	RandGeneratorByHistCommon( long int _seed = 123456789 ): RandGeneratorUniform( _seed ) {}

public:
	virtual double next() = 0;
};

class RandGeneratorByHist: public RandGeneratorByHistCommon
{
private:
	std::vector< double > from;
	std::vector< double > to;
	std::vector< double > freq;

public:
	RandGeneratorByHist( long int _seed = 123456789 ): RandGeneratorByHistCommon( _seed ) {}
	void setValues( int num, ... ) {
		va_list ap;
		va_start(ap, num);
		from.clear();
		to.clear();
		freq.clear();
		for ( int i =0; i < num; i++ ) {
			from.push_back(va_arg(ap, double));
			to.push_back(va_arg(ap, double));
			freq.push_back(va_arg(ap, double));
		}
		va_end(ap);
		summ = 0;
		for ( i = 0; i < freq.size(); i++ ) {
			summ += freq[i];
		}
	}
	void addValues( double _from, double _to, double _freq ) {
		from.push_back(_from);
		to.push_back(_to);
		freq.push_back(_freq);
		summ = 0;
		for ( int i = 0; i < freq.size(); i++ ) {
			summ += freq[i];
		}
	}
	virtual double next() {
		double ran1 = RandGeneratorUniform::next( 0, summ );
		double add = 0;
		for ( int i = 0; i < freq.size() - 1; i++ ) {
			if ( ran1 < freq[i] + add ) break;
			add += freq[i];
		}
		return RandGeneratorUniform::next( from[i], to[i] );
	}
};

class RandGeneratorByHistEnum: public RandGeneratorByHistCommon
{
private:
	std::vector<int> vals;
	std::vector<double> freq;

public:
	RandGeneratorByHistEnum( long int _seed = 123456789 ): RandGeneratorByHistCommon( _seed ) {}
	void setValues( int num, ... ) {
		va_list ap;
		va_start(ap, num);
		vals.clear();
		freq.clear();
		for ( int i =0; i < num; i++ ) {
			vals.push_back(va_arg(ap, int));
			freq.push_back(va_arg(ap, double));
		}
		va_end(ap);
		summ = 0;
		for (i = 0; i < freq.size(); i++ ) {
			summ += freq[i];
		}
	}
	void addValues( int _val, double _freq ) {
		vals.push_back(_val);
		freq.push_back(_freq);
		summ = 0;
		for ( int i = 0; i < freq.size(); i++ ) {
			summ += freq[i];
		}
	}
	virtual double next() {
		double ran1 = RandGeneratorUniform::next( 0, summ );
		double add = 0;
		for ( int i = 0; i < freq.size() - 1; i++ ) {
			if ( ran1 < freq[i] + add ) break;
			add += freq[i];
		}
		return vals[i];
	}
};

class RandGeneratorEnumerative: public RandGeneratorByHistCommon
{
private:
	std::vector< double > vals;
	int curr;

public:
	void setValues( int num, ... ) {
		va_list ap;
		va_start(ap, num);
		vals.clear();
		for ( int i =0; i < num; i++ ) {
			vals.push_back( va_arg(ap, double) );
		}
		va_end(ap);
		curr = 0;
	}
	void addValue( double val ) {
		vals.push_back(val);
		curr = 0;
	}
	virtual double next() {
		double res = vals[curr++];
		if ( curr >= vals.size() ) curr = 0;
		return res;
	}
};

} // namespace rdoParse 

#endif // RDOFUNC_H
