#ifndef RDORTP_RTP
#define RDORTP_RTP

#include "rdogramma.h"
#include "rdoStdFuncs.h"

namespace rdoParse 
{

int rtpparse( void* lexer );
int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void rtperror( char* mes );

class RDOErrorPos
{
public:
	int error_first_line;
	int error_first_pos;
	int error_last_line;
	int error_last_pos;
	RDOErrorPos(): 
		error_first_line( -1 ),
		error_first_pos( -1 ),
		error_last_line( -1 ),
		error_last_pos( -1 )
	{
	}
	void setErrorPos( const YYLTYPE& error_pos ) {
		error_first_line = error_pos.first_line;
		error_first_pos  = error_pos.first_column;
		error_last_line  = error_pos.last_line;
		error_last_pos   = error_pos.last_column;
	}
};

class RDORTPIntDefVal;

class RDORTPIntDiap: public RDODeletable
{
public:
	bool exist;
	int  minVal, maxVal;
	RDORTPIntDiap( int _minVal, int _maxVal );
	RDORTPIntDiap(): exist( false ) {};
	void check( const RDORTPIntDefVal* dv ) const;
};

class RDORTPDefVal: public RDODeletable
{
public:
	bool exist;
	RDORTPDefVal(bool _exist): exist(_exist) {}
	virtual int GetIntValue() const;
	virtual double GetRealValue() const;
	virtual const std::string *GetEnumValue() const;
};

class RDORTPIntDefVal: public RDORTPDefVal
{
public:
	int val;
	RDORTPIntDefVal(): RDORTPDefVal(false) {}
	RDORTPIntDefVal(int _val): val(_val), RDORTPDefVal(true) {}
	int GetIntValue() const { return val; }
};

class RDORTPResParam: public RDODeletable
{
public:
	enum ParamType { pt_int = 0, pt_real = 1, pt_enum = 2, pt_str = 3 };
	RDORTPDefVal *dv;
	RDORTPResParam(RDORTPDefVal *_dv): dv(_dv) {}
	virtual const RDORTPResParam *constructSuchAs() const;
	virtual const RDORTPResParam *constructSuchAs(const int defVal) const;
	virtual const RDORTPResParam *constructSuchAs(const double *const defVal) const;
	virtual const RDORTPResParam *constructSuchAs(const std::string *const defVal) const;
	virtual rdoRuntime::RDOValue getRSSDefaultValue() const = 0;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string* const val ) const = 0;
	virtual rdoRuntime::RDOValue getRSSIntValue( const int val ) const = 0;
	virtual rdoRuntime::RDOValue getRSSRealValue( const double* const val ) const = 0;
	virtual int getDiapTableFunc() const = 0;
	virtual ParamType getType() const = 0;
	virtual int writeModelStructure() const = 0;
};

class RDORTPIntResParam: public RDORTPResParam
{
public:
	RDORTPIntDiap *diap;
	RDORTPIntResParam();
	RDORTPIntResParam(RDORTPIntDiap *_diap, RDORTPIntDefVal *_dv);
	const RDORTPResParam *constructSuchAs(const int defVal) const;
	virtual rdoRuntime::RDOValue getRSSDefaultValue()const ;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string* const val ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue(const int val)const ;					// the function also check range if exist
	virtual rdoRuntime::RDOValue getRSSRealValue( const double* const val ) const;
	int getDiapTableFunc() const;
	ParamType getType() const { return pt_int; }
	int writeModelStructure() const;
};

class RDORTPRealDefVal;
class RDORTPRealDiap: public RDODeletable
{
public:
	bool   exist;
	double minVal, maxVal;
	RDORTPRealDiap( double _minVal, double _maxVal );
	RDORTPRealDiap(): exist( false ) {}
	void check( const RDORTPRealDefVal* dv ) const;
};

class RDORTPRealDefVal: public RDORTPDefVal
{
public:
	double val;
	RDORTPRealDefVal(): RDORTPDefVal(false) {}
	RDORTPRealDefVal(double _val): val(_val), RDORTPDefVal(true) {}
	double GetRealValue() const { return val; }
};

class RDORTPRealResParam: public RDORTPResParam
{
public:
	RDORTPRealResParam();
	RDORTPRealResParam( RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv );
	RDORTPRealDiap* diap;
	const RDORTPResParam *constructSuchAs(const double *const defVal) const;
	virtual rdoRuntime::RDOValue getRSSDefaultValue()const ;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string* const val ) const;
	virtual rdoRuntime::RDOValue getRSSRealValue(const double *const val)const ; 	// the function also check range if exist
	virtual rdoRuntime::RDOValue getRSSIntValue(const int val) const;				// this function too
	int getDiapTableFunc() const;
	ParamType getType() const { return pt_real; }
	int writeModelStructure() const;
};

class RDORTPEnum: public RDODeletable
{
public:
	std::vector<const std::string *> enumVals;
	RDORTPEnum(const std::string *const first) { enumVals.push_back(first); }
	void add(const std::string *const next);
	int findValue(const std::string *const val) const;
};

class RDORTPEnumDefVal: public RDORTPDefVal
{
public:
	const std::string *const value;
	RDORTPEnumDefVal(const std::string *const _value): value(_value), RDORTPDefVal(true) {}
	RDORTPEnumDefVal(): value(NULL), RDORTPDefVal(false) {}
	const std::string *GetEnumValue() const { return value; }
};

class RDORTPEnumResParam: public RDORTPResParam
{
public:
	RDORTPEnum *enu;
	RDORTPEnumResParam(RDORTPEnum *_enu, RDORTPEnumDefVal *_dv): enu(_enu), RDORTPResParam(_dv) {}
	const RDORTPResParam *constructSuchAs(const std::string *const defVal) const ;
	virtual rdoRuntime::RDOValue getRSSDefaultValue()const ;
	virtual rdoRuntime::RDOValue getRSSEnumValue(const std::string* const val) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( const int val ) const;
	virtual rdoRuntime::RDOValue getRSSRealValue( const double* const val ) const;
	int getDiapTableFunc() const;
	ParamType getType() const { return pt_enum; }
	int writeModelStructure() const;
};

class RDORTPParamDesc: public RDODeletable
{
	const std::string *const name;
	const RDORTPResParam *const parType;
public:
	RDORTPParamDesc( const std::string* const _name, const RDORTPResParam* const _parType);
	const std::string *const getName() const { return name; };
	const RDORTPResParam *const getType() const { return parType; };
	int writeModelStructure() const;
};

class RDORTPResType: public RDODeletable
{
protected:
	const std::string* const               name;
	const int                              number;
	const bool                             permanent;
	std::vector< const RDORTPParamDesc * > params;

public:
	RDORTPResType( const std::string* const _name, const bool _permanent );
	const std::string* const getName() const { return name;       };
	int getNumber() const                    { return number;     };
	bool isPermanent() const                 { return permanent;  };
	bool isTemporary() const                 { return !permanent; };

	void addParam( const RDORTPParamDesc* const param );
	const RDORTPParamDesc* findRTPParam( const std::string* const param ) const;
	int getRTPParamNumber( const std::string* const param ) const;
	const std::vector< const RDORTPParamDesc* >& getParams() const { return params; }

	int writeModelStructure() const;
};

}		// namespace rdoParse 

#endif //RDORTP_RTP
