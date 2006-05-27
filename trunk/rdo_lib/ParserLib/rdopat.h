#ifndef RDOPAT_PAT
#define RDOPAT_PAT

#include "rdogramma.h"
#include "rdoStdFuncs.h"

namespace rdoRuntime
{
class RDOCalc;
class RDOPatternRuntime;
class RDOSelectResourceCommon;
}

using namespace rdoRuntime;

namespace rdoParse 
{

int patparse( void* lexer );
int patlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void paterror( char* mes );

class RDORTPResType;
class RDOPATChoice;
class RDOPATFirst;
class RDORSSResource;
class RDOPATParamsSet;
class RDOOPROperation;
class RDODPTSearchActivity;
class RDODPTSomeActivity;
class RDODPTFreeActivity;
class RDOFUNFunctionParam;
struct RDOFUNArithm;
struct RDOFUNLogic;

enum ConvertStatus
{
	CS_Keep,
	CS_Create,
	CS_Erase,
	CS_NonExist,
	CS_NoChange
};

class RDORelevantResource: public RDODeletable
{
private:
	const std::string* const name;

public:
	int numberOfResource;
	const ConvertStatus begin;
	const ConvertStatus end;
	bool alreadyHaveConverter;
	RDOPATChoice* choice;
	RDOPATFirst* first;

	RDORelevantResource( const std::string* const _name, const int _numberOfResource, const ConvertStatus _begin, const ConvertStatus _end ):
		name( _name ),
		numberOfResource( _numberOfResource ),
		begin( _begin ),
		end( _end ),
		alreadyHaveConverter( false ),
		choice( NULL ),
		first( NULL )
	{
	}

	virtual const RDORTPResType *const getType() const = 0 ;
	const std::string *const getName() const { return name; };
	virtual RDOCalc *createSelectFirstResourceCalc() = 0;			// first without choice
	virtual RDOCalc *createSelectFirstResourceChoiceCalc() = 0;	// first with choice
	virtual RDOCalc *createSelectResourceChoiceCalc() = 0;		// first/withmax/withmin with choice
	virtual RDOSelectResourceCommon *createSelectResourceCommonChoiceCalc() = 0; // for common choice
};

class RDORelevantResourceDirect: public RDORelevantResource
{
	const RDORSSResource *const res;
public:
	RDORelevantResourceDirect(const std::string *const _name, const int _numberOfResource, const RDORSSResource *const _res, const ConvertStatus _begin, const ConvertStatus _end = CS_NoChange): 
		RDORelevantResource(_name, _numberOfResource, _begin, _end), res(_res) {}
	const RDORTPResType *const getType() const;
	RDOCalc *createSelectFirstResourceCalc();
	RDOCalc *createSelectFirstResourceChoiceCalc();
	RDOCalc *createSelectResourceChoiceCalc();
	RDOSelectResourceCommon *createSelectResourceCommonChoiceCalc();
};

class RDORelevantResourceByType: public RDORelevantResource
{
	const RDORTPResType *const type;
public:
	RDORelevantResourceByType(const std::string *const _name, const int _numberOfResource, const RDORTPResType *const _type, const ConvertStatus _begin, const ConvertStatus _end = CS_NoChange): 
		RDORelevantResource(_name, _numberOfResource, _begin, _end), type(_type) {}
	const RDORTPResType *const getType() const { return type; };
	RDOCalc *createSelectFirstResourceCalc();
	RDOCalc *createSelectFirstResourceChoiceCalc();
	RDOCalc *createSelectResourceChoiceCalc();
	RDOSelectResourceCommon *createSelectResourceCommonChoiceCalc();
};

class RDOPATPattern: public RDODeletable
{
friend RDOOPROperation;
friend RDODPTSearchActivity;
friend RDODPTSomeActivity;
friend RDODPTFreeActivity;
	const bool trace;
	std::vector<RDOFUNFunctionParam *>	params;
	const std::string *const name;

	bool useCommonChoice;
	bool useCommonWithMax;
	RDOFUNArithm *commonChoice;

//	RDOFUNArithm *time;

protected:
	std::vector<RDORelevantResource *>	relRes;
	RDORelevantResource *currRelRes;

