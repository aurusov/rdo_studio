#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <cstdint>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr_interface_wrapper.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOEnumType);

class RDOValue
{
public:
	RDOValue();
	~RDOValue();

	RDOValue(const RDOValue& rdovalue);
	RDOValue(const LPRDOType& pType);
	RDOValue(int value);
	RDOValue(std::size_t value);
	RDOValue(double value);
	RDOValue(bool value);
	RDOValue(const LPRDOEnumType& pEnum);
	RDOValue(const LPRDOEnumType& pEnum, const std::string& value);
	RDOValue(const LPRDOEnumType& pEnum, std::size_t index);
	RDOValue(const std::string& value);
	RDOValue(const char* value);
	RDOValue(const std::string& value, const LPRDOType& pType);

	template <class T>
	RDOValue(const LPRDOType& pType, const rdo::intrusive_ptr<T>& pObject);

	static RDOValue fromDouble(const LPRDOType& pType, double value);

	int getInt() const;
	uint32_t getUInt() const;
	int getEnumAsInt() const;
	LPRDOEnumType getEnum() const;
	double getDouble() const;
	bool getBool() const;
	const std::string& getString() const;
	const std::string& getIdentificator() const;

	template <class T>
	const rdo::intrusive_ptr<typename T::value_type>& getPointerByType() const;

	template <class V, class T>
	const rdo::intrusive_ptr<V>& getPointerByType() const;

	template <class T>
	const rdo::intrusive_ptr<typename T::value_type>& getPointerByInterface() const;

	template <class T>
	bool isType() const;

	bool getAsBool() const;
	std::string getAsString() const;
	std::string getAsStringForTrace() const;

	RDOValue& operator=(const RDOValue& rdovalue);
	bool operator==(const RDOValue& rdovalue) const;
	bool operator!=(const RDOValue& rdovalue) const;
	bool operator<(const RDOValue& rdovalue) const;
	bool operator>(const RDOValue& rdovalue) const;
	bool operator<=(const RDOValue& rdovalue) const;
	bool operator>=(const RDOValue& rdovalue) const;
	RDOValue operator&&(const RDOValue& rdovalue) const;
	RDOValue operator||(const RDOValue& rdovalue) const;
	RDOValue operator-() const;
	bool operator!() const;
	const RDOValue& operator++();
	RDOValue operator++(int inc);
	const RDOValue& operator--();
	RDOValue operator--(int inc);
	RDOValue& operator+=(const RDOValue& rdovalue);
	RDOValue& operator-=(const RDOValue& rdovalue);
	RDOValue& operator*=(const RDOValue& rdovalue);
	RDOValue& operator/=(const RDOValue& rdovalue);
	RDOValue operator+(const RDOValue& rdovalue) const;
	RDOValue operator-(const RDOValue& rdovalue) const;
	RDOValue operator*(const RDOValue& rdovalue) const;
	RDOValue operator/(const RDOValue& rdovalue) const;

	void  setUndefined(bool undefined);
	bool isUndefined () const;

	const LPRDOType& type() const;
	RDOType::TypeID typeID() const;

	RDOValue clone() const;

	//RDOValue  begin ();
	//RDOValue  end   ();
	//void      insert(const RDOValue& itr,    const RDOValue& itrFst, const RDOValue& itrLst);
	//void      erase (const RDOValue& itrFst, const RDOValue& itrLst                       );

	//void setArrayItem(const RDOValue& ind, const RDOValue& item);

private:
	PREDECLARE_POINTER(string_class);
	class string_class
		: public rdo::counter_reference
		, public std::string
	{
	public:
		string_class(const std::string& string);
		rdo::intrusive_ptr<string_class> clone() const;
	};

	typedef unsigned char Value[sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>)];

	Value m_value; // контейнер значения
	LPRDOType m_pType; // тип значения
	bool m_undefined; // неопределенность значения

	void set        (const RDOValue& rdovalue);
	void deleteValue();

	template <class T>
	T& __get();

	template <class T>
	const T& __get() const;

	void*& __voidPtrV();
	const void*& __voidPtrV() const;
	LPRDOEnumType __enumT() const;
	std::string& __stringV();
	const std::string& __stringV() const;

	template <class T> rdo::intrusive_ptr<T>& getPointer();
	template <class T> const rdo::intrusive_ptr<T>& getPointer() const;

	std::string onPointerAsString() const;
	bool onPointerEqual(const RDOValue& rdovalue) const;
	RDOValue& onPointerPlus(const RDOValue& rdovalue);
	RDOValue& onPointerMinus(const RDOValue& rdovalue);
	RDOValue& onPointerMult(const RDOValue& rdovalue);
	RDOValue& onPointerDiv(const RDOValue& rdovalue);
	int onPointerGetInt() const;
	uint32_t onPointerGetUInt() const;
	bool onPointerAnd(const RDOValue& rdovalue) const;
	bool onPointerOr(const RDOValue& rdovalue) const;
	RDOValue onPointerUMinus() const;
};

std::ostream& operator<< (std::ostream& stream, const RDOValue& rdovalue);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_value-inl.h"
