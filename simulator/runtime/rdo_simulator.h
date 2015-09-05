#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdobase.h"
#include "simulator/runtime/rdo_logic_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator: public RDOSimulatorBase
{
public:
    RDOSimulator();
    virtual ~RDOSimulator();

    void appendLogic(const LPIBaseOperation& pLogic, LPIBaseOperationContainer pParent);
    LPIBaseOperation getMustContinueOpr() const;
    void setMustContinueOpr(const LPIBaseOperation& pOperation);
    virtual void onPutToTreeNode() = 0;

    std::string writeActivitiesStructure(std::size_t& counter);

    std::size_t getSizeofSim() const;

    LPIBaseOperationContainer m_pMetaLogic;

protected:
    void appendBaseOperation(LPIBaseOperationContainer pLogic, const LPIBaseOperation& pBaseOperation);

    // Инициализирует нерегулярные события и блоки GENERATE: задает время первого срабатывания
    virtual void preProcess();

    virtual void onResetResult     () = 0;
    virtual void onCheckResult     () = 0;
    virtual void onAfterCheckResult() = 0;

    std::size_t m_sizeofSim;

private:
    LPIBaseOperation m_pOprMustContinue;

    virtual bool doOperation();
};

CLOSE_RDO_RUNTIME_NAMESPACE
