/*
 * copyright: (c) RDO-Team, 2011
 * filename : relres.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 16.04.11
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/relres.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOGetRelResParamCalc
// ----------------------------------------------------------------------------
RDOGetRelResParamCalc::RDOGetRelResParamCalc(int relNumb, int parNumb)
	: m_relNumb(relNumb)
	, m_parNumb(parNumb)
{}

REF(RDOValue) RDOGetRelResParamCalc::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelResParamDiapCalc
// ----------------------------------------------------------------------------
RDOSetRelResParamDiapCalc::RDOSetRelResParamDiapCalc(int relNumb, int parNumb, CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(LPRDOCalc) pCalc)
	: m_relNumb  (relNumb  )
	, m_parNumb  (parNumb  )
	, m_pCalc    (pCalc    )
	, m_min_value(min_value)
	, m_max_value(max_value)
{
	m_value = true;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->src_info());
	}
}

REF(RDOValue) RDOSetRelResParamDiapCalc::doCalc(PTR(RDORuntime) runtime)
{
	m_pCalc->calcValue(runtime);
	m_value = runtime->getResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb);
	if (m_value < m_min_value || m_value > m_max_value)
	{
		runtime->error(rdo::format(_T("Значение выходит за допустимый диапазон [%s..%s]: %s"), m_min_value.getAsString().c_str(), m_max_value.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOEraseResRelCalc
// ----------------------------------------------------------------------------
RDOEraseResRelCalc::RDOEraseResRelCalc(int rel_res_id, CREF(tstring) rel_res_name)
	: m_rel_res_id  (rel_res_id  )
	, m_rel_res_name(rel_res_name)
{
	m_value = 1;
}

REF(RDOValue) RDOEraseResRelCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->onEraseRes(runtime->getCurrentActivity()->getResByRelRes(m_rel_res_id), this);
	return m_value;
}

CREF(tstring) RDOEraseResRelCalc::getName() const
{
	return m_rel_res_name;
}

CLOSE_RDO_RUNTIME_NAMESPACE
