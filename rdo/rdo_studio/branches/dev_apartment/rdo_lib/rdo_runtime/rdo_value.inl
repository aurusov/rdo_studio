/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_value.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.07.2008
  \brief     RDOValue
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdodebug.h"
#include "rdo_lib/rdo_runtime/rdo_enum.h"
#include "rdo_lib/rdo_runtime/rdo_fuzzy_def.h"
#include "rdo_lib/rdo_runtime/rdo_array_def.h"
#include "rdo_lib/rdo_runtime/rdo_matrix_def.h"
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
inline RDOValue::RDOValue()
	: m_pType(g_unknow)
{}

inline RDOValue::~RDOValue()
{
	deleteValue();
}

inline RDOValue::RDOValue(CREF(RDOValue) rdovalue)
	: m_pType(g_unknow)
{
	set(rdovalue);
}

inline RDOValue::RDOValue(CREF(LPRDOType) pType)
	: m_pType(pType)
{
	switch (typeID())
	{
	case RDOType::t_unknow        : break;
	case RDOType::t_int           : m_value.i_value = 0; break;
	case RDOType::t_real          : m_value.d_value = 0; break;
	case RDOType::t_enum          : m_value.i_value = 0; break;
	case RDOType::t_bool          : m_value.b_value = false; break;
	case RDOType::t_string        : m_value.s_value = new smart_string(new string_class(_T(""))); break;
	case RDOType::t_identificator : m_value.s_value = new smart_string(new string_class(_T(""))); break;
	case RDOType::t_array         : m_value.p_data  = new RDOArrayValue(pType.object_static_cast<RDOArrayType>()); break;
	case RDOType::t_arrayIterator : m_value.p_data  = new PTR(void); break;
	case RDOType::t_matrix        : m_value.p_data  = new PTR(void); break;
	case RDOType::t_matrixIterator: m_value.p_data  = new PTR(void); break;
	default                       : throw RDOValueException();
	}
}

inline RDOValue::RDOValue(rsint value)
	: m_pType(g_int)
{
	m_value.i_value = value;
}

inline RDOValue::RDOValue(ruint value)
	: m_pType(g_int)
{
	m_value.i_value = value;
}

inline RDOValue::RDOValue(double value)
	: m_pType(g_real)
{
	m_value.d_value = value;
}

inline RDOValue::RDOValue(rbool value)
	: m_pType(g_bool)
{
	m_value.b_value = value;
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum)
	: m_pType(pEnum)
{
	if (pEnum->empty())
		RDOValueException();

	m_value.i_value = pEnum->findEnum(pEnum->getValues()[0]);
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, CREF(tstring) value)
	: m_pType(pEnum)
{
	m_value.i_value = pEnum->findEnum(value);
	if (m_value.i_value == RDOEnumType::END)
		RDOValueException();
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, ruint index)
	: m_pType(pEnum)
{
	if (index == RDOEnumType::END || index >= pEnum->getValues().size())
		RDOValueException();

	m_value.i_value = index;
}

inline RDOValue::RDOValue(CREF(RDOFuzzyValue) fuzzy)
	: m_pType(fuzzy.type())
{
	m_value.p_data = new RDOFuzzyValue(fuzzy);
}

inline RDOValue::RDOValue(CREF(tstring) value)
	: m_pType(g_string)
{
	m_value.s_value = new smart_string(new string_class(value));
}

inline RDOValue::RDOValue(CPTR(tchar) value)
	: m_pType(g_string)
{
	m_value.s_value = new smart_string(new string_class(value));
}

inline RDOValue::RDOValue(CREF(tstring) value, CREF(LPRDOType) pType)
	: m_pType(g_identificator)
{
	if (pType->typeID() != RDOType::t_identificator)
		RDOValueException();

	m_value.s_value = new smart_string(new string_class(value));
}

inline RDOValue::RDOValue(CREF(RDOArrayValue) arrayValue)
	: m_pType(arrayValue.type())
{
	m_value.p_data = new RDOArrayValue(arrayValue);
}

