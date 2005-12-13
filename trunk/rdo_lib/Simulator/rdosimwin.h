#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>

#include <rdocommon.h>
#include <rdokernel.h>

#include <rdobinarystream.h>
#include <rdocommon.h>

using namespace std;

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
	struct RDOConfig;
}
class RDOTrace;
namespace rdoParse {
	class RDOParser ;
}

namespace RDOSimulatorNS
{

class RdoSimulator
{
	rdoParse::RDOParser *parser;
	rdoRuntime::RDORuntime *runtime;
	bool canTrace;

	// UA 03.12.05 // изменил работу с уведомлениями
	// Теперь каждая треда должна регистироваться в кернеле.
	RDOKernel::RDOKernelSync* syncObject;
	void createSync() {
		deleteSync();
		syncObject = new RDOKernel::RDOKernelSync( ::GetCurrentThreadId() );
		kernel.insertSyncClient( syncObject );
	}
	void deleteSync() {
		if ( syncObject ) {
			kernel.removeSyncClient( syncObject );
			delete syncObject;
			syncObject = NULL;
		}
	}

	CWinThread* th;

	vector<RDOFrame *> frames;
	vector<int> scanCodes;
//	bool shiftPressed;
//	bool ctrlPressed;
	vector<string> areasActivated;

	void terminateModel();
	void closeModel(); 

	ShowMode showMode; // current show mode
	double showRate; // current show mode

	stringstream resultString;
public:
	RdoSimulator();
	~RdoSimulator();
	bool parseModel();
	void runModel();
	void stopModel();
	vector<RDOSyntaxError>* getErrors();
	double getModelTime();
	void parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info );

	const vector<RDOFrame *>& getFrames();
//	void addKeyPressed(int scanCode);
	void keyDown(int scanCode);
	void keyUp(int scanCode);
	void addAreaPressed(string& areaName);

	vector<const string *> getAllFrames();
	vector<const string *> getAllBitmaps();

	ShowMode getInitialShowMode();
	int getInitialFrameNumber();
	double getInitialShowRate();

	stringstream &getModelStructure();
	stringstream &getResults();

	ShowMode getShowMode() { return showMode; }
	void setShowMode(ShowMode _showMode) { showMode = _showMode; }
	double getShowRate() { return showRate; }
	void setShowRate(double _showRate) { showRate = _showRate; }

	friend UINT RunningThreadControllingFunction( LPVOID pParam );
	friend void frameCallBack(rdoRuntime::RDOConfig *config, void *param);
	friend void tracerCallBack(string *newString, void *param);
};

} // namespace RDOSimulatorNS

#endif //RDO_SIMULATOR_INTERFACE__