	RDOPatternRuntime *patRuntime;

protected:
	RDOPATPattern(const std::string *const _name, const bool _trace);
	virtual ~RDOPATPattern() {}

public:
	void add(RDOFUNFunctionParam *const _param);
	const RDOFUNFunctionParam *findPATPatternParam(const std::string *const paramName) const;
	int findPATPatternParamNum(const std::string *const paramName) const;
	const RDORelevantResource *findRelevantResource(const std::string *const resName) const;
	int findRelevantResourceNum(const std::string *const resName) const;
	virtual void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg, ConvertStatus end);
	virtual void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg);
	virtual void addRelRes(std::string *relName, std::string *resName, std::string *convBeg);
	const std::string *const getName() const { return name; }

	void setCommonChoiceFirst();
	void setCommonChoiceWithMin(RDOFUNArithm *arithm);
	void setCommonChoiceWithMax(RDOFUNArithm *arithm);
	virtual void setTime(RDOFUNArithm *arithm);
	void addRelResBody(std::string *resName);
	virtual void addRelResUsage(RDOPATChoice *choice, RDOPATFirst *first);
	virtual void addRelResConvert();
	virtual void addRelResConvertBegin(		bool trace, RDOPATParamsSet *parSet);
	virtual void addRelResConvertEnd(		bool trace, RDOPATParamsSet *parSet);
	virtual void addRelResConvertBeginEnd(	bool trace, RDOPATParamsSet *parSet, bool trace2, RDOPATParamsSet *parSet2);
	virtual void addRelResConvertRule(		bool trace, RDOPATParamsSet *parSet);
	virtual void addRelResConvertEvent(		bool trace, RDOPATParamsSet *parSet);
	virtual void end();

	virtual void testGoodForSearchActivity() const;
	virtual void testGoodForSomeActivity() const;
	virtual void testGoodForFreeActivity() const;

	int writeModelStructure() const;
	virtual char getModelStructureLetter() const = 0;
	std::string getPatternId() const;
};

class RDOPATPatternOperation: public RDOPATPattern
{
protected:
	RDOPATPatternOperation( bool _trace, std::string* _name );
public:
	RDOPATPatternOperation( std::string* _name, bool _trace );
	void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg, ConvertStatus end);
	void addRelRes(std::string *relName, std::string *resName, std::string *convBeg);
	void addRelResConvertBegin(	bool trace, RDOPATParamsSet *parSet);
	void addRelResConvertEnd(		bool trace, RDOPATParamsSet *parSet);
	void addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet, bool trace2, RDOPATParamsSet *parSet2);
	void testGoodForSomeActivity() const {}
	virtual char getModelStructureLetter() const { return 'A'; };
};

class RDOPATPatternEvent: public RDOPATPattern
{
public:
	RDOPATPatternEvent( std::string* _name, bool _trace );
	void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg);
	void addRelResUsage(RDOPATChoice *choice, RDOPATFirst *first);
	void addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet);
	void testGoodForFreeActivity() const {}
	char getModelStructureLetter() const { return 'I'; };
};

class RDOPATPatternRule: public RDOPATPattern
{
public:
	RDOPATPatternRule( std::string* _name, bool _trace );
	void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg);
	void setTime(RDOFUNArithm *arithm);
	void addRelResConvertRule(bool trace, RDOPATParamsSet *parSet);
	void testGoodForSearchActivity() const;
	void testGoodForSomeActivity() const {}
	char getModelStructureLetter() const { return 'R'; };
};

class RDOPATPatternKeyboard: public RDOPATPatternOperation
{
public:
	RDOPATPatternKeyboard( std::string *_name, bool _trace );
	char getModelStructureLetter() const { return 'K'; };
	void testGoodForFreeActivity() const {}
};

class RDOPATChoice: public RDODeletable
{
public:
	bool isEmpty;
	bool isNoCheck;
	RDOFUNLogic	*logic;

	RDOPATChoice()	{ isEmpty = true; }
	RDOPATChoice(int dummy)	{ isEmpty = false; isNoCheck = true; }
	RDOPATChoice(RDOFUNLogic *_logic): logic(_logic)	{ isEmpty = false; isNoCheck = false; }
};

class RDOPATFirst: public RDODeletable
{
public:
	bool isEmpty;
	bool isFirst;
	bool isWithMin;
	RDOFUNArithm	*arithm;

	RDOPATFirst()	{ isEmpty = true; }
	RDOPATFirst(int dummy)	{ isEmpty = false; isFirst = true; }
	RDOPATFirst(bool _isWithMin, RDOFUNArithm *_arithm): isWithMin(_isWithMin), arithm(_arithm)	
	{	isEmpty = false; isFirst = false; }
};

class RDOPATParamsSet: public RDODeletable
{
	std::vector<std::string *> paramNames;	
public:
	std::vector<RDOFUNArithm *> paramArithms;	
	std::vector<int> paramNumbs;	
	void addIdentif(std::string *paramName, RDOFUNArithm *paramArithm);
	void addIdentif(std::string *paramName);
	void checkParamsNumbers(RDORelevantResource *currRelRes);
};

}		// namespace rdoParse 

#endif //RDOPAT_PAT
