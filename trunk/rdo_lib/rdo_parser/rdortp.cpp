#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdortp.h"
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

int RDORTPEnum::findValue( const std::string* const val ) const
{
	std::vector<const std::string *>::const_iterator it = 
		std::find_if(enumVals.begin(), enumVals.end(), comparePointers<std::string>(val));

	if ( it == enumVals.end() ) {
		parser->error( rdoSimulator::RDOSyntaxError::RTP_WRONG_ENUM_PARAM_VALUE, val->c_str() );
	}

//	return (it - enumVals.begin()) + 1;
	return it - enumVals.begin();
}

RDORTPResType::RDORTPResType( const std::string* const _name, const bool _permanent ):
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
//	std::ostringstream str;
//	str << "Invalid default value: " << defVal;
//	parser->error(str.str().c_str());
	parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_INT_SUCHAS, defVal );
	return NULL;	// unreachable code...
}

const RDORTPResParam* RDORTPResParam::constructSuchAs( const double* const defVal ) const
{
//	std::ostringstream str;
//	str << "Invalid default value: " << *defVal;
//	parser->error(str.str().c_str());
	parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_REAL_SUCHAS, *defVal );
	return NULL;	// unreachable code...
}

const RDORTPResParam* RDORTPResParam::constructSuchAs( const std::string* const defVal ) const
{
//	parser->error(("Invalid default value: " + *defVal).c_str());
	parser->error( rdoSimulator::RDOSyntaxError::RTP_INVALID_DEFVAULT_ENUM_SUCHAS, defVal->c_str() );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSEnumValue( const std::string* const val ) const
{
	parser->error( rdo::format("Ќеверное значение параметра целого типа: %s", val->c_str()) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSEnumValue( const std::string* const val ) const
{
	parser->error( rdo::format("Ќеверное значение параметра вещественного типа: %s", val->c_str()) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSIntValue( const int val ) const 
{
	parser->error( rdo::format("Ќеверное значение параметра перечислимого типа: %d", val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSRealValue( const double* const val ) const
{
	parser->error( rdo::format("Ќеверное значение параметра перечислимого типа: %f", *val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSRealValue( const double* const val ) const
{
	parser->error( rdo::format("Ќеверное значение параметра целого типа: %f", *val) );
	return NULL;	// unreachable code...
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSDefaultValue() const 
{
	if ( !dv->exist ) {
//		parser->error( "No default value" );
		parser->error( "Ќет значени€ по-умолчанию" );
	}
	return rdoRuntime::RDOValue( dv->GetIntValue() );
}

rdoRuntime::RDOValue RDORTPIntResParam::getRSSIntValue( const int val ) const 
{
	if ( diap->exist ) {
		if ((val < diap->minVal) || (val > diap->maxVal)) {
//			parser->error( ("integer value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str() );
			parser->error( rdo::format("÷елочисленное значение выходит за допустимый диапазон [%d..%d]: %d", diap->minVal, diap->maxVal, val) );
		}
	}
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSDefaultValue() const 
{
	if ( !dv->exist ) {
//		parser->error( "No default value" );
		parser->error( "Ќет значени€ по-умолчанию" );
	}
	return rdoRuntime::RDOValue( dv->GetRealValue() );
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSRealValue( const double* const val ) const 
{
	if ( diap->exist ) {
		if ((*val < diap->minVal) || (*val > diap->maxVal)) {
//			parser->error(("real value " + toString(*val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
			parser->error( rdo::format("¬ещественное значение выходит за допустимый диапазон [%f..%f]: %f", diap->minVal, diap->maxVal, *val) );
		}
	}
	return rdoRuntime::RDOValue( *val );
}

rdoRuntime::RDOValue RDORTPRealResParam::getRSSIntValue( const int val ) const
{
	if ( diap->exist ) {
		if ((val < diap->minVal) || (val > diap->maxVal)) {
//			parser->error(("real value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
			parser->error( rdo::format("¬ещественное значение выходит за допустимый диапазон [%f..%f]: %d", diap->minVal, diap->maxVal, val) );
		}
	}
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSDefaultValue() const 
{
	if ( !dv->exist ) {
//		parser->error( "No default value" );
		parser->error( "Ќет значени€ по-умолчанию" );
	}
	return rdoRuntime::RDOValue( enu->findValue( dv->GetEnumValue() ) );
}

rdoRuntime::RDOValue RDORTPEnumResParam::getRSSEnumValue(const std::string *const val) const
{
	return enu->findValue(val);
}

RDORTPIntResParam::RDORTPIntResParam():
	RDORTPResParam( new RDORTPIntDefVal(0) ),
	diap( new RDORTPIntDiap() )
{
	diap->check( static_cast<RDORTPIntDefVal*>(dv) );
}

RDORTPIntResParam::RDORTPIntResParam(RDORTPIntDiap *_diap, RDORTPIntDefVal *_dv): 
	diap(_diap), 
	RDORTPResParam(_dv) 
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
		parser->error( "Ћева€ граница диапазона должна быть меньше правой" );
	}
}

void RDORTPIntDiap::check( const RDORTPIntDefVal* dv ) const
{
	if ( exist && dv->exist ) {
		if ((minVal > dv->GetIntValue()) || (maxVal < dv->GetIntValue())) {
//			parser->error(("Integer value " + toString(dv->GetIntValue()) + " out of range[" + toString(minVal) + ", " + toString(maxVal) + "]").c_str());
			parser->error( rdo::format("÷елочисленное значение выходит за допустимый диапазон [%d..%d]: %d", minVal, maxVal, dv->GetIntValue()) );
		}
	}
}

RDORTPRealResParam::RDORTPRealResParam():
	RDORTPResParam( new RDORTPRealDefVal(0) ),
	diap( new RDORTPRealDiap() )
{
	diap->check( static_cast<RDORTPRealDefVal*>(dv) );
}

RDORTPRealResParam::RDORTPRealResParam( RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPResParam( _dv ),
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
		parser->error( "Ћева€ граница диапазона должна быть меньше правой" );
	}
}

void RDORTPRealDiap::check( const RDORTPRealDefVal* dv ) const
{
	if ( exist && dv->exist ) {
		if ((minVal > dv->GetRealValue()) || (maxVal < dv->GetRealValue())) {
//			parser->error(("Integer value " + toString(dv->GetRealValue()) + " out of range[" + toString(minVal) + ", " + toString(maxVal) + "]").c_str());
			parser->error( rdo::format("¬ещественное значение выходит за допустимый диапазон [%f..%f]: %f", minVal, maxVal, dv->GetRealValue()) );
		}
	}
}

const RDORTPResParam *RDORTPIntResParam::constructSuchAs(const int defVal) const 
{
	RDORTPIntDefVal *newDV = new RDORTPIntDefVal(defVal);
	RDORTPIntResParam *rp = new RDORTPIntResParam(diap, newDV);
	return rp;
}

const RDORTPResParam *RDORTPRealResParam::constructSuchAs(const double *const defVal) const 
{
	RDORTPRealDefVal *newDV = new RDORTPRealDefVal(*defVal);
	RDORTPRealResParam *rp = new RDORTPRealResParam(diap, newDV);
	return rp;
}

const RDORTPResParam *RDORTPEnumResParam::constructSuchAs(const std::string *const defVal) const 
{
	enu->findValue(defVal);	 // if no value - Syntax exception will be thrown
	RDORTPEnumDefVal *newDV = new RDORTPEnumDefVal(defVal);
	RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, newDV);
	return rp;
}

int RDORTPDefVal::GetIntValue() const
{
//	parser->error( "Invalid default value" );
	parser->error( "Ќеверное значение по-умолчанию" );
	return 0;	// unreachable code...
}

double RDORTPDefVal::GetRealValue() const
{
//	parser->error( "Invalid default value" );
	parser->error( "Ќеверное значение по-умолчанию" );
	return 0.;	// unreachable code...
}

const std::string *RDORTPDefVal::GetEnumValue() const
{
//	parser->error( "Invalid default value" );
	parser->error( "Ќеверное значение по-умолчанию" );
	return NULL;	// unreachable code...
}

int RDORTPIntResParam::getDiapTableFunc() const 
{
	if ( !diap->exist ) {
//		parser->error("integer table function parameter must have range");
		parser->error( "ƒл€ параметра табличной функции должен быть задан допустимый диапазон" );
	}

	if ( diap->minVal != 1 ) {
//		parser->error("integer table function parameter must have minimum value = 1");
		parser->error( "ћинимальное значение диапазона должно быть 1" );
	}

	return diap->maxVal - diap->minVal + 1;
}

int RDORTPRealResParam::getDiapTableFunc() const
{
//	parser->error( "unexpected real table function parameter" );
	parser->error( "ѕараметр табличной функции может быть только целого типа" );
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
