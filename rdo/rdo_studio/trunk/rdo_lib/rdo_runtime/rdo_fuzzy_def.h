#ifndef RDO_FUZZY_DEF_H
#define RDO_FUZZY_DEF_H

#ifndef RDO_VALUE_H
#error include "rdo_value.h" first (и вообще, вместо "rdo_fuzzy_def.h" лучше юзать "rdo_fuzzy.h")
#endif

//
// Напрямую этот файл инклюдить не надо, юзайте
// #include "rdo_fuzzy.h"
//

#include "rdo_type.h"
#include "rdoruntime_object.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValueItem
// ----------------------------------------------------------------------------
struct RDOFuzzyValueItem
{
	RDOValue m_rdovalue;
	double   m_appertain;

	RDOFuzzyValueItem();
	RDOFuzzyValueItem(CREF(RDOValue) rdovalue, double appertain);

	rbool operator< (CREF(RDOFuzzyValueItem) value) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyValue
// ----------------------------------------------------------------------------
class  RDOFuzzyType;

class RDOFuzzyValue
{
public:
	RDOFuzzyValue(CREF(RDOFuzzyType)  type );
	RDOFuzzyValue(CREF(RDOFuzzyValue) value);
	~RDOFuzzyValue();

	typedef std::vector< RDOFuzzyValueItem > FuzzySet;

	REF(RDOFuzzyValue) append     (CREF(RDOValue) rdovalue, double appertain);
	REF(RDOFuzzyValue) operator() (CREF(RDOValue) rdovalue, double appertain)
	{
		return append(rdovalue, appertain);
	}

	CREF(RDOFuzzyType)        type () const;

	FuzzySet::const_iterator  begin() const;
	FuzzySet::const_iterator  end  () const;

