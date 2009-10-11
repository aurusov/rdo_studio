#ifndef RDORTP_H
#define RDORTP_H

#include "rdoparser_object.h"
#include "rdoparser_value.h"
#include "rdoparser_type.h"
#include "rdoparser_enum.h"
#include <rdoruntime_object.h>
#include <rdo_value.h>

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
	virtual void                  checkValue         ( const RDOValue& value ) const = 0;
	virtual rdoRuntime::RDOValue  getValue           ( const RDOValue& value ) const = 0;
	virtual rdoRuntime::RDOValue  getDefaultValue    ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs  ( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const = 0;

	virtual unsigned int          getDiapTableFunc   () const;

	virtual void                  writeModelStructure( std::ostream& stream ) const = 0;

	virtual const RDOType&       type() const = 0;
	rdoRuntime::RDOType::TypeID  typeID() const { return type()->typeID(); }

	void checkParamType( const RDOFUNArithm* const action ) const;

	const RDORTPDefVal& getDV() const { return *m_dv; }

protected:
	RDORTPDefVal* m_dv;

	// Для глобальный типов, напримиер, для параметров стандартных функций
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
	virtual ~RDORTPParamType() {};
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
	virtual ~RDORTPParam() {}
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
	virtual ~RDOFUNConst() {}
};

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
class RDORTPResType: public RDOParserObject, public RDOParserSrcInfo
{
public:
	typedef std::vector<CPTR(RDORTPParam)> ParamList;

	enum { UNDEFINED_PARAM = ~0 };

	RDORTPResType(PTR(RDOParser) _parser, CREF(RDOParserSrcInfo) _src_info, rbool _permanent);
	virtual ~RDORTPResType();
	CREF(tstring) name       () const   { return src_text();   };
	rsint         getNumber  () const   { return m_number;     };
	rbool         isPermanent() const   { return m_permanent;  };
	rbool         isTemporary() const   { return !m_permanent; };

	void addParam(CPTRC(RDORTPParam) param);
	void addParam(CREF(tstring) param_name, rdoRuntime::RDOType::TypeID param_typeID);
	CPTR(RDORTPParam) findRTPParam(CREF(tstring) param) const;

	ruint           getRTPParamNumber(CREF(tstring) param) const;
	CREF(ParamList) getParams        ()                    const { return m_params; }

	void writeModelStructure(REF(std::ostream) stream) const;

private:
	const rsint  m_number;
	const rbool  m_permanent;
	ParamList    m_params;
};

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - значение по-умолчанию
// ----------------------------------------------------------------------------
// Сначала цепляется к парсеру, а потом идет reparent на тип ресурса
// ----------------------------------------------------------------------------
class RDORTPDefVal: public RDOParserObject
{
public:
	RDORTPDefVal( RDOParser* _parser ):
		RDOParserObject( _parser )
	{
	}
	RDORTPDefVal( RDOParser* _parser, const RDOValue& value ):
		RDOParserObject( _parser ),
		m_value( value )
	{
	}
	RDORTPDefVal( const RDORTPDefVal& copy ):
		RDOParserObject( copy.parser() ),
		m_value( copy.m_value )
	{
	}
	virtual ~RDORTPDefVal()
	{
	}

	rbool isExist() const { return m_value.defined(); }

	const RDOValue& value() const { return m_value; }

	void setSrcInfo( const RDOParserSrcInfo& src_info )
	{
		m_value.setSrcInfo( src_info );
	}
	void setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
	{
		m_value.setSrcPos( first_line, first_pos, last_line, last_pos );
	}
	void setSrcFileType( rdoModelObjects::RDOFileType value )
	{
		m_value.setSrcFileType( value );
	}

private:
	RDOValue m_value;
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
	virtual ~RDORTPDiap() {}

	rbool isExist() const { return m_exist;     }
	T  getMin() const    { return m_min_value; }
	T  getMax() const    { return m_max_value; }

private:
	rbool m_exist;
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
	RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv );
	RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual ~RDORTPIntParamType() {}

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual unsigned int getDiapTableFunc() const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const RDOType& type() const { return rdoParse::g_int; }

	const RDORTPIntDiap& getDiap() const { return *m_diap; }

private:
	RDORTPIntDiap* m_diap;
	void init();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
class RDORTPRealParamType: public RDORTPParamType
{
public:
	RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv );
	RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual ~RDORTPRealParamType() {}

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const RDOType& type() const { return rdoParse::g_real; }
	const RDORTPRealDiap& getDiap() const { return *m_diap; }

private:
	RDORTPRealDiap* m_diap;
	void init();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
class RDORTPEnumParamType: public RDORTPParamType
{
public:
	RDORTPEnum* m_enum;
	std::string enum_name; // Используется в сообщениях об ошибках
	rbool        enum_fun;  // Используется в сообщениях об ошибках

	RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual ~RDORTPEnumParamType() {}

	virtual void                  checkValue       ( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue         ( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getDefaultValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual unsigned int getDiapTableFunc() const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const RDOType& type() const { return *m_enum; }

private:
	void init_src_info();
};

// ----------------------------------------------------------------------------
// ---------- RDORTPStringParamType
// ----------------------------------------------------------------------------
class RDORTPStringParamType: public RDORTPParamType
{
public:
	RDORTPStringParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual ~RDORTPStringParamType() {}

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const RDOType& type() const { return rdoParse::g_string; }
};

// ----------------------------------------------------------------------------
// ---------- RDORTPBoolParamType
// ----------------------------------------------------------------------------
class RDORTPBoolParamType: public RDORTPParamType
{
public:
	RDORTPBoolParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info );
	virtual ~RDORTPBoolParamType() {}

	virtual void                  checkValue( const RDOValue& value ) const;
	virtual rdoRuntime::RDOValue  getValue  ( const RDOValue& value ) const;

	virtual RDORTPParamType*      constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue = RDOValue() ) const;

	virtual void writeModelStructure( std::ostream& stream ) const;
	virtual const RDOType& type() const { return rdoParse::g_bool; }
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
	virtual ~RDORTPFuzzyMembershiftPoint() {}

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
	virtual ~RDORTPFuzzyMembershiftFun() {}

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
	virtual ~RDORTPFuzzyTerm() {}

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
	RDORTPFuzzyTermsSet( RDOParser* _parser )
		: RDOParserObject( _parser )
	{
	}
	virtual ~RDORTPFuzzyTermsSet() {}

	typedef RDORTPFuzzyTerm*	Item;
	typedef std::vector< Item >	Items;

	void add( Item term )
	{
		m_terms.push_back( term );
	}
	rbool empty() const
	{
		return m_terms.empty();
	}

private:
	Items m_terms; // набор терминов одного параметра
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
	virtual ~RDORTPFuzzyParam() {}

	const std::string&           name() const       { return src_info().src_text(); }

private:
	RDORTPFuzzyTermsSet* m_set; // набор терминов параметра
};

} // namespace rdoParse

#endif // RDORTP_H
