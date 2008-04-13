#ifndef RDOTRACE_H
#define RDOTRACE_H

#include "rdo.h"

#include <fstream>

namespace rdoRuntime {

class RDOSimulator;
class RDOSimulatorTrace;
class TreeNodeTrace;
//class RDODPTSearchTrace;
class RDORuleTrace;
class RDOIETrace;
class RDOResourceTrace;
class RDOOperationTrace;
class RDOTraceableObject;
class TreeRootTrace;
class RDOPokazTrace;
class TreeNode;
class TreeRoot;

class RDOEndL
{
public:
	virtual void onEndl() {}
};

inline std::ostream &operator << (std::ostream &stream, RDOEndL& rdoEndL)
{
//	stream << std::endl;
	rdoEndL.onEndl();
	return stream;
}

class RDOTrace
{
friend RDOSimulatorTrace;
friend RDOResourceTrace;

private:
	std::string fileName;
	std::ofstream out;
	bool canWriteToStream;

protected:
	bool isNullTracer;
	RDOEndL rdoEndL;

public:
	RDOTrace(): isNullTracer( true ), canWriteToStream( false ) {}
	RDOTrace( std::string fn ): fileName( fn ), out( fileName.begin(), std::ios_base::out ), isNullTracer( false ) {}
	virtual ~RDOTrace() {}

	void startWriting()   { canWriteToStream = true;  }
	void stopWriting()    { canWriteToStream = false; }
	bool canWrite() const { return canWriteToStream;  }
	bool isNull() const   { return isNullTracer;      }

	// Search in tree
	virtual void writeSearchBegin(double currentTime, std::string decisionPointId);
	virtual void writeSearchDecisionHeader();
	virtual void writeSearchDecision(RDOSimulator *sim, TreeNode *node);
	virtual void writeString(std::string);
	virtual void writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost);
	virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
	virtual void writeSearchResult(char letter, RDOSimulatorTrace *simTr, TreeRoot *treeRoot);

	virtual void writeIrregularEvent(RDOIETrace *ie, RDOSimulatorTrace *sim);
	virtual void writeRule(RDORuleTrace *rule, RDOSimulatorTrace *sim);
	virtual void writeAfterOperationBegin(RDOOperationTrace *op, RDOSimulatorTrace *sim);
	virtual void writeAfterOperationEnd(RDOOperationTrace *op, RDOSimulatorTrace *sim);

	virtual void writeTraceBegin(RDOSimulatorTrace *sim);
	virtual void writeModelBegin(RDOSimulatorTrace *sim);
	virtual void writeTraceEnd(RDOSimulatorTrace *sim);
	virtual void writeStatus(RDOSimulatorTrace *sim, char *status);

	virtual void writePermanentResources( RDOSimulatorTrace* sim, const std::list< RDOResourceTrace* >& res_perm );

	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim, const std::list< RDOResourceTrace* >& rel_res_list );

	virtual void writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok);

	virtual std::ostream& getOStream() { return out; }
	virtual RDOEndL& getEOL() { return rdoEndL; }
};

class RDOTraceableObject
{
public:
	enum { NONE = 0xFFFFFFFF };

	bool traceable() const                { return m_trace;  }
	void setTrace( bool trace )           { m_trace = trace; }

	unsigned int getTraceID() const       { return m_id;     }
	void setTraceID( unsigned int id )
	{
		setTraceID( id, id );
	}
	void setTraceID( unsigned int id, unsigned int str_id )
	{
		m_id     = id;
		m_str_id = toString( str_id );
	}

	std::string& traceId() const
	{
		if ( m_str_id.empty() ) {
			m_str_id = toString( m_id );
		}
		return m_str_id;
	}

protected:
	RDOTraceableObject( bool trace ):
		m_trace( trace ),
		m_id( NONE ),
		m_str_id( "" )
	{
	}
	virtual ~RDOTraceableObject()
	{
	}

private:
	bool                m_trace;
	unsigned int        m_id;
	mutable std::string m_str_id;
};

class RDOPatternTrace: public RDOTraceableObject
{
friend class RDOSimulatorTrace;
protected:
	RDOPatternTrace( bool trace ): RDOTraceableObject( trace ) {};

	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) = 0;
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true ) = 0;
};

class RDORuntime;

// Base class for all resources
class RDOResourceTrace: public RDOTraceableObject, public RDORuntimeContainer
{
public:
	enum ConvertStatus {
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};


	void makeTemporary( bool value )     { m_temporary = value; }
	ConvertStatus getState() const       { return m_state;      }
	void setState( ConvertStatus value ) { m_state = value;     }
	std::string traceResourceState( char prefix, RDOSimulatorTrace* sim );

protected:
	RDOResourceTrace( RDORuntime* sim, int id, bool tarce );
	RDOResourceTrace( const RDOResourceTrace& orig );
	virtual ~RDOResourceTrace();

	virtual std::string getTypeId()            = 0;
	virtual std::string traceParametersValue() = 0;

	ConvertStatus m_state;
	bool          m_temporary;

private:
	std::string m_typeId;

	std::string traceTypeId()
	{
		return m_typeId.empty() ? (m_typeId = getTypeId()) : m_typeId;
	}
};

class RDOPokazTrace: public RDOPokaz, public RDOTraceableObject, public RDORuntimeContainer
{
public:
	RDOPokazTrace( RDORuntime* runtime, bool trace );

	bool tracePokaz();
	virtual std::string traceValue() = 0;

protected:
	bool m_wasChanged;
};

} // namespace rdoRuntime

#endif // RDOTRACE_H
