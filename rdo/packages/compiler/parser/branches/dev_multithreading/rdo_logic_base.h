/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic_base.h
  \author    Клеванец Игорь (impus@hotbox.ru)
  \date      5.02.2012
  \brief     
  \indent    4T
*/

#ifndef _PARSER_LOGIC_BASE_H_
#define _PARSER_LOGIC_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
#include "simulator/compiler/parser/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSE_NAMESPACE

class RDOLogicBase: public RDOParserSrcInfo
{
public:
	RDOLogicBase(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	{}
	virtual ~RDOLogicBase()
	{}

	CREF(tstring) name              () const;
	rbool         setPrior          (REF(LPRDOFUNArithm) pPrior);
	rbool         getMultithreading () const;
	void          setMultithreading (rbool multithreading);
	void          setCondition      (CREF(LPRDOFUNLogic) pConditon = NULL);
	LPRDOFUNLogic getConditon       () const;
	LPILogic      getLogic          () const;

protected:
	LPRDOFUNLogic m_pConditon;
	LPILogic      m_pRuntimeLogic;
	rbool         m_multithreading;
};

CLOSE_RDO_PARSE_NAMESPACE

#endif // _PARSER_LOGIC_BASE_H_
