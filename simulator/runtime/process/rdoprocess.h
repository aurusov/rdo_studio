/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoprocess.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \authors   etc.
  \date      11.06.2006
  \brief     Процесснные операторы РДО
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_H_
#define _LIB_RUNTIME_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCBlock
  \brief   Базовый класс для процессных блоков (операторов) РДО
*/
class RDOPROCBlock: public IPROCBlock, public IInit, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPROCBlock)
	QUERY_INTERFACE(IInit     )
QUERY_INTERFACE_END

friend class RDOPROCTransact;
friend class RDOPROCProcess;
friend class RDOPROCResource;

protected:
	LPIPROCProcess  m_process;
	TransactList    m_transacts;

	RDOPROCBlock(LPIPROCProcess process);
	virtual ~RDOPROCBlock();

	DECLARE_IPROCBlock;
	DECLARE_IInit;
};

/*!
  \class   RDOPROCProcess
  \brief   Процесс в РДО
*/
class RDOPROCProcess: public RDOLogicSimple, public IPROCProcess, public RDOPatternPrior
{
DEFINE_IFACTORY(RDOPROCProcess)
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE       (IPROCProcess   )
	QUERY_INTERFACE_PARENT(RDOLogicSimple )
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END
friend class RDOPROCBlock;

public:
	DECLARE_IPROCProcess;

protected:
	std::string m_name;
	LPIPROCProcess m_parent;
	std::list<LPIPROCProcess> m_child;

private:
	RDOPROCProcess(const std::string& _name, const LPRDORuntime& pRuntime);
	LPIResourceType m_pTransactType;
};

PREDECLARE_POINTER(RDOPROCResource);

/*!
  \class   RDOPROCTransact
  \brief   Транзакт в РДО
*/
class RDOPROCTransact: public RDOResource
{
DECLARE_FACTORY(RDOPROCTransact);
public:
	LPRDOPROCResource getRes();
	void setRes(const LPRDOPROCResource& pResource);
	LPIPROCBlock& getBlock();
	void setBlock(const LPIPROCBlock& block);

	void next();
	virtual LPRDOResource clone(const LPRDORuntime& pRuntime) const;

private:
	RDOPROCTransact(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& paramsCalcs, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool permanentFlag);
	virtual ~RDOPROCTransact();

	LPIPROCBlock       m_block;
	LPRDOPROCResource  m_res;
};

/*!
  \class   RDOPROCResource
  \brief   Процессный ресурс РДО - обслуживает транзакты в процессах
*/
class RDOPROCResource: public RDOResource
{
DECLARE_FACTORY(RDOPROCResource);
friend class RDOPROCSeize;
friend class RDOPROCRelease;

public:
	std::string whoAreYou();
	virtual LPRDOResource clone(const LPRDORuntime& pRuntime) const;

protected:
	std::list<LPRDOPROCTransact> transacts;

private:
	RDOPROCResource(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& paramsCalcs, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool permanentFlag);
	virtual ~RDOPROCResource();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/rdoprocess-inl.h"

#endif // _LIB_RUNTIME_PROCESS_H_
