#ifndef RDO_SIMULATOR_INTERFACE__
#define RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include "rdoframe.h"

using namespace std;


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
