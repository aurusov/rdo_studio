#ifndef RDOTRACE_H
#define RDOTRACE_H

#include "rdo.h"

namespace rdoRuntime {

class RDOSimulatorTrace;
class TreeNodeTrace;
class RDODecisionPointTrace;
class RDORuleTrace;
class RDOIETrace;
class RDOResourceTrace;
class RDOOperationTrace;
class RDOTraceableObject;
class TreeRootTrace;
class RDOPokazTrace;

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
friend class RDOSimulatorTrace;
friend class RDORuntime;
friend class RDOTrace;

protected:
	RDOSimulatorTrace*  sim;
	bool                trace;
	int                 id;
	mutable std::string str_id;

	RDOTraceableObject( RDOSimulatorTrace* _sim ):
		sim( _sim ),
		trace( false ),
		id( -1 ),
		str_id( "" )
	{
	}
	virtual ~RDOTraceableObject()
	{
	}

public:
	bool isTrace() const                { return trace; }
	int getTraceID() const              { return id;    }
	std::string& traceId() const {
		if ( str_id.empty() ) {
			str_id = toString( id );
		}
		return str_id;
	}
	void setTraceID( int _id, int _str_id ) {
		id     = _id;
		str_id = toString( _str_id );
	}
};

class RDOPatternTrace
{
friend class RDOSimulatorTrace;
protected:
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) = 0;
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true ) = 0;
};

class RDORuntime;

// Base class for all resources
class RDOResourceTrace: public RDOTraceableObject
{
//friend RDOTrace;
//friend RDOSimulatorTrace;
//friend class rdoRuntime::RDORuntime;

public:
	enum ConvertStatus {
		CS_None = 0,
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};

private:
	std::string typeId;
	std::string traceTypeId() { return typeId.empty()?(typeId=getTypeId()):typeId; }

protected:
	RDOResourceTrace( RDOSimulatorTrace* sim, int _id, bool _tarce );
	RDOResourceTrace( const RDOResourceTrace& orig );
	virtual ~RDOResourceTrace();

	virtual std::string getTypeId()            = 0;
	virtual std::string traceParametersValue() = 0;

	ConvertStatus state;
	bool temporary;

public:
	void makeTemporary( bool value )     { temporary = value; }
	ConvertStatus getState() const       { return state;      }
	void setState( ConvertStatus value ) { state = value;     }
	std::string traceResourceState( char prefix, RDOSimulatorTrace* sim );
};

class RDOPokazTrace: public RDOPokaz, public RDOTraceableObject
{
protected:
	bool wasChanged;

public:
	RDOPokazTrace( RDORuntimeParent* _runtime, RDOSimulatorTrace* _sim ): // qq
		RDOPokaz( _runtime ),
		RDOTraceableObject( _sim ),
		wasChanged( true )
	{
	}
	bool tracePokaz();
	virtual std::string traceValue() = 0;
};

} // namespace rdoRuntime

#endif // RDOTRACE_H
