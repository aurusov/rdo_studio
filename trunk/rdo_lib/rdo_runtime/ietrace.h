#ifndef RDO_IETRACE
#define RDO_IETRACE

class RDOIETrace: public RDOIE, public RDOTraceableObject, protected RDOPatternTrace
{
friend RDOTrace;
friend RDOSimulatorTrace;

private:
	virtual const std::string& tracePatternId() const = 0;

protected:
	RDOIETrace( RDOSimulatorTrace* sim );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );
};

#endif // RDO_IETRACE
