#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>

#include <rdocommon.h>
#include <rdokernel.h>
#include <rdothread.h>

#include <rdobinarystream.h>
#include <rdocommon.h>

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
	struct RDOConfig;
}
class RDOTrace;
namespace rdoParse {
	class RDOParser ;
}

namespace rdosim
{

// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
class RDOThreadRunTime: public RDOThread
{
friend class RDOThreadSimulator;
protected:
	RDOThreadSimulator* simulator;
	bool                runtime_error;

	RDOThreadRunTime();
	virtual ~RDOThreadRunTime() {}; // „тобы нельз€ было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
	virtual void idle();
	virtual void start();
	virtual void stop();
};

// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
class RDOThreadSimulator: public RDOThread
{
friend class RDOThreadRunTime;
private:
	rdoParse::RDOParser*    parser;
	rdoRuntime::RDORuntime* runtime;
	bool canTrace;

	RDOThreadRunTime* thread_runtime;
	rdoModel::RDOExitCode exitCode;

	std::vector<RDOFrame *> frames;
	std::vector<int> scanCodes;
//	bool shiftPressed;
//	bool ctrlPressed;
	std::vector<std::string> areasActivated;

	void terminateModel();
	void closeModel(); 

	ShowMode showMode; // current show mode
	double showRate; // current show mode

	std::stringstream resultString;

protected:
	virtual ~RDOThreadSimulator(); // „тобы нельз€ было удалить через delete помещаем его в protected

	virtual void proc( RDOMessageInfo& msg );

	bool parseModel();
	void runModel();
	void stopModel();
	std::vector< RDOSyntaxError >* getErrors();

public:
	RDOThreadSimulator();

	double getModelTime();
	void parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info );

	const std::vector<RDOFrame *>& getFrames();
//	void addKeyPressed(int scanCode);
	void keyDown(int scanCode);
	void keyUp(int scanCode);
	void addAreaPressed(std::string& areaName);

	std::vector< const std::string* > getAllFrames();
	std::vector< const std::string* > getAllBitmaps();

	ShowMode getInitialShowMode();
	int getInitialFrameNumber();
	double getInitialShowRate();

	std::stringstream& getModelStructure();
	std::stringstream& getResults();

	ShowMode getShowMode()                 { return showMode;      }
	void setShowMode( ShowMode _showMode ) { showMode = _showMode; }
	double getShowRate()                   { return showRate;      }
	void setShowRate( double _showRate )   { showRate = _showRate; }

	friend void frameCallBack( rdoRuntime::RDOConfig* config, void* param );
	friend void tracerCallBack( std::string* newString, void* param );
};

} // namespace rdosim

#endif // RDO_SIMULATOR_INTERFACE__
