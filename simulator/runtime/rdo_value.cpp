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

RDOValue::RDOValue(const RDOValue& rdovalue)
{
	set(rdovalue);
}

RDOValue::RDOValue(const LPRDOType& pType)
	: m_pType(pType)
{
	switch (typeID())
	{
	case RDOType::t_unknow        : break;
	case RDOType::t_int           : __get<int>   () = 0; break;
	case RDOType::t_real          : __get<double>() = 0; break;
	case RDOType::t_enum          : __get<std::size_t> () = 0; break;
	case RDOType::t_bool          : __get<bool> () = false; break;
	case RDOType::t_string        : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class("")); break;
	case RDOType::t_identificator : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class("")); break;
	default                       : throw RDOValueException();
	}
	setUndefined(false);
}

RDOValue::RDOValue(int value)
	: m_pType(g_int)
{
	__get<int>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(std::size_t value)
	: m_pType(g_int)
{
        __get<std::size_t>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(double value)
	: m_pType(g_real)
{
	__get<double>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(bool value)
	: m_pType(g_bool)
{
	__get<bool>() = value;
	setUndefined(false);
}

RDOValue::RDOValue(const LPRDOEnumType& pEnum)
	: m_pType(pEnum)
{
	if (pEnum->empty())
		RDOValueException();

	__get<std::size_t>() = pEnum->findEnum(pEnum->getValues()[0]);
	setUndefined(false);
}

RDOValue::RDOValue(const LPRDOEnumType& pEnum, const std::string& value)
	: m_pType(pEnum)
{
	__get<std::size_t>() = pEnum->findEnum(value);
	if (__get<std::size_t>() == RDOEnumType::END)
		RDOValueException();
	setUndefined(false);
}

RDOValue::RDOValue(const LPRDOEnumType& pEnum, std::size_t index)
	: m_pType(pEnum)
{
	if (index == RDOEnumType::END || index >= pEnum->getValues().size())
		RDOValueException();

	__get<std::size_t>() = index;
	setUndefined(false);
}

RDOValue::RDOValue(const std::string& value)
	: m_pType(g_string)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>) >= sizeof(double));

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(false);
}

RDOValue::RDOValue(const char* value)
	: m_pType(g_string)
{
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
	setUndefined(false);
}

RDOValue::RDOValue(const std::string& value, const LPRDOType& pType)
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

RDOValue RDOValue::fromDouble(const LPRDOType& pType, double value)
{
	RDOValue result(pType);

	switch (result.typeID())
	{
	case RDOType::t_unknow: break;
	case RDOType::t_int   : result.__get<int>   () = int(value); break;
	case RDOType::t_real  : result.__get<double>() = value; break;
	case RDOType::t_enum  : result.__get<std::size_t> () = std::size_t(value); break;
	case RDOType::t_bool  : result.__get<bool> () = value != 0.0; break;
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

const LPRDOType& RDOValue::type() const
{
	return m_pType;
}

RDOType::TypeID RDOValue::typeID() const
{
	return m_pType->typeID();
}

std::ostream& operator<< (std::ostream& stream, const RDOValue& rdovalue)
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
}

int RDOValue::getInt() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int    : return __get<int>();
	case RDOType::t_real   : return (int)__get<double>();
	case RDOType::t_enum   : return __get<std::size_t>();
	case RDOType::t_bool   : return __get<bool>() ? 1 : 0;
	case RDOType::t_pointer: return onPointerGetInt();
	default                : break;
	}
	throw RDOValueException();
}

uint32_t RDOValue::getUInt() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int    : return __get<uint32_t>();
	case RDOType::t_real   : return (uint32_t)__get<double>();
	case RDOType::t_enum   : return __get<std::size_t>();
	case RDOType::t_bool   : return __get<bool>() ? 1 : 0;
	case RDOType::t_pointer: return onPointerGetUInt();
	default                : break;
	}
	throw RDOValueException();
}