inline RDOValue::RDOValue(CREF(RDOArrayIterator) aIterator)
	: m_pType(g_arrayIterator)
{
	m_value.p_data = new RDOArrayIterator(aIterator);
}

inline RDOValue::RDOValue(CREF(RDOMatrixValue) matrixValue)
	: m_pType(matrixValue.type())
{
	m_value.p_data = new RDOMatrixValue(matrixValue);
}

inline RDOValue::RDOValue(CREF(RDOMatrixIterator) mIterator)
	: m_pType(g_matrixIterator)
{
	m_value.p_data = new RDOMatrixIterator(mIterator);
}

inline void RDOValue::deleteValue()
{
	switch (typeID())
	{
	case RDOType::t_string       :
	case RDOType::t_identificator:
		deleteString();
		break;

	case RDOType::t_fuzzy:
		delete &__fuzzyV();
		break;

	case RDOType::t_array:
		delete &__arrayV();
		break;

	case RDOType::t_arrayIterator:
		delete &__arrayItr();
		break;

	case RDOType::t_matrixIterator:
		delete &__matrixItr();
		break;
	}
}

inline rsint RDOValue::getInt() const
{
	switch (typeID())
	{
	case RDOType::t_int  : return m_value.i_value;
	case RDOType::t_real : return (rsint)m_value.d_value;
	case RDOType::t_enum : return m_value.i_value;
	case RDOType::t_bool : return m_value.b_value ? 1 : 0;
	case RDOType::t_fuzzy: return const_cast<RDOValue*>(this)->__fuzzyV().defuzzyfication().getInt();
	}
	throw RDOValueException();
}

inline rsint RDOValue::getEnumAsInt() const
{
	switch (typeID())
	{
	case RDOType::t_int : return m_value.i_value;
	case RDOType::t_real: return (rsint)m_value.d_value;
	case RDOType::t_enum: return m_value.i_value;
	case RDOType::t_bool: return m_value.b_value ? 1 : 0;
	}
	throw RDOValueException();
}

inline LPRDOEnumType RDOValue::getEnum() const
{
	switch (typeID())
	{
	case RDOType::t_enum: return __enumT();
	}
	throw RDOValueException();
}

inline double RDOValue::getDouble() const
{
	switch (typeID())
	{
	case RDOType::t_int : return m_value.i_value;
	case RDOType::t_real: return m_value.d_value;
	case RDOType::t_enum: return m_value.i_value;
	case RDOType::t_bool: return m_value.b_value ? 1 : 0;
	}
	throw RDOValueException();
}

inline rbool RDOValue::getBool() const
{
	switch (typeID())
	{
	case RDOType::t_bool: return m_value.b_value;
	}
	throw RDOValueException();
}

inline rbool RDOValue::getAsBool() const
{
	switch (typeID())
	{
	case RDOType::t_int   : return m_value.i_value      ? true : false;
	case RDOType::t_real  : return m_value.d_value      ? true : false;
	case RDOType::t_enum  : return m_value.i_value      ? true : false;
	case RDOType::t_string: return !__stringV().empty() ? true : false;
	case RDOType::t_bool  : return m_value.b_value;
	}
	throw RDOValueException();
}

inline CREF(tstring) RDOValue::getString() const
{
	switch (typeID())
	{
	case RDOType::t_string: return __stringV();
	}
	throw RDOValueException();
}

inline CREF(tstring) RDOValue::getIdentificator() const
{
	switch (typeID())
	{
	case RDOType::t_identificator: return __stringV();
	}
	throw RDOValueException();
}

inline CREF(RDOArrayValue) RDOValue::getArray() const
{
	return *static_cast<CPTR(RDOArrayValue)>(m_value.p_data);
}

inline CREF(RDOMatrixValue) RDOValue::getMatrix() const
{
	return *static_cast<CPTR(RDOMatrixValue)>(m_value.p_data);
}

