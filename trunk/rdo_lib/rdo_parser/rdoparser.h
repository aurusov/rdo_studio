#ifndef RDOPARSER_PARSER
#define RDOPARSER_PARSER

#include "rdoparser_object.h"
#include "rdoparser_base.h"
#include <rdoruntime_object.h>
#include <rdocommon.h>

#include <algorithm>

namespace rdoRuntime
{
class RDORuntime;
class RDOFRMFrame;
}

namespace rdoParse
{

class RDORTPParam;
class RDORTPResType;
class RDORSSResource;
class RDOPATPattern;
class RDOOPROperation;
class RDOFUNFunction;
class RDOFUNSequence;
class RDOFUNConstant;
class RDOFUNGroup;
class RDOSMR;
class RDODPTSearch;
class RDODPTSome;
class RDODPTFreeActivity;
class RDOPMDPokaz;
class RDOPROCProcess;
class RDORTPEnumParamType;

class RDOSyntaxException: public rdoRuntime::RDOException
{
public:
   std::string getType() const { return "RDO Syntax Error"; }
   RDOSyntaxException( const std::string& str ): RDOException( str ) {}
};

class RDOParser
{
private:
	RDOParserObject* parsing_object;
	RDOSMR*          smr;

	struct Changes {
		std::string name;
		std::string value;
		Changes( const std::string& _name, const std::string& _value ):
			name( _name ),
			value( _value )
		{
		}
	};
	std::vector< Changes > changes;

protected:
	RDOParserList* parsers;
	RDOParserBase* parser_base;

	bool have_kw_Resources;
	bool have_kw_ResourcesEnd;
	bool have_kw_Operations;
	bool have_kw_OperationsEnd;

	std::vector< std::string* >         allNames;
	std::vector< double* >              allDoubles;
	std::vector< RDODeletable* >        allDeletables;

	std::vector< RDORTPResType* >           allRTPResType;
	std::vector< RDORTPEnumParamType* >     allEnums;
	std::vector< RDORSSResource* >          allRSSResource;
	std::vector< RDOPATPattern* >           allPATPatterns;
	std::vector< RDOOPROperation* >         allOPROperations;
	std::vector< rdoRuntime::RDOFRMFrame* > allFRMFrame;
	std::vector< RDOFUNConstant* >          allFUNConstant;
	std::vector< RDOFUNFunction* >          allFUNFunctions;
	std::vector< RDOFUNSequence* >          allFUNSequences;
	std::vector< RDOFUNGroup* >             allFUNGroupStack;
	std::vector< RDODPTSearch* >            allDPTSearch;
	std::vector< RDODPTSome* >              allDPTSome;
	std::vector< RDODPTFreeActivity* >      allDPTFreeActivity;
	std::vector< RDOPMDPokaz* >             allPMDPokaz;
	std::vector< RDOPROCProcess* >          allDPTProcess;

	RDODPTSearch* lastDPTSearch;
	RDODPTSome*   lastDPTSome;

public:
	RDOParser();
	~RDOParser();

	const std::vector< RDORTPResType* >&       getRTPResType() const      { return allRTPResType;      }
	const std::vector< RDORTPEnumParamType* >& getEnums() const           { return allEnums;           }
	const std::vector< RDORSSResource* >&      getRSSResources() const    { return allRSSResource;     }
	      std::vector< RDOFUNGroup* >&         getFUNGroupStack()         { return allFUNGroupStack;   }
	const std::vector< RDODPTSearch* >&        getDPTSearch() const       { return allDPTSearch;       }
	const std::vector< RDODPTSome* >&          getDPTSome() const         { return allDPTSome;         }
	const std::vector< RDODPTFreeActivity* >&  getDPTFreeActivity() const { return allDPTFreeActivity; }

	void checkFunctionName( const RDOParserSrcInfo& _src_info );
	void checkActivityName( const RDOParserSrcInfo& _src_info );
	void checkDPTName( const RDOParserSrcInfo& _src_info );

	void insertDeletables( RDODeletable* value ) {
		if ( value ) {
//			TRACE( "insertDeletables: %d\n", value );
			allDeletables.push_back( value );
		}
	}
	void removeDeletables( RDODeletable* value ) {
//		TRACE( "removeDeletables: %d\n", value );
		std::vector< RDODeletable* >::iterator it = std::find( allDeletables.begin(), allDeletables.end(), value );
		if ( it != allDeletables.end() ) {
			allDeletables.erase( it );
		} else {
			TRACE( "removeDeletables: faild !!!!!!!!!!!!!!!!!!!!\n" );
		}
	}
//	void insertDeletables( RDODeletable* value )            { if ( value ) allDeletables.push_back( value );                                         }
//	void removeDeletables( RDODeletable* value )            { allDeletables.erase( std::find( allDeletables.begin(), allDeletables.end(), value ) ); }

