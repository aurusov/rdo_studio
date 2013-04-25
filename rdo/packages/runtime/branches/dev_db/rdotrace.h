/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotrace.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировщик
  \indent    4T
*/

#ifndef _LIB_RUNTIME_TRACE_H_
#define _LIB_RUNTIME_TRACE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace_i.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/src/db/general_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNodeTrace;
class RDOEvent;
class RDORule;
class RDOOperation;
class RDOTraceableObject;
class TreeRootTrace;
class RDOResultTrace;
class TreeNode;
class TreeRoot;
PREDECLARE_POINTER(RDOResource);
PREDECLARE_POINTER(RDORuntime);

/*!
  \class     RDOEndL
  \brief     Рассылает броадкастом строку трассировки
*/
class RDOEndL
{
public:
	virtual void onEndl();
};

inline rdo::ostream &operator << (rdo::ostream &stream, RDOEndL& rdoEndL);

/*!
  \class     RDOTrace
  \brief     Формирует строки трассировки
*/
class RDOTrace
{
friend class RDOSimulatorTrace;
friend class RDOResource;

public:
	RDOTrace();
	virtual ~RDOTrace();

	rbool canTrace() const;

	void  startWriting();
	void  stopWriting();
	rbool canWrite() const;
	rbool isNull() const;

	// Search in tree
	virtual void writeSearchBegin(double currentTime, tstring decisionPointId);
	virtual void writeSearchDecisionHeader();
	virtual void writeSearchDecision(CREF(LPRDORuntime) pRuntime, TreeNode *node);
	virtual void writeString(tstring);
	virtual void writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost);
	virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
	virtual void writeSearchResult(char letter, CREF(LPRDORuntime) pRuntime, TreeRoot *treeRoot);

	virtual void writeEvent              (CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);
	virtual void writeRule               (CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);
	virtual void writeAfterOperationBegin(CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);
	virtual void writeAfterOperationEnd  (CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);

	virtual void writeTraceBegin(CREF(LPRDORuntime) pRuntime);
	virtual void writeModelBegin(CREF(LPRDORuntime) pRuntime);
	virtual void writeTraceEnd  (CREF(LPRDORuntime) pRuntime);
	virtual void writeStatus    (CREF(LPRDORuntime) pRuntime, CREF(tstring) status);

	virtual void writePermanentResources(CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) res_perm);

	tstring traceResourcesList( char prefix, CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) rel_res_list);

	virtual void writeResult(CREF(LPRDORuntime) pRuntime, PTR(RDOResultTrace) pok);

	PTR(GeneralDB) getTrcDB();
	int getSBid();

public:
	virtual REF(rdo::ostream) getOStream();
	virtual REF(RDOEndL)      getEOL();

protected:
	rbool         m_isNullTracer;

private:
	rbool         m_canWriteToStream;
	rdo::ofstream m_emptyOut;
	RDOEndL       m_emptyEndL;
	GeneralDB*    m_trcDB;
	int           m_sbId;
	int           m_so;
};

/*!
  \class     RDOTraceableObject
  \brief     Трассируемый объект
*/
class RDOTraceableObject: public ITrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(ITrace)
QUERY_INTERFACE_END

public:
	enum { NONE = 0xFFFFFFFF };

	rbool traceable() const;
	void  setTrace(rbool trace);

	ruint getTraceID() const;
	void  setTraceID(ruint id);
	void  setTraceID(ruint id, ruint str_id);

	REF(tstring) traceId() const;

protected:
	RDOTraceableObject(rbool trace, ruint id = NONE, tstring str = "");
	virtual ~RDOTraceableObject();

private:
	rbool           m_trace;
	ruint           m_id;
	mutable tstring m_str_id;
};

/*!
  \class     RDOResultTrace
  \brief     Трассируемый показатель
*/
class RDOResultTrace: public RDOTraceableObject, public IResultTrace, public IResultTraceValue
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IResultTrace)
QUERY_INTERFACE_END

public:
	RDOResultTrace(CREF(LPRDORuntime) pRuntime, rbool trace);

protected:
	LPRDORuntime m_pRuntime;
	rbool        m_wasChanged;

	DECLARE_IResultTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdotrace.inl"

#endif // _LIB_RUNTIME_TRACE_H_
