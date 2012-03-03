#include "pch.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdoparser.h"
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
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
void RDORTPParamType::checkParamType( const RDOFUNArithm* const action ) const
{
	switch ( typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			if ( action->typeID() == rdoRuntime::RDOType::t_real )
			{
				parser()->warning( action->src_info(), "Перевод вещественного числа в целое, возможна потеря данных" );
			}
			else if ( action->typeID() != rdoRuntime::RDOType::t_int )
			{
				parser()->error( action->src_info(), "Несоответствие типов. Ожидается целочисленное значение" );
			}
			rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(action->calc());
			if ( calc_const )
			{
				rdoRuntime::RDOValue value = calc_const->calcValue( parser()->runtime() );
				checkValue( RDOValue(value, action->src_info()) );
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			if ( action->typeID() != rdoRuntime::RDOType::t_real && action->typeID() != rdoRuntime::RDOType::t_int )
			{
				parser()->error( action->src_info(), "Несоответствие типов. Ожидается вещественное значение" );
			}
			else
			{
				rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(action->calc());
				if ( calc_const )
				{
					rdoRuntime::RDOValue value = calc_const->calcValue( parser()->runtime() );
					checkValue( RDOValue(value, action->src_info()) );
				}
			}
			break;
		}
		case rdoRuntime::RDOType::t_bool:
		{
			if ( action->typeID() != rdoRuntime::RDOType::t_bool )
			{
				parser()->error( action->src_info(), "Несоответствие типов. Ожидается булевское значение" );
			}
			break;
		}
		case rdoRuntime::RDOType::t_enum:
		{
			if ( action->typeID() == rdoRuntime::RDOType::t_identificator )
			{
				if ( static_cast<const RDORTPEnumParamType*>(this)->m_enum->getEnums().findEnum( action->value()->getAsString() ) == rdoRuntime::RDOEnumType::END )
				{
					if ( static_cast<const RDORTPEnumParamType*>(this)->enum_fun )
					{
						parser()->error( action->src_info(), rdo::format("Значение '%s' не может являться результатом функции: %s", action->value()->getAsString().c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
					else
					{
						parser()->error( action->src_info(), rdo::format("Значение '%s' не является элементом перечислимого параметра: %s", action->value()->getAsString().c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
				}
			}
			else if ( action->typeID() != rdoRuntime::RDOType::t_enum )
			{
				parser()->error_push_only( action->src_info(), rdo::format("Несоответствие типов. Ожидается перечислимый тип: %s", src_text().c_str()) );
				parser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_info(), rdo::format("Возможные значения: %s", static_cast<const RDORTPEnumParamType*>(this)->m_enum->getEnums().asString().c_str()) );
				parser()->error_push_done();
			}
			else if ( &action->enumType() != static_cast<const RDORTPEnumParamType*>(this)->m_enum )
			{
				if ( action->enumType() == *static_cast<const RDORTPEnumParamType*>(this)->m_enum )
				{
					parser()->error_push_only( action->src_info(), "Используются различные перечислимые типы с одинаковыми значениями" );
					parser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_info(), static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_text() );
					parser()->error_push_only( action->src_info(), "и" );
					parser()->error_push_only( action->enumType().src_info(), action->enumType().src_text() );
					parser()->error_push_only( action->src_info(), "Возможно, удобнее использовать первый из них как перечислимый, а второй как such_as на него, тогда параметры можно будет сравнивать и присваивать" );
				}
				else
				{
					parser()->error_push_only( action->src_info(), "Несоответствие перечислимых типов" );
					parser()->error_push_only( action->enumType().src_info(), action->enumType().src_text() );
					parser()->error_push_only( action->src_info(), "и" );
					parser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_info(), static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_text() );
				}
				parser()->error_push_done();
			}
			break;
		}
		case rdoRuntime::RDOType::t_string:
		{
			if ( action->typeID() != rdoRuntime::RDOType::t_string )
			{
				parser()->error( action->src_info(), "Несоответствие типов. Ожидается строка" );
			}
			break;
		}
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
}

rdoRuntime::RDOValue RDORTPParamType::getDefaultValue( const RDOValue& value ) const 
{
	if ( !m_dv->isExist() )
	{
		parser()->error_push_only( value.src_info(), "Нет значения по-умолчанию" );
		parser()->error_push_only( src_info(), "См. описание параметра" );
		parser()->error_push_done();
	}
	return m_dv->value().value();
}

unsigned int RDORTPParamType::getDiapTableFunc() const
{
	parser()->error( src_info(), "Параметр табличной функции может быть целого или перечислимого типа" );
	return 0;		// unreachable code...
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

void RDORTPParam::writeModelStructure( std::ostream& stream ) const
{
	stream << name() << " ";
	getType()->writeModelStructure( stream );
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

RDORTPResType::~RDORTPResType()
{
}

void RDORTPResType::addParam( const RDORTPParam* const param )
{
	if ( findRTPParam( param->name() ) )
	{
		parser()->error( param->src_info(), rdo::format("Параметр уже существует: %s", param->name().c_str()) );
	}
	m_params.push_back( param );
}

void RDORTPResType::addParam( const std::string param_name, rdoRuntime::RDOType::ID param_typeID )
{
}

const RDORTPParam* RDORTPResType::findRTPParam( const std::string& param ) const
{
	std::vector< const RDORTPParam* >::const_iterator it = std::find_if( m_params.begin(), m_params.end(), compareName<RDORTPParam>(param) );
	return it != m_params.end() ? *it : NULL;
}

unsigned int RDORTPResType::getRTPParamNumber( const std::string& param ) const
{
	std::vector< const RDORTPParam* >::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(param) );
	return it != m_params.end() ? it - m_params.begin() : UNDEFINED_PARAM;
}

void RDORTPResType::writeModelStructure( std::ostream& stream ) const
{
	stream << getNumber() << " " << name() << " " << getParams().size() << std::endl;
	for ( unsigned int i = 0; i < getParams().size(); i++ )
	{
		stream << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure( stream );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
RDORTPIntParamType::RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPDefVal(_parent->parser()) ),
	m_diap( new RDORTPIntDiap(_parent->parser()) )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
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
		src_text += " = " + m_dv->value().src_text();
	}
	setSrcText( src_text );
	if ( m_dv->isExist() )
	{
		checkValue( m_dv->value() );
	}
}

RDORTPParamType* RDORTPIntParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_int )
	{
		parser()->error( defValue.src_info(), rdo::format("Неверное значение по-умолчанию для параметра целого типа: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPIntDiap*      diap = new RDORTPIntDiap( *m_diap );
	RDORTPIntParamType* type = new RDORTPIntParamType( parent(), diap, dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPIntParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "I" << std::endl;
}

void RDORTPIntParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_int )
	{
		if ( m_diap->isExist() )
		{
			if ( value.value().getInt() < m_diap->getMin() || value.value().getInt() > m_diap->getMax() )
			{
				if ( value.src_filetype() == m_diap->src_filetype() && value.src_pos().m_last_line == m_diap->src_pos().m_last_line )
				{
					parser()->error( value.src_info(), rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_diap->getMin(), m_diap->getMax(), value.value().getInt()) );
				}
				else
				{
					parser()->error_push_only( value.src_info(), rdo::format("Значение выходит за допустимый диапазон [%d..%d]: %d", m_diap->getMin(), m_diap->getMax(), value.value().getInt()) );
					parser()->error_push_only( m_diap->src_info(), rdo::format("См. описание диапазона") );
					parser()->error_push_done();
				}
			}
		}
	}
	else
	{
		parser()->error( value.src_info(), rdo::format("Ожидается целое число, найдено '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPIntParamType::getValue( const RDOValue& value ) const
{
	checkValue( value );
	return rdoRuntime::RDOValue( value.value().getInt() );
}

unsigned int RDORTPIntParamType::getDiapTableFunc() const 
{
	if ( !m_diap->isExist() ) {
		parser()->error( src_info(), "Для параметра табличной функции должен быть задан допустимый диапазон" );
	}
	if ( m_diap->getMin() != 1 ) {
		parser()->error( src_info(), rdo::format("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]", m_diap->getMin(), m_diap->getMax()) );
	}
	return m_diap->getMax() - m_diap->getMin() + 1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
RDORTPRealParamType::RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPDefVal(_parent->parser()) ),
	m_diap( new RDORTPRealDiap(_parent->parser()) )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
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
	if ( m_diap->isExist() )
	{
		src_text += " " + m_diap->src_text();
	}
	if ( m_dv->isExist() )
	{
		src_text += " = " + m_dv->value().src_text();
	}
	setSrcText( src_text );
	if ( m_dv->isExist() )
	{
		checkValue( m_dv->value() );
	}
}

RDORTPParamType* RDORTPRealParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_int && defValue.value().typeID() != rdoRuntime::RDOType::t_real)
	{
		parser()->error( defValue.src_info(), rdo::format("Неверное значение по-умолчанию для параметра вещественного типа: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPRealDiap*      diap = new RDORTPRealDiap( *m_diap );
	RDORTPRealParamType* type = new RDORTPRealParamType( parent(), diap, dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPRealParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "R" << std::endl;
}

void RDORTPRealParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_real || value.value().typeID() == rdoRuntime::RDOType::t_int )
	{
		if ( m_diap->isExist() )
		{
			if ( value.value().getDouble() < m_diap->getMin() || value.value().getDouble() > m_diap->getMax() )
			{
				if ( value.src_filetype() == m_diap->src_filetype() && value.src_pos().m_last_line == m_diap->src_pos().m_last_line )
				{
					parser()->error( value.src_info(), rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_diap->getMin(), m_diap->getMax(), value.value().getDouble()) );
				}
				else
				{
					parser()->error_push_only( value.src_info(), rdo::format("Значение выходит за допустимый диапазон [%f..%f]: %f", m_diap->getMin(), m_diap->getMax(), value.value().getDouble()) );
					parser()->error_push_only( m_diap->src_info(), rdo::format("См. описание диапазона") );
					parser()->error_push_done();
				}
			}
		}
	}
	else
	{
		parser()->error( value.src_info(), rdo::format("Ожидается вещественное число, найдено '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPRealParamType::getValue( const RDOValue& value ) const
{
	checkValue( value );
	return rdoRuntime::RDOValue( value.value().getDouble() );
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
RDORTPEnumParamType::RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	m_enum( _enu ),
	enum_name( "" ),
	enum_fun( false )
{
	m_enum->reparent( this );
	m_dv->reparent( this );
	init_src_info();
	parser()->insertRTPEnumParamType( this );
}

void RDORTPEnumParamType::init_src_info()
{
	std::string src_text = m_enum->src_text();
	if ( m_dv->isExist() )
	{
		src_text += " = " + m_dv->value().src_text();
		setSrcPos( m_enum->getPosAsYY(), m_dv->value().getPosAsYY() );
	}
	else
	{
		setSrcPos( m_enum->getPosAsYY() );
	}
	setSrcText( src_text );
}

RDORTPParamType* RDORTPEnumParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_enum && defValue.value().typeID() != rdoRuntime::RDOType::t_identificator)
	{
		parser()->error( defValue.src_info(), rdo::format("Неверное значение по-умолчанию для параметра перечислимого типа: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		m_enum->findEnumValueWithThrow( such_as_src_info, defValue.value().getAsString() );
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcFileType( such_as_src_info.src_filetype() );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPEnum*          enu  = m_enum;
	RDORTPEnumParamType* type = new RDORTPEnumParamType( parent(), enu, dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPEnumParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "E " << m_enum->getEnums().getValues().size() << std::endl;
	for ( unsigned int i = 0; i < m_enum->getEnums().getValues().size(); i++ )
	{
		stream << "    " << i << " " << m_enum->getEnums().getValues().at(i) << std::endl;
	}
}

void RDORTPEnumParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_enum || value.value().typeID() == rdoRuntime::RDOType::t_identificator )
	{
		m_enum->findEnumValueWithThrow( value.src_info(), value.value().getAsString() );
	}
	else
	{
		parser()->error_push_only( value.src_info(), rdo::format("Ожидается перечислимый тип, найдено '%s'", value.value().getAsString().c_str()) );
		if ( src_pos().m_last_line != value.src_pos().m_last_line )
		{
			parser()->error_push_only( src_info(), rdo::format("См. перечисление: %s", src_text().c_str()) );
		}
		parser()->error_push_done();
	}
}

rdoRuntime::RDOValue RDORTPEnumParamType::getValue( const RDOValue& value ) const
{
	checkValue( value );
	return m_enum->findEnumValueWithThrow( value.src_info(), value.value().getAsString() );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getDefaultValue( const RDOValue& value ) const 
{
	rdoRuntime::RDOValue identificator = RDORTPParamType::getDefaultValue( value );
	return m_enum->findEnumValueWithThrow( value, identificator.getAsString() );
}

unsigned int RDORTPEnumParamType::getDiapTableFunc() const 
{
	return m_enum->getEnums().getValues().size();
}

// ----------------------------------------------------------------------------
// ---------- RDORTPStringParamType
// ----------------------------------------------------------------------------
RDORTPStringParamType::RDORTPStringParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info )
{
}

RDORTPParamType* RDORTPStringParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_string)
	{
		parser()->error( defValue.src_info(), rdo::format("Неверное значение по-умолчанию для параметра строкового типа: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPStringParamType* type = new RDORTPStringParamType( parent(), dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPStringParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_string )
	{
	}
	else
	{
		parser()->error( value.src_info(), rdo::format("Ожидается строка, найдено '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPStringParamType::getValue( const RDOValue& value ) const
{
	checkValue( value );
	return rdoRuntime::RDOValue( value.value().getString() );
}

void RDORTPStringParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "S" << std::endl;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPBoolParamType
// ----------------------------------------------------------------------------
RDORTPBoolParamType::RDORTPBoolParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info )
{
}

RDORTPParamType* RDORTPBoolParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_bool)
	{
		parser()->error( defValue.src_info(), rdo::format("Неверное значение по-умолчанию для параметра булевского типа: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPBoolParamType* type = new RDORTPBoolParamType( parent(), dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPBoolParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_bool )
	{
	}
	else
	{
		parser()->error( value.src_info(), rdo::format("Ожидается булевское значение, найдено '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPBoolParamType::getValue( const RDOValue& value ) const
{
	checkValue( value );
	return rdoRuntime::RDOValue( value.value().getBool() );
}

void RDORTPBoolParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "B" << std::endl;
}

/*
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности нечеткого терма
// ----------------------------------------------------------------------------
RDORTPFuzzyMembershiftFun::RDORTPFuzzyMembershiftFun( RDOParser* _parser ):
	RDOParserObject( _parser )
{
/*	for ( unsigned int i = 0; i < m_points.size(); i++ )
	{
//		double x = m_points[i]->getX();
	}

	Items::iterator it = m_points.begin();
	while ( it != m_points.end() )
	{
		double x = (*it)->getX();
		it++;
	}
}
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTerm - нечеткий термин
// ----------------------------------------------------------------------------
RDORTPFuzzyTerm::RDORTPFuzzyTerm( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyMembershiftFun* membersfift_fun):
	RDOParserObject( _parser )
{

}*/

} // namespace rdoParse
