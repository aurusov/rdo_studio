#include "pch.h"
#include "rdo_fuzzy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
class RDOFuzzyValueItemFind
{
public:
	RDOFuzzyValueItemFind(CREF(RDOValue) rdovalue)
		: m_rdovalue(rdovalue)
	{}
	rbool operator() (CREF(RDOFuzzyValueItem) item) const
	{
		return item.m_rdovalue == m_rdovalue;
	}
private:
	CREF(RDOValue) m_rdovalue;
};

RDOFuzzyValue RDOFuzzyValue::operator&& (CREF(RDOFuzzyValue) fuzzy_value) const
{
	if (type() != fuzzy_value.type())
		throw RDOValueException();

	RDOFuzzyValue fuzzy_result(type());
	// Найдем только пересекающие элементы и выберем минимальную функцию принадлежности
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while (it1 != m_fuzzySet.end())
	{
		FuzzySet::const_iterator it2 = std::find_if(fuzzy_value.m_fuzzySet.begin(), fuzzy_value.m_fuzzySet.end(), RDOFuzzyValueItemFind(it1->m_rdovalue));
		if (it2 != fuzzy_value.m_fuzzySet.end())
		{
			fuzzy_result.m_fuzzySet.push_back(RDOFuzzyValueItem(it1->m_rdovalue, rdo::rmin(it1->m_appertain, it2->m_appertain)));
		}
		++it1;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::operator|| (CREF(RDOFuzzyValue) fuzzy_value) const
{
	if (type() != fuzzy_value.type())
		throw RDOValueException();

	RDOFuzzyValue fuzzy_result(type());
	// Объединим элементы двух множеств
	// Если элемент одновременно принадлежит обоим множествам, то выберем максимальную функцию принадлежности
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while (it1 != m_fuzzySet.end())
	{
		FuzzySet::const_iterator it2 = std::find_if(fuzzy_value.m_fuzzySet.begin(), fuzzy_value.m_fuzzySet.end(), RDOFuzzyValueItemFind(it1->m_rdovalue));
		if (it2 != fuzzy_value.m_fuzzySet.end())
		{
			fuzzy_result.m_fuzzySet.push_back(RDOFuzzyValueItem(it1->m_rdovalue, rdo::rmax(it1->m_appertain, it2->m_appertain)));
		}
		else
		{
			// Не нашли пары
			fuzzy_result.m_fuzzySet.push_back(*it1);
		}
		++it1;
	}
	FuzzySet::const_iterator it2 = fuzzy_value.m_fuzzySet.begin();
	while (it2 != fuzzy_value.m_fuzzySet.end())
	{
		FuzzySet::const_iterator it1 = std::find_if(m_fuzzySet.begin(), m_fuzzySet.end(), RDOFuzzyValueItemFind(it2->m_rdovalue));
		if (it1 == m_fuzzySet.end())
		{
			// Не нашли пары
			fuzzy_result.m_fuzzySet.push_back(*it2);
		}
		++it2;
	}
	// Отсортируем, чтобы элементы были упорядочены
	std::sort(fuzzy_result.m_fuzzySet.begin(), fuzzy_result.m_fuzzySet.end());
	return fuzzy_result;
}

//! Декартово произведение (попарное) элементов двух множест с применением произвольной функции fun
RDOFuzzyValue RDOFuzzyValue::ext_binary(ExtBinaryFun fun, CREF(RDOFuzzyValue) fuzzy_value) const
{
	typedef std::map< RDOValue, double > Values;
	Values values;
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while (it1 != m_fuzzySet.end())
	{
		FuzzySet::const_iterator it2 = fuzzy_value.m_fuzzySet.begin();
		while (it2 != fuzzy_value.m_fuzzySet.end())
		{
			RDOValue rdo_value = fun(it1->m_rdovalue, it2->m_rdovalue);
			Values::iterator val = values.find(rdo_value);
			if (val == values.end())
			{
				values[rdo_value] = rdo::rmin(it1->m_appertain, it2->m_appertain);
			}
			else
			{
				values[rdo_value] = rdo::rmax(val->second, rdo::rmin(it1->m_appertain, it2->m_appertain));
			}
			it2++;
		}
		it1++;
	}
	if (!values.empty())
	{
		PTR(RDOFuzzySetDefinitionRangeDiscret) fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret(type().getParent());
		fuzzy_setDefinition->append(values.begin()->first, values.rbegin()->first);
		PTR(RDOFuzzyType) fuzzy_type  = new RDOFuzzyType(fuzzy_setDefinition);
		RDOFuzzyValue fuzzy_result(*fuzzy_type);
		Values::const_iterator val = values.begin();
		while (val != values.end())
		{
			fuzzy_result.append(val->first, val->second);
			val++;
		}
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

//! Преобразование элементов через произвольную функцию fun
RDOFuzzyValue RDOFuzzyValue::ext_unary(ExtUnaryFun fun) const
{
	typedef std::map< RDOValue, double > Values;
	Values values;
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while (it != m_fuzzySet.end())
	{
		RDOValue rdo_value = fun(it->m_rdovalue);
		Values::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->m_appertain;
		}
		else
		{
			values[rdo_value] = rdo::rmax(val->second, it->m_appertain);
		}
		it++;
	}
	if (!values.empty())
	{
		PTR(RDOFuzzySetDefinitionRangeDiscret) fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret(type().getParent());
		fuzzy_setDefinition->append(values.begin()->first, values.rbegin()->first);
		PTR(RDOFuzzyType) fuzzy_type  = new RDOFuzzyType(fuzzy_setDefinition);
		RDOFuzzyValue fuzzy_result(*fuzzy_type);
		Values::const_iterator val = values.begin();
		while (val != values.end())
		{
			fuzzy_result.append(val->first, val->second);
			val++;
		}
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOFuzzyValue RDOFuzzyValue::ext_unary(ExtUnaryFunP fun, PTR(void) param) const
{
	typedef std::map< RDOValue, double > Values;
	Values values;
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while (it != m_fuzzySet.end())
	{
		RDOValue rdo_value = fun(it->m_rdovalue, param);
		Values::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->m_appertain;
		}
		else
		{
			values[rdo_value] = rdo::rmax(val->second, it->m_appertain);
		}
		it++;
	}
	if (!values.empty())
	{
		PTR(RDOFuzzySetDefinitionRangeDiscret) fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret(type().getParent());
		fuzzy_setDefinition->append(values.begin()->first, values.rbegin()->first);
		PTR(RDOFuzzyType) fuzzy_type  = new RDOFuzzyType(fuzzy_setDefinition);
		RDOFuzzyValue fuzzy_result(*fuzzy_type);
		Values::const_iterator val = values.begin();
		while (val != values.end())
		{
			fuzzy_result.append(val->first, val->second);
			val++;
		}
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOFuzzyValue RDOFuzzyValue::a_mult(CREF(RDOFuzzyValue) fuzzy_value) const
{
	if (type() != fuzzy_value.type())
		throw RDOValueException();

	RDOFuzzyValue fuzzy_result(type());
	// Найдем только пересекающие элементы и перемножим функции принадлежности
	FuzzySet::const_iterator it1 = m_fuzzySet.begin();
	while (it1 != m_fuzzySet.end())
	{
		FuzzySet::const_iterator it2 = std::find_if(fuzzy_value.m_fuzzySet.begin(), fuzzy_value.m_fuzzySet.end(), RDOFuzzyValueItemFind(it1->m_rdovalue));
		if (it2 != fuzzy_value.m_fuzzySet.end())
		{
			fuzzy_result.m_fuzzySet.push_back( RDOFuzzyValueItem(it1->m_rdovalue, it1->m_appertain * it2->m_appertain) );
		}
		it1++;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::alpha(double appertain) const
{
	     if (appertain < 0) appertain = 0;
	else if (appertain > 1) appertain = 1;

	RDOFuzzyValue fuzzy_result(type());
	// Найдем отсечку
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while (it != m_fuzzySet.end())
	{
		if (it->m_appertain >= appertain)
		{
			fuzzy_result.m_fuzzySet.push_back( RDOFuzzyValueItem(it->m_rdovalue, appertain) );
		}
		it++;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::a_pow(double power) const
{
	RDOFuzzyValue fuzzy_result(*this);
	// Возведём в степень
	FuzzySet::iterator it = fuzzy_result.m_fuzzySet.begin();
	while (it != fuzzy_result.m_fuzzySet.end())
	{
		it->m_appertain = ::pow(it->m_appertain, power);
		it++;
	}
	return fuzzy_result;
}

RDOValue RDOFuzzyValue::defuzzyfication()
{
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	if (it == m_fuzzySet.end())
		return RDOValue();

	FuzzySet::const_iterator it_next = it;
	it_next++;
	if (it_next == m_fuzzySet.end())
		return it->m_rdovalue;

	double g      = 0;
	double f      = 0;
	double x      = it->m_rdovalue.getDouble();
	double x_next = it_next->m_rdovalue.getDouble();
	double a      = it->m_appertain;
	double b      = it_next->m_appertain;
	double h      = x_next - x;
	while (true)
	{
		double t = h*(a+b)/2.0;
		g += t*(x_next + x)/2.0;
		f += t;

		it++;
		it_next++;
		if (it_next == m_fuzzySet.end())
			break;

		x      = x_next;
		a      = b;
		x_next = it_next->m_rdovalue.getDouble();
		b      = it_next->m_appertain;
		h      = x_next - x;
	}
	return g / f;
}

tstring RDOFuzzyValue::getAsString() const
{
	if (m_fuzzySet.empty())
		return _T("[empty value]");

	tstring res = _T("");
	FuzzySet::const_iterator it = m_fuzzySet.begin();
	while (it != m_fuzzySet.end())
	{
		res += rdo::format(_T("[%.2lf/%s]"), it->m_appertain, it->m_rdovalue.getAsString().c_str());
		it++;
		if (it != m_fuzzySet.end())
		{
			res += _T(", ");
		}
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
RDOFuzzyValue RDOFuzzySetDefinitionFixed::getSuppliment(CREF(RDOFuzzyValue) value) const
{
	RDOFuzzyValue fuzzy_result(value.type());
	Range::const_iterator range = begin();
	while (range != end())
	{
		rbool found = false;
		RDOFuzzyValue::FuzzySet::const_iterator val = value.begin();
		while (val != value.end())
		{
			if (val->m_rdovalue == *range)
			{
				found = true;
				break;
			}
			val++;
		}
		if (found)
		{
			double appertain = 1 - val->m_appertain;
			if (appertain > 0)
			{
				fuzzy_result.append(val->m_rdovalue, appertain);
			}
		}
		else
		{
			fuzzy_result.append(*range, 1);
		}
		range++;
	}
	return fuzzy_result;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
rbool RDOFuzzySetDefinitionRangeDiscret::inRange(CREF(RDOValue) rdovalue) const
{
	Ranges::const_iterator it = m_range.begin();
	while (it != m_range.end())
	{
		if (rdovalue >= it->m_from && rdovalue <= it->m_till)
		{
			return true;
		}
		++it;
	}
	return false;
}

RDOFuzzyValue RDOFuzzySetDefinitionRangeDiscret::getSuppliment(CREF(RDOFuzzyValue) value) const
{
	RDOFuzzyValue fuzzy_result(value.type());
	Ranges::const_iterator range = m_range.begin();
	while (range != m_range.end())
	{
		for (RDOValue ritem = range->m_from; ritem <= range->m_till; ritem += m_step)
		{
			rbool found = false;
			RDOFuzzyValue::FuzzySet::const_iterator val = value.begin();
			while (val != value.end())
			{
				if (val->m_rdovalue == ritem)
				{
					found = true;
					break;
				}
				val++;
			}
			if (found)
			{
				double appertain = 1 - val->m_appertain;
				if (appertain > 0)
				{
					fuzzy_result.append(val->m_rdovalue, appertain);
				}
			}
			else
			{
				fuzzy_result.append(ritem, 1);
			}
		}
		range++;
	}
	return fuzzy_result;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
PTR(RDOFuzzyEmptyType) RDOFuzzyEmptyType::s_emptyType = NULL;

} // namespace rdoRuntime
