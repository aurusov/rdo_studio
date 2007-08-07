#ifndef RDOPAT_PAT
#define RDOPAT_PAT

#include "rdoparser_object.h"
#include <rdotrace.h>
#include <rdocalc.h>

namespace rdoRuntime
{
class RDOPatternRuntime;
}

namespace rdoParse 
{

int patparse( void* lexer );
int patlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void paterror( char* mes );

class RDORTPResType;
class RDOPATChoiceFrom;
class RDOPATChoiceOrder;
class RDORSSResource;
class RDOPATParamSet;
class RDOOPROperation;
class RDODPTSearchActivity;
class RDODPTSomeActivity;
class RDODPTFreeActivity;
class RDOFUNFunctionParam;
class RDOFUNArithm;
class RDOFUNLogic;
class RDORelevantResource;

// ----------------------------------------------------------------------------
// ---------- RDOPATPattern
// ----------------------------------------------------------------------------
class RDOPATPattern: public RDOParserObject
{
friend RDOOPROperation;
friend RDODPTSearchActivity;
friend RDODPTSomeActivity;
friend RDODPTFreeActivity;
private:
	const bool trace;
	std::vector< RDOFUNFunctionParam* > params;
	std::vector< RDORelevantResource* > relRes;
	std::string name;

	bool useCommonChoice;
	bool useCommonWithMax;
	RDOFUNArithm* commonChoice;

	int current_rel_res_index;

protected:
	RDOPATPattern( RDOParser* _parser, const std::string* const _name, const bool _trace );
	virtual ~RDOPATPattern() {}

	rdoRuntime::RDOPatternRuntime* patRuntime;

	virtual std::string getPatTypeStr() const = 0;

public:
	enum PatType {
		PT_IE,
		PT_Rule,
		PT_Operation,
		PT_Keyboard
	};
	virtual PatType getPatType() const = 0;
	bool    isHaveConvertEnd() const { return getPatType() == PT_Operation || getPatType() == PT_Keyboard; }

	static std::string StatusToStr( rdoRuntime::RDOResourceTrace::ConvertStatus value );
	rdoRuntime::RDOResourceTrace::ConvertStatus StrToStatus( const std::string& value, const YYLTYPE& convertor_pos );

	std::vector< RDORelevantResource* >::const_iterator rel_res_begin() const { return relRes.begin(); }
	std::vector< RDORelevantResource* >::const_iterator rel_res_end()   const { return relRes.end();   }
	int                                                 rel_res_count() const { return relRes.size();  }
	void beforeRelRensert( const std::string& rel_res_name, const YYLTYPE& pos );
	virtual void rel_res_insert( RDORelevantResource* rel_res, const YYLTYPE& pos );

	RDORelevantResource* currRelRes;

	void add(RDOFUNFunctionParam *const _param);
	const RDOFUNFunctionParam *findPATPatternParam(const std::string *const paramName) const;
	int findPATPatternParamNum(const std::string *const paramName) const;
	const RDORelevantResource* findRelevantResource( const std::string& resName ) const;
	int findRelevantResourceNum( const std::string& resName ) const;
	virtual void addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos ) = 0;
	const std::string& getName() const { return name; }

