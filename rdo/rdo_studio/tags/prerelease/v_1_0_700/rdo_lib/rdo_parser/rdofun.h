#ifndef RDOFUN_H
#define RDOFUN_H

#include "rdoparser_object.h"
#include "rdoparser_value.h"
#include "rdortp.h"
#include <rdoruntime_object.h>
#include <rdo_type.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOCalcConst;
class RDOCalcIsEqual;
class RDOCalcFuncParam;
class RDOCalcDoubleToIntByResult;
class RDOCalcFunctionCall;
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

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
class RDOFUNDoubleToIntByResult
{
public:
	void roundCalc();
	void push_back( rdoRuntime::RDOCalcDoubleToIntByResult* calc )
	{
		m_int_or_double.push_back( calc );
	}
	void insert( const RDOFUNDoubleToIntByResult& first )
	{
		m_int_or_double.insert( m_int_or_double.end(), first.m_int_or_double.begin(), first.m_int_or_double.end() );
	}
	void insert( const RDOFUNDoubleToIntByResult& first, const RDOFUNDoubleToIntByResult& second )
	{
		m_int_or_double.insert( m_int_or_double.end(), first.m_int_or_double.begin(), first.m_int_or_double.end() );
		m_int_or_double.insert( m_int_or_double.end(), second.m_int_or_double.begin(), second.m_int_or_double.end() );
	}

private:
	std::vector< rdoRuntime::RDOCalcDoubleToIntByResult* > m_int_or_double;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
class RDOFUNLogic: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOFUNArithm;

public:
	rdoRuntime::RDOCalc* getCalc( rdoRuntime::RDOType::TypeID _id = rdoRuntime::RDOType::t_real );

	RDOFUNLogic( const RDOFUNArithm& arithm );
	RDOFUNLogic( const RDOParserObject* _parent, rdoRuntime::RDOCalc* _calc, bool hide_warning = false );
	virtual ~RDOFUNLogic() {}

