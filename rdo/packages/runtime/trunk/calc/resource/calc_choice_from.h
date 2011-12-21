/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_choice_from.h
  \authors   Ѕарс јлександр
  \authors   ”русов јндрей (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     RDOCalc дл€ подбора релевантных ресурсов и создани€ ресурсов
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

/*!
  \class   RDOCalcCreateResource
  \brief   —оздание нового ресурса
*/
CALC(RDOCalcCreateResource)
{
DECLARE_FACTORY(RDOCalcCreateResource)
private:
	//! relResID == 0 дл€ ресурсов, создаваемых при инициализации модели
	RDOCalcCreateResource(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag, ruint relResID = 0);

	LPIResourceType        m_pResType;
	std::vector<RDOValue>  m_paramsCalcs;
	rbool                  m_traceFlag;
	rbool                  m_permanentFlag;
	ruint                  m_relResID;

	DECLARE_ICalc;
};

/*!
  \class   RDOSelectResourceCalc
  \brief   ¬ыбор ресурсов
*/
CALC(RDOSelectResourceCalc)
{
public:
	//! —пособ выбора релевантного ресурса
	enum Type
	{
		order_empty = 0, //!< —пособ выбора не задан
		order_first,     //!< ѕервый попавшийс€ (без предварительной сортировки)
		order_with_min,  //!< — минимальным значением выражени€
		order_with_max   //!< — максимальным значением выражени€
	};

protected:
	RDOSelectResourceCalc(int relResID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType = order_empty);

	int        m_relResID;
	LPRDOCalc  m_pCalcChoiceFrom;
	LPRDOCalc  m_pCalcOrder;
	Type       m_orderType;
};

/*!
  \class   RDOSelectResourceNonExistCalc
  \brief   RDOCalc дл€ оператора !Exist()
*/
CALC_SUB(RDOSelectResourceNonExistCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceNonExistCalc)
private:
	RDOSelectResourceNonExistCalc(int relResID);
	DECLARE_ICalc;
};

/*!
  \class   RDOSelectResourceDirectCalc
  \brief   RDOCalc дл€ пр€мого вычислени€ оператора Select()
  \todo    что это?
*/
CALC_SUB(RDOSelectResourceDirectCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceDirectCalc)
protected:
	RDOSelectResourceDirectCalc(int relResID, int resID, CREF(LPRDOCalc) pCalcChoiceFrom = NULL, CREF(LPRDOCalc) pCalcOrder = NULL, Type orderType = order_empty);

	int m_resID;

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

	DECLARE_ICalc;
};

/*!
  \class   RDOSelectResourceByTypeCalc
  \brief   ¬ыбор ресурсов по типу
*/
CALC_SUB(RDOSelectResourceByTypeCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCalc)
protected:
	RDOSelectResourceByTypeCalc(int relResID, int resTypeID, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type orderType = order_empty);

	int m_resTypeID;

	DECLARE_ICalc;
};

/*!
  \interface IRDOSelectResourceCommon
  \brief     »нтерфейс дл€ выбора ресурсов оператором Select()
*/
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

/*!
  \class   RDOSelectResourceCommonCalc
  \brief   RDOCalc дл€ оператора Select()
*/
CALC(RDOSelectResourceCommonCalc)
{
DECLARE_FACTORY(RDOSelectResourceCommonCalc)
private:
	typedef  std::vector<LPIRDOSelectResourceCommon>  SelectResourceCommonList;

	RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) resSelectorList, rbool useCommonWithMax, CREF(LPRDOCalc) pCalcChoiceFrom);

	LPRDOCalc                 m_pCalcChoiceFrom;
	SelectResourceCommonList  m_resSelectorList;
	rbool                     m_useCommonWithMax;

	void  getBest ( REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const;
	rbool getFirst( REF(std::vector< std::vector<int> >) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const;

	DECLARE_ICalc;
};

/*!
  \class   RDOSelectResourceDirectCommonCalc
  \brief   RDOCalc дл€ общего пр€мого вычислени€ оператора Select()
  \todo    что это?
*/
CALC_SUB(RDOSelectResourceDirectCommonCalc, RDOSelectResourceDirectCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceDirectCommonCalc)
public:
	std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const;
	virtual rbool    callChoice        (CREF(LPRDORuntime) pRuntime) const;

private:
	RDOSelectResourceDirectCommonCalc(int relResID, int resID, CREF(LPRDOCalc) pCalcChoiceFrom = NULL, CREF(LPRDOCalc) pCalcOrder = NULL, Type orderType = order_empty);
	virtual ~RDOSelectResourceDirectCommonCalc();
};

/*!
  \class   RDOSelectResourceByTypeCommonCalc
  \brief   RDOCalc дл€ общего вычислени€ оператора Select() по типу
  \todo    что это?
*/
CALC_SUB(RDOSelectResourceByTypeCommonCalc, RDOSelectResourceByTypeCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCommonCalc)
public:
	std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const;
	virtual rbool    callChoice        (CREF(LPRDORuntime) pRuntime) const;

private:
	RDOSelectResourceByTypeCommonCalc(int relResID, int resTypeID, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type orderType = order_empty);
	virtual ~RDOSelectResourceByTypeCommonCalc();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/resource/calc_choice_from.inl"

#endif // _LIB_RUNTIME_CALC_RESOURCE_CHOICE_FROM_H_
