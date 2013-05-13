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
	tstring                    m_name;
	LPIPROCProcess             m_parent;
	std::list<LPIPROCProcess>  m_child;

private:
	RDOPROCProcess(CREF(tstring) _name, CREF(LPRDORuntime) pRuntime);
	LPIResourceType m_pTransactType;
};

PREDECLARE_POINTER(RDOPROCResource);

/*!
  \class   RDOPROCTransact
  \brief   Транзакт в РДО
*/
CLASS_PARENT_OF(RDOPROCTransact, RDOResource)
{
DECLARE_FACTORY(RDOPROCTransact);
public:
	LPRDOPROCResource getRes();
	void setRes(CREF(LPRDOPROCResource) pResource);
	REF(LPIPROCBlock) getBlock();
	void setBlock(CREF(LPIPROCBlock) block);

	void next();
	virtual LPRDOResource clone(CREF(LPRDORuntime) pRuntime) const;

private:
	RDOPROCTransact(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool permanentFlag);
	virtual ~RDOPROCTransact();

	LPIPROCBlock       m_block;
	LPRDOPROCResource  m_res;
};

/*!
  \class   RDOPROCResource
  \brief   Процессный ресурс РДО - обслуживает транзакты в процессах
*/
CLASS_PARENT_OF(RDOPROCResource, RDOResource)
{
DECLARE_FACTORY(RDOPROCResource);
friend class RDOPROCSeize;
friend class RDOPROCRelease;

public:
	tstring whoAreYou();
	virtual LPRDOResource clone(CREF(LPRDORuntime) pRuntime) const;

protected:
	std::list<LPRDOPROCTransact> transacts;

private:
	RDOPROCResource(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool permanentFlag);
	virtual ~RDOPROCResource();
};

//! Тип РДО-ресурсов для создания ресуров-транзактов
//! \details Создает ресурсы, которые могут быть релевантны активностям и 
//!          событиям, а также обслуживать транзакты в процессах
typedef  RDOResourceTypeListT<RDOPROCTransact>         RDOResourceTypeTransact;
typedef  rdo::intrusive_ptr<RDOResourceTypeTransact>  LPRDOResourceTypeTransact;

//! Тип ресурсов для создания ресурсов-ресурсов (который свободен/занят с точки зрения процесснорго подхода)
//! \details Создает ресурсы, которые могут быть релевантны активностям и 
//!          событиям, а также становиться процесс-ресурсами (свободен/занят) в процессах
typedef  RDOResourceTypeListT<RDOPROCResource>         RDOResourceTypeProccess;
typedef  rdo::intrusive_ptr<RDOResourceTypeProccess>  LPRDOResourceTypeProccess;

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/rdoprocess.inl"

#endif // _LIB_RUNTIME_PROCESS_H_
