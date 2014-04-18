/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.07.2008
  \brief     Нечеткая логика
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- MemberFunctionProperties
// --------------------------------------------------------------------------------
LPFuzzySet FuzzySet::operator&& (const LPFuzzySet& pSet) const
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);
	// Найдем только пересекающие элементы и выберем минимальную функцию принадлежности
	FuzzySetDefinition::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet->find(it1->first);
		if (it2 != pSet->end())
		{
			pFuzzySetResult->operator[](it1->first) = (std::min)(it1->second, it2->second);
		}
		++it1;
	}
	return pFuzzySetResult;
}

LPFuzzySet FuzzySet::operator|| (const LPFuzzySet& pSet)const
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);
	// Объединим элементы двух множеств
	// Если элемент одновременно принадлежит обоим множествам, то выберем максимальную функцию принадлежности
	FuzzySetDefinition::const_iterator it1 = begin();
	while (it1 != end())
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet->find(it1->first);
		if (it2 != pSet->end())
		{
			pFuzzySetResult->operator[](it1->first) = (std::max)(it1->second, it2->second);
		}
		else
		{
			// Не нашли пары
			pFuzzySetResult->operator[](it1->first) = it1->second;
		}
		++it1;
	}
	FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet->begin();
	while (it2 != pSet->end())
	{
		FuzzySetDefinition::const_iterator it1 = find(it2->first);
		if (it1 == end())
		{
			// Не нашли пары
			pFuzzySetResult->operator[](it2->first) = it2->second;
		}
		++it2;
	}
	return pFuzzySetResult;
}
/// @todo комментарии в *.h
//! Декартово произведение (попарное) элементов двух множест с применением произвольной функции fun
LPFuzzySet MemberFunctionProperties::ext_binary(ExtBinaryFun fun, const LPFuzzySet& pSet1, const LPFuzzySet& pSet2)
{
	FuzzySet::FuzzySetDefinition values;
	FuzzySet::FuzzySetDefinition::const_iterator it1 = pSet1->begin();
	while (it1 != pSet1->end())
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet2->begin();
		while (it2 != pSet2->end())
		{
			RDOValue rdo_value = fun(it1->first, it2->first);
			FuzzySet::FuzzySetDefinition::iterator val = values.find(rdo_value);
			if (val == values.end())
			{
				values[rdo_value] = (std::min)(it1->second, it2->second);
			}
			else
			{
				values[rdo_value] = (std::max)(val->second, (std::min)(it1->second, it2->second));
			}
			++it2;
		}
		++it1;
	}

	if (!values.empty())
	{
		LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
		ASSERT(pFuzzySetResult);
		pFuzzySetResult->setValues(values);

		return pFuzzySetResult;
	}

	return LPFuzzySet();
}

//! Преобразование элементов через произвольную функцию fun
LPFuzzySet MemberFunctionProperties::ext_unary(ExtUnaryFun fun, const LPFuzzySet& pSet)
{
	FuzzySet::FuzzySetDefinition values;
	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();
	while (it != pSet->end())
	{
		RDOValue rdo_value = fun(it->first);
		FuzzySet::FuzzySetDefinition::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->second;
		}
		else
		{
			values[rdo_value] = (std::max)(val->second, it->second);
		}
		++it;
	}

	if (!values.empty())
	{

		LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
		ASSERT(pFuzzySetResult);
		pFuzzySetResult->setValues(values);

		return pFuzzySetResult;
	}

	return LPFuzzySet();
}

LPFuzzySet MemberFunctionProperties::ext_unary(ExtUnaryFunP fun, void* pParam, const LPFuzzySet& pSet)
{
	FuzzySet::FuzzySetDefinition values;
	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();
	while (it != pSet->end())
	{
		RDOValue rdo_value = fun(it->first, pParam);
		FuzzySet::FuzzySetDefinition::iterator val = values.find(rdo_value);
		if (val == values.end())
		{
			values[rdo_value] = it->second;
		}
		else
		{
			values[rdo_value] = (std::max)(val->second, it->second);
		}
		++it;
	}

	if (!values.empty())
	{

		LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
		ASSERT(pFuzzySetResult);
		pFuzzySetResult->setValues(values);
		return pFuzzySetResult;
	}

	return LPFuzzySet();
}

RDOValue fun_add (const RDOValue& value1, const RDOValue& value2) { return value1 + value2; }
RDOValue fun_sub (const RDOValue& value1, const RDOValue& value2) { return value1 - value2; }
RDOValue fun_mult(const RDOValue& value1, const RDOValue& value2) { return value1 * value2; }
RDOValue fun_div (const RDOValue& value1, const RDOValue& value2) { return value1 / value2; }

LPFuzzySet FuzzySet::operator+ (const LPFuzzySet& pSet) const
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_add, pThis, pSet);
}
LPFuzzySet FuzzySet::operator- (const LPFuzzySet& pSet) const 
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_sub, pThis, pSet);
}
LPFuzzySet FuzzySet::operator* (const LPFuzzySet& pSet) const
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_mult, pThis, pSet);
}
LPFuzzySet FuzzySet::operator/ (const LPFuzzySet& pSet) const
{
	LPFuzzySet pThis(const_cast<FuzzySet*>(this));
	return MemberFunctionProperties::ext_binary(fun_div, pThis, pSet);
}

