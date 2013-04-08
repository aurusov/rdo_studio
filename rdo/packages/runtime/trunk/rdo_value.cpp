/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_value.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

RDOValue RDOValue::fromDouble(CREF(LPRDOType) pType, double value)
{
	RDOValue result(pType);

	switch (result.typeID())
	{
	case RDOType::t_unknow: break;
	case RDOType::t_int   : result.__get<int>   () = int(value);    break;
	case RDOType::t_real  : result.__get<double>() = value;         break;
	case RDOType::t_enum  : result.__get<ruint> () = ruint(value);  break;
	case RDOType::t_bool  : result.__get<rbool> () = value != 0.0;  break;
	default               : throw RDOValueException();
	}
	result.setUndefined(true);

	return result;
}

REF(PTR(void)) RDOValue::__voidPtrV()
{
	return __get<PTR(void)>();
}

CREF(PTR(void)) RDOValue::__voidPtrV() const
{
	return *reinterpret_cast<CPTR(PTR(void))>(&const_cast<PTR(RDOValue)>(this)->m_value);
}

RDOValue RDOValue::clone() const
{
	bool undefined = isUndefined();
	const_cast<RDOValue*>(this)->setUndefined(true);

	RDOValue result;
	switch (typeID())
	{
	case RDOType::t_int          : result = RDOValue(getInt()); break;
	case RDOType::t_real         : result = RDOValue(getDouble()); break;
	case RDOType::t_enum         : result = RDOValue(getEnum(), getEnumAsInt()); break;
	case RDOType::t_bool         : result = RDOValue(getBool()); break;
	case RDOType::t_string       : result = RDOValue(getString()); break;
	case RDOType::t_identificator: result = RDOValue(getIdentificator(), g_identificator); break;
	case RDOType::t_pointer      :
		{
			LPRDOArrayType pThisArrayType = m_pType.object_dynamic_cast<RDOArrayType>();
			if (pThisArrayType)
			{
				LPRDOArrayValue pValue = getPointer<RDOArrayValue>();
				ASSERT(pValue);
				result = RDOValue(pThisArrayType, pValue->clone());
				break;
			}

			LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
			if (pThisArrayIterator)
			{
				LPRDOArrayIterator pIterator = pThisArrayIterator->clone();
				ASSERT(pIterator);
				result = RDOValue(pIterator, pIterator);
				break;
			}

			LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
			if (pThisMatrixIterator)
			{
				LPRDOMatrixIterator pIterator = pThisMatrixIterator->clone();
				ASSERT(pIterator);
				result = RDOValue(pIterator, pIterator);
				break;
			}
		}
		break;
	default: break;
	}

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPFuzzySet pCloneValue = pThisValue->clone();
	//	ASSERT(pCloneValue);

	//	return RDOValue(pCloneValue->type(), pCloneValue);
	//}

	if (result.typeID() == RDOType::t_unknow)
	{
		throw RDOValueException("Для rdo::runtime::RDOValue не определен метод clone()");
	}
	result.setUndefined(undefined);
	const_cast<RDOValue*>(this)->setUndefined(undefined);
	return result;
}

tstring RDOValue::onPointerAsString() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOArrayType pThisArrayType = m_pType.object_dynamic_cast<RDOArrayType>();
	if (pThisArrayType)
	{
		LPRDOArrayValue pValue = getPointer<RDOArrayValue>();
		ASSERT(pValue);
		return pValue->getAsString();
	}

	LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
	if (pThisArrayIterator)
	{
		return pThisArrayIterator->getValue().getAsString();
	}

	LPRDOMatrixType pThisMatrixType = m_pType.object_dynamic_cast<RDOMatrixType>();
	if (pThisMatrixType)
	{
		LPRDOMatrixValue pValue = getPointer<RDOMatrixValue>();
		ASSERT(pValue);
		return pValue->getAsString();
	}

	LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
	if (pThisMatrixIterator)
	{
		return pThisMatrixIterator->getValue().getAsString();
	}

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pValue = getPointer<FuzzySet>();
	//	ASSERT(pValue);
	//	return pValue->getAsString();
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод getAsString()");
}

