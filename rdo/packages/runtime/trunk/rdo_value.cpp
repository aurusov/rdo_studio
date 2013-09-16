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
#include "utils/src/debug/static_assert.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_enum.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_fuzzy.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
RDOValue::RDOValue()
	: m_pType(g_unknow)
{
	setUndefined(false);
}

RDOValue::RDOValue(CREF(RDOValue) rdovalue)
{
	set(rdovalue);
}

RDOValue::RDOValue(CREF(LPRDOType) pType)
	: m_pType(pType)
{
	switch (typeID())
	{
	case RDOType::t_unknow        : break;
	case RDOType::t_int           : __get<int>   () = 0; break;
	case RDOType::t_real          : __get<double>() = 0; break;
	case RDOType::t_enum          : __get<ruint> () = 0; break;
	case RDOType::t_bool          : __get<rbool> () = false; break;
	case RDOType::t_string        : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class("")); break;
	case RDOType::t_identificator : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class("")); break;
	default                       : throw RDOValueException();
	}
	setUndefined(false);
}

RDOValue::RDOValue(rsint value)
	: m_pType(g_int)
{
	__get<int>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(ruint value)
	: m_pType(g_int)
{
	__get<ruint>() = value;
	setUndefined(false);
}

#ifdef ARCHITECTURE_AMD64
RDOValue::RDOValue(ruint64 value)
	: m_pType(g_int)
{
	__get<ruint64>() = value;
}
#endif // ARCHITECTURES_AMD64

RDOValue::RDOValue(double value)
	: m_pType(g_real)
{
	__get<double>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(rbool value)
	: m_pType(g_bool)
{
	__get<rbool>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum)
	: m_pType(pEnum)
{
	if (pEnum->empty())
		RDOValueException();

	__get<ruint>() = pEnum->findEnum(pEnum->getValues()[0]);
	setUndefined(false);
}

RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, CREF(tstring) value)
	: m_pType(pEnum)
{
	__get<ruint>() = pEnum->findEnum(value);
	if (__get<ruint>() == RDOEnumType::END)
		RDOValueException();
	setUndefined(false);
}

RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, ruint index)
	: m_pType(pEnum)
{
	if (index == RDOEnumType::END || index >= pEnum->getValues().size())
		RDOValueException();

	__get<ruint>() = index;
	setUndefined(false);
}

RDOValue::RDOValue(CREF(tstring) value)
	: m_pType(g_string)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>) >= sizeof(double));

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(false);
}

RDOValue::RDOValue(CPTR(tchar) value)
	: m_pType(g_string)
{
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(false);
}

RDOValue::RDOValue(CREF(tstring) value, CREF(LPRDOType) pType)
	: m_pType(g_identificator)
{
	if (pType->typeID() != RDOType::t_identificator)
		RDOValueException();

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(false);
}

RDOValue::~RDOValue()
{
	deleteValue();
}

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
	result.setUndefined(false);

	return result;
}

void RDOValue::deleteValue()
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

rsint RDOValue::getInt() const
{
	if (isUndefined())
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

ruint RDOValue::getUInt() const
{
	if (isUndefined())
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

rsint RDOValue::getEnumAsInt() const
{
	if (isUndefined())
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

LPRDOEnumType RDOValue::getEnum() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_enum: return __enumT();
	default             : break;
	}
	throw RDOValueException();
}

double RDOValue::getDouble() const
{
	if (isUndefined())
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

rbool RDOValue::getBool() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool: return __get<rbool>();
	default             : break;
	}
	throw RDOValueException();
}

rbool RDOValue::getAsBool() const
{
	if (isUndefined())
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

CREF(tstring) RDOValue::getString() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_string: return __stringV();
	default               : break;
	}
	throw RDOValueException();
}

CREF(tstring) RDOValue::getIdentificator() const
{
	if (isUndefined()) 
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_identificator: return __stringV();
	default                      : break;
	}
	throw RDOValueException();
}

tstring RDOValue::getAsString() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int          : return rdo::format("%d", __get<int>());
	case RDOType::t_real         : return rdo::toString(__get<double>());
	case RDOType::t_enum         : return __enumT()->getValues().at(__get<ruint>());
	case RDOType::t_bool         : return __get<rbool>() ? "true" : "false";
	case RDOType::t_string       : return __stringV();
	case RDOType::t_identificator: return __stringV();
	case RDOType::t_pointer      : return onPointerAsString();
	default                      : break;
	}
	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод getAsString()");
}

tstring RDOValue::getAsStringForTrace() const
{
	if (isUndefined())
		return "#";

	switch (typeID())
	{
	case RDOType::t_int    : return rdo::format("%d", __get<int>());
	case RDOType::t_real   : return rdo::toString(__get<double>());
	case RDOType::t_enum   : return rdo::format("%d", __get<ruint>());
	case RDOType::t_bool   : return __get<rbool>() ? "true" : "false";
	case RDOType::t_string : return __stringV();
	case RDOType::t_pointer: return onPointerAsString();
	default                : break;
	}
	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод getAsStringForTrace()");
}

void RDOValue::set(CREF(RDOValue) rdovalue)
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

REF(RDOValue) RDOValue::operator= (CREF(RDOValue) rdovalue)
{
	deleteValue();
	set(rdovalue);
	return *this;
}

