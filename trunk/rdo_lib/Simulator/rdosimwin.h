#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>
#include <strstream>

using namespace std;

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
	bool parseModel();
public:
	typedef enum { NONE, PAT, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD } RDOFileType;

	RdoSimulator();
	~RdoSimulator();
	void runModel();
	void stopModel();
	void parseSMRFileInfo( strstream& smr, RDOSMRFileInfo& info );
	friend UINT RunningThreadControllingFunction( LPVOID pParam );
};

#endif //RDO_SIMULATOR_INTERFACE__
