#pragma warning(disable : 4786)  

#include "..\\memcheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <sstream>

#include "rdosimwin.h"
#include "rdokernel.h"
#include "rdorepository.h"

#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdosmr.h"
#include "rdofrm.h"

namespace RDOSimulatorNS
{

class RDORuntimeTracer: public RDOTrace, public RDOEndL
{
	LPVOID pParam;
  	TracerCallBack tracerCallBack;
	stringstream stream;
public:
   ostream &getOStream() { return stream; }
   RDOEndL& getEOL() { return *this; }
	void onEndl()
	{
		tracerCallBack(&stream.str(), pParam);
		stream.str("");
	}
	RDORuntimeTracer(TracerCallBack _tracerCallBack, LPVOID _pParam):
		tracerCallBack(_tracerCallBack), pParam(_pParam) { isNullTracer = false; }
};


const RDOFrame* RdoSimulator::getFrame()
{
	return frame;
}

void RdoSimulator::addKeyPressed(int scanCode)
{
	scanCodes.push_back(scanCode);
}

void RdoSimulator::addAreaPressed(string& areaName)
{
	areasActivated.push_back(areaName);
}

void frameCallBack(rdoRuntime::RDOConfig *config, void *param)
{
	RdoSimulator *simulator = (RdoSimulator *)param;
	if((config->showAnimation == SM_Animation) && (config->showAnimation == SM_Animation))
	{
		simulator->frame = config->frame;

		kernel.notify(RDOKernel::showFrame);

		config->keysPressed.insert(config->keysPressed.end(), simulator->scanCodes.begin(), simulator->scanCodes.end());
		simulator->scanCodes.clear();

		config->activeAreasMouseClicked.insert(config->activeAreasMouseClicked.end(), simulator->areasActivated.begin(), simulator->areasActivated.end());
		simulator->areasActivated.clear();

		delete config->frame;
		Sleep(config->realTimeDelay);
	}

	config->showAnimation = simulator->getShowMode();
}

void tracerCallBack(string *newString, void *param)
{
	RdoSimulator *simulator = (RdoSimulator *)param;
//	kernel.debug(newString->c_str());
	kernel.notifyString(RDOKernel.traceString, newString->c_str());
}

RdoSimulator::RdoSimulator(): 
	runtime(NULL), 
	parser(NULL), 
	th(NULL),
	frame(NULL)
{}

RdoSimulator::~RdoSimulator()
{
	terminateModel();
	closeModel();
}

UINT RunningThreadControllingFunction( LPVOID pParam )
{
	RdoSimulator *simulator = (RdoSimulator *)pParam;

	RDOTrace *tracer;
	rdoRuntime::RDOResult *resulter;

/////////////////   Creating tracer and resulter //////////////////////////////////
		if(simulator->parser->smr->traceFileName == NULL) 
			tracer = new RDOTrace();
		else
			tracer = new RDORuntimeTracer(tracerCallBack, pParam);
//			tracer = new RDOTrace(*simulator->parser->smr->traceFileName + ".trc");
//			tracer = new RDOTrace("trace.trc");

		resulter;
		if(simulator->parser->smr->resultsFileName == NULL) 
			resulter = new rdoRuntime::RDOResult();
		else
			resulter = new rdoRuntime::RDOResult((*simulator->parser->smr->resultsFileName + ".pmv").c_str());


/////////  RDO config initialization ////////////////////////
		simulator->runtime->config.showAnimation = simulator->parser->smr->showMode;
		int size = simulator->runtime->allFrames.size();
		for(int i = 0; i < size; i++)
			simulator->runtime->config.allFrameNames.push_back(*simulator->runtime->allFrames.at(i)->getName());

		simulator->runtime->config.currFrameToShow = simulator->parser->smr->frameNumber - 1;
		simulator->runtime->config.keysPressed.clear();
		simulator->runtime->config.mouseClicked = false;
		simulator->runtime->config.activeAreasMouseClicked.clear();
		simulator->runtime->config.frame = NULL;
		simulator->runtime->config.currTime = 0;
		simulator->runtime->config.newTime = 0;
		if(simulator->parser->smr->showRate)
			simulator->runtime->config.showRate = *simulator->parser->smr->showRate;
		else
			simulator->runtime->config.showRate = 60;	// default
		simulator->runtime->config.realTimeDelay = 0;




////////////////////////////////////////////////////////////////
/////////////////   Modelling	//////////////////////////////////
////////////////////////////////////////////////////////////////


	simulator->runtime->tracerCallBack = tracerCallBack;
	simulator->runtime->param = pParam;
	simulator->runtime->frameCallBack = frameCallBack;

	try
	{
		try {
			simulator->runtime->rdoInit(tracer, resulter);
		}
		catch(RDOException &ex) {
			string mess = ex.getType() + " : " + ex.mess + " during initialisation\n";
			kernel.notifyString(RDOKernel::debugString, mess);
			kernel.notify(RDOKernel::executeError);
			throw;
		}

		try {
			simulator->runtime->rdoRun();
		}
		catch(RDOException &ex) {
			string mess = ex.getType() + " : " + ex.mess + " during execution\n";
			kernel.notifyString(RDOKernel::debugString, mess);
			kernel.notify(RDOKernel::executeError);
			throw;
		}
		try {
			simulator->runtime->rdoDestroy();
		}
		catch(RDOException &ex) {
			string mess = ex.getType() + " : " + ex.mess + " during ending\n";
			kernel.notifyString(RDOKernel::debugString, mess);
			kernel.notify(RDOKernel::executeError);
			throw;
		}
		kernel.notify(RDOKernel::endExecuteModel);
		kernel.debug("End executing\n");
	}
	catch(RDOException &)
	{
	}

	simulator->th = NULL;

	return 0;
}

bool RdoSimulator::parseModel()
{
	kernel.notifyString(RDOKernel::buildString, "Start parsing\n");
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	ostringstream consol;

	try {
/////////////////   SMR file //////////////////////////////////
		stringstream SMRstream;
		kernel.getRepository()->loadSMR(SMRstream);
		if(SMRstream.good())
			parser->parseSMR1(&SMRstream, &consol);

/////////////////   RTP file //////////////////////////////////
		stringstream RTPstream1;
		kernel.getRepository()->loadRTP(RTPstream1);
		if(RTPstream1.good())
			parser->parseRTP(&RTPstream1, &consol);

/////////////////   RSS file //////////////////////////////////
		stringstream RSSstream;
		kernel.getRepository()->loadRSS(RSSstream);
		if(RSSstream.good())
			parser->parseRSS(&RSSstream, &consol);

/////////////////   FUN file //////////////////////////////////
		stringstream FUNstream;
		kernel.getRepository()->loadFUN(FUNstream);
		if(FUNstream.good())
			parser->parseFUN(&FUNstream, &consol);

/////////////////   PAT file //////////////////////////////////
		stringstream PATstream;
		kernel.getRepository()->loadPAT(PATstream);
		if(PATstream.good())
			parser->parsePAT(&PATstream, &consol);

/////////////////   OPR file //////////////////////////////////
		stringstream OPRstream;
		kernel.getRepository()->loadOPR(OPRstream);
		if(OPRstream.good())
			parser->parseOPR(&OPRstream, &consol);

/////////////////   DPT file //////////////////////////////////
		stringstream DPTstream;
		kernel.getRepository()->loadDPT(DPTstream);
		if(DPTstream.good())
			parser->parseDPT(&DPTstream, &consol);

/////////////////   PMD file //////////////////////////////////
		stringstream PMDstream;
		kernel.getRepository()->loadPMD(PMDstream);
		if(PMDstream.good())
			parser->parsePMD(&PMDstream, &consol);

/////////////////   FRM file //////////////////////////////////
		stringstream FRMstream;
		kernel.getRepository()->loadFRM(FRMstream);
		if(FRMstream.good())
			parser->parseFRM(&FRMstream, &consol);

/////////////////   SMR file //////////////////////////////////
		stringstream SMRstream2;
		kernel.getRepository()->loadSMR(SMRstream2);
		if(SMRstream2.good())
			parser->parseSMR2(&SMRstream2, &consol);
	}
	catch(RDOException &ex) {
		string mess = ex.getType() + " : " + ex.mess;
		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseError);
		closeModel();
		return false;
	}

