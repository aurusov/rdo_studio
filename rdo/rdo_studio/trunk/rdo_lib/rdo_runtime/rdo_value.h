#ifndef RDO_VALUE_H
#define RDO_VALUE_H

#include "rdo_type.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOType;
class RDOEnumType;
class RDOFuzzyValue;

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
	RDOValue(REF(RDOEnumType)    enums   );
	RDOValue(CREF(RDOEnumType)   enums, CREF(tstring) value);
	RDOValue(CREF(RDOEnumType)   enums, ruint index);
	RDOValue(CREF(RDOFuzzyValue) fuzzy   );
	RDOValue(CREF(tstring)       value   );
	RDOValue(CREF(tstring)       value, CREF(RDOType) type );

	rsint             getInt          () const;
	rsint             getEnumAsInt    () const;
	CREF(RDOEnumType) getEnum         () const;
	double            getDouble       () const;
	rbool             getBool         () const;
	CREF(tstring)     getString       () const;
	CREF(tstring)     getIdentificator() const;

	rbool   getAsBool          () const;
	tstring getAsString        () const;
	tstring getAsStringForTrace() const;

	REF(RDOValue) operator=  (CREF(RDOValue) rdovalue);
	rbool         operator== (CREF(RDOValue) rdovalue) const;
	rbool         operator!= (CREF(RDOValue) rdovalue) const;
	rbool         operator<  (CREF(RDOValue) rdovalue) const;
	rbool         operator<  (CREF(RDOValue) rdovalue);
	rbool         operator>  (CREF(RDOValue) rdovalue) const;
	rbool         operator<= (CREF(RDOValue) rdovalue) const;
	rbool         operator>= (CREF(RDOValue) rdovalue) const;
	RDOValue      operator&& (CREF(RDOValue) rdovalue) const;
	RDOValue      operator|| (CREF(RDOValue) rdovalue) const;
	RDOValue      operator-  () const;
	void          operator+= (CREF(RDOValue) rdovalue);
	void          operator-= (CREF(RDOValue) rdovalue);
	void          operator*= (CREF(RDOValue) rdovalue);
	void          operator/= (CREF(RDOValue) rdovalue);
	RDOValue      operator+  (CREF(RDOValue) rdovalue) const;
	RDOValue      operator-  (CREF(RDOValue) rdovalue) const;
	RDOValue      operator*  (CREF(RDOValue) rdovalue) const;
	RDOValue      operator/  (CREF(RDOValue) rdovalue) const;
	const RDOType* getType() 
	{
	return m_type;
	}
	CREF(RDOType)   type  () const;
	RDOType::TypeID typeID() const;

private:
	CPTR(RDOType) m_type;

	CREF(RDOEnumType)   __enumT  () const;
	 REF(tstring)       __stringV();
	CREF(tstring)       __stringV() const;
	 REF(RDOFuzzyValue) __fuzzyV ();
	CREF(RDOFuzzyValue) __fuzzyV () const;

	union {
		int          i_value;
		double       d_value;
		rbool        b_value;
		PTR(tstring) s_value;
		PTR(void)    p_data;
	} m_value;
};

} // namespace rdoRuntime

#include "rdo_value.inl"

#endif // RDO_VALUE_H
