#ifndef RDOFUN_FUN
#define RDOFUN_FUN

#include "rdoparser_object.h"
#include "rdortp.h"

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

class RDORTPResParam;
class RDORTPEnum;
class RDORTPResType;
class RDORTPParamDesc;

class RDOFUNFunctionParam: public RDODeletable, public RDOErrorPos
{
private:
	std::string*    name;
	RDORTPResParam* type;

public:
	RDOFUNFunctionParam( std::string* _name, RDORTPResParam* _type ):
		name( _name ),
		type( _type )
	{
	}
	const std::string* const getName() const { return name; };
	const RDORTPResParam* const getType() const { return type; };
};

class RDOFUNFunctionListElement: public RDOParserObject, public RDOErrorPos
{
public:
	RDOFUNFunctionListElement( const RDOParserObject* _parent ): RDOParserObject( _parent ) {}
	virtual ~RDOFUNFunctionListElement() {}
	virtual rdoRuntime::RDOCalcConst*   createResultCalc( const RDORTPResParam* const retType ) const = 0;
	virtual rdoRuntime::RDOCalcIsEqual* createIsEqualCalc( const RDOFUNFunctionParam* const param, const rdoRuntime::RDOCalcFuncParam* const funcParam ) const;
	virtual bool isEquivalence() const { return false; }
};

class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
public:
	std::string* value;
	RDOFUNFunctionListElementIdentif( const RDOParserObject* _parent, std::string* _value ):
		RDOFUNFunctionListElement( _parent ),
		value( _value )
	{
	}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
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
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
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
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

class RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
public:
	RDOFUNFunctionListElementEq( const RDOParserObject* _parent ): RDOFUNFunctionListElement( _parent ) {}
	bool isEquivalence() const { return true; }
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
class RDOFUNLogic: public RDODeletable, public RDOErrorPos
{
public:
	rdoRuntime::RDOCalc* calc;

	RDOFUNLogic( rdoRuntime::RDOCalc *_calc );

	RDOFUNLogic* operator &&( const RDOFUNLogic& second );
	RDOFUNLogic* operator ||( const RDOFUNLogic& second );
	RDOFUNLogic* operator_not();

	void setErrorPos( const YYLTYPE& error_pos );
	void setErrorPos( int first_line, int first_column, int last_line, int last_column );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
class RDOFUNArithm: public RDOParserObject, public RDOErrorPos
{
public:
	RDORTPResParam::ParamType type; // 0 - int, 1 - real, 2 - enum, 3 - string
	RDORTPEnum*  enu; // for type == 2
	std::string* str; // for type == 3

private:
	rdoRuntime::RDOCalc* calc;

	void init( std::string* resName, std::string* parName, const YYLTYPE& res_name_error_pos, const YYLTYPE& par_name_error_pos );
	void init( std::string* s, const YYLTYPE& error_pos );

public:
	RDOFUNArithm( RDOParser* _parser, RDORTPResParam::ParamType _type, rdoRuntime::RDOCalc* _calc, const YYLTYPE& error_pos );
	RDOFUNArithm( RDOParser* _parser, std::string* resName, std::string* parName, const YYLTYPE& res_name_error_pos, const YYLTYPE& par_name_error_pos );
	RDOFUNArithm( RDOParser* _parser, int n, const YYLTYPE& error_pos );
	RDOFUNArithm( RDOParser* _parser, double* d, const YYLTYPE& error_pos );
	RDOFUNArithm( RDOParser* _parser, std::string* s, const YYLTYPE& error_pos );

	RDOFUNArithm( const RDOParserObject* _parent, RDORTPResParam::ParamType _type, rdoRuntime::RDOCalc* _calc, const YYLTYPE& error_pos );
	RDOFUNArithm( const RDOParserObject* _parent, std::string* resName, std::string* parName, const YYLTYPE& res_name_error_pos, const YYLTYPE& par_name_error_pos );
	RDOFUNArithm( const RDOParserObject* _parent, int n, const YYLTYPE& error_pos );
	RDOFUNArithm( const RDOParserObject* _parent, double* d, const YYLTYPE& error_pos );
	RDOFUNArithm( const RDOParserObject* _parent, std::string* s, const YYLTYPE& error_pos );

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

