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
#include <algorithm>

#include "rdosimwin.h"
#include <rdokernel.h>
#include <rdorepository.h>

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


const vector<RDOFrame *>& RdoSimulator::getFrames()
{
	return frames;
}
/*
void RdoSimulator::addKeyPressed(int scanCode)
{
//	scanCodes.push_back(scanCode);
//	kernel.debug("addKeyPressed: %d", scanCode); 
}
  */
void RdoSimulator::keyDown(int scanCode)
{
//	if(scanCode == VK_SHIFT)
//		shiftPressed = true;
//	else if(scanCode == VK_CONTROL)
//		ctrlPressed = true;

	scanCodes.push_back(scanCode);
//	kernel.debug("keyDown: %d", scanCode); 
}

void RdoSimulator::keyUp(int scanCode)
{
//	if(scanCode == VK_SHIFT)
//		shiftPressed = false;
//	else if(scanCode == VK_CONTROL)
//		ctrlPressed = false;

	scanCodes.erase(remove(scanCodes.begin(), scanCodes.end(), scanCode), scanCodes.end());
//	kernel.debug("keyUp: %d", scanCode); 
}

void RdoSimulator::addAreaPressed(string& areaName)
{
	areasActivated.push_back(areaName);
}

void frameCallBack(rdoRuntime::RDOConfig *config, void *param)
{
	RdoSimulator *simulator = (RdoSimulator *)param;
	if((config->showAnimation == SM_Animation) && (simulator->showMode == SM_Animation))
	{
		Sleep(config->realTimeDelay);

		simulator->frames = config->frames;

		kernel.notify(RDOKernel::showFrame);
/*
		if(simulator->scanCodes.size() > 0)
		{
			kernel.debug("Scan Codes:"); 
			for(int j = 0; j < simulator->scanCodes.size(); j++)
				kernel.debug(" %d", simulator->scanCodes[j]);

			kernel.debug("\n"); 
		}
  */
//		simulator->scanCodes.erase(remove(simulator->scanCodes.begin(), simulator->scanCodes.end(), VK_SHIFT), simulator->scanCodes.end());
//		simulator->scanCodes.erase(remove(simulator->scanCodes.begin(), simulator->scanCodes.end(), VK_CONTROL), simulator->scanCodes.end());
		config->keysPressed.insert(config->keysPressed.end(), simulator->scanCodes.begin(), simulator->scanCodes.end());
//		simulator->scanCodes.clear();
//		if(simulator->shiftPressed)
//			config->keysPressed.push_back(VK_SHIFT);
//		if(simulator->ctrlPressed)
//			config->keysPressed.push_back(VK_CONTROL);

		config->activeAreasMouseClicked.insert(config->activeAreasMouseClicked.end(), simulator->areasActivated.begin(), simulator->areasActivated.end());
		simulator->areasActivated.clear();

		for(int i = 0; i < config->frames.size(); i++)
			delete config->frames.at(i);
		config->frames.clear();
	}
	else
	{
		simulator->frames.clear();
		kernel.notify(RDOKernel::showFrame);
	}

	config->showRate = simulator->getShowRate();
	config->showAnimation = simulator->getShowMode();
}

void tracerCallBack(string *newString, void *param)
{
	RdoSimulator *simulator = (RdoSimulator *)param;
//	kernel.notifyString(RDOKernel.traceString, newString->c_str());
	int pos = 0;
	if(newString->empty())
		return;

	if(!simulator->canTrace)
		return;

	for(;;)
	{
		int next = newString->find('\n', pos);
		string str = newString->substr(pos, next-pos);
		kernel.notifyString(RDOKernel.traceString, str.c_str());
		if(next == string::npos)
			break;
		pos = next + 1;
		if(pos >= newString->length())
			break;

		int aa = 0;
	}
}

RdoSimulator::RdoSimulator(): 
	runtime(NULL), 
	parser(NULL), 
	th(NULL)
//	,
//	shiftPressed(false),
//	ctrlPressed(false)
{}

RdoSimulator::~RdoSimulator()
{
	terminateModel();
	closeModel();
}

