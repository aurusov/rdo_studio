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

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "utils/smart_ptr/intrusive_ptr_interface_wrapper.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOFuzzyValue;
class RDOArrayValue;
class RDOArrayIterator;
class RDOMatrixValue;
class RDOMatrixIterator;
PREDECLARE_POINTER(RDOEnumType);

//! Значение переменных в РДО
class RDOValue
{
public:
	RDOValue();
	~RDOValue();

	RDOValue(CREF(RDOValue)      rdovalue);
	RDOValue(CREF(LPRDOType)     pType   );
	RDOValue(rsint               value   );
	RDOValue(ruint               value   );
	RDOValue(double              value   );
	RDOValue(rbool               value   );
	RDOValue(CREF(LPRDOEnumType) pEnum   );
	RDOValue(CREF(LPRDOEnumType) pEnum, CREF(tstring) value);
	RDOValue(CREF(LPRDOEnumType) pEnum, ruint index);
	RDOValue(CREF(RDOFuzzyValue) fuzzy   );
	RDOValue(CREF(tstring)       value   );
	RDOValue(CPTR(tchar)         value   );
	RDOValue(CREF(tstring)       value, CREF(LPRDOType) pType);
	RDOValue(CREF(RDOArrayValue)     arrayValue );
	RDOValue(CREF(RDOArrayIterator)  aIterator  );
	RDOValue(CREF(RDOMatrixValue)    matrixValue);
	RDOValue(CREF(RDOMatrixIterator) mIterator  );

	template <class T>
	RDOValue(CREF(LPRDOType) pType, CREF(rdo::intrusive_ptr<T>) pObject);

	rsint             getInt          () const;
	ruint             getUInt         () const;
	rsint             getEnumAsInt    () const;
	LPRDOEnumType     getEnum         () const;
	double            getDouble       () const;
	rbool             getBool         () const;
	CREF(tstring)     getString       () const;
	CREF(tstring)     getIdentificator() const;
	CREF(RDOArrayValue) getArray      () const;
	CREF(RDOMatrixValue)getMatrix     () const;


	rbool   getAsBool          () const;
	tstring getAsString        () const;
	tstring getAsStringForTrace() const;

	REF(RDOValue)  operator=  (CREF(RDOValue) rdovalue);
	rbool          operator== (CREF(RDOValue) rdovalue) const;
	rbool          operator!= (CREF(RDOValue) rdovalue) const;
	rbool          operator<  (CREF(RDOValue) rdovalue) const;
	rbool          operator<  (CREF(RDOValue) rdovalue);
	rbool          operator>  (CREF(RDOValue) rdovalue) const;
	rbool          operator<= (CREF(RDOValue) rdovalue) const;
	rbool          operator>= (CREF(RDOValue) rdovalue) const;
	RDOValue       operator&& (CREF(RDOValue) rdovalue) const;
	RDOValue       operator|| (CREF(RDOValue) rdovalue) const;
	RDOValue       operator-  () const;
	rbool          operator!  () const;
	CREF(RDOValue) operator++ ();
	RDOValue       operator++ (int inc);
	CREF(RDOValue) operator-- ();
	RDOValue       operator-- (int inc);
	void           operator+= (CREF(RDOValue) rdovalue);
	void           operator-= (CREF(RDOValue) rdovalue);
	void           operator*= (CREF(RDOValue) rdovalue);
	void           operator/= (CREF(RDOValue) rdovalue);
	RDOValue       operator+  (CREF(RDOValue) rdovalue) const;
	RDOValue       operator-  (CREF(RDOValue) rdovalue) const;
	RDOValue       operator*  (CREF(RDOValue) rdovalue) const;
	RDOValue       operator/  (CREF(RDOValue) rdovalue) const;
	RDOValue       operator[] (CREF(RDOValue) rdovalue);

	CREF(LPRDOType) type  () const;
	RDOType::TypeID typeID() const;

	RDOValue  begin ();
	RDOValue  end   ();
	void      insert(CREF(RDOValue) itr,    CREF(RDOValue) itrFst, CREF(RDOValue) itrLst);
	void      erase (CREF(RDOValue) itrFst, CREF(RDOValue) itrLst                       );

	void setArrayItem(CREF(RDOValue) ind, CREF(RDOValue) item);

private:
	//! Строковый тип данных
	OBJECT(string_class) IS INSTANCE_OF(tstring)
	{
	public:
		string_class(CREF(tstring) string);
		rdo::intrusive_ptr<string_class> clone() const;
	};

	//! Тип контейнера значения, размер определяется по максимальному размеру типа данных
	typedef rbyte Value[sizeof(rdo::intrusive_ptr_interface_wrapper<string_class>)];

	Value      m_value; //!< контейнер значения
	LPRDOType  m_pType; //!< тип значения

	void set        (CREF(RDOValue) rdovalue);
	void deleteValue();

	template <class T>
	REF(T) __get();

	template <class T>
	CREF(T) __get() const;

	 REF(PTR(void))         __voidPtrV  ();
	CREF(PTR(void))         __voidPtrV  () const;
	LPRDOEnumType           __enumT     () const;
	 REF(tstring)           __stringV   ();
	CREF(tstring)           __stringV   () const;
	 REF(RDOFuzzyValue)     __fuzzyV    ();
	CREF(RDOFuzzyValue)     __fuzzyV    () const;
	 REF(RDOArrayValue)     __arrayV    ();
	CREF(RDOArrayValue)     __arrayV    () const;
	 REF(RDOArrayIterator)  __arrayItr  ();
	CREF(RDOArrayIterator)  __arrayItr  () const;
	 REF(RDOMatrixValue)    __matrixV   ();
	CREF(RDOMatrixValue)    __matrixV   () const;
	 REF(RDOMatrixIterator) __matrixItr ();
	CREF(RDOMatrixIterator) __matrixItr () const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_value.inl"

#endif // _LIB_RUNTIME_VALUE_H_