int RDOValue::getEnumAsInt() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int : return __get<int>();
	case RDOType::t_real: return (int)__get<double>();
	case RDOType::t_enum: return __get<std::size_t>();
	case RDOType::t_bool: return __get<bool>() ? 1 : 0;
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
	case RDOType::t_int : return __get<int>();
	case RDOType::t_real: return __get<double>();
	case RDOType::t_enum: return __get<std::size_t>();
	case RDOType::t_bool: return __get<bool>() ? 1 : 0;
	default             : break;
	}
	throw RDOValueException();
}

bool RDOValue::getBool() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool: return __get<bool>();
	default             : break;
	}
	throw RDOValueException();
}

bool RDOValue::getAsBool() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int   : return __get<int>() ? true : false;
	case RDOType::t_real  : return __get<double>() ? true : false;
	case RDOType::t_enum  : return __get<std::size_t>() ? true : false;
	case RDOType::t_string: return !__stringV().empty() ? true : false;
	case RDOType::t_bool  : return __get<bool> ();
	default               : break;
	}
	throw RDOValueException();
}

const std::string& RDOValue::getString() const
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

const std::string& RDOValue::getIdentificator() const
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

std::string RDOValue::getAsString() const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int          : return rdo::format("%d", __get<int>());
	case RDOType::t_real         : return rdo::toString(__get<double>());
	case RDOType::t_enum         : return __enumT()->getValues().at(__get<std::size_t>());
	case RDOType::t_bool         : return __get<bool>() ? "true" : "false";
	case RDOType::t_string       : return __stringV();
	case RDOType::t_identificator: return __stringV();
	case RDOType::t_pointer      : return onPointerAsString();
	default                      : break;
	}
	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод getAsString()");
}

std::string RDOValue::getAsStringForTrace() const
{
	if (isUndefined())
		return "#";

	switch (typeID())
	{
	case RDOType::t_int    : return rdo::format("%d", __get<int>());
	case RDOType::t_real   : return rdo::toString(__get<double>());
	case RDOType::t_enum   : return rdo::format("%d", __get<std::size_t>());
	case RDOType::t_bool   : return __get<bool>() ? "true" : "false";
	case RDOType::t_string : return __stringV();
	case RDOType::t_pointer: return onPointerAsString();
	default                : break;
	}
	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод getAsStringForTrace()");
}

void RDOValue::set(const RDOValue& rdovalue)
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

RDOValue& RDOValue::operator= (const RDOValue& rdovalue)
{
	deleteValue();
	set(rdovalue);
	return *this;
}

bool RDOValue::operator== (const RDOValue& rdovalue) const
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
			case RDOType::t_bool: return __get<bool>() == rdovalue.__get<bool>();
			default             : break;
			}
			break;
		}
	case RDOType::t_enum:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_enum: if (m_pType == rdovalue.m_pType) return __get<std::size_t>() == rdovalue.__get<std::size_t>(); break;
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

bool RDOValue::operator!= (const RDOValue& rdovalue) const
{
	return !operator==(rdovalue);
}

bool RDOValue::operator< (const RDOValue& rdovalue) const
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
			case RDOType::t_bool: return __get<bool>() < rdovalue.__get<bool>();
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

bool RDOValue::operator> (const RDOValue& rdovalue) const
{
	return !operator<=(rdovalue);
}

bool RDOValue::operator<= (const RDOValue& rdovalue) const
{
	return operator<(rdovalue) || operator==(rdovalue);
}

bool RDOValue::operator>= (const RDOValue& rdovalue) const
{
	return operator>(rdovalue) || operator==(rdovalue);
}

