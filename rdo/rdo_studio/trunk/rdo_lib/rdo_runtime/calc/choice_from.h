/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      choice_from.h
 * @authors   Барс Александр, Урусов Андрей
 * @date      13.03.2011
 * @brief     RDOCalc для подбора релевантных ресурсов и создания ресурсов
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_CHOICE_FROM_H_
#define _LIB_RUNTIME_CALC_CHOICE_FROM_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOSelectResourceCalc
 * @brief   Выбор ресурсов
 *********************************************************************************/
CALC(RDOSelectResourceCalc)
{
public:
	/**
	 * @enum  Type
	 * @brief Способ выбора релевантного ресурса
	 */
	enum Type
	{
		order_empty = 0,
		order_first,
		order_with_min, 
		order_with_max
	};

protected:
	RDOSelectResourceCalc(int _rel_res_id, CREF(LPRDOCalc) _choice_calc, CREF(LPRDOCalc) _order_calc, Type _order_type = order_empty);

	int        rel_res_id;
	LPRDOCalc  choice_calc;
	LPRDOCalc  order_calc;
	Type       order_type;
};

/******************************************************************************//**
 * @class   RDOSelectResourceNonExistCalc
 * @brief   RDOCalc для оператора !Exist()
 *********************************************************************************/
CALC_SUB(RDOSelectResourceNonExistCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceNonExistCalc)
private:
	RDOSelectResourceNonExistCalc(int _rel_res_id);
	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOSelectResourceDirectCalc
 * @brief   unknown
 *********************************************************************************/
CALC_SUB(RDOSelectResourceDirectCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceDirectCalc)
protected:
	RDOSelectResourceDirectCalc(int _rel_res_id, int _res_id, CREF(LPRDOCalc) _choice_calc = NULL, CREF(LPRDOCalc) _order_calc = NULL, Type _order_type = order_empty);

	int res_id;

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOSelectResourceByTypeCalc
 * @brief   Выбор ресурсов по типу
 *********************************************************************************/
CALC_SUB(RDOSelectResourceByTypeCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCalc)
protected:
	RDOSelectResourceByTypeCalc(int _rel_res_id, int _resType, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type _order_type = order_empty);

	int resType;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @interface IRDOSelectResourceCommon
 * @brief     Интерфейс для выбора ресурсов оператором Select()
 *********************************************************************************/
OBJECT_INTERFACE(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(IRDOSelectResourceCommon)
public:
	virtual std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const = 0;
	virtual rbool            callChoice        (CREF(LPRDORuntime) pRuntime) const = 0;

protected:
	IRDOSelectResourceCommon();
	virtual ~IRDOSelectResourceCommon();
};

/******************************************************************************//**
 * @class   RDOSelectResourceCommonCalc
 * @brief   RDOCalc для оператора Select()
 *********************************************************************************/
CALC(RDOSelectResourceCommonCalc)
{
DECLARE_FACTORY(RDOSelectResourceCommonCalc)
private:
	typedef  std::vector<LPIRDOSelectResourceCommon>  SelectResourceCommonList;

	RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) _resSelectors, rbool _useCommonWithMax, CREF(LPRDOCalc) _choice_calc);

	LPRDOCalc                 choice_calc;
	SelectResourceCommonList  resSelectors;
	rbool                     useCommonWithMax;

	void  getBest ( REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const;
	rbool getFirst( REF(std::vector< std::vector<int> >) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOSelectResourceDirectCommonCalc
 * @brief   unknown
 *********************************************************************************/
CALC_SUB(RDOSelectResourceDirectCommonCalc, RDOSelectResourceDirectCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceDirectCommonCalc)
public:
	std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const;
	virtual rbool    callChoice        (CREF(LPRDORuntime) pRuntime) const;

private:
	RDOSelectResourceDirectCommonCalc(int _relNumb, int _resNumb, CREF(LPRDOCalc) _choice_calc = NULL, CREF(LPRDOCalc) _order_calc = NULL, Type _order_type = order_empty);
	virtual ~RDOSelectResourceDirectCommonCalc();
};

/******************************************************************************//**
 * @class   RDOSelectResourceByTypeCommonCalc
 * @brief   unknown
 *********************************************************************************/
CALC_SUB(RDOSelectResourceByTypeCommonCalc, RDOSelectResourceByTypeCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCommonCalc)
public:
	std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const;
	virtual rbool    callChoice        (CREF(LPRDORuntime) pRuntime) const;

private:
	RDOSelectResourceByTypeCommonCalc(int _relNumb, int _resType, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type _order_type = order_empty);
	virtual ~RDOSelectResourceByTypeCommonCalc();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/choice_from.inl"

#endif // _LIB_RUNTIME_CALC_CHOICE_FROM_H_
