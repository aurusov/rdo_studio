/*!
  \copyright (c) RDO-Team, 2008
  \file      rdo_fuzzy_def.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.07.2008
  \brief     Нечеткая логика
  \indent    4T
*/

#ifndef _LIB_RUNTIME_FUZZY_DEF_H_
#define _LIB_RUNTIME_FUZZY_DEF_H_

#ifndef _LIB_RUNTIME_VALUE_H_
#error include "rdo_lib/rdo_runtime/rdo_value.h" first (вместо "rdo_fuzzy_def.h" надо инклюдить "rdo_fuzzy.h")
#endif

//
// Напрямую этот файл инклюдить не надо, юзайте
// #include "rdo_lib/rdo_runtime/rdo_fuzzy.h"
//

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOFuzzyType);

/*!
  \class     RDOFuzzyValue
  \brief     Нечеткое значение
*/
class RDOFuzzyValue
{
public:
	typedef std::pair<RDOValue, double>                              FuzzyItem;
	typedef std::map<FuzzyItem::first_type, FuzzyItem::second_type>  FuzzySet;

	RDOFuzzyValue(CREF(LPRDOFuzzyType) pType);
	RDOFuzzyValue(CREF(RDOFuzzyValue)  value);
	~RDOFuzzyValue();

	REF(RDOFuzzyValue)         append     (CREF(RDOValue) rdovalue, double appertain);
	REF(RDOFuzzyValue)         operator() (CREF(RDOValue) rdovalue, double appertain);
	REF(double)                operator[] (CREF(RDOValue) rdovalue);
	FuzzySet::const_iterator   find       (CREF(RDOValue) rdovalue) const;
	FuzzyItem                  findValue  (CREF(RDOValue) rdovalue) const;
	FuzzySet::const_iterator   begin      () const;
	FuzzySet::const_iterator   end        () const;
	rbool                      empty      () const;
	CREF(LPRDOFuzzyType)       type       () const;