class RDOSimResulter: public RDOResult
{
	ostream &stream;
public:
	RDOSimResulter(ostream &_stream): stream(_stream) {isNullResult = false;}
   virtual std::ostream &getOStream() { return stream; }
};

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
		{
//			resulter = new rdoRuntime::RDOResult((*simulator->parser->smr->resultsFileName + ".pmv").c_str());
			simulator->resultString.str("");
			resulter = new RDOSimResulter(simulator->resultString);
		}


/////////  RDO config initialization ////////////////////////
		simulator->runtime->config.showAnimation = simulator->parser->smr->showMode;
		int size = simulator->runtime->allFrames.size();
		for(int i = 0; i < size; i++)
			simulator->runtime->config.allFrameNames.push_back(*simulator->runtime->allFrames.at(i)->getName());

		simulator->runtime->config.currFrameToShow = simulator->parser->smr->frameNumber - 1;
		simulator->runtime->config.keysPressed.clear();
		simulator->runtime->config.mouseClicked = false;
		simulator->runtime->config.activeAreasMouseClicked.clear();
		simulator->runtime->config.frames.clear();
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
	simulator->canTrace = true;

	rdoModel::RDOExitCode exitCode = rdoModel::EC_OK;
	// UA 19.08.04 // добавил код возврата и
	// перенес kernel.notify(RDOKernel::executeError) в самый конец,
	// чтобы он не вызывался дважды в случае двойного прерывания
	try
	{
		simulator->runtime->rdoInit(tracer, resulter);
		simulator->runtime->rdoRun();
		simulator->runtime->rdoDestroy();
		kernel.notify(RDOKernel::endExecuteModel);
//		kernel.debug("End executing\n");
	}
	catch(RDOSyntaxException &) 
	{
		exitCode = rdoModel::EC_RunTimeError;
	}
	catch(RDOInternalException &ex)		  
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.debug(mess.c_str());
		exitCode = rdoModel::EC_RunTimeError;
	}

	try
	{
		simulator->runtime->rdoDestroy();
	}
	catch(RDOSyntaxException &) 
	{
		exitCode = rdoModel::EC_RunTimeError;
	}
	catch(RDOInternalException &ex)		  
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.debug(mess.c_str());
		exitCode = rdoModel::EC_RunTimeError;
	}

	if (exitCode == rdoModel::EC_RunTimeError)
		kernel.notify(RDOKernel::executeError);

	simulator->th = NULL;

	kernel.callback(RDOKernel::modelExit, exitCode);

	return 0;
}

