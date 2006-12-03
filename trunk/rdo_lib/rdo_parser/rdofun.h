#ifndef RDOFUN_FUN
#define RDOFUN_FUN

#include "rdoStdFuncs.h"
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

class RDOFUNFunctionListElement: public RDODeletable, public RDOErrorPos
{
public:
	RDOFUNFunctionListElement(): RDODeletable() {}
	virtual ~RDOFUNFunctionListElement() {}
	virtual rdoRuntime::RDOCalcConst*   createResultCalc( const RDORTPResParam* const retType ) const = 0;
	virtual rdoRuntime::RDOCalcIsEqual* createIsEqualCalc( const RDOFUNFunctionParam* const param, const rdoRuntime::RDOCalcFuncParam* const funcParam ) const;
	virtual bool isEquivalence() const { return false; }
};

class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
public:
	std::string *value;
	RDOFUNFunctionListElementIdentif(std::string *_value) : value(_value) {}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

class RDOFUNFunctionListElementReal: public RDOFUNFunctionListElement
{
public:
	double *value;
	RDOFUNFunctionListElementReal(double *_value) : value(_value) {}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

class RDOFUNFunctionListElementInt: public RDOFUNFunctionListElement
{
public:
	int value;
	RDOFUNFunctionListElementInt(int _value) : value(_value) {}
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

class RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
public:
	bool isEquivalence() const { return true; }
	rdoRuntime::RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

class RDOFUNLogic: public RDODeletable, public RDOErrorPos
{
public:
	rdoRuntime::RDOCalc* calc;

	RDOFUNLogic( rdoRuntime::RDOCalc *_calc ): calc(_calc) {}
	RDOFUNLogic* operator &&( const RDOFUNLogic& second );
	RDOFUNLogic* operator ||( const RDOFUNLogic& second );
	RDOFUNLogic* operator_not();
};

class RDOFUNArithm: public RDODeletable, public RDOErrorPos
{
public:
	RDORTPResParam::ParamType type; // 0 - int, 1 - real, 2 - enum, 3 - string
	RDORTPEnum*  enu; // for type == 2
	std::string* str; // for type == 3

private:
	rdoRuntime::RDOCalc* calc;

public:
	RDOFUNArithm( RDORTPResParam::ParamType _type, rdoRuntime::RDOCalc* _calc ):
		type( _type ),
		enu( NULL ),
		str( NULL ),
		calc( _calc )
	{
	}

	rdoRuntime::RDOCalc* createCalc( const RDORTPResParam* const forType = NULL );
	RDORTPResParam::ParamType getType() const { return type; }

	RDOFUNArithm( std::string* resName, std::string* parName );
	RDOFUNArithm( int n );
	RDOFUNArithm( double d );
	RDOFUNArithm( double* d );
	RDOFUNArithm( std::string* s );

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
};

class RDOFUNCalculateIf: public RDODeletable
{
public:
	RDOFUNLogic *condition;
	std::string *funName;
	RDOFUNArithm *action;

	RDOFUNCalculateIf(RDOFUNLogic *_condition, std::string *_funName, RDOFUNArithm *_action);
};

class RDOParser;
class RDOFUNFunction: public RDODeletable
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
	RDOFUNFunction( const std::string* const _name, const RDORTPResParam* const _retType);
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

class RDOFUNParams: public RDODeletable, public RDOErrorPos
{
public:
	std::vector<RDOFUNArithm *> params;
	RDOFUNParams *addParameter(RDOFUNArithm *param){
		params.push_back(param);
		return this;
	}
	const RDOFUNArithm *createCall(const std::string *const funName) const;
	const RDOFUNArithm *createSeqCall(const std::string *const seqName) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
class RDOFUNGroup: public RDODeletable
{
public:
	const RDORTPResType* resType;

	RDOFUNGroup( const std::string* const _resTypeName );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
class RDOFUNGroupLogic: public RDOFUNGroup
{
public:
	const int funType;

