/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      choice_from.inl
 * @authors   Барс Александр, Урусов Андрей
 * @date      27.07.2011
 * @brief     RDOCalc для подбора релевантных ресурсов и создания ресурсов
 * @indent    4T
 *********************************************************************************/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceNonExistCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceNonExistCalc::RDOSelectResourceNonExistCalc(int _rel_res_id)
	: RDOSelectResourceCalc(_rel_res_id, NULL, NULL)
{
	m_value = 1;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceDirectCalc::RDOSelectResourceDirectCalc(int _rel_res_id, int _res_id, CREF(LPRDOCalc) _choice_calc, CREF(LPRDOCalc) _order_calc, Type _order_type)
	: RDOSelectResourceCalc(_rel_res_id, _choice_calc, _order_calc, _order_type)
	, res_id               (_res_id                                            )
{}

inline rbool RDOSelectResourceDirectCalc::compare(CREF(LPRDOCalc) pCalc) const
{
	LPRDOSelectResourceDirectCalc pDirectCalc = pCalc.object_dynamic_cast<RDOSelectResourceDirectCalc>();
	if (!pDirectCalc)
	{
		return false;
	}
	return rel_res_id == pDirectCalc->rel_res_id && res_id == pDirectCalc->res_id;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceByTypeCalc::RDOSelectResourceByTypeCalc(int _rel_res_id, int _resType, CREF(LPRDOCalc) pChoiceCalc, CREF(LPRDOCalc) pOrderCalc, Type _order_type)
	: RDOSelectResourceCalc(_rel_res_id, pChoiceCalc, pOrderCalc, _order_type)
	, resType              (_resType                                         )
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceCommonCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceCommonCalc::RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) _resSelectors, rbool _useCommonWithMax, CREF(LPRDOCalc) _choice_calc)
	: resSelectors    (_resSelectors    )
	, useCommonWithMax(_useCommonWithMax)
	, choice_calc     (_choice_calc     )
{
	if (choice_calc)
		setSrcInfo(choice_calc->src_info());
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCommonCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceDirectCommonCalc::RDOSelectResourceDirectCommonCalc(int _relNumb, int _resNumb, CREF(LPRDOCalc) _choice_calc, CREF(LPRDOCalc) _order_calc, Type _order_type)
	: RDOSelectResourceDirectCalc(_relNumb, _resNumb, _choice_calc, _order_calc, _order_type)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCommonCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceByTypeCommonCalc::RDOSelectResourceByTypeCommonCalc(int _relNumb, int _resType, CREF(LPRDOCalc) pChoiceCalc, CREF(LPRDOCalc) pOrderCalc, Type _order_type)
	: RDOSelectResourceByTypeCalc(_relNumb, _resType, pChoiceCalc, pOrderCalc, _order_type)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
