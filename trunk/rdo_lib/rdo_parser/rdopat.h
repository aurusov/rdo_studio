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

namespace rdoParse 
{

int patparse( void* lexer );
int patlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void paterror( char* mes );

class RDORTPResType;
class RDOPATChoice;
class RDOPATSelectType;
class RDORSSResource;
class RDOPATParamsSet;
class RDOOPROperation;
class RDODPTSearchActivity;
class RDODPTSomeActivity;
class RDODPTFreeActivity;
class RDOFUNFunctionParam;
class RDOFUNArithm;
class RDOFUNLogic;

class RDORelevantResource;

class RDOPATPattern: public RDODeletable
{
friend RDOOPROperation;
friend RDODPTSearchActivity;
friend RDODPTSomeActivity;
friend RDODPTFreeActivity;
private:
	const bool trace;
	std::vector< RDOFUNFunctionParam* >	params;
	std::vector< RDORelevantResource* > relRes;
	const std::string* const name;

	bool useCommonChoice;
	bool useCommonWithMax;
	RDOFUNArithm* commonChoice;

//	RDOFUNArithm* time;

protected:
	RDOPATPattern( const std::string* const _name, const bool _trace );
	virtual ~RDOPATPattern() {}

	rdoRuntime::RDOPatternRuntime* patRuntime;

	std::map< std::string, YYLTYPE > rel_res_pos;

public:
	enum ConvertStatus {
		CS_Keep,
		CS_Create,
		CS_Erase,
		CS_NonExist,
		CS_NoChange
	};
	enum PatType {
		PT_Rule,
		PT_Operation,
		PT_IE,
		PT_Keyboard
	};
	virtual PatType getPatType() const = 0;
	bool    isHaveConvertEnd() const { return getPatType() == PT_Operation || getPatType() == PT_Keyboard; }

	static std::string StatusToStr( ConvertStatus value );
	static ConvertStatus StrToStatus( const std::string& value );

	std::vector< RDORelevantResource* >::const_iterator res_begin() const { return relRes.begin(); }
	std::vector< RDORelevantResource* >::const_iterator res_end()   const { return relRes.end();   }
	int                                                 res_count() const { return relRes.size();  }
	void RDOPATPattern::res_insert( RDORelevantResource* res );

	RDORelevantResource* currRelRes;

	void add(RDOFUNFunctionParam *const _param);
	const RDOFUNFunctionParam *findPATPatternParam(const std::string *const paramName) const;
	int findPATPatternParamNum(const std::string *const paramName) const;
	const RDORelevantResource *findRelevantResource(const std::string *const resName) const;
	int findRelevantResourceNum(const std::string *const resName) const;
	virtual void addRelRes( std::string* relName, std::string* resName, ConvertStatus beg, ConvertStatus end );
	virtual void addRelRes( std::string* relName, std::string* resName, ConvertStatus beg );
	virtual void addRelRes( std::string* relName, std::string* resName, std::string* convBeg );
	const std::string* const getName() const { return name; }
	void setRelResPos( std::string* relRes, YYLTYPE* pos );

	void setCommonChoiceFirst();
	void setCommonChoiceWithMin( RDOFUNArithm* arithm );
	void setCommonChoiceWithMax( RDOFUNArithm* arithm );
	virtual void setTime( RDOFUNArithm* arithm );
	void addRelResBody( std::string* resName );
	virtual void addRelResUsage( RDOPATChoice* choice, RDOPATSelectType* first );
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

	virtual bool needTime() const { return false; }
};

class RDORelevantResource: public RDODeletable
{
private:
	const std::string* const name;

public:
	int numberOfResource;
	const RDOPATPattern::ConvertStatus begin;
	const RDOPATPattern::ConvertStatus end;
	bool alreadyHaveConverter;
	RDOPATChoice* choice;
	RDOPATSelectType* first;
	enum { stateNone = 0, choiceEmpty, choiceNoCheck, choiceFrom, choiceOrderEmpty, choiceOrderFirst, choiceOrderWithMin, choiceOrderWithMax, convertBegin, convertEnd } currentState;
	bool isChoiceFromState() const { return currentState == choiceEmpty || currentState == choiceNoCheck || currentState == choiceFrom; }

	RDORelevantResource( const std::string* const _name, const int _numberOfResource, const RDOPATPattern::ConvertStatus _begin, const RDOPATPattern::ConvertStatus _end ):
		name( _name ),
		numberOfResource( _numberOfResource ),
		begin( _begin ),
		end( _end ),
		alreadyHaveConverter( false ),
		choice( NULL ),
		first( NULL ),
		currentState( stateNone )
	{
	}

