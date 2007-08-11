#ifndef RDOFUN_FUN
#define RDOFUN_FUN

#include "rdoparser_object.h"
#include "rdortp.h"
#include <rdoruntime_object.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOCalcConst;
class RDOCalcIsEqual;
class RDOCalcFuncParam;
class RDOFunCalc;
class RDOCalcSeqInit;
class RDOCalcSeqNext;
class RDOFunCalcSelect;
}

namespace rdoParse 
{

int funparse( void* lexer );
int funlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void funerror( char* mes );

class RDORTPParamType;
class RDORTPEnum;
class RDORTPResType;
class RDORTPParam;

class RDOFUNFunctionParam: public RDODeletable, public RDOParserSrcInfo
{
private:
	std::string      name;
	RDORTPParamType* type;

public:
	RDOFUNFunctionParam( const std::string& _name, RDORTPParamType* _type ):
		name( _name ),
		type( _type )
	{
	}
	const std::string& getName() const           { return name; };
	const RDORTPParamType* const getType() const { return type; };
};

class RDOFUNFunctionListElement: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNFunctionListElement( const RDOParserObject* _parent ): RDOParserObject( _parent ) {}
	virtual ~RDOFUNFunctionListElement() {}
	virtual rdoRuntime::RDOCalcConst*   createResultCalc( const RDORTPParamType* const retType ) const = 0;
	virtual rdoRuntime::RDOCalcIsEqual* createIsEqualCalc( const RDOFUNFunctionParam* const param, const rdoRuntime::RDOCalcFuncParam* const funcParam ) const;
	virtual bool isEquivalence() const { return false; }
};

class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
public:
	std::string value;
	RDOFUNFunctionListElementIdentif( const RDOParserObject* _parent, const std::string& _value ):
		RDOFUNFunctionListElement( _parent ),
		value( _value )
	{
	}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPParamType *const retType) const;
};

class RDOFUNFunctionListElementReal: public RDOFUNFunctionListElement
{
public:
	double* value;
	RDOFUNFunctionListElementReal( const RDOParserObject* _parent, double* _value ):
		RDOFUNFunctionListElement( _parent ),
		value( _value )
	{
	}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPParamType *const retType) const;
};

class RDOFUNFunctionListElementInt: public RDOFUNFunctionListElement
{
public:
	int value;
	RDOFUNFunctionListElementInt( const RDOParserObject* _parent, int _value ):
		RDOFUNFunctionListElement( _parent ),
		value( _value )
	{
	}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPParamType *const retType) const;
};

class RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
public:
	RDOFUNFunctionListElementEq( const RDOParserObject* _parent ): RDOFUNFunctionListElement( _parent ) {}
	bool isEquivalence() const { return true; }
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPParamType *const retType) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
class RDOFUNLogic: public RDODeletable, public RDOParserSrcInfo
{
public:
	rdoRuntime::RDOCalc* calc;

	RDOFUNLogic( rdoRuntime::RDOCalc *_calc );

	RDOFUNLogic* operator &&( const RDOFUNLogic& second );
	RDOFUNLogic* operator ||( const RDOFUNLogic& second );
	RDOFUNLogic* operator_not();

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end );
	virtual void setSrcPos( const YYLTYPE& _error_pos );
	virtual void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end );
	virtual void setSrcPos( int first_line, int first_pos, int last_line, int last_pos );
	virtual void setSrcText( const std::string& value );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
class RDOFUNArithm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPParamType::ParamType type; // 0 - int, 1 - real, 2 - enum, 3 - string
	RDORTPEnum* enu; // for type == 2
	std::string str; // for type == 3

private:
	rdoRuntime::RDOCalc* calc;

	void init( const RDOParserSrcInfo& res_name_error_pos, const RDOParserSrcInfo& par_name_error_pos );
	void init( const std::string& value, const YYLTYPE& _pos );