	showMode = getInitialShowMode();

/*
	if(parser->errors.size() > 0)
	{
		for(int i = 0; i < parser->errors.size(); i++)
		{
			kernel.notifyString(RDOKernel::buildString, parser->errors.at(i).message);
		}

		return false;
	}
*/
	
	kernel.notify(RDOKernel::parseSuccess);
	kernel.notifyString(RDOKernel::buildString, "End parsing\n");


//	kernel.notifyString(RDOKernel::buildString, getModelStructure().c_str());

	return true;
}

void RdoSimulator::runModel()
{
	bool res = parseModel();
	if(res)
	{
		kernel.notify(RDOKernel::modelStarted);
		kernel.debug("Start executing\n");
		th = AfxBeginThread(RunningThreadControllingFunction, (LPVOID)this);
	}
}

void RdoSimulator::stopModel()
{
	terminateModel();
	kernel.notify(RDOKernel::modelStopped);
}

void RdoSimulator::terminateModel()
{
	if(th != NULL)
	{
		TerminateThread(th->m_hThread, -1);
		delete th;
		th = NULL;
	}
}

void RdoSimulator::closeModel()
{
	if(runtime)
		delete runtime;
	runtime = NULL;

	if(parser)
		delete parser;
	parser = NULL;
}

void RdoSimulator::parseSMRFileInfo( stringstream& smr, rdoModelObjects::RDOSMRFileInfo& info )
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	ostringstream consol;

	try {
		parser->parseSMR1(&smr, &consol);
	}
	catch(RDOException &ex) {
		string mess = ex.getType() + " : " + ex.mess + " in the first part of SMR file";
		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		return;
	}

	if(!parser->smr)
	{
		kernel.notifyString(RDOKernel::buildString, "SMR File seems to be empty\n");
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		return;
	}

	kernel.notifyString(RDOKernel::buildString, "SMR File read successfully\n");

	if(parser->smr->modelName)
		info.model_name = *parser->smr->modelName;

	if(parser->smr->resourceFileName)
		info.resource_file = *parser->smr->resourceFileName;

	if(parser->smr->oprIevFileName)
		info.oprIev_file = *parser->smr->oprIevFileName;

	if(parser->smr->frameFileName)
		info.frame_file = *parser->smr->frameFileName;

	if(parser->smr->statisticFileName)
		info.statistic_file = *parser->smr->statisticFileName;

	if(parser->smr->resultsFileName)
		info.results_file = *parser->smr->resultsFileName;

	if(parser->smr->traceFileName)
		info.trace_file = *parser->smr->traceFileName;

	closeModel();
}