	void insertRTPResType( RDORTPResType* value );
	void insertEnum( RDORTPEnumParamType* value );
	void insertRSSResource( RDORSSResource* value );
	void insertPATPattern( RDOPATPattern* value );
	void insertOPROperation( RDOOPROperation* value );
	void insertFRMFrame( rdoRuntime::RDOFRMFrame* value );
	void insertFUNConstant( RDOFUNConstant* value );
	void insertFUNFunction( RDOFUNFunction* value );
	void insertFUNSequences( RDOFUNSequence* value );
	void insertFUNGroup( RDOFUNGroup* value );
	void insertDPTSearch( RDODPTSearch* value );
	void insertDPTSome( RDODPTSome* value );
	void insertDPTFreeActivity( RDODPTFreeActivity* value );
	void insertPMDPokaz( RDOPMDPokaz* value );
	void insertDPTProcess( RDOPROCProcess* value );
	void insertChanges( const std::string& name, const std::string& value );
	RDORTPResType*           getLastRTPResType()    { return !allRTPResType.empty()   ? allRTPResType.back()   : NULL; }
	RDORSSResource*          getLastRSSResource()   { return !allRSSResource.empty()  ? allRSSResource.back()  : NULL; }
	RDOPATPattern*           getLastPATPattern()    { return !allPATPatterns.empty()  ? allPATPatterns.back()  : NULL; }
	rdoRuntime::RDOFRMFrame* getLastFRMFrame()      { return !allFRMFrame.empty()     ? allFRMFrame.back()     : NULL; }
	RDOFUNFunction*          getLastFUNFunction()   { return !allFUNFunctions.empty() ? allFUNFunctions.back() : NULL; }
	RDOPROCProcess*          getLastDPTProcess()    { return !allDPTProcess.empty()   ? allDPTProcess.back()   : NULL; }
	RDODPTSearch*            getLastDPTSearch()     { return lastDPTSearch;                                            }
	RDODPTSome*              getLastDPTSome()       { return lastDPTSome;                                              }
	RDOParserObject*         getLastParsingObject() { return parsing_object; }

	bool isHaveKWResources() const           { return have_kw_Resources;     }
	void setHaveKWResources( bool value )    { have_kw_Resources = value;    }
	bool isHaveKWResourcesEnd() const        { return have_kw_ResourcesEnd;  }
	void setHaveKWResourcesEnd( bool value ) { have_kw_ResourcesEnd = value; }

	bool isHaveKWOperations() const           { return have_kw_Operations;     }
	void setHaveKWOperations( bool value )    { have_kw_Operations = value;    }
	bool isHaveKWOperationsEnd() const        { return have_kw_OperationsEnd;  }
	void setHaveKWOperationsEnd( bool value ) { have_kw_OperationsEnd = value; }

	int getRTP_id() const      { return allRTPResType.size()  + 1; }
	int getRSS_id() const      { return allRSSResource.size() + 0; }
	int getPAT_id() const      { return allPATPatterns.size() + 0; }
	int getPMD_id() const      { return allPMDPokaz.size()    + 1; }
	int getFUNCONST_id() const { return allFUNConstant.size() + 0; }

	rdoRuntime::RDORuntime* runtime;

	std::vector< rdoSimulator::RDOSyntaxError > errors;

	std::stringstream modelStructure;
	std::stringstream& getModelStructure();
	std::string        getChanges() const;

	RDOSMR* getSMR() const      { return smr; }
	void setSMR( RDOSMR* _smr ) { smr = _smr; }
	bool hasSMR() const         { return smr ? true : false; }

	const RDORTPResType*      findRTPResType( const std::string& name ) const;
	const RDORSSResource*     findRSSResource( const std::string& name ) const;
	const RDOFUNConstant*     findFUNConst( const std::string& name ) const;
	const RDOFUNFunction*     findFunction( const std::string& name ) const;
	const RDOFUNSequence*     findSequence( const std::string& name ) const;
	const RDOPATPattern*      findPattern( const std::string& name ) const;
	const RDOOPROperation*    findOperation( const std::string& name ) const;
	const RDODPTFreeActivity* findFreeActivity( const std::string& name ) const;
	const RDOPMDPokaz*        findPMDPokaz( const std::string& name ) const;

	bool hasConstant() const { return !allFUNConstant.empty(); }

	void parse( int files = rdoModelObjects::obALL );
	void parse( rdoModelObjects::RDOParseType file );
	void parse( rdoModelObjects::RDOParseType file, std::istream& stream );
	void error( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error( const RDOParserSrcInfo& _src_info, rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error( const RDOParserSrcInfo& _src_info1, const RDOParserSrcInfo& _src_info2, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error_push_only( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error_push_only( const RDOParserSrcInfo& _src_info, rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void error_push_only( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error_push_only( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void error_push_done();
	void error_modify( const std::string& _message );
	void warning( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... );
	void warning( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void warning( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );

	void LoadStdFunctions();

	std::string* registerName( const char* name ) {
		std::vector< std::string* >::iterator it = allNames.begin();
		while ( it != allNames.end() ) {
			if ( (*(*it)) == name ) break;
			it++;
		}
		if ( it == allNames.end() ) {
			std::string* newName = new std::string( name );
			allNames.push_back( newName );
			return newName;
		} else {
			return *it;
		}
	}
	void addName( std::string* name ) {
		allNames.push_back( name );
	}
	double* addDouble( double* val ) {
		allDoubles.push_back( val );
		return val;
	}

	rdoModelObjects::RDOFileType getFileToParse() const       { return parser_base ? parser_base->type : rdoModelObjects::PAT;          }
	void lexer_setvalue( int val ) const                      { if ( parser_base ) parser_base->lexer_setvalue( val );                  }
	void lexer_loc_init() const                               { if ( parser_base ) parser_base->lexer_loc_init();                       }
	void lexer_loc_action() const                             { if ( parser_base ) parser_base->lexer_loc_action();                     }
	void lexer_loc_delta_pos( int value ) const               { if ( parser_base ) parser_base->lexer_loc_delta_pos( value );           }
	int  lexer_loc_line() const                               { return parser_base ? parser_base->lexer_loc_line() : -1;                }
	int  lexer_loc_pos() const                                { return parser_base ? parser_base->lexer_loc_pos() : 0;                  }
};

extern RDOParser* parser;

} // namespace rdoParse 

#endif // RDOPARSER_PARSER
