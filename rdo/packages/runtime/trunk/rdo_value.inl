/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_value.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.07.2008
  \brief     RDOValue
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <string.h>
#include <stdlib.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdodebug.h"
#include "utils/static_assert.h"
#include "simulator/runtime/rdo_enum.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
inline RDOValue::RDOValue()
	: m_pType(g_unknow)
{
	setUndefined(true);
}

inline RDOValue::~RDOValue()
{
	deleteValue();
}

inline RDOValue::RDOValue(CREF(RDOValue) rdovalue)
{
	set(rdovalue);
}

inline RDOValue::RDOValue(CREF(LPRDOType) pType)
	: m_pType(pType)
{
	switch (typeID())
	{
	case RDOType::t_unknow        : break;
	case RDOType::t_int           : __get<int>   () = 0; break;
	case RDOType::t_real          : __get<double>() = 0; break;
	case RDOType::t_enum          : __get<ruint> () = 0; break;
	case RDOType::t_bool          : __get<rbool> () = false; break;
	case RDOType::t_string        : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(_T(""))); break;
	case RDOType::t_identificator : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(_T(""))); break;
	default                       : throw RDOValueException();
	}
	setUndefined(true);
}

inline RDOValue::RDOValue(rsint value)
	: m_pType(g_int)
{
	__get<int>() = value;
	setUndefined(true);
}

inline RDOValue::RDOValue(ruint value)
	: m_pType(g_int)
{
	__get<ruint>() = value;
	setUndefined(true);
}

#ifdef ARCHITECTURE_AMD64
inline RDOValue::RDOValue(ruint64 value)
	: m_pType(g_int)
{
	__get<ruint64>() = value;
}
#endif // ARCHITECTURES_AMD64

inline RDOValue::RDOValue(double value)
	: m_pType(g_real)
{
	__get<double>() = value;
	setUndefined(true);
}

inline RDOValue::RDOValue(rbool value)
	: m_pType(g_bool)
{
	__get<rbool>() = value;
	setUndefined(true);
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum)
	: m_pType(pEnum)
{
	if (pEnum->empty())
		RDOValueException();

	__get<ruint>() = pEnum->findEnum(pEnum->getValues()[0]);
	setUndefined(true);
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, CREF(tstring) value)
	: m_pType(pEnum)
{
	__get<ruint>() = pEnum->findEnum(value);
	if (__get<ruint>() == RDOEnumType::END)
		RDOValueException();
	setUndefined(true);
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, ruint index)
	: m_pType(pEnum)
{
	if (index == RDOEnumType::END || index >= pEnum->getValues().size())
		RDOValueException();

	__get<ruint>() = index;
	setUndefined(true);
}

inline RDOValue::RDOValue(CREF(tstring) value)
	: m_pType(g_string)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>) >= sizeof(double));

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(true);
}

inline RDOValue::RDOValue(CPTR(tchar) value)
	: m_pType(g_string)
{
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(true);
}

inline RDOValue::RDOValue(CREF(tstring) value, CREF(LPRDOType) pType)
	: m_pType(g_identificator)
{
	if (pType->typeID() != RDOType::t_identificator)
		RDOValueException();

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(true);
}

template <class T>
inline RDOValue::RDOValue(CREF(LPRDOType) pType, CREF(rdo::intrusive_ptr<T>) pObject)
	: m_pType(pType)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<T>) >= sizeof(double));

	ASSERT(m_pType);
	ASSERT(
		typeID() == RDOType::t_string        ||
		typeID() == RDOType::t_identificator ||
		typeID() == RDOType::t_pointer
	);
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<T>(pObject);
	setUndefined(true);
}

inline void RDOValue::deleteValue()
{
	switch (typeID())
	{
	case RDOType::t_string       :
	case RDOType::t_identificator:
	case RDOType::t_pointer      :
		reinterpret_cast<rdo::LPIRefCounter>(&m_value)->release();
		break;
	default:
		break;
	}
}

inline rsint RDOValue::getInt() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int    : return __get<int>  ();
	case RDOType::t_real   : return (rsint)__get<double>();
	case RDOType::t_enum   : return __get<ruint>();
	case RDOType::t_bool   : return __get<rbool>() ? 1 : 0;
	case RDOType::t_pointer: return onPointerGetInt();
	default                : break;
	}
	throw RDOValueException();
}

inline ruint RDOValue::getUInt() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int    : return __get<ruint>();
	case RDOType::t_real   : return (ruint)__get<double>();
	case RDOType::t_enum   : return __get<ruint>();
	case RDOType::t_bool   : return __get<rbool>() ? 1 : 0;
	case RDOType::t_pointer: return onPointerGetUInt();
	default                : break;
	}
	throw RDOValueException();
}

