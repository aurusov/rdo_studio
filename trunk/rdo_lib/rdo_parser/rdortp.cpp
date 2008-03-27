#include "pch.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdoDefines.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

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
RDORTPDefVal::RDORTPDefVal( RDOParser* _parser, const RDORTPDefVal& dv ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( dv.src_info() ),
	exist( dv.exist )
{
}

RDORTPDefVal::RDORTPDefVal( RDOParser* _parser, bool _exist ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	exist( _exist )
{
}

RDORTPDefVal::RDORTPDefVal( RDOParser* _parser, bool _exist, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	exist( _exist )
{
}

int RDORTPDefVal::getIntValue() const
{
	parser()->error( src_info(), "Неверное значение по-умолчанию" );
//	parser()->error( "Invalid default value" );
	return 0;	// unreachable code...
}

double RDORTPDefVal::getRealValue() const
{
	parser()->error( src_info(), "Неверное значение по-умолчанию" );
	return 0;	// unreachable code...
}

const std::string& RDORTPDefVal::getEnumValue() const
{
	parser()->error( src_info(), "Неверное значение по-умолчанию" );
#pragma warning( disable: 4172 )
	return "";	// unreachable code...
#pragma warning( default: 4172 )
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
void RDORTPParamType::checkParamType( const RDOFUNArithm* const action, bool warning ) const
{
	switch ( getType() ) {
		case rdoRuntime::RDOValue::Type::rvt_int: {
			if ( action->getType() == rdoRuntime::RDOValue::Type::rvt_real ) {
//				if ( warning ) {
					parser()->warning( action->src_info(), "Перевод вещественного числа в целое, возможна потеря данных" );
//				} else {
//					parser()->error( action->src_info(), "Ожидается целое число" );
//				}
			} else if ( action->getType() != rdoRuntime::RDOValue::Type::rvt_int ) {
				parser()->error( action->src_info(), "Несоответствие типов. Ожидается целочисленное значение" );
			}
			rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(action->getCalc());
			if ( calc_const ) {
				rdoRuntime::RDOValue value = calc_const->calcValueBase( parser()->runtime() );
				checkRSSIntValue( value.getInt(), action->src_info() );
			}
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_real: {
			if ( action->getType() != rdoRuntime::RDOValue::Type::rvt_real && action->getType() != rdoRuntime::RDOValue::Type::rvt_int ) {
				parser()->error( action->src_info(), "Несоответствие типов. Ожидается вещественное значение" );
			} else {
				rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(action->getCalc());
				if ( calc_const ) {
					rdoRuntime::RDOValue value = calc_const->calcValueBase( parser()->runtime() );
					checkRSSRealValue( value.getDouble(), action->src_info() );
				}
			}
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_enum: {
			if ( action->getType() == rdoRuntime::RDOValue::Type::rvt_unknow ) {
				if ( static_cast<const RDORTPEnumParamType*>(this)->enu->getEnums().findEnum( action->str ) == -1 ) {
					if ( static_cast<const RDORTPEnumParamType*>(this)->enum_fun ) {
						parser()->error( action->src_info(), rdo::format("Значение '%s' не может являться результатом функции: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					} else {
						parser()->error( action->src_info(), rdo::format("Значение '%s' не является элементом перечислимого параметра: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
				}
			} else if ( action->getType() != rdoRuntime::RDOValue::Type::rvt_enum ) {
				parser()->error_push_only( action->src_info(), rdo::format("Несоответствие типов. Ожидается перечислимый тип: %s", src_text().c_str()) );
				parser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), rdo::format("Возможные значения: %s", static_cast<const RDORTPEnumParamType*>(this)->enu->src_text().c_str()) );
				parser()->error_push_done();
			} else if ( action->enu != static_cast<const RDORTPEnumParamType*>(this)->enu ) {
				if ( &action->enu->getEnums() == &static_cast<const RDORTPEnumParamType*>(this)->enu->getEnums() ) {
					parser()->error_push_only( action->src_info(), "Используются различные перечислимые типы с одинаковыми значениями" );
					parser()->error_push_only( action->enu->src_info(), action->enu->src_text() );
					parser()->error_push_only( action->src_info(), "и" );
					parser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), static_cast<const RDORTPEnumParamType*>(this)->enu->src_text() );
					parser()->error_push_only( action->src_info(), "Возможно, удобнее использовать первый из них как перечислимый, а второй как such_as на него, тогда параметры можно будет сравнивать и присваивать" );
				} else {
					parser()->error_push_only( action->src_info(), "Несоответствие перечислимых типов" );
					parser()->error_push_only( action->enu->src_info(), action->enu->src_text() );
					parser()->error_push_only( action->src_info(), "и" );
					parser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), static_cast<const RDORTPEnumParamType*>(this)->enu->src_text() );
				}
				parser()->error_push_done();
			}
			break;
		}
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
}

void RDORTPParamType::checkParamType( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& value_info ) const
{
	switch ( getType() ) {
		case rdoRuntime::RDOValue::Type::rvt_int: {
			checkRSSIntValue( value.getInt(), value_info );
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_real: {
			checkRSSRealValue( value.getDouble(), value_info );
			break;
		}
		case rdoRuntime::RDOValue::Type::rvt_enum: {
			if ( static_cast<const RDORTPEnumParamType*>(this)->enu->getEnums().findEnum( value_info.src_text() ) == -1 ) {
				parser()->error( value_info, rdo::format("Значение '%s' не является элементом перечислимого параметра: %s", value_info.src_text().c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
			}
			break;
		}
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam( RDORTPResType* _parent, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	m_parType( _parType ),
	m_resType( _parent )
{
}

RDORTPParam::RDORTPParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_parType( _parType ),
	m_resType( NULL )
{
}

int RDORTPParam::writeModelStructure() const
{
	parser()->modelStructure << getName() << " ";
	getType()->writeModelStructure();
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConst
// ----------------------------------------------------------------------------
RDOFUNConst::RDOFUNConst( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType ):
	RDORTPParam( _parser, _src_info, _parType )
{
}

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
RDORTPResType::RDORTPResType( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const bool _permanent ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_number( _parser->getRTP_id() ),
	m_permanent( _permanent )
{
	parser()->insertRTPResType( this );
}

void RDORTPResType::addParam( const RDORTPParam* const param )
{
	if ( findRTPParam( param->getName() ) ) {
		parser()->error( param->src_info(), rdoSimulator::RDOSyntaxError::RTP_SECOND_PARAM_NAME, param->getName().c_str() );
	}
	m_params.push_back( param );
}

void RDORTPResType::addParam( const std::string param_name, rdoRuntime::RDOValue::Type param_type )
{
}

const RDORTPParam* RDORTPResType::findRTPParam( const std::string& param ) const
{
	std::vector< const RDORTPParam* >::const_iterator it = std::find_if( m_params.begin(), m_params.end(), compareName<RDORTPParam>(param) );
	return it != m_params.end() ? *it : NULL;
}

int RDORTPResType::getRTPParamNumber( const std::string& param ) const
{
	std::vector< const RDORTPParam* >::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(param) );
	return it != m_params.end() ? it - m_params.begin() : -1;
}

int RDORTPResType::writeModelStructure() const
{
	parser()->modelStructure << getNumber() << " " << getName() << " " << getParams().size() << std::endl;
	for(int i = 0; i < getParams().size(); i++)
	{
		parser()->modelStructure << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure();
	}
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
RDORTPIntParamType::RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPIntDefVal(0) ),
	m_diap( new RDORTPIntDiap(_parent->parser()) )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	m_diap( _diap )
{
	init();
}

void RDORTPIntParamType::init()
{
	m_diap->reparent( this );
	m_dv->reparent( this );
	std::string src_text = "integer";
	if ( m_diap->isExist() ) {
		src_text += " " + m_diap->src_text();
	}
	if ( m_dv->isExist() ) {
		src_text += " = " + m_dv->src_text();
	}
	setSrcText( src_text );
	if ( m_dv->isExist() ) {
		getRSSIntValue( static_cast<RDORTPIntDefVal*>(m_dv)->getIntValue(), m_dv->src_info() );
	}
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *m_diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( *static_cast<RDORTPIntDefVal*>(m_dv) );
	RDORTPIntParamType* _type = new RDORTPIntParamType( parent(), _diap, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *m_diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( parser(), defVal, defVal_info );
	RDORTPIntParamType* _type = new RDORTPIntParamType( parent(), _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser()->error( defVal_info, rdo::format("Неверное значение по-умолчанию для параметра целого типа: %f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser()->error( defVal_info, rdo::format("Неверное значение по-умолчанию для параметра целого типа: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPIntParamType::writeModelStructure() const
{
	parser()->modelStructure << "I" << std::endl;
	return 0;
}

void RDORTPIntParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	parser()->error( _src_info, rdo::format("Ожидается целое число, найдено '%s'", val.c_str()) );
}

void RDORTPIntParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	if ( m_diap->isExist() ) {
		if ( val < m_diap->getMin() || val > m_diap->getMax() ) {
			if ( _src_info.src_filetype() == m_diap->src_filetype() && _src_info.src_pos().last_line == m_diap->src_pos().last_line ) {
				parser()->error( _src_info, rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_diap->getMin(), m_diap->getMax(), val) );
			} else {
				parser()->error_push_only( _src_info, rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_diap->getMin(), m_diap->getMax(), val) );
				parser()->error_push_only( m_diap->src_info(), rdo::format("См. описание диапазона") );
				parser()->error_push_done();
			}
//			parser()->error( ("integer value " + toString(val) + " out of range[" + toString(m_diap->min_value) + ", " + toString(m_diap->max_value) + "]").c_str() );
		}
	}
}

void RDORTPIntParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	parser()->error( _src_info, rdo::format("Ожидается целое число, найдено '%f'", val) );
}

rdoRuntime::RDOValue RDORTPIntParamType::getDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !m_dv->isExist() ) {
		parser()->error( _src_info, "Нет значения по-умолчанию" );
//		parser()->error( "No default value" );
	}
	return rdoRuntime::RDOValue( m_dv->getIntValue() );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSEnumValue( val, _src_info );
	return 0; // Не выполняется из-за checkRSSEnumValue
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSIntValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
	return rdoRuntime::RDOValue( (int)val ); // Не выполняется из-за checkRSSRealValue
}

int RDORTPIntParamType::getDiapTableFunc() const 
{
	if ( !m_diap->isExist() ) {
		parser()->error( src_info(), "Для параметра табличной функции должен быть задан допустимый диапазон" );
//		parser()->error("integer table function parameter must have range");
	}
	if ( m_diap->getMin() != 1 ) {
		parser()->error( src_info(), rdo::format("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]", m_diap->getMin(), m_diap->getMax()) );
//		parser()->error("integer table function parameter must have minimum value = 1");
	}
	return m_diap->getMax() - m_diap->getMin() + 1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
RDORTPRealParamType::RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPRealDefVal(0) ),
	m_diap( new RDORTPRealDiap(_parent->parser()) )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	m_diap( _diap )
{
	init();
}

void RDORTPRealParamType::init()
{
	m_diap->reparent( this );
	m_dv->reparent( this );
	std::string src_text = "real";
	if ( m_diap->isExist() ) {
		src_text += " " + m_diap->src_text();
	}
	if ( m_dv->isExist() ) {
		src_text += " = " + m_dv->src_text();
	}
	setSrcText( src_text );
	if ( m_dv->isExist() ) {
		getRSSRealValue( static_cast<RDORTPRealDefVal*>(m_dv)->getRealValue(), m_dv->src_info() );
	}
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *m_diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( *static_cast<RDORTPRealDefVal*>(m_dv) );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent(), _diap, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *m_diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( parser(), defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent(), _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *m_diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( parser(), defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( parent(), _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	parser()->error( defVal_info, rdo::format("Неверное значение по-умолчанию для параметра вещественного типа: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPRealParamType::writeModelStructure() const
{
	parser()->modelStructure << "R" << std::endl;
	return 0;
}

void RDORTPRealParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	parser()->error( _src_info, rdo::format("Ожидается вещественное число, найдено '%s'", val.c_str()) );
}

void RDORTPRealParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
}

void RDORTPRealParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	if ( m_diap->isExist() ) {
		if ( val < m_diap->getMin() || val > m_diap->getMax() ) {
			if ( _src_info.src_filetype() == m_diap->src_filetype() && _src_info.src_pos().last_line == m_diap->src_pos().last_line ) {
				parser()->error( _src_info, rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_diap->getMin(), m_diap->getMax(), val) );
			} else {
				parser()->error_push_only( _src_info, rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_diap->getMin(), m_diap->getMax(), val) );
				parser()->error_push_only( m_diap->src_info(), rdo::format("См. описание диапазона") );
				parser()->error_push_done();
			}
//			parser()->error(("real value " + toString(*val) + " out of range[" + toString(m_diap->min_value) + ", " + toString(m_diap->max_value) + "]").c_str());
		}
	}
}

rdoRuntime::RDOValue RDORTPRealParamType::getDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !m_dv->isExist() ) {
		parser()->error( _src_info, "Нет значения по-умолчанию" );
	}
	return rdoRuntime::RDOValue( m_dv->getRealValue() );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSEnumValue( val, _src_info );
	return 0; // Не выполняется из-за checkRSSEnumValue
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSIntValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSRealValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

int RDORTPRealParamType::getDiapTableFunc() const
{
	parser()->error( src_info(), "Параметр табличной функции может быть целого или перечислимого типа" );
//	parser()->error( "unexpected real table function parameter" );
	return 0;		// unreachable code...
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum( const RDOParserObject* _parent, const std::string& first ):
	RDOParserObject( _parent ),
	m_enums( NULL )
{
	m_enums = new rdoRuntime::RDOEnum( parser()->runtime() );
	m_enums->add( first );
}

RDORTPEnum::~RDORTPEnum()
{
}

void RDORTPEnum::add( const RDOParserSrcInfo& next )
{
	if ( m_enums->findEnum( next.src_text() ) != -1 ) {
		parser()->error( next, rdoSimulator::RDOSyntaxError::RTP_SECOND_ENUM_VALUE, next.src_text().c_str() );
	}
	m_enums->add( next.src_text() );
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const
{
	int result = m_enums->findEnum( val );
	if ( result == -1 ) {
		parser()->error_push_only( val_src_info, rdoSimulator::RDOSyntaxError::RTP_WRONG_ENUM_PARAM_VALUE, val.c_str() );
		parser()->error_push_only( src_info(), rdo::format("Возможные значения: %s", src_text().c_str()) );
		parser()->error_push_done();
	}
	return rdoRuntime::RDOValue( *m_enums, val );
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if ( m_enums->getValues().empty() ) {
		parser()->error( src_info(), "Внутренняя ошибка: Пустой перечислимый тип" );
	}
	return rdoRuntime::RDOValue( *m_enums );
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
	m_dv->reparent( this );
	init_src_info();
	parser()->insertEnum( this );
}

void RDORTPEnumParamType::init_src_info()
{
	std::string src_text = enu->src_text();
	if ( m_dv->isExist() ) {
		src_text += " = " + m_dv->src_text();
	}
	setSrcText( src_text );
	setSrcPos( enu->getPosAsYY(), m_dv->getPosAsYY() );
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( *static_cast<RDORTPEnumDefVal*>(m_dv) );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( parent(), _enu, _dv, _src_info );
	_dv->setSrcFileType( _src_info.src_filetype() );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	enu->findEnumValueWithThrow( defVal_info, rdo::format("%d", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	enu->findEnumValueWithThrow( defVal_info, rdo::format("%f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const 
{
	enu->findEnumValueWithThrow( defVal_info, defVal );
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( parser(), defVal );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( parent(), _enu, _dv, _src_info );
	_dv->setSrcInfo( defVal_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

int RDORTPEnumParamType::writeModelStructure() const
{
	parser()->modelStructure << "E " << enu->getEnums().getValues().size() << std::endl;
	for(int i = 0; i < enu->getEnums().getValues().size(); i++)
		parser()->modelStructure << "    " << i << " " << enu->getEnums().getValues().at(i) << std::endl;
	return 0;
}

void RDORTPEnumParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	enu->findEnumValueWithThrow( _src_info, val );
}

void RDORTPEnumParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	parser()->error_push_only( _src_info, rdo::format("Ожидается перечислимый тип, найдено '%d'", val) );
	if ( src_info().src_pos().last_line != _src_info.src_pos().last_line ) {
		parser()->error_push_only( src_info(), rdo::format("См. перечисление: %s", src_text().c_str()) );
	}
	parser()->error_push_done();
}

void RDORTPEnumParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	parser()->error( _src_info, rdo::format("Ожидается перечислимый тип, найдено '%f'", val) );
	if ( src_info().src_pos().last_line != _src_info.src_pos().last_line ) {
		parser()->error_push_only( src_info(), rdo::format("См. перечисление: %s", src_text().c_str()) );
	}
	parser()->error_push_done();
}

rdoRuntime::RDOValue RDORTPEnumParamType::getDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !m_dv->isExist() ) {
		parser()->error( _src_info, "Нет значения по-умолчанию" );
	}
	return enu->findEnumValueWithThrow( _src_info, m_dv->getEnumValue() );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	return enu->findEnumValueWithThrow( _src_info, val );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSIntValue( val, _src_info );
	return 0; // Не выполняется из-за checkRSSIntValue
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
	return 0; // Не выполняется из-за checkRSSRealValue
}

int RDORTPEnumParamType::getDiapTableFunc() const 
{
	return enu->getEnums().getValues().size();
}

} // namespace rdoParse
