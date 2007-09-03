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
class RDOPATPattern: public RDOParserObject, public RDOParserSrcInfo
{
friend RDOOPROperation;
friend RDODPTSearchActivity;
friend RDODPTSomeActivity;
friend RDODPTFreeActivity;
private:
	const bool trace;
	std::vector< RDOFUNFunctionParam* > params;
	std::vector< RDORelevantResource* > relRes;

	bool useCommonChoice;
	bool useCommonWithMax;
	RDOFUNArithm* commonChoice;

	int current_rel_res_index;

protected:
	RDOPATPattern( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, const bool _trace );
	virtual ~RDOPATPattern() {}

	rdoRuntime::RDOPatternRuntime* patRuntime;

	rdoRuntime::RDOCalc* createRelRes( const RDOPATParamSet* const parSet, bool trace ) const;
	virtual void addParamSetCalc( const RDOPATParamSet* const parSet, rdoRuntime::RDOCalc* calc );
	virtual std::string getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet ) = 0;
	virtual std::string getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet ) = 0;

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
	void beforeRelRensert( const RDOParserSrcInfo& rel_info );
	virtual void rel_res_insert( RDORelevantResource* rel_res );

	RDORelevantResource* currRelRes;

	void add(RDOFUNFunctionParam *const _param);
	const RDOFUNFunctionParam* findPATPatternParam( const std::string& paramName ) const;
	int findPATPatternParamNum( const std::string& paramName ) const;
	const RDORelevantResource* findRelevantResource( const std::string& resName ) const;
	int findRelevantResourceNum( const std::string& resName ) const;
	virtual void addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos ) = 0;
	const std::string& getName() const { return src_text(); }

	void setCommonChoiceFirst();
	void setCommonChoiceWithMin( RDOFUNArithm* arithm );
	void setCommonChoiceWithMax( RDOFUNArithm* arithm );
	virtual void setTime( RDOFUNArithm* arithm );
	void addRelResBody( const RDOParserSrcInfo& body_name );
	virtual void addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order );
	void addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos );
	void end();

	virtual void testGoodForSearchActivity() const;
	virtual void testGoodForSomeActivity() const;
	virtual void testGoodForFreeActivity() const;

	int writeModelStructure() const;
	virtual char getModelStructureLetter() const = 0;
	std::string getPatternId() const;
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternEvent
// ----------------------------------------------------------------------------
class RDOPATPatternEvent: public RDOPATPattern
{
protected:
	virtual std::string getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet );
	virtual std::string getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet );

public:
	RDOPATPatternEvent( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace );
	virtual void addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos );
	virtual void addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order );
	virtual void testGoodForFreeActivity() const {}
	virtual char getModelStructureLetter() const { return 'I'; };
	virtual PatType getPatType() const { return PT_IE; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternRule
// ----------------------------------------------------------------------------
class RDOPATPatternRule: public RDOPATPattern
{
protected:
	virtual std::string getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet );
	virtual std::string getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet );

public:
	RDOPATPatternRule( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace );
	virtual void addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos );
	virtual void setTime( RDOFUNArithm* arithm );
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
	// Конструктор вызывается из RDOPATPatternKeyboard
	RDOPATPatternOperation( RDOParser* _parser, bool _trace, const RDOParserSrcInfo& _name_src_info );
	virtual void rel_res_insert( RDORelevantResource* rel_res );
	virtual void addParamSetCalc( const RDOPATParamSet* const parSet, rdoRuntime::RDOCalc* calc );
	virtual std::string getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet );
	virtual std::string getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet );

