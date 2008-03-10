#ifndef RDORTP_H
#define RDORTP_H

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
//	enum ParamType { pt_int = 0, pt_real = 1, pt_enum = 2, pt_unknow = 3 };
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
	virtual rdoRuntime::RDOValue::ParamType getType() const = 0;
	virtual int writeModelStructure() const = 0;

	void checkParamType( const RDOFUNArithm* const action, bool warning = true ) const;
	void checkParamType( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& value_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
// Параметр ресурса (RDORTPParam) или константа(RDOFUNConst)
// С методом getResType() надо быть поосторожнее, т.к.
// он или RDORTPResType или NULL для константы
// ----------------------------------------------------------------------------
class RDORTPResType;

class RDORTPParam: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPParam( RDORTPResType* _parent, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType );
	const std::string&           getName() const    { return src_info().src_text(); }
	const RDORTPParamType* const getType() const    { return m_parType; }
	const RDORTPResType* const   getResType() const { return m_resType; }
	int writeModelStructure() const;

protected:
	RDORTPParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType );

	const RDORTPParamType* const m_parType;
	const RDORTPResType*   const m_resType;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNConst
// ----------------------------------------------------------------------------
class RDOFUNConst: public RDORTPParam
{
public:
	RDOFUNConst( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType );
};

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
class RDORTPResType: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPResType( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const bool _permanent );
	const std::string& getName() const       { return src_text();   };
	int getNumber() const                    { return m_number;     };
	bool isPermanent() const                 { return m_permanent;  };
	bool isTemporary() const                 { return !m_permanent; };

	void addParam( const RDORTPParam* const param );
	void addParam( const std::string param_name, rdoRuntime::RDOValue::ParamType param_type );
	const RDORTPParam* findRTPParam( const std::string& param ) const;
	int getRTPParamNumber( const std::string& param ) const;
	const std::vector< const RDORTPParam* >& getParams() const { return m_params; }

	int writeModelStructure() const;

private:
	const int                         m_number;
	const bool                        m_permanent;
	std::vector< const RDORTPParam* > m_params;
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
	RDORTPDefVal( RDOParser* _parser, const RDORTPDefVal& copy );
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
	RDORTPIntDefVal( RDOParser* _parser, const RDORTPIntDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		val( copy.val )
	{
	}
	RDORTPIntDefVal( const RDORTPIntDefVal& copy ):
		RDORTPDefVal( copy.getParser(), copy ),
		val( copy.val )
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
// ---------- RDORTPDiap - Шаблон классов диапазонов для integer и real
// ----------------------------------------------------------------------------
// Сначала цепляется к парсеру, а потом идет reparent на тип ресурса
// ----------------------------------------------------------------------------
template<class T>
class RDORTPDiap: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPDiap( RDOParser* _parser ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo(),
		m_exist( false ),
		m_min_value( 0 ),
		m_max_value( 0 )
	{
	}
	RDORTPDiap( RDOParser* _parser, const RDORTPDiap<T>& copy ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( copy.src_info() ),
		m_exist( copy.m_exist ),
		m_min_value( copy.m_min_value ),
		m_max_value( copy.m_max_value )
	{
	}
	RDORTPDiap( const RDORTPDiap<T>& copy ):
		RDOParserObject( copy.getParser() ),
		RDOParserSrcInfo( copy.src_info() ),
		m_exist( copy.m_exist ),
		m_min_value( copy.m_min_value ),
		m_max_value( copy.m_max_value )
	{
	}
	RDORTPDiap( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info ),
		m_exist( false ),
		m_min_value( 0 ),
		m_max_value( 0 )
	{
	}
	RDORTPDiap( RDOParser* _parser, T min_value, T max_value, const RDOParserSrcInfo& _src_info, const YYLTYPE& max_value_pos ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info ),
		m_exist( true ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
		init( &max_value_pos );
	}
	RDORTPDiap( T min_value, T max_value ):
		m_exist( true ),
		m_min_value( min_value ),
		m_max_value( max_value )
	{
		init( NULL );
	}
	bool isExist() const { return m_exist;     }
	T  getMin() const    { return m_min_value; }
	T  getMax() const    { return m_max_value; }

private:
	bool m_exist;
	T    m_min_value;
	T    m_max_value;

	void init( const YYLTYPE* const max_value_pos )
	{
		if ( max_value_pos && m_min_value > m_max_value ) {
			getParser()->error( *max_value_pos, "Левая граница диапазона должна быть меньше правой" );
		}
		setSrcText( rdo::format("[%s..%s]", rdoRuntime::RDOValue(m_min_value).getAsString().c_str(), rdoRuntime::RDOValue(m_max_value).getAsString().c_str()) );
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - integer даипазон, например, [1 .. 4]
// ----------------------------------------------------------------------------
typedef RDORTPDiap<int>    RDORTPIntDiap;
// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - real даипазон, например, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
typedef RDORTPDiap<double> RDORTPRealDiap;

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
	virtual rdoRuntime::RDOValue::ParamType getType() const { return rdoRuntime::RDOValue::pt_int; }
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
	RDORTPRealDefVal( RDOParser* _parser, const RDORTPRealDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		val( copy.val )
	{
	}
	RDORTPRealDefVal( const RDORTPRealDefVal& copy ):
		RDORTPDefVal( copy.getParser(), copy ),
		val( copy.val )
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
	virtual rdoRuntime::RDOValue::ParamType getType() const { return rdoRuntime::RDOValue::pt_real; }
	virtual int writeModelStructure() const;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOParserObject, public RDOParserSrcInfo
{
private:
	rdoRuntime::RDOEnum* enums;

public:
	RDORTPEnum( const RDOParserObject* _parent, const std::string& first );
	virtual ~RDORTPEnum();
	void add( const RDOParserSrcInfo& next );
	rdoRuntime::RDOValue findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const;
	rdoRuntime::RDOValue getFirstValue() const;
	const rdoRuntime::RDOEnum* getEnums() const { return enums; }
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
	RDORTPEnumDefVal( RDOParser* _parser, const RDORTPEnumDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		val( copy.val )
	{
		setSrcText( val );
	}
	RDORTPEnumDefVal( const RDORTPEnumDefVal& copy ):
		RDORTPDefVal( copy.getParser(), copy ),
		val( copy.val )
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
	virtual rdoRuntime::RDOValue::ParamType getType() const { return rdoRuntime::RDOValue::pt_enum; }
	virtual int writeModelStructure() const;
};

} // namespace rdoParse

#endif // RDORTP_H