	void setCommonChoiceFirst();
	void setCommonChoiceWithMin( RDOFUNArithm* arithm );
	void setCommonChoiceWithMax( RDOFUNArithm* arithm );
	virtual void setTime( RDOFUNArithm* arithm );
	void addRelResBody( std::string* resName );
	virtual void addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order );
	virtual void addRelResConvert();
	virtual void addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos ) = 0;
	virtual void end();

	virtual void testGoodForSearchActivity() const;
	virtual void testGoodForSomeActivity() const;
	virtual void testGoodForFreeActivity() const;

	int writeModelStructure() const;
	virtual char getModelStructureLetter() const = 0;
	std::string getPatternId() const;

	virtual bool needTime() const { return false; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternEvent
// ----------------------------------------------------------------------------
class RDOPATPatternEvent: public RDOPATPattern
{
protected:
	virtual std::string getPatTypeStr() const { return "irregular event"; }

public:
	RDOPATPatternEvent( RDOParser* _parser, std::string* _name, bool _trace );
	virtual void addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos );
	virtual void addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order );
	virtual void addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos );
	virtual void testGoodForFreeActivity() const {}
	virtual char getModelStructureLetter() const { return 'I'; };
	virtual bool needTime() const { return true; }
	virtual PatType getPatType() const { return PT_IE; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternRule
// ----------------------------------------------------------------------------
class RDOPATPatternRule: public RDOPATPattern
{
protected:
	virtual std::string getPatTypeStr() const { return "rule"; }

public:
	RDOPATPatternRule( RDOParser* _parser, std::string* _name, bool _trace );
	virtual void addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos );
	virtual void setTime(RDOFUNArithm *arithm);
	virtual void addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos );
	virtual void testGoodForSearchActivity() const;
	virtual void testGoodForSomeActivity() const {}
	virtual char getModelStructureLetter() const { return 'R'; };
	virtual PatType getPatType() const { return PT_Rule; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternOperation
// ----------------------------------------------------------------------------
class RDOPATPatternOperation: public RDOPATPattern
{
protected:
	RDOPATPatternOperation( RDOParser* _parser, bool _trace, std::string* _name );
	virtual std::string getPatTypeStr() const { return "operation"; }
	virtual void rel_res_insert( RDORelevantResource* rel_res, const YYLTYPE& pos );

public:
	RDOPATPatternOperation( RDOParser* _parser, std::string* _name, bool _trace );
	virtual void addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_pos );
	        void addRelRes( std::string* relName, std::string* typeName, rdoRuntime::RDOResourceTrace::ConvertStatus beg, rdoRuntime::RDOResourceTrace::ConvertStatus end, const YYLTYPE& rel_pos, const YYLTYPE& type_pos, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos );
	virtual void addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos );
	        void addRelResConvertEnd( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos );
	        void addRelResConvertBeginEnd( bool trace, RDOPATParamSet* parSet, bool trace2, RDOPATParamSet* parSet2, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos );
	virtual void testGoodForSomeActivity() const {}
	virtual char getModelStructureLetter() const { return 'A'; };
	virtual bool needTime() const { return true; }
	virtual PatType getPatType() const { return PT_Operation; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternKeyboard
// ----------------------------------------------------------------------------
class RDOPATPatternKeyboard: public RDOPATPatternOperation
{
protected:
	virtual std::string getPatTypeStr() const { return "keyboard"; }

public:
	RDOPATPatternKeyboard( RDOParser* _parser, std::string *_name, bool _trace );
	virtual char getModelStructureLetter() const { return 'K'; };
	virtual void testGoodForFreeActivity() const {}
	virtual PatType getPatType() const { return PT_Keyboard; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATChoiceFrom
// ----------------------------------------------------------------------------
class RDOPATChoiceFrom: public RDODeletable, public RDOParserSrcInfo
{
public:
	enum Type {
		ch_empty = 0,
		ch_nocheck,
		ch_from
	} type;

	RDOFUNLogic* logic;

	RDOPATChoiceFrom( Type _type, RDOFUNLogic* _logic = NULL ):
		type( _type ),
		logic( _logic )
	{
	};
	std::string asString() const {
		switch ( type ) {
			case ch_empty  : return "<Choice_не_указан>";
			case ch_nocheck: return "Choice NoCheck";
			case ch_from   : return "Choice from";
		}
		return "";
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPATChoiceOrder
// ----------------------------------------------------------------------------
class RDOPATChoiceOrder: public RDODeletable, public RDOParserSrcInfo
{
public:
	rdoRuntime::RDOSelectResourceCalc::Type type;

	RDOFUNArithm* arithm;

	RDOPATChoiceOrder( rdoRuntime::RDOSelectResourceCalc::Type _type, RDOFUNArithm* _arithm = NULL ):
		type( _type ),
		arithm( _arithm )
	{
	};
	std::string asString() const {
		switch ( type ) {
			case rdoRuntime::RDOSelectResourceCalc::st_empty   : return "<правило_выбора_не_указано>";
			case rdoRuntime::RDOSelectResourceCalc::st_first   : return "first";
			case rdoRuntime::RDOSelectResourceCalc::st_with_min: return "with_min";
			case rdoRuntime::RDOSelectResourceCalc::st_with_max: return "with_max";
		}
		return "";
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
class RDORelevantResource: public RDOParserObject, public RDOParserSrcInfo
{
private:
	std::string name;

protected:
	rdoRuntime::RDOCalc* getChoiceCalc() const;
	rdoRuntime::RDOCalc* getSelectCalc() const;
	rdoRuntime::RDOSelectResourceCalc::Type getSelectType() const;

public:
	int                rel_res_id;
	bool               alreadyHaveConverter;
	RDOPATChoiceFrom*  choice_from;
	RDOPATChoiceOrder* choice_order;
	const rdoRuntime::RDOResourceTrace::ConvertStatus begin;
	const rdoRuntime::RDOResourceTrace::ConvertStatus end;
	enum {
		stateNone = 0,
		choiceEmpty,
		choiceNoCheck,
		choiceFrom,
		choiceOrderEmpty,
		choiceOrderFirst,
		choiceOrderWithMin,
		choiceOrderWithMax,
		convertBegin,
		convertEnd
	} currentState;
	bool isChoiceFromState() const { return currentState == choiceEmpty || currentState == choiceNoCheck || currentState == choiceFrom; }

	RDORelevantResource( const RDOParserObject* _parent, const std::string* const _name, const int _rel_res_id, const rdoRuntime::RDOResourceTrace::ConvertStatus _begin, const rdoRuntime::RDOResourceTrace::ConvertStatus _end ):
		RDOParserObject( _parent ),
		name( *_name ),
		rel_res_id( _rel_res_id ),
		begin( _begin ),
		end( _end ),
		alreadyHaveConverter( false ),
		choice_from( NULL ),
		choice_order( NULL ),
		currentState( stateNone )
	{
	}

	const std::string& getName() const  { return name; };
	virtual const RDORTPResType* const getType() const                 = 0;

	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc()       = 0; // first without choice
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc() = 0; // first with choice
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc()      = 0; // first/withmax/withmin with choice
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc() = 0; // for common choice

	virtual bool isDirect() const                                      = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - по имени ресурса
// ----------------------------------------------------------------------------
class RDORelevantResourceDirect: public RDORelevantResource
{
private:
	const RDORSSResource* const res;

public:
	RDORelevantResourceDirect( const RDOParserObject* _parent, const std::string* const _name, const int _rel_res_id, const RDORSSResource* const _res, const rdoRuntime::RDOResourceTrace::ConvertStatus _begin, const rdoRuntime::RDOResourceTrace::ConvertStatus _end = rdoRuntime::RDOResourceTrace::CS_NoChange ):
		RDORelevantResource( _parent, _name, _rel_res_id, _begin, _end ),
		res( _res )
	{
	}
	const RDORSSResource* const getResource() const { return res; }
	virtual const RDORTPResType* const getType() const;
	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc();
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc();
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc();
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc();
	virtual bool isDirect() const  { return true; }
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceByType - по имени типа
// ----------------------------------------------------------------------------
class RDORelevantResourceByType: public RDORelevantResource
{
private:
	const RDORTPResType* const type;

public:
	RDORelevantResourceByType( const RDOParserObject* _parent, const std::string* const _name, const int _rel_res_id, const RDORTPResType* const _type, const rdoRuntime::RDOResourceTrace::ConvertStatus _begin, const rdoRuntime::RDOResourceTrace::ConvertStatus _end = rdoRuntime::RDOResourceTrace::CS_NoChange ): RDORelevantResource( _parent, _name, _rel_res_id, _begin, _end ), type( _type ) {}
	virtual const RDORTPResType* const getType() const { return type; };
	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc();
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc();
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc();
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc();
	virtual bool isDirect() const  { return false; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATParamSet - все операторы set для одного рел. ресурса
// ----------------------------------------------------------------------------
class RDOPATParamSet: public RDOParserObject, public RDOParserSrcInfo
{
private:
	const RDORelevantResource*  rel_res;
	std::vector< std::string* > paramNames;
	void checkParam( std::string* paramName );

public:
	RDOPATParamSet( const RDORelevantResource* _parent ):
		RDOParserObject( _parent ),
		rel_res( _parent )
	{
	}
	std::vector< RDOFUNArithm* > paramArithms;
	std::vector< int >           paramNumbs;
	void addSet( std::string* paramName, const YYLTYPE& param_name_pos, RDOFUNArithm* paramArithm = NULL );
	void setParamsNumbers();
};

} // namespace rdoParse

#endif // RDOPAT_PAT
