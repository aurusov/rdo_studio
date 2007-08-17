#include "pch.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdoDefines.h"
#include "rdoparser_lexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void rtperror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - значение по-умолчанию, базовый класс
// ----------------------------------------------------------------------------
RDORTPDefVal::RDORTPDefVal( const RDORTPDefVal& dv ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( dv.src_info() ),
	exist( dv.exist )
{
}

RDORTPDefVal::RDORTPDefVal( bool _exist ):
	RDOParserObject( parser ),
	RDOParserSrcInfo(),
	exist( _exist )
{
}

RDORTPDefVal::RDORTPDefVal( bool _exist, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( _exist )
{
}

int RDORTPDefVal::getIntValue() const
{
	parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
	parser->error( "Неверное значение по-умолчанию" );
//	parser->error( "Invalid default value" );
	return 0;	// unreachable code...
}

double RDORTPDefVal::getRealValue() const
{
	parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
	parser->error( "Неверное значение по-умолчанию" );
	return 0.;	// unreachable code...
}

const std::string& RDORTPDefVal::getEnumValue() const
{
	parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
	parser->error( "Неверное значение по-умолчанию" );
#pragma warning( disable: 4172 )
	return "";	// unreachable code...
#pragma warning( default: 4172 )
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
void RDORTPParamType::checkParamType( const RDOFUNArithm* const action ) const
{
	switch ( getType() ) {
		case RDORTPParamType::pt_int: {
			if ( action->getType() == RDORTPParamType::pt_real ) {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( action->src_pos().last_line, action->src_pos().last_pos );
				parser->warning( "Перевод вещественного числа в целое, возможна потеря данных" );
				parser->lexer_loc_restore();
			} else if ( action->getType() != RDORTPParamType::pt_int ) {
				parser->lexer_loc_set( action->src_pos().last_line, action->src_pos().last_pos );
				parser->error( "Несоответствие типов. Ожидается целое число" );
			}
			break;
		}
		case RDORTPParamType::pt_real: {
			if ( action->getType() != RDORTPParamType::pt_real && action->getType() != RDORTPParamType::pt_int ) {
				parser->lexer_loc_set( action->src_pos().last_line, action->src_pos().last_pos );
				parser->error( "Несоответствие типов. Ожидается вещественное число" );
			}
			break;
		}
		case RDORTPParamType::pt_enum: {
			if ( action->getType() == RDORTPParamType::pt_unknow ) {
				if ( static_cast<const RDORTPEnumParamType*>(this)->enu->findValue( action->str, false ) == -1 ) {
					parser->lexer_loc_set( action->src_pos().last_line, action->src_pos().last_pos );
					if ( static_cast<const RDORTPEnumParamType*>(this)->enum_fun ) {
						parser->error( rdo::format("Значение '%s' не может являться результатом функции: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					} else {
						parser->error( rdo::format("Значение '%s' не является элементом перечислимого параметра: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
				}
			} else if ( action->getType() != RDORTPParamType::pt_enum ) {
				parser->lexer_loc_set( action->src_pos().last_line, action->src_pos().last_pos );
				parser->error( "Несоответствие типов. Ожидается перечислимый тип" );
			} else if ( action->enu != static_cast<const RDORTPEnumParamType*>(this)->enu ) {
				parser->lexer_loc_set( action->src_pos().last_line, action->src_pos().last_pos );
				parser->error( "Несоответствие перечислимых типов" );
			}
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	name( _name ),
	parType( _parType ),
	resType( NULL )
{
	setSrcText( name + ": " + parType->src_text() );
}

RDORTPParam::RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo(),
	name( _name ),
	parType( _parType ),
	resType( _parent )
{
	setSrcText( name + ": " + parType->src_text() );
}

RDORTPParam::RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	name( _name ),
	parType( _parType ),
	resType( _parent )
{
	setSrcText( name + ": " + parType->src_text() );
}

int RDORTPParam::writeModelStructure() const
{
	parser->modelStructure << getName() << " ";
	getType()->writeModelStructure();
//	parser->modelStructure << " ";
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
RDORTPResType::RDORTPResType( RDOParser* _parser, const std::string& _name, const bool _permanent ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	name( _name ),
	number( parser->getRTP_id() ),
	permanent( _permanent )
{
	setSrcText( name );
	parser->insertRTPResType( this );
}

void RDORTPResType::addParam( const RDORTPParam* const param )
{
	if ( findRTPParam( param->getName() ) ) {
		parser->lexer_loc_restore();
		parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_PARAM_NAME, param->getName().c_str() );
	}
	params.push_back( param );
}

const RDORTPParam* RDORTPResType::findRTPParam( const std::string& param ) const
{
	std::vector<const RDORTPParam *>::const_iterator it = std::find_if( params.begin(), params.end(), compareName2<RDORTPParam>(param) );
	return it != params.end() ? *it : NULL;
}

int RDORTPResType::getRTPParamNumber( const std::string& param ) const
{
	std::vector<const RDORTPParam *>::const_iterator it = std::find_if(params.begin(), 
		params.end(), 
		compareName2<RDORTPParam>(param));
	if(it != params.end())
		return it - params.begin();

	return -1;
}

int RDORTPResType::writeModelStructure() const
{
	parser->modelStructure << getNumber() << " " << getName() << " " << getParams().size() << std::endl;
	for(int i = 0; i < getParams().size(); i++)
	{
		parser->modelStructure << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure();
	}
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - даипазон, например, [1 .. 4]
// ----------------------------------------------------------------------------
RDORTPIntDiap::RDORTPIntDiap():
	RDOParserObject( parser ),
	RDOParserSrcInfo(),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPIntDiap::RDORTPIntDiap( const RDORTPIntDiap& diap ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( diap.src_info() ),
	exist( diap.exist ),
	minVal( diap.minVal ),
	maxVal( diap.maxVal )
{
}

RDORTPIntDiap::RDORTPIntDiap( const RDOParserSrcInfo& _src_info ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPIntDiap::RDORTPIntDiap( int _minVal, int _maxVal, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( true ),
	minVal( _minVal ),
	maxVal( _maxVal )
{
	if ( minVal > maxVal ) {
		parser->lexer_loc_set( _max_value_pos.last_line, _max_value_pos.last_column );
		parser->error( "Левая граница диапазона должна быть меньше правой" );
	}
	setSrcText( rdo::format("[%d..%d]", minVal, maxVal) );
}

void RDORTPIntDiap::check( const RDORTPIntDefVal* dv ) const
{
	if ( isExist() && dv->isExist() ) {
		if ((minVal > dv->getIntValue()) || (maxVal < dv->getIntValue())) {
			parser->lexer_loc_set( dv->src_pos().last_line, dv->src_pos().last_pos );
			parser->error( rdo::format("Целочисленное значение выходит за допустимый диапазон [%d..%d]: %d", minVal, maxVal, dv->getIntValue()) );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
RDORTPIntParamType::RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPIntDefVal(0) ),
	diap( new RDORTPIntDiap() )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	diap( _diap )
{
	init();
}

void RDORTPIntParamType::init()
{
	diap->reparent( this );
	dv->reparent( this );
	std::string src_text = "integer";
	if ( diap->isExist() ) {
		src_text += " " + diap->src_text();
	}
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	diap->check( static_cast<RDORTPIntDefVal*>(dv) );
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( *static_cast<RDORTPIntDefVal*>(dv) );
	RDORTPIntParamType* _type = new RDORTPIntParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( defVal, defVal_info );
	RDORTPIntParamType* _type = new RDORTPIntParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->lexer_loc_set( defVal_info.src_pos().last_line, defVal_info.src_pos().last_pos );
	parser->error( rdo::format("Неверное значение по-умолчанию для параметра целого типа: %f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->lexer_loc_set( defVal_info.src_pos().last_line, defVal_info.src_pos().last_pos );
	parser->error( rdo::format("Неверное значение по-умолчанию для параметра целого типа: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPIntParamType::writeModelStructure() const
{
	parser->modelStructure << "I" << std::endl;
	return 0;
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSEnumValue( const std::string& val ) const
{
	parser->error( rdo::format("Ожидается целое число: %s", val.c_str()) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSRealValue( const double* const val ) const
{
	parser->error( rdo::format("Ожидается целое число: %f", *val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSDefaultValue() const 
{
	if ( !dv->isExist() ) {
		parser->error( "Нет значения по-умолчанию" );
//		parser->error( "No default value" );
	}
	return rdoRuntime::RDOValue( dv->getIntValue() );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSIntValue( const int val ) const 
{
	if ( diap->isExist() ) {
		if ((val < diap->minVal) || (val > diap->maxVal)) {
			parser->error( rdo::format("Целочисленное значение выходит за допустимый диапазон [%d..%d]: %d", diap->minVal, diap->maxVal, val) );
//			parser->error( ("integer value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str() );
		}
	}
	return rdoRuntime::RDOValue( val );
}

int RDORTPIntParamType::getDiapTableFunc() const 
{
	if ( !diap->isExist() ) {
		parser->error( "Для параметра табличной функции должен быть задан допустимый диапазон" );
//		parser->error("integer table function parameter must have range");
	}

	if ( diap->minVal != 1 ) {
		parser->error( "Минимальное значение диапазона должно быть 1" );
//		parser->error("integer table function parameter must have minimum value = 1");
	}

	return diap->maxVal - diap->minVal + 1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - даипазон, например, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
RDORTPRealDiap::RDORTPRealDiap():
	RDOParserObject( parser ),
	RDOParserSrcInfo(),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPRealDiap::RDORTPRealDiap( const RDORTPRealDiap& diap ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( diap.src_info() ),
	exist( diap.exist ),
	minVal( diap.minVal ),
	maxVal( diap.maxVal )
{
}

RDORTPRealDiap::RDORTPRealDiap( const RDOParserSrcInfo& _src_info ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( false ),
	minVal( 0 ),
	maxVal( 0 )
{
}

RDORTPRealDiap::RDORTPRealDiap( double _minVal, double _maxVal, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos ):
	RDOParserObject( parser ),
	RDOParserSrcInfo( _src_info ),
	exist( true ),
	minVal( _minVal ),
	maxVal( _maxVal )
{
	if ( minVal > maxVal ) {
		parser->lexer_loc_set( _max_value_pos.last_line, _max_value_pos.last_column );
		parser->error( "Левая граница диапазона должна быть меньше правой" );
	}
	setSrcText( rdo::format("[%f..%f]", minVal, maxVal) );
}

void RDORTPRealDiap::check( const RDORTPRealDefVal* dv ) const
{
	if ( isExist() && dv->isExist() ) {
		if ((minVal > dv->getRealValue()) || (maxVal < dv->getRealValue())) {
			parser->lexer_loc_set( dv->src_pos().last_line, dv->src_pos().last_pos );
			parser->error( rdo::format("Вещественное значение выходит за допустимый диапазон [%f..%f]: %f", minVal, maxVal, dv->getRealValue()) );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
RDORTPRealParamType::RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPRealDefVal(0) ),
	diap( new RDORTPRealDiap() )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	diap( _diap )
{
	init();
}

void RDORTPRealParamType::init()
{
	diap->reparent( this );
	dv->reparent( this );
	std::string src_text = "real";
	if ( diap->isExist() ) {
		src_text += " " + diap->src_text();
	}
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	diap->check( static_cast<RDORTPRealDefVal*>(dv) );
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( *static_cast<RDORTPRealDefVal*>(dv) );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent, _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->lexer_loc_set( defVal_info.src_pos().last_line, defVal_info.src_pos().last_pos );
	parser->error( rdo::format("Неверное значение по-умолчанию для параметра вещественного типа: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPRealParamType::writeModelStructure() const
{
	parser->modelStructure << "R" << std::endl;
	return 0;
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSEnumValue( const std::string& val ) const
{
	parser->error( rdo::format("Ожидается вещественное число: %s", val.c_str()) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSDefaultValue() const 
{
	if ( !dv->isExist() ) {
		parser->error( "Нет значения по-умолчанию" );
	}
	return rdoRuntime::RDOValue( dv->getRealValue() );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSRealValue( const double* const val ) const 
{
	if ( diap->isExist() ) {
		if ((*val < diap->minVal) || (*val > diap->maxVal)) {
			parser->error( rdo::format("Вещественное значение выходит за допустимый диапазон [%f..%f]: %f", diap->minVal, diap->maxVal, *val) );
//			parser->error(("real value " + toString(*val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
		}
	}
	return rdoRuntime::RDOValue( *val );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSIntValue( const int val ) const
{
	if ( diap->isExist() ) {
		if ((val < diap->minVal) || (val > diap->maxVal)) {
			parser->error( rdo::format("Вещественное значение выходит за допустимый диапазон [%f..%f]: %d", diap->minVal, diap->maxVal, val) );
		}
	}
	return rdoRuntime::RDOValue( val );
}

int RDORTPRealParamType::getDiapTableFunc() const
{
	parser->error( "Параметр табличной функции может быть только целого типа" );
//	parser->error( "unexpected real table function parameter" );
	return 0;		// unreachable code...
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
void RDORTPEnum::add( const std::string& next, const YYLTYPE& _pos )
{
	if ( std::find( enumVals.begin(), enumVals.end(), next ) != enumVals.end() ) {
		parser->lexer_loc_set( _pos.last_line, _pos.last_column );
		parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_ENUM_VALUE, next.c_str() );
	}
	enumVals.push_back( next );
}

int RDORTPEnum::findValue( const std::string& val, bool auto_error ) const
{
	std::vector< std::string >::const_iterator it = std::find( enumVals.begin(), enumVals.end(), val );
	if ( it == enumVals.end() ) {
		if ( auto_error ) {
			parser->error( rdoSimulator::RDOSyntaxError::RTP_WRONG_ENUM_PARAM_VALUE, val.c_str() );
		} else {
			return -1;
		}
	}

//	return (it - enumVals.begin()) + 1;
	return it - enumVals.begin();
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
RDORTPEnumParamType::RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPEnumDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	enu( _enu ),
	enum_name( "" ),
	enum_fun( false )
{
	enu->reparent( this );
	init_src_info();
	parser->insertEnum( this );
}

void RDORTPEnumParamType::init_src_info()
{
	std::string src_text = enu->src_text();
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	setSrcPos( enu->getPosAsYY(), dv->getPosAsYY() );
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( *static_cast<RDORTPEnumDefVal*>(dv) );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( parent, _enu, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->lexer_loc_set( defVal_info.src_pos().last_line, defVal_info.src_pos().last_pos );
	parser->error( rdo::format("Неверное значение по-умолчанию для параметра перечислимого типа: %d", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser->lexer_loc_set( defVal_info.src_pos().last_line, defVal_info.src_pos().last_pos );
	parser->error( rdo::format("Неверное значение по-умолчанию для параметра перечислимого типа: %f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const 
{
	parser->lexer_loc_backup();
	parser->lexer_loc_set( defVal_info.src_pos().last_line, defVal_info.src_pos().last_pos );
	enu->findValue( defVal );	 // if no value - Syntax exception will be thrown
	parser->lexer_loc_restore();
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( defVal );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( parent, _enu, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

int RDORTPEnumParamType::writeModelStructure() const
{
	parser->modelStructure << "E " << enu->enumVals.size() << std::endl;
	for(int i = 0; i < enu->enumVals.size(); i++)
		parser->modelStructure << "    " << i << " " << enu->enumVals.at(i) << std::endl;
	return 0;
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSIntValue( const int val ) const 
{
	parser->error( rdo::format("Ожидается перечислимый тип: %d", val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSRealValue( const double* const val ) const
{
	parser->error( rdo::format("Ожидается перечислимый тип: %f", *val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSDefaultValue() const 
{
	if ( !dv->isExist() ) {
		parser->error( "Нет значения по-умолчанию" );
	}
	return rdoRuntime::RDOValue( enu->findValue( dv->getEnumValue() ) );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSEnumValue( const std::string& val ) const
{
	return enu->findValue( val );
}

int RDORTPEnumParamType::getDiapTableFunc() const 
{
	return enu->enumVals.size();
}

} // namespace rdoParse