	RDOFUNGroupLogic( int _funType, const std::string* const _resTypeName ):
		RDOFUNGroup( _resTypeName ),
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
	RDOFUNSelect( const std::string* const _resTypeName ): RDOFUNGroup( _resTypeName ), select( NULL ) {}
	RDOFUNLogic* createFunSelect( RDOFUNLogic* cond = NULL );
	RDOFUNLogic* createFunSelect( int funType, RDOFUNLogic* cond );
	RDOFUNLogic* createFunSelectEmpty();
	RDOFUNArithm* createFunSelectSize();
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

class RDOFUNSequence: public RDODeletable
{
public:
	RDOFUNSequenceHeader* header;
	int base;
	rdoRuntime::RDOCalcSeqInit* initSeq;
	rdoRuntime::RDOCalcSeqNext* next;

protected:
	RDOFUNSequence( RDOFUNSequenceHeader* _header, int _base );
	void initResult();

public:
	virtual void createCalcs() = 0;
	virtual const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const = 0;
	const std::string *getName() const { return header->name; }
};

class RDOFUNSequenceUniform: public RDOFUNSequence
{
public:
	RDOFUNSequenceUniform(RDOFUNSequenceHeader *_header, int _base = 123456789);
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

class RDOFUNSequenceExponential: public RDOFUNSequence
{
public:
	RDOFUNSequenceExponential(RDOFUNSequenceHeader *_header, int _base =123456789);
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

class RDOFUNSequenceNormal: public RDOFUNSequence
{
public:
	RDOFUNSequenceNormal( RDOFUNSequenceHeader* _header, int _base = 123456789 );
	void createCalcs();
	const RDOFUNArithm* createCallCalc( const RDOFUNParams* const params ) const;
};

class RDOFUNSequenceByHistHeader: public RDODeletable
{
public:
	RDOFUNSequenceHeader* header;
	int                   base;

	RDOFUNSequenceByHistHeader( RDOFUNSequenceHeader* _header, int _base = 123456789 ):
		header( _header ),
		base( _base )
	{
	}
};

class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	RDOFUNSequenceByHist( RDOFUNSequenceByHistHeader* _header ):
		RDOFUNSequence( _header->header, _header->base )
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

	RDOFUNSequenceByHistReal( RDOFUNSequenceByHistHeader* _header, double _from, double _to, double _freq ):
		RDOFUNSequenceByHist( _header )
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
	std::vector< rdoRuntime::RDOValue> val;
	std::vector< double >              freq;

	RDOFUNSequenceByHistEnum( RDOFUNSequenceByHistHeader* _header, std::string* _val, double _freq ):
		RDOFUNSequenceByHist( _header )
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
	RDOFUNSequenceEnumerativeHeader(RDOFUNSequenceHeader *_header, int _base = 123456789):
		header(_header), base(_base) {}
};

class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
public:
	RDOFUNSequenceEnumerative(RDOFUNSequenceEnumerativeHeader *_header):
		RDOFUNSequence(_header->header, _header->base) {}
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

class RDOFUNSequenceEnumerativeInt: public RDOFUNSequenceEnumerative
{
public:
	std::vector<int> val;

	RDOFUNSequenceEnumerativeInt(RDOFUNSequenceEnumerativeHeader *_header, int _val):
		RDOFUNSequenceEnumerative(_header) {
		addInt(_val);
	}
	void addInt(int val);
	void createCalcs();
};

class RDOFUNSequenceEnumerativeReal: public RDOFUNSequenceEnumerative
{
public:
	std::vector<double> val;

	RDOFUNSequenceEnumerativeReal(RDOFUNSequenceEnumerativeHeader *_header, double * _val):
		RDOFUNSequenceEnumerative(_header) {
		addReal(_val);
	}
	void addReal(double *_val);
	void createCalcs();
};

class RDOFUNSequenceEnumerativeEnum: public RDOFUNSequenceEnumerative
{
public:
	std::vector< rdoRuntime::RDOValue > val;

	RDOFUNSequenceEnumerativeEnum(RDOFUNSequenceEnumerativeHeader *_header, std::string *_val):
		RDOFUNSequenceEnumerative(_header) {
		addEnum(_val);
	}
	void addEnum(std::string *_val);
	void createCalcs();
};

class RDOFUNConstant: public RDODeletable
{
public:
	RDORTPParamDesc *descr;
	int number;
	RDOFUNConstant( RDORTPParamDesc* _descr );
	const std::string *const getName() const { return descr->getName(); };
};

}		// namespace rdoParse 

#endif //RDOFUN_FUN