	/* 3.37  */  RDOFuzzyValue operator&& (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.40  */  RDOFuzzyValue operator|| (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.102 */  RDOFuzzyValue operator+  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.104 */  RDOFuzzyValue operator-  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.106 */  RDOFuzzyValue operator*  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.108 */  RDOFuzzyValue operator/  (CREF(RDOFuzzyValue) fuzzy_value) const;

	/* 3.116 */  RDOFuzzyValue u_minus() const;
	/* 3.117 */  RDOFuzzyValue u_obr  () const;
	/* 3.118 */  RDOFuzzyValue u_scale(double scale) const;
	/* 3.119 */  RDOFuzzyValue u_log  () const;

	/* 3.39  */  RDOFuzzyValue a_mult    (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.48  */  RDOFuzzyValue alpha     (double appertain) const;
	/* 3.62  */  RDOFuzzyValue suppliment() const;
	/* 3.78  */  RDOFuzzyValue a_con     () const;
	/* 3.79  */  RDOFuzzyValue a_dil     () const;

	/* 3.272 */ RDOValue defuzzyfication();

	tstring getAsString() const;

private:
	FuzzySet            m_fuzzySet;
	CPTR(RDOFuzzyType)  m_type;

	typedef RDOValue (*ExtUnaryFun )(CREF(RDOValue) value);
	typedef RDOValue (*ExtUnaryFunP)(CREF(RDOValue) value, PTR(void) param);
	typedef RDOValue (*ExtBinaryFun)(CREF(RDOValue) value1, CREF(RDOValue) value2);

	             RDOFuzzyValue a_pow     (double power) const;
	/* 3.114 */  RDOFuzzyValue ext_unary (ExtUnaryFun  fun) const;
	/* 3.114 */  RDOFuzzyValue ext_unary (ExtUnaryFunP fun, PTR(void) param) const;
	/* 3.83  */  RDOFuzzyValue ext_binary(ExtBinaryFun fun, CREF(RDOFuzzyValue) fuzzy_value) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyType
// ----------------------------------------------------------------------------
class  RDOFuzzySetDefinition;

class RDOFuzzyType: public RDOType, public RDORuntimeParent
{
public:
	RDOFuzzyType(PTR(RDOFuzzySetDefinition) fuzzySetDefinition);
	virtual ~RDOFuzzyType();
	virtual tstring asString() const;

	rbool operator== (CREF(RDOFuzzyType) type) const;
	rbool operator!= (CREF(RDOFuzzyType) type) const;

	rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
	RDOFuzzyValue  getSuppliment(CREF(RDOFuzzyValue) value) const;

private:
	typedef std::map< tstring, RDOFuzzyValue > Terms;

	Terms                       m_terms;
	PTR(RDOFuzzySetDefinition)  m_fuzzySetDefinition;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinition
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinition: public RDORuntimeObject
{
public:
	RDOFuzzySetDefinition(PTR(RDORuntimeParent) parent);
	virtual ~RDOFuzzySetDefinition();

	virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const = 0;
	virtual  RDOFuzzyValue  getSuppliment(CREF(RDOFuzzyValue) value) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionFixed
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinitionFixed: public RDOFuzzySetDefinition, private rdo::vector< RDOValue >
{
public:
	RDOFuzzySetDefinitionFixed(PTR(RDORuntimeParent) parent);
	virtual ~RDOFuzzySetDefinitionFixed();

	REF(RDOFuzzySetDefinitionFixed) append     (CREF(RDOValue) rdovalue);
	REF(RDOFuzzySetDefinitionFixed) operator() (CREF(RDOValue) rdovalue);

	virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
	virtual  RDOFuzzyValue  getSuppliment(CREF(RDOFuzzyValue) value) const;

private:
	typedef rdo::vector< RDOValue > Range;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzySetDefinitionRangeDiscret
// ----------------------------------------------------------------------------
class RDOFuzzySetDefinitionRangeDiscret: public RDOFuzzySetDefinition
{
public:
	RDOFuzzySetDefinitionRangeDiscret(PTR(RDORuntimeParent) parent, CREF(RDOValue) step = 1);
	virtual ~RDOFuzzySetDefinitionRangeDiscret();

	struct Range
	{
		RDOValue m_from;
		RDOValue m_till;

		Range(CREF(RDOValue) from, CREF(RDOValue) till);
	};
	typedef std::vector< Range > Ranges;

	REF(RDOFuzzySetDefinitionRangeDiscret) append     (CREF(RDOValue) from, CREF(RDOValue) till);
	REF(RDOFuzzySetDefinitionRangeDiscret) operator() (CREF(RDOValue) from, CREF(RDOValue) till);

	virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
	virtual  RDOFuzzyValue  getSuppliment(CREF(RDOFuzzyValue) value) const;

private:
	Ranges    m_range;
	RDOValue  m_step;
};

// ----------------------------------------------------------------------------
// ---------- RDOFuzzyEmptyType
// ----------------------------------------------------------------------------
class RDOFuzzyEmptyType: public RDOFuzzyType
{
public:
	static CREF(RDOFuzzyEmptyType) getInstance(PTR(RDORuntimeParent) parent);

private:
	static PTR(RDOFuzzyEmptyType) s_emptyType;

	// ----------------------------------------------------------------------------
	// ---------- RDOFuzzySetDefinitionEmpty
	// ----------------------------------------------------------------------------
	class RDOFuzzySetDefinitionEmpty: public RDOFuzzySetDefinition
	{
	public:
		RDOFuzzySetDefinitionEmpty(PTR(RDORuntimeParent) parent);
		virtual ~RDOFuzzySetDefinitionEmpty();

		virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
		virtual  RDOFuzzyValue  getSuppliment(CREF(RDOFuzzyValue) value) const;
	};

	RDOFuzzyEmptyType(PTR(RDORuntimeParent) parent);

	virtual ~RDOFuzzyEmptyType();
	virtual tstring asString() const;
};

} // namespace rdoRuntime

#endif // RDO_FUZZY_DEF_H
