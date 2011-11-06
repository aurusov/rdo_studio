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
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdodebug.h"
#include "utils/static_assert.h"
#include "simulator/runtime/rdo_enum.h"
#include "simulator/runtime/rdo_fuzzy_def.h"
#include "simulator/runtime/rdo_array_def.h"
#include "simulator/runtime/rdo_matrix_def.h"
#include "simulator/runtime/rdo_exception.h"
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
	case RDOType::t_int           : __get<int>   () = 0; break;
	case RDOType::t_real          : __get<double>() = 0; break;
	case RDOType::t_enum          : __get<int>   () = 0; break;
	case RDOType::t_bool          : __get<rbool> () = false; break;
	case RDOType::t_string        : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(_T(""))); break;
	case RDOType::t_identificator : new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(_T(""))); break;
	case RDOType::t_array         : new (&m_value) rdo::intrusive_ptr_interface_wrapper<RDOArrayType>(rdo::Factory<RDOArrayType>::create(pType.object_static_cast<RDOArrayType>())); break;
	case RDOType::t_matrix        : __voidPtrV   () = new PTR(void); break;
	case RDOType::t_matrixIterator: __voidPtrV   () = new PTR(void); break;
	default                       : throw RDOValueException();
	}
}

inline RDOValue::RDOValue(rsint value)
	: m_pType(g_int)
{
	__get<int>() = value;
}

inline RDOValue::RDOValue(ruint value)
	: m_pType(g_int)
{
	__get<int>() = value;
}

inline RDOValue::RDOValue(double value)
	: m_pType(g_real)
{
	__get<double>() = value;
}

inline RDOValue::RDOValue(rbool value)
	: m_pType(g_bool)
{
	__get<rbool>() = value;
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum)
	: m_pType(pEnum)
{
	if (pEnum->empty())
		RDOValueException();

	__get<int>() = pEnum->findEnum(pEnum->getValues()[0]);
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, CREF(tstring) value)
	: m_pType(pEnum)
{
	__get<int>() = pEnum->findEnum(value);
	if (__get<int>() == RDOEnumType::END)
		RDOValueException();
}

inline RDOValue::RDOValue(CREF(LPRDOEnumType) pEnum, ruint index)
	: m_pType(pEnum)
{
	if (index == RDOEnumType::END || index >= pEnum->getValues().size())
		RDOValueException();

	__get<int>() = index;
}

inline RDOValue::RDOValue(CREF(RDOFuzzyValue) fuzzy)
	: m_pType(fuzzy.type())
{
	__voidPtrV() = new RDOFuzzyValue(fuzzy);
}

inline RDOValue::RDOValue(CREF(tstring) value)
	: m_pType(g_string)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>) >= sizeof(double));

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
}

inline RDOValue::RDOValue(CPTR(tchar) value)
	: m_pType(g_string)
{
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
}

inline RDOValue::RDOValue(CREF(tstring) value, CREF(LPRDOType) pType)
	: m_pType(g_identificator)
{
	if (pType->typeID() != RDOType::t_identificator)
		RDOValueException();

	new (&m_value) rdo::intrusive_ptr_interface_wrapper<string_class>(new string_class(value));
}

inline RDOValue::RDOValue(CREF(LPRDOArrayIterator) pIterator)
	: m_pType(g_arrayIterator)
{
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<RDOArrayIterator>(pIterator);
}

inline RDOValue::RDOValue(CREF(RDOMatrixValue) matrixValue)
	: m_pType(matrixValue.type())
{
	__voidPtrV() = new RDOMatrixValue(matrixValue);
}

inline RDOValue::RDOValue(CREF(RDOMatrixIterator) mIterator)
	: m_pType(g_matrixIterator)
{
	__voidPtrV() = new RDOMatrixIterator(mIterator);
}

template <class T>
inline RDOValue::RDOValue(CREF(LPRDOType) pType, CREF(rdo::intrusive_ptr<T>) pObject)
	: m_pType(pType)
{
	STATIC_ASSERT(sizeof(rdo::intrusive_ptr_interface_wrapper<T>) >= sizeof(double));

	ASSERT(m_pType);
	ASSERT(
		typeID() == rdoRuntime::RDOType::t_string        ||
		typeID() == rdoRuntime::RDOType::t_identificator ||
		typeID() == rdoRuntime::RDOType::t_array         ||
		typeID() == rdoRuntime::RDOType::t_arrayIterator ||
		typeID() == rdoRuntime::RDOType::t_pointer
	);
	new (&m_value) rdo::intrusive_ptr_interface_wrapper<T>(pObject);
}