	/* 3.37 */  RDOFuzzyValue operator&& (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.40 */  RDOFuzzyValue operator|| (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.102*/  RDOFuzzyValue operator+  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.104*/  RDOFuzzyValue operator-  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.106*/  RDOFuzzyValue operator*  (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.108*/  RDOFuzzyValue operator/  (CREF(RDOFuzzyValue) fuzzy_value) const;

	/* 3.116*/  RDOFuzzyValue u_minus() const;
	/* 3.117*/  RDOFuzzyValue u_obr  () const;
	/* 3.118*/  RDOFuzzyValue u_scale(double scale) const;
	/* 3.119*/  RDOFuzzyValue u_log  () const;

	/* 3.39 */  RDOFuzzyValue a_mult    (CREF(RDOFuzzyValue) fuzzy_value) const;
	/* 3.48 */  RDOFuzzyValue alpha     (double appertain) const;
	/* 3.62 */  RDOFuzzyValue supplement() const;
	/* 3.78 */  RDOFuzzyValue a_con     () const;
	/* 3.79 */  RDOFuzzyValue a_dil     () const;

	/* 3.272*/ RDOValue defuzzyfication();

	tstring getAsString() const;

private:
	FuzzySet        m_fuzzySet;
	LPRDOFuzzyType  m_pType;

	FuzzySet::iterator  begin();
	FuzzySet::iterator  end  ();

	typedef RDOValue (*ExtUnaryFun )(CREF(RDOValue) value);
	typedef RDOValue (*ExtUnaryFunP)(CREF(RDOValue) value, PTR(void) param);
	typedef RDOValue (*ExtBinaryFun)(CREF(RDOValue) value1, CREF(RDOValue) value2);

	             RDOFuzzyValue a_pow     (double power) const;
	/* 3.114*/  RDOFuzzyValue ext_unary (ExtUnaryFun  fun) const;
	/* 3.114*/  RDOFuzzyValue ext_unary (ExtUnaryFunP fun, PTR(void) param) const;
	/* 3.83 */  RDOFuzzyValue ext_binary(ExtBinaryFun fun, CREF(RDOFuzzyValue) fuzzy_value) const;
};

PREDECLARE_POINTER(RDOFuzzySetDefinition);

/*!
  \class     RDOFuzzyType
  \brief     Тип нечеткой переменной
*/
class RDOFuzzyType: public RDOType
{
DECLARE_FACTORY(RDOFuzzyType);
public:
	virtual tstring  name      ()                    const;
	virtual RDOValue value_cast(CREF(RDOValue) from) const;

	rbool operator== (CREF(RDOFuzzyType) type) const;
	rbool operator!= (CREF(RDOFuzzyType) type) const;

	rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
	RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;

protected:
	RDOFuzzyType(CREF(LPRDOFuzzySetDefinition) pFuzzySetDefinition);
	virtual ~RDOFuzzyType();

private:
	typedef std::map< tstring, RDOFuzzyValue > Terms;

	Terms                    m_terms;
	LPRDOFuzzySetDefinition  m_fuzzySetDefinition;
};

/*!
  \class     RDOFuzzySetDefinition
  \brief     Нечеткое множество
*/
OBJECT(RDOFuzzySetDefinition)
{
DECLARE_FACTORY(RDOFuzzySetDefinition)
public:
	virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const = 0;
	virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const = 0;

protected:
	RDOFuzzySetDefinition();
	virtual ~RDOFuzzySetDefinition();
};

/*!
  \class     RDOFuzzySetDefinitionFixed
  \brief     Установка исправленного описания нечеткой переменной
  \todo      Что это?
*/
class RDOFuzzySetDefinitionFixed: public RDOFuzzySetDefinition
{
public:
	REF(RDOFuzzySetDefinitionFixed) append     (CREF(RDOValue) value);
	REF(RDOFuzzySetDefinitionFixed) operator() (CREF(RDOValue) value);

	virtual  rbool          inRange      (CREF(RDOValue)      value) const;
	virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;

private:
	RDOFuzzySetDefinitionFixed();
	virtual ~RDOFuzzySetDefinitionFixed();

	RDOFuzzyValue::FuzzySet  m_items;
};

/*!
  \class     RDOFuzzySetDefinitionRangeDiscret
  \brief     Определение дискретного диапазона
  \todo      что это?
*/
class RDOFuzzySetDefinitionRangeDiscret: public RDOFuzzySetDefinition
{
public:
	RDOFuzzySetDefinitionRangeDiscret(CREF(RDOValue) from, CREF(RDOValue) till, CREF(RDOValue) step = 1);
	virtual ~RDOFuzzySetDefinitionRangeDiscret();

	virtual  rbool          inRange      (CREF(RDOValue)      value) const;
	virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;

private:
	RDOValue  m_from;
	RDOValue  m_till;
	RDOValue  m_step;
};

/*!
  \class     RDOFuzzyEmptyType
  \brief     Простое значение нечеткого типа
*/
class RDOFuzzyEmptyType: public RDOFuzzyType
{
public:
	static LPRDOFuzzyType getInstance();

private:
	static PTR(RDOFuzzyEmptyType) s_emptyType;

	/*!
	  \class     RDOFuzzySetDefinitionEmpty
	  \brief     Определение простого нечеткого множества
	  \todo      что это?
	*/
	class RDOFuzzySetDefinitionEmpty: public RDOFuzzySetDefinition
	{
	public:
		RDOFuzzySetDefinitionEmpty();
		virtual ~RDOFuzzySetDefinitionEmpty();

		virtual  rbool          inRange      (CREF(RDOValue) rdovalue  ) const;
		virtual  RDOFuzzyValue  getSupplement(CREF(RDOFuzzyValue) value) const;
	};

	RDOFuzzyEmptyType();

	virtual ~RDOFuzzyEmptyType();
	virtual tstring asString() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_FUZZY_DEF_H_
