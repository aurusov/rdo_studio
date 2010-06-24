/*
 * copyright: (c) RDO-Team, 2010
 * filename : timenow.inl
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

LPContext ContextTimeNow::parser(PTR(RDOParser) pParser, CREF(RDOValue) value)
{
	NEVER_REACH_HERE;
	return LPContext();
}

rdoRuntime::LPRDOCalc ContextTimeNow::getCalc()
{
	if (!m_pCalc)
	{
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetTimeNow>::create();
		ASSERT(m_pCalc);
	}
	return m_pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE
