#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdortp.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdoDefines.h"
#include "rdoparser_lexer.h"

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

void RDORTPEnum::add(const std::string *const next) 
{ 
	if ( std::find_if(enumVals.begin(), enumVals.end(), comparePointers<std::string>(next)) != enumVals.end() )
		parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_ENUM_VALUE, next->c_str() );

	enumVals.push_back(next); 
}

int RDORTPEnum::findValue( const std::string* const val, bool auto_error ) const
{
	std::vector<const std::string *>::const_iterator it = 
		std::find_if(enumVals.begin(), enumVals.end(), comparePointers<std::string>(val));

	if ( it == enumVals.end() ) {
		if ( auto_error ) {
			parser->error( rdoSimulator::RDOSyntaxError::RTP_WRONG_ENUM_PARAM_VALUE, val->c_str() );
		} else {
			return -1;
		}
	}

//	return (it - enumVals.begin()) + 1;
	return it - enumVals.begin();
}

RDORTPResType::RDORTPResType( RDOParser* _parser, const std::string* const _name, const bool _permanent ):
	RDOParserObject( _parser ),
	name( _name ),
	number( parser->getRTP_id() ),
	permanent( _permanent )
{
	parser->insertRTPResType( this );
}

void RDORTPResType::addParam( const RDORTPParamDesc* const param )
{
	if ( findRTPParam( param->getName() ) ) {
		parser->lexer_loc_restore();
		parser->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_PARAM_NAME, param->getName()->c_str() );
	}
	params.push_back( param );
}

const RDORTPParamDesc* RDORTPResType::findRTPParam( const std::string* const param ) const
{
	std::vector<const RDORTPParamDesc *>::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDORTPParamDesc>(param) );
	return it != params.end() ? *it : NULL;
}

int RDORTPResType::getRTPParamNumber( const std::string* const param ) const
{
	std::vector<const RDORTPParamDesc *>::const_iterator it = std::find_if(params.begin(), 
		params.end(), 
		compareName<RDORTPParamDesc>(param));
	if(it != params.end())
		return it - params.begin();

	return -1;
}

int RDORTPResType::writeModelStructure() const
{
	parser->modelStructure << getNumber() << " " << *getName() << " " << getParams().size() << std::endl;
	for(int i = 0; i < getParams().size(); i++)
	{
		parser->modelStructure << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure();
	}
	return 0;
}

int RDORTPParamDesc::writeModelStructure() const
{
	parser->modelStructure << *getName() << " ";
	getType()->writeModelStructure();
//	parser->modelStructure << " ";
	return 0;
}

int RDORTPIntResParam::writeModelStructure() const
{
	parser->modelStructure << "I" << std::endl;
	return 0;
}

int RDORTPRealResParam::writeModelStructure() const
{
	parser->modelStructure << "R" << std::endl;
	return 0;
}

int RDORTPEnumResParam::writeModelStructure() const
{
	parser->modelStructure << "E " << enu->enumVals.size() << std::endl;
	for(int i = 0; i < enu->enumVals.size(); i++)
		parser->modelStructure << "    " << i << " " << *enu->enumVals.at(i) << std::endl;
	return 0;
}

const RDORTPResParam *RDORTPResParam::constructSuchAs() const 
{ 
	return this; 
}

const RDORTPResParam* RDORTPResParam::constructSuchAs( const int defVal ) const
{
	parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_SUCHAS, defVal );
	return NULL;	// unreachable code...
}

const RDORTPResParam* RDORTPResParam::constructSuchAs( const double* const defVal ) const
{
	parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_REAL_SUCHAS, *defVal );
	return NULL;	// unreachable code...
}

const RDORTPResParam* RDORTPResParam::constructSuchAs( const std::string* const defVal ) const
{
	parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_ENUM_SUCHAS, defVal->c_str() );
	return NULL;	// unreachable code...
}

