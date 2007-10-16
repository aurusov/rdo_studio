#ifndef RDOSIMWIN_H
#define RDOSIMWIN_H

#pragma warning(disable : 4786)  

#include <string>
#include <sstream>
#include <vector>
#include <afxwin.h>

#include <rdocommon.h>
#include <rdokernel.h>
#include <rdothread.h>

#include <rdobinarystream.h>
#include <rdocommon.h>

namespace rdoSimulator {
class RDOThreadSimulator;
}

namespace rdoRuntime {

class RDORuntime;
class RDOResult;

// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
class RDOThreadRunTime: public RDOThreadMT
{
friend class rdoSimulator::RDOThreadSimulator;

private:
	rdoSimulator::RDOThreadSimulator* simulator;
	bool                              runtime_error;
	SYSTEMTIME time_start;

	RDOThreadRunTime();
	virtual ~RDOThreadRunTime() {}; // „тобы нельз€ было удалить через delete
	virtual void proc( RDOMessageInfo& msg );
	virtual void idle();
	virtual void start();
	virtual void stop();
	void writeResultsInfo();

public:
	struct GetFrame {
		rdoSimulator::RDOFrame* frame;
		int                     frame_number;
		GetFrame( rdoSimulator::RDOFrame* _frame, int _frame_number ): frame( _frame ), frame_number( _frame_number ) {}
	};
	struct FrameAreaDown {
		int         frame_number;
		std::string area_name;
		FrameAreaDown( int _frame_number, const std::string& _area_name ): frame_number( _frame_number ), area_name( _area_name ) {}
	};
};

} // namespace rdoRuntime

class RDOTrace;
namespace rdoParse {
	class RDOParser;
}

namespace rdoSimulator
{

// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
class RDOThreadSimulator: public RDOThreadMT
{
friend class rdoRuntime::RDOThreadRunTime;
friend class RDORuntimeTracer;

private:
	rdoParse::RDOParser*    parser;
	rdoRuntime::RDORuntime* runtime;
	bool canTrace;

	rdoRuntime::RDOThreadRunTime* thread_runtime;
	rdoSimulator::RDOExitCode exitCode;

	void terminateModel();
	void closeModel(); 

	ShowMode showMode; // current show mode
	double showRate; // current show mode

	std::stringstream resultString;
	std::stringstream resultInfoString;

protected:
	virtual ~RDOThreadSimulator(); // „тобы нельз€ было удалить через delete помещаем его в protected

	virtual void proc( RDOMessageInfo& msg );

	bool parseModel();
	void runModel();
	void stopModel();
	std::vector< RDOSyntaxError > getErrors();

public:
	RDOThreadSimulator();

	void parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info );

	ShowMode getInitialShowMode();
	int getInitialFrameNumber();
	double getInitialShowRate();

	struct GetList {
		enum Type {
			frames,
			bitmaps 
		} type;
		std::list< std::string >* list;
		GetList( Type _type, std::list< std::string >* _list ): type( _type ), list( _list ) {}
	};
};

// --------------------------------------------------------------------
// ---------- RDOThreadCodeComp
// --------------------------------------------------------------------
class RDOThreadCodeComp: public RDOThreadMT
{
protected:
	rdoParse::RDOParser* parser;

	virtual ~RDOThreadCodeComp(); // „тобы нельз€ было удалить через delete помещаем его в protected
	virtual void proc( RDOMessageInfo& msg );

public:
	RDOThreadCodeComp();

	struct GetCodeComp {
		rdoModelObjects::RDOFileType file;
		int pos_x;
		int pos_y;
		std::string& result;
		GetCodeComp( rdoModelObjects::RDOFileType _file, int _pos_x, int _pos_y, std::string& data ):
			file( _file ),
			pos_x( _pos_x ),
			pos_y( _pos_y ),
			result( data )
		{
		}
	};
};

} // namespace rdoSimulator

#endif // RDOSIMWIN_H
