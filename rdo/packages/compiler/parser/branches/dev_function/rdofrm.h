/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOFRM_H_
#define _RDOFRM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/compiler/parser/context/context_param_definition.h"
#include "simulator/compiler/parser/type/function_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  frmparse(PTR(void) lexer);
int  frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void frmerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMCommandList
// --------------------------------------------------------------------------------
class RDOFRMCommandList
	: public RDOParserSrcInfo
	, public ContextParamDefinition
	, public IContextFind
{
DECLARE_FACTORY(RDOFRMCommandList);
public:
	CREF(tstring)                        name() const  { return src_info().src_text(); }
	virtual rdo::runtime::LPRDOFRMSprite list() const = 0;

	static LPExpression generateExpression(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) srcInfo);

protected:
	RDOFRMCommandList(CREF(RDOParserSrcInfo) src_info);
};
DECLARE_POINTER(RDOFRMCommandList);


// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite
// --------------------------------------------------------------------------------
CLASS(RDOFRMSprite):
	INSTANCE_OF (RDOFRMCommandList)
{
DECLARE_FACTORY(RDOFRMSprite);
public:
	void end();

	CREF(rdo::runtime::LPRDOFRMSprite) sprite() const { return m_pSprite; }

	LPExpression expression() const;

private:
	RDOFRMSprite(CREF(RDOParserSrcInfo) src_info);

	rdo::runtime::LPRDOFRMSprite  m_pSprite;
	LPContextMemory               m_pContextMemory;
	LPFunctionType                m_pFunctionType;

	rdo::runtime::LPRDOFRMSprite list() const { return m_pSprite; }

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMSprite);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
CLASS(RDOFRMFrame):
	INSTANCE_OF (RDOFRMCommandList)
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	void                              end  ();
	CREF(rdo::runtime::LPRDOFRMFrame) frame() const { return m_pFrame; }

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo);
	
	rdo::runtime::LPRDOFRMFrame m_pFrame;
	LPContextMemory             m_pContextMemory;

	rdo::runtime::LPRDOFRMSprite list() const { return m_pFrame; }

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOFRM_H_