inline tstring RDOValue::getAsString() const
{
	switch (typeID())
	{
	case RDOType::t_int          : return rdo::format(_T("%d"), m_value.i_value);
	case RDOType::t_real         : return rdo::toString(m_value.d_value);
	case RDOType::t_enum         : return __enumT()->getValues().at(m_value.i_value);
	case RDOType::t_bool         : return m_value.b_value ? _T("true") : _T("false");
	case RDOType::t_string       : return __stringV();
	case RDOType::t_identificator: return __stringV();
	case RDOType::t_fuzzy        : return __fuzzyV().getAsString();
	case RDOType::t_array         : return __arrayV().getAsString();
	case RDOType::t_arrayIterator : return __arrayItr().getValue().getAsString();
	case RDOType::t_matrix        : return __matrixV().getAsString();
	case RDOType::t_matrixIterator: return __matrixItr().getValue().getAsString();
	}
	throw RDOValueException(_T("Для rdoRuntime::RDOValue неопределен метод getAsString()"));
}

inline tstring RDOValue::getAsStringForTrace() const
{
	switch (typeID())
	{
	case RDOType::t_int   : return rdo::format(_T("%d"), m_value.i_value);
	case RDOType::t_real  : return rdo::toString(m_value.d_value);
	case RDOType::t_enum  : return rdo::format(_T("%d"), m_value.i_value);
	case RDOType::t_bool  : return m_value.b_value ? _T("true") : _T("false");
	case RDOType::t_string: return __stringV();
	case RDOType::t_array : return __arrayV().getAsString();
	case RDOType::t_matrix: return __matrixV().getAsString();
	}
	throw RDOValueException(_T("Для rdoRuntime::RDOValue неопределен метод getAsStringForTrace()"));
}

inline void RDOValue::deleteString()
{
	ASSERT(m_value.s_value);
	if (m_value.s_value->owner())
	{
		delete m_value.s_value;
	}
	else
	{
		m_value.s_value->release();
	}
}

inline void RDOValue::set(CREF(RDOValue) rdovalue)
{
	deleteValue();

	m_pType = rdovalue.m_pType;
	switch (typeID())
	{
	case RDOType::t_string       :
	case RDOType::t_identificator:
		{
			//! Заменяем указатель, а не вызываем rdo::smart_ptr operator=
			m_value.s_value = rdovalue.m_value.s_value;
			m_value.s_value->addref();
			break;
		}
	case RDOType::t_fuzzy:
		{
			m_value.p_data = new RDOFuzzyValue(rdovalue.__fuzzyV());
			break;
		}
	case RDOType::t_array:
		{
			m_value.p_data = new RDOArrayValue(rdovalue.__arrayV());
			break;
		}
	case RDOType::t_arrayIterator:
		{
			m_value.p_data = new RDOArrayIterator(rdovalue.__arrayItr());
			break;
		}
	case RDOType::t_matrix:
			{
				m_value.p_data = new RDOMatrixValue(rdovalue.__matrixV());
				break;
			}
	case RDOType::t_matrixIterator:
			{
				m_value.p_data = new RDOMatrixIterator(rdovalue.__matrixItr());
				break;
			}
	default:
		{
			m_value = rdovalue.m_value;
			break;
		}
	}
}

inline REF(RDOValue) RDOValue::operator= (CREF(RDOValue) rdovalue)
{
	set(rdovalue);
	return *this;
}

inline rbool RDOValue::operator== (CREF(RDOValue) rdovalue) const
{
	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return m_value.i_value == rdovalue.m_value.i_value;
			case RDOType::t_real: return m_value.i_value == rdovalue.m_value.d_value;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return m_value.d_value  == rdovalue.m_value.i_value;
			case RDOType::t_real: return m_value.d_value  == rdovalue.m_value.d_value;
			}
			break;
		}
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return m_value.b_value == rdovalue.m_value.b_value;
			}
			break;
		}
	case RDOType::t_enum:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_enum: if (m_pType == rdovalue.m_pType) return m_value.i_value == rdovalue.m_value.i_value; break;
			}
			break;
		}
	case RDOType::t_string:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_string: return __stringV() == rdovalue.__stringV();
			}
			break;
		}
	case RDOType::t_identificator:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_identificator: return __stringV() == rdovalue.__stringV();
			}
			break;
		}
	case RDOType::t_arrayIterator:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_arrayIterator: return __arrayItr() == rdovalue.__arrayItr();
			}
			break;
		}
	case RDOType::t_matrixIterator:
			{
				switch (rdovalue.typeID())
				{
			case RDOType::t_matrixIterator: return __matrixItr() == rdovalue.__matrixItr();
				}
				break;
			}
	}
	throw RDOValueException();
}