inline rsint RDOValue::getEnumAsInt() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int : return __get<int>  ();
	case RDOType::t_real: return (rsint)__get<double>();
	case RDOType::t_enum: return __get<ruint>();
	case RDOType::t_bool: return __get<rbool>() ? 1 : 0;
	default             : break;
	}
	throw RDOValueException();
}

inline LPRDOEnumType RDOValue::getEnum() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_enum: return __enumT();
	default             : break;
	}
	throw RDOValueException();
}

inline double RDOValue::getDouble() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int : return __get<int>   ();
	case RDOType::t_real: return __get<double>();
	case RDOType::t_enum: return __get<ruint> ();
	case RDOType::t_bool: return __get<rbool> () ? 1 : 0;
	default             : break;
	}
	throw RDOValueException();
}

inline rbool RDOValue::getBool() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool: return __get<rbool>();
	default             : break;
	}
	throw RDOValueException();
}

inline rbool RDOValue::getAsBool() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int   : return __get<int>   ()      ? true : false;
	case RDOType::t_real  : return __get<double>()      ? true : false;
	case RDOType::t_enum  : return __get<ruint> ()      ? true : false;
	case RDOType::t_string: return !__stringV().empty() ? true : false;
	case RDOType::t_bool  : return __get<rbool> ();
	default               : break;
	}
	throw RDOValueException();
}

inline CREF(tstring) RDOValue::getString() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_string: return __stringV();
	default               : break;
	}
	throw RDOValueException();
}

inline CREF(tstring) RDOValue::getIdentificator() const
{
	if (!m_undefined) 
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_identificator: return __stringV();
	default                      : break;
	}
	throw RDOValueException();
}

inline tstring RDOValue::getAsString() const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int          : return rdo::format(_T("%d"), __get<int>());
	case RDOType::t_real         : return rdo::toString(__get<double>());
	case RDOType::t_enum         : return __enumT()->getValues().at(__get<ruint>());
	case RDOType::t_bool         : return __get<rbool>() ? _T("true") : _T("false");
	case RDOType::t_string       : return __stringV();
	case RDOType::t_identificator: return __stringV();
	case RDOType::t_pointer      : return onPointerAsString();
	default                      : break;
	}
	throw RDOValueException(_T("Для rdo::runtime::RDOValue не определен метод getAsString()"));
}

inline tstring RDOValue::getAsStringForTrace() const
{
	if (!m_undefined)
		return _T("#");

	switch (typeID())
	{
	case RDOType::t_int    : return rdo::format(_T("%d"), __get<int>());
	case RDOType::t_real   : return rdo::toString(__get<double>());
	case RDOType::t_enum   : return rdo::format(_T("%d"), __get<ruint>());
	case RDOType::t_bool   : return __get<rbool>() ? _T("true") : _T("false");
	case RDOType::t_string : return __stringV();
	case RDOType::t_pointer: return onPointerAsString();
	default                : break;
	}
	throw RDOValueException(_T("Для rdo::runtime::RDOValue не определен метод getAsStringForTrace()"));
}

inline void RDOValue::set(CREF(RDOValue) rdovalue)
{
	m_pType     = rdovalue.m_pType;
	m_undefined = rdovalue.m_undefined;
	switch (typeID())
	{
	case RDOType::t_string       :
	case RDOType::t_identificator:
	case RDOType::t_pointer      :
		{
			memcpy(&m_value, &rdovalue.m_value, sizeof(m_value));
			reinterpret_cast<rdo::LPIRefCounter>(&m_value)->addref();
			break;
		}
	default:
		{
			memcpy(&m_value, &rdovalue.m_value, sizeof(m_value));
			break;
		}
	}
}

inline REF(RDOValue) RDOValue::operator= (CREF(RDOValue) rdovalue)
{
	deleteValue();
	set(rdovalue);
	return *this;
}

