/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.07.2008
  \brief     Нечеткая логика
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyValue
// --------------------------------------------------------------------------------
LPRDOFuzzyValue RDOFuzzyValue::operator&& (CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	if (type() != pFuzzyValue->type())
		throw RDOValueException();

	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(type());
	ASSERT(pFuzzyValueResult);
	// Найдем только пересекающие элементы и выберем минимальную функцию принадлежности
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = pFuzzyValue->find(it1->first);
		if (it2 != pFuzzyValue->end())
		{
			pFuzzyValueResult->operator[](it1->first) = rdo::rmin(it1->second, it2->second);
		}
		++it1;
	}
	return pFuzzyValueResult;
}

LPRDOFuzzyValue RDOFuzzyValue::operator|| (CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	if (type() != pFuzzyValue->type())
		throw RDOValueException();

	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(type());
	ASSERT(pFuzzyValueResult);
	// Объединим элементы двух множеств
	// Если элемент одновременно принадлежит обоим множествам, то выберем максимальную функцию принадлежности
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = pFuzzyValue->find(it1->first);
		if (it2 != pFuzzyValue->end())
		{
			pFuzzyValueResult->operator[](it1->first) = rdo::rmax(it1->second, it2->second);
		}
		else
		{
			// Не нашли пары
			pFuzzyValueResult->operator[](it1->first) = it1->second;
		}
		++it1;
	}
	FuzzySet::const_iterator it2 = pFuzzyValue->begin();
	while (it2 != pFuzzyValue->end())
	{
		FuzzySet::const_iterator it1 = find(it2->first);
		if (it1 == end())
		{
			// Не нашли пары
			pFuzzyValueResult->operator[](it2->first) = it2->second;
		}
		++it2;
	}
	return pFuzzyValueResult;
}

/// @todo комментарии в *.h
//! Декартово произведение (попарное) элементов двух множест с применением произвольной функции fun
LPRDOFuzzyValue RDOFuzzyValue::ext_binary(ExtBinaryFun fun, CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	FuzzySet values;
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = pFuzzyValue->begin();
		while (it2 != pFuzzyValue->end())
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
		LPRDOFuzzySetDefinitionRangeDiscret pFuzzySetDefinition = rdo::Factory<RDOFuzzySetDefinitionRangeDiscret>::create(values.begin()->first, values.rbegin()->first);
		ASSERT(pFuzzySetDefinition);

		LPRDOFuzzyType pFuzzyType = rdo::Factory<RDOFuzzyType>::create(pFuzzySetDefinition);
		ASSERT(pFuzzyType);

		LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(pFuzzyType);
		ASSERT(pFuzzyValueResult);
		pFuzzyValueResult->m_fuzzySet = values;

		return pFuzzyValueResult;
	}
	else
	{
		return rdo::Factory<RDOFuzzyValue>::create(RDOFuzzyEmptyType::getInstance());
	}
}

//! Преобразование элементов через произвольную функцию fun
LPRDOFuzzyValue RDOFuzzyValue::ext_unary(ExtUnaryFun fun) const
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
		LPRDOFuzzySetDefinitionRangeDiscret pFuzzySetDefinition = rdo::Factory<RDOFuzzySetDefinitionRangeDiscret>::create(values.begin()->first, values.rbegin()->first);
		ASSERT(pFuzzySetDefinition);

		LPRDOFuzzyType pFuzzyType = rdo::Factory<RDOFuzzyType>::create(pFuzzySetDefinition);
		ASSERT(pFuzzyType);

		LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(pFuzzyType);
		ASSERT(pFuzzyValueResult);
		pFuzzyValueResult->m_fuzzySet = values;

		return pFuzzyValueResult;
	}
	else
	{
		return rdo::Factory<RDOFuzzyValue>::create(RDOFuzzyEmptyType::getInstance());
	}
}

LPRDOFuzzyValue RDOFuzzyValue::ext_unary(ExtUnaryFunP fun, PTR(void) pParam) const
{
	FuzzySet values;
	FuzzySet::const_iterator it = begin();
	while (it != end())
	{
		RDOValue rdo_value = fun(it->first, pParam);
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
		LPRDOFuzzySetDefinitionRangeDiscret pFuzzySetDefinition = rdo::Factory<RDOFuzzySetDefinitionRangeDiscret>::create(values.begin()->first, values.rbegin()->first);
		ASSERT(pFuzzySetDefinition);

		LPRDOFuzzyType pFuzzyType = rdo::Factory<RDOFuzzyType>::create(pFuzzySetDefinition);
		ASSERT(pFuzzyType);

		LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(pFuzzyType);
		ASSERT(pFuzzyValueResult);
		pFuzzyValueResult->m_fuzzySet = values;

		return pFuzzyValueResult;
	}
	else
	{
		return rdo::Factory<RDOFuzzyValue>::create(RDOFuzzyEmptyType::getInstance());
	}
}

RDOValue fun_add (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 + value2; }
RDOValue fun_sub (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 - value2; }
RDOValue fun_mult(CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 * value2; }
RDOValue fun_div (CREF(RDOValue) value1, CREF(RDOValue) value2) { return value1 / value2; }

LPRDOFuzzyValue RDOFuzzyValue::operator+ (CREF(LPRDOFuzzyValue) pFuzzyValue) const { return ext_binary(fun_add,  pFuzzyValue); }
LPRDOFuzzyValue RDOFuzzyValue::operator- (CREF(LPRDOFuzzyValue) pFuzzyValue) const { return ext_binary(fun_sub,  pFuzzyValue); }
LPRDOFuzzyValue RDOFuzzyValue::operator* (CREF(LPRDOFuzzyValue) pFuzzyValue) const { return ext_binary(fun_mult, pFuzzyValue); }
LPRDOFuzzyValue RDOFuzzyValue::operator/ (CREF(LPRDOFuzzyValue) pFuzzyValue) const { return ext_binary(fun_div,  pFuzzyValue); }

