#ifndef RDOPARSER_PARSER
#define RDOPARSER_PARSER

using namespace std;
#include "rdolexinterface.h"
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
   string getType() const { return "RDO Syntax Error"; }
   RDOSyntaxException(const char *str): RDOException(str) {}
};

struct RDOInternalException: public RDOException
{
   string getType() const { return "RDO Syntax Error"; }
   RDOInternalException(const char *str): RDOException(str) {}
};

struct RDOSMR1OkException: public RDOException
{
   string getType() const { return ""; }
   RDOSMR1OkException(const char *str): RDOException(str) {}
};

class RDOParser
{
private:
	std::ostream* out;

protected:
	std::map< int, rdoParserBase > parsers;

public:
	FileToParse fileToParse;

	vector<string *> allRTPNames;
	vector<double *> allRTPDoubles;

	vector<RDORTPParamDesc *>	allRTPParamDesc;
	vector<RDORTPResType *>	allRTPResType;

	vector<RDORSSResource *>	allRSSResource;
	
	vector<RDOFUNConstant *>	allFUNConstant;
	vector<RDOFUNFunction *>	allFUNFunctions;
	vector<RDOFUNSequence *>	allFUNSequences;

	vector<RDOPATPattern *>	allPATPatterns;

	vector<RDODPTSearch *>	allDPTSearch;
	vector<RDODPTSome *>		allDPTSome;
	vector<RDODPTFreeActivity *>		allFreeActivity;

	vector<RDODeletable *>	allDeletables;

	RDORTPResType  *lastRTPResType;
	RDORSSResource *lastRSSResource;
	RDOFUNFunction *lastFUNFunction;
	RDOPATPattern  *lastPATPattern;
	RDODPTSearch   *lastDPTSearch;
	
	vector<RDOFUNGroup *> fUNGroupStack;

	int resourceTypeCounter;
	int resourceCounter;
	RDORuntime	*runTime;

	int patternCounter;
	int pokazCounter;
	int constCounter;

	RDOSMR *smr;

	vector<RDOSimulatorNS::RDOSyntaxError> errors;

	stringstream modelStructure;
	stringstream& getModelStructure();

	RDOParser();
	~RDOParser();

	void setSMR(RDOSMR *_smr) { smr = _smr; }

	const RDORTPResType *findRTPResType(const string *const type) const;
	const RDORSSResource *findRSSResource(const string *const name) const;
	const RDOFUNFunction *findFunction(const string *const name) const;
	const RDOFUNSequence *findSequence(const string *const name) const;
	const RDOPATPattern *findPattern(const string *const name) const;
	
	void setYylval(int val);
	void parseRTP(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseRSS(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseFUN(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parsePAT(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseOPR(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseDPT(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parsePMD(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseFRM(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseSMR1(std::istream* arg_yyin, std::ostream* arg_yyout);
	void parseSMR2(std::istream* arg_yyin, std::ostream* arg_yyout);
	void error( const char *mes );
	void error( string mes ) { error(mes.c_str()); }
	int lex();
	void addConstant(RDORTPParamDesc *const _cons);
	const RDOFUNConstant *RDOParser::findFUNConst(const string *const _cons) const;

	void LoadStdFunctions();
	string *registerName(const char *name)
	{
		string *newName = new string(name);
		allRTPNames.push_back(newName);
		return newName;
	}
};

extern RDOParser *currParser;

}		// namespace rdoParse 

#endif //RDOPARSER_PARSER