inline rbool RDOValue::operator== (CREF(RDOValue) rdovalue) const
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return __get<int>() == rdovalue.__get<int>();
			case RDOType::t_real: return __get<int>() == rdovalue.__get<double>();
			default             : break;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return __get<double>()  == rdovalue.__get<int>();
			case RDOType::t_real: return __get<double>()  == rdovalue.__get<double>();
			default             : break;
			}
			break;
		}
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<rbool>() == rdovalue.__get<rbool>();
			default             : break;
			}
			break;
		}
	case RDOType::t_enum:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_enum: if (m_pType == rdovalue.m_pType) return __get<ruint>() == rdovalue.__get<ruint>(); break;
			default             : break;
			}
			break;
		}
	case RDOType::t_string:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_string: return __stringV() == rdovalue.__stringV();
			default               : break;
			}
			break;
		}
	case RDOType::t_identificator:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_identificator: return __stringV() == rdovalue.__stringV();
			default                      : break;
			}
			break;
		}
	case RDOType::t_pointer:
		{
			return onPointerEqual(rdovalue);
		}
	default:
		break;
	}
	throw RDOValueException();
}

inline rbool RDOValue::operator!= (CREF(RDOValue) rdovalue) const
{
	return !operator==(rdovalue);
}

inline rbool RDOValue::operator< (CREF(RDOValue) rdovalue) const
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return __get<int>() < rdovalue.__get<int>();
			case RDOType::t_real: return __get<int>() < rdovalue.__get<double>();
			default             : break;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return __get<double>() < rdovalue.__get<int>();
			case RDOType::t_real: return __get<double>() < rdovalue.__get<double>();
			default             : break;
			}
			break;
		}
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<rbool>() < rdovalue.__get<rbool>();
			default             : break;
			}
			break;
		}
	case RDOType::t_string:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_string: return __stringV() < rdovalue.__stringV();
			default               : break;
			}
			break;
		}
	default:
		break;
	}
	throw RDOValueException();
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
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<rbool>() && rdovalue.__get<rbool>();
			default             : break;
			}
			break;
		}
	case RDOType::t_pointer: 
		{
			return onPointerAnd(rdovalue);
		}
	default:
		break;
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator|| (CREF(RDOValue) rdovalue) const
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<rbool>() || rdovalue.__get<rbool>();
			default             : break;
			}
			break;
		}
	case RDOType::t_pointer:
		{
			return onPointerAnd(rdovalue);
		}
	default:
		break;
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::operator- () const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int    : return RDOValue(-__get<int>   ());
	case RDOType::t_real   : return RDOValue(-__get<double>());
	case RDOType::t_bool   : return RDOValue(!__get<rbool> ());
	case RDOType::t_pointer: return onPointerUMinus();
	default                : throw RDOValueException();
	}
}

inline rbool RDOValue::operator! () const
{
	if (!m_undefined)
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int : return !__get<int>   (); break;
	case RDOType::t_real: return !__get<double>(); break;
	case RDOType::t_bool: return !__get<rbool> (); break;
	default             : throw RDOValueException();
	}
}

inline REF(RDOValue) RDOValue::operator+= (CREF(RDOValue) rdovalue)
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<int>() += rdovalue.getInt(); return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() += rdovalue.getDouble(); return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_string:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_string:
				{
					rdo::LPIRefCounter pIRefCountrer = reinterpret_cast<rdo::LPIRefCounter>(&m_value);
					ASSERT(pIRefCountrer);

					if (!pIRefCountrer->owner())
					{
						pIRefCountrer->release();
						new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(__stringV()));
					}

					__stringV() += rdovalue.__stringV();
					return *this;
				}
			default: break;
			}
			break;
		}
	case RDOType::t_pointer:
		{
			return onPointerPlus(rdovalue);
		}
	default:
		break;
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
	UNUSED(inc);

	if (!m_undefined)
		throw RDOUndefinedException();

	RDOValue prevValue(typeID() == RDOType::t_pointer ? clone() : *this);
	operator+=(1);
	return prevValue;
}

inline CREF(RDOValue) RDOValue::operator--()
{
	if (!m_undefined)
		throw RDOUndefinedException();

	operator-=(1);
	return *this;
}

inline RDOValue RDOValue::operator--(int inc)
{
	UNUSED(inc);

	if (!m_undefined)
		throw RDOUndefinedException();

	RDOValue prevValue(typeID() == RDOType::t_pointer ? clone() : *this);
	operator-=(1);
	return prevValue;
}

inline REF(RDOValue) RDOValue::operator-= (CREF(RDOValue) rdovalue)
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<int>() -= rdovalue.getInt(); return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() -= rdovalue.getDouble(); return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_pointer:
		{
			return onPointerMinus(rdovalue);
		}
	default:
		break;
	}

	throw RDOValueException();
}

