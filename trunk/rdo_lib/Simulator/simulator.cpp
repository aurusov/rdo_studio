#include "..\\memcheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4786)  

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

#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdosmr.h"
#include "simulator.h"
#include "rdofrm.h"

class RDOMainException: public RDOException
{
   string getType() const { return "RDO main programm error"; }
public:
	RDOMainException(char *mess): RDOException(mess) {}
	RDOMainException(string mess): RDOException(mess.c_str()) {}
};

bool RdoModel::parseModel(string smrFileName)
{ 
	runtime = NULL;

	try
	{
		consol << "Start parsing\n";
		parser = new rdoParse::RDOParser();
		runtime = parser->runTime;

////////////////////////////////////////////////////////////////
/////////////////   Parsing	//////////////////////////////////
////////////////////////////////////////////////////////////////

/////////////////   SMR file first part //////////////////////////////////
		ifstream smrFile1(smrFileName.c_str());
		if(!smrFile1.is_open())
			throw RDOMainException("Cannot open SMR file: " + smrFileName);

		try {
			parser->parseSMR1(&smrFile1, &consol);
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in the first part of " << smrFileName.c_str() << " (SMR) file\n" << flush;
			throw;
		}
		smrFile1.close();

///////////  Set current dir /////////////////////
		char modelDir[256];
		char *smrNamePtr;
		int res = GetFullPathName(smrFileName.c_str(), 256, modelDir, &smrNamePtr);
		if(res > 0)
		{
			char tmp = *smrNamePtr;
			*smrNamePtr = '\0';
			BOOL res1 = SetCurrentDirectory(modelDir);
			*smrNamePtr = tmp;
		}

/////////////////   RTP file //////////////////////////////////
		if(parser->smr->modelName == NULL) 
			throw RDOMainException("Cannot find model name in " + smrFileName + " (SMR) file");

		string rtpFileName(*parser->smr->modelName + ".rtp");
		ifstream rtpFile(rtpFileName.c_str());
		if(!rtpFile.is_open())
			throw RDOMainException("Cannot open RTP file: " + rtpFileName);

		try {
			parser->parseRTP(&rtpFile, &consol);
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << rtpFileName.c_str() << " (RTP) file\n" << flush;
			throw;
		}
		rtpFile.close();

/////////////////   RSS file //////////////////////////////////
		if(parser->smr->resourceFileName == NULL)
			throw RDOMainException("Cannot find resource file name in " + smrFileName + " (SMR) file");

		string rssFileName(*parser->smr->resourceFileName + ".rss");
		ifstream rssFile(rssFileName.c_str());
		if(!rssFile.is_open())
			throw RDOMainException("Cannot open RSS file: " + rssFileName);

		try {
			parser->parseRSS(&rssFile, &consol);
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << rssFileName.c_str() << " (RSS) file\n" << flush;
			throw;
		}
		rssFile.close();

/////////////////   FUN file //////////////////////////////////
		string funFileName(*parser->smr->modelName + ".fun");
		ifstream funFile(funFileName.c_str());
		if(funFile.is_open()) 
		{
			try {
				parser->parseFUN(&funFile, &consol);
			}
			catch(RDOException &ex) {
				consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << funFileName.c_str() << " (FUN) file\n" << flush;
				throw;
			}
			funFile.close();
		}

/////////////////   PAT file //////////////////////////////////
		string patFileName(*parser->smr->modelName + ".pat");
		ifstream patFile(patFileName.c_str());
		if(!patFile.is_open())
			throw RDOMainException("Cannot open PAT file: " + patFileName);

		try {
			parser->parsePAT(&patFile, &consol);
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << patFileName.c_str() << " (PAT) file\n" << flush;
			throw;
		}
		patFile.close();


		if(parser->smr->oprIevFileName != NULL)
		{
/////////////////   OPR file //////////////////////////////////
			string oprFileName(*parser->smr->oprIevFileName + ".opr");
			ifstream oprFile(oprFileName.c_str());
			if(!oprFile.is_open())
				throw RDOMainException("Cannot open OPR file: " + oprFileName);

			try {
				parser->parseOPR(&oprFile, &consol);
			}
			catch(RDOException &ex) {
				consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << oprFileName.c_str() << " (OPR) file\n" << flush;
				throw;
			}
			oprFile.close();
		}
		else
		{
/////////////////   DPT file //////////////////////////////////
			string dptFileName(*parser->smr->modelName + ".dpt");
			ifstream dptFile(dptFileName.c_str());
			if(!dptFile.is_open())
				throw RDOMainException("Cannot open DPT file: " + dptFileName);

			try {
				parser->parseDPT(&dptFile, &consol);
			}
			catch(RDOException &ex) {
				consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << dptFileName.c_str() << " (DPT) file\n" << flush;
				throw;
			}
			dptFile.close();
		}


/////////////////   PMD file //////////////////////////////////
		string pmdFileName(*parser->smr->modelName + ".pmd");
		if(parser->smr->statisticFileName != NULL) 
			pmdFileName = *parser->smr->statisticFileName + ".pmd";

		ifstream pmdFile(pmdFileName.c_str());
		if(!pmdFile.is_open() && (parser->smr->statisticFileName != NULL))
			throw RDOMainException("Cannot open PMD file: " + pmdFileName);

		if(pmdFile.is_open())
		{
			try {
				parser->parsePMD(&pmdFile, &consol);
			}
			catch(RDOException &ex) {
				consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << pmdFileName.c_str() << " (PMD) file\n" << flush;
				throw;
			}
			pmdFile.close();
		}

/////////////////   FRM file //////////////////////////////////
		string frmFileName(*parser->smr->modelName + ".frm");
		if(parser->smr->frameFileName != NULL) 
			frmFileName = *parser->smr->frameFileName + ".frm";

		ifstream frmFile(frmFileName.c_str());
		if(!frmFile.is_open() && (parser->smr->frameFileName != NULL))
			throw RDOMainException("Cannot open FRM file: " + frmFileName);

		if(frmFile.is_open())
		{
			try {
				parser->parseFRM(&frmFile, &consol);
			}
			catch(RDOException &ex) {
				consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << frmFileName.c_str() << " (FRM) file\n" << flush;
				throw;
			}
			frmFile.close();
		}

/////////////////   SMR file second part //////////////////////////////////
		ifstream smrFile2(smrNamePtr);
		if(!smrFile2.is_open())
			throw RDOMainException("Cannot open SMR file: " + smrFileName);

		try {
			parser->parseSMR2(&smrFile2, &consol);
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " in the second part of " << smrFileName.c_str() << " (SMR) file\n" << flush;
			throw;
		}
		smrFile2.close();


/////////////////   Creating tracer and resulter //////////////////////////////////
		tracer;
		if(parser->smr->traceFileName == NULL) 
			tracer = new RDOTrace();
		else
			tracer = new RDOTrace(*parser->smr->traceFileName + ".trc");
//			tracer = new RDOTrace("trace.trc");

		resulter;
		if(parser->smr->resultsFileName == NULL) 
			resulter = new rdoRuntime::RDOResult();
		else
			resulter = new rdoRuntime::RDOResult((*parser->smr->resultsFileName + ".pmv").c_str());


/////////  RDO config initialization ////////////////////////
		runtime->config.showAnimation = (RdoShowMode)parser->smr->showMode;
		int size = runtime->allFrames.size();
		for(int i = 0; i < size; i++)
			runtime->config.allFrameNames.push_back(*runtime->allFrames.at(i)->getName());

		runtime->config.currFrameToShow = parser->smr->frameNumber - 1;
		runtime->config.keysPressed.clear();
		runtime->config.mouseClicked = false;
		runtime->config.activeAreasMouseClicked.clear();
		runtime->config.frame = NULL;
		runtime->config.currTime = 0;
		runtime->config.newTime = 0;
		runtime->config.showRate = *parser->smr->showRate;
		runtime->config.realTimeDelay = 0;


	}
	catch(RDOMainException &ex)
	{
		consol << ex.mess.c_str() << "\n" << flush;
		return false;
	}
	catch(RDOException &)
	{
		return false;
	}

	return true;
}