public:
	RDOFUNArithm( RDOParser* _parser, RDORTPParamType::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( RDOParser* _parser, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info );
	RDOFUNArithm( RDOParser* _parser, int value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( RDOParser* _parser, double* value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( RDOParser* _parser, const std::string& value, const YYLTYPE& _pos );

	RDOFUNArithm( const RDOParserObject* _parent, RDORTPParamType::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( const RDOParserObject* _parent, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info );
	RDOFUNArithm( const RDOParserObject* _parent, int value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( const RDOParserObject* _parent, double* value, const RDOParserSrcInfo& src_info );
	RDOFUNArithm( const RDOParserObject* _parent, const std::string& value, const YYLTYPE& _pos );

	RDOFUNArithm* operator +( RDOFUNArithm& second );
	RDOFUNArithm* operator -( RDOFUNArithm& second );
	RDOFUNArithm* operator *( RDOFUNArithm& second );
	RDOFUNArithm* operator /( RDOFUNArithm& second );

	RDOFUNLogic* operator ==( RDOFUNArithm& second );
	RDOFUNLogic* operator !=( RDOFUNArithm& second );
	RDOFUNLogic* operator < ( RDOFUNArithm& second );
	RDOFUNLogic* operator > ( RDOFUNArithm& second );
	RDOFUNLogic* operator <=( RDOFUNArithm& second );
	RDOFUNLogic* operator >=( RDOFUNArithm& second );

	rdoRuntime::RDOCalc* createCalc( const RDORTPParamType* const forType = NULL );
	RDORTPParamType::ParamType getType() const { return type; }

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end );
	virtual void setSrcPos( const YYLTYPE& _error_pos );
	virtual void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end );
	virtual void setSrcPos( int first_line, int first_pos, int last_line, int last_pos );
	virtual void setSrcText( const std::string& value );
};

class RDOFUNCalculateIf: public RDOParserObject
{
public:
	RDOFUNLogic*  condition;
	std::string   funName;
	RDOFUNArithm* action;

	RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, const std::string& _funName, RDOFUNArithm* _action );
};

class RDOParser;
class RDOFUNFunction: public RDOParserObject
{
friend RDOParser;
private:
	std::string name;
	const RDORTPParamType *const retType;
	std::vector<const RDOFUNFunctionParam *>	   params;
	std::vector<const RDOFUNFunctionListElement *> listElems;	// for list and table
	std::vector<const RDOFUNCalculateIf *>	calculateIf;	// for algorithmic
	rdoRuntime::RDOFunCalc *functionCalc;

public:
	RDOFUNFunction( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _retType );
	void add(const RDOFUNFunctionParam *const _param);
	void add(const RDOFUNFunctionListElement *const _listElement);
	void add(const RDOFUNCalculateIf *const _calculateIf);
	const RDOFUNFunctionParam* const findFUNFunctionParam( const std::string& paramName ) const;
	int findFUNFunctionParamNum( const std::string& paramName ) const;
	void createListCalc();
	void createTableCalc();
	void createAlgorithmicCalc();
	const std::string& getName() const { return name; }
	const std::vector<const RDOFUNFunctionParam *> getParams() const { return params; }
	const rdoRuntime::RDOFunCalc *getFunctionCalc() const { return functionCalc; }
	const RDORTPParamType *const getType() const { return retType; }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
class RDOFUNParams: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNParams( RDOParser* _parser )            : RDOParserObject( _parser ) {}
	RDOFUNParams( const RDOParserObject* _parent ): RDOParserObject( _parent ) {}
	RDOParserSrcInfo name_error_pos;
	std::vector< RDOFUNArithm* > params;
	void addParameter( RDOFUNArithm* param ) {
		params.push_back( param );
	}
	RDOFUNArithm* createCall( const std::string& funName ) const;
	RDOFUNArithm* createSeqCall( const std::string& seqName ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
class RDOFUNGroup: public RDOParserObject, public RDOParserSrcInfo
{
protected:
	void init( const std::string& _resType );

public:
	const RDORTPResType* resType;

	RDOFUNGroup( RDOParser* _parser, const std::string& _resTypeName );
	RDOFUNGroup( const RDOParserObject* _parent, const std::string& _resTypeName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
class RDOFUNGroupLogic: public RDOFUNGroup
{
public:
	const int funType;

	RDOFUNGroupLogic( RDOParser* _parser, int _funType, const std::string& _resTypeName ):
		RDOFUNGroup( _parser, _resTypeName ),
		funType( _funType )
	{
	}
	RDOFUNGroupLogic( const RDOParserObject* _parent, int _funType, const std::string& _resTypeName ):
		RDOFUNGroup( _parent, _resTypeName ),
		funType( _funType )
	{
	}
	RDOFUNLogic* createFunLogic( RDOFUNLogic* cond );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
class RDOFUNSelect: public RDOFUNGroup
{
private:
	rdoRuntime::RDOFunCalcSelect* select;

public:
	RDOFUNSelect( RDOParser* _parser, const std::string& _resTypeName ):
		RDOFUNGroup( _parser, _resTypeName ),
		select( NULL )
	{
	}
/*
todo: а зачем ?
	RDOFUNSelect( const RDOParserObject* _parent, const std::string& _resTypeName ):
		RDOFUNGroup( _parent, _resTypeName ),
		select( NULL )
	{
		setSrcText( "Select( " + *_resTypeName + ": " );
	}
*/
	RDOFUNLogic* createFunSelect( RDOFUNLogic* cond = NULL );
	RDOFUNLogic* createFunSelectGroup( int funType, RDOFUNLogic* cond );
	RDOFUNLogic* createFunSelectEmpty();
	RDOFUNArithm* createFunSelectSize();

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const YYLTYPE& _error_pos );
	virtual void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end );
	virtual void setSrcPos( int first_line, int first_pos, int last_line, int last_pos );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
class RDOFUNSequence: public RDOParserObject
{
public:
	class RDOFUNSequenceHeader: public RDODeletable, public RDOParserSrcInfo
	{
	public:
		std::string      name;
		RDORTPParamType* type;
		RDOFUNSequenceHeader( const std::string& _name, RDORTPParamType* _type, const RDOParserSrcInfo& _src_info ):
			name(_name),
			type(_type)
		{
			setSrcInfo( _src_info );
		}
	};

	RDOFUNSequenceHeader* header;
	int base;
	rdoRuntime::RDOCalcSeqInit* initSeq;
	rdoRuntime::RDOCalcSeqNext* next;

protected:
	RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base );
	void initResult();
	void initCalcSrcInfo();

public:
	const std::string& getName() const  { return header->name; }
	virtual void createCalcs() = 0;
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
class RDOFUNSequenceUniform: public RDOFUNSequence
{
public:
	RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	virtual void createCalcs();
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
class RDOFUNSequenceExponential: public RDOFUNSequence
{
public:
	RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base =123456789 );
	virtual void createCalcs();
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
class RDOFUNSequenceNormal: public RDOFUNSequence
{
public:
	RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	virtual void createCalcs();
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	class RDOFUNSequenceByHistHeader: public RDODeletable
	{
	public:
		RDOFUNSequence::RDOFUNSequenceHeader* header;
		int                                   base;

		RDOFUNSequenceByHistHeader( RDOFUNSequence::RDOFUNSequenceHeader* _header, int _base = 123456789 ):
			RDODeletable(),
			header( _header ),
			base( _base )
		{
		}
	};

	RDOFUNSequenceByHist( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header ):
		RDOFUNSequence( _parser, _header->header, _header->base )
	{
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
public:
	std::vector< double > from;
	std::vector< double > to;
	std::vector< double > freq;

	RDOFUNSequenceByHistReal( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, double _from, double _to, double _freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addReal( _from, _to, _freq );
	}
	void addReal( double _from, double _to, double _freq );
	double lastTo() const { return !to.empty() ? to.back() : 0; }
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
public:
	std::vector< rdoRuntime::RDOValue > val;
	std::vector< double >               freq;

	RDOFUNSequenceByHistEnum( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, const std::string& _val, double _freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addEnum( _val, _freq );
	}
	void addEnum( const std::string& _val, double _freq );
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
public:
	class RDOFUNSequenceEnumerativeHeader: public RDODeletable
	{
	public:
		RDOFUNSequence::RDOFUNSequenceHeader* header;
		int base;
		RDOFUNSequenceEnumerativeHeader( RDOFUNSequence::RDOFUNSequenceHeader* _header, int _base = 123456789 ):
			header( _header ),
			base( _base )
		{
		}
	};

	RDOFUNSequenceEnumerative( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header ):
		RDOFUNSequence( _parser, _header->header, _header->base )
	{
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeInt
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerativeInt: public RDOFUNSequenceEnumerative
{
public:
	std::vector<int> val;

	RDOFUNSequenceEnumerativeInt( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header, int _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addInt(_val);
	}
	void addInt( int val );
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeReal
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerativeReal: public RDOFUNSequenceEnumerative
{
public:
	std::vector< double > val;

	RDOFUNSequenceEnumerativeReal( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header, double* _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addReal(_val);
	}
	void addReal( double* _val );
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeEnum
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerativeEnum: public RDOFUNSequenceEnumerative
{
public:
	std::vector< rdoRuntime::RDOValue > val;

	RDOFUNSequenceEnumerativeEnum( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header, const std::string& _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addEnum( _val );
	}
	void addEnum( const std::string& _val );
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
class RDOFUNConstant: public RDOParserObject
{
private:
	RDORTPParam* param;

public:
	int number;
	RDOFUNConstant( RDOParser* _parser, RDORTPParam* _param );
	const std::string&           getName() const  { return param->getName(); }
	const RDORTPParamType* const getType() const  { return param->getType(); }
	const RDORTPParam* const     getDescr() const { return param;            }
};

} // namespace rdoParse 

#endif //RDOFUN_FUN
