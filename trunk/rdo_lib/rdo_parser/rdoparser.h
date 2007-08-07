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
}

namespace rdoParse
{

class RDORTPParamDesc;
class RDORTPResType;
class RDORSSResource;
class RDOFUNFunction;
class RDOFUNSequence;
class RDOFUNConstant;
class RDOFUNGroup;
class RDOPATPattern;
class RDOSMR;
class RDODPTSearch;
class RDODPTSome;
class RDODPTFreeActivity;
class RDOPMDPokaz;
class RDOPROCProcess;

class RDOSyntaxException: public rdoRuntime::RDOException
{
public:
   std::string getType() const { return "RDO Syntax Error"; }
   RDOSyntaxException( const std::string& str ): RDOException( str ) {}
};

class RDOParser
{
protected:
	RDOParserList* parsers;
	RDOParserBase* parser_base;

	bool have_kw_Resources;
	bool have_kw_ResourcesEnd;

	std::vector< std::string* >        allNames;
	std::vector< double* >             allDoubles;
	std::vector< RDODeletable* >       allDeletables;

	std::vector< RDORTPResType* >      allRTPResType;
	std::vector< RDORTPParamDesc* >    allRTPParams;
	std::vector< RDORSSResource* >     allRSSResource;
	std::vector< RDOPATPattern* >      allPATPatterns;
	std::vector< RDOFUNConstant* >     allFUNConstant;
	std::vector< RDOFUNFunction* >     allFUNFunctions;
	std::vector< RDOFUNSequence* >     allFUNSequences;
	std::vector< RDOFUNGroup* >        allFUNGroupStack;
	std::vector< RDODPTSearch* >       allDPTSearch;
	std::vector< RDODPTSome* >         allDPTSome;
	std::vector< RDODPTFreeActivity* > allDPTFreeActivity;
	std::vector< RDOPMDPokaz* >        allPMDPokaz;
	std::vector< RDOPROCProcess* >     allDPTProcess;

	RDODPTSearch* lastDPTSearch;

public:
	RDOParser();
	~RDOParser();

	const std::vector< RDORTPResType* >&      getRTPResType() const      { return allRTPResType;      }
	const std::vector< RDORSSResource* >&     getRSSResources() const    { return allRSSResource;     }
	      std::vector< RDOFUNGroup* >&        getFUNGroupStack()         { return allFUNGroupStack;   }
	const std::vector< RDODPTSearch* >&       getDPTSearch() const       { return allDPTSearch;       }
	const std::vector< RDODPTSome* >&         getDPTSome() const         { return allDPTSome;         }
	const std::vector< RDODPTFreeActivity* >& getDPTFreeActivity() const { return allDPTFreeActivity; }

	void checkActivityName( const std::string* _name );

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

	void insertRTPResType( RDORTPResType* value )           { if ( value ) allRTPResType.push_back( value );    }
	void insertRTPParam( RDORTPParamDesc* value )           { if ( value ) allRTPParams.push_back( value );     }
	void insertRSSResource( RDORSSResource* value )         { if ( value ) allRSSResource.push_back( value );   }
	void insertPATPattern( RDOPATPattern* value )           { if ( value ) allPATPatterns.push_back( value );   }
	void insertFUNConstant( RDOFUNConstant* value )         { if ( value ) allFUNConstant.push_back( value );   }
	void insertFUNFunction( RDOFUNFunction* value )         { if ( value ) allFUNFunctions.push_back( value );  }
	void insertFUNSequences( RDOFUNSequence* value )        { if ( value ) allFUNSequences.push_back( value );  }
	void insertFUNGroup( RDOFUNGroup* value )               { if ( value ) allFUNGroupStack.push_back( value ); }
	void insertDPTSearch( RDODPTSearch* value )             { if ( value ) allDPTSearch.push_back( value );       lastDPTSearch = value; }
	void insertDPTSome( RDODPTSome* value )                 { if ( value ) allDPTSome.push_back( value );         lastDPTSearch = NULL;  }
	void insertDPTFreeActivity( RDODPTFreeActivity* value ) { if ( value ) allDPTFreeActivity.push_back( value ); lastDPTSearch = NULL;  }
	void insertPMDPokaz( RDOPMDPokaz* value )               { if ( value ) allPMDPokaz.push_back( value );      }
	void insertDPTProcess( RDOPROCProcess* value )          { if ( value ) allDPTProcess.push_back( value );    }
	RDORTPResType*  getLastRTPResType()                     { return !allRTPResType.empty()   ? allRTPResType.back()   : NULL; }
	RDORSSResource* getLastRSSResource()                    { return !allRSSResource.empty()  ? allRSSResource.back()  : NULL; }
	RDOPATPattern*  getLastPATPattern()                     { return !allPATPatterns.empty()  ? allPATPatterns.back()  : NULL; }
	RDOFUNFunction* getLastFUNFunction()                    { return !allFUNFunctions.empty() ? allFUNFunctions.back() : NULL; }
	RDOPROCProcess* getLastDPTProcess()                     { return !allDPTProcess.empty()   ? allDPTProcess.back()   : NULL; }
	RDODPTSearch*   getLastDPTSearch()                      { return lastDPTSearch;                                            }