inline void RDOValue::deleteValue()
{
	switch (typeID())
	{
	case RDOType::t_string       :
	case RDOType::t_identificator:
	case RDOType::t_array        :
	case RDOType::t_arrayIterator:
	case RDOType::t_pointer      :
		reinterpret_cast<rdo::LPIRefCounter>(&m_value)->release();
		break;

	case RDOType::t_fuzzy:
		delete &__fuzzyV();
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
	case RDOType::t_int  : return __get<int>  ();
	case RDOType::t_real : return (rsint)__get<double>();
	case RDOType::t_enum : return __get<int>  ();
	case RDOType::t_bool : return __get<rbool>() ? 1 : 0;
	case RDOType::t_fuzzy: return const_cast<PTR(RDOValue)>(this)->__fuzzyV().defuzzyfication().getInt();
	}
	throw RDOValueException();
}

inline ruint RDOValue::getUInt() const
{
	switch (typeID())
	{
	case RDOType::t_int  : return __get<ruint>();
	case RDOType::t_real : return (ruint)__get<double>();
	case RDOType::t_enum : return __get<ruint>();
	case RDOType::t_bool : return __get<rbool>() ? 1 : 0;
	case RDOType::t_fuzzy: return (ruint)const_cast<PTR(RDOValue)>(this)->__fuzzyV().defuzzyfication().getInt();
	}
	throw RDOValueException();
}

inline rsint RDOValue::getEnumAsInt() const
{
	switch (typeID())
	{
	case RDOType::t_int : return __get<int>  ();
	case RDOType::t_real: return (rsint)__get<double>();
	case RDOType::t_enum: return __get<int>  ();
	case RDOType::t_bool: return __get<rbool>() ? 1 : 0;
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
	case RDOType::t_int : return __get<int>   ();
	case RDOType::t_real: return __get<double>();
	case RDOType::t_enum: return __get<int>   ();
	case RDOType::t_bool: return __get<rbool> () ? 1 : 0;
	}
	throw RDOValueException();
}

inline rbool RDOValue::getBool() const
{
	switch (typeID())
	{
	case RDOType::t_bool: return __get<rbool>();
	}
	throw RDOValueException();
}

