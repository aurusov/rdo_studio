/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic_base.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      5.02.2012
  \brief     Базовый класс для точек принятия решений
  \indent    4T
*/

#ifndef _PARSER_LOGIC_BASE_H_
#define _PARSER_LOGIC_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/compiler/parser/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

OBJECT_VIRTUAL(RDOLogicBase)
	IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOLogicBase);
public:
	CREF(tstring) name             () const;
	rbool         setPrior         (REF(LPRDOFUNArithm) pPrior);
	rbool         getMultithreading() const;
	void          setMultithreading(rbool multithreading = false);
	void          setCondition     (CREF(LPRDOFUNLogic) pConditon = NULL);
	LPRDOFUNLogic getConditon      () const;
	LPILogic      getLogic         () const;
	virtual void  end              ();

protected:
	RDOLogicBase(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOLogicBase();

	LPRDOFUNLogic m_pConditon;
	LPILogic      m_pRuntimeLogic;
	rbool         m_multithreading;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _PARSER_LOGIC_BASE_H_
