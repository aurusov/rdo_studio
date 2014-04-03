/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_choice_from.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     RDOCalc для подбора релевантных ресурсов и создания ресурсов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_CHOICE_FROM_H_
#define _LIB_RUNTIME_CALC_RESOURCE_CHOICE_FROM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Выбор ресурсов
CALC(RDOSelectResourceCalc)
{
public:
	//! Способ выбора релевантного ресурса
	enum Type
	{
		order_empty = 0, //!< Способ выбора не задан
		order_first,     //!< Первый попавшийся (без предварительной сортировки)
		order_with_min,  //!< С минимальным значением выражения
		order_with_max   //!< С максимальным значением выражения
	};

	typedef  ruint                        ResourceID;
	typedef  std::vector<ResourceID>      ResourceIDList;
	typedef  std::vector<ResourceIDList>  ResourceIDTable;

protected:
	RDOSelectResourceCalc(ResourceID relResID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType = order_empty);

	ResourceID  m_relResID;
	LPRDOCalc   m_pCalcChoiceFrom;
	LPRDOCalc   m_pCalcOrder;
	Type        m_orderType;
};

//! RDOCalc для оператора !Exist()
CALC_SUB(RDOSelectResourceNonExistCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceNonExistCalc)
private:
	RDOSelectResourceNonExistCalc(ResourceID relResID);
	DECLARE_ICalc;
};

//! Выбор релевантного ресурса по имени ресурса
CALC_SUB(RDOSelectResourceDirectCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceDirectCalc)
protected:
	RDOSelectResourceDirectCalc(ResourceID relResID, ResourceID resID, CREF(LPRDOCalc) pCalcChoiceFrom = NULL, CREF(LPRDOCalc) pCalcOrder = NULL, Type orderType = order_empty);

	ResourceID m_resID;

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

	DECLARE_ICalc;
};

//! Выбор релевантного ресурса по типу
CALC_SUB(RDOSelectResourceByTypeCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCalc)
protected:
	RDOSelectResourceByTypeCalc(ResourceID relResID, ResourceID resTypeID, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type orderType = order_empty);

	ResourceID m_resTypeID;

	DECLARE_ICalc;
};

//! Интерфейс для выбора релевантных ресурсов
OBJECT_INTERFACE(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(IRDOSelectResourceCommon)
public:
	virtual void  getPossibleNumbers(CREF(LPRDORuntime)                          pRuntime,
	                                  REF(RDOSelectResourceCalc::ResourceIDList) resourceIDList) const = 0;
	virtual rbool callChoice        (CREF(LPRDORuntime) pRuntime)                                const = 0;

protected:
	IRDOSelectResourceCommon();
	virtual ~IRDOSelectResourceCommon();
};

//! Выбор
CALC(RDOSelectResourceCommonCalc)
{
DECLARE_FACTORY(RDOSelectResourceCommonCalc)
private:
	typedef  std::vector<LPIRDOSelectResourceCommon>  SelectResourceCommonList;
	typedef  RDOSelectResourceCalc::ResourceIDList    ResourceIDList;
	typedef  RDOSelectResourceCalc::ResourceIDTable   ResourceIDTable;

	RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) resSelectorList, rbool useCommonWithMax, CREF(LPRDOCalc) pCalcChoiceFrom);

	LPRDOCalc                 m_pCalcChoiceFrom;
	SelectResourceCommonList  m_resSelectorList;
	rbool                     m_useCommonWithMax;

	void  getBest (REF(ResourceIDTable) allNumbs, ruint level, REF(ResourceIDList) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const;
	rbool getFirst(REF(ResourceIDTable) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const;

	DECLARE_ICalc;
};

//! Выбор по имени ресурса
CALC_SUB(RDOSelectResourceDirectCommonCalc, RDOSelectResourceDirectCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceDirectCommonCalc)
public:
	virtual void  getPossibleNumbers(CREF(LPRDORuntime) pRuntime, REF(ResourceIDList) resourceIDList) const;
	virtual rbool callChoice        (CREF(LPRDORuntime) pRuntime)                                     const;

private:
	RDOSelectResourceDirectCommonCalc(ResourceID relResID, ResourceID resID, CREF(LPRDOCalc) pCalcChoiceFrom = NULL, CREF(LPRDOCalc) pCalcOrder = NULL, Type orderType = order_empty);
	virtual ~RDOSelectResourceDirectCommonCalc();
};

//! Выбор по типу
CALC_SUB(RDOSelectResourceByTypeCommonCalc, RDOSelectResourceByTypeCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCommonCalc)
public:
	virtual void  getPossibleNumbers(CREF(LPRDORuntime) pRuntime, REF(ResourceIDList) resourceIDList) const;
	virtual rbool callChoice        (CREF(LPRDORuntime) pRuntime)                                     const;

private:
	RDOSelectResourceByTypeCommonCalc(ResourceID relResID, ResourceID resTypeID, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type orderType = order_empty);
	virtual ~RDOSelectResourceByTypeCommonCalc();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_CHOICE_FROM_H_
