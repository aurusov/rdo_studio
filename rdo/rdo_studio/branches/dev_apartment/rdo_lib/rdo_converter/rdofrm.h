/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdofrm.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOFRM_H_
#define _CONVERTOR_RDOFRM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_converter/rdofun.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int  cnv_frmparse(PTR(void) lexer);
int  cnv_frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_frmerror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
OBJECT(RDOFRMFrame) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring)                name () const  { return src_info().src_text(); }
	PTR(rdoRuntime::RDOFRMFrame) frame()        { return m_pFrame;              }

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic = NULL);

	PTR(rdoRuntime::RDOFRMFrame) m_pFrame;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOFRM_H_
