#ifndef RDOTRACERFILEREADER_H
#define RDOTRACERFILEREADER_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <fstream>

// ----------------------------------------------------------------------------
// ---------- RDOTracerFileReader
// ----------------------------------------------------------------------------
class RDOTracerFileReader
{
protected:
	CWinThread* th;

	bool tracing;

	std::string model_name;
	std::string file_name;
	std::ifstream* trace_file;
	std::stringstream structure;

	std::string getNextLine();

	UINT delay;

public:
	RDOTracerFileReader();
	virtual ~RDOTracerFileReader();

	friend UINT RunningThreadControllingFunction( LPVOID pParam );

	std::string& getModelName() { return model_name; };
	std::stringstream& getModelStructure() { return structure; };

	bool isTracing() { return tracing; };
	
	void startTrace();
	void stopTrace();
	void pauseTrace();
	void unpauseTrace();
	UINT getDelay() const { return delay; };
	void setDelay( const UINT _delay ) { delay = _delay; };
};

extern RDOTracerFileReader trace_reader;

#endif // RDOTRACERFILEREADER_H