RDOValue RDOValue::operator&& (const RDOValue& rdovalue) const
{
	if (isUndefined() || rdovalue.isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<bool>() && rdovalue.__get<bool>();
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

RDOValue RDOValue::operator|| (const RDOValue& rdovalue) const
{
	if (isUndefined() || rdovalue.isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<bool>() || rdovalue.__get<bool>();
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
	case RDOType::t_bool   : return RDOValue(!__get<bool> ());
	case RDOType::t_pointer: return onPointerUMinus();
	default                : throw RDOValueException();
	}
}

bool RDOValue::operator! () const
{
	if (isUndefined())
		throw RDOUndefinedException();

	switch (typeID())
	{
	case RDOType::t_int : return !__get<int>   (); break;
	case RDOType::t_real: return !__get<double>(); break;
	case RDOType::t_bool: return !__get<bool> (); break;
	default             : throw RDOValueException();
	}
}

RDOValue& RDOValue::operator+= (const RDOValue& rdovalue)
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

const RDOValue& RDOValue::operator++()
{
	operator+=(1);
	return *this;
}

RDOValue RDOValue::operator++(int /*inc*/)
{
	if (isUndefined())
		throw RDOUndefinedException();

	RDOValue prevValue(typeID() == RDOType::t_pointer ? clone() : *this);
	operator+=(1);
	return prevValue;
}

const RDOValue& RDOValue::operator--()
{
	if (isUndefined())
		throw RDOUndefinedException();

	operator-=(1);
	return *this;
}

RDOValue RDOValue::operator--(int /*inc*/)
{
	if (isUndefined())
		throw RDOUndefinedException();

	RDOValue prevValue(typeID() == RDOType::t_pointer ? clone() : *this);
	operator-=(1);
	return prevValue;
}

RDOValue& RDOValue::operator-= (const RDOValue& rdovalue)
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

RDOValue& RDOValue::operator*= (const RDOValue& rdovalue)
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

RDOValue& RDOValue::operator/= (const RDOValue& rdovalue)
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

RDOValue RDOValue::operator+ (const RDOValue& rdovalue) const
{
	RDOValue value2(*this);
	value2 += rdovalue;
	return value2;
}

RDOValue RDOValue::operator- (const RDOValue& rdovalue) const
{
	RDOValue value2(*this);
	value2 -= rdovalue;
	return value2;
}

RDOValue RDOValue::operator* (const RDOValue& rdovalue) const
{
	RDOValue value2(*this);
	value2 *= rdovalue;
	return value2;
}

RDOValue RDOValue::operator/ (const RDOValue& rdovalue) const
{
	RDOValue value2(*this);
	value2 /= rdovalue;
	return value2;
}

//RDOValue RDOValue::operator[] (const RDOValue& rdovalue)
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
//void RDOValue::insert(const RDOValue& itr, const RDOValue& itrFst, const RDOValue& itrLst)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : __arrayV ()->insertItems(itr.__arrayItr()->getIterator(), itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
//	case RDOType::t_matrix: __matrixV().insertItems(itr.__matrixItr().getIterator(), itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
//	}
//	throw RDOValueException();
//}
//
//void RDOValue::erase(const RDOValue& itrFst, const RDOValue& itrLst)
//{
//	switch (typeID())
//	{
//	case RDOType::t_array : __arrayV ()->eraseItems(itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
//	case RDOType::t_matrix: __matrixV().eraseItems(itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
//	}
//	throw RDOValueException();
//}
//
//void RDOValue::setArrayItem(const RDOValue& ind, const RDOValue& item)
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

void*& RDOValue::__voidPtrV()
{
	return __get<void*>();
}

const void*& RDOValue::__voidPtrV() const
{
	return *reinterpret_cast<const void**>(&const_cast<RDOValue*>(this)->m_value);
}

LPRDOEnumType RDOValue::__enumT() const
{
	return m_pType.object_static_cast<RDOEnumType>();
}

std::string& RDOValue::__stringV()
{
	return *getPointer<string_class>().get();
}

const std::string& RDOValue::__stringV() const
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

std::string RDOValue::onPointerAsString() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	LPRDOArrayType pThisArrayType = m_pType.object_dynamic_cast<RDOArrayType>();
	if (pThisArrayType)
	{
		LPRDOArrayValue pValue = getPointer<RDOArrayValue>();
		ASSERT(pValue);
		return pValue->asString();
	}

	LPRDOArrayIterator pThisArrayIterator = m_pType.object_dynamic_cast<RDOArrayIterator>();
	if (pThisArrayIterator)
	{
		return pThisArrayIterator->asString();
	}

	LPRDOMatrixType pThisMatrixType = m_pType.object_dynamic_cast<RDOMatrixType>();
	if (pThisMatrixType)
	{
		LPRDOMatrixValue pValue = getPointer<RDOMatrixValue>();
		ASSERT(pValue);
		return pValue->asString();
	}

	LPRDOMatrixIterator pThisMatrixIterator = m_pType.object_dynamic_cast<RDOMatrixIterator>();
	if (pThisMatrixIterator)
	{
		return pThisMatrixIterator->asString();
	}

	LPRDOResourceTypeList pThisResource = m_pType.object_dynamic_cast<RDOResourceTypeList>();
	if (pThisResource)
	{
		return "nested_resource";
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

bool RDOValue::onPointerEqual(const RDOValue& rdovalue) const
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

	LPRDOResourceTypeList pThisResourceType = m_pType.object_dynamic_cast<RDOResourceTypeList>();
	if (pThisResourceType)
	{
		LPRDOResource pThisResource = getPointerByType<RDOResourceTypeList>();
		LPRDOResource pOtherResource = rdovalue.getPointerByType<RDOResourceTypeList>();
		if (pThisResource && pOtherResource)
		{
			return pThisResource == pOtherResource;
		}
	}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerEqual()");
}

RDOValue& RDOValue::onPointerPlus(const RDOValue& rdovalue)
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

RDOValue& RDOValue::onPointerMinus(const RDOValue& rdovalue)
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

RDOValue& RDOValue::onPointerMult(const RDOValue& /*rdovalue*/)
{
	ASSERT(typeID() == RDOType::t_pointer);

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

RDOValue& RDOValue::onPointerDiv(const RDOValue& /*rdovalue*/)
{
	ASSERT(typeID() == RDOType::t_pointer);

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

int RDOValue::onPointerGetInt() const
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

uint32_t RDOValue::onPointerGetUInt() const
{
	ASSERT(typeID() == RDOType::t_pointer);

	//LPRDOFuzzyType pThisFuzzyType = m_pType.object_dynamic_cast<RDOFuzzyType>();
	//if (pThisFuzzyType)
	//{
	//	LPFuzzySet pThisValue = getPointer<FuzzySet>();
	//	ASSERT(pThisValue);
	//	return (uint32_t)MemberFunctionProperties::defuzzyfication(pThisValue).getInt();
	//}

	throw RDOValueException("Для rdo::runtime::RDOValue не определен метод onPointerGetUInt()");
}

bool RDOValue::onPointerAnd(const RDOValue& /*rdovalue*/) const
{
	ASSERT(typeID() == RDOType::t_pointer);

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

bool RDOValue::onPointerOr(const RDOValue& /*rdovalue*/) const
{
	ASSERT(typeID() == RDOType::t_pointer);

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

void RDOValue::setUndefined(bool undefined)
{
	m_undefined = undefined;
}

bool RDOValue::isUndefined() const
{
	return m_undefined;
}

// --------------------------------------------------------------------------------
// -------------------- RDOValue::string_class
// --------------------------------------------------------------------------------
RDOValue::string_class::string_class(const std::string& string)
	: std::string(string)
{}

rdo::intrusive_ptr<RDOValue::string_class> RDOValue::string_class::clone() const
{
	return rdo::intrusive_ptr<string_class>(new string_class(c_str()));
}

CLOSE_RDO_RUNTIME_NAMESPACE