inline rbool RDOValue::getAsBool() const
{
	switch (typeID())
	{
	case RDOType::t_int   : return __get<int>   ()      ? true : false;
	case RDOType::t_real  : return __get<double>()      ? true : false;
	case RDOType::t_enum  : return __get<int>   ()      ? true : false;
	case RDOType::t_string: return !__stringV().empty() ? true : false;
	case RDOType::t_bool  : return __get<rbool> ();
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
	return *static_cast<CPTR(RDOArrayValue)>(__voidPtrV());
}

inline CREF(RDOMatrixValue) RDOValue::getMatrix() const
{
	return *static_cast<CPTR(RDOMatrixValue)>(__voidPtrV());
}

inline tstring RDOValue::getAsString() const
{
	switch (typeID())
	{
	case RDOType::t_int           : return rdo::format(_T("%d"), __get<int>());
	case RDOType::t_real          : return rdo::toString(__get<double>());
	case RDOType::t_enum          : return __enumT()->getValues().at(__get<int>());
	case RDOType::t_bool          : return __get<rbool>() ? _T("true") : _T("false");
	case RDOType::t_string        : return __stringV();
	case RDOType::t_identificator : return __stringV();
	case RDOType::t_fuzzy         : return __fuzzyV().getAsString();
	case RDOType::t_array         : return __arrayV()->getAsString();
	case RDOType::t_arrayIterator : return __arrayItr()->getValue().getAsString();
	case RDOType::t_matrix        : return __matrixV().getAsString();
	case RDOType::t_matrixIterator: return __matrixItr().getValue().getAsString();
	}
	throw RDOValueException(_T("Для rdoRuntime::RDOValue неопределен метод getAsString()"));
}

inline tstring RDOValue::getAsStringForTrace() const
{
	switch (typeID())
	{
	case RDOType::t_int   : return rdo::format(_T("%d"), __get<int>());
	case RDOType::t_real  : return rdo::toString(__get<double>());
	case RDOType::t_enum  : return rdo::format(_T("%d"), __get<int>());
	case RDOType::t_bool  : return __get<rbool>() ? _T("true") : _T("false");
	case RDOType::t_string: return __stringV();
	case RDOType::t_array : return __arrayV ()->getAsString();
	case RDOType::t_matrix: return __matrixV().getAsString();
	}
	throw RDOValueException(_T("Для rdoRuntime::RDOValue неопределен метод getAsStringForTrace()"));
}

inline void RDOValue::set(CREF(RDOValue) rdovalue)
{
	deleteValue();

	m_pType = rdovalue.m_pType;
	switch (typeID())
	{
	case RDOType::t_string       :
	case RDOType::t_identificator:
	case RDOType::t_array        :
	case RDOType::t_arrayIterator:
	case RDOType::t_pointer      :
		{
			memcpy(&m_value, &rdovalue.m_value, sizeof(m_value));
			reinterpret_cast<rdo::LPIRefCounter>(&m_value)->addref();
			break;
		}
	case RDOType::t_fuzzy:
		{
			__voidPtrV() = new RDOFuzzyValue(rdovalue.__fuzzyV());
			break;
		}
	case RDOType::t_matrix:
			{
				__voidPtrV() = new RDOMatrixValue(rdovalue.__matrixV());
				break;
			}
	case RDOType::t_matrixIterator:
			{
				__voidPtrV() = new RDOMatrixIterator(rdovalue.__matrixItr());
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
			case RDOType::t_int : return __get<int>() == rdovalue.__get<int>();
			case RDOType::t_real: return __get<int>() == rdovalue.__get<double>();
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return __get<double>()  == rdovalue.__get<int>();
			case RDOType::t_real: return __get<double>()  == rdovalue.__get<double>();
			}
			break;
		}
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<rbool>() == rdovalue.__get<rbool>();
			}
			break;
		}
	case RDOType::t_enum:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_enum: if (m_pType == rdovalue.m_pType) return __get<int>() == rdovalue.__get<int>(); break;
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
			case RDOType::t_int : return __get<int>() < rdovalue.__get<int>();
			case RDOType::t_real: return __get<int>() < rdovalue.__get<double>();
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int : return __get<double>() < rdovalue.__get<int>();
			case RDOType::t_real: return __get<double>() < rdovalue.__get<double>();
			}
			break;
		}
	case RDOType::t_bool:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_bool: return __get<rbool>() < rdovalue.__get<rbool>();
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
			case RDOType::t_bool: return __get<rbool>() && rdovalue.__get<rbool>();
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
			case RDOType::t_bool: return __get<rbool>() || rdovalue.__get<rbool>();
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
	case RDOType::t_int  : rdovalue.__get<int>   () = -__get<int>       (); break;
	case RDOType::t_real : rdovalue.__get<double>() = -__get<double>    (); break;
	case RDOType::t_bool : rdovalue.__get<rbool> () = !__get<rbool>     (); break;
	case RDOType::t_fuzzy: rdovalue.__fuzzyV     () = __fuzzyV().u_minus(); break;
	default              : throw RDOValueException();
	}
	return rdovalue;
}

inline rbool RDOValue::operator! () const
{
	switch (typeID())
	{
	case RDOType::t_int : return !__get<int>   (); break;
	case RDOType::t_real: return !__get<double>(); break;
	case RDOType::t_bool: return !__get<rbool> (); break;
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
			case RDOType::t_real: __get<int>() += rdovalue.getInt(); return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() += rdovalue.getDouble(); return;
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
				LPRDOArrayIterator pIt = __get<rdo::intrusive_ptr_interface_wrapper<RDOArrayIterator> >();
				ASSERT(pIt);
				pIt->operator +(rdovalue.getInt());
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
				__voidPtrV() = new RDOMatrixIterator(__matrixItr() + rdovalue.getInt());
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
	UNUSED(inc);

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
	UNUSED(inc);

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
			case RDOType::t_real: __get<int>() -= rdovalue.getInt(); return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() -= rdovalue.getDouble(); return;
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
				LPRDOArrayIterator pIt = __get<rdo::intrusive_ptr_interface_wrapper<RDOArrayIterator> >();
				ASSERT(pIt);
				pIt->operator -(rdovalue.getInt());
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
				__voidPtrV() = new RDOMatrixIterator(__matrixItr() - rdovalue.getInt());
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
			case RDOType::t_int : __get<int>() *= rdovalue.__get<int>(); return;
			case RDOType::t_real: __get<double>() = ((double)__get<int>()) * rdovalue.__get<double>(); m_pType = g_real; return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() *= rdovalue.getDouble(); return;
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
			case RDOType::t_real: __get<double>() = ((double)__get<int>()) / rdovalue.getDouble(); m_pType = g_real; return;
			}
			break;
		}
	case RDOType::t_real:
		{
			switch (rdovalue.typeID())
			{
			case RDOType::t_int :
			case RDOType::t_real: __get<double>() = __get<double>() / rdovalue.getDouble(); return;
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
	case RDOType::t_array : return __arrayV ()->operator [](rdovalue);
	case RDOType::t_matrix: return __matrixV()[rdovalue];
	}
	throw RDOValueException();
}

