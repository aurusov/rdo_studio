// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlock
// --------------------------------------------------------------------------------
RDOPROCBlock::RDOPROCBlock(LPIPROCProcess pProcess)
    : m_process(pProcess)
{
    LPIBaseOperation pThis(this);
    m_process.object_dynamic_cast<IBaseOperationContainer>()->append(pThis);
}

RDOPROCBlock::~RDOPROCBlock()
{}

RDOPROCBlock::TransactIt RDOPROCBlock::transactFind(const LPTransact& pTransact)
{
    return std::find(m_transacts.begin(), m_transacts.end(), pTransact);
}

RDOPROCBlock::TransactIt RDOPROCBlock::transactEnd()
{
    return m_transacts.end();
}

void RDOPROCBlock::transactGoIn(const LPTransact& pTransact)
{
    m_transacts.push_back(pTransact);
}

void RDOPROCBlock::transactGoOut(const LPTransact& pTransact)
{
    m_transacts.remove(pTransact);
}

LPIPROCProcess RDOPROCBlock::getProcess() const
{
    return m_process;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess(const std::string& name, const LPRDORuntime& pRuntime)
    : RDOLogicSimple(pRuntime, NULL)
    , m_name        (name          )
{}

void RDOPROCProcess::insertChild(LPIPROCProcess pProcess)
{
    if (pProcess)
    {
        m_child.push_back(pProcess);
        pProcess->setParent(this);
    }
}

void RDOPROCProcess::setParent(LPIPROCProcess pProcess)
{
    m_parent = pProcess;
}

LPIResourceType RDOPROCProcess::getTranType() const
{
    return m_pTransactType;
}

void RDOPROCProcess::next(const LPRDOPROCTransact& pTransact)
{
    if (pTransact->getBlock())
    {
        Iterator it = std::find(begin(), end(), pTransact->getBlock());
        // Если у транзакта есть блок
        if (it != end())
        {
            // Берем этот блок
            LPIPROCBlock block = it->object_dynamic_cast<IPROCBlock>();
            ASSERT(block);
            // Находим перемещаемый транзакт в списке его транзактов
            RDOPROCBlock::TransactIt it_res = block->transactFind(pTransact);
            // Если транзакт найден
            // @warning только что созданный транзакт не привязывается к блоку GENERATE!!!
            if (it_res != block->transactEnd()) 
            {
                // Удаляем его из списка транзактов этого блока
                block->transactGoOut(*it_res);
            }
            else
            {
                // Скорее всего здесь не будет ошибки, поскольку advance удаляет транзакты из себя
                //    getRuntime()->error( "Внутренняя ошибка: неучтенный транзакт (транзакт потерял свой блок)" );
            }
            // Переходим к следующему блоку
            ++it;
            // Если следующий блок существует
            if (it != end())
            {
                // Берем этот блок
                block = it->object_dynamic_cast<IPROCBlock>();
                ASSERT(block);
                pTransact->setBlock(block);
                // Записываем в конец списка этого блока перемещаемый транзакт
                block->transactGoIn(pTransact);
            }
            // Блок в из которого нужно было переместить транзакт был последним
            else 
            {
                //---------Вход в этот блок означает, что it-1 = последний блок для транзакта, 
                //---------в этом случае правильно будет удалить этот транзакт?????????
                //getRuntime()->error( "Некуда перемещать транзакт, блок в котором он был - последний" );
            }
        } 
        else
        {
            //getRuntime()->error( "Не существует такого блока, к которому привязан транзакт" );
        }
    } 
    else 
    {
        //getRuntime()->error( "К транзакту не привязан никакой блок" );
    }
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTransact
// --------------------------------------------------------------------------------
RDOPROCTransact::RDOPROCTransact(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& paramsCalcs, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool temporary)
    : RDOResource(pRuntime, paramsCalcs, pResType, resID, typeID, trace, temporary)
{
    m_state = RDOResource::ConvertStatus::CREATE;
    m_paramList.push_back(pRuntime->getCurrentTime());
}

RDOPROCTransact::~RDOPROCTransact()
{}

LPRDOPROCResource RDOPROCTransact::getRes()
{
    return m_res;
}

void RDOPROCTransact::setRes(const LPRDOPROCResource& pResource)
{
    m_res = pResource;
}

LPIPROCBlock& RDOPROCTransact::getBlock()
{
    return m_block;
}

void RDOPROCTransact::setBlock(const LPIPROCBlock& block)
{
    m_block = block;
}

LPRDOResource RDOPROCTransact::clone(const LPRDORuntime& pRuntime) const
{
    LPRDOResource pResource = rdo::Factory<RDOPROCTransact>::create(pRuntime, getParamList(), getResType(), getTraceID(), getType(), traceable(), m_temporary);
    ASSERT(pResource);
    return pResource;
}

void RDOPROCTransact::next()
{
    m_block->getProcess()->next(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& paramsCalcs, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool temporary)
    : RDOResource(pRuntime, paramsCalcs, pResType, resID, typeID, trace, temporary)
{}

RDOPROCResource::~RDOPROCResource()
{}

std::string RDOPROCResource::whoAreYou()
{
    return "procRes";
}

LPRDOResource RDOPROCResource::clone(const LPRDORuntime& pRuntime) const
{
    LPRDOResource pResource = rdo::Factory<RDOPROCResource>::create(pRuntime, getParamList(), getResType(), getTraceID(), getType(), traceable(), m_temporary);
    ASSERT(pResource);
    return pResource;
}

CLOSE_RDO_RUNTIME_NAMESPACE
