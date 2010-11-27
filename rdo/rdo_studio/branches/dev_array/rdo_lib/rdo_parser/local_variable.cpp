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
LocalVariable::LocalVariable(CREF(RDOValue) value, CREF(LPRDOFUNArithm) pArithm, CREF(LPRDOTypeParam) pParam)
	: m_value  (value  )
	, m_pArithm(pArithm)
	, m_pParam (pParam )
{}

CREF(RDOValue) LocalVariable::getValue() const
{
	return m_value;
}

LPRDOFUNArithm LocalVariable::getArithm() const
{
	return m_pArithm;
}

LPRDOTypeParam LocalVariable::getParam() const
{
	return m_pParam;
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

LPLocalVariable LocalVariableList::findLocalVariable(CREF(tstring) paramName) const
{
	STL_FOR_ALL_CONST(VariableList, m_variableList, LocalVariable_it)
	{
		if((*LocalVariable_it)->getValue()->getIdentificator() == paramName)
			return (*LocalVariable_it);
	}
	return NULL;
}

CLOSE_RDO_PARSER_NAMESPACE
