#ifndef RDOPARSER_PARSER
#define RDOPARSER_PARSER

using namespace std;
#include "rdolexinterface.h"
#include "rdoStdFuncs.h"
#include "rdosimcommon.h"

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

struct RDOSMR1OkException: public RDOException
{
   string getType() const { return ""; }
   RDOSMR1OkException(const char *str): RDOException(str) {}
};

class RDOParser
{
	ostream* out;
public:
	RDOSimulatorNS::FileToParse fileToParse;

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
	
	vector<RDOFUNGroup *> fUNGroupStack;

	int resourceTypeCounter;
	int resourceCounter;
	RDORuntime	*runTime;

	int patternCounter;
	int pokazCounter;
	int constCounter;

	RDOSMR *smr;

	vector<RDOSimulatorNS::RDOSyntaxError> errors;

	
	RDOParser();
	~RDOParser();

	void setSMR(RDOSMR *_smr) { smr = _smr; }

	const RDORTPResType *findRTPResType(const string *const type) const;
	const RDORSSResource *findRSSResource(const string *const name) const;
	const RDOFUNFunction *findFunction(const string *const name) const;
	const RDOFUNSequence *findSequence(const string *const name) const;
	const RDOPATPattern *findPattern(const string *const name) const;
	
	void setYylval(int val);
   void parseRTP(istream* arg_yyin, ostream* arg_yyout);
   void parseRSS(istream* arg_yyin, ostream* arg_yyout);
   void parseFUN(istream* arg_yyin, ostream* arg_yyout);
   void parsePAT(istream* arg_yyin, ostream* arg_yyout);
   void parseOPR(istream* arg_yyin, ostream* arg_yyout);
   void parseDPT(istream* arg_yyin, ostream* arg_yyout);
   void parsePMD(istream* arg_yyin, ostream* arg_yyout);
   void parseFRM(istream* arg_yyin, ostream* arg_yyout);
   void parseSMR1(istream* arg_yyin, ostream* arg_yyout);
   void parseSMR2(istream* arg_yyin, ostream* arg_yyout);
	void error( const char *mes );
	void error( string mes ) { error(mes.c_str()); }
	int lex(void);
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
