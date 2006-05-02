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
}

using namespace rdoRuntime;

namespace rdoParse 
{

int funparse( void* lexer );
int funlex( int* lpval, void* lexer );
void funerror( char* mes );

struct RDORTPResParam;
struct RDORTPEnum;
class RDORTPResType;
class RDORTPParamDesc;

class RDOFUNFunctionParam: public RDODeletable
{
	std::string *name;
	RDORTPResParam *type;

public:
	RDOFUNFunctionParam(std::string *_name, RDORTPResParam *_type):
		name(_name), type(_type) {}
	const std::string *const getName() const { return name; };
	const RDORTPResParam *const getType() const { return type; };
};

struct RDOFUNFunctionListElement: public RDODeletable
{
	virtual ~RDOFUNFunctionListElement() {}
	virtual RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const = 0;
	virtual RDOCalcIsEqual *createIsEqualCalc(const RDOFUNFunctionParam *const param, const RDOCalcFuncParam *const funcParam) const;
	virtual bool isEquivalence() const { return false; }
};

struct RDOFUNFunctionListElementItentif: public RDOFUNFunctionListElement
{
	std::string *value;
	RDOFUNFunctionListElementItentif(std::string *_value) : value(_value) {}
	RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

struct RDOFUNFunctionListElementReal: public RDOFUNFunctionListElement
{
	double *value;
	RDOFUNFunctionListElementReal(double *_value) : value(_value) {}
	RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

struct RDOFUNFunctionListElementInt: public RDOFUNFunctionListElement
{
	int value;
	RDOFUNFunctionListElementInt(int _value) : value(_value) {}
	RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

struct RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
	bool isEquivalence() const { return true; }
	RDOCalcConst *createResultCalc(const RDORTPResParam *const retType) const;
};

struct RDOFUNLogic: public RDODeletable
{
	RDOCalc *calc;

	RDOFUNLogic(RDOCalc *_calc):
		calc(_calc) {}
	RDOFUNLogic *operator &&(RDOFUNLogic &second);
	RDOFUNLogic *operator ||(RDOFUNLogic &second);
};

struct RDOFUNArithm: public RDODeletable
{
	int type;	// 0 - int, 1 - real, 2 - enum, 3 - string
	RDORTPEnum *enu;		// for type == 2
	std::string *str;		// for type == 3

private:
	RDOCalc *calc;

public:
	RDOCalc *createCalc(const RDORTPResParam *const forType = NULL);
	int getType() const { return type; }

	RDOFUNArithm(int _type, RDOCalc *_calc):
		type(_type), calc(_calc) {}

	RDOFUNArithm(std::string *resName, std::string *parName);
	RDOFUNArithm(int n);
	RDOFUNArithm(double *d);
	RDOFUNArithm(std::string *s);

	RDOFUNArithm *operator +(RDOFUNArithm &second);
	RDOFUNArithm *operator -(RDOFUNArithm &second);
	RDOFUNArithm *operator *(RDOFUNArithm &second);
	RDOFUNArithm *operator /(RDOFUNArithm &second);

	RDOFUNLogic *operator ==(RDOFUNArithm &second);
	RDOFUNLogic *operator !=(RDOFUNArithm &second);
	RDOFUNLogic *operator < (RDOFUNArithm &second);
	RDOFUNLogic *operator > (RDOFUNArithm &second);
	RDOFUNLogic *operator <=(RDOFUNArithm &second);
	RDOFUNLogic *operator >=(RDOFUNArithm &second);
};

struct RDOFUNCalculateIf: public RDODeletable
{
	RDOFUNLogic *condition;
	std::string *funName;
	RDOFUNArithm *action;

	RDOFUNCalculateIf(RDOFUNLogic *_condition, std::string *_funName, RDOFUNArithm *_action);
};

class RDOParser;
class RDOFUNFunction: public RDODeletable
{
friend RDOParser;

	const std::string *const name;
	const RDORTPResParam *const retType;
	std::vector<const RDOFUNFunctionParam *>	params;
	std::vector<const RDOFUNFunctionListElement *>	listElems;	// for list and table
	std::vector<const RDOFUNCalculateIf *>	calculateIf;	// for algorithmic
	RDOFunCalc *functionCalc;

public:
	RDOFUNFunction(const std::string *const _name, const RDORTPResParam *const _retType):
		name(_name), retType(_retType) {}

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
	const RDOFunCalc *getFunctionCalc() const { return functionCalc; }
	const RDORTPResParam *const getType() const { return retType; }
};

struct RDOFUNParams: public RDODeletable
{
	std::vector<RDOFUNArithm *> params;
	RDOFUNParams *addParameter(RDOFUNArithm *param){
		params.push_back(param);
		return this;
	}
	const RDOFUNArithm *createCall(const std::string *const funName) const;
	const RDOFUNArithm *createSeqCall(const std::string *const seqName) const;
};

struct RDOFUNGroup: public RDODeletable
{
	const int funType;
	const RDORTPResType *resType;

