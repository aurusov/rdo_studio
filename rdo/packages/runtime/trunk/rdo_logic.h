/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_logic.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      23.04.2008
  \brief     Логика - контейнер БЗ
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_H_
#define _LIB_RUNTIME_LOGIC_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/rdo_logic_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOLogic
  \tparam  Order Дисциплина очереди
  \brief   Базовый класс для логик РДО
*/
template <class Order>
class RDOLogic: public IBaseOperation, public IBaseOperationContainer, public ILogic, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IBaseOperationContainer)
	QUERY_INTERFACE(ILogic)
QUERY_INTERFACE_END

public:
	typedef  BaseOperationList                  ChildList;
	typedef  BaseOperationList::iterator        Iterator;
	typedef  BaseOperationList::const_iterator  CIterator;

protected:
	RDOLogic(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent = NULL);
	virtual ~RDOLogic();

	DECLARE_IBaseOperationContainer;

	LPRDOCalc                 m_pCondition;
	rbool                     m_lastCondition;
	ChildList                 m_childList;
	LPIBaseOperation          m_pFirst;
	LPIBaseOperationContainer m_pParent;
	rbool                     m_multithreading;

private:
	rbool checkSelfCondition(CREF(LPRDORuntime) pRuntime);
	void  start             (CREF(LPRDORuntime) pRuntime);
	void  stop              (CREF(LPRDORuntime) pRuntime);

	DECLARE_IBaseOperation;
	DECLARE_ILogic;
};

/*!
  \class   RDOOrderSimple
  \brief   Простая дисциплина очереди для логик БЗ
*/
class RDOOrderSimple
{
public:
	static LPIBaseOperation sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container);
};

/*!
  \class   RDOOrderMeta
  \brief   Дисциплина очереди для мета-логики БЗ
*/
class RDOOrderMeta
{
public:
	static LPIBaseOperation sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container);
};

/*!
  \class   RDOLogicSimple
  \brief   Простая логика БЗ
  \details Используется в DPRSome
*/
class RDOLogicSimple: public RDOLogic<RDOOrderSimple>
{
protected:
	DEFINE_IFACTORY(RDOLogicSimple);

	RDOLogicSimple(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent)
		: RDOLogic<RDOOrderSimple>(pRuntime, pParent)
	{}
	virtual ~RDOLogicSimple()
	{}
};

/*!
  \class   RDOLogicMeta
  \brief   Мета-логика БЗ
*/
class RDOLogicMeta: public RDOLogic<RDOOrderMeta>
{
protected:
	DEFINE_IFACTORY(RDOLogicMeta);

	RDOLogicMeta()
		: RDOLogic<RDOOrderMeta>(NULL)
	{}
	virtual ~RDOLogicMeta()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_logic.inl"

#endif // _LIB_RUNTIME_LOGIC_H_
