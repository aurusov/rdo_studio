/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_value.inl
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_enum.h"
#include "rdo_fuzzy_def.h"
#include "rdo_exception.h"
#include <rdodebug.h>
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
inline RDOValue::RDOValue()
	: m_type(&g_unknow)
{}

inline RDOValue::~RDOValue()
{
	switch (typeID())
	{
		case RDOType::t_string       :
		case RDOType::t_identificator:
		{
			deleteString();
			break;
		}
		case RDOType::t_fuzzy:
		{
			delete &__fuzzyV();
			break;
		}
	}
}

inline RDOValue::RDOValue(CREF(RDOValue) rdovalue)
	: m_type(&g_unknow)
{
	set(rdovalue);
}

inline RDOValue::RDOValue(CREF(RDOType) type)
	: m_type(&type)
{
	switch (typeID())
	{
		case RDOType::t_unknow       : break;
		case RDOType::t_int          : m_value.i_value = 0; break;
		case RDOType::t_real         : m_value.d_value = 0; break;
		case RDOType::t_enum         : m_value.i_value = 0; break;
		case RDOType::t_bool         : m_value.b_value = false; break;
		case RDOType::t_string       : m_value.s_value = new smart_tstring(new tstring(_T(""))); break;
		case RDOType::t_identificator: m_value.s_value = new smart_tstring(new tstring(_T(""))); break;
		default                      : throw RDOValueException();
	}
}

inline RDOValue::RDOValue(rsint value)
	: m_type(&g_int)
{
	m_value.i_value = value;
}

inline RDOValue::RDOValue(ruint value)
	: m_type(&g_int)
{
	m_value.i_value = value;
}

inline RDOValue::RDOValue(double value)
	: m_type(&g_real)
{
	m_value.d_value = value;
}

inline RDOValue::RDOValue(rbool value)
	: m_type(&g_bool)
{
	m_value.b_value = value;
}

inline RDOValue::RDOValue(REF(RDOEnumType) enums)
	: m_type(&enums)
{
	if (enums.empty())
		RDOValueException();

	m_value.i_value = enums.findEnum( enums.getValues()[0] );
}

inline RDOValue::RDOValue(CREF(RDOEnumType) enums, CREF(tstring) value)
	: m_type(&enums)
{
	m_value.i_value = enums.findEnum(value);
	if (m_value.i_value == RDOEnumType::END)
		RDOValueException();
}

inline RDOValue::RDOValue(CREF(RDOEnumType) enums, ruint index)
	: m_type(&enums)
{
	if (index == RDOEnumType::END || index >= enums.getValues().size())
		RDOValueException();

	m_value.i_value = index;
}

inline RDOValue::RDOValue(CREF(RDOFuzzyValue) fuzzy)
	: m_type(&fuzzy.type())
{
	m_value.p_data = new RDOFuzzyValue(fuzzy);
}

inline RDOValue::RDOValue(CREF(tstring) value)
	: m_type(&g_string)
{
	m_value.s_value = new smart_tstring(new tstring(value));
}

inline RDOValue::RDOValue(CPTR(tchar) value)
	: m_type(&g_string)
{
	m_value.s_value = new smart_tstring(new tstring(value));
}

inline RDOValue::RDOValue(CREF(tstring) value, CREF(RDOType) type)
	: m_type(&g_identificator)
{
	if (type.typeID() != RDOType::t_identificator)
		RDOValueException();

	m_value.s_value = new smart_tstring(new tstring(value));
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

inline CREF(RDOEnumType) RDOValue::getEnum() const
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

inline tstring RDOValue::getAsString() const
{
	switch (typeID())
	{
		case RDOType::t_int          : return rdo::format(_T("%d"), m_value.i_value);
		case RDOType::t_real         : return rdo::toString(m_value.d_value);
		case RDOType::t_enum         : return __enumT().getValues().at(m_value.i_value);
		case RDOType::t_bool         : return m_value.b_value ? _T("true") : _T("false");
		case RDOType::t_string       : return __stringV();
		case RDOType::t_identificator: return __stringV();
		case RDOType::t_fuzzy        : return __fuzzyV().getAsString();
	}
	throw RDOValueException();
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
	}
	throw RDOValueException();
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
	switch (typeID())
	{
		case RDOType::t_string       :
		case RDOType::t_identificator:
		{
			deleteString();
			break;
		}
	}
	m_type = rdovalue.m_type;
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
				case RDOType::t_enum: if (m_type == rdovalue.m_type) return m_value.i_value == rdovalue.m_value.i_value; break;
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
	}
	throw RDOValueException();
}

inline rbool RDOValue::operator!= (CREF(RDOValue) rdovalue) const
{
	return !operator==(rdovalue);
}

inline rbool RDOValue::operator< (CREF(RDOValue) rdovalue) const
{
	switch (typeID()) {
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
						m_value.s_value = new smart_tstring(new tstring(__stringV()));
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
	}
	throw RDOValueException();
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
				case RDOType::t_real: m_value.d_value = ((double)m_value.i_value) * rdovalue.m_value.d_value; m_type = &g_real; return;
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
				case RDOType::t_real: m_value.d_value = ((double)m_value.i_value) / rdovalue.getDouble(); m_type = &g_real; return;
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

inline CREF(RDOType) RDOValue::type() const
{
	return *m_type;
}

inline RDOType::TypeID RDOValue::typeID() const
{
	return type().typeID();
}

inline CREF(RDOEnumType) RDOValue::__enumT() const
{
	return *static_cast<CPTR(RDOEnumType)>(m_type);
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

inline REF(std::ostream) operator<< (REF(std::ostream) stream, CREF(RDOValue) rdovalue)
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

CLOSE_RDO_RUNTIME_NAMESPACE
