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

int usage(char *progName)
{
	cout << "Usage: " << progName << " <file_name.smr>\nPress any key ...";
	getch();
	return -1;
}

class RDOMainException: public RDOException
{
   string getType() const { return "RDO main programm error"; }
public:
	RDOMainException(char *mess): RDOException(mess) {}
	RDOMainException(string mess): RDOException(mess.c_str()) {}
};

int main(int argc, char *argv[]) 
{ 
	if(argc != 2)
		return usage(argv[0]);

	rdoRuntime::RDORuntime *runtime = NULL;

	try
	{
		cout << "Start parsing\n";
		rdoParse::RDOParser parser;
		runtime = parser.runTime;

////////////////////////////////////////////////////////////////
/////////////////   Parsing	//////////////////////////////////
////////////////////////////////////////////////////////////////

/////////////////   SMR file first part //////////////////////////////////
		string smrFileName(argv[1]);
		ifstream smrFile1(smrFileName.c_str());
		if(!smrFile1.is_open())
			throw RDOMainException("Cannot open SMR file: " + smrFileName);

		try {
			parser.parseSMR1(&smrFile1, &cout);
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in the first part of " << smrFileName.c_str() << " (SMR) file\n" << flush;
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
		if(parser.smr->modelName == NULL) 
			throw RDOMainException("Cannot find model name in " + smrFileName + " (SMR) file");

		string rtpFileName(*parser.smr->modelName + ".rtp");
		ifstream rtpFile(rtpFileName.c_str());
		if(!rtpFile.is_open())
			throw RDOMainException("Cannot open RTP file: " + rtpFileName);

		try {
			parser.parseRTP(&rtpFile, &cout);
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << rtpFileName.c_str() << " (RTP) file\n" << flush;
			throw;
		}
		rtpFile.close();

/////////////////   RSS file //////////////////////////////////
		if(parser.smr->resourceFileName == NULL)
			throw RDOMainException("Cannot find resource file name in " + smrFileName + " (SMR) file");

		string rssFileName(*parser.smr->resourceFileName + ".rss");
		ifstream rssFile(rssFileName.c_str());
		if(!rssFile.is_open())
			throw RDOMainException("Cannot open RSS file: " + rssFileName);

		try {
			parser.parseRSS(&rssFile, &cout);
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << rssFileName.c_str() << " (RSS) file\n" << flush;
			throw;
		}
		rssFile.close();

/////////////////   FUN file //////////////////////////////////
		string funFileName(*parser.smr->modelName + ".fun");
		ifstream funFile(funFileName.c_str());
		if(funFile.is_open()) 
		{
			try {
				parser.parseFUN(&funFile, &cout);
			}
			catch(RDOException &ex) {
				cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << funFileName.c_str() << " (FUN) file\n" << flush;
				throw;
			}
			funFile.close();
		}

/////////////////   PAT file //////////////////////////////////
		string patFileName(*parser.smr->modelName + ".pat");
		ifstream patFile(patFileName.c_str());
		if(!patFile.is_open())
			throw RDOMainException("Cannot open PAT file: " + patFileName);

		try {
			parser.parsePAT(&patFile, &cout);
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << patFileName.c_str() << " (PAT) file\n" << flush;
			throw;
		}
		patFile.close();


		if(parser.smr->oprIevFileName != NULL)
		{
/////////////////   OPR file //////////////////////////////////
			string oprFileName(*parser.smr->resourceFileName + ".opr");
			ifstream oprFile(oprFileName.c_str());
			if(!oprFile.is_open())
				throw RDOMainException("Cannot open OPR file: " + oprFileName);

			try {
				parser.parseOPR(&oprFile, &cout);
			}
			catch(RDOException &ex) {
				cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << oprFileName.c_str() << " (OPR) file\n" << flush;
				throw;
			}
			oprFile.close();
		}
		else
		{
/////////////////   DPT file //////////////////////////////////
			string dptFileName(*parser.smr->resourceFileName + ".dpt");
			ifstream dptFile(dptFileName.c_str());
			if(!dptFile.is_open())
				throw RDOMainException("Cannot open DPT file: " + dptFileName);

			try {
				parser.parseDPT(&dptFile, &cout);
			}
			catch(RDOException &ex) {
				cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << dptFileName.c_str() << " (DPT) file\n" << flush;
				throw;
			}
			dptFile.close();
		}


/////////////////   PMD file //////////////////////////////////
		string pmdFileName(*parser.smr->modelName + ".pmd");
		if(parser.smr->statisticFileName != NULL) 
			pmdFileName = *parser.smr->statisticFileName + ".pmd";

		ifstream pmdFile(pmdFileName.c_str());
		if(!pmdFile.is_open() && (parser.smr->statisticFileName != NULL))
			throw RDOMainException("Cannot open PMD file: " + pmdFileName);

		if(pmdFile.is_open())
		{
			try {
				parser.parsePMD(&pmdFile, &cout);
			}
			catch(RDOException &ex) {
				cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << pmdFileName.c_str() << " (PMD) file\n" << flush;
				throw;
			}
			pmdFile.close();
		}

/////////////////   FRM file //////////////////////////////////
		string frmFileName(*parser.smr->modelName + ".frm");
		if(parser.smr->frameFileName != NULL) 
			frmFileName = *parser.smr->frameFileName + ".frm";

		ifstream frmFile(frmFileName.c_str());
		if(!frmFile.is_open() && (parser.smr->frameFileName != NULL))
			throw RDOMainException("Cannot open FRM file: " + frmFileName);

		if(frmFile.is_open())
		{
			try {
				parser.parseFRM(&frmFile, &cout);
			}
			catch(RDOException &ex) {
				cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in " << frmFileName.c_str() << " (FRM) file\n" << flush;
				throw;
			}
			frmFile.close();
		}

/////////////////   SMR file second part //////////////////////////////////
		ifstream smrFile2(smrNamePtr);
		if(!smrFile2.is_open())
			throw RDOMainException("Cannot open SMR file: " + smrFileName);

		try {
			parser.parseSMR2(&smrFile2, &cout);
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " in the second part of " << smrFileName.c_str() << " (SMR) file\n" << flush;
			throw;
		}
		smrFile2.close();

////////////////////////////////////////////////////////////////
/////////////////   Modelling	//////////////////////////////////
////////////////////////////////////////////////////////////////

		RDOTrace *tracer;
		if(parser.smr->traceFileName == NULL) 
			tracer = new RDOTrace();
		else
			tracer = new RDOTrace(*parser.smr->traceFileName + ".trc");
//			tracer = new RDOTrace("trace.trc");

		RDOResult *resulter;
		if(parser.smr->resultsFileName == NULL) 
			resulter = new rdoRuntime::RDOResult();
		else
			resulter = new rdoRuntime::RDOResult((*parser.smr->resultsFileName + ".pmv").c_str());

		try {
			runtime->rdoInit(tracer, resulter);
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " during initialisation\n" << flush;
			throw;
		}

		cout << "Start executing\n";
		try {
			runtime->rdoRun();
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " during execution\n" << flush;
			throw;
		}
		try {
			runtime->rdoDestroy();
		}
		catch(RDOException &ex) {
			cout << ex.getType().c_str() << " : " << ex.mess.c_str() << " during ending\n" << flush;
			throw;
		}
		cout << "End executing\n";
	}
	catch(RDOMainException &ex)
	{
		cout << ex.mess.c_str() << "\n" << flush;
	}
	catch(RDOException &)
	{
	}

	cout << "Press any key ...";
	getch();
	delete runtime;

	return 0;
}
