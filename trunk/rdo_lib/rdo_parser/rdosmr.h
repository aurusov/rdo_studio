#ifndef RDOSMR_H
#define RDOSMR_H

#include "rdoparser_object.h"
#include <rdotrace.h>
#include <rdocommon.h>

namespace rdoRuntime
{
class RDOCalc;
}

namespace rdoParse 
{

int smr1parse( void* lexer );
int smr1lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr1error( char* mes );

int smr2parse( void* lexer );
int smr2lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr2error( char* mes );

class RDOFUNLogic;
class RDOFUNArithm;

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
class RDOFUNConstant;
class RDOFUNSequence;

class RDOSMR: public RDOParserObject
{
private:
	std::map< std::string, std::string > files;
	rdoSimulator::ShowMode showMode;
	int    frameNumber;
	double showRate;
	double runStartTime;
	double traceStartTime;
	double traceEndTime;
	YYLTYPE traceStartTime_pos;
	YYLTYPE traceEndTime_pos;
	RDOFUNLogic* terminateIf;

	class BreakPoint: public RDOParserObject, public RDOParserSrcInfo {
	public:
		BreakPoint( RDOSMR* smr, const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic );
	};
	std::vector< BreakPoint* > breakPoints;

public:
	RDOSMR( RDOParser* _parser, const std::string& _modelName );

	void setFile( const std::string& file_type, const std::string& file_name ) {
		files[ file_type ] = file_name;
	}
	bool hasFile( const std::string& file_type ) {
		return files.find( file_type ) != files.end();
	}
	std::string getFile( const std::string& file_type ) {
		return hasFile( file_type ) ? files[ file_type ] : "";
	}

	rdoSimulator::ShowMode getShowMode() const { return showMode;       }
	int    getFrameNumber() const              { return frameNumber;    }
	double getShowRate() const                 { return showRate;       }
	double getRunStartTime() const             { return runStartTime;   }
	double getTraceStartTime() const           { return traceStartTime; }
	double getTraceEndTime() const             { return traceEndTime;   }

	void setShowMode( rdoSimulator::ShowMode _showMode );
	void setFrameNumber( int value, const YYLTYPE& pos );
	void setShowRate( double value, const YYLTYPE& pos );
	void setRunStartTime( double value, const YYLTYPE& pos );
	void setTraceStartTime( double value, const YYLTYPE& pos );
	void setTraceEndTime( double value, const YYLTYPE& pos );

	void setTerminateIf( RDOFUNLogic* logic );
	void setConstValue( const RDOParserSrcInfo& const_info, RDOFUNArithm* arithm );
	void setResParValue( const RDOParserSrcInfo& res_info, const RDOParserSrcInfo& par_info, RDOFUNArithm* arithm );
	void setSeed( const RDOParserSrcInfo& seq_info, int base );
	void insertBreakPoint( const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic );
};

} // namespace rdoParse

#endif // RDOSMR_H
