#ifndef RDO_SIMULATOR_INTERFACE__
#define RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include "rdoframe.h"

using namespace std;

enum RdoShowMode
{
	RDO_SM_NoShow,
	RDO_SM_Monitor,
	RDO_SM_Animation
};

struct RDOConfig
{
//////// Interactive /////////////////
	RdoShowMode showAnimation;
	vector<string> allFrameNames;
	int currFrameToShow;
	vector<int> keysPressed;
	bool mouseClicked;
	vector<string> activeAreasMouseClicked;
	
//////// Frame /////////////////////
	RDOFrame *frame;

//////// Timing ///////////////////
	double currTime;			// model time
	double newTime;			// model time
	double showRate;
	double realTimeDelay;	// msec
};

typedef void (*TracerCallBack) (string *newString, void *param);
typedef void (*FrameCallBack) (RDOConfig *config, void *param);

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
}
class RDOTrace;
namespace rdoParse {
	class RDOParser ;
}

namespace std {
class strstream;
};

struct RDOSMRFileInfo;

class RdoModel
{
	rdoRuntime::RDORuntime *runtime;
	ostringstream consol;
	RDOTrace *tracer;
	rdoRuntime::RDOResult *resulter;
	rdoParse::RDOParser *parser;

public:
	bool parseModel(string smrFileName);
	void parseSMRFileInfo(strstream& smr, RDOSMRFileInfo& info);
	string getConsole();
	void executeModel(TracerCallBack tracerCallBack, FrameCallBack frameCallBack, void *param);
	~RdoModel();
};

#endif //RDO_SIMULATOR_INTERFACE__