inline RDOValue RDOValue::begin()
{
	switch (typeID())
	{
	case RDOType::t_array : return RDOValue(rdo::Factory<RDOArrayIterator>::create(__arrayV()->containerBegin()));
	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerBegin()));
	}
	throw RDOValueException();	
}

inline RDOValue RDOValue::end()
{
	switch (typeID())
	{
	case RDOType::t_array : return RDOValue(rdo::Factory<RDOArrayIterator>::create( __arrayV()->containerEnd()));
	case RDOType::t_matrix: return RDOValue(RDOMatrixIterator(__matrixV().containerEnd()));
	}
	throw RDOValueException();
}

inline void RDOValue::insert(CREF(RDOValue) itr, CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
{
	switch (typeID())
	{
	case RDOType::t_array : __arrayV ()->insertItems(itr.__arrayItr()->getIterator(), itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
	case RDOType::t_matrix: __matrixV().insertItems(itr.__matrixItr().getIterator(), itrFst.__matrixItr().getIterator(), itrLst.__matrixItr().getIterator()); return;
	}
	throw RDOValueException();	
}

inline void RDOValue::erase(CREF(RDOValue) itrFst, CREF(RDOValue) itrLst)
{
	switch (typeID())
	{
	case RDOType::t_array : __arrayV ()->eraseItems(itrFst.__arrayItr ()->getIterator(), itrLst.__arrayItr ()->getIterator()); return;
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
			__arrayV()->setArrayItem(ind, item);
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
	return *__get<rdo::intrusive_ptr_interface_wrapper<string_class> >().get();
}

inline CREF(tstring) RDOValue::__stringV() const
{
	return *__get<rdo::intrusive_ptr_interface_wrapper<string_class> >().get();
}

inline REF(RDOFuzzyValue) RDOValue::__fuzzyV()
{
	return *static_cast<PTR(RDOFuzzyValue)>(__voidPtrV());
}

inline CREF(RDOFuzzyValue) RDOValue::__fuzzyV() const
{
	return *static_cast<CPTR(RDOFuzzyValue)>(__voidPtrV());
}

inline REF(LPRDOArrayValue) RDOValue::__arrayV()
{
	return __get<rdo::intrusive_ptr_interface_wrapper<RDOArrayValue> >();
}

inline CREF(LPRDOArrayValue) RDOValue::__arrayV() const
{
	return __get<rdo::intrusive_ptr_interface_wrapper<RDOArrayValue> >();
}

inline REF(LPRDOArrayIterator) RDOValue::__arrayItr()
{
	return __get<rdo::intrusive_ptr_interface_wrapper<RDOArrayIterator> >();
}

inline CREF(LPRDOArrayIterator) RDOValue::__arrayItr() const
{
	return __get<rdo::intrusive_ptr_interface_wrapper<RDOArrayIterator> >();
}

inline REF(RDOMatrixValue) RDOValue::__matrixV()
{
	return *static_cast<PTR(RDOMatrixValue)>(__voidPtrV());
}

inline CREF(RDOMatrixValue) RDOValue::__matrixV() const
{
	return *static_cast<CPTR(RDOMatrixValue)>(__voidPtrV());
}

inline REF(RDOMatrixIterator) RDOValue::__matrixItr()
{
	return *static_cast<PTR(RDOMatrixIterator)>(__voidPtrV());
}

inline CREF(RDOMatrixIterator) RDOValue::__matrixItr() const
{
	return *static_cast<CPTR(RDOMatrixIterator)>(__voidPtrV());
}

inline REF(std::ostream) operator<< (REF(std::ostream) stream, CREF(RDOValue) rdovalue)
{
	stream << rdovalue.getAsStringForTrace();
	return stream;
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
