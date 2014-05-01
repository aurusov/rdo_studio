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
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace_i.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/rdo_object.h"
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

inline std::ostream& operator << (std::ostream& stream, RDOEndL& rdoEndL);

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

	bool canTrace() const;

	void startWriting();
	void stopWriting();
	bool canWrite() const;
	bool isNull() const;

	// Search in tree
	virtual void writeSearchBegin(double currentTime, std::string decisionPointId);
	virtual void writeSearchDecisionHeader();
	virtual void writeSearchDecision(const LPRDORuntime& pRuntime, TreeNode *node);
	virtual void writeString(std::string);
	virtual void writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost);
	virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
	virtual void writeSearchResult(char letter, const LPRDORuntime& pRuntime, TreeRoot *treeRoot);

	virtual void writeEvent              (const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);
	virtual void writeRule               (const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);
	virtual void writeAfterOperationBegin(const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);
	virtual void writeAfterOperationEnd  (const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);

	virtual void writeTraceBegin(const LPRDORuntime& pRuntime);
	virtual void writeModelBegin(const LPRDORuntime& pRuntime);
	virtual void writeTraceEnd  (const LPRDORuntime& pRuntime);
	virtual void writeStatus    (const LPRDORuntime& pRuntime, const std::string& status);

	virtual void writePermanentResources(const LPRDORuntime& pRuntime, const std::list<LPRDOResource>& res_perm);

	std::string traceResourcesList( char prefix, const LPRDORuntime& pRuntime, const std::list<LPRDOResource>& rel_res_list);

	virtual void writeResult(const LPRDORuntime& pRuntime, RDOResultTrace* pok);

public:
	virtual std::ostream& getOStream();
	virtual RDOEndL& getEOL();

protected:
	bool m_isNullTracer;

private:
	bool m_canWriteToStream;
	std::ofstream m_emptyOut;
	RDOEndL m_emptyEndL;
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

	bool traceable() const;
	void setTrace(bool trace);

	std::size_t getTraceID() const;
	void setTraceID(std::size_t id);
	void setTraceID(std::size_t id, std::size_t str_id);

	std::string& traceId() const;

protected:
	RDOTraceableObject(bool trace, std::size_t id = NONE, std::string str = "");
	virtual ~RDOTraceableObject();

private:
	bool m_trace;
	std::size_t m_id;
	mutable std::string m_str_id;
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
	RDOResultTrace(const LPRDORuntime& pRuntime, bool trace);

protected:
	LPRDORuntime m_pRuntime;
	bool m_wasChanged;

	DECLARE_IResultTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdotrace-inl.h"

#endif // _LIB_RUNTIME_TRACE_H_
