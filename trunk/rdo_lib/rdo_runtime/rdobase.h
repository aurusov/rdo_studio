#ifndef RDO_SIMULATOR_BASE
#define RDO_SIMULATOR_BASE

#include "rdodefines.h"
#include <rdocommon.h>

class RDOBaseOperation;

class RDOSimulatorBase
{
private:
	double currentTime;
	double nextTime;

	rdoRuntime::RunTimeMode mode;

	double       speed;
	unsigned int speed_range_max;
	unsigned int next_delay_count;
	unsigned int next_delay_current;

	double       showRate;
	double       msec_wait;
	unsigned int msec_prev;

protected:
	struct BOPlanned {
		RDOBaseOperation* opr;
		void*             param;
		BOPlanned()                                             : opr( NULL )    , param( NULL )       {}
		BOPlanned( const BOPlanned& copy )                      : opr( copy.opr ), param( copy.param ) {}
		BOPlanned( RDOBaseOperation* _opr, void* _param = NULL ): opr( _opr )    , param( _param )     {}
	};
	std::map< double, std::list< BOPlanned >* > timePointList;

	bool check_operation;

	void setCurrentTime( double value ) { currentTime = value; }

	// Overridables:

	// Override if you need more or less real time processing.
	// By default process as fast as possible.
	virtual void rdoDelay( double fromTime, double toTime ) = 0;

	// This function is called by engine for every moment of model time
	// until it returns true, which means "something chanded" in model.
	virtual bool doOperation() = 0;

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

	// Вызывается в самом начале прогона
	virtual void preProcess()  = 0;
	// Вызывается непосредвтсенно перед окончанием прогона
	virtual void postProcess() = 0;

	virtual bool isKeyDown() { return false; }

public:
	// These function for external use:
	void rdoInit();
//	void rdoRun();
	void rdoDestroy();
	bool rdoNext();
	void rdoPostProcess();

	double getCurrentTime() const                  { return currentTime; }

	rdoRuntime::RunTimeMode getMode() const        { return mode;        }
	void setMode( rdoRuntime::RunTimeMode _mode );

	double getSpeed() const                        { return speed;       }
	void setSpeed( double persent );

	double getShowRate() const                     { return showRate;    }
	void setShowRate( double value );

	void addTimePoint( double timePoint, RDOBaseOperation* opr = NULL, void* param = NULL  );

	RDOSimulatorBase();
	virtual ~RDOSimulatorBase() {}

	static unsigned int getMSec( const SYSTEMTIME& systime ) {
		return systime.wMilliseconds + systime.wSecond * 1000 + systime.wMinute * 1000 * 60 + systime.wHour * 1000 * 60 * 60;
	}
};

#endif // RDO_SIMULATOR_BASE