RDOValue fun_u_minus(CREF(RDOValue) rdovalue                 ) { return -rdovalue;                                     }
RDOValue fun_u_obr  (CREF(RDOValue) rdovalue                 ) { return RDOValue(1)/rdovalue;                          }
RDOValue fun_u_scale(CREF(RDOValue) rdovalue, PTR(void) scale) { return rdovalue * (*static_cast<PTR(double)>(scale)); }
RDOValue fun_u_log  (CREF(RDOValue) rdovalue                 ) { return rdovalue > 0 ? log(rdovalue.getDouble()) : 0;     }

LPRDOFuzzyValue RDOFuzzyValue::u_minus()             const { return ext_unary(fun_u_minus        ); }
LPRDOFuzzyValue RDOFuzzyValue::u_obr  ()             const { return ext_unary(fun_u_obr          ); }
LPRDOFuzzyValue RDOFuzzyValue::u_scale(double scale) const { return ext_unary(fun_u_scale, &scale); }
LPRDOFuzzyValue RDOFuzzyValue::u_log  ()             const { return ext_unary(fun_u_log          ); }

LPRDOFuzzyValue RDOFuzzyValue::a_mult(CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	if (type() != pFuzzyValue->type())
		throw RDOValueException();

	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(type());
	ASSERT(pFuzzyValueResult);

	// Найдем только пересекающие элементы и перемножим функции принадлежности
	FuzzySet::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::const_iterator it2 = pFuzzyValue->find(it1->first);
		if (it2 != pFuzzyValue->end())
		{
			pFuzzyValueResult->operator[](it1->first) = it1->second * it2->second;
		}
		it1++;
	}
	return pFuzzyValueResult;
}

LPRDOFuzzyValue RDOFuzzyValue::alpha(double appertain) const
{
	     if (appertain < 0) appertain = 0;
	else if (appertain > 1) appertain = 1;

	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(type());
	ASSERT(pFuzzyValueResult);

	// Найдем отсечку
	FuzzySet::const_iterator it = begin();
	while (it != end())
	{
		if (it->second >= appertain)
		{
			pFuzzyValueResult->operator[](it->first) = appertain;
		}
		it++;
	}
	return pFuzzyValueResult;
}

LPRDOFuzzyValue RDOFuzzyValue::a_pow(double power) const
{
	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(LPRDOFuzzyValue(const_cast<PTR(RDOFuzzyValue)>(this)));
	ASSERT(pFuzzyValueResult);

	// Возведём в степень
	FuzzySet::iterator it = pFuzzyValueResult->begin();
	while (it != pFuzzyValueResult->end())
	{
		it->second = ::pow(it->second, power);
		it++;
	}
	return pFuzzyValueResult;
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
	for (;;)
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

LPRDOFuzzyValue RDOFuzzyValue::clone() const
{
	return rdo::Factory<RDOFuzzyValue>::create(LPRDOFuzzyValue(const_cast<PTR(RDOFuzzyValue)>(this)));
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyType
// --------------------------------------------------------------------------------
//LPRDOActivatedValue RDOFuzzyType::fuzzyfication(CREF(RDOValue) rdovalue)
//{
//	TermSet::const_iterator it = begin();
//	if (this->inRange(rdovalue))
//	{
//		LPRDOActivatedValue pValue = rdo::Factory<RDOActivatedValue>::create();
//		for (it = begin(); it != end(); it++)
//		{
//			LPRDOFuzzyValue fuzzySet = it->second;
//			RDOFuzzyValue::FuzzyItem item = fuzzySet->findValue(rdovalue);
// 			pValue->append(it->first,item.second);
//		}
//		return (pValue);
//	}
//}
// --------------------------------------------------------------------------------
// -------------------- RDOFuzzySetDefinitionFixed
// --------------------------------------------------------------------------------
LPRDOFuzzyValue RDOFuzzySetDefinitionFixed::getSupplement(CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(pFuzzyValue->type());
	ASSERT(pFuzzyValueResult);

	RDOFuzzyValue::FuzzySet::const_iterator rangeItemIt = m_items.begin();
	while (rangeItemIt != m_items.end())
	{
		RDOFuzzyValue::FuzzyItem found = pFuzzyValue->findValue(rangeItemIt->first);
		pFuzzyValueResult->operator[](found.first) = 1.0 - found.second;
		++rangeItemIt;
	}
	return pFuzzyValueResult;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzySetDefinitionRangeDiscret
// --------------------------------------------------------------------------------
rbool RDOFuzzySetDefinitionRangeDiscret::inRange(CREF(RDOValue) rdovalue) const
{
	return rdovalue >= m_from && rdovalue <= m_till;
}

LPRDOFuzzyValue RDOFuzzySetDefinitionRangeDiscret::getSupplement(CREF(LPRDOFuzzyValue) pFuzzyValue) const
{
	LPRDOFuzzyValue pFuzzyValueResult = rdo::Factory<RDOFuzzyValue>::create(pFuzzyValue->type());
	ASSERT(pFuzzyValueResult);

	for (RDOValue ritem = m_from; ritem <= m_till; ritem += m_step)
	{
		RDOFuzzyValue::FuzzyItem found = pFuzzyValue->findValue(ritem);
		pFuzzyValueResult->operator[](found.first) = 1.0 - found.second;
	}
	return pFuzzyValueResult;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuzzyEmptyType
// --------------------------------------------------------------------------------
PTR(RDOFuzzyEmptyType) RDOFuzzyEmptyType::s_emptyType = NULL;

CLOSE_RDO_RUNTIME_NAMESPACE
