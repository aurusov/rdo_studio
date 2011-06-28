#ifndef RDOTRACE_H
#define RDOTRACE_H

#include <fstream>
#include <list>
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace_i.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNodeTrace;
class RDOEvent;
class RDORule;
class RDOOperation;
class RDOTraceableObject;
class TreeRootTrace;
class RDOPokazTrace;
class TreeNode;
class TreeRoot;
PREDECLARE_POINTER(RDOResource);
PREDECLARE_POINTER(RDORuntime);

// ----------------------------------------------------------------------------
// ---------- RDOEndL - Рассылает броадкастом строку трассировки
// ----------------------------------------------------------------------------
class RDOEndL
{
public:
	virtual void onEndl() {}
};

inline std::ostream &operator << (std::ostream &stream, RDOEndL& rdoEndL)
{
	rdoEndL.onEndl();
	return stream;
}

// ----------------------------------------------------------------------------
// ---------- RDOTrace - Формирует строки трассировки
// ----------------------------------------------------------------------------
class RDOTrace
{
friend class RDOSimulatorTrace;
friend class RDOResource;

public:
	RDOTrace(): m_isNullTracer( true ), m_canWriteToStream( false ) {}
	virtual ~RDOTrace() {}

	rbool canTrace() const { return !isNull() && canWrite();   }

	void  startWriting()   { m_canWriteToStream = true;        }
	void  stopWriting()    { m_canWriteToStream = false;       }
	rbool canWrite() const { return m_canWriteToStream;        }
	rbool isNull() const   { return m_isNullTracer;            }

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
	virtual void writeStatus    (CREF(LPRDORuntime) pRuntime, char *status);

	virtual void writePermanentResources(CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) res_perm);

	tstring traceResourcesList( char prefix, CREF(LPRDORuntime) pRuntime, const std::list< LPRDOResource >& rel_res_list );

	virtual void writePokaz(CREF(LPRDORuntime) pRuntime, RDOPokazTrace *pok);

public:
	virtual std::ostream& getOStream() { return m_emptyOut; }
	virtual RDOEndL&      getEOL()     { return m_emptyEndL;}

protected:
	rbool         m_isNullTracer;

private:
	rbool         m_canWriteToStream;
	std::ofstream m_emptyOut;
	RDOEndL       m_emptyEndL;
};

// ----------------------------------------------------------------------------
// ---------- RDOTraceableObject
// ----------------------------------------------------------------------------
class RDOTraceableObject: public ITrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(ITrace)
QUERY_INTERFACE_END

public:
	enum { NONE = 0xFFFFFFFF };

	rbool traceable() const        { return m_trace;  }
	void  setTrace(rbool trace)    { m_trace = trace; }

	ruint getTraceID() const       { return m_id;     }
	void  setTraceID(ruint id)
	{
		setTraceID(id, id);
	}
	void setTraceID(ruint id, ruint str_id)
	{
		m_id     = id;
		m_str_id = rdo::toString(str_id);
	}

	REF(tstring) traceId() const
	{
		if (m_str_id.empty())
		{
			m_str_id = rdo::toString(m_id);
		}
		return m_str_id;
	}

protected:
	RDOTraceableObject(rbool trace, ruint id = NONE, tstring str = _T(""))
		: m_trace (trace)
		, m_id    (id   )
		, m_str_id(str  )
	{}
	virtual ~RDOTraceableObject()
	{}

private:
	rbool               m_trace;
	ruint               m_id;
	mutable tstring m_str_id;
};

// ----------------------------------------------------------------------------
// ---------- RDOPokazTrace
// ----------------------------------------------------------------------------
class RDOPokazTrace: public RDOTraceableObject, public IPokazTrace, public IPokazTraceValue
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IPokazTrace)
QUERY_INTERFACE_END

public:
	RDOPokazTrace(CREF(LPRDORuntime) pRuntime, rbool trace);

protected:
	LPRDORuntime m_pRuntime;
	rbool        m_wasChanged;

	DECLARE_IPokazTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDOTRACE_H