void RDORTPResParam::checkParamType( const RDOFUNArithm* const action ) const
{
	switch ( getType() ) {
		case RDORTPResParam::pt_int: {
			if ( action->getType() == RDORTPResParam::pt_real ) {
				parser->lexer_loc_backup();
				parser->lexer_loc_set( action->error().last_line, action->error().last_column );
				parser->warning( "Перевод вещественного числа в целое, возможна потеря данных" );
				parser->lexer_loc_restore();
			} else if ( action->getType() != RDORTPResParam::pt_int ) {
				parser->lexer_loc_set( action->error().last_line, action->error().last_column );
				parser->error( "Несоответствие типов. Ожидается целое число" );
			}
			break;
		}
		case RDORTPResParam::pt_real: {
			if ( action->getType() != RDORTPResParam::pt_real && action->getType() != RDORTPResParam::pt_int ) {
				parser->lexer_loc_set( action->error().last_line, action->error().last_column );
				parser->error( "Несоответствие типов. Ожидается вещественное число" );
			}
			break;
		}
		case RDORTPResParam::pt_enum: {
			if ( action->getType() == RDORTPResParam::pt_str ) {
				if ( static_cast<const RDORTPEnumResParam*>(this)->enu->findValue( action->str, false ) == -1 ) {
					parser->lexer_loc_set( action->error().last_line, action->error().last_column );
					if ( static_cast<const RDORTPEnumResParam*>(this)->enum_fun ) {
						parser->error( rdo::format("Значение '%s' не может являться результатом функции: %s", action->str->c_str(), static_cast<const RDORTPEnumResParam*>(this)->enum_name.c_str()) );
					} else {
						parser->error( rdo::format("Значение '%s' не является элементом перечислимого параметра: %s", action->str->c_str(), static_cast<const RDORTPEnumResParam*>(this)->enum_name.c_str()) );
					}
				}
			} else if ( action->getType() != RDORTPResParam::pt_enum ) {
				parser->lexer_loc_set( action->error().last_line, action->error().last_column );
				parser->error( "Несоответствие типов. Ожидается перечислимый тип" );
			} else if ( action->enu != static_cast<const RDORTPEnumResParam*>(this)->enu ) {
				parser->lexer_loc_set( action->error().last_line, action->error().last_column );
				parser->error( "Несоответствие перечислимых типов" );
			}
			break;
		}
	}
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSEnumValue( const std::string* const val ) const
{
	parser->error( rdo::format("Ожидается целое число: %s", val->c_str()) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSEnumValue( const std::string* const val ) const
{
	parser->error( rdo::format("Ожидается вещественное число: %s", val->c_str()) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSIntValue( const int val ) const 
{
	parser->error( rdo::format("Ожидается перечислимый тип: %d", val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSRealValue( const double* const val ) const
{
	parser->error( rdo::format("Ожидается перечислимый тип: %f", *val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSRealValue( const double* const val ) const
{
	parser->error( rdo::format("Ожидается целое число: %f", *val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSDefaultValue() const 
{
	if ( !dv->exist ) {
		parser->error( "Нет значения по-умолчанию" );
//		parser->error( "No default value" );
	}
	return rdoRuntime::RDOValue( dv->GetIntValue() );
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSIntValue( const int val ) const 
{
	if ( diap->exist ) {
		if ((val < diap->minVal) || (val > diap->maxVal)) {
			parser->error( rdo::format("Целочисленное значение выходит за допустимый диапазон [%d..%d]: %d", diap->minVal, diap->maxVal, val) );
//			parser->error( ("integer value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str() );
		}
	}
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSDefaultValue() const 
{
	if ( !dv->exist ) {
		parser->error( "Нет значения по-умолчанию" );
	}
	return rdoRuntime::RDOValue( dv->GetRealValue() );
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSRealValue( const double* const val ) const 
{
	if ( diap->exist ) {
		if ((*val < diap->minVal) || (*val > diap->maxVal)) {
			parser->error( rdo::format("Вещественное значение выходит за допустимый диапазон [%f..%f]: %f", diap->minVal, diap->maxVal, *val) );
//			parser->error(("real value " + toString(*val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
		}
	}
	return rdoRuntime::RDOValue( *val );
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSIntValue( const int val ) const
{
	if ( diap->exist ) {
		if ((val < diap->minVal) || (val > diap->maxVal)) {
			parser->error( rdo::format("Вещественное значение выходит за допустимый диапазон [%f..%f]: %d", diap->minVal, diap->maxVal, val) );
		}
	}
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSDefaultValue() const 
{
	if ( !dv->exist ) {
		parser->error( "Нет значения по-умолчанию" );
	}
	return rdoRuntime::RDOValue( enu->findValue( dv->GetEnumValue() ) );
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSEnumValue(const std::string *const val) const
{
	return enu->findValue(val);
}

RDORTPIntResParam::RDORTPIntResParam( RDOParser* _parser, RDORTPIntDiap *_diap, RDORTPIntDefVal *_dv ):
	RDORTPResParam( _parser, _dv ),
	diap( _diap )
{
	diap->check(_dv);
}

RDORTPIntResParam::RDORTPIntResParam( const RDOParserObject* _parent ):
	RDORTPResParam( _parent, new RDORTPIntDefVal(0) ),
	diap( new RDORTPIntDiap() )
{
	diap->check( static_cast<RDORTPIntDefVal*>(dv) );
}

RDORTPIntResParam::RDORTPIntResParam( const RDOParserObject* _parent, RDORTPIntDiap *_diap, RDORTPIntDefVal *_dv ):
	RDORTPResParam( _parent, _dv ),
	diap( _diap )
{
	diap->check(_dv);
}

RDORTPIntDiap::RDORTPIntDiap( int _minVal, int _maxVal ):
	RDODeletable(),
	minVal( _minVal ),
	maxVal( _maxVal ),
	exist( true )
{
	if ( minVal > maxVal ) {
		parser->error( "Левая граница диапазона должна быть меньше правой" );
	}
}

void RDORTPIntDiap::check( const RDORTPIntDefVal* dv ) const
{
	if ( exist && dv->exist ) {
		if ((minVal > dv->GetIntValue()) || (maxVal < dv->GetIntValue())) {
			parser->error( rdo::format("Целочисленное значение выходит за допустимый диапазон [%d..%d]: %d", minVal, maxVal, dv->GetIntValue()) );
		}
	}
}

RDORTPRealResParam::RDORTPRealResParam( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPResParam( _parser, _dv ),
	diap( _diap )
{
	diap->check( _dv );
}

RDORTPRealResParam::RDORTPRealResParam( const RDOParserObject* _parent ):
	RDORTPResParam( _parent, new RDORTPRealDefVal(0) ),
	diap( new RDORTPRealDiap() )
{
	diap->check( static_cast<RDORTPRealDefVal*>(dv) );
}

RDORTPRealResParam::RDORTPRealResParam( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPResParam( _parent, _dv ),
	diap( _diap )
{
	diap->check( _dv );
}

RDORTPRealDiap::RDORTPRealDiap( double _minVal, double _maxVal ):
	minVal( _minVal ),
	maxVal( _maxVal ),
	exist( true )
{
	if ( minVal > maxVal ) {
		parser->error( "Левая граница диапазона должна быть меньше правой" );
	}
}

void RDORTPRealDiap::check( const RDORTPRealDefVal* dv ) const
{
	if ( exist && dv->exist ) {
		if ((minVal > dv->GetRealValue()) || (maxVal < dv->GetRealValue())) {
			parser->error( rdo::format("Вещественное значение выходит за допустимый диапазон [%f..%f]: %f", minVal, maxVal, dv->GetRealValue()) );
		}
	}
}

const RDORTPResParam* RDORTPIntResParam::constructSuchAs( const int defVal ) const
{
	RDORTPIntDefVal* newDV = new RDORTPIntDefVal( defVal );
	RDORTPIntResParam*  rp = new RDORTPIntResParam( parent, diap, newDV );
	return rp;
}

const RDORTPResParam *RDORTPRealResParam::constructSuchAs(const double *const defVal) const 
{
	RDORTPRealDefVal *newDV = new RDORTPRealDefVal( *defVal );
	RDORTPRealResParam *rp = new RDORTPRealResParam( parent, diap, newDV );
	return rp;
}

const RDORTPResParam *RDORTPEnumResParam::constructSuchAs(const std::string *const defVal) const 
{
	enu->findValue(defVal);	 // if no value - Syntax exception will be thrown
	RDORTPEnumDefVal *newDV = new RDORTPEnumDefVal(defVal);
	RDORTPEnumResParam *rp = new RDORTPEnumResParam( parent, enu, newDV );
	return rp;
}

int RDORTPDefVal::GetIntValue() const
{
	parser->error( "Неверное значение по-умолчанию" );
//	parser->error( "Invalid default value" );
	return 0;	// unreachable code...
}

double RDORTPDefVal::GetRealValue() const
{
	parser->error( "Неверное значение по-умолчанию" );
	return 0.;	// unreachable code...
}

const std::string *RDORTPDefVal::GetEnumValue() const
{
	parser->error( "Неверное значение по-умолчанию" );
	return NULL;	// unreachable code...
}

int RDORTPIntResParam::getDiapTableFunc() const 
{
	if ( !diap->exist ) {
		parser->error( "Для параметра табличной функции должен быть задан допустимый диапазон" );
//		parser->error("integer table function parameter must have range");
	}

	if ( diap->minVal != 1 ) {
		parser->error( "Минимальное значение диапазона должно быть 1" );
//		parser->error("integer table function parameter must have minimum value = 1");
	}

	return diap->maxVal - diap->minVal + 1;
}

int RDORTPRealResParam::getDiapTableFunc() const
{
	parser->error( "Параметр табличной функции может быть только целого типа" );
//	parser->error( "unexpected real table function parameter" );
	return 0;		// unreachable code...
}

int RDORTPEnumResParam::getDiapTableFunc() const 
{
	return enu->enumVals.size();
}

RDORTPParamDesc::RDORTPParamDesc( const std::string* const _name, const RDORTPResParam* const _parType ):
	name( _name ),
	parType( _parType )
{
	parser->insertRTPParam( this );
}

} // namespace rdoParse