rbool RDOValue::onPointerEqual(CREF(RDOValue) rdovalue) const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
	if (pThisArrayIterator)
	{
		LPRDOArrayIterator pValueArrayIterator = rdovalue.type().object_dynamic_cast<RDOArrayIterator>();
		if (pValueArrayIterator)
		{
			return pThisArrayIterator->equal(pValueArrayIterator);
		}
	}

	LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
	if (pThisMatrixIterator)
	{
		LPRDOMatrixIterator pValueMatrixIterator = rdovalue.type().object_dynamic_cast<RDOMatrixIterator>();
		if (pValueMatrixIterator)
		{
			return pThisMatrixIterator->equal(pValueMatrixIterator);
		}
	}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerEqual()");
}

REF(RDOValue) RDOValue::onPointerPlus(CREF(RDOValue) rdovalue)
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
	if (pThisArrayIterator)
	{
		if (rdovalue.typeID() == RDOType::t_int)
		{
			pThisArrayIterator->preInc(rdovalue.getInt());
			return *this;
		}
	}

	LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
	if (pThisMatrixIterator)
	{
		if (rdovalue.typeID() == RDOType::t_int)
		{
			pThisMatrixIterator->preInc(rdovalue.getInt());
			return *this;
		}
	}

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
	//	if (pValueFuzzyType)
	//	{
	//		pThisValue->operator+ (rdovalue.getPointer<FuzzySet>());
	//		return *this;
	//	}
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerPlus()");
}

REF(RDOValue) RDOValue::onPointerMinus(CREF(RDOValue) rdovalue)
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
	if (pThisArrayIterator)
	{
		if (rdovalue.typeID() == RDOType::t_int)
		{
			pThisArrayIterator->preInc(-rdovalue.getInt());
			return *this;
		}
	}

	LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
	if (pThisMatrixIterator)
	{
		if (rdovalue.typeID() == RDOType::t_int)
		{
			pThisMatrixIterator->preInc(-rdovalue.getInt());
			return *this;
		}
	}

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
	//	if (pValueFuzzyType)
	//	{
	//		pThisValue->operator -(rdovalue.getPointer<FuzzySet>());
	//		return *this;
	//	}
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerMinus()");
}

REF(RDOValue) RDOValue::onPointerMult(CREF(RDOValue) rdovalue)
{
	ASSERT(typeID() == RDOType::t_pointer);

	UNUSED(rdovalue);
	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
	//	if (pValueFuzzyType)
	//	{
	//		pThisValue->operator*(rdovalue.getPointer<FuzzySet>());
	//		return *this;
	//	}
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerMult()");
}

REF(RDOValue) RDOValue::onPointerDiv(CREF(RDOValue) rdovalue)
{
	ASSERT(typeID() == RDOType::t_pointer);

	UNUSED(rdovalue);
	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
	//	if (pValueFuzzyType)
	//	{
	//		pThisValue->operator/(rdovalue.getPointer<FuzzySet>());
	//		return *this;
	//	}
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerMult()");
}

rsint RDOValue::onPointerGetInt() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);
	//	return MemberFunctionProperties::defuzzyfication(pThisValue).getInt();
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerGetInt()");
}

ruint RDOValue::onPointerGetUInt() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);
	//	return (ruint)MemberFunctionProperties::defuzzyfication(pThisValue).getInt();
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerGetUInt()");
}

rbool RDOValue::onPointerAnd(CREF(RDOValue) rdovalue) const
{
	ASSERT(typeID() == RDOType::t_pointer);

	UNUSED(rdovalue);
	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
	//	if (pValueFuzzyType)
	//	{
	//		return pThisValue->operator&&(rdovalue.getPointer<FuzzySet>());
	//	}
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerAnd()");
}

rbool RDOValue::onPointerOr(CREF(RDOValue) rdovalue) const
{
	ASSERT(typeID() == RDOType::t_pointer);

	UNUSED(rdovalue);
	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);

	//	LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
	//	if (pValueFuzzyType)
	//	{
	//		return pThisValue->operator||(rdovalue.getPointer<FuzzySet>());
	//	}
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerOr()");
}

RDOValue RDOValue::onPointerUMinus() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pCloneValue = clone().getPointer<FuzzySet>();
	//	ASSERT(pCloneValue);
	//	return RDOValue(pCloneValue->type(), MemberFunctionProperties::u_minus(pCloneValue));
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerUMinus()");
}

void RDOValue::setUndefined(rbool undefined)
{
	m_undefined = undefined;
}

rbool RDOValue::isUndefined() const
{
	return m_undefined;
}

CLOSE_RDO_RUNTIME_NAMESPACE
