/*!
  \copyright (c) RDO-Team, 2009
  \file      relres.h
  \authors   Барс Александр, Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.12.2009
  \brief     RDOCalc для подбора релевантных ресурсов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_REL_RES_H_
#define _LIB_RUNTIME_CALC_REL_RES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/equaltype.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOGetRelResParamCalc
  \brief   Параметр релевантного ресурса
*/
CALC(RDOGetRelResParamCalc)
{
DECLARE_FACTORY(RDOGetRelResParamCalc)
private:
	RDOGetRelResParamCalc(ruint relResID, ruint paramID);

	ruint m_relResID;
	ruint m_paramID;

	DECALRE_ICalc;
};

/*!
  \class   RDOSetRelResParamCalc
  \tparam  equalType Тип присваивания
  \brief   RDOCalc для установки значения параметра релевантного ресурса
*/
template <EqualType equalType>
class RDOSetRelResParamCalc: public RDOCalc
{
public:
	RDOSetRelResParamCalc(ruint relResID, ruint paramID, CREF(LPRDOCalc) pCalc = NULL);
	virtual ~RDOSetRelResParamCalc();

protected:
	ruint     m_relResID;
	ruint     m_paramID;
	LPRDOCalc m_pCalc;

	DECALRE_ICalc;
};

/*!
  \class   RDOSetRelResParamDiapCalc
  \brief   RDOCalc ограничения диапазона параметра
*/
CALC(RDOSetRelResParamDiapCalc)
{
DECLARE_FACTORY(RDOSetRelResParamDiapCalc)
private:
	RDOSetRelResParamDiapCalc(ruint relResID, ruint paramID, CREF(RDOValue) minValue, CREF(RDOValue) maxValue, CREF(LPRDOCalc) pCalc);

	ruint     m_relResID;
	ruint     m_paramID;
	LPRDOCalc m_pCalc;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	DECALRE_ICalc;
};

/*!
  \class   RDOEraseResRelCalc
  \brief   RDOCalc для удаления релевантного ресурса
*/
CALC(RDOEraseResRelCalc)
{
DECLARE_FACTORY(RDOEraseResRelCalc)
public:
	CREF(tstring) getName() const;

private:
	RDOEraseResRelCalc(ruint relResID, CREF(tstring) relResName);

	ruint   m_relResID;
	tstring m_relResName;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/relres.inl"

#endif // _LIB_RUNTIME_CALC_REL_RES_H_