inline rbool RDOValue::operator!= (CREF(RDOValue) rdovalue) const
{
	return !operator==(rdovalue);
}

inline rbool RDOValue::operator< (CREF(RDOValue) rdovalue) const
{
	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return m_value.i_value < rdovalue.m_value.i_value;
			case RDOType::t_real: return m_value.i_value < rdovalue.m_value.d_value;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return m_value.d_value < rdovalue.m_value.i_value;
			case RDOType::t_real: return m_value.d_value < rdovalue.m_value.d_value;
			}
			break;
		}
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return m_value.b_value < rdovalue.m_value.b_value;
			}
			break;
		}
	case RDOType::t_string:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_string: return __stringV() < rdovalue.__stringV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline rbool RDOValue::operator< (CREF(RDOValue) rdovalue)
{
	return static_cast<CPTR(RDOValue)>(this)->operator< (rdovalue);
}

inline rbool RDOValue::operator> (CREF(RDOValue) rdovalue) const
{
	return !operator<=(rdovalue);
}

inline rbool RDOValue::operator<= (CREF(RDOValue) rdovalue) const
{
	return operator<(rdovalue) || operator==(rdovalue);
}

inline rbool RDOValue::operator>= (CREF(RDOValue) rdovalue) const
{
	return operator>(rdovalue) || operator==(rdovalue);
}

inline RDOValue RDOValue::operator&& (CREF(RDOValue) rdovalue) const
{
	switch (typeID())
	{
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return m_value.b_value && rdovalue.m_value.b_value;
			}
			break;
		}
	case RDOType::t_fuzzy:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_fuzzy: return __fuzzyV() && rdovalue.__fuzzyV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator|| (CREF(RDOValue) rdovalue) const
{
	switch (typeID())
	{
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return m_value.b_value || rdovalue.m_value.b_value;
			}
			break;
		}
	case RDOType::t_fuzzy:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_fuzzy: return __fuzzyV() || rdovalue.__fuzzyV();
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator- () const
{
	RDOValue rdovalue(*this);
	switch (typeID())
	{
	case RDOType::t_int  : rdovalue.m_value.i_value = -m_value.i_value; break;
	case RDOType::t_real : rdovalue.m_value.d_value = -m_value.d_value; break;
	case RDOType::t_bool : rdovalue.m_value.b_value = !m_value.b_value; break;
	case RDOType::t_fuzzy: rdovalue.__fuzzyV()      = __fuzzyV().u_minus(); break;
	default              : throw RDOValueException();
	}
	return rdovalue;
}

inline rbool RDOValue::operator! () const
{
	switch (typeID())
	{
	case RDOType::t_int : return !m_value.i_value; break;
	case RDOType::t_real: return !m_value.d_value; break;
	case RDOType::t_bool: return !m_value.b_value; break;
	default             : throw RDOValueException();
	}
}

