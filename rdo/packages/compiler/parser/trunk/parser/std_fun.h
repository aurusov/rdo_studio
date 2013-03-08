/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/parser/std_fun.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.04.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_STD_FUN_H_
#define _RDOPARSER_STD_FUN_H_

// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserSTDFUN
// --------------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSTDFUN);
private:
	RDOParserSTDFUN()
		: RDOParserItem(rdoModelObjects::FUN, NULL, NULL, NULL)
	{}

	virtual void parse(CREF(LPRDOParser) pParser);

	typedef rdo::vector<LPTypeInfo> ParamList;
	void generate    (CREF(tstring) name, CREF(rdo::runtime::LPRDOFunCalc) pCalc, CREF(LPRDOParam) pReturnType, CREF(ParamList) paramList) const;
	void generateReal(CREF(tstring) name, CREF(rdo::runtime::LPRDOFunCalc) pCalc, CREF(LPRDOParam) pReturnType, CREF(ParamList) paramList) const;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_STD_FUN_H_