	bool isHaveKWResources() const           { return have_kw_Resources;     }
	void setHaveKWResources( bool value )    { have_kw_Resources = value;    }
	bool isHaveKWResourcesEnd() const        { return have_kw_ResourcesEnd;  }
	void setHaveKWResourcesEnd( bool value ) { have_kw_ResourcesEnd = value; }

	int getRTP_id() const      { return allRTPResType.size()  + 1; }
	int getRSS_id() const      { return allRSSResource.size() + 0; }
	int getPAT_id() const      { return allPATPatterns.size() + 0; }
	int getPMD_id() const      { return allPMDPokaz.size()    + 1; }
	int getFUNCONST_id() const { return allFUNConstant.size() + 0; }

	rdoRuntime::RDORuntime* runTime;
	RDOSMR*                 smr;

	std::vector< rdoSimulator::RDOSyntaxError > errors;

	std::stringstream modelStructure;
	std::stringstream& getModelStructure();

	void setSMR( RDOSMR* _smr)  { smr = _smr; }

	const RDORTPResType*  findRTPResType( const std::string* const type ) const;
	const RDORTPResType*  findRTPResType( const std::string& name ) const;
	const RDORSSResource* findRSSResource( const std::string* const name ) const;
	const RDOFUNFunction* findFunction( const std::string* const name ) const;
	const RDOFUNSequence* findSequence( const std::string* const name ) const;
	const RDOPATPattern*  findPattern( const std::string& name ) const;

	void parse( int files = rdoModelObjects::obALL );
	void parse( rdoModelObjects::RDOParseType file );
	void parse( rdoModelObjects::RDOParseType file, std::istream& stream );
	void error( rdoSimulator::RDOSyntaxError::ErrorCode error_code, ... );
	void error( const std::string& message, rdoSimulator::RDOSyntaxError::ErrorCode error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void warning( rdoSimulator::RDOSyntaxError::ErrorCode error_code, ... );
	void warning( const std::string& message, rdoSimulator::RDOSyntaxError::ErrorCode error_code = rdoSimulator::RDOSyntaxError::UNKNOWN );
	void addConstant( RDORTPParamDesc* const _cons );
	const RDOFUNConstant* findFUNConst( const std::string* const _cons) const;
	bool hasConstant() const { return !allFUNConstant.empty(); }

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
	void lexer_loc_set( int error_line, int error_pos ) const { if ( parser_base ) parser_base->lexer_loc_set( error_line, error_pos ); }
	void lexer_loc_set( void* data ) const                    { if ( parser_base ) parser_base->lexer_loc_set( data );                  }
	void lexer_loc_set( void* data1, void* data2 ) const      { if ( parser_base ) parser_base->lexer_loc_set( data1, data2 );          }
	void lexer_loc_delta_pos( int value ) const               { if ( parser_base ) parser_base->lexer_loc_delta_pos( value );           }
	void lexer_loc_backup( void* data = NULL ) const          { if ( parser_base ) parser_base->lexer_loc_backup( data );               }
	void lexer_loc_restore() const                            { if ( parser_base ) parser_base->lexer_loc_restore();                    }
	int  lexer_loc_line() const                               { return parser_base ? parser_base->lexer_loc_line() : -1;                }
	int  lexer_loc_pos() const                                { return parser_base ? parser_base->lexer_loc_pos() : 0;                  }
};

extern RDOParser* parser;

} // namespace rdoParse 

#endif // RDOPARSER_PARSER
