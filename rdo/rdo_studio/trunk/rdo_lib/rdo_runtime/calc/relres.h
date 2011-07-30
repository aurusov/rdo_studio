/*
 * @copyright (c) RDO-Team, 2011
 * @file      relres.h
 * @authors   Барс Александр, Урусов Андрей
 * @date      28.12.09
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDOCALC_RELRES_H_
#define _RDOCALC_RELRES_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/equaltype.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOGetRelResParamCalc
// ********************************************************************************
CALC(RDOGetRelResParamCalc)
{
DECLARE_FACTORY(RDOGetRelResParamCalc)
private:
	RDOGetRelResParamCalc(ruint relResID, ruint paramID);

	ruint m_relResID;
	ruint m_paramID;

	DECALRE_ICalc;
};

// ********************************************************************************
// ******************** RDOSetRelResParamCalc
// ********************************************************************************
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

// ********************************************************************************
// ******************** RDOSetRelResParamDiapCalc
// ********************************************************************************
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

// ********************************************************************************
// ******************** RDOEraseResRelCalc
// ********************************************************************************
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

#endif // _RDOCALC_RELRES_H_