void RdoModel::executeModel(TracerCallBack tracerCallBack, FrameCallBack frameCallBack, void *param)
{
////////////////////////////////////////////////////////////////
/////////////////   Modelling	//////////////////////////////////
////////////////////////////////////////////////////////////////


	runtime->tracerCallBack = tracerCallBack;
	runtime->param = param;
	runtime->frameCallBack = frameCallBack;

	try
	{
		try {
			runtime->rdoInit(tracer, resulter);
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " during initialisation\n" << flush;
			throw;
		}

		consol << "Start executing\n";
		try {
			runtime->rdoRun();
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " during execution\n" << flush;
			throw;
		}
		try {
			runtime->rdoDestroy();
		}
		catch(RDOException &ex) {
			consol << ex.getType().c_str() << " : " << ex.mess.c_str() << " during ending\n" << flush;
			throw;
		}
		consol << "End executing\n";
	}
	catch(RDOException &)
	{
	}
}

RdoModel::~RdoModel()
{
	delete runtime;
	delete parser;
}

string RdoModel::getConsole()
{
	return consol.str();
}

RDOFrame::~RDOFrame()
{
	for(vector<RDOFrameElement *>::iterator it = elements.begin(); it != elements.end(); it++)
		delete (*it);

	elements.clear();
}

RDOTextElement::RDOTextElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground, string &_text, RDOTextAlign _align)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground), strText(_text), align(_align)
{ 
	type = text_type; 
}

RDOColoredElement::RDOColoredElement(RDOColor &_background, RDOColor &_foreground)
	: background(_background), foreground(_foreground)
{}

RDOBoundedElement::RDOBoundedElement(double _x, double _y, double _w, double _h)
	: x(_x), y(_y), w(_w), h(_h)
{}

RDORectElement::RDORectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = rect_type; 
}

RDOLineElement::RDOLineElement(double _x, double _y, double _w, double _h, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), foreground(_foreground)
{ 
	type = line_type; 
}

RDOSBmpElement::RDOSBmpElement(double _x, double _y, double _w, double _h, string &_bmp, string &_map)
	: RDOBoundedElement(_x, _y, _w, _h), bmp(_bmp), map(_map)
{ 
	type = s_bmp_type; 
}

RDOBitmapElement::RDOBitmapElement(double _x, double _y, string &_bmp, string &_map)
	: x(_x), y(_y), bmp(_bmp), map(_map)
{
	type = bitmap_type; 
}

RDOActiveElement::RDOActiveElement(double _x, double _y, double _w, double _h, string &_operName)
	: RDOBoundedElement(_x, _y, _w, _h), operName(_operName)
{ 
	type = active_type; 
}

RDORRectElement::RDORRectElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = r_rect_type; 
}

RDOEllipseElement::RDOEllipseElement(double _x, double _y, double _w, double _h, RDOColor &_background, RDOColor &_foreground)
	: RDOBoundedElement(_x, _y, _w, _h), RDOColoredElement(_background, _foreground)
{ 
	type = ellipse_type; 
}

RDOTriangElement::RDOTriangElement(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, RDOColor &_background, RDOColor &_foreground)
	: x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3), RDOColoredElement(_background, _foreground)
{ 
	type = triang_type; 
}
