#include "pch.h"
#include "rdo_fuzzy.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
RDOFuzzyValue RDOFuzzyValue::operator&& (CREF(RDOFuzzyValue) fuzzy_value) const
{
	if (type() != fuzzy_value.type())
		throw RDOValueException();

	RDOFuzzyValue fuzzy_result(type());
	// Найдем только пересекающие элементы и выберем минимальную функцию принадлежности
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = fuzzy_value.find(it1->first);
		if (it2 != fuzzy_value.end())
		{
			fuzzy_result[it1->first] = rdo::rmin(it1->second, it2->second);
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
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = fuzzy_value.find(it1->first);
		if (it2 != fuzzy_value.end())
		{
			fuzzy_result[it1->first] = rdo::rmax(it1->second, it2->second);
		}
		else
		{
			// Не нашли пары
			fuzzy_result[it1->first] = it1->second;
		}
		++it1;
	}
	FuzzySet::const_iterator it2 = fuzzy_value.begin();
	while (it2 != fuzzy_value.end())
	{
		FuzzySet::const_iterator it1 = find(it2->first);
		if (it1 == end())
		{
			// Не нашли пары
			fuzzy_result[it2->first] = it2->second;
		}
		++it2;
	}
	return fuzzy_result;
}

//! Декартово произведение (попарное) элементов двух множест с применением произвольной функции fun
RDOFuzzyValue RDOFuzzyValue::ext_binary(ExtBinaryFun fun, CREF(RDOFuzzyValue) fuzzy_value) const
{
	FuzzySet values;
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = fuzzy_value.begin();
		while (it2 != fuzzy_value.end())
		{
			RDOValue rdo_value = fun(it1->first, it2->first);
			FuzzySet::iterator val = values.find(rdo_value);
			if (val == values.end())
			{
				values[rdo_value] = rdo::rmin(it1->second, it2->second);
			}
			else
			{
				values[rdo_value] = rdo::rmax(val->second, rdo::rmin(it1->second, it2->second));
			}
			it2++;
		}
		it1++;
	}
	if (!values.empty())
	{
		PTR(RDOFuzzySetDefinitionRangeDiscret) fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret(type().getParent(), values.begin()->first, values.rbegin()->first);
		PTR(RDOFuzzyType) fuzzy_type = new RDOFuzzyType(fuzzy_setDefinition);
		RDOFuzzyValue fuzzy_result(*fuzzy_type);
		fuzzy_result.m_fuzzySet = values;
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
	FuzzySet values;
	FuzzySet::const_iterator it = begin();
	while (it != end())
	{
		RDOValue rdo_value = fun(it->first);
		FuzzySet::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->second;
		}
		else
		{
			values[rdo_value] = rdo::rmax(val->second, it->second);
		}
		it++;
	}
	if (!values.empty())
	{
		PTR(RDOFuzzySetDefinitionRangeDiscret) fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret(type().getParent(), values.begin()->first, values.rbegin()->first);
		PTR(RDOFuzzyType) fuzzy_type = new RDOFuzzyType(fuzzy_setDefinition);
		RDOFuzzyValue fuzzy_result(*fuzzy_type);
		fuzzy_result.m_fuzzySet = values;
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOFuzzyValue RDOFuzzyValue::ext_unary(ExtUnaryFunP fun, PTR(void) param) const
{
	FuzzySet values;
	FuzzySet::const_iterator it = begin();
	while (it != end())
	{
		RDOValue rdo_value = fun(it->first, param);
		FuzzySet::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->second;
		}
		else
		{
			values[rdo_value] = rdo::rmax(val->second, it->second);
		}
		it++;
	}
	if (!values.empty())
	{
		PTR(RDOFuzzySetDefinitionRangeDiscret) fuzzy_setDefinition = new RDOFuzzySetDefinitionRangeDiscret(type().getParent(), values.begin()->first, values.rbegin()->first);
		PTR(RDOFuzzyType) fuzzy_type = new RDOFuzzyType(fuzzy_setDefinition);
		RDOFuzzyValue fuzzy_result(*fuzzy_type);
		fuzzy_result.m_fuzzySet = values;
		return fuzzy_result;
	}
	else
	{
		return RDOFuzzyValue(RDOFuzzyEmptyType::getInstance(type().getParent()));
	}
}

RDOValue fun_add (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 + value2; }
RDOValue fun_sub (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 - value2; }
RDOValue fun_mult(CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 * value2; }
RDOValue fun_div (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 / value2; }

RDOFuzzyValue RDOFuzzyValue::operator+ (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_add,  fuzzy_value); }
RDOFuzzyValue RDOFuzzyValue::operator- (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_sub,  fuzzy_value); }
RDOFuzzyValue RDOFuzzyValue::operator* (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_mult, fuzzy_value); }
RDOFuzzyValue RDOFuzzyValue::operator/ (CREF(RDOFuzzyValue) fuzzy_value) const { return ext_binary(fun_div,  fuzzy_value); }

