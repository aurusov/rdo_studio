/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_value.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

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
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
	if (pThisArrayIterator)
	{
		LPRDOArrayIterator pIterator = pThisArrayIterator->clone();
		ASSERT(pIterator);
		return RDOValue(pIterator, pIterator);
	}

	LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
	if (pThisMatrixIterator)
	{
		LPRDOMatrixIterator pIterator = pThisMatrixIterator->clone();
		ASSERT(pIterator);
		return RDOValue(pIterator, pIterator);
	}

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyValue pCloneValue = pThisValue->clone();
		ASSERT(pCloneValue);

		return RDOValue(pCloneValue->type(), pCloneValue);
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод clone()"));
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

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pValue = getPointer<RDOFuzzyValue>();
		ASSERT(pValue);
		return pValue->getAsString();
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод getAsString()"));
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

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerEqual()"));
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

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
		if (pValueFuzzyType)
		{
			pThisValue->operator+ (rdovalue.getPointer<RDOFuzzyValue>());
			return *this;
		}
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerPlus()"));
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

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
		if (pValueFuzzyType)
		{
			pThisValue->operator- (rdovalue.getPointer<RDOFuzzyValue>());
			return *this;
		}
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerMinus()"));
}

REF(RDOValue) RDOValue::onPointerMult(CREF(RDOValue) rdovalue)
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
		if (pValueFuzzyType)
		{
			pThisValue->operator* (rdovalue.getPointer<RDOFuzzyValue>());
			return *this;
		}
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerMult()"));
}

REF(RDOValue) RDOValue::onPointerDiv(CREF(RDOValue) rdovalue)
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
		if (pValueFuzzyType)
		{
			pThisValue->operator/ (rdovalue.getPointer<RDOFuzzyValue>());
			return *this;
		}
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerMult()"));
}

rsint RDOValue::onPointerGetInt() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);
		return pThisValue->defuzzyfication().getInt();
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerGetInt()"));
}

ruint RDOValue::onPointerGetUInt() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);
		return (ruint)pThisValue->defuzzyfication().getInt();
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerGetUInt()"));
}

rbool RDOValue::onPointerAnd(CREF(RDOValue) rdovalue) const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
		if (pValueFuzzyType)
		{
			return pThisValue->operator&& (rdovalue.getPointer<RDOFuzzyValue>());
		}
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerAnd()"));
}

rbool RDOValue::onPointerOr(CREF(RDOValue) rdovalue) const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pThisValue = getPointer<RDOFuzzyValue>();
		ASSERT(pThisValue);

		LPRDOFuzzyType pValueFuzzyType = rdovalue.type().object_dynamic_cast<RDOFuzzyType>();
		if (pValueFuzzyType)
		{
			return pThisValue->operator|| (rdovalue.getPointer<RDOFuzzyValue>());
		}
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerOr()"));
}

RDOValue RDOValue::onPointerUMinus() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	if (pThisFuzzyType)
	{
		LPRDOFuzzyValue pCloneValue = clone().getPointer<RDOFuzzyValue>();
		ASSERT(pCloneValue);
		return RDOValue(pCloneValue->type(), pCloneValue->u_minus());
	}

	throw RDOValueException(_T("Для rdoRuntime::RDOValue не определен метод onPointerUMinus()"));
}

CLOSE_RDO_RUNTIME_NAMESPACE
