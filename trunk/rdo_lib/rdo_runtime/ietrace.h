#ifndef RDO_IETRACE
#define RDO_IETRACE

class RDOIETrace: public RDOIE, public RDOTraceableObject, RDOPattern
{
friend RDOTrace;
friend RDOSimulatorTrace;

   virtual std::string tracePatternId() = 0;
   virtual void onAfterIrregularEvent(RDOSimulator *sim);

protected:
   RDOIETrace(RDOSimulatorTrace *sim);
};

#endif // RDO_IETRACE
