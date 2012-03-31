/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_fuzzy.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     Нечеткая логика
  \indent    4T
*/

#ifndef _LIB_RUNTIME_FUZZY_H_
#define _LIB_RUNTIME_FUZZY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <set>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFuzzyValueRange);
PREDECLARE_POINTER(RDOFuzzyValue);
PREDECLARE_POINTER(RDOFuzzyMembership);


//! Диапазон значений(область определения)

class RDOFuzzyValueRange : public RDOValue
{
DECLARE_FACTORY(RDOFuzzyValueRange);
public:
	typedef std::set<RDOValue> Range;

	Range::const_iterator   begin       () const;
	Range::const_iterator   end         () const;
	rbool                   empty       () const;
	RDOValue                findValue   (CREF(RDOValue) rdovalue) const;

	rbool          checkRange(RDOValue checkValue);
	CREF(RDOValue) getRange  ();

private:
	RDOFuzzyValueRange(LPRDOType pType);
	RDOFuzzyValueRange(CREF(RDOValue) range1,CREF(RDOValue) range2);

	Range    m_range;
};
//! Функкция принадлежности

class RDOFuzzyMembership
{
DECLARE_FACTORY(RDOFuzzyMembership);
public:
	virtual double getValue(double x) = 0;
private:
	RDOFuzzyMembership();

	double             membership;
};

class RDOFuzzyMembershipZ: public RDOFuzzyMembership
{
DECLARE_FACTORY(RDOFuzzyMembershipZ)
public:
	double   getValue(double x);
private:
	RDOFuzzyMembershipZ();
	RDOFuzzyMembershipZ(RDOValue range1, RDOValue range2);

	double m_range1;
	double m_range2;
};

//! Нечеткое множество

OBJECT(RDOFuzzyValue)
{
DECLARE_FACTORY(RDOFuzzyValue);
public:
	typedef  std::pair<RDOValue, double>                              FuzzyItem;
	typedef  std::map <FuzzyItem::first_type, FuzzyItem::second_type> FuzzySet;

	double                     getAppertain(CREF(RDOValue) value);
	REF(RDOFuzzyValue)         append      (CREF(RDOValue) rdovalue);
	REF(RDOFuzzyValue)         operator()  (CREF(RDOValue) rdovalue);
	REF(double)                operator[]  (CREF(RDOValue) rdovalue);
	FuzzySet::const_iterator   find        (CREF(RDOValue) rdovalue) const;
	FuzzyItem                  findValue   (CREF(RDOValue) rdovalue) const;
	FuzzySet::const_iterator   begin       () const;
	FuzzySet::const_iterator   end         () const;
	rbool                      empty       () const;
	CREF(LPRDOType)            type        () const;
	LPRDOFuzzyValue            clone       () const;
	rbool                      inRange     (CREF(RDOValue) rdovalue);
	//CREF(LPRDOFuzzyValueRange::Range) getRange () {return m_Range};

	/* 3.37 */  LPRDOFuzzyValue operator&& (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.40 */  LPRDOFuzzyValue operator|| (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.102*/  LPRDOFuzzyValue operator+  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.104*/  LPRDOFuzzyValue operator-  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.106*/  LPRDOFuzzyValue operator*  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.108*/  LPRDOFuzzyValue operator/  (CREF(LPRDOFuzzyValue) pFuzzyValue) const;

	/* 3.116*/  LPRDOFuzzyValue u_minus() const;
	/* 3.117*/  LPRDOFuzzyValue u_obr  () const;
	/* 3.118*/  LPRDOFuzzyValue u_scale(double scale) const;
	/* 3.119*/  LPRDOFuzzyValue u_log  () const;

	/* 3.39 */  LPRDOFuzzyValue a_mult    (CREF(LPRDOFuzzyValue) pFuzzyValue) const;
	/* 3.48 */  LPRDOFuzzyValue alpha     (double appertain) const;
	/* 3.62 */  LPRDOFuzzyValue supplement() const;
	/* 3.78 */  LPRDOFuzzyValue a_con     () const;
	/* 3.79 */  LPRDOFuzzyValue a_dil     () const;

	/* 3.272*/ RDOValue defuzzyfication();

	tstring getAsString() const;

private:
	RDOFuzzyValue();
	RDOFuzzyValue(CREF(LPRDOFuzzyValueRange) pRange);
	RDOFuzzyValue(CREF(LPRDOFuzzyValueRange) pRange, CREF(LPRDOFuzzyMembership) pMembership);
	RDOFuzzyValue(CREF(LPRDOFuzzyValue)      pValue);
	virtual ~RDOFuzzyValue();

	FuzzySet             m_fuzzySet;
	LPRDOFuzzyValueRange m_Range;
//	LPRDOFuzzyMembership m_fuzzyMembership;

	FuzzySet::iterator  begin();
	FuzzySet::iterator  end  ();

	typedef RDOValue (*ExtUnaryFun )(CREF(RDOValue) rdovalue);
	typedef RDOValue (*ExtUnaryFunP)(CREF(RDOValue) rdovalue, PTR(void) pParam);
	typedef RDOValue (*ExtBinaryFun)(CREF(RDOValue) rdovalue1, CREF(RDOValue) rdovalue2);

	            LPRDOFuzzyValue a_pow     (double power) const;
	/* 3.114*/  LPRDOFuzzyValue ext_unary (ExtUnaryFun  fun) const;
	/* 3.114*/  LPRDOFuzzyValue ext_unary (ExtUnaryFunP fun, PTR(void) pParam) const;
	/* 3.83 */  LPRDOFuzzyValue ext_binary(ExtBinaryFun fun, CREF(LPRDOFuzzyValue) pFuzzyValue) const;
};

//PREDECLARE_POINTER(RDOActivatedValue);
PREDECLARE_POINTER(RDOValue);

//! Терм

class RDOFuzzyTerm
{
DECLARE_FACTORY(RDOFuzzyTerm);
public:
	RDOFuzzyTerm();
	virtual ~RDOFuzzyTerm();

	CREF(LPRDOFuzzyValue)   getFuzzySet() const;
	virtual tstring         name       () const;
	virtual RDOValue        value_cast (CREF(RDOValue) from) const;
	CREF(RDOValue)          getRange   () const;

//	rbool operator== (CREF(RDOFuzzyTerm) type) const;
//	rbool operator!= (CREF(RDOFuzzyTerm) type) const;

//	rbool           inRange      (CREF(RDOValue)        rdovalue) const;
//	LPRDOFuzzyValue getSupplement(CREF(LPRDOFuzzyValue) pValue  ) const;

private:
	tstring              m_name;
	LPRDOFuzzyValue      m_FuzzySet;
};
DECLARE_POINTER(RDOFuzzyTerm);

// !Лингвистическая переменная

OBJECT (RDOLingvoVariable)
{
DECLARE_FACTORY(RDOLingvoVariable)
public:
	typedef std::vector<RDOFuzzyTerm> TermSet;

	TermSet::const_iterator            begin ();
	TermSet::const_iterator            end   ();
	
	tstring                        getName      () {return name;};
	RDOLingvoVariable              append       (CREF(RDOFuzzyTerm) term);
	LPRDOLingvoVariable            fuzzyfication(CREF(RDOValue)value);

private:
	RDOLingvoVariable();
	RDOLingvoVariable(CREF(LPRDOFuzzyTerm) pTerm, tstring name);
	RDOLingvoVariable(CREF(RDOLingvoVariable));

	tstring              name;
	TermSet              m_TermSet;
};
CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_fuzzy.inl"

#endif // _LIB_RUNTIME_FUZZY_H_
