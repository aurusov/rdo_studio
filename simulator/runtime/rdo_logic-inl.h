// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning(disable : 4786)
#endif // COMPILER_VISUAL_STUDIO
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogic
// --------------------------------------------------------------------------------
template <class Order>
inline RDOLogic<Order>::RDOLogic(const LPRDORuntime& pRuntime, LPIBaseOperationContainer pParent)
    : m_pCondition    (NULL )
    , m_lastCondition (false)
    , m_pFirst        (NULL )
    , m_pParent       (pParent ? pParent : (pRuntime ? pRuntime->m_pMetaLogic : LPIBaseOperationContainer(NULL)))
    , m_multithreading(false)
{}

template <class Order>
inline RDOLogic<Order>::~RDOLogic()
{}

template <class Order>
inline void RDOLogic<Order>::init(const LPRDORuntime& pRuntime)
{
    if (pRuntime)
    {
        pRuntime->appendLogic(this, m_pParent);
    }
}

template <class Order>
inline void RDOLogic<Order>::setCondition(const LPRDOCalc& pCondition)
{
    m_pCondition = pCondition;
}

template <class Order>
inline void RDOLogic<Order>::setMultithreading(bool multithreading)
{
    m_multithreading = multithreading;
}

template <class Order>
inline void RDOLogic<Order>::onStart(const LPRDORuntime& pRuntime)
{
    m_lastCondition = checkSelfCondition(pRuntime);
    if (m_lastCondition)
    {
        start(pRuntime);
    }
}

template <class Order>
inline void RDOLogic<Order>::onStop(const LPRDORuntime& pRuntime)
{
    m_lastCondition = false;
    stop(pRuntime);
}

template <class Order>
inline bool RDOLogic<Order>::onCheckCondition(const LPRDORuntime& pRuntime)
{
    bool condition = checkSelfCondition(pRuntime);
    if (condition != m_lastCondition)
    {
        m_lastCondition = condition;
        if (condition)
        {
            start(pRuntime);
        }
        else
        {
            stop(pRuntime);
        }
    }

    if (!condition)
        return false;

    m_pFirst = Order::sort(pRuntime, m_childList);
    return m_pFirst ? true : false;
}

template <class Order>
inline IBaseOperation::ResultCode RDOLogic<Order>::onDoOperation(const LPRDORuntime& pRuntime)
{
    if (m_lastCondition)
    {
        if (!m_pFirst)
            return IBaseOperation::ResultCode::CANNOT_RUN;

        IBaseOperation::ResultCode result = m_pFirst->onDoOperation(pRuntime);
        if (result == IBaseOperation::ResultCode::MUST_CONTINUE)
            pRuntime->setMustContinueOpr(m_pFirst);

        return result;
    }
    else
    {
        return IBaseOperation::ResultCode::CANNOT_RUN;
    }
}

template <class Order>
inline IBaseOperation::ResultCode RDOLogic<Order>::onContinue(const LPRDORuntime& pRuntime)
{
    for(ChildList::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
    {
        if ((*it)->onContinue(pRuntime) == IBaseOperation::ResultCode::MUST_CONTINUE)
            return IBaseOperation::ResultCode::MUST_CONTINUE;
    }
    return IBaseOperation::ResultCode::CANNOT_RUN;
}

template <class Order>
inline bool RDOLogic<Order>::checkSelfCondition(const LPRDORuntime& pRuntime)
{
    return m_pCondition
        ? m_pCondition->calcValue(pRuntime).getAsBool()
        : true;
}

template <class Order>
inline void RDOLogic<Order>::start(const LPRDORuntime& pRuntime)
{
    for(ChildList::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
    {
        (*it)->onStart(pRuntime);
    }
}

template <class Order>
inline void RDOLogic<Order>::stop(const LPRDORuntime& pRuntime)
{
    for(ChildList::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
    {
        (*it)->onStop(pRuntime);
    }
}

template <class Order>
inline bool RDOLogic<Order>::empty() const
{
    return m_childList.empty();
}

template <class Order>
inline typename RDOLogic<Order>::Iterator RDOLogic<Order>::begin()
{
    return m_childList.begin();
}

template <class Order>
inline typename RDOLogic<Order>::Iterator RDOLogic<Order>::end()
{
    return m_childList.end();
}

template <class Order>
inline typename RDOLogic<Order>::CIterator RDOLogic<Order>::begin() const
{
    return m_childList.begin();
}

template <class Order>
inline typename RDOLogic<Order>::CIterator RDOLogic<Order>::end() const
{
    return m_childList.end();
}

template <class Order>
inline LPIBaseOperation& RDOLogic<Order>::back()
{
    return m_childList.back();
}

template <class Order>
inline void RDOLogic<Order>::append(const Item& item)
{
    m_childList.push_back(item);
}

template <class Order>
inline void RDOLogic<Order>::clear()
{
    m_childList.clear();
}

CLOSE_RDO_RUNTIME_NAMESPACE
