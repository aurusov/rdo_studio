#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>
#include <strstream>

#include "rdoframe.h"

using namespace std;

namespace rdoModelObjects {
struct RDOSMRFileInfo
{
	string model_name;
	string resource_file;
	string oprIev_file;
	string frame_file;
	string statistic_file;
	string results_file;
	string trace_file;
};
typedef enum { PAT, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC } RDOFileType;
}

namespace rdoRuntime {
	class RDORuntime;
	class RDOResult;
}
class RDOTrace;
namespace rdoParse {
	class RDOParser ;
}

class RdoSimulator
{
	rdoParse::RDOParser *parser;
	rdoRuntime::RDORuntime *runtime;

	CWinThread* th;
	void terminateModel();
	void closeModel(); 
public:
	RdoSimulator();
	~RdoSimulator();
	bool parseModel();
	void runModel();
	void stopModel();
	vector<RDOSyntaxError>* getErrors();
	double getModelTime();
	void parseSMRFileInfo( strstream& smr, rdoModelObjects::RDOSMRFileInfo& info );
	friend UINT RunningThreadControllingFunction( LPVOID pParam );
};

#endif //RDO_SIMULATOR_INTERFACE__
