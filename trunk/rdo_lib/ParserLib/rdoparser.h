#ifndef RDOPARSER_PARSER
#define RDOPARSER_PARSER

#include "rdogramma.h"
#include "rdoStdFuncs.h"
#include "rdocalcconst.h"
#include "rdoparser_base.h"

#include <rdocommon.h>

#include <algorithm>

namespace rdoRuntime
{
class RDORuntime;
}

using namespace rdoRuntime;

namespace rdoParse
{

class RDORTPParamDesc;
class RDORTPResType;
class RDORSSResource;
class RDOFUNFunction;
struct RDOFUNSequence;
struct RDOFUNConstant;
struct RDOFUNGroup;
class RDOPATPattern;
class RDOSMR;
class RDODPTSearch;
class RDODPTSome;
class RDODPTFreeActivity;
class RDOPMDPokaz;

struct RDOSyntaxException: public RDOException
{
   std::string getType() const { return "RDO Syntax Error"; }
   RDOSyntaxException(const char *str): RDOException(str) {}
};

struct RDOInternalException: public RDOException
{
   std::string getType() const { return "RDO Syntax Error"; }
   RDOInternalException(const char *str): RDOException(str) {}
};

class RDOParser
{
protected:
	RDOParserList  parsers;
	RDOParserBase* parser;

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

	RDODPTSearch* lastDPTSearch;

public:
	RDOParser();
	~RDOParser();

	const std::vector< RDORSSResource* >&     getRSSResources() const    { return allRSSResource;     }
	      std::vector< RDOFUNGroup* >&        getFUNGroupStack()         { return allFUNGroupStack;   }
	const std::vector< RDODPTSearch* >&       getDPTSearch() const       { return allDPTSearch;       }
	const std::vector< RDODPTSome* >&         getDPTSome() const         { return allDPTSome;         }
	const std::vector< RDODPTFreeActivity* >& getDPTFreeActivity() const { return allDPTFreeActivity; }

	void insertDeletables( RDODeletable* value )            { if ( value ) allDeletables.push_back( value );                                         }
	void removeDeletables( RDODeletable* value )            { allDeletables.erase( std::find( allDeletables.begin(), allDeletables.end(), value ) ); }

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
	RDORTPResType*  getLastRTPResType()                     { return !allRTPResType.empty()  ? allRTPResType.back()    : NULL; }
	RDORSSResource* getLastRSSResource()                    { return !allRSSResource.empty()  ? allRSSResource.back()  : NULL; }
	RDOPATPattern*  getLastPATPattern()                     { return !allPATPatterns.empty()  ? allPATPatterns.back()  : NULL; }
	RDOFUNFunction* getLastFUNFunction()                    { return !allFUNFunctions.empty() ? allFUNFunctions.back() : NULL; }
	RDODPTSearch*   getLastDPTSearch()                      { return lastDPTSearch;                                            }

	int getRTP_id() const      { return allRTPResType.size()  + 1; }
	int getRSS_id() const      { return allRSSResource.size() + 0; }
	int getPAT_id() const      { return allPATPatterns.size() + 0; }
	int getPMD_id() const      { return allPMDPokaz.size()    + 1; }
	int getFUNCONST_id() const { return allFUNConstant.size() + 1; }
//	int resourceTypeCounter;
//	int resourceCounter;
//	int patternCounter;
//	int pokazCounter;
//	int constCounter;

	RDORuntime* runTime;
	RDOSMR*     smr;

	std::vector< rdosim::RDOSyntaxError > errors;

	std::stringstream modelStructure;
	std::stringstream& getModelStructure();

	void setSMR( RDOSMR* _smr)  { smr = _smr; }

	const RDORTPResType *findRTPResType(const std::string *const type) const;
	const RDORSSResource *findRSSResource(const std::string *const name) const;
	const RDOFUNFunction *findFunction(const std::string *const name) const;
	const RDOFUNSequence *findSequence(const std::string *const name) const;
	const RDOPATPattern *findPattern(const std::string *const name) const;

	void parse( int files = rdoModelObjects::obALL );
	void parse( rdoModelObjects::RDOParseType file );
	void parse( rdoModelObjects::RDOParseType file, std::istream& stream );
	void error( rdosim::RDOSyntaxError::ErrorCode error_code, ... );
	void error( const std::string& message, rdosim::RDOSyntaxError::ErrorCode error_code = rdosim::RDOSyntaxError::UNKNOWN );
	void warning( rdosim::RDOSyntaxError::ErrorCode error_code, ... );
	void warning( const std::string& message, rdosim::RDOSyntaxError::ErrorCode error_code = rdosim::RDOSyntaxError::UNKNOWN );
	void addConstant(RDORTPParamDesc *const _cons);
	const RDOFUNConstant *RDOParser::findFUNConst(const std::string *const _cons) const;

	void LoadStdFunctions();
	std::string* registerName( const char* name ) {
		std::string* newName = new std::string( name );
		allNames.push_back( newName );
		return newName;
	}
	void addName( std::string* name ) {
		allNames.push_back( name );
	}
	double* addDouble( double* val ) {
		allDoubles.push_back( val );
		return val;
	}

	rdoModelObjects::RDOFileType getFileToParse() const       { return parser ? parser->type : rdoModelObjects::PAT;          }
	void lexer_setvalue( int val ) const                      { if ( parser ) parser->lexer_setvalue( val );                  }
	void lexer_loc_init() const                               { if ( parser ) parser->lexer_loc_init();                       }
	void lexer_loc_action() const                             { if ( parser ) parser->lexer_loc_action();                     }
	void lexer_loc_set( int error_line, int error_pos ) const { if ( parser ) parser->lexer_loc_set( error_line, error_pos ); }
	void lexer_loc_set( void* data ) const                    { if ( parser ) parser->lexer_loc_set( data );                  }
	void lexer_loc_delta_pos( int value ) const               { if ( parser ) parser->lexer_loc_delta_pos( value );           }
	void lexer_loc_backup( void* data = NULL ) const          { if ( parser ) parser->lexer_loc_backup( data );               }
	void lexer_loc_restore() const                            { if ( parser ) parser->lexer_loc_restore();                    }
	int  lexer_loc_line() const                               { return parser ? parser->lexer_loc_line() : -1;                }
	int  lexer_loc_pos() const                                { return parser ? parser->lexer_loc_pos() : 0;                  }
};

extern RDOParser *currParser;

} // namespace rdoParse 

#endif //RDOPARSER_PARSER