inline void RDOValue::operator+= (CREF(RDOValue) rdovalue)
{
	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.i_value += rdovalue.getInt(); return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.d_value += rdovalue.getDouble(); return;
			}
			break;
		}
	case RDOType::t_string:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_string:
				{
					ASSERT(m_value.s_value);
					if (!m_value.s_value->owner())
					{
						m_value.s_value->release();
						m_value.s_value = new smart_string(new string_class(__stringV()));
					}
					__stringV() += rdovalue.__stringV();
					return;
				}
			}
			break;
		}
	case RDOType::t_fuzzy:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() + rdovalue.__fuzzyV(); return;
			}
			break;
		}
	case RDOType::t_arrayIterator:
		{
			switch(rdovalue.typeID())
			{
			case RDOType::t_int:
				PTR(RDOArrayIterator) pPrevIt = &__arrayItr();
				m_value.p_data = new RDOArrayIterator(__arrayItr() + rdovalue.getInt());
				delete pPrevIt;
				return;
			}
			break;
		}
	case RDOType::t_matrixIterator:
		{
			switch(rdovalue.typeID())
			{
			case RDOType::t_int:
				PTR(RDOMatrixIterator) pPrevIt = &__matrixItr();
				m_value.p_data = new RDOMatrixIterator(__matrixItr() + rdovalue.getInt());
				delete pPrevIt;
				return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline CREF(RDOValue) RDOValue::operator++()
{
	operator+=(1);
	return *this;
}

inline RDOValue RDOValue::operator++(int inc)
{
	RDOValue prevValue(*this);
	operator+=(1);
	return prevValue;
}

inline CREF(RDOValue) RDOValue::operator--()
{
	operator-=(1);
	return *this;
}

inline RDOValue RDOValue::operator--(int inc)
{
	RDOValue prevValue(*this);
	operator-=(1);
	return prevValue;
}

inline void RDOValue::operator-= (CREF(RDOValue) rdovalue)
{
	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.i_value -= rdovalue.getInt(); return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.d_value -= rdovalue.getDouble(); return;
			}
			break;
		}
	case RDOType::t_fuzzy:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() - rdovalue.__fuzzyV(); return;
			}
			break;
		}
	case RDOType::t_arrayIterator:
		{
			switch(rdovalue.typeID())
			{
			case RDOType::t_int:
				PTR(RDOArrayIterator) pPrevIt = &__arrayItr();
				m_value.p_data = new RDOArrayIterator(__arrayItr() - rdovalue.getInt());
				delete pPrevIt;
				return;
			}
			break;
		}
	case RDOType::t_matrixIterator:
		{
			switch(rdovalue.typeID())
			{
			case RDOType::t_int:
				PTR(RDOMatrixIterator) pPrevIt = &__matrixItr();
				m_value.p_data = new RDOMatrixIterator(__matrixItr() - rdovalue.getInt());
				delete pPrevIt;
				return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline void RDOValue::operator*= (CREF(RDOValue) rdovalue)
{
	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : m_value.i_value *= rdovalue.m_value.i_value; return;
			case RDOType::t_real: m_value.d_value = ((double)m_value.i_value) * rdovalue.m_value.d_value; m_pType = g_real; return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.d_value *= rdovalue.getDouble(); return;
			}
			break;
		}
	case RDOType::t_fuzzy:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() * rdovalue.__fuzzyV(); return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline void RDOValue::operator/= (CREF(RDOValue) rdovalue)
{
	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.d_value = ((double)m_value.i_value) / rdovalue.getDouble(); m_pType = g_real; return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: m_value.d_value = m_value.d_value / rdovalue.getDouble(); return;
			}
			break;
		}
	case RDOType::t_fuzzy:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_fuzzy: __fuzzyV() = __fuzzyV() / rdovalue.__fuzzyV(); return;
			}
			break;
		}
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator+ (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 += rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator- (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 -= rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator* (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 *= rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator/ (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 /= rdovalue;
	return value2;
}

inline RDOValue RDOValue::operator[] (CREF(RDOValue) rdovalue)
{
	switch (typeID())
	{
	case RDOType::t_array : return __arrayV ()[rdovalue];
	case RDOType::t_matrix: return __matrixV()[rdovalue];
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::begin()
{
	switch (typeID())
	{
	case RDOType::t_array : return RDOValue(RDOArrayIterator (__arrayV ().containerBegin()));
	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerBegin()));
	}
	throw RDOValueException();	
}

inline RDOValue RDOValue::end()
{
	switch (typeID())
	{
	case RDOType::t_array : return RDOValue(RDOArrayIterator ( __arrayV().containerEnd()));
	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerEnd()));
	}
	throw RDOValueException();
}

inline void RDOValue::insert(CREF(RDOValue) itr, CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
{
	switch (typeID())
	{
	case RDOType::t_array : __arrayV ().insertItems(itr.__arrayItr( ).getIterator(), itrFst.__arrayItr ().getIterator(), itrLst.__arrayItr ().getIterator()); return;
	case RDOType::t_matrix: __matrixV().insertItems(itr.__matrixItr().getIterator(), itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
	}
	throw RDOValueException();	
}

inline void RDOValue::erase(CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
{
	switch (typeID())
	{
	case RDOType::t_array : __arrayV ().eraseItems(itrFst.__arrayItr ().getIterator(), itrLst.__arrayItr ().getIterator()); return;
	case RDOType::t_matrix: __matrixV().eraseItems(itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
	}
	throw RDOValueException();
}

inline void RDOValue::setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item)
{
	switch (typeID())
	{
	case RDOType::t_array :
		{
			__arrayV().setArrayItem(ind, item);
			return;
		}
	}
	throw RDOValueException();
}

inline CREF(LPRDOType) RDOValue::type() const
{
	return m_pType;
}

inline RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->typeID();
}

inline LPRDOEnumType RDOValue::__enumT() const
{
	return m_pType.object_static_cast<RDOEnumType>();
}

inline REF(tstring) RDOValue::__stringV()
{
	return *m_value.s_value->get();
}

inline CREF(tstring) RDOValue::__stringV() const
{
	return *m_value.s_value->get();
}

inline REF(RDOFuzzyValue) RDOValue::__fuzzyV()
{
	return *static_cast<RDOFuzzyValue*>(m_value.p_data);
}

inline CREF(RDOFuzzyValue) RDOValue::__fuzzyV() const
{
	return *static_cast<CPTR(RDOFuzzyValue)>(m_value.p_data);
}

inline REF(RDOArrayValue) RDOValue::__arrayV()
{
	return *static_cast<PTR(RDOArrayValue)>(m_value.p_data);
}

inline CREF(RDOArrayValue) RDOValue::__arrayV() const
{
	return *static_cast<CPTR(RDOArrayValue)>(m_value.p_data);
}

inline REF(RDOArrayIterator) RDOValue::__arrayItr()
{
	return *static_cast<RDOArrayIterator*>(m_value.p_data);
}

inline CREF(RDOArrayIterator) RDOValue::__arrayItr() const
{
	return *static_cast<CPTR(RDOArrayIterator)>(m_value.p_data);
}

inline REF(RDOMatrixValue) RDOValue::__matrixV()
{
	return *static_cast<RDOMatrixValue*>(m_value.p_data);
}

inline CREF(RDOMatrixValue) RDOValue::__matrixV() const
{
	return *static_cast<CPTR(RDOMatrixValue)>(m_value.p_data);
}

inline REF(RDOMatrixIterator) RDOValue::__matrixItr()
{
	return *static_cast<RDOMatrixIterator*>(m_value.p_data);
}

inline CREF(RDOMatrixIterator) RDOValue::__matrixItr() const
{
	return *static_cast<CPTR(RDOMatrixIterator)>(m_value.p_data);
}

inline REF(std::ostream) operator<< (REF(std::ostream) stream, CREF(RDOValue) rdovalue)
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

// --------------------------------------------------------------------------------
// -------------------- RDOValue::string_class
// --------------------------------------------------------------------------------
inline RDOValue::string_class::string_class(CREF(tstring) string)
	: tstring(string)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOValue::smart_string
// --------------------------------------------------------------------------------
inline RDOValue::smart_string::smart_string(PTR(string_class) pString)
	: parent_type(pString)
{}

inline PTR(RDOValue::string_class) RDOValue::smart_string::get()
{
	return parent_type::get();
}

inline CPTR(RDOValue::string_class) RDOValue::smart_string::get() const
{
	return parent_type::get();
}

inline void RDOValue::smart_string::addref()
{
	parent_type::addref();
}

inline void RDOValue::smart_string::release()
{
	parent_type::release();
}

inline rbool RDOValue::smart_string::owner()
{
	return parent_type::owner();
}

CLOSE_RDO_RUNTIME_NAMESPACE