RDOValue fun_u_minus(CREF(RDOValue) value                 ) { return -value;                                     }
RDOValue fun_u_obr  (CREF(RDOValue) value                 ) { return RDOValue(1)/value;                          }
RDOValue fun_u_scale(CREF(RDOValue) value, PTR(void) scale) { return value * (*static_cast<PTR(double)>(scale)); }
RDOValue fun_u_log  (CREF(RDOValue) value                 ) { return value > 0 ? log(value.getDouble()) : 0;     }

RDOFuzzyValue RDOFuzzyValue::u_minus()             const { return ext_unary(fun_u_minus        ); }
RDOFuzzyValue RDOFuzzyValue::u_obr  ()             const { return ext_unary(fun_u_obr          ); }
RDOFuzzyValue RDOFuzzyValue::u_scale(double scale) const { return ext_unary(fun_u_scale, &scale); }
RDOFuzzyValue RDOFuzzyValue::u_log  ()             const { return ext_unary(fun_u_log          ); }

RDOFuzzyValue RDOFuzzyValue::a_mult(CREF(RDOFuzzyValue) fuzzy_value) const
{
	if (type() != fuzzy_value.type())
		throw RDOValueException();

	RDOFuzzyValue fuzzy_result(type());
	// Найдем только пересекающие элементы и перемножим функции принадлежности
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = fuzzy_value.find(it1->first);
		if (it2 != fuzzy_value.end())
		{
			fuzzy_result[it1->first] = it1->second * it2->second;
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
	FuzzySet::const_iterator it = begin();
	while (it != end())
	{
		if (it->second >= appertain)
		{
			fuzzy_result[it->first] = appertain;
		}
		it++;
	}
	return fuzzy_result;
}

RDOFuzzyValue RDOFuzzyValue::a_pow(double power) const
{
	RDOFuzzyValue fuzzy_result(*this);
	// Возведём в степень
	FuzzySet::iterator it = fuzzy_result.begin();
	while (it != fuzzy_result.end())
	{
		it->second = ::pow(it->second, power);
		it++;
	}
	return fuzzy_result;
}

RDOValue RDOFuzzyValue::defuzzyfication()
{
	FuzzySet::const_iterator it = begin();
	if (it == end())
		return RDOValue();

	FuzzySet::const_iterator it_next = it;
	it_next++;
	if (it_next == end())
		return it->first;

	double g      = 0;
	double f      = 0;
	double x      = it->first.getDouble();
	double x_next = it_next->first.getDouble();
	double a      = it->second;
	double b      = it_next->second;
	double h      = x_next - x;
	while (true)
	{
		double t = h*(a+b)/2.0;
		g += t*(x_next + x)/2.0;
		f += t;

		it++;
		it_next++;
		if (it_next == end())
			break;

		x      = x_next;
		a      = b;
		x_next = it_next->first.getDouble();
		b      = it_next->second;
		h      = x_next - x;
	}
	return g / f;
}

tstring RDOFuzzyValue::getAsString() const
{
	if (empty())
		return _T("[empty value]");

	tstring res = _T("");
	FuzzySet::const_iterator it = begin();
	while (it != end())
	{
		rbool output = it->second > 0.0;
		if (output)
			res += rdo::format(_T("<%s/%.2lf>"), it->first.getAsString().c_str(), it->second);

		++it;

		if (output && it != end())
			res += _T(" ");
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
RDOFuzzyValue RDOFuzzySetDefinitionFixed::getSupplement(CREF(RDOFuzzyValue) value) const
{
	RDOFuzzyValue fuzzy_result(value.type());
	RDOFuzzyValue::FuzzySet::const_iterator rangeItemIt = m_items.begin();
	while (rangeItemIt != m_items.end())
	{
		RDOFuzzyValue::FuzzyItem found = value.findValue(rangeItemIt->first);
		fuzzy_result[found.first] = 1.0 - found.second;
		++rangeItemIt;
	}
	return fuzzy_result;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
rbool RDOFuzzySetDefinitionRangeDiscret::inRange(CREF(RDOValue) value) const
{
	return value >= m_from && value <= m_till;
}

RDOFuzzyValue RDOFuzzySetDefinitionRangeDiscret::getSupplement(CREF(RDOFuzzyValue) value) const
{
	RDOFuzzyValue fuzzy_result(value.type());
	for (RDOValue ritem = m_from; ritem <= m_till; ritem += m_step)
	{
		RDOFuzzyValue::FuzzyItem found = value.findValue(ritem);
		fuzzy_result[found.first] = 1.0 - found.second;
	}
	return fuzzy_result;
}

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
PTR(RDOFuzzyEmptyType) RDOFuzzyEmptyType::s_emptyType = NULL;

} // namespace rdoRuntime