	const std::string* const getName() const  { return name; };
	virtual const RDORTPResType* const getType() const                 = 0;
	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc()       = 0; // first without choice
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc() = 0; // first with choice
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc()      = 0; // first/withmax/withmin with choice
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc() = 0; // for common choice
	virtual bool isDirect() const  = 0;
};

class RDORelevantResourceDirect: public RDORelevantResource
{
private:
	const RDORSSResource* const res;

public:
	RDORelevantResourceDirect( const std::string* const _name, const int _numberOfResource, const RDORSSResource* const _res, const RDOPATPattern::ConvertStatus _begin, const RDOPATPattern::ConvertStatus _end = RDOPATPattern::CS_NoChange ): RDORelevantResource( _name, _numberOfResource, _begin, _end ), res( _res ) {}
	const RDORSSResource* const getResource() const { return res; }
	virtual const RDORTPResType* const getType() const;
	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc();
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc();
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc();
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc();
	virtual bool isDirect() const  { return true; }
};

class RDORelevantResourceByType: public RDORelevantResource
{
private:
	const RDORTPResType* const type;

public:
	RDORelevantResourceByType( const std::string* const _name, const int _numberOfResource, const RDORTPResType* const _type, const RDOPATPattern::ConvertStatus _begin, const RDOPATPattern::ConvertStatus _end = RDOPATPattern::CS_NoChange ): RDORelevantResource( _name, _numberOfResource, _begin, _end ), type( _type ) {}
	virtual const RDORTPResType* const getType() const { return type; };
	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc();
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc();
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc();
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc();
	virtual bool isDirect() const  { return false; }
};

class RDOPATPatternOperation: public RDOPATPattern
{
protected:
	RDOPATPatternOperation( bool _trace, std::string* _name );

public:
	RDOPATPatternOperation( std::string* _name, bool _trace );
	virtual void addRelRes( std::string* relName, std::string* resName, ConvertStatus beg, ConvertStatus end );
	virtual void addRelRes( std::string* relName, std::string* resName, std::string* convBeg );
	virtual void addRelResConvertBegin(	bool trace, RDOPATParamsSet *parSet);
	virtual void addRelResConvertEnd(		bool trace, RDOPATParamsSet *parSet);
	virtual void addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet, bool trace2, RDOPATParamsSet *parSet2);
	virtual void testGoodForSomeActivity() const {}
	virtual char getModelStructureLetter() const { return 'A'; };
	virtual bool needTime() const { return true; }
	virtual PatType getPatType() const { return PT_Operation; }
};

class RDOPATPatternEvent: public RDOPATPattern
{
public:
	RDOPATPatternEvent( std::string* _name, bool _trace );
	virtual void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg);
	virtual void addRelResUsage(RDOPATChoice *choice, RDOPATSelectType *first);
	virtual void addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet);
	virtual void testGoodForFreeActivity() const {}
	virtual char getModelStructureLetter() const { return 'I'; };
	virtual bool needTime() const { return true; }
	virtual PatType getPatType() const { return PT_IE; }
};

class RDOPATPatternRule: public RDOPATPattern
{
public:
	RDOPATPatternRule( std::string* _name, bool _trace );
	virtual void addRelRes(std::string *relName, std::string *resName, ConvertStatus beg);
	virtual void setTime(RDOFUNArithm *arithm);
	virtual void addRelResConvertRule(bool trace, RDOPATParamsSet *parSet);
	virtual void testGoodForSearchActivity() const;
	virtual void testGoodForSomeActivity() const {}
	virtual char getModelStructureLetter() const { return 'R'; };
	virtual PatType getPatType() const { return PT_Rule; }
};

class RDOPATPatternKeyboard: public RDOPATPatternOperation
{
public:
	RDOPATPatternKeyboard( std::string *_name, bool _trace );
	virtual char getModelStructureLetter() const { return 'K'; };
	virtual void testGoodForFreeActivity() const {}
	virtual PatType getPatType() const { return PT_Keyboard; }
};

class RDOPATChoice: public RDODeletable
{
public:
	enum Type {
		ch_empty = 0,
		ch_nocheck,
		ch_from
	} type;

	RDOFUNLogic* logic;

	RDOPATChoice( Type _type, RDOFUNLogic* _logic = NULL ):
		type( _type ),
		logic( _logic )
	{
	};
};

class RDOPATSelectType: public RDODeletable
{
public:
	enum Type {
		st_empty = 0,
		st_first,
		st_with_min, 
		st_with_max
	} type;

	RDOFUNArithm* arithm;

	RDOPATSelectType( Type _type, RDOFUNArithm* _arithm = NULL ):
		type( _type ),
		arithm( _arithm )
	{
	};
	std::string asString() const {
		switch ( type ) {
			case st_empty   : return "<не_указано>";
			case st_first   : return "first";
			case st_with_min: return "with_min";
			case st_with_max: return "with_max";
		}
		return "";
	}
};

class RDOPATParamsSet: public RDODeletable
{
private:
	std::vector< std::string* > paramNames;

public:
	std::vector< RDOFUNArithm* > paramArithms;
	std::vector< int >           paramNumbs;
	void addIdentif( std::string* paramName, RDOFUNArithm* paramArithm, const YYLTYPE& param_name_pos );
	void addIdentif( std::string* paramName, const YYLTYPE& param_name_pos );
	void checkParamsNumbers( std::string* paramName, const YYLTYPE& param_name_pos );
	void checkParamsNumbers( RDORelevantResource* currRelRes );
};

} // namespace rdoParse

#endif // RDOPAT_PAT