RDOValue fun_u_minus(const RDOValue& rdovalue             ) { return -rdovalue;                                    }
RDOValue fun_u_obr  (const RDOValue& rdovalue             ) { return RDOValue(1)/rdovalue;                         }
RDOValue fun_u_scale(const RDOValue& rdovalue, void* scale) { return rdovalue * (*static_cast<double*>(scale));    }
RDOValue fun_u_log  (const RDOValue& rdovalue             ) { return rdovalue > 0 ? log(rdovalue.getDouble()) : 0; }

LPFuzzySet MemberFunctionProperties::u_minus(const LPFuzzySet& pSet)               { return ext_unary(fun_u_minus,           pSet); }
LPFuzzySet MemberFunctionProperties::u_obr  (const LPFuzzySet& pSet)               { return ext_unary(fun_u_obr,             pSet); }
LPFuzzySet MemberFunctionProperties::u_scale(const LPFuzzySet& pSet, double scale) { return ext_unary(fun_u_scale, &scale, pSet);   }
LPFuzzySet MemberFunctionProperties::u_log  (const LPFuzzySet& pSet)               { return ext_unary(fun_u_log,             pSet); }


std::string FuzzySet::getAsString() const
{
	if (empty())
		return "[empty value]";

	std::string res = "";
	FuzzySetDefinition::const_iterator it = begin();
	while (it != end())
	{
		const bool output = it->second > 0.0;
		if (output)
			res += rdo::format("<%s/%.2lf>", it->first.getAsString().c_str(), it->second);

		++it;

		if (output && it != end())
			res += " ";
	}
	return res;
}

LPFuzzySet FuzzySet::clone() const
{
	return rdo::Factory<FuzzySet>::create(LPFuzzySet(const_cast<FuzzySet*>(this)));
}
// --------------------------------------------------------------------------------
// -------------------- MemberFunctionProperties
// --------------------------------------------------------------------------------

LPFuzzySet MemberFunctionProperties::a_mult(const LPFuzzySet& pSet1, const LPFuzzySet& pSet2)
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);

	// Найдем только пересекающие элементы и перемножим функции принадлежности
	FuzzySet::FuzzySetDefinition::const_iterator it1 = pSet1->begin();
	for (it1 = pSet1->begin(); it1 != pSet1->end(); ++it1)
	{
		FuzzySet::FuzzySetDefinition::const_iterator it2 = pSet2->find(it1->first);
		if (it2 != pSet2->end())
		{
			pFuzzySetResult->operator[](it1->first) = it1->second * it2->second;
		}
	}
	return pFuzzySetResult;
}

LPFuzzySet MemberFunctionProperties::alpha(const LPFuzzySet& pSet,double appertain)
{
	if (appertain < 0) appertain = 0;
	else if (appertain > 1) appertain = 1;

	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create();
	ASSERT(pFuzzySetResult);

	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();

	// Найдем отсечку
	while (it != pSet->end())
	{
		if (it->second >= appertain)
		{
			pFuzzySetResult->operator[](it->first) = pSet->operator[](it->first);
		}
		++it;
	}
	return pFuzzySetResult;
}

LPFuzzySet MemberFunctionProperties::supplement(const LPFuzzySet& pSet)
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create(pSet);
	ASSERT(pFuzzySetResult);

	FuzzySet::FuzzySetDefinition::iterator it = pFuzzySetResult->begin();
	while (it != pFuzzySetResult->end())
	{
		it->second = 1-(it->second);
		++it;
	}
	return(pFuzzySetResult);
}

LPFuzzySet MemberFunctionProperties::a_pow(LPFuzzySet pSet, double power)
{
	LPFuzzySet pFuzzySetResult = rdo::Factory<FuzzySet>::create(pSet);
	ASSERT(pFuzzySetResult);

	// Возведём в степень
	FuzzySet::FuzzySetDefinition::iterator it = pFuzzySetResult->begin();
	while (it != pFuzzySetResult->end())
	{
		it->second = pow(it->second, power); ///flag
		++it;
	}
	return pFuzzySetResult;
}

LPRDOLingvoVariable MemberFunctionProperties::fuzzyfication(const RDOValue&value, const LPRDOLingvoVariable& variable)
{
	LPRDOLingvoVariable pVariable = rdo::Factory<RDOLingvoVariable>::create(value, variable);
	for (RDOLingvoVariable::TermSet::const_iterator it = variable->begin(); it != variable->end(); ++it)
	{
		LPFuzzySet pSet = rdo::Factory<FuzzySet>::create(it->second);
		FuzzySet::FuzzyItem item = pSet->findValue(value);
		LPFuzzySet newSet = rdo::Factory<FuzzySet>::create();
		newSet->append(item.first, item.second);
		pVariable->append(it->first, newSet);
	}
	return pVariable;
}

RDOValue MemberFunctionProperties::defuzzyfication(const LPFuzzySet& pSet)
{
	FuzzySet::FuzzySetDefinition::const_iterator it = pSet->begin();
	if (it == pSet->end())
		return RDOValue();

	FuzzySet::FuzzySetDefinition::const_iterator it_next = it;
	++it_next;
	if (it_next == pSet->end())
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

		++it;
		++it_next;
		if (it_next == pSet->end())
			break;

		x      = x_next;
		a      = b;
		x_next = it_next->first.getDouble();
		b      = it_next->second;
		h      = x_next - x;
	}
	return g / f;
}

// --------------------------------------------------------------------------------
// -------------------- RDOLingvoVariable
// --------------------------------------------------------------------------------
CLOSE_RDO_RUNTIME_NAMESPACE