	rdoRuntime::RDOCalc* createCalc( const RDORTPResParam* const forType = NULL );
	RDORTPResParam::ParamType getType() const { return type; }

	void setErrorPos( const YYLTYPE& error_pos );
	void setErrorPos( int first_line, int first_column, int last_line, int last_column );
};

class RDOFUNCalculateIf: public RDOParserObject
{
public:
	RDOFUNLogic*  condition;
	std::string*  funName;
	RDOFUNArithm* action;

	RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, std::string* _funName, RDOFUNArithm* _action );
};

class RDOParser;
class RDOFUNFunction: public RDOParserObject
{
friend RDOParser;
private:
	const std::string *const name;
	const RDORTPResParam *const retType;
	std::vector<const RDOFUNFunctionParam *>	params;
	std::vector<const RDOFUNFunctionListElement *>	listElems;	// for list and table
	std::vector<const RDOFUNCalculateIf *>	calculateIf;	// for algorithmic
	rdoRuntime::RDOFunCalc *functionCalc;

public:
	RDOFUNFunction( RDOParser* _parser, const std::string* const _name, const RDORTPResParam* const _retType );
	void add(const RDOFUNFunctionParam *const _param);
	void add(const RDOFUNFunctionListElement *const _listElement);
	void add(const RDOFUNCalculateIf *const _calculateIf);
	const RDOFUNFunctionParam *const findFUNFunctionParam(const std::string *const paramName) const;
	int findFUNFunctionParamNum(const std::string *const paramName) const;
	void createListCalc();
	void createTableCalc();
	void createAlgorithmicCalc();
	const std::string *const getName() const { return name; }
	const std::vector<const RDOFUNFunctionParam *> getParams() const { return params; }
	const rdoRuntime::RDOFunCalc *getFunctionCalc() const { return functionCalc; }
	const RDORTPResParam *const getType() const { return retType; }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
class RDOFUNParams: public RDOParserObject, public RDOErrorPos
{
public:
	RDOFUNParams( RDOParser* _parser )            : RDOParserObject( _parser ) {}
	RDOFUNParams( const RDOParserObject* _parent ): RDOParserObject( _parent ) {}
	RDOErrorPos name_error_pos;
	std::vector< RDOFUNArithm* > params;
	RDOFUNParams* addParameter( RDOFUNArithm* param ) {
		params.push_back( param );
		return this;
	}
	const RDOFUNArithm* createCall( const std::string* const funName ) const;
	const RDOFUNArithm* createSeqCall( const std::string* const seqName ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
class RDOFUNGroup: public RDOParserObject, public RDOErrorPos
{
protected:
	void init( const std::string* const _resType );

public:
	const RDORTPResType* resType;

	RDOFUNGroup( RDOParser* _parser, const std::string* const _resTypeName );
	RDOFUNGroup( const RDOParserObject* _parent, const std::string* const _resTypeName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
class RDOFUNGroupLogic: public RDOFUNGroup
{
public:
	const int funType;

	RDOFUNGroupLogic( RDOParser* _parser, int _funType, const std::string* const _resTypeName ):
		RDOFUNGroup( _parser, _resTypeName ),
		funType( _funType )
	{
	}
	RDOFUNGroupLogic( const RDOParserObject* _parent, int _funType, const std::string* const _resTypeName ):
		RDOFUNGroup( _parent, _resTypeName ),
		funType( _funType )
	{
	}
	RDOFUNLogic* createFunLogic();
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
	RDOFUNSelect( RDOParser* _parser, const std::string* const _resTypeName ):
		RDOFUNGroup( _parser, _resTypeName ),
		select( NULL )
	{
	}
	RDOFUNSelect( const RDOParserObject* _parent, const std::string* const _resTypeName ):
		RDOFUNGroup( _parent, _resTypeName ),
		select( NULL )
	{
	}
	RDOFUNLogic* createFunSelect( RDOFUNLogic* cond = NULL );
	RDOFUNLogic* createFunSelectGroup( int funType, RDOFUNLogic* cond );
	RDOFUNLogic* createFunSelectEmpty();
	RDOFUNArithm* createFunSelectSize();

	void setErrorPos( const YYLTYPE& error_pos );
	void setErrorPos( int first_line, int first_column, int last_line, int last_column );
};

// ----------------------------------------------------------------------------
// ---------- Sequences
// ----------------------------------------------------------------------------
class RDOFUNSequenceHeader: public RDODeletable
{
public:
	std::string*    name;
	RDORTPResParam* type;
	RDOFUNSequenceHeader( std::string* _name, RDORTPResParam* _type ):
		name(_name),
		type(_type)
	{
	}
};

class RDOFUNSequence: public RDOParserObject
{
public:
	RDOFUNSequenceHeader* header;
	int base;
	rdoRuntime::RDOCalcSeqInit* initSeq;
	rdoRuntime::RDOCalcSeqNext* next;

protected:
	RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base );
	void initResult();

public:
	virtual void createCalcs() = 0;
	virtual const RDOFUNArithm* createCallCalc( const RDOFUNParams* const params ) const = 0;
	const std::string* getName() const { return header->name; }
};

class RDOFUNSequenceUniform: public RDOFUNSequence
{
public:
	RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

class RDOFUNSequenceExponential: public RDOFUNSequence
{
public:
	RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base =123456789 );
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

class RDOFUNSequenceNormal: public RDOFUNSequence
{
public:
	RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	void createCalcs();
	const RDOFUNArithm* createCallCalc( const RDOFUNParams* const params ) const;
};

class RDOFUNSequenceByHistHeader: public RDODeletable
{
public:
	RDOFUNSequenceHeader* header;
	int                   base;

	RDOFUNSequenceByHistHeader( RDOFUNSequenceHeader* _header, int _base = 123456789 ):
		RDODeletable(),
		header( _header ),
		base( _base )
	{
	}
};

class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	RDOFUNSequenceByHist( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header ):
		RDOFUNSequence( _parser, _header->header, _header->base )
	{
	}
	const RDOFUNArithm* createCallCalc(const RDOFUNParams* const params) const;
};

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
	void createCalcs();
};

class RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
public:
	std::vector< rdoRuntime::RDOValue > val;
	std::vector< double >               freq;

	RDOFUNSequenceByHistEnum( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, std::string* _val, double _freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addEnum( _val, _freq );
	}
	void addEnum( std::string* _val, double _freq );
	void createCalcs();
};

class RDOFUNSequenceEnumerativeHeader: public RDODeletable
{
public:
	RDOFUNSequenceHeader *header;
	int base;
	RDOFUNSequenceEnumerativeHeader( RDOFUNSequenceHeader* _header, int _base = 123456789 ):
		header( _header ),
		base( _base )
	{
	}
};

class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
public:
	RDOFUNSequenceEnumerative( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header ):
		RDOFUNSequence( _parser, _header->header, _header->base )
	{
	}
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

class RDOFUNSequenceEnumerativeInt: public RDOFUNSequenceEnumerative
{
public:
	std::vector<int> val;

	RDOFUNSequenceEnumerativeInt( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header, int _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addInt(_val);
	}
	void addInt(int val);
	void createCalcs();
};

class RDOFUNSequenceEnumerativeReal: public RDOFUNSequenceEnumerative
{
public:
	std::vector< double > val;

	RDOFUNSequenceEnumerativeReal( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header, double* _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addReal(_val);
	}
	void addReal(double *_val);
	void createCalcs();
};

class RDOFUNSequenceEnumerativeEnum: public RDOFUNSequenceEnumerative
{
public:
	std::vector< rdoRuntime::RDOValue > val;

	RDOFUNSequenceEnumerativeEnum( RDOParser* _parser, RDOFUNSequenceEnumerativeHeader* _header, std::string* _val ):
		RDOFUNSequenceEnumerative( _parser, _header )
	{
		addEnum(_val);
	}
	void addEnum(std::string *_val);
	void createCalcs();
};

class RDOFUNConstant: public RDOParserObject
{
public:
	RDORTPParamDesc* descr;
	int number;
	RDOFUNConstant( RDOParser* _parser, RDORTPParamDesc* _descr );
	const std::string* const getName() const { return descr->getName(); }
};

} // namespace rdoParse 

#endif //RDOFUN_FUN