rbool RDOValue::operator== (CREF(RDOValue) rdovalue) const
{
	if (isUndefined() || rdovalue.isUndefined())
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

rbool RDOValue::operator!= (CREF(RDOValue) rdovalue) const
{
	return !operator==(rdovalue);
}

rbool RDOValue::operator< (CREF(RDOValue) rdovalue) const
{
	if (isUndefined() || rdovalue.isUndefined())
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

rbool RDOValue::operator> (CREF(RDOValue) rdovalue) const
{
	return !operator<=(rdovalue);
}

rbool RDOValue::operator<= (CREF(RDOValue) rdovalue) const
{
	return operator<(rdovalue) || operator==(rdovalue);
}

rbool RDOValue::operator>= (CREF(RDOValue) rdovalue) const
{
	return operator>(rdovalue) || operator==(rdovalue);
}

RDOValue RDOValue::operator&& (CREF(RDOValue) rdovalue) const
{
	if (isUndefined() || rdovalue.isUndefined())
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

RDOValue RDOValue::operator|| (CREF(RDOValue) rdovalue) const
{
	if (isUndefined() || rdovalue.isUndefined())
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

RDOValue RDOValue::operator- () const
{
	if (isUndefined())
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

rbool RDOValue::operator! () const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int : return !__get<int>   (); break;
	case RDOType::t_real: return !__get<double>(); break;
	case RDOType::t_bool: return !__get<rbool> (); break;
	default             : throw RDOValueException();
	}
}

REF(RDOValue) RDOValue::operator+= (CREF(RDOValue) rdovalue)
{
	if (isUndefined() || rdovalue.isUndefined())
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

CREF(RDOValue) RDOValue::operator++()
{
	operator+=(1);
	return *this;
}

RDOValue RDOValue::operator++(int inc)
{
	UNUSED(inc);

	if (isUndefined())
		throw RDOUndefinedException();

	RDOValue prevValue(typeID() == RDOType::t_pointer ? clone() : *this);
	operator+=(1);
	return prevValue;
}

CREF(RDOValue) RDOValue::operator--()
{
	if (isUndefined())
		throw RDOUndefinedException();

	operator-=(1);
	return *this;
}

RDOValue RDOValue::operator--(int inc)
{
	UNUSED(inc);

	if (isUndefined())
		throw RDOUndefinedException();

	RDOValue prevValue(typeID() == RDOType::t_pointer ? clone() : *this);
	operator-=(1);
	return prevValue;
}

REF(RDOValue) RDOValue::operator-= (CREF(RDOValue) rdovalue)
{
	if (isUndefined() || rdovalue.isUndefined())
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

REF(RDOValue) RDOValue::operator*= (CREF(RDOValue) rdovalue)
{
	if (isUndefined() || rdovalue.isUndefined())
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

REF(RDOValue) RDOValue::operator/= (CREF(RDOValue) rdovalue)
{
	if (isUndefined() || rdovalue.isUndefined())
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

RDOValue RDOValue::operator+ (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 += rdovalue;
	return value2;
}

RDOValue RDOValue::operator- (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 -= rdovalue;
	return value2;
}

RDOValue RDOValue::operator* (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 *= rdovalue;
	return value2;
}

RDOValue RDOValue::operator/ (CREF(RDOValue) rdovalue) const
{
	RDOValue value2(*this);
	value2 /= rdovalue;
	return value2;
}

//RDOValue RDOValue::operator[] (CREF(RDOValue) rdovalue)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : return __arrayV ()->operator [](rdovalue);
//	case RDOType::t_matrix: return __matrixV()[rdovalue];
//	}
//	throw RDOValueException();
//}
//
//RDOValue RDOValue::begin()
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : return RDOValue(rdo::Factory<RDOArrayIterator>::create(__arrayV()->containerBegin()));
//	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerBegin()));
//	}
//	throw RDOValueException();
//}
//
//RDOValue RDOValue::end()
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : return RDOValue(rdo::Factory<RDOArrayIterator>::create( __arrayV()->containerEnd()));
//	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerEnd()));
//	}
//	throw RDOValueException();
//}
//
//void RDOValue::insert(CREF(RDOValue) itr, CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : __arrayV ()->insertItems(itr.__arrayItr()->getIterator(), itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
//	case RDOType::t_matrix: __matrixV().insertItems(itr.__matrixItr().getIterator(), itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
//	}
//	throw RDOValueException();
//}
//
//void RDOValue::erase(CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : __arrayV ()->eraseItems(itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
//	case RDOType::t_matrix: __matrixV().eraseItems(itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
//	}
//	throw RDOValueException();
//}
//
//void RDOValue::setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item)
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

REF(PTR(void)) RDOValue::__voidPtrV()
{
	return __get<PTR(void)>();
}

CREF(PTR(void)) RDOValue::__voidPtrV() const
{
	return *reinterpret_cast<CPTR(PTR(void))>(&const_cast<PTR(RDOValue)>(this)->m_value);
}

LPRDOEnumType RDOValue::__enumT() const
{
	return m_pType.object_static_cast<RDOEnumType>();
}

REF(tstring) RDOValue::__stringV()
{
	return *getPointer<string_class>().get();
}

CREF(tstring) RDOValue::__stringV() const
{
	return *getPointer<string_class>().get();
}

RDOValue RDOValue::clone() const
{
	bool undefined = isUndefined();
	const_cast<RDOValue*>(this)->setUndefined(false);

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

// --------------------------------------------------------------------------------
// -------------------- RDOValue::string_class
// --------------------------------------------------------------------------------
RDOValue::string_class::string_class(CREF(tstring) string)
	: tstring(string)
{}

rdo::intrusive_ptr<RDOValue::string_class> RDOValue::string_class::clone() const
{
	return rdo::intrusive_ptr<string_class>(new string_class(c_str()));
}

CLOSE_RDO_RUNTIME_NAMESPACE