inline REF(RDOValue) RDOValue::operator*= (CREF(RDOValue) rdovalue)
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : __get<int>() *= rdovalue.__get<int>(); return *this;
			case RDOType::t_real: __get<double>() = ((double)__get<int>()) * rdovalue.__get<double>(); m_pType = g_real; return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() *= rdovalue.getDouble(); return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_pointer:
		{
			return onPointerMult(rdovalue);
		}
	default:
		break;
	}

	throw RDOValueException();
}

inline REF(RDOValue) RDOValue::operator/= (CREF(RDOValue) rdovalue)
{
	if (!m_undefined || !rdovalue.getUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() = ((double)__get<int>()) / rdovalue.getDouble(); m_pType = g_real; return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() = __get<double>() / rdovalue.getDouble(); return *this;
			default             : break;
			}
			break;
		}
	case RDOType::t_pointer:
		{
			return onPointerDiv(rdovalue);
		}
	default:
		break;
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

//inline RDOValue RDOValue::operator[] (CREF(RDOValue) rdovalue)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : return __arrayV ()->operator [](rdovalue);
//	case RDOType::t_matrix: return __matrixV()[rdovalue];
//	}
//	throw RDOValueException();
//}
//
//inline RDOValue RDOValue::begin()
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : return RDOValue(rdo::Factory<RDOArrayIterator>::create(__arrayV()->containerBegin()));
//	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerBegin()));
//	}
//	throw RDOValueException();
//}
//
//inline RDOValue RDOValue::end()
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : return RDOValue(rdo::Factory<RDOArrayIterator>::create( __arrayV()->containerEnd()));
//	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerEnd()));
//	}
//	throw RDOValueException();
//}
//
//inline void RDOValue::insert(CREF(RDOValue) itr, CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : __arrayV ()->insertItems(itr.__arrayItr()->getIterator(), itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
//	case RDOType::t_matrix: __matrixV().insertItems(itr.__matrixItr().getIterator(), itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
//	}
//	throw RDOValueException();
//}
//
//inline void RDOValue::erase(CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : __arrayV ()->eraseItems(itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
//	case RDOType::t_matrix: __matrixV().eraseItems(itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
//	}
//	throw RDOValueException();
//}
//
//inline void RDOValue::setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array :
//		{
//			__arrayV()->setArrayItem(ind, item);
//			return;
//		}
//	}
//	throw RDOValueException();
//}

inline void RDOValue::setUndefined(CREF(rbool) undefined)
{
	m_undefined = undefined;
}

inline CREF(rbool) RDOValue::getUndefined() const
{
	return m_undefined;
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
	return *getPointer<string_class>().get();
}

inline CREF(tstring) RDOValue::__stringV() const
{
	return *getPointer<string_class>().get();
}

inline REF(rdo::ostream) operator<< (REF(rdo::ostream) stream, CREF(RDOValue) rdovalue)
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

template <class T>
inline REF(rdo::intrusive_ptr<T>) RDOValue::getPointer()
{
	return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

template <class T>
inline CREF(rdo::intrusive_ptr<T>) RDOValue::getPointer() const
{
	return __get<rdo::intrusive_ptr_interface_wrapper<T> >();
}

template <class T>
inline CREF(rdo::intrusive_ptr<typename T::value_type>) RDOValue::getPointerSafety() const
{
#ifdef _DEBUG
	rdo::intrusive_ptr<T> pType = type().object_dynamic_cast<T>();
	ASSERT(pType);
#endif // _DEBUG

	return getPointer<typename T::value_type>();
}

template <class T>
inline CREF(rdo::intrusive_ptr<typename T::value_type>) RDOValue::getPointerByInterfaceSafety() const
{
#ifdef _DEBUG
	rdo::interface_ptr<T> pType = type().interface_dynamic_cast<T>();
	ASSERT(pType);
#endif // _DEBUG

	return getPointer<typename T::value_type>();
}

template <class T>
inline rbool RDOValue::isType() const
{
	return type().object_dynamic_cast<T>();
}

template <class T>
inline REF(T) RDOValue::__get()
{
	return *reinterpret_cast<PTR(T)>(&m_value);
}

template <class T>
inline CREF(T) RDOValue::__get() const
{
	return *reinterpret_cast<CPTR(T)>(&m_value);
}

// --------------------------------------------------------------------------------
// -------------------- RDOValue::string_class
// --------------------------------------------------------------------------------
inline RDOValue::string_class::string_class(CREF(tstring) string)
	: tstring(string)
{}

inline rdo::intrusive_ptr<RDOValue::string_class> RDOValue::string_class::clone() const
{
	return rdo::intrusive_ptr<string_class>(new string_class(c_str()));
}

CLOSE_RDO_RUNTIME_NAMESPACE
