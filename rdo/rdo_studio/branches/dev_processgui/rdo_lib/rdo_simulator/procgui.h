/*
 * copyright: (c) RDO-Team, 2011
 * filename : procgui.h
 * author   : Шувалов Андрей
 * date     : 22.04.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _PROCGUI_H_
#define _PROCGUI_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_datablock.h"
// ===============================================================================

OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------
// ---------- ProcGUIBlock
// --------------------------------------------------------------------
OBJECT(ProcGUIBlock)
{
public:
	ProcGUIBlock(CREF(rdoParse::LPRDOParser) pParser, PTR(rdoRuntime::RDORuntime) pRuntime);
	virtual ~ProcGUIBlock();

	void Create   (REF(RPShapeDataBlockCreate   ) pParams);
	void Terminate(REF(RPShapeDataBlockTerminate) pParams);
	void Process  (std::vector <double>  pParams);
	void Advance  (std::vector <double>  pParams);

private:
	rdoParse::LPRDOParser        m_pParser;
	PTR(rdoRuntime::RDORuntime)  m_pRuntime;
	LPILogic                     m_pProcess;

	rdoRuntime::LPRDOCalcConst        ProcGUIBlock::getConstCalc  (double arg                        );
	rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getNormalCalc (int p_base,double arg1,double arg2);
	rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getUniformCalc(int p_base,double arg1,double arg2);
	rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getExpCalc    (int p_base,double arg1);
};

CLOSE_RDO_SIMULATOR_NAMESPACE

#endif //! _PROCGUI_H_
