/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoopr.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date
  \brief
  \indent    4T
*/

#ifndef _CONVERTOR_RDOOPR_H_
#define _CONVERTOR_RDOOPR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdodpt.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOOperations;
CLOSE_RDO_RUNTIME_NAMESPACE

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int  cnv_oprparse(void* lexer);
int  cnv_oprlex  (YYSTYPE* lpval, YYLTYPE* llocp, void* lexer);
void cnv_oprerror(YYLTYPE* llocp, void* lexer, const char* message);

// --------------------------------------------------------------------------------
// -------------------- RDOOPROperation
// --------------------------------------------------------------------------------
class RDOOPROperation: public RDODPTActivityHotKey
{
DECLARE_FACTORY(RDOOPROperation);
private:
	RDOOPROperation(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info);
};
DECLARE_POINTER(RDOOPROperation);

// --------------------------------------------------------------------------------
// -------------------- RDOOperations
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOOperations);
class RDOOperations
	: public rdo::counter_reference
	, public RDOLogicActivity<rdo::runtime::RDOOperations, RDOOPROperation>
{
DECLARE_FACTORY(RDOOperations);
private:
	RDOOperations(const RDOParserSrcInfo& src_info);
};

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDOOPR_H_
