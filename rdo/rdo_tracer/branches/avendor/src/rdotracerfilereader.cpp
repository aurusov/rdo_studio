#include "stdafx.h"
#include "rdotracerfilereader.h"
#include "../resource.h"
#include "rdotracerapp.h"
#include "./trace_files/rdotracer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

#define speed 30

RDOTracerFileReader trace_reader;

// ----------------------------------------------------------------------------
// ---------- RDOTracerFileReader
// ----------------------------------------------------------------------------
RDOTracerFileReader::RDOTracerFileReader()
	: tracing( false ),
	  trace_file( NULL )
{
}

RDOTracerFileReader::~RDOTracerFileReader()
{
	stopTrace();
}

// sting is declared here to avoid memory leaks
// when killing a thread
string str;

UINT RunningThreadControllingFunction( LPVOID pParam )
{
	RDOTracerFileReader* file_reader = (RDOTracerFileReader*)pParam;

	file_reader->trace_file = new ifstream;
	if ( !file_reader->trace_file )
		return 1;

	file_reader->trace_file->open( file_reader->file_name.c_str() );

	file_reader->structure.clear();
	
	do {
		*(file_reader->trace_file) >> str;
	} while( str != "Model_name" );
	*(file_reader->trace_file) >> str;
	*(file_reader->trace_file) >> file_reader->model_name;
	rdoTracerApp.mainFrame->setModelName( file_reader->model_name );

	do {
		*(file_reader->trace_file) >> str;
	} while( str != "$Resource_type" );

	do {
		file_reader->structure << str << " ";
		*(file_reader->trace_file) >> str;
	} while ( str != "$Tracing" );

	file_reader->getNextLine();

	tracer->beforeModelStartNotify();

	file_reader->tracing = true;

	while ( !file_reader->trace_file->eof() ) {
		tracer->traceStringNotify( file_reader->getNextLine() );
		if ( rdoTracerApp.mainFrame->getShowMode() != SM_NoShow )
			::Sleep( speed );
	}

	file_reader->th = NULL;

	if ( file_reader->trace_file->is_open() ) {
		file_reader->trace_file->close();
	}
	delete file_reader->trace_file;
	file_reader->trace_file = NULL;

	file_reader->tracing = false;

	tracer->modelStoppedNotify();

	return 0;
}

string RDOTracerFileReader::getNextLine()
{
	if ( trace_file && trace_file->is_open() ) {
		char str[4096];
		::FillMemory( &str[0], 4096, '\n' );
		trace_file->getline( &str[0], 4096 );
		return str;
	} else
		return "";

}

void RDOTracerFileReader::startTrace()
{
	string filter = format( IDS_TRACE_FILTER );

	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter.c_str() );
	if ( dlg.DoModal() != IDOK )
		return;

	file_name = dlg.GetPathName();
	rdoTracerApp.mainFrame->getTab()->deleteAllItems();

	th = AfxBeginThread(RunningThreadControllingFunction, (LPVOID)this);
}

void RDOTracerFileReader::stopTrace()
{
	if( th != NULL ) {
		::TerminateThread(th->m_hThread, -1);
		delete th;
		th = NULL;
		
		if ( trace_file ) {
			if ( trace_file->is_open() ) {
				trace_file->close();
			}
			delete trace_file; trace_file = NULL;
		}
		
		tracing = false;

		tracer->modelStoppedNotify();
	}

}

void RDOTracerFileReader::pauseTrace()
{
	th->SuspendThread();
}

void RDOTracerFileReader::unpauseTrace()
{
	th->ResumeThread();
}