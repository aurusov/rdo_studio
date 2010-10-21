/*
 * copyright: (c) RDO-Team, 2010
 * filename : global.cpp
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/global.h"
#include "rdo_lib/rdo_parser/context/timenow.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

ContextGlobal::ContextGlobal()
{
	m_timeNow = rdo::Factory<ContextTimeNow>::create();
	ASSERT(m_timeNow);
}

ContextGlobal::~ContextGlobal()
{}

LPContext ContextGlobal::parser(PTR(RDOParser) pParser, CREF(RDOValue) value)
{
	ASSERT(value.typeID() == rdoRuntime::RDOType::t_identificator);

	if (value->getIdentificator() == _T("Time_now") || value->getIdentificator() == _T("time_now") || value->getIdentificator() == _T("Системное_время") || value->getIdentificator() == _T("системное_время"))
	{
		return m_timeNow;
	}

	NEVER_REACH_HERE;

	return LPContext();

	//else if ( value->getIdentificator() == "Terminate_counter" || value->getIdentificator() == "terminate_counter" )
	//{
	//	m_value = rdo::Factory<RDOType__int>::create().cast<RDOType>();
	//	m_calc = new rdoRuntime::RDOCalcGetTermNow( parser()->runtime() );
	//	m_calc->setSrcInfo( src_info() );
	//	return;
	//}
	//else if ( value->getIdentificator() == "Seconds" || value->getIdentificator() == "seconds" )
	//{
	//	m_value = rdo::Factory<RDOType__real>::create().cast<RDOType>();
	//	m_calc = new rdoRuntime::RDOCalcGetSeconds( parser()->runtime() );
	//	m_calc->setSrcInfo( src_info() );
	//	return;
	//}
}

rdoRuntime::LPRDOCalc ContextGlobal::getCalc()
{
	NEVER_REACH_HERE;
	return rdoRuntime::LPRDOCalc();
}

LPLocalVariableList ContextGlobal::getLocalMemory()
{
	return LPLocalVariableList();
}

LPLocalVariableType ContextGlobal::getLocalVariableType()
{
	return LPLocalVariableType();
}

CLOSE_RDO_PARSER_NAMESPACE
