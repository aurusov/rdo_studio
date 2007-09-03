#ifndef RDORTP_RTP
#define RDORTP_RTP

#include "rdoparser_object.h"
#include <rdoruntime_object.h>

namespace rdoParse 
{

int rtpparse( void* lexer );
int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void rtperror( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
class RDOFUNArithm;
class RDORTPDefVal;

class RDORTPParamType: public RDOParserObject, public RDOParserSrcInfo
{
public:
	enum ParamType { pt_int = 0, pt_real = 1, pt_enum = 2, pt_unknow = 3 };
	RDORTPDefVal* dv;
	// Для глобальный типов, напрмиер, для параметров стандартных фыункций
	RDORTPParamType( RDOParser* _parser, RDORTPDefVal* _dv ):
		RDOParserObject( _parser ),
		dv( _dv )
	{
	}
	// Для нормальных параметров
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo(),
		dv( _dv )
	{
	}
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info ),
		dv( _dv )
	{
	}
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual int getDiapTableFunc() const = 0;
	virtual ParamType getType() const = 0;
	virtual int writeModelStructure() const = 0;

	void checkParamType( const RDOFUNArithm* const action ) const;
	void checkParamType( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& value_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
// Параметр ресурса или константа, т.е. с родителем надо быть поосторожнее,
// т.к. он или RDORTPResType или NULL (для константы)
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserObject, public RDOParserSrcInfo
{
private:
	std::string name;
	const RDORTPParamType* const parType;
	const RDORTPResType*   const resType;

public:
	RDORTPParam( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info );
	RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType );
	RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info );
	const std::string&           getName() const    { return name;    }
	const RDORTPParamType* const getType() const    { return parType; }
	const RDORTPResType* const   getResType() const { return resType; }
	int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
class RDORTPResType: public RDOParserObject, public RDOParserSrcInfo
{
protected:
	std::string                       name;
	const int                         number;
	const bool                        permanent;
	std::vector< const RDORTPParam* > params;

public:
	RDORTPResType( RDOParser* _parser, const std::string& _name, const bool _permanent );
	const std::string& getName() const       { return name;       };
	int getNumber() const                    { return number;     };
	bool isPermanent() const                 { return permanent;  };
	bool isTemporary() const                 { return !permanent; };

	void addParam( const RDORTPParam* const param );
	const RDORTPParam* findRTPParam( const std::string& param ) const;
	int getRTPParamNumber( const std::string& param ) const;
	const std::vector< const RDORTPParam* >& getParams() const { return params; }

	int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - значение по-умолчанию, базовый класс
// ----------------------------------------------------------------------------
// Сначала цепляется к парсеру, а потом идет reparent на тип ресурса
// ----------------------------------------------------------------------------
class RDORTPDefVal: public RDOParserObject, public RDOParserSrcInfo
{
private:
	bool exist;

public:
	RDORTPDefVal( RDOParser* _parser, const RDORTPDefVal& dv );
	RDORTPDefVal( RDOParser* _parser, bool _exist );
	RDORTPDefVal( RDOParser* _parser, bool _exist, const RDOParserSrcInfo& _src_info );
	virtual int getIntValue() const;
	virtual double getRealValue() const;
	virtual const std::string& getEnumValue() const;
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDefVal - значение по-умолчанию, например, = 20
// ----------------------------------------------------------------------------
class RDORTPIntDefVal: public RDORTPDefVal
{
private:
	int val;

public:
	RDORTPIntDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false )
	{
	}
	RDORTPIntDefVal( RDOParser* _parser, const RDORTPIntDefVal& dv ):
		RDORTPDefVal( _parser, dv ),
		val( dv.val )
	{
	}
	RDORTPIntDefVal( RDOParser* _parser, int _val ):
		RDORTPDefVal( _parser, true ),
		val( _val )
	{
		setSrcText( rdo::format("%d", val) );
	}
	RDORTPIntDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info )
	{
	}
	RDORTPIntDefVal( RDOParser* _parser, int _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		val( _val )
	{
		setSrcText( rdo::format("%d", val) );
	}
	virtual int getIntValue() const { return val; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - даипазон, например, [1 .. 4]
// ----------------------------------------------------------------------------
// Сначала цепляется к парсеру, а потом идет reparent на тип ресурса
// ----------------------------------------------------------------------------
class RDORTPIntDiap: public RDOParserObject, public RDOParserSrcInfo
{
private:
	bool exist;

public:
	int min_value;
	int max_value;
	RDORTPIntDiap( RDOParser* _parser );
	RDORTPIntDiap( RDOParser* _parser, const RDORTPIntDiap& diap );
	RDORTPIntDiap( RDOParser* _parser, const RDOParserSrcInfo& _src_info );
	RDORTPIntDiap( RDOParser* _parser, int _min_value, int _max_value, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos );
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
class RDORTPIntParamType: public RDORTPParamType
{
private:
	void init();

public:
	RDORTPIntDiap* diap;
	RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;					// the function also check range if exist
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual ParamType getType() const { return pt_int; }
	virtual int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDefVal - значение по-умолчанию, например, = 20.2
// ----------------------------------------------------------------------------
class RDORTPRealDefVal: public RDORTPDefVal
{
private:
	double val;

public:
	RDORTPRealDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false )
	{
	}
	RDORTPRealDefVal( RDOParser* _parser, const RDORTPIntDefVal& dv ):
		RDORTPDefVal( _parser, dv ),
		val( dv.getRealValue() )
	{
	}
	RDORTPRealDefVal( RDOParser* _parser, double _val ):
		RDORTPDefVal( _parser, true ),
		val( _val )
	{
		setSrcText( rdo::format("%f", val) );
	}
	RDORTPRealDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info )
	{
	}
	RDORTPRealDefVal( RDOParser* _parser, double _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		val( _val )
	{
		setSrcText( rdo::format("%f", val) );
	}
	virtual double getRealValue() const { return val; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - даипазон, например, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
// Сначала цепляется к парсеру, а потом идет reparent на тип ресурса
// ----------------------------------------------------------------------------
class RDORTPRealDiap: public RDOParserObject, public RDOParserSrcInfo
{
private:
	bool exist;

public:
	double min_value;
	double max_value;
	RDORTPRealDiap( RDOParser* _parser );
	RDORTPRealDiap( RDOParser* _parser, const RDORTPRealDiap& diap );
	RDORTPRealDiap( RDOParser* _parser, const RDOParserSrcInfo& _src_info );
	RDORTPRealDiap( RDOParser* _parser, double _min_value, double _max_value, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos );
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
class RDORTPRealParamType: public RDORTPParamType
{
private:
	void init();

public:
	RDORTPRealDiap* diap;
	RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;				// this function too
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info )const ; 	// the function also check range if exist
	virtual int getDiapTableFunc() const;
	virtual ParamType getType() const { return pt_real; }
	virtual int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOParserObject, public RDOParserSrcInfo
{
public:
	std::vector< std::string > enumVals;
	RDORTPEnum( const RDOParserObject* _parent, const std::string& first ):
		RDOParserObject( _parent )
	{
		enumVals.push_back( first );
	}
	void add( const std::string& next, const YYLTYPE& _pos );
	int findEnumValueWithThrow( const RDOParserSrcInfo& _src_info, const std::string& val ) const;
	int findEnumValueWithoutThrow( const std::string& val ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumDefVal
// ----------------------------------------------------------------------------
class RDORTPEnumDefVal: public RDORTPDefVal
{
private:
	std::string val;

public:
	RDORTPEnumDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false ),
		val( "" )
	{
	}
	RDORTPEnumDefVal( RDOParser* _parser, const RDORTPEnumDefVal& dv ):
		RDORTPDefVal( _parser, dv ),
		val( dv.val )
	{
		setSrcText( val );
	}
	RDORTPEnumDefVal( RDOParser* _parser, const std::string& _val ):
		RDORTPDefVal( _parser, true ),
		val( _val )
	{
		setSrcText( val );
	}
	RDORTPEnumDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info ),
		val( "" )
	{
	}
	RDORTPEnumDefVal( RDOParser* _parser, const std::string& _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		val( _val )
	{
		setSrcText( val );
	}
	virtual const std::string& getEnumValue() const { return val; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
class RDORTPEnumParamType: public RDORTPParamType
{
private:
	void init_src_info();

public:
	RDORTPEnum* enu;
	std::string enum_name; // Используется в сообщениях об ошибках
	bool        enum_fun;  // Используется в сообщениях об ошибках
	RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPEnumDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual ParamType getType() const { return pt_enum; }
	virtual int writeModelStructure() const;
};

} // namespace rdoParse

#endif // RDORTP_RTP
