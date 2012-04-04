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
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFuzzyType);
PREDECLARE_POINTER(RDOFuzzyValue);

//! Нечеткое значение
OBJECT(RDOFuzzyValue)
{
DECLARE_FACTORY(RDOFuzzyValue);
public:
	typedef  std::pair<RDOValue, double>                              FuzzyItem;
	typedef  std::map<FuzzyItem::first_type, FuzzyItem::second_type>  FuzzySet;

	REF(RDOFuzzyValue)         append     (CREF(RDOValue) rdovalue, double appertain);
	REF(RDOFuzzyValue)         operator() (CREF(RDOValue) rdovalue, double appertain);
	REF(double)                operator[] (CREF(RDOValue) rdovalue);
	FuzzySet::const_iterator   find       (CREF(RDOValue) rdovalue) const;
	FuzzyItem                  findValue  (CREF(RDOValue) rdovalue) const;
	FuzzySet::const_iterator   begin      () const;
	FuzzySet::const_iterator   end        () const;
	rbool                      empty      () const;
	CREF(LPRDOFuzzyType)       type       () const;
	LPRDOFuzzyValue            clone      () const;
	rbool                      inRange    (CREF(RDOValue) rdovalue);

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
//	/* 3.62 */  LPRDOFuzzyValue supplement() const;
	/* 3.78 */  LPRDOFuzzyValue a_con     () const;
	/* 3.79 */  LPRDOFuzzyValue a_dil     () const;

	/* 3.272*/ RDOValue defuzzyfication();

	tstring getAsString() const;

	virtual ~RDOFuzzyValue();

private:
//	RDOFuzzyValue(CREF(LPRDOFuzzySetDefinition)  pSetDefinition);
	RDOFuzzyValue(CREF(LPRDOFuzzyType)  pType );
	RDOFuzzyValue(CREF(LPRDOFuzzyValue) pValue);

	FuzzySet        m_fuzzySet;
	LPRDOFuzzyType  m_pType;

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

PREDECLARE_POINTER(RDOActivatedValue);
PREDECLARE_POINTER(RDOValue);

//! Нечеткая переменная

class RDOFuzzyType: public RDOType
{
DECLARE_FACTORY(RDOFuzzyType);
public:
	RDOFuzzyType();
	virtual ~RDOFuzzyType();

	RDOValue value_cast(CREF(RDOValue) from) const;
};
DECLARE_POINTER(RDOFuzzyType);

// ! Терм

OBJECT (RDOFuzzyTerm)
{
DECLARE_FACTORY (RDOFuzzyTerm)
public:

	RDOFuzzyTerm();
	~RDOFuzzyTerm();

	LPRDOFuzzyValue      getFuzzySet() const;
	tstring              getName    () const;

private:
	tstring         m_name;
	LPRDOFuzzyValue m_fuzzySet;
};

// !Лингвистическая переменная

OBJECT (RDOLingvoVariable)
{
DECLARE_FACTORY(RDOLingvoVariable)
public:

	typedef std::pair<tstring,RDOFuzzyValue>TermInVariable;
	typedef std::map<TermInVariable::first_type, TermInVariable::second_type>Set;
	
	Set::const_iterator            begin ();
	Set::const_iterator            end   ();
	
	tstring                        getName      () {return name;};
	RDOLingvoVariable              append       (CREF(TermInVariable) pair);
//	LPRDOLingvoVariable            fuzzyfication(CREF(RDOValue)value);

private:
	RDOLingvoVariable();
	RDOLingvoVariable(CREF(RDOLingvoVariable));
	~RDOLingvoVariable();

	Set     m_set;
	tstring name;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_fuzzy.inl"

#endif // _LIB_RUNTIME_FUZZY_H_