public:
	enum ConvertorType {
		convert_unknow,
		convert_begin,
		convert_end
	};

	RDOPATPatternOperation( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace );
	virtual void addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, const YYLTYPE& convertor_pos );
	        void addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResourceTrace::ConvertStatus beg, rdoRuntime::RDOResourceTrace::ConvertStatus end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos );
	        void addRelResConvertBeginEnd( bool trace_begin, RDOPATParamSet* parSet_begin, bool trace_end, RDOPATParamSet* parSet_end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos, const YYLTYPE& trace_begin_pos, const YYLTYPE& trace_end_pos );
	virtual void testGoodForSomeActivity() const {}
	virtual char getModelStructureLetter() const { return 'A'; };
	virtual PatType getPatType() const { return PT_Operation; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPATPatternKeyboard
// ----------------------------------------------------------------------------
class RDOPATPatternKeyboard: public RDOPATPatternOperation
{
public:
	RDOPATPatternKeyboard( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace );
	virtual char getModelStructureLetter() const { return 'K'; };
	virtual void testGoodForFreeActivity() const {}
	virtual PatType getPatType() const { return PT_Keyboard; }
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
class RDORelevantResource: public RDOParserObject, public RDOParserSrcInfo
{
private:
	std::string     name;
	RDOPATParamSet* param_set_begin;
	RDOPATParamSet* param_set_end;

protected:
	rdoRuntime::RDOCalc* getChoiceCalc() const;
	rdoRuntime::RDOCalc* getSelectCalc() const;
	rdoRuntime::RDOSelectResourceCalc::Type getSelectType() const;

public:
	int                rel_res_id;
	bool               alreadyHaveConverter;
	RDOPATChoiceFrom*  choice_from;
	RDOPATChoiceOrder* choice_order;
	RDOParserSrcInfo   body_name;
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

	RDORelevantResource( const RDOPATPattern* _parent, const std::string& _name, const int _rel_res_id, const rdoRuntime::RDOResourceTrace::ConvertStatus _begin, const rdoRuntime::RDOResourceTrace::ConvertStatus _end ):
		RDOParserObject( _parent ),
		name( _name ),
		rel_res_id( _rel_res_id ),
		begin( _begin ),
		end( _end ),
		alreadyHaveConverter( false ),
		choice_from( NULL ),
		choice_order( NULL ),
		param_set_begin( NULL ),
		param_set_end( NULL ),
		currentState( stateNone )
	{
	}

	const std::string& getName() const  { return name; };
	virtual const RDORTPResType* const getType() const                 = 0;

	virtual rdoRuntime::RDOCalc* createSelectEmptyResourceCalc()       = 0; // Предварительный выбор ресурсов в самом списке рел. ресурсов
	virtual rdoRuntime::RDOCalc* createSelectResourceChoiceCalc()      = 0; // Самый обыкновенный choice from + first/with_min/with_max
	virtual rdoRuntime::RDOCalc* createSelectFirstResourceChoiceCalc() = 0; // common first, который не пашет
	virtual rdoRuntime::RDOSelectResourceCommon* createSelectResourceCommonChoiceCalc() = 0; // common with_min/with_max

	virtual bool isDirect() const                                      = 0;

	RDOPATParamSet* createParamSet();
	void deleteParamSetBegin();
	const RDOPATParamSet* const getParamSetBegin() const { return param_set_begin; }
	const RDOPATParamSet* const getParamSetEnd() const   { return param_set_end;   }
	RDOPATPatternOperation::ConvertorType getConvertorType( const RDOPATParamSet* const param_set ) const {
		if ( param_set == param_set_begin ) {
			return RDOPATPatternOperation::convert_begin;
		} else if ( param_set == param_set_end ) {
			return RDOPATPatternOperation::convert_end;
		} else {
			return RDOPATPatternOperation::convert_unknow;
		}
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPATChoiceFrom
// ----------------------------------------------------------------------------
class RDOPATChoiceFrom: public RDOParserObject, public RDOParserSrcInfo
{
public:
	enum Type {
		ch_empty = 0,
		ch_nocheck,
		ch_from
	} type;

	RDOFUNLogic* logic;

	RDOPATChoiceFrom( const RDORelevantResource* _rel_res, const RDOParserSrcInfo& _src_info, Type _type, RDOFUNLogic* _logic = NULL ):
		RDOParserObject( _rel_res ),
		RDOParserSrcInfo( _src_info ),
		type( _type ),
		logic( _logic )
	{
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPATChoiceOrder
// ----------------------------------------------------------------------------
class RDOPATChoiceOrder: public RDOParserObject, public RDOParserSrcInfo
{
public:
	rdoRuntime::RDOSelectResourceCalc::Type type;

	RDOFUNArithm* arithm;

	RDOPATChoiceOrder( const RDORelevantResource* _rel_res, const RDOParserSrcInfo& _src_info, rdoRuntime::RDOSelectResourceCalc::Type _type, RDOFUNArithm* _arithm = NULL ):
		RDOParserObject( _rel_res ),
		RDOParserSrcInfo( _src_info ),
		type( _type ),
		arithm( _arithm )
	{
	}
	std::string asString() const {
		switch ( type ) {
			case rdoRuntime::RDOSelectResourceCalc::order_empty   : return "<правило_выбора_не_указано>";
			case rdoRuntime::RDOSelectResourceCalc::order_first   : return "first";
			case rdoRuntime::RDOSelectResourceCalc::order_with_min: return "with_min";
			case rdoRuntime::RDOSelectResourceCalc::order_with_max: return "with_max";
		}
		return "";
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - по имени ресурса
// ----------------------------------------------------------------------------
class RDORelevantResourceDirect: public RDORelevantResource
{
private:
	const RDORSSResource* const res;

public:
	RDORelevantResourceDirect( const RDOPATPattern* _parent, const std::string& _name, const int _rel_res_id, const RDORSSResource* const _res, const rdoRuntime::RDOResourceTrace::ConvertStatus _begin, const rdoRuntime::RDOResourceTrace::ConvertStatus _end = rdoRuntime::RDOResourceTrace::CS_NoChange ):
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
	RDORelevantResourceByType( const RDOPATPattern* _parent, const std::string& _name, const int _rel_res_id, const RDORTPResType* const _type, const rdoRuntime::RDOResourceTrace::ConvertStatus _begin, const rdoRuntime::RDOResourceTrace::ConvertStatus _end = rdoRuntime::RDOResourceTrace::CS_NoChange ):
		RDORelevantResource( _parent, _name, _rel_res_id, _begin, _end ),
		type( _type )
	{
	}
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
public:
	struct param_set {
		std::string   name;
		int           index;
		RDOFUNArithm* arithm;
		param_set(): name( "" ), index( -1 ), arithm( NULL ) {}
		param_set( const std::string& _name, int _index, RDOFUNArithm* _arithm ): name( _name ), index( _index ), arithm( _arithm ) {}
	};
	rdoRuntime::RDOResourceTrace::ConvertStatus convert_status;

	RDOPATParamSet( const RDORelevantResource* _parent, rdoRuntime::RDOResourceTrace::ConvertStatus _convert_status ):
		RDOParserObject( _parent ),
		convert_status( _convert_status )
	{
	}
	std::vector< param_set > params;
	void addSet( const std::string& paramName, const YYLTYPE& param_name_pos, RDOFUNArithm* paramArithm = NULL );
	bool isExist( const std::string& paramName ) const {
		std::vector< param_set >::const_iterator it = params.begin();
		while ( it != params.end() ) {
			if ( it->name == paramName ) {
				return true;
			}
			it++;
		}
		return false;
	}
	const RDORelevantResource* getRelRes() const { return static_cast<const RDORelevantResource*>(getParent()); }
};

} // namespace rdoParse

#endif // RDOPAT_PAT
