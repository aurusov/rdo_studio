#ifndef RDORTP_RTP
#define RDORTP_RTP

#include "rdogramma.h"
#include "rdoStdFuncs.h"

namespace rdoParse 
{

int rtpparse( void* lexer );
int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void rtperror( char* mes );

struct RDORTPIntDefVal;

struct RDORTPIntDiap: public RDODeletable
{
	bool exist;
	int minVal, maxVal;
	RDORTPIntDiap(int _minVal, int _maxVal): minVal(_minVal), maxVal(_maxVal), exist(true) {}
	RDORTPIntDiap(): exist(false) {}
	void check(const RDORTPIntDefVal *dv) const;
};

struct RDORTPDefVal: public RDODeletable
{
	bool exist;
	RDORTPDefVal(bool _exist): exist(_exist) {}
	virtual int GetIntValue() const;
	virtual double GetRealValue() const;
	virtual const std::string *GetEnumValue() const;
};

struct RDORTPIntDefVal: public RDORTPDefVal
{
	int val;
	RDORTPIntDefVal(): RDORTPDefVal(false) {}
	RDORTPIntDefVal(int _val): val(_val), RDORTPDefVal(true) {}
	int GetIntValue() const { return val; }
};

struct RDORTPResParam: public RDODeletable
{
	enum ParamType { pt_int = 0, pt_real = 1, pt_enum = 2 };
	RDORTPDefVal *dv;
	RDORTPResParam(RDORTPDefVal *_dv): dv(_dv) {}
	virtual const RDORTPResParam *constructSuchAs() const;
	virtual const RDORTPResParam *constructSuchAs(const int defVal) const;
	virtual const RDORTPResParam *constructSuchAs(const double *const defVal) const;
	virtual const RDORTPResParam *constructSuchAs(const std::string *const defVal) const;
	virtual RDOValue getRSSDefaultValue() const = 0;
	virtual RDOValue getRSSEnumValue(const std::string *const val) const;
	virtual RDOValue getRSSIntValue(const int val) const;
	virtual RDOValue getRSSRealValue(const double *const val) const;
	virtual int getDiapTableFunc() const = 0;
	virtual ParamType getType() const = 0;
	virtual int writeModelStructure() const = 0;
};

struct RDORTPIntResParam: public RDORTPResParam
{
	RDORTPIntDiap *diap;
	RDORTPIntResParam(RDORTPIntDiap *_diap, RDORTPIntDefVal *_dv);
	const RDORTPResParam *constructSuchAs(const int defVal) const;
	RDOValue getRSSDefaultValue()const ;
	RDOValue getRSSIntValue(const int val)const ;	// the function also check range if exist
	int getDiapTableFunc() const;
	ParamType getType() const { return pt_int; }
	int writeModelStructure() const;
};

struct RDORTPRealDefVal;
struct RDORTPRealDiap: public RDODeletable
{
	bool exist;
	double minVal, maxVal;
	RDORTPRealDiap(double _minVal, double _maxVal): minVal(_minVal), maxVal(_maxVal), exist(true) {}
	RDORTPRealDiap(): exist(false) {}
	void check(const RDORTPRealDefVal *dv) const;
};

struct RDORTPRealDefVal: public RDORTPDefVal
{
	double val;
	RDORTPRealDefVal(): RDORTPDefVal(false) {}
	RDORTPRealDefVal(double _val): val(_val), RDORTPDefVal(true) {}
	double GetRealValue() const { return val; }
};

struct RDORTPRealResParam: public RDORTPResParam
{
	RDORTPRealDiap *diap;
	RDORTPRealResParam(RDORTPRealDiap *_diap, RDORTPRealDefVal *_dv);
	const RDORTPResParam *constructSuchAs(const double *const defVal) const;
	RDOValue getRSSDefaultValue()const ;
	RDOValue getRSSRealValue(const double *const val)const ; 	// the function also check range if exist
	RDOValue getRSSIntValue(const int val) const;					// this function too
	int getDiapTableFunc() const;
	ParamType getType() const { return pt_real; }
	int writeModelStructure() const;
};

struct RDORTPEnum: public RDODeletable
{
	std::vector<const std::string *> enumVals;
	RDORTPEnum(const std::string *const first) { enumVals.push_back(first); }
	void add(const std::string *const next);
	int findValue(const std::string *const val) const;
};

struct RDORTPEnumDefVal: public RDORTPDefVal
{
	const std::string *const value;
	RDORTPEnumDefVal(const std::string *const _value): value(_value), RDORTPDefVal(true) {}
	RDORTPEnumDefVal(): value(NULL), RDORTPDefVal(false) {}
	const std::string *GetEnumValue() const { return value; }
};

struct RDORTPEnumResParam: public RDORTPResParam
{
	RDORTPEnum *enu;
	RDORTPEnumResParam(RDORTPEnum *_enu, RDORTPEnumDefVal *_dv): enu(_enu), RDORTPResParam(_dv) {}
	const RDORTPResParam *constructSuchAs(const std::string *const defVal) const ;
	RDOValue getRSSDefaultValue()const ;
	RDOValue getRSSEnumValue(const std::string *const val)const ;
	int getDiapTableFunc() const;
	ParamType getType() const { return pt_enum; }
	int writeModelStructure() const;
};


class RDORTPParamDesc: public RDODeletable
{
	const std::string *const name;
	const RDORTPResParam *const parType;
public:
	RDORTPParamDesc(const std::string *const _name, const RDORTPResParam *const _parType);
	const std::string *const getName() const { return name; };
	const RDORTPResParam *const getType() const { return parType; };
	int writeModelStructure() const;
};

class RDORTPResType: public RDODeletable
{
	const std::string *const name;
	std::vector<const RDORTPParamDesc *>	params;
	const bool isPermanent;		
	const int type;		
public:
	RDORTPResType(const std::string *const _name, const bool _isPermanent, const int _type):
		name(_name), isPermanent(_isPermanent), type(_type) {}
	void add(const RDORTPParamDesc *const _param);
	const RDORTPParamDesc *findRTPParam(const std::string *const param) const;
	int getRTPParamNumber(const std::string *const param) const;
	const std::string *const getName() const { return name; };
	bool isPerm() const { return isPermanent; };
	int getType() const { return type; };
	const std::vector<const RDORTPParamDesc *>& getParams() const { return params; }
	int writeModelStructure() const;
};



}		// namespace rdoParse 

#endif //RDORTP_RTP
