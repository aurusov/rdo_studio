/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_value.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_VALUE_H_
#define _RDO_VALUE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOEnumType;
class RDOFuzzyValue;
class RDOArrayValue;
class RDOArrayIterator;
class RDOMatrixValue;
class RDOMatrixIterator;
PREDECLARE_POINTER(RDOEnumType  );
PREDECLARE_POINTER(RDOFuzzyValue);
PREDECLARE_POINTER(RDOArrayValue);

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

	rsint             getInt          () const;
	rsint             getEnumAsInt    () const;
	LPRDOEnumType     getEnum         () const;
	double            getDouble       () const;
	rbool             getBool         () const;
	CREF(tstring)     getString       () const;
	CREF(tstring)     getIdentificator() const;
	CREF(RDOArrayValue) getArray        () const;
	CREF(RDOMatrixValue)getMatrix       () const;


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

	RDOValue  begin();
	RDOValue  end();
	void      insert(CREF(RDOValue) itr,    CREF(RDOValue) itrFst, CREF(RDOValue) itrLst);
	void      erase(CREF(RDOValue) itrFst, CREF(RDOValue) itrLst                       );

private:
	LPRDOType m_pType;

	void set        (CREF(RDOValue) rdovalue);
	void deleteValue();

	LPRDOEnumType       __enumT  () const;
	 REF(tstring)       __stringV();
	CREF(tstring)       __stringV() const;
	 REF(RDOFuzzyValue) __fuzzyV ();
	CREF(RDOFuzzyValue) __fuzzyV () const;
	 REF(RDOArrayValue) __arrayV ();
	CREF(RDOArrayValue) __arrayV () const;
	 REF(RDOArrayIterator)  __arrayItr ();
	CREF(RDOArrayIterator)  __arrayItr () const;
	 REF(RDOMatrixValue)    __matrixV  ();
	CREF(RDOMatrixValue)    __matrixV  () const;
	 REF(RDOMatrixIterator) __matrixItr();
	CREF(RDOMatrixIterator) __matrixItr() const;

	OBJECT(string_class) IS INSTANCE_OF(tstring)
	{
	public:
		string_class(CREF(tstring) string)
			: tstring(string)
		{}
	};

	class smart_string: public rdo::intrusive_ptr<string_class>
	{
	public:
		typedef rdo::intrusive_ptr<string_class> parent_type;

		smart_string(PTR(string_class) pString)
			: parent_type(pString)
		{}

		PTR(string_class)  get()       { return parent_type::get(); }
		CPTR(string_class) get() const { return parent_type::get(); }

		void  addref () { parent_type::addref ();      }
		void  release() { parent_type::release();      }
		rbool owner  () { return parent_type::owner(); }
	};
	void deleteString();

	union Value
	{
		int                i_value;
		double             d_value;
		rbool              b_value;
		PTR(smart_string)  s_value;
		PTR(void)          p_data;
	};
	Value m_value;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_value.inl"

#endif //! _RDO_VALUE_H_
