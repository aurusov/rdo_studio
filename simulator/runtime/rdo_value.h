/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_value.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     RDOValue
  \indent    4T
*/

#ifndef _LIB_RUNTIME_VALUE_H_
#define _LIB_RUNTIME_VALUE_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr_interface_wrapper.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOEnumType);

//! Значение переменных в РДО
class RDOValue
{
public:
	RDOValue();
	~RDOValue();

	RDOValue(CREF(RDOValue) rdovalue);
	RDOValue(CREF(LPRDOType) pType);
	RDOValue(rsint value);
	RDOValue(ruint value);
#ifdef ARCHITECTURE_AMD64
	RDOValue(ruint64 value);
#endif // ARCHITECTURES_AMD64
	RDOValue(double value);
	RDOValue(bool value);
	RDOValue(CREF(LPRDOEnumType) pEnum);
	RDOValue(CREF(LPRDOEnumType) pEnum, CREF(std::string) value);
	RDOValue(CREF(LPRDOEnumType) pEnum, ruint index);
	RDOValue(CREF(std::string) value);
	RDOValue(CPTR(char) value);
	RDOValue(CREF(std::string) value, CREF(LPRDOType) pType);

	template <class T>
	RDOValue(CREF(LPRDOType) pType, CREF(rdo::intrusive_ptr<T>) pObject);

	static RDOValue fromDouble(CREF(LPRDOType) pType, double value);

	rsint getInt() const;
	ruint getUInt() const;
	rsint getEnumAsInt() const;
	LPRDOEnumType getEnum() const;
	double getDouble() const;
	bool getBool() const;
	CREF(std::string) getString() const;
	CREF(std::string) getIdentificator() const;

	template <class T>
	CREF(rdo::intrusive_ptr<typename T::value_type>) getPointerByType() const;

	template <class Pair>
	CREF(rdo::intrusive_ptr<typename Pair::Value>) getPointerByType() const;

	template <class T>
	CREF(rdo::intrusive_ptr<typename T::value_type>) getPointerByInterface() const;

	template <class T>
	bool isType() const;

	bool getAsBool() const;
	std::string getAsString() const;
	std::string getAsStringForTrace() const;

	REF(RDOValue) operator=(CREF(RDOValue) rdovalue);
	bool operator==(CREF(RDOValue) rdovalue) const;
	bool operator!=(CREF(RDOValue) rdovalue) const;
	bool operator<(CREF(RDOValue) rdovalue) const;
	bool operator>(CREF(RDOValue) rdovalue) const;
	bool operator<=(CREF(RDOValue) rdovalue) const;
	bool operator>=(CREF(RDOValue) rdovalue) const;
	RDOValue operator&&(CREF(RDOValue) rdovalue) const;
	RDOValue operator||(CREF(RDOValue) rdovalue) const;
	RDOValue operator-() const;
	bool operator!() const;
	CREF(RDOValue) operator++();
	RDOValue operator++(int inc);
	CREF(RDOValue) operator--();
	RDOValue operator--(int inc);
	REF(RDOValue) operator+=(CREF(RDOValue) rdovalue);
	REF(RDOValue) operator-=(CREF(RDOValue) rdovalue);
	REF(RDOValue) operator*=(CREF(RDOValue) rdovalue);
	REF(RDOValue) operator/=(CREF(RDOValue) rdovalue);
	RDOValue operator+(CREF(RDOValue) rdovalue) const;
	RDOValue operator-(CREF(RDOValue) rdovalue) const;
	RDOValue operator*(CREF(RDOValue) rdovalue) const;
	RDOValue operator/(CREF(RDOValue) rdovalue) const;

	void  setUndefined(bool undefined);
	bool isUndefined () const;

	CREF(LPRDOType) type  () const;
	RDOType::TypeID typeID() const;

	RDOValue clone() const;

	//RDOValue  begin ();
	//RDOValue  end   ();
	//void      insert(CREF(RDOValue) itr,    CREF(RDOValue) itrFst, CREF(RDOValue) itrLst);
	//void      erase (CREF(RDOValue) itrFst, CREF(RDOValue) itrLst                       );

	//void setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item);

private:
	//! Строковый тип данных
	PREDECLARE_POINTER(string_class);
	class string_class
		: public rdo::counter_reference
		, public std::string
	{
	public:
		string_class(CREF(std::string) string);
		rdo::intrusive_ptr<string_class> clone() const;
	};

	//! Тип контейнера значения, размер определяется по максимальному размеру типа данных
	typedef rbyte Value[sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>)];

	Value m_value; //!< контейнер значения
	LPRDOType m_pType; //!< тип значения
	bool m_undefined; //!< неопределенность значения

	void set        (CREF(RDOValue) rdovalue);
	void deleteValue();

	template <class T>
	REF(T) __get();

	template <class T>
	CREF(T) __get() const;

	 REF(PTR(void)) __voidPtrV();
	CREF(PTR(void)) __voidPtrV() const;
	LPRDOEnumType __enumT   () const;
	 REF(std::string) __stringV ();
	CREF(std::string) __stringV () const;

	template <class T>  REF(rdo::intrusive_ptr<T>) getPointer();
	template <class T> CREF(rdo::intrusive_ptr<T>) getPointer() const;

	std::string onPointerAsString() const;
	bool onPointerEqual(CREF(RDOValue) rdovalue) const;
	REF(RDOValue) onPointerPlus(CREF(RDOValue) rdovalue);
	REF(RDOValue) onPointerMinus(CREF(RDOValue) rdovalue);
	REF(RDOValue) onPointerMult(CREF(RDOValue) rdovalue);
	REF(RDOValue) onPointerDiv(CREF(RDOValue) rdovalue);
	rsint onPointerGetInt() const;
	ruint onPointerGetUInt() const;
	bool onPointerAnd(CREF(RDOValue) rdovalue) const;
	bool onPointerOr(CREF(RDOValue) rdovalue) const;
	RDOValue onPointerUMinus() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_value.inl"

#endif // _LIB_RUNTIME_VALUE_H_
