#ifndef RDO_SIMULATOR_INTERFACE__
#define RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <rdocommon.h>

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
	std::ostringstream consol;
	RDOTrace *tracer;
	rdoRuntime::RDOResult *resulter;
	rdoParse::RDOParser *parser;

public:
	bool parseModel(std::string smrFileName);
	void parseSMRFileInfo(std::strstream& smr, RDOSMRFileInfo& info);
	std::string getConsole();
	void executeModel(TracerCallBack tracerCallBack, FrameCallBack frameCallBack, void *param);
	~RdoModel();
};

#endif //RDO_SIMULATOR_INTERFACE__
