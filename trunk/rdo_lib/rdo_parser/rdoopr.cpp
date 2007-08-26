#include "pch.h"
#include "rdoopr.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdopat.h"
#include "rdopatrtime.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int oprlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void oprerror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
RDOOPROperation::RDOOPROperation( RDOParser* _parser, const RDOParserSrcInfo& _name, const RDOParserSrcInfo& _pattern ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _name ),
	pattern( NULL ),
	currParam( 0 ),
	activity( NULL )
{
	const RDOOPROperation* opr = parser->findOperation( getName() );
	if ( opr ) {
		parser->error_push_only( src_info(), rdo::format("Операция '%s' уже существует", getName().c_str()) );
		parser->error_push_only( opr->src_info(), rdo::format("См. первое определение") );
		parser->error_push_done();
	}
	pattern = parser->findPattern( _pattern.src_text() );
	if ( !pattern ) {
		parser->error( _pattern, rdo::format("Неизвестный образец: %s",_pattern.src_text().c_str()) );
//		parser->error( "Undefined pattern name: " + _pattern.src_text() );
	}
	activity = pattern->patRuntime->createActivity( getName() );
	parser->insertOPROperation( this );
}

void RDOOPROperation::addParam( int intParam, const YYLTYPE& param_pos )
{
	if ( pattern->params.size() <= currParam ) {
		parser->error( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
//		parser->error( "Too much parameters for pattern : " + pattern->getName() + " in operation: " + getName() );
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case RDORTPParamType::pt_enum: parser->error( param_pos, rdo::format("Ожидается параметр перечислимого типа: %s", param->getType()->src_text().c_str()) ); break;
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue( intParam, param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = %d", pattern->getName().c_str(), param->getName().c_str(), intParam)) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDOOPROperation::addParam( double realParam, const YYLTYPE& param_pos )
{
	if ( pattern->params.size() <= currParam ) {
		parser->error( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case RDORTPParamType::pt_int : parser->error( param_pos, rdo::format("Ожидается параметр целого типа: %s", param->getType()->src_text().c_str()) ); break;
		case RDORTPParamType::pt_enum: parser->error( param_pos, rdo::format("Ожидается параметр перечислимого типа: %s", param->getType()->src_text().c_str()) ); break;
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue( realParam, param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = %f", pattern->getName().c_str(), param->getName().c_str(), realParam)) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDOOPROperation::addParam( const std::string& stringParam, const YYLTYPE& param_pos )
{
	if ( pattern->params.size() <= currParam ) {
		parser->error( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s', а если это имя новой операции, то оно должно заканчиваться двоеточием", pattern->getName().c_str(), getName().c_str()) );
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case RDORTPParamType::pt_int : parser->error( param_pos, rdo::format("Ожидается параметр целого типа: %s", param->getType()->src_text().c_str()) ); break;
		case RDORTPParamType::pt_real: parser->error( param_pos, rdo::format("Ожидается параметр вещественного типа: %s", param->getType()->src_text().c_str()) ); break;
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue( stringParam, param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = %s", pattern->getName().c_str(), param->getName().c_str(), stringParam.c_str())) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDOOPROperation::addParam( const YYLTYPE& param_pos )
{
	if ( pattern->params.size() <= currParam ) {
		parser->error( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
	}

	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	if ( !param->getType()->dv->isExist() ) {
		parser->error_push_only( param_pos, rdo::format("Нет значения по-умолчанию для параметра '%s'", param->src_text().c_str()) );
		parser->error_push_only( param->src_info(), rdo::format("См. параметр '%s', тип '%s'", param->src_text().c_str(), param->getType()->src_text().c_str()) );
		parser->error_push_done();
	}
	rdoRuntime::RDOValue val = param->getType()->getParamDefaultValue( param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = *", pattern->getName().c_str(), param->getName().c_str())) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDOOPROperation::addHotKey( const std::string& hotKey, const YYLTYPE& pattern_pos )
{
	if ( dynamic_cast<rdoRuntime::RDOActivityKeyboardRuntime*>(activity) ) {
		if ( !activity->addHotKey( hotKey ) ) {
			parser->error( pattern_pos, rdo::format("Неизвестная клавиша: %s", hotKey.c_str()) );
		}
	} else {
		parser->error( pattern_pos, "Образец не является клавиатурной операцией" );
	}
}

void RDOOPROperation::endOfDefinition( const YYLTYPE& opr_pos )
{
	if ( pattern->params.size() > currParam ) {
		RDOFUNFunctionParam* param = pattern->params.at( currParam );
		if ( pattern->params.size() == currParam + 1 ) {
			parser->error( opr_pos, rdo::format("Указаны не все параметра образца, ожидаемый параметр '%s' имеет тип '%s'", param->getName().c_str(), param->getType()->src_text().c_str()) );
		} else {
			parser->error_push_only( opr_pos, rdo::format("Указаны не все параметра образца:") );
			for ( int i = currParam; i < pattern->params.size(); i++ ) {
				param = pattern->params.at( i );
				parser->error_push_only( param->src_info(), rdo::format("Ожидаемый параметр '%s' имеет тип '%s'", param->getName().c_str(), param->getType()->src_text().c_str()) );
			}
			parser->error_push_done();
		}
//		parser->error("Too few parameters for pattern : " + pattern->getName() + " in operation: " + getName());
	}
}

} // namespace rdoParse 