bool RdoSimulator::parseModel()
{
//	kernel.notifyString(RDOKernel::buildString, "Start parsing\n");
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	ostringstream consol;

/*
		rdo::binarystream RTPstream1;
		kernel.getRepository()->loadRTP(RTPstream1);

		for(int i = 0; i < 10; i++)
		{
			if ( RTPstream1.eof() || RTPstream1.fail() )
				break;

			char buff[256];
			RTPstream1.read(buff, 255);
			buff[255] = 0;
			kernel.debug(buff);
		}



		kernel.notify(RDOKernel::parseError);
		closeModel();
		kernel.callback(RDOKernel::modelExit, rdoModel::EC_ParserError);
		return false;
  */
  


	try {
/////////////////   SMR file //////////////////////////////////
		rdo::binarystream SMRstream;
		kernel.getRepository()->load(rdoModelObjects::SMR, SMRstream);
		if(SMRstream.good())
			parser->parseSMR1(&SMRstream, &consol);

/////////////////   RTP file //////////////////////////////////
		rdo::binarystream RTPstream1;
		kernel.getRepository()->load(rdoModelObjects::RTP, RTPstream1);
		if(RTPstream1.good())
			parser->parseRTP(&RTPstream1, &consol);

/////////////////   RSS file //////////////////////////////////
		rdo::binarystream RSSstream;
		kernel.getRepository()->load(rdoModelObjects::RSS, RSSstream);
		if(RSSstream.good())
			parser->parseRSS(&RSSstream, &consol);

/////////////////   FUN file //////////////////////////////////
		rdo::binarystream FUNstream;
		kernel.getRepository()->load(rdoModelObjects::FUN, FUNstream);
		if(FUNstream.good())
			parser->parseFUN(&FUNstream, &consol);

/////////////////   PAT file //////////////////////////////////
		rdo::binarystream PATstream;
		kernel.getRepository()->load(rdoModelObjects::PAT, PATstream);
		if(PATstream.good())
			parser->parsePAT(&PATstream, &consol);

/////////////////   OPR file //////////////////////////////////
		rdo::binarystream OPRstream;
		kernel.getRepository()->load(rdoModelObjects::OPR, OPRstream);
		if(OPRstream.good())
			parser->parseOPR(&OPRstream, &consol);

/////////////////   DPT file //////////////////////////////////
		rdo::binarystream DPTstream;
		kernel.getRepository()->load(rdoModelObjects::DPT, DPTstream);
		if(DPTstream.good())
			parser->parseDPT(&DPTstream, &consol);

/////////////////   PMD file //////////////////////////////////
		rdo::binarystream PMDstream;
		kernel.getRepository()->load(rdoModelObjects::PMD, PMDstream);
		if(PMDstream.good())
			parser->parsePMD(&PMDstream, &consol);

/////////////////   FRM file //////////////////////////////////
		rdo::binarystream FRMstream;
		kernel.getRepository()->load(rdoModelObjects::FRM, FRMstream);
		if(FRMstream.good())
			parser->parseFRM(&FRMstream, &consol);

/////////////////   SMR file //////////////////////////////////
		rdo::binarystream SMRstream2;
		kernel.getRepository()->load(rdoModelObjects::SMR, SMRstream2);
		if(SMRstream2.good())
			parser->parseSMR2(&SMRstream2, &consol);
	}
	// UA 19.08.04 // добавил код возврата
	catch(RDOSyntaxException &ex) 
	{
//		string mess = ex.getType() + " : " + ex.mess;
//		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseError);
		closeModel();
		kernel.callback(RDOKernel::modelExit, rdoModel::EC_ParserError);
		return false;
	}
	catch(RDOInternalException &ex)
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseError);
		closeModel();
		kernel.callback(RDOKernel::modelExit, rdoModel::EC_ParserError);
		return false;
	}

	showMode = getInitialShowMode();
	showRate = getInitialShowRate();

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
//	kernel.notifyString(RDOKernel::buildString, "End parsing\n");


//	kernel.notifyString(RDOKernel::buildString, getModelStructure().str().c_str());

	return true;
}

void RdoSimulator::runModel()
{
	bool res = parseModel();
	if(res)
	{
		kernel.notify(RDOKernel::beforeModelStart);
//		kernel.debug("Start executing\n");
		th = AfxBeginThread(RunningThreadControllingFunction, (LPVOID)this);
		kernel.notify(RDOKernel::afterModelStart);
	}
}

void RdoSimulator::stopModel()
{
	canTrace = false;
	terminateModel();
	closeModel();
	kernel.notify(RDOKernel::modelStopped);
	// UA 19.08.04 // добавил код возврата
	kernel.callback(RDOKernel::modelExit, rdoModel::EC_UserBreak);
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

void RdoSimulator::parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info )
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	stringstream consol;

	try {
		parser->parseSMR1(&smr, &consol);
	}
	catch(RDOSyntaxException &) 
	{
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		info.error = true;
		return;
	}
	catch(RDOInternalException &ex)
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		info.error = true;
		return;
	}

	if(!parser->smr)
	{
		kernel.notifyString(RDOKernel::buildString, "SMR File seems to be empty\n");
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		info.error = true;
		return;
	}

//	kernel.notifyString(RDOKernel::buildString, "SMR File read successfully\n");

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
	info.error = false;
}

vector<RDOSyntaxError>* RdoSimulator::getErrors()
{
	vector<RDOSyntaxError>* res = NULL;

	if(!parser)
		return NULL;

	res = &parser->errors;
	if(res->size() > 0)
		return res;

	res = &runtime->errors;
	return res;
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

stringstream &RdoSimulator::getModelStructure()
{
	return parser->modelStructure;
}

stringstream &RdoSimulator::getResults()
{
	return resultString;
}


}// namespace RDOSimulatorNS
