/*
 * copyright: (c) RDO-Team, 2010
 * filename : local_variable.cpp
 * author   : Урусов Андрей
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- LocalVariable
// ----------------------------------------------------------------------------
LocalVariable::LocalVariable(CREF(RDOValue) value, CREF(rdoRuntime::LPRDOCalc) pCalc)
	: m_value (value)
	, m_pCalc (pCalc)
{}

CREF(RDOValue) LocalVariable::getValue() const
{
	return m_value;
}

rdoRuntime::LPRDOCalc LocalVariable::getCalc( ) const
{
	return m_pCalc;
}

// ----------------------------------------------------------------------------
// ---------- LocalVariableList
// ----------------------------------------------------------------------------
LocalVariableList::LocalVariableList()
{}

void LocalVariableList::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(pVariable);

	STL_FOR_ALL_CONST(VariableList, m_variableList, it)
	{
		if ((*it)->getValue()->getIdentificator() == pVariable->getValue()->getIdentificator())
		{
			rdoParse::g_error().push_only(pVariable->getValue().src_info(), rdo::format(_T("Переменная %s уже объявлена"), pVariable->getValue()->getIdentificator().c_str()));
			rdoParse::g_error().push_only((*it)->getValue().src_info(),     _T("См. первое описание"));
			rdoParse::g_error().push_done();
		}
	}

	m_variableList.push_back(pVariable);
}

// CREF(rdoRuntime::LPRDOCalc) LocalVariableList::getCalc()
// {
// 	rdoRuntime::LPRDOCalcList pCalcList = rdo::Factory<rdoRuntime::RDOCalcList>::create();
// 	STL_FOR_ALL(VariableList, m_variableList, LocalVariable_it)
// 	{
// 		pCalcList->addCalc((*LocalVariable_it)->getCalc());
// 	}
// 	rdoRuntime::LPRDOCalc pCalc = pCalcList;
// 	return pCalc;
// }

CREF(rdoRuntime::LPRDOCalc) LocalVariableList::findLocalVariable(CREF(tstring) paramName) const
{
	STL_FOR_ALL_CONST(VariableList, m_variableList, LocalVariable_it)
	{
		if((*LocalVariable_it)->getValue()->getIdentificator() == paramName)
			return (*LocalVariable_it)->getCalc();
	}
	return rdoRuntime::LPRDOCalc();
}

CLOSE_RDO_PARSER_NAMESPACE
