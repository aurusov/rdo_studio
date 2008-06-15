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
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const = 0;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getDefaultValue( const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSStringValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const = 0;
	virtual int getDiapTableFunc() const = 0;
	virtual rdoRuntime::RDOValue::Type getType() const = 0;
	virtual void writeModelStructure( std::ostream& stream ) const = 0;

	void checkParamType( const RDOFUNArithm* const action, bool warning = true ) const;
	void checkParamType( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& value_info ) const;

	const RDORTPDefVal& getDV() const { return *m_dv; }

protected:
	RDORTPDefVal* m_dv;

	// Для глобальный типов, напрмиер, для параметров стандартных фыункций
	RDORTPParamType( RDOParser* _parser, RDORTPDefVal* _dv ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo(),
		m_dv( _dv )
	{
	}
	// Для нормальных параметров
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo(),
		m_dv( _dv )
	{
	}
	RDORTPParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info ),
		m_dv( _dv )
	{
	}
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
	const std::string&           name() const       { return src_info().src_text(); }
	const RDORTPParamType* const getType() const    { return m_parType; }
	const RDORTPResType* const   getResType() const { return m_resType; }
	void writeModelStructure( std::ostream& stream ) const;

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
	const std::string& name() const          { return src_text();   };
	int getNumber() const                    { return m_number;     };
	bool isPermanent() const                 { return m_permanent;  };
	bool isTemporary() const                 { return !m_permanent; };

	void addParam( const RDORTPParam* const param );
	void addParam( const std::string param_name, rdoRuntime::RDOValue::Type param_type );
	const RDORTPParam* findRTPParam( const std::string& param ) const;

	int getRTPParamNumber( const std::string& param ) const;
	const std::vector< const RDORTPParam* >& getParams() const { return m_params; }


	void writeModelStructure( std::ostream& stream ) const;

//	const std::vector< const RDORTPFuzzyParam* >& getFuzzyParams() const { return m_fuzzy_params; }
//	void addFuzzyParam( const RDORTPFuzzyParam* const fuzzy_param );
//	int getRTPFuzzyParamNumber( const std::string& fuzzy_param ) const;
//	const RDORTPFuzzyParam* findRTPFuzzyParam( const std::string& fuzzy_param ) const;
private:
	const int                         m_number;
	const bool                        m_permanent;
	std::vector< const RDORTPParam* > m_params;
//	std::vector< const RDORTPFuzzyParam* > m_fuzzy_params;
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
	virtual const std::string& getStringValue() const;
	bool isExist() const { return exist; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDefVal - значение по-умолчанию, например, = 20
// ----------------------------------------------------------------------------
class RDORTPIntDefVal: public RDORTPDefVal
{
public:
	RDORTPIntDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false )
	{
	}
	RDORTPIntDefVal( RDOParser* _parser, const RDORTPIntDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		m_val( copy.m_val )
	{
	}
	RDORTPIntDefVal( const RDORTPIntDefVal& copy ):
		RDORTPDefVal( copy.parser(), copy ),
		m_val( copy.m_val )
	{
	}
	RDORTPIntDefVal( RDOParser* _parser, int val ):
		RDORTPDefVal( _parser, true ),
		m_val( val )
	{
		setSrcText( rdo::format("%d", m_val) );
	}
	RDORTPIntDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info )
	{
	}
	RDORTPIntDefVal( RDOParser* _parser, int val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		m_val( val )
	{
		setSrcText( rdo::format("%d", m_val) );
	}
	virtual int getIntValue() const { return m_val; }

private:
	int m_val;
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
		RDOParserObject( copy.parser() ),
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
	RDORTPDiap( RDOParser* _parser, T min_value, T max_value ):
		RDOParserObject( _parser ),
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
			parser()->error( *max_value_pos, "Левая граница диапазона должна быть меньше правой" );
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
public:
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
	virtual rdoRuntime::RDOValue getDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSStringValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;					// the function also check range if exist
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual rdoRuntime::RDOValue::Type getType() const { return rdoRuntime::RDOValue::rvt_int; }
	virtual void writeModelStructure( std::ostream& stream ) const;

	const RDORTPIntDiap& getDiap() const { return *m_diap; }

private:
	RDORTPIntDiap* m_diap;
	void init();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDefVal - значение по-умолчанию, например, = 20.2
// ----------------------------------------------------------------------------
class RDORTPRealDefVal: public RDORTPDefVal
{
public:
	RDORTPRealDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false )
	{
	}
	RDORTPRealDefVal( RDOParser* _parser, const RDORTPRealDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		m_val( copy.m_val )
	{
	}
	RDORTPRealDefVal( const RDORTPRealDefVal& copy ):
		RDORTPDefVal( copy.parser(), copy ),
		m_val( copy.m_val )
	{
	}
	RDORTPRealDefVal( RDOParser* _parser, double val ):
		RDORTPDefVal( _parser, true ),
		m_val( val )
	{
		setSrcText( rdo::format("%f", m_val) );
	}
	RDORTPRealDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info )
	{
	}
	RDORTPRealDefVal( RDOParser* _parser, double val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		m_val( val )
	{
		setSrcText( rdo::format("%f", m_val) );
	}
	virtual double getRealValue() const { return m_val; }

private:
	double m_val;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
class RDORTPRealParamType: public RDORTPParamType
{
public:
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
	virtual rdoRuntime::RDOValue getDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSStringValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;				// this function too
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info )const ; 	// the function also check range if exist
	virtual int getDiapTableFunc() const;
	virtual rdoRuntime::RDOValue::Type getType() const { return rdoRuntime::RDOValue::rvt_real; }
	virtual void writeModelStructure( std::ostream& stream ) const;
	const RDORTPRealDiap& getDiap() const { return *m_diap; }

private:
	RDORTPRealDiap* m_diap;
	void init();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
class RDORTPEnum: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPEnum( const RDOParserObject* _parent, const std::string& first );
	virtual ~RDORTPEnum();

	void add( const RDOParserSrcInfo& next );
	rdoRuntime::RDOValue findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const;
	rdoRuntime::RDOValue getFirstValue() const;
	rdoRuntime::RDOEnum& getEnums() { return *m_enums; }

private:
	rdoRuntime::RDOEnum* m_enums;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumDefVal
// ----------------------------------------------------------------------------
class RDORTPEnumDefVal: public RDORTPDefVal
{
public:
	RDORTPEnumDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false ),
		m_val( "" )
	{
	}
	RDORTPEnumDefVal( RDOParser* _parser, const RDORTPEnumDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		m_val( copy.m_val )
	{
		setSrcText( m_val );
	}
	RDORTPEnumDefVal( const RDORTPEnumDefVal& copy ):
		RDORTPDefVal( copy.parser(), copy ),
		m_val( copy.m_val )
	{
		setSrcText( m_val );
	}
	RDORTPEnumDefVal( RDOParser* _parser, const std::string& _val ):
		RDORTPDefVal( _parser, true ),
		m_val( _val )
	{
		setSrcText( m_val );
	}
	RDORTPEnumDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info ),
		m_val( "" )
	{
	}
	RDORTPEnumDefVal( RDOParser* _parser, const std::string& _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		m_val( _val )
	{
		setSrcText( m_val );
	}
	virtual const std::string& getEnumValue() const { return m_val; }

private:
	std::string m_val;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
class RDORTPEnumParamType: public RDORTPParamType
{
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
	virtual rdoRuntime::RDOValue getDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSStringValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual rdoRuntime::RDOValue::Type getType() const { return rdoRuntime::RDOValue::rvt_enum; }
	virtual void writeModelStructure( std::ostream& stream ) const;

private:
	void init_src_info();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPStringDefVal
// ----------------------------------------------------------------------------
class RDORTPStringDefVal: public RDORTPDefVal
{
public:
	RDORTPStringDefVal( RDOParser* _parser ):
		RDORTPDefVal( _parser, false ),
		m_val( "" )
	{
	}
	RDORTPStringDefVal( RDOParser* _parser, const RDORTPStringDefVal& copy ):
		RDORTPDefVal( _parser, copy ),
		m_val( copy.m_val )
	{
		setSrcText( m_val );
	}
	RDORTPStringDefVal( const RDORTPStringDefVal& copy ):
		RDORTPDefVal( copy.parser(), copy ),
		m_val( copy.m_val )
	{
		setSrcText( m_val );
	}
	RDORTPStringDefVal( RDOParser* _parser, const std::string& _val ):
		RDORTPDefVal( _parser, true ),
		m_val( _val )
	{
		setSrcText( m_val );
	}
	RDORTPStringDefVal( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, false, _src_info ),
		m_val( "" )
	{
	}
	RDORTPStringDefVal( RDOParser* _parser, const std::string& _val, const RDOParserSrcInfo& _src_info ):
		RDORTPDefVal( _parser, true, _src_info ),
		m_val( _val )
	{
		setSrcText( m_val );
	}
	virtual const std::string& getStringValue() const { return m_val; }

private:
	std::string m_val;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPStringParamType
// ----------------------------------------------------------------------------
class RDORTPStringParamType: public RDORTPParamType
{
public:
	RDORTPStringParamType( const RDOParserObject* _parent, RDORTPStringDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual const RDORTPParamType* constructSuchAs( const RDOParserSrcInfo& _src_info ) const;
	virtual const RDORTPParamType* constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual const RDORTPParamType* constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const;
	virtual void checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual void checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getDefaultValue( const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSStringValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const;
	virtual rdoRuntime::RDOValue getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const;
	virtual int getDiapTableFunc() const;
	virtual rdoRuntime::RDOValue::Type getType() const { return rdoRuntime::RDOValue::rvt_string; }
	virtual void writeModelStructure( std::ostream& stream ) const;
};
// ----------------------------------------------------------------------------
//------------------------------ FOR FUZZY LOGIC ------------------------------	
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftPoint - точка ф-ии принадлежности нечеткого терма
// ----------------------------------------------------------------------------
class RDORTPFuzzyMembershiftPoint: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyMembershiftPoint( RDOParser* _parser, const RDOParserSrcInfo& _src_info, double x_value, double y_value ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_x_value( x_value ),
		m_y_value( y_value )
		{
		}
	double  getX() const { return m_x_value; }
	double  getY() const { return m_y_value; }

private:
	double    m_x_value;
	double    m_y_value;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности для нечеткого терма
// ----------------------------------------------------------------------------

class RDORTPFuzzyMembershiftFun: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyMembershiftFun( RDOParser* _parser ):
		RDOParserObject( _parser )
		{
		}
	typedef RDORTPFuzzyMembershiftPoint* Item;
	typedef std::vector< Item >          Items;

	void add( Item point )
	{
		m_points.push_back( point );
	}
	double  getVal() const 
	{ 
		return m_value; 
	}

private:
	Items m_points;	// точки, определяющие ф-ию принадлежности
	double m_value;	// значение ф-ии принадлежности для конкретного четкого значения
};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTerm - нечеткий термин
// ----------------------------------------------------------------------------
class RDORTPFuzzyTerm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyTerm( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyMembershiftFun* membersfift_fun):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_fun (membersfift_fun)
		{
		}
	
	const	std::string& name()	const	{ return src_info().src_text(); }
	double  MemberShift()			const	{ return m_fun->getVal(); }

private:
	RDORTPFuzzyMembershiftFun* m_fun;
};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTermsSet - набор терминов одного параметра
// ----------------------------------------------------------------------------
class RDORTPFuzzyTermsSet: public RDOParserObject, public RDOParserSrcInfo
{
	public:
	RDORTPFuzzyTermsSet( RDOParser* _parser ):
		RDOParserObject( _parser )
		{
		}
	typedef RDORTPFuzzyTerm*	Item;
	typedef std::vector< Item >	Items;

	void add( Item term )
	{
		m_terms.push_back( term );
	}

private:
	Items m_terms;	// набор терминов одного параметра

};
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyParam
// ----------------------------------------------------------------------------
class RDORTPFuzzyParam : public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDORTPFuzzyParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyTermsSet* terms_set ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo( _src_info),
		m_set (terms_set)
		{
		}
	const std::string&           name() const       { return src_info().src_text(); }

private:
	RDORTPFuzzyTermsSet* m_set; // набор терминов параметра
};


} // namespace rdoParse

#endif // RDORTP_H
