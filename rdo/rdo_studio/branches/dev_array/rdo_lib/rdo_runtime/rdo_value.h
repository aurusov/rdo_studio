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
#include "rdo_common/rdosmart_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOType;
class RDOEnumType;
class RDOFuzzyValue;
class RDOArrayValue;
class RDOArrayIterator;

class RDOValue
{
public:
	RDOValue();
	~RDOValue();

	RDOValue(CREF(RDOValue)      rdovalue);
	RDOValue(CREF(RDOType)       type    );
	RDOValue(rsint               value   );
	RDOValue(ruint               value   );
	RDOValue(double              value   );
	RDOValue(rbool               value   );
	RDOValue(CREF(RDOEnumType)   enums   );
	RDOValue(CREF(RDOEnumType)   enums, CREF(tstring) value);
	RDOValue(CREF(RDOEnumType)   enums, ruint index);
	RDOValue(CREF(RDOFuzzyValue) fuzzy   );
	RDOValue(CREF(tstring)       value   );
	RDOValue(CPTR(tchar)         value   );
	RDOValue(CREF(tstring)       value, CREF(RDOType) type );
	RDOValue(CREF(RDOArrayValue)    arrayValue);
	RDOValue(CREF(RDOArrayIterator) aIterator );

	rsint               getInt          () const;
	rsint               getEnumAsInt    () const;
	CREF(RDOEnumType)   getEnum         () const;
	double              getDouble       () const;
	rbool               getBool         () const;
	CREF(tstring)       getString       () const;
	CREF(tstring)       getIdentificator() const;
	CREF(RDOArrayValue) getArray        () const;


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

	RDOValue  begin();
	RDOValue    end();
	void     insert(CREF(RDOValue) itr,    CREF(RDOValue) itrFst, CREF(RDOValue) itrLst);
	void      erase(CREF(RDOValue) itrFst, CREF(RDOValue) itrLst                       );

	CREF(RDOType)   type  () const;
	RDOType::TypeID typeID() const;

private:
	CPTR(RDOType) m_type;

	void set        (CREF(RDOValue) rdovalue);
	void deleteValue();

	CREF(RDOEnumType)       __enumT   () const;
	 REF(tstring)           __stringV ();
	CREF(tstring)           __stringV () const;
	 REF(RDOFuzzyValue)     __fuzzyV  ();
	CREF(RDOFuzzyValue)     __fuzzyV  () const;
	 REF(RDOArrayValue)     __arrayV  ();
	CREF(RDOArrayValue)     __arrayV  () const;
	 REF(RDOArrayIterator)  __arrayItr();
	CREF(RDOArrayIterator)  __arrayItr() const;

	class smart_tstring: public rdo::smart_ptr<tstring>
	{
	public:
		typedef rdo::smart_ptr<tstring> parent_type;

		smart_tstring(PTR(tstring) pString)
			: parent_type(pString, true)
		{}
		void addref () { parent_type::addref (); }
		void release() { parent_type::release(); }
	};
	void deleteString();

	union Value
	{
		int                i_value;
		double             d_value;
		rbool              b_value;
		PTR(smart_tstring) s_value;
		PTR(void)          p_data;
	};
	Value m_value;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_value.inl"

#endif //! _RDO_VALUE_H_
