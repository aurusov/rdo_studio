/*
 * copyright: (c) RDO-Team, 2011
 * filename : choice_from.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_CHOICE_FROM_H_
#define _RDOCALC_CHOICE_FROM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Выбор ресурсов
// ----------------------------------------------------------------------------
CALC(RDOSelectResourceCalc)
{
public:
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

CALC_SUB(RDOSelectResourceNonExistCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceNonExistCalc)
private:
	RDOSelectResourceNonExistCalc(int _rel_res_id)
		: RDOSelectResourceCalc(_rel_res_id, NULL, NULL)
	{
		m_value = 1;
	}
	DECALRE_ICalc;
};

CALC_SUB(RDOSelectResourceDirectCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceDirectCalc)
protected:
	RDOSelectResourceDirectCalc(int _rel_res_id, int _res_id, CREF(LPRDOCalc) _choice_calc = NULL, CREF(LPRDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceCalc(_rel_res_id, _choice_calc, _order_calc, _order_type)
		, res_id               (_res_id                                            )
	{}

	int res_id;

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const
	{
		LPRDOSelectResourceDirectCalc pDirectCalc = pCalc.object_dynamic_cast<RDOSelectResourceDirectCalc>();
		if (!pDirectCalc)
		{
			return false;
		}
		return rel_res_id == pDirectCalc->rel_res_id && res_id == pDirectCalc->res_id;
	}

	DECALRE_ICalc;
};

CALC_SUB(RDOSelectResourceByTypeCalc, RDOSelectResourceCalc)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCalc)
protected:
	RDOSelectResourceByTypeCalc(int _rel_res_id, int _resType, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceCalc(_rel_res_id, pChoiceCalc, pOrderCalc, _order_type)
		, resType              (_resType                                         )
	{}

	int resType;

	DECALRE_ICalc;
};

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

CALC(RDOSelectResourceCommonCalc)
{
DECLARE_FACTORY(RDOSelectResourceCommonCalc)
private:
	typedef  std::vector<LPIRDOSelectResourceCommon>  SelectResourceCommonList;

	RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) _resSelectors, rbool _useCommonWithMax, CREF(LPRDOCalc) _choice_calc)
		: resSelectors    (_resSelectors    )
		, useCommonWithMax(_useCommonWithMax)
		, choice_calc     (_choice_calc     )
	{
		if (choice_calc) setSrcInfo(choice_calc->src_info());
	}

	LPRDOCalc                 choice_calc;
	SelectResourceCommonList  resSelectors;
	rbool                     useCommonWithMax;

	void  getBest ( REF(std::vector< std::vector<int> >) allNumbs, ruint level, REF(std::vector<int>) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const;
	rbool getFirst( REF(std::vector< std::vector<int> >) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const;

	DECALRE_ICalc;
};

CALC_SUB(RDOSelectResourceDirectCommonCalc, RDOSelectResourceDirectCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceDirectCommonCalc)
public:
	std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const;
	virtual rbool    callChoice        (CREF(LPRDORuntime) pRuntime) const;

private:
	RDOSelectResourceDirectCommonCalc(int _relNumb, int _resNumb, CREF(LPRDOCalc) _choice_calc = NULL, CREF(LPRDOCalc) _order_calc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceDirectCalc(_relNumb, _resNumb, _choice_calc, _order_calc, _order_type)
	{}
	virtual ~RDOSelectResourceDirectCommonCalc();
};

CALC_SUB(RDOSelectResourceByTypeCommonCalc, RDOSelectResourceByTypeCalc)
	AND IMPLEMENTATION_OF(IRDOSelectResourceCommon)
{
DECLARE_FACTORY(RDOSelectResourceByTypeCommonCalc)
public:
	std::vector<int> getPossibleNumbers(CREF(LPRDORuntime) pRuntime) const;
	virtual rbool    callChoice        (CREF(LPRDORuntime) pRuntime) const;

private:
	RDOSelectResourceByTypeCommonCalc(int _relNumb, int _resType, CREF(LPRDOCalc) pChoiceCalc = NULL, CREF(LPRDOCalc) pOrderCalc = NULL, Type _order_type = order_empty)
		: RDOSelectResourceByTypeCalc(_relNumb, _resType, pChoiceCalc, pOrderCalc, _order_type)
	{}
	virtual ~RDOSelectResourceByTypeCommonCalc();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_CHOICE_FROM_H_
