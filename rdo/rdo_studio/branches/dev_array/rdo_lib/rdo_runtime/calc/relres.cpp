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
REF(RDOValue) RDOGetRelResParamCalc::doCalc(PTR(RDORuntime) runtime)
{
	m_value = runtime->getResParamVal(runtime->getCurrentActivity()->getResByRelRes(m_relNumb), m_parNumb);
	return m_value;
}

// ----------------------------------------------------------------------------
// ---------- RDOSetRelResParamDiapCalc
// ----------------------------------------------------------------------------
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
REF(RDOValue) RDOEraseResRelCalc::doCalc(PTR(RDORuntime) runtime)
{
	runtime->onEraseRes(runtime->getCurrentActivity()->getResByRelRes(m_rel_res_id), this);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
