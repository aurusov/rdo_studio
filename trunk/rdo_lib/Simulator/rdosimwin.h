#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <strstream>
#include <afxwin.h>

using namespace std;

class RdoModel;

namespace rdoParse {

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

};

class RdoSimulator
{
	RdoModel *model;
	CWinThread* th;
	terminateModel();
	closeModel();
public:
	RdoSimulator();
	~RdoSimulator();
	runModel(string smrFileName);
	stopModel();
	void parseSMRFileInfo( strstream& smr, rdoParse::RDOSMRFileInfo& info );
	friend UINT RunningThreadControllingFunction( LPVOID pParam );
};

#endif //RDO_SIMULATOR_INTERFACE__
