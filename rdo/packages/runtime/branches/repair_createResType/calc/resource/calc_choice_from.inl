/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_choice_from.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      27.07.2011
  \brief     RDOCalc для подбора релевантных ресурсов и создания ресурсов
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceNonExistCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceNonExistCalc::RDOSelectResourceNonExistCalc(int relResID)
	: RDOSelectResourceCalc(relResID, NULL, NULL)
{
	m_value = 1;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceDirectCalc::RDOSelectResourceDirectCalc(int relResID, int resID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType)
	: RDOSelectResourceCalc(relResID, pCalcChoiceFrom, pCalcOrder, orderType)
	, m_resID              (resID                                           )
{}

inline rbool RDOSelectResourceDirectCalc::compare(CREF(LPRDOCalc) pCalc) const
{
	LPRDOSelectResourceDirectCalc pDirectCalc = pCalc.object_dynamic_cast<RDOSelectResourceDirectCalc>();
	if (!pDirectCalc)
	{
		return false;
	}
	return m_relResID == pDirectCalc->m_relResID && m_resID == pDirectCalc->m_resID;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceByTypeCalc::RDOSelectResourceByTypeCalc(int relResID, int resTypeID, CREF(LPRDOCalc) pChoiceCalc, CREF(LPRDOCalc) pOrderCalc, Type orderType)
	: RDOSelectResourceCalc(relResID, pChoiceCalc, pOrderCalc, orderType)
	, m_resTypeID          (resTypeID                                   )
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceCommonCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceCommonCalc::RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) resSelectorList, rbool useCommonWithMax, CREF(LPRDOCalc) pCalcChoiceFrom)
	: m_resSelectorList (resSelectorList )
	, m_useCommonWithMax(useCommonWithMax)
	, m_pCalcChoiceFrom (pCalcChoiceFrom )
{
	if (m_pCalcChoiceFrom)
	{
		setSrcInfo(m_pCalcChoiceFrom->srcInfo());
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCommonCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceDirectCommonCalc::RDOSelectResourceDirectCommonCalc(int relResID, int resID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType)
	: RDOSelectResourceDirectCalc(relResID, resID, pCalcChoiceFrom, pCalcOrder, orderType)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCommonCalc
// --------------------------------------------------------------------------------
inline RDOSelectResourceByTypeCommonCalc::RDOSelectResourceByTypeCommonCalc(int relResID, int resTypeID, CREF(LPRDOCalc) pChoiceCalc, CREF(LPRDOCalc) pOrderCalc, Type orderType)
	: RDOSelectResourceByTypeCalc(relResID, resTypeID, pChoiceCalc, pOrderCalc, orderType)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
