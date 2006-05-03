#ifndef RDOPARSER_PARSER
#define RDOPARSER_PARSER

#include "rdoStdFuncs.h"
#include "rdocalcconst.h"
#include "rdoparser_base.h"

#include <rdocommon.h>

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
class RDOPATPattern;
struct RDOFUNGroup;
class RDOSMR;
struct RDOFUNConstant;
class RDODPTSearch;
class RDODPTSome;
class RDODPTFreeActivity;

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
private:
	std::ostream* out;

protected:
	RDOParserList parsers;

public:
	rdoModelObjects::RDOFileType fileToParse;

	std::vector<std::string *> allRTPNames;
	std::vector<double *> allRTPDoubles;

	std::vector<RDORTPParamDesc *>	allRTPParamDesc;
	std::vector<RDORTPResType *>	allRTPResType;

	std::vector<RDORSSResource *>	allRSSResource;
	
	std::vector<RDOFUNConstant *>	allFUNConstant;
	std::vector<RDOFUNFunction *>	allFUNFunctions;
	std::vector<RDOFUNSequence *>	allFUNSequences;

	std::vector<RDOPATPattern *>	allPATPatterns;

	std::vector<RDODPTSearch *>	allDPTSearch;
	std::vector<RDODPTSome *>		allDPTSome;
	std::vector<RDODPTFreeActivity *>		allFreeActivity;

	std::vector<RDODeletable *>	allDeletables;

	RDORTPResType  *lastRTPResType;
	RDORSSResource *lastRSSResource;
	RDOFUNFunction *lastFUNFunction;
	RDOPATPattern  *lastPATPattern;
	RDODPTSearch   *lastDPTSearch;
	
	std::vector<RDOFUNGroup *> fUNGroupStack;

	int resourceTypeCounter;
	int resourceCounter;
	RDORuntime	*runTime;

	int patternCounter;
	int pokazCounter;
	int constCounter;

	RDOSMR *smr;

	std::vector<RDOSimulatorNS::RDOSyntaxError> errors;

	std::stringstream modelStructure;
	std::stringstream& getModelStructure();

	RDOParser();
	~RDOParser();

	void setSMR(RDOSMR *_smr) { smr = _smr; }

	const RDORTPResType *findRTPResType(const std::string *const type) const;
	const RDORSSResource *findRSSResource(const std::string *const name) const;
	const RDOFUNFunction *findFunction(const std::string *const name) const;
	const RDOFUNSequence *findSequence(const std::string *const name) const;
	const RDOPATPattern *findPattern(const std::string *const name) const;

	RDOParserBase* parser;
	void parse( int files = rdoModelObjects::obALL );
	void parse( rdoModelObjects::RDOParseType file );
	void parse( rdoModelObjects::RDOParseType file, std::istream& stream );
	void error( const char* mes );
	void error( const std::string& mes ) { error( mes.c_str() ); }
	void addConstant(RDORTPParamDesc *const _cons);
	const RDOFUNConstant *RDOParser::findFUNConst(const std::string *const _cons) const;

	void LoadStdFunctions();
	std::string *registerName(const char *name)
	{
		std::string *newName = new std::string(name);
		allRTPNames.push_back(newName);
		return newName;
	}

	void setYylval( int val )         { parser->set_lexer_value( val ); }
	void addName( std::string* name ) { allRTPNames.push_back( name );  }
	double* addDouble( double* val ) {
		allRTPDoubles.push_back( val );
		return val;
	}

};

extern RDOParser *currParser;

}		// namespace rdoParse 

#endif //RDOPARSER_PARSER
