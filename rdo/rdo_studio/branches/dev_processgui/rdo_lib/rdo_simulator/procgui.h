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
// ---------- ProcGUICalc
// --------------------------------------------------------------------
class ProcGUICalc
{
public:
	ProcGUICalc(PTR(rdoRuntime::RDORuntime) pRuntime);
	virtual ~ProcGUICalc();

	rdoRuntime::LPRDOCalcConst        ProcGUICalc::getConstCalc  (double arg                        );
	rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getNormalCalc (int p_base,double arg1,double arg2);
	rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getUniformCalc(int p_base,double arg1,double arg2);
	rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getExpCalc    (int p_base,double arg1            );

private:
	PTR(rdoRuntime::RDORuntime)  m_pRuntime;
};

// --------------------------------------------------------------------
// ---------- ProcGUIProcess
// --------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlock);

OBJECT(ProcGUIProcess)
{
public:
	static tstring s_name_prefix;
	static tstring s_name_sufix;

	ProcGUIProcess(PTR(rdoRuntime::RDORuntime) pRuntime);

	void     insertBlock(CREF(LPProcGUIBlock) pBlock);
	LPILogic getProcess (                              ) {return m_pProcess;}

	virtual ~ProcGUIProcess();

private:
	PTR(rdoRuntime::RDORuntime)  m_pRuntime;
	LPILogic                     m_pProcess;
	std::list<LPProcGUIBlock> m_blockList;
};

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlock
// ----------------------------------------------------------------------------
OBJECT(ProcGUIBlock)
{
DECLARE_FACTORY(ProcGUIBlock);
protected:
	tstring          m_name;
	LPProcGUIProcess m_pProcess;

	ProcGUIBlock(CREF(LPProcGUIProcess) pProcess, CREF(tstring) pName);
	virtual ~ProcGUIBlock();
};

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockGenerate
// ----------------------------------------------------------------------------
class ProcGUIBlockGenerate: public ProcGUIBlock, public ProcGUICalc
{
DECLARE_FACTORY(ProcGUIBlockGenerate);
protected:
	LPIPROCBlock             m_pBlock;
	rdoParse::LPRDOParser    m_pParser;
	LPRPShapeDataBlockCreate m_pParams;

private:
	ProcGUIBlockGenerate(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockCreate) pParams /*CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc*/);
};

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockTerminate
// ----------------------------------------------------------------------------
class ProcGUIBlockTerminate: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIBlockTerminate);
protected:
	LPIPROCBlock                m_pBlock;
	LPRPShapeDataBlockTerminate m_pParams;

private:
	ProcGUIBlockTerminate(CREF(LPProcGUIProcess) pProcess, CREF(LPRPShapeDataBlockTerminate) pParams);
};

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockProcess
// ----------------------------------------------------------------------------
class ProcGUIBlockProcess: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIBlockProcess);
private:
	LPRPShapeDataBlockProcess m_pParams;
	LPProcGUIBlock            m_pBlock;

	ProcGUIBlockProcess(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams);
};

// ----------------------------------------------------------------------------
// ---------- ProcGUIAdvance
// ----------------------------------------------------------------------------
class ProcGUIAdvance: public ProcGUIBlock, public ProcGUICalc
{
DECLARE_FACTORY(ProcGUIAdvance);
private:
	ProcGUIAdvance(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(LPRPShapeDataBlockProcess) pParams);

	LPRPShapeDataBlockProcess m_pParams;
	LPIPROCBlock              m_pBlock;
};
DECLARE_POINTER(ProcGUIAdvance);

// ----------------------------------------------------------------------------
// ---------- ProcGUISeize
// ----------------------------------------------------------------------------
class ProcGUISeize: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUISeize);
public:
	void createRuntime();
	void addResource  (CREF(tstring) name);

private:
	ProcGUISeize(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams);

	rdoParse::LPRDOParser     m_pParser;
	LPIPROCBlock              m_pBlock;
	LPRPShapeDataBlockProcess m_pParams;
	std::list  <tstring>      m_resList;
	std::vector<rdoRuntime::parser_for_Seize>  m_parserForRuntime;
};
DECLARE_POINTER(ProcGUISeize);

// ----------------------------------------------------------------------------
// ---------- ProcGUIRelease
// ----------------------------------------------------------------------------
class ProcGUIRelease: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIRelease);
public:
	void createRuntime();
	void addResource  (CREF(tstring) name);

private:
	ProcGUIRelease(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams);

	rdoParse::LPRDOParser     m_pParser;
	LPIPROCBlock              m_pBlock;
	LPRPShapeDataBlockProcess m_pParams;
	std::list  <tstring>      m_resList;
	std::vector<rdoRuntime::parser_for_Seize>  m_parserForRuntime;
};
DECLARE_POINTER(ProcGUIRelease);

// ----------------------------------------------------------------------------
// ---------- ProcGUIQueue
// ----------------------------------------------------------------------------
class ProcGUIQueue: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIQueue);
public:
	void createRuntime();
	void setResource  (CREF(tstring) name);

private:
	ProcGUIQueue(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(tstring) name);

	tstring                   m_resourceName;
	rdoParse::LPRDOParser     m_pParser;
	LPIPROCBlock              m_pBlock;
	rdoRuntime::parser_for_Queue m_parserForRuntime;
};
DECLARE_POINTER(ProcGUIQueue);

// ----------------------------------------------------------------------------
// ---------- ProcGUIDepart
// ----------------------------------------------------------------------------
class ProcGUIDepart: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIDepart);
public:
	void createRuntime();
	void setResource  (CREF(tstring) name);

private:
	ProcGUIDepart(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(tstring) name);

	tstring                   m_resourceName;
	rdoParse::LPRDOParser     m_pParser;
	LPIPROCBlock              m_pBlock;
	rdoRuntime::parser_for_Queue m_parserForRuntime;
};
DECLARE_POINTER(ProcGUIDepart);

CLOSE_RDO_SIMULATOR_NAMESPACE

#endif //! _PROCGUI_H_