	RDOFUNLogic* operator &&( const RDOFUNLogic& second );
	RDOFUNLogic* operator ||( const RDOFUNLogic& second );
	RDOFUNLogic* operator_not();

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const RDOSrcInfo::Position& _pos );
	virtual void setSrcText( const std::string& value );
	void setSrcPos( const YYLTYPE& _error_pos )
	{
		RDOParserSrcInfo::setSrcPos( _error_pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
	{
		RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	}

private:
	rdoRuntime::RDOCalc*      m_calc;
	RDOFUNDoubleToIntByResult m_int_or_double;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
class RDOFUNArithm: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNArithm( RDOParser* parser, const RDOValue& value );
	RDOFUNArithm( RDOParser* parser, const RDOValue& value, rdoRuntime::RDOCalc* calc );
	RDOFUNArithm( RDOParser* parser, const RDOValue& res_name, const RDOValue& par_name );

	RDOFUNArithm( const RDOFUNArithm*    parent, const RDOValue& value );
	RDOFUNArithm( const RDOParserObject* parent, const RDOValue& value, rdoRuntime::RDOCalc* calc );
	RDOFUNArithm( const RDOFUNArithm*    parent, const RDOValue& res_name, const RDOValue& par_name );

	virtual ~RDOFUNArithm() {}

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

	rdoRuntime::RDOCalc*           createCalc( const RDORTPParamType* const forType = NULL );
	rdoRuntime::RDOCalc*           calc() const     { return m_calc;           }
	const RDOValue&                value() const    { return m_value;          }
	const RDOType&                 type() const     { return m_value.type();   }
	const RDORTPEnum&              enumType() const { return static_cast<const RDORTPEnum&>(type()); }
	rdoRuntime::RDOType::TypeID    typeID() const   { return type()->typeID(); }

	virtual void setSrcInfo( const RDOParserSrcInfo& src_info );
	virtual void setSrcPos( const RDOSrcInfo::Position& _pos );
	virtual void setSrcText( const std::string& value );
	void setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end )
	{
		RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	}
	void setSrcPos( const YYLTYPE& _error_pos )
	{
		RDOParserSrcInfo::setSrcPos( _error_pos );
	}
	void setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
	{
		RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	}

private:
	RDOValue                   m_value;
	rdoRuntime::RDOCalc*       m_calc;
	RDOFUNDoubleToIntByResult  m_int_or_double;

	void init( const RDOValue& value );
	void init( const RDOValue& res_name, const RDOValue& par_name );

	enum CastResult
	{
		CR_DONE,
		CR_CONTINUE
	};
	CastResult beforeCastValue( RDOFUNArithm& second );
	const RDOType* getPreType( const RDOFUNArithm& second );
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
class RDOFUNConstant: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNConstant( RDOParser* _parser, RDOFUNConst* _const );
	virtual ~RDOFUNConstant() {}

	const std::string&           name() const      { return m_const->name();    }
	const RDORTPParamType* const getType() const   { return m_const->getType(); }
	const RDOFUNConst* const     getDescr() const  { return m_const;            }
	int                          getNumber() const { return m_number;           }

private:
	RDOFUNConst* m_const;
	int          m_number;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// ----------------------------------------------------------------------------
class RDOFUNParams: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOParserSrcInfo funseq_name;
	std::vector< RDOFUNArithm* > params;

	RDOFUNParams( RDOParser* _parser ):
		RDOParserObject( _parser ),
		RDOParserSrcInfo()
	{
	}
	RDOFUNParams( const RDOParserObject* _parent ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo()
	{
	}
	virtual ~RDOFUNParams() {}

	void addParameter( RDOFUNArithm* param ) {
		params.push_back( param );
	}
	RDOFUNArithm* createCall( const std::string& funName ) const;
	RDOFUNArithm* createSeqCall( const std::string& seqName ) const;
};

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
class RDOFUNSequence: public RDOParserObject, public RDOParserSrcInfo
{
public:
	class RDOFUNSequenceHeader: public RDOParserObject, public RDOParserSrcInfo
	{
	private:
		const RDORTPParamType* type;

	public:
		RDOFUNSequenceHeader( RDOParser* _parser, const RDORTPParamType* _type, const RDOParserSrcInfo& _src_info ):
			RDOParserObject( _parser ),
			RDOParserSrcInfo( _src_info ),
			type( _type )
		{
		}
		const RDORTPParamType* getType() const { return type; }
	};

	RDOFUNSequenceHeader* header;
	rdoRuntime::RDOCalcSeqInit* init_calc;
	rdoRuntime::RDOCalcSeqNext* next_calc;

protected:
	int base;

	RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base );
	virtual ~RDOFUNSequence() {}

	void initResult();
	void initCalcSrcInfo();

public:
	const std::string& name() const  { return header->src_text(); }
	virtual void createCalcs() = 0;
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const = 0;
};

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
class RDOFUNSequenceUniform: public RDOFUNSequence
{
private:
	virtual void createCalcs();

public:
	RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
class RDOFUNSequenceExponential: public RDOFUNSequence
{
private:
	virtual void createCalcs();

public:
	RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base =123456789 );
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
class RDOFUNSequenceNormal: public RDOFUNSequence
{
private:
	virtual void createCalcs();

public:
	RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base = 123456789 );
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHist: public RDOFUNSequence
{
public:
	class RDOFUNSequenceByHistHeader: public RDOParserObject, public RDOParserSrcInfo
	{
	public:
		RDOFUNSequence::RDOFUNSequenceHeader* header;
		int                                   base;

		RDOFUNSequenceByHistHeader( RDOParser* _parser, RDOFUNSequence::RDOFUNSequenceHeader* _header, int _base = 123456789 ):
			RDOParserObject( _parser ),
			RDOParserSrcInfo( _header->src_info() ),
			header( _header ),
			base( _base )
		{
		}
	};

	RDOFUNSequenceByHist( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header ):
		RDOFUNSequence( _parser, _header->header, _header->base )
	{
		_header->reparent( this );
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistReal: public RDOFUNSequenceByHist
{
public:
	std::vector< rdoRuntime::RDOValue > m_from;
	std::vector< rdoRuntime::RDOValue > m_to;
	std::vector< rdoRuntime::RDOValue > m_freq;

	RDOFUNSequenceByHistReal( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, const RDOValue& from, const RDOValue& to, const RDOValue& freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addReal( from, to, freq );
	}
	void addReal( const RDOValue& from, const RDOValue& to, const RDOValue& freq );

private:
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
class RDOFUNSequenceByHistEnum: public RDOFUNSequenceByHist
{
public:
	std::vector< rdoRuntime::RDOValue > m_values;
	std::vector< rdoRuntime::RDOValue > m_freq;

	RDOFUNSequenceByHistEnum( RDOParser* _parser, RDOFUNSequenceByHistHeader* _header, const RDOValue& value, const RDOValue& freq ):
		RDOFUNSequenceByHist( _parser, _header )
	{
		addEnum( value, freq );
	}
	void addEnum( const RDOValue& value, const RDOValue& freq );

private:
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
class RDOFUNSequenceEnumerative: public RDOFUNSequence
{
public:
	RDOFUNSequenceEnumerative( RDOParser* _parser, RDOFUNSequenceHeader* _header, const RDOValue& value ):
		RDOFUNSequence( _parser, _header, 0 )
	{
		addValue( value );
	}
	virtual ~RDOFUNSequenceEnumerative()
	{
	}
	void addValue( const RDOValue& value )
	{
		m_values.push_back( header->getType()->getValue(value) );
	}
	virtual RDOFUNArithm* createCallCalc( const RDOFUNParams* const params, const RDOParserSrcInfo& src_info ) const;

private:
	std::vector< RDOValue > m_values;
	virtual void createCalcs();
};

// ----------------------------------------------------------------------------
// ---------- Функции
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionParam
// ----------------------------------------------------------------------------
// Параметры, описанные внури самой функции после клювевого слова $Parameters
// ----------------------------------------------------------------------------
class RDOFUNFunctionParam: public RDOParserObject, public RDOParserSrcInfo
{
private:
	RDORTPParamType* type;

public:
	RDOFUNFunctionParam( const RDOParserObject* _parent, const std::string& _name, RDORTPParamType* _type ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _name ),
		type( _type )
	{
	}
	RDOFUNFunctionParam( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, RDORTPParamType* _type ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info ),
		type( _type )
	{
	}
	virtual ~RDOFUNFunctionParam() {}

	const std::string& name() const              { return src_info().src_text(); }
	const RDORTPParamType* const getType() const { return type;                  }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElement: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNFunctionListElement( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info ):
		RDOParserObject( _parent ),
		RDOParserSrcInfo( _src_info )
	{
	}
	virtual ~RDOFUNFunctionListElement() {}
	virtual rdoRuntime::RDOCalcIsEqual* createIsEqualCalc( const RDORTPParamType* const retType, rdoRuntime::RDOCalcFuncParam* const funcParam, const RDOParserSrcInfo& _src_pos ) const;
	virtual rdoRuntime::RDOCalcConst*   createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const = 0;
	virtual bool isEquivalence() const { return false; }
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementIdentif: public RDOFUNFunctionListElement
{
public:
	RDOFUNFunctionListElementIdentif( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info ):
		RDOFUNFunctionListElement( _parent, _src_info )
	{
	}
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementReal: public RDOFUNFunctionListElement
{
public:
	double value;

	RDOFUNFunctionListElementReal( const RDOParserObject* _parent, const YYLTYPE& _pos, double _value ):
		RDOFUNFunctionListElement( _parent, RDOParserSrcInfo( _pos, rdo::format("%f", _value) ) ),
		value( _value )
	{
	}
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementInt: public RDOFUNFunctionListElement
{
public:
	int value;

	RDOFUNFunctionListElementInt( const RDOParserObject* _parent, const YYLTYPE& _pos, int _value ):
		RDOFUNFunctionListElement( _parent, RDOParserSrcInfo( _pos, rdo::format("%d", _value) ) ),
		value( _value )
	{
	}
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
class RDOFUNFunctionListElementEq: public RDOFUNFunctionListElement
{
public:
	RDOFUNFunctionListElementEq( const RDOParserObject* _parent, const YYLTYPE& _pos ):
		RDOFUNFunctionListElement( _parent, RDOParserSrcInfo( _pos, "=" ) )
	{
	}
	virtual bool isEquivalence() const { return true; }
	virtual rdoRuntime::RDOCalcConst* createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
class RDOFUNCalculateIf: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFUNLogic*  condition;
	RDOFUNArithm* action;

	RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, RDOFUNArithm* _action );
	virtual ~RDOFUNCalculateIf() {}
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
class RDOFUNFunction: public RDOParserObject, public RDOParserSrcInfo
{
friend class RDOParser;
public:
	RDOFUNFunction( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _retType );
	RDOFUNFunction( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _retType );
	virtual ~RDOFUNFunction() {}

	void add( const RDOFUNFunctionParam* const _param );
	void add( const RDOFUNFunctionListElement* const _listElement );
	void add( const RDOFUNCalculateIf* const _calculateIf );
	const RDOFUNFunctionParam* const findFUNFunctionParam( const std::string& paramName ) const;
	int findFUNFunctionParamNum( const std::string& paramName ) const;
	void createListCalc();
	void createTableCalc( const YYLTYPE& _elements_pos );
	void createAlgorithmicCalc( const RDOParserSrcInfo& _body_src_info );
	const std::string& name() const                                   { return src_info().src_text(); }
	const std::vector< const RDOFUNFunctionParam* > getParams() const { return params;                }

	void setFunctionCalc( rdoRuntime::RDOFunCalc* calc );
	rdoRuntime::RDOFunCalc* getFunctionCalc() const                   { return functionCalc;          }

	const RDORTPParamType* const getType() const                      { return retType;               }
	void insertPostLinked( rdoRuntime::RDOCalcFunctionCall* calc ) {
		post_linked.push_back( calc );
	}

private:
	const RDORTPParamType* const retType;
	std::vector< const RDOFUNFunctionParam* >       params;
	std::vector< const RDOFUNFunctionListElement* > elements;    // for list and table
	std::vector< const RDOFUNCalculateIf* >         calculateIf; // for algorithmic
	rdoRuntime::RDOFunCalc* functionCalc;
	std::vector< rdoRuntime::RDOCalcFunctionCall* > post_linked; // для рекурсивного вызова
};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
class RDOFUNGroup: public RDOParserObject, public RDOParserSrcInfo
{
protected:
	void init( const RDOParserSrcInfo& _res_info );

public:
	const RDORTPResType* resType;

	RDOFUNGroup( RDOParser* _parser, const RDOParserSrcInfo& _res_info );
	RDOFUNGroup( const RDOParserObject* _parent, const RDOParserSrcInfo& _res_info );
	virtual ~RDOFUNGroup() {}
};

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
class RDOFUNGroupLogic: public RDOFUNGroup
{
public:
	enum FunGroupType {
		fgt_unknow    = 0,
		fgt_exist     = 1,
		fgt_notexist  = 2,
		fgt_forall    = 3,
		fgt_notforall = 4
	};

private:
	const int funType;

public:
	RDOFUNGroupLogic( RDOParser* _parser, FunGroupType _funType, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parser, _res_info ),
		funType( _funType )
	{
	}
	RDOFUNGroupLogic( const RDOParserObject* _parent, FunGroupType _funType, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parent, _res_info ),
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
	RDOFUNSelect( RDOParser* _parser, const RDOParserSrcInfo& _res_info ):
		RDOFUNGroup( _parser, _res_info.src_text() ),
		select( NULL )
	{
	}
	void initSelect( const RDOFUNLogic* cond = NULL );
	RDOFUNLogic* createFunSelectGroup( RDOFUNGroupLogic::FunGroupType funType, RDOFUNLogic* cond );
	RDOFUNLogic* createFunSelectEmpty( const RDOParserSrcInfo& _empty_info );
	RDOFUNArithm* createFunSelectSize( const RDOParserSrcInfo& _size_info );
};

} // namespace rdoParse 

#endif // RDOFUN_H
