#include "..\\memcheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

#include "rdosimwin.h"
#include "simulator.h"
#include "rdokernel.h"

RdoSimulator::RdoSimulator(): model(NULL), th(NULL)
{}

RdoSimulator::~RdoSimulator()
{
	terminateModel();
	closeModel();
}

UINT RunningThreadControllingFunction( LPVOID pParam )
{
	RdoSimulator *simulator = (RdoSimulator *)pParam;
	simulator->model->executeModel(NULL, NULL, pParam);

	kernel.onNotify(RDOKernel::endExecuteModel);
	kernel.onNotifyString( RDOKernel::debugString, "End execute model" );

	return 0;
}

RdoSimulator::runModel(string smrFileName)
{
	terminateModel();
	closeModel();
	model = new RdoModel();
	bool res = model->parseModel(smrFileName);
	if(!res)
	{
		kernel.onNotify(RDOKernel::parseError);
		kernel.onNotifyString( RDOKernel::buildString, model->getConsole() );
	}
	else
	{
		kernel.onNotify(RDOKernel::modelStarted);
		kernel.onNotifyString( RDOKernel::debugString, "Start execute model" );
		th = AfxBeginThread(RunningThreadControllingFunction, (LPVOID)this);
	}
}

RdoSimulator::stopModel()
{
	terminateModel();
	kernel.onNotify(RDOKernel::modelStopped);
}

RdoSimulator::terminateModel()
{
	if(th != NULL)
	{
		TerminateThread(th->m_hThread, -1);
		delete th;
		th = NULL;
	}
}

RdoSimulator::closeModel()
{
	if(model != NULL)
	{
		delete model;
		model = NULL;
	}
}