	RDOFUNGroup(int _funType, const std::string *const _resType);
	RDOFUNLogic *createFunLogin();
	RDOFUNLogic *createFunLogin(RDOFUNLogic *cond);
};

//////////////////////////// Sequences	///////////////////////////////

struct RDOFUNSequenceHeader: public RDODeletable
{
	std::string *name;
	RDORTPResParam *type;
	RDOFUNSequenceHeader(std::string *_name, RDORTPResParam *_type): 
		name(_name), type(_type) {}
};

struct RDOFUNSequence: public RDODeletable
{
	RDOFUNSequenceHeader *header;
	int base;
	RDOCalcSeqInit *initSeq;
	RDOCalcSeqNext *next;
protected:
	RDOFUNSequence(RDOFUNSequenceHeader *_header, int _base);
public:
	virtual void createCalcs() = 0;
	virtual const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const = 0;
	const std::string *getName() const { return header->name; }
};

struct RDOFUNSequenceUniform: public RDOFUNSequence
{
	RDOFUNSequenceUniform(RDOFUNSequenceHeader *_header, int _base = 123456789);
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

struct RDOFUNSequenceExponential: public RDOFUNSequence
{
	RDOFUNSequenceExponential(RDOFUNSequenceHeader *_header, int _base =123456789);
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

struct RDOFUNSequenceNormal: public RDOFUNSequence
{
	RDOFUNSequenceNormal(RDOFUNSequenceHeader *_header, int _base = 123456789);
	void createCalcs();
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};


struct RDOFUNSequenceByHistHeader: public RDODeletable
{
	RDOFUNSequenceHeader *header;
	int base;
	RDOFUNSequenceByHistHeader(RDOFUNSequenceHeader *_header, int _base = 123456789):
		header(_header), base(_base) {}
};

struct RDOFUNSequenceByHist: public RDOFUNSequence
{
	RDOFUNSequenceByHist(RDOFUNSequenceByHistHeader *_header):
		RDOFUNSequence(_header->header, _header->base) {}
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

struct RDOFUNSequenceByHistInt: public RDOFUNSequenceByHist
{
	std::vector<int> from;
	std::vector<int> to;
	std::vector<double> freq;

	RDOFUNSequenceByHistInt(RDOFUNSequenceByHistHeader *_header, int _from, int _to, double *_freq):
		RDOFUNSequenceByHist(_header) {
		addInt(_from, _to, _freq);
	}
	void addInt(int _from, int _to, double *_freq);
	void createCalcs();
};

struct RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
	std::vector<double> from;
	std::vector<double> to;
	std::vector<double> freq;

	RDOFUNSequenceByHistReal(RDOFUNSequenceByHistHeader *_header, double * _from, double * _to, double *_freq):
		RDOFUNSequenceByHist(_header) {
		addReal(_from, _to, _freq);
	}
	void addReal(double * _from, double * _to, double *_freq);
	void createCalcs();
};

struct RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
	std::vector<RDOValue> val;
	std::vector<double> freq;

	RDOFUNSequenceByHistEnum(RDOFUNSequenceByHistHeader *_header, std::string *_val, double *_freq):
		RDOFUNSequenceByHist(_header) {
		addEnum(_val, _freq);
	}
	void addEnum(std::string *_val, double *_freq);
	void createCalcs();
};


struct RDOFUNSequenceEnumerativeHeader: public RDODeletable
{
	RDOFUNSequenceHeader *header;
	int base;
	RDOFUNSequenceEnumerativeHeader(RDOFUNSequenceHeader *_header, int _base = 123456789):
		header(_header), base(_base) {}
};

struct RDOFUNSequenceEnumerative: public RDOFUNSequence
{
	RDOFUNSequenceEnumerative(RDOFUNSequenceEnumerativeHeader *_header):
		RDOFUNSequence(_header->header, _header->base) {}
	const RDOFUNArithm *createCallCalc(const RDOFUNParams *const params) const;
};

struct RDOFUNSequenceEnumerativeInt: public RDOFUNSequenceEnumerative
{
	std::vector<int> val;

	RDOFUNSequenceEnumerativeInt(RDOFUNSequenceEnumerativeHeader *_header, int _val):
		RDOFUNSequenceEnumerative(_header) {
		addInt(_val);
	}
	void addInt(int val);
	void createCalcs();
};

struct RDOFUNSequenceEnumerativeReal: public RDOFUNSequenceEnumerative
{
	std::vector<double> val;

	RDOFUNSequenceEnumerativeReal(RDOFUNSequenceEnumerativeHeader *_header, double * _val):
		RDOFUNSequenceEnumerative(_header) {
		addReal(_val);
	}
	void addReal(double *_val);
	void createCalcs();
};

struct RDOFUNSequenceEnumerativeEnum: public RDOFUNSequenceEnumerative
{
	std::vector<RDOValue> val;

	RDOFUNSequenceEnumerativeEnum(RDOFUNSequenceEnumerativeHeader *_header, std::string *_val):
		RDOFUNSequenceEnumerative(_header) {
		addEnum(_val);
	}
	void addEnum(std::string *_val);
	void createCalcs();
};

struct RDOFUNConstant: public RDODeletable
{
	RDORTPParamDesc *descr;
	int number;
	RDOFUNConstant(RDORTPParamDesc *_descr, int _number)
		: descr(_descr), number(_number) {}
	const std::string *const getName() const { return descr->getName(); };
};

}		// namespace rdoParse 

#endif //RDOFUN_FUN
