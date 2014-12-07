#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/compiler/procgui/namespace.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_GUI_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlock
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RPShapeDataBlock);
class RPShapeDataBlock: public rdo::counter_reference
{
DECLARE_FACTORY(RPShapeDataBlock);
public:
    enum zakonRaspr
    {
        Const,
        Normal,
        Uniform,
        Exp,
        Triangular
    };

    void setBase(int base);
    void setDisp(double disp);
    void setExp(double exp);
    void setMax(double max);

    zakonRaspr getZakon() const { return m_zakon; }
    const std::string& getName () const { return m_name; }
    int getBase() const { return m_base; }
    double getDisp() const { return m_disp; }
    double getExp() const { return m_exp; }
    double getMax() const { return m_max; }

protected:
    RPShapeDataBlock(zakonRaspr zakon, const std::string& name);
    virtual ~RPShapeDataBlock() {}

    zakonRaspr m_zakon;
    std::string m_name;
    int m_base;
    double m_exp;
    double m_disp;
    double m_max;
};

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockCreate
// --------------------------------------------------------------------------------
class RPShapeDataBlockCreate: public RPShapeDataBlock
{
DECLARE_FACTORY(RPShapeDataBlockCreate)
public:
    boost::optional<std::size_t> getAmount() const { return m_amount; }
    void setAmount(boost::optional<std::size_t> amount);

    const rdo::runtime::LPIInternalStatistics& getStatistics() const { return m_pStatistics; }
    void setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics);

private:
    RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, const std::string& name);
    virtual ~RPShapeDataBlockCreate();

    boost::optional<std::size_t> m_amount;
    rdo::runtime::LPIInternalStatistics m_pStatistics;
};
DECLARE_POINTER(RPShapeDataBlockCreate);

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockTerminate
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RPShapeDataBlockTerminate);
class RPShapeDataBlockTerminate: public rdo::counter_reference
{
DECLARE_FACTORY(RPShapeDataBlockTerminate)
public:
    void setTermInc(int term_inc);
    int getTermInc() const { return m_term_inc; }
    std::string getName() const { return m_name; }

    const rdo::runtime::LPIInternalStatistics& getStatistics() const { return m_pStatistics; }
    void setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics);

private:
    RPShapeDataBlockTerminate(const std::string& name);
    virtual ~RPShapeDataBlockTerminate();

    int m_term_inc;
    std::string m_name;
    rdo::runtime::LPIInternalStatistics m_pStatistics;
};

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockProcess
// --------------------------------------------------------------------------------
class RPShapeDataBlockProcess: public RPShapeDataBlock
{
DECLARE_FACTORY(RPShapeDataBlockProcess)
public:
    enum Action
    {
        A_ADVANCE,
        A_SEIZE,
        A_RELEASE
    };

    typedef std::list<Action> ActionList;
    typedef std::list<std::string> ResNameList;

    void addAction(Action action);
    void addRes(const std::string& res);
    
    const ActionList& getActionList () const;
    const ResNameList& getResNameList() const;

    const rdo::runtime::LPIInternalStatistics& getStatistics() const { return m_pStatistics; }
    void setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics);

private:
    RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, const std::string& name);
    virtual ~RPShapeDataBlockProcess();

    ActionList  m_actionList;
    ResNameList m_resNameList;

    rdo::runtime::LPIInternalStatistics m_pStatistics;
};
DECLARE_POINTER(RPShapeDataBlockProcess);

CLOSE_COMPILER_GUI_NAMESPACE