vector<RDOSyntaxError>* RdoSimulator::getErrors()
{
	if(parser)
		return &parser->errors;
	else
		return NULL;
}

double RdoSimulator::getModelTime()
{
	if(runtime)
		return runtime->getTimeNow();
	else
		return 0.;
}

vector<const string *> RdoSimulator::getAllFrames()
{
	vector<const string *> vect;
	if(!runtime)
		return vect;

	int size = runtime->allFrames.size();
	for(int i = 0; i < size; i++)
	{
		RDOFRMFrame *frame = runtime->allFrames.at(i);
		vect.push_back(frame->getName());
	}

	return vect;
}

vector<const string *> RdoSimulator::getAllBitmaps()
{
	vector<const string *> vect;
	if(!runtime)
		return vect;

	int size = runtime->allFrames.size();
	for(int i = 0; i < size; i++)
	{
		RDOFRMFrame *frame = runtime->allFrames.at(i);
		frame->getAllBitmaps(vect);
	}

	return vect;
}

ShowMode RdoSimulator::getInitialShowMode()
{
	return parser->smr->showMode;
}

int RdoSimulator::getInitialFrameNumber()
{
	return parser->smr->frameNumber;
}

double RdoSimulator::getInitialShowRate()
{
	return *parser->smr->showRate;
}

string RdoSimulator::getModelStructure()
{
	return parser->modelStructure.str();
}


}// namespace RDOSimulatorNS
