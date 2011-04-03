/*
 * copyright: (c) RDO-Team, 2011
 * filename : expression.h
 * author   : Урусов Андрей
 * date     : 07.03.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_EXPRESSION_H_
#define _RDOPARSER_EXPRESSION_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Expression
// ----------------------------------------------------------------------------
OBJECT(Expression)
{
DECLARE_FACTORY(Expression);
public:
	CREF(LPRDOType)              type      () const;
	CREF(rdoRuntime::LPRDOCalc)  calc      () const;
	CREF(rdoRuntime::RDOSrcInfo) src_info  () const;
	void                         setSrcInfo(CREF(rdoRuntime::RDOSrcInfo) src_info);

	RDOValue                     constant  () const;

private:
	Expression(CREF(LPRDOType) pType, CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(rdoRuntime::RDOSrcInfo) src_info);
	virtual ~Expression();

	LPRDOType              m_pType;
	rdoRuntime::LPRDOCalc  m_pCalc;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_EXPRESSION_H_
