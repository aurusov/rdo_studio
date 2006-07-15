#ifndef RDO_SIMULATOR_BASE
#define RDO_SIMULATOR_BASE

#include "rdodefines.h"

class RDOSimulatorBase
{
private:
	ListDouble timePointList;
	double currentTime;

protected:
	inline void setCurrentTime(double time) { currentTime = time; }

	// These functions for use in derived classes:
	inline double getCurrentTime() { return currentTime; }

	// Overridables:

	// Override if you need more or less real time processing.
	// By default process as fast as possible.
	virtual void rdoDelay(double fromTime, double toTime) {} 

	// This function is called by engine for every moment of model time
	// until it returns true, which means "something chanded" in model.
	virtual bool doOperation(bool onlyEndOfOperations) = 0;

	// Override if need model time limit.
	// Default - no time limit.
	virtual bool endCondition() { return false; }

	// Called by engine during initialisation/deinitialisation.
	// Override if need special initialisation/deinitialisation.
	virtual void onInit() {}
	virtual void onDestroy() {}

	// One of these funcs called by engine just before end of process.
	virtual void onNothingMoreToDo() {}
	virtual void onEndCondition() {}

	// These functions called by engine before and after process
	virtual void preProcess() {}
	virtual void postProcess() {}

	bool onlyEndOfOperations;

public:

	// These function for external use:
	void rdoInit();
//	void rdoRun();
	void rdoDestroy();
	bool rdoNext();
	void rdoPostProcess();

	void addTimePoint( double timePoint );

	RDOSimulatorBase();
	virtual ~RDOSimulatorBase() {}
};

#endif // RDO_SIMULATOR_BASE
