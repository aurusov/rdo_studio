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
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  frmparse(PTR(void) lexer);
int  frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void frmerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
CLASS(RDOFRMFrame):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring)                     name () const  { return src_info().src_text(); }
	CREF(rdo::runtime::LPRDOFRMFrame) frame() const  { return m_pFrame;              }
	void                              end  ();

	static LPExpression generateExpression(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) srcInfo);

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo);

	rdo::runtime::LPRDOFRMFrame m_pFrame;
	LPContextMemory             m_pContextMemory;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMFrame);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite
// --------------------------------------------------------------------------------
CLASS(RDOFRMSprite):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDOFRMSprite);
public:
	CREF(tstring)                      name  () const  { return src_info().src_text(); }
	CREF(rdo::runtime::LPRDOFRMSprite) sprite() const  { return m_pSprite;             }
	void                               end  ();

private:
	RDOFRMSprite(CREF(RDOParserSrcInfo) src_info);

	rdo::runtime::LPRDOFRMSprite m_pSprite;
	LPContextMemory              m_pContextMemory;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMSprite);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOFRM_H_
