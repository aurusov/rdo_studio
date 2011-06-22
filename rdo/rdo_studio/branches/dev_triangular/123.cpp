#include "stdafx.h"
#include <iostream>
#include <map>
#include <math.h>

typedef unsigned int ruint;

class Uniform01
{
protected:
	Uniform01(ruint seed): m_seed(seed)
	{}

	double generate01()
	{
		m_seed = m_seed * 69069 + 1;
		return m_seed / 4294967296.0;
	}

private:
	ruint m_seed;
};


class Triang: public Uniform01
{
public:
	Triang(const double& mean, const int& min, const int& max, ruint seed)
		: Uniform01(seed  )
		, m_mean   (mean  )
		, m_min	   (min	  )
		, m_max	   (max   )
	{}

	double generate()
	{
		double result;
		double u01 = generate01();
		if ( u01 >= (m_mean-m_min)/(m_max-m_min))
		{
			for (result = -1; result < 0; result = asin(generate01()) - 0.570796326794897);
			result *= m_max - m_mean;
		}
		else
		{
			for (result = 1; result > 0; result = acos(generate01()) - 1);
			result *= m_mean - m_min;
		}
		return result + m_mean;
	}

private:
	double m_mean;
	double m_min;
	double m_max;
};


void main()
{
	Triang				triang			(3, 1, 10, 978557);

	double step = 0.1;
	typedef std::map<int, ruint> Histogram;
	Histogram m_histogram;

	for (ruint i = 0; i < 100000; i++)
	{
		double value = triang.generate();
		ruint index = static_cast<ruint>(value / step);
		std::pair<Histogram::iterator, bool> result =
			m_histogram.insert(Histogram::value_type(index, 0));
		if (!result.second)
		{
			++result.first->second;
		}
	}

	Histogram::const_iterator it = m_histogram.begin();
	while (it != m_histogram.end())
	{
		std::cout
			<< it->first
			<< ";"
			<< it->second
			<< std::endl;
		++it;
	}

	int i = 1;
}
