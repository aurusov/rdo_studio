#ifndef RDO_RDO_SIMULATOR_INTERFACE__
#define RDO_RDO_SIMULATOR_INTERFACE__

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>

using namespace std;

class RdoModel;

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
	friend UINT RunningThreadControllingFunction( LPVOID pParam );
};

#endif //RDO_SIMULATOR_INTERFACE__
