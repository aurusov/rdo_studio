#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/procgui/namespace.h"
#include "simulator/compiler/procgui/procgui_datablock.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_GUI_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ProcGUICalc
// --------------------------------------------------------------------------------
class ProcGUICalc
{
public:
    rdo::runtime::LPRDOCalcConst getConstCalc(double arg);
    rdo::runtime::LPRDOCalcFunctionCaller getNormalCalc(std::size_t base, double arg1, double arg2);
    rdo::runtime::LPRDOCalcFunctionCaller getUniformCalc(std::size_t base, double arg1, double arg2);
    rdo::runtime::LPRDOCalcFunctionCaller getExpCalc(std::size_t base, double arg);
    rdo::runtime::LPRDOCalcFunctionCaller getTriangularCalc(std::size_t base, double arg1, double arg2, double arg3);
    rdo::runtime::LPRDOCalc getCalc();

protected:
    ProcGUICalc(const rdo::runtime::LPRDORuntime& pRuntime, const LPRPShapeDataBlock& pParams);
    virtual ~ProcGUICalc();

private:
    rdo::runtime::LPRDORuntime m_pRuntime;
    LPRPShapeDataBlock m_pParams;
    rdo::runtime::LPRDOCalc m_pCalc;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlock);

PREDECLARE_POINTER(ProcGUIProcess);
class ProcGUIProcess: public rdo::counter_reference
{
DECLARE_FACTORY(ProcGUIProcess);
public:
    typedef std::list<LPProcGUIBlock> BlockList;
    typedef std::list<std::string> ResNameList;

    static std::string s_namePrefix;
    static std::string s_nameSufix;

    void clear();
    void insertBlock(const LPProcGUIBlock& pBlock );
    void initResources(const parser::LPRDOParser& pParser);
    void addResNameToBlock(const std::string& name);
    const LPILogic& getProcess () const { return m_pProcess; }

private:
    ProcGUIProcess(const rdo::runtime::LPRDORuntime& pRuntime);
    virtual ~ProcGUIProcess();

    rdo::runtime::LPRDORuntime  m_pRuntime;
    LPILogic                    m_pProcess;
    BlockList                   m_blockList;
    ResNameList                 m_resNameList;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlock
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlock);
class ProcGUIBlock: public rdo::counter_reference
{
DECLARE_FACTORY(ProcGUIBlock);
protected:
    ProcGUIBlock(const LPProcGUIProcess& pProcess, const std::string& name);
    virtual ~ProcGUIBlock();

private:
    std::string m_name;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlockGenerate
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlockGenerate);
class ProcGUIBlockGenerate
    : public ProcGUIBlock
    , public ProcGUICalc
{
DECLARE_FACTORY(ProcGUIBlockGenerate);
private:
    ProcGUIBlockGenerate(const LPProcGUIProcess& pProcess, const rdo::runtime::LPRDORuntime& pRuntime, const parser::LPRDOParser& pParser, const LPRPShapeDataBlockCreate& pParams);
    virtual ~ProcGUIBlockGenerate();

    LPIPROCBlock             m_pBlock;
    LPRPShapeDataBlockCreate m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlockTerminate
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlockTerminate);
class ProcGUIBlockTerminate: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIBlockTerminate);
private:
    ProcGUIBlockTerminate(const LPProcGUIProcess& pProcess, const LPRPShapeDataBlockTerminate& pParams);
    virtual ~ProcGUIBlockTerminate();

    LPIPROCBlock                m_pBlock;
    LPRPShapeDataBlockTerminate m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIBlockProcess
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIBlockProcess);
class ProcGUIBlockProcess: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIBlockProcess);
private:
    ProcGUIBlockProcess(const LPProcGUIProcess& pProcess, const rdo::runtime::LPRDORuntime& pRuntime, const parser::LPRDOParser& pParser, const LPRPShapeDataBlockProcess& pParams);
    virtual ~ProcGUIBlockProcess();

    LPProcGUIBlock            m_pBlock;
    LPRPShapeDataBlockProcess m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIAdvance
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIAdvance);
class ProcGUIAdvance
    : public ProcGUIBlock
    , public ProcGUICalc
{
DECLARE_FACTORY(ProcGUIAdvance);
private:
    ProcGUIAdvance(const LPProcGUIProcess& pProcess, const rdo::runtime::LPRDORuntime& pRuntime, const LPRPShapeDataBlockProcess& pParams);
    virtual ~ProcGUIAdvance();

    LPIPROCBlock              m_pBlock;
    LPRPShapeDataBlockProcess m_pParams;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUISeize
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUISeize);
class ProcGUISeize: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUISeize);
public:
    void createRuntime(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser);
    void addResourceName(const std::string& name);

private:
    ProcGUISeize(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser, const LPRPShapeDataBlockProcess& pParams);
    virtual ~ProcGUISeize();

    typedef std::list<std::string> ResNameList;
    typedef std::vector<rdo::runtime::parser_for_Seize> TODO_parser_for_Seize;

    LPIPROCBlock              m_pBlock;
    LPRPShapeDataBlockProcess m_pParams;
    ResNameList               m_resNameList;
    TODO_parser_for_Seize     m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIRelease
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIRelease);
class ProcGUIRelease: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIRelease);
public:
    void createRuntime(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser);
    void addResourceName(const std::string& name);

private:
    ProcGUIRelease(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser, const LPRPShapeDataBlockProcess& pParams);
    virtual ~ProcGUIRelease();

    typedef std::list<std::string> ResNameList;
    typedef std::vector<rdo::runtime::parser_for_Seize> TODO_parser_for_Seize;

    LPIPROCBlock              m_pBlock;
    LPRPShapeDataBlockProcess m_pParams;
    ResNameList               m_resNameList;
    TODO_parser_for_Seize     m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIQueue
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIQueue);
class ProcGUIQueue: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIQueue);
public:
    void createRuntime(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser);
    void setResource(const std::string& name);

private:
    ProcGUIQueue(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser, const std::string& name);
    virtual ~ProcGUIQueue();

    std::string m_resourceName;
    LPIPROCBlock m_pBlock;
    rdo::runtime::parser_for_Queue m_parserForRuntime;
};

// --------------------------------------------------------------------------------
// -------------------- ProcGUIDepart
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ProcGUIDepart);
class ProcGUIDepart: public ProcGUIBlock
{
DECLARE_FACTORY(ProcGUIDepart);
public:
    void createRuntime(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser);
    void setResource(const std::string& name);

private:
    ProcGUIDepart(const LPProcGUIProcess& pProcess, const parser::LPRDOParser& pParser, const std::string& name);
    virtual ~ProcGUIDepart();

    std::string m_resourceName;
    LPIPROCBlock m_pBlock;
    rdo::runtime::parser_for_Queue m_parserForRuntime;
};

CLOSE_COMPILER_GUI_NAMESPACE
