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

int smr_file_parse( void* lexer );
int smr_file_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr_file_error( char* mes );

int smr_sim_parse( void* lexer );
int smr_sim_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr_sim_error( char* mes );

class RDOFUNLogic;
class RDOFUNArithm;

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
class RDOFUNConstant;
class RDOFUNSequence;

class RDOSMR: public RDOParserObject
{
public:
	typedef std::map<std::string, std::string> StringTable;

	RDOSMR( RDOParser* _parser, const std::string& _modelName );

	void setFile( const std::string& file_type, const std::string& file_name )
	{
		files[ file_type ] = file_name;
	}
	bool hasFile( const std::string& file_type )
	{
		return files.find( file_type ) != files.end();
	}
	std::string getFile( const std::string& file_type )
	{
		return hasFile( file_type ) ? files[ file_type ] : "";
	}
	void setExternalModelName(const std::string& alias, const std::string& modelID )
	{
		m_extModelList[alias] = modelID;
	}
	std::string getExternalModelName( const std::string& alias ) const
	{
		StringTable::const_iterator it = m_extModelList.find(alias);
		return it != m_extModelList.end() ? it->second : "";
	}
	const StringTable& getExternalModelList() const
	{
		return m_extModelList;
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

private:
	StringTable  files;
	StringTable  m_extModelList;
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
};

} // namespace rdoParse

#endif // RDOSMR_H
